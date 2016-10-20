/*VER: 1*/ 
/*
**
**
**
**  2007.10.17 xueqt 
**      ����loadFreeOffer; isFreeOffer; m_poFreeOfferIndex
*/

#include "CommonMacro.h"
#include "HashList.h"
#ifndef OFFERMGR_H_HEADER_INCLUDED_BDB745F9
#define OFFERMGR_H_HEADER_INCLUDED_BDB745F9
enum OfferType
{
    OFFER_TYPE_BASE = 0,
    OFFER_TYPE_DISCT
};

class OfferAttr
{
	public:
		int m_iAttrId;
		char m_sAttrValue[32];
		OfferAttr *m_poNext;
};
      
class Offer
{
public:
    //��ƷID
    int m_iOfferID;
    //���ۼƻ���ʾ
    int m_iPricingPlan;
    //���ȼ�
    int m_iPriority;
    //��Ʒ���ͣ��������ײ�
    OfferType m_iOfferType;
    //��Ч����
    char m_sEffDate[16];
    //ʧЧ����
    char m_sExpDate[16];
    //Ʒ����Ϣ
    int m_iBrandID;
    //��Ʒ����
    OfferAttr *m_poOfferAttr;
		//add by zhaoziwei
		//��ƷЭ��������
		int m_iAgreementLength;
		//end
};
class OfferMgr
{
  //��Ʒ
  Offer * m_poOfferInfoList;

  //��Ʒ�洢������
  HashList<Offer *> *m_poOfferInfoIndex;
  
  //��Ʒ����
  OfferAttr *m_poOfferAttrList;
  
  //��Ʒ��������
  HashList<OfferAttr *> *m_poOfferAttrIndex;
    
  HashList<int> * m_poFreeOfferIndex;

	HashList<int>  * m_poSpeicalOfferAttrIndex;
    
	HashList<int>  * m_poNumberOfferAttrIndex;
	
  static bool m_bUploaded;
    
public:
    //## ���ض���Ʒ�������������
  void load();
    
  void loadFreeOffer();

	//������������Ϊ15������ID������ֵ��Ӧ�й�ϵ
	void loadSpecialAttr();

	//���������͵���Ʒ����
	void loadNumberAttr();

    //## ж�������ص���Ϣ
  void unload();
    
  Offer * getOfferInfoByID(int iOfferID);
    
  bool isFreeOffer(int iOfferID);

	bool checkSpecialAttr(int iAttrID,int &iOperation);

	bool checkNumberAttr(int iAttrID);
	
	char * getOfferAttrValue(int iOfferId,int iAttrId,char *sDate);
 
	bool getbrand(int iOfferId,int &ibrandid );    
    
  OfferMgr();
    
  ~OfferMgr();

private:
	int m_iFlag;
	
	int m_iUsedDefaultAttr;
};
#endif
