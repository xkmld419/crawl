/*VER: 1*/ 
/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident	"@(#)listbox.h	1.0	2001/01/11	ruanyongjiang" */
#ifndef _TIBS_LIST_H
#define _TIBS_LIST_H

#include <curses.h>
#include "control.h"
/*  ���ⲿ�����޹�    defined in listbox.c*/
extern int   ListDisplay(Control *pList,int iStartItem,int iKey); 
							/* ListBox ����ʾ */
extern int   ListEntry(Control *pList,long lParam);     /* ListBox ��ִ�к������ */


/*  �ṩ�ⲿ����  defined in listbox.c */
int ListItemState(Control *pList, int iNo);		    /* ȡ��listbox��iNo����ѡ��״̬ */
extern char *ListGetItem(Control *pList,int iNo);           /* ȡ��listbox��iNo����ѡ�е�item�� */ 
extern char *ListGetSelItem(Control *pList,int iNo);        /* ȡ��listbox��iNo����ѡ�е�item�� */ 
void ListSetItem(Control *pList,int iItemNo,int iState);    /* ����item ��״̬  */ 
int  ListSelNum(Control  *pList);			    /* �õ���ѡ�е� items ���� */
#endif


