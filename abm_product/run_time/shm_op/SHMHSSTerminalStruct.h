/*
*   ����TERMINAL MONITOR �ڴ�
*/
#ifndef __SHMHSS_TERMINAL_MONITOR_H_INCLUDED_
#define __SHMHSS_TERMINAL_MONITOR_H_INCLUDED_

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


#endif/*__SHMHSS_TERMINAL_MONITOR_H_INCLUDED_*/