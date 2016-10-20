/*VER: 1*/ 
#ifndef _STAT_ACCT_BALANCE_H_
#define _STAT_ACCT_BALANCE_H_

/////////////////////////////
//
// AcctBalance��
//
/////////////////////////////

#include "TStatTask.h"

#include "StdEvent.h"

#include "TBL_ACCT_BALANCE_CLASS.h"
#include "UserInfoReader.h"

/****AcctBalance*****/
/* ������ */
#define		FA_AB_ORG_LEVELID100				9301
#define		FA_AB_ORG_LEVELID200				9302
#define		FA_AB_ORG_LEVELID300				9303
#define		FA_AB_ORG_LEVELID400				9304
#define		FA_AB_ORG_LEVELID500				9305
#define		FA_AB_BALANCE_TYPE_ID				9306
#define		FA_AB_BALANCE_LEVEL					9307
#define		FA_AB_STATE_DATE					9308
#define		FA_AB_STATE							9309

/* ����� */                                
#define		FA_AB_BALANCE						9310



class StatAcctBalance;
#define  OID_ACCT_BALANCE						115

class StatAcctBalance: public TStatClass
{
public:
    StatAcctBalance();
    ~StatAcctBalance();
    
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
     TBL_ACCT_BALANCE_CLASS			*pRead;
    
    int GetExprFieldValue( int iFieldID, TFieldValue * pTFieldValue);
    
	UserInfoInterface 	interface;
    ServInfo			servinfo,*pservinfo;
    CustInfo			custinfo,*pcustinfo;
    
    Serv				serv,*pServ;
    
};

#endif

