/*VER: 3*/ 
#include "LoadUserInfoMgr.h"
#include "Process.h"
#include "CommonMacro.h"

int LoadUserInfoMgr::thread_num = 0;
bool LoadUserInfoMgr::bAcct     = false;
int LoadUserInfoMgr::thread_exception_flag = 0;
bool LoadUserInfoMgr::bForce    = false;

void* runThread(void* param)
{
	try{
		UserInfoAdmin* thr = (UserInfoAdmin*)param;
		thr->run();
	}catch(...){
		LoadUserInfoMgr::thread_exception_flag = -1;
		//Log::log(0,"�����߳��ӳ��쳣!ֱ�ӷ���");
		cout<<"�����쳣���߳��˳�,�ȴ������˳�!"<<endl;
		pthread_exit((void *)"�߳��˳�!");
	}
	return NULL;
}

void* runThreadDB(void* param)
{
	try{
		UserInfoAdmin* thr = (UserInfoAdmin*)param;
		thr->runDB();
	}catch(...){
		LoadUserInfoMgr::thread_exception_flag = -1;
		//Log::log(0,"�����߳��ӳ��쳣!ֱ�ӷ���");
		cout<<"�����쳣���߳��˳�,�ȴ������˳�!"<<endl;
		pthread_exit((void *)"�߳��˳�!");
	}
	return NULL;
}

void* runThreadMEM(void* param)
{
	try{
		UserInfoAdmin* thr = (UserInfoAdmin*)param;
		thr->runMEM();
	}catch(...){
		LoadUserInfoMgr::thread_exception_flag = -1;
		//Log::log(0,"�����߳��ӳ��쳣!ֱ�ӷ���");
		cout<<"�����쳣���߳��˳�,�ȴ������˳�!"<<endl;
		pthread_exit((void *)"�߳��˳�!");
	}
	return NULL;
}

LoadUserInfoMgr* LoadUserInfoMgr::uniqueInstance = NULL;

//##ModelId=44D5FB610356
LoadUserInfoMgr* LoadUserInfoMgr::getInstance()
{
	if ( uniqueInstance == NULL)
		uniqueInstance = new LoadUserInfoMgr();
	return uniqueInstance;
}

//##ModelId=44D5FB61035F
LoadUserInfoMgr::LoadUserInfoMgr()
{
	threadManger=LoadThreadManager::getInstance();
    
    #ifdef SHM_FILE_USERINFO
    m_pSHMFile = new SHMFileCtl();
    #endif
}

LoadUserInfoMgr::~LoadUserInfoMgr()
{
	delete threadManger;
    
    #ifdef SHM_FILE_USERINFO
    if(m_pSHMFile){
        delete m_pSHMFile;
        m_pSHMFile = 0;
    }
    #endif
}

void LoadUserInfoMgr::freeThread()
{
	threadManger->free();
	return;
}

void LoadUserInfoMgr::loadAll()
{
	try{
		UserInfoAdmin admin(bForce);
        
        #ifdef SHM_FILE_USERINFO
        if(m_pSHMFile->m_poStateData->m_iState[CHANGE_POINT_STATE] != FILE_POINT_STATE){
            Log::log(0,"ERR:���л�Ϊ�ļ�����ģʽ!");
            return;
        }
        #endif
        
		admin.load();
	}catch(...){
		cout<<"���м����������Ϸ����쳣�������˳�!"<<endl;
		throw;
	}
	return;
}

void LoadUserInfoMgr::loadTable(const int iMode)
{
	try{
    #ifdef SHM_FILE_USERINFO
    if(m_pSHMFile->m_poStateData->m_iState[CHANGE_POINT_STATE] != FILE_POINT_STATE){
        Log::log(0,"ERR:���л�Ϊ�ļ�����ģʽ!");
        return;
    }
    #endif
    
	UserInfoAdmin admin(bForce);
	admin.unloadTable(iMode);
	if(admin.cleanOffset(iMode))
	{
		admin.loadTable(iMode);
		admin.bindDataEx(iMode);
	}
	//������¼����ʻ��������¼�����Ʒʵ�������Ϣ
	if(iMode == ACCT)
	{
		admin.unloadTable(PRODUCT_OFFER_INFO);
		if(admin.cleanOffset(PRODUCT_OFFER_INFO))
		{
			admin.loadTable(PRODUCT_OFFER_INFO);
			admin.bindDataEx(PRODUCT_OFFER_INFO);
		}
	}
	thread_num = 0;
	}catch(...){
		cout<<"����������ϳ����쳣�������˳�!"<<endl;
		throw;
	}
	return;
}

void LoadUserInfoMgr::loadParallel()
{
	int iCount = 0;
	Log::log(0,"���м��ؿ�ʼ");
    
    #ifdef SHM_FILE_USERINFO
    if(m_pSHMFile->m_poStateData->m_iState[CHANGE_POINT_STATE] != FILE_POINT_STATE){
        Log::log(0,"ERR:���л�Ϊ�ļ�����ģʽ!");
        return;
    }
    #endif
    
	UserInfoAdmin * admin = new UserInfoAdmin(bForce);
	try{
		admin->unload();
	}
	catch(...){
		//Log::log(0,"ж���û����Ϲ����ڴ�����쳣�������˳�!");
		cout<<"ж���û����Ϲ����ڴ�����쳣�������˳�"<<endl;
		delete admin;
		admin = 0;
		throw;
	}
	try{
		admin->setRunInfo(-1,"CUST_AND_SERV");
		admin->loadPrefix();iCount++;
	}catch(...){
		//Log::log(0,"����CUST_AND_SERV���ϳ����쳣�������˳�!");
		cout<<"����CUST_AND_SERV���ϳ����쳣�������˳�!"<<endl;
		delete admin;
		admin = 0;
		throw;
	}
	thread_num++;
	this->createThreadRun(A_SERV_TYPE_CHANGE,"A_SERV_TYPE_CHANGE",admin);iCount++;
	this->createThreadRun(SERV_LOCATION,"SERV_LOCATION",admin);iCount++;
	this->createThreadRun(SERV_STATE_ATTR,"SERV_STATE_ATTR",admin);iCount++;
	this->createThreadRun(SERV_ATTR,"SERV_ATTR",admin);iCount++;
	this->createThreadRun(SERV_IDEN,"SERV_IDENTIFICATION",admin);iCount++;
	this->createThreadRun(SERV_PRODUCT_ANDATTR,"SERV_PRODUCT_ANDATTR",admin);iCount++;
	this->createThreadRun(SERV_ACCT,"SERV_ACCT",admin);iCount++;
	this->createThreadRun(ACCT,"ACCT",admin);iCount++;
		//�������� add by xn 2009.10.26
	this->createThreadRun(B_NP,"B_NP",admin);iCount++;
#ifdef	GROUP_CONTROL
	this->createThreadRun(A_ACC_NBR_OFFER_TYPE,"A_ACC_NBR_OFFER_TYPE",admin);iCount++;
#endif
	//������Ʒʵ��ʱ��Ҫ�ȴ�acct�����
	while(!bAcct)
	{
		if(thread_exception_flag == -1)
		{
			delete admin;
			admin = 0;
			throw 1;
		}
		Log::log(0,"����߳��� %d ��",thread_num);
		sleep(5);
	}
	this->createThreadRun(PRODUCT_OFFER_INFO,"PRODUCT_OFFER_INSINFO",admin);iCount++;
	bAcct = false;

	while(1)
	{
		if(thread_exception_flag == -1)
		{
			delete admin;
			admin = 0;
			throw 1;
		}
		if(thread_num == iCount)
		{
			admin->bindDataEx(ALL_INFO);
			Log::log(0,"����߳��� %d ��",thread_num);
			Log::log(0,"���м������");
			break;
		}
		Log::log(0,"����߳��� %d ��",thread_num);
		//this->freeThread();
		sleep(5);
	}
	delete admin;
	admin = 0;
	return;
}

void LoadUserInfoMgr::loadSuper()
{
	int iCount = 0;
	Log::log(0,"���м��ؿ�ʼ");
    
    #ifdef SHM_FILE_USERINFO
    if(m_pSHMFile->m_poStateData->m_iState[CHANGE_POINT_STATE] != FILE_POINT_STATE){
        Log::log(0,"ERR:���л�Ϊ�ļ�����ģʽ!");
        return;
    }
    #endif
    
	UserInfoAdmin * admin = new UserInfoAdmin(bForce);
	try{
		admin->unload();
	}
	catch(...){
		cout<<"ж���û����Ϲ����ڴ�����쳣�������˳�"<<endl;
		delete admin;
		admin = 0;
		throw 1;
	}

	try{
		admin->setRunInfo(-1,"CUST_AND_SERV");
		admin->loadPrefix();iCount++;
	}catch(...){
		cout<<"����CUST_AND_SERV���ϳ����쳣�������˳�!"<<endl;
		delete admin;
		admin = 0;
		throw 1;
	}
	thread_num++;
	this->createThreadRun(ACCT,"ACCT",admin);iCount++;
	this->createThreadRun(SERV_PRODUCT,"SERV_PRODUCT",admin);iCount++;
	this->createThreadRun(SERV_PRODUCT_ATTR,"SERV_PRODUCT_ATTR(DB)",admin,1);iCount++;
	this->createThreadRun(SERV_STATE_ATTR,"SERV_STATE_ATTR",admin);iCount++;
	this->createThreadRun(A_SERV_TYPE_CHANGE,"A_SERV_TYPE_CHANGE",admin);iCount++;
	this->createThreadRun(SERV_LOCATION,"SERV_LOCATION",admin);iCount++;
	this->createThreadRun(SERV_ATTR,"SERV_ATTR",admin);iCount++;
	this->createThreadRun(SERV_IDEN,"SERV_IDENTIFICATION",admin);iCount++;
	this->createThreadRun(SERV_ACCT,"SERV_ACCT",admin);iCount++;
	this->createThreadRun(OFFER_INSTANCE,"PRODUCT_OFFER_INSTANCE",admin);iCount++;
	this->createThreadRun(INSTANCE_DETAIL,"PRODUCT_OFFER_INSTANCE_DETAIL(DB)",admin,1);iCount++;
	#ifdef  GROUP_CONTROL
	this->createThreadRun(A_ACC_NBR_OFFER_TYPE,"A_ACC_NBR_OFFER_TYPE",admin);iCount++;
	#endif
	this->createThreadRun(SERV_PRODUCT_ATTR,"SERV_PRODUCT_ATTR(MEM)",admin,2);iCount++;
	this->createThreadRun(INSTANCE_DETAIL,"PRODUCT_OFFER_INSTANCE_DETAIL(MEM)",admin,2);iCount++;
		//�������� add by xn 2009.10.26
	this->createThreadRun(B_NP,"B_NP",admin) ;iCount++;
	
	while(1)
	{
		if(thread_exception_flag == -1)
		{
			delete admin;
			admin = 0;
			throw 1;
		}
		if(thread_num == iCount)
		{
			admin->bindDataEx(ALL_INFO);
			Log::log(0,"����߳��� %d ��",thread_num);
			Log::log(0,"���м������");
			break;
		}
		Log::log(0,"����߳��� %d ��",thread_num);
		sleep(10);
	}
	delete admin;
	admin = 0;
	return;
}

void LoadUserInfoMgr::unLoad()
{
	try{
        #ifdef SHM_FILE_USERINFO
        if(m_pSHMFile->m_poStateData->m_iState[CHANGE_POINT_STATE] != FILE_POINT_STATE){
            Log::log(0,"ERR:���л�Ϊ�ļ�����ģʽ!");
            return;
        }
        #endif
        
		UserInfoAdmin admin(bForce);
		admin.unload();
	}catch(...){
		cout<<"ж���û����Ϲ����ڴ�����쳣�������˳�"<<endl;
		throw;
	}
	return;
}

void LoadUserInfoMgr::createThreadRun(int iMode,char *message,UserInfoAdmin * admin,int iFlag)
{
	/*UserInfoAdmin * pThread = new UserInfoAdmin();
	pThread->setRunInfo(iMode,message);
	threadManger->add(pThread);*/
	if(thread_exception_flag == 0)
	{
		int iRet;
		admin->setRunInfo(iMode,message);
		pthread_t iThreadID;
		switch(iFlag)
		{
		case 0:
			iRet = pthread_create(&iThreadID,NULL,&runThread, (void*)admin);
			break;
		case 1:
			iRet = pthread_create(&iThreadID,NULL,&runThreadDB, (void*)admin);
			break;
		case 2:
			iRet = pthread_create(&iThreadID,NULL,&runThreadMEM, (void*)admin);
			break;
		default:
			break;
		}		
		if(iRet !=0)
		{
			Log::log(0,"�����߳� %s ʧ�ܣ������˳�!",message);
			delete admin;
			admin = 0;
			throw 1;
		}
		sleep(2);
		return;
	}
	return;
}
