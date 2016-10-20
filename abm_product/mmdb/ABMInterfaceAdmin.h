#ifndef _ABM_INTF_ADMIN_H_
#define _ABM_INTF_ADMIN_H_

#include "LocalCacheMgr.h"
#include "OfferAccuMgr.h"
#include "ProdOfferAggr.h"
#include "Value.h"
#ifdef HAVE_ABM
#include "abmclnt.h"
#include "abmaccess.h"
#endif

/**
 ** Notes: 
 **  1.�ۻ���ͬ����ѯ��
 **  2.�ۼ���ͬ���� �£�
 **  3.�ۻ����첽�� �£�
 **  4.�ۻ�������� �¡�
 **  Date: 2010-10-5
*/

    #define DEL_SIN(x)      \
            if( x ){        \
                delete x;   \
                x = 0;      \
            }

#define QUERY_SCOPE_ALL "ALL"
#define QUERY_SCOPE_NEG -1
#define QUERY_SCOPE_NIL 0

const int RET_SUC = 0;  //�ɹ�
const int RET_UPD = 1;  //��Ҫ����
const int RET_OTR = 2;  //����


const int GROUP_ASYN = 1;
const int GROUP_SYN = 2;

struct ABMDisctResult {
    DisctAccuData *m_poDisct;
    vector<DisctAccuDetailData *> m_vDetail;
};

//Part 1    --  ABM basic data container
typedef vector<EventAccuData *>                               abm_vec_e;
typedef vector<EventAccuData *>::iterator                     abm_vec_e_it;
typedef vector<DisctAccuData *>                               abm_vec_d;
typedef vector<DisctAccuData *>::iterator                     abm_vec_d_it;
typedef vector<DisctAccuDetailData *>                         abm_vec_d_d;
typedef vector<DisctAccuDetailData *>::iterator               abm_vec_d_d_it;
typedef vector<ABMDisctResult >                               abm_vec_d_r;
typedef vector<ABMDisctResult >::iterator                     abm_vec_d_r_it;
typedef map<DisctAccuData *, abm_vec_d_d>                     abm_d_dv_map;
typedef map<DisctAccuData *, abm_vec_d_d>::iterator           abm_d_dv_map_it;
//Part 2    --  Audit data container
typedef vector<AuditEventAccuData *>                          abm_vec_e_a;
typedef vector<AuditEventAccuData *>::iterator                abm_vec_e_a_it;
typedef vector<AuditDisctAccuData *>                          abm_vec_d_a;
typedef vector<AuditDisctAccuData *>::iterator                abm_vec_d_a_it;
typedef vector<AuditDisctAccuDetailData *>                    abm_vec_d_d_a;
typedef vector<AuditDisctAccuDetailData *>::iterator          abm_vec_d_d_a_it;
typedef map<AuditDisctAccuData *, abm_vec_d_d_a >             abm_d_dv_map_a;
typedef map<AuditDisctAccuData *, abm_vec_d_d_a >::iterator   abm_d_dv_map_a_it;
//Part 3    --  Synchronous data container
typedef vector<SynEventAccuData *>                            abm_vec_e_s;
typedef vector<SynEventAccuData *>::iterator                  abm_vec_e_s_it;
typedef vector<SynDisctAccuData *>                            abm_vec_d_s;
typedef vector<SynDisctAccuData *>::iterator                  abm_vec_d_s_it;
typedef vector<SynDisctAccuDetailData *>                      abm_vec_d_d_s;
typedef vector<SynDisctAccuDetailData *>::iterator            abm_vec_d_d_s_it;
typedef map<SynDisctAccuData *, abm_vec_d_d_s >               abm_d_dv_map_s;
typedef map<SynDisctAccuData *, abm_vec_d_d_s >::iterator     abm_d_dv_map_s_it;
//Part 4    -- HB basic data container
typedef vector<AccuCacheData *>                               hb_vec_e;
typedef vector<AccuCacheData *>::iterator                     hb_vec_e_it;
typedef vector<DisctCacheData *>                              hb_vec_d;
typedef vector<DisctCacheData *>::iterator                    hb_vec_d_it;
typedef vector<DisctDetailCacheData *>                        hb_vec_d_d;
typedef vector<DisctDetailCacheData *>::iterator              hb_vec_d_d_it;
typedef vector<DisctResult >                                  hb_vec_d_r;
typedef vector<DisctResult >::iterator                        hb_vec_d_r_it;
typedef map<DisctAccuData *, hb_vec_d_d>                      hb_d_dv_map;
typedef map<DisctAccuData *, hb_vec_d_d>::iterator            hb_d_dv_map_it;


class ABMIntfAdmin {

public:
    ABMIntfAdmin();

    ~ABMIntfAdmin();

    ////*---- �¼��ۻ���*/

    long getAccuByInst( long lOfferInstID, long lServID, long lMemberID,
                        int iAccuTypeID,long lCycleInstID, long lCycleTypeID,
                        long lBeginTime, long lEndTime, char *sSplitby = QUERY_SCOPE_ALL, 
                        int iBillingCycleID=QUERY_SCOPE_NEG );

    int getAllAccuByInst( long lOfferInstID,long lServID,long lMemberID,
                          int iAccuTypeID,long lCycleInstID, long lCycleTypeID,
                          long lBeginTime, long lEndTime, vector<AccuData > &vOut,
                          int iMergeFlag, int iBillingCycleID = QUERY_SCOPE_NEG);
    ////�ײ�δʵ�֣���Ҫ���ABM����֧��
    bool getAll(long lOfferInstID, vector<AccuData > &vAccuData, 
                int iBillingCycleID= QUERY_SCOPE_NEG);

    bool updateAccByInst(long lOfferInstID, long lMemberID, long lServID, 
                         int iAccuTypeID, long lCycleTypeID, long lBeiginTime,
                         long lEndTime, long lValue, int iBillingCycleID= QUERY_SCOPE_NEG,
                         char *sSplitby = QUERY_SCOPE_ALL, char *sAccuLevel=NULL, 
                         bool bReplace = false, long lVersionID = QUERY_SCOPE_NEG);
    //
    void printOfferInst(long lOfferInstID);

    ////*---- �Ż��ۻ���*/

    //��iCombineId = -1, iMeasureId = -1,����Ϊ�ж�����
    bool getAllDisctAggr( ProdOfferAggrData *pData, Value *pResult);

    //��iCombineId = -1, iMeasureId = -1,����Ϊ�ж�����
    bool getAllItemAggr( ProdOfferAggrData *pData, Value *pResult);

    bool getAllDisct( long lOfferInstId, int iBillingCycleId, int iCombineId, 
                      int iMeasureId, int iItemGroupId, int iInType, 
                      long lInValue, DisctAccuData *pDisctData, abm_vec_d_d &vDisctDetail,
                      int iOrgBillingCyleID= QUERY_SCOPE_NEG);

    /////���ʵ��:δʹ���� 
    bool getItemDetail(ProdOfferTotalData *pData, Value *pValue); 

    ////�ײ�δʵ�֣���Ҫ���ABM����֧��
    bool getAllAggr(long lOfferInstID,int iBillingCycleID, vector<ProdOfferTotalData>& vPOTData,
                    int iOrgBillingCycleID= QUERY_SCOPE_NEG, long lServID = QUERY_SCOPE_NIL);

    bool updateDisct(ProdOfferAggrData *pAggr, ProdOfferDetailData *pDetail, 
                     bool bReplace = false);
    ////��ҪABM����֧�֣�DistcAccuDetailData��Ҫ����ֶ�m_lItemDisctSeq
    bool updateDisctZero(long lOfferInstID, long lItemDisctSeq);

    void printOfferDisct();

    /*
    **�첽����(dangerous)
    ** 
    */ 
    bool asynAuditAllAccu(hb_vec_e &vEventAccu, hb_vec_d_r &vDisctResult,const long & lEventID);
    /*
    *ͬ������
    **  @return -
    **      RET_SUC                   �ɹ�
    **      RET_UPD                   ��Ҫ����
    **      RET_OTR                   ��������(����ִ�д���)
    **      ECODE_MQ_NOMSG            û����Ϣ
    */
    int asynGetAuditAllResult(vector<AuditEventAccuData *> &vEventAccu, 
                              map<AuditDisctAccuData *, vector<AuditDisctAccuDetailData *> > &vMap,
                              vector<EventAccuData *> &vFailEventData,
                              map<DisctAccuData*, vector<AuditDisctAccuDetailData *> > &vFailMap,
                              long &lEventID);
    /*
     *ͬ������
     *  @return -
     *      RET_UPD                  ��Ҫ����
     *      ����               ������
    */ 
    int realtimeGetAllResult(abm_vec_e_s &vEventAccu, 
                             abm_d_dv_map_s &vMap);
    //�첽��ѯ���˸��½��(��ʱ�ӿ�)
    int queryAuditResult( const long &lEventID, abm_vec_e_a & vEventData, 
                           abm_d_dv_map_a &vMap,
                           vector<EventAccuData *> &vFailEventData,
                           map<DisctAccuData*, vector<AuditDisctAccuDetailData *> > &vFailMap);

    //�¼��ۻ�����ѯ
    bool synQryEventAccu(EventAccuCond *pCondition,abm_vec_e &vOut );

    //�����¼��ۼ�����bRelpace-false,�ۼӣ�bReplace-true,����
    bool synUpdEventAccu(EventAccuData *pData, bool bReplace= false);

    //��ѯ�Ż��ۼ�������
    bool synQryDisctAggr(DisctAccuCond *pCondition, abm_vec_d &vDisctAggr);

    //��ѯ�Ż��ۼ�������
    bool synQryDisct(DisctAccuCond *pCondition, abm_d_dv_map &vMap);

    //�����Ż��ۼ�����bRelpace-false,�ۼӣ�bReplace-true,����
    bool synUpdDisct(DisctAccuData *pAggr, abm_vec_d_d &vDetail, bool bReplace = false);

    //ͨ�ò�ѯ
    bool synQryAll(EventAccuCond *pEventCond, abm_vec_e &vEventAccu, 
                   DisctAccuCond *pDisctCond, abm_d_dv_map &mDisctAccu);

private:
    //�ṹ��ת����ABM�� => HB��
    void accuABM2HB(EventAccuData *pABMData, AccuData *pHBData, 
                    long lCycleInstID=QUERY_SCOPE_NIL);

private:
    static LocalCacheMgr *m_poLocalCacheMgr;

    static OfferAccuMgr *m_poOfferAccuMgr;

    static ABM *m_poABMClient;
};

#endif //#ifndef _ABM_INTF_ADMIN_H_

