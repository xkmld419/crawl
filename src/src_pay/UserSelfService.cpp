#include "UserSelfService.h"
#include "UserSelfServiceSql.h"
#include "PaymentInfoSql.h"

#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "errorcode_pay.h"


static vector<R_PayAbilityQuerySvc *> g_vRPayAbilityQuerySvc;
static vector<R_PayAbilityQuerySvc *>::iterator g_itr1;

static vector<R_MicropaymentInfo *> g_vRMicropaymentInfo;
static vector<R_MicropaymentInfo *>::iterator g_itr2;
	
extern bool g_toPlatformFlag;

PayAbilityQuery::PayAbilityQuery()
{
		m_poSql = NULL;
		m_poUnpack = NULL;
		m_poPack = NULL;
		m_dccOperation = NULL ;
}

PayAbilityQuery::~PayAbilityQuery()
{
	if (m_poSql != NULL)
		delete m_poSql;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int PayAbilityQuery::init(ABMException &oExp)
{
	if ((m_poSql = new PayAbilityQuerySql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PayAbilityQuery::init malloc failed!");
		return -1;
	}
	
	if ((m_dccOperation = new DccOperation) == NULL)
	{
		ADD_EXCEPT0(oExp, "DccOperation::init malloc failed!");
		return -1;
	}
	
	return 0;
}

int PayAbilityQuery::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>���ԣ�PayAbilityQuery::deal �������ʼ");
	
	int iRet=0;
	int iSize=0;

	g_toPlatformFlag = true;
	
	m_poUnpack = (PayAbilityQueryCCR *)pCCR;
	m_poPack = (PayAbilityQueryCCA *)pCCA;
	
	m_poPack->clear();
	g_vRPayAbilityQuerySvc.clear();
	g_vRMicropaymentInfo.clear();
	
	A_PayAbilityQuerySvc oSvc={0} ;

	// ���յ���DCC������Ϣͷ��TT
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	try 
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[��������-ReverseDeductBalanc]:��ѯSEQʧ��!");
			throw GET_DCC_INFO_RECORD_SEQ_ERR;	
		}
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "֧������״̬��ѯ,����CCR��,��Ϣͷ��TT!");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������յ�CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_RECEVIE_DCC_CCR_ERR;
		}
		
		__DEBUG_LOG0_(0, "֧������״̬��ѯ,CCR��Ϣ�����!");
		//��Ϣ��� - ��һ��
		if ((iSize=m_poUnpack->getR_PayAbilityQuerySvc(g_vRPayAbilityQuerySvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "֧������״̬��ѯ::deal ���������Ϣ��ΪNULL");
			m_poPack->setRltCode(ECODE_NOMSG);
			throw DEDUCT_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "֧������״̬��ѯ::deal ��Ϣ�������!");
			m_poPack->setRltCode(ECODE_UNPACK_FAIL);
			throw DEDUCT_UNPACK_ERR;
		}
	
		
		// �����һ����Ϣ
		for (g_itr1=g_vRPayAbilityQuerySvc.begin(); g_itr1!=g_vRPayAbilityQuerySvc.end(); ++g_itr1)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack->getR_MicropaymentInfo(*g_itr1,g_vRMicropaymentInfo)) == 0 )
			{
				__DEBUG_LOG0_(0, "PayAbilityQuery::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack->setRltCode(ECODE_NOMSG);
				throw DEDUCT_UNPACK_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "PayAbilityQuery::deal ��Ϣ�������!");
				m_poPack->setRltCode(ECODE_UNPACK_FAIL);
				throw DEDUCT_UNPACK_ERR;
			}
			 
			__DEBUG_LOG0_(0, "����:�����һ����Ϣ........");	

			iRet = _deal(*g_itr1,g_vRMicropaymentInfo);

			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "֧������״̬��ѯ����ʧ��");
				throw iRet;
			}
		}
		//return iRet;
	}
	catch(TTStatus oSt)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "PayAbilityQuery::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet=value;
		__DEBUG_LOG1_(0, "֧������״̬��ѯʧ��:: ʧ����Ϣ value=%d", value);
	}
	if(iRet != 0)
	{
		// �򷵻�CCA��,���ش�����
		oSvc.m_svcResultCode=iRet;
		if (!m_poPack->addResPayAbilityQuerySvc(oSvc))
		{
			__DEBUG_LOG0_(0, "AllocateBalance:: _deal m_poPack ���ʧ��!");
			m_poPack->clear();
			m_poPack->setRltCode(ECODE_PACK_FAIL);
		}
		__DEBUG_LOG0_(0, "֧������״̬��ѯʧ��,iRet=[%d]",oSvc.m_svcResultCode);
		
	}
	return iRet;
}


int PayAbilityQuery::_deal(R_PayAbilityQuerySvc *pSvcIn,vector<R_MicropaymentInfo *> g_vRMicropaymentInfo)
{
	
	ABMException oExp;
		
	int iRet=0;
	time_t tTime;
	char sResponseTime[32]={0};
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================֧��״̬��ѯ=======================");
	__DEBUG_LOG0_(0, "=====>start,ȡ��һ��CCR������Ϣ:");
	
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:%s",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime��%s",pSvcIn->m_requestTime);


	UserSelfService pUserSelfService={0};
	
	// ����CCR��Ϣ׼��	
	strcpy(pUserSelfService.m_requestId,pSvcIn->m_requestId);		// ������ˮ��
	strcpy(pUserSelfService.m_requestTime,pSvcIn->m_requestTime);	// ����ʱ��

			
	A_PayAbilityQuerySvc oSvc={0} ;	// CCA ��Ӧ������Ϣ
	A_MicropaymentInfo oData={0} ;	// CCA ��Ӧ�۷���Ŀ��ϸ��Ϣ
	vector<struct A_MicropaymentInfo> vMicropaymentInfo;
		
	// ����CCR�е��û�����ͺ������Ͳ�ѯ�û����ʻ�������Ϣ
	ServAcctInfo oServAcct={0};

	
	try
	{	

		for (g_itr2=g_vRMicropaymentInfo.begin(); g_itr2!=g_vRMicropaymentInfo.end(); ++g_itr2)
		{	
			strcpy(pUserSelfService.m_servNbr,(*g_itr2)->m_servNbr);		//�û�����
			__DEBUG_LOG1_(0, "m_servNbr:%s", (*g_itr2)->m_servNbr);
			
			pUserSelfService.m_nbrType =(*g_itr2)->m_nbrType;		//��������
			__DEBUG_LOG1_(0, "m_nbrType:%d",(*g_itr2)->m_nbrType);
				
			pUserSelfService.m_servAttr = (*g_itr2)->m_servAttr;		//�û�����
			__DEBUG_LOG1_(0, "m_servAttr:%d",(*g_itr2)->m_servAttr);
			
			strcpy(pUserSelfService.m_sServicePlatformID,(*g_itr2)->m_sServicePlatformID);//ҵ��ƽ̨ID
			__DEBUG_LOG1_(0, "m_sServicePlatformID:%s",(*g_itr2)->m_sServicePlatformID);
		
			
		
			// ����ҵ���߼�����
			iRet = m_poSql->PayAbilityStatusInfo(pUserSelfService,oExp);
			
			tTime = time(NULL);
			tTime += 2209017599;
			long2date(tTime,sResponseTime);
			strcpy(oSvc.m_responseTime,sResponseTime);
		
			strcpy(oSvc.m_responseId,pUserSelfService.m_requestId);
			strcpy(oData.m_PayStatus,pUserSelfService.m_payStatus);
			
			__DEBUG_LOG1_(0, "m_responseTime:%s",oSvc.m_responseTime);
			__DEBUG_LOG1_(0, "m_responseId:%s",oSvc.m_responseId);
			__DEBUG_LOG1_(0, "m_PayStatus:%s",oData.m_PayStatus);
			
			vMicropaymentInfo.push_back(oData);
		}
		
		// ��� - ��һ�������Ϣ
		if (!m_poPack->addResPayAbilityQuerySvc(oSvc))
		{
			__DEBUG_LOG0_(0, "PayAbilityQuery:: _deal m_poPack ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_PACK_ERR;
		}
		// ���ԣ������oSvc����
		__DEBUG_LOG1_(0, "��һ�����CCA������,oSvc,responseId:[%s]",oSvc.m_responseId);
		
		// ��� - �ڶ���۷���ϸ
		for(int j=0;j<vMicropaymentInfo.size();j++)
		{
			if (!m_poPack->addResMicropaymentInfo(vMicropaymentInfo[j]))
			{
				__DEBUG_LOG0_(0, "PayAbilityQuery:: _deal m_poPack ���ʧ��!");
				m_poPack->setRltCode(ECODE_PACK_FAIL);
				throw DEDUCT_PACK_ERR;
			}
		}
		__DEBUG_LOG0_(0, "�ڶ������CCA������.");
	}
	catch(ABMException &oExp)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG0_(0, "ABMException....");
	}
	catch(int &value ) 
	{

		iRet=value;
		oSvc.m_svcResultCode=iRet;
		__DEBUG_LOG1_(0, "CCA,ҵ�񼶵Ľ������iRet:[%d]",iRet);
	}

	return iRet;	
}

 
 
static vector<R_BalanceTransSvc *> g_vRBalanceTransSvc;
static vector<R_BalanceTransSvc *>::iterator g_itrBT1;

static vector<R_BTMicropaymentInfo *> g_vRBTMicropaymentInfo;
static vector<R_BTMicropaymentInfo *>::iterator g_itrBT2;


BalanceTransQuery::BalanceTransQuery()
{
		m_poSql = NULL;
		m_poUnpack1 = NULL;
		m_poPack1 = NULL;
		m_dccOperation = NULL;
}

BalanceTransQuery::~BalanceTransQuery()
{
	if (m_poSql != NULL)
		delete m_poSql;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int BalanceTransQuery::init(ABMException &oExp)
{
	if ((m_poSql = new BalanceTransQuerySql) == NULL)
	{
		ADD_EXCEPT0(oExp, "BalanceTransQuery::init malloc failed!");
		return -1;
	}
	if ((m_dccOperation = new DccOperation) == NULL)
	{
		ADD_EXCEPT0(oExp, "DccOperation::init malloc failed!");
		return -1;
	}
	return 0;
}

int BalanceTransQuery::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>���ԣ�BalanceTransQuery::deal �������ʼ");
	
	int iRet=0;
	int iSize=0;

	m_poUnpack1 = (BalanceTransCCR *)pCCR;
	m_poPack1 = (BalanceTransCCA *)pCCA;
	
	m_poPack1->clear();
	g_vRBalanceTransSvc.clear();
	g_vRBTMicropaymentInfo.clear();
	
	A_BalanceTransSvc oSvc={0} ;
	
	g_toPlatformFlag = true;
	// ���յ���DCC������Ϣͷ��TT
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	try 
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[��������]:��ѯSEQʧ��!");
			throw GET_DCC_INFO_RECORD_SEQ_ERR;	
		}
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "���������ѯ,����CCR��,��Ϣͷ��TT!");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack1->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������յ�CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_RECEVIE_DCC_CCR_ERR;
		}
		
		__DEBUG_LOG0_(0, "���������ѯ,CCR��Ϣ�����!");
		//��Ϣ��� - ��һ��
		if ((iSize=m_poUnpack1->getBalanceTransSvc(g_vRBalanceTransSvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "���������ѯ::deal ���������Ϣ��ΪNULL");
			m_poPack1->setRltCode(ECODE_NOMSG);
			throw DEDUCT_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "���������ѯ::deal ��Ϣ�������!");
			m_poPack1->setRltCode(ECODE_UNPACK_FAIL);
			throw DEDUCT_UNPACK_ERR;
		}
	
		
		// �����һ����Ϣ
		for (g_itrBT1=g_vRBalanceTransSvc.begin(); g_itrBT1!=g_vRBalanceTransSvc.end(); ++g_itrBT1)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack1->getBTMicropaymentInfo(*g_itrBT1,g_vRBTMicropaymentInfo)) == 0 )
			{
				__DEBUG_LOG0_(0, "BalanceTransQuery::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack1->setRltCode(ECODE_NOMSG);
				throw DEDUCT_UNPACK_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "BalanceTransQuery::deal ��Ϣ�������!");
				m_poPack1->setRltCode(ECODE_UNPACK_FAIL);
				throw DEDUCT_UNPACK_ERR;
			}
			 
			__DEBUG_LOG1_(0, "����:�����һ����Ϣ........iSize:%d",iSize);	

			iRet = _deal(*g_itrBT1,g_vRBTMicropaymentInfo);

			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "���������ѯ����ʧ��");
				throw iRet;
			}
		}
		//return iRet;
	}
	catch(TTStatus oSt)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "BalanceTransQuery::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet=value;
		__DEBUG_LOG1_(0, "���������ѯʧ��:: ʧ����Ϣ value=%d", value);
	}
	if(iRet != 0)
	{
		// �򷵻�CCA��,���ش�����
		oSvc.m_svcResultCode=iRet;
		if (!m_poPack1->addResBalanceTransSvc(oSvc))
		{
			__DEBUG_LOG0_(0, "BalanceTransQuery:: _deal m_poPack ���ʧ��!");
			m_poPack1->clear();
			m_poPack1->setRltCode(ECODE_PACK_FAIL);
		}
		__DEBUG_LOG0_(0, "���������ѯʧ��,iRet=[%d]",oSvc.m_svcResultCode);
		
	}
	return iRet;
}


int BalanceTransQuery::_deal(R_BalanceTransSvc *pSvcIn,vector<R_BTMicropaymentInfo *> g_vRBTMicropaymentInfo)
{
	
	ABMException oExp;
		
	int iRet=0;
	time_t tTime;
	char sResponseTime[32]={0};
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================���������ѯ=======================");
	__DEBUG_LOG0_(0, "=====>start,ȡ��һ��CCR������Ϣ:");
	
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:%s",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime��%s",pSvcIn->m_requestTime);


	UserSelfService pUserSelfService={0};
	
	// ����CCR��Ϣ׼��	
	strcpy(pUserSelfService.m_requestId,pSvcIn->m_requestId);		// ������ˮ��
	strcpy(pUserSelfService.m_requestTime,pSvcIn->m_requestTime);	// ����ʱ��

			
	A_BalanceTransSvc oSvc={0} ;	// CCA ��Ӧ������Ϣ
	A_BTMicropaymentInfo oData={0} ;	// CCA ��Ӧ�۷���Ŀ��ϸ��Ϣ
	vector<struct A_BTMicropaymentInfo> vMicropaymentInfo;
		
	// ����CCR�е��û�����ͺ������Ͳ�ѯ�û����ʻ�������Ϣ
	ServAcctInfo oServAcct={0};

	
	try
	{	

		for (g_itrBT2=g_vRBTMicropaymentInfo.begin(); g_itrBT2!=g_vRBTMicropaymentInfo.end(); ++g_itrBT2)
		{	
			strcpy(pUserSelfService.m_servNbr,(*g_itrBT2)->m_servNbr);		//�û�����
			__DEBUG_LOG1_(0, "m_servNbr:%s", (*g_itrBT2)->m_servNbr);
			
			pUserSelfService.m_nbrType =(*g_itrBT2)->m_nbrType;		//��������
			__DEBUG_LOG1_(0, "m_nbrType:%d",(*g_itrBT2)->m_nbrType);
				
			pUserSelfService.m_servAttr = (*g_itrBT2)->m_servAttr;		//�û�����
			__DEBUG_LOG1_(0, "m_servAttr:%d",(*g_itrBT2)->m_servAttr);
			
			strcpy(pUserSelfService.m_sServicePlatformID,(*g_itrBT2)->m_sServicePlatformID);//ҵ��ƽ̨ID
			__DEBUG_LOG1_(0, "m_sServicePlatformID:%s",(*g_itrBT2)->m_sServicePlatformID);
		
			
		
			// ����ҵ���߼�����
			iRet = m_poSql->BalanceTransInfo(pUserSelfService,oExp);
			
			tTime = time(NULL);
			tTime += 2209017599;
			long2date(tTime,sResponseTime);
			strcpy(oSvc.m_responseTime,sResponseTime);
		
			strcpy(oSvc.m_responseId,pUserSelfService.m_requestId);
			
			oData.m_dateTransFreq=pUserSelfService.m_dateTransFreq;
			oData.m_transLimit=pUserSelfService.m_transLimit;
			oData.m_monthTransLimit=pUserSelfService.m_monthTransLimit;
			oData.m_autotransflag=pUserSelfService.m_autotransflag;
			oData.m_autobalancelimit=pUserSelfService.m_autobalancelimit;
			oData.m_autoAmountlimit=pUserSelfService.m_autoAmountlimit;
			
			__DEBUG_LOG1_(0, "m_responseTime:%s",oSvc.m_responseTime);
			__DEBUG_LOG1_(0, "m_responseId:%s",oSvc.m_responseId);
			
			__DEBUG_LOG1_(0, "m_dateTransFreq:%d",oData.m_dateTransFreq);
			__DEBUG_LOG1_(0, "m_transLimit:%d",oData.m_transLimit);
			__DEBUG_LOG1_(0, "m_month_TransLimit:%d",oData.m_monthTransLimit);
			__DEBUG_LOG1_(0, "m_auto_transflag:%d",oData.m_autotransflag);
			__DEBUG_LOG1_(0, "m_autobalancelimit:%d",oData.m_autobalancelimit);
			__DEBUG_LOG1_(0, "m_autoAmountlimit:%d",oData.m_autoAmountlimit);
			
			vMicropaymentInfo.push_back(oData);
		}
		
		// ��� - ��һ�������Ϣ
		if (!m_poPack1->addResBalanceTransSvc(oSvc))
		{
			__DEBUG_LOG0_(0, "PayAbilityQuery:: _deal m_poPack ���ʧ��!");
			m_poPack1->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_PACK_ERR;
		}
		// ���ԣ������oSvc����
		__DEBUG_LOG1_(0, "��һ�����CCA������,oSvc,responseId:[%s]",oSvc.m_responseId);
		
		// ��� - �ڶ���۷���ϸ
		for(int j=0;j<vMicropaymentInfo.size();j++)
		{
			if (!m_poPack1->addResBTMicropaymentInfo(vMicropaymentInfo[j]))
			{
				__DEBUG_LOG0_(0, "BalanceTransQuery:: _deal m_poPack ���ʧ��!");
				m_poPack1->setRltCode(ECODE_PACK_FAIL);
				throw DEDUCT_PACK_ERR;
			}
		}
		__DEBUG_LOG0_(0, "�ڶ������CCA������.");
	}
	catch(ABMException &oExp)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG0_(0, "ABMException....");
	}
	catch(int &value ) 
	{

		iRet=value;
		oSvc.m_svcResultCode=iRet;
		__DEBUG_LOG1_(0, "CCA,ҵ�񼶵Ľ������iRet:[%d]",iRet);
	}

	return iRet;	
}


static vector<R_BalanceTransResetSvc *> g_vRBalanceTransResetSvc;
static vector<R_BalanceTransResetSvc *>::iterator g_itrTR1;

static vector<R_TRMicropaymentInfo *> g_vRTRMicropaymentInfo;
static vector<R_TRMicropaymentInfo *>::iterator g_itrTR2;


BalanceTransReset::BalanceTransReset()
{
		m_poSql = NULL;
		m_poSqlPassBTR = NULL;
		m_poSqlRules = NULL;
		m_poUnpack1 = NULL;
		m_poPack1 = NULL;
		m_dccOperation = NULL;
		m_poSqlOpt = NULL;
}

BalanceTransReset::~BalanceTransReset()
{
	if (m_poSql != NULL)
		delete m_poSql;
	if (m_poSqlPassBTR != NULL)
		delete m_poSqlPassBTR;
	if (m_poSqlRules != NULL)
		delete m_poSqlRules;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
	if (m_poSqlOpt != NULL)
		delete m_poSqlOpt;	
}

int BalanceTransReset::init(ABMException &oExp)
{
	if ((m_poSql = new BalanceTransResetSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "BalanceTransReset::init malloc failed!");
		return -1;
	}
	
	if ((m_poSqlPassBTR = new PasswordResetSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PasswordResetSql::init malloc failed!");
		return -1;
	}
	if ((m_poSqlRules = new TransferRulesSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "TransferRulesSql::init malloc failed!");
		return -1;
	}
	
	if ((m_dccOperation = new DccOperation) == NULL)
	{
		ADD_EXCEPT0(oExp, "DccOperation::init malloc failed!");
		return -1;
	}
	
	if ((m_poSqlOpt = new PaymentInfoSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PaymentInfoSql::init malloc failed!");
		return -1;
	}
	return 0;
}

int BalanceTransReset::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>���ԣ�BalanceTransReset::deal �������ʼ");
	
	int iRet=0;
	int iSize=0;

	m_poUnpack1 = (BalanceTransResetCCR *)pCCR;
	m_poPack1 = (BalanceTransResetCCA *)pCCA;
	
	m_poPack1->clear();
	g_vRBalanceTransResetSvc.clear();
	g_vRTRMicropaymentInfo.clear();
	
	A_BalanceTransResetSvc oSvc={0} ;
	
	g_toPlatformFlag = true;
	// ���յ���DCC������Ϣͷ��TT
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	try 
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[��������]:��ѯSEQʧ��!");
			throw GET_DCC_INFO_RECORD_SEQ_ERR;	
		}
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "������������,����CCR��,��Ϣͷ��TT!");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack1->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������յ�CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_RECEVIE_DCC_CCR_ERR;
		}
		
		__DEBUG_LOG0_(0, "������������,CCR��Ϣ�����!");
		//��Ϣ��� - ��һ��
		if ((iSize=m_poUnpack1->getBalanceTransResetSvc(g_vRBalanceTransResetSvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "������������::deal ���������Ϣ��ΪNULL");
			m_poPack1->setRltCode(ECODE_NOMSG);
			throw DEDUCT_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "������������::deal ��Ϣ�������!");
			m_poPack1->setRltCode(ECODE_UNPACK_FAIL);
			throw DEDUCT_UNPACK_ERR;
		}
	
		
		// �����һ����Ϣ
		for (g_itrTR1=g_vRBalanceTransResetSvc.begin(); g_itrTR1!=g_vRBalanceTransResetSvc.end(); ++g_itrTR1)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack1->getTRMicropaymentInfo(*g_itrTR1,g_vRTRMicropaymentInfo)) == 0 )
			{
				__DEBUG_LOG0_(0, "BalanceTransReset::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack1->setRltCode(ECODE_NOMSG);
				throw DEDUCT_UNPACK_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "BalanceTransReset::deal ��Ϣ�������!");
				m_poPack1->setRltCode(ECODE_UNPACK_FAIL);
				throw DEDUCT_UNPACK_ERR;
			}
			 
			__DEBUG_LOG1_(0, "����:�����һ����Ϣ........iSize:%d",iSize);	

			iRet = _deal(*g_itrTR1,g_vRTRMicropaymentInfo);

			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "�����������ò���ʧ��");
				throw iRet;
			}
		}
		//return iRet;
	}
	catch(TTStatus oSt)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "BalanceTransReset::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet=value;
		__DEBUG_LOG1_(0, "������������ʧ��:: ʧ����Ϣ value=%d", value);
	}
	if(iRet != 0)
	{
		// �򷵻�CCA��,���ش�����
		oSvc.m_svcResultCode=iRet;
		if (!m_poPack1->addBalanceTransResetSvc(oSvc))
		{
			__DEBUG_LOG0_(0, "BalanceTransReset:: _deal m_poPack ���ʧ��!");
			m_poPack1->clear();
			m_poPack1->setRltCode(ECODE_PACK_FAIL);
		}
		__DEBUG_LOG0_(0, "������������ʧ��,iRet=[%d]",oSvc.m_svcResultCode);
		
	}
	return iRet;
}


int BalanceTransReset::_deal(R_BalanceTransResetSvc *pSvcIn,vector<R_TRMicropaymentInfo *> g_vRTRMicropaymentInfo)
{
	
	ABMException oExp;
		
	int iRet=0;
	time_t tTime;
	char sResponseTime[32]={0};
	char sTmp[32];
	bool bState=false;
	memset(sTmp,0,sizeof(sTmp));
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================������������=======================");
	__DEBUG_LOG0_(0, "=====>start,ȡ��һ��CCR������Ϣ:");
	
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:%s",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime��%s",pSvcIn->m_requestTime);


	UserSelfService pUserSelfService={0};
	StructPaymentSeqInfo  sStructPaymentSeqInfo={0};
	PaymentInfoALLCCR pPaymentInfoCCR={0};
	
	// ����CCR��Ϣ׼��	
	strcpy(pUserSelfService.m_requestId,pSvcIn->m_requestId);		// ������ˮ��
	strcpy(pUserSelfService.m_requestTime,pSvcIn->m_requestTime);	// ����ʱ��

			
	A_BalanceTransResetSvc oSvc={0} ;	// CCA ��Ӧ������Ϣ
	A_TRMicropaymentInfo oData={0} ;	// CCA ��Ӧ�۷���Ŀ��ϸ��Ϣ
	vector<struct A_TRMicropaymentInfo> vMicropaymentInfo;
		
	// ����CCR�е��û�����ͺ������Ͳ�ѯ�û����ʻ�������Ϣ
	ServAcctInfo oServAcct={0};

	
	try
	{	

		for (g_itrTR2=g_vRTRMicropaymentInfo.begin(); g_itrTR2!=g_vRTRMicropaymentInfo.end(); ++g_itrTR2)
		{	
			__DEBUG_LOG0_(0, "---------CCR���£�");
			strcpy(pUserSelfService.m_servNbr,(*g_itrTR2)->m_servNbr);		//�û�����
			__DEBUG_LOG1_(0, "m_servNbr:%s", (*g_itrTR2)->m_servNbr);
			
			pUserSelfService.m_nbrType =(*g_itrTR2)->m_nbrType;		//��������
			__DEBUG_LOG1_(0, "m_nbrType:%d",(*g_itrTR2)->m_nbrType);
				
			pUserSelfService.m_servAttr = (*g_itrTR2)->m_servAttr;		//�û�����
			__DEBUG_LOG1_(0, "m_servAttr:%d",(*g_itrTR2)->m_servAttr);
			
			strcpy(pUserSelfService.m_sServicePlatformID,(*g_itrTR2)->m_sServicePlatformID);//ҵ��ƽ̨ID
			__DEBUG_LOG1_(0, "m_sServicePlatformID:%s",(*g_itrTR2)->m_sServicePlatformID);
			
			strcpy(sTmp,(*g_itrTR2)->m_payPassword);//֧������
			__DEBUG_LOG1_(0, "m_payPassword:%s",(*g_itrTR2)->m_payPassword);
			
			pUserSelfService.m_dateTransFreq=(*g_itrTR2)->m_dateTransFreq;//ÿ�컮��Ƶ�ȣ���λ�Σ�
			__DEBUG_LOG1_(0, "m_dateTransFreq:%d",(*g_itrTR2)->m_dateTransFreq);
			
			pUserSelfService.m_transLimit=(*g_itrTR2)->m_transLimit;//ÿ�ν���޶��λ�֣�
			__DEBUG_LOG1_(0, "m_transLimit:%d",(*g_itrTR2)->m_transLimit);
			
			pUserSelfService.m_monthTransLimit=(*g_itrTR2)->m_monthTransLimit;//ÿ�½���޶��λ�֣�
			__DEBUG_LOG1_(0, "m_monthTransLimit:%d",(*g_itrTR2)->m_monthTransLimit);
			
			pUserSelfService.m_autotransflag=(*g_itrTR2)->m_autotransflag;//�Զ�������־��0-���Զ�������1-�Զ���
			__DEBUG_LOG1_(0, "m_autotransflag:%d",(*g_itrTR2)->m_autotransflag);
			
			pUserSelfService.m_autobalancelimit=(*g_itrTR2)->m_autobalancelimit;//�Զ�������ֵ����λ�֣�
			__DEBUG_LOG1_(0, "m_autobalancelimit:%d",(*g_itrTR2)->m_autobalancelimit);
			
			pUserSelfService.m_autoAmountlimit=(*g_itrTR2)->m_autoAmountlimit;//�Զ���������λ�֣�
			__DEBUG_LOG1_(0, "m_autoAmountlimit:%d",(*g_itrTR2)->m_autoAmountlimit);
			
		
			tTime = time(NULL);
			tTime += 2209017599;
			long2date(tTime,sResponseTime);
			strcpy(oSvc.m_responseTime,sResponseTime);
		
			strcpy(oSvc.m_responseId,pUserSelfService.m_requestId);
			
			oData.m_dateTransFreq=pUserSelfService.m_dateTransFreq;
			oData.m_transLimit=pUserSelfService.m_transLimit;
			oData.m_monthTransLimit=pUserSelfService.m_monthTransLimit;
			oData.m_autotransflag=pUserSelfService.m_autotransflag;
			oData.m_autobalancelimit=pUserSelfService.m_autobalancelimit;
			oData.m_autoAmountlimit=pUserSelfService.m_autoAmountlimit;
			
			__DEBUG_LOG0_(0, "---------CCA���£�");
			__DEBUG_LOG1_(0, "m_responseTime:%s",oSvc.m_responseTime);
			__DEBUG_LOG1_(0, "m_responseId:%s",oSvc.m_responseId);
			
			__DEBUG_LOG1_(0, "m_dateTransFreq:%d",oData.m_dateTransFreq);
			__DEBUG_LOG1_(0, "m_transLimit:%d",oData.m_transLimit);
			__DEBUG_LOG1_(0, "m_month_TransLimit:%d",oData.m_monthTransLimit);
			__DEBUG_LOG1_(0, "m_auto_transflag:%d",oData.m_autotransflag);
			__DEBUG_LOG1_(0, "m_autobalancelimit:%d",oData.m_autobalancelimit);
			__DEBUG_LOG1_(0, "m_autoAmountlimit:%d",oData.m_autoAmountlimit);
			
			vMicropaymentInfo.push_back(oData);
			
			if( ( iRet = m_poSqlPassBTR->PasswordResetAuthen(pUserSelfService,oExp) ) <0)
			{
				return PAY_PASSWD_AUTHEN_ERR;
			}
			
			
			__DEBUG_LOG2_(0, "��Ȩ����%s==%s",pUserSelfService.m_payPassword,sTmp);
			
			if(strncmp(pUserSelfService.m_payPassword,sTmp,strlen(pUserSelfService.m_payPassword))==0 &&
				strncmp(pUserSelfService.m_payPassword,sTmp,strlen(sTmp))==0)
			{
				//���û���ʶ����ҵ���¼��
				if( ( iRet = m_poSqlPassBTR->PasswordResetSelect(pUserSelfService,oExp) ) <0)
				{
					__DEBUG_LOG0_(0, "��ѯ�û���ʶʧ��!");
					throw PAY_PASSWD_SELECT_ERR;
				}
			
				//��ѯҵ����ˮ
				iRet=m_poSqlOpt->QueryPaySeq(sStructPaymentSeqInfo,bState);
				if(iRet!=0)
		        {
					__DEBUG_LOG0_(0, "��ѯҵ����ˮʧ��!");
					throw iRet;
		        }
		        
				pUserSelfService.m_lPaymentSeq=sStructPaymentSeqInfo.m_lPaymentSeq;
			
				if( (iRet = m_poSqlRules->TransferRulesQuery(pUserSelfService,oExp)) == 1 )
				{
					__DEBUG_LOG0_(0, "----��ѯû�л���֧�������¼����������---");
						iRet = m_poSqlRules->TransferRulesAcctId(pUserSelfService,oExp);
						
						pUserSelfService.m_datePayFreq =0;
						pUserSelfService.m_payLimit =0;
						pUserSelfService.m_monthPayLimit =0;
						pUserSelfService.m_microPayLimit =0;
						__DEBUG_LOG1_(0, "---��ѯ���˻���ʶ:%d",pUserSelfService.m_acctId);
						
						if( (iRet = m_poSqlRules->TransferRulesInsert(pUserSelfService,oExp)) == 0)
						{
							__DEBUG_LOG0_(0, "----��������֧������ɹ�---");
						}
						else
						{
							__DEBUG_LOG0_(0, "----��������֧������ʧ��---");
							oSvc.m_svcResultCode=TRANSFERRULES_INSERT_ERR;
						}
				}
				else
				{
					__DEBUG_LOG0_(0, "----��ѯ�л���֧�������¼��ֱ���޸�---");
					iRet = m_poSql->BalanceTransResetInfo(pUserSelfService,oExp);
				}
				
				strcpy(pPaymentInfoCCR.m_sOperateSeq,pUserSelfService.m_requestId);  //�ⲿ��ˮ
				strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0014"); // ��������
				pPaymentInfoCCR.m_lServID=atoi(pUserSelfService.m_userInfoId);  //�û���ʶ
				strcpy(pPaymentInfoCCR.m_sDescription,"�Է���ƽ̨������������");
				
				//��ҵ���¼
				iRet=m_poSqlOpt->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
				if(iRet!=0)
	        	{
					__DEBUG_LOG0_(0, "��¼ҵ����Ϣ��ʧ��!");
					throw iRet;
	        	}
			}
			else
			{
				oSvc.m_svcResultCode=PASSWD_AUTHEN_ERR;
			}
        	
		}
		
		// ��� - ��һ�������Ϣ
		if (!m_poPack1->addBalanceTransResetSvc(oSvc))
		{
			__DEBUG_LOG0_(0, "BalanceTransReset:: _deal m_poPack ���ʧ��!");
			m_poPack1->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_PACK_ERR;
		}
		// ���ԣ������oSvc����
		__DEBUG_LOG1_(0, "��һ�����CCA������,oSvc,responseId:[%s]",oSvc.m_responseId);
		
		// ��� - �ڶ���۷���ϸ
		for(int j=0;j<vMicropaymentInfo.size();j++)
		{
			if (!m_poPack1->addResTRMicropaymentInfo(vMicropaymentInfo[j]))
			{
				__DEBUG_LOG0_(0, "BalanceTransReset:: _deal m_poPack ���ʧ��!");
				m_poPack1->setRltCode(ECODE_PACK_FAIL);
				throw DEDUCT_PACK_ERR;
			}
		}
		__DEBUG_LOG0_(0, "�ڶ������CCA������.");
	}
	catch(ABMException &oExp)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG0_(0, "ABMException....");
	}
	catch(int &value ) 
	{

		iRet=value;
		oSvc.m_svcResultCode=iRet;
		__DEBUG_LOG1_(0, "CCA,ҵ�񼶵Ľ������iRet:[%d]",iRet);
	}

	return iRet;	
}


static vector<R_PasswordReset *> g_vRPasswordResetSvc;
static vector<R_PasswordReset *>::iterator g_itrPR1;

static vector<R_PRMicropaymentInfo *> g_vPRMicropaymentInfo;
static vector<R_PRMicropaymentInfo *>::iterator g_itrPR2;


PasswordReset::PasswordReset()
{
		m_poSql = NULL;
		m_poUnpack2 = NULL;
		m_poPack2 = NULL;
		m_dccOperation = NULL;
		m_poSqlOpt=NULL;
}

PasswordReset::~PasswordReset()
{
	if (m_poSql != NULL)
		delete m_poSql;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
	if (m_poSqlOpt != NULL)
		delete m_poSqlOpt;	
}

int PasswordReset::init(ABMException &oExp)
{
	if ((m_poSql = new PasswordResetSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PasswordReset::init malloc failed!");
		return -1;
	}
	
	if ((m_dccOperation = new DccOperation) == NULL)
	{
		ADD_EXCEPT0(oExp, "DccOperation::init malloc failed!");
		return -1;
	}
	
	if ((m_poSqlOpt = new PaymentInfoSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PaymentInfoSql::init malloc failed!");
		return -1;
	}
	return 0;
}

int PasswordReset::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>PasswordReset::deal �������ʼ");
	
	int iRet=0;
	int iSize=0;

	g_toPlatformFlag = true;
	
	m_poUnpack2 = (PasswordResetCCR *)pCCR;
	m_poPack2 = (PasswordResetCCA *)pCCA;
	
	m_poPack2->clear();
	g_vRPasswordResetSvc.clear();
	g_vPRMicropaymentInfo.clear();
	
	A_PasswordReset oSvc={0} ;

	// ���յ���DCC������Ϣͷ��TT
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	try 
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[֧�������޸�-PasswordReset]:��ѯSEQʧ��!");
			throw GET_DCC_INFO_RECORD_SEQ_ERR;	
		}
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "֧������״̬��ѯ,����CCR��,��Ϣͷ��TT!");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack2->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������յ�CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_RECEVIE_DCC_CCR_ERR;
		}
		
		__DEBUG_LOG0_(0, "֧�������޸�,CCR��Ϣ�����!");
		//��Ϣ��� - ��һ��
		if ((iSize=m_poUnpack2->getR_PasswordReset(g_vRPasswordResetSvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "֧�������޸�::deal ���������Ϣ��ΪNULL");
			m_poPack2->setRltCode(ECODE_NOMSG);
			throw DEDUCT_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "֧�������޸�::deal ��Ϣ�������!");
			m_poPack2->setRltCode(ECODE_UNPACK_FAIL);
			throw DEDUCT_UNPACK_ERR;
		}
	
		
		// �����һ����Ϣ
		for (g_itrPR1=g_vRPasswordResetSvc.begin(); g_itrPR1!=g_vRPasswordResetSvc.end(); ++g_itrPR1)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack2->getR_PRMicropaymentInfo(*g_itrPR1,g_vPRMicropaymentInfo)) == 0 )
			{
				__DEBUG_LOG0_(0, "PasswordReset::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack2->setRltCode(ECODE_NOMSG);
				throw DEDUCT_UNPACK_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "PasswordReset::deal ��Ϣ�������!");
				m_poPack2->setRltCode(ECODE_UNPACK_FAIL);
				throw DEDUCT_UNPACK_ERR;
			}
			 
			__DEBUG_LOG0_(0, "����:�����һ����Ϣ........");	

			iRet = _deal(*g_itrPR1,g_vPRMicropaymentInfo);

			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "֧�������޸Ĳ���ʧ��");
				throw iRet;
			}
		}
		//return iRet;
	}
	catch(TTStatus oSt)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "PasswordReset::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet=value;
		__DEBUG_LOG1_(0, "֧�������޸�ʧ��:: ʧ����Ϣ value=%d", value);
	}
	if(iRet != 0)
	{
		// �򷵻�CCA��,���ش�����
		oSvc.m_svcResultCode=iRet;
		if (!m_poPack2->addA_PasswordReset(oSvc))
		{
			__DEBUG_LOG0_(0, "PasswordReset:: _deal m_poPack ���ʧ��!");
			m_poPack2->clear();
			m_poPack2->setRltCode(ECODE_PACK_FAIL);
		}
		__DEBUG_LOG0_(0, "֧�������޸Ĳ���ʧ��,iRet=[%d]",oSvc.m_svcResultCode);
		
	}
	return iRet;
}


int PasswordReset::_deal(R_PasswordReset *pSvcIn,vector<R_PRMicropaymentInfo *> g_vPRMicropaymentInfo)
{
	
	ABMException oExp;
		
	int iRet=0;
	time_t tTime;
	char sResponseTime[32]={0};
	bool bState=false;
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================֧��������=======================");
	__DEBUG_LOG0_(0, "=====>start,ȡ��һ��CCR������Ϣ:");
	
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:%s",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime��%s",pSvcIn->m_requestTime);


	UserSelfService pUserSelfService={0};
	StructPaymentSeqInfo  sStructPaymentSeqInfo={0};
	PaymentInfoALLCCR pPaymentInfoCCR={0};
	
	// ����CCR��Ϣ׼��	
	strcpy(pUserSelfService.m_requestId,pSvcIn->m_requestId);		// ������ˮ��
	strcpy(pUserSelfService.m_requestTime,pSvcIn->m_requestTime);	// ����ʱ��

			
	A_PasswordReset oSvc={0} ;	// CCA ��Ӧ������Ϣ

	
	try
	{	

		for (g_itrPR2=g_vPRMicropaymentInfo.begin(); g_itrPR2!=g_vPRMicropaymentInfo.end(); ++g_itrPR2)
		{	
			
			strcpy(pUserSelfService.m_servNbr,(*g_itrPR2)->m_servNbr);		//�û�����
			__DEBUG_LOG1_(0, "m_servNbr:%s", (*g_itrPR2)->m_servNbr);
			
			pUserSelfService.m_nbrType =(*g_itrPR2)->m_nbrType;		//��������
			__DEBUG_LOG1_(0, "m_nbrType:%d",(*g_itrPR2)->m_nbrType);
				
			pUserSelfService.m_servAttr = (*g_itrPR2)->m_servAttr;		//�û�����
			__DEBUG_LOG1_(0, "m_servAttr:%d",(*g_itrPR2)->m_servAttr);
			
			strcpy(pUserSelfService.m_sServicePlatformID,(*g_itrPR2)->m_sServicePlatformID);//ҵ��ƽ̨ID
			__DEBUG_LOG1_(0, "m_sServicePlatformID:%s",(*g_itrPR2)->m_sServicePlatformID);
		
			strcpy(pUserSelfService.m_oldPassword,(*g_itrPR2)->m_oldPassword);//������
			__DEBUG_LOG1_(0, "m_oldPassword:%s",pUserSelfService.m_oldPassword);
			
			strcpy(pUserSelfService.m_newPassword,(*g_itrPR2)->m_newPassword);//������
			__DEBUG_LOG1_(0, "m_newPassword:%s",(*g_itrPR2)->m_newPassword);
			
			strcpy(pUserSelfService.m_actionId,(*g_itrPR2)->m_actionId);//��������
			__DEBUG_LOG1_(0, "m_actionId:%s",(*g_itrPR2)->m_actionId);
			
			
			
			tTime = time(NULL);
			tTime += 2209017599;
			long2date(tTime,sResponseTime);
			strcpy(oSvc.m_responseTime,sResponseTime);
		
			strcpy(oSvc.m_responseId,pUserSelfService.m_requestId);
			
			__DEBUG_LOG1_(0, "m_responseTime:%s",oSvc.m_responseTime);
			__DEBUG_LOG1_(0, "m_responseId:%s",oSvc.m_responseId);
			
			
			if( ( iRet = m_poSql->PasswordResetAuthen(pUserSelfService,oExp) ) <0)
			{
				return PAY_PASSWD_AUTHEN_ERR;
			}

			__DEBUG_LOG2_(0, "��Ȩ����%s==%s",pUserSelfService.m_payPassword,pUserSelfService.m_oldPassword);
			
			if(strncmp(pUserSelfService.m_payPassword,pUserSelfService.m_oldPassword,strlen(pUserSelfService.m_payPassword))==0 &&
				strncmp(pUserSelfService.m_payPassword,pUserSelfService.m_oldPassword,strlen(pUserSelfService.m_oldPassword))==0)
			{
				if( ( iRet = m_poSql->PasswordResetSelect(pUserSelfService,oExp) ) <0)
				{
					__DEBUG_LOG0_(0, "��ѯ�û���ʶʧ��!");
					throw PAY_PASSWD_SELECT_ERR;
				}
				
				//��ѯҵ����ˮ
				iRet=m_poSqlOpt->QueryPaySeq(sStructPaymentSeqInfo,bState);
				if(iRet!=0)
		        {
					__DEBUG_LOG0_(0, "��ѯҵ����ˮʧ��!");
					throw iRet;
		        }
		        
				pUserSelfService.m_lPaymentSeq=sStructPaymentSeqInfo.m_lPaymentSeq;
			
				if(strncmp(pUserSelfService.m_actionId,"01",2)==0)
				{
					if( ( iRet = m_poSql->PasswordResetUpdate(pUserSelfService,oExp) ) <0)
					{
						return	PAY_PASSWD_UPDATE_ERR;
					}
				}
				else if(strncmp(pUserSelfService.m_actionId,"02",2)==0)
				{
					if( (iRet = m_poSql->PasswordResetdelete(pUserSelfService,oExp) ) <0 )
					{
						return	PAY_PASSWD_DELETE_ERR;
					}
					
					if( ( iRet = m_poSql->PasswordResetSelect(pUserSelfService,oExp) )<0 )
					{
						return	PAY_PASSWD_SELECT_ERR;
					}
					
					
					if( (iRet = m_poSql->PasswordResetinsert(pUserSelfService,oExp) )<0)
					{
						return	PAY_PASSWD_INSERT_ERR;
					}
				}
				else
				{
					return PASSWD_FLAG_TYPE_ERR;
				}
				
				strcpy(pPaymentInfoCCR.m_sOperateSeq,pUserSelfService.m_requestId);  //�ⲿ��ˮ
				strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0010"); // ��������
				pPaymentInfoCCR.m_lServID=atoi(pUserSelfService.m_userInfoId);  //�û���ʶ
				strcpy(pPaymentInfoCCR.m_sDescription,"�Է���ƽ̨֧�������޸�");
				
				//��ҵ���¼
				iRet=m_poSqlOpt->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
				if(iRet!=0)
	        	{
					__DEBUG_LOG0_(0, "��¼ҵ����Ϣ��ʧ��!");
					throw iRet;
	        	}
			}
			else
			{
				oSvc.m_svcResultCode=PASSWD_AUTHEN_ERR;
			}
		}
		
		// ��� - ��һ�������Ϣ
		if (!m_poPack2->addA_PasswordReset(oSvc))
		{
			__DEBUG_LOG0_(0, "PasswordReset:: _deal m_poPack ���ʧ��!");
			m_poPack2->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_PACK_ERR;
		}
		// ���ԣ������oSvc����
		__DEBUG_LOG1_(0, "��һ�����CCA������,oSvc,responseId:[%s]",oSvc.m_responseId);
	
	}
	catch(ABMException &oExp)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG0_(0, "ABMException....");
	}
	catch(int &value ) 
	{

		iRet=value;
		oSvc.m_svcResultCode=iRet;
		__DEBUG_LOG1_(0, "CCA,ҵ�񼶵Ľ������iRet:[%d]",iRet);
	}

	return iRet;	
}



static vector<R_PayStatusReset *> g_vRPayStatusResetSvc;
static vector<R_PayStatusReset *>::iterator g_itrPSR1;

static vector<R_PSRMicropaymentInfo *> g_vPSRMicropaymentInfo;
static vector<R_PSRMicropaymentInfo *>::iterator g_itrPSR2;

PayStatusReset::PayStatusReset()
{
		m_poSql = NULL;
		m_poSqlPasswd = NULL;
		m_poUnpack = NULL;
		m_poPack = NULL;
		m_dccOperation = NULL;
		m_poSqlOpt=NULL;
}

PayStatusReset::~PayStatusReset()
{
	if (m_poSql != NULL)
		delete m_poSql;
	if (m_poSqlPasswd != NULL)
		delete m_poSqlPasswd;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
	if (m_poSqlOpt != NULL)
		delete m_poSqlOpt;	
}

int PayStatusReset::init(ABMException &oExp)
{
	if ((m_poSql = new PayStatusResetSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PayStatusReset::init malloc failed!");
		return -1;
	}
	
	if ((m_poSqlPasswd = new PasswordResetSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PasswordResetSql::init malloc failed!");
		return -1;
	}
	
	if ((m_dccOperation = new DccOperation) == NULL)
	{
		ADD_EXCEPT0(oExp, "DccOperation::init malloc failed!");
		return -1;
	}
	if ((m_poSqlOpt = new PaymentInfoSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PaymentInfoSql::init malloc failed!");
		return -1;
	}
	
	return 0;
}

int PayStatusReset::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>���ԣ�PayStatusReset::deal �������ʼ");
	
	int iRet=0;
	int iSize=0;

	g_toPlatformFlag = true;
	
	m_poUnpack = (PayStatusResetCCR *)pCCR;
	m_poPack = (PayStatusResetCCA *)pCCA;
	
	m_poPack->clear();
	g_vRPayStatusResetSvc.clear();
	g_vPSRMicropaymentInfo.clear();
	
	A_PayStatusReset oSvc={0} ;

	// ���յ���DCC������Ϣͷ��TT
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	try 
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[֧��״̬���-PayStatusReset]:��ѯSEQʧ��!");
			throw GET_DCC_INFO_RECORD_SEQ_ERR;	
		}
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "֧��״̬���,����CCR��,��Ϣͷ��TT!");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������յ�CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_RECEVIE_DCC_CCR_ERR;
		}
		
		__DEBUG_LOG0_(0, "֧��״̬���,CCR��Ϣ�����!");
		//��Ϣ��� - ��һ��
		if ((iSize=m_poUnpack->getR_PayStatusReset(g_vRPayStatusResetSvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "֧��״̬���::deal ���������Ϣ��ΪNULL");
			m_poPack->setRltCode(ECODE_NOMSG);
			throw DEDUCT_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "֧��״̬���::deal ��Ϣ�������!");
			m_poPack->setRltCode(ECODE_UNPACK_FAIL);
			throw DEDUCT_UNPACK_ERR;
		}
	
		
		// �����һ����Ϣ
		for (g_itrPSR1=g_vRPayStatusResetSvc.begin(); g_itrPSR1!=g_vRPayStatusResetSvc.end(); ++g_itrPSR1)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack->getR_PSRMicropaymentInfo(*g_itrPSR1,g_vPSRMicropaymentInfo)) == 0 )
			{
				__DEBUG_LOG0_(0, "PayStatusReset::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack->setRltCode(ECODE_NOMSG);
				throw DEDUCT_UNPACK_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "PayStatusReset::deal ��Ϣ�������!");
				m_poPack->setRltCode(ECODE_UNPACK_FAIL);
				throw DEDUCT_UNPACK_ERR;
			}
			 
			__DEBUG_LOG0_(0, "����:�����һ����Ϣ........");	

			iRet = _deal(*g_itrPSR1,g_vPSRMicropaymentInfo);

			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "֧��״̬�������ʧ��");
				throw iRet;
			}
		}
		//return iRet;
	}
	catch(TTStatus oSt)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "PayStatusReset::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet=value;
		__DEBUG_LOG1_(0, "֧��״̬���ʧ��:: ʧ����Ϣ value=%d", value);
	}
	if(iRet != 0)
	{
		// �򷵻�CCA��,���ش�����
		oSvc.m_svcResultCode=iRet;
		if (!m_poPack->addA_PayStatusReset(oSvc))
		{
			__DEBUG_LOG0_(0, "PayStatusReset:: _deal m_poPack ���ʧ��!");
			m_poPack->clear();
			m_poPack->setRltCode(ECODE_PACK_FAIL);
		}
		__DEBUG_LOG0_(0, "֧������״̬��ѯʧ��,iRet=[%d]",oSvc.m_svcResultCode);
		
	}
	return iRet;
}


int PayStatusReset::_deal(R_PayStatusReset *pSvcIn,vector<R_PSRMicropaymentInfo *> g_vPSRMicropaymentInfo)
{
	
	ABMException oExp;
		
	int iRet=0;
	time_t tTime;
	char sResponseTime[32]={0};
	char tPasswd[32];
	bool bState=false;
	
	memset(tPasswd,0,sizeof(tPasswd));
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================֧��״̬���=======================");
	__DEBUG_LOG0_(0, "=====>start,ȡ��һ��CCR������Ϣ:");
	
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:%s",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime��%s",pSvcIn->m_requestTime);


	UserSelfService pUserSelfService={0};
	StructPaymentSeqInfo  sStructPaymentSeqInfo={0};
	PaymentInfoALLCCR pPaymentInfoCCR={0};
	

	strcpy(pUserSelfService.m_requestId,pSvcIn->m_requestId);		// ������ˮ��
	strcpy(pUserSelfService.m_requestTime,pSvcIn->m_requestTime);	// ����ʱ��

			
	A_PayStatusReset oSvc={0} ;	// CCA ��Ӧ������Ϣ

	try
	{	

		for (g_itrPSR2=g_vPSRMicropaymentInfo.begin(); g_itrPSR2!=g_vPSRMicropaymentInfo.end(); ++g_itrPSR2)
		{	
			////CCR
			strcpy(pUserSelfService.m_servNbr,(*g_itrPSR2)->m_servNbr);		//�û�����
			__DEBUG_LOG1_(0, "m_servNbr:%s", (*g_itrPSR2)->m_servNbr);
			
			pUserSelfService.m_nbrType =(*g_itrPSR2)->m_nbrType;		//��������
			__DEBUG_LOG1_(0, "m_nbrType:%d",(*g_itrPSR2)->m_nbrType);
				
			pUserSelfService.m_servAttr = (*g_itrPSR2)->m_servAttr;		//�û�����
			__DEBUG_LOG1_(0, "m_servAttr:%d",(*g_itrPSR2)->m_servAttr);
			
			strcpy(pUserSelfService.m_sServicePlatformID,(*g_itrPSR2)->m_sServicePlatformID);//ҵ��ƽ̨ID
			__DEBUG_LOG1_(0, "m_sServicePlatformID:%s",(*g_itrPSR2)->m_sServicePlatformID);
		
			strcpy(tPasswd,(*g_itrPSR2)->m_payPassword);//֧������
			__DEBUG_LOG1_(0, "m_payPassword:%s",(*g_itrPSR2)->m_payPassword);
		
			strcpy(pUserSelfService.m_payStatus,(*g_itrPSR2)->m_payStatus);//֧��״̬
			__DEBUG_LOG1_(0, "m_payStatus:%s",(*g_itrPSR2)->m_payStatus);
			
			tTime = time(NULL);
			tTime += 2209017599;
			long2date(tTime,sResponseTime);
			strcpy(oSvc.m_responseTime,sResponseTime);
		
			strcpy(oSvc.m_responseId,pUserSelfService.m_requestId);
			strcpy(oSvc.m_payStatus,pUserSelfService.m_payStatus);
			
			__DEBUG_LOG1_(0, "m_responseTime:%s",oSvc.m_responseTime);
			__DEBUG_LOG1_(0, "m_responseId:%s",oSvc.m_responseId);
			__DEBUG_LOG1_(0, "m_payStatus:%s",oSvc.m_payStatus);
		
			//�����Ȩ
			if( ( iRet = m_poSqlPasswd->PasswordResetAuthen(pUserSelfService,oExp) ) <0)
			{
				return PAY_PASSWD_AUTHEN_ERR;
			}
			
			__DEBUG_LOG2_(0, "��Ȩ����%s==%s",pUserSelfService.m_payPassword,tPasswd);
			

			// ����ҵ���߼�����
			if(strncmp(pUserSelfService.m_payPassword,tPasswd,strlen(pUserSelfService.m_payPassword))==0 && \
				strncmp(pUserSelfService.m_payPassword,tPasswd,strlen(tPasswd))==0)
			{
				//��ѯ�û���ʶ��ҵ���¼��
				if( ( iRet = m_poSqlPasswd->PasswordResetSelect(pUserSelfService,oExp) ) <0)
				{
					__DEBUG_LOG0_(0, "��ѯҵ����ˮʧ��!");
					throw PAY_PASSWD_SELECT_ERR;
				}
			
				//��ѯҵ����ˮ
				iRet=m_poSqlOpt->QueryPaySeq(sStructPaymentSeqInfo,bState);
				if(iRet!=0)
		        {
					__DEBUG_LOG0_(0, "��ѯҵ����ˮʧ��!");
					throw iRet;
		        }
		        
				pUserSelfService.m_lPaymentSeq=sStructPaymentSeqInfo.m_lPaymentSeq;
			
				iRet = m_poSql->PayStatusResetUpdate(pUserSelfService,oExp);
				
				
				strcpy(pPaymentInfoCCR.m_sOperateSeq,pUserSelfService.m_requestId);  //�ⲿ��ˮ
				strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0015"); //ҵ�����
				pPaymentInfoCCR.m_lServID=atoi(pUserSelfService.m_userInfoId);  //�û���ʾ
				strcpy(pPaymentInfoCCR.m_sDescription,"�Է���ƽ̨֧��״̬���");
				
				//��ҵ���¼
				iRet=m_poSqlOpt->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
				if(iRet!=0)
	        	{
					__DEBUG_LOG0_(0, "��¼ҵ����Ϣ��ʧ��!");
					throw iRet;
	        	}
        	
			}
			else
			{
				oSvc.m_svcResultCode=PASSWD_AUTHEN_ERR;
			}
			
		}
		
		// ��� - ��һ�������Ϣ
		if (!m_poPack->addA_PayStatusReset(oSvc))
		{
			__DEBUG_LOG0_(0, "PayStatusReset:: _deal m_poPack ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_PACK_ERR;
		}
		// ���ԣ������oSvc����
		__DEBUG_LOG1_(0, "��һ�����CCA������,oSvc,responseId:[%s]",oSvc.m_responseId);
		

	}
	catch(ABMException &oExp)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG0_(0, "ABMException....");
	}
	catch(int &value ) 
	{

		iRet=value;
		oSvc.m_svcResultCode=iRet;
		__DEBUG_LOG1_(0, "CCA,ҵ�񼶵Ľ������iRet:[%d]",iRet);
	}

	return iRet;	
}


static vector<R_PayRulesQuerySvc *> g_vPayRulesQuerySvc;
static vector<R_PayRulesQuerySvc *>::iterator g_itrPRQ1;

static vector<R_PRQMicropaymentInfo *> g_vPRQMicropaymentInfo;
static vector<R_PRQMicropaymentInfo *>::iterator g_itrPRQ2;


PayRulesQuery::PayRulesQuery()
{
		m_poSql = NULL;
		m_poUnpack = NULL;
		m_poPack = NULL;
		m_dccOperation = NULL;
}

PayRulesQuery::~PayRulesQuery()
{
	if (m_poSql != NULL)
		delete m_poSql;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int PayRulesQuery::init(ABMException &oExp)
{
	if ((m_poSql = new PayRulesQuerySql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PayRulesQuery::init malloc failed!");
		return -1;
	}
	
	if ((m_dccOperation = new DccOperation) == NULL)
	{
		ADD_EXCEPT0(oExp, "DccOperation::init malloc failed!");
		return -1;
	}
	
	return 0;
}

int PayRulesQuery::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>���ԣ�PayRulesQuery::deal �������ʼ");
	
	int iRet=0;
	int iSize=0;

	g_toPlatformFlag = true;
	
	m_poUnpack = (PayRulesQueryCCR *)pCCR;
	m_poPack = (PayRulesQueryCCA *)pCCA;
	
	m_poPack->clear();
	g_vPayRulesQuerySvc.clear();
	g_vPRQMicropaymentInfo.clear();
	
	A_PayRulesQuerySvc oSvc={0} ;

	// ���յ���DCC������Ϣͷ��TT
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	try 
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[֧�������ѯ-PayRulesQuery]:��ѯSEQʧ��!");
			throw GET_DCC_INFO_RECORD_SEQ_ERR;	
		}
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "֧�������ѯ,����CCR��,��Ϣͷ��TT!");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������յ�CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_RECEVIE_DCC_CCR_ERR;
		}
		
		__DEBUG_LOG0_(0, "֧�������ѯ,CCR��Ϣ�����!");
		//��Ϣ��� - ��һ��
		if ((iSize=m_poUnpack->getR_PayRulesQuerySvc(g_vPayRulesQuerySvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "֧�������ѯ::deal ���������Ϣ��ΪNULL");
			m_poPack->setRltCode(ECODE_NOMSG);
			throw DEDUCT_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "֧�������ѯ::deal ��Ϣ�������!");
			m_poPack->setRltCode(ECODE_UNPACK_FAIL);
			throw DEDUCT_UNPACK_ERR;
		}
	
		
		// �����һ����Ϣ
		for (g_itrPRQ1=g_vPayRulesQuerySvc.begin(); g_itrPRQ1!=g_vPayRulesQuerySvc.end(); ++g_itrPRQ1)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack->getR_PRQMicropaymentInfo(*g_itrPRQ1,g_vPRQMicropaymentInfo)) == 0 )
			{
				__DEBUG_LOG0_(0, "PayRulesQuery::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack->setRltCode(ECODE_NOMSG);
				throw DEDUCT_UNPACK_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "PayRulesQuery::deal ��Ϣ�������!");
				m_poPack->setRltCode(ECODE_UNPACK_FAIL);
				throw DEDUCT_UNPACK_ERR;
			}
			 
			__DEBUG_LOG0_(0, "����:�����һ����Ϣ........");	

			iRet = _deal(*g_itrPRQ1,g_vPRQMicropaymentInfo);

			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "֧�������ѯ����ʧ��");
				throw iRet;
			}
		}
		//return iRet;
	}
	catch(TTStatus oSt)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "PayRulesQuery::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet=value;
		__DEBUG_LOG1_(0, "֧�������ѯʧ��:: ʧ����Ϣ value=%d", value);
	}
	if(iRet != 0)
	{
		// �򷵻�CCA��,���ش�����
		oSvc.m_svcResultCode=iRet;
		if (!m_poPack->addA_PayRulesQuerySvc(oSvc))
		{
			__DEBUG_LOG0_(0, "PayRulesQuery:: _deal m_poPack ���ʧ��!");
			m_poPack->clear();
			m_poPack->setRltCode(ECODE_PACK_FAIL);
		}
		__DEBUG_LOG0_(0, "֧�������ѯʧ��,iRet=[%d]",oSvc.m_svcResultCode);
		
	}
	return iRet;
}


int PayRulesQuery::_deal(R_PayRulesQuerySvc *pSvcIn,vector<R_PRQMicropaymentInfo *> g_vPRQMicropaymentInfo)
{
	
	ABMException oExp;
		
	int iRet=0;
	time_t tTime;
	char sResponseTime[32]={0};
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================֧�������ѯ=======================");
	__DEBUG_LOG0_(0, "=====>start,ȡ��һ��CCR������Ϣ:");
	
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:%s",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime��%s",pSvcIn->m_requestTime);


	UserSelfService pUserSelfService={0};
	
	// ����CCR��Ϣ׼��	
	strcpy(pUserSelfService.m_requestId,pSvcIn->m_requestId);		// ������ˮ��
	strcpy(pUserSelfService.m_requestTime,pSvcIn->m_requestTime);	// ����ʱ��

			
	A_PayRulesQuerySvc oSvc={0} ;	// CCA ��Ӧ������Ϣ
	A_PRQMicropaymentInfo oData={0} ;	// CCA ��Ӧ�۷���Ŀ��ϸ��Ϣ
	vector<struct A_PRQMicropaymentInfo> vPQRMicropaymentInfo;

	try
	{	

		for (g_itrPRQ2=g_vPRQMicropaymentInfo.begin(); g_itrPRQ2!=g_vPRQMicropaymentInfo.end(); ++g_itrPRQ2)
		{	
			strcpy(pUserSelfService.m_servNbr,(*g_itrPRQ2)->m_servNbr);		//�û�����
			__DEBUG_LOG1_(0, "m_servNbr:%s", (*g_itrPRQ2)->m_servNbr);
			
			pUserSelfService.m_nbrType =(*g_itrPRQ2)->m_nbrType;		//��������
			__DEBUG_LOG1_(0, "m_nbrType:%d",(*g_itrPRQ2)->m_nbrType);
				
			pUserSelfService.m_servAttr = (*g_itrPRQ2)->m_servAttr;		//�û�����
			__DEBUG_LOG1_(0, "m_servAttr:%d",(*g_itrPRQ2)->m_servAttr);
			
			strcpy(pUserSelfService.m_sServicePlatformID,(*g_itrPRQ2)->m_sServicePlatformID);//ҵ��ƽ̨ID
			__DEBUG_LOG1_(0, "m_sServicePlatformID:%s",(*g_itrPRQ2)->m_sServicePlatformID);
			
		
			// ����ҵ���߼�����
			iRet = m_poSql->PayRulesQueryInfo(pUserSelfService,oExp) ;
			
			
			tTime = time(NULL);
			tTime += 2209017599;
			long2date(tTime,sResponseTime);
			strcpy(oSvc.m_responseTime,sResponseTime);
			strcpy(oSvc.m_responseId,pUserSelfService.m_requestId);
			
			oData.m_datePayFreq=pUserSelfService.m_datePayFreq;
			oData.m_payLimit=pUserSelfService.m_payLimit;
			oData.m_monthPayLimit=pUserSelfService.m_monthPayLimit;
			oData.m_microPayLimit=pUserSelfService.m_microPayLimit;
			
			__DEBUG_LOG0_(0, "----------CCA---------");
			__DEBUG_LOG1_(0, "m_responseTime:%s",oSvc.m_responseTime);
			__DEBUG_LOG1_(0, "m_responseId:%s",oSvc.m_responseId);
			
			__DEBUG_LOG1_(0, "m_datePayFreq:%d",oData.m_datePayFreq);
			__DEBUG_LOG1_(0, "m_payLimit:%d",oData.m_payLimit);
			__DEBUG_LOG1_(0, "m_monthPayLimit:%d",oData.m_monthPayLimit);
			__DEBUG_LOG1_(0, "m_microPayLimit:%d",oData.m_microPayLimit);
			
			vPQRMicropaymentInfo.push_back(oData);
		}
		
		// ��� - ��һ�������Ϣ
		if (!m_poPack->addA_PayRulesQuerySvc(oSvc))
		{
			__DEBUG_LOG0_(0, "PayRulesQuery:: _deal m_poPack ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_PACK_ERR;
		}
		// ���ԣ������oSvc����
		__DEBUG_LOG1_(0, "��һ�����CCA������,oSvc,responseId:[%s]",oSvc.m_responseId);
		
		// ��� - �ڶ���۷���ϸ
		for(int j=0;j<vPQRMicropaymentInfo.size();j++)
		{
			if (!m_poPack->addA_PRQMicropaymentInfo(vPQRMicropaymentInfo[j]))
			{
				__DEBUG_LOG0_(0, "PayRulesQuery:: _deal m_poPack ���ʧ��!");
				m_poPack->setRltCode(ECODE_PACK_FAIL);
				throw DEDUCT_PACK_ERR;
			}
		}
		__DEBUG_LOG0_(0, "�ڶ������CCA������.");
	}
	catch(ABMException &oExp)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG0_(0, "ABMException....");
	}
	catch(int &value ) 
	{

		iRet=value;
		oSvc.m_svcResultCode=iRet;
		__DEBUG_LOG1_(0, "CCA,ҵ�񼶵Ľ������iRet:[%d]",iRet);
	}

	return iRet;	
}




static vector<R_PayRulesResetSvc *> g_vPayRulesResetSvc;
static vector<R_PayRulesResetSvc *>::iterator g_itrPSE1;

static vector<R_PRRMicropaymentInfo *> g_vPRRMicropaymentInfo;
static vector<R_PRRMicropaymentInfo *>::iterator g_itrPSE2;


PayRulesReset::PayRulesReset()
{
		m_poSql = NULL;
		m_poUnpack = NULL;
		m_poSqlPass=NULL;
		m_poSqlPayRules=NULL;
		m_poSqlOpt=NULL;
		m_poPack = NULL;
		m_dccOperation = NULL;
}

PayRulesReset::~PayRulesReset()
{
	if (m_poSql != NULL)
		delete m_poSql;
	if (m_poSqlPass != NULL)
		delete m_poSqlPass;
	if (m_poSqlPayRules != NULL)
		delete m_poSqlPayRules;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
	if (m_poSqlOpt != NULL)
		delete m_poSqlOpt;	
}

int PayRulesReset::init(ABMException &oExp)
{
	if ((m_poSql = new PayRulesResetSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PayRulesReset::init malloc failed!");
		return -1;
	}
	
	if ((m_poSqlPass = new PasswordResetSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PasswordResetSql::init malloc failed!");
		return -1;
	}
	if ((m_poSqlPayRules = new TransferRulesSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "TransferRulesSql::init malloc failed!");
		return -1;
	}
	
	if ((m_dccOperation = new DccOperation) == NULL)
	{
		ADD_EXCEPT0(oExp, "DccOperation::init malloc failed!");
		return -1;
	}
	
	if ((m_poSqlOpt = new PaymentInfoSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "PaymentInfoSql::init malloc failed!");
		return -1;
	}
	return 0;
}

int PayRulesReset::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>PayRulesReset::deal �������ʼ");
	
	int iRet=0;
	int iSize=0;

	g_toPlatformFlag = true;
	
	m_poUnpack = (PayRulesResetCCR *)pCCR;
	m_poPack = (PayRulesResetCCA *)pCCA;
	
	m_poPack->clear();
	g_vPayRulesResetSvc.clear();
	g_vPRRMicropaymentInfo.clear();
	
	A_PayRulesResetSvc oSvc={0} ;

	// ���յ���DCC������Ϣͷ��TT
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	try 
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[֧����������-PayRulesReset]:��ѯSEQʧ��!");
			throw GET_DCC_INFO_RECORD_SEQ_ERR;	
		}
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "֧����������,����CCR��,��Ϣͷ��TT!");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������յ�CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_RECEVIE_DCC_CCR_ERR;
		}
		
		__DEBUG_LOG0_(0, "֧����������,CCR��Ϣ�����!");
		//��Ϣ��� - ��һ��
		if ((iSize=m_poUnpack->getR_PayRulesResetSvc(g_vPayRulesResetSvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "֧����������::deal ���������Ϣ��ΪNULL");
			m_poPack->setRltCode(ECODE_NOMSG);
			throw DEDUCT_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "֧����������::deal ��Ϣ�������!");
			m_poPack->setRltCode(ECODE_UNPACK_FAIL);
			throw DEDUCT_UNPACK_ERR;
		}
	
		
		// �����һ����Ϣ
		for (g_itrPSE1=g_vPayRulesResetSvc.begin(); g_itrPSE1!=g_vPayRulesResetSvc.end(); ++g_itrPSE1)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack->getR_PRRMicropaymentInfo(*g_itrPSE1,g_vPRRMicropaymentInfo)) == 0 )
			{
				__DEBUG_LOG0_(0, "PayRulesReset::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack->setRltCode(ECODE_NOMSG);
				throw DEDUCT_UNPACK_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "PayRulesReset::deal ��Ϣ�������!");
				m_poPack->setRltCode(ECODE_UNPACK_FAIL);
				throw DEDUCT_UNPACK_ERR;
			}
			 
			__DEBUG_LOG0_(0, "����:�����һ����Ϣ........");	

			iRet = _deal(*g_itrPSE1,g_vPRRMicropaymentInfo);

			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "֧���������ò���ʧ��");
				throw iRet;
			}
		}
		//return iRet;
	}
	catch(TTStatus oSt)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "PayRulesReset::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet=value;
		__DEBUG_LOG1_(0, "֧����������ʧ��:: ʧ����Ϣ value=%d", value);
	}
	if(iRet != 0)
	{
		// �򷵻�CCA��,���ش�����
		oSvc.m_svcResultCode=iRet;
		if (!m_poPack->addA_PayRulesResetSvc(oSvc))
		{
			__DEBUG_LOG0_(0, "PayRulesReset:: _deal m_poPack ���ʧ��!");
			m_poPack->clear();
			m_poPack->setRltCode(ECODE_PACK_FAIL);
		}
		__DEBUG_LOG0_(0, "֧����������ʧ��,iRet=[%d]",oSvc.m_svcResultCode);
		
	}
	return iRet;
}


int PayRulesReset::_deal(R_PayRulesResetSvc *pSvcIn,vector<R_PRRMicropaymentInfo *> g_vPRRMicropaymentInfo)
{
	
	ABMException oExp;
		
	int iRet=0;
	time_t tTime;
	char sResponseTime[32]={0};
	char tPasswd[32];
	bool bState=false;
	
	memset(tPasswd,0,sizeof(tPasswd));
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================֧����������=======================");
	__DEBUG_LOG0_(0, "=====>start,ȡ��һ��CCR������Ϣ:");
	
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:%s",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime��%s",pSvcIn->m_requestTime);


	UserSelfService pUserSelfService={0};
	StructPaymentSeqInfo  sStructPaymentSeqInfo={0};
	PaymentInfoALLCCR pPaymentInfoCCR={0};
	
	// ����CCR��Ϣ׼��	
	strcpy(pUserSelfService.m_requestId,pSvcIn->m_requestId);		// ������ˮ��
	strcpy(pUserSelfService.m_requestTime,pSvcIn->m_requestTime);	// ����ʱ��

			
	A_PayRulesResetSvc oSvc={0} ;	// CCA ��Ӧ������Ϣ
	A_PRRMicropaymentInfo oData={0} ;	// CCA ��Ӧ�۷���Ŀ��ϸ��Ϣ
	vector<struct A_PRRMicropaymentInfo> vPRRMicropaymentInfo;

	try
	{	

		for (g_itrPSE2=g_vPRRMicropaymentInfo.begin(); g_itrPSE2!=g_vPRRMicropaymentInfo.end(); ++g_itrPSE2)
		{	
			strcpy(pUserSelfService.m_servNbr,(*g_itrPSE2)->m_servNbr);		//�û�����
			__DEBUG_LOG1_(0, "m_servNbr:%s", (*g_itrPSE2)->m_servNbr);
			
			pUserSelfService.m_nbrType =(*g_itrPSE2)->m_nbrType;		//��������
			__DEBUG_LOG1_(0, "m_nbrType:%d",(*g_itrPSE2)->m_nbrType);
				
			pUserSelfService.m_servAttr = (*g_itrPSE2)->m_servAttr;		//�û�����
			__DEBUG_LOG1_(0, "m_servAttr:%d",(*g_itrPSE2)->m_servAttr);
			
			strcpy(pUserSelfService.m_sServicePlatformID,(*g_itrPSE2)->m_sServicePlatformID);//ҵ��ƽ̨ID
			__DEBUG_LOG1_(0, "m_sServicePlatformID:%s",(*g_itrPSE2)->m_sServicePlatformID);
			
			strcpy(tPasswd,(*g_itrPSE2)->m_payPassword);//֧������
			__DEBUG_LOG1_(0, "m_payPassword:%s",(*g_itrPSE2)->m_payPassword);
			
			pUserSelfService.m_datePayFreq =(*g_itrPSE2)->m_datePayFreq;		////ÿ��֧��Ƶ�ȣ���λ�Σ�
			__DEBUG_LOG1_(0, "m_datePayFreq:%d",(*g_itrPSE2)->m_datePayFreq);
			
			pUserSelfService.m_payLimit =(*g_itrPSE2)->m_payLimit;		//ÿ��֧���޶��λ�֣�
			__DEBUG_LOG1_(0, "m_payLimit:%d",(*g_itrPSE2)->m_payLimit);
			
			pUserSelfService.m_monthPayLimit =(*g_itrPSE2)->m_monthPayLimit;		//ÿ��֧���޶��λ�֣�
			__DEBUG_LOG1_(0, "m_monthPayLimit:%d",(*g_itrPSE2)->m_monthPayLimit);
			
			pUserSelfService.m_microPayLimit =(*g_itrPSE2)->m_microPayLimit;		//С��֧����ֵ����λ�֣�λ�֣�
			__DEBUG_LOG1_(0, "m_microPayLimit:%d",(*g_itrPSE2)->m_microPayLimit);
			
			tTime = time(NULL);
			tTime += 2209017599;
			long2date(tTime,sResponseTime);
			strcpy(oSvc.m_responseTime,sResponseTime);
			strcpy(oSvc.m_responseId,pUserSelfService.m_requestId);
			
			oData.m_datePayFreq=pUserSelfService.m_datePayFreq;
			oData.m_payLimit=pUserSelfService.m_payLimit;
			oData.m_monthPayLimit=pUserSelfService.m_monthPayLimit;
			oData.m_microPayLimit=pUserSelfService.m_microPayLimit;
			
			__DEBUG_LOG0_(0, "----------CCA---------");
			__DEBUG_LOG1_(0, "m_responseTime:%s",oSvc.m_responseTime);
			__DEBUG_LOG1_(0, "m_responseId:%s",oSvc.m_responseId);
			
			__DEBUG_LOG1_(0, "m_datePayFreq:%d",oData.m_datePayFreq);
			__DEBUG_LOG1_(0, "m_payLimit:%d",oData.m_payLimit);
			__DEBUG_LOG1_(0, "m_monthPayLimit:%d",oData.m_monthPayLimit);
			__DEBUG_LOG1_(0, "m_microPayLimit:%d",oData.m_microPayLimit);
			
			vPRRMicropaymentInfo.push_back(oData);
		
			if( ( iRet = m_poSqlPass->PasswordResetAuthen(pUserSelfService,oExp) ) <0)
			{
				oSvc.m_svcResultCode= PAY_PASSWD_AUTHEN_ERR;
			}
			
			__DEBUG_LOG2_(0, "��Ȩ����%s==%s",pUserSelfService.m_payPassword,tPasswd);
			

			// ����ҵ���߼�����
			if(strncmp(pUserSelfService.m_payPassword,tPasswd,strlen(pUserSelfService.m_payPassword))==0 &&\
				strncmp(pUserSelfService.m_payPassword,tPasswd,strlen(tPasswd))==0 )
			{
				//��ѯ�û���ʶ����ҵ���¼��
				if( ( iRet = m_poSqlPass->PasswordResetSelect(pUserSelfService,oExp) ) <0)
				{
					__DEBUG_LOG0_(0, "��ѯҵ����ˮʧ��!");
					throw PAY_PASSWD_SELECT_ERR;
		        
				}
				
				//��ѯҵ����ˮ
				iRet=m_poSqlOpt->QueryPaySeq(sStructPaymentSeqInfo,bState);
				if(iRet!=0)
		        {
					__DEBUG_LOG0_(0, "��ѯҵ����ˮʧ��!");
					throw iRet;
		        }
		        
				pUserSelfService.m_lPaymentSeq=sStructPaymentSeqInfo.m_lPaymentSeq;
			
				if( (iRet = m_poSqlPayRules->TransferRulesQuery(pUserSelfService,oExp)) == 1 )
				{
					__DEBUG_LOG0_(0, "----��ѯû�л���֧�������¼����������---");
					iRet = m_poSqlPayRules->TransferRulesAcctId(pUserSelfService,oExp);
						
					pUserSelfService.m_dateTransFreq =0;
					pUserSelfService.m_transLimit =0;
					pUserSelfService.m_monthTransLimit =0;
					pUserSelfService.m_autotransflag =0;
					pUserSelfService.m_autobalancelimit =0;
					pUserSelfService.m_autoAmountlimit =0;
					__DEBUG_LOG1_(0, "---��ѯ���˻���ʶ:%d",pUserSelfService.m_acctId);
				
					if( (iRet = m_poSqlPayRules->TransferRulesInsert(pUserSelfService,oExp)) == 0)
					{
						__DEBUG_LOG0_(0, "----��������֧������ɹ�---");
					}
					else
					{
							__DEBUG_LOG0_(0, "----��������֧������ʧ��---");
							oSvc.m_svcResultCode=TRANSFERRULES_INSERT_ERR;
					}
				}
				else
				{
					__DEBUG_LOG0_(0, "----��ѯ�л���֧�������¼��ֱ���޸�---");
					iRet = m_poSql->PayRulesUpdate(pUserSelfService,oExp) ;
				}
				
				strcpy(pPaymentInfoCCR.m_sOperateSeq,pUserSelfService.m_requestId);  //�ⲿ��ˮ
				strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0013"); // ��������
				pPaymentInfoCCR.m_lServID=atoi(pUserSelfService.m_userInfoId);  //�û���ʾ
				strcpy(pPaymentInfoCCR.m_sDescription,"�Է���ƽ̨֧����������");
				
				//��ҵ���¼
				iRet=m_poSqlOpt->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
				if(iRet!=0)
	        	{
					__DEBUG_LOG0_(0, "��¼ҵ����Ϣ��ʧ��!");
					throw iRet;
	        	}
			}
			else
			{
				
				oSvc.m_svcResultCode=PASSWD_AUTHEN_ERR;
				
			}
			
		}
		
		// ��� - ��һ�������Ϣ
		if (!m_poPack->addA_PayRulesResetSvc(oSvc))
		{
			__DEBUG_LOG0_(0, "PayRulesReset:: _deal m_poPack ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_PACK_ERR;
		}
		// ���ԣ������oSvc����
		__DEBUG_LOG1_(0, "��һ�����CCA������,oSvc,responseId:[%s]",oSvc.m_responseId);
		
		// ��� - �ڶ���۷���ϸ
		for(int j=0;j<vPRRMicropaymentInfo.size();j++)
		{
			if (!m_poPack->addA_PRRMicropaymentInfo(vPRRMicropaymentInfo[j]))
			{
				__DEBUG_LOG0_(0, "PayRulesReset:: _deal m_poPack ���ʧ��!");
				m_poPack->setRltCode(ECODE_PACK_FAIL);
				throw DEDUCT_PACK_ERR;
			}
		}
		__DEBUG_LOG0_(0, "�ڶ������CCA������.");
	}
	catch(ABMException &oExp)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG0_(0, "ABMException....");
	}
	catch(int &value ) 
	{

		iRet=value;
		oSvc.m_svcResultCode=iRet;
		__DEBUG_LOG1_(0, "CCA,ҵ�񼶵Ľ������iRet:[%d]",iRet);
	}

	return iRet;	
}


static vector<R_BindingAccNbrQuery *> g_vRBindingAccNbrQuery;
static vector<R_BindingAccNbrQuery *>::iterator g_itrBDA1;

static vector<R_BDAMicropaymentInfo *> g_vRBDAMicropaymentInfo;
static vector<R_BDAMicropaymentInfo *>::iterator g_itrBDA2;


BindingAccNbrQuery::BindingAccNbrQuery()
{
		m_poSql = NULL;
		m_poUnpack = NULL;
		m_poPack = NULL;
		m_dccOperation = NULL;
}

BindingAccNbrQuery::~BindingAccNbrQuery()
{
	if (m_poSql != NULL)
		delete m_poSql;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int BindingAccNbrQuery::init(ABMException &oExp)
{
	if ((m_poSql = new BindingAccNbrQuerySql) == NULL)
	{
		ADD_EXCEPT0(oExp, "BindingAccNbrQuery::init malloc failed!");
		return -1;
	}
	if ((m_dccOperation = new DccOperation) == NULL)
	{
		ADD_EXCEPT0(oExp, "DccOperation::init malloc failed!");
		return -1;
	}
	return 0;
}

int BindingAccNbrQuery::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>���ԣ�BindingAccNbrQuery::deal �������ʼ");
	
	int iRet=0;
	int iSize=0;

	g_toPlatformFlag = true;
	
	m_poUnpack = (BindingAccNbrQueryCCR *)pCCR;
	m_poPack = (BindingAccNbrQueryCCA *)pCCA;
	
	m_poPack->clear();
	g_vRBindingAccNbrQuery.clear();
	g_vRBDAMicropaymentInfo.clear();
	
	A_BindingAccNbrQuery oSvc={0} ;

	// ���յ���DCC������Ϣͷ��TT
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	try 
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[�󶨺����ѯ-BindingAccNbrQuery]:��ѯSEQʧ��!");
			throw GET_DCC_INFO_RECORD_SEQ_ERR;	
		}
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "�󶨺����ѯ,����CCR��,��Ϣͷ��TT!");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������յ�CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_RECEVIE_DCC_CCR_ERR;
		}
		
		__DEBUG_LOG0_(0, "�󶨺����ѯ,CCR��Ϣ�����!");
		//��Ϣ��� - ��һ��
		if ((iSize=m_poUnpack->getR_BindingAccNbrQuery(g_vRBindingAccNbrQuery)) == 0 )
		{
			__DEBUG_LOG0_(0, "�󶨺����ѯ::deal ���������Ϣ��ΪNULL");
			m_poPack->setRltCode(ECODE_NOMSG);
			throw DEDUCT_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "�󶨺����ѯ::deal ��Ϣ�������!");
			m_poPack->setRltCode(ECODE_UNPACK_FAIL);
			throw DEDUCT_UNPACK_ERR;
		}
	
		
		// �����һ����Ϣ
		for (g_itrBDA1=g_vRBindingAccNbrQuery.begin(); g_itrBDA1!=g_vRBindingAccNbrQuery.end(); ++g_itrBDA1)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack->getR_BDAMicropaymentInfo(*g_itrBDA1,g_vRBDAMicropaymentInfo)) == 0 )
			{
				__DEBUG_LOG0_(0, "BindingAccNbrQuery::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack->setRltCode(ECODE_NOMSG);
				throw DEDUCT_UNPACK_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "BindingAccNbrQuery::deal ��Ϣ�������!");
				m_poPack->setRltCode(ECODE_UNPACK_FAIL);
				throw DEDUCT_UNPACK_ERR;
			}
			 
			__DEBUG_LOG0_(0, "����:�����һ����Ϣ........");	

			iRet = _deal(*g_itrBDA1,g_vRBDAMicropaymentInfo);

			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "�󶨺����ѯ����ʧ��");
				throw iRet;
			}
		}
		//return iRet;
	}
	catch(TTStatus oSt)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "BindingAccNbrQuery::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet=value;
		__DEBUG_LOG1_(0, "�󶨺����ѯʧ��:: ʧ����Ϣ value=%d", value);
	}
	if(iRet != 0)
	{
		// �򷵻�CCA��,���ش�����
		oSvc.m_svcResultCode=iRet;
		if (!m_poPack->addA_BindingAccNbrQuery(oSvc))
		{
			__DEBUG_LOG0_(0, "BindingAccNbrQuery:: _deal m_poPack ���ʧ��!");
			m_poPack->clear();
			m_poPack->setRltCode(ECODE_PACK_FAIL);
		}
		__DEBUG_LOG0_(0, "�󶨺����ѯʧ��,iRet=[%d]",oSvc.m_svcResultCode);
		
	}
	return iRet;
}


int BindingAccNbrQuery::_deal(R_BindingAccNbrQuery *pSvcIn,vector<R_BDAMicropaymentInfo *> g_vRBDAMicropaymentInfo)
{
	
	ABMException oExp;
		
	int iRet=0;
	time_t tTime;
	char sResponseTime[32]={0};
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================�󶨺����ѯ=======================");
	__DEBUG_LOG0_(0, "=====>start,ȡ��һ��CCR������Ϣ:");
	
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:%s",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime��%s",pSvcIn->m_requestTime);


	UserSelfService pUserSelfService={0};
	
	// ����CCR��Ϣ׼��	
	strcpy(pUserSelfService.m_requestId,pSvcIn->m_requestId);		// ������ˮ��
	strcpy(pUserSelfService.m_requestTime,pSvcIn->m_requestTime);	// ����ʱ��

	A_BindingAccNbrQuery oSvc={0} ;	// CCA ��Ӧ������Ϣ
	
	try
	{	

		for (g_itrBDA2=g_vRBDAMicropaymentInfo.begin(); g_itrBDA2!=g_vRBDAMicropaymentInfo.end(); ++g_itrBDA2)
		{	
			strcpy(pUserSelfService.m_servNbr,(*g_itrBDA2)->m_servNbr);		//�û�����
			__DEBUG_LOG1_(0, "m_servNbr:%s", (*g_itrBDA2)->m_servNbr);
			
			pUserSelfService.m_nbrType =(*g_itrBDA2)->m_nbrType;		//��������
			__DEBUG_LOG1_(0, "m_nbrType:%d",(*g_itrBDA2)->m_nbrType);
				
			pUserSelfService.m_servAttr = (*g_itrBDA2)->m_servAttr;		//�û�����
			__DEBUG_LOG1_(0, "m_servAttr:%d",(*g_itrBDA2)->m_servAttr);
			
			strcpy(pUserSelfService.m_sServicePlatformID,(*g_itrBDA2)->m_sServicePlatformID);//ҵ��ƽ̨ID
			__DEBUG_LOG1_(0, "m_sServicePlatformID:%s",(*g_itrBDA2)->m_sServicePlatformID);
		
			
		
			// ����ҵ���߼�����
			iRet = m_poSql->BindingAccNbrSelect(pUserSelfService,oExp);
			
			tTime = time(NULL);
			tTime += 2209017599;
			long2date(tTime,sResponseTime);
			strcpy(oSvc.m_responseTime,sResponseTime);
		
			strcpy(oSvc.m_responseId,pUserSelfService.m_requestId);
			strcpy(oSvc.m_randomAccNbr,pUserSelfService.m_randomAccNbr);
			
			__DEBUG_LOG1_(0, "m_responseTime:%s",oSvc.m_responseTime);
			__DEBUG_LOG1_(0, "m_responseId:%s",oSvc.m_responseId);
			__DEBUG_LOG1_(0, "m_randomAccNbr:%s",oSvc.m_randomAccNbr);

		}
		
		// ��� - ��һ�������Ϣ
		if (!m_poPack->addA_BindingAccNbrQuery(oSvc))
		{
			__DEBUG_LOG0_(0, "BindingAccNbrQuery:: _deal m_poPack ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_PACK_ERR;
		}
		// ���ԣ������oSvc����
		__DEBUG_LOG1_(0, "��һ�����CCA������,oSvc,responseId:[%s]",oSvc.m_responseId);
		
	}
	catch(ABMException &oExp)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG0_(0, "ABMException....");
	}
	catch(int &value ) 
	{

		iRet=value;
		oSvc.m_svcResultCode=iRet;
		__DEBUG_LOG1_(0, "CCA,ҵ�񼶵Ľ������iRet:[%d]",iRet);
	}

	return iRet;	
}

