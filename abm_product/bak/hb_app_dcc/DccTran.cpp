// ver 2.0
#include "DccTran.h"
#include <iconv.h>

int g_iFlowId;
int g_iProcId;
int* g_pDWANum = NULL;       // �����ڴ棬����м���DWAδ����
int* g_pCCANum = NULL;       // �����ڴ棬����м���CCAδ����
CSemaphore *g_pDWALock = NULL;
CSemaphore *g_pCCALock = NULL;
Socket g_socket;             // ����������ӵ�TCP����
char* g_pRecvBuffer = NULL;	 // socket���ջ���   sizeof(StdEvent) * (m_iMsgEventNum)
char* g_pSendBuffer = NULL;	 // socket���ͻ���   sizeof(StdEvent) * (m_iMsgEventNum)
char* g_pDWRBuffer = NULL;	 // DWR���ͻ���   sizeof(StdEvent) * (m_iMsgEventNum)
int  g_iMsgEventNum = 10;    // ������Ϣ��ŵ�����¼���+1����WF_MQ_ATTR����MSG_EVENT_NUM�����ֵ

int  g_iPort = 3868;		 // �������˿�
char g_sIpAddr[31] = {0};	 // ������IP��ַ
char g_sLocalIp[31] = {0};   // ����IP

char g_sOriginHost[256] = {0}; // ������豸��ʶ
//char g_sNeHost[256] = {0};     // ��Ԫ��ʶ
int  g_iTaskId;                // ��������ID

// createCER�Ĳ���   ��ʽ��hh|ee|Vendor-Id|Product-Name
// createDWR�Ĳ���   ��ʽ��hh|ee
// createDPR�Ĳ���   ��ʽ��hh|ee
char g_sMsgArgv[512] = {0};                
OcpMsgParser* g_pOcpMsgParser = NULL;  
dccParamsMgr* g_pDccParamMgr = NULL;

#define SHM_DWA_NUM    (IpcKeyMgr::getIpcKey(g_iFlowId, "SHM_DWA_NUM"))
#define SHM_CCA_NUM    (IpcKeyMgr::getIpcKey(g_iFlowId, "SHM_CCA_NUM"))
#define LOCK_DWA_NUM    (IpcKeyMgr::getIpcKey(g_iFlowId, "LOCK_DWA_NUM"))
#define LOCK_CCA_NUM    (IpcKeyMgr::getIpcKey(g_iFlowId, "LOCK_CCA_NUM"))

void updateNeState(int iBsnState, int iNeState)
{
	char sql[1024];
	DEFINE_QUERY (qry);

    if (iNeState == ONLINE_NORMAL)
    {
        memset (sql, 0, sizeof(sql));
        sprintf(sql, "INSERT INTO STAT_NE_DESC (ORIGIN_HOST, TASK_ID, BSN_STATE, NE_STATE, SYSUPTIME, STATE_TIME) "
                    "VALUES ('%s', '%d', %d, %d, SYSDATE, SYSDATE)", g_sOriginHost, g_iTaskId, iBsnState, iNeState);
    }
    else
    {
        char sSysUpTime[15] = {0};
        memset (sql, 0, sizeof(sql));
        sprintf(sql, "SELECT TO_CHAR(SYSUPTIME, 'YYYYMMDDHH24MISS') FROM STAT_NE_DESC WHERE BSN_STATE = '0' " 
                    "AND STATE_TIME IN (SELECT MAX(STATE_TIME) FROM STAT_NE_DESC)");
        qry.setSQL(sql);
        qry.open();
        if (qry.next())
        {
            strcpy(sSysUpTime, qry.field(0).asString());
        }
        qry.commit();
        qry.close();

        memset (sql, 0, sizeof(sql));
        if (strlen(sSysUpTime) == 0)
        {
            sprintf(sql, "INSERT INTO STAT_NE_DESC (ORIGIN_HOST, TASK_ID, BSN_STATE, NE_STATE, SYSUPTIME, STATE_TIME) "
                        "VALUES ('%s', '%d', %d, %d, NULL, SYSDATE)", g_sOriginHost, g_iTaskId, iBsnState, iNeState);
        }
        else
        {
            sprintf(sql, "INSERT INTO STAT_NE_DESC (ORIGIN_HOST, TASK_ID, BSN_STATE, NE_STATE, SYSUPTIME, STATE_TIME) "
                        "VALUES ('%s', '%d', %d, %d, to_date('%s', 'YYYYMMDDHH24MISS'), SYSDATE)", 
                        g_sOriginHost, g_iTaskId, iBsnState, iNeState, sSysUpTime);
        }
    }
    qry.setSQL(sql);
    qry.execute();
    qry.commit();
    qry.close();
}

DccTran::DccTran()
{ }

DccTran::~DccTran()
{ }

// ����SR���������ӽ��̺������
int DccTran::run()
{ 
	// ����monitor
	ConnectTerminalMonitor();
	int nConnFailedTimes = 0;

#ifndef DEF_LINUX
	sigset (SIGUSR2, DccTran::sigQuit);
#else
	signal (SIGUSR2, DccTran::sigQuit);
#endif
	int pid, pid1, pid2;
	pid = fork();
	switch(pid)
	{
	case -1:
		Log::log(0, "fork �ӽ���ʧ��");
		return -1;
	case 0:
        DenyInterrupt();
		nConnFailedTimes = 0;
		while (g_socket.connect(g_sIpAddr, g_iPort, g_sLocalIp) < 0)
		{
			if (getppid() == 1)    // ���������˳����ӽ���Ҳ�˳�
			{
				Log::log(0, "���������˳����ӽ����˳�");
				return 0;
			}
			nConnFailedTimes++;
			if (nConnFailedTimes%6 == 0)  // ÿ����ʧ��6�Σ�д��־һ��
			{
				Log::log(0, "Socket ����ʧ��");
			}
			sleep(10);
			continue;
		}
		Log::log(0, "Socket���ӳɹ�");

		pid1 = getpid(); 
		pid2 = fork();
		switch(pid2)
		{
		case -1:
			Log::log(0, "fork �����ʧ��");
			return -1;

		case 0:     // �����
            DenyInterrupt();
			sleep(2);
			DccTran::receiveDccMsg();   // ��SR������Ϣ������Ϣ����
			if (getppid() != 1)
			{
				Log::log(0, "����̽���KILL�ź�");
				kill(pid1,SIGUSR2); // ɱ�ӽ���
			}
			g_socket.closeSocket();
            g_pDWALock->P();
			if (*g_pDWANum <= 3)
                *g_pDWANum = 999;   // ֪ͨ���γ����ٷ�����Ϣ
            g_pDWALock->V();
			break;

		default:     // �ӽ���
            if (DccTran::sendDccMsg() < 0) // ����Ϣ���л�ȡ��Ϣ���͵�SR
            {
                // ��¼��Offline_���ϡ���stat_ne_desc
                updateNeState(OFFLINE, OFFLINE_FAULT);
            }
			g_socket.closeSocket();
            g_pDWALock->P();
			if (*g_pDWANum <= 3)
				*g_pDWANum = 999;   // ֪ͨ���γ����ٷ�����Ϣ
            g_pDWALock->V();
			break;
		}
		Log::log(0, "����[%d]�˳�", getpid());
		return 0;
	default:
		waitpid(pid, NULL, 0);
		sleep(10);
		Environment::getDBConn(true);
		g_socket.closeSocket();
		break;
	}

	return 0;
}

int DccTran::receiveDccMsg()
{
	Log::log(0, "receiveDccMsg start ......");

	EventSender *poSender = new EventSender (g_iProcId);

	int   recv_len, msg_len, ret;
	int   msgType, requestType;
	char *data = g_pRecvBuffer+sizeof(StdEvent);        // ǰ��Ԥ��һ��StdEvent��С
    char sTemp[32] = {0};

	g_pDWANum = (int *)DccTran::getShmAddr(SHM_DWA_NUM, sizeof(int));
    g_pDWALock = new CSemaphore();
    sprintf(sTemp, "%d", LOCK_DWA_NUM);
    g_pDWALock->getSem (sTemp, 1, 1);    

	g_pCCANum = (int *)getShmAddr(SHM_CCA_NUM, sizeof(int));
    g_pCCALock = new CSemaphore();
    sprintf(sTemp, "%d", LOCK_CCA_NUM);
    g_pCCALock->getSem (sTemp, 1, 1);    

	for(;;)
	{
		if (getppid() == 1)    // ���ӽ����˳��������Ҳ�˳�
		{
			Log::log(0, "�ӽ������˳���������˳�");
			return 0;
		}
		
		msg_len = 20;
		recv_len = 0;
		memset(g_pRecvBuffer, 0, sizeof(StdEvent)*(g_iMsgEventNum));
		// ��ȡDccͷ��20���ֽ�
		for( ; recv_len < msg_len  ;  )
		{
			ret = g_socket.selectRead((unsigned char *)(data+recv_len), msg_len - recv_len, 3);
			if( ret <= 0 )
			{
				break;
			}
			recv_len += ret;
		}
		if (recv_len < 20)
		{
			if (recv_len > 0)
				Log::log(0, "dcc head error");
			continue;
		}
		unsigned char buf[4];
		buf[1]=data[1];
		buf[2]=data[2];
		buf[3]=data[3];

		msg_len = buf[1]*65536+buf[2]*256+buf[3];//��Ϊ������תΪ�ֽ����޸����һЩ
		if (msg_len >= sizeof(StdEvent)*(g_iMsgEventNum-1))
		{
			Log::log(0, "DCC message is too length!");
		}

		// ��ȡDCC��Ϣ��
		for( ; recv_len < msg_len  ;  )
		{
			ret = g_socket.selectRead((unsigned char *)(data+recv_len), msg_len - recv_len, 15);
			if( ret <= 0 )
			{
				break;
			}
			recv_len += ret;
		}

		if( recv_len < msg_len - 20 )
		{
			Log::log(0, "recv length error!");
			continue;
		}

		msgType = data[5]*65536+data[6]*256+data[7];
		requestType = data[4] >> 7;
		switch (msgType)
		{
		case 257:     // CE
			if (!requestType)           // CER��Ŀǰ�����յ�
				Log::log(0, "---------------�յ�CEA��");
			break;
		case 272:     // CC
			if (requestType)        
			{
				Log::log(0, "---------------�յ�CCR��");
				AddTicketStat(TICKET_NORMAL);
                g_pCCALock->P();
                (*g_pCCANum)++;
                g_pCCALock->V();
			}
			else
			{
				Log::log(0, "---------------�յ�CCA��");
				AddTicketStat(TICKET_NORMAL);
			}
			break;
		case 280:     // DW
			if (requestType)        
				Log::log(0, "---------------�յ�DWR��");
			else
			{
				Log::log(0, "---------------�յ�DWA��");
                g_pDWALock->P();
				(*g_pDWANum)--;
                g_pDWALock->V();
				continue;
			}
			break;
		case 282:     // DP
			if (requestType)        
				Log::log(0, "---------------�յ�DPR��");
			else
			{
				Log::log(0, "---------------�յ�DPA��");
				continue;
			}
			break;
		}
		
		// ���ͽ����������ݵ���Ϣ����
		poSender->send(g_pRecvBuffer);

	}
	delete poSender;   poSender = NULL;
	return 0;
}

// �����˳�����0���������˳�����-1
int DccTran::sendDccMsg()
{
	Log::log(0, "sendDccMsg start ......");

	EventSender *poSender = new EventSender (g_iProcId);
	EventReceiver *poReceiver = new EventReceiver (g_iProcId);

    char sTemp[32] = {0};
	g_pDWANum = (int *)DccTran::getShmAddr(SHM_DWA_NUM, sizeof(int));
    g_pDWALock = new CSemaphore();
    sprintf(sTemp, "%d", LOCK_DWA_NUM);
    g_pDWALock->getSem (sTemp, 1, 1);    
    g_pDWALock->P();
	*g_pDWANum = 0;
    g_pDWALock->V();

	g_pCCANum = (int *)getShmAddr(SHM_CCA_NUM, sizeof(int));
    g_pCCALock = new CSemaphore();
    sprintf(sTemp, "%d", LOCK_CCA_NUM);
    g_pCCALock->getSem (sTemp, 1, 1);    
    g_pCCALock->P();
	*g_pCCANum = 0;
    g_pCCALock->V();
    

	if (sendCER() < 0)         // ����CER
	{
		Log::log(0, "����CERʧ�ܣ������ӽ����˳�");
		return -1;
	}
	Log::log(0, "����CER��SR----------------------");

	int iRet =0;
	int iSendNum = 0;
	int iActiveNum = 0;       // ����CCR���ʹ���
	Date DWRDate;
	Date tmpDate; 

	for (;;)
	{
		if (getppid() == 1)    // ���������˳����ӽ���Ҳ�˳�
		{
			Log::log(0, "���������˳����ӽ����˳�");
			return 0;
		}
        //g_pDWALock->P();
		if (*g_pDWANum > 3)
		{
            //g_pDWALock->V();
			Log::log(0, "���γ�ʱδ�յ�DWA");
			while (poReceiver->receive(g_pSendBuffer, false))  // ��ʱ��Ϣ���������ѳ�Ϊ��������
			{
				Log::log(0, "������Ϣ");
			}

			Log::log(0, "�ӽ����˳�");
			return -1;
		}
        //g_pDWALock->V();
		
		// ����Ϣ���н������ݵ�m_pSendBuffer��
		memset(g_pSendBuffer, 0, sizeof(StdEvent)*(g_iMsgEventNum));
		if (poReceiver->receive(g_pSendBuffer, false) <= 0)
		{
			tmpDate.getCurDate();
			if (tmpDate.diffSec(DWRDate) >= DWRINTERVAL)
			{
				Log::log(0, "����DWR��----------------------");
                g_pDWALock->P();
				(*g_pDWANum)++;
                g_pDWALock->V();
				if (sendDWR() < 0)
				{
					Log::log(0, "����DWRʧ�ܣ������ӽ����˳�");
					return -1;
				}
				DWRDate.getCurDate();
			}
			usleep(5000);
			continue;
		}

		// �ж��Ƿ�ΪdccUnPack�����ı�ʶCER����ʧ�ܵ���Ϣ
		StdEvent *pEvent = (StdEvent *)g_pSendBuffer;
		if (strcmp(pEvent->m_sBillingNBR, "CERFAILED") == 0)
		{
			
			Log::log(0, "CER����ʧ�ܣ������ط�----------------------");
			sleep(10);
			iSendNum++;
			if (iSendNum>5)
			{
				Log::log(0,"CER��������ʧ�ܣ��Ͽ�����");
				return -1;
			}
			if (sendCER() < 0)         // ����CER
			{
				Log::log(0, "����CERʧ�ܣ������ӽ����˳�");
				return -1;
			}

			continue;
		}
		else if (strcmp(pEvent->m_sBillingNBR, "ACTIVECCR") == 0)
		{
			
			Log::log(0, "���ͼ���CCR----------------------");
			AddTicketStat(TICKET_NORMAL);
			iActiveNum++;
			if (iActiveNum>5)
			{
				Log::log(0,"����CCR��������ʧ�ܣ�����DPR�Ͽ�����");
				memset (g_pSendBuffer, 0, sizeof(StdEvent)*(g_iMsgEventNum));
				strcpy(pEvent->m_sBillingNBR, "REBOOT");
				poSender->send(g_pSendBuffer);         // ���ͼ������ӶϿ���Ϣ��dccUnPack��ʹdccUnPack�����ط�����CCR
				sendDPR();
				Log::log(0, "�ӽ����˳�");
				return -1;
			}
		}
		else if (strcmp(pEvent->m_sBillingNBR, "DWA") == 0)
		{
			Log::log(0, "����DWA��----------------------");
		}
		else if (strcmp(pEvent->m_sBillingNBR, "DPA") == 0)
		{
			Log::log(0, "����DPA��----------------------");
		}
		else if (strcmp(pEvent->m_sBillingNBR, "CCR") == 0)
		{
			// ��Ϣ���н��յ�CCR�����Ǹ÷���SR��A����Ӧ�÷���DWRȷ����·�Ƿ�ͨ
            tmpDate.getCurDate();
            if (tmpDate.diffSec(DWRDate) >= DWRINTERVAL)
            {
                Log::log(0, "����DWR��----------------------");
                g_pDWALock->P();
                (*g_pDWANum)++;
                g_pDWALock->V();
                if (sendDWR() < 0)
                {
                    Log::log(0, "����DWRʧ�ܣ������ӽ����˳�");
                    return -1;
                }
                DWRDate.getCurDate();
            }
			
			Log::log(0, "����ҵ��CCR��----------------------");
			AddTicketStat(TICKET_NORMAL);
		}
		else
		{
			Log::log(0, "����ҵ��CCA��----------------------");
            g_pCCALock->P();
            (*g_pCCANum)--;
            g_pCCALock->V();
		}

		char *data = g_pSendBuffer + sizeof(StdEvent);         // ������Ϣǰ��洢��StdEvent
		unsigned char buf[4];
		buf[1]=data[1];
		buf[2]=data[2];
		buf[3]=data[3];
		int  msg_len = buf[1]*65536+buf[2]*256+buf[3];   // ��Ϣ����

		// ������Ϣ�������
		int num = 0;
		for ( ; num < msg_len ; )
		{
			iRet = g_socket.write ( (unsigned char *)(data + num), msg_len - num );

			if( iRet <= 0 )
			{
				Log::log(0, "socket send data error!");
				return -1;
			}
			num += iRet;
		}
        //g_pCCALock->P();
        if (*g_pCCANum > CCANOTSEND)
            // ��¼��Online�����ɡ���stat_ne_desc
            updateNeState(ONLINE, ONLINE_OVERLOAD);
        //g_pCCALock->V();

	}

	delete poSender;     poSender = NULL;
	delete poReceiver;   poReceiver = NULL;
	return 0;
}

int DccTran::sendCER()
{
	// ��g_sMsgArgv��ֵ
	if (g_pDccParamMgr == NULL)
		g_pDccParamMgr = new dccParamsMgr;

	g_pDccParamMgr->getDccHeadValue(NULL);
	char buff[256] = {0};
	memset(g_sMsgArgv, 0, sizeof(g_sMsgArgv));
	sprintf(g_sMsgArgv, "%d|%d", g_pDccParamMgr->m_iHopByHop, g_pDccParamMgr->m_iEndToEnd);

	if (!ParamDefineMgr::getParam("DCC", "VENDOR_ID", buff))
	{
		Log::log(0, "����b_param_define���Ƿ�����VENDOR_ID");
		return -1;
	}
	strcat(g_sMsgArgv, "|");  strcat(g_sMsgArgv, buff);
	memset(buff, 0, sizeof(buff));
	if (!ParamDefineMgr::getParam("DCC", "PRODUCT_NAME", buff))
	{
		Log::log(0, "����b_param_define���Ƿ�����PRODUCT_NAME");
		return -1;
	}
	strcat(g_sMsgArgv, "|"); strcat(g_sMsgArgv, buff);

	// ����CER����g_pSendBuffer+sizeof(StdEvent)��
	memset(g_pSendBuffer, 0, sizeof(StdEvent)*(g_iMsgEventNum));
	g_pOcpMsgParser->createCER(g_sMsgArgv, g_pSendBuffer+sizeof(StdEvent), 
				sizeof(StdEvent)*(g_iMsgEventNum-1));

	char *data = g_pSendBuffer + sizeof(StdEvent);
	unsigned char buf[4];
	buf[1]=data[1];
	buf[2]=data[2];
	buf[3]=data[3];
	int  msg_len = buf[1]*65536+buf[2]*256+buf[3];   // ��Ϣ����
	int  iRet =0;
	// ������Ϣ�������
	int num = 0;
	for ( ; num < msg_len ; )
	{
		iRet = g_socket.write ( (unsigned char *)(data + num), msg_len - num );

		if( iRet <= 0 )
		{
			Log::log(0, "socket send CER error!");
			return -1;
		}
		num += iRet;
	}
	Log::log(0, "---------------------send CER begin----------------------");
	string strLogContext=g_pOcpMsgParser->getAllAvp(true);
	char buffer[61441] = {0};
	getPack(data, buffer, 61441);
	Log::log(0, "\n%s\n%s", strLogContext.c_str(), buffer);
	Log::log(0, "---------------------send CER end------------------------");

	return 0;
}

int DccTran::sendDWR()
{
	// ��g_sMsgArgv��ֵ
	if (g_pDccParamMgr == NULL)
		g_pDccParamMgr = new dccParamsMgr;

	g_pDccParamMgr->getDccHeadValue(NULL);
	char buff[256] = {0};
	memset(g_sMsgArgv, 0, sizeof(g_sMsgArgv));
	sprintf(g_sMsgArgv, "%d|%d", g_pDccParamMgr->m_iHopByHop, g_pDccParamMgr->m_iEndToEnd);

	// ����DWR����g_pDWRBuffer+sizeof(StdEvent)��
	memset(g_pDWRBuffer, 0, sizeof(StdEvent)*(g_iMsgEventNum));
	g_pOcpMsgParser->createDWR(g_sMsgArgv, g_pDWRBuffer+sizeof(StdEvent), 
				 sizeof(StdEvent)*(g_iMsgEventNum-1));

	char *data =g_pDWRBuffer + sizeof(StdEvent);
	unsigned char buf[4];
	buf[1]=data[1];
	buf[2]=data[2];
	buf[3]=data[3];
	int  msg_len = buf[1]*65536+buf[2]*256+buf[3];   // ��Ϣ����
	int  iRet =0;
	// ������Ϣ�������
	int num = 0;
	for ( ; num < msg_len ; )
	{
		iRet = g_socket.write ( (unsigned char *)(data + num), msg_len - num );

		if( iRet <= 0 )
		{
			Log::log(0, "socket send DWR error!");
			return -1;
		}
		num += iRet;
	}

	return 0;
}

int DccTran::sendDPR()
{
	// ��g_sMsgArgv��ֵ
	if (g_pDccParamMgr == NULL)
		g_pDccParamMgr = new dccParamsMgr;

	g_pDccParamMgr->getDccHeadValue(NULL);
	char buff[256] = {0};
	memset(g_sMsgArgv, 0, sizeof(g_sMsgArgv));
	sprintf(g_sMsgArgv, "%d|%d", g_pDccParamMgr->m_iHopByHop, g_pDccParamMgr->m_iEndToEnd);

	// ����DPR����g_pSendBuffer+sizeof(StdEvent)��
	memset(g_pSendBuffer, 0, sizeof(StdEvent)*(g_iMsgEventNum));
	g_pOcpMsgParser->createDPR(g_sMsgArgv, g_pSendBuffer+sizeof(StdEvent), 
				 sizeof(StdEvent)*(g_iMsgEventNum-1));

	char *data = g_pSendBuffer + sizeof(StdEvent);
	unsigned char buf[4];
	buf[1]=data[1];
	buf[2]=data[2];
	buf[3]=data[3];
	int  msg_len = buf[1]*65536+buf[2]*256+buf[3];   // ��Ϣ����
	int  iRet =0;
	// ������Ϣ�������
	int num = 0;
	for ( ; num < msg_len ; )
	{
		iRet = g_socket.write ( (unsigned char *)(data + num), msg_len - num );

		if( iRet <= 0 )
		{
			Log::log(0, "socket send DPR error!");
			return -1;
		}
		num += iRet;
	}
	Log::log(0, "---------------------send DPR begin----------------------");
	string strLogContext=g_pOcpMsgParser->getAllAvp(true);
	char buffer[61441] = {0};
	getPack(data, buffer, 61441);
	Log::log(0, "\n%s\n%s", strLogContext.c_str(), buffer);
	Log::log(0, "---------------------send DPR end------------------------");

	return 0;
}

void DccTran::getPack(char *buf, char *sPack, int iPackLen)
{
	if (iPackLen < 0)
		return;

	unsigned char buf1[4] = {0};
	buf1[1] = buf[1];
	buf1[2] = buf[2];
	buf1[3] = buf[3];

	int uiLen = buf1[1]*65536+buf1[2]*256+buf1[3];//��Ϊ������תΪ�ֽ����޸����һЩ
	char temp[8] = {0};
	for (unsigned int i = 0; i < uiLen; i++)
	{
		memset(temp, 0, 8);
		unsigned char c = (*(buf + i));
		sprintf(temp,"%x%x ", ((c >> 4) & 0xf), (c & 0xf)); 
		strcat(sPack, temp);
	}
}

void DccTran::sigQuit(int signo)
{
	Log::log(0, "PID=[%d] catch quit signal(signo = %d)",getpid(),signo);
	g_socket.closeSocket();
	Log::log(0, "process exit!");
	exit(0);
}

void* DccTran::getShmAddr(key_t shmKey, int shmSize)
{
	int shmid;  
	if((shmid=shmget(shmKey, shmSize, SHM_R|SHM_W|IPC_EXCL|IPC_CREAT))==-1)
	{
		if(errno==EEXIST)
			shmid=shmget(shmKey,0,0);
		else
			return NULL;
	}
	return shmat(shmid, NULL, 0);
}

int DccTran::init()
{
	char sql[1024];
	memset (sql, 0, sizeof(sql));
	sprintf(sql, "SELECT MAX(A.MSG_EVENT_NUM) FROM WF_MQ_ATTR A");

	DEFINE_QUERY (qry);
	qry.setSQL(sql);
	qry.open();
	qry.next();
	g_iMsgEventNum = qry.field(0).asInteger();
	qry.commit();
	qry.close();
	
	g_pRecvBuffer = new char[sizeof(StdEvent) * (g_iMsgEventNum)];			// socket���ջ���
	g_pSendBuffer = new char[sizeof(StdEvent) * (g_iMsgEventNum)];			// socket���ͻ���
	g_pDWRBuffer = new char[sizeof(StdEvent) * (g_iMsgEventNum)];			// DWR���ͻ���

	g_pOcpMsgParser = new OcpMsgParser("dictionary.xml");


	ParamDefineMgr::getParam("DCC", "ORIGIN_HOST", g_sOriginHost);
	if (strlen(g_sOriginHost) == 0)
	{
		Log::log(0, "����b_param_define���Ƿ�����ORIGIN_HOST");
		return -1;
	}
	char sOriginRealm[81] = {0};
	if (!ParamDefineMgr::getParam("DCC", "ORIGIN_REALM", sOriginRealm))
	{
		Log::log(0, "����b_param_define���Ƿ�����ORIGIN_REALM");
		return -1;
	}
	memset(g_sLocalIp, 0, sizeof(g_sLocalIp));
	if (!ParamDefineMgr::getParam("DCC", "LOCAL_IP", g_sLocalIp))
	{
		Log::log(0, "����b_param_define���Ƿ�����LOCAL_IP");
		return -1;
	}

	g_pOcpMsgParser->initLocalInfo(g_sOriginHost, sOriginRealm, g_sLocalIp);

	g_pDccParamMgr = new dccParamsMgr;
	
	return 0;
}

void DccTran::printManual()
{
	Log::log (0,
				"********\n"
				"dccTran ���÷�����\n"
				"-a IpAddr   ������IP��ַ\n"
				"-p Port     �������˿�\n"
                "-o taskID   �����\n");
}

bool DccTran::prepare(int argc, char **argv)
{
    int iTaskId = -1;

	for (int i=1; i<argc; ++i)
	{
		if (argv[i][0] != '-')
		{
			printManual();
			return false;
		}
		switch (argv[i][1] | 0x20)
		{
		case 'a':
			i++;
			strcpy(g_sIpAddr, argv[i]);
			break;
		case 'p':
			i++;
			g_iPort = atoi(argv[i]);
			break;
        case 'o':
            i++;
            iTaskId = atoi(argv[i]);
            break;
		default:
			printManual();
			return false;
		}
	}

	if (!(strlen(g_sIpAddr)) || !g_iPort)
	{
		char sValue[81] = {0};
		if (ParamDefineMgr::getParam("DCC", "IPADDR", sValue))
			strcpy(g_sIpAddr, sValue);
		if (strlen(g_sIpAddr) == 0)
		{
			Log::log(0, "�������в���ָ��������b_param_define������SR��IP��ַ");
			printManual();
			return false;
		}

		if (ParamDefineMgr::getParam("DCC", "PORT", sValue))
			g_iPort = atoi(sValue);
		if (g_iPort == 0)
		{
			Log::log(0, "�������в���ָ��������b_param_define������SR�Ķ˿�");
			printManual();
			return false;
		}
	}
    
    if (iTaskId == -1)
    {
        Log::log(0, "�������в���ָ��ִ������ID");
        printManual();
        return false;
    }
	char sql[1024];
    DEFINE_QUERY(qry);
	memset (sql, 0, sizeof(sql));
	sprintf(sql, "SELECT dcc_ne_host FROM trans_dcc_task WHERE dcc_task_id = %d", iTaskId);
	qry.setSQL(sql);
	qry.open();
	if (qry.next()) {
        //strcpy(g_sNeHost, qry.field(0).asString());
        g_iTaskId = iTaskId;
    }
    else
    {
        Log::log(0, "����trans_dcc_task���Ƿ�������%d", iTaskId);
        return false;
    }
    qry.commit();
    qry.close();

	Log::log(0, "��Ҫ���ӵ�SR��IP��ַΪ��%s", g_sIpAddr);
	Log::log(0, "��Ҫ���ӵ�SR�Ķ˿�Ϊ��%d", g_iPort);

	return true;
}

int run_onlyone(const char *filename)
{
    int  fd, val;
    char buf[10];
    memset(buf, 0, sizeof(buf));

    if ((fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
        return -1;

    // try and set a write lock on the entire file
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
            exit(0);     // gracefully exit, daemon is already running
        else
            return -1;
    }

    // truncate to zero length, now that we have the lock
    if (ftruncate(fd, 0) < 0)
        return -1;

    // and write our process ID
    sprintf(buf, "%d\n", getpid());
    if (write(fd, buf, strlen(buf)) != (int)strlen(buf))
        return -1;

    // set close-on-exec flag for descriptor
    if ( (val = fcntl(fd, F_GETFD, 0)) < 0)
        return -1;
    val |= FD_CLOEXEC;
    if (fcntl(fd, F_SETFD, val) < 0)
        return -1;

    // leave file open until we terminate: lock will be held
   
    return 0;
}

int main(int argc, char **argv)
{
	int pid;
	int iProcId, iFlowId;

	pid = getpid();
	// ����monitor
	ConnectTerminalMonitor();
	
	// �������ݿ�
	DEFINE_QUERY(qry);
	
	// ������־�ļ���
	char *p = NULL;
	char sTIBS_HOME[81];
	if ((p=getenv ("TIBS_HOME")) == NULL)
		sprintf (sTIBS_HOME, "/home/bill/TIBS_HOME");
	else 
		sprintf (sTIBS_HOME, "%s", p);

	iProcId = GetProcessID();

	char sSqlcode[256] = {0};
	sprintf (sSqlcode, "select billflow_id from wf_process where process_id=%d", iProcId);
	qry.setSQL(sSqlcode);
	qry.open();
	if (qry.next()) {
		iFlowId = qry.field(0).asInteger();
	} else {
		THROW(MBC_Process+3);
	}
	qry.close();

	// LOG_FILE 
	char sLogFile[81] = {0};
	sprintf (sLogFile, "%s/log/process_%d.log", sTIBS_HOME, iProcId);
	Log::setFileName (sLogFile);

	// ���˽�������
	char sLockPath[81] = {0};
	sprintf(sLockPath, "%s/lockpath/%d", sTIBS_HOME,iProcId);
	char sCmd[1000];
	memset(sCmd, 0, sizeof(sCmd));
	sprintf (sCmd,"mkdir -p %s/lockpath",sTIBS_HOME);
	system(sCmd);
	// ����
	if (run_onlyone(sLockPath)<0)
	{
		Log::log (0, "--��������ʧ��--");
		THROW(MBC_Process+3);	
	}

	Log::log(0, "dccTran run ......");

	if (DccTran::prepare(argc, argv) != true)
    {
        DisconnectTerminalMonitor();
		return -1;
    }

	if (DccTran::init() < 0)
    {
        DisconnectTerminalMonitor();
		return -1;
    }

	g_iFlowId = iFlowId;
	g_iProcId = iProcId;


	DenyInterrupt();
	while (!GetInterruptFlag())
	{
		DccTran::run();           

		if (getpid() != pid)
			return 0;
	}

    // ��¼��Offline��������stat_ne_desc
    updateNeState(OFFLINE, OFFLINE_NORMAL);
	// �˳�monitor
	DisconnectTerminalMonitor();

	return 0;
}

