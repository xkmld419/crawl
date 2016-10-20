#include "ReverseDepositInfo.h"
#include "PaymentInfoSql.h"
#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"


static vector<ReverseDeposit *> g_vServInfo;
static vector<ReverseDeposit *>::iterator g_itrInfo;

ReverseDepositInfo::ReverseDepositInfo()
{
    m_poSql = new PaymentInfoSql;
    pAllocateBalanceSql =new AllocateBalanceSql;
    m_poUnpack = NULL;
    m_poPack = NULL;
    m_dccOperation = new DccOperation;	
}

ReverseDepositInfo:: ~ReverseDepositInfo()
{
    FreeObject(m_poSql);
    FreeObject(pAllocateBalanceSql);
    FreeObject(m_dccOperation );
}

int ReverseDepositInfo::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
    int iRet=0;
    
    m_poUnpack = (ReverseDepositInfoCCR *)pCCR;
    m_poPack = (ReverseDepositInfoCCA *)pCCA;
    m_poPack->clear();
    g_vServInfo.clear();
    ResponsReverseDeposit oRes={0};
    try 
    {
    
	    __DEBUG_LOG0_(0, "��ֵ�����������Ϣ��");
	    //��Ϣ���
	    if ((iRet=m_poUnpack->getReverseDepositInfo(g_vServInfo)) == 0)
	    {
		__DEBUG_LOG0_(0, "��ֵ��������� ��Ϣ��ΪNULL");
		m_poPack->setRltCode(ECODE_NOMSG);
		throw REVERSE_DEP_UNPACK_ERR;
	    }
	    else if (iRet < 0) 
	    {
		__DEBUG_LOG0_(0, "��ֵ������ ��Ϣ�������!");
		m_poPack->setRltCode(ECODE_UNPACK_FAIL);
		throw REVERSE_DEP_UNPACK_ERR;
	    }
	    for(unsigned  int i=0;i<g_vServInfo.size();i++)
	    {
		    __DEBUG_LOG2_(0,"��ֵ�������:[%d].m_sReqSerial=%s",i,g_vServInfo[i]->m_sReqSerial );
		    __DEBUG_LOG2_(0,"��ֵ�������:[%d].m_sPaymentID=%s",i, g_vServInfo[i]->m_sPaymentID);
		    __DEBUG_LOG2_(0,"��ֵ�������:[%d].m_sDestinationType=%s",i, g_vServInfo[i]->m_sDestinationType);
		    __DEBUG_LOG2_(0,"��ֵ�������:[%d].m_sDestinationAccount=%s",i,g_vServInfo[i]->m_sDestinationAccount );
		    __DEBUG_LOG2_(0,"��ֵ�������:[%d].m_iDestinationAttr=%d",i, g_vServInfo[i]->m_iDestinationAttr);
	    }

	    for(g_itrInfo=g_vServInfo.begin();g_itrInfo!=g_vServInfo.end();g_itrInfo++)
	    {
			iRet=_deal(*g_itrInfo);
			if(iRet!=0)
				throw iRet;
	    }
	    __DEBUG_LOG0_(0, "��ֵ����: _deal  ���!");
	    oRes.m_iRltCode=0;
	    strcpy(oRes.m_sReqSerial,g_vServInfo[0]->m_sReqSerial);
	    if (!m_poPack->addRespReverseDepositCCA(oRes)) 
	    {
		__DEBUG_LOG0_(0, "��ֵ����:: _deal m_poPack ���ʧ��!");
		m_poPack->setRltCode(ECODE_PACK_FAIL);
		throw REVERSE_DEP_PACK_ERR;
	    }

    }
    catch(TTStatus oSt) 
    {
	iRet=REVERSE_DEP_TT_ERR;
        __DEBUG_LOG1_(0, "��ֵ����ʧ��::deal oSt.err_msg=%s", oSt.err_msg);
    }
    catch(int &value)
    {
	iRet=value;
        __DEBUG_LOG1_(0, "��ֵ����ʧ��::deal value=%d", value);
	
    }
    if(iRet!=0)
    {
	     __DEBUG_LOG0_(0, "��ֵ����ʧ��,��ʧ�ܰ�!");
	    oRes.m_iRltCode=iRet;
	    strcpy(oRes.m_sReqSerial,g_vServInfo[0]->m_sReqSerial);
	    if (!m_poPack->addRespReverseDepositCCA(oRes)) 
	    {
		__DEBUG_LOG0_(0, "��ֵ����ʧ��:: _deal m_poPack ���ʧ��!");
		m_poPack->setRltCode(ECODE_PACK_FAIL);
	    }
    }
    return iRet;
}

int ReverseDepositInfo::_deal(ReverseDeposit *&pInOne)
{
	//��ֵ������
	//1�������ϣ�
	//
	int iRet=0;
	ServAcctInfo oServAcct={0};
	
	ABMException oExp;
	struDccHead ccaHead={0};
	char dccType[2]="R";
	long lRecordIdSeq=0;
	try 
	{
	
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
			__DEBUG_LOG1_(0, "��ֵ��������DCCͷ��=[%d]",iRet);
			throw iRet;
		}
		//��ѯ�û���Ϣ
		iRet=pAllocateBalanceSql->preQryServAcctInfo(pInOne->m_sDestinationAccount,2,oServAcct,oExp);
		switch(iRet)
		{
		
			case 0://������
				{
					__DEBUG_LOG0_(0, "��ֵ�������ҵ��û�����");
					iRet=DoReverseDeposit(pInOne,oServAcct);
					if(iRet!=0)
					{
						__DEBUG_LOG1_(0, "�����û����ϣ���ֵ����  ʧ��==[%d]",iRet);
						throw iRet;
					}
				}
				break;
			default://����
				{
					__DEBUG_LOG1_(0, "��ֵ��������ѯ�û����ϳ���=[%d]",iRet);
					throw iRet;
				}
		}
	
		return 0;

		
	}
	catch(TTStatus oSt) 
	{
		iRet=REVERSE_DEP_TT_ERR;
		__DEBUG_LOG1_(0, "��ֵ����ʧ�ܣ�������Ϣ=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "��ֵ����ʧ�ܣ�������Ϣ=%d",value);
	}
	

	return iRet;
} 

int ReverseDepositInfo::DoReverseDeposit(ReverseDeposit *&pInOne,ServAcctInfo &oServAcct)
{
	//2����¼ҵ�������ͬʱ����Դ��¼״̬��ʱ��
	//3����ѯ�����Դ���жϳ�ֵ���������Ƿ��б仯��
	//4�����������Դ���Ϊ0��ʱ�䣬
	//5����������˱�״̬����
	//6����¼���֧����
	//7����¼��Դ֧�ֹ�ϵ��
	int iRet=0;
	bool bState=false;
	string m_sOperType;
	StructPaymentSeqInfo sStructPaymentSeqInfo={0};
	AcctBalanceInfo *pAcctBalanceInfo=NULL;
	vector<BalanceSourceInfo *>vBalanceSourceInfo;
	vBalanceSourceInfo.clear();
	PaymentInfoALLCCR pPaymentInfoCCR={0};
	long lReversePaymentID=0L,lPaymentID=0L;
	try
	{
		iRet=m_poSql->QueryOptID(pInOne->m_sPaymentID,lPaymentID);
		if(iRet!=1)
		{
			__DEBUG_LOG0_(0, "��ѯԭҵ���¼opt_idʧ��!");
			throw REVERSE_DEP_QRY_OPTID_ERR;
		}
		iRet=m_poSql->QryPaymentInfo(lPaymentID,pPaymentInfoCCR);
		if(iRet!=1)
		{
			__DEBUG_LOG0_(0, "��ѯԭҵ���¼ʧ��!");
			throw REVERSE_DEP_QRY_DATA_ERR;
		}

		iRet=m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bState);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "��������ѯ����ʧ��  =[%d]",iRet);
			throw iRet;
		}
		lReversePaymentID=sStructPaymentSeqInfo.m_lPaymentSeq;

		//��¼ҵ���
		memset(pPaymentInfoCCR.m_sOperateSeq,'\0',sizeof(pPaymentInfoCCR.m_sOperateSeq));
		strcpy(pPaymentInfoCCR.m_sOperateSeq,pInOne->m_sReqSerial);

		memset(pPaymentInfoCCR.m_sCapabilityCode,'\0',sizeof(pPaymentInfoCCR.m_sCapabilityCode));
		strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0017");

		sStructPaymentSeqInfo.m_lRbkPaymentSeq=lPaymentID;

		memset(pPaymentInfoCCR.m_sDescription,'\0',sizeof(pPaymentInfoCCR.m_sDescription));
		strcpy(pPaymentInfoCCR.m_sDescription,"��ֵ����");

		memset(pPaymentInfoCCR.m_sOrderState,'\0',sizeof(pPaymentInfoCCR.m_sOrderState));
		strcpy(pPaymentInfoCCR.m_sOrderState,"C0F");
		//��¼F����ҵ����ˮ
		iRet=m_poSql->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
		if(iRet!=0)
		{
			__DEBUG_LOG0_(0, "��¼ҵ���¼ʧ��!");
			throw iRet;
		}

		//����ҵ����ˮ
		iRet=m_poSql->updateOperation(lPaymentID);
		if(iRet!=0)
		{
			__DEBUG_LOG0_(0, "����ԭҵ���¼ʧ��!");
			throw iRet;
		}

		//��ѯ�����Դ	
		iRet=m_poSql->qryBalanceSource(lPaymentID,vBalanceSourceInfo);
		if(iRet!=0)
		{
			__DEBUG_LOG0_(0, "��ѯ�����Դ��¼ʧ��!");
			throw iRet;
		}

		//δ�ҵ����������������Դ��¼
		if(vBalanceSourceInfo.size()<=0)
		{
			__DEBUG_LOG0_(0, "δ�ҵ����������������Դ��¼!");
			throw -1;
		}
		for(vector<BalanceSourceInfo *>::iterator it=vBalanceSourceInfo.begin();it!=vBalanceSourceInfo.end();++it)
		{
			memset(&sStructPaymentSeqInfo,0,sizeof(StructPaymentSeqInfo));
			if((*it)->m_lAmount!=(*it)->m_lBalance)
			{
				__DEBUG_LOG0_(0, "��ֵ���������Դ����ѷ����仯!");
				throw REVERSE_DEP_BALANCE_CHANGE_ERR;
			}

			//���������Դ���Ϊ0
			sStructPaymentSeqInfo.m_lPaymentAmount=(*it)->m_lBalance;
			iRet=m_poSql->updateBalanceSource(sStructPaymentSeqInfo,*it);
			if(iRet!=0)
			{
				__DEBUG_LOG1_(0, "��ֵ���������¸��������Դʧ��[%d]!",iRet);
				throw iRet;
			}

			pAcctBalanceInfo =new AcctBalanceInfo;
			iRet=m_poSql->qryAcctBalance((*it)->m_lAcctBalanceId,pAcctBalanceInfo);
			if(iRet!=1)
			{
				__DEBUG_LOG2_(0, "��ֵ��������������˱�ʧ��  =[%d],acct_balance_id=[%d]",iRet,(*it)->m_lAcctBalanceId);
				throw iRet;
			}

			pAcctBalanceInfo->m_lBalance=(-1)*((*it)->m_lBalance);

			__DEBUG_LOG2_(0, "��ֵ�����������û�����˱����  =[%ld],[%ld]",pAcctBalanceInfo->m_lBalance,(*it)->m_lBalance);
			iRet=m_poSql->updateAcctBalance((*it)->m_lAcctBalanceId,pAcctBalanceInfo->m_lBalance);
			if(iRet!=0)
			{
				__DEBUG_LOG1_(0, "��ֵ�����������û�����˱�ʧ��  =[%d]",iRet);
				throw iRet;
			}
			bState=true;
			iRet=m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bState);
			if(iRet!=0)
			{
				__DEBUG_LOG1_(0, "��ֵ��������ѯ����ʧ��  =[%d]",iRet);
				throw iRet;
			}
			sStructPaymentSeqInfo.m_lPaymentSeq=lReversePaymentID;
			//��¼֧����
			m_sOperType.clear();
			m_sOperType="5UH";
			iRet=m_poSql->InsertBalancePayout(sStructPaymentSeqInfo,*it,m_sOperType);
			if(iRet!=0)
			{
				__DEBUG_LOG1_(0, "��ֵ��������¼���֧��ʧ��  =[%d]",iRet);
				throw iRet;
			}
			//��¼��Դ֧����ϵ��
			iRet=m_poSql->InsertBalanceSourcePayoutRela(sStructPaymentSeqInfo,*it);
			if(iRet!=0)
			{
				__DEBUG_LOG1_(0, "��ֵ��������¼�����Դ֧����ϵʧ��  =[%d]",iRet);
				throw iRet;
			}
			FreeObject(pAcctBalanceInfo);

		}
		FreeVector(vBalanceSourceInfo);
		
		return 0;
	}
	catch(TTStatus oSt) 
	{
		iRet=REVERSE_DEP_TT_ERR;
		__DEBUG_LOG1_(0, "��ֵ����ʧ��,������Ϣ=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "��ֵ����ʧ��,������Ϣ=%d",value);
	}
	FreeObject(pAcctBalanceInfo);
	FreeVector(vBalanceSourceInfo);

	return iRet;
}




