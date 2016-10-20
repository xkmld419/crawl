/***************************************************************************
*	Copyright (c) 2010,���������Ƽ��ɷ����޹�˾���żƷ��з���
*	All rights reserved.
*	
*	�ļ���: LocalCacheInfo.h
*	����:	���۽�����ػ������ݽṹ���弰����������װ
*	���ߣ�	wangs
*	���ڣ�	2010-08-13
*	�汾��	1.0
*	���ļ�¼��
*   2010-08-13 ���� AccuCacheData��DisctCacheData��DisctDetailCacheData����
*   2010-08-14 ����ÿ�����˵�����������ֶ�˳��HBר���ֶηŵ���ײ�
*   2010-10-5  ���ӳ���ABM�������ۻ�����Ľṹ���壬���ݲ�����������
****************************************************************************/

#ifndef _LOCAL_CACHE_INFO_H_
#define _LOCAL_CACHE_INFO_H_

#include "SHMData.h"
#include "SHMIntHashIndex.h"
#include "SHMData_A.h"
#include "SHMIntHashIndex_A.h"
#include "StdEvent.h"
#include "DataIncrementMgr.h"
#include "IpcKey.h"
#include "AcctItemAccu.h"

using namespace std;

const bool FORCE_YES							= true;
const bool FORCE_NO							= false;
const unsigned int TYPE_CACHE_INIT			= 0;	//��ʼ̬
const unsigned int TYPE_CACHE_RDY				= 1;	//׼������̬
const unsigned int TYPE_CACHE_SUCCESS			= 2;	//����ɹ�̬
const unsigned int TYPE_CACHE_FAIL			= 3;	//����ʧ��̬

const unsigned int TYPE_SEND_NO				= 0;	//δ����״̬
const unsigned int TYPE_SEND_YES				= 1;	//�ѷ���״̬

#define CACHE_INDEX				(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_Index"))
#define ADDRESS_CACHE_DATA		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_AddressData"))
#define EVENT_WAIT_INDEX		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_EventWaitIndex"))
#define EVENT_WAIT_DATA			(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_EventWaitData"))
#define EVENT_CACHE_DATA		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_EventData"))
#define EVENT_PRICE_CACHE_DATA	(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_EventPriceData"))
#define EVENT_AGGR_CACHE_DATA	(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_EventAggrData"))
#define ACCT_ITEM_CACHE_DATA	(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_AcctItemData"))
#define DISCT_AGGR_CACHE_DATA	(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_DisctAggrData"))
#define DISCT_DETAIL_CACHE_DATA	(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_DisctDetailData"))
#define MIDDLE_CACHE_INDEX		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_MiddleIndex"))
#define MIDDLE_INFO_DATA		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_MiddleInfoData"))
#define MIDDLE_CACHE_DATA		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(), "SHM_CACHE_MiddleData"))
#define LOCK_KEY_CACHE_DATA		(IpcKeyMgr::getIpcKey(FORCE_NO,getFlowID(),"SHM_CACHE_LockKeyData"))//��FORCE_YES��ΪFORCE_NO ������������

/***************************************************************************
*	������AccuCacheData
*	���ܣ��¼��ۻ����������ݽṹ����
*   ˵����
*	1���Ʒѷ������ȷ�ϣ�ABM���ܣ��������־˵���ɹ�
*	2���Ʒѷ������ȷ�ϣ�ABMȷ�����ݲ�һ�£����в���ķ��أ�m_lAccuInstID������ֵ��
*		����1�����߶��е���ֵ��һ�£�����ABM����ֵ��������
*		����2��HB������ABM���У�HB��ABM��ֵ����������ABM����ֵ��������
*		����3��HB������ABMû�����ݣ��϶��ɹ�������ʧ��
***************************************************************************/
struct AccuCacheData
{
	long m_lAccuInstID;			//��������HB���ɣ�ABM���ȶ�ʱ���ʧ��ʱ���ش�����������ֵ
	long m_lOfferInstID;		//��Ʒʵ��ID
	long m_lMemberID;			//��Ա��ϸID
	long m_lServID;				//�û�ID
	int  m_iAccuTypeID;			//�ۻ�������ID
	int  m_iBillingCycleID;		//Ĭ�ϴ򿪣������ú����
	long m_lCycleTypeID;		//��������ID
	long m_lBeginDate;			//��ʼʱ�䣬��ʽ��20100813153848
	long m_lEndDate;			//����ʱ�䣬��ʽ��20100813153848
	char m_sSplitby[32];		//�������
	long m_lCycleInstID;		//����ʵ��ID
	long m_lValue;				//ԭֵ
	long m_lAddValue;			//����ֵ
	long m_lABMKeyID;			//ABM����
    long m_lVersionID;			//�汾ID

	//HB��
	int  m_cOperation;			//��ʶ��¼��������('i':������¼,'u':���ϼ�¼���ۻ�,'d': ɾ��,'r'���ο���¼��'o' �ϼ�¼)
	int  m_iFileID;				//file_id
	int  m_iEventSourceType;	//�¼�Դ����
	int  m_iOrgID;				//ORG_ID
	long m_lEventID;			//�¼�ID
	char m_sStateDate[16];			//״̬ʱ��

	unsigned int m_iNextOffset;  //ָ����һ��������
};

/***************************************************************************
*	������DisctCacheData
*	���ܣ��Ż��ۻ������ܻ������ݽṹ����
*   ˵����
*	1���Ʒѷ������ȷ�ϣ�ABM���ܣ��������־˵���ɹ�
*	2���Ʒѷ������ȷ�ϣ�ABMȷ�����ݲ�һ�£����в���ķ��أ�m_lItemDisctSeq������ֵ��
*		����1�����߶��е���ֵ��һ�£�����ABM����ֵ��������
*		����2��HB������ABM���У�HB��ABM��ֵ����������ABM����ֵ��������
*		����3��HB������ABMû�����ݣ��϶��ɹ�������ʧ��
***************************************************************************/
struct DisctCacheData
{
	long m_lItemDisctSeq;		//��������HB���ɣ�ABM���ȶ�ʱ���ʧ��ʱ���ش�����������ֵ
	long m_lOfferInstID;		//��Ʒʵ��ID
	int  m_iBillingCycleID;		//����ID
	int  m_iOrgBillingCycleID;	//ԭ����ID
	int  m_iCombineID;			//�������ID
	int  m_iMeasureID;			//���۶���ID
	int  m_iItemGroupID;		//�Ʒ���Ŀ��ID
	int  m_iInType;				//�����������
	long m_lInValue;			//�������ֵ
	long m_lValue;				//ԭ����ֵ
	long m_lDisctValue;			//ԭ�Ż�ֵ
	long m_lAddValue;			//����ֵ����
	long m_lAddDisctValue;		//�Ż�ֵ����
	long m_lABMKeyID;			//ABM����
    long m_lVersionID;			//�汾ID

	//HB��
	int  m_cOperation;			//��ʶ��¼��������('i':������¼,'u':���ϼ�¼���ۻ�,'d': ɾ��,'r'���ο���¼��'o' �ϼ�¼)
	int  m_iFileID;				//file_id
	int  m_iEventSourceType;	//�¼�Դ����
	int  m_iOrgID;				//ORG_ID

	unsigned int m_iNextOffset;  //ָ����һ��������
	unsigned int m_iDetailOffset;//ָ����ϸ������
};

/***************************************************************************
*	������DisctDetailCacheData
*	���ܣ��Ż��ۻ�����ϸ�������ݽṹ����
***************************************************************************/
struct DisctDetailCacheData
{
	long m_lItemDetailSeq;		//����
	long m_lServID;				//�û�ID
	int  m_iBillingCycleID;		//����ID
	int  m_iAcctItemTypeID;		//��Ŀ����ID
	long m_lValue;				//ԭ����ֵ
	long m_lDisctValue;			//ԭ�Ż�ֵ
	long m_lAddValue;			//����ֵ����
	long m_lAddDisctValue;		//�Ż�ֵ����
	long m_lABMKeyID;			//ABM����
    long m_lVersionID;			//�汾ID

	//HB��
	int  m_cOperation;			//��ʶ��¼��������('i':������¼,'u':���ϼ�¼���ۻ�,'d': ɾ��,'r'���ο���¼��'o' �ϼ�¼)
	long m_lItemDisctSeq;		//�⽡
	long m_lEventID;			//�¼�ID
	int  m_iFileID;				//file_id
	int  m_iEventSourceType;	//�¼�Դ����
	int  m_iOrgID;				//ORG_ID

	unsigned int m_iNextOffset;//ָ����һ��������
};


/***************************************************************************
*	������DisctCache
*	���ܣ��Ż��ۻ�������ṹ��װ
***************************************************************************/
struct DisctAccu
{
	DisctCacheData  m_oDisct;
	vector<DisctDetailCacheData > m_voDetail;
};

/***************************************************************************
*	������EventPriceCacheData
*	���ܣ��嵥���۹켣�������ݽṹ����
***************************************************************************/
struct EventPriceCacheData
{
	EventPriceData m_oData;
	unsigned int m_iNextOffset;//ָ����һ��������
};

/***************************************************************************
*	������AcctItemCacheData
*	���ܣ����ʻ������ݽṹ����
***************************************************************************/
struct AcctItemCacheData
{
	AcctItemAddAggrData m_oData;
	unsigned int m_iNextOffset;//ָ����һ��������
};

//EndFileData ����������ڿ���������⣬�������ҵ�����������Ƿ���Ҫ���漰��ش����߼�

/***************************************************************************
*	������StdEventCacheData
*	���ܣ��¼��������ݽṹ����
***************************************************************************/
struct StdEventCacheData
{
	StdEvent m_oData;
	unsigned int m_iNextOffset;//ָ����һ��������
};

/***************************************************************************
*	������AddressCacheData
*	���ܣ�������������ݵĵ�ַ�������ṹ����
***************************************************************************/
struct AddressCacheData
{
	long		   m_lEventID;//�¼�ID
	unsigned int m_iStdEventOffset;//ָ���¼�������
	unsigned int m_iEventPriceOffset;//ָ���¼��켣������
	unsigned int m_iEventAccuOffset;//ָ���¼��ۻ���������
	unsigned int m_iDisctAggrOffset;//ָ���Ż��ۻ���������
	unsigned int m_iAcctItemOffset;//ָ�����ʵ�����
	unsigned int m_iLockKeyOffset;//ָ�����ؽ��ֵ�����
	long		   m_lBeginDate;//����ʱ�䣬��ʽ��20100813153848
	unsigned int m_iSend;//�Ƿ�����ABM��TYPE_SEND_NO/TYPE_SEND_YES)
	unsigned int m_iType;//����������
	unsigned int m_iProcessID;//����ID
};

/***************************************************************************
*	������MiddleInfoData
*	���ܣ��м���Ϣ��������ݵĽṹ����
***************************************************************************/
struct MiddleInfoData
{
	unsigned int m_iProcessID;//����ID
	unsigned int m_iDataBegOffset;//ָ���������Ŀ�ʼ����
	unsigned int m_iDataEndOffset;//ָ���������Ľ�������
	unsigned int m_iWaitOffset;
};

/***************************************************************************
*	������MiddleCacheData
*	���ܣ��м�������������ݵĽṹ����
***************************************************************************/
struct MiddleCacheData
{
	unsigned int m_iWaitEventOffset;
	unsigned int m_iAddressOffset;//ָ���ַ��������
	unsigned int m_iNextOffset;//ָ����һ��������
	unsigned int m_iOtherOffset;
	unsigned long m_iOffset1;
	unsigned long m_iOtherOffset1;
};

/***************************************************************************
*	������LockKeyData
*	���ܣ������ؽ��ֽṹ����
***************************************************************************/
struct LockKeyData
{
	long m_lKey;//Keyֵ
	unsigned int m_iNextOffset;//ָ����һ��������
	unsigned int m_iOtherOffset;
	unsigned long m_iOffset1;
	unsigned long m_iOtherOffset1;
};

/***************************************************************************
*	������LocalCacheInfo
*	���ܣ�ʵ�ֻ��������ݵĲ�����װ
***************************************************************************/
class LocalCacheInfo
{
public:
	LocalCacheInfo();
	operator bool();

protected:
	void bindData();

	//������������
	static SHMIntHashIndex_A *m_poCacheIndex;

	//�׵�ַ������
	static SHMData_A<AddressCacheData> *m_poAddressCacheData;

	//�׵�ַ������ָ��
	static AddressCacheData *m_poAddressCache;

	//�ȴ��¼�������
	static SHMIntHashIndex_A *m_poEventWaitIndex;

	//�ȴ��¼�������
	static SHMData_A<StdEventCacheData> *m_poEventWaitData;

	//�ȴ��¼�������ָ��
	static StdEventCacheData *m_poEventWait;

	//�����¼�������
	static SHMData_A<StdEvent> *m_poEventCacheData;

	//�����¼�������ָ��
	static StdEvent *m_poEventCache;

	//�¼��켣������
	static SHMData_A<EventPriceCacheData> *m_poEventPriceCacheData;

	//�¼��켣������ָ��
	static EventPriceCacheData *m_poEventPriceCache;

	//�¼��ۻ���������
	static SHMData_A<AccuCacheData> *m_poAccuCacheData;

	//�¼��ۻ���������ָ��
	static AccuCacheData *m_poAccuCache;

	//����������
	static SHMData_A<AcctItemCacheData> *m_poAcctItemCacheData;

	//����������ָ��
	static AcctItemCacheData *m_poAcctItemCache;

	//�Żݻ���������
	static SHMData_A<DisctCacheData> *m_poDisctCacheData;

	//�Żݻ���������ָ��
	static DisctCacheData *m_poDisctCache;

	//�Ż���ϸ������
	static SHMData_A<DisctDetailCacheData> *m_poDisctDetailCacheData;

	//�Ż���ϸ������ָ��
	static DisctDetailCacheData *m_poDisctDetailCache;

	//�м�������
	static SHMIntHashIndex *m_poMiddleIndex;

	//�м���Ϣ������
	static SHMData<MiddleInfoData> *m_poMiddleInfoData;

	//�м���Ϣ������ָ��
	static MiddleInfoData *m_poMiddleInfo;

	//�м�������
	static SHMData_A<MiddleCacheData> *m_poMiddleCacheData;

	//�м�������ָ��
	static MiddleCacheData *m_poMiddleCache;

	//���ؽ���������
	static SHMData_A<LockKeyData> *m_poKeyCacheData;

	//���ؽ���������ָ��
	static LockKeyData *m_poKeyCache;

	//�Ƿ�ɹ����ӹ����ڴ�
	static bool m_bAttached;

private:
	void freeAll();
};

#endif /* _LOCAL_CACHE_INFO_H_ */
