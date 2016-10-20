#ifndef	_INDEPEND_PROCESS_
#define	_INDEPEND_PROCESS_

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>

#include "StdEvent.h"
#include "CommonMacro.h"
#include "Environment.h"
#include "Date.h"
#include "mntapi.h"
#include "Variant.h"

//#include "WorkFlow.h" //������ģ��ͷ�ļ�

//#define PROC_RELOAD_RDY    1
//#define PROC_RELOAD_RUN    2
//#define PROC_RELOAD_OK     4
//#define PROC_RELOAD_FAIL   8
//#define PROC_RELOAD_VIRTUAL_PARAM   16
//#define PROC_RELOAD_HEAD_REGION     32  
//#define PROC_RELOAD_MOBILE_REGION   64  
//#define PROC_RELOAD_ZONE            128 
//#define PROC_RELOAD_ORGEX           256 
//#define PROC_RELOAD_TONE_EVENT_RULE 512
//
//
////���̱�ǿ���ʹ�õ�5��long��ֵ
//const int MNT_SAVE_L_INCEPT_BLOCK = 1;
//const int MNT_SAVE_L_INCEPT_REC = 2;
//const int MNT_SAVE_L_3 = 3;
//const int MNT_SAVE_L_4 = 4;
//const int MNT_SAVE_L_RELOAD=5;  ////���ز����ź�
void AbortProcessEx(int isignal);
void StopProcessEx(int isignal);



#define DEFINE_MAIN_INDEPEND(x) InDependProcess *GetProcess () { return (new x()); }

//##ModelId=41E1DCF40120
//##Documentation
//## ���̵Ļ��࣬��̨�Ʒ�ʵʱ��������Ľ���ֱ�ӴӴ���̳�
class InDependProcess
{
public:
	InDependProcess(void);
	virtual ~InDependProcess(void);
 public:
	 bool Init(char* sProcessName);
    //##ModelId=41ECE8C800B4
    //##Documentation
    //## ���麯��������ʵ�ִ˷���
    virtual int run() = 0;

    //##ModelId=41E1DD3D03E2
    //##Documentation
    //## ��־�ӿ�
    int log(int log_level, char *format,  ...);
    //##ModelId=41E1DD440067
    //##Documentation
    //## �澯�ӿ�
    int warn();
    //##ModelId=42252E770024
    static int m_iAppID;
    //##ModelId=4270B32E0304
    static char m_sTIBS_HOME[80];
    //�˽��̼�����ַ
    static char sLockPath[100]; 
  protected:
	int run_onlyone(const char *filename);
    //##ModelId=42252E8300AD
    char m_sStartTime[16];
public:
    //##ModelId=4270B895019A
    static int m_iSysPID;

  };


#endif
