/*
 * ֧������
 */
#include "ReversePay.h"

#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "errorcode_pay.h"

static vector<ReversePaySvc *> g_vReversePaySvc;
static vector<ReversePaySvc *>::iterator g_itr;

ReversePay::ReversePay()
{
	m_poUnpack = NULL;
	m_poPack = NULL;
	
	m_paymentInfo = new PaymentInfo;
	m_dccOperation = new DccOperation;
}

ReversePay::~ReversePay()
{
	if (m_paymentInfo != NULL)
		delete m_paymentInfo;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int ReversePay::init(ABMException &oExp)
{
	return 0;
}

int ReversePay::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>֧����������,�������ʼ<=============");
	
	int iRet=0;
	int iSize=0;
	m_poUnpack = (ReversePayCCR *)pCCR;
	m_poPack = (ReversePayCCA *)pCCA;
	
	m_poPack->clear();
	g_vReversePaySvc.clear();

	//��Ϣ���
	if ((iSize=m_poUnpack->getReversePaySvc(g_vReversePaySvc)) == 0)
	{
		__DEBUG_LOG0_(0, "ReversePay::deal ��Ϣ��ΪNULL");
		m_poPack->setRltCode(ECODE_NOMSG);
		return -1;
	}
	else if (iSize < 0)
	{
		__DEBUG_LOG0_(0, "ReversePay::deal ��Ϣ�������!");
		m_poPack->setRltCode(ECODE_UNPACK_FAIL);
		return -1;
	}

	try
	{
		__DEBUG_LOG0_(0, "����֧������ҵ���߼�����..............");
		for (g_itr=g_vReversePaySvc.begin(); g_itr!=g_vReversePaySvc.end(); ++g_itr) 
		{
			if (_deal(*g_itr) != 0)
			break;
		}
		return 0;
	}
	catch(TTStatus oSt)
	{
		__DEBUG_LOG1_(0, "ReversePay::deal oSt.err_msg=%s", oSt.err_msg);
	}
	m_poPack->clear();
	m_poPack->setRltCode(ECODE_TT_FAIL);
	
	return 0;
}

int ReversePay::_deal(ReversePaySvc *pSvcIn)
{
	int iRet=0;
	// ����--ȡ��һ��CCR��Ϣ
	__DEBUG_LOG0_(0, "========================֧����������=======================");
	__DEBUG_LOG1_(0,"===>oSvcCCR.lOperationAction=[%ld]",pSvcIn->lOperationAction);
	__DEBUG_LOG1_(0,"===>oSvcCCR.sReqSerial=[%s]",pSvcIn->sReqSerial);
	__DEBUG_LOG1_(0,"===>oSvcCCR.sSpId=[%s]",pSvcIn->sSpId);
	__DEBUG_LOG1_(0,"===>oSvcCCR.sSpName=[%s]",pSvcIn->sSpName);
	__DEBUG_LOG1_(0,"===>oSvcCCR.sServicePlatformId=[%s]",pSvcIn->sServicePlatformId);
	__DEBUG_LOG1_(0,"===>oSvcCCR.sStaffId=[%d]",pSvcIn->iStaffId);
	
	
	// ��ȡCCR������Ϣ
	ReversePaySvc reverseBizInfo={0};
	
	reverseBizInfo.lOperationAction = pSvcIn->lOperationAction;	// 
	strcpy(reverseBizInfo.sOperationSeq,pSvcIn->sOperationSeq);	// ������ˮ
	strcpy(reverseBizInfo.sReqSerial,pSvcIn->sReqSerial);	// ��Ҫȡ���۷ѵ�ԭ֧��������ˮ��
	strcpy(reverseBizInfo.sSpId,pSvcIn->sSpId);			// �̼�ID
	strcpy(reverseBizInfo.sSpName,pSvcIn->sSpName);			// �̼�����
	strcpy(reverseBizInfo.sServicePlatformId,pSvcIn->sServicePlatformId);// ƽ̨ID
	reverseBizInfo.iStaffId = pSvcIn->iStaffId;
	
	// ֧������ҵ���߼�����
	iRet = reversePay(reverseBizInfo);
	__DEBUG_LOG0_(0, "iRet = reversePay(reverseBizInfo),iRet = [%d]",iRet);
	if(iRet !=0)
	{
		__DEBUG_LOG0_(0, "========================֧����������ʧ��=======================");
		return -1;
	}
	
	ReversePayRes1 oSvc;			// CCA1
	vector<ReversePayRes2> vReversePayRes2; // CCA2
	
	// ��ҵ��������ֵ��CCA
	strcpy(oSvc.sParaFieldResult,"avp-err-ccr-code");
	
	ReversePayRes2 reversePayRes2;
	strcpy(reversePayRes2.sOperateSeq,reverseBizInfo.sOperationSeq); // ����������ˮ
	strcpy(reversePayRes2.sReqSerial,reverseBizInfo.sReqSerial);  // ��������ˮ��
	strcpy(reversePayRes2.sParaFieldResult,"avp-err-cca-code");
	
	vReversePayRes2.push_back(reversePayRes2);
	
	// CCA��Ϣ���
	// ��� - ��һ�������Ϣ
	if (!m_poPack->addReversePayCCA1(oSvc))
	{
		__DEBUG_LOG0_(0, "ReversePay:: _deal m_poPack ���ʧ��!");
		m_poPack->setRltCode(ECODE_PACK_FAIL);
		return -1;
	}
	
	// ���ԣ������oSvc����
	__DEBUG_LOG1_(0, "��һ�����CCA�����ɣ�oSvc��%d",oSvc.lServiceRltCode);
	
	
	// ��� - �ڶ�����Ϣ
	for(int j=0;j<vReversePayRes2.size();j++)
	{
		if (!m_poPack->addReversePayCCA2(vReversePayRes2[j]))
		{
			__DEBUG_LOG0_(0, "ReversePay:: _deal m_poPack ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			return -1;
		}	
	}
	
	__DEBUG_LOG0_(0, "�ڶ������CCA������.");
	
	
	return 0;
}


int ReversePay::reversePay(ReversePaySvc &oRvsInfo)
{
	bool bFlag=false;
	int iRet=0;
	long lDeductAmount=0L;
	long m_lBalance=0L;
	char sOldRequestId[64]={0};

	// ��������ҵ����ˮ�ţ�oRvsInfo.m_sOldRequestId
	strcpy(sOldRequestId,oRvsInfo.sReqSerial);
	__DEBUG_LOG1_(0, "��������ˮ�ţ�ReversePaySvc::sReqSerial=[%s]",oRvsInfo.sReqSerial);
	//__DEBUG_LOG1_(0, "ReversePaySvc::sReqSerial=[%s]",sOldRequestId.c_str());
	vector<BalancePayoutInfo*>  vBalancePayOut;
	try
	{
		//��������ҵ�����߼�
		// 1.���ݱ������Ļ�����ˮ�ţ�����ҵ���¼����ȡ�����ܽ��
		//m_paymentInfo->qryOptInfo();
		
		// 2.����ҵ���¼��ˮ�������֧����¼
		//long lOldRequestId = atol(sOldRequestId.c_str()); // ���Ͳ�ƥ�䣬��ʱд��
		
		// ���֧������
		iRet = m_paymentInfo->ReversePayoutInfo(sOldRequestId,oRvsInfo,vBalancePayOut);
		
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "ReverseDeductBalance::reversePay ȡ֧����Ϣ����");
		}
		__DEBUG_LOG1_(0, "===>֧����Ϣ,ҵ����ˮ��OPT_ID:%d",vBalancePayOut[0]->m_lOptId);
		__DEBUG_LOG1_(0, "===>֧����Ϣ,֧�����Amount:%d",vBalancePayOut[0]->m_lAmount);
		__DEBUG_LOG1_(0, "===>֧����Ϣ:%d",vBalancePayOut[0]->m_lBalance);
		
	}
	catch(TTStatus oSt) 
	{
		iRet=-1;
		__DEBUG_LOG1_(0, "ReverseDeductBalance::reversePay oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "ReverseDeductBalance::reversePay value=%d",value);
	}	
	
	return iRet;
}
