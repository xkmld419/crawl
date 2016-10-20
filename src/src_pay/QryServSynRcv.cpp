#include "QryServSynRcv.h"

static vector<QryServSynData *> g_vData;					//����ͬ�����Ӧ��
static vector<QryServSynData *>::iterator g_its;	

QryServSynRcv::QryServSynRcv()
{
    m_poSql = NULL;
    //oCCA = NULL;
    //oCCR = NULL;
}

QryServSynRcv:: ~QryServSynRcv()
{
    if (m_poSql != NULL)
        delete m_poSql;
}

int QryServSynRcv::init(ABMException &oExp)
{
	 if ((m_poSql=new QryServSynSql) == NULL) {
        ADD_EXCEPT0(oExp, "QryServInfor::init malloc failed!");
        return -1;
    }
/*
    if (m_poSql->init(oExp) != 0) {
        ADD_EXCEPT0(oExp, "QryServInfor::init m_poSql->init failed!");
        return -1;
    }
*/
    return 0;
}

int QryServSynRcv::deal(QryServSynCCA *pCCA)
{
	int iRet;
	if ((pCCA==NULL))
        return -1;
	oCCA = (QryServSynCCA *)pCCA;
	struDccHead *pHead;
	pHead = (struDccHead *)oCCA->m_sDccBuf;
	g_vData.clear();
	
	//���
   if((iRet = oCCA->getServSyn(g_vData)) == 0){
		          __DEBUG_LOG0_(0, "QryServSyn:: _deal ��Ϣ��ΪNULL!");
		         return -1;
	}
	else if (iRet < 0)
	{
		__DEBUG_LOG0_(0, "QryServSyn:: _deal ��Ϣ���������!");	
	}
	for(int i = 0; i <g_vData.size(); ++i)
	{
		try{
			if(g_vData[i]->m_iRltCode == 0){
			iRet = _deal(g_vData[i],pHead->m_sSessionId);
			__DEBUG_LOG1_(0, "QryServSynRcv::deal iRet=%d", iRet);
			}
			else
				continue;
				oCCA->clear();
		}
		catch(TTStatus oSt)
		{
			__DEBUG_LOG1_(0, "QryServSynRcv::deal oSt.err_msg=%s", oSt.err_msg);
			oCCA->clear();
		}	
	}
	return iRet;
}

int QryServSynRcv::_deal(QryServSynData *pData,char *m_oSessionId)
{
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
		iRet =-1;
		__DEBUG_LOG1_(0, "��������ͬ��ʧ�ܣ�������Ϣ=%s", oSt.err_msg);
	}
	catch(int &value)
	{
		iRet = value;
		__DEBUG_LOG1_(0, "��������ͬ��ʧ�ܣ�������Ϣ=%d",value);
	}
   //ҵ����ɺ�
   oCCA->clear();
   
   return iRet;
}