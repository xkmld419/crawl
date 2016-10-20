/***************************************************************************
*	Copyright (c) 2010,���������Ƽ��ɷ����޹�˾���żƷ��з���
*	All rights reserved.
*	
*	�ļ���: LocalCacheMgr.h
*	����:	���ػ����ۻ��������װ
*	���ߣ�	wangs
*	���ڣ�	2010-10-06
*	�汾��	1.0
*	���ļ�¼��
****************************************************************************/

#ifndef _LOCAL_CACHE_MGR_H
#define _LOCAL_CACHE_MGR_H

#include "LocalCacheInfo.h"
#include "ParamDefineMgr.h"
#include "CSemaphore.h"
#include "DataIncrementMgr.h"
#include "AccuLock.h"
#include "ProdOfferAggr.h"
#include "EventSender.h"
#include "CommonMacro.h"
#include <vector>
#include <map>
#ifdef HAVE_ABM
#include "abmcmdaccu.h"
#endif
using namespace std;
class OfferAccuMgr;

//������۽�����
#define PROCESS_NUM_MAX		64

//���ӿڽ��̲�����
#define CACHE_GET_LOCK		8

//���ػ�������С����
#define CHCHE_NUM_MAX		(ParamDefineMgr::getLongParam(getFlowID(), "CACHE_NUM", "LOCAL_CACHE_BASE"))
#define EVENT_WAIT_FACTOR	(ParamDefineMgr::getLongParam(getFlowID(), "CACHE_NUM", "EVENT_WAIT_FACTOR"))
#define EVENT_PRICE_FACTOR	(ParamDefineMgr::getLongParam(getFlowID(), "CACHE_NUM", "EVENT_PRICE_FACTOR"))
#define EVENT_AGGR_FACTOR	(ParamDefineMgr::getLongParam(getFlowID(), "CACHE_NUM", "EVENT_AGGR_FACTOR"))
#define DISCT_AGGR_FACTOR	(ParamDefineMgr::getLongParam(getFlowID(), "CACHE_NUM", "DISCT_AGGR_FACTOR"))
#define DISCT_DETAIL_FACTOR	(ParamDefineMgr::getLongParam(getFlowID(), "CACHE_NUM", "DISCT_DETAIL_FACTOR"))
#define ACCT_ITEM_FACTOR	(ParamDefineMgr::getLongParam(getFlowID(), "CACHE_NUM", "ACCT_ITEM_FACTOR"))
#define LOCK_KEY_FACTOR		(ParamDefineMgr::getLongParam(getFlowID(), "CACHE_NUM", "LOCK_KEY_FACTOR"))

//����KEYֵ����
#define CACHE_INDEX_LOCK				(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_Index"))
#define ADDRESS_CACHE_DATA_LOCK			(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_AddressData"))
#define EVENT_CACHE_DATA_LOCK			(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_EventCacheData"))
#define EVENT_WAIT_INDEX_LOCK			(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_EventWaitIndex"))
#define EVENT_PRICE_CACHE_DATA_LOCK		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_EventPriceData"))
#define EVENT_AGGR_CACHE_DATA_LOCK		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_EventAggrData"))
#define ACCT_ITEM_CACHE_DATA_LOCK		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_AcctItemData"))
#define DISCT_AGGR_CACHE_DATA_LOCK		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_DisctAggrData"))
#define DISCT_DETAIL_CACHE_DATA_LOCK	(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_DisctDetailData"))
#define MIDDLE_CACHE_INDEX_LOCK			(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_MiddleIndex"))
#define MIDDLE_INFO_DATA_LOCK			(IpcKeyMgr::getIpcKey(-1, "SEM_CACHE_MiddleInfoData"))//����ֵ[.,.+PROCESS_NUM_MAX]
#define MIDDLE_CACHE_DATA_LOCK			(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SEM_CACHE_MiddleData"))
#define CACHE_GET_DATA_LOCK				(IpcKeyMgr::getIpcKey(-1, "SEM_CACHE_GetData"))//����ֵ[.,.+CACHE_GET_LOCK]

//��ѯ���ػ����Ż��ۻ����Ľṹ����
struct DisctResult
{
	DisctCacheData * m_pDisct;
	vector<DisctDetailCacheData *> m_vDetail;
};

/*//�ؽ��ֶ�Ӧ���׵�ַ ��ʱ���ã�����ɾ��
struct KeyAddress
{
	long m_lEventID;
	unsigned int m_iOffset;
}*/

//���ػ��������
class LocalCacheMgr : public LocalCacheInfo
{
public:
	LocalCacheMgr(AcctItemAccuMgr *poAcctItemMgr,OfferAccuMgr *poAccuMgr,
		DataIncrementMgr* poInDB,AccuLockMgr * poLockMgr,EventSender * poSender,int iProc);

	LocalCacheMgr();

	//���������¼�
	bool addEvent(StdEvent * pData);

	//���ӵȴ��¼�
	bool addEvent(StdEvent * pData,long lEventID);

	//�����¼��켣
	bool addEventPricing(EventPriceData *pData);

	//�����¼��ۻ���
	bool addAccuAggr(AccuCacheData *pData);

	//��������
	bool addAcctItemAggr(AcctItemAddAggrData *pData);

	//�����Żݻ���
	bool addAcctDisctAggr(DisctCacheData *pData);

	//�����Ż���ϸ
	bool addAcctDisctDetail(DisctDetailCacheData *pData);

	//�������ؽ���
	bool addLockKey(long *pKeyBuf,int iNum);

	//���ػ�������
	bool clearCache(long lEventID,bool bunlock = false);

	//��ѯ���ػ����ۻ�������
	bool getSyncAccu(vector<AccuCacheData *> &vAccu,vector<DisctResult> &vDisct,long &lEventID);

	//��ѯ���ػ����ۻ������ݣ�ͨ��������¼�ID��
	bool getSyncAccuByEventID(long lEventID,vector<AccuCacheData *> &vAccu,vector<DisctResult> &vDisct);

	//���¼�ȷ�ϳɹ�
	bool syncSuccess(long lEventID);

	//������ȷ�ϳɹ�
	void syncSuccess(int iProcID);

	//��ʧ��״̬Ϊ�ɹ�״̬
	void setFailToSuccess(int iProcID);

	//abm�ӿ�ȷ�ϳɹ����ۻ�������

	//bool syncSuccess(long lEventID,vector<AuditEventAccuData *> &vpAccuData,
	//	map<AuditDisctAccuData *,vector<AuditDisctAccuDetailData *>>  &mpDisctData);

	//abm�ӿ�ȷ��ʧ�ܵ��ۻ�������
	bool syncFail(long lEventID);

	//һ�����۽��������������ύ
	bool commit();

	//һ�����۽�����ӻ���
	bool rollback();

	//���ػ���������
	void create();

	//���ػ�����ɾ��
	void remove();

	//��ʼ������
	void init();

	//��ʱ�¼���Դ��ʼ��
	void initEventSource();

	//��ʼ����ʱָ��
	void initTempP();

public:
	//����������
	bool cacheProc();

	//ת���ȴ��¼�
	void convertWaitEvent(int iLocation,StdEvent ** pEvent);

	//ת��ʧ���¼�
	void convertFailEvent(int iLocation,StdEvent ** pEvent);

	//�����ȴ���������
	void relaWaitProc(long lEventID,bool bRelaOther = false);

    //��ʱ����
    bool overdue(const Date & time, long &lEventID);

private:

	//��ʼ������
	void initEnv();	

	//�������
	bool syncResult(long lEventID,int iType);

	//�������
	void insertData(unsigned int iOffset);

	//����
	void unlockKey(unsigned int iOffset);

private:
	//�ź�������
	static CSemaphore * m_poCacheIndexLock;
	static CSemaphore * m_poAddressDataLock;
	static CSemaphore * m_poEventCacheDataLock;
	static CSemaphore * m_poEventWaitLock;
	static CSemaphore * m_poEventPriceDataLock;
	static CSemaphore * m_poEventAggrDataLock;
	static CSemaphore * m_poAcctItemDataLock;
	static CSemaphore * m_poDisctAggrDataLock;
	static CSemaphore * m_poDisctDetailDataLock;
	static CSemaphore * m_poMiddleIndexLock;
	static CSemaphore * m_poMiddleInfoDataLock[PROCESS_NUM_MAX];
	static CSemaphore * m_poMiddleCacheDataLock;
	static CSemaphore * m_poGetDataLock[CACHE_GET_LOCK];

	//��ַָ�붨��
	AddressCacheData * m_poAddAddress;
	//��ַ����
	unsigned int m_iAddressOffset;
	//��ʱָ�붨��
	EventPriceCacheData * m_poEventPriceLast;
	AccuCacheData * m_poAccuLast;
	AcctItemCacheData * m_poAcctItemLast;
	DisctCacheData *m_poDisctLast;
	DisctDetailCacheData *m_poDisctDetailLast;

	//��ַ��С
	unsigned int m_iAddressTotal;
	//��ַƫ��
	unsigned int m_iOffset;

	//��Ա����ָ��
	int m_iProcID;
	AcctItemAccuMgr *m_poAcctItemMgr;
	OfferAccuMgr    *m_poAccuMgr;
	AccuLockMgr	*m_poLockMgr;
	DataIncrementMgr *m_poInDB;
	EventSender * m_poSender;

	//�����ʵ�����
	AcctItemTransData m_oItemData[MAX_ITEM_RECORD];

public:
	//ʧ�ܻ�����ַ
	vector<unsigned int> m_viAddressFail;
	//�ȴ�������ַ
	vector<unsigned int> m_viAddressWait;
};

#endif //_LOCAL_CACHE_MGR_H
