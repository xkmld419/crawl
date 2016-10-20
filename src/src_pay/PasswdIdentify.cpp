#include "PasswdIdentify.h"
#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "PublicConndb.h"

static vector<LoginPasswdStruct *> g_vLoginPasswdStruct;
static vector<LoginPasswdStruct *>::iterator g_itr1; 
	
static vector<LoginPasswdCond *> g_vLoginPasswdCond;
static vector<LoginPasswdCond *>::iterator g_itr2;
	
PasswdIdentify::PasswdIdentify()
{
    m_poPasswdPack = NULL;
    m_poUnpack = NULL;
    m_poPack = NULL;
    m_dccOperation = new DccOperation;
}

PasswdIdentify:: ~PasswdIdentify()
{
    if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int PasswdIdentify::deal(ABMCCR *pCCR, ABMCCA *pSendPack)
{
	__DEBUG_LOG0_(0, "=========>PasswdIdentify::deal �������ʼ");
	__DEBUG_LOG0_(0, "=========>1���������Է��������CCR...");
	int iRet=0;
	int iSize=0;
	
	m_poUnpack = (LoginPasswdCCR *)pCCR; // �������CCR
	m_poPack= (LoginPasswdCCA *)pSendPack;
	m_poPasswdPack = (PasswdIdentifyCCR *)pSendPack;    // �������CCR
	
	m_poPack->clear();
	m_poPasswdPack->clear();
	g_vLoginPasswdStruct.clear();
	g_vLoginPasswdCond.clear();
	
	//��Ϣ��� - ��һ��
	if ((iSize=m_poUnpack->getLoginPasswdRespons(g_vLoginPasswdStruct)) == 0 )
	{
		__DEBUG_LOG0_(0, "PasswdIdentify::deal �Է���ƽ̨�����½������֤�����������Ϣ��ΪNULL");
		m_poPack->setRltCode(ECODE_NOMSG);
		return -1;
	}
	else if (iSize < 0)
	{
		__DEBUG_LOG0_(0, "PasswdIdentify::deal ��Ϣ�������!");
		m_poPack->setRltCode(ECODE_UNPACK_FAIL);
		return -1;
	}
	
	try
	{		
		// �����һ����Ϣ
		for (g_itr1=g_vLoginPasswdStruct.begin(); g_itr1!=g_vLoginPasswdStruct.end(); ++g_itr1)
		{
			// ��detail�� - �ڶ���
			if ((iSize=m_poUnpack->getLoginPasswd(*g_itr1,g_vLoginPasswdCond)) == 0 )
			{
				__DEBUG_LOG0_(0, "PasswdIdentify::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack->setRltCode(ECODE_NOMSG);
				return -1;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "PasswdIdentify::deal ��Ϣ�������!");
				m_poPack->setRltCode(ECODE_UNPACK_FAIL);
				return -1;
			}
			 
			__DEBUG_LOG0_(0, "=========>2��_deal.�����Է���ƽ̨�������CCR��Ϣ.");			
			// ����
			if (_deal(*g_itr1,g_vLoginPasswdCond) != 0)
				break;
		}
		__DEBUG_LOG1_(0, "xxxxxx=PasswdIdentify::deal iRet=%d",iRet);
	}
	catch(TTStatus oSt)
	{
		iRet = -1;
		__DEBUG_LOG1_(0, "PasswdIdentify::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "PasswdIdentify::QryPayoutInfo value=%d",value);
	}
	
	// ҵ����ʧ�ܣ��򷵻ذ���TODO��
	if(iRet!=0)
	{
		m_poPack->clear();
		m_poPack->setRltCode(ECODE_TT_FAIL);
	}
	__DEBUG_LOG1_(0, "PasswdIdentify::deal iRet=%d",iRet);
	return iRet;
}

int PasswdIdentify::_deal(LoginPasswdStruct *oRes,vector<LoginPasswdCond* > g_vLoginPasswdCond)
{
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "=======================��½�����Ȩ=======================");
	__DEBUG_LOG0_(0, "=========>ȡ�Է���ƽ̨����CCR��һ�������Ϣ:");
	__DEBUG_LOG1_(0, "[��½�����Ȩ-�շ�CCR,PasswdIdentify],������ˮ��m_sReqID:%s",oRes->m_sReqID);
	__DEBUG_LOG1_(0, "[��½�����Ȩ-�շ�CCR,PasswdIdentify],����ʱ�䣺m_iReqTime:%d",oRes->m_iReqTime);
	
	if(oRes->m_iReqTime > 0)
		long2date(oRes->m_iReqTime,oRes->m_requestTime);
	else
		GetTime(oRes->m_requestTime);
	
	int iRet=0;
	int LoginServType; //�û����ͣ��������û� OR ͨ��֤�û�
	
	try
	{
		for (g_itr2=g_vLoginPasswdCond.begin(); g_itr2!=g_vLoginPasswdCond.end(); ++g_itr2)
		{
			__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,PasswdIdentify]:ȡ�Է���ƽ̨����CCR�ڶ���֧����Ϣ��");
			 LoginServType = (*g_itr2)->m_hDestType;
			//__DEBUG_LOG1_(0, "[��½�����Ȩ-��CCR],�û�����,m_sDestAccount:[%s]",(*g_itr2)->m_sDestAccount);
			__DEBUG_LOG1_(0, "[��½�����Ȩ-��CCR],��ʶ����,m_hDestType:[%d](1-ͨ��֤�ʺ� 2-ҵ��ƽ̨�ʺ�)",LoginServType);
			//__DEBUG_LOG1_(0, "[��½�����Ȩ-��CCR],�û�����,m_hDestAttr:[%d]",(*g_itr2)->m_hDestAttr);
			//__DEBUG_LOG1_(0, "[��½�����Ȩ-��CCR],ҵ��ƽ̨��ʶ,m_sServPlatID:[%s]",(*g_itr2)->m_sServPlatID);
			//__DEBUG_LOG1_(0, "[��½�����Ȩ-��CCR],��¼����,m_sLoginPasswd:[%s]",(*g_itr2)->m_sLoginPasswd);
			
			switch(LoginServType)
			{
				case 2:
					//�������û�
					//����3.4.1.3HSS�����Ȩ�ӿ�
					__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,PasswdIdentify]:�������û�����HSS�����Ȩ�ӿ�");
					
					iRet = loginHSSIdentify(oRes,*g_itr2);
			 		if(iRet!=0)
			 		{
			 			throw -1;
			 			__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,PasswdIdentify]:��½�����Ȩ��ȫ������HSS��Ȩʧ��");
			 		}
			 		break;
				case 1:
					//ͨ��֤�û�
					//����3.4.3.1ͨ��֤�û���Ȩ�ӿ�-ȫ������UDB��֤,���޷�ʵ��
					__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,PasswdIdentify]:ͨ��֤�û�����ͨ��֤�û���Ȩ�ӿ�");
					//ͨ��֤�û������Ȩ����
					
					iRet = loginHSSIdentify(oRes,*g_itr2);
			 		if(iRet!=0)
			 		{
			 			throw -1;
			 			__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,PasswdIdentify]:��½�����Ȩ��ȫ������HSS��Ȩʧ��");
			 		}
					
					break;
			}
		}
	}
	catch(TTStatus oSt) 
	{
		iRet=-1;
		__DEBUG_LOG1_(0, "[��½�����Ȩ-�շ�CCR,PasswdIdentify] oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��½�����Ȩ-�շ�CCR,PasswdIdentify] iRet=%d",value);
	}
	
	return iRet;	
}

//�������û�����ȫ��HSS�������Ȩ
int PasswdIdentify::loginHSSIdentify(LoginPasswdStruct *oRes,LoginPasswdCond * g_itr2)
{
	int iRet=0;
	
	//����CCR��Ϣ
	__DEBUG_LOG0_(0, "========================��������ABM��½�����ȨCCR��TT=======================");
	__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,�Է���ƽ̨]�Է�ƽ̨����CCR��ϸ:");
	__DEBUG_LOG1_(0, "������ˮ��m_sReqID:[%s]",oRes->m_sReqID);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_iReqTime:[%d]",oRes->m_iReqTime);
	__DEBUG_LOG1_(0, "�û����룺m_sDestAccount:[%s]",g_itr2->m_sDestAccount);
	__DEBUG_LOG1_(0, "��ʶ���ͣ�m_hDestType:[%d]",g_itr2->m_hDestType);
	__DEBUG_LOG1_(0, "�û����ԣ�m_hDestAttr:[%d]",g_itr2->m_hDestAttr);
	__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��m_sServPlatID:[%s]",g_itr2->m_sServPlatID);
	__DEBUG_LOG1_(0, "��½���룺m_sLoginPasswd:[%s]",g_itr2->m_sLoginPasswd);
	
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	
	try
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,�Է���ƽ̨]:����CCR����Ϣͷ��Ϣ����TT");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,�Է���ƽ̨]:����CCR����Ϣͷ��Ϣʧ��");
			throw -1;
		}
		
		// dcc��ҵ��������Ϣ
		__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,�Է���ƽ̨]:����CCR��ҵ����Ϣ����TT");
		iRet = m_dccOperation->savePlatformCCRforIdentify(receiveDccHead.m_sSessionId,oRes,g_itr2);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������-�շ�CCR,��������]:����CCRҵ����Ϣʧ��");
			throw -1;
		}
		
		// ��CCR������Ϣ
		__DEBUG_LOG0_(0, "========================���½�����ȨCCR������Ϣ=======================");
		
		// HSS�������ȨCCR
		PasswdIdentifyCond oReqPasswdIdentify;
		
		//����׼��
		strcpy(oReqPasswdIdentify.m_sReqID,oRes->m_sReqID);	//������ˮ
		oReqPasswdIdentify.m_hDsnType = g_itr2->m_hDestType;		//�û�����
		strcpy(oReqPasswdIdentify.m_sDsnNbr,g_itr2->m_sDestAccount);	//�û�����
		oReqPasswdIdentify.m_hDsnAttr = g_itr2->m_hDestAttr;	//�û�����
		strcpy(oReqPasswdIdentify.m_sServPlatID,g_itr2->m_sServPlatID);	//ҵ��ƽ̨��ʶ
		strcpy(oReqPasswdIdentify.m_sLoginPasswd,g_itr2->m_sLoginPasswd);	//��½����
		
		__DEBUG_LOG1_(0, "������ˮ��oReqPasswdIdentify.m_sReqID:[%s]",oReqPasswdIdentify.m_sReqID);
		__DEBUG_LOG1_(0, "�û����ͣ�oReqPasswdIdentify.m_hDsnType:[%d]",oReqPasswdIdentify.m_hDsnType);
		__DEBUG_LOG1_(0, "�û����룺oReqPasswdIdentify.m_sDsnNbr:[%s]",oReqPasswdIdentify.m_sDsnNbr);
		__DEBUG_LOG1_(0, "�û����ԣ�oReqPasswdIdentify.m_hDsnAttr:[%d]",oReqPasswdIdentify.m_hDsnAttr);
		__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��oReqPasswdIdentify.m_sServPlatID:[%s]",oReqPasswdIdentify.m_sServPlatID);
		__DEBUG_LOG1_(0, "��½���룺oReqPasswdIdentify.m_sLoginPasswd:[%s]",oReqPasswdIdentify.m_sLoginPasswd);
		
		//������Ϣͷ
		// ����Session-id
		long lSessionIdSeq;
		iRet = m_dccOperation->QueryPaySeq(lSessionIdSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,����HSS��]:��ѯDCC�Ự��ʶSession-Idʧ��");
			throw -1;
		}
		
		struct struDccHead Head;
		char sSessionId[64];
		time_t tTime;
		tTime = time(NULL);
		// 2011.8.1
		snprintf(Head.m_sSessionId,sizeof(Head.m_sSessionId),"%s%010d%s%04d\0","ABM@001.ChinaTelecom.com;",tTime,";",lSessionIdSeq);
		__DEBUG_LOG1_(0, "SESSION_ID:[%s]",Head.m_sSessionId);
			
		strcpy(Head.m_sOutPlatform,""); // �ⲿƽ̨,SGWʹ��
		//strcpy(Head.m_sOutPlatform,"001.ChinaTelecom.com");
		strcpy(Head.m_sOrignHost,"ABM@001.ChinaTelecom.com"); // 2011.8.1
		strcpy(Head.m_sOrignRealm,"001.ChinaTelecom.com");

		char sOrgId[5]={0};
		char sLocalAreaId[5]={0};
		iRet = m_dccOperation->qryAccountOrgId(oReqPasswdIdentify.m_sDsnNbr,sOrgId,sLocalAreaId);
		
		__DEBUG_LOG1_(0, "[��½�����Ȩ-�շ�CCR,����HSS��]:������������:[%s]",sOrgId);
		__DEBUG_LOG1_(0, "[��½�����Ȩ-�շ�CCR,����HSS��]:������������:[%s]",sLocalAreaId);

		if(iRet !=0 )
		{
			__DEBUG_LOG0_(0, "error");
			throw -1;
		}
		
		snprintf(Head.m_sDestRealm,sizeof(Head.m_sDestRealm),"%s.ChinaTelecom.com","001");
		__DEBUG_LOG1_(0, "Head.m_sDestRealm:[%s]",Head.m_sDestRealm);
		
		// ȡ������OrgId
		snprintf(Head.m_sSrvTextId,sizeof(Head.m_sSrvTextId),"PaymentPasswdAuthen.Micropayment@001.ChinaTelecom.com");
		//strcpy(Head.m_sSrvTextId,"PaymentPasswdAuthen.Micropayment@%s.ChinaTelecom.com",sOrgId);
		
		Head.m_iAuthAppId = 4;
		Head.m_iReqType = 4;
		Head.m_iReqNumber  = 0;
		Head.m_iResultCode = 0;
		strcpy(Head.m_sSrvFlowId,""); // ������ȡԴm_sSrvFlowId��׷��
		snprintf(Head.m_sSrvFlowId,sizeof(Head.m_sSrvFlowId),"%s%s",receiveDccHead.m_sSrvFlowId,"test---");
		
		// ����CCR��Ϣͷ��ֵ
		memcpy((void *)(m_poPasswdPack->m_sDccBuf),(void *)&Head,sizeof(Head));
		
		m_poPasswdPack->m_iCmdID = ABMCMD_PASSWD_IDENTIFY;
		
		//���
		if(!m_poPasswdPack->addPasswdIdentify(oReqPasswdIdentify))
			{
			__DEBUG_LOG0_(0, "PasswdIdentify::loginHSSIdentify CCR���ʧ��!");
			m_poPasswdPack->setRltCode(ECODE_PACK_FAIL);
			throw -1;
		}
		__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,����HSS��]:CCA������");
		
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		// ���淢����CCR��Ϣ
		__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,����HSS��]:���淢����DCC����Ϣ��TT");
		iRet = m_dccOperation->insertDccInfo(Head,dccType,lDccRecordSeq,receiveDccHead.m_sSessionId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��½�����Ȩ-�շ�CCR,����HSS��]:����DCCҵ����Ϣʧ��");
			throw -1;
		}
	}
	catch(TTStatus oSt)
	{
		iRet = -1;
		__DEBUG_LOG1_(0, "[��½�����Ȩ-�շ�CCR,����HSS��]: oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��½�����Ȩ-�շ�CCR,����HSS��]: iRet=%d",value);
	}
	__DEBUG_LOG1_(0, "[��½�����Ȩ-�շ�CCR,����HSS��]-ActiveAllocateBalance::deductBalanceOut,iRet=:[%d]",iRet);
	
	return iRet;
}
