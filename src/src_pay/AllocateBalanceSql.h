#ifndef __ALLOCATE_BALANCE_SQL_H_INCLUDED_
#define __ALLOCATE_BALANCE_SQL_H_INCLUDED_

#include "TTTns.h"
#include "dcc_ra.h"
#include "PublicConndb.h"

class ABMException;

class AllocateBalanceSql : public TTTns
{
	public:
		AllocateBalanceSql();
		~AllocateBalanceSql();
		
		//int initDbConn(ABMException &oExp);
		
		// ��ѯ�û���Ϣ
		int preQryServAcctInfo(string servNbr,int iServAttr,ServAcctInfo &oServAcct,ABMException &oExp);
		
		// ���������ѯ
		int preQryDeductRule(long lAcctId,TransRule &oTransRule,ABMException &oExp);
		
		// ͳ���û�����Ƶ�ȼ����
		int preQryServDayDeductInfo(long lServId,TransRule &oTransRuleDay,TransRule &oTransRuleMonth,ABMException &oExp);
		
};

#endif/*__ALLOCATE_BALANCE_SQL_H_INCLUDED_*/

