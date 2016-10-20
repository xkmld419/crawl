#include "Application.h"
#include "LogV2.h"
#include <iostream>

using namespace std;

Application *GetApplication ();
Application *g_poProc = 0;

int main(int argc, char *argv[])
{
	int oc;
	ABMException oExp;
	char *p = strrchr(argv[0], '/');
    if (p == 0) {
    	strncpy(g_sAppName, argv[0], sizeof(g_sAppName)-1);	
    }
    else {
    	strncpy(g_sAppName, p+1, sizeof(g_sAppName)-1);	
    }

    char * paraOpt = "p:P:n:N";
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
        default:
            printf("������δ֪���� %c\n", oc);
            break;
        }
    }
    
    
    if ((g_poProc=GetApplication()) == NULL) {
        cout<<"GetApplication ʧ��, �����˳�!"<<endl;
        return -1;	
    }
    
    if (g_poProc->appInit() != 0) {
        cout<<"appInit ʧ��, �����˳�!"<<endl;
        return -1;	
    }
    
    
    if (g_poProc->init(oExp) != 0) {
		__INIT_LOG_(oExp);
        return -1;
    }
	
    __DEBUG_LOG2_(0, "Ӧ�ó��� %s -p %d ����...", g_sAppName, g_iProcID);
    
    g_poProc->run();
    
    g_poProc->destory();
    
    delete g_poProc;
    
    __DEBUG_LOG2_(0, "Ӧ�ó���%s -p %d ֹͣ...", g_sAppName, g_iProcID);
    
	return 0;	

}
