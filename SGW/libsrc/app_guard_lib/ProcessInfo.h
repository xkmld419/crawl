/*VER: 2*/ 
#ifndef __PROCESS_INFO_H___
#define __PROCESS_INFO_H___
#include <time.h>
#include "../../etc/LocalDefine.h"
#include "CommonMacro.h"
#ifndef MAX_APP_NUM
#define MAX_APP_NUM 1024  //Ӧ�ý��̵�������,���� "LocalDefine.h" �б��ػ�����
#endif
#ifndef MAX_APP_PARAM_LEN
#define MAX_APP_PARAM_LEN 80
#endif


const int MAX_CMAMSG_CNT  = 32;
const int MAX_APPINFO__LEN = 256;   //����Ӧ�ý�����Ϣ����󳤶�
const int MAX_CMAMSG_LEN  = 1024; //��monitor��������󻺳�

const int MAX_OUTMSG_LEN  = 1024;  //��client��������󻺳�
const int INFO_Q_SIZE     = 100;   //Ӧ�ó��������Ϣ��������


/* ticket_type define */
#define TICKET_NORMAL           0
#define TICKET_ERROR            1
#define TICKET_BLACK            2
#define TICKET_OTHER            3

const char WF_START  = 'B'; //���������� 
const char WF_NORMAL = 'E'; //�����˳�
const char WF_ABORT = 'X';  //�쳣��ֹ
const char WF_STOP = 'K';   //ǰ̨���Ƚ��淢��ֹͣ����
const char WF_RUNNING = 'R'; //��������
// ����״̬cState: A-δ���� B-������ R-������ E-���н��� X-�쳣 K-ǰ̨���Ƚ��淢��ֹͣ����



/* state define */
#define ST_SLEEP       0
#define ST_INIT        1
#define ST_RUNNING     2
#define ST_WAIT_BOOT   3
#define ST_WAIT_DOWN   4
#define ST_KILLED      5
#define ST_ABORT       6
#define ST_DESTROY     7
#define ST_IDLE        8


const int PETRI_ST_NORMAL=0;
const int PETRI_ST_OUT_ACCT_RDY=1;
const int PETRI_ST_OUT_ACCT_PRE=2;
const int PETRI_ST_OUT_ACCTING=3;


#define MAX_MSG_LEN 10
#define MAX_MSG_NUM 10

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
	//������ add by hsir
//	int iThruput;
	
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

#ifdef __cplusplus
extern "C" {
#endif

int CreateShm (long lSize, void **pRet);
int AttachShm (long lSize, void **pRet);
int DetachShm ();
int DestroyShm ();
int InitPV ();
int DestroyPV ();
int P() ;
int V();

#ifdef __cplusplus
};
#endif


#endif
