/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident "@(#)hbmodectl_f.c	1.0	2011/05/26	<AutoCreate>" */
#include "ThreeLogGroup.h"
#include "CheckPointMgr.h"
#include "hbmodectl.h"
#include "../app_mode/mode.h"
#include "mainmenu.h"
#include "CommandCom.h"
#include "Log.h"
Control *hbmodectl_handle;
THssMode *pTHssMode = 0;
CommandCom *pModeCommandCom = 0;
bool bHBModeInit = false;
bool bHBMODEInitClient = false;
int hbmodectl_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    /* Add your self code here: */

    return ch;
}

void hbmodectl_entry(Control *pHandle) /* hbmodectl ����ʱ�������� */
{   
    hbmodectl_handle = pHandle;
    /* Add your self code here: */
	if(!pTHssMode)
		pTHssMode = new THssMode();
	
	pModeCommandCom = new CommandCom();
	if(!pModeCommandCom->InitClient())
		bHBModeInit = false;
	else
		bHBModeInit = true;
	
	pTHssMode->init(60,pModeCommandCom);
	Control *pHBModeList = GetCtlByID (hbmodectl_handle,6);	
	CtlShow (pHBModeList);
}

int hbmodectlCtl1Press(Control *pCtl,int ch,long lParam)
{   /* �鿴button�İ�����Ϣ���� */
    /* Add your self code here: */
	if(ch == '\r')
	{
		char sNowStat[256] = {0};
		char sNowStatUse[256] = {0};
		if(!bHBModeInit)
		{
			prtmsg (pCtl, 20, 12, "ģʽ�л����ܳ�ʼ��ʧ��,���˳��ؽ�!");
			return ch;
		}
		pTHssMode->getNowSysState(_SYS_STATE_ ,sNowStat);
		if(sNowStat[0] != '\0')
		{
			sprintf(sNowStatUse,"%s%s\0","ϵͳ��ǰ״̬: ",sNowStat);
			prtmsg (pCtl, 20, 12, sNowStatUse);
		} else {
			prtmsg (pCtl, 20, 12, "ϵͳ״̬��ȡʧ��,��ǰ״̬δ֪");
		}				
	}
    return ch;
}
int hbmodectlCtl4Press(Control *pCtl,int ch,long lParam)
{   /* �ύbutton�İ�����Ϣ���� */
    /* Add your self code here: */
	if(ch == '\r')
	{
		char sNowStat[256] = {0};
		pTHssMode->getNowSysState(_SYS_STATE_ ,sNowStat);
		if(sNowStat[0] == '\0')
		{
			prtmsg (pCtl, 20, 12, "��ǰ״̬δ֪");
      return ch;
    }
    
    if((strcmp(sNowStat,"offline")==0))
    {
      prtmsg (pCtl, 20, 12, "��ǰϵͳ״̬Ϊoffline�������л�");
      return ch;
    }
    
    
		Control *pEdit = GetCtlByID (hbmodectl_handle, 17);
		if(pEdit->sData[0] != '\0' 
		  && ((!strcmp(pEdit->sData,"online")) || 
		     (!strcmp(pEdit->sData,"offline")) || 
		     (!strcmp(pEdit->sData,"maintenance")))
		  )
    {   /* ��������button�İ�����Ϣ���� */
      
      prtmsg (pCtl, 20, 12, "��ʼ�л�״̬......");	
			int iRes = pTHssMode->changeSysState(_SYS_STATE_,sNowStat,pEdit->sData,true,0,0);
			if(iRes == 1)
			{				
				prtmsg (pCtl, 20, 12, "ϵͳ״̬�л��ɹ�");
			} 
			else 
			{
				prtmsg (pCtl, 20, 12, "ϵͳ״̬�л�ʧ��");
			}
		}
		else
		{
				prtmsg (pCtl, 20, 12, "�������������״̬��ֵ");
		}
	}


    return ch;
}

int hbmodectlCtl8Press(Control *pCtl,int ch,long lParam)
{   /* ����ģʽ�İ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}

int hbmodectlCtl5Press(Control *pCtl,int ch,long lParam)
{   /* �˳��İ�����Ϣ���� */
    /* Add your self code here: */
	if(ch == '\r')
	{
				if(pModeCommandCom)
				{
					delete pModeCommandCom;
					pModeCommandCom = 0;
				}
				mainmenu mm;
				mm.run();
			
			return FORM_KILL;
    }

    return ch;
}
int hbmodectlCtl6Press(Control *pCtl,int ch,long lParam)
{   /* ����ܵİ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
int hbmodectlCtl13Press(Control *pCtl,int ch,long lParam)
{   /* ״̬��ʶ�İ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
int hbmodectlCtl17Press(Control *pCtl,int ch,long lParam)
{   /* NEW_STATE�İ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}

