#ifndef __ABM_SYN_RCV_H_INCLUDED_
#define __ABM_SYN_RCV_H_INCLUDED_

#include "abmcmd.h"
#include "QryServSynSql.h"
#include "DccOperation.h"
#include "DccOperationSql.h"

class ABMException;
class DccOperation;

/*
*   �������ϲ�����
*	 ȫ������HSS���յ�ȫ��ABM���͵Ŀ�������ͬ������󣬷��ؽ��CCA
*/

class ABMSynRcv
{
	public:
		ABMSynRcv();
		~ABMSynRcv();
		
		int init(ABMException &oExp);
		int deal(ABMCCR *pRecivePack, ABMCCA *pSendPack); // ������յ�CCA ������CCA

	private:
		int _deal(QryServSynData *pData,char *m_oSessionId);
		int _dealErr(QryServSynData *pData,char *m_oSessionId);
		
	private:
		ABMException *oExp;
		DccOperation *m_dccOperation;
		QryServSynCCA *m_poUnpack;	//���-ȫ��HSS���ص�CCA�� ȫ��HSS��ȫ��ABM
		QryServSynSql *m_poSql;
};

#endif /*__ABM_SYN_RCV_H_INCLUDED_*/
