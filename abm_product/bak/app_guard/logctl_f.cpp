/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident "@(#)logctl_f.c	1.0	2010/10/02	<AutoCreate>" */
#include "ThreeLogGroup.h"
//#include "logctl.h"
//#include "Log.h"
#include "mainmenu.h"
//#include "LogFileMgr.h"
//#include "../app_init/BaseFunc.h"

Control *logctl_handle;

#define HBLOG_LIST_NUM  1024
#define HBLOG_LINE_LEN  65

char *hblog_list_index[HBLOG_LIST_NUM];
char hblog_list_data[HBLOG_LIST_NUM][80];
ThreeLogGroup* pThreeLogGroup = NULL;
 
int getLogInfo(int iProcessID,char* pDate);
int checkTime(const char* pchString);


int logctl_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    /* Add your self code here: */
	if (ch == 27) { /* KEY_ESC */
			return FORM_KILL;
    }
    return ch;
}

void logctl_entry(Control *pHandle) /* logctl ����ʱ�������� */
{   
    logctl_handle = pHandle;
    /* Add your self code here: */
    pThreeLogGroup = NULL;
    pThreeLogGroup = new ThreeLogGroup();
    if(pThreeLogGroup == NULL)
    {
    	prtmsg (pHandle, 15, 5, "��ʼ����־��ʧ��");
    }

}

int logctlCtl1Press(Control *pCtl,int ch,long lParam)
{   /* �鿴button�İ�����Ϣ���� */
    /* Add your self code here: */
	if (ch == '\r') {
		Control *pList = GetCtlByID (logctl_handle, 6);
		Control *pProcessEdit = GetCtlByID(logctl_handle, 13);//��
		Control *pDateEdit = GetCtlByID(logctl_handle, 15);//ʱ��
		
		if(pDateEdit->sData[0])
		{
			if(checkTime(pDateEdit->sData) < 0)
			{
				prtmsg (pCtl, 20, 40, "ʱ���ʽ����ȷ");
				return ch;
			}
		}
		int rows = 0;
		int iProcess_id = atoi(pProcessEdit->sData);
		if(iProcess_id == 0)
		{
				prtmsg (pCtl, 20, 40, "����ID����ȷ");
				return ch;
		}
		rows = getLogInfo(atoi(pProcessEdit->sData),pDateEdit->sData);
		pList->iAdditional = rows;
		pList->pData = hblog_list_index;
 
		CtlShow(pList);
		CtlAtv( pCtl,pCtl );
    }

    return ch;
}
int logctlCtl3Press(Control *pCtl,int ch,long lParam)
{   /* �˳��İ�����Ϣ���� */
    /* Add your self code here: */
	if (ch == '\r') {
			mainmenu mm;
        	mm.run();
			return FORM_KILL;
    }
    return ch;
}
int logctlCtl6Press(Control *pCtl,int ch,long lParam)
{   /* ����ܵİ�����Ϣ���� */
    /* Add your self code here: */
    return ch;
}
int logctlCtl13Press(Control *pCtl,int ch,long lParam)
{   /* ϵͳ��־�İ�����Ϣ���� */
    /* Add your self code here: */
	if(ch == '\r')
	{
		logctlCtl1Press(pCtl, ch, lParam);
	}
    return ch;
}

int logctlCtl15Press(Control *pCtl,int ch,long lParam)
{   /* ��־ʱ��İ�����Ϣ���� */
    /* Add your self code here: */
	if(ch == '\r')
	{
		logctlCtl1Press(pCtl, ch, lParam);
	}

    return ch;
}

int getLogInfo(int iProcessID,char* pDate)
{
	  for(int i=0;i<HBLOG_LIST_NUM;i++)
    {
        memset(hblog_list_data[i],'\0',80);
        hblog_list_index[i] = 0;
    }
	  if(iProcessID<=0)
		return 0;
    if(pThreeLogGroup == NULL)
    return 0;
    
    vector<LogInfoData> vec_logInfo;
    bool Res = pThreeLogGroup->GetLoggInfo(vec_logInfo,GROUPPROCESS,iProcessID,pDate);
   // getLogData(iProcessID,vec_logStr);
    
    if(Res)
    {
		int iPos = 0;
		    
		    sprintf(hblog_list_data[iPos],"%-20s%-20s%","��־ʱ��","��־����");
			  hblog_list_index[iPos] = hblog_list_data[iPos];
			  iPos++;
        for(vector<LogInfoData>::iterator itr=vec_logInfo.begin();itr!=vec_logInfo.end();itr++)
        {
			   if(iPos==HBLOG_LIST_NUM)
				  break;
					
			    sprintf(hblog_list_data[iPos],"%-20s%-20s%",itr->m_sLogTime,itr->m_sInfo);
			    hblog_list_index[iPos] = hblog_list_data[iPos];
			    iPos++;
        }
        return iPos;
    }
    else {
        return -1;
    }
}

int checkTime(const char* pchString)
{
    int num,i;
    char tmp[10];

    if(strlen(pchString) != 14)
        return -1;
    int iLen = strlen(pchString);
    for (i=0;i<iLen;i++)
    {
        if ((pchString[i]<'0')||(pchString[i]>'9'))
            return -1;
    }
    strncpy(tmp , pchString , 2);
    tmp[2] = 0;
    num = atoi(tmp);
    if((num < 0) || (num > 23))
        return -1;
    strncpy(tmp , pchString + 2 , 2);
    tmp[2] = 0;
    num = atoi(tmp);
    if((num < 0) || (num > 59))
        return -1;
    strncpy(tmp , pchString + 4 , 2);
    tmp[2] = 0;
    num = atoi(tmp);
    if((num < 0) || (num > 59))
        return -1;

    return 1;
}
