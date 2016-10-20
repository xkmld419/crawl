/*VER: 2*/ 
/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident "@(#)login_f.c	1.0	2002/03/18	<AutoCreate>" */
#ifndef _LOGIN_F_Q_H_
#define _LOGIN_F_Q_H_
#endif
#include "login.h"
#include "ProcessInfo.h"
#include "mainmenu.h"
#include "password.h"

#include <unistd.h>

Control *login_handle;
THeadInfo *pInfoHead = NULL;
TProcessInfo *pProcInfoHead = NULL;

//�����õ�ȫ�����̺�����(num,fid1,fid2,fid3...)
int g_iflowid[16];
//�����õ�ȫ��App_ID����(num,fid1,fid2,fid3...)
int g_iappid[16];
//ͨ���ַ���������̺�����(����>0�ɹ�)
int gStr2Arr(char *strfid)
{
	char stmp[64] = {0};
	char *p = stmp;
	int itmp = 0;
	//����
	int n = 0;
	int slen = strlen(strfid);
//	printf("\n\n\n\n\n\n\n strfid=%s;\n",strfid);
	//sleep(2);
	if( slen == 0 )
	{
		g_iflowid[0] = 0;
		return 1;
	}
	
	for(int i=0;i<=slen;i++)
	{
		if(strfid[i] == ','||i == slen)
		{
			itmp = atoi( stmp );
//			printf("stmp=%s,itmp=%d\n",stmp,itmp);
			*(++p) = '\0';
			if(itmp)
				g_iflowid[++n] = itmp;
			p = stmp;
			memset(stmp,0,sizeof(stmp));
		}
		else
			*(p++) = strfid[i];
//			printf("stmp=%s;*++p\n",stmp);
	}
	g_iflowid[0] = n;
//	printf("n=%d\n",g_iflowid[0]);
	return 1;
}

//ͨ�����̺��������������ַ���(����>0�ɹ�)
int gArr2Str(char *strfid)
{
	for( int i=0;i<g_iflowid[0];i++ )
	{
		if(i == 0)
			sprintf( strfid,"%d\0",g_iflowid[i + 1] );
		else
			sprintf( strfid,"%s,%d\0",strfid,g_iflowid[i + 1] );
	}
	return 1;
}

//flowid�Ƿ������̺�������(����>0�ɹ�)
int IsInArr(int ifid)
{
	for(int i=1;i<=g_iflowid[0];i++)
	{
		if(ifid == g_iflowid[i])
			return 1;
	}
	return 0;
}

/////////////////////////////////////////
//ͨ���ַ������App_ID����(����>0�ɹ�)
int gStr2Appid(char *strfid)
{
	char stmp[64] = {0};
	char *p = stmp;
	int itmp = 0;
	//����
	int n = 0;
	int slen = strlen(strfid);
//	printf("\n\n\n\n\n\n\n strfid=%s;\n",strfid);
	//sleep(2);
	if( slen == 0 )
	{
		g_iappid[0] = 0;
		return 1;
	}
	
	for(int i=0;i<=slen;i++)
	{
		if(strfid[i] == ','||i == slen)
		{
			itmp = atoi( stmp );
//			printf("stmp=%s,itmp=%d\n",stmp,itmp);
			*(++p) = '\0';
			if(itmp)
				g_iappid[++n] = itmp;
			p = stmp;
			memset(stmp,0,sizeof(stmp));
		}
		else
			*(p++) = strfid[i];
//			printf("stmp=%s;*++p\n",stmp);
	}
	g_iappid[0] = n;
//	printf("n=%d\n",g_iflowid[0]);
	return 1;
}

//ͨ��App_ID�������������ַ���(����>0�ɹ�)
int gAppid2Str(char *strfid)
{
	for( int i=0;i<g_iappid[0];i++ )
	{
		if(i == 0)
			sprintf( strfid,"%d\0",g_iappid[i + 1] );
		else
			sprintf( strfid,"%s,%d\0",strfid,g_iappid[i + 1] );
	}
	return 1;
}


//flowid�Ƿ���AppID������(����>0�ɹ�)
int IsInAppid(int ifid)
{
	for(int i=1;i<=g_iappid[0];i++)
	{
		if(ifid == g_iappid[i])
			return 1;
	}
	return 0;
}

/////////////////////////////////////////

int GetOriPassword (unsigned char sStr[]);
int SetPassword (unsigned char sStr[]);

int login_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    /* Add your self code here: */
	if (ch == 27) { /* KEY_ESC */
			return FORM_KILL;
    }
    return ch;
}

void login_entry(Control *pHandle) /* login ����ʱ�������� */
{   
    login_handle = pHandle;
    /* Add your self code here: */


}

int loginCtl7Press(Control *pCtl,int ch,long lParam)
{   /* ���������İ�����Ϣ���� */
    /* Add your self code here: */

    return ch;
}
int loginCtl8Press(Control *pCtl,int ch,long lParam)
{   /* ȷ�ϰ�ť�İ�����Ϣ���� */
    /* Add your self code here: */
    unsigned char sCurPassword[50], sOldPassword[50];

    if (ch != '\r') return ch;

    GetOriPassword (sOldPassword);
    strcpy ((char *)sCurPassword, (char *) GetCtlByID (login_handle, 7)->sData);

    if (!strcmp ((char *)sOldPassword, (char *)sCurPassword)) {

	DetachShm ();
	if (AttachShm (0, (void **)&pInfoHead) >= 0) {
	    pProcInfoHead = &pInfoHead->ProcInfo;
	} else {
	    prtmsg (pCtl, 19, 21, "���ӹ����ڴ����!");
	    return 0;
	}
	mainmenu mm;
	return mm.run();

    } else {
	prtmsg (pCtl, 19, 21, "Wrong Password!");
    }

    return ch;
}
int loginCtl9Press(Control *pCtl,int ch,long lParam)
{   /* �˳���ť�İ�����Ϣ���� */
    /* Add your self code here: */
	if (ch == '\r') {
		return FORM_KILL;
	}

	return ch;
}
int loginCtl10Press(Control *pCtl,int ch,long lParam)
{   /* �����밴ť�İ�����Ϣ���� */
    /* Add your self code here: */
   
    if (ch == '\r') {
	password pw;
	pw.run();
	FormShow (login_handle);
	return ID (7);
    }

    return ch;
}



int GetOriPassword (unsigned char sStr[])
{
	FILE *fp;
	unsigned char sBuff[256];
	int i, iNum;

	fp = fopen ("xqdata", "rb");
	if (fp == NULL) {
		sStr[0] = 0;
		return 0;
	}

	memset (sBuff, 0, 256);
	fread (sBuff, 256, 1, fp);
	iNum = strlen ((char *)sBuff);
	for (i=0; i<iNum; i++) {
		sStr[i] = ~sBuff[i];
	}
	sStr[i] = 0;
	fclose (fp);

	return 0;
}

int SetPassword (unsigned char sStr[])
{
	FILE *fp;
	int i, iLen;
	char sBuff[256];

	iLen = strlen ((char *)sStr);
	fp = fopen ("xqdata", "wb");
	if (fp == NULL) {
		perror ("SetPassword");
		return -1;
	}
	
	for (i=0; i<iLen; i++) {
		sBuff[i] = ~sStr[i];
	}
	sBuff[i] = 0;
	fwrite (sBuff, strlen ((char *)sBuff), 1, fp);

	fclose (fp);
	return 0;
}

