/* Copyright (c) 2001-<2010> Linkage, All rights Reserved */
/* #ident "@(#)hbmainmenu_f.c	1.0	2010/05/06	<AutoCreate>" */
//#include "abmbilltables.h"
#include "abmmainmenu.h"
#include "abmsqlfunc.h"
#include "abmlogin.h"
#include "abmappcommand.h"
#include "abmtablesmenu.h"
#include "abmclear.h"
bool UserFlag;
char sFullStaffID[50];
//���ӵ�½�û����Ȩ
#include "GroupDroitMgr.h"

#define LINENUM  512


char * hbtrim( char *sp )
{
	char sDest[LINENUM];
	char *pStr;
	int i = 0;

	if ( sp == NULL )
		return NULL;

	pStr = sp;
	while ( *pStr == ' ' || *pStr == '\t' )  pStr ++;
	strcpy( sDest, pStr );

	i = strlen( sDest ) - 1;
	while((i>=0)&&(sDest[i] == ' ' || sDest[i] == '\t' || sDest[i] == '\r'|| sDest[i] == '\n' ) )
	{
		sDest[i] = '\0';
		i -- ;
	}

	strcpy( sp, sDest );

	return ( sp );
}

Control *hbmainmenu_handle;

int hbmainmenu_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    /* Add your self code here: */

    return ch;
}

void hbmainmenu_entry(Control *pHandle) /* hbmainmenu ����ʱ�������� */
{   
    hbmainmenu_handle = pHandle;
    /* Add your self code here: */
    /*
    GroupDroitMgr *MDroitMgr = new GroupDroitMgr();
	strcpy(sFullStaffID,MDroitMgr->getUsrName());
	UserFlag = MDroitMgr->IsRootUsr();
	*/
	UserFlag=true;
}

int hbmainmenuCtl1Press(Control *pCtl,int ch,long lParam)
{   /* �Ʒѽ��㹦�ܱ�button�İ�����Ϣ���� */
    /* Add your self code here: */
    if (ch == '\r') {
    	 hbtablesmenu tm;
    	tm.run() ;
		return FORM_KILL;
	 }

    return ch;
}

int hbmainmenuCtl2Press(Control *pCtl,int ch,long lParam)
{   /* SQL����button�İ�����Ϣ���� */
    /* Add your self code here: */

	if (ch == '\r') {
		hbsqlfunc sf;
		sf.run() ;
		return FORM_KILL;
	}

    return ch;
}

int hbmainmenuCtl3Press(Control *pCtl,int ch,long lParam)
{   /* Ӧ������button�İ�����Ϣ���� */
    /* Add your self code here: */
  if (ch == '\r') {
  	hbappcommand ac ;
  	ac.run();
		return FORM_KILL;
	}
 
    return ch;
}

int hbmainmenuCtl4Press(Control *pCtl,int ch,long lParam)
{   /* �˳��İ�����Ϣ���� */
    /* Add your self code here: */

 if (ch == '\r') {
	  hbclear hc ;
	  hc.run() ;
		return FORM_KILL;
	}
    return ch;
}

int hbmainmenuCtl5Press(Control *pCtl,int ch,long lParam)
{   /* �˳��İ�����Ϣ���� */
    /* Add your self code here: */

 if (ch == '\r') {
	  //hblogin lg;
   // lg.run();
		return FORM_KILL;
	}
    return ch;
}


