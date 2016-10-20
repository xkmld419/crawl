/*VER: 3*/ 
#ifndef _MNT_API_H_
#define _MNT_API_H_

#include <stdarg.h>
#include <map>

#include "IpcKey.h"
#include "WfPublic.h"
#include "ProcessInfo.h"

#include "CommonMacro.h"
#include "KeyTree.h"
//#include "EventTypeMgr.h"

const int WF_MAX_FILE_NAME_LEN = 256;


#ifdef __cplusplus
extern "C" {
#endif

	// modify by huff proc_id deliver by param at 2011-07-15
    /* function declare */
    int ConnectTerminalMonitor (int iProcID=-1);
    int DisconnectTerminalMonitor (int iState = ST_SLEEP);
    int AddTicketStat (int iType, int iNum=1);
//	int AddThruput (int iNum);
    int SetProcState (int iState);
    int WriteMsg(int code, const char *fmt, ...);
    int WriteErr(int code, const char *fmt, ...);
    int GetBillFlowID ();
    void SetProcHead(THeadInfo*);    
    /// workflow api begin. 
    // modify by huff proc_id deliver by param at 2011-07-15 
    bool ConnectMonitor(int iProcID=-1);
    
    //WF_NORMAL--�����˳�;                  WF_ABORT   --�쳣��ֹ; 
    //WF_STOP  --ǰ̨���Ƚ��淢��ֹͣ����;  WF_RUNNING --��������
    bool DisConnectMonitor(char cMode = WF_NORMAL);
    
    /*
    ���Ӧ�ý�����־���澯��Ϣ��ǰ̨���Ƚ���
        iAlertCode == 0ʱ ��ʾ�Ǹ澯��Ϣ
            (AlertCode �ڱ�BILL.WF_ALERT_DEFINE ���н���,��ģ�����Ӹ澯����ʱ,
            ��Ҫ��CommonMacro.h���ͷ�ļ������ݿ����������Ӧ�ļ�¼)
        bInsertDB: �Ƿ���Ҫ��澯��־��(wf_alert_log)
    
    */

	void PutPetriRet(int iRet, char *sRetInfo);
	void ClearPetriRet();
    bool Pprintf(int iAlertCode,bool bInsertDB,int iFileID,char const *fmt,...);   
    
    /*20090625 shell�ʹ��д��־�޷�����Pprintf,��Ϊ�Ǳ������g_pAppInfoΪ�գ�
    Ϊ�˲�Ӱ�����е��ã���������д��־�ĺ���*/
    bool Pprintf_ex(int iAlertCode,bool bInsertDB,int iFileID,TAppInfo *pAppInfo,char const *fmt,...); 
    
    bool AddProcessCnt(int _iAddCnt=1); //���ӵ�ǰӦ�ý������ݴ�����(����һ�μ�1)
        
    int GetProcessID();   //��ȡ��ǰӦ�ý���ID,��ӦWF_PROCESS.PROCESS_ID
    
    //## �����¼��������ڴ�(���Ǿɵ��¼�)
    bool SaveLastEvent(StdEvent *pEvt);
    
    //## ȡ����󱣴���¼�(�����ظ�ȡ, ���¼�����false)
    bool GetLastEvent(StdEvent *pEvt);
    
    //## ��ȡ�Ʒ��¼��Ĵ���״̬(��Ӧ�ó������)
    bool SetEventState (int _iState);
    bool GetEventState (int *piState);
    
    //## �桢ȡһ���ڴ�����
    //   iLen ��Ч����-�ֽ���  SAVE_BUFF_LEN  
    //   (#define SAVE_BUFF_LEN  128    at WfPublic.h)
    bool SetSaveBuff (void *pt, int iLen);
    bool GetSaveBuff (void *pt, int iLen);
    
    //## ��ȡ5��long�͵�ֵ(iValueID = 1~5)
    bool SetSaveValue (int iValueID, long lValue);
    bool GetSaveValue (int iValueID, long &lValue);

    //## ��ָ�����̵��û���ѹ��־(���iProcessIDΪ0, ��ȡ��ǰ����ID)
    bool SetServBlockFlag (int iFlag, int iProcessID = 0);
    //## ȡָ�����̵��û���ѹ��־(���iProcessIDΪ0, ��ȡ��ǰ����ID), ��ȡ��������-1
    int  GetServBlockFlag (int iProcessID = 0);
    
    //## ������iProcessID �Ƿ����
    bool CheckProcAlive (int iProcessID);


    bool SetSaveValueByProcID (int iValueID, long lValue, int iProcessID);
    bool GetSaveValueByProcID (int iValueID, long &lValue, int iProcessID);
    int AddSendTickets(int iFileID,int iTickets);
    int GetLastSendTickets(int iFileID);
    
#ifdef __cplusplus
};
#endif
/*
class TStatLog;

class TStatLogMgr {
public:
    //ͳ��ģ����������
    static void addInEvent(int _iFileID, int _iEvtTypeID, int iCnt=1);
    static void addInEvent(int _iFileID, StdEvent *pEvt, int iCnt=1);
    static void addInEventEX(int _iFileID, StdEvent *pEvt, int iCnt=1);
    
    static void addInEventEX(int _iFileID, int _iEventTypeID,int iFromProcessID,int iCnt=1);
	  static void addOutEventEX(int _iFileID, int _iEventTypeID, StdEvent *pEvt, int iFromProcessID,int iDesProcessID[],int iDesProIDCount,int iCnt=1);
    
    //ͳ��ģ���������
    static void addOutEvent(int _iFileID, int _iEvtTypeID, StdEvent *pEvt, int iCnt=1);
    static void addOutEvent(int _iFileID, StdEvent *pEvt, int iCnt=1);
    static void addOutEventEX(int _iFileID, int iFromProcessId, StdEvent *pEvt, int _iDesProcessID[],int _iDesProIDCount, int iCnt=1);
    
    static void addRemainAEX (int _iFileID, StdEvent *pEvt, int iCnt=1);
    static void addRemainBEX (int _iFileID, StdEvent *pEvt, int iCnt=1);
    
    static void addRemainA (int _iFileID, int iCnt=1);
    static void addRemainB (int _iFileID, int iCnt=1);
    static void addDistributeRemianA(int _iFileID, int iAppID,int iFromProcessId,StdEvent *pEvt, int _iDesProcessID[],int _iDesProIDCount, int iCnt);
    static void addDistributeRemianB(int _iFileID, int iFromProcessId,StdEvent *pEvt,int _iDesProIDCount, int iCnt);
    
    static void insertLog(int _iFileID);      //һ���ļ��������ύ
    static void insertLogEx(int _iFileID,int _iFromProcessID,int _iProcessIDBuf[],int _iProCount);      //һ���ļ��������ύ
    
    static void setForceFlag(bool bFlag);
    
    static void insertLogForce();//12.9forǿ���ύ ���ڽ��̶�ֱ�ӵ��þ�̬���� ����������Ч ֻ�����Ե�ǿ���ύ
    ~TStatLogMgr();

private:
	static void addDistributeRemianB(int _iFileID, int iFromProcessID,int iDesProIDCount,int iCnt);
	static void addDistributeRemianA(int _iFileID, int iAppID, int iFromProcessID,int iDesProcessID[],int iDesProIDCount,int iCnt);
		
	static int getParentSecLevelEvtType(int _iEvtTypeID);
    static void initSecLevelEventType();
    static TStatLog *searchLogByFileID(int iFileID);
    static TStatLog *searchLogEX(string sMapKey);
    static void addDistributeRemianA(int _iFileID, int iCnt=1);
    static void addDistributeRemianB(int _iFileID, int iCnt=1);
    
private:
    
    static TStatLog *m_poLog;
    static KeyTree<int>* m_pSecLevelEventType;
    //static EventTypeMgr* m_poEventTypeMgr;
    
    static map<string,TStatLog *> m_mapStatLog;
	static map<string, TStatLog*>::iterator m_mapIter;
    static string m_strMapKey;
    static char m_sTMapKey[64]; 
    
    static bool m_bForceFlag;
private:
#ifdef _STAT_FILE_LOG    
	
	//static char m_sLogFilePath[200]; �ŵ�process�г�ʼ��
	
	static char m_sLogOpenFilePath[300];
	
	static char m_sLogCloseFilePath[300];
	
	static char m_sLogCreateTime[16];//�������и�����
	
	static char m_sLogFileName[100];
	
	static char m_sLogFullFileName[400];
	
	static FILE *m_pFile;
	
	static int m_iLogRecords;
	
	static bool writeStatLog(TStatLog* pStatLog);
	
	static int indbStatLog(TStatLog* pStatLog, int iLogRecords);
	
	static bool initLogFilePath();
	
	static int commitStatLog(bool bCommit = false);
	
	static int commitStatLogForce();
	
	static bool checkDelay();
#endif	
};


class TStatLog {

friend class TStatLogMgr;
public:
    TStatLog(int _iFileID = 0,int _iFromProcessID = 0,int _iDescProcessID = 0) {
        reset();
        m_iFileID = _iFileID;
        m_iDescProcessID = _iDescProcessID;//Ŀ����̺�
    	m_iFromProcessID = _iFromProcessID;//��Դ���̺�
    }
    
    void addInEvent(int _iEvtTypeID, int iCnt=1);   //ͳ��ģ����������
    void addOutEvent(int _iEvtTypeID, StdEvent *pEvt, int iCnt=1);  //ͳ��ģ���������
    void insertLog();      //һ���ļ��������ύ
    void insertLogEx();      //һ���ļ��������ύ
    void setFileID (int _iFileID) { m_iFileID = _iFileID; }
    
private:
    void addInNormal(int iCnt=1);   //��ȷ�¼������� 
    void addInBlack(int iCnt=1);    //�ڻ��¼������� 
    void addInDup(int iCnt=1);      //�ظ��¼������� 
    void addInPreErr(int iCnt=1);   //Ԥ��������¼�������(Ԥ�����Ҳ�������,���������Ҳ���)
    void addInPricErr(int iCnt =1); //���۴����¼�������
    void addInUseless(int iCnt=1);  //��Ч�¼�������
    void addOutNormal(StdEvent *pEvt, int iCnt=1);  //��ȷ�¼������
    void addOutBlack(StdEvent *pEvt, int iCnt=1);   //�ڻ��¼������
    void addOutDup(StdEvent *pEvt, int iCnt=1);     //�ظ��¼������ 
    void addOutPreErr(StdEvent *pEvt, int iCnt=1);  //Ԥ��������¼������ 
    void addOutPricErr(StdEvent *pEvt, int iCnt=1); //���۴����¼������
    void addOutUseless(StdEvent *pEvt, int iCnt=1); //��Ч�¼������
    void addFormatErr(int iCnt=1);  //Ԥ�����ʽ����
    void addOutFormatErr(StdEvent *pEvt, int iCnt=1);
    
    void addRemainA (int iCnt=1) {m_iRemainA += iCnt;}
    void addRemainB (int iCnt=1) {m_iRemainB += iCnt;}
    
    void addDistributeRemianA(int iCnt=1);
    void addDistributeRemianB(int iCnt=1);
  
public://�ļ�����޸�
    int m_iFileID;   //�����ļ�ID
    char m_sFileName[WF_MAX_FILE_NAME_LEN]; //�����ļ���
    int m_iInNormal; //��ȷ�¼�������
    int m_iInBlack;  //�ڻ��¼�������
    int m_iInDup;    //�ظ��¼�������
    int m_iInPreErr;   //Ԥ��������¼�������(Ԥ�����Ҳ�������)
    int m_iInPricErr;  //���۴����¼�������
    int m_iInUseless;  //��Ч�¼�������
    int m_iOutNormal;  //��ȷ�¼������
    int m_iOutBlack;   //�ڻ��¼������
    int m_iOutDup;     //�ظ��¼������
    int m_iOutPreErr;  //Ԥ��������¼������
    int m_iOutPricErr; //���۴����¼������
    int m_iOutUseless; //��Ч�¼������
    int m_iFormatErr;  //��ʽ����(ָԤ�����ȡ�ĸ�ʽ���󻰵�)
    int m_iOutFormatErr; //��ʽ����(ָԤ�����ȡ�ĸ�ʽ���󻰵�)
    int m_iRemainA;  //�����ֶ�1 ()
    int m_iRemainB;  //�����ֶ�2 ()
    
    int m_iDistributeRemainA; //�ַ�����
    int m_iDistributeRemainB; //�ַ����
    
    int m_iDescProcessID;//Ŀ����̺�
    int m_iFromProcessID;//��Դ���̺�

	int m_iTempProcessID;//��ʱ����
	
	long m_lNormalDuration; //����������ʱ���ۻ�
	long m_lNormalAmount; //���������������ۻ�
	long m_lNormalCharge; //���������ķ����ۻ�
	
	long m_lBlackDuration;
	long m_lBlackAmount;
	long m_lBlackCharge;
	
	long m_lDupDuration;
	long m_lDupAmount;
	long m_lDupCharge;
	
	long m_lPreErrDuration;
	long m_lPreErrAmount;
	long m_lPreErrCharge;
	
	long m_lPricErrDuration;
	long m_lPricErrAmount;
	long m_lPricErrCharge;
	
	long m_lUseLessDuration;
	long m_lUseLessAmount;
	long m_lUseLessCharge;
	
	long m_lFormatErrDuration;
	long m_lFormatErrAmount;
	long m_lFormatErrCharge;
    
#ifdef _STAT_FILE_LOG    
	int m_iProcLogID;
	int m_iProcessID;
	int m_iBillflowID;
	char m_sMsgDate[9];
	char m_sMsgTime[7];
#endif     
    TStatLog* m_poNext;
    
private:
    void reset();
};

///////////////////////////////////////////////////////////////////
// ClassName: TAlertLogMgr
// Function : �澯��־����( wf_alert_log ), �ṩӦ�ý�����־����ӿ�
// comment  : Pprintf() �ڲ�����, ��Ҫ������Ϣֱ��Insert&&Commit
//-----------------------------------------------------------------
class TAlertLogMgr {
 public:
    int        m_iProcessID;
    int        m_iAppPID;
    int        m_iBillFlowID;
    int        m_iInforType;   //��Ϣ���ͣ� ������Ϣ or ������Ϣ��
    char       m_sRecordTime;  //��Ϣ������ʱ��
    char       m_sInfor[MAX_APPINFO__LEN];
};
*/

class MntAPI {
  public:
    static TProcessInfo *m_pProcInfoHead;
    static THeadInfo * m_poInfoHead; 
    
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
    static int  stopProcess (int iProcID, int iWaitSecs=0, bool bCheckMQ=false);
    
    // ���ؽ���(iProcID)����Ϣ��������Ϣ��Ŀ, ��Ϣ���в����ڷ���-1
    static int  getMsgNumber (int iProcID);
    
    // �������� iProcID
    // ����
    //   1 -- �ɹ�
    //   0 -- ��ǰ�����Ѿ���������״̬, ����Ҫ�ٴ�����
    //  -1 -- ����ʧ�� (����ID������) 
    //  -2 -- ��ǰ����״̬�쳣(���ܷ��������ź�)
    //  -3 -- ���������źź��������ɹ�
    static int  startProcess (int iProcID);
  
  private:
    static TProcessInfo * getProcessInfo (int iProcID);
    
};

#endif

