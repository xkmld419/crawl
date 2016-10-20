#include "Hbinstalltest.h"
#include "MessageQueue.h"
#include "StdEvent.h"
#include "msgqueue.h"
#include "IpcKey.h"

//#include <error.h>
#include <iostream>
using namespace std;

int main()
{	
	char setorder[500] = {0};
	char runorder[500]= {0};
	char downorder[500]= {0};
	char billset[500]= {0};
	char billrun[500]= {0};
	char billdown[500]= {0};
	
	MessageQueue mq3(13);
	MessageQueue mq4(14);
	MessageQueue mq5(15);
	
	Log::log(0,"------����ʵ��------");
	Log::log(0,"------�ͻ�IDΪ     : 10001\n");
	Log::log(0,"------���к���     : 18922353301\n");
	Log::log(0,"------ͨ��ʱ��     : 30000����\n");
	Log::log(0,"------������ƷΪ   : ��Ʒ����\n");
	Log::log(0,"------��������     : ÿ����2��\n");
	Log::log(0,"------Ԥ�Ʋ������� : 600000 \n");
	Log::log(0,"------�ƷѰ�װ��֤��ʼ------");
	Hbinstalltest *Hbtest = new Hbinstalltest();
	
	if(Hbtest->Init() !=0) return -1;
	//ȡ��ϵͳ��������
	if(Hbtest->getenviroment() !=0) return -1;
	//д���Ի���
	Log::log(0,"------���Ի�������");
	if(Hbtest->writeTestList())
		Log::log(0,"------д���Ի����ɹ�!\n\n");
	else 
		Log::log(0,"------ʧ�� !\n\n");
	
	//��������������ϺͶ�����Ϣ
	
	Log::log(0,"------�������ϺͶ�����Ϣ����");
	if(Hbtest->createInfo()){
		Log::log(0,"------�����û���Ϣ���سɹ�!\n\n");
	}
	else
		{
			Log::log(0,"------�����û���Ϣ����ʧ��!\n\n");
			return -1;
		}
	
	
	//���̼���ر�����
	Log::log(0,"------�������̼���ر�����");
	if(Hbtest->billflowCfg())
		Log::log(0,"------�����������óɹ�!\n\n");
	else
	{
		Log::log(0,"------������������ʧ��!\n\n");
		return -1;
	}
	
		
	//���ݵ�ǰ������Ϣ�������ݿ��
	Hbtest->checkTableByInfo();

	//ϵͳ��������(��gardmain,�趨����,�����ۼ���,�ؽ�����,�û���Ϣ���ص�)
	Log::log(0,"------��������");
	Log::log(0,"------��������ʼ...\n");
	
	strncpy(billset,Hbtest->SET,sizeof(billset));
	strcpy(setorder,"sh ");
	strcat(setorder,billset);
	int ire = system(setorder);
	
	
	sleep(2);

	//������Ҫ����Ϣ����
  mq3.open (true, true, sizeof (StdEvent), 20);
  mq4.open (true, true, sizeof (StdEvent), 20);
  mq5.open (true, true, sizeof (StdEvent), 20);
  
  Log::log(0,"------������Ҫ����Ϣ����");
  sleep (10);
  
  bool bRunRest=true;
	//����˳������
	//if(Hbtest->beginRunPro(GATHER_PROCESS) < 0) bRunRest=false;
	if(Hbtest->beginRunPro(PREP_PROCESS) < 0) bRunRest=false;
	if(Hbtest->beginRunPro(FORMAT_PROCESS) < 0) bRunRest=false;
	if(Hbtest->beginRunPro(DUP_PROCESS) < 0) bRunRest=false;
	if(Hbtest->beginRunPro(PRICING_PROCESS) < 0) bRunRest=false;
	if(Hbtest->beginRunPro(WRITEFILEDB_PROCESS) < 0) bRunRest=false;
	if(Hbtest->beginRunPro(FILEINSTORE_PROCESS) < 0) bRunRest=false;
		
	if(bRunRest)
	{
		sleep (10);
		if(/*Hbtest->processState(GATHER_PROCESS) == 0
		&&*/ Hbtest->processState(PREP_PROCESS) == 0
		&& Hbtest->processState(FORMAT_PROCESS) == 0
		&& Hbtest->processState(DUP_PROCESS) == 0
		&& Hbtest->processState(PRICING_PROCESS) == 0
		&& Hbtest->processState(WRITEFILEDB_PROCESS) == 0
		&& Hbtest->processState(FILEINSTORE_PROCESS) == 0)
	  {
			Log::log(0,"���е�������������!\n");
			
		}
		else//ֹͣ���н���
		{
			Hbtest->stopPro(GATHER_PROCESS);
			Hbtest->stopPro(PREP_PROCESS);
			Hbtest->stopPro(FORMAT_PROCESS);
			Hbtest->stopPro(DUP_PROCESS);
			Hbtest->stopPro(PRICING_PROCESS);
			Hbtest->stopPro(WRITEFILEDB_PROCESS);
			Hbtest->stopPro(FILEINSTORE_PROCESS);
			
			//ɱ��Ϣ����
      mq3.remove ();
      mq4.remove ();
      mq5.remove ();
      
      return -1;
			
		}
		
	}
	else//�н����������������д���
	{
		Hbtest->stopPro(GATHER_PROCESS);
		Hbtest->stopPro(PREP_PROCESS);
		Hbtest->stopPro(FORMAT_PROCESS);
		Hbtest->stopPro(DUP_PROCESS);
		Hbtest->stopPro(PRICING_PROCESS);
		Hbtest->stopPro(WRITEFILEDB_PROCESS);
		Hbtest->stopPro(FILEINSTORE_PROCESS);
		
		//ɱ��Ϣ����
    mq3.remove ();
    mq4.remove ();
    mq5.remove ();
    
    return -1;
	}
	
	Log::log(0,"���Ե� 20 �� ���ڼ�������Ƿ�����!!!\n");
	sleep (20);
	
	if(Hbtest->printTestinfo())
		Log::log(0,"------���̲��Գɹ�!\n");
	else
		Log::log(0,"------δ�ܳɹ����,���̲���ʧ��,����LOG��־!\n");
		
		
  Hbtest->stopPro(GATHER_PROCESS);
	Hbtest->stopPro(PREP_PROCESS);
	Hbtest->stopPro(FORMAT_PROCESS);
	Hbtest->stopPro(DUP_PROCESS);
	Hbtest->stopPro(PRICING_PROCESS);
	Hbtest->stopPro(WRITEFILEDB_PROCESS);
	Hbtest->stopPro(FILEINSTORE_PROCESS);
	
	//ɱ��Ϣ����
	Log::log(0,"------ɾ����Ϣ����!!!\n");
	
  mq3.remove ();
  mq4.remove ();
  mq5.remove ();
	
	if(Hbtest != NULL)
		delete Hbtest;
	return 0;
}
