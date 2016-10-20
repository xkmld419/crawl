#include "ReadAppConf.h"
#include "MessageQueue.h"
#include "QryServSynSend.h"
#include "ABMSynSend.h"
#include "LogV2.h"
#include "abmcmd.h"
#include "PublicConndb.h"

static QryServSynSend *g_poSynSend = NULL;						//����ͬ�������·�������

/*
*ABMSYNSEND��ʼ������
*/
ABMException oExp6;
int ABMSynSend::init(ABMException &oExp)
{
		if (g_iProcID == -1) {
		ADD_EXCEPT0(oExp, "���������б������� -p ��!");
		return -1;	
}

    if ((g_poSynSend=new QryServSynSend) == NULL) {
        ADD_EXCEPT0(oExp, "ABMMgr::init g_poSyn malloc failed!");
        return -1;
    } 
    if (g_poSynSend->init(oExp) != 0) {
        ADD_EXCEPT0(oExp, "ABM�ۻ���ҵ�������ʼ��ʧ��!");
        return -1;
    }
    	if (m_pPublicConndb->init(oExp) != 0)
	{
		ADD_EXCEPT0(oExp, "ABMCheck::init m_pPublicConndb->init failed!");
		return -1;
	}
    
	return 0;	
}

/*
	����ͬ�������·�ҵ����send�ˣ�
	�������ܣ�1.�����������û�����CCR����ȫ��HSSͬ�����Ͷ���֪ͨ�����ɹ�
*/

int ABMSynSend::run()
{
	int iRet = -1;
	while(!RECEIVE_STOP())
	{
		iRet = g_poSynSend->deal();
		
		// �ύ
		if(iRet==0)
		{
        		__DEBUG_LOG0_(0, "m_poAllocateBalance->deal  ---0");
			m_pPublicConndb->Commit(oExp6);
		}
		else
		{
        		__DEBUG_LOG0_(0, "m_poAllocateBalance->deal  ---1");
			m_pPublicConndb->RollBack(oExp6);
		}
		sleep(60);
	}
	
	__DEBUG_LOG0_(0, "�����յ���ȫ�˳��ź�, ��ȫ�˳�");
	
	return 0;
}

int ABMSynSend::destroy()
{
	//�ͷ�ҵ������

	#define __FREE_PTR(P) \
/*
       if (p != NULL) { \
            delete p; p = NULL; }
*/

    __FREE_PTR(g_poSynSend);
    //__FREE_PTR(g_poSndMQ);
    //__FREE_PTR(g_poRcvMQ);
	
	return 0;
}
