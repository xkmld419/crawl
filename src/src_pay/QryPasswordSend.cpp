#include "QryPasswordSend.h"
#include "QryPasswordsendSql.h"
#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"


static vector<PasswordResponsStruct *> g_vRes;
static vector<PasswordResponsStruct *>::iterator g_its;

static vector<QryPasswordCond *> g_vPass;
static vector<QryPasswordCond *>::iterator g_itr;

QryPasswordSend::QryPasswordSend()
{
    m_poSql = NULL;
    m_poUnpack = NULL;
    m_poPack = NULL;
    m_dccOperation = new DccOperation;
}

QryPasswordSend:: ~QryPasswordSend()
{
    if (m_poSql != NULL)
        delete m_poSql;
    
     FreeObject(m_dccOperation);
}

int QryPasswordSend::init(ABMException &oExp)
{
    if ((m_poSql=new QryPasswordSendSql) == NULL) {
        ADD_EXCEPT0(oExp, "QryPasswordSend::init malloc failed!");
        return -1;
    }
    try{
    if(m_poSql->init(oExp) != 0){
    		ADD_EXCEPT0(oExp, "QryPasswordSend::init m_poSql->init failed!");
			return -1;
    	}
   		 return 0;
   		}
   		catch(TTStatus oSt)
         {
             ADD_EXCEPT1(oExp, "QryPasswordSend::init oSt.err_msg=%s", oSt.err_msg);
         }
        return -1;
}

int QryPasswordSend::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
    int iRet;
    m_poUnpack = (QryPasswordCCR *)pCCR;
    m_poPack = (QryPasswordCCA *)pCCA;
    m_poPack->clear();
    g_vRes.clear();
    g_vPass.clear();
    //g_psct=malloc(sizeof(struct PasswordResponsStruct));
    
    //��Ϣ���
/*
    if ((iRet=m_poUnpack->getpasswdRespons(g_psct)) == 0) {
        __DEBUG_LOG0_(0, "QryPasswordSend::deal ��Ϣ��ΪNULL");
        m_poPack->setRltCode(ECODE_NOMSG);
        return -1;
    }
    else if (iRet < 0) {
        __DEBUG_LOG0_(0, "QryPasswordSend::deal ��Ϣ�������!");
        m_poPack->setRltCode(ECODE_UNPACK_FAIL);
        return -1;
    }
    
    if ((iRet=m_poUnpack->getpasswdCond(g_vPass)) == 0) {
        __DEBUG_LOG0_(0, "QryPasswordSend::deal ��Ϣ��ΪNULL");
        m_poPack->setRltCode(ECODE_NOMSG);
        return -1;
    }
    else if (iRet < 0) {
        __DEBUG_LOG0_(0, "QryPasswordSend::deal ��Ϣ�������!");
        m_poPack->setRltCode(ECODE_UNPACK_FAIL);
        return -1;
    }
*/
	/*һ����*/
	if ((iRet=m_poUnpack->getpasswdRespons(g_vRes)) == 0) {
        __DEBUG_LOG0_(0, "QryPasswordSend::deal ��Ϣ��ΪNULL");
        m_poPack->setRltCode(ECODE_NOMSG);
        return -1;
    }
    else if (iRet < 0) {
        __DEBUG_LOG0_(0, "QryPasswordSend::deal ��Ϣ�������!");
        m_poPack->setRltCode(ECODE_UNPACK_FAIL);
        return -1;
    }
    //
    try {
        for (g_its=g_vRes.begin(); g_its!=g_vRes.end(); ++g_its) {
        	/*������*/
			        	if ((iRet=m_poUnpack->getServPassWd(*g_its,g_vPass)) == 0) {
			        __DEBUG_LOG0_(0, "QryPasswordSend::deal ��Ϣ��ΪNULL");
			        m_poPack->setRltCode(ECODE_NOMSG);
			        return -1;
			    }
			    else if (iRet < 0) {
			        __DEBUG_LOG0_(0, "QryPasswordSend::deal ��Ϣ�������!");
			        m_poPack->setRltCode(ECODE_UNPACK_FAIL);
			        return -1;
			    }
			    for(g_itr=g_vPass.begin();g_itr!=g_vPass.end();++g_itr){
			            if (_deal(*g_its,*g_itr) != 0)
			                break;
			              }
        }
        return 0;
    }
    catch(TTStatus oSt) {
        __DEBUG_LOG1_(0, "QryPasswordSend::deal oSt.err_msg=%s", oSt.err_msg);
    }
    m_poPack->clear();
    m_poPack->setRltCode(ECODE_TT_FAIL);
    
    return 0;
}

int QryPasswordSend::_deal(PasswordResponsStruct *oRes,QryPasswordCond *pIn)
{
	int i;
   int j;
   char m[8];
   char randompd[8]={};
   time_t t;
   QryPasswordData oData;
   ABMException oExp;
   char  sCurSms[1024];
   char sBuffer[1024];
   
   struDccHead ccaHead={0};
	char dccType[2]="R";
	long lRecordIdSeq=0;
   
   	int iRet=0;
   	int iRes;
   
   //�����������
   srand((unsigned) time(&t));

   for(i=0; i<4; i++){
        j=rand() % 10;
      sprintf(m,"%d",j);
      strcat(randompd,m);
   }
   
   	sprintf(sCurSms,"�𾴵Ŀͻ���������ʹ���й���������֧������,�������Ϊ%s\n",randompd);
	
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
		
	   //�ж��Ƿ��ǵ�һ���·�
		iRes=m_poSql->selectServNbr(pIn,oExp);
	   
			if(iRes == 0){
	   //д��������
	   		__DEBUG_LOG0_(0, "׼��insertpwSql...");
	    iRet = m_poSql->insertpwSql(randompd,oRes,pIn,oExp);
	    if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>insertpwSql error");
				throw iRet;
			}
		}else if(iRes == 1){
			//������������
				 iRet = m_poSql->updatepwSql(randompd,pIn,oExp);
				 if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>updatepwSql error");
				throw iRet;
			}
		}
	   
	   //�����û�������û�ID
	   	 iRet = m_poSql->selectServID(pIn,oExp);
		  if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>selectServID error");
				throw iRet;
			}
			
		 //д���Ź�����
		iRet = m_poSql->insertSendSql(m_poSql->servId,sCurSms,pIn,oExp);
		if(iRet !=0 )
			{
				__DEBUG_LOG0_(0, "=====>insertSendSql error");
				throw iRet;
			}
     }
     catch(TTStatus oSt) 
	{
		iRet=-1;
		__DEBUG_LOG1_(0, "PaymentInfo::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "PaymentInfo::deal value=%d",value);
	}
   		//����CCA
	oData.m_SevResCode=iRet;   //�������
	snprintf(oData.ResID, sizeof(oData.ResID), "%s", oRes->m_ReqID);
	//strcpy(oData.ResTime,oRes->m_ReqTime);
	oData.ResTime=oRes->m_ReqTime;
	
	 if (!m_poPack->addServPassWdData(oData)) {
            __DEBUG_LOG0_(0, "QryServInfor:: _deal m_poPack ���ʧ��!");
            m_poPack->setRltCode(ECODE_PACK_FAIL);
            return -1;
        }
  
    return iRes;
}
