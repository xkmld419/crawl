/*VER: 1*/ 
#ifndef _STAT_OTHER_EVENT_H_
#define _STAT_OTHER_EVENT_H_

/////////////////////////////
//
// OTHER_EVENT��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_OTHER_EVENT_CLASS.h"
//#include "TBL_SERV_CLASS.h"
#include "UserInfoReader.h"
#include "Date.h"
/****OtherEvents*****/
/* ������ */
#define		FA_OTHER_BILLFLOW_ID				1401
#define		FA_OTHER_BILLING_ORG_ID 		1402
#define		FA_OTHER_STAT_DAY    				1403
#define		FA_OTHER_STAT_HOUR				  1404
#define		FA_OTHER_SWITCH_ID  				1405
#define		FA_OTHER_TRUNK_IN     			1406
#define		FA_OTHER_TRUNK_OUT    			1407
#define		FA_OTHER_FILE_ID  					1408
#define		FA_OTHER_EVENT_STATE				1409
#define		FA_OTHER_ERROR_ID   				1410
#define		FA_OTHER_CYCLE_BEGIN_DAY		1411
#define		FA_OTHER_CYCLE_END_DAY			1412
                                            
/* ����� */                                
#define		FA_OTHER_PAGE								1430
#define		FA_OTHER_DURATION						1431
#define		FA_OTHER_BILL_DURATION			1432
#define		FA_OTHER_SEND_BYTES					1433
#define		FA_OTHER_RECV_BYTES					1434
#define		FA_OTHER_ORG_CHARGE1				1435
#define		FA_OTHER_CYCLE_DAY					1436



class StatOtherEvent;
#define  OID_OTHER_EVENT				120

class StatOtherEvent: public TStatClass
{
public:
    StatOtherEvent();
    ~StatOtherEvent();
    
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
    TBL_OTHER_EVENT_CLASS 	* pRead;
    //TBL_SERV_CLASS			TblServClass;
    /*TBL_CUST_CLASS			TblCustClass;*/
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

