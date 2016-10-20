#include "QryServInfosynSql.h"
#include "ABMException.h"

extern TimesTenCMD *m_pInsertServHis;//��¼SERV��ʷ��Ϣ��
extern TimesTenCMD *m_pInsertAcctHis;//��¼ACCT��ʷ��Ϣ��
extern TimesTenCMD *m_pInsertServAcctHis;//��¼SERV_ACCT��ʷ��Ϣ��
extern TimesTenCMD *m_pDeleteAcctBalance;//ɾ������˵����Ϊ0����
extern TimesTenCMD *m_pDeleteServInfo;//ɾ��SERV����Ϣ
extern TimesTenCMD *m_pDeleteAcctInfo;//ɾ��ACCT����Ϣ
extern TimesTenCMD *m_pDeleteServAcctInfo;//ɾ��SERV_ACCT����Ϣ
extern TimesTenCMD *m_pChangeInservHis;//�����ü�¼SERV��ʷ��Ϣ��
extern TimesTenCMD *m_pChangeNbr;//����

extern TimesTenCMD *m_pChangeAcct;//����
extern TimesTenCMD *m_pUpdateServAcct;//��������SERV_ACCT��
using namespace std;


QryServInfosynSql::QryServInfosynSql()
{
}

QryServInfosynSql::~QryServInfosynSql()
{
}
/*
int QryServInfosynSql::init(ABMException &oExp)
{
	try{
			if (GET_CURSOR(m_poSend, m_poTTConn, m_sConnTns, oExp) != 0) {
            ADD_EXCEPT0(oExp, "QryServInfosynSql::init m_poSend init failed!");
            return -1;
        }
        
			return 0;
		}
		catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
    }
    return -1;
}
*/
//����
int QryServInfosynSql::createAcct(QryServSynCond *syn,ABMException &oExp)
{
	__DEBUG_LOG0_(0, "enter createAcct...");
	try
	{
		m_pChangeAcct->setParam(1,syn->m_sDsnNbr);
		m_pChangeAcct->Execute();
		m_pChangeAcct->Close();
		
		return 0;
	}
	catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}

int QryServInfosynSql::updateServAcct(QryServSynCond *syn,ABMException &oExp)
{
		__DEBUG_LOG0_(0, "enter createAcct...");
		try
	{
		
		m_pUpdateServAcct->setParam(1,syn->m_sDsnNbr);
		m_pUpdateServAcct->setParam(2,syn->m_sDsnNbr);
		m_pUpdateServAcct->Execute();
		m_pUpdateServAcct->Close();
		
		return 0;
	}
	catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}

//����
int QryServInfosynSql::insertServHis(QryServSynCond *syn,ABMException &oExp)
{
		__DEBUG_LOG0_(0, "enter insertServHis...");
		try
		{
        if(strcmp(syn->m_sActType,"02") == 0){
        		m_pInsertServHis->setParam(1,"2HG");	//����
        		__DEBUG_LOG0_(0, "setParam ����");
        	}
        else if(strcmp(syn->m_sActType,"03") == 0){
        		m_pInsertServHis->setParam(1,"2HB");	//����
        		__DEBUG_LOG0_(0, "setParam 2HB");
        	}
        
        __DEBUG_LOG0_(0, "satParam STATE...");
		  m_pInsertServHis->setParam(2,syn->m_sDsnNbr);
		  __DEBUG_LOG0_(0, "satParam DSNNBR...");
		  
		  m_pInsertServHis->Execute();
		  __DEBUG_LOG0_(0, "Execute...");
		  //m_pInsertServHis->Commit();
		  m_pInsertServHis->Close();        
         return 0;
		}
		catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}

int QryServInfosynSql::insertAcctHis(QryServSynCond *syn,ABMException &oExp)
{		
		__DEBUG_LOG0_(0, "enter insertAcctHis...");
		try
		{
        if(strcmp(syn->m_sActType,"02") == 0){
        		m_pInsertAcctHis->setParam(1,"2G");	//����
        	}
        else if(strcmp(syn->m_sActType,"03") == 0){
        		m_pInsertAcctHis->setParam(1,"2X");		//����
        	}
        
		  m_pInsertAcctHis->setParam(2,syn->m_sDsnNbr);
		  
		  m_pInsertAcctHis->Execute();
		  __DEBUG_LOG0_(0, "Execute...");
		  //m_pInsertAcctHis->Commit();
		  m_pInsertAcctHis->Close();        
         return 0;
		}
		catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}

int QryServInfosynSql::insertServAcctHis(QryServSynCond *syn,ABMException &oExp)
{
		__DEBUG_LOG0_(0, "enter insertServAcctHis...");
		try
		{
        if(strcmp(syn->m_sActType,"02") == 0){
        		m_pInsertServAcctHis->setParam(1,"2HG");	//����
        	}
        else if(strcmp(syn->m_sActType,"03") == 0){
        		m_pInsertServAcctHis->setParam(1,"2HB");	//����
        	}
        	
		  m_pInsertServAcctHis->setParam(2,syn->m_sDsnNbr);
		  
		  m_pInsertServAcctHis->Execute();
		  __DEBUG_LOG0_(0, "Execute...");
		  //m_pInsertServAcctHis->Commit();
		  m_pInsertServAcctHis->Close();        
         return 0;
		}
		catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}

int QryServInfosynSql::deleteAcctBalance(QryServSynCond *syn,ABMException &oExp)
{
		__DEBUG_LOG0_(0, "enter deleteAcctBalance...");
		try
		{
		  m_pDeleteAcctBalance->setParam(1,syn->m_sDsnNbr);
		  
		  m_pDeleteAcctBalance->Execute();
		  __DEBUG_LOG0_(0, "Execute...");
		  //m_pDeleteAcctBalance->Commit();
		  m_pDeleteAcctBalance->Close();        
         return 0;
		}
		catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}

int QryServInfosynSql::deleteServInfo(QryServSynCond *syn,ABMException &oExp)
{
		__DEBUG_LOG0_(0, "enter deleteServInfo...");
		try
		{
		  m_pDeleteServInfo->setParam(1,syn->m_sDsnNbr);
		  
		  m_pDeleteServInfo->Execute();
		  __DEBUG_LOG0_(0, "Execute...");
		  //m_pDeleteServInfo->Commit();
		  m_pDeleteServInfo->Close();        
        return 0;
		}
		catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}

int QryServInfosynSql::deleteAcctInfo(QryServSynCond *syn,ABMException &oExp)
{
		__DEBUG_LOG0_(0, "enter deleteAcctInfo...");
	try{
		  m_pDeleteAcctInfo->setParam(1,syn->m_sDsnNbr);
		  
		  m_pDeleteAcctInfo->Execute();
		  __DEBUG_LOG0_(0, "Execute...");
		  //m_pDeleteAcctInfo->Commit();
		  m_pDeleteAcctInfo->Close();        
        return 0;
		}
		catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}

int QryServInfosynSql::deleteServAcctInfo(QryServSynCond *syn,ABMException &oExp)
{
		__DEBUG_LOG0_(0, "enter deleteServAcctInfo...");
	try{
		  m_pDeleteServAcctInfo->setParam(1,syn->m_sDsnNbr);
		  
		  m_pDeleteServAcctInfo->Execute();
		  __DEBUG_LOG0_(0, "Execute...");
		  //m_pDeleteServAcctInfo->Commit();
		  m_pDeleteServAcctInfo->Close();        
        return 0;
		}
		catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}

//����
int QryServInfosynSql::changeInservHis(QryServSynCond *syn,ABMException &oExp)
{
		__DEBUG_LOG0_(0, "enter changeInservHis...");
	try{
			m_pChangeInservHis->setParam(1,"2HH");		//����
			m_pChangeInservHis->setParam(2,syn->m_lServID);
			m_pChangeInservHis->Execute();
			__DEBUG_LOG0_(0, "Execute...");
			m_pChangeInservHis->Close();
			
			return 0;
		}
		catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}

int QryServInfosynSql::changeNbr(QryServSynCond *syn,ABMException &oExp)
{
		__DEBUG_LOG0_(0, "enter changeNbr...");
	try{
			m_pChangeNbr->setParam(1,syn->m_sDsnNbr);
			m_pChangeNbr->setParam(2,syn->m_lServID);
			m_pChangeNbr->Execute();
			__DEBUG_LOG0_(0, "Execute...");
			m_pChangeNbr->Close();
			
			return 0;
		}
		catch (TTStatus oSt) {
        ADD_EXCEPT1(oExp, "QryServInfosynSql::init oSt.err_msg=%s", oSt.err_msg);
		}
		return -1;
}