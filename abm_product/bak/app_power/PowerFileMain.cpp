#include "PowerFile.h"
#include "PowerXml.h"

int main()
{	
	PowerFile a;	

	Log::log(0,"��ʼ�����ļ���ÿ15����ɨ��һ��...");
	while (1){
		a.getFile();
		a.setUserInfo();
		a.MainProcess();	
		a.sendFile();
		Date d;
		//Log::log(0,"ϵͳ�����У���ǰʱ��:%s",d.toString("yyyy-mm-dd hh:mi:ss"));
		sleep(15*1);//ÿ15���Ӵ���һ��
		//sleep(15*60);//ÿ15���Ӵ���һ��
		//exit(0);//-----------------------------------------
	}
	return 0;
}
