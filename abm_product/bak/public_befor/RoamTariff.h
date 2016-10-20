
#ifndef ROAMTARIFF_H_INCLUDED_HEADER
#define ROAMTARIFF_H_INCLUDED_HEADER

#include "HashList.h"
#include "CommonMacro.h"
#include "EventSection.h"
#include "Log.h"
//#include "CommandCom.h"
#include "OrgMgrEx.h"

/**
 *  
 * @ ���������ʷ� (�����빦��δ����,��ע��) 
 * @date 2010-08-25 
 * @author wuan
 */

#define CTOC_CARRIER 1
#define CTOG_CARRIER 2


#define PEER_TYPE_LOCAL     1   //���б���
#define PEER_TYPE_CHINA     2   //�����й�
#define PEER_TYPE_OTHER     3   //�����������ҵ���

/*������For 2.8*/
#define CTOC_CARRIER_NOT_EXIST  311
#define CTOC_CARRIER_EXP        312
#define CTOC_CARRIER_NOT_EFF    313
#define CTOC_COUNTRY_NOT_EXIST  314
#define CTOC_SECTOR_NOT_EXIST   315

#define CTOG_CARRIER_NOT_EXIST  321
#define CTOG_CARRIER_EXP        322
#define CTOG_CARRIER_NOT_EFF    323
#define CTOG_COUNTRY_NOT_EXIST  324
#define CTOG_SPONSOR_NOT_EXIST  325
#define CTOG_SECTOR_NOT_EXIST   326

#define TOKEN       "��"



//�ʷ���Ϣ
struct TariffInfo {
    //���������ʷ�����ʶ--index
    int m_iSectorNo;              
    //�����ʷ����ʶ 
    int m_iSpecialGroupID; 
    //�¼����ͱ�ʶ            
    int m_iEventTypeID;          
    //����         
    long m_lValue;          

    char m_sEffDate[9];
    char m_sExpDate[9];

    //ͬһ�ʷ������µ���һ���ʷ�               
    unsigned int m_iNext;               
};


//������Ӫ�����ʷ�����ϵ
struct CarrierInfo {
    //������Ӫ�̴���+������Ӫ�̴���+ImsiType--index
    char m_sCarrierCode[8]; 
    //������Ӫ�̴���
    char m_sSponsorCode[8];
    //
    char m_sImsiType[2];            
    //������Ӫ�����ڹ���
    char m_sVisitedCountry[500];         
    //�ʷ�����ʶ 
    char m_sSectorID[20];   
    //�ʷ������� 
    //char m_sSectorName[100]; 
    // ��Ч���� 
    char m_sEffDate[9];     
    //ʧЧ����           
    char m_sExpDate[9];              

    //ͬһ��Ӫ�̵���һ����¼
    int m_iNext;
};

//������Ӫ����Ϣ
struct SponsorInfo {
    //������Ӫ�̵�IMSI��ʶ
    char m_sSponsorID[8];
    //������Ӫ�̴���
    char m_sSponsorCode[8];
    //ȱʡ�ʷ���
    int m_iDefaultSectorNo;
};

//������Ϣ
struct CountryInfo {
    //���ҵ�������
    char m_sCountryCode[4];
    //����
    char m_sAreaCode[10];
    //���ҵ�����������
    char m_sCnName[100];
    char m_sEffDate[9];
    char m_sExpDate[9];

    //ͬһ�����Ҵ�����һ������
    int m_iNext;
};

//�����ʷ���
struct SpecialGroupMember {
    int m_iGroupID;
    //��Ա(����)
    char m_sMember[10];

    int m_iNext;
};



class RoamTariffMgr {
public:
    ~RoamTariffMgr();
    /**
    *   @param iBelongOrgID:��������Org
    */
    static RoamTariffMgr *getInstance();
    /**
     * @param 
     * in  
     *        pEvent         �¼�
     *        sCarrierCode   ��Ӫ�̴���
     *        sAreaCode      ����
     * out 
     *        lValue         ����
     * @return �ɹ�true/ʧ��false
     */
    bool getValue(EventSection *pEvent, char *sCarrierCode,
                  char *sAreaCode,  long &lValue);
    // /** 
    // * @param sDate: yyyymmdd,Ĭ��ʹ�õ�ǰʱ��
    // * @return iErrNo:1-������ 2-��ʧЧ 3-δ��Ч 4-�ظ�
    // */
    //bool auditCountry(int iOrgID, int &iErrNo, char *sDate=0);

    #ifdef PRIVATE_MEMORY
    static void load( );
    static void unload();
    static void reload( );
    #endif

    SponsorInfo * getSponsorInfo( char *sSponsorID);
    CarrierInfo * getCarrierInfo( char *sCarrierCode, char *sDate=0);
    #ifdef PRIVATE_MEMORY
    CountryInfo * getCountryInfo( char * sCnName);  //�˷���������
    #endif
    //CountryInfo * getCountryInfo( int iOrgID, char *sDate=0);
    bool getSectorNo(char *sSector, int &iSectorNo);

    //bool checkSponsor(char *sSponsorCode, int &iType, int &iErrNo);
    //bool checkCarrier(char *sCarrierCode, int iType, int &iErrNo,
    //                  char *sDate=0);
    //bool checkCountry(char *sCarrierCode, int iType, int &iErrNo);
    //bool checkSector(char *sSectorID, int iType, int &iErrNo);

    bool getAreaCode(char *CountryCode, char *sAreaCode, int iSize, char *sDate=0);
    CountryInfo * getCountryByAreaCode(char *sAreaCode, char *sDate=0);
    bool isBelong(int iGroupID, char *sAreaCode);

protected:
    bool getValue( int iSectorID, int iEventTypeID, char *sAreaCode,
                   char *sDate, long &lValue) ;

    #ifdef PRIVATE_MEMORY
    static void loadCarrierInfo( );
    static void loadTariffInfo( );
    static void loadSectorInfo( );
    static void loadCountryInfo( );
    static void loadSpecialGroupMember();
    static void loadSponsorInfo();
    #endif
private:
    char *strsep(char **stringp, const char *delim);
    void parseKey(char *key, char *sCarrierCode, const char *substr);

private:

    RoamTariffMgr();

    #ifdef PRIVATE_MEMORY
    static HashList<int > *m_poTariffIndex;
    static TariffInfo *m_poTariffList;

    static HashList<int > *m_poCarrierIndex;
    static CarrierInfo *m_poCarrierList;

    static HashList<int > *m_poSectorIndex;

    static HashList<int > *m_poCountryIndex;////--
    static HashList<int > *m_poCountryCodeIndex;
    static HashList<int > *m_poCountryAreaIndex;
    static CountryInfo *m_poCountryList;

    static SponsorInfo *m_poSponsorList;
    static HashList<int > *m_poSponsorIndex;

    static SpecialGroupMember *m_poMemberList;
    static HashList<int> *m_poMemberIndex;

    static bool m_bUpLoaded;
    #endif

    static OrgMgrEx *m_poOrgMgrEx;
    static RoamTariffMgr *m_poRoamTariffMgr;
    
public:
    //For Debug Use
    void printAll();
};



#endif //#ifdef ROAMTARIFF_H_INCLUDED_HEADER

