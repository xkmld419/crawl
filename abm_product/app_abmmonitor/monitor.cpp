/*VER: 1*/ 
/* Copyright (c) 2000-<2003> Linkage, All rights Reserved */
/* #ident "@(#)main.c    1.0    2001.1.11    ruanyongjiang" */

#include <signal.h>
#include <iostream>
#include "TOCIQuery.h"
#include "Log.h"
#include "login.h"  



using namespace std;

//char ** g_argv;

char *gsState[] = {
	"SLP",   
	"INIT", //׼������
	"RUN",
	"BOOT",
	"DOWN",
	"Out",//��ǰ��kill
	"ABRT",
	"DOWN",
	"DROP",
	"DEST",
	"KILL"
};


void debug()
{
   sleep(1);   
}

int main()
{
    try {
        signal (SIGINT, SIG_IGN);
        signal (SIGQUIT, SIG_IGN);
                
        startwin();
        login lg;
        lg.run();
        endwin();
    }
    //# OCI Exception
    catch (TOCIException &e) {
        cout<<"[EXCEPTION]: TOCIException catched"<<endl;
        cout<<"  *Err: "<<e.getErrMsg()<<endl;
        cout<<"  *SQL: "<<e.getErrSrc()<<endl;
        Log::log (0, "�յ�TOCIException�˳�\n[TOCIEXCEPTION]: %s", e.getErrMsg());
    }

    //# Exception
    catch (Exception &e) {
        cout << "[EXCEPTION]: " << e.descript () << endl;
        Log::log (0, "�յ�Exception�˳�\n[EXCEPTION]: %s", e.descript());
    }

    //# Other Exception
    catch (...) {
        cout << "[EXCEPTION]: Exception catched" <<endl;
        Log::log (0, "�յ�����Exception�˳�");
    }
} 


