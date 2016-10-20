#ifndef DISCT_QUERY_H
#define DISCT_QUERY_H

/*
* Copyright (c) 2007,
* All rights reserved.
*
* �ļ����ƣ� DisctQuery.h
* �ļ���ʶ��
* ժ    Ҫ�� ��ѯ�Żݹ켣�ӿڣ���ѯ����Ĵ����������
*
* ��ǰ�汾��1.0
* ��    �ߣ��Ű�Ʒ
* ������ڣ�2009�� �� ��
*
*/


#include <vector>
#include "OfferAccuMgr.h"
#include "ParamDefineMgr.h"
#include "ProdOfferAggr.h"
#include "UserInfoReader.h"
#include "HashList.h"
#include "FreeAccuQuery.h"

using namespace std;


struct ItemNameData
{
    char m_sName[128];
    int  m_iKeyID;
};


class DisctQueryResult {
public:
    char m_sOfferName[128];     //��Ʒ����
    char m_sElement[128];       //��Ա����
    char m_sAccNBR[128];      //����
    char m_sAcctGroupName[128];  ///��Ŀ������
    char m_sAcctItemName[128];  ///��Ŀ����
    char m_sQueryName[128];        //��ѯ����
    int  m_iQueryID;              //����
    int  m_iQueryFlag;            //��ѯ������־
    int  m_iOfferID;              //��ƷID
    int  m_iItemGroupID;         ///��Ŀ��
    int  m_iAcctItemTypeID;      ///��Ŀ����
    long m_lValue;               //Ǯ
    long m_lItemDisctSeq;        //�������
};




class DisctQuery{
public:

    //sDate�������������ȡsysdate������vector����ļ�¼��,�������ֵС��0��ʾ����
    int query(vector<DisctQueryResult> &vResult,
            long lServID, char *sDate=0);

    //sDate�������������ȡsysdate������vector����ļ�¼��,�������ֵС��0��ʾ����
    int queryDB(vector<DisctQueryResult> &vResult,
            long lServID, char *sDate=0);
public:
    DisctQuery();
    ~DisctQuery();


private:
    void load();
    void loadItemName(const char * sTableName, const char * sLoadSql,  
            ItemNameData **pList, HashList<ItemNameData *> **pIndex);
    void transRes( DisctQueryResult *pResDisct, ProdOfferTotalData * pData );
    
private:

    int m_iBillingCycleID;
    long m_lServID;
    char m_sDate[16];

    OfferInsQO   *m_poOfferInstBuf;
    BillingCycleMgr * m_poBCM;
    ProdOfferDisctAggr * m_poProdOfferDisctAggr;

    HashList<ItemNameData *> *m_poOfferNameIndex;
    HashList<ItemNameData *> *m_poAcctItemNameIndex;
    HashList<ItemNameData *> *m_poAcctGroupNameIndex;

    ItemNameData * m_poOfferNameList;
    ItemNameData * m_poAcctItemNameList;
    ItemNameData * m_poAcctGroupNameList;

    vector<TableData> vet85;
    vector<TableData> vet87;
    
};

/*
class ProdOfferTotalData
{
    public:
     long   m_lItemDisctSeq;
     long	m_lProdOfferID; -------ע�⣬�ڴ�����ŵ���offer_inst_id���Żݹ켣��ѯ�����offerid
     int	m_iBillingCycleID;
     int    m_iProdOfferCombine;
     int    m_iPricingMeasure;
     int    m_iItemGroupID;
     int    m_iInType;              //�������ͣ�15����Աʵ����2���û���12����Ʒ��3���ͻ���16����Ʒ�µĳ�Ա
     long   m_lInValue;
     long   m_lServID;
     int	m_iAcctItemTypeID;		    //��Ŀ����
     int    m_iValue;
     bool   m_bInOutType;               //����������ͣ�true:���룬false�������
     bool	m_bNewRec;				    //�Ƿ�Ϊ������¼(false:������¼,true:���ϼ�¼���ۻ�)
}
*/

#endif

