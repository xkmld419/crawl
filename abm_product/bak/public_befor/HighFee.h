#ifndef _HIGHFEE_H_
#define _HIGHFEE_H_ 

#include <stdlib.h>   
#include <stdio.h>  
#include "Environment.h"
#include <map>
#include <vector>

using namespace std;

class HighFeePlan
{
public:
	HighFeePlan():
	  m_iHiFeePlanID(0),
	  m_iOrgID(0),
	  m_iHiFeeStrategyID(0)
	  {
		  memset(m_sEffDate,0,sizeof(m_sEffDate));
		  memset(m_sExpDate,0,sizeof(m_sExpDate));
	  };
	~HighFeePlan(){};

public:
	int		m_iHiFeePlanID;		//�ƻ���ʶ	
	int		m_iOrgID;			//�����ʶ	
	int		m_iHiFeeStrategyID;	//���Ա�ʶ	
	char	m_sEffDate[14];		//��Чʱ��	
	char	m_sExpDate[14];		//ʧЧʱ��
};

class HighFeeSection
{
public:
	HighFeeSection():
	  m_iHiFeeSectionID(0),
	  m_iHiFeeStrategyID(0),
	  m_iHiFeeLevelID(0),
	  m_lStartValue(0),
	  m_lEndValue(0)
	  {
	  };
	~HighFeeSection(){};
		
public:
	int		m_iHiFeeSectionID;	//�����ʶ	
	int		m_iHiFeeStrategyID;	//���Ա�ʶ	
	int		m_iHiFeeLevelID;	//�����ʶ	
	long	m_lStartValue;		//������ʼֵ
	long	m_lEndValue;		//������ֵֹ
	//HighFeeSection *m_poNext;
};

//�߶�ƻ�
typedef map<int,HighFeePlan *> MAPHIPLAN;
//�߶����
typedef vector<HighFeeSection*> VECHISECTION;
typedef map<int,VECHISECTION*> MAPHISECTION;

class HighFee
{
public:
	HighFee(void);
	~HighFee(void);

public:
	MAPHIPLAN		mapHiPlan;
	MAPHISECTION	    mapHiSection;
	
public:
	int LoadHiFeeParam();
	int GetLevelID(int iOrgID, long lValue,long lCharge);
	long GetThreshold(int iOrgID,int iLevelID);
};

#endif
