/*VER: 1*/ 
#ifndef _STAT_BILL_H_
#define _STAT_BILL_H_

/////////////////////////////
//
// Payment��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_BILL_CLASS.h"
#include "UserInfoReader.h"

/****PayMent*****/
/* ������ */
#define		FA_BILL_BSS_ORG_LEVELID100			9701
#define		FA_BILL_BSS_ORG_LEVELID200			9702
#define		FA_BILL_BSS_ORG_LEVELID300			9703
#define		FA_BILL_BSS_ORG_LEVELID400			9704
#define		FA_BILL_BSS_ORG_LEVELID500			9705
#define		FA_BILL_STAFF_ID					9706
#define		FA_BILL_PAYED_METHOD				9707
#define 	FA_BILL_BILLING_CYCLE_ID			9708
#define		FA_BILL_STATE_DATE					9709
#define		FA_BILL_STATE						9710
#define		FA_BILL_STAT_DATE					9711
                                                 
/* ����� */                                     
#define		FA_BILL_BALANCE						9712
#define		FA_BILL_CNT							9713



class StatBill;
#define  OID_BILL								119

class StatBill: public TStatClass
{
public:
    StatBill();
    ~StatBill();
    
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
     TBL_BILL_CLASS			*pRead;
    
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

