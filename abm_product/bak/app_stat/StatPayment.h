/*VER: 1*/ 
#ifndef _STAT_PAYMENT_H_
#define _STAT_PAYMENT_H_

/////////////////////////////
//
// Payment��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_PAYMENT_CLASS.h"
#include "UserInfoReader.h"

/****PayMent*****/
/* ������ */
#define		FA_P_BSS_ORG_LEVELID100			9601
#define		FA_P_BSS_ORG_LEVELID200			9602
#define		FA_P_BSS_ORG_LEVELID300			9603
#define		FA_P_BSS_ORG_LEVELID400			9604
#define		FA_P_BSS_ORG_LEVELID500			9605
#define		FA_P_STAFF_ID					9606
#define		FA_P_PAYED_METHOD				9607
#define 	FA_P_OPERATION_TYPE				9608
#define		FA_P_STATE_DATE					9609
#define		FA_P_STATE						9610
#define		FA_P_STAT_DATE					9611
                                             
/* ����� */                                 
#define		FA_P_BALANCE					9612
#define		FA_P_PAYMENT_CNT				9613



class StatPayment;
#define  OID_PAYMENT						118

class StatPayment: public TStatClass
{
public:
    StatPayment();
    ~StatPayment();
    
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
     TBL_PAYMENT_CLASS			*pRead;
    
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

