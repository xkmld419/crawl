/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident "@(#)sysinfo_f.c	1.0	2010/09/16	<AutoCreate>" */
#ifndef _SYSTEM_F_Q_H_
#define _SYSTEM_F_Q_H_
#endif
#include "../app_init/HssParamMonitor.h"
#include "ShroudCurseDefine.h"
#include "sysinfo.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "mainmenu.h"
#include "Environment.h"
#include "ProcessInfo.h"
#include "Log.h"
#include "MBC.h"
Control *sysinfo_handle;

THeadInfo *m_poSysInfoHead = 0;
int m_idisk_space_warning_value = 100;
int m_icpu_warning_value = 100;
int m_itable_space_warning_value = 100;

#define MAX_SYS_INFO_NUM	100
#define HBMONITOR_MAX_STRING_LEN	150


char *sys_list_index[MAX_SYS_INFO_NUM];
char sys_list_data[MAX_SYS_INFO_NUM][HBMONITOR_MAX_STRING_LEN];

int table_list_num = 0;
int g_sys_info_flag = 1;//实时查看使用的标识
int getSysInfo();
void MemsetSysData();
int getInfo();
int getTableSpace();
int getHBmonitorDiskSpace();
void InitWarningInfo();
void Sysinfo_MonitorEnd ();
void getElement(char* src,char delim,int iCnt,char* res);
double atoduble(char* src);
bool GetMemCpuInfo(int &MemTotal,int &MemFree, int &MemPrecent,char* pCpuUser,char* pCpuSys,char* pCpuIdel);


void Sysinfo_MonitorEnd ()
{
    g_sys_info_flag = 0;
}

int sysinfo_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    /* Add your self code here: */
	if (ch == 27) { /* ESC */
    	return FORM_KILL;
    }
    return ch;
}

void sysinfo_entry(Control *pHandle) /* sysinfo 创建时触发调用 */
{   
    sysinfo_handle = pHandle;
    /* Add your self code here: */
	//InitWarningInfo();
	Control *pSysList = GetCtlByID (sysinfo_handle, 6);
	int rows = getSysInfo();
	pSysList->iAdditional = rows;
	pSysList->pData = sys_list_index;

}

int sysinfoCtl1Press(Control *pCtl,int ch,long lParam)
{   /* 性能统计button的按键消息处理 */
    /* Add your self code here: */
	Control *pSysList = GetCtlByID (sysinfo_handle, 6);
	if(ch == '\r')
	{
		//showSysInfo(pCtl,ch);
		int rows = getSysInfo();
		pSysList->iAdditional = rows;
		pSysList->pData = sys_list_index;
		CtlShow (pSysList);
		CtlAtv (pCtl, pCtl);
	}

    return ch;
}
int sysinfoCtl2Press(Control *pCtl,int ch,long lParam)
{   /* 磁盘空间的按键消息处理 */
    /* Add your self code here: */

	Control *pSysList = GetCtlByID (sysinfo_handle, 6);
	if(ch == '\r')
	{
		//showSysInfo(pCtl,ch);
		int rows = getHBmonitorDiskSpace();
		pSysList->iAdditional = rows;
		pSysList->pData = sys_list_index;
		CtlShow (pSysList);
		CtlAtv (pCtl, pCtl);
	}
    return ch;
}
int sysinfoCtl3Press(Control *pCtl,int ch,long lParam)
{   
	 /* 表空间的按键消息处理 */
    /* Add your self code here: */

	Control *pSysList = GetCtlByID (sysinfo_handle, 6);
	if(ch == '\r')
	{
		//showSysInfo(pCtl,ch);
		int rows = getTableSpace();
		pSysList->iAdditional = rows;
		pSysList->pData = sys_list_index;
		CtlShow (pSysList);
		CtlAtv (pCtl, pCtl);
	}
    return ch;
}
    

int sysinfoCtl4Press(Control *pCtl,int ch,long lParam)
{ 
	 /* 退出的按键消息处理 */
    /* Add your self code here: */
	if (ch == '\r') {
		mainmenu mm;
		mm.run();
		return FORM_KILL;
	}

    return ch; 
}

int sysinfoCtl6Press(Control *pCtl,int ch,long lParam)
{   /* 主框架的按键消息处理 */
    /* Add your self code here: */


    return ch;
}

void MemsetSysData()
{
	 for(int i=0;i<MAX_SYS_INFO_NUM;i++)
	 {
	 	memset(sys_list_data[i],'\0',HBMONITOR_MAX_STRING_LEN);
		sys_list_index[i] = 0;
	 }
}

int getInfo()
{
	if (AttachShm (0, (void **)&m_poSysInfoHead) < 0) 
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int getSysInfo()
{ 
	  
		MemsetSysData();
		char cLine[1024] = {0};
		char cLastLine[1024] = {0};
		int iLen = 0;
		char cCommand[200] = {0};
		int iMemTotal = 0;
		int iMemFree = 0;
		int iMemPrecent = 0;
		char cCpuUser[20] = {0};
		char cCpuSys[20] = {0};
		char cCpuIdel[20] = {0};
		char delim = ' ';
		
		if(!GetMemCpuInfo(iMemTotal,iMemFree,iMemPrecent,cCpuUser,cCpuSys,cCpuIdel))
		{
			return 0;
		}
	
		
		/*
				
		fp = popen("vmstat -I","r");
		
		if(fp == NULL)
		{
			return 0;
		}
		
		//得到最后一行
		while(fgets(cLine,1024,fp))
		{
			strcpy(cLastLine,cLine);
		}
		
		pclose(fp);
		
		char delim = ' ';
		
		 //cpu
		 
		 char user[4] = {0};
		 char csystem[4] = {0};
		 char idel[4] = {0};
		 
		 getElement(cLastLine,delim,15,user);
		 getElement(cLastLine,delim,16,csystem);
		 getElement(cLastLine,delim,17,idel);
		 */
		 sprintf(sys_list_data[iLen],"%s\0","内存使用情况：");
			sys_list_index[iLen] = sys_list_data[iLen];
			iLen++;
		 sprintf(sys_list_data[iLen],"%-20s%-20s%-20s\0","Total memory(M)","free memory(M)","Precent(%)");
			sys_list_index[iLen] = sys_list_data[iLen];	
			
		 iLen++;
		 sprintf(sys_list_data[iLen],"%-20d%-20d%-2d%s\0",iMemTotal,iMemFree,iMemPrecent,"%");
			sys_list_index[iLen] = sys_list_data[iLen];	
		  iLen++;
			
		 sprintf(sys_list_data[iLen],"%s\0","CPU使用情况：");
			sys_list_index[iLen] = sys_list_data[iLen];
			iLen++;
		 sprintf(sys_list_data[iLen],"%-20s%-20s%-20s\0","user ","system","idel ");
			sys_list_index[iLen] = sys_list_data[iLen];			
		  iLen++;
		  
		 sprintf(sys_list_data[iLen],"%-20s%-20s%-20s\0",cCpuUser,cCpuSys,cCpuIdel);
			sys_list_index[iLen] = sys_list_data[iLen];	
			iLen++;
			
		#ifdef DEF_AIX	
	  FILE* fp = NULL;
	  fp = popen("iostat -st","r");
		
		if(fp == NULL)
		{
			return 0;
		}
		
		//得到最后一行
		memset(cLine,0,1024);
		while(fgets(cLine,1024,fp))
		{
			strcpy(cLastLine,cLine);
				memset(cLine,0,1024);
		}
		
		pclose(fp);
		
		//I/O
   char cKbps[100] = {0};
   char cTps[100] = {0};
   char cKb_read[100] = {0};
   char cKb_wrtn[100] = {0};
   
   	 getElement(cLastLine,delim,2,cKbps);
		 getElement(cLastLine,delim,3,cTps);
		 getElement(cLastLine,delim,4,cKb_read);
		 getElement(cLastLine,delim,5,cKb_wrtn);
   
    		 sprintf(sys_list_data[iLen],"%s\0","I/O使用情况：");
			sys_list_index[iLen] = sys_list_data[iLen];
			iLen++;
			
		 sprintf(sys_list_data[iLen],"%-10s%-10s%-20s%-20s\0","Kbps: ","tps:","Kb_read: ","Kb_wrtn: ");
			sys_list_index[iLen] = sys_list_data[iLen];	
			iLen++;
			
				 sprintf(sys_list_data[iLen],"%-10s%-10s%-20s%-20s\0",cKbps,cTps,cKb_read,cKb_wrtn);
			sys_list_index[iLen] = sys_list_data[iLen];	
			iLen++;
			
			return iLen;
   
						
		#endif
		
	 #ifdef DEF_HP
	 FILE* fp = NULL;
	  fp = popen("iostat","r");
		
		if(fp == NULL)
		{
			return 0;
		} 
		
		memset(cLine,0,1024);
		//前三行不要
		fgets(cLine,1024,fp);
		fgets(cLine,1024,fp);
		fgets(cLine,1024,fp);
		
		char cbps[100] = {0};
    char csps[100] = {0};
    char cmsps[100] = {0};
    
    float dbps = 0;
    float dsps = 0;
    float dmsps = 0;
		
		while(fgets(cLine,1024,fp))
		{
		  getElement(cLine,delim,2,cbps);
		  getElement(cLine,delim,3,csps);
		  getElement(cLine,delim,4,cmsps); 
		  
		  dbps  += atoduble(cbps); 
		  dsps  += atoduble(csps); 
		  dmsps += atoduble(cmsps); 
				memset(cLine,0,1024);
		}
		
		pclose(fp);
		
   
   
    		 sprintf(sys_list_data[iLen],"%s\0","I/O使用情况：");
			sys_list_index[iLen] = sys_list_data[iLen];
			iLen++;
			
		 sprintf(sys_list_data[iLen],"%-20s%-20s%-20s\0","bps ","tps ","msps ");
			sys_list_index[iLen] = sys_list_data[iLen];	
			iLen++;
			
				 sprintf(sys_list_data[iLen],"%-20.2f%-20.2f%-20.2f\0",dbps,dsps,dmsps);
			sys_list_index[iLen] = sys_list_data[iLen];	
			iLen++;
			
			return iLen;
	 #endif	
		
		
}

//获取磁盘空间
int getHBmonitorDiskSpace()
{
	//INFOPOINT(1,"%s","HBMonitor功能获取系统磁盘空间信息getHBmonitorDiskSpace()");
	/*
	资源类	disk_space_warning_value	监控服务	磁盘空间告警阈值	生效规则：立即 
	取值范围：75～90  
	建议取值：厂家自行确定	当系统的磁盘空间使用率超过预先阀值时告警。
	*/
  MemsetSysData();
	int iPos = 0;
	int iRtn = 0;
	char *sTok="	 \r\n";//tab," ",,"\r","\n";
	char sLine[1024];
	char sTemp[20] = {0};
	char sFileSys[50] = {0};
	char sFree[20] = {0};
	char sUsed[20] = {0};
	int iDisk = 0;
	//float fAvail;
	//printf("------------DISKSPACE---------------\n");
#ifdef DEF_AIX
	FILE* fp = popen("df -k","r");
	if(!fp)
	{
		//布告警信息点
		return -1;
	}
	fgets(sLine,1024,fp);//第一行丢弃

	while(fgets(sLine,1024,fp))
	{
		//strncpy(sFileSys,strtok(sLine," "),50);//Filesystem
		memset(sFileSys,0,50);
		memset(sFree,0,20);
		memset(sUsed,0,20);
		strtok(sLine,sTok);//Filesystem
		strtok(NULL,sTok);//KB blocks
		strncpy(sFree,strtok(NULL,sTok),20);//Free
		strncpy(sUsed,strtok(NULL,sTok),20);//%Used
		strtok(NULL,sTok);//Iused
		strtok(NULL,sTok);//%Iused
		strncpy(sFileSys,strtok(NULL,sTok),15);//Mounted on
		sFileSys[14] = 0;
		sFree[5] = 0;
		sUsed[4] = 0;
		//sprintf(m_sDiskArray[iDisk++],"%-15s   %6s   %5s",sFileSys,sFree,sUsed);
		//printf("%-15s   %6s   %5s\n",sFileSys,sFree,sUsed);
		//if( atoi(sUsed) > m_idisk_space_warning_value )
		//{
		//	iRtn = -1;
		//	SETLOGPRINT;
		//	ALARMLOG28(0,MBC_CLASS_Fatal,10,"[%s]磁盘空间不足,剩余[%sK],使用率[%s%]",
		//		sFileSys,sFree,sUsed);
		//}
		if(sFileSys[0]!='\0' && sFree[0]!='\0' && sUsed[0]!='\0')
			sprintf(sys_list_data[iPos],"%s%s%s%s%s%s%s\0","[",sFileSys,"]磁盘空间,剩余[",sFree,"K],使用率[",sUsed,"]");
			sys_list_index[iPos] = sys_list_data[iPos];
			iPos++;
	}
	pclose(fp);
#endif

#ifdef DEF_HP
	FILE* fp = popen("bdf","r");
	if(!fp)
	{
		return -1;
	}
	fgets(sLine,1024,fp);//
	while(fgets(sLine,HBMONITOR_MAX_STRING_LEN,fp))
	{
		memset(sFileSys,0,50);
		memset(sTemp,0,20);
		memset(sFree,0,20);
		memset(sUsed,0,20);
		//strncpy(sFileSys,strtok(sLine,sTok),50);//Filesystem
		strtok(sLine,sTok);
		strncpy(sTemp,strtok(NULL,sTok),20);//kbytes
		if(sTemp[0] == '\0')
		{
			fgets(sLine,HBMONITOR_MAX_STRING_LEN,fp);
			strtok(sLine,sTok);
		}
		strtok(NULL,sTok);//used
		strncpy(sFree,strtok(NULL,sTok),20);//avail
		strncpy(sUsed,strtok(NULL,sTok),20);//%Used
		strncpy(sFileSys,strtok(NULL,sTok),50);//Mounted on
		//fAvail = atof(sFree)/1024/1024;
		//sprintf(sFree,"%f",fAvail);
		sFileSys[14] = 0;
		sFree[5] = 0;
		sUsed[4] = 0;
		//sprintf(m_sDiskArray[iDisk++],"%-15s   %6s   %5s",sFileSys,sFree,sUsed);
		//printf("%-15s   %6s   %5s\n",sFileSys,sFree,sUsed);
		/*
		if( atoi(sUsed) > m_idisk_space_warning_value )
		{
			iRtn = -1;
			SETLOGPRINT;
			ALARMLOG28(0,MBC_CLASS_Fatal,10,"[%s]磁盘空间不足,剩余[%sK],使用率[%s%]",
				sFileSys,sFree,sUsed);
		}
		*/
		if(sFileSys[0]!='\0' && sFree[0]!='\0' && sUsed[0]!='\0')
			sprintf(sys_list_data[iPos],"%s%s%s%s%s%s%s\0","[",sFileSys,"]磁盘空间,剩余[",sFree,"K],使用率[",sUsed,"]");
			sys_list_index[iPos] = sys_list_data[iPos];
			iPos++;
	}
	pclose(fp);
#endif
	
	//if( iRtn == 0 )
	//	INFOPOINT(1,"%s","磁盘空间状态正常!");	
//printf("------------------------------------\n");
	
	return iPos;
}
int getTableSpace()
{
  MemsetSysData();
	

	char sSql[]="select a.a1 as name, "//--表空间名称
		" c.c2 as type, "//--类型
		" c.c3 as extent, "//--区管理
		" b.b2 / 1024 / 1024 as size_, "//--表空间大小M
		" (b.b2 - a.a2) / 1024 / 1024 as used, "//--已使用M
		" to_number(substr((b.b2 - a.a2) / b.b2 * 100, 1, 5)) as Utilization "//-- 利用率
		" from (select tablespace_name a1, sum(nvl(bytes, 0)) a2"
		" from dba_free_space"
		" group by tablespace_name) a,"
		" (select tablespace_name b1, sum(bytes) b2"
		" from dba_data_files"
		" group by tablespace_name) b,"
		" (select tablespace_name c1, contents c2, extent_management c3"
		" from dba_tablespaces) c"
		" where a.a1 = b.b1"
		" and c.c1 = b.b1";
	TOCIQuery qry(Environment::getDBConn());
	qry.close( );
	qry.setSQL( sSql );
	qry.open( );
	TabSpace oTabSpac;
	char sLineInfo[120] = {0};
	table_list_num = 0;
	while(qry.next( ) )
	{
		sLineInfo[0] = 0;
		strcpy(oTabSpac.m_strName	, qry.field(0).asString());
		strcpy(oTabSpac.m_strType	, qry.field(1).asString());
		strcpy(oTabSpac.m_strExtent , qry.field(2).asString());
		oTabSpac.m_iSize	= qry.field(3).asInteger();
		oTabSpac.m_iUsed	= qry.field(4).asInteger();
		oTabSpac.m_iUtili	= qry.field(5).asInteger();
		sprintf(sLineInfo,"%s%s%s%d%s%d%s","表空间[",oTabSpac.m_strName,"]已用[",oTabSpac.m_iSize,"]使用率[",oTabSpac.m_iUtili,"]");
		
		/*
		if( oTabSpac.m_iUtili > m_itable_space_warning_value )
		{
			SETLOGPRINT;
			ALARMLOG28(0,MBC_CLASS_Fatal,13,"%s",sLineInfo);
		}
		*/
		if(table_list_num<MAX_SYS_INFO_NUM)
		{
			strncpy(sys_list_data[table_list_num],sLineInfo,120);
			sys_list_index[table_list_num] = sys_list_data[table_list_num];
			table_list_num++;
			memset(sLineInfo,0,120);
		}
	}
	qry.close();
	return table_list_num;
}
void InitWarningInfo()
{
	char sDiskWarningValue[12] = {0};
	char sCPUWarningValue[12] = {0};
	char sTableSpaceWarningValue[12] = {0};
	CommandCom *m_pCommandCom = new CommandCom();
	if(!m_pCommandCom || !m_pCommandCom->InitClient())
		return;
	m_pCommandCom->readIniString("WARNING","disk",sDiskWarningValue,"");
	if(sDiskWarningValue[0] != '\0')
		m_idisk_space_warning_value = atoi(sDiskWarningValue);
	m_pCommandCom->readIniString("WARNING","cpu",sCPUWarningValue,"");
	if(sCPUWarningValue[0] != '\0')
		m_icpu_warning_value = atoi(sCPUWarningValue);
	m_pCommandCom->readIniString("WARNING","tablespace",sTableSpaceWarningValue,"");
	if(sTableSpaceWarningValue[0] != '\0')
		m_itable_space_warning_value = atoi(sTableSpaceWarningValue);
	delete m_pCommandCom;
	m_pCommandCom = 0;
}
/*
  函数作用：找到字符串里面第iCnt个被delim分割的字符
  函数入参：
     src  原始字符串
     delim  分割字符
     iCnt   要取第几个字符
    出参：
      res 结果
   
   例如：src=1 234 567  3454
   getElement(src,' ',2,res)
   res = 567
*/
void getElement(char* src,char delim,int iCnt,char* res)
{
  char *p = src;
  int index = 0;
  int Count = 0;
  while(p)
  {
    if(*p == delim)
    {
      Count++;
      
      if(Count == iCnt)
      {
        //找到第一个不是分隔符的字符
        while(*p == delim)
        {
          p++;
        }
        while(*p != delim)
        {
          res[index] = *p;
		  index++;
		  p++;
        }
        return;
      }
      else
      {
        while(*p == delim)
        {
          p++;
        }
      }
    }
    else
    {
      p++;
    }
  }
} 

double atoduble(char* src)
{
	double res = 0;
	bool bAffter = false;;
	
	char *p = src;
	
	int iCount = 0;
	
	while(*p && (*p) != '\n')
	{
		if(*p != '.')
		{

			  res = res*10+((*p)-'0');
		
			if(bAffter)
			{
				iCount++;
			}
		}
		else
		{
			bAffter = true;
		}
		p++;
	}
	
	for(int i = 1;i<= iCount;i++)
	{
		res = res/10;
	}
	
	return res;
}


bool GetMemCpuInfo(int &MemTotal,int &MemFree, int &MemPrecent,char* pCpuUser,char* pCpuSys,char* pCpuIdel)
{
	char ScanBuff[256]={'\0'};
	char sTmpBuff[128]={'\0'};
	char sTmp[128]={'\0'};
	char sTotalMem[128]={'\0'};
	char sFreeMem[128]={'\0'};

	char *p=NULL;
	FILE * fp=NULL;

	#ifdef DEF_LINUX
	strcpy(ScanBuff," free -m|sed -n '2p'|awk '{print $2}' ");
	if((fp = popen(ScanBuff ,"r")) == 0)
	{
		throw TException("DCCollectFactor::PrepareMemPercent Error");
	}
	if (fgets(sTmpBuff,sizeof(sTmpBuff),fp))
	{
		pclose(fp);
		strcpy(sTotalMem,sTmpBuff);
		
	}
	else
	{
		pclose(fp);
		return false;
	}
	#else
	
	#ifdef DFF_AIX
	strcpy(ScanBuff," vmstat |grep \"System\"|grep -v \"grep\"|awk '{print $4}' ");//命令结果为mem=41984MB
	#endif
	#ifdef DEF_HP
	strcpy(ScanBuff," machinfo|grep \"Memory\"|awk '{print $2}' ");//Memory = 16353 MB (15.969727 GB)
	#endif
	if((fp = popen(ScanBuff ,"r")) == 0)
	{
		throw TException("DCCollectFactor::PrepareMemPercent Error");
	}
	if (fgets(sTmpBuff,sizeof(sTmpBuff),fp))
	{
		pclose(fp);
		#ifdef DEF_AIX
		//strcpy(sTmp,&sTmpBuff[4]);//去掉前四位mem=
		p=strstr(sTmpBuff,"=");
		strcpy(sTmp,p+1);
		p=NULL;
		p=index(sTmp,'M');
		strncpy(sTotalMem,sTmp,p-sTmp);//去掉后两位MB
		strcat(sTotalMem,'\0');
		#endif	
		
		#ifdef DEF_HP
		 strcpy(sTotalMem,sTmpBuff);
		#endif	
	}
	else
	{
		pclose(fp);
		return false;
	}
	#endif

	memset(ScanBuff,0x00,sizeof(ScanBuff));

	#ifdef DFF_AIX
	//hpunix vmstat 1 4 |sed -n '3,7p'|awk 'BEGIN{total = 0} {total += $4} END{print total/4/1024}'
	strcpy(ScanBuff," vmstat 1 2|sed -n '8p'|awk '{print $4,$14,$15,$16}' ");//获取FREE内存，已经转化为MB
	#endif
	#ifdef DEF_HP
	strcpy(ScanBuff," vmstat 1 2 |sed -n '4p'|awk '{print $5,$16,$17,$18}' ");//获取FREE内存，已经转化为MB
	#endif
	#ifdef DEF_LINUX
	strcpy(ScanBuff," vmstat 1 2 |sed -n '4p'|awk '{print $5,$13,$14,$15}' ");//获取FREE内存，已经转化为MB	
	#endif
	if((fp = popen(ScanBuff ,"r")) == 0)
	{
		return false;
	}
	if (fscanf(fp,"%s %s %s %s\n",sFreeMem,pCpuUser,pCpuSys,pCpuIdel) == EOF)
	{
		pclose(fp);	
		return false;
	}
	else
	{
		pclose(fp);
		
	}
	// //## 返回使用百分比，精确到整数
	MemTotal = atoi(sTotalMem);
	if(MemTotal == 0)
	{
		 return false;
	}
	MemFree = atoi(sFreeMem)/256;
	
	
	MemPrecent=((MemTotal-MemFree)*100/MemTotal);
	return true;
}