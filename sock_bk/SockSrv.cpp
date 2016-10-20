#include "SockSrv.h"
#include "ABMException.h"
#include "TcpSock.h"
#include "LogV2.h"
#include "abmobject.h"
#include "SockSrvConf.h"
#include "MessageQueue.h"
#include <pthread.h>
using namespace std;

//�߳���Դ״̬
#define RSC_INIT   0          //��ʼ
#define RSC_ENABLE 0          //����
#define RSC_RCV_UNABLE 1      //���ղ�����
#define RSC_SND_UNABLE 2      //���Ͳ�����
#define RSC_CLEAR  3          //������

//�߳���Դ
class PthResource
{
public:
    
    PthResource() {
        m_iRcvPthID = -1;
        m_iSndPthID = -1;
        m_poRcvBuf = NULL;
        m_poSndBuf = NULL;
        m_poRcvMQ = NULL;
        m_poSndMQ = NULL;
        m_poSock = NULL;  
        m_iState = RSC_INIT;  
    }
    
    ~PthResource()
    {
        freeRsc();    
    }
    
    bool stop() {
        return m_iState != RSC_ENABLE;    
    }
    
    int mallocSock() {
        if ((m_poSock=new AcceptSock) == NULL) {
            __DEBUG_LOG0_(0, "PthResource::mallocSock m_poSock malloc failed!");
            return -1;    
        }    
        return 0;
    }
    
    int mallocRsc(ABMException &oExp, int iRcvKey, int iSndKey) {
        if ((m_poRcvMQ=new MessageQueue(iRcvKey)) == NULL) {
            __DEBUG_LOG0_(0, "PthResource::mallocRsc m_poRcvMQ malloc failed!");
            return -1;    
        }    
        if (m_poRcvMQ->open(oExp, true, true) != 0) {
            LogV2::logExcep(oExp);
            return -1;        
        }
        if ((m_poSndMQ=new MessageQueue(iSndKey)) == NULL) {
            __DEBUG_LOG0_(0, "PthResource::mallocRsc m_poSndMQ malloc failed!");
            return -1;    
        }    
        if (m_poSndMQ->open(oExp, true, true) != 0) {
            LogV2::logExcep(oExp);
            return -1;        
        }
        
        if ((m_poRcvBuf=new ABMCCR) == NULL) {
            __DEBUG_LOG0_(0, "PthResource::mallocRsc m_poRcvBuf malloc failed!");
            return -1;    
        }
        
        if ((m_poSndBuf=new ABMCCA) == NULL) {
            __DEBUG_LOG0_(0, "PthResource::mallocRsc m_poSndBuf malloc failed!");
            return -1;    
        } 
        m_iState = RSC_ENABLE; 

        return 0;
    }
    
    void freeRsc()
    {
    #define __FREE_PTR(x) if (x != NULL) { \
                            delete x; x = NULL; }
        __FREE_PTR(m_poRcvBuf);
        __FREE_PTR(m_poSndBuf);
        __FREE_PTR(m_poRcvMQ);
        __FREE_PTR(m_poSndMQ);
        __FREE_PTR(m_poSock);                            
    }
    
    pthread_t m_iRcvPthID;       //�����߳�
    pthread_t m_iSndPthID;       //�����߳�
    ABMCCR *m_poRcvBuf;          //���ջ��� 
    ABMCCA *m_poSndBuf;          //���ͻ���
    MessageQueue *m_poRcvMQ;     //������Ϣ����
    MessageQueue *m_poSndMQ;     //������Ϣ����
    AcceptSock   *m_poSock;      //��·SOCKET 
    SockSrvConf   m_oConf;       //������Ϣ  
    int m_iState;                //��Դ״̬ 
};

static pthread_attr_t g_oPthAttr = 0;   //�߳�����

static void pthExit(int iSig)
{
    
}

//DCC�����߳�
static void *rcvDCCPth(void *arg)
{
    int iRet;
    PthResource *pRsc = (PthResource *)arg;   
    ABMCCR *pCCR = pRsc->m_poRcvBuf;
    pCCR->m_lType = pRsc->m_oConf.m_iLinkID;
    __DEBUG_LOG2_(0, "::rcvDCCPth socket=%d �Ľ����߳�=%d �ɹ�ע��!", pRsc->m_poSock->getSock(), \
                                                        pRsc->m_iRcvPthID);
    
    while (!pRsc->stop()) {
        if ((iRet=pRsc->m_poSock->recvDCC((void *)pCCR->m_sDccBuf, sizeof(ABMCCR)-8)) < 0)
            break;
        if ((iRet=pRsc->m_poSndMQ->Send((void *)pCCR, iRet+8, true, false)) <= 0) {
            if (iRet == -2)
                continue;
            __DEBUG_LOG2_(0, "::rcvDCCPth socket=%d ������Ϣ���в���ʧ��! errno=%d", pRsc->m_poSock->getSock(), errno);
            exit(0);    
        }          
    }
    
    //֪ͨ�����߳��˳�
    pRsc->m_iState |= RSC_RCV_UNABLE;
    pthread_kill(pRsc->m_iSndPthID, SIGUSR1);
    __DEBUG_LOG2_(0, "::rcvDCCPth socket=%d �Ľ����߳�=%d ע��!", pRsc->m_poSock->getSock(), \
                                                        pRsc->m_iRcvPthID);
    pthread_exit(0);
    return NULL;
}

//DCC�����߳�
static void *sndDCCPth(void *arg)
{
    int iRet;
    PthResource *pRsc = (PthResource *)arg;
    ABMCCA *pCCA = pRsc->m_poSndBuf;
    int iType = pRsc->m_oConf.m_iLinkID;
    __DEBUG_LOG2_(0, "::sndDCCPth socket=%d �ķ����߳�=%d �ɹ�ע��!", pRsc->m_poSock->getSock(), \
                                                        pRsc->m_iSndPthID);
    while (!pRsc->stop()) {
        if ((iRet=pRsc->m_poRcvMQ->Receive((void *)pCCA, sizeof(ABMCCA), iType, true)) <= 0) {
            if (iRet == -2)
                continue;
            __DEBUG_LOG2_(0, "::sndDCCPth socket=%d ������Ϣ���в���ʧ��! errno=%d", pRsc->m_poSock->getSock(), errno);
            exit(0);        
        } 
        __DEBUG_LOG2_(0, "::sndDCCPth socket=%d ������Ϣ���д�С! sock���ʹ�С%d", iRet, iRet-8);
        if (pRsc->m_poSock->sendDCC((void *)pCCA->m_sDccBuf, iRet-8) == -1)
            break;            
    }
    pRsc->m_iState |= RSC_SND_UNABLE;
    if (pRsc->m_iState != RSC_CLEAR)
        pthread_kill(pRsc->m_iRcvPthID, SIGUSR1);
    __DEBUG_LOG2_(0, "::sndDCCPth socket=%d �ķ����߳�=%d ע��!", pRsc->m_poSock->getSock(), \
                                                        pRsc->m_iRcvPthID);                                                        
    pthread_exit(0); 
    return NULL;
}

SockSrv::SockSrv()
{
    m_poLsn = NULL;
    m_vRsc.clear();
}

SockSrv::~SockSrv()
{
    if (m_poLsn != NULL) 
        delete m_poLsn;
}

int SockSrv::init(ABMException &oExp)
{
    int iRet;
    if (g_iProcID == -1) {
        ADD_EXCEPT0(oExp, "SockSrv::init �������� -p ����û������!");
        return -1;
    }
    
    if ((iRet=pthread_attr_init(&g_oPthAttr)) != 0) {
        ADD_EXCEPT1(oExp, "SockSrv::init pthread_attr_init failed! iRet=%d", iRet);
        return -1;
    }
    if ((iRet=pthread_attr_setdetachstate(&g_oPthAttr, PTHREAD_CREATE_DETACHED)) !=0) {
        ADD_EXCEPT1(oExp, "SockSrv::init pthread_attr_setdetachstate failed! iRet=%d", iRet);
        return -1;
    }
    if (SigHandlerSet(SIGUSR1, pthExit) != 0)
        return -1;
    
    return 0;    
}


/*
*   ��������
*/
int SockSrv::run()
{
    
    int iRet;
    SockSrvConf oConf;
    PthResource *pRsc;
    ABMException oExp;
    vector<PthResource *>::iterator itr;
    if ((m_poLsn=new ListenSock)== NULL) {
        __DEBUG_LOG0_(0, "SockSrv::init m_poLsn malloc failed!");
        return -1;
    }
    if (ReadSockConf::getListenConf(g_iProcID, oConf) != 0) {
       __DEBUG_LOG1_(0, "SockSrv::init g_iProcID=%d ��ȡ����������Ϣʧ��!", g_iProcID);
       return -1;
    }
    if (m_poLsn->listen(oConf.m_sLocIP, oConf.m_iLocPort) != 0) {
        __DEBUG_LOG0_(0, "SockSrv::run m_poLsn->listen failed!");
        return -1;    
    } 
    
    while (true) {
        if ((pRsc=new PthResource) == NULL) {
            __DEBUG_LOG0_(0, "SockSrv::run pRsc malloc failed!");
            exit(0);    
        }
        if (pRsc->mallocSock() != 0)
            exit(0);
        if (m_poLsn->accept(*(pRsc->m_poSock)) != 0)
            exit(0);
        if (ReadSockConf::getAcceptConf(g_iProcID, \
                                        pRsc->m_poSock->getRemIP(), \
                                        pRsc->m_poSock->getRemPort(), \
                                        pRsc->m_oConf) != 0) {
            __DEBUG_LOG3_(0, "SockSrv::run Illegal IP=%s Port=%d , ����˾ܾ���½, �����ر�socket=%d!", \
                pRsc->m_poSock->getRemIP(),  pRsc->m_poSock->getRemPort(), pRsc->m_poSock->getSock());
            pRsc->m_poSock->close();
            delete pRsc;
            continue;                                        
        } 
        __DEBUG_LOG3_(0,"SockSrv::run Client IP = %s, Port = %d Socket=%d ��½�ɹ�!", \
            pRsc->m_poSock->getRemIP(), pRsc->m_poSock->getRemPort(), pRsc->m_poSock->getSock());
        
        //������Դ
        if (pRsc->mallocRsc(oExp, pRsc->m_oConf.m_iRcvMQ, pRsc->m_oConf.m_iSndMQ) != 0)
            exit(0);
        else
            m_vRsc.push_back(pRsc);
            
        //���������߳�
        if ((iRet=pthread_create(&pRsc->m_iRcvPthID, &g_oPthAttr, rcvDCCPth, (void *)pRsc)) != 0) {
            __DEBUG_LOG1_(0, "SockSrv::run pthread_create failed! iRet= %d", iRet);
            exit(0);    
        }
        
        //���������߳�
        if ((iRet=pthread_create(&pRsc->m_iSndPthID, &g_oPthAttr, sndDCCPth, (void *)pRsc)) != 0) {
            __DEBUG_LOG1_(0, "SockSrv::run pthread_create failed! iRet= %d", iRet);
            exit(0);    
        }
        
        //����Ƿ��пɻ�����Դ
        for (itr=m_vRsc.begin(); itr!=m_vRsc.end(); ) {
            if ((*itr)->m_iState == RSC_CLEAR) {
                delete (*itr);
                itr = m_vRsc.erase(itr);    
            }
            else
                ++itr;    
        }
    }            
            
    return 0;
}
