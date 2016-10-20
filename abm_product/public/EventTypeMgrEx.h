/*VER: 5*/ 
#ifndef _EVENT_TYPE_MGR_H_
#define _EVENT_TYPE_MGR_H_

#include "Log.h"
#include "TOCIQuery.h"
#include "KeyTree.h"
#include "HashList.h"
#include "OrgMgrEx.h"
#include "EdgeRoamMgr.h"
#include "FormatMethod.h"
#include "Head2OrgIDMgr.h"
#include "LongEventTypeRuleEx.h"
#include "OrgLongTypeMgr.h"

#include <list>

#define     MAX_SEGMENT_LENGTH     64

//��ӦB_NUMBER_TYPE
#define     NUMBER_TYPE_PN          "52Z"
#define     NUMBER_TYPE_IMSI        "52F"
    

using namespace std ;

class EventTypeMgrEx;
class EventTypeMgrEx
{
public:
    
    ~EventTypeMgrEx(void);

public:

    static EventTypeMgrEx* GetInstance(void);
    bool GetAreaCode(int iOrgID,char* pAreaCode,int nSize);

    bool GetOrgIDbyNbr(const char* pNbr,const char* pTime,
        int& iOrgID,const char* tszType="50Z",int* piPartyID =0,int* piPartnerID =0);

    //��ȡ�����ĳ�;����������
    bool fillLongTypeAndRoamID(EventSection* pEventSection,int iType=0);

    //��������к͵������Ĺ����������������
    bool fillOrgID(EventSection* pEventSection);


    int GetEventTypeID(EventSection* pEventSection);

    //��ȡû�����εĺ�ת�����ĳ�;����
    int GetLongTypeNoRoamFW(EventSection* pEventSection);
    //��ȡ���κ�ת�����ĳ�;����
    int GetLongTypeRoamFW(EventSection* pEventSection);

        //������ľ��뷵�س�;����
    int GetLongTypeByDistance(long lDistance);

    //����ľ��뷵����������
    int GetRoamTypeByDistance(long lDistance);

    //�������ź�ORG
    //ARG   >>which         int     0x001,����,0x010,����,3,0x100����������
    //      >>va            int     1,������,2,���ε�
    bool fillAreaCode(EventSection* pEventSection, int which=0, int va=0);

    //��ȡ�����л�����������ORG_ID
    int GetOrgID(EventSection* pEventSection,int iType);
    /***ɽ��������ȡHomeType �� ServType****/

    //ɽ����������ͨ��td_inter_fee_code �� td_msisdn_seg ����ȡHomeType��ServType
    bool GetSDSpecTypeB(EventSection* pEventSection,char* sType,int iSize,int iType=0);
    /***ɽ��������ȡHomeType �� ServType****/
private:
    bool InitInstance(void);
    void ExitInstance(void);


    
    //ͨ������ORG_ID�ж�
    inline int GetLongTypeByOrgID(int iOrgID1,int iOrgID2);
public:
    //ͨ������ORG_ID�ж���������
    int GetRoamTypeByOrgID(int iOrgID1,int iOrgID2);
private:
    //�ƷѺ���û�������жϳ�;���ͺ��¼�����
    inline bool Rule_BillingNoRoam(EventSection* pEventSection);

    //�ƷѺ�������
    inline bool Rule_BillingRoam(EventSection* pEventSection,int iType=0);

    //��ȡָ�������εػ�����ص�����
    inline bool fillAreaCodeSpecify(EventSection* pEventSection,int which=0, int va=0);
private:
    EventTypeMgrEx(void);

    static EventTypeMgrEx* m_gpEventTypeMgrEx;

    Head2OrgIDMgr* m_pLocalHeadMgr;
    
    HeadEx* m_pLocalHead;

    //CHLRMgr* m_pHlrMgr;
    char m_sLongTypeNoModify[32];

    MobileRegionMgr* m_pRoamMgr;
    OrgMgrEx* m_pOrgMgrEx;

    LongEventTypeRuleMgrEx* m_pEventRuleMgr;

    int m_iBillingOrgID,m_iOtherOrgID,m_iBillingVisitOrgID,
        m_iOtherVisitOrgID;
    int m_iCallingCarrier,m_iCalledCarrier;
    /***ɽ��������ȡHomeType �� ServType****/
    typedef struct _HEAD_SPEC_INFO_
    {
        char sHomeType[8];
        char sServType[8];
        char sAreaCode[MAX_BILLING_AREA_CODE_LEN];
        char sHead[11];
        char sEffDate[MAX_START_DATE_LEN];
        char sExpDate[MAX_START_DATE_LEN];
    }HeadSpecInfo;
    KeyTree<HeadSpecInfo*>* m_pHeadSpecInfo;  //����
    KeyTree<HeadSpecInfo*>* m_pHeadSpecInfo_Inter ; //����
    KeyTree<HeadSpecInfo*>* m_pHeadSpecInfo_national;
    /***ɽ��������ȡHomeType �� ServType****/
};


#endif


