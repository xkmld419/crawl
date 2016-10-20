/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef SHMMIXINDEX_H_HEADER_INCLUDED_BDBB74AD
#define SHMMIXINDEX_H_HEADER_INCLUDED_BDBB74AD
#include "SHMAccess.h"
class SimpleSHM;

#define MAX_MIX_TYPE_LEN 	5

#define ATTACH_MIX_INDEX(X, Z) \
        X = new SHMMixHashIndex (Z); \
        if (!(X)) { \
            freeAll (); \
            THROW(MBC_UserInfo+1); \
        } \
        if (!(X->exist())) { \
            m_bAttached = false; \
        }


//## ����Ҫ�����ڴ�8���ֽڶ���
class SHMMixStringList
{
public:
    unsigned int m_iValue;
    unsigned int m_iNext;
    //
    char m_sKey[8];
};

//
class SHMMixStringTree
{
public:
	//TREE��10��ָ��
    unsigned int m_iNext[10];
    unsigned int m_iValue;
};

//
class SHMMixTypeInfo
{
public:
	char m_sType[MAX_MIX_TYPE_LEN];
    unsigned int m_iKeyLen;
};

//## long�Լ��ַ����͹����ڴ���������Ҫ�����ڴ��л�ʹ�õ�������
//## �������KeyTree�㷨���ܺĿռ�,һ��û�г���10^MIX_STRING_KEY_LEN�η���������,����TREE�ṹ����Ŀ�ݲ�����
//## ����������ʱ�򣬽���ʹ��SHMMixHashIndex��
class SHMMixHashIndex : public SHMAccess
{
  public:
    //
	SHMMixHashIndex(char *shmname);
	SHMMixHashIndex(long shmkey);
	SHMMixHashIndex(unsigned int iType,char *pAddr);
	SHMMixHashIndex(char *sType,char *pAddr);
	
	void add(long lKey,unsigned int value);
	bool get(long lKey, unsigned int *pvalue);
	char *m_sMixKey;
    //##ModelId=424429AC001A
    ~SHMMixHashIndex();

    //##ModelId=42442A8B0093
    void add(char *key, unsigned int value);

    //##ModelId=42442A9101C8
    bool get(char *key, unsigned int *pvalue);

    bool getMax(char *key, unsigned int *pvalue);
	
    //## ����������
    void create(
        //## ���Ԫ�ظ���
        unsigned int itemnumber, 
        //## key����󳤶� ,����һ��Ĭ��ֵ��ҪΪlong���͵�����׼����
        unsigned int keylen=18, 
        //## keytree�Ľڵ�����Ԫ����Ŀ�ı���,���ֵ���ڴ����ò������ص�ʱ������Ҫ�� ����Խ��ֵԽ��
        unsigned int multiple = 3,unsigned int keyAddLen = 3,unsigned int reuseNum = 0, bool VarRay = false) ;
    void empty();
	//modify by jinx for app_guard 070402
	//��Щ���ṩ�ܵ��ڴ�ʹ�����
	unsigned int getTCount();
	unsigned int getTTotal();
	unsigned int getlCount();
	unsigned int getlTotal();
	
	//��������ṩ��Ӧm_iType���ݻ�ȡ�����Լ�ʵ��ʹ�����ĺ���
	unsigned int getCount(bool ReInitCount=false);
	unsigned int getTotal();
	
	unsigned int getType()
	{
		return m_iType;
	}
	bool typeExist();
public://ʵ�鹦��
	//
	bool removeIndex(char *key); //���ն����ṩ�ĺ���
	// ������
	unsigned int m_iErrNo;
	char m_sErrDesc[128];
	
	//���ù��� ���븴����Ч�����ݿռ�
	unsigned int ReuseMalloc();
	//���ò���
	unsigned int  m_iMultiple;//�Ժ���Ϊ��ʱ����
	unsigned int  m_iReuseListTotalNum;//�Ժ���Ϊ��ʱ����
	unsigned int  m_iReuseTreeTotalNum;//��ʱ����
	// ��ȡһ�����õ�LIST�ڵ�
	SHMMixStringList *getList();
	// ��ȡһ�����õ�TREE�ڵ�
	SHMMixStringTree *getTree();
	// ������Ը��õ�TREE��ַ
	unsigned int MallocReuseTree();
	// ������Ը��õ�list��ַ
	unsigned int MallocReuseList();
	// 
	bool addReuseList(unsigned int iListOffset);
	bool addReuseTree(unsigned int iTreeOffset);
	// 
	bool deleteList(unsigned int iListOffset);
	bool deleteTree(unsigned int iTreeOffset);
	SHMMixStringTree *getTree(unsigned int offset);
  private:
  	char *m_pAddr;//��ַ
  	//
	char *m_sType;//�Ժ����Ϊ��
	unsigned int  m_iType;//�Ժ���Ϊ��ʱ����
	
	unsigned int  m_iCount;// ��������Ч�ʵ�
	bool m_bInitCount;//��m_iCountһ��ʹ��
    //
    void open(char *pAddr = 0);
    //
    inline SHMMixStringList *getList(unsigned int offset);
	
	
    //
    unsigned int *m_piListTotal;

    //##ModelId=4244332E0088
    unsigned int *m_piListUsed;

    //##ModelId=424433510223
    unsigned int *m_piTreeTotal;

    //##ModelId=4244335F02B9
    unsigned int *m_piTreeUsed;

    //## SHMStringList����������
    unsigned int *m_piListLen;
	
	SHMMixTypeInfo *m_poTypeInfo;//��ʱδʹ��
	unsigned int *m_piTypNum;//֧�ֱ䳤�����������Ŀ //��ʱδʹ��
	
	//���ù���
	unsigned int *m_piReuseList;//��¼���Ը��õ�LIST��ƫ��
	//unsigned int *m_piListCurAdd;//�������ȡ����
	unsigned int *m_piListCurGet;
	unsigned int *m_piReuseListCanUse;
	unsigned int *m_piReuseListTotal;
	//
	unsigned int *m_piReuseTree;
	//unsigned int *m_piTreeCurAdd;//�������ȡ����
	unsigned int *m_piTreeCurGet;
	unsigned int *m_piReuseTreeCanUse;
	unsigned int *m_piReuseTreeTotal;
    //
    char *m_poList;

    //
    SHMMixStringTree *m_poTree;
    //
    unsigned int *m_piKeyLen;


};


#endif /*  */
