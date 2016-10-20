#include "ActiveSendReverse.h"
#include "ReadAppConf.h"
#include "MessageQueue.h"
#include "ActiveSendReverseBiz.h"
#include "LogV2.h"
#include "abmcmd.h"

static MessageQueue *g_poSndMQ = NULL;         				//������Ϣ���� 
static ActiveSendReverseBiz *g_activeSendReverseBiz     = NULL;         //ABMCenterҵ������
static ABMCCR g_oSndBuf;                 				//���ͻ���

/*
 *	ActiveSendReverse�������ĳ�ʼ������    
 */
int ActiveSendReverse::init(ABMException &oExp)
{
	if (g_iProcID == -1) 
	{
		ADD_EXCEPT0(oExp, "���������б������� -p ��!");
		return -1;	
	}
	
	//��ȡ�����ļ�
	ABMConfStruct appconf;
	if (ABMConf::getConf(g_iProcID, appconf, oExp) != 0) 
	{
		ADD_EXCEPT1(oExp, "ABMMgr::init ʧ��, ����ABMMgr -p %d ��abm_app_cfg.ini������!", g_iProcID);
		return -1;		
	}

	//����IPC��Դ 
	if ((g_poSndMQ = new MessageQueue(appconf.m_iSndKey)) == NULL) 
	{
        	ADD_EXCEPT0(oExp, "ABMMgr::init g_poSndMQ malloc failed!");
        	return -1;
    	}
    	
	if (g_poSndMQ->open(oExp, true, true) != 0) 
	{
        	ADD_EXCEPT1(oExp, "ABMMgr::init g_poSndMQ->open ��Ϣ��������ʧ��!", appconf.m_iSndKey);
        	return -1;
    	}
    
	if ((g_activeSendReverseBiz = new ActiveSendReverseBiz) == NULL) 
	{
        	ADD_EXCEPT0(oExp, "ActiveSendReverse::init g_activeSendReverseBiz malloc failed!");
        	return -1;
    	} 
	if (g_activeSendReverseBiz->init(oExp) != 0) 
	{
        	ADD_EXCEPT0(oExp, "ABM�������ͳ�������ҵ�������ʼ��ʧ��!");
        	return -1;
    	}
    
	return 0;	
}

/*
 *	ҵ����
 *	��������:
 *	         ɨ��ACTIVE_CCR_LOG��־������״̬Ϊ00X�ļ�¼��ʡABM���ͳ������� 
 */
int ActiveSendReverse::run()
{
	int iRet=-1;
		
	while (!RECEIVE_STOP()) 
	{
		g_oSndBuf.clear();
		g_oSndBuf.m_iSize = 0;
		g_activeSendReverseBiz->deal((ABMCCR*)&g_oSndBuf);
		
		//������Ϣ��ʡABM
		//__DEBUG_LOG0_(0, "���ͻ���������Ϣ��ʡABM����Ϣ����");
		if(g_oSndBuf.m_iSize == 0)
		{
			continue;	
		}
		else
		{
			if (g_poSndMQ->Send((void *)&g_oSndBuf, g_oSndBuf.m_iSize) < 0) 
			{
				__DEBUG_LOG1_(0, "��Ϣ���з���ʧ��, �����˳�! errno = %d", errno);
				return -1;	
			}
			__DEBUG_LOG0_(0, "���ͻ���������Ϣ��ʡABM����Ϣ���гɹ���");
		}
		
	}
	__DEBUG_LOG0_(0, "�����������ͻ�������-�����յ���ȫ�˳��ź�, ��ȫ�˳�");
	
	return 0;
}

int ActiveSendReverse::destroy()
{
	//�ͷ�ҵ������
	#define __FREE_PTR(p) \
	if (p != NULL) { \
		delete p; p = NULL; }
	
	__FREE_PTR(g_activeSendReverseBiz);
	__FREE_PTR(g_poSndMQ);
	
	return 0;
}

