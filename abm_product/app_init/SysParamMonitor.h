#ifndef PARAM_MONITOR_F_H
#define PARAM_MONITOR_F_H

#include  <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include "HashList.h"
#include "CommandCom.h"
#include "macro.h"
#include "SysParamInfo.h"
#include "SysParamDefine.h"

#ifdef ID(_n_)
#undef ID(_n_)
#endif

#ifdef erase
#undef erase
#endif

#ifdef move
#undef move
#endif

#include "BaseFunc.h"
#include "BaseRuleMgr.h"
#include "Log.h"


#define SYS_PARAM_LINE_VAL	74  // <=80
#define MAX_PARAM_VAL_LEN  80

class SysParamMonitor
{
      public: 
             SysParamMonitor();
             ~SysParamMonitor();
             void init(int iWaitTime,CommandCom *pCmdCom);
			 // ������Ϣ(����ʾ������)  
			 char *param_list_index[MAX_ITEMS];
			 int  getRows();    //��ȡ��ʾ����     	
			 SysParamHis *getParamRecordByLine(int iLine);				   	   			 			   			  			   
			 SysParamHis* getParamHisByOffset(int iOffset);	 
			 int dealParamOpera(char *sSectName,char *sParamVal, long lCMDID);	
			 //�������
			 BaseRuleMgr *m_poBaseRuleMgr;
			 int is_zh_ch(char p);			   	   			 			   			  			   
	 private://����ʹ�õĴ���	 20110812 	 		 
			 CommandCom *m_poCmdCom;		
	 		 int  m_iWaitTime;	//��õȴ�ʱ��
			 char param_list_data[MAX_ITEMS][MAX_PARAM_VAL_LEN]; //��������	 		 
			 SysParamHis *m_poSysParamHis;
			 int dealWithMem(char *sSectName, char *sSysParamValue, long lCMDID);
             int m_iCnt; // �������� 
			 int m_iRowNum;	  // ��ʾ�õ�����
			 HashList<int> *pIndexList;//��¼�к���������ݵĶ�Ӧ��ϵ
	private:		   
			 // ��ȡ������Ϣ������Ű浽param_list_data�Լ�param_list_index
	  	     int GetParamInfoAll();	
			 // ���ַ���Ŀ,data�Ŀ�ʼ�±�;ͬʱ����index��data����ϵ,sStrAdd:Ҫ�ڲ���ǰ���ӵ��ַ�,��'.OldValue='
			 int analyseParamInfo(int &iListPos,char *sParamName,char *sParam,char *sStrAdd,int iSysParamKey,int type=1);		 	 
};

#endif
