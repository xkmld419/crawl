#ifndef __ABM_SRV_SUPPORT_H_INCLUDED_
#define __ABM_SRV_SUPPORT_H_INCLUDED_

class ABMCCR;
class ABMCCA;
class ABMException;
class PaymentInfo;//֧����
class PublicConndb;//��������������
class AllocateBalance;//������
class DepositInfo;//��ֵ������
class ReverseDeductBalance;
class QryPasswordSend;//����������֤�·���
class QryServInfosyn;//��������������������ͬ��
class HssQuery;
class QueryAll;
class ActiveAllocateBalance; // ����������-CCR
class ActiveAllocateBalanceBiz; // ����������-CCA
class ReverseDepositInfo;//��ֵ����
class ReversePay; // ֧��������
class PasswdIdentify;	//�����Ȩ
class PasswdIdentifyBiz; //�����Ȩ
class PasswdChange;	//��½�����޸�
class PasswdChangeBiz;	//��½�����޸�
class ABMSynRcv;//��������CCAͬ������
class PayAbilityQuery;//֧������״̬��ѯ
class BalanceTransQuery;//���������ѯ
class BalanceTransReset;//������������
class PasswordReset;//֧�������޸�
class PayStatusReset;//֧��״̬���
class PayRulesQuery;//֧��״̬��ѯ
class PayRulesReset;//֧��״̬���
class BindingAccNbrQuery;//֧��״̬���

class ABMSvcSupt
{
	public:	ABMSvcSupt();	
		~ABMSvcSupt();
		int init(ABMException &oExp);	
		int deal(ABMCCR *pCCR, ABMCCA *pCCA);
	private:	
		PayAbilityQuery *m_poPayAbilityQuery;//֧������״̬��ѯ	
		BalanceTransQuery *m_poBalanceTransQuery;//֧������״̬��ѯ	
		BalanceTransReset *m_poBalanceTransReset;//֧������״̬��ѯ	
		PasswordReset *m_poPasswordReset;//֧�������޸�
		PayStatusReset *m_poPayStatusReset;//֧��״̬���
		PayRulesQuery *m_poPayRulesQuery;//֧�������ѯ
		PayRulesReset *m_poPayRulesReset;//֧��������
		BindingAccNbrQuery *m_poBindingAccNbrQuery;//�󶨺����ѯ
		
		PaymentInfo  *m_poPaymentInfo;//֧��	
		AllocateBalance *m_poAllocateBalance;//����	
		PublicConndb *m_pPublicConndb;//�������ݿ�����	
		DepositInfo  *m_poDepositInfo;//��ֵ֧��	
		ReverseDeductBalance *m_poReverseDeductBalance;	
		QryPasswordSend *m_poQrySrv;//����������֤�·�	
		QryServInfosyn *m_poQryInfosyn;//��������������������ͬ��        
		HssQuery     *m_poHssQuery;          
		QueryAll     *m_poQueryAll;        
		ActiveAllocateBalance *m_poActiveAllocateBalance; //��������-CCR        
		ActiveAllocateBalanceBiz *m_poActiveAllocateBalanceBiz; //��������-CCA	
		ReverseDepositInfo *m_pReverseDepositInfo;//��ֵ����	
		ReversePay *m_poReversePay; // ֧������		
		PasswdIdentify *m_pIdentify;//�����Ȩ��CCR	
		PasswdIdentifyBiz *m_pIdentifyBiz;//�����Ȩ��CCA��CCA	
		PasswdChange *m_pChange;	//�����޸ķ�CCR	
		PasswdChangeBiz *m_pChangeBiz;	//�����޸���CCA��CCA	
		ABMSynRcv *m_pSynRcv;//��������ͬ����CCA
};
#endif /*__ABM_SRV_SUPPORT_H_INCLUDED_*/
