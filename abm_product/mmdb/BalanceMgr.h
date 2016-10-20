/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef BALANCEMGR_H_HEADER_INCLUDED
#define BALANCEMGR_H_HEADER_INCLUDED

#include <vector>
#include "SHMData.h"
#include "SHMIntHashIndex.h"

using namespace std;

//#define BALANCEBUF_DATA_COUNT	3000000			//��������ݶγ���
#define BALANCEBUF_DATA_COUNT	(ParamDefineMgr::getLongParam("MEMORY_DB", "BALANCE_DATA_COUNT"))

struct BalanceData{					//��¼�ṹ
	long lAcctID;					//�ʻ���ʶ
	long lServID;					//�û���ʶ
	int iAcctItemTypeID;				//��Ŀ���ͱ�ʶ
	int iBalance;					//Ƿ��
	unsigned int iNextAcctOffset;
	unsigned int iNextServOffset;
};

class BalanceMgr
{
  public:
	BalanceMgr();

	~BalanceMgr();

	int getServBalance(long lServID);
	int getAcctBalance(long lAcctID);
	long GetAcctUServBalance(long lServID,long lAcctID);
	void load();
	void unload();
  int UpdateBalanceMemory();
  int getAcctBalanceV(long lAcctID,vector<struct BalanceData> & vData);
  void printBalanceMemory();
  private:
	static SHMData<BalanceData> * m_poBalanceData;
	static SHMIntHashIndex * m_poBalanceAcctIndex;
	static SHMIntHashIndex * m_poBalanceServIndex;

  private:
	static bool m_bUploaded;

};

#endif /* BALANCEMGR_H_HEADER_INCLUDED */
