#include "AllocateBalance.h"
#include "AllocateBalanceSql.h"

#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "errorcode_pay.h"

static vector<R_AllocateBalanceSvc *> g_vRAllocateBalanceSvc;
static vector<R_AllocateBalanceSvc *>::iterator g_itr1;

static vector<R_RequestDebitAcctItem *> g_vRRequestDebitAcctItem;
static vector<R_RequestDebitAcctItem *>::iterator g_itr2;

extern TimesTenCMD *m_poBalance;

AllocateBalance::AllocateBalance()
{
		m_poSql = NULL;
		m_poUnpack = NULL;
		m_poPack = NULL;
		m_paymentInfo = new PaymentInfo;
		m_dccOperation = new DccOperation;
}

AllocateBalance::~AllocateBalance()
{
	if (m_poSql != NULL)
		delete m_poSql;
	if (m_paymentInfo != NULL)
		delete m_paymentInfo;
	if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int AllocateBalance::init(ABMException &oExp)
{
	if ((m_poSql = new AllocateBalanceSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "AllocateBalance::init malloc failed!");
		return -1;
	}
	return 0;
}

int AllocateBalance::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	__DEBUG_LOG0_(0, "=========>���ԣ�AllocateBalance::deal �������ʼ");
	
	int iRet=0;
	int iSize=0;

	m_poUnpack = (AllocateBalanceCCR *)pCCR;
	m_poPack = (AllocateBalanceCCA *)pCCA;
	
	m_poPack->clear();
	g_vRAllocateBalanceSvc.clear();
	g_vRRequestDebitAcctItem.clear();
	
	A_AllocateBalanceSvc oSvc={0} ;

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
		__DEBUG_LOG0_(0, "����ABM��������,����CCR��,��Ϣͷ��TT!");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������յ�CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_RECEVIE_DCC_CCR_ERR;
		}
		
		__DEBUG_LOG0_(0, "����ABM��������,CCR��Ϣ�����!");
		//��Ϣ��� - ��һ��
		if ((iSize=m_poUnpack->getR_AllocateBalanceSvc(g_vRAllocateBalanceSvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "����ABM��������::deal ���������Ϣ��ΪNULL");
			m_poPack->setRltCode(ECODE_NOMSG);
			throw DEDUCT_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "����ABM��������::deal ��Ϣ�������!");
			m_poPack->setRltCode(ECODE_UNPACK_FAIL);
			throw DEDUCT_UNPACK_ERR;
		}
	
		// ��ѯ�������� 
		__DEBUG_LOG0_(0, "��ѯ��������!");	
		
		// �����һ����Ϣ
		for (g_itr1=g_vRAllocateBalanceSvc.begin(); g_itr1!=g_vRAllocateBalanceSvc.end(); ++g_itr1)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack->getR_RequestDebitAcctItem(*g_itr1,g_vRRequestDebitAcctItem)) == 0 )
			{
				__DEBUG_LOG0_(0, "AllocateBalance::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack->setRltCode(ECODE_NOMSG);
				throw DEDUCT_UNPACK_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "AllocateBalance::deal ��Ϣ�������!");
				m_poPack->setRltCode(ECODE_UNPACK_FAIL);
				throw DEDUCT_UNPACK_ERR;
			}
			 
			__DEBUG_LOG0_(0, "����:�����һ����Ϣ........");	
			// ����	
			/*
			if (_deal(*g_itr1,g_vRRequestDebitAcctItem) != 0)
			break;
			*/
			iRet = _deal(*g_itr1,g_vRRequestDebitAcctItem);
			// ���������������������У�飬��������	
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "���ı�����������ʧ��");
				throw iRet;
			}
		}
		//return iRet;
	}
	catch(TTStatus oSt)
	{
		iRet = DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "AllocateBalance::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet=value;
		__DEBUG_LOG1_(0, "����ABM��������ʧ��:: ʧ����Ϣ value=%d", value);
	}
	if(iRet != 0)
	{
		// �򷵻�CCA��,���ش�����
		oSvc.m_svcResultCode=iRet;
		if (!m_poPack->addResBalanceSvcData(oSvc))
		{
			__DEBUG_LOG0_(0, "AllocateBalance:: _deal m_poPack ���ʧ��!");
			m_poPack->clear();
			m_poPack->setRltCode(ECODE_PACK_FAIL);
		}
		__DEBUG_LOG0_(0, "����ABM��������ʧ��,iRet=[%d]",oSvc.m_svcResultCode);
		
	}
	return iRet;
}

/*
 * �����������
 * ---��ѯ��������У��
 * ---�����൱�������е���ȡ����
 * ---
 * ---��ҵ����ˮ��¼�����������Դ��¼����������ʱ���¼
 */
int AllocateBalance::_deal(R_AllocateBalanceSvc *pSvcIn,vector<R_RequestDebitAcctItem *> g_vRRequestDebitAcctItem)
{
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================��������=======================");
	__DEBUG_LOG0_(0, "=====>start,ȡ��һ��CCR������Ϣ:");
	
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:%s",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "�û����룺m_servNbr��%s",pSvcIn->m_servNbr);
	__DEBUG_LOG1_(0, "�������ͣ�m_nbrType��%d",pSvcIn->m_nbrType);
	__DEBUG_LOG1_(0, "�û����ԣ�m_servAttr��%d",pSvcIn->m_servAttr);
	
	__DEBUG_LOG1_(0, "�绰���ţ�m_areaCode��%s",pSvcIn->m_areaCode);
	
	if(pSvcIn->m_nbrType==2)
	{
		if(!(pSvcIn->m_servAttr==2 || pSvcIn->m_servAttr==99))
			snprintf(pSvcIn->m_servNbr,sizeof(pSvcIn->m_servNbr),"%s%s\0",pSvcIn->m_areaCode,pSvcIn->m_servNbr);
		__DEBUG_LOG1_(0, "�����ŵ��û����룺m_servNbr��%s",pSvcIn->m_servNbr);
	}
	
	ABMException oExp;
		
	int iRet=0;
	char sBuffer[256];
	
	PaymentInfoALLCCR pPaymentInfoCCR={0};
	PaymentInfoALLCCA pPaymentInfoCCA={0};
	
	// ����CCR��Ϣ׼��	
	strcpy(pPaymentInfoCCR.m_sOperateSeq,pSvcIn->m_requestId);		// ������ˮ��
	strcpy(pPaymentInfoCCR.m_sDestinationAccount,pSvcIn->m_servNbr);	// �û�����
	pPaymentInfoCCR.m_iDestinationIDtype = pSvcIn->m_nbrType;		// ��������
	//strcpy(pPaymentInfoCCR.m_sDestinationIDtype,pSvcIn->m_nbrType);	// ��������
	pPaymentInfoCCR.m_iDestinationAttr = pSvcIn->m_servAttr;		// �û�����
	strcpy(pPaymentInfoCCR.m_sAreaCode,pSvcIn->m_areaCode);			// �绰����
	pPaymentInfoCCR.m_iStaffID = pSvcIn->m_staffId;				// ����ԱID
	strcpy(pPaymentInfoCCR.m_sDeductDate,pSvcIn->m_deductDate);		// �۷�ʱ��
	//pPaymentInfoCCR.m_iBalance_unit = pSvcIn->m_balanceUnit;		// ��λ-��
	strcpy(pPaymentInfoCCR.m_sMicropayType,"0");
	strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0004");
	
			
	// ����۷�ABM��ʶ
	A_AllocateBalanceSvc oSvc={0} ;	// CCA ��Ӧ������Ϣ
	A_ResponseDebitAcctItem oData={0} ;	// CCA ��Ӧ�۷���Ŀ��ϸ��Ϣ
	vector<struct A_ResponseDebitAcctItem> vResAcctItem;
		
	// ����CCR�е��û�����ͺ������Ͳ�ѯ�û����ʻ�������Ϣ
	ServAcctInfo oServAcct={0};
	
	bool bHaveRules = true;
	try
	{	
		__DEBUG_LOG1_(0, "=====>��ѯ�û���Ϣ��m_servNbr=%s.",pSvcIn->m_servNbr);
		__DEBUG_LOG1_(0, "=====>��ѯ�û���Ϣ��m_nbrType=%d.",pSvcIn->m_nbrType);
		iRet = m_poSql->preQryServAcctInfo(pSvcIn->m_servNbr,pSvcIn->m_nbrType,oServAcct,oExp);
		if(iRet != 0 )
		{
			if(iRet == 1)
			{
				__DEBUG_LOG0_(0, "=====>�û���Ϣ������");
				throw DEDUCT_SERV_NOT_EXIST;
			}
			__DEBUG_LOG0_(0, "=====>�û���Ϣ��ѯʧ��");
			throw DEDUCT_SERV_QRY_ERR;
		}
		// �û���Ϣ��ѯ���
		__DEBUG_LOG1_(0, "=====>�û�������Ϣ,�ʻ���ʶ��ACCT_ID=%d.",oServAcct.m_acctId);
		__DEBUG_LOG1_(0, "=====>�û�������Ϣ,�û���ʶ��SERV_ID=%d.",oServAcct.m_servId);
		__DEBUG_LOG1_(0, "=====>�û�������Ϣ,�û����룺ACCT_NBR=%s.",oServAcct.m_servNbr);
		
		pPaymentInfoCCR.m_lServID = oServAcct.m_servId;
		
		int iCnt = 0;
		TransRule oTransRule={0};	// ��������
		TransRule oTransRuleDay={0};	// �շ���
		TransRule oTransRuleMonth={0};	// �·���
		for (g_itr2=g_vRRequestDebitAcctItem.begin(); g_itr2!=g_vRRequestDebitAcctItem.end(); ++g_itr2)
		{	
			__DEBUG_LOG0_(0, "=====>start,����ҵ���߼�����");
			
			// ���������ѯ��У��	
			iRet = m_poSql->preQryDeductRule(oServAcct.m_acctId,oTransRule,oExp);
			if(iRet != 0 )
			{
				if(iRet == 1)
				{
					__DEBUG_LOG0_(0, "=====>�û��������򲻴��ڡ�");
					bHaveRules =false;
					// throw TRANSFER_RULUES_NO_EXIT;
				}
				else
				{
					__DEBUG_LOG0_(0, "=====>�û����������ѯʧ��");
					throw TRANSFER_RULUES_QRY_ERR;
				}
			}
			__DEBUG_LOG1_(0, "=====>����������Ϣ,�ʻ���ʶ��ACCT_ID=%d.",oTransRule.lAcctId);
			__DEBUG_LOG1_(0, "=====>����������Ϣ,�ջ���Ƶ�ȣ�lDayTransFreq=%d.",oTransRule.lDayTransFreq);
			__DEBUG_LOG1_(0, "=====>����������Ϣ,�ջ����޶lTransLimit=%d.",oTransRule.lTransLimit);
			__DEBUG_LOG1_(0, "=====>����������Ϣ,�»���Ƶ�ȣ�lMonthTransLimit=%d.",oTransRule.lMonthTransLimit);
				
			// ͳ�Ƶ�ǰ�����·��������������������
			iRet = m_poSql->preQryServDayDeductInfo(oServAcct.m_servId,oTransRuleDay,oTransRuleMonth,oExp);
			__DEBUG_LOG1_(0, "=====>�û��ѻ�����Ϣ�����������lDayTransFreq=%d.",oTransRuleDay.lDayTransFreq);
			__DEBUG_LOG1_(0, "=====>�û��ѻ�����Ϣ�������lTransLimit=%d.",oTransRuleDay.lTransLimit);	
			__DEBUG_LOG1_(0, "=====>�û��ѻ�����Ϣ�����½�lMonthTransLimit=%d.",oTransRuleMonth.lMonthTransLimit);
			
			if(bHaveRules)
			{// ���ڹ���,����У��,�޹��򣬲�У��	
				// ��������У��
				__DEBUG_LOG0_(0, "��������У��");
				if(oTransRuleDay.lDayTransFreq >= oTransRule.lDayTransFreq)
				{	// �����ѷ������� > ÿ�컮��Ƶ��
					__DEBUG_LOG0_(0, "�����ѷ������� > ÿ�컮��Ƶ��");
					throw OVER_DAY_TRANS_FREQ;
				}
				else if(oTransRuleDay.lTransLimit >= oTransRule.lTransLimit)
				{	// �������ѷ������ > ÿ�컮���޶�
					__DEBUG_LOG0_(0, "�������ѷ������ > ÿ�컮���޶�");
					throw OVER_DAY_TRANS_LIMIT;
				}
				else if(oTransRuleMonth.lMonthTransLimit >= oTransRule.lMonthTransLimit)
				{	// �����ѷ������ > ÿ�»����޶�
					
					__DEBUG_LOG0_(0, "�����ѷ������ > ÿ�»����޶�");
					throw OVER_MONTH_TRANS_LIMIT;
				}
			}	
			__DEBUG_LOG0_(0, "=====>end,����ҵ���߼�����.");
			
			// ���Դ��룺׼������CCA���ݣ���һ�������Ϣ
			strcpy(oSvc.m_responseId, "6684801");	// ��Ӧ��ˮ
			oSvc.m_svcResultCode = 1;		// ҵ�񼶵Ľ������
			oSvc.m_balanceUnit = 0;			// ��λ-��
			oSvc.m_deductAmount = 10;		// �ɹ��������
			oSvc.m_acctBalance = 0;			// �ʻ�ʣ����
			
			// ���Դ��룺׼������CCA���ݣ��ڶ���۷���Ŀ������ϸ��Ϣ	
			oData.m_billingCycle=201105;		// ��������YYYYMMDD	
			strcpy(oData.m_acctItemTypeId, "2");	// ��Ŀ���ͱ�ʶ	
			oData.m_acctBalanceId = 888;		// �۷��ʱ���ʶ
			oData.m_deductAmount = 10;		// �ʱ��������
			oData.m_acctBalanceAmount = 10;		// �ʱ�ʣ����	
				
				
			
			//strcpy(pPaymentInfoCCR.m_sOperateSeq,"66848001");	// ������ˮ��
			pPaymentInfoCCR.m_iStaffID = 66848;
			strcpy(pPaymentInfoCCR.m_sDestinationAccount,oServAcct.m_servNbr);
			strcpy(pPaymentInfoCCR.m_sDestinationtype,"2");
			pPaymentInfoCCR.m_iDestinationAttr = 99;
	
			pPaymentInfoCCR.m_lCharge_type=0;
			pPaymentInfoCCR.m_iBalance_unit=0;
			strcpy(pPaymentInfoCCR.m_sAreaCode,"025");
			strcpy(pPaymentInfoCCR.m_sDescription,"����");
			
			pPaymentInfoCCR.m_lPayAmount = 0 - (*g_itr2)->m_deductAmount;		// �����������¼��¼�����
			strcpy(pPaymentInfoCCR.m_sAcctItemTypeID,(*g_itr2)->m_acctItemTypeId);	// ��Ŀ����
			pPaymentInfoCCR.m_lCharge_type = (*g_itr2)->m_chargeType;		// ����һ�ο���
			
			// ����ҵ����
			__DEBUG_LOG0_(0, "=====>start,m_paymentInfo->RecodePaymentInfo.");
			
			if((*g_itr2)->m_deductAmount <= 0)
			{
				__DEBUG_LOG0_(0, "�������С�ڵ���0,��Ч.");
				throw DEDUCT_AMOUNT_ERR;
			}
			
			// ����ҵ���߼�����
			strcpy(pPaymentInfoCCR.m_sOrderState,"C0C");	
			iRet = m_paymentInfo->DeductBalanceInfo(pPaymentInfoCCR,pPaymentInfoCCA);
			
			if(iRet!=0)
			{	// ����ʱ�����
				__DEBUG_LOG0_(0, "=====>start,m_paymentInfo->RecodePaymentInfo.");
				throw BALANCE_NOT_DEDUCT;
			}
			__DEBUG_LOG0_(0, "=====>end,m_paymentInfo->RecodePaymentInfo.");
			if(iCnt==0)
			{	// CCA������Ϣ��ֵ-��Ӧ����,ֻ��¼һ��
				strcpy(oSvc.m_responseId,pPaymentInfoCCR.m_sOperateSeq);	// ��Ӧ��ˮ
				oSvc.m_svcResultCode = pPaymentInfoCCA.m_iSvcResultCode;	// ҵ�񼶵Ľ������
				oSvc.m_balanceUnit = pPaymentInfoCCR.m_iBalance_unit;		// ��λ-��
				oSvc.m_deductAmount = pPaymentInfoCCA.m_lDeductAmountAll;	// �ɹ��������
				oSvc.m_acctBalance = pPaymentInfoCCA.m_lAcctBalance;		// �ʻ�ʣ����
				
				__DEBUG_LOG1_(0, "CCA������Ϣ,��Ӧ��ˮ��[%s]",oSvc.m_responseId);
				__DEBUG_LOG1_(0, "CCA������Ϣ,ҵ�񼶵Ľ�����룺[%d]",oSvc.m_svcResultCode);
				__DEBUG_LOG1_(0, "CCA������Ϣ,��λ��[%d]",oSvc.m_balanceUnit);
				__DEBUG_LOG1_(0, "CCA������Ϣ,�ɹ�������[%ld]",oSvc.m_deductAmount);
				__DEBUG_LOG1_(0, "CCA������Ϣ,�ʻ�ʣ���[%ld]",oSvc.m_acctBalance);
				__DEBUG_LOG1_(0, "CCA������Ϣ,AVP code �����룺[%s]",oSvc.m_sParaFieldResult);
			}
			iCnt++;
			
			// CCA������Ϣ��ֵ-�۷���ϸ
			oData.m_billingCycle=(*g_itr2)->m_billingCycle;			// ��������YYYYMM	
			strcpy(oData.m_acctItemTypeId,(*g_itr2)->m_acctItemTypeId);	// ��Ŀ���ͱ�ʶ	
			oData.m_acctBalanceId = pPaymentInfoCCA.m_lAcctBalanceId;	// �۷��ʱ���ʶ
			oData.m_deductAmount = pPaymentInfoCCA.m_lDeductAmountAll;	// �ʱ��������
			oData.m_acctBalanceAmount = pPaymentInfoCCA.m_lAcctBalanceAmount;	// �ʱ�ʣ����	
			//strcpy(oData.m_sParaFieldResult,"0");	// AVP code - Error	// ����Ĭ��д��
			
			vResAcctItem.push_back(oData);
			__DEBUG_LOG1_(0, "CCA������Ϣ,��������YYYYMM��[%d]",oData.m_billingCycle);
			__DEBUG_LOG1_(0, "CCA������Ϣ,��Ŀ���ͱ�ʶ��[%s]",oData.m_acctItemTypeId);
			__DEBUG_LOG1_(0, "CCA������Ϣ,�۷��ʱ���ʶ��[%d]",oData.m_acctBalanceId);
			__DEBUG_LOG1_(0, "CCA������Ϣ,�ʱ�������[%d]",oData.m_deductAmount);
			__DEBUG_LOG1_(0, "CCA������Ϣ,�ʱ�ʣ���[%d]",oData.m_acctBalanceAmount);
		}
		
		// ��� - ��һ�������Ϣ
		if (!m_poPack->addResBalanceSvcData(oSvc))
		{
			__DEBUG_LOG0_(0, "AllocateBalance:: _deal m_poPack ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_PACK_ERR;
		}
		// ���ԣ������oSvc����
		__DEBUG_LOG1_(0, "��һ�����CCA������,oSvc,responseId:[%s]",oSvc.m_responseId);
		
		// ��� - �ڶ���۷���ϸ
		for(int j=0;j<vResAcctItem.size();j++)
		{
			if (!m_poPack->addResDebitAcctData(vResAcctItem[j]))
			{
				__DEBUG_LOG0_(0, "AllocateBalance:: _deal m_poPack ���ʧ��!");
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

