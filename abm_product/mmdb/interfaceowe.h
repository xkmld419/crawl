#ifndef interface_H_HEADER_INCLUDED
#define interface_H_HEADER_INCLUDED

#ifdef ABM_BALANCE
#include "abmclnt.h"
#include "abmaccess.h"   //����ͷ�ļ�
#endif

#include "AcctItemAccu.h"
#include "Environment.h"
#include "TOCIQuery.h"
#include "Log.h"

#include <iostream>

using namespace std;

class AbmInterfaceManage
{
#ifdef ABM_BALANCE
public:
    AbmInterfaceManage();
    ~AbmInterfaceManage();
    
    //��ȡ�ӿڳ�ʼ������ֵ,0��ʾ�ɹ�
    int getInitResult();
    
    //ʵʱ�۷�,0��ʾ�ɹ�
    int pachCharge(DeductBalanceR &oData, vector<DeductAcctItem*> &vecBal);
    int transCharge();
    int queryBalance(RealDeductA &queryreal);

    //����ʵʱ�۷�,0��ʾ�ɹ�
    int resetBatCharge();
    int pachBatCharge(DeductBalanceR &oData, vector<DeductAcctItem*> &vecBal);
    int transBatCharge();
    int queryBatBalance(RealDeductA &queryreal, int ioffset);
    long getBatCmdNum();
    long getBatResultNum();
    
    //��ȡ�ϵ�,0��ʾ�ɹ�
    int getBreakPoint(long lFileID, long &loffset);

    //��ѯʵʱ�۷ѽ��,0��ʾ�ɹ�
    int querypayresult(long lAcctID, long lServID, PayResultQryA &payresult);

    //��ѯ�ʻ������ϸ,0��ʾ�ɹ�
    int queryBalanceDetail(long lAcctID, vector<BalanceInfoS *> &vecBalanceInfoS);
    
    //��ѯ�ʻ�ʵʱǷ����ϸ,0��ʾ�ɹ�
    int queryCurAggrDetail(long lAcctID, vector<AcctItemInfoHB *> &vecAcctItemInfoHB);
    
    //����֪ͨabm,0��ʾ�ɹ�
    int modifyAcctOld(long lAcctID, long lServID, long lTransID, int iBillingCycleID, vector<AcctAggrHB01 *> &vecData);
    int modifyAcctNew(long lAcctID, long lServID, long lTransID, int iBillingCycleID, int iBillingMode, vector<AcctAggrHB01 *> &vecData);

    //�û�Ԥ�󸶷���ת��֪ͨabm,0��ʾ�ɹ�
    int transBillingMode(long lAcctID, long lServID, long lTypeID);

    //������
    bool dealacct_item_withoutabm(char const *sTable);
    bool dealacct_item_withabm(char const *sTable);
    

private:
   ABM *pInst;   //ABM�ӿڳ�ʼ��
   
   int m_iInitRet;          //�ӿڳ�ʼ������ֵ
 
   //ʵʱ�۷�
   RealDeductCmd qryCmd;    // ʵʱǷ������
   ABMResult qryRlt;        //��ѯ��� 
   RealDeductResult *prlt;  //�۷Ѳ�ѯ��Ϣ
 
   //ʵʱ���ò�ѯ����
   RealQueryCmd qryCmd1;   // ʵʱ���ò�ѯ��������
   ABMResult qryRlt1;      //��ѯ���2
   RealQueryResult *prlt1; //ʵʱ���ò�ѯ���Ƚ��
 
   //�۷ѽ��ʵʱ��ѯ
   PayResultQryCmd qryCmd2;    // �۷ѽ��ʵʱ��ѯ����
   ABMResult qryRlt2;          //��ѯ��� 
   PayResultQryResult *prlt2;  //�۷ѽ��ʵʱ��ѯ��Ϣ
   
	 //��ѯ�ʻ�ʵʱ������ϸ
	 QueryAcctItemCmd qaiCmd;        //��ѯ����
	 QueryAcctItemResult qaiResult;  //��ѯ���
   
   //��ѯ�ʻ������ϸ
   QueryBalance qb;
	 QryBalanceSCmd qbSCmd;         //��ѯ����
	 QryBalanceSResult qbSResult;   //��ѯ���

   //����֪ͨABM
   ModifyAcctOldCmd maoCmd;       //��ѯ����
   ModifyAcctOldResult maoResult; //��ѯ���
   
   ModifyAcctNewCmd manCmd;       //��ѯ����
   ModifyAcctNewResult manResult; //��ѯ���
   
   //�û�Ԥ�󸶱仯֪ͨabm
   StatusTransCmd stCmd;          //��ѯ����
   StatusTransResult stResult;    //��ѯ���

   //����ʵʱ�۷�
   BatRealDeductCmd      brdCmd;    //��ѯ����
   BatRealDeductResult   brdResult; //��ѯ���
#endif
};


#endif /* interface_H_HEADER_INCLUDED */

