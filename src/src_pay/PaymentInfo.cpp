#include "PaymentInfo.h"
#include "PaymentInfoSql.h"
#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"
#include "dcc_ra.h"


static vector<AbmPayCond *> g_vServ;
static vector<AbmPayCond *>::iterator g_itr;

PaymentInfo::PaymentInfo()
{
    m_poSql = new PaymentInfoSql;
    pAllocateBalanceSql =new AllocateBalanceSql;
    m_poUnpack = NULL;
    m_poPack = NULL;
    m_dccOperation = new DccOperation;	
}

PaymentInfo:: ~PaymentInfo()
{
    FreeObject(m_poSql);
    FreeObject(pAllocateBalanceSql);
    FreeObject(m_dccOperation );
}

int PaymentInfo::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
    int iRet;
    m_poUnpack = (AbmPaymentCCR *)pCCR;
    m_poPack = (AbmPaymentCCA *)pCCA;
    m_poPack->clear();
    g_vServ.clear();
    
    __DEBUG_LOG0_(0, "֧����Ϣ::unpack ��Ϣ��");
    //��Ϣ���
    if ((iRet=m_poUnpack->getPayInfo(g_vServ)) == 0)
    {
        __DEBUG_LOG0_(0, "֧����Ϣ::deal ��Ϣ��ΪNULL");
        m_poPack->setRltCode(ECODE_NOMSG);
        return PAYMENT_UNPACK_ERR;
    }
    else if (iRet < 0) 
    {
        __DEBUG_LOG0_(0, "֧����Ϣ::deal ��Ϣ�������!");
        m_poPack->setRltCode(ECODE_UNPACK_FAIL);
        return PAYMENT_UNPACK_ERR;
    }
    for(unsigned  int i=0;i<g_vServ.size();i++)
    {
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sOperatype=%s",i,g_vServ[i]->m_sOperatype );
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sOperateSeq=%s",i, g_vServ[i]->m_sOperateSeq);
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sSpID=%s",i,g_vServ[i]->m_sSpID );
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sSpName=%s",i,g_vServ[i]->m_sSpName );
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sServicePlatformID=%s",i, g_vServ[i]->m_sServicePlatformID);
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_iOperateAction=%d",i, g_vServ[i]->m_iOperateAction );

	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_iStaffID=%d",i,g_vServ[i]->m_iStaffID );
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sMicropayType=%s",i, g_vServ[i]->m_sMicropayType);
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sDestinationAccount=%s",i,g_vServ[i]->m_sDestinationAccount );
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sDestinationtype=%s",i, g_vServ[i]->m_sDestinationtype);
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_iDestinationAttr=%d",i, g_vServ[i]->m_iDestinationAttr );

	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sPayPassword=%s",i,g_vServ[i]->m_sPayPassword );
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sRandomPassword=%s",i, g_vServ[i]->m_sRandomPassword);
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sPayDate=%s",i,g_vServ[i]->m_sPayDate );
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_iPayDate=%d",i,g_vServ[i]->m_iPayDate );
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sOrderID=%s",i, g_vServ[i]->m_sOrderID);
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_sDescription=%s",i, g_vServ[i]->m_sDescription );
	    __DEBUG_LOG2_(0,"֧����Ϣ���:[%d].m_lPayAmount=%d",i, g_vServ[i]->m_lPayAmount );
    }

    try 
    {
        iRet=_deal(g_vServ);
        __DEBUG_LOG1_(0, "֧�����::iRet=%d", iRet);
    }
    catch(TTStatus oSt) 
    {
        __DEBUG_LOG1_(0, "֧��ʧ��::oSt.err_msg=%s", oSt.err_msg);
	m_poPack->clear();
    	m_poPack->setRltCode(ECODE_TT_FAIL);
    }
    return iRet;
}

int PaymentInfo::_deal(vector<AbmPayCond *>&vIn)
{
	//У������
	//���������֤�������
	//У��۷�����8�C�۷�
	//�����û���ʶ��ѯ�˻���ϸ��У���Ƿ��㹻���۷�(����)����¼ҵ���¼��Ϣ��,
	//��ѯ�����Դ��Ϣ��ÿ����Դ��Ϣ���´�����Դ�ĵ�ǰ����¼���֧������¼�����Դ֧����ϵ����������˱���
	int iRet=0;
	bool bState=false;
	ResponsPayOne oRes={0};
	ResponsPayTwo oData={0};

	PaymentInfoALLCCR pPaymentInfoCCR={0};
	PaymentInfoALLCCA pPaymentInfoCCA={0};
	ServAcctInfo oServAcct={0};

	vector<ResponsPayTwo>vRespTwo;
	ResponsPayTwo pRespTwo={0};
	vRespTwo.clear();
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
			__DEBUG_LOG1_(0, "֧������DCCͷ��=[%d]",iRet);
			throw iRet;
		}
		
	
		for(vector<AbmPayCond *>::iterator iter=vIn.begin();iter!=vIn.end();iter++)
		{
			memset(&pPaymentInfoCCR,0,sizeof(PaymentInfoALLCCR));
			memset(&pPaymentInfoCCA,0,sizeof(PaymentInfoALLCCA));
			memset(&oServAcct,0,sizeof(oServAcct));

			strcpy(pPaymentInfoCCR.m_sOperateSeq,(*iter)->m_sOperateSeq);
			pPaymentInfoCCR.m_iBalance_unit=1;
			strcpy(pPaymentInfoCCR.m_sSpID,(*iter)->m_sSpID);
			strcpy(pPaymentInfoCCR.m_sSpName,(*iter)->m_sSpName);
			strcpy(pPaymentInfoCCR.m_sServicePlatformID,(*iter)->m_sServicePlatformID);
			pPaymentInfoCCR.m_iStaffID=(*iter)->m_iStaffID;
			strcpy(pPaymentInfoCCR.m_sMicropayType,(*iter)->m_sMicropayType);
			strcpy(pPaymentInfoCCR.m_sDestinationAccount,(*iter)->m_sDestinationAccount);
			__DEBUG_LOG1_(0, "֧������:m_sDestinationAccount=[%s]",(*iter)->m_sDestinationAccount);
			__DEBUG_LOG1_(0, "֧������::pPaymentInfoCCR.m_sDestinationAccount=[%s]",pPaymentInfoCCR.m_sDestinationAccount);
			strcpy(pPaymentInfoCCR.m_sDestinationtype,(*iter)->m_sDestinationtype);
			pPaymentInfoCCR.m_iDestinationAttr=(*iter)->m_iDestinationAttr;
			memset((*iter)->m_sPayDate,'\0',sizeof((*iter)->m_sPayDate));
			if(((*iter)->m_iPayDate)>0)
				long2date((*iter)->m_iPayDate,(*iter)->m_sPayDate);
			else
				GetTime((*iter)->m_sPayDate);
			__DEBUG_LOG1_(0, "֧��ʱ��::pPaymentInfoCCR.m_sPayDate=[%s]",(*iter)->m_sPayDate);
			strcpy(pPaymentInfoCCR.m_sDeductDate,(*iter)->m_sPayDate);
			strcpy(pPaymentInfoCCR.m_sOrderID,(*iter)->m_sOrderID);
			strcpy(pPaymentInfoCCR.m_sDescription,(*iter)->m_sDescription);
			pPaymentInfoCCR.m_lPayAmount=(*iter)->m_lPayAmount;
			strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0003");
			strcpy(pPaymentInfoCCR.m_sOrderState,"C0C");
			//��ѯ�û���Ϣ
			iRet=pAllocateBalanceSql->preQryServAcctInfo(pPaymentInfoCCR.m_sDestinationAccount,2,oServAcct,oExp);
			if(iRet!=0)
			{
				__DEBUG_LOG1_(0, "��ѯ�û�����ʧ��=[%d]",iRet);
				throw iRet;
			}
			pPaymentInfoCCR.m_lServID=oServAcct.m_servId;
			__DEBUG_LOG1_(0, "pPaymentInfoCCR.m_lServID==[%d]",pPaymentInfoCCR.m_lServID);
			//У��֧������
			bState=true;
			iRet=m_poSql->ComparePasswd(*iter,pPaymentInfoCCR,bState);
			__DEBUG_LOG1_(0, " У��֧��������=[%d]",iRet);
			if(iRet!=0)
			{
				throw iRet;
			}
			//У������漴����
			bState=false;
			iRet=m_poSql->ComparePasswd(*iter,pPaymentInfoCCR,bState);
			__DEBUG_LOG1_(0, " �����漴����У����=[%d]",iRet);
			if(iRet!=0)
			{
				throw iRet;
			}
			//�۷Ѷ���
			iRet=RecodePaymentInfo( pPaymentInfoCCR,pPaymentInfoCCA);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "֧��ʧ��!");
				throw iRet;
			}
			sprintf(pRespTwo.m_sReqSerial,"%d",pPaymentInfoCCA.m_lPaymentID);
			strcpy(pRespTwo.m_sOperateSeq,(*iter)->m_sOperateSeq);
			vRespTwo.push_back(pRespTwo);
		}

		
	}
	catch(TTStatus oSt) 
	{
		iRet=PAYMENT_TT_ERR;
		__DEBUG_LOG1_(0, "֧��ʧ�� oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "֧��ʧ�� value=%d",value);
	}
	oRes.m_iRltCode=iRet;
	if (!m_poPack->addRespons(oRes)) 
	{
		__DEBUG_LOG0_(0, "֧�� m_poPack ���ʧ��!");
		m_poPack->setRltCode(ECODE_PACK_FAIL);
		return PAYMENT_PACK_ERR;
	}
	__DEBUG_LOG2_(0, "֧��:: _deal m_poPack ��1��=[%d][%d]!",oRes.m_iRltCode,iRet);
	if( iRet!=0 )
	{
	    sprintf(pRespTwo.m_sReqSerial,"%d",-1);
	    strcpy(pRespTwo.m_sOperateSeq,vIn[0]->m_sOperateSeq);
	    vRespTwo.push_back(pRespTwo);
	    
	}
	for(unsigned int k=0;k<vRespTwo.size();k++)
	{
	    __DEBUG_LOG1_(0, "֧��:: _deal m_poPack ��2��=[%s]!",vRespTwo[k].m_sReqSerial);
	    __DEBUG_LOG1_(0, "֧��:: _deal m_poPack ��2��=[%s]!",vRespTwo[k].m_sOperateSeq);
		if (!m_poPack->addPayData(vRespTwo[k]))
		{
			__DEBUG_LOG0_(0, "֧��:: _deal m_poPack ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			return  PAYMENT_PACK_ERR;
		}
	}

	return iRet;
} 
//�����û���ʶ��ѯ�˻���ϸ��У���Ƿ��㹻���۷�(����)����¼ҵ���¼��Ϣ��,
//��ѯ�����Դ��Ϣ��ÿ����Դ��Ϣ���´�����Դ�ĵ�ǰ����¼���֧������¼�����Դ֧����ϵ����������˱���
int PaymentInfo::RecodePaymentInfo(PaymentInfoALLCCR &pPaymentInfoCCR,PaymentInfoALLCCA &pPaymentInfoCCA)
{

	bool bState=false;
	int iRet=0;
	long lPaymentAmount=0L;
	long m_lBalance=0L;
	char sDate[15];
	string  m_sOperType;


	vector<AcctBalanceInfo*>vAcctBalanceInfo;
	vector<BalanceSourceInfo *>vBalanceSourceInfo;
	vAcctBalanceInfo.clear();
	vBalanceSourceInfo.clear();
	StructPaymentSeqInfo  sStructPaymentSeqInfo={0};
	
	try 
	{

		
		//ȡ��ǰϵͳʱ��
		memset(sDate,'\0',sizeof(sDate));
		GetDate(sDate);

		//��ѯ�˻���ϸ
		iRet=m_poSql->QueryAcctBalanceInfo(pPaymentInfoCCR,vAcctBalanceInfo);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "֧������ѯ�˻���ϸʧ��==[%d]",iRet);
			throw iRet;
		}
		__DEBUG_LOG1_(0, "֧������ѯ�˻���ϸ���.size==[%d]",vAcctBalanceInfo.size());
		
		//У���Ƿ��㹻�۷�
		for(vector<AcctBalanceInfo*>::iterator it=vAcctBalanceInfo.begin();it!=vAcctBalanceInfo.end();it++)
		{
			if(((strlen((*it)->m_sEffDate)>=8)&&(memcmp((*it)->m_sEffDate,sDate,8)>0))||((strlen((*it)->m_sExDate)>=8)&&(memcmp((*it)->m_sExDate,sDate,8)<0)))
			{
				continue;
			}

			m_lBalance+=(*it)->m_lBalance;
		}
		if( (m_lBalance) < (pPaymentInfoCCR.m_lPayAmount) )
		{
			__DEBUG_LOG0_(0, "֧�����˱�������֧��!");
			throw BALANCE_NOT_PAYMENT_ERR;
		}
		//��ѯҵ����ˮ
		iRet=m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bState);
		if(iRet!=0)
        	{
			__DEBUG_LOG0_(0, "֧������ѯҵ����ˮʧ��!");
			throw iRet;
        	}
		pPaymentInfoCCA.m_lPaymentID=sStructPaymentSeqInfo.m_lPaymentSeq;
		//��¼ҵ����Ϣ��
		iRet=m_poSql->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
		if(iRet!=0)
        	{
			__DEBUG_LOG0_(0, "֧������¼ҵ����Ϣ��ʧ��!");
			throw iRet;
        	}
		for(vector<AcctBalanceInfo*>::iterator it=vAcctBalanceInfo.begin();it!=vAcctBalanceInfo.end();it++)
		{
			__DEBUG_LOG1_(0, " ֧�����˱�m_lAcctBalanceID=[%d] ",(*it)->m_lAcctBalanceID);
			FreeVector(vBalanceSourceInfo);
			//�ж��˻���Ч��
			if(((strlen((*it)->m_sEffDate)>=8)&&(memcmp((*it)->m_sEffDate,sDate,8)>0))||((strlen((*it)->m_sExDate)>=8)&&(memcmp((*it)->m_sExDate,sDate,8)<0)))
			{
			    	__DEBUG_LOG0_(0, "֧���˻���ʧЧ!");
			    	__DEBUG_LOG2_(0, "֧���˻���ʧЧ,��Чʱ��=[%s],��ǰʱ��=[%s]!",(*it)->m_sEffDate,sDate);
			    	__DEBUG_LOG2_(0, "֧���˻���ʧЧ,ʧЧʱ��=[%s],��ǰʱ��=[%s]!",(*it)->m_sExDate,sDate);
				continue;

			}
			//��ѯ�����Դ��
			iRet=m_poSql->QueryBalanceSourceInfo((*it)->m_lAcctBalanceID,vBalanceSourceInfo);
			if(iRet!=0)
            		{
			    	__DEBUG_LOG0_(0, "֧������ѯ�����Դ��ʧ��!");
				throw iRet;
            		}
			//ѭ��������Դ��Ϣ��ǰ���
			vector<BalanceSourceInfo*>::iterator iter=vBalanceSourceInfo.begin();
			while((pPaymentInfoCCR.m_lPayAmount)>0&&(iter!=vBalanceSourceInfo.end()))
			{
				if( (*iter)->m_lBalance<=0 )
				{
					__DEBUG_LOG1_(0, "֧����Դ����֧��������Ѱ��һ��֧����Դ,seq=[%d]!",(*iter)->m_lOperIncomeID);
					continue;
				}
				if((pPaymentInfoCCR.m_lPayAmount)>=((*iter)->m_lBalance))
				{
					sStructPaymentSeqInfo.m_lPaymentAmount=(*iter)->m_lBalance;
					pPaymentInfoCCR.m_lPayAmount =pPaymentInfoCCR.m_lPayAmount-(*iter)->m_lBalance;
				}
				else
				{
					sStructPaymentSeqInfo.m_lPaymentAmount=pPaymentInfoCCR.m_lPayAmount;
					pPaymentInfoCCR.m_lPayAmount=0;
				}
				bState=true;
				//��ѯҵ����ˮ
				iRet=m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bState);
				if(iRet!=0)
                		{
			        	__DEBUG_LOG0_(0, "֧������ѯҵ����ˮʧ��!");
					throw iRet;
                		}
				__DEBUG_LOG1_(0, "֧��:m_lPaymentSeq[%d]!",sStructPaymentSeqInfo.m_lPaymentSeq);
				__DEBUG_LOG1_(0, "֧��:m_lBalanceSourceSeq [%d]!",sStructPaymentSeqInfo.m_lBalanceSourceSeq);
				__DEBUG_LOG1_(0, "֧��:m_lBalancePayoutSeq [%d]!",sStructPaymentSeqInfo.m_lBalancePayoutSeq);
				__DEBUG_LOG1_(0, "֧��:m_lSourcePayoutSeq [%d]!",sStructPaymentSeqInfo.m_lSourcePayoutSeq);
				__DEBUG_LOG1_(0, "��֧��:m_lPaymentAmount [%d]!",sStructPaymentSeqInfo.m_lPaymentAmount);
				__DEBUG_LOG1_(0, "δ֧��:m_lPaymentAmount [%d]!",pPaymentInfoCCR.m_lPayAmount);
				//��¼���֧����
				m_sOperType.clear();
				m_sOperType="5UI";
				iRet=m_poSql->InsertBalancePayout(sStructPaymentSeqInfo,*iter,m_sOperType);
				if(iRet!=0)
                		{
			        	__DEBUG_LOG0_(0, "֧������¼���֧����ʧ��!");
					throw iRet;
                		}
				//��¼��Դ֧����ϵ��
				iRet=m_poSql->InsertBalanceSourcePayoutRela(sStructPaymentSeqInfo,*iter);
				if(iRet!=0)
                		{	
			        	__DEBUG_LOG0_(0, "֧������¼��Դ֧����ϵ��ʧ��!");
					throw iRet;
                 		}
				//��������˱���
				iRet=m_poSql->updateAcctBalance(sStructPaymentSeqInfo,*it);
				if(iRet!=0)
                		{
			        	__DEBUG_LOG0_(0, "֧������������˱���ʧ��!");
					throw iRet;
                		}
				//���������Դ��
				iRet=m_poSql->updateBalanceSource(sStructPaymentSeqInfo,*iter);
				if(iRet!=0)
                		{
			        	__DEBUG_LOG0_(0, "֧�������������Դ��ʧ��!");
					throw iRet;
                		}
				iter++;
			}
		}
	}
	catch(TTStatus oSt) 
	{
		iRet=PAYMENT_TT_ERR;
		__DEBUG_LOG1_(0, "֧��ʧ��::oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "֧��ʧ��:: value=%d",value);
	}
	FreeVector(vAcctBalanceInfo);
	FreeVector(vBalanceSourceInfo);
	return iRet;

}

// ֧������ 
int PaymentInfo::ReversePayoutInfo(char *sOldRequestId,ReversePaySvc &oRvsInfo,vector<BalancePayoutInfo*> &vBalancePayOut)
{
	int iRet;
	
	long lOperPayoutId;
	long lAcctBalanceId;
	long lAmount;
	long lOptId;
	bool bFlag = false;
	
	StructPaymentSeqInfo  sStructPaymentSeqInfo={0};// ҵ����ˮ��Ϣ
	BalanceSourceInfo *pBalanceSourceInfo=NULL;
	
	AcctBalanceInfo *pAcctBalanceInfo=new AcctBalanceInfo;
	try
	{
		PaymentInfoALLCCR pPaymentInfoCCR={0};
		StructPaymentSeqInfo  sStructPaymentSeqInfo={0};
		// ����ҵ����ˮ��
		iRet = m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bFlag);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "ȡҵ����ˮ��ʧ�ܡ�");
			throw iRet;
		}
		
		strcpy(pPaymentInfoCCR.m_sOperateSeq ,oRvsInfo.sOperationSeq); // �ⲿ��ˮ��
		strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0006"); // ��������
		strcpy(pPaymentInfoCCR.m_sServicePlatformID,oRvsInfo.sServicePlatformId);// ƽ̨ID
		strcpy(pPaymentInfoCCR.m_sSpID,oRvsInfo.sSpId);// �̼�ID
		strcpy(pPaymentInfoCCR.m_sSpName,oRvsInfo.sSpName);// �̼�����
		strcpy(pPaymentInfoCCR.m_sOrderState,"COF");
		strcpy(pPaymentInfoCCR.m_sDescription,"֧������");// ��������
		
		// todo:�����Ӹ����ⲿ��ˮ�ţ�����ҵ����ˮ�ţ���¼������ҵ����ˮ��
		//char sForeignId[64]={0};
		
		//strcpy(sForeignId,ltoa(lOldRequestId));
		long lOptId;
		long lOldRequestId=0L;
		iRet = m_poSql->qryOptidByForeignId(sOldRequestId,lOptId);
		lOldRequestId = lOptId; // 
		
		__DEBUG_LOG0_(0, "�����ⲿ��ˮ��ҵ����ˮ��");
		__DEBUG_LOG1_(0, "ҵ����ˮ�ţ�%d",lOptId);
		
		sStructPaymentSeqInfo.m_lRbkPaymentSeq = lOldRequestId; // ��������ˮ�� 
		// ���뻮������ҵ����ˮ��¼	
		/* ����֧����¼��ͳ�Ƴ������
		iRet = m_poSql->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "����ҵ����ˮ��ʧ�ܡ�");
			throw iRet;
		}
		*/
		
		// 1.���±�������ˮ�ŵ�״̬ΪC0R
		iRet = m_poSql->updateOperation(lOptId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������ҵ����ˮ��״̬�޸�ʧ�ܡ�");
			throw iRet;
		}
		
		// ��ѯ֧����Ϣ
		// todo:�����Ӧ����opt_id
		iRet=m_poSql->qryPayoutInfo(lOldRequestId,vBalancePayOut);
		
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "��ѯ���������֧��ʧ�ܡ�");
			throw iRet;
		}
		long lTotalAmount=0L;
		for(vector<BalancePayoutInfo*>::iterator it=vBalancePayOut.begin();it!=vBalancePayOut.end();it++)
		{
			lAcctBalanceId = (*it)->m_lAcctBalanceId;
			lAmount = (*it)->m_lAmount;
			lOptId = (*it)->m_lOptId;
			lOperPayoutId = (*it)->m_lOperPayoutId;
			lTotalAmount+=lAmount;// �������ܽ��
			__DEBUG_LOG1_(0, "���֧��lAcctBalanceId=[%ld] ",lAcctBalanceId);
			__DEBUG_LOG1_(0, "���֧��lAmount=[%ld]",lAmount);
			__DEBUG_LOG1_(0, "���֧��lOptId=[%ld]",lOptId);
			__DEBUG_LOG1_(0, "���֧��ID[%ld]",lOperPayoutId);
			
			// ��ѯ��������¼��Ӧ�������Դ,���֧��ID����Դ֧����ϵ��
			iRet = m_poSql->qryBalanceSource(lOptId,pBalanceSourceInfo);
			
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "PaymentInfo::QryPayoutInfo ������¼�����Դ�����ڻ��ѯʧ��");
				throw iRet;
			}
			
			
			// ���������Դ��ʣ����+lAmount
			//pBalanceSourceInfo->m_lBalance = pBalanceSourceInfo->m_lBalance + lAmount;
			sStructPaymentSeqInfo.m_lPaymentAmount = -lAmount;		
			iRet = m_poSql->updateBalanceSource(sStructPaymentSeqInfo,pBalanceSourceInfo);
			
			// �������֧����¼״̬10X
			iRet = m_poSql->updateBalancePayout(lOperPayoutId);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "�������֧����¼״̬10Xʧ��");
				throw iRet;
			}
			
			
			// �����Դ��֧����¼��ϵ�� - ����״̬10X
			iRet = m_poSql->updateBalanceSourcePayoutRela(lOperPayoutId);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "�����Դ��֧����¼��ϵ�� - ����״̬10Xʧ��");
				throw iRet;
			}
			
			
			// 5.��Ӧ����ʱ����¼����
			pAcctBalanceInfo->m_lAcctBalanceID = lAcctBalanceId;
			
			// ��ѯ����ʱ�
			iRet = m_poSql->qryAcctBalance(lAcctBalanceId,pAcctBalanceInfo);
			
			
			// ����ʱ����
			iRet = m_poSql->updateAcctBalance(sStructPaymentSeqInfo,pAcctBalanceInfo);			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "����ʱ�������ʧ��");
				throw iRet;
			}	

	
		}
		
		pPaymentInfoCCR.m_lPayAmount = lTotalAmount;
		// ���뻮������ҵ����ˮ��¼	
		iRet = m_poSql->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "����ҵ����ˮ��ʧ�ܡ�");
			throw iRet;
		}
	}
	catch(TTStatus oSt) 
	{
		iRet=REVERSE_PAYMENT_TT_ERR;
		__DEBUG_LOG1_(0, "PaymentInfo::QryPayoutInfo oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "PaymentInfo::QryPayoutInfo value=%d",value);
	}
	
	FreeObject(pAcctBalanceInfo);
	FreeObject(pBalanceSourceInfo);
	
	return iRet;

}

// ����-����
int PaymentInfo::DeductBalanceInfo(PaymentInfoALLCCR &pPaymentInfoCCR,PaymentInfoALLCCA &pPaymentInfoCCA)
{
	bool bState=false;
	int iRet=0;
	long lPaymentAmount=0L;
	long m_lBalance=0L;

	vector<AcctBalanceInfo*>vAcctBalanceInfo;
	vector<BalanceSourceInfo *>vBalanceSourceInfo;
	vAcctBalanceInfo.clear();
	vBalanceSourceInfo.clear();
	StructPaymentSeqInfo  sStructPaymentSeqInfo={0};
	
	try 
	{
		//��ѯ����ʱ���ϸ
		iRet=m_poSql->QueryAcctBalanceInfo(pPaymentInfoCCR,vAcctBalanceInfo);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "QueryAcctBalanceInfo==[%d]",iRet);
			throw iRet;
		}
		__DEBUG_LOG1_(0, "QueryAcctBalanceInfo.size==[%d]",vAcctBalanceInfo.size());
		
		//У���Ƿ��㹻�۷�
		for(vector<AcctBalanceInfo*>::iterator it=vAcctBalanceInfo.begin();it!=vAcctBalanceInfo.end();it++)
			m_lBalance+=(*it)->m_lBalance;
			
		__DEBUG_LOG1_(0, "����ʱ��ܽ��m_lBalance==[%d]",m_lBalance);	
		__DEBUG_LOG1_(0, "�軮�����m_lBalance==[%d]",0-pPaymentInfoCCR.m_lPayAmount);
		
		// �ʻ�ʣ����
		pPaymentInfoCCA.m_lAcctBalance = m_lBalance + pPaymentInfoCCR.m_lPayAmount;
		
		if( (m_lBalance) < (0-pPaymentInfoCCR.m_lPayAmount))
		{
			__DEBUG_LOG0_(0, "���� - �ʱ������Ի���!");
			throw -1;
		}
		//��ѯҵ����ˮ - PaymentSeq
		iRet=m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bState);
		if(iRet!=0)
		{
			__DEBUG_LOG0_(0, "���� - ����ҵ����ˮ��ʧ��");
			throw iRet;
		}
		// ��Ӧ��Ϣ
		pPaymentInfoCCA.m_lPaymentID=sStructPaymentSeqInfo.m_lPaymentSeq;  // ҵ����ˮ��
		
		//��¼ҵ����Ϣ��
		iRet=m_poSql->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo); // ����ҵ���¼��Ϣ
		
		if(iRet!=0)
		{
			__DEBUG_LOG0_(0, "���� - ����ҵ����Ϣ��ʧ��");
			throw iRet;
		}
		
		// ��������ʱ����
		long lPayAmount = 0 - pPaymentInfoCCR.m_lPayAmount; // �������
		long lBalance;
		BalanceSourceInfo balanceSourceInfo;
		for(vector<AcctBalanceInfo*>::iterator iter=vAcctBalanceInfo.begin();iter!=vAcctBalanceInfo.end();iter++)
		{
			__DEBUG_LOG1_(0, " vAcctBalanceInfo.m_lAcctBalanceID=[%d] ",(*iter)->m_lAcctBalanceID);

			lBalance = (*iter)->m_lBalance; // ����ʱ����
			
			if(lPayAmount==0)	// ���軮��
				break;
			
			if(lPayAmount >= lBalance)
			{
				lPayAmount = lPayAmount - lBalance; // ���������
				sStructPaymentSeqInfo.m_lPaymentAmount = lBalance;
				balanceSourceInfo.m_lAmount = 0-lBalance;
			}
			else
			{
				sStructPaymentSeqInfo.m_lPaymentAmount = lPayAmount; // ÿ�λ������
				balanceSourceInfo.m_lAmount = 0-lPayAmount; // �����Դ��¼���
				lPayAmount=0;	
			}
				
			//��ѯҵ����ˮ - �����Դ��¼��ˮ
			bState = true;
			iRet=m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bState);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "���� - ����ҵ����ˮ��ʧ��");
				throw iRet;
			}

			// ��������ʱ��� - ����ʱ��۷�
			iRet=m_poSql->updateAcctBalance(sStructPaymentSeqInfo,*iter);
			if(iRet!=0)
			{
		        	__DEBUG_LOG0_(0, "���� - ��������ʱ�ʧ��");
				throw iRet;
        		}
			
			// �����Դ�����һ����Ӧ�۷ѵĸ�ֵ
			// �����Դ����׼��
			
			// ���������Դ��¼��
			// ��¼�����Դ�� - ��¼һ�ʸ�Ǯ
			// �����Դ��ֵ
			balanceSourceInfo.m_lOperIncomeID = sStructPaymentSeqInfo.m_lBalanceSourceSeq;
			balanceSourceInfo.m_lAcctBalanceId = (*iter)->m_lAcctBalanceID;
			
			
			pPaymentInfoCCA.m_lAcctBalanceId = (*iter)->m_lAcctBalanceID; // �۷��˱���ʶ
			pPaymentInfoCCA.m_lDeductAmountAll = sStructPaymentSeqInfo.m_lPaymentAmount; // �˱��������
			// �˱�ʣ����
			pPaymentInfoCCA.m_lAcctBalanceAmount = lBalance - pPaymentInfoCCA.m_lDeductAmountAll;
			
			//balanceSourceInfo.m_lAmount = -10;
			strcpy(balanceSourceInfo.m_sSourceType,"5VH");// �����������
			strcpy(balanceSourceInfo.m_sSourceDesc,"����");
			balanceSourceInfo.m_lPaymentID=sStructPaymentSeqInfo.m_lPaymentSeq;
			balanceSourceInfo.m_lBalance=balanceSourceInfo.m_lAmount;
			strcpy(balanceSourceInfo.m_sAllowDraw,"ADY");
	
			iRet = m_poSql->insertBalanceSource(balanceSourceInfo);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "���� - ���������Դʧ��");
				throw iRet;
			}	
		}
	}
	catch(TTStatus oSt) 
	{
		iRet=BALANCE_PAYOUT_TT_ERR;
		__DEBUG_LOG1_(0, "PaymentInfo::DeductBalanceInfo oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "PaymentInfo::DeductBalanceInfo value=%d",value);
	}
	FreeVector(vAcctBalanceInfo);
	FreeVector(vBalanceSourceInfo);
	return iRet;
}

// ��������
// 1.�޸�ԭҵ����ˮ��¼״̬ΪC0R
// 2.ҵ����ˮ��¼�г�ֵһ����Ǯ��״̬ΪC0F
// 3.���������Դ��balance�ֶ�
// 4.ͬʱ��������ʱ���
// 5.�����֧������Ǯ
// 6.�������Դ֧����ϵ��¼

int PaymentInfo::ReverseDeductBalance(char* sOldRequestId,ReqReverseDeductSvc &oRvsInfo)
{
	int iRet=-1;
	
	long lOperPayoutId=0L;
	long lAcctBalanceId=0L;
	long lAmount=0L;
	long lOptId=0L;
	bool bFlag = false;
	string m_sOperType;
	
	long lBalanceSourceId=0L;
	
	long lServId=0L;
	
	
	StructPaymentSeqInfo  sStructPaymentSeqInfo={0};// ҵ����ˮ��Ϣ
	BalanceSourceInfo *pBalanceSourceInfo=NULL;
	
	AcctBalanceInfo *pAcctBalanceInfo=new AcctBalanceInfo;
	vector<BalanceSourceInfo *> vBalanceSourceInfo;
	try
	{
		PaymentInfoALLCCR pPaymentInfoCCR={0};
		StructPaymentSeqInfo  sStructPaymentSeqInfo={0};
		// ����ҵ����ˮ��
		iRet = m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bFlag);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "ȡҵ����ˮ��ʧ�ܡ�");
			throw iRet;
		}
		
		// �������������ˮ�Ų��ұ�������¼ҵ����ˮ
		//iRet = m_poSql->QueryOptID(sOldRequestId,lOptId);
		//iRet = m_poSql->qryOptidByForeignId(sOldRequestId,lOptId);
		// Modify 2011.7.5 ���Ӳ�ѯServ_Id
		iRet = m_poSql->qryOptInfoByForeignId(sOldRequestId,lOptId,lServId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "���ұ�����ҵ����ˮ��ʧ�ܡ�");
			throw iRet;
		}
		
		strcpy(pPaymentInfoCCR.m_sOperateSeq ,oRvsInfo.m_sRequestId); // �ⲿ��ˮ��
		strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0018"); // ��������
		strcpy(pPaymentInfoCCR.m_sServicePlatformID,oRvsInfo.m_sABMId);// ƽ̨ID
		strcpy(pPaymentInfoCCR.m_sDescription,"��������");// ��������
		strcpy(pPaymentInfoCCR.m_sOrderState,"COF");
		pPaymentInfoCCR.m_lServID= lServId; // ������ҵ����ˮ���û���ʶ
		__DEBUG_LOG1_(0, "������ҵ����ˮ���û���ʶlServId=[%ld]",lServId);
		sStructPaymentSeqInfo.m_lRbkPaymentSeq = lOptId; // ������ҵ����ˮ��
		__DEBUG_LOG1_(0, "������ҵ����ˮ��lOptId=[%ld]",lOptId);
		
		// 1.���±�������ˮ�ŵ�״̬ΪC0R
		iRet = m_poSql->updateOperation(lOptId);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "������ҵ����ˮ��״̬�޸�ʧ�ܡ�");
			throw iRet;
		}
		// m_lPayAmount ��������ô��ȡ������
		/*
		// 2.���뻮������ҵ����ˮ��¼, ״̬C0F	
		iRet = m_poSql->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "����ҵ����ˮ��ʧ�ܡ�");
			throw iRet;
		}
		*/
		
		long lTotalAmount=0L;
		
		
		// 3.���ݱ�������ˮ�Ų�ѯ����ʱ����Ӧ�������Դ
		iRet = m_poSql->qryBalanceSource(lOptId,vBalanceSourceInfo);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "��ѯ������ҵ����ˮ�������Դ��Ϣʧ�ܡ�");
			throw iRet;
		}
		
		for(vector<BalanceSourceInfo*>::iterator it=vBalanceSourceInfo.begin();it!=vBalanceSourceInfo.end();it++)
		{
			// �������֧������Դ֧����ˮ
			bFlag = true;
			iRet = m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bFlag);
			if(iRet != 0)
			{
				__DEBUG_LOG0_(0, "ȡҵ����ˮ��ʧ�ܡ�");
				throw iRet;
			}
			
			// ȡ�����Դ������Ϣ
			lBalanceSourceId = (*it)->m_lOperIncomeID; // �����Դ��ʶ
			lAcctBalanceId = (*it)->m_lAcctBalanceId; // ����ʱ���ʶ
			lAmount = (*it)->m_lAmount;	// ���
			
			// ��д��Դ��ˮ��,�˴�δ�����µ���Դ��¼
			sStructPaymentSeqInfo.m_lBalanceSourceSeq = lBalanceSourceId;
			
			// 4.���������Դ��Balance
			sStructPaymentSeqInfo.m_lPaymentAmount = lAmount; // ���������Դ��¼�е�ʣ����
			iRet = m_poSql->updateBalanceSource(sStructPaymentSeqInfo,(*it));
			if(iRet != 0)
			{
				__DEBUG_LOG0_(0, "���±�����ҵ����ˮ�������Դ��Ϣʧ�ܡ�");
				throw iRet;
			}
			// 5.�����֧����¼ ***
			m_sOperType.clear();
			m_sOperType="5UK";
			iRet = m_poSql->InsertBalancePayout(sStructPaymentSeqInfo,(*it),m_sOperType);
			if(iRet != 0)
			{
				__DEBUG_LOG0_(0, "���뱻����ҵ����ˮ�����֧����Ϣʧ�ܡ�");
				throw iRet;
			}
			// 6.����Դ֧����¼
			iRet = m_poSql->InsertBalanceSourcePayoutRela(sStructPaymentSeqInfo,(*it));
			if(iRet != 0)
			{
				__DEBUG_LOG0_(0, "���뱻����ҵ����ˮ�������Դ֧����Ϣʧ�ܡ�");
				throw iRet;
			}
			// 7.��������ʱ���¼
			long ltmp = 0-lAmount;
			iRet = m_poSql->updateAcctBalance(lAcctBalanceId,ltmp);
			if(iRet != 0)
			{
				__DEBUG_LOG0_(0, "��������ʱ���Ϣʧ�ܡ�");
				throw iRet;
			}
			
			lTotalAmount +=ltmp;
		}
		
		// 2.���뻮������ҵ����ˮ��¼, ״̬C0F	
		pPaymentInfoCCR.m_lPayAmount = lTotalAmount;
		iRet = m_poSql->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "����ҵ����ˮ��ʧ�ܡ�");
			throw iRet;
		}
			
		/*
		// ��ѯ֧����Ϣ
		iRet=m_poSql->qryPayoutInfo(lOldRequestId,vBalancePayOut);
		
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "��ѯ���������֧��ʧ�ܡ�");
			throw iRet;
		}
		
		for(vector<BalancePayoutInfo*>::iterator it=vBalancePayOut.begin();it!=vBalancePayOut.end();it++)
		{
			lAcctBalanceId = (*it)->m_lAcctBalanceId;
			lAmount = (*it)->m_lAmount;
			lOptId = (*it)->m_lOptId;
			lOperPayoutId = (*it)->m_lOperPayoutId;
			
			__DEBUG_LOG1_(0, "���֧��lAcctBalanceId=[%ld] ",lAcctBalanceId);
			__DEBUG_LOG1_(0, "���֧��lAmount=[%ld]",lAmount);
			__DEBUG_LOG1_(0, "���֧��lOptId=[%ld]",lOptId);
			__DEBUG_LOG1_(0, "���֧��ID[%ld]",lOperPayoutId);
			
			// ��ѯ��������¼��Ӧ�������Դ,���֧��ID����Դ֧����ϵ��
			iRet = m_poSql->qryBalanceSource(lOptId,pBalanceSourceInfo);
			
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "PaymentInfo::QryPayoutInfo ������¼�����Դ�����ڻ��ѯʧ��");
				throw iRet;
			}
			
			
			// ���������Դ��ʣ����+lAmount
			//pBalanceSourceInfo->m_lBalance = pBalanceSourceInfo->m_lBalance + lAmount;
			sStructPaymentSeqInfo.m_lPaymentAmount = -lAmount;		
			iRet = m_poSql->updateBalanceSource(sStructPaymentSeqInfo,pBalanceSourceInfo);
			
			// �������֧����¼״̬10X
			iRet = m_poSql->updateBalancePayout(lOperPayoutId);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "�������֧����¼״̬10Xʧ��");
				throw iRet;
			}
			
			
			// �����Դ��֧����¼��ϵ�� - ����״̬10X
			iRet = m_poSql->updateBalanceSourcePayoutRela(lOperPayoutId);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "�����Դ��֧����¼��ϵ�� - ����״̬10Xʧ��");
				throw iRet;
			}
			
			
			// 5.��Ӧ����ʱ����¼����
			pAcctBalanceInfo->m_lAcctBalanceID = lAcctBalanceId;
			
			// ��ѯ����ʱ�
			iRet = m_poSql->qryAcctBalance(lAcctBalanceId,pAcctBalanceInfo);
			
			
			// ����ʱ����
			iRet = m_poSql->updateAcctBalance(sStructPaymentSeqInfo,pAcctBalanceInfo);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "����ʱ�������ʧ��");
				throw iRet;
			}	
		}
		*/
	}
	catch(TTStatus oSt) 
	{
		iRet=REVER_BALANCE_PAYOUT_TT_ERR;
		__DEBUG_LOG1_(0, "PaymentInfo::QryPayoutInfo oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "PaymentInfo::QryPayoutInfo value=%d",value);
	}
	
	FreeObject(pAcctBalanceInfo);
	FreeObject(pBalanceSourceInfo);
	
	return iRet;
}


