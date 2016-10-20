/*VER: 1*/ 
#ifndef _STAT_ERROR_EVENT_H_
#define _STAT_ERROR_EVENT_H_

/////////////////////////////
//
// ERROR_EVENT��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_ERROR_EVENT_CLASS.h"
#include "TBL_SERV_CLASS.h"
#include "UserInfoReader.h"

/****CallEvents*****/
/* ������ */

#define 	FA_ERROR_BILL_FLOW_ID						10201
#define		FA_ERROR_BILLING_ORG_ID					10202
#define		FA_ERROR_START_DATE							10203
#define		FA_ERROR_SWITCH_ID							10204
#define		FA_ERROR_FILE_ID								10205
#define		FA_ERROR_CREATED_DATE						10206
#define		FA_ERROR_EVENT_STATE						10207
#define		FA_ERROR_STATE_DATE							10208
#define		FA_ERROR_ERROR_ID								10209

             
/* ����� */
                          
#define		FA_DATA_PAGE										10210



class StatError;
#define  OID_ERROR										125

class StatError: public TStatClass
{
public:
    StatError();
    ~StatError();
    
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
    TBL_ERROR_EVENT_CLASS 	* pRead;
    TBL_SERV_CLASS			TblServClass;
    /*TBL_CUST_CLASS			TblCustClass;*/
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

