#include "ABMSvcSupport.h"
#include "ABMException.h"
#include "PaymentInfo.h"
#include "DepositInfo.h"
#include "HssQuery.h"
#include "QueryAll.h"
#include "abmcmd.h"
#include "LogV2.h"
#include "PublicConndb.h"
#include "AllocateBalance.h"
#include "UserSelfService.h"
#include "ReverseDeductBalance.h"
#include "QryPasswordSend.h"
#include "QryServInfosyn.h"
#include "ActiveAllocateBalance.h"
#include "ActiveAllocateBalanceBiz.h"
#include "ReverseDepositInfo.h"
#include "ReversePay.h"
#include "PasswdIdentify.h"
#include "PasswdIdentifyBiz.h"
#include "PasswdChange.h"
#include "PasswdChangeBiz.h"
#include "ABMSynRcv.h"

ABMSvcSupt::ABMSvcSupt()
{
    m_poPaymentInfo=NULL;//֧��
    m_pPublicConndb=NULL;
    m_poAllocateBalance = NULL;//����
    m_poDepositInfo=NULL;//��ֵ����
    m_poReverseDeductBalance = NULL;
    m_poQrySrv = NULL;//����������֤�·�
    m_poQryInfosyn = NULL;//��������������ͬ��
    m_poHssQuery = NULL;
    m_poQueryAll = NULL;
    
    m_poPayAbilityQuery = NULL;//֧������״̬��ѯ	
    m_poBalanceTransQuery = NULL;//���������ѯ
    m_poBalanceTransReset = NULL;//���������ѯ
    m_poPasswordReset = NULL;//֧�������޸�
    m_poPayStatusReset = NULL;//֧��״̬���
    m_poPayRulesQuery = NULL;//֧�������ѯ
    m_poPayRulesReset = NULL;//֧��������
    m_poBindingAccNbrQuery = NULL;//�󶨺����ѯ
    
    m_poActiveAllocateBalance = NULL; // ��������-CCR
    m_poActiveAllocateBalanceBiz = NULL; // ��������-CCA
    m_pReverseDepositInfo=NULL;//��ֵ����
	m_poReversePay = NULL; // ֧������
	
	m_pIdentify = NULL;//�����Ȩ
	m_pIdentifyBiz = NULL;//�����Ȩ
	m_pChange = NULL;//�����޸�
	m_pChangeBiz = NULL;//�����޸�
	m_pSynRcv = NULL;//����
}

ABMSvcSupt:: ~ABMSvcSupt()
{
	FreeObject(m_poPayAbilityQuery);
    FreeObject(m_poBalanceTransQuery);
    FreeObject(m_poBalanceTransReset);
    FreeObject(m_poPasswordReset);
    FreeObject(m_poPayStatusReset);
    FreeObject(m_poPayRulesQuery);
    FreeObject(m_poPayRulesReset);
    FreeObject(m_poBindingAccNbrQuery);
    
    FreeObject(m_poPaymentInfo);
    FreeObject(m_pPublicConndb);
    FreeObject(m_poAllocateBalance);
    FreeObject(m_poDepositInfo);
	FreeObject(m_poReverseDeductBalance);
	FreeObject(m_poQrySrv);
	FreeObject(m_poQryInfosyn);
    FreeObject(m_poHssQuery);
    FreeObject(m_poQueryAll);
    FreeObject(m_poActiveAllocateBalance);
    FreeObject(m_pReverseDepositInfo);
	FreeObject(m_poReversePay);
	FreeObject(m_poActiveAllocateBalanceBiz);
	FreeObject(m_pIdentify);
	FreeObject(m_pIdentifyBiz);
	FreeObject(m_pChange);
	FreeObject(m_pChangeBiz);
	FreeObject(m_pSynRcv);
}

int ABMSvcSupt::init(ABMException &oExp)
{
	
	 // ֧��״̬��ѯ��ʼ��
    if ((m_poPayAbilityQuery=new PayAbilityQuery) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poPayAbilityQuery malloc failed!");
        return -1;
    }
     // ���������ѯ��ʼ��
    if ((m_poBalanceTransQuery=new BalanceTransQuery) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poBalanceTransQuery malloc failed!");
        return -1;
    }
     // �����������ó�ʼ��
    if ((m_poBalanceTransReset=new BalanceTransReset) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poBalanceTransReset malloc failed!");
        return -1;
    }
     // ֧����������ʼ��
    if ((m_poPasswordReset=new PasswordReset) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poPasswordReset malloc failed!");
        return -1;
    }
    
     // ֧��״̬���
    if ((m_poPayStatusReset=new PayStatusReset) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poPayStatusReset malloc failed!");
        return -1;
    }
    // ֧�������ѯ
    if ((m_poPayRulesQuery=new PayRulesQuery) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poPayRulesQuery malloc failed!");
        return -1;
    }
     // ֧��������
    if ((m_poPayRulesReset=new PayRulesReset) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poPayRulesReset malloc failed!");
        return -1;
    }
     // �󶨺����ѯ
    if ((m_poBindingAccNbrQuery=new BindingAccNbrQuery) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poBindingAccNbrQuery malloc failed!");
        return -1;
    }
    
	//֧��
    if ((m_poPaymentInfo=new PaymentInfo) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::new m_poPaymentInfo malloc failed!");
        return -1;
    }
	// ֧������
    if ((m_poReversePay=new ReversePay) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poReversePay malloc failed!");
        return -1;
    }
    if ((m_pPublicConndb=new PublicConndb) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::new m_poPaymentInfo malloc failed!");
        return -1;
    }
    // ����������ʼ��
    if ((m_poAllocateBalance=new AllocateBalance) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poAllocateBalance malloc failed!");
        return -1;
    }
    // ����������ʼ��-CCR
    if ((m_poActiveAllocateBalance=new ActiveAllocateBalance) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poActiveAllocateBalance malloc failed!");
        return -1;
    }
    // ����������ʼ��-CCA
    if ((m_poActiveAllocateBalanceBiz=new ActiveAllocateBalanceBiz) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poActiveAllocateBalance malloc failed!");
        return -1;
    }
    //��ֵ֧�� 
    if ((m_poDepositInfo=new DepositInfo) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poDepositInfo malloc failed!");
        return -1;
    }
    //��ֵ����
    if ((m_pReverseDepositInfo=new ReverseDepositInfo) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init ReverseDepositInfo malloc failed!");
        return -1;
    }
	if ((m_poReverseDeductBalance=new ReverseDeductBalance) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poReverseDeductBalance malloc failed!");
        return -1;
    }
    if (m_pPublicConndb->init(oExp) != 0) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_pPublicConndb->init failed!");
        return -1;
    }
    //����������֤�·���ʼ��
    if ((m_poQrySrv=new QryPasswordSend) == NULL) {
    	ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poQrySrv malloc failed!");
    	return -1;
    }
    if (m_poQrySrv->init(oExp) != 0) {
    	ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poQrySrv->init failed!");
    	return -1;
    }
    //��������������ͬ��
    if ((m_poQryInfosyn=new QryServInfosyn) == NULL) {
    	ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poQryInfosyn malloc failed!");
    	return -1;
    }
    if (m_poQryInfosyn->init(oExp) != 0) {
    	ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poQryInfosyn->init failed!");
    	return -1;
    }
    
    if ((m_poHssQuery = new HssQuery) == NULL)
    {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poHssQuery malloc failed!");
        return -1;
    }
    if (m_poHssQuery->init(oExp) != 0)
    {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poHssQuery->init failed!");
        return -1;
    }

    if ((m_poQueryAll = new QueryAll) == NULL)
    {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poQueryAll malloc failed!");
        return -1;
    }
    if (m_poQueryAll->init(oExp) != 0)
    {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poQueryAll->init failed!");
        return -1;
    }
    //��½�����Ȩ-CCR
     if ((m_pIdentify=new PasswdIdentify) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init PasswdIdentify malloc failed!");
        return -1;
    }
    //��½�����Ȩ-CCA
    if ((m_pIdentifyBiz=new PasswdIdentifyBiz) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init PasswdIdentifyBiz malloc failed!");
        return -1;
    }
    //��½�����޸�-CCR
    if ((m_pChange=new PasswdChange) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init PasswdChange malloc failed!");
        return -1;
    }
    //��½�����޸�-CCA
    if ((m_pChangeBiz=new PasswdChangeBiz) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init PasswdChangeBiz malloc failed!");
        return -1;
    }
    //����-CCA
    if ((m_pSynRcv=new ABMSynRcv) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init ABMSynRcv malloc failed!");
        return -1;
    }
    if (m_pSynRcv->init(oExp) != 0)
    {
        ADD_EXCEPT0(oExp, "ABMSvcSupt::init m_poQueryAll->init failed!");
        return -1;
    }
    
    return 0;
}

int ABMSvcSupt::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	ABMException oExp;
    int iRet=0;
    if ((pCCR==NULL) || (pCCA==NULL))
        return -1;
    pCCA->memcpyh(pCCR);
    
    switch (pCCR->m_iCmdID) 
    {
    	case ABMCMD_PAY_ABILITY_QRY:
    	{ // ֧������״̬��ѯ
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                
                struDccHead Head;
	
			memcpy((void *)&Head,(void *)(pCCR->m_sDccBuf),sizeof(Head));
		
			iRet=m_poPayAbilityQuery->deal(pCCR,pCCA);

			if(iRet==0)
			{
	        		__DEBUG_LOG0_(0, "m_poPayAbilityQuery->deal  ---0");
				m_pPublicConndb->Commit(oExp);
			}
			else
			{
	        		__DEBUG_LOG0_(0, "m_poPayAbilityQuery->deal  ---1");
				m_pPublicConndb->RollBack(oExp);
			}
	       		break;
    	}
    	case ABMCMD_BAL_TRANS_QRY:
    	{ // ���������ѯ
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                
                struDccHead Head;
	
			memcpy((void *)&Head,(void *)(pCCR->m_sDccBuf),sizeof(Head));
		
			iRet=m_poBalanceTransQuery->deal(pCCR,pCCA);

			if(iRet==0)
			{
	        		__DEBUG_LOG0_(0, "m_poBalanceTransQuery->deal  ---0");
				m_pPublicConndb->Commit(oExp);
			}
			else
			{
	        		__DEBUG_LOG0_(0, "m_poBalanceTransQuery->deal  ---1");
				m_pPublicConndb->RollBack(oExp);
			}
	       		break;
    	}
    	case ABMCMD_TRANS_RULES_RESET:// ������������
    	{ 
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                
            struDccHead Head;
	
			memcpy((void *)&Head,(void *)(pCCR->m_sDccBuf),sizeof(Head));
		
			iRet=m_poBalanceTransReset->deal(pCCR,pCCA);

			if(iRet==0)
			{
	        		__DEBUG_LOG0_(0, "m_poBalanceTransReset->deal  ---0");
				m_pPublicConndb->Commit(oExp);
			}
			else
			{
	        		__DEBUG_LOG0_(0, "m_poBalanceTransReset->deal  ---1");
				m_pPublicConndb->RollBack(oExp);
			}
	       		break;
    	}
    	case ABMCMD_PASSWORD_RESET:// ֧��������
    	{ 
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                
            struDccHead Head;
	
			memcpy((void *)&Head,(void *)(pCCR->m_sDccBuf),sizeof(Head));
		
			iRet=m_poPasswordReset->deal(pCCR,pCCA);

			if(iRet==0)
			{
	        		__DEBUG_LOG0_(0, "m_poPasswordReset->deal  ---0");
				m_pPublicConndb->Commit(oExp);
			}
			else
			{
	        		__DEBUG_LOG0_(0, "m_poPasswordReset->deal  ---1");
				m_pPublicConndb->RollBack(oExp);
			}
	       		break;
    	}
    	case ABMCMD_PAYSTATUS_RESET:// ֧��״̬���
    	{ 
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                
            struDccHead Head;
	
			memcpy((void *)&Head,(void *)(pCCR->m_sDccBuf),sizeof(Head));
		
			iRet=m_poPayStatusReset->deal(pCCR,pCCA);

			if(iRet==0)
			{
	        		__DEBUG_LOG0_(0, "m_poPayStatusReset->deal  ---0");
				m_pPublicConndb->Commit(oExp);
			}
			else
			{
	        		__DEBUG_LOG0_(0, "m_poPayStatusReset->deal  ---1");
				m_pPublicConndb->RollBack(oExp);
			}
	       		break;
    	}
    	case ABMCMD_PAYRULE_QRY:// ֧�������ѯ
    	{ 
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                
            struDccHead Head;
	
			memcpy((void *)&Head,(void *)(pCCR->m_sDccBuf),sizeof(Head));
		
			iRet=m_poPayRulesQuery->deal(pCCR,pCCA);

			if(iRet==0)
			{
	        		__DEBUG_LOG0_(0, "m_poPayRulesQuery->deal  ---0");
				m_pPublicConndb->Commit(oExp);
			}
			else
			{
	        		__DEBUG_LOG0_(0, "m_poPayRulesQuery->deal  ---1");
				m_pPublicConndb->RollBack(oExp);
			}
	       		break;
    	}
    	case ABMCMD_PAYRULE_RESET:// ֧����������
    	{ 
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                
            struDccHead Head;
	
			memcpy((void *)&Head,(void *)(pCCR->m_sDccBuf),sizeof(Head));
		
			iRet=m_poPayRulesReset->deal(pCCR,pCCA);

			if(iRet==0)
			{
	        		__DEBUG_LOG0_(0, "m_poPayRulesReset->deal  ---0");
				m_pPublicConndb->Commit(oExp);
			}
			else
			{
	        		__DEBUG_LOG0_(0, "m_poPayRulesReset->deal  ---1");
				m_pPublicConndb->RollBack(oExp);
			}
	       		break;
    	}
    	case ABMCMD_BINDINGACCNBR_QRY:// �󶨺����ѯ
    	{ 
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                
            struDccHead Head;
	
			memcpy((void *)&Head,(void *)(pCCR->m_sDccBuf),sizeof(Head));
		
			iRet=m_poBindingAccNbrQuery->deal(pCCR,pCCA);

			if(iRet==0)
			{
	        		__DEBUG_LOG0_(0, "m_poBindingAccNbrQuery->deal  ---0");
				m_pPublicConndb->Commit(oExp);
			}
			else
			{
	        		__DEBUG_LOG0_(0, "m_poBindingAccNbrQuery->deal  ---1");
				m_pPublicConndb->RollBack(oExp);
			}
	       		break;
    	}
    	case ABMPAYCMD_INFOR :
    	{
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
        	iRet=m_poPaymentInfo->deal(pCCR,pCCA);
		if(iRet==0)
		{
        		__DEBUG_LOG0_(0, "m_poPaymentInfo->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
        		__DEBUG_LOG0_(0, "m_poPaymentInfo->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
        	break;
    	}
  	case ABMCMD_ALLOCATE_BALANCE:
    	{
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                
                struDccHead Head;
	
		memcpy((void *)&Head,(void *)(pCCR->m_sDccBuf),sizeof(Head));
		
		if( strcmp(Head.m_sDestRealm,"")==0)
		{
			iRet=m_poActiveAllocateBalanceBiz->deal(pCCR,pCCA);
		}
		else
		{
			iRet=m_poAllocateBalance->deal(pCCR,pCCA);
		}
                
                //iRet=m_poAllocateBalance->deal(pCCR,pCCA);
		if(iRet==0)
		{
        		__DEBUG_LOG0_(0, "m_poAllocateBalance->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
        		__DEBUG_LOG0_(0, "m_poAllocateBalance->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
                break;
    	}
    	case ABMCMD_PLATFORM_DEDUCT_BALANCE:
    	{
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                iRet=m_poActiveAllocateBalance->deal(pCCR,pCCA);
		if(iRet==0)
		{
        		__DEBUG_LOG0_(0, "m_poAllocateBalance->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
        		__DEBUG_LOG0_(0, "m_poAllocateBalance->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
                break;
    	}
    	case ABMCMD_ACTIVE_ALLOCATE_BALANCE:
    	{
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                iRet=m_poActiveAllocateBalanceBiz->deal(pCCR,pCCA);
		if(iRet==0)
		{
        		__DEBUG_LOG0_(0, "m_poActiveAllocateBalanceBiz->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
        		__DEBUG_LOG0_(0, "m_poActiveAllocateBalanceBiz->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
                break;
    	}
	case ABMDEPOSITCMD_INFOR:
	{
        	__DEBUG_LOG1_(0, "ABMSvcSupt::��ֵ = %d", pCCR->m_iCmdID);
                iRet=m_poDepositInfo->deal(pCCR,pCCA);
		if(iRet==0)
		{
        		__DEBUG_LOG0_(0, "m_poAllocateBalance->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
        		__DEBUG_LOG0_(0, "m_poAllocateBalance->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
                break;
	}
	case ABM_REVERSEEPOSIT_CMD:
	{
        	__DEBUG_LOG1_(0, "ABMSvcSupt::��ֵ���� = %d", pCCR->m_iCmdID);
                iRet=m_pReverseDepositInfo->deal(pCCR,pCCA);
		if(iRet==0)
		{
        		__DEBUG_LOG0_(0, "m_pReverseDepositInfo->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
        		__DEBUG_LOG0_(0, "m_pReverseDepositInfo->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
                break;
	}
	case ABMCMD_REVERSE_DEDUCT_BALANCE:
    	{
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                iRet=m_poReverseDeductBalance->deal(pCCR,pCCA);
		if(iRet==0)
		{
        		__DEBUG_LOG0_(0, "m_poReverseDeductBalance->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
        		__DEBUG_LOG0_(0, "m_poReverseDeductBalance->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
                break;
    	}
		case ABMCMD_REVERSE_PAY:
    	{
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                iRet=m_poReversePay->deal(pCCR,pCCA);
		if(iRet==0)
		{
        		__DEBUG_LOG0_(0, "m_poReversePay->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
        		__DEBUG_LOG0_(0, "m_poReversePay->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
                break;
    	}
   case ABMCMD_QRY_SRV_PSWD:
   		{
   			__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
   			m_poQrySrv->deal(pCCR, pCCA);
			break;
   		}
   	case ABMCMD_QRY_SRV_SYN:
   		{
   			__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
   			
   			struDccHead Head;
			memcpy((void *)&Head,(void *)(pCCR->m_sDccBuf),sizeof(Head));
			if( strcmp(Head.m_sDestRealm,"")==0)
			{
				iRet=m_pSynRcv->deal(pCCR,pCCA);
			}else{
                iRet=m_poQryInfosyn->deal(pCCR,pCCA);
              }
			if(iRet==0)
			{
	        		__DEBUG_LOG0_(0, "m_poQryInfosyn->deal  ---0");
				m_pPublicConndb->Commit(oExp);
			}
			else
			{
	        		__DEBUG_LOG0_(0, "m_poQryInfosyn->deal  ---1");
				m_pPublicConndb->RollBack(oExp);
			}
                break;	
   		}
    case ABMCMD_QRY_BAL_INFOR:
    {
        struct struDccHead tmpHead;
		memcpy((void *)&tmpHead,(void *)(pCCR->m_sDccBuf),sizeof(tmpHead));
        if (strlen(tmpHead.m_sDestRealm) != 0)
        {
            __DEBUG_LOG0_(0, "������ѯ��������յ�CCR.");
            iRet = m_poHssQuery->deal(pCCR, pCCA);
            if(iRet == 0)
                {
                        __DEBUG_LOG0_(0, "m_poHssQuery->deal  ---0");
                        m_pPublicConndb->Commit(oExp);
                }
                else
                {
                        __DEBUG_LOG0_(0, "m_poHssQuery->deal  ---1");
                        m_pPublicConndb->RollBack(oExp);
                }
        }
        else
        {
            __DEBUG_LOG0_(0, "������ѯ��������յ�CCA.");
            iRet = m_poQueryAll->deal(pCCR, pCCA);
            if(iRet == 0)
                {
                        __DEBUG_LOG0_(0, "m_poQueryAll->deal  ---0");
                        m_pPublicConndb->Commit(oExp);
                }
                else
                {
                        __DEBUG_LOG0_(0, "m_poQueryAll->deal  ---1");
                        m_pPublicConndb->RollBack(oExp);
                }
        }
        break;
    }
    case ABMCMD_QRY_REC_INFOR:// 14.���˼�¼��ѯ.
        m_poHssQuery->deal(pCCR, pCCA);
        break;
    case ABMCMD_QRY_PAY_INFOR:// 15.���׼�¼��ѯ
        m_poHssQuery->deal(pCCR, pCCA);
        break;
    case ABMCMD_QRY_BIL_INFOR:// 16.�����˵���ѯ
        m_poHssQuery->deal(pCCR, pCCA);
        break;
    case ABMCMD_QRY_MICROPAY_INFO:
    	{
    		__DEBUG_LOG0_(0, "�Է��������.........CCR��");
        	iRet = m_poQueryAll->deal(pCCR, pCCA);
        	if(iRet == 0)
		{
			__DEBUG_LOG0_(0, "m_poQueryAll->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
			__DEBUG_LOG0_(0, "m_poQueryAll->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
        	
        	break;
	}
    case ABMCMD_LOGIN_PASSWD:
   	 		{
        			__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                iRet=m_pIdentify->deal(pCCR,pCCA);
		if(iRet == 0)
                {
                        __DEBUG_LOG0_(0, "m_pIdentify->deal  ---0");
                        m_pPublicConndb->Commit(oExp);
                }
                else
                {
                        __DEBUG_LOG0_(0, "m_pIdentify->deal  ---1");
                        m_pPublicConndb->RollBack(oExp);
                }
                break;
       	}
    case ABMCMD_PASSWD_IDENTIFY:
    		{
    			__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                iRet=m_pIdentifyBiz->deal(pCCR,pCCA);
		if(iRet == 0)
		{
			__DEBUG_LOG0_(0, "m_pIdentifyBiz->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
			__DEBUG_LOG0_(0, "m_pIdentifyBiz->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
                break;
    		}
    case ABMCMD_PASSWD_CHANGE_CCR:
    		{
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                iRet=m_pChange->deal(pCCR,pCCA);
                if(iRet == 0)
		{
			__DEBUG_LOG0_(0, "m_pChange->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
			__DEBUG_LOG0_(0, "m_pChange->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
                break;
       	}
    case ABMCMD_PASSWD_CHANGE_CCA:
    		{
    		__DEBUG_LOG1_(0, "ABMSvcSupt::deal  Command ID = %d", pCCR->m_iCmdID);
                iRet=m_pChangeBiz->deal(pCCR,pCCA);
                 if(iRet == 0)
		{
			__DEBUG_LOG0_(0, "m_pChangeBiz->deal  ---0");
			m_pPublicConndb->Commit(oExp);
		}
		else
		{
			__DEBUG_LOG0_(0, "m_pChangeBiz->deal  ---1");
			m_pPublicConndb->RollBack(oExp);
		}
                break;
    		}
    default:
    
        	__DEBUG_LOG1_(0, "ABMSvcSupt::deal Receive unknown Command ID = %d", pCCR->m_iCmdID);
        	break;
    }

    return 0;
    
}
