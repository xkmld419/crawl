#ifndef _SRV_SHM_MGR_H_
#define _SRV_SHM_MGR_H_

#include "ABMException.h"
#include "PerfStat.h"
#include "SimpleSHM.h"
#include "SockSrvSHM.h"
#include "AccuTTStruct.h"

//��abm����ʹ�õĹ����ڴ棬ʹ��һ��
//
struct SrvShmData {
    StatData m_oStatData;
    SockAppStruct m_oSockData;
    AccuTTStruct m_oAccuTTData;
};

class SrvShmMgr {
public:
    //��abminit���ã����乲���ڴ�
    //-1 ϵͳ����
    //-2 �������ô���
    static int mallocMem(ABMException &oExp);
    static int removeMem(ABMException &oExp);

    //��socksrv, srvlog�ȵ��ã����ӹ����ڴ�
    //-1 ϵͳ����
    //-2 �������ô���
    static int open(ABMException &oExp, bool bWrite=false, bool bRead=true);

    //��socksrv, srvlog�ȵ��ã���ȡ��StatData����ָ��
    //����0����û��open
    static StatData * getStatData() { return m_poStatData; };

    static SockAppStruct * getSockData() { return m_poSockData; };

    static AccuTTStruct* getTTData() { return m_poAccuTTData; };

private:
    static long getKey(ABMException &oExp);
    static int  openSHM(ABMException &oExp, bool iCreate, unsigned int iPermission);

private:
    static SimpleSHM * m_poSHM;
    static StatData * m_poStatData;
    static SockAppStruct * m_poSockData;
    static AccuTTStruct * m_poAccuTTData;
};

#endif

