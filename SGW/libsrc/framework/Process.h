// Copyright (c) 2011
// All rights reserved.

#ifndef PROCESS_H_HEADER_INCLUDED_BDCA7789
#define PROCESS_H_HEADER_INCLUDED_BDCA7789
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
#include <vector>


using namespace std;

#include "Date.h"

#include "EventReceiver.h"
#include "EventSender.h"
#include "OcpMsgParser.h"
#include "WithinMessage.h"
#include "InnerStruct.h"
#include "MBC.h"

#include "TSQLIteQuery.h"
#include "SgwDefine.h"
#include "CommandCom.h"
#include "CommandCom.h"
#include "SGWSHMParamApi.h"
#include "ThreeLogGroup.h"
#include "mntapi.h"
#include "WorkFlow.h" 
#include "SHMSGWInfoBase.h"
#include "SelfManage.h"
#include "SgwStatMem.h"

void AbortProcess(int isignal);

#define DEFINE_MAIN(x) Process *GetProcess () { return (new x()); }

class InnerStruct;

//## ���̵Ļ��࣬��̨�Ʒ�ʵʱ��������Ľ���ֱ�ӴӴ���̳�
class Process
{
public:
	//##ModelId=4313CB2501D9
	//##Documentation
	//## ���麯��������ʵ�ִ˷���
	virtual int run() {return 0;};

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
	Process();

	//##ModelId=4313CB2501E4
	//##Documentation
	//## �ͷű�����������ڴ�
	virtual ~Process();

	static void SetProcessSignal();

	static void ClearProcessSignal();

	//## ���̱�ʶ
	static int m_iProcID;

	//## Ӧ�ó����ʶ
	static int m_iAppID;

	//## ��������ʶ
	static int m_iServeID;

	//���̺�ID
	static int m_iFlowID;    


	static	char m_sAPP_HOME[200];

	static char m_sDictFile[200];


	//## ���ջ���
	static string m_sSendBuf;
	static string m_sReceBuf;

	static int m_iIfNeedBufParse;

	//##ModelId=4313CB2501D8
	static int m_iSysPID;

	//�Ƿ��յ���Ϣ���е���Ϣ
	static bool isReceive;

	//��Ϣ����BUF����
	static int m_iMqBufType;

	//·�ɱ�ʶ
	static int m_iRouteRule;

	//������
	static char m_sHOST_NAME[MAX_HOST_NAME_SIZE+1];	

	static OcpMsgParser *m_pOcpMsgParser;

	static ProtoToBusiData m_tProtoData;

	static AvpPublicMember m_CAvpPublicMember;
	static CCRPublicMember m_CCCRPublicMember;
	static CCAPublicMember m_CCCAPublicMember;

	static CapabilitiesExchangeRequest m_CCapExchReq;
	static CapabilitiesExchangeAnswer  m_CCapExchAns;

	static ActiveCCR m_CActiveCCR;
	static SLAUpdateCCR m_CSLAUpdateCCR;
	static CongestionCCR m_CCongestionCCR;
	static SASUpdateCCR m_CSASUpdateCCR;
	static SGWSPACCR m_CSGWSPACCR;
	static SGWSPACCA m_CSGWSPACCA;
	static LoadBalanceCCR m_CLoadBalanceCCR;
	static InnerStruct	*m_poInnerStruct;
	static ThreeLogGroup *m_pLogObj;
	static CommandCom *m_poCmdCom;
	static SelfManage *m_pSelfManage;
	StatAllInfoMgr m_StatAllInfoMgr;

	static long m_lRecvTime;
	static long m_lSendTime;
protected:

	EventReceiver *m_poReceiver;


	EventSender *m_poSender;

private:


	void loadConfig();

	int  GetProcessID();
public:
	static bool GetSysParamInfo();
};

extern int g_argc;
extern char ** g_argv;
extern Process *g_pProc;
extern void AbortProcess(int isignal);
extern void StopProcess(int isignal);
extern void NormalExitProcess();


#endif /* PROCESS_H_HEADER_INCLUDED_BDCA7786 */

