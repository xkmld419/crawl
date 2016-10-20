#include "ActiveAllocateBalance.h"
#include "AllocateBalanceSql.h"

#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "PublicConndb.h"
#include "errorcode_pay.h"

static vector<ReqPlatformSvc *> g_vReqPlatformSvc;
static vector<ReqPlatformSvc *>::iterator g_itr1;
	
static vector<ReqPlatformPayInfo *> g_vReqPlatformPayInfo;
static vector<ReqPlatformPayInfo *>::iterator g_itr2;

extern TimesTenCMD *m_poBalance;

ActiveAllocateBalance::ActiveAllocateBalance()
{
		m_poSql = NULL;
		m_poUnpack = NULL;
		m_poPack = NULL;
		m_poActivePack = NULL;
		m_paymentInfo = new PaymentInfo;
		
		m_dccOperation = new DccOperation;
		
		
}

ActiveAllocateBalance::~ActiveAllocateBalance()
{
	if (m_poSql != NULL)
		delete m_poSql;	
	if (m_paymentInfo != NULL)
		delete m_paymentInfo;
			
	if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int ActiveAllocateBalance::init(ABMException &oExp)
{
	if ((m_poSql = new AllocateBalanceSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "AllocateBalance::init malloc failed!");
		return -1;
	}
	
	return 0;
}

int ActiveAllocateBalance::deal(ABMCCR *pCCR, ABMCCA *pSendPack)
{
	__DEBUG_LOG0_(0, "=========ActiveAllocateBalance::deal �������ʼ============");
	__DEBUG_LOG0_(0, "[��������-�շ�CCR,ActiveAllocateBalance]���������Է��������CCR!");
	
	int iRet=0;
	int iSize=0;
	
	m_poUnpack = (PlatformDeductBalanceCCR *)pCCR; // �������CCR
	m_poPack = (AllocateBalanceCCR *)pSendPack;    // �������CCR
	
	m_poPack->clear();
	g_vReqPlatformSvc.clear();
	g_vReqPlatformPayInfo.clear();

	//��Ϣ��� - ��һ��
	if ((iSize=m_poUnpack->getReqPlatformSvc(g_vReqPlatformSvc)) == 0 )
	{
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:deal �Է���ƽ̨���󻮲������������Ϣ��ΪNULL!");
		m_poPack->setRltCode(ECODE_NOMSG);
		return UNPACK_PLATFORM_CCR_NOMSG;
	}
	else if (iSize < 0)
	{
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:deal ��Ϣ�������!");
		m_poPack->setRltCode(ECODE_UNPACK_FAIL);
		return UNPACK_PLATFORM_CCR_ERR;
	}
	
	try
	{		
		// �����һ����Ϣ
		for (g_itr1=g_vReqPlatformSvc.begin(); g_itr1!=g_vReqPlatformSvc.end(); ++g_itr1)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack->getReqPlatformPayInfo(*g_itr1,g_vReqPlatformPayInfo)) == 0 )
			{
				__DEBUG_LOG0_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:deal ������ϸ��Ϣ��ΪNULL!");
				m_poPack->setRltCode(ECODE_NOMSG);
				throw UNPACK_PLATFORM_CCR_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:deal ��Ϣ�������!");
				m_poPack->setRltCode(ECODE_UNPACK_FAIL);
				throw UNPACK_PLATFORM_CCR_ERR;
			}
			 
			__DEBUG_LOG0_(0, "=========>2��_deal.�����Է���ƽ̨�������CCR��Ϣ.");			
			// ����	
			iRet = 	_deal(*g_itr1,g_vReqPlatformPayInfo);	
			if (iRet != 0)
				break;
		}
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:deal iRet=%d",iRet);
	}
	catch(TTStatus oSt)
	{
		iRet = ACTIVE_DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,ActiveAllocateBalance],ҵ��������: iRet=%d",value);
	}
	
	// ҵ����ʧ�ܣ��򷵻ذ���TODO��
	if(iRet!=0)
	{
		m_poPack->clear();
		m_poPack->setRltCode(ECODE_TT_FAIL);
	}
	__DEBUG_LOG1_(0, "ActiveAllocateBalance::deal iRet=%d",iRet);
	return iRet;
}

// �������Ͳ���
int ActiveAllocateBalance::_deal(ReqPlatformSvc *pSvcIn,vector<ReqPlatformPayInfo *> g_vReqPlatformPayInfo)
{
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================��������=======================");
	__DEBUG_LOG0_(0, "=========>ȡ�Է���ƽ̨����CCR��һ�������Ϣ:");
	__DEBUG_LOG1_(0, "[��������-�շ�CCR,ActiveAllocateBalance],������ˮ��m_requestId:%s",pSvcIn->m_requestId);
	//__DEBUG_LOG1_(0, "[��������-�շ�CCR,ActiveAllocateBalance],����ʱ�䣺m_iRequestTime:%d",pSvcIn->m_iRequestTime);
	__DEBUG_LOG1_(0, "[��������-�շ�CCR,ActiveAllocateBalance],����ʱ�䣺m_requestTime:%s",pSvcIn->m_requestTime);
	
	/* Modify 2011.7.7. ȥ��ʱ��ת��
	if(pSvcIn->m_iRequestTime > 0)
		long2date(pSvcIn->m_iRequestTime,pSvcIn->m_requestTime);
	else
		GetTime(pSvcIn->m_requestTime);
	*/
	int iRet=0;
	int iBalanceTransFlag; // ��������
	
	try
	{
		for (g_itr2=g_vReqPlatformPayInfo.begin(); g_itr2!=g_vReqPlatformPayInfo.end(); ++g_itr2)
		{
		
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:ȡ�Է���ƽ̨����CCR�ڶ���֧����Ϣ��");
			 iBalanceTransFlag = (*g_itr2)->m_iBalanceTransFlag;
			__DEBUG_LOG1_(0, "[��������-�շ�CCR,ActiveAllocateBalance],�������ͣ�[%d](1-ʡ��ȫ����2-ȫ����ʡ)",iBalanceTransFlag);
			 switch(iBalanceTransFlag)
			 {
			 	case 1:
			 		// ʡ��ȫ��
			 		// ʡABM��������
			 		// ����ABM��������ֵҵ��
			 		__DEBUG_LOG0_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:������ʡ��ȫ��ҵ����");
			 		iRet = deductBalanceIn(pSvcIn,*g_itr2);
			 		if(iRet!=0)
			 		{
			 			throw CENTER_ABM_DEDUCT_IN_ERR;
			 			__DEBUG_LOG0_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:������ʡ��ȫ��ҵ����ʧ��");
			 		}
			 		break;
			 	case 2:
			 		// ȫ����ʡ
			 		// ����ABM��������
			 		// ����ABM������ҵ��
			 		__DEBUG_LOG0_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:������ȫ����ʡҵ����");
			 		 iRet = deductBalanceOut(pSvcIn,*g_itr2);
			 		if(iRet!=0)
			 		{
			 			throw CENTER_ABM_DEDUCT_OUT_ERR;
			 			__DEBUG_LOG0_(0, "[��������-�շ�CCR,ActiveAllocateBalance]:������ȫ����ʡҵ����ʧ��");
			 		}
			 		break;
			 }
		}
		
		for(int j=0;j<g_vReqPlatformPayInfo.size();j++)
		{
			delete g_vReqPlatformPayInfo[j];
			g_vReqPlatformPayInfo[j]= NULL;
		}
		g_vReqPlatformPayInfo.clear();
	}
	catch(TTStatus oSt) 
	{
		iRet=REVERSE_PAYMENT_TT_ERR;
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,ActiveAllocateBalance] oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,ActiveAllocateBalance] iRet=%d",value);
	}
	return iRet;	
}

// ������ֵ 
int ActiveAllocateBalance::deductBalanceIn(ReqPlatformSvc *pSvcIn,ReqPlatformPayInfo *g_itr2)
{
	int iRet=0;
	
	// ����CCR��Ϣ
	R_AllocateBalanceSvc reqAllocateBalanceSvc;
	vector<R_RequestDebitAcctItem> vResAcctItem;

	// ������ϸ
	R_RequestDebitAcctItem reqAllocateBalanceItem={0};
	// ��CCR��Ϣ
	__DEBUG_LOG0_(0, "========================����ʡABM��ֵCCR��TT=======================");
	__DEBUG_LOG0_(0, "[��������-�շ�CCR,������ֵ]-�Է�ƽ̨����CCR��ϸ:");
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:[%s]",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime:[%s]",pSvcIn->m_requestTime);
	
	__DEBUG_LOG1_(0, "�û����룺m_sDestinationAccount:[%s]",g_itr2->m_sDestinationAccount);
	__DEBUG_LOG1_(0, "��ʶ���ͣ�m_iDestinationType:[%d]",g_itr2->m_iDestinationType);
	__DEBUG_LOG1_(0, "�û����ԣ�m_iDestinationAttr:[%d]",g_itr2->m_iDestinationAttr);
	__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��m_sSvcPlatformId:[%s]",g_itr2->m_sSvcPlatformId);
	__DEBUG_LOG1_(0, "֧�����룺m_sPayPassword:[%s]",g_itr2->m_sPayPassword);
	__DEBUG_LOG1_(0, "�������ͣ�m_iBalanceTransFlag:[%d]",g_itr2->m_iBalanceTransFlag);
	__DEBUG_LOG1_(0, "���ĵ�λ���ͣ�m_iUnitTypeId:[%d]",g_itr2->m_iUnitTypeId);
	__DEBUG_LOG1_(0, "�軮����m_lDeductAmount:[%ld]",g_itr2->m_lDeductAmount);
	
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	try
	{	
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,������ֵ]:����CCR����Ϣͷ��Ϣ,��TT");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,������ֵ]:����CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_PLATFOR_CCR_ERR;
		}
		
		// dcc��ҵ��������Ϣ
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,������ֵ]:����CCR��ҵ����Ϣ����TT");
		iRet = m_dccOperation->savePlatformCCRBiz(receiveDccHead.m_sSessionId,pSvcIn,g_itr2);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,������ֵ]:����CCRҵ����Ϣʧ��");
			throw SAVE_PLATFOR_CCR_BIZ_ERR;
		}
	
		// ��CCR������Ϣ
		__DEBUG_LOG0_(0, "========================��CCR������Ϣ=======================");
		
		// ��������CCR
		// ��һ��CCR��ֵ
		strcpy(reqAllocateBalanceSvc.m_requestId,pSvcIn->m_requestId); // ������ˮ
		strcpy(reqAllocateBalanceSvc.m_servNbr,g_itr2->m_sDestinationAccount);// �û�����
		reqAllocateBalanceSvc.m_nbrType = 2;// ��������
		reqAllocateBalanceSvc.m_servAttr=g_itr2->m_iDestinationAttr; // �û�����
		//strcpy(reqAllocateBalanceSvc.m_areaCode, "010");// ����
		reqAllocateBalanceSvc.m_staffId=0; // ���ţ�д����ÿʡһ��
		//strcpy(reqAllocateBalanceSvc.m_deductDate,pSvcIn->m_requestTime); // ����ʱ��
		
		long lRequestTime=0L;
		//date2long(lRequestTime);
		//reqAllocateBalanceSvc.m_iDeductDate=lRequestTime; // ����ʱ��Ϊ��ǰʱ��
		
		time_t tTime;
		
		tTime = time(NULL);
		reqAllocateBalanceSvc.m_iDeductDate = tTime+2209017599;
		
		//reqAllocateBalanceSvc.m_iDeductDate = pSvcIn->m_iRequestTime;
		
		strcpy(reqAllocateBalanceSvc.m_abmId,g_itr2->m_sSvcPlatformId);  // ҵ��ƽ̨��ʶ
		
		__DEBUG_LOG1_(0, "������ˮ��reqAllocateBalanceSvc.m_requestId:[%s]",reqAllocateBalanceSvc.m_requestId);
		__DEBUG_LOG1_(0, "�û����룺reqAllocateBalanceSvc.m_servNbr:[%s]",reqAllocateBalanceSvc.m_servNbr);
		__DEBUG_LOG1_(0, "�������ͣ�reqAllocateBalanceSvc.m_nbrType:[%d]",reqAllocateBalanceSvc.m_nbrType);
		__DEBUG_LOG1_(0, "�û����ԣ�reqAllocateBalanceSvc.m_servAttr:[%d]",reqAllocateBalanceSvc.m_servAttr);
		//__DEBUG_LOG1_(0, "���ţ�reqAllocateBalanceSvc.m_areaCode:[%s]",reqAllocateBalanceSvc.m_areaCode);
		__DEBUG_LOG1_(0, "���ţ�reqAllocateBalanceSvc.m_staffId:[%d]",reqAllocateBalanceSvc.m_staffId);
		__DEBUG_LOG1_(0, "����ʱ�䣺reqAllocateBalanceSvc.m_iDeductDate:[%d]",reqAllocateBalanceSvc.m_iDeductDate);
		__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��reqAllocateBalanceSvc.m_abmId:[%s]",reqAllocateBalanceSvc.m_abmId);
		
		// �ڶ���CCR��ֵ
		// ȡ��ǰϵͳʱ����������
		time_t timep;
		struct tm *p;
		time(&timep);
		p=gmtime(&timep);
		char sBillCycle[9]={0};
		snprintf(sBillCycle,sizeof(sBillCycle),"%d%d%s\0",(1900+p->tm_year),(1+p->tm_mon),"01");
		
		reqAllocateBalanceItem.m_billingCycle = atol(sBillCycle);
		strcpy(reqAllocateBalanceItem.m_acctItemTypeId,"1005");
		reqAllocateBalanceItem.m_balanceUnit = g_itr2->m_iUnitTypeId;
		reqAllocateBalanceItem.m_deductAmount = g_itr2->m_lDeductAmount; // �軮���Ľ��
		reqAllocateBalanceItem.m_chargeType = 2;
		
		__DEBUG_LOG1_(0, "�������ڣ�reqAllocateBalanceItem.m_abmId:[%d]",reqAllocateBalanceItem.m_billingCycle);
		__DEBUG_LOG1_(0, "��Ŀ���ͱ�ʶ��reqAllocateBalanceItem.m_acctItemTypeId:[%s]",reqAllocateBalanceItem.m_acctItemTypeId);
		__DEBUG_LOG1_(0, "��λ��reqAllocateBalanceItem.m_balanceUnit:[%d]",reqAllocateBalanceItem.m_balanceUnit);
		__DEBUG_LOG1_(0, "�軮����reqAllocateBalanceItem.m_deductAmount:[%ld]",reqAllocateBalanceItem.m_deductAmount);
		__DEBUG_LOG1_(0, "�۷����ͣ�reqAllocateBalanceItem.m_chargeType:[%d]",reqAllocateBalanceItem.m_chargeType);
		
		vResAcctItem.push_back(reqAllocateBalanceItem);
		
		// ����CCR��Ϣͷ
		// DeductFromABM. Micropayment@<DomainName>.ChinaTelecom.com
		// ����Session-id
		long lSessionIdSeq;
		iRet = m_dccOperation->QueryPaySeq(lSessionIdSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "��ѯDCC�Ự��ʶSession-Idʧ��");
			throw QRY_SESSION_ID_ERR;
		}
		struct struDccHead Head;
		char sSessionId[64];
		
		tTime = time(NULL);
		// 2011.8.1
		snprintf(Head.m_sSessionId,sizeof(Head.m_sSessionId),"%s%010d%s%04d\0","ABM@001.ChinaTelecom.com;",tTime,";",lSessionIdSeq);
		
		//snprintf(Head.m_sSessionId,sizeof(Head.m_sSessionId),"%s;%s;%06d\0","ABM@001.ChinaTelecom.com",reqAllocateBalanceSvc.m_deductDate,lSessionIdSeq);
		
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,������ֵ]:SESSION_ID:[%s]",Head.m_sSessionId);
	
		//strcpy(Head.m_sSrvTextId,"DeductFromABM.Micropayment@025.ChinaTelecom.com");
		strcpy(Head.m_sOutPlatform,""); // �ⲿƽ̨��SGWʹ��
		strcpy(Head.m_sOrignHost,"ABM@001.ChinaTelecom.com"); // 2011.8.1
		strcpy(Head.m_sOrignRealm,"001.ChinaTelecom.com");
		
		// ��ȡ���󻮲�����Ĺ�����
		char sOrgId[4];
		char sLocalAreaId[4];
		iRet = m_dccOperation->qryAccountOrgId(reqAllocateBalanceSvc.m_servNbr,sOrgId,sLocalAreaId);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "��ѯ�û�������������ʧ��");
			throw QRY_SERV_ORGID_ERR;
		}
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,��������]:������������:[%s]",sLocalAreaId);
		strcpy(reqAllocateBalanceSvc.m_areaCode, sLocalAreaId);// ����
		__DEBUG_LOG1_(0, "���ţ�reqAllocateBalanceSvc.m_areaCode:[%s]",reqAllocateBalanceSvc.m_areaCode);
		snprintf(Head.m_sDestRealm,sizeof(Head.m_sDestRealm),"%s.ChinaTelecom.com",sOrgId);
		//strcpy(Head.m_sDestRealm,"025.ChinaTelecom.com");
		
		snprintf(Head.m_sSrvTextId,sizeof(Head.m_sSrvTextId),"DeductFromABM.Micropayment@%s.ChinaTelecom.com",sOrgId);
		__DEBUG_LOG1_(0, "��Ϣͷ��Ŀ���豸��ʶ-Head.m_sDestRealm:[%s]",Head.m_sDestRealm);
		
		Head.m_iAuthAppId = 4;
		Head.m_iReqType = 4;
		Head.m_iReqNumber  = 0;
		Head.m_iResultCode = 0;
		strcpy(Head.m_sSrvFlowId,""); // ������ȡԴm_sSrvFlowId��׷��
		snprintf(Head.m_sSrvFlowId,sizeof(Head.m_sSrvFlowId),"%s%s",receiveDccHead.m_sSrvFlowId,"test---");
		// �Լ���service-flow-id
		// 201107281124300000000100-00100-000000003001-1-010009-RechargeFromABM000.Micropayment.001
		char sSnDate[15]={0};
		long2date(reqAllocateBalanceSvc.m_iDeductDate,sSnDate);
		
		snprintf(Head.m_sSrvFlowId,sizeof(Head.m_sSrvFlowId),"%s08%d00-00100-000000003001-1-010010-RechargeFromABM000.Micropayment.001",sSnDate,lDccRecordSeq);
		
		memcpy((void *)(m_poPack->m_sDccBuf),(void *)&Head,sizeof(Head));
		
		m_poPack->m_iCmdID = ABMCMD_ALLOCATE_BALANCE;
	
		// ��� - ��һ�������Ϣ CCR	
		if (!m_poPack->addAllocateBalanceSvc(reqAllocateBalanceSvc))
		{
			__DEBUG_LOG0_(0, "AcctiveAllocateBalance::deductBalanceIn ��һ��CCR���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			return ACTIVE_PACK_CCR_ERR;
		}
		__DEBUG_LOG0_(0, "m_poPack->m_iCmdID=%d",m_poPack->m_iCmdID);
		
		__DEBUG_LOG0_(0, "��һ�����CCA������");
		
		// ��� - �ڶ���۷���ϸ
		for(int j=0;j<vResAcctItem.size();j++)
		{
			if (!m_poPack->addRequestDebitAcctItem(vResAcctItem[j]))
			{
				__DEBUG_LOG0_(0, "AllocateBalance:: _deal m_poPack ���ʧ��!");
				m_poPack->setRltCode(ECODE_PACK_FAIL);
				throw ACTIVE_PACK_CCR_ERR;
			}	
		}
		
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,������ֵ]:�ڶ��㸶����ϸCCR������.");
	
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		
		// ���淢����CCR��Ϣ
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,������ֵ]:���淢����DCC����Ϣ��TT");
		// modify.���ӱ���ԴDCC�ỰSession-id
		iRet = m_dccOperation->insertDccInfo(Head,dccType,lDccRecordSeq,receiveDccHead.m_sSessionId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,������ֵ]:����DCCҵ����Ϣʧ��");
			throw SAVE_PLATFOR_CCR_BIZ_ERR;
		}
		
		// ��չһ�ű�����������������ȥ��CCR��������ˮ�Ĺ�ϵ������ʡ��δ����CCAʱ��
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:���淢����CCR,SESSION_ID��ҵ��������ˮ��Ϣ��TT");
		__DEBUG_LOG1_(0, "������ˮ��reqAllocateBalanceSvc.m_requestId:[%s]",reqAllocateBalanceSvc.m_requestId);
		__DEBUG_LOG1_(0, "SESSION_ID:[%s]",Head.m_sSessionId);
		char sActionType[4]={0};
		strncpy(sActionType,"002",sizeof(sActionType));
		iRet = m_dccOperation->insertActiveCCR(Head.m_sSessionId,reqAllocateBalanceSvc.m_requestId,sActionType,sOrgId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:����CCR��Ϣʧ��");
			throw SAVE_PLATFOR_CCR_BIZ_ERR;
		}
	}
	catch(TTStatus oSt)
	{
		iRet = ACTIVE_DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,������ֵ]: oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,������ֵ] iRet=%d",value);
	}
	
	return iRet;
}

// ����
int ActiveAllocateBalance::deductBalanceOut(ReqPlatformSvc *pSvcIn,ReqPlatformPayInfo *g_itr2)
{
	int iRet=0;
	
	m_poActivePack = (ActiveAllocateBalanceCCR *)m_poPack;    // �������CCR
	
	// ��CCR��Ϣ
	__DEBUG_LOG0_(0, "========================��������ABM����CCR��TT=======================");
	__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]�Է�ƽ̨����CCR��ϸ:");
	__DEBUG_LOG1_(0, "������ˮ��m_requestId:[%s]",pSvcIn->m_requestId);
	//__DEBUG_LOG1_(0, "����ʱ�䣺m_iRequestTime:[%d]",pSvcIn->m_iRequestTime);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime:[%s]",pSvcIn->m_requestTime);
	__DEBUG_LOG1_(0, "�û����룺m_sDestinationAccount:[%s]",g_itr2->m_sDestinationAccount);
	__DEBUG_LOG1_(0, "��ʶ���ͣ�m_iDestinationType:[%d]",g_itr2->m_iDestinationType);
	__DEBUG_LOG1_(0, "�û����ԣ�m_destinationAttr:[%d]",g_itr2->m_iDestinationAttr);
	__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��m_sSvcPlatformId:[%s]",g_itr2->m_sSvcPlatformId);
	__DEBUG_LOG1_(0, "֧�����룺m_sPayPassword:[%s]",g_itr2->m_sPayPassword);
	__DEBUG_LOG1_(0, "�������ͣ�m_iBalanceTransFlag:[%d]",g_itr2->m_iBalanceTransFlag);
	__DEBUG_LOG1_(0, "���ĵ�λ���ͣ�m_iUnitTypeId:[%d]",g_itr2->m_iUnitTypeId);
	__DEBUG_LOG1_(0, "�軮����m_sDestinationAccount:[%ld]",g_itr2->m_lDeductAmount);
	
	/* Modify 2011.7.7 ȥ��ʱ���ʽת�������Է���ƽ̨����ֱ��ʹ���ַ�����ʽ
	if(pSvcIn->m_iRequestTime > 0)
		long2date(pSvcIn->m_iRequestTime,pSvcIn->m_requestTime);
	else
		GetTime(pSvcIn->m_requestTime);
	*/
	
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	char sReceiveSessionId[100]={0};
	try
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:����CCR����Ϣͷ��Ϣ����TT");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:����CCR����Ϣͷ��Ϣʧ��");
			throw SAVE_PLATFOR_CCR_ERR;
		}
		
		// ��ȡ����DCC��Ϣͷ��m_sSrvFlowId
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:����CCR��m_sSrvFlowId:[%s]",receiveDccHead.m_sSrvFlowId);
		
		// dcc��ҵ��������Ϣ
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:����CCR��ҵ����Ϣ����TT");
		iRet = m_dccOperation->savePlatformCCRBiz(receiveDccHead.m_sSessionId,pSvcIn,g_itr2);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:����CCRҵ����Ϣʧ��");
			throw SAVE_PLATFOR_CCR_BIZ_ERR;
		}
		
		// ��CCR������Ϣ
		__DEBUG_LOG0_(0, "========================��CCR������Ϣ=======================");
		
		// ��������CCR
		ReqBalanceSvc oReqBalanceSvc;
		
		// ����׼��
		strcpy(oReqBalanceSvc.m_requestId,pSvcIn->m_requestId); // ������ˮ				
		strcpy(oReqBalanceSvc.m_destinationId,g_itr2->m_sDestinationAccount);//����ֵ�û�����
		oReqBalanceSvc.m_destinationIdType = 2;	// Ĭ�ϣ���д��
		oReqBalanceSvc.m_destinationAttr = g_itr2->m_iDestinationAttr; // �û�����
		
		long lRequestTime=0L;
		//strcpy(oReqBalanceSvc.m_requestTime,sRequestTime); // ����ʱ��-ϵͳ��ǰʱ��
		//date2long(lRequestTime);
		//oReqBalanceSvc.m_iRequestTime=lRequestTime;	
		
		time_t tTime;
		
		tTime = time(NULL);
		oReqBalanceSvc.m_iRequestTime = tTime+2209017599;
		
		
		//oReqBalanceSvc.m_iRequestTime=pSvcIn->m_iRequestTime;		
		oReqBalanceSvc.m_balanceUnit = g_itr2->m_iUnitTypeId;	// ��λ����
		oReqBalanceSvc.m_rechargeAmount	= g_itr2->m_lDeductAmount; // ��ֵ���	
		//strcpy(oReqBalanceSvc.m_areaCode,"001"); // ��д��
		oReqBalanceSvc.m_blanceType = 0; // �������,�޷���ã���д��
			
		__DEBUG_LOG1_(0, "������ˮ��oReqBalanceSvc.m_requestId:[%s]",oReqBalanceSvc.m_requestId);
		__DEBUG_LOG1_(0, "����ֵ�û����룺oReqBalanceSvc.m_destinationId:[%s]",oReqBalanceSvc.m_destinationId);
		__DEBUG_LOG1_(0, "�������ͣ�oReqBalanceSvc.m_destinationIdType:[%d]",oReqBalanceSvc.m_destinationIdType);
		__DEBUG_LOG1_(0, "�û����ԣ�oReqBalanceSvc.m_destinationAttr:[%d]",oReqBalanceSvc.m_destinationAttr);
		__DEBUG_LOG1_(0, "����ʱ�䣺oReqBalanceSvc.m_iRequestTime:[%d]",oReqBalanceSvc.m_iRequestTime);
		__DEBUG_LOG1_(0, "��λ���ͣ�oReqBalanceSvc.m_balanceUnit:[%d]",oReqBalanceSvc.m_balanceUnit);
		__DEBUG_LOG1_(0, "��ֵ��oReqBalanceSvc.m_rechargeAmount:[%ld]",oReqBalanceSvc.m_rechargeAmount);
		//__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��oReqBalanceSvc.m_areaCode:[%s]",oReqBalanceSvc.m_areaCode);	
		__DEBUG_LOG1_(0, "������ͣ�oReqBalanceSvc.m_blanceType:[%d]",oReqBalanceSvc.m_blanceType);	
				
		// ������Ϣͷ
		// ����Session-id
		long lSessionIdSeq;
		iRet = m_dccOperation->QueryPaySeq(lSessionIdSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:��ѯDCC�Ự��ʶSession-Idʧ��");
			throw QRY_SESSION_ID_ERR;
		}
		
		struct struDccHead Head;
		char sSessionId[64];

		tTime = time(NULL);
		// 2011.8.1
		snprintf(Head.m_sSessionId,sizeof(Head.m_sSessionId),"%s%010d%s%04d\0","ABM@001.ChinaTelecom.com;",tTime,";",lSessionIdSeq);
		
		__DEBUG_LOG1_(0, "SESSION_ID:[%s]",Head.m_sSessionId);
		
		//strcpy(Head.m_sSrvTextId,"RechargeFromABM000.Micropayment@025.ChinaTelecom.com");
		
		strcpy(Head.m_sOutPlatform,""); // �ⲿƽ̨,SGWʹ��
		strcpy(Head.m_sOrignHost,"ABM@001.ChinaTelecom.com"); // 2011.8.1
		strcpy(Head.m_sOrignRealm,"001.ChinaTelecom.com");
		
		// ��ȡ���󻮲�����Ĺ�����
		char sOrgId[5]={'\0'};
		char sLocalAreaId[5]={0};
		iRet = m_dccOperation->qryAccountOrgId(oReqBalanceSvc.m_destinationId,sOrgId,sLocalAreaId);
		
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,��������]:������������:[%s]",sOrgId);
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,��������]:������������:[%s]",sLocalAreaId);
		
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:��ѯ�û�������������ʧ��");
			throw QRY_SERV_ORGID_ERR;
		}
		
		strcpy(oReqBalanceSvc.m_areaCode,sLocalAreaId);
		__DEBUG_LOG1_(0, "���ţ�oReqBalanceSvc.m_areaCode:[%s]",oReqBalanceSvc.m_areaCode);
		
		snprintf(Head.m_sDestRealm,sizeof(Head.m_sDestRealm),"%s.ChinaTelecom.com",sOrgId);
		//strcpy(Head.m_sDestRealm,"025.ChinaTelecom.com");
		snprintf(Head.m_sSrvTextId,sizeof(Head.m_sSrvTextId),"RechargeFromABM000.Micropayment@%s.ChinaTelecom.com",sOrgId);
		
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,��������]:��Ϣͷ��Ŀ���豸��ʶ-Head.m_sDestRealm:[%s]",Head.m_sDestRealm);
			
		Head.m_iAuthAppId = 4;
		Head.m_iReqType = 4;
		Head.m_iReqNumber  = 0;
		Head.m_iResultCode = 0;
		strcpy(Head.m_sSrvFlowId,""); // ������ȡԴm_sSrvFlowId��׷��
		// �Լ���service-flow-id
		// 201107281124300000000100-00100-000000003001-1-010009-RechargeFromABM000.Micropayment.001
		char sSnDate[15]={0};
		long2date(oReqBalanceSvc.m_iRequestTime,sSnDate);
		
		snprintf(Head.m_sSrvFlowId,sizeof(Head.m_sSrvFlowId),"%s%s",receiveDccHead.m_sSrvFlowId,"test---");
		
		snprintf(Head.m_sSrvFlowId,sizeof(Head.m_sSrvFlowId),"%s08%d00-00100-000000003001-1-010009-RechargeFromABM000.Micropayment.001",sSnDate,lDccRecordSeq);
		
		// ����CCR��Ϣͷ��ֵ
		memcpy((void *)(m_poPack->m_sDccBuf),(void *)&Head,sizeof(Head));
		
		m_poPack->m_iCmdID = ABMCMD_ACTIVE_ALLOCATE_BALANCE;
		
		
		// ���
		// ��� - ��һ�������Ϣ CCR	
		if (!m_poActivePack->addReqBalanceSvc(oReqBalanceSvc))
		{
			__DEBUG_LOG0_(0, "AcctiveAllocateBalance::deductBalanceOut ��һ��CCR���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw ACTIVE_PACK_CCR_OUT_ERR;
		}
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:��һ�����CCA������");
		
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		// ���淢����CCR��Ϣ
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:���淢����DCC����Ϣ��TT");
		iRet = m_dccOperation->insertDccInfo(Head,dccType,lDccRecordSeq,receiveDccHead.m_sSessionId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:����DCCҵ����Ϣʧ��");
			throw SAVE_PLATFOR_CCR_BIZ_ERR;
		}
		
		// ��չһ�ű�����������������ȥ��CCR��������ˮ�Ĺ�ϵ������ʡ��δ����CCAʱ��
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:���淢����CCR,SESSION_ID��ҵ��������ˮ��Ϣ��TT");
		__DEBUG_LOG1_(0, "������ˮ��oReqBalanceSvc.m_requestId:[%s]",oReqBalanceSvc.m_requestId);
		__DEBUG_LOG1_(0, "SESSION_ID:[%s]",Head.m_sSessionId);
		__DEBUG_LOG1_(0, "ORG_ID:[%s]",sOrgId);
		
		char sActionType[4]={0};
		strncpy(sActionType,"001",sizeof(sActionType));
		iRet = m_dccOperation->insertActiveCCR(Head.m_sSessionId,oReqBalanceSvc.m_requestId,sActionType,sOrgId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:����CCR��Ϣʧ��");
			throw SAVE_PLATFOR_CCR_BIZ_ERR;
		}
	}
	catch(TTStatus oSt)
	{
		iRet = ACTIVE_DEDUCT_TT_ERR;
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,��������]: oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��������-�շ�CCR,��������]: iRet=%d",value);
	}
	__DEBUG_LOG1_(0, "[��������-�շ�CCR,��������]-ActiveAllocateBalance::deductBalanceOut,iRet=:[%d]",iRet);
	return iRet;
}

