/*VER: 2*/ 
#ifndef DATA_OFFER_MGR_H
#define DATA_OFFER_MGR_H

#include <map>
using  namespace std;

class DataProductOffer
{
	public:
	 	DataProductOffer():m_iControlID(0),m_iOfferID(0),m_iOfferDetailID(0),
	 					   m_iRefID(0),m_lThresholdValue(0),m_iStopType(0),
	 					   m_iOffsetCycleEnd(0),m_iRemindOfferDetailID(0),m_iOrgID(0)
	 	{
	 		memset(m_sRemark, 0, sizeof(m_sRemark));
	 		memset(m_sThresholdUnitName, 0, sizeof(m_sThresholdUnitName));
	 		memset(m_sJudgeInstStartDate, 0, sizeof(m_sJudgeInstStartDate));
	 	}
	 	
	 	~DataProductOffer()
	 	{
	 	}
	 	
	 	//���кţ�PK��
    	int m_iControlID;	
   		//��Ʒ����
		int m_iOfferID;
		//ͣ������ID;
		int m_iOfferDetailID;	
		//�ж϶��󣬶�Ӧ���ۻ���
		int m_iRefID;	
		//ͣ���ż�ֵ
		long m_lThresholdValue;
		//ͣ���������ͣ�ֱ��д�빤����
		int m_iStopType;
		//����ĩƫ��ʱ�䣨���ӣ�������ʱ��������ĩƫ��ʱ����
		int m_iOffsetCycleEnd;
		//�������Ѷ���ID���������п��ǣ�
		int m_iRemindOfferDetailID;
		//�������ѹ�������
		int m_iRemindType;
		//ע��
		char m_sRemark[101];
		//������ʶ���·��ã�
		int m_iOrgID;	
		//��λ���� add 2009.11.20
		char m_sThresholdUnitName[21];
		//��ʱ�������Ч���ײ��û����������ſ�
		char m_sJudgeInstStartDate[15];
};

typedef map<int, DataProductOffer *> DATA_OFFER_DETAIL_MAP;
typedef map<int, DATA_OFFER_DETAIL_MAP *> DATA_OFFER_MAP;

class OfferInsQO;

class DataProductOfferMgr
{
public:
    DataProductOfferMgr(); 
    ~DataProductOfferMgr();
    	
    void load();
    void unload();
    
public:
	//Ĭ�ϱ����ж�detail_id
	DataProductOffer * getInfoByOfferInst(OfferInsQO *opOfferInst);

    DataProductOffer * getInfoByOfferID(int iOfferID);
    DataProductOffer * getInfoByOfferID(int iOfferID, char *sInstEffDate);
    
    DataProductOffer * getInfoByOfferDetailID(int iOfferID, int iOfferDetailID);
    DataProductOffer * getInfoByOfferDetailID(int iOfferID, int iOfferDetailID, char *sInstEffDate);
    
    bool isDataOfferInst(int iOfferID);
    bool isDataOfferInst(int iOfferID, char *sInstEffDate);
    
    bool isDataOfferInstDetail(int iOfferID, int iOfferDetailID);
    bool isDataOfferInstDetail(int iOfferID, int iOfferDetailID, char *sInstEffDate);

private:
    DATA_OFFER_MAP m_poOfferInfoMap;
    DATA_OFFER_MAP::iterator m_pOfferIter;

    DATA_OFFER_DETAIL_MAP::iterator m_pOfferDetailIter;    	
    
    static bool m_bUploaded;
    
    bool m_bJudgeInstEffDate;
    
    //bool m_bJudgeInstDetail;Ĭ�ϱ����ж�detail_id
};

#endif
