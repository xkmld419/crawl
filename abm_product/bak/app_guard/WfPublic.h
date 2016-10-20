/*VER: 2*/ 
#ifndef __WF_PUBLIC_H___
#define __WF_PUBLIC_H___

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <memory.h>
#include <iostream>
//#include <atmi.h>
//#include <fml32.h>
//#include <userlog.h>
#include <unistd.h>
#include <time.h>

#include "Exception.h"
#include "Date.h"
#include "StdEvent.h"
#include "Environment.h"
#include "IpcKey.h"
#include "ProcessInfo.h"

#undef WF_FILE
#define WF_FILE __FILE__,__LINE__

using namespace std;

const int MEMBER_NULL = -99;
const int USLEEP_LONG = 10000; //˯��ʱ�� ΢�� 
const int MAX_SQL_LEN = 2048;
const int ORGID_STR_LEN = 256;
const int MAX_ORG_CNT = 100;
const int OUT_MSG_MIN_CNT = 3;

#define PERM  0660


#define WF_WAIT            0

#define MNT_LOG_DEAD       1000 //MNTǰ̨δ��¼

#define MNT_REQ_REGISTER   1010
#define REG_ACCEPT         1011
#define REG_REJECT_MAXNUM  1012
#define MNT_REQ_UNREGISTER  1020
#define REG_UNREGISTERED    1021 //mnt log out
#define MNT_REQ_APPINFO     1030 //mnt req appinfo
#define REQ_APPINFO_FILLED  1031 //info got
#define REQ_SEND_COMMAND    1041 //mnt req exec command
#define REQ_COMMAND_SENDING 1042 //sending to rmtCma
#define REQ_COMMAND_SENT    1043 //command executed.
#define NO_PRIVILEGE        1044 //no privilege.
#define REG_APP_MAXNUM      1045 //��̨Ӧ�ý��̴ﵽ�������
#define REMOTECMA_NOT_START 1046 //rmtCMA not start.
#define COMMAND_EXEC_ERR    1047

#define REQ_CHANGESTATUS      1050
#define REQ_CHANGESTATUS_OVER 1051
#define REQ_CHANGESTATUS_ERR  1052
#define REQ_STATUS_CHANGEING  1053
#define REQ_REFRESH_RUNPLAN  1060
#define REFRESHING_RUNPLAN   1061
#define RUNPLAN_REFRESHED    1062
#define REFRESHED_ERR        1063

#define CLIENT_LOG_DEAD      2010

#define REQ_CLIENT_LOGIN     2011
#define SERVER_ACCEPT        2012
#define MAX_CLIENT_NUM       2013
#define REQ_CLIENT_LOGOUT    2014
#define REQ_GET_COMMAND      2015
#define REQ_NO_COMMAND       2016
#define REQ_COMMAND_RECV     2017 //ȡ��Զ��ָ��
#define REQ_ACK_COMMAND      2019
#define ACK_COMMAND_END      2020
#define REQ_SEND_REMOTEMSG   2021
#define REMOTEMSG_SENT       2022
#define SERVER_REJECT        2023

#define DEFAULT_BUFF_LEN  1024
#define MAX_PARSEBUFF_LEN 2048*4
#define MSG_MAXLEN        256  //TParseBuff.m_sMsg�ĵ�λ����
#define MSG_MAXCNT        100  //TParseBuff.m_sMsg������

#define LOG_AS_VIEW       0
#define LOG_AS_CONTROL    1

#define APP_REG_REGISTER      3000
#define APP_REG_UNREGISTER    3001
#define APP_REG_ACCEPT        3002
#define APP_REG_REJECT        3003
#define APP_LOG_DEAD          3004

#define SAVE_BUFF_LEN         128

#ifndef WF_LOCKOP__
#define WF_LOCKOP__
#define LockSafe(x,y)   { struct sembuf LOCKSAFE={ (y),-1,SEM_UNDO };   semop((x),&LOCKSAFE,1); }
#define UnlockSafe(x,y) { struct sembuf UNLOCKSF={ (y), 1,SEM_UNDO };   semop((x),&UNLOCKSF,1); }
#endif

extern "C" {
//##ȥ���ַ���ǰ��Ŀո��
void AllTrim(char sBuf[]);

//##ȥ�������ֽ�ǰ���NULLֵ
void TrimHeadNull(char *src,int ilen);

void replace(char * sql,char *src,char *des);

}


class TWfLog {
 public:
    static void log(char const *fmt,...);
    static void log(int iErrCode,char const *fmt,...);
    static void log(char const *sFileName, int iLine, char const *fmt,...);
    static void writeLog(char *sMsg) ;
    static void writeLog2(char *sMsg) ;
    static void ignoreTuxBuildErr() {
        //cout<<h_tmenv<<h_atmi<<h_fml32<<h_userlog<<endl;
// cout<<h_tmenv<<h_atmi<<h_fml32<<endl;
    }
 private:    
    static Date m_oDate;
};

///////////////////////////////////////////////////////////////////
/*
  ClassName: TLogProcessMgr
  Function : ��������������־����( log_process ), ���ػ����̹���
  Comment  : 
      �ػ�����ִ������ָ���,����insertLogProcess���ɼ�¼, 
      ��̨Ӧ�ý��� -- ���Ӻ�,����process_id,Ѱ���ڴ�TAppInfoλ��,����db 
      sql���      -- ִ��ָ������db�еļ�¼. 
-----------------------------------------------------------------*/
class TLogProcessMgr {
 public:
 
    //�ػ����̴����̨Ӧ�ý���ע����¼ʱ����(_iAppPid == 0)
    //�ػ����̴����̨Ӧ�ý������Ӻ����    (_iAppPid != 0)
    static void updateLogProcess(int _iProcessLogID, char _cState, int _iAppPid = 0);
    
    //�ػ�����������̨Ӧ�ý���ǰ����(Ŀǰ�ػ�����ִ�е�ָ��ȫ������,���� ֹͣ,�ı�״̬)
    static int insertLogProcess( int _iProcID, const char *sCommand,
        int iStaffID = -1,int iPlanID = -1 );
    
 private:
     static int getProcessLogID();
};


/**********************************************************
ClassName: TParseBuff
Function : ����/��ʽ�� �ڲ������ʽ�� ���̼� ͨѶ�ַ���
comment  : ��ʽ��������ɵ�λ��ʽΪ "<��ʶ������=��ֵ>"
  ��ʶ�����������--
   iBillFlowID/iProcessID/iHostID/sHostAddr/iProcessCnt/iMsgType
   iStaffID/iPlanID/iLogMode/iLogID/iAuthID/iMainGuardIndex/iResult
   iOperation(ָ��: 1-start 2-stop 3-ChangeStatus 4-Refresh)
   cState(A-δ���� R-������ E-���н��� X-�쳣 K-ǰ̨���Ƚ��淢��ֹͣ����)
   sMsg
Restrict : 
 1.ֻ�� "sMsg" ���ظ������ڸ�ʽ�����ǺϷ���
 2."sMsg" ����������ظ�����ʱ,���������ø������һ�γ��ֵ�ֵ����ǰ���ֵ
 3.�� "i" ��ͷ�Ŀ�ʶ��������Ӧ����ֵ��������ֵ�͵��ַ���,��2046(��������)
 4.�� "s" ��ͷ�Ŀ�ʶ��������Ӧ����ֵ�����������ַ���,�����ܺ�:"<",">","="
 5.������Сд����
 6."iProcessCnt" �ǵ�ǰ���̴�����¼���(�޷��ų�����),������"i"��ͷΪ����
Demo     : <iBillFlowID=3><iProcessID=2046>...<sMsg=Str_1>...<sMsg=Str_N>
---------------------------------------------------------*/
class TParseBuff{
public:
    TParseBuff(); 
    TParseBuff(char *sInBuff); //sInBuff => m_Str.

    void reset(); //��ֵ�͵ĳ�Ա������ʼ��Ϊ MEMBER_NULL, �ַ���Ϊ��
    bool parseBuff(char *sInBuff); //sInBuff => m_Str and parse it.

    int  getBillFlowID() { return  m_iBillFlowID; }
    int  getProcessID()  { return  m_iProcessID; }
    int  getHostID()     { return  m_iHostID; }
    int  getStaffID()    { return  m_iStaffID; }
    int  getPlanID()     { return  m_iPlanID; }
    int  getLogMode()    { return  m_iLogMode; } 
    int  getLogID()      { return  m_iLogID; } 
    int  getAuthID()     { return  m_iAuthID; }
    int  getOperation()  { return  m_iOperation; } 
    int  getResult()     { return m_iResult; }
    unsigned long getProcessCnt() { return m_lProcessCnt; }
    unsigned long getNormalCnt() { return m_lNormalCnt; }
    unsigned long getErrorCnt() { return m_lErrorCnt; }
    unsigned long getBlackCnt() { return m_lBlackCnt; }
    unsigned long getOtherCnt() { return m_lOtherCnt; }
    
    int  getMainGuardIndex()      { return m_iMainGuardIndex; }
    void getHostAddr(char *ip)    { if (ip) strcpy(ip,m_sHostAddr); }
    void getSysUserName(char *usr)    { if (usr) strcpy(usr,m_sSysUserName); }
    void getOrgIDStr(char *str)   { if (str) strcpy(str,m_sOrgIDStr); }
    char const *getMsg(int iOffSet = 0)  {
        if (iOffSet <= m_iMsgOff)
            return m_sMsg[iOffSet];
        else return "";        
    }
    char getState()  { return m_cState; }
    
    void setBillFlowID(int id)  { m_iBillFlowID = id;}
    void setProcessID(int id)   { m_iProcessID = id;}
    void setHostID(int id)      { m_iHostID = id;}
    void setHostAddr(char* str) {
        strncpy(m_sHostAddr,str,15); 
        m_sHostAddr[15] = 0;
        AllTrim(m_sHostAddr);
    }
    void setSysUserName(char* str) {
        strncpy(m_sSysUserName,str,31); 
        m_sSysUserName[31] = 0;
        AllTrim(m_sSysUserName);
    }
    void setOrgIDStr(char* str) {
        strncpy(m_sOrgIDStr,str,ORGID_STR_LEN);
        m_sOrgIDStr[ORGID_STR_LEN] = 0;
        AllTrim(m_sOrgIDStr);
    }
    void setLogID(int id)  { m_iLogID = id;}
    void setAuthID(int id) { m_iAuthID = id;}
    void setResult(int id) { m_iResult = id;}
    void setStaffID(int id) { m_iStaffID = id;}
    void setPlanID(int id) { m_iPlanID =  id;}
    void setOperation(int id) { m_iOperation = id;}
    void setMainGuardIndex(int id) { m_iMainGuardIndex = id;}
    void setProcessCnt(unsigned long id) {m_lProcessCnt = id;}
    void setNormalCnt(unsigned long id) {m_lNormalCnt = id;}
    void setErrorCnt(unsigned long id) {m_lErrorCnt = id;}
    void setBlackCnt(unsigned long id) {m_lBlackCnt = id;}
    void setOtherCnt(unsigned long id) {m_lOtherCnt = id;}
    
    void setState(char _cState) { m_cState = _cState; }
    
    void addMsg(const char *msg); //ѹ���ı���Ϣ��m_sMsg
    bool getBuiltStr(char *sOutBuff); //��ȡm_Str
    char *getBuiltMsg(); //���m_sMsg <sMsg=...>...<sMsg=...> 

    
private:
    bool parse(); //if (m_str) parse it.
    void formatBuff(); //�� m_sMsg �����еķָ����滻��
    void parseResultSave(char *pAtt, char *pValue);
    bool build(); //���ڲ���Ա����build���ַ��� => m_Str
    
    unsigned long stringToUL (char *pValue);
    
    char m_Str[MAX_PARSEBUFF_LEN+1]; //��ŷ�װ�õ��ַ���
    
    int  m_iBillFlowID;
    int  m_iProcessID;
    unsigned long m_lProcessCnt;
    unsigned long m_lNormalCnt;
    unsigned long m_lErrorCnt;
    unsigned long m_lBlackCnt;
    unsigned long m_lOtherCnt;
    int  m_iHostID;
    char m_sHostAddr[16];
    char m_sSysUserName[32];
    int  m_iStaffID;
    int  m_iPlanID;
    int  m_iLogMode;
    int  m_iLogID;
    int  m_iAuthID;
    int  m_iMainGuardIndex;
    int  m_iOperation; // 1-start 2-stop  3-ChangeStatus 4-Refresh
    char m_cState;     //CHANGE_APPSTATUS (A-δ���� R-������ E-���н��� X-�쳣 K-ǰ̨���Ƚ��淢��ֹͣ����))
    int  m_iResult;    //MONITOR_LOGIN
    char m_sOrgIDStr[ORGID_STR_LEN+1];
    char m_sMsg[MSG_MAXCNT][MSG_MAXLEN+1]; //Ӧ���ı���Ϣ
    int  m_iMsgOff;    //��Ϣ����m_sMsg��������Ϣ�洢ƫ����
};


//############################################################
//  ������ṹ����
class TMntReqInfo{
 public:
    int   m_iStatus; 
    int   m_iRegNum;                 // ��¼�ļƷ�������
    int   m_iBillFlowID;             // iBillFlowID: �Ʒ�����ID
    int   m_iLogMode;                // ��¼ģʽ�� LOG_AS_VIEW  LOG_AS_CONTROL
    int   m_iStaffID;    
    char  m_sHostAddr[15];
    int   m_iIndex;                     // ǰ̨Monitor��ؽ���ID
    int   m_igAuthID;                // ���̼�������֤ID  ��ʼ��Ϊ0
    int   m_iAuthID;                 // ���ID,��Ϣ����ʱ��
    int   m_iProcessID;
    int   m_iOperation;
    char  m_cState;
    char  m_sOrgIDStr[ORGID_STR_LEN+1];
    char  m_sMsgBuf[MAX_CMAMSG_LEN]; // ��Ϣ��������,��������ش�Ŵ�������Ϣ ...
    TMntReqInfo() { m_iRegNum = 0; }
};

class  TClientReqInfo{
 public:
    int  m_iStatus;
    char m_cState;      // ����״̬
    int  m_iRegNum;     // the Client Number logged
    int  m_iHostID; 
    char m_sSysUserName[32];
    int  m_iIndex;      // Client-key
    int  m_iProcessID;
    int  m_iStaffID;
    int  m_iPlanID;
    int  m_iResult;
    int  m_iOperation;              //(1-start, 2-stop, 3-ChangeStatus, 4-Refresh)
    int  m_iMainGuardIndex;         //Զ��Ӧ�ý�����������ػ�����AppInfo�е��±�
    char m_sMsgBuf[MAX_OUTMSG_LEN];
    TClientReqInfo() { m_iRegNum = 0; }
};

class TAppReqInfo{
 public:
    int       m_iStatus;    // APP_REG_REGISTER,APP_REG_UNREGISTER,APP_REG_ACCEPT,APP_REG_REJECT
    int       m_iProcessID; // WF_PROCESS.PROCESS_ID
    int       m_iAppPID;    // ��̨Ӧ�ý���PID
    int       m_iIndex;     // Ӧ�� ConnectMonitorʱָ����λ��
    char      m_cState;
    int       m_iSaveBufIdx;
};


//## ����ÿ����������ʵ��
//## �ػ�������ִ������ָ��ʱ,���ɼ�¼,������wf_log_process��¼, ���� process_log_id;
//## 
class TAppInfo {

 friend class TSHMCMA;

 public:
    int        m_iProcessLogID; // ��Ӧ WF_LOG_PROCESS.process_log_id
    int        m_iProcessID;    // -1ʱ,��ʾδʹ�ù�
    int        m_iMainGuardIndex; 
                         // ��Զ�̼������ʹ�ã���ʶԶ��Ӧ�ý����� ������ػ�����AppInfo�е��±�
    int        m_iBillFlowID;   // �Ʒ�����ID
    char       m_cState; //Ӧ�ý���״̬(������״̬�ı�ʱά��WF_LOG_PROCESS ��)
                         //A-δ���� B-������ R-������ E-���н��� X-�쳣 K-ǰ̨���Ƚ��淢��ֹͣ����
    int        m_iAppPID;
    int	 	m_iAppType;
    unsigned long m_lProcessCnt;
    unsigned long m_lNormalCnt;
    unsigned long m_lErrorCnt;
    unsigned long m_lBlackCnt;
    unsigned long m_lOtherCnt;
    
    time_t         m_tLatestCalcTime; //�ϴμ��㴦��Ч�ʵ�ʱ��
    unsigned long  m_lLatestProcCnt;  //�ϴμ���ʱ�Ĵ�������
    float          m_fTreatRate;      //���һ�μ�����Ĵ����ٶ�
    
    int        m_iInfoHead;
    int        m_iInfoTail;
    char       m_asMsg[INFO_Q_SIZE][MAX_APPINFO__LEN]; // Ӧ�ý��̵������Ϣ
 	int        m_iRet ;///���ӷ��ظ�petri��ֵ
 	char       m_sErrMsg[MAX_APPINFO__LEN]; ///���ӷ��ظ�petri�Ĵ�����Ϣ��

    TAppInfo() { init(); m_iProcessID = -1;}

    void init() {
        m_iProcessID = -1; bNewMsg = false; bNewState = true;
        m_cState = 'A'; m_iProcessLogID = -1; m_iMainGuardIndex = -1; 
        m_lProcessCnt = 0; m_lNormalCnt=0; m_lErrorCnt=0; m_lBlackCnt =0; m_lOtherCnt=0;
        m_iInfoHead = 0; m_iInfoTail = 0;  
        m_iRet=0; memset(m_sErrMsg,0,sizeof(m_sErrMsg) );
    }

    bool putRetInfo(int iRet, char *sRetInfo);
    bool getRetInfo(int & iRet, char *sOutInfo);
	
    void pushInfo(char *_sMsg);
    bool getInfo( char *_sMsg); //ȡ�����һ����Ϣʱ
    char getState() { bNewState = false; return m_cState; }
    bool checkNeedOutput();
    
    bool checkAlive(); //��鵱ǰ�����ں�̨�Ƿ���Ļ���(����m_iAppPID)

    void setState( char _cState) { 
        m_cState = _cState; 
        bNewMsg = true;
        bNewState = true;
        TLogProcessMgr::updateLogProcess(m_iProcessLogID, _cState);
    }
    
    void forceOutput() { bNewMsg = true; }

//����
    void getOtherProcessInfo();
    
 private:
    bool  bNewMsg;      //�Ƿ���Ҫ�������ʵ����Ϣ 
    bool  bNewState;    //״̬�Ƿ��иı��
    
};

//## ���Ӧ�ý��̹�����Ϣ()
class TAppSaveBuff {
 friend class TSHMCMA; 
 public:
    void init () 
    { 
        m_iProcessID = -1; 
        m_bExistEvent = false; 
        m_iEventState = 0;
        m_iServBlockFlag = 0;
        memset (m_lSaveValue, 0, sizeof(long)*5);
    }
    void saveEvent (StdEvent &Evt)    //## �����¼� (���̿����recieve �¼�ʱ�ѵ���)
    {
        m_oEvent = Evt;
        m_bExistEvent = true;
        m_iEventState = 0;
    }
    void setEventState (int _iState)  //## ��Ӧ�ó����ڴ����¼������б����¼��Ĵ���״̬
    { 
        m_iEventState = _iState; 
    }
    bool getEvent (StdEvent &Evt) 
    {
        if (!m_bExistEvent)
            return false;
        Evt = m_oEvent;
        return true;
    }
    
    bool getEventState (int *piState) //## �¼������ڷ���false; ���ڷ���true, *piState = ʵ��״̬
    {
        if (!m_bExistEvent)
            return false;        
        *piState = m_iEventState;
        return true;
    }
    
    void setSaveBuff (void *pt, int iLen) {
        int iAdjustLen = iLen<SAVE_BUFF_LEN ? iLen:SAVE_BUFF_LEN;
        memcpy (m_sSaveBuf, pt, iAdjustLen);
    }
    void getSaveBuff (void *pt, int iLen) {
        int iAdjustLen = iLen<SAVE_BUFF_LEN ? iLen:SAVE_BUFF_LEN;
        memcpy (pt, m_sSaveBuf, iAdjustLen);
    }
    bool setSaveValue (int iValueID, long lValue) {
        if (iValueID<1 || iValueID>5) return false;
        m_lSaveValue[iValueID-1] = lValue;
        return true;
    }
    bool getSaveValue (int iValueID, long &lValue) {
        if (iValueID<1 || iValueID>5) return false;
        lValue = m_lSaveValue[iValueID-1];
        if (lValue == 0) return false;
        return true;
    }
    
    void setServBlockFlag (int iFlag)
    {
        m_iServBlockFlag = iFlag;
    }
    int  getServBlockFlag ()
    {
        return m_iServBlockFlag;
    }
    
    int  getProcessID() { return m_iProcessID; }
    void setProcessID(int id) { m_iProcessID = id; }
    
 private:
    int  m_iProcessID;
    bool m_bExistEvent;
    int  m_iEventState; //## ����¼���ǰ�Ĵ���״̬, �����ֵ��Ӧ�ó����Լ�����. ��ʼֵ0
    StdEvent m_oEvent;
    unsigned char m_sSaveBuf[SAVE_BUFF_LEN];
    long m_lSaveValue[5];
    int  m_iServBlockFlag;
};

class TSHMCMA {
 public:
    //ǰ̨��ؽ����¼����Ϣ����������
    int            m_iMonitorLock;  // ÿ��Monitor����ʱ����
    TMntReqInfo    MntReqInfo;      // ǰ̨�����������ػ����̵���Ϣ�����ӿڻ���    

    //Ӧ�ó����¼����Ϣת���������� 
    int            m_iAppLock;      // Ӧ�ó����¼ʱConnectMonitor�������ź���
    TAppReqInfo    AppReqInfo;      // Ӧ�ó��������¼ʱ����ʱ��¼��Ϣ
    TAppInfo       AppInfo[MAX_APP_NUM];
                                    // ����ض�Ӧÿ����̨Ӧ�ý���,Զ�̼��ֻ��ӦԶ��Ӧ�ý���)
        
    //Զ�̼�ص�¼����Ϣת����������
    int            m_iClientLock;   // Զ�̼��Client������������ػ�����ʱ����
    TClientReqInfo ClientReqInfo;   // Զ�̼�� �� �������Ϣ�����ӿڻ���
    
    //## ���ÿ��Ӧ�ý����������¼�
    TAppSaveBuff   AppSaveBuff[MAX_APP_NUM];
    //------------------------------------------------------------------------------
    
    TAppInfo*  getAppInfoByProcID(int _iProcID) ;
    
    TAppSaveBuff* getAppSaveBuffByProcID(int _iProcID);
    
    int changeState(int _iProcID, char _cState) ;
    
    //��ȡһ�����е� TAppInfo, found:return 1; notFound:return 0; running:return -1;
    int getIdleAppInfo(int _iProcID, int &iIndex) ;
    
    void setMainGuardIndex(int _iProcID,int _iMainGuardIndex) ;
    
    void printShm() ;

};

#endif

