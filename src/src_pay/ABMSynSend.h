/*
*ABM�˿�������ͬ����������·���send��
*/

#ifndef __ABMSYN_SEND_H_INCLUDED_
#define __ABMSYN_SEND_H_INCLUDED_

//#include "Application.h"
#include "QryServSynSend.h"

class ABMSynSend :public Application
{
	public:
		ABMSynSend() {};
		
	virtual int init(ABMException &oExp);
	
	virtual int run();		
	
	virtual int destroy();
	
	private:
		PublicConndb *m_pPublicConndb;//�������ݿ�����
};

DEFINE_MAIN(ABMSynSend)

#endif /*__ABMSYN_SEND_H_INCLUDE_*/
