#ifndef __DCC_RA_H_INCLUDED_
#define __DCC_RA_H_INCLUDED_

#include "abmcmd.h"
#include "abmobject.h"

/*
#define UTF8_MAX  32
#define TEXT_MAX  256
*/
typedef unsigned int ui32;

typedef unsigned long ui64;
/*
#define ABMCMD_ALLOCATE_BALANCE  6684801   //��������

#define ABMCMD_REVERSE_PAY  6684802   //֧������

#define ABMCMD_ACTIVE_ALLOCATE_BALANCE 6684803 // ��������

#define ABMCMD_REVERSE_DEDUCT_BALANCE 6684804 // ��������

#define ABMCMD_PLATFORM_DEDUCT_BALANCE 6684805 // �Է���ƽ̨��������

#define ABMCMD_PLATFORM_DEDUCT_BALANCE_BIZ 6684806 // 

*/
/*
struct ResponsStruct
{
	char m_sResID[UTF8_MAX];
	int m_iRltCode;
	int m_iAlign;
};
*/

/*=============================��������======================*/
// �û��ʻ���Ϣ
struct ServAcctInfo
{
	long m_servId;			// �û���ʶ
	long m_acctId;			// �ʻ���ʶ
	char m_servNbr[UTF8_MAX];	// �û�����
	long m_nbrType;		// ��������	
};

// ������������
struct TransRule
{
	long lAcctId;
	long lDayTransFreq;	// ÿ��Ƶ��
	long lTransLimit;	// ÿ���޶�
	long lMonthTransLimit;	// ÿ���޶�
				// �Զ�������ʶ
				// �Զ�������ֵ
				// �Զ��������
	char sType[UTF8_MAX];		// �޶����ͣ�0�����죬1������
		 	
};

/*
// ����CCR��Ϣ - ������Ϣ
struct R_AllocateBalanceSvc
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_servNbr[UTF8_MAX];	// �û�����
	short m_nbrType;		// ��������
	short m_servAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char m_areaCode[UTF8_MAX];	// �绰����
	ui32 m_staffId;			// ����ԱID
	char m_deductDate[32];		// �۷�ʱ��
	int m_iDeductDate;
	int m_iTest;// ����
	char m_abmId[UTF8_MAX];		// ����۷�ABM��ʶ
};

// ����CCR��Ϣ - ����۷ѵ���Ŀ��ϸ��Ϣ
struct R_RequestDebitAcctItem
{
	//char m_billingCycle[UTF8_MAX];	// ��������YYYYMM	
	ui32 m_billingCycle;			// ��������YYYYMM	
	char m_acctItemTypeId[UTF8_MAX];	// ��Ŀ���ͱ�ʶ	
	ui32 m_balanceUnit;			// ��λ-��
	long m_deductAmount;			// �������
	int m_chargeType;			// ����һ����ȫ������	
};

// ����CCA��Ϣ - ������Ϣ
struct A_AllocateBalanceSvc
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	long m_lAcctId;			// �۷������ʶ
	ui32 m_balanceUnit;		// ��λ-��	
	long m_deductAmount;		// �ɹ��������
	long m_acctBalance;		// �ʻ�ʣ����
};

// ����CCA��Ϣ - ��Ӧ�۷ѵ���Ŀ��ϸ��Ϣ
struct A_ResponseDebitAcctItem
{
	//char m_billingCycle[UTF8_MAX];	// ��������YYYYMM	
	ui32 m_billingCycle;		// ��������YYYYMM	
	char m_acctItemTypeId[UTF8_MAX];// ��Ŀ���ͱ�ʶ	
	ui64 m_acctBalanceId;		// �۷��˱���ʶ
	long m_deductAmount;		// �ʱ��������
	long m_acctBalanceAmount;	// �ʱ�ʣ����	
};

class AllocateBalanceCCR : public ABMTpl<__DFT_T1_,R_AllocateBalanceSvc, R_RequestDebitAcctItem, ABMCMD_ALLOCATE_BALANCE>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		
		// ���
		bool addAllocateBalanceSvc(R_AllocateBalanceSvc &data)
		{
			return __add2(data);
		}
		
		bool addRequestDebitAcctItem(R_RequestDebitAcctItem &data)
		{
			return __add3(data);	
		}
	
		// ���
		// CCR����ȡ����������Ϣ
		int getR_AllocateBalanceSvc(vector<R_AllocateBalanceSvc *> &vec)
		{
			return __getValue2(vec);
		}
		// CCR����ȡ������Ŀ��ϸ��Ϣ
		int getR_RequestDebitAcctItem(R_AllocateBalanceSvc *pSvc,vector<R_RequestDebitAcctItem *> &vec)
		{
			return __getValue3(pSvc,vec);	
		}
	
};

class AllocateBalanceCCA : public ABMTpl<__DFT_T1_, A_AllocateBalanceSvc, A_ResponseDebitAcctItem, ABMCMD_ALLOCATE_BALANCE>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���

		bool addResBalanceSvcData(A_AllocateBalanceSvc &data)
		{
			return __add2(data);
		}
	
		bool addResDebitAcctData(A_ResponseDebitAcctItem &data)
		{
			return __add3(data);
		}
	
		// ���

		
		int getResBalanceSvcData(vector<A_AllocateBalanceSvc*> &vec)
		{
			return __getValue2(vec);
		}

		int getAcctBalanceData(A_AllocateBalanceSvc *pPtr, vector<A_ResponseDebitAcctItem *> &vec)
		{
			return __getValue3(pPtr, vec);
		}
};

*/

/*============================�����������============================*/
/*
// CCR
struct ReqBalanceSvc
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_destinationId[UTF8_MAX];	// ����ֵ�û�����
	int m_destinationIdType;	// ��������
	int m_destinationAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char m_areaCode[UTF8_MAX];	// �绰����
	char m_requestTime[32];		// ����ʱ��
	int m_iRequestTime;		
	ui32 m_balanceUnit;		// ��λ
	long m_rechargeAmount;		// ��ֵ���
	int m_blanceType;		// �������
	ui32 m_test;// ����
};

// CCA
struct ResBalanceSvc
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; // ������Ϣ
	char m_responseTime[32];	// ��Ӧʱ��
	int m_iResponseTime;
	ui32 m_balanceUnit;		// ��λ
	long m_balanceAmount;		// ������ʡABM�ʻ����
	int m_blanceType;		// �������
};

class ActiveAllocateBalanceCCR : public ABMTpl<__DFT_T1_,ReqBalanceSvc,__DFT_T3_, ABMCMD_ACTIVE_ALLOCATE_BALANCE>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		
		// ���
		bool addReqBalanceSvc(ReqBalanceSvc &data)
		{
			return __add2(data);
		}
	
		// ���
		// CCR����ȡ����������Ϣ
		int getReqBalanceSvc(vector<ReqBalanceSvc *> &vec)
		{
			return __getValue2(vec);
		}
};

class ActiveAllocateBalanceCCA : public ABMTpl<__DFT_T1_,ResBalanceSvc,__DFT_T3_, ABMCMD_ACTIVE_ALLOCATE_BALANCE>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���
		bool addResBalanceSvc(ResBalanceSvc &data)
		{
			return __add2(data);
		}
		
		int getResBalanceSvc(vector<ResBalanceSvc *> &vec)
		{
			return __getValue2(vec);
		}

};
*/
/*===========================�Է���ƽ̨��������==================================*/
// �Է���ƽ̨����CCR
// CCR ������Ϣ
/*
struct ReqPlatformSvc
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[64];		// ����ʱ��
	int m_iRequestTime;
	int m_test;// ����
};

// ֧������Ϣ��
struct ReqPlatformPayInfo
{
	char m_sDestinationAccount[UTF8_MAX];	// �û�����
	int m_iDestinationType;			// ��ʶ����
	int m_iDestinationAttr;			// �û�����
	char m_sSvcPlatformId[UTF8_MAX];	// ҵ��ƽ̨��ʶ
	char m_sPayPassword[UTF8_MAX];		// ֧������
	int m_iBalanceTransFlag;		// ��������
	int m_iUnitTypeId;			// ���ĵ�λ����
	long m_lDeductAmount;			// �軮�����
	
};

// CCA ������Ϣ
struct ResPlatformSvc
{
	ui32 m_svcResultCode;			// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX];	// ������Ϣ
	char m_responseId[UTF8_MAX];		// ��Ӧ��ˮ
	char m_responseTime[UTF8_MAX];		// ��Ӧʱ��	
	int m_iResponseTime;
	//int iTest;// ����
	
	int m_iUnitTypeId;			// ���ĵ�λ����
	long m_lDeductAmount;			// �ɹ������Ľ��
	int m_iTest;// ����
	
};

// ֧������Ϣ
struct ResPlatformPayInfo
{
	int m_iUnitTypeId;			// ���ĵ�λ����
	long m_lDeductAmount;			// �ɹ������Ľ��
	int m_iTest;// ����
};

// ����ʱ���ϸ
struct ResPlatformAcctBalance
{
	int m_iBalanceSource;			// ����ʱ���Դ
	long m_lBalanceAmount;			// ����ʱ���ǰ���	
	int m_iTest;//����
};

// CCR������
class PlatformDeductBalanceCCR : public ABMTpl<__DFT_T1_,ReqPlatformSvc,ReqPlatformPayInfo, ABMCMD_PLATFORM_DEDUCT_BALANCE>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		
		// ���-��һ�������Ϣ
		bool addReqPlatformSvc(ReqPlatformSvc &data)
		{
			return __add2(data);
		}
	
		// ���-�ڶ���֧����Ϣ
		bool addReqPlatformPayInfo(ReqPlatformPayInfo &data)
		{
			return __add3(data);	
		}
	
	
		// ���-CCR����ȡ����������Ϣ
		int getReqPlatformSvc(vector<ReqPlatformSvc *> &vec)
		{
			return __getValue2(vec);
		}
		
		// ���-CCR���ڶ���֧����Ϣ
		int getReqPlatformPayInfo(ReqPlatformSvc *pSvc,vector<ReqPlatformPayInfo *> &vec)
		{
			return __getValue3(pSvc,vec);	
		}
};
*/

// CCA������
/*
class PlatformDeductBalanceCCA : public ABMTpl<ResPlatformSvc,ResPlatformPayInfo,ResPlatformAcctBalance, ABMCMD_PLATFORM_DEDUCT_BALANCE>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���
		bool addResPlatformSvc(ResPlatformSvc &data)
		{
			return __add1(data);
		}
		

		bool addResPlatformPayInfo(ResPlatformPayInfo &data)
		{
			return __add2(data);
		}
	
		bool addResPlatformAcctBalance(ResPlatformAcctBalance &data)
		{
			return __add3(data);
		}
	
		// ���
		int getResPlatformSvc(vector<ResPlatformSvc *> &vec)
		{
			return __getValue1(vec);
		}
		

		
		//int getResPlatformPayInfo(ResPlatformSvc *pSvc,vector<ResPlatformPayInfo *> &vec)
		//{
		//	return __getValue3(pSvc,vec);
		//}
		
		int getResPlatformPayInfo(vector<ResPlatformPayInfo *> &vec)
		{
			return __getValue2(vec);
		}
		
		int getResPlatformAcctBalance(ResPlatformPayInfo *pPtr,vector<ResPlatformAcctBalance *> &vec)
		{
			return __getValue3(pPtr,vec);
		}
		
		
	
};
*/
/*
class PlatformDeductBalanceCCA : public ABMTpl<__DFT_T1_,ResPlatformSvc,ResPlatformAcctBalance, ABMCMD_PLATFORM_DEDUCT_BALANCE>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���
		bool addResPlatformSvc(ResPlatformSvc &data)
		{
			return __add2(data);
		}
			
		bool addResPlatformAcctBalance(ResPlatformAcctBalance &data)
		{
			return __add3(data);
		}
	
		// ���
		int getResPlatformSvc(vector<ResPlatformSvc *> &vec)
		{
			return __getValue2(vec);
		}
		
		int getResPlatformAcctBalance(ResPlatformSvc *pPtr,vector<ResPlatformAcctBalance *> &vec)
		{
			return __getValue3(pPtr,vec);
		}
};
*/


/*===========================��������===============================*/
/*
// CCR
struct ReqReverseDeductSvc
{
	char m_sRequestId[UTF8_MAX];		// ������ˮ
	char m_sRequestTime[32];		// ����ʱ��
	char m_sOldRequestId[UTF8_MAX];		// �������Ļ�����ˮ��
	char m_sABMId[UTF8_MAX];		// �������ABM��ʶ
	int m_iActionType;			// ������������,0:�������,1:��������
};

// CCA
struct ResReverseDeductData
{
	char m_sResponseId[UTF8_MAX];		// ������ˮ
	ui32 m_iServiceRltCode;			// ҵ�񼶵Ľ������
	char m_sResponseTime[32];		// ��Ӧʱ��
	int m_iTest;
};

class ReverseDeductBalanceCCR : public ABMTpl<__DFT_T1_,ReqReverseDeductSvc,__DFT_T3_, ABMCMD_REVERSE_DEDUCT_BALANCE>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		// ���
		bool addReqReverseDeductSvc(ReqReverseDeductSvc &data)
		{
			return __add2(data);
		}
	
		// ���
		// CCR����ȡ����������Ϣ
		int getReqReverseDeductSvc(vector<ReqReverseDeductSvc *> &vec)
		{
			return __getValue2(vec);
		}
};

class ReverseDeductBalanceCCA : public ABMTpl<__DFT_T1_,ResReverseDeductData,__DFT_T3_, ABMCMD_REVERSE_DEDUCT_BALANCE>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���
		bool addResReverseDeductData(ResReverseDeductData &data)
		{
			return __add2(data);
		}
		
		int getResReverseDeductData(vector<ResReverseDeductData *> &vec)
		{
			return __getValue2(vec);
		}

};
*/

/*===========================֧������===================================*/
/********************************************
 * ֧�������ӿڶ���
 *
 ****************************************** */
/*
struct ReversePaySvc
{
	long lOperationAction;			// ����������,9 - �۷�ȡ��
	char sOperationSeq[UTF8_MAX];		// ������ˮ
	char sReqSerial[UTF8_MAX];		// ��Ҫȡ���۷ѵ�ԭ֧��������ˮ��
	char sSpId[UTF8_MAX];			// �̼�ID
	char sServicePlatformId[UTF8_MAX];	// ƽ̨ID
	ui32 iStaffId;				// ����ԱID
	ui32 iTest;// ����
};

struct ReversePayRes1
{
	long lServiceRltCode;		// ҵ�񼶵Ľ������
	char sParaFieldResult[UTF8_MAX];// ������Ϣ
};

struct ReversePayRes2
{
	char sOperateSeq[UTF8_MAX];	// ����������ˮ
	char sReqSerial[UTF8_MAX];	// �Ʒ�ϵͳ�ĳ�����ˮ��
	char sParaFieldResult[UTF8_MAX];// ������Ϣ
};

class ReversePayCCR : public ABMTpl<ReversePaySvc,__DFT_T2_, __DFT_T3_, ABMCMD_REVERSE_PAY>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		
		// ���
		bool addReversePaySvc(ReversePaySvc &data)
		{
			return __add1(data);
		}
		
		// ���
		// CCR����ȡ����������Ϣ
		int getReversePaySvc(vector<ReversePaySvc *> &vec)
		{
			return __getValue1(vec);
		}
};

class ReversePayCCA : public ABMTpl<__DFT_T1_, ReversePayRes1, ReversePayRes2, ABMCMD_REVERSE_PAY>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���
		bool addReversePayCCA1(ReversePayRes1 &data)
		{
			return __add2(data);
		}
	
		bool addReversePayCCA2(ReversePayRes2 &data)
		{
			return __add3(data);
		}
	
		// ���	
		int getReversePayCCA1(vector<ReversePayRes1*> &vec)
		{
			return __getValue2(vec);
		}

		int getReversePayCCA2(ReversePayRes1 *pPtr, vector<ReversePayRes2 *> &vec)
		{
			return __getValue3(pPtr, vec);
		}
};
*/


/*
// ���֧����¼�ṹ����
struct BalancePayoutInfo
{
	long m_lOperPayoutId; 	// ֧��������ˮ
	long m_lAcctBalanceId;	// ����ʱ���ʶ
	long m_lBillingCycleId; // �������ڱ�ʶ
	long m_lOptId;		// ҵ����ˮ
	char m_sOperType[3];	// ��������
	char m_sOperDate[32];	// ����ʱ��
	long m_lAmount;		// ���
	long m_lBalance;	// ��������
	long m_lPrnCount;	// ��ӡ����
	char m_sState[3];	// ״̬
	char m_sStateDate[32];	// ״̬����
	long m_lStaffId;	// ����Ա����	
}
*/

#endif /*__DCC_RA_H_INCLUDED_*/
