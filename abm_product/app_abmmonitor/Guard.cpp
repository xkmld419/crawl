/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved
#include <string.h>

#include "Environment.h"

#include "GetinfoMonitor.h" 
#include "Guard.h"
#include "mntapi.h" 

#include "Server.h"
#include "CommonMacro.h"
#include "LocalDefine.h"
#include "WfPublic.h"


extern char **environ;

extern int         g_iShmID;
extern TSHMCMA *g_pShmTable;
extern TRouteTable *g_pRouteTable;
extern int   g_iLocalHostID;

extern long g_lWorkFlowKey ;
extern long g_lLockWorkFlowKey ;
extern long g_lLockApplicationKey ;
extern long g_lLockClientKey ;

int GetAppIndex(int _iProcessID);
int GetSavBufIndex(int _iProcessID);

union Semnum {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

int InitLockGrp( const int iKey, const short iLockNum, const ushort iInitValue ) {

    int iLockGrpID;
    short i;
    Semnum arg;

    // generate the lock using semaphore
    if ( (iLockGrpID = semget(iKey, iLockNum, PERM|IPC_EXCL|IPC_CREAT)) == -1 ) {
        if ( (iLockGrpID = semget(iKey, iLockNum, 0)) == -1 ) {
            perror( "get lock-id error" );
            return -1;
        }
    }

    // set the init-value of the semaphore
    arg.array = (ushort *)malloc( sizeof(ushort)*iLockNum );
    for (i = 0 ; i < iLockNum ; i++ )
        arg.array[i] = iInitValue;
    if (semctl(iLockGrpID, 0, SETALL, arg) == -1 ) {
        semctl( iLockGrpID, 0, IPC_RMID, 0 );
        return -1;
    }
    free(arg.array);

    return iLockGrpID;
}


void sig_chld(int signo) {
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0)
        ;
}

void catch_sigchld (int singal)
{
    int status;

    wait(&status);
}

void TServerAppInfo::updateState() {

        MonitorInfo oInfoMonitor;
        oInfoMonitor.getInfoCfg();
        oInfoMonitor.m_oSHMCSemaphore.P();
        int iRet=0;
        while(iRet<(*oInfoMonitor.m_piNum)){
            if(oInfoMonitor.m_poMonitorCfg[iRet].m_iProcessID==m_iProcessID){
                oInfoMonitor.m_poMonitorCfg[iRet].m_sState=m_cState;
            }
            iRet++;
        }
        oInfoMonitor.m_oSHMCSemaphore.V();
}

void GetWorkFlowKey ()
{
    ABMException oExp;
    MonitorInfo oIPCMonitor;
    if (!g_lWorkFlowKey)
        g_lWorkFlowKey = oIPCMonitor.getInfoIpc(oExp,"SHM_WorkFlow");
    if (!g_lLockWorkFlowKey)
        g_lLockWorkFlowKey = oIPCMonitor.getInfoIpc(oExp,"SEM_LockWorkflow");
    if (!g_lLockApplicationKey)
        g_lLockApplicationKey = oIPCMonitor.getInfoIpc(oExp,"SEM_LockApplication");
    if (!g_lLockClientKey)
        g_lLockClientKey = oIPCMonitor.getInfoIpc(oExp,"SEM_LockCLient");
	//	Environment::getDBConn()->commit();
}

// ͨѶ��ʼ��,��ʼ���ź���
int CommunicationInit(bool bInit) 
{    
    char sAction[100];
    
    bool bCreateShmFlag = false;
    
    sprintf (sAction,"--����--");
    
    signal(SIGCHLD,SIG_IGN);
    
    GetWorkFlowKey ();
        
    if (g_lWorkFlowKey<0 || g_lLockWorkFlowKey<0 || g_lLockApplicationKey<0 ||g_lLockClientKey<0)
    {
        char sTmp[1000];
        sprintf (sTmp,"��ȡMC_���̵��ȹ����ڴ��ַ����:\n SHM_WorkFlow:%ld \n"
            " SEM_LockWorkflow:%ld \n SEM_LockApplication:%ld \n SEM_LockCLient:%ld \n",
            g_lWorkFlowKey, g_lLockWorkFlowKey, g_lLockApplicationKey, g_lLockClientKey
        );
        TWfLog::log (0,sTmp);
        cout << sTmp <<endl;
        
        return -1;
    }
    
    TWfLog::log(0, "��ʼ��MC_���̵��ȹ����ڴ�...");
    
    if (GetShmAddress(&g_pShmTable,g_lWorkFlowKey)<0 ) {
        
        g_iShmID = shmget(g_lWorkFlowKey,sizeof(TSHMCMA),PERM|IPC_CREAT|IPC_EXCL);
        
        if (g_iShmID <0){
            if ((g_iShmID = shmget(g_lWorkFlowKey,0,0))<0){
                TWfLog::log (0,"MC_���̵��ȹ����ڴ�(%ld)����ʧ��",g_lWorkFlowKey);
                return -1;
            }
        }
        
        g_pShmTable = (TSHMCMA *)shmat(g_iShmID,(void *)0,0);
        if ((int *)g_pShmTable == (int *)(-1)){
            TWfLog::log (0,"MC_���̵��ȹ����ڴ�(%ld)����ʧ��",g_lWorkFlowKey);
            return -1;
        }
        memset(g_pShmTable,0,sizeof(TSHMCMA));

        sprintf (sAction,"<<����>>");
        
        bCreateShmFlag = true;
        
        // �ź�����ʼ��
        TWfLog::log(0, "��ʼ���ź���...");
        
        g_pShmTable->m_iMonitorLock = InitLockGrp(g_lLockWorkFlowKey,1,1);
        if (g_pShmTable->m_iMonitorLock < 0) {
            TWfLog::log(WF_FILE," ��ʼ���ź���ʧ�� %ld ",g_lLockWorkFlowKey);
            return -1;
        }
        g_pShmTable->m_iAppLock = InitLockGrp(g_lLockApplicationKey,1,1);
        if (g_pShmTable->m_iAppLock < 0) {
            TWfLog::log(WF_FILE," ��ʼ���ź���ʧ�� %ld ",g_lLockApplicationKey);
            return -1;        
        }
        g_pShmTable->m_iClientLock = InitLockGrp(g_lLockClientKey,1,1);
        if (g_pShmTable->m_iClientLock < 0) {
            TWfLog::log(WF_FILE," ��ʼ���ź���ʧ�� %ld ",g_lLockClientKey);
            return -1;        
        }
    }
    
    TWfLog::log (0,"MC_���̵��ȹ����ڴ�(%ld)%s�ɹ�!",g_lWorkFlowKey, sAction);
    cout<<"MC_���̵��ȹ����ڴ�("<<g_lWorkFlowKey<<")"<<sAction<<"�ɹ�!"<<endl;    
    
    if (!bInit && !bCreateShmFlag)
        return 1;
    
    // Init default value
    for (int iPos = 0; iPos < MAX_APP_NUM; iPos++) {
        g_pShmTable->AppSaveBuff[iPos].init();
        g_pShmTable->AppInfo[iPos].init();
    }
    
    return 1;
}


//////////////////////////// Class implementation  /////////////////////////

void TMsgQueueMgr::load()
{
    TProcessQueue  **ppNextProc;
    char sql[2000];
    
    m_poQueueHead = NULL;
    
    ppNextProc = &(m_poQueueHead);
    
    MonitorInfo oInfoMonitor;
    oInfoMonitor.getInfoCfg();
    oInfoMonitor.m_oSHMCSemaphore.P();
    for(int i=0;i<*oInfoMonitor.m_piNum;i++){
        *ppNextProc = new TProcessQueue;
        (*ppNextProc)->m_iProcessID = oInfoMonitor.m_poMonitorCfg[i].m_iProcessID;
		    ( *ppNextProc)->m_iBillFlowID= 7;
	      (*ppNextProc)->m_iMqID = oInfoMonitor.m_poMonitorCfg[i].m_iRecvMsg;
        (*ppNextProc)->m_iQueueSize = 1;	
        (*ppNextProc)->m_iOutMqID = oInfoMonitor.m_poMonitorCfg[i].m_iSendMsg;  
        ppNextProc  = &((*ppNextProc)->m_poNextProcess);
    }
    oInfoMonitor.m_oSHMCSemaphore.V();
    return;
}
void TMsgQueueMgr::unload()
{
    if (m_poQueueHead) 
        delete m_poQueueHead;
        
    return;
}

TProcessQueue* TMsgQueueMgr::searchProcQueue(int _iProcID)
{
    for (TProcessQueue *pt=m_poQueueHead; pt; pt=pt->m_poNextProcess)
        if (pt->m_iProcessID == _iProcID)
            return pt;

    return NULL;
}
//����������ӵ�еĶ���( ����δ���ö���,����false)
bool TMsgQueueMgr::createSelfQueue(int _iProcID)
{
    bool bRet = false;
    ABMException oabmExp;
    TProcessQueue *pt=m_poQueueHead;
    if(!pt)  return false;
    while(pt){
        if (pt->m_iProcessID == _iProcID && pt->m_iMqID>0)  {
               MessageQueue oQueue(pt->m_iMqID);
               oQueue.open(oabmExp,false,true);
               oQueue.close();
        } else {
        
        }
        pt=pt->m_poNextProcess;
    } 
    return true;
}

//## ɾ�����̶���
bool TMsgQueueMgr::deleteSelfQueue(int _iProcID)
{
    TProcessQueue* pProcQue = searchProcQueue(_iProcID);
    ABMException oabmExp;
    while(pProcQue){
    if (pProcQue && pProcQue->m_iMqID>0) {
        MessageQueue oQueue(pProcQue->m_iMqID);
        oQueue.open(oabmExp,true,true);
        oQueue.remove(oabmExp);
    }
    pProcQue=pProcQue->m_poNextProcess;
    }
    return true;
}

//## �������̹��������ж���
bool TMsgQueueMgr::createAssociateQueue(int _iProcID)
{
    createSelfQueue(_iProcID);
    
    TProcessQueue* pProcQue = searchProcQueue(_iProcID);
    
    for (; pProcQue; pProcQue=pProcQue->m_poOutProcQueue)
        createSelfQueue(pProcQue->m_iProcessID);
    
    return true;
}

//## ɾ�����̹��������ж���
bool TMsgQueueMgr::deleteAssociateQueue(int _iProcID)
{
    deleteSelfQueue(_iProcID);
    
    TProcessQueue* pProcQue = searchProcQueue(_iProcID);
    
    for (;pProcQue; pProcQue=pProcQue->m_poOutProcQueue)
        deleteSelfQueue(_iProcID);
    
    return true;
}

//## ��������������Ϣ����
bool TMsgQueueMgr::createBillFlowQueue(int _iBillFlowID)
{
    TProcessQueue* pProcQue = m_poQueueHead;
    
    for (; pProcQue; pProcQue=pProcQue->m_poNextProcess) {
        if (pProcQue->m_iBillFlowID == _iBillFlowID)
            createSelfQueue(pProcQue->m_iProcessID);
    }
    
    return true;
}

//## ɾ������������Ϣ����
bool TMsgQueueMgr::deleteBillFlowQueue(int _iBillFlowID)
{
    TProcessQueue* pProcQue = m_poQueueHead;
    
    for (; pProcQue; pProcQue=pProcQue->m_poNextProcess) {
        if (pProcQue->m_iBillFlowID == _iBillFlowID)
            deleteSelfQueue(pProcQue->m_iProcessID);
    }
        
    return true;
}

//##��ȡ������Ϣ��Ŀ
int TMsgQueueMgr::getMsgNumber(int _iProcessID)
{
    int iRet = -1;
    
    TProcessQueue* pProcQue = searchProcQueue(_iProcessID);
    ABMException oabmExp;
    if (pProcQue && pProcQue->m_iMqID>0) {
        MessageQueue oQueue(pProcQue->m_iMqID);
        if (!oQueue.exist()) return -1;
        //oQueue.open();
        oQueue.open(oabmExp,true,true);
        iRet = oQueue.getMessageNumber();
    }
    
    return iRet;
}

//##��ȡ��Ϣ����ʹ�ðٷֱ�
int TMsgQueueMgr::getPercent(int _iProcessID)
{
    int iRet = -1;
    
    TProcessQueue* pProcQue = searchProcQueue(_iProcessID);
    ABMException oabmExp;
    if (pProcQue && pProcQue->m_iMqID>0) {
        MessageQueue oQueue(pProcQue->m_iMqID);
        if (!oQueue.exist()) return -1;
        //oQueue.open();
        oQueue.open(oabmExp,true,true);
        iRet = oQueue.getOccurPercent();
    }
    
    return iRet;
}

void TMsgQueueMgr::printQueue(char sBuff[]) {
    TProcessQueue *pt;
    sBuff[0]=0;
    ABMException oabmExp;
    for (pt = m_poQueueHead; pt; pt = pt->m_poNextProcess) {
        if(pt->m_iMqID <= 0) continue;
        MessageQueue oQueue(pt->m_iMqID);
        if (!oQueue.exist()) continue;
        //oQueue.open();
        oQueue.open(oabmExp,true,true);
        sprintf(sBuff,"%s ProcessID: %-9d  MsqID: %-9d  MsgNumber: %-9d  UsedPercent: %-9d \n",
            sBuff,pt->m_iProcessID, oQueue.getSysID(), oQueue.getMessageNumber(),
            oQueue.getOccurPercent() );
        
    }
}


//###################################################################

void TRouteTable::load()
{
    TServerHostInfo **ppHostInfo;
    TServerAppInfo  **ppAppInfo;
    
    //DEFINE_QUERY(qry);

    m_poMsgQueue = new TMsgQueueMgr;
    
    m_poHostInfo = NULL;
    m_poAppInfo = NULL;
    ppHostInfo = &m_poHostInfo;
    ppAppInfo  = &m_poAppInfo;
    ABMException oExp;
    char sExecPath[256];
    //char *penv = getenv("ABMAPP_DEPLOY");
    char *penv = getenv("ABM_PRO_DIR");
    if (!penv) {
        //ADD_EXCEPT0(oExp, "miss environment ABMAPP_DEPLOY, please add it to your.profile");
        ADD_EXCEPT0(oExp, "miss environment ABM_PRO_DIR, please add it to your.profile");
        return ;
    }
    if (penv[strlen(penv)-1] != '/') {
        snprintf (sExecPath, sizeof(sExecPath)-1, "%s/bin",penv);
    } else {
        snprintf (sExecPath, sizeof(sExecPath)-1, "%sbin",penv);
    }

    char *pUserName = getenv("LOGNAME");
    MonitorInfo oInfoMonitor;
    oInfoMonitor.getInfoCfg(); 
    oInfoMonitor.m_oSHMCSemaphore.P(); 
    for(int i=0;i<(*oInfoMonitor.m_piNum);i++) {
        
        *ppAppInfo = new TServerAppInfo;
        
        (*ppAppInfo)->m_iAppID      = oInfoMonitor.m_poMonitorCfg[i].m_iAppID;
        (*ppAppInfo)->m_iProcessID  = oInfoMonitor.m_poMonitorCfg[i].m_iProcessID;
        (*ppAppInfo)->m_iBillFlowID = 7;
        (*ppAppInfo)->m_iAutoRun    = 0;
        (*ppAppInfo)->m_iAppType    = 0;
        (*ppAppInfo)->m_iHostID     = 1;
        (*ppAppInfo)->m_iOrgID      = 1;
        
       (*ppAppInfo)->m_sSqlStatement[0]='\0';
        
        char sTmp[2];
        char sTmpExecCommand[80];
        sTmp[0]=oInfoMonitor.m_poMonitorCfg[i].m_sState;
        //strncpy( sTmp, oInfoMonitor.m_poMonitorCfg[i].m_sState,1 );
        (*ppAppInfo)->m_cState      = sTmp[0];
        strncpy( (*ppAppInfo)->m_sExecPath , sExecPath,sizeof((*ppAppInfo)->m_sExecPath)-1);
       snprintf(sTmpExecCommand,sizeof(sTmpExecCommand),"%s -p %d %s ",oInfoMonitor.m_poMonitorCfg[i].m_sAppExec,\
              oInfoMonitor.m_poMonitorCfg[i].m_iProcessID,oInfoMonitor.m_poMonitorCfg[i].m_sParammeter);

        strncpy( (*ppAppInfo)->m_sExecCommand ,sTmpExecCommand,sizeof((*ppAppInfo)->m_sExecCommand)-1);
        strncpy ((*ppAppInfo)->m_sSysUserName, pUserName,sizeof((*ppAppInfo)->m_sSysUserName)-1);
        
        if(oInfoMonitor.m_poMonitorCfg[i].m_iAutoRun == 0){
            (*ppAppInfo)->m_bAutoBootWhenAbort = false;
        } else {
            (*ppAppInfo)->m_bAutoBootWhenAbort = true;
        }
        //(*ppAppInfo)->m_bAutoBootWhenAbort = 1;
        
        (*ppAppInfo)->m_iMqId = oInfoMonitor.m_poMonitorCfg[i].m_iRecvMsg;
         ppAppInfo = &( (*ppAppInfo)->m_poNext );
    }
    oInfoMonitor.m_oSHMCSemaphore.V();
    
}

void TRouteTable::unload()
{
    if (m_poHostInfo) {
        delete m_poHostInfo;
        m_poHostInfo = NULL;
    }
    if (m_poAppInfo) {
        delete m_poAppInfo;
        m_poAppInfo = NULL;
    }
    if (m_poMsgQueue) {
        delete m_poMsgQueue;
        m_poMsgQueue = NULL;
    }
}


//## ����iProcID��ȡ���̶�ӦTServerAppInfo��ַ, �Ҳ��� return NULL
TServerAppInfo* TRouteTable::getServerAppInfo(int _iProcID)
{
    TServerAppInfo *pAppInfo;
    
    for (pAppInfo = m_poAppInfo; pAppInfo; pAppInfo = pAppInfo->m_poNext) {
        if (pAppInfo->m_iProcessID == _iProcID) {
            return pAppInfo;
        }
    }
    
    return NULL;
}
    

//## ����hostname����HostID, If can't found, return -1.
int TRouteTable::getHostIDByHostName(char *_hostname) 
{
    TServerHostInfo *pHostInfo;
    for (pHostInfo = m_poHostInfo; pHostInfo; pHostInfo = pHostInfo->m_poNext ) {
        if (strcmp(pHostInfo->m_sHostName,_hostname) == 0)
            return pHostInfo->m_iHostID;
    }
    return -1;
}

//## ���� host_id ���� hostname
bool TRouteTable::getHostNameByHostID(int _iHostID, char _sHostName[])
{
    TServerHostInfo *pHostInfo;
    
    for (pHostInfo = m_poHostInfo; pHostInfo; pHostInfo = pHostInfo->m_poNext ) {
        if (pHostInfo->m_iHostID == _iHostID) {
            strcpy (_sHostName, pHostInfo->m_sHostName);
            return true;
        }
    }
    _sHostName[0] = 0;
    return false;
}

//## ���� host_id ����������Ϣ
TServerHostInfo * TRouteTable::getHostInfoByHostID(int _iHostID)
{
    TServerHostInfo *pHostInfo;
    
    for (pHostInfo = m_poHostInfo; pHostInfo; pHostInfo = pHostInfo->m_poNext ) {
        if (pHostInfo->m_iHostID == _iHostID)
            return pHostInfo;
    }
    return 0;
}

//## ���� _iProcID ���ҽ��̲��������ID, if can't found, return -1;
int TRouteTable::getHostIDByProcessID(int _iProcID, char *_sUserName)
{
    TServerAppInfo  *pAppInfo; 
    for (pAppInfo = m_poAppInfo; pAppInfo; pAppInfo = pAppInfo->m_poNext ) {
        if (pAppInfo->m_iProcessID == _iProcID) {
            strcpy (_sUserName, pAppInfo->m_sSysUserName);
            return pAppInfo->m_iHostID;
        }
    }
    return -1;
}

//## �������� (0-ʧ��, 1-�ɹ�)
int TRouteTable::startApp(int _iProcID,int _iProcessLogID, int *pFailedReason/* = 0x0*/)
{
    TServerAppInfo *pt = getServerAppInfo(_iProcID);
    
    if (pFailedReason) *pFailedReason = 0;
    
    if (!pt) {
        TWfLog::log(0,"TRouteTable::startApp, ���б����δ�ҵ� ProcessID = %d ��Ӧ�Ľ���!",_iProcID);
        return false;
    }

    int iIdx,iRes;
    iRes = g_pShmTable->getIdleAppInfo(_iProcID,iIdx);
    if (iRes == 0){
        TWfLog::log(0,"TRouteTable::startApp ProcessID:%d , Can't find idle AppInfo",_iProcID);
        return false;
    }
    if (iRes < 0){
        TWfLog::log(0,"TRouteTable::startApp ProcessID:%d , ��⵽�����Ѿ�������",_iProcID);
        if (pFailedReason) *pFailedReason = iRes;
        return false;
    }

    g_pShmTable->AppInfo[iIdx].init();
    g_pShmTable->AppInfo[iIdx].m_iProcessID = _iProcID;
    g_pShmTable->AppInfo[iIdx].m_iBillFlowID = pt->m_iBillFlowID;
    g_pShmTable->AppInfo[iIdx].m_iProcessLogID = _iProcessLogID;
    g_pShmTable->changeState(_iProcID,WF_START);

    switch(pt->m_iAppType) {
        
        case 0: //##  ��̨����            
            
            /*g_pRouteTable->*/
            createAssociMsgQueue(_iProcID); //ȷ����ǰ�����漰����Ϣ���ж���������
            
            pt->setState(WF_START); // ��Ϊ'����'״̬, �ȴ��������Ľ��̵��� ConnectMonitor();

            g_pShmTable->AppInfo[iIdx].m_iAppPID = startCommand(pt);
            
            if (g_pShmTable->AppInfo[iIdx].m_iAppPID <= 0){ //ִ��δ�ɹ�
                TWfLog::log(WF_FILE,"TRouteTable::startApp err after start Command: %s",pt->m_sExecCommand);
                g_pShmTable->AppInfo[iIdx].init();
                return false;
            }
            
            break;
                        
        case 1: // PL/SQL��

            pt->setState(WF_RUNNING);

            g_pShmTable->AppInfo[iIdx].m_iAppPID = startSQLCommand(pt);
            
            if (g_pShmTable->AppInfo[iIdx].m_iAppPID <= 0){ //ִ��δ�ɹ�
                TWfLog::log(WF_FILE,"TRouteTable::start_SqlCommand err  ProcessID:%d",pt->m_iProcessID);
                g_pShmTable->AppInfo[iIdx].init();
                return false;
            }
            
            break;        
        
        case 2: // ǰ̨Ӧ�ó���
            TWfLog::log(WF_FILE,"TRouteTable::startApp, ǰ̨�������ں�̨����");
            return false;
        
        case 3: // ��̨Unix shell�ű�
            
            pt->setState(WF_START);
            
            g_pShmTable->AppInfo[iIdx].m_iAppPID = startShell(pt);
            
            break;

        default:
            TWfLog::log(WF_FILE,"TRouteTable::startApp, unknown AppType:%d",pt->m_iAppType);
            return false;
    }
        
    return true;
}


//## ִ�б�����Ŀ�ִ�д���
int TRouteTable::startCommand(TServerAppInfo *pServApp)
{
    int iPos = 0,iForkRet = 0;

    char sPath[80],sCommand[80],*argv[20];
    char sString[80],sString2[80],sString3[80],sString4[80],*pt;
    char sExecName[80];
    strcpy(sPath,pServApp->m_sExecPath);    
    strcpy(sExecName,pServApp->m_sExecCommand);
    
    TWfLog::log(WF_FILE,"StartCommand: %s \n ExecPath: %s",sExecName,sPath);
    pt = strtok(sExecName," \n");
    argv[iPos++] = strdup(pt);    
    strcpy(sCommand,pt);
    while((pt = strtok(NULL," \n")) != NULL){
        argv[iPos] = strdup(pt);
        TWfLog::log(WF_FILE,"argv[%d],%s",iPos,argv[iPos]);
        iPos++;
    }
    argv[iPos] = NULL;
    
    iForkRet = fork();
    switch(iForkRet){
        
        case 0:
            if (strlen(sPath)) {
                chdir(sPath);
            }
            else {
                TWfLog::log(WF_FILE,"Not give ExecPath. sCommand:%s",sCommand);
            }
                
            usleep(10000);
            //Environment::getDBConn(true); //���ݿ�Ҫ���ӽ�����Ҫ�����������ݿ�
            
            sprintf(sString,"PROCESS_ID=%d",pServApp->m_iProcessID);
            putenv(sString);            
            sprintf(sString2,"BILLFLOW_ID=%d",pServApp->m_iBillFlowID);
            putenv(sString2);
            sprintf(sString3,"APP_ID=%d",pServApp->m_iAppID);
            putenv(sString3);
            sprintf(sString4,"USE_WF=1");
            putenv(sString4);
            
            execve(sCommand,argv,environ);
            
            exit(0);
            break;
            
        case -1:
            perror("fork");
            TWfLog::log(WF_FILE,"start_Command: fork err");
            cout<<"start_Command: fork err"<<endl;
            break;
            
        default:
            //Environment::getDBConn(true);
            TWfLog::log(0,"child process create ,pid:%d",iForkRet);
            cout<<"\n\n\n\n====>> Child process create ,pid:"<<iForkRet<<endl;
            break;
    }
    
    for (int i=0; i<iPos; i++) {
        if (argv[i]) { cout<<"argv["<<i<<"] = "<<argv[i]<<endl; delete argv[i]; }
        else break;
    }    
    return iForkRet;
}

//## ִ��PLSQL
int TRouteTable::startSQLCommand(TServerAppInfo *pServApp)
{
    int iForkRet = 0;
    char sSqlCommand[MAX_SQL_LEN];
    
    TWfLog::log(WF_FILE,"startSQL_Command: ProcessID-> %d",pServApp->m_iProcessID);
    
    strcpy(sSqlCommand,pServApp->m_sSqlStatement);

    iForkRet = fork();
    
     pServApp->setState(WF_NORMAL);
     if (g_pShmTable)
        g_pShmTable->changeState(pServApp->m_iProcessID,WF_NORMAL);
    /*
    switch (iForkRet) {
        
        case 0: // �����ӽ���
            try {
                Environment::getDBConn(true); //���ݿ�Ҫ���ӽ�����Ҫ�����������ݿ�
                DEFINE_QUERY(qry);
                
                usleep(10000); //?                
                //TWfLog::log(WF_FILE,sSqlCommand);
                qry.close();
                qry.setSQL(sSqlCommand);
                qry.execute();
                qry.commit();
                qry.close();
               
                pServApp->setState(WF_NORMAL);
                if (g_pShmTable)
                    g_pShmTable->changeState(pServApp->m_iProcessID,WF_NORMAL);
            }
            catch (TOCIException &e) {
                TWfLog::log(WF_FILE,"Execute sql err: \n%s \n%s ",e.getErrMsg(),e.getErrSrc());
                try {
                    pServApp->setState(WF_ABORT);
                    if (g_pShmTable)
                        g_pShmTable->changeState(pServApp->m_iProcessID,WF_ABORT);
                }
                catch (TOCIException &e2) {
                    TWfLog::log(WF_FILE,"pServApp->setState  or  g_pShmTable->changeState err:"
                        "%s \n%s ",e2.getErrMsg(),e2.getErrSrc());
                }
            }
            exit(0);
            
        case -1:
            perror("fork");
            TWfLog::log(WF_FILE,"Start_SqlCommand: fork err. ProcessID:%d",pServApp->m_iProcessID);
            break;
            
        default:
            Environment::getDBConn(true);
            TWfLog::log(WF_FILE,"Sql child process create ,pid:%d",iForkRet);
            break;
    }
    */
    return iForkRet;
}


//## ִ��Unix Shell�ű�
int TRouteTable::startShell(TServerAppInfo *pServApp)
{
    int iPos = 0,iForkRet = 0;

    char sPath[80],sCommand[80],*argv[20];
    char *pt;
    char sExecName[80];
    strcpy(sPath,pServApp->m_sExecPath);    
    strcpy(sExecName,pServApp->m_sExecCommand);
    
    TWfLog::log(WF_FILE,"start_Shell: sExecCommand-> %s\n ExecPath: %s",sExecName,sPath);
    pt = strtok(sExecName," \n");
    argv[iPos++] = strdup(pt);    
    strcpy(sCommand,pt);
    while((pt = strtok(NULL," \n")) != NULL){
        argv[iPos] = strdup(pt);
        TWfLog::log(WF_FILE,"argv[%d],%s",iPos,argv[iPos]);
        iPos++;
    }
    argv[iPos] = NULL;
    
    iForkRet = fork();
    switch(iForkRet){
        
        case 0:
            if (strlen(sPath))
                chdir(sPath);    
            usleep(10000);
            
            //Environment::getDBConn(true); //���ݿ�Ҫ���ӽ�����Ҫ�����������ݿ�
            
            // �˴�Ĭ�Ͻű���ִ�гɹ�
            pServApp->setState(WF_NORMAL);
            g_pShmTable->changeState(pServApp->m_iProcessID,WF_NORMAL);

            execvp(sCommand,argv);
                    
            exit(0);
            break;
            
        case -1:
            perror("fork");
            TWfLog::log(WF_FILE,"start_Command: fork err");
            break;
            
        default:
            //Environment::getDBConn(true);
            TWfLog::log(WF_FILE,"child process create ,pid:%d",iForkRet);
            break;
    }
    
    for (int i=0; i<iPos; i++) {
        if (argv[i]) { cout<<"argv["<<i<<"] = "<<argv[i]<<endl; delete argv[i]; }
        else break;
    }    
    return iForkRet;
}


//## ֹͣ���� (0-ʧ��, 1-�ɹ�)
int TRouteTable::stopApp(int _iProcID)
{
    TAppInfo* pt = g_pShmTable->getAppInfoByProcID(_iProcID);

    if (!pt) {
        TWfLog::log(WF_FILE,"TRouteTable::stopApp, ����ʵ��AppInfo��δ�ҵ� ProcessID = %d ��Ӧ�Ľ���!",_iProcID);
        return 0;
    }
    
    TServerAppInfo *ptServApp = getServerAppInfo(_iProcID);
    if (!ptServApp) {
        TWfLog::log(WF_FILE,"TRouteTable::stopApp, ���б��ServAppInfo��δ�ҵ� ProcessID = %d ��Ӧ�Ľ���!",_iProcID);
        return 0;
    }
    
    if (ptServApp->m_iAppType <= 3) {
        TWfLog::log(WF_FILE,"Stop process:%d, appPID:%d ",_iProcID,pt->m_iAppPID);
        if (ptServApp->m_iAppType == 0) {//��̨������Ŀ�ִ�д������
            
            if (pt->checkAlive()) {            
                pt->setState ('B'); //ɱ֮ǰ��"�м�̬" 
                ptServApp->setState('B');
                kill(pt->m_iAppPID,SIGUSR1);
                usleep(100000);
            }
            else {
                pt->setState ('X');
                ptServApp->setState('X');
            }
        }
        else //�Ǳ������, ������sql�ű�ִ�н��̡�Shell�ű�����
            kill(pt->m_iAppPID,9);
        return 1;
    }
    else if (ptServApp->m_iAppType == 4) {
        TWfLog::log(WF_FILE,"TRouteTable::stopApp, ǰ̨�������ں�ֹ̨ͣ");
        return 0;
    }
    else {
        TWfLog::log(WF_FILE,"TRouteTable::stopApp, unknown AppType:%d",ptServApp->m_iAppType);
        return 0;
    }
}

//## ǿ�иı�״̬(0-ʧ��, 1-�ɹ�)
int TRouteTable::changeState(int _iProcID,char _cState)
{
    TServerAppInfo *pt_server = getServerAppInfo(_iProcID);
    if (!pt_server) {
        TWfLog::log(WF_FILE,"TRouteTable::changeStatus, ���б����δ�ҵ� ProcessID = %d ��Ӧ�Ľ���!",_iProcID);
        return 0;
    }
    pt_server->setState(_cState);
    
    return 1;
}

void TRouteTable::killAllApp ()
{    
    int iLocalHostID;

    char *sUserName = NULL;
	  if ((sUserName = getenv ("ABM_HOST_NAME")) == NULL) {
		    cout<<"Please set env value ABM_HOST_NAME. For example  export ABM_HOST_NAME=abm3"<<endl;
		    THROW(-1);
    }
    
    iLocalHostID = getHostIDByHostName(sUserName);

    for (int i=0; i<MAX_APP_NUM; i++) {
        if (g_pShmTable->AppInfo[i].m_cState==WF_START || 
            g_pShmTable->AppInfo[i].m_cState==WF_RUNNING )
            {//�ý����Ѿ�������
            char sSysUserName[100];
            
            int iHostID = getHostIDByProcessID(g_pShmTable->AppInfo[i].m_iProcessID, sSysUserName);
            
            if (iHostID!=iLocalHostID || strcmp(sSysUserName,sUserName))
                continue;
            
            if (g_pShmTable->AppInfo[i].checkAlive()) {
                
                kill (g_pShmTable->AppInfo[i].m_iAppPID,9);
                
                changeState(g_pShmTable->AppInfo[i].m_iProcessID, WF_ABORT);
                
                TWfLog::log (WF_FILE,"Child application (ProcessID:%d, SysPID:%d) killed!",
                    g_pShmTable->AppInfo[i].m_iProcessID, g_pShmTable->AppInfo[i].m_iAppPID);
            }
        }
    }
}

//##################################################################

//���Ӧ�ý��̵�״̬
//�������״̬�쳣(״̬running, ����ʵ�ʲ�����), ����Abort״̬
//�����������Ϊ�쳣��Ҫ����, �����������ý���
void CheckApplication (int iHostID, char sUserName[])
{
    static int iCnt=0;
    static char sMsg[100];
    
    static time_t last=0;
    static time_t now=0;
    
    time (&now);
    if ((now - last) <= 3)
        return;
    
    last = now;
    
    if (!g_pRouteTable) return;
    
    for (int iPos=0; iPos<MAX_APP_NUM; iPos++)
    {
        //������Ǳ��������û�����,�򲻴���
        TServerAppInfo *pt = g_pRouteTable->getServerAppInfo (\
                    g_pShmTable->AppInfo[iPos].m_iProcessID );
        
        if (!pt) continue;
        
        if (pt->m_iHostID != iHostID  || strcmp (pt->m_sSysUserName, sUserName))
            continue;
        
        if (g_pShmTable->AppInfo[iPos].m_cState=='R' && !g_pShmTable->AppInfo[iPos].checkAlive())
        {
            g_pShmTable->AppInfo[iPos].setState ('X');
            sprintf (sMsg,"[Warning]�ػ����̼�⵽����(%d)�쳣�˳�!", g_pShmTable->AppInfo[iPos].m_iProcessID);
            g_pShmTable->AppInfo[iPos].pushInfo(sMsg);
            continue; //��⵽�쳣, ����������
        }
        
        if (g_pShmTable->AppInfo[iPos].m_cState == 'X' && pt->m_bAutoBootWhenAbort
            && !g_pShmTable->AppInfo[iPos].checkAlive()
            /*�����߼��Ͽ�����: Ӧ�ý��̸�DisconnectMonitor(Abort),��δ�������̵������
            ���������ý��̻᲻�ɹ�, ��Ϊ���ļ�������һ��Ӧ�ý���ֻ������һ��ʵ��*/ 
        )
        {
            int iProLogID = TLogProcessMgr::insertLogProcess(pt->m_iProcessID,"start", 0);
            int iRet = g_pRouteTable->startApp(pt->m_iProcessID, iProLogID);
            if (iRet == 0) {
                TWfLog::log(WF_FILE, "[Warning]�Զ������쳣�����Ľ���(%s)<ProcessID:%d>ʧ��!",
                    pt->m_sExecCommand, pt->m_iProcessID);
            }
        }       
    }
}


//��ӦӦ�ó�������ע���¼/ע��
void ResponseAppConnect()
{
    TAppReqInfo *pAppRq = &(g_pShmTable->AppReqInfo);
    
    if (pAppRq->m_iStatus == APP_REG_REGISTER) { //Ӧ�ó����¼
        
        int iIdx = GetAppIndex(pAppRq->m_iProcessID);
        int iSavIdx = GetSavBufIndex(pAppRq->m_iProcessID);
        
        if (iIdx<0 ) {
            TWfLog::log(WF_FILE,"App Login error. ProcessID:%d ",pAppRq->m_iProcessID);
            pAppRq->m_iStatus = APP_REG_REJECT;            
            return;
        }

        if (iSavIdx < 0) {
            TWfLog::log(WF_FILE,"App Login error. ����ID:%d ���� '������¼���������' ʧ��!",
                pAppRq->m_iProcessID);
            pAppRq->m_iStatus = APP_REG_REJECT;            
            return;
        }
                
        //�� AppInfo(����ʵ��) ״̬
        //g_pShmTable->AppInfo[iIdx].m_cState = WF_RUNNING;
        g_pShmTable->changeState(pAppRq->m_iProcessID,WF_RUNNING);
        g_pShmTable->AppInfo[iIdx].m_iAppPID = pAppRq->m_iAppPID;
        
        //�� ServAppInfo(���б��) ״̬
        int iRes = g_pRouteTable->changeState(pAppRq->m_iProcessID,WF_RUNNING);
        if (iRes == 0)
            TWfLog::log(WF_FILE,"App Login error. changeState.");
        
        //>> ���� m_wf_log_process(app_pid,start_date,state)
        cout<<"\niIdx: "<<iIdx<<endl;
        cout<<"iSavIdx: "<<iSavIdx<<endl;
        cout<<"AppInfo["<<iIdx<<"].m_iProcessLogID : "<< g_pShmTable->AppInfo[iIdx].m_iProcessLogID <<endl;
        cout<<"AppInfo["<<iIdx<<"].m_iAppPID : "<< g_pShmTable->AppInfo[iIdx].m_iAppPID <<endl;
        TLogProcessMgr::updateLogProcess(g_pShmTable->AppInfo[iIdx].m_iProcessLogID,
                WF_RUNNING,g_pShmTable->AppInfo[iIdx].m_iAppPID);
        
        pAppRq->m_iIndex = iIdx;
        pAppRq->m_iSaveBufIdx = iSavIdx;
        TWfLog::log(WF_FILE,"App Login sucess! ");
        pAppRq->m_iStatus = APP_REG_ACCEPT; //ע��ɹ�
    }
    else if (pAppRq->m_iStatus == APP_REG_UNREGISTER ) { //Ӧ�ó���ע��
        
        int iIdx;
        for (iIdx = 0; iIdx<MAX_APP_NUM; iIdx++ ) {
            if (g_pShmTable->AppInfo[iIdx].m_iProcessID == pAppRq->m_iProcessID)
                break;
        }
        if (iIdx == MAX_APP_NUM ) { //δ�ҵ���Ӧ�Ľ���
            TWfLog::log(WF_FILE,"App logout error. ProcessID:%d not found in ShmTable",
                pAppRq->m_iProcessID);
            return;
        }
        
        if (pAppRq->m_iAppPID != g_pShmTable->AppInfo[iIdx].m_iAppPID) {
        //ϵͳ����ID��Ӧ����
            TWfLog::log(WF_FILE,"App Logout error. ProcessID:%d, AppPID:%d; PID in SHM:%d",
                pAppRq->m_iProcessID,pAppRq->m_iAppPID, g_pShmTable->AppInfo[iIdx].m_iAppPID );
            return;
        }
        
        //�� AppInfo(����ʵ��) ״̬
        g_pShmTable->changeState(pAppRq->m_iProcessID,pAppRq->m_cState);
        
        //�� ServAppInfo(���б��) ״̬
        int iRes = g_pRouteTable->changeState(pAppRq->m_iProcessID,pAppRq->m_cState);
        if (iRes == 0)
            TWfLog::log(WF_FILE,"App Logout error. changeState.");
        
        
        TWfLog::log(WF_FILE,"App LogOut sucess! ");
        pAppRq->m_iStatus = APP_REG_ACCEPT;
        
        return;
    }    
    return;
}

// indexID or -1
int GetAppIndex(int _iProcID)
{
    for (int idx = 0; idx<MAX_APP_NUM; idx++) {
        if (g_pShmTable->AppInfo[idx].m_iProcessID == _iProcID)
            return idx;
    }
    
    //���ǽ��̲����ػ��������������
    if (g_pShmTable && g_pRouteTable) {

        int iIdx,iRes;
        
        TServerAppInfo *pt = g_pRouteTable->getServerAppInfo(_iProcID);
        if (!pt) {
            TWfLog::log(0,"GetAppIndex Err., ���б����δ�ҵ� ProcessID = %d ��Ӧ�Ľ���!",_iProcID);
            return -1;
        }
        
        iRes = g_pShmTable->getIdleAppInfo(_iProcID,iIdx);
        if (iRes == 0){
            TWfLog::log(0,"GetAppIndex Err.  ProcessID:%d , Can't find idle AppInfo",_iProcID);
            return -1;
        }
        if (iRes < 0){
            TWfLog::log(0,"GetAppIndex Err.  ProcessID:%d , ��⵽�����Ѿ�������",_iProcID);
            return -1;
        }
        
        g_pShmTable->AppInfo[iIdx].init();
        g_pShmTable->AppInfo[iIdx].m_iProcessID = _iProcID;
        g_pShmTable->AppInfo[iIdx].m_iBillFlowID = pt->m_iBillFlowID;
        g_pShmTable->AppInfo[iIdx].m_iProcessLogID = 0;
        //g_pShmTable->changeState(_iProcID,WF_START);
        
        return iIdx;
    }
    else 
        return -1;
}

int GetSavBufIndex(int _iProcessID)
{
    int iSavBufIdx = -1;
    
    for (iSavBufIdx = 0; iSavBufIdx<MAX_APP_NUM; iSavBufIdx++) {
        if (g_pShmTable->AppSaveBuff[iSavBufIdx].getProcessID() == _iProcessID)
        {
            //g_pShmTable->AppSaveBuff[iSavBufIdx].setProcessID(_iProcessID);
            return iSavBufIdx;
        }
    }
    
    for (iSavBufIdx = 0; iSavBufIdx<MAX_APP_NUM; iSavBufIdx++) {
        if (g_pShmTable->AppSaveBuff[iSavBufIdx].getProcessID() == -1)
        {
            g_pShmTable->AppSaveBuff[iSavBufIdx].setProcessID(_iProcessID);
            return iSavBufIdx;
        }
    }

    return -1;
}

void CalcTreatRate()
{
    static time_t tNow;
    time (&tNow);
    
    for (int iIdx = 0; iIdx<MAX_APP_NUM; iIdx++ ) {
        
        TAppInfo *pt = &(g_pShmTable->AppInfo[iIdx]);
        
        if (   pt->m_iProcessID > 0                 //��Ч����ID
            && pt->m_cState == WF_RUNNING           //����������
            && (tNow - pt->m_tLatestCalcTime) >= 10 //�������10������
        ){
            float fProcAdded = (float)(pt->m_lProcessCnt - pt->m_lLatestProcCnt);
            int   iTimeAdded = tNow - pt->m_tLatestCalcTime;
            pt->m_fTreatRate = (float)fProcAdded/iTimeAdded;
            pt->m_tLatestCalcTime = tNow;
            pt->m_lLatestProcCnt = pt->m_lProcessCnt;
        }
    }
}

///////////////////////////////////////////////////////////////


