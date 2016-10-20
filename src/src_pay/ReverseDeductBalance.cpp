// ReverseDeductBalance.cpp
#include "ReverseDeductBalance.h"
#include "AllocateBalanceSql.h"

#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "PublicConndb.h"
#include "errorcode_pay.h"

static vector<ReqReverseDeductSvc *> g_vReqReverseDeductSvc;
static vector<ReqReverseDeductSvc *>::iterator g_itr;

extern TimesTenCMD *m_poBalance;

ReverseDeductBalance::ReverseDeductBalance()
{
		m_poSql = NULL;
		m_poUnpack = NULL;
		m_poPack = NULL;
		m_paymentInfo = new PaymentInfo;
		m_dccOperation = new DccOperation;
}

ReverseDeductBalance::~ReverseDeductBalance()
{
	if (m_poSql != NULL)
		delete m_poSql;
	if (m_paymentInfo != NULL)
		delete m_paymentInfo;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int ReverseDeductBalance::init(ABMException &oExp)
{
	if ((m_poSql = new AllocateBalanceSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "ReverseDeductBalance::init malloc failed!");
		return -1;
	}
	return 0;
}

int ReverseDeductBalance::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>��������,�������ʼ<=============");
	
	int iRet=0;
	int iSize=0;

	m_poUnpack = (ReverseDeductBalanceCCR *)pCCR;
	m_poPack = (ReverseDeductBalanceCCA *)pCCA;
	
	m_poPack->clear();
	g_vReqReverseDeductSvc.clear();

	ResReverseDeductData oSvcData={0};

	//���յ�CCR��Ϣͷ��TT
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
		__DEBUG_LOG0_(0, "[��������-ReverseDeductBalanc]:������յĻ�����������CCR����Ϣͷ��Ϣ����TT!");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������-ReverseDeductBalanc]:������յĻ�����������CCR����Ϣͷ��Ϣʧ��!");
			throw SAVE_RECEVIE_DCC_CCR_ERR;
		}
		
		//��Ϣ��� - ��һ��
		if ((iSize=m_poUnpack->getReqReverseDeductSvc(g_vReqReverseDeductSvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "[��������-ReverseDeductBalanc]:��������::deal ���������Ϣ��ΪNULL");
			m_poPack->setRltCode(ECODE_NOMSG);
			throw UNPACK_REVERSE_DEDUCT_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "[��������-ReverseDeductBalanc]:��������::deal ��Ϣ�������!");
			m_poPack->setRltCode(ECODE_UNPACK_FAIL);
			throw UNPACK_REVERSE_DEDUCT_ERR;
		}
	
		__DEBUG_LOG0_(0, "[��������-ReverseDeductBalanc]:���û�������ҵ���߼�����!");
		// �����һ����Ϣ
		for (g_itr=g_vReqReverseDeductSvc.begin(); g_itr!=g_vReqReverseDeductSvc.end(); ++g_itr)
		{			 
			__DEBUG_LOG0_(0, "[��������-ReverseDeductBalanc]:�����һ����Ϣ!");	
			// ����CCR������Ϣִ��ҵ���߼�����
			iRet = _deal(*g_itr);
			if (iRet != 0)
			{
				strcpy(oSvcData.m_sResponseId,(*g_itr)->m_sRequestId);
				break;
			}
		}
	}
	catch(TTStatus oSt)
	{
		iRet = REVERSE_DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "[��������-ReverseDeductBalanc]:��������::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��������-ReverseDeductBalanc]:��������::deal ҵ�񼶷��ؽ��iRet=%d",value);
	}
	
	if(iRet != 0)
	{
		oSvcData.m_iServiceRltCode=iRet;
		
		time_t tTime;
		tTime = time(NULL);
		oSvcData.m_iResponseTime = tTime;
		if (!m_poPack->addResReverseDeductData(oSvcData))
		{
			__DEBUG_LOG0_(0, "ReverseDeductBalance:: _deal CCA������Ϣ���ʧ��!");
			m_poPack->clear();
			m_poPack->setRltCode(ECODE_TT_FAIL);
		}
	}
	
	return iRet;
}

/*
 * ��������ҵ���߼�����
 *
 */
int ReverseDeductBalance::_deal(ReqReverseDeductSvc *pSvcIn)
{
	int iRet=0;
	
	if(pSvcIn->m_iRequestTime > 0)
		long2date(pSvcIn->m_iRequestTime,pSvcIn->m_sRequestTime);
	else
		GetTime(pSvcIn->m_sRequestTime);
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================������������=======================");
	__DEBUG_LOG1_(0,"===>����������������ˮ��oSvcCCR.m_sRequestId=[%s]",pSvcIn->m_sRequestId);
	__DEBUG_LOG1_(0,"===>������������������ˮ��oSvcCCR.m_sOldRequestId=[%s]",pSvcIn->m_sOldRequestId);
	__DEBUG_LOG1_(0,"===>��������������ʱ��[����]��oSvcCCR.m_iRequestTime=[%u]",pSvcIn->m_iRequestTime);
	__DEBUG_LOG1_(0,"===>��������������ʱ��[����]��oSvcCCR.m_sRequestTime=[%s]",pSvcIn->m_sRequestTime);
	__DEBUG_LOG1_(0,"===>��������������ABMƽ̨��ʶ��oSvcCCR.m_sABMId=[%s]",pSvcIn->m_sABMId);
	__DEBUG_LOG1_(0,"===>���������������������ͣ�oSvcCCR.m_iActionType=[%d]",pSvcIn->m_iActionType);
	
	ReqReverseDeductSvc oRvsInfo={0};
	
	strcpy(oRvsInfo.m_sRequestId,pSvcIn->m_sRequestId);
	strcpy(oRvsInfo.m_sOldRequestId,pSvcIn->m_sOldRequestId);
	oRvsInfo.m_iRequestTime = pSvcIn->m_iRequestTime;
	strcpy(oRvsInfo.m_sRequestTime,pSvcIn->m_sRequestTime);
	strcpy(oRvsInfo.m_sABMId,pSvcIn->m_sABMId);
	oRvsInfo.m_iActionType = pSvcIn->m_iActionType;
	
	ResReverseDeductData oSvcData={0};
	try 
	{
		iRet = reverseDeduct(oRvsInfo);
		
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[��������-ReverseDeductBalanc]:ҵ����ʧ��!");
			throw REVERSE_DEDUCT_BIZ_ERR;
		}
		
		__DEBUG_LOG0_(0, "[��������-ReverseDeductBalanc]:CCA������Ϣ���!");
		
		// ��������
		//strcpy(oSvcData.m_sResponseId,"6684804");
		//oSvcData.m_iServiceRltCode=0;
		//strcpy(oSvcData.m_sResponseTime,"2011-5-18");
		
		// ����CCA��ֵ
		strcpy(oSvcData.m_sResponseId,pSvcIn->m_sRequestId);
		oSvcData.m_iServiceRltCode=iRet;
		time_t tTime;
		
		tTime = time(NULL);
		oSvcData.m_iResponseTime = tTime+2209017599;
		
		__DEBUG_LOG1_(0, "[��������-��Ӧʱ��:[%u]",oSvcData.m_iResponseTime);
		// ͨ��ת������ȡ��ǰʱ������
		// date2long(oSvcData.m_iResponseTime);
		
		// ��� - ��һ��CCA��Ӧ������Ϣ
		if (!m_poPack->addResReverseDeductData(oSvcData))
		{
			__DEBUG_LOG0_(0, "ReverseDeductBalance:: _deal CCA������Ϣ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw REVERSE_DEDUCT_PACK_ERR;
		}
		
		__DEBUG_LOG1_(0, "[��������-ReverseDeductBalanc]:����CCA������,oSvc:%s",oSvcData.m_sResponseId);
	}
	catch(TTStatus oSt)
	{
		iRet = REVERSE_DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "[��������-ReverseDeductBalanc]:ReverseDeductBalance::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��������-ReverseDeductBalanc]:ReverseDeductBalance::deal value=%d",value);
	}	

	return iRet;	
}


/*
 * ������������
 */
int ReverseDeductBalance::reverseDeduct(ReqReverseDeductSvc &oRvsInfo)
{
	bool bFlag=false;
	int iRet=0;
	long lDeductAmount=0L;
	long m_lBalance=0L;
	char sOldRequestId[UTF8_MAX]={0};

	// ��������ҵ����ˮ�ţ�oRvsInfo.m_sOldRequestId
	//string sOldRequestId = oRvsInfo.m_sOldRequestId;
	strcpy(sOldRequestId,oRvsInfo.m_sOldRequestId);
	__DEBUG_LOG1_(0, "ReverseDeductBalance::sOldRequestId=[%s]",oRvsInfo.m_sOldRequestId);
	//__DEBUG_LOG1_(0, "ReverseDeductBalance::sOldRequestId=[%s]",sOldRequestId.c_str());
	vector<BalancePayoutInfo*>  vBalancePayOut;
	try
	{
		//��������ҵ�����߼�
		// 1.���ݱ������Ļ�����ˮ�ţ�����ҵ���¼����ȡ�����ܽ��
		//m_paymentInfo->qryOptInfo();
		
		// 2.����ҵ���¼��ˮ�������֧����¼	
	
		// ��������
		iRet = m_paymentInfo->ReverseDeductBalance(sOldRequestId,oRvsInfo);
		if(iRet !=0)
		{
			throw REVERSE_DEDUCT_BIZ_ERR;
			__DEBUG_LOG0_(0, "��������ҵ����ʧ��");
		}	
	}
	catch(TTStatus oSt) 
	{
		iRet=REVERSE_DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "ReverseDeductBalance::reversePay oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "ReverseDeductBalance::reversePay value=%d",value);
	}	
	
	return iRet;
}

