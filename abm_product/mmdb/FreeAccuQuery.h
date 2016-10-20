/*VER: 13*/ 
#ifndef FREE_ACCU_QUERY_H
#define FREE_ACCU_QUERY_H

#include <vector>
#include "OfferAccuMgr.h"
#include "ParamDefineMgr.h"
#include "ProdOfferAggr.h"
#include "LifeCycleMgr.h"
#include "UserInfoReader.h"
#include "HashList.h"
#include "ID.h"
#include "Value.h"
#include <math.h>
#include "AttrValueCalc.h"
#include "BillConditionMgr.h"
#include "PricingInfoMgr.h"
#include "AcctItemMgr.h"

using namespace std;

const int OPER_FLAG_CG = 1;
const int OPER_FLAG_PARSER = 2;
const int OPER_FLAG_SECTIONPARSER = 3;
const int OPER_FLAG_OFFERNOPERCENT = 4;
const int OPER_FLAG_SECTIONPARSER_IN = 5;
const int JUST_FLAG_ESTU=2;

struct ItemName
{
	char m_sName[128];
	int  m_iCalRuleId;
	//��չ�ۻ�������:�� ���ѣ����ţ������WAP�������ȵȣ�
	int m_RemindTypeID;
};

struct TableData
{
	int		m_iBillingCycleID;
	int		m_iOrgID;
	char	m_sTableName[51];
};

struct ConditionVal 
{
	int m_iOfferID;
	int m_iCombineID;
	int m_iObjectID;
	ID * m_poParamID1;
	ID * m_poDesParamID;
	LifeCycle *m_poLifeCycle;
	int m_iMeasureSeq;	
    ConditionVal * m_poNext;
};

struct ConditionValTemp
{
	int  m_iOfferID;
	int  m_iCombineID;
	int  m_iObjectID;
	char m_sParamID1[32];
	char m_sDesParamID[32];
	int  m_iLifeCycleID;
	int  m_iMeasureSeq;	
};

struct ConditionResult
{
	int  m_iCombineID;
	long m_lValue ;
	int m_iMeasureSeq;
};
//sunjy 
struct FreeOfferAttrEx
{
	int  m_iQueryID;
	int  m_iOfferID;
	int  m_iAttrID;
	char m_sAttrValue[32];
	int  m_iValue;
	int  m_iType;
	FreeOfferAttrEx *m_poNext;
};

class FreeAccuQueryResult {
public:
    char m_sOfferName[128];     //��Ʒ����
    char m_sExtendItem[200];    //��������Ŀ wuan 2010-4-29
    char m_sAccuName[128];      //�ۻ�������
    char m_sStartTime[16];      //��ʼʱ��
    char m_sEndTime[16];        //����ʱ��
    char m_sUnitName[32];       // ��λ
    char m_sTotal[16];          //����
    char m_sAlready[16];        //������
    char m_sLeft[16];           //ʣ��
    char m_sRemark[128];        //��ע ����չ���ڴ�Ų�ѯ����
	int	  m_iQueryID;			  //����
	int	  m_iQueryFlag;			  //��ѯ������־
	int	  m_iOfferID;			  //��ƷID
	int	  m_iCalRuleID;			  //b_accu_cal_rule�������
	int  m_iAccuType;           //�ۻ���������:0 -- ���� 1 -- ʹ����
	int	 m_iRemindType;         //��չ�ۻ�������:�� ���ѣ����ţ������WAP�������ȵȣ�
    int m_iAccuTypeID;          //�ۻ����ͱ�ʾ
	char m_sSplitby[32];		//�������
};

class FreeAccuCfg {
    public:
		int m_iQueryID;
        //int m_iAccuTypeID; 
		ID * m_poObject;
        //ID * m_poFreeValue;
        int m_iUnitNume;
        int m_iUnitDeno;
        char m_sUnitName[32];
		int m_iQueryFlag;
		int m_iAdjustVal;
		int m_iAdjustFlag;
		char m_sQueryDesc[128];
		int m_iOperFlag;
		int m_iOfferID;
		char m_sExtendItem[200];    //��������Ŀwuan 2010-4-29
    	int m_iMergeFlag;           //�ۻ�����ѯ����ϲ���־��wuan 2010-05-17
        FreeAccuCfg * m_poNext;
    };

class FreeAccuQuery {
public:

    //sDate�������������ȡsysdate������vector����ļ�¼��
    int query(vector<FreeAccuQueryResult> &vResult,
            long lServID, char *sDate=0);

    //sDate�������������ȡsysdate������vector����ļ�¼��
    int queryDB(vector<FreeAccuQueryResult> &vResult,
            long lServID, char *sDate=0);

    int remindQueryAccu(vector<FreeAccuQueryResult> &vResult, 
		        EventSection * poEvent, OfferInsQO *pQO);
  
	static FreeAccuQuery * m_pQuery;
public:
    FreeAccuQuery();
	~FreeAccuQuery();

private:
    int queryOfferInst(vector<FreeAccuQueryResult> &vResult,
            long lServID,OfferInsQO *pQO);
    
	int queryOfferInstDB(vector<FreeAccuQueryResult> &vResult,
            long lServID,OfferInsQO *pQO);

	//int getConditionVal(ID * pParam1ID,ConditionResult * pOutConditionResult);
	int getConditionVal(FreeAccuCfg * pAccuCfg,ConditionResult * pOutConditionResult);
	int operProcess(vector<FreeAccuQueryResult> &vResult,long lServID,
		OfferInsQO *pQO,FreeAccuCfg * pAccuCfg);
	int getAggrByServ(long lServID,long lOfferInstID,char * sEffDate, char * sExpDate,long &lVal);
	void addResult(vector<FreeAccuQueryResult> &vResult,char * sOfferName,char * sAccuName,
		char * sEffDate,char * sExpDate,long lMaxVal,long lUsedVal,
		FreeAccuCfg * pAccuCfg,int iCalRuleID = 0,int iRemindTypeID = 0,char * sSpliby = 0);
	bool getCurDays(int &iUseDays,int &iToalDays);
	void parserObject(FreeAccuCfg * pAccuCfg);
    void loadCfg();
	void loadInit();
	void unload();

private:
    static OfferAccuMgr *m_poAccuMgr;
    static OfferInsQO   *m_poOfferInstBuf;
	static ConditionResult   * m_poResultBuf;
	static BillingCycleMgr * m_poBCM;
	static ProdOfferDisctAggr * m_poProdOfferDisctAggr;
	static ConditionVal  * m_pConditionValList;
	static HashList<ConditionVal *> *m_pConditionVal;

private:
	float MyRound(float fVal)
	{
		return (float)((long)(fVal*1000+5)/10)/100;
	}

	static FreeAccuCfg *m_poAccuCfgList ;
	static ItemName * m_poOfferNameList;
	static ItemName * m_poAccuNameList;
    static HashList<FreeAccuCfg *> *m_poAccuIndex;
    static HashList<FreeAccuCfg *> *m_poItemIndex;
	static HashList<ItemName *> *m_poOfferNameIndex;
	static HashList<ItemName *> *m_poAccuNameIndex;   
	static HashList<int> * m_poOfferID;
	static HashList<int> * m_poTableName;//����Ѿ�������ı���
	static AccuTypeCfg * m_pCfg;
	
	static int	m_iFirstMonth;
	static int m_iMemFlag;
	
	//sunjy
	static FreeOfferAttrEx *m_poAttrExlist;
	static HashList<FreeOfferAttrEx *> *m_poAttrExIndex;

	static BillConditionMgr    *m_poConditionMgr;
	static PricingInfoMgr      *m_poPricingInfoMgr;
	static HashList<int>		*m_poOfferSectionParser;
	static EventSection			*pEvent;

	static int	m_iDisplayRealCharge;//��ʾ�����ۻ�����ʹ��ֵΪʵ��ֵ�Ŀ��أ�1 ��ʾ
	static int	m_iDisplayRealAggr;  //��ʾʱ�����ۻ�����ʹ��ֵΪʵ��ֵ�Ŀ��أ�1 ��ʾ
	static int m_iCount;	//�ۻ�����ѯ��������
	static int m_iCountVal; //���ݿ����򶨼۹����������
	static bool m_bUploaded;

	int m_iBillingCycleID;
	char m_sDate[15];
	int m_iComNum;
	int m_ioCombineID[MAX_PRICING_COMBINE_NUMBER];
};

#endif

