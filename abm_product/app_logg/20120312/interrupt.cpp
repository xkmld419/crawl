#include "interrupt.h"

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
