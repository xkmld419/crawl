#include <unistd.h>
#include <iostream>
#include "AbmProcess.h"

using namespace std;

bool PrintUsage();
int main(int argc,char ** argv)
{
	char opt;
	bool bRun=false,bStop=false;
	int iProcessId=0,iBillFlowID=0;
	//int bIsLogin;
	char sAppName[24];
	//bIsLogin=-1;
	memset(sAppName,0,sizeof(sAppName));
	ABMProcess *pABMP=new ABMProcess();	
	if(argc<4||argc>4)
	{
		PrintUsage();
		return -1;
	}
	
	if((strcmp(argv[1],"-o")==0)&&(strcmp(argv[2],"-p")==0))
	{
		iProcessId=atoi(argv[3]);
		if(pABMP->RunProcess(iProcessId)!=0)
		{
			cout<<"��������ʧ��"<<endl;
		}
		return 0;
	}
	
	
	if((strcmp(argv[1],"-c")==0)&&(strcmp(argv[2],"-p")==0))
	{
		iProcessId=atoi(argv[3]);
		if(pABMP->StopProcess(iProcessId)!=0)
		{
			cout<<"����ֹͣʧ��"<<endl;
		}
		return 0;
	}
	
	
	if((strcmp(argv[1],"-a")==0)&&(strcmp(argv[2],"-p")==0))
	{
		iProcessId=atoi(argv[3]);
		if(pABMP->ShowProcess(iProcessId)!=0)
		{
			cout<<"������Ϣ����ʧ��"<<endl;
		}
		return 0;
	}
	
	
	if((strcmp(argv[1],"-a")==0)&&(strcmp(argv[2],"-m")==0))
	{
		strcpy(sAppName,argv[3]);
		if(pABMP->ShowProcessInfoByApp(sAppName)!=0)
		{
			cout<<"��ģ�������Ϣʧ��"<<endl;
		}
		return 0;
	}
	
	
	PrintUsage();
	delete pABMP;
	pABMP=NULL;
	return 0;
		
}


bool PrintUsage()
{
		printf("\n *******************************************************************************************   \n") ;
		cout <<"usage: " <<"abmprocess"<<" [-a|-o|-c]| [-p|-m] \n";
		cout<<"abmprocess"<<" -a	��ʾ��Ϣ��������ID���������ơ�����/�߳�״̬��CPUռ�á��ڴ�ռ�á�IO��Ϣ��"<<'\n';
		cout<<"abmprocess"<<" -o ��������"<<'\n';
		cout<<"abmprocess"<<" -c ֹͣ����"<<'\n';
		cout<<"abmprocess"<<" -m ���ģ������,����ģ����ʾ������Ϣ"<<'\n';
		
		cout<<"abmprocess"<<" [-o|-c] -p �������ID������ָֻ�Դ˽���ID����"<<'\n';
		cout<<"abmprocess"<<" [-a] [-p|-m]  �������ID����ģ�����ƣ���ʾ������ϸ��Ϣ"<<'\n';
		
		printf("\n *******************************************************************************************   \n") ;
		return true;
}
