/*VER: 1*/ 
#ifndef _USAGETYPEMGR_H_
#define _USAGETYPEMGR_H_

#include "CommonMacro.h"
#include "KeyTree.h"
#include "HashList.h"

class UsageTypeMgr
{
public:
    UsageTypeMgr(void);
    ~UsageTypeMgr(void);


public:
    //װ�� b_usage_type ��
    void load();

    //ж��B_USAGE_TYPE����
    void unload();

    //�Ƿ���Ҫ������ʼƷѵ�Ԫ
    bool needCal(int nUsageTypeID);

private:

private:
    static bool m_bUploaded;    //����Ƿ��Ѿ�װ��

public:
    typedef struct _USAGE_TYPE_
    {
        int iUsageTypeID;
        int bNeedCal;
		char m_sUsageTypeCode[50];
    }stUsageType;
private:
    static UsageTypeMgr::stUsageType *m_poEventTypeList;
    static HashList<UsageTypeMgr::stUsageType*>* m_poUsageTypeSerial;  //UsageTypeID����
};

#endif
