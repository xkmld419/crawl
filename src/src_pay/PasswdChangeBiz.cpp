#include "PasswdChangeBiz.h"
#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "PublicConndb.h"

static vector<PasswdChangeData *> g_vPasswdChangeData;    // HSS�˷���CCA 
static vector<PasswdChangeData *>::iterator g_itr1;
	
static char cPreSessionId[100]={0};

PasswdChangeBiz::PasswdChangeBiz()
{
		m_poUnpack = NULL;
		m_poPack = NULL;		
		m_dccOperation = new DccOperation;	
}

PasswdChangeBiz::~PasswdChangeBiz()
{
	if (m_dccOperation != NULL)
		delete m_dccOperation;
}

int PasswdChangeBiz::deal(ABMCCR *pRecivePack, ABMCCA *pSendPack)
{
	__DEBUG_LOG0_(0, "=========>PasswdChangeBiz::deal ��Ӧ����ʼ");
	__DEBUG_LOG0_(0, "=========>1������ȫ��HSS���ص�CCA...");
	
	PasswdChangeStruct reqStruct;
	PasswdChangeCond reqCond;
	
	//PasswdChangeCond oCCAData;	//����ABM��Ӧ�Է���ƽ̨��CCA������Ϣ
	
	int iRet=0;
	int iSize=0;
	int LoginServType; //�û����ͣ��������û� OR ͨ��֤�û�
	
	// ��ȡpRecivePack�Ĺ���CCA��Ϣͷ
	char sSessionId[64];
	struDccHead Head;

	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"A"); 
	long lDccRecordSeq=0L; // ��ˮ�ţ�����
	
	try
	{
		// ȡ��Ϣ������Ϣͷ��Ϣ
		memcpy((void *)&Head,(void *)(pRecivePack->m_sDccBuf),sizeof(Head));
		// �յ�CCA��Session-Id
		__DEBUG_LOG1_(0, "=======>�յ�ȫ��HSS���ص�CCA����Ϣͷ��SESSION_ID:[%s]",Head.m_sSessionId);
		//����CCA��Ӧ��ˮȥ���Է���ƽ̨����CCR�ĻỰID
		iRet = m_dccOperation->qryPreSessionId(Head.m_sSessionId,sSessionId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "��ѯCCA�ỰID��ԴSession Idʧ��.");
		}
		__DEBUG_LOG1_(0, "========>�Է���ƽ̨����CCR��SESSION_ID:[%s]",sSessionId);
		strcpy(cPreSessionId,sSessionId);
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "��ѯDCC_INFO_RECORD_SEQʧ��");
			throw -1;
		}
		// ���յ���CCA��Ϣͷ��TT
		iRet = m_dccOperation->insertDccInfo(Head,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "����ȫ��HSS����CCA����Ϣͷ��Ϣʧ��");
			throw -1;
		}
		// ����sessionIdȡTT��Ӧ��CCR��Ϣ
		__DEBUG_LOG0_(0, "=========>����sessionIdȡTT��Ӧ��CCR[�Է���ƽ̨CCR������Ϣ]��Ϣ:");
	
		iRet = m_dccOperation->getPasswdChangeInfo(sSessionId,reqStruct,reqCond);  //�½�����
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "��ȡ�Է���ƽ̨CCR����ҵ����Ϣʧ��");
			throw -1;
		}
		__DEBUG_LOG1_(0, "getCCRInfo,iRet=[%d]",iRet);
		
		//�û����ͣ��������û�ORͨ��֤�û�
		LoginServType=reqCond.m_hDestType;
		
		//���
		m_poPack = (PasswdChangeCCA *)pSendPack;
		m_poUnpack = (PasswdChangeCCA *)pRecivePack;
		
		if(iSize=m_poUnpack->getPasswdChangeData(g_vPasswdChangeData) == 0)
			{
				__DEBUG_LOG0_(0, "PasswdChangeBiz::deal ȫ��HSS����CCA�����������Ϣ��ΪNULL");
				m_poPack->setRltCode(ECODE_NOMSG);
				throw  -1;
			}
			else if(iSize < 0)
				{
					__DEBUG_LOG0_(0, "PasswdChangeBiz::deal ��Ϣ�������!");
					m_poPack->setRltCode(ECODE_UNPACK_FAIL);
					throw -1;
				}
			
		//CCA�������
		for(g_itr1=g_vPasswdChangeData.begin();g_itr1!=g_vPasswdChangeData.end();++g_itr1)
		{
			iRet=_deal(reqStruct,*g_itr1);
			__DEBUG_LOG1_(0, "PasswdChangeBiz::deal iRet=%d", iRet);
		}
	}
	catch(TTStatus oSt)
	{
		iRet = -1;
		__DEBUG_LOG1_(0, "PasswdChangeBiz::deal oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "PasswdChangeBiz::QryPayoutInfo value=%d",value);
	}
	
	if(iRet != 0)
	{
		// �򷵻ذ�����������
		m_poPack->clear();
		m_poPack->setRltCode(ECODE_TT_FAIL);
	}
	
	return iRet;	
}

int PasswdChangeBiz::_deal(PasswdChangeStruct oRes,PasswdChangeData *pData)
{
	int iRet=0;
	PasswdChangeData oCCAData;	//����ABM��Ӧ�Է���ƽ̨��CCA������Ϣ
	char sSessionId[64];
	struDccHead Head;

	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"A"); 
	long lDccRecordSeq=0L; // ��ˮ�ţ�����
	
	//CCA�������
	
	try{
		//���ɷ����Է���ƽ̨��CCA��
		strcpy(oCCAData.m_sResID,oRes.m_sReqID);
		oCCAData.m_hRltCode = pData->m_hRltCode;
		strcpy(oCCAData.m_sParaRes,pData->m_sParaRes);
		
		time_t tTime;
		tTime = time(NULL);
		oCCAData.m_iResTime=tTime;
					
		//������Ϣͷ
		// Session-id - �����Է���ƽ̨��Session��Ҫ����������ˮ�Ŵ�DCCҵ����Ϣ��¼���л�ȡ
		strcpy(Head.m_sSessionId,cPreSessionId);
		__DEBUG_LOG1_(0, "����CCA��SESSION_ID:[%s]",Head.m_sSessionId);
		
		strcpy(Head.m_sSrvTextId,"LogonPassword_Reset.Micropayment@001.ChinaTelecom.com");
		strcpy(Head.m_sOutPlatform,"001.ChinaTelecom.com");
		strcpy(Head.m_sOrignHost,"ABM2@001.ChinaTelecom.com");
		strcpy(Head.m_sOrignRealm,"001.ChinaTelecom.com");
		
		// ��ȡ���󻮲�����Ĺ�����
		char sOrgId[4];
		char sLocalAreaId[5]={0};
		char sAccoutNbr[32];
		strcpy(sAccoutNbr,"18951765356");
		//iRet = m_dccOperation->qryAccountOrgId(reqPayInfo.m_iDestinationAttr,sOrgId); // ���Ժ�ſ�
		iRet = m_dccOperation->qryAccountOrgId(sAccoutNbr,sOrgId,sLocalAreaId); // ����ʱд��
		if(iRet !=0 )
		{
			__DEBUG_LOG0_(0, "ȡ����������������ʧ��.");
			throw -1;
		}
		
		snprintf(Head.m_sDestRealm,sizeof(Head.m_sDestRealm),"%s.ChinaTelecom.com",sOrgId);
		__DEBUG_LOG1_(0, "��Ϣͷ��Ŀ���豸��ʶ-Head.m_sDestRealm:[%s]",Head.m_sDestRealm);
		
		Head.m_iAuthAppId = 4;
		Head.m_iReqType = 4;
		Head.m_iReqNumber  = 0;
		Head.m_iResultCode = 0;
		
		memcpy((void *)(m_poPack->m_sDccBuf),(void *)&Head,sizeof(Head));
		
		//�����Է���ƽ̨CCA���
		if(!m_poPack->addPasswdChangeData(oCCAData))
		{
			__DEBUG_LOG0_(0, "PasswdChangeBiz::deal �����Է���ƽ̨CCA���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw -1;
		}
		__DEBUG_LOG0_(0, "�����Է���ƽ̨CCA������");
		
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		if(iRet !=0 )
		{
			__DEBUG_LOG0_(0, "ȡDCC_INFO_RECORD_SEQʧ��.");
			throw -1;
		}
		
		// ���淢����CCA��Ϣ
		__DEBUG_LOG0_(0, "���淢����DCC����Ϣ��TT");
		iRet = m_dccOperation->insertDccInfo(Head,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "����DCCҵ����Ϣʧ��");
			throw -1;
		}
	}
	catch(TTStatus oSt)
	{
		iRet = -1;
		__DEBUG_LOG1_(0, "[��½�����޸�-�շ�CCA,�����Է����]: oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "[��½�����޸�-�շ�CCA,�����Է����]: iRet=%d",value);
	}
	return iRet;
}