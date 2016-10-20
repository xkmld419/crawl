#include "AllocateBalanceSql.h"
#include "ABMException.h"
#include "LogV2.h"

#define ATTR_ACCT_ID 0
#define ATTR_SERV_ID 1
#define ATTT_SERV_NBR 2

using namespace std;

extern TimesTenCMD *m_poPublicConn;

AllocateBalanceSql::AllocateBalanceSql()
{
	
}

AllocateBalanceSql::~AllocateBalanceSql()
{
}

/* 
 * ���ݺ��뼰�������ͣ���ѯ�û����ʻ���Ϣ
 *
 */
int AllocateBalanceSql::preQryServAcctInfo(string servNbr,int iServAttr,ServAcctInfo &oServAcct,ABMException &oExp)
{
	char sql[1024];
	bool bFlag = false;
	int iRet = 1;
	try 
	{
		string sPreSql;
		sPreSql = "SELECT A.SERV_ID,A.ACC_NBR,B.ACCT_ID FROM SERV A,SERV_ACCT B WHERE A.SERV_ID = B.SERV_ID AND B.STATE='10A' ";
		switch(iServAttr){
			case ATTR_ACCT_ID:
			{
				sPreSql += " AND B.ACCT_ID =:p0";
				break;
			}	
			case ATTR_SERV_ID:
			{
				sPreSql += " AND A.SERV_ID =:p0";
				break;
			}
			case ATTT_SERV_NBR: 
			{
				sPreSql += " AND A.ACC_NBR =:p0";
				break;
			}	
			default:
				ADD_EXCEPT0(oExp, "AllocateBalanceSql::preQryServAcctInfo iServAttr invalid!");	
		}
		strcpy(sql,sPreSql.c_str());	
		snprintf(sql, sizeof(sql), "%s", sql);
		m_poPublicConn->Prepare(sql);
		//m_poPublicConn->Commit();
		
		m_poPublicConn->setParam(1,servNbr.c_str());
		m_poPublicConn->Execute();
		
		while (!m_poPublicConn->FetchNext()) 
		{	// ȡ�û��ʻ���Ϣ
			m_poPublicConn->getColumn(1, &oServAcct.m_servId);
			m_poPublicConn->getColumn(2, oServAcct.m_servNbr);
			m_poPublicConn->getColumn(3, &oServAcct.m_acctId);
			bFlag = true;
		}	
		m_poPublicConn->Close();
		if(bFlag)
		{
			iRet = 0;
		}
		
		return iRet;
	}
	catch (TTStatus oSt)
	{
		ADD_EXCEPT1(oExp, "AllocateBalanceSql::preQryServAcctInfo oSt.err_msg=%s", oSt.err_msg);
	}
	iRet = -1;
	
	return iRet;	
}

/*
 * ��ѯ��������
 *
 * 2011.5.13 
 */
int AllocateBalanceSql::preQryDeductRule(long lAcctId,TransRule &oTransRule,ABMException &oExp)
{
	char sql[1024];
	int iRet=1;
	bool bFlag = false;
	try 
	{
		string sPreSql;
		sPreSql += " SELECT A.ACCT_ID,A.DATA_TRANS_FREQ,TRANS_LIMIT,MONTH_TRANS_LIMIT,";
		sPreSql += "        A.AUTO_TRANS_FLAG,A.AUTO_TRANS_THRD,A.AUTO_TRANS_AMOUNT,DATA_PAY_FREQ,PAY_LIMIT,MONTH_PAY_LIMIT,MICRO_PAY_LIMIT ";
		sPreSql += " FROM TRANSFER_RULES A where A.ACCT_ID=:p0";		
			
		strcpy(sql,sPreSql.c_str());
			
		snprintf(sql, sizeof(sql), "%s", sql);

		m_poPublicConn->Prepare(sql);
		//m_poPublicConn->Commit();

		m_poPublicConn->setParam(1,lAcctId);
		m_poPublicConn->Execute();
		while (!m_poPublicConn->FetchNext()) 
		{	// ȡ��������������Ϣ
			m_poPublicConn->getColumn(1, &oTransRule.lAcctId);		// �ʻ���ʶ
	
			m_poPublicConn->getColumn(2, &oTransRule.lDayTransFreq);	// ÿ��Ƶ��
			m_poPublicConn->getColumn(3, &oTransRule.lTransLimit);		// ÿ���޶�
			m_poPublicConn->getColumn(4, &oTransRule.lMonthTransLimit);	// ÿ���޶�
			bFlag=true;
		}	
		m_poPublicConn->Close();
		if(bFlag)
		{
			iRet = 0;
		}
	}
	catch (TTStatus oSt)
	{
		iRet = -1;
		ADD_EXCEPT1(oExp, "AllocateBalanceSql::preQryDeductRule oSt.err_msg=%s", oSt.err_msg);
	}
	return iRet;
}

/*
 * ͳ���û�����Ƶ�ȼ����
 *
 * 2011.5.13
 */
int AllocateBalanceSql::preQryServDayDeductInfo(long lServId,TransRule &oTransRuleDay,TransRule &oTransRuleMonth,ABMException &oExp)
{
	char sql[1024];
	char cType[8];
	try 
	{
		string sPreSql;
		
		sPreSql += " SELECT COUNT(*) cnt, SUM(AMOUNT)  sum_amount, '0' sum_type " ; 
  		sPreSql += " FROM OPERATION A ";
		sPreSql += " WHERE A.OPT_DATE >=TO_DATE(TO_CHAR(SYSDATE, 'YYYY/MM/DD'), 'YYYY/MM/DD') ";
		sPreSql += " AND A.SERV_ID=:p0 ";
		sPreSql += " UNION ALL ";
		sPreSql += "  SELECT COUNT(*) cnt, SUM(AMOUNT) sum_amount,'1' sum_type ";
		sPreSql += "  FROM OPERATION A ";
		sPreSql += "  WHERE A.OPT_DATE >= TO_DATE( TO_CHAR(SYSDATE, 'YYYY/MM')||'/01', 'YYYY/MM/DD') ";
		sPreSql += " AND A.OPT_DATE <= TO_DATE(TO_CHAR(SYSDATE, 'YYYY/MM/DD hh24:mi:ss'), 'YYYY/MM/DD hh24:mi:ss')";
		sPreSql += " AND A.SERV_ID=:p0";
		
		strcpy(sql,sPreSql.c_str());
			
		snprintf(sql, sizeof(sql), "%s", sql);
		m_poPublicConn->Prepare(sql);
		//m_poPublicConn->Commit();
		__DEBUG_LOG0_(0, "=====>������SQL============================start");
		m_poPublicConn->setParam(1,lServId);
		m_poPublicConn->Execute();
		__DEBUG_LOG0_(0, "=====>������SQL============================1");
		double dValue;
		while (!m_poPublicConn->FetchNext()) 
		{
			__DEBUG_LOG0_(0, "=====>������SQL============================2");
			// ȡ�ѷ���������Ϣ
			m_poPublicConn->getColumn(3,cType);
			__DEBUG_LOG0_(0, "=====>������SQL============================3");
			__DEBUG_LOG1_(0, "=====>��cType:%s",cType);
			if(memcmp(cType,"0",1) == 0)
			{
				m_poPublicConn->getColumn(1,&oTransRuleDay.lDayTransFreq);
				__DEBUG_LOG0_(0, "=====>������SQL============================4");
				// ���췢���������������
				m_poPublicConn->getColumn(2,&dValue);
				oTransRuleDay.lTransLimit=dValue;
			}
			else
			{
				// ���·����������
				__DEBUG_LOG0_(0, "=====>������SQL============================�¶��");
				m_poPublicConn->getColumn(2,&dValue);
				oTransRuleMonth.lMonthTransLimit=dValue;
				//m_poPublicConn->getColumn(2, &oTransRuleMonth.lMonthTransLimit);
				__DEBUG_LOG0_(0, "=====>������SQL============================5");

			}
			__DEBUG_LOG1_(0, "=====>��lDayTransFreq=%d.",oTransRuleDay.lDayTransFreq);
			__DEBUG_LOG1_(0, "=====>��lTransLimit=%d.",oTransRuleDay.lTransLimit);	
			__DEBUG_LOG1_(0, "=====>��lMonthTransLimit=%d.",oTransRuleMonth.lMonthTransLimit);
		}	
		m_poPublicConn->Close();
		__DEBUG_LOG0_(0, "=====>������SQL============================end");
		return 0;
	}
	catch (TTStatus oSt)
	{
		ADD_EXCEPT1(oExp, "AllocateBalanceSql::preQryDeductRule oSt.err_msg=%s", oSt.err_msg);
	}
	return -1;	
}

