#ifndef PARAM_MONITOR_F_H
#define PARAM_MONITOR_F_H

#include  <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include "HashList.h"
#include "CommandCom.h"
#include "../app_init/SysParamClient.h"
#include "macro.h"
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

#define MAX_PARAM_SELECT_NUM  1024
#define MAX_PARAM_VAL_LEN  1024 //������С

#define FIR_NAME_LEN			71

#define MAX_ROW_CHAR_NUM		80 //һ������ַ���Ŀ

#define HBPARAM_NAME		63   //<=75
#define HBPARAM_LINE_VAL	70  // <=75

#define MAX_HB_SECT_LEN		256

#define _DBUG_T //���Եĺ��ʶ

struct StdParmMap
{
	int iStart;//��ʼ������ֵ
	int iLen; //����,�����˼���param_list_data
	
	int iParamNameLen; //���������˼���(��Щֵ��*10֮���)
	int iParamName4OrLen; //
	int iParamName4NewLen; //
	int iOldValLen;//ԭֵ����
	int iNewValLen;//��ֵ����
	int iEffDateLen;//��Ч��������
	
	int getEndSeq()//�������±�
	{
		return iStart+iLen-1;
	}
	
};



class HBParamMonitor
{
      public: 
             HBParamMonitor();
             ~HBParamMonitor();
             void init(int iWaitTime,CommandCom *pCmdCom);
			 // ������Ϣ(����ʾ������)  
			 char *param_list_index[MAX_ITEMS];
			 // �����ⲿ����������
			 void linkParamListIndex(char pData[MAX_ITEMS][80],char *pIndex[MAX_ITEMS]);
			 // ʵ�ʴ�����() //������ʽ:sVal����SYSTEM.location=beijing,lCMDIDΪMSG_PARAM_CHANGE
			 long dealParamOpera(char *sSectName, char *sVal, long lCMDID);
			 // ����֮ǰ��ȡ�����ݵ��б��,��0��ʼ
			 long dealParamOpera(int iLine ,char *sNewVal,char *sEffDate, long lCMDID);   
			 int  getRows();    //��ȡ��ʾ����     
			 bool getErrInfo(char *sErrInfo); 
			 void clearEnv(); //����ȫ�����   
	 public: //�������ƺ���		   		 			 
			 char m_sErrInfo[70];//������Ϣ
			 int ruleCheck(long lCMD,char *sWholeVlaue);
			 ParamHisRecord* getParamRecordByOffset(int iOffset);//����			 
			 SysParamClient *m_poCmdOpera; //��������Ĵ���  					   	   			 			   			  			   
			 ParamHisRecord *getParamRecordByLine(int iLine); 					   	   			 			   			  			   
	 private:	 	 		 	 
			 CommandCom *m_poCmdCom; //��initserverͨ�ŵı���		
	 		 int  m_iWaitTime;	//��õȴ�ʱ��
			 char param_list_data[MAX_ITEMS][MAX_PARAM_VAL_LEN]; //��������	 		 
			 ParamHisRecord m_poParamHisRecord[MAX_ITEMS]; //��¼���initserver�Ĳ���������Ϣ,ȫ��sys�����ı���
			 
             int m_iParamNum; // �������� 
			 int m_iRowNum;	  // ��ʾ�õ�����
			 HashList<int> *pIndexList;//��¼�к���������ݵĶ�Ӧ��ϵ
			 //�������
			 BaseRuleMgr *m_poBaseRuleMgr;
	private:		   
			 // ��ȡ������Ϣ������Ű浽param_list_data�Լ�param_list_index
	  	     long GetParamInfoAll();	
			 // ���ַ���Ŀ,data�Ŀ�ʼ�±�;ͬʱ����index��data����ϵ,sStrAdd:Ҫ�ڲ���ǰ���ӵ��ַ�,��'.OldValue='
			 int analyseParamInfo(int &iListPos,char *sParamName,char *sParam,char *sStrAdd,int iSysParamKey);		 
			 
			 // ��������Ϊ0			 			 
			 void resetParams(); 			 
			 void memsetSysInfo();
			 void FormatParamHisRecord();//��ʽ����
			 
	  public: //�����ú���
	  		 void show(long lCMD=0);		 
	  protected:			 			        			 
	  		 void copyParamHisRecord(ParamHisRecord *pPRecord);			   		 		 		   			 
			 ParamHisRecord pParamRollBack[10];
			 int iRoolBackTime;//�ع�����	
			 // ��ʾ�õ�ȫ�ֲ���(�����ڶεĸ���,��Ҳ��¼����Ϣ)<��,TSysParamInfo�±�>	 			 
	 		 static TSysParamInfo *SysParamList; 
};

#endif
