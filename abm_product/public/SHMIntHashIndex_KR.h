/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef SHMINTHASHINDEX_KEYREUSE_H_HEADER_INCLUDED
#define SHMINTHASHINDEX_KEYREUSE_H_HEADER_INCLUDED
#include <memory.h>
#include "SHMAccess.h"
#include "SHMIntHashIndex.h"




#define ATTACH_INT_INDEX_KR(X, Z) \
        X = new SHMIntHashIndex_KR (Z); \
        if (!(X)) { \
            freeAll (); \
            THROW(MBC_UserInfo+1); \
        } \
        if (!(X->exist())) { \
            m_bAttached = false; \
        }

////zhangap
//////// KR : Key Reuse ,key���ð汾
/////////���keyҪ���õĻ���������ִ��add����ʱ�õ����ź����������������Ŀ��ܸ��ǵ�ǰ���
const long SHM_INT_KEY_REUSE = -12345L;  ////keyΪ-12345��ʾ���key��������
////��add һ��key��value Ϊ0��ʾ���ɾ����,����ʱ���ٷ�����
////������ƣ����ܱ�֤100%�ܱ����ã����ǿ��Ա�֤������������󲻳���(*m_piHashValue)/total*100%
////���ĺô��ǿ��Ժ�ԭ����SHMIntHashIndex������ҵ����100%����
////Ҳ����˵��ҵ��������Χ�ڣ��������汾����ֱ�ӻ���ʹ��


//##ModelId=4244323E0173
class SHMIntList_KR
{
    friend class SHMIntHashIndex_KR;
    
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
class SHMIntHashIndex_KR : public SHMAccess
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
                && SHM_INT_KEY_REUSE != m_pIdx->m_poList[m_iNextOffset].m_lKey
                && 0 != m_pIdx->m_poList[m_iNextOffset].m_iValue){
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
                if(m_iCur>m_iHash){
                    return false;
                }
            }
            while( 1 ){
                j=m_pIdx->m_poHash[m_iCur-1];
                if( 0 == j){
                    m_iCur++;
                    m_iNextOffset = 0;
                    m_iCurOffset = 0;
                    if(m_iCur>m_iHash){
                        return false;
                    }
                    continue;
                }
                if(0 == m_iNextOffset )
                    m_iNextOffset = j;
                    
                while( m_iNextOffset != 0 
                    &&( SHM_INT_KEY_REUSE == m_pIdx->m_poList[m_iNextOffset].m_lKey
                        || 0 == m_pIdx->m_poList[m_iNextOffset].m_iValue ) ){
                    m_iNextOffset = m_pIdx->m_poList[m_iNextOffset].m_iNext;
                }
                
                if(0 == m_iNextOffset ){
                    m_iCur++;
                    m_iNextOffset = 0;
                    m_iCurOffset = 0;
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

		Iteration ( unsigned int iHash, SHMIntHashIndex_KR * pIdx):
		m_iCur(0), m_iCurOffset(0),m_iNextOffset(0), m_iHash(iHash), m_pIdx(pIdx)
		{}

		Iteration (const Iteration & right)
		{
			m_iCur = right.m_iCur;
			m_iHash = right.m_iHash;
			m_iCurOffset = right.m_iCurOffset;
			m_iNextOffset = right.m_iNextOffset;
			m_pIdx = right.m_pIdx;
		}

        bool setValue( unsigned int iValue ){
            if( m_iCurOffset ){
                m_pIdx->m_poList[m_iCurOffset].m_iValue = iValue;
                if(0 == iValue){
                    m_pIdx->m_poList[m_iCurOffset].m_lKey = SHM_INT_KEY_REUSE;
                }
                return true;
            }
            return false;
        }
        
        bool removeMe(){
            if( m_iCurOffset ){
                m_pIdx->m_poList[m_iCurOffset].m_iValue = 0;
                m_pIdx->m_poList[m_iCurOffset].m_lKey = SHM_INT_KEY_REUSE;
                return true;
            }
            return false;
        }
        
	  private:
		unsigned int m_iCur;
		unsigned int m_iHash;
		unsigned int m_iCurOffset;
		unsigned int m_iNextOffset;
		
		SHMIntHashIndex_KR * m_pIdx;
	};
	
	Iteration getIteration ()
	{
		return Iteration ( (*m_piHashValue)+1, this);
	}


	friend class Iteration;
	
    //##ModelId=424426D6039B
    SHMIntHashIndex_KR(char *shmname);

    //##ModelId=424426D603B9
    SHMIntHashIndex_KR(long shmkey);

    //##ModelId=424426D603D7
    ~SHMIntHashIndex_KR();

    //##ModelId=42442A29038B
    void add(long key, unsigned int value);

    //##ModelId=42442A5B0134
    bool get(long key, unsigned int *pvalue);

	bool eraseData(const long key);
	bool revokeIdx(const long key, unsigned int &value );

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
			return (*m_piUsed)-1;

		return 0;
	};

	unsigned int getTotal()
	{
		if (*m_piTotal)
			return (*m_piTotal);

		return 0;
	};
	unsigned int getMemSize();

	unsigned int getUsedSize();
			
	unsigned long getMemAdress();
	
	time_t getLoadTime();
	

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
    SHMIntList_KR *m_poList;

};




#endif /* SHMINTHASHINDEX_H_HEADER_INCLUDED_BDBB0F0A */
