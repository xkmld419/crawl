#include "QryServInfosyn.h"
#include "QryServInfosynSql.h"
#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"

static vector<QryServSynCond *> g_vSyn;
static vector<QryServSynCond *>::iterator g_its; 

QryServInfosyn::QryServInfosyn()
{
    m_poSql = NULL;
    m_poUnpack = NULL;
    m_poPack = NULL;
    m_dccOperation = new DccOperation;
}

QryServInfosyn:: ~QryServInfosyn()
{
    if (m_poSql != NULL)
        delete m_poSql;
    
    FreeObject(m_dccOperation);
}

int QryServInfosyn::init(ABMException &oExp)
{
    if ((m_poSql=new QryServInfosynSql) == NULL) {
        ADD_EXCEPT0(oExp, "QryServInfosyn::init malloc failed!");
        return -1;
    }
/*
    try{
    if(m_poSql->init(oExp) != 0){
    		ADD_EXCEPT0(oExp, "QryServInfosyn::init m_poSql->init failed!");
			return -1;
    	}
   		 return 0;
   		}
   		catch(TTStatus oSt)
         {
             ADD_EXCEPT1(oExp, "QryServInfosyn::init oSt.err_msg=%s", oSt.err_msg);
         }
        return -1;
 */
 	return 0;
}

int QryServInfosyn::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
	int iRet;
	m_poUnpack = (QryServSynCCR *)pCCR;
	m_poPack = (QryServSynCCA *)pCCA;
	m_poPack->clear();
	g_vSyn.clear();
	
	//��Ϣ���
	if ((iRet=m_poUnpack->getServSyn(g_vSyn)) == 0)
    {
        __DEBUG_LOG0_(0, "QryServInfosyn::deal ��Ϣ��ΪNULL");
        m_poPack->setRltCode(ECODE_NOMSG);
        return -1;
    }
    else if (iRet < 0) 
    {
        __DEBUG_LOG0_(0, "QryServInfosyn::deal ��Ϣ�������!");
        m_poPack->setRltCode(ECODE_UNPACK_FAIL);
        return -1;
    }
    try{
  		for (int i=0; i<g_vSyn.size(); ++i){	
							iRet=_deal(g_vSyn[i]);		
				__DEBUG_LOG1_(0, "QryServInfosyn::deal iRet=%d", iRet);
			  	}
		}
		catch(TTStatus oSt) 
			  {
			      __DEBUG_LOG1_(0, "QryServInfosyn::deal oSt.err_msg=%s", oSt.err_msg);
				m_poPack->clear();
			  	m_poPack->setRltCode(ECODE_TT_FAIL);
			  }
    
    return iRet;
}

int QryServInfosyn::_deal(QryServSynCond *pIn)
{
	ABMException oExp;
	QryServSynData oRes;
	struDccHead ccaHead={0};
	char dccType[2]="R";
	long lRecordIdSeq=0;
	
	int iRet = 0;
	__DEBUG_LOG1_(0, "pIn->m_sReqID=%s", pIn->m_sReqID);
	__DEBUG_LOG1_(0, "pIn->m_hDsnType=%d", pIn->m_hDsnType);
	__DEBUG_LOG1_(0, "pIn->m_sDsnNbr=%s", pIn->m_sDsnNbr);
	__DEBUG_LOG1_(0, "pIn->m_hDsnAttr=%d", pIn->m_hDsnAttr);
	__DEBUG_LOG1_(0, "pIn->m_sServPlatID=%s", pIn->m_sServPlatID);
	__DEBUG_LOG1_(0, "pIn->m_lServID=%ld", pIn->m_lServID);
	__DEBUG_LOG1_(0, "pIn->m_lAcctID=%ld", pIn->m_lAcctID);
	__DEBUG_LOG1_(0, "pIn->m_sActType=%s", pIn->m_sActType);
	__DEBUG_LOG1_(0, "pIn->m_sAccPin=%s", pIn->m_sAccPin);
	
	
	try
	{
		//��Ϣͷ���
		iRet=m_dccOperation->QueryDccRecordSeq(lRecordIdSeq);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "֧��ȡDCC���д�=[%d]",iRet);
			throw iRet;
		}
		memcpy((void *)&ccaHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(ccaHead));
		iRet=m_dccOperation->insertDccInfo(ccaHead,dccType,lRecordIdSeq);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "֧������DCCͷ��=[%d]",iRet);
			throw iRet;
		}
		
	switch(atoi(pIn->m_sActType)){
		case 2:
			//����
			// add 2011.7.5
			//serv_his���¼
			iRet = m_poSql->insertServHis(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>insertServHis error");
				throw iRet;
			}
			// ���Serv���¼
			iRet = m_poSql->deleteServInfo(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>deleteAcctInfo error");
				throw iRet;
			}
			
			//дAcct����־��¼��
			iRet = m_poSql->insertAcctHis(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>insertAcctHis error");
				throw iRet;
			}
			iRet = m_poSql->deleteAcctInfo(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>deleteAcctInfo error");
				throw iRet;
			}
			//����ACCT
			iRet = m_poSql->createAcct(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>createAcct error");
				throw iRet;
			}
			//�޸�SERV_ACCT
			iRet = m_poSql->updateServAcct(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>updateServAcct error");
				throw iRet;
			}
			break;
		case 3:
			//����			
			//serv_his���¼
			iRet = m_poSql->insertServHis(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>insertServHis error");
				throw iRet;
			}
			//acct_his���¼	
			iRet = m_poSql->insertAcctHis(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>insertAcctHis error");
				throw iRet;
			}
			//serv_acct_his���¼	
			iRet = m_poSql->insertServAcctHis(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>insertServAcctHis error");
				throw iRet;
			}
			//acct_balance��������Ϊ0��¼
			iRet = m_poSql->deleteAcctBalance(pIn,oExp);
			 if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>deleteAcctBalanceHis error");
				throw iRet;
			}	
			//��serv_acct���¼
			iRet = m_poSql->deleteServAcctInfo(pIn,oExp);
			 if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>deleteServAcctInfo error");
				throw iRet;
			}
			//��acct���¼
			iRet = m_poSql->deleteAcctInfo(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>deleteAcctInfo error");
				throw iRet;
			}
			//��serv���¼
		   iRet = m_poSql->deleteServInfo(pIn,oExp);
		   if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>deleteAcctInfo error");
				throw iRet;
			}
			break;
		case 4:
			//����
			//��¼SERV��ʷ��
			iRet = m_poSql->changeInservHis(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>changeInservHis error");
				throw iRet;
			}
			//�����滻
			iRet = m_poSql->changeNbr(pIn,oExp);
			if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>changeNbr error");
				throw iRet;
			}	
			break;
			default:
        __DEBUG_LOG1_(0, "QryServInfosyn::_deal Receive unknown ActType = %s", pIn->m_sActType);
        break;
		}
	}
	catch(TTStatus oSt) 
	{
		iRet=-1;
		__DEBUG_LOG1_(0, "����or����or����ʧ�ܣ�������Ϣ=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "����or����or����ʧ�ܣ�������Ϣ=%d",value);
	}
	//CCA���

	oRes.m_iRltCode = iRet;
	strcpy(oRes.m_sResID,pIn->m_sReqID);
	if (iRet == 0){
		sprintf(oRes.m_sParaRes,"");
	}else{
		sprintf(oRes.m_sParaRes,"HSS Acc_Nbr=<%s>, ServId=<%ld>,Acct=<%ld>",pIn->m_sDsnNbr,pIn->m_lServID,pIn->m_lAcctID);
	}
	if (!m_poPack->addServSyn(oRes)) 
	{
		__DEBUG_LOG0_(0, "QryServInfosyn:: _deal m_poPack ���ʧ��!");
		m_poPack->setRltCode(ECODE_PACK_FAIL);
		return  -1;
	}
	
	return iRet;
}
