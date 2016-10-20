//ActiveSendReverseBizSql.h
#ifndef __ACTIVE_SEND_REVERSE_BIZ_SQL_H_INCLUDED_
#define __ACTIVE_SEND_REVERSE_BIZ_SQL_H_INCLUDED_

#include "TTTns.h"
#include "PublicConndb.h"
#include <vector>

#define REVERSE_DEDUCT_DELAY_SECOND	"1001";// ϵͳ���أ������������ͻ��������ӳ�����

struct ActiveSendLog
{
	char sRequestId[32];
	char sRequestType[4];
	char sState[4];	
	char sOrgId[5];
};


struct SysSwitch
{
	char *sSwitchId[5];
	char *sValue[2];
	char *sDesc[101];	
};

class ABMException;

class ActiveSendReverseBizSql : public TTTns
{
	public:
		ActiveSendReverseBizSql();
		~ActiveSendReverseBizSql();
		
		// ��ѯδ�յ�ʡ�ظ��ļ�����������CCR
		int qryActiveSendLog(vector<ActiveSendLog> &vActiveSendLog,ABMException &oExp);
		
		// ����CCR��¼״̬
		int updateCCRInfo(char *sRequestId);
		
		// ��ѯϵͳ���أ�
		int getSystemSwitch(char *switchId,char *switchValue);
		
		// ��ѯ���ͳ������ӳ�����
		int getDelaySecond(long &lDelaySecond);
		
		// ������ʱ����������ѯ��¼
		int qryActiveSendLog(vector<ActiveSendLog> &vActiveSendLog,long &lDelaySecond,ABMException &oExp);
};

#endif/*__ACTIVE_SEND_REVERSE_BIZ_SQL_H_INCLUDED_*/
