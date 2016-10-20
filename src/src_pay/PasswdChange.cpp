#include "PasswdChange.h"
#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "PublicConndb.h"

static vector<PasswdChangeStruct *> g_vPasswdChangeStruct;
static vector<PasswdChangeStruct *>::iterator g_itr1; 
	
static vector<PasswdChangeCond *> g_vPasswdChangeCond;
static vector<PasswdChangeCond *>::iterator g_itr2;

PasswdChange::PasswdChange()
{
    m_poPasswdPack = NULL;
    m_poPack = NULL;
    m_dccOperation = new DccOperation;
}

PasswdChange:: ~PasswdChange()
{
    if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int PasswdChange::deal(ABMCCR *pCCR, ABMCCA *pSendPack)
{
	__DEBUG_LOG0_(0, "=========>PasswdChange::deal �������ʼ");
	__DEBUG_LOG0_(0, "=========>1���������Է��������CCR...");
	int iRet=0;
	int iSize=0;
	
	m_poUnpack = (PasswdChangeCCR *)pCCR; // �������CCR
	m_poPack= (PasswdChangeCCA *)pSendPack;
	m_poPasswdPack = (PasswdChangeCCR *)pSendPack;    // �������CCR 
	
	m_poPack->clear();
	m_poPasswdPack->clear();
	g_vPasswdChangeStruct.clear();
	g_vPasswdChangeCond.clear();
	
	//��Ϣ��� - ��һ��
	if ((iSize=m_poUnpack->getPasswdChangeRespons(g_vPasswdChangeStruct)) == 0 )
	{
		__DEBUG_LOG0_(0, "PasswdChange::deal �Է���ƽ̨�����½������֤�����������Ϣ��ΪNULL");
		m_poPack->setRltCode(ECODE_NOMSG);
		return -1;
	}
	else if (iSize < 0)
	{
		__DEBUG_LOG0_(0, "PasswdChange::deal ��Ϣ�������!");
		m_poPack->setRltCode(ECODE_UNPACK_FAIL);
		return -1;
	}
	
	try
	{		
		// �����һ����Ϣ
		for (g_itr1=g_vPasswdChangeStruct.begin(); g_itr1!=g_vPasswdChangeStruct.end(); ++g_itr1)
		{
			// ��detail�� - �ڶ���
			if ((iSize=m_poUnpack->getPasswdChange(*g_itr1,g_vPasswdChangeCond)) == 0 )
			{
				__DEBUG_LOG0_(0, "PasswdChange::deal ������ϸ��Ϣ��ΪNULL");
				m_poPack->setRltCode(ECODE_NOMSG);
				return -1;
			}
			else if (iSize < 0)
			{
				__DEBUG_LOG0_(0, "PasswdChange::deal ��Ϣ�������!");
				m_poPack->setRltCode(ECODE_UNPACK_FAIL);
				return -1;
			}
			 
			__DEBUG_LOG0_(0, "=========>2��_deal.�����Է���ƽ̨�������CCR��Ϣ.");			
			// ����
			if (_deal(*g_itr1,g_vPasswdChangeCond) != 0)
				break;
		}
		__DEBUG_LOG1_(0, "xxxxxx=PasswdChange::deal iRet=%d",iRet);
	}
	catch(TTStatus oSt)
	{
		iRet = -1;
		__DEBUG_LOG1_(0, "PasswdChange::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "PasswdChange::QryPayoutInfo value=%d",value);
	}
	
	// ҵ����ʧ�ܣ��򷵻ذ���TODO��
	if(iRet!=0)
	{
		m_poPack->clear();
		m_poPack->setRltCode(ECODE_TT_FAIL);
	}
	__DEBUG_LOG1_(0, "PasswdChange::deal iRet=%d",iRet);
	return iRet;
}

int PasswdChange::_deal(PasswdChangeStruct *oRes,vector<PasswdChangeCond *> g_vPasswdChangeCond)
{
	// ����--ȡ��һ����Ϣ
	__DEBUG_LOG0_(0, "=======================��½�����޸�=======================");
	__DEBUG_LOG0_(0, "=========>ȡ�Է���ƽ̨����CCR��һ�������Ϣ:");
	__DEBUG_LOG1_(0, "[��½�����޸�-�շ�CCR,PasswdChange],������ˮ��m_sReqID:%s",oRes->m_sReqID);
	__DEBUG_LOG1_(0, "[��½�����޸�-�շ�CCR,PasswdChange],����ʱ�䣺m_iReqTime:%d",oRes->m_iReqTime);
	
	//if(oRes->m_iReqTime > 0)
		//long2date(oRes->m_iReqTime,oRes->m_requestTime);
	//else
		//GetTime(oRes->m_requestTime);
	
	int iRet=0;
	int LoginServType; //�û����ͣ��������û� OR ͨ��֤�û�
	
	try
	{
		for (g_itr2=g_vPasswdChangeCond.begin(); g_itr2!=g_vPasswdChangeCond.end(); ++g_itr2)
		{
			__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,PasswdChange]:ȡ�Է���ƽ̨����CCR�ڶ���֧����Ϣ��");
			 LoginServType = (*g_itr2)->m_hDestType;
			__DEBUG_LOG1_(0, "[��½�����޸�-�շ�CCR,PasswdChange],�ж��û����ͣ�[%d](1-ͨ��֤�ʺ� 2-ҵ��ƽ̨�ʺ�)",LoginServType);
			
			switch(LoginServType)
			{
				case 2:
					//�������û�
					//����3.4.1.4HSS�����޸Ľӿ�
					__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,PasswdChange]:�������û�����HSS�����޸Ľӿ�");
					
					iRet = PasswdHSSChange(oRes,*g_itr2);
			 		if(iRet!=0)
			 		{
			 			throw -1;
			 			__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,PasswdChange]:��½�����Ȩ��ȫ������HSS�޸�ʧ��");
			 		}
			 		break;
				case 1:
					//ͨ��֤�û�
					//����3.4.3.1ͨ��֤�û������޸Ľӿ�
					__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,PasswdChange]:ͨ��֤�û�����ͨ��֤�û��޸Ľӿ�");
					//ͨ��֤�û������Ȩ����
					break;
			}
		}
	}
	catch(TTStatus oSt) 
	{
		iRet=-1;
		__DEBUG_LOG1_(0, "[��½�����޸�-�շ�CCR,PasswdChange] oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��½�����޸�-�շ�CCR,PasswdChange] iRet=%d",value);
	}
	
	return iRet;	
}

//�������û�����ȫ��HSS�������޸�
int PasswdChange::PasswdHSSChange(PasswdChangeStruct *oRes,PasswdChangeCond *g_itr2)
{
	int iRet=0;
	
	//��������CCR��Ϣ
	PasswdChangeStruct oReqPasswdStruct;
	PasswdChangeCond oReqPasswdChange={0};
	vector<PasswdChangeCond> vResPasswdChangeItem;
	
	//����CCR��Ϣ
	__DEBUG_LOG0_(0, "========================��������ABM��½�����޸�CCR��TT=======================");
	__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,�Է���ƽ̨]�Է�ƽ̨����CCR��ϸ:");
	__DEBUG_LOG1_(0, "������ˮ��m_sReqID:[%s]",oRes->m_sReqID);
	__DEBUG_LOG1_(0, "����ʱ�䣺m_iReqTime:[%d]",oRes->m_iReqTime);
	__DEBUG_LOG1_(0, "�û����룺m_sDestAccount:[%s]",g_itr2->m_sDestAccount);
	__DEBUG_LOG1_(0, "��ʶ���ͣ�m_hDestType:[%d]",g_itr2->m_hDestType);
	__DEBUG_LOG1_(0, "�û����ԣ�m_hDestAttr:[%d]",g_itr2->m_hDestAttr);
	__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��m_sServPlatID:[%s]",g_itr2->m_sServPlatID);
	__DEBUG_LOG1_(0, "��ǰ���룺m_sOldPasswd:[%s]",g_itr2->m_sOldPasswd);
	__DEBUG_LOG1_(0, "�������룺m_sNewPasswd:[%s]",g_itr2->m_sNewPasswd);
	
	struct struDccHead receiveDccHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	
	try
	{
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		
		// dcc����Ϣͷ��Ϣ
		__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,�Է���ƽ̨]:����CCR����Ϣͷ��Ϣ����TT");
		// ��ȡ�յ���CCR���е���Ϣͷ
		memcpy((void *)&receiveDccHead,(void *)(m_poUnpack->m_sDccBuf),sizeof(receiveDccHead));
		// ����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(receiveDccHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,�Է���ƽ̨]:����CCR����Ϣͷ��Ϣʧ��");
			throw -1;
		}
		
		// dcc��ҵ��������Ϣ
		__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,�Է���ƽ̨]:����CCR��ҵ����Ϣ����TT");
		iRet = m_dccOperation->savePlatformCCRforChange(receiveDccHead.m_sSessionId,oRes,g_itr2);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,����HSS��]:����CCRҵ����Ϣʧ��");
			throw -1;
		}
		
		// ��CCR������Ϣ
		__DEBUG_LOG0_(0, "========================���½�����޸�CCR������Ϣ=======================");
		
		//����׼��
		//��һ����Ϣ
		GetTime(oReqPasswdStruct.m_requestTime);		//����ʱ��
		time_t sTime;
		sTime = time(NULL);
		oReqPasswdStruct.m_iReqTime=sTime;
		
		strcpy(oReqPasswdStruct.m_sReqID,oRes->m_sReqID);	//������ˮ
		
		//�ڶ�����ϸ
		strcpy(oReqPasswdChange.m_sDestAccount,g_itr2->m_sDestAccount);	//�û�����
		oReqPasswdChange.m_hDestType = g_itr2->m_hDestType;		//�û�����
		oReqPasswdChange.m_hDestAttr = g_itr2->m_hDestAttr;	//�û�����
		strcpy(oReqPasswdChange.m_sServPlatID,g_itr2->m_sServPlatID);	//ҵ��ƽ̨��ʶ
		strcpy(oReqPasswdChange.m_sOldPasswd,g_itr2->m_sOldPasswd);	//��ǰ����
		strcpy(oReqPasswdChange.m_sNewPasswd,g_itr2->m_sNewPasswd);	//��������
		
		__DEBUG_LOG1_(0, "������ˮ��PasswdChangeStruct.m_sReqID:[%s]",oReqPasswdStruct.m_sReqID);
		__DEBUG_LOG1_(0, "����ʱ�䣺PasswdChangeStruct.m_requestTime:[%s]",oReqPasswdStruct.m_requestTime);
		__DEBUG_LOG1_(0, "�û����룺oReqPasswdChange.m_sDestAccount:[%s]",oReqPasswdChange.m_sDestAccount);
		__DEBUG_LOG1_(0, "�û����ͣ�oReqPasswdChange.m_hDestType:[%d]",oReqPasswdChange.m_hDestType);
		__DEBUG_LOG1_(0, "�û����ԣ�oReqPasswdChange.m_hDestAttr:[%d]",oReqPasswdChange.m_hDestAttr);
		__DEBUG_LOG1_(0, "ҵ��ƽ̨��ʶ��oReqPasswdChange.m_sServPlatID:[%s]",oReqPasswdChange.m_sServPlatID);
		__DEBUG_LOG1_(0, "��½���룺oReqPasswdChange.m_sOldPasswd:[%s]",oReqPasswdChange.m_sOldPasswd);
		__DEBUG_LOG1_(0, "��½���룺oReqPasswdChange.m_sNewPasswd:[%s]",oReqPasswdChange.m_sNewPasswd);
		
		vResPasswdChangeItem.push_back(oReqPasswdChange);
		
		//������Ϣͷ
		// ����Session-id
		long lSessionIdSeq;
		iRet = m_dccOperation->QueryPaySeq(lSessionIdSeq);
		if(iRet !=0)
		{
			__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,����HSS��]:��ѯDCC�Ự��ʶSession-Idʧ��");
			throw -1;
		}
		
		struct struDccHead Head;
		char sSessionId[64];
		time_t tTime;
		tTime = time(NULL);
		
		// 2011.8.1
		snprintf(Head.m_sSessionId,sizeof(Head.m_sSessionId),"%s%010d%s%04d\0","ABM@001.ChinaTelecom.com;",tTime,";",lSessionIdSeq);
		__DEBUG_LOG1_(0, "SESSION_ID:[%s]",Head.m_sSessionId);
	
		strcpy(Head.m_sSrvTextId,"LogonPassword_Reset.Micropayment@001.ChinaTelecom.com");
		strcpy(Head.m_sOutPlatform,"001.ChinaTelecom.com");
		strcpy(Head.m_sOrignHost,"ABM@001.ChinaTelecom.com"); // 2011.8.1
		strcpy(Head.m_sOrignRealm,"001.ChinaTelecom.com");
		strcpy(Head.m_sDestRealm,"001.ChinaTelecom.com");
/*	
		char sOrgId[4];
		
		char sAccoutNbr[32];
		strcpy(sAccoutNbr,"18951765356");
		//iRet = m_dccOperation->qryAccountOrgId(reqPayInfo.m_iDestinationAttr,sOrgId); 

		if(iRet !=0 )
		{
			__DEBUG_LOG0_(0, "error");
			throw -1;
		}
		
		snprintf(Head.m_sDestRealm,sizeof(Head.m_sDestRealm),"%s.ChinaTelecom.com",sOrgId);
		__DEBUG_LOG1_(0, "Head.m_sDestRealm:[%s]",Head.m_sDestRealm);
*/	
		Head.m_iAuthAppId = 4;
		Head.m_iReqType = 4;
		Head.m_iReqNumber  = 0;
		Head.m_iResultCode = 0;
		
		// ����CCR��Ϣͷ��ֵ
		memcpy((void *)(m_poPasswdPack->m_sDccBuf),(void *)&Head,sizeof(Head));
		
		m_poPasswdPack->m_iCmdID = ABMCMD_PASSWD_CHANGE_CCA;
		
		//�����һ��CCR
		if (!m_poPasswdPack->addPasswdChangeRespons(oReqPasswdStruct))
		{
			__DEBUG_LOG0_(0, "PasswdChange::PasswdHSSChange ��һ��CCR���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			return -1;
		}
		__DEBUG_LOG1_(0, "m_poPasswdPack->m_iCmdID=%d",m_poPasswdPack->m_iCmdID);
		
		__DEBUG_LOG0_(0, "��һ�����CCA������");
		
		// ��� - �ڶ���۷���ϸ
		for(int j=0;j<vResPasswdChangeItem.size();j++)
		{
			if (!m_poPasswdPack->addPasswdChange(vResPasswdChangeItem[j]))
			{
				__DEBUG_LOG0_(0, "PasswdChange:: PasswdHSSChange m_poPack ���ʧ��!");
				m_poPack->setRltCode(ECODE_PACK_FAIL);
				throw -1;
			}	
		}
		
		__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,����HSS��]:CCA������");
		
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		// ���淢����CCR��Ϣ
		__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,����HSS��]:���淢����DCC����Ϣ��TT");
		iRet = m_dccOperation->insertDccInfo(Head,dccType,lDccRecordSeq,receiveDccHead.m_sSessionId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��½�����޸�-�շ�CCR,����HSS��]:����DCCҵ����Ϣʧ��");
			throw -1;
		}
	}
	catch(TTStatus oSt)
	{
		iRet = -1;
		__DEBUG_LOG1_(0, "[��½�����޸�-�շ�CCR,����HSS��]: oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��½�����޸�-�շ�CCR,����HSS��]: iRet=%d",value);
	}
	__DEBUG_LOG1_(0, "[��½�����޸�-�շ�CCR,����HSS��]-ActiveAllocateBalance::deductBalanceOut,iRet=:[%d]",iRet);
	
	return iRet;
}