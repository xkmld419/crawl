/*VER: 1*/ 
#include <time.h>

//#include "GuardMain.h"
#include "Log.h"
#include "Environment.h"
#include "MessageQueueH.h"
//#include "ParamDefineMgr.h"
#include "mntapi.h"

#include "ABMException.h"
#include "GetinfoMonitor.h"

#ifndef DISCARD_EVENT_TYPE_ID
#define DISCARD_EVENT_TYPE_ID 9999
#endif

#define MONITOR_CLOSE

extern char ** g_argv;

TSHMCMA *   g_pShmTable = NULL;
TAppInfo *  g_pAppInfo = NULL;

static THeadInfo *pInfoHead = NULL;
static TProcessInfo *pCurProc = NULL;
static int g_iRegistered;
static int g_iProcessID = 0;
static int g_iBillflowID;



//static int g_bUseWf = 0;
static TAppSaveBuff *g_poAppSaveBuff = NULL;
static int shm_id;
static void *pMemHead = NULL;

//��ع����ڴ�ID(�ڱ�m_b_ipc_cfg�ж���)
long g_lBillMntKey = 0; 
long g_lWorkFlowKey = 0;
long g_lLockWorkFlowKey = 0;
long g_lLockApplicationKey = 0;
long g_lLockClientKey = 0;

int ConnectTerminalMonitor (int g_iProcID)
{
    int i, iProcID;
    char *env;

    //env = getenv ("PROCESS_ID");
    //if (env != NULL) {
        iProcID = g_iProcID;
/*#ifdef FUNCTIONTEST_MODE
        printf ("[MESSAGE]: PROCESS_ID = [%d]\n", iProcID);
#endif
    } else {
        printf ("[MESSAGE]: No PROCESS_ID specified in environment\n");
        return -1;
    }
*/
    if (AttachShm (0, (void **)&pInfoHead) < 0) {
        printf ("[MESSAGE]: Not ConnectTerminalMonitor!\n");
        return -1;
    }
    
    MntAPI::m_pProcInfoHead = &pInfoHead->ProcInfo;
    
    pCurProc = &pInfoHead->ProcInfo;
    for (i=0; i<pInfoHead->iProcNum; i++) {

        if (pCurProc->iProcessID == iProcID)
            break;
        pCurProc++;
    }

    if (i >= pInfoHead->iProcNum) {
        pCurProc = NULL;
        return -1;
    }

    time ((time_t *)&pCurProc->lLoginTime);
    
    pCurProc->tSaveTime = pCurProc->lLoginTime;
    pCurProc->lSaveTickets = 0;
    
    pCurProc->iAllTickets = 0;
    pCurProc->iNormalTickets = 0;
    pCurProc->iErrTickets = 0;
    pCurProc->iBlackTickets = 0;
    pCurProc->iOtherTickets = 0;

    pCurProc->iSysPID = getpid ();

    pCurProc->iState = ST_RUNNING;

    return 0;

}

int DisconnectTerminalMonitor (int iState)
{
   // Log::log (0, "%s(ProcID:%d) into DisconnectTerminalMonitor()",g_argv[0], g_iProcessID);
    if (pCurProc) {
        time ((time_t *)&pCurProc->lLoginTime);
        pCurProc->iState = iState;
    }

    DetachShm ();
    pCurProc = NULL;
    
    return 0;
}

int AddTicketStat (int iType, int iNum)
{
    if (!pCurProc)
        return 0;

    switch (iType) {
    case TICKET_NORMAL:
        pCurProc->iNormalTickets += iNum;
        break;
    case TICKET_ERROR:
        pCurProc->iErrTickets += iNum;
        break;
    case TICKET_BLACK:
        pCurProc->iBlackTickets += iNum;
        break;
    case TICKET_OTHER:
        pCurProc->iOtherTickets += iNum;
        break;
    default:
        pCurProc->iOtherTickets += iNum;
        break;
    }

    pCurProc->iAllTickets += iNum;
    
    return pCurProc->iAllTickets;
}

int SetProcState (int iState)
{
    pCurProc->iState = iState;
    return 0;
}


/*
 *����
 *  GetBillFlowID
 *����
 *  ���ص�ǰ���̵����ʶ
 */
int GetBillFlowID()
{
    char * env;
    if (!pCurProc){
            if( (env=(char *)getenv("BILLFLOW_ID") )!=NULL)
                    return  atoi(env);
            else
                    return 0;
    }

    return pCurProc->iBillFlowID;
}


static bool GetBillMntKey ()
{
    ABMException oExp;
    MonitorInfo oIPCMonitor;
    g_lBillMntKey = oIPCMonitor.getInfoIpc(oExp,"SHM_BillMnt");
    //Environment::getDBConn()->commit();
    if (g_lBillMntKey >=0)
        return true;
    
    TWfLog::log (0,"��ȡ�����ڴ������(SHM_BillMnt)");
    
    return false;   
}

int CreateShm (long lSize, void **pRet)
{
    if (!g_lBillMntKey) {
        if (!GetBillMntKey ())
            return -1;
    }
    
    shm_id = shmget (g_lBillMntKey, lSize, 0660|IPC_EXCL|IPC_CREAT);
    if (shm_id < 0) {
        return -1;
    }

    pMemHead = shmat (shm_id, 0, 0);
    if (pMemHead == NULL) {
        return -1;
    }

    *pRet = (void *)pMemHead;
    return 0;

}

int AttachShm (long lSize, void **pRet)
{
    if (!g_lBillMntKey) {
        if (!GetBillMntKey ())
            return -1;
    }
    
    //add by yks 2007.07.27
    if (pMemHead)
        DetachShm ();
    
    shm_id = shmget (g_lBillMntKey, lSize, 0660);
    if (shm_id < 0) {
        return -1;
    }

    pMemHead = shmat (shm_id, 0, 0);
    if (pMemHead == NULL) {
        return -1;
    }

    *pRet = (void *)pMemHead;
    return 0;
}

int DetachShm ()
{
    shmdt ((char *)pMemHead);
    pMemHead = NULL;
    return 0;
}

int DestroyShm ()
{
    if (!g_lBillMntKey) {
        if (!GetBillMntKey ())
            return -1;
    }
    
    shm_id = shmget (g_lBillMntKey, 0, 0660);
    if (shm_id < 0) {
        return -1;
    }

    shmctl (shm_id, IPC_RMID, 0);
    return 0;
}

/////////////////////////////////////////////////////////////////////


void TreatSingleQuotes(char src[])
{
    char sTemp[2000] = {0};
    char *ptTmp = &(sTemp[0]);
    char *ptSrc = &(src[0]);
    int iLen=0;
    for ( ; *ptSrc; ptSrc=ptSrc+1) {
        if (*ptSrc == '\'') {
            *ptTmp='\'';
            ptTmp = ptTmp+1;
            iLen++;
            *ptTmp='\'';
            ptTmp = ptTmp+1;
            iLen++;
        }
        else {
            *ptTmp = *ptSrc;
            ptTmp = ptTmp+1;
            iLen++;
        }
        if (iLen>=999) {
           sTemp[999]=0;
           break;
        }
    }
    strcpy(src,sTemp);
}

bool ConnectMonitor(int g_iProcID)
{
    cout<<endl;
    
    //char *pt;
    char sMsg[100];
    
    int iShmID;
    
    if ((g_iProcessID=GetProcessID(g_iProcID)) == -1)
        return false;
    
    usleep(100000);
    ABMException oExp;
    MonitorInfo oIPCMonitor;
    
    if (!g_lWorkFlowKey) {
        g_lWorkFlowKey = oIPCMonitor.getInfoIpc(oExp,"SHM_WorkFlow");
        if (g_lWorkFlowKey<0) {
            TWfLog::log (0,"��ȡ�����ڴ��ַʧ��(SHM_WorkFlow)");
            return false;
        }
    }
    
    if ((iShmID = shmget(g_lWorkFlowKey,0,0))<0){
        perror("shmget");
        return false;
    }
    
    g_pShmTable = (TSHMCMA *)shmat(iShmID,(void *)0,0);
        
    if (-1 == (long)g_pShmTable){
        perror("shmat");
        return false;
    }

    cout<<"[MESSAGE]: Process:"<<g_iProcessID<<" Monitor Connecting ..."<<endl;
    
    LockSafe(g_pShmTable->m_iAppLock,0);
    
    cout<<endl<<"locked..."<<endl;
    
    g_pShmTable->AppReqInfo.m_iProcessID = g_iProcessID;
    g_pShmTable->AppReqInfo.m_iAppPID = getpid();
    g_pShmTable->AppReqInfo.m_iStatus = APP_REG_REGISTER;
    
    sprintf (sMsg, "�ػ����ܾ̾�Ӧ�ý������Ӽ�ص�����");
    while( g_pShmTable->AppReqInfo.m_iStatus == APP_REG_REGISTER)
    {
        static int iCount = 0;
        if (++iCount > 1000) {
            g_pShmTable->AppReqInfo.m_iStatus = APP_REG_REJECT;
            sprintf (sMsg, "�ػ�����δ����Ӧ�������Ӽ�ص�����,����GuardMain���̲�����!");
            break;
        }
        usleep(10000);
    }
    
    if (g_pShmTable->AppReqInfo.m_iStatus != APP_REG_ACCEPT){
        cout<<"[MESSAGE]: "<<sMsg<<endl;
        UnlockSafe(g_pShmTable->m_iAppLock,0);
        return false;
    }
    
    // get GuardMain msg.
    g_poAppSaveBuff = &( g_pShmTable->AppSaveBuff[g_pShmTable->AppReqInfo.m_iSaveBufIdx]);
    
    g_pAppInfo = &( g_pShmTable->AppInfo[g_pShmTable->AppReqInfo.m_iIndex]);
        
    g_iRegistered = 1;
    
    g_iBillflowID = g_pAppInfo->m_iBillFlowID;
    
    UnlockSafe(g_pShmTable->m_iAppLock,0);

    Pprintf(0,1,0,"Ӧ�ý���������(ProcessID:%d)",g_iProcessID); 
    
    return true;
}

//## Ӧ�ó����˳����
bool DisConnectMonitor(char _cMode)
{
    
    if (g_iRegistered == 0)
        return -1;
    
    /*
    if (g_bUseWf != 1) {
        cout<<"[MESSAGE]: Do not need Disconnect WorkFlow."<<endl;
        return true;
    }
    */
#ifdef FUNCTIONTEST_MODE
    Log::log (0, "�˳����������!");
    TWfLog::log (0, "�˳����������!");
    cout<<endl<<" �˳���������� ..."<<endl;
#endif        
    LockSafe(g_pShmTable->m_iAppLock,0);
    
    g_pShmTable->AppReqInfo.m_iProcessID = g_iProcessID;
    
    if (g_pAppInfo)
        g_pShmTable->AppReqInfo.m_iAppPID = g_pAppInfo->m_iAppPID;
        
    g_pShmTable->AppReqInfo.m_cState = _cMode;
    g_pShmTable->AppReqInfo.m_iStatus = APP_REG_UNREGISTER;
    
    while( g_pShmTable->AppReqInfo.m_iStatus == APP_REG_UNREGISTER ) {
        static int i = 0;
        usleep(10000);
        if (i++>1000)
            break;
    }
    Pprintf(0,1,0,"Ӧ�ý���ֹͣ!(ProcessID:%d)",g_iProcessID); 
    g_iRegistered = 0;
    UnlockSafe(g_pShmTable->m_iAppLock,0);
    shmdt( (char*)g_pShmTable );
    
    g_pAppInfo = 0x0;
    
    cout<<"[MESSAGE]: Process:"<<g_iProcessID<<" Monitor Disconnected . "<<endl<<endl;

    return true;
}


/*
���Ӧ�ý�����־���澯��Ϣ��ǰ̨���Ƚ���
    iAlertCode == 0ʱ ��ʾ�Ǹ澯��Ϣ
        (AlertCode �ڱ�BILL.WF_ALERT_DEFINE ���н���,��ģ�����Ӹ澯����ʱ,
        ��Ҫ��CommonMacro.h���ͷ�ļ������ݿ����������Ӧ�ļ�¼)
    bInsertDB: �Ƿ���Ҫ��澯��־��(m_wf_alert_log)

*/
bool Pprintf(int iAlertCode,bool bInsertDB,int iFileID,char const *fmt,...)
{
    va_list argptr;
    char sMsgSrc[1024],sMsgBuilt[1024];

    /*    ������ݿ�û�гɹ����ӣ� ǿ�Ʋ����*******Ҫ���ۣ�by xueqt */
    if (!(DB_CONNECTED)) bInsertDB = false;
    
    va_start(argptr, fmt);
    vsprintf(sMsgSrc,fmt, argptr);
    va_end(argptr);

    if (iAlertCode) {
        sprintf(sMsgBuilt,"[WARNING]: �澯����:%d |",iAlertCode);
    } else {
        sprintf(sMsgBuilt,"[MESSAGE]: ");
    }

    if (iFileID>0) {
        sprintf(sMsgBuilt,"%s �����ļ�ID:%d ",sMsgBuilt,iFileID);
    }

    sprintf(sMsgBuilt,"%s%s",sMsgBuilt,sMsgSrc);
    
    //�滻sMsgSrc�еĵ�����
    TreatSingleQuotes(sMsgSrc);
    
    sMsgBuilt[MAX_APPINFO__LEN-1]=0;
    
    int iProcLogID=0;
        
    if (g_pAppInfo) {
        g_pAppInfo->pushInfo(sMsgBuilt);
        iProcLogID = g_pAppInfo->m_iProcessLogID;
    }
    
    /*
    if (bInsertDB) {
        char sql[2048];
        DEFINE_QUERY(qry);
        qry.close();
        qry.setSQL("select seq_alert_log_id.nextval log_id from dual");
        qry.open();         
        qry.next();         
        int iLogID = qry.field("log_id").asInteger();
 
        qry.close();
        sprintf(sql,
          " insert into m_wf_alert_log(alert_log_id,process_log_id,msg_date,msg_time, \n"
          " process_id,file_id,alert_code,alert_content) values ( \n"
          " %d,%d,to_char(sysdate,'yyyymmdd'),to_char(sysdate,'hh24:mi:ss'), \n"
          " %d,%d,%d,%d,'%s' )",iLogID,iProcLogID,g_iProcessID,
          iFileID,iAlertCode,sMsgSrc);

        qry.setSQL(sql);
        qry.execute();
        qry.commit();
        qry.close();
    }
    */
    return true;      
}


/*
 * ����
 *����WriteMsg
 * ����
 *    Ӧ�ó��������Ϣ
 * ����
 *    ��Ϣ����
 *    ��Ϣ����
 */
int WriteMsg(int code, const char *fmt, ...)
{

        va_list ap;
        char sBuff[256];

        va_start (ap, fmt);
                vsprintf (sBuff, fmt, ap);
        vprintf(fmt,ap);
        va_end (ap);

        if (!pCurProc)
                return 0;

    time ((time_t *)&(pCurProc->MsgItem[pCurProc->iCurMsgPos].lTime));
    pCurProc->MsgItem[pCurProc->iCurMsgPos].iMsgType = code;
    pCurProc->MsgItem[pCurProc->iCurMsgPos].iNewFlag = 1;
    strncpy (pCurProc->MsgItem[pCurProc->iCurMsgPos].sMsg, sBuff, MAX_MSG_LEN);
    pCurProc->iCurMsgPos = (pCurProc->iCurMsgPos+1) % MAX_MSG_NUM;

        return 0;
}

/*
 * ����
 *����WriteErr
 * ����
 *    Ӧ�ó�����������澯
 * ����
 *    ��Ϣ����
 *    ��Ϣ����
 */
int WriteErr(int code, const char *fmt, ...)
{

        va_list ap;
        char sBuff[256];

        va_start (ap, fmt);
                vsprintf (sBuff, fmt, ap);
        vprintf(fmt,ap);
        va_end (ap);

        if (!pCurProc)
                return 0;

    time ((time_t *)&(pCurProc->ErrItem[pCurProc->iCurErrPos].lTime));
    pCurProc->ErrItem[pCurProc->iCurErrPos].iMsgType = code;
    pCurProc->ErrItem[pCurProc->iCurErrPos].iNewFlag = 1;
    strncpy (pCurProc->ErrItem[pCurProc->iCurErrPos].sMsg, sBuff, MAX_MSG_LEN);
    pCurProc->iCurErrPos = (pCurProc->iCurErrPos+1) % MAX_MSG_NUM;

        return 0;
}


//���ӵ�ǰӦ�ý������ݴ�����(����һ�μ�1)
bool AddProcessCnt(int _iAddCnt)
{
    if (!g_pAppInfo) 
        return false;
    
    g_pAppInfo->m_lProcessCnt += _iAddCnt;
    return true;
}

//��ȡ��ǰӦ�ý���ID,��Ӧm_wf_process.PROCESS_ID
//δ��ȡ��ʱ,���� -1 .
int GetProcessID(int g_iProcID)
{
    char *pt;
    
    if (g_iProcessID > 0)
        return g_iProcessID;
    else 
        g_iProcessID=g_iProcID;
    /*
    if ((pt = getenv ("PROCESS_ID")) == NULL) {
        cout << "[ERROR]: getenv(PROCESS_ID) == NULL, ��ȡ��������PROCESS_IDʧ��" <<endl;
        return -1;
    }
    g_iProcessID = atoi (pt);
    cout <<"[MESSAGE]: Envionment variable: PROCESS_ID = "<<g_iProcessID<<endl;
    */
    // ˳����� g_iBillflowID (����Ӧ�ý��̲�����MC��ʱ��g_iBillflowIDΪ0)
    g_iBillflowID = 7;
    
    //qry.close();
    
    return g_iProcessID;
}

bool SaveLastEvent(StdEvent *pEvt)
{
    if (!g_poAppSaveBuff) return false;
    g_poAppSaveBuff->saveEvent(*pEvt);
    return true;
}
bool GetLastEvent(StdEvent *pEvt)
{
    if (!g_poAppSaveBuff) return false;

    return g_poAppSaveBuff->getEvent(*pEvt);
}
bool SetEventState (int _iState) //## ���õ�ǰ�¼��Ĵ���״̬, ��ʼֵ0
{
    if (!g_poAppSaveBuff) return false;
    g_poAppSaveBuff->setEventState (_iState);
    
    return true;
}
bool GetEventState (int *piState)
{   //## �¼������ڷ���false; ���ڷ���true, *piState = ʵ��״̬
    if (!g_poAppSaveBuff) return false;
    return g_poAppSaveBuff->getEventState (piState);
}
bool SetSaveBuff (void *pt, int iLen)
{
    if (!g_poAppSaveBuff) return false;
    g_poAppSaveBuff->setSaveBuff (pt, iLen);
    return true;
}
bool GetSaveBuff (void *pt, int iLen)
{
    if (!g_poAppSaveBuff) return false;
    g_poAppSaveBuff->getSaveBuff (pt, iLen);
    return true;
}
bool SetSaveValue (int iValueID, long lValue)
{
    if (!g_poAppSaveBuff) return false;
    return g_poAppSaveBuff->setSaveValue (iValueID, lValue);
}
bool GetSaveValue (int iValueID, long &lValue)
{
    if (!g_poAppSaveBuff) return false;
    return g_poAppSaveBuff->getSaveValue (iValueID, lValue);
}


// (�ڲ�ʹ��)���ݽ���ID��ȡ��SavBuf��ַ
TAppSaveBuff *GetSaveBufByProcID (int iProcessID)
{
    int iProcID = iProcessID;
    TAppSaveBuff *pSavBuf = g_poAppSaveBuff; //Ĭ�����õ�ǰ���̵ĵ�ַ
        
    //## ȡ����������ID
    if (iProcID == 0) { //## �������ID��    
        /*if (g_iProcessID <= 0) {
            GetProcessID();
            if (g_iProcessID <=0 )
                return 0x0;
        }*/
        iProcID = g_iProcessID;
    }
    
    if ((iProcID == g_iProcessID) && pSavBuf)
        return pSavBuf;
    
    if (!g_pShmTable) return false;
    
    pSavBuf = g_pShmTable->getAppSaveBuffByProcID(iProcID);
    
    return pSavBuf;
}

//## ��ȡָ�����̵��û���ѹ��־(���iProcessIDΪ0, ��ȡ��ǰ����ID)
bool SetServBlockFlag (int iFlag, int iProcessID)
{
    TAppSaveBuff *pSavBuf = GetSaveBufByProcID (iProcessID);
    
    if (!pSavBuf) return false;
    
    pSavBuf->setServBlockFlag(iFlag);
    
    return true;
}
int  GetServBlockFlag (int iProcessID)
{
    TAppSaveBuff *pSavBuf = GetSaveBufByProcID (iProcessID);
        
    if (!pSavBuf) return -1;
    
    return pSavBuf->getServBlockFlag(); 
}

//## ������iProcessID �Ƿ����
bool CheckProcAlive (int iProcessID)
{
    TAppInfo *pApp ;
    
    if (iProcessID <= 0 || !g_pShmTable) return false;
    
    pApp = g_pShmTable->getAppInfoByProcID (iProcessID);
    
    if (!pApp) return false;
    
    return pApp->checkAlive ();
}


/////////////////////////////////////////////////

void TStatLog::reset()
{
    m_sFileName[0] = 0;
    m_iInNormal=0; 
    m_iInBlack =0; 
    m_iInDup=0;
    m_iInPreErr=0; 
    m_iInPricErr=0; 
    m_iInUseless=0;
    m_iOutNormal=0; 
    m_iOutBlack=0; 
    m_iOutDup=0; 
    m_iOutPreErr=0; 
    m_iOutPricErr=0; 
    m_iOutUseless=0;
    m_iFormatErr=0; 
    m_iOutFormatErr=0; 
    m_iRemainA =0; 
    m_iRemainB=0;
    m_poNext = NULL;
}

void TStatLog::addInNormal(int iCnt)
{
    m_iInNormal+=iCnt;
}
void TStatLog::addInBlack(int iCnt)
{
    m_iInBlack+=iCnt;

}
void TStatLog::addInDup(int iCnt)
{
    m_iInDup+=iCnt;
}
void TStatLog::addInPreErr(int iCnt)
{
    m_iInPreErr+=iCnt;
}
void TStatLog::addInPricErr(int iCnt)
{
    m_iInPricErr+=iCnt;
}
void TStatLog::addInUseless(int iCnt)
{
    m_iInUseless+=iCnt;
}
void TStatLog::addOutNormal(int iCnt)
{
    m_iOutNormal+=iCnt;
    
    if (g_pAppInfo)
        g_pAppInfo->m_lNormalCnt+=iCnt;
}
void TStatLog::addOutBlack(int iCnt)
{
    m_iOutBlack+=iCnt;
    
    if (g_pAppInfo)
        g_pAppInfo->m_lBlackCnt+=iCnt;
}
void TStatLog::addOutDup(int iCnt)
{
    m_iOutDup+=iCnt;
    
    if (g_pAppInfo)
        g_pAppInfo->m_lOtherCnt+=iCnt;
}
void TStatLog::addOutPreErr(int iCnt)
{
    m_iOutPreErr+=iCnt;
    
    if (g_pAppInfo)
        g_pAppInfo->m_lErrorCnt+=iCnt;
}
void TStatLog::addOutPricErr(int iCnt)
{
    m_iOutPricErr+=iCnt;
    
    if (g_pAppInfo)
        g_pAppInfo->m_lErrorCnt+=iCnt;
}            
void TStatLog::addOutUseless(int iCnt)
{
    m_iOutUseless+=iCnt;
    
    if (g_pAppInfo)
        g_pAppInfo->m_lOtherCnt+=iCnt;
}
void TStatLog::addFormatErr(int iCnt)
{
    m_iFormatErr+=iCnt;
    
    if (g_pAppInfo)
        g_pAppInfo->m_lErrorCnt+=iCnt;
}

void TStatLog::addOutFormatErr(int iCnt)
{
    this->m_iOutFormatErr+=iCnt;
    
    if (g_pAppInfo)
        g_pAppInfo->m_lErrorCnt+=iCnt;
}




// ���ؽ���(iProcID)����Ϣ��������Ϣ��Ŀ, ��Ϣ���в����ڷ���-1
int  MntAPI::getMsgNumber (int iProcID)
{
    TProcessInfo *pt = getProcessInfo (iProcID);
    if (!pt)  return -1;
    
    if (pt->iMqID <= 0)
        return -1;
    
    MessageQueue mq (pt->iMqID);
    //MessageQueue *mq = new MessageQueue(pt->iMqID);
    if (mq.exist()) {
    //if (mq->exist ()) {
        return mq.getMessageNumber();
        //return mq->getMessageNumber();
    }
    else
        return -1;
}


// ͣ���� iProcID
// bCheckMQ : �Ƿ�ȴ�����������Ϣ�������ݴ��������ͣ����
// iWaitSecs: �ȴ� iWaitSecs ���,��Ȼδ��ֹͣ����, �򷵻�. (Ϊ0ʱ,һֱ�ȴ�)
// ����
//   1 -- �ɹ�
//   0 -- ���̵�ǰ����δ����״̬ (�� sleep,abort��), ����Ҫִ��ֹͣ����
//  -1 -- ��Ϣ�������ݳ�ʱδ������
//  -2 -- �ѷ���ֹͣ�����ź�, ���ǽ��̳�ʱδ�˳�
//  -3 -- ���̵�ǰ״̬�쳣 (��һֱ���� ��������<init>, ����ֹͣ<downing>״̬), 
//        ��������ֹͣ�����ź�
//  -4 -- ����ID������
int  MntAPI::stopProcess (int iProcID, int iWaitSecs, bool bCheckMQ)
{
    int iSleep = 0, iCnt = 0;
    
    TProcessInfo *pt = getProcessInfo (iProcID);
    if (!pt)  return -4;
    
    if (pt->iState==ST_KILLED || pt->iState==ST_ABORT || pt->iState==ST_SLEEP)
        return 0;
    
    while (pt->iState==ST_INIT || pt->iState==ST_DESTROY 
        || pt->iState==ST_WAIT_BOOT  || pt->iState==ST_WAIT_DOWN )
    {
        if (iCnt++ > 5)
            return -3;
        sleep (1);
    }
    
    //## ����ж���,�ȵ����п�
    if (bCheckMQ) {
        while (getMsgNumber(iProcID) > 0) {
            if (iSleep++ > iWaitSecs && iWaitSecs!=0)
                return -1;
            sleep (1);
        }
    }
    
    //## ֹͣ����
    iSleep = 0;
    if (pt->iState != ST_RUNNING)
        return -4;
    
    Log::log (0, "����ֹͣ����:[%s](%d)", pt->sName, iProcID);
    TWfLog::log (0, " @@@@@@@@ Ӧ�ý���[%s](%d)����ֹͣ����:[%s](%d)", 
            pCurProc->sName,g_iProcessID,  pt->sName,iProcID);
    
    pt->iState = ST_WAIT_DOWN;
    
    while (pt->iState != ST_KILLED && pt->iState != ST_ABORT && pt->iState != ST_SLEEP)
    {
        if (iSleep++ > iWaitSecs  &&  iWaitSecs!=0)
        {
            Log::log (0, "����ֹͣ����ʧ��:[%s](%d)", pt->sName, iProcID);
            TWfLog::log (0, " @@@@@@@@ Ӧ�ý���[%s](%d)����ֹͣ����ʧ��:[%s](%d)", 
                pCurProc->sName,g_iProcessID,  pt->sName,iProcID);
            return -2;
        }
        sleep (1);
    }
    
    return 1;
}
    
    
// �������� iProcID
// ����
//   1 -- �ɹ�
//   0 -- ��ǰ�����Ѿ���������״̬, ����Ҫ�ٴ�����
//  -1 -- ����ʧ�� (����ID������) 
//  -2 -- ��ǰ����״̬�쳣(���ܷ��������ź�)
//  -3 -- ���������źź��������ɹ�
int  MntAPI::startProcess (int iProcID)
{
    int iSleep = 0;
    
    TProcessInfo *pt = getProcessInfo (iProcID);
    if (!pt)  return -1;
    
    if (pt->iState==ST_RUNNING || pt->iState==ST_WAIT_BOOT ||pt->iState==ST_INIT)
        return 0;
    
    if (pt->iState==ST_DESTROY || pt->iState==ST_WAIT_DOWN)
        return -2;
    
    Log::log (0, "������������:[%s](%d)", pt->sName, iProcID);
    TWfLog::log (0, " @@@@@@@@ Ӧ�ý���[%s](%d)������������:[%s](%d)", 
            pCurProc->sName,g_iProcessID,  pt->sName,iProcID);
    
    pt->iState = ST_WAIT_BOOT;
    
    while (pt->iState != ST_RUNNING)
    {
        if (iSleep++ > 5)
            return -3;
        sleep (1);
    }
    
    return 1;
}

TProcessInfo * MntAPI::getProcessInfo (int iProcID)
{
    TProcessInfo *pt;
    
    for (pt = m_pProcInfoHead;  pt;  pt++)
    {
        if (pt->iProcessID == iProcID)
            return pt;
    }
    
    return 0;
}

/////////////////////////////////////

//KeyTree<int>* TStatLogMgr::m_pSecLevelEventType = NULL;

//EventTypeMgr* TStatLogMgr::m_poEventTypeMgr = NULL;

//TStatLog* TStatLogMgr::m_poLog = NULL;

TProcessInfo *MntAPI::m_pProcInfoHead = NULL;

