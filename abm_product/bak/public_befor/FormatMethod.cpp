/*VER: 3*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "FormatMethod.h"
#include "EventSection.h"
#include "LongTypeMgr.h"
#ifndef PRIVATE_MEMORY
#include "ParamInfoInterface.h"
#endif

extern char g_sFormatRecord[MAX_FORMAT_RECORD_LEN];


//##ModelId=4279B41E02B4
KeyTree<Trunk>* FormatMethod::m_poTrunkTree = NULL;

KeyTree<TServiceInfo2>* FormatMethod::m_poServiceInfo = NULL;

LocalHeadMgr* FormatMethod::m_poLocalHeadMgr = NULL;

//##ModelId=4279D0650064
KeyTree<int>* FormatMethod::m_pAreaCode = NULL;

//##ModelId=4279D44A02E9
KeyTree<HLR>* FormatMethod::m_pHLR = NULL;

//##ModelId=4279E935022A
KeyTree<int>* FormatMethod::m_pCarrier = NULL;

//##ModelId=427B3DF0029F
TServiceInfo* FormatMethod::m_aServiceInfo = NULL;

PNSegBilling* FormatMethod::m_aPNSegBilling = NULL;

//##ModelId=427B0F7A0057
int FormatMethod::m_iMaxPreCodeLen;

//##ModelId=427B0F990265
int FormatMethod::m_iMinPreCodeLen;

//##ModelId=427B3E34026B
int FormatMethod::m_iMaxAreaCodeLen;

//##ModelId=427B3E4802E1
int FormatMethod::m_iMinAreaCodeLen;

//##ModelId=427C2957029D
int FormatMethod::m_iMaxSegCodeLen;

//##ModelId=427C29730095
int FormatMethod::m_iMinSegCodeLen;
//�������� ��ʱ��¼�Ĳ���
unsigned int FormatMethod::iMaxPreCodeLen = 0;
unsigned int FormatMethod::iMinPreCodeLen = 0;
unsigned int FormatMethod::iMaxAreaCodeLen = 0;
unsigned int FormatMethod::iMinAreaCodeLen = 0;
unsigned int FormatMethod::iMaxSegCodeLen = 0;
unsigned int FormatMethod::iMinSegCodeLen = 0;

//##ModelId=427C5AD6010C
TSwitchInfo* FormatMethod::m_aSwitchInfo = NULL;

//##ModelId=427C5BD00274
KeyTree<int>* FormatMethod::m_pSwitchInfoPosTree = NULL;

KeyTree<int>* FormatMethod::m_pSwitchOPCInfoTree = NULL;


//����    
KeyTree<int>* FormatMethod::m_poTimesType = NULL;

//##ModelId=427C71EC0042
int FormatMethod::m_iServiceInfoCnt;

//##ModelId=427C727F00E3
int FormatMethod::m_iSwitchInfoCnt;

int FormatMethod::m_iPNSegBillingCnt;

OrgMgr * FormatMethod::m_poOrgMgr = NULL;

int FormatMethod::m_bAreaCodeCycleFit = 0;

int FormatMethod::m_iSelAreaPart = 0;

KeyTree<VisitAreaCodeRule>* FormatMethod::m_pVisitAreaCode = NULL;

KeyTree<EdgeRoamRule>* FormatMethod::m_pEdgeRoam = NULL;


FilterRule * FormatMethod::m_poFilterRule = NULL;

CustomFilter * FormatMethod::m_poCustomFilter=NULL;

KeyTree<SwitchItemType>* FormatMethod::m_poSwitchItemTypeTree = NULL;

vector<string> FormatMethod::m_strLocalAreaCode;
    
KeyTree<TrunkBilling>* FormatMethod::m_poTrunkBilling = NULL;   
    

int FormatMethod::m_iCallingOrgIDRule = 0;

int FormatMethod::m_iCalledOrgIDRule = 0;
    
int FormatMethod::m_iTrimMobHeadZero = 0;



//##ModelId=427986E40143
FormatMethod::FormatMethod(bool _InitAll)
{
    m_poTimesType = NULL;
    m_poTrunkTree = NULL;
    m_pAreaCode = NULL;
    m_pHLR = NULL;
    m_pCarrier = NULL;
    m_aServiceInfo = NULL;
    m_aPNSegBilling = NULL;
    m_aSwitchInfo = NULL;
    m_pSwitchInfoPosTree = NULL;
    m_poSwitchItemTypeTree = NULL;
    m_poTrunkBilling = NULL;

    m_poCustomFilter=NULL;

    if (!_InitAll) {        
        return;
    }

    loadTrunk();
    loadAreaCode();
    loadHLR();
    loadCarrier();
    //loadServiceInfo();
    loadServiceInfo2();
    loadSwitchInfo();
    
    loadTrunkBilling();
    loadTimesType();

}

//##ModelId=427986FF0083
FormatMethod::~FormatMethod()
{
    unloadTrunk();
    unloadAreaCode();
    unloadHLR();
    unloadCarrier();
    //unloadServiceInfo();
    unloadServiceInfo2();
    unloadSwitchInfo();
    unloadPNSegBilling();
    unloadTimesType();

    if (m_poCustomFilter) {
        m_poCustomFilter->unload ();
        delete m_poCustomFilter;
        m_poCustomFilter=NULL;
    }

    
    if (m_poLocalHeadMgr)
        delete m_poLocalHeadMgr;
    
    if (m_poOrgMgr)
        delete m_poOrgMgr;
    
}

//##ModelId=4279B2710389
void FormatMethod::loadTrunk()
{
    unloadTrunk();
    
    m_poTrunkTree = new KeyTree<Trunk>;
    if (!m_poTrunkTree)
        THROW(MBC_FormatMethod+1);
    
    DEFINE_QUERY(qry);
    
    qry.close();
    qry.setSQL(" select switch_id||'+'||trunk_code||'+'||'0' skey, "
        " trunk_type_id, to_number(need_billing) need_billing "
        " from b_trunk  where trunk_side <> '1' "
        "     union  "
        " select switch_id||'+'||trunk_code||'+'||'1' skey,  "
        " trunk_type_id, to_number(need_billing) need_billing  "   
        " from b_trunk where trunk_side <> '0' ");
    qry.open();
    
    while (qry.next()) {
        Trunk oTrunk;
        char sTemp[30];
        oTrunk.m_iTrunkTypeID = qry.field("trunk_type_id").asInteger();
        //strncpy(sTemp,qry.field("need_billing").asString(),2);
        //oTrunk.m_cNeedBilling = sTemp[0];
        oTrunk.m_iNeedBilling = qry.field("need_billing").asInteger();
        strcpy(sTemp,qry.field("skey").asString());

        m_poTrunkTree->add(sTemp,oTrunk);
    }
    
    qry.close();
}

//##ModelId=4279B96B0387
void FormatMethod::unloadTrunk()
{
    if (m_poTrunkTree) {
        delete m_poTrunkTree;
        m_poTrunkTree = NULL;
    }
}


void FormatMethod::loadServiceInfo2()
{
    unloadServiceInfo2();
    
    m_poServiceInfo = new KeyTree<TServiceInfo2>;
    if (!m_poServiceInfo)
        THROW(MBC_FormatMethod+2);
    
    DEFINE_QUERY(qry);
    
    qry.setSQL("select service_type_id,party_role_id,org_id,to_char(org_id) s_org_id from b_sp_service");
    qry.open();
    
    while (qry.next()) {
        TServiceInfo2 oSvcInfo;
        char sOrgIDKey[10];
        oSvcInfo.m_iOrgID = qry.field("org_id").asInteger();
        oSvcInfo.m_iPartyRoleID = qry.field("party_role_id").asInteger();
        oSvcInfo.m_iServiceTypeID = qry.field("service_type_id").asInteger();
        strcpy(sOrgIDKey,qry.field("s_org_id").asString());
        
        m_poServiceInfo->add(sOrgIDKey,oSvcInfo);
    }
    qry.close();
    
    
    char sTempValue[10];
    
    m_iCallingOrgIDRule = 0;
    m_iCalledOrgIDRule = 0;
    
    memset (sTempValue, 0, 10);
    if (ParamDefineMgr::getParam ("FORMAT_METHOD","CALLING_ORG_ID_RULE", sTempValue) )
        m_iCallingOrgIDRule = atoi (sTempValue);
    
    memset (sTempValue, 0, 10);
    if (ParamDefineMgr::getParam ("FORMAT_METHOD","CALLED_ORG_ID_RULE", sTempValue) )
        m_iCalledOrgIDRule = atoi (sTempValue);
}

void FormatMethod::unloadServiceInfo2()
{
    if (m_poServiceInfo) {
        delete m_poServiceInfo;
        m_poServiceInfo = NULL;
    }
}



//##ModelId=4279B29B011D
#ifdef PRIVATE_MEMORY
bool FormatMethod::getTrunkByKey(char *skey, Trunk *pTrunk)
{
    if (!pTrunk) return false;
    
    if (!m_poTrunkTree) //return false;
        loadTrunk();
    
    if (m_poTrunkTree->get(skey,pTrunk))
        return true;
    
    return false;
}
#else
bool FormatMethod::getTrunkByKey(char *skey, Trunk *pTrunk)
{
    if (!pTrunk) 
        return false;

    unsigned int iIdx = 0;

	if(!(G_PPARAMINFO->m_poTrunkIndex->get(skey, &iIdx)))
        return false;
    else
    {
		*pTrunk = *(G_PPARAMINFO->m_poTrunkList + iIdx);
        return true;
    }
}
#endif


//##ModelId=4279B60802E0

bool FormatMethod::getTrunk(int _iSwitchID, char *_sTrunkCode, char _cSide, Trunk* _pTrunk)
{
    static char sKey[100];
    
    sprintf(sKey,"%d+%s+%c",_iSwitchID,_sTrunkCode,_cSide);
    if (getTrunkByKey(sKey,_pTrunk))
        return true;

/*    
    //˫���м�
    sprintf(sKey,"%d+%s+2",_iSwitchID,_sTrunkCode);
    if (getTrunkByKey(sKey,_pTrunk))
        return true;
*/    
    return false;
}


//##ModelId=4279D21D0099
void FormatMethod::loadAreaCode()
{
    unloadAreaCode();
    
    m_pAreaCode = new KeyTree<int>;
    if (!m_pAreaCode)
        THROW(MBC_FormatMethod+3);

    DEFINE_QUERY(qry);
    
    qry.close();
    qry.setSQL("select area_code,long_group_type_id from b_area_code");
    qry.open();
    
    while (qry.next()) {
        m_pAreaCode->add(qry.field("area_code").asString(),qry.field("long_group_type_id").asInteger());
    }

    qry.close();
    qry.setSQL("select max(length(area_code)) max_len, min(length(area_code)) min_len  from b_area_code");
    qry.open();
    if (qry.next()) {
        m_iMaxAreaCodeLen = qry.field("max_len").asInteger();
        m_iMinAreaCodeLen = qry.field("min_len").asInteger();
    }
    else {
        m_iMaxAreaCodeLen = 0;
        m_iMinAreaCodeLen = 0;
    }
    
    qry.close();
    
    //## ȡ������ƥ�������Ϣ(�Ƿ�ѭ��ƥ�䡢ѭ�����ŵ�ѡ��)
    char sValue[1000];
    sValue[0] = 0;
    ParamDefineMgr::getParam("PRE_PROCESS","AREACODE_CYC_FIT",sValue);
    m_bAreaCodeCycleFit = atoi(sValue);
    
    sValue[0] = 0;
    ParamDefineMgr::getParam("PRE_PROCESS","SEL_AREA_PART",sValue);
    m_iSelAreaPart = atoi(sValue);
    
    sValue[0] = 0;
    ParamDefineMgr::getParam("FORMAT_METHOD","TRIM_MOBILE_HEAD_ZERO", sValue);
    m_iTrimMobHeadZero = atoi(sValue);    
}

//##ModelId=4279D235031E
void FormatMethod::unloadAreaCode()
{
    if (m_pAreaCode) {
        delete m_pAreaCode;
        m_pAreaCode = NULL;
    }
}

//##ModelId=4279D24E00C1
#ifdef PRIVATE_MEMORY
bool FormatMethod::getAreaCodeInfo(char* _sAreaCode, int &_iLongGroupType)
{
    if (!m_pAreaCode)
        loadAreaCode();
    
    int bRet = m_pAreaCode->get(_sAreaCode,&_iLongGroupType);
    
    if (bRet) return true;
    
    return false;
}
#else
bool FormatMethod::getAreaCodeInfo(char* _sAreaCode, int &_iLongGroupType)
{
	unsigned int iVal = 0;
    if(!(G_PPARAMINFO->m_poAreaCodeIndex->get(_sAreaCode, &iVal)))
	{
        return false;    
    } else{
		_iLongGroupType = (int)iVal;
        return true;
	}
}
#endif

//##ModelId=4279D2A2019E
void FormatMethod::loadHLR()
{
    unloadHLR();
    
    m_pHLR = new KeyTree<HLR>;
    if (!m_pHLR) THROW(MBC_FormatMethod+4);
    
    DEFINE_QUERY(qry);
    
    qry.close();
    qry.setSQL("select code_segment,area_code from b_hlr");
    qry.open();
    
    while (qry.next()) {
        HLR oHlr;
        strcpy(oHlr.m_sAreaCode, qry.field("area_code").asString());
        m_pHLR->add(qry.field("code_segment").asString(), oHlr);
    }

    qry.close();
    qry.setSQL("select max(length(code_segment)) max_len,min(length(code_segment)) min_len from b_hlr");
    qry.open();
    if (qry.next()) {
        m_iMaxSegCodeLen = qry.field("max_len").asInteger();
        m_iMinSegCodeLen = qry.field("min_len").asInteger();
    }
    else {
        m_iMaxSegCodeLen = 0;
        m_iMinSegCodeLen = 0;
    }
    
    qry.close();
}

//##ModelId=4279D2DA0217
void FormatMethod::unloadHLR()
{
    if (m_pHLR) {
        delete m_pHLR;
        m_pHLR = NULL;
    }
}


void FormatMethod::loadVisitAreaCode()
{
    unloadVisitAreaCode();
    m_pVisitAreaCode = new KeyTree<VisitAreaCodeRule>;
    if (!m_pVisitAreaCode)
        THROW(MBC_FormatMethod+12);
    
    DEFINE_QUERY(qry);
    
    qry.close();
    qry.setSQL("select trim(msc)||decode(lac,null,'',('-'||trim(lac)||decode(cell_id,null,'','-'||trim(cell_id)))) sKey, "
        " trim(area_code) area_code from b_visit_area_code");
    qry.open();
    
    while (qry.next()) {
        VisitAreaCodeRule oCodeRule;
        strcpy(oCodeRule.m_sAreaCode, qry.field("area_code").asString());
        m_pVisitAreaCode->add(qry.field("sKey").asString(), oCodeRule);
    }
    
    qry.close();
}

void FormatMethod::unloadVisitAreaCode()
{
    if (m_pVisitAreaCode) {
        delete m_pVisitAreaCode;
        m_pVisitAreaCode = NULL;
    }
}

//## ���� _sKey,�ҵ���Ӧ�� VisitAreaCodeRule �ṹ
#ifdef PRIVATE_MEMORY
bool FormatMethod::getVisitAreaCodeRule(char* _sKey, VisitAreaCodeRule *_pRule)
{
    if (!m_pVisitAreaCode)
        loadVisitAreaCode();
    
    return m_pVisitAreaCode->get(_sKey,_pRule);
}
#else
bool FormatMethod::getVisitAreaCodeRule(char* _sKey, VisitAreaCodeRule *_pRule)
{
	unsigned int iIdx = 0;
    if(G_PPARAMINFO->m_pVisitAreaCodeIndex->get(_sKey,&iIdx))
	{
		*_pRule = *(G_PPARAMINFO->m_poVisitAreaCodeRuleList + iIdx);
		return true;
	} else {
		return false;
	}
}
#endif

//## ���� _sMSC,_sLAC,_sCellID , ������Ӧ�� VisitAreaCodeRule
//## �ҵ�,����Ӧ�ĵ��õ�����_sVisitAreaCode; �Ҳ���,�����κθı�
bool FormatMethod::getVisitAreaCodeByMSC(char* _sMSC, char* _sLAC, char* _sCellID,
        char *_sVisitAreaCode)
{
    static char s_sKey[40];
    static VisitAreaCodeRule s_oCodeRule;
    
    sprintf(s_sKey,"%s-%s-%s", _sMSC, _sLAC, _sCellID);
    
    if (getVisitAreaCodeRule(s_sKey, &s_oCodeRule)) {
        strcpy(_sVisitAreaCode, s_oCodeRule.m_sAreaCode);
        return true;
    }
    
    sprintf(s_sKey,"%s-%s", _sMSC, _sLAC);
    if (getVisitAreaCodeRule(s_sKey, &s_oCodeRule)) {
        strcpy(_sVisitAreaCode, s_oCodeRule.m_sAreaCode);
        return true;
    }
        
    sprintf(s_sKey,"%s", _sMSC);
    if (getVisitAreaCodeRule(s_sKey, &s_oCodeRule)) {
        strcpy(_sVisitAreaCode, s_oCodeRule.m_sAreaCode);
        return true;
    }
    
    //ģ��ƥ��MSC
    char sTempMSC[30];
    int iLen = strlen(_sMSC)-1;
    strcpy(sTempMSC,_sMSC);
    while (iLen>0) {
        sTempMSC[iLen] = 0;
        if (getVisitAreaCodeRule(sTempMSC, &s_oCodeRule)){
            strcpy(_sVisitAreaCode, s_oCodeRule.m_sAreaCode);
            return true;
        }
        iLen--;
    }
    
    return false;
}

void FormatMethod::loadEdgeRoam()
{
    unloadEdgeRoam();
    m_pEdgeRoam = new KeyTree<EdgeRoamRule>;
    if (!m_pEdgeRoam)
        THROW(MBC_FormatMethod+13);
    
    DEFINE_QUERY(qry);
    
    qry.close();
    qry.setSQL("select trim(msc)||decode(lac,null,'',('-'||trim(lac)||decode(cell_id,null,'','-'||trim(cell_id)))) sKey, "
        " trim(area_code) area_code from b_edge_roam");
    qry.open();
    
    while (qry.next()) {
        EdgeRoamRule oCodeRule;
        strcpy(oCodeRule.m_sAreaCode, qry.field("area_code").asString());
        m_pEdgeRoam->add(qry.field("sKey").asString(), oCodeRule);
    }
    
    qry.close();
}

void FormatMethod::unloadEdgeRoam()
{
    if (m_pEdgeRoam) {
        delete m_pEdgeRoam;
        m_pEdgeRoam = NULL;
    }
}

//## ���� _sKey,�ҵ���Ӧ�� EdgeRoamRule �ṹ
#ifdef PRIVATE_MEMORY
bool FormatMethod::getEdgeRoamRule(char* _sKey, EdgeRoamRule *_pRule)
{
    if (!m_pEdgeRoam)
        loadEdgeRoam();
    
    return m_pEdgeRoam->get(_sKey,_pRule);
}
#else
bool FormatMethod::getEdgeRoamRule(char* _sKey, EdgeRoamRule *_pRule)
{
	unsigned int iIdx = 0;
    if(G_PPARAMINFO->m_pEdgeRoamIndex->get(_sKey,&iIdx))
	{
		*_pRule = *(G_PPARAMINFO->m_poEdgeRoamRuleList + iIdx);
		return true;
	} else {
		_pRule = 0;
		return false;
	}
}
#endif

//## ���� _sMSC,_sLAC,_sCellID , ������Ӧ�� EdgeRoamRule
//## �ҵ�,����Ӧ�ı߽����ε�����_sEdgeRoam; �Ҳ���,�����κθı�
bool FormatMethod::getEdgeRoamByMSC(char* _sMSC, char* _sLAC, char* _sCellID,
        char *_sEdgeRoam)
{
    static char s_sKey[40];
    static EdgeRoamRule s_oRoamRule;
    
    sprintf(s_sKey,"%s-%s-%s", _sMSC, _sLAC, _sCellID);
    if (getEdgeRoamRule(s_sKey, &s_oRoamRule)) {
        strcpy(_sEdgeRoam, s_oRoamRule.m_sAreaCode);
        return true;
    }

    sprintf(s_sKey,"%s-%s", _sMSC, _sLAC);
    if (getEdgeRoamRule(s_sKey, &s_oRoamRule)) {
        strcpy(_sEdgeRoam, s_oRoamRule.m_sAreaCode);
        return true;
    }

    sprintf(s_sKey,"%s", _sMSC);
    if (getEdgeRoamRule(s_sKey, &s_oRoamRule)) {
        strcpy(_sEdgeRoam, s_oRoamRule.m_sAreaCode);
        return true;
    }
    
    return false;
}

void FormatMethod::getVisitAreaCodeOf3G(EventSection *_poES)
{
    //������ȡ���õ���Ϣ
    char sHcode[20];
    //����MOC����:
    //���к���ͨ��������Ϣ��ȡ���õ����ţ�
    //���к�������ж�̬���κţ�һ��MOC�����л������ж�̬���κţ���ͨ�����ö�̬���κ�ȡ���õ����ź���,�����̬���κ�Ϊ�գ���ô�����к���Ĺ��������ŷŵ����к���ĵ��õ�������;
    //ͬʱ�����к��뵽�õ������ø��ƷѺ���ĵ��õ����š�    
    if(CALLTYPE_MOC == _poES->m_iCallTypeID)
    {
        if (!getVisitAreaCodeByMSC(_poES->m_sMSC, _poES->m_sLAC, _poES->m_sCellID,
            _poES->m_oEventExt.m_sCallingVisitAreaCode))
        {    
            //"�Ҳ���" ���� "֮ǰҲδԤ��" �Ʒѷ����õ����� ����Ϊ�� 
            if (0 == _poES->m_oEventExt.m_sCallingVisitAreaCode[0]) 
            {
                _poES->m_iErrorID = 1800;
                return;
            }
        }
        
        if(0 != _poES->m_sRomingNBR[0])
        {
            if (!getHLRInfoByMobileNBR(_poES->m_sRomingNBR,_poES->m_oEventExt.m_sCalledVisitAreaCode,sHcode))
            {    
                //"�Ҳ���" ���� "֮ǰҲδԤ��" �Ʒѷ����õ����� ����Ϊ��
                if (0 == _poES->m_oEventExt.m_sCalledVisitAreaCode[0]) 
                {
                    _poES->m_iErrorID = 1801;
                    return;
                }
            }
        }
        else
        {
            strcpy(_poES->m_oEventExt.m_sCalledVisitAreaCode,_poES->m_sCalledAreaCode);
        }
        
        strcpy(_poES->m_sBillingVisitAreaCode,_poES->m_oEventExt.m_sCallingVisitAreaCode); 
    }
    //����MTC����
    //���к���ͨ��������Ϣ��ȡ���õ�����;
    //���к���ĵ��õ����ž�ȡ���к���Ĺ��������š�ͬʱ�ѱ��к��뵽�õ������ø��ƷѺ���ĵ��õ����š�
    else if(CALLTYPE_MTC == _poES->m_iCallTypeID)  
    {
        if (!getVisitAreaCodeByMSC(_poES->m_sMSC, _poES->m_sLAC, _poES->m_sCellID,
            _poES->m_oEventExt.m_sCalledVisitAreaCode))
        {    
            //"�Ҳ���" ���� "֮ǰҲδԤ��" �Ʒѷ����õ����� ����Ϊ�� 
            if (0 == _poES->m_oEventExt.m_sCalledVisitAreaCode[0]) 
            {
                _poES->m_iErrorID = 1802;
                return;
            }
        }
        strcpy(_poES->m_oEventExt.m_sCallingVisitAreaCode,_poES->m_sCallingAreaCode); 
        strcpy(_poES->m_sBillingVisitAreaCode,_poES->m_oEventExt.m_sCalledVisitAreaCode); 
    }
    //��ת����
    //���ں�ת��������Ϊ���к���A����ת����B�ͺ�תĿ�����C��
    //���������к���ŵ���A�����к���ŵ���C������������ŵ���B
    //���к���ĵ��õ����ž�ȡ���������ţ�
    //��ת������Ϊ���������룬ͬʱ�ǼƷѺ��룬��ת����ͨ��������Ϣ��ȡ���õ����ţ�
    //�������к�תĿ�����Ķ�̬���κ��룬ͨ�����ö�̬���κ�ȡ���õ����ź����������̬���κ�Ϊ�գ���ô����תĿ�����Ĺ��������ŷŵ���ת����ĵ��õ������ϣ�Ŀǰϵͳû�к�תĿ�����ĵ��õ���������ֶΣ���Ҫ���ӣ���
    //ͬʱ�Ѻ�ת���뵽�õ������ø��ƷѺ���ĵ��õ����š�
    else if(CALLTYPE_FORWARD == _poES->m_iCallTypeID)    
    {
        strcpy(_poES->m_oEventExt.m_sCallingVisitAreaCode,_poES->m_sCallingAreaCode);
        //�ر�˵����Ŀǰ���ڲ����޸�StdEvent�Ľṹ��������ʱ��m_oEventExt.m_sFileName�ֶη��õ���������ĵ��õ�����
        if (!getVisitAreaCodeByMSC(_poES->m_sMSC, _poES->m_sLAC, _poES->m_sCellID,
            _poES->m_oEventExt.m_sFileName))
        {    
            //"�Ҳ���" ���� "֮ǰҲδԤ��" �Ʒѷ����õ����� ����Ϊ�� 
            if (0 == _poES->m_oEventExt.m_sFileName[0]) 
            {
                _poES->m_iErrorID = 1802;
                return;
            }
        }
        strcpy(_poES->m_sBillingVisitAreaCode,_poES->m_oEventExt.m_sFileName); 
        //��תĿ�������Ϊ���к���
        if(0 != _poES->m_sRomingNBR[0])
        {
            if (!getHLRInfoByMobileNBR(_poES->m_sRomingNBR,_poES->m_oEventExt.m_sCalledVisitAreaCode,sHcode))
            {    
                //"�Ҳ���" ���� "֮ǰҲδԤ��" �Ʒѷ����õ����� ����Ϊ��
                if (0 == _poES->m_oEventExt.m_sCalledVisitAreaCode[0]) 
                {
                    _poES->m_iErrorID = 1801;
                    return;
                }
            }
        }
        else
        {
            strcpy(_poES->m_oEventExt.m_sCalledVisitAreaCode,_poES->m_sCalledAreaCode);
        }
    }
}

void FormatMethod::judgeEdgeRoamOf3G(EventSection *_poES)
{
    //�жϱ߽�����
    //�Ʒѷ�����ͨ��MSC��LAC ��CellID���õ��߽罻�������ǵ������ţ�AREA_CODE����
    //��ȡ�������Ƚ�������MSC��LAC ��CellID3���ֶδ������ű�ȥ��ȡ��
    //����󲻵����ͽ�MSC��LAC �Ϳ��ֶδ������ű�ȥ��ȡ��
    //��������󲻵�����ô��MSC�����ֶκͿ��ֶδ���ȥȥ��ȡ��
    //��������󲻵���˵���˽��������漰�߽����ε������
    //������и��ǣ���ô�����������óɱ߽����Σ�ͬʱ���Ʒѵ��õ������óɱ߽罻�������ǵ������ţ�
    //���MOC���������е��õ������óɱ߽罻�������ǵ������ţ�
    //���MTC�����ѱ��е��õ������óɱ߽罻�������ǵ������š�
    char sRoamArea[20];
    if (getEdgeRoamByMSC(_poES->m_sMSC, _poES->m_sLAC, _poES->m_sCellID, sRoamArea)) 
    {
        if(CALLTYPE_MOC == _poES->m_iCallTypeID)
        {
            //����߽����θ��ǵظ����й�����һ������ô�����е��õ��ó����й�����
            if(!strcmp(sRoamArea,_poES->m_sCallingAreaCode))
            {
                strcpy(_poES->m_oEventExt.m_sCallingVisitAreaCode,_poES->m_sCallingAreaCode);
                strcpy(_poES->m_sBillingVisitAreaCode,_poES->m_oEventExt.m_sCallingVisitAreaCode); 
                //�ñ߽���������
                _poES->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM_BORD;
            }
        }
        else if(CALLTYPE_MTC == _poES->m_iCallTypeID)
        {
            //����߽����θ��ǵظ����й�����һ������ô�����е��õ������óɱ��й���������
            if(!strcmp(sRoamArea,_poES->m_sCalledAreaCode))
            {
                strcpy(_poES->m_oEventExt.m_sCalledVisitAreaCode,_poES->m_sCalledAreaCode);
                strcpy(_poES->m_sBillingVisitAreaCode,_poES->m_oEventExt.m_sCalledVisitAreaCode); 
                //�ñ߽���������
                _poES->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM_BORD;
            }
        }
        else if(CALLTYPE_FORWARD == _poES->m_iCallTypeID)
        {
            //����߽����θ��ǵظ�������������һ������ô�����������õ������óɱ��й���������
            if(!strcmp(sRoamArea,_poES->m_sThirdPartyAreaCode))
            {
                strcpy(_poES->m_oEventExt.m_sFileName,_poES->m_sThirdPartyAreaCode);
                strcpy(_poES->m_sBillingVisitAreaCode,_poES->m_oEventExt.m_sFileName); 
                //�ñ߽���������
                _poES->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM_BORD;
            }
        }
    }
}

void FormatMethod::getRoamTypeOf3G(EventSection *_poES)
{
    int iLongGroupTypeHome,iLongGroupTypeVisit;
    
    //���������ж�
    //����ƷѺ���Ĺ��������Ÿ��ƷѺ���ĵ��õ�������ͬ����ô��������Ϊ0��
    if(!strcmp(_poES->m_sBillingVisitAreaCode,_poES->m_sBillingAreaCode))
    {
        _poES->m_oEventExt.m_iRoamTypeID = ROAMTYPE_NOT_ROAM;
    }
    //����ƷѺ���Ĺ��������Ÿ��ƷѺ���ĵ��õ����Ų���ͬ�����ò�ͬ����������
    else
    {
        getAreaCodeInfo(_poES->m_sBillingAreaCode, iLongGroupTypeHome);
        getAreaCodeInfo(_poES->m_sBillingVisitAreaCode, iLongGroupTypeVisit);
        //����ƷѺ���Ĺ��������ŵĳ�;��������ʡ�ڣ����õ����ŵĳ�;��������ʡ�ڣ�
        //��ô��������Ϊʡ�����Σ�
        if((iLongGroupTypeHome == LONG_GROUPTYPE_PROV)&&(iLongGroupTypeVisit == LONG_GROUPTYPE_PROV))
        {
            _poES->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM_PROV;
        }
        //����ƷѺ���Ĺ��������ŵĳ�;��������ʡ�ڣ����õ����ŵĳ�;�������ǹ���,
        //��ô��������Ϊ����������
        else if((iLongGroupTypeHome == LONG_GROUPTYPE_PROV)&&(iLongGroupTypeVisit == LONG_GROUPTYPE_NATION))
        {
            _poES->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM_NATION_OUT;
        }
        //����ƷѺ���Ĺ��������ŵĳ�;�������ǹ��ڣ����õ����ŵĳ�;��������ʡ�ڣ�
        //��ô��������Ϊ�������룻
        else if((iLongGroupTypeHome == LONG_GROUPTYPE_NATION)&&(iLongGroupTypeVisit == LONG_GROUPTYPE_PROV))
        {
            _poES->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM_NATION_IN;
        }
        //����ƷѺ���Ĺ��������ŵĳ�;�������ǹ��ʣ����õ����ŵĳ�;��������ʡ�ڣ�
        //��ô��������Ϊ��������
        else if
        (
              (
                  (iLongGroupTypeHome == LONG_GROUPTYPE_INTER)||
                (iLongGroupTypeHome == LONG_GROUPTYPE_THIRT)||
                 (iLongGroupTypeHome == LONG_GROUPTYPE_AMERC)||
                 (iLongGroupTypeHome == LONG_GROUPTYPE_KONG)||
                 (iLongGroupTypeHome == LONG_GROUPTYPE_XK)||
                 (iLongGroupTypeHome == LONG_GROUPTYPE_AM)||
                 (iLongGroupTypeHome == LONG_GROUPTYPE_TW)||
                 (iLongGroupTypeHome == LONG_GROUPTYPE_HS)
              )
              &&
              (iLongGroupTypeVisit == LONG_GROUPTYPE_PROV)
        )
        {
            _poES->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM_NATION_IN;
        }
        //����ƷѺ���Ĺ��������ŵĳ�;��������ʡ�ڣ����õ����ŵĳ�;�������ǹ��ʣ�
        //��ô��������Ϊ��������
        else if
        (
            (iLongGroupTypeHome == LONG_GROUPTYPE_PROV)
            &&
            (
                (iLongGroupTypeVisit == LONG_GROUPTYPE_THIRT)||
                 (iLongGroupTypeVisit == LONG_GROUPTYPE_AMERC)||
                 (iLongGroupTypeVisit == LONG_GROUPTYPE_KONG)||
                 (iLongGroupTypeVisit == LONG_GROUPTYPE_XK)||
                 (iLongGroupTypeVisit == LONG_GROUPTYPE_AM)||
                 (iLongGroupTypeVisit == LONG_GROUPTYPE_TW)||
                 (iLongGroupTypeVisit == LONG_GROUPTYPE_HS)
             )    
        )
        {
            _poES->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM_NATION_OUT;
        }
    }  
    
    judgeEdgeRoamOf3G(_poES);
    return;
    
}

void FormatMethod::getMOCLongTypeOf3G(EventSection *_poES)
{
    //��������Ĭ��ֵ
    _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
    
    //�ж��Ƿ�ͬ��
    //�ж����к���İݷõغͱ��к���İݷõ���ͬ������û�����κ�ͬ�����ε����
    if(!strcmp(_poES->m_oEventExt.m_sCalledVisitAreaCode,_poES->m_oEventExt.m_sCallingVisitAreaCode))
    {
        _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
        return;
    }
    
    //�ж��Ƿ�����ͬ������
    if
    (    //�ж����й����غͱ��е��õ���ͬ
        (!strcmp(_poES->m_sCallingAreaCode,_poES->m_oEventExt.m_sCalledVisitAreaCode))
        &&
        //�����жϱ��й����غ����е��õ���ͬ
        (!strcmp(_poES->m_sCalledAreaCode,_poES->m_oEventExt.m_sCallingVisitAreaCode))
    )
    {
        _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
        return;
    }
    
    //�ж��Ƿ񱾵غ��У�������е��õغ���ͱ��й����غ���
    if(!strcmp(_poES->m_oEventExt.m_sCallingVisitAreaCode,_poES->m_sCalledAreaCode))
    {
        _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
        return;
    }
    

    //�������ֱ��ȡ���к���Ĺ����صĳ�;��������Ϊ��;���ͣ�
    int _iLongGroupType;
    getAreaCodeInfo(_poES->m_sCalledAreaCode,_iLongGroupType);
    getLongType(_poES,_iLongGroupType);
    
    return;
}

void FormatMethod::getMTCLongTypeOf3G(EventSection *_poES)
{
    //��������Ĭ��ֵ
    _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
    
    //�ж��Ƿ�ͬ��
    //�ж����к���İݷõغͱ��к���İݷõ���ͬ������û�����κ�ͬ�����ε����
    if(!strcmp(_poES->m_oEventExt.m_sCalledVisitAreaCode,_poES->m_oEventExt.m_sCallingVisitAreaCode))
    {
        _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
        return;
    }
    
    //�ж��Ƿ�������ͬ������
    if
    (    //�ж����й����غͱ��е��õ���ͬ
        (!strcmp(_poES->m_sCallingAreaCode,_poES->m_oEventExt.m_sCalledVisitAreaCode))
        &&
        //�����жϱ��й����غ����е��õ���ͬ
        (!strcmp(_poES->m_sCalledAreaCode,_poES->m_oEventExt.m_sCallingVisitAreaCode))
    )
    {
        _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
        return;
    }
    
    //�жϱ��й����غ���ͱ��е��õغ����Ƿ���ͬ
    if(!strcmp(_poES->m_oEventExt.m_sCalledVisitAreaCode,_poES->m_sCalledAreaCode))
    {
        _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
        return;
    }
    
    
    //�������ֱ��ȡ���к���ĵ��õصĳ�;��������Ϊ��;���ͣ�
    int _iLongGroupType;
    getAreaCodeInfo(_poES->m_oEventExt.m_sCalledVisitAreaCode,_iLongGroupType);
    getLongType(_poES,_iLongGroupType);
    
    return;
}    

void FormatMethod::getFORWARDLongTypeOf3G(EventSection *_poES)
{
    //��������Ĭ��ֵ
    _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN;
    
    int iLongType1,iLongType2;
    int _iLongGroupType;
        
    //���ں�ת��������Ϊ���к���A����ת����B�ͺ�תĿ�����C��
    //���������к���ŵ���A�����к���ŵ���C������������ŵ���B
    //���к���ĵ��õ����ž�ȡ���������ţ�
    //��ת������Ϊ���������룬ͬʱ�ǼƷѺ��룬��ת����ͨ��������Ϣ��ȡ���õ����ţ�
    //�������к�תĿ�����Ķ�̬���κ��룬ͨ�����ö�̬���κ�ȡ���õ����ź����������̬���κ�Ϊ�գ���ô����תĿ�����Ĺ��������ŷŵ���ת����ĵ��õ������ϣ�Ŀǰϵͳû�к�תĿ�����ĵ��õ���������ֶΣ���Ҫ���ӣ���
    //ͬʱ�Ѻ�ת���뵽�õ������ø��ƷѺ���ĵ��õ����š�
    //�ر�˵����Ŀǰ���ڲ����޸�StdEvent�Ľṹ��������ʱ��m_oEventExt.m_sFileName�ֶη��õ���������ĵ��õ�����
    
    //���ͨ���ȽϺ�ת����Ĺ����غ͵��õأ�ȷ�Ϻ�ת����û�з�������,
    if(!strcmp(_poES->m_oEventExt.m_sFileName,_poES->m_sThirdPartyAreaCode))
    {
        //�ж��Ƿ���ͬ������
        //�ж��Ƿ�ͬ��
        //�жϺ�ת����İݷõغͺ�תĿ�����İݷõ���ͬ������û�����κ�ͬ�����ε����
        if(!strcmp(_poES->m_oEventExt.m_sFileName,_poES->m_oEventExt.m_sCalledVisitAreaCode))
        {
            _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
            return;
        }
        
        //�ж��Ƿ�����ͬ������
        if
        (    //�жϺ�ת����ĵ��õغͺ�תĿ�����Ĺ�������ͬ
            (!strcmp(_poES->m_oEventExt.m_sFileName,_poES->m_sCalledAreaCode))
            &&
            //�����жϺ�ת��������غͺ�תĿ�����ĵ��õ���ͬ
            (!strcmp(_poES->m_sThirdPartyAreaCode,_poES->m_oEventExt.m_sCalledVisitAreaCode))
        )
        {
            _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
            return;
        }
        
        //�жϺ�ת����ĵ��õغͺ�תĿ�����Ĺ������Ƿ���ͬ
        if(!strcmp(_poES->m_sCalledAreaCode,_poES->m_oEventExt.m_sFileName))
        {
            _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; 
            return;
        }
        
        //��ô��;����ȡ���ں�תĿ�����Ĺ�����
        getAreaCodeInfo(_poES->m_sCalledAreaCode,_iLongGroupType);
        getLongType(_poES,_iLongGroupType);
        return;
    }
        
    //�����Ǻ�ת���뷢�����ε������
    if(strcmp(_poES->m_oEventExt.m_sFileName,_poES->m_sThirdPartyAreaCode)!=0)
    {
        //����ȡ�ú�ת����ӹ����ص����εصĳ�;����
        //��ô��;����ȡ���ں�ת����ĵ��õ�
        getAreaCodeInfo(_poES->m_oEventExt.m_sFileName,_iLongGroupType);
        getLongType(_poES,_iLongGroupType);
        
        //����ֻ����ֻ��һ�γ�;�����:
        //��1�����������������ĵ��õظ����к���Ĺ�������ͬ
        //����һ���������Ȼ����������ĵ��õظ����к���Ĺ����ز�ͬ
        //���ǵ���������ĵ��õغͱ��еĵ��õ���ͬ��Ҳ���Ƿ���ͬ�����ε����
        //��2������¶�ֻ����һ�γ�;
        if
        (
            (!strcmp(_poES->m_oEventExt.m_sFileName,_poES->m_sCalledAreaCode))
            ||
            (!strcmp(_poES->m_oEventExt.m_sFileName,_poES->m_oEventExt.m_sCalledVisitAreaCode))
        )
        {
            //�Ѿ�ȡ����;����
            return;
        }
        else if(_poES->m_iBusinessKey == BUSINESSKEY_FORWARDTYPE_UNCOND)
        {
            //ֻ����һ�γ�;���Ѿ�ȡ����;����
            return;
        }
        else
        //���ᷢ��2�γ�;
        {
            iLongType1 = _poES->m_oEventExt.m_iLongTypeID;
            _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN;
            //��ȡ��2�γ�;
            //��ô��;����ȡ���ں�ת����ĵ��õ�
            getAreaCodeInfo(_poES->m_oEventExt.m_sCalledVisitAreaCode,_iLongGroupType);
            getLongType(_poES,_iLongGroupType);
            iLongType2 = _poES->m_oEventExt.m_iLongTypeID;
            //����Ҫ���жϺ�ת����ĵ��õغͺ�תĿ�����ĵ��õ�֮���Ƿ�ͬ�ǣ���Ϊͬ���Ѿ���ǰ���жϹ��ˣ�
            if((iLongType1 == LONGTYPE_PROV)&&(iLongType2 == LONGTYPE_NATION))
            {
                _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN_PROV_NATION;
                return;
            }
            else if((iLongType1 == LONGTYPE_NATION)&&(iLongType2 == LONGTYPE_NATION))
            {
                _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN_NATION_NATION;
                return;
            }
            else if((iLongType1 == LONGTYPE_PROV)&&(iLongType2 == LONGTYPE_PROV))
            {
                _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN_PROV_PROV;
                return;
            }
            else if((iLongType1 == LONGTYPE_NATION)&&(iLongType2 == LONGTYPE_PROV))
            {
                _poES->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN_NATION_PROV;
                return;
            }
        }
    }
    
    return;
    
}    

//## ����3Gҵ�����������/��;����
void FormatMethod::formatRoamAndLongTypeOf3G(EventSection *_poES)
{
    if
    (
        (_poES->m_iCallTypeID!=CALLTYPE_MOC)
        &&
        (_poES->m_iCallTypeID!=CALLTYPE_MTC)
        &&
        (_poES->m_iCallTypeID!=CALLTYPE_FORWARD)
    )
    {
        //����Ҫ�����˳�
        return;
    }
    
    
    //�����Ǹ����µ����۾������õĴ����裺
    //������ȡ���õ�
    getVisitAreaCodeOf3G(_poES);
    
    if(_poES->m_iErrorID > 0)
    {
        return;
    }

    //��ȡ��������
    getRoamTypeOf3G(_poES);

    //�жϳ�;����
    if(CALLTYPE_MOC == _poES->m_iCallTypeID)
    {
        getMOCLongTypeOf3G(_poES);
        return;
    }
    else if(CALLTYPE_MTC == _poES->m_iCallTypeID)
    {
        getMTCLongTypeOf3G(_poES);
        return;
    }
    if(CALLTYPE_FORWARD == _poES->m_iCallTypeID)
    {
        getFORWARDLongTypeOf3G(_poES);
        return;
    }
    
    return;
    
}


//__________________________________________________________


//##ModelId=4279D2E6008E
#ifdef PRIVATE_MEMORY
bool FormatMethod::getHLRInfo(char* _sCodeSegment, HLR* _pHLR)
{
    if (!_pHLR) return false;
    
    if (!m_pHLR)
        loadHLR();
    
    int bRet = m_pHLR->get(_sCodeSegment,_pHLR);
    
    if (bRet) return true;
    
    return false;
}
#else
bool FormatMethod::getHLRInfo(char* _sCodeSegment, HLR* _pHLR)
{
    if (!_pHLR) return false;  
    unsigned int iIdx = 0;
    if(!(G_PPARAMINFO->m_poHLRIndex->get(_sCodeSegment, &iIdx)))
        return false;
    else
    {
        *_pHLR = *(G_PPARAMINFO->m_poHLRList + iIdx);
        return true;
    }
}
#endif

//�˺���Ϊ����ȷ����û����ϴӶ��õ�����ȷ��ʼʱ�䣻
//��Ϊ�����¼�����û�п�ʼʱ�䣬�޷���ȷ��ȷ����û����ϣ�
//���Ҵ����û����ܸĺţ����Կ�ʼʱ����Ҫ��֤��ȡ���ڿ�ʼʱ�仹��ȥ����ʱ�䣻
//ϵͳ�ڹ����ĳ��������¼��������ڿ�ʼʱ��ͽ���ʱ�䣬�ֱ���ڿ�ʼʱ��ͽ���ʱ��2���ֶ��ϣ�
//�ú����������ڽ���ʱ��ȥ�����û����ϣ�
//������ɹ�����ô�����ڿ�ʼʱ��ȥ�����û����ϣ�
//���ں������ܸı�ƷѺ����ר��ҵ�����ڲ����ǣ���Ϊר��Ĭ�ϲ���ĺ�
void FormatMethod::getStartDateForDS(EventSection *_poEventSection)
{
    if((_poEventSection->m_oEventExt.m_iSourceEventType!=6)&&(_poEventSection->m_oEventExt.m_iSourceEventType!=9))
        return;
    
    if(_poEventSection->m_iErrorID != 0)
        return ;
    
    char sStartDate[15];
    strncpy(sStartDate,_poEventSection->m_sStartDate,14);
    sStartDate[14]=0;
        
    //## ͨ�������������õĽ���ʱ�䣨����ĩʱ�䣩��ͼ�
    strcpy(_poEventSection->m_sStartDate,_poEventSection->m_sEndDate);
    _poEventSection->bindUserInfo();
    
    //## �����ɹ��ͷ���
    if (_poEventSection->m_iServID > 0)
    {
        //��Ҫ���serv_id,ԭ�����ڿ��ܼƷѺ���������Ĺ�����������Ҫ�ı䣬�����Ϊר������ȣ�
        _poEventSection->m_iServID = 0 ;
        _poEventSection->m_iErrorID = 0;
        return;
    }
    //����󶨲��ɹ�
    else
    {
        //���ñ����ڿ�ʼʱ����а�
        strcpy(_poEventSection->m_sStartDate,sStartDate);
        g_poEventSection->bindUserInfo();
        
        if(_poEventSection->m_iServID > 0 )
        {
            //��Ҫ���serv_id,ԭ�����ڿ��ܼƷѺ���������Ĺ�����������Ҫ�ı䣬�����Ϊר������ȣ�
            _poEventSection->m_iServID = 0 ;
            _poEventSection->m_iErrorID = 0;
            return;
        }
        else
        //��������Ҳ����û����ϣ���ͨ����ʼʱ�仹���޸�Ϊ���ڿ�ʼʱ��
        {
            strcpy(_poEventSection->m_sStartDate,sStartDate);
            _poEventSection->m_iServID = 0 ;
            _poEventSection->m_iErrorID = 0;
            return;
        }
    }
}

//##ModelId=4279E8200358
void FormatMethod::loadCarrier()
{
    unloadCarrier();
    
    m_pCarrier = new KeyTree<int>;
    if (!m_pCarrier) THROW(MBC_FormatMethod+5);
    
    DEFINE_QUERY(qry);
    
    qry.close();
    qry.setSQL("select precode,carrier_type_id from b_carrier");
    qry.open();
    
    while (qry.next()) {
        m_pCarrier->add(qry.field("precode").asString(),qry.field("carrier_type_id").asInteger());
    }
    
    qry.close();
    qry.setSQL("select max(length(precode)) max_len, min(length(precode)) min_len  from b_carrier");
    qry.open();
    if (qry.next()) {
        m_iMaxPreCodeLen = qry.field("max_len").asInteger();
        m_iMinPreCodeLen = qry.field("min_len").asInteger();
    }
    else {
        m_iMaxPreCodeLen = 0;
        m_iMinPreCodeLen = 0;
    }
    
    qry.close();
}

//##ModelId=4279E84C0199
void FormatMethod::unloadCarrier()
{
    if (m_pCarrier) {
        delete m_pCarrier;
        m_pCarrier = NULL;
    }
}

//##ModelId=4279E86C0308
#ifdef PRIVATE_MEMORY
bool FormatMethod::getCarrierInfo(char* _sPreCode, int &_iCarrierType)
{
    if (!m_pCarrier)
        loadCarrier();
    
    int bRet = m_pCarrier->get(_sPreCode, &_iCarrierType);
    
    if (bRet) return true;
    
    return false;
}
#else
bool FormatMethod::getCarrierInfo(char* _sPreCode, int &_iCarrierType)
{
	unsigned int iCarrierType = 0;
    if(G_PPARAMINFO->m_poCarrierIndex->get(_sPreCode, &iCarrierType))
	{
		_iCarrierType = (int)iCarrierType;
		return true;
	} 
	return false;
}
#endif

//##ModelId=427AE7520204
//## ������ǰָ̨������ FuncName:"�������͹���"
//## ����: ��ȡ�����������->g_poStdEventExt.m_iCarrierTypeID��ȥ�����к�����Ӧ���ֹ�
//## ǰ������: ���к���ĳ����ֹ�ǰ����������
#ifdef PRIVATE_MEMORY  
void FormatMethod::formatCarrier(EventSection *_poEventSection)
{
    //char sTempNbr[MAX_CALLED_NBR_LEN] = {0};
    
    if (!m_pCarrier)
        loadCarrier();
    
/*
    int iNbrLen = strlen(_poEventSection->m_sCalledNBR);                //���к��볤��
    int iCodeLen = m_iMaxPreCodeLen<iNbrLen ? m_iMaxPreCodeLen:iNbrLen;  //����Ҫ��ȡ�Ƚϵĳ���
    
    strncpy(sTempNbr, _poEventSection->m_sCalledNBR, iCodeLen);  //��ȡ�Ƚϵĳ���
    
    for (int iPos=iCodeLen; iPos>=m_iMinPreCodeLen; iPos--) {
        
        int iCarType;
        
        sTempNbr[iPos]=0;
        
        if (getCarrierInfo(sTempNbr, iCarType) ) {
            
            _poEventSection->m_oEventExt.m_iCarrierTypeID = iCarType;
            
            char *pt = &(_poEventSection->m_sCalledNBR[iPos]);
            
            strcpy(_poEventSection->m_sCalledNBR,pt);
            
            return;
        }
    }
*/
    static int iCarrierType, iMatchLen;
    if (m_pCarrier->getMax(_poEventSection->m_sCalledNBR, &iCarrierType, &iMatchLen)) 
    {
        strcpy(_poEventSection->m_sCalledNBR, (_poEventSection->m_sCalledNBR)+iMatchLen);
        _poEventSection->m_oEventExt.m_iCarrierTypeID = iCarrierType;
        return;
    }
    
    _poEventSection->m_oEventExt.m_iCarrierTypeID = 0;
}
#else //
void FormatMethod::formatCarrier(EventSection *_poEventSection)
{
    static unsigned int SHM_iCarrierType,SHM_iMatchLen;
	//
    if (G_PPARAMINFO->m_poCarrierIndex->getMax(_poEventSection->m_sCalledNBR, &SHM_iCarrierType,&SHM_iMatchLen)) 
    {
        strcpy(_poEventSection->m_sCalledNBR, (_poEventSection->m_sCalledNBR)+SHM_iMatchLen);
        _poEventSection->m_oEventExt.m_iCarrierTypeID = (int)SHM_iCarrierType;
        return;
    }
    _poEventSection->m_oEventExt.m_iCarrierTypeID = 0;
}
#endif

//##ModelId=427B155301D6
//## �����м�����ǰ,��Ҫ��ȷ��������ID
void FormatMethod::formatTrunkTypeID(EventSection *_poEventSection)
{
    static Trunk obTrunk ;
    
    _poEventSection->m_iTrunkNotBillFlag = 0;
    
    //ȡ���м���Ϣ
    if (getTrunk(_poEventSection->m_iSwitchID, _poEventSection->m_sTrunkIn, '0', &obTrunk)) 
    {
        _poEventSection->m_iTrunkInTypeID = obTrunk.m_iTrunkTypeID;
        if (obTrunk.m_iNeedBilling == 0) _poEventSection->m_iTrunkNotBillFlag = 1;
    }
    else
        _poEventSection->m_iTrunkInTypeID = 0;
    
    //ȡ���м���Ϣ
    if (getTrunk(_poEventSection->m_iSwitchID, _poEventSection->m_sTrunkOut, '1', &obTrunk)) 
    {
        _poEventSection->m_iTrunkOutTypeID = obTrunk.m_iTrunkTypeID;
        if (obTrunk.m_iNeedBilling == 0) _poEventSection->m_iTrunkNotBillFlag = 1;
    }
    else
        _poEventSection->m_iTrunkOutTypeID = 0;
}


//##ModelId=427B21E60370
//## ����������������(�����̶�ҵ���ƶ�ҵ��),���̶�ҵ��������к�������Žص�,
//## ͬʱ��ȡ��/���г�;������
void FormatMethod::formatAreaCode(EventSection *_poEventSection)
{
    static int  s_iCalledInterFlag = -1; //0-���к������Ų���ȡ, 1-���к������Ž�ȡ��
    static char sTempCalledNBR[MAX_ORG_CALLED_NBR_LEN];
    if (s_iCalledInterFlag == -1) {
        char sTemp[1000];
        if (ParamDefineMgr::getParam("FORMAT_METHOD", "CALLED_INTER_FLAG", sTemp)) {
            if (sTemp[0] == '1')  s_iCalledInterFlag = 1;
            else                  s_iCalledInterFlag = 0;
        }
        else //δָ��
            s_iCalledInterFlag = 1; //## Ĭ��Ҫ��ȡ��������
    }
    if (!s_iCalledInterFlag) strcpy (sTempCalledNBR, _poEventSection->m_sCalledNBR);

    
    if (_poEventSection->m_sCallingNBR[0])    //## ���к���
        formatAccNBR(_poEventSection->m_sCallingNBR, _poEventSection->m_sCallingAreaCode,
            1,_poEventSection->m_iSwitchID);
            
    if (_poEventSection->m_sCalledNBR[0])     //## ���к���
    {
        formatAccNBR(_poEventSection->m_sCalledNBR, _poEventSection->m_sCalledAreaCode,
            2,_poEventSection->m_iSwitchID);
        
        if (!s_iCalledInterFlag)
            strcpy (_poEventSection->m_sCalledNBR, sTempCalledNBR);
    }
    
    if (_poEventSection->m_sThirdPartyNBR[0]) //## ������������벻��,����������
        formatAccNBR(_poEventSection->m_sThirdPartyNBR,_poEventSection->m_sThirdPartyAreaCode,
            3,_poEventSection->m_iSwitchID);
    
    if (_poEventSection->m_sAccountNBR[0])    //## ����ҵ���ʺ�
        formatAccNBR(_poEventSection->m_sAccountNBR,_poEventSection->m_sAccountAreaCode,
            4,_poEventSection->m_iSwitchID);
    
    // �ó�;������
    int iLongGrpTypeID;
    
    if (_poEventSection->m_sCallingAreaCode[0] &&
            getAreaCodeInfo(_poEventSection->m_sCallingAreaCode,iLongGrpTypeID))
        _poEventSection->m_oEventExt.m_iCallingLongGroupTypeID = iLongGrpTypeID;
    else
        _poEventSection->m_oEventExt.m_iCallingLongGroupTypeID = 0;
    
    if (_poEventSection->m_sCalledAreaCode[0] &&
            getAreaCodeInfo(_poEventSection->m_sCalledAreaCode,iLongGrpTypeID))
        _poEventSection->m_oEventExt.m_iCalledLongGroupTypeID = iLongGrpTypeID; 
    else
        _poEventSection->m_oEventExt.m_iCalledLongGroupTypeID = 0;

}

// add by yangks 2007.03.03  for SC
//## ����������������(�����̶�ҵ���ƶ�ҵ��),���̶�ҵ��������к�������Žص�,
//## ͬʱ��ȡ��/���г�;������
void FormatMethod::format_AreaCode_V2(EventSection *_poEventSection)
{
    if (_poEventSection->m_sCallingNBR[0])    //## ���к���
        format_AccNBR_V2 (_poEventSection->m_sCallingNBR, _poEventSection->m_sCallingAreaCode,
            1,_poEventSection->m_iSwitchID);
            
    if (_poEventSection->m_sCalledNBR[0])     //## ���к���
        format_AccNBR_V2 (_poEventSection->m_sCalledNBR, _poEventSection->m_sCalledAreaCode,
            2,_poEventSection->m_iSwitchID);
    
    if (_poEventSection->m_sThirdPartyNBR[0]) //## ������������벻��,����������
        format_AccNBR_V2 (_poEventSection->m_sThirdPartyNBR,_poEventSection->m_sThirdPartyAreaCode,
            3,_poEventSection->m_iSwitchID);
    
    if (_poEventSection->m_sAccountNBR[0])    //## ����ҵ���ʺ�
        format_AccNBR_V2 (_poEventSection->m_sAccountNBR,_poEventSection->m_sAccountAreaCode,
            4,_poEventSection->m_iSwitchID);
    
    // �ó�;������
    int iLongGrpTypeID;
    
    if (_poEventSection->m_sCallingAreaCode[0] &&
            getAreaCodeInfo(_poEventSection->m_sCallingAreaCode,iLongGrpTypeID))
        _poEventSection->m_oEventExt.m_iCallingLongGroupTypeID = iLongGrpTypeID;
    else
        _poEventSection->m_oEventExt.m_iCallingLongGroupTypeID = 0;
    
    if (_poEventSection->m_sCalledAreaCode[0] &&
            getAreaCodeInfo(_poEventSection->m_sCalledAreaCode,iLongGrpTypeID))
        _poEventSection->m_oEventExt.m_iCalledLongGroupTypeID = iLongGrpTypeID; 
    else
        _poEventSection->m_oEventExt.m_iCalledLongGroupTypeID = 0;

}


//##ModelId=427B4AA501F2
//## �� _sAccNBR �ֽ�Ϊ ���롢����, �ֱ����� _sAccNBR, _sAreaCode ��
//## ǰ������: ��ǰ����Ϊ�Ϸ�����,�Ϸ������������£�
//##    cond_010: ����+�̶�����   02587654321
//                ����+�̶�����   2587654321  (�����һ��������,��Ҫ��ǰ̨���������м�"0",ת����cond_010)
//           ����+����+�̶�����   02502587654321/0252587654321 (����Ҫ��ѭ��ƥ��ʱ 
//                                  b_param_define.param_code='AREACODE_CYC_FIT' ��Ӧparam_value=1
//##    cond_020: ����+�ƶ�����   02513913930122 (��ȡ����)
//##    cond_030: �ƶ�����        13345678999
//##    cond_031: �ƶ�����        013345678999 
//##    cond_032: 3G  ����        (0)15......... [���ݲ��� 157........(����)/158........(��ݸ)]
//##    cond_040: �̶�����������  87654321    (Ŀǰ��������ж����ж�,���ݽ�������������)
void FormatMethod::formatAccNBR(char* _sAccNBR, char* _sAreaCode, int _iCallerOrCalled, int _iSwitchID)
{
    int iLongGrpTypeID;
#ifdef PRIVATE_MEMORY
    if (!m_pAreaCode) //�������δ����, m_iMaxAreaCodeLen Ϊ0,����,����������
        loadAreaCode();
#else
	char *pKey = "CycleFit";
	unsigned int iSHMRes = 0;
	if(!G_PPARAMINFO->m_poAreaCodeIndex->get(pKey,&iSHMRes))
	{
		m_bAreaCodeCycleFit = 0;
	}
	m_bAreaCodeCycleFit = (int)iSHMRes;
	
	pKey = "MobHeadZero";
	if(!G_PPARAMINFO->m_poAreaCodeIndex->get(pKey,&iSHMRes))
	{
		m_iTrimMobHeadZero = 0;
	}
	m_iTrimMobHeadZero = (int)iSHMRes;
	
	//
	pKey = "SelAreaPart";
	if(!G_PPARAMINFO->m_poAreaCodeIndex->get(pKey,&iSHMRes))
	{
		m_iSelAreaPart = 0;
	}
	m_iSelAreaPart = (int)iSHMRes;
#endif    
    // cond_010, cond_020  (��ȡ�� _sAccNBR ǰ������)
    if (_sAccNBR[0] == '0' && strncmp(_sAccNBR,"013",3) && strncmp(_sAccNBR,"015",3)&&strncmp(_sAccNBR,"014",3)&&strncmp(_sAccNBR,"018",3)) { 
        
        int iRet;
        if (m_bAreaCodeCycleFit)
            iRet = interceptAreaCodeCycle(_sAccNBR, _sAreaCode, iLongGrpTypeID);
        else
            iRet = interceptAreaCode(_sAccNBR, _sAreaCode, iLongGrpTypeID);
        
        if (!iRet) {            
            _sAreaCode[0]=0;
            return;
        }       
        return;
    }
    
    // cond_030:  _sAccNBR = "13345678999"
    if ((!strncmp(_sAccNBR,"13",2))||(!strncmp(_sAccNBR,"15",2))||(!strncmp(_sAccNBR,"14",2))||(!strncmp(_sAccNBR,"18",2))) {
#ifdef USE_HLR_CONVERT
        char sHcode[20];
        getHLRInfoByMobileNBR(_sAccNBR, _sAreaCode, sHcode);
#else
        TSwitchInfo obSwitchInfo;
        //���ݽ�������Ĭ������
        if (getSwitchInfoBySwitchID(_iSwitchID, &obSwitchInfo)) {
            strcpy(_sAreaCode, obSwitchInfo.m_sAreaCode);
        }
#endif
        return ;
    }
    
    // cond_031:  _sAccNBR = "013345678999"
    if ((!strncmp(_sAccNBR,"013",3))||(!strncmp(_sAccNBR,"015",3))||(!strncmp(_sAccNBR,"014",3))||(!strncmp(_sAccNBR,"018",3))) {
#ifdef USE_HLR_CONVERT
        char sHcode[20];
        getHLRInfoByMobileNBR(_sAccNBR, _sAreaCode, sHcode);
#else
        strcpy(_sAreaCode,"0"); //## "0"Ϊ����Ĭ������
#endif

        if (m_iTrimMobHeadZero)
            strcpy(_sAccNBR, _sAccNBR+1);
        
        return;
    }
    
    // cond_040:  �̶��������������ý���������  87654321
    int iLenTemp = strlen(_sAccNBR);
    if (_sAccNBR[0]!='0' && iLenTemp<=D_M_ACCNBR_LEN) {//## D_M_ACCNBR_LEN �̻�������󳤶� ��CommonMacro.h�ж���
        TSwitchInfo obSwitchInfo;
        //���ݽ�������Ĭ������
        if (getSwitchInfoBySwitchID(_iSwitchID, &obSwitchInfo)) {
            strcpy(_sAreaCode, obSwitchInfo.m_sAreaCode);
            return;
        }
    }
    
    // ����δ֪���: Ĭ��ȡ����������
    TSwitchInfo obSwitchInfo;
    if (getSwitchInfoBySwitchID(_iSwitchID, &obSwitchInfo)) {
        strcpy(_sAreaCode, obSwitchInfo.m_sAreaCode);
    }
}

//�ڶ��汾 add by yangks 2007.03.03
//## �� _sAccNBR �ֽ�Ϊ ���롢����, �ֱ����� _sAccNBR, _sAreaCode ��
//## ǰ������: ��ǰ����Ϊ�Ϸ�����,�Ϸ������������£�
//##    cond_010: ����+�̶�����   02587654321
//                ����+�̶�����   2587654321  (�����һ��������,��Ҫ��ǰ̨���������м�"0",ת����cond_010)
//           ����+����+�̶�����   02502587654321/0252587654321 (����Ҫ��ѭ��ƥ��ʱ 
//                                  b_param_define.param_code='AREACODE_CYC_FIT' ��Ӧparam_value=1
//##    cond_020: ����+�ƶ�����   02513913930122 (��ȡ����)
//##    cond_030: �ƶ�����        13345678999
//##    cond_031: �ƶ�����        013345678999 
//##    cond_032: 3G  ����        (0)15......... [���ݲ��� 157........(����)/158........(��ݸ)]
//##    cond_040: �̶�����������  87654321    (Ŀǰ��������ж����ж�,���ݽ�������������)
void FormatMethod::format_AccNBR_V2 (char* _sAccNBR, char* _sAreaCode, 
                int _iCallerOrCalled, int _iSwitchID)
{
    int iLongGrpTypeID;
#ifdef PRIVATE_MEMORY
    if (!m_pAreaCode) //�������δ����, m_iMaxAreaCodeLen Ϊ0,����,����������
        loadAreaCode();
#else
	char *pKey = "CycleFit";
	unsigned int iSHMRes = 0;
	if(!G_PPARAMINFO->m_poAreaCodeIndex->get(pKey,&iSHMRes))
	{
		m_bAreaCodeCycleFit = 0;
	}
	m_bAreaCodeCycleFit = (int)iSHMRes;
	
	pKey = "MobHeadZero";
	if(!G_PPARAMINFO->m_poAreaCodeIndex->get(pKey,&iSHMRes))
	{
		m_iTrimMobHeadZero = 0;
	}
	m_iTrimMobHeadZero = (int)iSHMRes;
	
	//
	pKey = "SelAreaPart";
	if(!G_PPARAMINFO->m_poAreaCodeIndex->get(pKey,&iSHMRes))
	{
		m_iSelAreaPart = 0;
	}
	m_iSelAreaPart = (int)iSHMRes;
#endif    
    // cond_010, cond_020  (��ȡ�� _sAccNBR ǰ������)
    if (_sAccNBR[0] == '0' && strncmp(_sAccNBR,"013",3) && strncmp(_sAccNBR,"015",3)
        &&strncmp(_sAccNBR,"014",3)&&strncmp(_sAccNBR,"018",3)
    ) {         
        int iRet;
        if (m_bAreaCodeCycleFit)
            iRet = interceptAreaCodeCycle(_sAccNBR, _sAreaCode, iLongGrpTypeID);
        else
            iRet = interceptAreaCode(_sAccNBR, _sAreaCode, iLongGrpTypeID);
        
        if (!iRet) {            
            _sAreaCode[0]=0;
            return;
        }       
        return;
    }
    
    // cond_030:  _sAccNBR = "13345678999"
    if ((!strncmp(_sAccNBR,"13",2))||(!strncmp(_sAccNBR,"15",2))||(!strncmp(_sAccNBR,"14",2))||(!strncmp(_sAccNBR,"18",2))) {
        char sHcode[20];
        bool bRet = getHLRInfoByMobileNBR(_sAccNBR, _sAreaCode, sHcode);
        if (!bRet) {
            TSwitchInfo obSwitchInfo;
            //���ݽ�������Ĭ������
            if (getSwitchInfoBySwitchID(_iSwitchID, &obSwitchInfo)) {
                strcpy(_sAreaCode, obSwitchInfo.m_sAreaCode);
            }
        }
        return ;
    }
    
    // cond_031:  _sAccNBR = "013345678999"
    if ((!strncmp(_sAccNBR,"013",3))||(!strncmp(_sAccNBR,"015",3))||(!strncmp(_sAccNBR,"014",3))||(!strncmp(_sAccNBR,"018",3))) {
        char sHcode[20];
        bool bRet = getHLRInfoByMobileNBR(_sAccNBR, _sAreaCode, sHcode);

        if (!bRet)
            strcpy(_sAreaCode,"0"); //## "0"Ϊ����Ĭ������

#ifdef DEF_HEBEI
        // called_nbr must be get rid of 0, because called_area_code was assigned 0
        strcpy(_sAccNBR, _sAccNBR+1);
#else
        if (m_iTrimMobHeadZero)
            strcpy(_sAccNBR, _sAccNBR+1);
#endif
        return;
    }
     
    // cond_040:  �̶��������������ý���������  87654321
    int iLenTemp = strlen(_sAccNBR);
    if (_sAccNBR[0]!='0' && iLenTemp<=D_M_ACCNBR_LEN) {//## D_M_ACCNBR_LEN �̻�������󳤶� ��CommonMacro.h�ж���
        TSwitchInfo obSwitchInfo;
        //���ݽ�������Ĭ������
        if (getSwitchInfoBySwitchID(_iSwitchID, &obSwitchInfo)) {
            strcpy(_sAreaCode, obSwitchInfo.m_sAreaCode);
            return;
        }
    }
    
    // ����δ֪���: Ĭ��ȡ����������
    TSwitchInfo obSwitchInfo;
    if (getSwitchInfoBySwitchID(_iSwitchID, &obSwitchInfo)) {
        strcpy(_sAreaCode, obSwitchInfo.m_sAreaCode);
    }
}

//##ModelId=427C5B31006D
void FormatMethod::loadSwitchInfo()
{
    unloadSwitchInfo();
    
    DEFINE_QUERY(qry);
    
    qry.close();
    qry.setSQL("select count(*) cnt from b_switch_info");
    qry.open();
    qry.next();
    
    m_iSwitchInfoCnt = qry.field("cnt").asInteger();
    
    if (!m_iSwitchInfoCnt) return;
    
    m_aSwitchInfo = new TSwitchInfo[m_iSwitchInfoCnt];
    if (!m_aSwitchInfo) 
        THROW(MBC_FormatMethod+6);
    
    m_pSwitchInfoPosTree = new KeyTree<int>;
    if (!m_pSwitchInfoPosTree)
        THROW(MBC_FormatMethod+7);
    
    // add by yangks 2007.02.12 for Jiangsu Dev
    m_pSwitchOPCInfoTree = new KeyTree<int>;
    if (!m_pSwitchOPCInfoTree)
        THROW(MBC_FormatMethod+20);
    
    qry.close();
    
    /* mod by yks 2007.07.06  for JS_TIBS_REQ_20070706_001_yangks
       ������Ĭ�ϵ�NetworkIDͳһΪ0
    qry.setSQL("select switch_id,file_name_rule,area_code,source_event_type,switch_type_id, "
        " nvl(network_id, -1) network_id, nvl(switch_opc_code,'#') switch_opc_code "
        " from b_switch_info ");
    */
    qry.setSQL("select switch_id,file_name_rule,area_code,source_event_type,switch_type_id, "
        " nvl(network_id, 0) network_id, nvl(switch_opc_code,'#') switch_opc_code "
        " from b_switch_info ");
    qry.open();
    
    for (int iPos=0; qry.next() && (iPos<m_iSwitchInfoCnt); iPos++) {
        m_aSwitchInfo[iPos].m_iSwitchID = qry.field("switch_id").asInteger();
        m_aSwitchInfo[iPos].m_iSourceEventType = qry.field("source_event_type").asInteger();
        m_aSwitchInfo[iPos].m_iSwitchTypeID = qry.field("switch_type_id").asInteger();
        strcpy(m_aSwitchInfo[iPos].m_sFileNameRule, qry.field("file_name_rule").asString());
        strcpy(m_aSwitchInfo[iPos].m_sAreaCode, qry.field("area_code").asString());

        //add by yangks 2007.02.12 for Jiangsu Dev
        m_aSwitchInfo[iPos].m_iNetworkID = qry.field("network_id").asInteger();
        strcpy(m_aSwitchInfo[iPos].m_sSwichOpcCode,qry.field("switch_opc_code").asString());
        if (m_aSwitchInfo[iPos].m_sSwichOpcCode[0] != '#')
            m_pSwitchOPCInfoTree->add(m_aSwitchInfo[iPos].m_sSwichOpcCode, iPos);
        
        m_pSwitchInfoPosTree->add(m_aSwitchInfo[iPos].m_iSwitchID,iPos);
    }
    
    qry.close();
}

//##ModelId=427C5B6003BD
void FormatMethod::unloadSwitchInfo()
{
    if (m_aSwitchInfo) {
        delete m_aSwitchInfo;
        m_aSwitchInfo = NULL;
    }
    
    if (m_pSwitchInfoPosTree) {
        delete m_pSwitchInfoPosTree;
        m_pSwitchInfoPosTree = NULL;
    }
}

//##ModelId=427C5B6D0091
#ifdef PRIVATE_MEMORY
bool FormatMethod::getSwitchInfoBySwitchID(int _iSwitchID, TSwitchInfo* _pSwitchInfo)
{
    if (!_pSwitchInfo) return false;
    
    if (!m_pSwitchInfoPosTree)
        loadSwitchInfo();
    
    int iPos;
    
    bool bRet = m_pSwitchInfoPosTree->get(_iSwitchID,&iPos);
    
    if (!bRet) return false; //not found
    
    *_pSwitchInfo = m_aSwitchInfo[iPos];
    
    return true;
}
#else
bool FormatMethod::getSwitchInfoBySwitchID(int _iSwitchID, TSwitchInfo* _pSwitchInfo)
{
    if (!_pSwitchInfo) return false;  
    unsigned int iPos = 0;
    bool bRet =  G_PPARAMINFO->m_poSwitchIndex->get(_iSwitchID, &iPos);
    if (!bRet) 	return false; //not found
    *_pSwitchInfo = *(G_PPARAMINFO->m_poSwitchList + iPos);//memcpy(_pSwitchInfo,G_PPARAMINFO->m_poSwitchList + iPos,sizeof(TSwitchInfo));
    return true;
}
#endif

#ifdef PRIVATE_MEMORY
bool FormatMethod::getSwitchInfoBySwitchOPC(char *_sOpcCode, TSwitchInfo* _pSwitchInfo)
{
    if (!_pSwitchInfo) return false;
    
    if (!m_pSwitchOPCInfoTree)
        loadSwitchInfo();
    
    int iPos;
    bool bRet =  m_pSwitchOPCInfoTree->get(_sOpcCode, &iPos);
    if (!bRet) return false;
    
    *_pSwitchInfo = m_aSwitchInfo[iPos];
    
    return true;
}
#else
bool FormatMethod::getSwitchInfoBySwitchOPC(char *_sOpcCode, TSwitchInfo* _pSwitchInfo)
{
    if (!_pSwitchInfo) return false;
    unsigned int iPos;
    bool bRet =  G_PPARAMINFO->m_poSwitchOPCIndex->get(_sOpcCode, &iPos);
    if (!bRet) return false;
    *_pSwitchInfo = *(G_PPARAMINFO->m_poSwitchList + iPos);
	//memcpy(_pSwitchInfo,G_PPARAMINFO->m_poSwitchList + iPos,sizeof(TSwitchInfo));
    return true;
}
#endif

//##ModelId=427C330302DA
//## �����ֻ�����(�������ǰ��0,�������ı� _sMobileNBR ��ֵ)
//## �������(�ֻ�����ĵ�����,���δ�ҵ���Ӧ�Ŷ�,���ı�_sAreaCode��ֵ)
//## ��� �ֻ��Ŷ�(���δ�ҵ�,���ı�_sHcode��ֵ)
//#ifdef PRIVATE_MEMORY
bool FormatMethod::getHLRInfoByMobileNBR(char* _sMobileNBR, char *_sAreaCode, char* _sHcode)
{
    int iPos;
    char sTempNBR[32];
    HLR obHLR;
    
    strcpy(sTempNBR,_sMobileNBR);
    if (sTempNBR[0]=='0')
        strcpy(sTempNBR,sTempNBR+1);
#ifdef PRIVATE_MEMORY    
    if (!m_pHLR)
        loadHLR();
#else
	char *p ="max";
	if(!G_PPARAMINFO->m_poHLRIndex->get(p,&iMaxSegCodeLen))
	{
		iMaxSegCodeLen = 0;
	}
	p ="min";
	if(!G_PPARAMINFO->m_poHLRIndex->get(p,&iMinSegCodeLen))
	{
		iMinSegCodeLen = 0;
	}
	m_iMaxSegCodeLen = (int)iMaxSegCodeLen;
	m_iMinSegCodeLen = (int)iMinSegCodeLen;
#endif    
    for (iPos=m_iMaxSegCodeLen; iPos>=m_iMinSegCodeLen; iPos-- ) {
        sTempNBR[iPos] = 0;
        if (getHLRInfo(sTempNBR,&obHLR)) {
            strcpy(_sHcode, sTempNBR);
            strcpy(_sAreaCode, obHLR.m_sAreaCode);
            return true;
        }
    }
    
    //δƥ�䵽�Ŷ�
    return false;
}

//##ModelId=427B9CC600EB
//## ��ȡ��׼�� ����+���� ��ʽ��_sAccNBR �е�����, ���ظ� _sAreaCode, _sAccNBR���ű��ص�
bool FormatMethod::interceptAreaCode(char* _sAccNBR, char* _sAreaCode, int &_iLongGroupType)
{
    char sTemp[MAX_ORG_CALLED_NBR_LEN] = {0};
    char sTempAccNBR[MAX_ORG_CALLED_NBR_LEN] = {0};
#ifdef PRIVATE_MEMORY  
    if (!m_pAreaCode) //�������δ����, m_iMaxAreaCodeLen Ϊ0,����,����������
        loadAreaCode();
#else
	char *p = "max";
	iMaxAreaCodeLen = 0;
	iMinAreaCodeLen = 0;
    if(!G_PPARAMINFO->m_poAreaCodeIndex->get(p,&iMaxAreaCodeLen))
	{
		iMaxAreaCodeLen = 0;
	}
	p = "min";
	if(!G_PPARAMINFO->m_poAreaCodeIndex->get(p,&iMinAreaCodeLen))
	{
		iMinAreaCodeLen = 0;
	}
	m_iMaxAreaCodeLen = (int)iMaxAreaCodeLen;
	m_iMinAreaCodeLen = (int)iMinAreaCodeLen;
#endif  
    int iAccNbrLen = strlen(_sAccNBR);
    int iMaxAreaLen = m_iMaxAreaCodeLen<iAccNbrLen ? m_iMaxAreaCodeLen:iAccNbrLen;
    
    strcpy(sTempAccNBR,_sAccNBR);
    strcpy(sTemp,_sAccNBR);
    
    for (int iPos=iMaxAreaLen; iPos>=m_iMinAreaCodeLen; iPos--) {
        sTemp[iPos] = 0;
        if (getAreaCodeInfo(sTemp,_iLongGroupType)) {
            strcpy(_sAreaCode,sTemp);
            strcpy(_sAccNBR,sTempAccNBR+iPos);
            return true;
        }
    }
    
    _iLongGroupType = 0;
    _sAreaCode[0] = 0;
    
    return false;
}

//## ��������: ѭ��ƥ��/��ȡ���������
// ����ƥ�䵽��������, ����1, ��ƥ�䵽Ĭ�Ϲ�������"0",�򷵻�2
//      ���� _sAccNBR �����Ž�ȡ�������� _sAreaCode��, ���ó�;������ _iLongGroupType
// ���ƥ�䲻��,����0, �� _sAreaCodeΪ��; _iLongGroupType=0;
int FormatMethod::interceptAreaCodeCycle(char* _sAccNBR, char* _sAreaCode, int &_iLongGroupType)
{
    static char sFirstFitArea[MAX_ORG_CALLED_NBR_LEN] = {0};
    static int iFirstLongGrpType = 0;
    
    char sTemp[MAX_ORG_CALLED_NBR_LEN] = {0};
    char sTempAccNBR[MAX_ORG_CALLED_NBR_LEN] = {0};
#ifdef PRIVATE_MEMORY    
    if (!m_pAreaCode) //�������δ����, m_iMaxAreaCodeLen Ϊ0,����,����������
        loadAreaCode();
#else
	char *p = "max";
	iMaxAreaCodeLen = 0;
	iMinAreaCodeLen = 0;
    if(!G_PPARAMINFO->m_poAreaCodeIndex->get(p,&iMaxAreaCodeLen))
	{
		iMaxAreaCodeLen = 0;
	}
	p = "min"; 
	if(!G_PPARAMINFO->m_poAreaCodeIndex->get(p,&iMinAreaCodeLen))
	{
		iMinAreaCodeLen = 0;
	}
	m_iMaxAreaCodeLen = (int)iMaxAreaCodeLen;
	m_iMinAreaCodeLen = (int)iMinAreaCodeLen;
	
	p = "SelAreaPart";
	unsigned int iSHMRes = 0;
	if(!G_PPARAMINFO->m_poAreaCodeIndex->get(p,&iSHMRes))
	{
		m_iSelAreaPart = 0;
	}
	m_iSelAreaPart = (int)iSHMRes;
#endif  
    int iAccNbrLen = strlen(_sAccNBR);
    int iMaxAreaLen = m_iMaxAreaCodeLen<iAccNbrLen ? m_iMaxAreaCodeLen:iAccNbrLen;

/*
    if (_sAccNBR[0]!='0' && strlen(_sAccNBR)>=D_M_ACCNBR_LEN+1) {
        sprintf(sTemp,"0%s",_sAccNBR);
    }
    else {
        strcpy(sTemp,_sAccNBR);
    }
*/
    strcpy(sTemp,_sAccNBR);
    strcpy(sTempAccNBR,sTemp);

    for (int iPos=iMaxAreaLen; iPos>=m_iMinAreaCodeLen; iPos--) {
        sTemp[iPos] = 0;
        if (getAreaCodeInfo(sTemp,_iLongGroupType)) {
            strcpy(_sAreaCode,sTemp);
            strcpy(_sAccNBR,sTempAccNBR+iPos);
            
            if (m_iSelAreaPart && sFirstFitArea[0] == 0) { //## ��ǰ�ǵ�һ��ƥ������
                strcpy(sFirstFitArea,sTemp);
                iFirstLongGrpType = _iLongGroupType;
            }
            
            if (!strcmp(_sAreaCode,"0"))     //@@ ֻƥ�䵽����Ĭ������"0", (��������ѭ��ƥ��)
                return 2;
            
            if (!strncmp(_sAreaCode,"00",2)) //@@ ƥ�䵽��������,����ĺ����������Ԥ֪,������ѭ��ƥ��
                return 1;
                
            //## ��ȡһ�����ź�� _sAccNBR �������:
            //  1. 87654321 ���� �̺��� 110
            //  2. 13566666666
            //  3. 013566666666
            //  4. 0512******** (��0�����ţ�����)
            //  5. 512********  (����0�����ţ�����)  [�ݲ������������,�����е����ű�����0]
            int iAccNbrLen = strlen(_sAccNBR);
            if (iAccNbrLen <= D_M_ACCNBR_LEN && _sAccNBR[0] != '0')             //## �������: 1
                return 1;
            if (iAccNbrLen>=D_M_MOBNBR_LEN-1 && iAccNbrLen<=D_M_MOBNBR_LEN+1 
                    && !strncmp(_sAccNBR,"13",2)&& !strncmp(_sAccNBR,"15",2)&& !strncmp(_sAccNBR,"18",2)&& !strncmp(_sAccNBR,"14",2))                               //## �������: 2
                return 1;
            if (iAccNbrLen>=D_M_MOBNBR_LEN && iAccNbrLen<=D_M_MOBNBR_LEN+2
                    && !strncmp(_sAccNBR,"013",3)&& !strncmp(_sAccNBR,"014",3)&& !strncmp(_sAccNBR,"015",3)&& !strncmp(_sAccNBR,"018",3))                              //## �������: 3
                return 1;
            
            if (
                (_sAccNBR[0]=='0')                                              //## �������: 4
            )
            {
                int iLongGroupTypeSav = _iLongGroupType; //## ���浱ǰֵ
                int iRet = FormatMethod::interceptAreaCodeCycle(_sAccNBR, _sAreaCode, _iLongGroupType);
                if (!iRet || iRet==2) { //δƥ�䵽 ���� ֻƥ�䵽����Ĭ������"0"
                    _iLongGroupType = iLongGroupTypeSav;
                    strcpy(_sAreaCode,sTemp);
                    if (iRet==2) {
                        sprintf(sTemp,"0%s",_sAccNBR);
                        strcpy(_sAccNBR,sTemp);
                    }
                }
            }
            
            if (m_iSelAreaPart) {
                strcpy(_sAreaCode,sFirstFitArea);
                _iLongGroupType = iFirstLongGrpType;
                sFirstFitArea[0] = 0;
            }
            
            return 1;
        }
    }
    
    _iLongGroupType = 0;
    _sAreaCode[0] = 0;
    
    return 0;
}


void FormatMethod::loadPNSegBilling()
{
    char sMaxEndNBRAhead[MAX_CALLING_NBR_LEN] = {0};
    
    unloadPNSegBilling();
    
    DEFINE_QUERY(qry);
    
    qry.close();
    qry.setSQL("select count(*) cnt from b_pn_seg_billing");
    qry.open();
    qry.next();
    
    m_iPNSegBillingCnt = qry.field("cnt").asInteger();
    
    if (!m_iPNSegBillingCnt) {
        m_aPNSegBilling = new PNSegBilling[1]; //Ŀ���� m_aPNSegBilling�ǿ�,��ʾ�����ع�
        return;
    }
    
    m_aPNSegBilling = new PNSegBilling[m_iPNSegBillingCnt];
    if (!m_aPNSegBilling)
        THROW(MBC_FormatMethod+8);
    
    qry.close();
    qry.setSQL(" select  area_code, begin_acc_nbr, end_acc_nbr, acc_nbr, "
        " to_char(eff_date,'yyyymmddhh24miss') eff_date, "
        " to_char(exp_date,'yyyymmddhh24miss') exp_date, "
        " to_number(nvl(seg_flag,0) ) seg_flag "
        " from b_pn_seg_billing order by begin_acc_nbr, end_acc_nbr " );
    qry.open();
    
    // �� begin_nbr ��С�����ַ�������, �������� m_aPNSegBilling ��
    for (int iPos=0; qry.next() && (iPos<m_iPNSegBillingCnt); iPos++) {
        strcpy(m_aPNSegBilling[iPos].m_sAreaCode, qry.field("area_code").asString());
        strcpy(m_aPNSegBilling[iPos].m_sBeginNBR, qry.field("begin_acc_nbr").asString());
        strcpy(m_aPNSegBilling[iPos].m_sEndNBR, qry.field("end_acc_nbr").asString());       
        strcpy(m_aPNSegBilling[iPos].m_sAccNBR, qry.field("acc_nbr").asString());
        strcpy(m_aPNSegBilling[iPos].m_sEffDate, qry.field("eff_date").asString());
        strcpy(m_aPNSegBilling[iPos].m_sExpDate, qry.field("exp_date").asString());
        
        strcpy(m_aPNSegBilling[iPos].m_sMaxEndNBRAhead,sMaxEndNBRAhead);
        if (strcmp(m_aPNSegBilling[iPos].m_sEndNBR, sMaxEndNBRAhead)>0)
            strcpy(sMaxEndNBRAhead, m_aPNSegBilling[iPos].m_sEndNBR);

        m_aPNSegBilling[iPos].m_iSegFlag=qry.field("seg_flag").asInteger();


        
    }
    
    qry.close();
}


void FormatMethod::unloadPNSegBilling()
{
    if (m_aPNSegBilling) {
        delete m_aPNSegBilling;
        m_aPNSegBilling = NULL;
    }
}

#ifdef PRIVATE_MEMORY 
bool FormatMethod::getYGPNSegBilling(char *_sAccNBR, char *_sAreaCode,
        char *_sDate, PNSegBilling &_oSeg)
{
    if (!m_aPNSegBilling)
        loadPNSegBilling();
    
    int iHead = 0, iTail = m_iPNSegBillingCnt-1;
    int iMid ;
    if (m_iPNSegBillingCnt == 0) return false;
    // �۰����, ��ͼ��m_aPNSegBilling���ҵ�һ�� m_sBeginNBR == _sAccNBR ��m_aPNSegBilling��¼
    // ���ܵĽ��: 
    //     1. �ҵ�, iMid ָ�� һ�� m_sBeginNBR == _sAccNBR ��m_aPNSegBilling��¼
    //     2. δ�ҵ�, �������: 
    //          a. iMid ָ�����һ�� m_sBeginNBR < _sAccNBR ��m_aPNSegBilling��¼
    //          b. iMid ָ����ǰһ�� m_sBeginNBR > _sAccNBR ��m_aPNSegBilling��¼
    while (iHead <= iTail) {
        iMid = (iHead + iTail)/2;
        int iRet = strcmp(_sAccNBR, m_aPNSegBilling[iMid].m_sBeginNBR);
        if (iRet > 0) {
            iHead = iMid+1;
            continue;
        }
        if (iRet < 0) {
            iTail = iMid-1;
            continue;
        }
        //found:
        break;
    }
    
    //Ŀ��: ��iMid ָ�� ���һ�� (m_sBeginNBR <= _sAccNBR) �� m_aPNSegBilling ��¼.
    int iRet = strcmp(_sAccNBR,m_aPNSegBilling[iMid].m_sBeginNBR);
    if (iRet < 0) {
        iMid--;
    }
    else if (iRet == 0) { //�α���ͼ���<����>�����Ƿ����뵱ǰm_sBeginNBR��ͬ�ļ�¼
        while (iMid < m_iPNSegBillingCnt-1) {
            iMid++;
            if (strcmp(_sAccNBR,m_aPNSegBilling[iMid].m_sBeginNBR)<0) {
                iMid--;
                break;
            }
        }   
    }
    
    // ��ǰ����ƥ��
    for ( ; iMid>=0 ; iMid--) {
        
        int iRetA = strcmp(_sAccNBR,m_aPNSegBilling[iMid].m_sEndNBR);
        //huangxy 20050718 modified,iRetA < 0 --->iRetA <= 0
        if (iRetA <= 0) { // _sAccNBR �ڵ�ǰ�Ŷ���
            if(strlen(m_aPNSegBilling[iMid].m_sEndNBR) == strlen(_sAccNBR) &&
            	 strlen(m_aPNSegBilling[iMid].m_sBeginNBR) == strlen(_sAccNBR) )  { ///�ϸ�ƥ��
	            if (strcmp(_sAreaCode, m_aPNSegBilling[iMid].m_sAreaCode)) //���Ų�һ��
	                continue;
	            
	            if (strcmp(_sDate,m_aPNSegBilling[iMid].m_sEffDate)*
	                    strcmp(_sDate,m_aPNSegBilling[iMid].m_sExpDate)<=0 )
	            {
	                _oSeg = m_aPNSegBilling[iMid];
	                return true;
	            }
	        }
	        else 
	        	continue ;
        }
        
        // �ж���ǰ�Ƿ��� m_sEndNBR ���� _sAccNBR С�ļ�¼
        if (strcmp(_sAccNBR, m_aPNSegBilling[iMid].m_sMaxEndNBRAhead)>0) { //û��
            break;
        }
    }
    return false;
}
#else
bool FormatMethod::getYGPNSegBilling(char *_sAccNBR, char *_sAreaCode,
        char *_sDate, PNSegBilling &_oSeg)
{
	m_iPNSegBillingCnt = G_PPARAMINFO->m_aPNSegBillingData->getCount();
	PNSegBilling *pPNSegBilling = G_PPARAMINFO->m_aPNSegBilling;
	
    int iHead = 1, iTail = m_iPNSegBillingCnt;
    int iMid ;
    if (m_iPNSegBillingCnt == 0) return false;
    // �۰����, ��ͼ��m_aPNSegBilling���ҵ�һ�� m_sBeginNBR == _sAccNBR ��m_aPNSegBilling��¼
    // ���ܵĽ��: 
    //     1. �ҵ�, iMid ָ�� һ�� m_sBeginNBR == _sAccNBR ��m_aPNSegBilling��¼
    //     2. δ�ҵ�, �������: 
    //          a. iMid ָ�����һ�� m_sBeginNBR < _sAccNBR ��m_aPNSegBilling��¼
    //          b. iMid ָ����ǰһ�� m_sBeginNBR > _sAccNBR ��m_aPNSegBilling��¼
    while (iHead <= iTail) {
        iMid = (iHead + iTail)/2;
        int iRet = strcmp(_sAccNBR, pPNSegBilling[iMid].m_sBeginNBR);
        if (iRet > 0) {
            iHead = iMid+1;
            continue;
        }
        if (iRet < 0) {
            iTail = iMid-1;
            continue;
        }
        //found:
        break;
    }
    
    //Ŀ��: ��iMid ָ�� ���һ�� (m_sBeginNBR <= _sAccNBR) �� m_aPNSegBilling ��¼.
    int iRet = strcmp(_sAccNBR,pPNSegBilling[iMid].m_sBeginNBR);
    if (iRet < 0) {
        iMid--;
    }
    else if (iRet == 0) { //�α���ͼ���<����>�����Ƿ����뵱ǰm_sBeginNBR��ͬ�ļ�¼
        while (iMid < m_iPNSegBillingCnt) {
            iMid++;
            if (strcmp(_sAccNBR,pPNSegBilling[iMid].m_sBeginNBR)<0) {
                iMid--;
                break;
            }
        }   
    }
    
    // ��ǰ����ƥ��
    for ( ; iMid>0 ; iMid--) {
        
        int iRetA = strcmp(_sAccNBR,pPNSegBilling[iMid].m_sEndNBR);
        //huangxy 20050718 modified,iRetA < 0 --->iRetA <= 0
        if (iRetA <= 0) { // _sAccNBR �ڵ�ǰ�Ŷ���
            if(strlen(pPNSegBilling[iMid].m_sEndNBR) == strlen(_sAccNBR) &&
            	 strlen(pPNSegBilling[iMid].m_sBeginNBR) == strlen(_sAccNBR) )  { ///�ϸ�ƥ��
	            if (strcmp(_sAreaCode, pPNSegBilling[iMid].m_sAreaCode)) //���Ų�һ��
	                continue;
	            
	            if (strcmp(_sDate,pPNSegBilling[iMid].m_sEffDate)*
	                    strcmp(_sDate,pPNSegBilling[iMid].m_sExpDate)<=0 )
	            {
	                _oSeg = pPNSegBilling[iMid];
	                return true;
	            }
	        }
	        else 
	        	continue ;
        }
        
        // �ж���ǰ�Ƿ��� m_sEndNBR ���� _sAccNBR С�ļ�¼
        if (strcmp(_sAccNBR, pPNSegBilling[iMid].m_sMaxEndNBRAhead)>0) { //û��
            break;
        }
    }
    return false;
}
#endif

#ifdef PRIVATE_MEMORY 
bool FormatMethod::getPNSegBilling(char *_sAccNBR, char *_sAreaCode,
        char *_sDate, PNSegBilling &_oSeg)
{
    if (!m_aPNSegBilling)
        loadPNSegBilling();
    
    int iHead = 0, iTail = m_iPNSegBillingCnt-1;
    int iMid ;
    if (m_iPNSegBillingCnt == 0) return false;
    // �۰����, ��ͼ��m_aPNSegBilling���ҵ�һ�� m_sBeginNBR == _sAccNBR ��m_aPNSegBilling��¼
    // ���ܵĽ��: 
    //     1. �ҵ�, iMid ָ�� һ�� m_sBeginNBR == _sAccNBR ��m_aPNSegBilling��¼
    //     2. δ�ҵ�, �������: 
    //          a. iMid ָ�����һ�� m_sBeginNBR < _sAccNBR ��m_aPNSegBilling��¼
    //          b. iMid ָ����ǰһ�� m_sBeginNBR > _sAccNBR ��m_aPNSegBilling��¼
    while (iHead <= iTail) {
        iMid = (iHead + iTail)/2;
        int iRet = strcmp(_sAccNBR, m_aPNSegBilling[iMid].m_sBeginNBR);
        if (iRet > 0) {
            iHead = iMid+1;
            continue;
        }
        if (iRet < 0) {
            iTail = iMid-1;
            continue;
        }
        //found:
        break;
    }
    
    //Ŀ��: ��iMid ָ�� ���һ�� (m_sBeginNBR <= _sAccNBR) �� m_aPNSegBilling ��¼.
    int iRet = strcmp(_sAccNBR,m_aPNSegBilling[iMid].m_sBeginNBR);
    if (iRet < 0) {
        iMid--;
    }
    else if (iRet == 0) { //�α���ͼ���<����>�����Ƿ����뵱ǰm_sBeginNBR��ͬ�ļ�¼
        while (iMid < m_iPNSegBillingCnt-1) {
            iMid++;
            if (strcmp(_sAccNBR,m_aPNSegBilling[iMid].m_sBeginNBR)<0) {
                iMid--;
                break;
            }
        }   
    }
    
    // ��ǰ����ƥ��
    for ( ; iMid>=0 ; iMid--) {
        
        int iRetA = strcmp(_sAccNBR,m_aPNSegBilling[iMid].m_sEndNBR);
        //huangxy 20050718 modified,iRetA < 0 --->iRetA <= 0
        if (iRetA <= 0) { // _sAccNBR �ڵ�ǰ�Ŷ���
            
            if (strcmp(_sAreaCode, m_aPNSegBilling[iMid].m_sAreaCode)) //���Ų�һ��
                continue;
            
            if (strcmp(_sDate,m_aPNSegBilling[iMid].m_sEffDate)*
                    strcmp(_sDate,m_aPNSegBilling[iMid].m_sExpDate)<=0 )
            {
                _oSeg = m_aPNSegBilling[iMid];
                return true;
            }
            
        }
        
        // �ж���ǰ�Ƿ��� m_sEndNBR ���� _sAccNBR С�ļ�¼
        if (strcmp(_sAccNBR, m_aPNSegBilling[iMid].m_sMaxEndNBRAhead)>0) { //û��
            break;
        }
    }
    return false;
}
#else //
bool FormatMethod::getPNSegBilling(char *_sAccNBR, char *_sAreaCode,
        char *_sDate, PNSegBilling &_oSeg)
{
	int iHead = 1;  
    m_iPNSegBillingCnt = G_PPARAMINFO->m_aPNSegBillingData->getCount();
	PNSegBilling *pPNSegBilling = G_PPARAMINFO->m_aPNSegBilling;
	int iTail = m_iPNSegBillingCnt;
    int iMid ;
    if (m_iPNSegBillingCnt == 0) return false;
    // �۰����, ��ͼ��m_aPNSegBilling���ҵ�һ�� m_sBeginNBR == _sAccNBR ��m_aPNSegBilling��¼
    // ���ܵĽ��: 
    //     1. �ҵ�, iMid ָ�� һ�� m_sBeginNBR == _sAccNBR ��m_aPNSegBilling��¼
    //     2. δ�ҵ�, �������: 
    //          a. iMid ָ�����һ�� m_sBeginNBR < _sAccNBR ��m_aPNSegBilling��¼
    //          b. iMid ָ����ǰһ�� m_sBeginNBR > _sAccNBR ��m_aPNSegBilling��¼
    while (iHead <= iTail) {
        iMid = (iHead + iTail)/2;
        int iRet = strcmp(_sAccNBR, pPNSegBilling[iMid].m_sBeginNBR);
        if (iRet > 0) {
            iHead = iMid+1;
            continue;
        }
        if (iRet < 0) {
            iTail = iMid-1;
            continue;
        }
        //found:
        break;
    } 
    //Ŀ��: ��iMid ָ�� ���һ�� (m_sBeginNBR <= _sAccNBR) �� m_aPNSegBilling ��¼.
    int iRet = strcmp(_sAccNBR,pPNSegBilling[iMid].m_sBeginNBR);
    if (iRet < 0) {
        iMid--;
    }
    else if (iRet == 0) { //�α���ͼ���<����>�����Ƿ����뵱ǰm_sBeginNBR��ͬ�ļ�¼
        while (iMid < m_iPNSegBillingCnt) {
            iMid++;
            if (strcmp(_sAccNBR,pPNSegBilling[iMid].m_sBeginNBR)<0) {
                iMid--;
                break;
            }
        }   
    }
    // ��ǰ����ƥ��
    for ( ; iMid>0 ; iMid--) {     
        int iRetA = strcmp(_sAccNBR,pPNSegBilling[iMid].m_sEndNBR);
        //huangxy 20050718 modified,iRetA < 0 --->iRetA <= 0
        if (iRetA <= 0) { // _sAccNBR �ڵ�ǰ�Ŷ���           
            if (strcmp(_sAreaCode, pPNSegBilling[iMid].m_sAreaCode)) //���Ų�һ��
                continue;           
            if (strcmp(_sDate,pPNSegBilling[iMid].m_sEffDate)*
                    strcmp(_sDate,pPNSegBilling[iMid].m_sExpDate)<=0 )
            {
                _oSeg = pPNSegBilling[iMid];
                return true;
            }          
        }    
        // �ж���ǰ�Ƿ��� m_sEndNBR ���� _sAccNBR С�ļ�¼
        if (strcmp(_sAccNBR, pPNSegBilling[iMid].m_sMaxEndNBRAhead)>0) { //û��
            break;
        }
    }
    return false;
}
#endif
bool FormatMethod::formatYGPNSegAccNBR(EventSection *poEvt)
{
#ifdef  USE_formatPNSegAccNbrEx
    //## ����Ҫ��ר���Ʒ�ʱ��Ҫ���� ������/�м� ��Ϣ
    return SegBillingMgr::formatPNSegAccNbrEx (poEvt);
    //return;
#endif
  
#ifdef  ENABLE_PN_SEG_NOT_BILLING
    /////���⴦��,���ڲ��ֺ���,��Ȼ��ר���Ŷ�,���ǲ���Ҫȥ�滻��ר������
    if( SegBillingMgr::segNotBilling(
            poEvt->m_sBillingNBR,
            poEvt->m_sBillingAreaCode,
            poEvt->m_sStartDate) )
        return false;
#endif
  
  
    static PNSegBilling oPNSegBilling;
    
    if (getYGPNSegBilling(poEvt->m_sBillingNBR,poEvt->m_sBillingAreaCode,poEvt->m_sStartDate, oPNSegBilling))
    {
//////ʹ�úŶα�־���жϴ���ʽ
        if(oPNSegBilling.m_iSegFlag==0)
        {//////ʹ��ר���Ʒ�
            strcpy(poEvt->m_sBillingNBR, oPNSegBilling.m_sAccNBR);
            poEvt->unbindUserInfo();      
            return true;      
        }
        else
        {//////ר�������мƷѷ�ʽ����
            if(poEvt->m_iServID <=0 )
            {
                poEvt->unbindUserInfo();
                /////
                if( poEvt->m_sBillingAreaCode[0]=='\0')
                {
                    strcpy(poEvt->m_sBillingAreaCode, poEvt->m_sCallingAreaCode);
                }                    
                if( poEvt->m_sBillingNBR[0]=='\0')
                {
                    strcpy(poEvt->m_sBillingNBR, poEvt->m_sCallingNBR);
                }
                poEvt->bindUserInfo();
            }
            if(poEvt->m_iServID >0 )
            {////�е��û� �������мƷ�
                /////���ﲻ�ò���
            }
            else
            {///�޵��û� ʹ��ר���Ʒ�
                strcpy(poEvt->m_sBillingNBR, oPNSegBilling.m_sAccNBR);
                poEvt->unbindUserInfo();      
                return true;             
            }
        }
    }
    return false;
}

bool FormatMethod::formatPNSegAccNBR(EventSection *poEvt)
{
#ifdef  USE_formatPNSegAccNbrEx
    //## ����Ҫ��ר���Ʒ�ʱ��Ҫ���� ������/�м� ��Ϣ
    return SegBillingMgr::formatPNSegAccNbrEx (poEvt);
    //return;
#endif
  
#ifdef  ENABLE_PN_SEG_NOT_BILLING
    /////���⴦��,���ڲ��ֺ���,��Ȼ��ר���Ŷ�,���ǲ���Ҫȥ�滻��ר������
    if( SegBillingMgr::segNotBilling(
            poEvt->m_sBillingNBR,
            poEvt->m_sBillingAreaCode,
            poEvt->m_sStartDate) )
        return false;
#endif
  
  
    static PNSegBilling oPNSegBilling;
    
    if (getPNSegBilling(poEvt->m_sBillingNBR,poEvt->m_sBillingAreaCode,poEvt->m_sStartDate, oPNSegBilling))
    {
//////ʹ�úŶα�־���жϴ���ʽ
        if(oPNSegBilling.m_iSegFlag==0)
        {//////ʹ��ר���Ʒ�
            strcpy(poEvt->m_sBillingNBR, oPNSegBilling.m_sAccNBR);
            poEvt->unbindUserInfo();      
            return true;      
        }
        else
        {//////ר�������мƷѷ�ʽ����
            if(poEvt->m_iServID <=0 )
            {
                poEvt->unbindUserInfo();
                /////
                if( poEvt->m_sBillingAreaCode[0]=='\0')
                {
                    strcpy(poEvt->m_sBillingAreaCode, poEvt->m_sCallingAreaCode);
                }                    
                if( poEvt->m_sBillingNBR[0]=='\0')
                {
                    strcpy(poEvt->m_sBillingNBR, poEvt->m_sCallingNBR);
                }
                poEvt->bindUserInfo();
            }
            if(poEvt->m_iServID >0 )
            {////�е��û� �������мƷ�
                /////���ﲻ�ò���
            }
            else
            {///�޵��û� ʹ��ר���Ʒ�
                strcpy(poEvt->m_sBillingNBR, oPNSegBilling.m_sAccNBR);
                poEvt->unbindUserInfo();      
                return true;             
            }
        }
    }
    return false;
}


//##ModelId=427B27FD0371
void FormatMethod::loadServiceInfo()
{
    char sMaxEndNBRAhead[MAX_CALLING_NBR_LEN] = {0};
    
    unloadServiceInfo();
    
    DEFINE_QUERY(qry);
    
    qry.close();
    qry.setSQL("select count(*) cnt from b_sp_service");
    qry.open();
    qry.next();
    
    m_iServiceInfoCnt = qry.field("cnt").asInteger();
    
    if (!m_iServiceInfoCnt) return;
    
    m_aServiceInfo = new TServiceInfo[m_iServiceInfoCnt];
    if (!m_aServiceInfo)
        THROW(MBC_FormatMethod+9);
    
    qry.close();
    qry.setSQL("select decode(substr(area_code,1,1),'*','*',area_code) area_code, "
        " begin_nbr, end_nbr, service_type_id, party_role_id "
        " from B_SP_SERVICE order by begin_nbr, end_nbr " );
    qry.open();
    
    // �� begin_nbr ��С�����ַ�������, �������� m_aServiceInfo ��
    for (int iPos=0; qry.next() && (iPos<m_iServiceInfoCnt); iPos++) {
        strcpy(m_aServiceInfo[iPos].m_sAreaCode, qry.field("area_code").asString());
        strcpy(m_aServiceInfo[iPos].m_sBeginNBR, qry.field("begin_nbr").asString());
        strcpy(m_aServiceInfo[iPos].m_sEndNBR, qry.field("end_nbr").asString());
        m_aServiceInfo[iPos].m_iServiceTypeID = qry.field("service_type_id").asInteger();
        m_aServiceInfo[iPos].m_iPartyRoleID = qry.field("party_role_id").asInteger();
        
        strcpy(m_aServiceInfo[iPos].m_sMaxEndNBRAhead,sMaxEndNBRAhead);
        if (strcmp(m_aServiceInfo[iPos].m_sEndNBR, sMaxEndNBRAhead)>0)
            strcpy(sMaxEndNBRAhead, m_aServiceInfo[iPos].m_sEndNBR);
    }
    
    qry.close();
}

//##ModelId=427B28560008
void FormatMethod::unloadServiceInfo()
{
    if (m_aServiceInfo) {
        delete m_aServiceInfo;
        m_aServiceInfo = NULL;
    }
}


//##ModelId=427B286F0267
bool FormatMethod::getServiceInfo(char* _sAccNBR, TServiceInfo &_info, char* _sAreaCode)
{
    if (!m_aServiceInfo)
        loadServiceInfo();
    
    int iHead = 0, iTail = m_iServiceInfoCnt-1;
    int iMid ;
    
    // �۰����, ��ͼ��m_aServiceInfo���ҵ�һ�� m_sBeginNBR == _sAccNBR ��m_aServiceInfo��¼
    // ���ܵĽ��: 
    //     1. �ҵ�, iMid ָ�� һ�� m_sBeginNBR == _sAccNBR ��m_aServiceInfo��¼
    //     2. δ�ҵ�, �������: 
    //          a. iMid ָ�����һ�� m_sBeginNBR < _sAccNBR ��m_aServiceInfo��¼
    //          b. iMid ָ����ǰһ�� m_sBeginNBR > _sAccNBR ��m_aServiceInfo��¼
    while (iHead <= iTail) {
        iMid = (iHead + iTail)/2;
        int iRet = strcmp(_sAccNBR, m_aServiceInfo[iMid].m_sBeginNBR);
        if (iRet > 0) {
            iHead = iMid+1;
            continue;
        }
        if (iRet < 0) {
            iTail = iMid-1;
            continue;
        }
        //found:
        break;
    }
    
    //Ŀ��: ��iMid ָ�� ���һ�� (m_sBeginNBR <= _sAccNBR) �� m_aServiceInfo ��¼.
    int iRet = strcmp(_sAccNBR,m_aServiceInfo[iMid].m_sBeginNBR);
    if (iRet < 0) {
        iMid--;
    }
    else if (iRet == 0) { //�α���ͼ���<����>�����Ƿ����뵱ǰm_sBeginNBR��ͬ�ļ�¼
        while (iMid<m_iServiceInfoCnt) {
            iMid++;
            if (strcmp(_sAccNBR,m_aServiceInfo[iMid].m_sBeginNBR)<0) {
                iMid--;
                break;
            }
        }   
    }
    
    // ��ǰ����ƥ��
    for ( ; iMid>=0 ; iMid--) {
        
        int iRetA = strcmp(_sAccNBR,m_aServiceInfo[iMid].m_sEndNBR);
        
        if (iRetA <= 0) { // _sAccNBR �ڵ�ǰ�Ŷ���
            
            //����Ŷβ���������,��һ����������
            if (m_aServiceInfo[iMid].m_sAreaCode[0]=='*') {
                _info = m_aServiceInfo[iMid];
                return true;
            }
            
            //��ǰ�Ŷ���Ҫ��������
            if (_sAreaCode) { //����������
                if (!strcmp(_sAreaCode, m_aServiceInfo[iMid].m_sAreaCode)) {//����һ��
                    _info = m_aServiceInfo[iMid];
                    return true;
                }
            }               
        }
        
        // �ж���ǰ�Ƿ��� m_sEndNBR ���� _sAccNBR С�ļ�¼
        if (strcmp(_sAccNBR, m_aServiceInfo[iMid].m_sMaxEndNBRAhead)>0) { //û��
            break;
        }
    }
    return false;
}

bool FormatMethod::getServiceInfo2(int _iLongGrpType, TServiceInfo2 *_pInfo, 
        char* _sNBR, char* _sStartDate, char* _sAreaCode, int iOrgIDRule, int iNetworkID,bool bIsZ)
{
    static char sTemp[100];
    char sAreaCode[20];
    
    if (!m_poOrgMgr) {
        m_poOrgMgr = new OrgMgr;
        if (!m_poOrgMgr)
            THROW(MBC_FormatMethod+10);
    }
    
    LocalHead oLocalHead;
    
    int iOrgID = 0;
    
    //first, find the org_id
    if (_iLongGrpType == LONG_GROUPTYPE_PROV) { //ʡ�ڳ�;������
        strcpy(sAreaCode,_sAreaCode);
        
        //������������������ȡOrgID
        if (iOrgIDRule == 1) {			
			if(bIsZ)
			{
				if(G_PUSERINFO->getServ_Z(G_SERV, _sAreaCode, _sNBR,_sStartDate))
				{
					iOrgID = G_PSERV->getOrgID();
				}
			}else
			{
				if(G_PUSERINFO->getServ_A(G_SERV, _sAreaCode,_sNBR, _sStartDate, iNetworkID))
				{
					iOrgID = G_PSERV->getOrgID();
				}
			}
        }
    }
    else if (_iLongGrpType == LONG_GROUPTYPE_NATION) { //���ڳ�;������
        strcpy(sAreaCode,"0");
    }
    else if (_iLongGrpType == 0){ //��;������δ��ֵ
        strcpy(sAreaCode,_sAreaCode);
    }
    else { //����: �� ���� 13�� ���� �۰�̨ �� ��;������
        strcpy(sAreaCode,"00");
    }
    
    if (!m_poLocalHeadMgr) {
        m_poLocalHeadMgr = new LocalHeadMgr;
        if (!m_poLocalHeadMgr)
            THROW(MBC_FormatMethod+11);
    }
    
    //if (!iOrgID)
    if (iOrgID <= 0) 
    {
        if (!m_poLocalHeadMgr->searchLocalHead(_sNBR, sAreaCode, _sStartDate,&oLocalHead)) {//�Ҳ�����ͷ
            //>> �ô�����
            iOrgID = 0;
            return false;       
        }    
        iOrgID = oLocalHead.getOrgID();
    }
    
    int iSrcOrgID = iOrgID;    
    
    
    //second, find the ServiceInfo by Org_id.    
    static char sOrgID[10];

    while (1) { //org_id �ɵ�ǰ���򸸽ڵ����β���
        sprintf(sOrgID,"%d",iOrgID);
#ifdef PRIVATE_MEMORY
        if (m_poServiceInfo->get(sOrgID,_pInfo)){
            _pInfo->m_iOrgID = iSrcOrgID; // OrgID �� ���ڵ��ֵ
            return true;
        }
#else
		unsigned int iOffset = 0;
		if (G_PPARAMINFO->m_poServiceInfo2Index->get(sOrgID,&iOffset))
		{
			*_pInfo = *(G_PPARAMINFO->m_poServiceInfo2List + iOffset);
			_pInfo->m_iOrgID = iSrcOrgID;// OrgID �� ���ڵ��ֵ
            return true;
        }
#endif
        iOrgID = m_poOrgMgr->getParentID(iOrgID);
        
        if (iOrgID == -1)
            break;
    }
    
    //>>δ�ҵ�ҵ��������Ϣ,����Ҫ����OrgID
    _pInfo->m_iOrgID = iSrcOrgID;
    _pInfo->m_iServiceTypeID = 0;
    _pInfo->m_iPartyRoleID = 0;
    
    return true;
}

//##ModelId=427B1EE20076
void FormatMethod::formatServiceInfo(EventSection *_poEventSection)
{
    TServiceInfo obSvcInfo;
    
    if (getServiceInfo(_poEventSection->m_sCallingNBR, obSvcInfo,
            _poEventSection->m_sCallingAreaCode)) {
        _poEventSection->m_iCallingSPTypeID = obSvcInfo.m_iPartyRoleID;
        _poEventSection->m_iCallingServiceTypeID = obSvcInfo.m_iServiceTypeID;
    }
    
    if (getServiceInfo(_poEventSection->m_sCalledNBR, obSvcInfo,
            _poEventSection->m_sCalledAreaCode)) {
        _poEventSection->m_iCalledSPTypeID = obSvcInfo.m_iPartyRoleID;
        _poEventSection->m_iCalledServiceTypeID = obSvcInfo.m_iServiceTypeID;
    }
}


void FormatMethod::formatServiceInfo2_Old(EventSection *_poEvt)
{
	static TServiceInfo2 oSvcInfo;
#ifdef PRIVATE_MEMORY   
    if (!m_poServiceInfo)
        loadServiceInfo2();    
#else
  	unsigned int iOff = 0;
	char *pKey = "m_iCalledOrgIDRule";
	if (G_PPARAMINFO->m_poServiceInfo2Index->get(pKey,&iOff))
	{
		m_iCalledOrgIDRule = (int)iOff;
	} else {
		m_iCalledOrgIDRule = 0;
	}
	
	pKey = "m_iCallingOrgIDRule";
	if (G_PPARAMINFO->m_poServiceInfo2Index->get(pKey,&iOff))
	{
		m_iCallingOrgIDRule = (int)iOff;
	} else {
		m_iCallingOrgIDRule = 0;
	}
#endif    
    if (getServiceInfo2(_poEvt->m_oEventExt.m_iCallingLongGroupTypeID,&oSvcInfo,
            _poEvt->m_sCallingNBR, _poEvt->m_sStartDate, _poEvt->m_sCallingAreaCode, 
            m_iCallingOrgIDRule, _poEvt->m_iNetworkID,false)) {
        _poEvt->m_iCallingSPTypeID = oSvcInfo.m_iPartyRoleID;
        _poEvt->m_iCallingServiceTypeID = oSvcInfo.m_iServiceTypeID;
        _poEvt->m_oEventExt.m_iCallingOrgID = oSvcInfo.m_iOrgID;
    }
    
    if (getServiceInfo2(_poEvt->m_oEventExt.m_iCalledLongGroupTypeID,&oSvcInfo,
            _poEvt->m_sCalledNBR, _poEvt->m_sStartDate, _poEvt->m_sCalledAreaCode,
            m_iCalledOrgIDRule, _poEvt->m_iNetworkID,true)) {
        _poEvt->m_iCalledSPTypeID = oSvcInfo.m_iPartyRoleID;
        _poEvt->m_iCalledServiceTypeID = oSvcInfo.m_iServiceTypeID;
        _poEvt->m_oEventExt.m_iCalledOrgID = oSvcInfo.m_iOrgID;
    }
};
void FormatMethod::formatServiceInfo2(EventSection *_poEvt)
{
    static TServiceInfo2 oSvcInfo;
#ifdef PRIVATE_MEMORY    
    if (!m_poServiceInfo)
        loadServiceInfo2();    
#else
  	unsigned int iOff = 0;
	char *pKey = "m_iCalledOrgIDRule";
	if (G_PPARAMINFO->m_poServiceInfo2Index->get(pKey,&iOff))
	{
		m_iCalledOrgIDRule = (int)iOff;
	} else {
		m_iCalledOrgIDRule = 0;
	}
	
	pKey = "m_iCallingOrgIDRule";
	if (G_PPARAMINFO->m_poServiceInfo2Index->get(pKey,&iOff))
	{
		m_iCallingOrgIDRule = (int)iOff;
	} else {
		m_iCallingOrgIDRule = 0;
	}   
#endif
	char * pBillingAreaCode;
	char * pBillingNBR;
	char * pOtherAreaCode;
	char * pOtherNBR;
	if(CALLTYPE_MTC != _poEvt->m_iCallTypeID)
	{
		pBillingAreaCode = _poEvt->m_sCallingAreaCode;
		pBillingNBR = _poEvt->m_sCallingNBR;
		pOtherAreaCode = _poEvt->m_sCalledAreaCode;
		pOtherNBR = _poEvt->m_sCalledNBR;
	}else
	{
		pBillingAreaCode = _poEvt->m_sCalledAreaCode;
		pBillingNBR = _poEvt->m_sCalledNBR;
		pOtherAreaCode = _poEvt->m_sCallingAreaCode;
		pOtherNBR = _poEvt->m_sCallingNBR;
	}
    
    if (getServiceInfo2(_poEvt->m_oEventExt.m_iCallingLongGroupTypeID,&oSvcInfo,
            pBillingNBR, _poEvt->m_sStartDate, pBillingAreaCode, 
            m_iCallingOrgIDRule, _poEvt->m_iNetworkID,false)) {
        _poEvt->m_iCallingSPTypeID = oSvcInfo.m_iPartyRoleID;
        _poEvt->m_iCallingServiceTypeID = oSvcInfo.m_iServiceTypeID;
        _poEvt->m_oEventExt.m_iCallingOrgID = oSvcInfo.m_iOrgID;
    }
    
    if (getServiceInfo2(_poEvt->m_oEventExt.m_iCalledLongGroupTypeID,&oSvcInfo,
            pOtherNBR, _poEvt->m_sStartDate,pOtherAreaCode,
            m_iCalledOrgIDRule, _poEvt->m_iNetworkID,true)) {
        _poEvt->m_iCalledSPTypeID = oSvcInfo.m_iPartyRoleID;
        _poEvt->m_iCalledServiceTypeID = oSvcInfo.m_iServiceTypeID;
        _poEvt->m_oEventExt.m_iCalledOrgID = oSvcInfo.m_iOrgID;
    }   
}

//##ModelId=427B20FD028E
void FormatMethod::formatRoamAndLongType(EventSection *_poEventSection)
{
    //if (_poEventSection->m_iCallTypeID <= 2 && strlen(_poEventSection->m_sBillingVisitAreaCode)==0)
    if (strlen(_poEventSection->m_sBillingVisitAreaCode)==0)
        strcpy(_poEventSection->m_sBillingVisitAreaCode, _poEventSection->m_sCallingAreaCode);
    
    //����Ʒѷ�����Ϊ��,��Ĭ�ϼƷѷ�����/����(������,�������Ÿ�ֵ���Ʒѷ�,�Ʒѷ�����)
    if (_poEventSection->m_sBillingNBR[0]==0) {
        strcpy(_poEventSection->m_sBillingNBR, _poEventSection->m_sCallingNBR);
        strcpy(_poEventSection->m_sBillingAreaCode, _poEventSection->m_sCallingAreaCode);
        _poEventSection->unbindUserInfo();
    }
    
    getRoamType(_poEventSection);

    if (_poEventSection->m_iCallTypeID <= 1) { //���л���
        
        //���е��õ�����!=���й��������� && Event.��������!="ͬ������"
        if (strcmp(_poEventSection->m_sBillingVisitAreaCode,_poEventSection->m_sCalledAreaCode)
                && _poEventSection->m_oEventExt.m_iRoamTypeID != ROAMTYPE_ROAM_SAME) 
        {
            //�����ĳ�;����
            getLongType(_poEventSection,_poEventSection->m_oEventExt.m_iCalledLongGroupTypeID);
        }
        else {
            _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; //��;����-�л�
        }
    }
    else //if (_poEventSection->m_iCallTypeID == 2)
    { //���л���
        //���е��õ�����!=���й��������� && Event.��������!="ͬ������"
        if (strcmp(_poEventSection->m_sBillingVisitAreaCode,_poEventSection->m_sCalledAreaCode)
                && _poEventSection->m_oEventExt.m_iRoamTypeID != ROAMTYPE_ROAM_SAME) 
        {
            //�����ĳ�;����
            getLongType(_poEventSection,_poEventSection->m_oEventExt.m_iCalledLongGroupTypeID);
        }
        else {
            _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN; //��;����-�л�
        }       
        //getLongType(_poEventSection,_poEventSection->m_oEventExt.m_iCalledLongGroupTypeID);
/*      if (���е��õ�����!=���й��������� && Event.��������!="ͬ������")
            Event.��;���� = "��;";
        Else
            Event.��;���� = "�л�";
*/  
    }
}

#ifdef PRIVATE_MEMORY
int FormatMethod::filter(EventSection *_poEventSection)
{
    if (!m_poFilterRule) {
        m_poFilterRule = new FilterRule ();
        m_poFilterRule->load ();
    }

    return m_poFilterRule->filter ((StdEvent *)_poEventSection);
}
#else
int FormatMethod::filter(EventSection *_poEventSection)
{
    FilterRuleCheckValue mFilterRuleCheckValue;
    return mFilterRuleCheckValue.filter ((StdEvent *)_poEventSection);
}
#endif

int FormatMethod::customFilter(EventSection *_poEventSection)
{
    if (!m_poCustomFilter) {
        m_poCustomFilter = new CustomFilter ();
        m_poCustomFilter->load ();
    }

    return m_poCustomFilter->filter ((StdEvent *)_poEventSection);
}


//##ModelId=427C9762021B
void FormatMethod::getLongType(EventSection *_poEventSection, int _iLongGroupType)
{
    if (LongTypeMgr::getLongType (_poEventSection))
        return;
    
    switch (_iLongGroupType) {
      case  LONG_GROUPTYPE_PROV :
        _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_PROV ;
        break;
      case  LONG_GROUPTYPE_NATION :
        _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_NATION ;
        break;
      case  LONG_GROUPTYPE_INTER :
        _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_INTER ;
        break;
      case  LONG_GROUPTYPE_THIRT :
        _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_THIRT ;
        break;
      case  LONG_GROUPTYPE_AMERC :
        _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_AMERC ;
        break;

    // b_area_code �������Ŷ�Ӧ�ĳ�;�����Ϳ��ܻ�Ѹۡ��ġ�̨�ֿ�
      case  LONG_GROUPTYPE_KONG : //�۰�̨��;������
        _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_KONG ;
        break;

      case  LONG_GROUPTYPE_XK :   //��۳�;������
        _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN_XK ;
        break;
      case  LONG_GROUPTYPE_AM :   //���ų�;������
        _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN_AM ;
        break;
      case  LONG_GROUPTYPE_TW :   //̨�峤;������
        _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN_TW ;
        break;
      case  LONG_GROUPTYPE_HS :   //̨�峤;������
        _poEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN_HS ;
        break;
    }
}


//##ModelId=427C96D00121
void FormatMethod::getRoamType(EventSection *_poEventSection)
{
    // �Ʒѷ����õ����� == �Ʒѷ�����������
    if (!strcmp(_poEventSection->m_sBillingVisitAreaCode,_poEventSection->m_sBillingAreaCode)) {
        _poEventSection->m_oEventExt.m_iRoamTypeID = ROAMTYPE_NOT_ROAM; //��������-������
        return;
    }
    else {
        //_poEventSection->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM; //��������-����
    }
    
    //�ж��߽�����,������δ��  //>...
    if (0) {
        _poEventSection->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM_BORD; //��������-�߽�����
        
        // �Ʒѵ��õ����� <- �Ʒѹ���������
        strcpy(_poEventSection->m_sBillingVisitAreaCode,_poEventSection->m_sBillingAreaCode);
        return;
    }
    
    //�ж�ͬ���������� //>...
    //���з����õ�����(�����л��ǼƷѷ�?)==���е��õ����� && ���е��õ�����!=���й���������
    if (!strcmp(_poEventSection->m_sBillingVisitAreaCode,"" ) 
            && strcmp("","" )) {
        _poEventSection->m_oEventExt.m_iRoamTypeID = ROAMTYPE_ROAM_SAME; //��������-ͬ������
        return;
    }
    
    //>... ������������� ���� ǰ̨��������ָ��
}


//##ModelId=4281CFED0068
bool FormatMethod::formatLongEventType(EventSection *_poEventSection)
{
    LongEventTypeRule *pRule;
    
    if (LongEventTypeRuleMgr::getRule(_poEventSection,&pRule)) {

        #ifdef RECORD_EVENT_RULE
            sprintf(g_sFormatRecord,"%s\n LongID:%d",g_sFormatRecord,pRule->getRuleID());
        #endif
            
        _poEventSection->m_iEventTypeID = pRule->getOutEventTypeID();
    }
    return true;
}

//##ModelId=4281D05B028D
bool FormatMethod::formatLocalEventType(EventSection *_poEventSection)
{
    LocalEventTypeRule *pRule;
    
    if (LocalEventTypeRuleMgr::getRule(_poEventSection,&pRule)) {
        
        #ifdef RECORD_EVENT_RULE
            sprintf(g_sFormatRecord,"%s\n LocalID:%d",g_sFormatRecord,pRule->getRuleID());
        #endif
        
        _poEventSection->m_iEventTypeID = pRule->getOutEventTypeID();
    }
    return true;
}

bool  FormatMethod::getPhysicsNbr(EventSection *_poEventSection)
{
    char const * OrgCallingphNBR;
    char const * OrgCalledphNBR;
    char const * ThirdPartyphNBR;
    
    OrgCallingphNBR=G_PUSERINFO->convertAccNBR(_poEventSection->m_sOrgCallingNBR,25,1,_poEventSection->m_sStartDate);
    OrgCalledphNBR=G_PUSERINFO->convertAccNBR(_poEventSection->m_sOrgCalledNBR,25,1,_poEventSection->m_sStartDate);
    ThirdPartyphNBR=G_PUSERINFO->convertAccNBR(_poEventSection->m_sThirdPartyNBR,25,1,_poEventSection->m_sStartDate);
    

    if(_poEventSection->m_sOrgCallingPhysicalNBR[0]=='\0')
    {
        if(OrgCallingphNBR!=NULL)
        {
            strcpy(_poEventSection->m_sOrgCallingPhysicalNBR,OrgCallingphNBR);
        }
        else  
        {
            strcpy(_poEventSection->m_sOrgCallingPhysicalNBR,_poEventSection->m_sOrgCallingNBR);
        }
    }    
      

    if(_poEventSection->m_sOrgCalledPhysicalNBR[0]=='\0')
    { 
        if(OrgCalledphNBR!=NULL)
        {
            strcpy(_poEventSection->m_sOrgCalledPhysicalNBR,OrgCalledphNBR);
        }
        else 
        {
            strcpy(_poEventSection->m_sOrgCalledPhysicalNBR,_poEventSection->m_sOrgCalledNBR);
        }
    }
      
    
    if(_poEventSection->m_sThirdPartyPhysicalNBR[0]=='\0')
    {
        if(ThirdPartyphNBR!=NULL)
        {
            strcpy(_poEventSection->m_sThirdPartyPhysicalNBR,ThirdPartyphNBR);
        }
        else 
        {
            strcpy(_poEventSection->m_sThirdPartyPhysicalNBR,_poEventSection->m_sThirdPartyNBR);
        }
    }

    strcpy(_poEventSection->m_sCallingPhysicalNBR,_poEventSection->m_sOrgCallingPhysicalNBR);
    strcpy(_poEventSection->m_sCalledPhysicalNBR,_poEventSection->m_sOrgCalledPhysicalNBR);
      
    return true; 
}


//## �����к�����������ת�����߼����� (Դ�Ժ�������)
void FormatMethod::convertPhyNbrToLogicNbr (EventSection *pEvt)
{
    if (!pEvt || !pEvt->m_sCallingNBR[0])
        return;
    
    if (!G_PUSERINFO->getServ (G_SERV, pEvt->m_sCallingNBR, pEvt->m_sStartDate, 
            pEvt->m_iNetworkID, true))
        return;
    
    static char const *pNbr;
    
    pNbr = G_SERV.getAccNBR (25, pEvt->m_sStartDate);
    
    if (!pNbr) return;
    
    strcpy (pEvt->m_sCallingNBR, pNbr);
}

//## �����к�����ƶ�����ת�����߼����� (Դ�Ժ�������)
void FormatMethod::convertMobNbrToLogicNbr (EventSection *pEvt)
{
    if (!pEvt || !pEvt->m_sCallingNBR[0])
        return;
    
    if (!G_PUSERINFO->getServ (G_SERV, pEvt->m_sCallingNBR, pEvt->m_sStartDate, 
            pEvt->m_iNetworkID))
        return;
    
    static char const *pNbr;
    
    pNbr = G_SERV.getAccNBR (25, pEvt->m_sStartDate);
    
    if (!pNbr) return;
    
    strcpy (pEvt->m_sCallingNBR, pNbr);
}

//��ʼ���м̼Ʒ�
void FormatMethod::loadTrunkBilling()
{
    unloadTrunkBilling();
    m_poTrunkBilling = new KeyTree<TrunkBilling>;
    
    DEFINE_QUERY(qry);
        
    qry.close();

#ifdef  DEF_SICHUAN
//////�Ĵ�����Ҫ�õ��ɼ������м̼Ʒ�

    qry.setSQL(" select switch_id, gather_pot, trunk_code, "
        " to_char(eff_date,'yyyymmddhh24miss') eff_date, "
        " to_char(exp_date,'yyyymmddhh24miss') exp_date, "
        " acc_nbr, to_number(trunk_side) trunk_side, nvl(switch_id,-1)||'+'|| nvl(gather_pot,-1) || '+' ||trunk_code skey "
        " from b_trunk_billing order by switch_id " );
  
    qry.open();
    
    while (qry.next()) {
        TrunkBilling oTrunkBilling;
        char sKey[30]; memset(sKey, 0, sizeof(sKey));
        oTrunkBilling.m_iSwitch_id = qry.field("switch_id").asInteger();
        oTrunkBilling.m_iGatherPot = qry.field("gather_pot").asInteger();//////gather_pot
        strcpy(oTrunkBilling.m_sTrunk_Code, qry.field("trunk_code").asString());
        strcpy(oTrunkBilling.m_sEffDate, qry.field("eff_date").asString());
        strcpy(oTrunkBilling.m_sExpDate, qry.field("exp_date").asString());
        strcpy(oTrunkBilling.m_sAccNBR, qry.field("acc_nbr").asString());
        oTrunkBilling.m_iTrunkSide = qry.field("trunk_side").asInteger();
        strcpy(sKey, qry.field("skey").asString());
        
        m_poTrunkBilling->add(sKey, oTrunkBilling);
    }    


  #else     

    qry.setSQL(" select  switch_id, trunk_code, "
        " to_char(eff_date,'yyyymmddhh24miss') eff_date, "
        " to_char(exp_date,'yyyymmddhh24miss') exp_date, "
        " acc_nbr, to_number(trunk_side) trunk_side, switch_id||'+'||trunk_code skey "
        " from b_trunk_billing order by switch_id " );
    qry.open();
    
    while (qry.next()) {
        TrunkBilling oTrunkBilling;
        char sKey[30]; memset(sKey, 0, sizeof(sKey));
        oTrunkBilling.m_iSwitch_id = qry.field(0).asInteger();
        strcpy(oTrunkBilling.m_sTrunk_Code, qry.field(1).asString());
        strcpy(oTrunkBilling.m_sEffDate, qry.field(2).asString());
        strcpy(oTrunkBilling.m_sExpDate, qry.field(3).asString());
        strcpy(oTrunkBilling.m_sAccNBR, qry.field(4).asString());
        oTrunkBilling.m_iTrunkSide = qry.field(5).asInteger();
        strcpy(sKey, qry.field(6).asString());
        
        m_poTrunkBilling->add(sKey, oTrunkBilling);
    }
    
    
  #endif

   
    qry.close();
}


void FormatMethod::unloadTrunkBilling()
{
    if (m_poTrunkBilling) {
        delete m_poTrunkBilling;
        m_poTrunkBilling = NULL;
    }
}

#ifdef PRIVATE_MEMORY
bool FormatMethod::getTrunkBilling(int _iSwitchID, char *_sTrunkInCode, char *_sTrunkOutCode,char *_sDate,TrunkBilling *_oTrunk)
{
    if(!m_poTrunkBilling)
        loadTrunkBilling();
    
    bool bRet = false;
    char sKey[30];
    
    //�������м̲���
    sprintf(sKey, "%d+%s", _iSwitchID, _sTrunkInCode);    
    bRet = m_poTrunkBilling->get(sKey, _oTrunk);

    if (bRet) 
    {   
        if (strcmp(_sDate,_oTrunk->m_sEffDate)>=0 && strcmp(_sDate,_oTrunk->m_sExpDate) < 0 )
        {
            //˫���м̻����м�
            if (_oTrunk->m_iTrunkSide == 1 || _oTrunk->m_iTrunkSide == 2)
                return true;
        }
    }               

    //�Ҳ��������ó��м̲���
    sprintf(sKey,"%d+%s", _iSwitchID, _sTrunkOutCode);    
    bRet = m_poTrunkBilling->get(sKey, _oTrunk);

    if (bRet) 
    {   
        if (strcmp(_sDate,_oTrunk->m_sEffDate)>=0 && strcmp(_sDate,_oTrunk->m_sExpDate) < 0 )
        {
            //˫���м̻���м�
            if (_oTrunk->m_iTrunkSide == 1 || _oTrunk->m_iTrunkSide == 3)
                return true;
        }
    }   
    
    return false;
}
#else
bool FormatMethod::getTrunkBilling(int _iSwitchID, char *_sTrunkInCode, char *_sTrunkOutCode,char *_sDate,TrunkBilling *_oTrunk)
{
    bool bRet = false;
    char sKey[30];
    
    //�������м̲���
    sprintf(sKey, "%d+%s", _iSwitchID, _sTrunkInCode);    
	unsigned int iOffset = 0;    
    bRet = G_PPARAMINFO->m_poTrunkBillingIndex->get(sKey, &iOffset);
    if (bRet) 
    {   
		*_oTrunk = *(G_PPARAMINFO->m_poTrunkBillingList + iOffset);
        if (strcmp(_sDate,_oTrunk->m_sEffDate)>=0 && strcmp(_sDate,_oTrunk->m_sExpDate) < 0 )
        {
            //˫���м̻����м�
            if (_oTrunk->m_iTrunkSide == 1 || _oTrunk->m_iTrunkSide == 2)
                return true;
        }
    }               
    //�Ҳ��������ó��м̲���
    sprintf(sKey,"%d+%s", _iSwitchID, _sTrunkOutCode);    
    bRet = G_PPARAMINFO->m_poTrunkBillingIndex->get(sKey, &iOffset);
    if (bRet) 
    {   
		*_oTrunk = *(G_PPARAMINFO->m_poTrunkBillingList + iOffset);
        if (strcmp(_sDate,_oTrunk->m_sEffDate)>=0 && strcmp(_sDate,_oTrunk->m_sExpDate) < 0 )
        {
            //˫���м̻���м�
            if (_oTrunk->m_iTrunkSide == 1 || _oTrunk->m_iTrunkSide == 3)
                return true;
        }
    }    
    return false;
}
#endif



////�Ĵ���Ҫʹ�òɼ������м̼Ʒ�(�滻����)
#ifdef PRIVATE_MEMORY
bool FormatMethod::getTrunkBilling(int _iSwitchID, int _iGatherPot,
    char *_sTrunkInCode, char *_sTrunkOutCode,char *_sDate, TrunkBilling *_oTrunk)
{
    if(!m_poTrunkBilling)
        loadTrunkBilling();
    
    bool bRet = false;
    char sKey[30];

////��ʹ�ý�����+�ɼ���+�м�.Ȼ�󵥶��ý�����,�ٵ����òɼ���,
    
    //�������м̲���
    sprintf(sKey, "%d+%d+%s", _iSwitchID, _iGatherPot,  _sTrunkInCode);     
    bRet = m_poTrunkBilling->get(sKey, _oTrunk);
    
   /* if(bRet==false)
        {
        sprintf(sKey, "%d+%d+%s", _iSwitchID, -1,   _sTrunkInCode);      
        bRet = m_poTrunkBilling->get(sKey, _oTrunk);
        if(bRet==false)
            {
            sprintf(sKey, "%d+%d+%s",-1,  _iGatherPot,   _sTrunkInCode);    
            bRet = m_poTrunkBilling->get(sKey, _oTrunk);        
            }
        }
    */

    if (bRet) 
    {   
        if (strcmp(_sDate,_oTrunk->m_sEffDate)>=0 && strcmp(_sDate,_oTrunk->m_sExpDate) < 0 )
        {
            //˫���м̻����м�
            if (_oTrunk->m_iTrunkSide == 1 || _oTrunk->m_iTrunkSide == 2)
                return true;
        }
    }               


    //�Ҳ��������ó��м̲���
    sprintf(sKey, "%d+%d+%s", _iSwitchID, _iGatherPot,  _sTrunkOutCode);     
    bRet = m_poTrunkBilling->get(sKey, _oTrunk);
    /*
    if(bRet==false)
        {
        sprintf(sKey, "%d+%d+%s", _iSwitchID,  -1,  _sTrunkOutCode);      
        bRet = m_poTrunkBilling->get(sKey, _oTrunk);
        if(bRet==false)
            {
            sprintf(sKey, "%d+%d+%s", -1, _iGatherPot,  _sTrunkOutCode);    
            bRet = m_poTrunkBilling->get(sKey, _oTrunk);        
            }
        }

    */

    if (bRet) 
    {   
        if (strcmp(_sDate,_oTrunk->m_sEffDate)>=0 && strcmp(_sDate,_oTrunk->m_sExpDate) < 0 )
        {
            //˫���м̻���м�
            if (_oTrunk->m_iTrunkSide == 1 || _oTrunk->m_iTrunkSide == 3)
                return true;
        }
    }   

    
    return false;
}
#else
bool FormatMethod::getTrunkBilling(int _iSwitchID, int _iGatherPot,
    char *_sTrunkInCode, char *_sTrunkOutCode,char *_sDate, TrunkBilling *_oTrunk)
{    
    bool bRet = false;
    char sKey[30];

////��ʹ�ý�����+�ɼ���+�м�.Ȼ�󵥶��ý�����,�ٵ����òɼ���,
    
    //�������м̲���
	unsigned int iOffset = 0; 
    sprintf(sKey, "%d+%d+%s", _iSwitchID, _iGatherPot,  _sTrunkInCode);     
    bRet = G_PPARAMINFO->m_poTrunkBillingIndex->get(sKey, &iOffset);

    if (bRet) 
    {   
		*_oTrunk = *(G_PPARAMINFO->m_poTrunkBillingList + iOffset);
        if (strcmp(_sDate,_oTrunk->m_sEffDate)>=0 && strcmp(_sDate,_oTrunk->m_sExpDate) < 0 )
        {
            //˫���м̻����м�
            if (_oTrunk->m_iTrunkSide == 1 || _oTrunk->m_iTrunkSide == 2)
                return true;
        }
    }               

    //�Ҳ��������ó��м̲���
    sprintf(sKey, "%d+%d+%s", _iSwitchID, _iGatherPot,  _sTrunkOutCode);     
    bRet = G_PPARAMINFO->m_poTrunkBillingIndex->get(sKey, &iOffset);

    if (bRet) 
    {   
		*_oTrunk = *(G_PPARAMINFO->m_poTrunkBillingList + iOffset);
        if (strcmp(_sDate,_oTrunk->m_sEffDate)>=0 && strcmp(_sDate,_oTrunk->m_sExpDate) < 0 )
        {
            //˫���м̻���м�
            if (_oTrunk->m_iTrunkSide == 1 || _oTrunk->m_iTrunkSide == 3)
                return true;
        }
    }   
    return false;
}
#endif




bool FormatMethod::formatTrunkAccNBR(EventSection *_poEvt)
{

    TrunkBilling oTrunkBilling;

#ifdef  DEF_SICHUAN    
////// _poEvt->m_iOffers[3],
if( getTrunkBilling(
    _poEvt->m_iSwitchID,atoi( _poEvt->m_sReservedField[3] ), 
    _poEvt->m_sTrunkIn,_poEvt->m_sTrunkOut,_poEvt->m_sStartDate, &oTrunkBilling)  )
{
    strcpy(_poEvt->m_sBillingNBR, oTrunkBilling.m_sAccNBR);
    _poEvt->unbindUserInfo();
    
    return true;
}

#else

    if (getTrunkBilling(_poEvt->m_iSwitchID,_poEvt->m_sTrunkIn,_poEvt->m_sTrunkOut,_poEvt->m_sStartDate, &oTrunkBilling))
    {
       strcpy(_poEvt->m_sBillingNBR, oTrunkBilling.m_sAccNBR);
       _poEvt->unbindUserInfo();
       
        #ifdef DEF_SHANXI
		#ifdef PRIVATE_MEMORY
        static HashList<int> *  pHSpecProduct=NULL;
        if(!pHSpecProduct){
            pHSpecProduct = new HashList<int>(20);
            DEFINE_QUERY(qry);
            qry.close();
            qry.setSQL("select product_id, state from b_spec_trunk_product");
            qry.open();
        	while( qry.next() ){
        	    if( qry.field(1).asString()[2] == 'A'){
        	        pHSpecProduct->add( qry.field(0).asInteger(), 1 );
        	    }
        	}
        	qry.close();
            	
        }        
            //add by fangzj 2006.12.13
            //���ز���Ļ�������;ֱ��ҵ���ԼƷѺ����Ӫҵ�����ж�ͨ������[Ŀǰ��oTrunkBilling.m_sAccNBR������]
            int    productId=0;
            int    iTemp=0;
            if (G_PUSERINFO->getServ (G_SERV, oTrunkBilling.m_sAccNBR, _poEvt->m_sStartDate)) 
            {   
                //����Ʒ�ǻز�ҵ��,��������ȡ�ƷѺ�����,���к���ȡ�ƷѺ���
                productId=G_SERV.getProductID(_poEvt->m_sStartDate);    
                ////if ((productId!=-1)&&(productId!=708))    
                if ((productId!=-1)&&(! pHSpecProduct->get(productId, &iTemp) ) )
                {  
                   //���������š����к��롢���г�;��(ǿ�ó�ʡ�ڳ�;)     
                   strncpy(_poEvt->m_sCallingAreaCode, oTrunkBilling.m_sAccNBR,4);
                   _poEvt->m_sCallingAreaCode[4]='\0';
                   
                   strcpy(_poEvt->m_sCallingNBR,oTrunkBilling.m_sAccNBR+4);
                   
                   _poEvt->m_oEventExt.m_iCallingLongGroupTypeID=10;
                }
            }
		#else
			//���ز���Ļ�������;ֱ��ҵ���ԼƷѺ����Ӫҵ�����ж�ͨ������[Ŀǰ��oTrunkBilling.m_sAccNBR������]
            int    productId=0;
            int    iTemp=0;
			unsigned int    iSHMTemp=0;
            if (G_PUSERINFO->getServ (G_SERV, oTrunkBilling.m_sAccNBR, _poEvt->m_sStartDate)) 
            {   
                //����Ʒ�ǻز�ҵ��,��������ȡ�ƷѺ�����,���к���ȡ�ƷѺ���
                productId=G_SERV.getProductID(_poEvt->m_sStartDate);    
                ////if ((productId!=-1)&&(productId!=708))    
                if ((productId!=-1)&&(! G_PPARAMINFO->m_poSpecTrunkProductIndex->get(productId, &iSHMTemp) ) )
                {  
                   //���������š����к��롢���г�;��(ǿ�ó�ʡ�ڳ�;)     
                   strncpy(_poEvt->m_sCallingAreaCode, oTrunkBilling.m_sAccNBR,4);
                   _poEvt->m_sCallingAreaCode[4]='\0';
                   
                   strcpy(_poEvt->m_sCallingNBR,oTrunkBilling.m_sAccNBR+4);
                   
                   _poEvt->m_oEventExt.m_iCallingLongGroupTypeID=10;
                }
            }
		#endif
        #endif  
            
        #ifdef DEF_LIAONING
            //ԭʼ������Ϊ���к���
            strcpy(_poEvt->m_sCallingNBR, _poEvt->m_sOrgCallingNBR);

            //�м̺����������Ϊ��������
            if (strncmp(oTrunkBilling.m_sAccNBR, "024", 3)==0)
            {
                strncpy(_poEvt->m_sCallingAreaCode, oTrunkBilling.m_sAccNBR, 3);
                _poEvt->m_sCallingAreaCode[3]='\0';
            }   
            else
            {
                strncpy(_poEvt->m_sCallingAreaCode, oTrunkBilling.m_sAccNBR, 4);
                _poEvt->m_sCallingAreaCode[4]='\0';
            }   

            _poEvt->m_oEventExt.m_iCallingLongGroupTypeID=10;
        #endif  
                
        return true;
    }
#endif

    return false;
}

void FormatMethod::unloadSwitchItemType()
{
    if (m_poSwitchItemTypeTree) {
        delete m_poSwitchItemTypeTree;
        m_poSwitchItemTypeTree = NULL;
    }
}

void FormatMethod::loadSwitchItemType()
{
    unloadSwitchItemType();
    
    m_poSwitchItemTypeTree = new KeyTree<SwitchItemType>;
    
    DEFINE_QUERY(qry);
    
    qry.close();
    qry.setSQL(" select switch_id||'+'||nvl(tariff_type,-1)||bill_mode skey, switch_id,  "
        " nvl(tariff_type,-1) tariff_type, event_type_id, acct_item_type_id, bill_mode "
        " from b_switch_item_type ");
    qry.open();
    
    while (qry.next()) {
        SwitchItemType oSwitchItemType;
        char sKey[30];
        oSwitchItemType.m_iSwitchID = qry.field("switch_id").asInteger();
        oSwitchItemType.m_iTariffType = qry.field("tariff_type").asInteger();
        oSwitchItemType.m_iEventTypeID = qry.field("event_type_id").asInteger();
        oSwitchItemType.m_iAcctItemTypeID = qry.field("acct_item_type_id").asInteger();
        oSwitchItemType.m_iBillMode = qry.field("bill_mode").asInteger();       
        strcpy(sKey, qry.field("skey").asString());

        m_poSwitchItemTypeTree->add(sKey, oSwitchItemType);
    }
    
    qry.close();
}

#ifdef PRIVATE_MEMORY
void FormatMethod::formatSwitchItem(EventSection *_pEvt)
{
    if (!m_poSwitchItemTypeTree)
        loadSwitchItemType();
            
    char sKey[100] = {0};   
    bool bRet = false;
    SwitchItemType oSwitchItemType;

    //�����ֶ�2������Ϊtariff_type
    sprintf(sKey,"%d+%s%s", _pEvt->m_iSwitchID, _pEvt->m_sReservedField[1], _pEvt->m_sReservedField[0]);

    bRet = m_poSwitchItemTypeTree->get(sKey, &oSwitchItemType);

    if (bRet) {
        _pEvt->m_iEventTypeID = oSwitchItemType.m_iEventTypeID;
        _pEvt->m_oChargeInfo[0].m_iAcctItemTypeID = oSwitchItemType.m_iAcctItemTypeID;
        return;
    } 
    else  //�ÿյķ������Ͳ�
    {
        sprintf(sKey,"%d+-1%s", _pEvt->m_iSwitchID, _pEvt->m_sReservedField[0]);
    
        bRet = m_poSwitchItemTypeTree->get(sKey, &oSwitchItemType);
        
        if (bRet) {
            _pEvt->m_iEventTypeID = oSwitchItemType.m_iEventTypeID;
            _pEvt->m_oChargeInfo[0].m_iAcctItemTypeID = oSwitchItemType.m_iAcctItemTypeID;
        }   
    }
    return;
}
#else
void FormatMethod::formatSwitchItem(EventSection *_pEvt)
{
    char sKey[100] = {0};   
    bool bRet = false;
    SwitchItemType oSwitchItemType;

    //�����ֶ�2������Ϊtariff_type
    sprintf(sKey,"%d+%s%s", _pEvt->m_iSwitchID, _pEvt->m_sReservedField[1], _pEvt->m_sReservedField[0]);
	unsigned int iOffset = 0;
    bRet = G_PPARAMINFO->m_poSwitchItemTypeIndex->get(sKey, &iOffset);

    if (bRet) {
		oSwitchItemType = *(G_PPARAMINFO->m_poSwitchItemTypeList + iOffset);
        _pEvt->m_iEventTypeID = oSwitchItemType.m_iEventTypeID;
        _pEvt->m_oChargeInfo[0].m_iAcctItemTypeID = oSwitchItemType.m_iAcctItemTypeID;
        return;
    } 
    else  //�ÿյķ������Ͳ�
    {
        sprintf(sKey,"%d+-1%s", _pEvt->m_iSwitchID, _pEvt->m_sReservedField[0]);
    
        bRet = G_PPARAMINFO->m_poSwitchItemTypeIndex->get(sKey, &iOffset);
        
        if (bRet) {
			oSwitchItemType = *(G_PPARAMINFO->m_poSwitchItemTypeList + iOffset);
            _pEvt->m_iEventTypeID = oSwitchItemType.m_iEventTypeID;
            _pEvt->m_oChargeInfo[0].m_iAcctItemTypeID = oSwitchItemType.m_iAcctItemTypeID;
        }   
    }
    return;
}
#endif


void FormatMethod::loadLocalAreaCode()
{
    DEFINE_QUERY (qry) ;
    m_strLocalAreaCode.clear();

    //���SQL���
    string sSql = "select area_code from b_area_code where long_group_type_id=10 order by area_code";

    qry.setSQL (sSql.c_str());
    qry.open ();

    while(qry.next ())
        m_strLocalAreaCode.push_back(qry.field(0).asString());

    qry.close ();
}

#ifdef PRIVATE_MEMORY
void FormatMethod::formatLocalAreaCode(EventSection *_poEventSection)
{
    vector<string>::iterator itr;
    char sTemp[33];
    
    if (m_strLocalAreaCode.empty())
        loadLocalAreaCode();
    
    //ѭ��������+�����������
    for (itr=m_strLocalAreaCode.begin(); itr!=m_strLocalAreaCode.end(); itr++)
    {
        strcpy(sTemp, (*itr).c_str());
        strcat(sTemp, _poEventSection->m_sBillingNBR);
    
        //�ҵ����ϣ������Ÿ�ֵ�Ʒ�����
        if (G_PUSERINFO->getServ (G_SERV, sTemp, _poEventSection->m_sStartDate, _poEventSection->m_iNetworkID)) {
            strcpy(_poEventSection->m_sBillingAreaCode, (*itr).c_str());
            _poEventSection->unbindUserInfo();
            break;
        }
    }
}
#else
void FormatMethod::formatLocalAreaCode(EventSection *_poEventSection)
{
    char sTemp[33];
    
    if (G_PPARAMINFO->m_poStrLocalAreaCodeData->getCount() == 0)
		return;
    
    //ѭ��������+�����������
	unsigned int i = 1;
    for (; i<=G_PPARAMINFO->m_poStrLocalAreaCodeData->getCount(); i++)
    {
        strcpy(sTemp, (G_PPARAMINFO->m_poStrLocalAreaCodeList + i)->m_sAreaCode);
        strcat(sTemp, _poEventSection->m_sBillingNBR);
    
        //�ҵ����ϣ������Ÿ�ֵ�Ʒ�����
        if (G_PUSERINFO->getServ (G_SERV, sTemp, _poEventSection->m_sStartDate, _poEventSection->m_iNetworkID)) {
            strcpy(_poEventSection->m_sBillingAreaCode, (G_PPARAMINFO->m_poStrLocalAreaCodeList + i)->m_sAreaCode);
            _poEventSection->unbindUserInfo();
            break;
        }
    }
}
#endif


//add by yangks 2006.12.31  �������μ��㹦��(�ӱ�����)
void FormatMethod::loadTimesType()
{
    unloadTimesType();
    
    m_poTimesType = new KeyTree<int>;
    
    DEFINE_QUERY(qry);
    
    int iEventTypeID;
    int iTimesType;
    
    qry.close();
    qry.setSQL("select event_type_id,times_type from b_times_type ");
    qry.open();
    
    while (qry.next()) 
    {
        iEventTypeID = qry.field("event_type_id").asInteger();
        iTimesType = qry.field("times_type").asInteger();
        m_poTimesType->add(iEventTypeID,iTimesType);
    }
    
    qry.close();

}

//add by yangks 2006.12.31  �������μ��㹦��(�ӱ�����)
void FormatMethod::unloadTimesType()
{
    if (m_poTimesType) {
        delete m_poTimesType;
        m_poTimesType = NULL;
    }
}

//add by yangks 2006.12.31  �������μ��㹦��(�ӱ�����)
#ifdef PRIVATE_MEMORY
void FormatMethod::formatTimes(EventSection *_pEvt)
{
    if (!m_poTimesType)
        loadTimesType();
            
    int iType;   
    bool bRet = false;
    
    bRet = m_poTimesType->get(_pEvt->m_iEventTypeID,&iType);

    if (bRet) 
    {
        switch(iType)
        {
        //���ڣ�
        case 1:
            if(_pEvt->m_lDuration <= 180)
            {
                _pEvt->m_iTimes = 2; 
            }
            else
            {
                _pEvt->m_iTimes = (_pEvt->m_lDuration+59)/60-1 ; 
            }
            break;
        //����
        case 2:
            _pEvt->m_iTimes = (_pEvt->m_lDuration+59)/60 ;
            break;            
        default:
            _pEvt->m_iTimes = 0;
            break;
        }
    } 
    
    return;
}
#else 
void FormatMethod::formatTimes(EventSection *_pEvt)
{     
    unsigned int iType;   
    bool bRet = false;
    bRet = G_PPARAMINFO->m_poTimesTypeIndex->get(_pEvt->m_iEventTypeID,&iType);
	
    if (bRet) 
    {
        switch(iType)
        {
        //���ڣ�
        case 1:
            if(_pEvt->m_lDuration <= 180)
            {
                _pEvt->m_iTimes = 2; 
            }
            else
            {
                _pEvt->m_iTimes = (_pEvt->m_lDuration+59)/60-1 ; 
            }
            break;
        //����
        case 2:
            _pEvt->m_iTimes = (_pEvt->m_lDuration+59)/60 ;
            break;            
        default:
            _pEvt->m_iTimes = 0;
            break;
        }
    } 
    return;
}
#endif

int FormatMethod::getSwitchTypeID(int _iSwitchID)
{
    TSwitchInfo obSwitchInfo;
    //���ݽ�������Ĭ������
    if (getSwitchInfoBySwitchID(_iSwitchID, &obSwitchInfo)) {
        return obSwitchInfo.m_iSwitchTypeID;
    }
    
    return -1;
}

