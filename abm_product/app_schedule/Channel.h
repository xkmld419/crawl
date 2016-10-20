/******************************
description:
author: xn
version: 1.0
date: 2010-04-28
******************************/
/*״̬��Ӧ��
	"SLP",               #define ST_SLEEP       0
	"INIT", //׼������   #define ST_INIT        1
	"RUN",               #define ST_RUNNING     2
	"BOOT",              #define ST_WAIT_BOOT   3
	"DOWN",              #define ST_WAIT_DOWN   4
	"KILL",              #define ST_KILLED      5
	"ABRT",              #define ST_ABORT       6
	"Down"               #define ST_DESTROY     7
*/
#ifndef _Channel_H_
#define _Channel_H_
#include "CommandCom.h"
#include "StdAfx.h"
//#include "MBC_jt.h"
#include "MBC.h"
#include "ThreeLogGroup.h"

#define CHANNEL_EMPTY 0			//��
#define CHANNEL_NOMAL 1			//����
#define CHANNEL_LOW   2			//��ˮ
#define CHANNEL_HIGH  3			//��ˮ
#define CHANNEL_FULL  4			//��

#define MAX_STATE_LEN 30		//ϵͳ״̬����

class Channel
{
	public:
		Channel();
		~Channel();
	public:
		static int connectGuardMain();
		static int getProcTask(int iProcID);
		static int calcWater(int iVpID);
		static char* trim(char* sp);
	public:
		static CommandCom* m_pCmdCom;												//�ͷ���ͨѶ
		static THeadInfo *m_pInfoHead;											//GuadMain�׵�ַ
		static TProcessInfo *m_pProcInfoHead;								//������Ϣ�׵�ַ
		static char m_sSysRunState[MAX_STATE_LEN];							//ϵͳ����״̬��ȫ�֣�
		static ThreeLogGroup* m_pLog;                         //��־�澯
};
#endif
