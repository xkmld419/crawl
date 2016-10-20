#ifndef __DCC_PACK_H
#define ___DCC_PACK_H


#include "Process.h"
#include "Log.h"
#include "interrupt.h"
#include "StdEvent.h"
#include "CSemaphore.h"
#include "SHMData.h"

#include "OcpMsgParser.h"
#include "DccLog.h"

/*������Ϣ��m_sRecvBuffer
����StdEvent���ֶα�ʶ��Ϣ���ͣ������m_sSendBuffer+sizeof(StdEvent)��
����m_sSendBuffer*/
class DccPack : public Process
{
public:
	DccPack();
	~DccPack();

	int run();
private:
	int init();

	char *m_pRecvBuffer;   // ���ջ��� sizeof(StdEvent) * (m_iMsgEventNum)
	char *m_pSendBuffer;   // ���ͻ��� sizeof(StdEvent) * (m_iMsgEventNum)
	int  m_iMsgEventNum;   // ������Ϣ��ŵ�����¼���+1����WF_MQ_ATTR����MSG_EVENT_NUM�����ֵ

	OcpMsgParser *m_pOcpMsgParser;  
};

#endif

