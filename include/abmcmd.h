#ifndef __ABMCMD_H_INCLUDED_
#define __ABMCMD_H_INCLUDED_

#include "abmobject.h"

#define UTF8_MAX  64
#define UTF8_MAX_32  32
#define TEXT_MAX  256
typedef unsigned int ui32;
typedef unsigned long ui64;
typedef unsigned long time_s;

#define ABMCMD_PAY_ABILITY_QRY  31          //3.4.6.9 ֧������״̬��ѯ
#define ABMCMD_BAL_TRANS_QRY  32            //3.4.6.6 ���������ѯ
#define ABMCMD_TRANS_RULES_RESET  33        //3.4.6.7 ������������
#define ABMCMD_PASSWORD_RESET  34           //3.4.6.8 ֧�������޸�
#define ABMCMD_PAYSTATUS_RESET  35          //3.4.6.10 ֧��״̬���
#define ABMCMD_PAYRULE_QRY  36              //3.4.6.11 ֧�������ѯ
#define ABMCMD_PAYRULE_RESET 37 			//3.4.6.12 ֧��������
#define ABMCMD_BINDINGACCNBR_QRY 38			//3.4.6.3 ֧��������


#define ABMCMD_QRY_SRV_INFOR  1               //3.4.1.1 ��ѯ���ϲ�����
#define ABMCMD_QRY_SRV_PSWD   8               //3.4.2.5 ������֤�����·�
#define ABMPAYCMD_INFOR       9               //3.4.2.1 ֧������
#define ABMCMD_QRY_SRV_SYN  18   //����ͬ��
#define ABMCMD_LOGIN_PASSWD 20	//��½�����Ȩ
#define ABMCMD_PASSWD_IDENTIFY 21 //HSS�������Ȩ
#define ABMCMD_PASSWD_CHANGE_CCR 22 //��½�����޸�������CCR
#define ABMCMD_PASSWD_CHANGE_CCA 23 //��½�����޸Ľ���CCA
#define ABMCMD_ALLOCATE_BALANCE  6684801	//3.4.5.1 ��������
#define ABMCMD_REVERSE_PAY  6684802		//3.4.2.4 ֧������
#define ABMCMD_ACTIVE_ALLOCATE_BALANCE 6684803	//3.4.5.2 ��������
#define ABMCMD_REVERSE_DEDUCT_BALANCE 6684804	//3.4.5.5 ��������
#define ABMCMD_PLATFORM_DEDUCT_BALANCE 6684805	//3.4.6.5 �Է���ƽ̨��������
#define ABMCMD_PLATFORM_DEDUCT_BALANCE_BIZ 6684806 // 
#define ABMCMD_PLATFORM_QUEUE 66848999
#define ABMDEPOSITCMD_INFOR  10    //deposit
#define ABM_REVERSEEPOSIT_CMD  11   //reverse deposit
const int  ABMCMD_QRY_BAL_INFOR = 13;  //����ѯ
const int  ABMCMD_QRY_REC_INFOR = 14;  //���˼�¼��ѯ
const int  ABMCMD_QRY_PAY_INFOR = 15;  //���׼�¼��ѯ
const int  ABMCMD_QRY_BIL_INFOR = 16;  //�����˵���ѯ
const int  ABMCMD_QRY_MICROPAY_INFO = 4;//������ѯ
const int  ABMCMD_QRY_MICROPAYCCA_INFO = 44; //����������ѯʡABM��������Ϣ

/*********************************************************************
 *
 *               3.4.1.1 ���ϲ�ѯ������
 *
 *********************************************************************/
//*3.4.1.1 ���ϲ�ѯ������
struct ResponsStruct
{
    char m_sResID[UTF8_MAX];
    int m_iRltCode;
    int m_iAlign;    
};
struct QryServInforCond
{
    char  m_sReqID[UTF8_MAX];
    char  m_sServPlatID[UTF8_MAX];
    char  m_sDsnNbr[UTF8_MAX];
    short m_hDsnType;
    short m_hDsnAttr;
    ui32  m_uiQryType;    
};

class QryServInforCCR : public ABMTpl<QryServInforCond, __DFT_T2_, __DFT_T3_, ABMCMD_QRY_SRV_INFOR>
{
public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addServInfor(QryServInforCond &data) {
        return __add1(data);
    }
    
    int getServInfor(vector<QryServInforCond *> &vec) {
        return __getValue1(vec);    
    }
};
 
struct QryServInforData
{
    long  m_lServID;
    long  m_lLocServID;
    long  m_lAcctID;
    ui32  m_uiProdID;
    ui32  m_uiAreaCode;
    char  m_sAccNbr[32];
    char  m_sStateDate[32];
    short m_hServState;
    short m_hPayFlag;        
};

class QryServInforCCA : public ABMTpl<__DFT_T1_, ResponsStruct, QryServInforData, ABMCMD_QRY_SRV_INFOR>
{
public:
    void clear() {
        return ABMObject::clear();    
    } 
    
    bool addRespons(ResponsStruct &data) {
        return __add2(data);    
    }   
    
    bool addServData(QryServInforData &data) {
        return __add3(data);    
    }
    
    int getRespons(vector<ResponsStruct*> &vec) {
        return __getValue2(vec);     
    }
    
    int getServData(ResponsStruct *pPtr, vector<QryServInforData*> &vec) {
        return __getValue3(pPtr, vec);    
    }
};  



/*********************************************************************
 *
 *                3.4.6.13 ����ѯ
 *
 *********************************************************************/
//3.4.6.13 ����ѯ
struct QueryBalance
{
	char m_sReqID[UTF8_MAX];
	char m_sDestID[UTF8_MAX];
	int  m_iDestIDType;
	int  m_iDestAttr;
	char m_sAreaCode[UTF8_MAX];
	int  m_iQueryFlg;
	int  m_iQueryItemTyp;
};

class HssQueryBalanceCCR : public ABMTpl<QueryBalance, __DFT_T2_, __DFT_T3_, ABMCMD_QRY_BAL_INFOR>
{
public:
	void clear() {
        return ABMObject::clear();    
    }

	bool addReqBal(QueryBalance &data)
	{
		return __add1(data);
	}

	int getReqBal(vector<QueryBalance *> &data)
	{
		return __getValue1(data);
	}
	
};

//����ѯ���ؽ��
struct ResBalanceCode
{
	char m_sResID[UTF8_MAX];
	ui32 m_uiRltCode;
	long m_lTotalBalAvail;
        char m_sParaFieldResult[UTF8_MAX];
};
struct ResQueryBalance
{
	char m_sBalItmDetail[52];
	int  m_iUnitTypID;
	long m_lBalAmount;
	long m_lBalAvailAble;
	//long m_lBalanceOweUsed;
	long m_lBalUsed;
	long m_lBalReserved;
	ui32 m_uiEffDate;
	ui32 m_uiExpDate;
};

class HssQueryBalanceCCA : public ABMTpl<__DFT_T1_, ResBalanceCode, ResQueryBalance, ABMCMD_QRY_BAL_INFOR>
{
public:
	void clear() {
        return ABMObject::clear();    
    }
	
	bool addRespons(ResBalanceCode &data)
	{
        return __add2(data);    
    }
	
	bool addResBal(ResQueryBalance &data)
	{
		return __add3(data);
	}

	int getResPons(vector<ResBalanceCode *> &data)
	{
		return __getValue2(data);
	}

	long getResBal(ResBalanceCode * addr, vector<ResQueryBalance *> &data)
	{
		return __getValue3(addr, data);
	}
};


/*********************************************************************
 *
 *                 3.4.6.14 ���˼�¼��ѯ
 *
 *********************************************************************/
//3.4.6.14 ���˼�¼��ѯ
struct RechargeRecQuery
{
	char m_sOperSeq[UTF8_MAX];
	char m_sDestAcct[UTF8_MAX];
	int  m_iDestType;
	int  m_iDestAttr;
	char m_sSrvPlatID[UTF8_MAX];
	ui32 m_uiStartTime;
	ui32 m_uiExpTime;
};

class HssRechargeRecQueryCCR : public ABMTpl<RechargeRecQuery, __DFT_T2_, __DFT_T3_, ABMCMD_QRY_REC_INFOR>
{
public:
	void clear() {
        return ABMObject::clear();    
    }

	bool addRec(RechargeRecQuery &data)
	{
		return __add1(data);
	}

	int getRec(vector <RechargeRecQuery *> &data)
	{
		return __getValue1(data);
	}
};

//���˼�¼��ѯ���ؽ��
struct ResRecCode
{
	char m_sSeq[UTF8_MAX];
	ui32 m_uiRltCode;
        int  m_iAlign;
        char m_sParaFieldResult[UTF8_MAX];
};
struct ResRechargeRecQuery
{
	ui32 m_uiResult;
	ui32 m_uiOperAct;
	char m_sRechargeTime[UTF8_MAX];
	char m_sDestAcct[UTF8_MAX];
	int  m_iDestAttr;
        int  m_iAlign;
	char m_sSrvPlatID[UTF8_MAX];
	ui32 m_uiChargeType;
	ui32 m_uiRechargeAmount;
};

class HssRechargeRecQueryCCA : public ABMTpl<__DFT_T1_, ResRecCode, ResRechargeRecQuery, ABMCMD_QRY_REC_INFOR>
{
public:
	void clear() {
        return ABMObject::clear();    
    }

	bool addRespons(ResRecCode &data)
	{
        return __add2(data);
    }

	bool addRec(ResRechargeRecQuery &data)
	{
		return __add3(data);
	}

	int getRespons(vector<ResRecCode *> &data)
	{
		return __getValue2(data);
	}

	long getRec(ResRecCode * addr, vector<ResRechargeRecQuery *> &data)
	{
		return __getValue3(addr, data);
	}
};


/*********************************************************************
 *
 *                 3.4.6.15 ���׼�¼��ѯ
 *
 *********************************************************************/
//3.4.6.15 ���׼�¼��ѯ
struct QueryPayLogByAccout
{
	ui32 m_uiOperAct;
	char m_sOperSeq[UTF8_MAX];
	char m_sSPID[UTF8_MAX];
	char m_sSPName[UTF8_MAX];
	char m_sSrvPlatID[UTF8_MAX];
	char m_sDestAcct[UTF8_MAX];
	int  m_iDestType;
	int  m_iDestAttr;
	ui32 m_uiStartTime;
	ui32 m_uiExpTime;
};

class HssQueryPayLogByAccoutCCR : public ABMTpl<QueryPayLogByAccout, __DFT_T2_, __DFT_T3_, ABMCMD_QRY_PAY_INFOR>
{
public:
	void clear() {
        return ABMObject::clear();    
    }

	bool addPay(QueryPayLogByAccout &data)
	{
		return __add1(data);
	}

	int getPay(vector<QueryPayLogByAccout *> &data)
	{
		return __getValue1(data);
	}
};

//���׼�¼��ѯ���ؽ��
struct ResPayCode
{
	char m_sSeq[UTF8_MAX];
        char m_sParaFieldResult[UTF8_MAX];
	ui32 m_uiRltCode;
        int  m_iAlign;
};
struct ResQueryPayLogByAccout
{
	char m_sOderID[UTF8_MAX];
	char m_sDesc[52];
	ui32 m_uiPayDate;
	char m_sSPID[UTF8_MAX];
	char m_sSrvPlatID[UTF8_MAX];
	char m_sStatus[UTF8_MAX];
	char m_sMicropayType[UTF8_MAX];
	long m_lPayAmount;
};

class HssQueryPayLogByAccoutCCA : public ABMTpl<__DFT_T1_, ResPayCode, ResQueryPayLogByAccout, ABMCMD_QRY_PAY_INFOR>
{
public:
	void clear() {
        return ABMObject::clear();    
    }

	bool addRespons(ResPayCode &data)
	{
		return __add2(data);    
	}

	bool addPay(ResQueryPayLogByAccout &data)
	{
		return __add3(data);
	}

	int getRespons(vector<ResPayCode *> &data)
	{
		return __getValue2(data);
	}

	long getPay(ResPayCode * addr, vector<ResQueryPayLogByAccout *> &data)
	{
		return __getValue3(addr, data);
	}
};


/*********************************************************************
 *
 *                 3.4.6.16 �����˵���ѯ
 *
 *********************************************************************/
//3.4.6.16 �����˵���ѯ
struct RechargeBillQuery
{
    char m_sOperSeq[UTF8_MAX];
    char m_sDestAcct[UTF8_MAX];
    int  m_iDestType;
    int  m_iDestAttr;
    ui32 m_uiBilCycID;
};

class HssRechargeBillQueryCCR : public ABMTpl<RechargeBillQuery, __DFT_T2_, __DFT_T3_, ABMCMD_QRY_BIL_INFOR>
{
public:
    void clear() {
        return ABMObject::clear();    
    }

    bool addBil(RechargeBillQuery &data)
    {
        return __add1(data);
    }

    int getBill(vector<RechargeBillQuery *> &data)
    {
        return __getValue1(data);
    }
};

//�����˵���ѯ���ؽ��
struct ResBilCode
{
    char m_sSeq[UTF8_MAX];
    ui32 m_uiRltCode;
    int  m_iAlign;
    char m_sParaFieldResult[UTF8_MAX];
};
struct ResRechargeBillQuery
{
    long m_lPayMount;
    int  m_iBalance;
    int  m_iLastBalance;
};

class HssRechargeBillQueryCCA : public ABMTpl<__DFT_T1_, ResBilCode, ResRechargeBillQuery, ABMCMD_QRY_BIL_INFOR>
{
public:
    void clear() {
        return ABMObject::clear();    
    }
	
    bool addRespons(ResBilCode &data)
    {
        return __add2(data);    
    }

    bool addBil(ResRechargeBillQuery &data)
    {
        return __add3(data);
    }

    int getRespons(vector<ResBilCode *> &data)
    {
        return __getValue2(data);
    }

    long getBil(ResBilCode* addr, vector<ResRechargeBillQuery *> &data)
    {
        return __getValue3(addr, data);
    }
};

//3.4.6.4������ѯ
struct QueryAllPay
{
public:
    char m_sReqID[UTF8_MAX];
    ui32 m_uiReqTime;
    ui32 m_uialign; //�ֽڶ���
    char m_sDestAcct[UTF8_MAX];
    int  m_iDestType;
    int  m_iDestAttr;
    char m_sServPlatID[UTF8_MAX];
    int  m_iQueryFlg;
    int  m_iQueryItemType;
};
class HssPaymentQueryCCR : public ABMTpl<QueryAllPay, __DFT_T2_, __DFT_T3_, ABMCMD_QRY_MICROPAY_INFO>
{
public:
    void clear() {
        return ABMObject::clear();    
    }
	
    bool addReq(QueryAllPay &data)
    {
        return __add1(data);    
    }

    int  getReq(vector <QueryAllPay *> &data)
    {
        return __getValue1(data);
    }
};

//����������Ϣ
struct MicroPayRlt
{
    ui32 m_uiRltCode;
    ui32 m_uiResTime;
    char m_sResID[UTF8_MAX];
    char m_sParaFieldResult[UTF8_MAX];
};

struct MicroPaySrv
{
    int m_iBalSrc;
    int m_iUnitTypID;
    long m_lBalAmount;
    char m_sRequestId[UTF8_MAX];
};

class HssResMicroPayCCA : public ABMTpl<__DFT_T1_,MicroPayRlt,MicroPaySrv,ABMCMD_QRY_MICROPAY_INFO>	
{
public:
    void clear() {
        return ABMObject::clear();    
    }

    bool addRlt(MicroPayRlt &data)
    {
        return __add2(data);
    }
	
    bool addSrv(MicroPaySrv &data)
    {
        return __add3(data);
    }

    int getRlt(vector<MicroPayRlt *> &data)
    {
        return __getValue2(data);
    }
    int getSrv(MicroPayRlt *addr, vector<MicroPaySrv *> &data)
    {
        return __getValue3(addr,data);
    }
};



/*********************************************************************
 *
 *                 3.4.2.1֧����صĽṹ����
 *
 *********************************************************************/
//֧��CCR
typedef struct _Abm_Pay_Cond
{
    char  m_sOperatype[UTF8_MAX_32];//ҵ���������
    ui32  m_iOperateAction;//����������,8�C�۷�
    char  m_sOperateSeq[UTF8_MAX_32];//֧��������ˮ��
    char  m_sSpID[UTF8_MAX_32];//�̼�ID
    char  m_sSpName[UTF8_MAX_32];//�̼�ID
    char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
    ui32  m_iStaffID;//����ԱID
    char  m_sMicropayType[UTF8_MAX_32];//֧������	0�������ʻ���1��ǿ���2���ÿ�
    char  m_sDestinationAccount[UTF8_MAX_32];//�û���ʶ
    char  m_sDestinationtype[UTF8_MAX_32];//��ʶ����,1��ͨ��֤�ʺ�,2��ҵ��ƽ̨
    int m_iDestinationAttr;//�û�����(���û�����Ϊ�û�ҵ�����ʱ��д)0-�̻�; 1-С��ͨ; 2-�ƶ�,3-ADSL,4-���ܹ���,5-�����ǿ� 99-δ֪
    char  m_sPayPassword[UTF8_MAX_32];//֧������
    char  m_sRandomPassword[UTF8_MAX_32];//������֤�������
    char  m_sPayDate[UTF8_MAX_32];//����ʱ��
    unsigned int   m_iPayDate;//����ʱ��
    char  m_sOrderID[UTF8_MAX_32];//����ID
    char  m_sDescription[UTF8_MAX_32];//������Ҫ����
    long  m_lPayAmount;//֧�����    
    int   m_itest; 
}AbmPayCond;

//֧��CCA
typedef struct _ResponsPayOne
{
    int m_iRltCode;//ҵ�񼶵Ľ������
    int m_iRltCodeE;
    char m_sParaFieldResult[UTF8_MAX_32];//avp code
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
 
/*******************************************************************
 *
 *             3.4.2.5 ������֤�����·�
 *
 *******************************************************************/
struct PasswordResponsStruct
{
    char m_ReqID[UTF8_MAX_32];
    ui32 m_ReqTime;
};

//*3.4.2.5 ������֤�����·�
struct QryPasswordCond
{
    char m_sDestAct[UTF8_MAX_32];
    int m_DestType;
    int m_DestAttr;
    char m_SevPlatID[UTF8_MAX_32];
    char m_RanAccNbr[UTF8_MAX_32];
    char m_OrdID[UTF8_MAX_32];
};

struct QryPasswordData
{
        ui32 m_SevResCode;
        char ResID[UTF8_MAX_32];
        ui32 ResTime;
};

class QryPasswordCCR : public ABMTpl<__DFT_T1_, PasswordResponsStruct, QryPasswordCond, ABMCMD_QRY_SRV_PSWD>
{
public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addpasswdRespons(PasswordResponsStruct &data) {
    return __add2(data);
        }
        
    bool addServPassWd(QryPasswordCond &data) {
      return __add3(data);    
   }
  
   int getpasswdRespons(vector<PasswordResponsStruct*> &vec) {
      return __getValue2(vec);     
   }

   int getServPassWd(PasswordResponsStruct *pPtr, vector<QryPasswordCond*> &vec) {
      return __getValue3(pPtr, vec);    
   }
};

class QryPasswordCCA :public ABMTpl<QryPasswordData, __DFT_T2_, __DFT_T3_, ABMCMD_QRY_SRV_PSWD>
{
public:
    void clear() {
        return ABMObject::clear();    
    } 
    
    bool addServPassWdData(QryPasswordData &data) {
        return __add1(data);    
    }
    
    int getServPassWdData(vector<QryPasswordData *> &vec) {
        return __getValue1(vec);    
    }
}; 

/*******************************************************************
 *
 *             3.4.1.2 ����ͬ��
 *
 *******************************************************************/
//����ͬ��������CCR
struct QryServSynCond
{
	char m_sReqID[UTF8_MAX];			//������ˮ
	int m_hDsnType;							//�˺�ֵ����
	char m_sDsnNbr[UTF8_MAX];			//�˺�ֵ
	int m_hDsnAttr;								//�û�����
	char m_sServPlatID[UTF8_MAX];	//ҵ��ƽ̨��ʶ
	long m_lServID;								//ȫ�������û���ʶ
	long m_lAcctID;								//ȫ�������˻���ʶ
	char m_sActType[UTF8_MAX];		//������嶯��
	char m_sAccPin[UTF8_MAX];						//�û���¼����
};

class QryServSynCCR: public ABMTpl<QryServSynCond, __DFT_T2_, __DFT_T3_, ABMCMD_QRY_SRV_SYN>
{
	public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addServSyn(QryServSynCond &data) {
        return __add1(data);
    }
    
    int getServSyn(vector<QryServSynCond *> &vec) {
        return __getValue1(vec);    
    }
};

//����ͬ��������CCA
struct QryServSynData
{
	char m_sResID[UTF8_MAX];			//��Ӧ��ˮ
	int  m_iRltCode;									//ҵ�񼶽������
	ui64 m_lServId;					// ʡ��ServId,ʡHSS�ظ�ʱ��д
	char m_sParaRes[UTF8_MAX];			//������Ϣ
	int  m_iTest;//����
};

class QryServSynCCA:public ABMTpl<QryServSynData, __DFT_T2_, __DFT_T3_, ABMCMD_QRY_SRV_SYN>
{
	public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addServSyn(QryServSynData &data) {
        return __add1(data);
    }
    
    int getServSyn(vector<QryServSynData *> &vec) {
        return __getValue1(vec);    
    }
};


//������ֵCCR
typedef struct _Deposit_Serv_Cond_One
{
    char  m_sReqSerial[UTF8_MAX_32];//��ֵ������ˮ��
    //char  m_sBalanceType[UTF8_MAX_32];//�������
    ui32  m_iBalanceType;//�������
    ui32  m_iBalanceTypeE;//����
    ui32  m_iDestinationBalanceUnit;//��λ��0���֣�1����
    ui32  m_iVoucherValue;//��ֵ�����
    ui32  m_iProlongDays;//��ֵ���ӳ���Ч��(����)
    char  m_sDestinationType[UTF8_MAX_32];//�������ͣ�5BA���˻���5BB���ͻ���5BC���û���
    char  m_sDestinationAccount[UTF8_MAX_32];//	����ֵ�û��ı�ʶ
    int   m_iDestinationAttr;//����ֵ�û�������
    ui32  m_iBonusStartTime;//�Ż���Ч����ʼʱ�䣬yyyymmdd
    ui32  m_iBonusExpirationTime;//�Ż���Ч���ս�ʱ�䣬yyyymmdd
    long  m_lDestinationAttrDetail;
    char  m_sAreaCode[UTF8_MAX];


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
    int  m_iRltCodeE;//	��ֵ���0���ɹ����ǣ���ʧ�ܴ������
    char  m_sReqSerial[UTF8_MAX_32];//��ֵ������ˮ��
}ResponsDepositOne;
//������ֵCCA
typedef struct _ResponsDepositTwo
{
    int  m_iDestinationBalance;//�˱������
    //char  m_sBalanceType[UTF8_MAX_32];//�������
    ui32  m_iBalanceType;//�������
    ui32  m_iBalanceTypE;//�������
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

/*******************************************************************
 *
 *             3.4.6.1 ��½�����Ȩ
 *
 *******************************************************************/
 struct LoginPasswdStruct
 {
 	char m_sReqID[UTF8_MAX];			//������ˮ
    char m_requestTime[64];     // ����ʱ��
 	ui32 m_iReqTime;
	ui32 m_itest;
 };
 
 struct LoginPasswdCond
 {
 	char m_sDestAccount[UTF8_MAX];	//�û�����
 	int m_hDestType;	//��ʶ����
 	int m_hDestAttr;	//�û�����
 	char m_sServPlatID[UTF8_MAX];	//ҵ��ƽ̨��ʶ
 	char m_sLoginPasswd[UTF8_MAX];//��½����
 };
 
 struct LoginPasswdData
 {
 	char m_sResID[UTF8_MAX];	//��Ӧ��ˮ
 	int m_hRltCode;	//ҵ�񼶽������
 	char m_sParaRes[UTF8_MAX];			//������Ϣ
 	ui32 m_iResTime;	//Ӧ��ʱ��
 };
 
 class LoginPasswdCCR : public ABMTpl<__DFT_T1_, LoginPasswdStruct, LoginPasswdCond, ABMCMD_LOGIN_PASSWD>
{
public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addLoginPasswdRespons(LoginPasswdStruct &data) {
    return __add2(data);
        }
        
    bool addLoginPasswd(LoginPasswdCond &data) {
      return __add3(data);    
   }
  
   int getLoginPasswdRespons(vector<LoginPasswdStruct*> &vec) {
      return __getValue2(vec);     
   }

   int getLoginPasswd(LoginPasswdStruct *pPtr, vector<LoginPasswdCond*> &vec) {
      return __getValue3(pPtr, vec);    
   }
};

class LoginPasswdCCA :public ABMTpl<LoginPasswdData, __DFT_T2_, __DFT_T3_, ABMCMD_LOGIN_PASSWD>
{
public:
    void clear() {
        return ABMObject::clear();    
    } 
    
    bool addLoginPasswdData(LoginPasswdData &data) {
        return __add1(data);    
    }
    
    int getLoginPasswdData(vector<LoginPasswdData *> &vec) {
        return __getValue1(vec);    
    }
};

/*******************************************************************
 *
 *             3.4.6.1 ����HSS�˼�Ȩ
 *
 *******************************************************************/
 struct PasswdIdentifyCond
 {
 	char m_sReqID[UTF8_MAX];			//������ˮ
 	int m_hDsnType;	//�˺�ֵ����
 	char m_sDsnNbr[UTF8_MAX];			//��ѯֵ
 	int m_hDsnAttr;	//�û�����
 	char m_sServPlatID[UTF8_MAX];	//ҵ��ƽ̨��ʶ
 	char m_sLoginPasswd[UTF8_MAX];//��½����
 };
 
 struct PasswdIdentifyData
 {
 	char m_sResID[UTF8_MAX];			//��Ӧ��ˮ
 	int m_hRltCode;	//ҵ�񼶽������
 	char m_sParaRes[UTF8_MAX];			//������Ϣ
 };
 
 class PasswdIdentifyCCR: public ABMTpl<PasswdIdentifyCond, __DFT_T2_, __DFT_T3_, ABMCMD_PASSWD_IDENTIFY>
{
	public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addPasswdIdentify(PasswdIdentifyCond &data) {
        return __add1(data);
    }
    
    int getPasswdIdentify(vector<PasswdIdentifyCond *> &vec) {
        return __getValue1(vec);    
    }
};

class PasswdIdentifyCCA:public ABMTpl<PasswdIdentifyData, __DFT_T2_, __DFT_T3_, ABMCMD_PASSWD_IDENTIFY>
{
	public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addPasswdIdentify(PasswdIdentifyData &data) {
        return __add1(data);
    }
    
    int getPasswdIdentify(vector<PasswdIdentifyData *> &vec) {
        return __getValue1(vec);    
    }
};

/*******************************************************************
 *
 *             3.4.5.1 ��������
 *
 *******************************************************************/
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
	ui32 m_iTest;
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
	int m_iTest;
};

// ����CCA��Ϣ - ������Ϣ
struct A_AllocateBalanceSvc
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; //AVP ERROR CODE 
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
	ui32 m_test;
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
		/*
		bool addRespons(ResponsStruct &data)
		{
			return __add2(data);
		}
		*/
		bool addResBalanceSvcData(A_AllocateBalanceSvc &data)
		{
			return __add2(data);
		}
	
		bool addResDebitAcctData(A_ResponseDebitAcctItem &data)
		{
			return __add3(data);
		}
	
		// ���
		/*
		int getRespons(vector<ResponsStruct*> &vec)
		{
			return __getValue2(vec);
		}*/
		
		int getResBalanceSvcData(vector<A_AllocateBalanceSvc*> &vec)
		{
			return __getValue2(vec);
		}

		int getAcctBalanceData(A_AllocateBalanceSvc *pPtr, vector<A_ResponseDebitAcctItem *> &vec)
		{
			return __getValue3(pPtr, vec);
		}
};


/*******************************************************************
 *
 *             3.4.5.2 ȫ���������� ������������
 *
 *******************************************************************/
// CCR
struct ReqBalanceSvc
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_destinationId[UTF8_MAX];	// ����ֵ�û�����
	int m_destinationIdType;	// ��������
	int m_destinationAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char m_areaCode[UTF8_MAX];	// �绰����
	char m_requestTime[32];		// ����ʱ��
	unsigned int m_iRequestTime;		
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
	unsigned int m_iResponseTime;
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

/*******************************************************************
 *
 *                    3.4.5.5 ��������
 *
 *******************************************************************/
// CCR
struct ReqReverseDeductSvc
{
	char m_sRequestId[UTF8_MAX];		// ������ˮ
	char m_sRequestTime[32];		// ����ʱ��
        unsigned int m_iRequestTime;	        	// ����ʱ��
	char m_sOldRequestId[UTF8_MAX];		// �������Ļ�����ˮ��
	char m_sABMId[UTF8_MAX];		// �������ABM��ʶ
	int m_iActionType;			// ������������,0:�������,1:��������
//	int m_iTest;
};

// CCA
struct ResReverseDeductData
{
	char m_sResponseId[UTF8_MAX];		// ������ˮ
	ui32 m_iServiceRltCode;			// ҵ�񼶵Ľ������
        char sParaFieldResult[UTF8_MAX]; 
        ui32 m_iTest;
	char m_sResponseTime[32];		// ��Ӧʱ��
	unsigned int  m_iResponseTime;		// ��Ӧʱ��
	int m_iTest2;
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

/*******************************************************************
 *
 *                    3.4.6.5 ����(�Է���ƽ̨���𻮲�����)
 *
 *******************************************************************/
// CCR ������Ϣ
struct ReqPlatformSvc
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[64];		// ����ʱ��
	unsigned int m_iRequestTime;
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
	unsigned int m_iResponseTime;
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

// CCA������
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

/*******************************************************************
 *
 *                    3.4.2.4 ֧������
 *
 *******************************************************************/
struct ReversePaySvc
{
	long lOperationAction;			// ����������,9 - �۷�ȡ��
	char sOperationSeq[UTF8_MAX];		// ������ˮ
	char sReqSerial[UTF8_MAX];		// ��Ҫȡ���۷ѵ�ԭ֧��������ˮ��
	char sSpId[UTF8_MAX];			// �̼�ID
	char sSpName[UTF8_MAX];			// �̼�����
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

/*******************************************************************
 *
 *             3.4.1.4.2�����޸�
 *
 *******************************************************************/
 
struct PasswdChangeStruct
{
	char m_sReqID[UTF8_MAX];			//������ˮ
 	char m_requestTime[64];     //����ʱ��
 	ui32 m_iReqTime;
	ui32 test;
};	

struct PasswdChangeCond
{
	char m_sDestAccount[UTF8_MAX];	//�û�����
 	int m_hDestType;	//��ʶ����
 	int m_hDestAttr;	//�û�����
 	char m_sServPlatID[UTF8_MAX];	//ҵ��ƽ̨��ʶ
 	char m_sOldPasswd[UTF8_MAX];//��ǰ��������
 	char m_sNewPasswd[UTF8_MAX];	//���ú�����
};

struct PasswdChangeData
{
 	char m_sResID[UTF8_MAX];	//��Ӧ��ˮ
 	int m_hRltCode;	//ҵ�񼶽������
 	char m_sParaRes[UTF8_MAX];			//������Ϣ
 	//char m_responseTime[64];	//Ӧ��ʱ��
 	ui32 m_iResTime;	//Ӧ��ʱ��
};

class PasswdChangeCCR : public ABMTpl<__DFT_T1_, PasswdChangeStruct, PasswdChangeCond, ABMCMD_PASSWD_CHANGE_CCR>
{
public:
    void clear() {
        return ABMObject::clear();
    }
    
    bool addPasswdChangeRespons(PasswdChangeStruct &data) {
    return __add2(data);
        }
        
    bool addPasswdChange(PasswdChangeCond &data) {
      return __add3(data);    
   }
  
   int getPasswdChangeRespons(vector<PasswdChangeStruct*> &vec) {
      return __getValue2(vec);     
   }

   int getPasswdChange(PasswdChangeStruct *pPtr, vector<PasswdChangeCond*> &vec) {
      return __getValue3(pPtr, vec);    
   }
};

class PasswdChangeCCA :public ABMTpl<PasswdChangeData, __DFT_T2_, __DFT_T3_, ABMCMD_PASSWD_CHANGE_CCR>
{
public:
    void clear() {
        return ABMObject::clear();    
    } 
    
    bool addPasswdChangeData(PasswdChangeData &data) {
        return __add1(data);    
    }
    
    int getPasswdChangeData(vector<PasswdChangeData *> &vec) {
        return __getValue1(vec);    
    }
};


/*******************************************************************
 *
 *             3.4.6.9 ֧������״̬��ѯ
 *
 *******************************************************************/
// ֧������״̬��ѯCCR��Ϣ - ������Ϣ
struct R_PayAbilityQuerySvc
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[32];		// ����ʱ��
};

// ֧������״̬��ѯCCR��Ϣ - ֧������Ϣ��
struct R_MicropaymentInfo
{
	char m_servNbr[UTF8_MAX];	// �û�����
	int m_nbrType;		// ��������
	int m_servAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
};

// ֧������״̬��ѯCCA��Ϣ - ������Ϣ
struct A_PayAbilityQuerySvc
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; //AVP ERROR CODE 
	char m_responseTime[32];	// ��Ӧʱ��
	ui32 m_test;		// ����λ��

};

// ֧������״̬��ѯCCA��Ϣ -֧������Ϣ�� 
struct A_MicropaymentInfo
{
	char m_PayStatus[UTF8_MAX];//֧������״̬
};

class PayAbilityQueryCCR : public ABMTpl<__DFT_T1_,R_PayAbilityQuerySvc, R_MicropaymentInfo, ABMCMD_PAY_ABILITY_QRY>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		
		// ���
		bool addPayAbilityQuerySvc(R_PayAbilityQuerySvc &data)
		{
			return __add2(data);
		}
		
		bool addMicropaymentInfo(R_MicropaymentInfo &data)
		{
			return __add3(data);	
		}
	
		// ���
		// CCR����ȡ����������Ϣ
		int getR_PayAbilityQuerySvc(vector<R_PayAbilityQuerySvc *> &vec)
		{
			return __getValue2(vec);
		}
		// CCR����ȡ������Ŀ��ϸ��Ϣ
		int getR_MicropaymentInfo(R_PayAbilityQuerySvc *pSvc,vector<R_MicropaymentInfo *> &vec)
		{
			return __getValue3(pSvc,vec);	
		}
	
};

class PayAbilityQueryCCA : public ABMTpl<__DFT_T1_, A_PayAbilityQuerySvc, A_MicropaymentInfo, ABMCMD_PAY_ABILITY_QRY>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���
		/*
		bool addRespons(ResponsStruct &data)
		{
			return __add2(data);
		}
		*/
		bool addResPayAbilityQuerySvc(A_PayAbilityQuerySvc &data)
		{
			return __add2(data);
		}
	
		bool addResMicropaymentInfo(A_MicropaymentInfo &data)
		{
			return __add3(data);
		}
	
		// ���
		/*
		int getRespons(vector<ResponsStruct*> &vec)
		{
			return __getValue2(vec);
		}*/
		
		int getResPayAbilityQuerySvc(vector<A_PayAbilityQuerySvc*> &vec)
		{
			return __getValue2(vec);
		}

		int getResMicropaymentInfo(A_PayAbilityQuerySvc *pPtr, vector<A_MicropaymentInfo *> &vec)
		{
			return __getValue3(pPtr, vec);
		}
};

/*******************************************************************
 *
 *             3.4.6.6 ���������ѯ
 *
 *******************************************************************/
// ���������ѯCCR��Ϣ - ������Ϣ
struct R_BalanceTransSvc
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[32];		// ����ʱ��
};

// ���������ѯCCR��Ϣ - ֧������Ϣ��
struct R_BTMicropaymentInfo
{
	char m_servNbr[UTF8_MAX];	// �û�����
	int m_nbrType;		// ��������
	int m_servAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
};

// ���������ѯCCA��Ϣ - ������Ϣ
struct A_BalanceTransSvc
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; //AVP ERROR CODE 
	char m_responseTime[32];	// ��Ӧʱ��
	ui32 m_test;		// ����λ��

};

// ���������ѯCCA��Ϣ -֧������Ϣ�� 
struct A_BTMicropaymentInfo
{
	int m_dateTransFreq; //ÿ�컮��Ƶ�ȣ���λ�Σ�
	int m_transLimit;    //ÿ�ν���޶��λ�֣�
	int m_monthTransLimit; //ÿ�½���޶��λ�֣�
	int m_autotransflag;  //�Զ�������־��0-���Զ�������1-�Զ���
	int m_autobalancelimit; //�Զ�������ֵ����λ�֣�
	int m_autoAmountlimit;  //�Զ���������λ�֣�
};

class BalanceTransCCR : public ABMTpl<__DFT_T1_,R_BalanceTransSvc, R_BTMicropaymentInfo, ABMCMD_BAL_TRANS_QRY>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		
		// ���
		bool addBalanceTransSvc(R_BalanceTransSvc &data)
		{
			return __add2(data);
		}
		
		bool addBTMicropaymentInfo(R_BTMicropaymentInfo &data)
		{
			return __add3(data);	
		}
	
		// ���
		// CCR����ȡ����������Ϣ
		int getBalanceTransSvc(vector<R_BalanceTransSvc *> &vec)
		{
			return __getValue2(vec);
		}
		// CCR����ȡ������Ŀ��ϸ��Ϣ
		int getBTMicropaymentInfo(R_BalanceTransSvc *pSvc,vector<R_BTMicropaymentInfo *> &vec)
		{
			return __getValue3(pSvc,vec);	
		}
	
};

class BalanceTransCCA : public ABMTpl<__DFT_T1_, A_BalanceTransSvc, A_BTMicropaymentInfo, ABMCMD_BAL_TRANS_QRY>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���
		bool addResBalanceTransSvc(A_BalanceTransSvc &data)
		{
			return __add2(data);
		}
	
		bool addResBTMicropaymentInfo(A_BTMicropaymentInfo &data)
		{
			return __add3(data);
		}
	
		// ���
		
		int getResBalanceTransSvc(vector<A_BalanceTransSvc*> &vec)
		{
			return __getValue2(vec);
		}

		int getResBTMicropaymentInfo(A_BalanceTransSvc *pPtr, vector<A_BTMicropaymentInfo *> &vec)
		{
			return __getValue3(pPtr, vec);
		}
};


/*******************************************************************
 *
 *             3.4.6.7 ������������
 *
 *******************************************************************/
// ������������CCR��Ϣ - ������Ϣ
struct R_BalanceTransResetSvc
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[32];		// ����ʱ��
};

// ������������CCR��Ϣ - ֧������Ϣ��
struct R_TRMicropaymentInfo
{
	char m_servNbr[UTF8_MAX];	// �û�����
	int m_nbrType;		// ��������
	int m_servAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
	char m_payPassword[UTF8_MAX];//֧������
	
	int m_dateTransFreq; //ÿ�컮��Ƶ�ȣ���λ�Σ�
	int m_transLimit;    //ÿ�ν���޶��λ�֣�
	int m_monthTransLimit; //ÿ�½���޶��λ�֣�
	int m_autotransflag;  //�Զ�������־��0-���Զ�������1-�Զ���
	int m_autobalancelimit; //�Զ�������ֵ����λ�֣�
	int m_autoAmountlimit;  //�Զ���������λ�֣�
};

// ������������CCA��Ϣ - ������Ϣ
struct A_BalanceTransResetSvc
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; //AVP ERROR CODE 
	char m_responseTime[32];	// ��Ӧʱ��
	ui32 m_test;		// ����λ��

};

// ������������CCA��Ϣ -֧������Ϣ�� 
struct A_TRMicropaymentInfo
{
	int m_dateTransFreq; //ÿ�컮��Ƶ�ȣ���λ�Σ�
	int m_transLimit;    //ÿ�ν���޶��λ�֣�
	int m_monthTransLimit; //ÿ�½���޶��λ�֣�
	int m_autotransflag;  //�Զ�������־��0-���Զ�������1-�Զ���
	int m_autobalancelimit; //�Զ�������ֵ����λ�֣�
	int m_autoAmountlimit;  //�Զ���������λ�֣�
};

class BalanceTransResetCCR : public ABMTpl<__DFT_T1_,R_BalanceTransResetSvc, R_TRMicropaymentInfo, ABMCMD_TRANS_RULES_RESET>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		
		// ���
		bool addBalanceTransResetSvc(R_BalanceTransResetSvc &data)
		{
			return __add2(data);
		}
		
		bool addTRMicropaymentInfo(R_TRMicropaymentInfo &data)
		{
			return __add3(data);	
		}
	
		// ���
		// CCR����ȡ����������Ϣ
		int getBalanceTransResetSvc(vector<R_BalanceTransResetSvc *> &vec)
		{
			return __getValue2(vec);
		}
		// CCR����ȡ������Ŀ��ϸ��Ϣ
		int getTRMicropaymentInfo(R_BalanceTransResetSvc *pSvc,vector<R_TRMicropaymentInfo *> &vec)
		{
			return __getValue3(pSvc,vec);	
		}
	
};

class BalanceTransResetCCA : public ABMTpl<__DFT_T1_, A_BalanceTransResetSvc, A_TRMicropaymentInfo, ABMCMD_TRANS_RULES_RESET>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���
		bool addBalanceTransResetSvc(A_BalanceTransResetSvc &data)
		{
			return __add2(data);
		}
	
		bool addResTRMicropaymentInfo(A_TRMicropaymentInfo &data)
		{
			return __add3(data);
		}
	
		// ���
		
		int getResBalanceTransResetSvc(vector<A_BalanceTransResetSvc*> &vec)
		{
			return __getValue2(vec);
		}

		int getResRTMicropaymentInfo(A_BalanceTransResetSvc *pPtr, vector<A_TRMicropaymentInfo *> &vec)
		{
			return __getValue3(pPtr, vec);
		}
};




/*******************************************************************
 *
 *             3.4.6.8֧�������޸�
 *
 *******************************************************************/
 
struct R_PasswordReset
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[32];		// ����ʱ��
};	

struct R_PRMicropaymentInfo
{
	char m_servNbr[UTF8_MAX];	// �û�����
	int m_nbrType;		// ��������
	int m_servAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
 	char m_oldPassword[UTF8_MAX];//��ǰ��������
 	char m_newPassword[UTF8_MAX];	//���ú�����
 	char m_actionId[UTF8_MAX];	//���ú�����
};

struct A_PasswordReset
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; //AVP ERROR CODE 
	char m_responseTime[32];	// ��Ӧʱ��
	ui32 m_test;		// ����λ��
};

class PasswordResetCCR : public ABMTpl<__DFT_T1_, R_PasswordReset, R_PRMicropaymentInfo, ABMCMD_PASSWORD_RESET>
{
public:
    void clear() 
    {
        return ABMObject::clear();
    }
    
    bool addR_PasswordReset(R_PasswordReset &data) 
    {
    	return __add2(data);
    }
        
    bool addR_PRMicropaymentInfo(R_PRMicropaymentInfo &data) 
    {
    	return __add3(data);    
   	}
  
   int getR_PasswordReset(vector<R_PasswordReset*> &vec) 
   {
      	return __getValue2(vec);     
   }

   int getR_PRMicropaymentInfo(R_PasswordReset *pPtr, vector<R_PRMicropaymentInfo*> &vec) 
   {
      	return __getValue3(pPtr, vec);    
   }
};

class PasswordResetCCA :public ABMTpl<A_PasswordReset, __DFT_T2_, __DFT_T3_, ABMCMD_PASSWORD_RESET>
{
public:
    void clear() {
        return ABMObject::clear();    
    } 
    
    bool addA_PasswordReset(A_PasswordReset &data) {
        return __add1(data);    
    }
    
    int getA_PasswordReset(vector<A_PasswordReset *> &vec) {
        return __getValue1(vec);    
    }
};



/*******************************************************************
 *
 *             3.4.6.10֧��״̬���
 *
 *******************************************************************/
 
struct R_PayStatusReset
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[32];		// ����ʱ��
};	

struct R_PSRMicropaymentInfo
{
	char m_servNbr[UTF8_MAX];	// �û�����
	int m_nbrType;		// ��������
	int m_servAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
 	char m_payStatus[UTF8_MAX];//֧��״̬
 	char m_payPassword[UTF8_MAX];	//֧������

};

struct A_PayStatusReset
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; //AVP ERROR CODE 
	char m_responseTime[32];	// ��Ӧʱ��
	ui32 m_test;		// ����λ��
	char m_payStatus[UTF8_MAX];//֧��״̬
};

class PayStatusResetCCR : public ABMTpl<__DFT_T1_, R_PayStatusReset, R_PSRMicropaymentInfo, ABMCMD_PAYSTATUS_RESET>
{
public:
    void clear() 
    {
        return ABMObject::clear();
    }
    
    bool addR_PayStatusReset(R_PayStatusReset &data) 
    {
    	return __add2(data);
    }
        
    bool addR_PSRMicropaymentInfo(R_PSRMicropaymentInfo &data) 
    {
    	return __add3(data);    
   	}
  
   int getR_PayStatusReset(vector<R_PayStatusReset*> &vec) 
   {
      	return __getValue2(vec);     
   }

   int getR_PSRMicropaymentInfo(R_PayStatusReset *pPtr, vector<R_PSRMicropaymentInfo*> &vec) 
   {
      	return __getValue3(pPtr, vec);    
   }
};

class PayStatusResetCCA :public ABMTpl<A_PayStatusReset, __DFT_T2_, __DFT_T3_, ABMCMD_PAYSTATUS_RESET>
{
public:
    void clear() {
        return ABMObject::clear();    
    } 
    
    bool addA_PayStatusReset(A_PayStatusReset &data) {
        return __add1(data);    
    }
    
    int getA_PayStatusReset(vector<A_PayStatusReset *> &vec) {
        return __getValue1(vec);    
    }
};



/*******************************************************************
 *
 *             3.4.6.11 ֧�������ѯ
 *
 *******************************************************************/
// ֧�������ѯCCR��Ϣ - ������Ϣ
struct R_PayRulesQuerySvc
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[32];		// ����ʱ��
};

// ֧�������ѯCCR��Ϣ - ֧������Ϣ��
struct R_PRQMicropaymentInfo
{
	char m_servNbr[UTF8_MAX];	// �û�����
	int m_nbrType;		// ��������
	int m_servAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
};

// ֧�������ѯCCA��Ϣ - ������Ϣ
struct A_PayRulesQuerySvc
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; //AVP ERROR CODE 
	char m_responseTime[32];	// ��Ӧʱ��
	ui32 m_test;		// ����λ��

};

// ֧�������ѯCCA��Ϣ -֧������Ϣ�� 
struct A_PRQMicropaymentInfo
{
	int m_datePayFreq;//ÿ��֧��Ƶ�ȣ���λ�Σ�
	int m_payLimit;//ÿ��֧���޶��λ�֣�
	int m_monthPayLimit;//ÿ��֧���޶��λ�֣�
	int m_microPayLimit;//С��֧����ֵ����λ�֣�λ�֣�
};

class PayRulesQueryCCR : public ABMTpl<__DFT_T1_,R_PayRulesQuerySvc, R_PRQMicropaymentInfo, ABMCMD_PAYRULE_QRY>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		
		// ���
		bool addR_PayRulesQuerySvc(R_PayRulesQuerySvc &data)
		{
			return __add2(data);
		}
		
		bool addR_PRQMicropaymentInfo(R_PRQMicropaymentInfo &data)
		{
			return __add3(data);	
		}
	
		// ���
		// CCR����ȡ����������Ϣ
		int getR_PayRulesQuerySvc(vector<R_PayRulesQuerySvc *> &vec)
		{
			return __getValue2(vec);
		}
		// CCR����ȡ������Ŀ��ϸ��Ϣ
		int getR_PRQMicropaymentInfo(R_PayRulesQuerySvc *pSvc,vector<R_PRQMicropaymentInfo *> &vec)
		{
			return __getValue3(pSvc,vec);	
		}
	
};

class PayRulesQueryCCA : public ABMTpl<__DFT_T1_, A_PayRulesQuerySvc, A_PRQMicropaymentInfo, ABMCMD_PAYRULE_QRY>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���
		bool addA_PayRulesQuerySvc(A_PayRulesQuerySvc &data)
		{
			return __add2(data);
		}
	
		bool addA_PRQMicropaymentInfo(A_PRQMicropaymentInfo &data)
		{
			return __add3(data);
		}
	
		// ���
		
		int getA_PayRulesQuerySvc(vector<A_PayRulesQuerySvc*> &vec)
		{
			return __getValue2(vec);
		}

		int getA_PRQMicropaymentInfo(A_PayRulesQuerySvc *pPtr, vector<A_PRQMicropaymentInfo *> &vec)
		{
			return __getValue3(pPtr, vec);
		}
};



/*******************************************************************
 *
 *             3.4.6.12 ֧��������
 *
 *******************************************************************/
// ֧��������CCR��Ϣ - ������Ϣ
struct R_PayRulesResetSvc
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[32];		// ����ʱ��
};

// ֧��������CCR��Ϣ - ֧������Ϣ��
struct R_PRRMicropaymentInfo
{
	char m_servNbr[UTF8_MAX];	// �û�����
	int m_nbrType;		// ��������
	int m_servAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
	
	char m_payPassword[UTF8_MAX];//֧������
	
	int m_datePayFreq;//ÿ��֧��Ƶ�ȣ���λ�Σ�
	int m_payLimit;//ÿ��֧���޶��λ�֣�
	int m_monthPayLimit;//ÿ��֧���޶��λ�֣�
	int m_microPayLimit;//С��֧����ֵ����λ�֣�λ�֣�
};

// ֧��������CCA��Ϣ - ������Ϣ
struct A_PayRulesResetSvc
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; //AVP ERROR CODE 
	char m_responseTime[32];	// ��Ӧʱ��
	ui32 m_test;		// ����λ��

};

// ֧��������CCA��Ϣ -֧������Ϣ�� 
struct A_PRRMicropaymentInfo
{
	int m_datePayFreq;//ÿ��֧��Ƶ�ȣ���λ�Σ�
	int m_payLimit;//ÿ��֧���޶��λ�֣�
	int m_monthPayLimit;//ÿ��֧���޶��λ�֣�
	int m_microPayLimit;//С��֧����ֵ����λ�֣�λ�֣�
};

class PayRulesResetCCR : public ABMTpl<__DFT_T1_,R_PayRulesResetSvc, R_PRRMicropaymentInfo, ABMCMD_PAYRULE_RESET>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		
		// ���
		bool addR_PayRulesResetSvc(R_PayRulesResetSvc &data)
		{
			return __add2(data);
		}
		
		bool addR_PRRMicropaymentInfo(R_PRRMicropaymentInfo &data)
		{
			return __add3(data);	
		}
	
		// ���
		// CCR����ȡ����������Ϣ
		int getR_PayRulesResetSvc(vector<R_PayRulesResetSvc *> &vec)
		{
			return __getValue2(vec);
		}
		// CCR����ȡ������Ŀ��ϸ��Ϣ
		int getR_PRRMicropaymentInfo(R_PayRulesResetSvc *pSvc,vector<R_PRRMicropaymentInfo *> &vec)
		{
			return __getValue3(pSvc,vec);	
		}
	
};

class PayRulesResetCCA : public ABMTpl<__DFT_T1_, A_PayRulesResetSvc, A_PRRMicropaymentInfo, ABMCMD_PAYRULE_RESET>
{
	public:
		void clear() {
			return ABMObject::clear();
		}
	
		// ���
		bool addA_PayRulesResetSvc(A_PayRulesResetSvc &data)
		{
			return __add2(data);
		}
	
		bool addA_PRRMicropaymentInfo(A_PRRMicropaymentInfo &data)
		{
			return __add3(data);
		}
	
		// ���
		
		int getA_PayRulesResetSvc(vector<A_PayRulesResetSvc*> &vec)
		{
			return __getValue2(vec);
		}

		int getA_PRRMicropaymentInfo(A_PayRulesResetSvc *pPtr, vector<A_PRRMicropaymentInfo *> &vec)
		{
			return __getValue3(pPtr, vec);
		}
};


/*******************************************************************
 *
 *             3.4.6.3 �󶨺����ѯ
 *
 *******************************************************************/
// ֧�������ѯCCR��Ϣ - ������Ϣ
struct R_BindingAccNbrQuery
{
	char m_requestId[UTF8_MAX];	// ������ˮ
	char m_requestTime[32];		// ����ʱ��
};

// ֧�������ѯCCR��Ϣ - ֧������Ϣ��
struct R_BDAMicropaymentInfo
{
	char m_servNbr[UTF8_MAX];	// �û�����
	int m_nbrType;		// ��������
	int m_servAttr;		// �û����ԣ���������Ϊ�û���������ʱ������д
	char  m_sServicePlatformID[UTF8_MAX_32];//ҵ��ƽ̨ID
};

// ֧�������ѯCCA��Ϣ - ������Ϣ
struct A_BindingAccNbrQuery
{
	char m_responseId[UTF8_MAX];	// ��Ӧ��ˮ
	ui32 m_svcResultCode;		// ҵ�񼶵Ľ������
	char m_sParaFieldResult[UTF8_MAX]; //AVP ERROR CODE 
	char m_responseTime[32];	// ��Ӧʱ��
	ui32 m_test;		// ����λ��
	
	char m_randomAccNbr[32]; //�󶨺���
};

class BindingAccNbrQueryCCR : public ABMTpl<__DFT_T1_,R_BindingAccNbrQuery, R_BDAMicropaymentInfo, ABMCMD_BINDINGACCNBR_QRY>
{
	public:
		void clear()
		{
			return ABMObject::clear();
		}
		
		// ���
		bool addR_BindingAccNbrQuery(R_BindingAccNbrQuery &data)
		{
			return __add2(data);
		}
		
		bool addR_BDAMicropaymentInfo(R_BDAMicropaymentInfo &data)
		{
			return __add3(data);	
		}
	
		// ���
		// CCR����ȡ����������Ϣ
		int getR_BindingAccNbrQuery(vector<R_BindingAccNbrQuery *> &vec)
		{
			return __getValue2(vec);
		}
		// CCR����ȡ������Ŀ��ϸ��Ϣ
		int getR_BDAMicropaymentInfo(R_BindingAccNbrQuery *pSvc,vector<R_BDAMicropaymentInfo *> &vec)
		{
			return __getValue3(pSvc,vec);	
		}
	
};

class BindingAccNbrQueryCCA :public ABMTpl<A_BindingAccNbrQuery, __DFT_T2_, __DFT_T3_, ABMCMD_BINDINGACCNBR_QRY>
{
public:
    void clear() {
        return ABMObject::clear();    
    } 
    
    bool addA_BindingAccNbrQuery(A_BindingAccNbrQuery &data) {
        return __add1(data);    
    }
    
    int getA_BindingAccNbrQuery(vector<A_BindingAccNbrQuery *> &vec) {
        return __getValue1(vec);    
    }
};


#endif/*__ABMCMD_H_INCLUDED_*/
