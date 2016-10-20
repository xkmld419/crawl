#ifndef _ABM_PROCESS_H
#define _ABM_PROCESS_H
//#include "../app_guard/ProcessInfo.h"
//#include "hbcfg.h"
#include <signal.h>
//#include "TOCIQuery.h"
#include "ThreeLogGroup.h"
#include <vector>
#include "ReadCfg.h"

using namespace std;

#define   MAX_LENTH    200          //�ַ�������󳤶�


class ABMProcess
{
	private:
		
	public:
		ABMProcess();
		~ABMProcess();
		int ShowProcessInfo(char * sMsg);
		ThreeLogGroup m_oLog;
		//int ShowSubmoduleStatus(int mode,char * sTime);//mode[1]:-b -p��vp_id���в�ѯ
		int ShowSubmoduleStatus();
		int RunProcess(int iProcessId);
		int StopProcess(int iProcessId);
		int ShowProcess(int iProcessId);
		int ShowProcessInfoByApp(char * sAppName);
		int CheckProcess(int iProcessId);
		int StartProcess(int iProcessId);
		int iProcessId;
		bool PrintUsage();
		ReadCfg *m_poConfS; 
};



#endif
