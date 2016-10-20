// ActiveAllocateBalanceBiz.cpp 
#include "ActiveAllocateBalanceBiz.h"
#include "AllocateBalanceSql.h"

#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"

static vector<ResBalanceSvc *> g_vResBalanceSvc;    // ��������CCA 
static vector<ResBalanceSvc *>::iterator g_itr1;

static vector<A_AllocateBalanceSvc *> g_vAAllocateBalanceSvc;	// ��������CCA��������	
static vector<A_AllocateBalanceSvc *>::iterator g_itr2;

static vector<A_ResponseDebitAcctItem *> g_vAResponseDebitAcctItem; // ��������CCA��Ŀ��ϸ
static vector<A_ResponseDebitAcctItem *>::iterator g_itr3;

extern TimesTenCMD *m_poBalance;

static char cPreSessionId[100]={0};

static long lServId=0L;

static char sCCASessionId[100]={0};

ActiveAllocateBalanceBiz::ActiveAllocateBalanceBiz()
{
		m_poSql = NULL;
		m_poUnpack = NULL;
		m_poActiveUnpack = NULL;
		m_poPack = NULL;
		m_paymentInfo = new PaymentInfo;
		
		m_dccOperation = new DccOperation;	
		
		m_depositInfo = new DepositInfo;
}

ActiveAllocateBalanceBiz::~ActiveAllocateBalanceBiz()
{
	if (m_poSql != NULL)
		delete m_poSql;	
	if (m_paymentInfo != NULL)
		delete m_paymentInfo;
		
	if (m_dccOperation != NULL)
		delete m_dccOperation;
	if (m_depositInfo != NULL)
		delete m_depositInfo;
}

int ActiveAllocateBalanceBiz::init(ABMException &oExp)
{
	if ((m_poSql = new AllocateBalanceSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "ActiveAllocateBalanceBiz::init malloc failed!");
		return -1;
	}
	return 0;
}

int ActiveAllocateBalanceBiz::deal(ABMCCR *pRecivePack, ABMCCA *pSendPack)
{
	__DEBUG_LOG0_(0, "=========>���ԣ�ActiveAllocateBalanceBiz::deal ��Ӧ����ʼ");
	__DEBUG_LOG0_(0, "=========>1������ʡABM���ص�CCA...");
	int iRet=0;
	int iBalanceTransFlag=2; // ��������
	
	// ��ȡpRecivePack�Ĺ���CCA��Ϣͷ
	char sSessionId[101]={0};
	struDccHead ccaHead;

	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"A"); 
	
	long lDccRecordSeq=0L; // ��ˮ�ţ�����
	
	ReqPlatformSvc reqSvc;
	ReqPlatformPayInfo reqPayInfo;
	
	try
	{
		// ȡ��Ϣ������Ϣͷ��Ϣ
		memcpy((void *)&ccaHead,(void *)(pRecivePack->m_sDccBuf),sizeof(ccaHead));
		// �յ�CCA��Session-Id
		__DEBUG_LOG1_(0, "=======>�յ�ʡABM���ص�CCA����Ϣͷ��SESSION_ID:[%s]",ccaHead.m_sSessionId);
		
		// ȡ��CCA�е�SessionId,ҵ������������DCC_INFO_RECORD��
		strcpy(sCCASessionId,ccaHead.m_sSessionId);
		
		//����CCA��Ӧ��ˮȥ���Է���ƽ̨����CCR�ĻỰID
		iRet = m_dccOperation->qryPreSessionId(ccaHead.m_sSessionId,sSessionId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "��ѯCCA�ỰID��ԴSession Idʧ��.");
			throw QRY_PRE_SESSION_ID_ERR;
		}
		__DEBUG_LOG1_(0, "========>�Է���ƽ̨����CCR��SESSION_ID:[%s]",sSessionId);
		strcpy(cPreSessionId,sSessionId);
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "��ѯDCC_INFO_RECORD_SEQʧ��");
			throw SAVE_RECEIVE_DCC_CCA_ERR;
		}
		
		// ���յ���CCA��Ϣͷ��TT
		iRet = m_dccOperation->insertDccInfo(ccaHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "�����Է���ƽ̨CCA����Ϣͷ��Ϣʧ��");
			throw SAVE_RECEIVE_DCC_CCA_ERR;
		}
		
		// ����sessionIdȡTT��Ӧ��CCR��Ϣ
		__DEBUG_LOG0_(0, "=========>����sessionIdȡTT��Ӧ��CCR[�Է���ƽ̨CCR������Ϣ]��Ϣ:");
	
		iRet = m_dccOperation->getCCRInfo(sSessionId,reqSvc,reqPayInfo);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "��ȡ�Է���ƽ̨CCR����ҵ����Ϣʧ��");
			throw SAVE_RECEIVE_DCC_CCA_ERR;
		}
		__DEBUG_LOG0_(0, "getCCRInfo,iRet=[%d]",iRet);
		// ��������
		iBalanceTransFlag = reqPayInfo.m_iBalanceTransFlag;
		
		// �����û������ȡ�û���ʶ
		ServAcctInfo oServAcct;
		ABMException oExp;
		__DEBUG_LOG1_(0, "�û�����:[%s]",reqPayInfo.m_sDestinationAccount);
		iRet = m_poSql->preQryServAcctInfo(reqPayInfo.m_sDestinationAccount,2,oServAcct,oExp);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "�����û������ѯ�û�����ʧ��");
			throw 13847;
		}
		lServId = oServAcct.m_servId;
		__DEBUG_LOG1_(0, "�û���ʶServ_ID:[%ld]",lServId);
		/*
		__DEBUG_LOG0_(0, "=========>�Է�ƽ̨����CCR��ϸ:");
		__DEBUG_LOG1_(0, "������ˮ��m_requestId:[%s]",reqSvc.m_requestId);
		__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime:[%s]",reqSvc.m_requestTime);
		
		__DEBUG_LOG1_(0, "�û����룺m_sDestinationAccount:[%s]",reqPayInfo.m_sDestinationAccount);
		__DEBUG_LOG1_(0, "��ʶ���ͣ�m_iDestinationType:[%d]",reqPayInfo.m_iDestinationType);
		__DEBUG_LOG1_(0, "�û����ԣ�m_iDestinationAttr:[%d]",reqPayInfo.m_iDestinationAttr);
		__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��m_sSvcPlatformId:[%s]",reqPayInfo.m_sSvcPlatformId);
		__DEBUG_LOG1_(0, "֧�����룺m_sPayPassword:[%s]",reqPayInfo.m_sPayPassword);
		__DEBUG_LOG1_(0, "�������ͣ�m_iBalanceTransFlag:[%d]",reqPayInfo.m_iBalanceTransFlag);
		__DEBUG_LOG1_(0, "���ĵ�λ���ͣ�m_iUnitTypeId:[%d]",reqPayInfo.m_iUnitTypeId);
		__DEBUG_LOG1_(0, "�軮����m_lDeductAmount:[%ld]",reqPayInfo.m_lDeductAmount);
		*/

		// ���ݲ�ѯ�����������ͣ��������
		m_poPack = (PlatformDeductBalanceCCA *)pSendPack;
		if(iBalanceTransFlag == 1)
		{
			m_poUnpack = (AllocateBalanceCCA *)pRecivePack;		  // ���-��ӦCCA,ʡ��ȫ��
			iRet = deductBalanceIn(reqSvc,reqPayInfo);
			// �Ի�����ֵ�����������У�飬��������	
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "������ֵ����ʧ��",);
				throw DEDUCT_BIZ1_IN_ERR;
			}	
		}
		else if(iBalanceTransFlag == 2)
		{
			m_poActiveUnpack = (ActiveAllocateBalanceCCA *)pRecivePack; // ���-��ӦCCA,ȫ����ʡ	
			iRet = deductBalanceOut(reqSvc,reqPayInfo);
			// ���������������������У�飬��������	
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "������������ʧ��");
				throw DEDUCT_BIZ2_IN_ERR;
			}
		}
	}
	catch(TTStatus oSt)
	{
		iRet = ACTIVE_DEDUCT_BIZ_TT_ERR;
		__DEBUG_LOG1_(0, "ActiveAllocateBalanceBiz::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "ActiveAllocateBalanceBiz::deal value=%d",value);
	}
	
	if(iRet != 0)
	{
		// �򷵻ذ�����������
		//m_poPack->clear();
		//m_poPack->setRltCode(ECODE_TT_FAIL);
		__DEBUG_LOG0_(0, "ActiveAllocateBalanceBiz::deal �����շ�CCAʧ��");
	}
	return iRet;
}

// �������Ͳ���
/*
int ActiveAllocateBalanceBiz::_deal(ReqPlatformSvc *pSvcIn,vector<ReqPlatformPayInfo *> g_vReqPlatformPayInfo)
{
	
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "========================��������=======================");
	__DEBUG_LOG0_(0, "=========>ȡ�Է���ƽ̨����CCR��һ�������Ϣ:");
	__DEBUG_LOG1_(0, "m_requestId:%s",pSvcIn->m_requestId);
	__DEBUG_LOG1_(0, "m_requestTime:%s",pSvcIn->m_requestTime);
	
	int iRet=0;
	int iBalanceTransFlag; // ��������
	
	for (g_itr2=g_vReqPlatformPayInfo.begin(); g_itr2!=g_vReqPlatformPayInfo.end(); ++g_itr2)
	{
	
		__DEBUG_LOG0_(0, "=========>ȡ�Է���ƽ̨����CCR�ڶ���֧����Ϣ��");
		 iBalanceTransFlag = (*g_itr2)->m_iBalanceTransFlag;
		__DEBUG_LOG1_(0, "=========>�������ͣ�[%d](1-ʡ��ȫ����2-ȫ����ʡ)",iBalanceTransFlag);
		 switch(iBalanceTransFlag)
		 {
		 	case 1:
		 		// ʡ��ȫ��
		 		// ʡABM��������
		 		// ����ABM��������ֵҵ��
		 		__DEBUG_LOG0_(0, "=========>������ʡ��ȫ��ҵ����");
		 		iRet = deductBalanceIn(pSvcIn,*g_itr2);
		 		if(iRet != 0)
		 		{
		 			__DEBUG_LOG0_(0, "=========>������ʡ��ȫ��ҵ����ʧ��");
		 			// �鷵��CCA
		 		}
		 		break;
		 	case 2:
		 		// ȫ����ʡ
		 		// ����ABM��������
		 		// ����ABM������ҵ��
		 		__DEBUG_LOG0_(0, "=========>������ȫ����ʡҵ����");
		 		 iRet = deductBalanceOut(pSvcIn,*g_itr2);
		 		 if(iRet !=0)
		 		 {
		 		 	__DEBUG_LOG0_(0, "=========>������ȫ����ʡҵ����");
		 			// �鷵��CCA
		 		 }
		 		break;
		 }
	}

	return iRet;	
}
*/

// ������ֵ��ҵ���߼�����
int ActiveAllocateBalanceBiz::deductBalanceIn(ReqPlatformSvc &reqSvc,ReqPlatformPayInfo &reqPayInfo)
{
	int iRet=0;
	int iSize=0;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"A"); 
	
	// ����ABM��Ӧ�Է���ƽ̨��CCA������Ϣ
	ResPlatformSvc oCCASvc;
	ResPlatformPayInfo oCCAPayInfo;
	ResPlatformAcctBalance oCCABalanceDetail;
	
	// 
	long lSessionIdSeq=0L;	// SessionId SEQ
	struct struDccHead Head;
	char sSessionId[64];
	time_t tTime;
	
	long lDccRecordSeq=0L;
	char sResponseTime[32]={0};
	g_toPlatformFlag = true; // �����Է���ƽ̨����Ϣ
	try
	{
		// ��CCA��
		__DEBUG_LOG0_(0, "========>��ʡABM���ص�CCA��-������������CCA");
		
		//��Ϣ��� - ��һ��
		g_vAAllocateBalanceSvc.clear();
		if ((iSize=m_poUnpack->getResBalanceSvcData(g_vAAllocateBalanceSvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "ActiveAllocateBalanceBiz::deductBalanceIn ʡABM����CCA�����������Ϣ��ΪNULL");
			m_poPack->setRltCode(ECODE_NOMSG);
			throw  RETURN_CCA_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "ActiveAllocateBalanceBiz::deductBalanceIn ��Ϣ�������!");
			m_poPack->setRltCode(ECODE_UNPACK_FAIL);
			throw RETURN_CCA_UNPACK_ERR;
		}
		__DEBUG_LOG1_(0, "============>iSize=[%d],��һ����Ϣ���������!",iSize);
		// �����һ����Ϣ
		for (g_itr2=g_vAAllocateBalanceSvc.begin(); g_itr2!=g_vAAllocateBalanceSvc.end(); ++g_itr2)
		{
			// ����SVC��detail�� - �ڶ���
			if ((iSize=m_poUnpack->getAcctBalanceData(*g_itr2,g_vAResponseDebitAcctItem)) == 0 )
			{
				__DEBUG_LOG0_(0, "AllocateBalance::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack->setRltCode(ECODE_NOMSG);
				throw RETURN_CCA_UNPACK_NOMSG;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "AllocateBalance::deal ��Ϣ�������!");
				m_poPack->setRltCode(ECODE_UNPACK_FAIL);
				throw RETURN_CCA_UNPACK_ERR;
			}
			
			__DEBUG_LOG0_(0, "=========>����ʡABM���ص�CCA��Ϣ.ִ�л�����ֵҵ����");			
			// ����					
			__DEBUG_LOG1_(0, "ʡABM����CCA-��Ӧ��ˮ��[%s]",(*g_itr2)->m_responseId);
			__DEBUG_LOG1_(0, "ʡABM����CCA-ҵ�񼶵Ľ�����룺[%d]", (*g_itr2)->m_svcResultCode);
			__DEBUG_LOG1_(0, "ʡABM����CCA-�۷������ʶ��[%ld]", (*g_itr2)->m_lAcctId);
			__DEBUG_LOG1_(0, "ʡABM����CCA-��λ��[%d]", (*g_itr2)->m_balanceUnit);
			__DEBUG_LOG1_(0, "ʡABM����CCA-�ɹ�������[%ld]", (*g_itr2)->m_deductAmount);
			__DEBUG_LOG1_(0, "ʡABM����CCA-�ʻ�ʣ���[%ld]", (*g_itr2)->m_acctBalance);
			
			// 2011.7.14.������չ��ACTIVE_CCR_LOG��״̬��������Ӧ��ˮ����״̬Ϊ00A
			iRet = m_dccOperation->updateActiveCCR((*g_itr2)->m_responseId);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "����ACTIVE_CCR_LOG״̬ʧ��.");
				throw DEDUCT_OUT_ERR;
			}
			
			if((*g_itr2)->m_svcResultCode != 0)
			{
				__DEBUG_LOG0_(0, "ʡABM����CCA-ҵ�񼶽�������0��");
				throw RETURN_BIZ_CODE_ERR;
			}
			
			oCCAPayInfo.m_lDeductAmount=(*g_itr2)->m_deductAmount; // �ɹ������Ľ��
			
			for (g_itr3=g_vAResponseDebitAcctItem.begin(); g_itr3!=g_vAResponseDebitAcctItem.end(); ++g_itr3)
			{	
				__DEBUG_LOG1_(0, "ʡABM����CCA-��������YYYYMM��[%d]",(*g_itr3)->m_billingCycle);
				__DEBUG_LOG1_(0, "ʡABM����CCA-��Ŀ���ͱ�ʶ��[%s]",(*g_itr3)->m_acctItemTypeId);
				__DEBUG_LOG1_(0, "ʡABM����CCA-�ʿ۷��˱���ʶ��[%ld]",(*g_itr3)->m_acctBalanceId);
				__DEBUG_LOG1_(0, "ʡABM����CCA-�ʱ�������[%ld]",(*g_itr3)->m_deductAmount);
				__DEBUG_LOG1_(0, "ʡABM����CCA-�ʱ�ʣ���[%ld]",(*g_itr3)->m_acctBalanceAmount);
			
				// TODO:����CCAҵ�񼶵Ľ�������ж��Ƿ���������ֵҵ��
				
				// CCA��Ӧ�ɹ�����������ֵҵ��
				__DEBUG_LOG0_(0, "========>CCA��Ӧ�ɹ�����������ֵҵ��");
				DepositServCondOne *pInOne = new DepositServCondOne;
				vector<DepositServCondTwo *> vInTwo;
				
				strcpy(pInOne->m_sReqSerial,(*g_itr2)->m_responseId);	// ��ֵ������ˮ-CCA��Ӧ��ˮ
				//strcpy(pInOne->m_sBalanceType,"10");			// �������
				pInOne->m_iBalanceType=10;
				pInOne->m_iDestinationBalanceUnit = (*g_itr2)->m_balanceUnit;// ��λ     
				pInOne->m_iVoucherValue=(*g_itr3)->m_deductAmount;	// �������
				pInOne->m_iProlongDays = 30;				// ��Ч��-��д��
				strcpy(pInOne->m_sDestinationType,"5BC");		// ��������
				strcpy(pInOne->m_sDestinationAccount,reqPayInfo.m_sDestinationAccount);	// �û�����
				//strcpy(pInOne->m_sDestinationAccount,"18951765356");	// �û�����-������д��
				pInOne->m_iDestinationAttr=reqPayInfo.m_iDestinationAttr;// �û�����
				pInOne->m_iDestinationAttr=2;				 // ���ԣ���д��
				pInOne->m_lDestinationAttrDetail=0;
				strcpy(pInOne->m_sAreaCode,"010");			// ���ţ���д��
				
				iRet = m_depositInfo->_dealIn(pInOne,vInTwo);
				if(iRet !=0 )
				{
					__DEBUG_LOG0_(0, "������ֵҵ����ʧ��.");
					throw DEDUCT_IN_ERR;
				}
				
				// ����DCC��Ϣͷ��Ϣ-DCC_INFO_RECORD ,SessionId
				iRet = m_dccOperation->updateDccInfo(sCCASessionId);
				if(iRet !=0 )
				{
					__DEBUG_LOG0_(0, "����DCC_INFO_RECORD״̬ʧ��.");
					throw DEDUCT_OUT_ERR;
				}
				
				oCCAPayInfo.m_iUnitTypeId = pInOne->m_iDestinationBalanceUnit; // ��λ����
				
				__DEBUG_LOG0_(0, "CCA��Ӧ�ɹ���������ֵҵ�����.");
				// CCA��Ӧ�ɹ���������ֵҵ�����
			}
			
			/*
			// 2011.7.14.������չ��ACTIVE_CCR_LOG��״̬��������Ӧ��ˮ����״̬Ϊ00A
			iRet = m_dccOperation->updateActiveCCR((*g_itr2)->m_responseId);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "����ACTIVE_CCR_LOG״̬ʧ��.");
				throw DEDUCT_OUT_ERR;
			}
			*/
		}
		
		/*
		for(int j=0;j<g_vAResponseDebitAcctItem.size();j++)
		{
			delete g_vAResponseDebitAcctItem[j];
			g_vAResponseDebitAcctItem[j]= NULL;
		}
		g_vAResponseDebitAcctItem.clear();
		
		for(int j=0;j<g_vAAllocateBalanceSvc.size();j++)
		{
			delete g_vAAllocateBalanceSvc[j];
			g_vAAllocateBalanceSvc[j]= NULL;
		}
		
		g_vAAllocateBalanceSvc.clear();
		*/
		
		oCCASvc.m_svcResultCode = 0;
		strcpy(oCCASvc.m_sParaFieldResult,"0");	// ������Ϣ
		strcpy(oCCASvc.m_responseId,reqSvc.m_requestId); // ����CCA����Ӧ��ˮ���Է���ƽ̨��������ˮ
		//strcpy(oCCASvc.m_responseId,"response-id-20110529");// ����ʱд��
		
		tTime = time(NULL);
		tTime += 2209017599;
		long2date(tTime,sResponseTime);
		strcpy(oCCASvc.m_responseTime,sResponseTime);
		
		//strcpy(oCCASvc.m_responseTime,"20110529"); // TODO����ת��
		
		// ����ʱ��д��xxxxxxxxxxxxxxxxxxxx
		oCCAPayInfo.m_iUnitTypeId = 2;
		oCCAPayInfo.m_lDeductAmount=10;
		oCCABalanceDetail.m_iBalanceSource = 1;
		oCCABalanceDetail.m_lBalanceAmount = 100;
		
		// Modify 2011.6.15,ȡ��
		// ����CCA��Ϣͷ
		// ����Session-id
		iRet = m_dccOperation->QueryPaySeq(lSessionIdSeq);
		if(iRet !=0 )
		{
			__DEBUG_LOG0_(0, "ȡSession-id����ʧ��.");
			throw GET_SESSION_ID_SEQ_ERR;
		}
		
		//tTime = time(NULL);
		// 2011.8.1
		snprintf(Head.m_sSessionId,sizeof(Head.m_sSessionId),"%s%010d%s%04d\0","ABM@001.ChinaTelecom.com;",tTime,";",lSessionIdSeq);
		
		strcpy(Head.m_sSessionId,cPreSessionId);
			
		__DEBUG_LOG1_(0, "����CCA��SESSION_ID:[%s]",Head.m_sSessionId);
		
		strcpy(Head.m_sSrvTextId,"DeductFromABM.Micropayment@001.ChinaTelecom.com");
		strcpy(Head.m_sOutPlatform,"");
		strcpy(Head.m_sOrignHost,"ABM@001.ChinaTelecom.com"); // 2011.8.1
		strcpy(Head.m_sOrignRealm,"001.ChinaTelecom.com");
		
		// ��ȡ���󻮲�����Ĺ�����
		char sOrgId[5]={0};
		
		char sAccoutNbr[32]={0};
		char sLocalAreaId[5]={0};
		strcpy(sAccoutNbr,reqPayInfo.m_sDestinationAccount);
		//iRet = m_dccOperation->qryAccountOrgId(reqPayInfo.m_iDestinationAttr,sOrgId); // ���Ժ�ſ�
		//iRet = m_dccOperation->qryAccountOrgId(sAccoutNbr,sOrgId); // ����ʱд��
		iRet = m_dccOperation->qryAccountOrgId(sAccoutNbr,sOrgId,sLocalAreaId);
		if(iRet !=0 )
		{
			__DEBUG_LOG0_(0, "ȡ����������������ʧ��.");
			throw GET_ACCT_NBR_ORGID_ERR;
		}
		
		snprintf(Head.m_sDestRealm,sizeof(Head.m_sDestRealm),"%s.ChinaTelecom.com",sOrgId);

		__DEBUG_LOG1_(0, "��Ϣͷ��Ŀ���豸��ʶ-Head.m_sDestRealm:[%s]",Head.m_sDestRealm);
		
		Head.m_iAuthAppId = 4;
		Head.m_iReqType = 4;
		Head.m_iReqNumber  = 0;
		Head.m_iResultCode = 0;
	
		memcpy((void *)(m_poPack->m_sDccBuf),(void *)&Head,sizeof(Head));
		
		//m_poPack->m_iCmdID = ABMCMD_ALLOCATE_BALANCE;
		//m_poPack->m_iCmdID = ABMCMD_PLATFORM_QUEUE;
		
		
		// ��CCA���������Է���ƽ̨oCCAPayInfo
		__DEBUG_LOG0_(0, "========>��CCA���������Է���ƽ̨");
		// CCA ��һ�������Ϣ
		if (!m_poPack->addResPlatformSvc(oCCASvc))
		{
			__DEBUG_LOG0_(0, "ActiveAllocateBalanceBiz::deductBalanceIn ��һ��CCA���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_IN_PACK_ERR;
		}
		__DEBUG_LOG0_(0, "��һ�����CCA������");
		

		// CCA ��������ϸ��Ϣ
		if (!m_poPack->addResPlatformAcctBalance(oCCABalanceDetail))
		{
			__DEBUG_LOG0_(0, "ActiveAllocateBalanceBiz::deductBalanceIn ������CCA���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_IN_PACK_ERR;
		}
		
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0 )
		{
			__DEBUG_LOG0_(0, "ȡDCC_INFO_RECORD_SEQʧ��.");
			throw GET_DCC_INFO_RECORD_SEQ_ERR;
		}
		
		// ���淢����CCR��Ϣ
		__DEBUG_LOG0_(0, "���淢����DCC����Ϣ��TT");
		iRet = m_dccOperation->insertDccInfo(Head,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "����DCCҵ����Ϣʧ��");
			throw SAVE_SEND_DCC_CCA_ERR;
		}
		m_poPack->m_iCmdID = ABMCMD_PLATFORM_DEDUCT_BALANCE;
		
	}
	catch(TTStatus oSt)
	{
		iRet = ACTIVE_DEDUCT_BIZ_TT_ERR;
		__DEBUG_LOG1_(0, "AllocateBalance::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "AllocateBalance::deal value=%d",value);
	}	
	
	for(int j=0;j<g_vAResponseDebitAcctItem.size();j++)
	{
		delete g_vAResponseDebitAcctItem[j];
		g_vAResponseDebitAcctItem[j]= NULL;
	}
	g_vAResponseDebitAcctItem.clear();
	
	for(int j=0;j<g_vAAllocateBalanceSvc.size();j++)
	{
		delete g_vAAllocateBalanceSvc[j];
		g_vAAllocateBalanceSvc[j]= NULL;
	}
	
	g_vAAllocateBalanceSvc.clear();
	
	return iRet;
}

// ������ҵ���߼�����
int ActiveAllocateBalanceBiz::deductBalanceOut(ReqPlatformSvc &reqSvc,ReqPlatformPayInfo &reqPayInfo)
{
	int iRet=0;
	int iSize=0;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"A"); 
	
	// CCA������Ϣ
	ResPlatformSvc oCCASvc;
	ResPlatformPayInfo oCCAPayInfo;
	ResPlatformAcctBalance oCCABalanceDetail;
	
	long lSessionIdSeq;
	struct struDccHead Head;
	char sSessionId[64];
	time_t tTime;
	
	long lDccRecordSeq=0L;
	
	char sResponseTime[32]={0};
	g_toPlatformFlag = true; // �����Է���ƽ̨����Ϣ
	try
	{
		
		// ��CCA��
		__DEBUG_LOG0_(0, "��ʡABM���ص�CCA��-������������CCA");
		
		// CCA��Ӧ�ɹ���������ҵ��
		__DEBUG_LOG0_(0, "========>CCA��Ӧ�ɹ���������ҵ��");
		
		//��Ϣ��� - ��һ��
		if ((iSize=m_poActiveUnpack->getResBalanceSvc(g_vResBalanceSvc)) == 0 )
		{
			__DEBUG_LOG0_(0, "ActiveAllocateBalanceBiz::deductBalanceIn ����ABM�������󻮳�-ʡABM����CCA�����������Ϣ��ΪNULL");
			m_poPack->setRltCode(ECODE_NOMSG);
			throw RETURN_CCA_UNPACK_NOMSG;
		}
		else if (iSize < 0)
		{
			__DEBUG_LOG0_(0, "ActiveAllocateBalanceBiz::deductBalanceOut ��Ϣ�������!");
			m_poPack->setRltCode(ECODE_UNPACK_FAIL);
			throw RETURN_CCA_UNPACK_ERR;
		}
		
		// �����һ����Ϣ
		__DEBUG_LOG1_(0, "ҵ��ṹ���С��size:[%d].",g_vResBalanceSvc.size());
		for (g_itr1=g_vResBalanceSvc.begin(); g_itr1!=g_vResBalanceSvc.end(); ++g_itr1)
		{ 
			__DEBUG_LOG0_(0, "=========>_deal.����ʡABM���ص�CCA��Ϣ.ִ����������ҵ����");			
			// ����							
			__DEBUG_LOG1_(0, "ʡABM����CCA-��Ӧ��ˮ��[%s]",(*g_itr1)->m_responseId);
			__DEBUG_LOG1_(0, "ʡABM����CCA-ҵ�񼶵Ľ�����룺[%d]", (*g_itr1)->m_svcResultCode);
			__DEBUG_LOG1_(0, "ʡABM����CCA-������Ϣ��[%s]", (*g_itr1)->m_sParaFieldResult);
			
			__DEBUG_LOG1_(0, "ʡABM����CCA-��Ӧʱ�䣺[%d]", (*g_itr1)->m_responseTime);
			__DEBUG_LOG1_(0, "ʡABM����CCA-��λ��[%d]", (*g_itr1)->m_balanceUnit);
			__DEBUG_LOG1_(0, "ʡABM����CCA-������ʡABM�ʻ���[%ld]", (*g_itr1)->m_balanceAmount);
			__DEBUG_LOG1_(0, "ʡABM����CCA-������ͣ�[%ld]", (*g_itr1)->m_blanceType);
			
			__DEBUG_LOG0_(0, "========>CCA��Ӧ�ɹ���������ҵ��");
			// ��������У��
			
			// 2011.7.14.������չ��ACTIVE_CCR_LOG��״̬��������Ӧ��ˮ����״̬Ϊ00A
			iRet = m_dccOperation->updateActiveCCR((*g_itr1)->m_responseId);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "����ACTIVE_CCR_LOG״̬ʧ��.");
				throw DEDUCT_OUT_ERR;
			}
			
			if((*g_itr1)->m_svcResultCode != 0)
			{
				__DEBUG_LOG0_(0, "ʡABM����CCA-ҵ�񼶽�������0��");
				throw RETURN_BIZ_CODE_ERR;
			}
			
			
			// ��������
			PaymentInfoALLCCR pPaymentInfoCCR={0};
			PaymentInfoALLCCA pPaymentInfoCCA={0};
			
			// ����CCR��Ϣ׼��	
			strcpy(pPaymentInfoCCR.m_sOperateSeq,reqSvc.m_requestId);			// ������ˮ��
			strcpy(pPaymentInfoCCR.m_sDestinationAccount,reqPayInfo.m_sDestinationAccount);	// �û�����
			pPaymentInfoCCR.m_iDestinationIDtype = 2;					// ��������
			pPaymentInfoCCR.m_iDestinationAttr = reqPayInfo.m_iDestinationAttr;		// �û�����
			strcpy(pPaymentInfoCCR.m_sAreaCode,"010");					// �绰����
			pPaymentInfoCCR.m_iStaffID = 66848;						// ����ԱID
			strcpy(pPaymentInfoCCR.m_sDeductDate,(*g_itr1)->m_responseTime);		// �۷�ʱ��
			strcpy(pPaymentInfoCCR.m_sMicropayType,"0");
			strcpy(pPaymentInfoCCR.m_sCapabilityCode,"1004");
			strcpy(pPaymentInfoCCR.m_sDescription,"����ABM��������");
			pPaymentInfoCCR.m_lPayAmount = 0-reqPayInfo.m_lDeductAmount;			// �軮�����
			//pPaymentInfoCCR.m_lServID = 161; // ��д��
			pPaymentInfoCCR.m_lServID = lServId;
			
			// ����ҵ���߼�����	
			__DEBUG_LOG1_(0, "����ǰ��paymentInfoCCR�軮����%ld.",pPaymentInfoCCR.m_lPayAmount);
			iRet = m_paymentInfo->DeductBalanceInfo(pPaymentInfoCCR,pPaymentInfoCCA);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "��������ҵ����ʧ��.");
				throw DEDUCT_OUT_ERR;
			}
			
			// ����DCC��Ϣͷ��Ϣ-DCC_INFO_RECORD ,SessionId
			iRet = m_dccOperation->updateDccInfo(sCCASessionId);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "����DCC_INFO_RECORD״̬ʧ��.");
				throw DEDUCT_OUT_ERR;
			}
			
			/*
			// 2011.7.14.������չ��ACTIVE_CCR_LOG��״̬��������Ӧ��ˮ����״̬Ϊ00A
			iRet = m_dccOperation->updateActiveCCR((*g_itr1)->m_responseId);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "����ACTIVE_CCR_LOG״̬ʧ��.");
				throw DEDUCT_OUT_ERR;
			}
			*/
			
			
			// ����CCA��Ϣ-�ϲ�����һ��ṹ
			//oCCAPayInfo.m_lDeductAmount=0-pPaymentInfoCCR.m_lPayAmount; // �ɹ������Ľ��
			//oCCAPayInfo.m_iUnitTypeId = (*g_itr1)->m_balanceUnit; // ��λ����
			
			oCCASvc.m_lDeductAmount=0-pPaymentInfoCCR.m_lPayAmount; // �ɹ������Ľ��
			oCCASvc.m_iUnitTypeId = (*g_itr1)->m_balanceUnit; // ��λ����
			
			__DEBUG_LOG0_(0, "========>�����Է���ƽ̨��CCA����λ����:[%d]",oCCAPayInfo.m_iUnitTypeId);	
			__DEBUG_LOG0_(0, "========>��������ҵ�������");	
			
			oCCABalanceDetail.m_lBalanceAmount = pPaymentInfoCCA.m_lAcctBalanceAmount; // ����ʱ�ʣ����
		}
		
		__DEBUG_LOG1_(0, "g_vResBalanceSvc.size()=[%d]!",g_vResBalanceSvc.size());
		
		
		
		oCCASvc.m_svcResultCode = 0;
		strcpy(oCCASvc.m_sParaFieldResult,"");	// ����Avp Code
		
		strcpy(oCCASvc.m_responseId,reqSvc.m_requestId); // ����CCA����Ӧ��ˮ���Է���ƽ̨��������ˮ
		//strcpy(oCCASvc.m_responseId,"response-id-20110529");// ����ʱд��
		tTime = time(NULL);
		tTime += 2209017599;
		long2date(tTime,sResponseTime);
		strcpy(oCCASvc.m_responseTime,sResponseTime);
		
		// ����ʱ��д������������������
		/* 2011.7.11 ȥ��
		oCCAPayInfo.m_iUnitTypeId = 2;
		oCCAPayInfo.m_lDeductAmount=10;
		oCCABalanceDetail.m_iBalanceSource = 1;  // ����ʱ���Դ
		oCCABalanceDetail.m_lBalanceAmount = 300;  // ����ʱ���ǰ���
		*/

		
		// ����CCA��Ϣͷ
		// Session-id - �����Է���ƽ̨��Session��Ҫ����������ˮ�Ŵ�DCCҵ����Ϣ��¼���л�ȡ
		
		iRet = m_dccOperation->QueryPaySeq(lSessionIdSeq);
		
		// 2011.8.1
		snprintf(Head.m_sSessionId,sizeof(Head.m_sSessionId),"%s%010d%s%04d\0","ABM@001.ChinaTelecom.com;",tTime,";",lSessionIdSeq);
			
		__DEBUG_LOG1_(0, "SESSION_ID:[%s]",Head.m_sSessionId);
		
		strcpy(Head.m_sSrvTextId,"DeductFromABM.Micropayment@001.ChinaTelecom.com");
		strcpy(Head.m_sOutPlatform,""); // �ⲿƽ̨��SGW��
		strcpy(Head.m_sOrignHost,"ABM@001.ChinaTelecom.com"); // 2011.8.1
		strcpy(Head.m_sOrignRealm,"001.ChinaTelecom.com");
		
		// ��ȡ���󻮲�����Ĺ�����
		char sOrgId[5]={0};
		
		char sAccoutNbr[32]={0};
		char sLocalAreaId[5]={0};
		//strcpy(sAccoutNbr,"18951765356");
		strcpy(sAccoutNbr,reqPayInfo.m_sDestinationAccount);
		//iRet = m_dccOperation->qryAccountOrgId(sAccoutNbr,sOrgId); // ����ʱд�����˴������Է���ƽ̨��
		//iRet = m_dccOperation->qryAccountOrgId(reqPayInfo.m_iDestinationAttr,sOrgId); // ���Ժ�ſ�
		iRet = m_dccOperation->qryAccountOrgId(sAccoutNbr,sOrgId,sLocalAreaId);
		if(iRet !=0 )
		{
			__DEBUG_LOG0_(0, "ȡ����������������ʧ��.");
			throw GET_ACCT_NBR_ORGID_ERR;
		}	
	
		snprintf(Head.m_sDestRealm,sizeof(Head.m_sDestRealm),"%s.ChinaTelecom.com",sOrgId);

		__DEBUG_LOG1_(0, "��Ϣͷ��Ŀ���豸��ʶ-Head.m_sDestRealm:[%s]",Head.m_sDestRealm);
		
		Head.m_iAuthAppId = 4;
		Head.m_iReqType = 4;
		Head.m_iReqNumber  = 0;
		Head.m_iResultCode = 0;
	
		memcpy((void *)(m_poPack->m_sDccBuf),(void *)&Head,sizeof(Head));
		
		// ��CCA���������Է���ƽ̨
		__DEBUG_LOG0_(0, "========>��CCA���������Է���ƽ̨");
			__DEBUG_LOG0_(0, "========>��CCA���������Է���ƽ̨");
		// CCA ��һ�������Ϣ
		if (!m_poPack->addResPlatformSvc(oCCASvc))
		{
			__DEBUG_LOG0_(0, "ActiveAllocateBalanceBiz::deductBalanceIn ��һ��CCA���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_IN_PACK_ERR;
		}
		__DEBUG_LOG0_(0, "��һ��CCA������");
		
		// CCA �ڶ�����
		if (!m_poPack->addResPlatformAcctBalance(oCCABalanceDetail))
		{
			__DEBUG_LOG0_(0, "ActiveAllocateBalanceBiz::deductBalanceIn �ڶ���CCA���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw DEDUCT_IN_PACK_ERR;
		}
		__DEBUG_LOG0_(0, "�ڶ���CCA������");
		
		//m_poPack->m_iCmdID = ABMCMD_PLATFORM_QUEUE;//ABMCMD_ALLOCATE_BALANCE;
		
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0 )
		{
			__DEBUG_LOG0_(0, "ȡDCC_INFO_RECORD_SEQʧ��.");
			throw GET_DCC_INFO_RECORD_SEQ_ERR;
		}
		
		
		// ���淢����CCA��Ϣ
		__DEBUG_LOG0_(0, "���淢����DCC-CCA����Ϣ��TT");
		iRet = m_dccOperation->insertDccInfo(Head,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "����DCC-CCA��Ϣʧ��");
			throw SAVE_SEND_DCC_CCA_ERR;
		}
		m_poPack->m_iCmdID = ABMCMD_PLATFORM_DEDUCT_BALANCE;
	}
	catch(TTStatus oSt)
	{
		iRet = ACTIVE_DEDUCT_BIZ_TT_ERR;
		__DEBUG_LOG1_(0, "ActiveAllocateBalanceBiz::deductBalanceOut oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "ActiveAllocateBalanceBiz::deductBalanceOut value=%d",value);
	}	
	for(int j=0;j<g_vResBalanceSvc.size();j++)
	{
		delete g_vResBalanceSvc[j];
		g_vResBalanceSvc[j]= NULL;
	}
	g_vResBalanceSvc.clear();
	
	
	return iRet;
}

