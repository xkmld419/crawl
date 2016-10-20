/*VER: 9*/ 
#ifndef  __RT_MAKE_STOP_H__
#define  __RT_MAKE_STOP_H__

#include "StdEvent.h"
#include "Process.h"
#include "MBC.h"
#include "Log.h"
#include "AttrTransMgr.h"
//#include "TransactionMgr.h"
#include "InstTableListMgr.h"
//#include "EventSection.h"
#include "UserInfoReader.h"
#include "Date.h"
#include "CommonMacro.h"
#include "UserInfo.h"
#include "UserInfoReader.h"
#include <atmi.h>
#include <tx.h>
#include <fml32.h>

//#define	 DEF_STAFF_ID 	1;
#define	 DEF_STAFF_ID (ParamDefineMgr::getLongParam("RT_MAKE_STOP", "DEFAULT_STAFF"))

/* �ο�Ƿ�Ѵ���ҵ�����͡�OWE_BUSINESS_TYPE���е�����ֵ��*/
#define  OWE_BTYPE_ID_CJ		1					/*�߽�*/
#define  OWE_BTYPE_ID_TJ		6					/*��ͣ��*/
#define  OWE_BTYPE_ID_ST		2					/*˫ͣ��*/
#define  OWE_BTYPE_ID_DK        8                   /*����*/
#define  OWE_BTYPE_ID_SK        9                   /*˫��*/
#define  OWE_BTYPE_ID_TX		17					/*�������*/
#define  OWE_BTYPE_ID_CR    19          /*͸֧����*/
#define  OWE_BTYPE_ID_ZZ    31          /*��ֵ�����Ƿ������û��ſ��ж�,Ĭ�ϲ�����,���ڼ�¼������*/
#define  OWE_STATE	 "2HS"
#define  OPEN_STATE  "2HA"
#define  STOP_STATE  "2HD"
//add by yangch for SuZhou

struct B_RT_STOP_DETAIL{
		long			lStopStepID;
		long		    lServID;
		int				iType;
		int				iBillingCycleID;
		int				iBillFlowID;
		int				iOrgID300;
		int				iBillingOrgID;
		char 			sStartDate[MAX_START_DATE_LEN];
		long			lCharge1;
		int				iAcctItemTypeID1;
		int				iSourceEventType;
};


struct SERV_OWE_CHARGE_BUF{
    long lServID;
    long lRtCharge;
    long lOweCharge;
    long lCharge;
};

typedef struct TStopLimit{
	int iLimitID;
	int iOrgID;
	int iLimitType;
	int iLimitMax;
	char cLimitUnit;
	int iLimitAmount;
	int iLimitMin;
	char sLimitSlot[10];
	int iStopLimitType[3];
}TStopLimit;


typedef struct TLimitCmp{
    struct TStopLimit aStopLimit;
	struct TLimitCmp *pNext;
}TLimitCmp;

struct B_STOP_OFFER{
	long lServID;
	int  iOweBTypeID;
};

#define     A_CNT       0
#define     A_CMP       1
#define     TIME_SUM    0
#define     TIME_SLOT   1
#define     TIME_FIRST  2

//����ͣ����ֵ�Ŀ���
#define STOP_LIMIT_TJ    0    //��ͣ
#define STOP_LIMIT_ST    1    //˫ͣ
#define STOP_LIMIT_CJ    2    //�߽�

//����һ����ά���飬��0λ��ʾ�����ۼ���
//��1λ��ʾ�Ƿ����Ƚϣ�1�����룬0����
#define     MIN_ARRAY   1440

//��ȡ��������
#define TABLE_TYPE_ACCT_ITEM_AGGR   11

#define		ONCE_STOP_CNT	 10000
#define		MAX_OWE_DUN_CNT	 5

#define     MAX_TIMESET_CNT     6

struct BackupDataCfg
{
    char sTimeMin[MAX_TIMESET_CNT][MAX_TIMESET_CNT];
	char sTimeMax[MAX_TIMESET_CNT][MAX_TIMESET_CNT];
	int iSillCnt;
};

class Rt_Stop_Make :public UserInfoBase
{
		private:
			int	iOrgID300;
			int	iProcssID;
			long glMaxCharge;
			long glMinCharge;
			char g_sSourceType[50];
			bool bSourceType;
			static long	glBalance;
			static long	glOweCharge;
			static long	glAggrCharge;
			static int  giBalanceCnt;
			static int 	giOweCnt;
			static int	giAggrCnt;
			static int  giCreditCnt;
			static int 	giAcctCredit;
			
			static long giLastlServID;			
			static int  giCalcCredit;
			static int  giCalcLevel;//�ϴμ��㼶��			
			
			static int 	giBillingCycleID;
			static long glSourceServID;
			static  long lacctResItemBalance;
			static long lservResItemBalance;
			static int oneSleepValidTime;
			
			//static char gsPreTableName[256];
			//B_RT_STOP_DETAIL	b_rt_stop_detail;
			AttrTransMgr		*pAttrTransMgr;
			static 	char sTableName[256];
			InstTableListMgr	tablelistMgr;
			int iServCnt;
			StdEvent gstrEvent;
			BillingCycleMgr * m_poBCM;
			TOCIQuery *qry;
			void  InsertStopMiddle(long lServID,long lAcctID,int iErrorID);
			int CheckStopMiddleExist(long lServID);
			void removeStopMiddleLog(long lServID);
			
			HashList<struct BackupDataCfg>  * m_pBackupData;
			void loadBackupData();
			void backupData(int iOweBTypeID);
			void insertBackData(int iOweBTypeID);
			
		public:
		    static void resetBalanceCnt(){ giBalanceCnt = 0;};
			static char gsPreTableName[256];
			static int	giCurOrgID300;
			static long 	glMaxChargeVlue;

			Rt_Stop_Make(int iProcessID,int iOrgID300);	
            ~Rt_Stop_Make();
            B_RT_STOP_DETAIL	b_rt_stop_detail;
			int		getServStopRecord();
			void	deleteStopDetail();
			void 	analyzeTalbeStopDetail();
			void	doWithStopDetail();
			//ͣ���Ҵ߽�,�߽������
			void	OweDunAndWorkOrder(const long lAcctID);
			//ͣ���Ҵ߽�,���޴߽ɴ���
			void	OweDunAndWorkOrderServ(const long lServID);
			void  InsertWorkOrder(long lServID,int iOweBTypeID,char * sState);
			void	WorkOrderFromAcct(const long lAcctID,int iOweBTypeID);
			void 	InsertOweDun(long lServID,int iOweBTypeID);
			void  OweDunFromAcct(const long lAcctID,int iOweBTypeID);
			void	OpenWorkOrderFromAcct(const long lAcctID,int iOweBTypeID);
			long  GetSerialNbr();
			long  GetOweDunID();
			int		GetOweSpecialServ(long lServID,int iType);
			int		GetOweSpecialProduct(int iProductID,int iOweBTypeID,int iBillingModeID);
			int 	CheckStopExist(long lServID);
			int 	CheckOweDunExist(long lServID,int iOweBusinessTypeID);
			void    UpdateServState(long lServID, int iOweBTypeID, char sState[],char sPreState[]);
			static  int GetAcctCreditDB(const long lAcctID);
			static  int GetServCreditDB(const long lServID);
			static  int GetServBillingMode(const long lServID);
			static	long GetCurAggrCharge(long lServID,long lAcctID,int iOrgID,int iAcctItemGroupID,int iBillingCycleID);
			static	long GetDBPartyRoleCharge(long lServID,int iBillingCycleID,int iPartyRoleID);
			static	long GetCurAggrChargeComm(long lServID,long lAcctID,int iBillingCycleID,int iType,int iAcctItemGroupID);
			static	long GetPreAggrChargeCommZz(long lServID,long lAcctID,int iBillingCycleID,int iType,int iOrgID);
			static	long GetCurAggrChargeBill(long lServID,long lAcctID,int iBillingCycleID,int iType,int iAcctItemGroupID,char * sPreTableName);
			static	long GetOweCharge(long lServID,long lAcctID,int iAcctItemGroupID);
    		static  long getNoPartyRoleOwe(long lServID,long lAcctID,int iType,int iPartyRoleID);
    		static	long  GetBalanceCharge(long lServID,long lAcctID, int iAcctItemGroupID);
			static	long  GetUpperBalance(long lServID,long lAcctID,int iBillingCycleID, int iAcctItemGroupID);
			static bool checkTime(bool &isFirst);
			int		GetServState(long lServID,char sState[4],int *iBillingModeID,int *iProductID);
			void	InsertOweDunCnt(long lAcctID,int iBalance,int iCnt);
			void	UpdateOweDunCnt(long lAcctID,int iBalance,int iCnt);
			void	CheckOweDunCnt(long lAcctID,int *iCnt,int *iInsertFlag);
			//���޸ģ�long lServID, long lAcctID.20070316
			static long getAggrCharge(long lServID,long lAcctID,int iBillingCycle,int iType,int iAcctItemType);		
			static long getPartyCharge(long lServID,long lAcctID,int iBillingCycle,int iType,int iPartyRoleID);
			static long GetRealCharge(long lServID,long lAcctID,int iType,int iBillingCycleID,int iAcctItemGroupID);
			static long Get10ACHARGE_DBComm(long lServID,long lAcctID,int iType,int iBillingCycleID,int iAcctItemGroupID);
			static long Get10ACHARGE_DBBill(long lServID,long lAcctID,int iType,int iBillingCycleID,int iAcctItemGroupID,char * sTableName);
			//static TOCIDatabase gpDBLink;
			int BillConn(char sUser[],char sPwd[],char sStr[]);
			int DisBillConn();
			void OpenWorkOrder(long lServID,int iOweBTypeID);
			int CheckServState(char *sState,int iType);
			
			static long LocalFunction(long lAcctID, long lServID,int iBillingCycleID, int iParam1 = -1, int iParam2 = -1);
			
			static int GetDiffMin(char sStartTime[15], char sCurTime[15]);
			void InitStopLimit(int iOrgID);
			static void DoWaitWork(int iLimitID);
			static int GetBeginTimeArray(char sStartTime[], char sSlotTime[]);
			static int AddCountLimit(int iLimitFlag);
			static int GetDateStr(char sDate[]);
			int  DealWithHighFee(long lServID,int iOweBTypeID,char *state);
			int GetAnotherMem(long lAcctID, long lServID, long &lAnotherServID,int iSourceOweBTypeID);
			
			//IP����
			void InsertIPOweDun(long lServID, int iAction);
			int CheckOweDunIPExist(long lServID);
			
			void InsertOrderBank(long lServID, int iType);
			int CheckAutoFunds(long lServID);
			
			static int getStopMiddleLateDate(long lServID);
			static long getStopMiddleCharge(long lServID);

            static int  GetBalanceByServe(long lAcctID,long lServID,int iParam1, int iParam2,long * plBalance);
            static long GetOweChargeByMemory(long lAcctID,long lServID,int iBillingCycleID,int iAcctItemTypeID);
            static long GetCurChargeByMemoryZz(long lAcctID,long lServID,int iBillingCycleID,int iAcctItemTypeID);
            static long GetBalanceByFunc(long lAcctID,long lServID,int iBillingCycleID,int iParam1, int iParam2 );
			static long GetBalanceByFuncEx(long lAcctID,long lServID,int iBillingCycleID,int iParam1, int iParam2 );            
            static long GetBalance(long lAcctID,long lServID,int iFundsLevelID,int iBillingCnt,int iCredit = 1);
            static long GetOweChargeByStruct(long lAcctID,long lServID,int iAcctItemTypeID);
            void OpenCommit();
     //���Ӵ��ʻ��������û������
     static	long  GetAcctUnionServBalance(long lServID,long lAcctID,int iBillingCycleID, int iAcctItemGroupID);
     static	long  GetAcctUnionServBalaceByTj(long lServID,long lAcctID,int iBillingCycleID, int iAcctItemGroupID);
     void  InsertCreditLog(long lServID,int iMaxCreditValue,char sState[]);
     static int getCredibilityUser(long lServID);
     void setSourceType(char sSourceType[]);
     static vector<struct B_STOP_OFFER>  m_strStopOffer;
     int CheckOweDunExistCDMA(long lServID,int iOweBusinessTypeID);
     int loadOfferDetail(long lServID,int iSourceOweBTypeID);
     static long getValidBalance(long lAcctID,long lServID,int iBillingCycleID,long * plServItemBalance,long * plAcctItemBalance);
};

#endif

