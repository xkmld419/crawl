// ActiveSendReverseBiz.h
#ifndef __ACTIVE_SEND_REVERSE_BIZ_H_INCLUDED_
#define __ACTIVE_SEND_REVERSE_BIZ_H_INCLUDED_

class ABMCCR;
class ABMCCA;
class ABMException;
class PublicConndb;//��������������

class ActiveSendReverseBizSql;
class DccOperation;
class ReverseDeductBalanceCCR;

class ActiveSendReverseBiz
{
	public:	
		ActiveSendReverseBiz();	
		~ActiveSendReverseBiz();
		int init(ABMException &oExp);	
		int deal(ABMCCR *pSendCCR);
		
	private:	
		PublicConndb *m_pPublicConndb;//�������ݿ�����
		
		ActiveSendReverseBizSql *m_pActiveReverseSql; 	
		
		DccOperation *m_dccOperation;
		
		ReverseDeductBalanceCCR *pSendPack;
};
#endif /*__ACTIVE_SEND_REVERSE_BIZ_H_INCLUDED_*/








