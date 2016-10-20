/***************************************************************************
*	Copyright (c) 2010,���������Ƽ��ɷ����޹�˾���żƷ��з���
*	All rights reserved.
*	
*	�ļ���: TempAccuMgr.h
*	����:	�����м��ۻ�����������װ
*	���ߣ�	wangs
*	���ڣ�	2010-10-09
*	�汾��	1.0
*	���ļ�¼��
****************************************************************************/

#ifndef _TEMP_ACCU_MGR_H
#define _TEMP_ACCU_MGR_H

#include "LocalCacheInfo.h"
#include "ProdOfferAggr.h"
#include "OfferAccuMgr.h"
#include "AccuTypeCfg.h"
#include <vector>

using namespace std;

/*
//LocalCacheInfo.h���ж���
struct DisctAccu
{
	DisctCacheData  m_oDisct;
	vector<DisctDetailCacheData > m_voDetail;
}
*/

//AccuTypeCfg.h���ж��壨�¼��ۻ�����ѯ�������壩
/*struct EventAccuQuery
{
	long m_lInstID;
	vector<int> m_viAccuTypeID;
};*/

const int DISCT_AGGR_NUM		= 128;

class OfferAccuMgr;

class TempAccuMgr
{
	friend class OcsEngine;
public:
	TempAccuMgr(OfferAccuMgr * pAccuMgr);
	
	//������ʼ��
	void init();

	//����¼��ۻ�����ѯ����
	void addEventAccuCond(EventAccuQuery * pCond);

	//����Ż��ۻ�����ѯ����
	void addDisctAccuCond(long lInstID);

	//ʹ�õõ�����Ʒʵ��ID����������������ݲ�ѯ
	void queryData();

	//��Ӳο����¼��ۻ������ݲ�����ֵ
	long addRefEventAccuData(long lOfferInstID,int iAccuTypeID,long lCycleInstID,
		long lServID,long lMemberID,char sSplitby[]);

	//��ѯ�ο����Ż��ۻ�������
	long getRefDisctData(ProdOfferAggrData * pData);

	//����Ż��ۻ�������
	void addDisctData(ProdOfferTotalData *pData,int iOutNum = 0);

	//��ӱ仯���¼��ۻ�������
	void addUpdateEventAccuData(vector<EventAddAggrData> &voData);

	//��ѯ�Ż��ۻ�����ʵ������
	int getDisctAggrNum();

	//��ѯ�Ƿ����ۻ�������
	bool isAggrEmpty();

public:
	//��ѯ�õ��Ż��ۻ�����ͷָ��
	int getDisctAggrHead(ProdOfferTotalData *pData,DisctAccu **ppDisctAccu);

public:
	//�¼��ۻ������ս��
	vector<AccuCacheData> m_voEventAccu;

	//�Ż��ۻ������ս��
	vector<DisctAccu> m_voDisctAccu;

private:
	//�¼��ۻ������ָ�붨��
	AccuTypeCfg * m_poAccuCfg ;
	OfferAccuMgr  * m_poAccuMgr;

private:
	//��ѯ�����¼��ۻ�������
	vector<AccuData> m_voEventAccuBase;

	//�¼��ۻ�����ѯ��������
	vector<EventAccuQuery> m_vEventAccuCond;

	//�Ż��ۻ�����ѯ��������
	vector<long> m_vDisctAccuCond;

	//��ʱ������
	long m_lSeq;

	//�⼸������ÿ������
	long m_lCycleInstID;
	int	  m_iCycleTypeID;
	char m_sBeginTime[16];
	char m_sEndTime[16];
};

#endif //_TEMP_ACCU_MGR_H
