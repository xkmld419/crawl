#ifndef _DCC_MSG_API_H_HEADER_
#define _DCC_MSG_API_H_HEADER_
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "DccMsgParser.h"
#include "LogV2.h"

#define MSG_SIZE 4096


class DccApi {
public:
    
    DccApi():m_poDcc(0) {}
    ~DccApi() { if (m_poDcc != 0) delete m_poDcc; }    
    
    //static int init();
    //static int cer2cea(char *sCer, int iLen, char *sCea);
        
    int init();
    
    //����CER����CEA
    int cer2cea(char *sCer, int iLen, char *sCea);
    
    //����DWR����DWA
    int dwr2dwa(char *sDwr, int iLen, char *sDwa);
    
    //����DPR����DPA
    int dpr2dpa(char *sDpr, int iLen, char *sDpa);
    
    //�õ�����õ�CER
    int getCer(char *sBuf, int iMaxLen);
    
    //�õ�����õ�DWR
    int getDwr(char *sBuf, int iMaxLen);
    
    //�õ�����õ�DPR
    int getDpr(char *sBuf, int iMaxLen);
    
private:
    
    //static DccMsgParser *m_poDcc;
    DccMsgParser *m_poDcc;
};

#endif