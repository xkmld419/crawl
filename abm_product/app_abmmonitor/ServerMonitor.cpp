/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "Server.h"

static const int TIME_OUT = 500;

static TSHMCMA *g_pShmTable = NULL;


extern "C" {

int tpsvrinit(int argc, char **argv)
{
    userlog("\n    server_monitor starting...");
    ABMException oExp;
    MonitorInfo oIPCMonitor;
    long lShmKey = oIPCMonitor.getInfoIpc(oExp,"SHM_WorkFlow");
    if (lShmKey < 0) {
        userlog("\n    server_monitor start error when tpsvrinit, getshm err shmkey\n"
                "    ��ȡ�����ڴ�KEYʧ��! ���ܱ�m_b_ipc_cfgδ����, �˳�!");
        exit(0);
    }
    
    //��ȡ�������ַ
    if (GetShmAddress(&g_pShmTable,lShmKey)<0 ){
        userlog("\n    server_monitor start error when tpsvrinit, getshm err shmkey:%d\n"
                "    ���ӹ����ڴ�ʧ��! �����ܼ���! �˳�!",lShmKey);
        g_pShmTable = NULL;
        exit(0);
    }
    userlog("\n    server_monitor started.");
    Environment::getDBConn()->commit();
    return 0;
}

//######################################################################

/***********************************************************************
    ����: MONITOR_LOGIN
    ���ܣ�ǰ̨���Monitor�Ʒ����̵�¼������ػ�ʱ����
    ���������
        //##iBillFlowID(�Ʒ�����ID) (�˲�����ȡ��)
        sOrgID(���Ƶ�OrgID�б�, �����ʽ <sOrgID=1|23|4|15|...|n>)
        sHostAddr(ǰ̨�������IP��ַ);
        iStaffID(ǰ̨��ص�¼Ա����);
        iLogMode(��¼ģʽ��1-�Թ۵�¼,ֻ�ܲ鿴; 2-ǿ�м��,����ִ��ָ��)
    ����ֵ��
        iResult(0-ע��ʧ��; 1-ע��ɹ�; 2-�������Ϸ�)
        iLogID(��¼ID)
        iAuthID(�����֤ID)
----------------------------------------------------------------------*/
void MONITOR_LOGIN(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);
    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    int iRet = ParaList.parseBuff(sQqstBuf);
    
    if (!iRet || ParaList.getStaffID()<0 ||    ParaList.getLogMode()<0 ){
        ParaList.reset();
        ParaList.setResult(2); //����������Ϸ�
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock the signal.
    LockSafe(g_pShmTable->m_iMonitorLock,0);
    
    g_pShmTable->MntReqInfo.m_iBillFlowID = ParaList.getBillFlowID();
    g_pShmTable->MntReqInfo.m_iStaffID = ParaList.getStaffID();
    g_pShmTable->MntReqInfo.m_iLogMode = ParaList.getLogMode();
    ParaList.getHostAddr(g_pShmTable->MntReqInfo.m_sHostAddr);
    ParaList.getOrgIDStr(g_pShmTable->MntReqInfo.m_sOrgIDStr);
    
    g_pShmTable->MntReqInfo.m_iStatus = MNT_REQ_REGISTER;

    int iTimeOut = 0;
   	while( g_pShmTable->MntReqInfo.m_iStatus == MNT_REQ_REGISTER)
    {//�ȴ����ػ����̴���...
				Environment::getDBConn()->commit();
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            g_pShmTable->MntReqInfo.m_iStatus = WF_WAIT;
            ParaList.reset();
            ParaList.setResult(0); //time out,failed
            ParaList.getBuiltStr(sQqstBuf);
            
            UnlockSafe(g_pShmTable->m_iMonitorLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }

    ParaList.reset();
    
    //���ػ����̴�����״̬������: REG_REJECT_MAXNUM,REG_ACCEPT
    if (g_pShmTable->MntReqInfo.m_iStatus == REG_ACCEPT) {
        ParaList.setResult(1);
        ParaList.setLogID(g_pShmTable->MntReqInfo.m_iIndex);
        ParaList.setAuthID(g_pShmTable->MntReqInfo.m_iAuthID);
    }
    else {
        ParaList.setResult(0);
    }
    
    ParaList.getBuiltStr(sQqstBuf);

    //unlock signal.
    UnlockSafe(g_pShmTable->m_iMonitorLock,0);

    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
}


/***********************************************************************
    ����: MONITOR_LOGOUT
    ���ܣ�ǰ̨���Monitor�ļƷ������˳����ʱ����
    ���������
        iLogID(��¼ID)
        iAuthID(�����֤ID)
    ����ֵ��
        iResult(0-ʧ��; 1-�ɹ�; 2-�������Ϸ�; 3-�����Ӳ�����)
----------------------------------------------------------------------*/
void MONITOR_LOGOUT(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);

    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);

    
    int iRet = ParaList.parseBuff(sQqstBuf);
    
    if (!iRet || ParaList.getLogID()<0 || ParaList.getAuthID()<0){
        ParaList.reset();
        ParaList.setResult(2); //����������Ϸ�
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock the signal.
    LockSafe(g_pShmTable->m_iMonitorLock,0);

    g_pShmTable->MntReqInfo.m_iIndex = ParaList.getLogID();
    g_pShmTable->MntReqInfo.m_iAuthID = ParaList.getAuthID();
    g_pShmTable->MntReqInfo.m_iStatus = MNT_REQ_UNREGISTER;
    
    int iTimeOut = 0;
    while (g_pShmTable->MntReqInfo.m_iStatus == MNT_REQ_UNREGISTER)
    {//�ȴ����ػ����̴���...
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            g_pShmTable->MntReqInfo.m_iStatus = WF_WAIT;
            ParaList.reset();
            ParaList.setResult(0); //time out,failed
            ParaList.getBuiltStr(sQqstBuf);
            
            UnlockSafe(g_pShmTable->m_iMonitorLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }

    ParaList.reset();
    
    //���ػ����̴�����״̬������: MNT_LOG_DEAD,REG_UNREGISTERED
    if (g_pShmTable->MntReqInfo.m_iStatus == REG_UNREGISTERED ) {
        ParaList.setResult(1); //success.
    }
    else {
        ParaList.setResult(3); //connection dead.
    }

    ParaList.getBuiltStr(sQqstBuf);

    //unlock signal.
    UnlockSafe(g_pShmTable->m_iMonitorLock,0);

    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    
}


/***********************************************************************
    ����: RECV_APPINFO
    ���ܣ�ǰ̨���Monitor���յ�ǰ�Ʒ����̶�Ӧ��Ӧ�ý��������Ϣ
        (ÿ��ֻ���һ��process����Ϣ,����״̬��Ϣ/���������Ϣ/������)
    ���������
        iLogID(��¼ID)
        iAuthID(�����֤ID)
    ����ֵ��
      iResult(0-ʧ��; 1-�ɹ�; 2-�������Ϸ�; 3-�����Ӳ�����)
      iProcessID(����ID)
      cState(����״̬)
      iProcessCnt(���̴���������)  
        ps:iProcessCnt�ں�̨���޷��ų����͵�ֵ,���ﵽ���ֵ��,���Զ���0
      sMsg[����](���������Ϣ)����ʽΪ:<sMsg=s1><sMsg=s2>...<sMsg=sN>
          ps:(û����ϢʱΪsMsg��)

Mod 2005/06/13
RECV_APPINFO ���صĸ�ʽ��
��ʽ������(��һ�� ֻ��iResult,��ͬ�ڵ�iProcessID������ͬ)
<iResult=1>$<iProcessID=11><...>$<iProcessID=12><...>$<iProcessID=11><...>
----------------------------------------------------------------------*/
void RECV_APPINFO(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);

    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    int iRet = ParaList.parseBuff(sQqstBuf);
    
    if (!iRet || ParaList.getLogID()<0 || ParaList.getAuthID()<0)
    {
        ParaList.reset();
        ParaList.setResult(2); //����������Ϸ�
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock the signal.
    LockSafe(g_pShmTable->m_iMonitorLock,0);
    
    g_pShmTable->MntReqInfo.m_iIndex = ParaList.getLogID();
    g_pShmTable->MntReqInfo.m_iAuthID = ParaList.getAuthID();
    g_pShmTable->MntReqInfo.m_iStatus = MNT_REQ_APPINFO;
    
    int iTimeOut = 0;
    while( g_pShmTable->MntReqInfo.m_iStatus == MNT_REQ_APPINFO) 
    {//�ȴ����ػ����̴���...
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            g_pShmTable->MntReqInfo.m_iStatus = WF_WAIT;
            ParaList.reset();
            ParaList.setResult(0); //time out,failed
            ParaList.getBuiltStr(sQqstBuf);
            
            UnlockSafe(g_pShmTable->m_iMonitorLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }

    //���ػ����̴�����״̬������: MNT_LOG_DEAD,REQ_APPINFO_FILLED
    if (g_pShmTable->MntReqInfo.m_iStatus == REQ_APPINFO_FILLED) {
        //ParaList.parseBuff(g_pShmTable->MntReqInfo.m_sMsgBuf);
        //ParaList.setResult(1); //success.    
        sprintf(sQqstBuf,"<iResult=1>%s",g_pShmTable->MntReqInfo.m_sMsgBuf);
    }
    else {
        ParaList.setResult(3); //connection dead.
        ParaList.getBuiltStr(sQqstBuf);
    }

    //ParaList.getBuiltStr(sQqstBuf);

    //unlock signal.
    UnlockSafe(g_pShmTable->m_iMonitorLock,0);
    
    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
}


void RECV_PASSINFO(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);

    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    int iRet = ParaList.parseBuff(sQqstBuf);
    
    if (!iRet || ParaList.getLogID()<0 || ParaList.getAuthID()<0)
    {
        ParaList.reset();
        ParaList.setResult(2); //JdHk2NJ}2;:O7(
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
	LockSafe(g_pShmTable->m_iMonitorLock,0);
    
    g_pShmTable->MntReqInfo.m_iIndex = ParaList.getLogID();        g_pShmTable->MntReqInfo.m_iAuthID = ParaList.getAuthID();
    g_pShmTable->MntReqInfo.m_iMqID = ParaList.getMqId();        g_pShmTable->MntReqInfo.m_iFlag = ParaList.getFlag();	 g_pShmTable->MntReqInfo.m_iProcessID = ParaList.getProcessID();        g_pShmTable->MntReqInfo.m_iStatus = MNT_REQ_PASSINFO;
    int iTimeOut =0;
     while( g_pShmTable->MntReqInfo.m_iStatus == MNT_REQ_PASSINFO) 
    {
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            g_pShmTable->MntReqInfo.m_iStatus = WF_WAIT;
            ParaList.reset();
            ParaList.setResult(0); //time out,failed
            ParaList.getBuiltStr(sQqstBuf);
            
            UnlockSafe(g_pShmTable->m_iMonitorLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }
    if (g_pShmTable->MntReqInfo.m_iStatus == REQ_PASSINFO_FILLED) {
        sprintf(sQqstBuf,"<iResult=1>$%s",g_pShmTable->MntReqInfo.m_sMsgBuf);
    }
    else
    {if (g_pShmTable->MntReqInfo.m_iStatus == NO_PASSINFO)
       {
       ParaList.setResult(3);
       ParaList.addMsg("NO PASSINFO");
       ParaList.getBuiltStr(sQqstBuf);
       
    }
    else {
        ParaList.setResult(3); //connection dead.
        ParaList.getBuiltStr(sQqstBuf);
    }
   } 
    //ParaList.getBuiltStr(sQqstBuf);

    //unlock signal.
    UnlockSafe(g_pShmTable->m_iMonitorLock,0);
    
    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
}


/***********************************************************************
    ����: SEND_COMMAND
    ����: ǰ̨���Monitor����ִ��ָ��(����/ֹͣ��̨Ӧ�ó���)ʱ����
    �������:
        iLogID(��¼ID)
        iAuthID(�����֤ID)
        iProcessID(��Ӧm_wf_process.PROCESS_ID)
        iOperation(���� 1-����; 2-ֹͣ)
    ����ֵ:
        iResult(0-ʧ��; 1-�ɹ�; 2-�������Ϸ�; 3-�����Ӳ�����, 4-�����Ѿ�����)
        sMsg( iRuesult = 0 ʱ��ʧ����Ϣ)
----------------------------------------------------------------------*/
void SEND_COMMAND(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);

    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    int iRet = ParaList.parseBuff(sQqstBuf);
    
    if (!iRet || ParaList.getLogID()<0 || ParaList.getAuthID()<0){
        ParaList.reset();
        ParaList.setResult(2); //����������Ϸ�
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock the signal.
    LockSafe(g_pShmTable->m_iMonitorLock,0);
    
    g_pShmTable->MntReqInfo.m_iIndex = ParaList.getLogID();
    g_pShmTable->MntReqInfo.m_iAuthID = ParaList.getAuthID();
    g_pShmTable->MntReqInfo.m_iProcessID = ParaList.getProcessID();
    g_pShmTable->MntReqInfo.m_iOperation = ParaList.getOperation();
    strcpy( g_pShmTable->MntReqInfo.m_sMsgBuf,sQqstBuf);
    
    g_pShmTable->MntReqInfo.m_iStatus = REQ_SEND_COMMAND; //����ִ��ָ��
    
    int iTimeOut = 0;
    while (g_pShmTable->MntReqInfo.m_iStatus == REQ_SEND_COMMAND ||
        g_pShmTable->MntReqInfo.m_iStatus == REQ_COMMAND_SENDING)
    {//waiting...
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            g_pShmTable->MntReqInfo.m_iStatus = WF_WAIT;
            ParaList.reset();
            ParaList.setResult(0); //time out,failed
            if (g_pShmTable->MntReqInfo.m_iStatus == REQ_SEND_COMMAND)
                ParaList.addMsg("�����δ��Ӧ");
            else
                ParaList.addMsg("ָ��͸�Զ�̼���쳣");
            ParaList.getBuiltStr(sQqstBuf);
            
            UnlockSafe(g_pShmTable->m_iMonitorLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }
    
    ParaList.reset();
    
    //���ػ����̴�����״̬������: 
    //MNT_LOG_DEAD,NO_PRIVILEGE,REG_APP_MAXNUM,
    //REQ_COMMAND_SENT,COMMAND_EXEC_ERR,REMOTECMA_NOT_START
    if (g_pShmTable->MntReqInfo.m_iStatus == MNT_LOG_DEAD) 
        ParaList.setResult(3); //connection dead.
    else if (g_pShmTable->MntReqInfo.m_iStatus == REQ_COMMAND_SENT)
        ParaList.setResult(1); //success.
    else if (g_pShmTable->MntReqInfo.m_iStatus == NO_PRIVILEGE) {
        ParaList.setResult(0);
        ParaList.addMsg("�Բ�������ִ�и�ָ���Ȩ��");
    }
    else if (g_pShmTable->MntReqInfo.m_iStatus == REG_APP_MAXNUM) {
        ParaList.setResult(0);
        ParaList.addMsg("�Բ��𣬺�ִ̨�е�Ӧ�ý��̴ﵽϵͳ������������");
    }
    else if (g_pShmTable->MntReqInfo.m_iStatus == REMOTECMA_NOT_START) {
        char sTemp[1024];
        ParaList.setResult(0);
        sprintf (sTemp," Զ���ػ�����GuardRemote��δ����!\n"
            "\nAction: ������Ӧ�������û�[%s]��ִ�� 'rmnt -b'\n",
            g_pShmTable->MntReqInfo.m_sMsgBuf
        );
        ParaList.addMsg(sTemp);        
    }
    else {
        ParaList.setResult(0);
        ParaList.addMsg("ָ��ִ��ʧ��");
        ParaList.addMsg(g_pShmTable->MntReqInfo.m_sMsgBuf);
    }
    
    ParaList.getBuiltStr(sQqstBuf);

    //unlock signal.
    UnlockSafe(g_pShmTable->m_iMonitorLock,0);

    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
}


/***********************************************************************
    ����: CHANGE_STATUS
    ����: ǰ̨���Monitor����ǿ�иı���̽ڵ�״ָ̬��ʱ����
    �������:
        iLogID(��¼ID)
        iAuthID(�����֤ID)
        iProcessID(��Ӧm_wf_process.PROCESS_ID)
        cState(���ĵ�Ŀ��״̬) (A-δ���� R-������ E-���н��� X-�쳣 K-ǰ̨���Ƚ��淢��ֹͣ����)
    ����ֵ:
        iResult(0-ʧ��; 1-�ɹ�; 2-�������Ϸ�; 3-�����Ӳ�����)
----------------------------------------------------------------------*/
void CHANGE_STATUS(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);

    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    int iRet = ParaList.parseBuff(sQqstBuf);
    char cState = ParaList.getState();
    if (!iRet || ParaList.getLogID()<0 || ParaList.getAuthID()<0
        || ( cState!='A' && cState!='R' && cState!='E' )
    )
    {
        ParaList.reset();
        ParaList.setResult(2); //����������Ϸ�
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock the signal.
    LockSafe(g_pShmTable->m_iMonitorLock,0);
    
    g_pShmTable->MntReqInfo.m_iIndex = ParaList.getLogID();
    g_pShmTable->MntReqInfo.m_iAuthID = ParaList.getAuthID();
    g_pShmTable->MntReqInfo.m_iProcessID = ParaList.getProcessID();
    g_pShmTable->MntReqInfo.m_cState = ParaList.getState();
    //strcpy( g_pShmTable->MntReqInfo.m_sMsgBuf, sQqstBuf);
    sprintf (g_pShmTable->MntReqInfo.m_sMsgBuf,"<iOperation=3>%s",sQqstBuf);
    
    g_pShmTable->MntReqInfo.m_iStatus = REQ_CHANGESTATUS; //����ִ��ָ��
    
    int iTimeOut = 0;
    while (g_pShmTable->MntReqInfo.m_iStatus == REQ_CHANGESTATUS ||
        g_pShmTable->MntReqInfo.m_iStatus == REQ_STATUS_CHANGEING)
    {//waiting...
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            g_pShmTable->MntReqInfo.m_iStatus = WF_WAIT;
            ParaList.reset();
            ParaList.setResult(0); //time out,failed
            ParaList.getBuiltStr(sQqstBuf);
            
            UnlockSafe(g_pShmTable->m_iMonitorLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }
    
    ParaList.reset();
    
    //���ػ����̴�����״̬������: 
    //MNT_LOG_DEAD, NO_PRIVILEGE, REMOTECMA_NOT_START
    //REQ_CHANGESTATUS_OVER, REQ_CHANGESTATUS_ERR
    if (g_pShmTable->MntReqInfo.m_iStatus == MNT_LOG_DEAD) 
        ParaList.setResult(3); //connection dead.
    else if (g_pShmTable->MntReqInfo.m_iStatus == REQ_CHANGESTATUS_OVER)
        ParaList.setResult(1); //success.
    else if (g_pShmTable->MntReqInfo.m_iStatus == NO_PRIVILEGE) {
        ParaList.setResult(0);
        ParaList.addMsg("�Բ�������ִ�и�ָ���Ȩ��");
    }
    else if (g_pShmTable->MntReqInfo.m_iStatus == REMOTECMA_NOT_START) {
        ParaList.setResult(0);
        ParaList.addMsg("Զ���ػ�����δ����");
    }
    else {
        ParaList.setResult(0);
        ParaList.addMsg("״̬�ı�ָ��ִ��ʧ��");
    }
    
    ParaList.getBuiltStr(sQqstBuf);

    //unlock signal.
    UnlockSafe(g_pShmTable->m_iMonitorLock,0);

    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);

}


/***********************************************************************
    ����: REFRESH_RUNPLAN
    ����: ǰ̨���Monitor����ִ��ָ��(�������мƻ����)ʱ����
    �������:
        iLogID(��¼ID)
        iAuthID(�����֤ID)
    ����ֵ:
        iResult(0-ʧ��; 1-�ɹ�; 2-�������Ϸ�; 3-�����Ӳ�����)
----------------------------------------------------------------------*/
void REFRESH_RUNPLAN(TPSVCINFO *rqst)
{
    TParseBuff ParaList;
    char *sQqstBuf;
    sQqstBuf = tpalloc("STRING",NULL,MAX_CMAMSG_LEN);

    int iLen = rqst->len;
    memcpy(sQqstBuf, rqst->data, iLen);
    sQqstBuf[iLen] = 0;
    TrimHeadNull(sQqstBuf,iLen);
    
    int iRet = ParaList.parseBuff(sQqstBuf);
    
    if (!iRet || ParaList.getLogID()<0 || ParaList.getAuthID()<0){
        ParaList.reset();
        ParaList.setResult(2); //����������Ϸ�
        ParaList.getBuiltStr(sQqstBuf);
        tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
    }
    
    //lock the signal.
    LockSafe(g_pShmTable->m_iMonitorLock,0);
    
    g_pShmTable->MntReqInfo.m_iIndex = ParaList.getLogID();
    g_pShmTable->MntReqInfo.m_iAuthID = ParaList.getAuthID();
    //strcpy( g_pShmTable->MntReqInfo.m_sMsgBuf,sQqstBuf);
    
    g_pShmTable->MntReqInfo.m_iStatus = REQ_REFRESH_RUNPLAN; //����ִ��ָ��
    
    int iTimeOut = 0;
    while (g_pShmTable->MntReqInfo.m_iStatus == REQ_REFRESH_RUNPLAN ||
        g_pShmTable->MntReqInfo.m_iStatus == REFRESHING_RUNPLAN)
    {//waiting...
        usleep(USLEEP_LONG);
        if (iTimeOut++ > TIME_OUT) { 
            g_pShmTable->MntReqInfo.m_iStatus = WF_WAIT;
            ParaList.reset();
            ParaList.setResult(0); //time out,failed
            ParaList.getBuiltStr(sQqstBuf);
            
            UnlockSafe(g_pShmTable->m_iMonitorLock,0);
            tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
        }
    }
    
    ParaList.reset();
    
    //���ػ����̴�����״̬������: 
    //MNT_LOG_DEAD, NO_PRIVILEGE, RUNPLAN_REFRESHED, REFRESHED_ERR
    if (g_pShmTable->MntReqInfo.m_iStatus == MNT_LOG_DEAD) 
        ParaList.setResult(3); //connection dead.
    else if (g_pShmTable->MntReqInfo.m_iStatus == RUNPLAN_REFRESHED)
        ParaList.setResult(1); //success.
    else if (g_pShmTable->MntReqInfo.m_iStatus == NO_PRIVILEGE) {
        ParaList.setResult(0);
        ParaList.addMsg("�Բ�������ִ�и�ָ���Ȩ��");
    }
    else {
        ParaList.setResult(0);
        ParaList.addMsg("ˢ�����мƻ�ָ��ִ��ʧ��");
    }
    
    ParaList.getBuiltStr(sQqstBuf);

    //unlock signal.
    UnlockSafe(g_pShmTable->m_iMonitorLock,0);

    tpreturn(TPSUCCESS,0L,sQqstBuf,strlen(sQqstBuf),0L);
}


//######################################################################

} //--End extern "C"
