#include "packDccMsg.h"

static MessageQueue *g_poRcvMQ = NULL;         //������Ϣ����
static MessageQueue *g_poSndMQ = NULL;         //������Ϣ���� 

static ABMCCA    g_oRcvBuf;                    //���ͻ�����������ҵ�������
static StruMqMsg g_sSndBuf;                    //���ͻ�����������socket
//static struDccHead goDccHead;                //dcc��Ϣ�Ĺ���ͷ����Ϣ
extern NodeConfStru g_oNodeConf;               //ҵ��ڵ��һЩ�̶�����
int SockID;


//����������
char m_sSessionIdCCR[128];
char m_sservNUMCCR[128];
char m_sdstrealmCCR[128];
//����������

int PackMsgMgr::init(ABMException &oExp)
{
    char *p;
    
    if (g_iProcID == -1) {
        ADD_EXCEPT0(oExp, "���������б������� -p ��!");
        return -1;  
    }
    
    //��ȡ�����ļ�
    DccConfStru struConf;
    if (DccConf::getDccConf(g_iProcID, struConf, oExp) != 0) {
        ADD_EXCEPT1(oExp, "ParserMsgMgr::init ʧ��, ����ParserMsgMgr -p %d ��abm_app_cfg.ini������!", g_iProcID);
        return -1;      
    }
    
    //ҵ��ڵ���ص�������Ϣ
	if (DccConf::getNodeConf(g_oNodeConf, oExp) != 0) {
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
    
	SockID=struConf.m_iSockID;
    m_poDcc = new DccMsgParser(struConf.m_sDictFile);
    if (NULL == m_poDcc) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init m_poDcc malloc failed!");
        return -1;
    }
    
    return 0;
}

int PackMsgMgr::run()
{
    int iRet, iFlag;
    int iMsgLen;
	
     //����������
		string sORACLEName = "ABM";
        string sORACLEuser = "abm1";
        string sORACLEpwd = "abm1";
		  OracleConn *pOracleConn = NULL;
          TOCIQuery *pQryCursor = NULL;  
          pOracleConn = new OracleConn;        
          if (!pOracleConn->connectDb(sORACLEuser, sORACLEpwd, sORACLEName)) {
               printf("ORACLE CONNECT ERROR");
               return  - 1;
          }
          DEFINE_OCICMDPOINTBYCONN(pQryCursor, (*pOracleConn));
          if (pQryCursor == NULL) {
              printf("pQryCursor ERROR");
              return  - 1;
          }
		//����������  
    while (!RECEIVE_STOP()) {
        if ((iRet=g_poRcvMQ->Receive((void *)&g_oRcvBuf, sizeof(g_oRcvBuf), 0, true)) < 0) {
            if (iRet == -2) {
                continue;   
            }   
            __DEBUG_LOG1_(0, "Ӧ�ó������Ϣ���н���ʧ��, �����˳�! errno=%d", errno);
            return -1;
        }
        //disp_buf((char *)&g_oRcvBuf, sizeof(g_oRcvBuf) + sizeof(long));
        //��Ҫ���������������������������Ӧ���
        //iFlag = 
		//lijianchen 20110530
        struDccHead *pHead = (struDccHead *)g_oRcvBuf.m_sDccBuf;
        if (strcmp(g_oNodeConf.m_sOriginHost, pHead->m_sOrignHost) == 0) {
            iFlag = 1;
            pHead->m_iHH = getH2H();
            pHead->m_iEE = getE2E();
        } else {
            iFlag = 0;
        }
        
		if (setDccHead(m_poDcc, &g_oRcvBuf,iFlag,m_sSessionIdCCR,m_sdstrealmCCR) != 0) {
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
				
			case ABMCMD_REVERSE_PAY:          //3.4.2.4  ֧������
		            iRet = ReversePaySvcA(m_poDcc, &g_oRcvBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "ReversePaySvcA: �������.");
		                continue;
		            }
		            break;
					
            case ABMPAYCMD_INFOR:                       //3.4.2.1 ֧��
                iRet = packChargeA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packChargeA: DCC�������.");
		            continue;
		        }
                break;
            case ABMCMD_QRY_SRV_PSWD:                  //3.4.2.5 ������֤�������·�
            	iRet = packRandPaswA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packRandPaswA: DCC�������.");
		            continue;
		        }
                break;
			case ABMCMD_ALLOCATE_BALANCE:                          //3.4.5.1 ��������
			   //lijianchen 20110530
			    if (iFlag == 0)
                    iRet = AllocateBalanceSvcA(m_poDcc, &g_oRcvBuf);
                else
                    iRet = packDeductBalR(m_poDcc, &g_oRcvBuf);
                
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "AllocateBalanceSvcA: DCC�������.");
		            continue;
		        }
                break;
                
            case ABMCMD_REVERSE_DEDUCT_BALANCE:                    //3.4.5.5
             //lijianchen 20110530
		        if (iFlag == 0)
                    iRet = ReqReverseDeductSvcA(m_poDcc, &g_oRcvBuf);
                else
                    iRet = packRvsChargeR(m_poDcc, &g_oRcvBuf);

                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "ReqReverseDeductSvcA: DCC�������.");
		            continue;
		        }
                break;
				
			case ABMCMD_QRY_BIL_INFOR:                            //3.4.6.16 �����˵���ѯ
                iRet = RechargeBillQueryA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "RechargeBillQueryA: DCC�������.");
		            continue;
		        }
                break;
			
			case ABMCMD_QRY_PAY_INFOR:                            //3.4.6.15
                iRet = QueryPayLogByAccoutA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "QueryPayLogByAccoutA: DCC�������.");
		            continue;
		        }
                break;
			
			case ABMCMD_QRY_REC_INFOR:                            //3.4.6.14
                iRet = RechargeRecQueryA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "RechargeRecQueryA: DCC�������.");
		            continue;
		        }
                break;
                
            case ABMCMD_QRY_MICROPAY_INFO:                       //3.4.6.4  ������ѯ   
                iRet = packQryBalDrawA(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packQryBalDrawA: DCC�������.");
		            continue;
		        }
                break;
                
            case ABMCMD_QRY_MICROPAYCCA_INFO:                    //3.4.6.4  ������ѯ ����abm��������
                iRet = packQryBalDrawR(m_poDcc, &g_oRcvBuf);
                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "packQryBalDrawR: DCC�������.");
		            continue;
		        }
                break;
			case ABMCMD_QRY_BAL_INFOR:      //3.4.6.13,  3.4.5.3,  3.4.5.4 ����ѯ
			 //lijianchen 20110530
			    if (iFlag == 0)
                    iRet = QueryBalanceA(m_poDcc, &g_oRcvBuf);
                else
                    iRet = packQryBalanceR(m_poDcc, &g_oRcvBuf,m_sservNUMCCR);

                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "QueryBalanceR: DCC�������.");
		            continue;
		        }
                break; 
				
			case ABMDEPOSITCMD_INFOR:      //3.4.4.1 ������ֵ VC�淶2.1  �ų�
		            iRet = DepositServCondOneA(m_poDcc, &g_oRcvBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "DepositServCondOneA���������.");
		                continue;
		            }
		            break;
					
			case ABM_REVERSEEPOSIT_CMD:      //3.4.4.3 ��ֵ���� VC�淶2.3  �ų�
			        iRet = ReverseDepositA(m_poDcc, &g_oRcvBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "DepositServCondOneA���������.");
		                continue;
		            }
		            break;
			
			case ABMCMD_PLATFORM_DEDUCT_BALANCE:      ////3.4.6.5  ȫ����������  zhuyong
			/**
		            iRet = ReqPlatformSvcA(m_poDcc, &g_oRcvBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
					**/
		            break;
					
			case ABMCMD_ACTIVE_ALLOCATE_BALANCE:      //3.4.5.2 ����ȫ����������ȫ�����ķ���CCR  zhuyong
		            iRet = ReqBalanceSvcR(m_poDcc, &g_oRcvBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break;
			case ABMCMD_QRY_SRV_SYN:      //3.4.1.2  ����ͬ��
			    if (iFlag == 0)
                    iRet = QryServSynCondA(m_poDcc, &g_oRcvBuf);
                else
                    iRet = QryServSynCondRT(m_poDcc, &g_oRcvBuf);

                if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "QueryBalanceR: DCC�������.");
		            continue;
		        }
                break; 
			
			case ABMCMD_PASSWD_IDENTIFY:      //3.4.1.3 �����Ȩ ABM����HSS
		            iRet = PasswdIdentifyCondRT(m_poDcc, &g_oRcvBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break;
					
			case ABMCMD_LOGIN_PASSWD:      //3.4.6.1 ��½��Ȩ �Է�ƽ̨����ABM
		            iRet = LoginPasswdCCRA(m_poDcc, &g_oRcvBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break;
			
            default:
                __DEBUG_LOG1_(0, "δ֪����������[%d].", g_oRcvBuf.m_iCmdID);
		        continue;
                break;
        }
        //g_sSndBuf.m_lMsgType = g_oRcvBuf.m_lType;
		/***
		if (strcmp(g_oNodeConf.m_sOriginRealm, pHead->m_sDestRealm) == 0)
		{
		   g_sSndBuf.m_lMsgType = 2;
		} else {
		g_sSndBuf.m_lMsgType = 1;
		}
		***/
		if(g_oRcvBuf.m_iCmdID == ABMCMD_QRY_BAL_INFOR){
		 pQryCursor->setSQL("insert into ABM_RESULT "
            "(SESSION_ID,RESULT_DATE,SERV_NUMBER,INFO_FLAG,DEST_REALM) "
            "VALUES (:p0,sysdate,:p1,'R',:p2)");
        pQryCursor->setParameter("p0", m_sSessionIdCCR);
        pQryCursor->setParameter("p1", m_sservNUMCCR);
        pQryCursor->setParameter("p2", m_sdstrealmCCR);
        pQryCursor->execute();
        pQryCursor->close();
        pQryCursor->commit();
		}
		g_sSndBuf.m_lMsgType = SockID;
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
