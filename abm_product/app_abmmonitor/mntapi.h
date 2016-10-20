/*VER: 1*/ 
#ifndef _MNT_API_H_
#define _MNT_API_H_

#include <stdarg.h>


#include "WfPublic.h"
#include "ProcessInfo.h"

#include "CommonMacro.h"
#include "KeyTree.h"
//#include "EventTypeMgr.h"

const int WF_MAX_FILE_NAME_LEN = 256;

//TSHMCMA  *g_pShmTable = NULL;
//long g_lBillMntKey = 0; 
//long g_lWorkFlowKey = 0;
//long g_lLockWorkFlowKey = 0;
//long g_lLockApplicationKey = 0;
//long g_lLockClientKey = 0;

#ifdef __cplusplus
extern "C" {
#endif
    /* function declare */
    int ConnectTerminalMonitor (int g_iProcID);
    int DisconnectTerminalMonitor (int iState = ST_SLEEP);
    int AddTicketStat (int iType, int iNum=1);
    int SetProcState (int iState);
    int WriteMsg(int code, const char *fmt, ...);
    int WriteErr(int code, const char *fmt, ...);
    int GetBillFlowID ();
            
    /// workflow api begin.  
    bool ConnectMonitor(int g_iProcID);
    
    //WF_NORMAL--�����˳�;                  WF_ABORT   --�쳣��ֹ; 
    //WF_STOP  --ǰ̨���Ƚ��淢��ֹͣ����;  WF_RUNNING --��������
    bool DisConnectMonitor(char cMode = WF_NORMAL);
    
    /*
    ���Ӧ�ý�����־���澯��Ϣ��ǰ̨���Ƚ���
        iAlertCode == 0ʱ ��ʾ�Ǹ澯��Ϣ
            (AlertCode �ڱ�BILL.WF_ALERT_DEFINE ���н���,��ģ�����Ӹ澯����ʱ,
            ��Ҫ��CommonMacro.h���ͷ�ļ������ݿ����������Ӧ�ļ�¼)
        bInsertDB: �Ƿ���Ҫ��澯��־��(m_wf_alert_log)
    
    */
    bool Pprintf(int iAlertCode,bool bInsertDB,int iFileID,char const *fmt,...);    
    
    bool AddProcessCnt(int _iAddCnt=1); //���ӵ�ǰӦ�ý������ݴ�����(����һ�μ�1)
        
    int GetProcessID(int g_iProcID);   //��ȡ��ǰӦ�ý���ID,��Ӧm_wf_process.PROCESS_ID
    
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

#ifdef __cplusplus
};
#endif

class TStatLog;

class TStatLogMgr {
public:
    //ͳ��ģ����������
    static void addInEvent(int _iFileID, int _iEvtTypeID, int iCnt=1);
    static void addInEvent(int _iFileID, StdEvent *pEvt, int iCnt=1);
    
    //ͳ��ģ���������
    static void addOutEvent(int _iFileID, int _iEvtTypeID, int iCnt=1);
    static void addOutEvent(int _iFileID, StdEvent *pEvt, int iCnt=1);
    
    static void addRemainA (int _iFileID, int iCnt=1);
    static void addRemainB (int _iFileID, int iCnt=1);
    
    static void insertLog(int _iFileID);      //һ���ļ��������ύ
    static int getParentSecLevelEvtType(int _iEvtTypeID);

private:
    static void initSecLevelEventType();
    static TStatLog *searchLogByFileID(int iFileID);
    static TStatLog *m_poLog;
    static KeyTree<int>* m_pSecLevelEventType;
    //static EventTypeMgr* m_poEventTypeMgr;
};


class TStatLog {

friend class TStatLogMgr;
public:
    TStatLog(int _iFileID = 0) {
        reset();
        m_iFileID = _iFileID;
    }
    void addInEvent(int _iEvtTypeID, int iCnt=1);   //ͳ��ģ����������
    void addOutEvent(int _iEvtTypeID, int iCnt=1);  //ͳ��ģ���������
    void insertLog();      //һ���ļ��������ύ
    void setFileID (int _iFileID) { m_iFileID = _iFileID; }
    
private:
    void addInNormal(int iCnt=1);   //��ȷ�¼������� 
    void addInBlack(int iCnt=1);    //�ڻ��¼������� 
    void addInDup(int iCnt=1);      //�ظ��¼������� 
    void addInPreErr(int iCnt=1);   //Ԥ��������¼�������(Ԥ�����Ҳ�������,���������Ҳ���)
    void addInPricErr(int iCnt =1); //���۴����¼�������
    void addInUseless(int iCnt=1);  //��Ч�¼�������
    void addOutNormal(int iCnt=1);  //��ȷ�¼������
    void addOutBlack(int iCnt=1);   //�ڻ��¼������
    void addOutDup(int iCnt=1);     //�ظ��¼������ 
    void addOutPreErr(int iCnt=1);  //Ԥ��������¼������ 
    void addOutPricErr(int iCnt=1); //���۴����¼������
    void addOutUseless(int iCnt=1); //��Ч�¼������
    void addFormatErr(int iCnt=1);  //Ԥ�����ʽ����
    void addOutFormatErr(int iCnt=1);
    
    void addRemainA (int iCnt=1) {m_iRemainA += iCnt;}
    void addRemainB (int iCnt=1) {m_iRemainB += iCnt;}
    
private:
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
    
    TStatLog* m_poNext;
    
private:
    void reset();
};

///////////////////////////////////////////////////////////////////
// ClassName: TAlertLogMgr
// Function : �澯��־����( m_wf_alert_log ), �ṩӦ�ý�����־����ӿ�
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


class MntAPI {
  public:
    static TProcessInfo *m_pProcInfoHead; 
    
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

