/*VER: 1*/ 
#ifndef _STAT_AREFUNDLOG_H_
#define _STAT_AREFUNDLOG_H_

/////////////////////////////
//
// A_Refund_Log��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_A_REFUND_LOG_CLASS.h"
#include "UserInfoReader.h"

/****CallEvents*****/
/* ������ 102*/
#define		FA_REFUNDLOG_BSS_ORG_LEVELID100			20001
#define		FA_REFUNDLOG_BSS_ORG_LEVELID200			20002
#define		FA_REFUNDLOG_BSS_ORG_LEVELID300			20003
#define		FA_REFUNDLOG_BSS_ORG_LEVELID400			20004
#define		FA_REFUNDLOG_BSS_ORG_LEVELID500			20005
#define		FA_REFUNDLOG_STAFF_ID					20006
#define		FA_REFUNDLOG_PAYED_METHOD				20007
#define		FA_REFUNDLOG_BALANCE_TYPE_ID			20008
#define		FA_REFUNDLOG_STATE						20009
#define		FA_REFUNDLOG_STAT_DATE					20010
/* ����� */                                
#define		FA_REFUNDLOG_BALANCE					20051
#define		FA_REFUNDLOG_BILL_CNT					20052

class StatARefundLog;
#define  OID_AREFUNDLOG							200

class StatARefundLog: public TStatClass
{
public:
    StatARefundLog();
    ~StatARefundLog();
    
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
     TBL_A_REFUND_LOG_CLASS			*pRead;
    
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

