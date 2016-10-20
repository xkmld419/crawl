/*
 *�û���ҵ��ƽ̨��ʹ��ȫ������ABMͳһ֧���ʻ�����֧����
 *ҵ��ƽ̨����֧�������ȫ������ABM��,���յ����󣬵�HSS�н���֧������ļ�Ȩ(Ŀǰ���ڼ���abm��Ȩ),
 *��Ȩ�ɹ���ABM��ɿ۷ѣ�������֧����Ӧ��ҵ��ƽ̨
 *2011/05/08
 */
#ifndef __ABMPAYCMD_H_INCLUDED_
#define __ABMPAYCMD_H_INCLUDED_

#include "abmobject.h"
#include "abmcmd.h"
#include "errorcode_pay.h"

#define UTF8_MAX_32  32
#define TEXT_MAX_256  256
//typedef unsigned int ui32;
typedef unsigned long ui64;
//#define ABMPAYCMD_INFOR  9   //֧������
//#define ABMDEPOSITCMD_INFOR  10   //��ֵ��������
//#define ABM_REVERSEEPOSIT_CMD  11   //��ֵ����

//�����ڲ�ʹ��
typedef struct _Struct_Payment_Info
{
	long m_lPaymentSeq;//�۷���ˮ
	long m_lBalanceSourceSeq;//�����Դ��ˮ
	long m_lBalancePayoutSeq;//���֧����ˮ
	long m_lSourcePayoutSeq;//�����Դ֧����ˮ
	long m_lPaymentAmount;//ÿ�θ�����
	long m_lRbkPaymentSeq;//ԭ�۷���ˮ
	long m_lAcctBalanceID;//�˱���seq
}StructPaymentSeqInfo;
//����˱���Ϣ��
typedef struct _Acct_Balance_Info
{
   long m_lAcctBalanceID;
   long m_lBalanceTypeID;
   char m_sEffDate[UTF8_MAX_32];
   char m_sExDate[UTF8_MAX_32];
   long  m_lBalance;
   long m_lCycleUpper;
   long m_lCycleLower;
   char m_sCycleUpperType[UTF8_MAX_32];
   char m_sCycleLowerType[UTF8_MAX_32];
   long m_lAcctID;
   long m_lServID;
   long m_lItemGroupID;
   char  m_sState[UTF8_MAX_32];
   char  m_sStateDate[UTF8_MAX_32];
   char  m_sBankAcct[UTF8_MAX_32];
   long m_lObjectTypeID;
   void clear()
   {
	   m_lAcctBalanceID=-1;
	   m_lBalanceTypeID=-1;
	   memset(m_sEffDate,0,sizeof(m_sEffDate));
	   memset(m_sExDate,0,sizeof(m_sExDate));
	   m_lBalance=0;
	   m_lCycleUpper=-1;
	   m_lCycleLower=-1;
	   memset(m_sCycleUpperType,0,sizeof(m_sCycleUpperType));
	   memset(m_sCycleLowerType,0,sizeof(m_sCycleLowerType));
	   m_lAcctID=-1;
	   m_lServID=-1;
	   m_lItemGroupID=-1;
	   memset(m_sState,0,sizeof(m_sState));
	   memset(m_sStateDate,0,sizeof(m_sStateDate));
	   memset(m_sBankAcct,0,sizeof(m_sBankAcct));
	   m_lObjectTypeID=-1;
   };
}AcctBalanceInfo;

//֧���ӿ�
typedef struct _Payment_info_All_CCR
{
    	char  m_sOperatype[UTF8_MAX_32];
	char  m_sOperateSeq[UTF8_MAX_32];//��ˮ��
	ui32  m_iStaffID;//����ԱID
	char  m_sDestinationAccount[UTF8_MAX_32];//�û���ʶ����
    	char  m_sDestinationtype[UTF8_MAX_32];//��ʶ����,1��ͨ��֤�ʺ�,2��ҵ��ƽ̨
    	//short m_iDestinationAttr;//�û�����(���û�����Ϊ�û�ҵ�����ʱ��д)0-�̻�; 1-С��ͨ; 2-�ƶ�,3-ADSL,4-���ܹ���,5-�����ǿ� 99-δ֪
    	int m_iDestinationAttr;//�û�����(���û�����Ϊ�û�ҵ�����ʱ��д)0-�̻�; 1-С��ͨ; 2-�ƶ�,3-ADSL,4-���ܹ���,5-�����ǿ� 99-δ֪
	 long  m_lPayAmount;//֧�����     
	 long  m_lCharge_type;//�Ƿ�һ�ο���
	 ui32  m_iBalance_unit;//��λ
	 char  m_sDeductDate[UTF8_MAX_32];//�۷�ʱ��
	 char  m_sAreaCode[UTF8_MAX_32];//����
	 char  m_sAcctItemTypeID[UTF8_MAX_32];//��Ŀ����
	 char  m_sSpID[UTF8_MAX_32];//�̼�ID
	 char  m_sSpName[UTF8_MAX_32];//�̼�����
	 char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
	 char  m_sMicropayType[UTF8_MAX_32];//֧������	0�������ʻ���1��ǿ���2���ÿ�
	 char  m_sOrderID[UTF8_MAX_32];//����ID
	 char  m_sDescription[UTF8_MAX_32];//������Ҫ����
	 char  m_sCapabilityCode[UTF8_MAX_32];//��������
	 int   m_iDestinationIDtype;//�û���������ID
	 long  m_lServID;//�û���ʶ
	 char  m_sOrderState[UTF8_MAX_32];//����״̬
	 long  m_lPaymentID;//opt_id
	 char  m_sRecType[UTF8_MAX_32];
	 char  m_sOptDate[UTF8_MAX_32];
	 char  m_sPayType[UTF8_MAX_32];
	
}PaymentInfoALLCCR;
typedef struct _Payment_info_All_CCA
{
	long m_lDeductAmount;// �ʱ��������	
	long m_lAcctBalanceAmount;// �ʱ�ʣ����	
	ui64 m_lAcctBalanceId;// �۷��˱���ʶ	
	long m_lDeductAmountAll;// �ɹ��������	
	long m_lAcctBalance;// �ʻ�ʣ����	
	ui32 m_iSvcResultCode;// ҵ�񼶵Ľ������	
	long m_lAcctId;	// �۷������ʶ��
	long m_lPaymentID;//�۷���ˮ
	
}PaymentInfoALLCCA;
//�����Դ
typedef  struct _Balance_Source_Info
{
  long  m_lOperIncomeID;
  long m_lAcctBalanceId ;
  char m_sOperType[UTF8_MAX_32]; 
  long m_iStaffID ;
  char m_sOperDate[UTF8_MAX_32];
  long m_lAmount;
  char m_sSourceType[UTF8_MAX_32];
  char m_sSourceDesc[UTF8_MAX_32];
  char m_sState[UTF8_MAX_32];
  char m_sStateDate[UTF8_MAX_32];
  long m_lAcctBalanceObjID;
  char m_sInvOffer[UTF8_MAX_32];
  char m_sAllowDraw[UTF8_MAX_32];
  char m_sCurrState[UTF8_MAX_32];
  char m_sCurrDate[UTF8_MAX_32];
  long m_lPaymentID;
  long m_lBalance;
  long m_lEffDateOffset;
  long m_lExpDateOffset;
  long m_lRemainAmount;
}BalanceSourceInfo;

// ���֧����¼�ṹ����
typedef struct _Balance_Payout_Info{	
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
}BalancePayoutInfo;


typedef struct _User_Self_Service
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[32];		// ����ʱ��
	char m_servNbr[UTF8_MAX];	// �û�����
	int m_nbrType;		// ��������
	int m_servAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
	
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; //AVP ERROR CODE 
	char m_responseTime[32];	// ��Ӧʱ��
	char m_payStatus[UTF8_MAX];//֧������״̬
	
	int m_dateTransFreq; //ÿ�컮��Ƶ�ȣ���λ�Σ�
	int m_transLimit;    //ÿ�ν���޶��λ�֣�
	int m_monthTransLimit; //ÿ�½���޶��λ�֣�
	int  m_autotransflag;  //�Զ�������־��0-���Զ�������1-�Զ���
	int m_autobalancelimit; //�Զ�������ֵ����λ�֣�
	int m_autoAmountlimit;  //�Զ���������λ�֣�
	
	char m_oldPassword[32];//��ǰ������
	char m_newPassword[32]; //���ú������
	char m_actionId[32];//01-�޸�����,02-�½�����
	char m_userInfoId[32];		// �û���ʶ
	
	char m_payPassword[32];//֧������
	
	char m_randomAccNbr[32]; //�󶨺���
	
	int m_datePayFreq;//ÿ��֧��Ƶ�ȣ���λ�Σ�
	int m_payLimit;//ÿ��֧���޶��λ�֣�
	int m_monthPayLimit;//ÿ��֧���޶��λ�֣�
	int m_microPayLimit;//С��֧����ֵ����λ�֣�
	
	int m_acctId;
	
	long m_lPaymentSeq;//ҵ����ˮ
}UserSelfService;

/*
//֧��CCR
typedef struct _Abm_Pay_Cond
{
    char  m_sOperatype[UTF8_MAX_32];//ҵ���������
    ui32  m_iOperateAction;//����������,8�C�۷�
    char  m_sOperateSeq[UTF8_MAX_32];//֧��������ˮ��
    char  m_sSpID[UTF8_MAX_32];//�̼�ID
    char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
    ui32  m_iStaffID;//����ԱID
    char  m_sMicropayType[UTF8_MAX_32];//֧������	0�������ʻ���1��ǿ���2���ÿ�
    char  m_sDestinationAccount[UTF8_MAX_32];//�û���ʶ
    char  m_sDestinationtype[UTF8_MAX_32];//��ʶ����,1��ͨ��֤�ʺ�,2��ҵ��ƽ̨
    //short m_iDestinationAttr;//�û�����(���û�����Ϊ�û�ҵ�����ʱ��д)0-�̻�; 1-С��ͨ; 2-�ƶ�,3-ADSL,4-���ܹ���,5-�����ǿ� 99-δ֪
    int m_iDestinationAttr;//�û�����(���û�����Ϊ�û�ҵ�����ʱ��д)0-�̻�; 1-С��ͨ; 2-�ƶ�,3-ADSL,4-���ܹ���,5-�����ǿ� 99-δ֪
    char  m_sPayPassword[UTF8_MAX_32];//֧������
    char  m_sRandomPassword[UTF8_MAX_32];//������֤�������
    char  m_sPayDate[UTF8_MAX_32];//����ʱ��
    unsigned int  m_iPayDate;//����ʱ��
    char  m_sOrderID[UTF8_MAX_32];//����ID
    char  m_sDescription[UTF8_MAX_32];//������Ҫ����
    long  m_lPayAmount;//֧�����     
}AbmPayCond;

//֧��CCA
typedef struct _ResponsPayOne
{
    int m_iRltCode;//ҵ�񼶵Ľ������
    int m_iRltCodeE;//HP�ڴ��ַ�������⣬��Ҫ����8���ֽ�
}ResponsPayOne;
//֧��CCA
typedef struct _ResponsPayTwo
{
    char m_sOperateSeq[UTF8_MAX_32];//������ˮ
    char m_sReqSerial[UTF8_MAX_32];//�Ʒ�ϵͳ�Ŀ۷���ˮ��
}ResponsPayTwo;

class AbmPaymentCCR : public ABMTpl<AbmPayCond, __DFT_T2_, __DFT_T3_, ABMPAYCMD_INFOR>
{
public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addPayInfo(AbmPayCond &data) {
        return __add1(data);
    }
    
    int getPayInfo(vector<AbmPayCond *> &vec) {
        return __getValue1(vec);    
    }
};
 


class AbmPaymentCCA : public ABMTpl<__DFT_T1_, ResponsPayOne, ResponsPayTwo, ABMPAYCMD_INFOR>
{
public:
    void clear() {
        return ABMObject::clear();    
    } 
    
    bool addRespons(ResponsPayOne &data) {
        return __add2(data);    
    }   
    
    bool addPayData(ResponsPayTwo &data) {
        return __add3(data);    
    }
    
    int getRespons(vector<ResponsPayOne*> &vec) {
        return __getValue2(vec);     
    }
    
    int getPayData(ResponsPayOne *pPtr, vector<ResponsPayTwo*> &vec) {
        return __getValue3(pPtr, vec);    
    }
};   
*/
//������������
typedef struct _Serv_Acct_Info_Seq
{
	long m_lServID;
	long m_lAcctID;
	long m_lServAcctID;
	long m_lAcctBalanceID;
	long m_lBalanceSourceID;
	long m_lPaymentID;
	long m_lCustID;
}ServAcctInfoSeq;
/*
//������ֵCCR
typedef struct _Deposit_Serv_Cond_One
{
    char  m_sReqSerial[UTF8_MAX_32];//��ֵ������ˮ��
    //char  m_sBalanceType[UTF8_MAX_32];//�������
    ui32  m_iBalanceType;//�������
    ui32  m_iBalanceTypE;//����
    ui32  m_iDestinationBalanceUnit;//��λ��0���֣�1����
    ui32  m_iVoucherValue;//��ֵ�����
    ui32  m_iProlongDays;//��ֵ���ӳ���Ч��(����)
    char  m_sDestinationType[UTF8_MAX_32];//�������ͣ�5BA���˻���5BB���ͻ���5BC���û���
    char  m_sDestinationAccount[UTF8_MAX_32];//	����ֵ�û��ı�ʶ
    int   m_iDestinationAttr;//����ֵ�û�������
    ui32  m_iBonusStartTime;//�Ż���Ч����ʼʱ�䣬yyyymmdd
    ui32  m_iBonusExpirationTime;//�Ż���Ч���ս�ʱ�䣬yyyymmdd
    long  m_lDestinationAttrDetail;//����ֵ�û���������ϸ
    char  m_sAreaCode[UTF8_MAX_32];//����


    char m_sSourceType[UTF8_MAX_32];//�����Դ����
    char m_sSourceDesc[UTF8_MAX_32];//�����Դ����
    char m_sAllowDraw[UTF8_MAX_32];//�Ƿ������ת
}DepositServCondOne;
//������ֵCCR
typedef struct _Deposit_Serv_Cond_Two
{
    ui32  m_iBonusUnit;//�Żݶλ��0���֣�1����
    ui32  m_iBonusAmount;//������Żݽ��
    //char  m_sBalanceType[UTF8_MAX_32];//�������
    ui32  m_iBalanceType;//�������
    ui32  m_iBalanceTypeE;//����
    ui32  m_iBonusStartTime;//�Ż���Ч����ʼʱ�䣬yyyymmdd
    ui32  m_iBonusExpirationTime;//�Ż���Ч���ս�ʱ�䣬yyyymmdd

}DepositServCondTwo;


//������ֵCCA
typedef struct _ResponsDepositOne
{
    int  m_iRltCode;//	��ֵ���0���ɹ����ǣ���ʧ�ܴ������
    int  m_iRltCodeE;//	8λ����
    char  m_sReqSerial[UTF8_MAX_32];//��ֵ������ˮ��
}ResponsDepositOne;
//������ֵCCA
typedef struct _ResponsDepositTwo
{
    int  m_iDestinationBalance;//�˱������
    char  m_sBalanceType[UTF8_MAX_32];//�������
    ui32  m_iDestinationBalanceUnit;//��λ��0���֣�1����
    ui32  m_iDestinationEffectiveTime;//�����Ч����(YYYYMMDD)
    ui32  m_iDestinationExpirationTime;//�˻�����Ч��yyyymmdd
}ResponsDepositTwo;

class DepositInfoCCR : public ABMTpl<__DFT_T1_, DepositServCondOne, DepositServCondTwo, ABMDEPOSITCMD_INFOR>
{
public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addDepositOneCCR(DepositServCondOne &data) {
        return __add2(data);
    }

    bool addDepositTwoCCR(DepositServCondTwo  &data) {
        return __add3(data);
    }
    
    int getDepositOneCCR(vector<DepositServCondOne *> &vec) {
        return __getValue2(vec);    
    }
    int getDepositTwoCCR(DepositServCondOne *pDep, vector<DepositServCondTwo *> &vec) {
        return __getValue3(pDep,vec);    
    }
};
 


class DepositInfoCCA : public ABMTpl<__DFT_T1_, ResponsDepositOne, ResponsDepositTwo, ABMDEPOSITCMD_INFOR>
{
public:
    void clear() {
        return ABMObject::clear();    
    } 
    
    bool addRespDepositOneCCA(ResponsDepositOne &data) {
        return __add2(data);    
    }   
    
    bool addRespDepositTwoCCA(ResponsDepositTwo &data) {
        return __add3(data);    
    }
    
    int getRespDepositOneCCA(vector<ResponsDepositOne*> &vec) {
        return __getValue2(vec);     
    }
    
    int getRespDepositTwoCCA(ResponsDepositOne *pPtr, vector<ResponsDepositTwo*> &vec) {
        return __getValue3(pPtr, vec);    
    }
};  

*/
/*
//��ֵ����CCR
typedef struct _Reverse_Deposit_Info
{
    char  m_sReqSerial[UTF8_MAX_32];//����������ˮ��
    char  m_sPaymentID[UTF8_MAX_32];//ԭ��ֵ������ˮ��
    char  m_sDestinationType[UTF8_MAX_32];//�������ͣ�5BA���˻���5BB���ͻ���5BC���û���
    char  m_sDestinationAccount[UTF8_MAX_32];//	����ֵ�û��ı�ʶ
    int   m_iDestinationAttr;//����ֵ�û�������
    int   m_iDestinationAttrE;//����8λ

}ReverseDeposit;
//��ֵ����CCA
typedef struct _Respons_Reverse_Deposit
{
    char m_sReqSerial[UTF8_MAX_32];//����������ˮ��
    int  m_iRltCode;//���0���ɹ����ǣ���ʧ�ܴ������
    int  m_iRltCodeE;//����8λ	
}ResponsReverseDeposit;


class ReverseDepositInfoCCR : public ABMTpl<ReverseDeposit,  __DFT_T2_, __DFT_T3_, ABM_REVERSEEPOSIT_CMD>
{
public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addReverseDepositInfo(ReverseDeposit &data) {
        return __add1(data);
    }
    
    int getReverseDepositInfo(vector<ReverseDeposit *> &vec) {
        return __getValue1(vec);    
    }
};
 


class ReverseDepositInfoCCA : public ABMTpl<__DFT_T1_, ResponsReverseDeposit, __DFT_T3_, ABM_REVERSEEPOSIT_CMD>
{
public:
    void clear() {
        return ABMObject::clear();    
    } 
    
    bool addRespReverseDepositCCA(ResponsReverseDeposit &data) {
        return __add2(data);    
    }   
    
    
    int getRespReverseDepositCCA(vector<ResponsReverseDeposit*> &vec) {
        return __getValue2(vec);     
    }
    
}; 
*/
#endif/*__ABMDEPOSITCMD_H_INCLUDED_*/


