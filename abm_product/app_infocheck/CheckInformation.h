#ifndef __CHECK_INFORMATION_H_HEADER
#define __CHECK_INFORMATION_H_HEADER

#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;
#include "CheckStatInterface.h"

const int MODULE_GATHER  =1;
const int MODULE_PREP    =2;
const int MODULE_PRICING =3;
const int MODULE_INSTORE =4;

#define CAPABILITY_TITLE "S-KPIID|S-KPIName|S-KBP|S-KBPName|F-Min|F-Avg|F-Max"
#define CONFIG_TITLE "S-KPIID|S-KPIName|S-KBP|S-KBPName|I-Action|S-Value"
#define ALARM_TITLE "S-KPIID|S-KPIName|S-AlarmID|S-KBP|S-KBPName|D-FirstTime|D-LastTime|I-EventLevel|I-EventState|D-AckTime|S-AckUser|D-ClearTime|S-ClearUser|I-Count"

struct CDR_Info 
{
	long recvAllFileCount;
	long recvDelayFileCount;
	long recvAllRecordCount;
	long recvDelayRecordCount;
	long recvErrFileCount;
	
	//��������-Ԥ����
	long prepAllRecordCount;
	long prepErrRecordCount;
	long prepDupRecordCount;
	
	//��������-����
	long pricingAllRecordCount;
	long pricingUseLessRecordCount;
	long pricingErrRecordCount;
	long pricingDupRecordCount;
	long pricingBlackRecordCount;
	long pricingOkRecordCount;
	
	//��������-���
	long indbInAllRecordCount;
	long indbOutAllRecordCount;
	long indbErrRecordCount;
	
	void init();
	void parse();
};


class Information
{
private :	
	map<string,string> m_pCodeList;
	map<string,string> m_pKpiList;
	int m_iDelayTimeSecs; //��ʱʱ��
	int m_mode;
	int m_iFlag;
	
private :
	vector<string> m_vGatherInfo;
	vector<string> m_vPrepInfo;
	vector<string> m_vPricingInfo;
	vector<string> m_vInDbInfo;
	
	string strValue;
	float ratio;
	char buf[32+1];
	char m_source[255];
	char m_busiclass[255];
	
	//�澯�ж�
	bool m_bPrepInOut;
	bool m_bRecvOutPrepIn;
	bool m_bPricingInOut;
	bool m_bPrepOutPricingIn;
	bool m_bPricingOutIndbIn;
	
private :
	bool gatherInfo();
	bool prepInfo();
	bool pricingInfo();
	bool indbInfo();
	bool init(int iFlag);
	bool checkPerformance(int mode);
	void print(vector<string> vec);
	
	bool insertAlarm(string kpiId,string kpiName,string alarmId,string kbpId,string kbpName,Date occurTime,int modeType);
	long getAppInfo(int appId,bool in);
	bool getAppAlarm(int mode,vector<string> &vec);
	
	//add 2010-11-22
  long getAppInfo(const char *appId,bool in);
public :
	Information();
	~Information();
	//������Ϣ��
  bool checkGather();
	bool checkPerp();
	bool checkPricing();
	bool checkIndb();
	
	//������Ϣ��-ȫ��
	bool checkAllProductInfo();
	bool checkAllProductOfferInfo();
	bool checkAllPricingPlanInfo();
	
	//������Ϣ��-����
	bool checkChangedProductInfo();
	bool checkChangedProductOfferInfo();
	
	//�澯��Ϣ���м�����
	bool checkBalance(int mode);  //�����м�澯��Ϣ������modeȡֵ2��Ԥ����3�����ۣ�4�����
	
	//�澯��Ϣ��
	bool checkPrepAlarm();
	bool checkPricingAlarm();
	bool checkIndbAlarm();
	bool writePrepAlarm();
	bool writePricingAlarm();
	bool writeIndbAlarm();
	
public :
	map<int ,CDR_Info> m_mapCDRInfo;
	map<int ,CDR_Info>::iterator m_mapIter;
		

};

#endif
