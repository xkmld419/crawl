#include "ABMSynRcv.h"
#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "PublicConndb.h"

static vector<QryServSynData *> g_vData;					//����ͬ�����Ӧ��
static vector<QryServSynData *>::iterator g_its;	
	
static char cPreSessionId[100]={0};

ABMSynRcv::ABMSynRcv()
{
		m_poUnpack = NULL;
		m_poSql = NULL;
		m_dccOperation = new DccOperation;	
}

ABMSynRcv::~ABMSynRcv()
{
	if (m_dccOperation != NULL)
		delete m_dccOperation;
	if (m_poSql != NULL)
        delete m_poSql;
}

int ABMSynRcv::init(ABMException &oExp)
{
	 if ((m_poSql=new QryServSynSql) == NULL) {
        ADD_EXCEPT0(oExp, "ABMSynRcv::init malloc failed!");
        return -1;
    }
    return 0;
}

int ABMSynRcv::deal(ABMCCR *pRecivePack, ABMCCA *pSendPack)
{
	__DEBUG_LOG0_(0, "=========>ABMSynRcv::deal ��Ӧ����ʼ");
	__DEBUG_LOG0_(0, "=========>1������ȫ��HSS���ص�CCA...");
	
	int iRet=0;
	int iSize=0;
	
	// ��ȡpRecivePack�Ĺ���CCA��Ϣͷ
	char sSessionId[64];
	struDccHead Head;

	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"A"); 
	long lDccRecordSeq=0L; // ��ˮ�ţ�����
	
		// ȡ��Ϣ������Ϣͷ��Ϣ
		memcpy((void *)&Head,(void *)(pRecivePack->m_sDccBuf),sizeof(Head));
		// �յ�CCA��Session-Id
		__DEBUG_LOG1_(0, "=======>�յ�ȫ��HSS���ص�CCA����Ϣͷ��SESSION_ID:[%s]",Head.m_sSessionId);
		strcpy(cPreSessionId,Head.m_sSessionId);
		
		//���
		m_poUnpack = (QryServSynCCA *)pRecivePack;
		
		 if((iRet = m_poUnpack->getServSyn(g_vData)) == 0){
		         __DEBUG_LOG0_(0, "ABMSynRcv:: deal ��Ϣ��ΪNULL!");
		         return INFO_SYN_UNPACK_ERR;
		}
		else if (iRet < 0)
		{
			__DEBUG_LOG0_(0, "ABMSynRcv:: deal ��Ϣ���������!");	
			return INFO_SYN_UNPACK_ERR;
		}
		
	try
	{
		//CCA�������
		for(int i = 0; i <g_vData.size(); ++i)
		{
			if(g_vData[i]->m_iRltCode == 0){
			iRet = _deal(g_vData[i],cPreSessionId);
			__DEBUG_LOG1_(0, "ABMSynRcv::deal iRet=%d", iRet);
			}
			else{
			iRet = _dealErr(g_vData[i],cPreSessionId);
			__DEBUG_LOG1_(0, "ABMSynRcv::dealErr iRet=%d", iRet);
			}
				
			m_poUnpack->clear();
		}
	}
	catch(TTStatus oSt)
	{
		__DEBUG_LOG1_(0, "ABMSynRcv::deal oSt.err_msg=%s", oSt.err_msg);
	}
	return iRet;
}

int ABMSynRcv::_deal(QryServSynData *pData,char *m_oSessionId)
{
	__DEBUG_LOG0_(0, "enter ABMSynRcv::_deal" );
	ABMException oExp;
    int iRet=0;
   
   //ҵ����:�жϷ��ؽ������
   try{
   		//ƴ��������д���Ź�����
	iRet = m_poSql->insertSendSql(m_oSessionId,oExp);
	if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "=====>insertSendSql error");
			throw iRet;
		}
   //�û���������ͬ�����״̬  
	iRet = m_poSql->updateServAddState(m_oSessionId,oExp);
	if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "=====>deleteServAdd error");
			throw iRet;
		}
	}
	catch(TTStatus oSt)
	{
		iRet = INFOSYN_TT_ERR;
		__DEBUG_LOG1_(0, "��������ͬ��ʧ�ܣ�������Ϣ=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet = value;
		__DEBUG_LOG1_(0, "��������ͬ��ʧ�ܣ�������Ϣ=%d",value);
	}
   return iRet;
}

int ABMSynRcv::_dealErr(QryServSynData *pData,char *m_oSessionId)
{
	__DEBUG_LOG0_(0, "enter ABMSynRcv::_dealErr" );
	ABMException oExp;
    int iRet=0;
    
   //ҵ����
   try
   {
   		//HSS�˷��ش������TT
   		iRet = m_poSql->insertUserAddErr(m_oSessionId,pData->m_iRltCode,oExp);
   		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "=====>insertUserAddErr error");
			throw iRet;
		}
   }
   catch(TTStatus oSt)
	{
		iRet = INFOSYN_TT_ERR;
		__DEBUG_LOG1_(0, "��������ͬ���������Ϣʧ�ܣ�������Ϣ=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet = value;
		__DEBUG_LOG1_(0, "��������ͬ���������Ϣʧ�ܣ�������Ϣ=%d",value);
	}
   return iRet;
}
