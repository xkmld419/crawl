/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident "@(#)test1_f.c	1.0	2012/03/13	<AutoCreate>" */
#include "test1.h"

Control *test1_handle;

int test1_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    /* Add your self code here: */

    return ch;
}

void test1_entry(Control *pHandle) /* test1 ����ʱ�������� */
{   
    test1_handle = pHandle;
    /* Add your self code here: */


}

int test1Ctl1Press(Control *pCtl,int ch,long lParam)
{   /* ABMϵͳ�������button�İ�����Ϣ���� */
    /* Add your self code here: */

if (ch == '\r') {
	  //hblogin lg;
   // lg.run();
		return FORM_KILL;
	}
    return ch;
}
int test1Ctl2Press(Control *pCtl,int ch,long lParam)
{   /* SQL����button�İ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
int test1Ctl3Press(Control *pCtl,int ch,long lParam)
{   /* Ӧ������button�İ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
int test1Ctl4Press(Control *pCtl,int ch,long lParam)
{   /* ��������ļ�button�İ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
int test1Ctl5Press(Control *pCtl,int ch,long lParam)
{   /* �˳��İ�����Ϣ���� */
    /* Add your self code here: */

if (ch == '\r') {
	  //hblogin lg;
   // lg.run();
		return FORM_KILL;
	}
    return ch;
}

