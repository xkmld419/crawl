#include "CheckStatPlan.h"

#include "Date.h"
#include "EventSection.h"

#ifndef _check_stat_sleep
#define _check_stat_sleep  3 //s
#endif

//HashList<TAreaCode> *CheckProcess::m_pProcAreaHash = 0;
char StatInterface::m_sDefaultAreaCode[10] = {0};
    
PlanItem *StatInterface::m_pPlan = 0x0; //## ָ��ǰ����ִ�еļƻ�

static void ShowCtl (char cMethod);

static void WritePlanInfo ();

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
    
    if (p)
        sprintf (sLogFile, "%s/log/CheckStat.log", p);
    else
        strcpy (sLogFile, "CheckStat.log");
    
    Log::setFileName (sLogFile);
    
    CheckStatPlan::m_argv = argv;
    
    int iOperation = 1; //1-����; 2-ֹͣ
    
    int iRunTimes = -1;    
    if (argc >= 2) {
        iRunTimes = atoi (argv[1]);
        if (iRunTimes == 0) {
            iRunTimes = -1;
            if (!strcmp(argv[1],"d"))
                iOperation = 2;
            else if (!strcmp(argv[1],"v") || !strcmp(argv[1],"V")) {
                ShowCtl (argv[1][0]);
                exit (0);
            }
            else {
                cout << "�������Ϸ�" << endl;
                return -1;
            }             
        }
    }
    
    pid_t pExistPID = 0;
    pid_t pMianPid = CheckStatPlan::regMainProcess (pExistPID);
    
    //## ��ǰ��ֹͣ�����̲��� 
    if (iOperation == 2) {
        if (pExistPID) {
            cout << "[Message] ����������-CheckStat ..." << endl;
            CheckStatPlan::setQuitFlag ();
        }
        else
            cout << "[Warning] CheckStat-�����̲�����!" << endl;
        
        return 0;
    }
    
    if (pMianPid == 0) 
    {        
        Log::log (0, "������(%s)�Ѿ�����һ������ʵ�� PID:%ld ", argv[0], pExistPID);
        return -1;
    }
    
    CheckStatPlan::updateState (0, "RUN", "����������...");
    Log::log (0, "%s (������--%ld)��ʼ����...", argv[0], pMianPid);
    
    while (iRunTimes && !CheckStatPlan::getQuitFlag ())
    {
        PlanItem oPlan;
        
        CheckStatPlan::openPlan ();
        
        while (CheckStatPlan::getPlan (oPlan))
        {
            if (!CheckStatPlan::analyzePlan (oPlan))
                continue;
            
            DB_LINK->disconnect();
            
            int iSon = fork();
            int iGrandSon;
            
            switch (iSon)
            {
                case 0:
                    iGrandSon = fork();
                    usleep (1000);
                    switch (iGrandSon) 
                    {
                        case 0:
                            usleep (1000);
                            DB_LINK->connect();
                            CheckStatPlan::startPlan (oPlan, argv);
                            Log::log (0, "+++ GrandSon process Success, PlanID=%d, PID:%d",oPlan.m_iPlanID, getpid());

							_exit(0);
                      
                        case -1:
                            Log::log (0, "GrandSon process fork err");
                            break;
                        default:
                            Log::log (0, "--- GrandSon process Created, PlanID=%d, PID:%d",oPlan.m_iPlanID, iGrandSon);
                            break;
                    }

					_exit(0);
                  
                case -1:
                    Log::log (0, "Son process fork err");
                    break;
                default:
                    Log::log (99, "=> Son process Created, pid:%d", iSon);
                    waitpid (iSon, NULL, 0);
                    break;
            }
                        
            DB_LINK->connect();
        }
        
        CheckStatPlan::closePlan ();
        
        if (iRunTimes > 0)
            iRunTimes --;
        
        WritePlanInfo ();
        
        sleep (_check_stat_sleep);
    }
    
    CheckStatPlan::updateState (0, "END", "��������ֹͣ!");
    cout<<endl;
    Log::log (0, "%s (������)�˳�����!", argv[0]);
    
    return 0;
}


static int getPlanInfo (int iPID, char *sMem, char *sCpu)
{
    char sCommand[256];
    FILE *fp;
    int ret;

    strcpy (sMem, "0");
    strcpy (sCpu, "0");    
    
#ifndef _linux_
    sprintf (sCommand, "UNIX95= ps -p %d -o pcpu -o vsz |grep -v ""CPU"" |awk '{print $1, $2}'", iPID);
    fp = popen (sCommand, "r");
    if (fp == NULL)
        return -1;

    while(1){
        if (feof(fp)) break;
        if (fscanf (fp, "%s %s\n", sCpu, sMem) == EOF)
            ret = -1;
        else
            ret = 0;
    }
#else
    sprintf (sCommand, "ps -p %d -o '%%C %%z' |grep -v ""CPU"" |awk '{print $1, $2}'", iPID);

    fp = popen (sCommand, "r");
    if (fp == NULL)
        return -1;

    if (fscanf (fp, "%s %s\n", sCpu, sMem) == EOF)
        ret = -1;
    else
        ret = 0;
#endif

	int iCpu = atoi(sCpu);
	int iMem = atoi(sMem)/1024;
    memset( sCpu,0,strlen(sCpu) );
	memset( sMem,0,strlen(sMem) );
	sprintf( sCpu,"%d",iCpu );
	sprintf( sMem,"%d",iMem );
    
    pclose (fp);
    return ret;
}

class _TPlanInfo {
  public:
    int   m_iPlanID;
    char  m_sItemDesc[61];
    char  m_sState[6];
    char  m_sStateDate[15];
    int   m_iSysPid;
    char  m_sMem[10];
    char  m_sCpu[4];
};

#define __MAX_PLAN_NUM 500
static _TPlanInfo s_oPlanInfo[__MAX_PLAN_NUM];
static long s_lTimes = 0;
static long s_lLines = 0;

//ˢ�¼ƻ���Ϣ
static void FreshPlan (int iWriteFlag = 0)
{
    memset (&s_oPlanInfo, 0, sizeof(_TPlanInfo) * __MAX_PLAN_NUM);

    DEFINE_QUERY (qry);
    qry.setSQL ("select plan_id, trim(substr(item_desc,1,30)) item_desc,"
        " nvl(state,'END') state, nvl(sys_pid,0) sys_pid ,"
        " to_char(state_date,'MM/DD hh24:mi:ss') state_date "
        " from b_check_stat_plan "
        " where plan_id >= 0 order by plan_id");
    qry.open ();
    int iPos = 0;
    
    while (qry.next())
    {
        s_oPlanInfo[iPos].m_iPlanID = qry.field("plan_id").asInteger();
        strcpy (s_oPlanInfo[iPos].m_sItemDesc, qry.field("item_desc").asString());
        strcpy (s_oPlanInfo[iPos].m_sState, qry.field("state").asString());
        s_oPlanInfo[iPos].m_iSysPid = qry.field ("sys_pid").asInteger();
        strcpy (s_oPlanInfo[iPos].m_sStateDate, qry.field ("state_date").asString());
        
        if (strcmp (s_oPlanInfo[iPos].m_sState, "RUN")) {
            s_oPlanInfo[iPos].m_iSysPid = 0;
            s_oPlanInfo[iPos].m_sMem[0] = '0';
            s_oPlanInfo[iPos].m_sCpu[0] = '0';
        }
        else {
            int iRet = getPlanInfo (s_oPlanInfo[iPos].m_iSysPid,
                        s_oPlanInfo[iPos].m_sMem, s_oPlanInfo[iPos].m_sCpu);
            
            if (iWriteFlag && iRet==0 && s_oPlanInfo[iPos].m_iPlanID!=0) 
            {//## ����ǰ���̵���Դռ�����д��ӿڱ� Stat_interface
                //MNT_SELF_CAPA_01: ����ID;����ʱ��;�ڴ�ռ��(M);cpuռ����
                StatInterface oIt ("MNT_SELF_CAPA_01");
                Date d;
                sprintf (oIt.sDimensions, "%d;%s;%s;%s", 
                        s_oPlanInfo[iPos].m_iPlanID, 
                        d.toString(),
                        s_oPlanInfo[iPos].m_sMem,
                        s_oPlanInfo[iPos].m_sCpu
                );
                oIt.insert (true);
            }                
        }
        
        if (!strcmp (s_oPlanInfo[iPos].m_sState, "ERR"))
            strcpy (s_oPlanInfo[iPos].m_sState, "error");
        
        iPos ++;
    }
    
    s_lLines = iPos;
}

//���ƻ��ĵ�ǰ״̬��Ϣ������Ļ
static void ShowPlan (char cMethod)
{
    system ("clear");
    cout << "\n==== BSN.���ܼ����Ϣ����ȡ����״̬��Ϣ("<<++s_lTimes<<") ====\n\n";
    cout << "PlanID State   SYS_PID MEM(M) CPU(%) STATE_DATE      PlanDesc" << endl;
    char sMsg[1024];
    for (int iPos=0; iPos<s_lLines; iPos ++)
    {
        sprintf (sMsg, "%6d %-5s %9d %6s  %3s%%  %14s  %-60s",
            s_oPlanInfo[iPos].m_iPlanID,
            s_oPlanInfo[iPos].m_sState,
            s_oPlanInfo[iPos].m_iSysPid,
            s_oPlanInfo[iPos].m_sMem,
            s_oPlanInfo[iPos].m_sCpu,
            s_oPlanInfo[iPos].m_sStateDate,
            s_oPlanInfo[iPos].m_sItemDesc
        );
        
        if (cMethod=='V' && strcmp (s_oPlanInfo[iPos].m_sState, "RUN"))
            continue;
        
        cout << sMsg << endl;
    }
    cout << endl << "(Note: Ctrl+\\ �˳�!)"<< endl << endl;
}

static void Quit (int isignal)
{
    exit(0);
}

static void ShowCtl (char cMethod)
{
    signal (SIGQUIT, Quit);
    signal (SIGINT, Quit);
    signal (SIGTERM, Quit);
    signal (SIGABRT, Quit);
    signal (SIGTSTP, Quit);
    
    memset (&s_oPlanInfo, 0, sizeof(_TPlanInfo) * __MAX_PLAN_NUM);
    
    while (true)
    {
        FreshPlan ();
        ShowPlan (cMethod);
        sleep (2);
    }
}

static void WritePlanInfo ()
{
    static int s_iCnt = 0;
    
    #ifndef WRITE_PLAN_INFO_TIME_LEN
    #define WRITE_PLAN_INFO_TIME_LEN  60 //s
    #endif
    if (++s_iCnt < WRITE_PLAN_INFO_TIME_LEN/_check_stat_sleep)
        return;
    
    s_iCnt = 0;
    
    //FreshPlan (1);
    FreshPlan (0); //modify for hss test 2011.06.28 ȱ��
}

/*
create table B_Check_Stat_Plan (
    plan_id       number(9)  not null,     -- pk  ����˵��:plan_id=0�ļ�¼������������̵�����״̬
    item_desc     varchar2(1024),          -- ͳ���������
    open_state    number(1)  not null,     -- �Ƿ񿪷Ŵ�ͳ���� 0-������; 1-���� (���Ϊ0,������[plan_id=0]�������˳�)
    app_type      number(1)  not null,     -- 0-�ͷ��ӽ���ֱ�ӽ���ͳ��; 1-�ͷ��ӽ��̵���stat_name�е�PLSQL��
    stat_name     VARCHAR2(4000) not null, -- ��app_type=0ʱ,��ʾͳ�Ƶ�����ID,IDΪϵͳ����,��������; ��app_type=1ʱ,��ʾ���õ�PLSQL��
    interval_type number(1)  not null,     -- 1-��ʾtime_interval��ʱ����,  2-��ʾtime_interval��ÿ�µĹ̶�ʱ��
    time_interval varchar2(8)  not null,   -- ����ʾʱ����ʱ,��λΪ��(0��ʾֻ��next_run_timeʱͳ��һ�� ͳ��ִ��ǰ,����next_run_time��Ϊ�����); ����ʾÿ�¹̶�ʱ��ʱ,ʱ���ʽΪ:'DDHH24MISS',��ʾÿ���¹̶�ʱ������
    next_run_time date,           -- �´�ͳ��ʱ�� (�ɳ����Զ���)
    run_parameter varchar2(512),  -- �´�����ʱ���������
    sucess_times  number(12) default 0 not null, -- �ɹ�����(ִ�гɹ�ʱ,�ӽ���ִ�м���++)
    failed_times  number(12) default 0 not null, -- ʧ�ܴ���(�������̼���ӽ����Ƿ��쳣�˳�,ִ�м���++)
    state         varchar2(3),    -- ״̬ RUN-����ִ��, END-ִ�����, ERR-ִ�г���, NUL-app_type/stat_name��Ч
    debug_flag    number(1)  default 0 not null, -- Ϊ1ʱ, ָ���ӽ�������ʱ�������״̬(���ӽ�������ʱ,��������sleepѭ��,�ȴ�����)
    state_date    date  ,         -- ״̬ʱ��
    sys_pid       number(9),      -- ��̨ϵͳ���̺�PID
    exec_name     varchar2(256),  -- ������ (���Ը���sys_pid��exec_nameȷ��ͳ���ӽ����Ƿ����)
    message       varchar2(4000)  -- ������Ϣ
);
alter table B_CHECK_STAT_PLAN
  add constraint pk_check_stat_plan primary key (PLAN_ID);


comment on column B_CHECK_STAT_PLAN.PLAN_ID
  is 'pk  ����˵��:plan_id=0�ļ�¼������������̵�����״̬';
comment on column B_CHECK_STAT_PLAN.ITEM_DESC
  is 'ͳ���������';
comment on column B_CHECK_STAT_PLAN.OPEN_STATE
  is '�Ƿ񿪷Ŵ�ͳ���� 0-������; 1-���� (���Ϊ0,������[plan_id=0]�������˳�)';
comment on column B_CHECK_STAT_PLAN.APP_TYPE
  is '0-�ͷ��ӽ���ֱ�ӽ���ͳ��; 1-�ͷ��ӽ��̵���stat_name�е�PLSQL��';
comment on column B_CHECK_STAT_PLAN.STAT_NAME
  is '��app_type=0ʱ,��ʾͳ�Ƶ�����ID,IDΪϵͳ����,��������; ��app_type=1ʱ,��ʾ���õ�PLSQL��';
comment on column B_CHECK_STAT_PLAN.INTERVAL_TYPE
  is '1-��ʾtime_interval��ʱ����,  2-��ʾtime_interval��ÿ�µĹ̶�ʱ��';
comment on column B_CHECK_STAT_PLAN.TIME_INTERVAL
  is '����ʾʱ����ʱ,��λΪ��(0��ʾֻ��next_run_timeʱͳ��һ�� ͳ��ִ��ǰ,����next_run_time��Ϊ�����); ����ʾÿ�¹̶�ʱ��ʱ,ʱ���ʽΪ:''DDHH24MISS'',��ʾÿ���¹̶�ʱ������';
comment on column B_CHECK_STAT_PLAN.NEXT_RUN_TIME
  is '�´�����ͳ�Ƶ�ʱ�� (�ɳ����Զ���)';
comment on column B_CHECK_STAT_PLAN.run_parameter
  is '�´�����ʱ���������';
comment on column B_CHECK_STAT_PLAN.SUCESS_TIMES
  is '�ɹ�����(ִ�гɹ�ʱ,�ӽ���ִ�м���++)';
comment on column B_CHECK_STAT_PLAN.FAILED_TIMES
  is 'ʧ�ܴ���(�������̼���ӽ����Ƿ��쳣�˳�,ִ�м���++)';
comment on column B_CHECK_STAT_PLAN.STATE
  is '״̬ RUN-����ִ��, END-ִ�����, ERR-ִ�г���, NUL-app_type/stat_name��Ч';
comment on column B_CHECK_STAT_PLAN.DEBUG_FLAG
  is 'Ϊ1ʱ, ָ���ӽ�������ʱ�������״̬(���ӽ�������ʱ,��������sleepѭ��,�ȴ�����)';
comment on column B_CHECK_STAT_PLAN.STATE_DATE
  is '״̬ʱ��';
comment on column B_CHECK_STAT_PLAN.SYS_PID
  is '��̨ϵͳ���̺�PID';
comment on column B_CHECK_STAT_PLAN.EXEC_NAME
  is '������ (���Ը���sys_pid��exec_nameȷ��ͳ���ӽ����Ƿ����)';
comment on column B_CHECK_STAT_PLAN.MESSAGE
  is '������Ϣ';
*/
