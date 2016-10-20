#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include "LogV2.h"
#include "Application.h"

using namespace std;

#define ABM_HOME "ABMAPP_DEPLOY"

int g_iProcID = -1;
int g_iNodeID = -1;
char g_sAppName[32] = {0}; 
volatile sig_atomic_t g_sigReceived = 0;

bool ReceiveStop(void);
void SigHandler(int sig);

void SigHandler(int sig)
{
	g_sigReceived = sig;
}//modify by nwp 060310

bool ReceiveStop(void)
{
	return (g_sigReceived==0)? false: true;
}



int SigHandlerSet(int sig, void (*handler)(int))
{
    struct sigaction new_action;
    struct sigaction old_action;
    
    memset(&new_action, 0x00, sizeof(new_action));
    memset(&old_action, 0x00, sizeof(old_action));

    new_action.sa_handler = handler;
    sigemptyset(&new_action.sa_mask);
    sigfillset (&new_action.sa_mask);

    if (sigaction(sig, &new_action, &old_action) != 0) 
    	return -1;	
    
    return 0;
}

int HandleSigactions()
{
    //����SIGHUP, SIGINT, SIGQUIT
    sigset_t mask_action;
    sigemptyset(&mask_action);
    sigaddset(&mask_action, SIGHUP);
    sigaddset(&mask_action, SIGINT);
    sigaddset(&mask_action, SIGQUIT);
    /*SOCK_STREAM socket, ��һ���Զ������ر� ���߱��˹رյ�fd
     ��������ʱ, �����SIGPIPE�ź�*/
    sigaddset(&mask_action, SIGPIPE);
    if (sigprocmask(SIG_BLOCK, &mask_action, 0) != 0) {
    	perror("sigprocmask");
    	return -1;	
    }
    //��ȫ�˳��ź�
    if (SigHandlerSet(SIGTERM, SigHandler) != 0) 
    	return -1;	

    return 0;	

}


Application::Application()
{
	memset(m_sEnvAppDly, 0x00, sizeof(m_sEnvAppDly));
	memset(m_sHostName, 0x00, sizeof(m_sHostName));	
}

int Application::appInit()
{
    //��ʼ�����л���
    char *p = getenv(ABM_HOME);
    if (p == NULL) {
        cout<<"��������"<<ABM_HOME<<"û������,�����ʼ��ʧ�� ��"<<endl;
        return -1;
    }
    strncpy(m_sEnvAppDly, p, sizeof(m_sEnvAppDly)-2);
    if (m_sEnvAppDly[strlen(m_sEnvAppDly)-1] != '/') 
        m_sEnvAppDly[strlen(m_sEnvAppDly)] = '/';
    
	//��־���ʼ��
    char sLogFile[256];
	snprintf(sLogFile, sizeof(sLogFile), "%slog/%s_%d_%d.log", \
        						m_sEnvAppDly, g_sAppName, g_iProcID, getpid());
	LogV2::setFileName(sLogFile);
    
	//��������
	char  sLockFile[256];
	snprintf(sLockFile, sizeof(sLockFile), "%slock/%s_%d.lock", \
                                 m_sEnvAppDly, g_sAppName, g_iProcID);	
    if (tryLock(sLockFile) != 0) {
        __DEBUG_LOG0_(0, "��������ʧ��,�����˳��� ���-p����!");
    	return -1;	
        
    }
	
	//license��Ȩ
	int opt;
	if (gethostname(m_sHostName, sizeof(m_sHostName)) != 0) {
	}
	
	//�ź���ע��
    if (HandleSigactions() != 0) {
        __DEBUG_LOG0_(0,"Ӧ�ó����ź���ע��ʧ��!");
        return -1;	
    }
    return 0;
}


Application::~Application ( ) 
{

}

int Application::tryLock(const char *pfname)
{
	int  fd;
	char buf[16];
	memset(buf,'\0',sizeof(buf));
	if ((fd=open(pfname, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0) {
		__DEBUG_LOG0_(0,"Ӧ�ó���������ʱ���ļ�ʧ��!");
		return -1;
	}
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;
	if (fcntl(fd, F_SETLK, &lock) < 0) {
		__DEBUG_LOG0_(0,"Ӧ�ó����ļ�����ʧ��!");
		return -1;
	}
	
	if (ftruncate(fd, 0) < 0) {
		__DEBUG_LOG0_(0,"Ӧ�ó�������ʱftruncate��������ʧ��!");
		return -1;
	}
	snprintf(buf, sizeof(buf), "%ld\n", (long)getpid());	
	if (write(fd, buf, strlen(buf)) != (int)strlen(buf)) {
		__DEBUG_LOG0_(0,"����ʱwriteϵͳ����ʧ��!");
		return -1;
	}
	return 0;
		
}

int Application::destory()
{
	return 0;	
}

int Application::init(ABMException &oExp)
{
	return 0;	
}
