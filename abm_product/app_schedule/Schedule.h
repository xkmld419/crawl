
#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include "StdAfx.h"
#include "MBC.h"
//#include "MBC_jt.h"
#include "SysParamDefine.h"



class Schedule
{
	public:
		
		Schedule();
		
		~Schedule();
		
	public:

		int isTimeOut();															//��ʼ��һ�ֵ���

		int refresh();																//ˢ������

		int modifySysState(char* state);						//�޸�ϵͳ״̬SYSTEM.state 
		
		int modifySysRunState(char* state);						//����޸�ϵͳ����״̬
		
		int checkSysRunStateEx();											//ϵͳ����״̬���
				
		int registerToInitEx();												//��initserver����ע��

		int getCurVirProcessor();											//��ȡϵͳ��ǰ״̬�µ��鴦����
		
		int testwhile();
		
		int runEx();																	//ѭ������

		int getSysConfigEx();													//��ȡϵͳ����
				
		char* parserState();													//���ݴ��������м�̬������Ŀ��̬
		//char* parserState(char* sMidState,char* sState);
		
		int doDynamicScheduleEx();										//���ߵ�ˮ
		
		int stateTransitionEx();											//��Ӧ״̬��Ǩ
		
		int checkPoint();															//��Ӧcheckpoint
		
		int modifyCheckPointState(int iState);									//�޸�checkpoint��״̬
		
		int doBackUp();																//��Ӧ����̬
		
		int moveBackData();														//�ѱ���̬����ʱ������Ų����
		
		bool GetMemInfo(); //�õ��ڴ���Ϣ
		
		bool GetCpuInfo(); //�õ�cpu��Ϣ
		
	private:
		
		int m_iWaitTime;															//����ʱ��
		
		int m_iShift;																	//��λ��Ϣ
		
		int m_iAutoCtlFlag;														//ϵͳ�Ƿ����øߵ�ˮ�Զ�����
		
		char m_sSysState[15];													//ϵͳ״̬
		
		char m_sMidSysState[50];											//ϵͳ�м�̬ 0|ONLINE|OFFLINE
				
		int m_iStartTime;															//����ֹͣ��ʱ��ֵ
		
		vector<VirtualProcessor*> m_vVirtual;					//��״̬���漰���鴦����
		
		int m_iCpuHighValue;													//CPU����
		int m_iMemHighValue;													//MEM����
		int m_iStateTimeOut;													//ϵͳ״̬��Ǩ��ʱ�ж�
		int m_iCheckPointState;												//checkPoint״̬
		int m_iMemPercent;                            //�ڴ�ʹ����
		int m_iCpuPercent;                            //cpuʹ����
		
		
	public:
		
		char m_sRunState[30];													//SYSTEM.run_state
		
		vector<SysParamHis> m_vtSysParamHis;				//ԤԼ�ĸ���
};
#endif
