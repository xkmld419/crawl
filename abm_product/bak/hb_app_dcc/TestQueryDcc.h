#ifndef TEST_QUERY_DCC_H
#define TEST_QUERY_DCC_H

#include <string.h>
#include <fstream>
#include <string>
using namespace std;

#include "Process.h"
#include "Log.h"
#include "StdEvent.h"
#include "Date.h"

#include "TAvpParser.h"
#include "dcc_ParamsMgr.h"
#include "dcc_FileMgr.h"

class TestQueryDcc : public Process
{
public:
    int run();

private:
    int prepare();
    int init();

    int readAvps();
    int createRequest();

private:
    TAVPObject m_recvObj;           // TAVPObject ��һ��map<string, string>��ǰ�����һ��TAvpHead
    TAVPObject m_sendObj;
    char *m_pRecvBuffer;			// ���ջ����� sizeof(StdEvent) * (m_iMsgEventNum)
    char *m_pSendBuffer;			// ���ͻ����� sizeof(StdEvent) * (m_iMsgEventNum)
    int  m_iMsgEventNum;            // ������Ϣ��ŵ�����¼���+1����WF_MQ_ATTR����MSG_EVENT_NUM�����ֵ
    char m_sOriginHost[256];                         // ������豸��ʶ
    char m_sOriginRealm[256];
    char m_sDestinationRealm[256];
    char m_sAuthApplicationId[8]; 
	dccParamsMgr *m_pDccParamMgr;    

    char m_sFileName[256];
};

#endif

