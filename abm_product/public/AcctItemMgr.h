/*VER: 3*/ 
#ifndef ACCTITEMMGR_H_HEADER_INCLUDED_BDB75C9B
#define ACCTITEMMGR_H_HEADER_INCLUDED_BDB75C9B

#include "CommonMacro.h"
#include "HashList.h"
class SimpleMatrix;
class ParamInfoMgr;

/* 20050619 ��Ӷ�B_ACCT_ITEM_GROUP_MEMBER�Ĺ��� */
//##ModelId=42639ED800D7
class AcctItem
{
    friend class AcctItemMgr;
    friend class AcctItemLoad;
    friend class ParamInfoMgr;
    friend class AssPriData;    
  public:
    //##ModelId=42639F2B03A7
    AcctItem();

  private:
    //##ModelId=42639EE1018E
    int m_iAcctItemID;

    //##ModelId=42639EE8026A
    int m_iParentID;
		
		int m_iPartyRoleID;
		
    char m_sAcctItemCode[20];

    //##ModelId=42639EFF02A9
    AcctItem *m_poParent;

    AcctItem *m_poOther;

    int m_iOffset;
#ifndef PRIVATE_MEMORY
    unsigned int m_iParentOffset;   //����Ŀ�����������е�ƫ����
    unsigned int m_iOtherOffset;    //ͬһ A �� B��Ŀ�����������е�ƫ����
#endif

};

struct AcctItemChild
{
	int m_iAcctItemID;
	int m_iChildNum;
	int m_aoChildItemID[MAX_ACCTITEM_PERGROUP];
};

struct ItemGroupMemberB
{
	int m_iItemObjectID;
	int m_iObjectType;
	int m_iExcludeFlag;
	ItemGroupMemberB * m_poNext;
};

struct ItemGroupMember {
    int m_iItemTypeID;
    ItemGroupMember * m_poNext;
#ifndef PRIVATE_MEMORY
    int m_iItemGroupID;
    unsigned int m_iNextOffset;
#endif
};

//##ModelId=4248E6290090
//##Documentation
//## ��Ŀ���͹���
class AcctItemMgr
{
  public:
    //##ModelId=4248E6400256
    void load();

    //##ModelId=4248E64302A0
    void unload();

    //##ModelId=4248E65203D8
    bool getBelongGroup(int acctitem, int acctitemgroup );

    //�Ʒ��Լ�����Ŀ��
    bool getBelongGroupB(int acctitem, int bacctitemgroup );

    bool getBelongGroupB(int acctitem, int bacctitemgroup, bool bDiscount);
    bool getBelongGroupB(int acctitem, int bacctitemgroup, ParamInfoMgr* pParamInfoMgr);

    //##ModelId=4248E9C003B2
    AcctItemMgr();
	AcctItemMgr(int iMode);


    //##ModelId=4248E9D20065
    ~AcctItemMgr();

    //##ModelId=42639DC8008F
    bool getBelong(int iChildItem, int iParentItem);

    //##ModelId=42639E4800B1
    int getParent(int iChildItem);

    //##ModelId=42841D1B02B4
    //##Documentation
    //## ������Ŀ���ͣ���ȡ��Ӧ���Żݵ���Ŀ���ͣ�
    //## �磺�����Ѷ�Ӧ��Ϊ�����Żݷ�
    int getDisctItem(int iNormalItemType);

    bool isDisctItem(int iAcctItemType);

		bool isPartyRoleItem(int iAcctItemType,int iPartyRoleID);
		
    int *getAcctItems(int iAcctItemGroupID, int *number = 0);

    //�Ʒ��Լ�����Ŀ��
    int *getAcctItemsB(int iBAcctItemGroupID, int * number = 0);

    //���ظ���Ŀ�������еĻ������õ���Ŀ��
    int *getBaseAcctItemsB(int iBAcctItemGroupID, int * number = 0);

    //##ModelId=42841D350352
    //##Documentation
    //## �����Ż���Ŀ���ͣ������ȡ��Ӧ����Ŀ����
    //## �磬
    //## ���ݳ����Żݷѻ�ȡ���Żݷ���Ŀ����
    int reverseDisctItem(int iDisctItemType);

  private:
    //##ModelId=42639E8800F0
    void loadGroup();

    //�Ʒ��Լ�����Ŀ��
    void loadGroupB();

    //##ModelId=42639E8E02E3
    void loadItem();

	void loadGroupMeberB();
	void loadBasicGroupMeberB();
	void analyzeItem();
	int getItemChild(int iAcctItemID,int * pItemChild);
	int getGroupDetail(long lGroupID);

    //##ModelId=4248E8FC0306
    static HashList<int> *m_poItemSerial;

    //##ModelId=4248E92401D7
    static HashList<int> *m_poGroupSerial;

    //�Ʒ��Լ�����Ŀ��
    static HashList<int> *m_poGroupSerialB;

    //##ModelId=4248E8DF0160
    static SimpleMatrix *m_poMatrix;

    //�Ʒ��Լ�����Ŀ��
    static SimpleMatrix *m_poMatrixB;

    //##ModelId=42639EA40168
    static bool m_bUploaded;

    //##ModelId=42639F8F036F
    static HashList<AcctItem *> *m_poIndex;

    //##ModelId=42639F53020A
    static AcctItem *m_poAcctItemList;

    static int m_iCount;
    
	static int * m_piIncludeDiscount;
    static ItemGroupMember ** m_pBasicMember;
    static ItemGroupMember ** m_pDisctMember;

	/**********************************************/
	static HashList<int> * m_poItemParentIndex;
	static ItemGroupMember * m_poItemParentList;

	static HashList<int> * m_poItemChildIndex;
	static AcctItemChild * m_poItemChildList;
	
	static HashList<int> * m_poGroupMemberBIndex;
	static ItemGroupMemberB * m_poGroupMebmerBList;

	static HashList<int> * m_poBasicGroupMemberBIndex;
	static ItemGroupMemberB * m_poBasicGroupMebmerBList;

	HashList<int> * m_poItemTemp;
	
	HashList<int> * m_poYes;
	HashList<int> * m_poNo;
	HashList<int> * m_poChildYes;
	HashList<int> * m_poChildNo;
    /***********************************************/

	int m_aoAcctItemID[MAX_ACCTITEM_PERGROUP];
	int m_iIndex;

};


#endif /* ACCTITEMMGR_H_HEADER_INCLUDED_BDB75C9B */
