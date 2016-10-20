/*VER: 1*/
// Copyright (c) 2009,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef SHMINTHASHINDEX_AX_H_HEADER 
#define SHMINTHASHINDEX_AX_H_HEADER 
#include <memory.h>
#include "SHMAccess.h"
#include "MBC.h"
#include "CSemaphore.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

//// ��ѭ��ʹ���������������࣬ʹ��SHMIntList������е�next���������ڴ濪��
////���Ӱ汾��¼������Ѿ��� SHMIntHashIndex��������
////��Ϊ�����ڴ���չ��ɾ�����õȲ�������Ҫ������������Ϣ�������Ҫ������������������ʹ��ʱ�ᵼ�����ݻ���
////
//// �Ű�Ʒ

#define ATTACH_INT_INDEX_AX(X, Z) \
        X = new SHMIntHashIndex_AX (Z); \
        if (!(X)) { \
            freeAll (); \
            THROW(MBC_UserInfo+1); \
        } \
        if (!(X->exist())) { \
            m_bAttached = false; \
        }

//##ModelId=4244323E0173
class SHMIntList_AX
{
    friend class SHMIntHashIndex_AX;
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
class SHMIntHashIndex_AX : public SHMAccess
{
public:

class IntListExpand_AX{
    public:
        IntListExpand_AX( ){m_pShm = 0;};
        SHMIntList_AX &  operator [](unsigned int idx){
            SHMIntList_AX *pBase = (SHMIntList_AX *)(m_pShm->m_sSegment[0]);

                
            if( idx <= (*m_pShm->m_piSeg)  ){
                return  *(pBase+idx) ;
            }

            for(int i=1; i<MAX_SHM_EX_CNT+1;i++){
                if( idx <= (*(m_pShm->m_piSeg+i)) ){
                    pBase = (SHMIntList_AX *)(m_pShm->m_sSegment[i]);
                    idx -= (*(m_pShm->m_piSeg+i-1)) ;
                    return  *(pBase+idx-1) ;
                }

            }
            THROW(MBC_SHMLruData+2);
        };

        SHMIntHashIndex_AX * m_pShm;

};

	class Iteration
	{
	  public:
		bool next(long & key, unsigned int & value)
		{
			unsigned int j;
            
            if(m_iCur>m_iHash){
                return false;
            }
            m_pIdx->m_oDataLock.P();
            if( m_iNextOffset !=0
                && 0 != m_pIdx->m_poList[m_iNextOffset].m_iValue){
                
                m_puiPrePosition=&m_pIdx->m_poList[m_iCurOffset].m_iNext;
                m_iCurOffset = m_iNextOffset;

                value = m_pIdx->m_poList[m_iCurOffset].m_iValue;
                key = m_pIdx->m_poList[m_iCurOffset].m_lKey;
                m_iNextOffset = m_pIdx->m_poList[m_iCurOffset].m_iNext;
                m_pIdx->m_oDataLock.V();
                return true;

            }
            if(0==m_iNextOffset){
                
                m_iCur++;
                m_iNextOffset = 0;
                m_iCurOffset = 0;
                m_puiPrePosition=& m_pIdx->m_poHash[m_iCur-1];
                if(m_iCur>m_iHash){
                    m_pIdx->m_oDataLock.V();
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
                        m_pIdx->m_oDataLock.V();
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
                        m_pIdx->m_oDataLock.V();
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
			m_pIdx->m_oDataLock.V();
			return true;		
		}

		Iteration ( unsigned int iHash, SHMIntHashIndex_AX * pIdx):
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
                m_pIdx->m_oDataLock.P();
                m_pIdx->m_poList[m_iCurOffset].m_iValue = iValue;
                if(0 == iValue){
                    bool bRet = removeMe();
                    m_pIdx->m_oDataLock.V();
                    return bRet;
                }
                m_pIdx->m_oDataLock.V();
                return true;
            }
            return false;
        }
        
        bool removeMe(){

#ifdef SHM_EXPAND_GLOBAL_POS
            IntListExpand_AX  m_poList;
            m_poList.m_pShm = m_pIdx;

#else
            ////ʹ�õ�������ɾ�����ڵ�
            SHMIntList_AX * m_poList = m_pIdx->m_poList;
#endif
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
		
		SHMIntHashIndex_AX * m_pIdx;
	};
	
	    
  public:
    Iteration getIteration ()
	{
	    m_oDataLock.P();
		Iteration iter( (*m_piHashValue)+1, this);
		m_oDataLock.V();
		return iter;
	};




friend class Iteration;
friend class IntListExpand_AX;
    
    //##ModelId=424426D6039B
    SHMIntHashIndex_AX(char *shmname );

    //##ModelId=424426D603B9
    SHMIntHashIndex_AX(long shmkey, long extSize=33554432, int iMaxExtCnt=2);

    //##ModelId=424426D603D7
    ~SHMIntHashIndex_AX();

    //##ModelId=42442A29038B
    void add(long key, unsigned int value);

    //##ModelId=42442A5B0134
    bool get(long key, unsigned int *pvalue);

    bool revokeIdx(long key, unsigned int &value);
    void reset();  
    unsigned int malloc();
    void showDetail();
    unsigned int expandMem();
    CSemaphore * getDataLock(){ return &m_oDataLock; };
    
    //##ModelId=42442B02030B
    //##Documentation
    //## ����������
    void create(
        //##Documentation
        //## ���Ԫ�ظ���
        unsigned int itemnumber) ;
    
    //##���������Ϊ����ҵ���ڴ潻��ʹ��
    void empty();

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

    unsigned int *m_puiSegment;
    
#ifdef SHM_EXPAND_GLOBAL_POS
    IntListExpand_AX  m_poList;
    SHMIntList_AX *m_pListUnit;

#else
    //##ModelId=424432820167
    SHMIntList_AX *m_poList;
#endif


    unsigned int *m_piHead;//ָ��ɷ�������ͷ��㣬ÿ����Ҫ����һ�������ͷ��ȡ
    unsigned int *m_piTail;//ָ��ɷ�������β�ӵ㣬ÿ��ɾ��һ������ӵ�β��
    unsigned int *m_piVersion; ////�ڴ�ṹ�İ汾�� ��3
    unsigned int *m_piDelOver;
    
    unsigned int *m_piExpandCnt; ////��¼��չ����
    unsigned int m_iLastTotal;

    CSemaphore m_oDataLock;
};




    


#endif  
