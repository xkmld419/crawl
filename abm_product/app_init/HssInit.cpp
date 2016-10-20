#include "HssInit.h"
#include "Log.h"
#include "DebugTools.h"
#include "unistd.h"
#include "CommandCom.h"
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <functional>

extern int g_argcex;
extern char ** g_argvex;
#define MAXFILE 65535
int hbInit::m_gServerState=0;
struct sigaction hbInit::m_gsigaction;
const char* SECTION_SYSTEM="SYSTEM";
const char* PARAM_STATE="state";
DEFINE_MAIN_EX(hbInit)

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
	umask(0);
	for(i=0;i<MAXFILE;i++)
		close(i);
	execve(pAppName,argv,environ);
	return 0;
}



bool hbInit::StopServer(void)
{
	if(!InitClient())
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
	cout<<"�����Ĳ��������ļ�"<<endl;
	
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
	if(!readIniString(SECTION_SYSTEM,PARAM_STATE,sSystemState,0))
		return false;
	strState=sSystemState;
	return true;
}

bool hbInit::CheckCfgFile(void)
{
	string strEtcDir = 	m_strTIBSHOME_Path;
	strEtcDir.append("/etc");
	string strBillingCfgFile  =strEtcDir+"/hssconfig";
	string strBillingCfgFileStd = strEtcDir+"/hssconfig.std";
	if(-1== access(strBillingCfgFile.c_str(),F_OK|R_OK|W_OK))
		return false;
	if(-1==access(strBillingCfgFileStd.c_str(),F_OK|R_OK|W_OK))
		return false;
	return true;
}

bool hbInit::CheckNowSysteState(void)
{
	cout<<"��鵱ǰϵͳ״̬"<<endl;
	string strTmp;
	if(m_bChecked){
		if(GetInputYesOrNo()){
			GetNowSystemState(strTmp);
			if(strTmp.compare("online")==0||strTmp.compare("maintenance")==0){
				cout<<"ϵͳ��ǰ״̬����"<<endl;
				return false;
			}
		}else
			return true;
	}else{
		GetNowSystemState(strTmp);
		if(strTmp.compare("online")==0||strTmp.compare("maintenance")==0){
				cout<<"ϵͳ��ǰ״̬����"<<endl;
				return false;	
		}
		return true;
	}
}
int hbInit::run()
{	
	if(!Init())
		return 0;
	if(!PrepareParam())
		return 0;
	string strState;
	int fd1;
	if(!m_bDisplay){
		dup2(1,fd1);
		close(1);
	}
	if(CheckProcessIsStop("initserver")){
		if(CheckAll(m_bChecked)){
		if(!StartService())
			return -1;
		}
	}
	if(!InitClient())
		return false;
	if(m_eMode == MODE_START)
	{
		CheckNowSysteState();
		ShiftServerMode(STATE_ONLINE);
	}if(m_eMode== MODE_MAIN)
	{
		if(CheckNowSysteState())
			ShiftServerMode(STATE_MAIN);
	}
	GetNowSystemState(strState);
	if(!m_bDisplay){
		dup2(fd1,1);
		cout.clear();
	}
	if(strState.empty())
		cout<<"��ǰϵͳ״̬�޷�ʶ��"<<endl;
	else
		cout<<"��ǰϵͳ״̬:"<<strState<<endl;
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
}

hbInit::~hbInit(void)
{
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
	//return true;

	m_strTIBSHOME_Path= getenv( "HSSDIR" );
	if(m_strTIBSHOME_Path.empty())
	{
		//û�����û�������
		m_strTIBSHOME_Path =DEFAULT_ENV_PATH;
	}
	return true;
}

bool hbInit::ShiftServerMode(_SERVICE_STATE_ ServiceState)
{
	//if(!InitClient())
	//	return false;
	if(CheckProcessIsStop("initserver")){
		if(CheckAll(m_bChecked)){
			if(!StartService())
				return -1;
		}
	}
	char sSystemState[64]={0};
	if(!readIniString(SECTION_SYSTEM,PARAM_STATE,sSystemState,0))
		return false;

   if(!readIniString(SECTION_SYSTEM,"mid_state",sSystemState,0))
      return false;

	string strTmp = "SYSTEM.state=";
	if(ServiceState==STATE_MAIN){
		strTmp.append("maintenance");
	}else if(ServiceState==STATE_ONLINE){
		strTmp.append("online");
	}else
		return false;

	changeSectionKey(strTmp);

    string strTmp1 = "SYSTEM.mid_state=";
    if(ServiceState==STATE_MAIN)
    {
       strTmp1.append("maintenance");
    }
    else if(ServiceState==STATE_ONLINE)
    {
        strTmp1.append("online");
    }
    else
       return false;

   changeSectionKey(strTmp1);
   
   
    char sCmd[100] = {0};
    time_t tt = time(NULL);
	 
	 struct tm *p = localtime(&tt);
	 
	 char chargedate[15] = {0};
	 
	 strftime(chargedate,15,"%Y%m%d%H%M%S",p);
	
	//sprintf(chargedate,"%d%d%d%d%d%d",p->tm_year+1900, p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	
	sprintf(sCmd,"SYSTEM.state_date=%s\0",chargedate);
	
	changeSectionKey(sCmd);
 
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
	StartApp("initserver",argvInitServer,strBinDir.c_str());
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
		cout<<"�ȴ���������.........................................."<<c[i]<<flush;
		sleep(1);
		cout<<'\r';
		if(m_gServerState==1)
		{
			cout<<"���������ɹ�"<<endl;
			Log::log(0,"���������ɹ�:%d",0);
			CommandCom *pCmdCom =new CommandCom();
			if(pCmdCom!=0)
			{
				pCmdCom->InitClient();
				string strTmp = "SYSTEM.state=online";
				if(m_eMode==MODE_MAIN)
					strTmp ="SYSTEM.state=maintenace";	
				return pCmdCom->changeSectionKey(strTmp);
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
	if(g_argcex<2)
	{
		m_eMode=MODE_START;
		return true;
	}
	int ch;
	while ((ch = getopt(g_argcex,g_argvex, "vysh")) != -1)
	{
		switch(ch)
		{
			case 'v':
			{ 
				if(m_eMode == MODE_UNKNOW)
				{
				  m_eMode=MODE_START;
				} 
				m_bDisplay=true;
			}break;
		case 'y':
			{
				m_bDisplay=true;
				m_bChecked=true;
			}break;
		case 's':
			{
				m_eMode=MODE_MAIN;
			}break;
		case 'h':
			{
				cout <<" ******************************************************************" << endl;
				cout <<"  ʹ�÷�ʽ:" << endl;
				cout <<" hbinit [-s|-h] [-y] [-v]"<<endl;
				cout <<" ��������:"<<endl;
				cout <<" û�в�����ϵͳ������   on-lineģʽ" << endl;
				cout <<" -s ��ϵͳ������   maintenanceģʽ" << endl;
				cout <<" -v  ��ʾ������� "<<endl;
				cout <<" -h �����������Ի��ʹ�ð���" << endl;
				cout <<" -y ÿһ��������Ҫȷ��" << endl;
				cout <<" ******************************************************************" << endl;
				return 0;
			}break;
		}
	}
	if(MODE_UNKNOW == m_eMode)
	{
		cout<<"�޷�ʶ��Ĳ��� : ";
		for(int i=1;i<g_argcex;i++){
			cout<<g_argvex[i];
		}
		cout<<endl;
		return false;
	}
	return true;
}


