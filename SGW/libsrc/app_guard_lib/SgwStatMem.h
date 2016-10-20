#ifndef SGW_STAT_MEM_H
#define SGW_STAT_MEM_H


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
//#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <pthread.h>
#include <sys/shm.h>

#include "ProcessInfo.h"
#include "CSemaphore.h"

#include "CommandCom.h"

#define MAX_STAT_EXTNET_INFO 2048		//�ⲿ��Ԫ����
#define MAX_STAT_NETINFO_COUNT 2048		//��Ԫ�������
#define MAX_STAT_SERVICE_COUNT 512		//ҵ������,���ȼ�
#define MAX_STAT_PROCESS_INFO 1024		//���̴������
#define MAX_STAT_BLOCK_INFO 1024		//ӵ�����
#define MAX_STAT_INFOPOINT_INFO 1024	//ӵ�����

enum E_SHOWFLAG
{
	ON_TODAY = 1,
	ON_ONLINE = 2,
	ON_HOUR =3
};

//�ⲿ��Ԫ
struct S_ExtNetInfo{
	long lExtNetInfoID;			//�ⲿ��Ԫ��ʾ
	char sNetInfoName[100];		//��Ԫ��
	long lUserID;				//�û���ʾ
	char sUserName[30];			//�û���
	//long lServiceID;			//ҵ��������ʾ
	char sServiceName[128];		//ҵ��������
	long lToatl;				//��������
	long lSuccNum;				//�ɹ�������
	S_ExtNetInfo(){
		lExtNetInfoID = 0;
		memset(sUserName,0,30);
		memset(sServiceName,0,30);
		lExtNetInfoID = 0;
		lUserID = 0;
	}
};

struct S_ExtNetInfo_Buff{
	int iTotal;
	int iDay;
	int iHour;
	struct S_ExtNetInfo aExtNetInfo_[MAX_STAT_EXTNET_INFO];
};

//��Ԫ�������
struct S_NetInfoCount{
	long lNetInfoID;			//��Ԫ��ʾ
	char sNetInfoName[100];		//��Ԫ��
	//long lServiceID;			//ҵ��������ʾ
	char sServiceName[128];		//ҵ��������
	long lReqNum;				//������
	int lReqSuccNum;			//����ɹ���
	long lReqData;				//������(k)
	long lCallServiceNum;		//�������ô���
	long lCallServiceSucc;		//���óɹ���
	S_NetInfoCount(){
		memset(sNetInfoName,0,30);
		memset(sServiceName,0,30);
		lNetInfoID = 0;
		lReqNum = 0;
		lReqSuccNum = 0;
		lReqData = 0;
		lCallServiceNum = 0;
		lCallServiceSucc = 0;		
	}
};

struct S_NetInfoCount_Buff{
	int iTotal;
	int iDay;
	int iHour;
	struct S_NetInfoCount aNetInfoCount_[MAX_STAT_NETINFO_COUNT];
};

//ҵ���������ȼ�
struct S_ServiceCount{
	//long lServiceID;			//ҵ��������ʾ
	char sServiceName[128];		//ҵ��������
	int iPLevel;				//���ȼ�
	long lTotal;				//��������
	long lSuccNum;				//�ɹ�������
};

struct S_ServiceCount_Buff{
	int iTotal;
	int iDay;
	int iHour;
	struct S_ServiceCount aServiceCount_[MAX_STAT_SERVICE_COUNT];
};


//���̴������
struct S_ProcessInfo{
	int iProcessID;//����ID
	long iAllTickets;
	long iNormalTickets;
	long iBlackTickets;
	long iErrTickets;
	long iOtherTickets;

};

struct S_ProcessInfo_Buff{
	int iTotal;
	int iDay;
	int iHour;
	struct S_ProcessInfo aProcessInfo_[MAX_STAT_PROCESS_INFO];
};



//ͳ����Ϣ
struct S_AllCount{
	long lReqNum;		//1)	���������
	long lTimeOutNum;	//2)	����ʱ����(������²��ظ�Ӧ����Ϣ)��
	long lSuccNum;		//3)	Ӧ��ɹ�����������ҵ��ɹ������ҵ����ʧ�ܵ��������
	long lFaultNum;		//4)	Ӧ��ʧ�ܴ���������SGW����Ļظ�������SR·��ʧ�ܵĴ�������
	long lAuthNum;		//5)	������Ȩʧ�ܴ�����
	long lCodeFaultNum;	//6)	Э��ת��ʧ�ܴ�����
	long lTranNum;		//7)	��Ϣת��ʧ�ܴ�����
	long lMarkCheckNum;	//8)	ǩ����֤ʧ�ܴ�����
	long lSRFaultNum;	//9)	SR·��ʧ�ܴ���
	long lOtherNum;//��������
};

struct S_AllCount_Buff{
	int iDay;
	int iHour;
	struct S_AllCount aAllCount_;
};

//ӵ�����
struct S_BlockInfo{
	//long lServiceID;			//ҵ��������ʾ
	char sServiceName[128];		//ҵ��������
	int iPLevel;				//���ȼ�
	long lInNum;				//����Ϣ
	long lOutNum;				//����Ϣ
	long lDiscardNum;			//������Ϣ
	
};

struct S_BlockInfo_Buff{
	int iTotal;
	struct S_BlockInfo aBlockInfo_[MAX_STAT_BLOCK_INFO];
};

//��Ϣ��
//��Ϣ��1
//index [m_szServiceContextId,m_szOriginHost,m_szProtocolType,m_iResultCode,
//	m_iSuccessFlag,m_iReqDir,m_szProtocolType,m_iMsgType]
struct S_InfoPoint
{
	int	m_iMsgType;			// ��Ϣ����
	int	m_iSuccessFlag;			// �Ƿ�ɹ�
	int	m_iReqDir;			// ����
	long m_lReqCount;			// ��Ϣ�� �ۼ�
	long m_lReqBytes;			// ��Ϣ��С Byte �ۼ�
	int	m_iResultCode;			// ������
	char m_szKey[100];			// ����||��Ϣ���ͣ�1|0��
	char m_szServiceContextId[100];		// ��������
	char m_szOriginHost[100];		// ���� OriginHost
	char m_szProtocolType[10];		// Э�����ͣ�SOAP|SCSP|TUXEDO|DCC��
};
struct S_InfoPoint_Buff
{
	int iTotal;
	struct S_InfoPoint aInfoPoint_[MAX_STAT_INFOPOINT_INFO];
};

//��Ϣ��2 top 10
//index��m_szService_context_id,m_szMajor_avp_value,m_szProtocolType��
struct S_InfoPointTop
{
	long m_lCount;				// ��Ϣ�� �ۼ�
	char m_szService_context_id[100];	// ��������
	char m_szMajor_avp_value[100];		// �ؼ��ֶ�ֵ
	char m_szKey[100];			// ����||��Ϣ���ͣ�1|0��
	char m_szProtocolType[10];		// Э�����ͣ�SOAP|SCSP|TUXEDO|DCC��
};

struct S_InfoPointTop_Buff
{
	int iTotal;
	struct S_InfoPointTop aInfoPointTop_[MAX_STAT_INFOPOINT_INFO];
};


struct S_StatAllInfo
{
	//���ؾ������
	int iBalancePer;

	//ӵ�����
	int iBlockLevel;		//ӵ������
	char sBlockTime[16];	//ӵ������ʱ��
	int iRateCfg;			//���õ��ļ�����
	int iRate;				//�ļ�����


	//ͳ����Ϣ
	struct S_AllCount_Buff aAllCount[3];

	//����
	struct S_ProcessInfo_Buff aProcessInfo[3];
	
	//ҵ���������ȼ�
	struct S_ServiceCount_Buff aServiceCount[3];

	//��Ԫ����
	struct S_NetInfoCount_Buff aNetInfoCount[3];

	//�ⲿ��Ԫ
	struct S_ExtNetInfo_Buff aExtNetInfo[3];

	//ӵ�����
	struct S_BlockInfo_Buff aBlockInfo;

	//��Ϣ��
	struct S_InfoPoint_Buff aInfoPoint;
	struct S_InfoPointTop_Buff aInfoPointTop;
};



class StatAllInfoMgr
{
	public:
		StatAllInfoMgr();
		~StatAllInfoMgr();
	public:
		static struct S_StatAllInfo *pStatAllInfo;
		static TProcessInfo *pProcInfoHead;
		static THeadInfo *pInfoHead;
		static bool AttachStatShm();
		static bool DetachStatShm();
		static bool GetStatMemKey();
		static bool GetStatLock();
		static bool CreateStatShm();
		static bool DestroyStatShm();
		static bool ClearStatShm();
		static bool ConnectGuardMain();
		static void GetDayAndHour(char* sDatetime,int& iDay,int& iHour);
		
	private:
		static long m_lKey;
		static long m_lLockKey;
		static CSemaphore *pSem;
		
		static CommandCom* m_pCmdCom;
	public:
		//���º����漰�������������붼Ϊ����

		//��Ԫ��������ӿ�
		static bool addNetInfoCount(long lNetInfoID,char *sNetInfoName, char *sServiceName,int iReqNumAdd/*����*/,
			bool bReqSucc, int iReqDataAdd/*����*/,int iCallServiceAdd,bool bCallSucc,char* sDatetime = NULL );

		//�ⲿ��Ԫ����ӿ�
		static bool addExtNetInfo(long lExtNetInfoID,char* sNetInfoName,long lUserID, char* sUserName, 
			char* sServiceName,int iNum,bool bSucc,char* sDatetime= NULL );

		//ҵ�����ȼ�
		static bool addServiceCount(char *sServiceName,int iPLevel,int iNum,bool bSucc,char* sDatetime= NULL);

		//ͳ����Ϣ
		static bool addAllInfo(int iReqNum, int iTimeOutNum,int iSuccNum,int iFaultNum,
			int iAuthNum,int iCodeFaultNum,int iTranNum,int iMarkCheckNum,int iSRFaultNum,
			int iOtherNum,char* sDatetime= NULL);
		
		//����ͳ�ƣ�����ά��ÿ�����̴����������á�
		static bool addTicket(int iProcessID,int iType/*0:Normal;1:Error*/,int iNum,char* sDatetime= NULL);

		//ӵ������
		static bool setBlockLevel(int iBlockLevel,int iRateCfg,char* sBlockTime);

		//ӵ��ʱ�ļ�����
		static bool setBlockRate(int iRate);

		//ӵ��ͳ��
		static bool addBlockInfo( char* sServiceName,int iPLevel, int lInNum, int lOutNum,int lDiscardNum,
			char* sDatetime= NULL);

		//���ø��ؾ������
		static bool setBalancePer(int iPer);

		//��Ϣ��
		static bool addInfoPoint(struct S_InfoPoint *pInfoPoint);
		static int getInfoPoint(struct S_InfoPoint *pInfoPoint,int iBuffLen);
		static bool delInfoPoint();

		static bool addInfoPointTop(struct S_InfoPointTop *pInfoPointTop);
		static int getInfoPointTop(struct S_InfoPointTop *pInfoPointTop,int iBuffLen);
		static bool delInfoPointTop();
		
};


#endif

