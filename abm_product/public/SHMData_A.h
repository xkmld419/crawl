/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef SHMDATA_A_H_HEADER_INCLUDED_BDBB63C4
#define SHMDATA_A_H_HEADER_INCLUDED_BDBB63C4
////˵�����ɰ������õ��ڴ����
////      ʹ��һ�������¼ɾ���������ʱ�ȷ�����еģ��ٷ���ԭ��ɾ������
////�ŵ㣺����죬�ҷ���ʱ����Ӱ��ɾ����ֻҪ�����������룬ɾ���������룬�����ɾ����������ͬʱ���У�����Ҫ����
////���㣺�����������Լ��������������ʱ��Ҫ���и��ӵ�������ܵõ������
////zhangap
////����޸�ʱ�䣺2010-01-29 


//unsigned int i = (* m_piUsedSE );
//if(i!=0  && 0== m_pIArray[i]){
//   ////�жϵ���û��������
//   
//}
//if( 0== m_pIArray[(* m_piTotalSE)] ){
//    ////�жϵ���û��ɾ������
//    
//}
//if( 0 == (*m_piDelOver) ){
//    ////Ҳ���жϵ���û��ɾ������
//}



#include "HashList.h"
#include "SHMAccess.h"
#include <iostream>
#include <unistd.h>

using namespace std;

#define ATTACH_DATA_A(X, Y, Z) \
        X = new SHMData_A<Y> (Z); \
        if (!(X)) { \
            freeAll (); \
            THROW(MBC_UserInfo+1); \
        } \
        if (!(X->exist())) { \
            m_bAttached = false; \
        }


//## �����ڴ�������ģ���࣬Ҫ���ŵ����ݵ�
//## ÿ��Ԫ�صĴ�С��С��32���ֽڣ����Ԫ�ص�
//## ��СС��32���ֽڣ����������
template <class T>
class SHMData_A : public SHMAccess
{
  public:
	  typedef T value_type;

	
	class Iteration
	{
      private:
        unsigned int m_iCurOffset;
        SHMData_A<T> * m_pDataA;
        unsigned char *pData;
        unsigned int m_iStateCnt;
    
	  private:

        unsigned int getInfo()
        {   /// '0'��ʾ����� '1'��ʾ������
            ////����������״̬��������
            unsigned int uiTemp = (*(m_pDataA->m_piTotalSE));
            
            pData = new unsigned char[uiTemp];
            memset(pData, '0', sizeof(unsigned char)*uiTemp );

            unsigned int i = (*(m_pDataA->m_piUsedSE));
            unsigned int j =0;

            if(i!=0  && 0==m_pDataA->m_pIArray[i]){
                ////��û����������ɾ����ֱ�Ӵ�0��ʼ������ֵ����ʵ�������������ڵ�ƫ��
                uiTemp = i;
                i=0;
                
                for(; i != (*(m_pDataA->m_piDeledSE)); i++){
                    if( i>= uiTemp ){
                        ////����������м��������̽����˴������ݵķ�����ͷŲ���
                        break;
                    }
                    j = m_pDataA->m_pIArray[i];
                    if( 0== j){
                        j = i;
                    }
                    pData[j]++;                       
                }
                return uiTemp;
            } 
            

            for(; i != (*(m_pDataA->m_piDeledSE)); i++){
                if( i>= uiTemp ){
                    i = 0;
                }
                j = m_pDataA->m_pIArray[i];
                if( 0== j){
                    j = i;
                }
                if( '0' == pData[j] ){
                    ////������¼�óɿ���
                    pData[j] = '1';
                    
                }else{
                    ////������¼���ֶ��ɾ��
                    pData[j]++;
                }
            }
            
            return uiTemp;

        }

	  public:
	    void close(){ 
	        if(pData){ delete [] pData; pData =0;}
	        m_iCurOffset = 1;
	        m_iStateCnt = 0;
	    }
	    
	    
	    
		bool next(T & tOut, unsigned int &uiOffsetOut)
		{
			if(!pData){
			    m_iStateCnt = getInfo();
			}
			if(pData && m_iCurOffset>0){
			    while( m_iCurOffset< m_iStateCnt ){
    			    if('0' == pData[m_iCurOffset] ){
    			        uiOffsetOut = m_iCurOffset;
        			    tOut = m_pDataA->m_pPointer[ m_iCurOffset ];
        			    m_iCurOffset++;
        			    
        			    return true;
        			}
        			m_iCurOffset++;
        		}
    			return false;
			}      
			return false;		
		}

		bool nextBatch(unsigned int uiMax, T tOut[], unsigned int &iOutCnt)
		{
			if(!pData){
			    m_iStateCnt = getInfo();
			}
			unsigned int iTotal = (*(m_pDataA->m_piTotalSE));
			unsigned int iCnt=0;
			if(pData && m_iCurOffset>0 && uiMax){
			    while(iCnt<uiMax && m_iCurOffset < m_iStateCnt ){
    			    if('0' == pData[m_iCurOffset] ){
        			    tOut[iCnt] = m_pDataA->m_pPointer[ m_iCurOffset ];
        			    iCnt++;
        			}
        			m_iCurOffset++;
        			
        		}
        		if(iCnt){
        		    iOutCnt = iCnt;
        		    return true;
        		}
        		iOutCnt = 0;
    			return false;
			}      
			return false;		
		}

		Iteration ( unsigned int iCur, SHMData_A<T> * pSHMDataA):
		    m_iCurOffset(iCur), m_pDataA(pSHMDataA),pData(0),m_iStateCnt(0)
		{}

		Iteration (const Iteration & right)
		{
			m_iCurOffset = right.m_iCurOffset;
			m_pDataA = right.m_pDataA;
            pData = 0;
            m_iStateCnt = 0;
		}
		
		~Iteration () 
		{ if(pData)
		    { delete [] pData; pData =0;}
		}
		     

	};
	
	Iteration getIteration ()
	{
		return Iteration ( 1, this);
	}

  public:
	friend class Iteration;
    SHMData_A(char *shmname, bool bShm = true);

    SHMData_A(long shmkey);

    ~SHMData_A();
    
    operator T *();

    void create(
        unsigned int itemnumber) ;

    void reset();

    bool revoke( unsigned int uiOffset);
    void showDetail();
    bool checkHealth();
    unsigned int kick(unsigned int uiBeginOff, 
        int iNxtOffset, int iExpOffset, const char * sDate, unsigned int * o_uiKickSelf, bool * o_bKickSelf );
    unsigned int kickAndRevoke(unsigned int uiBeginOff, 
        int iNxtOffset, int iExpOffset, const char * sDate,
        int iChkOffset1, int iChkOffset2, unsigned int * o_uiKickSelf , bool * o_bKickSelf);
   
    unsigned int malloc();

    unsigned int getCount();

    unsigned int getTotal();

    unsigned long getDelTotal();
    unsigned int  getDelFree ();
    time_t getLoadTime();
	unsigned  int  getMemSize();
	unsigned  int  getUsedSize();
	unsigned long  getMemAdress();
	unsigned long getRealSize();
    
    
    inline void checkSafe(){
        if(0 == *m_piFlag) return;
        while(*m_piFlag){
            usleep(102400);
        }            
    };
    
    bool setRevokeFlag( unsigned int iFlag){
        if(m_poSHM && m_piFlag){
            *m_piFlag = iFlag; 
            return true;
        }
        return false;
    };
    
    bool ifSetRevokeFlag(){
        if( *m_piFlag )
            return true;
        return false;
        };

    
  private:

    void open();

  protected :
    unsigned int *m_piTotal;
    unsigned int *m_piUsed;
    T *m_pPointer;
    ///�ڴ�ṹģʽID�����������ԣ��ϰ汾��0����2����1(����)��2(����) ��ȡֵ��
    unsigned int *m_piVersion; 
    unsigned int *m_piTotalSE;
    unsigned int *m_piUsedSE;
    unsigned int *m_piDeledSE;
    unsigned int *m_piFlag;     ///�����λ����ʾ�����ڴ������������ʱ������������Ҫ��ͣ
    unsigned int *m_piDelOver;  ///��¼ɾ�������ﵽtotal�Ĵ���
    unsigned int *m_pIArray; 
};


template <class T>
SHMData_A<T>::SHMData_A(char *shmname, bool bShm) :
SHMAccess (shmname, bShm)
{
    if (m_poSHM){
        open ();
        if( 1 != *m_piVersion )
            THROW(MBC_SHMData+1);
    }
}


template <class T>
SHMData_A<T>::SHMData_A(long shmkey) :
SHMAccess (shmkey)
{
    if (m_poSHM){
        open ();
        if( 1 != *m_piVersion )
            THROW(MBC_SHMData+1);
    }

}


//##ModelId=42441AD801E8
template <class T>
SHMData_A<T>::~SHMData_A()
{
}

template <class T>
SHMData_A<T>::operator T *()
{
    return m_pPointer;
}


template <class T>
void SHMData_A<T>::create(unsigned int itemnumber) 
{
    if( sizeof(T)< 16 )
        THROW(MBC_SHMData+2);
    SHMAccess::create((itemnumber+2)* (sizeof(T)+sizeof(int))  );
    open ();

    ////�ϳ����������ڴ������޷�����ռ�
    *m_piTotal = 2;
    *m_piUsed = 2;

    *m_piVersion = 1;
    *m_piTotalSE = itemnumber + 1 ;
    
    m_pIArray =(unsigned int *) &m_pPointer[(*m_piTotalSE)+1];

    *m_piUsedSE = 1;
    *m_piDeledSE= 0;

    if( sizeof(T)< 32 ){
        ///1���ṹ��Ų��¿�����Ϣ����ʱ���±��2��ʼ,��itemnumber+1����
        ///ƽʱ�±��Ǵ�1��itemnumber����
        *m_piUsedSE = 2;
        *m_piDeledSE = 1;
        m_pIArray[0] = itemnumber+1;
    }
}

template <class T>
void SHMData_A<T>::reset()
{
    *m_piUsedSE = 1;
    *m_piDeledSE= 0;
    memset( m_pIArray, 0, (*m_piTotalSE) * sizeof(int) );
    *m_piDelOver =0;
    
    if( sizeof(T)< 32 ){
        ///1���ṹ��Ų��¿�����Ϣ����ʱ���±��2��ʼ,��itemnumber+1����
        ///ƽʱ�±��Ǵ�1��itemnumber����
        *m_piUsedSE  = 2;
        *m_piDeledSE = 1;
        m_pIArray[0] = *m_piTotalSE;///itemnumber+1;
    }

}


template <class T>
unsigned int SHMData_A<T>::malloc()
{
    if (*m_piDeledSE == *m_piUsedSE)
        return 0;
    unsigned int uiRet = m_pIArray[(*m_piUsedSE)];
    if(!uiRet)
        uiRet = (*m_piUsedSE);
    
    (*m_piUsedSE)++;
    if( (*m_piUsedSE) >= (*m_piTotalSE) )
        (*m_piUsedSE) = 0 ;
   
    return uiRet;
    
}


template <class T>
void SHMData_A<T>::open()
{
    m_piTotal = (unsigned *)((char *)(*m_poSHM)) + 1;
    m_piUsed = m_piTotal + 1;
    m_pPointer = (T *)( (char *)(*m_poSHM));
    
    m_piVersion = m_piTotal - 1;
    m_piTotalSE = m_piTotal + 2;
    m_piFlag    = m_piTotal + 3;
    m_piUsedSE  = m_piTotal + 4;
    m_piDeledSE = m_piTotal + 5;
    m_piDelOver = m_piTotal + 6;
    if( (*m_piTotalSE) ) {
        m_pIArray =(unsigned int *) &m_pPointer[(*m_piTotalSE)+1];

    }else{
        m_pIArray = 0;
    }



}
template <class T>
unsigned int SHMData_A<T>::getCount()
{
    unsigned int iCnt = 0;

    if( (*m_piUsedSE) > (*m_piDeledSE) ){
        iCnt = ( (*m_piUsedSE) - (*m_piDeledSE) -1 );
    }else{
        iCnt = (*m_piTotalSE)- 1 -  (  (*m_piDeledSE) - (*m_piUsedSE) );    
    }

    return iCnt;
}


template <class T>
unsigned int SHMData_A<T>::getTotal()
{
    if (*m_piTotalSE)
        return (*m_piTotalSE)-1;

    return 0;   
}

///��Ҫ�ڳ��������ֹͬһ��uiOffset����revoke���Σ�һ���������ξͻ��ҵ��ڴ���
template <class T>
bool SHMData_A<T>::revoke( unsigned int uiOffset)
{
    if(!m_poSHM)
        return false;
        
    if( ((*m_piDeledSE)+1 )%(*m_piTotalSE) == (*m_piUsedSE) )
        return false;
    
    T oTemp;
    memset(&oTemp, 0, sizeof(T) );
    if(memcmp(& m_pPointer[uiOffset], &oTemp, sizeof(T)) ==0){
        ////�������ظ�ɾ����������һ������������˵���û�����κθ�ֵ���Ͼͱ�ɾ��
        return false;
    }
    
    m_pIArray[(*m_piDeledSE)] = uiOffset;
    memset( & m_pPointer[uiOffset], 0, sizeof(T) );

///����ֱ���� ++���жϻ�0����ֹ��ʱ�����ڷ���ռ�
    if( (*m_piDeledSE)+1 >= (*m_piTotalSE) ){
        (*m_piDeledSE) = 0;
        (*m_piDelOver)++;
    }else{
        (*m_piDeledSE)++;
    }
        
    return true;   

}

////��ȡ�Դ���������ɾ����������
template <class T>
unsigned long SHMData_A<T>::getDelTotal( )
{
 
    
    if( sizeof(T) <32 )
        return ( (*m_piTotalSE) * (*m_piDelOver) + (*m_piDeledSE)  ) - 1;

    return ( (*m_piTotalSE) * (*m_piDelOver) + (*m_piDeledSE)  );
}


////��ȡ Ŀǰ����ɾ��״̬����δ�����·����ȥ������
template <class T>
unsigned int SHMData_A<T>::getDelFree( )
{
    if( (*m_piDelOver) ){
        ////�Ѿ�ȫ��ɾ��������ô���п���״̬�Ķ�����ǰɾ���õ���λ��
        return getTotal()-getCount();
    }else{
        if( (*m_piUsedSE) > (*m_piDeledSE) ){
            if( sizeof(T) <32 )
                return (*m_piDeledSE)-1;
            else
                return (*m_piDeledSE) ;
        }else{
            return (*m_piDeledSE)-(*m_piUsedSE);
        }
    }
    
    
}

////����ظ�ɾ��
////�����Ƿ񽡿�
template <class T>
bool SHMData_A<T>::checkHealth()
{
    unsigned char * pData = new unsigned char[(*m_piTotalSE)];
    memset(pData, '0', sizeof(char)*(*m_piTotalSE)  );
    unsigned int i = (* m_piUsedSE );
    unsigned int j =0;
    bool bRet = true;
    
    for(; i != (* m_piDeledSE ); i++){
        if( i>= (* m_piTotalSE ) ){
            i = 0;
        }
        j = m_pIArray[i];
        if( 0== j){
            j = i;
        }
        if( '0' == pData[j] ){
            ////������¼����
            pData[j] = '1';
            
        }else{
            ////������¼���ֶ��ɾ��
            pData[j]++;
            bRet = false;
            cout<< pData[j]<<"��ɾ����¼:"<< j<<",";
        }
        cout<<endl;
    }
    delete []pData;

    if( 0== m_pIArray[(* m_piTotalSE)]  ){
        ////��û��ɾ������
        if((*m_piDelOver)!=0 ){
            bRet = false;
            cout<<"ɾ����¼���ڴ�У��1ʧ��.";
        }
    }
    
    if( 0!= m_pIArray[(* m_piTotalSE)]  ){
        ////�Ѿ�ɾ������
        if( 0 == (*m_piDelOver) ){
            bRet = false;
            cout<<"ɾ����¼���ڴ�У��2ʧ��.";
        }
        unsigned int i = (* m_piUsedSE );
        if( i!=0  && 0== m_pIArray[i] ){
            bRet = false;
            cout<<"ɾ����¼���ڴ�У��3ʧ��.";
        }
        ////���ɾ���������ͱ����������
        ////�����жϻ���һ��������ֻ���ȷ��������ſ��ܴ���ɾ����

    }

    return bRet;
    
}

template <class T>
void SHMData_A<T>::showDetail()
{
    if(!m_poSHM)
        return ;
//    cout<<"�ڴ�ģʽ��"<<  (*m_piVersion)<<", �����ṹ���С��"<< sizeof(T) << endl;
//    cout<<"�ڴ�������"<<  getTotal()  <<", ��ʹ��������"<< getCount() << endl;
//    cout<<"�Դ�������ɾ������������"<< getDelTotal() << endl;
//    cout<<"���п���������"<< getTotal()-getCount() << ", ��������ɾ��������Ϊ��"<< getDelFree() << endl;

    char sText[256];
    char keys[64];
    if(m_sSHMName) {
        strncpy(keys, m_sSHMName, sizeof(keys));
    }else{
        sprintf(keys, "%ld", m_lSHMKey);
    }
    if( !m_poSHM ){
    snprintf(sText, sizeof(sText), "%-10s 0x%-10lx %2u %-12s %-9d %-10u %-10u %4d%%",
        keys, 0,(*m_piVersion), "DATA_A", sizeof(T), 0, 0, 0 );
    }else{
    char * pp = (char * )(*m_poSHM);
    unsigned int uiTotal = getTotal();
    unsigned int uiCount = getCount();
    int percent = 100*uiCount/uiTotal;
    if( (100*uiCount)%uiTotal )
        percent++;
    snprintf(sText, sizeof(sText), "%-10s 0x%-10lx %2u %-12s %-9d %-10u %-10u %4u%% %-9u %-10lu",
        keys, pp,(*m_piVersion), "DATA_A", sizeof(T), uiTotal, uiCount, percent,
        getDelFree(), getDelTotal() );
    }
    cout<< sText<<endl;


}


////������µ��˵�1���ڵ㣬��������o_uiKickSelf
////���������ĸ���
template <class T>
unsigned int SHMData_A<T>::kick(unsigned int uiBeginOff, 
    int iNxtOffset, int iExpOffset, const char * sDate, unsigned int * o_uiKickSelf, bool * o_bKickSelf )
{
    if(!sDate || iNxtOffset<0 || iExpOffset<0 )
        return 0 ;

    unsigned int uiDataOff = uiBeginOff;
    unsigned int iCnt=0;
    *o_bKickSelf=false;
    if( uiDataOff ){
        unsigned int uiDataNext = *(unsigned int *)( ( (char *)&m_pPointer[uiDataOff] )+ iNxtOffset);
        unsigned int uiSelf= uiDataOff;
        
        while(uiDataNext){
            char * pExp = ( ( (char *)&m_pPointer[uiDataNext] ) + iExpOffset);
            if( strncmp(pExp, sDate, 8)<0 ){
                ///���ڵ�����
                *(unsigned int *)( ( (char *)&m_pPointer[uiDataOff] )+ iNxtOffset) =
                *(unsigned int *)( ( (char *)&m_pPointer[uiDataNext])+ iNxtOffset);
                
                *(unsigned int *)( ( (char *)&m_pPointer[uiDataNext])+ iNxtOffset)=0;
                iCnt++;
                uiDataNext=*(unsigned int *)( ( (char *)&m_pPointer[uiDataOff] )+ iNxtOffset);
                continue;
            }
            uiDataOff = uiDataNext;
            uiDataNext=*(unsigned int *)( ( (char *)&m_pPointer[uiDataOff] )+ iNxtOffset);
        }
        char * pExp = ( ( (char *)&m_pPointer[uiSelf] ) + iExpOffset);
        if( strncmp(pExp, sDate, 8)<0 ){
            *o_uiKickSelf =
            *(unsigned int *)( ( (char *)&m_pPointer[uiSelf])+ iNxtOffset);
            
            *(unsigned int *)( ( (char *)&m_pPointer[uiSelf])+ iNxtOffset)=0;
            iCnt++;
            *o_bKickSelf = true;
            
        }
        
    }
    return iCnt;
}


////������µ��˵�1���ڵ㣬��������o_uiKickSelf
////����ɾ���ĸ���
template <class T>
unsigned int SHMData_A<T>::kickAndRevoke(unsigned int uiBeginOff, 
    int iNxtOffset, int iExpOffset, const char * sDate,
    int iChkOffset1, int iChkOffset2 , unsigned int  *o_uiKickSelf, bool * o_bKickSelf)
{
    if(!sDate || iNxtOffset<0 || iExpOffset<0 )
        return 0 ;

    unsigned int uiDataOff = uiBeginOff;
    unsigned int iCnt=0;
    *o_bKickSelf=false;
    
    if( uiDataOff ){
        unsigned int uiDataNext = *(unsigned int *)( ( (char *)&m_pPointer[uiDataOff] )+ iNxtOffset);
        unsigned int uiSelf= uiDataOff;
        
        while(uiDataNext){
            char * pExp = ( ( (char *)&m_pPointer[uiDataNext] ) + iExpOffset);
            if( strncmp(pExp, sDate, 8)<0 
                && ( iChkOffset1<0 
                    || 0 == *(unsigned int *)( ( (char *)&m_pPointer[uiDataOff] )+ iChkOffset1) )
                && ( iChkOffset2<0 
                    || 0 == *(unsigned int *)( ( (char *)&m_pPointer[uiDataOff] )+ iChkOffset2) )
                ){
                ///���ڵ�������check�������Ѿ����
                *(unsigned int *)( ( (char *)&m_pPointer[uiDataOff] )+ iNxtOffset) =
                *(unsigned int *)( ( (char *)&m_pPointer[uiDataNext])+ iNxtOffset);
                
                *(unsigned int *)( ( (char *)&m_pPointer[uiDataNext])+ iNxtOffset)=0;
                revoke(uiDataNext);
                iCnt++;
                uiDataNext=*(unsigned int *)( ( (char *)&m_pPointer[uiDataOff] )+ iNxtOffset);
                continue;
            }
            uiDataOff = uiDataNext;
            uiDataNext=*(unsigned int *)( ( (char *)&m_pPointer[uiDataOff] )+ iNxtOffset);
        }
        
        char * pExp = ( ( (char *)&m_pPointer[uiSelf] ) + iExpOffset);
        if( strncmp(pExp, sDate, 8)<0 
                && ( iChkOffset1<0 
                    || 0 == *(unsigned int *)( ( (char *)&m_pPointer[uiSelf] )+ iChkOffset1) )
                && ( iChkOffset2<0 
                    || 0 == *(unsigned int *)( ( (char *)&m_pPointer[uiSelf] )+ iChkOffset2) )
            ){
            *o_uiKickSelf =
            *(unsigned int *)( ( (char *)&m_pPointer[uiSelf])+ iNxtOffset);
            
            *(unsigned int *)( ( (char *)&m_pPointer[uiSelf])+ iNxtOffset)=0;
            iCnt++;
            revoke(uiSelf);
            *o_bKickSelf = true;
            
        }
    }
    return iCnt;
}
template <class T>
unsigned int SHMData_A<T>::getMemSize()
{
	unsigned int  size = 0;
	if (*m_piTotalSE)
	{
		size = (*m_piTotalSE)* sizeof(T);
	}
	if (size)
		return size ;
	
	return 0;
}
template <class T>
unsigned int SHMData_A<T>::getUsedSize()
{
	unsigned int size = 0;
	
	if (*m_piUsedSE)
	{
		 size = (*m_piUsedSE)* sizeof(T);
	}
	if (size)
		return size ;
	
	return 0;
}

template <class T>
unsigned long SHMData_A<T>::getMemAdress()
{		
	if (m_poSHM)
	{
		return   (unsigned long)m_poSHM;
	}
	
	return 0;
}

template <class T>
unsigned long SHMData_A<T>::getRealSize()
{
	  unsigned long size = m_poSHM->size();
		 return size;

}

template <class T>
time_t  SHMData_A<T>::getLoadTime()
{
	time_t shm_ctime;
	
	if (m_poSHM)
	{
		shm_ctime = m_poSHM->getShmTime();	
		
		return shm_ctime;
	}
	return  0;
	
}


//////���ػ�����Ϣ
//template <class T>
//int SHMData_A<T>::getInfo(char * sOut, int iMax )
//{
//    char keys[64];
//    if(m_sSHMName) {
//        strncpy(keys, m_sSHMName, sizeof(name));
//    }else{
//        sprintf(keys, "%ld", m_lSHMKey);
//    }
//    int i = snprintf(sOut, iMax, "%10s 0x%10x %12s %9d %10u %10u %4d%%",
//        keys, (*m_poSHM), "DATA_A", sizeof(T), getTotal(), getCount(), 100L*getCount()/getTotal() );
//
//    return i;
//}

#endif
