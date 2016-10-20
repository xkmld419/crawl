#ifndef _LOG_GROUP_H
#define _LOG_GROUP_H

#ifdef DEF_HP
#define _XOPEN_SOURCE_EXTENDED
#endif

#include "Log.h"
#include "MBC.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include "ReadIni.h"
#include "Exception.h"
#include "Environment.h"
#include "ThreeLogGroup.h"
#include "hbcfg.h"
#include "CheckPointMgr.h"
//#include "HbAdminMgr.h"

#define GROUPCREAT   1               //���������ڴ�
#define GROUPFREE    2               //�ͷŹ����ڴ�
#define GROUPADD     3               //�����־���Ա�ļ�
#define GROUPDELETE  4               //ɾ����־���Ա�ļ�
#define GROUPPRINTF  5               //��ʾ��־����Ϣ
#define GROUPQUIT    6               //�л���־��
#define GROUPSET     7               //���ù鵵
#define GROUPBAK     8               //�ֶ������ļ�
#define GROUPOUT     9               //����ļ���Ϣ
#define FILECDOWN    10              //checkpoint�ĵ���
#define FILECLOAD    11              //checkpoint�ļ���
#define FILETEST     12              //��������
#define PIGEONHOLE   13              //�ֶ��鵵
#define GROUPHELP    14              //��������ֲ�
#define GROUPMODULE  15              //��ģ�����Ʋ�ѯ��־
#define GROUPPROCESS 16              //���������Ʋ�ѯ��־
#define GROUPCHECKPT 17              //��ѯcheckpoint��־
#define GROUPINFOLV  18              //����־�ȼ���ѯ��־
#define GROUPCLASS   19              //��������Ʋ�ѯ��־
#define GROUPSTAFFID 20              //�����Ų�ѯ��־
#define GROUPTABNAME 21              //��������ѯ��־
#define GROUPSQLTYPE 22              //����������ѯ��־
#define GROUPPARAM   23              //����������ѯ��־

#define DISOPERAT    95              //����չ��-class-ϵͳҵ����̴���
#define DISPARAM     96              //����չ��-class-��������
#define DISSYS       97              //����չ��-class-ϵͳ����
#define DISATTEMP    98              //����չ��-class-���̵���
#define DISACCESS    99              //����չ��-class-���ݿ����
#define DISFORALL   100              //����չ��-class-ȫ��

#define LIENUM       (ParamDefineMgr::getLongParam("HBLOGG","LIE_NUM"))

//������ʾ
//typedef int  (* GETNUM)(void);//����ָ��

struct Log_Menu
{
    int Index;          //����
    int iNum;
//    GETNUM pFunc;       //��ȡ���ݵĺ���ָ��
    char	ChName[100];  //�������� 
    
    Log_Menu()
    {
        Index = 0;
        iNum = 0;
        memset(ChName,0,sizeof(ChName));
    }
};

#define COLS_LENS 120

class DisplayLogg
{
public:
	static void DisInit();
	static void Loggstartwin();
	static void LoggLoadGeneralInfo();
	static void LoggLoadEmptyMenu();
    static void LoggStartProblemProc();
	static void LoggloadAll(vector<string> &m_vInfo);
	static void LoggInitAll_1();
	static void LoggStartProblemProcAll();
	static void LoggInitProcessInfo(int flag);
	static void LoggProcessMain_1();
    static int LoggProcessMain(int flag);

	static void LoggRebuildProblemProc();
	static void LoggRebuildProcessAll();
	static void LoggRebuildProcess();
	
	static void LoggTimeBreak_1(int signo);
	static void LoggTimeBreak_ProblemProc(int signo);
	static void LoggTimeBreak_Process(int signo);
	static void LoggTimeBreak_ProblemProcAll(int signo);

private:
	static int INTERVAL;
	static int iLoggPageSize;
	static int Logg_Main2SelectedRow;
	static int Logg_Main2StartRow ;
	static int Logg_Main3SelectedRow;
	static int Logg_Main3StartRow;
	static int iDisParamCnt;
	static int iDisParamCnt_ALL;
	static int Logg_ProblemProcSelectedRow;
	static int Logg_ProblemProcStartRow;
	static int NOW_USED;
	static Log_Menu* Logg_pMenuInfo;
	static Log_Menu* Logg_pMenuInfoAll;
	static int LogFlagAll;
};

class LogGroup : public DisplayLogg
{
public:
    LogGroup();
    ~LogGroup();

    bool init();
    int  Deal();
//    void ParamMain(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc);
    bool prepare(char g_argv[ARG_NUM][ARG_VAL_LEN],int g_argc);

private:
    void printUsage();

    bool IfCDisplayInfo();
    bool AnalyzesParam(char *sParam,char *sReturn);
    bool AnalyInfoByFile(int LogFlag,int StandId1,char *sStandId1,vector<string> &vDisplayInfo);
    bool AnalyAlarmInfoFromFile(char *sStandId1,vector<string> &vDisplayInfo);
    bool AnalyParamInfo(int LogFlag,char *sStandId1,vector<string> &vParamInfo,vector < string > & vDisInfo);
//	bool GetProcessIdByName(char *sParam,char *sReturn);
    bool GetAppIdByName(char *sParam,char *sReturn);
    bool GetModuleIdByName(char *sParam,char *sReturn);
    bool CheckParamModule();
    bool CheckModuleForMem();
    bool CheckParamProcess();
    bool CheckProcessForAlarm();
    int  CheckParamClass();
    bool CheckParamName();

    void DisplayLogInfo(char *pFile,char *pPath);
    void DisplayInfoByModuleDB();
    void DisplayInfoByProcessDB();
    void DisplayCheckPointDB();
    void DisplayStaffidDB();
    void DisplayInfoByTableNameDB();
    void DisplayInfoBySqlTypeDB();

    bool DisplayDataInfo(int idisNum,bool bspecify=false);
    bool DisplayForInterface(int LogFlag);

    int  DealForModule();
    int  DealForProcess();
    int  DealForParam();
    int  DealForLevel();
    int  DealForTableName();
    int  DealForSqlType();
    int  DealForClass();
    int  DealForCheckPoint();
	int  DealForStaffId();
	
    int  m_iMode;
    int  m_iDisMode;
    int  m_iGroupId;
    int  m_iLine;
    int  m_iModuleId;
    bool m_bPigeonhole;
    bool m_bQryFile;
    char m_sPath[500];
    char m_sFileName[500];
    char m_sBatch[100];
    char m_sParam[100];
    
    ThreeLogGroupMgr * m_poLogGroup;
    CheckPointMgr * m_poCheckPoint; //CheckPoint������
    CommandCom * m_poCmdCom;

public:
    static vector<string> m_vConverInfo;
    static vector<string> m_vAlarmInfo;
    static vector<string> m_vParamInfo;
    static vector<string> m_vSysInfo;
    static vector<string> m_vAttemperInfo;
    static vector<string> m_vAccessInfo;
    static vector<string> m_vDisplayInfo;
    static vector<string> m_vDmlInfo;

};

class LogGroupAdmin
{
public:
    LogGroupAdmin(){}
    ~LogGroupAdmin(){}

    void ParamMain(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc);
};

#endif
