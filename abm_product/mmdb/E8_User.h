/*VER: 1*/ 
#ifndef _E8_USER_H
#define _E8_USER_H

//#include "UserInfo.h"
#include "UserInfoCtl.h"
#include "DataEventMem.h"
#include <vector>

#define E8_USER_INFO_NUM	"E8_USER"
#define E8_EXP_TABLE		"E8_USER_TO_TABLE"
#define DATA_BUFFER			1
#define INDEX_BUFFER		2
#define COMMIT_BUFFER		2000	

using namespace std;

class E8UserCtl:public DataEventMemCtl
{
public:	
	E8UserCtl(int iGroup);
	~E8UserCtl();
	void create();
	void loadE8User();     //�����ڴ�
	void unLoadE8User();   //ж���ڴ�	
	//void checkMemUse();    //�鿴ʹ����	
	//void alarmMemUser();   //ʹ�����澯		
	bool updateE8UserInfo(DataE8AccountInfo *pData,char *sAction);	
	bool getBillingCycleOffsetTime(int iOffset,char * strDate);
#ifdef  USERINFO_REUSE_MODE
	void  revokeE8User();//��Data_user�ѹ��ڵ�����ռ���ڴ��ͷ�
#endif		

private:	
	void linkUserName(DataE8AccountInfo &pData,int iPos);		
};


class E8UserMgr : public DataEventMemBase
{
public:
	E8UserMgr();	
	E8UserMgr(int iGroup);
	~E8UserMgr();
	//�����ʺŻ�ȡ��Ϣ
	bool getE8User(DataE8AccountInfo &E8Info,char *e8Account,char *sAreaCode,char *sStartTime,char cGetMode='N');
	void checkMemUse();    //�鿴ʹ����	
	void dispUserInfo(char *sUserName);
	void dispAll();
	void alarmMemUser(char *sRcvAlarmAccNbr);   //ʹ�����澯
	void cmpMem_TabUserInfo(char *sEndDay,char *sRcvAlarmAccNbr);
	void expUserInfo(char *sEndDay,char *sExpTabName);
	void inTableCommit(char *sEndDay,char *sExpTabName);
private:
	//iMemMode �� 1��������ʹ�������2��������ʹ�����
	int  percentOfMemUsed(int iMemMode);
	//void insMonitorInterface(char *sInfo,char *sRcvAlarmAccNbr);		
	vector<DataE8AccountInfo> vE8UserList;
	vector<int> vIntList;
};


#endif
