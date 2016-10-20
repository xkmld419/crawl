#include <stdlib.h>
#include "Environment.h"
#include "ParamDefineMgr.h"
#include "Log.h"
#include "LogDB.h"
#include <sys/types.h>
#include <signal.h>
int main(int argc ,char *argv[])
{
    int i = 1;
    int iSysProcID = 0;
    char sTemp[6];
    char sString[1024];
    
    try{
        ParamDefineMgr::getParam("LOGDB", "LOGDB_SWITCH", sTemp, 5);
        if(!atoi(sTemp)){
            Log::log(0, "LOG��⿪��δ��\n");
            return -1;
        }
        
        LogDB::init();
        
        while (i < argc){
            if (argv[i][0] != '-'){
                i++;
                continue;
            }
            
            switch(argv[i][1] | 0x20){
                case 'k':
                    iSysProcID = LogDB::getSysProc();
                    if(iSysProcID > 0){
                        kill(iSysProcID, SIGKILL);
                        LogDB::freeSHMBuf();
                    }
                    Log::log(0, "ɾ�������ڴ�ͳ�פ�������\n");
                    i++;
                    break;
                    
                case 'c':
                    LogDB::logFileOper();
                    Log::log(0, "���������ڴ�ͳ�פ�������\n");
                    i++;
                    break;
                    
                default:
                    i++;
                    Log::log(0, "********************************************************************\n"
                                 "  LOG��� logdbmgr Version 1.0.0\n"
                                 "  ���÷���:\n"
                                 "  ����ģʽ: logdbmgr -c (���������ڴ�ͳ�פ����)\n"
                                 "            logdbmgr -k (ɾ�������ڴ�ͳ�פ����)\n"
                                 "********************************************************************\n"
                              );
                    break;
            }
        }
        
        if(argc == 1)
            Log::log(0, "********************************************************************\n"
                                 "  LOG��� logdbmgr Version 1.0.0\n"
                                 "  ���÷���:\n"
                                 "  ����ģʽ: logdbmgr -c (���������ڴ�ͳ�פ����)\n"
                                 "            logdbmgr -k (ɾ�������ڴ�ͳ�פ����)\n"
                                 "********************************************************************\n"
                              );
    }
    catch( TOCIException & e ){
        snprintf( sString, 1024, "LOG��⴦���յ�TOCIException\n[TOCIEXCEPTION]: TOCIException catched\n"
            "*Err: %s\n""  *SQL: %s\n********\n", e.getErrMsg(),e.getErrSrc());
        Log::log( 0, sString );
    }
    catch( TException & e ){
        snprintf( sString, 1024, "LOG��⴦���յ�TException\n[EXCEPTION]: %s\n********\n", e.GetErrMsg());
        Log::log( 0, sString );
    }
    catch( Exception & e ){
        snprintf( sString, 1024, "LOG��⴦���յ�Exception\n[EXCEPTION]: %s\n********\n", e.descript());
        Log::log( 0, sString );
    }
    
    return 0;
}
