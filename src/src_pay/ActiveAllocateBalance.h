#ifndef __ACTIVE_Allocate_BALANCE_H_INCLUDED_
#define __ACTIVE_Allocate_BALANCE_H_INCLUDED_

#include "dcc_ra.h"

#include "abmpaycmd.h"
#include "PaymentInfo.h"
#include "DccOperation.h"

class ABMException;
class AllocateBalanceSql;
class PaymentInfo;
class DccOperation;

/*
 * ֧���ʻ������� - ��������
 *
 * ����ABM�����Է���ƽ̨CCR���жϣ�
 * 1.ʡ��ȫ��(ʡABM��������)   - ����ABM������ֵ
 * 2.ȫ����ʡ(����ABM��������) - ����ABM����
 *
 * 2011.5.16
 **/
class ActiveAllocateBalance
{
	public:
		ActiveAllocateBalance();
		~ActiveAllocateBalance();	
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pCCA); // �������յ�CCR
		int _deal(ReqPlatformSvc *pSvcIn,vector<ReqPlatformPayInfo *> g_vReqPlatformPayInfo);
		
		int deductBalanceIn(ReqPlatformSvc *pSvcIn,ReqPlatformPayInfo *g_itr2);
		int deductBalanceOut(ReqPlatformSvc *pSvcIn,ReqPlatformPayInfo *g_itr2);
	private:
		
		AllocateBalanceSql *m_poSql;
			
		PlatformDeductBalanceCCR *m_poUnpack; // ���-�Է���ƽ̨��CCR��
		AllocateBalanceCCR *m_poPack;         // ���-����ʡABM������CCR��	
		ActiveAllocateBalanceCCR *m_poActivePack; // ���-��������������CCR��
		
		PaymentInfo *m_paymentInfo;		
		
		DccOperation *m_dccOperation;
};
 

#endif /*__ACTIVE_Allocate_BALANCE_H_INCLUDED_*/