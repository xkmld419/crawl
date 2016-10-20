#include "testxmlmsg.h"

#define MSG_SIZE 4096

struct StruMqMsg {
    long m_lMsgType;
    char m_sMsg[MSG_SIZE];
};

static MessageQueue *g_poRcvMQ = NULL;      //������Ϣ����
static MessageQueue *g_poSndMQ = NULL;      //������Ϣ����

static ABMCCA    g_oRcvBuf;                    //���ͻ�����������ҵ�������
static StruMqMsg g_sSndBuf;                    //���ͻ�����������socket

int PackXMLMsg::init(ABMException &oExp)
{
    if ((g_poSndMQ=new MessageQueue(10007)) == NULL) {
        ADD_EXCEPT0(oExp, "PackXMLMsg::init g_poSndMQ malloc failed!");
        return -1;
    }
	if (g_poSndMQ->open(oExp, true, true) != 0) {
        ADD_EXCEPT0(oExp, "PackXMLMsg::init g_poSndMQ->open ��Ϣ��������ʧ��!");
        return -1;
    }
	 //snprintf(m_sXmlFile, sizeof(m_sXmlFile), "%log/pack_%d.xml" ,getHomeDir(), getpid());
    if ((m_poXmlDoc = new TiXmlDocument()) == NULL) {
        ADD_EXCEPT0(oExp, "PackXMLMsg::init m_poXmlDoc malloc failed!");
        return -1;
    }
    return 0;
}

int PackXMLMsg::destroy()
{
	//�ͷ�ҵ������
    #define __FREE_PTR(p) \
        if (p != NULL) { \
            delete p; p = NULL; }
    
    __FREE_PTR(g_poSndMQ);
    __FREE_PTR(g_poRcvMQ);
	
	return 0;
}

/***
int PackXMLMsg::run()
{
    int iRet;
    int iMsgLen;	
    while (!RECEIVE_STOP()) {
      
        //g_sSndBuf.m_lMsgType = g_oRcvBuf.m_lType;
		g_sSndBuf.m_lMsgType = 1;
        
        //���xml����
        TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "gbk", "" ); 
        m_poXmlDoc->LinkEndChild(decl);
		
		int icmd;
		cout<<"������Ҫ�����ҵ��"<<endl;
		cout<<"�˳�  0"<<endl;
		cout<<"���� Balance_Trans  1"<<endl;
		cout<<"����ѯ Query  2"<<endl;
		cout<<"������ѯ Balance_Query  3"<<endl;
		cout<<"�����˵���ѯ RechargeBillQuery  4"<<endl;
		cout<<"���׼�¼��ѯ QueryPayLogByAccout  5"<<endl;
		cout<<"���˼�¼��ѯ RechargeRecQuery  6"<<endl;
		cout<<"֧�� Charge  7"<<endl;
		cout<<"֧������״̬��ѯ PayStatus_Query 8"<<endl;
		cout<<"���������ѯ TransRules_Query 9"<<endl;
		cout<<"������������ TransRules_Reset 10"<<endl;
		cout<<"֧��&��½�����޸� Password_Reset 11"<<endl;
		cout<<"֧������״̬��� PayStatus_Reset 12"<<endl;

		cin>>icmd;
        
        switch (icmd)
        {
            case 1:   //����
                iRet = packTransBalA();
                break;
				
            case 2:  //����ѯ
                iRet = packQueryA();
				break;
			
			case 3:  //������ѯ
			    iRet = packQryBalA();
				break;

			case 4: //�����˵���ѯ
			    iRet = packQryRechargeBillA();
				break;
			case 5: //���׼�¼��ѯ
			    iRet = packQryPayLogA();
				break;
			case 6: //���˼�¼��ѯ
			    iRet = packRechargeRecA();
				break;
			case 7: 	    //3.4.6.2 ֧��
				iRet = packChargeA();
				break;
				
			case 8: 	    // ֧������״̬��ѯ
				iRet = packQryPayStatusA();
				break;
				
			case 9: 	    // ֧������״̬��ѯ
				iRet = packQryTransRuleA();
				break;
			case 10: 	    // ������������
				iRet = packResetTransRuleA();
				break;
			case 11: 	    // ������������
				iRet = packResetPassWdA();
				break;
			case 12: 	    // ������������
				iRet = packResetPayStatusA();
				break;

			case 0:
			    goto __OPEN;
            default:
                break;
        }
        
        if (iRet != 0) {
            m_poXmlDoc->Clear();
            __DEBUG_LOG1_(0, "��Ϣ������̳�������ţ�%d. ", g_oRcvBuf.m_iCmdID);
            continue;
        }
        

		//������ڴ沢���xml�е�����
        TiXmlPrinter printer;
		printer.SetIndent( "" );
        printer.SetLineBreak( "" );
        m_poXmlDoc->Accept( &printer );
        snprintf(g_sSndBuf.m_sMsg+4, MSG_SIZE-4, printer.CStr());
        iMsgLen = printer.Size() + 4;
        //ͷ4���ֽڰ�������Ϣ��dcc��Ϣһ��
        *(int *)g_sSndBuf.m_sMsg = htonl(iMsgLen);
        g_sSndBuf.m_sMsg[0] = 0x01;
        //
        m_poXmlDoc->Clear();
		
        
#ifdef _DEBUG_TI_XML_
        cout<<"CCA��Ϣ����ǰԭʼ��Ϣ����\n"<<g_sSndBuf.m_sMsg<<endl;
#endif
          
        if (g_poSndMQ->Send((void *)&g_sSndBuf, iMsgLen+sizeof(long)) < 0) {
	        __DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			return -1;	
		} 
		
    }
    
__OPEN:
    __DEBUG_LOG0_(0, "�����յ���ȫ�˳��ź�, ��ȫ�˳�");
    
    return 0;
}
***/

//20110819xkm

int PackXMLMsg::run()
{
    int iRet;
    int iMsgLen;
	char sTmp[256];
	char sTmpNUM[256];
	string sTmpT;
	while(1)
   {
	ABMException oExp;
	ReadCfg *m_poConfS = NULL; 
	char * const pLable = "TEST_CONF";
    if (m_poConfS == NULL) { 
        if ((m_poConfS=new ReadCfg) == NULL) {
            ADD_EXCEPT0(oExp, "ABMConf::getConf m_poConfS malloc failed!");
            return -1;
        }
    }
    //char *p = getenv(ABM_HOME);
	char sValue[128];
    char path[1024];

 //   char sTmpC[1024] = {0};
 //   strncpy(sTmpC, p, sizeof(sTmpC)-2);
 //   if (sTmpC[strlen(sTmpC)-1] != '/') {
 //       sTmpC[strlen(sTmpC)] = '/';
 //   }

    snprintf(path, sizeof(path), "/ABM/app/deploy/config/abm_Balance_number.ini");
    if (m_poConfS->read(oExp, path, pLable) != 0) {
        ADD_EXCEPT2(oExp, "AccuConf::getConf ��ȡ�����ļ�%s�е�%s����", path, pLable);
        return -1;
    }
    int TransFlag =2;
	for (TransFlag;TransFlag>0;TransFlag--){
    int iLines = m_poConfS->getRows();
	int i = 0;
	int j = 0;
	memset(sValue, 0x00, sizeof(sValue));
	
    for(i;i<iLines;i++)
	{
	 j = i+1;
	 cout<<"�������������"<<j<<endl;
	 if (!m_poConfS->getValue(oExp, sValue, "number", j)) {
            ADD_EXCEPT1(oExp, "�����ļ���%s��δ����numberѡ��,��ȡʧ��!", pLable);
            return -1;
        }
	
    //sTmpT.assign(sValue);
	//strncpy(sTmpNUM,(LPCTSTR)sTmpT,sizeof(sTmpNUM));  
	int iVar;

	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("Balance_Trans")); 
	root->LinkEndChild(pRltCode);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","898901100707164230",tTime);
	pRltCode = new TiXmlElement("Request_Id");
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	pRltCode = new TiXmlElement("Request_Time");
	snprintf(sTmp, sizeof(sTmp), "%10d", 2248988800);
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	

	TiXmlElement * pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sValue)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=1;
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=123456;
	pElem = new TiXmlElement("Pay_Password");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Balance_TransFlag");
	snprintf(sTmp, sizeof(sTmp), "%d", TransFlag);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=0;
	pElem = new TiXmlElement("UnitType_Id");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=1;
	pElem = new TiXmlElement("Deduct_Amount");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);

    //if (iRet != 0) {
    //        m_poXmlDoc->Clear();
    //        __DEBUG_LOG1_(0, "��Ϣ������̳�������ţ�%d. ", g_oRcvBuf.m_iCmdID);
    //       continue;
    //    }
   

		//������ڴ沢���xml�е�����
        TiXmlPrinter printer;
		printer.SetIndent( "" );
        printer.SetLineBreak( "" );
        m_poXmlDoc->Accept( &printer );
        snprintf(g_sSndBuf.m_sMsg+4, MSG_SIZE-4, printer.CStr());
        iMsgLen = printer.Size() + 4;
        //ͷ4���ֽڰ�������Ϣ��dcc��Ϣһ��
        *(int *)g_sSndBuf.m_sMsg = htonl(iMsgLen);
        g_sSndBuf.m_sMsg[0] = 0x01;
        //
        m_poXmlDoc->Clear();
		
        
#ifdef _DEBUG_TI_XML_
        //cout<<"CCA��Ϣ����ǰԭʼ��Ϣ����\n"<<g_sSndBuf.m_sMsg<<endl;
#endif
          
        if (g_poSndMQ->Send((void *)&g_sSndBuf, iMsgLen+sizeof(long)) < 0) {
	        __DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			return -1;	
		} 
		sleep(1);
	}
	sleep(10);
	}
	sleep(500);
	
   }
}


//3.4.6.5 ����
int PackXMLMsg::packTransBalA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("Balance_Trans")); 
	root->LinkEndChild(pRltCode);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","002201100707164230",tTime);
	pRltCode = new TiXmlElement("Request_Id");
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	
	pRltCode = new TiXmlElement("Request_Time");
	snprintf(sTmp, sizeof(sTmp), "%10d", 2248988800);
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	TiXmlElement * pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Pay_Password");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	cout<<"�����뻮������ 1 ��������(������ֵ)  2 ��������(��������)"<<endl;
    cin>>iVar;
	
	pElem = new TiXmlElement("Balance_TransFlag");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("UnitType_Id");
	snprintf(sTmp, sizeof(sTmp), "%d", 0);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	cout<<"�����뻮�����"<<endl;
	cin>>iVar;
	pElem = new TiXmlElement("Deduct_Amount");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	return 0;
}


//3.4.6.13 ����ѯ
int PackXMLMsg::packQueryA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("Query")); 
	root->LinkEndChild(pRltCode);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","002201100707164230",tTime);
	pRltCode = new TiXmlElement("Request_Id");
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Balance_Information");
	root->LinkEndChild(pPayInfoB);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	TiXmlElement * pElem = new TiXmlElement("Destination_Id");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_Id_Type");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	cout<<"����������"<<endl;
    cin>>iVar;
	pElem = new TiXmlElement("AreaCode");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);

	pElem = new TiXmlElement("Query_Flag");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Query_Item_Type");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	
	return 0;
}


//3.4.6.4 ������ѯ
int PackXMLMsg::packQryBalA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("Balance_Query")); 
	root->LinkEndChild(pRltCode);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","898901100707164230",tTime);
	pRltCode = new TiXmlElement("Request_Id");
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	pRltCode = new TiXmlElement("Request_Time");
	snprintf(sTmp, sizeof(sTmp), "%10d", 2248988800);
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Balance_Information");
	root->LinkEndChild(pPayInfoB);
	
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	TiXmlElement * pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=1;
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);

	pElem = new TiXmlElement("Query_Flag");
	snprintf(sTmp, sizeof(sTmp), "%d", 0);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Query_Item_Type");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	return 0;
}


//3.4.6.16 �����˵���ѯ
int PackXMLMsg::packQryRechargeBillA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("RechargeBillQuery")); 
	root->LinkEndChild(pRltCode);
	
	/**
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","002201100707164230",tTime);
	pRltCode = new TiXmlElement("Request_Id");
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	pRltCode = new TiXmlElement("Request_Time");
	snprintf(sTmp, sizeof(sTmp), "%10d", 2248988800);
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	***/
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	TiXmlElement * pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","002201100707164230",tTime);
	pElem = new TiXmlElement("Operate_Seq");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
    iVar = 20110601;
	pElem = new TiXmlElement("Billing_Cycle_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);	
	
	return 0;
}

//3.4.6.15 ���׼�¼��ѯ
int PackXMLMsg::packQryPayLogA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("QueryPayLogByAccout")); 
	root->LinkEndChild(pRltCode);
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	
	TiXmlElement * pElem = new TiXmlElement("Operate_Action");
	snprintf(sTmp, sizeof(sTmp), "%d", 13);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","002201100707164230",tTime);
	pElem = new TiXmlElement("Operate_Seq");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("SP_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", 23456);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", 23456);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Query_Start_Time");
	snprintf(sTmp, sizeof(sTmp), "%d", 20110701);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
    iVar = 20110711;
	pElem = new TiXmlElement("Query_Expiration_Time");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);	
	
	return 0;
}

//3.4.6.14 ���˼�¼��ѯ
int PackXMLMsg::packRechargeRecA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("RechargeRecQuery")); 
	root->LinkEndChild(pRltCode);
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	
		time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","002201100707164230",tTime);
	TiXmlElement * pElem = new TiXmlElement("Operate_Seq");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
		
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Query_Start_Time");
	snprintf(sTmp, sizeof(sTmp), "%d", 20110701);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
    iVar = 20110711;
	pElem = new TiXmlElement("Query_Expiration_Time");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);	
	
	return 0;
}


 //3.4.6.2 ֧��
int PackXMLMsg::packChargeA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("Charge")); 
	root->LinkEndChild(pRltCode);
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	
	TiXmlElement * pElem = new TiXmlElement("Operate_Action");
	snprintf(sTmp, sizeof(sTmp), "%d", 8);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
		time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","889901100707164230",tTime);
	pElem = new TiXmlElement("Operate_Seq");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("SP_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Staff_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", 888);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Micropay_Type");
	snprintf(sTmp, sizeof(sTmp), "%d", 0);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
		
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Pay_Password");
	snprintf(sTmp, sizeof(sTmp), "%s", "123456");
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Random_Password");
	snprintf(sTmp, sizeof(sTmp), "%s", "123456");
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Pay_Date");
	snprintf(sTmp, sizeof(sTmp), "%d", 20110712);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Order_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", 98765);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Description");
	snprintf(sTmp, sizeof(sTmp), "%s", "֧��ƽ̨");
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);	
	
	cout<<"֧�����"<<endl;
	cin>>iVar;
	pElem = new TiXmlElement("PayAmount");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);	
	
	return 0;
}


// ֧������״̬��ѯ
int PackXMLMsg::packQryPayStatusA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("PayStatus_Query")); 
	root->LinkEndChild(pRltCode);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","898901100707164230",tTime);
	pRltCode = new TiXmlElement("Request_Id");
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	pRltCode = new TiXmlElement("Request_Time");
	snprintf(sTmp, sizeof(sTmp), "%10d", 2248988800);
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	TiXmlElement * pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=1;
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	return 0;
}



// ���������ѯ
int PackXMLMsg::packQryTransRuleA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("TransRules_Query")); 
	root->LinkEndChild(pRltCode);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","898901100707164230",tTime);
	pRltCode = new TiXmlElement("Request_Id");
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	pRltCode = new TiXmlElement("Request_Time");
	snprintf(sTmp, sizeof(sTmp), "%10d", 2248988800);
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	TiXmlElement * pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=1;
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	return 0;
}



// ������������
int PackXMLMsg::packResetTransRuleA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	cout<<"������������ TransRules_Reset 10"<<endl;
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("TransRules_Reset")); 
	root->LinkEndChild(pRltCode);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","898901100707164230",tTime);
	pRltCode = new TiXmlElement("Request_Id");
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	pRltCode = new TiXmlElement("Request_Time");
	snprintf(sTmp, sizeof(sTmp), "%10d", 2248988800);
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	TiXmlElement * pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=1;
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=123456;
	pElem = new TiXmlElement("Pay_Password");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=3;
	pElem = new TiXmlElement("Date_Trans_Freq");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=5;
	pElem = new TiXmlElement("Trans_Limit");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=7;
	pElem = new TiXmlElement("Month_Trans_Limit");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=0;
	pElem = new TiXmlElement("Auto_Trans_Flag");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=3;
	pElem = new TiXmlElement("Auto_Balance_limit");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=3;
	pElem = new TiXmlElement("Auto_Amount_limit");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	
	return 0;
}



// 8 ֧��&��½�����޸�
int PackXMLMsg::packResetPassWdA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("Password_Reset")); 
	root->LinkEndChild(pRltCode);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","898901100707164230",tTime);
	pRltCode = new TiXmlElement("Request_Id");
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	pRltCode = new TiXmlElement("Request_Time");
	snprintf(sTmp, sizeof(sTmp), "%10d", 2248988800);
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	TiXmlElement * pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=1;
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	cout<<"�����������"<<endl;
    cin>>iVar;
	//iVar=123456;
	pElem = new TiXmlElement("Old_Password");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	cout<<"������������"<<endl;
    cin>>iVar;
	pElem = new TiXmlElement("New_Password");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	cout<<"������01-�޸�����,02-�½�����"<<endl;
    cin>>sTmp;
	pElem = new TiXmlElement("Action_id");
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
		
	return 0;
}



//10.֧������״̬���
int PackXMLMsg::packResetPayStatusA()
{
    int iRet;
	int iVar;
    char sTmp[256];
	
	TiXmlElement * root = new TiXmlElement("Service_Information");
	m_poXmlDoc->LinkEndChild(root);
	
	TiXmlElement * pRltCode = new TiXmlElement("Service_Context_Id");
	pRltCode->LinkEndChild( new TiXmlText("PayStatus_Reset")); 
	root->LinkEndChild(pRltCode);
	
	time_t tTime;
    tTime = time(NULL);
    snprintf(sTmp,sizeof(sTmp),"%s%010d\0","898901100707164230",tTime);
	pRltCode = new TiXmlElement("Request_Id");
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	pRltCode = new TiXmlElement("Request_Time");
	snprintf(sTmp, sizeof(sTmp), "%10d", 2248988800);
	pRltCode->LinkEndChild( new TiXmlText(sTmp)); 
	root->LinkEndChild(pRltCode);
	
	
	TiXmlElement * pPayInfoB = new TiXmlElement("Micropayment_Info");
	root->LinkEndChild(pPayInfoB);
	
	cout<<"�������û�����"<<endl;
    cin>>sTmp;
	TiXmlElement * pElem = new TiXmlElement("Destination_Account");
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	pElem = new TiXmlElement("Destination_type");
	snprintf(sTmp, sizeof(sTmp), "%d", 1);
	pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	   
	pElem = new TiXmlElement("Destination_Attr");
	snprintf(sTmp, sizeof(sTmp), "%d", 2);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=1;
	pElem = new TiXmlElement("Service_Platform_ID");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=12;
	pElem = new TiXmlElement("Pay_Status");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
	iVar=123456;
	pElem = new TiXmlElement("Pay_Password");
	snprintf(sTmp, sizeof(sTmp), "%d", iVar);
    pElem->LinkEndChild( new TiXmlText(sTmp)); 
	pPayInfoB->LinkEndChild(pElem);
	
		
	return 0;
}





