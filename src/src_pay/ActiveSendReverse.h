// ActiveSendReverse.h ��Ӧʡ��δ����CCA������������ͳ�������

#ifndef __ACTIVE_SEND_REVERSE_H_INCLUDED_
#define __ACTIVE_SEND_REVERSE_H_INCLUDED_

#include "ReadAppConf.h"
#include "LogV2.h"
#include "abmcmd.h"
#include "Application.h"

class ActiveSendReverse : public Application
{
public:
	ActiveSendReverse() {};
	
	virtual ~ActiveSendReverse() {};
		
	virtual int init(ABMException &oExp);
	
	virtual int run();		
	
	virtual int destroy();
	
};

DEFINE_MAIN(ActiveSendReverse)

#endif /*__ACTIVE_SEND_REVERSE_H_INCLUDED_*/

