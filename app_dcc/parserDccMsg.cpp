#include "parserDccMsg.h"



static MessageQueue *g_poRcvMQ = NULL;         //������Ϣ����
static MessageQueue *g_poSndMQ = NULL;         //������Ϣ���� 
static MessageQueue *g_poSndPeerMQ = NULL;     //������Ϣ����,����sock������
static MessageQueue *g_poSndCCAMQ  = NULL;     //ҵ�������������CCR���յ��Ļذ������������Ϣ����

static ABMCCR g_oSndBuf;                  //���ͻ�����������ҵ�������
static StruMqMsg   g_sRecvBuf;            //���ջ����������մ�socket���͹�������Ϣ
static StruMqMsg   g_sPeerBuf;            //���ͻ������������������ҵ������̣�ֱ�ӷ���socketʱ�õ�
//static struDccHead goDccHead;           //dcc��Ϣ�Ĺ���ͷ����Ϣ
extern NodeConfStru g_oNodeConf;          //ҵ��ڵ��һЩ�̶�����
//����������
char m_sSessionIdFD[128];
char m_sOriginHostFD[128];
char m_sDstRealmFD[128];
//ACE_UINT32 U32Var;
ACE_UINT32 m_iresultCodeFD;
ACE_UINT32 m_iSresultCodeFD;
//����������

int ParserMsgMgr::init(ABMException &oExp)
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
    //����sock����������Ϣ����
    if (struConf.m_iSndPeerKey <= 0 || struConf.m_iSndCcaKey <=0) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init ����������Ҫ����mqsnd_peer��mqsend_cca.");
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
    //�����������Ӧ�Ļذ���������Ϣ����
    if ((g_poSndCCAMQ=new MessageQueue(struConf.m_iSndCcaKey)) == NULL) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init g_poSndPeerMQ malloc failed!");
        return -1;
    }
	if (g_poSndCCAMQ->open(oExp, true, true) != 0) {
        ADD_EXCEPT1(oExp, "ParserMsgMgr::init g_poSndPeerMQ->open ��Ϣ��������ʧ��!", struConf.m_iSndKey);
        return -1;
    }
    
    m_poDcc = new DccMsgParser(struConf.m_sDictFile);
    if (NULL == m_poDcc) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init m_poDcc malloc failed!");
        return -1;
    }
    //20110604
    m_poSql = new DccSql;
    if (NULL == m_poSql) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init m_poSql malloc failed!");
        return -1;
    }
    if (m_poSql->init(oExp) != 0) {
        ADD_EXCEPT0(oExp, "ParserMsgMgr::init m_poSql init failed!");
        return -1;
    }
     //20110604
    return 0;
}

int ParserMsgMgr::run()
{
    int iRecvLen, iRet, iCmdId, iSize, iFlag;
    string sTemp,sProductName;
	char m_sOriginName[30];
	char m_sOriginNameHSS[30]="HSS2@001.ChinaTelecom.com";
    MessageQueue *pMqTmp;
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
       //����������
        m_iSresultCodeFD=2222;
		m_poDcc->getAvpValue("Session-Id", sTemp);
        snprintf(m_sSessionIdFD, sizeof(m_sSessionIdFD), sTemp.c_str());
		
	    m_poDcc->getAvpValue("Origin-Host", sTemp);
	    snprintf(m_sOriginHostFD, sizeof(m_sOriginHostFD), sTemp.c_str());
		m_poDcc->getAvpValue("Destination-Realm", sTemp);
	    snprintf(m_sDstRealmFD, sizeof(m_sDstRealmFD), sTemp.c_str());
		
		m_poDcc->getAvpValue("Result-Code", m_iresultCodeFD);
		
	    m_poDcc->getAvpValue("Service-Information.Service-Result-Code", m_iSresultCodeFD);
		long m_lSresultCodeFD =m_iSresultCodeFD;
		if((strcmp(m_sOriginHostFD,"ABM@001.ChinaTelecom.com")!= 0 )&&(strcmp(m_sOriginHostFD,"SR1@001.ChinaTelecom.com")!= 0 )){
		//if(strcmp(m_sOriginHostFD,"ABM@001.ChinaTelecom.com")!= 0 ){
		if ((m_iresultCodeFD==2001)&&(m_iSresultCodeFD==0)){
		   pQryCursor->setSQL("insert into ABM_RESULT "
            "(SESSION_ID,ORIGIN_HOST,RESULT_DATE,RESULT_CODE,SERVICE_RESULT_CODE) "
            "VALUES (:p0,:p1,sysdate,0,:p2)");
        pQryCursor->setParameter("p0", m_sSessionIdFD);
        pQryCursor->setParameter("p1", m_sOriginHostFD);
		pQryCursor->setParameter("p2", m_lSresultCodeFD);
        pQryCursor->execute();
        pQryCursor->close();
        pQryCursor->commit();
		}
		else{
		   pQryCursor->setSQL("insert into ABM_RESULT "
            "(SESSION_ID,ORIGIN_HOST,RESULT_DATE,RESULT_CODE,SERVICE_RESULT_CODE) "
            "VALUES (:p0,:p1,sysdate,1,:p2)");
        pQryCursor->setParameter("p0", m_sSessionIdFD);
        pQryCursor->setParameter("p1", m_sOriginHostFD);
		pQryCursor->setParameter("p2", m_lSresultCodeFD);
        pQryCursor->execute();
        pQryCursor->close();
        pQryCursor->commit();
		}
		}
		if (((m_iresultCodeFD!=2001)&&(m_iresultCodeFD!=0))||((m_iSresultCodeFD==2222)&&(strcmp(m_sDstRealmFD,"001.ChinaTelecom.com")!= 0 )&&
		    ((strcmp(m_sOriginHostFD,"ABM@001.ChinaTelecom.com")!= 0 )&&(strcmp(m_sOriginHostFD,"SR1@001.ChinaTelecom.com")!= 0)))){
		    	
		continue;
		}
		//����������
		
		
		g_sPeerBuf.m_lMsgType = g_sRecvBuf.m_lMsgType;
		g_oSndBuf.m_lType =g_sRecvBuf.m_lMsgType;
		 
		if ( (iRet = m_poDcc->getDiameterCommandCodeInfo()) == COMMAND_CODE_CE ) {    //��������
		    if (m_poDcc->getDiameterHeader().flags.r) {  //������Ϣ
		        iRet = packCEA(m_poDcc, &g_sPeerBuf);
		        if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "���CEA��Ϣ����.");
		            continue;
		        }        
		    } else {                                     //Ӧ����Ϣ
			    m_poDcc->getAvpValue("Origin-Host", sProductName);
				snprintf(m_sOriginName, sizeof(m_sOriginName), sProductName.c_str());
				if(strcmp(m_sOriginName,m_sOriginNameHSS)!=0){
		            __DEBUG_LOG0_(0, "�յ���������Ӧ���,���������������.");
		            iRet = packNetCtrl(m_poDcc, &g_sPeerBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "��������������.");
		                continue;
		            }
					if (g_poSndPeerMQ->Send((void *)&g_sPeerBuf, iRet + sizeof(long)) < 0) {
			            __DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			         return -1;	
		            }
				}
		    }
			/***
		    if (g_poSndPeerMQ->Send((void *)&g_sPeerBuf, iRet + sizeof(long)) < 0) {
			    __DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			    return -1;	
		    }
			***/
		} else if (iRet == COMMAND_CODE_DW) {                                         //�豸���
		    if (m_poDcc->getDiameterHeader().flags.r) {   //������Ϣ
		        iRet = packDWA(m_poDcc, &g_sPeerBuf);
		        if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "���DWA��Ϣ����.");
		            continue;
		        }
		    
		        if (g_poSndPeerMQ->Send((void *)&g_sPeerBuf, iRet + sizeof(long)) < 0) {
			        __DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			        return -1;	
		        }
		    } else {                                      //Ӧ����Ϣ
		        __DEBUG_LOG0_(0, "�յ��豸���Ӧ���������.");
		    }
		} else if (iRet == COMMAND_CODE_DP) {                                         //����Զ�
		    if (m_poDcc->getDiameterHeader().flags.r) {  //������Ϣ
		        iRet = packDPA(m_poDcc, &g_sPeerBuf);
		        if (iRet < 0 ) {
		            __DEBUG_LOG0_(0, "����DPA��Ϣ����.");
		            continue;
		        }
		    
		        if (g_poSndPeerMQ->Send((void *)&g_sPeerBuf, iRet + sizeof(long)) < 0) {
			        __DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			        return -1;	
		        }
		    } else {                                              //Ӧ����Ϣ
		        __DEBUG_LOG0_(0, "�յ�����Զ�Ӧ���������.");
		    }
		} else if (iRet == COMMAND_CODE_CC) {                                         //ҵ����Ϣ
		    //��ȡ������Ϣ�Ĺ������֣��������Ϣ����
		    iFlag = (m_poDcc->getDiameterHeader().flags.r)?1:0;
		    g_oSndBuf.clear();
		    iCmdId = getDccHead(m_poDcc, &g_oSndBuf, iFlag, m_poSql->m_poQrySession);
		    if (iCmdId < 0 ) {
		        __DEBUG_LOG0_(0, "��ȡdcc��Ϣͷ����Ϣ����.");
		        continue;
		    }
		    
		    switch (iCmdId)
		    {
		        case ABMCMD_QRY_SRV_INFOR:          //3.4.1.1 ���ϲ�ѯ������
		            iRet = parserInfoQryR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserInfoQryR: �������.");
		                continue;
		            }
		            break;
					
				case ABMCMD_REVERSE_PAY:          //3.4.2.4  ֧������
		            iRet = ReversePaySvcR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "ReversePaySvcR: �������.");
		                continue;
		            }
		            break;
		        case ABMPAYCMD_INFOR:               //3.4.2.1 ֧��
		            iRet = parserChargeR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserChargeR���������.");
		                continue;
		            }
		            break;
				case ABMCMD_QRY_SRV_PSWD:          //3.4.2.5 ������֤�������·�
					iRet = parserRandPswdR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserRandPswdR���������.");
		                continue;
		            }
		            break;
		        case ABMCMD_ALLOCATE_BALANCE:      //3.4.5.1 ��������
		           //lijianchen 20110530
				    if (iFlag == 1) {
		                iRet = AllocateBalanceSvcR(m_poDcc, &g_oSndBuf);
		            } else {
		                iRet = parserDeductBalA(m_poDcc, &g_oSndBuf);
		            }
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "AllocateBalanceSvcR. �������.");
		                continue;
		            }
		            break;
				case ABMCMD_REVERSE_DEDUCT_BALANCE:   //3.4.5.5 
			//lijianchen 20110530
				    if (iFlag == 1) {
		                iRet = ReqReverseDeductSvcR(m_poDcc, &g_oSndBuf);
		            } else {
		                iRet = parserRvsChargeA(m_poDcc, &g_oSndBuf);
		            }
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "ReqReverseDeductSvcR. �������.");
		                continue;
		            }
		            break;
		            	
				case ABMCMD_QRY_BIL_INFOR:            //3.4.6.16 �����˵���ѯ
		            iRet = RechargeBillQueryR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "RechargeBillQueryR���������.");
		                continue;
		            }
		            break;
					
				case ABMCMD_QRY_PAY_INFOR:            //3.4.6.15 �����˵���ѯ
		            iRet = QueryPayLogByAccoutR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "QueryPayLogByAccoutR���������.");
		                continue;
		            }
		            break;
					
				case ABMCMD_QRY_REC_INFOR:            //3.4.6.14 
		            iRet = RechargeRecQueryR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "RechargeRecQueryR���������.");
		                continue;
		            }
		            break;
		            
		        case ABMCMD_QRY_MICROPAY_INFO:        //3.4.6.4  ������ѯ  �յ���Χϵͳ����������
		            iRet = parserQryBalDrawR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawR���������.");
		                continue;
		            }
		            break;
		            
		        case ABMCMD_QRY_MICROPAYCCA_INFO:      //3.4.6.4  ������ѯ  ��Χϵͳ��abm������������CCR�Ļذ�
		             iRet = parserQryBalDrawA(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break;
				case ABMCMD_QRY_BAL_INFOR:      //3.4.6.13,  3.4.5.3,  3.4.5.4 ����ѯ
		       //lijianchen  20220530
			        if (iFlag == 1) {
		                iRet = QueryBalanceR(m_poDcc, &g_oSndBuf);
		            } else {
		                iRet = parserQryBalanceA(m_poDcc, &g_oSndBuf);
		            }
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break;
				case ABMDEPOSITCMD_INFOR:      //3.4.4.1 ������ֵ VC�淶2.1  �ų�
		             iRet = DepositServCondOneR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break;
				case ABM_REVERSEEPOSIT_CMD:      //3.4.4.3 ��ֵ���� VC�淶2.3  �ų�
		             iRet = ReverseDepositR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break;
					
				case ABMCMD_PLATFORM_DEDUCT_BALANCE:      ////3.4.6.5  ȫ����������  zhuyong
		             iRet = ReqPlatformSvcR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break;
					
				case ABMCMD_ACTIVE_ALLOCATE_BALANCE:      //3.4.5.2 ����  ��ȫ������CCAzhuyong
		             iRet = ReqBalanceSvcA(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break;
		        case ABMCMD_QRY_SRV_SYN:      //3.4.1.2 ����ͬ��
			        if (iFlag == 1) {
		                iRet = QryServSynCondR(m_poDcc, &g_oSndBuf);
		            } else {
		                iRet = QryServSynCondAT(m_poDcc, &g_oSndBuf);
		            }
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break; 
				case ABMCMD_PASSWD_IDENTIFY:      //3.4.1.3 �����Ȩ ABM����HSS
		             iRet = PasswdIdentifyCondAT(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "parserQryBalDrawA���������.");
		                continue;
		            }
		            break;	
				case ABMCMD_LOGIN_PASSWD:      //3.4.6.1 ��½��Ȩ �Է�ƽ̨����ABM
		             iRet = LoginPasswdCCRR(m_poDcc, &g_oSndBuf);
		            if (iRet < 0 ) {
		                __DEBUG_LOG0_(0, "LoginPasswdCCRR���������.");
		                continue;
		            }
		            break;	
		        default:
		            __DEBUG_LOG1_(0, "δ֪����������[%d].", iCmdId);
		            continue;
		            break;
            }
#ifdef _DEBUG_DCC_
            cout<<"parserDccMsg send packet:"<<endl;
            cout<<"command_id--"<<g_oSndBuf.m_iCmdID<<endl;
            cout<<"size--"<<g_oSndBuf.m_iSize<<endl;
            //cout<<"first number--"<<g_oSndBuf.m_iFirNum<<endl;
            //cout<<"second number--"<<g_oSndBuf.m_iSecNum<<endl;
            //cout<<"flags--"<<g_oSndBuf.m_iFlag<<endl;
#endif
            //���ݲ�ͬ��ͬ��Ϣ���ͷŵ���ͬ����Ϣ����
            /*
            if (iFlag == 1)            //�ж���CCR����CCA
                pMqTmp = g_poSndMQ;    //�����������������Ϣ����
            else
                pMqTmp = g_poSndCCAMQ; //��������Ļذ���������������Ϣ����
            */
            
            //���ݲ�ͬ��ͬ��Ϣ���ͷŵ���ͬ����Ϣ����
            switch (iCmdId)
            {
                case ABMCMD_QRY_MICROPAY_INFO:       //3.4.6.4  ������ѯ  �յ���Χϵͳ����������
                case ABMCMD_QRY_MICROPAYCCA_INFO:    //3.4.6.4  ������ѯ  ��Χϵͳ��abm������������CCR�Ļذ�
                    pMqTmp = g_poSndMQ; 
                    break;
                default:
                    pMqTmp = g_poSndMQ;
                    break;
            }
            
		    if (pMqTmp->Send((void *)&g_oSndBuf, g_oSndBuf.m_iSize) < 0) {
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
	__FREE_PTR(g_poSndCCAMQ);
	
	return 0;
}
