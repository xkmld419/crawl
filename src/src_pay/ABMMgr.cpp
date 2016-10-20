#include "ABMMgr.h"
#include "ReadAppConf.h"
#include "MessageQueue.h"
#include "ABMSvcSupport.h"
#include "LogV2.h"
#include "abmcmd.h"

static MessageQueue *g_poRcvMQ = NULL;         //������Ϣ����
static MessageQueue *g_poSndMQ = NULL;         //������Ϣ���� 
static ABMSvcSupt *g_poSvc     = NULL;         //ABMCenterҵ������
static ABMCCR g_oRcvBuf;                 //���ջ���
static ABMCCA g_oSndBuf;                 //���ͻ���

// add 2011.7.6
static MessageQueue *g_poSndToPlatformMQ = NULL;         //������Ϣ����-���Է���ƽ̨ 
bool g_toPlatformFlag = false;

/*
*	ABM-CENTER �������ĳ�ʼ������    
*/
int ABMMgr::init(ABMException &oExp)
{
	if (g_iProcID == -1) {
		ADD_EXCEPT0(oExp, "���������б������� -p ��!");
		return -1;	
	}
	
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
    
    // add 2011.7.6 ��ʼ�������Է���ƽ̨��Ϣ����
    if ((g_poSndToPlatformMQ=new MessageQueue(appconf.m_iSndToPlatformKey)) == NULL) {
        ADD_EXCEPT0(oExp, "ABMMgr::init g_poSndToPlatformMQ malloc failed!");
        return -1;
    }
    
    if (g_poSndToPlatformMQ->open(oExp, true, true) != 0) {
        ADD_EXCEPT1(oExp, "ABMMgr::init g_poSndToPlatformMQ->open ��Ϣ��������ʧ��!", appconf.m_iSndToPlatformKey);
        return -1;
    }
    
    if ((g_poSvc=new ABMSvcSupt) == NULL) {
        ADD_EXCEPT0(oExp, "ABMMgr::init g_poSvc malloc failed!");
        return -1;
    } 
    if (g_poSvc->init(oExp) != 0) {
        ADD_EXCEPT0(oExp, "ABM�ۻ���ҵ�������ʼ��ʧ��!");
        return -1;
    }
    
	return 0;	
}

/*
	�ۻ���ҵ����
	��������:
	         �ӽ��ն���ȡ��Ϣ, �ַ�������ҵ���ദ��, ���ؽ�������Ͷ��� 
*/
int ABMMgr::run()
{
	int iRet;
		
	while (!RECEIVE_STOP()) {

		if ((iRet=g_poRcvMQ->Receive((void *)&g_oRcvBuf, sizeof(g_oRcvBuf), 0, true)) < 0) {
			if (iRet == -2) {
				continue;	
			}	
			__DEBUG_LOG1_(0, "Ӧ�ó������Ϣ���н���ʧ��, �����˳�! errno=%d", errno);
			return -1;
		}			
		
		g_poSvc->deal((ABMCCR*)&g_oRcvBuf, (ABMCCA*)&g_oSndBuf); 
		
		// add 2011.7.6 �����жϷ��͵��ĸ����У�1.ʡABM �� 2.�Է���ƽ̨
		struDccHead Head;
	
		memcpy((void *)&Head,(void *)(g_oSndBuf.m_sDccBuf),sizeof(Head));
		
		if(g_toPlatformFlag)
		{
			// �������Է���ƽ̨
			__DEBUG_LOG0_(0, "������Ϣ���Է���ƽ̨����Ϣ����");
			//������Ϣ�����ö�
			if (g_poSndToPlatformMQ->Send((void *)&g_oSndBuf, g_oSndBuf.m_iSize) < 0) {
			__DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			g_toPlatformFlag = false;
			return -1;	
			}
			g_toPlatformFlag = false;
		}
		else
		{
			//������Ϣ�����ö�
			__DEBUG_LOG0_(0, "������Ϣ��ʡABM����Ϣ����");
			if (g_poSndMQ->Send((void *)&g_oSndBuf, g_oSndBuf.m_iSize) < 0) {
			__DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
			return -1;	
			}
		}	
	}
	
	__DEBUG_LOG0_(0, "�����յ���ȫ�˳��ź�, ��ȫ�˳�");
	
	return 0;
}

int ABMMgr::destroy()
{
	//�ͷ�ҵ������
    #define __FREE_PTR(p) \
        if (p != NULL) { \
            delete p; p = NULL; }

    __FREE_PTR(g_poSvc);
    __FREE_PTR(g_poSndMQ);
    __FREE_PTR(g_poRcvMQ);
	
	return 0;
}
