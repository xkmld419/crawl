#include "abmpaycmd.h"
#include "MessageQueue.h"
#include "ABMException.h"
#include <iostream>
using namespace std;
int Deposit();
int ReverseDepositClient();

int main(int argc, char *argv[])
{

	int iRet=0;	
	string sFlag;
	sFlag.clear();
    	cout<<"������������ͣ�1��VC��ֵ��2����ֵ����:"<<endl;
	cin>>sFlag;
	if(!sFlag.compare("1"))
	{
		cout<<"��ʼ��ֵ!"<<endl;
		iRet=Deposit();
	}
	else if(!sFlag.compare("2"))
	{
		cout<<"��ʼ��ֵ����!"<<endl;
		iRet=ReverseDepositClient();
	}
	else
	{
		cout<<"���������������!"<<endl;
	}
	return 0;
    
}
int Deposit()
{
    string sAccNbr,sFlag;
    cout<<"�������ֵ����,������ϢĬ��:"<<endl;
    cin>>sAccNbr;
    cout<<"�Ƿ�����Ż�Y/N:"<<endl;
    cin>>sFlag;
    ABMException oExp;
    
    DepositServCondOne oCmd;
    strcpy(oCmd.m_sReqSerial, "98765432");
    strcpy(oCmd.m_sBalanceType, "2");
    oCmd.m_iDestinationBalanceUnit = 0;
    oCmd.m_iVoucherValue = 80;
    oCmd.m_iProlongDays = 30;
    strcpy(oCmd.m_sDestinationType , "5BC");
    strcpy(oCmd.m_sDestinationAccount ,sAccNbr.c_str());
    strcpy(oCmd.m_sAreaCode,"0514");
    oCmd.m_iDestinationAttr = 8;
    oCmd.m_iBonusStartTime = 20110520;
    oCmd.m_iBonusExpirationTime = 20110520;

    DepositServCondTwo oCmdTwo;
    oCmdTwo.m_iBonusUnit=0;
    oCmdTwo.m_iBonusAmount=3;
    strcpy(oCmdTwo.m_sBalanceType,"3");
    oCmdTwo.m_iBonusStartTime=20110101;
    oCmdTwo.m_iBonusExpirationTime=20110601;

    DepositInfoCCR oCCR;
    DepositInfoCCA oCCA;
    
    oCCR.clear();
    if (!oCCR.addDepositOneCCR(oCmd)) {
        cout<<"pack ��ͨ��ֵ failed!"<<endl;
        return -1;
    }
    if(!(sFlag.compare("Y"))||(!sFlag.compare("y")))
    {
	    cout<<"�����Ż�����"<<endl;
	    if (!oCCR.addDepositTwoCCR(oCmdTwo)) {
		cout<<"pack  �Ż� failed!"<<endl;
		return -1;
	    }
    }
    MessageQueue *pMQ = new MessageQueue(11005);
    if (pMQ->open(oExp, true, true) != 0) {
        cout<<"pMQ->open failed!"<<endl;
        return -1;
    }
    
    MessageQueue *pMQ2 = new MessageQueue(11006);
    if (pMQ2->open(oExp, true, true) != 0) {
        cout<<"pMQ->open failed!"<<endl;
        return -1;
    }
    
    if (pMQ->Send((void *)&oCCR, oCCR.m_iSize) <= 0) {
        cout<<"pMQ->Send failed!"<<endl;
        return -1;
    }

    if (pMQ2->Receive((void *)&oCCA, sizeof(oCCA), 0) <= 0) {
        cout<<"pMQ->Receive failed!"<<endl;
        return -1;
    }

    vector<ResponsDepositOne*> vRes;
    vector<ResponsDepositTwo*> vData;
    if (oCCA.getRespDepositOneCCA(vRes) <= 0) {
        cout<<"oCCA->getRespons failed!"<<endl;
        return -1;
    }
    for (int i=0; i<vRes.size(); ++i) {
        cout<<"ҵ������"<<vRes[i]->m_iRltCode<<endl;
        cout<<"������ˮ"<<vRes[i]->m_sReqSerial<<endl;
        vData.clear();
	if(vRes[i]->m_iRltCode==0)
	{
		if (oCCA.getRespDepositTwoCCA(vRes[i], vData) <= 0) {
		    cout<<"getServData failed!"<<endl;
		    return -1;
		} 

		for (int j=0; j<vData.size(); ++j) {
		    cout<<"m_iDestinationBalance="<<vData[j]->m_iDestinationBalance<<endl;
		    cout<<"m_sBalanceType="<<vData[j]->m_sBalanceType<<endl;
		    cout<<"m_iDestinationBalanceUnit="<<vData[j]->m_iDestinationBalanceUnit<<endl;
		    cout<<"m_iDestinationEffectiveTime="<<vData[j]->m_iDestinationEffectiveTime<<endl;
		    cout<<"m_iDestinationExpirationTime="<<vData[j]->m_iDestinationExpirationTime<<endl;
		   
		}
	}
    }
    return 0;
}
int ReverseDepositClient()
{
	string sAccNbr;
    cout<<"������ԭ��ֵ������ˮ��,������ϢĬ��:"<<endl;
    cin>>sAccNbr;
    ABMException oExp;
    
    ReverseDeposit oCmd;
    strcpy(oCmd.m_sReqSerial, "98765432");
    strcpy(oCmd.m_sPaymentID, sAccNbr.c_str());
    strcpy(oCmd.m_sDestinationType , "5BC");
    strcpy(oCmd.m_sDestinationAccount ,"18951765356");
    oCmd.m_iDestinationAttr = 8;

  
    ReverseDepositInfoCCR oCCR;
    ReverseDepositInfoCCA oCCA;
    
    oCCR.clear();
    if (!oCCR.addReverseDepositInfo(oCmd)) {
        cout<<"pack ��ֵ���� failed!"<<endl;
        return -1;
    }
   
    MessageQueue *pMQ = new MessageQueue(11005);
    if (pMQ->open(oExp, true, true) != 0) {
        cout<<"pMQ->open failed!"<<endl;
        return -1;
    }
    
    MessageQueue *pMQ2 = new MessageQueue(11006);
    if (pMQ2->open(oExp, true, true) != 0) {
        cout<<"pMQ->open failed!"<<endl;
        return -1;
    }
    
    if (pMQ->Send((void *)&oCCR, oCCR.m_iSize) <= 0) {
        cout<<"pMQ->Send failed!"<<endl;
        return -1;
    }

    if (pMQ2->Receive((void *)&oCCA, sizeof(oCCA), 0) <= 0) {
        cout<<"pMQ->Receive failed!"<<endl;
        return -1;
    }

    vector<ResponsReverseDeposit*> vRes;
    vRes.clear();
    if (oCCA.getRespReverseDepositCCA(vRes) <= 0) {
        cout<<"oCCA->getRespons failed!"<<endl;
        return -1;
    }
    for (int i=0; i<vRes.size(); ++i) {
        cout<<"ҵ������"<<vRes[i]->m_iRltCode<<endl;
        cout<<"������ˮ"<<vRes[i]->m_sReqSerial<<endl;
    }
    return 0;
}



