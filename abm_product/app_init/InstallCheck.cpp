#include "InstallCheck.h"
#include "Log.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>


using namespace std;
InstallCheck::InstallCheck()
{
	err = new char[256];
	m_poCIniFileMgr = new CIniFileMgr();
	m_poHssProcess = new HssProcessInit();
	//m_pCmdCom = new CommandCom();
	m_poBaseRuleMgr = new BaseRuleMgr();
	pProcessCtl = new ShmProcessControl();
	if(!err || !m_poCIniFileMgr || !m_poBaseRuleMgr || !pProcessCtl || !m_poHssProcess)
	{
		Log::log(0,"û�ڴ�ռ���,����!");
		THROW(1);
	}
	/*
	if(!pProcessCtl->Init())
	{
		Log::log(0,"�ڴ��ʼ��ʧ��,����!");
		THROW(1);
	}	
	*/
	m_poSysParamHis = 0;
	//m_pCmdCom->InitClient();
}

InstallCheck::~InstallCheck()
{
	if(m_poCIniFileMgr)
		delete m_poCIniFileMgr;
	if(err)
		delete []err;
	if(m_poBaseRuleMgr)
		delete m_poBaseRuleMgr;
	if(!m_poSysParamHis)
		delete []m_poSysParamHis;
}
int InstallCheck::checkAllParam()
{
	if(getAllParam() <=0 )
		return 0;
	if(!m_poSysParamHis)
		return 0;
	for(int i=0 ;i<m_iCnt ;i++)
	{
		int res  = checkParam(m_poSysParamHis[i].sSectionName,m_poSysParamHis[i].sParamName,m_poSysParamHis[i].sParamValue,m_poSysParamHis[i].sDate);
		if(res != 0)
			return res;
	}
	return 0;
}



int  InstallCheck::getAllParam()
{
	 //
	 /*if(!m_pCmdCom->getSection(NULL,&m_poSysParamHis,m_iCnt))
	 {
	 	return 0;
	 }*/
	 char *p = getenv("BILLDIR");
	 if(!p)
	 	return 0;
	 char filename[1024] = {0};
	 if(p[strlen(p)-1] == '/')
	 {
	 		sprintf(filename,"%s%s",p,"etc/hssconfig");
	 } else {
	 	  sprintf(filename,"%s/%s",p,"etc/hssconfig");
	 }
	 
	 m_poCIniFileMgr->LoadIniFromFile(filename);
	 if(!m_poCIniFileMgr->getSection(NULL,&m_poSysParamHis,m_iCnt))
	 {
	 		return 0;
	 }
	 return m_iCnt;
}

bool  InstallCheck::initprocess(bool usemem)
{
	 //
	 char *pMnt = "mnt -b";
	 if(system(pMnt) != 0){
	 	Log::log(0,"��ʼ��ʧ��,�������!");
		return false;
	 }
	 pMnt = "mnt -r";
	 if(system(pMnt) != 0){
	 	Log::log(0,"���ó�ʼ������ʧ��,��������!");
		return false;
	 }
	 
	 	if(!pProcessCtl->Init())
	 {
		Log::log(0,"����GuardMain�����ڴ�ʧ��,����!");
		THROW(1);
	 }	
	
	
	  Log::log(0,"��ʼ����HSS�ĸ���ģ�顭��");
	  
	  if(m_poHssProcess->getProcess("hssserver")>0)	
	  {
	  	for(vector<HssProInfo>::iterator itr = m_poHssProcess->vec_process.begin();itr!=m_poHssProcess->vec_process.end();itr++)
		{
	  		pProcessCtl->startProcess((int)(*itr).proid);
		}
	  }
	  
	  if(m_poHssProcess->getProcess("hssclient")>0)	
	  {
	  	for(vector<HssProInfo>::iterator itr = m_poHssProcess->vec_process.begin();itr!=m_poHssProcess->vec_process.end();itr++)
		{
	  		pProcessCtl->startProcess((int)(*itr).proid);
	  	}
	  }
	  
	  if(m_poHssProcess->getProcess("hssftp")>0)	
	  {
	  	for(vector<HssProInfo>::iterator itr = m_poHssProcess->vec_process.begin();itr!=m_poHssProcess->vec_process.end();itr++)
		{
	  		pProcessCtl->startProcess((int)(*itr).proid);
	  	}
	  }
	  
	  if(m_poHssProcess->getProcess("hssdccparse")>0)	
	  {
	  	for(vector<HssProInfo>::iterator itr = m_poHssProcess->vec_process.begin();itr!=m_poHssProcess->vec_process.end();itr++)
		{
	  		pProcessCtl->startProcess((int)(*itr).proid);
	  	}
	  }
	  
	  if(m_poHssProcess->getProcess("hssdccpkg")>0)	
	  {
	  	for(vector<HssProInfo>::iterator itr = m_poHssProcess->vec_process.begin();itr!=m_poHssProcess->vec_process.end();itr++)
		{
	  		pProcessCtl->startProcess((int)(*itr).proid);
	  	}
	  }
	 
	  Log::log(0,"���ݽ���ģ��������ϡ�");
	  
	  if(m_poHssProcess->getProcess("hssquery")>0)	
	  {
	  	for(vector<HssProInfo>::iterator itr = m_poHssProcess->vec_process.begin();itr!=m_poHssProcess->vec_process.end();itr++)
		{
	  		pProcessCtl->startProcess((int)(*itr).proid);
	  	}
	  }
	  
	  if(m_poHssProcess->getProcess("hssorder")>0)	
	  {
	  	for(vector<HssProInfo>::iterator itr = m_poHssProcess->vec_process.begin();itr!=m_poHssProcess->vec_process.end();itr++)
		{
	  		pProcessCtl->startProcess((int)(*itr).proid);
	  	}
	  }
		
	  if(m_poHssProcess->getProcess("hss")>0)	
	  {
	  	for(vector<HssProInfo>::iterator itr = m_poHssProcess->vec_process.begin();itr!=m_poHssProcess->vec_process.end();itr++)
		{
	  		pProcessCtl->startProcess((int)(*itr).proid);
	  	}
	  }
	  
	  if(m_poHssProcess->getProcess("hssregist")>0)	
	  {
	  	for(vector<HssProInfo>::iterator itr = m_poHssProcess->vec_process.begin();itr!=m_poHssProcess->vec_process.end();itr++)
		{
	  		pProcessCtl->startProcess((int)(*itr).proid);
	  	}
	  }
	  Log::log(0,"���ݸ���ģ��������ϡ�");
	  
	  /*if(usemem)
	  {
	  	pProcessCtl->StartProc("hssstore");
	  	pProcessCtl->StartProc("hsspublish");
	  } else {
	  	m_poHssProcess->startApp("hssstore");
	  	m_poHssProcess->startApp("hsspublish");
	  }*/
	  Log::log(0,"���ݷ���ģ��������ϡ�");
	  Log::log(0,"������֤�ɹ�");
	 return true;
}

int InstallCheck::checkParam(char *section,char*param,char*value,char*effdate)
{
	 if(section && section[0] != '\0')
	 {
	 	if(param && param[0] != '\0')
		{
			//if(m_poBaseRuleMgr->IsGroupParam(section,param))
			//	return true; // �����
			int res = m_poBaseRuleMgr->checkNowValue(section,param,value,effdate);
			if(res != 0)
			{
				printf("%s%s\n","�������ʧ��,�쳣��Ϣ:",m_poBaseRuleMgr->sErrInfo);
			}
			return res;
		} else {
			printf("%s%s%s\n","��[",section,"]�в�����Ϊ��,��Ч����,����");
		}
	 } else {
	 	//printf("%s\n","����Ч����,����Ϊ��,����");
	 }
	 return 0;
}


