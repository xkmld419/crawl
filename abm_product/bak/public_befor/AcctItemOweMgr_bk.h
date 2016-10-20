/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef ACCTITEMOWEMGR_H_HEADER_INCLUDED
#define ACCTITEMOWEMGR_H_HEADER_INCLUDED

#include "SHMData.h"
#include "SHMIntHashIndex.h"

//#define ACCTITEMOWEBUF_DATA_COUNT	4000000			//��������ݶγ���
//20070827 �˴�ע�͵�����ʡ����LocalDefine��ȥ���塣
//��Ϊ0506Ƿ�������޷����أ�����3000000��Ϊ4000000

struct AcctItemOweData{					//��¼�ṹ
	long iAcctID;					//�ʻ���ʶ
	long iServID;					//�û���ʶ
	int iBillingCnt;			//��������
	//int iAcctItemTypeID;				//��Ŀ���ͱ�ʶ
	long lAmount;					//Ƿ��
	unsigned int iNextAcctOffset;
	unsigned int iNextServOffset;
};

class AcctItemOweMgr
{
  public:
	AcctItemOweMgr();

	~AcctItemOweMgr();

	int getServOwe(long i_iServID);
	int getAcctOwe(long i_iAcctID);
	int getBillingCnt(long i_iAcctID);
	void load();
	void unload();
  private:
	static SHMData<AcctItemOweData> * m_poOweData;
	static SHMIntHashIndex * m_poOweAcctIndex;
	static SHMIntHashIndex * m_poOweServIndex;

  private:
	static bool m_bUploaded;

	//void getParameter();
};

#endif /* ACCTITEMOWEMGR_H_HEADER_INCLUDED */
