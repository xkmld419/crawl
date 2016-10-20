/*VER: 1*/ 
//����Ҫ����monitor�ȵ� main
#include "ProcessEx.h"
#include "Log.h"
#include <algorithm>
#include <functional>

ProcessEx *GetProcess ();

extern int g_argcex;
extern char ** g_argvex;
extern ProcessEx *g_pProc;

#include<sys/types.h>
#include <sys/stat.h>
#define MAXFILE 65535

//ʹ�÷���ģʽ����
void StartAsService(void)
{
	//���շ���ģʽ����
	pid_t pid;
	int i;
	pid=fork();
	if(pid<0){
		printf("error in fork\n");
		exit(1);
	}else if(pid>0) 
		exit(0); 
	setsid();
	/*
	chdir("/");
	umask(0);
	*/
	for(i=0;i<MAXFILE;i++)
		close(i);
}
int main (int argc, char **argv)
{
	for(int i=1;i<argc;i++)	{
		string strargv= argv[i];
		transform(strargv.begin(),strargv.end(),strargv.begin(),::tolower);	
		if(strargv.compare("-service")==0 ||(strargv.compare("service")==0)){
			StartAsService();
		}
	}
	char sString[4096]={0};
	try {

		//# ȫ�������в���, �ӽ������Ҫʹ��, ͨ��extern�ķ�ʽ����
		g_argcex = argc;
		g_argvex = argv;

		//# ��ȡ�����ַ, GetProcess �������ȫ�ֺ���
		//# �����ж����: DEFINE_MAIN(class_name) ����,�ɸú�ȥ�ṩGetProcess����
		g_pProc = GetProcess ();
		//g_pProc->Init(argv[0]);

		//TODO ::initû����־�ļ���
		Log::log (0,"[Message]: ��ǰ���� PID = %d", getpid());

		//# run() Ϊ�麯��, ��ִ̬�������run()����
		g_pProc->run();

		delete g_pProc;

		Pprintf(0,1,0,"Ӧ�ó��������˳���");
		return 0;

	}

	//# OCI Exception
	catch (TOCIException &e) {
		cout<<e.getErrMsg()<<e.getErrSrc()<<endl;
		if (g_pProc) {
			delete g_pProc;
			g_pProc = 0;
		}

		snprintf (sString,4095, "�յ�TOCIException�˳�\n[TOCIEXCEPTION]: TOCIException catched\n"
			"  *Err: %s\n"
			"  *SQL: %s", e.getErrMsg(), e.getErrSrc());

		Log::log (0, sString);

		DB_LINK->rollback ();
		//Pprintf (9011, 1, 0, sString);      

	}

	//# TException
	catch (TException &e) {
		if (g_pProc) {
			delete g_pProc;
			g_pProc = 0;
		}
		snprintf (sString,4095, "�յ�TException�˳�\n[EXCEPTION]: %s", e.GetErrMsg());
		Log::log (0, sString);

		DB_LINK->rollback ();
		Pprintf (9012, 1, 0, sString);
		AbortProcessEx(-2);
	}

	//# Exception
	catch (Exception &e) {
		if (g_pProc) {
			delete g_pProc;
			g_pProc = 0;
		}
		snprintf (sString,4095, "�յ�Exception�˳�\n[EXCEPTION]: %s", e.descript());
		Log::log (0, sString);

		if (DB_CONNECTED) {
			DB_LINK->rollback ();
		}

		Pprintf (9013, 1, 0, sString);
		AbortProcessEx(-3);
	}

	//# Other Exception
	catch (...) {
		if (g_pProc) {
			delete g_pProc;
			g_pProc = 0;
		}
		snprintf (sString,4095, "�յ�����Exception�˳�");
		Log::log (0, sString);

		DB_LINK->rollback ();
		Pprintf (9014, 1, 0, sString);
		AbortProcessEx(-4);
	}

}

