#ifndef	_GLOBAL_TOOLS_H_
#define	_GLOBAL_TOOLS_H_
#include <iostream>
#include "Date.h"
#include <string>
#define	_CONVERT_UPPER 1
#define _CONVERT_LOWER 2

using namespace std;
class GlobalTools
{
public:
    static bool CheckDateString(const char* sDate);
    static char* upperLowerConvert( char* srcStr , const int flag );
    
    //������һ�ѵļ�鹤��
    //�������Ƿ�ֹͣ
    static bool CheckProcessIsStop(pid_t pid);
    //ͨ���ű��������Ƿ����
    static bool CheckProcessIsStop(const char* pAppName);
    static int CheckProcessNum(const char* sAppName);

    //ͨ���ű�ɱ������
    static bool KillAppByShell(const char* pAppName);
    static int	StartApp(const char*pAppName,char* argv[32],const char* pDir=0);
    static int	StartShell(const char*pAppName,char* argv[32],const char* pDir=0);

};

//��Ʒ����һЩ����

namespace StandardDevelopMent
{
    class  StandardTools
    {
    public:
        static void GetStdConfigFile(string& strFileName);
        static void GetConfigPath(string& strFileName);
    };
};




#endif

