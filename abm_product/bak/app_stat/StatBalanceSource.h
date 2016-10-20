/*VER: 1*/ 
#ifndef _STAT_BALANCE_SOURCE_H_
#define _STAT_BALANCE_SOURCE_H_

/////////////////////////////
//
// BalanceSource��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_BALANCE_SOURCE_CLASS.h"
#include "UserInfoReader.h"

/****BalanceSource*****/
/* ������ */
#define		FA_BS_ORG_LEVELID100				9401
#define		FA_BS_ORG_LEVELID200				9402
#define		FA_BS_ORG_LEVELID300				9403
#define		FA_BS_ORG_LEVELID400				9404
#define		FA_BS_ORG_LEVELID500				9405
#define		FA_BS_BALANCE_TYPE_ID				9406
#define		FA_BS_BALANCE_LEVEL					9407
#define		FA_BS_STATE_DATE					9408
#define		FA_BS_STATE							9409
#define		FA_BS_STAT_DATE						9410

/* ����� */                                
#define		FA_BS_BALANCE						9411



class StatBalanceSource;
#define  OID_BALANCE_SOURCE						116

class StatBalanceSource: public TStatClass
{
public:
    StatBalanceSource();
    ~StatBalanceSource();
    
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
     TBL_BALANCE_SOURCE_CLASS			*pRead;
    
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

