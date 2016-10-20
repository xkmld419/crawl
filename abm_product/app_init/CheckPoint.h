#ifndef CHECKPOINT_H_HEADER_INCLUDED_BDD
#define CHECKPOINT_H_HEADER_INCLUDED_BDD

#include "Log.h"
#include <vector>


#include "SysParamInfo.h"
#include "SysParamDefine.h"
#include "CheckPointMgr.h"
//#include "CommandCom.h"

#define MAX_WAIT_LONG_TIME  1700   // �ȴ�������Ϣ��ʱ��,��λ��0.1��
#define MAX_WAIT_T  700

struct FileInfo
{
	char sline[192];
	char date[16];
	char batchid[16];
	int  pid;
	char cline;
};

class CheckPoint
{
    public:
            CheckPoint();
            ~CheckPoint();
            
			int run();
			bool touchInfoDown();
    public:          
            void getBatch();
			void chkrec();//�����Ϣ���ݼ�¼���ļ� 
	private:
			CheckPointMgr* m_pCheckPoint;
	private:
			bool Link;
			bool getData();
            char sBatch[512];
			char sWaitTime[16];
			int iWaitTime;
};

#endif
