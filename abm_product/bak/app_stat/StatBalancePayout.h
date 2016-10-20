/*VER: 1*/ 
#ifndef _STAT_BALANCE_PAYOUT_H_
#define _STAT_BALANCE_PAYOUT_H_

/////////////////////////////
//
// BalancePayout��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_BALANCE_PAYOUT_CLASS.h"
#include "UserInfoReader.h"

/****BalancePayout*****/
/* ������ */
#define		FA_BP_ORG_LEVELID100				9501
#define		FA_BP_ORG_LEVELID200				9502
#define		FA_BP_ORG_LEVELID300				9503
#define		FA_BP_ORG_LEVELID400				9504
#define		FA_BP_ORG_LEVELID500				9505
#define		FA_BP_BALANCE_TYPE_ID				9506
#define		FA_BP_BALANCE_LEVEL					9507
#define		FA_BP_STATE_DATE					9508
#define		FA_BP_STATE							9509
#define		FA_BP_STAT_DATE						9510

/* ����� */                                
#define		FA_BP_BALANCE						9511



class StatBalancePayout;
#define  OID_BALANCE_PAYOUT						117

class StatBalancePayout: public TStatClass
{
public:
    StatBalancePayout();
    ~StatBalancePayout();
    
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
     TBL_BALANCE_PAYOUT_CLASS			*pRead;
    
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

