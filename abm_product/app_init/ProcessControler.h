#ifndef	_PROCESS_CONTROL_H_
#define	_PROCESS_CONTROL_H_

//ͨ��guardmain���Ƴ�����ͣ
#include "mntapi.h"
#include <vector>
#include <string>
#include <pthread.h>
using namespace std;
bool CheckIsActive(void*); 

bool CheckIsStop(void* pProcInfoTmp);
class ManagedProcessInfo
{
public:
	string	strAppName;
	string	strAppPath;
	string	strParam;
	int		iManagedType;		//0,GuardMain�й�,1,ֱ������
	int		iProcID;		
	int		iDepProcID;
	void*	pParamData;
	ManagedProcessInfo(){ iManagedType=iProcID=iDepProcID=0;pParamData=0;} ;
	bool operator == (
		const ManagedProcessInfo &oProcessInfo);
};

//���̿������ӿ�
class ProcessControl
{
public:
	ProcessControl(){};
	virtual ~ProcessControl(){};
	virtual int stopProcess(int iProcID, int iWaitSecs=0, 
		bool bCheckMQ=false)=0;
	virtual int getMsgNum(int iProcID)=0;
	virtual	int startProcess(int iProcID)=0;
	virtual bool	GetAllProc(vector<ManagedProcessInfo>& vecProcess)=0;
	virtual bool	FindSleepProcID(int& iProcID)=0;
};
//�����ڴ�ӿ�
class ShmProcessControl:
	public ProcessControl
{
public:
	ShmProcessControl(){};
	~ShmProcessControl(){};
	int stopProcess(int iProcID,int iWaitSecs=0,bool bCheckMQ=false);
	int getMsgNum(int iProcID);
	int startProcess(int iProcID);
	bool Init(void);
	bool GetAllProc(vector<ManagedProcessInfo>& vecProcess);
	bool	FindSleepProcID(int& iProcID);
private:
	bool AttachShm(void);
};

//�����Ŀ��������õĶ��� ProcessControl�Ľӿ�
class HBProcessControl;
class HBProcessControl
{
public:
	~HBProcessControl(){};
	static HBProcessControl* GetInstance();
	bool SwitchControl(ProcessControl* pProcessCtrl){
		m_pProcessCtl = pProcessCtrl;
		MntAPI::m_pProcInfoHead==0;
		MntAPI::m_poInfoHead==0;
	};
	//ע����mntapi.h
	int  stopProcess (int iProcID, int iWaitSecs=0, 
		bool bCheckMQ=false);
	int  getMsgNumber (int iProcID);
	int   startProcess (int iProcID);
	bool GetAllProc(vector<ManagedProcessInfo>& vecProcess);
	//��ȡȱʡ�Ľ��̣�����GuardMain
	bool GetDefaultProc(vector<ManagedProcessInfo>& vecProcess);

	//��һ�����е�ProcID
	bool FindSleepProcID(int& iProcID);
	static vector<ManagedProcessInfo>* m_gpvecProcesss;

private:

	HBProcessControl(){};
	static HBProcessControl* m_gpProcessControl;
	//��ʼ��
	bool Init(void);
	static pthread_mutex_t g_mutexInit;

	ProcessControl* m_pProcessCtl;
};


#endif
