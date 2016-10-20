/*VER: 1*/ 
#ifndef _STAT_REDO_EVENT_H_
#define _STAT_REDO_EVENT_H_

/////////////////////////////
//
// REDO_EVENT��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_REDO_EVENT_CLASS.h"
#include "TBL_SERV_CLASS.h"
#include "UserInfoReader.h"

/****RedoEvents*****/
/* ������ */
#define		FA_REDO_BILL_FLOW_ID							10301
#define		FA_REDO_BILLING_CYCLE_ID					10302
#define		FA_REDO_CUST_TYPE_ID							10303
#define		FA_REDO_EVENT_TYPE_ID							10304
#define 	FA_REDO_BILLING_AREA_CODE					10305
#define		FA_REDO_BILLING_VISIT_AREA_CODE		10306
#define 	FA_REDO_CALLING_AREA_CODE					10307
#define		FA_REDO_CALLED_AREA_CODE					10308
#define		FA_REDO_ACCOUNT_AREA_CODE					10309
#define		FA_REDO_SERV_CREATE_DATE					10310
#define		FA_REDO_SERV_STATE								10311
#define		FA_REDO_FREE_TYPE_ID							10312
#define		FA_REDO_BILLING_MODE							10313
#define		FA_REDO_START_HOUR								10314
#define		FA_REDO_START_DAY									10315
#define		FA_REDO_STAT_HOUR									10316
#define		FA_REDO_STAT_DAY									10317
#define		FA_REDO_SWITCH_ID									10318
#define		FA_REDO_ORG_LEVELID100						10319
#define		FA_REDO_ORG_LEVELID200						10320
#define		FA_REDO_ORG_LEVELID300						10321
#define		FA_REDO_ORG_LEVELID400						10322
#define		FA_REDO_ORG_LEVELID500						10323
#define		FA_REDO_TRUNK_IN									10324
#define		FA_REDO_TRUNK_OUT									10325
#define		FA_REDO_PRODUCT_ID								10326
#define		FA_REDO_ERROR_ID									10327


                                            
/* ����� */                                
#define		FA_REDO_PAGE											10360
#define		FA_REDO_DURATION									10361


class StatRedoEvent;
#define  OID_REDO_EVENT											126

class StatRedoEvent: public TStatClass
{
public:
    StatRedoEvent();
    ~StatRedoEvent();
    
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
    TBL_REDO_EVENT_CLASS 	* pRead;
    TBL_SERV_CLASS			TblServClass;
    /*TBL_CUST_CLASS			TblCustClass;*/
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
		UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

