#ifndef _APP_LOG_GROUP_H_
#define _APP_LOG_GROUP_H_

#include "MBC.h"
#include "LoggDis.h"
#include "ThreeLogGroupDefine.h"
#include "ThreeLogGroup.h"
#include "ThreeLogGroupMgr.h"
#include "CommandCom.h"
#include "../app_tools/CheckPointMgr.h"

#include "../app_tools/CommandBase.h"

class LoggMgr:public CommandBase 
{
 public:

    LoggMgr();
    ~LoggMgr();

    int run();

 private:

    bool init();
    bool deal();
    bool prepare();

    bool AnalyzesParam(char *sParam,char *sReturn);
    bool CheckParamName();
    bool DisplayInfo(int iMode);
    bool DisPlayInfoForWeb(vector < string > & v_list);
    
    void StringCopy(vector<string> &vDest, vector<string> &vSrc);
    
    void printUsage();

    static bool GetField(int iSeq,char *sBuf,char *sParam);
    static bool GreaterMark(const string & s1, const string & s2);
    
    bool CheckProcessId(int iProcessID);
    bool CheckAppId(int iAppID);
	int GetAppIDByName(char *sAppName);

    int  m_iMode;
    int  m_iGroupId;
    
    char m_sPath[500];
    char m_sParam[100];
    char m_sFileName[500];
    char m_sParamFile[500];
    
    bool m_bPigeonhole;                     //���ù鵵
    bool m_bParamErr;
    
    ThreeLogGroup * m_poLogg;               //��־��ʵ��
    ThreeLogGroupMgr * m_poLoggMgr;         //��־�����
    ReadIni * m_RD;                         //���ļ��ӿ�
   CommandCom *m_pCommandComm;             //���Ĳ����ӿ�
   
    int m_iNum;
    CheckPointMgr *pPoint;
};

#endif // _APP_LOG_GROUP_H_
