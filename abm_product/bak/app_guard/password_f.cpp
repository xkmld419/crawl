/*VER: 1*/ 
/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident "@(#)password_f.c	1.0	2002/03/25	<AutoCreate>" */
#include "password.h"

Control *password_handle;
int GetOriPassword (unsigned char sStr[]);
int SetPassword (unsigned char sStr[]);

int password_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    /* Add your self code here: */

    return ch;
}

void password_entry(Control *pHandle) /* password ����ʱ�������� */
{   
    password_handle = pHandle;
    /* Add your self code here: */


}

int passwordCtl7Press(Control *pCtl,int ch,long lParam)
{   /* ԭ���������İ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
int passwordCtl8Press(Control *pCtl,int ch,long lParam)
{   /* �����������İ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
int passwordCtl9Press(Control *pCtl,int ch,long lParam)
{   /* ȷ�������������İ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
int passwordCtl10Press(Control *pCtl,int ch,long lParam)
{   /* ȷ�ϰ�ť�İ�����Ϣ���� */
    /* Add your self code here: */

    char sOriPassword[50];

    if (ch != '\r') return ch;

    GetOriPassword ((unsigned char *)sOriPassword);
    if (strcmp (sOriPassword, GetCtlByID (password_handle, 7)->sData)) {
	prtmsg (pCtl, 19, 21, "�������!");
	return ch;
    }
	
    if (strcmp (
	GetCtlByID (password_handle, 8)->sData,
	GetCtlByID (password_handle, 9)->sData
	)) {
	prtmsg (pCtl, 19, 21, "�������������벻��");
	return ch;
    }

    SetPassword ((unsigned char *)GetCtlByID (password_handle, 8)->sData);
    prtmsg (pCtl, 19, 21, "���������óɹ�");
    return FORM_KILL;

}
int passwordCtl11Press(Control *pCtl,int ch,long lParam)
{   /* �˳���ť�İ�����Ϣ���� */
    /* Add your self code here: */

    if (ch == '\r') {
	return FORM_KILL;
    }

    return ch;
}

