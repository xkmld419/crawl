#include "CheckStatPlan.h"
#include "Date.h"
#include "EventSection.h"

//HashList<TAreaCode> *CheckProcess::m_pProcAreaHash = 0;
char StatInterface::m_sDefaultAreaCode[10] = {0};
    
PlanItem *StatInterface::m_pPlan = 0x0; //## ָ��ǰ����ִ�еļƻ�



/*
    �����в���˵��:
     �޲���         ---- ����������, ������ѭ��    CheckStat
     ����������N    ---- ����������, ѭ��N���˳�   CheckStat N
     �������ַ�d    ---- ֹͣ������                CheckStat d
     �������ַ�v|V  ---- �鿴����״̬[V:ֻ�鿴�����е�����]
     CheckStat ���̵�����ͣ�����Ѿ��� mnt �ű�����ӿ��Ʋ���:
         mnt -bstat: ����ϵͳ�����Ϣ��ͳ�Ƴ���[CheckStat]
         mnt -dstat: ֹͣϵͳ�����Ϣ��ͳ�Ƴ���[CheckStat d]
*/
int main (int argc, char ** argv)
{
    cout<<endl;
    
    char *p = getenv ("TIBS_HOME");
    char sLogFile[100];
   	//Log::init(MBC_APP_testinfo); 
    if (p)
        sprintf (sLogFile, "%s/log/CheckStat.log", p);
    else
        strcpy (sLogFile, "CheckStat.log");
    
    Log::setFileName (sLogFile);
    
    CheckStatPlan::m_argv = argv;
    int iTaskId =0 ;    
    if (argc >= 2) {
        iTaskId = atoi (argv[1]);
    }else {
    	cout<<"�������������ID"<<endl ;
    }
    
    /*
    pid_t pExistPID = 0;
    pid_t pMianPid = CheckStatPlan::regMainProcess (pExistPID);
        
    if (pMianPid == 0) 
    {        
        Log::log (0, "������(%s)�Ѿ�����һ������ʵ�� PID:%ld ", argv[0], pExistPID);
        return -1;
    }
    
    CheckStatPlan::updateState (0, "RUN", "����������...");
    Log::log (0, "%s (������--%ld)��ʼ����...", argv[0], pMianPid);
    */
    PlanItem oPlan;   
    CheckStatPlan::openPlan ();        
    while (CheckStatPlan::getPlan (oPlan))
    {
    	if( oPlan.m_iPlanID != iTaskId )
    		continue ;
    		
    // if (!CheckStatPlan::analyzePlan (oPlan) )
    //      continue;
          
      CheckStatPlan::startPlan (oPlan, argv);
        break ;
    }
        
    
    CheckStatPlan::updateState (0, "END", "��������ֹͣ!");
    Log::log (0, "%s (������)�˳�����!", argv[0]);
    
    return 0;
}


