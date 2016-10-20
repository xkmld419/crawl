// B_REBILL_TASK �������������

#include <sys/wait.h>
#include "CRebillTask.h"

#include "VirtualFileMaker.h"
#include "InstTableListMgr.h"
#include "Process.h"
#include "DataIncrementMgr.h"
#include "IpcKey.h"
#include "EventIDMaker.h"

DEFINE_MAIN (CRebillTask);

extern char **environ;

//extern OrgMgr * gpOrgMgr;



//���û�����
//��˳�����Ĳ���  
const int m_aiRebillServStepID[]=
    {
    REBILL_TASK_STEP_BEGIN ,
    REBILL_TASK_STEP_WAITING ,
    REBILL_TASK_STEP_FILTER ,
    REBILL_TASK_STEP_BACKUPSERV ,
    REBILL_TASK_STEP_ROLLBACKSERV,
    REBILL_TASK_STEP_SENDSERV,
    REBILL_TASK_STEP_RENTSET,
    REBILL_TASK_STEP_THROUGH, 
    REBILL_TASK_STEP_FREE,
    REBILL_TASK_STEP_END
    };

const char m_asRebillServStepStr[][4]=
    {
    "BEG",
    "WAT",
    "FLT",
    "BAK",
    "ROL",
    "SND",
    "RNT",
    "THR",
    "FRE",
    "END"
    };


//���ļ�����
const int m_aiRebillFileStepID[]=
{
    REBILL_TASK_STEP_BEGIN  ,
    REBILL_TASK_STEP_FILESCAN  ,
    REBILL_TASK_STEP_BLOCKSERV ,       
    REBILL_TASK_STEP_CLRDUP       ,
    REBILL_TASK_STEP_BACKUPFILE   ,
    REBILL_TASK_STEP_ROLLBACKFILE ,
    REBILL_TASK_STEP_RESENDFILE,
    REBILL_TASK_STEP_THROUGH, 
    REBILL_TASK_STEP_FREE,    
    REBILL_TASK_STEP_END
    };

const char m_asRebillFileStepStr[][4]=
    {
    "BEG",    
    "FSC",
    "BLK",    
    "DUP",
    "BAK",
    "ROL",
    "RSF",
    "THR",
    "FRE",
    "END"
    };



const int m_aiRebillEventStepID[]=
    {
    REBILL_TASK_STEP_BEGIN  ,
    REBILL_TASK_STEP_BACKUPEVENT,        
    REBILL_TASK_STEP_HISSCAN  ,
    REBILL_TASK_STEP_BLOCKSERV ,       
    REBILL_TASK_STEP_CLRDUP       ,
    REBILL_TASK_STEP_ROLLBACKSERV,
    REBILL_TASK_STEP_SENDSERV,
    REBILL_TASK_STEP_THROUGH, 
    REBILL_TASK_STEP_FREE,    
    REBILL_TASK_STEP_END
    };

const char m_asRebillEventStepStr[][4]=
    {
    "BEG",
    "BAK",
    "HSC",
    "BLK",    
    "DUP",
    "ROL",
    "SND",
    "THR",
    "FRE",
    "END"
    };

const int CREBILL_SERV_STEP_NUM=sizeof(m_aiRebillServStepID)/sizeof(int);
const int CREBILL_FILE_STEP_NUM=sizeof(m_aiRebillFileStepID)/sizeof(int);
const int CREBILL_EVENT_STEP_NUM=sizeof(m_aiRebillEventStepID)/sizeof(int);

   ////event_type_id ��set_fee������û������ֶ�,����Ҫ�����Լ���ֵ,ԭ����Ӧ���ڱ��ֶ��������
const int  SET_FEE_EVENT_TYPE_ID=174;
   ///�����¼�
const int  DISCARD_EVENT_TYPE_ID=9999;

DataIncrementMgr*  CRebillTask::m_pDataIncrementMgr=NULL;


// ȱʡ���캯��
CRebillTask::CRebillTask()
{
    //# �����û����Ϲ����ڴ�
    //Environment::useUserInfo ();

    //## �����ۻ��������ڴ�
    //G_PTRANSMGR = new TransactionMgr (); 
    ///G_PTRANSMGR->begin ();

    m_pDataIncrementMgr
        =new DataIncrementMgr(DATAINC_MODE_PRICING |DATAINC_MODE_PUTINDB);
    m_pFileDbMgr=new FileDbMgr(m_pDataIncrementMgr);
    delete m_pFileDbMgr;
    m_pFileDbMgr=NULL;

    m_hiRebillStep=new HashList<int>(9);
    m_hpCRebillTable=new HashList<CRebillTable*>(10);
    

    m_iBakUseRB=0;
    m_iEvtSType[REBILL_TASK_TYPE_SERV]=EVENT_SOURCE_TYPE_SERV_REBILL;
    m_iEvtSType[REBILL_TASK_TYPE_FILE]=EVENT_SOURCE_TYPE_FILE_REBILL;
    m_iEvtSType[REBILL_TASK_TYPE_EVENT]=EVENT_SOURCE_TYPE_EVENT_REBILL;

    m_bStepRebill=true;
    //m_bStepRebill=false;
    m_bBlockServ=true;
    m_bWaitGatherFile=false;
    m_bCheckGroup=true;
    
}

// ��������
CRebillTask::~CRebillTask()
{


    if(m_pDataIncrementMgr!=NULL) delete m_pDataIncrementMgr; 
    if(m_pFileDbMgr!=NULL) delete m_pFileDbMgr; 
    

    if(m_hpCRebillTable!=NULL)
        {
        HashList<CRebillTable*>::Iteration iterCRebillTable=
            m_hpCRebillTable->getIteration();
        CRebillTable* pCRebillTable;
        while(iterCRebillTable.next(pCRebillTable)==true)
            delete pCRebillTable;
        
        m_hpCRebillTable->destroy();
        delete m_hpCRebillTable;
        }    

    
    if(m_hiRebillStep!=NULL)
        {    
        m_hiRebillStep->destroy();
        delete m_hiRebillStep;
        }
    
}




// �������������ID,����
inline long CRebillTask::GetTaskId() const
{
    return  m_lTaskID[m_iTaskType];
}


// ��ö�Ӧ��������ID
inline long CRebillTask::GetBillingCycleId() const
{
    return m_lBillingCycleID;
}

// ���ö�Ӧ��������ID
inline void CRebillTask::SetBillingCycleId(long i_lBillingCycleID)
{
    this->m_lBillingCycleID = i_lBillingCycleID;
}


//## ���Ի�ȡһ�������������������
// �ɹ�: ���� 1
// ʧ��: ���� 0
int CRebillTask::fetchRebillTask( long i_lTaskID)
{
    DEFINE_QUERY (qry);
    
    //# ���� B_REBILL_TASK ��Ĳ���
    qry.setSQL ("lock table B_REBILL_TASK in exclusive mode");
    qry.execute ();
    qry.close ();

    if(i_lTaskID >0 )
    {////��ȡ�ض�������ID
        qry.setSQL ("select task_id, billing_cycle_id, billing_cycle_type_id,"
            " nvl(task_type,0) task_type, upper(nvl(task_step,'BEG')) task_step,"
            " nvl(sub_task_id,0) sub_task_id,nvl(last_file_id,0) last_file_id"
            " from B_REBILL_TASK where task_id=:vTaskID  and task_state in ('RDY','RUN')" );
        
        qry.setParameter("vTaskID",  i_lTaskID);
        qry.open ();
        
        if (!qry.next ()) 
        {
            qry.commit (); //# ����
            qry.close ();
            return 0;
        }
    }
    else
    {
        qry.setSQL ("select task_id, billing_cycle_id, billing_cycle_type_id,"
            " nvl(task_type,0) task_type, upper(nvl(task_step,'BEG')) task_step,"
            " nvl(sub_task_id,0) sub_task_id, nvl(last_file_id,0) last_file_id"
            " from B_REBILL_TASK where task_state in ('RDY','RUN') order by task_id" );
        qry.open ();
        
        if (!qry.next ()) 
        {
            qry.commit (); //# ����
            qry.close ();
            return 0;
        }
    }        

    m_lBillingCycleID = qry.field("billing_cycle_id").asLong();
    m_iBillingCycleTypeID = qry.field("billing_cycle_type_id").asInteger();
    m_iTaskType = qry.field("task_type").asInteger();
    m_iLastFileID= qry.field("last_file_id").asInteger();

    m_lTaskID[0]=m_lTaskID[1]=m_lTaskID[2]=0;
    
    if(m_iTaskType!=REBILL_TASK_TYPE_SERV)
        m_lTaskID[REBILL_TASK_TYPE_SERV]=qry.field("sub_task_id").asLong();
        


    
    if(m_iTaskType<0 || m_iTaskType>2) 
        m_iTaskType=0;
    m_lTaskID[m_iTaskType]=qry.field("task_id").asLong();

    
    m_hiRebillStep->clearAll();
    switch(m_iTaskType )
    {
        case REBILL_TASK_TYPE_SERV:
            ////���û�����Ҳ��ָ������ѹ����
           //// m_bBlockServ=true;///���û����˵Ļ�,�����ѹ�û�����
            ///���û�����
            ///������
            for(int jTemp=0; jTemp<CREBILL_SERV_STEP_NUM; jTemp++)
                {
                m_hiRebillStep->add((char *)m_asRebillServStepStr[jTemp], jTemp);
                }
            break;
            
        case REBILL_TASK_TYPE_FILE:
            //���ļ�����
            ///������
            for(int jTemp=0; jTemp<CREBILL_FILE_STEP_NUM; jTemp++)
                {
                m_hiRebillStep->add((char *)m_asRebillFileStepStr[jTemp], jTemp);
                }
            
        case REBILL_TASK_TYPE_EVENT:
            ////���¼�����
            ///������
            for(int jTemp=0; jTemp<CREBILL_EVENT_STEP_NUM; jTemp++)
                {
                m_hiRebillStep->add((char *)m_asRebillEventStepStr[jTemp], jTemp);
                } 
            break;
            
        default:  
            break;

    }

    
    ///���û�����,����������,��Ϊ������ײ͵Ļ��˲����˵�һ��ʱ���
    ////������ײͶ����������ڻ��˵�,���ٻ��˵�һ��ʱ���
 
    if(m_hiRebillStep->get(  qry.field("task_step").asString(), &m_iTaskStep)==false)
    {
        m_iTaskStep=-1;
        Log::log(0, "task_id:%ld  ��task_stepδ֪״̬!", GetTaskId() );
        THROW(MBC_Rebill+1);
        return 0;
    }
    
    qry.close();

    qry.setSQL("update B_REBILL_TASK set TASK_STATE = 'RUN' where task_id = :vTaskID ");
    qry.setParameter("vTaskID", this->GetTaskId() );
    qry.execute();
    
    qry.commit (); //# �ύ && ����
    qry.close ();













////�����ݿ��ж�ȡ���õĻ�����Щ������
///    ParamReader oParamReader("B_PARAM_DEFINE", "PARAM_SEGMENT", "PARAM_CODE", "PARAM_VALUE");
    char sTmp_rebill[50];
    char sTmp_resend[50];
    sTmp_rebill[0]='\0';
    sTmp_resend[0]='\0';
    
    if(m_iTaskType==REBILL_TASK_TYPE_SERV)
        {//���û�����
        
        ParamDefineMgr::getParam("REBILL", "REBILL_TABLE_TYPE_SERV", sTmp_rebill, 50 );
        if(sTmp_rebill[0]=='\0')
            {
            Log::log(0,"��ע��:��B_PARAM_DEFINE��û�����ð��û����˵ı�����,\n"
                "ʹ��Ĭ�� 1,2,3,4,51,10,11,85,95,96,86,87");
            strncpy(sTmp_rebill,"1,2,3,4,51,10,11,85,95,96,86,87",50);
            }
        ParamDefineMgr::getParam("REBILL", "RESEND_TABLE_TYPE_SERV", sTmp_resend, 50 );
        if(sTmp_resend[0]=='\0')
            {
            Log::log(0,"��ע��:��B_PARAM_DEFINE��û�����ð��û����˵��ط��ͱ�����,\n"
                "ʹ��Ĭ�� 1,2,3");
            strncpy(sTmp_resend,"1,2,3",50);
            }  

        }
    else
        if(m_iTaskType==REBILL_TASK_TYPE_FILE)
        {//���ļ����� 1,2,3,4,90,92,10,11,51,83,85,95,96,86,87
        ParamDefineMgr::getParam("REBILL", "REBILL_TABLE_TYPE_FILE", sTmp_rebill, 50 );
        if(sTmp_rebill[0]=='\0')
            {
            Log::log(0,"��ע��:��B_PARAM_DEFINE��û�����ð��ļ����˵ı�����,\n"
                "ʹ��Ĭ�� 1,2,3,4,90,92,10,11,51,85,95,96,86,87");
            strncpy(sTmp_rebill,"1,2,3,4,90,92,10,11,51,85,95,96,86,87",50);
            }
        ParamDefineMgr::getParam("REBILL", "RESEND_TABLE_TYPE_FILE", sTmp_resend, 50  );
        if(sTmp_resend[0]=='\0')
            {
            Log::log(0,"��ע��:��B_PARAM_DEFINE��û�����ð��ļ����˵��ط��ͱ�����,\n"
                "ʹ��Ĭ�� 1,2,3,90,92");
            strncpy(sTmp_resend,"1,2,3,90,92",50);
            }          
        }
        else
        {//���¼����� 1,2,3,4,90,92,10,11,51,83,85,95,96,86
        ParamDefineMgr::getParam("REBILL", "REBILL_TABLE_TYPE_EVENT", sTmp_rebill, 50  );
        if(sTmp_rebill[0]=='\0')
            {
            Log::log(0,"��ע��:��B_PARAM_DEFINE��û�����ð��¼����˵ı�����,\n"
                "ʹ��Ĭ�� 1,2,3,4,90,92,10,11,51,85,95,96,86,87");
            strncpy(sTmp_rebill,"1,2,3,4,90,92,10,11,51,85,95,96,86,87",50);
            } 

        ParamDefineMgr::getParam("REBILL", "RESEND_TABLE_TYPE_EVENT", sTmp_resend, 50  );
        if(sTmp_resend[0]=='\0')
            {
            Log::log(0,"��ע��:��B_PARAM_DEFINE��û�����ð��¼����˵��ط��ͱ�����,\n"
                "ʹ��Ĭ�� 1,2,3,90,92");
            strncpy(sTmp_resend,"1,2,3,90,92",50);
            }         
        }            


    strncpy(m_sResendEventTable, sTmp_resend, 50);
    
    int i=0;
    int iTmpTableType=0;
    CRebillTable* pCRebillTable;

    
    while(sTmp_rebill[i]!='\0')
        {
        iTmpTableType= atoi(sTmp_rebill+i) ;
        if(iTmpTableType>0 && m_hpCRebillTable->get(iTmpTableType, &pCRebillTable)==false)
            m_hpCRebillTable->add(iTmpTableType, new CRebillTable( iTmpTableType) );
        
        for( ; sTmp_rebill[i]!='\0'; i++)
            if(sTmp_rebill[i]==',')
                {
                i++;
                break;
                }
        }

    i=0;
    while(sTmp_resend[i]!='\0')
        {
        iTmpTableType= atoi(sTmp_resend+i) ;
        if(iTmpTableType>0 )
            {
            m_viResendEventTable.push_back(iTmpTableType);
            }
        
        for( ; sTmp_resend[i]!='\0'; i++)
            if(sTmp_resend[i]==',')
                {
                i++;
                break;
                }
        }


    return 1;
}


void CRebillTask::finishRebillTask()
{
	DEFINE_QUERY (qry);

    qry.setSQL ("update B_REBILL_TASK"
        " set TASK_STATE='END',state_date=sysdate"
        " where task_id=:vTaskID");    

    qry.setParameter ("vTaskID", this->GetTaskId());
    qry.execute ();
    qry.commit ();
    qry.close ();
    Log::log(0,"rebill ���̽���,task_id:%d.", GetTaskId() );
    
}



void CRebillTask::setTaskStep(char const* i_sState, long i_lTaskID)
{
    DEFINE_QUERY (qry);

    qry.setSQL (" update B_REBILL_TASK"
                " set TASK_STEP=:vStep,"
                " state_date=sysdate"
                " where task_id=:vTaskID");    

    qry.setParameter ("vStep", i_sState);
    qry.setParameter ("vTaskID",i_lTaskID);    
    qry.execute ();
    qry.commit ();
    qry.close ();

}


void CRebillTask::setTaskState(char const* i_sState, long i_lTaskID)
{
    DEFINE_QUERY (qry);

    qry.setSQL (" update B_REBILL_TASK"
                " set TASK_STATE=:vState,"
                " state_date=sysdate"
                " where task_id=:vTaskID");    

    qry.setParameter ("vState", i_sState);
    qry.setParameter ("vTaskID",i_lTaskID);    
    qry.execute ();
    qry.commit ();
    qry.close ();

}







//# ����������id ���嵥�������ȡ�� m_aoTableList ��
void CRebillTask::initTableNameList (int iBillingCycleID)
{
    int iPos = 0;
    
    DEFINE_QUERY (qry);
    
    qry.setSQL ("Select Distinct table_type,table_name,nvl(org_id,1) org_id,nvl(billflow_id,-1) billflow_id"
        " From b_inst_table_list "
        " Where billing_cycle_id = :vBCID or billing_cycle_id is null  Order By table_type");
    qry.setParameter ("vBCID", iBillingCycleID);
    qry.open ();
    
    while (qry.next()) {
        m_aoTableList[iPos].m_iTableType = qry.field("table_type").asInteger();
        strcpy (m_aoTableList[iPos].m_sTableName, qry.field("table_name").asString());
        m_aoTableList[iPos].m_iOrgID = qry.field("org_id").asInteger();
        m_aoTableList[iPos].m_iBillFlowID= qry.field("billflow_id").asInteger();
        iPos ++;
        
        if (iPos >= __MAX_TABLE_NUM__) {
            Log::log(1,"rebill Error");
            Log::log(1,"��������:%d�Ķ�Ӧ���¼������ϵͳ����%d",iBillingCycleID,__MAX_TABLE_NUM__);
            exit(0);
        }
    }
    
    m_aoTableList[iPos].m_iTableType = -1;
    
    qry.close();
}

//# ��ȡһ����ǰ�����͵� �����Լ�billflow��org_id ,�ظ��ı���,ֻȡ��һ�ε�����
bool  CRebillTask::getTableName ( int iTableType, 
    char sTabName[], int * o_iInstOrgID,  int * o_iInstFlowID )

{
    static int aiLastTabType = 0; //#���һ�β��ұ���������
    static int aiPos ; //#��ǰ�±�ƫ����
    static HashList<int> hiTableName(10);  /////������ֹ�ظ�ȡ
    int iTmp;
    
    if (iTableType <= 0)
        return false;
    
    if (iTableType != aiLastTabType) { //# ��һ�λ�ȡ���� iTableType �ı���
        aiLastTabType = iTableType;
        aiPos = 0;
        hiTableName.clearAll();
    }
    for ( ; m_aoTableList[aiPos].m_iTableType!=-1; aiPos++) {
        if (m_aoTableList[aiPos].m_iTableType == iTableType 
            &&  hiTableName.get(m_aoTableList[aiPos].m_sTableName, &iTmp)==false )
        {
            strcpy (sTabName, m_aoTableList[aiPos].m_sTableName);
            *o_iInstOrgID=m_aoTableList[aiPos].m_iOrgID;
            *o_iInstFlowID=m_aoTableList[aiPos].m_iBillFlowID;
            
            hiTableName.add(m_aoTableList[aiPos].m_sTableName, 1);
            aiPos ++;
            return true;
        }
        if (m_aoTableList[aiPos].m_iTableType > iTableType)
            break;
    }

    hiTableName.clearAll();
    return false;
}

//# ��ȡһ����ǰ�����͵� �����Լ�billflow��org_id ,�ظ��ı���,ֻȡ��һ�ε�����
bool  CRebillTask::getTableName (  int iTableType, long i_lServID, 
    char sTabName[],  int * o_iInstOrgID,  int * o_iInstFlowID  )
{
    Serv serv;
    Date d;
    
    static int aiLastTabType = 0; //#���һ�β��ұ���������
    static int aiPos ; //#��ǰ�±�ƫ����
    static OrgMgr * pOrgMgr=NULL;
    static HashList<int> hiTableName(10);  /////������ֹ�ظ�ȡ
    int iTmp;


    if(pOrgMgr==NULL) pOrgMgr=new OrgMgr;

    if (!G_PUSERINFO->getServ (serv, i_lServID, d.toString ())) 
        return false;
    
    if (iTableType <= 0)
        return false;
    
    if (iTableType != aiLastTabType)
        { //# ��һ�λ�ȡ���� iTableType �ı���
        aiLastTabType = iTableType;
        aiPos = 0;
        hiTableName.clearAll();
    }
    
    for ( ; m_aoTableList[aiPos].m_iTableType!=-1; aiPos++)
        {
        if (m_aoTableList[aiPos].m_iTableType == iTableType
                &&  pOrgMgr->getBelong(serv.getOrgID(),m_aoTableList[aiPos].m_iOrgID)
                &&  hiTableName.get(m_aoTableList[aiPos].m_sTableName, &iTmp)==false
                ) 
            {
            strcpy (sTabName, m_aoTableList[aiPos].m_sTableName);
            *o_iInstOrgID=m_aoTableList[aiPos].m_iOrgID;
            *o_iInstFlowID=m_aoTableList[aiPos].m_iBillFlowID;
            
            hiTableName.add(m_aoTableList[aiPos].m_sTableName, 1);
            aiPos ++;
            return true;
            }
        if (m_aoTableList[aiPos].m_iTableType > iTableType)
            break;
    }   

    if(pOrgMgr!=NULL) 
        {
        delete pOrgMgr;
        pOrgMgr=NULL;
        }
    hiTableName.clearAll();
    return false;
}



int CRebillTask::getRollBackTableType( int i_iTableType)
{
    static bool bInit=true;
    HashList<int> hRBTable(5); 
    int iRBTable;
    
    if(bInit==true)
        {
        TOCIQuery Qry(DB_LINK);
        Qry.setSQL("select event_table_type_id,rb_table_type_id"
            " from b_event_table_type"
            " where rb_table_type_id is not null ");
        Qry.open();
        while(Qry.next())
            {
            hRBTable.add(Qry.field(0).asInteger(),  Qry.field(1).asInteger());
            }
        bInit=false;
        }
    if(hRBTable.get(i_iTableType, &iRBTable)==true)
        return iRBTable;

    return i_iTableType;
}


//���û�����
void CRebillTask::rollbackServ()
{

///�����ڴ�
    rollbackAggr();

/////���˵�set and cycle
    rollbackEventList();
    
}



//���嵥����
void CRebillTask::rollbackEvent()
{
///�����ڴ�
    rollbackAggr();
 ////�����嵥
    rollbackEventList();

}


////���ļ�����
void CRebillTask::rollbackFile()
{

///�����ڴ�
    rollbackAggr();
 ////�����嵥
    rollbackEventList();
 
}



/////�����嵥,����д���ļ�
void CRebillTask::rollbackEventList()
{
    CRebillTable* pCRebillTable;    
    bool bSkip=false;

    HashList<CRebillTable*>::Iteration iterRebill= m_hpCRebillTable->getIteration();


    while ( iterRebill.next(pCRebillTable) )
    {
        //���˵��ۻ���ı����ͺ���֪�Ĳ���Ҫ���˵ı�����
        bSkip=false;
        switch(pCRebillTable->m_iTableType)
            {
            case TABLE_TYPE_EVENT_AGGR :
            case TABLE_TYPE_ACCT_ITEM_AGGR :
            case TABLE_TYPE_DAILY_BK_ACCT_ITEM_AGGR :
            case TABLE_TYPE_EVENT_AGGR_RB :
            case TABLE_TYPE_ACCT_ITEM_AGGR_RB :
            case TABLE_TYPE_ENDFILE :
            case TABLE_TYPE_CALL_EVENT_PRICE :
            case TABLE_TYPE_DATA_EVENT_PRICE :
            case TABLE_TYPE_VALUE_EVENT_PRICE :
            case TABLE_TYPE_CYCLE_EVENT_PRICE :
            case TABLE_TYPE_SET_FEE_EVENT_PRICE :
            case TABLE_TYPE_EVENT_PRICE :
            case TABLE_TYPE_CYCLE_INST :
            case TABLE_TYPE_ACCT_DISCT_DETAIL :
            case TABLE_TYPE_ACCT_DISCT_DETAIL_RB :
            case TABLE_TYPE_ACCT_DISCT_AGGR :     
                   bSkip=true;
                   
            default :
                break;
            }
        
        if(bSkip)
            continue;
//////�������Ҫ���ͳ�ȥ��,�ڷ��͵�ʱ��д�����ļ�,����Ҳ����д�����ļ�
        if( m_bWaitGatherFile ==false )
            {////����
            for(int iTmp=0; iTmp < m_viResendEventTable.size();  iTmp++)
                {
                if(m_viResendEventTable[iTmp] == pCRebillTable->m_iTableType)
                    {
                    bSkip=true;
                    break;
                    }
                }

            if(bSkip)
                continue;

            }

//////////������,�õ���Ҫ���˵��嵥������
        
        ////�����ڴ沢���    
        char* pEvtData=new char[pCRebillTable->getDataStructSize()];
        memset(pEvtData, 0, pCRebillTable->getDataStructSize());

        while(pCRebillTable->fetchEvent(
                pEvtData, GetTaskId(), ROLLBACK_FLAG)
                ==true
                )
        {
          if(pCRebillTable->m_iTableType == EVENT_TABLE_TYPE_SET_FEE_EVENT )
            {/////�ײͷѱ� �ֶ�̫����
            ((BaseEventData*)pEvtData)->strStdEvent.m_iEventTypeID=SET_FEE_EVENT_TYPE_ID;
            ((BaseEventData*)pEvtData)->strStdEvent.m_iBillingOrgID=pCRebillTable->m_iInstOrgID;
            }
            pCRebillTable->rollAndWriteDB(m_iEvtSType[m_iTaskType], pEvtData);
            pCRebillTable->setRebillFlag(ROLLBACK_FLAG);
        }        
        delete pEvtData;

    }

    

}

/////�����ڴ��е����ݣ�����д���ļ�
void CRebillTask::rollbackAggr()
{

    CRebillTable* pCRebillTable;


////�����ڴ��ۼ���
    int  aiRBAggrTableType[3];
aiRBAggrTableType[0]
    = m_iBakUseRB >0 ? TABLE_TYPE_EVENT_AGGR_RB :  TABLE_TYPE_EVENT_AGGR ;
aiRBAggrTableType[1]
    = m_iBakUseRB >0 ? TABLE_TYPE_EVENT_AGGR_RB :  TABLE_TYPE_ACCT_ITEM_AGGR ;
aiRBAggrTableType[2]
    = m_iBakUseRB >0 ? TABLE_TYPE_ACCT_DISCT_DETAIL_RB :  TABLE_TYPE_ACCT_DISCT_DETAIL ;

HashList<struct AcctDisctAggrData *> phProdOfferID(800);

if(m_hpCRebillTable ->get (TABLE_TYPE_ACCT_DISCT_AGGR , &pCRebillTable)==true)
    {
    char* pEvtData=new char[pCRebillTable->getDataStructSize()];
    while(pCRebillTable->fetchEvent(
            pEvtData, GetTaskId(), 0)
            ==true
            )
        {
        phProdOfferID.add(( (struct AcctDisctAggrData*) pEvtData)->lItemDisctSeq  ,
            (struct AcctDisctAggrData*)pEvtData);
        
        pEvtData=new char[pCRebillTable->getDataStructSize()]; 
        }
    delete [] pEvtData;
    } 


    for(int iTmp=0; iTmp< sizeof(aiRBAggrTableType)/sizeof(int) ; iTmp++  )
        {
        //
        if( m_hpCRebillTable->get(aiRBAggrTableType[iTmp], &pCRebillTable)==false)
            continue;

        ////�����ڴ沢���    
        char* pEvtData=new char[pCRebillTable->getDataStructSize()];
        //memset(pEvtData, 0, pCRebillTable->getDataStructSize());
        
        while(pCRebillTable->fetchEvent(
                pEvtData, GetTaskId(), ROLLBACK_FLAG)
                ==true
                )
            {
            if( aiRBAggrTableType[iTmp]== TABLE_TYPE_ACCT_DISCT_DETAIL_RB
                || aiRBAggrTableType[iTmp]== TABLE_TYPE_ACCT_DISCT_DETAIL )
                {
                struct AcctDisctAggrData* pTmp=NULL;
                phProdOfferID.get( ((struct AcctDisctDetailData* )pEvtData) ->lItemDisctSeq , &pTmp) ;
                pCRebillTable->rollAndWriteDB(m_iEvtSType[m_iTaskType], 
                    pEvtData,
                    (char *)pTmp);
                }
            else
                {
                pCRebillTable->rollAndWriteDB(m_iEvtSType[m_iTaskType], pEvtData);
                }
            pCRebillTable->setRebillFlag(ROLLBACK_FLAG);
            }        
        delete [] pEvtData;

        }

}


/////�����ڴ��е����ݣ�����д���ļ�
/////���Ǹ���b_rebill���������������,����ֱ��ʹ���ڴ��е����������˲�д�ļ�, �ٶȸ���
/////�ӿ�û��׼����,�Ȳ�ʹ���������
void CRebillTask::rollbackAggr_V2()
{
    long lServID;
    bool bOK=true;

    CRebillTaskServ::openTaskServ (this->GetTaskId ());
    
    while (1)
    {
        lServID = CRebillTaskServ::fetchServID ();
        if (lServID == 0)
        {
            break;
        }
        
        bOK=true;
        while(1)
            {
            if(bOK==false)
                {
                m_pDataIncrementMgr->rollback();
                m_pDataIncrementMgr->forceCommit();
                
                }
           
            ////���� acct_item_aggr
            struct AcctItemAddAggrData oAcctItemAggrData;
            vector<AcctItemData *> vpAcctItemData;
            bool bNew=false;
                
            memset(&oAcctItemAggrData, 0, sizeof(struct AcctItemAddAggrData));
            oAcctItemAggrData.iEventSourceType=m_iEvtSType[m_iTaskType];
            
            AcctItemAccuMgr* m_pAcctItemAccuMgr=new AcctItemAccuMgr();
            
            m_pAcctItemAccuMgr->getAll( lServID,   m_lBillingCycleID, vpAcctItemData);
            for(int iTmp=0; iTmp<vpAcctItemData.size(); iTmp++)
                {
                oAcctItemAggrData.iBillingCycleID=vpAcctItemData[iTmp]->m_iBillingCycleID;
                oAcctItemAggrData.iCharge=0-vpAcctItemData[iTmp]->m_iValue;
                oAcctItemAggrData.iItemSourceID=vpAcctItemData[iTmp]->m_iItemSourceID;
                oAcctItemAggrData.iOrgID=1;
                oAcctItemAggrData.iPayItemTypeID=vpAcctItemData[iTmp]->m_iPayItemTypeID;
                oAcctItemAggrData.iPayMeasure=vpAcctItemData[iTmp]->m_iPayMeasure;
                oAcctItemAggrData.lAcctID=vpAcctItemData[iTmp]->m_lAcctID;
                oAcctItemAggrData.lAcctItemID=vpAcctItemData[iTmp]->m_lAcctItemID;
                oAcctItemAggrData.lEventID=0;
                oAcctItemAggrData.lAcctID=vpAcctItemData[iTmp]->m_lAcctID;
                oAcctItemAggrData.lOfferInstID=vpAcctItemData[iTmp]->m_lOfferInstID;
                oAcctItemAggrData.lServID=vpAcctItemData[iTmp]->m_lServID;
                strncpy(oAcctItemAggrData.sStateDate, vpAcctItemData[iTmp]->m_sStateDate, 15);

                
                if(! m_pDataIncrementMgr->addAcctItemAddAggr( &oAcctItemAggrData ) )
                    {
                    bOK=false;
                    break;
                    }
                m_pAcctItemAccuMgr->update(&oAcctItemAggrData, bNew);
                }
                if(bOK==false)
                    continue;
                
            ////���� event_aggr
            ///�ӿ�û��׼����
            ///OfferAccuMgr
            ///��Ҫ�� acct_item_aggr �����ṩ�ӿڵ��������,

            ////���� acct_disct, acct_disct_aggr
            


            if(bOK==true)
                break;
            }


        
    }

    
    CRebillTaskServ::closeTaskServ();

    
}




//# ���͵�ǰ�¼���������Ϣ����
void CRebillTask::sendEvent(StdEvent* i_poStdEvent )
{
    static long s_lSumCnt = 0;

///�����°�  
/*    g_poEventSection->bindUserInfo(true);

    if (i_poStdEvent->m_iErrorID>=2000 && i_poStdEvent->m_iErrorID<3000)
    {
        i_poStdEvent->m_iEventTypeID = NOOWNER_EVENT_TYPE;
    }
*/
    ////���˵Ļ���
    i_poStdEvent->m_iEventSourceType=m_iEvtSType[m_iTaskType];
            
//=ǿ�Ƹı�file_id ���Ա�ӿ����۵�ƿ������----ͳ���Ѿ��޸�,����ʹ������file_id
//        i_poStdEvent->m_iFileID=m_iFileID;

    m_poSender->send( i_poStdEvent );
        
    if (!(++s_lSumCnt%10000)) {
        SEND_FORCE_COMMIT_EVENT();
    }

}



/////������task_id ��HIS ���е��嵥���ݷ��ͳ�ȥ
void CRebillTask::resendHisData()
{
    int iForSend;  ///�÷��͵��嵥
    char sStartDate[16]="20801231235959";   ////��ʼ�������Ƚ�ʱ��
    int iResendEventTypeNum=m_viResendEventTable.size();


    bool* bIfHave=new bool[iResendEventTypeNum];        
    CRebillTable** apCRebillTable=new CRebillTable*[iResendEventTypeNum];
    BaseEventData* m_poEventData=new BaseEventData[iResendEventTypeNum];
    
    //## �ӱ��ݱ���ȡ�嵥��¼����
    Pprintf (0,true,0,".. ��ʼ�����������¼�(TASK_ID: %ld) ...", this->GetTaskId());
    Log::log (0,".. ��ʼ�����������¼�(TASK_ID: %ld) ...", this->GetTaskId());

   
    for(int iTemp=0; iTemp<iResendEventTypeNum; iTemp++)
    {
        if(m_hpCRebillTable->get(
            m_viResendEventTable[iTemp], &(apCRebillTable[iTemp]))
            ==false)
            {
            bIfHave[iTemp]=false;
            continue;
            }
        
        bIfHave[iTemp]
            =apCRebillTable[iTemp]->fetchEvent(
                (char *)&(m_poEventData[iTemp]), 
                this->GetTaskId(),
                SEND_SERV_FLAG,
                "order by start_date" );
        
    }

    while (1)
    {
        iForSend=-1;
        
        for(int iTemp=0;iTemp<iResendEventTypeNum; iTemp++)
        {
            
            if( bIfHave[iTemp]==true)
                {
                if( iForSend==-1)
                    {
                    iForSend=iTemp;
                     strncpy(sStartDate, m_poEventData[iTemp].strStdEvent.m_sStateDate, 15);
                    }
                else  if(strncmp(m_poEventData[iTemp].strStdEvent.m_sStateDate, sStartDate, 15)<=0 )
                    {
                    iForSend=iTemp;
                    strncpy(sStartDate, m_poEventData[iTemp].strStdEvent.m_sStateDate, 15);
                    }
                }

        }
        if(iForSend==-1)
            break;/////û�пɷ��͵��¼���
                                
        g_poEventSection=(EventSection *)&(m_poEventData[iForSend].strStdEvent);
        
          
        ////�Լ�д���ļ� �û�������
        m_poEventData[iForSend].strStdEvent.m_iEventSourceType=m_iEvtSType[m_iTaskType];
        while(false == m_pDataIncrementMgr->addEvent(
                &(m_poEventData[iForSend].strStdEvent), DATAINC_OP_CHAR_DELETE)
                )
            {
            m_pDataIncrementMgr->rollback();
            m_pDataIncrementMgr->forceCommit();
            if(m_pDataIncrementMgr->ifCanWrite()==false)
                usleep(200000);
            }
        ////ʹ���µ�eventID 
        m_poEventData[iForSend].strStdEvent.m_lEventID = EventIDMaker::getNextEventID ();
        ////���͸����ν���,���ͳɹ����ύ
        sendEvent( &(m_poEventData[iForSend].strStdEvent) );            
        m_pDataIncrementMgr->commit();
        m_iLastFileID=m_poEventData[iForSend].strStdEvent.m_iFileID;
        apCRebillTable[iForSend]->setRebillFlag(SEND_SERV_FLAG);

    
        bIfHave[iForSend]
            =apCRebillTable[iForSend]->fetchEvent(
                (char*)&(m_poEventData[iForSend]),  
                this->GetTaskId(),
                SEND_SERV_FLAG,
                "order by start_date" );

    }   

    ///�������
    //## ���� ǿ���ύ�ź�
    ///�������ύ
    {
    struct EndFileData strEndFile;
    Date d;
    strEndFile.cOperation='i';
    strEndFile.iBillFlowID=Process::m_iFlowID;
    strEndFile.iBillingCycleID=GetBillingCycleId();
    strEndFile.iEventSourceType=m_iEvtSType[m_iTaskType];
    strEndFile.iFileID=m_iFileID;
    strEndFile.iOrgID=0;
    strcpy(strEndFile.sEndTime, d.toString() );
    while( m_pDataIncrementMgr->addEndFile(&strEndFile) ==false)
        {
        m_pDataIncrementMgr->rollback();
        m_pDataIncrementMgr->forceCommit();
        if(m_pDataIncrementMgr->ifCanWrite()==false)
            usleep(200000);        
        }
    
    }
    m_pDataIncrementMgr->forceCommit();

if(m_iTaskType == REBILL_TASK_TYPE_SERV )
    {
////ʹ��һ�������¼�������,
    for(int iTemp=0;iTemp<iResendEventTypeNum; iTemp++)
        {
        if(apCRebillTable[iTemp]->getFetchNum() >0 )
            {
            m_poEventData[iTemp].strStdEvent.m_iFileID= m_iFileID;
            m_poEventData[iTemp].strStdEvent.m_iEventTypeID = DISCARD_EVENT_TYPE_ID; 
            m_poSender->send (&m_poEventData[iTemp].strStdEvent);
            SEND_FILE_END_EVENT(m_iFileID);
            break;
            }
        }
    }

    m_iLastFileID=m_iFileID;
    SEND_FORCE_COMMIT_EVENT();



    for(int iTemp=0; iTemp<iResendEventTypeNum; iTemp++)
    {
        Pprintf (0,true,0,"## %s rebill��ȡ�¼���:%ld",
            apCRebillTable[iTemp]->m_sTableRemark, 
            apCRebillTable[iTemp]->getFetchNum() );
        Log::log (0,"## %s rebill��ȡ�¼���:%ld", 
            apCRebillTable[iTemp]->m_sTableRemark,
            apCRebillTable[iTemp]->getFetchNum());
        
    }


    DEFINE_QUERY (qry);
    char sSql[150];
    snprintf(sSql, 150, 
        " update b_rebill_task set last_file_id =%d"
        " where task_id =%ld",
        m_iFileID,
        GetTaskId()
        );
    qry.setSQL(sSql);
    qry.execute();
    qry.commit();


    delete [] bIfHave;
    delete []  apCRebillTable;
    delete []  m_poEventData; 


    
}




/////������Ѻ��ײ�
////��Ҫͬ�����±�: wf_application

void CRebillTask::execRentAndSetFee ()
{
    char *pLogName = getenv("LOGNAME");
    char sHostName[500];
    
    int iLocalHostID = IpcKeyMgr::getHostInfo(sHostName);
    if (iLocalHostID == -1) 
        iLocalHostID = 1;


    DEFINE_QUERY (qry);
    
    char sSQL[2048];

//////����-n����, ȡ�� -n ������
    snprintf (sSQL,2048,
        " Select a.process_id,a.billflow_id, nvl(a.exec_path,'') exec_path, "
        " b.exec_name||' -t %ld '|| "
        "   substr(parameter, instr(parameter, '-n'),"
        "    instr(parameter||' ',' ',instr(parameter,'-n'),2) - instr(parameter, '-n')  )"
        "  exec_name"
        " from wf_process a, wf_application b, wf_billflow c "
        " where a.app_id = b.app_id and a.billflow_id = c.billflow_id "
        " And b.app_id  In (5,6) "
        " and host_id=%d and sys_username='%s'"
        " order by a.process_id ",
        this->GetTaskId () ,
        iLocalHostID,
        pLogName);


    qry.close ();
    qry.setSQL (sSQL);
    qry.open ();
    
    while (qry.next()) {
        int  iBillFlowID, iProcID;
        char sCmd[100], sPath[100];
        
        iBillFlowID = qry.field ("billflow_id").asInteger ();
        iProcID = qry.field ("process_id").asInteger ();
        strcpy (sCmd, qry.field ("exec_name").asString ());
        strcpy (sPath, qry.field ("exec_path").asString ());
        

        Log::log (0,"-- %s ������̨����: process_id(%d), billflow_id(%d), ����ָ��[ %s ]", g_argv[0],iProcID, iBillFlowID, sCmd);
        Pprintf (0,true,0,"-- %s ������̨����: process_id(%d), billflow_id(%d), ����ָ��[ %s ]", g_argv[0],iProcID, iBillFlowID, sCmd );        
        startCommand (iBillFlowID, iProcID, sCmd, sPath);
        
        sleep(2);
    }
    qry.close ();
    Log::log (0,"-- ������%s �����ĺ�̨����ִ�����, �뵽��̨���ȷ�ϣ�[�ɸ��ݽ�����־]", g_argv[0]);
}




// ��������
void CRebillTask::startCommand (int _iBillFlowID, int _iProcID, char _sCmd[], char _sPath[])
{
    int iPos = 0, iForkRet = 0, iForkGrandSonRet = 0;

    char sPath[80], sCommand[80], sExecName[80];
    char sString[80], sString2[80];
    char *argv[20];
    char *pt;
    
    strcpy (sPath,_sPath);  
    strcpy (sExecName,_sCmd);
    
    pt = strtok (sExecName," \n");
    argv[iPos++] = strdup(pt);  
    strcpy (sCommand,pt);
    while ((pt = strtok (NULL," \n")) != NULL){
        argv[iPos] = strdup(pt);
        iPos++;
    }
    argv[iPos] = NULL;
    
    //## �����ӽ���
    iForkRet = fork(); 
    switch (iForkRet){
        
        case 0:
            usleep(10000);
            
            //## ���������
            iForkGrandSonRet = fork();
            
            switch (iForkGrandSonRet) {
                
                case 0:
                    if (strlen (sPath)) {
                        chdir (sPath);
                    }
                        
                    usleep(10000);
                    Environment::getDBConn(true); //���ݿ�Ҫ���ӽ�����Ҫ�����������ݿ�
                    
                    sprintf (sString,"PROCESS_ID=%d", _iProcID);
                    putenv (sString);           
                    sprintf (sString2,"BILLFLOW_ID=%d", _iBillFlowID);
                    putenv (sString2);
                    
                    execve (sCommand,argv,environ);
                    
                    exit(0);
                    break;
                
                case -1:
                    perror("fork GrandSon");
                    exit (0);
                    break;
                    
                default:
                    exit (0); 
                    break;
            }
            
            exit (0); //�ӽ����˳�
            break;
            
        case -1:
            perror ("fork Son");
            Pprintf (0,true,0,"start_Command: fork son err.");
            Log::log (0, "start_Command: fork son err.");
            break;
            
        default:
            //## ���������ӽ��� SIGCHLD �ź�
            waitpid (iForkRet, NULL, 0);
            
            //## �����������������ݿ�
            Environment::getDBConn(true);   
            break;
    }
    
    for (int i=0; i<iPos; i++) {
        if (argv[i]) { 
            cout<<"argv["<<i<<"] = "<<argv[i]<<endl; 
            free (argv[i]);
        }
        else break;
    }
}



void CRebillTask::waitForTaskStep(char* i_sStep,  long i_lTaskID)
{
    TOCIQuery qry(DB_LINK);

    while(1)
    {
        qry.setSQL ("select TASK_STEP from B_REBILL_TASK"
                    " where task_id=:vTaskID"
                    " and task_step=:vTaskStep");    

        qry.setParameter ("vTaskStep", i_sStep);
        qry.setParameter ("vTaskID", i_lTaskID);

        qry.open ();
        if(qry.next()==true)
            break;
        qry.close ();
        sleep(5);
    }

    
}


void CRebillTask::waitForBlockServ()
{

    if(m_bBlockServ!=true)
        {
        return ; 
        }

    Log::log(0, "��ʼ���ͻ�ѹ�������������...");

    TOCIQuery Qry(DB_LINK);
    TOCIQuery Qry_ins(DB_LINK);

    Qry.setSQL("select task_step from B_REBILL_TASK where task_id=:vTaskID");
    Qry.setParameter("vTaskID",  m_lTaskID[REBILL_TASK_TYPE_SERV]);
    Qry.open();
    
    if(Qry.next()==true  && strcmp("FLT", Qry.field(0).asString()  ) ==0)
        {///�Ѿ�����˻�ѹ
        return ;
        }
    
    Qry.close();

    
    ////��ȡ����process_id
    char sTmp[512];
    sprintf(sTmp, 
        " insert into b_serv_block_trigger(PROCESS_ID,STATE,STATE_DATE,PROCESS_STATE,REBILL_TASK_ID)"
        " select process_id,'RDY',sysdate,0,%d from wf_process"
        " where app_id=3"
        " and process_id not in "
        " (select process_id from b_serv_block_trigger where state in ('RDY','RUN','END') )",
        m_lTaskID[REBILL_TASK_TYPE_SERV]);
    
    Qry_ins.setSQL(sTmp);
    Qry_ins.execute();
    Qry_ins.commit();
    Qry_ins.close();

    setTaskStep("WAT", m_lTaskID[REBILL_TASK_TYPE_SERV]);
    
    Qry.setSQL("select distinct process_id from b_serv_block_trigger");
    Qry_ins.setSQL(
        " update b_serv_block_trigger"
        " set PROCESS_STATE=:vProcState, STATE_DATE=sysdate"
        " where PROCESS_ID=:vProcID and state='RDY'");
    Qry.open();


    ////������ѭ���ȴ�gdb����
    //while(1)
    //    sleep(1);
    
    while (Qry.next() )
        {
        int iProcID=Qry.field(0).asInteger(); 
        Qry_ins.setParameter("vProcID",  iProcID );
        
        ////SetServBlockFlag �ǿ���ṩ�ĺ���
        
        if( SetServBlockFlag(REBILL_SERV_BLOCK, iProcID )==false )
            {
            if (CheckProcAlive(Qry.field(0).asInteger() )==true)
                {Log::log(0, "����û�з����ܹ����ڴ�,��Ȼ��������...");
                 THROW(MBC_Rebill+2);
                }
            Qry_ins.setParameter("vProcState", 1);
            }
        else
            {//
            if (CheckProcAlive(Qry.field(0).asInteger() )==true)
                {//����Ŀǰ������
                Qry_ins.setParameter("vProcState", 0);      
                }
            else
                {///������������
                Qry_ins.setParameter("vProcState", 1);
             
                }
            }
        
        Qry_ins.execute();   
        }

    Qry_ins.commit();
    
    Log::log(0, "�ȴ�����������Ӧ��ѹ�û�...");

while(1)
{///�ȴ�������Ӧ��ѹ�û�
    Qry.close();
    Qry.setSQL("select process_id from b_serv_block_trigger"
        " where state='RDY' and process_state=0");
    Qry.open();
    if(Qry.next() ==false )
        break;

    do
        {
          if(CheckProcAlive(Qry.field(0).asInteger() )==false) 
            {////������������
            Qry_ins.setParameter("vProcID", Qry.field(0).asInteger() );
            Qry_ins.setParameter("vProcState", 1);
            Qry_ins.execute(); 
            }
          
        } while(Qry.next());

    sleep(10);
    
}      

        setTaskStep("FLT", m_lTaskID[REBILL_TASK_TYPE_SERV]);
        
}

void CRebillTask::waitForFree()
{
    if(m_bBlockServ==true)
        {
    Log::log(0, "����������췢���ͷŻ�������...");

    TOCIQuery Qry(DB_LINK);

    Qry.setSQL("select task_step from B_REBILL_TASK where task_id=:vTaskID");
    Qry.setParameter("vTaskID",  m_lTaskID[REBILL_TASK_TYPE_SERV]);
    Qry.open();
    
    if(Qry.next()==true  && strcmp("FRE", Qry.field(0).asString()  ) ==0)
        {///�Ѿ�������ͷ�
        return ;
        }

    ////�Ѷ�Ӧ�İ��û����˵�������ΪTHR ״̬
    setTaskStep("THR", m_lTaskID[REBILL_TASK_TYPE_SERV]);
    

    Log::log(0, "�ȴ����������ͷŻ�ѹ�û��Ļ���...");
    while(1)
        {

        Qry.close();
        Qry.setSQL("select process_id from b_serv_block_trigger"
            " where state in ('RUN','RDY')");
        Qry.open();
        while(Qry.next() ==true)
            {
            SetServBlockFlag(REBILL_SERV_FREE, Qry.field(0).asInteger() );
            }

        ///�ȴ�������Ӧ�ͷŻ���
        Qry.close();
        Qry.setSQL("select process_id from b_serv_block_trigger"
            " where state  in ('RUN','THR') ");
        Qry.open();
        if(Qry.next() ==false )
            break;

        sleep(10);
            
        }      

    Qry.close();
///////����Щ������������ʼ�ն�û�������ĳ����״̬ȥ��
//    Qry.setSQL("update b_serv_block_trigger"
//        " set state='EXP'"
//        " where state ='RDY' and process_state=1");
//    Qry.execute();
//    Qry.commit();

    ///////�Ѵ�����¼��ɾ����
    Qry.setSQL("truncate table b_serv_block_trigger");
    Qry.execute();
    Qry.commit();


    setTaskStep("FRE", m_lTaskID[REBILL_TASK_TYPE_SERV]);

    Log::log(0, "�ͷ����...");
    
        }

}



void CRebillTask::backupRebillServID()
{
    long lServID;
    char sTname[100];
    int iInstOrgID=0;
    int iInstFlowID=0;

    int iTableTypeTmp=( m_iBakUseRB>0 ? TABLE_TYPE_EVENT_AGGR_RB : TABLE_TYPE_EVENT_AGGR);
    HashList<int> hsTnameTmp(10);
    int iTmp;
    
    int i=0;  ////serv_id�ĸ���
    
    CRebillTaskServ::openTaskServ (this->GetTaskId ());
    Log::log (0,"��ȡ���������ݿ��� (task_id : %ld) �������¼�", this->GetTaskId());
    Pprintf (0,true,0,"��ȡ���������ݿ��� (task_id : %ld) �������¼�", this->GetTaskId());


    while (1)
    {
        lServID = CRebillTaskServ::fetchServID ();
        if (lServID == 0)
        {
            break;
        }
        printf("\r�Ѵ�����[%d]��SERV_ID,���ڴ����SERV_IDΪ[%ld]",i,lServID);
        fflush(stdout);

        HashList<CRebillTable*>::Iteration iterCRebillTable=
            m_hpCRebillTable->getIteration();
        CRebillTable* pCRebillTable;
        while( iterCRebillTable.next(pCRebillTable)==true )
            {
            if(pCRebillTable->getTableType() == TABLE_TYPE_ACCT_DISCT_AGGR)
                {///////�� TABLE_TYPE_ACCT_DISCT_AGGR �������Ĺ켣��֮����
                continue;
                }
            while (getTableName (pCRebillTable->getTableType(), lServID,
                    sTname,  &iInstOrgID,  &iInstFlowID))
                {
                pCRebillTable->backupRebillServ(
                    lServID, this->GetTaskId(), m_iBakUseRB, sTname, iInstOrgID, iInstFlowID);
                  /////�嵥�ۼ���,����offer_inst_id ��ȫ������
                if( m_bCheckGroup ==true 
                    && pCRebillTable->getTableType()== iTableTypeTmp
                    && hsTnameTmp.get(sTname, &iTmp)==false )
                    {
                    hsTnameTmp.add(sTname, 1);
                    pCRebillTable->backupByOfferInst( this->GetTaskId(), sTname, iInstOrgID, iInstFlowID);
                    }                
                }
            }

        HashList<CRebillTable*>::Iteration iterCRebillTable2=
            m_hpCRebillTable->getIteration();
        while( iterCRebillTable2.next(pCRebillTable)==true )
            {
            if(pCRebillTable->getTableType() == TABLE_TYPE_ACCT_DISCT_AGGR)
                {
                while (getTableName (pCRebillTable->getTableType(), lServID,
                    sTname,  &iInstOrgID,  &iInstFlowID))
                    {
                    pCRebillTable->backupRebillServ(
                        lServID, this->GetTaskId(), m_iBakUseRB, sTname, iInstOrgID, iInstFlowID);
                    }

                }
            }
    
        ++i;
    }

    
    ////���е��û����ݶ����ݺ���֮��� �ύ��������setTaskStep ����ȥ��
    setTaskStep(m_asRebillServStepStr[m_iTaskStep+1], m_lTaskID[REBILL_TASK_TYPE_SERV]);

    CRebillTaskServ::closeTaskServ();
    printf("\n");
    Log::log (0,"��ȡ (task_id : %ld) �������¼����, �������� (%d) ��SERV_ID", 
        this->GetTaskId(),  i  );
    Pprintf (0,true,0,"��ȡ (task_id : %ld) �������¼����, �������� (%d) ��SERV_ID ", 
        this->GetTaskId(),  i  );
}




void CRebillTask::checkParameter ()
{
    BillingCycleMgr bcm;
    Date d;
    
    BillingCycle * pCycle = bcm.getBillingCycle (m_iBillingCycleTypeID, d.toString());
    
    if (pCycle->getBillingCycleID () != this->GetBillingCycleId() ) {
        Pprintf (0,1,0, "����������[��ǰ����(%ld)������������(%ld)-����(%ld) �� ϵͳ��ǰ����(%ld)��һ��]",
            this->GetTaskId(), m_iBillingCycleTypeID, m_lBillingCycleID, pCycle->getBillingCycleID ());
        Log::log (0, "����������[��ǰ����(%ld)������������(%ld)-����(%ld) �� ϵͳ��ǰ����(%ld)��һ��]",
            this->GetTaskId(), m_iBillingCycleTypeID, m_lBillingCycleID, pCycle->getBillingCycleID ());
        THROW (12345);
    }
}



int CRebillTask::run()
{

    int  iTemp=0;
    long  lPreBillingCycleID=0;

    //m_bStepRebill=true;
    m_bStepRebill = false;
    m_bBlockServ=true;
    m_bWaitGatherFile=false;
    m_bCheckGroup=true;

    m_iBakUseRB=0;

    m_iLastFileID=0;
    m_iFileID=0;
    
    m_lTaskID[0]=m_lTaskID[1]=m_lTaskID[2]=0;
    
    signal(SIGPIPE, SIG_IGN);
    
    for(int i=1; i<g_argc; i+=2)
        {
        
        ////�Ƿ��ѹ�û�����
        if(strcmp("-block", g_argv[i])==0 )
            {
            m_bBlockServ=atoi( g_argv[i+1])>0 ? true : false;
            continue;
            }

        
        ///step=0 ģʽ���ڵ�һ�����֮��ֱ�����ڶ���
        /// step=1 ģʽ���ڵ�һ�����֮��ͣ��
        if(strcmp("-step", g_argv[i])==0 )
            {
            m_bStepRebill=atoi( g_argv[i+1])>0 ? true : false;
            continue;
            }   

        //// ���½��ܲɼ��ļ� ����ֱ�Ӱ�ϵͳ�Ļ���ֱ�����·���
        /////resend Ϊ1  , waitGather �෴,��0 
        if(strcmp("-resend", g_argv[i])==0 )
            {
            m_bWaitGatherFile=atoi( g_argv[i+1])>0 ? false :  true ;
            continue;
            }        

        ////groupģʽΪ�����û�Ⱥ��ȡ���Ӱ���servID
        if(strcmp("-group", g_argv[i])==0 )
            {
            m_bCheckGroup=atoi( g_argv[i+1])>0 ? true : false;
            continue;
            }   

        //// 
        if(strcmp("-useRB", g_argv[i])==0 )
            {
            m_iBakUseRB=atoi( g_argv[i+1])>0 ? 1 : 0;
            continue;
            }   
        
        }

Log::log(0, "��������:block %d, step %d, resend %d, group %d, useRB %d",
    m_bBlockServ, m_bStepRebill, ( !m_bWaitGatherFile) , m_bCheckGroup, m_iBakUseRB);


    while(1)
        {

        if (!this->fetchRebillTask( m_lTaskID[ REBILL_TASK_TYPE_SERV ] )  )
            { //# �޴����������
            Log::log (0,"�޴������������������");
            Pprintf (0,0,0,"�޴������������������.");
            
            return 0;           // ���û����Ҫ�����۵��������˳�
            }

        char sTmp[20];
            
        switch(m_iTaskType)
            {
            case 0:
                strcpy(sTmp, "���û�����");
                break;
            case 1:            
                strcpy(sTmp, "���ļ�����");
                break;
            case 2:            
                strcpy(sTmp, "����������");
                break;

           default :
                break;
            }
        
            
            
        Log::log (0,"��ȡ������������,%s(task_id : %ld)", sTmp, this->GetTaskId());
        Pprintf (0,true,0,"��ȡ������������,%s(task_id : %ld)",sTmp,  this->GetTaskId());
        


        //## ���ص�ǰ�����Ӧ���ڵı����б�
        if(lPreBillingCycleID !=  this->GetBillingCycleId() )
            {
            //## �����������ĺϷ���
            checkParameter (); 
            initTableNameList ( this->GetBillingCycleId() );
            }
        lPreBillingCycleID= this->GetBillingCycleId() ;


        switch(m_iTaskType)
            {
            case REBILL_TASK_TYPE_SERV:
                runRebillServ();
                break;
            case REBILL_TASK_TYPE_FILE:
                runRebillFile();
                break;
            case REBILL_TASK_TYPE_EVENT:
                runRebillEvent();
                break;            
            }

      
            ///������ǰ��û�����, ������һ�����û����˵�����
            if(m_iTaskType!=REBILL_TASK_TYPE_SERV)
            {
                /////���ֻ�ѹ״̬, �����ð��û�����ֱ����backup����
                if( m_bStepRebill==false)
                {
                     setTaskStep("BAK", m_lTaskID[REBILL_TASK_TYPE_SERV]);
                }
                else
                {
                    break;
                }
            }      

        }////////whileѭ������

    return 0;
}



int CRebillTask::runRebillServ()
{

    Log::log(0,  "\n[����] ���û����˻ᵼ��������ײͷѵ�����,\n"
        "���,�뱣֤�ڰ��û������ڼ䲻Ҫ���������������\n"
        "��ȷ��crontab��������ײ�����ע�͵�");

char sTmp[100];
snprintf(sTmp, 100, "rebill serv [taskid %d] begin at %s",
    m_lTaskID[REBILL_TASK_TYPE_SERV], 
    m_asRebillServStepStr[m_iTaskStep]
    );

m_iFileID = VirtualFileMaker::makeVirtualFile (sTmp , Process::m_iProcID);
VirtualFileMaker::setFileState(m_iFileID, (char*)"BEG");



    
while(m_iTaskStep>=0 && m_iTaskStep<CREBILL_SERV_STEP_NUM-1)
    {
    
    switch(m_aiRebillServStepID[m_iTaskStep])
        {
        case  REBILL_TASK_STEP_BEGIN:

            prepareRebillServ();
            break;
        
        case  REBILL_TASK_STEP_WAITING:
            waitForBlockServ(); ///�ȴ���ѹ�û��Ļ���
            break;

        case  REBILL_TASK_STEP_FILTER:
            //���ڻ�ѹ�Ļ�,ֱ��������Ĳ��账��

            break;

        case  REBILL_TASK_STEP_BACKUPSERV:
            {
            //## �嵥,�ۻ��� ����ת��             
            // ��"�û���"�Ĵ���ʽ
            backupRebillServID();
            }
            break;
            
        case  REBILL_TASK_STEP_ROLLBACKSERV:
            {
            Pprintf (0,true,0,".. ��ʼ���� ��ǰ�����Ӧ���û��ۻ�������(TASK_ID: %ld) ...",
                m_lTaskID[REBILL_TASK_TYPE_SERV] );
            Log::log (0,".. ��ʼ���� ��ǰ�����Ӧ���û��ۻ�������(TASK_ID: %ld) ...",
                m_lTaskID[REBILL_TASK_TYPE_SERV] );
            
            rollbackServ();
            }
            break;

        
        case  REBILL_TASK_STEP_SENDSERV:

            resendHisData();    ////�����嵥
            break;

        case REBILL_TASK_STEP_RENTSET:
            {
            /////���� ��Ѻ��ײͳ���
            this->execRentAndSetFee();
          
            Pprintf (0,true,0,"## ����������(task_id: %ld)��Ѻ��ײʹ������ ��",
                m_lTaskID[REBILL_TASK_TYPE_SERV] );
            Log::log (0,"## ����������(task_id: %ld)��Ѻ��ײʹ������ ��", 
                m_lTaskID[REBILL_TASK_TYPE_SERV] );
          
            }

            break;

        case REBILL_TASK_STEP_THROUGH:
            waitForFree();//�ȴ��ͷ����ѹ�Ļ���
            break;
            
        case REBILL_TASK_STEP_FREE:
            //�����ͷ�����,���Խ�������������
            ////�ȴ����
            waitDB( );
            
            break;
            
        default :

            break;
            
        }
    
    m_iTaskStep++;
    setTaskStep(m_asRebillServStepStr[m_iTaskStep], m_lTaskID[REBILL_TASK_TYPE_SERV] );
    
    }

    finishRebillTask();

    VirtualFileMaker::setFileState(m_iFileID, (char*)"END");
    Log::log(0,  "\n[����] ��ȷ��crontab��������ײ�����ָ����� ");


return 0;


}


int CRebillTask::runRebillFile()
{
    char sTmp[100];
    snprintf(sTmp, 100, "rebill file [taskid %d] begin at %s",
        m_lTaskID[REBILL_TASK_TYPE_FILE], 
        m_asRebillFileStepStr[m_iTaskStep]
        );

    m_iFileID = VirtualFileMaker::makeVirtualFile (sTmp , Process::m_iProcID);
    VirtualFileMaker::setFileState(m_iFileID, (char*)"BEG");    
    
    while(m_iTaskStep>=0 && m_iTaskStep<CREBILL_FILE_STEP_NUM-1)
    {
    
        switch(m_aiRebillFileStepID[m_iTaskStep])
        {
        case  REBILL_TASK_STEP_BEGIN:

            break;

        case  REBILL_TASK_STEP_FILESCAN:
            fileScan();
            if(m_bBlockServ==true)
            {
                prepareRebillServ();////��ͬ�����û�Ⱥ�Ķ��ҳ���
            }
            break;
        
        case  REBILL_TASK_STEP_BLOCKSERV:
            waitForBlockServ(); ///���û��ĺ��������Ȳ���������ϵͳ

            break;

        case  REBILL_TASK_STEP_CLRDUP:
            clearDupInfo();
            break;

        case  REBILL_TASK_STEP_BACKUPFILE:
            backupRebillFile();
            break;
            
        case  REBILL_TASK_STEP_ROLLBACKFILE:
            rollbackFile();
            break;

        
        case  REBILL_TASK_STEP_RESENDFILE:
            ////Ҫô�Ӳɼ����½����ļ�,Ҫô�Ѹոջ��˵��ļ��Ļ������·��ͳ�ȥ
            if(m_bWaitGatherFile==true)
                waitForResendFile();
            else
                resendHisData();
            
            if(m_bStepRebill==false)
                {//ֻ�����ļ����˵Ļ�,����ֱ�Ӱѻ�ѹ�Ļ����ų���
                //����,ֱ�ӽ������ļ�����,���������û�����
                m_hiRebillStep->get((char *)"FRE", &m_iTaskStep);
                }            
            break;

        case REBILL_TASK_STEP_THROUGH:
            if(m_bStepRebill==true)
                waitForFree();
            
            break;

        case REBILL_TASK_STEP_FREE:
            ////����Ӧ�İ��û����˵������ΪRDY״̬
            if(m_bStepRebill==true)
                {////�ų�������,�Ͱѻ�ѹ��Ϊû�л�ѹ����
                setTaskStep("RDY", m_lTaskID[REBILL_TASK_TYPE_SERV]);
                }


            break;
            
        case REBILL_TASK_STEP_END:
            //////�ȴ�������

            waitDB( );

            
            if(m_bStepRebill==false)
            {////��Ҫֱ�������û�����
                Log::log(0, " �����ݿ����, ����׼���ܶ�Ӧ�İ��û�����... ");
            }

            break;
            
        default :

            break;
            
        }

    m_iTaskStep++;
    setTaskStep(m_asRebillFileStepStr[m_iTaskStep], GetTaskId());
    
    }

finishRebillTask();
setTaskState("RDY", m_lTaskID[REBILL_TASK_TYPE_SERV]);

    VirtualFileMaker::setFileState(m_iFileID, (char*)"END");

return 0;


}



int CRebillTask::runRebillEvent()
{
char sTmp[100];
snprintf(sTmp, 100, "rebill event [taskid %d] begin at %s",
    m_lTaskID[REBILL_TASK_TYPE_SERV], 
    m_asRebillEventStepStr[m_iTaskStep]
    );

m_iFileID = VirtualFileMaker::makeVirtualFile (sTmp , Process::m_iProcID);
    VirtualFileMaker::setFileState(m_iFileID, (char*)"BEG");    


while(m_iTaskStep>=0 && m_iTaskStep<CREBILL_EVENT_STEP_NUM-1)
    {
    
    switch(m_aiRebillEventStepID[m_iTaskStep])
        {
        case  REBILL_TASK_STEP_BEGIN:

            break;

        case  REBILL_TASK_STEP_BACKUPEVENT:
            backupRebillEvent();
            break;

        case  REBILL_TASK_STEP_HISSCAN:
            hisScan();
            if(m_bBlockServ==true)
                {
                prepareRebillServ();////��ͬ�����û�Ⱥ�Ķ��ҳ���
                }
            break;
        
        case  REBILL_TASK_STEP_BLOCKSERV:
            waitForBlockServ(); ///���û��ĺ��������Ȳ���������ϵͳ

            break;

        case  REBILL_TASK_STEP_CLRDUP:
            clearDupInfo();
            break;

        case  REBILL_TASK_STEP_ROLLBACKSERV:
            rollbackEvent();
            break;
            
        case  REBILL_TASK_STEP_SENDSERV:
            resendHisData();
            break;


        case REBILL_TASK_STEP_THROUGH:
            if(m_bStepRebill==true)
                waitForFree();
            break;

        case REBILL_TASK_STEP_FREE:
            ////����Ӧ�İ��û����˵������ΪRDY״̬
            if(m_bStepRebill==true)
                {/////��ѹ�Ļ����ų�����,���ǲ���ѹ��״̬��
                setTaskStep("RDY", m_lTaskID[REBILL_TASK_TYPE_SERV]);
                }
            
            break;
            
        case REBILL_TASK_STEP_END:
            //////�ȴ�������

            waitDB( );

            if(m_bStepRebill==false)
                {////��Ҫֱ�������û�����
                Log::log(0, " �����ݿ����, ����׼���ܶ�Ӧ�İ��û�����... ");
                }
            break;
            
        default :

            break;
            
        }

    m_iTaskStep++;
    setTaskStep(m_asRebillEventStepStr[m_iTaskStep], GetTaskId());
    
    }

finishRebillTask();
setTaskState("RDY", m_lTaskID[REBILL_TASK_TYPE_SERV]);

    VirtualFileMaker::setFileState(m_iFileID, (char*)"END");

return 0;


}












///����table_type  �� file_id ����ȡ��rebill���ļ���
bool  CRebillTask::getRebillFileName (char* o_sFileName, int i_iTableType, long i_lFileID)
{
    static int s_CursorOpenFlag = 0;
    static TOCIQuery* s_pQry;

    if (s_CursorOpenFlag == 0)
        {
        s_CursorOpenFlag=1;
        s_pQry=new TOCIQuery(DB_LINK);
        s_pQry->setSQL(
            " select a.file_path||'/'||a.file_name file_name"
            " from b_package_file_list a,b_file_package b"
            " where a.package_id=b.package_id"
            " and b.state in ('RDY','END')"
            " and a.file_id=:vFileID"
            " and a.table_type=:vTableType"
            " and a.state in ('RDY','END')"
            " and nvl(record_cnt,0)>0");
        s_pQry->setParameter("vTableType",i_iTableType);
        s_pQry->setParameter("vFileID",i_lFileID);
        
        s_pQry->open();
        }

    if(s_pQry->next() ==true)
        {
        strncpy(o_sFileName, s_pQry->field("file_name").asString(),  600);
        return true;
        }

    delete s_pQry;
    s_CursorOpenFlag=0;
    return false;

}



///���� file_id ����ȡ��rebill��  file_name �� table_type
bool  CRebillTask::getRebillFile (char* o_sFileName, int* o_iTableType, long i_lFileID)
{
    static int s_CursorOpenFlag = 0;
    static TOCIQuery* s_pQry;

    if (s_CursorOpenFlag == 0)
        {
        s_CursorOpenFlag=1;
        s_pQry=new TOCIQuery(DB_LINK);
        s_pQry->setSQL(
            " select a.file_id,a.table_type,a.file_path||'/'||a.file_name file_name"
            " from b_package_file_list a,b_file_package b"
            " where a.package_id=b.package_id"
            " and b.state in ('RDY','END')"
            " and a.file_id=:vFileID"
            " and a.state in ('RDY','END')"
            " and nvl(record_cnt,0)>0");
        s_pQry->setParameter("vFileID",i_lFileID);
        s_pQry->open();
        }

    if(s_pQry->next() ==true)
        {
        *o_iTableType=s_pQry->field("table_type").asInteger();
        strncpy(o_sFileName, s_pQry->field("file_name").asString(), 600 );
        return true;
        }

    delete s_pQry;
    s_CursorOpenFlag=0;
    return false;
    
}



//ɨ�����е��嵥,��ȡserv_id,�½�һ�����û����˵�task
void CRebillTask::fileScan ()
{
    HashList<long>  hlServID(500);
    
    long lFileID;
    long lServID;
    long _tmp;
    
    char sTname[600];
    sTname[0]='\0';
    
    int aiTableType[3]=
    {
        EVENT_TABLE_TYPE_CALL_EVENT,
        EVENT_TABLE_TYPE_DATA_EVENT,
        EVENT_TABLE_TYPE_VALUE_ADDED_EVENT
    };

    
    int iServIdOffset;
    CRebillTable* apCRebillTable[3] ;
    BaseEventData* pBaseEventData=0;
    iServIdOffset=(char*)&( pBaseEventData->strStdEvent.m_iServID)-
        (char*)pBaseEventData;
    

    for(int iTemp=0; iTemp<3; iTemp++)
    {
        m_hpCRebillTable->get(aiTableType[iTemp], &(apCRebillTable[iTemp]));
    }

    CRebillTaskFile::openTaskFile(m_lTaskID[REBILL_TASK_TYPE_FILE]);
    while (1)
    {
        lFileID = CRebillTaskFile::fetchFileID();
        if (lFileID == 0)
        {
            break;
        }
        
        for (int i=0; i<3; i++)
        {
            while (getRebillFileName (sTname, aiTableType[i], lFileID))
            {
                vector<void*>  vData;
                CTableAttrTool::Detach(aiTableType[i], sTname, vData);
                for(int jTmp=0; jTmp<vData.size(); jTmp++)
                {
                    lServID= *(long*)((char*)(vData[jTmp])+iServIdOffset);
                    if(hlServID.get(lServID, &_tmp)==false)
                        hlServID.add(lServID,lServID);
                }
                
                vData.clear();
            }
        }

        
    }



    m_lTaskID[REBILL_TASK_TYPE_SERV]
        =insertRebillTask(REBILL_TASK_TYPE_SERV);
    
    char sSql[1024];
    TOCIQuery Qry(DB_LINK);
    snprintf(sSql,1024,
        "insert into b_rebill_task_serv "
        "(task_id,serv_id)"
        "values"
        "(%ld,:vServID)",
        m_lTaskID[REBILL_TASK_TYPE_SERV]
        );
    Qry.setSQL(sSql);

    HashList<long>::Iteration iterServID
        =hlServID.getIteration();

    while( iterServID.next(lServID)==true )
    {
        Qry.setParameter("vServID", lServID);
        Qry.execute();
    }
    
    Qry.commit();


    CRebillTaskFile::closeTaskFile();

}


//����task_id
long CRebillTask::insertRebillTask(int i_iTaskType )
{
    TOCIQuery Qry(DB_LINK);
    char sSql[1024];
    long lTaskID;

    strncpy(sSql,"select SEQ_REBILL_TASK_ID.nextval from dual",1024);
    Qry.setSQL(sSql);
    
    Qry.open() ;
    if(Qry.next() )
        lTaskID=Qry.field(0).asLong();
    
    Qry.close();
    snprintf(sSql, 1024, "begin"
        " INSERT INTO B_REBILL_TASK"
        " ("
        "TASK_ID,BILLING_CYCLE_TYPE_ID,BILLING_CYCLE_ID,TASK_STATE,"
        "CREATED_DATE,STATE_DATE,TASK_TYPE,TASK_STEP,SUB_TASK_ID"
        " )"
        " VALUES "
        " ("
        "%ld,1,%d,'CRT',"
        "sysdate,null,%d,'BEG',null"
        " );"
        " update B_REBILL_TASK set sub_task_id=%ld where task_id=%ld;"
        " end;",
        lTaskID,GetBillingCycleId(),
        i_iTaskType,
        lTaskID,GetTaskId() );

    Qry.setSQL(sSql);
    Qry.execute();
//    Qry.commit();

    return lTaskID;

}



/////ɨ���嵥���ȡserv_id
void CRebillTask::hisScan ()
{
    HashList<long>  hlServID(500);
    
    long lFileID;
    long lServID;
    long _tmp;


////�嵥���˵ı�����    
    int aiTableType[4]=
        {
        EVENT_TABLE_TYPE_CALL_EVENT,
        EVENT_TABLE_TYPE_DATA_EVENT,
        EVENT_TABLE_TYPE_VALUE_ADDED_EVENT,
        EVENT_TABLE_TYPE_REDO_EVENT,
  ///      EVENT_TABLE_TYPE_OTHER_EVENT   û��serv_id
        };

    int iServIdOffset;
    CRebillTable* apCRebillTable[4] ;

    
    BaseEventData* pBaseEventData=0;
    iServIdOffset=(char*)&( pBaseEventData->strStdEvent.m_iServID)-
        (char*)pBaseEventData;
    
  
    
    for(int iTemp=0; iTemp<4; iTemp++)
        {
        if(m_hpCRebillTable->get(
                aiTableType[iTemp], &(apCRebillTable[iTemp]))
            ==true)
            apCRebillTable[iTemp]->scanServID(GetTaskId(), hlServID);
        }


    m_lTaskID[REBILL_TASK_TYPE_SERV]
        =insertRebillTask(REBILL_TASK_TYPE_SERV);
    
    char sSql[1024];
    TOCIQuery Qry(DB_LINK);
    snprintf(sSql,1024,
        "insert into b_rebill_task_serv "
        "(task_id,serv_id)"
        "values"
        "(%ld,:vServID)",
        m_lTaskID[REBILL_TASK_TYPE_SERV]
        );
    Qry.setSQL(sSql);

    HashList<long>::Iteration iterServID
        =hlServID.getIteration();

    while( iterServID.next(lServID)==true )
        {
        Qry.setParameter("vServID", lServID);
        Qry.execute();
        }
    
    Qry.commit();

 
}



////����Щ���Ѿ��������е��û���������û�Ҳ�ӽ�ȥ
void CRebillTask::prepareRebillServ()
{

    if( m_bCheckGroup ==false)
        {
        return ;
        }
    Log::log(0, "��ʼ׼����Ӱ��������û�...");

    TOCIQuery Qry(DB_LINK);
    TOCIQuery Qry_group(DB_LINK);
    
    char sSql[512];
    char sSql_group[1024];
    HashList< long> hlServID(3000) ;
    HashList< long> hlServIDGroup(3000); ////��Ϊͬ��Ⱥ�ж���Ҫ�����ӵ�
    long lServID;
    long lTmp;

    hlServID.clearAll();
    hlServIDGroup.clearAll();

    
    snprintf(sSql, 512, 
        "select distinct serv_id from b_rebill_task_serv where task_id=%ld",
        m_lTaskID[REBILL_TASK_TYPE_SERV]);
    Qry.setSQL(sSql);
    Qry.open();
    while(Qry.next())
        {
        lServID=Qry.field("serv_id").asLong();
        hlServID.add(lServID, lServID);
        }

    HashList<long>::Iteration iterServID=hlServID.getIteration();

  snprintf(sSql_group,1024,
        " select distinct b.INSTANCE_ID serv_id from product_offer_instance_detail a,product_offer_instance_detail b"
        " where a.INSTANCE_ID=:vlServID"
        " and a.PRODUCT_OFFER_INSTANCE_ID=b.PRODUCT_OFFER_INSTANCE_ID"  
        " and a.INSTANCE_TYPE='10A'"
        " and b.INSTANCE_TYPE='10A'"
        " and b.state='00A'"
        " and b.eff_date < ( select cycle_end_date from billing_cycle where billing_cycle_id=%d)"
        " and b.exp_date >=( select cycle_end_date from billing_cycle where billing_cycle_id=%d)",
        GetBillingCycleId(),
        GetBillingCycleId()
        );
        
        
    
    while(iterServID.next(lServID) )
        {
        Qry_group.setSQL(sSql_group);
        Qry_group.setParameter("vlServID", lServID);
        Qry_group.open();
        while(Qry_group.next())
            {
            lServID=Qry_group.field("serv_id").asLong();            
            if(hlServID.get(lServID,&lTmp )==false &&
                hlServIDGroup.get(lServID,&lTmp)==false )
                hlServIDGroup.add(lServID, lServID);
            }
        Qry_group.close();
        
        }
    
    Qry.close();
    
    snprintf(sSql,512,
        "insert into b_rebill_task_serv "
        "(task_id,serv_id)"
        "values"
        "(%ld,:vServID)",
        m_lTaskID[REBILL_TASK_TYPE_SERV]
        );
    Qry.setSQL(sSql);

    HashList<long>::Iteration iterServIDGroup
        =hlServIDGroup.getIteration();

    while( iterServIDGroup.next(lServID)==true )
        {
        Qry.setParameter("vServID", lServID);
        Qry.execute();
        }
    
    Qry.commit();

    
}







////ɾ��������Ϣ
void CRebillTask::clearDupInfo ()
{
    TOCIQuery Qry(DB_LINK);
    char sSql[2048];
    char sSqlIns[2048];

    strncpy(sSql,"select * from B_DUP_DEL_Trigger ",2048);
    while(1)
    {
        Qry.setSQL(sSql);
        Qry.open();
        if( Qry.next() )
        {//// �Ѿ�������������û�н����Ĳ�������
            sleep(10);
            Qry.close();
        }
        else 
            break;
    }
    
    ////��ռ�¼��
    Qry.close();
    Qry.setSQL("truncate table B_DUP_CHECK_BACK");
    Qry.execute();
    
    Qry.close();    
    Qry.setSQL("truncate table B_DUP_DEL_Trigger");
    Qry.execute();


    ////�����¼���� B_DUP_CHECK_BACK
    snprintf(sSql,2048,
    "select 'select EVENT_ID,CALLING_NBR,START_DATE,DURATION from'||"
    "' b_rebill_'||prefix_table_name||'_his where task_id=%ld'"
    "  from b_event_table_type where event_table_type_id in (%s)",
    this->GetTaskId(),
    m_sResendEventTable);

    strcpy(sSqlIns, "insert into B_DUP_CHECK_BACK (EVENT_ID,CALLING_NBR,START_TIME,DURATION) ");
    Qry.close();
    Qry.setSQL(sSql);    
    Qry.open();
    if( Qry.next() )
        {
        strncat(sSqlIns, Qry.field(0).asString(), 2048);
        }
    while(Qry.next() )
        {
        strncat(sSqlIns, " union all ", 2048);
        strncat(sSqlIns, Qry.field(0).asString(), 2048);
        }
    
    Qry.close();
    Qry.setSQL(sSqlIns);
    Qry.execute();
    Qry.commit();


////�����¼��������
    snprintf(sSql,2048,
    "select 'select distinct  bill_flow_id from'||"
    "' b_rebill_'||prefix_table_name||'_his where task_id=%ld'"
    "  from b_event_table_type where event_table_type_id in (%s)",
    this->GetTaskId(),
    m_sResendEventTable);

    strcpy(sSqlIns,
        "insert into B_DUP_DEL_Trigger (PROCESS_ID,STATE,RECORD) "
        "select process_id,'RDY',null from wf_process where app_id=7  and billflow_id in (");
    Qry.close();
    Qry.setSQL(sSql);    
    Qry.open();
    if( Qry.next() )
        {
        strncat(sSqlIns, Qry.field(0).asString(), 2048);
        }
    while(Qry.next() )
        {
        strncat(sSqlIns, " union ", 2048);
        strncat(sSqlIns, Qry.field(0).asString(), 2048);
        }
    strncat(sSqlIns, " )", 2048);

    Qry.close();
    Qry.setSQL(sSqlIns);
    Qry.execute();
    Qry.commit();

    Qry.close();
    strncpy(sSql,"select * from B_DUP_DEL_Trigger where state in ('RDY','RUN')",2048);
    while(1)
        {
        Qry.setSQL(sSql);
        Qry.open();
        if( Qry.next() )
            {//// ���ڵĻ�˵����û�����
            sleep(10);
            Qry.close();
            }
        else 
            break;
        }

    long lCnt=-1L;

    strncpy(sSql,
        "select sum(nvl(a.record,0))-nvl(count(b.event_id),0) from B_DUP_DEL_Trigger a , B_DUP_CHECK_BACK b",
        2048);
    Qry.close();
    Qry.setSQL(sSql);
    Qry.open();
    if( Qry.next() )
        {
        lCnt=Qry.field(0).asLong();
        }
     Qry.close();   
     
    if(lCnt!=0L)
        {////ɾ���ļ�¼�����������������
        Log::log(0, "ɾ���Ĳ��ؼ�¼�����������������!��鿴��B_DUP_DEL_Trigger,B_DUP_CHECK_BACK��B_DUP_DEL_LOG");
        Pprintf(0,  true, 0, "ɾ���Ĳ��ؼ�¼�����������������!��鿴��B_DUP_DEL_Trigger,B_DUP_CHECK_BACK��B_DUP_DEL_LOG");
        }
    else
        {
        Qry.setSQL("truncate table B_DUP_DEL_Trigger");
        Qry.execute();
        Qry.commit();
        }

    

}


void CRebillTask::backupRebillFile ()
{

    long lFileID;
    int iTableType;
    
    char sTname[600];
    sTname[0]='\0';

    //int i=0;
    CRebillTaskFile::openTaskFile(m_lTaskID[REBILL_TASK_TYPE_FILE]);
    while (1)
    {
        lFileID = CRebillTaskFile::fetchFileID();
        if (lFileID == 0)
        {
            break;
        }


        while (getRebillFile (sTname, &iTableType, lFileID))
        {
        vector<void*>  vData;
        CRebillTable* pCRebillTable;
        int iRBTable=getRollBackTableType(iTableType);
        CTableAttrTool::Detach( iRBTable, sTname, vData);
        if(m_hpCRebillTable->get(iRBTable,  &pCRebillTable)==true)
            pCRebillTable->backupRebillFile(
                 this->GetTaskId(),  vData);
        
        vData.clear();
        }
      //  ++i;
        
    }

    ////һ��file_id��������֮����ύ���ݿ������setTaskStep��������
      

}



void CRebillTask::backupRebillEvent()
{
    char sTname[60];
    int iInstOrgID=0;
    int iInstFlowID=0;    

    HashList<CRebillTable*>::Iteration iterCRebillTable=
        m_hpCRebillTable->getIteration();
    CRebillTable* pCRebillTable;
    
    while( iterCRebillTable.next(pCRebillTable)==true )
    {
    while (getTableName (pCRebillTable->getTableType(), 
        sTname,  &iInstOrgID,  &iInstFlowID))
        {
        pCRebillTable->backupRebillEvent(
             this->GetTaskId(),  sTname, iInstOrgID, iInstFlowID);
        }
    }

}


void CRebillTask::waitForResendFile()
{


}

//////�ȴ������ݿ�
void CRebillTask::waitDB( )
{
   if(m_iLastFileID<=0)
    return ;

    char sSQL[2048];
    sprintf (sSQL, "select count(*) from b_package_file_list where  file_id=%d and table_type=%d",
        m_iLastFileID, 
        TABLE_TYPE_ENDFILE);

    DEFINE_QUERY (qry);

    bool bWait(true);

    sleep (2);

    Log::log(0, " ���ڵȴ�д�ļ����... ");
    
    while (bWait)
        {
        qry.setSQL(sSQL);
        qry.open ();
        qry.next ();
        if (qry.field (0).asInteger () > 0)
            {
            bWait = false;
            }
        qry.close ();
        if (bWait) sleep (5);
        }

/////rebill������������������ֱ��д�ļ������
/*//////////////д�ļ����֮��, �ر��ļ���, �����
    qry.close ();
    sprintf (sSQL, "update b_file_package set state='RDY' where PACKAGE_ID = %ld ", m_lPackageID);
    qry.setSQL(sSQL);
    qry.execute();
    qry.commit();

/////////////*/
    qry.close ();
    sprintf (sSQL, "select count(*) from end_file_log where file_id=%d",  m_iLastFileID);

    bWait=true;

    sleep (2);

    Log::log(0, " ���ڵȴ������ݿ����... ");
    
    while (bWait)
        {
        qry.setSQL(sSQL);
        qry.open ();
        qry.next ();
        if (qry.field (0).asInteger () > 0)
            {
            bWait = false;
            }
        qry.close ();
        if (bWait) sleep (5);
        }


}




