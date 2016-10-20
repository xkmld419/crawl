#include "abmcmd.h"
#include "dcc_ra.h"
#include "MessageQueue.h"
#include "ABMException.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

    ABMException oExp;

    // ��������CCR
    R_AllocateBalanceSvc oSvcCCR;
    R_RequestDebitAcctItem oAcctItemCCR;
    
    // ������ӦCCA
    A_AllocateBalanceSvc oSvcCCA;	
    A_ResponseDebitAcctItem oAcctItemCCA;
    
    AllocateBalanceCCR oCCR;
    AllocateBalanceCCA oCCA;
    
    oCCR.clear();
    oCCA.clear();
    // ׼������
    strcpy(oSvcCCR.m_requestId, "6684801");
    strcpy(oSvcCCR.m_servNbr, "139888888");
    oSvcCCR.m_nbrType = 1;
    oSvcCCR.m_servAttr = 1;
    strcpy(oSvcCCR.m_areaCode, "025");
    oSvcCCR.m_staffId = 1;
    strcpy(oSvcCCR.m_deductDate, "2011-5-10");
    strcpy(oSvcCCR.m_abmId, "1");
    

    // ���CCR - ��һ��
    if (!oCCR.addAllocateBalanceSvc(oSvcCCR)) {
        cout<<"pack failed!"<<endl;
        return -1;
    }
    
    // ׼������
    strcpy(oAcctItemCCR.m_billingCycle, "201105");
    strcpy(oAcctItemCCR.m_acctItemTypeId, "1");
    oAcctItemCCR.m_balanceUnit = 0;
    oAcctItemCCR.m_deductAmount = 10;
    oAcctItemCCR.m_chargeType = 0;
    
    // ���CCR - �ڶ���
    if (!oCCR.addRequestDebitAcctItem(oAcctItemCCR)) {
        cout<<"pack failed!"<<endl;
        return -1;
    }

    
    // ��Ϣ����
    MessageQueue *pMQ = new MessageQueue(11001);
    if (pMQ->open(oExp, true, true) != 0) {
        cout<<"pMQ->open failed!"<<endl;
        return -1;
    }
    
    MessageQueue *pMQ2 = new MessageQueue(11002);
    if (pMQ2->open(oExp, true, true) != 0) {
        cout<<"pMQ->open failed!"<<endl;
        return -1;
    }
    
    // ����CCR�����
    if (pMQ->Send((void *)&oCCR, oCCR.m_iSize) <= 0) {
        cout<<"pMQ->Send failed!"<<endl;
        return -1;
    }

    // ����CCA��Ӧ��
    if (pMQ2->Receive((void *)&oCCA, sizeof(oCCA), 0) <= 0) {
        cout<<"pMQ->Receive failed!"<<endl;
        return -1;
    }


    vector<A_AllocateBalanceSvc*> vSvc;
    vector<A_ResponseDebitAcctItem*> vData;
    
    // ȡ��һ��CCA��Ϣ
    if (oCCA.getResBalanceSvcData(vSvc) <= 0) {
        cout<<"oCCA->getResBalanceSvcData failed!"<<endl;
        return -1;
    }
    
    // ȡ�ڶ���CCA��Ϣ
    if (oCCA.getAcctBalanceData(vSvc[0],vData) <= 0) {
        cout<<"oCCA->getAcctBalanceData failed!"<<endl;
        return -1;
    }
    
    /* ��CCA��ȡ����
    for (int i=0; i<vRes.size(); ++i) {
        cout<<vRes[i]->m_sResID<<endl;
        cout<<vRes[i]->m_iRltCode<<endl;

        vData.clear();
        if (oCCA.getServData(vRes[i], vData) <= 0) {
            cout<<"getServData failed!"<<endl;
            return -1;
        } 

        for (int j=0; j<vData.size(); ++j) {
            cout<<"m_lServID="<<vData[j]->m_lServID<<endl;
            cout<<"m_lLocServID="<<vData[j]->m_lLocServID<<endl;
            cout<<"m_lAcctID="<<vData[j]->m_lAcctID<<endl;
            cout<<"m_uiProdID="<<vData[j]->m_uiProdID<<endl;
            cout<<"m_uiAreaCode="<<vData[j]->m_uiAreaCode<<endl;
            cout<<"m_sAccNbr="<<vData[j]->m_sAccNbr<<endl;
            cout<<"m_sStateDate="<<vData[j]->m_sStateDate<<endl;
            cout<<"m_hServState="<<vData[j]->m_hServState<<endl;
            cout<<"m_hPayFlag="<<vData[j]->m_hPayFlag<<endl;
        }
    }
    */
    
    return 0;
}

