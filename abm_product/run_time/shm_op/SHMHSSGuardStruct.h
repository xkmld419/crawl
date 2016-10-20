/*
*   ���ӼƷ�2.8�汾��GUARD_MAIN �ڴ�ṹ��
*/

#ifndef __HSS_APP_GUARD_H_INCLUDED_
#define __HSS_APP_GUARD_H_INCLUDED_

#include <sys/sem.h>
#include <sys/time.h>

#define STD_EVENT_SIZE 4096

const int MAX_CMAMSG_CNT  = 32;
const int MAX_APPINFO__LEN = 256;   //����Ӧ�ý�����Ϣ����󳤶�
const int MAX_CMAMSG_LEN  = 1024; //��monitor��������󻺳�

const int MAX_OUTMSG_LEN  = 1024;  //��client��������󻺳�
const int INFO_Q_SIZE     = 100;   //Ӧ�ó��������Ϣ��������
const int ORGID_STR_LEN = 256;

#ifndef SAVE_BUFF_LEN
#define SAVE_BUFF_LEN         128
#endif

#ifndef MAX_APP_NUM
//#define MAX_APP_NUM 1024  //Ӧ�ý��̵�������,���� "LocalDefine.h" �б��ػ�����
#endif


#ifndef LockSafe
#define LockSafe(x,y)   { struct sembuf LOCKSAFE={ (y),-1,SEM_UNDO };   semop((x),&LOCKSAFE,1); }
#endif

#ifndef UnlockSafe
#define UnlockSafe(x,y) { struct sembuf UNLOCKSF={ (y), 1,SEM_UNDO };   semop((x),&UNLOCKSF,1); }
#endif

#ifndef APP_REG_REGISTER
#define APP_REG_REGISTER      3000
#endif

#ifndef APP_REG_UNREGISTER
#define APP_REG_UNREGISTER    3001
#endif

#ifndef APP_REG_ACCEPT
#define APP_REG_ACCEPT        3002
#endif

#ifndef APP_REG_REJECT
#define APP_REG_REJECT        3003
#endif

#ifndef APP_LOG_DEAD
#define APP_LOG_DEAD          3004
#endif


//-TERMINAL MONITOR

#ifndef MAX_MSG_LEN
#define MAX_MSG_LEN 10
#define MAX_MSG_NUM 10
#endif

#ifndef MAX_APP_PARAM_LEN
#define MAX_APP_PARAM_LEN 256     // Ӧ�ý������Я����������
#endif

#ifndef ST_RUNNING
#define ST_INIT        1
#define ST_RUNNING     2
#define ST_WAIT_BOOT   3
#endif

namespace __HSS {
    
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

    

 private:

    bool  bNewMsg;      //�Ƿ���Ҫ�������ʵ����Ϣ 

    bool  bNewState;    //״̬�Ƿ��иı��

    

};



//## ���Ӧ�ý��̹�����Ϣ()

class TAppSaveBuff {

 friend class TSHMCMA; 

 public:

    

 private:

    int  m_iProcessID;

    bool m_bExistEvent;

    int  m_iEventState; //## ����¼���ǰ�Ĵ���״̬, �����ֵ��Ӧ�ó����Լ�����. ��ʼֵ0

    char m_sBuf[STD_EVENT_SIZE];

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

    TAppInfo       AppInfo[1024];

                                    // ����ض�Ӧÿ����̨Ӧ�ý���,Զ�̼��ֻ��ӦԶ��Ӧ�ý���)

        

    //Զ�̼�ص�¼����Ϣת����������

    int            m_iClientLock;   // Զ�̼��Client������������ػ�����ʱ����

    TClientReqInfo ClientReqInfo;   // Զ�̼�� �� �������Ϣ�����ӿڻ���

    

    //## ���ÿ��Ӧ�ý����������¼�

    TAppSaveBuff   AppSaveBuff[1024];

    //------------------------------------------------------------------------------
};

typedef struct TMsgInfo {

    int iNewFlag;

    long lTime;

    int iMsgType;

    char sMsg[MAX_MSG_LEN];

}TMsgInfo;



typedef struct TProcessInfo 

{

    int  iProcessID;

    int  iAppID;

    int  iBillFlowID;

    int  iSysPID;

//  int  iDBUserID;

    int  iMqID; //��Ϣ����ID

    char sName[96];

    char sExecFile[256];

    char sExecPath[256];

    char sParam[MAX_APP_PARAM_LEN];

    long lLoginTime;

    int  iState;

    long iAllTickets;

    long iNormalTickets;

    long iBlackTickets;

    long iErrTickets;

    long iOtherTickets;

    long iCurMsgPos;

    long iCurErrPos;

    TMsgInfo MsgItem[MAX_MSG_NUM];

    TMsgInfo ErrItem[MAX_MSG_NUM];

    int  iFreeFlag;

    

    time_t tSaveTime;

    long   lSaveTickets;

    int iBlockFlag; //��ѹ��ʶ

    int iTreatRate;//��������Ч��

    int iIORate; //IOƵ��

    int iCPU;

    int iMEM;

    long lAllIOCnt;//��IO����

    long lLatestIOCnt;//�����ϴ�IO����

    long lLatestProcCnt;//�����ϴεĻ�����  

    time_t tLatestCalcTime;//�ϴμ����ʱ��

    char sFlowName[30];//���̵�����Ҳ����ȥ

    char sMsgInfo[100];//�����Ϣ��



    /*add by xn for hbstat

    StatInfo[0]�ǵ����ͳ�ƣ�StatInfo[1]��������������ͳ�ƣ�

    StatInfo[2]���Ե�ǰСʱ���������ۼƵ�ͳ��*/

  //  TStatInfo StatInfo[3];

    /*add by xn for schedule*/

    int iVpID;	//�鴦����ID��Ĭ��-1�򲻲���schedule����

    int iTaskNum; //xn ����������

    int iNextTaskNum; //xn���ζ����������ֵ

    int iNextVpID;//����vp 



	int iPetriState;

	int iLastState;/////���̵��ϴε�״̬

       

    /*add by xn for prep abort*/

    int iFileID;        //���ڴ�����ļ���

    int iSendTickets;   //���ļ��Ѿ��������εĻ�����

} TProcessInfo;



typedef struct THeadInfo {

    int iSysPID;

    int iProcNum;

    int iRefreshFlag;

    //add by jx for 2.8 test

    int iCpuUsed; //CPUʹ����

    int iMemUsed;//�ڴ�ʹ����



    char bootTime[16]; //add by xn 20100719 for hbstat



    TProcessInfo ProcInfo;

}THeadInfo;

};

#endif/*__HSS_APP_GUARD_H_INCLUDED_*/