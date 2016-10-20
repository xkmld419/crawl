/*VER: 1*/ 
#ifndef _REALSTAT_TASK_CLASS_H__
#define _REALSTAT_TASK_CLASS_H__
#include "TOCIQuery.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <new.h>

enum{VT_UNKNOW=-1,VT_INT,VT_STRING} ;

#ifndef U_LONG_T 
#define U_LONG_T
typedef		unsigned long  	U_LONG;
#endif

#ifndef U_LLONG_T 
#define U_LLONG_T
typedef		unsigned long long  	U_LLONG;
#endif

#define BIT32OS

#ifdef BIT32OS
typedef	U_LLONG 	POINT_TYPE;
#define FORMAT_STR	"%llu"
#endif

#ifdef BIT64OS
typedef  U_LONG		POINT_TYPE;
#define FORMAT_STR	"%lu"
#endif

#define  FA_Expr                   10000

class StatMonthList;

/******ͳ����ֵ�ڵ�*****/
class TFieldValue
{
public:
	TFieldValue(void);
	TFieldValue(long);
	TFieldValue(char *);
	TFieldValue(TFieldValue &);
	~TFieldValue();
	void SetValue(long);				/*	δ֪����������£�ֱ�Ӹ�ֵ�������ݱ������͸�ֵ������ */
	void SetValue(char *);				/*	δ֪����������£�ֱ�Ӹ�ֵ�������ݱ������͸�ֵ������ */
	TFieldValue& operator += (TFieldValue& d2 ) ;
	TFieldValue& operator = (TFieldValue& d2 ) ;
	bool		operator == (TFieldValue& d2 ) const;
	bool		operator != (TFieldValue& d2 ) const;
	
	void AutoSetValue(long);			/* ��֪����������£��Զ�ת����ֵ���ͣ�����ֵ
											1.���������Σ���ֱ�Ӹ�ֵ
											2.�������ַ��ͣ����Ƚ����ͱ���ת�����ַ��ͺ��ٸ�ֵ����
										*/
	void AutoSetValue(char*);			/* ��֪����������£��Զ�ת����ֵ���ͣ�����ֵ
											1.�������ַ��ͣ���ֱ�Ӹ�ֵ
											2.���������ͣ����Ƚ��ַ��ͱ���ת�������ͺ��ٸ�ֵ����
										*/
	void SetDefaultValue(void);			/* ��֪����������£�����Ĭ��ֵ
											1.�������ַ��ͣ�����ֵ��ֵΪ'NULL'
											2.���������ͣ�����ֵ��ֵΪ-1
										*/
	
	char	ValType;					/*VT_UNKNOW:δ֪���ͣ� VT_INT:���Σ� VT_STRING���ַ���*/
	union{
		long	lValue;					/*integerֵ*/
		char*	sValue;					/*stringֵ*/
	}value;
};


class TStatClass{
public:
	TStatClass(int iStatClassID);
	TStatClass();
	~TStatClass();
	void SetClassID(int iStatClassID);	/* ����ͳ�ƴ����ʶ */
	
	void OutputResult();  				/* ������� */
    void ProcessStat();	  				/* ����ǰ��¼ */
	void FreeResult();					/* �ͷų���û��ʹ�õ��ڴ�ڵ� */
	bool GetRecord();					/* ȡ��������ģ�����
											true:	�м�¼Ҫͳ�ƣ�
											false��	������Ҫͳ�ƣ�
										*/
	void Commit();						/* �ύ��� */
	/* �ϵ�Ҫ���1��ʼ��ű��*/
	
	virtual POINT_TYPE GetMaxPoint(char * sTableName,POINT_TYPE Point)=NULL;	
										/*
											ȡ��Ӧ����С��Point�����ϵ�ֵ,�����ݷ���0;
										*/
	virtual POINT_TYPE GetMinPoint(char * sTableName,POINT_TYPE Point)=NULL;	
										/*
											ȡ��Ӧ���д���Point����С�ϵ�ֵ,�����ݷ���0; 
										*/

	virtual void OpenCursor(char * sTableName,POINT_TYPE MinPoint,POINT_TYPE MaxPoint)=NULL;
										/*
											����'INS'���͵�ͳ����
											�򿪶�Ӧ���[MinPoint,MaxPoint]�հ�������α�
											
											����'UPD'���͵�ͳ����
											�򿪶�Ӧ���α꣬MinPoint��MaxPoint�޺���
										*/
										
	virtual bool GetRecordFromTable(POINT_TYPE &BreakPoint)=NULL;		
										/* 
										  ȡ����,�ṩ��ģ���������ģ�� 
											����'INS'���͵�ͳ���ࣺ
												true:	�м�¼Ҫͳ�ƣ�
												false��	������Ҫͳ�ƣ�
												lBreakPoint �����ص�ǰ�Ķϵ�ֵ��
											����'UPD'���͵�ͳ���ࣺ
												true:	�м�¼Ҫͳ�ƣ�
												false��	������Ҫͳ�ƣ�
												lBreakPoint �����뷵�أ�
										*/
										
	virtual void CloseCursor()=NULL;	/* �رձ��δ򿪵��α� */
	
    virtual int  GetFieldValue(int iFieldID,TFieldValue * pTFieldValue)=NULL;
    									/* �ṩ����ģ�����
    										-1 : ʧ��
    										0 ���ɹ����
    										1 �����γɹ���ɣ��´λ�������,
    											�ʺ��ڷ��ض����ֵ
    									*/	
    virtual int  GetCycle()=NULL;		/* ȡ�ķֱ����� ���������*/
    

	int 			m_ResultCount;		/* �ڴ��е�ͳ�Ƽ�¼�� */
	
private:
	void			SavePoint(POINT_TYPE BreakPoint);
	void 			SavePointToDB();
	bool 			GetRecord_INS();
	bool			GetRecord_UDP();
	int				iStatClassID;		/* ͳ�����ʶ*/
	StatMonthList	*pStatMonthListHead;/* ��ͳ������ͷ*/
	int				iRecordNum;			/* �Ѿ������¼��*/		
	int				TotalCount;			/* �ܵ�ͳ�Ƽ�¼�� */
	time_t			NowTime;			/* ���ʱ��ȡ��ʱ��	*/
	bool			iOutPutFlag;		/* �����־		*/
	
	char			sStatClassType[4];	/* ͳ��������� :
											'INS': ����ԴΪֻ�������� ,�����嵥����ͳ��
 											'UPD'������ԴΪinsert��update�����ͣ����ڼ��м���ͳ��
										*/
										
	/* ����Ϊ�ϵ㴦�����ݶ��� */
	char 			sTableName[50];		/* ��ǰ����ı���,��Ҫ��ʼ�� */
	POINT_TYPE		BreakPoint;			/* ��ǰ��Ĵ���ε� */
	bool			bFlushBreakPointFlag;/* ˢ�¶ϵ��־,��Ҫ��ʼ�� */
	bool 			bOpenFlag;	/* ��Ҫ��ʼ������ */
	bool			bOpenCurFlag,bTurnOutFlag;
	POINT_TYPE 		lBeginPoint,lEndPoint,lCurPoint;
	POINT_TYPE		lMaxPoint1,lMinPoint1,lMaxPoint2,lMinPoint2;
	
	
friend class TStatTask;

};

extern void my_new_error();

#endif
