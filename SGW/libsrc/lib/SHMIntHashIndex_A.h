/*VER: 1*/
// Copyright (c) 2009,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef SHMINTHASHINDEX_A_H_HEADER_INCLUDED 
#define SHMINTHASHINDEX_A_H_HEADER_INCLUDED 
#include <memory.h>
#include "SHMAccess.h"


////zhangap ��ѭ��ʹ���������������࣬ʹ��SHMIntList������е�next���������ڴ濪��
////���Ӱ汾��¼������Ѿ��� SHMIntHashIndex��������
//// 

#define ATTACH_INT_INDEX_A(X, Z) \
				if (Z==0) { \
					THROW(MBC_UserInfo+1); \
				} \
        X = new SHMIntHashIndex_A (Z); \
        if (!(X)) { \
            freeAll (); \
            THROW(MBC_UserInfo+1); \
        } \
        if (!(X->exist())) { \
            m_bAttached = false; \
        }

//##ModelId=4244323E0173
class SHMIntList_A
{
    friend class SHMIntHashIndex_A;
public:
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
class SHMIntHashIndex_A : public SHMAccess
{
public:

	class Iteration
	{
	  public:
		bool next(long & key, unsigned int & value)
		{
			unsigned int j;
            
            if(m_iCur>m_iHash){
                return false;
            }
            if( m_iNextOffset !=0
                && 0 != m_pIdx->m_poList[m_iNextOffset].m_iValue){
                
                 if(*m_puiPrePosition!=m_iNextOffset ){
                    m_puiPrePosition=&m_pIdx->m_poList[m_iCurOffset].m_iNext;
                } 
        
                m_iCurOffset = m_iNextOffset;

                value = m_pIdx->m_poList[m_iCurOffset].m_iValue;
                key = m_pIdx->m_poList[m_iCurOffset].m_lKey;
                m_iNextOffset = m_pIdx->m_poList[m_iCurOffset].m_iNext;
                return true;

            }
            if(0==m_iNextOffset){
                
                m_iCur++;
                m_iNextOffset = 0;
                m_iCurOffset = 0;
                m_puiPrePosition=& m_pIdx->m_poHash[m_iCur-1];
                if(m_iCur>m_iHash){
                    return false;
                }
            }
            while( 1 ){
                j=m_pIdx->m_poHash[m_iCur-1];
                m_puiPrePosition=& m_pIdx->m_poHash[m_iCur-1];
                if( 0 == j){
                    m_iCur++;
                    m_iNextOffset = 0;
                    m_iCurOffset = 0;
                    m_puiPrePosition=& m_pIdx->m_poHash[m_iCur-1];
                    if(m_iCur>m_iHash){
                        return false;
                    }
                    continue;
                }
                if(0 == m_iNextOffset )
                    m_iNextOffset = j;
                    
                while( m_iNextOffset != 0 
                    &&( 0 == m_pIdx->m_poList[m_iNextOffset].m_iValue ) ){
                    m_puiPrePosition=& m_pIdx->m_poList[m_iNextOffset].m_iNext;
                    m_iNextOffset = m_pIdx->m_poList[m_iNextOffset].m_iNext;
                    
                }
                
                if(0 == m_iNextOffset ){
                    m_iCur++;
                    m_iNextOffset = 0;
                    m_iCurOffset = 0;
                    m_puiPrePosition=& m_pIdx->m_poHash[m_iCur-1];
                    if(m_iCur>m_iHash){
                        return false;
                    }
                    continue;
                }
                
                break;
                
            }
            m_iCurOffset = m_iNextOffset;
			value = m_pIdx->m_poList[m_iCurOffset].m_iValue;
			key = m_pIdx->m_poList[m_iCurOffset].m_lKey;
			m_iNextOffset = m_pIdx->m_poList[m_iCurOffset].m_iNext;
			return true;		
		}

		Iteration ( unsigned int iHash, SHMIntHashIndex_A * pIdx):
		m_iCur(0), m_iCurOffset(0),m_iNextOffset(0),m_puiPrePosition(0), m_iHash(iHash), m_pIdx(pIdx)
		{}

		Iteration (const Iteration & right)
		{
			m_iCur = right.m_iCur;
			m_iHash = right.m_iHash;
			m_iCurOffset = right.m_iCurOffset;
			m_iNextOffset = right.m_iNextOffset;
			m_puiPrePosition = right.m_puiPrePosition;
			m_pIdx = right.m_pIdx;
		}

        bool setValue( unsigned int iValue ){
            if( m_iCurOffset ){
                m_pIdx->m_poList[m_iCurOffset].m_iValue = iValue;
                if(0 == iValue){
                    return removeMe();
                }
                return true;
            }
            return false;
        }
        
        bool removeMe(){
            ////ʹ�õ�������ɾ�����ڵ�
            SHMIntList_A * m_poList = m_pIdx->m_poList;
            unsigned int * m_piUsed = m_pIdx->m_piUsed;
            unsigned int * m_piTotal = m_pIdx->m_piTotal;
            unsigned int * m_piTail = m_pIdx->m_piTail;
            unsigned int * m_piHead = m_pIdx->m_piHead;


            if(m_puiPrePosition && (*m_puiPrePosition == m_iCurOffset) ){
                *m_puiPrePosition = m_iNextOffset;
                
                //value = m_poList[m_iCurOffset].m_iValue;
                m_poList[m_iCurOffset].m_lKey = 0;
                m_poList[m_iCurOffset].m_iValue = 0;
                
                *m_puiPrePosition = m_poList[m_iCurOffset].m_iNext;
                m_poList[m_iCurOffset].m_iNext = 0;
                
                if( *m_piUsed <= *m_piTotal )//�����ռ�δ����
                    m_poList[*m_piTail].m_iNext = m_iCurOffset;
                else
                    *m_piHead = m_iCurOffset;
    
                *m_piTail = m_iCurOffset;
                (*m_piUsed)--;
                return true;
            }
            return false;
            
        }
	  private:
		unsigned int m_iCur;
		unsigned int m_iHash;
		unsigned int m_iCurOffset;
		unsigned int m_iNextOffset;
		unsigned int * m_puiPrePosition;
		
		SHMIntHashIndex_A * m_pIdx;
	};
	
	    
  public:
    Iteration getIteration ()
	{
		return Iteration ( (*m_piHashValue)+1, this);
	}
	
	friend class Iteration;
	
    //##ModelId=424426D6039B
    SHMIntHashIndex_A(char *shmname);

    //##ModelId=424426D603B9
    SHMIntHashIndex_A(long shmkey);

    //##ModelId=424426D603D7
    ~SHMIntHashIndex_A();

    //##ModelId=42442A29038B
    void add(long key, unsigned int value);

    //##ModelId=42442A5B0134
    bool get(long key, unsigned int *pvalue);

    bool revokeIdx(long key, unsigned int &value);
    void reset();  
    unsigned int malloc();
    void showDetail();
    
    //##ModelId=42442B02030B
    //##Documentation
    //## ����������
    void create(
        //##Documentation
        //## ���Ԫ�ظ���
        unsigned int itemnumber) ;
    
    //##���������Ϊ����ҵ���ڴ潻��ʹ��
    void empty();
    
    unsigned int getMemSize();

	int getUsedSize();
			
	unsigned long getMemAdress();
	
	time_t getLoadTime();
	

    unsigned int getCount()
    {
        if (*m_piUsed)
            return (*m_piUsed)-1;

        return 0;
    };

    unsigned int getTotal()
    {
        if (*m_piTotal)
            return (*m_piTotal);

        return 0;
    };


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

    //##ModelId=42442FA101B7
    unsigned int *m_poHash;

    //##ModelId=424432820167
    SHMIntList_A *m_poList;

    unsigned int *m_piHead;//ָ��ɷ�������ͷ��㣬ÿ����Ҫ����һ�������ͷ��ȡ
    unsigned int *m_piTail;//ָ��ɷ�������β�ӵ㣬ÿ��ɾ��һ������ӵ�β��
    unsigned int *m_piVersion; ////�ڴ�ṹ�İ汾�� ��3
    unsigned int *m_piDelOver; 
};




#endif  
