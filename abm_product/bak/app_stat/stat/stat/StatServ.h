/*VER: 1*/ 
#ifndef _STAT_SERV_H_
#define _STAT_SERV_H_

/////////////////////////////
//
// Serv��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_SERV_CLASS.h"
#include "UserInfoReader.h"

/****CallEvents*****/
/* ������ */
#define		FA_S_ORG_LEVELID100				9101
#define		FA_S_ORG_LEVELID200				9102
#define		FA_S_ORG_LEVELID300				9103
#define		FA_S_ORG_LEVELID400				9104
#define		FA_S_ORG_LEVELID500				9105
#define		FA_S_SERV_CREATE_DATE			9106
#define		FA_S_STATE_DATE					9107
#define		FA_S_PRODUCT_ID					9108
#define		FA_S_BILLING_MODE				9109
#define		FA_S_CUST_TYPE_ID				9110
#define		FA_S_STATE						9111
#define		FA_S_OFFER_ID					9112
#define		FA_S_AGENT_ID					9113
/* ����� */                                
#define		FA_S_SERV_CNT					9114

class StatServ;
#define  OID_SERV							3

class StatServ: public TStatClass
{
public:
    StatServ();
    ~StatServ();
    
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
     TBL_SERV_CLASS			*pRead;
    
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

