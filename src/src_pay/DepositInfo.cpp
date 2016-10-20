#include "DepositInfo.h"
#include "PaymentInfoSql.h"
#include "errorcode.h"
#include "ABMException.h"
#include "LogV2.h"


static vector<DepositServCondOne *> g_vServInfo;
static vector<DepositServCondTwo *> g_vServData;
static vector<DepositServCondOne *>::iterator g_itrInfo;
static vector<DepositServCondTwo *>::iterator g_itrData;
vector<long>vAcctBalanceID;

DepositInfo::DepositInfo()
{
    m_poSql = new PaymentInfoSql;
    pAllocateBalanceSql =new AllocateBalanceSql;
    m_poUnpack = NULL;
    m_poPack = NULL;
    m_dccOperation = new DccOperation;
}

DepositInfo:: ~DepositInfo()
{
    FreeObject(m_poSql);
    FreeObject(pAllocateBalanceSql);
    FreeObject(m_dccOperation );
}

int DepositInfo::deal(ABMCCR *pCCR, ABMCCA *pCCA)
{
    int iRet;
    string sEffDate,sExDate;
    PaymentInfoALLCCR pPaymentInfoCCR={0};
    vAcctBalanceID.clear();
    
    m_poUnpack = (DepositInfoCCR *)pCCR;
    m_poPack = (DepositInfoCCA *)pCCA;
    m_poPack->clear();
    g_vServInfo.clear();
    g_vServData.clear();
    ResponsDepositOne oRes={0};
    ResponsDepositTwo pDepositTwo={0};
    vector<AcctBalanceInfo*>vAcctBalanceInfo;
    vAcctBalanceInfo.clear();
    try 
    {
    
	    __DEBUG_LOG0_(0, "vc��ֵ ��Ϣ��");
	    //��Ϣ���
	    if ((iRet=m_poUnpack->getDepositOneCCR(g_vServInfo)) == 0)
	    {
		__DEBUG_LOG0_(0, "vc��ֵ::deal ��Ϣ��ΪNULL");
		m_poPack->setRltCode(ECODE_NOMSG);
		throw DEPOSIT_UNPACK_ERR;
	    }
	    else if (iRet < 0) 
	    {
		__DEBUG_LOG0_(0, "vc��ֵ::deal ��Ϣ�������!");
		m_poPack->setRltCode(ECODE_UNPACK_FAIL);
		throw DEPOSIT_UNPACK_ERR;
	    }
	    for(unsigned  int i=0;i<g_vServInfo.size();i++)
	    {
		    g_vServInfo[i]->m_iBonusStartTime=0;
		    g_vServInfo[i]->m_iBonusExpirationTime=0;
		    /*
		    if(strlen(g_vServInfo[i]->m_sBalanceType)<=0)
		    	memset(g_vServInfo[i]->m_sBalanceType,'\0',sizeof(g_vServInfo[i]->m_sBalanceType));
		    */
		    if(g_vServInfo[i]->m_iBalanceType<=0)
			    g_vServInfo[i]->m_iBalanceType=10;
				
		    __DEBUG_LOG2_(0,"vc��ֵ���:[%d].m_sReqSerial=%s",i,g_vServInfo[i]->m_sReqSerial );
		    //__DEBUG_LOG2_(0,"vc��ֵ���:[%d].m_sBalanceType=%s",i, g_vServInfo[i]->m_sBalanceType);
		    __DEBUG_LOG2_(0,"vc��ֵ���:[%d].m_sBalanceType=%d",i, g_vServInfo[i]->m_iBalanceType);
		    __DEBUG_LOG2_(0,"vc��ֵ���:[%d].m_iDestinationBalanceUnit=%d",i,g_vServInfo[i]->m_iDestinationBalanceUnit );
		    __DEBUG_LOG2_(0,"vc��ֵ���:[%d].m_iVoucherValue=%d",i, g_vServInfo[i]->m_iVoucherValue);
		    __DEBUG_LOG2_(0,"vc��ֵ���:[%d].m_iProlongDays=%d",i, g_vServInfo[i]->m_iProlongDays );

		    __DEBUG_LOG2_(0,"vc��ֵ���:[%d].m_sDestinationType=%s",i, g_vServInfo[i]->m_sDestinationType);
		    __DEBUG_LOG2_(0,"vc��ֵ���:[%d].m_sDestinationAccount=%s",i,g_vServInfo[i]->m_sDestinationAccount );
		    __DEBUG_LOG2_(0,"vc��ֵ���:[%d].m_iDestinationAttr=%d",i, g_vServInfo[i]->m_iDestinationAttr);
		    __DEBUG_LOG2_(0,"vc��ֵ���:[%d].m_sAreaCode=%s",i, g_vServInfo[i]->m_sAreaCode);
	    }

	    for(g_itrInfo=g_vServInfo.begin();g_itrInfo!=g_vServInfo.end();g_itrInfo++)
	    {
        	iRet=m_poUnpack->getDepositTwoCCR(*g_itrInfo,g_vServData);
        	__DEBUG_LOG1_(0, "vc��ֵ�ڶ��������::_deal iRet=%d", iRet);
        	if ( iRet>= 0)
		{
			iRet=_deal(*g_itrInfo,g_vServData);
			if(iRet!=0)
				throw iRet;
		}
		else
		{
			throw iRet;
		}
	    }
	    __DEBUG_LOG0_(0, "vc��ֵ�ɹ�:: ��1��!");
	    oRes.m_iRltCode=0;
	    strcpy(oRes.m_sReqSerial,g_vServInfo[0]->m_sReqSerial);
	    if (!m_poPack->addRespDepositOneCCA(oRes)) 
	    {
		__DEBUG_LOG0_(0, "vc��ֵ:: _deal m_poPack ��1��ʧ��!");
		m_poPack->setRltCode(ECODE_PACK_FAIL);
		throw DEPOSIT_PACK_ERR;
	    }
	    __DEBUG_LOG1_(0, "vc��ֵ�˻���ϸ����:[%d]",vAcctBalanceID.size());
    	    AcctBalanceInfo *m_pAcctBalanceInfo=new AcctBalanceInfo;
	    if(!m_pAcctBalanceInfo)
	    {
	    	__DEBUG_LOG0_(0, "vc��ֵ�����ڴ����");
	    	throw ECODE_MEM_MALLOC_FAIL;
	    }
	    for(unsigned int ii=0;ii<vAcctBalanceID.size();ii++)
	    {
		m_pAcctBalanceInfo->clear();
	    	__DEBUG_LOG2_(0, "vc��ֵ�˻���ϸ:[%d][%ld]",ii,vAcctBalanceID[ii]);
	    	//��ѯ�˻���ϸ
		iRet=0;
	    	iRet=m_poSql->qryAcctBalance(vAcctBalanceID[ii],m_pAcctBalanceInfo);
		if(iRet!=1)
		{
			__DEBUG_LOG1_(0, "��ѯ�˻���ϸ,ʧ��=[%d]",iRet);
			throw iRet;
		}
		sEffDate.clear();
		sExDate.clear();
		memset(&pDepositTwo,0,sizeof(ResponsDepositTwo));
		sEffDate=m_pAcctBalanceInfo->m_sEffDate;
		sExDate=m_pAcctBalanceInfo->m_sExDate;

		pDepositTwo.m_iDestinationBalance=m_pAcctBalanceInfo->m_lBalance;
		//sprintf(pDepositTwo.m_sBalanceType,"%d",m_pAcctBalanceInfo->m_lBalanceTypeID);
		pDepositTwo.m_iBalanceType=m_pAcctBalanceInfo->m_lBalanceTypeID;
		pDepositTwo.m_iDestinationBalanceUnit=0;
		pDepositTwo.m_iDestinationEffectiveTime=atoi(sEffDate.substr(0,8).c_str());
		pDepositTwo.m_iDestinationExpirationTime=atoi(sExDate.substr(0,8).c_str());

		__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_iDestinationBalance ��2��=[%d]!",pDepositTwo.m_iDestinationBalance);
		//__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_sBalanceType ��2��=[%s]!",pDepositTwo.m_sBalanceType);
		__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_iBalanceType ��2��=[%d]!",pDepositTwo.m_iBalanceType);
		__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_iBalanceUnit ��2��=[%d]!",pDepositTwo.m_iDestinationBalanceUnit);
		__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_iEff ��2��=[%d]!",pDepositTwo.m_iDestinationEffectiveTime);
		__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_iExp ��2��=[%d]!",pDepositTwo.m_iDestinationExpirationTime);

		if (!m_poPack->addRespDepositTwoCCA(pDepositTwo))
		{
			__DEBUG_LOG0_(0, "vc��ֵ���ذ�:: _deal m_poPack ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw  DEPOSIT_PACK_ERR;
		}
		iRet=0;
	    }

	    /*
	    strcpy(pPaymentInfoCCR.m_sDestinationAccount,g_vServInfo[0]->m_sDestinationAccount);
	    //��ѯ�˻���ϸ
	    iRet=m_poSql->QueryAcctBalanceInfo(pPaymentInfoCCR,vAcctBalanceInfo);
	    if(iRet!=0)
	    {
			__DEBUG_LOG1_(0, "��ѯ�˻���ϸ,ʧ��=[%d]",iRet);
			throw iRet;
	    }
	    for(vector<AcctBalanceInfo*>::iterator it=vAcctBalanceInfo.begin();it!=vAcctBalanceInfo.end();it++)
	    {
		sEffDate.clear();
		sExDate.clear();
		memset(&pDepositTwo,0,sizeof(ResponsDepositTwo));
		sEffDate=(*it)->m_sEffDate;
		sExDate=(*it)->m_sExDate;

		pDepositTwo.m_iDestinationBalance=(*it)->m_lBalance;
		sprintf(pDepositTwo.m_sBalanceType,"%d",(*it)->m_lBalanceTypeID);
		pDepositTwo.m_iDestinationBalanceUnit=0;
		pDepositTwo.m_iDestinationEffectiveTime=atoi(sEffDate.substr(0,8).c_str());
		pDepositTwo.m_iDestinationExpirationTime=atoi(sExDate.substr(0,8).c_str());

		__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_iDestinationBalance ��2��=[%d]!",pDepositTwo.m_iDestinationBalance);
		__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_sBalanceType ��2��=[%s]!",pDepositTwo.m_sBalanceType);
		__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_iBalanceUnit ��2��=[%d]!",pDepositTwo.m_iDestinationBalanceUnit);
		__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_iEff ��2��=[%d]!",pDepositTwo.m_iDestinationEffectiveTime);
		__DEBUG_LOG1_(0, "vc��ֵ���ذ�:: m_iExp ��2��=[%d]!",pDepositTwo.m_iDestinationExpirationTime);

		if (!m_poPack->addRespDepositTwoCCA(pDepositTwo))
		{
			__DEBUG_LOG0_(0, "vc��ֵ���ذ�:: _deal m_poPack ���ʧ��!");
			m_poPack->setRltCode(ECODE_PACK_FAIL);
			throw  DEPOSIT_PACK_ERR;
		}
	   }
	   */

    }
    catch(TTStatus oSt) 
    {
        __DEBUG_LOG1_(0, "vc��ֵʧ��:: ʧ����Ϣ=%s", oSt.err_msg);
    }
    catch(int &value)
    {
	iRet=value;
        __DEBUG_LOG1_(0, "vc��ֵʧ��:: ʧ����Ϣ value=%d", value);
	
    }
    if(iRet!=0)
    {
	     __DEBUG_LOG0_(0, "��ֵʧ��!");
	    oRes.m_iRltCode=iRet;
	    strcpy(oRes.m_sReqSerial,g_vServInfo[0]->m_sReqSerial);
	    if (!m_poPack->addRespDepositOneCCA(oRes)) 
	    {
		__DEBUG_LOG0_(0, "DepositInfo:: _deal m_poPack ���ʧ��!");
		m_poPack->setRltCode(ECODE_PACK_FAIL);
	    }
    }
    for(vector<AcctBalanceInfo*>::iterator it=vAcctBalanceInfo.begin();it!=vAcctBalanceInfo.end();it++)
    {
	delete *it;
    }
    vAcctBalanceInfo.clear();
    
    return iRet;
}

int DepositInfo::_deal(DepositServCondOne *&pInOne,vector<DepositServCondTwo *>&vInTwo)
{
	//��ֵ������
	//У�������Ƿ���ڣ����ȿ�����,��ֵ
	int iRet=0;
	bool bState=false;
	ServAcctInfo oServAcct={0};
	ServAcctInfoSeq oServAcctInfoSeq={0};
	ServAcctInfoSeq oSendSeq={0};
	DepositServCondOne   *pInTwo=NULL;
	PaymentInfoALLCCR pPaymentInfoCCR={0};
	StructPaymentSeqInfo  sStructPaymentSeqInfo={0};
	
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
			__DEBUG_LOG1_(0, "��ֵ����DCCͷ��=[%d]",iRet);
			throw iRet;
		}
		//��ͨ��ֵ
		//��ѯ�û���Ϣ
		iRet=pAllocateBalanceSql->preQryServAcctInfo(pInOne->m_sDestinationAccount,2,oServAcct,oExp);
		switch(iRet)
		{
			case 1://������
				{
					__DEBUG_LOG0_(0, "û�����ϣ���ʼ�����û���Ϣ,Ȼ���ֵ  ");
					iRet=CreateServAcctInfo(pInOne,oServAcctInfoSeq);
					if(iRet!=0)
					{
						__DEBUG_LOG1_(0, "�����û���Ϣ  ʧ��==[%d]",iRet);
						throw iRet;
					}
				}
				break;
			case 0://������
				{
					__DEBUG_LOG0_(0, "�����û����ϣ���ֵ");
					oServAcctInfoSeq.m_lServID=oServAcct.m_servId;
					oServAcctInfoSeq.m_lAcctID=oServAcct.m_acctId;
					iRet=UpdateAcctInfo(pInOne,oServAcctInfoSeq);
					if(iRet!=0)
					{
						__DEBUG_LOG1_(0, "�����û����ϣ���ֵ  ʧ��==[%d]",iRet);
						throw iRet;
					}
				}
				break;
			default://����
				{
					__DEBUG_LOG1_(0, "��ѯ�û����ϳ���=[%d]",iRet);
					throw iRet;
				}
		}
	
		//��ѯҵ����ˮ
		bState=false;
		iRet=m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bState);
		if(iRet!=0)
        	{
			__DEBUG_LOG0_(0, "��ѯҵ����ˮʧ��  error");
			throw iRet;
        	}
		//��ѯ��������
		bState=true;
		iRet=m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bState);
		if(iRet!=0)
        	{
			__DEBUG_LOG0_(0, "��ѯ�������г���  error");
			throw iRet;
        	}
		//��¼ҵ���
		strcpy(pPaymentInfoCCR.m_sOperateSeq,pInOne->m_sReqSerial);
		strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0001");
		pPaymentInfoCCR.m_lServID=oServAcctInfoSeq.m_lServID;
		sStructPaymentSeqInfo.m_lRbkPaymentSeq=0;
		strcpy(pPaymentInfoCCR.m_sSpID,"1");
		strcpy(pPaymentInfoCCR.m_sServicePlatformID,"1");
		strcpy(pPaymentInfoCCR.m_sOrderID,"1");
		strcpy(pPaymentInfoCCR.m_sDescription,"VC��ֵ");
		strcpy(pPaymentInfoCCR.m_sMicropayType,"0");
		strcpy(pPaymentInfoCCR.m_sOrderState,"C0C");
		pPaymentInfoCCR.m_lPayAmount=pInOne->m_iVoucherValue;
		iRet=m_poSql->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
		if(iRet!=0)
        	{
			__DEBUG_LOG0_(0, "��¼ҵ����ˮ�����!");
			throw iRet;
        	}
		
		sStructPaymentSeqInfo.m_lAcctBalanceID=oServAcctInfoSeq.m_lAcctBalanceID;
		strcpy(pInOne->m_sSourceType,"5VA");
		strcpy(pInOne->m_sSourceDesc,"VC��ͨ��ֵ");
		strcpy(pInOne->m_sAllowDraw,"ADY");
		//��¼��Դ��
		iRet=m_poSql->InsertBalanceSource(pInOne,sStructPaymentSeqInfo);
		if(iRet!=0)
        	{
			__DEBUG_LOG0_(0, "��¼�����Դ��ʧ��! ");
			throw iRet;
        	}
		vAcctBalanceID.push_back(sStructPaymentSeqInfo.m_lAcctBalanceID);

		//�Żݳ�ֵ,ֻ��¼�˱�+��Դ��
		for(vector<DepositServCondTwo *>::iterator iter=vInTwo.begin();iter!=vInTwo.end();iter++)
		{

			__DEBUG_LOG0_(0, "��ֵ�����Ż�,��ʼ�Żݳ�ֵ");
			__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_iBonusUnit=[%d]",(*iter)->m_iBonusUnit);
			//__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_sBalanceType=[%s]",(*iter)->m_sBalanceType);
			__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_iBalanceType=[%d]",(*iter)->m_iBalanceType);
			__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_iBonusStartTime=[%d]",(*iter)->m_iBonusStartTime);
			__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_iBonusExpirationTime=[%d]",(*iter)->m_iBonusExpirationTime);
			__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_iBonusAmount=[%d]",(*iter)->m_iBonusAmount);
			pInTwo=new DepositServCondOne;
			pInTwo->m_iDestinationBalanceUnit=(*iter)->m_iBonusUnit;//�Żݶλ��0���֣�1����
			pInTwo->m_iVoucherValue=(*iter)->m_iBonusAmount;//������Żݽ��
			//strcpy(pInTwo->m_sBalanceType,(*iter)->m_sBalanceType);//�������
			pInTwo->m_iBalanceType=(*iter)->m_iBalanceType;//�������
			pInTwo->m_iBonusStartTime=(*iter)->m_iBonusStartTime;//�Ż���Ч����ʼʱ�䣬yyyymmdd
			pInTwo->m_iBonusExpirationTime=(*iter)->m_iBonusExpirationTime;//�Ż���Ч���ս�ʱ�䣬yyyymmdd
			strcpy(pInTwo->m_sDestinationAccount,pInOne->m_sDestinationAccount);
			iRet=UpdateAcctInfo(pInTwo,oServAcctInfoSeq);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "�����û��˻�ʧ��  ");
				throw iRet;
			}
			strcpy(pInTwo->m_sSourceType,"5VF");
			strcpy(pInTwo->m_sSourceDesc,"VC�Żݳ�ֵ");
			strcpy(pInTwo->m_sAllowDraw,"ADN");
			bState=false;
			memset(&oSendSeq,0,sizeof(ServAcctInfoSeq));
			iRet=m_poSql->QueryServAcctSeq(oSendSeq,bState);
			if(iRet!=0)
			{
				__DEBUG_LOG1_(0, "��ѯ�û�����ʧ��=[%d]!",iRet);
				throw iRet;
			}
			sStructPaymentSeqInfo.m_lBalanceSourceSeq=oSendSeq.m_lBalanceSourceID;
			sStructPaymentSeqInfo.m_lAcctBalanceID=oServAcctInfoSeq.m_lAcctBalanceID;
			//��¼��Դ��
			iRet=m_poSql->InsertBalanceSource(pInTwo,sStructPaymentSeqInfo);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "��¼�����Դʧ��!");
				throw iRet;
			}
			FreeObject(pInTwo);
			vAcctBalanceID.push_back(sStructPaymentSeqInfo.m_lAcctBalanceID);
		}
		return 0;

		
	}
	catch(TTStatus oSt) 
	{
		iRet=DEPOSIT_TT_ERR;
		__DEBUG_LOG1_(0, "��ֵʧ�ܣ�������Ϣ=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "��ֵʧ�ܣ�������Ϣ=%d",value);
	}
	FreeObject(pInTwo);
	

	return iRet;
} 
int DepositInfo::CreateServAcctInfo(DepositServCondOne *&pInOne,ServAcctInfoSeq &oServAcctInfoSeq)
{
	int iRet;
	bool bState=true;
	try
	{
		__DEBUG_LOG0_(0, "���û�����,��ʼ�����û�����");
		//��ѯ����,��ʱserv_id�ȶ������д��� 
		iRet=m_poSql->QueryServAcctSeq(oServAcctInfoSeq,bState);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "��ѯ�û�����ʧ��=[%d]",iRet);
			throw iRet;
		}
		//��¼serv��
		iRet=m_poSql->InsertServInfo(oServAcctInfoSeq,pInOne);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "��¼�û���Ϣ��ʧ�� =[%d]",iRet);
			throw iRet;
		}
		//��¼acct��
		iRet=m_poSql->InsertAcctInfo(oServAcctInfoSeq,pInOne);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "��¼�˻���ʧ�� =[%d]",iRet);
			throw iRet;
		}
		//��¼serv_acct��
		iRet=m_poSql->InsertServAcctInfo(oServAcctInfoSeq,pInOne);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "��¼�û��˻���Ӧ��ϵ��ʧ��  =[%d]",iRet);
			throw iRet;
		}
		//��¼�û���Ϣ����
		iRet=m_poSql->InsertUserInfoAdd(oServAcctInfoSeq,pInOne);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "��¼�û���Ϣ����  =[%d]",iRet);
			throw iRet;
		}
		//�����û�֧������
		bState=true;
		iRet=m_poSql->InsertAttUserInfo(oServAcctInfoSeq,bState);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "�����û�֧������  =[%d]",iRet);
			throw iRet;
		}
		//�����û���ѯ����
		bState=false;
		iRet=m_poSql->InsertAttUserInfo(oServAcctInfoSeq,bState);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "�����û�֧������  =[%d]",iRet);
			throw iRet;
		}
		//��¼�û�������Ϣ����Ҫ��¼���ţ�
		iRet=m_poSql->InsertAppUserInfo(pInOne,oServAcctInfoSeq);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "�����û�����  =[%d]",iRet);
			throw iRet;
		}

		//��¼acct_balance��
		pInOne->m_iBonusStartTime=0;
		pInOne->m_iBonusExpirationTime=0;
		iRet=m_poSql->InsertAcctBalance(oServAcctInfoSeq,pInOne);
		if(iRet!=0)
		{
			__DEBUG_LOG1_(0, "��¼����˱���ʧ��  =[%d]",iRet);
			throw iRet;
		}
		return 0;
	}
	catch(TTStatus oSt) 
	{
		iRet=DEPOSIT_TT_ERR;
		__DEBUG_LOG1_(0, "�����û�����ʧ�� ,������ϢΪ=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "�����û�����ʧ�� ,������ϢΪ=%d",value);
	}

	return iRet;
}
int DepositInfo::UpdateAcctInfo(DepositServCondOne *&pInOne,ServAcctInfoSeq &oServAcctInfoSeq)
{
	int iRet;
	bool bState=false;
	long lDepositAmount=0;
	StructPaymentSeqInfo sStructPaymentSeqInfo={0};
	AcctBalanceInfo *pAcctBalanceInfo=NULL;
	try
	{
		//�����˱����Ͳ�ѯ�˱�
		pAcctBalanceInfo=new AcctBalanceInfo;
		if(!pAcctBalanceInfo)
		{
			__DEBUG_LOG0_(0, "�����ڴ�ʧ��");
			throw ECODE_MEM_MALLOC_FAIL;
		}
		/*
		if(strlen(pInOne->m_sBalanceType)<=0)
		{
			strcpy(pInOne->m_sBalanceType,"10");
		}
		*/
		iRet=m_poSql->QueryAcctBalanceSimple(pInOne,pAcctBalanceInfo);
		switch(iRet)
		{
			case 1://����
				{
					__DEBUG_LOG0_(0, "�����û�����˱�,����");
					oServAcctInfoSeq.m_lAcctBalanceID=pAcctBalanceInfo->m_lAcctBalanceID;
					lDepositAmount=pInOne->m_iVoucherValue;
					sStructPaymentSeqInfo.m_lPaymentAmount = (-1)*lDepositAmount;
					iRet=m_poSql->updateAcctBalance(sStructPaymentSeqInfo,pAcctBalanceInfo);
					if(iRet!=0)
					{
						__DEBUG_LOG1_(0, "�����û�����˱�ʧ��  =[%d]",iRet);
						throw iRet;
					}
				}
				break;
			case 0://��¼
				{
					__DEBUG_LOG0_(0, "�����û�����˱�����¼");
					iRet=m_poSql->QueryServAcctSeq(oServAcctInfoSeq,bState);
					if(iRet!=0)
					{
						__DEBUG_LOG1_(0, "��ѯ����˱�����,ʧ�� =[%d]",iRet);
						throw iRet;
					}
					iRet=m_poSql->InsertAcctBalance(oServAcctInfoSeq,pInOne);
					if(iRet!=0)
					{
						__DEBUG_LOG1_(0, "��¼����˱���ʧ��  =[%d]",iRet);
						throw iRet;
					}
				}
				break;
			default:
				{
					__DEBUG_LOG1_(0, "��ѯ����˱�,ʧ��[%d]",iRet);
					throw iRet;
				}
				break;
		}
		FreeObject(pAcctBalanceInfo);
		
		return 0;
	}
	catch(TTStatus oSt) 
	{
		iRet=DEPOSIT_TT_ERR;
		__DEBUG_LOG1_(0, "�����û����ϲ���¼��Ϣʧ��,������Ϣ=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "�����û����ϲ���¼��Ϣʧ��,������Ϣ=%d",value);
	}
	FreeObject(pAcctBalanceInfo);

	return iRet;
}

int DepositInfo::_dealIn(DepositServCondOne *&pInOne,vector<DepositServCondTwo *>&vInTwo)
{
	//��ֵ������
	//У�������Ƿ���ڣ����ȿ�����,��ֵ
	int iRet=0;
	bool bState=false;
	ServAcctInfo oServAcct={0};
	ServAcctInfoSeq oServAcctInfoSeq={0};
	ServAcctInfoSeq oSendSeq={0};
	DepositServCondOne   *pInTwo=NULL;
	PaymentInfoALLCCR pPaymentInfoCCR={0};
	StructPaymentSeqInfo  sStructPaymentSeqInfo={0};
	
	ABMException oExp;
	try 
	{
	
		//��ͨ��ֵ
		//��ѯ�û���Ϣ
		iRet=pAllocateBalanceSql->preQryServAcctInfo(pInOne->m_sDestinationAccount,2,oServAcct,oExp);
		switch(iRet)
		{
			case 1://������
				{
					__DEBUG_LOG0_(0, "û�����ϣ���ʼ�����û���Ϣ,Ȼ���ֵ  ");
					iRet=CreateServAcctInfo(pInOne,oServAcctInfoSeq);
					if(iRet!=0)
					{
						__DEBUG_LOG1_(0, "�����û���Ϣ  ʧ��==[%d]",iRet);
						throw iRet;
					}
				}
				break;
			case 0://������
				{
					__DEBUG_LOG0_(0, "�����û����ϣ���ֵ");
					oServAcctInfoSeq.m_lServID=oServAcct.m_servId;
					oServAcctInfoSeq.m_lAcctID=oServAcct.m_acctId;
					iRet=UpdateAcctInfo(pInOne,oServAcctInfoSeq);
					if(iRet!=0)
					{
						__DEBUG_LOG1_(0, "�����û����ϣ���ֵ  ʧ��==[%d]",iRet);
						throw iRet;
					}
				}
				break;
			default://����
				{
					__DEBUG_LOG1_(0, "��ѯ�û����ϳ���=[%d]",iRet);
					throw iRet;
				}
		}
	
		//��ѯҵ����ˮ
		bState=false;
		iRet=m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bState);
		if(iRet!=0)
        	{
			__DEBUG_LOG0_(0, "��ѯҵ����ˮʧ��  error");
			throw iRet;
        	}
		//��ѯ��������
		bState=true;
		iRet=m_poSql->QueryPaySeq(sStructPaymentSeqInfo,bState);
		if(iRet!=0)
        	{
			__DEBUG_LOG0_(0, "��ѯ�������г���  error");
			throw iRet;
        	}
		//��¼ҵ���
		strcpy(pPaymentInfoCCR.m_sOperateSeq,pInOne->m_sReqSerial);
		strcpy(pPaymentInfoCCR.m_sCapabilityCode,"0005");
		pPaymentInfoCCR.m_lServID=oServAcctInfoSeq.m_lServID;
		sStructPaymentSeqInfo.m_lRbkPaymentSeq=0;
		strcpy(pPaymentInfoCCR.m_sSpID,"1");
		strcpy(pPaymentInfoCCR.m_sServicePlatformID,"1");
		strcpy(pPaymentInfoCCR.m_sOrderID,"1");
		strcpy(pPaymentInfoCCR.m_sDescription,"������ֵ");
		strcpy(pPaymentInfoCCR.m_sMicropayType,"0");
		strcpy(pPaymentInfoCCR.m_sOrderState,"C0C");
		pPaymentInfoCCR.m_lPayAmount=pInOne->m_iVoucherValue;
		iRet=m_poSql->InsertPaymentInfo(pPaymentInfoCCR,sStructPaymentSeqInfo);
		if(iRet!=0)
        	{
			__DEBUG_LOG0_(0, "��¼ҵ����ˮ�����!");
			throw iRet;
        	}
		
		sStructPaymentSeqInfo.m_lAcctBalanceID=oServAcctInfoSeq.m_lAcctBalanceID;
		strcpy(pInOne->m_sSourceType,"5VB");
		strcpy(pInOne->m_sSourceDesc,"������ֵ");
		strcpy(pInOne->m_sAllowDraw,"ADY");
		//��¼��Դ��
		iRet=m_poSql->InsertBalanceSource(pInOne,sStructPaymentSeqInfo);
		if(iRet!=0)
        	{
			__DEBUG_LOG0_(0, "��¼�����Դ��ʧ��! ");
			throw iRet;
        	}

		//�Żݳ�ֵ,ֻ��¼�˱�+��Դ��
		for(vector<DepositServCondTwo *>::iterator iter=vInTwo.begin();iter!=vInTwo.end();iter++)
		{

			__DEBUG_LOG0_(0, "��ֵ�����Ż�,��ʼ�Żݳ�ֵ");
			__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_iBonusUnit=[%d]",(*iter)->m_iBonusUnit);
			//__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_sBalanceType=[%s]",(*iter)->m_sBalanceType);
			__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_iBalanceType=[%d]",(*iter)->m_iBalanceType);
			__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_iBonusStartTime=[%d]",(*iter)->m_iBonusStartTime);
			__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_iBonusExpirationTime=[%d]",(*iter)->m_iBonusExpirationTime);
			__DEBUG_LOG1_(0, "�Ż����� (*iter)->m_iBonusAmount=[%d]",(*iter)->m_iBonusAmount);
			pInTwo=new DepositServCondOne;
			pInTwo->m_iDestinationBalanceUnit=(*iter)->m_iBonusUnit;//�Żݶλ��0���֣�1����
			pInTwo->m_iVoucherValue=(*iter)->m_iBonusAmount;//������Żݽ��
			//strcpy(pInTwo->m_sBalanceType,(*iter)->m_sBalanceType);//�������
			pInTwo->m_iBalanceType=(*iter)->m_iBalanceType;//�������
			pInTwo->m_iBonusStartTime=(*iter)->m_iBonusStartTime;//�Ż���Ч����ʼʱ�䣬yyyymmdd
			pInTwo->m_iBonusExpirationTime=(*iter)->m_iBonusExpirationTime;//�Ż���Ч���ս�ʱ�䣬yyyymmdd
			strcpy(pInTwo->m_sDestinationAccount,pInOne->m_sDestinationAccount);
			iRet=UpdateAcctInfo(pInTwo,oServAcctInfoSeq);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "�����û��˻�ʧ��  ");
				throw iRet;
			}
			strcpy(pInTwo->m_sSourceType,"5VF");
			strcpy(pInTwo->m_sSourceDesc,"VC�Żݳ�ֵ");
			strcpy(pInTwo->m_sAllowDraw,"ADN");
			bState=false;
			memset(&oSendSeq,0,sizeof(ServAcctInfoSeq));
			iRet=m_poSql->QueryServAcctSeq(oSendSeq,bState);
			if(iRet!=0)
			{
				__DEBUG_LOG1_(0, "��ѯ�û�����ʧ��=[%d]!",iRet);
				throw iRet;
			}
			sStructPaymentSeqInfo.m_lBalanceSourceSeq=oSendSeq.m_lBalanceSourceID;
			sStructPaymentSeqInfo.m_lAcctBalanceID=oServAcctInfoSeq.m_lAcctBalanceID;
			//��¼��Դ��
			iRet=m_poSql->InsertBalanceSource(pInTwo,sStructPaymentSeqInfo);
			if(iRet!=0)
			{
				__DEBUG_LOG0_(0, "��¼�����Դʧ��!");
				throw iRet;
			}
			FreeObject(pInTwo);
		}
		return 0;

		
	}
	catch(TTStatus oSt) 
	{
		iRet=-1;
		__DEBUG_LOG1_(0, "��ֵʧ�ܣ�������Ϣ=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "��ֵʧ�ܣ�������Ϣ=%d",value);
	}
	FreeObject(pInTwo);
	

	return iRet;
} 



