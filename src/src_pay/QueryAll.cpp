#include "QueryAll.h"
#include "QueryAllSql.h"
#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "Date.h"
#include "abmobject.h"
#include <string.h>

static vector<QueryAllPay *> g_vAll;

extern bool g_toPlatformFlag;

QueryAll::QueryAll()
{
    m_poSql = NULL;
    
    m_poQueryAllCCR = NULL;
    
    m_poQueryAllCCA = NULL;
    
    m_poQueryAllSndCCR = NULL;

    m_iCount = 0;
    
    m_dccOperation = NULL;
    
}

QueryAll::~QueryAll()
{
    g_vAll.clear();
    #define __FREE_PTR_(p) if(p != NULL)\
        {\
            delete p;\
            p = NULL;\
        }
    __FREE_PTR_(m_poSql);
    __FREE_PTR_(m_dccOperation);

}

int QueryAll::init(ABMException &oExp)
{
    m_poSql = new QueryAllSql;

    if(m_poSql == NULL)
    {
        ADD_EXCEPT0(oExp, "�����ڴ�ʧ��"); 
        return -1;
    }

    return m_poSql->init(oExp);
    
    m_dccOperation = new DccOperation;
    
}
int QueryAll::deal(ABMCCR *poQueryMsg, ABMCCA *poQueryRlt)
{
    int iRet = 0;
    __DEBUG_LOG0_(0, "������ѯ����ʼ");
    poQueryRlt->memcpyh(poQueryMsg);
    memset((void *)&m_oRlt,0x00,sizeof(m_oRlt));
    memset((void *)&m_oCCRHead,0x00,sizeof(m_oCCRHead));
    
	char sDestRealm[32]={0};
    //��ȡDCC��Ϣͷ
	memcpy((void *)&m_oCCRHead,(void *)(poQueryMsg->m_sDccBuf),sizeof(m_oCCRHead));
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iAuthAppId=%d",m_oCCRHead.m_iAuthAppId);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iReqNumber=%d",m_oCCRHead.m_iReqNumber);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iReqType=%d",m_oCCRHead.m_iReqType);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iResultCode=%d",m_oCCRHead.m_iResultCode);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sDestRealm=%s",m_oCCRHead.m_sDestRealm);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sOrignHost=%s",m_oCCRHead.m_sOrignHost);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sOutPlatform=%s",m_oCCRHead.m_sOutPlatform);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sSessionId=%s",m_oCCRHead.m_sSessionId);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sSrvFlowId=%s",m_oCCRHead.m_sSrvFlowId);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sSrvTextId=%s",m_oCCRHead.m_sSrvTextId);
    
    
	// ��Ϣͷ�л�ȡĿ�ĵ�ַ
	//���Ŀ�ĵ�ַΪ������Ϊ��ʡ�ﷵ�ص�CCA�������Ϊ������Ϊ���Է���ƽ̨����������
	strcpy(sDestRealm,m_oCCRHead.m_sDestRealm);
    if (strlen(sDestRealm) != 0)
    {
    	__DEBUG_LOG0_(0, "�����Է���ƽ̨�����CCR,����CCR��ʡ��[dQueryAllCCR]");
        iRet = dQueryAllCCR(poQueryMsg, poQueryRlt);
    }
    else
    {
    	__DEBUG_LOG0_(0, "����ʡ���ص�CCA,����CCA���Է���ƽ̨.[dQueryAllCCA]");
        iRet = dQueryAllCCA(poQueryMsg, poQueryRlt);
    }
    if (iRet != 0)
    {
        HssResMicroPayCCA * tmp = NULL;
        tmp = (HssResMicroPayCCA *)poQueryRlt;
        m_oRlt.m_uiResTime = time(NULL);
        m_oRlt.m_uiRltCode = iRet;
        if (!tmp->addRlt(m_oRlt))
        {
            __DEBUG_LOG0_(0, "deal::deal ���������Ϣʧ��");
            tmp->clear();
        }
    }

    __DEBUG_LOG0_(0, "ҵ����ɹ�");
    return 0;
}

// ������ѯ-��������CCR
int QueryAll::dQueryAllCCR(ABMCCR * poQueryMsg, ABMCCA * poQueryRlt)
{
    __DEBUG_LOG0_(0, "�����Է���ƽ̨��CCR����.");
    int iRet;
    poQueryRlt->memcpyh(poQueryMsg);
    
    m_poQueryAllCCR = (HssPaymentQueryCCR *)poQueryMsg;
    m_poQueryAllCCA = (HssResMicroPayCCA *)poQueryRlt;
    m_poQueryAllSndCCR = (HssQueryBalanceCCR *)poQueryRlt;
    m_poQueryAllSndCCR->clear();
    m_poQueryAllCCA->clear();
    g_vAll.clear();

    vector<QueryAllPay *>::iterator itvAll;

    //��Ϣ���
    if ((iRet=m_poQueryAllCCR->getReq(g_vAll)) == 0) {
        __DEBUG_LOG0_(0, "getReq::deal ��Ϣ��ΪNULL");
        m_poQueryAllCCA->setRltCode(ECODE_NOMSG);
        return QRY_ECODE_NOMSG_ERR;
    }
    else if (iRet < 0) {
        __DEBUG_LOG0_(0, "getReqBal::deal ��Ϣ�������!");
        m_poQueryAllCCA->setRltCode(ECODE_UNPACK_FAIL);
        return QRY_ECODE_UNPACK_FAIL;
    }
    if (iRet != 1){
        __DEBUG_LOG0_(0, "getReqBal::deal ��Ϣ�������������Ϣ�ж���!");
        m_poQueryAllCCA->setRltCode(ECODE_UNPACK_FAIL);
        return QRY_ECODE_UNPACK_FAIL;
    }
    
    try {
        for (itvAll=g_vAll.begin(); itvAll!=g_vAll.end(); ++itvAll) 
        {
            __DEBUG_LOG0_(0, "���û�����ѯҵ����.");
            iRet = _dQueryAll(*itvAll);
            return iRet;
        }
    }
    catch(TTStatus oSt) {
        __DEBUG_LOG1_(0, "getReqBal::deal oSt.err_msg=%s", oSt.err_msg);
    }
    
    m_poQueryAllCCA->clear();
    m_poQueryAllCCA->setRltCode(ECODE_TT_FAIL);
    
    return 0;

}

int QueryAll::_dQueryAll(QueryAllPay *pIn)
{
    int iRet = -1;
    __DEBUG_LOG0_(0, "_dQueryAll.��ʼ");
    MicroPaySrv tmpSrv;
    snprintf(m_oRlt.m_sResID, sizeof(m_sReqID),"%s",pIn->m_sReqID);
    vector<MicroPaySrv> vData;
    vData.clear();
    char sORG_ID[8];
    char strTmp[20];
    double dtmp;
    int iServPlatForm = 0;
    
    /*
    if (pIn->m_iDestType == 2)
    {
    	__DEBUG_LOG0_(0, "��ʶ���ͣ�1.ͨ��֤�˺ţ�Ĭ�ϣ���2.ҵ��ƽ̨�˺�");
        return QRY_USER_TYPE_ERR;
    }
    */
    __DEBUG_LOG1_(0, "��ӡ���ܵ���������Ϣm_iDestAttr=%d",pIn->m_iDestAttr);
    __DEBUG_LOG1_(0, "��ӡ���ܵ���������Ϣm_iDestType=%d",pIn->m_iDestType);
    __DEBUG_LOG1_(0, "��ӡ���ܵ���������Ϣm_iQueryFlg=%d",pIn->m_iQueryFlg);
    __DEBUG_LOG1_(0, "��ӡ���ܵ���������Ϣm_iQueryItemType=%d",pIn->m_iQueryItemType);
    __DEBUG_LOG1_(0, "��ӡ���ܵ���������Ϣm_sDestAcct=%s",pIn->m_sDestAcct);
    __DEBUG_LOG1_(0, "��ӡ���ܵ���������Ϣm_sReqID=%s",pIn->m_sReqID);// �Է���ƽ̨����ID
    __DEBUG_LOG1_(0, "��ӡ���ܵ���������Ϣm_sServPlatID=%s",pIn->m_sServPlatID);
    __DEBUG_LOG1_(0, "��ӡ���ܵ���������Ϣm_uiReqTime=%d",pIn->m_uiReqTime);
    
    // add 2011.7.19
    if(pIn->m_iDestType == 2)
    {
    	// �������û��ʺ�,����ѯ֧���ʻ����,�����Է���ƽ̨	
    	iRet = _dQueryCenterABM(pIn);
    	return iRet;
    }

    TimesTenCMD *pTmp = m_poSql->m_poQueryAll;
    pTmp->setParam(1, pIn->m_sDestAcct);

    //ɾ��ƥ���û����ͺ��û�����
    //snprintf(strTmp, sizeof(strTmp), "%d\0", pIn->m_iDestType);
    //pTmp->setParam(2, strTmp);
    //snprintf(strTmp, sizeof(strTmp), "%d\0", pIn->m_iDestAttr);
    //pTmp->setParam(3, strTmp);
    //pTmp->setParam(4, pIn->m_iQueryFlg);
    //ȥ��ƽ̨ID
    //sscanf(pIn->m_sServPlatID,"%d",&iServPlatForm);
    //pTmp->setParam(2, iServPlatForm);
    pTmp->Execute();
    while (!pTmp->FetchNext()) {
        memset((void *)&tmpSrv, 0x00, sizeof(tmpSrv));
        tmpSrv.m_iBalSrc = 2;
        pTmp->getColumn(1, &tmpSrv.m_iUnitTypID);
        pTmp->getColumn(2, &tmpSrv.m_lBalAmount);
        pTmp->getColumn(3, sORG_ID);
        
        vData.push_back(tmpSrv);
    }
    pTmp->Close();
    
    if (vData.size() == 0 )
    {
        __DEBUG_LOG1_(0, "_dQueryAll:: _deal  ��ѯ�޼�¼!err_code = %d",QRY_BALANCE_NODATA_ERR);
        return QRY_BALANCE_NODATA_ERR;
    }
    if (vData.size() != 1)
    {
        __DEBUG_LOG1_(0, "_dQueryAll:: _deal  ��ѯ������¼!err_code = %d",QRY_MUL_RECORD_ERR);
        return QRY_MUL_RECORD_ERR;
    }
    
    long lseq;
    if (m_poSql->GetSeq(lseq) != 0)
    {
        __DEBUG_LOG1_(0, "_dQueryAll:: _deal get seq failed !err_code=%d",QRY_GET_SEQUENCE_FAIL_ERR);
        return QRY_GET_SEQUENCE_FAIL_ERR;
    }
    
    //�����Է���ƽ̨��������ϢͷDCCͷ
    /*
    TimesTenCMD *pTmpInsDcc = m_poSql->m_poInsertDcc;
    try{
        pTmpInsDcc->setParam(1, m_oCCRHead.m_sSessionId);
    	pTmpInsDcc->setParam(2, m_oCCRHead.m_sOrignHost);
    	pTmpInsDcc->setParam(3, m_oCCRHead.m_sOrignRealm);
    	pTmpInsDcc->setParam(4, m_oCCRHead.m_sDestRealm);
    	pTmpInsDcc->setParam(5, m_oCCRHead.m_sSrvTextId);
    	pTmpInsDcc->setParam(6, m_oCCRHead.m_sSrvFlowId);
    	pTmpInsDcc->setParam(7, lseq);
    	pTmpInsDcc->Execute();
    	pTmpInsDcc->Commit();
    }
    catch (TTStatus oSt)
    {
        __DEBUG_LOG1_(0, "_dQueryAll:: _deal  oSt.err_msg=%s!",oSt.err_msg);
        __DEBUG_LOG1_(0, "_dQueryAll:: _deal  ����DCC��Ϣͷʧ��,err_code=%d!",QRY_INSERT_DCC_FAIL_ERR);
        return QRY_INSERT_DCC_FAIL_ERR;
    }
    __DEBUG_LOG0_(0, "_dQueryAll:: _deal  ����DCC��Ϣͷ���");
   */	
	
    // ����CCR��Ϣͷ����TT-add 2011.7.9
    	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	iRet = m_dccOperation->insertDccInfo(m_oCCRHead,dccType,lseq);
	if(iRet != 0)
	{
		__DEBUG_LOG0_(0, "[������ѯ-�����Է���ƽ̨CCR]:�����Է���ƽ̨CCR����Ϣͷ��Ϣʧ��");
		throw SAVE_PLATFOR_CCR_ERR;
	}	
	
	
    //����ȫ��ABM��ѯ������
    TimesTenCMD *pTmpInsMsg = m_poSql->m_poInsertMsg;
    try{
        
    	pTmpInsMsg->setParam(1, m_oCCRHead.m_sSessionId);
    	pTmpInsMsg->setParam(2, tmpSrv.m_iUnitTypID);
    	pTmpInsMsg->setParam(3, tmpSrv.m_lBalAmount);
    	pTmpInsMsg->setParam(4, pIn->m_sReqID); // �����Է���ƽ̨��������ˮ��
    	pTmpInsMsg->Execute();
    	pTmpInsMsg->Commit();
    }
    catch (TTStatus oSt)
    {
        __DEBUG_LOG1_(0, "_dQueryAll:: _deal  oSt.err_msg=%s!",oSt.err_msg);
        __DEBUG_LOG1_(0, "_dQueryAll:: _deal  ����ȫ������ABM���ʧ��,err_code=%d!",QRY_SAVEABMDATA_FAIL_ERR);
        return QRY_SAVEABMDATA_FAIL_ERR;
    }
    __DEBUG_LOG0_(0, "_dQueryAll:: _deal  ����ȫ������ҵ���������");
    //������Ϣͷ
    struct struDccHead Head;
    memset((void *)&Head,0x00,sizeof(Head));
    Date sessionid;
    // modify 2011.7.9
    // snprintf(Head.m_sSessionId,sizeof(Head.m_sSessionId),"%s",m_oCCRHead.m_sSessionId);
    // �鷢�͵�CCR session-id
    
    // ����Session-id
	long lSessionIdSeq;
	iRet = m_dccOperation->QueryPaySeq(lSessionIdSeq);
	if(iRet !=0)
	{
		__DEBUG_LOG0_(0, "��ѯDCC�Ự��ʶSession-Idʧ��");
		throw QRY_SESSION_ID_ERR;
	}
    
    	time_t tTime;
	tTime = time(NULL);
	// 2011.8.1
	snprintf(Head.m_sSessionId,sizeof(Head.m_sSessionId),"%s%010d%s%04d\0","ABM@001.ChinaTelecom.com;",tTime,";",lSessionIdSeq);
		
    snprintf(Head.m_sSrvTextId, sizeof(Head.m_sSrvTextId),"Query.Balance\@%s.ChinaTelecom.com",sORG_ID);
    snprintf(Head.m_sDestRealm, sizeof(Head.m_sDestRealm),"%s.ChinaTelecom.com",sORG_ID);
    snprintf(Head.m_sOrignHost,sizeof(Head.m_sOrignHost),"%s","ABM2\@001.ChinaTelecom.com");
    strcpy(Head.m_sOrignRealm,"001.ChinaTelecom.com");
    strcpy(Head.m_sSrvFlowId,"qryTest"); // ������ȡԴm_sSrvFlowId��׷��
    Head.m_iAuthAppId = 4;
    Head.m_iReqType = 4;
    
    __DEBUG_LOG0_(0, "_dQueryAll:: _deal  ת����Ϣ����Ϣͷ");
    __DEBUG_LOG1_(0, "[��������-�շ�CCR,������ѯ]:SESSION_ID:[%s]",Head.m_sSessionId);
    __DEBUG_LOG1_(0, "_dQueryAll:: ���͵�ʡ���m_sDestRealm=%s!",Head.m_sDestRealm);
    __DEBUG_LOG1_(0, "_dQueryAll:: ���͵�ʡ���m_sSessionId=%s!",Head.m_sSessionId);
    __DEBUG_LOG1_(0, "_dQueryAll:: ���͵�ʡ���m_sSrvTextId=%s!",Head.m_sSrvTextId);
    __DEBUG_LOG1_(0, "_dQueryAll:: ���͵�ʡ���m_sOrignHost=%s!",Head.m_sOrignHost);
    m_poQueryAllSndCCR->m_iCmdID = ABMCMD_QRY_BAL_INFOR;
    m_poQueryAllSndCCR->m_lType = 1;
    QueryBalance tmpQueryBalance;
    memset((void *)&tmpQueryBalance,0x00,sizeof(tmpQueryBalance));
    strcpy(tmpQueryBalance.m_sDestID,pIn->m_sDestAcct);
    tmpQueryBalance.m_iDestIDType = 2; //��д��.�û���������,0-�ͻ�ID,1-�û�ID,2-�û�����
    tmpQueryBalance.m_iDestAttr = pIn->m_iDestAttr;
    // ����
    char sOrgId[4];
    char sLocalAreaId[4];
    iRet = m_dccOperation->qryAccountOrgId(pIn->m_sDestAcct,sOrgId,sLocalAreaId);
    if(iRet != 0 )
    {
    	__DEBUG_LOG0_(0, "_dQueryAll:: _deal  ��ѯ��������ʧ��.");
    }
    strcpy(tmpQueryBalance.m_sAreaCode,sLocalAreaId);
    
    tmpQueryBalance.m_iQueryFlg = pIn->m_iQueryFlg;
    tmpQueryBalance.m_iQueryItemTyp = pIn->m_iQueryItemType;
    memcpy((void *)m_poQueryAllSndCCR->m_sDccBuf,(void *)&Head,sizeof(Head));
    m_poQueryAllSndCCR->clear();

    __DEBUG_LOG0_(0,"��ӡת����������Ϣ");
    __DEBUG_LOG1_(0,"��ӡת����������Ϣm_iDestAttr=%d",tmpQueryBalance.m_iDestAttr);
    __DEBUG_LOG1_(0,"��ӡת����������Ϣm_iDestIDType=%d",tmpQueryBalance.m_iDestIDType);
    __DEBUG_LOG1_(0,"��ӡת����������Ϣm_iQueryFlg=%d",tmpQueryBalance.m_iQueryFlg);
    __DEBUG_LOG1_(0,"��ӡת����������Ϣm_iQueryItemTyp=%d",tmpQueryBalance.m_iQueryItemTyp);
    __DEBUG_LOG1_(0,"��ӡת����������Ϣm_sAreaCode=%s",tmpQueryBalance.m_sAreaCode);
    __DEBUG_LOG1_(0,"��ӡת����������Ϣm_sDestID=%s",tmpQueryBalance.m_sDestID);
    __DEBUG_LOG1_(0,"��ӡת����������Ϣm_iCmdID=%d",m_poQueryAllSndCCR->m_iCmdID);
    if (!m_poQueryAllSndCCR->addReqBal(tmpQueryBalance)) 
    {
        __DEBUG_LOG1_(0, "_dQueryAll:: _deal m_poQueryAllSndCCR ������͵�ʡ�������ʧ��,err_code!",QRY_PACKMSG_TO_PROC_ERR);
        m_poQueryAllSndCCR->setRltCode(ECODE_PACK_FAIL);
        return QRY_PACKMSG_TO_PROC_ERR;
    }
    __DEBUG_LOG0_(0,"���������Ϣ�ɹ�");
    
    // add 2011.7.9
	// ȡDCC_INFO_RECORD_SEQ
	long lDccRecordSeq = 0L;
	strcpy(dccType,"R"); 
	
	iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
	
	// ���淢����CCR��Ϣ
	__DEBUG_LOG0_(0, "[������ѯ-�շ�CCR,������ѯ]:���淢����DCC����Ϣ��TT");
	// modify.���ӱ���ԴDCC�ỰSession-id
	iRet = m_dccOperation->insertDccInfo(Head,dccType,lDccRecordSeq,m_oCCRHead.m_sSessionId);
	if(iRet != 0)
	{
		__DEBUG_LOG0_(0, "[��������-�շ�CCR,������ѯ]:����DCCҵ����Ϣʧ��");
		throw SAVE_PLATFOR_CCR_BIZ_ERR;
	}
    
    return 0;
}  

// ���Ϊ�������û���ֻ��ѯ֧���˻��󷵻�
int QueryAll::_dQueryCenterABM(QueryAllPay *pIn)
{
    int iRet = -1;
    __DEBUG_LOG0_(0, "_dQueryCenterABM.�������û����Է���ƽ̨������ѯ");
    // �����Է���ƽ̨��CCA
    HssResMicroPayCCA * tmpRlt = NULL;
    tmpRlt = (HssResMicroPayCCA *)m_poQueryAllCCA;	
	
    
    MicroPaySrv tmpSrv;
    // ��Ӧ��ˮ
    snprintf(m_oRlt.m_sResID, sizeof(m_sReqID),"%s",pIn->m_sReqID);
    vector<MicroPaySrv> vData;
    vData.clear();
    char sORG_ID[8];
    char strTmp[20];
    double dtmp;
    int iServPlatForm = 0;

    __DEBUG_LOG1_(0, "��ӡ�Է���ƽ̨������Ϣm_iDestAttr=%d",pIn->m_iDestAttr);
    __DEBUG_LOG1_(0, "��ӡ�Է���ƽ̨������Ϣm_iDestType=%d",pIn->m_iDestType);
    __DEBUG_LOG1_(0, "��ӡ�Է���ƽ̨������Ϣm_iQueryFlg=%d",pIn->m_iQueryFlg);
    __DEBUG_LOG1_(0, "��ӡ�Է���ƽ̨������Ϣm_iQueryItemType=%d",pIn->m_iQueryItemType);
    __DEBUG_LOG1_(0, "��ӡ�Է���ƽ̨������Ϣm_sDestAcct=%s",pIn->m_sDestAcct);
    __DEBUG_LOG1_(0, "��ӡ�Է���ƽ̨������Ϣm_sReqID=%s",pIn->m_sReqID);// �Է���ƽ̨����ID
    __DEBUG_LOG1_(0, "��ӡ�Է���ƽ̨������Ϣm_sServPlatID=%s",pIn->m_sServPlatID);
    __DEBUG_LOG1_(0, "��ӡ�Է���ƽ̨������Ϣm_uiReqTime=%d",pIn->m_uiReqTime);
   
    TimesTenCMD *pTmp = m_poSql->m_poQueryAll;
    pTmp->setParam(1, pIn->m_sDestAcct);
    pTmp->Execute();
    while (!pTmp->FetchNext()) {
        memset((void *)&tmpSrv, 0x00, sizeof(tmpSrv));
        tmpSrv.m_iBalSrc = 2;
        pTmp->getColumn(1, &tmpSrv.m_iUnitTypID);
        pTmp->getColumn(2, &tmpSrv.m_lBalAmount);
        pTmp->getColumn(3, sORG_ID);
        
        vData.push_back(tmpSrv);
    }
    pTmp->Close();
    
    if (vData.size() == 0 )
    {
        __DEBUG_LOG1_(0, "_dQueryAll:: _deal  ��ѯ�޼�¼!err_code = %d",QRY_BALANCE_NODATA_ERR);
        return QRY_BALANCE_NODATA_ERR;
    }
    if (vData.size() != 1)
    {
        __DEBUG_LOG1_(0, "_dQueryAll:: _deal  ��ѯ������¼!err_code = %d",QRY_MUL_RECORD_ERR);
        return QRY_MUL_RECORD_ERR;
    }

    //======================
    Date cur;
    m_oRlt.m_uiRltCode = 0;
    m_oRlt.m_uiResTime = compute(cur.toString());
    // Modfiy 2011.7.11 ��Ӧ��ˮӦ��Ϊ�Է���ƽ̨��������ˮ
    strncpy(m_oRlt.m_sResID,pIn->m_sReqID,sizeof(m_oRlt.m_sResID));
    
    tmpRlt->m_iCmdID = ABMCMD_QRY_MICROPAY_INFO;
    __DEBUG_LOG0_(0, "��ӡ������Ӧ��Ϣ");
    __DEBUG_LOG1_(0, "��ӡ������Ӧ��Ϣ:������:m_uiRltCode=[%d]",m_oRlt.m_uiRltCode);
    __DEBUG_LOG1_(0, "��ӡ������Ӧ��Ϣm_sResID=%s",m_oRlt.m_sResID);
    __DEBUG_LOG1_(0, "��ӡ������Ӧ��Ϣ:��Ӧʱ��:m_uiResTime=[%d]",m_oRlt.m_uiResTime);
    __DEBUG_LOG1_(0, "��ӡ������Ӧ��Ϣ:m_sParaFieldResult=[%s]",m_oRlt.m_sParaFieldResult);
    __DEBUG_LOG1_(0, "��ӡ������Ӧ��Ϣ:m_iCmdID=[%d]",tmpRlt->m_iCmdID);
    iRet = tmpRlt->addRlt(m_oRlt);
    if (iRet != true)
    {
        __DEBUG_LOG0_(0, "������ص��Է���ƽ̨����Ϣ��������!");
        return QRY_PACK_SERVICE_FAIL_ERR;
    }
    __DEBUG_LOG0_(0, "���������ѯ��Ӧ�Ľ������ɡ�");

    __DEBUG_LOG0_(0, "���ȫ����������");
    __DEBUG_LOG1_(0, "���ȫ����������m_iBalSrc=%d",tmpSrv.m_iBalSrc);
    __DEBUG_LOG1_(0, "���ȫ����������m_iUnitTypID=%d",tmpSrv.m_iUnitTypID);
    __DEBUG_LOG1_(0, "���ȫ����������m_lBalAmount=%ld",tmpSrv.m_lBalAmount);
    iRet = tmpRlt->addSrv(tmpSrv);
    if (iRet != true)
    {
        __DEBUG_LOG0_(0, "������ص��Է���ƽ̨����Ϣȫ������������!");
        return QRY_PACK_SERVICE_FAIL_ERR;
    }

    // ���Է���ƽ̨��CCA
    g_toPlatformFlag = true;
    
    return 0;
}

// ������ѯ-����ʡ���ص�CCA������CCA���Է���ƽ̨
int QueryAll::dQueryAllCCA(ABMCCR * poQueryMsg, ABMCCA * poQueryRlt)
{
    __DEBUG_LOG0_(0, "����ʡ���ص�CCA��������CCA���Է���ƽ̨");
    int iRet;
    HssQueryBalanceCCA * tmp = NULL;
    HssResMicroPayCCA * tmpRlt = NULL;
    tmp = (HssQueryBalanceCCA *)poQueryMsg;
    tmpRlt = (HssResMicroPayCCA *)poQueryRlt;
    memset((void *)poQueryRlt, 0x00,sizeof(ABMCCA)); //���÷��ص��Է���ƽ̨����ϢΪ��
    vector<ResBalanceCode *> vPayRlt;
    vector<ResQueryBalance *> vPaySrv;
    MicroPaySrv tmpSrv;
    vPayRlt.clear();
    vPaySrv.clear();
    
    struct struDccHead Head;
    memset((void *)&Head,0x00,sizeof(Head));
    memcpy((void *)&Head, tmp->m_sDccBuf, sizeof(Head));
    __DEBUG_LOG0_(0, "��ӡ���յ���ʡ�����Ӧ����Ϣͷ");
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iAuthAppId=%d",Head.m_iAuthAppId);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iReqNumber=%d",Head.m_iReqNumber);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iReqType=%d",Head.m_iReqType);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iResultCode=%d",Head.m_iResultCode);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sDestRealm=%s",Head.m_sDestRealm);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sOrignHost=%s",Head.m_sOrignHost);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sOutPlatform=%s",Head.m_sOutPlatform);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sSessionId=%s",Head.m_sSessionId);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sSrvFlowId=%s",Head.m_sSrvFlowId);
    __DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sSrvTextId=%s",Head.m_sSrvTextId);
     
     char sSessionId[101]={0};
     struct struDccHead ttHead;
     memset((void *)&ttHead, 0x00, sizeof(ttHead));
     TimesTenCMD *pTmpGetDcc = m_poSql->m_poGetDcc;
     try
     {
	// add 2011.7.9
	// ����ʡ�ﷵ�ص�CCA-session-id,���Է���ƽ̨������session-id
	//����CCA��Ӧ��ˮȥ���Է���ƽ̨����CCR�ĻỰID
	
	iRet = m_dccOperation->qryPreSessionId(Head.m_sSessionId,sSessionId);
	if(iRet != 0)
	{
		__DEBUG_LOG0_(0, "��ѯCCA�ỰID��ԴSession Idʧ��.");
		return QRY_PRE_SESSION_ID_ERR;
	}
	__DEBUG_LOG1_(0, "========>�Է���ƽ̨����CCR��SESSION_ID:[%s]",sSessionId);

	//��ѯ���ݿ�,����session_id��ѯ���Է���ƽ̨��������Ϣͷ
        pTmpGetDcc->setParam(1, Head.m_sSessionId);
        pTmpGetDcc->Execute();
        if(!pTmpGetDcc->FetchNext()) 
        {
            pTmpGetDcc->getColumn(1, ttHead.m_sSessionId);
            pTmpGetDcc->getColumn(2, ttHead.m_sOrignHost);
            pTmpGetDcc->getColumn(3, ttHead.m_sOrignRealm);
            pTmpGetDcc->getColumn(4, ttHead.m_sDestRealm);
            pTmpGetDcc->getColumn(5, ttHead.m_sSrvTextId);
            pTmpGetDcc->getColumn(6, ttHead.m_sSrvFlowId);
        }
        
         pTmpGetDcc->Close();
	if (strlen(ttHead.m_sSessionId) == 0)
	{
		__DEBUG_LOG0_(0, "deal::deal ��ѯDCC��ϢͷΪ��,err_code=%d",QRY_GET_DCC_NOMSG_ERR);
		return QRY_GET_DCC_NOMSG_ERR;
	}
	
	__DEBUG_LOG0_(0, "��ӡ���ݽ��ܵ���Ϣsession_id��ѯ������Ϣͷ");
	__DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iAuthAppId=%d",ttHead.m_iAuthAppId);
	__DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iReqNumber=%d",ttHead.m_iReqNumber);
	__DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iReqType=%d",ttHead.m_iReqType);
	__DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_iResultCode=%d",ttHead.m_iResultCode);
	__DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sDestRealm=%s",ttHead.m_sDestRealm);
	__DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sOrignHost=%s",ttHead.m_sOrignHost);
	__DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sOutPlatform=%s",ttHead.m_sOutPlatform);
	__DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sSessionId=%s",ttHead.m_sSessionId);
	__DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sSrvFlowId=%s",ttHead.m_sSrvFlowId);
	__DEBUG_LOG1_(0, "��ӡ���ܵ�����Ϣ����Ϣͷm_sSrvTextId=%s",ttHead.m_sSrvTextId);
    }
    catch(TTStatus oSt)
    {
        __DEBUG_LOG1_(0, "deal::deal ��ѯDCC��Ϣͷʧ��,err_msg=%s",oSt.err_msg);
        __DEBUG_LOG1_(0, "deal::deal ��ѯDCC��Ϣͷʧ��,err_code=%d",QRY_GET_DCC_FAIL_ERR);
        return QRY_GET_DCC_FAIL_ERR;
    }
   
    
    
    //��ѯ���ݿ�,����session_id��ѯ��ȫ������ABM������
    memset((void *)&tmpSrv, 0x00, sizeof(tmpSrv));
    TimesTenCMD *pTmpGetMsg = m_poSql->m_poGetMsg;
    try
    {
    	// todo:Ӧ�ø����Է���ƽ̨��session-idȥ��ѯ
        //pTmpGetMsg->setParam(1, Head.m_sSessionId);
        pTmpGetMsg->setParam(1, sSessionId);
        pTmpGetMsg->Execute();
        if(!pTmpGetMsg->FetchNext()) 
        {
            tmpSrv.m_iBalSrc = 2;
            pTmpGetMsg->getColumn(1, &tmpSrv.m_iUnitTypID);
            pTmpGetMsg->getColumn(2, &tmpSrv.m_lBalAmount);
            pTmpGetMsg->getColumn(3, tmpSrv.m_sRequestId);
        }
    }
    catch(TTStatus oSt)
    {
        __DEBUG_LOG1_(0, "deal::deal ��ѯȫ��ABM����ʧ��,err_msg=%s",oSt.err_msg);
        __DEBUG_LOG1_(0, "deal::deal ��ѯȫ��ABM����ʧ��,err_code=%d",QRY_GET_ABMDATA_FAIL_ERR);
        return QRY_GET_ABMDATA_FAIL_ERR;
    }
    pTmpGetDcc->Close();
    if (tmpSrv.m_iBalSrc == 0)
    {
        __DEBUG_LOG0_(0, "deal::deal ��ѯ�����ȫ��ABM����Ϊ��,err_code=%d",QRY_GET_ABMDATA_NODATA_ERR);
        return QRY_GET_ABMDATA_NODATA_ERR;
    }

    __DEBUG_LOG0_(0, "��ӡ���ݽ��ܵ���Ϣsession_id��ѯ����ȫ�����ĵ�����");
    __DEBUG_LOG1_(0, "��ѯȫ�����ĵ�����-����ʱ���Դ:m_iBalSrc=%d",tmpSrv.m_iBalSrc);
    __DEBUG_LOG1_(0, "��ѯȫ�����ĵ�����-�������:m_iUnitTypID=%d",tmpSrv.m_iUnitTypID);
    __DEBUG_LOG1_(0, "��ѯȫ�����ĵ�����-���:m_lBalAmount=%ld",tmpSrv.m_lBalAmount);
    __DEBUG_LOG1_(0, "��ѯȫ�����ĵ�����-�����Է���ƽ̨����Ӧ��ˮ:m_sRequestId:%s",tmpSrv.m_sRequestId);
    
    //������Ϣͷ
    memcpy((void *)tmpRlt->m_sDccBuf,(void *)&ttHead,sizeof(ttHead));
    tmpRlt->clear();
    __DEBUG_LOG0_(0, "���÷����Է���ƽ̨��������ѯ�����CCA����Ϣͷ���");

    //��Ϣ���
    if ((iRet=tmp->getResPons(vPayRlt)) == 0) {
        __DEBUG_LOG0_(0, "getRlt::deal ����ʡ�ﷵ�ص���Ϣ����err_msg=%d",QRY_UNPACK_PROC_FAIL_ERR);
        m_oRlt.m_uiRltCode = QRY_UNPACK_PROC_FAIL_ERR;
        return QRY_UNPACK_PROC_FAIL_ERR;
    }
    else if (iRet < 0) {
        __DEBUG_LOG0_(0, "getRlt::deal ����ʡ�ﷵ�ص���Ϣ����err_msg=%d",QRY_UNPACK_PROC_FAIL_ERR);
        m_oRlt.m_uiRltCode = QRY_UNPACK_PROC_FAIL_ERR;
        return QRY_UNPACK_PROC_FAIL_ERR;
    }
    if (iRet != 1){
        __DEBUG_LOG0_(0, "getRlt::deal ����ʡ�ﷵ�ص���Ϣ����err_msg=%d",QRY_UNPACK_PROC_FAIL_ERR);
        m_oRlt.m_uiRltCode = QRY_UNPACK_PROC_FAIL_ERR;
        return QRY_UNPACK_PROC_FAIL_ERR;
    }
    __DEBUG_LOG0_(0, "����ʡ�ﷵ�ص���Ϣ�ķ����벿��:���.��һ����Ӧ��Ϣ��");
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ�ķ����벿��:���Ի�����֧���ʻ��������:m_lTotalBalAvail=%ld",vPayRlt[0]->m_lTotalBalAvail);
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ�ķ����벿��:m_sParaFieldResult=%s",vPayRlt[0]->m_sParaFieldResult);
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ�ķ����벿��:m_uiRltCode=%d",vPayRlt[0]->m_uiRltCode);
    
    
    for (int i=0; i<vPayRlt.size(); ++i)
    {
        tmp->getResBal(vPayRlt[i], vPaySrv);
        break;
    }

    /*
    if (vPaySrv.size() != 1)
    {
        __DEBUG_LOG0_(0, "ʡABM���ص���Ϣ�������!");
        m_oRlt.m_uiRltCode = QRY_UNPACK_PROC_FAIL_ERR;
        return QRY_UNPACK_PROC_FAIL_ERR;
    }
    */
    
    for(int j=0;j<vPaySrv.size();j++)
    {
    __DEBUG_LOG0_(0, "����ʡ�ﷵ�ص���Ϣ��ҵ�񲿷����");
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ��ҵ�񲿷����:m_iUnitTypID=%d",vPaySrv[j]->m_iUnitTypID);
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ��ҵ�񲿷����:����ʱ�ʣ����:m_lBalAmount=[%ld]",vPaySrv[j]->m_lBalAmount);
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ��ҵ�񲿷����:����ʱ����¿�ʹ�����:m_lBalAvailAble=[%ld]",vPaySrv[j]->m_lBalAvailAble);
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ��ҵ�񲿷����:m_lBalReserved=%ld",vPaySrv[j]->m_lBalReserved);
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ��ҵ�񲿷����:m_lBalUsed=%ld",vPaySrv[j]->m_lBalUsed);
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ��ҵ�񲿷����:m_sBalItmDetail=%s",vPaySrv[j]->m_sBalItmDetail);
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ��ҵ�񲿷����:m_uiEffDate=%ld",vPaySrv[j]->m_uiEffDate);
    __DEBUG_LOG1_(0, "����ʡ�ﷵ�ص���Ϣ��ҵ�񲿷����:m_uiExpDate=%ld",vPaySrv[j]->m_uiExpDate);
    }

    Date cur;
    m_oRlt.m_uiRltCode = 0;
    m_oRlt.m_uiResTime = compute(cur.toString());
    // Modfiy 2011.7.11 ��Ӧ��ˮӦ��Ϊ�Է���ƽ̨��������ˮ
    //snprintf(m_oRlt.m_sResID, sizeof(m_oRlt.m_sResID), 
    //    "002%s%08d00\0", cur.toString(),m_iCount++);
    //snprintf(m_oRlt.m_sResID,sizeof(m_oRlt.m_sResID),"%s\0",);
    strncpy(m_oRlt.m_sResID,tmpSrv.m_sRequestId,sizeof(m_oRlt.m_sResID));
    
    tmpRlt->m_iCmdID = ABMCMD_QRY_MICROPAY_INFO;
    __DEBUG_LOG0_(0, "��ӡ������Ӧ��Ϣ");
    __DEBUG_LOG1_(0, "��ӡ������Ӧ��Ϣ:������:m_uiRltCode=[%d]",m_oRlt.m_uiRltCode);
    __DEBUG_LOG1_(0, "��ӡ������Ӧ��Ϣm_sResID=%s",m_oRlt.m_sResID);
    __DEBUG_LOG1_(0, "��ӡ������Ӧ��Ϣ:��Ӧʱ��:m_uiResTime=[%d]",m_oRlt.m_uiResTime);
    __DEBUG_LOG1_(0, "��ӡ������Ӧ��Ϣ:m_sParaFieldResult=[%s]",m_oRlt.m_sParaFieldResult);
    __DEBUG_LOG1_(0, "��ӡ������Ӧ��Ϣ:m_iCmdID=[%d]",tmpRlt->m_iCmdID);
    iRet = tmpRlt->addRlt(m_oRlt);
    if (iRet != true)
    {
        __DEBUG_LOG0_(0, "������ص��Է���ƽ̨����Ϣ��������!");
        return QRY_PACK_SERVICE_FAIL_ERR;
    }
    __DEBUG_LOG0_(0, "���������ѯ��Ӧ�Ľ������ɡ�");

    __DEBUG_LOG0_(0, "���ȫ����������");
    __DEBUG_LOG1_(0, "���ȫ����������m_iBalSrc=%d",tmpSrv.m_iBalSrc);
    __DEBUG_LOG1_(0, "���ȫ����������m_iUnitTypID=%d",tmpSrv.m_iUnitTypID);
    __DEBUG_LOG1_(0, "���ȫ����������m_lBalAmount=%ld",tmpSrv.m_lBalAmount);
    iRet = tmpRlt->addSrv(tmpSrv);
    if (iRet != true)
    {
        __DEBUG_LOG0_(0, "������ص��Է���ƽ̨����Ϣȫ������������!");
        return QRY_PACK_SERVICE_FAIL_ERR;
    }
    __DEBUG_LOG0_(0, "���ȫ������������ɡ�");
    memset((void *)&tmpSrv,0x00,sizeof(tmpSrv));
    tmpSrv.m_iBalSrc = 1;
    tmpSrv.m_iUnitTypID = vPaySrv[0]->m_iUnitTypID;
    // Modfiy 2011.7.11 ��ʱȡ�ʱ�ʣ����
    //tmpSrv.m_lBalAmount = vPayRlt[0]->m_lTotalBalAvail;
    tmpSrv.m_lBalAmount = vPayRlt[0]->m_lTotalBalAvail;
    
    __DEBUG_LOG0_(0, "���ʡ������ݡ�");
    __DEBUG_LOG1_(0, "���ʡ�������:m_iBalSrc=%d",tmpSrv.m_iBalSrc);
    __DEBUG_LOG1_(0, "���ʡ�������:m_iUnitTypID=%d",tmpSrv.m_iUnitTypID);
    __DEBUG_LOG1_(0, "���ʡ�������:m_lBalAmount=%ld",tmpSrv.m_lBalAmount);
    iRet = tmpRlt->addSrv(tmpSrv);
    if (iRet != true)
    {
        __DEBUG_LOG0_(0, "������ص��Է���ƽ̨����ϢʡABM������!");
        return QRY_PACK_SERVICE_FAIL_ERR;
    }
    __DEBUG_LOG0_(0, "���ʡ���������ɡ�");
    
    // ���Է���ƽ̨��CCA
    g_toPlatformFlag = true;
    
    return 0;

}

unsigned int QueryAll::compute(char *str)
{
    char strdate[20];
    strncpy(strdate,str,14);
    strdate[14] = '\0';
    struct tm t;
    t.tm_sec = atoi (strdate+12);
    strdate[12] = 0;
    t.tm_min = atoi (strdate+10);
    strdate[10] = 0;
    t.tm_hour = atoi (strdate+8);
    strdate[8] = 0;
    t.tm_mday = atoi (strdate+6);
    strdate[6] = 0;
    t.tm_mon = atoi (strdate+4) - 1;
    strdate[4] = 0;
    t.tm_year = atoi (strdate) - 1900;
    return (unsigned int)mktime(&t);
}


