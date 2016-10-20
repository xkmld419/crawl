/*VER: 7*/ 
#include "EventTypeMgrEx.h"
#include "assert.h"
#include "DistanceMgr.h"
#include "NbrTypeMgr.h"

EventTypeMgrEx* EventTypeMgrEx::m_gpEventTypeMgrEx = 0;
EventTypeMgrEx::EventTypeMgrEx(void)
{
}

EventTypeMgrEx::~EventTypeMgrEx(void)
{
}

EventTypeMgrEx* EventTypeMgrEx::GetInstance(void)
{
    if(!EventTypeMgrEx::m_gpEventTypeMgrEx)
    {
        EventTypeMgrEx::m_gpEventTypeMgrEx = new EventTypeMgrEx();
        if(!EventTypeMgrEx::m_gpEventTypeMgrEx->InitInstance())
        {
            delete EventTypeMgrEx::m_gpEventTypeMgrEx;
            EventTypeMgrEx::m_gpEventTypeMgrEx =0;
        }
    }
    return EventTypeMgrEx::m_gpEventTypeMgrEx;
}

bool EventTypeMgrEx::GetSDSpecTypeB(EventSection* pEventSection,char* sType,int iLen,int iType)
{
    char* pOtherAreaCode=0;
    char* pOhterNbr=0;
    if(CALLTYPE_MTC != pEventSection->m_iCallTypeID)
    {
        pOtherAreaCode = &pEventSection->m_sCalledAreaCode[0];
        pOhterNbr = &pEventSection->m_sCalledNBR[0];
    }else
    {
        pOtherAreaCode = &pEventSection->m_sCallingAreaCode[0];
        pOhterNbr = &pEventSection->m_sCallingNBR[0];
    }
    if((strncmp(pOtherAreaCode,"00",2)==0)&&(0!=m_pHeadSpecInfo_Inter))
    {
        //����
        HeadSpecInfo* pHeadSpecInfo =0;
        if (m_pHeadSpecInfo_Inter->get(pOtherAreaCode,&pHeadSpecInfo))
        {
            if(pHeadSpecInfo==0)
                return false;
            if(pEventSection->m_sStartDate[0]!=0)
            {
                if((strcmp(pEventSection->m_sStartDate,pHeadSpecInfo->sEffDate)<0)||
                    (strcmp(pEventSection->m_sStartDate,pHeadSpecInfo->sExpDate)>=0))
                    return false;
            }
            if(0==iType)
            {
                //HomeType
                strncpy(sType,pHeadSpecInfo->sHomeType,(strlen(pHeadSpecInfo->sHomeType)>iLen ?iLen:strlen(pHeadSpecInfo->sHomeType)));
                sType[(strlen(pHeadSpecInfo->sHomeType)>iLen ?iLen:strlen(pHeadSpecInfo->sHomeType))]=0;
            }else
            {
                strncpy(sType,pHeadSpecInfo->sServType,(strlen(pHeadSpecInfo->sServType)>iLen ?iLen:strlen(pHeadSpecInfo->sServType)));
                sType[(strlen(pHeadSpecInfo->sServType)>iLen ?iLen:strlen(pHeadSpecInfo->sServType))]=0;
                //ServType;
								return true;
            }
            return true;
        }else
            return false;
    }else{
        //����
        HeadSpecInfo* pHeadSpecInfo =0;
        char sHeadTemp[MAX_CALLED_NBR_LEN]={0};
        strcpy(sHeadTemp,pOhterNbr);
        int iLenTmp = strlen(sHeadTemp);
		if(!NbrTypeMgr::ifMobileNbr(sHeadTemp))
		{
			//�Է������ǹ̶��绰��������ȡ
			if(0 ==m_pHeadSpecInfo_national)
				return false;
			if(m_pHeadSpecInfo_national->get(pOtherAreaCode,&pHeadSpecInfo))
			{
				if(0 == pHeadSpecInfo)
					return false;
				if(pEventSection->m_sStartDate[0]!=0)
				{
					if((strcmp(pEventSection->m_sStartDate,pHeadSpecInfo->sEffDate)<0)||
						(strcmp(pEventSection->m_sStartDate,pHeadSpecInfo->sExpDate)>=0))
						return false;
					if(0==iType)
					{
						strncpy(sType,pHeadSpecInfo->sHomeType,(strlen(pHeadSpecInfo->sHomeType)>iLen ?iLen:strlen(pHeadSpecInfo->sHomeType)));
						sType[(strlen(pHeadSpecInfo->sHomeType)>iLen ?iLen:strlen(pHeadSpecInfo->sHomeType))]=0;
						return true;
					}else
					{
						strncpy(sType,pHeadSpecInfo->sServType,(strlen(pHeadSpecInfo->sServType)>iLen ?iLen:strlen(pHeadSpecInfo->sServType)));
						sType[(strlen(pHeadSpecInfo->sServType)>iLen ?iLen:strlen(pHeadSpecInfo->sServType))]=0;
						//ServType;
						return true;
					}
				}else
					return false;
			}else
				return false;

		}else
		{
			if(0 == m_pHeadSpecInfo)
				return false;
        while(iLenTmp>0)
        {
            sHeadTemp[iLenTmp--]=0;
            if (m_pHeadSpecInfo->get(sHeadTemp,&pHeadSpecInfo))
            {
                if(pHeadSpecInfo==0)
                    continue;
                if(pEventSection->m_sStartDate[0]!=0)
                {
                    if((strcmp(pEventSection->m_sStartDate,pHeadSpecInfo->sEffDate)<0)||
                        (strcmp(pEventSection->m_sStartDate,pHeadSpecInfo->sExpDate)>=0))
                        continue;
                }
                if(0==iType)
                {
                    strncpy(sType,pHeadSpecInfo->sHomeType,(strlen(pHeadSpecInfo->sHomeType)>iLen ?iLen:strlen(pHeadSpecInfo->sHomeType)));
                    sType[(strlen(pHeadSpecInfo->sHomeType)>iLen ?iLen:strlen(pHeadSpecInfo->sHomeType))]=0;
                    return true;
                }else
                {
                    strncpy(sType,pHeadSpecInfo->sServType,(strlen(pHeadSpecInfo->sServType)>iLen ?iLen:strlen(pHeadSpecInfo->sServType)));
                    sType[(strlen(pHeadSpecInfo->sServType)>iLen ?iLen:strlen(pHeadSpecInfo->sServType))]=0;
                   return true; 
			//ServType;
                }
                continue;
            }else
                continue;
        }
    }
	}
    return false;
}
bool EventTypeMgrEx::InitInstance(void)
{
    //��ʼ��
    m_pLocalHeadMgr = new Head2OrgIDMgr();
    m_pLocalHead = new HeadEx();

    m_pOrgMgrEx = OrgMgrEx::GetInstance();
    m_pEventRuleMgr = new LongEventTypeRuleMgrEx();
    m_pRoamMgr = new MobileRegionMgr();
    char sSDSpecFlag[8]={0};
    char sSDHomeTableName[32]={0};
    char sSDInterTableName[32]={0};
	char sSDNationalTableName[32]={0};
    m_pHeadSpecInfo_Inter = 0;
    m_pHeadSpecInfo =0;
	m_pHeadSpecInfo_national = 0;
		if(!ParamDefineMgr::getParam("ATTR_FORMAT","LONGTYPENOMODI",m_sLongTypeNoModify))
			return true;
    if(!ParamDefineMgr::getParam("ATTR_FORMAT", "SPEC_TYPE_FLAG",sSDSpecFlag ))
        return true;
    if (!ParamDefineMgr::getParam("ATTR_FORMAT","HOME_TABLE_NAME",sSDHomeTableName))
    {
        return true;
    }
    if(!ParamDefineMgr::getParam("ATTR_FORMAT","INTER_TABLE_NAME",sSDInterTableName))
        return true;

	if(!ParamDefineMgr::getParam("ATTR_FORMAT","NATIONAL_TABLE_NAME",sSDNationalTableName))
		return true;
    if((sSDSpecFlag[0]=='1')&&(sSDHomeTableName[0]!=0)&&(sSDInterTableName[0]!=0))
    {
        m_pHeadSpecInfo_Inter = new KeyTree<HeadSpecInfo*>();
        DEFINE_QUERY(qry);
        qry.close();
        char sSql[4096]={0};
        sprintf(sSql," select trim(NATION_AREA_CODE),TO_CHAR(TO_DATE(BEGIN_TIME,'YYYYMMDDHH24MISS'),'YYYYMMDDHH24MISS') " 
             " ,TO_CHAR(TO_DATE(END_TIME,'YYYYMMDDHH24MISS'),'YYYYMMDDHH24MISS'),nvl(serv_type,''), "
             " NVL(HOME_TYPE,'') from %s",sSDInterTableName);
        qry.setSQL(sSql);
        qry.open();
        while(qry.next())
        {
            HeadSpecInfo* pHeadSpecInfo = new HeadSpecInfo();
            memset(pHeadSpecInfo,0,sizeof(HeadSpecInfo));
            strcpy(pHeadSpecInfo->sAreaCode,qry.field(0).asString());
            strcpy(pHeadSpecInfo->sEffDate,qry.field(1).asString());
            strcpy(pHeadSpecInfo->sExpDate,qry.field(2).asString());
            strcpy(pHeadSpecInfo->sServType,qry.field(3).asString());
            strcpy(pHeadSpecInfo->sHomeType,qry.field(4).asString());
            m_pHeadSpecInfo_Inter->add(pHeadSpecInfo->sAreaCode,pHeadSpecInfo);
        }
        qry.close();

        m_pHeadSpecInfo = new KeyTree<HeadSpecInfo*>();
        sprintf(sSql,"select trim(AREA_CODE),TO_CHAR(TO_DATE(BEGIN_TIME,'YYYYMMDDHH24MISS'),'YYYYMMDDHH24MISS') " 
            " ,TO_CHAR(TO_DATE(END_TIME,'YYYYMMDDHH24MISS'),'YYYYMMDDHH24MISS'),nvl(serv_type,''), "
            "  NVL(HOME_TYPE,''),MSISDN_SEG from %s ",sSDHomeTableName);
        qry.setSQL(sSql);
        qry.open();
        while(qry.next())
        {
            HeadSpecInfo* pHeadSpecInfo = new HeadSpecInfo();
            memset(pHeadSpecInfo,0,sizeof(HeadSpecInfo));
            strcpy(pHeadSpecInfo->sAreaCode,qry.field(0).asString());
            strcpy(pHeadSpecInfo->sEffDate,qry.field(1).asString());
            strcpy(pHeadSpecInfo->sExpDate,qry.field(2).asString());
            strcpy(pHeadSpecInfo->sServType,qry.field(3).asString());
            strcpy(pHeadSpecInfo->sHomeType,qry.field(4).asString());
            strcpy(pHeadSpecInfo->sHead,qry.field(5).asString());
            m_pHeadSpecInfo->add(pHeadSpecInfo->sHead,pHeadSpecInfo);
        }
        qry.close();

		m_pHeadSpecInfo_national = new KeyTree<HeadSpecInfo*>();
		sprintf(sSql," select trim(AREA_CODE),TO_CHAR(TO_DATE(BEGIN_TIME,'YYYYMMDDHH24MISS'),'YYYYMMDDHH24MISS') "
            " ,TO_CHAR(TO_DATE(END_TIME,'YYYYMMDDHH24MISS'),'YYYYMMDDHH24MISS'),nvl(serv_type,''), "
            "  NVL(HOME_TYPE,''),AREA_CODE from %s ",sSDNationalTableName);
		qry.setSQL(sSql);
		qry.open();
		while(qry.next())
		{
			HeadSpecInfo* pHeadSpecInfo = new HeadSpecInfo();
            memset(pHeadSpecInfo,0,sizeof(HeadSpecInfo));
            strcpy(pHeadSpecInfo->sAreaCode,qry.field(0).asString());
            strcpy(pHeadSpecInfo->sEffDate,qry.field(1).asString());
            strcpy(pHeadSpecInfo->sExpDate,qry.field(2).asString());
            strcpy(pHeadSpecInfo->sServType,qry.field(3).asString());
            strcpy(pHeadSpecInfo->sHomeType,qry.field(4).asString());
            strcpy(pHeadSpecInfo->sHead,qry.field(5).asString());
            m_pHeadSpecInfo_national->add(pHeadSpecInfo->sHead,pHeadSpecInfo);
		}
		qry.close();
    }
    return true;
}

void EventTypeMgrEx::ExitInstance(void)
{
    //�˳�����
    if(m_pLocalHeadMgr)
        delete m_pLocalHeadMgr;
    m_pLocalHeadMgr = 0;

    if(m_pLocalHead)
        delete m_pLocalHead;
    m_pLocalHead =0;

    if(m_pEventRuleMgr)
        delete m_pEventRuleMgr;
    m_pEventRuleMgr  =0;

    if(m_pRoamMgr)
        delete m_pRoamMgr;
    m_pRoamMgr = 0;
}

bool EventTypeMgrEx::GetAreaCode(int iOrgID,char* pAreaCode,int nSize)
{
    if((0== pAreaCode)||(0>= nSize))
        return false;
    return m_pOrgMgrEx->getAreaCode(iOrgID,pAreaCode,nSize);
}

bool EventTypeMgrEx::GetOrgIDbyNbr(const char*pNbr,const char* pTime,
                                   int& iOrgID,const char* tszType,int* piPartyID,int* piPartnerID )
{
    if(0==pNbr)
        return false;
    if(!m_pLocalHeadMgr->searchLocalHead(pNbr,pTime,this->m_pLocalHead,tszType))
    {
        //û���ҵ�OrgID
        return false;
    }
    iOrgID = m_pLocalHead->getOrgID();
    if(0 != piPartyID)
        *piPartyID = m_pLocalHead->getPartyID();
    if(0!= piPartnerID)
        *piPartnerID = m_pLocalHead->getPartnerID();
    return true;
}

bool EventTypeMgrEx::Rule_BillingRoam(EventSection* pEventSection,int iType)
{
    if(0 == pEventSection)
        return false;
    int iLongType1,iLongType2;

    if((ROAMTYPE_ROAM_BORD == pEventSection->m_oEventExt.m_iStdRoamTypeID)||
    (ROAMTYPE_ROAM_BORD_EX== pEventSection->m_oEventExt.m_iStdRoamTypeID))
    {
        //pEventSection->m_oEventExt.m_iStdRoamTypeID = pEventSection->m_oEventExt.m_iRoamTypeID;
           
        pEventSection->m_oEventExt.m_iStdLongTypeID =
            GetLongTypeByOrgID (pEventSection->m_iBillingOrgIDEx_VS, pEventSection->m_iOtherOrgIDEx_AD);

        pEventSection->m_oEventExt.m_iLongTypeID = 
             pEventSection->m_oEventExt.m_iStdLongTypeID;
        if(pEventSection->m_oEventExt.m_iStdLongTypeID ==-1)
        {
		pEventSection->m_oEventExt.m_iStdRoamTypeID = pEventSection->m_oEventExt.m_iRoamTypeID;
            pEventSection->m_iErrorID = 1804;
            //return false;
        }

        //if(pEventSection->m_oEventExt.m_iStdLongTypeID<= LONGTYPE_LATN)
        //{
        //    //�߽����β������εص�������
        //    return true;
        //}
	if(0==iType)
	{
		pEventSection->m_oEventExt.m_iLongTypeID = GetLongTypeByOrgID(pEventSection->m_iBillingOrgIDEx_AD,pEventSection->m_iOtherOrgIDEx_AD);
	}else
	{
		if(1 == iType)
		{
			if(ROAMTYPE_ROAM_BORD_EX == pEventSection->m_oEventExt.m_iStdRoamTypeID)
			{
				pEventSection->m_oEventExt.m_iLongTypeID = pEventSection->m_oEventExt.m_iStdLongTypeID;
			}
			else
			{
				if((pEventSection->m_iBillingOrgIDEx_AD==pEventSection->m_iOtherOrgIDEx_AD)||(m_pOrgMgrEx->getBelong(pEventSection->m_iOtherOrgIDEx_AD,pEventSection->m_iBillingOrgIDEx_AD))||(pEventSection->m_iBillingOrgIDEx_VS== pEventSection->m_iOtherOrgIDEx_AD)||(m_pOrgMgrEx->getBelong(pEventSection->m_iOtherOrgIDEx_AD,pEventSection->m_iBillingOrgIDEx_VS)))
				{
			pEventSection->m_oEventExt.m_iLongTypeID = GetLongTypeByOrgID(pEventSection->m_iBillingOrgIDEx_AD,pEventSection->m_iBillingOrgIDEx_AD);
				}else
				{
					pEventSection->m_oEventExt.m_iLongTypeID=GetLongTypeByOrgID(pEventSection->m_iBillingOrgIDEx_VS,pEventSection->m_iBillingOrgIDEx_AD);	
				}
			}
		}else
		{
			if((pEventSection->m_iBillingOrgIDEx_AD==pEventSection->m_iOtherOrgIDEx_AD)||(m_pOrgMgrEx->getBelong(pEventSection->m_iOtherOrgIDEx_AD,pEventSection->m_iBillingOrgIDEx_AD))||(pEventSection->m_iBillingOrgIDEx_VS== pEventSection->m_iOtherOrgIDEx_AD)||(m_pOrgMgrEx->getBelong(pEventSection->m_iOtherOrgIDEx_AD,pEventSection->m_iBillingOrgIDEx_VS)))
			{
			pEventSection->m_oEventExt.m_iLongTypeID = GetLongTypeByOrgID(pEventSection->m_iBillingOrgIDEx_AD,pEventSection->m_iBillingOrgIDEx_AD);
			}else
			{
				pEventSection->m_oEventExt.m_iLongTypeID=GetLongTypeByOrgID(pEventSection->m_iBillingOrgIDEx_VS,pEventSection->m_iBillingOrgIDEx_AD);
			}
		}
	}
	pEventSection->m_oEventExt.m_iStdRoamTypeID = pEventSection->m_oEventExt.m_iRoamTypeID;
	pEventSection->m_oEventExt.m_iRoamTypeID = ROAMTYPE_NOT_ROAM;
	if(-1 ==pEventSection->m_oEventExt.m_iLongTypeID)
	{
	    pEventSection->m_iErrorID = 1804;
		return false;
	}
       // pEventSection->m_oEventExt.m_iLongTypeID = GetLongTypeByOrgID(pEventSection->m_iBillingOrgIDEx_AD,pEventSection->m_iOtherOrgIDEx_AD);
        /*if(iLongType1 <= LONGTYPE_LATN)
        {
            pEventSection->m_oEventExt.m_iLongTypeID = iLongType1;
            return true;
        }
        iLongType1 = GetLongTypeByOrgID(pEventSection->m_iBillingOrgIDEx_AD,pEventSection->m_iOtherOrgIDEx_VS);
        if(iLongType1 <= LONGTYPE_LATN)
        {
            pEventSection->m_oEventExt.m_iLongTypeID = iLongType1;
            return true;
        }
        iLongType1 = GetLongTypeByOrgID (pEventSection->m_iBillingOrgIDEx_VS,pEventSection->m_iOtherOrgIDEx_VS);
        if (iLongType1 <= LONGTYPE_LATN) 
        {
            pEventSection->m_oEventExt.m_iLongTypeID = iLongType1;
            return true;
        }*/
        if(-1 ==pEventSection->m_oEventExt.m_iLongTypeID)
        {
            pEventSection->m_iErrorID = 1804;
            return false;
        }
        return true;

    }else
    {
        //�ƷѺ���������״̬
        //�����Ƿ�ͬ��
        //�Է��������εغͼƷѺ��뵽�õ���ͬ
        
        pEventSection->m_oEventExt.m_iLongTypeID = 
            GetLongTypeByOrgID(pEventSection->m_iBillingOrgIDEx_VS,pEventSection->m_iOtherOrgIDEx_AD);


        pEventSection->m_oEventExt.m_iStdRoamTypeID = 
            pEventSection->m_oEventExt.m_iRoamTypeID;

        {
            pEventSection->m_oEventExt.m_iStdLongTypeID =
                pEventSection->m_oEventExt.m_iLongTypeID;
        }
        return true;
    }
	return true;
}


bool EventTypeMgrEx::Rule_BillingNoRoam(EventSection* pEventSection)
{
    if(0 == pEventSection)
        return false;
    //�ƷѺ���û������
    int nRoamType = ROAMTYPE_NOT_ROAM;
    //�жϼƷѺ���ͶԷ�����ĵ��õصĳ�;��ϵ
    int iLongType1,iLongType2;
    pEventSection->m_oEventExt.m_iRoamTypeID = ROAMTYPE_NOT_ROAM;
    pEventSection->m_oEventExt.m_iStdRoamTypeID = ROAMTYPE_NOT_ROAM;

    if(CALLTYPE_MTC != pEventSection->m_iCallTypeID)
    {
        //���л���
        //û�������� �ƷѺ���ORG�ͱ��к���ORG�ó� ��;����
        pEventSection->m_oEventExt.m_iStdLongTypeID =
            GetLongTypeByOrgID(pEventSection->m_iBillingOrgIDEx_AD,
                pEventSection->m_iOtherOrgIDEx_AD);

        pEventSection->m_oEventExt.m_iLongTypeID =
                pEventSection->m_oEventExt.m_iStdLongTypeID;

				if(strcmp(m_sLongTypeNoModify,"1")!=0)
				{
        	if(pEventSection->m_oEventExt.m_iStdLongTypeID>LONGTYPE_LATN_EX && 
        	    pEventSection->m_iOtherOrgIDEx_AD !=pEventSection->m_iOtherOrgIDEx_VS)
        	{
        	    //����������ͬ��
        	    iLongType2 = GetLongTypeByOrgID(pEventSection->m_iBillingOrgIDEx_AD,
        	        pEventSection->m_iOtherOrgIDEx_VS);
        	
        	    if(iLongType2 <= LONGTYPE_LATN_EX) {
        	        //����
        	        pEventSection->m_oEventExt.m_iLongTypeID = iLongType2;
        	    }else
        	    {
        	        //�б߽�
        	       /* if(MobileRegionMgr::CheckCellEdgeType(pEventSection->m_sMSC,pEventSection->m_sLAC,
        	            pEventSection->m_sCellID,m_iOtherOrgID,1))
        	        {
        	            pEventSection->m_oEventExt.m_iStdRoamTypeID =
        	                GetRoamTypeByOrgID(pEventSection->m_iBillingOrgIDEx_AD,
        	                m_iOtherOrgID);
        	        }*/
        	    }
        	}
      	}
        return true;
    }else
    {
        //���л���
        //û�����ζ��㱾��
        pEventSection->m_oEventExt.m_iLongTypeID = LONGTYPE_LATN_EX;
        //��֤���Ǳ߽������� StdLongTypeID��LongTypeID,StdRoamTypeID��RoamTypeIDһ��
        pEventSection->m_oEventExt.m_iStdLongTypeID = LONGTYPE_LATN_EX;
        pEventSection->m_oEventExt.m_iStdRoamTypeID= pEventSection->m_oEventExt.m_iRoamTypeID;
        return true;

    }
    return true;
}

int EventTypeMgrEx::GetEventTypeID(EventSection* pEventSection)
{
    if(0 ==pEventSection)
        return -1;

    /*if(!fillLongTypeAndRoamID(pEventSection))
    {
        //û���ҵ���Ӧ���¼�����
        pEventSection->m_iErrorID =1999;
        return pEventSection->m_iErrorID;
    }*/

    LongEventTypeRuleEx* pEventRuleEx = 0;
    if(!m_pEventRuleMgr->getRule(pEventSection,&pEventRuleEx))
    {
        //û���ҵ���Ӧ���¼�����
        pEventSection->m_iErrorID =1999;
        return pEventSection->m_iErrorID;
    }
    pEventSection->m_iEventTypeID = pEventRuleEx->getOutEventTypeID();
    return pEventSection->m_iEventTypeID;
    
}

bool EventTypeMgrEx::fillAreaCodeSpecify(EventSection* pEventSection,int iWho,int iWhich)
{
    if(0 == iWho)
        iWho=7;
    if(0 == iWhich)
        iWhich =3;
    if(0 == pEventSection)
        return false;

    bool bADSet= false;
    int iParterIDCalling=0;
    int iParterIDCalled =0;
    if (CALLTYPE_MTC != pEventSection->m_iCallTypeID)
    {
        iParterIDCalled = pEventSection->m_iParterID;
    }else
    {
        iParterIDCalling = pEventSection->m_iParterID;
    }
    if(iWho&0x001)
    {
        //���к���
        if(iWhich&0x01)
        {
            bADSet = true;
            //������
            if((pEventSection->m_oEventExt.m_sIMSI[0]!=0)&&
                strncmp(pEventSection->m_oEventExt.m_sIMSI,"460",3)!=0)
            {
                GetOrgIDbyNbr(pEventSection->m_oEventExt.m_sIMSI
                    ,pEventSection->m_sStartDate,pEventSection->m_iCallingOrgIDEx_AD,
                    NUMBER_TYPE_IMSI,
                    &pEventSection->m_iCallingSPTypeID,&iParterIDCalling);
            }else
            {
                GetOrgIDbyNbr(pEventSection->m_sCallingNBR
                    ,pEventSection->m_sStartDate,pEventSection->m_iCallingOrgIDEx_AD,NUMBER_TYPE_PN,
                    &pEventSection->m_iCallingSPTypeID,
                    &iParterIDCalling);
            }
            bool bRet = GetAreaCode(pEventSection->m_iCallingOrgIDEx_AD,
                pEventSection->m_sCallingAreaCode,
                MAX_CALLING_AREA_CODE_LEN-1);
            if(bRet)
            {
                if(strncmp(pEventSection->m_sCallingNBR,pEventSection->m_sCallingAreaCode,
                    strlen(pEventSection->m_sCallingAreaCode)) ==0)
                {
                    strcpy(pEventSection->m_sCallingNBR,
                        &(pEventSection->m_sCallingNBR[strlen(pEventSection->m_sCallingAreaCode)]));
                }
            }
        }

        if(iWhich&0x02)
        {
            if(bADSet)
                pEventSection->m_iCallingOrgIDEx_VS = pEventSection->m_iCallingOrgIDEx_AD;
            //���ε�
            if(0 == m_pRoamMgr)
                return false;
            if((pEventSection->m_sMSC[0]==0)||
                (!m_pRoamMgr->GetOrgID(pEventSection->m_sMSC,pEventSection->m_sLAC,
                pEventSection->m_sCellID,pEventSection->m_sStartDate,
                pEventSection->m_iCallingOrgIDEx_VS                
                )))
            {
                if(pEventSection->m_oEventExt.m_sCallingVisitAreaCode[0] !=0)
                {
                    int iSPTypeID =0;
                    if(pEventSection->m_iBusinessKey !=  FORWARDTYPE_UNCOND_EX)
                        GetOrgIDbyNbr(pEventSection->m_oEventExt.m_sCallingVisitAreaCode,
                        pEventSection->m_sStartDate,pEventSection->m_iCallingOrgIDEx_VS
                        ,NUMBER_TYPE_PN);
                }
            }
			if((0<pEventSection->m_iCallingOrgIDEx_VS)&&(pEventSection->m_oEventExt.m_sCallingVisitAreaCode[0]==0))
			{
				GetAreaCode(pEventSection->m_iCallingOrgIDEx_VS,
					pEventSection->m_oEventExt.m_sCallingVisitAreaCode,
					MAX_CALLING_AREA_CODE_LEN-1);
			}
        }
    }
    if(iWho&0x02)
    {
        bADSet =false;
        //���к���
        if(iWhich&0x01)
        {
            bADSet =true;
            //������
            if(!GetOrgIDbyNbr(pEventSection->m_sCalledNBR
                ,pEventSection->m_sStartDate,pEventSection->m_iCalledOrgIDEx_AD,
                NUMBER_TYPE_PN,&pEventSection->m_iCalledSPTypeID,
                &iParterIDCalled))
            {
                if(!NbrTypeMgr::ifMobileNbr(pEventSection->m_sCalledNBR))
                {
                    char sNbr[MAX_CALLED_NBR_LEN+MAX_CALLED_AREA_CODE_LEN]={0};
                    sprintf(sNbr,"%s%s",pEventSection->m_sCalledAreaCode,pEventSection->m_sCalledNBR);
                    if(!GetOrgIDbyNbr(sNbr
                        ,pEventSection->m_sStartDate,pEventSection->m_iCalledOrgIDEx_AD,
                        NUMBER_TYPE_PN,
                        &pEventSection->m_iCalledSPTypeID,
                        &iParterIDCalled))
                    {
                    sprintf(sNbr,"%s%s",pEventSection->m_oEventExt.m_sCallingVisitAreaCode
                        ,pEventSection->m_sCalledNBR);
                    GetOrgIDbyNbr(sNbr
                        ,pEventSection->m_sStartDate,pEventSection->m_iCalledOrgIDEx_AD,
                        NUMBER_TYPE_PN,
                        &pEventSection->m_iCalledSPTypeID,
                        &iParterIDCalled);
                    }
                }
            }
            bool bRet = GetAreaCode(pEventSection->m_iCalledOrgIDEx_AD,
                pEventSection->m_sCalledAreaCode,
                MAX_CALLING_AREA_CODE_LEN-1);
            if(bRet)
            {
                if(strncmp(pEventSection->m_sCalledNBR,pEventSection->m_sCalledAreaCode,
                    strlen(pEventSection->m_sCalledAreaCode)) ==0)
                {
                    strcpy(pEventSection->m_sCalledNBR,
                        &(pEventSection->m_sCalledNBR[strlen(pEventSection->m_sCalledAreaCode)]));
                }
            }
        }
        if(iWhich&0x02)
        {
            if(bADSet)
                pEventSection->m_iCalledOrgIDEx_VS = pEventSection->m_iCalledOrgIDEx_AD;
            //���ε�
            if(CALLTYPE_MTC != pEventSection->m_iCallTypeID)
            {
                //�Ǳ��л���
                GetOrgIDbyNbr(pEventSection->m_sRomingNBR
                    ,pEventSection->m_sStartDate,pEventSection->m_iCalledOrgIDEx_VS);
            }else
            {
                if((pEventSection->m_sMSC[0]==0)||
                    (!m_pRoamMgr->GetOrgID(pEventSection->m_sMSC,pEventSection->m_sCalledLAC,
                    pEventSection->m_sCalledCellID,pEventSection->m_sStartDate,
                    pEventSection->m_iCalledOrgIDEx_VS)))
                {
                    // ���û�� MSC �ñ�������
                    if(pEventSection->m_oEventExt.m_sCalledVisitAreaCode[0]!=0)
                    {
                        int iSPTypeID =0;
                        GetOrgIDbyNbr(pEventSection->m_oEventExt.m_sCalledVisitAreaCode,
                            pEventSection->m_sStartDate,
                            pEventSection->m_iCalledOrgIDEx_VS );
                    }
                }
				if((0<pEventSection->m_iCalledOrgIDEx_VS)&&(pEventSection->m_oEventExt.m_sCalledVisitAreaCode[0]==0))
				{
					GetAreaCode(pEventSection->m_iCalledOrgIDEx_VS,
						pEventSection->m_oEventExt.m_sCalledVisitAreaCode,
						MAX_CALLING_AREA_CODE_LEN-1);
				}
            }
        }
    }
    if(iWho&0x04)
    {
        bADSet =false;
        //����������
        if(iWhich&0x01)
        {
            bADSet = true;
            //������
            int iCarrierThired;
            GetOrgIDbyNbr(pEventSection->m_sThirdPartyNBR
                ,pEventSection->m_sStartDate,pEventSection->m_iThirdOrgIDEx_AD);

            bool bRet = GetAreaCode(pEventSection->m_iThirdOrgIDEx_AD,
                pEventSection->m_sThirdPartyAreaCode,
                MAX_CALLING_AREA_CODE_LEN-1);
            if(bRet)
            {
                if(strncmp(pEventSection->m_sThirdPartyNBR,pEventSection->m_sThirdPartyAreaCode,
                    strlen(pEventSection->m_sThirdPartyAreaCode)) ==0)
                {
                    strcpy(pEventSection->m_sThirdPartyNBR,
                        &(pEventSection->m_sThirdPartyNBR[strlen(pEventSection->m_sThirdPartyAreaCode)]));
                }
            }

        }
        if(iWhich&0x02)
        {
            if(bADSet)
                pEventSection->m_iThirdOrgIDEx_VS = pEventSection->m_iThirdOrgIDEx_AD;
            //���ε�
            //��ʱû�е������������ε��жϵ�Ҫ��
        }
    }

    if(CALLTYPE_MTC != pEventSection->m_iCallTypeID)
    {
        pEventSection->m_iBillingOrgIDEx_AD = pEventSection->m_iCallingOrgIDEx_AD;
        pEventSection->m_iBillingOrgIDEx_VS = pEventSection->m_iCallingOrgIDEx_VS;
        pEventSection->m_iOtherOrgIDEx_AD = pEventSection->m_iCalledOrgIDEx_AD;
        pEventSection->m_iOtherOrgIDEx_VS = pEventSection->m_iCalledOrgIDEx_VS;
        strcpy(pEventSection->m_sBillingAreaCode,pEventSection->m_sCallingAreaCode);
        strcpy(pEventSection->m_sBillingVisitAreaCode,pEventSection->m_oEventExt.m_sCallingVisitAreaCode);
        strcpy(pEventSection->m_sBillingNBR,pEventSection->m_sCallingNBR);
        pEventSection->m_iParterID = iParterIDCalled;
    }else
    {
        
        pEventSection->m_iBillingOrgIDEx_VS =  pEventSection->m_iCalledOrgIDEx_VS; 
        pEventSection->m_iOtherOrgIDEx_AD = pEventSection->m_iCallingOrgIDEx_AD;
        pEventSection->m_iOtherOrgIDEx_VS = pEventSection->m_iCallingOrgIDEx_VS;
        strcpy(pEventSection->m_sBillingAreaCode,pEventSection->m_sCalledAreaCode);
        strcpy(pEventSection->m_sBillingVisitAreaCode,pEventSection->m_oEventExt.m_sCalledVisitAreaCode);
        strcpy(pEventSection->m_sBillingNBR,pEventSection->m_sCalledNBR);
        pEventSection->m_iParterID =iParterIDCalling;

        if(pEventSection->m_iCallingOrgIDEx_AD<=0)
        {
            if(0!= pEventSection->m_oEventExt.m_sCalledVisitAreaCode[0])
            {
                if(NbrTypeMgr::ifMobileNbr(pEventSection->m_oEventExt.m_sCalledVisitAreaCode))
                {
                    GetOrgIDbyNbr(pEventSection->m_oEventExt.m_sCalledVisitAreaCode
                        ,pEventSection->m_sStartDate,pEventSection->m_iCalledOrgIDEx_AD,
                        NUMBER_TYPE_PN , &pEventSection->m_iCalledSPTypeID,&pEventSection->m_iParterID);
                }
            }
        }
        pEventSection->m_iBillingOrgIDEx_AD =  pEventSection->m_iCalledOrgIDEx_AD; 
    }
	if(pEventSection->m_iBillingOrgID <=0)
	{
		pEventSection->m_iBillingOrgID =pEventSection->m_iBillingOrgIDEx_AD ;
	}
    return true;
}

bool EventTypeMgrEx::fillAreaCode(EventSection*pEventSection, int iWho, int iWhich)
{
    if(0 == pEventSection)
        return false;


    int iPartyIDA,iPartyIDB;
    if((iWho!=0)||(0!= iWhich))
        return fillAreaCodeSpecify(pEventSection,iWho,iWhich);

    char* pBillingNbr,*pOtherNbr;
    char* pBillingAreaCode,*pOtherAreaCode;
    char* pBillingAreaCodeVS,*pOtherAreaCodeVS;
    int* pBillingOrgAD,*pOtherOrgAD;
    int* pBillingOrgVS,*pOtherOrgVS ;
    char* pLac,*pCellID;
    int* pSpTypeID;

    if(CALLTYPE_MTC != pEventSection->m_iCallTypeID)
    {
        pBillingNbr = &pEventSection->m_sCallingNBR[0];
        pOtherNbr = &pEventSection->m_sCalledNBR[0];
        pBillingOrgAD = &pEventSection->m_iCallingOrgIDEx_AD;
        pBillingOrgVS =&pEventSection->m_iCallingOrgIDEx_VS;
        pOtherOrgAD = &pEventSection->m_iCalledOrgIDEx_AD;
        pOtherOrgVS= &pEventSection->m_iCalledOrgIDEx_VS;
        pLac = &pEventSection->m_sLAC[0];
        pCellID = &pEventSection->m_sCellID[0];
        pBillingAreaCode = &pEventSection->m_sCallingAreaCode[0];
        pOtherAreaCode = &pEventSection->m_sCalledAreaCode[0];
        pBillingAreaCodeVS = &pEventSection->m_oEventExt.m_sCallingVisitAreaCode[0];
        pOtherAreaCodeVS=&pEventSection->m_oEventExt.m_sCalledVisitAreaCode[0];
        pSpTypeID =&pEventSection->m_iCalledSPTypeID;
    }else
    {
        pBillingNbr = &pEventSection->m_sCalledNBR[0];
        pOtherNbr =&pEventSection->m_sCallingNBR[0];
        pOtherOrgAD  = &pEventSection->m_iCallingOrgIDEx_AD;
        pOtherOrgVS  =&pEventSection->m_iCallingOrgIDEx_VS;
        pBillingOrgAD = &pEventSection->m_iCalledOrgIDEx_AD;
        pBillingOrgVS= &pEventSection->m_iCalledOrgIDEx_VS;
        pLac = &pEventSection->m_sCalledLAC[0];
        pCellID = &pEventSection->m_sCalledCellID[0];
        pOtherAreaCode  = &pEventSection->m_sCallingAreaCode[0];
        pBillingAreaCode = &pEventSection->m_sCalledAreaCode[0];
        pOtherAreaCodeVS  = &pEventSection->m_oEventExt.m_sCallingVisitAreaCode[0];
        pBillingAreaCodeVS =&pEventSection->m_oEventExt.m_sCalledVisitAreaCode[0];
        pSpTypeID = &pEventSection->m_iCallingSPTypeID;
    }

    //��ȡ�ƷѺ��������

    if((pEventSection->m_oEventExt.m_sIMSI[0]!=0)
        &&strncmp(pEventSection->m_oEventExt.m_sIMSI,"460",3)!=0)
    {
        if(!GetOrgIDbyNbr(pEventSection->m_oEventExt.m_sIMSI
            ,pEventSection->m_sStartDate,*pBillingOrgAD,
            NUMBER_TYPE_IMSI))
        {
            {
                pEventSection->m_iErrorID = 1803;//�Ʒѷ�ͨ��IMSIû����ȡ��Org
            }
        }else
        {
            pEventSection->m_iBillingOrgID = *pBillingOrgAD;
        }
    }else
    {
        if(!GetOrgIDbyNbr(pBillingNbr
            ,pEventSection->m_sStartDate,*pBillingOrgAD))
        {
            pEventSection->m_iErrorID = 1803; //�Ʒѷ�ͨ����ͷû����ȡ��org;
        }else
        {
            pEventSection->m_iBillingOrgID = *pBillingOrgAD;
        }
    }
    // �Ʒ�����
    bool bRet = GetAreaCode(*pBillingOrgAD,
        pBillingAreaCode,
        MAX_CALLING_AREA_CODE_LEN-1);
    if(bRet)
    {
        if(strncmp(pBillingNbr,pBillingAreaCode,
            strlen(pBillingAreaCode)) ==0)
        {
            strcpy(pBillingNbr,
                &(pBillingNbr[strlen(pBillingAreaCode)]));
        }
    }else
    {
        if(pEventSection->m_iErrorID != 1803)
            pEventSection->m_iErrorID = 1901; //Orgƥ������ʧ��
    }

    if((1803!= pEventSection->m_iErrorID)&&(1901!=pEventSection->m_iErrorID ))
    {
        pEventSection->m_iBillingOrgIDEx_AD =*pBillingOrgAD ;
        strcpy(pEventSection->m_sBillingAreaCode,
            pBillingAreaCode);
        strcpy(pEventSection->m_sBillingNBR,
            pBillingNbr);
    }

    //Ĭ���������ε�=������
    *pBillingOrgVS = *pBillingOrgAD;

    if((pEventSection->m_sMSC[0]==0)||
    	(!m_pRoamMgr->GetOrgID(pEventSection->m_sMSC,pLac,
                pCellID,pEventSection->m_sStartDate,
                *pBillingOrgVS)))
    {
 //       if(!m_pRoamMgr->GetOrgID(pEventSection->m_sMSC,pLac,
 //               pCellID,pEventSection->m_sStartDate,
 //               *pBillingOrgVS))
        if(*pBillingAreaCodeVS != 0 )
        { 
            //����������ת 
            //��������ת�Ļ���billingVisitAreaCode�ŵ���A�����ε�
            if(CALLTYPE_FWD != pEventSection->m_iCallTypeID  ||
              pEventSection->m_iBusinessKey !=  FORWARDTYPE_UNCOND_EX)
                GetOrgIDbyNbr(pBillingAreaCodeVS
                ,pEventSection->m_sStartDate,*pBillingOrgVS);
        } 
    }
    pEventSection->m_iBillingOrgIDEx_VS=*pBillingOrgVS; 

    bRet= GetAreaCode(*pBillingOrgVS,
        pBillingAreaCodeVS,
        MAX_CALLING_AREA_CODE_LEN-1);
    if(!bRet)
    {
        //�Ʒѷ����ŵ�������ȡʧ��
        pEventSection->m_iErrorID = 1902;
    }else
    {
        strcpy(pEventSection->m_sBillingVisitAreaCode,pBillingAreaCodeVS);
    }

    //�Է���������� 

     if(!GetOrgIDbyNbr(pOtherNbr
        ,pEventSection->m_sStartDate,*pOtherOrgAD,
        NUMBER_TYPE_PN,pSpTypeID,&pEventSection->m_iParterID))
     {
         if(NbrTypeMgr::ifMobileNbr(pOtherNbr))
         {
             pEventSection->m_iErrorID = 1802;
             return true;
         }else
         {
         char sNbr[MAX_CALLED_NBR_LEN+MAX_CALLED_AREA_CODE_LEN]={0};
	 sprintf(sNbr,"%s%s",pOtherAreaCode,pOtherNbr);
	 if(!GetOrgIDbyNbr(sNbr
	     ,pEventSection->m_sStartDate,*pOtherOrgAD,
	  NUMBER_TYPE_PN,
	pSpTypeID,
	     &pEventSection->m_iParterID))
	     {
         sprintf(sNbr,"%s%s",pBillingAreaCodeVS,pOtherNbr);
         if(!GetOrgIDbyNbr(sNbr
             ,pEventSection->m_sStartDate,*pOtherOrgAD,
             NUMBER_TYPE_PN,
             pSpTypeID,
             &pEventSection->m_iParterID))
         {
              pEventSection->m_iErrorID = 1802;
         }
             }
	 }
     }

     bRet= GetAreaCode(*pOtherOrgAD,
         pOtherAreaCode,
         MAX_CALLING_AREA_CODE_LEN-1);
     if(!bRet)
     {
         pEventSection->m_iErrorID = 1903;
     }else
     {
        if(strncmp(pOtherNbr,pOtherAreaCode,
            strlen(pOtherAreaCode)) ==0)
        {
            strcpy(pOtherNbr,
                &(pOtherNbr[strlen(pOtherAreaCode)]));
        }
    }

     //�Է��������ε�
     *pOtherOrgVS = *pOtherOrgAD;
     int iCarrierCalled =0;
     if(pEventSection->m_sRomingNBR[0])
     {
         GetOrgIDbyNbr(pEventSection->m_sRomingNBR,
             pEventSection->m_sStartDate,*pOtherOrgVS
             );
     }else
     {
         if(pOtherAreaCodeVS[0] !=0)
         {
             GetOrgIDbyNbr(pOtherAreaCodeVS,
                 pEventSection->m_sStartDate,*pOtherOrgVS
                 );
         }
     }

     pEventSection->m_iOtherOrgIDEx_AD=*pOtherOrgAD;
     pEventSection->m_iOtherOrgIDEx_VS=*pOtherOrgVS;

     pEventSection->m_oEventExt.m_iCallingOrgID = pEventSection->m_iCallingOrgIDEx_AD;
     pEventSection->m_oEventExt.m_iCalledOrgID = pEventSection->m_iCalledOrgIDEx_AD;    

     return bRet; 
}

bool EventTypeMgrEx::fillLongTypeAndRoamID(EventSection* pEventSection,int iType)
{
    if(0== pEventSection)
        return false;

/*
# ���Ҫ�Ӳο�Ŀǰ���еĹ��������ݲ����������ȴ��û����ϻ�ȡ�����еĹ�����
*/
    if((pEventSection->m_oEventExt.m_iRoamTypeID !=0)&&
        (pEventSection->m_oEventExt.m_iLongTypeID!=0))
        return true;
  
    if (!pEventSection->m_iBillingOrgIDEx_AD) {
        if(!fillAreaCode(pEventSection)) {
            return false;
        }
    }


    //����ʹ���û����ϻ�ȡ�ļƷѺź��������ORG��ȡ��������
    if (!pEventSection->m_oEventExt.m_iRoamTypeID) {
        if(0 != pEventSection->m_iBillingOrgID)
        {
            pEventSection->m_oEventExt.m_iRoamTypeID = 
                GetRoamTypeByOrgID(pEventSection->m_iBillingOrgID
                ,pEventSection->m_iBillingOrgIDEx_VS);
        }else
        {
            pEventSection->m_oEventExt.m_iRoamTypeID = 
                GetRoamTypeByOrgID(pEventSection->m_iBillingOrgIDEx_AD
                ,pEventSection->m_iBillingOrgIDEx_VS);
            pEventSection->m_iBillingOrgID = pEventSection->m_iBillingOrgIDEx_AD;
        }
    }

    //if(pEventSection->m_oEventExt.m_iRoamTypeID!= ROAMTYPE_NOT_ROAM )
   // {
        if(CALLTYPE_MTC != pEventSection->m_iCallTypeID)
        {
            if(pEventSection->m_sMSC[0]!=0)
            {
                if(MobileRegionMgr::CheckCellEdgeType(pEventSection->m_sMSC,pEventSection->m_sLAC,
                    pEventSection->m_sCellID,pEventSection->m_sStartDate,pEventSection->m_iBillingOrgIDEx_AD,1))
                {
                    pEventSection->m_oEventExt.m_iStdRoamTypeID = ROAMTYPE_ROAM_BORD;
                }
				if((ROAMTYPE_ROAM_BORD != pEventSection->m_oEventExt.m_iStdRoamTypeID)&&
				(pEventSection->m_oEventExt.m_iRoamTypeID== ROAMTYPE_NOT_ROAM ))
				{
					if(MobileRegionMgr::CheckCellEdgeType(pEventSection->m_sMSC,pEventSection->m_sLAC,
						pEventSection->m_sCellID,pEventSection->m_sStartDate,pEventSection->m_iOtherOrgIDEx_AD,1))
					{
						pEventSection->m_oEventExt.m_iStdRoamTypeID = ROAMTYPE_ROAM_BORD_EX;
					}
				}
            }
        }else
        {
            if(pEventSection->m_sMSC[0]!=0)
            {
                if(MobileRegionMgr::CheckCellEdgeType(pEventSection->m_sMSC,
                    pEventSection->m_sCalledLAC,
                    pEventSection->m_sCalledCellID,
                    pEventSection->m_sStartDate,
                    pEventSection->m_iBillingOrgIDEx_AD,1))
                {
                    pEventSection->m_oEventExt.m_iStdRoamTypeID = ROAMTYPE_ROAM_BORD;
                }
				if((ROAMTYPE_ROAM_BORD!= pEventSection->m_oEventExt.m_iStdRoamTypeID)&&
				(pEventSection->m_oEventExt.m_iRoamTypeID== ROAMTYPE_NOT_ROAM ))
				{
					if(MobileRegionMgr::CheckCellEdgeType(pEventSection->m_sMSC,
						pEventSection->m_sCalledLAC,
						pEventSection->m_sCalledCellID,
						pEventSection->m_sStartDate,
						pEventSection->m_iOtherOrgIDEx_AD,1))
					{
						pEventSection->m_oEventExt.m_iStdRoamTypeID = ROAMTYPE_ROAM_BORD_EX;
					}
				}
            }
        }
		if((pEventSection->m_oEventExt.m_iRoamTypeID!= ROAMTYPE_NOT_ROAM )||
		(pEventSection->m_oEventExt.m_iStdRoamTypeID== ROAMTYPE_ROAM_BORD_EX))
		{
			Rule_BillingRoam(pEventSection,iType);
		}else
		{
			Rule_BillingNoRoam(pEventSection);
		}
	return true;
}

int EventTypeMgrEx::GetRoamTypeByDistance(long lDistance)
{

    CDistanceMgr* pDistanceMgr = CDistanceMgr::GetInstance();
    if(0== pDistanceMgr)
        return -1;
    else
    {
        int iRet =0;
        if(!pDistanceMgr->GetRoamTypeByDistance(lDistance,iRet))
            return -1;
        else
            return iRet;
    }
}
int EventTypeMgrEx::GetRoamTypeByOrgID(int iOrgID1,int iOrgID2)
{
    long lDis = OrgLongTypeMgr::GetLongTypeByOrg(iOrgID1,iOrgID2);
    int iRoamType = this->GetRoamTypeByDistance(lDis);
    return iRoamType;
}
int EventTypeMgrEx::GetLongTypeByOrgID(int iOrgID1,int iOrgID2)
{
    long lDis = OrgLongTypeMgr::GetLongTypeByOrg(iOrgID1,iOrgID2);
    int iLongType = GetLongTypeByDistance(lDis);
    return iLongType;
}
int EventTypeMgrEx::GetLongTypeByDistance(long lDistance)
{
    CDistanceMgr* pDistanceMgr = CDistanceMgr::GetInstance();
    if(0== pDistanceMgr)
        return -1;
    else
    {
        int iRet =0;
        if(!pDistanceMgr->GetLongTypeByDistance(lDistance,iRet))
            return -1;
        else
            return iRet;
    }   
    return -1;

}

