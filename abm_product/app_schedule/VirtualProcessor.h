
#ifndef _VIRTUAL_PROCESSOR_H_
#define _VIRTUAL_PROCESSOR_H_

#include <iostream>
#include "StdAfx.h"
using namespace std;


#define START											0			//������
#define STOP											1			//ͣ����
#define NOACTION_STATE						2			//ˮλ״̬���䣬��������
#define NOACTION_TIME							3			//ʱ�䷧ֵδ������������
#define NOACTION_NOMAL						4			//����ˮλ����������
#define NOACTION_NEXT							5			//�¸�ͨ��Ҳ�Ǹ�ˮ����������

//�鴦����������ϵͳ״̬�¶�Ӧ��״̬
#define MODULE_STOP								0			//ֹͣ					0��Ϊͣ����������
#define MODULE_RUNING							1			//��������ҵ����  1��Ϊ�� ����������
#define MODULE_MAINTAINANCE 			2			//ά��

#define MODULE_ABORT							3			//�쳣
#define MODULE_FULL								4			//����
#define MODULE_IDLE								5			//���У���ҵ����
#define MODULE_PROC_RUN_ABORT					6			//�н����쳣,ͬʱ�н�������
#define MODULE_PROC_STOP_ABORT					7			//�н����쳣,�޽���������

#define MAX_OWN_APP_NUM						100		//һ���鴦���������Ľ�����

class Channel;						//ͨ��

//���̽ṹ
struct ProcessInfo{
	int iPos;								//��GuardMain�е�����λ��
	int iOpFlag;						//�������ĺۼ� 0δ������ 1������
	int iDestState;					//Ŀ��״̬
	int iCheckCnt;					//����Ŀ��̬���ȷ�ϴ���
	ProcessInfo(){
		iPos = -1;
		iOpFlag = 0;
		iDestState = -1;
		iCheckCnt = 0;
	}
};
//�鴦�����ṹ
class VirtualProcessor
{
	public:
		VirtualProcessor(int ID);
		~VirtualProcessor();	
	public:
		int getVirtualState(int& state);										//��ȡ�鴦������ǰ״̬
		int getAllProcDetailNum(int& boot,int& init,int& run,int& kill,int& destory,int& abort,int& sleep); //��ȡ����״̬�½��̸���
		int refreshEx();																		//���¶�ȡ����
		int getProcInfoEx();
		int getVirtualActionEx();														//��ȡ�账����Ӧ���Ķ���
		int isDataSrc();																		//�жϸ��鴦�����Ƿ�������Դ
		int getProcInfoFromGuardMain();											//��ȡ�鴦���µĽ���
		int startOneProcEx();																//����ĳ�����̣������ɹ�ʧ���ж�
		int stopOneProcEx();																//ֹͣĳ�����̣������ɹ�ʧ�ܵ��ж�
		int checkProcState();																//����鴦�����½���״̬�Ƿ�ﵽĿ��̬
		int getOneBusyProcEx();															// ��ȡһ���������еĽ��̣�����ֹͣ
		int getOneIdleProcEx();															// ��ȡһ���Ѿ���ֹ�Ľ��̣���������
		int checkVirtualState();														//����鴦��������Ӧ�ߵ�ˮ���Ͳ������
		int getBusyProcNumEx();															//����ST_INIT ST_WAIT_BOOT ST_RUNNING�Ľ��̸�����ͬʱ��ST_INIT ST_WAIT_BOOT�Ľ��̴��ϴ������
		int startMinProcEx();																//������С������
		int firstCheckProc();																//��һ�μ�⣬�����Ҫ���ֱ����Ҫͣ�����������Դͷ����ֱ��ͣ�����ϱ�ʶ�������������
		int stopAllSrcProc();																//ֱ��ͣ����Դͷ
		int stopLeftProc();																	//��ʣ�µĽ��̸�ͣ��ͣ��
		int checkTask();																		//����鴦����ͨ����ˮλ����m_iCanStop���
		int forceStopProc();																//ǿ��ֹͣ�鴦�����н��̣������ˮλ��״̬
		int isErrExist();																		//�鿴�鴦�������Ƿ��н���״̬������
		int isBusy();																				//�鿴�Ƿ񳬸���
		int isBackUpSrc();																	//�Ƿ��Ǳ���̬Դ
		int checkBackUpTask();															//����̬ʱ������ˮλ���⴦��
	public:
		int m_iVirtualId;																		//�鴦����ID
		int m_iVirtualState;																//�鴦�����ĵ�ǰ״̬
		int m_iDestState;																		//��ǰϵͳ״̬���鴦������Ŀ��״̬
		int m_iCheckFlag;																		//�����ж���û�б�Ҫ���
		int m_iCanStop;																			//���̶�����ͣ��
		int m_iSrcStopCheckFlag;														//���鴦�����Ƿ�������Դͷ
		int m_iForceStop;																		//ǿɱ�ı�־
		int m_iBusyFlag;																		//���鴦�����ĳ����ɱ��
		int m_iAutoSchedule;																//�����Զ����ȿ��أ������ֵΪ0�򲻻����ߵ�ˮ����
		int m_iBackUpSrc;																		//����̬Դ
	private:
		int m_iMinProc; 																		//���ٽ�����
		int m_iMaxProc;																			//��������
		int m_iMaxShiftProc;																//��λ��Ӧ����������
		int m_iDurTimeOut;																			//״̬����ʱ�䷧ֵ
		//Date m_dLastTime;																		//�ϴ�״̬ʱ��
		time_t m_iLastTime;																	//�ϴ�״̬ʱ��
		int m_iChLastState;																	//�鴦������Ӧͨ������һ�ε�ˮλ״̬
		int m_iCnt;																					//�鴦�����½��̵ĸ���
		int m_iAbort;																				//abort̬�����Ƿ������Ŀ���
		int m_iShift;																				//ϵͳ��λ
		int m_iIfDelete;                                    //������������Сʱ���Ƿ���ݱ仯ɾ��������
		int m_iIfInsert;                                    //����С�������������ʱ���Ƿ���ݱ仯���ӽ�����
		
		int m_iSysMaxProcCnt;																	//ϵͳ����������̸���
		int m_iBusyTimeOut;																			//ϵͳ������ʱ�䷧ֵ
		ProcessInfo m_ProcInfoList[MAX_OWN_APP_NUM];				//�鴦����ӵ�еĽ���
	public:
		static int g_RunProcNum;														//��ǰ�Ѿ������е�ȫ�ֽ��̸���
};
#endif
