#include "packDccMsg.h"

static MessageQueue *g_poRcvMQ = NULL;         //������Ϣ����
static MessageQueue *g_poSndMQ = NULL;         //������Ϣ���� 

static ABMCCA    g_oRcvBuf;                    //���ͻ�����������ҵ�������
static StruMqMsg g_sSndBuf;                    //���ͻ�����������socket
//static struDccHead goDccHead;                //dcc��Ϣ�Ĺ���ͷ����Ϣ


int PackMsgMgr::init(ABMException &oExp)
{
    char *p;
    
    if (g_iProcID == -1) {
        ADD_EXCEPT0(oExp, "���������б������� -p ��!");
        return -1;  
    }
    
    //��ȡ�����ļ�
    DccConfStru struConf;
    if (ABMConf::getDccConf(g_iProcID, struConf, oExp) != 0) {
        ADD_EXCEPT1(oExp, "ParserMsgMgr::init ʧ��, ����ParserMsgMgr -p %d ��abm_app_cfg.ini������!", g_iProcID);
        return -1;      
    }
    
    //��Ϣ���г�ʼ��
    if ((g_poRcvMQ=new MessageQueue(struConf.m_iRcvKey)) == NULL) {
        ADD_EXCEPT0(oExp, "ABMMgr::init g_poRcvMQ malloc failed!");
        return -1;
    }
    if (g_poRcvMQ->open(oExp, true, true) != 0) {
        ADD_EXCEPT1(oExp, "ParserMsgMgr::init g_poRcvMQ->open ��Ϣ��������ʧ��!", struConf.m_iRcvKey);
        return -1;
    }
    if ((g_poSndMQ=new MessageQueue(struConf.m_iSndKey)) == NULL) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init g_poSndMQ malloc failed!");
        return -1;
    }
    if (g_poSndMQ->open(oExp, true, true) != 0) {
        ADD_EXCEPT1(oExp, "ParserMsgMgr::init g_poSndMQ->open ��Ϣ��������ʧ��!", struConf.m_iSndKey);
        return -1;
    }
    
    m_poDcc = new DccMsgParser(struConf.m_sDictFile);
    if (NULL == m_poDcc) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init m_poDcc malloc failed!");
        return -1;
    }
    
    return 0;
}

int PackMsgMgr::run()
{
    int iRet;
    int iMsgLen;
    
    while (!RECEIVE_STOP()) {
        if ((iRet=g_poRcvMQ->Receive((void *)&g_oRcvBuf, sizeof(g_oRcvBuf), 0, true)) < 0) {
            if (iRet == -2) {
                continue;   
            }   
            __DEBUG_LOG1_(0, "Ӧ�ó������Ϣ���н���ʧ��, �����˳�! errno=%d", errno);
            return -1;
        }
        
        if (setCCAHead(m_poDcc, &g_oRcvBuf) != 0) {
            __DEBUG_LOG0_(0, "�����Ϣ�������ֳ���ʧ��! ");
            continue;
        }
        
        switch (g_oRcvBuf.m_iCmdID)
        {
            case ABMCMD_QRY_SRV_INFOR:                  //3.4.1.1 ���ϲ�ѯ������
                iRet = packInfoQryA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packInfoQryA: DCC�������.");
		            continue;
		        }
                break;
            case ABMPAYCMD_INFOR:                       //3.4.2.1 ֧��
                iRet = packChargeA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packInfoQryA: DCC�������.");
		            continue;
		        }
                break;
				
			case ABMCMD_ALLOCATE_BALANCE:                       //3.4.5.1 ��������
                iRet = AllocateBalanceSvcA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packInfoQryA: DCC�������.");
		            continue;
		        }
                break;
				
			case ABMCMD_QRY_BIL_INFOR:                       //3.4.6.16 �����˵���ѯ
                iRet = RechargeBillQueryA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packInfoQryA: DCC�������.");
		            continue;
		        }
                break;
			
			case ABMCMD_QRY_PAY_INFOR:                       //3.4.6.15
                iRet = QueryPayLogByAccoutA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packInfoQryA: DCC�������.");
		            continue;
		        }
                break;
			
			case ABMCMD_REVERSE_DEDUCT_BALANCE:                       //3.4.5.5
                iRet = ReqReverseDeductSvcA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packInfoQryA: DCC�������.");
		            continue;
		        }
                break;
				
			case ABMCMD_QRY_REC_INFOR:                       //3.4.6.14
                iRet = RechargeRecQueryA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packInfoQryA: DCC�������.");
		            continue;
		        }
                break;
				
				
            default:
                break;
        }
        
#ifdef _DEBUG_DCC_
        cout<<"CCA���ǰavp��"<<endl;
        m_poDcc->getAllAvp();
#endif        
        iMsgLen = m_poDcc->parseAppToRaw(g_sSndBuf.m_sMsg, MSG_SIZE);
        if (iMsgLen <= 0) {
            __DEBUG_LOG0_(0, "���ʧ��! ");
            continue;
        }
        
#ifdef _DEBUG_DCC_
        cout<<"CCA��Ϣ����ǰԭʼ��Ϣ����"<<endl;
        disp_buf((char *)&g_sSndBuf, iMsgLen + sizeof(long));
#endif          
        if (g_poSndMQ->Send((void *)&g_sSndBuf, iMsgLen+sizeof(long)) < 0) {
	        __DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			return -1;	
		} 
    }
    
    return 0;
}

int PackMsgMgr::destroy()
{
    //�ͷ�ҵ������
    #define __FREE_PTR(p) \
        if (p != NULL) { \
            delete p; p = NULL; }
    
    __FREE_PTR(g_poSndMQ);
    __FREE_PTR(g_poRcvMQ);
    
    return 0;
}
