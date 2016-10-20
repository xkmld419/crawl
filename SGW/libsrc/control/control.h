/*VER: 1*/ 
/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident	"@(#)control.h	1.0	2001/03/23	ruanyongjiang" */

/**********************************************
           �ؼ��ṹ�Լ����÷���  
   ���ܸ�Ҫ��
       �ṩ�ַ��ն˵Ŀؼ���ʾ�Լ��ؼ���Ϊ
   ʹ��˵����
       ������ı�д��        
       #include "controls.h"
       #include "FormSample.h"
       main(){
            startwin();				  // ����curses����       
	    FormSample fs;
	    fs.run ();
            endwin();				  // ����curses����       
	}
       ����ı�д��
	    1.  ��д  .rc �ļ��� ( ��� ../src/�е����� )
            2.  �ÿ�ִ�г��� makeform �����ɽ�����򼰶��û��ӿڳ����Լ�ͷ�ļ� .
		�磺 makeform login.rc ==> ���� login_j.c �� login_f.c �Լ� login.hͷ�ļ�
	    3.  �û���login_f.c �еĸ��ؼ������¼���д  

       ���룺gcc -o main main.c login_j.c login_f.c -lcontrols -lcurses

       �����ṩ��һЩ������Ļ��ʾ�ĺ����� drawbox() ,drawblank() , prtmsg() ;
                                
 **********************************************/

#ifndef CONTROL_H
#define CONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

#include "macro.h"

typedef int  (* FUNC)(void *pObject,long lParam); /* �ؼ�ִ�к������ */
typedef int  (* KEYPRESS)(void *pObject,int ich,long lParam);
					      /* �Զ���ؼ������¼� */
typedef enum{BOX,LABEL,BUTTON,EDIT,LIST }TYPE;/* �ؼ����� */

typedef struct TControl{
    int      iId;
    int      iTop,iLeft,iWidth,iHeight;   /* �ؼ���λ�����С */  
    TYPE     eType;                       /* �ؼ������ */
    int      bFrame;                      /* 0:��Ҫ�߿� 1: Ҫ�߿� */
    int      iHotKey;			  /* �ؼ����ȼ�  2001.2.5 ���� */
    int      iAdditional;                 /* �ؼ��������ص� */
                                          /*  ��EDIT: ��ʾ�������� 
						      ��ѡ��AMOUNT,NUMERIC,NUMBER,PASSWD,STATIC,STRING
					      ��LIST: ����list�е� ITEMS ��Ŀ  
    					  */     
    int      bSingle;                     /* LIST�ؼ�ʹ�ê 1:��ѡ  ����:��ѡ  */
    WINDOW   *pWin;                       /* �ؼ����ڴ��ڵ�ָ�� */
    char     sData[MAX_ITEMS];            /* �ؼ��Ĵ��ռ� */
    char     **pData;                     /* �ؼ���ָ����ָ�� */
    struct TControl *pIndex,*pTab,*pUp,*pDown,*pLeft,*pRight;  /* �ؼ���Χ�Ŀؼ� pTabָTAB˳��*/
    FUNC     pFunc;                       /* �ؼ���ִ�к������ */
    KEYPRESS pKeyPress;                   /* �ؼ��İ��������¼� ��ӷ�ʽ ���緵�ص���0�����пؼ��ṩ�ļ����¼�*/
    KEYPRESS pHotKeyPress;                /* �ؼ���ϵͳ�ȼ������¼� ��ӷ�ʽ ���緵�ص���0�����пؼ��ṩ�ļ����¼�*/
} Control;
/*  �ؼ������ṩ�Ľӿ�       defined in control.c */
Control *CtlInit(TYPE eType,int iId,int iTop,int iLeft,int iWidth,int iHeight);
		        		  /* �ؼ����Ի� */
void     CtlSetDir(Control *pCtl,Control *pUp,Control *pDown,Control *pLeft,Control *pRight,Control *pTab,Control *pIndex);
					  /* ���ÿؼ���Χ�ؼ���TAB˳�� */
int      FormRun(Control *pFormHandle);     /* ����ؼ��¼�ѭ�� */
Control *GetCtlByID(Control *pFormHandle,int iId); /* �õ�ָ��ID�Ŀؼ� */

/*  һ��Ϊ�ؼ��ڲ�ʵ��ʹ�� */  
int      FormClear(Control *pFormHandle);   /* ��Form�е�Edit List�ؼ���������� */
int      FormKill(Control *pFormHandle);    /* �ͷ����пؼ� */
int      FormShow(Control *pFormHandle);    /* ���пؼ�����ʾ */
void     CtlShow(Control *pCtl);          /* �ؼ�����ʾ  */
void     CtlRedraw(Control *pCtl);        /* �ؼ���ʾ���ػ� */
Control *CtlChg(Control *pFirst,Control *curbtn,Control *chgbtn,int ich); /* �ı䵱ǰ�ؼ�  */
int      CtlAtv(Control *pCurBtn,Control *pNxtBtn); /* �ı佹��    */
void     CtlClear (Control *pCtl);        /* clear display */

/* �����ṩ��һЩ���ú���      defined in control.c */
void     drawbox(int iTop,int iLeft,int iWidth,int iHeight);   /* ��box */
void     drawblank(int iTop,int iLeft,int iWidth,int iHeight); /* ���հ�������������ʾ�Ŀ��ƣ���FORM���л� */
void     prtmsg(Control *pCtl,int iTop,int iLeft,char []);     /* ������MessageBox���� */ 
void     startwin( );					       /* ����curses����       */
int      yorn(Control *pCtl,int iTop,int iLeft,char []);       /* Yes or No �ļ��̵ȴ��ش� */
int      g_date(int day[3]);

int SelfBox (Control *pCtl);
#endif





