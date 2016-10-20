#ifndef	_INITSERVER_H_
#define	_INITSERVER_H_
#include "BaseProcess.h"
#include "interrupt.h"
#include <vector>
#include <map>
#include <sstream>
#include "MBC.h"
#include "IniFileMgr.h"
#include "Date.h"
#include "CDBAccessIF.h"
#include <fstream>

using namespace std;

#define	MAX_INI_KEY_VALUE_LEN		256
#define	MEMORY_SECTION			"MEMORY"
#define	MEMORY_PARAM_NAME	"param.ipc_key_value"
#define	MEMORY_INIT_SIZE	"param.init_data_size"
#define	MEMORY_LOCK_KEY		"param.sem_key_value"

#define	LOG_SECTION					"LOG"
#define	LOG_PATH							"log_path"
#define	LOG_ALARM_LEVEL			"log_alarm_level"
#define	LOG_ALARMDB_LEVEL	"log_alarmdb_level"
#define	MONITOR_SECTION		"MONITOR"
#define	MONITOR_RESTART_TIMES	"process_restart_times"
#define	MSGRECONVER_TIMES	300


#define	PROCESS_SECTION			"PROCESS"
#define	PROCESS_START_NUM	"process_start_num"
#define	PROCESS_NAME				"process_start_name"
#define	PROCESS_PARAM			"process_start_param"
#define	PROCESS_RUN_ONCE		"process_run_once"
#define	PROCESS_TYPE			"process_start_type"
#define	PARAM_BILLINGCONFIG	"SGWCONFIG"
using namespace std;

//##ModelId=4BFE1CA8031A
class CInitServer :
	public BaseProcess 
{
public:
    //##ModelId=4BFE1CA9007E
    CInitServer(void);
    //##ModelId=4BFE1CA90081
	~CInitServer(void);

    //##ModelId=4BFE1CA90087
	int run();
private:

	//����ʱִ��
	//##ModelId=4BFE1CA900DF
    void OnIdle(void);

public:

	enum SERVICE_STATE { SERVICE_ACTIVE=1,
		SERVICE_SLEEP,
		SERVICE_STOP,
		SERVICE_ERROR,
		SERVICE_ERROR_PARAM,
		SERVICE_ERROR_MEMORY,
		SERVICE_PROCESS_RUN,
		SERVICE_PROCESS_MEMMGR,
		SERVICE_PROCESS_SCHEDULE,
		SERVICE_PROCESS_MONITOR};
private:

	void ModifyTimingParameters(void);


	//��ʼ��
	//##ModelId=4BFE1CA900F5
    bool Init(void);

	//��������
	bool ServiceStart(void);

	//װ�ؽ��̶�Ӧ����Ϣ����
	//##ModelId=4BFE1CA900FA
    bool LoadProcessMQ(void);

	void ReplaceIniKernelParam(stIniCompKey& oIniCompKey);
	bool CheckProcessIsDown(pid_t ipid,int iSecs=10);
	CSemaphore* m_pSem;


	string m_strTIBSHOME_Path;
	string m_strTIBSHOME_CfgFile;
	string m_strTIBSHOME_CfgStdFile;
	string m_strMsgLogFilePath;

	//���Ĳ����ڴ� ��ֵ,����ֵ,�ڴ��С
	string m_strInitShmName;
	unsigned long m_lInitShmSize;

	int m_iServerState;
private:
	map<time_t,int>m_mapPidTimes;
	//�л�����״̬
	inline void SetServiceState(SERVICE_STATE STATE);
	//��ȡIni�ļ����ڴ���Ϣ
	bool GetSHMInfo(void);
	bool GetLogInfo(void);

	pid_t m_iPidBoss;
	//�����˳����
	bool ServiceExit(pid_t iPid);
	void SetBossPid(pid_t iPid){ m_iPidBoss =iPid;};//�ϰ����ID����ͣ
	//У�����,1��У�������ļ�����
	void CheckParam(vector<stIniCompKey>& vtCompKey,
		vector<stIniCompKey>& vtBadCompKey,
		vector<stIniCompKey>& vtRtCompKey);

	SERVICE_STATE m_State;
	union sigval m_unsigval;
	//m_unsigval.sival_int=m_State;//1�ɹ���2ʧ��
	CDBAccessIF* m_pDBAccessIF;
	vector<ProcessExecInfo>m_strStartProcess;
	int	m_iProcessNum;
	bool GetStartProcess(void);
	bool StartProcess(ProcessExecInfo& oProcessExecInfo);

	void MonitorProcess(void);
	int m_iRestartTimes;
	int m_iPid;
	int m_iRunCnt;
	int m_iSleepTimes;
public:
	//ThreeLogGroupMgr * m_poLoggMgr;	
	ThreeLogGroup *m_pLogg;
};

#endif

