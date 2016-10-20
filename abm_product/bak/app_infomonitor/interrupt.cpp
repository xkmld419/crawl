#include"interrupt.h"
//#include"MBC_SR.h"
//#include "logalertapi.h"
#include"Log.h"

bool __InterruptFlag;

/*
Function Name		: __Interrupt
Description		: �жϴ���
Input Parameters	:
Signo ------------------: �жϺ�
Returns			: none
complete time		: 2000/08/18
test time		: 2000/08/18
*/ 
void __Interrupt(int Signo)
{
  // add by ztl 2010.09.17
	if(Signo != SIGUSR1)
  //C_LogAlertApi::WriteInfo2mq(LOG_LEVEL_1,LOG_TYPE_ALARM,LOG_SUBTYPE_END,
		//          MBC_SR_Process+4,__LINE__, __FILE__,NULL,0,"Ӧ�ó����յ��˳��ź� %d �쳣�˳�",Signo);	
	//ALARMLOG28(0,MBC_CLASS_Process,199,"Ӧ�ó����յ��˳��ź� %d �쳣�˳�",Signo);
	
	signal(Signo,__Interrupt);
	__InterruptFlag = true;
}

/*
Function Name		: Deny__Interrupt
Description		: ���ж�
Input Parameters	: none
Returns			: none
complete time		: 2000/08/18
test time		: 2000/08/18
*/ 
void DenyInterrupt(void)
{
	__InterruptFlag = false;

	signal(SIGINT,__Interrupt);
	signal(SIGQUIT,__Interrupt);
	signal(SIGTERM,__Interrupt);

	signal(SIGFPE,__Interrupt);
	signal(SIGUSR1,__Interrupt);
}

/*
Function Name		: Allow__Interrupt
Description		: ���ж�
Input Parameters	: none
Returns			: none
complete time		: 2000/08/18
test time		: 2000/08/18
*/ 
void AllowInterrupt(void)
{
	signal(SIGINT,SIG_DFL);
	signal(SIGQUIT,SIG_DFL);
	signal(SIGTERM,SIG_DFL);

	signal(SIGFPE,SIG_DFL);
	signal(SIGUSR1,SIG_DFL);
}

/*
Function Name		: Get__InterruptFlag
Description		: ȡ���жϱ�־��
Input Parameters	: none
Returns			: none
complete time		: 2000/08/18
test time		: 2000/08/18
*/ 
bool GetInterruptFlag(void)
{
	return __InterruptFlag;
}
