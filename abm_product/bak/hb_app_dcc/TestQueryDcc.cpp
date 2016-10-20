#include "TestQueryDcc.h"

DEFINE_MAIN (TestQueryDcc)

int TestQueryDcc::run()
{
    if (prepare() < 0)
        return -1;
    if (init() < 0)
        return -1;
    if (readAvps() < 0)
        return -1;

    memset(m_pSendBuffer, 0, sizeof(StdEvent)*m_iMsgEventNum);
    string sServiceType = (m_sendObj[string("461")]).substr(0, (m_sendObj[string("461")]).find_first_of("@"));
    strcpy(((StdEvent*)m_pSendBuffer)->m_sCallingNBR, sServiceType.c_str());     // �������븳ҵ������
    strcpy(((StdEvent*)m_pSendBuffer)->m_sBillingNBR, "CCR");            // �ƷѺ��븳CCR

    string strRecvLog;
    char sHead[256] = {0};

    // �����Ϣ����
    while (m_poReceiver->receive(m_pRecvBuffer, false) > 0)
    {
        memset(m_pRecvBuffer, 0, sizeof(StdEvent) * m_iMsgEventNum);
        continue;
    }

    for (int i=0; i<3; i++)
    {
        createRequest();
        m_sendObj.toCString(m_pSendBuffer+sizeof(StdEvent));  // ��m_sendObj��ֵת����m_pSendBuffer+sizeof(StdEvent)��
        m_poSender->send(m_pSendBuffer); 
        Log::log(0, "����%s��ѯCCR�����", sServiceType.c_str());
        
        sleep(3);

        if (m_poReceiver->receive(m_pRecvBuffer, false) <= 0)
        {
            Log::log(0, "��ʱδ�յ�CCA");
            usleep(5000);
            continue;
        }
        m_recvObj.setObjectValue(m_pRecvBuffer+sizeof(StdEvent));       // ��m_recvObj��ֵ
        m_recvObj.avpHead.toCString(sHead);
        strRecvLog = string(sHead);
        TAVPObject::iterator itr = m_recvObj.begin();
        for (; itr!=m_recvObj.end(); itr++)
        {
            strRecvLog.append("\n");
            strRecvLog.append(itr->first);
            strRecvLog.append(":=");
            strRecvLog.append(itr->second);
        }
        Log::log(0, "�յ���CCA����:\n%s", strRecvLog.c_str());
        
        break;
    }

    return 0;
}

int TestQueryDcc::prepare()
{
    if (g_argc < 2)
    {
        Log::log(0, "testQueryDcc ҵ��ǰ׺");
        return -1;
    }

    char *p;
    if ((p = getenv("BILLDIR")) == NULL)
    {
		Log::log(0, "Please set env value BILLDIR. For example  export BILLDIR=/bill/rtbill/BILLDIR");
        return -1;
    }
    if (p[strlen(p)] == '/')
        p[strlen(p)] = '\0';
    
    sprintf(m_sFileName, "%s/data/dcc/queryCCR/%s", p, g_argv[1]);

    return 0;
}

int TestQueryDcc::init()
{
	char sql[1024];
	DEFINE_QUERY (qry);
	sprintf(sql, "SELECT MAX(A.MSG_EVENT_NUM) FROM WF_MQ_ATTR A");
	qry.setSQL(sql);
	qry.open();
	qry.next();
	m_iMsgEventNum = qry.field(0).asInteger();
	qry.commit();
	qry.close();
	m_pRecvBuffer = new char[sizeof(StdEvent) * m_iMsgEventNum];			// ���ջ�����
	m_pSendBuffer = new char[sizeof(StdEvent) * m_iMsgEventNum];			// ���ͻ�����

	if (!ParamDefineMgr::getParam("DCC", "ORIGIN_HOST", m_sOriginHost))
	{
		Log::log(0, "����b_param_define���Ƿ�����ORIGIN_HOST");
		return -1;
	}
	if (!ParamDefineMgr::getParam("DCC", "ORIGIN_REALM", m_sOriginRealm))
	{
		Log::log(0, "����b_param_define���Ƿ�����ORIGIN_REALM");
		return -1;
	}
	if (!ParamDefineMgr::getParam("DCC", "DESTINATION_REALM", m_sDestinationRealm))
	{
		Log::log(0, "����b_param_define���Ƿ�����DESTINATION_REALM");
		return -1;
	}
	if (!ParamDefineMgr::getParam("DCC", "AUTH_APPLICATION_ID", m_sAuthApplicationId))
	{
		Log::log(0, "����b_param_define���Ƿ�����AUTH_APPLICATION_ID");
		return -1;
	}

    return 0;
}

int TestQueryDcc::readAvps()
{
    char sLineBuff[1024] = {0};
    string strLine;
    string::size_type pos = 0;
    m_sendObj.clear();
    char *p;
    
    ifstream fin(m_sFileName);
    while (fin.getline(sLineBuff, 1024))
    {
        if (strlen(sLineBuff)<4 || sLineBuff[0]=='#')   // ����С��4������#��ʼ���к���
            continue;
        p=strchr(sLineBuff, '\r');
        if (p != NULL)
            *p = '\0';
        strLine = sLineBuff;
        pos = strLine.find_first_of("=");
        m_sendObj[strLine.substr(0, pos)] = strLine.substr(pos+1);
    }
    fin.close();

    if ((m_sendObj[string("461")]).length() < 5)
    {
        Log::log(0, "461����Ϊ��");
        return -1;
    }

    return 0;
}

int TestQueryDcc::createRequest()
{
    if (m_pDccParamMgr == NULL)
        m_pDccParamMgr = new dccParamsMgr;
	m_pDccParamMgr->getDccHeadValue(m_sOriginHost);

    TAvpHead *pHead = &(m_sendObj.avpHead);
    pHead->Version = 1;
    pHead->CommandCode = COMMAND_CODE_CC;
    pHead->CommandFlag = 0x80;
    pHead->AppId = 4;
    pHead->HopId = m_pDccParamMgr->m_iHopByHop;
    pHead->EndId = m_pDccParamMgr->m_iEndToEnd;

    m_sendObj[string("263")] = string(m_pDccParamMgr->m_sSessionID);
    m_sendObj[string("264")] = string(m_sOriginHost);
    m_sendObj[string("296")] = string(m_sOriginRealm);
    m_sendObj[string("283")] = string(m_sDestinationRealm);
    m_sendObj[string("258")] = string(m_sAuthApplicationId);
    //m_sendObj[string("461")] = pDataRow->m_sServiceContext;
    m_sendObj[string("416")] = string("4");
    m_sendObj[string("415")] = string("0");
    
    return 0;
}

