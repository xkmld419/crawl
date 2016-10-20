#ifndef __DEPOSIT_INFOR_H_INCLUDED_
#define __DEPOSIT_INFOR_H_INCLUDED_

#include "abmpaycmd.h"
#include "PaymentInfoSql.h"
#include "AllocateBalanceSql.h"
#include "DccOperation.h"
#include "DccOperationSql.h"

class ABMException;
/*
*  ��ֵ����������
*/

class DepositInfo
{

public:

    DepositInfo();
    ~DepositInfo();
    int deal(ABMCCR *pCCR, ABMCCA *pCCA);
    int _deal(DepositServCondOne *&pInOne,vector<DepositServCondTwo *>&vInTwo);
    
    // ������ֵ
    int _dealIn(DepositServCondOne *&pInOne,vector<DepositServCondTwo *>&vInTwo);
    //���Ͽ���
    int CreateServAcctInfo(DepositServCondOne *&pInOne,ServAcctInfoSeq &oServAcctInfoSeq);
    //���ϴ���,
    int UpdateAcctInfo(DepositServCondOne *&pInOne,ServAcctInfoSeq &oServAcctInfoSeq);

private:

    PaymentInfoSql *m_poSql;
    
    DepositInfoCCR *m_poUnpack;
    DepositInfoCCA *m_poPack;

    AllocateBalanceSql  *pAllocateBalanceSql;
    DccOperation *m_dccOperation;
    
};

#endif/*__DEPOSIT_INFOR_H_INCLUDED_*/


