/*VER: 2*/ 
/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident "@(#)mainmenu_f.c	1.0	2007/04/09	<AutoCreate>" */
#ifndef _MAINMENU_Q_H_
#define _MAINMENU_Q_H_
#endif
#include "mainmenu.h"
#include "ProcessInfo.h"
#include "process.h"
#include "ticket.h"
#include "msgqueue.h"
//#include "sharedmemory.h"
#include "paramset.h"
#include "sysinfo.h"
#include "checkpointctl.h"
#include "hbmodectl.h"
#include "logctl.h"
#include "ability.h"

Control *mainmenu_handle;
extern THeadInfo *pInfoHead;
extern TProcessInfo *pProcInfoHead;

extern int g_iflowid[16];
extern int gStr2Arr(char *strfid);
extern int gArr2Str(char *strfid);
extern int IsInArr(int ifid);
extern char g_sUsrName[256];


void setFlowID()
{
	char* sFidtmp = 0;
	sFidtmp = GetCtlByID( mainmenu_handle, 13)->sData;
	if( strcmp( sFidtmp,"*" ) ==0 )
		g_iflowid[0] = 0;
	else
		gStr2Arr(sFidtmp);
}

int mainmenu_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    /* Add your self code here: */
    if (ch == 27) { /* KEY_ESC */
			return FORM_KILL;
    }
    return ch;
}

void mainmenu_entry(Control *pHandle) /* mainmenu ����ʱ�������� */
{   
    mainmenu_handle = pHandle;
    /* Add your self code here: */
   Control *pFlowEdit;
	  pFlowEdit = GetCtlByID(mainmenu_handle, 13);
		
		char tmp[32] = {0};
		if(g_iflowid[0])
		{
			gArr2Str(tmp);
			sprintf( pFlowEdit->sData,"%s\0",tmp );
		}

}

int mainmenuCtl13Press(Control *pCtl,int ch,long lParam)
{   /* ���̺ŵİ�����Ϣ���� */
    /* Add your self code here: */
   		if (ch == '\r') {
			setFlowID();
		}

    return ch;
}
int mainmenuCtl1Press(Control *pCtl,int ch,long lParam)
{   /* ��Ϣ����button�İ�����Ϣ���� */
    /* Add your self code here: */
     if (ch == '\r') {
    	setFlowID();
			msgqueue mq;
			mq.run();
			return FORM_KILL;
    }

    return ch;
}
int mainmenuCtl10Press(Control *pCtl,int ch,long lParam)
{   /* ����״̬button�İ�����Ϣ���� */
    /* Add your self code here: */
    if (ch == '\r') {
    	setFlowID();
			DetachShm ();
			if (AttachShm (0, (void **)&pInfoHead) >= 0) 
			{
			        pProcInfoHead = &pInfoHead->ProcInfo;
			    } else {
			        prtmsg (pCtl, 19, 41, "���ӹ����ڴ����");
			        return 0;
			    }
			
			process proc;
			proc.run();
			return FORM_KILL;
		}
	return ch;
}
int mainmenuCtl2Press(Control *pCtl,int ch,long lParam)
{   /* ����״̬button�İ�����Ϣ���� */
    /* Add your self code here: */
    		if (ch == '\r') {
			setFlowID();
			DetachShm ();
			if (AttachShm (0, (void **)&pInfoHead) >= 0) {
			        pProcInfoHead = &pInfoHead->ProcInfo;
			    } else {
			        prtmsg (pCtl, 19, 41, "���ӹ����ڴ����");
			        return 0;
			    }
			ticket tkt;
			tkt.run();
			return FORM_KILL;
		}

    return ch;
}
int mainmenuCtl3Press(Control *pCtl,int ch,long lParam)
{   /* PAMbutton�İ�����Ϣ���� */
    /* Add your self code here: */
    	if (ch == '\r') {
		paramset paramset;
		paramset.run();
		return FORM_KILL;
	}

    return ch;
}
int mainmenuCtl5Press(Control *pCtl,int ch,long lParam)
{   /* SYSbutton�İ�����Ϣ���� */
    /* Add your self code here: */

    	if(ch == '\r'){
		sysinfo msysinfo;
		msysinfo.run();
		return FORM_KILL;
	}
    return ch;
}
int mainmenuCtl4Press(Control *pCtl,int ch,long lParam)
{   /* �˳��İ�����Ϣ���� */
    /* Add your self code here: */
   	if (ch == '\r') {
    		setFlowID();
			return FORM_KILL;
    }

    return ch;
}
int mainmenuCtl19Press(Control *pCtl,int ch,long lParam)
{   /* checkpoint_but�İ�����Ϣ���� */
    /* Add your self code here: */
    
    	if(ch == '\r'){
		checkpointctl mcheckpointctl;
		mcheckpointctl.run();
		return FORM_KILL;
	}

    return ch;
}
int mainmenuCtl18Press(Control *pCtl,int ch,long lParam)
{   /* ��־_button�İ�����Ϣ���� */
    /* Add your self code here: */
  if( ch == '\r')
	{
		logctl mlogctl;
		mlogctl.run();
		return FORM_KILL;
	}

    return ch;
}
int mainmenuCtl17Press(Control *pCtl,int ch,long lParam)
{   /* ģʽ_button�İ�����Ϣ���� */
    /* Add your self code here: */
   if( ch == '\r')
	 {
		 hbmodectl mhbmodectl;
		 mhbmodectl.run();
		 return FORM_KILL;
	 }

    return ch;
}

int mainmenuCtl15Press(Control *pCtl,int ch,long lParam)
{   /* ��������_button�İ�����Ϣ���� */
    /* Add your self code here: */
   if( ch == '\r')
	 {
		 ability abilityctl;
		 abilityctl.run();
		 return FORM_KILL;
	 }

    return ch;
}
int mainmenuCtl6Press(Control *pCtl,int ch,long lParam)
{   /* ����ܵİ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
