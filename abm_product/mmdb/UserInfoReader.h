/*VER: 8*/ 
#ifndef USERINFOREADER_H_HEADER_INCLUDED_BDB745F9
#define USERINFOREADER_H_HEADER_INCLUDED_BDB745F9

#include "../../etc/LocalDefine.h"

#include "CommonMacro.h"
#include "UserInfo.h"
#include "StdEvent.h"
#include "BillingCycle.h"
#include "BillingCycleMgr.h"

#include <vector>
#include "AcctItemMgr.h"
#include "BillingCycleMgr.h"
#include "OrgMgr.h"
#include "StateConvertMgr.h"
#include "Environment.h"
#include "NetworkMgr.h"
#include "OfferMgr.h"
#include "SHMFileInfo.h"
class PricingCombine;
using namespace std;

class BillingCycleMgr;
class Serv;


//#define USERINFO_LIMIT "USERINFO_MEM_LIMIT"
#ifdef SHM_FILE_USERINFO
//##ModelId=424A47C303AB
class UserInfoReader : public SHMFileInfo
{
  public:
    //##ModelId=42525763028B
    UserInfoReader();
	UserInfoReader(int iMode);

  protected:
    //##ModelId=424A47DC01D0
    static BillingCycleMgr * m_poBillingCycleMgr;

    //##ModelId=424A47DC01EE
    static AcctItemMgr * m_poAcctItemMgr;

    //##ModelId=424A47DC0202
    static OrgMgr * m_poOrgMgr;

    static StateConvertMgr * m_poStateConvertMgr;
    
    static NetworkPriorityMgr * m_poNetworkMgr;
    
    static OfferMgr * m_poOfferMgr;
    
    #ifdef USERINFO_OFFSET_ALLOWED
    static int m_offsetSecs;

    static int m_beginOffsetSecs;

    void loadOffsetSecs();
    #endif

    long execOperation(int iOperation,StdEvent * m_poEvent,int iAttrID = 0);
};
#else
//##ModelId=424A47C303AB
class UserInfoReader : public UserInfoBase
{
  public:
    //##ModelId=42525763028B
    UserInfoReader();
	UserInfoReader(int iMode);

  protected:
    //##ModelId=424A47DC01D0
    static BillingCycleMgr * m_poBillingCycleMgr;

    //##ModelId=424A47DC01EE
    static AcctItemMgr * m_poAcctItemMgr;

    //##ModelId=424A47DC0202
    static OrgMgr * m_poOrgMgr;

    static StateConvertMgr * m_poStateConvertMgr;
    
    static NetworkPriorityMgr * m_poNetworkMgr;
    
    static OfferMgr * m_poOfferMgr;
    
    #ifdef USERINFO_OFFSET_ALLOWED
    static int m_offsetSecs;

    static int m_beginOffsetSecs;

    void loadOffsetSecs();
    #endif

    long execOperation(int iOperation,StdEvent * m_poEvent,int iAttrID = 0);
};
#endif
//##ModelId=4257416200BE
//##Documentation
//## 20050409���������࣬
//## ����serv��servproduct��cust�����Զ���
//## ����Ϊ�����Է�������ʱ��Ҫ��ȡĳ������Ʒʵ��
//## ���������ԣ�������Щ���Ե���Чʱ�䣬ʧЧʱ�䣬
//## ���������Ӵ��ࡣ
class UserInfoAttr
{
  public:
    //##ModelId=42578E4F0129
    int &getAttrID();

    //##ModelId=42578E570383
    char *getAttrValue();

    //##ModelId=42578E60015F
    char *getEffDate();

    //##ModelId=42578E6603B7
    char *getExpDate();

  private:
    //##ModelId=4257439E0369
    //##Documentation
    //## ���Ա�ʶ
    int m_iAttrID;

    //##ModelId=425743B102C6
    char m_sAttrValue[32];

    //##ModelId=425743ED02C2
    char m_sEffDate[16];

    //##ModelId=425743F503B4
    char m_sExpDate[16];

};

//##ModelId=42414E1D038F
class ServProduct : public UserInfoReader
{
	friend class ServProductIteration;
	friend class Serv;
	friend class UserInfoInterface;
  public:
    //##ModelId=42414E7A0176
    long getServProductID();

    //##ModelId=42414E81022B
    long getServID();

    //##ModelId=42414E86026E
    int getProductID();

    bool getEff(char *sDate);

    int getUsingDay(char *sBeginDate, char *sEndDate, Serv * pServ = 0);

    //##ModelId=42414E8C0226
    char *getEffDate();

    //##ModelId=42414E9202F7
    char *getExpDate();

    //##ModelId=42414E99010D
    char * getAttr(int attrid, char *sDate = 0);

    //##ModelId=42578D9C03AC
    int getAttrs(vector<UserInfoAttr> &ret);

    //##ModelId=4271EA030243
    //##Documentation
    //## ����ServProductInfo��ָ�룬
    //## ����ط�����������ȥ��
    //## ��ʱ�ȼ����������
    ServProductInfo *getServProductInfo();

  private:
    //##ModelId=4248A99203A3
    ServProductInfo *m_poServProductInfo;

    //##ModelId=4248A9AD006D
    ServInfo *m_poServInfo;

};

//##ModelId=42414EE6038E
class ServProductIteration : public UserInfoReader
{
	friend class Serv;
  public:
    //##ModelId=42414F1C02D8
    bool next(ServProduct &servProduct);
  private:
    //##ModelId=4248AEBB0190
    unsigned int m_iCurOffset;


    //##ModelId=4248AF2200F8
    bool m_bAll;
    //##ModelId=4248B13C0110
    char m_sTime[16];


};
//��Ʒʵ����ѯ����
class OfferInsQO : public UserInfoReader
{
public:
    //��Ʒʵ��
    ProdOfferIns* m_poOfferIns;
    //��Ʒʵ����ϸ
    OfferDetailIns* m_poDetailIns;
    //���ȼ���
    int m_iPriority;
    //��Ʒ���
    int m_iOfferType;
    //�Ƿ����ۻ���
    bool m_bAccued;
    
    char m_sTime[16];
    //�����Ʒʵ������������ֵ
    int getOfferInsAttr(ProdOfferInsAttr ret[MAX_OFFER_ATTRS], char * sDate = 0);
    //�����Ʒʵ������������ֵ������ʧЧʱ�䣬������չʾ�ã�
    int getOfferInsAttrAndDate(ProdOfferInsAttr ret[MAX_OFFER_ATTRS], char * sDate = 0);

    //�����Ʒʵ����ĳ������ֵ
    int getOfferInsAttrByAttrID(ProdOfferInsAttr ret[MAX_OFFER_ATTRS], int iAttrID, char * sDate = 0,StdEvent * poEvent =0);
	//ֻ��ȡ��Ʒʵ��ĳ������ֵ�ĸ���
    int getOfferInsAttrCntByAttrID(int iAttrID,char* sDate =0,StdEvent * poEvent =0);
    //�����Ʒʵ����ĳ������ֵ(@TODO�����������Ӧ��Ϊconst)
    char *getOfferAttr(int iAttrID, char *sDate = 0,StdEvent * poEvent =0);
    //��Ʒʵ����ĳ������ֵ�Ƿ�ΪiAttrVal
    bool isOfferAttrValue(int iAttrID, char* iAttrVal, char *sDate = 0,StdEvent * poEvent =0);
    //��Ʒʵ���Ƿ���ĳ����Ա
    bool isOfferMember(int iMemberID, char *sDate = 0);
    //�����Ʒʵ�������г�Ա
    int getOfferInsMember(OfferDetailIns ret[MAX_OFFER_MEMBERS], 
                                                            char *sDate = 0);
    OfferDetailIns * getOneMemberIns(int iMemberID, char *sDate = 0);
    int getOfferInsMember(OfferDetailIns * ret[MAX_OFFER_MEMBERS],
                            int iMemberID, char * sDate = 0);
    //�ж��û����˻��Ƿ����ڸ���Ʒ��iMemberID��Ա
    bool isOfferInsMember(long lServID, long lAcctID, int iMemberID, char *sDate = 0);
		//add by zhaoziwei
		//�����Ʒʵ����Э������Ϣ
		bool getOfferInsAgreement(ProdOfferInsAgreement &ret,char * sDate = 0);
		//end

private:
    bool __isOfferMember(long lProdOfferInsID, int iMemberID, char *sDate = 0);
    
    int __getOfferInsMember(OfferDetailIns ret[MAX_OFFER_MEMBERS], long lProdOfferInsID, char *sDate = 0);
    
    bool __isOfferInsMember(long lProdOfferInsID, long lServID, long lAcctID, int iMemberID, char *sDate);
};
//��Ʒʵ�����ʽӿ�
class OfferInsIteration : public UserInfoReader
{
    friend class UserInfoInterface;
public:
    bool next(ProdOfferIns &offerins);
private:
    unsigned int m_iCurOffset;
};
//##ModelId=4232A6400001
//##Documentation
//## ����Ʒʵ���ࡣ
//## �����Ӧһ������Ʒʵ��
class Serv : public UserInfoReader
{
    friend class ServIteration;
    friend class UserInfoInterface;

  public:

	//##ModelId=4232A90601B0
    //##Documentation
    //## ��ȡ����ƷID
    long getServID();

    //##ModelId=4232AE380131
    //##Documentation
    //## ������Ʒʵ���Ƿ������ĳ��Ʒʵ��
    bool belongOfferInstance(long instanceid, char *sdate = 0);
    
	char const * getServTypeEffDate(char *sDate = 0);

        //ĳ��ʱ�����Ƿ��Ǹò�Ʒ
    //pBeginDate < ExpDate && pEndDate > pEffDate
    bool hasProduct(int nProductID,char* pBeginDate,char* pEndDate=0);

    char const * getAccNBR(int iType, char *stime);

    //##ModelId=4232AE690131
    //##Documentation
    //## �Ƿ������ĳ��Ʒ
    bool belongOffer(int offerid, char *sdate = 0);

    //##ModelId=4232AEBF0370
    //##Documentation
    //## �Ƿ���ĳ������Ʒ
    //## ����еĻ������ص�sp��
    bool hasServProduct(int productid, ServProduct & sp, char *sdate) ;
    
    //ָ����ʱ���Ƿ��ڸø�����Ʒ����ʧЧʱ�䷶Χ��
    bool hasServProduct(int nServProductID,char* pBeginDate,char* pEndDate=0);

	//��ͨ������Ʒ��ʱ���Ƿ���ָ����ʱ�䷶Χ��
    bool hasServProductEx(int nServProductID,char* pBeginDate,char* pEndDate=0);

    //##ModelId=4232B01003D8
    //##Documentation
    //## ��ȡ������Ʒ��ͨʱ��
    char const *getProductEffDate(int productid, char *sdate = 0);

    //����serv_product_id��ȡ������Ʒ��ͨʱ��
    char const *getServProductEffDate(long iServProductID, char *sdate = 0);

    //##ModelId=4232B430006F
    //##Documentation
    //## ������ά������ʶ
    int getOrgID(
        //##Documentation
        //## ʱ�䣬���Ϊ�գ������ ��������Ʒʵ����ʱ������ ������
        char *sDate = 0);

    //##ModelId=4232B69901F3
    //##Documentation
    //## ��ȡĳ��Ŀ���͵ĸ����˺�
    long getAcctID(int acctitemtype);
    long getAcctID(int acctitemtype,char * sDate);
	long getAcctID(int acctitemtype,StdEvent * poEvent);    
    void getAcctID(vector<long> & vAcctID);
    void getAcctID(vector<long> & vAcctID,char * sDate);
	void getAcctID(vector<long> & vAcctID,StdEvent * poEvent);

    //##ModelId=4232B6FC02F0
    //##Documentation
    //## ��ȡ��ʵ����������������
    int getBillingCycleType();

    //##ModelId=4232B70B032E
    //##Documentation
    //## ����ĳʱ����������������
    BillingCycle const * getBillingCycle(char *time = 0, int iCycleType = -1);

    //##ModelId=423505310263
    //##Documentation
    //## ��ȡ��ʵ����ĳʱ��������õĶ��ۼƻ������ȼ��Ӹߵ�����
    int getPricingPlans(
        //##Documentation
        ExPricingPlan ret[MAX_PRICINGPLANS], char * sDate = 0);

    //##ModelId=423506410214
    //##Documentation
    //## ��ȡ�����Ķ��ۼƻ�����Ʒ�ģ���û�з���0
    int getBasePricingPlan(char *sDate = 0);

    //##ModelId=423506930302
    //##Documentation
    //## ����CUSTID
    long getCustID();

    //##ModelId=42355F1103A7
    //##Documentation
    //## ����cust����
    char * getCustType();

    //##ModelId=423677D80135
    //##Documentation
    //## ��ȡ������Ʒ�����ͻ�����������Ʒ��id
    int getCustServ(
        //##Documentation
        //## ��������vector
        vector<long> &ret);

    int getProductID(char *sDate = 0);


    //##ModelId=42414DD400F5
    int getOffers(int  offers[MAX_OFFERS], char *sdate = 0);

    //##ModelId=42414DF202E3
    int getOfferInstances(long offerinstances[MAX_OFFERINSTANCES], char *sdate = 0);

    //##ModelId=42414E330142
    bool getServProductIteration(ServProductIteration &servProductIteration, 
        //##Documentation
        //## ĳʱ�䣬
        //## �����ָ������Ϊ�ǵ�ǰserv��ʱ��
        //## ĳʱ����Ч�ĸ�����Ʒ
        char *sdate = 0,
        //##Documentation
        //## �Ƿ����е�servproduct��
        //## ���Ϊtrue����ȡ���serv�����е�servproduct�������Ƿ���Ч������ʱ�ڶ�����������
        bool isall = false);

    bool getServProduct(long lServProductID, ServProduct & servProduct);

    //##ModelId=4242BA380063
    bool belongOrg(int orgid);

    //##ModelId=42437460021F
    //##Documentation
    //## ��ȡĳʱ�䣬������Ʒʵ����״̬
    char *getState(char *sDate = 0);
    
    char *getPreState();

    //����û����µ�״̬
    char *getLatestState();

	//����û����µ�״̬����Чʱ���1��
    char *getLatestStateDate();

    //##ĳʱ�̵ļƷ�״̬
    char *getBillingState(char *sDate = 0);

    //##ĳһ���Ƿ�����
    bool getUsing(Date d);	

    //##��ȡ[sBeginDate, sEndDate]��ʹ������
    int  getUsingDay(char *sBeginDate, char *sEndDate);

    //##ModelId=4248BC6B013E
    bool belongServGroup(long groupid, char *sdate = 0);

    //##ModelId=4248BC7600EA
    bool belongCustGroup(long groupid);
    //##ModelId=425A2BD10371
    char *getRentDate();

    //##ModelId=425A2BD902E6
    //##ModelId=425A2BD902E6
    char *getCreateDate();

    //PricingCombine *getCombine(int iEventType, char *sDate = 0);
    //##ModelId=425A2BEE0264
    char *getNscDate();

    //##ModelId=425A2BFD0374
    char *getStateDate(char * sDate = 0);
    //##ModelId=426F002B014E
    //##Documentation
    //## ����ServInfo��ָ�룬
    //## ����ط�����������ȥ��
    //## ��ʱ�ȼ����������
    ServInfo *getServInfo();
    //##ModelId=4271EA3D0051
    //##Documentation
    //## ����CustInfo��ָ�룬
    //## ����ط�����������ȥ��
    //## ��ʱ�ȼ����������
    CustInfo *getCustInfo();

    char * getAttr(int attrid, char *sDate = 0);
	char * getAttrNew(int attrid, char * sDate, char * m_sValue, int iSize);
    int getAttrs(vector<UserInfoAttr> &ret);


	bool isAttrValue(int attrid, char *sValue, char *sDate=0);
	bool isAttrValueMax(int attrid, char *sValue, char *sDate=0);

    int getAttrNumber(int attrid, char *sDate=0);

    void setTime(char *sDate);
    
    int getFreeType();
    //ȡ�û����е���Ʒ,����������Ʒ
    int getOfferIns(OfferInsQO ret[MAX_OFFER_INSTANCE_NUMBER],char *sDate=0);
    //ȡ�û��Ļ�����Ʒ
    int getBaseOfferIns(OfferInsQO * ret, char *sDate = 0);
    //�ж��û��Ƿ���ĳ����Ʒ
    bool hasOffer (int iOfferID, char *sDate = 0);
    //���ػ�����Ʒ��ʶ,�Ҳ�������-1
    int getBaseOfferID(char *sDate = 0);
    int getOfferInsQO(OfferInsQO& ret,long lOfferInstID); 
    // ��ָ����ʱ�������ڣ�ȡ�û��ڱ�����������Ч����Ʒ������������Ʒ
    int getOfferInsBillingCycle(OfferInsQO ret[MAX_OFFER_INSTANCE_NUMBER], char *sDate);
    
		bool isServAttrChanged(const char *sBeginTime,const char *sEndTime);
		
		bool isServStateAttrChanged(const char *sBeginTime,const char *sEndTime);
		
		bool isServTypeChanged(const char *sBeginTime,const char *sEndTime);
		
		bool isServProductChanged(const char *sBeginTime,const char *sEndTime);
		
		bool isServProductAttrChanged(const char *sBeginTime,const char *sEndTime);
		
		bool isProdOffer_Ins_Attr_Detail_Changed(const char *sBeginTime,const char *sEndTime);
		
		bool isServChanged(const char *sBeginTime,const char *sEndTime);
    
  private:
    //##ModelId=4247F5370312
    char m_sTime[16];

    //##ModelId=425A2B880167
    ServInfo *m_poServInfo;

    //##ModelId=425A2B88016E
    CustInfo *m_poCustInfo;

    //##ModelId=425A2B880178
    ServProductInfo *m_poServProductInfo;

    //##ModelId=425A2B88018C
    ServAcctInfo *m_poServAcctInfo;

};

//##ModelId=42414EDA00FC
//##Documentation
//## Ŀǰ���ṩȫ�ֵı��У�Ĭ��get���ǵ�ǰʱ���serv
class ServIteration : public UserInfoReader
{
  friend class UserInfoInterface;

  public:
    //##ModelId=42414F0F010C
    bool next(Serv &serv);
    bool nextByOrg(Serv &serv);

    /*  ���� nextByOrg��ʱ�� ����ʹ�����½ӿڣ�   */
    bool belongCurServLocation(char *sTime);

  private:
    //##ModelId=424A00FD01A9
    unsigned int m_iCurOffset;

    unsigned int m_iLastOffset;


    //##ModelId=424A01060134
    char m_sTime[16];

};

class CustIteration : public UserInfoReader
{
  public:
	friend class UserInfoInterface;

  public:
	bool next(long & lCustID);
	
  private:
	unsigned int m_iCurOffset;

};

class ServGroupIteration : public UserInfoReader
{
  public:
	friend class UserInfoInterface;

  public:
	bool next(long & lGroupID);

  private:
	unsigned int m_iCurOffset;
};

//##ModelId=41D259580067
//##Documentation
//## �û����϶�ȡ�ӿ�
//## Ŀǰ���Ŀ鶫�������ĸ������ڴ�����д����������Կ����������ÿ�����
class UserInfoInterface : public UserInfoReader
{
  public:
    //##ModelId=42353FFC014D
    //##Documentation
    //## ��ȡ�Ƿ����
    operator bool();

    static bool getAcctBelongOrg(long lAcctID, int iOrgID);
    static bool getCustBelongOrg(long lCustID, int iOrgID);
    static bool getServGroupBelongOrg(int iServGroupID, int iOrgID);

    //##ModelId=4232A42F02B3
    //##Documentation
    //## ����ҵ����룬�ҳ�����Ʒʵ����ؿͻ����ϡ������ڲ�����ĳʱ�̣�һ��ҵ������Ӧ�������Ʒʵ�������
    static bool getServ(
        //##Documentation
        //## ���������Ķ���
        Serv &ret, 
        //##Documentation
        //## ҵ�����
        char *accnbr, 
        //##Documentation
        //## ʱ��
        //## yyyymmddhhmiss
        char stime[16], 
        int pNetworkID = 0,
        bool isPhy=false) ;

    ////��װ���ȰѺ��������ƴ�����ң��Ҳ����Ļ�ֱ��ʹ�ú�����
    static bool getServ_A(
        //## ���������Ķ���
        Serv &ret, 
        //ҵ����������
        char *area_code,        
        //## ҵ�����
        char *accnbr, 
        //## ʱ�� yyyymmddhhmiss
        char stime[16], 
        int pNetworkID = 0,
        bool isPhy=false) ;

	////��װ�����ڶԶ˺��벻ʹ�������ʶ���ȸ��ݺ������ң�Ȼ��Ѻ��������ƴ�����ң��Ժ�����жԶ˲���ȫ�����������
    static bool getServ_Z(
        //## ���������Ķ���
        Serv &ret,
        //ҵ����������
        char *area_code,       
        //## ҵ�����
        char *accnbr,
        //## ʱ�� yyyymmddhhmiss
        char stime[16],
        bool isPhy=false);

	//����ҵ������ҳ�һ��˫�ŵ���һ���û�
	static bool getServ(
        //���������Ķ���
        Serv &ret, 
        //ҵ�����
        char *accnbr, 
		//serv_Id
		long lServID,
        //ʱ�� yyyymmddhhmiss
        char stime[16],
        bool isPhy=false);

    static char const * convertAccNBR(char *accnbr, int iSrcType, int iDestType, char *stime);

    static bool getServProduct(long lServProductID, ServProduct & servProduct);

    //##ModelId=423677970042
    //##Documentation
    //## ����SERVID���ҳ�����Ʒʵ����ؿͻ����ϡ�
    static bool getServ(
        //##Documentation
        //## ���������Ķ���
        Serv &ret, 
        //##Documentation
        //## ҵ�����
        long servid, 
        //##Documentation
        //## ʱ��
        //## yyyymmddhhmiss
        char stime[16]) ;
    
    
    //## ���ݺ���/�������� �����û�����
    static bool getServ (
        //## ���������Ķ���
        Serv &ret,
        //## ����
        char *accnbr,
        //## ��������
        int  iType,
        //## ʱ��
        char *stime
    );

	//## ���ݺ���/�������͡������ʶ �����û�����
    static bool getServN (
        //## ���������Ķ���
        Serv &ret,
        //## ����
        char *accnbr,
        //## ��������
        int  iType,
        //## ʱ��
        char *stime,
		//�����ʶ
		int pNetworkID = 0
    );
    
    static int getCustServ(long lCustID, vector<long> &vServ);
    static int getGroupServ(long lGroupID, vector<long> &vServ, char *sDate);
    static int getCustAcct(long lCustID, vector<long> &vAcct);

    static int getAcctCreditAmount(long lAcctID);
    static int getAcctCreditGrade(long lAcctID);

    //##ModelId=4234E8030031
    //##Documentation
    //## �����Щ��̬������û�б���ʼ�������û�б���ʼ�������г�ʼ�������ӹ����ڴ棩��
    UserInfoInterface();
	UserInfoInterface(int iMode);

    //##ModelId=42414DB202A5
    static bool getServIteration(ServIteration &servIteration);

    static bool getServIterByOrg(ServIteration &servIter, int iOrgID);

    static bool getCustIteration(CustIteration &custIteration);

    static bool getServGroupIteration(ServGroupIteration &servGroupIteration);
    
    static int getServOrg(long lServID, char * sDate);
   // int getCustomMadeServ(vector<long> &ret, int iObjectID, unsigned int iCustPlanOffset, int iAcctItem=0); 

    long getCustomParamValue (int iParamID, unsigned int iCustPlanOffset);

    int getCustomPricingPlans(ExPricingPlan ret[MAX_PRICINGPLANS],int iObjectType,long lObjectID,char *sDate);

    long getCustomObjectID(int iPlanObjectID, unsigned int iCustPlanOffset);

	bool isBindServObject(long lServID, unsigned int iCustPlanOffset);

    char const *getCustomPlanEffDate(unsigned int iCustPlanOffset);

    CustPricingObjectInfo const * getCustomObject(int iPlanObjectID, unsigned int iCustPlanOffset);

    int getPayInfo(vector<ServAcctInfo> &ret, long lAcctID, char * sDate=0);
	int getPayInfo(vector<ServAcctInfo> &ret, long lAcctID, StdEvent * poEvent);

    CustPricingTariffInfo * getCustTariff(unsigned int iCustPlanOffset, char *sCalledNBR);
    
    //cdk������Ʒʵ���Ĳ�ѯ
    //����ʻ�����Ʒʵ��
    int getAcctOfferIns(OfferInsQO ret[MAX_OFFER_INSTANCE_NUMBER], long lAcctID, char *sDate);
    //��ÿͻ�����Ʒʵ��
    int getCustOfferIns(OfferInsQO ret[MAX_OFFER_INSTANCE_NUMBER], long lCustID, char *sDate);
    //������Ʒʵ����ʶ���������������������Ʒʵ��
    int getAllOfferInsByID(OfferInsQO ret[MAX_OFFER_INSTANCE_NUMBER], long lProdOfferInsID, char *sDate);
    int getOfferInsByID(OfferInsQO& ret,long lProdOfferInstID,char* sDate); 
	//xueqt ������Ʒʵ����ʶ��������Ʒʵ��, 
    ProdOfferIns * getOfferIns(long lOfferInstID);

    //������Ʒʵ���α�
    bool getOfferInsIteration(OfferInsIteration &offerinsiter);
    //�ж���Ʒʵ��������Ϣ
    bool isOfferInsBelongOrg(long lOfferInstID, int iOrgID, char * sDate);
    //##ModelId=4232B69901F3
    //##Documentation
    //## ��ȡĳ��Ŀ���͵ĸ����˺�
    long getAcctID(long lServID, int acctitemtype);
    long getAcctID(long lServID, int acctitemtype, char * sDate);
	long getAcctID(long lServID, int acctitemtype, StdEvent * poEvent);
		//����NP������Ϣ
	//add by jx 2009-10-26
	static bool getNpInfo(NpInfo &oNpInfo,char *sAccNbr,char *sDate = 0);
	//���������ڴ��е�serv��Ϣ
	static bool clearServ(Serv &cur);



#ifdef GROUP_CONTROL
	bool checkAccNbrOfferType(char * pAccNbr,char * pStartDate,int iOfferType);
	bool getAccNbrOfferType(char* pAccNbr,char* pStartDate,AccNbrOfferType& oAccNbrOfferType);
#endif

#ifdef GROUP_CONTROL
	bool getGroupInfoByID(GroupInfo &groupInfo,int iGroupID,char *sDate=0);
	int getGroupMemberByAccNbr(GroupMember ret[GROUP_NUM],char * pAccNbr,char *sDate=0);
	int getNumberGroupByAccNbr(NumberGroup ret[GROUP_NUM],char * pAccNbr,char *sDate=0);
	int getGroupResult(GroupInfoResult ret[GROUP_NUM],char * pCAccNbr,char * pGAccNbr,char * sDate=0);
#endif

#ifdef	IMSI_MDN_RELATION
	bool getImsiMdnRelationByImsi(ImsiMdnRelation &oRelation, char *sImsi,long iType, const char *sDate=0);
	void showImsiMdnRelationByImsi(char *sImsi, const char *sDate=0);
	void showAllImsiMdnRelation();
#endif
#ifdef	IMMEDIATE_VALID
	int getRollbackInstByServ(long lServID,OfferInsQO * pOfferBuf, int iOfferNum,
		long ret[MAX_OFFER_INSTANCE_NUMBER],const char *sDate=0);
	bool updateRollbackInstByServ(long lServID,long lInstID);
#endif	
};


#endif /* USERINFOREADER_H_HEADER_INCLUDED_BDB745F9 */
