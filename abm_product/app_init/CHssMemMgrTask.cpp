#include "CHssMemMgrTask.h"
#include "SimpleSHM.h"
#include "Exception.h"

//##ModelId=4C0311EF02CB
CHbMemMgrTask::CHbMemMgrTask()
{
}


//##ModelId=4C0311EF02F9
CHbMemMgrTask::~CHbMemMgrTask()
{
}

//##ModelId=4C0311F603B9
bool CHbMemMgrTask::getTaskResult(CmdMsg* pMsg,long& lMsgID,variant& vtRet,int* piRetSize)
{
//Log::log(0,"ĪҪ����ĪҪ�������������,�����������Ϊ %s",typeid(this).name());
	if(0== pMsg)
		return false;

	long lCmd = 	pMsg->GetMsgCmd();
	string strUrl = pMsg->GetMsgSrc();
	if(strUrl.empty())
	{
		Log::log(0,"û��ָ���ڴ��ֵ");
		vtRet =MBC_Server+1;
		return false;
	}
	
	variant vt;
	pMsg->GetMsgContent(vt);


	long lKey = atol(strUrl.c_str());
	if(MSG_MEM_CREATE == lCmd)
	{
		//���������ڴ�
		long lSize= vt.asLong();
		return CreateShm(lKey,lSize);
	}else
	{
		if(MSG_MEM_DESTORY == lCmd)
		{
			//�����ڴ�
			return DestoryShm(lKey);
		}
	}
	return true;
}

//##ModelId=4C03121403D7
bool CHbMemMgrTask::Init()
{
	return CHbTaskBase::Init();
}

bool CHbMemMgrTask::DestoryShm(long lKey)
{
	SimpleSHM * pShmTmp = new SimpleSHM(lKey,0);
	try{
		pShmTmp->remove();
	}catch( Exception& e)
	{
		Log::log(0,"�����ڴ�����ʧ��");
		m_vtRet = MBC_Server+1;
		return false;
	}
	return true;
}
bool CHbMemMgrTask::CreateShm(long lKey,unsigned long size)
{
	//���������ڴ�
	SimpleSHM * pShmTmp = new SimpleSHM(lKey,size);
	try
	{
		//�򿪹����ڴ�
		pShmTmp->open(true);
		delete pShmTmp; pShmTmp=0;
	}catch( Exception& e)
	{
		Log::log(0,"�����ڴ洴��ʧ��");
		m_vtRet = MBC_Server+1;
		return false;
	}
	return true;
}
