/*VER: 1*/
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.
// VER: 1.1  huff 
// �޸� 1 �� erase, ɾ��ʱ ���������������Ķ�Ӧ��ϵ����
//        2.   add    ��������������
//        3.   CycleSHMIntList ȥ�� iIdleNext �ɿ��ж��п��� 

#ifndef RERecycleSHMIntHashIndex_H_HEADER_INCLUDED_BDBB0F0A
#define RERecycleSHMIntHashIndex_H_HEADER_INCLUDED_BDBB0F0A
#include <memory.h>
#include "SHMAccess.h"

#define Cycle_ATTACH_INT_INDEX(X, Z) \
    X = new ReRecycleSHMIntHashIndex (Z); \
    if (!(X)) { \
    freeAll (); \
    THROW(MBC_UserInfo+1); \
    } \
    if (!(X->exist())) { \
    m_bAttached = false; \
    }

//##ModelId=4244323E0173
class RecycleSHMIntList
{
    friend class RecycleSHMIntHashIndex;

    //##ModelId=424432520367
    long m_lKey;

    //##ModelId=4244325B026F
    unsigned int m_iValue;

    //##ModelId=424432640377
    unsigned int m_iNext;

};

//##ModelId=424423010070
//##Documentation
//## KeyΪ�����Ĺ����ڴ�����
//## ����HASHLIST�㷨
class ReRecycleSHMIntHashIndex : public SHMAccess
{
public:
    //##ModelId=424426D6039B
    RecycleSHMIntHashIndex(char *shmname);

    //##ModelId=424426D603B9
    RecycleSHMIntHashIndex(long shmkey);

    //##ModelId=424426D603D7
    ~RecycleSHMIntHashIndex();

    //##ModelId=42442A29038B
    void add(long key, unsigned int value);

    //##ModelId=42442A5B0134
    bool get(long key, unsigned int *pvalue);

    bool erase(long key, unsigned int &value);

    void reset();

    //##ModelId=42442B02030B
    //##Documentation
    //## ����������
    void create( 
        //##Documentation
        //## ���Ԫ�ظ���
        unsigned int itemnumber) ;

    //##���������Ϊ����ҵ���ڴ潻��ʹ��
    void empty();

    //modify by jinx for app_guard 070402
    unsigned int getCount()
    {
        if (*m_piUsed)
            return (*m_piUsed);

        return 0;
    };

    unsigned int getTotal()
    {
        if (*m_piTotal)
            return (*m_piTotal);

        return 0;
    };

    long getValue(unsigned int key);

private:
    //##ModelId=4244D04101A3
    void open();

    //##ModelId=42442E01039A
    //##Documentation
    //## hash��ɢ��
    unsigned int *m_piHashValue;

    //##ModelId=42442F710136
    unsigned int *m_piTotal;

    //##ModelId=42442F8A02FF
    unsigned int *m_piUsed;

    unsigned int *m_piHead;//ͷ���
    unsigned int *m_piTail;//β�ӵ�

    //##ModelId=42442FA101B7
    unsigned int *m_poHash;

    //##ModelId=424432820167
    RecycleSHMIntList *m_poList;

};




#endif /* RecycleSHMIntHashIndex_H_HEADER_INCLUDED_BDBB0F0A */
