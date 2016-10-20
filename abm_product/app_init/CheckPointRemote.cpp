
#include "CheckPoint.h"
#include "Log.h"
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>

#define MAXFILE	65535

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
	if(setsid()<0)
	{
		printf("error in setsid\n"); 
  		exit(0); 
	} 
	chdir("/");
	umask(0);
	for(i=0;i<MAXFILE;i++)
		close(i);
}


int main (int argc, char **argv)
{
	Log::log (0,"[Message]: ��ǰ���� PID = %d", getpid());
	/*for(int i=1;i<argc;i++)
	{
		string strargv= argv[i];
		transform(strargv.begin(),strargv.end(),strargv.begin(),::tolower);	
		if(strargv.compare("-service")==0 ||(strargv.compare("service")==0)||strargv.compare("-s")==0 )
		{
			StartAsService();
		}
	}*/
	if(argc>1)
	{
		string strargv= argv[1];
		transform(strargv.begin(),strargv.end(),strargv.begin(),::tolower);	
		if(strargv.compare("-service")==0 || strargv.compare("service")==0)
		{
			StartAsService();
		}
	}
	Log::log(0,"׼������");
	char sString[4096];
  	try {
        CheckPoint *p = new CheckPoint ();
		if(p)
		{
			//������ڴ�������ص�ʵ��ִ�к���
			Log::log(0,"��ʼ����checkpoint����!");
			p->run();
      	} else {
      		Log::log(0,"�ڴ�ռ䲻��,����!");
      		return 0;
      	}
        printf("Ӧ�ó��������˳�!\n");
        return 0;
		
   }
   
  //OCI Exception
  catch (TOCIException &e) 
  {
        cout<<e.getErrMsg()<<e.getErrSrc()<<endl;
        snprintf (sString,4095, "�յ�TOCIException�˳�\n[TOCIEXCEPTION]: TOCIException catched\n"
                "  *Err: %s\n"
                "  *SQL: %s", e.getErrMsg(), e.getErrSrc());
        Log::log (0, sString);  
  }
  //# TException
  catch (TException &e) {
        snprintf (sString,4095, "�յ�TException�˳�\n[EXCEPTION]: %s", e.GetErrMsg());
        Log::log (0, sString);
  }
  //# Exception
  catch (Exception &e) {
        snprintf (sString,4095, "�յ�Exception�˳�\n[EXCEPTION]: %s", e.descript());
        Log::log (0, sString);
  }
  //# Other Exception
  catch (...) {
        snprintf (sString,4095, "�յ�����Exception�˳�");
        Log::log (0, sString);
  }

}

