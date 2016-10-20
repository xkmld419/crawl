/*VER: 1*/ 
#ifndef _STAT_ACCT_ITEM_H_
#define _STAT_ACCT_ITEM_H_

/////////////////////////////
//
// AcctItem��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_ACCT_ITEM_CLASS.h"
#include "UserInfoReader.h"

/****CallEvents*****/
/* ������ */
#define		FA_A_ORG_LEVELID100				9201
#define		FA_A_ORG_LEVELID200				9202
#define		FA_A_ORG_LEVELID300				9203
#define		FA_A_ORG_LEVELID400				9204
#define		FA_A_ORG_LEVELID500				9205
#define		FA_A_STATE						9206
#define		FA_A_PARTNER_ID					9207
#define		FA_A_CUST_TYPE_ID				9208
#define		FA_A_ACCT_ITEM_TYPE_ID			9209
#define		FA_A_BILLING_CYCLE_ID			9210
#define		FA_A_PAYMENT_METHOD				9211
#define		FA_A_STATE_DATE					9212
#define		FA_A_SERV_STATE					9213
#define		FA_A_BILLING_MODE				9214
#define		FA_A_SERV_ID					9216
#define		FA_A_ACCT_ID					9217
#define		FA_A_OFFER_ID					9218
/* ����� */                                
#define		FA_A_CHARGE						9215


class StatAcctItem;
#define  OID_ACCT_ITEM						114

class StatAcctItem: public TStatClass
{
public:
    StatAcctItem();
    ~StatAcctItem();
    
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
     TBL_ACCT_ITEM_CLASS			*pRead;
    
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

