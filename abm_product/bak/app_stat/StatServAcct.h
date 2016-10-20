/*VER: 1*/ 
#ifndef _STAT_SERV_ACCT_H_
#define _STAT_SERV_ACCT_H_

/////////////////////////////
//
// ServAcct��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_SERV_ACCT_CLASS.h"
#include "UserInfoReader.h"

/****CallEvents*****/
/* ������ */
#define		FA_SA_ORG_LEVELID100			8101
#define		FA_SA_ORG_LEVELID200			8102
#define		FA_SA_ORG_LEVELID300			8103
#define		FA_SA_ORG_LEVELID400			8104
#define		FA_SA_ORG_LEVELID500			8105
#define		FA_SA_STATE_DATE				8107
#define		FA_SA_PRODUCT_ID				8108
#define		FA_SA_STATE						8111
#define		FA_SA_OFFER_ID					8112
#define  	FA_SA_SERV_ID					8115
#define 	FA_SA_ACCT_ID					8116
#define		FA_SA_ITEM_GROUP_ID				8117
#define 	FA_SA_BILLING_CYCLE_ID			8118

class StatServAcct;
#define  OID_SERV_ACCT						127

class StatServAcct: public TStatClass
{
public:
    StatServAcct();
    ~StatServAcct();
    
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
	TBL_SERV_ACCT_CLASS			*pRead;
    
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

