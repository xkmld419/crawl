#include "InstallCheck.h"
#include "Log.h"


int main(int argc, char* argv[])
{
	InstallCheck *pCheck = new InstallCheck();
	if(pCheck)
	{
		int res = pCheck->checkAllParam();
		if(res != 0){
			Log::log(0,"�к��Ĳ�������ʧ��!");
			return 1;
		} else {
			Log::log(0,"���Ĳ�������ɹ�!");
		}
		bool mem = true;
		if(argc>1 && strcmp(argv[1],"-ds")==0)
			mem = false;
		if(!pCheck->initprocess(mem)){
			Log::log(0,"�г�������ʧ��!");
			return 1;//��0ʧ��
		}
	} else {
		Log::log(0,"�ڴ治��,����!");
	}
	return 0;
}


