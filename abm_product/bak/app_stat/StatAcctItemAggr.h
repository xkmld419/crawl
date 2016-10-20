/*VER: 1*/ 
#ifndef _STAT_ACCT_ITEM_AGGR_H_
#define _STAT_ACCT_ITEM_AGGR_H_

/////////////////////////////
//
// AcctItemAggr��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_ACCT_ITEM_AGGR_CLASS.h"
#include "UserInfoReader.h"

/****ACCT_ITEM_AGGR*****/
/* ������ */
#define		FA_AGGR_ORG_LEVELID100				10101
#define		FA_AGGR_ORG_LEVELID200				10102
#define		FA_AGGR_ORG_LEVELID300				10103
#define		FA_AGGR_ORG_LEVELID400				10104
#define		FA_AGGR_ORG_LEVELID500				10105
#define		FA_AGGR_STATE									10106
#define		FA_AGGR_PARTNER_ID						10107
#define		FA_AGGR_CUST_TYPE_ID					10108
#define		FA_AGGR_ACCT_ITEM_TYPE_ID			10109
#define		FA_AGGR_BILLING_CYCLE_ID			10110
#define		FA_AGGR_PAYMENT_METHOD				10111
#define		FA_AGGR_STAT_DATE							10112
#define		FA_AGGR_SERV_STATE						10113
#define		FA_AGGR_BILLING_MODE					10114
#define		FA_AGGR_SERV_ID								10116
#define		FA_AGGR_ACCT_ID								10117
#define		FA_AGGR_OFFER_ID							10118
#define		FA_AGGR_FREE_TYPE_ID					10119
#define 	FA_AGGR_PRODUCT_ID						10121

/* ����� */                             
#define		FA_AGGR_CHARGE								10115
#define		FA_AGGR_PAGE									10120

class StatAcctItemAggr;
#define  OID_ACCT_ITEM_AGGR							124

class StatAcctItemAggr: public TStatClass
{
public:
    StatAcctItemAggr();
    ~StatAcctItemAggr();
    
    /* �ϵ�Ҫ���1��ʼ��ű��*/
    
   	POINT_TYPE GetMaxPoint(char * sTableName,POINT_TYPE Point);	
										/*
											ȡ��Ӧ����С��Point�����ϵ�ֵ,�����ݷ���0;
										*/
	POINT_TYPE GetMinPoint(char * sTableName,POINT_TYPE Point);	
										/*
											ȡ��Ӧ���д���Point����С�ϵ�ֵ,�����ݷ���0; 
										*/

	void OpenCursor(char * sTableName,POINT_TYPE MinPoint,POINT_TYPE MaxPoint);
										/*
											��[MinPoint,MaxPoint]�հ�������α�
										*/
	bool GetRecordFromTable(POINT_TYPE &BreakPoint);		
										/* ȡ����,�ṩ��ģ���������ģ�� 
											true:	�м�¼Ҫͳ�ƣ�
											false��	������Ҫͳ�ƣ�
											lBreakPoint �����ص�ǰ�Ķϵ�ֵ��
										*/
	void CloseCursor();					/* �رձ��δ򿪵��α� */
	
    int  GetFieldValue(int iFieldID,TFieldValue * pTFieldValue);
    									/* �ṩ����ģ�����
    										-1 : ʧ��
    										0 ���ɹ����
    										1 �����γɹ���ɣ��´λ�������,
    											�ʺ��ڷ��ض����ֵ
    									*/	
    int  GetCycle();					/* ȡ�ķֱ����� ���������*/
    
private:
     TBL_ACCT_ITEM_AGGR_CLASS			*pRead;
    
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

