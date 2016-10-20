// Copyright (c) 2011
// All rights reserved.

#include "Process.h"
#include "Log.h"
#include "Exception.h"
#include "ReadIni.h"


int Process::m_iProcID;
int Process::m_iAppID;	
int Process::m_iServeID;
int Process::m_iSysPID;
int Process::m_iFlowID;
int Process::m_iIfNeedBufParse;

char Process::m_sAPP_HOME[200] = {0};
char Process::m_sDictFile[200]={0};
char Process::m_sHOST_NAME[MAX_HOST_NAME_SIZE+1]={0};
int Process::m_iRouteRule;

string Process::m_sSendBuf;
string Process::m_sReceBuf;
	
bool Process::isReceive = false;
long Process::m_lRecvTime = 0;
long Process::m_lSendTime = 0;
OcpMsgParser *Process::m_pOcpMsgParser=NULL;
InnerStruct *Process::m_poInnerStruct=NULL;
	
///CommandCom* Process::m_pCommandCom=0;
ProtoToBusiData Process::m_tProtoData;


AvpPublicMember Process::m_CAvpPublicMember;
CCRPublicMember Process::m_CCCRPublicMember;
CCAPublicMember Process::m_CCCAPublicMember;
CapabilitiesExchangeRequest Process::m_CCapExchReq;
CapabilitiesExchangeAnswer  Process::m_CCapExchAns;
ActiveCCR Process::m_CActiveCCR;
	
SLAUpdateCCR Process::m_CSLAUpdateCCR;
CongestionCCR Process::m_CCongestionCCR;
SASUpdateCCR Process::m_CSASUpdateCCR;
SGWSPACCR Process::m_CSGWSPACCR;
SGWSPACCA Process::m_CSGWSPACCA;
LoadBalanceCCR Process::m_CLoadBalanceCCR;
	
ThreeLogGroup *Process::m_pLogObj = 0;
CommandCom *Process::m_poCmdCom=NULL;
SelfManage *Process::m_pSelfManage = NULL;



Process *g_pProc = 0;
int g_argc;
char ** g_argv;

void AbortProcess(int isignal)
{
	//�����Ϣ���е���Ϣ���յ�SIGTERM�źź��˳���������ҵ�� 
	if (Process::isReceive && isignal == SIGTERM)
		return ;

	Process::ClearProcessSignal();

	int mtmp_iSysPID=getpid();

	////Pprintf(0,1,0,"Ӧ�ó����յ��ź�(%d)�쳣�˳�!!", isignal);
	printf("Ӧ�ó����յ��ź�(%d)�쳣�˳�!!", isignal);
	if (Process::m_iSysPID==mtmp_iSysPID){//���������ӽ��̵�����£�ֻ�и������˳��ǲŵ���
		DisConnectMonitor (WF_ABORT);
		DisconnectTerminalMonitor (ST_ABORT);
		cout<<"������["<<mtmp_iSysPID<<"]�˳�!"<<endl;
	}
	else{
		cout<<"�ӽ���["<<mtmp_iSysPID<<"]�˳�!"<<endl;
	}


	Log::log (0, "Ӧ�ó����յ��ź�(%d)�쳣�˳�--", isignal);

///	Process::m_pProRunMgr->ForceKilled(isignal);//  
	Log::log (0, "Ӧ�ó����յ��ź�(%d)�쳣�˳�--", isignal);
	exit (-1);
}

void StopProcess(int isignal)
{
	//�����Ϣ���е���Ϣ���յ�SIGTERM�źź��˳���������ҵ��  add by panlq 2008-12-06
	if (Process::isReceive && isignal == SIGTERM)
		return ;

	Process::ClearProcessSignal();

	int mtmp_iSysPID=getpid();

	////Pprintf(0,1,0,"Ӧ�ó����˳�!(���Ƚ��̷���)");
	printf("Ӧ�ó����˳�!(���Ƚ��̷���)");
	if (Process::m_iSysPID==mtmp_iSysPID){//���������ӽ��̵�����£�ֻ�и������˳��ǲŵ���
		DisConnectMonitor(WF_STOP);
		DisconnectTerminalMonitor ();
		cout<<"������["<<mtmp_iSysPID<<"]�˳�!"<<endl;
	}else{
		cout<<"�ӽ���["<<mtmp_iSysPID<<"]�˳�!"<<endl;
	}
///	Process::m_pProRunMgr->ForceKilled(isignal);
	Log::log (0, "Ӧ�ó����˳�!(���Ƚ��̷���)");
	exit (0);
}

void NormalExitProcess()
{
		Process::ClearProcessSignal();

		int mtmp_iSysPID=getpid();

		////Pprintf(0,1,0,"Ӧ�ó��������˳���");
		printf("Ӧ�ó��������˳���");
		if (Process::m_iSysPID==mtmp_iSysPID){//���������ӽ��̵�����£�ֻ�и������˳��ǲŵ���
///			DisconnectTerminalMonitor ();
			cout<<"������["<<mtmp_iSysPID<<"]�˳�!"<<endl;
		}else{
			cout<<"�ӽ���["<<mtmp_iSysPID<<"]�˳�!"<<endl;
		}
//		Process::m_pProRunMgr->ForceKilled(0);//  
	  Log::log (0, "Ӧ�ó��������˳�");
		exit(1);
}

//##ModelId=4313CB2501DB
int Process::log(int log_level, char *format,  ...)
{
	va_list ap;
	Log::log (log_level, format, ap);

	return 1;
}

//##ModelId=4313CB2501E2
int Process::warn()
{
	return 0;
}

void Process::SetProcessSignal()
{
	// �����źŴ���
	if (signal(SIGQUIT,AbortProcess) == SIG_ERR) {
		THROW(MBC_Process+4);
	}
	if (signal(SIGTERM,AbortProcess) == SIG_ERR) {
		THROW(MBC_Process+5);
	}
	if (signal(SIGINT,AbortProcess) == SIG_ERR)  {
		THROW(MBC_Process+6);
	}
	if (signal(SIGSEGV,AbortProcess) == SIG_ERR)  {
		THROW(MBC_Process+8);
	}

	if (signal(SIGCHLD,SIG_IGN) == SIG_ERR)  {
		THROW(MBC_Process+8);
	}

	//SIGUSR1 �ź�Ϊǰ̨���Ƚ��淢��ֹͣ���̿�������ʱ,�ɺ�̨�ػ����̲���
	if (signal(SIGUSR1,StopProcess) == SIG_ERR)  {
		THROW(MBC_Process+7);
	}
}

void Process::ClearProcessSignal()
{
	// �����źŴ���
	if (signal(SIGQUIT,SIG_DFL) == SIG_ERR) {
		printf("Can't catch SIGQUIT");
	}
	if (signal(SIGTERM,SIG_DFL) == SIG_ERR) {
		printf("Can't catch SIGTERM");
	}
	if (signal(SIGINT,SIG_DFL) == SIG_ERR)  {
		printf("Can't catch SIGINT");
	}
	if (signal(SIGSEGV,SIG_DFL) == SIG_ERR)  {
		printf("Can't catch SIGSEGV");
	}

	if (signal(SIGCHLD,SIG_DFL) == SIG_ERR)  {
		printf("Can't catch SIGCHLD");
	}

	//SIGUSR1 �ź�Ϊǰ̨���Ƚ��淢��ֹͣ���̿�������ʱ,�ɺ�̨�ػ����̲���
	if (signal(SIGUSR1,SIG_DFL) == SIG_ERR)  {
		printf("Can't catch SIGUSR1");
	}
}

void Process::loadConfig()
{
	char sql[1024]={0};
	DEFINE_QUERY(qry);
	//SQLITEQuery qry;

	/* ȡ���̵� APP_ID, BILLFLOW_ID */

	sprintf (sql, "select a.app_id, a.host_Id, nvl(DICT_FILE, '') DICT_FILE, c.NEED_MSG_PARSE,a.billflow_id \
  						from wf_process a, wf_hostinfo b, wf_application c \
 						where a.host_Id = b.host_Id \
   						and a.APP_ID = c.APP_ID \
   						and a.Process_Id =%d AND B.HOST_NAME='%s' ", m_iProcID, m_sHOST_NAME);
   						
	qry.setSQL(sql);
	qry.open();
	if (qry.next())
	{
		//m_iAppID = qry.getFieldAsInteger(0);
		m_iAppID = qry.field("app_id").asInteger();
		//m_iServeID = qry.getFieldAsInteger(1);
		m_iServeID = qry.field("host_Id").asInteger();
		//strcpy(m_sDictFile,qry.getFieldAsString(2));
		strcpy(m_sDictFile,qry.field("DICT_FILE").asString());
		//m_iIfNeedBufParse = qry.getFieldAsInteger(3);
		m_iIfNeedBufParse = qry.field("NEED_MSG_PARSE").asInteger();
		//m_iFlowID = qry.getFieldAsInteger(4);
		m_iFlowID = qry.field("billflow_id").asInteger();
	}
	else
	{
		THROW(MBC_Process+3);
	}
	qry.close();
}

Process::Process()
{
	
	char m_sStartTime[512];
	char *p, sLogFile[512];
	string str;
/*	
    //ע����Ĳ���
    if(!Process::Register())
        THROW(MBC_initserver+1);
    int iLogLevel =m_pCommandCom->readIniInteger("LOG","log_info_level",-1);
    if(iLogLevel==-1)
    {
        Log::log(0,"���Ĳ�����û��������Ӧ��[LOG].log_info_level��������!��ʱ����Ĭ��ֵΪ2");
        iLogLevel=2;
    }
*/    
	/* PROXY_HOME */
	Process::m_pLogObj = new ThreeLogGroup();
	if ((p=getenv ("APP_HOME")) == NULL){
		cout<<"Please set env value APP_HOME. For example  export APP_HOME=/home/bill/SGW_HOME"<<endl;
		THROW(-1);
	exit(1);
	}
	else
		sprintf (m_sAPP_HOME, "%s", p);

	char *pp;
	if ((pp=getenv ("SGW_HOST_NAME")) == NULL)
	{
		cout<<"Please set env value SGW_HOST_NAME. For example  export ZHJS_HOST_NAME=abm3"<<endl;
		THROW(-1);
	}
	sprintf (m_sHOST_NAME, "%s", pp);

	/* ȡ���̻�����Ϣ:PROCESS_ID */
	if ((m_iProcID = GetProcessID()) == -1) {
		sprintf (sLogFile, "%s/log/process.log", m_sAPP_HOME);
		Log::setFileName (sLogFile);
		THROW(MBC_Process+2);
		exit(1);
	}

	/* LOG_FILE */
	sprintf (sLogFile, "%s/log/process_%d.log", m_sAPP_HOME, m_iProcID);
	Log::setFileName (sLogFile);
	Log::log (0, "--��������--");

	/* SYS PID */
	m_iSysPID = getpid();

	/*��ȡ������Ϣprocess��*/
	loadConfig();

	isReceive = false;
	SetProcessSignal();

	if (!ConnectMonitor() ) 
	{
		cout<<"Process:"<<m_iProcID<<" ConnectMonitor err."<<endl;
	}
	
	/* ���Ӻ�̨�ն�monitor */
	ConnectTerminalMonitor ();
	
	/* �������� */
	Date date;
	strcpy (m_sStartTime, date.toString (NULL));
	
	/* ��ʼ��EventReceiver, EventSender */
	m_poReceiver = new EventReceiver (m_iProcID);
	m_poSender = new EventSender (m_iProcID);

	if (!m_pOcpMsgParser)
		m_pOcpMsgParser= new OcpMsgParser();

	if (!m_poInnerStruct)
		m_poInnerStruct = new InnerStruct();
	
	if (!m_poCmdCom)
		m_poCmdCom = new CommandCom();
	
	if(m_poCmdCom)
		m_poCmdCom->InitClient();	
		
	if (!m_pSelfManage)
		m_pSelfManage = new SelfManage();
			
		
		
	if(0!=SHMSGWInfoBase::attachALL())
	{
		Process::m_pLogObj->sgwlog(MBC_MEM_LINK_FAIL,Process::m_iProcID,Process::m_iSysPID,Process::m_iAppID,LOG_LEVEL_ERROR,LOG_TYPE_BUSI,-1,"","���ӹ����ڴ�ʧ��!");
		throw(MBC_MEM_LINK_FAIL);
	}
		
	m_iRouteRule=0;				
}

//##ModelId=4313CB2501E4
Process::~Process()
{
	Log::log (1000, "Process::~Process()");
		
	if (m_pOcpMsgParser) delete m_pOcpMsgParser;
		Log::log (1000, "delete m_pOcpMsgParser");
			
	if (m_poReceiver) delete m_poReceiver;
		Log::log (1000, "delete m_poReceiver");

	if (m_poSender) delete m_poSender;
		Log::log (1000, "delete m_poSender");	
					
	if (m_poInnerStruct) delete m_poInnerStruct;
		Log::log (1000, "delete m_poInnerStruct");
			
	if(m_pLogObj != NULL)
	{		
		delete m_pLogObj;
		m_pLogObj = 0;
	}
	
	if(m_pSelfManage !=NULL)
	{
		delete m_pSelfManage;
		m_pSelfManage = 0;
	}
	
	
	
	DisconnectTerminalMonitor ();
	Log::log (1000, "DisConnectTerminalMonitor");			
	
	Log::log (0, "--����ִ�н����˳�(����)--");						
}

//��ȡ��ǰӦ�ý���ID
//δ��ȡ��ʱ,���� -1 .
int Process::GetProcessID()
{
    char *pt;
    
    if ((pt = getenv ("PROCESS_ID")) == NULL) {
        printf ("[ERROR]: getenv(PROCESS_ID) == NULL, "
                "��ȡ��������PROCESS_IDʧ��" );
        return -1;
    }

    int g_iProcessID = atoi (pt);
    
    printf ("[MESSAGE]: Envionment variable: PROCESS_ID = %d\n",
            g_iProcessID);
    
    return g_iProcessID;
}

/*
CmdMsg* Process::GetMsg(long lType)
{
	if(m_pCommandCom==0)
		return 0;
	return m_pCommandCom->GetMsg(lType);
}

bool Process::Register()
{
	//����0�Ž��̶�Ӧ����Ϣ����
	if(0 == m_pCommandCom)
	{
		try{
			m_pCommandCom = new CommandCom;
		}catch (bad_alloc& e)
		{
			badalloc(e);
		}
	}
	if(0== m_pCommandCom)
		return false;
	if(!m_pCommandCom->InitClient())
	{
		delete m_pCommandCom; m_pCommandCom=0;
		return false;
	}
	return true;
}

bool Process::InitService(void)
{
	if(0 == m_pCommandCom)
	{
		try{
			m_pCommandCom = new CommandCom;
		}catch (bad_alloc& e)
		{
			badalloc(e);
		}
	}
	if(0== m_pCommandCom)
		return false;
	return m_pCommandCom->InitService();
}

bool Process::SubscribeCmd(string& strUrl,long iCmd)
{
	//��ע���ԤԼ
	//ԤԼ�����Ҫ�ػ��ź�
	if(0 == m_pCommandCom)
		return false;
	return m_pCommandCom->SubscribeCmd(strUrl,iCmd);
}

bool Process::GetSysParamInfo()
{

    return true;
}
*/
