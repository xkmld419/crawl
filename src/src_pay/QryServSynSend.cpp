#include "QryServSynSend.h"
#include "errorcode.h"
#include "ABMException.h"

static MessageQueue *g_poRcvMQ = NULL;         //������Ϣ����
static MessageQueue *g_poSndMQ = NULL;         //������Ϣ����

static vector<struct QryServSynCond> g_vServ;				//�û�������Ϣ
static vector<QryServSynCond *>::iterator g_itr;
	
//static vector<QryServSynData *> g_vData;					//����ͬ�����Ӧ��
//static vector<QryServSynData *>::iterator g_its;		

QryServSynSend::QryServSynSend()
{
    m_poSql = NULL;
    //oCCA = NULL;
    //oCCR = NULL;
    m_dccOperation = new DccOperation;
}

QryServSynSend:: ~QryServSynSend()
{
    if (m_poSql != NULL)
        delete m_poSql;
    if (m_dccOperation != NULL)
		delete m_dccOperation;
    FreeObject(g_poSndMQ);
    FreeObject(g_poRcvMQ);
}

int QryServSynSend::init(ABMException &oExp)
{
	//��ȡ�����ļ�
    ABMConfStruct appconf;
	if (ABMConf::getConf(g_iProcID, appconf, oExp) != 0) {
		ADD_EXCEPT1(oExp, "ABMMgr::init ʧ��, ����ABMMgr -p %d ��abm_app_cfg.ini������!", g_iProcID);
		return -1;		
	}
	
//����IPC��Դ 
	if ((g_poRcvMQ=new MessageQueue(appconf.m_iRcvKey)) == NULL) {
        ADD_EXCEPT0(oExp, "ABMMgr::init g_poRcvMQ malloc failed!");
        return -1;
    }
	if (g_poRcvMQ->open(oExp, true, true) != 0) {
        ADD_EXCEPT1(oExp, "ABMMgr::init g_poRcvMQ->open ��Ϣ��������ʧ��!", appconf.m_iRcvKey);
        return -1;
    }
	if ((g_poSndMQ=new MessageQueue(appconf.m_iSndKey)) == NULL) {
        ADD_EXCEPT0(oExp, "ABMMgr::init g_poSndMQ malloc failed!");
        return -1;
    }
	if (g_poSndMQ->open(oExp, true, true) != 0) {
        ADD_EXCEPT1(oExp, "ABMMgr::init g_poSndMQ->open ��Ϣ��������ʧ��!", appconf.m_iSndKey);
        return -1;
    }
	
    if ((m_poSql=new QryServSynSql) == NULL) {
        ADD_EXCEPT0(oExp, "QryServSynSql::init malloc failed!");
        return -1;
    }
    if (m_poSql->init(oExp) != 0) {
        ADD_EXCEPT0(oExp, "QryServSynSql::init m_poSql->init failed!");
        return -1;
    }
    return 0;
}

int QryServSynSend::deal()
{
	QryServSynCCR oCCR;
	QryServSynCCA oCCA;
	char sBuff[64];
	
	//struDccHead *pHead;
	//pHead = (struDccHead *)oCCR.m_sDccBuf;
	
	struct struDccHead pHead;
	char dccType[1];	// DCC��Ϣ����
	strcpy(dccType,"R"); 
	long lDccRecordSeq=0L;
	int iRet;
    oCCR.clear();
    oCCA.clear();
    ABMException oExp;
    m_poSql->vec.clear();
    m_poSql->ses.clear();
    //g_vData.clear();
    
	try
	{
	//ɨ���û���Ϣ������
	if(iRet = m_poSql->selectServAdd(oExp) < 0){
		__DEBUG_LOG1_(0, "ɨ���û���Ϣ�������=[%d]",iRet);
		throw iRet;
		}

	//��HSS����CCR����
	for(int j=0;j<m_poSql->vec.size();j++){
		//��Ϣͷ��ֵ
		if(iRet = m_poSql->selectSession(m_poSql->vec[j].m_sDsnNbr,oExp) <0){
			__DEBUG_LOG1_(0, "��ѯ��������ˮ�Ŵ�=[%d]",iRet);
			throw iRet;
			}
			for(int i=0;i<m_poSql->ses.size();i++){
				snprintf(pHead.m_sSessionId,sizeof(pHead.m_sSessionId),"%s\0",m_poSql->ses[i]);
				}
		//snprintf(pHead.m_sSessionId,sizeof(pHead.m_sSessionId),"%s\0",m_poSql->m_oSessionId);
		snprintf(pHead.m_sSrvTextId, sizeof(pHead.m_sSrvTextId),"%s\0","InfoSynchroFromABM000.Micropayment@001.ChinaTelecom.com");
		snprintf(pHead.m_sSrvFlowId,sizeof(pHead.m_sSrvFlowId),"%s\0","sn-qqqwe-aaabbbcccddd-version-flowid-track");
		snprintf(pHead.m_sOrignHost,sizeof(pHead.m_sOrignHost),"%s\0","ABM@001.ChinaTelecom.com"); // 2011.8.1
		snprintf(pHead.m_sOrignRealm,sizeof(pHead.m_sOrignRealm),"%s\0","001.ChinaTelecom.com");
		snprintf(pHead.m_sDestRealm,sizeof(pHead.m_sDestRealm),"%s\0","001.ChinaTelecom.com");
		
		pHead.m_iReqType  = 4;
		pHead.m_iReqNumber = 0;		
		pHead.m_iAuthAppId = 4;
		pHead.m_iResultCode = 0;
		
		// ����CCR��Ϣͷ��ֵ
		memcpy((void *)(oCCR.m_sDccBuf),(void *)&pHead,sizeof(pHead));
		// ȡDCC_INFO_RECORD_SEQ
		iRet = m_dccOperation->QueryDccRecordSeq(lDccRecordSeq);
		//����CCR��Ϣͷ����TT
		iRet = m_dccOperation->insertDccInfo(pHead,dccType,lDccRecordSeq);
		if(iRet != 0)
		{
			__DEBUG_LOG0_(0, "[��������ͬ��-��CCR]:����CCR����Ϣͷ��Ϣʧ��");
			throw  iRet;
		}
		oCCR.m_iCmdID = ABMCMD_QRY_SRV_SYN;
		
		//���
		if(!oCCR.addServSyn(m_poSql->vec[j])){
			__DEBUG_LOG0_(0, "QryServSyn:: _deal oCCR ���ʧ��!");
			oCCA.setRltCode(ECODE_PACK_FAIL);
			return INFO_SYN_PACK_ERR;
			}
			//����CCR
			if (g_poSndMQ->Send((void *)&oCCR, oCCR.m_iSize) <= 0) {
				__DEBUG_LOG0_(0, "QryServSyn:: _deal oCCR ���Ͱ�ʧ��!");
				return INFO_SYN_PACK_ERR;
				}
				
			oCCR.clear();
			}
	}
	catch(TTStatus oSt) 
	{
		iRet=INFOSYN_TT_ERR;
		__DEBUG_LOG1_(0, "֧��ʧ�� oSt.err_msg=%s", oSt.err_msg);
	}
	catch(int &value ) 
	{
		iRet=value;
		__DEBUG_LOG1_(0, "֧��ʧ�� value=%d",value);
	}	
	
	return iRet;
};
