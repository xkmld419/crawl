/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef LOG_H_HEADER_INCLUDED_BD8F4FE0
#define LOG_H_HEADER_INCLUDED_BD8F4FE0
#ifndef _LOGIN_F_H_
#ifndef _MAINMENU_H_
#ifndef _PROCESS_F_H_
#ifndef _TICKET_F_H_

#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "Date.h"
#include "Exception.h"

#include "CommonMacro.h"

//using namespace std;

class LogFileData;
class LogInfoData;
class ThreeLogGoupData;
class ThreeLogGroupBase;
class ThreeLogGroupMgr; 
#define ERRCCNT 3
#define MONITOR 397
//##ModelId=4270E21703DE

#define ALARM_LOG_OFF
#define BALARMMAXSIZE 					52428800

class Log
{
  public:
    //##ModelId=4270E91103E7
    Log();

    //##ModelId=4270E9120027
    virtual ~Log();

    //##ModelId=4270E93D03CC
    static void setFileName(char *name);

	static void setAlarmName(char *name);

	static void setParamName(char *name);

	static void setLogHome(char *pPath);

    //##ModelId=4270E983020A
	static void logEx(int log_level, char const *format, ...);
	
    static void log(int log_level, char const *format, ...);

	static void pointlog(int log_level,char const *format, ...);

	static void alarmlog(int log_level,int ErrId,int err,int line,char const *file,char const *function,char const *format, ...);

	static void alarmlog28(int log_level,int Class_id,int ErrId,int err,int line,char const *file,char const *function,char const *format, ...);

	static void paramlog(char const *format, ...);

	static void LoggLog(char const *format, ...);

	static void bottomAlarmLog(char *pData);

	static bool CommitLogFile();

	static void init(int iModleId);
	static void setPrintFlag(bool flag = false);
  private:
	
	static bool CutLogFile();

	static bool CheckFile();

	static bool DeleteOldLog(char *pFile);

	static bool GetInitDirFile(char *pPath,bool bFromPro);

	static bool ErrFileWrite(char *pData,char *pfileName,int iSeq);
	static bool ErrFileWriteEx(char *pData,char *pfileName,int iSeq);
	static bool AlarmLogDb(int log_level,char* sErrid,int err,int line,char const *file,char const *function,char const *sForm);

  public:

	static int m_iInfoLevel;				//ҵ����Ϣ��ȼ�

	static int m_iAlarmLevel;				//�澯��Ϣ��ȼ�

	static int m_iAlarmDbLevel;				//���ȼ�

	static int m_interval;					//��־��Ϣ��������

	static int m_iMaxLogFileSize;			//��־�ļ��������Դ ��λ:M

	static int m_iMode;						//д��־�ļ��ķ�ʽ

	static int m_iModleId;				    //ģ���

	static ThreeLogGroupMgr * m_pologg;		//��־��ӿ�

  private:
    //##ModelId=4270E22C021C
    static int m_iLogLevel;

	static int m_iProcessId;

    //##ModelId=4270E89302D7
    static char m_sLogFileName[256];

	static char m_sInfoPointName[256];		

	static char m_sAlarmFileName[256];

	static char m_sParamFileName[256];

	static char m_sLog_HOME[80];

    //##ModelId=4270E89F00D6
    static FILE *m_pLogFile;

	static std::vector<std::string> m_vLogInfo;

	static std::ofstream *m_pofstream;

	static long m_lSeq;

	static bool m_bPrintf;				//�Ƿ��ӡ��������Ϣ������
};
#define SETLOGPRINT Log::setPrintFlag(false)
#define RESETLOGPRINT Log::setPrintFlag(true)

//ҵ����Ϣ��ɱ������ [ע��]���ڲ��ֱ�������֧�ֿɱ�������пɱ����Ϊ�յ����,������Ӧ��������޿ɱ�������"%s" 
//����޿ɱ�μ���"%s":			INFOPOINT(1,"%s","this is a test");
//����пɱ������Ҫ��:		INFOPOINT(1,"this a test a=%d",1);

//��Ϣ��ӿ�
#define INFOPOINT(x,y,args...) {if(Log::m_iInfoLevel!=-1)	Log::pointlog(x,y,args);}
//�澯�ӿ�
#define ALARMLOG(x,y,z,args...) {if(Log::m_iAlarmLevel!=-1)	Log::alarmlog(x,y,errno, __LINE__, __FILE__,__FUNCTION__,z,args);}
//���2.8�ĸ澯�ӿ�
#define ALARMLOG28(x,c,y,z,args...) {if(Log::m_iAlarmLevel!=-1)	Log::alarmlog28(x,c,y,errno, __LINE__, __FILE__,__FUNCTION__,z,args);}
//��־��ӿ�

#define LOGG(x,y){ }
                             /*
				  if(!Log::m_pologg)\
					Log::m_pologg = new ThreeLogGroupMgr();\
				  Log::m_pologg->LogGroupWriteFile(x,y);\

				}*/

#endif
#endif
#endif
#endif
#endif /* LOG_H_HEADER_INCLUDED_BD8F4FE0 */
