#ifndef CMDPARSERPOOLMGR_H_HEADER_INCLUDED_B40C5E9A
#define CMDPARSERPOOLMGR_H_HEADER_INCLUDED_B40C5E9A

#include "HssTask.h"
#include "CmdReceiver.h"
#include "CmdSender.h"
#include "CmdParserProc.h"
#include "Log.h"
#include "CmdMsg.h"
#include "InDependProcess.h"
#include "TOCIQuery.h"
#include "Environment.h"
#include "IpcKey.h"
#include "CSemaphore.h"
#include <vector>

using namespace std;

//CmdParser��Ϣ������̳�
//##ModelId=4BF3C2300237
class CmdParserPoolMgr
{
  public:
    //##ModelId=4BF3CABD0020
    CmdParserPoolMgr();

    //##ModelId=4BF3CAC5034E
    virtual ~CmdParserPoolMgr();
    //��ʼ�����̳�
    //##ModelId=4BF3CF49003E
    bool Init(void );

	void addDefaultMsg(CmdMsg* pMsg);

    //�������ύ�����̳�ִ��
    //##ModelId=4BF3CFA9032C
    void addTask(CmdMsg* pMsg);
    //##ModelId=4BF3D3D10290
    unsigned int const& getMaxProcessCnt() const;


    //##ModelId=4BF3D3D102ED
    void setMaxProcessCnt(unsigned int left);

  
    //##ModelId=4BF3D3D103C2
    unsigned int const& getMixProcessCnt() const;


    //##ModelId=4BF3D3D2001A
    void setMixProcessCnt(unsigned int left);
    //##ModelId=4BF5F26A0322
    void setMsgSender(
        //��Ϣ���������ڽ���Ϣ���͸�ִ�н���
		CmdSender* pMsgSender);


    //##ModelId=4BF5F2A801D2
    void setMsgRecverCmdParser(
        //��Ϣ������,�û�����init�ķ�����Ϣ
        CmdReceiver* pMsgRecver);

	void setMsgSenderCmdParser(
		//��Ϣ�����������ӽ���ִ����������initserver�㱨���
		CmdSender* pMsgSender);


//public:
//	static CSemaphore* getPoolSem(void);

  private:
    //����һ������
    //##ModelId=4BF3D045033F
    void addProcess();

    //##ModelId=4BF3D0550312
    //����һ������
    //##ModelId=4BF3D0550312
    bool subProcess();

    //##ModelId=4BF3D30103D0
    vector<pid_t> m_vectorPid;
    //##ModelId=4BF3CEE9026A
    unsigned int m_iMaxProcessCnt;
    //##ModelId=4BF3CEFA0229
    unsigned int m_iMixProcessCnt;
    //##ModelId=4BF5F3E90308
    CmdSender* m_pMsgSender;
    
    //##ModelId=4BF5F3FE0226
    CmdReceiver* m_pMsgRecverCmdParser;

	CmdSender* m_pMsgSenderCmdParser;

	CSemaphore* m_pSemaphore;

	vector<CmdMsg* > m_vctDefaultMsg;
};



#endif /* CMDPARSERPOOLMGR_H_HEADER_INCLUDED_B40C5E9A */
