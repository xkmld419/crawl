/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident "@(#)checkpointctl_f.c	1.0	2010/09/30	<AutoCreate>" */
#include "ThreeLogGroup.h"
#include "CheckPointMgr.h"
#include "checkpointctl.h"
#include "mainmenu.h"
//#include "TOCIQuery.h"
//#include "Environment.h"
//#include "BaseFunc.h"
//#include "Log.h"
#include <signal.h>
#include <time.h>
#include <unistd.h>
//#include "Log.h"
#include <string.h>

#define MAX_LIST_NUM  1024

#define MAX_CHECK_POINT_INFO_NUM    15000 
extern char g_sUsrName[256];

Control *checkpointctl_handle;
ThreeLogGroup* pCtlThreeLogGroup = NULL;


char *batchinfo_list_index[MAX_CHECK_POINT_INFO_NUM];
char batchinfo_list_data[MAX_CHECK_POINT_INFO_NUM][80];
int iBatchInfoNum = 0;

int GetCheckPointInfo(char* cTime);
int CheckTime(const char* pchString);
int checkpointctl_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    /* Add your self code here: */
    if (ch == 27) { /* KEY_ESC */
			return FORM_KILL;
    }
    return ch;
}

void checkpointctl_entry(Control *pHandle) /* checkpointctl ����ʱ�������� */
{   
    checkpointctl_handle = pHandle;
    /* Add your self code here: */
	   pCtlThreeLogGroup = NULL;
    pCtlThreeLogGroup = new ThreeLogGroup();
    if(pCtlThreeLogGroup == NULL)
    {
    	prtmsg (pHandle, 15, 5, "��ʼ����־��ʧ��");
    }
    
    int rows = GetCheckPointInfo(NULL);

    Control *pCheckPointList = GetCtlByID (checkpointctl_handle,6);	
    pCheckPointList->iAdditional = rows;
	pCheckPointList->pData = batchinfo_list_index;		
	CtlShow (pCheckPointList);
}

int checkpointctlCtl1Press(Control *pCtl,int ch,long lParam)
{   /* �鿴button�İ�����Ϣ���� */
    /* Add your self code here: */
	if (ch == '\r') 
    {
    	Control *pDateEdit = GetCtlByID(checkpointctl_handle, 13);//ʱ��
    	
    	if(pDateEdit->sData[0])
		  {
			  if(CheckTime(pDateEdit->sData) < 0)
			  {
			   	prtmsg (pCtl, 20, 40, "ʱ���ʽ����ȷ");
				  return ch;
			  }
		  }
    	 
    
			int iResLine = 0;
			iResLine = GetCheckPointInfo(pDateEdit->sData);
 
      Control *pCheckPointList = GetCtlByID (checkpointctl_handle,6);	
      pCheckPointList->iAdditional = iResLine;
	    pCheckPointList->pData = batchinfo_list_index;		
	    CtlShow (pCheckPointList);
    }
    return ch;
}
int checkpointctlCtl2Press(Control *pCtl,int ch,long lParam)
{     /* �˳��İ�����Ϣ���� */
    /* Add your self code here: */
	if (ch == '\r') {
			mainmenu mm;
			mm.run();
		
		return FORM_KILL;
	}
    return ch;
}
int checkpointctlCtl6Press(Control *pCtl,int ch,long lParam)
{   /* ����ܵİ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
int checkpointctlCtl13Press(Control *pCtl,int ch,long lParam)
{   /* ���κŵİ�����Ϣ���� */
    /* Add your self code here: */
	if (ch == '\r') {
		//	checkpointctlCtl1Press(pCtl,ch,lParam);
			//Control *pBatchEdit = GetCtlByID(checkpointctl_handle, 13);//��
    		//AllTrimN(pBatchEdit->sData);
	}

    return ch;
}

int GetCheckPointInfo(char* cTime)
{
	char cDate[15];
	memset(cDate,0,15);
	if(cTime[0])
	{
		strcpy(cDate,cTime);
	}
	else
	{
		strcpy(cDate,"20110701000000");
	}
	
	for(int i=0;i<MAX_LIST_NUM;i++)
    {
        memset(batchinfo_list_data[i],'\0',80);
        batchinfo_list_index[i] = 0;
    }
 
    if(pCtlThreeLogGroup == NULL)
    return 0;
    
    vector<LogInfoData> vec_logInfo;
    bool Res = pCtlThreeLogGroup->GetLoggInfo(vec_logInfo,GROUPCODE,MBC_ACTION_CHECK_POINT,cDate);
 
    if(Res)
    {
		int iPos = 0;
		    
		    sprintf(batchinfo_list_data[iPos],"%-15s%-20s%","���ʱ��","�������");
			  batchinfo_list_index[iPos] = batchinfo_list_data[iPos];
			  iPos++;
        for(vector<LogInfoData>::iterator itr=vec_logInfo.begin();itr!=vec_logInfo.end();itr++)
        {
			   if(iPos==MAX_LIST_NUM)
				  break;
					
			    sprintf(batchinfo_list_data[iPos],"%-15s%-20s%",itr->m_sLogTime,itr->m_sInfo);
			    batchinfo_list_index[iPos] = batchinfo_list_data[iPos];
			    iPos++;
        }
        return iPos;
    }
    else {
        return -1;
    }
}

int CheckTime(const char* pchString)
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