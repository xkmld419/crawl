#ifndef __DCC_UN_PACK_H
#define __DCC_UN_PACK_H 

#include <pthread.h> 
#include <sstream>
#include "Process.h"
#include "Log.h"
#include "interrupt.h"
#include "StdEvent.h"
#include "Date.h"

#include "OcpMsgParser.h"
#include "dcc_ParamsMgr.h"
#include "dcc_FileMgr.h"
#include "DccLog.h"
#include "dcc_ConstDef.h"

class DccUnPack : public Process
{
public:
	DccUnPack();
	~DccUnPack();

	int run();    
private:
	char *m_pRecvBuffer;			// ���ջ����� sizeof(StdEvent) * (m_iMsgEventNum)
	char *m_pSendBuffer;			// ���ͻ����� sizeof(StdEvent) * (m_iMsgEventNum)
	int  m_iMsgEventNum;            // ������Ϣ��ŵ�����¼���+1����WF_MQ_ATTR����MSG_EVENT_NUM�����ֵ

	char (*m_ContextIds)[256];                       // Ҫ�����ҵ��Service-Context-Id��
	int  m_iContextNum;                              // Ҫ�����ҵ��������m_ContextIds������
    char m_sOriginHost[256];                         // ������豸��ʶ
	char m_sNeHost[256];				             // Ҫ�����ҵ���Ӧ��NE_HOST
	char m_sActiveContext[256];                      // ����CCR��Ӧ��Service-Context-Id
	char m_sServiceContextId[256];                   // ����󷵻ص�Service-Context-Id
	int  m_iMsgType;                                 // ����󷵻صİ�����

	int  m_isActiveCCAOK;                            // ����CCA�Ƿ񷵻أ���ʼ�ѷ��أ�1���أ�0δ����
	Date m_dDate;  
	int  m_nTime;                                    // ���ʱʱ��

    int m_iTaskId;
    char* m_shmServiceType;
	//createActiveCCR�Ĳ��� ��ʽ��Session-Id|hh|ee|Destination-Realm|Auth-Application-Id|Service-Context-Id|CC-Request-Type|CC-Request-Number
	char m_ActiveCCRArgv[512];  

	OcpMsgParser *m_pOcpMsgParser;  
	dccParamsMgr *m_pDccParamMgr;
	FileToDccMgr *m_pFileToDccMgr;

private:
    void printManual();
    bool prepare();
	int init();
	// �����������Ϣ
	int procMsg();      

	// ��������CCR
	int createActiveCCR();
void* getShmAddr(key_t , int );
	// ��m_sParseBuffer��ǰ�����StdEvent��m_sSendBuffer
	// packTypeΪCCR��CCA����ֵ��StdEvent�е�billingNbr
	// iTransFileTypeIDΪ����Ϣ��ҵ�����ͣ���TransFileTypeID��Ӧ����ֵ��StdEvent�е�m_iBillingTypeID
	void prefixStdEvent(const char *sPackType = NULL, int iTransFileTypeID = 0);

    // ������״̬���뵽��Ԫ������stat_ne_desc
    void updateNeState(int iBsnState, int iNeState);
	void getShmServiceType(const char *, char *);

	char* m_pcopyBuffer;
};



#endif


