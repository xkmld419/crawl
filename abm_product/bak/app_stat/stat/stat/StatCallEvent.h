/*VER: 1*/ 
#ifndef _STAT_CALL_EVENT_H_
#define _STAT_CALL_EVENT_H_

/////////////////////////////
//
// CALL_EVENT��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_CALL_EVENT_CLASS.h"
#include "TBL_SERV_CLASS.h"
#include "UserInfoReader.h"

/****CallEvents*****/
/* ������ */
#define		FA_C_PAYMENT_METHOD				9001
#define		FA_C_ACCT_ITEM_BILL_STATE		9002
#define		FA_C_CUST_TYPE_ID				9003
#define		FA_C_EVENT_TYPE_ID				9004
#define		FA_C_CALLED_AREA				9005
#define		FA_C_SERV_CREATE_DATE			9006
#define		FA_C_ACCT_ITEM_TYPE_ID			9007
#define		FA_C_SERV_STATE					9008
#define		FA_C_FREE_TYPE_ID				9009
#define		FA_C_BILLING_MODE				9010
#define		FA_C_STAT_HOUR					9011
#define		FA_C_STAT_DAY					9012
#define		FA_C_SWITCH_ID					9013
#define		FA_C_ORG_LEVELID100				9014
#define		FA_C_ORG_LEVELID200				9015
#define		FA_C_ORG_LEVELID300				9016
#define		FA_C_ORG_LEVELID400				9017
#define		FA_C_ORG_LEVELID500				9018
#define		FA_C_BILLING_CYCLE_ID			9019
#define		FA_C_PRODUCT_ID					9020
#define		FA_C_OFFER_ID					9021
#define		FA_C_CALLING_SP_TYPE_ID			9022
#define		FA_C_CALLED_SP_TYPE_ID			9023
#define		FA_C_BILLING_VISIT_AREA_CODE	9024
                                            
/* ����� */                                
#define		FA_C_PAGE						9025
#define		FA_C_DURATION					9026
#define		FA_C_BILL_DURATION				9027
#define		FA_C_COUNTS						9028
#define		FA_C_ORG_CHARGE1				9029
#define		FA_C_STD_CHARGE1				9030
#define		FA_C_CHARGE1					9031



class StatCallEvent;
#define  OID_CALL_EVENT				2

class StatCallEvent: public TStatClass
{
public:
    StatCallEvent();
    ~StatCallEvent();
    
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
    TBL_CALL_EVENT_CLASS 	* pRead;
    TBL_SERV_CLASS			TblServClass;
    /*TBL_CUST_CLASS			TblCustClass;*/
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

