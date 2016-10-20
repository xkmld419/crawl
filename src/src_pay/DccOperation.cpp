//DccOperation.cpp
#include "DccOperation.h"
#include "DccOperationSql.h"
ABMException oExp3;
DccOperation::DccOperation()
{
	pDccSql = new DccOperationSql;
}

DccOperation:: ~DccOperation()
{
	FreeObject(pDccSql);
}

// ��ѯSessionId Seq. ���������޸�һ��
int DccOperation::QueryPaySeq(long &lSessionIdSeq)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������");
	try
	{
		iRet = pDccSql->QueryPaySeq(lSessionIdSeq);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::QueryPaySeq oSt.err_msg=%s", oSt.err_msg);
	}

	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}

// ����ҵ��ƽ̨������CCR������Ϣ
int DccOperation::savePlatformCCR(PlatformDeductBalanceCCR *receiveCCR)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������");
	try
	{
		iRet = pDccSql->savePlatformCCR(receiveCCR);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::savePlatformCCR oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}	

// ����ҵ��ƽ̨����CCR�����ҵ����Ϣ
int DccOperation::savePlatformCCRBiz(char *sSessionId,ReqPlatformSvc *pSvcIn,ReqPlatformPayInfo *pSvcInBiz)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ��CCRҵ�����ݱ������");
	
	try
	{
		iRet = pDccSql->savePlatformCCRBiz(sSessionId,pSvcIn,pSvcInBiz);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::savePlatformCCRBiz oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ��CCRҵ�����ݱ����������");
	return iRet;
}

// ��ȡ�Է���ƽ̨��CCRҵ��������Ϣ
int DccOperation::getCCRInfo(char *sSessionId,ReqPlatformSvc &reqSvc,ReqPlatformPayInfo &reqPayInfo)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������");
	
	try
	{
		iRet = pDccSql->getCCRInfo(sSessionId,reqSvc,reqPayInfo);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::savePlatformCCRBiz oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG1_(0, "===>DccOperation::getCCRInfo,iRet=[%d]",iRet);
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}

//  ��ȡ�û�����������������
int DccOperation::qryAccountOrgId(char *accountNbr,char *orgId,char *localAreaId)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������");
	
	try
	{
		iRet = pDccSql->qryAccountOrgId(accountNbr,orgId,localAreaId);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::savePlatformCCRBiz oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}

int DccOperation::QueryDccRecordSeq(long &lRecordIdSeq)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "��ѯDCC_INFO_RECORD_SEQ����");
	try
	{
		iRet = pDccSql->QueryDccRecordSeq(lRecordIdSeq);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::QueryDccRecordSeq oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}

// �����շ���DCC��Ϣͷ��Ϣ
int DccOperation::insertDccInfo(struDccHead &dccHead,char *dccType,long &lRecordIdSeq)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������");
	try
	{
		iRet = pDccSql->insertDccInfo(dccHead,dccType,lRecordIdSeq);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::savePlatformCCRBiz oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}

// �����շ���DCC��Ϣͷ��Ϣ-��������ʱ������Դ����ػ���SessionId
int DccOperation::insertDccInfo(struDccHead &dccHead,char *dccType,long &lRecordIdSeq,char *sPreSessionId)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������");
	try
	{
		iRet = pDccSql->insertDccInfo(dccHead,dccType,lRecordIdSeq,sPreSessionId);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::savePlatformCCRBiz oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}

// ������ӦSESSION_ID ����ԴSESSION_ID
int DccOperation::qryPreSessionId(char *sSessionId,char *sPreSessionId)
{
	int iRet=1;
	// ����SQLִ��
	try
	{
		__DEBUG_LOG0_(0, "��ѯPRE_SESSION_ID����");
		iRet = pDccSql->qryPreSessionId(sSessionId,sPreSessionId);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::qryPreSessionId: oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG1_(0, "��ѯPRE_SESSION_ID��������,sPreSessionId=[%s]",sPreSessionId);
	__DEBUG_LOG1_(0, "��ѯPRE_SESSION_ID��������,iRet=[%d]",iRet);
	return iRet;
}

//�����Է���ƽ̨����CCR�����ҵ����Ϣ-��½�����Ȩ��
int DccOperation::savePlatformCCRforIdentify(char *sSessionId,LoginPasswdStruct *pStruct,LoginPasswdCond *pCond)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ��CCRҵ�����ݱ������");
	
	try
	{
		iRet = pDccSql->savePlatformCCRforIdentify(sSessionId,pStruct,pCond);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::savePlatformCCRforIdentify oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ��CCRҵ�����ݱ����������");
	return iRet;
}

//��ȡ�Է���ƽ̨��CCRҵ��������Ϣ-�����Ȩ����
int DccOperation::getLoginIdentifyCCRInfo(char *sSessionId,LoginPasswdStruct &pStruct,LoginPasswdCond &pCond)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������");
	
	try
	{
		iRet = pDccSql->getLoginIdentifyCCRInfo(sSessionId,pStruct,pCond);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::savePlatformCCRforIdentify oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG1_(0, "===>DccOperation::getLoginIdentifyCCRInfo,iRet=[%d]",iRet);
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}

//�����Է���ƽ̨����CCR�����ҵ����Ϣ-��½�����޸���
int DccOperation::savePlatformCCRforChange(char *sSessionId,PasswdChangeStruct *pStruct,PasswdChangeCond *pCond)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ��CCRҵ�����ݱ������");
	
	try
	{
		iRet = pDccSql->savePlatformCCRforChange(sSessionId,pStruct,pCond);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::savePlatformCCRforChange oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ��CCRҵ�����ݱ����������");
	return iRet;
}

//��ȡ�Է���ƽ̨��CCRҵ��������Ϣ-�����޸�����
int DccOperation::getPasswdChangeInfo(char *sSessionId,PasswdChangeStruct &pStruct,PasswdChangeCond &pCond)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������");
	
	try
	{
		iRet = pDccSql->getPasswdChangeInfo(sSessionId,pStruct,pCond);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::getPasswdChangeInfo oSt.err_msg=%s", oSt.err_msg);
	}
	__DEBUG_LOG1_(0, "===>DccOperation::getPasswdChangeInfo,iRet=[%d]",iRet);
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}

int DccOperation::updateDccInfo(char *sCCASessionId)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������");
	try
	{
		iRet = pDccSql->updateDccInfo(sCCASessionId);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::updateDccInfo oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}

// �����������͵�CCR-��������
int DccOperation::insertActiveCCR(char *sSessionId,char *sRequestId,char *sActionType,char *sOrgId)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������-����ACTIVE_CCR_LOG");
	try
	{
		iRet = pDccSql->insertActiveCCR(sSessionId,sRequestId,sActionType,sOrgId);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::insertActiveCCR oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������-����ACTIVE_CCR_LOG");
	return iRet;
}

int DccOperation::updateActiveCCR(char *sCCARequestId)
{
	int iRet=1;
	// ����SQLִ��
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ������-����ACTIVE_CCR_LOG");
	try
	{
		iRet = pDccSql->updateActiveCCR(sCCARequestId);
	}
	catch (TTStatus oSt) 
	{
		ADD_EXCEPT1(oExp3, "DccOperation::updateActiveCCR oSt.err_msg=%s", oSt.err_msg);
	}
	
	__DEBUG_LOG0_(0, "����SQLִ�����ݱ����������");
	return iRet;
}