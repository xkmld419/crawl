#ifndef CHBPROCESSMGRTASK_H_HEADER_INCLUDED_B3F9B544
#define CHBPROCESSMGRTASK_H_HEADER_INCLUDED_B3F9B544
#include "HssTask.h"

typedef struct _PROC_TASK_
{
	string strProcName;
	string strProcExecPath;
	unsigned int uiProcID;
}ProcTask;

class CmdMsg;

//���������Ϣ,ͣ����,�����,��ȡ������Ϣ
//##ModelId=4C0616EF028F
class CHbProcessMgrTask : public CHbTaskBase
{
  public:
    //##ModelId=4C06172C0394
    CHbProcessMgrTask();

    //##ModelId=4C06172C03E6
    virtual ~CHbProcessMgrTask();
    //##ModelId=4C0617C0012F
    bool Init();


    //##ModelId=4C0617B2025C
    bool getTaskResult(CmdMsg *vt, long& lMsgID,variant& vtRet,int* iRetSize=0);


  private:
    //ָֹͣ������
    //##ModelId=4C061A4602E0
    bool killProcess( int ipid);
    //��������
    //##ModelId=4C061A5A00FD
    bool startProcess(ProcessExecInfo* pProcInfo,  int& iProcID);

};



#endif /* CHBPROCESSMGRTASK_H_HEADER_INCLUDED_B3F9B544 */
