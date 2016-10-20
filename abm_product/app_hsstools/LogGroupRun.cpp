#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "Log.h"
#include "Process.h"
#include "interrupt.h"
#include "Environment.h"
#include "ThreeLogGroup.h"
//#include "HbAdminMgr.h"

class LogGroupRun: public Process
{
public:
    LogGroupRun ();
    ~LogGroupRun ();

public:
    int run();
private:
    bool CheckShm();
    void GetSysParamInfo();

    ThreeLogGroupMgr* m_poMgr;
};

DEFINE_MAIN (LogGroupRun)

LogGroupRun::LogGroupRun ()
{
    m_poMgr = new ThreeLogGroupMgr();
    Log::init(MBC_APP_loggrum);
}

LogGroupRun::~LogGroupRun()
{
    if(m_poMgr)
    {
	    delete m_poMgr;
        m_poMgr = 0;
    }
}

bool LogGroupRun::CheckShm()
{
    int iSysID=0;
    iSysID = shmget (SHM_LOG_GROUP_DATA,0,0);
    if(iSysID<0)
        return false;
    return true;
}

void LogGroupRun::GetSysParamInfo()
{
    vector<SysParamHis> m_vtSysParamHis;

    Process::m_pCommandCom->FlushParam(&m_vtSysParamHis);

    for(int i=0;i<m_vtSysParamHis.size();i++)
    {
        if(!strcmp(m_vtSysParamHis[i].sSectionName,"LOG"))
        {
            if(!strcmp(m_vtSysParamHis[i].sParamName,"log_alarm_level"))
            {
                int temInfoLevl = atoi(m_vtSysParamHis[i].sParamValue);
                if(temInfoLevl<-1)
                {
                    Log::log(0,"���Ĳ�������---�޸ĳ����߼���Χ,�������޸�");
                    continue;
                }               
                int lastLevl = Log::m_iAlarmLevel;
                Log::m_iAlarmLevel = temInfoLevl;
                Log::log(0,"���Ĳ�������---�澯��Ϣ����־�ȼ�������%d�����Ϊ%d��",lastLevl,temInfoLevl);    
            }
            else if(!strcmp(m_vtSysParamHis[i].sParamName,"log_alarmdb_level"))
            {
                int temInfoLevl = atoi(m_vtSysParamHis[i].sParamValue);
                if(temInfoLevl<-1)
                {
                    Log::log(0,"���Ĳ�������---�޸ĳ����߼���Χ,�������޸�");
                    continue;
                }               
                int lastLevl = Log::m_iAlarmDbLevel;
                Log::m_iAlarmDbLevel = temInfoLevl;
                Log::log(0,"���Ĳ�������---�澯��Ϣ�����ȼ�������%d�����Ϊ%d��",lastLevl,temInfoLevl);    
            }
            else if(!strcmp(m_vtSysParamHis[i].sParamName,"log.interval"))
            {
                int teminval = atoi(m_vtSysParamHis[i].sParamValue);
                if(teminval<0)
                {
                    Log::log(0,"���Ĳ�������---�޸ĳ����߼���Χ,�������޸�");
                    continue;                      
                }
                int lastinval = Log::m_interval;
                Log::m_interval = teminval;
                Log::log(0,"���Ĳ�������---��Ϣ��������������%d ����Ϊ%d ��",lastinval,teminval);
            }
            else if(!strcmp(m_vtSysParamHis[i].sParamName,"log_group_mode"))
            {
                int temMode = atoi(m_vtSysParamHis[i].sParamValue);
                if(temMode<0)
                {
                    Log::log(0,"���Ĳ�������---�޸ĳ����߼���Χ,�������޸�");
                    continue;                    
                }
                m_poMgr->SetGroupWriteMode(temMode);
                Log::log(0,"���Ĳ�������---��־��д���ݷ�ʽ�������Ϊ%d ",temMode);
            }
            else if(!strcmp(m_vtSysParamHis[i].sParamName,"log_group_checkpoint_size"))
            {
                int iTouchSize = atoi(m_vtSysParamHis[i].sParamValue);
                if(iTouchSize<=0 || iTouchSize>=100)
                {
                    Log::log(0,"���Ĳ�������---�޸ĳ����߼���Χ,�������޸�");
                    continue;                     
                }
                m_poMgr->SetGroupCheckTouch(iTouchSize);
                Log::log(0,"���Ĳ�������---checkpoint�л���־�鷧ֵ���Ϊ%d ",iTouchSize);
            }
        }
    }
    m_vtSysParamHis.clear(); //��һ����ʷ��Ϣ
    return;
}
int LogGroupRun::run()
{
    string errMsg;
    int i=1;
    //int bIsLogin =-1;

 	//HbAdminMgr * cHbLogin = new HbAdminMgr();
	//bIsLogin = HbAdminMgr::getAuthenticate();

//	if(bIsLogin==-1)
//	{
//		Log::log(0, "���ȵ�¼��֤!");
//		return 0;
//	}
//	
//	if(bIsLogin==0)
//	{
//		Log::log(0, "δ��¼��֤,�������е�¼��֤����!");
//		return 0;
//	}


    Log::log (0, "%s", "hbloggrun run!");
    while(1)
    {
        if(!CheckShm())
        {
            Log::log(0,"���������ڴ�ʧ�ܣ������Ƿ񴴽�");
            return -1;
        }
        GetSysParamInfo();
        if(m_poMgr->GetGroupWriteMode()!=WRITESHARE)
            continue;
        if(!m_poMgr->exist())
        {
            Log::log(0,"������־�鹲���ڴ��Ƿ񴴽�\n");
            return -1;
        }
        /*
        if(!m_poMgr->CheckInfoCallBack())
        {
            Log::log(0,"��ֽ���ִ�л��չ����ڴ�ʧ��\n");
            return -1;                 
        }
        */
        if(!m_poMgr->CommitInfoFile())
        {
            Log::log(0,"��ֽ���ִ������ڴ��ļ�ʧ��\n");
            return -1;                       
        }
        if(!m_poMgr->Check75CheckPoint())
        {
            Log::log(0,"��ֽ���ִ��checkpointʧ��\n");
            return -1;                
        }
        i = Log::m_interval>0 ? Log::m_interval : 1;
        sleep(i);
    }
    return 0;
}

