#ifndef _PETRI_H_
#define _PETRI_H_
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
//#include <wait.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "p_MQ.h"
#include "p_ParseBuff.h"
#include "P_messageSend.h"

#include "TOCIQuery.h"
//#include "WfPublic.h"
#include "Server.h"
#include "IpcKey.h"
#include "Date.h"

#ifdef HB_28
#include "ThreeLogGroup.h"
#include "Log.h"
#endif


#define PLACENUM 32
#define STATE_LENGTH 30
#define NAME_LENGTH 128
#define MSG_LENGTH 1024*4



#define _KILLED -999
#define _APPNULL -666

#define REG_SIGNAL signal(16,gotKilled)
	

namespace PETRI {


inline void LOG(char *s)
{
	Date now;
	char execdate[15];
	now.getTimeString(execdate);
	execdate[14]=0;
	printf("[%s] %s,%d line:%s\n",execdate,__FILE__,__LINE__,s);
}


void gotKilled(int sig); 


 
#define TIME_OUT 500

struct cmd_msg {
	long mtype;//��Ϣ����
	char mbody[MSG_LENGTH];
};

struct _P_MQ {

#define P_MQ _P_MQ::instance()
#define P_CMDMSG P_MQ.cmdMQ.getMsg()
#define P_ACKMSG P_MQ.ackMQ.getMsg()
	static inline _P_MQ& instance() {
		static _P_MQ pmq;
		return pmq;
	};

	MQ<cmd_msg> cmdMQ;
	MQ<cmd_msg> ackMQ;

protected:
	_P_MQ() {
		LOG("init MQ\n");
		cmdMQ.setKey(0x0010);
		ackMQ.setKey(0x0011);
	};

};

struct _P_BUFF {
#define P_BUFF _P_BUFF::instance()
#define P_SENDBUFF P_BUFF.sendBuff
#define P_RECVBUFF P_BUFF.recvBuff
	static inline _P_BUFF& instance() {
		static _P_BUFF pbuff;
		return pbuff;
	};

	P_ParseBuff recvBuff;
	P_ParseBuff sendBuff;

protected:
	_P_BUFF(){};

};



struct _P_DB{
#define P_DB _P_DB::instance()
#define DEFINE_PETRI_QUERY(x) TOCIQuery x (P_DB.getDB())
#define DEFINE_PETRI_QUERY_SETCONN(x,u,p,c) TOCIQuery x (P_DB.getDB(true,u,p,c))
//#define DEFINE_PETRI_QUERY_CHILD(x) TOCIQuery x (P_DB.getDBchild())
	static inline _P_DB & instance() {
		static _P_DB pdb;
		return pdb;
	};


	void getConnectInfo();
	void setConnectInfo(char *puser,char *ppasswd,char *pconstr);
	TOCIDatabase* getDB(bool reLogin = false, char *puser =0, char *ppasswd = 0 ,char *pconstr = 0);
//	TOCIDatabase* getDBchild(bool reLogin = false);


protected:
	_P_DB(){
		b_Connect = false;
//		b_Connectchild = false;
		s_user[0]=0;
		s_passwd[0]=0;
		s_constr[0]=0;
	};


private:
	bool b_Connect;
//	bool b_Connectchild;
  char s_user[24];
  char s_passwd[24];
  char s_constr[32];
  TOCIDatabase db;
//  TOCIDatabase dbchild;
};
	
/*
struct cmd_msg {
	long mtype;//��Ϣ����
	long req_id; //�����ʶ 
	long tran_id; //��Ǩ��ʶ
	long staff_id; //���ű�ʶ
	bool needAck; // �Ƿ���ҪӦ�� 
	int ret; //ִ�н��
	char msg[128]; //ִ����Ϣ
};
*/


struct _P_MNT {

#define P_MNT _P_MNT::instance()
	static inline _P_MNT& instance() {
		static _P_MNT mnt;
		return mnt;
	};

	TSHMCMA* getShmTable();
	void checkLogin();
	TSHMCMA *p_shmTable;
	int		i_index;
	int		i_authID;
	char	s_hostAddr[15];
	string s_orgID;

	
	void GetLogInfo(bool reLogin = false);
	void SaveLogInfo();
	bool login();
	bool logout();


protected:
	_P_MNT() {
		p_shmTable = NULL;
		i_index = -1;
		i_authID = -1;
//		GetLogInfo();
		s_hostAddr[0]=0;
		s_orgID = "1";
		if (i_index == -1 || i_authID == -1) {
			login();
//			SaveLogInfo();
		}
	};

	//GetShmAddress


private:
	//bool KeepAlive();

};

/*
struct P_PARSEBUFF {

	static P_PARSEBUFF& instance() {
		static P_PARSEBUFF pb;
		return pb;
	};
protected:

	P_PARSEBUFF() {
		i_transactionID = -1;
		i_transitionID = -1;
		memset(s_srcState,0,sizeof(s_srcState));
		memset(s_descState,0,sizeof(s_descState));
		memset(s_msg,0,sizeof(s_msg));

	};

private:
	TParseBuff paser;
	int		i_transactionID;
	long	i_transitionID;
	char	s_srcState[3];
	char	s_descState[3];
	char	s_msg[1024];

};
*/

/*
struct thread {

	thread( void*(*exe)(void*),void*inArg = NULL) ;


	private:
		pthread_t thread_id;
		void* (*exeBody)(void*);
		void* ret,*arg;

};
*/


struct places {

	places() {
		l_transID = -1;
		i_plcKind = -1;
		i_size = 0;
		v_plcStatelist.clear();
		for (int i =0 ;i<PLACENUM;i++ ) {
			a_plcID[i]=0;
			a_plcType[i] = -1;
			a_plcToknum [i] = 0;
			a_plcState[i].clear();
		};


	};
	places(long transID){
		l_transID = transID;
		i_plcKind = -1;
		i_size = 0;
		v_plcStatelist.clear();
		for (int i =0 ;i<PLACENUM;i++ ) {
			a_plcID[i]=0;
			a_plcType[i] = -1;
			a_plcToknum [i] = 0;
			a_plcState[i].clear();
		};


	};
	void	setTransID(long transID);
	void	erase();
	bool	checkFire(const vector<string>& transState); //����Ƿ���ϵ������
	int lock();
	bool	getFirstPlace(long drawID);
	void	load(int i_subnet = 0);
	bool	tokenMove(string s_state = "0");
	void	getNextTrans(vector <long> & vt);
	bool	checkState(const string& in_State);
	void	setIn();
	void	setOut();
	bool	CheckSubnetEndPlcs();
	long	getParentTransID();
	string  getEndPlcsState();


private:
	
	bool	tokenOut();
	bool	tokenIn(string s_state = "0");
	void	loadInPlcs();
	void	loadOutPlcs(int i_subnet = 0);
	bool	in(const string& s1,const vector<string>& v1);

	long	l_transID;
	int		i_plcKind;//0�����������1���������
	int		i_size; //������Ŀ
	long	a_plcID[PLACENUM];
	int		a_plcType[PLACENUM];// �������ͣ����ƻ����� 
	int		a_plcToknum[PLACENUM]; //�������Ƹ���
	long	a_plcDrawID[PLACENUM]; // The DrawID the places belong to
	string a_plcState[PLACENUM]; //������ǰ״̬
	char	a_plcName[PLACENUM][NAME_LENGTH+1]; //The Name of places
	vector<string> v_plcStatelist; //����״̬�б�

};

struct exec;


struct timePlan {
	long lPlanID;
	char cPlanType;  //S:single  C:cycle
	char sBeginTime[15];
	char sEndTime[15];
	int  iWeekDay;
	long lTimeOffset;
	long lTimeInterval;
	char cIntervalUnit; //M:month D:day H:hour  T:minute
	timePlan* nextPlan;
};


struct transition {

	transition() {

		l_transID = -1;
		l_drawID = -1;
		i_conType = -1;//��Ǩ���� 0:�� 1:���
		i_execAttr = -1 ;//�Զ����ԣ�0:�Զ� 1:�˹�
		i_execType = -1; //ִ������ 0����̨���� 1��SQL 3��SHELL�ű�
		l_execObjID = -1 ; //ִ��Ŀ�꣺ process_id,sql_info_id
		i_subnetType = -1; // 0:��Ǩ 1:����
		l_subDrawID = -1;
		i_timeType = -1;//0;˲ʱ 1��ʱ�� Timed
		l_transactionID = 0;
		l_logID = -1;
		l_staffID = -1;
		l_shProcessID = -1;
		l_appTypeID = -1;
		tExec = NULL;
		tPlan = NULL;

		memset(s_transName,0,sizeof(s_transName));

		s_execResult.clear();//ִ�н��
		s_inState.clear();//����״̬
		s_execState.clear();//ִ��״̬
		v_inStateList.clear();//����״̬�б�
		v_outStateList.clear();//���״̬�б�


	};

	transition(long transID) {

		l_transID = transID;
		l_drawID = -1;
		i_conType = -1;//��Ǩ���� 0:�� 1:���
		i_execAttr = -1 ;//�Զ����ԣ�0:�Զ� 1:�˹�
		i_execType = -1; //ִ������ 0����̨���� 1��SQL 3��SHELL�ű�
		l_execObjID = -1 ; //ִ��Ŀ�꣺ process_id,sql_info_id
		i_subnetType = -1; // 0:��Ǩ 1:����
		l_subDrawID = -1;
		i_timeType = -1;//0;˲ʱ 1��ʱ�� Timed
		l_transactionID = 0;
		l_logID = -1;
		l_staffID = -1;
		l_shProcessID = -1;
		l_appTypeID = -1;
		tExec = NULL;
		tPlan = NULL;

		memset(s_transName,0,sizeof(s_transName));

		s_execResult.clear();//ִ�н��
		s_inState.clear();//����״̬
		s_execState.clear();//ִ��״̬
		v_inStateList.clear();//����״̬�б�
		v_outStateList.clear();//���״̬�б�

	};
	
	~transition(){
		delete tExec;
		delete tPlan;
	}

	bool	execute();
	bool	load();
	bool	checkExec();
	bool	checkEnd();
	bool	doChangeState(const string& state);
	void	kill();
	string	query();
	bool	checkTime();
	long	getLogID();
	int		getExecType();
	int		getSubnetType();
	long	getSubDrawID();
	long	getDrawID();
	long	getExecObjID();
	long	getTransID() {return l_transID ;}
	char* getName(){ return s_transName;}
	int		getExecAttr() {return i_execAttr;}
	string getExecState() { return s_execState;}
	string getExecResult();
	void	getInStateList(vector<string>& outList);
	void	updateTransExecLog(const string& sColumnName,const string& sColumnValue= "");
	void	updateTransExecLog(const string& sColumnName,const int iColumnValue);
	void	updateTransExecLog(const string& sColumnName,const long lColumnValue);
	long	getCurrentLogID();
	long	getLastLogID();
	long	getShPorcessID(){return l_shProcessID;};
	int 	AutoPlaceSendMessage(long transID,const string& inState,string transName);
	void   loadForMessage(message &ts,long transID);
	bool 	checkTimeforMessage(long BEGIN_DEFER);
	bool	switchState(const string& inState,const string& inMsg);//change themself and net
	bool	switchState(const string& inState,const string& inMsg,const string& inAttr);//just change themself
	void	createLog();
	void	getTransactionID();

	long	l_staffID;
	long	l_transactionID;
	long 	l_appTypeID;
	exec	*tExec;
	timePlan *tPlan;
	string s_execState;//ִ��״̬
  Date oldTime;   

private:
	
	//void lock();������¼:������������Ĺ����ԣ�����ͨ������Ǩ��¼��ʵ�ֿ����Ļ��⡣

	long	l_transID;
	long	l_drawID;
	int		i_conType;//��Ǩ���� 0:�� 1:���
	int		i_execAttr;//�Զ����ԣ�0:�Զ� 1:�˹�
	int		i_execType; //ִ������ 0����̨���� 1��SQL 3��SHELL�ű�
	long	l_execObjID; //ִ��Ŀ�꣺ process_id,sql_info_id
	int		i_subnetType; // 0:��Ǩ 1:����
	long	l_subDrawID;
	int		i_timeType;//0;˲ʱ 1��ʱ�� Timed
	long	l_logID;
	long	l_shProcessID;


	char	s_transName[NAME_LENGTH+1]; //Name of the Transition

	string s_execResult;//ִ�н��
	string s_inState;//����״̬
	vector <string> v_inStateList;//����״̬�б�
	vector <string> v_outStateList;//���״̬�б�

	//places inPlcs; //�������
	//places outPlcs; //�������
	//vector<long> v_plcs;//�����ƶ��Ŀ���
private:
	void	switchState();
	void	transLogin();
	void	nextLogID();
	bool	saveExecTime();
	bool	initExec();

	void	getParamList(vector<string>& vParamList);
	string	parseCommParam(long lParamID);

};


struct exec { //ҵ��ִ����
	virtual string doExec(const vector<string>& vParamList) =  0;//ִ�����ⲿ�ӿ�
	virtual string doKill()= 0;//
	virtual string doQuery() = 0;//
	virtual string init() = 0;//��ʼ��ִ������
	string getResult();
//	void saveErrMsg(const char* in_msg);
	//virtual void* thd_f(void*) = 0;//�߳�ִ������
	//thread* execThread;//ҵ��ִ�в��ֶ�����һ���߳�

	/*
	static void gotKilled(int sig) {
		exit(-999);
	};
	*/


	//void updateTransState(const string& cState);//�ŵ���Ǩ��
	exec(long objID) {
		l_execObjID = objID;
	};

	exec(transition* inTs) {
		ts = inTs;
		l_execObjID = ts->getExecObjID();
	};
	virtual ~exec(){};



	long l_execObjID;
	transition * ts;
	string s_execPath,s_execName,s_execParam;
		
};

struct execProcess:public exec{ //����ִ����
	execProcess(transition* inTs):exec(inTs){
		l_execObjID = ts->getExecObjID();
	};
	//execProcess(long objID):exec(objID){};

	virtual string doExec(const vector<string>& vParamList);//ִ�����ⲿ�ӿ�
	virtual string doKill();//
	virtual string doQuery();//
	virtual string init();//��ʼ��ִ������
	virtual ~execProcess(){};


};

struct execShell:public exec{ //Shellִ����
	execShell(transition* inTs):exec(inTs){l_execObjID = ts->getExecObjID();};
	execShell(long objID):exec(objID){};
	virtual string doExec(const vector<string>& vParamList);//ִ�����ⲿ�ӿ�
	virtual string doKill();//
	virtual string doQuery();//
	virtual string init();//��ʼ��ִ������
	virtual ~execShell(){};
	string sLogFile;
	string getLogfile();


};

struct execSQL:public exec { //SQL ִ����
	execSQL(transition* inTs):exec(inTs){ l_execObjID = ts->getExecObjID();};
	execSQL(long objID):exec(objID){};
	virtual string doExec( const vector<string>& vParamList);//ִ�����ⲿ�ӿ�
	virtual string doKill();//
	virtual string doQuery();//
	virtual string init();//��ʼ��ִ������
	virtual ~execSQL(){};

};


struct synTsState {
	synTsState() {
		m_lActionID = -1;
	};
	void dosynTsState();
	void dosynNetState();
	long getTsLatestID();

private:
	long m_lActionID;
};



inline void updateTransState(unsigned long lTransID)
{
		transition tmp_trans(lTransID);
		tmp_trans.l_staffID = P_RECVBUFF.getStaffID();
		tmp_trans.l_transactionID  = P_RECVBUFF.getTransactionID();
		bool bret=tmp_trans.load();
		if (!bret) return;
		places tmp_P;
		tmp_P.setTransID(lTransID);
		tmp_P.setIn();
		tmp_P.load();
		vector<string> tmp_stateList;
		tmp_trans.getInStateList(tmp_stateList);

		if (tmp_trans.checkExec() ) return; //Skip The Running Transitions

		if (tmp_P.checkFire(tmp_stateList) ) {
			tmp_trans.switchState("A","");
		} else {
			if (tmp_trans.getExecState() != "E" )
				tmp_trans.switchState("D","");
		}
};


inline void updateTransStateExecd(unsigned long lTransID)
{
		transition tmp_trans(lTransID);
		tmp_trans.l_staffID = P_RECVBUFF.getStaffID();
		tmp_trans.l_transactionID  = P_RECVBUFF.getTransactionID();
		bool bret=tmp_trans.load();
		if (!bret) return;
		places tmp_P;
		tmp_P.setTransID(lTransID);
		tmp_P.setIn();
		tmp_P.load();
		vector<string> tmp_stateList;
		tmp_trans.getInStateList(tmp_stateList);

		if (tmp_trans.checkExec() ) return; //Skip The Running Transitions

		if (tmp_P.checkFire(tmp_stateList) ) {
			tmp_trans.switchState("A","");
		}
};


};


#endif
