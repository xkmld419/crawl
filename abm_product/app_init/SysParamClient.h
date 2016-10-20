#ifndef SYSPARAMCLIENT_H_HEADER_INCLUDED_BDD
#define SYSPARAMCLIENT_H_HEADER_INCLUDED_BDD
#include <vector>

//#include "CmdMsg.h"
#include "interrupt.h"
#include <iostream>
#include  <stdlib.h>
#include "syscfg.h"
#include "MsgDefineClient.h"
#include "MsgDefineServer.h"
#include "SysParamInfo.h"
#include "SysParamDefine.h"
#include "BaseFunc.h"
#include "CommandCom.h"
#include "Log.h"
#include "IniFileMgr.h"
#include "BaseRuleMgr.h"

#include <map>

#ifdef _ARBITRATION_
#include "../app_accustd/ArbitrationMgr.h"
#include "../cai/interrupt_A.h"
#endif
#define PAGE_MAX_NUM   7    // һҳ�������ʾ����Ϣ���д�����CMDMSG��Ŀ
#define MAX_WAIT_LONG_TIME  30   // �ȴ�������Ϣ��ʱ��,��λ��1��
#define MAX_WAIT_T  10
#define PAGE_MAX_SHOW_NUM   12    // һҳ�������ʾ�Ĳ�����Ŀ

#define HBPARAM		"HBPARAM"
#define LIST_LISTALL	"LIST_LISTALL"

class SysParamClient 
{
      public:
             SysParamClient();
			 SysParamClient(int iMode);
             ~SysParamClient();
			 int resolveHBParam(char **Argv,int Argc);
			 // ��ʼ������
			 void init(CommandCom *pCmdCom,int iWaitTime=MAX_WAIT_T);
			 // �ϲ�����,��Ҫ�ǽ����������
			 int resolveUsrParam(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc);
			 
			 // ��������ʽ�Լ���ֵ�Ƿ���Ϲ涨
             bool IfSuitRule(long lCMDID,char *sParamSetVal,string &strDsc,bool Show=true);
			 
	 		 //�¹���
			 int resolveMemParam(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc);
			 int dealWithMem(char *sSectName,char *sSysParamValue,long lCMDID,bool ShowErrInfo);
			 int dealMem(char *sSectName, string sSysParamValue, long lCMDID,bool Show);
			 //��ʾ������
			 long showMemMsgValues();
			 SysParamHis *m_poSysParamHis;
			 SysParamHis *m_poSysParamHisEx;
			 int m_iCnt;
			 int m_iCntEx;
			 unsigned int m_iErrNo;
			 
			 //MEMORY_DB ����ʾ
			 bool prompt(string sysparam,bool show = false);
			 // �����ṩ������Ϣ
			 void showHowToUseCMD();
			 void showResults( void *pData, int iNum);
			 // ��������
			 void reSetBaseParams();
			 // ������ʽת��
			 long VecToSysParamInfo(ParamHisRecord *pParamHisRecord,int &iParamNum,long lCMDID);
			 //
	         int resolveParam(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc);
	  		 //��ù����������մ���initserver������strDsc
	  		 bool paramParser(char *sAllParamVal,string &strDsc);
			 ParamHisRecord mRecord[1];//������Ļ�������
	  public:
	  		 bool change2hssString(char *value,string &strdesc);
			 //ThreeLogGroup TLog;
	  public: //monitor	  		 
			 // ר��Ϊmonitor���ĺ���[��Ҫ��ȥ���˲��ֲ���Ҫ����֤�Լ�������ʾ]
			 int deal4Monitor(char *sParamName, string sSysParamValue, long lCMDID, CommandCom *pCmdCom);	 	
			 int deal4Monitor(char *sParamName, string sSysParamValue, long lCMDID);
			 //
			 void setDebugState(bool _Debug);//_DebugΪtrue����ʾ������ʾ[��ʾ����Ļ�monitor����������]			 	 
	  public:
	  		 //ǰ̨����
			 void setFrontInfo(bool bFront,int iBegin, int iEnd, const char *sSplit);		 	 
			 long m_lSrcCMDID;//��¼ÿ�β���������ID			 	 
	  private:
	         ////ǰ̨��صĿ���
	         bool m_bFront;
	         int  m_iBegin;
	         int  m_iEnd;
	         const char *m_pSplit;   
	  		 /* ��δ����ǻ�������*/
			 bool m_bSetParams;
			 bool m_bRefreshParams;
			 bool m_bListParams;
			 bool m_bCreateParams;
			 bool m_bDeleteParams;
			 bool m_bListAllParams;
			 bool m_bShowHelp;
			 
			 //long m_lSrcCMDID;//��¼ÿ�β���������ID	
			 int  m_iArgc;//��¼ÿ�εĲ�������			 			 
			 //����initserver�Ĳ���
			 CommandCom *m_poCmdCom; 	
			 
			 //�û�����(ȥ����hbparam -set��Щ֮�������,ʵ����������)				 
			 char m_sParamsValue[ARG_VAL_LEN];
			 			 
			 //�������,ͳһȥ����̬������ʹ��
			 BaseRuleMgr *m_poBaseRuleMgr;				 	  		 
	  private:		 		 				 	 
			 // ��������,��������������sParamName
			 void analyseParamName(char *sParamValue,char *sParamName);	
			 // ������ʽ���,��Ҫ������ݵĻ�����ʽ��
			 bool checkBaseFormat(char *sParamDate,long lCMD);
			 //��ʾһҳ����
			 void showOnePage(SysParamHis *pData,int iNowPos,int iTotalNum);
			 //������ҳ��ʾ����	 
			 int showControl(int &iPos);
			 SysParamHis *m_pSysParamGet;
	  public://�����ٲ�ʹ�� ���ֻ���в����ٲ�ƽ̨�Ļ���ʹ��
	         #ifdef _ARBITRATION_
	  		 	ArbitrationMgr *m_pArbitrationMgr;
			 	int m_iState;
			 #endif
			 void send2Arbi(string str);
	  		 bool arbi;
			 bool getArbiResult(char *sValue);	
			 void recData();
	  
	  public:	 //2.8�������԰汾ʹ�� �����Ѳ�ʹ��	 			 
			 int deal(char *sParamName, string sSysParamValue, long lCMDID, CommandCom *pCmdCom);//��initserverͨ��
			 int deal(char *sParamName, string sSysParamValue, long lCMDID,bool Show=true);			 			  		 			 
			 		 		 
			 // �����Ϣ��������	
			 void clearMsg();
			 // ʱ��ȴ�����
			 int waitTimer(int &iTimer,long lCMDID,bool Show=false);
			 // ���ȴ�ʱ��
			 int m_iWaitMax;
			 //������Ϣ�Ļ���,�������ɴ�����Щ����	 
			 //vector<CmdMsg*> vec_CmdMsg; 			 
			 // ���vector����			 			 
			 void clearPtr(); 	
			 int m_iVecNum;//vec_CmdMsg�Ĳ�������
			 //��initserver ͨ�ŵĻ�������[һ��ͨ��]
	 	  	 int dealWithServer(char *sSectName,char *sSysParamValue,long lCMDID,bool ShowErrInfo=true);	 
			 //��ʾ������
			 long showMsgValues(bool Show=true);	//��ʾ����
			 // ��ʼ������
			 void initParam(char *sSectionName);
			 char *getInitParamValue();
			 char m_sInitParamValue[2048];
			 char m_sInitSectionName[2048];
	  private://2.8�����汾 �����Ѳ�ʹ��	 
			 //void showAgain(int i,vector<CmdMsg*>::iterator it);
			
			 bool checkFrontSet(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc);
			 bool getFrontBeginEnd(int begineOffset, int endOffset,char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc);
	  public://���Ժ���
	  		 void show();
	  public: //��Щ����ǰ�ϲ�����Ĺ���,���ڲ��ֲ�ֳ�ȥ ����ʹ��
	         /* ��Щ�ǻ�ȡ���ݵ� */
			 char sLogLevel[512];
			 void setLogLevel();
			 bool m_bLogOut;
			 //bool paramFormatCheck(string sParamVal,long lCMD); // ������ʽ����
			 bool check();	// ������	
			 bool m_bMONDebug;//TRUE����ʾ������ʾ	
			 bool ParamLink; //��Ԫ��	
			 char m_sParamSetVal[ARG_VAL_LEN*2];//�û�����(ȥ����hbparam -set��Щ֮�������,ʵ����������)	
			 char m_sMONErrInfo[70];//������Ϣ��¼				 	
			 int dealParam(char *sParamName, string sSysParamValue, long lCMDID);	
			 // �������Ƿ���Ϲ�������ֵ
			 int ruleCheck(long lCMDID,char *m_sSysParamValue);	
			 
};

#endif

