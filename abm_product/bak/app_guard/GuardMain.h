/*VER: 1*/ 
#ifndef __WF_GUARD_MAIN_H__
#define __WF_GUARD_MAIN_H__

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Guard.h"
#include "OrgMgr.h"

#include "TOCIQuery.h"

#include "ProcessInfo.h"

const int MAIN_CYCLE_DELAY = 10000;  //0.01 ��
const int OUT_MSG_DELAY    = 200000; //0.2 ��
const int MAX_OUT_MSG_CNT  = 10;


//////  ����Ϊmnt���Ͻ����Ĵ��� ///////////////////////
////////////////////////////////////////////////////////

int  InitWorkFlowGuard();
void WorkFlowGuard();
void CalcTreatRateEx();//add by xn
void ResponseMntConnect();
void ResponseClientConnect();

void ResponseRecvAppInfo();
void ResponseSendCommand();
void ResponseChangeStatus();
void ResponseRefreshRunplan();
void ResponseGetCommand();
void ResponseAckCommand();
void ResponseSendRemoteMsg();

void OutputMsgIntoFrontBuff(); 
void CheckMonitorAlive();
void CheckClientAlive();
void ScanOrdinalPlan();
void ScanTimeRunPlan();

///////////////////////////////////////////////////////////////////
// ClassName: TMonitorFront
// Function : ǰ̨���̵�������, ��Ϣ����
//-----------------------------------------------------------------
class TMonitorFront {
 public:
    TMonitorFront() { 
        init();
    }
    ~TMonitorFront() {};
    
    void init();
    
    // ѹ���̨Ӧ�ý�����Ϣ�� m_sMsg[m_iMsgHead]; ���򸲸���ɵ���Ϣ;
    bool pushMsg(const char _sMsg[], bool bNeed=false);
    
    // ��ȡһ����̨Ӧ�ý�����Ϣ from m_sMsg[m_iMsgTail];
    bool getMsg(char _rsMsg[]);
    
    // ���ǰ̨�����Ƿ񻹻���
    bool checkLinkAlive() {
        if (m_bInit) { m_bInit = 0; return true;}        
        Date o_tmpDate;
        o_tmpDate.addSec(-3600*48); //2�첻�
        if (o_tmpDate>m_oLastOutDate)
            return true;
        else return false;
    }
    
    int     m_iStatus;        // -1:δ����(����)  1:����
    int     m_iAuthID;        // iAuthID �����ػ����̴���Monitor��¼ʱά��;
      // �����֤ID����:[[ ��ǰ̨Monitorĳ���Ʒ����̳�ʱ��δ����
      // <ͨ��ʱ���жϵķ���,���ṹ��iLastReqTime����¼��һ�����������Ϣ��ʱ��>,
      // ���ػ���������Ϊ���쳣�˳�,ע���� { iStatus = -1;MntReqInfo.m_iRegNum --; }
      // �����֤ID�Ƿ�ֹ ĳ��ǰ̨���̵ļ�ؽ��� �����쳣ԭ��ʱ��δ���ͳ������
      // �����ػ�����ע���������������ϣ�����ʱ����iIndex�Ѿ������������ؽ��� 
      // ����ɴ��ҵ�������� ]]
    int     m_iBillFlowID;        // �Ʒ�����ID
    int     m_iLogMode;            // ��¼ģʽ�� LOG_AS_VIEW  LOG_AS_CONTROL
    int     m_iStaffID;
    char    m_sHostAddr[15];    //Monitor����IP��ַ
    
    void    parseOrgIDStr(char *str);
    
    bool    belongOrg(int iOrgID); //iOrgID �Ƿ����ڵ�ǰ���ӵĹ���Χ
    
 private:
    char    m_sMsg[MAX_CMAMSG_CNT][MAX_APPINFO__LEN*2];  // Formated
      //��̨�����Ϣm_sMsg[x]: <iProcessID=***><cState='*'><sMsg=***1>...<sMsg=***n>
    int     m_iMsgHead;
    int     m_iMsgTail;
    
    char    m_sMsgNeed[MAX_CMAMSG_CNT][MAX_APPINFO__LEN*2];  //ǰ̨�������Ϣ
    int     m_iMsgNeedHead;
    int     m_iMsgNeedTail;
    
    int        m_aiOrgID[MAX_ORG_CNT];

    Date    m_oLastOutDate;     // ���һ�����MNT��Ϣ��ʱ��
    
    bool    m_bInit;
};


///////////////////////////////////////////////////////////////////
// ClassName: TClientInfo
// Function : Զ�̼���ػ�����/ָ�� ��Ϣ����
//-----------------------------------------------------------------
struct TPlanCommand {
    TPlanCommand() { m_poNext = NULL ; }
    char m_sCommMsg[MAX_OUTMSG_LEN];
    TPlanCommand *m_poNext;
};

class TClientInfo {

 private:
 
    int  m_iMntCommStatus;   // 0-��Զ��MNTָ��(ִ��)��Ϣ; 1-MNTָ���ȡ��δ����; 2-��MNTָ�����ȡ
    char m_sMntCommand[MAX_OUTMSG_LEN]; // Formated MNT command.

    int  m_iPlanCommStatus;             // 0-��Զ�̼ƻ�ָ��(ִ��)��Ϣ; 1-Զ�̼ƻ�ָ���ȡ��δ����
    char m_sPlanAckMsg[MAX_OUTMSG_LEN]; 
    TPlanCommand *m_poCommList;         // �ƻ�ָ���б� Plan command List.
    
    // get head record.
    bool getPlanCommand(char *_sCommand) 
    {
        TPlanCommand *pt = m_poCommList;
        if (!pt) return false;
        m_poCommList = pt->m_poNext;
        strcpy( _sCommand, pt->m_sCommMsg);
        delete pt;
        m_iPlanCommStatus = 1;
        return true;
    }
    
 public: 
    int  m_iHostID;    // InitValue -> -1 (show it is not used)
    char m_sSysUserName[32];
    
    Date m_oLastTouch;

    TClientInfo() { 
        init();
    }    
    void init() {
        m_iHostID = -1; 
        m_iMntCommStatus = 0;
        m_iPlanCommStatus = 0;
        m_poCommList = NULL;
        m_oLastTouch.getCurDate();    
    }
        
    // ѹ��MNTָ��,�Ѵ���MNTָ���ָ��δ������->fail;  success-1, fail-0
    bool pushMntCommand(const char *_sCommand) {
        if (m_iMntCommStatus == 0) {
            strcpy( m_sMntCommand, _sCommand);
            m_iMntCommStatus = 2;
            return true;
        }
        else return false;
    }
    
    // add to the tail.
    void pushPlanCommand(const char *_sCommand)
    {
        TPlanCommand **ppt;
        for (ppt = &m_poCommList; (*ppt); ppt = &((*ppt)->m_poNext) ) ;
        
        ppt = ppt;    
        (*ppt) = new TPlanCommand;
        strcpy( (*ppt)->m_sCommMsg, _sCommand);
        
        //cout<<ppt<<" "<<&m_poCommList<<endl; 
    }
        
    // ��ȡ��Զ�����ӵĴ�ִ��ָ��( ��֮ǰ��ָ���ȡ��δ����,��������ȡ�µ�ָ��,return false )
    // ��ָ�� @return 0; ȡ��ָ��@return 1; ��δ���ؽ����ָ��(�쳣)@return 2;
    int getCommand(char *_sCommand)
    {
        m_oLastTouch.getCurDate();
        
        if ( m_iMntCommStatus == 1 || m_iPlanCommStatus == 1)  //��ָ���ȡ��δ����
            return 2;
            
        if ( m_iMntCommStatus == 2 ) { //��MNTָ��
            strcpy( _sCommand,m_sMntCommand);
            m_iMntCommStatus = 1;
            return 1;
        }
        
        if ( getPlanCommand(_sCommand) ) {//ȡ����һ���ƻ�ָ��
            m_iPlanCommStatus = 1;
            return 1;
        }
        
        //��ָ��:
        return 0;
    }
    
    //��һ����ȡ�ߵ�ָ��ִ�����,���ظ�Զ�̼�����ӹ�����. 
    //�����MNTָ�� @return 1; ���Ǽƻ�ָ�� @return 2;
    //Err @return 0;
    int ackCommand() { 
        if (m_iMntCommStatus == 1) { m_iMntCommStatus = 0; return 1; }
        else if (m_iPlanCommStatus == 1) { m_iPlanCommStatus = 0; return 2; }
        else return 0; //err.
    }
    
    bool checkLinkAlive() {
        Date o_tmpDate;
        o_tmpDate.addSec(-30); //30������
        if (o_tmpDate>m_oLastTouch)
            return true;
        else return false;
    }
};


///////////////////////////////////////////////////////////////////
// ClassName: TTimeRunPlan
// Function : ��ʱִ�мƻ����� (��Ӧ wf_time_plan,wf_run_time_plan)
//-----------------------------------------------------------------
class TRunPlan ;
class TTimeRunPlan {
 public:
    TTimeRunPlan()  { load(); }
    ~TTimeRunPlan() { unload(); }
    void load();
    void unload();
    void reload()   { unload(); load(); }
    
    //����Ƿ��е���(����ƻ�ִ��ʱ���)�Ľ���,���û��,����0
    //�����,���� ��ִ�еĽ��̸���, ���� TRunPlan.m_bShouldRun = true.
    int checkArrive();
    
    //��� _pRunPlan ָ���ʱ��ƻ���ʽ�Ƿ���ȷ
    // ��ȷ->���� true
    // ����->���� false, ���ñ� wf_time_plan.check_err = 1
    bool checkFormatCorrect(TRunPlan *_pRunPlan);
        
    //�ӵ�ǰ�ƻ��в��ҵ�һ����Ҫ���еļƻ�(RunPlan.m_bShouldRun == true),
    //�ҵ� ���� ProcessID,_rRunPlanID,_iStaffID,
    //     ����TRunPlan.m_bShouldRun = false  
    //���򷵻� -1
    int  getShouldRunProcess(int &_rRunPlanID, int &_iStaffID);

 private:
 
    TRunPlan *m_poRunPlan;
    Date  m_oCurDate;
    
    void UpdateErrPlan(int _iPlanID);
};

class TRunPlan {
 public:
    TRunPlan() { m_bShouldRun = false; m_poNext = NULL; m_poLastCheckDate = NULL; }
    ~TRunPlan() {
        if (m_poNext) { 
            delete m_poNext; 
            delete m_poLastCheckDate;
            m_poNext = NULL;
            m_poLastCheckDate = NULL;
        }
        #ifdef WF_DEBUG
        cout<<"Del m_iProcessID:"<<m_iProcessID<<endl;
        printf(" Time: %s��%s��%s��%sʱ%s��; (����%s)",m_sYear,m_sMonth,m_sDay,m_sHour,m_sMinute,m_sWeekDay);
        cout<<"\n m_bShouldRun:"<<m_bShouldRun;
        cout<<endl<<endl;
        #endif
    }

    int     m_iRunPlanID; //��ʱ���мƻ���ʶ
    int     m_iPlanID;    //��ʱ�ƻ���ʶ
    int        m_iProcessID;
    int     m_iStaffID;
    char    m_sYear[5];
    char    m_sMonth[3];
    char    m_sDay[3];
    char    m_sWeekDay[2];
    char    m_sHour[3];
    char    m_sMinute[3];
    bool    m_bShouldRun;
    Date    *m_poLastCheckDate;
    TRunPlan *m_poNext;    
};


///////////////////////////////////////////////////////////////////
// ClassName: TOrdinalPlan
// Function : ˳��ִ�мƻ����� (��Ӧ wf_nodeconnector,wf_process )
//-----------------------------------------------------------------
class TNodeConnector {
 public:    
     TNodeConnector() { m_poNextPrefix = NULL; m_poNextProcess = NULL; }
     ~TNodeConnector() {
         #ifdef WF_DEBUG
         cout<<"PrefixID = "<<m_iPrefixID<<"; ProcID = "<<m_iProcessID<<endl;
         #endif
         if (m_poNextPrefix)  { delete m_poNextPrefix;  m_poNextPrefix = NULL; }
         if (m_poNextProcess) { delete m_poNextProcess; m_poNextProcess = NULL; }
     }
     
    int    m_iProcessID;
    int    m_iPrefixID;    //ǰ�ý���ID
    char   m_cPrefixState; //ǰ�ý��̵�״̬
    
    TNodeConnector  *m_poNextPrefix;  //��ͬ m_iProcessID ����һ���ڵ�
    TNodeConnector  *m_poNextProcess; //��һ�� m_iProcessID ͷ�ڵ�

    void setPrefixState(TRouteTable *pRtTab);
    
    //��鵱ǰ m_iProcessID �Ƿ�Ӧ�ñ�����, ��,��������RouteTable�е�ָ��.
    TServerAppInfo* checkShouldRun(TRouteTable *pRtTab);
    
 private:    
    bool checkPrefixEnd() { //��鵱ǰ m_iProcessID ������ǰ�ý����Ƿ�����������״̬
        if (m_cPrefixState!='E') return false;
        
        TNodeConnector *pt;
        for (pt=m_poNextPrefix; pt; pt=pt->m_poNextPrefix)
            if (pt->m_iPrefixID != 'E') break;
        if (!pt) return true;
        return false;
    }
};

class TOrdinalPlan { 
 public:
    TOrdinalPlan() { m_poPlan = NULL; load(); }
    ~TOrdinalPlan() { unload(); }

    TNodeConnector *m_poPlan;
    
    void load();
    void unload();
    void reload() { unload(); load(); }
    
    //��ȡ _iProcessID ��Ӧ��ǰ�ý��� ����ͷָ��,�Ҳ��� return NULL
    TNodeConnector* getAutoProcInfo(int _iProcessID);
    
    void refreshNodeState(TRouteTable *pRtTab) ;
};


//#################################################################
#endif


