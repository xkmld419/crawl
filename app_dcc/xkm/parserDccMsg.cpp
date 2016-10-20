#include "parserDccMsg.h"



static MessageQueue *g_poRcvMQ = NULL;         //������Ϣ����
static MessageQueue *g_poSndMQ = NULL;         //������Ϣ���� 
static MessageQueue *g_poSndPeerMQ = NULL;     //������Ϣ����,����sock������

static ABMCCR g_oSndBuf;                  //���ͻ�����������ҵ�������
static StruMqMsg   g_sRecvBuf;            //���ջ����������մ�socket���͹�������Ϣ
static StruMqMsg   g_sPeerBuf;            //���ͻ������������������ҵ������̣�ֱ�ӷ���socketʱ�õ�
//static struDccHead goDccHead;           //dcc��Ϣ�Ĺ���ͷ����Ϣ

int ParserMsgMgr::init(ABMException &oExp)
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
    //����sock����������Ϣ����
    if (struConf.m_iSndPeerKey <= 0) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init ����������Ҫ����mqsnd_peer.");
        return -1;
    }
    if ((g_poSndPeerMQ=new MessageQueue(struConf.m_iSndPeerKey)) == NULL) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init g_poSndPeerMQ malloc failed!");
        return -1;
    }
	if (g_poSndPeerMQ->open(oExp, true, true) != 0) {
        ADD_EXCEPT1(oExp, "ParserMsgMgr::init g_poSndPeerMQ->open ��Ϣ��������ʧ��!", struConf.m_iSndKey);
        return -1;
    }
    
    m_poDcc = new DccMsgParser(struConf.m_sDictFile);
    if (NULL == m_poDcc) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init m_poDcc malloc failed!");
        return -1;
    }
    
    return 0;
}

int ParserMsgMgr::run()
{
    int iRecvLen, iRet, iSize;
    string sTemp;
    
    while (!RECEIVE_STOP()) {
        if ((iRecvLen=g_poRcvMQ->Receive((void *)&g_sRecvBuf, MSG_SIZE, 0, true)) < 0) {
			if (iRecvLen == -2) {
				continue;	
			}	
			__DEBUG_LOG1_(0, "Ӧ�ó������Ϣ���н���ʧ��, �����˳�! errno=%d", errno);
			return -1;
		}
		
		iRecvLen -= sizeof(long);
		if (iRecvLen <= 0) {
		    __DEBUG_LOG1_(0, "���յ���Ϣ����������Ϣ���ȣ�%d.", iRecvLen);
		    return -1;
		}
		
#ifdef _DEBUG_DCC_
        cout<<"���յ���ԭʼ��Ϣ����"<<endl;
        disp_buf((char *)&g_sRecvBuf, iRecvLen + sizeof(long));
#endif		
		
		//���
		iRet = m_poDcc->parseRawToApp(g_sRecvBuf.m_sMsg, iRecvLen);
		if (iRet != 0) {
		    __DEBUG_LOG1_(0, "��Ϣ�������������:%d.", -iRet);
		    continue;
		}
		
#ifdef _DEBUG_DCC_
        cout<<"���յ���DCC��Ϣ����"<<endl;
        m_poDcc->getAllAvp();
#endif	
		
		if ( (iRet = m_poDcc->getDiameterCommandCodeInfo()) == COMMAND_CODE_CE ) {         //��������
		    /*  ��ʱ������
		    iRet = packECA(m_poDcc, g_sPeerBuf, &iSize);
		    if (iRet < 0 ) {
		        __DEBUG_LOG0_(0, "����CEA��Ϣ����.");
		        continue;
		    }
		    
		    if (g_poSndPeerMQ->Send((void *)&g_sPeerBuf, iSize) < 0) {
			    __DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			    return -1;	
		    }
		    */ 
		} else if (iRet == COMMAND_CODE_CC) {                                              //ҵ����Ϣ
		    //��ȡ������Ϣ�Ĺ������֣��������Ϣ����
		    g_oSndBuf.clear();
		    iRet = getCCRHead(m_poDcc, &g_oSndBuf);
		    if (iRet < 0 ) {
		        __DEBUG_LOG0_(0, "��ȡdcc��Ϣͷ����Ϣ����.");
		        continue;
		    }
		    
		    switch (iRet)
		    {
		        case ABMCMD_QRY_SRV_INFOR:       //3.4.1.1 ���ϲ�ѯ������
		            iRet = parserInfoQryR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserInfoQryR: �������.");
		                continue;
		            }
		            g_oSndBuf.m_iCmdID = ABMCMD_QRY_SRV_INFOR;
		            break;
		        case ABMPAYCMD_INFOR:            //3.4.2.1 ֧��
		            iRet = parserChargeR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserChargeR���������.");
		                continue;
		            }
		            g_oSndBuf.m_iCmdID = ABMPAYCMD_INFOR;
		            break;
					
				case ABMCMD_ALLOCATE_BALANCE:            //3.4.5.1 ��������
		            iRet = AllocateBalanceSvcR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserChargeR���������.");
		                continue;
		            }
		            g_oSndBuf.m_iCmdID = ABMCMD_ALLOCATE_BALANCE;
		            break;
					
				case ABMCMD_QRY_BIL_INFOR:            //3.4.6.16 �����˵���ѯ
		            iRet = RechargeBillQueryR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserChargeR���������.");
		                continue;
		            }
		            g_oSndBuf.m_iCmdID = ABMCMD_QRY_BIL_INFOR;
		            break;
					
				case ABMCMD_QRY_PAY_INFOR:            //3.4.6.15 �����˵���ѯ
		            iRet = QueryPayLogByAccoutR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserChargeR���������.");
		                continue;
		            }
		            g_oSndBuf.m_iCmdID = ABMCMD_QRY_PAY_INFOR;
		            break;
					
				case ABMCMD_REVERSE_DEDUCT_BALANCE:            //3.4.5.5 
		            iRet = ReqReverseDeductSvcR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserChargeR���������.");
		                continue;
		            }
		            g_oSndBuf.m_iCmdID = ABMCMD_REVERSE_DEDUCT_BALANCE;
		            break;
					
				case ABMCMD_QRY_REC_INFOR:            //3.4.6.14 
		            iRet = RechargeRecQueryR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserChargeR���������.");
		                continue;
		            }
		            g_oSndBuf.m_iCmdID = ABMCMD_QRY_REC_INFOR;
		            break;
					
		        default:
		            __DEBUG_LOG1_(0, "δ֪����������[%d].", iRet);
		            continue;
		            break;
            }
		    
            //������Ϣ�����ö�
		    if (g_poSndMQ->Send((void *)&g_oSndBuf, g_oSndBuf.m_iSize) < 0) {
			    __DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			    return -1;	
		    } 
		    
		} //ҵ����Ϣ
	}
	
	__DEBUG_LOG0_(0, "�����յ���ȫ�˳��ź�, ��ȫ�˳�");
	
	return 0;
}

int ParserMsgMgr::destroy()
{
	//�ͷ�ҵ������
    #define __FREE_PTR(p) \
        if (p != NULL) { \
            delete p; p = NULL; }
    
    __FREE_PTR(g_poSndMQ);
    __FREE_PTR(g_poRcvMQ);
    __FREE_PTR(g_poSndPeerMQ);
	
	return 0;
}


