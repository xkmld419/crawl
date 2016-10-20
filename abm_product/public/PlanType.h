/*VER: 1*/ 
#ifndef PLAN_TYPE_H
#define PLAN_TYPE_H

#include "HashList.h"

/*
	�Ѷ��ۼƻ�������������.
	Ϊ�˽������ҵ��� �˺ź͹̶��绰 ������; 
	��Ϊ������Ʋ�֧�����������ϵ�����������
*/

class PlanTypeMgr
{
  public:
	int getPlanType(int iPricingPlanID);

  public:
	PlanTypeMgr();
	
	void load();
	void unload();

  private:
	static bool m_bUploaded;
	static HashList<int> * m_poIndex;
  	
};

#endif
