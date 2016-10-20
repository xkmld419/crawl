/*VER: 9*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef USERINFO_H_HEADER_INCLUDED_BDBC7D5F
#define USERINFO_H_HEADER_INCLUDED_BDBC7D5F


/*	�Ƿ��ϸ��պ������Чʱ���ʧЧʱ������serv_id	*/
#define STRICT_SERV_IDENTIFICATION
//��������Ȳ��ϸ�Ҫ��
/*	�����Ҫ���ϸ���ң���ȡ������꣬���±������	*/

#include "SHMData.h"
#include "SHMData_A.h"
#include "SHMData_B.h"
#include "SHMStringTreeIndex.h"
#include "SHMStringTreeIndex_A.h"
#include "SHMIntHashIndex.h"
#include "SHMIntHashIndex_A.h"
#include "IpcKey.h"
#include "BillingCycleMgr.h"
#include "AcctItemMgr.h"
#include "OrgMgr.h"
#include "IpcKey.h"
//#include "SegmentMgr.h"


#ifdef  USERINFO_REUSE_MODE
///////�� USERINFO_REUSE_MODE
    #define USERINFO_SHM_DATA_TYPE  SHMData_A
    #define USERINFO_SHM_INT_INDEX_TYPE  SHMIntHashIndex_A
    #define USERINFO_SHM_STR_INDEX_TYPE  SHMStringTreeIndex_A
	#define USERINFO_ATTACH_DATA(X, Y, Z) \
		X = new SHMData_A<Y> (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}

    #define USERINFO_ATTACH_FILEDATA(X, Y, Z) \
		X = new SHMData_A<Y> (Z, false); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_FILEDATA failed!\n",__FILE__, __LINE__); \
		}

	#define USERINFO_ATTACH_INT_INDEX(X, Z) \
		X = new SHMIntHashIndex_A (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}

	#define USERINFO_ATTACH_STRING_INDEX(X, Z) \
		X = new SHMStringTreeIndex_A (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}


#else
///////�ر� USERINFO_REUSE_MODE 
    #define USERINFO_SHM_DATA_TYPE  SHMData
    #define USERINFO_SHM_INT_INDEX_TYPE  SHMIntHashIndex
    #define USERINFO_SHM_STR_INDEX_TYPE  SHMStringTreeIndex
	#define USERINFO_ATTACH_DATA(X, Y, Z) \
		X = new SHMData<Y> (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}

    #define USERINFO_ATTACH_FILEDATA(X, Y, Z) \
		X = new SHMData<Y> (Z, false); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_FILEDATA failed!\n",__FILE__, __LINE__); \
		}

	#define USERINFO_ATTACH_INT_INDEX(X, Z) \
		X = new SHMIntHashIndex (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}

	#define USERINFO_ATTACH_STRING_INDEX(X, Z) \
		X = new SHMStringTreeIndex (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}
#endif

struct pathFullName
{
        char sFilePath[128];

        char sCustName[128];  // CUST???t��??��??
        char sProductName[128];  // PRODUCT???t��??��??
        char sServName[128];  // SERV???t��??��??
        char sServTypeName[128];  // SERVTYPE???t��??��??
        char sServLocName[128];  // SERVLOCATION???t��??��??
        char sServStateName[128];  // SERVSTATEATTR???t��??��??
        char sServAttrName[128];  // SERVATTR???t��??��??
    #ifdef IMSI_MDN_RELATION
        char sImsiMdnRelatName[128];  // IMSIMDNRELAT???t��??��??
    #endif
        char sServIdenName[128];  // SERVIDEN???t��??��??
        char sServProdName[128];  // SERVPROD???t��??��??
        char sServProdAttrName[128];  // SERVPRODATTR???t��??��??
        char sServAcctName[128];  // SERVACCT???t��??��??
        char sAcctName[128];  // ACCT???t��??��??
        #ifdef GROUP_CONTROL
        char sGroupInfoName[128];  // GROUPINFO???t��??��??
        char sGroupMemberName[128];  // GROUPMEMBER???t��??��??
        char sNumberGroupName[128];  // NUMBERGROUP???t��??��??
        char sACCNbrOfferTypeName[128];  // ACCNBROFFERTYPE???t��??��??
        #endif
        char sOfferInsName[128];        // OFFERINS???t��??��??
        char sOfferInsAttrName[128];    // OFFERINSATTR???t��??��??
        char sOfferInsDetailName[128];  // OFFERINSDETAIL???t��??��??
	char sOfferInsAgrName[128];  // 
	char sNpName[128];
};

#define CUSTFILE_INFO_IDX           IpcKeyMgr::getIpcKey (-1,"SHM_CUSTFILE_INFO_IDX")
#define PRODFILE_INFO_IDX           IpcKeyMgr::getIpcKey (-1,"SHM_PRODFILE_INFO_IDX")
#define SERVFILE_INFO_IDX           IpcKeyMgr::getIpcKey (-1,"SHM_SERVFILE_INFO_IDX")
#define SERVLOCFILE_INFO_IDX        IpcKeyMgr::getIpcKey (-1,"SHM_SERVLOCFILE_INFO_IDX")
#ifdef	IMSI_MDN_RELATION
#define IMSIMDNRELATFILE_INFO_IDX   IpcKeyMgr::getIpcKey (-1,"SHM_IMSIMDNRELATFILE_INFO_IDX")
#endif
#define SERVIDENTFILE_INFO_IDX      IpcKeyMgr::getIpcKey (-1,"SHM_SERVIDENTFILE_INFO_IDX")
#define SERVPRODFILE_INFO_IDX       IpcKeyMgr::getIpcKey (-1,"SHM_SERVPRODFILE_INFO_IDX")
#define SERVACCTFILE_ACCT_IDX       IpcKeyMgr::getIpcKey (-1,"SHM_SERVACCTFILE_ACCT_IDX")
#define ACCTFILE_INFO_IDX           IpcKeyMgr::getIpcKey (-1,"SHM_ACCTFILE_INFO_IDX")
#ifdef GROUP_CONTROL
#define GROUPFILE_INFO_IDX          IpcKeyMgr::getIpcKey (-1,"SHM_GROUPFILE_INFO_IDX")
#define GROUPFILE_MEMBER_IDX        IpcKeyMgr::getIpcKey (-1,"SHM_GROUPFILE_MEMBER_IDX")
#define NUMBERFILE_GROUP_IDX        IpcKeyMgr::getIpcKey (-1,"SHM_NUMBERFILE_GROUP_IDX")
#define ACCNBROFFERTYPEFILE_IDX     IpcKeyMgr::getIpcKey (-1,"SHM_ACCNBROFFERTYPEFILE_IDX")
#define	ACCNBROFFERTYPEITEMFILE_IDX IpcKeyMgr::getIpcKey (-1,"SHM_ACCNBROFFERTYPEITEMFILE_IDX")
#endif
#define PRODOFFERINSFILE_IDX        IpcKeyMgr::getIpcKey (-1,"SHM_PRODOFFERINSFILE_IDX")
#define OFFERDETAILINS_IDX          IpcKeyMgr::getIpcKey (-1,"SHM_OFFERDETAILINS_IDX")
#define NPINFOFILE_IDX              IpcKeyMgr::getIpcKey (-1,"SHM_NPINFOFILE_IDX")

#define CUST_INFO_DATA                          IpcKeyMgr::getIpcKey (-1,"SHM_CUST_INFO_DATA")
#define CUST_INFO_INDEX                         IpcKeyMgr::getIpcKey (-1,"SHM_CUST_INFO_INDEX")
#define CUSTATTR_INFO_DATA                      IpcKeyMgr::getIpcKey (-1,"SHM_CUSTATTR_INFO_DATA")


//#define CUSTGROUP_INFO_DATA                   IpcKeyMgr::getIpcKey (-1,"SHM_CUSTGROUP_INFO_DATA")
//#define CUSTPRICINGPLAN_INFO_DATA             IpcKeyMgr::getIpcKey (-1,"SHM_CUSTPRICINGPLAN_INFO_DATA")

//#define CUSTPRICINGPARAM_INFO_DATA		    0x418
//#define CUSTPRICINGOBJECT_INFO_DATA		    0x419
//#define CUSTPRICINGTARIFF_INFO_DATA           0x421
//#define CUSTPRICINGTARIFF_INFO_INDEX          0x422
//#define CUSTPRICINGPLAN_INFO_INDEX		    0x41a

#define PRODUCT_INFO_DATA                       IpcKeyMgr::getIpcKey (-1,"SHM_PRODUCT_INFO_DATA")
#define PRODUCT_INFO_INDEX                      IpcKeyMgr::getIpcKey (-1,"SHM_PRODUCT_INFO_INDEX")
//#define OFFER_INFO_DATA                       0X407
//#define OFFER_INFO_INDEX                      0X408

#define SERV_INFO_DATA                          IpcKeyMgr::getIpcKey (-1,"SHM_SERV_INFO_DATA")
#define SERV_INFO_INDEX                         IpcKeyMgr::getIpcKey (-1,"SHM_SERV_INFO_INDEX")
#define SERVLOCATION_INFO_DATA                  IpcKeyMgr::getIpcKey (-1,"SHM_SERVLOCATION_INFO_DATA")
#define SERVLOCATION_INFO_INDEX                 IpcKeyMgr::getIpcKey (-1,"SHM_SERVLOCATION_INFO_INDEX")
#define SERVSTATE_INFO_DATA                     IpcKeyMgr::getIpcKey (-1,"SHM_SERVSTATE_INFO_DATA")
#define SERVATTR_INFO_DATA                      IpcKeyMgr::getIpcKey (-1,"SHM_SERVATTR_INFO_DATA")
#define SERVIDENT_INFO_DATA                     IpcKeyMgr::getIpcKey (-1,"SHM_SERVIDENT_INFO_DATA")
#define SERVIDENT_INFO_INDEX                    IpcKeyMgr::getIpcKey (-1,"SHM_SERVIDENT_INFO_INDEX")

//#define SERVGROUP_INFO_DATA                   0X410

#define SERVPRODUCT_INFO_DATA                   IpcKeyMgr::getIpcKey (-1,"SHM_SERVPRODUCT_INFO_DATA")
#define SERVPRODUCT_INFO_INDEX                  IpcKeyMgr::getIpcKey (-1,"SHM_SERVPRODUCT_INFO_INDEX")
#define SERVPRODUCTATTR_INFO_DATA               IpcKeyMgr::getIpcKey (-1,"SHM_SERVPRODUCTATTR_INFO_DATA")

#define SERVACCT_INFO_DATA                      IpcKeyMgr::getIpcKey (-1,"SHM_SERVACCT_INFO_DATA")
#define SERVACCT_ACCT_INDEX			            IpcKeyMgr::getIpcKey (-1,"SHM_SERVACCT_ACCT_INDEX")

//#define OFFERINSTANCE_INFO_DATA               0X415
//#define OFFERINSTANCE_INFO_INDEX              0X416
//#define SERVPRICINGPLAN_INFO_DATA             0X417

#define ACCT_INFO_DATA                          IpcKeyMgr::getIpcKey (-1,"SHM_ACCT_INFO_DATA")
#define ACCT_INFO_INDEX                         IpcKeyMgr::getIpcKey (-1,"SHM_ACCT_INFO_INDEX")

//#define SERVGROUPBASE_INFO_DATA			    0X41E
//#define SERVGROUPBASE_INFO_INDEX		        0X41F

#define SERVTYPE_INFO_DATA			            IpcKeyMgr::getIpcKey (-1,"SHM_SERVTYPE_INFO_DATA")
//��Ʒʵ��
#define PRODOFFERINS_DATA                       IpcKeyMgr::getIpcKey (-1,"SHM_PRODOFFERINS_DATA")
#define PRODOFFERINS_INDEX                      IpcKeyMgr::getIpcKey (-1,"SHM_PRODOFFERINS_INDEX")
#define OFFERDETAILINS_DATA                     IpcKeyMgr::getIpcKey (-1,"SHM_OFFERDETAILINS_DATA")
#define OFFERDETAILINS_INSIDOFFER_INDEX         IpcKeyMgr::getIpcKey (-1,"SHM_OFFERDETAILINS_INSIDOFFER_INDEX")
#define PRODOFFERINSATTR_DATA                   IpcKeyMgr::getIpcKey (-1,"SHM_PRODOFFERINSATTR_DATA")
//add by zhaoziwei
#define PRODOFFERINSAGREEMENT_DATA                   IpcKeyMgr::getIpcKey (-1,"SHM_PRODOFFERINSAGREEMENT_DATA")
//end

#ifdef GROUP_CONTROL
#define GROUP_INFO_DATA							IpcKeyMgr::getIpcKey (-1,"SHM_GROUP_INFO_DATA")
#define GROUP_INFO_INDEX						IpcKeyMgr::getIpcKey (-1,"SHM_GROUP_INFO_INDEX")
#define GROUP_MEMBER_DATA						IpcKeyMgr::getIpcKey (-1,"SHM_GROUP_MEMBER_DATA")
#define GROUP_MEMBER_INDEX						IpcKeyMgr::getIpcKey (-1,"SHM_GROUP_MEMBER_INDEX")
#define NUMBER_GROUP_DATA						IpcKeyMgr::getIpcKey (-1,"SHM_NUMBER_GROUP_DATA")
#define NUMBER_GROUP_INDEX						IpcKeyMgr::getIpcKey (-1,"SHM_NUMBER_GROUP_INDEX")
#endif
#ifdef GROUP_CONTROL
#define ACCNBROFFERTYPE_DATA					IpcKeyMgr::getIpcKey (-1,"SHM_ACCNBROFFERTYPE_DATA")
#define ACCNBROFFERTYPE_INDEX					IpcKeyMgr::getIpcKey (-1,"SHM_ACCNBROFFERTYPE_INDEX")
#define	SHM_ACCNBROFFERTYPE_ITEM_INDEX				IpcKeyMgr::getIpcKey (-1,"SHM_ACCNBROFFERTYPE_ITEM_INDEX")
#endif

#ifdef	IMSI_MDN_RELATION
#define IMSI_MDN_RELATION_INDEX					IpcKeyMgr::getIpcKey (-1,"SHM_IMSI_MDN_RELATION_INDEX")
#define IMSI_MDN_RELATION_DATA					IpcKeyMgr::getIpcKey (-1,"SHM_IMSI_MDN_RELATION_DATA")
#endif
//�������� �������� add by xn 2009.10.22
#define NP_INFO_DATA                          IpcKeyMgr::getIpcKey (-1,"SHM_NP_DATA")
#define NP_INFO_INDEX                         IpcKeyMgr::getIpcKey (-1,"SHM_NP_DATA_INDEX")

//##ModelId=4243D79B0218
//##Documentation
//## �ͻ�Ⱥ��Ϣ����cust֮�����
class CustGroupInfo
{
public:
    //##ModelId=4243D7BB03D6
    //##Documentation
    //## �ͻ�Ⱥ��ʶ
    long m_lGroupID;

    //##ModelId=4243D7C40158
    //##Documentation
    //## �ͻ���ʶ
    long m_lCustID;

    //##ModelId=4243D7CA026F
    //##Documentation
    //## ͬһ���ͻ�����һ��Ⱥ��Ϣ
    unsigned int m_iNextOffset;

};

//##ModelId=41CA41290076
//##Documentation
//## �ͻ���Ϣ��Ҫ��������custid����������serv֮ǰ����
class CustInfo
{
    public:
    
    //##ModelId=41D3C2980246
    //##Documentation
    //## �ͻ���ʶ
    long m_lCustID;

    //##ModelId=41D3C2BB00B6
    char m_sCustType[4];

    //##ModelId=41D3C2E700FF
    //int m_iCustLocation;

    //##ModelId=41D3C2ED031A
    ////char m_bIsVip; 

    //##ModelId=41D3C30C01AC
    //##Documentation
    //## �ϼ��ͻ���ʶ
    ////long m_lParentID;

    //##ModelId=41D3C31600C0
    //##Documentation
    //## �ͻ�ͳһ����
   // char m_sCustCode[32];

    //##ModelId=41D3C32D02A4
    char m_sState[4];

    //##ModelId=41D3C33100B5
    //char m_sEffDate[16];

    //##ModelId=41D3C3400175
    //char m_sExpDate[16];

    //##ModelId=4243A96903CB
    //##Documentation
    //## ָ��ÿͻ��ĵ�һ��������Ϣ
    unsigned int m_iCustPlanOffset;
    //##ModelId=4244159E02C1
    unsigned int m_iServOffset;


    //##ModelId=4247E6AB00BA
    unsigned int m_iGroupOffset;

    unsigned int m_iAcctOffset;
    //��Ʒ��ϸ������
    unsigned int m_iOfferDetailOffset;
};

struct AcctItemOweData
{				
		long acctItemId;
	
		long iServID;	
	
		long iAcctID;	
		
		long iAmount;		
	
		int iBillingCnt;
		
		unsigned int iNextAcctOffset;
	
		unsigned int iNextServOffset;		
	
		char state[4];		
	
		char isValid;
		
		int iParTID ;
		
		char m_sExpDate[16];
};

class AcctCreditInfo
{
public:
	long m_lAcctID;
	long m_lServID;
	int m_iCreditGrade;
	int m_iCreditAmount;
	unsigned int m_iNextOffset;//�ʻ�����һ���û�
};

//##ModelId=41CA62CF031B
//##Documentation
//## �˻���Ϣ��
//## Ŀǰ���ǲ����ص������ڴ棬û���뵽ʲô�ط����ȡ�����Ϣ
class AcctInfo
{
public:
    //##ModelId=41D3B6150041
    long m_lAcctID;

    //##ModelId=41D3C24A0009
    long m_lCustID;

    //##ModelId=41D3C1C800D4
    int m_iCreditGrade;

    //##ModelId=41D3C1D00325
    int m_iCreditAmount;

    //##ModelId=41D3C2750340
    char m_sState[4];

    //##ModelId=41D3C2790115
    //jchar m_sStateDate[16];

    unsigned int m_iNextOffset; //ͬһ��cust�������һ��acct
    //��Ʒ��ϸ������
    unsigned int m_iOfferDetailOffset;

};

//##ModelId=41CA412F0396
//##Documentation
//## ����Ʒʵ����Ϣ����Ҫ��Ϣ��ӦSERV��
//## Ҫ��������servid�����������Ǹ��£�
//## order by custid
class ServInfo
{
  public:
    //##ModelId=42303C5600E2
    //##Documentation
    //## ����Ʒ��ʶ
    long m_lServID;
    //##ModelId=41D27D9C0392
    //##Documentation
    //## �ͻ���ʶ
    long m_lCustID;

    //##ModelId=42436B3C015C
    int m_iProductID;

    //##ModelId=42303BE3037C
    char m_sCreateDate[16];

    //##ModelId=42303C1A02A9
    //##Documentation
    //## ��������
    char m_sRentDate[16];

    //##ModelId=42303C260089
    char m_sNscDate[16];

    //##ModelId=4230F22803CA
    //##Documentation
    //## ״̬
    char m_sState[4];

    //##ModelId=4230F2360077
    char m_sStateDate[16];

    //##ModelId=42303C300278
    //##Documentation
    //## �������
	//short m_iCycleType;
    int m_iCycleType;

    //##ModelId=42436B860054
    char m_sAccNbr[32];

    //##ModelId=42436B720240
    int m_iBillingMode;

    //##ModelId=42436BD40179
    int m_iFreeType;

    //##ModelId=4230EC0F03D9
    //##Documentation
    //## ������Ʒ��ƫ����
    //--unsigned int m_iProductOffset;

    //##ModelId=42355E8D01A9
    //##Documentation
    //## ͬһ�ͻ����¸�SERV
    unsigned int m_iCustNext;

    //##ModelId=42304127015D
    //##Documentation
    //## ��Ʒʵ��ָʾ
    unsigned int m_iOfferInstanceOffset;

    //##ModelId=42310AD0007D
    //##Documentation
    //## ֧����ϵָʾ
    unsigned int m_iServAcctOffset;

    //##ModelId=4243733B01BA
    //##Documentation
    //## ״̬�ı���Ϣ
    unsigned int m_iStateOffset;

    //##ModelId=4243808F024C
    //##Documentation
    //## ������Ʒʵ��ָʾ
    unsigned int m_iServProductOffset;

    //##ModelId=4244078801FD
    //##Documentation
    //## ָ��ServLocationInfo����
    //## ��servid�ĵ�һ����¼
    unsigned int m_iLocationOffset;

    //##ModelId=42493026038F
    //--unsigned int m_iServPlanOffset;

    //##ModelId=4247BA1001CA
    unsigned int m_iGroupOffset;

    //##ModelId=4243A9B5033E
    //##Documentation
    //## �ͻ����Ի�������Ϣ
    //--unsigned int m_iCustPlanOffset;

    unsigned int m_iCustOffset;

    unsigned int m_iAttrOffset;

    unsigned int m_iServTypeOffset;

    unsigned int m_iServIdentOffset;
    
    //��Ʒ��ϸ������
    unsigned int m_iOfferDetailOffset;
    //������Ʒ��ϸ��ƫ��
    unsigned int m_iBaseOfferOffset;
};

/*
	�û���Ʒ�ı����¼	
*/
class ServTypeInfo
{
  public:
	long m_lServID;
	int m_iProductID;

	char m_sEffDate[16];
	char m_sExpDate[16];

	int m_iPricingPlanID;

	unsigned int m_iNextOffset;
};

//##ModelId=42303812025F
//##Documentation
//## ����Ʒ��ʶ��Ϣ����Ӧ����Ʒ��ʶ��
//## ����Ϣ����������ҵ������ȡ��ʶ
//## ҵ����룭>����Ʒʵ����ʶ��������Ʒʵ��
//## ��serv�������
class ServIdentInfo
{
public:

    //##ModelId=4230383E032A
    //##Documentation
    //## ��ҵ�����
    char m_sAccNbr[32];

    //##ModelId=423038BA02C4
    //##Documentation
    //## ����Ʒ��ʶ
    long m_lServID;

    //##ModelId=423038D70027
    //##Documentation
    //## ��Ч����
    char m_sEffDate[16];

    //##ModelId=423038EB0026
    //##Documentation
    //## ʧЧ����
    char m_sExpDate[16];

    //##ModelId=42303908026C
    //##Documentation
    //## ��Ӧ��ServInfo��ƫ����
    unsigned int m_iServOffset;

    //##ModelId=4238D88F00E4
    //##Documentation
    //## ��ͬҵ��������һ����¼������ʱ������
    unsigned int m_iNextOffset;

    //##ModelId=424375570383
    //##Documentation
    //## ͬһ������Ʒʵ������һ��ServIdentInfo�����ڲ��ã����Ҫ����servid��ȡ��ҵ����룬����ֶξ���Ҫʹ��
    unsigned int m_iServNextOffset;

    int m_iType;
    //��������
    int m_iNetworkID;
    //��������ƥ������ȼ���
    int m_iNetworkPriority;

};

//##ModelId=42303A3A017C
//##Documentation
//## ������Ʒʵ����Ϣ��Ҫ��������servproductid��������order by servid��productid��effdate
//## ��serv������أ��������Ҫ��ȡ�������ԣ�����Ҫ����������
class ServProductInfo
{
  public:
    //##ModelId=42303A4602E2
    //##Documentation
    //## ������Ʒʵ����ʶ
    long m_lServProductID;
    //##ModelId=42303A5C0077
    //##Documentation
    //## ����Ʒ��ʶ
    long m_lServID;
    //##ModelId=42303A6201A2
    //##Documentation
    //## ��Ʒ��ʶ
    int m_iProductID;
    //##ModelId=42303A6901F2
    //##Documentation
    //## ��Чʱ��
    char m_sEffDate[16];
    //##ModelId=42303A7C034E
    //##Documentation
    //## ʧЧʱ��
    char m_sExpDate[16];

    //##ModelId=42303A9001B2
    //##Documentation
    //## ĳ����Ʒ��ʶ����һ��������Ʒ
    unsigned int m_iNextOffset;

    //##ModelId=424382AC0037
    //##Documentation
    //## ��Ӧ��serv��ƫ���������ڸ��ݸ�����Ʒʵ����ȡ����Ʒʵ������Ϣ
    unsigned int m_iServOffset;

    //##ModelId=42577F790380
    unsigned int m_iAttrOffset;

};

//##ModelId=42303F7502EA
//##Documentation
//## ��������OfferInstanceID������,�������Ǵ���Ʒ��ϸʵ��������
class OfferInstanceInfo
{
public:
    //##ModelId=42303F9D019D
    //##Documentation
    //## ��Ʒʵ����ʶ
    long m_lOfferInstanceID;

    //##ModelId=42303FA702E2
    //##Documentation
    //## ��Ʒ��ʶ
    int m_iOfferID;

    //##ModelId=42303FBF016A
    //##Documentation
    //## ���ۼƻ���ʶ
    int m_iPricingPlan;

    //##ModelId=4230429500BA
    //##Documentation
    //## ��Чʱ��
    char m_sEffDate[16];

    //##ModelId=4230429E033D
    //##Documentation
    //## ʧЧʱ��
    char m_sExpDate[16];

    //##ModelId=4243BD060168
    //##Documentation
    //## ��ʵ��ID
    ////long m_lChildInstanceID;

    //##ModelId=4243BAEC029B
    //##Documentation
    //## ��ʵ����ƫ����
    unsigned int m_iParentOffset;
    //##ModelId=4247D69C03C6
    //##Documentation
    //## ͬһ��child����ͬʱ�����ڲ�ͬ��parent��ָ����һ��
    unsigned int m_iNextOffset;
    //##ModelId=4247EF3C02DA
    char m_sState[4];
    //##ModelId=424B9D7B0269
    long m_lDetailID;

};

//##ModelId=4230EA47006C
//##Documentation
//## �û���������
//## ��֧�ֻ�ȡĳ��OrgID���������SERV
//## ����ʱ���ȼ���ServInfo��ServLocationInfo����ServID��sEffDate�����������������Ͳ�Ҫ��������������ServInfo����������ȡServInfo��λ�ã�Ȼ������ServInfo��iLocationOffset��
//## ����ʱ�����ָ���ĳServID����Ϣ��ͨ��ServInfo���������Ǻǡ�
class ServLocationInfo
{
  public:

    //##ModelId=4230EA4F02EF
    //##Documentation
    //## ����Ʒ��ʶ����ʵ��������ݽṹ������Ҫ����ΪĿǰ��֧�ֻ�ȡĳ��OrgID���������SERV
    long m_lServID;

    //##ModelId=4230EA55018F
    //##Documentation
    //## ��ά�����ı�ʶ
    int m_iOrgID;

    //##ModelId=4230EA8C012A
    char m_sEffDate[16];

    //##ModelId=4230EA940262
    char m_sExpDate[16];

    //##ModelId=4230EAD30244
    //##Documentation
    //## ͬһ������Ʒ��ʶ�ĵ������Ե���һ����
    //## ������Чʱ������
    unsigned int m_iNextOffset;

    //ͬһ��org����һ��
    unsigned int m_iOrgNext;

};

//��Ʒʵ���Ľṹ
class ProdOfferIns
{
public:
    //��Ʒʵ��ID
    long m_lProdOfferInsID;
    //��Ʒʵ����ϸ��ƫ��
    unsigned int m_iOfferDetailInsOffset;
    //�ۼ�����ƫ��
    unsigned int m_iAccumuOffset;
    //��Ʒʵ�����Ե�ƫ��
    unsigned int m_iOfferAttrOffset;
    //add by zhaoziwei
    //��Ʒʵ��Э���ڵ�ƫ��
    unsigned int m_iOfferAgreementOffset;
    //end
    //��ƷID
    int m_iOfferID;
    //��Ч����
    char m_sEffDate[16];
    //ʧЧ����
    char m_sExpDate[16];
};
//��Ʒ��ϸʵ���Ľṹ,����ͨ��(�û�,�ʻ�,�ͻ�,��Ʒʵ��ID==m_iInsType)���ҵ�,�����Ҫ����������ƷID��index
class OfferDetailIns
{
public:
    //��Ʒʵ����ϸID
    long m_lOfferDetailInsID;
	//��Ʒʵ��ID(20090413 wangs Ϊ�˲������ȵ�ϸ����
    long m_lOfferInsID;
    //��ϸID(����Ϊ�û�ID���ʻ�ID���ͻ�ID����Ʒʵ��ID)
    long m_lInsID;
    //��ϸID����(1,2,3,4)
    int m_iInsType;
    //��Ʒ��ϸID
    int m_iMemberID;
    //��Ʒʵ����ƫ����
    unsigned int m_iParentOffset;
    //ͬһ��ʵ���������һ����ϸ��ƫ����
    unsigned int m_iNextDetailOffset;
    //ͬһ����ϸID����һ����ϸ��ƫ����
    unsigned int m_iNextProdOfferOffset;
    //��Ч����
    char m_sEffDate[16];
    //ʧЧ����
    char m_sExpDate[16];
};

class OfferDetailInsBase
{
public:
    //��Ʒʵ����ϸID
    long m_lOfferDetailInsID;
	//��Ʒʵ��ID
    long m_lOfferInsID;
    //��ϸID(����Ϊ�û�ID���ʻ�ID���ͻ�ID����Ʒʵ��ID)
    long m_lInsID;
    //��ϸID����(1,2,3,4)
    int m_iInsType;
    //��Ʒ��ϸID
    int m_iMemberID;
    //��Ч����
    char m_sEffDate[16];
    //ʧЧ����
    char m_sExpDate[16];
};

//��Ʒ����ʵ��
class ProdOfferInsAttr
{
public:
    //���Ա�ʶ
    int m_iAttrID;
    //��Ч����
    char m_sEffDate[16];
    //ʧЧ����
    char m_sExpDate[16];
    //����ֵ
    char m_sAttrValue[32];
    //���
    long m_iAttrSeq;
    //ͬһ����Ʒʵ���������һ������ʵ��
    unsigned int m_iNextOfferInsAttrOffset;
};

//add by zhaoziwei
//��Ʒʵ��Э����
class ProdOfferInsAgreement
{
public:
    //��Ʒʵ��Э����ID
    long m_lAgreementInstID;
    //��Ʒʵ��ID
    long m_lProdOfferInsID;
    //��Ч����
    char m_sEffDate[16];
    //ʧЧ����
    char m_sExpDate[16];
    //ͬһ����Ʒʵ���������һ��Э����ʵ��
    unsigned int m_iNextOfferInsAgreementOffset;
};
//end

//##ModelId=4230EB89003D
class ProductInfo
{
public:
    //##ModelId=4230EB9103E2
    int m_iProductID;

    //##ModelId=4230EBA102A5
    int m_iProviderID;

    //##ModelId=4230EBD3036F
    int m_iPricingPlanID;
    //��Ʒ������������
    int m_iNetworkID;
    //��������ƥ�����ȼ�
    int m_iNetworkPriority;
};

//##ModelId=4231071600AD
//##Documentation
//## �����ƹ�ϵ
//## ��serv�������
//## order by servid
class ServAcctInfo
{

 public:
 	
 	//��ϵ��ʶ
	long m_lServAcctID;
    //##ModelId=42310A750195
    //##Documentation
    //## �˻���ʶ
    long m_lAcctID;

    //##ModelId=42310AA000D8
    //##Documentation
    //## ����Ʒ��ʶ
    long m_lServID;

    //##ModelId=42310AA9038E
    //##Documentation
    //## ��Ŀ��
    int m_iAcctItemGroup;

    //##ModelId=423505E002FB
    //##Documentation
    //## ״̬����Ϊ����ǻ
    //## 1Ϊ���0Ϊ�ǻ
    int m_iState;

    char m_sEffDate[16];

    char m_sExpDate[16];

    //##ModelId=42310ABB0037
    //##Documentation
    //## ͬһ����Ʒ��ʶ��һ�����ƹ�ϵ
    unsigned int m_iNextOffset;

    //##ͬһ��ACCT����һ��ƫ����
    unsigned int m_iAcctNextOffset;
};


//##ModelId=42310B7502AB
//##Documentation
//## �ͻ����Ի�������Ϣ
//## ��cust�������
class CustPricingPlanInfo
{
public:
    //##ModelId=42310B920004
    long m_lCustID;

    //##ModelId=42310BA4008C
    int m_iPricingPlanID;

    //##ModelId=42310BAD0266
    int  m_iObjectType;

    //##ModelId=42310BBA0015
    long m_lObjectID;

    //##ModelId=42310BC802D2
    char m_sEffDate[16];

    //##ModelId=42310BD1002C
    char m_sExpDate[16];

    //##ModelId=42310BDE0180
    unsigned int m_iNextOffset;
    //##ModelId=424B592702E8
    //##Documentation
    //## ���Ի�������Ϣ��ʶ��
    //## ����
    long m_lCustPlanID;

    //##ModelId=42AE46330172
    unsigned int m_iParamOffset;


    //##ModelId=42AE463B030E
    unsigned int m_iObjectOffset;
};

//##ModelId=42437DDA020A
//##Documentation
//## ����Ʒʵ���������ԣ������Ŀǰ�Ȳ����أ�����Ժ���Ҫ�õ��������أ�����ҲӦ���ǰ���ĳ�������أ�
class ServAttrInfo
{
public:
    //##ModelId=42437E370362
    //##Documentation
    //## ����Ʒ��ʶ
    long m_lServID;

    //##ModelId=42437E4702CF
    //##Documentation
    //## ���Ա�ʶ
    int m_iAttrID;

    //##ModelId=42437E5002AA
    //##Documentation
    //## ����ֵ
    char m_sValue[16];

    //##ModelId=42437E5800CA
    //##Documentation
    //## ��Ч����
    char m_sEffDate[16];

    //##ModelId=42437E5E02F0
    //##Documentation
    //## ʧЧ����
    char m_sExpDate[16];

    //##ModelId=42437E6502FA
    //##Documentation
    //## ͬһ������Ʒʵ������һ������ֵ��Ϣ���������Ա�ʶ����
    unsigned int m_iNextOffset;

};

//##ModelId=424378CB037A
//##Documentation
//## ����Ʒʵ����״̬��Ϣ
class ServStateInfo
{
public:
    //##ModelId=4243793301FD
    //##Documentation
    //## ����Ʒʵ����ʶ�����ﲢ����Ҫ�ã���Ϊ����Ҫ��ȡĳһʱ�䣬ĳһ״̬�µ�����servid
    long m_lServID;

    //##ModelId=4243793B005A
    //##Documentation
    //## Ƿ������
    int m_iOweType;

    //##ModelId=42437955029C
    //##Documentation
    //## ״̬
    char m_sState[4];

    //##ModelId=4243796000FD
    //##Documentation
    //## ��Чʱ��
    char m_sEffDate[16];

    //##ModelId=42437967025C
    //##Documentation
    //## ʧЧʱ��
    char m_sExpDate[16];

    //##ModelId=4243798800C9
    //##Documentation
    //## ͬһ��SERVID��Ӧ��״̬��Ϣ����һ������ʱ������
    unsigned int m_iNextOffset;

};

//##ModelId=424382DD002E
//##Documentation
//## ������Ʒʵ����������Ϣ��Ŀǰ��������أ�����Ҫ�����أ�����ҲӦ�ð���ĳ���������أ�����������ȷ����ЩҪ���أ�����ServProductInfo�������
class ServProductAttrInfo
{
public:
    //##ModelId=42438321037F
    //##Documentation
    //## ������Ʒʵ����ʶ��
    long m_lServProductID;

    //##ModelId=42438342012D
    //##Documentation
    //## ���Ա�ʶ
    int m_iAttrID;

    //##ModelId=424383490106
    //##Documentation
    //## ����ֵ
    char m_sValue[16];

    //##ModelId=4243835603D5
    //##Documentation
    //## ��Ч����
    char m_sEffDate[16];

    //##ModelId=4243835E0106
    //##Documentation
    //## ʧЧ����
    char m_sExpDate[16];

    //##ModelId=4243838303C6
    //##Documentation
    //## ͬһ��������Ʒʵ���µ���һ�����ԣ��������Ա�ʶ����
    unsigned int m_iNextOffset;

};

//##ModelId=4243B93400AE
//##Documentation
//## ����Ʒ���ۼƻ���Ϣ������Ǹ��ݿͻ����Ի�������Ϣ����Ʒ���ۼƻ�����Ʒ���ۼƻ����������,
//## Ŀǰ���Ȳ�����
//## ������������Ϣ�󣬼����ȡ
class ServPricingPlanInfo
{
public:
    //##ModelId=4243B9D1032A
    //##Documentation
    //## ���ۼƻ���ʶ
    int m_iPricingPlanID;

    //##ModelId=4243BA000115
    //##Documentation
    //## ��Чʱ��
    char m_sEffDate[16];

    //##ModelId=4243BA0C022B
    //##Documentation
    //## ʧЧʱ��
    char m_sExpDate[16];

    //##ModelId=4243BA1803C3
    //##Documentation
    //## ��һ�����ۼƻ���Ϣ
    unsigned int m_iNextOffset;

};

//##ModelId=4243BDB903AA
//##Documentation
//## ��Ʒ��Ϣ����Ʒ��Ϣ�Ͳ�Ʒ��Ϣ��Ӧ��ֻ�����ӣ������޸ģ�����������ƷID�Ͳ�ƷID������
class OfferInfo
{
public:
    //##ModelId=4243BE0E024E
    int m_iOfferID;

    //##ModelId=4243BE1E030F
    int m_iPricingPlan;

    ///int null;

};

//##ModelId=4243C95F01BE
//##Documentation
//## �û�Ⱥ��Ϣ����serv���غ���أ��������Բ�������
class ServGroupInfo
{
public:
    //##ModelId=4243CA150197
    //##Documentation
    //## �û�ȺID
    long m_lGroupID;

    //##ModelId=4243CA520145
    //##Documentation
    //## ����Ʒʵ����ʶ
    long m_lServID;

    //##ModelId=4243CA7203E0
    //##Documentation
    //## ��Ч����
    char m_sEffDate[16];

    //##ModelId=4243CA7D038C
    //##Documentation
    //## ʧЧ����
    char m_sExpDate[16];

    //##ModelId=4243CA9200A7
    //##Documentation
    //## ͬһ��servid����һ���û�Ⱥ��Ϣ
    unsigned int m_iNextOffset;

    unsigned int m_iGroupNext;

};

/*	add by xueqt 2005-08-18	*/
class ServGroupBaseInfo
{
public:
	long m_lGroupID;
	long m_lGroupServID;
	//unsigned int m_iCustPlanOffset;
	unsigned int m_iCustOffset;
	unsigned int m_iServOffset;

	// add by xueqt 20060512
	int m_iGroupType; // 1: Tradition;  2: Realtime
};

//##ModelId=4243D69B036E
//##Documentation
//## �ͻ�������Ϣ���������Ҫ�õ��ͻ������ԣ�����ȫ�������ݱ�ĸ�ʽ���Խ�Լ�ռ䣩
class CustAttrInfo
{
public:
    //##ModelId=4243D6EA00DD
    //##Documentation
    //## �ͻ���ʶ
    long m_lCustID;

    //##ModelId=4243D6F102E5
    //##Documentation
    //## ���Ա�ʶ
    int m_iAttrID;

    //##ModelId=4243D6FF01A5
    //##Documentation
    //## ����ֵ
    char m_sValue[32];

    //##ModelId=4243D70701B1
    //##Documentation
    //## ��Ч����
    char m_sEffDate[16];

    //##ModelId=4243D74C0354
    //##Documentation
    //## ʧЧ����
    char m_sExpDate[32];

    //##ModelId=4243D754012F
    //##Documentation
    //## ͬһ��cust����һ������ֵ
    unsigned int m_iNextOffset;

};

//##ModelId=424B98C602AC
class InstanceDetailUpdateInfo
{
public:
    //##ModelId=424BA08200F0
    long m_lDetailID;

    //##ModelId=424B990B0387
    //##Documentation
    //## ��Ʒʵ����ʶ
    long m_lOfferInstanceID;

    //##ModelId=424B990B03A6
    //##Documentation
    //## ��Ʒ��ʶ
    int m_iOfferID;

    //##ModelId=424B990B03E2
    //##Documentation
    //## ��Чʱ��
    char m_sEffDate[16];

    //##ModelId=424B990C0022
    //##Documentation
    //## ʧЧʱ��
    char m_sExpDate[16];

    //##ModelId=424B990C0068
    //##Documentation
    //## ��ʵ����ƫ����
    char  m_sObjectType[4];

    //##ModelId=424B990C0090
    //##Documentation
    //## ͬһ��child����ͬʱ�����ڲ�ͬ��parent��ָ����һ��
    long m_lObjectID;

    //##ModelId=424B990C00B8
    char m_sState[4];

};

//add 20060710
class CustPricingTariffInfo
{
  public:
    long m_lCustPlanID;
    int  m_iUnit;
    int  m_iRate;
    int  m_iPrecision;
    
    unsigned int m_iLeft;
    unsigned int m_iRight;
    int  m_iBalance;
};

//##ModelId=42AE43D203C5
class CustPricingParamInfo
{
  public:
    //##ModelId=42AE43E502AA
    //##Documentation
    //## �ͻ�������Ϣ��ʶ
    long m_lCustPlanID;

    //##ModelId=42AE440F03D7
    int m_iParamID;

    //##ModelId=42AE441A0333
    long m_lValue;

    //##ModelId=42AE444000E8
    unsigned int m_iNextOffset;

};

//##ModelId=42AE45230077
class CustPricingObjectInfo
{
  public:
    //##ModelId=42AE457603B5
    long m_lCustPlanID;

    //##ModelId=42AE45AC0150
    int m_iPricingObjectID;

    //##ModelId=42AE45ED02F8
    int m_iObjectType;

    //##ModelId=42AE4605018A
    long m_lObjectID;

    //##ModelId=42AE46150223
    unsigned int m_iNextOffset;

};

#ifdef GROUP_CONTROL
class GroupInfo
{
public:
	int		m_iGroupID;
	char	m_sGroupName[101];
	char	m_sGroupDesc[251];
	int		m_iConditionID;
	int		m_iStrategyID;
	char	m_sEffDate[16];
	char	m_sExpDate[16];
	int		m_iPriority;
	int		m_iOrgID;

	unsigned int m_iNextOffset;//ͬһgroup_id����һ��¼
};

class GroupMember
{
public:
	long	m_lMemberID;
	char	m_sAccNbr[33];
	int		m_iGroupID;
	char	m_sEffDate[16];
	char	m_sExpDate[16];

	unsigned int m_iNextOffset;//ͬһ�������һ��¼
};

class NumberGroup
{
public:
	long	m_lRelationID;
	char	m_sAccNbr[33];
	int		m_iGroupID;
	char	m_sEffDate[16];
	char	m_sExpDate[16];
	long	m_lOfferInstID;

	unsigned int m_iNextOffset;//ͬһ�������һ��¼
};
class AccNbrOfferType
{
public:
	long m_lItemID;
	char m_sAccNbr[32];		//����
	 //## ��Чʱ��
    char m_sEffDate[16];

    //##ModelId=424B990C0022
    //##Documentation
    //## ʧЧʱ��
    char m_sExpDate[16];
	int  m_iOfferType;
	unsigned int m_iNextOffset;
};

struct GroupInfoResult
{
	int		m_iGroupID;
	int		m_iConditionID;
	int		m_iStrategyID;
	int		m_iPriority;
	long	m_lOfferInstID;
};
#endif

#ifdef	IMSI_MDN_RELATION
struct ImsiMdnRelation{
	long	m_lRelationID;
	char	m_sImsi[MAX_IMSI_LEN];
	char	m_sAccNbr[MAX_CALLING_NBR_LEN];
	char	m_sAreaCode[MAX_BILLING_VISIT_AREA_CODE];
	char	m_sEffDate[16];	
	char	m_sExpDate[16];	
	int 	m_itype;

	unsigned int m_iNextOffset; //ͬһImsi����һ��¼
};
#endif

//�������� add by xn 2009.10.22
class NpInfo
{
	public:
	//	long m_lNpID;//���� ��Ҫ��
		//long m_lAgreementID;//�ͺ���һ��Ψһָ��һ����¼
		char m_sAccNbr[33]; //NP����
		int  m_iOrgNetwork;//ԭ����
		int  m_iNetwork;//������
		int  m_iNpState;//״̬ 5��
		char m_sEffDate[16];//��Чʱ��
		char m_sExpDate[16];//ʧЧʱ��
		unsigned int  m_iNext;//��ͬ�ĺ�������һ��
};
//##ModelId=42451EF10130
class UserInfoBase
{
//	friend class IvpnInfoAdmin;	//add by zhangyk	
  public:
    //##ModelId=4247679B031B
    UserInfoBase();
	UserInfoBase(pathFullName oFileName);
    //##ModelId=424A148D0363
    operator bool();

 // protected:
    //##ModelId=424930FE0349
    void bindData();

    //##ModelId=4247582700E2
    //##Documentation
    //## �ͻ���Ϣ������
    static USERINFO_SHM_DATA_TYPE<CustInfo> *m_poCustData;

    //##ModelId=424758460317
    //##Documentation
    //## �ͻ���Ϣ����������������custid������
    static USERINFO_SHM_INT_INDEX_TYPE *m_poCustIndex;

    //##ModelId=4247586C039E
    //##Documentation
    //## �ͻ�������Ϣ������
    static USERINFO_SHM_DATA_TYPE<CustPricingPlanInfo> *m_poCustPricingPlanData;

    static USERINFO_SHM_INT_INDEX_TYPE * m_poCustPricingPlanIndex;

    static USERINFO_SHM_DATA_TYPE<CustPricingTariffInfo> *m_poCustTariffData;
    static USERINFO_SHM_STR_INDEX_TYPE *m_poCustTariffIndex;

    //##ModelId=424758860201
    //##Documentation
    //## �ͻ�Ⱥ��Ϣ������
    static USERINFO_SHM_DATA_TYPE<CustGroupInfo> *m_poCustGroupData;

    //##ModelId=424758A301F9
    //##Documentation
    //## �ͻ�����������
    ////static SHMData<CustAttrInfo> *m_poCustAttrData;


    //##ModelId=424759C600D5
    //##Documentation
    //## ����Ʒʵ������������
    static USERINFO_SHM_DATA_TYPE<ServAttrInfo> *m_poServAttrData;

    //##ModelId=42475A4A0139
    //##Documentation
    //## ������Ʒʵ����������������
    static USERINFO_SHM_DATA_TYPE<ServProductAttrInfo> * m_poServProductAttrData;

    //##ModelId=42475E120131
    //##Documentation
    //## �Ƿ�ɹ����ӹ����ڴ�
    static bool m_bAttached;

    //##ModelId=424759500248
    //##Documentation
    //## ����Ʒʵ����������������
    static USERINFO_SHM_DATA_TYPE<ServLocationInfo> *m_poServLocationData;

    static USERINFO_SHM_INT_INDEX_TYPE * m_poServLocationIndex;


    //##ModelId=4247598E0142
    //##Documentation
    //## ����Ʒʵ����ʶ������
    static USERINFO_SHM_DATA_TYPE<ServIdentInfo> *m_poServIdentData;

  
    //##ModelId=42492E2300B3
    static CustInfo *m_poCust;

    //##ModelId=4247597A0360
    //##Documentation
    //## ����Ʒʵ��״̬����������
    static USERINFO_SHM_DATA_TYPE<ServStateInfo> *m_poServStateData;


    //##ModelId=42475A6B0028
    //##Documentation
    //## �����ƹ�ϵ������
    static USERINFO_SHM_DATA_TYPE<ServAcctInfo> *m_poServAcctData;

  
    //##ModelId=42492E400083
    static ServInfo *m_poServ;

    //##ModelId=424758FB021D
    //##Documentation
    //## ��Ʒ��Ϣ������
    static USERINFO_SHM_INT_INDEX_TYPE *m_poProductIndex;


    //##ModelId=424945490231
    static CustPricingPlanInfo *m_poCustPlan;

    //##ModelId=424758E1022A
    //##Documentation
    //## ��Ʒ��Ϣ������
    static USERINFO_SHM_DATA_TYPE<OfferInfo> *m_poOfferData;


    //##ModelId=424759FA01B6
    //##Documentation
    //## ����Ʒ��ʶ������������ҵ����������
    static USERINFO_SHM_STR_INDEX_TYPE *m_poServIdentIndex;

  
    //##ModelId=42492E7F003D
    static ServIdentInfo *m_poServIdent;

  
    //##ModelId=424948060351
    static ProductInfo *m_poProduct;

    //##ModelId=42475A1300B7
    //##Documentation
    //## ������Ʒʵ��������
    static USERINFO_SHM_DATA_TYPE<ServProductInfo> *m_poServProductData;


    //##ModelId=42492E4902FD
    static CustGroupInfo *m_poCustGroup;

  
    //##ModelId=42492E9A0317
    static ServProductInfo *m_poServProduct;
    //##ModelId=42475D3902A1
    //##Documentation
    //## ����Ʒʵ�����ۼƻ���Ϣ��
    static USERINFO_SHM_DATA_TYPE<ServPricingPlanInfo> *m_poServPricingPlanData;


    //##ModelId=42492E570018
    static ServLocationInfo *m_poServLocation;

  
    //##ModelId=42492EA902C9
    static ServAcctInfo *m_poServAcct;

    //##ModelId=42475909028C
    //##Documentation
    //## ��Ʒ��Ϣ������
    static USERINFO_SHM_INT_INDEX_TYPE *m_poOfferIndex;


    //##ModelId=42492EBD028B
    static OfferInstanceInfo *m_poOfferInstance;

    //##ModelId=42475A330212
    //##Documentation
    //## ������Ʒʵ������������������Ǹ�����Ʒʵ���ĸ������ԣ����������Ҫ��
    static USERINFO_SHM_INT_INDEX_TYPE *m_poServProductIndex;

    //## ServAcct���Acct������
    static USERINFO_SHM_INT_INDEX_TYPE * m_poServAcctIndex;


    //##ModelId=42492ED60151
    static ServGroupInfo *m_poServGroup;

    //##ModelId=424758CC0252
    //##Documentation
    //## ��Ʒ��Ϣ������
    static USERINFO_SHM_DATA_TYPE<ProductInfo> *m_poProductData;


    //##ModelId=42475A9101DB
    //##Documentation
    //## ��Ʒʵ����ϸ��Ϣ
    static USERINFO_SHM_DATA_TYPE<OfferInstanceInfo> *m_poOfferInstanceData;

  
    //##ModelId=424BAF5B0150
    static USERINFO_SHM_INT_INDEX_TYPE *m_poOfferInstanceIndex;


    //##ModelId=42475D610136
    //##Documentation
    //## �û�Ⱥ��Ϣ������
    static USERINFO_SHM_DATA_TYPE<ServGroupInfo> *m_poServGroupData;

    static USERINFO_SHM_DATA_TYPE<ServGroupBaseInfo> * m_poServGroupBaseData;
    static USERINFO_SHM_INT_INDEX_TYPE * m_poServGroupBaseIndex;

    //##ModelId=4247593001D3
    //##Documentation
    //## ����Ʒʵ��������
    static USERINFO_SHM_DATA_TYPE<ServInfo> *m_poServData;

    static USERINFO_SHM_DATA_TYPE<ServTypeInfo> * m_poServTypeData;

    static USERINFO_SHM_DATA_TYPE<CustPricingParamInfo> * m_poCustPricingParamData;

    static USERINFO_SHM_DATA_TYPE<CustPricingObjectInfo> * m_poCustPricingObjectData;

    static USERINFO_SHM_DATA_TYPE<AcctInfo> * m_poAcctData;
    
    static AcctInfo * m_poAcct;

    static USERINFO_SHM_INT_INDEX_TYPE * m_poAcctIndex;

    //##ModelId=4247593E0350
    //##Documentation
    //## ����Ʒʵ��������
    static USERINFO_SHM_INT_INDEX_TYPE *m_poServIndex;
    
    //��Ʒʵ�����ݺ�����
    static USERINFO_SHM_DATA_TYPE<ProdOfferIns> * m_poProdOfferInsData;
    static ProdOfferIns * m_poProdOfferIns;
    static USERINFO_SHM_INT_INDEX_TYPE * m_poProdOfferInsIndex;
    static USERINFO_SHM_DATA_TYPE<OfferDetailIns> * m_poOfferDetailInsData;
    static OfferDetailIns * m_poOfferDetailIns;
    static USERINFO_SHM_INT_INDEX_TYPE * m_poOfferDetailInsOfferInsIndex;
    static USERINFO_SHM_DATA_TYPE<ProdOfferInsAttr> * m_poProdOfferInsAttrData;
    static ProdOfferInsAttr * m_poProdOfferInsAttr;
    //add by zhaoziwei
		static USERINFO_SHM_DATA_TYPE<ProdOfferInsAgreement> * m_poProdOfferInsAgreementData;
		static ProdOfferInsAgreement * m_poProdOfferInsAgreement;
		//end
		
    //##ModelId=42492E70030D
    static ServStateInfo *m_poServState;

    static ServAttrInfo * m_poServAttr;

    static ServProductAttrInfo * m_poServProductAttr;

    static CustAttrInfo * m_poCustAttr;

#ifdef GROUP_CONTROL
	static GroupInfo * m_poGroupInfo;
	static USERINFO_SHM_DATA_TYPE<GroupInfo> *m_poGroupInfoData;
	static USERINFO_SHM_INT_INDEX_TYPE *m_poGroupInfoIndex;
	static GroupMember * m_poGroupMember;
	static USERINFO_SHM_DATA_TYPE<GroupMember> *m_poGroupMemberData;
	static USERINFO_SHM_STR_INDEX_TYPE *m_poGroupMemberIndex;
	static NumberGroup * m_poNumberGroup;
	static USERINFO_SHM_DATA_TYPE<NumberGroup> *m_poNumberGroupData;
	static USERINFO_SHM_STR_INDEX_TYPE *m_poNumberGroupIndex;

	static USERINFO_SHM_DATA_TYPE<AccNbrOfferType>* m_poAccNbrData;
	static USERINFO_SHM_STR_INDEX_TYPE * m_poAccNbrIndex;
	static USERINFO_SHM_INT_INDEX_TYPE *m_poAccNbrItemIndex;
	static AccNbrOfferType* m_poAccNbr;
	
#endif

#ifdef IMSI_MDN_RELATION 
	static ImsiMdnRelation * m_poImsiMdnRelation;
	static USERINFO_SHM_DATA_TYPE<ImsiMdnRelation> * m_poImsiMdnRelationData;
	static USERINFO_SHM_STR_INDEX_TYPE* m_poImsiMdnRelationIndex;//imsi����
#endif

//�������� add by xn 2009.10.22
	static USERINFO_SHM_DATA_TYPE<NpInfo> *m_poNpData;
	static NpInfo *m_poNp;
	static USERINFO_SHM_STR_INDEX_TYPE* m_poNpIndex;
//����

 // private:
    //##ModelId=42452759033D
    //##Documentation
    //## ���ع����ڴ���Ϣ
    void loadSHMInfo();
    //##ModelId=4247790103D4
    void freeAll();
    
    void closeAll();

};

//Ϊ�˿ͻ����Ի��Ķ��۲����Ͷ��۶���
class ExPricingPlan
{
	public:
		int m_iPricingPlanID;
		unsigned int m_iCustPlanOffset;
};

#endif /* USERINFO_H_HEADER_INCLUDED_BDBC7D5F */
