/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "Log.h"
#include "Exception.h"
#include "ReadIni.h"
#include "InDependProcess.h"



char InDependProcess::m_sTIBS_HOME[80] = {0};
char InDependProcess::sLockPath[100] = {0};
int InDependProcess::m_iSysPID=0;
int g_argcex;
char ** g_argvex;

void AbortProcessEx(int isignal)
{
	Pprintf (9001,1,0, "Ӧ�ó����յ��ź�(%d)�쳣�˳�!!", isignal);
	Log::log (0, "Ӧ�ó����յ��ź�(%d)�쳣�˳�--", isignal);

	exit (-1);
}

void StopProcessEx(int isignal)
{
	Pprintf(0,1,0,"Ӧ�ó����˳�!(���Ƚ��̷���)");
	Log::log (0, "Ӧ�ó����˳�!(���Ƚ��̷���)");

	exit (0);
}


//##ModelId=41E1DD3D03E2
int InDependProcess::log(int log_level, char *format,  ...)
{
	va_list ap;
	Log::log (log_level, format, ap);
	return 1;
}

int InDependProcess::run_onlyone(const char *filename)
{
	int  fd, val;
	char buf[10];
	memset(buf, 0, sizeof(buf));

	if ((fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0){
        //ALARMLOG28(0,MBC_CLASS_File,101,"���ļ�ʧ��: File[%s]",filename);
        if(errno == ENOENT)
            //ALARMLOG28(0,MBC_CLASS_File,106,"ָ���ļ�������: File[%s]",filename);
		return -1;
	}

	// try and set a write lock on the entire file
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;

	if (fcntl(fd, F_SETLK, &lock) < 0)
	{
		if (errno == EACCES || errno == EAGAIN)
			exit(0);     // gracefully exit, daemon is already running
		else
			return -1;
	}

	// truncate to zero length, now that we have the lock
	if (ftruncate(fd, 0) < 0)
		return -1;

	// and write our process ID
	sprintf(buf, "%d\n", getpid());
	if (write(fd, buf, strlen(buf)) != (int)strlen(buf))
		return -1;

	// set close-on-exec flag for descriptor
	if ( (val = fcntl(fd, F_GETFD, 0)) < 0)
		return -1;
	val |= FD_CLOEXEC;
	if (fcntl(fd, F_SETFD, val) < 0)
		return -1;

	// leave file open until we terminate: lock will be held

	return 0;
}


//##ModelId=41E1DD440067
int InDependProcess::warn()
{
	return 0;
}

bool InDependProcess::Init(char *sProcessName)
{
	if(!sProcessName)
		return false;

	char *p =  NULL; 
	/* SYS PID */
	m_iSysPID = getpid();

	char sLogFile[256], sSqlcode[256];
	memset(sLogFile, 0, sizeof(sLogFile));
	memset(sSqlcode, 0, sizeof(sSqlcode));

	/* TIBS_HOME */

	if ((p=getenv ("ABM_PRO_DIR")) == NULL){
				//ALARMLOG28(0,MBC_CLASS_Fatal, 14,"%s","��������δ����[TIBS_HOME]");
                sprintf (m_sTIBS_HOME, "/home/bill/SGW_HOME");
         } else {
                sprintf (m_sTIBS_HOME, "%s", p);
		}
	/* ���˽�������*/

	string strProcessName=sProcessName;
	int nPos=strProcessName.find_last_of('/');
	if(-1 !=nPos)
		strProcessName.erase(0,nPos+1);
#ifdef	ONLY_ONE_PROCESS
	sprintf(sLockPath, "%s/lockpath/%s", m_sTIBS_HOME,strProcessName.c_str());
#else
	sprintf(sLockPath, "%s/lockpath/%s_%d", m_sTIBS_HOME
	,strProcessName.c_str(),m_iSysPID);
#endif

	char sCmd[1000];
	memset(sCmd, 0, sizeof(sCmd));
	sprintf (sCmd,"mkdir -p %s/lockpath",m_sTIBS_HOME);
	system(sCmd);

	if (run_onlyone(sLockPath)<0) 
	{
		Log::log (0, "--��������ʧ��--");
		THROW(MBC_Process+3);	
	}

	/* LOG_FILE */
#ifdef	ONLY_ONE_PROCESS
	sprintf (sLogFile, "%s/log/process_%s.log", m_sTIBS_HOME, strProcessName.c_str());
#else
	sprintf (sLogFile, "%s/log/%s_%d.log", m_sTIBS_HOME
	, strProcessName.c_str(),m_iSysPID);
#endif
	Log::setFileName (sLogFile);

	Log::log (0, "--��������--");


	// �������̵��Ⱥ�̨�ػ����
#ifdef ATTR_FORMAT
	sleep(60);
#endif

	{ // �����źŴ���
		if (signal(SIGQUIT,AbortProcessEx) == SIG_ERR) {
			Pprintf(ALERT_SIG_ERR,1,0,"Can't catch SIGQUIT"); 
			THROW(MBC_Process+4);	
		}
		if (signal(SIGTERM,AbortProcessEx) == SIG_ERR) {
			Pprintf(ALERT_SIG_ERR,1,0,"Can't catch SIGTERM"); 
			THROW(MBC_Process+5);	
		}
		if (signal(SIGINT,AbortProcessEx) == SIG_ERR)  {
			Pprintf(ALERT_SIG_ERR,1,0,"Can't catch SIGINT"); 
			THROW(MBC_Process+6);	
		}
		if (signal(SIGSEGV,AbortProcessEx) == SIG_ERR)  {
			Pprintf(ALERT_SIG_ERR,1,0,"Can't catch SIGSEGV"); 
			THROW(MBC_Process+8);	
		}
		signal (SIGILL, AbortProcessEx);
		signal (SIGABRT, AbortProcessEx);
		signal (SIGFPE, AbortProcessEx);
		signal (SIGPIPE, AbortProcessEx);


		//SIGUSR1 �ź�Ϊǰ̨���Ƚ��淢��ֹͣ���̿�������ʱ,�ɺ�̨�ػ����̲���
		if (signal(SIGUSR1,StopProcessEx) == SIG_ERR)  {
			Pprintf(ALERT_SIG_ERR,1,0,"Can't catch SIGUSR1"); 
			THROW(MBC_Process+7);	
		}
	}


	/* �������� */
	Date date;
	strcpy (m_sStartTime, date.toString (NULL));
	return true;
}
//##ModelId=41ECE8A80158
InDependProcess::InDependProcess()
{

}

//##ModelId=41F6035A031A
InDependProcess::~InDependProcess()
{
	Log::log (1000, "InDependProcess::~Process()");

	Log::log (0, "--����ִ�н����˳�(����)--");
}



