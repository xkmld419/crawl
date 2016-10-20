// ActiveAllocateBalanceBiz.h 
#ifndef __ACTIVE_ALLOCATE_BALANCE_BIZ_H_INCLUDED_
#define __ACTIVE_ALLOCATE_BALANCE_BIZ_H_INCLUDED_

#include "dcc_ra.h"

#include "abmpaycmd.h"
#include "PaymentInfo.h"
#include "DccOperation.h"
#include "DepositInfo.h"

class ABMException;
class AllocateBalanceSql;
class PaymentInfo;
class DccOperation;
class DepositInfo;

extern bool g_toPlatformFlag;

/*
 * ֧���ʻ������� - ��������
 *
 * ����ABM����ʡABM��CCA�󣬴�TT�в����Է���ƽ̨�����CCR���ж�
 * 1.ʡ��ȫ��(ʡABM��������)   - ����ABM������ֵ
 * 2.ȫ����ʡ(����ABM��������) - ����ABM����
 *
 * 2011.5.27
 **/
class ActiveAllocateBalanceBiz
{
	public:
		ActiveAllocateBalanceBiz();
		~ActiveAllocateBalanceBiz();	
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pCCA); // ������յ�CCA��������CCA
			
		int deductBalanceIn(ReqPlatformSvc &reqSvc,ReqPlatformPayInfo &reqPayInfo);
		int deductBalanceOut(ReqPlatformSvc &reqSvc,ReqPlatformPayInfo &reqPayInfo);
	private:
		
		AllocateBalanceSql *m_poSql;
			
		AllocateBalanceCCA *m_poUnpack; 		// ���-ʡABM���ص�CCA����ʡ��ȫ��
		ActiveAllocateBalanceCCA *m_poActiveUnpack;	// ���-ʡABM���ص�CCA����ȫ����ʡ
		
		PlatformDeductBalanceCCA *m_poPack;	// ���-���ظ��Է���ƽ̨��CCA��
			
		PaymentInfo *m_paymentInfo;		
		
		DccOperation *m_dccOperation;
		
		DepositInfo *m_depositInfo;
};

#endif /*__ACTIVE_ALLOCATE_BALANCE_BIZ_H_INCLUDED_*/