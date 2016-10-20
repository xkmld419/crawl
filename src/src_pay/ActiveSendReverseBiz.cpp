//ActiveSendReverseBiz.cpp
#include "ActiveSendReverseBiz.h"
#include "ActiveSendReverseBizSql.h"
#include "ABMException.h"
#include "abmcmd.h"
#include "LogV2.h"
#include "PublicConndb.h"
#include "DccOperation.h"

static ABMException oExp;
class ActiveSendReverseBizSql;

bool g_flag = true;

ActiveSendReverseBiz::ActiveSendReverseBiz()
{
	m_pPublicConndb=NULL;
	m_pActiveReverseSql = NULL;
	m_dccOperation = NULL;
}

ActiveSendReverseBiz:: ~ActiveSendReverseBiz()
{
	FreeObject(m_pPublicConndb);
	FreeObject(m_pActiveReverseSql);
	FreeObject(m_dccOperation);
}

int ActiveSendReverseBiz::init(ABMException &oExp)
{
	if (m_pPublicConndb->init(oExp) != 0) 
	{
		ADD_EXCEPT0(oExp, "ActiveSendReverseBiz::init m_pPublicConndb->init failed!");
		return -1;
	}
	
	if ((m_pActiveReverseSql = new ActiveSendReverseBizSql) == NULL)
	{
		ADD_EXCEPT0(oExp, "ActiveSendReverseBiz::init malloc failed!");
		return -1;
	}
	
	
	if ((m_dccOperation = new DccOperation) == NULL)
	{
		ADD_EXCEPT0(oExp, "ActiveSendReverseBiz::init malloc failed!");
		return -1;
	}
	
	return 0;
}

int ActiveSendReverseBiz::deal(ABMCCR *pSendCCR)
{
	ABMException oExp;
	int iRet=0;
	vector<ActiveSendLog> vActiveSendLog;
	
	char sSwitchId[5]={0};
	char sSwitchValue[2]={0};
	long lDelaySecond=0L;
	strcpy(sSwitchId,"1001");
	// ȡϵͳ����,SYS_SWITCH:1001,���Ϊ1,��ѯ���������ļ�¼
	iRet = m_pActiveReverseSql->getSystemSwitch(sSwitchId,sSwitchValue);
	
	//__DEBUG_LOG1_(0, "ϵͳ����1001,valuse:[%s]",sSwitchValue);	
	
	if(strcmp(sSwitchValue,"1") == 0)
	{
		// ȡ����ʱ���ӳٷ�������
		iRet = m_pActiveReverseSql->getDelaySecond(lDelaySecond);
		
		//__DEBUG_LOG1_(0, "���õ��ӳٷ��͵�����:[%ld]",lDelaySecond);	
		
		// ɨ������������CCR��־��¼��
		iRet = m_pActiveReverseSql->qryActiveSendLog(vActiveSendLog,lDelaySecond,oExp);
	}
	else
	{
		// 1001ϵͳ����ֵΪ0��ȡ���м�¼,ɨ������������CCR��־��¼��
		iRet = m_pActiveReverseSql->qryActiveSendLog(vActiveSendLog,oExp);
	}
	//__DEBUG_LOG1_(0, "ɨ����������CCR��Ϣ��־,��¼��:[%d]",vActiveSendLog.size());	
	//__DEBUG_LOG0_(0, "ɨ����������CCR��Ϣ��־");
	
	if(vActiveSendLog.size() > 0)
	{
		ReverseDeductBalanceCCR *pSendPack = new ReverseDeductBalanceCCR;
		pSendPack = (ReverseDeductBalanceCCR *)pSendCCR;
		for(int i=0;i<vActiveSendLog.size();i++)
		{	 
			pSendPack->clear();
			
			ReqReverseDeductSvc reverseDeductSvc;
			// ��ֵ-��װCCR��-ҵ����Ϣ
			// ����������ˮ
			strcpy(reverseDeductSvc.m_sRequestId,"20110716"); // ������ˮ
			
			// ��ǰʱ��,ת������
			time_t tTime; 
			tTime = time(NULL);
			char sCurDate[32]={0};
			long2date(tTime+2209017599,sCurDate);
			
			snprintf(reverseDeductSvc.m_sRequestId,sizeof(reverseDeductSvc.m_sRequestId),"002%s%08d00\0",sCurDate,1);
			reverseDeductSvc.m_iRequestTime = tTime;// ����ʱ��
			
			strcpy(reverseDeductSvc.m_sOldRequestId,vActiveSendLog[i].sRequestId);// �������Ļ�����ˮ��
			strcpy(reverseDeductSvc.m_sABMId,"001"); // �������ABM��ʶ,����ABM
			if(strcmp(vActiveSendLog[i].sRequestType,"001") == 0)
			{
				reverseDeductSvc.m_iActionType = 0;	// ������������,0:�������,1:��������
			}
			else if(strcmp(vActiveSendLog[i].sRequestType,"002") == 0)
			{
				reverseDeductSvc.m_iActionType = 1;
			}	
			
			// ��ѯ��sOrgId
			char sOrgId[5]={0};
			strcpy(sOrgId,vActiveSendLog[i].sOrgId);
			
			__DEBUG_LOG1_(0, "��������CCR,������ˮ,m_sRequestId:[%s]",reverseDeductSvc.m_sRequestId);
			__DEBUG_LOG1_(0, "��������CCR,����ʱ��,m_iRequestTime:[%d]",reverseDeductSvc.m_iRequestTime);
			__DEBUG_LOG1_(0, "��������CCR,��������ˮ��,m_sOldRequestId:[%s]",reverseDeductSvc.m_sOldRequestId);
			__DEBUG_LOG1_(0, "��������CCR,�������ABM��ʶ,m_sABMId:[%s]",reverseDeductSvc.m_sABMId);
			__DEBUG_LOG1_(0, "��������CCR,��������,m_iActionType:[%d]",reverseDeductSvc.m_iActionType);
			__DEBUG_LOG1_(0, "��������CCR,��������,sOrgId:[%s]",sOrgId);
			__DEBUG_LOG0_(0, "=====================================");
			
			// CCR
			// ��CCR����Ϣͷ��Ϣ
			// ����CCR��Ϣͷ
			// DeductFromABM. Micropayment@<DomainName>.ChinaTelecom.com
			// ����Session-id
			long lSessionIdSeq;
			iRet = m_dccOperation->QueryPaySeq(lSessionIdSeq);
			if(iRet !=0)
			{
				__DEBUG_LOG0_(0, "��ѯDCC�Ự��ʶSession-Idʧ��");
				throw QRY_SESSION_ID_ERR;
			}
			struct struDccHead Head;
			char sSessionId[64]={0};
			
			//time_t tTime;
			tTime = time(NULL);
			// 2011.8.1
			snprintf(Head.m_sSessionId,sizeof(Head.m_sSessionId),"%s%010d%s%04d\0","ABM@001.ChinaTelecom.com;",tTime,";",lSessionIdSeq);
			strcpy(Head.m_sOutPlatform,""); // �ⲿƽ̨��SGWʹ��
			strcpy(Head.m_sOrignHost,"ABM@001.ChinaTelecom.com"); // 2011.8.1
			strcpy(Head.m_sOrignRealm,"001.ChinaTelecom.com");
			
			snprintf(Head.m_sDestRealm,sizeof(Head.m_sDestRealm),"%s.ChinaTelecom.com",sOrgId);
	
			snprintf(Head.m_sSrvTextId,sizeof(Head.m_sSrvTextId),"ReverseChargeFromABM.Micropayment@%s.ChinaTelecom.com",sOrgId);
			__DEBUG_LOG1_(0, "��Ϣͷ��Ŀ���豸��ʶ-Head.m_sDestRealm:[%s]",Head.m_sDestRealm);
			
			Head.m_iAuthAppId = 4;
			Head.m_iReqType = 4;
			Head.m_iReqNumber  = 0;
			Head.m_iResultCode = 0;
			strcpy(Head.m_sSrvFlowId,""); // ������ȡԴm_sSrvFlowId��׷��
			//snprintf(Head.m_sSrvFlowId,sizeof(Head.m_sSrvFlowId),"%s%s","-","-");
			memcpy((void *)(pSendPack->m_sDccBuf),(void *)&Head,sizeof(Head));
			
			//pSendPack->addReqReverseDeductSvc(reverseDeductSvc);
			
			// �������
			if (!pSendPack->addReqReverseDeductSvc(reverseDeductSvc) )
			{
				__DEBUG_LOG0_(0, "ActiveSendReverseBiz::deal ����������һ��CCR���ʧ��!");
				//pSendPack->setRltCode(ECODE_PACK_FAIL);
				//throw ACTIVE_PACK_CCR_OUT_ERR;
			}
			
			__DEBUG_LOG0_(0, "[������������]:��һ�����CCR������");
			
			pSendPack->m_iCmdID = ABMCMD_REVERSE_DEDUCT_BALANCE;
			// ����CCR��¼״̬
			
			iRet = m_pActiveReverseSql->updateCCRInfo(reverseDeductSvc.m_sOldRequestId);
			if(iRet != 0)
			{
				__DEBUG_LOG0_(0, "[������������]:����CCR��¼ʧ��");
			}
			
			break;
		}
		
		delete pSendPack;
		pSendPack = NULL;
	}

	// �ύ
	if(iRet==0)
	{
		//__DEBUG_LOG0_(0, "ActiveSendReverseBiz->deal  ---0");
		m_pPublicConndb->Commit(oExp);
	}
	else
	{
		//__DEBUG_LOG0_(0, "ActiveSendReverseBiz->deal  ---1");
		//m_pPublicConndb->RollBack(oExp);
	}
	
	return iRet;
}













