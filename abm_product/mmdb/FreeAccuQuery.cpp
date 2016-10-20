/*VER: 13*/ 
#include "FreeAccuQuery.h"
#include "Log.h"
#include <algorithm>
#include <string.h>
#include "TransactionMgr.h"
#include "Exception.h"

FreeAccuQuery * FreeAccuQuery::m_pQuery = 0;
OfferAccuMgr * FreeAccuQuery::m_poAccuMgr = 0;
OfferInsQO   * FreeAccuQuery::m_poOfferInstBuf = 0;
ConditionResult   * FreeAccuQuery::m_poResultBuf = 0;
BillingCycleMgr * FreeAccuQuery::m_poBCM = 0;
ProdOfferDisctAggr * FreeAccuQuery::m_poProdOfferDisctAggr = 0;
ConditionVal  * FreeAccuQuery::m_pConditionValList = 0;
HashList<ConditionVal *> * FreeAccuQuery::m_pConditionVal = 0;

FreeAccuCfg * FreeAccuQuery::m_poAccuCfgList = 0;
ItemName * FreeAccuQuery::m_poOfferNameList = 0;
ItemName * FreeAccuQuery::m_poAccuNameList = 0;
HashList<FreeAccuCfg *> * FreeAccuQuery::m_poAccuIndex = 0;
HashList<FreeAccuCfg *> * FreeAccuQuery::m_poItemIndex = 0;
HashList<ItemName *> * FreeAccuQuery::m_poOfferNameIndex = 0;
HashList<ItemName *> * FreeAccuQuery::m_poAccuNameIndex = 0;
HashList<int> * FreeAccuQuery::m_poOfferID = 0;
HashList<int> * FreeAccuQuery::m_poTableName = 0;
AccuTypeCfg * FreeAccuQuery::m_pCfg = 0;

FreeOfferAttrEx * FreeAccuQuery::m_poAttrExlist = 0;
HashList<FreeOfferAttrEx *> * FreeAccuQuery::m_poAttrExIndex = 0;

BillConditionMgr    * FreeAccuQuery::m_poConditionMgr = 0;
PricingInfoMgr      * FreeAccuQuery::m_poPricingInfoMgr = 0;
HashList<int>		* FreeAccuQuery::m_poOfferSectionParser = 0;
EventSection		* FreeAccuQuery::pEvent = 0;

int	FreeAccuQuery::m_iFirstMonth = 0;
int FreeAccuQuery::m_iMemFlag = 0;
int	FreeAccuQuery::m_iDisplayRealCharge = 0;
int	FreeAccuQuery::m_iDisplayRealAggr = 0;
int FreeAccuQuery::m_iCount = 0;
int FreeAccuQuery::m_iCountVal = 0;
bool FreeAccuQuery::m_bUploaded = false;

void FreeAccuQuery::loadInit()
{
	if(!G_PORGMGR)
	{
         G_PORGMGR = new OrgMgr ();
		 if (!G_PORGMGR){
			 Log::log(0,"FreeAccuQuery::loadInit�� new OrgMgr() �����ڴ�ռ�ʧ��1��");
			 THROW(1);
		 }
    }
	
    m_poAccuMgr = new OfferAccuMgr ();
	if(!m_poAccuMgr){
		Log::log(0,"FreeAccuQuery::loadInit�� new OfferAccuMgr() �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}
	m_poProdOfferDisctAggr = new ProdOfferDisctAggr();
	if(!m_poProdOfferDisctAggr){
		Log::log(0,"FreeAccuQuery::loadInit�� new ProdOfferDisctAggr() �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}
	m_poOfferInstBuf = new OfferInsQO[MAX_OFFER_INSTANCE_NUMBER];
	if(!m_poOfferInstBuf){
		Log::log(0,"FreeAccuQuery::loadInit�� new OfferInsQO[%d] �����ڴ�ռ�ʧ��1��",MAX_OFFER_INSTANCE_NUMBER);
		THROW(1);
	}
	m_poResultBuf = new ConditionResult[MAX_PRICING_COMBINE_NUMBER];
	if(!m_poResultBuf){
		Log::log(0,"FreeAccuQuery::loadInit�� new ConditionResult[%d] �����ڴ�ռ�ʧ��1��",MAX_PRICING_COMBINE_NUMBER);
		THROW(1);
	}
	m_poBCM = new BillingCycleMgr ();
	if(!m_poBCM){
		Log::log(0,"FreeAccuQuery::loadInit�� new m_poBCM() �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}
	m_pCfg = new AccuTypeCfg();
	if(!m_pCfg){
		Log::log(0,"FreeAccuQuery::loadInit�� new AccuTypeCfg() �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}
	m_poOfferID =  new HashList<int> (MAX_OFFER_INSTANCE_NUMBER>>1);
	if (!m_poOfferID){
		Log::log(0,"FreeAccuQuery::loadInit�� new m_poOfferID �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}
	m_poOfferSectionParser =  new HashList<int> (MAX_OFFER_INSTANCE_NUMBER>>1);
	if (!m_poOfferSectionParser){
		Log::log(0,"FreeAccuQuery::loadInit�� new m_poOfferSectionParser �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}
	m_poTableName =  new HashList<int> (5);

	char strFlag[2];
	memset(strFlag,0,sizeof(strFlag));
	if(ParamDefineMgr::getParam("ACCU_QUERY", "CONFIG_MEM", strFlag,1))
	{
		m_iMemFlag = atoi(strFlag);//�������ڴ��500�͵��ۻ��������ж�
	}
	memset(strFlag,0,sizeof(strFlag));
	if(ParamDefineMgr::getParam("ACCU_QUERY", "FIRST_MONTH", strFlag,1))
	{
		m_iFirstMonth = atoi(strFlag);
	}
	memset(strFlag,0,sizeof(strFlag));
	if(ParamDefineMgr::getParam("ACCU_QUERY", "REAL_CHARGE", strFlag,1))
	{
		m_iDisplayRealCharge = atoi(strFlag);
	}
	memset(strFlag,0,sizeof(strFlag));
	if(ParamDefineMgr::getParam("ACCU_QUERY", "REAL_AGGR", strFlag,1))
	{
		m_iDisplayRealAggr = atoi(strFlag);
	}
}

void FreeAccuQuery::unload()
{
	m_bUploaded = false;
	
	if(m_poAccuMgr != NULL){
		delete m_poAccuMgr;
		m_poAccuMgr = NULL;
	}
	if(m_poOfferInstBuf != NULL){
		delete [] m_poOfferInstBuf;
		m_poOfferInstBuf = NULL;
	}
	if(m_poResultBuf != NULL){
		delete [] m_poResultBuf;
		m_poResultBuf = NULL;
	}
	for (int i= 0;i<m_iCountVal ;i++)
	{
		if (m_pConditionValList[i].m_poParamID1 != NULL){
			delete m_pConditionValList[i].m_poParamID1;
			m_pConditionValList[i].m_poParamID1 = NULL;
		}
		if (m_pConditionValList[i].m_poDesParamID != NULL){
			delete m_pConditionValList[i].m_poDesParamID;
			m_pConditionValList[i].m_poDesParamID = NULL;
		}
	}
	if(m_pConditionValList != NULL){
		delete [] m_pConditionValList;
		m_pConditionValList = NULL;
	}
	if(m_pConditionVal != NULL){
		delete m_pConditionVal;
		m_pConditionVal = NULL;
	}
	if(m_poBCM != NULL){
		delete m_poBCM;
		m_poBCM = NULL;
	}
	if(m_poProdOfferDisctAggr != NULL){
		delete m_poProdOfferDisctAggr;
		m_poProdOfferDisctAggr = NULL;
	}
	if(m_poAccuIndex != NULL){
		delete m_poAccuIndex;
		m_poAccuIndex = NULL;
	}
	if(m_poItemIndex != NULL){
		delete m_poItemIndex;
		m_poItemIndex = NULL;
	}
	for (int i=0;i<m_iCount;i++)
	{
		if (m_poAccuCfgList[i].m_poObject != NULL){
			delete m_poAccuCfgList[i].m_poObject;
			m_poAccuCfgList[i].m_poObject = NULL;
		}
	}
	if(m_poAccuCfgList != NULL){
		delete []m_poAccuCfgList;
		m_poAccuCfgList = NULL;
	}
	if(m_poOfferNameIndex != NULL){
		delete m_poOfferNameIndex;
		m_poOfferNameIndex = NULL;
	}
	if(m_poAccuNameIndex != NULL){
		delete m_poAccuNameIndex;
		m_poAccuNameIndex = NULL;
	}
	if(m_poOfferNameList != NULL){
		delete[] m_poOfferNameList;
		m_poOfferNameList = NULL;
	}
	if(m_poAccuNameList != NULL){
		delete[] m_poAccuNameList;
		m_poAccuNameList  = NULL;
	}

	if(m_poOfferID != NULL){
		delete m_poOfferID;
		m_poOfferID = NULL;
	}
	if(m_poTableName != NULL){
		delete m_poTableName;
		m_poTableName = NULL;
	}
	if(m_pCfg != NULL){
		delete m_pCfg;
		m_pCfg = NULL;
	}
	if (m_poConditionMgr != NULL)
	{
		delete m_poConditionMgr;
		m_poConditionMgr = NULL;
	}
	if (m_poPricingInfoMgr != NULL){
		delete m_poPricingInfoMgr;
		m_poPricingInfoMgr = NULL;
	}
	if (m_poOfferSectionParser != NULL){
		delete m_poOfferSectionParser;
		m_poOfferSectionParser = NULL;
	}
	if (pEvent != NULL){
		delete pEvent;
		pEvent = NULL;
	}
	if(m_poAttrExlist != NULL){
		delete[] m_poAttrExlist;
		m_poAttrExlist = NULL;
	}
	if(m_poAttrExIndex != NULL){
		delete m_poAttrExIndex;
		m_poAttrExIndex = NULL;
	}
	if (G_PORGMGR != NULL){
		delete G_PORGMGR;
		G_PORGMGR = NULL;
	}
	if (G_PACCTITEMMGR != NULL){
		delete G_PACCTITEMMGR;
		G_PACCTITEMMGR = NULL;
	}
	if (G_PTRANSMGR != NULL){
		delete G_PTRANSMGR;
		G_PTRANSMGR = NULL;
	}
	if (G_POFFERMGR != NULL){
		delete G_POFFERMGR;
		G_POFFERMGR = NULL;
	}
	m_iFirstMonth = 0;
	m_iMemFlag = 0;
	m_iDisplayRealCharge = 0;
	m_iDisplayRealAggr = 0;
	m_iCount = 0;
	m_iCountVal = 0;
}

int FreeAccuQuery::remindQueryAccu(vector<FreeAccuQueryResult> &vResult, EventSection * poEvent, OfferInsQO *pQO)
{
    int iRet = 0;

    if(NULL == pEvent){
        pEvent = new EventSection();
    }
    memcpy(pEvent, poEvent, sizeof(EventSection));
    strncpy(m_sDate, pEvent->m_sStartDate, 14);
    iRet = queryOfferInst(vResult, pEvent->m_iServID, pQO);
    
    return iRet;
}

int FreeAccuQuery::query(vector<FreeAccuQueryResult> &vResult,
long lServID, char *sDate)
{
    int i, j, r;
	int iTemp= 0,m=0;

    if (!sDate) {
        Date d;
        strcpy (m_sDate, d.toString ());
	}else{
		strcpy(m_sDate,sDate);
	}
    
    #ifdef SHM_FILE_USERINFO
    G_SHMFILE;
    #endif

    if (!G_PUSERINFO->getServ (G_SERV, lServID, m_sDate))
        return 0;
    
    j = G_PSERV->getOfferIns (m_poOfferInstBuf);
    if (!j) return 0;

    r = 0;

	m_poOfferID->clearAll();
    for (i=0; i<j; i++) {
		if(m_poOfferID->get((m_poOfferInstBuf+i)->m_poOfferIns->m_iOfferID,&m)){
			continue;
		}else{
			m_poOfferID->add((m_poOfferInstBuf+i)->m_poOfferIns->m_iOfferID,i);
		}
        iTemp = queryOfferInst (vResult, lServID, m_poOfferInstBuf+i);
		if(iTemp == -1){//�������ݲ�ȫ
			return 0;
		}
		r +=iTemp;
		if( 0 == iTemp){
			m_poOfferID->remove((m_poOfferInstBuf+i)->m_poOfferIns->m_iOfferID);
		}
    }

	//������ӡΪ��ʱʹ�ã���ʽ��������Ҫ��ȥ->��ʼ
	/*cout<<"--------------------�ƷѺ�̨��¼�� "<<r<<" ��-------------------------"<<endl;
	char tab[10];
	char tab2[10];
	strcpy(tab,"       ");
	strcpy(tab2,"  ");
	vector<FreeAccuQueryResult>::iterator iter=vResult.begin();
	FreeAccuQueryResult  p;
	int t=0;
	cout<<"��Ʒ����"<<tab<<"�ۻ�������"<<tab<<"��ʼʱ��"<<tab<<"����ʱ��"<<tab<<"��λ"
			<<tab<<"����"<<tab<<"������"<<tab<<"ʣ��"<<tab<<"��ע"<<endl;
	while(iter!=vResult.end())
	{
		p=(FreeAccuQueryResult )vResult[t];
		cout<<p.m_sOfferName<<tab2<<p.m_sAccuName<<tab2<<p.m_sStartTime<<tab2<<p.m_sEndTime<<tab2<<p.m_sUnitName
			<<tab2<<p.m_sTotal<<tab2<<p.m_sAlready<<tab2<<p.m_sLeft<<tab2<<p.m_sRemark<<endl;
    	iter++;
    	t++;
	}
	cout<<"--------------------��ӡ�������Ѻ�̨��¼������--------------------"<<endl;*/
    //������ӡΪ��ʱʹ�ã���ʽ��������Ҫ��ȥ->����

	return r;
}

int FreeAccuQuery::queryDB(vector<FreeAccuQueryResult> &vResult,
long lServID, char *sDate)
{
    int i, j, r;
	int iTemp= 0,m=0;

    if (!sDate) {
        Date d;
        strcpy (m_sDate, d.toString ());
	}else{
		strcpy(m_sDate,sDate);
	}
    
    #ifdef SHM_FILE_USERINFO
    G_SHMFILE;
    #endif

    if (!G_PUSERINFO->getServ (G_SERV, lServID, m_sDate))
        return 0;
    
    j = G_PSERV->getOfferIns (m_poOfferInstBuf);
    if (!j) return 0;

    r = 0;

	m_poOfferID->clearAll();
    for (i=0; i<j; i++) {
		if(m_poOfferID->get((m_poOfferInstBuf+i)->m_poOfferIns->m_iOfferID,&m)){
			continue;
		}else{
			m_poOfferID->add((m_poOfferInstBuf+i)->m_poOfferIns->m_iOfferID,i);
		}
        iTemp = queryOfferInstDB(vResult, lServID, m_poOfferInstBuf+i);
		if(iTemp == -1){//�������ݲ�ȫ
			return 0;
		}
		r +=iTemp;
		if( 0 == iTemp){
			m_poOfferID->remove((m_poOfferInstBuf+i)->m_poOfferIns->m_iOfferID);
		}
    }
	return r;
}

int FreeAccuQuery::getConditionVal(FreeAccuCfg * pAccuCfg,ConditionResult * pOutConditionResult)
{
	int i = 0;
	long lTemp = 0;
	ConditionVal * p;
	int iMember;
	if(m_pConditionVal->get(G_POFFERINSQO->m_poOfferIns->m_iOfferID,&p))
	{
		while(p)
		{
			iMember = G_POFFERINSQO->m_poDetailIns->m_iMemberID;
			if (p->m_iObjectID!=0 && p->m_iObjectID!=iMember && p->m_iObjectID!=-1)
			{
                p = p->m_poNext;
                continue;
            }
			if(!p->m_poLifeCycle->check(m_sDate, G_POFFERINSQO->m_poOfferIns))
			{
                p = p->m_poNext;
                continue;
            }
			if( (p->m_poParamID1->m_iAttrID == pAccuCfg->m_poObject->m_iAttrID)
				&& (p->m_poParamID1->m_iOwner == pAccuCfg->m_poObject->m_iOwner)
				&& (p->m_poParamID1->m_lValue == pAccuCfg->m_poObject->m_lValue))
			{
				pOutConditionResult[i].m_iCombineID = p->m_iCombineID;
				pOutConditionResult[i].m_iMeasureSeq = p->m_iMeasureSeq;
				FreeOfferAttrEx *pAttrEx;
				if(pAccuCfg->m_iAdjustFlag==JUST_FLAG_ESTU && m_poAttrExIndex->get(pAccuCfg->m_iQueryID,&pAttrEx) )
				{
					while(pAttrEx)
					{
						char *cAttrValue= G_POFFERINSQO->getOfferAttr(pAttrEx->m_iAttrID,m_sDate);
						if(cAttrValue && !strcmp(pAttrEx->m_sAttrValue,cAttrValue) )
						{
							lTemp=pAttrEx->m_iValue;
							if(pAttrEx->m_iType==2)
							{
								char *sVal=G_POFFERINSQO->getOfferAttr(pAttrEx->m_iValue,m_sDate);
								if (sVal){
									lTemp = atol(sVal);
								}else{
									lTemp = 0;
								}
							}
							break;
						}
						pAttrEx=pAttrEx->m_poNext;
					}
					pOutConditionResult[i].m_lValue = lTemp;
				}
				else
				{
					lTemp = p->m_poDesParamID->m_lValue;
                    
                    if(p->m_poDesParamID->m_iAttrID == 5002)  //5002ʱȡӪҵ����,���⴦��pOutConditionResult[i].m_lValue
					{
                        char *sVal = 0;
                        
                        if(p->m_poDesParamID->m_iOwner == 12 && p->m_poDesParamID->m_iExt == 0)  //�ϵ�����ʽ���:5002_12_����IDʱ,�ж�m_iAttrID=5002��m_iOwner=12ʱ,�����ϵķ�ʽ����
                        {
                            sVal = G_POFFERINSQO->getOfferAttr(p->m_poDesParamID->m_lValue, m_sDate);
                        }
                        else if(p->m_poDesParamID->m_iExt == 1)  //�µ�����ʽ���:5002_D��Ʒ��ԱID_����IDʱ,������m_iExt=1��m_iAttrID=5002ʱ,�����µĴ���
                        {
                            //���ȸ��ݵ�ǰ���۵���Ʒʵ��ȡ�õ�ǰ��Ʒ��ϸ�µ�����serv
                            int iOfferDetailInsCount = 0;
                            OfferDetailIns * offerDetailIns[MAX_OFFER_MEMBERS];
                            iOfferDetailInsCount = G_POFFERINSQO->getOfferInsMember(offerDetailIns, p->m_poDesParamID->m_iOwner, m_sDate);
                            
                            for(int i = 0; i < iOfferDetailInsCount; i++)
                            {
                                if(SERV_INSTANCE_TYPE == offerDetailIns[i]->m_iInsType)
                                {
                                    static Serv serv;
                                    bool retValue = false;
                                    retValue = G_PUSERINFO->getServ(serv, offerDetailIns[i]->m_lInsID, m_sDate);
                                    if(retValue)
                                    {
                                        //ȡ�û�������Ч��Ʒʵ��
                                        int iOfferInsCount = 0;
                                        OfferInsQO ret[MAX_OFFER_INSTANCE_NUMBER];
                                        iOfferInsCount = serv.getOfferIns(ret, m_sDate);

                                        for(int j = 0; j < iOfferInsCount; j++)
                                        {
                                            sVal = ret[j].getOfferAttr(p->m_poDesParamID->m_lValue, m_sDate);
                                            
                                            if(sVal)            //�ҵ���һ������ֵ,���˳�,����������Ʒ
                                            {
                                                break;
                                            }
                                        }
                                    }
                                }
                                if(sVal)    //�ҵ���һ������ֵ,�Ͳ��������û�����
                                {
                                    break;
                                }
                            }
                            
                        }
                        else if(p->m_poDesParamID->m_iExt == 2)  //�µ�����ʽ���:5002_F��ƷID_����IDʱ,������m_iExt=2��m_iAttrID=5002ʱ,�����µĴ���
                        {
                            //ȡ�û���������Ʒʵ��
                            int iOfferInsCount = 0;
                            OfferInsQO ret[MAX_OFFER_INSTANCE_NUMBER];
                            iOfferInsCount = G_SERV.getOfferIns(ret);
                            for(int i = 0; i < iOfferInsCount; i++)
                            {
                                if(p->m_poDesParamID->m_iOwner == ret[i].m_poOfferIns->m_iOfferID)
                                {
                                    sVal = ret[i].getOfferAttr(p->m_poDesParamID->m_lValue, m_sDate);
                                }
                                if(sVal)
                                {
                                    break;
                                }
                            }
                        }                        

                        if(sVal){
                            lTemp = atol(sVal);
                        }else{
                            lTemp = 0;
                        }
                    }
					/*if(p->m_poDesParamID->m_iOwner == 12 && p->m_poDesParamID->m_iAttrID == 5002)
					{
						char * sVal = G_POFFERINSQO->getOfferAttr(p->m_poDesParamID->m_lValue,m_sDate);
						if(sVal){
							lTemp = atol(sVal);
						}else{
							lTemp = 0;
						}
					}*/
                    
					pOutConditionResult[i].m_lValue = lTemp;
				}
				i++;
				if(i == MAX_PRICING_COMBINE_NUMBER)
				{
					Log::log(0,"����������Ѵ�ϵͳ������ %d �������н�ȡ",MAX_PRICING_COMBINE_NUMBER);
					return i;
				}
			}
			p = p->m_poNext;
		}
	}
	return i;
}

int FreeAccuQuery::getAggrByServ(long lServID,long lOfferInstID,char * sEffDate, char * sExpDate,long &lVal)
{
	int iRet = 0;
	char sSqlcode[500];
	TOCIQuery qry(Environment::getDBConn());
	//��Ҫ�������SQL��ִ�мƻ�����֤Ҫ�õ�offer_inst_id������
	sprintf (sSqlcode, 	"SELECT nvl(value,0) value "
						" FROM b_serv_aggr_query "
						" WHERE serv_id=%ld "
						" AND offer_inst_id=%ld "
						" AND eff_date=to_date('%s','yyyymmddhh24miss') "
						" AND exp_date=to_date('%s','yyyymmddhh24miss') ",
						lServID,lOfferInstID,sEffDate,sExpDate);

	qry.setSQL (sSqlcode);
	qry.open ();
	while (qry.next ())
	{
		lVal = lVal+qry.field(0).asLong();
		iRet++;
	}
	qry.close ();
	qry.commit();
	return iRet;
}

bool FreeAccuQuery::getCurDays(int &iUseDays,int &iToalDays)
{
	bool bRet = false;
	char sDate[15];
	iUseDays = 0;
	iToalDays = 0;
	strcpy(sDate,G_SERV.getCreateDate());
	BillingCycle * pCycle = m_poBCM->getOccurBillingCycle (1,m_sDate);
	if(pCycle)
	{
		if(strcmp(sDate,pCycle->getStartDate())>=0 && 
			strcmp(sDate,pCycle->getEndDate())<0)
		{
			Date dCreateDate;
			Date dCycleEnd;
			dCreateDate.parse(sDate);
			dCycleEnd.parse(pCycle->getEndDate());
			iUseDays = dCycleEnd-dCreateDate;
			iToalDays = pCycle->getDays();
			bRet = true;
			if(iUseDays<=0 || iToalDays<=0)
			{
				bRet =  false;
				Log::log(0,"�����û��Ŀ���ʱ��õ�����������ȷ������������Ϊ[%d],��ʹ������Ϊ[%d]"
				"���Խ��",iToalDays,iUseDays);
			}
		}
	}
	return bRet;
}

void FreeAccuQuery::addResult(vector<FreeAccuQueryResult> &vResult,char * sOfferName,
							  char * sAccuName,char * sEffDate,char * sExpDate,long lMaxVal,
							  long lUsedVal,FreeAccuCfg * pAccuCfg,int iCalRuleID,
							  int iRemindTypeID,char * sSpliby)
{
	int iTotalDays = 1,iUseDays = 0;
	FreeAccuQueryResult  result;
	memset(&result,0,sizeof(FreeAccuQueryResult));
	float fResult =0.0;
#ifdef DEF_SICHUAN
	int iTemp = 0;
#endif
	strcpy(result.m_sOfferName,sOfferName);
	strcpy(result.m_sAccuName,sAccuName);
	strcpy(result.m_sStartTime,sEffDate);
	strcpy(result.m_sEndTime,sExpDate);
	strcpy(result.m_sUnitName , pAccuCfg->m_sUnitName);
	strcpy(result.m_sRemark,pAccuCfg->m_sQueryDesc);
	result.m_iQueryID = pAccuCfg->m_iQueryID;
	result.m_iQueryFlag = pAccuCfg->m_iQueryFlag;
	result.m_iOfferID = pAccuCfg->m_iOfferID;
	result.m_iCalRuleID = iCalRuleID;
	result.m_iRemindType = iRemindTypeID;
    result.m_iAccuTypeID = pAccuCfg->m_poObject->m_lValue;
    strcpy(result.m_sExtendItem, pAccuCfg->m_sExtendItem);          ///2010-4-29
	if (NULL != sSpliby){
		strncpy(result.m_sSplitby,sSpliby,sizeof(result.m_sSplitby));
	}

	if(pAccuCfg->m_poObject->m_iAttrID == 500)
        result.m_iAccuType = 0;
    else
        result.m_iAccuType = 1;
    
	if(m_iFirstMonth ==1 && pAccuCfg->m_iOperFlag != OPER_FLAG_OFFERNOPERCENT && getCurDays(iUseDays,iTotalDays))
	{
		fResult = (float)(((float)(lMaxVal*iUseDays)/iTotalDays)*pAccuCfg->m_iUnitNume)/pAccuCfg->m_iUnitDeno;
	}else
	{
		fResult = (float)(lMaxVal*pAccuCfg->m_iUnitNume)/pAccuCfg->m_iUnitDeno;
	}
	fResult = MyRound(fResult);
#ifdef DEF_SICHUAN
	iTemp = fResult;
	if(iTemp == fResult){
		sprintf(result.m_sTotal,"%d",iTemp);
	}
	else{
		sprintf(result.m_sTotal,"%.2f",fResult);
	}
#else
	sprintf(result.m_sTotal,"%.2f",fResult);
#endif
	fResult = (float)(lUsedVal*pAccuCfg->m_iUnitNume)/pAccuCfg->m_iUnitDeno;
	fResult =  MyRound(fResult);
	if(!(m_iDisplayRealCharge == 1 && pAccuCfg->m_poObject->m_iAttrID == 500)
		&& !(m_iDisplayRealAggr == 1 && pAccuCfg->m_poObject->m_iAttrID != 500))
	{
		if((fResult - atof(result.m_sTotal))>0){
			fResult = atof(result.m_sTotal);
		}
	}
#ifdef DEF_SICHUAN
	iTemp = fResult;
	if(iTemp == fResult){
		sprintf(result.m_sAlready,"%d",iTemp);
	}
	else{
		sprintf(result.m_sAlready,"%.2f",fResult);
	}
#else
	sprintf(result.m_sAlready,"%.2f",fResult);
#endif
	fResult = atof(result.m_sTotal)-atof(result.m_sAlready);
	if(fResult<0){
		fResult = 0;
	}
#ifdef DEF_SICHUAN
	iTemp = fResult;
	if(iTemp == fResult){
		sprintf(result.m_sLeft,"%d",iTemp);
	}
	else{
		sprintf(result.m_sLeft,"%.2f",fResult);
	}
#else
	sprintf(result.m_sLeft,"%.2f",fResult);
#endif
	vResult.push_back(result);
	return;
}

int FreeAccuQuery::operProcess(vector<FreeAccuQueryResult> &vResult,long lServID,
		OfferInsQO *pQO,FreeAccuCfg * pAccuCfg)
{
	int iRet = 0;
	char sOfferName[128];
	char sAccuName[128];
	memset(sOfferName,0,sizeof(sOfferName));
	memset(sAccuName,0,sizeof(sAccuName));
	ItemName * sName;
	AccuType *pAccu = m_pCfg->getAccuType (pAccuCfg->m_poObject->m_lValue);
	if (!pAccu)
	{
		return iRet;
	}
	char sBeginTime[15];
	char sEndTime[15];
	int iReturnFlag = m_pCfg->getCycleTime(pQO,m_iBillingCycleID,
		pAccu->m_iCycleTypeID,m_sDate,sBeginTime,sEndTime);
	if (iReturnFlag == 0)
	{
		return iRet;
	}

	long lVal = 0;
	int iNum = 0;
	switch(pAccuCfg->m_iOperFlag)
	{
	case 1:
		iNum = getAggrByServ(lServID,pQO->m_poOfferIns->m_lProdOfferInsID,sBeginTime,sEndTime,lVal);
		if(iNum>0)
		{
			if(m_poOfferNameIndex->get(pQO->m_poOfferIns->m_iOfferID,&sName)){
				strcpy(sOfferName,sName->m_sName);
			}
			if(m_poAccuNameIndex->get(pAccu->m_iAccuTypeID,&sName)){
				strcpy(sAccuName,sName->m_sName);
			}
			addResult(vResult,sOfferName,sAccuName,sBeginTime,
				sEndTime,pAccuCfg->m_iAdjustVal,lVal,pAccuCfg);
			iRet++;
		}
		break;
	default:
		break;
	}
	return iRet;
}

void FreeAccuQuery::parserObject(FreeAccuCfg * pAccuCfg)
{
	if(pAccuCfg == NULL) return;
	if(!strstr(pAccuCfg->m_sQueryDesc,"$")) return;
	int iAttr;
	char sTemp[2000];
	char * sVal;
	char sResult[10];
	string strTemp(pAccuCfg->m_sQueryDesc);
	const char * p = pAccuCfg->m_sQueryDesc;
	const char * p2,*p1;
	p1 = strstr(pAccuCfg->m_sQueryDesc,"$");
	while(p1)
	{
		p1++;
		p2 = strstr(p1,"$");
		if(p2)
		{
			memset(sTemp,0,sizeof(sTemp));
			strncpy(sTemp,p1,p2-p1);
			p2++;
			if(sTemp[0])
			{
				/*iAttr = atoi(sTemp);
				sVal = G_POFFERINSQO->getOfferAttr(iAttr,m_sDate);*/
				memset(sResult,0,sizeof(sResult));
				AttrValueCalc m_pAttrcalc;
				int iResult = m_pAttrcalc.run(&sTemp[0],G_POFFERINSQO,&m_sDate[0]);
				sprintf(sResult,"%d",iResult);
				sResult[9]='\0';
				sVal = sResult;
			}else{
				sVal = NULL;
			}
			if(sVal)
			{
				strTemp = strTemp.replace((p1-p)-1,(p2-p1)+1,sVal);
			}else{
				strTemp = strTemp.replace((p1-p)-1,(p2-p1)+1,"");
			}
			p = strTemp.c_str();
			p1 = strstr(p,"$");
		}else
		{
			p1 = strstr(p1,"$");
		}
	}
	memset(pAccuCfg->m_sQueryDesc,0,sizeof(pAccuCfg->m_sQueryDesc));
	strncpy(pAccuCfg->m_sQueryDesc,strTemp.c_str(),sizeof(pAccuCfg->m_sQueryDesc)-1);
}

int FreeAccuQuery::queryOfferInst(vector<FreeAccuQueryResult> &vResult,
long lServID, OfferInsQO *pQO)
{
    int iRet = 0;
	ID * pID;
	long lMaxVal =-1;
	//long lResult = 0;
	float fResult =0.0;
	long lTemp = 0;
	int iMeasureSeq  = 0;
	ItemName * sName;
	char sOfferName[128];
	char sAccuName[128];
	vector<long> lVec;

	BillingCycle const * pCycle = 
        m_poBCM->getOccurBillingCycle (1,m_sDate);
    if (pCycle) {
		m_iBillingCycleID = pCycle->getBillingCycleID ();
	}else{
		Log::log(0,"����ʱ���ѯ���ڲ����ڣ�ʱ��Ϊ��%s",m_sDate);
		return -1;
	}
    //1, ���ۼ�ʹ����---��Ҫ ��Ʒʵ��, �ۼ�������, ����ʵ��
    FreeAccuCfg * pAccuCfg;
	G_POFFERINSQO = pQO;
    if (m_poAccuIndex->get (pQO->m_poOfferIns->m_iOfferID, &pAccuCfg)) 
	{		
		long lServ_ID = 0;
		long lMemberID = 0;
		while (pAccuCfg) 
		{
			if(pAccuCfg->m_iOperFlag == OPER_FLAG_CG && pAccuCfg->m_iAdjustFlag == 1)
			{
				iRet =  iRet + operProcess(vResult,lServID,pQO,pAccuCfg);
				pAccuCfg = pAccuCfg->m_poNext;
				continue;
			}
			memset(sOfferName,0,sizeof(sOfferName));
			memset(sAccuName,0,sizeof(sAccuName));
			lMaxVal = -1;
			int iCount = 0;
			if (OPER_FLAG_SECTIONPARSER == pAccuCfg->m_iOperFlag)
			{
				pEvent->m_iServID = lServID;
				strcpy(pEvent->m_sStartDate,m_sDate);
				m_poResultBuf[0].m_lValue = 0;
				m_poResultBuf[0].m_iCombineID = 0;
				m_poResultBuf[0].m_iMeasureSeq = 0;
				if (m_poPricingInfoMgr->getSectionValue(pAccuCfg->m_poObject,m_poResultBuf[0].m_iCombineID,
					m_poResultBuf[0].m_lValue,m_poResultBuf[0].m_iMeasureSeq,pEvent)){
					iCount = 1;
				}
			} else if (OPER_FLAG_SECTIONPARSER_IN == pAccuCfg->m_iOperFlag) 
			{
				pEvent->m_iServID = lServID;
				strcpy(pEvent->m_sStartDate,m_sDate);
				lVec.clear();
				iCount = m_poPricingInfoMgr->getSectionValue(pAccuCfg->m_poObject,lVec,pEvent);
				sort(lVec.begin(),lVec.end());
			}else
			{
				iCount = getConditionVal(pAccuCfg,m_poResultBuf);
			}
			if ( pAccuCfg->m_iOperFlag != OPER_FLAG_SECTIONPARSER_IN ) 
			{
				for (int m=0;m<iCount;m++)
				{
					if(lMaxVal<m_poResultBuf[m].m_lValue)
					{
						lMaxVal=m_poResultBuf[m].m_lValue;
					}
				}
			}
			if(pAccuCfg->m_iAdjustFlag == 1)
			{
				if(lMaxVal == -1){
					lMaxVal = 0;
				}
				lMaxVal = lMaxVal+pAccuCfg->m_iAdjustVal;
			}
			if(lMaxVal<=0 && pAccuCfg->m_iOperFlag != OPER_FLAG_SECTIONPARSER_IN){
				pAccuCfg = pAccuCfg->m_poNext;
				continue;
			}

			AccuType *pAccu = m_pCfg->getAccuType (pAccuCfg->m_poObject->m_lValue);
			if (!pAccu) {
				pAccuCfg = pAccuCfg->m_poNext;
				continue;
			}

			CycleInstance * cycle = m_poAccuMgr->getCycleInst (
							G_POFFERINSQO, pAccu, m_sDate, m_iBillingCycleID);

			if (!cycle) {
				pAccuCfg = pAccuCfg->m_poNext;
				continue;
			}
	        
			lServ_ID = 0;
			lMemberID = 0;
	    
			switch (pAccu->m_sAccuLevel[2]) {
			case '3':
				lServ_ID = lServID;
				break;
			case '2':
				lMemberID = G_POFFERINSQO->m_poDetailIns->m_lOfferDetailInsID;
				break;
			default:
				break;
			}

            vector <AccuData > vpAccuData;
            vpAccuData.clear();
            //��ѯ�������������ļ�¼
            int iResult = m_poAccuMgr->getAllAccByInst (G_POFFERINSQO->m_poOfferIns->m_lProdOfferInsID, 
                                                        pAccu->m_iAccuTypeID,
                                                        cycle->m_lCycleInstID,
                                                        m_iBillingCycleID,
                                                        lServ_ID, lMemberID,vpAccuData,
                                                        pAccuCfg->m_iMergeFlag);
			if(m_poOfferNameIndex->get(pQO->m_poOfferIns->m_iOfferID,&sName)){
				strcpy(sOfferName,sName->m_sName);
			}
			int iCalRuleID = 0;
			int iRemindTypeID = 0;
			if(m_poAccuNameIndex->get(pAccu->m_iAccuTypeID,&sName)){
				strcpy(sAccuName,sName->m_sName);
				iCalRuleID = sName->m_iCalRuleId;
				iRemindTypeID = sName->m_RemindTypeID;
			}
			if(pAccuCfg->m_iOperFlag == OPER_FLAG_PARSER){
				parserObject(pAccuCfg);
			}
			//��Ҫ���������
			if (0 == iResult){
				if (pAccuCfg->m_iOperFlag == OPER_FLAG_SECTIONPARSER_IN)
				{
					if (!iCount){
						pAccuCfg = pAccuCfg->m_poNext;
						continue;
					}
					for(int j = 0; j < iCount; ++j)
					{
						if( lVec[j] )
						{
							lMaxVal = lVec[j];
							break;
						}
					}
				}
				addResult(vResult,sOfferName,sAccuName,cycle->m_sBeginTime,
					cycle->m_sEndTime,lMaxVal,0,pAccuCfg,iCalRuleID,iRemindTypeID);
				iRet++;
			}else{
				bool bMax = false;
				if(pAccuCfg->m_iOperFlag == OPER_FLAG_SECTIONPARSER_IN){
					if (!iCount){
						pAccuCfg = pAccuCfg->m_poNext;
						continue;
					}
					bMax = true;
				}
				for (int i=0;i<iResult;i++)
				{
					if(bMax)
					{
						lMaxVal = lVec[lVec.size()-1];
						for(int j = 0; j < iCount; ++j)
						{
                            if (lVec[j] >= vpAccuData[i].m_lValue) {
								lMaxVal = lVec[j];
								break;
							}
						}
					}
					addResult(vResult,sOfferName,sAccuName,cycle->m_sBeginTime,
						cycle->m_sEndTime,lMaxVal,vpAccuData[i].m_lValue,
						pAccuCfg,iCalRuleID,iRemindTypeID,vpAccuData[i].m_sSplitby);
					iRet++;
				}
			}
			pAccuCfg = pAccuCfg->m_poNext;
		}
	}

    //2, ���ʵ��Ż�---��Ҫ֪��combine_id, pricing_measure, ����
    FreeAccuCfg * pItemCfg;
	iMeasureSeq  = 0;
	long lTempVal = 0;
    if (m_poItemIndex->get (pQO->m_poOfferIns->m_iOfferID, &pItemCfg)) 
	{
		Value *pResult = new Value();
		if(!pResult){
			Log::log(0,"FreeAccuQuery��pResult�����ڴ�ռ�ʧ�ܣ�");
			return iRet;
		}
		while (pItemCfg) 
		{
			memset(sOfferName,0,sizeof(sOfferName));
			memset(sAccuName,0,sizeof(sAccuName));
			m_iComNum = 0;
			lTempVal = 0;
			lMaxVal = -1;
			int iCount = 0;
			if (OPER_FLAG_SECTIONPARSER == pItemCfg->m_iOperFlag)
			{
				m_poResultBuf[0].m_iCombineID = 0;
				m_poResultBuf[0].m_lValue = 0;
				m_poResultBuf[0].m_iMeasureSeq = 0;
				pEvent->m_iServID = lServID;
				strcpy(pEvent->m_sStartDate,m_sDate);
				if (m_poPricingInfoMgr->getSectionValue(pItemCfg->m_poObject,m_poResultBuf[0].m_iCombineID,
					m_poResultBuf[0].m_lValue,m_poResultBuf[0].m_iMeasureSeq,pEvent)){
					iCount = 1;
				}
			}else
			{
				iCount = getConditionVal(pItemCfg,m_poResultBuf);
			}
			for (int m=0;m<iCount;m++)
			{
				if(lMaxVal<=m_poResultBuf[m].m_lValue)
				{
					lMaxVal=m_poResultBuf[m].m_lValue;
					iMeasureSeq = m_poResultBuf[m].m_iMeasureSeq;
					m_ioCombineID[m_iComNum] = m_poResultBuf[m].m_iCombineID;
					m_iComNum ++;
				}
			}
			if(pItemCfg->m_iAdjustFlag == 1)
			{
				if(lMaxVal == -1){
					lMaxVal = 0;
				}
				lMaxVal = lMaxVal+pItemCfg->m_iAdjustVal;
			}
			if(lMaxVal<=0){
				pItemCfg = pItemCfg->m_poNext;
				continue;
			}
		 
			pResult->m_lValue = 0;
			ProdOfferAggrData aProdOfferAggrData;
			aProdOfferAggrData.m_lProdOfferID = 
							G_POFFERINSQO->m_poOfferIns->m_lProdOfferInsID;
			aProdOfferAggrData.m_iBillingCycleID = m_iBillingCycleID;
			aProdOfferAggrData.m_iPricingMeasure = iMeasureSeq;
			aProdOfferAggrData.m_iItemGroupID = pItemCfg->m_poObject->m_lValue;
			if (!pItemCfg->m_poObject->m_iExt)
			{
				switch (pItemCfg->m_poObject->m_iOwner)
				{
				case 2:
					aProdOfferAggrData.m_iInType = 2;
					aProdOfferAggrData.m_lInValue = lServID;
					break;

				case 12:
					aProdOfferAggrData.m_iInType = 12;
					aProdOfferAggrData.m_lInValue =
								G_POFFERINSQO->m_poOfferIns->m_iOfferID;
					break;

				case 15:
					aProdOfferAggrData.m_iInType = 15;
					aProdOfferAggrData.m_lInValue =
								G_POFFERINSQO->m_poDetailIns->m_lOfferDetailInsID;
					break;

				case 3:
					aProdOfferAggrData.m_iInType = 3;
						aProdOfferAggrData.m_lInValue = lServID;//��������serv_id����cust_id��û����
						break;
				}
			}
			else
			{
				aProdOfferAggrData.m_iInType = 16;
				aProdOfferAggrData.m_lInValue =pItemCfg->m_poObject->m_iOwner;
			}
			for (int m=0;m<m_iComNum;m++)
			{
				aProdOfferAggrData.m_iProdOfferCombine =m_ioCombineID[m];
#ifdef DEF_SICHUAN 
				if(m_poProdOfferDisctAggr->getAllDisctAggr(&aProdOfferAggrData,pResult))
				{
					if(lTempVal<pResult->m_lValue)
					{
						lTempVal = pResult->m_lValue;
					}
				}		

#else
				if(m_poProdOfferDisctAggr->getAllItemAggr(&aProdOfferAggrData,pResult))
				{
					if(lTempVal<pResult->m_lValue)
					{
						lTempVal = pResult->m_lValue;
					}
				}
#endif
			}

			pResult->m_lValue = lTempVal;
			if(m_poOfferNameIndex->get(pQO->m_poOfferIns->m_iOfferID,&sName)){
				strcpy(sOfferName,sName->m_sName);
				strcpy(sAccuName,sOfferName);///Ĭ���ۻ���������Ʒ��
			}
			if(pItemCfg->m_iOperFlag == OPER_FLAG_PARSER){
				parserObject(pItemCfg);
			}
			if (m_iMemFlag == 1)
			{
				strcpy(sAccuName,pItemCfg->m_sQueryDesc);
			}
			addResult(vResult,sOfferName,sAccuName,pCycle->getStartDate(),
				pCycle->getEndDate(),lMaxVal,pResult->m_lValue,pItemCfg);
            iRet++;	
			pItemCfg = pItemCfg->m_poNext;
        }
		delete pResult;
		pResult = NULL;
    }
    return iRet;
}

int FreeAccuQuery::queryOfferInstDB(vector<FreeAccuQueryResult> &vResult,
long lServID, OfferInsQO *pQO)
{
    int iRet = 0;
	long lRet =0;
	ID * pID;
	long lMaxVal =-1;
	//long lResult = 0;
	float fResult = 0;
	long lTemp = 0;
	int iMeasureSeq  = 0;
	ItemName * sName;
	char sOfferName[128];
	char sAccuName[128];
	vector<TableData> vet500110,vet500184,vet500;
	bool b10 = false,b84 = false;
	bool bVal = false;
	int iTableType,iTemp;
	char sSql[512];
	vector<long> lVec;

	TableData tabData;

	TOCIQuery qry(Environment::getDBConn());

	BillingCycle const * pCycle = 
        m_poBCM->getOccurBillingCycle (1,m_sDate);
    if (pCycle) {
		m_iBillingCycleID = pCycle->getBillingCycleID ();
	}else{
		Log::log(0,"����ʱ���ѯ���ڲ����ڣ�ʱ��Ϊ��%s",m_sDate);
		return -1;
	}
	memset(sSql,0,sizeof(sSql));
	sprintf(sSql,"select distinct table_type,nvl(billing_cycle_id,0) billing_cycle_id, "
				"nvl(org_id,-1) org_id,table_name "
				"from b_inst_table_list where table_type in (10,84) "
				" and (billing_cycle_id =%d or billing_cycle_id is null)"
				"order by table_type,billing_cycle_id,org_id desc",m_iBillingCycleID);

	qry.setSQL(sSql);
	qry.open();
	while(qry.next()){
		iTableType = qry.field(0).asInteger();
		if(iTableType == 10){
			b10 =  true;
			tabData.m_iBillingCycleID = qry.field(1).asInteger();
			tabData.m_iOrgID = qry.field(2).asInteger();
			strcpy(tabData.m_sTableName , qry.field(3).asString());
			vet500110.push_back(tabData);
		}else{
			b84 = true;
			tabData.m_iBillingCycleID = qry.field(1).asInteger();
			tabData.m_iOrgID = qry.field(2).asInteger();
			strcpy(tabData.m_sTableName , qry.field(3).asString());
			vet500184.push_back(tabData);
		}
	}
	qry.close();
	qry.commit();

	if(!b10 || !b84){
		Log::log(0,"ʹ�����ۻ���ѯ�����ñ�ȫ�������ã���ѯʱ��Ϊ��%s",m_sDate);
		return -1;
	}else{
		//1, ���ۼ�ʹ����---��Ҫ ��Ʒʵ��, �ۼ�������, ����ʵ��
		FreeAccuCfg * pAccuCfg;
		G_POFFERINSQO = pQO;
		if (m_poAccuIndex->get (pQO->m_poOfferIns->m_iOfferID, &pAccuCfg)) 
		{
			long lServ_ID = 0;
			long lMemberID = 0;
			lRet = 0;
			m_poTableName->clearAll();
			while (pAccuCfg)
			{
				if(pAccuCfg->m_iOperFlag == OPER_FLAG_CG && pAccuCfg->m_iAdjustFlag == 1)
				{
					iRet =  iRet + operProcess(vResult,lServID,pQO,pAccuCfg);
					pAccuCfg = pAccuCfg->m_poNext;
					continue;
				}
				memset(sOfferName,0,sizeof(sOfferName));
				memset(sAccuName,0,sizeof(sAccuName));
				lMaxVal = -1;
				bVal = false;
				int iCount = 0;				
				if (OPER_FLAG_SECTIONPARSER == pAccuCfg->m_iOperFlag)
				{
					m_poResultBuf[0].m_iCombineID = 0;
					m_poResultBuf[0].m_lValue = 0;
					m_poResultBuf[0].m_iMeasureSeq = 0;
					pEvent->m_iServID = lServID;
					strcpy(pEvent->m_sStartDate,m_sDate);
					if (m_poPricingInfoMgr->getSectionValue(pAccuCfg->m_poObject,m_poResultBuf[0].m_iCombineID,
						m_poResultBuf[0].m_lValue,m_poResultBuf[0].m_iMeasureSeq,pEvent)){
						iCount = 1;
					}
				} else if (OPER_FLAG_SECTIONPARSER_IN == pAccuCfg->m_iOperFlag)
				{
					pEvent->m_iServID = lServID;
					strcpy(pEvent->m_sStartDate,m_sDate);
					lVec.clear();
					iCount = m_poPricingInfoMgr->getSectionValue(pAccuCfg->m_poObject,lVec,pEvent);
					sort(lVec.begin(),lVec.end());
				}else
				{
					iCount = getConditionVal(pAccuCfg,m_poResultBuf);
				}
				if(pAccuCfg->m_iOperFlag != OPER_FLAG_SECTIONPARSER_IN)
				{
					for (int m=0;m<iCount;m++)
					{
						if(lMaxVal<m_poResultBuf[m].m_lValue)
						{
							lMaxVal=m_poResultBuf[m].m_lValue;
						}
					}
				}
				if(pAccuCfg->m_iAdjustFlag == 1)
				{
					if(lMaxVal == -1){
						lMaxVal = 0;
					}
					lMaxVal = lMaxVal+pAccuCfg->m_iAdjustVal;
				}
				if(lMaxVal<=0 && pAccuCfg->m_iOperFlag != OPER_FLAG_SECTIONPARSER_IN){
					pAccuCfg = pAccuCfg->m_poNext;
					continue;
				}

				AccuType *pAccu = m_pCfg->getAccuType (pAccuCfg->m_poObject->m_lValue);
				if (!pAccu) {
					pAccuCfg = pAccuCfg->m_poNext;
					continue;
				}

				char sBeginTime[15];
				char sEndTime[15];
				int  iReturnFlag;

				iReturnFlag = m_pCfg->getCycleTime(G_POFFERINSQO,m_iBillingCycleID,pAccu->m_iCycleTypeID,
					m_sDate,sBeginTime,sEndTime);
				if (iReturnFlag == 0){
					pAccuCfg = pAccuCfg->m_poNext;
					continue;
				}
				int iResult = 0;
				vector<AccuData > vAccuData;
				vAccuData.clear();
				for(int i=0;i<vet500184.size();i++)
				{
					if(bVal) break;
					tabData = (TableData)vet500184[i];
					if (m_poTableName->get(tabData.m_sTableName,&iTemp)){
						continue;
					}else{
						m_poTableName->add(tabData.m_sTableName,1);
					}
					if(tabData.m_iBillingCycleID>0 && tabData.m_iBillingCycleID!=m_iBillingCycleID) continue;
					if(tabData.m_iOrgID>-1 ){
						if (!G_PORGMGR->getBelong(G_PSERV->getOrgID(),tabData.m_iOrgID)) continue;
					}
					memset(sSql,0,sizeof(sSql));
					//��Ҫ����ʼʱ�䡢����ʱ�䡢�������͵�����
					sprintf(sSql,"select nvl(max(cycle_inst_id),0) cycle_inst_id "
						"from %s where cycle_begin_time=to_date(%s,'yyyymmddhh24miss')  "
						"and cycle_end_time=to_date(%s,'yyyymmddhh24miss')"
						"and cycle_type_id=%d ",tabData.m_sTableName,sBeginTime,sEndTime,pAccu->m_iCycleTypeID);
					qry.setSQL(sSql);
					qry.open();
					if(qry.next())
					{
						long lCycleInstId = qry.field(0).asLong();
						qry.close();
						qry.commit();
						if(lCycleInstId == 0){
							continue;
						}
						//��ѯ�ۻ���
						for(int j=0;j<vet500110.size();j++)
						{
							tabData = (TableData)vet500110[j];
							if (m_poTableName->get(tabData.m_sTableName,&iTemp)){
								continue;
							}else{
								m_poTableName->add(tabData.m_sTableName,1);
							}
							if(tabData.m_iBillingCycleID>0 && tabData.m_iBillingCycleID!=m_iBillingCycleID) continue;
							if(tabData.m_iOrgID>0 ){
								if (!G_PORGMGR->getBelong(G_PSERV->getOrgID(),tabData.m_iOrgID)) continue;
							}
							lServ_ID = 0;
							lMemberID = 0;					    
							switch (pAccu->m_sAccuLevel[2]) {
							case '3':
								lServ_ID = lServID;
								break;
							case '2':
								lMemberID = G_POFFERINSQO->m_poDetailIns->m_lOfferDetailInsID;
								break;
							default:
								break;
							}
							memset(sSql,0,sizeof(sSql));
							//��Ҫ����Ʒʵ��ID������������ж�����¼���������󣩰�ʱ��ȡ���һ�����ڴ��߼�����һ��
#ifdef SPLIT_ACCU_BY_BILLING_CYCLE
							sprintf(sSql,"select value,split_by from %s where offer_inst_id = %ld and cycle_inst_id = %ld "
								" and acc_type_id = %d and serv_id=%ld and inst_member_id = %ld "
								" and billing_cycle_id = %d order by state_date desc",tabData.m_sTableName,
								G_POFFERINSQO->m_poOfferIns->m_lProdOfferInsID,lCycleInstId,
								pAccu->m_iAccuTypeID,lServ_ID,lMemberID,m_iBillingCycleID);
#else
							sprintf(sSql,"select value,split_by from %s where offer_inst_id = %ld and cycle_inst_id = %ld "
								" and acc_type_id = %d and serv_id=%ld and inst_member_id = %ld "
								" order by state_date desc",tabData.m_sTableName,
								G_POFFERINSQO->m_poOfferIns->m_lProdOfferInsID,lCycleInstId,
								pAccu->m_iAccuTypeID,lServ_ID,lMemberID);
#endif
							qry.setSQL(sSql);
							qry.open();
							AccuData oData;							
							while(qry.next()){
								memset(&oData,0,sizeof(oData));
								oData.m_lValue = qry.field(0).asLong();
								strncpy(oData.m_sSplitby,
									qry.field(1).asString(),sizeof(oData.m_sSplitby));
								vAccuData.push_back(oData);
								//bVal = true;
								iResult++;
							}
							qry.close();
							qry.commit();
						}
					}else{
						qry.close();
						qry.commit();
					}
				}
				/*if(!bVal){//û�����ݿ��¼Ҳ��ʾ
					pAccuCfg = pAccuCfg->m_poNext;
					continue;
				}*/
				if(m_poOfferNameIndex->get(pQO->m_poOfferIns->m_iOfferID,&sName)){
					strcpy(sOfferName,sName->m_sName);
				}
				int iCalRuleID = 0;
				int iRemindTypeID = 0;
				if(m_poAccuNameIndex->get(pAccu->m_iAccuTypeID,&sName)){
					strcpy(sAccuName,sName->m_sName);
					iCalRuleID = sName->m_iCalRuleId;
					iRemindTypeID = sName->m_RemindTypeID;
				}
				if(pAccuCfg->m_iOperFlag == OPER_FLAG_PARSER){
					parserObject(pAccuCfg);
				}
				//��Ҫ���������
				if (0 == iResult){
					if (pAccuCfg->m_iOperFlag == OPER_FLAG_SECTIONPARSER_IN)
					{
						if (!iCount){
							pAccuCfg = pAccuCfg->m_poNext;
							continue;
						}
					  for(int j = 0; j < iCount; ++j)
					  {
						  if( lVec[j] )
						  {
							  lMaxVal = lVec[j];
						  	break;
						  }
					  }
					}
					addResult(vResult,sOfferName,sAccuName,sBeginTime,
						sEndTime,lMaxVal,0,pAccuCfg,iCalRuleID,iRemindTypeID);
					iRet++;
				}else
				{
					bool bMax = false;
					if(pAccuCfg->m_iOperFlag == OPER_FLAG_SECTIONPARSER_IN){
						if (!iCount){
							pAccuCfg = pAccuCfg->m_poNext;
							continue;
						}
						bMax = true;
					}
					for (int i=0;i<iResult;i++)
					{
						if(bMax)
						{
							lMaxVal = lVec[lVec.size()-1];
							for(int j = 0; j < iCount; ++j)
							{
								if(lVec[j] >= vAccuData[i].m_lValue)
								{
									lMaxVal = lVec[j];
									break;
								}
							}
						}
						addResult(vResult,sOfferName,sAccuName,sBeginTime,sEndTime,lMaxVal,
							vAccuData[i].m_lValue,pAccuCfg,iCalRuleID,iRemindTypeID,
							vAccuData[i].m_sSplitby);
						iRet++;
					}
				}
				pAccuCfg = pAccuCfg->m_poNext;
			}
		}
	}

	memset(sSql,0,sizeof(sSql));
    //2, ���ʵ��Ż�---��Ҫ֪��combine_id, pricing_measure, ����
	sprintf(sSql,"select nvl(org_id,-1) org_id,table_name "
				"from b_inst_table_list where table_type =87 "
				"and billing_cycle_id=%d order by org_id desc",m_iBillingCycleID);
	qry.setSQL(sSql);
	qry.open();
	while(qry.next()){
		tabData.m_iOrgID = qry.field(0).asInteger();
		strcpy(tabData.m_sTableName ,qry.field(1).asString());
		vet500.push_back(tabData);
	}
	qry.close();
	qry.commit();

	if(!vet500.size()){
		Log::log(0,"�����ۻ���ѯ�����ñ��Ż�״̬����ȫ�������ã���ѯʱ��Ϊ��%s",m_sDate);
		return -1;
	}else{
		FreeAccuCfg * pItemCfg;
		iMeasureSeq  = 0;
		bVal = false;
		lRet = 0;
		long lTempRet = 0;
		if (m_poItemIndex->get (pQO->m_poOfferIns->m_iOfferID, &pItemCfg)) {
			m_poTableName->clearAll();
			while (pItemCfg)
			{
				memset(sOfferName,0,sizeof(sOfferName));
				memset(sAccuName,0,sizeof(sAccuName));
				lMaxVal = -1;
				m_iComNum = 0;
				lTempRet = 0;
				bVal = false;
				int iCount = 0;
				if (OPER_FLAG_SECTIONPARSER == pItemCfg->m_iOperFlag)
				{
					m_poResultBuf[0].m_iCombineID = 0;
					m_poResultBuf[0].m_lValue = 0;
					m_poResultBuf[0].m_iMeasureSeq = 0;
					pEvent->m_iServID = lServID;
					strcpy(pEvent->m_sStartDate,m_sDate);
					if (m_poPricingInfoMgr->getSectionValue(pItemCfg->m_poObject,m_poResultBuf[0].m_iCombineID,
						m_poResultBuf[0].m_lValue,m_poResultBuf[0].m_iMeasureSeq,pEvent)){
						iCount = 1;
					}
				}else
				{
					iCount = getConditionVal(pItemCfg,m_poResultBuf);
				}
				for (int m=0;m<iCount;m++)
				{
					if(lMaxVal<=m_poResultBuf[m].m_lValue)
					{
						lMaxVal=m_poResultBuf[m].m_lValue;
						iMeasureSeq = m_poResultBuf[m].m_iMeasureSeq;
						m_ioCombineID[m_iComNum] = m_poResultBuf[m].m_iCombineID;
						m_iComNum ++;
					}
				}				
				if(pItemCfg->m_iAdjustFlag == 1)
				{
					if(lMaxVal == -1){
						lMaxVal = 0;
					}
					lMaxVal = lMaxVal+pItemCfg->m_iAdjustVal;
				}
				if(lMaxVal<=0){
					pItemCfg = pItemCfg->m_poNext;
					continue;
				}
				for(int i=0;i<vet500.size();i++){
					tabData = (TableData)vet500[i];
					if (m_poTableName->get(tabData.m_sTableName,&iTemp)){
						continue;
					}else{
						m_poTableName->add(tabData.m_sTableName,1);
					}
					if(tabData.m_iOrgID>-1 ){
						if (!G_PORGMGR->getBelong(G_PSERV->getOrgID(),tabData.m_iOrgID)) continue;
					}
					int iInType = 0;
					long lInValue  = 0;
					
					if (!pItemCfg->m_poObject->m_iExt) {
						switch (pItemCfg->m_poObject->m_iOwner) {
						case 2:
							iInType = 2;
							lInValue = lServID;
							break;
						case 12:
							iInType = 12;
							lInValue = G_POFFERINSQO->m_poOfferIns->m_iOfferID;
							break;
						case 15:
							iInType = 15;
							lInValue = G_POFFERINSQO->m_poDetailIns->m_lOfferDetailInsID;
							break;
						case 3:
							iInType = 3;
							lInValue = lServID;//��������serv_id����cust_id��û����
							break;
						}
					}else{
						iInType = 16;
						lInValue =pItemCfg->m_poObject->m_iOwner;
					}
					for (int m=0;m<m_iComNum;m++)
					{
						memset(sSql,0,sizeof(sSql));
						//��Ҫ����Ʒʵ��ID������
#ifdef DEF_SICHUAN          //�Ĵ���Ҫ���Ż�ֵ��ѯ
						sprintf(sSql,"select abs(disct_value)  "
							"from %s where offer_inst_id=%ld and billing_cycle_id=%d and pricing_combine_id=%d "
							"and measure_seq = %d and item_group_id = %ld and owner_type=%d and owner_id=%ld ",
							tabData.m_sTableName,G_POFFERINSQO->m_poOfferIns->m_lProdOfferInsID,
							m_iBillingCycleID,m_ioCombineID[m],
							iMeasureSeq,pItemCfg->m_poObject->m_lValue,iInType,lInValue);
#else
						sprintf(sSql,"select value "
							"from %s where offer_inst_id=%ld and billing_cycle_id=%d and pricing_combine_id=%d "
							"and measure_seq = %d and item_group_id = %ld and owner_type=%d and owner_id=%ld ",
							tabData.m_sTableName,G_POFFERINSQO->m_poOfferIns->m_lProdOfferInsID,
							m_iBillingCycleID,m_ioCombineID[m],
							iMeasureSeq,pItemCfg->m_poObject->m_lValue,iInType,lInValue);
#endif
						qry.setSQL(sSql);
						qry.open();
						if(qry.next()){
							lRet = qry.field(0).asLong();
							if(lTempRet<lRet)
							{
								lTempRet = lRet;
							}
							bVal = true;
							qry.close();
							qry.commit();
							break;
						}
						qry.close();
						qry.commit();
					}
				}
				/*if(!bVal){//û�����ݿ��¼Ҳ��ʾ
					pItemCfg = pItemCfg->m_poNext;
					continue;
				}*/
				lRet = lTempRet;
				if(m_poOfferNameIndex->get(pQO->m_poOfferIns->m_iOfferID,&sName)){
					strcpy(sOfferName,sName->m_sName);
					strcpy(sAccuName,sOfferName);//Ĭ���ۻ���������Ʒ��
				}
				if(pItemCfg->m_iOperFlag == OPER_FLAG_PARSER){
					parserObject(pItemCfg);
				}
				if (m_iMemFlag == 1)
				{
					strcpy(sAccuName,pItemCfg->m_sQueryDesc);
				}
				addResult(vResult,sOfferName,sAccuName,pCycle->getStartDate(),
					pCycle->getEndDate(),lMaxVal,lRet,pItemCfg);
				iRet++;					
				pItemCfg = pItemCfg->m_poNext;
			}
		}
	}
    return iRet;
}

FreeAccuQuery::FreeAccuQuery()
{
    Environment::useUserInfo(USER_INFO_BASE);

	if (!m_bUploaded){		
		loadInit();
		loadCfg();
		m_bUploaded = true;
	}
	m_iComNum = 0;
	m_iBillingCycleID = 0;
	memset(m_sDate,0,sizeof(m_sDate));
	memset(m_ioCombineID,0,sizeof(m_ioCombineID));
}

FreeAccuQuery::~FreeAccuQuery()
{
}

void FreeAccuQuery::loadCfg()
{
	int iCount = 0;
	int iParserNum = 0;
	int m,n;
	char sSqlcode[1024];
	ID * pID = 0;
	FreeAccuCfg * pAccuTemp = 0;
	FreeAccuCfg * pAcctItemTemp = 0;
	FreeAccuCfg * pItemCfg = 0;
	FreeAccuCfg * pAccuCfg = 0;
	TOCIQuery qry(Environment::getDBConn());

	memset(sSqlcode,0,sizeof(sSqlcode));
	sprintf (sSqlcode, "select count(1) "
			"    from b_offer_query_free_accu");
	qry.setSQL(sSqlcode);
	qry.open();
	if(qry.next()){
		iCount = qry.field(0).asInteger();
	}
	qry.close();
	qry.commit();
	iCount = iCount+2;
	m_iCount = iCount;

	m_poAccuCfgList = new FreeAccuCfg[iCount];
	memset(m_poAccuCfgList,0,(sizeof(FreeAccuCfg))*iCount);
	if(!m_poAccuCfgList)
	{
		Log::log(0,"FreeAccuQuery::loadCfg�� new FreeAccuCfg[iCount] �����ڴ�ռ�ʧ�ܣ�");
		THROW(1);
	}
	m_poAccuIndex = new HashList<FreeAccuCfg *>(iCount);
	if(!m_poAccuIndex)
	{
		Log::log(0,"FreeAccuQuery::loadCfg�� new HashList<FreeAccuCfg *> �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}
    m_poItemIndex = new HashList<FreeAccuCfg *>(iCount>>1);
	if(!m_poItemIndex)
	{
		Log::log(0,"FreeAccuQuery::loadCfg�� new HashList<FreeAccuCfg *> �����ڴ�ռ�ʧ��2��");
		THROW(1);
	}

    memset(sSqlcode,0,sizeof(sSqlcode));
    sprintf (sSqlcode, "select query_id,offer_id,query_object_id,"
             " nvl(unit_convert_nume,0),nvl(unit_convert_deno,0),target_unit_name,nvl(query_flag,0) query_flag"
             " ,nvl(adjust_value,0) adjust_value,nvl(adjust_flag,0) adjust_flag,query_desc,nvl(oper_flag,0), "
             " extend_item,NVL(merge_flag, 1) "

             "    from b_offer_query_free_accu"
             "  order by offer_id");

	qry.setSQL (sSqlcode);
	qry.open ();
	m = 0;
	while (qry.next ()) {
		pID = new ID(qry.field(2).asString());
		if(!pID){
			Log::log(0,"FreeAccuQuery::loadCfg��new ID() �����ڴ�ռ�ʧ�ܣ�");
			THROW(1); 
		}
		switch(pID->m_iAttrID)
		{
		case 500://����
			pItemCfg = m_poAccuCfgList+m;
			if(!pItemCfg){
				Log::log(0,"FreeAccuQuery::loadCfg��new FreeAccuCfg(500) �����ڴ�ռ�ʧ�ܣ�");
				return;
			}
			pItemCfg->m_iQueryID = qry.field(0).asInteger();
			if(m_poItemIndex->get(qry.field(1).asInteger(),&pAcctItemTemp)){
				for(;pAcctItemTemp->m_poNext;pAcctItemTemp=pAcctItemTemp->m_poNext);
				pAcctItemTemp->m_poNext=pItemCfg;
			}else{
				m_poItemIndex->add(qry.field(1).asInteger(),pItemCfg);
			}
			pItemCfg->m_iOfferID = qry.field(1).asInteger();
			pItemCfg->m_poNext=NULL;
			pItemCfg->m_iUnitNume=qry.field(3).asInteger();
			pItemCfg->m_iUnitDeno=qry.field(4).asInteger();
			if(pItemCfg->m_iUnitDeno==0){
				pItemCfg->m_iUnitDeno = 1;
				pItemCfg->m_iUnitNume = 0;
				Log::log(0,"����ķ�ĸΪ�㣬query_id=",qry.field(0).asInteger());
			}
			strcpy(pItemCfg->m_sUnitName,qry.field(5).asString());
			pItemCfg->m_iQueryFlag = qry.field(6).asInteger();
			pItemCfg->m_iAdjustVal = qry.field(7).asInteger();
			pItemCfg->m_iAdjustFlag = qry.field(8).asInteger();
			strcpy(pItemCfg->m_sQueryDesc,qry.field(9).asString());
			pItemCfg->m_iOperFlag = qry.field(10).asInteger();
			strcpy(pItemCfg->m_sExtendItem, qry.field(11).asString());      ///
			pItemCfg->m_iMergeFlag = qry.field(12).asInteger();
			if (OPER_FLAG_SECTIONPARSER == pItemCfg->m_iOperFlag){
				iParserNum ++;
				m_poOfferSectionParser->add(pItemCfg->m_iOfferID,iCount);
			}
			pItemCfg->m_poObject = new ID(qry.field(2).asString());
			break;

		case 5001://ʹ����
			pAccuCfg = m_poAccuCfgList+m;
			if(!pAccuCfg){
				Log::log(0,"FreeAccuQuery::loadCfg��new FreeAccuCfg(5001) �����ڴ�ռ�ʧ�ܣ�");
				return;
			}
			if(m_poAccuIndex->get(qry.field(1).asInteger(),&pAccuTemp)){
				for(;pAccuTemp->m_poNext;pAccuTemp=pAccuTemp->m_poNext);
				pAccuTemp->m_poNext=pAccuCfg;
			}else{
				m_poAccuIndex->add(qry.field(1).asInteger(),pAccuCfg);
			}
			pAccuCfg->m_iOfferID = qry.field(1).asInteger();
			pAccuCfg->m_poNext=NULL;
			pAccuCfg->m_iQueryID = qry.field(0).asInteger();
			pAccuCfg->m_iUnitNume=qry.field(3).asInteger();
			pAccuCfg->m_iUnitDeno=qry.field(4).asInteger();
			if(pAccuCfg->m_iUnitDeno==0){
				pAccuCfg->m_iUnitDeno = 1;
				pAccuCfg->m_iUnitNume = 0;
				Log::log(0,"����ķ�ĸΪ�㣬query_id=",qry.field(0).asInteger());
			}
			strcpy(pAccuCfg->m_sUnitName,qry.field(5).asString());
			pAccuCfg->m_iQueryFlag = qry.field(6).asInteger();
			pAccuCfg->m_iAdjustVal = qry.field(7).asInteger();
			pAccuCfg->m_iAdjustFlag = qry.field(8).asInteger();
			strcpy(pAccuCfg->m_sQueryDesc,qry.field(9).asString());
			pAccuCfg->m_iOperFlag = qry.field(10).asInteger();
			strcpy(pAccuCfg->m_sExtendItem, qry.field(11).asString());  ///
      pAccuCfg->m_iMergeFlag = qry.field(12).asInteger();
			if (OPER_FLAG_SECTIONPARSER == pAccuCfg->m_iOperFlag ||
				OPER_FLAG_SECTIONPARSER_IN == pAccuCfg->m_iOperFlag){
				iParserNum ++;
				m_poOfferSectionParser->add(pAccuCfg->m_iOfferID,iCount);
			}
			pAccuCfg->m_poObject = new ID(qry.field(2).asString());
			//pAccuCfg->m_iAccuTypeID = pID->m_lValue;			
			break;

		default:
			Log::log(0,"δ֪���͵Ĺ���querty_id=%d",qry.field(0).asInteger());
			break;
		}
		delete pID;
		pID =  NULL;
		m++;
		if (m > iCount){
			Log::log(0,"FreeAccuQuery::loadCfg�� �ڼ���b_offer_query_free_accu�����ݹ��������ݷ��ɸı䣬������");
			THROW(1);
		}
	}
	qry.close ();
	qry.commit();

	if (iParserNum > 0)
	{
		m_poConditionMgr = new BillConditionMgr (10);//����ֱ����10�����ٴ���������ȡ��
		if (NULL == m_poConditionMgr)
		{
			Log::log(0,"FreeAccuQuery::loadCfg�� new BillConditionMgr(10) �����ڴ�ռ�ʧ��1��");
			THROW(1);
		}
		if(!G_PACCTITEMMGR) G_PACCTITEMMGR = new AcctItemMgr();
		if (!G_PACCTITEMMGR){
			Log::log(0,"FreeAccuQuery::loadCfg�� new AcctItemMgr() �����ڴ�ռ�ʧ��1��");
			THROW(1);
		}
		if(!G_PTRANSMGR) G_PTRANSMGR = new TransactionMgr();
		if (!G_PTRANSMGR){
			Log::log(0,"FreeAccuQuery::loadCfg�� new TransactionMgr() �����ڴ�ռ�ʧ��1��");
			THROW(1);
		}
		m_poPricingInfoMgr = new PricingInfoMgr (m_poConditionMgr);
		if (NULL == m_poPricingInfoMgr)
		{
			Log::log(0,"FreeAccuQuery::loadCfg�� new PricingInfoMgr(m_poConditionMgr) �����ڴ�ռ�ʧ��1��");
			THROW(1);
		}
		if(!G_POFFERMGR)
		{
			G_POFFERMGR = new OfferMgr();
			if (!G_POFFERMGR){
				Log::log(0,"FreeAccuQuery::loadCfg�� new OfferMgr() �����ڴ�ռ�ʧ��1��");
				THROW(1);
			}
			G_POFFERMGR->loadNumberAttr();
			G_POFFERMGR->loadSpecialAttr();
			G_POFFERMGR->load();
		}

		pEvent = new EventSection();
		if (NULL == pEvent)
		{
			Log::log(0,"FreeAccuQuery::loadCfg�� new EventSection() �����ڴ�ռ�ʧ��1��");
			THROW(1);
		}
		memset(pEvent,0,sizeof(EventSection));
//�����ض���ʼ��
#ifdef HAINAN_APPORTON
		pEvent->m_lDuration = 100;
		pEvent->m_lSumAmount = 1048576+1;
#endif
	}

	memset(sSqlcode,0,sizeof(sSqlcode));
	sprintf (sSqlcode, "select count(1)"
			"    from product_offer");
	qry.setSQL (sSqlcode);
	qry.open ();
	if(qry.next ()) {
		iCount = qry.field(0).asInteger();
	}
	qry.close ();
	qry.commit();
	iCount = iCount+2;

	ItemName * pTemp = 0;
	int i = 0;
	m_poOfferNameList = new ItemName[iCount];
	memset(m_poOfferNameList,0,(sizeof(ItemName))*iCount);
	if(!m_poOfferNameList)
	{
		Log::log(0,"FreeAccuQuery::loadCfg�� new ItemName[iCount] �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}
	m_poOfferNameIndex = new HashList<ItemName *>(iCount>>1);
	if(!m_poOfferNameIndex)
	{
		Log::log(0,"FreeAccuQuery::loadCfg�� new HashList<ItemName *> �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}

	memset(sSqlcode,0,sizeof(sSqlcode));
	sprintf (sSqlcode, "select offer_id,offer_name,nvl(offer_query_name,'no') offer_query_name"
			"    from product_offer");
	qry.setSQL (sSqlcode);
	qry.open ();
	while (qry.next ())
	{
		pTemp = m_poOfferNameList+i;
		if (strcmp(qry.field(2).asString(),"no") == 0)
		{
			strncpy(pTemp->m_sName,qry.field(1).asString(),sizeof(pTemp->m_sName)-1);
		}
		else
		{
			strncpy(pTemp->m_sName,qry.field(2).asString(),sizeof(pTemp->m_sName)-1);
		}
		m_poOfferNameIndex->add(qry.field(0).asInteger(),pTemp);
		i++;
	}
	qry.close ();
	qry.commit();

	memset(sSqlcode,0,sizeof(sSqlcode));
	sprintf (sSqlcode, "select count(1)"
			"    from b_accu_type");
	qry.setSQL (sSqlcode);
	qry.open ();
	if(qry.next ()) {
		iCount = qry.field(0).asInteger();
	}
	qry.close ();
	qry.commit();
	iCount = iCount+1;

	i = 0;
	m_poAccuNameList =  new ItemName[iCount];
	memset(m_poAccuNameList,0,(sizeof(ItemName))*iCount);
	if(!m_poAccuNameList)
	{
		Log::log(0,"FreeAccuQuery::loadCfg�� new ItemName[iCount] �����ڴ�ռ�ʧ��2��");
		THROW(1);
	}
	m_poAccuNameIndex = new HashList<ItemName *>(iCount);
	if(!m_poAccuNameIndex)
	{
		Log::log(0,"FreeAccuQuery::loadCfg�� new HashList<ItemName *> �����ڴ�ռ�ʧ��2��");
		THROW(1);
	}
	
	memset(sSqlcode,0,sizeof(sSqlcode));
	sprintf (sSqlcode, "select nvl(accu_type_id,0) accu_type_id,accu_type_name,"
		"nvl(cal_rule_id,0) cal_rule_id,nvl(remind_type,0) remind_type from b_accu_type");
	qry.setSQL (sSqlcode);
	qry.open ();
	while (qry.next ()) {
		pTemp = m_poAccuNameList+i;
		strncpy(pTemp->m_sName,qry.field(1).asString(),sizeof(pTemp->m_sName)-1);
		pTemp->m_iCalRuleId = qry.field(2).asInteger();
        pTemp->m_RemindTypeID = qry.field(3).asInteger();
		m_poAccuNameIndex->add(qry.field(0).asInteger(),pTemp);
		i++;
	}
	qry.close ();
	qry.commit();

	LifeCycleMgr lifeCycleMgr;
	vector<ConditionValTemp> vTemp;
	ConditionValTemp oTemp;
	iCount = 0;

	memset(sSqlcode,0,sizeof(sSqlcode));
#ifdef DEF_SICHUAN           //���Ĵ����ҵ�񵥶�����
	sprintf (sSqlcode,
		"SELECT distinct  a.offer_id,pc.pricing_combine_id,"
		"	    nvl(pc.life_cycle_id,0) life_cycle_id,"
		"	    nvl(pc.pricing_objects_id,0) pricing_objects_id,"
		"	    ls.param1_id,ls.condition_val_id,nvl(bm.measure_seq,0) measure_seq "
		"  FROM b_offer_query_free_accu a,"
		"	    product_offer b,"
		"	    pricing_combine pc,"
		"	    pricing_section ps,"
		"	    logic_statement ls,"
		"	    b_strategy_measure bm "
		" WHERE a.offer_id=b.offer_id "
		"   AND b.pricing_plan_id=pc.pricing_plan_id "
		"   AND pc.event_pricing_strategy_id=ps.event_pricing_strategy_id "
		"   AND ps.condition_id=ls.condition_id "
		"   AND pc.event_pricing_strategy_id=bm.strategy_id"
		" union all "
		" select distinct a.offer_id, "
		" pc.pricing_combine_id, "
		"  nvl(pc.life_cycle_id, 0) life_cycle_id, "
		"  nvl(pc.pricing_objects_id, 0) pricing_objects_id,"
		"  ast.param1_id, "
		"  ast.param2_id condition_val_id, "
		"  nvl(bm.measure_seq, 0) measure_seq "
		" from b_offer_query_free_accu a,  "
		" product_offer           b, "
		" assign_statement        ast, "
		" pricing_section         ps, "
		" pricing_combine         pc, "
		" b_strategy_measure      bm, "
		" discount_desc           dd "
		" where a.query_flag = 99 "
		" and a.offer_id = b.offer_id "
		" and b.pricing_plan_id = pc.pricing_plan_id "
		" and pc.event_pricing_strategy_id = ps.event_pricing_strategy_id "
		" and ps.pricing_section_id = dd.pricing_section_id "
		" and dd.operation_id = ast.operation_id "
		" and pc.event_pricing_strategy_id = bm.strategy_id "
		" and a.query_object_id = ast.param1_id");
#else
	sprintf (sSqlcode,
		"SELECT distinct  a.offer_id,pc.pricing_combine_id,"
		"     nvl(pc.life_cycle_id,0) life_cycle_id,"
		"           nvl(pc.pricing_objects_id,0) pricing_objects_id,"
		"           ls.param1_id,ls.condition_val_id,nvl(bm.measure_seq,0) measure_seq "
		"  FROM b_offer_query_free_accu a,"
		"           product_offer b,"
		"           pricing_combine pc,"
		"           pricing_section ps,"
		"           logic_statement ls,"
		"           b_strategy_measure bm "
		" WHERE a.offer_id=b.offer_id "
		"   AND b.pricing_plan_id=pc.pricing_plan_id "
		"   AND pc.event_pricing_strategy_id=ps.event_pricing_strategy_id "
		"   AND ps.condition_id=ls.condition_id "
		"   AND pc.event_pricing_strategy_id=bm.strategy_id");
#endif
	qry.setSQL (sSqlcode);
	qry.open ();
	while(qry.next())
	{
		memset(&oTemp,0,sizeof(oTemp));
		oTemp.m_iOfferID = qry.field(0).asInteger();
		oTemp.m_iCombineID = qry.field(1).asInteger();
		oTemp.m_iLifeCycleID = qry.field(2).asInteger();
		oTemp.m_iObjectID = qry.field(3).asInteger();
		strncpy(oTemp.m_sParamID1,qry.field(4).asString(),sizeof(oTemp.m_sParamID1)-1);
		strncpy(oTemp.m_sDesParamID,qry.field(5).asString(),sizeof(oTemp.m_sParamID1)-1);
		oTemp.m_iMeasureSeq = qry.field(6).asInteger();
		vTemp.push_back(oTemp);
		iCount ++;
	}

	if(iCount <2)
	{
		iCount = iCount+2;
	}
	m_iCountVal = iCount;
	m_pConditionValList = new ConditionVal[iCount];
	memset(m_pConditionValList,0,(sizeof(ConditionVal))*iCount);
	if(!m_pConditionValList)
	{
		Log::log(0,"FreeAccuQuery::loadCfg��new ConditionVal ���������ڴ�ռ�ʧ�ܣ�");
		THROW(1);
	}
	m_pConditionVal = new HashList<ConditionVal *> (iCount>>1);
	if(!m_pConditionVal)
	{
		Log::log(0,"FreeAccuQuery::loadCfg��new HashList<ConditionVal *> �����ڴ�ռ�ʧ�ܣ�");
		THROW(1);
	}
	vector<ConditionValTemp>::iterator iter;
	ConditionVal * p = NULL;
	ConditionVal * pTempVal = NULL;
	iter = vTemp.begin();
	iCount = 0;
	for(;iter!=vTemp.end();iter++,iCount++)
	{
		p = m_pConditionValList+iCount;
		
		p->m_iOfferID = iter->m_iOfferID;
		p->m_iCombineID = iter->m_iCombineID;
		p->m_iObjectID = iter->m_iObjectID;
		p->m_poParamID1 = new ID(iter->m_sParamID1);
		if(!(p->m_poParamID1))
		{
			Log::log(0,"FreeAccuQuery::loadCfg��new ID(iter->m_sParamID1) �����ڴ�ռ�ʧ�ܣ�");
			THROW(1);
		}
		p->m_poDesParamID = new ID(iter->m_sDesParamID);
		if(!(p->m_poDesParamID))
		{
			Log::log(0,"FreeAccuQuery::loadCfg��new ID(iter->m_sDesParamID) �����ڴ�ռ�ʧ�ܣ�");
			THROW(1);
		}
		p->m_poLifeCycle = lifeCycleMgr.getLifeCycle(iter->m_iLifeCycleID);
		p->m_iMeasureSeq = iter->m_iMeasureSeq;
		p->m_poNext = 0;
		if(m_pConditionVal->get(p->m_iOfferID,&(pTempVal)))
		{
			p->m_poNext = pTempVal;
		}
		m_pConditionVal->add (p->m_iOfferID, p);
	}
	vTemp.clear();
	qry.close ();
	qry.commit();
	
	memset(sSqlcode,0,sizeof(sSqlcode));
	sprintf (sSqlcode, "select count(1)"
			"    from b_offer_query_accu_ex");
	qry.setSQL (sSqlcode);
	qry.open ();
	if(qry.next ()) {
		iCount = qry.field(0).asInteger();
	}
	qry.close ();
	qry.commit();
	iCount = iCount+2;
	
	m_poAttrExlist = new FreeOfferAttrEx[iCount];
	memset(m_poAttrExlist,0,(sizeof(FreeOfferAttrEx))*iCount);
	if(!m_poAttrExlist)
	{
		Log::log(0,"FreeAccuQuery::loadCfg�� new OfferAttrEx[iCount] �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}
	
	m_poAttrExIndex = new HashList<FreeOfferAttrEx *>(iCount>>1);
	if(!m_poOfferNameIndex)
	{
		Log::log(0,"FreeAccuQuery::loadCfg�� new HashList<FreeOfferAttrEx *> �����ڴ�ռ�ʧ��1��");
		THROW(1);
	}
	
	FreeOfferAttrEx *pTempAttrEx;
	FreeOfferAttrEx *pAttrEx;
	i=0;
	memset(sSqlcode,0,sizeof(sSqlcode));
    sprintf (sSqlcode, "select query_id,offer_id,attr_id,attr_value,accu_value,value_type"
			"    from b_offer_query_accu_ex");
	qry.setSQL (sSqlcode);
	qry.open ();
	while (qry.next ()) {
		pAttrEx = m_poAttrExlist+i;
    if(m_poAttrExIndex->get(qry.field(0).asInteger(),&pTempAttrEx)){
				for(;pTempAttrEx->m_poNext;pTempAttrEx=pTempAttrEx->m_poNext);
				pTempAttrEx->m_poNext=pAttrEx;
			}else{
				m_poAttrExIndex->add(qry.field(0).asInteger(),pAttrEx);
			}
	  pAttrEx->m_iQueryID=qry.field(0).asInteger();
		pAttrEx->m_iOfferID=qry.field(1).asInteger();
		pAttrEx->m_iAttrID=qry.field(2).asInteger();
		strcpy(pAttrEx->m_sAttrValue,qry.field(3).asString());
		pAttrEx->m_iValue=qry.field(4).asInteger();
		pAttrEx->m_iType=qry.field(5).asInteger();
		i++;
	}
	qry.close ();
	qry.commit();
}

