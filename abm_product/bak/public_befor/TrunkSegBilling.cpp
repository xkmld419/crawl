/*VER: 1*/ 
#include "TrunkSegBilling.h"
#ifndef PRIVATE_MEMORY
#include "ParamInfoInterface.h"
#endif
bool SegBillingMgr::m_bInited = false;

KeyTree<TrunkTreeNode *> * SegBillingMgr::m_pSwtichTrunkTree = NULL;

KeyTree<TrunkTreeNode *> * SegBillingMgr::m_pSwtichTree = NULL;

TrunkTreeNode * SegBillingMgr::m_pNolimitNode = NULL;


void SegBillingMgr::load ()
{
    if (!m_bInited)
        m_bInited = true;
    else  return;
    
    DEFINE_QUERY (qry);
    int iCnt;
    
    // ������/�м� ��Ϊ�յ����
    qry.setSQL ("select count(*) from b_pn_seg_billing"
            " where switch_id is null and trunk_in is null");
    qry.open ();
    qry.next ();
    iCnt = qry.field(0).asInteger();
    qry.close();
    if (iCnt)
    {
        m_pNolimitNode = new TrunkTreeNode;
        
        loadTrunkSeg (m_pNolimitNode, "where switch_id is null and trunk_in is null");
    }
    
    
    // ��������Ϊ��, �м�Ϊ�յ����
    qry.setSQL ("select count(*) from b_pn_seg_billing"
            " where switch_id is not null and trunk_in is null");
    qry.open ();
    qry.next ();
    iCnt = qry.field(0).asInteger();
    qry.close ();
    
    if (iCnt)
    {
        m_pSwtichTree = new KeyTree<TrunkTreeNode *>;
        
        qry.setSQL ("select distinct switch_id from b_pn_seg_billing"
                " where switch_id is not null and trunk_in is null");
        qry.open ();
        
        while (qry.next ())
        {
            char sKey[65], sql[1024];
            
            long lSwitchID = qry.field(0).asLong();
            
            sprintf (sKey,"%ld", lSwitchID);
            
            TrunkTreeNode *pNode = new TrunkTreeNode;
            
            m_pSwtichTree->add (sKey, pNode);
            
            sprintf (sql, " where switch_id = %ld and trunk_in is null ", lSwitchID);
            
            loadTrunkSeg (pNode, sql);
        }
    }
    qry.close ();
    
    
    // ������/�м� ����Ϊ�յ����
    qry.setSQL ("select count(*) from b_pn_seg_billing"
            " where switch_id is not null and trunk_in is not null");
    qry.open ();
    qry.next ();
    iCnt = qry.field(0).asInteger();
    qry.close ();
    
    if (iCnt)
    {
        m_pSwtichTrunkTree = new KeyTree<TrunkTreeNode *>;
        
        qry.setSQL ("select distinct switch_id, trunk_in from b_pn_seg_billing"
                " where switch_id is not null and trunk_in is not null");
        qry.open ();
        
        while (qry.next ())
        {
            char sKey[65], sTrunkIn[20], sql[1024];
            
            long lSwitchID = qry.field(0).asLong();
            
            strcpy (sTrunkIn, qry.field(1).asString());
            
            sprintf (sKey,"%ld_%s", lSwitchID, sTrunkIn);
            
            TrunkTreeNode *pNode = new TrunkTreeNode;
            
            m_pSwtichTrunkTree->add (sKey, pNode);
            
            sprintf (sql, " where switch_id = %ld and trunk_in = '%s'", lSwitchID, sTrunkIn);
            
            loadTrunkSeg (pNode, sql);
        }
    }
    qry.close ();
}

void SegBillingMgr::loadTrunkSeg (TrunkTreeNode *pNode, const char *sWhere)
{
    char sMaxEndNBRAhead[MAX_CALLING_NBR_LEN] = {0};
    
    char sql[2048];
    
    DEFINE_QUERY (qry);
    
    qry.close();
    sprintf (sql, "select count(*) cnt from b_pn_seg_billing "
        " %s ", sWhere);
    qry.setSQL (sql);
    qry.open();
    qry.next();
    
    pNode->m_iSegBillingCnt = qry.field("cnt").asInteger();
    if (pNode->m_iSegBillingCnt == 0)
        throw (sql);
    
    pNode->m_pSegArray = new PNSegBilling[pNode->m_iSegBillingCnt];
    
    qry.close();
    sprintf (sql, " select  area_code, begin_acc_nbr, end_acc_nbr, acc_nbr, "
        " to_char(eff_date,'yyyymmddhh24miss') eff_date, "
        " to_char(exp_date,'yyyymmddhh24miss') exp_date, "
        " to_number(nvl(seg_flag,0)) seg_flag "
        " from b_pn_seg_billing "
        " %s "
        " order by begin_acc_nbr, end_acc_nbr ", sWhere);
    qry.setSQL (sql);
    qry.open();
    
    // �� begin_nbr ��С�����ַ�������, �������� pNode->m_pSegArray ��
    for (int iPos=0; qry.next() && (iPos<pNode->m_iSegBillingCnt); iPos++) 
    {
        strcpy (pNode->m_pSegArray[iPos].m_sAreaCode, qry.field("area_code").asString());
        strcpy (pNode->m_pSegArray[iPos].m_sBeginNBR, qry.field("begin_acc_nbr").asString());
        strcpy (pNode->m_pSegArray[iPos].m_sEndNBR, qry.field("end_acc_nbr").asString());       
        strcpy (pNode->m_pSegArray[iPos].m_sAccNBR, qry.field("acc_nbr").asString());
        strcpy (pNode->m_pSegArray[iPos].m_sEffDate, qry.field("eff_date").asString());
        strcpy (pNode->m_pSegArray[iPos].m_sExpDate, qry.field("exp_date").asString());
        pNode->m_pSegArray[iPos].m_iSegFlag = qry.field("seg_flag").asInteger();
        
        strcpy (pNode->m_pSegArray[iPos].m_sMaxEndNBRAhead, sMaxEndNBRAhead);
        
        if (strcmp (pNode->m_pSegArray[iPos].m_sEndNBR, sMaxEndNBRAhead) >0 )
            strcpy (sMaxEndNBRAhead, pNode->m_pSegArray[iPos].m_sEndNBR);
    }
    
    qry.close();
}

#ifdef PRIVATE_MEMORY
PNSegBilling * SegBillingMgr::getSegBilling (TrunkTreeNode *pNode, 
        char *_sAccNBR, char *_sAreaCode, char *_sDate)
{
    if (!pNode->m_iSegBillingCnt)
        return 0;        
    
    int iHead = 0, iTail = pNode->m_iSegBillingCnt - 1;
    int iMid ;
    
    // �۰����, ��ͼ��m_pSegArray���ҵ�һ�� m_sBeginNBR == _sAccNBR �ļ�¼
    // ���ܵĽ��: 
    //     1. �ҵ�, iMid ָ�� һ�� m_sBeginNBR == _sAccNBR �ļ�¼
    //     2. δ�ҵ�, �������: 
    //          a. iMid ָ�����һ�� m_sBeginNBR < _sAccNBR �ļ�¼
    //          b. iMid ָ����ǰһ�� m_sBeginNBR > _sAccNBR �ļ�¼
    while (iHead <= iTail) 
    {
        iMid = (iHead + iTail)/2;
        int iRet = strcmp (_sAccNBR, pNode->m_pSegArray[iMid].m_sBeginNBR);
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
    
    //Ŀ��: ��iMid ָ�� ���һ�� (m_sBeginNBR <= _sAccNBR) �ļ�¼.
    int iRet = strcmp (_sAccNBR, pNode->m_pSegArray[iMid].m_sBeginNBR);
    if (iRet < 0) {
        iMid--;
    }
    else if (iRet == 0) 
    { //�α���ͼ���<����>�����Ƿ����뵱ǰm_sBeginNBR��ͬ�ļ�¼
        //while (iMid < pNode->m_iSegBillingCnt)
        while (iMid < pNode->m_iSegBillingCnt-1) 
        {
            iMid++;
            if (strcmp(_sAccNBR,pNode->m_pSegArray[iMid].m_sBeginNBR)<0) 
            {
                iMid--;
                break;
            }
        }   
    }
    
    // ��ǰ����ƥ��
    for ( ; iMid>=0 ; iMid--) 
    {        
        int iRetA = strcmp(_sAccNBR,pNode->m_pSegArray[iMid].m_sEndNBR);
        
        if (iRetA <= 0) { // _sAccNBR �ڵ�ǰ�Ŷ���
            
            if (strcmp(_sAreaCode, pNode->m_pSegArray[iMid].m_sAreaCode)) //���Ų�һ��
                continue;
            
            if (strcmp(_sDate,pNode->m_pSegArray[iMid].m_sEffDate) *
                strcmp(_sDate,pNode->m_pSegArray[iMid].m_sExpDate) <= 0)
            {
                return  (PNSegBilling *) &(pNode->m_pSegArray[iMid]);
            }
        }
        
        // �ж���ǰ�Ƿ��� m_sEndNBR ���� _sAccNBR С�ļ�¼
        if (strcmp(_sAccNBR, pNode->m_pSegArray[iMid].m_sMaxEndNBRAhead) > 0) { //û��
            break;
        }
    }
    
    return 0;
}
#else
PNSegBilling * SegBillingMgr::getSegBilling (TrunkTreeNode *pNode, char *_sAccNBR, char *_sAreaCode, char *_sDate)
{
    if (!pNode->m_iSegBillingCnt)
        return 0;           
    int iHead = pNode->m_iSegArray, iTail = pNode->m_iSegArray + pNode->m_iSegBillingCnt - 1;
	PNSegBilling *m_pSegArray = G_PPARAMINFO->m_poPNSegBillingList;
    int iMid ;
	
    while (iHead <= iTail) 
    {
        iMid = (iHead + iTail)/2;
        int iRet = strcmp (_sAccNBR, m_pSegArray[iMid].m_sBeginNBR);
        if (iRet > 0) {
            iHead = iMid+1;
            continue;
        }
        if (iRet < 0) {
            iTail = iMid-1;
            continue;
        }
        break;
    }   
    //Ŀ��: ��iMid ָ�� ���һ�� (m_sBeginNBR <= _sAccNBR) �ļ�¼.
    int iRet = strcmp (_sAccNBR, m_pSegArray[iMid].m_sBeginNBR);
    if (iRet < 0) {
        iMid--;
    }
    else if (iRet == 0) 
    { //�α���ͼ���<����>�����Ƿ����뵱ǰm_sBeginNBR��ͬ�ļ�¼
        //while (iMid < pNode->m_iSegBillingCnt)
        while (iMid < (pNode->m_iSegArray + pNode->m_iSegBillingCnt-1)) 
        {
            iMid++;
            if (strcmp(_sAccNBR,m_pSegArray[iMid].m_sBeginNBR)<0) 
            {
                iMid--;
                break;
            }
        }   
    }    
    // ��ǰ����ƥ��
    for ( ; iMid>=pNode->m_iSegArray ; iMid--) 
    {        
        int iRetA = strcmp(_sAccNBR,m_pSegArray[iMid].m_sEndNBR);       
        if (iRetA <= 0) { // _sAccNBR �ڵ�ǰ�Ŷ���            
            if (strcmp(_sAreaCode, m_pSegArray[iMid].m_sAreaCode)) //���Ų�һ��
                continue;           
            if (strcmp(_sDate,m_pSegArray[iMid].m_sEffDate) *
                strcmp(_sDate,m_pSegArray[iMid].m_sExpDate) <= 0)
            {
                return  (PNSegBilling *) &(m_pSegArray[iMid]);
            }
        }        
        // �ж���ǰ�Ƿ��� m_sEndNBR ���� _sAccNBR С�ļ�¼
        if (strcmp(_sAccNBR, m_pSegArray[iMid].m_sMaxEndNBRAhead) > 0) { //û��
            break;
        }
    }   
    return 0;
}
#endif

#ifdef PRIVATE_MEMORY
bool SegBillingMgr::formatPNSegAccNbrEx (EventSection *poEvt)
{
  
#ifdef  ENABLE_PN_SEG_NOT_BILLING
    /////���⴦��,���ڲ��ֺ���,��Ȼ��ר���Ŷ�,���ǲ���Ҫȥ�滻��ר������
    if( SegBillingMgr::segNotBilling(
            poEvt->m_sBillingNBR,
            poEvt->m_sBillingAreaCode,
            poEvt->m_sStartDate) )
        return false;
    
#endif

    
    static PNSegBilling  *pSegBilling = NULL;
    static TrunkTreeNode *pNode;
    static char sKey[65];
    
    if (!m_bInited)
        load ();
    
    // �����ȼ����� m_pSwtichTrunkTree /m_pSwtichTree /pSegBilling
    pSegBilling = NULL;
    
    if (m_pSwtichTrunkTree)
    {
        sprintf (sKey, "%d_%s", poEvt->m_iSwitchID, poEvt->m_sTrunkIn);
        if (m_pSwtichTrunkTree->get (sKey, &pNode))
        {
            pSegBilling = getSegBilling (pNode, 
                poEvt->m_sBillingNBR, poEvt->m_sBillingAreaCode, poEvt->m_sStartDate);
        }
    }
    
    if (m_pSwtichTree && !pSegBilling)
    {
        sprintf (sKey, "%d", poEvt->m_iSwitchID);
        if (m_pSwtichTree->get (sKey, &pNode))
        {
            pSegBilling = getSegBilling (pNode, 
                poEvt->m_sBillingNBR, poEvt->m_sBillingAreaCode, poEvt->m_sStartDate);
        }
    }
    
    if (m_pNolimitNode && !pSegBilling)
    {
        pSegBilling = getSegBilling (m_pNolimitNode, 
                poEvt->m_sBillingNBR, poEvt->m_sBillingAreaCode, poEvt->m_sStartDate);
    }
    
    if (pSegBilling)
    {
        if (!pSegBilling->m_iSegFlag)
        {// ʹ��ר���Ʒ�
            strcpy (poEvt->m_sBillingNBR, pSegBilling->m_sAccNBR);
            poEvt->unbindUserInfo();
            
            return true;
        }
        else
        {// ר�������мƷѷ�ʽ����
            if (poEvt->m_iServID <=0)
            {
                poEvt->unbindUserInfo();
                
                if (poEvt->m_sBillingAreaCode[0]=='\0')
                {
                    strcpy(poEvt->m_sBillingAreaCode, poEvt->m_sCallingAreaCode);
                }                    
                if (poEvt->m_sBillingNBR[0]=='\0')
                {
                    strcpy (poEvt->m_sBillingNBR, poEvt->m_sCallingNBR);
                }
                poEvt->bindUserInfo();
            }
            if (poEvt->m_iServID >0 )
            {////�е��û� �������мƷ�
                /////���ﲻ�ò���
            }
            else
            {///�޵��û� ʹ��ר���Ʒ�
                strcpy (poEvt->m_sBillingNBR, pSegBilling->m_sAccNBR);
                poEvt->unbindUserInfo();  
                
                return true;                 
            }
        }
    }
    
    return false;
}
#else
bool SegBillingMgr::formatPNSegAccNbrEx (EventSection *poEvt)
{  
#ifdef  ENABLE_PN_SEG_NOT_BILLING
    /////���⴦��,���ڲ��ֺ���,��Ȼ��ר���Ŷ�,���ǲ���Ҫȥ�滻��ר������
    if( SegBillingMgr::segNotBilling(
            poEvt->m_sBillingNBR,
            poEvt->m_sBillingAreaCode,
            poEvt->m_sStartDate) )
        return false;
#endif 
    static PNSegBilling  *pSegBilling = NULL;
    static TrunkTreeNode *pNode;
    static char sKey[65];    
	TrunkTreeNode *pNolimitNode = 0;
	TrunkTreeNode *pNodeList = G_PPARAMINFO->m_poTrunkTreeNodeList;
    // �����ȼ����� m_pSwtichTrunkTree /m_pSwtichTree /pSegBilling
    pSegBilling = NULL;
    sKey[0] = '\0';
    sprintf (sKey, "%d_%s", poEvt->m_iSwitchID, poEvt->m_sTrunkIn);
	unsigned int iOffset = 0;
	if (G_PPARAMINFO->m_pSwtichTrunkTree->get (sKey, &iOffset))
    {
		pNode = pNodeList + iOffset;
    	pSegBilling = getSegBilling (pNode, poEvt->m_sBillingNBR, poEvt->m_sBillingAreaCode, poEvt->m_sStartDate);
    }
    
    if (!pSegBilling)
    {
		iOffset = 0;
		sKey[0] = '\0';
        sprintf (sKey, "%d_", poEvt->m_iSwitchID);
        if (G_PPARAMINFO->m_pSwtichTrunkTree->get (sKey, &iOffset))
        {
			pNode = pNodeList + iOffset;
            pSegBilling = getSegBilling (pNode, poEvt->m_sBillingNBR, poEvt->m_sBillingAreaCode, poEvt->m_sStartDate);
        }
    }
    if (!pSegBilling)
    {
		iOffset = 0;
		char *p = "null_null";
		if (G_PPARAMINFO->m_pSwtichTrunkTree->get (p, &iOffset))
        {
			pNolimitNode = pNodeList + iOffset;
        	pSegBilling = getSegBilling (pNolimitNode, poEvt->m_sBillingNBR, poEvt->m_sBillingAreaCode, poEvt->m_sStartDate);
    	}
	}    
    if (pSegBilling)
    {
        if (!pSegBilling->m_iSegFlag)
        {// ʹ��ר���Ʒ�
            strcpy (poEvt->m_sBillingNBR, pSegBilling->m_sAccNBR);
            poEvt->unbindUserInfo();            
            return true;
        }
        else
        {// ר�������мƷѷ�ʽ����
            if (poEvt->m_iServID <=0)
            {
                poEvt->unbindUserInfo();               
                if (poEvt->m_sBillingAreaCode[0]=='\0')
                {
                    strcpy(poEvt->m_sBillingAreaCode, poEvt->m_sCallingAreaCode);
                }                    
                if (poEvt->m_sBillingNBR[0]=='\0')
                {
                    strcpy (poEvt->m_sBillingNBR, poEvt->m_sCallingNBR);
                }
                poEvt->bindUserInfo();
            }
            if (poEvt->m_iServID >0 )
            {////�е��û� �������мƷ�
                /////���ﲻ�ò���
            }
            else
            {///�޵��û� ʹ��ר���Ʒ�
                strcpy (poEvt->m_sBillingNBR, pSegBilling->m_sAccNBR);
                poEvt->unbindUserInfo();                 
                return true;                 
            }
        }
    }    
    return false;
}
#endif


#ifdef PRIVATE_MEMORY
PNSegBilling * SegBillingMgr::getSegBilling(EventSection *poEvt, 
        char *_sAccNBR, char *_sAreaCode, char *_sDate)
{
    static PNSegBilling  *pSegBilling = NULL;
    static TrunkTreeNode *pNode;
    static char sKey[65];
    
    if (!m_bInited)
        load ();
    
    // �����ȼ����� m_pSwtichTrunkTree /m_pSwtichTree /pSegBilling
    pSegBilling = NULL;
    
    if (m_pSwtichTrunkTree)
    {
        sprintf (sKey, "%d_%s", poEvt->m_iSwitchID, poEvt->m_sTrunkIn);
        if (m_pSwtichTrunkTree->get (sKey, &pNode))
        {
            pSegBilling = SegBillingMgr::getSegBilling (pNode, 
                _sAccNBR, _sAreaCode, _sDate);
        }
    }
    
    if (m_pSwtichTree && !pSegBilling)
    {
        sprintf (sKey, "%d", poEvt->m_iSwitchID);
        if (m_pSwtichTree->get (sKey, &pNode))
        {
            pSegBilling = SegBillingMgr::getSegBilling (pNode, 
                _sAccNBR, _sAreaCode, _sDate);
        }
    }
    
    if (m_pNolimitNode && !pSegBilling)
    {
        pSegBilling = SegBillingMgr::getSegBilling (m_pNolimitNode, 
                _sAccNBR, _sAreaCode, _sDate);
    }

    return  pSegBilling;
    
}
#else
PNSegBilling * SegBillingMgr::getSegBilling(EventSection *poEvt, char *_sAccNBR, char *_sAreaCode, char *_sDate)
{
    static PNSegBilling  *pSegBilling = NULL;
    static TrunkTreeNode *pNode;
    static char sKey[65];     
    // �����ȼ����� m_pSwtichTrunkTree /m_pSwtichTree /pSegBilling
    pSegBilling = NULL;  
	TrunkTreeNode *pNodeList = G_PPARAMINFO->m_poTrunkTreeNodeList;
	unsigned int iOffset = 0;
    sKey[0] = '\0';
    sprintf (sKey, "%d_%s", poEvt->m_iSwitchID, poEvt->m_sTrunkIn);		
    if (G_PPARAMINFO->m_pSwtichTrunkTree->get (sKey, &iOffset))
    {
		pNode = pNodeList + iOffset;
    	pSegBilling = SegBillingMgr::getSegBilling (pNode, _sAccNBR, _sAreaCode, _sDate);
    }
    
    if (!pSegBilling)
    {
		sKey[0] = '\0';
        sprintf (sKey, "%d_", poEvt->m_iSwitchID);
		iOffset = 0;
        if (G_PPARAMINFO->m_pSwtichTrunkTree->get (sKey, &iOffset))
        {
			pNode = pNodeList + iOffset;
            pSegBilling = SegBillingMgr::getSegBilling (pNode, _sAccNBR, _sAreaCode, _sDate);
        }
    }   
    if (!pSegBilling)
    {
		iOffset = 0;
		char *p = "null_null";
        if (G_PPARAMINFO->m_pSwtichTrunkTree->get (p, &iOffset))
        {
			pNode = pNodeList + iOffset;
        	pSegBilling = SegBillingMgr::getSegBilling (pNode,_sAccNBR, _sAreaCode, _sDate);
		}
    }
    return  pSegBilling;
}
#endif
 

/////���⴦��,���ڲ��ֺ���,��Ȼ��ר���Ŷ�,���ǲ���Ҫȥ�滻��ר������
////���� true ���ʾ����Ҫȥ�滻
#ifdef PRIVATE_MEMORY
bool SegBillingMgr::segNotBilling(
        char *_sAccNBR, char *_sAreaCode, char *_sDate)
{

    static bool bReadDB = true;
    static HashList<struct  strPNNotBilling>  hoNotBilling(200);
    struct strPNNotBilling oNotBilling;
    
    if (bReadDB)
    {
        DEFINE_QUERY(qry);
    
        qry.close();
        qry.setSQL ("select AREA_CODE||ACC_NBR  NBR,"
          " to_char( EFF_DATE,'yyyymmddhh24miss') EFF_DATE," 
    	    " to_char( EXP_DATE,'yyyymmddhh24miss') EXP_DATE "
    	    " from b_pn_seg_not_billing ");
        qry.open();
        while (qry.next())
        {
            strncpy (oNotBilling.m_sEffDate, qry.field("EFF_DATE").asString(),  15);
            strncpy (oNotBilling.m_sExpDate, qry.field("EXP_DATE").asString(), 15 );
            hoNotBilling.add (qry.field("NBR").asString(),   oNotBilling);
        }
        qry.close();
    
        bReadDB = false;
    }
    
    static char sNbr[MAX_CALLING_NBR_LEN+MAX_CALLING_AREA_CODE_LEN];
    snprintf (sNbr, MAX_CALLING_NBR_LEN+MAX_CALLING_AREA_CODE_LEN,
        "%s%s", _sAreaCode, _sAccNBR);
    
    if (hoNotBilling.get (sNbr,&oNotBilling))
    {
        if (strcmp (_sDate, oNotBilling.m_sEffDate) >=0 
            && strcmp (_sDate, oNotBilling.m_sExpDate) <=0)
            return true;
    }
    
    return false;
     
}
#else
bool SegBillingMgr::segNotBilling(char *_sAccNBR, char *_sAreaCode, char *_sDate)
{
    struct strPNNotBilling *pNotBilling; 
    static char sNbr[MAX_CALLING_NBR_LEN+MAX_CALLING_AREA_CODE_LEN];
	sNbr[0] = '\0';
    snprintf (sNbr, MAX_CALLING_NBR_LEN+MAX_CALLING_AREA_CODE_LEN, "%s%s", _sAreaCode, _sAccNBR); 
	unsigned int iOffset = 0;  
    if (G_PPARAMINFO->m_poPNNotBillingIndex->get (sNbr,&iOffset))
    {
		pNotBilling = G_PPARAMINFO->m_poPNNotBillingList + iOffset;
        if (strcmp (_sDate, pNotBilling->m_sEffDate) >=0 && strcmp (_sDate, pNotBilling->m_sExpDate) <=0)
            return true;
    }   
    return false;    
}
#endif

