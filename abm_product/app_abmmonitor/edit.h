/*VER: 1*/ 
/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident	"@(#)edit.h	1.0	2001/01/11	ruanyongjiang" */
#ifndef _EDIT_H
#define _EDIT_H

#include <curses.h>
#include "control.h"
#include "CommonMacro.h"

/*  ���ⲿ�����޹�       defined in edit.c   */
extern int  EditEntry(Control *pCtl,long lParam); /* Edit ִ�к������ */
extern int  DoCheck(Control *pCtl, char *pStrBuf,int ch,int iInputType,int iWidth,int icnt);
						  /* Edit ��������ַ���������Ӧ���� */
extern int  EditKeyPress(Control *pCtl,int ich,long lParam);
						  /* Edit �����¼� */
#endif

