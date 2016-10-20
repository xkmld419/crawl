#ifndef	_HB_CHECKPOINT_H_
#define	_HB_CHECKPOINT_H_

//#include "CheckPointMgr.h"
//#include "CheckPointInfo.h"
#include "ProcessControler.h"

class hbCheckPoint
{
public:
	hbCheckPoint();
	~hbCheckPoint();
	bool Init(void);

	int CreateChkPoint(void);
	//����Ƿ��������ڴ�
	bool CheckCanDownShm(void);
	//hss
	bool CheckCanDownShm(string strAppName,int time=30);
	bool StartProc(string strAppName);
	
//ֹͣ���е�����Դ
	bool StopAllProc(const string&,int iSec=0);

	bool GetAllProc(const string& strAppName,vector<int>&);
	
	bool StartAllProc(void);

	bool CheckAllMsg(int iSec=0);

private:
	HBProcessControl* m_pProcessControler;

	vector<int>m_vecProcID;

};



#endif
