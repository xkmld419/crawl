#ifndef __PAYMENT_INFO_SQL_H_INCLUDED_
#define __PAYMENT_INFO_SQL_H_INCLUDED_

#include "TTTns.h"
#include "abmpaycmd.h"
#include "LogV2.h"
#include "PublicConndb.h"

class ABMException;

class PaymentInfoSql : public TTTns
{

public:

	PaymentInfoSql();
	~PaymentInfoSql();
	//����У��
	int ComparePasswd(AbmPayCond *&pAbmPayCond,PaymentInfoALLCCR &pPaymentInfoCCR,bool &bState);
	//��ѯ����
	int QueryPaySeq(StructPaymentSeqInfo &sStructPaymentSeqInfo,bool &bState);
	//��¼ҵ���
	int InsertPaymentInfo(PaymentInfoALLCCR &pPaymentInfoCCR,StructPaymentSeqInfo  &sStructPaymentSeqInfo);
	//��ѯ�˱���
	int QueryAcctBalanceInfo(PaymentInfoALLCCR &pPaymentInfoCCR,vector<AcctBalanceInfo *>&vAcctBalanceInfo);

	//��ѯ�����Դ�� 
	int QueryBalanceSourceInfo(long  &m_lAcctBalanceID,vector<BalanceSourceInfo *>&vBalanceSourceInfo);
	//��¼֧����
	int InsertBalancePayout(StructPaymentSeqInfo &sStructPaymentSeqInfo,BalanceSourceInfo *&pBalanceSourceInfo,const string &m_sOperType);
	//�����˱���
	int updateAcctBalance(StructPaymentSeqInfo &sStructPaymentSeqInfo,AcctBalanceInfo *&pAcctBalanceInfo);
	//���������Դ��
	int updateBalanceSource(StructPaymentSeqInfo &sStructPaymentSeqInfo,BalanceSourceInfo *&pBalanceSourceInfo);
	//������Դ֧����ϵ��
	int InsertBalanceSourcePayoutRela(StructPaymentSeqInfo &sStructPaymentSeqInfo,BalanceSourceInfo *&pBalanceSourceInfo);
	// ��ѯ֧����
	int qryPayoutInfo(long lOldRequestId,vector<BalancePayoutInfo *>&vBalancePayOut);
	// ��ѯ�����Դ��Ϣ
	int qryBalanceSource(long lOptId,BalanceSourceInfo *&pBalanceSourceInfo);
	// �������֧����¼״̬
	int updateBalancePayout(long lOperPayoutId);
	int updateBalanceSourcePayoutRela(long lOperPayoutId);
	//��ѯ����
	int QueryServAcctSeq(ServAcctInfoSeq &oServAcctInfoSeq,bool &bState);
	//��¼serv
	int InsertServInfo(ServAcctInfoSeq &oServAcctInfoSeq,DepositServCondOne *&pInOne);
	//��¼acct
	int InsertAcctInfo(ServAcctInfoSeq &oServAcctInfoSeq,DepositServCondOne *&pInOne);
	//��¼serv_acct
	int InsertServAcctInfo(ServAcctInfoSeq &oServAcctInfoSeq,DepositServCondOne *&pInOne);
	//��¼acct_balance
	int InsertAcctBalance(ServAcctInfoSeq &oServAcctInfoSeq,DepositServCondOne *&pInOne);
	//��¼balance_source
	int InsertBalanceSource(DepositServCondOne *&pInOne,StructPaymentSeqInfo &sStructPaymentSeqInfo);
	//��ѯacct_balance
	int QueryAcctBalanceSimple(DepositServCondOne *&pInOne,AcctBalanceInfo *&pAcctBalanceInfo);
	//��¼user_info_add
	int InsertUserInfoAdd(ServAcctInfoSeq &oServAcctInfoSeq,DepositServCondOne *&pInOne);
	
	//��ѯacct_balance
	int qryAcctBalance(long &lAcctBalanceId,AcctBalanceInfo *&pAcctBalanceInfo);
	// ��¼�����Դ - ����
	int insertBalanceSource(BalanceSourceInfo &balanceSourceInfo);
	//�����û�֧������
	int InsertAttUserInfo(ServAcctInfoSeq &oServAcctInfoSeq,bool &bState);
	//��¼�û�����
	int InsertAppUserInfo(DepositServCondOne *&pInOne,ServAcctInfoSeq &oServAcctInfoSeq);
	// ����ҵ����ˮ״̬ - ��������
    	int updateOperation(long lOperPayoutId);
    	// �����Դ��ѯ - ��������
    	int qryBalanceSource(long lOptId,vector<BalanceSourceInfo *>&vBalanceSource);
    	// ��������ʱ���¼ - ������������
    	int updateAcctBalance(long &lAcctBalanceId,long &lPaymentAmount);
	//��ѯҵ���¼
	int QryPaymentInfo(long &lPaymentID,PaymentInfoALLCCR &pPaymentInfoCCR);
	int QueryOptID(char *m_sForeignID,long &lPaymentID);
    
    	int qryOptidByForeignId(char *sForeignId,long &lOptId);
    	
    	int qryOptInfoByForeignId(char *sForeignId,long &lOptId,long &lServId);
};

#endif/*__PAYMENT_INFO_SQL_H_INCLUDED_*/

