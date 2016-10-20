#ifndef OVERTIMEMGR_INCLUDED_HEADER_H
#define OVERTIMEMGR_INCLUDED_HEADER_H

/**
 * date :   2011-1-17 
 * func :   ��ʱ���� 
 */


#include "LocalCacheMgr.h"
#include "AccuLock.h"
#include "ABMInterfaceAdmin.h"
#include "Process.h"
#include "Date.h"

const int DEFAULT_INTERVAL = 30;    //Ĭ�ϳ�ʱʱ����(��)
const int MINIMUM_INTERVAL = 5;     //��С��ʱʱ����(��)

class OverTimeMgr : public Process {
public:
    OverTimeMgr();
    ~OverTimeMgr();

    int run();
    bool localBufProc();
    bool accuLockProc();

private:
    bool prepare();
    void printUsage();
private:
    Date m_oDate;       //��ʱ��㣺�������ʱ��������Ϊ��ʱ����

    static LocalCacheMgr *m_poLocalCacheMgr;
    static AccuLockMgr *m_poAccuLockMgr;
    static ABMIntfAdmin *m_poABMAdmin;
    
    static OfferAccuMgr *m_poOfferAccuMgr;
    static ProdOfferDisctAggr *m_poProdOfferMgr;

    int m_iTime;        //���м��(��)
    static bool m_bExit;
};


#endif //#ifndef OVERTIMEMGR_INCLUDED_HEADER_H

