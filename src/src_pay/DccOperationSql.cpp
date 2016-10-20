// DccOperationSql.cpp
#include "DccOperationSql.h"
using namespace std;
ABMException oExp2;
extern TimesTenCMD *m_pSavePlatformCCR;
extern TimesTenCMD *m_pSavePlatformCCRBiz;
extern TimesTenCMD *m_poPublicConn;
extern TimesTenCMD *m_pSaveDcc;
extern TimesTenCMD *m_pUpdateDcc;

extern TimesTenCMD *m_pLoginSaveDcc;	//��½�����Ȩ
extern TimesTenCMD *m_pPasswdChangeDcc; //��½�����޸�

extern TimesTenCMD *m_pSaveActiveCCR; // ����CCR

DccOperationSql::DccOperationSql()
{
	
}

DccOperationSql:: ~DccOperationSql()
{
	
}

int DccOperationSql::QueryPaySeq(long &lSessionIdSeq)
{
	string sql;
	try {
		sql.clear();
		//��ѯDCC SESSION_ID_SEQ

		sql="SELECT SESSION_ID_SEQ.NEXTVAL FROM DUAL ";
				    
		m_poPublicConn->Prepare(sql.c_str());

		m_poPublicConn->Execute();
		while (!m_poPublicConn->FetchNext()) 
		{
		
			m_poPublicConn->getColumn(1, &lSessionIdSeq);
		}
		m_poPublicConn->Close();
		return 0;
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp2, "DccOperationSql::QueryPaySeq oSt.err_msg=%s", oSt.err_msg);
	}
	return -1;
}

// �����Է���ƽ̨�����CCR��Ϣͷ��Ϣ
int DccOperationSql::savePlatformCCR(PlatformDeductBalanceCCR *receiveCCR)
{
	try 
	{
		//TODO�� �ӽ��ܵ��Է���ƽ̨��CCR������Ϣͷ�л�ȡ��Ϣ
		
		struct struDccHead ccrHead;
		char sSessionId[64]={0};
		memcpy((void *)&ccrHead,(void *)(receiveCCR->m_sDccBuf),sizeof(ccrHead));
		// ��Ϣͷ�л�ȡSession-Id
		strcpy(sSessionId,ccrHead.m_sSessionId);
		strcpy(sSessionId,"ABM2@001.ChinaTelecom;xxxxx"); // ����ʱ��д��
		
		/*
		m_pSavePlatformCCR->setParam(1, receiveCCR->);
		m_pSavePlatformCCR->setParam(2, receiveCCR->);
		m_pSavePlatformCCR->setParam(3, receiveCCR->);
		m_pSavePlatformCCR->setParam(4, receiveCCR->);
		m_pSavePlatformCCR->setParam(5, receiveCCR->);
		m_pSavePlatformCCR->setParam(6, receiveCCR->);
		
		*/
			
		__DEBUG_LOG0_(0, "SQL��丳ֵ............");	
	 	m_pSavePlatformCCR->setParam(1, "xxx1");
		m_pSavePlatformCCR->setParam(2, "xxx2");
		m_pSavePlatformCCR->setParam(3, "xxx3");
		m_pSavePlatformCCR->setParam(4, "xxx4");
		m_pSavePlatformCCR->setParam(5, "xxx5");
		m_pSavePlatformCCR->setParam(6, "xxx6");
	
		m_pSavePlatformCCR->Execute();
         	__DEBUG_LOG0_(0, "Insert table:[PLATFORM_CCR] ok");
		m_pSavePlatformCCR->Close();
		__DEBUG_LOG0_(0, "�����Է���ƽ̨������CCR��Ϣ");	
		return 0;
	}
	catch (TTStatus oSt)
	{
		ADD_EXCEPT1(oExp2, "DccOperationSql::savePlatformCCR oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG0_(0, "�����Է���ƽ̨����CCRʧ��" );
	return -1;
}

// �����Է���ƽ̨�����CCRҵ����Ϣ
int DccOperationSql::savePlatformCCRBiz(char *sSessionId,ReqPlatformSvc *pSvcIn,ReqPlatformPayInfo *pSvcInBiz)
{
	try 
	{
		/*
	 	m_pSavePlatformCCRBiz->setParam(1, "ABM2@001.ChinaTelecom;xxxxx");
		m_pSavePlatformCCRBiz->setParam(2, "request-001");
		m_pSavePlatformCCRBiz->setParam(3, "189000000");
		m_pSavePlatformCCRBiz->setParam(4, 1);
		m_pSavePlatformCCRBiz->setParam(5, 2);
		m_pSavePlatformCCRBiz->setParam(6, "ABM2");
		m_pSavePlatformCCRBiz->setParam(7, "1234567");
		m_pSavePlatformCCRBiz->setParam(8, 1);
		m_pSavePlatformCCRBiz->setParam(9, 2);
		m_pSavePlatformCCRBiz->setParam(10, 2);
		*/
		m_pSavePlatformCCRBiz->setParam(1, sSessionId);
		m_pSavePlatformCCRBiz->setParam(2, pSvcIn->m_requestId);
		m_pSavePlatformCCRBiz->setParam(3, pSvcInBiz->m_sDestinationAccount);
		m_pSavePlatformCCRBiz->setParam(4, pSvcInBiz->m_iDestinationType);
		m_pSavePlatformCCRBiz->setParam(5, pSvcInBiz->m_iDestinationAttr);
		m_pSavePlatformCCRBiz->setParam(6, pSvcInBiz->m_sSvcPlatformId);
		m_pSavePlatformCCRBiz->setParam(7, pSvcInBiz->m_sPayPassword);
		m_pSavePlatformCCRBiz->setParam(8, pSvcInBiz->m_iBalanceTransFlag);
		m_pSavePlatformCCRBiz->setParam(9, pSvcInBiz->m_iUnitTypeId);
		m_pSavePlatformCCRBiz->setParam(10,pSvcInBiz->m_lDeductAmount);
		
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��sSessionId:[%s]",sSessionId);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_requestId:[%s]",pSvcIn->m_requestId);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_sDestinationAccount:[%s]",pSvcInBiz->m_sDestinationAccount);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_iDestinationType:[%d]",pSvcInBiz->m_iDestinationType);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_iDestinationAttr:[%d]",pSvcInBiz->m_iDestinationAttr);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_sSvcPlatformId:[%s]",pSvcInBiz->m_sSvcPlatformId);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_sPayPassword:[%s]",pSvcInBiz->m_sPayPassword);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_iBalanceTransFlag:[%d]",pSvcInBiz->m_iBalanceTransFlag);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_iUnitTypeId:[%d]",pSvcInBiz->m_iUnitTypeId);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_lDeductAmount:[%ld]",pSvcInBiz->m_lDeductAmount);
		
		
		m_pSavePlatformCCRBiz->Execute();
		
         	__DEBUG_LOG0_(0, "Insert table:[savePlatformCCRBiz] ok");
		m_pSavePlatformCCRBiz->Close();
		__DEBUG_LOG0_(0, "�����Է���ƽ̨������CCRҵ����Ϣ�ɹ�");	
		return 0;
	}
	catch (TTStatus oSt)
	{
		ADD_EXCEPT1(oExp2, "DccOperationSql::savePlatformCCRBiz oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG0_(0, "�����Է���ƽ̨����CCRҵ����Ϣʧ��" );
	return -1;
}

// ��ȡ�Է���ƽ̨�����CCRҵ����Ϣ
int DccOperationSql::getCCRInfo(char *sSessionId,ReqPlatformSvc &reqSvc,ReqPlatformPayInfo &reqPayInfo)
{
	
	__DEBUG_LOG1_(0, "�ỰIDxxx��sSessionId:[%s]",sSessionId);
	string sql;
	int iRet = 1;
	try {
		sql.clear();
		sql="  SELECT SESSION_ID,";
		sql+="	      REQUEST_ID,";
		sql+="        TO_CHAR(REQUEST_DATE,'YYYYMMDDHH24MISS'),";
		sql+="        DEST_ACCOUNT,";
		sql+="        DEST_TYPE,";
		sql+="        DEST_ATTR,";
		sql+="        SVC_PLATFORM_ID,";
		sql+="        PAY_PASSWORD,";
		sql+="        BALANCE_TRANS_FLAG,";
		sql+="        UNIT_TYPE_ID,";
		sql+="        DEDUCT_AMOUNT";
		sql+=" FROM PLATFORM_CCRBIZ_INFO WHERE SESSION_ID=:SESSION_ID ";
			    
		m_poPublicConn->Prepare(sql.c_str());

		m_poPublicConn->setParam(1,sSessionId);
		
		m_poPublicConn->Execute();

		while (!m_poPublicConn->FetchNext()) 
		{
			__DEBUG_LOG0_(0, "=========>SQL��ѯCCR��ϸ:");	
			m_poPublicConn->getColumn(2, reqSvc.m_requestId);
			__DEBUG_LOG1_(0, "������ˮ��m_requestId:[%s]",reqSvc.m_requestId);
			m_poPublicConn->getColumn(3, reqSvc.m_requestTime);
			__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime:[%s]",reqSvc.m_requestTime);
			m_poPublicConn->getColumn(4, reqPayInfo.m_sDestinationAccount);
			__DEBUG_LOG1_(0, "�û����룺m_sDestinationAccount:[%s]",reqPayInfo.m_sDestinationAccount);
			m_poPublicConn->getColumn(5, &reqPayInfo.m_iDestinationType);
			__DEBUG_LOG1_(0, "��ʶ���ͣ�m_iDestinationType:[%d]",reqPayInfo.m_iDestinationType);
			m_poPublicConn->getColumn(6, &reqPayInfo.m_iDestinationAttr);
			__DEBUG_LOG1_(0, "�û����ԣ�m_iDestinationAttr:[%d]",reqPayInfo.m_iDestinationAttr);
			m_poPublicConn->getColumn(7, reqPayInfo.m_sSvcPlatformId);
			__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��m_sSvcPlatformId:[%s]",reqPayInfo.m_sSvcPlatformId);
			m_poPublicConn->getColumn(8, reqPayInfo.m_sPayPassword);
			__DEBUG_LOG1_(0, "֧�����룺m_sPayPassword:[%s]",reqPayInfo.m_sPayPassword);
			m_poPublicConn->getColumn(9, &reqPayInfo.m_iBalanceTransFlag);
			__DEBUG_LOG1_(0, "�������ͣ�m_iBalanceTransFlag:[%d]",reqPayInfo.m_iBalanceTransFlag);
			m_poPublicConn->getColumn(10, &reqPayInfo.m_iUnitTypeId);
			__DEBUG_LOG1_(0, "���ĵ�λ���ͣ�m_iUnitTypeId:[%d]",reqPayInfo.m_iUnitTypeId);
			m_poPublicConn->getColumn(11, &reqPayInfo.m_lDeductAmount);
			__DEBUG_LOG1_(0, "�軮����m_lDeductAmount:[%ld]",reqPayInfo.m_lDeductAmount);
			
			iRet = 0;
		}
		m_poPublicConn->Close();
		
		__DEBUG_LOG1_(0, "========>DccOperationSql::getCCRInfo success,iRet:[%d]",iRet);
		return iRet;
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp2, "DccOperationSql::QueryPaySeq oSt.err_msg=%s", oSt.err_msg);
	}
	return -1;
}

// ��ȡ�û�����������������
int DccOperationSql::qryAccountOrgId(char *accountNbr,char *orgId,char *localAreaId)
{
	string sql;
	int iRet = 1;
	try {
		sql.clear();
		sql="  SELECT A.ORG_ID ,A.LOCAL_AREA_ID";
		sql+=" FROM AREA_ORG A,APP_USER_INFO B,SERV C";
		sql+=" WHERE C.SERV_ID = B.SERV_ID ";
		sql+=" AND B.LOCAL_AREA_ID = A.LOCAL_AREA_ID ";
		sql+=" AND C.ACC_NBR=:ACC_NBR ";
			    
		m_poPublicConn->Prepare(sql.c_str());

		m_poPublicConn->setParam(1,accountNbr);
		
		m_poPublicConn->Execute();

		while (!m_poPublicConn->FetchNext()) 
		{
			m_poPublicConn->getColumn(1, orgId);
			__DEBUG_LOG1_(0, "�û����������������룺orgId:[%s]",orgId);
			m_poPublicConn->getColumn(2, localAreaId);
			__DEBUG_LOG1_(0, "�û�������������areaCode:[%s]",localAreaId);
			iRet = 0;
		}
		m_poPublicConn->Close();
		
		return iRet;
	}
	catch (TTStatus oSt) 
	{
		iRet = -1;
		ADD_EXCEPT1(oExp2, "DccOperationSql::qryAccountOrgId oSt.err_msg=%s", oSt.err_msg);
	}
	return iRet;
}

// DCC_INFO_RECORD_SEQ
int DccOperationSql::QueryDccRecordSeq(long &lRecordIdSeq)
{
	string sql;
	try {
		sql.clear();
		//��ѯ DCC_INFO_RECORD_SEQ

		sql="SELECT DCC_INFO_RECORD_SEQ.NEXTVAL FROM DUAL ";
				    
		m_poPublicConn->Prepare(sql.c_str());

		m_poPublicConn->Execute();
		while (!m_poPublicConn->FetchNext()) 
		{
		
			m_poPublicConn->getColumn(1, &lRecordIdSeq);
		}
		m_poPublicConn->Close();
		return 0;
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp2, "DccOperationSql::QueryDccRecordSeq oSt.err_msg=%s", oSt.err_msg);
	}
	return -1;
}

// �����շ���DCC��Ϣͷ��Ϣ
int DccOperationSql::insertDccInfo(struDccHead &dccHead,char *dccType,long &lRecordIdSeq)
{
	int iRet = 0;
	try 
	{
		__DEBUG_LOG0_(0, "SQL��丳ֵ............");	

		m_pSaveDcc->setParam(1, lRecordIdSeq);
		m_pSaveDcc->setParam(2, dccHead.m_sSessionId);
		m_pSaveDcc->setParam(3, dccHead.m_sOrignHost);
		m_pSaveDcc->setParam(4, dccHead.m_sOrignRealm);
		m_pSaveDcc->setParam(5, dccHead.m_sDestRealm);
		m_pSaveDcc->setParam(6, dccHead.m_sSrvTextId);
		m_pSaveDcc->setParam(7, dccHead.m_sSrvFlowId);
		m_pSaveDcc->setParam(8, dccType);

		strcpy(dccHead.m_sSrvFlowId,"xx");

		__DEBUG_LOG1_(0, "SESSION_ID:[%s]",dccHead.m_sSessionId);
		__DEBUG_LOG1_(0, "m_sOrignHost:[%s]",dccHead.m_sOrignHost);
		__DEBUG_LOG1_(0, "m_sOrignRealm:[%s]",dccHead.m_sOrignRealm);
		__DEBUG_LOG1_(0, "m_sDestRealm:[%s]",dccHead.m_sDestRealm);
		__DEBUG_LOG1_(0, "m_sSrvTextId:[%s]",dccHead.m_sSrvTextId);
		__DEBUG_LOG1_(0, "m_sSrvFlowId:[%s]",dccHead.m_sSrvFlowId);
		__DEBUG_LOG1_(0, "dccType:[%s]",dccType);

		m_pSaveDcc->Execute();
         	__DEBUG_LOG0_(0, "Insert table:[DCC_INFO_RECORD] ok");
         	
		m_pSaveDcc->Close();
		
		__DEBUG_LOG0_(0, "����DCC��Ϣ�ɹ�");	
		return iRet;
	}
	catch (TTStatus oSt)
	{
		iRet = -1;
		ADD_EXCEPT1(oExp2, "DccOperationSql::savePlatformCCR oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG0_(0, "����DCC��Ϣʧ��" );
	return iRet;
}

// �����շ���DCC��Ϣͷ��Ϣ-��������ʱ���豣��Դ����Ự��Session-id
int DccOperationSql::insertDccInfo(struDccHead &dccHead,char *dccType,long &lRecordIdSeq,char *sPreSessionId)
{
	int iRet = 0;
	try 
	{
		__DEBUG_LOG0_(0, "SQL��丳ֵ............");	

		m_pSaveDcc->setParam(1, lRecordIdSeq);
		m_pSaveDcc->setParam(2, dccHead.m_sSessionId);
		m_pSaveDcc->setParam(3, dccHead.m_sOrignHost);
		m_pSaveDcc->setParam(4, dccHead.m_sOrignRealm);
		m_pSaveDcc->setParam(5, dccHead.m_sDestRealm);
		m_pSaveDcc->setParam(6, dccHead.m_sSrvTextId);
		m_pSaveDcc->setParam(7, dccHead.m_sSrvFlowId);
		m_pSaveDcc->setParam(8, dccType);
		m_pSaveDcc->setParam(9, sPreSessionId);

		__DEBUG_LOG1_(0, "SESSION_ID:[%s]",dccHead.m_sSessionId);
		__DEBUG_LOG1_(0, "m_sOrignHost:[%s]",dccHead.m_sOrignHost);
		__DEBUG_LOG1_(0, "m_sOrignRealm:[%s]",dccHead.m_sOrignRealm);
		__DEBUG_LOG1_(0, "m_sDestRealm:[%s]",dccHead.m_sDestRealm);
		__DEBUG_LOG1_(0, "m_sSrvTextId:[%s]",dccHead.m_sSrvTextId);
		__DEBUG_LOG1_(0, "m_sSrvFlowId:[%s]",dccHead.m_sSrvFlowId);
		__DEBUG_LOG1_(0, "dccType:[%s]",dccType);
		__DEBUG_LOG1_(0, "preSessionId:[%s]",sPreSessionId);

		m_pSaveDcc->Execute();
         	__DEBUG_LOG0_(0, "Insert table:[DCC_INFO_RECORD] ok");
         	
		m_pSaveDcc->Close();
		
		__DEBUG_LOG0_(0, "����DCC��Ϣ�ɹ�");	
		return iRet;
	}
	catch (TTStatus oSt)
	{
		iRet = -1;
		ADD_EXCEPT1(oExp2, "DccOperationSql::savePlatformCCR oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG0_(0, "����DCC��Ϣʧ��" );
	return iRet;
}

// ������ӦSESSION_ID ����ԴSESSION_ID
int DccOperationSql::qryPreSessionId(char *sSessionId,char *sPreSessionId)
{
	__DEBUG_LOG0_(0, "===========>DccOperationSql::qryPreSessionId" );
	string sql;
	int iRet = 1;
	try {
		sql.clear();
		//��ѯ DCC_INFO_RECORD,��ӦCCA,ǰһ��
		sql="SELECT PRE_SESSION_ID FROM DCC_INFO_RECORD WHERE SESSION_ID=:SESSION_ID ";
				    
		m_poPublicConn->Prepare(sql.c_str());
		__DEBUG_LOG1_(0, "=======xx====>sSessionId:[%s]",sSessionId);
		m_poPublicConn->setParam(1,sSessionId);
		__DEBUG_LOG0_(0, "===========>DccOperationSql::qryPreSessionId---1" );
		m_poPublicConn->Execute();
		__DEBUG_LOG0_(0, "===========>DccOperationSql::qryPreSessionId-----2" );
		while (!m_poPublicConn->FetchNext()) 
		{
			m_poPublicConn->getColumn(1,sPreSessionId);
			__DEBUG_LOG1_(0, "DccOperationSql::qryPreSessionId,PRE_SESSION_ID=[%s]",sPreSessionId );
			iRet = 0;
		}
		m_poPublicConn->Close();
		return iRet;
	}
	catch (TTStatus oSt) 
	{
		iRet = -1;
		ADD_EXCEPT1(oExp2, "DccOperationSql::qryPreSessionId oSt.err_msg=%s", oSt.err_msg);
	}
	return iRet;
}

//��½�����Ȩ�¼�
int DccOperationSql::savePlatformCCRforIdentify(char *sSessionId,LoginPasswdStruct *pStruct,LoginPasswdCond *pCond)
{
	try
	{
		m_pLoginSaveDcc->setParam(1, sSessionId);
		m_pLoginSaveDcc->setParam(2, pStruct->m_sReqID);
		m_pLoginSaveDcc->setParam(3, pCond->m_sDestAccount);
		m_pLoginSaveDcc->setParam(4, pCond->m_hDestType);
		m_pLoginSaveDcc->setParam(5, pCond->m_hDestAttr);
		m_pLoginSaveDcc->setParam(6, pCond->m_sServPlatID);
		m_pLoginSaveDcc->setParam(7, pCond->m_sLoginPasswd);
		
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��sSessionId:[%s]",sSessionId);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_requestId:[%s]",pStruct->m_sReqID);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_sDestinationAccount:[%s]",pCond->m_sDestAccount);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_iDestinationType:[%d]",pCond->m_hDestType);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_iDestinationAttr:[%d]",pCond->m_hDestAttr);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_sSvcPlatformId:[%s]",pCond->m_sServPlatID);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_sPayPassword:[%s]",pCond->m_sLoginPasswd);
		m_pLoginSaveDcc->Execute();
		
         	__DEBUG_LOG0_(0, "Insert table:[savePlatformCCRBiz] ok");
		m_pLoginSaveDcc->Close();
		__DEBUG_LOG0_(0, "�����Է���ƽ̨������CCR��Ϣ");	
		return 0;
	}
	catch (TTStatus oSt)
	{
		ADD_EXCEPT1(oExp2, "DccOperationSql::savePlatformCCRBiz oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG0_(0, "�����Է���ƽ̨����CCRʧ��" );
	return -1;
}

//��ȡ�Է���ƽ̨�����CCRҵ����Ϣ-�����Ȩ��
int DccOperationSql::getLoginIdentifyCCRInfo(char *sSessionId,LoginPasswdStruct &pStruct,LoginPasswdCond &pCond)
{
	__DEBUG_LOG1_(0, "�ỰIDxxx��sSessionId:[%s]",sSessionId);
	string sql;
	int iRet = 1;
	try {
		sql.clear();
		sql="  SELECT SESSION_ID,";
		sql+="	      REQUEST_ID,";
		sql+="        TO_CHAR(REQUEST_DATE,'YYYYMMDDHH24MISS'),";
		sql+="        DEST_ACCOUNT,";
		sql+="        DEST_TYPE,";
		sql+="        DEST_ATTR,";
		sql+="        SVC_PLATFORM_ID,";
		sql+="        LOGIN_PASSWORD";
		sql+=" FROM PLATFORM_LOGINIDENTIFY_INFO WHERE SESSION_ID=:SESSION_ID ";
			    
		m_poPublicConn->Prepare(sql.c_str());

		m_poPublicConn->setParam(1,sSessionId);
		
		m_poPublicConn->Execute();

		while (!m_poPublicConn->FetchNext()) 
		{
			__DEBUG_LOG0_(0, "=========>SQL��ѯCCR��ϸ:");	
			m_poPublicConn->getColumn(2, pStruct.m_sReqID);
			__DEBUG_LOG1_(0, "������ˮ��m_sReqID:[%s]",pStruct.m_sReqID);
			m_poPublicConn->getColumn(3, pStruct.m_requestTime);
			__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime:[%s]",pStruct.m_requestTime);
			m_poPublicConn->getColumn(4, pCond.m_sDestAccount);
			__DEBUG_LOG1_(0, "�û����룺m_sDestAccount:[%s]",pCond.m_sDestAccount);
			m_poPublicConn->getColumn(5, &pCond.m_hDestType);
			__DEBUG_LOG1_(0, "��ʶ���ͣ�m_hDestType:[%d]",pCond.m_hDestType);
			m_poPublicConn->getColumn(6, &pCond.m_hDestAttr);
			__DEBUG_LOG1_(0, "�û����ԣ�m_hDestAttr:[%d]",pCond.m_hDestAttr);
			m_poPublicConn->getColumn(7, pCond.m_sServPlatID);
			__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��m_sServPlatID:[%s]",pCond.m_sServPlatID);
			m_poPublicConn->getColumn(8, pCond.m_sLoginPasswd);
			__DEBUG_LOG1_(0, "��½���룺m_sLoginPasswd:[%s]",pCond.m_sLoginPasswd);
			
			iRet = 0;
		}
		m_poPublicConn->Close();
		
		__DEBUG_LOG1_(0, "========>DccOperationSql::getCCRInfo success,iRet:[%d]",iRet);
		return iRet;
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp2, "DccOperationSql::QueryPaySeq oSt.err_msg=%s", oSt.err_msg);
	}
	return -1;
}

//��½�����޸��¼�
int DccOperationSql::savePlatformCCRforChange(char *sSessionId,PasswdChangeStruct *pStruct,PasswdChangeCond *pCond)
{
	try
	{
		m_pPasswdChangeDcc->setParam(1, sSessionId);
		m_pPasswdChangeDcc->setParam(2, pStruct->m_sReqID);
		m_pPasswdChangeDcc->setParam(3, pCond->m_sDestAccount);
		m_pPasswdChangeDcc->setParam(4, pCond->m_hDestType);
		m_pPasswdChangeDcc->setParam(5, pCond->m_hDestAttr);
		m_pPasswdChangeDcc->setParam(6, pCond->m_sServPlatID);
		m_pPasswdChangeDcc->setParam(7, pCond->m_sOldPasswd);
		m_pPasswdChangeDcc->setParam(8, pCond->m_sNewPasswd);
		
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��sSessionId:[%s]",sSessionId);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_requestId:[%s]",pStruct->m_sReqID);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_sDestinationAccount:[%s]",pCond->m_sDestAccount);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_iDestinationType:[%d]",pCond->m_hDestType);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_iDestinationAttr:[%d]",pCond->m_hDestAttr);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_sSvcPlatformId:[%s]",pCond->m_sServPlatID);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_sOldPassword:[%s]",pCond->m_sOldPasswd);
		__DEBUG_LOG1_(0, "[�Է���ƽ̨CCR-ҵ����Ϣ]��m_sNewPassword:[%s]",pCond->m_sNewPasswd);
		m_pPasswdChangeDcc->Execute();
		
         	__DEBUG_LOG0_(0, "Insert table:[savePlatformCCRforChange] ok");
		m_pPasswdChangeDcc->Close();
		__DEBUG_LOG0_(0, "�����Է���ƽ̨������CCR��Ϣ");	
		return 0;
	}
	catch (TTStatus oSt)
	{
		ADD_EXCEPT1(oExp2, "DccOperationSql::savePlatformCCRforChange oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG0_(0, "�����Է���ƽ̨����CCRʧ��" );
	return -1;
}

//��ȡ�Է���ƽ̨�����CCRҵ����Ϣ-�����޸���
int DccOperationSql::getPasswdChangeInfo(char *sSessionId,PasswdChangeStruct &pStruct,PasswdChangeCond &pCond)
{
	__DEBUG_LOG1_(0, "�ỰIDxxx��sSessionId:[%s]",sSessionId);
	string sql;
	int iRet = 1;
	try {
		sql.clear();
		sql="  SELECT SESSION_ID,";
		sql+="	      REQUEST_ID,";
		sql+="        TO_CHAR(REQUEST_DATE,'YYYYMMDDHH24MISS'),";
		sql+="        DEST_ACCOUNT,";
		sql+="        DEST_TYPE,";
		sql+="        DEST_ATTR,";
		sql+="        SVC_PLATFORM_ID,";
		sql+="        OLD_PASSWORD,";
		sql+="        NEW_PASSWORD";
		sql+=" FROM PLATFORM_PASSWDCHANGE_INFO WHERE SESSION_ID=:SESSION_ID ";
			    
		m_poPublicConn->Prepare(sql.c_str());

		m_poPublicConn->setParam(1,sSessionId);
		
		m_poPublicConn->Execute();

		while (!m_poPublicConn->FetchNext()) 
		{
			__DEBUG_LOG0_(0, "=========>SQL��ѯCCR��ϸ:");	
			m_poPublicConn->getColumn(2, pStruct.m_sReqID);
			__DEBUG_LOG1_(0, "������ˮ��m_sReqID:[%s]",pStruct.m_sReqID);
			m_poPublicConn->getColumn(3, pStruct.m_requestTime);
			__DEBUG_LOG1_(0, "����ʱ�䣺m_requestTime:[%s]",pStruct.m_requestTime);
			m_poPublicConn->getColumn(4, pCond.m_sDestAccount);
			__DEBUG_LOG1_(0, "�û����룺m_sDestAccount:[%s]",pCond.m_sDestAccount);
			m_poPublicConn->getColumn(5, &pCond.m_hDestType);
			__DEBUG_LOG1_(0, "��ʶ���ͣ�m_hDestType:[%d]",pCond.m_hDestType);
			m_poPublicConn->getColumn(6, &pCond.m_hDestAttr);
			__DEBUG_LOG1_(0, "�û����ԣ�m_hDestAttr:[%d]",pCond.m_hDestAttr);
			m_poPublicConn->getColumn(7, pCond.m_sServPlatID);
			__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��m_sServPlatID:[%s]",pCond.m_sServPlatID);
			m_poPublicConn->getColumn(8, pCond.m_sOldPasswd);
			__DEBUG_LOG1_(0, "��½���룺m_sLoginPasswd:[%s]",pCond.m_sOldPasswd);
			m_poPublicConn->getColumn(9, pCond.m_sNewPasswd);
			__DEBUG_LOG1_(0, "��½���룺m_sLoginPasswd:[%s]",pCond.m_sNewPasswd);
			
			iRet = 0;
		}
		m_poPublicConn->Close();
		
		__DEBUG_LOG1_(0, "========>DccOperationSql::getCCRInfo success,iRet:[%d]",iRet);
		return iRet;
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp2, "DccOperationSql::getPasswdChangeInfo oSt.err_msg=%s", oSt.err_msg);
	}
	return -1;
}

int DccOperationSql::updateDccInfo(char *sCCASessionId)
{
	int iRet = 0;
	try 
	{
		__DEBUG_LOG0_(0, "SQL��丳ֵ............");	

		m_pUpdateDcc->setParam(1, sCCASessionId);

		__DEBUG_LOG1_(0, "sCCASessionId:[%s]",sCCASessionId);

		m_pUpdateDcc->Execute();
		__DEBUG_LOG0_(0, "Update table:[DCC_INFO_RECORD] ok");
		
		m_pUpdateDcc->Close();
		
		__DEBUG_LOG0_(0, "����DCC��Ϣ�ɹ�");	
		return iRet;
	}
	catch (TTStatus oSt)
	{
		iRet = -1;
		ADD_EXCEPT1(oExp2, "DccOperationSql::updateDccInfo oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG0_(0, "����DCC��Ϣʧ��" );
	return iRet;
}

// ����ACTIVE_CCR_LOG. Add 2011.7.14
int DccOperationSql::insertActiveCCR(char *sSessionId,char *sRequestId,char *sActionType,char *sOrgId)
{
	int iRet = 0;
	try 
	{
		__DEBUG_LOG0_(0, "ACTIVE_CCR_LOG,SQL��丳ֵ............");	

		m_pSaveActiveCCR->setParam(1, sSessionId);
		m_pSaveActiveCCR->setParam(2, sRequestId);
		m_pSaveActiveCCR->setParam(3, sActionType);
		m_pSaveActiveCCR->setParam(4, sOrgId);

		__DEBUG_LOG1_(0, "SESSION_ID:[%s]",sSessionId);
		__DEBUG_LOG1_(0, "sRequestId:[%s]",sRequestId);
		__DEBUG_LOG1_(0, "sActionType:[%s]",sActionType);
		__DEBUG_LOG1_(0, "sOrgId:[%s]",sOrgId);

		m_pSaveActiveCCR->Execute();
         	__DEBUG_LOG0_(0, "Insert table:[ACTIVE_CCR_LOG] ok");
         	
		m_pSaveActiveCCR->Close();
		
		__DEBUG_LOG0_(0, "����CCR-ACTIVE_CCR_LOG��Ϣ�ɹ�");	
		return iRet;
	}
	catch (TTStatus oSt)
	{
		iRet = -1;
		ADD_EXCEPT1(oExp2, "DccOperationSql::insertActiveCCR oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG0_(0, "����CCR-ACTIVE_CCR_LOG��Ϣʧ��" );
	return iRet;
}

int DccOperationSql::updateActiveCCR(char *sCCARequestId)
{
	int iRet = 0;
	try 
	{
		__DEBUG_LOG0_(0, "SQL��丳ֵ............");	

		m_pUpdateCCR->setParam(1, sCCARequestId);

		__DEBUG_LOG1_(0, "sCCASessionId:[%s]",sCCARequestId);

		m_pUpdateCCR->Execute();
		__DEBUG_LOG0_(0, "Update table:[ACTIVE_CCR_LOG] ok");
		
		m_pUpdateCCR->Close();
		
		__DEBUG_LOG0_(0, "����DCC��Ϣ�ɹ�");	
		return iRet;
	}
	catch (TTStatus oSt)
	{
		iRet = -1;
		ADD_EXCEPT1(oExp2, "DccOperationSql::updateActiveCCR oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG0_(0, "����DCC��Ϣʧ��" );
	return iRet;
}