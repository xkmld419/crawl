#include "Hbcleantest.h"

int main()
{
	Hbcleantest *Htest = new Hbcleantest();
	//ȡ��ϵͳ��������
	if (Htest->getenviroment() != 0)
	{
		return -1;
	}
	
	if(Htest->Init() != 0) 
		return -1;
	//���/
	Log::log(0,"--��ֹͣ�Ʒ�����ģ��");
	sleep (2);
	
	Log::log(0,"--������Ϣ�������");
	sleep (2);
	
	Log::log(0,"--�м����������");
	sleep (2);
		
	if(Htest->cleanInfo())
		Log::log(0,"---------���ݿ��������");
			
  sleep (2);
	Log::log(0,"--���������������");
	
	//������Ի���
	//if(Htest->cleanTestList())
		//Log::log(0,"--���Ի����������");
	
	//����Ʒѹ����м����ݺʹ���������
	//if(Htest->cleanTempInfo())
		//Log::log(0,"--�м������������");
	
	//if(Htest->cleanBillflowcfg())
		//Log::log(0,"--���������������");
	delete Htest;
	return 0;
}
