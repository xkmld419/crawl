#ifndef HSS_PROCESS_INIT
#define HSS_PROCESS_INIT

#define MAX_EXEC_PATH_LEN 	512
#define MAX_PRP_NAME_LEN 		128
#define MAX_PARAMTER_LEN		128
#include "Log.h"
#include "Environment.h"
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>


class HssProInfo
{
public:
		HssProInfo();
		~HssProInfo();
		
		long proid;//PROCESS_ID
		char exename[MAX_PRP_NAME_LEN]; // ִ�г����� Prep��Щ
		char execpath[MAX_EXEC_PATH_LEN]; //ִ��·��
		char parameter[MAX_PARAMTER_LEN];
		char cSate[1]; //״̬
		long flowid; //���̺�
};

class HssProcessInit
{
		  public:
		  				HssProcessInit();
		  				~HssProcessInit();
		  				//  
		  				int startApp(char *appname);
		  //private:
		  				//  
		  				int getProcess(char *appname);
		  				//  
		  				bool startProcess(HssProInfo proinfo);
		  				//
		  				vector<HssProInfo> vec_process;
};

#endif