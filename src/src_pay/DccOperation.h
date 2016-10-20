#ifndef __DCC_OPERATION_H_INCLUDED_
#define __DCC_OPERATION_H_INCLUDED_

#include "abmcmd.h"
#include "dcc_ra.h"

class ABMException;
class DccOperationSql;


// ���յ��ͷ��͵�DCC����������
class DccOperation
{
	public:
		DccOperation();
		~DccOperation();
		
		int QueryPaySeq(long &lSessionIdSeq);
		int savePlatformCCR(PlatformDeductBalanceCCR *receiveCCR);	
		int savePlatformCCRBiz(char *sSessionId,ReqPlatformSvc *pSvcIn,ReqPlatformPayInfo *pSvcInBiz);
		int savePlatformCCRforIdentify(char *sSessionId,LoginPasswdStruct *pStruct,LoginPasswdCond *pCond); 	//��½�����Ȩ�¼Ӻ���
		int savePlatformCCRforChange(char *sSessionId,PasswdChangeStruct *pStruct,PasswdChangeCond *pCond);	//��½�����޸��¼Ӻ���
		
		int getCCRInfo(char *sSessionId,ReqPlatformSvc &reqSvc,ReqPlatformPayInfo &reqPayInfo);
		int getLoginIdentifyCCRInfo(char *sSessionId,LoginPasswdStruct &pStruct,LoginPasswdCond &pCond);	//��½�����Ȩ�¼Ӻ���
		int getPasswdChangeInfo(char *sSessionId,PasswdChangeStruct &pStruct,PasswdChangeCond &pCond); //��½�����޸��¼Ӻ���
		
		int qryAccountOrgId(char *accountNbr,char *orgId,char *localAreaId);
		
		int QueryDccRecordSeq(long &lRecordIdSeq);

		int insertDccInfo(struDccHead &dccHead,char *dccType,long &lRecordIdSeq);
		
		int insertDccInfo(struDccHead &dccHead,char *dccType,long &lRecordIdSeq,char *preSessionId);
		
		int qryPreSessionId(char *sSessionId,char *sPreSessionId);
		
		int updateDccInfo(char *sCCASessionId);
		
		int insertActiveCCR(char *sSessionId,char *sRequestId,char *sActionType,char *sOrgId);
		
		int updateActiveCCR(char *sCCARequestId);
		
	private:	
		DccOperationSql *pDccSql;
	
};

#endif /* __DCC_OPERATION_H_INCLUDED_ */ 