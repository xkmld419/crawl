#ifndef __USERSELF_SERVICE_H_INCLUDED_
#define __USERSELF_SERVICE_H_INCLUDED_

#include "dcc_ra.h"

#include "abmpaycmd.h"
#include "DccOperation.h"

class ABMException;
class PayAbilityQuerySql;
class BalanceTransQuerySql;
class BalanceTransResetSql;
class PasswordResetSql;
class DccOperation;
class PayStatusResetSql;
class PayRulesQuerySql;
class PayRulesResetSql;
class BindingAccNbrQuerySql;
class TransferRulesSql;
class PaymentInfoSql;
/*
 * 
 *
 * �Է���ƽ̨֧������״̬��ѯ
 * 
 * 2011.7.14
 */
class PayAbilityQuery
{
	public:
		PayAbilityQuery();
		~PayAbilityQuery();	
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pCCA);
		int _deal(R_PayAbilityQuerySvc *pSvcIn,vector<R_MicropaymentInfo *> g_vRMicropaymentInfo);
	private:
		
		PayAbilityQuerySql *m_poSql;
			
		PayAbilityQueryCCR *m_poUnpack;
		PayAbilityQueryCCA *m_poPack;	
		
		
		DccOperation *m_dccOperation;
		
};

/*
 * 
 *
 * �Է���ƽ̨���������ѯ
 * 
 * 2011.7.14
 */
class BalanceTransQuery
{
	public:
		BalanceTransQuery();
		~BalanceTransQuery();	
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pCCA);
		int _deal(R_BalanceTransSvc *pSvcIn,vector<R_BTMicropaymentInfo *> g_vRBTMicropaymentInfo);
	private:
		
		BalanceTransQuerySql *m_poSql;
			
		BalanceTransCCR *m_poUnpack1;
		BalanceTransCCA *m_poPack1;	
		
		
		DccOperation *m_dccOperation;
		
};

/*
 * 
 *
 * �Է���ƽ̨������������
 * 
 * 2011.7.14
 */
class BalanceTransReset
{
	public:
		BalanceTransReset();
		~BalanceTransReset();	
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pCCA);
		int _deal(R_BalanceTransResetSvc *pSvcIn,vector<R_TRMicropaymentInfo *> g_vRTRMicropaymentInfo);
	private:
		
		BalanceTransResetSql *m_poSql;
		PasswordResetSql *m_poSqlPassBTR;
		TransferRulesSql *m_poSqlRules;
		
		PaymentInfoSql *m_poSqlOpt;
		
		BalanceTransResetCCR *m_poUnpack1;
		BalanceTransResetCCA *m_poPack1;	
		
		
		DccOperation *m_dccOperation;
		
};

/*
 * �Է���ƽ̨֧�������޸�
 * 
 * 2011.7.14
 */
class PasswordReset
{
	public:
		PasswordReset();
		~PasswordReset();	
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pCCA);
		int _deal(R_PasswordReset *pSvcIn,vector<R_PRMicropaymentInfo *> g_vPRMicropaymentInfo);
	private:
		
		PasswordResetSql *m_poSql;
			
		PasswordResetCCR *m_poUnpack2;
		PasswordResetCCA *m_poPack2;	
		PaymentInfoSql *m_poSqlOpt;
		
		DccOperation *m_dccOperation;
		
};


/*
 * 
 *
 * �Է���ƽ̨֧������״̬���
 * 
 * 2011.7.14
 */
class PayStatusReset
{
	public:
		PayStatusReset();
		~PayStatusReset();	
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pCCA);
		int _deal(R_PayStatusReset *pSvcIn,vector<R_PSRMicropaymentInfo *> g_vRPSRMicropaymentInfo);
	private:
		
		PayStatusResetSql *m_poSql;
		PasswordResetSql *m_poSqlPasswd;
			
		PayStatusResetCCR *m_poUnpack;
		PayStatusResetCCA *m_poPack;	
		PaymentInfoSql *m_poSqlOpt;
		
		
		DccOperation *m_dccOperation;
		
};

/*
 * 
 *
 * �Է���ƽ̨֧�������ѯ
 * 
 * 2011.7.14
 */
class PayRulesQuery
{
	public:
		PayRulesQuery();
		~PayRulesQuery();	
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pCCA);
		int _deal(R_PayRulesQuerySvc *pSvcIn,vector<R_PRQMicropaymentInfo *> g_vPRQMicropaymentInfo);
	private:
		
		PayRulesQuerySql *m_poSql;
			
		PayRulesQueryCCR *m_poUnpack;
		PayRulesQueryCCA *m_poPack;	
		
		
		DccOperation *m_dccOperation;
		
};

/*
 * 
 *
 * �Է���ƽ̨֧��������
 * 
 * 2011.7.14
 */
class PayRulesReset
{
	public:
		PayRulesReset();
		~PayRulesReset();	
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pCCA);
		int _deal(R_PayRulesResetSvc *pSvcIn,vector<R_PRRMicropaymentInfo *> g_vPRRMicropaymentInfo);
	private:
		
		PayRulesResetSql *m_poSql;
		PasswordResetSql *m_poSqlPass;
		TransferRulesSql *m_poSqlPayRules;
		PaymentInfoSql *m_poSqlOpt;
		
		PayRulesResetCCR *m_poUnpack;
		PayRulesResetCCA *m_poPack;	
		
		
		DccOperation *m_dccOperation;
		
};

/*
 * 
 *
 * �Է���ƽ̨֧��������
 * 
 * 2011.7.14
 */
class BindingAccNbrQuery
{
	public:
		BindingAccNbrQuery();
		~BindingAccNbrQuery();	
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pCCA);
		int _deal(R_BindingAccNbrQuery *pSvcIn,vector<R_BDAMicropaymentInfo *> g_vRBDAMicropaymentInfo);
	private:
		
		BindingAccNbrQuerySql *m_poSql;

		BindingAccNbrQueryCCR *m_poUnpack;
		BindingAccNbrQueryCCA *m_poPack;	
		
		
		DccOperation *m_dccOperation;
		
};

#endif