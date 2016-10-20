#include "Application.h"
#include "LogV2.h"
#include "HSSLogV3.h"
#include <iostream>

Application *GetApplication ();
Application *g_poProc = 0;

int main(int argc, char *argv[])
{
	int oc, iRet;
	ABMException oExp;
	char *p = strrchr(argv[0], '/');
    if (p == 0) {
    	strncpy(g_sAppName, argv[0], sizeof(g_sAppName)-1);	
    }
    else {
    	strncpy(g_sAppName, p+1, sizeof(g_sAppName)-1);	
    }

    char * paraOpt = "p:P:n:N:m:M";
    while ((oc=getopt(argc, argv, paraOpt)) != -1) {
        switch (oc) {
        case 'p':
        case 'P':
            g_iProcID = atoi(optarg);
            break;
        case 'n':
        case 'N':
            g_iNodeID = atoi(optarg);
            break;
        case 'm':
        case 'M':
            g_iModuID = atoi(optarg);
            break;    
        default:
            printf("������δ֪���� %c\n", oc);
            DIRECT_LOG(V3_INFO, V3_SCHEDUL,  MBC_PARAM_THREAD_NONSUPP_ERROR, "Ӧ�ó��� %s ����δ֪��������%c", argv[0], oc);
            break;
        }
    }
    
    if ((g_iProcID==-1) || (g_iModuID==-1)) {
        std::cout<<"������������ -p x -m x ����!"<<endl;
        DIRECT_LOG(V3_FATAL, V3_SCHEDUL, MBC_PARAM_THREAD_LOST_ERROR, "Ӧ�ó��� %s �������������� -p x -m x ����!" , argv[0]);    
        return -1;         
    }
    
    if ((g_iProcID<0) || (g_iModuID<0) ) {
        DIRECT_LOG(V3_FATAL, V3_SCHEDUL,  MBC_PARAM_THREAD_VALUE_ERROR, "Ӧ�ó��� %s �����������ֵ ����Ǹ�!" , argv[0]);
        return -1;        
    }
    
    if ((g_poProc=GetApplication()) == NULL) {
        std::cout<<"GetApplication ʧ��, �����˳�!"<<endl;
        return -1;	
    }

	try {
    
    if (g_poProc->appInit(oExp) != 0) {
        oExp.printErr();
        return -1;	
    }
    
    
    if (g_poProc->init(oExp) != 0) {
		__INIT_LOG_(oExp);
        return -1;
    }
	
    __DEBUG_LOG3_(0, "Ӧ�ó��� %s -p %d -m %d ����...", g_sAppName, g_iProcID, g_iModuID);
    DIRECT_LOG(V3_INFO, V3_SCHEDUL, MBC_ACTION_PROCESS_START, "Ӧ�ó��� %s -p %d -m %d ����...", g_sAppName, g_iProcID, g_iModuID);
    
    iRet = g_poProc->run();

	}
	catch (...) {
		std::cout<<"catch sigbort signal"<<endl;
	}
    
   //  g_poProc->destory();
    
    delete g_poProc;
    
    __DEBUG_LOG3_(0, "Ӧ�ó���%s -p %d -m %d ֹͣ...", g_sAppName, g_iProcID, g_iModuID);
    
    if (RcvExcepSig()) {
        DIRECT_LOG(V3_FATAL, V3_SCHEDUL, MBC_THREAD_EXIT_ERROR, "Ӧ�ó���%s -p %d -m %d receive unexcept signal!", g_sAppName, g_iProcID, g_iModuID);        
    }
    if (!iRet) {
        DIRECT_LOG(V3_INFO, V3_SCHEDUL, MBC_ACTION_PROCESS_END, "Ӧ�ó���%s -p %d -m %d ֹͣ...", g_sAppName, g_iProcID, g_iModuID);    
    }
    else {
        DIRECT_LOG(V3_FATAL, V3_SCHEDUL, MBC_THREAD_EXIT_ERROR, "Ӧ�ó���%s -p %d -m %d ֹͣ...", g_sAppName, g_iProcID, g_iModuID);
    }
	return 0;	

}
