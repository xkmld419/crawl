#include "DccUnPack.h"

DEFINE_MAIN (DccUnPack)

#define SHM_SERVICETYPE    (IpcKeyMgr::getIpcKey(Process::m_iFlowID, "SHM_SERVICETYPE"))
/*
void* getShmAddr(key_t shmKey, int shmSize)
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
*/


void getPack(char *buf, char *sPack, int iPackLen)
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

DccUnPack::DccUnPack()
{
}

DccUnPack::~DccUnPack()
{
    //m_mapTransContext.clear();
    delete []m_ContextIds;
    m_ContextIds = NULL;
    delete m_pRecvBuffer;
    m_pRecvBuffer = NULL;
    delete m_pSendBuffer;
    m_pSendBuffer = NULL;
    delete m_pOcpMsgParser;
    m_pOcpMsgParser = NULL;
    delete m_pFileToDccMgr;
    m_pFileToDccMgr = NULL;
}

void DccUnPack::printManual()
{
    Log::log (0,
              "********\n"
              "dccUnPack ���÷�����\n"
              "-o taskID   �����\n");
}


bool DccUnPack::prepare()
{
    m_iTaskId = -1;

    for (int i=1; i<g_argc; ++i)
    {
        if (g_argv[i][0] != '-')
        {
            printManual();
            return false;
        }
        switch (g_argv[i][1] | 0x20)
        {
        case 'o':
            i++;
            m_iTaskId = atoi(g_argv[i]);
            break;
        default:
            printManual();
            return false;
        }
    }

    if (m_iTaskId == -1)
    {
        Log::log(0, "�������в���ָ��ִ������ID");
        printManual();
        return false;
    }
    return true;
}

int DccUnPack::init()
{
    char sql[1024];
    DEFINE_QUERY (qry);

    memset (sql, 0, sizeof(sql));
    sprintf(sql, "SELECT SUM(1) FROM TRANS_FILE_SERVICE_CONTEXT WHERE TRANS_FILE_TYPE_ID<>0 AND DCC_TASK_ID=%d", m_iTaskId);
    qry.setSQL(sql);
    qry.open();
    qry.next();
    m_iContextNum = qry.field(0).asInteger();
    qry.commit();
    qry.close();

    m_ContextIds = new char[m_iContextNum][256];

    int i = 0;
    memset (sql, 0, sizeof(sql));
    sprintf(sql, "SELECT A.TRANS_FILE_TYPE_ID, A.SERVICE_CONTEXT FROM TRANS_FILE_SERVICE_CONTEXT A WHERE A.TRANS_FILE_TYPE_ID<>0 AND A.DCC_TASK_ID=%d", m_iTaskId);
    qry.setSQL(sql);
    qry.open();
    while (qry.next())
    {
        // m_mapTransContext[qry.field(1).asString()] = qry.field(0).asInteger();
        strcpy(m_ContextIds[i++], qry.field(1).asString());
    }
    qry.commit();
    qry.close();

    m_pFileToDccMgr = FileToDccMgr::getInstance();
    m_pFileToDccMgr->init();

    ParamDefineMgr::getParam("DCC", "ORIGIN_HOST", m_sOriginHost);
    if (strlen(m_sOriginHost) == 0)
    {
        Log::log(0, "����b_param_define���Ƿ�����ORIGIN_HOST");
        return -1;
    }

    memset(sql, 0, sizeof(sql));
    sprintf(sql, "SELECT  A.SERVICE_CONTEXT,B.DCC_NE_HOST FROM TRANS_FILE_SERVICE_CONTEXT a,TRANS_DCC_TASK B "
            " WHERE A.DCC_TASK_ID=B.DCC_TASK_ID AND A.TRANS_FILE_TYPE_ID=0 AND B.DCC_TASK_ID=%d", m_iTaskId);
    qry.setSQL(sql);
    qry.open();
    qry.next();
    strcpy(m_sActiveContext, qry.field(0).asString());
    strcpy(m_sNeHost,qry.field(1).asString());
    qry.commit();
    qry.close();

    memset (sql, 0, sizeof(sql));
    sprintf(sql, "SELECT MAX(A.MSG_EVENT_NUM) FROM WF_MQ_ATTR A");
    qry.setSQL(sql);
    qry.open();
    qry.next();
    m_iMsgEventNum = qry.field(0).asInteger();
    qry.commit();
    qry.close();

    m_pRecvBuffer = new char[sizeof(StdEvent) * m_iMsgEventNum];			// ���ջ�����
    m_pSendBuffer = new char[sizeof(StdEvent) * m_iMsgEventNum];			// ���ͻ�����
    m_pcopyBuffer = new char[sizeof(StdEvent) * m_iMsgEventNum];	   //����һ��

    m_pOcpMsgParser = new OcpMsgParser("dictionary.xml");


    char sOriginHost[81] = {0};
    ParamDefineMgr::getParam("DCC", "ORIGIN_HOST", sOriginHost);
    if (strlen(sOriginHost) == 0)
    {
        Log::log(0, "����b_param_define���Ƿ�����ORIGIN_HOST");
    }
    char sOriginRealm[81] = {0};
    if (!ParamDefineMgr::getParam("DCC", "ORIGIN_REALM", sOriginRealm))
    {
        Log::log(0, "����b_param_define���Ƿ�����ORIGIN_REALM");
        return -1;
    }
    char sLocalIp[64] = {0};
    if (!ParamDefineMgr::getParam("DCC", "LOCAL_IP", sLocalIp))
    {
        Log::log(0, "����b_param_define���Ƿ�����LOCAL_IP");
        return -1;
    }

    m_pOcpMsgParser->initLocalInfo(sOriginHost, sOriginRealm, sLocalIp);

    m_pDccParamMgr = new dccParamsMgr;
    m_pDccParamMgr->getDccHeadValue(m_sNeHost);

    m_nTime = 10;          // active CCA ��ʱʱ��ȱʡΪ10
    char buffer[256];
    if (ParamDefineMgr::getParam("DCC", "ACTIVE_TIMEOUT", buffer))
        m_nTime = atoi(buffer);

    m_isActiveCCAOK = 1;

    char sLogPath[125] = {0};
    if (!ParamDefineMgr::getParam("DCC", "LOG_PATH", sLogPath))
    {
        Log::log(0, "����b_param_define���Ƿ�����LOG_PATH");
        return -1;
    }
    DccLog::setFileName(sLogPath, "DccUnPack", GetProcessID());   // ������־�ļ���


    m_shmServiceType =(char*)m_pOcpMsgParser->getshm();
    return 0;
}

int DccUnPack::run()
{
    if (prepare() < 0)
        return -1;

    if (init() < 0)
        return -1;

    DenyInterrupt();

    Date dDate;
    Log::log(0, "dccProc run ......");
    while (!GetInterruptFlag())
    {
        if (m_isActiveCCAOK == 0)    // ����CCAδ����
        {
            dDate.getCurDate();     // ��ȡ��ǰʱ��
            if (dDate.diffSec(m_dDate) > m_nTime)      // �ж��Ƿ�ʱ
            {
                /* ���ͼ���CCR*/
                createActiveCCR();
                prefixStdEvent("ACTIVECCR");
                m_poSender->send(m_pSendBuffer);
                Log::log(0,"��ʱδ�յ�����CCA�������·��ͼ���CCR��");
                m_dDate.getCurDate();    // ��¼�ط�ʱ��
            }
        }

        memset(m_pRecvBuffer, 0, sizeof(StdEvent)*m_iMsgEventNum);
        if (m_poReceiver->receive(m_pRecvBuffer, false) <= 0)
        {
            usleep(5000);
            continue;
        }
         //����һ�ݱ���
         memset(m_pcopyBuffer,0x00,sizeof(StdEvent)*m_iMsgEventNum);
	memcpy(m_pcopyBuffer,m_pRecvBuffer,sizeof(StdEvent)*m_iMsgEventNum);

        // ��鼤��CCR�Ƿ���������ʧ�ܣ���ʧ���´β��жϳ�ʱ
        StdEvent *pEvent = (StdEvent *)m_pRecvBuffer;
        if (strcmp(pEvent->m_sBillingNBR, "REBOOT") == 0)
        {
            Log::log(0,"����CCR��������ʧ�ܣ������жϳ�ʱ");
            m_isActiveCCAOK = 1;
            continue;
        }

        memset(m_sServiceContextId, 0, sizeof(m_sServiceContextId));
        memset(m_pSendBuffer, 0, sizeof(StdEvent) * m_iMsgEventNum);

        // ��� m_pSendBuffer+sizeof(StdEvent)�д�Ž��������ӦA���Ĵ�����������ԭ��Ϣ���ͣ�Service-Context-Id
        m_iMsgType = m_pOcpMsgParser->parserPack(m_pRecvBuffer+sizeof(StdEvent), sizeof(StdEvent)*(m_iMsgEventNum-1),
                     m_pSendBuffer+sizeof(StdEvent), sizeof(StdEvent)*(m_iMsgEventNum-1), m_sServiceContextId);

        if (procMsg() < 0)
        {
            Log::log(0, "��Ϣ�޷�����");
            // add 20100601
            // ��¼��Ԫ״̬��Online���ϡ���stat_ne_desc
            updateNeState(ONLINE, ONLINE_FAULT);
        }

    }
    DccLog::close();

    return 0;
}

int DccUnPack::procMsg()
{
    int transFileTypeID = 0;

    // ��Service-Context-Id!=0�����ö�Ӧ��ҵ�����͵�transFileTypeID
    // ��һ�η��ص�CCA��Ϊ��������ظ�������Service-Context-Id
    if (strlen(m_sServiceContextId)!=0)
    {
        transFileTypeID = m_pFileToDccMgr->getFileTypeByServiceContext(m_sServiceContextId);
    }

    string strAvpBuf;
    char sPackBuf[61441] = {0};
    string sDelimitLine(100, '*');
    string strLogContext = string("");      // ��Ű�����־����

std::string tmp;
    DEFINE_QUERY (qry);
    switch (m_iMsgType)
    {
    case CEA:
        // �ж�CER�Ƿ�ʧ��
        if (atoi(m_pSendBuffer+sizeof(StdEvent)) != 2001)     // CER����ʧ��
        {
            Log::log(0, "CER����ʧ�ܣ�������Ϣ��dccTran");
            memset(m_pSendBuffer, 0, sizeof(StdEvent) * m_iMsgEventNum);
            prefixStdEvent("CERFAILED");
            m_poSender->send(m_pSendBuffer);
            break;
        }
        Log::log(0,"�յ�CEA��CER���ͳɹ�");
        strAvpBuf=m_pOcpMsgParser->getAllAvp(true);
        getPack(m_pRecvBuffer+sizeof(StdEvent), sPackBuf, 61441);
        strLogContext += strAvpBuf + sPackBuf + '\n' + sDelimitLine;
        DccLog::log(strLogContext.c_str());       // ��¼������־

        // ���ô���ӿ����ɼ���CCR��m_pSendBuffer+sizeof(StdEvent)
        createActiveCCR();
        strAvpBuf=m_pOcpMsgParser->getAllAvp(true);
        memset(sPackBuf, 0, sizeof(sPackBuf));
        strLogContext = string("");
        getPack(m_pSendBuffer+sizeof(StdEvent), sPackBuf, 61441);
        strLogContext += strAvpBuf + sPackBuf + '\n' + sDelimitLine;
        DccLog::log(strLogContext.c_str());       // ��¼������־

        prefixStdEvent("ACTIVECCR");
        m_poSender->send(m_pSendBuffer);
        Log::log(0,"�ѷ��ͼ���CCR��");
        m_isActiveCCAOK = 0;  // ����CCAδ����
        m_dDate.getCurDate();  // ��¼����ʱ��
        AddTicketStat(TICKET_NORMAL);

        break;
    case CCR1:
        // ���������Ľ��ǰ��StdEvent��m_pSendBuffer
        strAvpBuf=m_pOcpMsgParser->getAllAvp(true);
        getPack(m_pRecvBuffer+sizeof(StdEvent), sPackBuf, 61441);
        strLogContext += strAvpBuf + sPackBuf + '\n' + sDelimitLine;
        DccLog::log(strLogContext.c_str());       // ��¼������־
        prefixStdEvent("CCR", transFileTypeID);

        //modified by zhufengsheng 2011.03.17
        //�������ҵ��Դ��
        //����Ǿ�̬�������ҵ��
        //�����֮���
        //�µ�ҵ����Ҳ���^^^����
        //��ҵ�����ƴ�ղ��𻰵�
        tmp = m_pSendBuffer + sizeof(StdEvent);
        if(tmp.find("^^^") == std::string::npos)
        {
            //��Դ���dccUnPack�������
            strcpy(((StdEvent*)(m_pcopyBuffer))->m_sBillingNBR, "RAW");
            Log::log(0,"�յ���ҵ��CCR,����Դ����������������");
            m_poSender->send(m_pcopyBuffer);
        }
        else
        {
            //������֮���
            //strcpy(((StdEvent*)(m_pSendBuffer))->m_sCalledAreaCode,"OLD");
            Log::log(0,"����CCR���ɹ���̬������");
            m_poSender->send(m_pSendBuffer);
        }

        AddTicketStat(TICKET_NORMAL);


        //��������־��2010-12-07 by ���ʤ

        try
        {
            std::string sContext = m_pSendBuffer+sizeof(StdEvent);
            if(sContext.find("^^^")!=std::string::npos)
            {
                sContext =sContext.substr(sContext.find("^^^")+3);
            }
            std::string sSessionId;
            m_pOcpMsgParser->getAvpValue("Session-Id",sSessionId);
            long iHopByhop =  m_pOcpMsgParser->getHopByHop();

            char szSql[2048]= {0};
            sprintf(szSql,"insert into dcc_trans_log(sessionid,hopbyhop,recivemsgtype,in_sys_time,indb_time,RECEIVE_CONTEXT)  \
                                    values('%s',%ld,'CCR',to_timestamp('%s','yyyymmddhh24missff'),systimestamp,'%s')",
                    sSessionId.c_str(),iHopByhop,((StdEvent*)(m_pRecvBuffer))->m_sReservedField[0],sContext.c_str());
            qry.close();
            qry.setSQL(szSql);
            qry.execute();
            qry.commit();
            qry.close();
        }
        catch ( TOCIException &e )
        {
            std::ostringstream os_log;
            os_log.str( "" );
            os_log << "***********************sql error*********************** \n" << std::endl << e.getErrCode()
            << std::endl <<e.GetErrMsg() << std::endl << e.getErrSrc() << std::endl;
            Log::log( 0,os_log.str().c_str());
        }
        //end of add by ���ʤ2010-12-07
        break;

    case CCA:
        strAvpBuf=m_pOcpMsgParser->getAllAvp(true);
        getPack(m_pRecvBuffer+sizeof(StdEvent), sPackBuf, 61441);
        strLogContext += strAvpBuf + sPackBuf + '\n' + sDelimitLine;
        DccLog::log(strLogContext.c_str());       // ��¼������־
        AddTicketStat(TICKET_NORMAL);

        if (strlen(m_sServiceContextId)!=0)   // ����CCA�����ж��Ƿ�ʧ��
        {
            char *p=strchr(m_pSendBuffer+sizeof(StdEvent),'|');
            int iResultCode=0;
            int iServiceResultCode=-1;
            if (p)
            {
                iResultCode=atoi(m_pSendBuffer+sizeof(StdEvent));
                iServiceResultCode = atoi(p+1);
            }
            Log::log(0,"�յ�����CCA��result_code:%d,serivce_result_code=%d,ServiceContextID = %s,",iResultCode,iServiceResultCode,m_sServiceContextId);
            if (!p || iResultCode!=2001 ||  iServiceResultCode!=0)    // ����ʧ�ܣ����·��ͼ���CCR
            {
                createActiveCCR();
                prefixStdEvent("ACTIVECCR");
                m_poSender->send(m_pSendBuffer);
                Log::log(0,"����ʧ�ܣ����·��ͼ���CCR��");
                m_dDate.getCurDate();    // ��¼�ط�ʱ��
            }
            else
            {
                Log::log(0,"����ɹ�");
                m_isActiveCCAOK = 1;   // ����ɹ�
                // add 20100601
                // ��¼��Ԫ״̬��Online_��������stat_ne_desc
                updateNeState(ONLINE, ONLINE_NORMAL);
            }
        }
        else
        {
            // ���������Ľ��ǰ��StdEvent��m_pSendBuffer
            prefixStdEvent("CCA", transFileTypeID);
//------------------------------------------------------------------------------
            std::string sessionid = m_pOcpMsgParser->getSessionid();
            char serviceType[SHM_ITEM_LEN+1]= {0};
            getShmServiceType(sessionid.c_str(),serviceType);
            //�Ҳ��������ڴ��е�serviceType��ʾ����ҵ��
            if(strcmp(serviceType,"")==0)
            {
                //���ͽ������ַ���
                //strcpy(((StdEvent*)(m_pSendBuffer))->m_sBillingNBR,"RAW");
                m_poSender->send(m_pSendBuffer);
                Log::log(0,"����CCA���ɹ���̬������ҵ��");
            }
            else
            {
                //����Դ��
                strcpy(((StdEvent*)(m_pcopyBuffer))->m_sBillingNBR,"RAW");
                m_poSender->send(m_pcopyBuffer);
	       Log::log(0,"�յ���ҵ��CCA,����Դ����������������");
            }
//------------------------------------------------------------------------------


            //�������dcc_trans_log modified by ���ʤ2010.12.07
            std::string sContext = m_pSendBuffer+sizeof(StdEvent);

            try
            {
                char szSql[2048]= {0};
                sprintf(szSql,"update dcc_trans_log t set t.recivemsgtype = 'CCA',t.in_sys_time = to_timestamp('%s','yyyymmddhh24missff') ,  \
                                         RECEIVE_CONTEXT = '%s' where t.sessionid = '%s' and t.hopbyhop = %ld",
                        ((StdEvent*)(m_pRecvBuffer))->m_sReservedField[0],
                        sContext.c_str(),
                        m_pOcpMsgParser->getSessionid().c_str(),
                        m_pOcpMsgParser->getHopByHop());
                qry.close();
                qry.setSQL(szSql);
                qry.execute();
                qry.commit();
                qry.close();
            }
            catch ( TOCIException &e )
            {
                std::ostringstream os_log;
                os_log.str( "" );
                os_log << "***********************sql error*********************** \n" << std::endl
                << e.getErrCode()<< std::endl <<e.GetErrMsg() << std::endl << e.getErrSrc() << std::endl;
                Log::log( 0,os_log.str().c_str());
            }
        }
        break;

    case DWR:
        // ���������ɵ�A��ǰ��StdEvent��m_pSendBuffer
        prefixStdEvent("DWA");
        m_poSender->send(m_pSendBuffer);
        Log::log(0,"�յ�DWR��������DWA��");
        break;
    case DPR:
        // ���������ɵ�A��ǰ��StdEvent��m_pSendBuffer
        prefixStdEvent("DPA");
        m_poSender->send(m_pSendBuffer);
        Log::log(0,"�յ�DPR��������DPA��");
        break;

    default:
        // ����
        strAvpBuf=m_pOcpMsgParser->getAllAvp(true);
        getPack(m_pRecvBuffer+sizeof(StdEvent), sPackBuf, 61441);
        DccLog::log("%s%s\n%s", strAvpBuf.c_str(), sPackBuf, sDelimitLine.c_str());       // ��¼������־
        break;
    }

    return 0;
}

int DccUnPack::createActiveCCR()
{
    if (m_pDccParamMgr == NULL)
        m_pDccParamMgr = new dccParamsMgr;

    m_pDccParamMgr->getDccHeadValue(m_sNeHost);

    /*��m_ActiveCCRArgv��ֵ*/
    char buff[256] = {0};
    memset(m_ActiveCCRArgv, 0, sizeof(m_ActiveCCRArgv));
    sprintf(m_ActiveCCRArgv, "%s|%d|%d", m_pDccParamMgr->m_sSessionID, m_pDccParamMgr->m_iHopByHop, m_pDccParamMgr->m_iEndToEnd);

//---------------------------------------------------------------------------
/*
    char *p = strrchr(m_pDccParamMgr->m_sSessionID, ';');
    unsigned int hh = atol(p+1);
    memset(m_shmServiceType+(hh%SHM_ITEM_NUM)*SHM_ITEM_LEN, 0, SHM_ITEM_LEN);
    strcpy(m_shmServiceType+(hh%SHM_ITEM_NUM)*SHM_ITEM_LEN, "Active.Net-Ctrl");
    */
//---------------------------------------------------------------------------


    if (!ParamDefineMgr::getParam("DCC", "DESTINATION_REALM", buff))
    {
        Log::log(0, "����b_param_define���Ƿ�����DESTINATION_REALM");
        return -1;
    }
    strcat(m_ActiveCCRArgv, "|");
    strcat(m_ActiveCCRArgv, buff);
    memset(buff, 0, sizeof(buff));
    if (!ParamDefineMgr::getParam("DCC", "AUTH_APPLICATION_ID", buff))
    {
        Log::log(0, "����b_param_define���Ƿ�����AUTH_APPLICATION_ID");
        return -1;
    }
    strcat(m_ActiveCCRArgv, "|");
    strcat(m_ActiveCCRArgv, buff);
    strcat(m_ActiveCCRArgv, "|");
    strcat(m_ActiveCCRArgv, m_sActiveContext);
    strcat(m_ActiveCCRArgv, "|4|0");

    return m_pOcpMsgParser->createActiveCCR(m_ActiveCCRArgv, m_ContextIds, m_iContextNum,
                                            m_pSendBuffer+sizeof(StdEvent), sizeof(StdEvent)*(m_iMsgEventNum-1));
}

void DccUnPack::prefixStdEvent(const char *sPackType, int iTransFileTypeID)
{
    StdEvent *pEvent = (StdEvent*)m_pSendBuffer;

    if (sPackType != NULL)
        strcpy(pEvent->m_sBillingNBR, sPackType);
    pEvent->m_iBillingTypeID = iTransFileTypeID;

}


void DccUnPack::updateNeState(int iBsnState, int iNeState)
{
    char sql[1024];
    DEFINE_QUERY (qry);

    if (iNeState == ONLINE_NORMAL)
    {
        memset (sql, 0, sizeof(sql));
        sprintf(sql, "INSERT INTO STAT_NE_DESC (ORIGIN_HOST, TASK_ID, BSN_STATE, NE_STATE, SYSUPTIME, STATE_TIME) "
                "VALUES ('%s', '%d', %d, %d, SYSDATE, SYSDATE)", m_sOriginHost, m_iTaskId, iBsnState, iNeState);
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
                    "VALUES ('%s', '%d', %d, %d, NULL, SYSDATE)", m_sOriginHost, m_iTaskId, iBsnState, iNeState);
        }
        else
        {
            sprintf(sql, "INSERT INTO STAT_NE_DESC (ORIGIN_HOST, TASK_ID, BSN_STATE, NE_STATE, SYSUPTIME, STATE_TIME) "
                    "VALUES ('%s', '%d', %d, %d, to_date('%s', 'YYYYMMDDHH24MISS'), SYSDATE)",
                    m_sOriginHost, m_iTaskId, iBsnState, iNeState, sSysUpTime);
        }
    }
    qry.setSQL(sql);
    qry.execute();
    qry.commit();
    qry.close();
}


void DccUnPack::getShmServiceType(const char *sessionId, char *serviceType)
{
    char *p = strrchr((char*)sessionId, ';');
    unsigned int hh = atol(p+1);
    //m_shmServiceType = (char *)getShmAddr(SHM_SERVICETYPE, SHM_ITEM_NUM * SHM_ITEM_LEN);
    strcpy(serviceType, m_shmServiceType+(hh%SHM_ITEM_NUM)*SHM_ITEM_LEN);
    //modified bu zhufengsheng 2011.03.17
}

void* DccUnPack::getShmAddr(key_t shmKey, int shmSize)
{
    int shmid;
    if((shmid=shmget(shmKey, shmSize, SHM_R|SHM_W|IPC_EXCL|IPC_CREAT))==-1)
    {
        if(errno==EEXIST)
            shmid=shmget(shmKey,0,0);
        else
            return NULL;
    }
         void* p = NULL;
		 p = shmat(shmid, NULL, 0);
	return  p;
    
}
