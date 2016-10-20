/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "Server.h"

static const int TIME_OUT = 500;

static TSHMCMA *g_pShmTable = NULL;


extern "C" { //begin extern "C"

int tpsvrinit(int argc, char **argv)
{
    userlog("\n    server_client starting...");
    ABMException oExp;
    MonitorInfo oIPCMonitor;
    long lShmKey = oIPCMonitor.getInfoIpc(oExp,"SHM_WorkFlow");
		Environment::getDBConn()->commit();    	
    if (lShmKey < 0) {
        userlog("\n    server_client start error when tpsvrinit, getshm err SHM_WorkFlow\n"
                "    ��ȡ�����ڴ�KEYʧ��! ���ܱ�m_b_ipc_cfgδ����, �˳�!");
        exit(0);
    }
    
    //��ȡ�������ַ
    if (GetShmAddress(&g_pShmTable,lShmKey)<0 ){
        userlog("\n    server_client start error when tpsvrinit, getshm err shmkey:%d\n"
                "    ��ȡ������ʧ��,����������ػ�����δ����! �����ܼ���!",lShmKey);
        g_pShmTable = NULL;
        exit(0);
    }
    userlog("\n    server_client started.");
    
    return 0;
}


//######################################################################

/***********************************************************************
    ����: CLIENT_LOGIN
    ���ܣ�Զ�̼�ؿͻ������������ʱ����
    ���������
        iHostID(�ͻ�������ID, ��Ӧm_wf_hostinfo.HOST_ID) 
          --�ͻ����ػ���������ʱ����Ĭ�ϸ���hostnameȡID
        sSysUserName(Unix �û���)
    ����ֵ��
        iLogID(��¼ID; -1��¼ʧ��)
        sMsg(������Ϣ. ��iLogID == -1ʱ��Ч)
----------------------------------------------------------------------*/
void CLIENT_LOGIN(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);
    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    int iRet = ParaList.parseBuff(sQqstBuf);
    if (!iRet){
        ParaList.reset();
        ParaList.setLogID(-1); //����������Ϸ�,��¼ʧ��
        ParaList.addMsg("��¼������ػ�ʱ����������Ϸ���");
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock the signal.
    LockSafe(g_pShmTable->m_iClientLock,0);
    g_pShmTable->ClientReqInfo.m_iHostID = ParaList.getHostID();
    ParaList.getSysUserName(g_pShmTable->ClientReqInfo.m_sSysUserName);
    g_pShmTable->ClientReqInfo.m_iStatus = REQ_CLIENT_LOGIN;
    
    ParaList.reset();
    int iTimeOut = 0;
    while(g_pShmTable->ClientReqInfo.m_iStatus == REQ_CLIENT_LOGIN)
    { //wait...
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) {
			//8��13�ռ�
			//g_pShmTable->ClientReqInfo.m_iStatus = WF_WAIT;
            ParaList.setLogID(-1); //time out,failed
            ParaList.addMsg("��¼����س�ʱ��");
            ParaList.getBuiltStr(sQqstBuf);
            UnlockSafe(g_pShmTable->m_iClientLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }
    
    //���ػ����̴�����״̬������: 
    //SERVER_ACCEPT MAX_CLIENT_NUM SERVER_REJECT
    if (g_pShmTable->ClientReqInfo.m_iStatus == SERVER_ACCEPT){
        ParaList.setLogID(g_pShmTable->ClientReqInfo.m_iIndex);
    }
    else if (g_pShmTable->ClientReqInfo.m_iStatus == MAX_CLIENT_NUM)
    {
        ParaList.setLogID(-1);
        ParaList.addMsg("Զ���ػ�������Ӵﵽ������ƣ�");
    }
    else {
        ParaList.setLogID(-1);
        ParaList.addMsg(g_pShmTable->ClientReqInfo.m_sMsgBuf);
    }
    ParaList.getBuiltStr(sQqstBuf);
    UnlockSafe(g_pShmTable->m_iClientLock,0);
    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
}


/***********************************************************************
    ����: CLIENT_LOGOUT
    ���ܣ�Զ�̼�ؿͻ������������ʱ����
    ���������
        iLogID(��¼ID)
        iHostID(����ID)
        sSysUserName(Unix �û���)
    ����ֵ��
        iResult(1�ɹ�; 0ʧ��)
----------------------------------------------------------------------*/
void CLIENT_LOGOUT(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);
    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    int iRet = ParaList.parseBuff(sQqstBuf);
    if (!iRet){
        ParaList.reset();
        ParaList.setResult(0); //����������Ϸ�,ע����¼ʧ��
        ParaList.addMsg("����������Ϸ���");
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock the signal.
    LockSafe(g_pShmTable->m_iClientLock,0);
    g_pShmTable->ClientReqInfo.m_iHostID = ParaList.getHostID();
    g_pShmTable->ClientReqInfo.m_iIndex = ParaList.getLogID();
    ParaList.getSysUserName(g_pShmTable->ClientReqInfo.m_sSysUserName);
    g_pShmTable->ClientReqInfo.m_iStatus = REQ_CLIENT_LOGOUT;
    
    ParaList.reset();
    int iTimeOut = 0;
    while(g_pShmTable->ClientReqInfo.m_iStatus == REQ_CLIENT_LOGOUT)
    { //wait...
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            ParaList.setResult(0); //time out,failed
            ParaList.addMsg("ע���˳�����س�ʱ��");
            ParaList.getBuiltStr(sQqstBuf);            
            UnlockSafe(g_pShmTable->m_iClientLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }
    
    //���ػ����̴�����״̬������: 
    //SERVER_ACCEPT SERVER_REJECT
    if (g_pShmTable->ClientReqInfo.m_iStatus == SERVER_ACCEPT){
        ParaList.setResult(1);
    }
    else {
        ParaList.setResult(0);
        ParaList.addMsg("ע���˳������ʧ�ܣ�");
    }
    ParaList.getBuiltStr(sQqstBuf);
    UnlockSafe(g_pShmTable->m_iClientLock,0);
    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
}


/***********************************************************************
    ����: GET_COMMAND
    ���ܣ�Զ�̼�ؿͻ��˻�ȡ����Ҫִ�е�ָ��ʱ����
    ���������
        iLogID(��¼ID)
        iHostID(����ID)
    ����ֵ��
        iResult    (0-ʧ��, 1-�в���ָ��, 2-��ǰ��ָ��)
        iOperation (1-start, 2-stop, 3-ChangeStatus, 4-Refresh)
        iProcessID (ָ������Ľ��̺�)
        iStaffID
        iPlanID
        cState
        sMsg (iResult=0ʱ���ʧ����Ϣ; )
----------------------------------------------------------------------*/
void GET_COMMAND(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);
		
    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    DEFINE_QUERY(qry);
    qry.commit();
		qry.close();
		
    int iRet = ParaList.parseBuff(sQqstBuf);
    if (!iRet){
        ParaList.reset();
        ParaList.setResult(0);
        ParaList.addMsg("����������Ϸ���");
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock signal
    LockSafe(g_pShmTable->m_iClientLock,0);
    g_pShmTable->ClientReqInfo.m_iHostID = ParaList.getHostID();
    g_pShmTable->ClientReqInfo.m_iIndex = ParaList.getLogID();
    g_pShmTable->ClientReqInfo.m_iStatus = REQ_GET_COMMAND;
    
    ParaList.reset();
    int iTimeOut = 0;
    while(g_pShmTable->ClientReqInfo.m_iStatus == REQ_GET_COMMAND)
    { //wait...
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            ParaList.setResult(0); //time out
            ParaList.addMsg("��ȡָ�ʱ��");            
            ParaList.getBuiltStr(sQqstBuf);            
            UnlockSafe(g_pShmTable->m_iClientLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);               
        }
    }
    
    //���ػ����̴�����״̬������:
    //REQ_COMMAND_RECV CLIENT_LOG_DEAD REQ_NO_COMMAND
    if (g_pShmTable->ClientReqInfo.m_iStatus == REQ_COMMAND_RECV){
        ParaList.setResult(1);
        ParaList.setOperation(g_pShmTable->ClientReqInfo.m_iOperation);
        ParaList.setProcessID(g_pShmTable->ClientReqInfo.m_iProcessID);
        ParaList.setStaffID(g_pShmTable->ClientReqInfo.m_iStaffID);
        ParaList.setPlanID(g_pShmTable->ClientReqInfo.m_iPlanID);
        ParaList.setState(g_pShmTable->ClientReqInfo.m_cState);
    }
    else if (g_pShmTable->ClientReqInfo.m_iStatus == CLIENT_LOG_DEAD){
        ParaList.setResult(0);
        ParaList.addMsg("Err: CLIENT_LOG_DEAD");
    }
    else if (g_pShmTable->ClientReqInfo.m_iStatus == REQ_NO_COMMAND){
        ParaList.setResult(2);
    }
    else {
        ParaList.setResult(0);
        ParaList.addMsg("Err: Unknown reason.");
    }
    ParaList.getBuiltStr(sQqstBuf);
    UnlockSafe(g_pShmTable->m_iClientLock,0);
    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
}


/***********************************************************************
    ����: ACK_COMMAND
    ���ܣ�����ָ��ִ�к�Ľ����Ϣ(��GET_COMMAND�����)
    ���������
        iLogID(��¼ID)
        iHostID(����ID)
        iProcessID(ָ������Ľ���ID)
        iOperation(ָ���) (1-start, 2-stop, 3-ChangeStatus, 4-Refresh)
        iResult(0-ʧ��, 1-�ɹ�) ָ���ִ�����
        sMsg(ָ��ִ�з��ص��ı���Ϣ)
    ����ֵ��
        iResult(0-ʧ��, 1-�ɹ�) ���׵�ִ�����
        //iMainGuardIndex(��ָ��Ϊ����ָ��ʱ,��ʶԶ��Ӧ�ý�����������ػ�����AppInfo�е��±�)
        sMsg (ʧ��ԭ��)
----------------------------------------------------------------------*/
void ACK_COMMAND(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);
    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    int iRet = ParaList.parseBuff(sQqstBuf);
    if (!iRet){
        ParaList.reset();
        ParaList.setResult(0); //����������Ϸ�
        ParaList.addMsg("����������Ϸ���");
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock the signal.
    LockSafe(g_pShmTable->m_iClientLock,0);
    g_pShmTable->ClientReqInfo.m_iHostID = ParaList.getHostID();
    g_pShmTable->ClientReqInfo.m_iIndex = ParaList.getLogID();
    g_pShmTable->ClientReqInfo.m_iProcessID = ParaList.getProcessID();
    g_pShmTable->ClientReqInfo.m_iResult = ParaList.getResult();
    g_pShmTable->ClientReqInfo.m_iOperation = ParaList.getOperation();
    strcpy(g_pShmTable->ClientReqInfo.m_sMsgBuf,ParaList.getMsg());
    
    g_pShmTable->ClientReqInfo.m_iStatus = REQ_ACK_COMMAND;
    
    ParaList.reset();
    int iTimeOut = 0;
    while(g_pShmTable->ClientReqInfo.m_iStatus == REQ_ACK_COMMAND)
    { //wait...
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            ParaList.setResult(0); //time out,failed
            ParaList.addMsg("����ָ��ִ�н����ʱ��");            
            ParaList.getBuiltStr(sQqstBuf);            
            UnlockSafe(g_pShmTable->m_iClientLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }
    
    //���ػ����̴�����״̬������:
    // CLIENT_LOG_DEAD ACK_COMMAND_END
    if (g_pShmTable->ClientReqInfo.m_iStatus == ACK_COMMAND_END)
    {
        ParaList.setResult(1);
        //ParaList.setMainGuardIndex(g_pShmTable->ClientReqInfo.m_iMainGuardIndex);
        //ParaList.addMsg("�ѻ�ȡ��iMainGuardIndex");
    }
    else if (g_pShmTable->ClientReqInfo.m_iStatus == CLIENT_LOG_DEAD) {
        ParaList.setResult(0);
        ParaList.addMsg("client log dead.");
    }
    else {
        ParaList.setResult(0);
        ParaList.addMsg("Failed. Unknown reason.");
    }
    ParaList.getBuiltStr(sQqstBuf);
    
    TWfLog::log(0,"server_client send ACK_COMMAND_END msg:%s",sQqstBuf);
    
    UnlockSafe(g_pShmTable->m_iClientLock,0);
    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
}


/***********************************************************************
    ����: SEND_REMOTEMSG
    ���ܣ�Զ�̼�ط���һ��Ӧ�ó���״̬�������Ϣ�������
    ���������
        iLogID(��¼ID)
        iHostID(����ID)
        iProecessID
        iProcessCnt
        cState(����״̬)
        iMainGuardIndex
        <sMsg=str_msg>...
    ����ֵ��
        iResult(0-ʧ��, 1-�ɹ�)
----------------------------------------------------------------------*/
void SEND_REMOTEMSG(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);
    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    int iRet = ParaList.parseBuff(sQqstBuf);
    if (!iRet){
        ParaList.reset();
        ParaList.setResult(0); //����������Ϸ�
        ParaList.addMsg("����������Ϸ���");
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock the signal.
    LockSafe(g_pShmTable->m_iClientLock,0);
    g_pShmTable->ClientReqInfo.m_iHostID = ParaList.getHostID();
    g_pShmTable->ClientReqInfo.m_iIndex = ParaList.getLogID();
    //g_pShmTable->ClientReqInfo.m_cState = ParaList.getState();
    //strcpy(g_pShmTable->ClientReqInfo.m_sMsgBuf,ParaList.getBuiltMsg());
    strcpy(g_pShmTable->ClientReqInfo.m_sMsgBuf,sQqstBuf);
    
    g_pShmTable->ClientReqInfo.m_iStatus = REQ_SEND_REMOTEMSG;

    ParaList.reset();
    int iTimeOut = 0;    
    while(g_pShmTable->ClientReqInfo.m_iStatus == REQ_SEND_REMOTEMSG)
    { //wait...
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            ParaList.setResult(0); //time out,failed
            ParaList.addMsg("�����Ϣ��ʱ��");            
            ParaList.getBuiltStr(sQqstBuf);            
            UnlockSafe(g_pShmTable->m_iClientLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }
    
    //���ػ����̴�����״̬������:
    // CLIENT_LOG_DEAD REMOTEMSG_SENT
    if (g_pShmTable->ClientReqInfo.m_iStatus == REMOTEMSG_SENT)
    {
        ParaList.setResult(1);
    }
    else if (g_pShmTable->ClientReqInfo.m_iStatus == CLIENT_LOG_DEAD) {
        ParaList.setResult(0);
        ParaList.addMsg("client log dead.");
    }
    else {
        ParaList.setResult(0);
        ParaList.addMsg("Failed. Unknown reason.");
    }
    ParaList.getBuiltStr(sQqstBuf);
    UnlockSafe(g_pShmTable->m_iClientLock,0);
    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);    
}

//######################################################################
} //end extern "C"

