/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef _SHMSTRINGTREEINDEX_H_HEADER_INCLUDED_
#define _SHMSTRINGTREEINDEX_H_HEADER_INCLUDED_
#include "SHMAccess.h"

#define ATTACH_STRING_INDEX(X, Z) \
        X = new SHMStringTreeIndex (Z); \
        if (!(X)) { \
            freeAll (); \
            THROW(MBC_UserInfo+1); \
        } \
        if (!(X->exist())) { \
            m_bAttached = false; \
        }



//##ModelId=424431C703DF
//##Documentation
//## ����Ҫ�����ڴ�8���ֽڶ���
class SHMStringListEx
{
    friend class SHMStringTreeIndexEx;

    //##ModelId=424431FD01A2
    unsigned int m_iValue;

    //##ModelId=4244320B00DA
    unsigned int m_iNext;

    //##ModelId=424431EF02EC
    char m_sKey[8];

};

//##ModelId=424433A902A2
class SHMStringTreeEx
{
    friend class SHMStringTreeIndexEx;

    //##ModelId=424433B603B9
    unsigned int m_iNext[36];

    //##ModelId=424433C400B6
    unsigned int m_iValue;

};

//##ModelId=4244298B0031
//##Documentation
//## �ַ����͹����ڴ���������Ҫ���ڶ���ҵ��������������
//## �������KeyTree�㷨���ܺĿռ䣬Ԥ���û�������800��
//## ��ʱ�򣬽���ʹ��sHMStringHashIndex��
class SHMStringTreeIndexEx : public SHMAccess
{
  public:
    //##ModelId=424429AB039E
    SHMStringTreeIndexEx(char *shmname);

    //##ModelId=424429AB03BC
    SHMStringTreeIndexEx(long shmkey);

    //##ModelId=424429AC001A
    ~SHMStringTreeIndexEx();

    //##ModelId=42442A8B0093
    void add(char *key, unsigned int value);

    //##ModelId=42442A9101C8
    bool get(char *key, unsigned int *pvalue);

    bool getMax(char *key, unsigned int *pvalue);

    //##ModelId=42442B0802C3
    //##Documentation
    //## ����������
    void create(
        //##Documentation
        //## ���Ԫ�ظ���
        unsigned int itemnumber, 
        //##Documentation
        //## key����󳤶�
        unsigned int keylen, 
        //##Documentation
        //## keytree�Ľڵ�����Ԫ����Ŀ�ı���
        unsigned int multiple = 3) ;
    void empty();
//modify by jinx for app_guard 070402
	unsigned int getCount()
	{
		if (*m_piTreeUsed)
			return (*m_piTreeUsed);

		return 0;
	};

	unsigned int getTotal()
	{
		if (*m_piTreeTotal)
			return (*m_piTreeTotal);

		return 0;
	};

	unsigned int getlCount()
	{
		if (*m_piListUsed)
			return (*m_piListUsed);

		return 0;		
	};

	unsigned int getlTotal()
	{
		if (*m_piListTotal)
			return (*m_piListTotal);

		return 0;
	};
	
  private:
    //##ModelId=4244EBC6017B
    void open();
    //##ModelId=424512FC000C
    inline SHMStringListEx *getList(unsigned int offset);


    //##ModelId=4244331C0050
    unsigned int *m_piListTotal;

    //##ModelId=4244332E0088
    unsigned int *m_piListUsed;

    //##ModelId=424433510223
    unsigned int *m_piTreeTotal;

    //##ModelId=4244335F02B9
    unsigned int *m_piTreeUsed;

    //##ModelId=42443696006E
    //##Documentation
    //## SHMStringList����������
    unsigned int *m_piListLen;

    //##ModelId=424432C50326
    char *m_poList;

    //##ModelId=424433EE034B
    SHMStringTreeEx *m_poTree;
    //##ModelId=424501EC00DE
    unsigned int *m_piKeyLen;


};


#endif /* _SHMSTRINGTREEINDEX_H_HEADER_INCLUDED_ */
