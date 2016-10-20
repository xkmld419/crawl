#ifndef	_HB_INIT_
#define	_HB_INIT_

#include "ProcessEx.h"
#include "MsgDefineServer.h"

#define			PARAM_MODE_SHIFT		"-r"
#define			PARAM_MODE_TEST			"-t"
#define			PARAM_MODE_EFFDATE		"-b"
#define			PARAM_MODE_EXPDATE		"-e"

#define	DEFAULT_ENV_PATH		"/opt/opthss"


class hbInit:
	public ProcessEx
{
public:
	hbInit(void);
	~hbInit(void);
	int run();
	bool Init(void);
	//�����������
	bool PrepareParam(void);
	enum _RUN_MODE_
    {
		MODE_UNKNOW =0,
		MODE_START	=1,	//����initserver
		MODE_SHIFT		=2, //��offline�л���online
		MODE_TESTMSG =3, //������Ϣ
   		MODE_MAIN=4,
		MODE_MULTPARAM=5,
		MODE_RESTART=6,
		MODE_DOWN=7
		 
	};  
	enum _SERVICE_STATE_
	{
		STATE_ONLINE=0,
		STATE_OFFLINE=1,
		STATE_MAIN=2,
		STATE_UNKOWN=3
	};
private:
	//���Է����Ƿ����
	bool TestServerActiveSignal(int iTimes=60);
	//��������
	bool StartService();
	//�л�״̬
	bool ShiftServerMode(_SERVICE_STATE_ ServiceState);

	//ֹͣ����
	bool StopServer(void);

	//��黷��
	bool CheckAll(bool bConfirm=false);
	bool CheckCfgFile(void);
	bool CheckNowSysteState(void);
	bool GetNowSystemState(string& strState);
	
	bool GetInputYesOrNo(void);
	static void OnSignal(int,siginfo_t*,void*);
	static struct sigaction m_gsigaction;
	static int m_gServerState;
private:
	_RUN_MODE_ m_eMode;
	bool	m_bChecked;	//�Ƿ���Ҫ����ȷ��
	bool	m_bDisplay;	//�Ƿ���ʾ�������
	string m_strTIBSHOME_Path;
	string m_strBeginDate;
	string m_strEndDate;
	string m_strShiftState;
	int m_iOtherType;
	int m_iRep;
	int iabc;


};

#endif
