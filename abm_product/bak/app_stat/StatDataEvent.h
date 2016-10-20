/*VER: 1*/ 
#ifndef _STAT_DATA_EVENT_H_
#define _STAT_DATA_EVENT_H_

/////////////////////////////
//
// DATA_EVENT��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_DATA_EVENT_CLASS.h"
#include "TBL_SERV_CLASS.h"
#include "UserInfoReader.h"

/****CallEvents*****/
/* ������ */
#define		FA_DATA_PAYMENT_METHOD					9801
#define		FA_DATA_ACCT_ITEM_BILL_STATE		9802
#define		FA_DATA_CUST_TYPE_ID						9803
#define		FA_DATA_EVENT_TYPE_ID					9804
#define		FA_DATA_CALLED_AREA						9805
#define		FA_DATA_SERV_CREATE_DATE				9806
#define		FA_DATA_ACCT_ITEM_TYPE_ID			9807
#define		FA_DATA_SERV_STATE							9808
#define		FA_DATA_FREE_TYPE_ID						9809
#define		FA_DATA_BILLING_MODE						9810
#define		FA_DATA_STAT_HOUR							9811
#define		FA_DATA_STAT_DAY								9812
#define		FA_DATA_SWITCH_ID							9813
#define		FA_DATA_ORG_LEVELID100				9814
#define		FA_DATA_ORG_LEVELID200				9815
#define		FA_DATA_ORG_LEVELID300				9816
#define		FA_DATA_ORG_LEVELID400				9817
#define		FA_DATA_ORG_LEVELID500				9818
#define		FA_DATA_BILLING_CYCLE_ID			9819
#define		FA_DATA_PRODUCT_ID						9820
#define		FA_DATA_OFFER_ID							9821
#define		FA_DATA_CALLING_SP_TYPE_ID		9822
#define		FA_DATA_CALLED_SP_TYPE_ID			9823
#define		FA_DATA_BILLING_VISIT_AREA_CODE	9824
             
/* ����� */                               
#define		FA_DATA_PAGE									9825
#define		FA_DATA_DURATION							9826
#define		FA_DATA_BILL_DURATION				9827
#define		FA_DATA_COUNTS								9828
#define		FA_DATA_ORG_CHARGE1					9829
#define		FA_DATA_STD_CHARGE1					9830
#define		FA_DATA_CHARGE1							9831
#define		FA_DATA_RECV_BYTES					9832
#define		FA_DATA_SEND_BYTES					9833
#define		FA_DATA_RECV_PACKETS				9834
#define		FA_DATA_SEND_PACKETS				9835


class StatDataEvent;
#define  OID_DATA_EVENT				121

class StatDataEvent: public TStatClass
{
public:
    StatDataEvent();
    ~StatDataEvent();
    
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
    TBL_DATA_EVENT_CLASS 	* pRead;
    TBL_SERV_CLASS			TblServClass;
    /*TBL_CUST_CLASS			TblCustClass;*/
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

