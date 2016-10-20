//ActiveSendReverseBizSql.cpp
#include "ActiveSendReverseBizSql.h"
#include "ABMException.h"
#include "LogV2.h"

using namespace std;

extern TimesTenCMD *m_poPublicConn;

ActiveSendReverseBizSql::ActiveSendReverseBizSql()
{
	
}

ActiveSendReverseBizSql::~ActiveSendReverseBizSql()
{
	
}

/* 
 * ��ѯACTIVE_CCR_LOG��״̬Ϊ00X�ļ�¼
 *
 */
int ActiveSendReverseBizSql::qryActiveSendLog(vector<ActiveSendLog> &vActiveSendLog,ABMException &oExp)
{
	//__DEBUG_LOG0_(0, "ActiveSendReverseBizSql::qryActiveSendLog---��ѯACTIVE_CCR_LOG��״̬");
	char sql[1024];
	bool bFlag = false;
	int iRet = 1;
	try 
	{
		string sPreSql;
		sPreSql = "SELECT A.SESSION_ID,A.REQUEST_ID,A.CREATE_DATE,A.REQUEST_TYPE,A.ORG_ID,A.STATE,A.STATE_DATE FROM ACTIVE_CCR_LOG A WHERE A.STATE='00X' ";
		
		strcpy(sql,sPreSql.c_str());	
		snprintf(sql, sizeof(sql), "%s", sql);
		m_poPublicConn->Prepare(sql);

		//m_poPublicConn->setParam(1,servNbr.c_str());
		m_poPublicConn->Execute();
		
		//__DEBUG_LOG1_(0, "ActiveSendReverseBizSql::qryActiveSendLog---SQL:[%s]",sql);
		int iCnt =0;
		while (!m_poPublicConn->FetchNext()) 
		{	// ȡ��¼��Ϣ
			ActiveSendLog avtiveSendLog;
			m_poPublicConn->getColumn(2, avtiveSendLog.sRequestId);
			m_poPublicConn->getColumn(4, avtiveSendLog.sRequestType);
			m_poPublicConn->getColumn(5, avtiveSendLog.sOrgId);
			
			vActiveSendLog.push_back(avtiveSendLog);
			bFlag = true;
			iCnt++;
		}	
		//__DEBUG_LOG1_(0, "ActiveSendReverseBizSql::qryActiveSendLog---iCnt:[%d]",iCnt);
		m_poPublicConn->Close();
		if(bFlag)
		{
			iRet = 0;
		}
		
		return iRet;
	}
	catch (TTStatus oSt)
	{
		ADD_EXCEPT1(oExp, "ActiveSendReverseBizSql::qryActiveSendLog oSt.err_msg=%s", oSt.err_msg);
	}
	iRet = -1;
	
	return iRet;	
}

int ActiveSendReverseBizSql::updateCCRInfo(char *sRequestId)
{
	int iRet = 0;
	try 
	{
		//__DEBUG_LOG0_(0, "SQL��丳ֵ............");	

		m_pUpdateCCR->setParam(1, sRequestId);

		//__DEBUG_LOG1_(0, "sRequestId:[%s]",sRequestId);

		m_pUpdateCCR->Execute();
		//__DEBUG_LOG0_(0, "Update table:[ACTIVE_CCR_LOG] ok");
		
		m_pUpdateCCR->Close();
		
		//__DEBUG_LOG0_(0, "����DCC-ACTIVE_CCR_LOG��Ϣ�ɹ�");	
		return iRet;
	}
	catch (TTStatus oSt)
	{
		iRet = -1;
		//ADD_EXCEPT1(oExp2, "ActiveSendReverseBizSql::updateCCRInfo oSt.err_msg=%s", oSt.err_msg);
	}
	//__DEBUG_LOG0_(0, "ACTIVE_CCR_LOG-����DCC��Ϣʧ��" );
	return iRet;
}

int ActiveSendReverseBizSql::getSystemSwitch(char *switchId,char *switchValue)
{
	//__DEBUG_LOG0_(0, "ActiveSendReverseBizSql::getSystemSwitch---��ѯϵͳ����-SYS_SWITCH");
	char sql[1024];
	bool bFlag = false;
	int iRet = 1;
	try 
	{
		string sPreSql;
		sPreSql = "SELECT A.SWITCH_ID,A.STATE,A.SWITCH_DESC FROM SYS_SWITCH A WHERE A.SWITCH_ID=:SWITCH_ID ";
		
		strcpy(sql,sPreSql.c_str());	
		snprintf(sql, sizeof(sql), "%s", sql);
		m_poPublicConn->Prepare(sql);

		m_poPublicConn->setParam(1,switchId);
		m_poPublicConn->Execute();
		
		//__DEBUG_LOG1_(0, "ActiveSendReverseBizSql::getSystemSwitch---SQL:[%s]",sql);
		int iCnt =0;
		while (!m_poPublicConn->FetchNext()) 
		{	// ȡ����ֵ
			ActiveSendLog avtiveSendLog;
			m_poPublicConn->getColumn(2, switchValue);
			bFlag = true;
			iCnt++;
		}	
		//__DEBUG_LOG1_(0, "ActiveSendReverseBizSql::getSystemSwitch---iCnt:[%d]",iCnt);
		m_poPublicConn->Close();
		if(bFlag)
		{
			iRet = 0;
		}
		
		return iRet;
	}
	catch (TTStatus oSt)
	{
		//ADD_EXCEPT1(oExp, "ActiveSendReverseBizSql::getSystemSwitch oSt.err_msg=%s", oSt.err_msg);
	}
	iRet = -1;
	
	return iRet;		
}

int ActiveSendReverseBizSql::getDelaySecond(long &lDelaySecond)
{
	//__DEBUG_LOG0_(0, "ActiveSendReverseBizSql::getDelaySecond---��ѯ�ӳٷ�������");
	char sql[1024];
	bool bFlag = false;
	int iRet = 1;
	try 
	{
		string sPreSql;
		sPreSql = "SELECT A.CFG_ID,A.DELAY_SECOND FROM REVERSE_DEDUCT_CFG A ";
		
		strcpy(sql,sPreSql.c_str());	
		snprintf(sql, sizeof(sql), "%s", sql);
		m_poPublicConn->Prepare(sql);

		//m_poPublicConn->setParam(1,switchId);
		m_poPublicConn->Execute();
		
		//__DEBUG_LOG1_(0, "ActiveSendReverseBizSql::getDelaySecond---SQL:[%s]",sql);
		int iCnt =0;
		while (!m_poPublicConn->FetchNext()) 
		{	// ȡ����ֵ
			ActiveSendLog avtiveSendLog;
			m_poPublicConn->getColumn(2, &lDelaySecond);
			bFlag = true;
			iCnt++;
		}	
		//__DEBUG_LOG1_(0, "ActiveSendReverseBizSql::getDelaySecond---iCnt:[%d]",iCnt);
		m_poPublicConn->Close();
		if(bFlag)
		{
			iRet = 0;
		}
		
		return iRet;
	}
	catch (TTStatus oSt)
	{
		//ADD_EXCEPT1(oExp, "ActiveSendReverseBizSql::getSystemSwitch oSt.err_msg=%s", oSt.err_msg);
	}
	iRet = -1;
	
	return iRet;		
}

int ActiveSendReverseBizSql::qryActiveSendLog(vector<ActiveSendLog> &vActiveSendLog,long &lDelaySecond,ABMException &oExp)
{
	//__DEBUG_LOG0_(0, "ActiveSendReverseBizSql::qryActiveSendLog---��ѯACTIVE_CCR_LOG��״̬");
	char sql[1024];
	bool bFlag = false;
	int iRet = 1;
	
	try 
	{
		string sPreSql;

		sPreSql =  " SELECT A.SESSION_ID, ";
		sPreSql += "        A.REQUEST_ID, ";
		sPreSql += "        A.CREATE_DATE,";
		sPreSql += "        A.REQUEST_TYPE,";
		sPreSql += "        A.ORG_ID,";
		sPreSql += "        A.STATE,";
		sPreSql += "        A.STATE_DATE ";
		sPreSql += " FROM ACTIVE_CCR_LOG A ";
		sPreSql += " WHERE A.STATE='00X' ";
		sPreSql += " AND sysdate > A.CREATE_DATE+interval '";
		sPreSql +=  ltoa(lDelaySecond);
		sPreSql += "' second;";

		
		/*
		sPreSql =  " SELECT A.SESSION_ID, ";
		sPreSql += "        A.REQUEST_ID, ";
		sPreSql += "        A.CREATE_DATE,";
		sPreSql += "        A.REQUEST_TYPE,";
		sPreSql += "        A.ORG_ID,";
		sPreSql += "        A.STATE,";
		sPreSql += "        A.STATE_DATE ";
		sPreSql += " FROM ACTIVE_CCR_LOG A ";
		sPreSql += " WHERE A.STATE='00X' ";
		sPreSql += " AND sysdate > A.CREATE_DATE+interval :lDelaySecond second ";
		*/
		
		strcpy(sql,sPreSql.c_str());	
		snprintf(sql, sizeof(sql), "%s", sql);
		m_poPublicConn->Prepare(sql);

		//m_poPublicConn->setParam(1,lDelaySecond);
		m_poPublicConn->Execute();
		
		//__DEBUG_LOG1_(0, "ActiveSendReverseBizSql::qryActiveSendLog---SQL:[%s]",sql);
		int iCnt =0;
		while (!m_poPublicConn->FetchNext()) 
		{	// ȡ��¼��Ϣ
			ActiveSendLog avtiveSendLog;
			m_poPublicConn->getColumn(2, avtiveSendLog.sRequestId);
			m_poPublicConn->getColumn(4, avtiveSendLog.sRequestType);
			m_poPublicConn->getColumn(5, avtiveSendLog.sOrgId);
			
			vActiveSendLog.push_back(avtiveSendLog);
			bFlag = true;
			iCnt++;
		}	
		//__DEBUG_LOG1_(0, "ActiveSendReverseBizSql::qryActiveSendLog---iCnt:[%d]",iCnt);
		m_poPublicConn->Close();
		if(bFlag)
		{
			iRet = 0;
		}
		
		return iRet;
	}
	catch (TTStatus oSt)
	{
		ADD_EXCEPT1(oExp, "ActiveSendReverseBizSql::qryActiveSendLog oSt.err_msg=%s", oSt.err_msg);
	}
	iRet = -1;
	
	return iRet;	
}
