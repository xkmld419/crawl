#include "sgwInit.h"
#include "Log.h"
#include "unistd.h"
#include "CommandCom.h"
#include "Date.h"
#include "MBC_ABM.h"

#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <functional>

#include "GlobalTools.h"
extern int g_argc;
extern char ** g_argv;
#define MAXFILE 65535
int hbInit::m_gServerState=0;
struct sigaction hbInit::m_gsigaction;
const char* SECTION_SYSTEM="SYSTEM";
const char* PARAM_STATE="state";
DEFINE_MAIN(hbInit)
using namespace StandardDevelopMent;
bool CheckProcessIsStop(pid_t pid)
{
	if(pid==0)
		return true;
	if((kill(pid, 0) == -1 && errno == ESRCH))
		return true;
	return false;
};

bool CheckProcessIsStop(const char* cstrAppName)
{
	if(0== cstrAppName)
		return true;//�����ڱ�ʾ�Ѿ�ֹͣ
	char sCheckShell[256]={0};
	sprintf(sCheckShell," ps -ef|grep %s |grep -iv 'grep'| "
		" awk '{print $1,$2}' |grep  `whoami` |awk '{print $2}' ",cstrAppName);
	FILE* fd = popen(sCheckShell,"r");
	if(fd==0)
		return true;
	if(feof(fd)==EOF)
		return true;
	char sPid[32]={0};
	if(fscanf(fd,"%s",sPid)==EOF)
		return true;
	pclose(fd);
	return CheckProcessIsStop(atoi(sPid)); 
};

int StartApp(const char* pAppName,char* argv[32],const char* pBinDir)
{
	char tmp[1024] = {0};
	sprintf(tmp,"%s/%s",pBinDir,pAppName);
	if(-1 == access(tmp,X_OK|F_OK|R_OK|W_OK)){
		//ALARMLOG28(0,MBC_CLASS_Fatal,5,"��������ʧ��:%s",pAppName);
		Log::log(0,"��������ʧ��:%s",pAppName);
		return -1;
	}
	//�Ҳ�֪�������ܷ�����ڶ��߳������ĳ����������
	//������˷��������� ... 
	//����������: ���߳��л᲻�ɱ����ʹ�õ���������string
	//���̲߳���������ʱ�ֵ�����fork�ǰ���unix��дʱ����ԭ���µĽ��������״̬
	//Ҳ����ס�ģ���û���˶����ر�
	pid_t pid;
	int i;
	pid=fork();
	if(pid<0){
		printf("error in fork\n");
		exit(1);
	}else if(pid>0){
		return pid;
	}
	sleep(1);
	setsid();
	
	if(pBinDir==0)
		chdir("/");
	else
		chdir(pBinDir);
/*		
	umask(0);
	for(i=0;i<MAXFILE;i++)
		close(i);
*/
	execve(pAppName,argv,environ);
	return 0;
}



bool hbInit::StopServer(void)
{
	if(!pCommandCom->InitClient())
		return false;
	return true;
}

bool hbInit::GetInputYesOrNo(void)
{
	cout<<"y/n: ";
	string strGet;
	cin >>strGet;
	transform(strGet.begin(),strGet.end(),strGet.begin(),::tolower);
	if(!strGet.compare("yes")||!strGet.compare("y"))
		return true;
	return false;
}
bool hbInit::CheckAll(bool bConfirm)
{
	bool bGet=true;
	if(m_bDisplay){
	cout<<"�����Ĳ��������ļ�"<<endl;
	}	
	if(bConfirm)
		bGet =GetInputYesOrNo();
	if(bGet){
		if(!CheckCfgFile())
			return false;
	}
	return true;
}

bool hbInit::GetNowSystemState(string& strState)
{
	char sSystemState[64]={0};
	if(!pCommandCom->readIniString(SECTION_SYSTEM,PARAM_STATE,sSystemState,0))
		return false;
	strState=sSystemState;
	return true;
}

bool hbInit::CheckCfgFile(void)
{
	string strBillingCfgFile ;
	StandardTools::GetStdConfigFile(strBillingCfgFile);
	string strBillingCfgFileStd = strBillingCfgFile+".std";
	if(-1== access(strBillingCfgFile.c_str(),F_OK|R_OK|W_OK)) {
		char sLogMsg[256] = {0};
    sprintf(sLogMsg,"������Ϊ101:�ļ������ڻ����ļ����ɶ�д, �����ļ� :%s",strBillingCfgFile.c_str());
		Log::log(0,"%s",sLogMsg);			
		m_bAccess = true;
		return false;
	}
	if(-1==access(strBillingCfgFileStd.c_str(),F_OK|R_OK|W_OK))
		return false;
	return true;
}
 
bool hbInit::CheckNowSysteState(void)
{
	if(m_bDisplay)
	cout<<"��鵱ǰϵͳ״̬"<<endl;
	string strTmp;	
	
	char sHome[254] = {0};
	char sTemp[256] = {0};
	char * p = NULL;
	if ((p=getenv ("ABM_PRO_DIR")) == NULL)
	  	sprintf (sHome, "/home/bill/ABM_HOME");
	else
	  	sprintf (sHome, "%s", p);
  char sFile[254];
	if (sHome[strlen(sHome)-1] == '/') {
	  sprintf (sFile, "%setc/abmconfig", sHome);
	} else {
	  sprintf (sFile, "%s/etc/abmconfig", sHome);
	}		

	
	
	if(m_bChecked){
		if(GetInputYesOrNo()){
			GetNowSystemState(strTmp);
			if(strTmp == "")
			{
					ReadIni mIni;
					mIni.readIniString (sFile, "system", "state", sTemp,"offline");
					strTmp = sTemp;
			}
			if(strTmp.compare("online")==0||strTmp.compare("maintenance")==0){
				if(m_bDisplay)
				cout<<"ϵͳ��ǰ״̬����"<<endl;
				return false;
			}
		}else
			return true;
	}else{
		GetNowSystemState(strTmp);
		if(strTmp == "")
		{
				ReadIni mIni;
				mIni.readIniString (sFile, "system", "state", sTemp,"offline");
				strTmp = sTemp;
		}		
		if(strTmp.compare("online")==0||strTmp.compare("maintenance")==0){
				if(m_bDisplay)
				cout<<"ϵͳ��ǰ״̬����"<<endl;
				return false;	
		}
		return true;
	}
}

bool hbInit::UnLoadAllMen()
{
	int iOK = -1;
	//iOK = SHMCmdSet::unloadSHMALL();
	if(iOK == 0) {
		return true;
	}
		return false;
}


bool hbInit::CreateLogMem()
{
	/*   ��ʼ��������ʼ��������־��  */
	if(CommandCom::m_poLogg ){
		delete CommandCom::m_poLogg; 
		CommandCom::m_poLogg=0;
	}
	m_poLoggMgr = new ThreeLogGroupMgr();
	if(!m_poLoggMgr)
		{
		  Log::log(0,"%s","������־�鴴��ʧ��");
		  return false;
		}
	if(!m_poLoggMgr->init())
		{
		  Log::log(0,"%s","������־�鴴��ʧ��");
		  return false;
		}			
	 if(!m_poLoggMgr->create())
		{
		  Log::log(0,"%s","������־�鴴��ʧ��");
		  return false;
		}	
	delete m_poLoggMgr; 
	m_poLoggMgr=NULL;	
		if(!CommandCom::m_poLogg )
		{
			CommandCom::m_poLogg = new ThreeLogGroup(true);
				if(!CommandCom::m_poLogg )
				{
		     Log::log(0,"%s","������־���ʼ��ʧ��");
		     return false;
		   }
		}			
		CommandCom::m_poLogg->InitLogGroup();
		CommandCom::m_poLogg->m_iLogProid=0;
		CommandCom::m_poLogg->m_iLogAppid=0;
		/*   ������־��  �������  */		
		
		/*if(CommandCom::m_poLogg){
			delete CommandCom::m_poLogg; 
		  CommandCom::m_poLogg=0;
		}
		
		CommandCom::m_poLogg = new ThreeLogGroup();
			if(!CommandCom::m_poLogg )
				{
		     Log::log(0,"%s","������־���ʼ��ʧ��");
		     return false;
		   }*/
		return true;
}



bool hbInit::LoadData()
{
	char sHome[254] = {0};
	char sBatch[256] = {0};
	char * p = NULL;
	if ((p=getenv ("ABM_PRO_DIR")) == NULL)
	  	sprintf (sHome, "/home/bill/ABM_HOME");
	else
	  	sprintf (sHome, "%s", p);
  char sFile[254];
	if (sHome[strlen(sHome)-1] == '/') {
	  sprintf (sFile, "%setc/abmconfig", sHome);
	} else {
	  sprintf (sFile, "%s/etc/abmconfig", sHome);
	}		
	ReadIni mIni;
	mIni.readIniString (sFile, "CHECKPOINT", "log_checkpoint_batch", sBatch,NULL);	
  CheckPointMgr *pCheckPointMgr = new CheckPointMgr();
  if(pCheckPointMgr == NULL) {	  
		return false;
	}
	pCheckPointMgr->touchInfoLoad(sBatch);
	if(pCheckPointMgr!=NULL) {
		delete pCheckPointMgr;
		pCheckPointMgr = NULL;
	}
	return true;
}

int hbInit::run()
{	
	char sLogMsg[2048] = {0};			
	
	int iBootService = 0;
	
	if(!Init()) {		
		return 0;
	}
	if(!CheckCfgFile())
			return false;	
	if(!PrepareParam())
		return 0;
	string strState;
	int fd1;
/*	if(!m_bDisplay){
		dup2(1,fd1);
		close(1);
	}*/
	if(CheckProcessIsStop("initserver")||CheckNowSysteState()){		
		Log::log(0,"��ʼ������־�ڴ�");
	  bool bCreateOK = CreateLogMem();
	  if(!bCreateOK) {
		  Log::log(0,"������־�ڴ�ʧ��");
		  return 0;
	  }
	  Log::log(0,"��ʼ����ͳ���ڴ�");
		/**
		bCreateOK = StatAllInfoMgr::CreateStatShm();
	  if(!bCreateOK) {
		  Log::log(0,"����ͳ���ڴ�ʧ��");
		  return 0;
	  }
	  StatAllInfoMgr::ClearStatShm();
	  **/
		
		/*ҵ����̵��ڴ洴��*/
	
	int cCreateOK = 0;
	printf("�Ƿ���Ҫж�����е�ҵ���ڴ� 1:��, ��������\n");
	scanf("%d",&cCreateOK);
	if(cCreateOK == 1) {
		Log::log(0,"��ʼж������ҵ���ڴ�");
		UnLoadAllMen();
		Log::log(0,"ҵ���ڴ�ж�ؽ���");
	}
	printf("�Ƿ���Ҫ�������е�ҵ���ڴ� 1:��, ��������\n");
	scanf("%d",&cCreateOK);
	if(cCreateOK == 1) {
		Log::log(0,"��ʼ��������ҵ���ڴ�");
		Log::log(0,"ҵ���ڴ洴������");
	}
	
	int cLoadOK = 0;
	printf("�Ƿ���Ҫ���¼���ҵ������ 1:�� ��������\n");
	scanf("%d",&cLoadOK);
	if(cLoadOK==1) {		
		Log::log(0,"��ʼ���¼���ҵ������:");			
		bool bRet = LoadData();
		if(!bRet) {
			Log::log(0,"����ҵ������ʧ��");
			return 0;
		}
		Log::log(0,"ҵ�����ݼ��ؽ���!");
	}
	
	if(CheckProcessIsStop("initserver")){
		if(CheckAll(m_bChecked)){
			if(!StartService())
				return -1;
			else
			{
				iBootService = 1;
			}
		}
	}
	}else {
		CheckCfgFile();
	}
	if( iBootService == 1 )
	{
		Log::log(0,"�ȴ�1�Ž�������!");
		for(int i = 0;i<10;i++)
		{
			//for(int n = 0;n<i;n++)
			cout<<'.';
			sleep(1);	
		}
		cout<<'\r';
	}
	
	if(m_bAccess) {
		return -1;
	}
	
  if(	pCommandCom == NULL) {
  	pCommandCom = new CommandCom();
  }		
	if(!pCommandCom->InitClient())
		return false;
		
  if(	m_pLogg == NULL) {
  	m_pLogg = new ThreeLogGroup();
  }
  
  
	if(m_eMode == MODE_START)//-v �򲻼Ӳ�������
	{
		bool bTmp1 = CheckNowSysteState();
		bool bTmp = false;
		if(bTmp1)
			bTmp = ShiftServerMode(STATE_ONLINE);
		if(m_bOK) {
			Date d;
			if(bTmp){
			   sprintf(sLogMsg,"�û�:%s,��ʱ���:%s ������abminit �Cv������ʾabminit�Ĵ�����̲����ɹ���",m_strUserName.c_str(),d.toString());
			   pCommandCom->reLoad();
			   m_pLogg->log(MBC_DISPLAY_SYSTEM,LOG_LEVEL_INFO,LOG_TYPE_SYSTEM,sLogMsg);
		  }else {
		  	sprintf(sLogMsg,"�û�:%s,��ʱ���:%s ������abminit �Cv������ʾabminit�Ĵ�����̲���ʧ�ܡ�",m_strUserName.c_str(),d.toString());
		  	m_pLogg->log(MBC_DISPLAY_SYSTEM,LOG_LEVEL_WARNING,LOG_TYPE_SYSTEM,sLogMsg);
		  }
		  Log::log(0,"%s",sLogMsg);
		  
		}
		
		if(m_bShiftOK) {
			Date d;
			if(bTmp1) {
			  if(bTmp){
			    sprintf(sLogMsg,"�û�:%s,��ʱ���:%s ������abminit �����л�ϵͳ״̬��offlineģʽ��Ϊonline�������ɹ���",m_strUserName.c_str(),d.toString());
					pCommandCom->reLoad();
					m_pLogg->log(MBC_DISPLAY_SYSTEM,LOG_LEVEL_INFO,LOG_TYPE_SYSTEM,sLogMsg);
		    }else {
		  	  sprintf(sLogMsg,"�û�:%s,��ʱ���:%s ������abminit �����л�ϵͳ״̬��offlineģʽ��Ϊonline������ʧ�ܡ�",m_strUserName.c_str(),d.toString());
		  	  m_pLogg->log(MBC_DISPLAY_SYSTEM,LOG_LEVEL_WARNING,LOG_TYPE_SYSTEM,sLogMsg);
		    }
		    Log::log(0,"%s",sLogMsg);
		    
		}else {
			sprintf(sLogMsg,"��ǰ״̬��Ϊofflineģʽ������ģʽ");
			Log::log(0,"%s",sLogMsg);
			m_pLogg->log(MBC_DISPLAY_SYSTEM,LOG_LEVEL_WARNING,LOG_TYPE_SYSTEM,sLogMsg);
		}
	}
	}if(m_eMode== MODE_RELOAD)  //-r���� 
	{
		pCommandCom->reLoad();
	}if(m_eMode== MODE_MAIN)  //-s���� 
	{
		bool bTmp1 = false;
		if(CheckNowSysteState())
			bTmp1 = ShiftServerMode(STATE_MAIN);		
		Date d;
		if(bTmp1) {
		  sprintf(sLogMsg,"�û�:%s,��ʱ���:%s ������abminit �Cs�����л�ϵͳ״̬��offlineģʽ��Ϊmaintenance�������ɹ���",m_strUserName.c_str(),d.toString());
			pCommandCom->reLoad();
			m_pLogg->log(MBC_SWITCH_SYSTEM,LOG_LEVEL_INFO,LOG_TYPE_SYSTEM,sLogMsg);		
		}else {
			sprintf(sLogMsg,"�û�:%s,��ʱ���:%s ������abminit �Cs�����л�ϵͳ״̬��offlineģʽ��Ϊmaintenance������ʧ�ܡ�",m_strUserName.c_str(),d.toString());
			m_pLogg->log(MBC_SWITCH_SYSTEM,LOG_LEVEL_WARNING,LOG_TYPE_SYSTEM,sLogMsg);		
		}
		Log::log(0,"%s",sLogMsg);
		m_pLogg->log(MBC_SWITCH_SYSTEM,LOG_LEVEL_INFO,LOG_TYPE_SYSTEM,sLogMsg);		
	}
	GetNowSystemState(strState);
	/*if(!m_bDisplay){
		dup2(fd1,1);
		cout.clear();
	}*/
	if(strState.empty()){
		if(m_bDisplay)
		cout<<"��ǰϵͳ״̬�޷�ʶ��"<<endl;
	}
	else
	{
		if(m_bDisplay)
		cout<<"��ǰϵͳ״̬:"<<strState<<endl;
	}
	return 0;
}

void hbInit::OnSignal(int signum,siginfo_t *info,void *myact)
{
	if((info!=0)&&(info->si_value.sival_int==1))
	{
		//�����ɹ�
		m_gServerState=1;
	}
	if((info!=0)&&(info->si_value.sival_int==2))
	{
		//����ʧ��
		m_gServerState=2;
	}
	if((info!=0)&&(info->si_value.sival_int==7))
	{
		m_gServerState=7;
		cout<<'\r';
		cout<<"����1�Ž���................................."<<endl;
	}
	if(m_gServerState==1)
	{
		cout<<'\r';
		cout<<"InitServer Start......................................OK"<<endl;
	}
	//���ɹ���Ҳ������
	sigaction(signum,&m_gsigaction,NULL) ;
}

hbInit::hbInit(void)
{
	m_eMode=MODE_UNKNOW;
	m_gServerState =0;
	m_bChecked=false;
	m_bDisplay =false;
	m_bOK = false;
	m_bShiftOK = false;
	m_bAccess = false;
	m_pLogg = NULL;
	pCommandCom = NULL;
	struct passwd pwd;
          struct passwd *result;
         char logBuffer[1024]={0};
           char pwdBuffer[1024]={0};

           if (getlogin_r (logBuffer, 1024) == 0)
             if (getpwnam_r (logBuffer, &pwd, pwdBuffer, 1024, &result) == 0){
			m_strUserName=pwd.pw_name;
	}
}

hbInit::~hbInit(void)
{
	/*if(m_poLoggMgr!=NULL) {
		delete m_poLoggMgr;
		m_poLoggMgr = NULL;
	}*/
}

bool hbInit::Init()
{
	//��ʼ
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	sigemptyset(&m_gsigaction.sa_mask);
	act.sa_flags=SA_SIGINFO;
	act.sa_sigaction=OnSignal;
	if(sigaction(SIGUSR1,&act,&m_gsigaction) < 0)
	{
		return false;
	}

	m_strTIBSHOME_Path= getenv( ENV_HOME );
	if(m_strTIBSHOME_Path.empty())
	{
		//û�����û�������
		m_strTIBSHOME_Path =DEFAULT_ENV_PATH;
	}
	return true;
}

bool hbInit::ShiftServerMode(_SERVICE_STATE_ ServiceState)
{
	if(!pCommandCom->InitClient())
		return false;
	if(CheckProcessIsStop("initserver")){
		if(CheckAll(m_bChecked)){
			if(!StartService())
				return false;
		}
	}
	char sSystemState[64]={0};
	if(!pCommandCom->readIniString(SECTION_SYSTEM,PARAM_STATE,sSystemState,0))
		return false;
 
	string strTmp = "SYSTEM.state=";
	string sTmp = "SYSTEM.billing_online_time=";
	string sStateDate = "SYSTEM.state_date=";
	if(ServiceState==STATE_MAIN){
		strTmp.append("maintenance");
	}else if(ServiceState==STATE_ONLINE){
		strTmp.append("online");						
	}else
		return false;

	pCommandCom->changeSectionKey(strTmp);	
	
	Date d;	
	sStateDate.append(d.toString());
  pCommandCom->changeSectionKey(sStateDate);	
	
	if(ServiceState==STATE_ONLINE){
		strTmp="SYSTEM.mid_state=online";
		pCommandCom->changeSectionKey(strTmp);

		sTmp.append(d.toString());
		pCommandCom->changeSectionKey(sTmp);
		Log::log(0,"ϵͳ�л���online��ʱ��Ϊ%s",d.toString());	
	}
	else if(ServiceState==STATE_MAIN){
		strTmp="SYSTEM.mid_state=maintenance";
		pCommandCom->changeSectionKey(strTmp);

		//sTmp.append(d.toString());
		//changeSectionKey(sTmp);
		Log::log(0,"ϵͳ�л���maintenance��ʱ��Ϊ%s",d.toString());	
	}
	return true;
}


bool hbInit::StartService(void)
{
	string strBinDir = m_strTIBSHOME_Path;
	strBinDir.append("/bin");
	char sPid[32]={0};
	sprintf(sPid,"%d",getpid());
	char* argvInitServer[32]={"initserver",0,"-service",0};
	argvInitServer[1]=sPid;
	if(0>StartApp("initserver",argvInitServer,strBinDir.c_str()))
		return false;
	return TestServerActiveSignal(240);
}

bool hbInit::TestServerActiveSignal(int iTimes)
{
	iTimes=iTimes;
	char* c[3]={"/ ","--","\\ "};
	int j =0;	
	while(j++<iTimes)
	{
		int i=j%3;
		if(m_bDisplay){
		cout<<"�ȴ���������.........................................."<<c[i]<<flush;
		}
		sleep(1);
		if(m_bDisplay){	
		cout<<'\r';
		}
		if(m_gServerState==1)
		{
			if(m_bDisplay){
			cout<<"���������ɹ�"<<endl;
			}
			Log::log(0,"���������ɹ�:%d",0);
			CommandCom *pCmdCom =new CommandCom();
			if(pCmdCom!=0)
			{
				pCmdCom->InitClient();
				return true;
			}		
			return true;
		}
		if(m_gServerState==2)
		{
			//SetProcessState(2);
			Log::log(0,"��������ʧ��:%d",0);
			return false;
		}
	}
	cout<<endl;	
	Log::log(0,"����������ʱ,��Ҳ��֪������û������:%d",0);
	return false;
}

bool hbInit::PrepareParam(void)
{
	m_bOK = false;
	m_bShiftOK = false;
	m_bShiftOK = true;
	if(g_argc<2)
	{
		m_eMode=MODE_START;
		m_bShiftOK = true;
		m_bDisplay=true;
		//m_bChecked=true;
		return true;
	}
	int ch;
	
	while ((ch = getopt(g_argc,g_argv, "vysicr?h")) != -1)
	{
		m_eMode=MODE_START;
		switch(ch)
		{
		case 'c':
			{
				m_eMode=MODE_DOWN;
			}break;
		case 'y':
			{
				m_bDisplay=true;
				m_bChecked=true;
			}break;
		case 'i':
			{
				m_eMode=MODE_START;
			}break;
		case 'v':
			{
				m_eMode=MODE_START;
				m_bShiftOK = false;
				m_bDisplay=true;
				m_bOK =true;
			}break;
		case 'r':
			{
				m_eMode=MODE_RELOAD;
			}break;
		case 's':
			{
				m_eMode=MODE_MAIN;
			}break;
		case '?':
		case 'h':
			{
				cout <<" ******************************************************************" << endl;
				cout <<"  ʹ�÷�ʽ:" << endl;
				cout <<" abminit [-s|-h] [-r] [-v]"<<endl;
				cout <<" ��������:"<<endl;
				cout <<" û�в�����ϵͳ������onlineģʽ" << endl;
				cout <<" -s ��ϵͳ������maintenanceģʽ" << endl;
				//cout <<" -i ��ϵͳ������  online ģʽ"<<endl;
				//cout <<" -v ��ʾ������� "<<endl;
				cout <<" -h �����������Ի��ʹ�ð���" << endl;
				cout <<" -r ����0�Ž��̣����²����ڴ� "<<endl;
				//cout <<" -y ÿһ��������Ҫȷ��" << endl;
				cout <<" ******************************************************************" << endl;
				return 0;
			}break;
		}
	}
	if(MODE_UNKNOW == m_eMode)
	{
		cout<<"�޷�ʶ��Ĳ��� : ";
		for(int i=1;i<g_argc;i++){
			cout<<g_argv[i];
		}
		cout<<endl;
		return false;
	}
	return true;
}


