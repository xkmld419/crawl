/*VER: 1*/ 
/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident	"@(#)button.h	1.0	2001/01/11	ruanyongjiang" */
#ifndef _BUTTON_H
#define _BUTTON_H

#include <curses.h>
#include "control.h"       
#include <time.h>
#include <stdlib.h>
#include <string.h>


/*  ���ⲿ�����޹�     defined in button.c */
extern void  BtnChgClr(Control *pButton,int iAttr);     /* button����ʾ�ı� */
extern int   BtnEntry(Control *pButton,long lParam);    /* buttonִ�к������ */
int          BtnKeyPress(Control *pButton,int ich,long lParam);
							/* button�ؼ������¼���� */
#endif


