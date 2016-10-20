#ifndef _BASE_PROCESS_H_
#define _BASE_PROCESS_H_
#include <string>
using namespace std;
#define DEFINE_MAIN(x) BaseProcess *GetProcess () { return (new x()); }

//�ź���Ӧ�����Ķ���
typedef void(*SignalAction)(int);

class BaseProcess
{
public:
    typedef enum RUN_FLAG{
        ULIMIT=1,
        USER,
        SYSTEM 
    };
public:
	//##Documentation
	//## ���麯��������ʵ�ִ˷���
	virtual int run()=0 ;

	//##ModelId=4313CB2501DB
	//##Documentation
	//## ��־�ӿ�
	int log(int log_level, char *format,  ...);

	//##ModelId=4313CB2501E2
	//##Documentation
	//## �澯�ӿ�
	int warn();

	//##ModelId=4313CB2501E3
	//##Documentation
	//## ��ʼ��
	BaseProcess(RUN_FLAG sFlag=ULIMIT);
    virtual ~BaseProcess(void);


    //�쳣�źŴ�����
    void RegistAbortFunc(SignalAction pActionFunc){m_pAbortFunc=pActionFunc;};
    void RegistStopFunc(SignalAction pActionFunc){m_pStopFunc=pActionFunc;};


    static void AbortProcess(int iSignal);
    static void StopProcess(int iSignal);
    static void NormalExitProcess(void);

    static void SetProcessSignal();
	static void ClearProcessSignal();

protected:
    virtual void GetLogFileName(string& strFileName);
    
    virtual void GetAppPathName(string& strPath);

private:
    int run_onlyone(const char *filename);

public:
    static	char m_sAPP_HOME[200];
    static  char m_sAPP_NAME[64];
	static int m_iSysPID;

private:
    static SignalAction m_pAbortFunc ;
    static SignalAction m_pStopFunc;

    
};

extern int g_argc;
extern char ** g_argv;
extern BaseProcess *g_pProc;

#endif
