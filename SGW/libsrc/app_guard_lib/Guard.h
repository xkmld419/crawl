/*VER: 3*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef WF_GUARD_H__
#define WF_GUARD_H__

#include "WfPublic.h"
#include "mntapi.h"

#include <sys/types.h>
#include <sys/wait.h>

#include <StdEvent.h>
#include <MessageQueue.h>

const int HOSTNAME_MAX_LEN = 33;

void GetWorkFlowKey ();

//��ʼ��,��ʼ���ź���
int CommunicationInit(bool bInit = false);

//����Ӧ�ó����¼���
void ResponseAppConnect();

//����Ӧ�ó���Ĵ���Ч��
void CalcTreatRate();

//ά��Ӧ�ý���
void CheckApplication (int iHostID, char sUserName[]);

///////////////////////////////////////////////////////////////////
// ClassName: TMsgQueueMgr
// Function : ������Ϣ���й�����
//-----------------------------------------------------------------
class TProcessQueue { 
 //���ÿ�����̵���Ϣ����
 public:
    int m_iProcessID;
    int m_iBillFlowID;
    int m_iMqID;
    int m_iQueueSize;
	int m_iMsgEventNum;      // ÿ����Ϣ������StdEvent��

    TProcessQueue *m_poOutProcQueue;
    TProcessQueue *m_poNextProcess;
    
    TProcessQueue() { 
        m_poOutProcQueue=NULL; m_poNextProcess=NULL; 
        m_iProcessID=0; m_iBillFlowID=0; m_iMqID=0; m_iQueueSize=0;
    }
    ~TProcessQueue(){
        #ifdef WF_DEBUG
        cout<<"Deleting m_iProcessID:"<<m_iProcessID<<" m_iMqID:"<<m_iMqID<<endl;
        #endif
        if (m_poOutProcQueue) delete m_poOutProcQueue;
        if (m_poNextProcess)  delete m_poNextProcess;
    }
};

class TMsgQueueMgr {
 public:
    TMsgQueueMgr() { load(); }
    ~TMsgQueueMgr() { unload(); }
    void reload() { unload(); load(); }
    bool createSelfQueue(int iProcessID);      //����������ӵ�еĶ���
    bool deleteSelfQueue(int iProcessID);      //ɾ�����̶���
    bool createAssociateQueue(int iProcessID); //�������̹��������ж���
    bool deleteAssociateQueue(int iProcessID); //ɾ�����̹��������ж���
    bool createBillFlowQueue(int iBillFlowID);  //��������������Ϣ����
    bool deleteBillFlowQueue(int iBillFlowID);  //ɾ������������Ϣ����
    
    int getMsgNumber(int _iProcessID); //��ȡ������Ϣ��Ŀ
    int getPercent(int _iProcessID);   //��ȡ��Ϣ����ʹ�ðٷֱ�
    
    //��ӡ�������
    void printQueue(char sBuff[]);
    
 private:
    void load();
    void unload();
    
    TProcessQueue *searchProcQueue(int _iProcID);
    
    TProcessQueue *m_poQueueHead;
};


///////////////////////////////////////////////////////////////////
// ClassName: TServerHostInfo
// Function : ���ػ�/Զ���ػ�ģ�鹲�û����ඨ�� (wf_host)
//-----------------------------------------------------------------
struct TServerHostInfo {

    TServerHostInfo() {    m_poNext = NULL; }
    
    ~TServerHostInfo() {
        if (m_poNext) { delete m_poNext; m_poNext = NULL;}

        #ifdef WF_DEBUG
        cout<<"> Delete HostInfo->m_iHostID   : "<<m_iHostID<<endl;
        cout<<">        HostInfo->m_sHostName : "<<m_sHostName<<endl;
        #endif
    }
    
    int    m_iHostID;                   
    char   m_sHostAddr[16];                //���� IP
    char   m_sHostName[HOSTNAME_MAX_LEN];  //���� hostname

    TServerHostInfo *m_poNext;
};


///////////////////////////////////////////////////////////////////
// ClassName: TServerAppInfo
// Function : Ӧ�ý���ִ�мƻ����� (wf_process)
//-----------------------------------------------------------------
class TServerAppInfo {

 friend class TRouteTable;

 public:

    TServerAppInfo() { m_poNext = NULL; m_bAutoBootWhenAbort=false;}
    
    ~TServerAppInfo() {
        if (m_poNext) { delete m_poNext; m_poNext = NULL; }
        #ifdef WF_DEBUG
        cout << "> Delete AppInfo->m_iProcessID : " << m_iProcessID;
        cout << "; m_cState : " << m_cState;
        cout << "; m_iBillFlowID : "<< m_iBillFlowID ;
        cout << "; m_sExecCommand : "<<m_sExecCommand;
        cout << "; m_sExecPath : "<<m_sExecPath;
        cout << endl;
        #endif
    }   

   void setState(char _cState) { 
        m_cState = _cState; 
        updateState(); 
    }
   
    int    m_iAppID;
    int    m_iProcessID;        //���̱�ʶ,WF_PROCESS.PROCESS_ID
    int    m_iBillFlowID;       //�Ʒ����̱�ʶ
    int    m_iHostID;           //������ʶ
    int    m_iAutoRun;          //0:���Զ�ִ�� 1:�����Զ�ִ��(��ǰ�ý��̶��������ʱ)
    int    m_iAppType;          
               //0:��̨���� 1:��̨Unix shell�ű� 2:�洢���� 3:PL/SQL�� 4:ǰ̨Ӧ�ó��� 
    char   m_sExecCommand[MAX_APP_PARAM_LEN];  //ִ���ļ���������
    char   m_sExecPath[80];     //·��
    char   m_sSqlStatement[2000]; // m_iAppType == 3 ʱ��Ч
    char   m_cState;            //����״̬ A-δ����; R-��������; E-��������; X-�쳣 

    char   m_sDBUserName[20];   //�������ݿ���û���
    char   m_sDBConnectStr[20]; //�������ݿ�����Ӵ�
    
    int    m_iOrgID;            //����������OrgID
    
    char   m_sSysUserName[32];  //��̨Unix�û���
    
    bool   m_bAutoBootWhenAbort; //abort ��������Ƿ��Զ�����
    
    TServerAppInfo  *m_poNext;
    
 private:    
    void updateState() {
        char sql[1000];
        DEFINE_QUERY(qry);
        sprintf(sql,"update wf_process set STATE = '%c' where process_id = %d",m_cState,m_iProcessID);
	try{
        qry.setSQL(sql);
        qry.execute();
        qry.commit();
        qry.close();
	}catch( TOCIException& e){
		qry.rollback();
                qry.close();
                return ;
		
	}	
    }
};


///////////////////////////////////////////////////////////////////
// ClassName: TRouteTable
// Function : ����Ӧ�ý��̲����� (wf_host + wf_process)
//-----------------------------------------------------------------
class TRouteTable {
 public:
    TRouteTable()  { load(); }
    ~TRouteTable() { unload(); }
    void load();
    void unload();
    void reload()  { unload(); load(); }
    
    //## ����iProcID��ȡ���̶�ӦTServerAppInfo��ַ, �Ҳ��� return NULL
    TServerAppInfo* getServerAppInfo(int _iProcID);
    
    //## ����hostname����HostID, If can't found, return -1.
    int getHostIDByHostName(char *_hostname);

    TServerHostInfo * getHostInfoByHostID(int _iHostID);
    bool getHostNameByHostID(int _iHostID, char _sHostName[]);
    
    //## ���� _iProcID ���ҽ��̲��������ID, if can't found, return -1;
    int getHostIDByProcessID(int _iProcID, char *_sUserName);
    
    //## �������� (0-ʧ��, 1-�ɹ�)
    int startApp(int _iProcID,int _iProcessLogID = 0, int *pFailedReason = 0x0);
    
    //## ֹͣ���� (0-ʧ��, 1-�ɹ�)
    int stopApp(int _iProcID);
    
    //## ǿ�иı� TServerAppInfo �Ľ���״̬(0-ʧ��, 1-�ɹ�)
    int changeState(int _iProcID,char _cState);

    //## ���host_id�Ƿ����
    bool touchHostID(int _iHostID) {
        TServerHostInfo *pHostInfo;
        for (pHostInfo = m_poHostInfo; pHostInfo; pHostInfo = pHostInfo->m_poNext ) {
            if (pHostInfo->m_iHostID == _iHostID )
                return true;
        }
        return false;
    }
    
    void createAssociMsgQueue(int _iProcessID) {
        m_poMsgQueue->createAssociateQueue(_iProcessID);
    }
    
    void killAllApp ();
    
 private:
     TServerHostInfo *m_poHostInfo;
     TServerAppInfo  *m_poAppInfo;
     TMsgQueueMgr    *m_poMsgQueue; //��Ϣ����ά��
    
    //## ִ�б�����Ŀ�ִ�д���
     int startCommand(TServerAppInfo *);
    
     //## ִ��PLSQL
    int startSQLCommand(TServerAppInfo *,TAppInfo *);
    
    //## ִ��Unix Shell�ű�
    int startShell(TServerAppInfo *,TAppInfo *);
    
};


//#################################################################
#endif

