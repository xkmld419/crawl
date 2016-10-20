/*VER: 15*/ 
#include "rt_make_stop.h"
#include <vector>
#include "HashList.h"
#include "QueryData.h"
#include "TransactionMgr.h"
#include "AcctItemAccu.h"
#include "stopBalanceMgr.h"
#include "stopChargeMgr.h"

#ifdef DEF_BEIJING
#define STOP_RETURN_SERV_CHARGE
#elif DEF_HAINAN
#define STOP_RETURN_SERV_CHARGE
#endif

TOCIDatabase gpDBLink_STOP;

#define AREA_CODE       ((FLDID32)167772272)
#define ACC_NBR         ((FLDID32)167772269)
#define ACCT_NBR_97     ((FLDID32)167772268)
#define STAFF_ID        ((FLDID32)33554575)
#define BALANCE         ((FLDID32)33554545)
#define OWE_CHARGE      ((FLDID32)33564444)
#define BILL_CHARGE     ((FLDID32)33554550)
#define	EXP_DATE        ((FLDID32)167772432)
#define ACCT_ID         ((FLDID32)33554534) 
#define	SMSG            ((FLDID32)167782162)
#define SERV_ID         ((FLDID32)167772301)
#define ACCT_ID         ((FLDID32)33554534)
#define	ACCT_SEQ_NBR	((FLDID32)33554535)

char Rt_Stop_Make::sTableName[256];
long     Rt_Stop_Make::glBalance;
long     Rt_Stop_Make::glOweCharge;
long     Rt_Stop_Make::glAggrCharge;
int      Rt_Stop_Make::giBalanceCnt;
int      Rt_Stop_Make::giOweCnt;
int      Rt_Stop_Make::giAggrCnt;
int      Rt_Stop_Make::giCreditCnt;
int      Rt_Stop_Make::giAcctCredit;

long  	 Rt_Stop_Make::giLastlServID = 0;	
int 	 Rt_Stop_Make::giCalcCredit	= -1;
int 	 Rt_Stop_Make::giCalcLevel = 0;		
int	 Rt_Stop_Make::giCurOrgID300=0;	
char	 Rt_Stop_Make::gsPreTableName[256];
int      Rt_Stop_Make::giBillingCycleID;
	
long Rt_Stop_Make::lacctResItemBalance;
long Rt_Stop_Make::lservResItemBalance;
	
	
//TOCIDatabase	Rt_Stop_Make::gpDBLink;
//InstTableListMgr	Rt_Stop_Make::tablelistMgr;	
vector<struct B_STOP_OFFER> Rt_Stop_Make::m_strStopOffer;
long     Rt_Stop_Make::glSourceServID;
static int gaiTimeCount[MIN_ARRAY][3][2];
static TLimitCmp *gpLimitCmpHead;

static char gsStartTime[15];
static int  giForwardMin, giCurMin;

int  giServeInitFlag=0;
FBFR32 *sendbuf, *rcvbuf;

//acct_id��serv_id������40000����
struct SERV_OWE_CHARGE_BUF strOweCharge[40000];
int  giOweChargeCnt;

//����ͣ����ֵ�Ŀ���
#define STOP_LIMIT_TJ    0    //��ͣ
#define STOP_LIMIT_ST    1    //˫ͣ
#define STOP_LIMIT_CJ    2    //�߽�
#define SERV_USE_PROC   (ParamDefineMgr::getLongParam("MEMORY_DB", "MakeStopFlag1"))
#define VALUE_METHOD    (ParamDefineMgr::getLongParam("MEMORY_DB", "MakeStopFlag2"))
#define OWE_DAYS    (ParamDefineMgr::getLongParam("MEMORY_DB", "MakeStopFlag3"))
#define TX_DAYS    (ParamDefineMgr::getLongParam("MEMORY_DB", "BALANCE_TX_DAY"))
#define INSERT_WORK_ORDER_LOG    (ParamDefineMgr::getLongParam("RT_MAKE_STOP", "IS_INSERT_WORK_ORDER_LOG"))
#define CREDIT_DEFAULT_VALUE (ParamDefineMgr::getLongParam("RT_MAKE_STOP", "CREDIT_DEFAULT_VALUE"))
#define VALUE_MAX_CHARGE    (ParamDefineMgr::getLongParam("RT_MAKE_STOP", "VALUE_MAX_CHARGE"))	
#define REMIND_TIME1_MIN_VALUE (ParamDefineMgr::getLongParam("RT_MAKE_STOP", "REMIND_TIME1_MIN_VALUE"))
#define REMIND_TIME1_MAX_VALUE (ParamDefineMgr::getLongParam("RT_MAKE_STOP", "REMIND_TIME1_MAX_VALUE"))
#define REMIND_TIME2_MIN_VALUE (ParamDefineMgr::getLongParam("RT_MAKE_STOP", "REMIND_TIME2_MIN_VALUE"))
#define REMIND_TIME2_MAX_VALUE (ParamDefineMgr::getLongParam("RT_MAKE_STOP", "REMIND_TIME2_MAX_VALUE"))	
#define STOP_HIGH_FEE (ParamDefineMgr::getLongParam("RT_MAKE_STOP", "STOP_HIGH_FEE"))	
#define  STOP_DELAY_TIME (ParamDefineMgr::getLongParam("RT_MAKE_STOP", "STOP_DELAY_TIME"))		
	
int  	Rt_Stop_Make::oneSleepValidTime = 1;	
int Rt_Stop_Make::DisBillConn()
{
	try {
			gpDBLink_STOP.disconnect ();
		} catch (...) {
	};
	return 1;
};

void Rt_Stop_Make::setSourceType(char sSourceType[])
{ 
	memset(g_sSourceType,0,sizeof(g_sSourceType));
	strcpy(g_sSourceType,sSourceType);
	bSourceType = true;
	return;
}

int Rt_Stop_Make::BillConn(char sUser[],char sPwd[],char sStr[])
{
	gpDBLink_STOP.connect(sUser,sPwd,sStr);
	return 1;
}

int Rt_Stop_Make::getServStopRecord()
{
		static vector<B_RT_STOP_DETAIL> 	v_b_stop_detail;
		static int 		i=-1;
		char	 sSql[2000];
		//DEFINE_QUERY(qry);
		B_RT_STOP_DETAIL	temp_b_stop_detail;
		
		TOCIQuery qry(&gpDBLink_STOP);
		//qry = new TOCIQuery(&gpDBLink_STOP);
		
		if (i == -1 || i == v_b_stop_detail.size()){ // ��ȡ��¼
			v_b_stop_detail.clear();
			i=0;
		#ifdef BIG_STOP
		if (iOrgID300 > 0)
		{	
			if (bSourceType){
				sprintf(sSql,"select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300,\
				BILLING_ORG_ID,START_DATE,CHARGE1,ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID from ( \
				select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300, \
				BILLING_ORG_ID,TO_CHAR(START_DATE,'YYYYMMDDHH24MISS') START_DATE,CHARGE1,\
				ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID \
				from B_RT_STOP_DETAIL where union_org_id=%d \
				and source_event_type_id in (%s) \
				order by stop_step_id asc) where rownum<%d ",iOrgID300,g_sSourceType,ONCE_STOP_CNT);
			} else {			
				sprintf(sSql,"select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300,\
				BILLING_ORG_ID,START_DATE,CHARGE1,ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID from ( \
       			 select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300, \
				BILLING_ORG_ID,TO_CHAR(START_DATE,'YYYYMMDDHH24MISS') START_DATE,CHARGE1,\
				ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID \
				from B_RT_STOP_DETAIL where union_org_id=%d \
				order by stop_step_id asc) where rownum<%d  ",iOrgID300,ONCE_STOP_CNT);
			}
		}
		else
		{
			if (bSourceType){
				sprintf(sSql,"select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300,\
				BILLING_ORG_ID,START_DATE,CHARGE1,ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID from ( \
				select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300, \
				BILLING_ORG_ID,TO_CHAR(START_DATE,'YYYYMMDDHH24MISS') START_DATE,CHARGE1,\
				ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID \
				from B_RT_STOP_DETAIL_EX where union_org_id=%d \
				and source_event_type_id in (%s) \
				order by stop_step_id asc) where rownum<%d ",iOrgID300,g_sSourceType,ONCE_STOP_CNT);
			} else {			
				sprintf(sSql,"select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300,\
				BILLING_ORG_ID,START_DATE,CHARGE1,ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID from ( \
       			 select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300, \
				BILLING_ORG_ID,TO_CHAR(START_DATE,'YYYYMMDDHH24MISS') START_DATE,CHARGE1,\
				ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID \
				from B_RT_STOP_DETAIL_EX where union_org_id=%d \
				order by stop_step_id asc) where rownum<%d  ",iOrgID300,ONCE_STOP_CNT);
			}	
				
		}
		#else
		if (bSourceType){
				sprintf(sSql,"select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300,\
				BILLING_ORG_ID,START_DATE,CHARGE1,ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID from ( \
				select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300, \
				BILLING_ORG_ID,TO_CHAR(START_DATE,'YYYYMMDDHH24MISS') START_DATE,CHARGE1,\
				ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID \
				from B_RT_STOP_DETAIL where union_org_id=%d \
				and source_event_type_id in (%s) \
				order by stop_step_id asc) where rownum<%d ",iOrgID300,g_sSourceType,ONCE_STOP_CNT);
			} else {			
				sprintf(sSql,"select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300,\
				BILLING_ORG_ID,START_DATE,CHARGE1,ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID from ( \
       			 select stop_step_id,id,type,billing_cycle_id,billflow_id,org_id_300, \
				BILLING_ORG_ID,TO_CHAR(START_DATE,'YYYYMMDDHH24MISS') START_DATE,CHARGE1,\
				ACCT_ITEM_TYPE_ID1,SOURCE_EVENT_TYPE_ID \
				from B_RT_STOP_DETAIL where union_org_id=%d \
				order by stop_step_id asc) where rownum<%d  ",iOrgID300,ONCE_STOP_CNT);
			}	
		
		#endif 	

			qry.close();
			qry.setSQL(sSql);	
			qry.open();
			for(;qry.next();){
					temp_b_stop_detail.lStopStepID=qry.field("stop_step_id").asLong();
					temp_b_stop_detail.lServID=qry.field("id").asLong();
					temp_b_stop_detail.iType=qry.field("type").asInteger();
					temp_b_stop_detail.iBillingCycleID=qry.field("billing_cycle_id").asInteger();
					temp_b_stop_detail.iBillFlowID=qry.field("billflow_id").asInteger();
					temp_b_stop_detail.iOrgID300=qry.field("org_id_300").asInteger();
					temp_b_stop_detail.iBillingOrgID = qry.field("BILLING_ORG_ID").asInteger();
					strcpy(temp_b_stop_detail.sStartDate,qry.field("START_DATE").asString());
					temp_b_stop_detail.lCharge1 = qry.field("CHARGE1").asInteger();
					temp_b_stop_detail.iAcctItemTypeID1 = qry.field("ACCT_ITEM_TYPE_ID1").asInteger();
					temp_b_stop_detail.iSourceEventType = qry.field("SOURCE_EVENT_TYPE_ID").asInteger();
					if (v_b_stop_detail.size()>0 && v_b_stop_detail.back().lServID == temp_b_stop_detail.lServID)
							continue;
				else 
							v_b_stop_detail.push_back(temp_b_stop_detail);
			}
			qry.close();
		}
		
		if (v_b_stop_detail.size() == 0){
			i=-1;
			return 0;
		}
		else{
			b_rt_stop_detail=v_b_stop_detail[i++];
			return 1;
		}
}

void Rt_Stop_Make::analyzeTalbeStopDetail()
{
		//DEFINE_QUERY(qry);
		//qry = new TOCIQuery(&gpDBLink_STOP);
		
		TOCIQuery qry(&gpDBLink_STOP);
		qry.close();
		qry.setSQL("ANALYZE TABLE B_RT_STOP_DETAIL COMPUTE STATISTICS ");
		qry.execute();
		qry.commit();
		qry.close();//���ӹر�
}

void Rt_Stop_Make::deleteStopDetail()
{
	DEFINE_QUERY(qry1);
	TOCIQuery qry(&gpDBLink_STOP);
	//qry = new TOCIQuery(&gpDBLink_STOP);
	char sSql[2000];
	
	//һ����ͬ���³���30���û��Ļ�,��ɾ�������ͬ�������е��û���¼
	if (iServCnt > 30)
		  if (bSourceType)
		  {
		  	sprintf(sSql," DELETE /*+rule+*/ B_RT_STOP_DETAIL WHERE ID IN "
			          "          (SELECT SERV_ID FROM SERV_ACCT WHERE ACCT_ID in "
								"            (SELECT ACCT_ID FROM SERV_ACCT  WHERE SERV_ID=%ld AND STATE='10A') "
								"            AND STATE='10A'"
								"           ) "
								" AND TYPE=%d AND SOURCE_EVENT_TYPE_ID IN (%s) AND STOP_STEP_ID<=%ld ",b_rt_stop_detail.lServID,
									b_rt_stop_detail.iType,g_sSourceType,b_rt_stop_detail.lStopStepID);
		  }else{
			sprintf(sSql," DELETE /*+rule+*/ B_RT_STOP_DETAIL WHERE ID IN "
			          "          (SELECT SERV_ID FROM SERV_ACCT WHERE ACCT_ID in "
								"            (SELECT ACCT_ID FROM SERV_ACCT  WHERE SERV_ID=%ld AND STATE='10A') "
								"            AND STATE='10A'"
								"           ) "
								" AND TYPE=%d AND STOP_STEP_ID<=%ld ",b_rt_stop_detail.lServID,
									b_rt_stop_detail.iType,b_rt_stop_detail.lStopStepID);
		}
	else{
		if(bSourceType){
			sprintf(sSql," DELETE B_RT_STOP_DETAIL WHERE ID=%ld AND TYPE=%d AND SOURCE_EVENT_TYPE_ID IN (%s) AND STOP_STEP_ID<=%ld ",
						b_rt_stop_detail.lServID,b_rt_stop_detail.iType,g_sSourceType,b_rt_stop_detail.lStopStepID);
		}else{
		sprintf(sSql," DELETE B_RT_STOP_DETAIL WHERE ID=%ld AND TYPE=%d AND STOP_STEP_ID<=%ld ",
						b_rt_stop_detail.lServID,b_rt_stop_detail.iType,b_rt_stop_detail.lStopStepID);
		}
	}
	qry.close();
	qry.setSQL(sSql);
	qry.execute();
	qry.commit();
	qry.close();
	qry1.commit();
	qry1.close();
}

Rt_Stop_Make::~Rt_Stop_Make()
{
    if(m_pBackupData != NULL){
        m_pBackupData->destroy();
        delete m_pBackupData;
        m_pBackupData = 0;
    }  
    if (pAttrTransMgr)
    {
   		delete pAttrTransMgr;
   		pAttrTransMgr = NULL;
    }
    if (G_PAGGRMGR)
    {
    	delete G_PAGGRMGR;
    	G_PAGGRMGR = NULL;
    }
    if (m_poBCM)
    {
    	delete m_poBCM;
    	m_poBCM = NULL;
    }
}

Rt_Stop_Make::Rt_Stop_Make(int iProcessID,int iOrgID300):bSourceType(false)
{

	this->iProcssID=iProcssID;	
	this->iOrgID300=iOrgID300;
	memset(g_sSourceType,0,sizeof(g_sSourceType));

	glOweCharge = 0;
	glAggrCharge = 0;
	glBalance = 0;
	giAcctCredit = 0;
	giCreditCnt = 0;
	giOweCnt = 0;
	giAggrCnt = 0;
	giBalanceCnt = 0;
	iServCnt = 0;
	
	char sDate[15];
	GetDateStr(sDate);
    strcpy(gsStartTime, sDate);
	InitStopLimit(iOrgID300); //add by yangch for test 20070319
	
	/*	ʹ���û�����	*/
	Environment::useUserInfo ();
	
	pAttrTransMgr = new AttrTransMgr(iProcessID);
	
	if (G_PAGGRMGR == 0)
	{
		G_PAGGRMGR = new AggrMgr;
		if (!G_PAGGRMGR) THROW(MBC_APP_STOP+1);
	}
	if (m_poBCM == 0)
	{
		m_poBCM = new BillingCycleMgr ();
    }
    if (m_pBackupData == 0)
	{
    	m_pBackupData = new HashList<struct BackupDataCfg> (123);
    }
    
 
    loadBackupData();
}

void Rt_Stop_Make::loadBackupData()
{
    struct BackupDataCfg strTempData;
    int iBusiTypeID = 0;
    char sTempStr[32];
    char * sTemp;
    int i = 0, j = 0;
    
    DEFINE_QUERY(qryBackup);
    memset(&strTempData, 0, sizeof(strTempData));
    qryBackup.close();
    qryBackup.setSQL("select backup_time_min, backup_time_max, busi_type_id "
        "from b_rtstop_backup_cfg a where a.state = '10A'");
    qryBackup.open();
    while(qryBackup.next()){
        memset(sTempStr, 0, sizeof(sTempStr));
        strncpy(sTempStr, qryBackup.field("backup_time_min").asString(), 32);
        sTemp = strtok(sTempStr, ",");
        i = 0;
        while(sTemp != NULL){
            if(i == MAX_TIMESET_CNT){
                Log::log(0,"�������ʱ���MAX_TIMESET_CNT��");
	   	 		exit;
            }
            
            strncpy(strTempData.sTimeMin[i], sTemp, MAX_TIMESET_CNT-1);
            sTemp = strtok(NULL, ",");
            i ++;
        }
        
        memset(sTempStr, 0, sizeof(sTempStr));
        strncpy(sTempStr, qryBackup.field("backup_time_max").asString(), 32);
        sTemp = strtok(sTempStr, ",");
        j = 0;
        while(sTemp != NULL){
            if(j == MAX_TIMESET_CNT){
                Log::log(0,"�������ʱ���MAX_TIMESET_CNT��");
	   	 		exit;
            }
            
            strncpy(strTempData.sTimeMax[j], sTemp, MAX_TIMESET_CNT-1);
            sTemp = strtok(NULL, ",");
            j ++;
        }
        
        strTempData.iSillCnt = (i >= j) ? j : i;
        
        iBusiTypeID = qryBackup.field("busi_type_id").asInteger();
        
        m_pBackupData->add(iBusiTypeID, strTempData);
    }
}

void Rt_Stop_Make::insertBackData(int iOweBTypeID)
{
    DEFINE_QUERY(qryBackupData);
    char sSql[1024];
    
    sprintf(sSql,"insert INTO b_rtstop_backup (   \
        serv_id,billing_cycle_id,billflow_id,billing_org_id,created_date, \
        charge,acct_item_type_id,source_event_type_id,busi_type_id,state) \
        VALUES (:SERV_ID,:BILLING_CYCLE_ID,:BILLFLOW_ID,:BILLING_ORG_ID,SYSDATE, \
        :CHARGE,:ACCT_ITEM_TYPE_ID,:SOURCE_EVENT_TYPE_ID,:BUSI_TYPE_ID,'10A')");
    qryBackupData.close();
    qryBackupData.setSQL(sSql);
    qryBackupData.setParameter("SERV_ID", b_rt_stop_detail.lServID);
    qryBackupData.setParameter("BILLING_CYCLE_ID", b_rt_stop_detail.iBillingCycleID);
    qryBackupData.setParameter("BILLFLOW_ID", b_rt_stop_detail.iBillFlowID);
    qryBackupData.setParameter("BILLING_ORG_ID", b_rt_stop_detail.iBillingOrgID);
    qryBackupData.setParameter("CHARGE", b_rt_stop_detail.lCharge1);
    qryBackupData.setParameter("ACCT_ITEM_TYPE_ID", b_rt_stop_detail.iAcctItemTypeID1);
    qryBackupData.setParameter("SOURCE_EVENT_TYPE_ID", b_rt_stop_detail.iSourceEventType);
    qryBackupData.setParameter("BUSI_TYPE_ID", iOweBTypeID);
    qryBackupData.execute();
}

void Rt_Stop_Make::backupData(int iOweBTypeID)
{
    struct BackupDataCfg oTempData;
    
    if(m_pBackupData->get(iOweBTypeID, &oTempData)){
        Date dCurr;
        char sTempTime[15] = {0};
        
        dCurr.getTimeString(sTempTime);
        for(int i = 0; i < oTempData.iSillCnt; i ++){
            if((strcmp(&sTempTime[8], oTempData.sTimeMin[i]) >= 0)
                && (strcmp(&sTempTime[8], oTempData.sTimeMax[i]) < 0)){
                insertBackData(iOweBTypeID);
                break;
            }
        }
    }
}

void Rt_Stop_Make::doWithStopDetail()
{
    /* m_iErrorID:
			:0		��Ҫ����
			:1		�û���ͣ
			:2		�ʻ���ͣ
			:3		�û��߽�
			:4      �ʻ��߽�
			:5		�ʻ�ͣ��,δ��Ҫ��߽ɴ��������ٴ߽�(����)
			:6		�ʻ�˫��
			:7	    �ʻ�����
			:8      �û�ͣ�����߽�
			:9      �û�����
			:10     �û�˫��
			:11     �û�˫ͣ
			:12 	�˻�˫ͣ
			:13     һ��˫���߽��ж�
			:14     һ��˫��ͣ���ж�
			:15     IP�������⴦��
			:16   �ʻ��������,�����ʻ��߽�
			:17   �û��������,�����û��߽�
			:18     һ��˫����ͣ�ж�
			:19		�û���ͣ�Ϳۼ����ö�
			:20   CDMA�û�����߽�JS
			:20   �������ö��ж�,�������������û�˫ͣ
			:21   Ԥ������
			:22   CDMAС������
			:23   ������Ʒͣ������ָ����Ʒ��Ա��ͣ������
	*/
	DEFINE_QUERY(qry);
	char 	sState[5];
	//int	iAcctID,iBillingModeID,iProductID;
	int iBillingModeID,iProductID;
	long lAcctID; //modify by yangch
	Date sys_date;
	char sDate[16];
	
	glBalance = 0;
	glOweCharge = 0;
	glAggrCharge = 0;	
	giBalanceCnt  = 0;
	giOweCnt = 0;
	giAggrCnt = 0;
	iServCnt = 0;
	giCreditCnt = 0;
	giAcctCredit = 0;
	giLastlServID = 0;	
	giCalcCredit = -1;	
	giCalcLevel = 0;
	giOweChargeCnt = 0;
	
	
	strcpy(sDate,sys_date.toString());
	
	memset(&gstrEvent,0,sizeof(gstrEvent));
	gstrEvent.m_iErrorID=0;
	gstrEvent.m_oEventExt.m_iSourceEventType = b_rt_stop_detail.iSourceEventType;
	gstrEvent.m_iServID = b_rt_stop_detail.lServID;
	gstrEvent.m_iBillingCycleID = b_rt_stop_detail.iBillingCycleID;
/*ʹ�õ�ǰʱ��2007.08.07 suzhou*/
	strcpy(b_rt_stop_detail.sStartDate,sDate);
	strcpy(gstrEvent.m_sStartDate,b_rt_stop_detail.sStartDate);
	gstrEvent.m_iBillingOrgID = b_rt_stop_detail.iBillingOrgID;
	gstrEvent.m_oChargeInfo[0].m_lCharge = b_rt_stop_detail.lCharge1;
	gstrEvent.m_oChargeInfo[0].m_iAcctItemTypeID = b_rt_stop_detail.iAcctItemTypeID1;
	gstrEvent.m_lSendAmount = 0;
	gstrEvent.m_lRecvAmount = 0;
	giCurOrgID300=b_rt_stop_detail.iOrgID300;
	
	if (!(G_PUSERINFO->getServ (G_SERV, b_rt_stop_detail.lServID, sDate))) {
			//�û�����û���ҵ�
			Log::log (91, "û���ҵ�SERVID:%ld %s", b_rt_stop_detail.lServID, sDate);
			return;
	}
	
	//ȡ����ǰʱ������ڱ�ʶ
	BillingCycle const * pCycle = m_poBCM->getOccurBillingCycle (G_PSERV->getBillingCycleType (),sDate);
  if (pCycle) {
                gstrEvent.m_iBillingCycleID = pCycle->getBillingCycleID ();
                giBillingCycleID = gstrEvent.m_iBillingCycleID;
  } else {
           Log::log (0, "[ERROR]: �Ҳ�����������[servid:%ld, date:%s]",b_rt_stop_detail.lServID , sDate);
           return;
  }

	gstrEvent.m_iProdID = G_SERV.getServInfo ()->m_iProductID;
  char sTempBillingMode[20],*pTempBillingMode;
  pTempBillingMode=G_PSERV->getAttr (ID_BILLING_MODE);
  if (pTempBillingMode == NULL)
   {
       Log::log(0,"SERV_ID:%ld,�Ҳ���Ԥ�󸶷ѱ�־!",b_rt_stop_detail.lServID);   
       strcpy(sTempBillingMode,"99");
   }
  else strcpy(sTempBillingMode,pTempBillingMode);

	gstrEvent.m_iBillingTypeID = atoi(sTempBillingMode);
	
	//������ȡ��������ֵ,��ȡbilling_mode_id
	if (gstrEvent.m_iBillingTypeID == 0)
				gstrEvent.m_iBillingTypeID = G_PSERV->getServInfo ()->m_iBillingMode;
	//����ѱ�־����Ԥ���ѱ�־
	gstrEvent.m_iNoBillFlag = G_SERV.getServInfo ()->m_iBillingMode;

	//����������Ʒ��ʶ(�ӿ��޸��ˣ����ȡ����������ƷID)
	//G_PSERV->getOffers (gstrEvent.m_lOffers, b_rt_stop_detail.sStartDate);
	
	//add by yangch 20070430
	gstrEvent.m_lOffers[0] = G_PSERV->getBaseOfferID (b_rt_stop_detail.sStartDate);

	BillingCycleMgr bcm;
	
//2007.08.01 sz ʹ�����µ�billing_cycle_id	
	b_rt_stop_detail.iBillingCycleID = gstrEvent.m_iBillingCycleID;
	tablelistMgr.getTableName(b_rt_stop_detail.iBillingCycleID,b_rt_stop_detail.iBillFlowID,TABLE_TYPE_ACCT_ITEM_AGGR,b_rt_stop_detail.iOrgID300,Rt_Stop_Make::sTableName);
	
	strcpy(sState,bcm.getPreBillingCycle(b_rt_stop_detail.iBillingCycleID)->getState());
	//sState[2]='A';
	if (sState[2]=='R' || sState[2]=='A' || sState[2]=='B' )
	{
		int iPreBillingCycleID;
		iPreBillingCycleID = bcm.getPreBillingCycle(b_rt_stop_detail.iBillingCycleID)->getBillingCycleID ();
		if (!tablelistMgr.getTableName(iPreBillingCycleID,b_rt_stop_detail.iBillFlowID,TABLE_TYPE_ACCT_ITEM_AGGR,b_rt_stop_detail.iOrgID300,gsPreTableName))
		{
			Log::log (0, "δ��������:%d�����ʱ�",iPreBillingCycleID);
			return;
		}
	}
	//��ʼ���ڶ����жϱ�ʶֵ
	gstrEvent.m_oEventExt.m_iPartialIndicator = 0;
	glSourceServID = b_rt_stop_detail.lServID;
	
	pAttrTransMgr->trans( &gstrEvent );
	//����
	lAcctID=G_PSERV->getAcctID(b_rt_stop_detail.iAcctItemTypeID1);
	//giAcctCredit=G_PUSERINFO->getAcctCreditAmount (lAcctID);
	
	//���еڶ����ж�
	if (gstrEvent.m_oEventExt.m_iPartialIndicator == 1)
	{
		//������ͣ����жϣ���ϸ��
		if (GetOweSpecialServ(gstrEvent.m_iServID,OWE_BTYPE_ID_TJ))
			return;
		if (GetOweSpecialServ(gstrEvent.m_iServID,OWE_BTYPE_ID_CJ))
			return;
		if (GetOweSpecialServ(gstrEvent.m_iServID,OWE_BTYPE_ID_ST))
			return;

		int iErrorTmp;
		
		iErrorTmp = gstrEvent.m_iErrorID;
		
		InsertStopMiddle(b_rt_stop_detail.lServID,lAcctID,gstrEvent.m_iErrorID);
		gstrEvent.m_oEventExt.m_iSourceEventType = 13;
		gstrEvent.m_iErrorID = 0;
		pAttrTransMgr->trans( &gstrEvent );
		if (gstrEvent.m_iErrorID==0) return;
		removeStopMiddleLog(b_rt_stop_detail.lServID);

		gstrEvent.m_iErrorID = iErrorTmp;
	}
	
	if (gstrEvent.m_iErrorID==0)
		return;
	
	//����ֵ���޺�����
	//�������ֽ�����Ϊ��ֵ����,�����ֽ�����Ϊ��ֵ����
	glMaxCharge =gstrEvent.m_lSendAmount;
	glMinCharge = gstrEvent.m_lRecvAmount;
	
	if (gstrEvent.m_iErrorID==1){/*�û�ͣ��*/
	    if (GetOweSpecialServ(gstrEvent.m_iServID,OWE_BTYPE_ID_TJ))
			return;
	
		/*��Ҫ���ǵ����ڴ��ͺ��ڱ�����*/
		if ( !GetServState(b_rt_stop_detail.lServID,sState,&iBillingModeID,&iProductID) )
			return;

		//�ж��û��Ƿ����ڲ�ͣ���Ĳ�Ʒ
    	if (GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_TJ,iBillingModeID)) 
    	    return;
		
		if (CheckServState(sState,OWE_BTYPE_ID_TJ) == 0) 
		    return;
		  
		 if (gstrEvent.m_oChargeInfo[0].m_lCharge >STOP_HIGH_FEE)
		 {		 	
			 DealWithHighFee(b_rt_stop_detail.lServID,OWE_BTYPE_ID_TJ,sState);
		} 	
		else
		{			
			InsertWorkOrder(b_rt_stop_detail.lServID,OWE_BTYPE_ID_TJ,sState);
			UpdateServState(b_rt_stop_detail.lServID,OWE_BTYPE_ID_TJ,OWE_STATE,sState);
	    	AddCountLimit(STOP_LIMIT_TJ);
		}
	}
	
	if (gstrEvent.m_iErrorID==2){/*�ʻ�ͣ��*/
		
		 if (gstrEvent.m_oChargeInfo[0].m_lCharge >STOP_HIGH_FEE)
		 {		 	
			 DealWithHighFee(b_rt_stop_detail.lServID,OWE_BTYPE_ID_TJ,sState);
		} 	
		else
		{	
			WorkOrderFromAcct(lAcctID,OWE_BTYPE_ID_TJ);
		}
	}
	
	if (gstrEvent.m_iErrorID==3){/*�û��߽�*/
		if ( !GetServState(b_rt_stop_detail.lServID,sState,&iBillingModeID,&iProductID) )
			return;
			
		if (CheckServState(sState,OWE_BTYPE_ID_CJ)==0) 
		    return;
		    
		if (GetOweSpecialServ(b_rt_stop_detail.lServID,OWE_BTYPE_ID_CJ))
			return;

		//�ж��û�
    if (!GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_CJ,iBillingModeID)) 
    	 return;
			
		if (CheckOweDunExist(b_rt_stop_detail.lServID,OWE_BTYPE_ID_CJ)==1)/*����Ҫ�߽�*/
			return;	
				
		InsertOweDun(b_rt_stop_detail.lServID,OWE_BTYPE_ID_CJ);
		AddCountLimit(STOP_LIMIT_CJ);
	}
	
	if (gstrEvent.m_iErrorID == 4){/*�ʻ��߽�*/
		OweDunFromAcct(lAcctID,OWE_BTYPE_ID_CJ);
	}
	
	///Add 20050922,������������,Ҫ���û��߽�5��,���ߵ�Ƿ��δ��5��������߽�
	if (gstrEvent.m_iErrorID == 5){/*�ʻ�ͣ�����߽�*/
		OweDunAndWorkOrder(lAcctID);
	}
	
	if (gstrEvent.m_iErrorID == 6){/*�ʻ�˫��*/
		OpenWorkOrderFromAcct(lAcctID,OWE_BTYPE_ID_SK);
	}
	
	if (gstrEvent.m_iErrorID == 7){/*�ʻ�����*/
		OpenWorkOrderFromAcct(lAcctID,OWE_BTYPE_ID_DK);
	}
	
	if (gstrEvent.m_iErrorID == 8){/*�û�ͣ�����߽�*/
		OweDunAndWorkOrderServ(b_rt_stop_detail.lServID);
	}
	
	if (gstrEvent.m_iErrorID == 9){/*�û�����*/
		OpenWorkOrder(b_rt_stop_detail.lServID,OWE_BTYPE_ID_DK);
	}
	
	if (gstrEvent.m_iErrorID == 10){/*�û�˫��*/
		OpenWorkOrder(b_rt_stop_detail.lServID,OWE_BTYPE_ID_SK);
	}

    //add by yangch for SuZhou	
	if (gstrEvent.m_iErrorID == 11){/*�û�˫ͣ */
	    if (GetOweSpecialServ(gstrEvent.m_iServID,OWE_BTYPE_ID_ST))
		    return;
			
		/*��Ҫ���ǵ����ڴ��ͺ��ڱ�����*/
		if ( !GetServState(b_rt_stop_detail.lServID,sState,&iBillingModeID,&iProductID) )
			return;

		//�ж��û��Ƿ����ڲ�ͣ���Ĳ�Ʒ
        if (GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_ST,iBillingModeID)) 
            return;
		
		//CheckServState�������Ӷ�˫ͣ״̬���ж�
		if (CheckServState(sState,OWE_BTYPE_ID_ST)==0) 
		    return;
		if(gstrEvent.m_oChargeInfo[0].m_lCharge >STOP_HIGH_FEE)
		{		 	
			 DealWithHighFee(b_rt_stop_detail.lServID,OWE_BTYPE_ID_ST,sState);
		} 	
		else
		{
			InsertWorkOrder(b_rt_stop_detail.lServID,OWE_BTYPE_ID_ST,sState);
			UpdateServState(b_rt_stop_detail.lServID,OWE_BTYPE_ID_ST,STOP_STATE,sState);
   			AddCountLimit(STOP_LIMIT_ST);
		}
	}

    //add by yangch for SuZhou
	if (gstrEvent.m_iErrorID == 12){/*�˻�˫ͣ */
		
		if(gstrEvent.m_oChargeInfo[0].m_lCharge >STOP_HIGH_FEE)
		{		 	
			 DealWithHighFee(b_rt_stop_detail.lServID,OWE_BTYPE_ID_ST,sState);
		} 	
		else
		{
	    	WorkOrderFromAcct(lAcctID,OWE_BTYPE_ID_ST);
		}
	}

	if (gstrEvent.m_iErrorID == 13){
	    long lAnotherMember;
	    long lServIDTemp;
	    
	    lServIDTemp = b_rt_stop_detail.lServID;
	    
	    if (GetAnotherMem(lAcctID, b_rt_stop_detail.lServID, lAnotherMember,gstrEvent.m_iErrorID) == -1)
	        Log::log(0,"һ��˫���û�ld%�Ĳ��뷽��������.", b_rt_stop_detail.lServID);
	            
	    for (int i = 0; i < 2; i++, lServIDTemp = lAnotherMember)	    
	    {
	        if (lServIDTemp == 0)
	            continue;

			if (i==1)
			{
				giBalanceCnt = 0;
				glBalance = LocalFunction(lAcctID,lServIDTemp,b_rt_stop_detail.iBillingCycleID,-1,-1);
				
				/*��������������ڴ������뷧ֵ�ڣ�����߽�*/
				if (glBalance>glMaxCharge || glBalance<glMinCharge)
					return;
			}
			
			if ( !GetServState(lServIDTemp, sState, &iBillingModeID, &iProductID) )
			    continue;
			    
		    if (CheckServState(sState, OWE_BTYPE_ID_CJ)==0)
		        continue;
		        
		    if (GetOweSpecialServ(lServIDTemp, OWE_BTYPE_ID_CJ))
				continue;
				
		    if (CheckOweDunExist(lServIDTemp,OWE_BTYPE_ID_CJ)==1)/*����Ҫ�߽�*/
				continue;
				
		    InsertOweDun(lServIDTemp, OWE_BTYPE_ID_CJ);
		    AddCountLimit(STOP_LIMIT_CJ);
	    }
    }	
	
	if (gstrEvent.m_iErrorID == 14){
	    long lAnotherMember;
	    long lServIDTemp;
	    
	    lServIDTemp = b_rt_stop_detail.lServID;
	    
	    if (GetAnotherMem(lAcctID, b_rt_stop_detail.lServID, lAnotherMember,gstrEvent.m_iErrorID) == -1)
	        Log::log(0,"һ��˫���û�ld%�Ĳ��뷽��������.", b_rt_stop_detail.lServID);
	            
	    for (int i = 0; i < 2; i++, lServIDTemp = lAnotherMember)	    
	    {
	        if (lServIDTemp == 0)
	            continue;
	            
	        if (GetOweSpecialServ(lServIDTemp,OWE_BTYPE_ID_ST))
		        continue;
			
		    /*��Ҫ���ǵ����ڴ��ͺ��ڱ�����*/
		    if ( !GetServState(lServIDTemp, sState, &iBillingModeID, &iProductID) )
			    continue;

		    //�ж��û��Ƿ����ڲ�ͣ���Ĳ�Ʒ
        if (GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_ST,iBillingModeID)) 
          continue;
		
		    //CheckServState�������Ӷ�˫ͣ״̬���ж�
		    if (CheckServState(sState,OWE_BTYPE_ID_ST)==0) 
		        continue;
		
			if (i==1)
			{
				giBalanceCnt = 0;
				glBalance = LocalFunction(lAcctID,lServIDTemp,b_rt_stop_detail.iBillingCycleID,-1,-1);
			}

		    InsertWorkOrder(lServIDTemp,OWE_BTYPE_ID_ST,sState);
		    UpdateServState(lServIDTemp,OWE_BTYPE_ID_ST,STOP_STATE,sState);
   		    AddCountLimit(STOP_LIMIT_ST);
	    }
    }
    
    if (gstrEvent.m_iErrorID == 15){        
        if(CheckAutoFunds(b_rt_stop_detail.lServID))
        {
            InsertOrderBank(b_rt_stop_detail.lServID,1);
            return;    
        }        
        
        int iRet = CheckOweDunIPExist(b_rt_stop_detail.lServID);
        
        if (iRet == 3)
            return;
        else
            InsertIPOweDun(b_rt_stop_detail.lServID, 1);
    }
    if (gstrEvent.m_iErrorID==16){/*�ʻ��������*/
    	OweDunFromAcct(lAcctID,OWE_BTYPE_ID_TX);
    }
    
    if (gstrEvent.m_iErrorID==17){/*�û��������*/
    	if ( !GetServState(b_rt_stop_detail.lServID,sState,&iBillingModeID,&iProductID) )
				return;
				
			if (CheckServState(sState,OWE_BTYPE_ID_TX)==0) 
			    return;
			    
			if (GetOweSpecialServ(b_rt_stop_detail.lServID,OWE_BTYPE_ID_TX))
				return;
	
			//�ж��û�
	    if (!GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_TX,iBillingModeID)) 
	    	 return;
				
			if (CheckOweDunExist(b_rt_stop_detail.lServID,OWE_BTYPE_ID_TX)==1)/*����Ҫ�߽�*/
				return;	
					
			InsertOweDun(b_rt_stop_detail.lServID,OWE_BTYPE_ID_TX);
			AddCountLimit(STOP_LIMIT_CJ);
    }

	//һ��˫����ͣ
	if (gstrEvent.m_iErrorID == 18){
	    long lAnotherMember;
	    long lServIDTemp;
	    
	    lServIDTemp = b_rt_stop_detail.lServID;
	    
	    if (GetAnotherMem(lAcctID, b_rt_stop_detail.lServID, lAnotherMember,gstrEvent.m_iErrorID) == -1)
	        Log::log(0,"һ��˫���û�ld%�Ĳ��뷽��������.", b_rt_stop_detail.lServID);
	            
	    for (int i = 0; i < 2; i++, lServIDTemp = lAnotherMember)	    
	    {
	        if (lServIDTemp == 0)
	            continue;
	            
	        if (GetOweSpecialServ(lServIDTemp,OWE_BTYPE_ID_TJ))
		        continue;
			
		    /*��Ҫ���ǵ����ڴ��ͺ��ڱ�����*/
		    if ( !GetServState(lServIDTemp, sState, &iBillingModeID, &iProductID) )
			    continue;

		    //�ж��û��Ƿ����ڲ�ͣ���Ĳ�Ʒ
        if (GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_TJ,iBillingModeID)) 
          continue;
		
		    //CheckServState�������Ӷ�˫ͣ״̬���ж�
		    if (CheckServState(sState,OWE_BTYPE_ID_TJ)==0) 
		        continue;
		
			if (i==1)
			{
				giBalanceCnt = 0;
				glBalance = LocalFunction(lAcctID,lServIDTemp,b_rt_stop_detail.iBillingCycleID,-1,-1);
			}
			
			if(gstrEvent.m_oChargeInfo[0].m_lCharge >STOP_HIGH_FEE)
			{			 	
			 	DealWithHighFee(b_rt_stop_detail.lServID,OWE_BTYPE_ID_ST,sState);
			} 		
			else
			{
		    	InsertWorkOrder(lServIDTemp,OWE_BTYPE_ID_TJ,sState);
		   	 	UpdateServState(lServIDTemp,OWE_BTYPE_ID_TJ,OWE_STATE,sState);
   		    	AddCountLimit(STOP_LIMIT_TJ);
	    	}
	    }
    }
    
    //�û�ͣ�������öȿۼ�
    if (gstrEvent.m_iErrorID == 19){
    	if (GetOweSpecialServ(gstrEvent.m_iServID,OWE_BTYPE_ID_TJ))
			return;
	
			/*��Ҫ���ǵ����ڴ��ͺ��ڱ�����*/
			if ( !GetServState(b_rt_stop_detail.lServID,sState,&iBillingModeID,&iProductID) )
				return;

			//�ж��û��Ƿ����ڲ�ͣ���Ĳ�Ʒ
    	if (GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_TJ,iBillingModeID)) 
    	  	  return;
		
			if (CheckServState(sState,OWE_BTYPE_ID_TJ)==0) 
		  	  return;

	  	//���ӿۼ����öȺ���,����¼��־��,ͬʱ��ͣ������
	  	InsertCreditLog(b_rt_stop_detail.lServID,2000,sState);
  	}
  	
#ifdef DEF_JIANGSU

  	if (gstrEvent.m_iErrorID==20){/*CDMA�û��߽�*/
		if ( !GetServState(b_rt_stop_detail.lServID,sState,&iBillingModeID,&iProductID) )
			return;
			
		if (CheckServState(sState,OWE_BTYPE_ID_CJ)==0) 
		    return;
		    
		if (GetOweSpecialServ(b_rt_stop_detail.lServID,OWE_BTYPE_ID_CJ))
			return;

		//�ж��û�
    if (!GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_CJ,iBillingModeID)) 
    	 return;
			
		if (CheckOweDunExist(b_rt_stop_detail.lServID,OWE_BTYPE_ID_CR)==1)/*����Ҫ�߽�*/
			return;
		
//		if (giAcctCredit==0)
//			giAcctCredit=G_PUSERINFO->getAcctCreditAmount (lAcctID);
		
		InsertOweDun(b_rt_stop_detail.lServID,OWE_BTYPE_ID_CR);
		AddCountLimit(STOP_LIMIT_CJ);
    }

#else

    //�������ö��ж�,�������������û�˫ͣ
    if (gstrEvent.m_iErrorID == 20){
    	if (GetOweSpecialServ(gstrEvent.m_iServID,OWE_BTYPE_ID_ST))
			return;
	
			/*��Ҫ���ǵ����ڴ��ͺ��ڱ�����*/
			if ( !GetServState(b_rt_stop_detail.lServID,sState,&iBillingModeID,&iProductID) )
				return;

			//�ж��û��Ƿ����ڲ�ͣ���Ĳ�Ʒ
    	if (GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_ST,iBillingModeID)) 
    	  	  return;
		
			if (CheckServState(sState,OWE_BTYPE_ID_ST)==0) 
		  	  return;
			
			giAcctCredit = GetServCreditDB(b_rt_stop_detail.lServID);
			
			//����Ҫ������ۿ������+���ö�>0,�Ͳ�ͣ��
		  if (glBalance+giAcctCredit>0) return;
		  if(gstrEvent.m_oChargeInfo[0].m_lCharge >STOP_HIGH_FEE)
		 {		 	
			DealWithHighFee(b_rt_stop_detail.lServID,OWE_BTYPE_ID_ST,sState);
		} 	
		else
		{
		  	
		  InsertWorkOrder(b_rt_stop_detail.lServID,OWE_BTYPE_ID_ST,sState);
			UpdateServState(b_rt_stop_detail.lServID,OWE_BTYPE_ID_ST,STOP_STATE,sState);
			AddCountLimit(STOP_LIMIT_ST); 
  		}
  	}

#endif

	if (gstrEvent.m_iErrorID==22){/*CDMAС������*/
		if ( !GetServState(b_rt_stop_detail.lServID,sState,&iBillingModeID,&iProductID) )
			return;
			
		if (CheckServState(sState,OWE_BTYPE_ID_CJ)==0) 
		    return;
		    
		if (GetOweSpecialServ(b_rt_stop_detail.lServID,OWE_BTYPE_ID_CJ))
			return;

		//�ж��û�
    if (!GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_CJ,iBillingModeID)) 
    	 return;
			
		if (CheckOweDunExistCDMA(b_rt_stop_detail.lServID,OWE_BTYPE_ID_CJ)==1)/*����Ҫ�߽�*/
			return;	
				
		InsertOweDun(b_rt_stop_detail.lServID,OWE_BTYPE_ID_CJ);
		AddCountLimit(STOP_LIMIT_CJ);
	}
	
	if (gstrEvent.m_iErrorID==23){/*������Ʒͣ��*/
		loadOfferDetail(b_rt_stop_detail.lServID,gstrEvent.m_iErrorID);
		
		vector<struct B_STOP_OFFER>::iterator itr;
		long lTmpServID;
		int  iTmpTypeID;
		
		for(itr=m_strStopOffer.begin();itr!=m_strStopOffer.end();itr++)
		{
			lTmpServID=(*itr).lServID;
			iTmpTypeID=(*itr).iOweBTypeID;
		
			if (GetOweSpecialServ(lTmpServID,iTmpTypeID))
				continue;
			if ( !GetServState(lTmpServID,sState,&iBillingModeID,&iProductID) )
				continue;
			if (GetOweSpecialProduct(iProductID,iTmpTypeID,iBillingModeID)) 
				continue;
			
			#ifdef DEF_SICHUAN  /*�Ĵ���������Ʒͣ����,Ҫ��ſ���δ����״̬��ͣ��*/
			 if (strcmp(sState,"2HN")!=0)
			 	  if(CheckServState(sState,iTmpTypeID)==0) continue;
			#else
			if (CheckServState(sState,iTmpTypeID)==0) 
				continue;
			#endif
			
			if (iTmpTypeID == OWE_BTYPE_ID_CJ){
				if (CheckOweDunExist(lTmpServID,OWE_BTYPE_ID_CJ)==1)/*����Ҫ�߽�*/
					continue;	
				InsertOweDun(lTmpServID,OWE_BTYPE_ID_CJ);
				AddCountLimit(STOP_LIMIT_CJ);
			}
			if (iTmpTypeID == OWE_BTYPE_ID_TJ){
				if(gstrEvent.m_oChargeInfo[0].m_lCharge >STOP_HIGH_FEE)
				{		 	
					 DealWithHighFee(b_rt_stop_detail.lServID,OWE_BTYPE_ID_ST,sState);
				} 	
				else
				{
					InsertWorkOrder(lTmpServID,OWE_BTYPE_ID_TJ,sState);
					UpdateServState(lTmpServID,OWE_BTYPE_ID_TJ,OWE_STATE,sState);
					AddCountLimit(STOP_LIMIT_TJ);
				}
			}	
			if (iTmpTypeID == OWE_BTYPE_ID_ST){
				if(gstrEvent.m_oChargeInfo[0].m_lCharge >STOP_HIGH_FEE)
				{		 	
					 DealWithHighFee(b_rt_stop_detail.lServID,OWE_BTYPE_ID_ST,sState);
				} 	
				else
				{
					InsertWorkOrder(lTmpServID,OWE_BTYPE_ID_ST,sState);
					UpdateServState(lTmpServID,OWE_BTYPE_ID_ST,STOP_STATE,sState);
					AddCountLimit(STOP_LIMIT_ST);
				}
			}
		}
	}
}

void Rt_Stop_Make::InsertCreditLog(long lServID,int iMaxCreditValue,char sState[])
{
	DEFINE_QUERY(qry);
	char sSql[2000];
	int iCreditValue;
	sprintf(sSql,"select sum(nvl(CREDIT_AMOUNT,0)) credit_amount from acct_credit where serv_id=%ld "
	" AND sysdate BETWEEN nvl(eff_date,sysdate-1) and nvl(exp_date,sysdate+1) ",lServID);
	
	qry.close();
  	qry.setSQL(sSql);
  	qry.open();
  	if (!qry.next()){
		iCreditValue=0;
	}
	else
	{
		iCreditValue=qry.field("CREDIT_AMOUNT").asInteger();
	}
	
	//����Ҫ������ۿ������+���ö�>=0,�Ͳ�ͣ��
	if (glBalance+iCreditValue>=0) return;
	
	InsertWorkOrder(b_rt_stop_detail.lServID,OWE_BTYPE_ID_TJ,sState);
	UpdateServState(b_rt_stop_detail.lServID,OWE_BTYPE_ID_TJ,OWE_STATE,sState);
	AddCountLimit(STOP_LIMIT_TJ);
}

void Rt_Stop_Make::InsertWorkOrder(long lServID,int iOweBTypeID,char *sState)
{
		int iStaffID;
  	    long OrderSerialNbr;
		DEFINE_QUERY(qry);
		char sSql[2000];
		int  isInsertLog=INSERT_WORK_ORDER_LOG;
		
		iStaffID=DEF_STAFF_ID;
		OrderSerialNbr=GetSerialNbr();
		sprintf(sSql,"INSERT INTO A_WORK_ORDER (ORDER_SERIAL_NBR,OWE_BUSINESS_TYPE_ID,STAFF_ID,SERV_ID,CREATED_DATE ,COMPLETED_DATE,STATE,STATE_DATE,PROCESS_COUNT,PRE_SERV_STATE,REMARK,CHARGE,BALANCE,CREDIT,OWE_TASK_ITEM_ID,SERV_ITME_BALANCE,ACCT_ITME_BALANCE) VALUES (:OrderSerialNbr,:OWE_BUSINESS_TYPE_ID,:STAFF_ID,:SERV_ID,SYSDATE,NULL,'P0C',SYSDATE,NULL,:PRE_SERV_STATE,NULL,:CHARGE,:BALANCE,:CREDIT,:OWE_TASK_ITEM_ID,:SERV_ITME_BALANCE,:ACCT_ITME_BALANCE )");
		
		qry.close();
		qry.setSQL(sSql);
		qry.setParameter("OrderSerialNbr",OrderSerialNbr);
		qry.setParameter("OWE_BUSINESS_TYPE_ID",iOweBTypeID);
		qry.setParameter("STAFF_ID",iStaffID);
		qry.setParameter("SERV_ID",lServID);
		qry.setParameter("PRE_SERV_STATE",sState);
		qry.setParameter("CHARGE",glOweCharge+glAggrCharge);
		qry.setParameter("BALANCE",glBalance);  //add by yangch 20070612
		qry.setParameter("CREDIT",giAcctCredit);
		qry.setParameter("OWE_TASK_ITEM_ID",0);
		qry.setParameter("SERV_ITME_BALANCE",lservResItemBalance); //add by zhougc 20110409
		qry.setParameter("ACCT_ITME_BALANCE",lacctResItemBalance);//add by zhougc 20110409
		qry.execute();Log::log(0,"CHARGE=%d",glAggrCharge);

        //���մ�work_order_log
		if (isInsertLog)
		{
			sprintf(sSql,"INSERT INTO A_WORK_ORDER_LOG (ORDER_SERIAL_NBR,OWE_BUSINESS_TYPE_ID,STAFF_ID,SERV_ID,CREATED_DATE ,COMPLETED_DATE,STATE,STATE_DATE,PROCESS_COUNT,PRE_SERV_STATE,REMARK,CHARGE,BALANCE,CREDIT,OWE_TASK_ITEM_ID,SERV_ITME_BALANCE,ACCT_ITME_BALANCE) VALUES (:OrderSerialNbr,:OWE_BUSINESS_TYPE_ID,:STAFF_ID,:SERV_ID,SYSDATE,NULL,'P0C',SYSDATE,NULL,:PRE_SERV_STATE,NULL,:CHARGE,:BALANCE,:CREDIT,:OWE_TASK_ITEM_ID,:SERV_ITME_BALANCE,:ACCT_ITME_BALANCE )");
		
			qry.close();
			qry.setSQL(sSql);
			qry.setParameter("OrderSerialNbr",OrderSerialNbr);
			qry.setParameter("OWE_BUSINESS_TYPE_ID",iOweBTypeID);
			qry.setParameter("STAFF_ID",iStaffID);
			qry.setParameter("SERV_ID",lServID);
			qry.setParameter("PRE_SERV_STATE",sState);
			qry.setParameter("CHARGE",glOweCharge+glAggrCharge);
			qry.setParameter("BALANCE",glBalance);  //add by yangch 20070612
			qry.setParameter("CREDIT",giAcctCredit);
			qry.setParameter("OWE_TASK_ITEM_ID",0);
			qry.setParameter("SERV_ITME_BALANCE",lservResItemBalance);
			qry.setParameter("ACCT_ITME_BALANCE",lservResItemBalance);
			qry.execute();
		}
        
        backupData(iOweBTypeID);
}

//�û�����
void Rt_Stop_Make::OpenWorkOrder(const long lServID,int iOweBTypeID)
{
	int iStaffID,iBillingModeID,iProductID;
	DEFINE_QUERY(qry);
	DEFINE_QUERY(qryInsert);
	char sState[5];

    iStaffID=DEF_STAFF_ID;

    if ( !GetServState(lServID,sState,&iBillingModeID,&iProductID) )
		return;
			
	if (strcmp(sState,"2HS")!=0&&strcmp(sState,"2HD")!=0)
		return;
	
	#ifdef DEF_SICHUAN
		char *attrVal = G_PSERV->getAttr(10001);
		if(!strcmp(attrVal,"1"))return;
	#endif
			
	InsertWorkOrder(lServID,iOweBTypeID,sState);
	UpdateServState(lServID,iOweBTypeID,OPEN_STATE,sState);
}

void Rt_Stop_Make::OpenCommit()
{
	DEFINE_QUERY(qry1);
	TOCIQuery qry(&gpDBLink_STOP);

	
	qry.commit();
	qry.close();
	qry1.commit();
	qry1.close();
}

//�ʻ�����  ��ֻ���뵽�������У�stopopen��ʱ�ӱ��л�ȡ�������������и���
void Rt_Stop_Make::OpenWorkOrderFromAcct(const long lAcctID,int iOweBTypeID)
{
		int iStaffID,iBillingModeID,iProductID;
		DEFINE_QUERY(qry);
		DEFINE_QUERY(qryInsert);
		char sState[10];
		vector<ServAcctInfo> vPayInfo;
		vector<ServAcctInfo>::iterator iterPay;

        iStaffID=DEF_STAFF_ID;

		if (!G_PUSERINFO->getPayInfo (vPayInfo, lAcctID)) return;

		HashList<int> oHashList (10000);
		int iTemp;

		for (iterPay=vPayInfo.begin(); iterPay!=vPayInfo.end(); iterPay++) {
			if (oHashList.get ((*iterPay).m_lServID, &iTemp)) continue;
			oHashList.add ((*iterPay).m_lServID, 1);
			iServCnt++;

	  	if ( !GetServState((*iterPay).m_lServID,sState,&iBillingModeID,&iProductID) )
			continue;
			
		if (strcmp(sState,"2HS")!=0&&strcmp(sState,"2HD")!=0)
			continue;
		
		InsertWorkOrder((*iterPay).m_lServID,iOweBTypeID,sState);
		UpdateServState((*iterPay).m_lServID,iOweBTypeID,OPEN_STATE,sState);
	  	
	  }
}

//�ʻ�ͣ��
void Rt_Stop_Make::WorkOrderFromAcct(const long lAcctID,int iOweBTypeID)
{
		int iStaffID,iBillingModeID,iProductID;
		DEFINE_QUERY(qry);
		DEFINE_QUERY(qryInsert);
		char sState[10];
		vector<ServAcctInfo> vPayInfo;
		vector<ServAcctInfo>::iterator iterPay;

        iStaffID=DEF_STAFF_ID;

		if (!G_PUSERINFO->getPayInfo (vPayInfo, lAcctID)) return;

		HashList<int> oHashList (10000);
		int iTemp;

		for (iterPay=vPayInfo.begin(); iterPay!=vPayInfo.end(); iterPay++) {
			if (oHashList.get ((*iterPay).m_lServID, &iTemp)) continue;
			oHashList.add ((*iterPay).m_lServID, 1);
			iServCnt++;
	  	
	  	if (iOweBTypeID == OWE_BTYPE_ID_TJ)
	  	{
    	  	if (GetOweSpecialServ((*iterPay).m_lServID,OWE_BTYPE_ID_TJ))
			    	continue;
	  	
	  	    if ( !GetServState((*iterPay).m_lServID,sState,&iBillingModeID,&iProductID) )
			    	continue;
			
					//�ж��û��Ƿ����ڲ�ͣ���Ĳ�Ʒ
					if (GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_TJ,iBillingModeID)) 
			    	continue;
			
			if (CheckServState(sState,OWE_BTYPE_ID_TJ)==0) continue;

			InsertWorkOrder((*iterPay).m_lServID,iOweBTypeID,sState);
			UpdateServState((*iterPay).m_lServID,iOweBTypeID,OWE_STATE,sState);
            AddCountLimit(STOP_LIMIT_TJ);	  	 
               
	  	}else{  //add by yangch for SuZhou
	  	    if (GetOweSpecialServ((*iterPay).m_lServID,OWE_BTYPE_ID_TJ))
			    continue;
			
			
	  	
    	  	if ( !GetServState((*iterPay).m_lServID,sState,&iBillingModeID,&iProductID) )
				continue;
			
				//�ж��û��Ƿ����ڲ�ͣ���Ĳ�Ʒ
				if (GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_ST,iBillingModeID)) 
			    continue;
			    
    		if (CheckServState(sState,OWE_BTYPE_ID_ST)==0) continue;

			InsertWorkOrder((*iterPay).m_lServID,iOweBTypeID,sState);
			UpdateServState((*iterPay).m_lServID,iOweBTypeID,STOP_STATE,sState);
            AddCountLimit(STOP_LIMIT_ST);			
	  	}
	  	    
	  }
}

long Rt_Stop_Make::GetSerialNbr()
{
  long lOrderSerialNbr;
  DEFINE_QUERY(qry);
	char sSql[2000];
  
  qry.close();
  qry.setSQL("SELECT SEQ_ORDER_SERIAL_NBR.NEXTVAL SEQ FROM DUAL");
  qry.open();
  if (!qry.next()){
		throw TException(sSql);
	}
	else{
		lOrderSerialNbr=qry.field("SEQ").asLong();
	}
	
  return lOrderSerialNbr;
}


/*��ͣ��**/
int Rt_Stop_Make::GetOweSpecialServ(long lServID,int iType)
{
  DEFINE_QUERY(qry);
  
#ifdef CHECK_A_IVPN_SERV_SPECIAL

	char sTempsql[1024]={0};
	int iSum = 0;
	
	sprintf(sTempsql, "select count(*)from A_IVPN_SERV_SPECIAL where forbid_uc_sp = 1   "
				" and eff_date < sysdate and exp_date > sysdate and serv_id  = %d",
					lServID);

  qry.close();
	qry.setSQL(sTempsql);
	qry.open();
	bool bexeResult = qry.next();
	if(bexeResult)
		iSum = qry.field(0).asInteger();
	qry.close();
	
	//������Ч��¼
	if(iSum > 0) 
	{
		return 1;
	}
	else
	{        
		 ;
	}
			
#endif
// end #ifdef CHECK_A_IVPN_SERV_SPECIAL

  char 	sSql[2000];
  
  sprintf(sSql,"SELECT SERV_ID FROM A_OWE_SPECIAL_SERV \
    WHERE SERV_ID=%ld \
    AND OWE_BUSINESS_TYPE_ID =%d \
    AND sysdate \
    BETWEEN nvl(eff_date,sysdate-1) and nvl(exp_date,sysdate+1) \
    AND ROWNUM<2",lServID,iType);
    
  qry.close();
  qry.setSQL(sSql);
  qry.open();
  
  if (!qry.next()){
		return 0;
	}
	else{
  	return 1;
	}
}

/*�߽�ͣ����Ʒ��ʶ**/
int Rt_Stop_Make::GetOweSpecialProduct(int iProductID,int iOweBTypeID,int iBillingModeID)
{
  DEFINE_QUERY(qry);
	char 	sSql[2000];
  sprintf(sSql,"SELECT PRODUCT_ID FROM B_OWE_SPECIAL_PRODUCT WHERE PRODUCT_ID=%d	\
  AND OWE_BUSINESS_TYPE_ID =%d AND (NVL(BILLING_MODE_ID,-1)=-1 or nvl(billing_mode_id,-1) =%d)	\
  AND ROWNUM<2",iProductID,iOweBTypeID,iBillingModeID);
  qry.close();
  qry.setSQL(sSql);
  qry.open();
  
  if (!qry.next()){
		return 0;
	}
	else{
  	return 1;
	}
}

/*����û��Ƿ��Ѿ���ͣ����¼������1��2��ʾ�У�����0��ʾû��*/
/*
״̬˵��
ACTION:
1	�û���ͣ
2	�û�˫ͣ
3	����
4	˫��
5	����ͣ������
*/
int Rt_Stop_Make::CheckStopExist(long lServID)
{

	DEFINE_QUERY(qry);
	int iOweBTypeID;
	qry.close();
	qry.setSQL("SELECT OWE_BUSINESS_TYPE_ID FROM A_WORK_ORDER WHERE SERV_ID=:SERV_ID AND STATE='P0C' ORDER BY TO_NUMBER(ORDER_SERIAL_NBR) DESC ");
	qry.setParameter("SERV_ID",lServID);
	qry.open();
	
	if (!qry.next())
		return 0;
	else{
		iOweBTypeID=qry.field("OWE_BUSINESS_TYPE_ID").asInteger();
		/*???? ���Ͳ�֪����û�仯Ҫȷ��*/
		/*�д����������ж�״̬*/
		if (iOweBTypeID==OWE_BTYPE_ID_TJ)
			return OWE_BTYPE_ID_TJ;
	}
	
	return 0;
}

int Rt_Stop_Make::CheckOweDunExist(long lServID,int iOweBusinessTypeID)
{
   char sState[6];
   int  iOweBTypeID;
   long	lCurBalance;
   long	lMaxCharge;
   long	lMinCharge;
   int	iAcctCredit;
   long	lOweCharge;
   int  iOweDays=OWE_DAYS;
   char sSql[2000];

   DEFINE_QUERY(qry);
   
   //�������ѵ�ʱ������
   if (iOweBusinessTypeID==OWE_BTYPE_ID_TX)
   {
   		iOweDays = TX_DAYS;
   }
   
//�޸Ĵ߽ɼ��Ϊ������ suzhou
#ifndef DEF_HAINAN
   sprintf(sSql,"SELECT /*+rule+*/STATE,OWE_BUSINESS_TYPE_ID,MAX_CHARGE,MIN_CHARGE,BALANCE,NVL(OWE_CHARGE,0) OWE_CHARGE,ACCT_CREDIT	\
     FROM	A_OWE_DUN_LOG WHERE SERV_ID=%ld	AND STATE_DATE > SYSDATE - %d	\
     	AND OWE_BUSINESS_TYPE_ID=%d AND STATE<>'I0X' ORDER BY created_date DESC",lServID,iOweDays,iOweBusinessTypeID);
#else
   sprintf(sSql,"SELECT /*+rule+*/STATE,OWE_BUSINESS_TYPE_ID,MAX_CHARGE,MIN_CHARGE,BALANCE,NVL(OWE_CHARGE,0) OWE_CHARGE,ACCT_CREDIT	\
     FROM	A_OWE_DUN_LOG WHERE SERV_ID=%ld	AND STATE_DATE > SYSDATE - %d	\
     AND STATE<>'I0X' ORDER BY created_date DESC",lServID,iOweDays);
#endif
   qry.close();


   qry.setSQL(sSql);

   qry.open();
   if (!qry.next())
			return 0;
	 
	 strcpy(sState,qry.field("STATE").asString());
	 iOweBTypeID=qry.field("OWE_BUSINESS_TYPE_ID").asInteger();
	 lCurBalance = qry.field("BALANCE").asLong();
	 lMaxCharge = qry.field("MAX_CHARGE").asLong();
	 lMinCharge = qry.field("MIN_CHARGE").asLong();
	 lOweCharge = qry.field("OWE_CHARGE").asLong();
	 iAcctCredit = qry.field("ACCT_CREDIT").asInteger();
	 
		//if(iOweBTypeID==OWE_BTYPE_ID_CJ && strcmp(sState,"I0C")==0)
    //	return 1;
		#ifdef DEF_HAINAN
		  //���������ж�,������һ�εĴ߽ɶ���Ϊֹͣ�߽ɻ�ֹͣ����,���ٴδ߽�
		  if (iOweBTypeID == 18 || iOweBTypeID == 7)
		  	return 0;
		#endif
		
		/**��������������ϴ����,������߽�,�����û��ϴν���Ǯ**/
		#ifndef DEF_HAINAN
     if(glBalance>lCurBalance || lOweCharge != glOweCharge )
    	 return 0;
    #endif
    
    /*�ϴη�ֵ�뱾�η�ֵ�Ƚ�,���������ֵ��,�����д߽�**/
    if(lMinCharge==glMinCharge && lMaxCharge==glMaxCharge)
    	return 1;

    return 0;
}

int Rt_Stop_Make::CheckOweDunExistCDMA(long lServID,int iOweBusinessTypeID)
{
   char sState[6];
   int  iOweBTypeID;
   long	lCurBalance;
   long	lMaxCharge;
   long	lMinCharge;
   int	iAcctCredit;
   long	lOweCharge;
   int  iOweDays=OWE_DAYS;
   char sSql[2000];

   DEFINE_QUERY(qry);

   sprintf(sSql,"SELECT /*+rule+*/STATE,OWE_BUSINESS_TYPE_ID,MAX_CHARGE,MIN_CHARGE,BALANCE,NVL(OWE_CHARGE,0) OWE_CHARGE,ACCT_CREDIT	\
     FROM	A_OWE_DUN_LOG WHERE SERV_ID=%ld	\
     	AND OWE_BUSINESS_TYPE_ID=%d AND STATE<>'I0X' ORDER BY created_date DESC",lServID,iOweBusinessTypeID);

   qry.close();

   qry.setSQL(sSql);

   qry.open();
   if (!qry.next())
			return 0;
	 
	 strcpy(sState,qry.field("STATE").asString());
	 iOweBTypeID=qry.field("OWE_BUSINESS_TYPE_ID").asInteger();
	 lCurBalance = qry.field("BALANCE").asLong();
	 lMaxCharge = qry.field("MAX_CHARGE").asLong();
	 lMinCharge = qry.field("MIN_CHARGE").asLong();
	 lOweCharge = qry.field("OWE_CHARGE").asLong();
	 iAcctCredit = qry.field("ACCT_CREDIT").asInteger();
		
		/**��������������ϴ����,������߽�,�����û��ϴν���Ǯ**/
    if(glBalance>lCurBalance || lOweCharge != glOweCharge )
    	return 0;
    	
    /*�ϴη�ֵ�뱾�η�ֵ�Ƚ�,���������ֵ��,�����д߽�**/
    if(lMinCharge==glMinCharge && lMaxCharge==glMaxCharge)
    	return 1;

    return 0;
}

void Rt_Stop_Make::UpdateServState(long lServID, int iOweBTypeID, char sState[],char sPreState[])
{
    long lServUseProc=0;
    char sInsertState[10];
    lServUseProc = SERV_USE_PROC;

	DEFINE_QUERY(qry);
	qry.close();
    
    strcpy(sInsertState,sState);
    
	#ifdef DEF_HAINAN
    if(strcmp(sPreState,"2HC")==0)
    {
    	if(iOweBTypeID == OWE_BTYPE_ID_TJ)
    	{ 			
    			strcpy(sInsertState,"2IS");
    	}
    	if(iOweBTypeID == OWE_BTYPE_ID_ST)
    	{				
    			strcpy(sInsertState,"2ID");    			
    	}
    }
    #endif
     
    #ifdef DEF_GUIZHOU
    if(strcmp(sPreState,"2HC")==0) strcpy(sInsertState,"2IS");
    #endif
    
    
    if (lServUseProc==1)
    {
        qry.setSQL("BEGIN OWE_UPDATE_SERV(:SERV_ID,:OWEBTYPEID,:STATE); END;");
        qry.setParameter("SERV_ID",lServID);
        qry.setParameter("OWEBTYPEID",iOweBTypeID);
        qry.setParameter("STATE",sInsertState);
    }
    else
    {
        qry.setSQL("UPDATE SERV SET STATE=:STATE,STATE_DATE=SYSDATE WHERE SERV_ID=:SERV_ID");
        qry.setParameter("SERV_ID",lServID);
        qry.setParameter("STATE",sInsertState);
    }

    qry.execute();

}

long Rt_Stop_Make::GetOweDunID()
{
  long lOweDunID;
  DEFINE_QUERY(qry);
  qry.close();
  qry.setSQL("SELECT SEQ_OWE_DUN_ID.NEXTVAL SEQ FROM DUAL");
  qry.open();
  if (!qry.next()){
		throw TException("SELECT SEQ_OWE_DUN_ID.NEXTVAL SEQ FROM DUAL");
	}
	else{
		lOweDunID=qry.field("SEQ").asLong();
	}
	
  return lOweDunID;
}

void Rt_Stop_Make::InsertOweDun(long lServID,int iOweBTypeID)
{
	int iStaffID;
  long lOweDunID;
  DEFINE_QUERY(qry);
	char sSql[2000];
  
  iStaffID=DEF_STAFF_ID;
	lOweDunID = GetOweDunID();

	sprintf(sSql,"INSERT INTO A_OWE_DUN (	\
			OWE_DUN_ID,OWE_BUSINESS_TYPE_ID,STAFF_ID,SERV_ID,BILLING_CYCLE_ID ,	\
			CHARGE,DUE,CREATED_DATE ,STATE,STATE_DATE,OWE_TASK_ITEM_ID,MAX_CHARGE,	\
			MIN_CHARGE,BALANCE,ACCT_CREDIT,OWE_CHARGE) \
			VALUES (:OWE_DUN_ID,:OWE_BUSINESS_TYPE_ID,:STAFF_ID,:SERV_ID,:BILLING_CYCLE_ID,	\
				:CHARGE,:DUE,SYSDATE,'I0C',SYSDATE,:OWE_TASK_ITEM_ID,:MAX_CHARGE,	\
				:MIN_CHARGE,:BALANCE,:ACCT_CREDIT ,:OWE_CHARGE )");
	qry.close();
	qry.setSQL(sSql);
	qry.setParameter("OWE_DUN_ID",lOweDunID);
	qry.setParameter("OWE_BUSINESS_TYPE_ID",iOweBTypeID);
	qry.setParameter("STAFF_ID",iStaffID);
	qry.setParameter("SERV_ID",lServID);
	
	qry.setParameter("BILLING_CYCLE_ID",gstrEvent.m_iBillingCycleID);
	qry.setParameter("CHARGE",glOweCharge+glAggrCharge);
	qry.setParameter("DUE",0);
	qry.setParameter("OWE_TASK_ITEM_ID",0);
	
	qry.setParameter("MAX_CHARGE",glMaxCharge);
	qry.setParameter("MIN_CHARGE",glMinCharge);
	qry.setParameter("BALANCE",glBalance);  //add by yangch 20070612
	qry.setParameter("ACCT_CREDIT",giAcctCredit);
	qry.setParameter("OWE_CHARGE",glOweCharge);
	qry.execute();


	sprintf(sSql,"INSERT INTO A_OWE_DUN_LOG (	\
		OWE_DUN_LOG_ID,OWE_BUSINESS_TYPE_ID,STAFF_ID,SERV_ID,BILLING_CYCLE_ID ,	\
		CHARGE,DUE,CREATED_DATE ,STATE,STATE_DATE,OWE_TASK_ITEM_ID,MAX_CHARGE,	\
		MIN_CHARGE,BALANCE,ACCT_CREDIT,OWE_CHARGE,SOURCE_SERV_ID) \
		VALUES (:OWE_DUN_LOG_ID,:OWE_BUSINESS_TYPE_ID,:STAFF_ID,:SERV_ID,:BILLING_CYCLE_ID,\
			:CHARGE,:DUE,SYSDATE,'I0C',SYSDATE,:OWE_TASK_ITEM_ID,:MAX_CHARGE,	\
			:MIN_CHARGE,:BALANCE,:ACCT_CREDIT,:OWE_CHARGE,:SOURCE_SERV_ID)");
	qry.close();
	qry.setSQL(sSql);
	qry.setParameter("OWE_DUN_LOG_ID",lOweDunID);
	qry.setParameter("OWE_BUSINESS_TYPE_ID",iOweBTypeID);
	qry.setParameter("STAFF_ID",iStaffID);
	qry.setParameter("SERV_ID",lServID);
	
	qry.setParameter("BILLING_CYCLE_ID",gstrEvent.m_iBillingCycleID);
	qry.setParameter("CHARGE",glOweCharge+glAggrCharge);
	qry.setParameter("DUE",0);
	qry.setParameter("OWE_TASK_ITEM_ID",0);

	qry.setParameter("MAX_CHARGE",glMaxCharge);
	qry.setParameter("MIN_CHARGE",glMinCharge);
	qry.setParameter("BALANCE",glBalance);
	qry.setParameter("ACCT_CREDIT",giAcctCredit);
	qry.setParameter("OWE_CHARGE",glOweCharge);
	qry.setParameter("SOURCE_SERV_ID",glSourceServID);
	qry.execute();

	//Value value;
	//IDParser::getValue((EventSection *)(&b_rt_stop_detail.StdEvent_Data), "51900" , &value);
	//glBalance = value.m_lValue;
    backupData(iOweBTypeID);
}

//�ʻ��߽�
void Rt_Stop_Make::OweDunFromAcct(const long lAcctID,int iOweBTypeID)
{
	int iStaffID;
	int	iBillingModeID,iProductID;
  DEFINE_QUERY(qry);
	char	sState[10];
	int iCnt,iInsertFlag;
	int iFlag = 0;
	vector<ServAcctInfo> vPayInfo;
	vector<ServAcctInfo>::iterator iterPay;

  iStaffID=DEF_STAFF_ID;

	if (!G_PUSERINFO->getPayInfo (vPayInfo, lAcctID)) return;

	HashList<int> oHashList (10000);
	int iTemp;

	if (iOweBTypeID == OWE_BTYPE_ID_CJ)
	{
		CheckOweDunCnt(lAcctID,&iCnt,&iInsertFlag);
	
		if (iCnt > MAX_OWE_DUN_CNT) return;
	}

	for (iterPay=vPayInfo.begin(); iterPay!=vPayInfo.end(); iterPay++) {
		
		if (oHashList.get ((*iterPay).m_lServID, &iTemp)) continue;
		oHashList.add ((*iterPay).m_lServID, 1);
		iServCnt++;
		if (GetOweSpecialServ((*iterPay).m_lServID,iOweBTypeID))	continue;
			
		if (CheckOweDunExist((*iterPay).m_lServID,iOweBTypeID)==1)/*����Ҫ�߽�*/
				continue;
		
		if ( !GetServState((*iterPay).m_lServID,sState,&iBillingModeID,&iProductID) )
					continue;
		if (CheckServState(sState,iOweBTypeID)==0) continue;

		if (!GetOweSpecialProduct(iProductID,iOweBTypeID,iBillingModeID)) continue;
		iFlag = 1;
		InsertOweDun((*iterPay).m_lServID,iOweBTypeID);
		AddCountLimit(STOP_LIMIT_CJ);
	}


	if (iFlag == 1 && iOweBTypeID == OWE_BTYPE_ID_CJ){
		
		if (iInsertFlag == 1) InsertOweDunCnt(lAcctID,glBalance,iCnt);
		else UpdateOweDunCnt(lAcctID,glBalance,iCnt);
	}

}

//�ж��û��Ĵ߽ɴ���
void Rt_Stop_Make::CheckOweDunCnt(long lAcctID,int *iCnt,int *iInsertFlag)
{
	DEFINE_QUERY(qry);
	char sSelectSql[2000];
	long  lBalance;
	long  lOweCharge;
	int  iAcctCredit;
	
	*iInsertFlag = 0;
	*iCnt = 0;
	sprintf(sSelectSql,"SELECT OWE_DUN_CNT,BALANCE,OWE_CHARGE,ACCT_CREDIT FROM B_ACCT_OWE_DUN WHERE ACCT_ID=:ACCT_ID AND STATE_DATE>sysdate-30");
  qry.setSQL(sSelectSql);
  qry.setParameter("ACCT_ID",lAcctID);
  qry.open();
  if (qry.next())
  {
  	*iCnt = qry.field("OWE_DUN_CNT").asInteger();
  	lBalance = qry.field("BALANCE").asLong();
  	lOweCharge = qry.field("OWE_CHARGE").asLong();
  	iAcctCredit = qry.field("ACCT_CREDIT").asInteger();
  }
  else
  {
  	*iCnt = 0;
  	lBalance = 0;
  	*iInsertFlag = 1;
  }
  if (lBalance != glBalance || lOweCharge != glOweCharge || iAcctCredit!=giAcctCredit) *iCnt = 1;//�������Ѿ��������,�����´߽�
  	else if (*iCnt >= MAX_OWE_DUN_CNT) *iCnt = *iCnt + 1;
   else *iCnt = *iCnt + 1;

	qry.close();
	
}

//�����û�˫ͣ���ʹ߽ɵĹ���
void Rt_Stop_Make::OweDunAndWorkOrderServ(const long lServID)
{
	int iStaffID;
	int	iBillingModeID,iProductID;
	int	iFlag;
    
    DEFINE_QUERY(qry);
    DEFINE_QUERY(qrySelect);
	char sState[10];

	iFlag = 0;//�жϸ��ʻ����Ƿ����û��߽�,Ϊ1���з����߽�
    iStaffID=DEF_STAFF_ID;

    if ( !GetServState(lServID,sState,&iBillingModeID,&iProductID) )
		return;
		
	if (!GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_CJ,iBillingModeID))
	    iFlag = -1;
	else if (GetOweSpecialServ(lServID,OWE_BTYPE_ID_CJ)) 
	    iFlag = -1;
	    
	//if (strcmp(sState,"2HC")!=0&&strcmp(sState,"2HE")!=0&&strcmp(sState,"2HX")!=0&&strcmp(sState,"2IX")!=0&&iFlag == 0) 
	if (CheckServState(sState,OWE_BTYPE_ID_CJ)==1 && iFlag == 0 )
	{
						//ͣ���û��Ĵ߽ɷ�ֵ����Ϊ0-Ƿ�ѽ��
						glMaxCharge = 0;
						glMinCharge = glBalance-glOweCharge-glAggrCharge;
						InsertOweDun(lServID,OWE_BTYPE_ID_CJ);
						iFlag = 1;
						AddCountLimit(STOP_LIMIT_CJ);
	}
	//�ж��Ƿ���ͣ��,��δͣ��,��ͣ������
	if (GetOweSpecialServ(lServID,OWE_BTYPE_ID_TJ))
				return;

	//�ж��û��Ƿ����ڲ�ͣ���Ĳ�Ʒ
	if (GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_TJ,iBillingModeID)) return;

	//if (strcmp(sState,"2HC")==0||strcmp(sState,"2HD")==0||strcmp(sState,"2HS")==0||strcmp(sState,"2HE")==0||strcmp(sState,"2HX")==0||strcmp(sState,"2IX")==0||strcmp(sState,"2PX")==0) return;
	if (CheckServState(sState,OWE_BTYPE_ID_TJ)==0) return;
	InsertWorkOrder(lServID,OWE_BTYPE_ID_TJ,sState);
	UpdateServState(lServID,OWE_BTYPE_ID_TJ,OWE_STATE,sState);
    AddCountLimit(STOP_LIMIT_TJ);
}

//�����ʻ�ͣ���ʹ߽ɵĹ���
void Rt_Stop_Make::OweDunAndWorkOrder(const long lAcctID)
{
	int iStaffID;
	int	iBillingModeID,iProductID;
	int iCnt;
	int iInsertFlag;
	int	iFlag;
    DEFINE_QUERY(qry);
    DEFINE_QUERY(qrySelect);
	char sState[10];
	vector<ServAcctInfo> vPayInfo;
	vector<ServAcctInfo>::iterator iterPay;

	iFlag = 0;//�жϸ��ʻ����Ƿ����û��߽�,Ϊ1���з����߽�
    iStaffID=DEF_STAFF_ID;

	if (!G_PUSERINFO->getPayInfo (vPayInfo, lAcctID)) return;

	HashList<int> oHashList (10000);
	int iTemp;
	
	CheckOweDunCnt(lAcctID,&iCnt,&iInsertFlag);
	
	for (iterPay=vPayInfo.begin(); iterPay!=vPayInfo.end(); iterPay++) {
			
		if (oHashList.get ((*iterPay).m_lServID, &iTemp)) continue;
		oHashList.add ((*iterPay).m_lServID, 1);
  	iServCnt++;
  	if ( !GetServState((*iterPay).m_lServID,sState,&iBillingModeID,&iProductID) )
					continue;
		
		if (iCnt <= MAX_OWE_DUN_CNT )
		{
			if (!GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_CJ,iBillingModeID)) iFlag = -1;
			else if (GetOweSpecialServ((*iterPay).m_lServID,OWE_BTYPE_ID_CJ)) iFlag = -1;
			//if (strcmp(sState,"2HC")!=0&&strcmp(sState,"2HE")!=0&&strcmp(sState,"2HX")!=0&&strcmp(sState,"2IX")!=0&&iFlag == 0) 
			if (CheckServState(sState,OWE_BTYPE_ID_CJ)==1 && iFlag == 0)
			{
						//ͣ���û��Ĵ߽ɷ�ֵ����Ϊ0-Ƿ�ѽ��
						glMaxCharge = 0;
						glMinCharge = glBalance-glOweCharge-glAggrCharge;
						InsertOweDun((*iterPay).m_lServID,OWE_BTYPE_ID_CJ);
						iFlag = 1;
			}
		}
		//�ж��Ƿ���ͣ��,��δͣ��,��ͣ������
		if (GetOweSpecialServ((*iterPay).m_lServID,OWE_BTYPE_ID_TJ))
					continue;
		//if (strcmp(sState,"2HC")==0||strcmp(sState,"2HD")==0||strcmp(sState,"2HS")==0||strcmp(sState,"2HE")==0||strcmp(sState,"2HX")==0||strcmp(sState,"2IX")==0) continue;
		if (CheckServState(sState,OWE_BTYPE_ID_TJ)==0) continue;
		
		//�ж��û��Ƿ����ڲ�ͣ���Ĳ�Ʒ
		if (GetOweSpecialProduct(iProductID,OWE_BTYPE_ID_TJ,iBillingModeID)) continue;
		
		InsertWorkOrder((*iterPay).m_lServID,OWE_BTYPE_ID_TJ,sState);
		UpdateServState((*iterPay).m_lServID,OWE_BTYPE_ID_TJ,OWE_STATE,sState);
	}
	
	if (iFlag == 1 )
	{
		if (iInsertFlag == 1 ) InsertOweDunCnt(lAcctID,glBalance,iCnt);
		  else UpdateOweDunCnt(lAcctID,glBalance,iCnt);
	}
	
}

void Rt_Stop_Make::UpdateOweDunCnt(long lAcctID,int iBalance,int iCnt)
{
	DEFINE_QUERY(qry);
	qry.close();
	qry.setSQL("UPDATE B_ACCT_OWE_DUN SET OWE_DUN_CNT=:OWE_DUN_CNT,BALANCE=:BALANCE, \
	 OWE_CHARGE=:OWE_CHARGE,AGGR_CHARGE=:AGGR_CHARGE,ACCT_CREDIT=:ACCT_CREDIT,	\
	 	STATE_DATE=SYSDATE WHERE ACCT_ID=:ACCT_ID");
	qry.setParameter("OWE_DUN_CNT",iCnt);
	qry.setParameter("BALANCE",iBalance);
	qry.setParameter("OWE_CHARGE",glOweCharge);
	qry.setParameter("AGGR_CHARGE",glAggrCharge);
	qry.setParameter("ACCT_CREDIT",giAcctCredit);
	qry.setParameter("ACCT_ID",lAcctID);
	qry.execute();
	return;
}

void Rt_Stop_Make::InsertOweDunCnt(long lAcctID,int iBalance,int iCnt)
{
	DEFINE_QUERY(qry);
	qry.close();
	qry.setSQL("INSERT INTO B_ACCT_OWE_DUN(ACCT_ID,OWE_DUN_CNT,BALANCE,OWE_CHARGE,AGGR_CHARGE,STATE_DATE,ACCT_CREDIT) \
	VALUES(:ACCT_ID,:OWE_DUN_CNT,:BALANCE,:OWE_CHARGE,:AGGR_CHARGE,SYSDATE,:ACCT_CREDIT)");
	qry.setParameter("ACCT_ID",lAcctID);
	qry.setParameter("OWE_CHARGE",glOweCharge);
	qry.setParameter("AGGR_CHARGE",glAggrCharge);
	qry.setParameter("OWE_DUN_CNT",iCnt);
	qry.setParameter("BALANCE",iBalance);
	qry.setParameter("ACCT_CREDIT",giAcctCredit);
	qry.execute();
	return;
}


int  Rt_Stop_Make::GetServState(long lServID,char sState[4],int *iBillingModeID,int *iProductID)
{
		DEFINE_QUERY(qry);
		qry.close();
		qry.setSQL("SELECT STATE,BILLING_MODE_ID,PRODUCT_ID FROM SERV WHERE SERV_ID=:SERV_ID");
		qry.setParameter("SERV_ID",lServID);
		qry.open();
		
		if(!qry.next())
			return 0;
		else{
			strcpy(sState,qry.field("STATE").asString());
			*iBillingModeID = qry.field("BILLING_MODE_ID").asInteger();
			*iProductID = qry.field("PRODUCT_ID").asInteger();
			return 1;
		}
}
long	Rt_Stop_Make::GetCurAggrChargeComm(long lServID,long lAcctID,int iBillingCycleID,int iType,int iAcctItemGroupID)
{
		//DEFINE_QUERY(qry);
		TOCIQuery qry(&gpDBLink_STOP);
		char	sSql[2000];
		long	lCharge=0;
				
		//����Ѿ�ȡ��һ��,���ظ���ȡ,���Ч��
		//if (giAggrCnt > 0 ) return glAggrCharge;
		
		switch (iType){
		case 2:
			sprintf(sSql,"SELECT NVL(SUM(NVL(CHARGE,0)),0) CHARGE FROM %s WHERE SERV_ID=:SERV_ID AND BILLING_CYCLE_ID=:BILLING_CYCLE_ID "
			" AND NVL(FREE_FLAG,0) = 0 "
//ԭsqlִ�н��� suzhou
//			" AND ((:ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)  "
//			"       OR ACCT_ITEM_TYPE_ID IN (SELECT ACCT_ITEM_OBJECT_ID FROM B_ACCT_ITEM_GROUP_MEMBER WHERE ITEM_GROUP_ID=:ITEM_GROUP_ID)) ",gsPreTableName);
			" AND ((:ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)) ",gsPreTableName);
			qry.close();
			qry.setSQL(sSql);
			qry.setParameter("SERV_ID",lServID);
			//qry.setParameter("TABLE_NAME",gsPreTableName);
			//qry.setParameter("ORG_ID",iOrgID);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.setParameter("BILLING_CYCLE_ID",iBillingCycleID);
			qry.open();
			if(!qry.next())
				lCharge= 0;
			else 
				lCharge= qry.field("CHARGE").asLong();
			break;
		case 6:
			sprintf(sSql,"SELECT NVL(SUM(NVL(CHARGE,0)),0) CHARGE FROM %s WHERE ACCT_ID=:ACCT_ID AND BILLING_CYCLE_ID=:BILLING_CYCLE_ID "
			" AND NVL(FREE_FLAG,0) = 0 "
//ԭsqlִ�н��� suzhou
//			" AND ((:ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)  "
//			"       OR ACCT_ITEM_TYPE_ID IN (SELECT ACCT_ITEM_OBJECT_ID FROM B_ACCT_ITEM_GROUP_MEMBER WHERE ITEM_GROUP_ID=:ITEM_GROUP_ID)) ",gsPreTableName);
			" AND ((:ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)) ",gsPreTableName);
			qry.close();
			qry.setSQL(sSql);
			qry.setParameter("ACCT_ID",lAcctID);
			//qry.setParameter("ORG_ID",iOrgID);
			//qry.setParameter("TABLE_NAME",gsPreTableName);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.setParameter("BILLING_CYCLE_ID",iBillingCycleID);
			qry.open();
			if(!qry.next())
				lCharge= 0;
			else 
				lCharge= qry.field("CHARGE").asLong();
			break;
		default:
			lCharge = 0;
			break;
		}
		glAggrCharge = lCharge;
		giAggrCnt++;
		return lCharge;
}


long	Rt_Stop_Make::GetCurAggrChargeBill(long lServID,long lAcctID,int iBillingCycleID,int iType,int iAcctItemGroupID,char * sPreTableName)
{
		DEFINE_QUERY(qry);
		char	sSql[2000];
		long	lCharge=0;
				
		//����Ѿ�ȡ��һ��,���ظ���ȡ,���Ч��
		//if (giAggrCnt > 0 ) return glAggrCharge;
		
		switch (iType){
		case 2:
			sprintf(sSql,"SELECT NVL(SUM(NVL(CHARGE,0)),0) CHARGE FROM %s WHERE SERV_ID=:SERV_ID AND BILLING_CYCLE_ID=:BILLING_CYCLE_ID "
			" AND NVL(FREE_FLAG,0) = 0 "
			" AND ((:ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)  "
			"       OR ACCT_ITEM_TYPE_ID IN (SELECT ACCT_ITEM_OBJECT_ID FROM B_ACCT_ITEM_GROUP_MEMBER WHERE ITEM_GROUP_ID=:ITEM_GROUP_ID)) ",sPreTableName);
			qry.close();
			qry.setSQL(sSql);
			qry.setParameter("SERV_ID",lServID);
			//qry.setParameter("TABLE_NAME",sPreTableName);
			//qry.setParameter("ORG_ID",iOrgID);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.setParameter("BILLING_CYCLE_ID",iBillingCycleID);
			qry.open();
			if(!qry.next())
				lCharge= 0;
			else
				lCharge= qry.field("CHARGE").asLong();
			break;
		case 6:
			sprintf(sSql,"SELECT NVL(SUM(NVL(CHARGE,0)),0) CHARGE FROM %s WHERE ACCT_ID=:ACCT_ID AND BILLING_CYCLE_ID=:BILLING_CYCLE_ID "
			" AND NVL(FREE_FLAG,0) = 0 "
			" AND ((:ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)  "
			"       OR ACCT_ITEM_TYPE_ID IN (SELECT ACCT_ITEM_OBJECT_ID FROM B_ACCT_ITEM_GROUP_MEMBER WHERE ITEM_GROUP_ID=:ITEM_GROUP_ID)) ",sPreTableName);
			qry.close();
			qry.setSQL(sSql);
			qry.setParameter("ACCT_ID",lAcctID);
			//qry.setParameter("ORG_ID",iOrgID);
			//qry.setParameter("TABLE_NAME",sPreTableName);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.setParameter("BILLING_CYCLE_ID",iBillingCycleID);
			qry.open();
			if(!qry.next())
				lCharge= 0;
			else 
				lCharge= qry.field("CHARGE").asLong();
			break;
		default:
			lCharge = 0;
			break;
		}
		glAggrCharge = lCharge;
		giAggrCnt++;
		return lCharge;
}

int Rt_Stop_Make::GetAcctCreditDB(const long lAcctID)
{
	DEFINE_QUERY(qry);
	int iDefaultValue;
	
	if (giCreditCnt > 0) return giAcctCredit;
	iDefaultValue = CREDIT_DEFAULT_VALUE;
	qry.close();
	qry.setSQL("select nvl(sum(credit_amount),:DEFAULT_VALUE) from acct_credit where acct_id = :ACCT_ID "
	" AND sysdate BETWEEN nvl(eff_date,sysdate-1) and nvl(exp_date,sysdate+1) ");
	qry.setParameter("ACCT_ID",lAcctID);
	qry.setParameter("DEFAULT_VALUE",iDefaultValue);
	qry.open();
	if (!qry.next())
		giAcctCredit = iDefaultValue;
	else giAcctCredit = qry.field(0).asInteger();
	giCreditCnt++;
	return giAcctCredit;
}

int Rt_Stop_Make::GetServCreditDB(const long lServID)
{
	DEFINE_QUERY(qry);
	int iDefaultValue;
	
	iDefaultValue = CREDIT_DEFAULT_VALUE;
	qry.close();
	qry.setSQL("select nvl(sum(credit_amount),:DEFAULT_VALUE) from acct_credit where serv_id = :SERV_ID "
	" AND sysdate BETWEEN nvl(eff_date,sysdate-1) and nvl(exp_date,sysdate+1) ");
	qry.setParameter("SERV_ID",lServID);
	qry.setParameter("DEFAULT_VALUE",iDefaultValue);
	qry.open();
	if (qry.next())
		iDefaultValue = qry.field(0).asInteger();
	return iDefaultValue;
}

int Rt_Stop_Make::GetServBillingMode(const long lServID)
{
	DEFINE_QUERY(qry);
	int iBillingMode;
	if (giCreditCnt > 0) return giAcctCredit;
	qry.close();
	qry.setSQL("select billing_mode_id from serv where serv_id = :SERV_ID ");
	qry.setParameter("SERV_ID",lServID);
	qry.open();
	if (!qry.next())
		return 1;
	else iBillingMode = qry.field(0).asInteger();
	return iBillingMode;
}

long Rt_Stop_Make::GetOweCharge(long lServID,long lAcctID,int iAcctItemGroupID)
{/*���������ṩ�������*/
		DEFINE_QUERY(qry);
		long	lOweCharge=0;
		
		//����Ѿ�ȡ��һ��,���ظ���ȡ,���Ч��
		if (giOweCnt > 0 ) return glOweCharge;
		
		if (lServID!=-1){
			
			qry.close();
			qry.setSQL("select nvl(sum(nvl(amount,0)),0) amount from acct_item_owe where state in (select owe_state from A_PAY_OWE_STATE WHERE PAYMENT_METHOD=999) "
			" and billing_cycle_id not in (select billing_cycle_id from billing_cycle where state in ('10B','10A','10R') ) "
			" and SERV_ID=:SERV_ID AND (ACCT_ITEM_TYPE_ID IN (SELECT ACCT_ITEM_OBJECT_ID FROM B_ACCT_ITEM_GROUP_MEMBER WHERE ITEM_GROUP_ID=:ITEM_GROUP_ID AND STATE='M0A') or :ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)");
			qry.setParameter("SERV_ID",lServID);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.open();
			if(!qry.next())
				lOweCharge = 0;
			else 
				lOweCharge=qry.field("amount").asLong();
		}
		else if (lAcctID!=-1){
			
			qry.close();
			qry.setSQL("select nvl(sum(nvl(amount,0)),0) amount from acct_item_owe where state in (select owe_state from A_PAY_OWE_STATE WHERE PAYMENT_METHOD=999) "
			" and billing_cycle_id not in (select billing_cycle_id from billing_cycle where state in ('10B','10A','10R') ) "
			" and ACCT_ID=:ACCT_ID AND (ACCT_ITEM_TYPE_ID IN (SELECT ACCT_ITEM_OBJECT_ID FROM B_ACCT_ITEM_GROUP_MEMBER WHERE ITEM_GROUP_ID=:ITEM_GROUP_ID AND STATE='M0A') or :ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)");
			qry.setParameter("ACCT_ID",lAcctID);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.open();
			if(!qry.next())
				lOweCharge = 0;
			else 
				lOweCharge=qry.field("amount").asLong();
		}
		//Add 20050922
		glOweCharge = lOweCharge;
		giOweCnt++;
		return lOweCharge;
}


long Rt_Stop_Make::getNoPartyRoleOwe(long lServID,long lAcctID,int iType,int iPartyRoleID)
{/*���������ṩ�������*/
		DEFINE_QUERY(qry);
		long	lOweCharge=0;
		
		//����Ѿ�ȡ��һ��,���ظ���ȡ,���Ч��
		
		switch (iType){
		case 2:
			qry.close();
			qry.setSQL("select nvl(sum(nvl(amount,0)),0) amount from acct_item_owe where state in (select owe_state from A_PAY_OWE_STATE WHERE PAYMENT_METHOD=999) "
			" and billing_cycle_id not in (select billing_cycle_id from billing_cycle where state in ('10B','10A') ) "
			" and SERV_ID=:SERV_ID AND (ACCT_ITEM_TYPE_ID NOT IN (SELECT nvl(ACCT_ITEM_TYPE_ID,0) FROM ACCT_ITEM_TYPE WHERE nvl(party_role_id,0)=:PARTY_ROLE_ID))");
			qry.setParameter("SERV_ID",lServID);
			qry.setParameter("PARTY_ROLE_ID",iPartyRoleID);
			qry.open();
			if(!qry.next())
				lOweCharge = 0;
			else 
				lOweCharge=qry.field("amount").asLong();
			
			break;
		
		case 6:
			
			qry.close();
			qry.setSQL("select nvl(sum(nvl(amount,0)),0) amount from acct_item_owe where state in (select owe_state from A_PAY_OWE_STATE WHERE PAYMENT_METHOD=999) "
			" and billing_cycle_id not in (select billing_cycle_id from billing_cycle where state in ('10B','10A') ) "
			" and ACCT_ID=:ACCT_ID AND (ACCT_ITEM_TYPE_ID NOT IN (SELECT nvl(ACCT_ITEM_TYPE_ID,0) FROM ACCT_ITEM_TYPE WHERE nvl(party_role_id,0)=:PARTY_ROLE_ID))");
			qry.setParameter("ACCT_ID",lAcctID);
			qry.setParameter("PARTY_ROLE_ID",iPartyRoleID);
			qry.open();
			if(!qry.next())
				lOweCharge = 0;
			else 
				lOweCharge=qry.field("amount").asLong();
			break;
			
		}
		//Add 20050922
		glOweCharge = lOweCharge;
		return lOweCharge;
}


long Rt_Stop_Make::GetBalanceCharge(long lServID,long lAcctID, int iAcctItemGroupID)
{/*���������ṩ�������*/
		DEFINE_QUERY(qry);
		char	 sSql[2000],sDate[16];
		long	lAcctBalance=0;
		Date d;
		//����Ѿ�ȡ��һ��,���ظ���ȡ,���Ч��,����֤�����в��ܼ�ȡ�ʻ����,��ȡ�û����
		if (giBalanceCnt > 0 ) return glBalance;
		strncpy(sDate,d.toString(),8);
		sDate[8]=0;
		if (lServID!=-1){
			qry.close();
			sprintf(sSql,"select nvl(sum(NVL(BALANCE,0) ),0) BALANCE  from ACCT_BALANCE where STATE='10A' and SERV_ID=:SERV_ID "
			             " and to_char(sysdate,'yyyymmdd') BETWEEN to_char(nvl(eff_date,to_date('20000101','yyyymmdd')),'yyyymmdd') and to_char(nvl(exp_date,to_date('20600101','yyyymmdd')),'yyyymmdd') "
			             " and (ITEM_GROUP_ID=:ITEM_GROUP_ID or :ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)");
			qry.setSQL(sSql);
			qry.setParameter("SERV_ID",lServID);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.open();
			if(!qry.next())
				lAcctBalance = 0;
			else 
				 lAcctBalance=qry.field("BALANCE").asLong();
				
		}
		else if (lAcctID!=-1){/* ????�ʻ�ȡֵ�������⡡*/
			qry.close();
			sprintf(sSql,"select nvl(sum(nvl(BALANCE,0) ),0) BALANCE  from ACCT_BALANCE where STATE='10A' and ACCT_ID=:ACCT_ID "
		  " and to_char(sysdate,'yyyymmdd') BETWEEN to_char(nvl(eff_date,to_date('20000101','yyyymmdd')),'yyyymmdd') and to_char(nvl(exp_date,to_date('20600101','yyyymmdd')),'yyyymmdd') "
			" and (ITEM_GROUP_ID=:ITEM_GROUP_ID or :ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)");
			qry.setSQL(sSql);
			qry.setParameter("ACCT_ID",lAcctID);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.open();
			if(!qry.next())
				lAcctBalance= 0;
			else 
				lAcctBalance= qry.field("BALANCE").asLong();
			
		}
		//Add 20050922
		glBalance = lAcctBalance;
		
		giBalanceCnt++;
		
		return lAcctBalance;
}

long Rt_Stop_Make::GetUpperBalance(long lServID,long lAcctID, int iBillingCycleID,int iAcctItemGroupID)
{/*���������ṩ�������*/
		DEFINE_QUERY(qry);
		char	 sSql[2000],sDate[16];
		long	 lAcctBalance=0;
		BillingCycleMgr bcm;
		int     iBillingCnt;
		char sState[5];
		Date d;
		//����Ѿ�ȡ��һ��,���ظ���ȡ,���Ч��,����֤�����в��ܼ�ȡ�ʻ����,��ȡ�û����
		if (giBalanceCnt > 0 ) return glBalance;
		strncpy(sDate,d.toString(),8);
		sDate[8]=0;
		strcpy(sState,bcm.getPreBillingCycle(iBillingCycleID)->getState());
        if (sState[2]=='A' or sState[2]=='B' or sState[2]=='D' or sState[2]=='R') iBillingCnt = 2;
         else iBillingCnt = 1 ;
		if (lServID!=-1){
			qry.close();
			sprintf(sSql,"select nvl(sum(nvl(decode(cycle_upper,Null,balance,decode(sign(cycle_upper*%d-balance),-1,cycle_upper*%d,balance)),0) ),0) BALANCE  "
			"	from ACCT_BALANCE where STATE='10A' and SERV_ID=:SERV_ID "
			" and (exists (Select 1 From billing_cycle Where "
			" (billing_cycle_id In (Select last_billing_cycle_id From billing_cycle Where "
			" state in ('10R','10A','10B','10D') And billing_cycle_type_id=1) Or (state in ('10R','10A','10B','10D') And billing_cycle_type_id=1)) "
								" And eff_date Between cycle_begin_date And cycle_end_date-1/86400) "
			" or Exists (Select 1 From billing_cycle Where "
			" (billing_cycle_id In (Select last_billing_cycle_id From billing_cycle Where "
			" state in ('10R','10A','10B','10D') And billing_cycle_type_id=1) Or (state in ('10R','10A','10B','10D') And billing_cycle_type_id=1)) "
								" And (exp_date-1/86400) Between cycle_begin_date And cycle_end_date-1/86400) "
			" or sysdate BETWEEN nvl(eff_date,sysdate-1) and nvl(exp_date-1/86400,sysdate+1) ) "
			" and (ITEM_GROUP_ID=:ITEM_GROUP_ID or :ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)",iBillingCnt,iBillingCnt);

			qry.setSQL(sSql);
			qry.setParameter("SERV_ID",lServID);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.open();
			if(!qry.next())
				lAcctBalance = 0;
			else 
				lAcctBalance=qry.field("BALANCE").asLong();
				
		}
		else if (lAcctID!=-1){/* ????�ʻ�ȡֵ�������⡡*/
			qry.close();
			sprintf(sSql,"select nvl(sum(nvl(decode(cycle_upper,Null,balance,decode(sign(cycle_upper*%d-balance),-1,cycle_upper*%d,balance)),0) ),0) BALANCE "
			" from ACCT_BALANCE where STATE='10A' and ACCT_ID=:ACCT_ID "
			" and (exists (Select 1 From billing_cycle Where "
			" (billing_cycle_id In (Select last_billing_cycle_id From billing_cycle Where "
			" state in ('10R','10A','10B','10D') And billing_cycle_type_id=1) Or (state in ('10R','10A','10B','10D') And billing_cycle_type_id=1)) "
								" And eff_date Between cycle_begin_date And cycle_end_date-1/86400) "
			" or Exists (Select 1 From billing_cycle Where "
			" (billing_cycle_id In (Select last_billing_cycle_id From billing_cycle Where "
			" state in ('10R','10A','10B','10D') And billing_cycle_type_id=1) Or (state in ('10R','10A','10B','10D') And billing_cycle_type_id=1)) "
								" And exp_date-1/86400 Between cycle_begin_date And cycle_end_date-1/86400) "
			" or sysdate BETWEEN nvl(eff_date,sysdate-1) and nvl(exp_date-1/86400,sysdate+1) ) "
			" and (ITEM_GROUP_ID=:ITEM_GROUP_ID or :ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 or :ITEM_GROUP_ID=1)",iBillingCnt,iBillingCnt);

			qry.setSQL(sSql);
			qry.setParameter("ACCT_ID",lAcctID);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.open();
			if(!qry.next())
				lAcctBalance= 0;
			else 
				lAcctBalance= qry.field("BALANCE").asLong();
			
		}
		//Add 20050922
		glBalance = lAcctBalance;
		
		giBalanceCnt++;
		
		return lAcctBalance;
}

long Rt_Stop_Make::getAggrCharge(long lServID,long lAcctID,int iBillingCycle,int iType,int iAcctItemType)
{
	long lAggrCharge = 0;
	
	switch (iType){
		case 2:
				if (iAcctItemType == 0 || iAcctItemType == -1 || iAcctItemType == 1)
				{
					lAggrCharge = G_PAGGRMGR->getAcctItemAggr (lServID, \
								iBillingCycle, 0);
				}
				else
				{
						int * piAcctItems = G_PACCTITEMMGR->getAcctItemsB (iAcctItemType);
						while (*piAcctItems != -1){
							lAggrCharge += G_PAGGRMGR->getAcctItemAggr (lServID, iBillingCycle,  *piAcctItems); 
							piAcctItems++;
						}
				}
				break;
		case 6:
				vector<ServAcctInfo> vPayInfo;
				vector<ServAcctInfo>::iterator iterPay;
				HashList<int> oHashList (10000);
				int iTemp;
				
				if (!G_PUSERINFO->getPayInfo (vPayInfo, lAcctID)) break;

				for (iterPay=vPayInfo.begin(); iterPay!=vPayInfo.end(); iterPay++) {
							if (iAcctItemType == 0 || iAcctItemType == -1 || iAcctItemType == 1){
								if (oHashList.get ((*iterPay).m_lServID, &iTemp)) continue;
								oHashList.add ((*iterPay).m_lServID, 1);
								lAggrCharge += G_PAGGRMGR->getAcctItemAggr (
									(*iterPay).m_lServID, iBillingCycle, 0);
							}
							else
							{
								int * pAcctItemTypes = G_PACCTITEMMGR->getAcctItems((*iterPay).m_iAcctItemGroup);
								while (*pAcctItemTypes != -1) {
									lAggrCharge += G_PAGGRMGR->getAcctItemAggr ( 
										(*iterPay).m_lServID, iBillingCycle, *pAcctItemTypes); 
									pAcctItemTypes++;
						  	}
						  }
				}
				break;
	}
	
	return lAggrCharge;
}


long Rt_Stop_Make::getPartyCharge(long lServID,long lAcctID,int iBillingCycle,int iType,int iPartyRoleID)
{
	long lPartyRoleCharge = 0;
	BillingCycleMgr bcm;
	char sState[5];
	
  strcpy(sState,bcm.getPreBillingCycle(iBillingCycle)->getState());
	switch (iType){
		case 2:
				lPartyRoleCharge = 
						G_PAGGRMGR->getPartyRoleAggr(lServID,iBillingCycle,iPartyRoleID);
				
				if (sState[2]=='R' || sState[2]=='A' || sState[2]=='B' )
				{
			
					lPartyRoleCharge=lPartyRoleCharge + GetDBPartyRoleCharge(lServID,iBillingCycle,iPartyRoleID);
				}
				break;   
		case 6:
				vector<ServAcctInfo> vPayInfo;
				vector<ServAcctInfo>::iterator iterPay;
				HashList<int> oHashList (10000);
				int iTemp;
				
				if (!G_PUSERINFO->getPayInfo (vPayInfo, lAcctID)) break;

				for (iterPay=vPayInfo.begin(); iterPay!=vPayInfo.end(); iterPay++) {
								if (oHashList.get ((*iterPay).m_lServID, &iTemp)) continue;
								oHashList.add ((*iterPay).m_lServID, 1);
								lPartyRoleCharge += G_PAGGRMGR->getPartyRoleAggr(
									(*iterPay).m_lServID,iBillingCycle,iPartyRoleID);
								if (sState[2]=='R' || sState[2]=='A' || sState[2]=='B' )
								{
									 lPartyRoleCharge=lPartyRoleCharge + GetDBPartyRoleCharge(lServID,iBillingCycle,iPartyRoleID);
								}
				}
				break;
	}
	
	return lPartyRoleCharge;
}

long Rt_Stop_Make::GetRealCharge(long lServID,long lAcctID,int iType,int iBillingCycleID,int iAcctItemGroupID)
{
		//����Ѿ�ȡ��һ��,���ظ���ȡ,���Ч��
		//if (giAggrCnt > 0 ) return glAggrCharge;

		BillingCycleMgr bcm;
		int iBillingCycle;
		long lAggrCharge = 0;
		char sState[5];
		lAggrCharge = getAggrCharge(lServID,lAcctID,iBillingCycleID,iType,iAcctItemGroupID);
		strcpy(sState,bcm.getPreBillingCycle(iBillingCycleID)->getState());
		if (sState[2]=='R' || sState[2]=='A' || sState[2]=='B')
		{
			iBillingCycle = bcm.getPreBillingCycle (iBillingCycleID)->getBillingCycleID ();
			lAggrCharge=lAggrCharge + getAggrCharge(lServID,lAcctID,iBillingCycle,iType,iAcctItemGroupID);
		}
		
		glAggrCharge = lAggrCharge;
		
		giAggrCnt++;
		
		return lAggrCharge;
}

long Rt_Stop_Make::GetDBPartyRoleCharge(long lServID,int iBillingCycleID,int iPartyRoleID)
{
	TOCIQuery qry(&gpDBLink_STOP);
	
	char	sSql[2000];
	long lCharge = 0;
	sprintf(sSql,"SELECT NVL(SUM(NVL(CHARGE,0)),0) FROM  %s "
	" where SERV_ID=%ld "
	" AND ACCT_ITEM_TYPE_ID IN (SELECT nvl(ACCT_ITEM_TYPE_ID,0) FROM ACCT_ITEM_TYPE WHERE nvl(party_role_id,0)=%d) "
	,gsPreTableName,lServID,iPartyRoleID);
	qry.close();
			qry.setSQL(sSql);
	qry.open();
	if(!qry.next())
		lCharge = 0;
	else lCharge = qry.field(0).asLong();
	
	return lCharge;
}

long Rt_Stop_Make::GetCurAggrCharge(long lServID, long lAcctID,int iOrgID,int iAcctItemGroupID,int iBillingCycleID)
{
		DEFINE_QUERY(qry);
		char	sSql[2000];
		long	lCharge=0;
				
		//����Ѿ�ȡ��һ��,���ظ���ȡ,���Ч��
		if (giAggrCnt > 0 ) return glAggrCharge;
		
		//B_GetRealChargeӦ����acct�û���
		if (lServID!=-1){
			//sprintf(sSql,"SELECT SUM(CHARGE) CHARGE FROM %s WHERE SERV_ID=:SERV_ID AND BILLING_CYCLE_ID=:BILLING_CYCLE_ID AND ( ACCT_ITEM_TYPE_ID IN (SELECT ACCT_ITEM_OBJECT_ID FROM B_ACCT_ITEM_GROUP_MEMBER WHERE ITEM_GROUP_ID=:ITEM_GROUP_ID AND STATE='M0A' ) or :ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 )",sTableName);
			sprintf(sSql,"SELECT B_GetRealCharge(:BILLING_CYCLE_ID,:ORG_ID,:SERV_ID,-1,:ITEM_GROUP_ID) CHARGE FROM DUAL");
			qry.close();
			qry.setSQL(sSql);
			qry.setParameter("SERV_ID",lServID);
			qry.setParameter("ORG_ID",iOrgID);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.setParameter("BILLING_CYCLE_ID",iBillingCycleID);
			qry.open();
			if(!qry.next())
				lCharge= 0;
			else 
				lCharge= qry.field("CHARGE").asLong();
		}
		else if (lAcctID!=-1){
			//sprintf(sSql,"SELECT SUM(CHARGE) CHARGE FROM %s WHERE ACCT_ID=:ACCT_ID AND BILLING_CYCLE_ID=:BILLING_CYCLE_ID AND ( ACCT_ITEM_TYPE_ID IN (SELECT ACCT_ITEM_OBJECT_ID FROM B_ACCT_ITEM_GROUP_MEMBER WHERE ITEM_GROUP_ID=:ITEM_GROUP_ID AND STATE='M0A' ) or :ITEM_GROUP_ID=-1 or :ITEM_GROUP_ID=0 )",sTableName);
			sprintf(sSql,"SELECT B_GetRealCharge(:BILLING_CYCLE_ID,:ORG_ID,-1,:ACCT_ID,:ITEM_GROUP_ID) CHARGE FROM DUAL");
			qry.close();
			qry.setSQL(sSql);
			qry.setParameter("ACCT_ID",lAcctID);
			qry.setParameter("ORG_ID",iOrgID);
			qry.setParameter("ITEM_GROUP_ID",iAcctItemGroupID);
			qry.setParameter("BILLING_CYCLE_ID",iBillingCycleID);
			qry.open();
			if(!qry.next())
				lCharge= 0;
			else 
				lCharge= qry.field("CHARGE").asLong();
		}
		//Add 20050922
		glAggrCharge = lCharge;
		giAggrCnt++;
		return lCharge;
}


long Rt_Stop_Make::Get10ACHARGE_DBComm(long lServID,long lAcctID,int iType,int iBillingCycleID,int iAcctItemGroupID)
{
		//����Ѿ�ȡ��һ��,���ظ���ȡ,���Ч��
		//if (giAggrCnt > 0 ) return glAggrCharge;

		BillingCycleMgr bcm;
		int iBillingCycle;
		long lAggrCharge = 0;
		char sState[5];
		lAggrCharge = getAggrCharge(lServID,lAcctID,iBillingCycleID,iType,iAcctItemGroupID);
		strcpy(sState,bcm.getPreBillingCycle(iBillingCycleID)->getState());

		if (sState[2]=='R' || sState[2]=='A' || sState[2]=='B' )
		{
			iBillingCycle = bcm.getPreBillingCycle (iBillingCycleID)->getBillingCycleID ();
			
			lAggrCharge=lAggrCharge + GetCurAggrChargeComm(lServID,lAcctID,iBillingCycle,iType,iAcctItemGroupID);
		}
		
		glAggrCharge = lAggrCharge;
		
		giAggrCnt++;
		
		return lAggrCharge;
}

long Rt_Stop_Make::Get10ACHARGE_DBBill(long lServID,long lAcctID,int iType,int iBillingCycleID,int iAcctItemGroupID,char * sTableName)
{
		//����Ѿ�ȡ��һ��,���ظ���ȡ,���Ч��
		//if (giAggrCnt > 0 ) return glAggrCharge;

		BillingCycleMgr bcm;
		int iBillingCycle;
		long lAggrCharge = 0;
		char sState[5];
		char sPreTableName[200];
		lAggrCharge = getAggrCharge(lServID,lAcctID,iBillingCycleID,iType,iAcctItemGroupID);
		strcpy(sState,bcm.getPreBillingCycle(iBillingCycleID)->getState());
		if (sState[2]=='R' || sState[2]=='A' || sState[2]=='B' )
		{
			iBillingCycle = bcm.getPreBillingCycle(iBillingCycleID)->getBillingCycleID ();
			lAggrCharge=lAggrCharge + GetCurAggrChargeBill(lServID,lAcctID,iBillingCycle,iType,iAcctItemGroupID,sTableName);
		}
		
		glAggrCharge = lAggrCharge;
		
		giAggrCnt++;
		
		return lAggrCharge;
}

//����0��ʾ������
int Rt_Stop_Make::CheckServState(char *sState,int iType)
{
	#ifdef DEF_HAINAN
	  if (strcmp(sState,"2HC")==0)
	  	return 1;
	#endif
	//���û�״̬�ж�,ֻ����2HS,��Ҫ�ǿ����û�˫ͣ��ʱ��,����2HS,���Լ���˫ͣ
	//����2SX��Ԥ�����״̬��2008.06.30 ����.����
	//����2PX ����
	if (strcmp(sState,"2HC")==0||strcmp(sState,"2HD")==0||strcmp(sState,"2HE")==0||
			strcmp(sState,"2HX")==0||strcmp(sState,"2IX")==0||strcmp(sState,"2HB")==0||
			strcmp(sState,"2HF")==0||strcmp(sState,"2HH")==0||strcmp(sState,"2SX")==0||
			strcmp(sState,"2HN")==0||strcmp(sState,"2IS")==0||strcmp(sState,"2ID")==0||
			strcmp(sState,"2PX")==0 )
		return 0;

	if (iType !=OWE_BTYPE_ID_ST){
		if (strcmp(sState,"2HS")==0)
		   return 0;
	}

	return 1;
}

int Rt_Stop_Make::GetDiffMin(char sStartTime[15], char sCurTime[15])
{
    Date dStart(sStartTime);
    Date dCur(sCurTime);
    long lMin;
    long iDiv = dCur.diffSec(dStart);
    
	lMin = iDiv/60;

  	return lMin;
}

void Rt_Stop_Make::InitStopLimit(int iOrgID)
{
    TStopLimit tTemp;
    int iCnt;
    
    char sSql[2000];
    
    gpLimitCmpHead = NULL;
    giForwardMin = giCurMin = 0;
    
    char sSysdate[15],sLimitSlot[10];
	int i,j;
	char *p;
	int iSlotHourB, iSlotHourE, iSlotMinB, iSlotMinE;
	char sTwo[3];
    char sStopType[10];
    char sTemp[2];
    int iTJ, iST, iCJ;
    char sUnitLimit[2];

	TLimitCmp *pNewLimitCmp;
    
    for (int j = 0; j < 2; j++)
    {
        for(int k = 0; k < 3; k++)
        {
            for (int i = 0; i < MIN_ARRAY; i++)
            {
                gaiTimeCount[i][k][j] = 0;
            }
        }
    }
    DEFINE_QUERY(qry);
    sprintf(sSql, "select count(*) from b_make_stop_limit"
                " where org_id = %d", iOrgID);
    
    qry.setSQL(sSql);
    qry.open();
    qry.next();
    iCnt = qry.field(0).asInteger();
    qry.close();
    
    if (iCnt == 0)
        return;
    
    //Stop_typeĬ�϶������жϣ�1_1_1��
    sprintf(sSql, "SELECT LIMIT_ID,"
				 "ORG_ID,"
				 "nvl(LIMIT_TYPE,0),"
				 "nvl(LIMIT_MAX,100000),"
				 "nvl(LIMIT_UNIT,'H'),"
				 "nvl(LIMIT_AMOUNT, 100),"
				 "nvl(LIMIT_SLOT,'0000-0200'),"
				 "nvl(STOP_TYPE, '1_1_1')"
		         " FROM B_MAKE_STOP_LIMIT"
		         " WHERE ORG_ID = %d"
		         " ORDER BY LIMIT_ID ", iOrgID);

    qry.setSQL(sSql);
    qry.open();
    while(qry.next())
    {
        tTemp.iLimitID = qry.field(0).asInteger();
        tTemp.iOrgID   = qry.field(1).asInteger();
        tTemp.iLimitType = qry.field(2).asInteger();
        tTemp.iLimitMax = qry.field(3).asInteger();
        strcpy(sUnitLimit, qry.field(4).asString());
        tTemp.cLimitUnit = sUnitLimit[0];
        
        tTemp.iLimitAmount = qry.field(5).asInteger();
        strcpy(tTemp.sLimitSlot, qry.field(6).asString());
        strcpy(sStopType, qry.field(7).asString());

        strncpy(sTemp, sStopType, 1);
        iTJ = atoi(sTemp);
        tTemp.iStopLimitType[0] = iTJ;
        strncpy(sTemp, &sStopType[2], 1);
        iST = atoi(sTemp);
        tTemp.iStopLimitType[1] = iST;
        strncpy(sTemp, &sStopType[4], 1);
        iCJ = atoi(sTemp);
        tTemp.iStopLimitType[2] = iCJ;
        	
        pNewLimitCmp = (TLimitCmp *)malloc(sizeof(TLimitCmp));
    	pNewLimitCmp->aStopLimit.iLimitID = tTemp.iLimitID;
    	pNewLimitCmp->aStopLimit.iOrgID = tTemp.iOrgID;
    	pNewLimitCmp->aStopLimit.iLimitType = tTemp.iLimitType;
    	pNewLimitCmp->aStopLimit.iLimitMax = tTemp.iLimitMax ;
    	pNewLimitCmp->aStopLimit.cLimitUnit = tTemp.cLimitUnit;
    	
    	pNewLimitCmp->aStopLimit.iLimitAmount = tTemp.iLimitAmount;

    	strcpy(pNewLimitCmp->aStopLimit.sLimitSlot, tTemp.sLimitSlot);
    	
    	pNewLimitCmp->aStopLimit.iStopLimitType[0] = tTemp.iStopLimitType[0];
    	pNewLimitCmp->aStopLimit.iStopLimitType[1] = tTemp.iStopLimitType[1];
    	pNewLimitCmp->aStopLimit.iStopLimitType[2] = tTemp.iStopLimitType[2];
    	
    	pNewLimitCmp->pNext = NULL;

	    pNewLimitCmp->aStopLimit.iLimitMin = tTemp.iLimitAmount;

    	if (tTemp.iLimitType == TIME_FIRST) {
    		if(tTemp.cLimitUnit=='M') {
    		    for (int j = 0; j < 3; j++)
    			for (i=0;i<tTemp.iLimitAmount;i++) {
	    			gaiTimeCount[i][j][A_CMP]=TIME_FIRST;
			    }
		    } else if (tTemp.cLimitUnit=='H') {
		        for (int j = 0; j < 3; j++)
			    for (i=0;i<tTemp.iLimitAmount*60;i++) {
				    gaiTimeCount[i][j][A_CMP]=TIME_FIRST;
				    pNewLimitCmp->aStopLimit.iLimitMin=tTemp.iLimitAmount*60;
			    }
		    }
	    }
	    /*���㵱ǰ���ù����ܵ�ͳ�Ʒ�����*/
	    if(tTemp.iLimitType==TIME_SUM && tTemp.cLimitUnit=='H') {
		    pNewLimitCmp->aStopLimit.iLimitMin=tTemp.iLimitAmount*60;
	    }

	    if(tTemp.iLimitType==TIME_SLOT) {/*ʱ��HHMM-HHMM��ʽ*/
		    strcpy(sLimitSlot, tTemp.sLimitSlot);
		    strncpy(sTwo, sLimitSlot, 2);
		    iSlotHourB=atoi(sTwo);
		    strncpy(sTwo, &sLimitSlot[2], 2);
		    iSlotMinB=atoi(sTwo);
		    strncpy(sTwo, &sLimitSlot[5], 2);
		    iSlotHourE=atoi(sTwo);
		    strncpy(sTwo, &sLimitSlot[7], 2);
		    iSlotMinE=atoi(sTwo);

		    if(iSlotHourE<iSlotHourB) {
			    iSlotHourE=iSlotHourE+24;
		    }
		    pNewLimitCmp->aStopLimit.iLimitMin=(iSlotHourE-iSlotHourB)*60+(iSlotMinE-iSlotMinB);

			//����SlotType�ĳ�ʼ�� 2007.08.15 suzhou
			int iBeginMin;
			iBeginMin = GetBeginTimeArray(gsStartTime, pNewLimitCmp->aStopLimit.sLimitSlot);
			
			for(i=0; i<pNewLimitCmp->aStopLimit.iLimitMin; i++)
			{
				if(iBeginMin+i < MIN_ARRAY)
				{
					for (int j = 0; j < 3; j++)
					{
						gaiTimeCount[iBeginMin+i][j][A_CMP]=TIME_SLOT;
					}
				}
				else
				{
					for (int j = 0; j < 3; j++)
					{
					gaiTimeCount[iBeginMin+i-MIN_ARRAY][j][A_CMP]=TIME_SLOT;
					}
				}
			}
	    }
	    
	    pNewLimitCmp->pNext=gpLimitCmpHead;
	    gpLimitCmpHead = pNewLimitCmp;        
    } 
    
    qry.close();
}


void Rt_Stop_Make::DoWaitWork(int iLimitID)
{
	char sMsg[200];
	char sTemp[200];
	int iNum=0;
	char sql[2000];

	sprintf(sMsg, "��ǰͣ��������LIMIT_ID=[%d]�ķ�ֵ�趨,����������\n", iLimitID);
	printf("%s\n", sMsg);

	DEFINE_QUERY(qry);
	
	sprintf(sql, "INSERT INTO B_MAKE_STOP_LIMIT_ALARM(LIMIT_ID,ALARM_MSG,STATE_DATE,CREATED_DATE,DEAL_FLAG)"
	             "values(:LIMIT_ID, :ALARM_MSG, SYSDATE, SYSDATE, 0)");
    qry.setSQL(sql);
	qry.setParameter("LIMIT_ID", iLimitID);
	qry.setParameter("ALARM_MSG", sMsg);
    qry.execute();
    qry.commit();
    qry.close();
        
	while(1)
	{
        #ifdef SHM_FILE_USERINFO
        G_SHMFILE;
        #endif
        
	    strcpy(sql,"SELECT COUNT(*) FROM B_MAKE_STOP_LIMIT_ALARM WHERE DEAL_FLAG=0");
        qry.setSQL(sql);
        qry.open();
        qry.next();
        iNum = qry.field(0).asInteger();
        qry.close();
		if(iNum>0)
			sleep(60);
		else
		{
			//����澯������ڴ�ֵ

			for(int k = 0; k < 3; k++)
			{
				for (int i = 0; i < MIN_ARRAY; i++)
				{
					gaiTimeCount[i][k][A_CNT] = 0;
				}
			}

			break;
		}
	}    
}


int Rt_Stop_Make::GetBeginTimeArray(char sStartTime[], char sSlotTime[])
{
	char sStart[15],sSlot[10];
	char sHour[3],sMin[3];
	char sStartHour[3], sStartMin[3];

	int iDiff;
	int iHour,iMin,iStartHour,iStartMin;

	strcpy (sStart, sStartTime);
	strcpy (sSlot, sSlotTime);

	strncpy(sHour, sSlotTime, 2);
	strncpy(sMin, &sSlotTime[2], 2);

	iHour = atoi(sHour);
	iMin = atoi(sMin);

	strncpy(sStartHour, &sStartTime[8], 2);
	strncpy(sStartMin, &sStartTime[10], 2);
	iStartHour = atoi(sStartHour);
	iStartMin = atoi(sStartMin);

	iDiff = iHour*60+iMin-(iStartHour*60+iStartMin);
	if(iDiff<0) {
		iDiff+=MIN_ARRAY;
	}
	return iDiff;    
}

int Rt_Stop_Make::AddCountLimit(int iLimitFlag)
{
	char sHour[3];
	int iMin,i,j;
	long lCount=0;
	int iFirstFlag;
	int lCmpCnt=0;
	long lRunMin;
	int iBeginMin,iEndMin;
	char sDate[15];

	TLimitCmp *pLimitCmpCur;

	iFirstFlag=0;
	GetDateStr(sDate);
	lRunMin = GetDiffMin(gsStartTime, sDate);
	giCurMin = lRunMin%MIN_ARRAY;

	if(giCurMin != giForwardMin){

		/*�µ�һ���ӿ���������*/
		int i;

		if (giCurMin>giForwardMin)
		{
			for (i=giForwardMin+1;i<=giCurMin;i++)
			{
				for (int t = 0; t < 3; t++)
					gaiTimeCount[i][t][A_CNT]=0;
			}
		}
		else
		{
			for (i=0;i<=giCurMin;i++)
			{
				for (int t = 0; t < 3; t++)
					gaiTimeCount[i][t][A_CNT]=0;
			}
			for (i=giForwardMin+1;i<=1439;i++)
			{
				for (int t = 0; t < 3; t++)
					gaiTimeCount[i][t][A_CNT]=0;
			}
		}

		giForwardMin=giCurMin;
	}

	if (iLimitFlag == STOP_LIMIT_TJ)
	{
        gaiTimeCount[giCurMin][0][A_CNT]++;
	}
	if (iLimitFlag == STOP_LIMIT_ST)
	{
		gaiTimeCount[giCurMin][1][A_CNT]++;
	}
	if (iLimitFlag == STOP_LIMIT_CJ)
	{
		gaiTimeCount[giCurMin][2][A_CNT]++;
	}

	for(pLimitCmpCur=gpLimitCmpHead;pLimitCmpCur;pLimitCmpCur=pLimitCmpCur->pNext)
	{
		lCount = 0;

		/*0:����ǰʱ���ۼƣ�1:ָ��ʱ�����; 2:��ʼ������ */
		if(pLimitCmpCur->aStopLimit.iLimitType == TIME_SUM )
		{
			for(i=0; i<pLimitCmpCur->aStopLimit.iLimitMin; i++)
			{
				if(giCurMin-i>=0) 
				{
/*
					if(gaiTimeCount[giCurMin-i][0][A_CNT] == 0 
					&& gaiTimeCount[giCurMin-i][1][A_CNT] == 0 
					&& gaiTimeCount[giCurMin-i][2][A_CNT] == 0 )
						break; */ /*��ʾ��ǰ���ӵ���,0��ʾ�µ�һ���ӿ�ʼ��*/
					//if(gaiTimeCount[giCurMin-i][0][A_CMP] == 0)/*�ڿ�ʼָ��ʱ�����Ƿ�����ۼӣ�0��ʾ�����ۼ�*/
					{
						if(pLimitCmpCur->aStopLimit.iStopLimitType[0] == 1 && gaiTimeCount[giCurMin-i][0][A_CMP] == TIME_SUM )
							lCount+=gaiTimeCount[giCurMin-i][0][A_CNT];
						if(pLimitCmpCur->aStopLimit.iStopLimitType[1] == 1 && gaiTimeCount[giCurMin-i][1][A_CMP] == TIME_SUM)
							lCount+=gaiTimeCount[giCurMin-i][1][A_CNT];
						if(pLimitCmpCur->aStopLimit.iStopLimitType[2] == 1 && gaiTimeCount[giCurMin-i][2][A_CMP] == TIME_SUM)
							lCount+=gaiTimeCount[giCurMin-i][2][A_CNT];
					}
				}
				else
				{
/*
					if(gaiTimeCount[MIN_ARRAY+giCurMin-i][0][A_CNT] == 0
					&& gaiTimeCount[MIN_ARRAY+giCurMin-i][1][A_CNT] == 0
					&& gaiTimeCount[MIN_ARRAY+giCurMin-i][2][A_CNT] == 0)
						break;*/
					//if(gaiTimeCount[MIN_ARRAY+giCurMin-i][0][A_CMP] == 0) /*�ڿ�ʼָ��ʱ���ڵĲ��μ��ۼ�*/
					{
						if(pLimitCmpCur->aStopLimit.iStopLimitType[0] == 1 && gaiTimeCount[MIN_ARRAY+giCurMin-i][0][A_CMP] == TIME_SUM )
							lCount+=gaiTimeCount[MIN_ARRAY+giCurMin-i][0][A_CNT];
						if(pLimitCmpCur->aStopLimit.iStopLimitType[1] == 1 && gaiTimeCount[MIN_ARRAY+giCurMin-i][1][A_CMP] == TIME_SUM )
							lCount+=gaiTimeCount[MIN_ARRAY+giCurMin-i][1][A_CNT];
						if(pLimitCmpCur->aStopLimit.iStopLimitType[2] == 1 && gaiTimeCount[MIN_ARRAY+giCurMin-i][2][A_CMP] == TIME_SUM )
							lCount+=gaiTimeCount[MIN_ARRAY+giCurMin-i][2][A_CNT];
					}
				}
			}
		}
		else if(pLimitCmpCur->aStopLimit.iLimitType == TIME_SLOT)
		{
			iBeginMin = GetBeginTimeArray(gsStartTime, pLimitCmpCur->aStopLimit.sLimitSlot);
			for(i=0; i<pLimitCmpCur->aStopLimit.iLimitMin; i++)
			{
				if(iBeginMin+i < MIN_ARRAY)
				{
/*
					if(gaiTimeCount[iBeginMin+i][0][A_CNT] == 0
					&& gaiTimeCount[iBeginMin+i][1][A_CNT] == 0
					&& gaiTimeCount[iBeginMin+i][2][A_CNT] == 0)
						break;*/
					//if(gaiTimeCount[iBeginMin+i][0][A_CMP] == 0)/*�ڿ�ʼָ��ʱ���ڵĲ��μ��ۼ�*/
					if(pLimitCmpCur->aStopLimit.iStopLimitType[0] == 1 && gaiTimeCount[iBeginMin+i][0][A_CMP] == TIME_SLOT )
						lCount+=gaiTimeCount[iBeginMin+i][0][A_CNT];
					if(pLimitCmpCur->aStopLimit.iStopLimitType[1] == 1 && gaiTimeCount[iBeginMin+i][1][A_CMP] == TIME_SLOT )
						lCount+=gaiTimeCount[iBeginMin+i][1][A_CNT];
					if(pLimitCmpCur->aStopLimit.iStopLimitType[2] == 1 && gaiTimeCount[iBeginMin+i][2][A_CMP] == TIME_SLOT )
						lCount+=gaiTimeCount[iBeginMin+i][2][A_CNT];
				}
				else
				{
/*
					if(gaiTimeCount[iBeginMin+i-MIN_ARRAY][0][A_CNT] == 0
					&& gaiTimeCount[iBeginMin+i-MIN_ARRAY][1][A_CNT] == 0
					&& gaiTimeCount[iBeginMin+i-MIN_ARRAY][2][A_CNT] == 0)
						break;*/
					//if(gaiTimeCount[iBeginMin+i-MIN_ARRAY][0][A_CMP] == 0)/*�ڿ�ʼָ��ʱ���ڵĲ��μ��ۼ�*/
					if(pLimitCmpCur->aStopLimit.iStopLimitType[0] == 1 && gaiTimeCount[iBeginMin+i-MIN_ARRAY][0][A_CMP] == TIME_SLOT )
						lCount+=gaiTimeCount[iBeginMin+i-MIN_ARRAY][0][A_CNT];
					if(pLimitCmpCur->aStopLimit.iStopLimitType[1] == 1 && gaiTimeCount[iBeginMin+i-MIN_ARRAY][1][A_CMP] == TIME_SLOT )
						lCount+=gaiTimeCount[iBeginMin+i-MIN_ARRAY][1][A_CNT];
					if(pLimitCmpCur->aStopLimit.iStopLimitType[2] == 1 && gaiTimeCount[iBeginMin+i-MIN_ARRAY][2][A_CMP] == TIME_SLOT )
						lCount+=gaiTimeCount[iBeginMin+i-MIN_ARRAY][2][A_CNT];
				}
			}
		}
		else if(pLimitCmpCur->aStopLimit.iLimitType == TIME_FIRST)
		{
			for(i=0;i<pLimitCmpCur->aStopLimit.iLimitMin;i++)
			{
				//if((i>giCurMin) || (gaiTimeCount[i][0][A_CNT] == 0)) break;
				if(i>giCurMin) break;
				if(pLimitCmpCur->aStopLimit.iStopLimitType[0] == 1)
					lCount+=gaiTimeCount[i][0][A_CNT];
				if(pLimitCmpCur->aStopLimit.iStopLimitType[1] == 1)
					lCount+=gaiTimeCount[i][1][A_CNT];
				if(pLimitCmpCur->aStopLimit.iStopLimitType[2] == 1)
					lCount+=gaiTimeCount[i][2][A_CNT];
			}
		}

		if(lCount>=pLimitCmpCur->aStopLimit.iLimitMax) {
			DoWaitWork(pLimitCmpCur->aStopLimit.iLimitID);
			return  1;
		}
	}
	return 1;
}

int Rt_Stop_Make::GetDateStr(char sSysDate[])
{
        struct tm *tim;
        long ltim;
#ifdef  DEF_HP

#else
        daylight =0;
        putenv ("TZ=");
        tzset();
#endif
        time(&ltim);
        tim = localtime(&ltim);
        sprintf(sSysDate,"%04d%02d%02d%02d%02d%02d",tim->tm_year+1900,tim->tm_mon+1,tim->tm_mday,
                tim->tm_hour,tim->tm_min,tim->tm_sec);
        return 1;
}



//return 0 �ɹ���-1 ʧ��
int Rt_Stop_Make::GetBalanceByServe(long lAcctID,long lServID,int iParam1, int iParam2,long * plBalance)
{

    int  ret;
    long lStaffID = 1;//���÷���ʹ�ù���1
    char sServID[30],sAcctID[30];
    long len,lBalance,lOweBalance,lBillBalance;

    *plBalance = 0;

/*
    if (giServeInitFlag==0 )
    {
        ret = tpinit((TPINIT *)NULL);
        if ( ret == -1)
        {
            Log::log (0, "tpinit error %d %s ",tperrno,tpstrerror(tperrno));
            printf("tpinit error %d %s\n",tperrno,tpstrerror(tperrno));
        }

        sendbuf = (FBFR32 *) tpalloc("FML32",NULL,500+1000);
        if ( sendbuf == NULL)
        {
            Log::log (0, "tpalloc error %d %s ",tperrno,tpstrerror(tperrno));
            printf("tpalloc error %d %s\n",tperrno,tpstrerror(tperrno));
        }

        rcvbuf = (FBFR32 *) tpalloc("FML32",NULL,500+1000);
        if ( rcvbuf == NULL)
        {
            Log::log(0,"tpalloc error %d %s ",tperrno,tpstrerror(tperrno));
            printf("tpalloc error %d %s\n",tperrno,tpstrerror(tperrno));
        }
        len = Fsizeof32(rcvbuf);

        giServeInitFlag = 1;
    }

    sprintf(sServID,"%ld",lServID);
    sprintf(sAcctID,"%ld",lAcctID);

    Fchg32(sendbuf, SERV_ID, 0, sServID,0);
    Fchg32(sendbuf, ACCT_NBR_97, 0, sAcctID,0);
    Fchg32(sendbuf, STAFF_ID, 0, (char *)&lStaffID,0);
    
    ret = tpcall( "EXACT_QRY_BAL",(char *)sendbuf, 0, (char **)&rcvbuf, &len, 0);

    if (ret == -1)
    {
        char cMsg[500];
        FLDLEN32 errlen = sizeof(cMsg);
        bzero(cMsg,500);
        ret= Fget32(rcvbuf, SMSG, 0, cMsg, &errlen);
        string sMsg = cMsg;
        Log::log(0,"app error %s ", sMsg);
        printf("app error %s\n", sMsg);
        if(ret == -1)
        {
            Log::log(0,"tpcall error %d %s,serv_id:%s ",tperrno,tpstrerror(tperrno),sServID);
            printf("tpcall error %d %s,serv_id:%s\n",tperrno,tpstrerror(tperrno),sServID);
        }
        *plBalance = 0;
        return -1;
    }
    else
    {
        Fget32(rcvbuf, BALANCE, 0, (char *)&lBalance, 0);
        Fget32(rcvbuf, OWE_CHARGE, 0, (char *)&lOweBalance, 0);
        Fget32(rcvbuf, BILL_CHARGE, 0, (char *)&lBillBalance, 0);

        *plBalance = lBalance;
        return 0;
    }
*/

    return -1;
}

//GetBalanceByFunc����
//�����ڴ�ӿ�
/*acct_id Ϊ�ձ�ʾȡ�û�Ƿ�ѣ���ֵ��ʾȡ��ͬ��Ƿ��
  serv_id ����Ϊ��
  ��Ŀ�����ݲ�����
*/
long Rt_Stop_Make::GetOweChargeByMemory(long lAcctID,long lServID,int iBillingCycleID,int iAcctItemTypeID)
{
    long lCharge=0;
    int  iServCharge=0;
    AcctItemAccuMgr m;
    TOCIQuery qry1(&gpDBLink_STOP);
    DEFINE_QUERY(qry2);
    long lSubAcctID,lSubServID;
    BillingCycleMgr bcm;
    int iBillingModeID;
    char sState[5];
    char sql[2000];

    Serv ServInfo;
    Date d;

    //iBillingModeID = atoi(G_PSERV->getAttr (ID_BILLING_MODE));

    if (!(G_PUSERINFO->getServ (ServInfo, lServID, d.toString()))) {
        //�û�����û���ҵ�
        Log::log (91, "û���ҵ�SERVID:%ld %s", lServID, d.toString());
        return 0;
	}
#ifndef DEF_SHANDONG
    char sTempBillingMode[20],*pTempBillingMode;
    pTempBillingMode=ServInfo.getAttr (ID_BILLING_MODE);
    if (pTempBillingMode == NULL)
    {
       Log::log(0,"SERV_ID:%ld,�Ҳ���Ԥ�󸶷ѱ�־!",lServID);   
       strcpy(sTempBillingMode,"99");
    }
    else strcpy(sTempBillingMode,pTempBillingMode);

    iBillingModeID = atoi(sTempBillingMode);

    if (iBillingModeID==0)
        iBillingModeID = ServInfo.getServInfo ()->m_iBillingMode;

    if (iBillingModeID!=2) return 0;
#endif

    strcpy(sState,bcm.getPreBillingCycle(iBillingCycleID)->getState());
    if (sState[2]=='R' || sState[2]=='A' || sState[2]=='B' )
    {
        //�����ݿ�ȡ
        if (lAcctID == -1)       
            lCharge = Get10ACHARGE_DBComm(lServID,lAcctID,2,iBillingCycleID,1);
        else
            lCharge = Get10ACHARGE_DBComm(lServID,lAcctID,6,iBillingCycleID,1);
    }
    else
    {
        //���ڴ�ȡ
        if (lAcctID == -1)
        {
            iServCharge = m.get(lServID,iBillingCycleID,0);
            lCharge = iServCharge;
        }
        else
        {
            qry1.close();
            qry1.setSQL(" select acct_id,serv_id  from serv_acct "
                        " where acct_id=:v_acct_id and state='10A' ");
            qry1.setParameter("v_acct_id",lAcctID);
            qry1.open();
            while (qry1.next())
            {
                lSubAcctID = qry1.field(0).asLong();
                lSubServID = qry1.field(1).asLong();

                iServCharge = m.get(lSubServID,iBillingCycleID,0);
                lCharge = lCharge + iServCharge;
            }
        }
    }


    return lCharge;

}

//GetBalanceByFunc����
//ȡ������֯���Ƿ��
/*acct_id Ϊ�ձ�ʾȡ�û�Ƿ�ѣ���ֵ��ʾȡ��ͬ��Ƿ��
  serv_id ����Ϊ��
  ��Ŀ�����ݲ�����
*/
long Rt_Stop_Make::GetOweChargeByStruct(long lAcctID,long lServID,int iAcctItemTypeID)
{
    long lCharge = 0;
    int i;

    if (lAcctID == -1)
    {
        for (i=0;i<giOweChargeCnt;i++)
        {
            if (strOweCharge[i].lServID == lServID)
            {
                lCharge = strOweCharge[i].lRtCharge + strOweCharge[i].lOweCharge;
                break;
            }
        }
    }
    else
    {
        for (i=0;i<giOweChargeCnt;i++)
        {
            lCharge = lCharge + strOweCharge[i].lRtCharge + strOweCharge[i].lOweCharge;
        }
    }

    return lCharge;
}

//GetBalanceByFunc����
/*FundsLevel:1 �û�+���˻� 2 �û� 3 ���˻�*/
long Rt_Stop_Make::GetBalance(long lAcctID,long lServID,int iFundsLevelID,int iBillingCnt,int iCredit)
{
    long lCharge=0,lAcctCredit;
    long lBalance=0,lOweCharge=0;
    char sSql[2000],sBalanceType[1000];
    int iCreditFlag = 0;
   
    #ifdef DEF_SICHUAN
      iCreditFlag = iCredit;//��ʱֻ����Ĵ�
    #endif
   
    #ifdef DEF_SHANDONG
     iCreditFlag = 1;
    #endif
   
   
    if (!ParamDefineMgr::getParam ("RT_MAKE_STOP","NO_BALANCE_TYPE", sBalanceType) )
        strcpy(sBalanceType,"0");
    
    AllTrim(sBalanceType);
/*
    TOCIQuery qry1(&gpDBLink_STOP);
    TOCIQuery qry2(&gpDBLink_STOP);
*/

    DEFINE_QUERY(qry1);
//    DEFINE_QUERY(qry2);

    if (iFundsLevelID == 1)
    {
        qry1.close();
        #ifdef DEF_HAINAN
        qry1.setSQL(" select nvl(sum(nvl(decode(cycle_upper,Null,balance,decode(sign(cycle_upper*:v_billingcnt-balance),-1,cycle_upper*:v_billingcnt,balance)),0) ),0) BALANCE "
        		"	from acct_balance "
            " where acct_id=:v_acct_id and state='10A' " 
            " and (ITEM_GROUP_ID	<>20001 or OBJECT_TYPE_ID<>3)"
            " and (exists ( select * from billing_cycle where  billing_cycle_type_id=1 "
            " and billing_cycle_id "
            " in (select billing_cycle_id from billing_cycle where  billing_cycle_type_id=1 "
            " and state in ('10R','10A','10B','10D')) and   to_char(cycle_begin_date,'yyyymm') "
            " between to_char(nvl(eff_date,sysdate-1000),'yyyymm') "
            " and to_char(nvl(exp_date,sysdate+1000),'yyyymm') ) ) " );
        #else
        sprintf(sSql," select nvl(sum(nvl(decode(cycle_upper,Null,balance,decode(sign(cycle_upper*:v_billingcnt-balance),-1,cycle_upper*:v_billingcnt,balance)),0) ),0) BALANCE "
        		"	from acct_balance "
            " where acct_id=:v_acct_id and state='10A' " 
            " and (exists ( select * from billing_cycle where  billing_cycle_type_id=1 "
            " and billing_cycle_id "
            " in (select billing_cycle_id from billing_cycle where  billing_cycle_type_id=1 "
            " and state in ('10R','10A','10B','10D')) and   to_char(cycle_begin_date,'yyyymm') "
            " between to_char(nvl(eff_date,sysdate-1000),'yyyymm') "
            " and to_char(nvl(exp_date,sysdate+1000),'yyyymm') ) ) "
            " and balance_type_id not in ( %s ) " ,sBalanceType);
        qry1.setSQL(sSql);
	#endif
        qry1.setParameter("v_acct_id",lAcctID);
				qry1.setParameter("v_billingcnt",iBillingCnt);
        qry1.open();
        qry1.next();
        lBalance= qry1.field(0).asLong();
        qry1.close();
        if (iCreditFlag == 1){
          lAcctCredit = GetAcctCreditDB(lAcctID);
          lBalance = lBalance + lAcctCredit;
      	}
    }

    if (iFundsLevelID == 2)
    {
        qry1.close();
        #ifdef DEF_HAINAN
        qry1.setSQL(" select nvl(sum(nvl(decode(cycle_upper,Null,balance,decode(sign(cycle_upper*:v_billingcnt-balance),-1,cycle_upper*:v_billingcnt,balance)),0) ),0) BALANCE "
        		"	from acct_balance "
            " where acct_id=:v_acct_id and state='10A' " 
            " and (ITEM_GROUP_ID	<>20001 or OBJECT_TYPE_ID<>3)"
            " and (exists ( select * from billing_cycle where  billing_cycle_type_id=1 "
            " and billing_cycle_id "
            " in (select billing_cycle_id from billing_cycle where  billing_cycle_type_id=1 "
            " and state in ('10R','10A','10B','10D')) and   to_char(cycle_begin_date,'yyyymm') "
            " between to_char(nvl(eff_date,sysdate-1000),'yyyymm') "
            " and to_char(nvl(exp_date,sysdate+1000),'yyyymm') ) ) " );
        #else
        sprintf(sSql," select nvl(sum(nvl(decode(cycle_upper,Null,balance,decode(sign(cycle_upper*:v_billingcnt-balance),-1,cycle_upper*:v_billingcnt,balance)),0) ),0) BALANCE "
        		" from acct_balance "
                    " where serv_id=:v_serv_id and state='10A' "
            " and (exists ( select * from billing_cycle where  billing_cycle_type_id=1 "
            " and billing_cycle_id "
            " in (select billing_cycle_id from billing_cycle where  billing_cycle_type_id=1 "
            " and state in ('10R','10A','10B','10D')) and   to_char(cycle_begin_date,'yyyymm') "
            " between to_char(nvl(eff_date,sysdate-1000),'yyyymm') "
            " and to_char(nvl(exp_date,sysdate+1000),'yyyymm') ) ) " 
            " and balance_type_id not in ( %s ) " ,sBalanceType);
        qry1.setSQL(sSql);
	#endif
        qry1.setParameter("v_serv_id",lServID);
				qry1.setParameter("v_billingcnt",iBillingCnt);
        qry1.open();
        qry1.next();
        lBalance= qry1.field(0).asLong();
        qry1.close();
    }

    if (iFundsLevelID == 3)
    {
        qry1.close();
        #ifdef DEF_HAINAN
        qry1.setSQL(" select nvl(sum(nvl(decode(cycle_upper,Null,balance,decode(sign(cycle_upper*:v_billingcnt-balance),-1,cycle_upper*:v_billingcnt,balance)),0) ),0) BALANCE "
        		"	from acct_balance "
            " where acct_id=:v_acct_id and state='10A' " 
            " and (ITEM_GROUP_ID	<>20001 or OBJECT_TYPE_ID<>3)"
            " and (exists ( select * from billing_cycle where  billing_cycle_type_id=1 "
            " and billing_cycle_id "
            " in (select billing_cycle_id from billing_cycle where  billing_cycle_type_id=1 "
            " and state in ('10R','10A','10B','10D')) and   to_char(cycle_begin_date,'yyyymm') "
            " between to_char(nvl(eff_date,sysdate-1000),'yyyymm') "
            " and to_char(nvl(exp_date,sysdate+1000),'yyyymm') ) ) " );
        #else
        sprintf(sSql," select nvl(sum(nvl(decode(cycle_upper,Null,balance,decode(sign(cycle_upper*:v_billingcnt-balance),-1,cycle_upper*:v_billingcnt,balance)),0) ),0) BALANCE "
        		"from acct_balance "
                    " where acct_id=:v_acct_id and serv_id is null and state='10A' "
            " and (exists ( select * from billing_cycle where  billing_cycle_type_id=1 "
            " and billing_cycle_id "
            " in (select billing_cycle_id from billing_cycle where  billing_cycle_type_id=1 "
            " and state in ('10R','10A','10B','10D')) and   to_char(cycle_begin_date,'yyyymm') "
            " between to_char(nvl(eff_date,sysdate-1000),'yyyymm') "
            " and to_char(nvl(exp_date,sysdate+1000),'yyyymm') ) ) "
            " and balance_type_id not in ( %s ) " ,sBalanceType);
        qry1.setSQL(sSql);
	#endif
        qry1.setParameter("v_acct_id",lAcctID);
				qry1.setParameter("v_billingcnt",iBillingCnt);
        qry1.open();
        qry1.next();
        lBalance= qry1.field(0).asLong();
        qry1.close();
        if (iCreditFlag == 1)
        {
          lAcctCredit = GetAcctCreditDB(lAcctID);
          lBalance = lBalance + lAcctCredit;
        }
    }

    lCharge = lBalance - lOweCharge;
    return lCharge;
}

long Rt_Stop_Make::getValidBalance(long lAcctID,long lServID,int iBillingCycleID,long * plServItemBalance,long * plAcctItemBalance)
{
	long lValidBalance,lServItemBalance,lAcctItemBalance;
	StopChargeMgr * pStopCharge = NULL;
	
	pStopCharge = new StopChargeMgr(gsPreTableName);
	lValidBalance = pStopCharge->getBillBalance(lAcctID,lServID,iBillingCycleID,&lServItemBalance,&lAcctItemBalance);	
	glOweCharge = pStopCharge->stopCharge;
	glAggrCharge = 0;
	
	delete pStopCharge;
	pStopCharge = NULL;
	glBalance = lValidBalance;

	lservResItemBalance = lServItemBalance;
	lacctResItemBalance= lAcctItemBalance;
	
	return glBalance;
}


//return charge
long Rt_Stop_Make::GetBalanceByFunc(long lAcctID,long lServID,int iBillingCycleID,int iParam1, int iParam2 )
{
    long lCharge=0;

    int iCnt=0,iBalanceFlag=0;
    long lBalance=0,lServBalance=0,lAcctBalance=0,lServBalanceOther=0;
    long lOweCharge=0,lOweChargeOther=0;
    long lTmpBalance=0,lTmpServBalance=0;
    long lSubAcctID,lSubServID;
    
    int iCreditFlag = 0;
    int iPayRoleIDFlag = 0;

    long lChargedk=0;
    long lChargeOwe=0;
    //����ȡ��������޶���ж�
    BillingCycleMgr bcm;
    int     iBillingCnt , iPreBillingCycle;
		char sState[5],sDate[16];
		Date d;
		strncpy(sDate,d.toString(),8);
		sDate[8]=0;
		int iFullBalanceFlag =0;

		#ifdef DEF_GUIZHOU
    long lChargeZz=0,lChargeOweZz=0;
    static int iBillingOrg=0;
    static long lValueMaxCharge=0;
    static vector<int> m_vecAcctItemType;
    //��̬����,��ȡorg��Ӧ����Ŀ������޶�,����û���org��ǰһ���û���һ��,��ô�Ͳ���������ȡ
    if (iBillingOrg != giCurOrgID300)
    	 {
          //��ȡ������org
          iBillingOrg = giCurOrgID300;
          glMaxChargeVlue = VALUE_MAX_CHARGE;
          lValueMaxCharge = glMaxChargeVlue;
    	    m_vecAcctItemType.clear();
    	    DEFINE_QUERY(qry);;
          qry.close();
          qry.setSQL("select distinct acct_item_type_id from A_INC_ACCT_ITEM_TYPE where org_id in (1,:v_org_id) and state='10A' ");
          qry.setParameter("v_org_id",iBillingOrg);
    	    qry.open();
    	    while(qry.next())
    	        m_vecAcctItemType.push_back(qry.field(0).asInteger());
    	    qry.close();     
    	 }
		#endif

		#ifdef DEF_SICHUAN
		  iFullBalanceFlag = 1;
    	  iCreditFlag = iParam1;
    	  iPayRoleIDFlag = iParam2;
		#endif
		#ifdef DEF_SHANDONG
		  iFullBalanceFlag = 1;
		#endif
		#ifdef DEF_HAINAN
		  iFullBalanceFlag = 1;
		#endif
		strcpy(sState,bcm.getPreBillingCycle(iBillingCycleID)->getState());
		if (sState[2]=='A' or sState[2]=='B' or sState[2]=='D' or sState[2]=='R') iBillingCnt = 2;
		else iBillingCnt = 1 ;		
    		iPreBillingCycle = bcm.getPreBillingCycle(iBillingCycleID)->getBillingCycleID ();
//�ڴ�ȡ����    TOCIQuery qry1(&gpDBLink_STOP);
    //DEFINE_QUERY(qry1);
/*
    qry1.close();
    qry1.setSQL(" select nvl(count(0),0)  from serv_acct "
                " where acct_id=:v_acct_id and state='10A' ");
    qry1.setParameter("v_acct_id",lAcctID);

    qry1.open();
    qry1.next();
    iCnt = qry1.field(0).asInteger();
    qry1.close();
*/

    //long alServID[20000],lTemp;
    struct SERV_OWE_CHARGE_BUF strOweChargeTmp;
    int i,j;

    vector<ServAcctInfo> vPayInfo;
    vector<ServAcctInfo>::iterator iterPay;
    StopBalanceMgr sbm;

    sbm.loadOweCharge(lAcctID,iPayRoleIDFlag);
		#ifdef DEF_GUIZHOU
    sbm.loadOweChargeZz(lAcctID,iBillingOrg);
		#endif
    //û������˱��������û�ʵʱǷ�ѣ��û���ʷǷ��
    if ( GetBalance(lAcctID,lServID,1,iBillingCnt,iCreditFlag) == 0 )
    {
        #ifdef DEF_SICHUAN
          lCharge = Get10ACHARGE_DBComm(lServID,-1,2,iBillingCycleID,-1);
          if (iPayRoleIDFlag == 1)//������ش����޳�99�����շѣ�
          {	
          	long lPartyCharge = getPartyCharge(lServID,-1,iBillingCycleID,2,99);
          	lCharge = lCharge - lPartyCharge;
          }	
        #else
          lCharge = GetOweChargeByMemory(-1,lServID,iBillingCycleID,-1);
        #endif
        lChargeOwe = sbm.getHistoryOweCharge(-1,lServID,sState,iPreBillingCycle);//+��ʷǷ��
        
		    #ifdef DEF_GUIZHOU
        if (!GetOweSpecialServ(lServID,OWE_BTYPE_ID_ZZ))
        	 {  //��ʵʱ��ֳǷ�� 
              for(vector<int>::iterator it=m_vecAcctItemType.begin(); it!=m_vecAcctItemType.end(); it++)
              {	
          	  lChargeZz = lChargeZz + GetCurChargeByMemoryZz(-1,lServID,iBillingCycleID,*it);
          	  }
          	  lChargeZz = lChargeZz + GetPreAggrChargeCommZz(lServID,-1,iBillingCycleID,2,iBillingOrg);
          	  //cout<<"GetBalance==0��ʵʱ��ֳǷ��lChargeZz="<<lChargeZz<<endl;
          	  //����ʷ��ֳǷ��
              lChargeOweZz = sbm.getHistoryOweChargeZz(-1,lServID,sState,iPreBillingCycle);
        	    //cout<<"GetBalance==0����ʷ��ֳǷ��lChargeOweZz= "<<lChargeOweZz<<endl;
              lChargeZz = (lChargeZz+lChargeOweZz)>lValueMaxCharge ? lValueMaxCharge:lChargeZz+lChargeOweZz;
              lChargeOwe = lChargeOwe-lChargeZz;
        	 }
        #endif

        lCharge = lCharge + lChargeOwe;
        if (lCharge<0) lCharge=0;
        glAggrCharge = lCharge;
        lCharge = 0 - lCharge;
        return lCharge;
    }

    if (!G_PUSERINFO->getPayInfo (vPayInfo, lAcctID))  return 9999999999l;
    for (iterPay=vPayInfo.begin(),iCnt=0; iterPay!=vPayInfo.end(); iterPay++,iCnt++) {
        strOweCharge[iCnt].lServID = (*iterPay).m_lServID;
        #ifdef DEF_SICHUAN
        	strOweCharge[iCnt].lRtCharge = Get10ACHARGE_DBComm(strOweCharge[iCnt].lServID,-1,2,iBillingCycleID,-1);
        	if (iPayRoleIDFlag == 1)//������ش����޳�99�����շѣ�
         	{	
          		long lSCPartyCharge = getPartyCharge(strOweCharge[iCnt].lServID,-1,iBillingCycleID,2,99);          
          		strOweCharge[iCnt].lRtCharge = strOweCharge[iCnt].lRtCharge - lSCPartyCharge;
          	}
        #else
          strOweCharge[iCnt].lRtCharge = GetOweChargeByMemory(-1,strOweCharge[iCnt].lServID,iBillingCycleID,-1);
        #endif
        strOweCharge[iCnt].lOweCharge = sbm.getHistoryOweCharge(-1,strOweCharge[iCnt].lServID,sState,iPreBillingCycle);//��߲�����ʷǷ��
        strOweCharge[iCnt].lCharge = 0;
		#ifdef DEF_GUIZHOU
        lChargeZz=0;
        lChargeOweZz=0;	
        if (!GetOweSpecialServ(strOweCharge[iCnt].lServID ,OWE_BTYPE_ID_ZZ))
        	 {  //��ʵʱ��ֳǷ�� 
              for(vector<int>::iterator it=m_vecAcctItemType.begin(); it!=m_vecAcctItemType.end(); it++)
          	     lChargeZz = lChargeZz + GetCurChargeByMemoryZz(-1,strOweCharge[iCnt].lServID,iBillingCycleID,*it);
              lChargeZz = lChargeZz + GetPreAggrChargeCommZz(strOweCharge[iCnt].lServID,-1,iBillingCycleID,2,iBillingOrg);
             // cout<<"��serv_id="<<strOweCharge[iCnt].lServID<<"ʵʱ��ֳǷ��lChargeZz="<<lChargeZz<<endl;
              //����ʷ��ֳǷ��
              lChargeOweZz = sbm.getHistoryOweChargeZz(-1,strOweCharge[iCnt].lServID,sState,iPreBillingCycle);
        	   //cout<<"��serv_id="<<strOweCharge[iCnt].lServID<<"����ʷ��ֳǷ��lChargeOweZz= "<<lChargeOweZz<<endl;
              if ((lChargeZz+lChargeOweZz)>lValueMaxCharge)
              	 { //ʵʱ��ֵǷ�Ѻ���ʷ��ֵǷ�ѵĺʹ�����ֵ��������ֵ,��ô��ʵʱǷ�Ѻ���ʷǷ���м�ȥ�����ֵ�����ֵ
              	   lChargeZz = strOweCharge[iCnt].lRtCharge;
              	   lChargeOweZz = strOweCharge[iCnt].lOweCharge;
              	   strOweCharge[iCnt].lOweCharge=lChargeOweZz>lValueMaxCharge ? lChargeOweZz-lValueMaxCharge : 0 ;
              	   strOweCharge[iCnt].lRtCharge=strOweCharge[iCnt].lOweCharge>0 ? lChargeZz:(lChargeZz+lChargeOweZz)-lValueMaxCharge;
              	 }
              else
              	 {
              	 	//����,ʵʱǷ�Ѽ�����ʵʱ��ֵǷ��,��ʷǷ�Ѽ������ʷ��ֵǷ��
              	   strOweCharge[iCnt].lRtCharge=strOweCharge[iCnt].lRtCharge-lChargeZz; 
              	   strOweCharge[iCnt].lOweCharge=strOweCharge[iCnt].lOweCharge-lChargeOweZz; 
              	 }	 
        	 }
     #endif
        if (strOweCharge[iCnt].lServID == lServID) glAggrCharge=strOweCharge[iCnt].lRtCharge+ strOweCharge[iCnt].lOweCharge;
        	
    }

//    qry1.close();

    giOweChargeCnt = iCnt;
/*
    for (i = 0;i<iCnt;i++)
    {
        for (j=iCnt-1;j>i;j--)
        {
            if (alServID[i]>alServID[j])
            {
                lTemp = alServID[i];
                alServID[i] = alServID[j];
                alServID[j] = lTemp;
            }
        }
    }
*/

    for (i = 0;i<iCnt;i++)
    {
        for (j=iCnt-1;j>i;j--)
        {
            if (strOweCharge[i].lServID>strOweCharge[j].lServID)
            {
                memcpy(&strOweChargeTmp , &strOweCharge[i],sizeof(struct SERV_OWE_CHARGE_BUF));
                memcpy(&strOweCharge[i] , &strOweCharge[j],sizeof(struct SERV_OWE_CHARGE_BUF));
                memcpy(&strOweCharge[j] , &strOweChargeTmp,sizeof(struct SERV_OWE_CHARGE_BUF));
            }
        }
    }

    //��֯���ã��ȵֿ�ʵʱ���ٵֿ���ʷ
    for (i=0;i<iCnt;i++)
    {
        if (strOweCharge[i].lRtCharge < 0)
        {
            lChargedk = lChargedk - strOweCharge[i].lRtCharge;
            strOweCharge[i].lRtCharge = 0;
        }
        if (strOweCharge[i].lOweCharge < 0)
        {
            lChargedk = lChargedk - strOweCharge[i].lOweCharge;
            strOweCharge[i].lOweCharge = 0;
        }
    }
    //�ֿ�ʵʱ
    for (i=0;i<iCnt;i++)
    {
        if (lChargedk == 0) break;

        if (strOweCharge[i].lRtCharge == 0) continue;

        if ( strOweCharge[i].lRtCharge < lChargedk )
        {
            lChargedk = lChargedk - strOweCharge[i].lRtCharge;
            strOweCharge[i].lRtCharge = 0;
        }
        else
        {
            strOweCharge[i].lRtCharge = strOweCharge[i].lRtCharge - lChargedk;
            lChargedk = 0;
        }
    }
    //�ֿ���ʷ
    for (i=0;i<iCnt;i++)
    {
        if (lChargedk == 0) break;

        if (strOweCharge[i].lOweCharge == 0) continue;

        if ( strOweCharge[i].lOweCharge < lChargedk )
        {
            lChargedk = lChargedk - strOweCharge[i].lOweCharge;
            strOweCharge[i].lOweCharge = 0;
        }
        else
        {
            strOweCharge[i].lOweCharge = strOweCharge[i].lOweCharge - lChargedk;
            lChargedk = 0;
        }
    }

    if (iCnt == 1)
    {
        lBalance = GetBalance(lAcctID,lServID,1,iBillingCnt,iCreditFlag);
        //lOweCharge = GetOweChargeByMemory(lAcctID,lServID,-1);
        lOweCharge = GetOweChargeByStruct(-1,lServID,-1);
        lCharge = lBalance - lOweCharge;
    }

    if (iCnt >= 2)
    {
        
        sbm.loadAcctBalance(lAcctID,iBillingCnt);

        iBalanceFlag = 1;
    }

    if (iCnt > 1)
    {
        if (iBalanceFlag == 0)
        {
            lAcctBalance = GetBalance(lAcctID,lServID,3,iBillingCnt,iCreditFlag);
            lServBalance = GetBalance(lAcctID,lServID,2,iBillingCnt,iCreditFlag);
        }
        else
        {
            lAcctBalance = sbm.getAcctBalance(lAcctID,lServID,3,iCreditFlag);
            lServBalance = sbm.getAcctBalance(lAcctID,lServID,2);//
        }
        //lOweCharge = GetOweChargeByMemory(-1,lServID,-1);
        lOweCharge = GetOweChargeByStruct(-1,lServID,-1);

/*    
        qry1.setSQL(" select acct_id,serv_id  from serv_acct "
                    " where acct_id=:v_acct_id and state='10A' ");
        qry1.setParameter("v_acct_id",lAcctID);

        qry1.open();
        while (qry1.next())
*/
        for (i=0;i<iCnt;i++)
        {
/*
            lSubAcctID = qry1.field(0).asLong();
            lSubServID = qry1.field(1).asLong();
*/
            lSubAcctID = lAcctID;
            lSubServID = strOweCharge[i].lServID;

            lTmpBalance = 0;
            lOweChargeOther = 0;

            if (lSubServID == lServID)
            {
                lTmpServBalance = lOweCharge - lServBalance;
                if (lTmpServBalance > 0)
                {
                    /*if (lTmpServBalance > lAcctBalance)
                        lTmpServBalance = lAcctBalance;*/
                    lAcctBalance = lAcctBalance - lTmpServBalance;
                }
            }
            else
            {
                if (iBalanceFlag == 0)
                    lServBalanceOther = GetBalance(lAcctID,lSubServID,2,iBillingCnt,iCreditFlag);
                else
                    lServBalanceOther = sbm.getAcctBalance(lAcctID,lSubServID,2);

                //lOweChargeOther = GetOweChargeByMemory(-1,lSubServID,-1);
                lOweChargeOther = GetOweChargeByStruct(-1,lSubServID,-1);
                lTmpBalance = lOweChargeOther - lServBalanceOther;
                if (lTmpBalance > 0)
                    lAcctBalance = lAcctBalance - lTmpBalance;

            }
            
            if (iFullBalanceFlag) ///��Ҫ�����������,�����Ƿ���0
              if (lServID > 0)
              {
              	//����ʻ����<0,���û������ֿ۴���0,�ɵ�Ҫ��ֱ�ӷ����û������ֿ۽��
              	if (lAcctBalance < 0)
              	{
              		if(lServBalance - lOweCharge >= 0)
              			return lServBalance - lOweCharge;              		              			
             		#ifdef STOP_RETURN_SERV_CHARGE
 						//�ֿ۵�ǰ�û�֮ǰ�˻������<0ֱ�ӷ����û����ֿ����
 						//�ֿ۵�ǰ�û�ʱ�˻������<0 ���ص���ֹĿǰ�ֿ۵��˻������
 						//�ֿ۵�ǰ�û�֮���˻������<0����0
                		if(lSubServID != lServID)  //add by ql @hainan 080407
                    		lAcctBalance = 0;
 
                    	//if (lTmpServBalance < 0)lTmpServBalance>0or=0
                    	//    lTmpServBalance = 0;
                    	
                    	lCharge = ( lServBalance - lOweCharge ) + lTmpServBalance + lAcctBalance;
                    	
                    	return lCharge;              			
              		#endif	              			
              	}
              }
           else
            if (lServID > 0)
            {
                if (lAcctBalance <= 0)
                {
                	if(lSubServID != lServID)  //add by ql @hainan 080407
                    lAcctBalance = 0;

                    if (lTmpServBalance < 0)
                        lTmpServBalance = 0;

                    lCharge = ( lServBalance - lOweCharge ) + lTmpServBalance + lAcctBalance;

                    return lCharge;
                }
            }

        }

//        qry1.close();

        if (lTmpServBalance < 0)
            lTmpServBalance = 0;

        lCharge = ( lServBalance - lOweCharge ) + lTmpServBalance + lAcctBalance;

    }

    return lCharge;

}

//return charge
//����ԭ�����޸� ����Ӱ������ʡ��
long Rt_Stop_Make::GetBalanceByFuncEx(long lAcctID,long lServID,int iBillingCycleID,int iParam1, int iParam2 )
{
    long lCharge=0;

    int iCnt=giOweChargeCnt,iBalanceFlag=0;//�����Ѹ�
    long lBalance=0,lServBalance=0,lAcctBalance=0,lServBalanceOther=0;
    long lOweCharge=0,lOweChargeOther=0;
    long lTmpBalance=0,lTmpServBalance=0;
    long lSubAcctID,lSubServID;
    
    int iCreditFlag = 0;
    int iPayRoleIDFlag = 0;

    long lChargedk=0;
    long lChargeOwe=0;
    //����ȡ��������޶���ж�
    BillingCycleMgr bcm;
    int     iBillingCnt , iPreBillingCycle;
		char sState[5],sDate[16];
		Date d;
		strncpy(sDate,d.toString(),8);
		sDate[8]=0;
		int iFullBalanceFlag =0;
		#ifdef DEF_SICHUAN
		  iFullBalanceFlag = 1;
    	  iCreditFlag = iParam1;
    	  iPayRoleIDFlag = iParam2;
		#endif
		#ifdef DEF_SHANDONG
		  iFullBalanceFlag = 1;
		#endif
		#ifdef DEF_HAINAN
		  iFullBalanceFlag = 1;
		#endif
		//������μ�������С���ܲ�һ���Ⱥ���
		strcpy(sState,bcm.getPreBillingCycle(iBillingCycleID)->getState());
		if (sState[2]=='A' or sState[2]=='B' or sState[2]=='D' or sState[2]=='R') iBillingCnt = 2;
		else iBillingCnt = 1 ;		
    		iPreBillingCycle = bcm.getPreBillingCycle(iBillingCycleID)->getBillingCycleID ();

    static StopBalanceMgr sbm;
    
	if (giCalcLevel == 0)//δ���������������һ�Σ����㼶��������1
	{	
    	sbm.loadOweCharge(lAcctID,iPayRoleIDFlag);
    }
    
    //û������˱��������û�ʵʱǷ�ѣ��û���ʷǷ��//
    if ( GetBalance(lAcctID,lServID,1,iBillingCnt,iCreditFlag) == 0 )
    {
    	if (giCalcLevel > 0)
    	{
    		return glAggrCharge;
    	}	
        #ifdef DEF_SICHUAN
          lCharge = Get10ACHARGE_DBComm(lServID,-1,2,iBillingCycleID,-1);
          if (iPayRoleIDFlag == 1)//������ش����޳�99�����շѣ�
          {	
          	long lPartyCharge = getPartyCharge(lServID,-1,iBillingCycleID,2,99);
          	lCharge = lCharge - lPartyCharge;
          }	
        #else
          lCharge = GetOweChargeByMemory(-1,lServID,iBillingCycleID,-1);
        #endif
        lChargeOwe = sbm.getHistoryOweCharge(-1,lServID,sState,iPreBillingCycle);//+��ʷǷ��
        
        lCharge = lCharge + lChargeOwe;
        if (lCharge<0) lCharge=0;
        glAggrCharge = lCharge;
        lCharge = 0 - lCharge;
        
        giCalcLevel = 1;//����loadOweCharge���� �˴���glAggrCharge 
        
        return lCharge;
    }

	int i,j;
	
//Ƿ�ѵֿ���
if (giCalcLevel < 2)//�ϴ�����û�����ö� ���ܻ��� 1���˳�
{	
    struct SERV_OWE_CHARGE_BUF strOweChargeTmp;

    vector<ServAcctInfo> vPayInfo;
    vector<ServAcctInfo>::iterator iterPay;
    if (!G_PUSERINFO->getPayInfo (vPayInfo, lAcctID))  return 9999999999l;
    for (iterPay=vPayInfo.begin(),iCnt=0; iterPay!=vPayInfo.end(); iterPay++,iCnt++) {
        strOweCharge[iCnt].lServID = (*iterPay).m_lServID;
        #ifdef DEF_SICHUAN
        	strOweCharge[iCnt].lRtCharge = Get10ACHARGE_DBComm(strOweCharge[iCnt].lServID,-1,2,iBillingCycleID,-1);
        	if (iPayRoleIDFlag == 1)//������ش����޳�99�����շѣ�
         	{	
          		long lSCPartyCharge = getPartyCharge(strOweCharge[iCnt].lServID,-1,iBillingCycleID,2,99);          
          		strOweCharge[iCnt].lRtCharge = strOweCharge[iCnt].lRtCharge - lSCPartyCharge;
          	}
        #else
          strOweCharge[iCnt].lRtCharge = GetOweChargeByMemory(-1,strOweCharge[iCnt].lServID,iBillingCycleID,-1);
        #endif
        strOweCharge[iCnt].lOweCharge = sbm.getHistoryOweCharge(-1,strOweCharge[iCnt].lServID,sState,iPreBillingCycle);//��߲�����ʷǷ��
        strOweCharge[iCnt].lCharge = 0;
        if (strOweCharge[iCnt].lServID == lServID) glAggrCharge=strOweCharge[iCnt].lRtCharge+ strOweCharge[iCnt].lOweCharge;
        	
    }

//    qry1.close();

    giOweChargeCnt = iCnt;
/*
    for (i = 0;i<iCnt;i++)
    {
        for (j=iCnt-1;j>i;j--)
        {
            if (alServID[i]>alServID[j])
            {
                lTemp = alServID[i];
                alServID[i] = alServID[j];
                alServID[j] = lTemp;
            }
        }
    }
*/

    for (i = 0;i<iCnt;i++)
    {
        for (j=iCnt-1;j>i;j--)
        {
            if (strOweCharge[i].lServID>strOweCharge[j].lServID)
            {
                memcpy(&strOweChargeTmp , &strOweCharge[i],sizeof(struct SERV_OWE_CHARGE_BUF));
                memcpy(&strOweCharge[i] , &strOweCharge[j],sizeof(struct SERV_OWE_CHARGE_BUF));
                memcpy(&strOweCharge[j] , &strOweChargeTmp,sizeof(struct SERV_OWE_CHARGE_BUF));
            }
        }
    }

    //��֯���ã��ȵֿ�ʵʱ���ٵֿ���ʷ
    for (i=0;i<iCnt;i++)
    {
        if (strOweCharge[i].lRtCharge < 0)
        {
            lChargedk = lChargedk - strOweCharge[i].lRtCharge;
            strOweCharge[i].lRtCharge = 0;
        }
        if (strOweCharge[i].lOweCharge < 0)
        {
            lChargedk = lChargedk - strOweCharge[i].lOweCharge;
            strOweCharge[i].lOweCharge = 0;
        }
    }
    //�ֿ�ʵʱ
    for (i=0;i<iCnt;i++)
    {
        if (lChargedk == 0) break;

        if (strOweCharge[i].lRtCharge == 0) continue;

        if ( strOweCharge[i].lRtCharge < lChargedk )
        {
            lChargedk = lChargedk - strOweCharge[i].lRtCharge;
            strOweCharge[i].lRtCharge = 0;
        }
        else
        {
            strOweCharge[i].lRtCharge = strOweCharge[i].lRtCharge - lChargedk;
            lChargedk = 0;
        }
    }
    //�ֿ���ʷ
    for (i=0;i<iCnt;i++)
    {
        if (lChargedk == 0) break;

        if (strOweCharge[i].lOweCharge == 0) continue;

        if ( strOweCharge[i].lOweCharge < lChargedk )
        {
            lChargedk = lChargedk - strOweCharge[i].lOweCharge;
            strOweCharge[i].lOweCharge = 0;
        }
        else
        {
            strOweCharge[i].lOweCharge = strOweCharge[i].lOweCharge - lChargedk;
            lChargedk = 0;
        }
    }
    
    giCalcLevel = 2;//����Ƿ�ѵĵֿ�
}
//�����
//iCnt ����giOweChargeCnt
    if (giOweChargeCnt == 1)
    {
        lBalance = GetBalance(lAcctID,lServID,1,iBillingCnt,iCreditFlag);
        //lOweCharge = GetOweChargeByMemory(lAcctID,lServID,-1);
        lOweCharge = GetOweChargeByStruct(-1,lServID,-1);
        lCharge = lBalance - lOweCharge;
    }

    if (giOweChargeCnt >= 2)
    {
        //
        if (giCalcLevel < 3)
        {	
        	sbm.loadAcctBalance(lAcctID,iBillingCnt);//�����ʱ�򲻿������öȣ�get��ʱ���ٿ��Ƿ������ö�
        	giCalcLevel = 3;
        }	

        iBalanceFlag = 1;//��������
    }
//��������Ҫ����
    if (giOweChargeCnt > 1)
    {
        if (iBalanceFlag == 0)
        {
            lAcctBalance = GetBalance(lAcctID,lServID,3,iBillingCnt,iCreditFlag);
            lServBalance = GetBalance(lAcctID,lServID,2,iBillingCnt);//�û��������ö�
        }
        else
        {
            lAcctBalance = sbm.getAcctBalance(lAcctID,lServID,3,iCreditFlag);
            lServBalance = sbm.getAcctBalance(lAcctID,lServID,2);
        }
        //lOweCharge = GetOweChargeByMemory(-1,lServID,-1);
        lOweCharge = GetOweChargeByStruct(-1,lServID,-1);

/*    
        qry1.setSQL(" select acct_id,serv_id  from serv_acct "
                    " where acct_id=:v_acct_id and state='10A' ");
        qry1.setParameter("v_acct_id",lAcctID);

        qry1.open();
        while (qry1.next())
*/
//
        for (i=0;i<giOweChargeCnt;i++)
        {
/*
            lSubAcctID = qry1.field(0).asLong();
            lSubServID = qry1.field(1).asLong();
*/
            lSubAcctID = lAcctID;
            lSubServID = strOweCharge[i].lServID;

            lTmpBalance = 0;
            lOweChargeOther = 0;

            if (lSubServID == lServID)
            {
                lTmpServBalance = lOweCharge - lServBalance;
                if (lTmpServBalance > 0)
                {
                    /*if (lTmpServBalance > lAcctBalance)
                        lTmpServBalance = lAcctBalance;*/
                    lAcctBalance = lAcctBalance - lTmpServBalance;
                }
            }
            else
            {
                if (iBalanceFlag == 0)
                    lServBalanceOther = GetBalance(lAcctID,lSubServID,2,iBillingCnt,iCreditFlag);
                else
                    lServBalanceOther = sbm.getAcctBalance(lAcctID,lSubServID,2);

                //lOweChargeOther = GetOweChargeByMemory(-1,lSubServID,-1);
                lOweChargeOther = GetOweChargeByStruct(-1,lSubServID,-1);
                lTmpBalance = lOweChargeOther - lServBalanceOther;
                if (lTmpBalance > 0)
                    lAcctBalance = lAcctBalance - lTmpBalance;

            }
            
            if (iFullBalanceFlag) ///��Ҫ�����������,�����Ƿ���0
              if (lServID > 0)
              {
              	//����ʻ����<0,���û������ֿ۴���0,�ɵ�Ҫ��ֱ�ӷ����û������ֿ۽��
              	if (lAcctBalance < 0)
              	{
              		if(lServBalance - lOweCharge >= 0)
              			return lServBalance - lOweCharge;
              	}
              }
           else
            if (lServID > 0)
            {
                if (lAcctBalance <= 0)
                {
                	if(lSubServID != lServID)  //add by ql @hainan 080407
                    lAcctBalance = 0;

                    if (lTmpServBalance < 0)
                        lTmpServBalance = 0;

                    lCharge = ( lServBalance - lOweCharge ) + lTmpServBalance + lAcctBalance;

                    return lCharge;
                }
            }

        }

//        qry1.close();

        if (lTmpServBalance < 0)
            lTmpServBalance = 0;

        lCharge = ( lServBalance - lOweCharge ) + lTmpServBalance + lAcctBalance;

    }

    return lCharge;

}

long Rt_Stop_Make::LocalFunction(long lAcctID, long lServID, int iBillingCycleID,int iParam1, int iParam2)
{
    //DEFINE_QUERY(qry);
    TOCIQuery qry(&gpDBLink_STOP);  //��ʱ
    int iValueMethod = VALUE_METHOD;
    //int iValueMethod ;
    int ret;
    long lCharge;
    
	char sSql[2000];
	//long	lCharge=0;
#ifdef DEF_SICHUAN
	if (giBalanceCnt > 0)//�Ѿ��������
	{
		if (lServID == giLastlServID)//�ϴμ����ServID�ʹ˴��Ƿ�һ��
		{	
			if (giCalcCredit ==	iParam1)//�ϴμ�������öȺʹ˴��Ƿ�Ҳһ��
				return glBalance;
				
			/*if (giCalcCredit == 1)	//��ֹ���� �Ȳο����ö� ������Ĭ�� �Ȳ��ο����ö� ��ο�
			{
				giCreditCnt = 0;
				giAcctCredit = 0;
			}	*/
		}	
		else
		{
			giCalcLevel = 0;
		}	
	}	
	else
	{
		giCalcLevel = 0;//�������öȲ�һ�²ų�ʼ��
	}	
	
	giCalcCredit = 	iParam1;//δ���㣬���߲�һ�� �������öȲ�����
	giLastlServID = lServID;
	
#else
	if (giBalanceCnt > 0) return glBalance;
#endif		

    //test
    //iValueMethod = 2;

    //���÷���
    if (iValueMethod == 1)
    {
        ret = GetBalanceByServe(lAcctID,lServID,iParam1,iParam2,&lCharge );

        if (ret == -1)
        {
            Log::log(0,"�������ʧ�ܣ��ĵ�������serv_id=%ld ",lServID);
            lCharge = GetBalanceByFunc(lAcctID,lServID,iBillingCycleID,iParam1,iParam2 );
            glBalance = lCharge;
            giBalanceCnt++;
        }
        else
        {
            glBalance = lCharge;
            giBalanceCnt++;
        }
    }

    //���ú�̨����
    if (iValueMethod == 2)
    {        
	#ifdef DEF_SICHUAN
		lCharge = GetBalanceByFuncEx(lAcctID,lServID,iBillingCycleID,iParam1,iParam2 );
	#else
		lCharge = GetBalanceByFunc(lAcctID,lServID,iBillingCycleID,iParam1,iParam2 );
	#endif		        
		glBalance = lCharge;
        giBalanceCnt++;
    }

    //ֱ�ӵ��ú���
    if (iValueMethod == 3)
    {
    //GetCalcBalanceӦ����acct�û���
    sprintf(sSql,"SELECT GetCalcBalance(:lAcctID,:lServID,:iParam1) CHARGE FROM DUAL");
    qry.close();
    qry.setSQL(sSql);
    qry.setParameter("lAcctID",lAcctID);
    qry.setParameter("lServID",lServID);
    qry.setParameter("iParam1",iParam1);
    qry.open();
    if(!qry.next())
        glBalance= 0;
    else 
        glBalance= qry.field("CHARGE").asInteger();
		
    qry.close();

    giBalanceCnt++;	
    }

   	return glBalance;  

}

int Rt_Stop_Make::GetAnotherMem(long lAcctID, long lServID, long &lAnotherServID, int iSourceOweBTypeID)
{
    DEFINE_QUERY(qry);
    char sSql[2000], sSql1[1000];
    long lAnotherAcctID;

//������ b_make_stop_offer ���701,1276 2007.08.07 suzhou     
    sprintf(sSql, "SELECT instance_id FROM product_offer_instance_detail WHERE product_offer_instance_id in "
                 "(SELECT a.product_offer_instance_id "
                 "FROM product_offer_instance a, product_offer_instance_detail b "
                 "WHERE a.product_offer_instance_id = b.product_offer_instance_id "
                 "AND b.instance_id = :INSTANCE_ID "
                 " AND a.product_offer_id in ( select offer_id from b_make_stop_offer "
                 " where source_business_type_id= :SOURCE_TYPE_ID ) "
                 "AND a.state='10A' "
                 "AND b.instance_type='10A') "
                 " AND instance_id <> :INSTANCE_ID "
                 " and trunc(sysdate)>=trunc(nvl(eff_date,sysdate-1)) "
                 " and trunc(sysdate)<trunc(nvl(exp_date,sysdate+1)) " );

    qry.close();
    qry.setSQL(sSql);
    qry.setParameter("INSTANCE_ID",lServID);
    qry.setParameter("SOURCE_TYPE_ID",iSourceOweBTypeID);
    qry.open();
    
    if(!qry.next())
    {
        lAnotherServID = 0;
        return 0;
    }
    
    lAnotherServID = qry.field(0).asLong();

    //����һ����¼,������
    if (qry.next())
    {
        lAnotherServID = 0;       
        return -1; 
    }
        
    qry.close();
    
    sprintf(sSql1, "select acct_id from serv_acct where serv_id = :SERV_ID and state='10A'");
    qry.setSQL (sSql1);
    qry.setParameter("SERV_ID",lAnotherServID);
    qry.open();
    
    if(!qry.next())
    {
        lAnotherServID = 0;
        return 0;
    }
    
    lAnotherAcctID = qry.field(0).asLong();
    
    if (lAcctID != lAnotherAcctID)
    {
        lAnotherServID = 0;
        return 0;
    }
    
    qry.close();
    
    return 1;
}



/**������Ҫ��IP�����û��͵����ı�,iServSeqNbr����Ҫ��**/
/*#ifdef DEF_SUZHOU */
void Rt_Stop_Make::InsertIPOweDun(long lServID, int iOweBTypeID)
{
    int iStaffID;
    long lOweDunID;

    DEFINE_QUERY(qry);
    char sSql[2000];
    
	iStaffID = DEF_STAFF_ID;

    AddCountLimit(STOP_LIMIT_TJ);

    lOweDunID = GetOweDunID();

//glOweChargeʵʱǷ�ѵ���ȡ,����ϵͳ��ͳһ
/*  if( glOweCharge == -1 )
    {
	    EXEC SQL AT :DB_BILL SELECT GETREALCHARGE(-1,:lServID,:iServSeqNbr,-1)
		   INTO :glOweCharge
		   FROM DUAL;
	    if (DataNotFound) glOweCharge=0;
            if (sqlca.sqlcode<0) glOweCharge=0;
    }
*/
		
    sprintf(sSql,"INSERT INTO A_OWE_DUN_IP_SERV (OWE_DUN_ID,STAFF_ID,SERV_ID,SERV_SEQ_NBR,\
                  BILLING_CYCLE_ID ,CHARGE,DUE,OWE_BUSINESS_TYPE_ID,CREATED_DATE,STATE,MAX_CHARGE,\
                  MIN_CHARGE,CUR_BALANCE,STATE_DATE) \
                  VALUES (:OWE_DUN_ID,:STAFF_ID,:SERV_ID,:SERV_SEQ_NBR,:BILLING_CYCLE_ID,:CHARGE,\
                  0,:OWE_BUSINESS_TYPE_ID,SYSDATE,'I0C',:MAX_CHARGE,:MIN_CHARGE,:CUR_BALANCE,SYSDATE )");
		
    qry.close();
    qry.setSQL(sSql);
    qry.setParameter("OWE_DUN_ID", lOweDunID);
    qry.setParameter("STAFF_ID", iStaffID);
    qry.setParameter("SERV_ID", lServID);
    qry.setParameter("SERV_SEQ_NBR", 1);
    qry.setParameter("BILLING_CYCLE_ID", gstrEvent.m_iBillingCycleID);
    qry.setParameter("CHARGE", glOweCharge+glAggrCharge);
    qry.setParameter("OWE_BUSINESS_TYPE_ID", iOweBTypeID);
    qry.setParameter("MAX_CHARGE", glMaxCharge);
    qry.setParameter("MIN_CHARGE", glMinCharge);
    qry.setParameter("CUR_BALANCE", glBalance);
    qry.execute();


    sprintf(sSql,"INSERT INTO A_OWE_DUN_IP_SERV_LOG (OWE_DUN_LOG_ID,STAFF_ID,SERV_ID,SERV_SEQ_NBR,\
                  BILLING_CYCLE_ID ,CHARGE,DUE,OWE_BUSINESS_TYPE_ID,CREATED_DATE,STATE,MAX_CHARGE,\
                  MIN_CHARGE,CUR_BALANCE,STATE_DATE) \
                  VALUES (:OWE_DUN_LOG_ID,:STAFF_ID,:SERV_ID,:SERV_SEQ_NBR,:BILLING_CYCLE_ID,:CHARGE,\
                  0,:OWE_BUSINESS_TYPE_ID,SYSDATE,'I0C',:MAX_CHARGE,:MIN_CHARGE,:CUR_BALANCE,SYSDATE )");
		
    qry.close();
    qry.setSQL(sSql);
    qry.setParameter("OWE_DUN_LOG_ID", lOweDunID);
    qry.setParameter("STAFF_ID", iStaffID);
    qry.setParameter("SERV_ID", lServID);
    qry.setParameter("SERV_SEQ_NBR", 1);
    qry.setParameter("BILLING_CYCLE_ID", gstrEvent.m_iBillingCycleID);
    qry.setParameter("CHARGE", glOweCharge+glAggrCharge);
    qry.setParameter("OWE_BUSINESS_TYPE_ID", iOweBTypeID);
    qry.setParameter("MAX_CHARGE", glMaxCharge);
    qry.setParameter("MIN_CHARGE", glMinCharge);
    qry.setParameter("CUR_BALANCE", glBalance);
    qry.execute();
    
    backupData(iOweBTypeID);
}
/*#endif*/



/*#ifdef DEF_SUZHOU*/
int Rt_Stop_Make::CheckOweDunIPExist(long lServID)
{
    
    char sState[6], sAction[6];
    int  iOweBTypeID;
    long lCurBalance;
    long lMaxCharge;
    long lMinCharge;

    DEFINE_QUERY(qry);

    qry.close();
    qry.setSQL("SELECT /*+rule+*/STATE,OWE_BUSINESS_TYPE_ID,NVL(MAX_CHARGE,0) MAX_CHARGE,NVL(MIN_CHARGE,0) MIN_CHARGE,NVL(CUR_BALANCE,0) CUR_BALANCE	\
              FROM A_OWE_DUN_IP_SERV_LOG \
              WHERE OWE_DUN_LOG_ID = (SELECT MAX(OWE_DUN_LOG_ID) FROM A_OWE_DUN_IP_SERV_LOG WHERE SERV_ID = :SERV_ID)");
    
    qry.setParameter("SERV_ID", lServID);
    qry.open();
   
    if (!qry.next())
        return 0;
	 
    strcpy(sState,qry.field("STATE").asString());
    iOweBTypeID = qry.field("OWE_BUSINESS_TYPE_ID").asInteger();
    lCurBalance = qry.field("MAX_CHARGE").asLong();
    lMaxCharge  = qry.field("MIN_CHARGE").asLong();
    lMinCharge  = qry.field("CUR_BALANCE").asLong();
    qry.close();

    AllTrim(sState);

    if(iOweBTypeID == 1 && strcmp(sState,"I0C") == 0)
        return 3;

    /*�ϴν����뱾�η�ֵ�Ƚ�,�������ֵⷧ��,�򲻴߽�**/
    if(iOweBTypeID == 1 && lCurBalance>=glMinCharge && lCurBalance<glMaxCharge)
    	return 3;
    	
    return 0;
}
/*#endif*/

int Rt_Stop_Make::CheckStopMiddleExist(long lServID)
{
	DEFINE_QUERY(qry);

	qry.close();
	qry.setSQL("SELECT 1 FROM B_STOP_MIDDLE_INFO WHERE SERV_ID=:SERV_ID AND STATE='P0C'");
	qry.setParameter("SERV_ID",lServID);
	qry.open();
	
	if (!qry.next())
		return 0;
	else{
		return 1;
	}
	return 0;
}


//�����Զ���ֵ����
int Rt_Stop_Make::CheckAutoFunds(long lServID)
{
	int iRet=0;
	
	DEFINE_QUERY( qry );
	
	qry.close();
	qry.setSQL("SELECT COUNT(*) COUNT FROM A_BANK_SERV_AUTOFUNDS WHERE SERV_ID = :SERV_ID AND STATE = 'F0A'");
	qry.setParameter("SERV_ID", lServID);
	qry.open();
    if(!qry.next())
        return iRet;
	
	iRet = qry.field("COUNT").asInteger(); 
	qry.close();
	
	return iRet;
}

/*1-owe,2-stop*/
void Rt_Stop_Make::InsertOrderBank(long lServID, int iType)
{
	int iStaffID,iRet;
	long SerialNbr;
	
	DEFINE_QUERY(qry);
	
	qry.close();
	qry.setSQL("SELECT COUNT(*) COUNT FROM A_ORDER_TO_BANK WHERE SERV_ID = :SERV_ID AND STATE = '0'");
	qry.setParameter("SERV_ID", lServID);
	qry.open();
	if (!qry.next())
	    return;
	iRet = qry.field("COUNT").asInteger();	
	qry.close();
	
	if (iRet > 0) 
	    return;
	
	iStaffID = DEF_STAFF_ID;
  
	if (iType==1)
  	    SerialNbr = GetOweDunID();
	if (iType==2)
		SerialNbr = GetSerialNbr();

    qry.setSQL("INSERT INTO A_ORDER_TO_BANK(ORDER_NBR,STAFF_ID,SERV_ID,SERV_SEQ_NBR,CREATED_DATE,SOURCE_TYPE_ID, \
    CUR_BALANCE,STATE) VALUES (:ORDER_NBR,:STAFF_ID,:SERV_ID,:SERV_SEQ_NBR,SYSDATE,:SOURCE_TYPE_ID,:CUR_BALANCE,:STATE)");

    qry.setParameter("ORDER_NBR",SerialNbr);
    qry.setParameter("STAFF_ID",iStaffID);
    qry.setParameter("SERV_ID",lServID);
	qry.setParameter("SERV_SEQ_NBR",1);
	qry.setParameter("SOURCE_TYPE_ID",iType);
	qry.setParameter("CUR_BALANCE",glBalance);
	qry.setParameter("STATE",0);
	qry.execute();	
}

void  Rt_Stop_Make::InsertStopMiddle(long lServID,long lAcctID,int iErrorID)
{
	int iOweBusinessType;
	long OrderSerialNbr;
	switch (iErrorID){
		case 1:
			iOweBusinessType = OWE_BTYPE_ID_TJ;
			break;
		case 2:
		case 5:
		case 8:
		case 11:
			iOweBusinessType = OWE_BTYPE_ID_ST;
			break;
		case 14:
			iOweBusinessType = OWE_BTYPE_ID_ST;
			break;
		case 3:
		case 4:
		case 13:
			iOweBusinessType = OWE_BTYPE_ID_CJ;
			break;
		case 6:
		case 10:
			iOweBusinessType = OWE_BTYPE_ID_SK;
			break;
		case 7:
		case 9:
			iOweBusinessType = OWE_BTYPE_ID_DK;
			break;
		case 12:
			iOweBusinessType = OWE_BTYPE_ID_ST;
			break;
		case 18:
			iOweBusinessType = OWE_BTYPE_ID_TJ;
			break;
		default:
			iOweBusinessType = OWE_BTYPE_ID_CJ;
			break;
	}
	if (!CheckStopMiddleExist(lServID))
	{
		DEFINE_QUERY(qry);
		char sSql[2048];
		
		glOweCharge = GetOweCharge(lServID,-1,-1);
		glAggrCharge = GetOweChargeByMemory(-1,lServID,giBillingCycleID,-1);

		OrderSerialNbr=GetSerialNbr();
		sprintf(sSql,"INSERT INTO B_STOP_MIDDLE_INFO (ORDER_SERIAL_NBR,SERV_ID,ACCT_ID,"
		"OWE_BUSINESS_TYPE_ID,ACTION,CREATED_DATE ,STATE,STATE_DATE,"
		"OWE_CHARGE,RT_CHARGE,BALANCE,CREDIT) "
		"VALUES (:OrderSerialNbr,:SERV_ID,:ACCT_ID,:OWE_BUSINESS_TYPE_ID,:ACTION,SYSDATE,'P0C',sysdate,"
		":OWE_CHARGE,:RT_CHARGE,:BALANCE,:CREDIT )");
		
		qry.close();
		qry.setSQL(sSql);
		qry.setParameter("OrderSerialNbr",OrderSerialNbr);
		qry.setParameter("SERV_ID",lServID);
		qry.setParameter("ACCT_ID",lAcctID);
		qry.setParameter("OWE_BUSINESS_TYPE_ID",iOweBusinessType);
		qry.setParameter("ACTION",0);
		qry.setParameter("OWE_CHARGE",glOweCharge);
		qry.setParameter("RT_CHARGE",glAggrCharge);
		qry.setParameter("BALANCE",glBalance);
		qry.setParameter("CREDIT",giAcctCredit);
		
		qry.execute();
		
		sprintf(sSql,"INSERT INTO B_STOP_MIDDLE_INFO_LOG (ORDER_SERIAL_NBR,SERV_ID,ACCT_ID,"
		"OWE_BUSINESS_TYPE_ID,ACTION,CREATED_DATE ,STATE,STATE_DATE,"
		"OWE_CHARGE,RT_CHARGE,BALANCE,CREDIT) "
		"VALUES (:OrderSerialNbr,:SERV_ID,:ACCT_ID,:OWE_BUSINESS_TYPE_ID,:ACTION,SYSDATE,'P0C',sysdate,"
		":OWE_CHARGE,:RT_CHARGE,:BALANCE,:CREDIT )");
		
		qry.close();
		qry.setSQL(sSql);
		qry.setParameter("OrderSerialNbr",OrderSerialNbr);
		qry.setParameter("SERV_ID",lServID);
		qry.setParameter("ACCT_ID",lAcctID);
		qry.setParameter("OWE_BUSINESS_TYPE_ID",iOweBusinessType);
		qry.setParameter("ACTION",0);
		qry.setParameter("OWE_CHARGE",glOweCharge);
		qry.setParameter("RT_CHARGE",glAggrCharge);
		qry.setParameter("BALANCE",glBalance);
		qry.setParameter("CREDIT",giAcctCredit);
		
		qry.execute();
	}
	
}

void Rt_Stop_Make::removeStopMiddleLog(long lServID)
{
		DEFINE_QUERY(qry);
		char sSql[2048];
		
		sprintf(sSql,"delete B_STOP_MIDDLE_INFO WHERE SERV_ID=:SERV_ID ");
		qry.close();
		qry.setSQL(sSql);
		qry.setParameter("SERV_ID",lServID);
		qry.execute();
		
		sprintf(sSql,"update B_STOP_MIDDLE_INFO_LOG SET STATE='P0P',state_date=sysdate "
		" WHERE SERV_ID=:SERV_ID AND STATE='P0C' ");
		qry.close();
		qry.setSQL(sSql);
		qry.setParameter("SERV_ID",lServID);
		qry.execute();
}

int Rt_Stop_Make::getStopMiddleLateDate(long lServID)
{
		DEFINE_QUERY(qry);
		char sSql[2048],sDate[30],sCurDate[30];
		int iDays;
		
		sprintf(sSql,"SELECT to_char(created_date,'yyyymmddhh24miss') created_date from B_STOP_MIDDLE_INFO WHERE SERV_ID=:SERV_ID ");
		qry.close();
		qry.setSQL(sSql);
		qry.setParameter("SERV_ID",lServID);
		qry.open();
		
		if (!qry.next()) return 0;
		strcpy(sDate, qry.field("created_date").asString());
		
/*
		sprintf(sSql,"SELECT to_char(sysdate,'yyyymmddhh24miss') cur_date from DUAL ");
		qry.close();
		qry.setSQL(sSql);
		qry.open();
		strcpy(sCurDate,qry.field("cur_date").asString());
*/
		Date d0;
		strcpy(sCurDate,d0.toString ());

		Date begin_date(sDate);
		Date end_date(sCurDate);
		iDays = end_date.diffStrictDay(begin_date);

		return iDays;
}

long Rt_Stop_Make::getStopMiddleCharge(long lServID)
{
		DEFINE_QUERY(qry);
		char sSql[2048],sDate[30],sCurDate[30];
		long lMiddleCharge;
		
		sprintf(sSql,"SELECT sum(nvl(owe_charge,0)+nvl(rt_charge,0)) charge from B_STOP_MIDDLE_INFO "
		" WHERE SERV_ID=:SERV_ID ");
		qry.close();
		qry.setSQL(sSql);
		qry.setParameter("SERV_ID",lServID);
		qry.open();
		
		if (!qry.next()) return 0;
		lMiddleCharge = qry.field("charge").asLong();
		return lMiddleCharge;
}

long Rt_Stop_Make::GetAcctUnionServBalance(long lServID,long lAcctID, int iBillingCycleID,int iAcctItemGroupID)
{
		DEFINE_QUERY(qry);
		char	 sSql[2000],sDate[16];
		long	lAcctBalance=0;
		BillingCycleMgr bcm;
		int     iBillingCnt;
		char sState[5];
		Date d;

		if (giBalanceCnt > 0 ) return glBalance;
		strncpy(sDate,d.toString(),8);
		sDate[8]=0;
		strcpy(sState,bcm.getPreBillingCycle(iBillingCycleID)->getState());
        if (sState[2]=='A' or sState[2]=='B' or sState[2]=='D' or sState[2]=='R') iBillingCnt = 2;
         else iBillingCnt = 1 ;
		qry.close();
		sprintf(sSql,"select nvl(sum(nvl(decode(cycle_upper,Null,balance,decode(sign(cycle_upper*%d-balance),-1,cycle_upper*%d,balance)),0) ),0) BALANCE "
			" from ACCT_BALANCE where STATE='10A' "
			" and ((ACCT_ID=:ACCT_ID AND NVL(ITEM_GROUP_ID,0)=0 AND NVL(SERV_ID,0)=0) OR (ACCT_ID=:ACCT_ID AND NVL(SERV_ID,0)=:SERV_ID)) "
			" and (exists (Select 1 From billing_cycle Where "
			" (billing_cycle_id In (Select last_billing_cycle_id From billing_cycle Where "
			" state in ('10R','10A','10B','10D') And billing_cycle_type_id=1) Or (state in ('10R','10A','10B','10D') And billing_cycle_type_id=1)) "
								" And eff_date Between cycle_begin_date And cycle_end_date-1/86400) "
			" or Exists (Select 1 From billing_cycle Where "
			" (billing_cycle_id In (Select last_billing_cycle_id From billing_cycle Where "
			" state in ('10R','10A','10B','10D') And billing_cycle_type_id=1) Or (state in ('10R','10A','10B','10D') And billing_cycle_type_id=1)) "
								" And exp_date-1/86400 Between cycle_begin_date And cycle_end_date-1/86400) "
			" or sysdate BETWEEN nvl(eff_date,sysdate-1) and nvl(exp_date-1/86400,sysdate+1) ) "
			,iBillingCnt,iBillingCnt);

			qry.setSQL(sSql);
			qry.setParameter("ACCT_ID",lAcctID);
			qry.setParameter("SERV_ID",lServID);
			qry.open();
			if(!qry.next())
				lAcctBalance= 0;
			else 
				lAcctBalance= qry.field("BALANCE").asLong();

		glBalance = lAcctBalance;
		
		giBalanceCnt++;
		
		return lAcctBalance;
}

int Rt_Stop_Make::getCredibilityUser(long lServID)
{
	DEFINE_QUERY(qry);
	char sSql[2000];
	sprintf(sSql,"SELECT 1 FROM B_CREDIBILITY_USER WHERE SERV_ID=%ld "
	" AND sysdate BETWEEN nvl(eff_date,sysdate-1) and nvl(exp_date,sysdate+1) "
	" AND ROWNUM<2 ",lServID);
	qry.close();
	qry.setSQL(sSql);
	qry.open();
	if(!qry.next())
		return 0;
	else
		return 1;
}
long Rt_Stop_Make::GetAcctUnionServBalaceByTj(long lServID,long lAcctID, int iBillingCycleID,int iAcctItemGroupID)
{
		DEFINE_QUERY(qry);
		char	 sSql[2000],sDate[16];
		long	lAcctBalance=0;
		BillingCycleMgr bcm;
		int     iBillingCnt;
		char sState[5];
		Date d;

		//if (giBalanceCnt > 0 ) return glBalance;
		strncpy(sDate,d.toString(),8);
		sDate[8]=0;
		strcpy(sState,bcm.getPreBillingCycle(iBillingCycleID)->getState());
        if (sState[2]=='A' or sState[2]=='B' or sState[2]=='D' or sState[2]=='R') iBillingCnt = 2;
         else iBillingCnt = 1 ;
		qry.close();
		sprintf(sSql,"select nvl(sum(nvl(decode(cycle_upper,Null,balance,decode(sign(cycle_upper*%d-balance),-1,cycle_upper*%d,balance)),0) ),0) BALANCE "
			" from ACCT_BALANCE where STATE='10A' "
			" and ((ACCT_ID=:ACCT_ID AND NVL(SERV_ID,0)=0) OR (ACCT_ID=:ACCT_ID AND NVL(SERV_ID,0)=:SERV_ID)) "
			" and (exists (Select 1 From billing_cycle Where "
			" (billing_cycle_id In (Select last_billing_cycle_id From billing_cycle Where "
			" state in ('10A','10B','10D') And billing_cycle_type_id=1) Or (state in ('10R','10A','10B','10D') And billing_cycle_type_id=1)) "
								" And eff_date Between cycle_begin_date And cycle_end_date-1/86400) "
			" or Exists (Select 1 From billing_cycle Where "
			" (billing_cycle_id In (Select last_billing_cycle_id From billing_cycle Where "
			" state in ('10A','10B','10D') And billing_cycle_type_id=1) Or (state in ('10R','10A','10B','10D') And billing_cycle_type_id=1)) "
								" And exp_date-1/86400 Between cycle_begin_date And cycle_end_date-1/86400) "
			" or sysdate BETWEEN nvl(eff_date,sysdate-1) and nvl(exp_date-1/86400,sysdate+1) ) "
			,iBillingCnt,iBillingCnt);

			qry.setSQL(sSql);
			qry.setParameter("ACCT_ID",lAcctID);
			qry.setParameter("SERV_ID",lServID);
			qry.open();
			if(!qry.next())
				lAcctBalance= 0;
			else 
				lAcctBalance= qry.field("BALANCE").asLong();

		glBalance = lAcctBalance;
		
		giBalanceCnt++;
		
		return lAcctBalance;
}


int Rt_Stop_Make::loadOfferDetail(long lServID,int iSourceOweBTypeID)
{
	
	DEFINE_QUERY (qry1);
	DEFINE_QUERY (qry2);
	unsigned int i= 0;
	char sSql[2048],sSql1[2048];
	struct B_STOP_OFFER tempStopOffer;
	vector<struct B_STOP_OFFER>::iterator itr;	
	long lAcctID,lTmpAcctID;	  	
	
	sprintf(sSql, "select acct_id from serv_acct where serv_id = :SERV_ID and state='10A'");
	qry1.close ();
	qry1.setSQL (sSql);
	qry1.setParameter("SERV_ID",lServID);
	qry1.open();
	if(!qry1.next())
	{
		lAcctID = 0;
		return 0;
	}	
	lAcctID = qry1.field(0).asLong();				
	m_strStopOffer.clear();	 
	#ifdef DEF_JIANGSU
	sprintf(sSql1 ,"select distinct e.instance_id,nvl(d.exclude_id,0) exclude_id,\
					 d.owe_business_type_id from product_offer_instance_detail e,\
					 (select a.product_offer_instance_id,c.offer_id,c.offer_detail_id,\
					 c.owe_business_type_id,c.exclude_id from product_offer_instance a,\
					 product_offer_instance_detail b,b_make_stop_offer c where a.product_offer_id\
					 =c.offer_id and b.instance_id= %ld and a.product_offer_instance_id=\
					 b.product_offer_instance_id and c.source_business_type_id= %d) d where\
					 e.product_offer_instance_id=d.product_offer_instance_id and e.offer_detail_id\
					 =d.offer_detail_id",lServID,iSourceOweBTypeID);
	#else
	sprintf(sSql1 ," select e.instance_id,d.owe_business_type_id\
				  from product_offer_instance_detail e,\
				 (select a.product_offer_instance_id,c.offer_detail_id,c.owe_business_type_id \
				 from product_offer_instance a,product_offer_instance_detail b,b_make_stop_offer c\
				 where a.product_offer_id=c.offer_id\
				and   b.instance_id= %ld\
				and   a.product_offer_instance_id=b.product_offer_instance_id\
				and   c.source_business_type_id= %d ) d\
				where e.product_offer_instance_id=d.product_offer_instance_id\
				and   e.offer_detail_id=d.offer_detail_id ",lServID,iSourceOweBTypeID);
	#endif
	qry2.close ();
	qry2.setSQL (sSql1);
	qry2.open ();	
		
	while(qry2.next())
	{  		
		#ifdef DEF_JIANGSU
		if(qry2.field("exclude_id").asLong()>0)	
		{	
			int flag = 0 ;	
			Date m_ptime;			
			if(qry2.field("instance_id").asLong()==lServID) return 0;			
			
			long offerId = qry2.field("exclude_id").asLong();
			long insId = qry2.field("instance_id").asLong();
			int m_iOfferInstNum = 0;	
			OfferInsQO    m_poOfferInstBuf[512];	
			char *pdate = m_ptime.toString("YYYYMMDDHHMISS");	
			
			m_iOfferInstNum = G_PUSERINFO->getAllOfferInsByID(m_poOfferInstBuf,insId,pdate);		
	  		if(m_iOfferInstNum ==0)
	  		{
	  			Log::log(0,"�û�%ldû�ж�����Ʒ��ϵ",insId);
	  			continue;
	  		}
			for(int i=1; i<=m_iOfferInstNum; i++)
			{		
		   		if( m_poOfferInstBuf[i-1].m_poOfferIns->m_iOfferID ==offerId)		
				{	
					flag =1 ;
					break;									
		       	}       			
			}	
			if(flag ==0)
			{
				Log::log(0,"���û�%ld������ϵ����",insId);	
			}
			continue;
		}	
		#endif			
		tempStopOffer.lServID = qry2.field("instance_id").asLong();
		tempStopOffer.iOweBTypeID =qry2.field("owe_business_type_id").asInteger();														
		
		qry1.close ();
		qry1.setSQL (sSql);
		qry1.setParameter("SERV_ID",tempStopOffer.lServID);
		qry1.open();	
		if(!qry1.next())continue;	
		lTmpAcctID = qry1.field(0).asLong();		
		if (lAcctID != lTmpAcctID) continue;	
		int tag = 0;
		for(itr=m_strStopOffer.begin();itr!=m_strStopOffer.end();itr++)
		{
			if(tempStopOffer.lServID ==(*itr).lServID)
			{	
				tag =1;
			}						
		}
		if(tag ==0)
		{	
			m_strStopOffer.push_back(tempStopOffer);	
		}
	}	
	qry1.close();
	qry2.close();
	return 0;
}

long Rt_Stop_Make::GetCurChargeByMemoryZz(long lAcctID,long lServID,int iBillingCycleID,int iAcctItemTypeID)
{
    long lCharge=0;
    int  iServCharge=0;
    AcctItemAccuMgr m;
    TOCIQuery qry1(&gpDBLink_STOP);
    long lSubAcctID,lSubServID;
    int iBillingModeID;
    char sql[2000];
    Serv ServInfo;
    Date d;

    //iBillingModeID = atoi(G_PSERV->getAttr (ID_BILLING_MODE));

    if (!(G_PUSERINFO->getServ (ServInfo, lServID, d.toString()))) {
        //�û�����û���ҵ�
        Log::log (91, "û���ҵ�SERVID:%ld %s", lServID, d.toString());
        return 0;
	}
#ifndef DEF_SHANDONG
    char sTempBillingMode[20],*pTempBillingMode;
    pTempBillingMode=ServInfo.getAttr (ID_BILLING_MODE);
    if (pTempBillingMode == NULL)
    {
       Log::log(0,"SERV_ID:%ld,�Ҳ���Ԥ�󸶷ѱ�־!",lServID);   
       strcpy(sTempBillingMode,"99");
    }
    else strcpy(sTempBillingMode,pTempBillingMode);

    iBillingModeID = atoi(sTempBillingMode);

    if (iBillingModeID==0)
        iBillingModeID = ServInfo.getServInfo ()->m_iBillingMode;

    if (iBillingModeID!=2) return 0;
#endif

    //���ڴ�ȡ
    if (lAcctID == -1)
    {
        iServCharge = m.get(lServID,iBillingCycleID,iAcctItemTypeID);
        lCharge = iServCharge;
    }
    else
    {
        qry1.close();
        qry1.setSQL(" select acct_id,serv_id  from serv_acct "
                    " where acct_id=:v_acct_id and state='10A' ");
        qry1.setParameter("v_acct_id",lAcctID);
        qry1.open();
        while (qry1.next())
        {
            lSubAcctID = qry1.field(0).asLong();
            lSubServID = qry1.field(1).asLong();

            iServCharge = m.get(lSubServID,iBillingCycleID,0);
            lCharge = lCharge + iServCharge;
        }
    }
    glAggrCharge = lCharge;
    return lCharge;
}

long	Rt_Stop_Make::GetPreAggrChargeCommZz(long lServID,long lAcctID,int iBillingCycleID,int iType,int iOrgID)
{
		//DEFINE_QUERY(qry);
		TOCIQuery qry(&gpDBLink_STOP);
    BillingCycleMgr bcm;
    char sState[5];
		char	sSql[2000];
		long	lCharge=0;
		int iPreBillingCycleID;
				
    strcpy(sState,bcm.getPreBillingCycle(iBillingCycleID)->getState());
    if (sState[2]=='R' || sState[2]=='A' || sState[2]=='B' )
    {
    	  iPreBillingCycleID = bcm.getPreBillingCycle (iBillingCycleID)->getBillingCycleID ();
    		switch (iType){
    		case 2:
    			sprintf(sSql,"SELECT NVL(SUM(NVL(CHARGE,0)),0) CHARGE FROM %s WHERE SERV_ID=:SERV_ID AND BILLING_CYCLE_ID=:BILLING_CYCLE_ID "
    			" AND NVL(FREE_FLAG,0) = 0 "
    			" AND acct_item_type_id in (select acct_item_type_id from A_INC_ACCT_ITEM_TYPE where org_id in (1,:ORG_ID) and state='10A') ",gsPreTableName);
    			qry.close();
    			qry.setSQL(sSql);
    			qry.setParameter("SERV_ID",lServID);
    			qry.setParameter("BILLING_CYCLE_ID",iPreBillingCycleID);
    			qry.setParameter("ORG_ID",iOrgID);
    			qry.open();
    			if(!qry.next())
    				lCharge= 0;
    			else 
    				lCharge= qry.field("CHARGE").asLong();
    			break;
    		case 6:
    			sprintf(sSql,"SELECT NVL(SUM(NVL(CHARGE,0)),0) CHARGE FROM %s WHERE ACCT_ID=:ACCT_ID AND BILLING_CYCLE_ID=:BILLING_CYCLE_ID "
    			" AND NVL(FREE_FLAG,0) = 0 "
    			" AND acct_item_type_id in (select acct_item_type_id from A_INC_ACCT_ITEM_TYPE where org_id in (1,:ORG_ID) and state='10A') ",gsPreTableName);
    			qry.close();
    			qry.setSQL(sSql);
    			qry.setParameter("ACCT_ID",lAcctID);
    			qry.setParameter("BILLING_CYCLE_ID",iPreBillingCycleID);
    			qry.setParameter("ORG_ID",iOrgID);
    			qry.open();
    			if(!qry.next())
    				lCharge= 0;
    			else 
    				lCharge= qry.field("CHARGE").asLong();
    			break;
    		default:
    			lCharge = 0;
    			break;
    		}
    }		
		glAggrCharge = glAggrCharge+lCharge;
		giAggrCnt++;
		return lCharge;
}
int  Rt_Stop_Make::DealWithHighFee(long lServID,int iOweBTypeID,char *state)	
{	
	DEFINE_QUERY (qry);
	char sSql[800]={0};
	long  OrderSerialNbr = GetSerialNbr();
	int iStaffID = DEF_STAFF_ID;
	InsertOweDun(lServID, iOweBTypeID);
	
	sprintf(sSql,"INSERT INTO a_work_order_prep (ORDER_PRE_SERIAL_NBR,\
			OWE_BUSINESS_TYPE_ID,STAFF_ID,SERV_ID,CREATED_DATE ,COMPLETED_DATE,STATE,STATE_DATE,\
			PROCESS_COUNT,PRE_SERV_STATE,REMARK,CHARGE,BALANCE,CREDIT,OWE_TASK_ITEM_ID,SERV_ITME_BALANCE,\
			ACCT_ITME_BALANCE) VALUES (ORDER_PRE_SERIAL_NBR.NEXTVAL,:OWE_BUSINESS_TYPE_ID,:STAFF_ID,:SERV_ID,\
			SYSDATE,SYSDATE+(%d/1440),'PSA',SYSDATE,NULL,:PRE_SERV_STATE,NULL,:CHARGE,:BALANCE,:CREDIT,\
			:OWE_TASK_ITEM_ID,:SERV_ITME_BALANCE,:ACCT_ITME_BALANCE)",STOP_DELAY_TIME );		
	qry.close();
	qry.setSQL(sSql);
	qry.setParameter("OWE_BUSINESS_TYPE_ID",iOweBTypeID);
	qry.setParameter("STAFF_ID",iStaffID);
	qry.setParameter("SERV_ID",lServID);
	qry.setParameter("PRE_SERV_STATE",state);
	qry.setParameter("CHARGE",glOweCharge+glAggrCharge);
	qry.setParameter("BALANCE",glBalance);  
	qry.setParameter("CREDIT",giAcctCredit);
	qry.setParameter("OWE_TASK_ITEM_ID",0);
	qry.setParameter("SERV_ITME_BALANCE",lservResItemBalance); 
	qry.setParameter("ACCT_ITME_BALANCE",lacctResItemBalance);
	qry.execute();
	qry.close();
	
	memset(sSql,0,sizeof(sSql));
	sprintf(sSql,"INSERT INTO A_WORK_ORDER_LOG (ORDER_SERIAL_NBR,OWE_BUSINESS_TYPE_ID,STAFF_ID,SERV_ID,CREATED_DATE ,COMPLETED_DATE,STATE,STATE_DATE,PROCESS_COUNT,PRE_SERV_STATE,REMARK,CHARGE,BALANCE,CREDIT,OWE_TASK_ITEM_ID,SERV_ITME_BALANCE,ACCT_ITME_BALANCE) VALUES (:OrderSerialNbr,:OWE_BUSINESS_TYPE_ID,:STAFF_ID,:SERV_ID,SYSDATE,NULL,'PSA',SYSDATE,NULL,:PRE_SERV_STATE,'�߶��ӻ�ͣ��',:CHARGE,:BALANCE,:CREDIT,:OWE_TASK_ITEM_ID,:SERV_ITME_BALANCE,:ACCT_ITME_BALANCE )");
	
	qry.close();
	qry.setSQL(sSql);
	qry.setParameter("OrderSerialNbr",OrderSerialNbr);
	qry.setParameter("OWE_BUSINESS_TYPE_ID",iOweBTypeID);
	qry.setParameter("STAFF_ID",iStaffID);
	qry.setParameter("SERV_ID",lServID);
	qry.setParameter("PRE_SERV_STATE",state);
	qry.setParameter("CHARGE",glOweCharge+glAggrCharge);
	qry.setParameter("BALANCE",glBalance); 
	qry.setParameter("CREDIT",giAcctCredit);
	qry.setParameter("OWE_TASK_ITEM_ID",0);
	qry.setParameter("SERV_ITME_BALANCE",lservResItemBalance);
	qry.setParameter("ACCT_ITME_BALANCE",lservResItemBalance);
	qry.execute();
	
	return 1;
}
bool Rt_Stop_Make::checkTime(bool &isFirst)
{
    static char sTime[4][6];
    char m_sTime[15];
    Date dCurr;
    dCurr.getTimeString(m_sTime);

    if(isFirst){
        snprintf(sTime[0], 5, "%04d", REMIND_TIME1_MIN_VALUE);
        snprintf(sTime[1], 5, "%04d", REMIND_TIME1_MAX_VALUE);
        snprintf(sTime[2], 5, "%04d", REMIND_TIME2_MIN_VALUE);
        snprintf(sTime[3], 5, "%04d", REMIND_TIME2_MAX_VALUE);
        isFirst = false;
    }
    if(((strcmp(&m_sTime[8], sTime[0]) >= 0)
        &&(strcmp(&m_sTime[8], sTime[1]) <= 0))
        ||((strcmp(&m_sTime[8], sTime[2]) >= 0)
        &&(strcmp(&m_sTime[8], sTime[3]) <= 0)))
        {    
        	oneSleepValidTime = 0;    	
        	return true;
        }
    else
    {
    	oneSleepValidTime = 1;
        return false;
	}
	
}

