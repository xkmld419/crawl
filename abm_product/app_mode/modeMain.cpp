#include "mode.h"
#include "Log.h"
#include "mntapi.h"
#include "ParamDefineMgr.h"

int main(int argc, char* argv[])
{
	TMode oMode;
	CommandCom oCommandCom;
	//Log::init(MBC_APP_hbmode);
	if(oCommandCom.InitClient() == false)
	{
		cout << "���ӹ����ڴ����" <<endl;
		exit(0);
	}
	//Log::m_iAlarmLevel = 3;
	//Log::m_iAlarmDbLevel = -1;
	oMode.init(0,&oCommandCom);
	return oMode.deal(argv,argc);
}
