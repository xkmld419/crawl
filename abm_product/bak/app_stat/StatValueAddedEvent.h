/*VER: 1*/ 
#ifndef _STAT_VALUE_ADDED_EVENT_H_
#define _STAT_VALUE_ADDED_EVENT_H_

/////////////////////////////
//
// VALUE_ADDED_EVENT��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_VALUE_ADDED_EVENT_CLASS.h"
#include "TBL_SERV_CLASS.h"
#include "UserInfoReader.h"

/****ValueAddedEvents*****/
/* ������ */
#define		FA_ADDVALUE_PAYMENT_METHOD					10001
#define		FA_ADDVALUE_ACCT_ITEM_BILL_STATE		10002
#define		FA_ADDVALUE_CUST_TYPE_ID						10003
#define		FA_ADDVALUE_EVENT_TYPE_ID						10004
#define		FA_ADDVALUE_CALLED_AREA							10005
#define		FA_ADDVALUE_SERV_CREATE_DATE				10006
#define		FA_ADDVALUE_ACCT_ITEM_TYPE_ID				10007
#define		FA_ADDVALUE_SERV_STATE							10008
#define		FA_ADDVALUE_FREE_TYPE_ID						10009
#define		FA_ADDVALUE_BILLING_MODE						10010
#define		FA_ADDVALUE_STAT_HOUR								10011
#define		FA_ADDVALUE_STAT_DAY								10012
#define		FA_ADDVALUE_SWITCH_ID								10013
#define		FA_ADDVALUE_ORG_LEVELID100					10014
#define		FA_ADDVALUE_ORG_LEVELID200					10015
#define		FA_ADDVALUE_ORG_LEVELID300					10016
#define		FA_ADDVALUE_ORG_LEVELID400					10017
#define		FA_ADDVALUE_ORG_LEVELID500					10018
#define		FA_ADDVALUE_BILLING_CYCLE_ID				10019
#define		FA_ADDVALUE_PRODUCT_ID							10020
#define		FA_ADDVALUE_OFFER_ID								10021
#define		FA_ADDVALUE_CALLING_SP_TYPE_ID			10022
#define		FA_ADDVALUE_CALLED_SP_TYPE_ID				10023
#define		FA_ADDVALUE_BILLING_VISIT_AREA_CODE	10024
                                              
/* ����� */                                  
#define		FA_ADDVALUE_PAGE										10025
#define		FA_ADDVALUE_DURATION								10026
#define		FA_ADDVALUE_BILL_DURATION						10027
#define		FA_ADDVALUE_COUNTS									10028
#define		FA_ADDVALUE_ORG_CHARGE1							10029
#define		FA_ADDVALUE_STD_CHARGE1							10030
#define		FA_ADDVALUE_CHARGE1									10031



class StatValueAddedEvent;
#define  OID_VALUE_ADDED_EVENT				123

class StatValueAddedEvent: public TStatClass
{
public:
    StatValueAddedEvent();
    ~StatValueAddedEvent();
    
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
    TBL_VALUE_ADDED_EVENT_CLASS 	* pRead;
    TBL_SERV_CLASS			TblServClass;
    /*TBL_CUST_CLASS			TblCustClass;*/
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

