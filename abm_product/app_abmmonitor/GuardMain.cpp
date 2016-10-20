/*VER: 3*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#define _JUST_DBLINK_

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

#include "GetinfoMonitor.h"
#include "Environment.h"
#include "GuardMain.h"
#include "HashList.h"
#include "WfPublic.h"
extern char **environ; 
#include "ABMException.h"

//HP����ռ�ù���

//HP����ռ�ù���


extern TSHMCMA *g_pShmTable;  //ͨ�Ŵ�����ı�
extern long g_lBillMntKey ; 
extern long g_lWorkFlowKey ;
extern long g_lLockWorkFlowKey ;
extern long g_lLockApplicationKey ;
extern long g_lLockClientKey ;

const int MAX_MNT_NUM = 200;
const int MAX_CLT_NUM = 10;

char **g_argv;
int  g_iShmID;               //�����ڴ��ʶ
int  g_iLocalHostID = 1;


TRouteTable  *g_pRouteTable = NULL;       //���б��
TMonitorFront g_aMonitorFront[MAX_MNT_NUM];
TClientInfo  g_aClientLink[MAX_CLT_NUM]; 

//static OrgMgr *g_pOrgMgr=NULL;

static TTimeRunPlan   *g_pTimeRunPlanHead = NULL;  //��ʱ���мƻ�
static TOrdinalPlan   *g_pOrdinalPlanHead = NULL;  //˳���Զ�ִ�мƻ�

//���ǰ̨�Ƿ���ڿ�������  ��-�����±�, ��-����-1
int CheckMonitorCtrlExist(int _iBillFlowID); 

//���Զ�������Ƿ���� ��-�����±�, ��-����-1
int CheckClientExist(int _iHostID, char _sSysUserName[]); 

int GetNextMonitorIndex();
int GetAppIndex(int _iProcessID);

static void PushMntMsg(char *sTemp, int iProcessID);

static THeadInfo *pInfoHead = NULL;
static TProcessInfo *pProcInfoHead = NULL;
static char *pLogName = 0;

void InitProcInfoQry (TOCIQuery &qry);
int GetProcNum ();
int InitProcShm ();
int ExecuteBootDown ();
int ExecuteBootDown_v2 ();

void CheckGuardMianExist();
void ShutdownGuard ();
int DestroyGuardMainShm ();
void  UpdateProcState (int _iProID);
void CheckMonitorApp ();
int GetSysProcInfo (int iPID, char *sMem, char *sCpu);

void StopGuard(int isignal) {
    TWfLog::log(0,"Catched signal SIGUSR1! GuardMain Exit!");
    exit(0);
}
int main(int argc, char ** argv)
{
    char opt;
    int iProcNum, iTimes=5;
    if (argc < 2) {
        cout <<"usage: " <<argv[0]<<" [-b]|[-d]|[-k]|[-r] \n";
        exit (1);
    }

    if (signal(SIGUSR1,StopGuard) == SIG_ERR)  {
        cout<<"Can't catch SIGUSR1"<<endl;
        TWfLog::log(0,"Can't catch SIGUSR1");
        exit(0);
    }
    MonitorInfo oInfoMonitor;
    ABMException oExpmon;
    oInfoMonitor.putCfgShm(oExpmon);
    if ( (opt=getopt(argc,argv,"bdkr"))!=EOF){
        switch (opt){
        case 'b': //����ػ��������� (�����ڴ� ��������,�򴴽�; ������,ֱ������)
            break;
        case 'd': //����ػ������˳� (�����Ϣ���)
            ShutdownGuard ();
            exit(1);
        case 'k': //ֹͣ��ǰ����UnixӦ�ý���,����ػ������˳�,�������Ӧ�����ڴ�
            if (AttachShm (MAX_APP_NUM*sizeof (THeadInfo), (void **)&pInfoHead) >= 0) 
            {
                
                CommunicationInit();
                
                if (kill(pInfoHead->iSysPID,0)==0) 
                {
                    kill (pInfoHead->iSysPID, 9);
                    printf ("\n����ػ����� GuardMain(%d) killed!\n", pInfoHead->iSysPID);
                }

                pProcInfoHead = &pInfoHead->ProcInfo;
                for (int i=0; i<pInfoHead->iProcNum; i++) {
                    if ((pProcInfoHead+i)->iState == ST_RUNNING ||
                        (pProcInfoHead+i)->iState == ST_INIT ||
                        (pProcInfoHead+i)->iState == ST_DESTROY ||
                        (pProcInfoHead+i)->iState == ST_WAIT_DOWN) {
                        if ((pProcInfoHead+i)->iSysPID > 0)
                            kill ((pProcInfoHead+i)->iSysPID, 9);
                        (pProcInfoHead+i)->iState == ST_KILLED;
                        UpdateProcState((pProcInfoHead+i)->iProcessID);
                        printf ("Child process %d killed\n", (pProcInfoHead+i)->iSysPID);
                    }
                }
            }
            else { //��������������
                cout << "\n����ػ�����GuardMain������,\n-->����ɾ�����������й����ڴ�..."<<endl;
            }
            DestroyGuardMainShm();
            exit(1);
        
        case 'r':
            if (AttachShm (MAX_APP_NUM*sizeof (THeadInfo), (void **)&pInfoHead) <0 ) {
                cout << "\n��ع�������������, ����ʧ�ܣ�"<<endl;
                exit(0);
            }
            if (kill(pInfoHead->iSysPID,0)){
                cout << "\n����ػ�����GuardMain������, ����ʧ�ܣ�"<<endl;
                exit(0);
            }
            
            TWfLog::log(0,"\n��������������Ϣ ...");
            cout<<"\n��������������Ϣ ..."<<endl;
            pInfoHead->iRefreshFlag = 1;
            while (iTimes--) {
                if (pInfoHead->iRefreshFlag == 0) {
                    TWfLog::log(0,"\n��������������Ϣ �ɹ�!");
                    cout<<"\n��������������Ϣ �ɹ�!"<<endl;
                    exit(0);
                }
                sleep(1);
            }
            TWfLog::log(0,"\n��������������Ϣ ʧ��!");
            cout<<"\n��������������Ϣ ʧ��!"<<endl;
            exit(1);
            
        default:
            cout <<"usage: " <<argv[0]<<" [-b]|[-d]|[-k]|[-r] \n";
            exit(1);
        }
    }
    
    try {

        CheckGuardMianExist();
        
        //old mnt
        if (InitProcShm () < 0) {
            cout<<endl<<"����ػ����̳�ʼ�������ڴ�ʧ��,�˳�!"<<endl;
            exit (1);
        }
        
        //ͨѶ��ʼ��,��ʼ���ź���
        int iRet = CommunicationInit(false);
        if (iRet<0 ) 
        {
            cout<<"MC_���ȹ����ڴ洴��ʧ��"<<endl;
            TWfLog::log(WF_FILE,"GuardMain: get ShmMemory err.");
            exit(0);
        }
        cout<<endl;
                
        g_pRouteTable = new TRouteTable;
        
        g_pTimeRunPlanHead = new TTimeRunPlan;
        
        g_pOrdinalPlanHead = new TOrdinalPlan;
        
        //g_pOrgMgr = new OrgMgr;
        //DEFINE_QUERY(qry);
        
        while(1) {
            
            //qry.commit();
            //qry.close();
            
            //����monitor�������/ͣ��������
            ExecuteBootDown();
            
            //����ǰ̨���ȼ�ؽ�����������
            ResponseMntConnect();
            
            //������Ӧ�ó����¼���� (Ӧ�ó���ͨ���ӿڵ��� ConnectMonitor,DisConnectMonitor )    
            ResponseAppConnect();
            
            //����Զ���ػ�������������
            ResponseClientConnect();  
            
            //����Ӧ�ó���Ĵ���Ч��
            CalcTreatRate();
            
            //����ǰ̨��������
            ResponseRecvAppInfo();
            ResponseSendCommand();
            ResponseChangeStatus();
            ResponseRefreshRunplan();
            ResponseRecvPassInfo(); 
            //Զ�̼��ָ��/��Ϣ����
            ResponseGetCommand();
            ResponseAckCommand();
            ResponseSendRemoteMsg();
            
            //�������ڴ���,����Ӧ�ý��������Ϣѹ��ǰ̨��� TMonitorFront �Ļ�����(�ȴ�ǰ̨��ȡ)
            OutputMsgIntoFrontBuff();
            
            //��ʱ���ǰ̨���Monitor�ĸ��Ʒ����̵������Ƿ��쳣������
            //CheckMonitorAlive();
            
            //��ʱ���Զ�̼��Client�������Ƿ��쳣������
            //CheckClientAlive();
            
            //��ʱ��顢ά��Ӧ�ó����״̬,�����쳣���
            CheckMonitorApp ();
            CheckApplication (g_iLocalHostID, pLogName);
            
            //ɨ����Ҫ������ʱ�����Ľ��̲�����,Զ�̵ķ���ָ��
            ScanTimeRunPlan();
            
            //ɨ����Ҫ�����Զ�˳�������Ľ��̲�����,Զ�̵ķ���ָ��
            ScanOrdinalPlan();
            
            usleep(MAIN_CYCLE_DELAY);
        }
    }
    catch (TOCIException &e) {
        cout << e.getErrMsg () << endl << e.getErrSrc()<<endl;
        TWfLog::log(WF_FILE, "%s \n %s\n", e.getErrMsg(),e.getErrSrc() );
    }
    catch (Exception &e) {
        cout << e.descript() << endl;
        TWfLog::log(WF_FILE, e.descript() );
    }
    catch (...) {
        cout << "err." <<endl;
        TWfLog::log(WF_FILE,"���ػ�����(guard_main)�쳣�˳���δ֪����");
    }
    
    delete g_pRouteTable;
    delete g_pTimeRunPlanHead;
    delete g_pOrdinalPlanHead;
    //delete g_pOrgMgr;
}

////////////////////////////////////////////////////////////////

void  UpdateProcState (int _iProID)
{
    
    //�� AppInfo(WF_STOP) ״̬
    //g_pShmTable->AppInfo[iIdx].m_cState = WF_;
    g_pShmTable->changeState(_iProID,WF_STOP);
    
    //�� ServAppInfo(WF_STOP) ״̬
    if (!g_pRouteTable)
        g_pRouteTable = new TRouteTable;
    
    g_pRouteTable->changeState(_iProID,WF_STOP);
}

int DestroyGuardMainShm ()
{
    int iRet=0;
    char sCommand[100];
    
    GetWorkFlowKey ();

    TWfLog::log (0,"��������������ɾ�� ...");
    cout<<"\n��������������ɾ�� ..."<<endl;
    
    if (g_lBillMntKey > 0) {
        sprintf (sCommand,"ipcrm -M 0x%x",g_lBillMntKey);
        cout<<"\nExecute: "<<sCommand<<"("<<g_lBillMntKey<<")"<<endl;
        system(sCommand);
    }
    if (g_lWorkFlowKey > 0) {
        sprintf (sCommand,"ipcrm -M 0x%x",g_lWorkFlowKey);
        cout<<"\nExecute: "<<sCommand<<"("<<g_lWorkFlowKey<<")"<<endl;
        system(sCommand);
    }    
    if (g_lLockWorkFlowKey > 0) {
        sprintf (sCommand,"ipcrm -S 0x%x",g_lLockWorkFlowKey);
        cout<<"\nExecute: "<<sCommand<<"("<<g_lLockWorkFlowKey<<")"<<endl;
        system(sCommand);
    }
    if (g_lLockApplicationKey > 0) {
        sprintf (sCommand,"ipcrm -S 0x%x",g_lLockApplicationKey);
        cout<<"\nExecute: "<<sCommand<<"("<<g_lLockApplicationKey<<")"<<endl;
        system(sCommand);
    }
    if (g_lLockClientKey > 0) {
        sprintf (sCommand,"ipcrm -S 0x%x",g_lLockClientKey);
        cout<<"\nExecute: "<<sCommand<<"("<<g_lLockClientKey<<")"<<endl;
        system(sCommand);
    }
    sleep(1);
    cout<<"\n��������������ɾ�� Done!"<<endl;
        
    return 0;
}

int GetProcNum ()
{
    //char sTemp[2000];
    int iRet;
    MonitorInfo oInfoMonitor;
    oInfoMonitor.getInfoCfg();
    iRet=*oInfoMonitor.m_piNum;
    return iRet;
}

void ShutdownGuard ()
{
    if (AttachShm ((MAX_APP_NUM)*sizeof (THeadInfo), (void **)&pInfoHead) >= 0) {
        
        int iSysPID = pInfoHead->iSysPID;
        DetachShm ();
        
        if (kill(iSysPID,0) != 0) {
            cout << "\n����ػ�����GuardMain������!"<<endl;
            return;
        }
        
        kill(iSysPID,SIGUSR1);
        
        while (kill(iSysPID,0) == 0) { //�ػ�������Ȼ����
            static int iTimes =0;
            if (iTimes>3) {
                cout << "\n����ػ�����GuardMain(PID:"<<iSysPID<<")ֹͣʧ��!"<<endl;
                return;
            }
            iTimes ++;
            sleep(1);
        }
        TWfLog::log (0,"����ػ�����GuardMain(PID:%d)��ֹͣ!",iSysPID);
        cout<<"\n����ػ�����GuardMain(PID:"<<iSysPID<<")��ֹͣ!"<<endl;
    } 
    else { //��������������
        cout << "\n����ػ�����GuardMain��������������"<<endl;
    }
    return ;
}

void CheckGuardMianExist()
{
    if (AttachShm ((MAX_APP_NUM)*sizeof (THeadInfo), (void **)&pInfoHead) >= 0) {
        
        if (kill(pInfoHead->iSysPID,0) == 0) { //��ǰ����������Ӧ�ļ���ػ����̴���
            TWfLog::log (0,"����ػ�����GuardMain(PID:%d)�Ѵ���, ����Ҫ�ٴ�����!",pInfoHead->iSysPID);
            cout << "����ػ�����GuardMain(PID:"<<pInfoHead->iSysPID<<")�Ѵ���,����Ҫ�ٴ�����!"<<endl;
            DetachShm ();
            exit (0);
        }
        DetachShm ();
    } 
    return ;
}


void InitProcInfoQry (TOCIQuery &qry)
{

}

int InitProcShm ()
{
    int i = 0, iReloadFlag = 0, iProcNum;
    char sTemp[2000];
    ABMException oExp;
    char sExecPath[256];
    //char *penv = getenv("ABMAPP_DEPLOY");
    char *penv = getenv("ABM_PRO_DIR");
    if (!penv) {
        //ADD_EXCEPT0(oExp, "miss environment ABMAPP_DEPLOY, please add it to your.profile");
        ADD_EXCEPT0(oExp, "miss environment ABM_PRO_DIR, please add it to your.profile");
        return 0;
    }
    if (penv[strlen(penv)-1] != '/') {
        snprintf (sExecPath, sizeof(sExecPath)-1, "%s/bin",penv);
        //snprintf (sExecPath, sizeof(sExecPath)-1, "%s/src/app_abmmonitor",penv);
    } else {
        snprintf (sExecPath, sizeof(sExecPath)-1, "%sbin",penv);
        //snprintf (sExecPath, sizeof(sExecPath)-1, "%ssrc/app_abmmonitor",penv);
    }
    pLogName = getenv("LOGNAME");
    
    //---changed by jiangmj 2008��08��07
        g_iLocalHostID = 1;

    try {
        iProcNum = GetProcNum();
        if ( iProcNum > (MAX_APP_NUM-5-(MAX_APP_NUM/10)) ) {
            // ���������ռ��п��ܲ���
            sprintf (sTemp,"ϵͳ��������Ľ�����(MAX_APP_NUM=%d)���ܲ�����Ҫ��!"
                "\n--��ǰABMϵͳ���ŵĽ�����Ϊ:%d \n"
                "������ $TIBS_HOME/config/LocalDefine.h �������ػ�����",MAX_APP_NUM,iProcNum);
            TWfLog::log (0,sTemp);
            cout<<sTemp<<endl;
            return -1;
        }
        
        if (AttachShm (MAX_APP_NUM*sizeof (THeadInfo), (void **)&pInfoHead) >= 0) {
            sprintf (sTemp,"monitor��ع����ڴ�(%ld)--����--�ɹ�!", g_lBillMntKey);
            TWfLog::log (0,sTemp);
            cout<<sTemp<<endl;
            iReloadFlag = 0;
        } 
        else if (CreateShm (MAX_APP_NUM*sizeof (THeadInfo), (void **)&pInfoHead) < 0) {
            sprintf (sTemp,"monitor��ع����ڴ�(%ld)����ʧ��!", g_lBillMntKey);
            TWfLog::log (0,sTemp);
            cout<<sTemp<<endl;
            return -1;
        }
        else {
            iReloadFlag = 1;
            sprintf (sTemp,"monitor��ع����ڴ�(%ld)<<����>>�ɹ�!", g_lBillMntKey);
            TWfLog::log (0,sTemp);
            cout<<sTemp<<endl;
        }    
        
        pInfoHead->iSysPID = getpid ();
        pInfoHead->iProcNum = iProcNum;
        pProcInfoHead = &pInfoHead->ProcInfo;
    }
    catch (TException &e) {
        cout << "CreateShm error" <<endl;
        return -1;
    }
    
    if (iReloadFlag == 0)
        return 0;

    try {
        MonitorInfo oInfoMonitor;
        oInfoMonitor.getInfoCfg();
        oInfoMonitor.m_oSHMCSemaphore.P(); 
        while(i<(*oInfoMonitor.m_piNum)){
            
            (pProcInfoHead+i)->iBillFlowID = 7;
            (pProcInfoHead+i)->iAppID			 = oInfoMonitor.m_poMonitorCfg[i].m_iAppID;
            (pProcInfoHead+i)->iProcessID  = oInfoMonitor.m_poMonitorCfg[i].m_iProcessID;
            (pProcInfoHead+i)->iMqID   = oInfoMonitor.m_poMonitorCfg[i].m_iRecvMsg;
            (pProcInfoHead+i)->iState      = ST_SLEEP;
            strcpy ((pProcInfoHead+i)->sName, oInfoMonitor.m_poMonitorCfg[i].m_sProcessName);
            (pProcInfoHead+i)->iVpID = oInfoMonitor.m_poMonitorCfg[i].m_lVpID;
            (pProcInfoHead+i)->sName[20] = 0;
            
            char sName[256];
            strcpy (sName, oInfoMonitor.m_poMonitorCfg[i].m_sProcessName);
            strcpy ((pProcInfoHead+i)->sExecPath, sExecPath);
            strcpy ((pProcInfoHead+i)->sExecFile, oInfoMonitor.m_poMonitorCfg[i].m_sAppExec);
            strcpy ((pProcInfoHead+i)->sParam, oInfoMonitor.m_poMonitorCfg[i].m_sParammeter);

            i++;
        }
        oInfoMonitor.m_oSHMCSemaphore.V();

    } catch (TOCIException &e) {
        cout << "Exception: " << e.getErrMsg ();
        return -1;
    }

    return 0;
}

void SetProcInfo(int iSeek, TProcessInfo *_pInfo)
{
    (pProcInfoHead+iSeek)->iBillFlowID = _pInfo->iBillFlowID;
    (pProcInfoHead+iSeek)->iAppID			 = _pInfo->iAppID;
    (pProcInfoHead+iSeek)->iProcessID = _pInfo->iProcessID;
    (pProcInfoHead+iSeek)->iMqID = _pInfo->iMqID;
    (pProcInfoHead+iSeek)->iState = ST_SLEEP;
    strcpy ((pProcInfoHead+iSeek)->sName, _pInfo->sName);
    strcpy ((pProcInfoHead+iSeek)->sExecPath, _pInfo->sExecPath);
    strcpy ((pProcInfoHead+iSeek)->sExecFile, _pInfo->sExecFile);
    strcpy ((pProcInfoHead+iSeek)->sParam, _pInfo->sParam);
    (pProcInfoHead+iSeek)->iVpID			 = _pInfo->iVpID;
    (pProcInfoHead+iSeek)->iFreeFlag = 0; //ռ�ñ�ʶ
}


void CheckMonitorApp ()
{
    for (int i=0; i<pInfoHead->iProcNum; i++) {
        if ((pProcInfoHead+i)->iState == ST_RUNNING)
            if (kill ((pProcInfoHead+i)->iSysPID,0))
                (pProcInfoHead+i)->iState = ST_ABORT;
    }
}


void UpdateProcShmInfo (TProcessInfo *_pInfo)
{
    bool bExist = false;
    int i;
        
    for (i=0; i<pInfoHead->iProcNum; i++) {
        if (((pProcInfoHead+i)->iProcessID == _pInfo->iProcessID)&&((pProcInfoHead+i)->iAppID == _pInfo->iAppID)) {
            int iStateTmp = (pProcInfoHead+i)->iState;
            SetProcInfo (i, _pInfo);
            (pProcInfoHead+i)->iState = iStateTmp;
            return;
        }
    }

    //���Բ���һ�����ñ�����ɾ���Ľ��̶�Ӧ�ļ�¼�ڵ�,����֮
    for (int iSeek=0; iSeek<pInfoHead->iProcNum; iSeek++){
        if ((pProcInfoHead+iSeek)->iProcessID == -1) {
            SetProcInfo(iSeek, _pInfo);
            return;
        }        
    }    
    
    if (pInfoHead->iProcNum >= (MAX_APP_NUM-2)) {
        char sTemp[2000];
        sprintf (sTemp,"������������������(ProcNum:%d),"
            "����ProcessID=%d ������Ϣ����ʧ��!",pInfoHead->iProcNum,_pInfo->iProcessID);
        TWfLog::log (sTemp);
        cout<<endl<<sTemp<<endl;
        return ;
    }
    
    SetProcInfo (i, _pInfo);
    
    pInfoHead->iProcNum ++;
}

// ˢ��monitor��ع����ڴ�Ľ���������Ϣ
// ����: ��ǰ̨MC�޸������������Ϣ,�ύˢ���ź�ʱ, �ػ����̵��ô˺���,
//       ˢ��monitor���������Ľ���������Ϣ   
void RefreshProcShm ()
{
    char sTemp[2048];
    TProcessInfo tmpProcInfo, *pInfo;
    pInfo = &tmpProcInfo;
    ABMException oExp;
    char sExecPath[256];
    //char *penv = getenv("ABMAPP_DEPLOY");
    char *penv = getenv("ABM_PRO_DIR");
    if (!penv) {
        ADD_EXCEPT0(oExp, "miss environment ABMAPP_DEPLOY, please add it to your.profile");
        return ;
    }
    if (penv[strlen(penv)-1] != '/') {
        snprintf (sExecPath, sizeof(sExecPath)-1, "%s/bin",penv);
    } else {
        snprintf (sExecPath, sizeof(sExecPath)-1, "%sbin",penv);
    }
    for (int i=0; i<pInfoHead->iProcNum; i++) {
        (pProcInfoHead+i)->iFreeFlag = 1;
    }
    try 
    {
        MonitorInfo oInfoMonitor;
        oInfoMonitor.getInfoCfg();
        oInfoMonitor.m_oSHMCSemaphore.P();
        for(int iRet=0;iRet<(*oInfoMonitor.m_piNum);++iRet){

            pInfo->iBillFlowID = 7;
            pInfo->iAppID			 = oInfoMonitor.m_poMonitorCfg[iRet].m_iAppID;
            pInfo->iProcessID  = oInfoMonitor.m_poMonitorCfg[iRet].m_iProcessID;
            pInfo->iMqID   = oInfoMonitor.m_poMonitorCfg[iRet].m_iRecvMsg;
            strcpy (pInfo->sName, oInfoMonitor.m_poMonitorCfg[iRet].m_sProcessName);
            strcpy (pInfo->sExecPath, sExecPath);
            strcpy (pInfo->sExecFile, oInfoMonitor.m_poMonitorCfg[iRet].m_sAppExec);
            strcpy (pInfo->sParam,  oInfoMonitor.m_poMonitorCfg[iRet].m_sParammeter);
            
            pInfo->iVpID = oInfoMonitor.m_poMonitorCfg[iRet].m_lVpID;
            UpdateProcShmInfo (pInfo);
        }
        oInfoMonitor.m_oSHMCSemaphore.V();

    } catch (TOCIException &e) {
        cout << "Exception: " << e.getErrMsg ();
        return ;
    }
    
    //������нڵ�
    for (int i=0; i<pInfoHead->iProcNum; i++) {
        if ((pProcInfoHead+i)->iFreeFlag == 1)
            (pProcInfoHead+i)->iProcessID = -1;
    }
}

int ExecuteBootDown ()
{
    
    {//add by yks
        ExecuteBootDown_v2();
        return 0;
    }
    
    int i, j, ret;
    char env1[80], env2[80], *argv[10], *p, sBuff[200], sExecFile[200];
    long pid;

    for (i=0; i<pInfoHead->iProcNum; i++) {
        switch ((pProcInfoHead+i)->iState) {
        case ST_WAIT_BOOT:
            (pProcInfoHead+i)->iState = ST_INIT;
            pid = fork ();
            if (pid == 0) {
                char sPath[256];

                strcpy (sPath, (pProcInfoHead+i)->sExecFile);
                j = strlen (sPath)-1;
                for (; j>=0; j--) {
                    if (sPath[j] == '/')
                        break;
                    sPath[j] = 0;
                }
                if (j == 0)
                    strcpy (sExecFile, (pProcInfoHead+i)->sExecFile);
                else
                    strcpy (sExecFile, &(pProcInfoHead+i)->sExecFile[j+1]);

                ret = chdir ((pProcInfoHead+i)->sExecPath);

                sprintf (env1, "PROCESS_ID=%d", (pProcInfoHead+i)->iProcessID);
                printf ("putenv: %s\n", env1);
                putenv (env1);

                sprintf (env2, "BILLFLOW_ID=%d", (pProcInfoHead+i)->iBillFlowID);
                printf ("putenv: %s\n", env2);
                putenv (env2);

                argv[0] = sExecFile;
                strcpy (sBuff, (pProcInfoHead+i)->sParam);
                p = strtok (sBuff, " ");
                for (j=1; j<10; j++) {
                    if (p == NULL) break;
                    argv[j] = strdup (p);
                    p = strtok (NULL, " ");
                }
                argv[j] = NULL;
                
                usleep (100000);
                printf ("[Message]: Start command:%s %s\n",
                        (pProcInfoHead+i)->sExecFile, (pProcInfoHead+i)->sParam);
                execve ((pProcInfoHead+i)->sExecFile, argv, environ);
                exit (0);
            
            } else {
                printf ("Child process created, pid=%d\n", (int)pid);
                (pProcInfoHead+i)->iSysPID = pid;
            }
            break;

        case ST_WAIT_DOWN:
            kill ((pProcInfoHead+i)->iSysPID, SIGUSR1);
            (pProcInfoHead+i)->iState = ST_KILLED;
            printf ("Child process killed, pid=%d\n", (pProcInfoHead+i)->iSysPID);
    
            waitpid (-1, NULL, WNOHANG|WUNTRACED);
            
            break;
            
        }
    }

    waitpid (-1, NULL, WNOHANG|WUNTRACED);

    return 0;
}

int ExecuteBootDown_v2 ()
{
    int i, iRet, iProcessLogID ;
    int iFailedReason = 0;
    
    if (pInfoHead->iRefreshFlag == 1) {
        RefreshProcShm ();
        g_pRouteTable->reload();
        pInfoHead->iRefreshFlag = 0;
    }
    
    for (i=0; i<pInfoHead->iProcNum; i++) {
        switch ((pProcInfoHead+i)->iState) {
        case ST_WAIT_BOOT:
            
            (pProcInfoHead+i)->iState = ST_INIT;
            
            iProcessLogID = TLogProcessMgr::insertLogProcess(
                (pProcInfoHead+i)->iProcessID,"start",0);
            
            iRet = 0;
            if (g_pRouteTable) {
                iRet = g_pRouteTable->startApp((pProcInfoHead+i)->iProcessID,
                            iProcessLogID, &iFailedReason);
            }
            if (iRet==0) {
                
                if (iFailedReason == -1) { //�����Ѿ���������
                    (pProcInfoHead+i)->iState = ST_RUNNING;
                    return 0;
                }
                
                (pProcInfoHead+i)->iState = ST_ABORT;
                TWfLog::log (0,"��������ʧ��! (ProcessID:%d)",(pProcInfoHead+i)->iProcessID);
                cout<<"\n��������ʧ��! (ProcessID:"<<(pProcInfoHead+i)->iProcessID<<")"<<endl;
                return 0;
            }
            
            break;

        case ST_WAIT_DOWN:
            (pProcInfoHead+i)->iState = ST_DESTROY;
            if (kill((pProcInfoHead+i)->iSysPID,0) == 0 && (pProcInfoHead+i)->iSysPID>0) 
            {
                kill ((pProcInfoHead+i)->iSysPID, SIGUSR1);
                usleep (5000);
                printf ("send signal(SIGUSR1) to Child process,pid=%d\n", (pProcInfoHead+i)->iSysPID);
            
                waitpid (-1, NULL, WNOHANG|WUNTRACED);
            }
            else {
                (pProcInfoHead+i)->iState = ST_KILLED;
            }
            break;
        case ST_DESTROY:
            if (kill((pProcInfoHead+i)->iSysPID,0) != 0) 
            {
                (pProcInfoHead+i)->iState = ST_KILLED;
            }
            break;
        case ST_ENFORCE_DOWN:
            (pProcInfoHead+i)->iState = ST_ENDORCE_DESTROY;
            if (kill((pProcInfoHead+i)->iSysPID,0) == 0 && (pProcInfoHead+i)->iSysPID>0) 
            {
                kill ((pProcInfoHead+i)->iSysPID, 9);
                usleep (5000);
                printf ("send signal(SIGUSR1) to Child process,pid=%d\n", (pProcInfoHead+i)->iSysPID);
            
                waitpid (-1, NULL, WNOHANG|WUNTRACED);
            }
            else {
                (pProcInfoHead+i)->iState = ST_ENDORCE_KILLED;
            }
            break;
        case ST_ENDORCE_DESTROY:
            if (kill((pProcInfoHead+i)->iSysPID,0) != 0) 
            {
                (pProcInfoHead+i)->iState = ST_ENDORCE_KILLED;
            }
            break;
        }
    }

    waitpid (-1, NULL, WNOHANG|WUNTRACED);

    return 0;
}

//// mnt end ////

void ResponseMntConnect()
{
    TMntReqInfo *pt = &(g_pShmTable->MntReqInfo);
    
    if (pt->m_iStatus == MNT_REQ_REGISTER ) //��¼
    {
        if (pt->m_iRegNum >= MAX_MNT_NUM ) { //�������ﵽ���
            pt->m_iStatus = REG_REJECT_MAXNUM;
            return ;
        }

        int iIndex = GetNextMonitorIndex(); //��ȡ��һ�������±�
        if (iIndex == -1) {
            pt->m_iStatus = REG_REJECT_MAXNUM;
            return ;            
        }
        
        //����������Ϣ
        g_aMonitorFront[iIndex].init();
        g_aMonitorFront[iIndex].m_iAuthID = pt->m_igAuthID++;
		//����Billflowid 8��13��
        g_aMonitorFront[iIndex].m_iBillFlowID = pt->m_iBillFlowID;
        g_aMonitorFront[iIndex].parseOrgIDStr(pt->m_sOrgIDStr);
        g_aMonitorFront[iIndex].m_iStaffID = pt->m_iStaffID;
        g_aMonitorFront[iIndex].m_iLogMode = pt->m_iLogMode;
        strcpy(g_aMonitorFront[iIndex].m_sHostAddr, pt->m_sHostAddr);        
        g_aMonitorFront[iIndex].m_iStatus = 1;
        
        pt->m_iRegNum++; //����������Ŀ

        //���ش�����
        pt->m_iIndex = iIndex;
        pt->m_iAuthID = g_aMonitorFront[iIndex].m_iAuthID;
        pt->m_iStatus = REG_ACCEPT;
        
        return;
    }
    else if ( pt->m_iStatus == MNT_REQ_UNREGISTER ) //�˳�
    {
        if (pt->m_iAuthID != g_aMonitorFront[pt->m_iIndex].m_iAuthID || 
            g_aMonitorFront[pt->m_iIndex].m_iStatus == -1)
        {
            pt->m_iStatus = MNT_LOG_DEAD;
            return ;
        }
        
        g_aMonitorFront[pt->m_iIndex].m_iStatus = -1;

        pt->m_iRegNum--; //����������Ŀ
        
        // �ı�Ӧ�ó���״̬�����б��״̬
        //...
        
        pt->m_iStatus = REG_UNREGISTERED;
                
        return;
    }
    return;
}

//������� _iBillFlowID �Ƿ���ڿ�������,��-�����±�, ��-����-1
int CheckMonitorCtrlExist(int _iBillFlowID)
{
    for (int i=0; i<MAX_MNT_NUM; i++) {
        if (g_aMonitorFront[i].m_iStatus == 1 && g_aMonitorFront[i].m_iLogMode == LOG_AS_CONTROL )
            return i;
    }
    return -1;
}

//��ȡ��һ�������±�, �� ����-1
int GetNextMonitorIndex()
{
    for (int i=0; i<MAX_MNT_NUM; i++) {
        if (g_aMonitorFront[i].m_iStatus == -1)
            return i;
    }
    return -1;
}


////////////////////////////////////////////////////////////////

void ResponseClientConnect()
{
    char sHostName[100];
    
    TClientReqInfo *pclt = &(g_pShmTable->ClientReqInfo);
    
    if (pclt->m_iStatus == REQ_CLIENT_LOGIN ) {
        
        g_pRouteTable->getHostNameByHostID (pclt->m_iHostID, sHostName);
        
        if (pclt->m_iRegNum == MAX_CLT_NUM ) { //Զ���������ﵽ���
            pclt->m_iStatus = MAX_CLIENT_NUM;
            return;
        }
        
        //����¼��iHostID�Ƿ���Ч
        if (!g_pRouteTable->touchHostID(pclt->m_iHostID)) {
            strcpy(g_pShmTable->ClientReqInfo.m_sMsgBuf,"����ID��Ч");
            pclt->m_iStatus = SERVER_REJECT;
            return;
        }
        
        //��� ��HostID����Ӧ�û����� �Ƿ��Ѿ�����  g_aClientLink[MAX_CLT_NUM]
        int iIdx;
        for (iIdx=0; iIdx<MAX_CLT_NUM; iIdx++ ) {
            if (g_aClientLink[iIdx].m_iHostID == pclt->m_iHostID 
             && !strcmp(g_aClientLink[iIdx].m_sSysUserName, pclt->m_sSysUserName)
            ){
                strcpy(g_pShmTable->ClientReqInfo.m_sMsgBuf,"�ظ���¼");
                pclt->m_iStatus = SERVER_REJECT;
                return;
            }
        }
        
        for (iIdx=0; iIdx<MAX_CLT_NUM; iIdx++ ) {
            if (g_aClientLink[iIdx].m_iHostID == -1)
                break;
        }
        if (iIdx == MAX_CLT_NUM ) {
            strcpy(g_pShmTable->ClientReqInfo.m_sMsgBuf,"Զ������������!");
            TWfLog::log(WF_FILE,"GuardRemote login fail. Can't find a useful g_aClientLink[?].m_iHostID == -1");
            cout<<"\n"<<endl;
            pclt->m_iStatus = SERVER_REJECT;
            return;
        }
        
        pclt->m_iIndex = iIdx ;

        g_aClientLink[iIdx].m_iHostID = pclt->m_iHostID ;
        strcpy (g_aClientLink[iIdx].m_sSysUserName,pclt->m_sSysUserName) ;
        g_aClientLink[iIdx].m_oLastTouch.getCurDate() ;
        
        TWfLog::log(WF_FILE," ^_^ GuardRemote Login ACCEPT! "
            "\n   -->IndexID:%d, HostID:%d, HostName:%s, UserName:%s",
            pclt->m_iIndex, pclt->m_iHostID,sHostName, pclt->m_sSysUserName);
        
        pclt->m_iRegNum++ ;
        pclt->m_iStatus = SERVER_ACCEPT ;
        
        return;
    }
    else if ( pclt->m_iStatus == REQ_CLIENT_LOGOUT ) {
        
        g_pRouteTable->getHostNameByHostID (pclt->m_iHostID, sHostName);
        
        if (g_aClientLink[pclt->m_iIndex].m_iHostID != pclt->m_iHostID 
          || strcmp(g_aClientLink[pclt->m_iIndex].m_sSysUserName,pclt->m_sSysUserName)
        ){
            TWfLog::log(WF_FILE,"GuardRemote Logout failed. HostID:%d, HostName:%s, UserName:%s",
                pclt->m_iHostID, sHostName, pclt->m_sSysUserName);
            pclt->m_iStatus = SERVER_REJECT;
            return;
        }
        
        TWfLog::log(WF_FILE," ^_^ GuardRemote LogOut Ok!! "
            "\n   -->IndexID:%d, HostID:%d, HostName:%s, UserName:%s",
            pclt->m_iIndex, pclt->m_iHostID, sHostName, pclt->m_sSysUserName);
        
        g_aClientLink[g_pShmTable->ClientReqInfo.m_iIndex].init(); //Զ�̼���˳�ʱ,�ָ������ӻ����ԭʼ״̬
        pclt->m_iRegNum--;
        pclt->m_iStatus = SERVER_ACCEPT;
        return;
    }    
}

////////////////////////////////////////////////////////////////

void ResponseSendCommand()
{
    char sProcSysUserName[32], sHostName[100];
    int iProcHostID;
    int iRes, iProcessLogID;
    
    TMntReqInfo *pMntRq = &(g_pShmTable->MntReqInfo);

    if (pMntRq->m_iStatus != REQ_SEND_COMMAND) //��ָ��
        return;
    
    TMonitorFront *pFront = &(g_aMonitorFront[pMntRq->m_iIndex]);
    
    // check iIndexID and iAuthID
    if (pFront->m_iStatus == -1 || pFront->m_iAuthID != pMntRq->m_iAuthID) {
        TWfLog::log(WF_FILE,"ResponseSendCommand: Check AuthID err. "
            "rq_iIndex:%d,rq_iAuthID:%d,true_AuthID:%d",
            pMntRq->m_iIndex, pMntRq->m_iAuthID,pFront->m_iAuthID);
        
        pMntRq->m_iStatus = MNT_LOG_DEAD; //log dead.
        return;
    }
    
    // check privilege
    if (pFront->m_iLogMode != LOG_AS_CONTROL) {
        pMntRq->m_iStatus = NO_PRIVILEGE;
        return;
    }
        
    iProcHostID = g_pRouteTable->getHostIDByProcessID(pMntRq->m_iProcessID,sProcSysUserName);
    
    if (iProcHostID != g_iLocalHostID || strcmp(pLogName,sProcSysUserName)) {//Զ������ָ��
        int iIdx = CheckClientExist(iProcHostID, sProcSysUserName);
        if (iIdx<0 ) {
            TWfLog::log(WF_FILE,"ResponseSendCommand: REMOTECMA_NOT_START, \n"
                "HostID:%d, UserName:%s, ProcessID:%d \n"
                "Զ��ָ�� %s δ�ܷ���ִ��!",
                iProcHostID, sProcSysUserName, pMntRq->m_iProcessID, pMntRq->m_sMsgBuf);
            g_pRouteTable->getHostNameByHostID (iProcHostID,sHostName);
            sprintf (pMntRq->m_sMsgBuf,"%s:%s",sHostName,sProcSysUserName);
            pMntRq->m_iStatus = REMOTECMA_NOT_START;
            return;
        }
        
        if (!g_aClientLink[iIdx].pushMntCommand(pMntRq->m_sMsgBuf))
        {//push failed.
            sprintf(pMntRq->m_sMsgBuf,"ResponseSendCommand: �쳣,ͬһʱ��ĳһԶ��������Ҫִ�ж���ָ��");
            TWfLog::log(WF_FILE,pMntRq->m_sMsgBuf);
            pMntRq->m_iStatus = COMMAND_EXEC_ERR;
        }
        
        pMntRq->m_iStatus = REQ_COMMAND_SENDING;
        
        return;
    }
    
    //�Ǳ���ָ��
    TServerAppInfo* pServApp = g_pRouteTable->getServerAppInfo(pMntRq->m_iProcessID);
    
    //check proces exists
    if (!pServApp) {
        sprintf(pMntRq->m_sMsgBuf,"ָ��ProcessID:%d �������б����",pMntRq->m_iProcessID);
        pMntRq->m_iStatus = COMMAND_EXEC_ERR;
        return;
    }
    
    char sOperate[10];
    if (pMntRq->m_iOperation == 1) sprintf(sOperate,"start");
    if (pMntRq->m_iOperation == 2) sprintf(sOperate,"stop");
        
    switch( pMntRq->m_iOperation) {
        case 1: // start
            iProcessLogID = TLogProcessMgr::insertLogProcess(pMntRq->m_iProcessID,
                (const char *)sOperate,pFront->m_iStaffID);
            iRes = g_pRouteTable->startApp(pMntRq->m_iProcessID,iProcessLogID);
            break;
            
        case 2: // stop
            iRes = g_pRouteTable->stopApp(pMntRq->m_iProcessID);
            break;
            
        default: //unknown
            sprintf(pMntRq->m_sMsgBuf,"������ʽiOperationID:%d ����ʶ��",pMntRq->m_iOperation);
            pMntRq->m_iStatus = COMMAND_EXEC_ERR;
            return;
    }
    
    if (iRes == 0) { //ָ��ִ��ʧ��
        sprintf(pMntRq->m_sMsgBuf,"ResponseSendCommand: ָ��ִ��ʧ�� - WF_GM001");
        pMntRq->m_iStatus = COMMAND_EXEC_ERR;
        return;        
    }
    
    pMntRq->m_iStatus = REQ_COMMAND_SENT;
    return;
}

//���Զ�������Ƿ���� ��-�����±�, ��-����-1
int CheckClientExist(int _iHostID, char _sSysUserName[])
{
    for (int iIdx = 0; iIdx<MAX_CLT_NUM; iIdx++) {
        if (g_aClientLink[iIdx].m_iHostID == _iHostID \
                && !strcmp(g_aClientLink[iIdx].m_sSysUserName, _sSysUserName))
            return iIdx;
    }    
    return -1;
}

////////////////////////////////////////////////////////////////

void ResponseGetCommand()
{
    TClientReqInfo *pCltRq = &(g_pShmTable->ClientReqInfo);
    TClientInfo *pCltInfo = &(g_aClientLink[pCltRq->m_iIndex]);
    
    if (pCltRq->m_iStatus != REQ_GET_COMMAND)
        return;
    
    if (pCltRq->m_iHostID != pCltInfo->m_iHostID){
        pCltRq->m_iStatus = CLIENT_LOG_DEAD;
        return;
    }
    
    TParseBuff ParaList;
    char sCmd[1024];
    int iRet = pCltInfo->getCommand(sCmd);
    if (iRet == 0) { //no command
        pCltRq->m_iStatus = REQ_NO_COMMAND;
        return;
    }
    else if (iRet == 2) { //command exist, but there is a command not acked.
        pCltRq->m_iStatus = REQ_NO_COMMAND;
        TWfLog::log(WF_FILE,"Զ�̼�ؿͻ���δ�ܻ�ȡ��ִ��ָ���Ϊ����������δACK��ָ��");
        usleep(500000);
        return;
    }
    
    //a command have got.    
    iRet = ParaList.parseBuff(sCmd);
    if (iRet == 0) { // exception: parse err
        pCltRq->m_iStatus = REQ_NO_COMMAND;
        TWfLog::log(WF_FILE,"Response_GetCommand Parse err. ����Զ��ָ��ʧ��. Command:%s",sCmd);
        return;
    }
    
    pCltRq->m_iResult = ParaList.getResult();
    pCltRq->m_iOperation = ParaList.getOperation();
    pCltRq->m_iProcessID = ParaList.getProcessID();
    pCltRq->m_iStaffID = ParaList.getStaffID();
    pCltRq->m_iPlanID = ParaList.getPlanID();
    
    pCltRq->m_cState = ParaList.getState(); //add 2007.01.29
    
    if (pCltRq->m_iResult == 0)
        strcpy(pCltRq->m_sMsgBuf,ParaList.getMsg());
    
    pCltRq->m_iStatus = REQ_COMMAND_RECV;

    //cout<<"GuardMain send to Rmt Command:"<<sCmd<<endl;
        
    return;
}

////////////////////////////////////////////////////////////////

void ResponseAckCommand()
{
    char sTmp[100];
    TClientReqInfo *pCltRq = &(g_pShmTable->ClientReqInfo);
    TClientInfo *pCltInfo = &(g_aClientLink[pCltRq->m_iIndex]);
    TMntReqInfo *pMntRq = &(g_pShmTable->MntReqInfo);
    
    if (pCltRq->m_iStatus != REQ_ACK_COMMAND)
        return;
    
    //check alive
    if (pCltRq->m_iHostID != pCltInfo->m_iHostID 
        || strcmp(pCltRq->m_sSysUserName, pCltInfo->m_sSysUserName)
    ){
        pCltRq->m_iStatus = CLIENT_LOG_DEAD;
        return;
    }
    
    if (!pCltInfo->ackCommand()) {
        TWfLog::log(WF_FILE,"ResponseAckCommand,pCltInfo->ackCommand return 0. ");
        return;
    }
    
    if (pCltRq->m_iResult == 0) { //Զ��ָ��ִ��ʧ��

        strcpy(pMntRq->m_sMsgBuf, pCltRq->m_sMsgBuf);
        
        if (pMntRq->m_iStatus == REQ_COMMAND_SENDING) {
            pMntRq->m_iStatus = COMMAND_EXEC_ERR;
        }
        else if (pMntRq->m_iStatus == REQ_CHANGESTATUS_OVER) {
            pMntRq->m_iStatus = REQ_CHANGESTATUS_ERR;
        }
        else if (pMntRq->m_iStatus == RUNPLAN_REFRESHED) {
            pMntRq->m_iStatus = REFRESHED_ERR;
        }
        
        pCltRq->m_iStatus = ACK_COMMAND_END; 
        
        return;
    }
    
    //Զ��ָ��ִ�гɹ�    
    switch (pMntRq->m_iStatus) {
        case REQ_COMMAND_SENDING :
            pMntRq->m_iStatus = REQ_COMMAND_SENT;
            
            //�������������ָ��, ��ҪΪ�����ɵĽ�����������з��� iMainGuardIndex
            if (pCltRq->m_iOperation == 1)
            {
                int iMainGuardIndex, iRet;
                iRet = g_pShmTable->getIdleAppInfo(pCltRq->m_iProcessID, iMainGuardIndex);
                if (iRet == 1) {
                    g_pShmTable->AppInfo[iMainGuardIndex].m_iProcessID = pCltRq->m_iProcessID;
                }
                else {
                    TWfLog::log(WF_FILE,"δ��ΪԶ��Ӧ�ó������iMainGuardIndex, getIdleAppInfo return:%d",iRet);
                }
            }
            break;
        case REQ_STATUS_CHANGEING :
            pMntRq->m_iStatus = REQ_CHANGESTATUS_OVER;
            sprintf (sTmp,"<iProcessID=%d><cState=%c>",pCltRq->m_iProcessID,pCltRq->m_cState);
            PushMntMsg(sTmp, pCltRq->m_iProcessID);
            break;
        case REFRESHING_RUNPLAN :
            //if (CheckIfAllLinkAcked())
                pMntRq->m_iStatus = RUNPLAN_REFRESHED;
            break;
        default :
            TWfLog::log(WF_FILE,"ResponseAckCommand, Զ�̷���ָ��ִ�н��,��ǰ̨���ǵȴ����״̬");
            break;
    }
    pCltRq->m_iStatus = ACK_COMMAND_END;
}

////////////////////////////////////////////////////////////////

void ResponseChangeStatus()
{
    char sProcSysUserName[32];
    
    TMntReqInfo   *pMntRq = &(g_pShmTable->MntReqInfo);
    TMonitorFront *pFront = &(g_aMonitorFront[pMntRq->m_iIndex]);
    
    if (pMntRq->m_iStatus != REQ_CHANGESTATUS) //no changestatus command.
        return;

    // check iIndexID and iAuthID
    if (pMntRq->m_iStatus == -1 || pFront->m_iAuthID != pMntRq->m_iAuthID) {
        TWfLog::log(WF_FILE,"ResponseSendCommand: Check AuthID err. rq_iIndex:%d,rq_iAuthID:%d,true_AuthID:%d",
            pMntRq->m_iIndex, pMntRq->m_iAuthID,pFront->m_iAuthID);
        
        pMntRq->m_iStatus = MNT_LOG_DEAD; //log dead.
        return;
    }
    
    // check privilege
    if (pFront->m_iLogMode != LOG_AS_CONTROL) {
        pMntRq->m_iStatus = NO_PRIVILEGE;
        return;
    }
    
    int iHostID = g_pRouteTable->getHostIDByProcessID(pMntRq->m_iProcessID,sProcSysUserName);
    
    TWfLog::log(WF_FILE,"print ihostid,hostid:%d \n",iHostID);
    if (iHostID != g_iLocalHostID || strcmp(pLogName,sProcSysUserName)) {//Զ������ָ��
        int iIdx = CheckClientExist(iHostID,sProcSysUserName);
        if (iIdx<0 ) {
            TWfLog::log(WF_FILE,"ResponseChangeStatus: REMOTECMA_NOT_START, \n"
                "HostID:%d, UserName:%s, ProcessID:%d \n"
                "Զ��ָ��<%s>δ�ܷ���ִ��!",
                pMntRq->m_iProcessID, sProcSysUserName, pMntRq->m_iProcessID, pMntRq->m_sMsgBuf);
            pMntRq->m_iStatus = REMOTECMA_NOT_START;
            return;
        }
        
        cout<<"Rmt cmd: "<<pMntRq->m_sMsgBuf<<endl;
        if (!g_aClientLink[iIdx].pushMntCommand(pMntRq->m_sMsgBuf))
        {//push failed.
            sprintf(pMntRq->m_sMsgBuf,"ResponseChangeStatus: �쳣,ͬһʱ��ĳһԶ��������Ҫִ�ж���ָ��");
            TWfLog::log(WF_FILE,pMntRq->m_sMsgBuf);
            pMntRq->m_iStatus = REQ_CHANGESTATUS_ERR;
        }
        
        pMntRq->m_iStatus = REQ_STATUS_CHANGEING;
        
        return;
    }
    
    //�Ǳ���ָ��
    g_pRouteTable->changeState(pMntRq->m_iProcessID,pMntRq->m_cState);
    g_pShmTable->changeState(pMntRq->m_iProcessID,pMntRq->m_cState);
    
    pMntRq->m_iStatus = REQ_CHANGESTATUS_OVER;
    
    char sTemp[100];
    sprintf(sTemp,"<iProcessID=%d><cState=%c>",pMntRq->m_iProcessID,pMntRq->m_cState);
    PushMntMsg(sTemp,pMntRq->m_iProcessID);
    
    return;
}

////////////////////////////////////////////////////////////////

void ResponseRefreshRunplan()
{
    TMntReqInfo   *pMntRq = &(g_pShmTable->MntReqInfo);
    TMonitorFront *pFront = &(g_aMonitorFront[pMntRq->m_iIndex]);
        
    if (pMntRq->m_iStatus != REQ_REFRESH_RUNPLAN) //no refresh runplan command.
        return;
    
    // check iIndexID and iAuthID
    if (pMntRq->m_iStatus == -1 || pFront->m_iAuthID != pMntRq->m_iAuthID) {
        TWfLog::log(WF_FILE,"ResponseRefreshRunplan: Check AuthID err. rq_iIndex:%d,rq_iAuthID:%d,true_AuthID:%d",
            pMntRq->m_iIndex, pMntRq->m_iAuthID,pFront->m_iAuthID);
        
        pMntRq->m_iStatus = MNT_LOG_DEAD; //log dead.
        return;
    }
    
    // check privilege
    if (pFront->m_iLogMode != LOG_AS_CONTROL) {
        pMntRq->m_iStatus = NO_PRIVILEGE;
        return;
    }
    
    // �������ػ���������ڴ����б��
    g_pRouteTable->reload();
    RefreshProcShm (); //add by yks 2007.01.15
    
    delete g_pTimeRunPlanHead;
    g_pTimeRunPlanHead = new TTimeRunPlan;
    
    delete g_pOrdinalPlanHead;
    g_pOrdinalPlanHead = new TOrdinalPlan;
    
    // ��������Զ�̼�����������б��(����ָ�����Զ��������)
    bool bHaveRmtLink = false;
    for (int idx=0; idx<MAX_CLT_NUM; idx++) {
        if (g_aClientLink[idx].m_iHostID >= 0)    {
            g_aClientLink[idx].pushMntCommand("<iOperation=4>");
            bHaveRmtLink = true;
        }
    }
    if (bHaveRmtLink)
        pMntRq->m_iStatus = REFRESHING_RUNPLAN;
    else
        pMntRq->m_iStatus = RUNPLAN_REFRESHED;
}


////////////////////////////////////////////////////////////////

void ResponseRecvAppInfo()
{
    bool bDataExist = false;
    TMntReqInfo   *pMntRq = &(g_pShmTable->MntReqInfo);
    TMonitorFront *pFront = &(g_aMonitorFront[pMntRq->m_iIndex]);
    
    if (pMntRq->m_iStatus != MNT_REQ_APPINFO) // no recvAppStatus command.
        return;

    // check iIndexID and iAuthID    
    if (pMntRq->m_iStatus == -1 || pFront->m_iAuthID != pMntRq->m_iAuthID) {
        TWfLog::log(WF_FILE,"ResponseRecvAppInfo: Check AuthID err. rq_iIndex:%d,rq_iAuthID:%d,true_AuthID:%d",
            pMntRq->m_iIndex, pMntRq->m_iAuthID,pFront->m_iAuthID);
        
        pMntRq->m_iStatus = MNT_LOG_DEAD; //log dead.
        return;
    }

    char sTemp[MAX_APPINFO__LEN*2];
    pMntRq->m_sMsgBuf[0]=0;

    while (pFront->getMsg(sTemp)) {
        bDataExist = true;
        sTemp[MAX_APPINFO__LEN*2-1]=0;
        sprintf(pMntRq->m_sMsgBuf,"%s$%s",pMntRq->m_sMsgBuf,sTemp);
        if (strlen(pMntRq->m_sMsgBuf)>=(MAX_CMAMSG_LEN-(MAX_APPINFO__LEN*2))) 
            break;
    }
    
    if (!bDataExist) {
        pMntRq->m_sMsgBuf[0]=0; //����Ϣ
    }
    pMntRq->m_iStatus=REQ_APPINFO_FILLED;

}

////////////////////////////////////////////////////////////////

void ResponseSendRemoteMsg()
{
    TClientReqInfo *pCltRq = &(g_pShmTable->ClientReqInfo);
    TClientInfo *pCltInfo = &(g_aClientLink[pCltRq->m_iIndex]);
    
    if (pCltRq->m_iStatus != REQ_SEND_REMOTEMSG)
        return;
    
    if (pCltRq->m_iHostID != pCltInfo->m_iHostID
        || strcmp(pCltRq->m_sSysUserName, pCltInfo->m_sSysUserName)
    ){
        pCltRq->m_iStatus = CLIENT_LOG_DEAD;
        return;
    }
    
    TParseBuff ParaList;
    ParaList.parseBuff(pCltRq->m_sMsgBuf);
    
    TAppInfo* pAppInfo = g_pShmTable->getAppInfoByProcID(ParaList.getProcessID());
    if (!pAppInfo) {
        TWfLog::log(WF_FILE,"ResponseSendRemoteMsg: %s",pCltRq->m_sMsgBuf);
        pCltRq->m_iStatus = REMOTEMSG_SENT+123; //err.
        return;
    }
    else {
        pAppInfo->m_lProcessCnt = ParaList.getProcessCnt();
        pAppInfo->m_lNormalCnt = ParaList.getNormalCnt();
        pAppInfo->m_lErrorCnt = ParaList.getErrorCnt();
        pAppInfo->m_lBlackCnt = ParaList.getBlackCnt();
        pAppInfo->m_lOtherCnt = ParaList.getOtherCnt();
        g_pShmTable->changeState(pAppInfo->m_iProcessID, ParaList.getState());
        
        for (int iPos=0; iPos<MSG_MAXCNT; iPos++) {
            char sTemp[MAX_APPINFO__LEN];
            strcpy(sTemp,ParaList.getMsg(iPos));
            if (strlen(sTemp)!=0) {
                pAppInfo->pushInfo(sTemp);
            }
            else  break;
        }
    }
    pCltRq->m_iStatus = REMOTEMSG_SENT;
}
//////////////////////////////////////////////////////////////////////////

void ResponseRecvPassInfo()
{
    ABMException oabmExp;
    TMntReqInfo   *pMntRq = &(g_pShmTable->MntReqInfo);
    TMonitorFront *pFront = &(g_aMonitorFront[pMntRq->m_iIndex]);
    //TParseBuff ParaList;
    //ParaList.reset();
    if (pMntRq->m_iStatus != MNT_REQ_PASSINFO) //no recvpassinfo  command.
        return;
    // check iIndexID and iAuthID
    if (pMntRq->m_iStatus == -1 || pFront->m_iAuthID != pMntRq->m_iAuthID) {
        TWfLog::log(WF_FILE,"ResponseSendCommand: Check AuthID err. rq_iIndex:%d,rq_iAuthID:%d,true_AuthID:%d",
            pMntRq->m_iIndex, pMntRq->m_iAuthID,pFront->m_iAuthID);
        
        pMntRq->m_iStatus = MNT_LOG_DEAD; //log dead.
        return;
    }
    
    // check privilege
    if (pFront->m_iLogMode != LOG_AS_CONTROL) {
        pMntRq->m_iStatus = NO_PRIVILEGE;
        return;
    }
    TServerAppInfo *pAppInfo = NULL; 
    int  TCPU = 0;
    bool havemqid = false;
    int iMsgNum=0;
    int iQueueUsed=0;
   if (pMntRq->m_iFlag == 2) {
       pMntRq->m_sMsgBuf[0]=0;
       //char stemp[128]={0};
       char sCPU[100] ={0}; 
       char sMEM[100] = {0};
	     for(pAppInfo = g_pRouteTable->m_poAppInfo;pAppInfo;pAppInfo = pAppInfo->m_poNext) {
            if(pAppInfo->m_iProcessID== pMntRq->m_iProcessID) {
                havemqid = true;
                TAppInfo* pt = g_pShmTable->getAppInfoByProcID(pAppInfo->m_iProcessID);
                if (pt->m_cState =='R') {
                    GetSysProcInfo (pt->m_iAppPID, sMEM, sCPU);
                    sprintf(pMntRq->m_sMsgBuf, "<iProcessID=%d><m_cState=%c><sMEM=%s><sCPU=%s>", pAppInfo->m_iProcessID,pAppInfo->m_cState,sMEM,sCPU);
                    pMntRq->m_iStatus=REQ_PASSINFO_FILLED;
	                  return;
			          } else {
                    sprintf(pMntRq->m_sMsgBuf, "<iProcessID=%d><m_cState=%c><sMEM=0><sCPU=0>", pAppInfo->m_iProcessID,pAppInfo->m_cState);
                    pMntRq->m_iStatus=REQ_PASSINFO_FILLED;
	                  return;
			          }
		        }
		   }
   }
    MessageQueue MqStat(pMntRq->m_iMqID);
    if (MqStat.exist())
    { 
    } else {
        /*DEFINE_QUERY(qry);
        char stempsql [256] = {0};
        sprintf(stempsql,"select mq_maxnum,mq_size from m_wf_mq_attr where mq_id = %d",pMntRq->m_iMqID);
        qry.close();
        qry.setSQL(stempsql);
        qry.open();
        qry.next();
        MqStat.open(oabmExp,true,true);*/
    } 
    if (pMntRq->m_iFlag == 1) {
        char sCPU[100] ={0}; 
        char sMEM[100] = {0};
	      iMsgNum = MqStat.getMessageNumber();
        iQueueUsed = MqStat.getOccurPercent();
        for(pAppInfo = g_pRouteTable->m_poAppInfo;pAppInfo;pAppInfo = pAppInfo->m_poNext){
            if(pAppInfo->m_iMqId == pMntRq->m_iMqID) {
                havemqid = true;
       
                if(pAppInfo->m_cState !='R')
	                  continue; 	
            //ParaList.setProcessID(pAppInfo->m_iProcessID);
	     //ParaList.setState(pAppInfo->m_cState);
          //  ParaList.getBuiltStr(stemp1);
                TAppInfo* pt = g_pShmTable->getAppInfoByProcID(pAppInfo->m_iProcessID);
                GetSysProcInfo (pt->m_iAppPID, sMEM, sCPU);
                TCPU += atoi(sCPU);
	          }
        }
		    sprintf(pMntRq->m_sMsgBuf, "<iMqId=%d><iQueueUsed=%d><iMsgMum=%d><TCPU=%d>", pMntRq->m_iMqID,iQueueUsed,iMsgNum,TCPU);
        pMntRq->m_iStatus=REQ_PASSINFO_FILLED;
        return;
    }
     
   if (pMntRq->m_iFlag ==3){  
       pMntRq->m_sMsgBuf[0]=0;
       char stemp[20000]={0};
       for(pAppInfo = g_pRouteTable->m_poAppInfo;pAppInfo;pAppInfo = pAppInfo->m_poNext) {
           if(pAppInfo->m_iMqId == pMntRq->m_iMqID) {
               char stemp1[256] = {0} ;
               havemqid = true;
      
               TAppInfo* pt = g_pShmTable->getAppInfoByProcID(pAppInfo->m_iProcessID);
               char sCPU[100] ={0}; 
               char sMEM[100] = {0};
	             if(pt->m_cState == 'R') {
                   GetSysProcInfo (pt->m_iAppPID, sMEM, sCPU);
                   TCPU += atoi(sCPU);
                   sprintf(stemp1,"<iProcessID=%d><cState=%c><sMEN=%s><sCPU=%s>",pAppInfo->m_iProcessID,\
                       pAppInfo->m_cState,sMEM,sCPU);
	             } else {
                   sprintf(stemp1,"<iProcessID=%d><cState=%c><sMEN=0><sCPU=0>",pAppInfo->m_iProcessID,pAppInfo->m_cState);
		           }
               sprintf(stemp,"%s%s",stemp,stemp1);
           }
       // continue;
     }
    if(havemqid) {
        iMsgNum = MqStat.getMessageNumber();
        iQueueUsed = MqStat.getOccurPercent();

        sprintf(pMntRq->m_sMsgBuf, "%s<iMqId=%d><iQueueUsed=%d><iMsgMum=%d><TCPU=%d>", stemp,pMntRq->m_iMqID,iQueueUsed,iMsgNum,TCPU);
        pMntRq->m_iStatus=REQ_PASSINFO_FILLED;
    }
    else


     pMntRq->m_iStatus = NO_PASSINFO;

   }
}
////////////////////////////////////////////////////////////////
void PushMntMsg(char *sTemp, int iProcessID)
{
    TServerAppInfo *pServAppInfo = g_pRouteTable->getServerAppInfo(iProcessID);
    if (!pServAppInfo)
        return;
    
    for(int iMntIdx=0; iMntIdx<MAX_MNT_NUM; iMntIdx++) { 
    //ǰ̨����ѭ��(�������ͬʱ��Ҫͬһ�����̵���Ϣ)
        if (g_aMonitorFront[iMntIdx].m_iStatus==1 && 
            g_aMonitorFront[iMntIdx].belongOrg(pServAppInfo->m_iOrgID) )
        {
            g_aMonitorFront[iMntIdx].pushMsg(sTemp);
        }
    }
}

void OutputMsgIntoFrontBuff()
{
    static int aiCycCnt=0;
    int iMsgSumLen=0;
    char sTemp[MAX_APPINFO__LEN*4];
    TServerAppInfo *pAppInfo; 
    int iMod = OUT_MSG_DELAY/MAIN_CYCLE_DELAY;
    if (iMod>0) {
        aiCycCnt = (++aiCycCnt)%iMod;
        if (aiCycCnt)
            return;
    }
    
    for (int iAppIdx=0; iAppIdx<MAX_APP_NUM; iAppIdx++) { //Ӧ�ý���ѭ��
        
        iMsgSumLen = 0;
        TParseBuff ParaList;
        ParaList.reset();
        
        if (g_pShmTable->AppInfo[iAppIdx].m_iProcessID < 0) //�ڵ�δ�����ù�
            continue;
        //> ����Ƿ���Ҫ����ý�����Ϣ
        if (!g_pShmTable->AppInfo[iAppIdx].checkNeedOutput())
            continue;
        
        //��ʽ��Ӧ�ý�����Ϣ
        ParaList.setProcessID(g_pShmTable->AppInfo[iAppIdx].m_iProcessID);
        ParaList.setBillFlowID(g_pShmTable->AppInfo[iAppIdx].m_iBillFlowID);
        ParaList.setState(g_pShmTable->AppInfo[iAppIdx].getState());
        ParaList.setProcessCnt(g_pShmTable->AppInfo[iAppIdx].m_lProcessCnt);
        ParaList.setNormalCnt(g_pShmTable->AppInfo[iAppIdx].m_lNormalCnt);
        ParaList.setErrorCnt(g_pShmTable->AppInfo[iAppIdx].m_lErrorCnt);
        ParaList.setBlackCnt(g_pShmTable->AppInfo[iAppIdx].m_lBlackCnt);
        ParaList.setOtherCnt(g_pShmTable->AppInfo[iAppIdx].m_lOtherCnt);
        pAppInfo = g_pRouteTable->getServerAppInfo(g_pShmTable->AppInfo[iAppIdx].m_iProcessID);
        ParaList.setMqId(pAppInfo->m_iMqId); 
        for(int i=0; i<MAX_OUT_MSG_CNT; i++) { //ÿ�ε���Ӧ�ý��������� MAX_OUT_MSG_CNT ��
            char str[MAX_APPINFO__LEN];
            if ( g_pShmTable->AppInfo[iAppIdx].getInfo(str) ) {
                ParaList.addMsg(str);
                iMsgSumLen = iMsgSumLen + strlen(str);
                if (iMsgSumLen >= MAX_APPINFO__LEN-64)
                    break;
            }
            else break;
        }
        ParaList.getBuiltStr(sTemp);
        sTemp[MAX_APPINFO__LEN*2-1] = 0;
        
        PushMntMsg(sTemp,g_pShmTable->AppInfo[iAppIdx].m_iProcessID);
    }
}

////////////////////////////////////////////////////////////////
// ���ǰ̨���ӵ�ͬʱ��������޳�ʱ�䴦�� REQ_COMMAND_SENDING ״̬�����Ӳ�����֮
void CheckMonitorAlive()
{
    static int s_iLoopCnt=0;
    s_iLoopCnt++;
    s_iLoopCnt = s_iLoopCnt%100;
    if (s_iLoopCnt!=0)
        return;
    
    //��ѭ��ÿѭ��100��,���һ��ǰ̨����    
    for (int iPos=0; iPos<MAX_MNT_NUM; iPos++) {
        if (g_aMonitorFront[iPos].m_iStatus != 1)
            continue;
        if (!g_aMonitorFront[iPos].checkLinkAlive()) { //dead
            g_aMonitorFront[iPos].init();
            TWfLog::log(WF_FILE,"CheckMonitorAlive: Monitor Link timeout, dead.");
        }
    }
}

////////////////////////////////////////////////////////////////

void CheckClientAlive()
{
    static int s_iLoopCnt=123;
    s_iLoopCnt++;
    s_iLoopCnt = s_iLoopCnt%101;
    if (s_iLoopCnt!=0)
        return;
    
    //��ѭ��ÿѭ��101��,���һ��Զ������
    for (int iPos=0; iPos<MAX_CLT_NUM; iPos++) {
        if (g_aClientLink[iPos].m_iHostID < 0)
            continue;
        if (!g_aClientLink[iPos].checkLinkAlive()) { //dead
            g_aClientLink[iPos].init();
            TWfLog::log(WF_FILE,"CheckClientAlive: Client link timeout, dead.");
        }
    }    
}

////////////////////////////////////////////////////////////////

void ScanTimeRunPlan()
{
    static int s_iLoopCnt=0;
    s_iLoopCnt++;
    s_iLoopCnt = s_iLoopCnt%(500000/MAIN_CYCLE_DELAY); //ÿ0.5������ɨ��һ��
    if (s_iLoopCnt!=0) return;

    if (g_pTimeRunPlanHead->checkArrive() <= 0 )
        return;
    
    while(1) {
        
        int iProcID,iPlanID,iStaffID, iCltIdx;
        char sCmd[256];
        
        iProcID = g_pTimeRunPlanHead->getShouldRunProcess(iPlanID,iStaffID);
        
        if (iProcID<0) return;

        sprintf(sCmd,"<iProcessID=%d><iOperation=1><iStaffID=%d><iPlanID=%d>",iProcID,iStaffID,iPlanID);
        
        //�жϽ�������������
        char sSysUserName[32];
        int iHostID = g_pRouteTable->getHostIDByProcessID(iProcID,sSysUserName);
        
        if (iHostID==g_iLocalHostID && !strcmp(pLogName,sSysUserName)) {
            
            //ȷ�ϵ�ǰ����δ����"����"||"����"״̬
            TAppInfo * pAppInfo = g_pShmTable->getAppInfoByProcID(iProcID);
            if (pAppInfo) {
                if (pAppInfo->m_cState=='B' || pAppInfo->m_cState=='R') {
                    TWfLog::log(WF_FILE,"ScanTimeRunPlan:�Զ������жϵ�Ӧ��������ID:%d, �����ָý��̴���״̬:%c",
                        iProcID,pAppInfo->m_cState);
                    continue;
                }
            }
            
            //��������
            int iProcLogID = TLogProcessMgr::insertLogProcess(iProcID,
                    (const char *)sCmd,iStaffID,iPlanID);
            
            int iRet = g_pRouteTable->startApp(iProcID,iProcLogID);
            
            if (iRet==0) { //����ʧ��
                TWfLog::log(WF_FILE,"ScanTimeRunPlan: ��������ʧ��, startApp(%d,%d)",iProcID,iProcLogID);
            }
            continue;
        }
        
        //Զ��ָ��
        for (iCltIdx=0; iCltIdx<MAX_CLT_NUM; iCltIdx++) {
            if (g_aClientLink[iCltIdx].m_iHostID == iHostID)
                break;
        }
        if (iCltIdx==MAX_CLT_NUM) {
            TWfLog::log(WF_FILE,"�ƻ�Զ��ָ��δ�ҵ���Ӧ����. Command:%s",sCmd);
            return;
        }
        
        g_aClientLink[iCltIdx].pushPlanCommand(sCmd); //ѹ����ӦԶ��ָ�����
    }
}

////////////////////////////////////////////////////////////////

void ScanOrdinalPlan()
{
    static int s_iLoopCnt=0;
    s_iLoopCnt++;
    s_iLoopCnt = s_iLoopCnt%120;
    if (s_iLoopCnt!=0) return;

    TNodeConnector *pNode;
    
    if (!g_pOrdinalPlanHead) {
        static int s_iFlag=0;
        if (s_iFlag==0) {
            TWfLog::log(WF_FILE,"g_pOrdinalPlanHead not init");
            s_iFlag++ ; s_iFlag=s_iFlag%10000;
        }
        return;
    }
    
    g_pOrdinalPlanHead->refreshNodeState(g_pRouteTable);
    
    // ����ѭ��
    for (pNode=g_pOrdinalPlanHead->m_poPlan; pNode; pNode=pNode->m_poNextProcess) {
        
        char sCmd[1024],sLogCmd[256];
        int  iCltIdx;
        
        TServerAppInfo *pServApp = pNode->checkShouldRun(g_pRouteTable);
        if ( !pServApp) 
            continue;
        
        sprintf(sCmd,"<iProcessID=%d><iOperation=1><iStaffID=-2><iPlanID=-2>",pServApp->m_iProcessID);
        
        if (pServApp->m_iAppType<3)
            strcpy(sLogCmd, pServApp->m_sExecCommand);
        else
            sprintf(sLogCmd,"SQL statement");
        
        //�ж��Ƿ��Ǳ�������
        char sProcSysUserName[20];
        g_pRouteTable->getHostIDByProcessID (pServApp->m_iProcessID,sProcSysUserName);
        
        if (pServApp->m_iHostID==g_iLocalHostID && !strcmp(pLogName,sProcSysUserName)) {
            
            int iProcLogID = TLogProcessMgr::insertLogProcess(pServApp->m_iProcessID,
                    (const char *)sLogCmd,-2,-2); // -2��ʾ˳���Զ�ִ��
            int iRet = g_pRouteTable->startApp(pServApp->m_iProcessID,iProcLogID);
            
            if (iRet==0) { //����ʧ��
                TWfLog::log(WF_FILE,"ScanOrdinalPlan: ��������ʧ��, startApp(%d,%d)",pServApp->m_iProcessID,iProcLogID);
            }
            continue;
        }
        
        //Զ��ָ��
        for (iCltIdx=0; iCltIdx<MAX_CLT_NUM; iCltIdx++) {
            if (g_aClientLink[iCltIdx].m_iHostID == pServApp->m_iHostID)
                break;
        }
        if (iCltIdx==MAX_CLT_NUM) {
            TWfLog::log(WF_FILE,"�ƻ�Զ��ָ��δ�ҵ���Ӧ����. Command:%s",sCmd);
            return;
        }
        
        g_aClientLink[iCltIdx].pushPlanCommand(sCmd); //ѹ����ӦԶ��ָ�����
    }
}

int GetSysProcInfo (int iPID, char *sMem, char *sCpu)

{
    char sCommand[256];
    FILE *fp;
    int ret;

    strcpy (sMem, "0");
    strcpy (sCpu, "0");
    
    
#ifdef DEF_HP
#define _guard_def_unix95__
#endif
#ifdef DEF_SOLARIS
#define _guard_def_unix95__
#endif

#ifdef DEF_UNIX95
#define _guard_def_unix95__
#endif

#ifdef _guard_def_unix95__
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
////////////////////////////////////////////////////////////////

void TMonitorFront::init() {
    m_iStatus = -1; 
    m_iAuthID = 0;
    m_iBillFlowID = 0;
    m_iLogMode = 0;
    m_iStaffID = 0;
    m_iMsgHead = 0; //ָ����һ����ѹ��ĵ�ַ
    m_iMsgTail = 0; //ָ����һ��������ĵ�ַ
    m_iMsgNeedHead = 0; //ָ����һ����ѹ��ĵ�ַ
    m_iMsgNeedTail = 0; //ָ����һ��������ĵ�ַ
    m_sHostAddr[0] = 0;
    
    m_bInit = 1;
}

// ѹ���̨Ӧ�ý�����Ϣ�� m_sMsg[m_iMsgHead]/m_sMsgNeed[m_iMsgNeedHead]; ���򸲸���ɵ���Ϣ;
bool TMonitorFront::pushMsg(const char _sMsg[], bool bNeed)
{
    int iLen = strlen(_sMsg);
    
    if (iLen >= MAX_APPINFO__LEN*2)
        iLen = MAX_APPINFO__LEN*2-1;
        
    strncpy( m_sMsg[m_iMsgHead], _sMsg, iLen);
    m_sMsg[m_iMsgHead][iLen] = 0;
        
    m_iMsgHead++;
    m_iMsgHead = m_iMsgHead%MAX_CMAMSG_CNT;
    
    //������( MAX_CMAMSG_CNT-1 ��)
    if (m_iMsgHead == m_iMsgTail) {
        m_iMsgTail++;
        m_iMsgTail = m_iMsgTail%MAX_CMAMSG_CNT;
    }

    return true;
}


// ��ȡһ����̨Ӧ�ý�����Ϣ from m_sMsg[m_iMsgTail];
bool TMonitorFront::getMsg(char _sMsg[])
{
    if (m_iMsgHead == m_iMsgTail)
        return false;
    
    strcpy(_sMsg, m_sMsg[m_iMsgTail]);
    
    m_iMsgTail++;
    m_iMsgTail = m_iMsgTail%MAX_CMAMSG_CNT;
    
    return true;
}

//## ��OrgID�������� m_aiOrgID ������. ��Ч��ֵ��-99
void  TMonitorFront::parseOrgIDStr(char *str)
{
    char sVale[10];
    memset(sVale,0,10);
    for (int iPos=0; iPos<MAX_ORG_CNT; iPos++) {
        m_aiOrgID[iPos] = -99;
    }
    
    int iArrPos=0;
    int iStrLen = strlen(str);
    for (int iStrPos=0; iStrPos <= iStrLen; iStrPos++) {
        static int iValPos=0;
        if (str[iStrPos]=='|' || str[iStrPos]==0) { //�����ָ���
            if (iValPos>0) { 
                m_aiOrgID[iArrPos] = atoi(sVale);
                memset(sVale,0,10);
                iValPos = 0;
                iArrPos++;
                if (iArrPos >= MAX_ORG_CNT)
                    return;
            }
            else continue;
        }
        else {
            sVale[iValPos]=str[iStrPos];
            iValPos++;
        }
    }
}

bool  TMonitorFront::belongOrg(int iOrgID)
{
    //mod by yangks 2005.08.18
    if (iOrgID == 0)  //added 2005.08.18
        return true;  //added 2005.08.18
    
    for (int iPos=0; iPos<MAX_ORG_CNT; iPos++) {
        if (m_aiOrgID[iPos]==-99)
            return false;
        /*
        if (g_pOrgMgr->getBelong(iOrgID, m_aiOrgID[iPos]) || iOrgID==0 ||
                g_pOrgMgr->getBelong(m_aiOrgID[iPos],iOrgID))
            return true;
        */
        if (iOrgID == m_aiOrgID[iPos])
            return true;
    }
    
    return false;
}


////////////////////////////////////////////////////////////////


//���ؼƻ�ʱ,��Ҫ���ƻ��ĸ�ʽ�Ƿ�Ϸ�, ���Ϸ�ʱ�ñ� m_wf_time_plan.check_err = 1
void TTimeRunPlan::load()
{

}


//��� _pRunPlan ָ���ʱ��ƻ���ʽ�Ƿ���ȷ
// ��ȷ->���� true
// ����->���� false
bool TTimeRunPlan::checkFormatCorrect(TRunPlan * _p)
{    
    if (_p->m_sYear[0] != '*'   && strlen(_p->m_sYear) != 4) return false;
    if (_p->m_sMonth[0] != '*'  && strlen(_p->m_sMonth) != 2) return false;
    if (_p->m_sDay[0] != '*'    && strlen(_p->m_sDay) != 2) return false;
    if (_p->m_sHour[0] != '*'   && strlen(_p->m_sHour) != 2) return false;
    if (_p->m_sMinute[0] != '*' && strlen(_p->m_sMinute) != 2) return false;
    if (_p->m_sWeekDay[0] != '*'&& strlen(_p->m_sWeekDay) != 1) return false;

//    if (_p->m_sYear[0] != '*'   && atoi(_p->m_sYear) > 9000) return false;
    if (_p->m_sMonth[0] != '*'  && atoi(_p->m_sMonth) > 12) return false;
    if (_p->m_sDay[0] != '*'    && atoi(_p->m_sDay) > 31) return false;
    if (_p->m_sHour[0] != '*'   && atoi(_p->m_sHour) > 24) return false;
    if (_p->m_sMinute[0] != '*' && atoi(_p->m_sMinute) > 60) return false;
    if (_p->m_sWeekDay[0] != '*'&& atoi(_p->m_sWeekDay) > 7) return false;


    return true;
}

//�ñ� m_wf_time_plan.check_err = 1 where plan_id = _iPlanID
void TTimeRunPlan::UpdateErrPlan(int _iPlanID)
{
    return;
}


//��鵱ǰ�Ƿ����µ���(����ƻ�ִ��ʱ���)�Ľ���,���û��,����0
//�����,���� ��ִ�еĽ��̸���, ������Ӧ��TRunPlan.m_bShouldRun = true.
int TTimeRunPlan::checkArrive()
{    
    int iShouldRunCnt = 0;
    
    m_oCurDate.getCurDate();
    
    for (TRunPlan *pt = m_poRunPlan;  pt;  pt = pt->m_poNext) {
        
        if (pt->m_poLastCheckDate == NULL) {
            pt->m_poLastCheckDate = new Date;
            pt->m_poLastCheckDate->addMin(-100); //��һ�����ɽڵ�Dateʵ��
        }
        
        if (pt->m_bShouldRun) continue;
        
        // �����ǰ�ƻ� ���һ���ж���ShouldRunʱ��ʱ��� ��һ����֮��,���жϵ�ǰ�ƻ�,�����κβ���
        pt->m_poLastCheckDate->addMin(1);
        if ( *(pt->m_poLastCheckDate) > m_oCurDate ){
            pt->m_poLastCheckDate->addMin(-1);
            continue;
        }
        pt->m_poLastCheckDate->addMin(-1);
        
        if (pt->m_sYear[0]!='*'   && atoi(pt->m_sYear)!=m_oCurDate.getYear() )  continue;
        if (pt->m_sMonth[0]!='*'  && atoi(pt->m_sMonth)!=m_oCurDate.getMonth())  continue;
        if (pt->m_sDay[0]!='*'    && atoi(pt->m_sDay) !=m_oCurDate.getDay() )  continue;
        if (pt->m_sHour[0]!='*'   && atoi(pt->m_sHour)!=m_oCurDate.getHour() )  continue;
        if (pt->m_sMinute[0]!='*' && atoi(pt->m_sMinute)!=m_oCurDate.getMin() ) continue;
        if (pt->m_sWeekDay[0]!='*'&& atoi(pt->m_sWeekDay)!=m_oCurDate.getWeek() ) continue;
        
        pt->m_poLastCheckDate->getCurDate();
        
        pt->m_bShouldRun = true;
        iShouldRunCnt ++;
    }
    
    return iShouldRunCnt;
}


//�ӵ�ǰ�ƻ��в��ҵ�һ����Ҫ���еļƻ�(RunPlan.m_bShouldRun == true),
//�ҵ� ���� ProcessID,_rRunPlanID,_iStaffID,
//     ����TRunPlan.m_bShouldRun = false  
//���򷵻� -1
int  TTimeRunPlan::getShouldRunProcess(int &_rRunPlanID, int &_iStaffID)
{
    for (TRunPlan *pt = m_poRunPlan; pt; pt = pt->m_poNext) {
        if (pt->m_bShouldRun) {
            pt->m_bShouldRun = false;
            _rRunPlanID = pt->m_iRunPlanID;
            _iStaffID = pt->m_iStaffID;
            return pt->m_iProcessID;
        }
    }
    _rRunPlanID = -1;
    _iStaffID = -1;
    return -1;
}


void TTimeRunPlan::unload() 
{
    delete m_poRunPlan;
    m_poRunPlan = NULL;
    return;
}

////////////////////////////////////////////////////////////////

void TOrdinalPlan::load()
{
}

void TOrdinalPlan::unload()
{
    if (m_poPlan) {
        delete m_poPlan; 
        m_poPlan = NULL;
    } 
}


//��ȡ _iProcessID ��Ӧ��ǰ�ý��� ����ͷָ��,�Ҳ��� return NULL
TNodeConnector* TOrdinalPlan::getAutoProcInfo(int _iProcessID)
{
    TNodeConnector *pt;
    
    for (pt = m_poPlan; pt; pt = pt->m_poNextProcess) {
        if (pt->m_iProcessID == _iProcessID)
            return pt;
    }
    
    return NULL;
}

void TOrdinalPlan::refreshNodeState(TRouteTable *pRtTab) {
    if (!m_poPlan) 
        return ;
    m_poPlan->setPrefixState(pRtTab);
}

void TNodeConnector::setPrefixState(TRouteTable *pRtTab) {
    
    TServerAppInfo* pServApp;
    
    pServApp = pRtTab->getServerAppInfo(m_iPrefixID);
    if (!pServApp) {
        TWfLog::log(WF_FILE,"TNodeConnector::setPrefixState, RouteTable���Ҳ���processID=%d",m_iPrefixID);
    }
    else {
        m_cPrefixState = pServApp->m_cState;
    }
    
    if (m_poNextPrefix)
        m_poNextPrefix->setPrefixState(pRtTab);
    if (m_poNextProcess)
        m_poNextProcess->setPrefixState(pRtTab);
}


//��鵱ǰ m_iProcessID �Ƿ�Ӧ�ñ�����, ��,��������RouteTable�е�ָ��.
TServerAppInfo* TNodeConnector::checkShouldRun(TRouteTable *pRtTab) {
    
    TServerAppInfo* pServApp = pRtTab->getServerAppInfo(m_iProcessID);
    
    if (!pServApp) {
        TWfLog::log(WF_FILE,"TNodeConnector::checkShouldRun, RouteTable���Ҳ���processID=%d",m_iProcessID);
    }
    
    if (pServApp->m_cState=='A' && pServApp->m_iAppType<4 && checkPrefixEnd()) //��ǰm_iProcessID״̬����Ϊ'A',��ǰ�ý���End
        return pServApp;
    
    return NULL;
}



////////////////////////////////////////////////////////////////
