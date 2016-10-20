#include "StatFormBase.h"
#include "StatBaseMgr.h"

#include "OnlineTime.h"
//#include "ProcessRuning.h"
//#include "MsgQueue.h"
//#include "ProcessSes.h"
#include "LoggInfo.h"
#include "UserLogin.h"
//#include "AllStatInfo.h"
//#include "SHMInfoShow.h"
//#include "SgwStatMem.h"

#include "ThreeLogGroup.h"

StatBaseMgr::StatBaseMgr()
{
	pDraw = NULL;
}

StatBaseMgr::~StatBaseMgr()
{
	if(pDraw != NULL)
		delete pDraw;
}
int StatBaseMgr::run()
{
	string sErrMsg;
	string sAgrv;
	try{
		if(g_argc < 2){
			PrintUsage();
			return -1;
		}
		pDraw = NULL;
		for(int i=1; i<g_argc; i++)
		{
			if(g_argv[i][0] != '-'){
				PrintUsage();
				return -1;
			}
			sAgrv = g_argv[i];
			if( sAgrv == "-a" )
			{
				PrintUsage();
				return 0;
			}
			else if(sAgrv == "-s")
			{
				pDraw = new OnlineTime();
			}
			else if(sAgrv == "-r")
			{
				//pDraw = new ProcessRuning();
			}
			else if(sAgrv == "-Q")
			{
				//pDraw = new MsgQueue();
			}
			else if(sAgrv == "-ses")
			{
				//pDraw = new ProcessSes();
			}
			else if(sAgrv == "-seg")
			{
				//pDraw = new SHMInfoShow();
			}
			else if(sAgrv == "-L")
			{
				pDraw = new LoggInfo();
			}
			else if(sAgrv == "-u")
			{
				pDraw = new UserLogin();
			}
			/*
			else if(sAgrv == "-p")//����
			{
				StatFormBase::SetShowFlag(ON_TODAY);
				pDraw = new AllStatInfo();
			}
			else if(sAgrv == "-P")//��������
			{
				StatFormBase::SetShowFlag(ON_ONLINE);
				pDraw = new AllStatInfo();
			}
			else if(sAgrv == "-o")//��ǰСʱ
			{
				StatFormBase::SetShowFlag(ON_HOUR);
				pDraw = new AllStatInfo();
			}
			*/
			else if(sAgrv == "-t")
			{
				i++;
				if(i<=g_argc)
					StatFormBase::SetInterval( atoi(g_argv[i]) );
			}
			else if(sAgrv == "-m")
			{
				i++;
				if(i<=g_argc)
				{
					sAgrv = g_argv[i];
					StatFormBase::SetMoudle( (char*)sAgrv.c_str() );
				}
			}
			else
			{
				printf("��֧�ֵĲ���ѡ���ο�sgwstat -a\n\n");
				return -1;
			}
		}
		if(!pDraw)
		{
			//printf("��֧�ֵĲ���ѡ���ο�sgwstat -h\n\n");
			PrintUsage();
			return -1;
		}
		//pDraw->StartWin();
		pDraw->Build();
		pDraw->EndWin();
	}
	catch(string ex){
		sErrMsg.append("string error:").append(ex);
	}
	catch(TOCIException &ex){
		sErrMsg.append("TOCIException error:").append(ex.getErrMsg()).append(ex.getErrSrc());
	}
	catch(TException &ex){
		sErrMsg.append("TException error:").append(ex.GetErrMsg());
	}
	catch(exception &ex){
		sErrMsg.append("exception error:").append(ex.what());
	}
	catch(...){
		sErrMsg.append("unkown error");
	}
	if(!sErrMsg.empty())
	{
		printf("��������:%s,�����˳�!\n", sErrMsg.c_str());
		return -1;
	}
	return 0;
}


void StatBaseMgr::PrintUsage()
{
	printf("\nsgwstat useage:\n\n" 
"		-a		�г�����ѡ�� \n"
"		-s		�г���ǰϵͳ��level0״̬��SGW-ONLINE\n"
"				���ϴ��������������˶೤ʱ�䣨��ʽ��***��**ʱ**��**�룩   \n"
"		-t		ÿ������ظ�ѡ��                                          \n"
"		-p		��ʾ�����ͳ����Ϣ���������ٰ�����                        \n"
"				��������������ɹ��������������������ô��������óɹ�����  \n"
"		-P		��ʾ�����������ۼƵ�ͳ����Ϣ���������ٰ���������������    \n"
"				����ɹ��������������������ô��������óɹ�����            \n"
"		-o		��ʾ�Ե�ǰСʱ���������ۼƵ�ͳ����Ϣ���������ٰ�����      \n"
"				��������������ɹ��������������������ô��������óɹ�����  \n"
"		-r		��ʾϵͳʵʱ���������������ٰ�������ѹ������              \n"
"				��ǰ��������ÿ��ʱ�䵥λ�����ܣ���                        \n"
"		-m		[module]�����̷߳�������                                  \n"
"		-Q		��ʾSGWϵͳ��Ϣ���е�ʹ�����                             \n"
"		-L		��ʾ��־�ļ���ʹ�����                                    \n"
"		-u		��ʾ�����賿��Ŀǰϵͳ�û���¼���                        \n"
"		-ses		��ʾϵͳ����/�߳����������ͬsgwprocess �Ca                \n"
"		-seg		��ʾ��ǰ�ڴ��ʹ��״��������ͬsgwshm �Cstat                \n"
"		-ca		����Stat�ڴ湩ͳ��ҵ�������Ϣʹ�ã����봴��         \n"
"		-de		ɾ��stat�ڴ�                                          \n");
}


DEFINE_MAIN(StatBaseMgr);

