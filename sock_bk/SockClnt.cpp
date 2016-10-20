#include "SockClnt.h"
#include "ABMException.h"
#include "LogV2.h"
#include "TcpSock.h"
#include "abmobject.h"
#include "SockSrvConf.h"
#include "MessageQueue.h"
#include "dccapi.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <vector>
using namespace std;

#define SELECT_MAX   5 
#define OUTTIME_MAX  3
int iDccDWR =1;

void disp_buf(char *buf, int size)
{
  int i;
  printf("buf [%d] start:\n", size);
  for( i = 0 ; i < size; i ++ )
  {
        if( i%16 == 0 )
                printf("\n");
        printf("%02X ", buf[i]&0xff);
  }
  printf("\nbuf end\n");
}

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
    }

    ~PthResource()
    {
        freeRsc();
    }

    int mallocSock() {
        if ((m_poSock=new ConnSock) == NULL) {
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
    ConnSock   *m_poSock;       //��·SOCKET
    SockSrvConf   m_oConf;       //������Ϣ
    int m_iState;                //��Դ״̬
};

static pthread_attr_t g_oPthAttr = 0;   //�߳�����
static vector<SockSrvConf> g_vSockConf;

//dcc api��֧�ֶ��߳� ��д��ȫ��, ���̻߳���BUG
static DccApi *g_poDcc = NULL;
static char g_sCerBuf[1024], g_sDwrBuf[1024];
static int g_iCerLen, g_iDwrLen;

static void *sndDCCPth(void *arg);   

static void *createSockPth(void *arg)
{
    int iRet, iTms;
    PthResource *pRsc = (PthResource *)arg;
    if (pRsc->m_poSock->connect(pRsc->m_oConf.m_sLocIP, \
                                pRsc->m_oConf.m_iLocPort, \
                                pRsc->m_oConf.m_sRemIP, \
                                pRsc->m_oConf.m_iRemPort) != 0) {
        __DEBUG_LOG4_(0, "::createSockPth localip=%s localport=%d,remoteip=%s remoteport=%d connect failed!", \
           pRsc->m_oConf.m_sLocIP, pRsc->m_oConf.m_iLocPort, pRsc->m_oConf.m_sRemIP, pRsc->m_oConf.m_iRemPort);
        exit(0);
    }
    
    //����CER��
    if (pRsc->m_poSock->sendCer((const void *)g_sCerBuf, g_iCerLen) == -1) {
        __DEBUG_LOG1_(0, "::createSockPth socket=%d ����CER��ʧ��!", pRsc->m_poSock->getSock());
        exit(0);    
    }
    if ((iRet=pthread_create(&pRsc->m_iSndPthID, &g_oPthAttr, sndDCCPth, (void *)pRsc)) != 0) {
        __DEBUG_LOG1_(0, "::createSockPth pthread_create failed! iRet= %d", iRet);
        exit(0);
    }
    
    //���������DCC��������Ϣ����
    ABMCCA *pCCA = (ABMCCA *)pRsc->m_poRcvBuf;
    pCCA->m_lType = pRsc->m_oConf.m_iLinkID;
    while (true) {
        if ((iRet=pRsc->m_poSock->selectRD(SELECT_MAX)) == 0) {
            if (iTms++ >= OUTTIME_MAX) {
                __DEBUG_LOG0_(0, "createSockPth ����������˻ظ���ʱ,socket�ؽ�");
                goto __Rebuilt;    
            }
            if(iDccDWR>30){
            if (pRsc->m_poSock->sendCer((const void *)g_sDwrBuf, g_iDwrLen) == -1) {
                __DEBUG_LOG0_(0, "createSockPth ����������ʧ��, socket �ؽ�");
                goto __Rebuilt;        
            } 
            iDccDWR=1;
          } 
           iDccDWR=iDccDWR+1;     
        }
        else if (iRet == -1) {
            __DEBUG_LOG0_(0, "createSockPth SELECT FAILED!");
            exit(0);
        }
        iTms = 0;
        if ((iRet=pRsc->m_poSock->recvDCC((void *)pCCA->m_sDccBuf, sizeof(ABMCCA)-8)) == -1) {
            __DEBUG_LOG1_(0, "::createSockPth socket=%d pRsc->m_poSock->recvDCC failed!", pRsc->m_poSock->getSock());
            goto __Rebuilt;
        }

        if (pRsc->m_poSndMQ->Send((void *)pCCA, iRet+8, true, false) <= 0) {
            __DEBUG_LOG3_(0, "createSockPth socket=%d ������Ϣ����key=%d����! errno=%d", \
                     pRsc->m_poSock->getSock(), pRsc->m_oConf.m_iSndMQ, errno);
            exit(0);
        }
        continue;
        
        __Rebuilt:
        if (pRsc->m_poSock->rebuiltSock() == -1) {
            __DEBUG_LOG0_(0, "createSockPth pRsc->m_poSock->rebuiltSock failed!");
            exit(0);    
        } 
        //����CER��
        if (pRsc->m_poSock->sendCer((const void *)g_sCerBuf, g_iCerLen) == -1) {
            __DEBUG_LOG1_(0, "::createSockPth socket=%d ����CER��ʧ��!", pRsc->m_poSock->getSock());
            goto __Rebuilt;    
        }   
    }
    return NULL;
}

//����Ϣ���н���DCC��������
static void *sndDCCPth(void *arg)
{
    int iRet;
    PthResource *pRsc = (PthResource *)arg;
    ABMCCA *pCCA = pRsc->m_poSndBuf;
    int iType = pRsc->m_oConf.m_iLinkID;
    char sBuff[1024];

    while (true) {
        if ((iRet=pRsc->m_poRcvMQ->Receive((void *)pCCA, sizeof(ABMCCA), iType, true)) <= 0) {
            __DEBUG_LOG3_(0, "::sndDCCPth socket=%d ������Ϣ���� key=%d ����ʧ��! errno=%d", \
                pRsc->m_poSock->getSock(), pRsc->m_oConf.m_iRcvMQ, errno);
            exit(0);
        }
        memcpy(sBuff, pCCA->m_sDccBuf, iRet -8);
        disp_buf(sBuff, iRet -8);
        if (pRsc->m_poSock->sendDCC((void *)pCCA->m_sDccBuf, iRet-8) == -1) {
            __DEBUG_LOG1_(0, "::sndDCCPth socket=%d ����DCC��ʧ��!", pRsc->m_poSock->getSock());
            exit(0);
        }
    }
    return NULL;
}

SockClnt::SockClnt()
{
    
}

SockClnt::~SockClnt()
{
    
}        

int SockClnt::init(ABMException &oExp)
{
    int iRet;
    if (g_iProcID == -1) {
        ADD_EXCEPT0(oExp, "SockClnt::init �������� -p ����û������!");
        return -1;
    }

    if ((iRet=pthread_attr_init(&g_oPthAttr)) != 0) {
        ADD_EXCEPT1(oExp, "SockClnt::init pthread_attr_init failed! iRet=%d", iRet);
        return -1;
    }
    if ((iRet=pthread_attr_setdetachstate(&g_oPthAttr, PTHREAD_CREATE_DETACHED)) !=0) {
        ADD_EXCEPT1(oExp, "SockClnt::init pthread_attr_setdetachstate failed! iRet=%d", iRet);
        return -1;
    }
    
    if ((g_poDcc=new DccApi) == NULL) {
        ADD_EXCEPT0(oExp, "SockClnt::init g_poDcc malloc failed!");
        return -1;
    }
    if (g_poDcc->init() != 0) {
        ADD_EXCEPT0(oExp, "SockClnt::init g_poDcc init failed!");
        return -1;
    }

    g_iCerLen = g_poDcc->getCer(g_sCerBuf, sizeof(g_sCerBuf));
    g_iDwrLen = g_poDcc->getDwr(g_sDwrBuf, sizeof(g_sDwrBuf));
    m_mLink.clear();

    return 0;
}

//���̶߳�ȡ���������̺߳�, �ȴ�SIGUP�ź� ����
int SockClnt::run()
{
    int iRet;
    PthResource *pRsc;
    ABMException oExp;
    MapIndex oIndex;
    vector<SockSrvConf>::iterator itr;
    map<MapIndex, PthResource*>::iterator itrmap;

    __Refresh:
    g_vSockConf.clear();
    if (ReadSockConf::getConnectConf(g_iProcID, g_vSockConf) != 0) {
        __DEBUG_LOG0_(0, "SockClnt::run ReadSockConf::getConnectConf failed!");
        exit(0);
    }
    for (itr=g_vSockConf.begin(); itr!=g_vSockConf.end(); ++itr) {
        oIndex.m_iPort = itr->m_iLocPort;
        snprintf(oIndex.m_sIP, sizeof(oIndex.m_sIP), "%s", itr->m_sLocIP);
        itrmap = m_mLink.find(oIndex);
        if (itrmap != m_mLink.end())
            continue;
        //����
        if ((pRsc=new PthResource) == NULL) {
            __DEBUG_LOG0_(0, "SockClnt::run pRsc malloc failed!");
            exit(0);
        }
        if (pRsc->mallocSock() != 0) {
            __DEBUG_LOG0_(0, "SockClnt::run pRsc->mallocSock failed!");
            exit(0);
        }
        if (pRsc->mallocRsc(oExp, itr->m_iRcvMQ, itr->m_iSndMQ) != 0) {
            __DEBUG_LOG0_(0, "SockClnt::run pRsc->mallocRsc failed!");
            exit(0);
        }
        //����socket��Ϣ��pRsc
        memcpy((void *)&(pRsc->m_oConf), (const void *)&itr->m_iLinkID, sizeof(pRsc->m_oConf));
        if ((iRet=pthread_create(&pRsc->m_iRcvPthID, &g_oPthAttr, createSockPth, (void *)pRsc)) != 0) {
            __DEBUG_LOG1_(0, "SockClnt::run pthread_create failed! iRet= %d", iRet);
            exit(0);
        }
        m_mLink.insert(pair<MapIndex, PthResource*>(oIndex, pRsc));
    }
    //������˯��60��ˢ�¡������ĳ��źŻ���ˢ��
    sleep(60);
    goto  __Refresh;

    return 0;
}
