/*VER: 1  ר�Ÿ������޸ĵİ汾*/
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.
#ifndef SHMLRUDATA_H_HEADER_INCLUDED
#define SHMLRUDATA_H_HEADER_INCLUDED

#include "HashList.h"
#include "SHMAccess.h"
#include "CSemaphore.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
 
/* =======================
ShmLruData �ࣺ
�ṩ�����ڴ���ʵ�LRU���ܣ�
�ṩ����ɾ�����ܣ�
�ṩ�ύ���ع����ܣ�
�ṩ��session��֧�֣�
֧�ֶ�̬��չ(�����key�����9��key������������չ��Ĭ�Ͽ���ʹ�ã����key���ⲿ��Ҫ�滮��Ŀǰ�涨��Ҫ0��β)

����ʱֱ��ʹ���±���������з��ʱ�ʾ���޸ģ���Ҫ��ʱcommit()
�ⲿ����ǵ����޸ģ������޸�ǰ��Ҫ�Ƚ�������������������ͼ�޸�����sessionδ�ύ������ʱ���쳣��
�ⲿ����ʱ����Ҫ��try���ڽػ��쳣������ʧ�ܡ�

ʹ�÷�ʽ��
    SHMLruData<AcctItemAddAggrData> *  pLruAggr2 
      = new SHMLruData<AcctItemAddAggrData>(100057804L, 
        "/cbilling/cbill_test/pub/tibs_bill_c/FileTest/MFile.bin" , 1,
        67108864, 8 ) ;
����˵���� 
long shmkey,   
const char * sFileName, ----
int iSession, 
long extSize, 
int iMaxExtCnt

����iMaxExtCnt���8 

�Զ����ļ���ʽ��
sFileName���ܿ��ļ�����¼����LRU��Ҫ��صĿ������ݣ�������Զ�������Ŀ¼����Ž����ļ���
�����ļ�������512M����


�������ȵ��� create(total_cnt) ������
ʹ���ߵ��� malloc() ����õ��±꣬�������׼���ã�
����������ڷ���֮ǰ��׼���ã�ʹ�ô�������malloc�ٶȻ��һ���,��������malloc��ֱ�Ӹ�ֵ����
Ȼ��ʹ���±���������ж�ȡ����������ʹ��getForWrite getForRead �ֱ�д�����Ͷ�����

ʹ���ߵ��� revoke(index) ɾ�����䵽���±�
ʹ���ߵ���commit()ȷ���޸ģ�ʹ��rollback�ع��޸�

ʹ���߲���ɾ��δ��������±꣬���ܶ��Ѿ�ɾ�����±�ִ�ж��ɾ�����������쳣��
ɾ������session��δ�ύ���±꽫�������쳣�������Ҫ�����쳣

�쳣������£�
MBC_SHMLruData
MBC_SHMLruData+1 : �����ڴ�ṹ����--���鴴�����������ʹ�õĳ����Ƿ�ͬ���汾
MBC_SHMLruData+2 : �����±�Խ��--
MBC_SHMLruData+3 : ��ͼ�޸�����session�����޸ĵ�����--
MBC_SHMLruData+4 : �ع��ռ䲻��--
MBC_SHMLruData+5 : �����ڴ涼�ڽ����޸ģ�û�пɹ������Ŀռ�--
MBC_SHMLruData+6 : ���ʵ��±��Ѿ���ɾ��--
MBC_SHMLruData+7 : ��Ӧ�Ĵ����ļ����ܴ�--
MBC_SHMLruData+8 : ��Ӧ�Ĵ����ļ�������--
MBC_SHMLruData+9 : ��Ӧ�Ĵ����ļ�д�����ݳ���--
MBC_SHMLruData+10 : ���ܲ�������session--
MBC_SHMLruData+11 : session����
MBC_SHMLruData+12 : �ڴ治��
MBC_SHMLruData+13 : �ڴ���ڴ���

zhangap
2010-5-5
======================= */


const int SHM_LRU_FILENAME_LEN=128;
const int SHM_LRU_MAX_SESSION =1024; 

#define SHM_LRU_CURR_VERSION   12

////�����ݳ���40%��д�ļ�
#define  SHM_LRU_DIRTY_HIGH_LEVEL   (*puiFreshHighPct)
#define  SHM_LRU_DIRTY_LOW_LEVEL    (*puiFreshLowPct)
#define  SHM_LRU_DIRTY_CLOCKCNT     (*puiFreshInterval)

const char SHM_LRU_FLAG_NORM = 0;   ////����δ���޸�
const char SHM_LRU_FLAG_MODIFY = 1u;   ////���޸Ĺ�
const char SHM_LRU_FLAG_FORDEL = 2u;  ////����ɾ�����
const char SHM_LRU_FLAG_DEL = 3u;  ////�Ѿ���ɾ��
const char SHM_LRU_FLAG_CANSEE = 0x0FE; ///����Ϊ�������ݱ���ѯ���ķֽ�㣬С�������ܲ�ѯ��������������
const char SHM_LRU_FLAG_LOG = 0x40;  ////����ΪLog��ʹ��


const char sTestLruFile[]="/opt/bill/app/lru_data/lrutest.bin";
const long lTestLruKey=8100099L;

const char sTestLruFile_2[]="/opt/bill/app/lru_data/lrutest.2.bin";
const long lTestLruKey_2=2009162201L;


const int SHM_LRU_BATCH_SIZE = 67108864;
////268435456; //256M
////   536870912; ////512M �ļ���С��
    

#define ATTACH_DATA_LRU(X, Y, Z, J,K,M,L ) \
        X = new SHMLruData<Y> (Z,J,K,M,L ); \
        if (!(X)) { \
            freeAll(); \
            THROW(MBC_UserInfo+1); \
        } \
        if (!(X->exist())) { \
            m_bAttached = false; \
        }




class FileLruHead
{
    public:
        unsigned int uiHashValue;
        unsigned int uiIterCnt;
        unsigned int uiRealCnt;
        unsigned int uiMallocCnt;
        unsigned int uiExpandCnt;
        unsigned int uiVer;
        long lKey;
        unsigned int uiState;     ////д�ļ���״̬0������1����д
        unsigned int uiDelete;
        unsigned int uiContentSize;
        char sName[8];
};

template <class T1>
class FileLruData
{
    public:
      unsigned int uiNext;
      T1   oData;
};


template <class T>
class SHMLruData : public SHMAccess
{
  public:
//    class LruUnitDataSimple{
//    public:
//        LruUnitData oSimple;
//    };
//    
    class LruUnitData
    {
    public:
        unsigned int uiListPre; 
        unsigned int uiListNext;
        unsigned int uiListLog; ////������ʱ����ű��ݵ�log�飻log��ʱ�����sessionID;
                
        unsigned int uiIndex;         ////����
        unsigned int uiListIdxID;     ////��������id
        unsigned int uiListIdxNext;   ////������next

        unsigned char sFlag[1]; ////0��ʾ�������� ������SHM_LRU_FLAG_xxx������
        T   oListData;
    };

    class LruExpand{
    public:
        LruExpand( ){m_pLruData = 0;};
        LruUnitData &  operator [](unsigned int idx){
            LruUnitData *pBase = (LruUnitData *)(m_pLruData->m_sSegment[0]);

//            if( idx == 467283 )
//            printf("iSegment:  %lx:%u,%lx:%u,%lx:%u\n", 
//                m_pLruData->m_sSegment[0],*(m_pLruData->m_piSeg),
//                m_pLruData->m_sSegment[1],*(m_pLruData->m_piSeg+1) ,
//                m_pLruData->m_sSegment[2],*(m_pLruData->m_piSeg+2) );
                
            if( idx <= (*m_pLruData->m_piSeg)  ){
                return  *(pBase+idx) ;
            }

            
            for(int i=1; i<MAX_SHM_EX_CNT+1;i++){
                if( idx <= (*(m_pLruData->m_piSeg+i)) ){
                    pBase = (LruUnitData *)(m_pLruData->m_sSegment[i]);
                    idx -= (*(m_pLruData->m_piSeg+i-1)) ;
                    return  *(pBase+idx-1) ;
                }

            }
            THROW(MBC_SHMLruData+2);
        };

        SHMLruData * m_pLruData;

    };
    
    friend class LruExpand;
    
  public:
    static long getNeedSize(unsigned int itemnumber );

  public:

    SHMLruData(char *shmname, const char * sFileName, int iSession, long extSize, int iMaxExtCnt);
    SHMLruData(long shmkey,   const char * sFileName, int iSession, long extSize, int iMaxExtCnt);
    ~SHMLruData();
    
    T &  operator [](unsigned int indexBig);
    const T& operator [](unsigned int indexBig) const ;

    bool getForWrite(unsigned int indexBig, T& Tin);
    bool getForRead(unsigned int indexBig, T& Tout) ;
    T&   getForRead(unsigned int indexBig ) ;
    T&   getForWrite(unsigned int indexBig ) ;
        
    bool getForMemRead(unsigned int indexBig, T & Tout) ;
    
    unsigned int getDirty(int i ){ return *(puiDirty+i); };
    void create(unsigned int itemnumber) ;

    unsigned int malloc();
    unsigned int malloc(T & indata );
    
    ///���ݸ���
    unsigned int getCount(){ 
        if(puiRealCnt) return (*puiRealCnt);
        else return 0; };
        
    ///�����ڴ����������
    unsigned int getTotal(){
        if(puiIterCnt) return (*puiIterCnt);
        else return 0; };
    
    ///�����ڴ��е�����,����Logռ�õĲ�������������
    unsigned int getMemCount(){
        if(puiFreeCnt && puiIterCnt ) return (*puiIterCnt)-(*puiFreeCnt);
        else return 0; };

    ///�����ڴ��е���������,ȥ��Logռ�õĲ��֣����ڼ��������ݰٷֱ�
    unsigned int getMemDataCount(){
        if((*puiLogCnt)>(*puiLogTotalCnt) ) return (*puiIterCnt)-(*puiFreeCnt)+(*puiLogTotalCnt)-(*puiLogCnt);
        else return (*puiIterCnt)-(*puiFreeCnt); };
    
    ///����������
    unsigned int getDirtyCount(){
        if( puiDirtyCnt ) return (*puiDirtyCnt);
        else return 0; };

    ///�����ݰٷֱ�
    unsigned int getDirtyPercent(){
        if( getTotal() ) return 100*getDirtyCount()/getTotal();
        else return 0;
        };

    ///���������ύ��
    unsigned int getCommitDirtyCount(){
        return getDirtyCount()-(*puiLogCnt);
        };
    ///��־ʹ�ðٷֱȣ�ע�⣺�ɳ���100%
    unsigned int getLogPercent(){
        if( *puiLogTotalCnt ) return 100*(*puiLogCnt)/(*puiLogTotalCnt);
        else return 0; };

    ///����������
    unsigned int getHitrate(){
        if( (*puiAccessCnt) >= 100 ){
            (*puiHitrate) = 100*((*puiAccessCnt)-(*puiFailCnt))/(*puiAccessCnt);
            if( (*puiHitrate)>100){
                /////���ִ���100 ��ʵ�Ǹ���
                (*puiHitrate)=0;
            }
            if((*puiAccessCnt) >= 1000){
                (*puiAccessCnt)=0;
                (*puiFailCnt)=0;
            }
        }
        return (*puiHitrate); 
    };

    unsigned int resetHitrateCalc(){
        (*puiAccessCnt) = 0;
        (*puiFailCnt) = 0;
        return (*puiHitrate); 
    };

    bool getState( unsigned int & uiState, long int & StateTime ){
        if(puiState ){
            uiState = *puiState;
            StateTime = *pulStateTime;
            return true;
        }
        return false; 
    };
    
    bool setState( unsigned int uiState ){
        if( puiState ){
            *puiState = uiState ;
            setStateTime();
            return true;
        }
        return false; 
    };    

    bool setStateTime( ){
        if( pulStateTime ){
        	struct timeval tv;
        	struct timezone tz;
            if(0 == gettimeofday (&tv , &tz) ){
                *pulStateTime = tv.tv_sec;
            }
            return true;
        }
        return false; 
    };   

    char * getBaseP(){ return (char *)(*m_poSHM);  };

    CSemaphore * getDataLock(){    return &m_oDataLock; };
    
    void setNextSize(long lNextSize ){SHMAccess::m_lNextSize =lNextSize; };
    void setMaxExCnt( int iMaxExCnt ){SHMAccess::m_iMaxExCnt =iMaxExCnt; };

    void setExthwm( int iExthwm ){ if(puiExthwm){ *puiExthwm=iExthwm;} };
    unsigned int  getExthwm(  ){ 
        if(puiExthwm) return (*puiExthwm);
        else return 0;
         };
         
    void setFreshHighPct(int ii){  if(puiFreshHighPct){(*puiFreshHighPct) = ii;} };
    unsigned int  getFreshHighPct(){ 
        if(puiFreshHighPct) return (*puiFreshHighPct);
        else return 0;
         };

    void setFreshLowPct(int ii){  if(puiFreshLowPct){(*puiFreshLowPct) = ii;} };
    unsigned int  getFreshLowPct(){ 
        if(puiFreshLowPct) return (*puiFreshLowPct);
        else return 0;
         };

    void setFreshInt(int ii){  if(puiFreshInterval){(*puiFreshInterval) = ii;} };
    unsigned int  getFreshInt(){ 
        if(puiFreshInterval) return (*puiFreshInterval);
        else return 0;
         };

         
    void setWrnhwm( int ii ){ if(puiWrnhwm){ *puiWrnhwm=ii;} };
    unsigned int  getWrnhwm( ){ 
        if(puiWrnhwm) return (*puiWrnhwm);
        else return 0;
         };

    void setUpperLength( int ii ){ if(puiUpperLength){ *puiUpperLength=ii;} };
    unsigned int  getUpperLength( ){ 
        if(puiUpperLength) return (*puiUpperLength);
        else return 0;
         };

    unsigned int  getLruLength( ){ 
        if(getTotal()>(*puiInitLength)) return getTotal();
        else return (*puiInitLength) ;
         };

    void setCommitDownFile( int ii ){ if(puiCommitDownFile){ *puiCommitDownFile=ii;} };
    unsigned int  getCommitDownFile( ){ 
        if(puiCommitDownFile) return (*puiCommitDownFile);
        else return 0;
         };

    unsigned int  getExpandCnt( ){ 
        if( puiExpandCnt ) return (*puiExpandCnt);
        else return 0;
         };
///��������ļ��Ļ�����������1��������0
    void setWriteFileFlag( int ii ){ if(puiWriteFileFlag){ *puiWriteFileFlag=ii;} };
    unsigned int  getWriteFileFlag( ){ 
        if(puiWriteFileFlag) return (*puiWriteFileFlag);
        else return 0;
         };
	
    void setExpandMaxSize( int ii ){ if(puiExpandMaxSize){ *puiExpandMaxSize=ii;} };
    unsigned int  getExpandMaxSize( ){ 
        if(puiExpandMaxSize) return (*puiExpandMaxSize);
        else return 0;
         };


    void reset();
    bool revoke( unsigned int uiOffset);
    void showDetail();
    void showNormal( bool bDetail = false );

    unsigned int commit();
    unsigned int rollback();
    void commitSession(unsigned int iSession);
    void rollbackSession (unsigned int iSession);

    bool getByFile( unsigned int iOffset, T& outT );    
    unsigned int  writeToFile(bool bForce = false );
    unsigned int  recoverFromFile( );
    void truncFile();
    
    unsigned int expandMem(bool bTreatFile = true );
        
  private:
    void init();
    void initFile(bool bTrunc );
    void open();
    unsigned int mallocLog();
    unsigned int mallocFree();
    unsigned int doPageIn(unsigned int index, bool ifModify );
    unsigned int doPageOut(unsigned int index );
    unsigned int doDirtyOut(unsigned int index);
    unsigned int doListOut(unsigned int indexReal, unsigned int * pHead, unsigned int *pCnt );
    unsigned int doListMoveFirst(unsigned int indexReal, unsigned int * pHead);
    void doListIn(unsigned int indexReal, unsigned int * pHead, unsigned int *pCnt );
    
    void addToNormal( unsigned int index );
    void addToDirty( unsigned int index , int i_iSession );
    void addToFree( unsigned int index );
    void addToFreeLog( unsigned int index );
    
    void addToDelIndex( unsigned int indexBig, unsigned int index);
    unsigned int addToIndex(unsigned int indexBig, unsigned int index );
    unsigned int delFromIndex(unsigned int indexBig, int iFlag );


    unsigned int getLruDataForMalloc(unsigned int indexBig) ;
    unsigned int getLruDataForWrite(unsigned int indexBig, bool bFreshNew = false );
    unsigned int getLruDataForRead(unsigned int indexBig, FileLruData<T> * pInitData = 0);

    unsigned int moveIndex(unsigned int indexBig, unsigned int uiNewHash);
//    unsigned int writeFileAll( FILE * fp );
    unsigned int writeFileBatch( FILE * fpHead, bool bAll );
    unsigned int writeFileHead( FILE * fpHead, FileLruHead & oHead, unsigned int uiState );
    unsigned int expandFile(FILE *fp);
    FILE * prepareFile( int iTimes, bool bTrunc, bool bCreate=true);
  private:
///���������ڴ�
     unsigned int * puiFree;
     unsigned int * puiNorm;
     unsigned int * puiLog;

     unsigned int * puiDirty;////[SHM_LRU_MAX_SESSION];
     unsigned int * puiDelete;  ///Delete ���ǵ���������¼��int�Ƕ�����±�

     unsigned int * puiFreeCnt;
     unsigned int * puiNormCnt;
     unsigned int * puiDirtyCnt;
     unsigned int * puiLogCnt;
     unsigned int * puiLogTotalCnt;
     unsigned int * puiLogBegin; ////Log�����俪ʼ�ĵط�
     unsigned int * puiHashValue; /////����hashֵ�� ����ڴ治��չ����(*puiHashValue)����(*puiIterCnt)
    
     unsigned int * puiIterCnt;
     unsigned int * puiRealCnt;
     unsigned int * puiMallocCnt;
    //unsigned int * puiUsedCnt; -- ����ͨ�� NormCnt+DirtyCnt+LogCnt�õ� ���� IterCnt-FreeCnt
     unsigned int * puiExpandCnt;
    
    unsigned int *puiVer;
    unsigned int *puiContentSize;
     unsigned int *puiSessionCnt;
   
     unsigned int *puiAccessCnt;   ///���ʴ���
     unsigned int *puiFailCnt;     ///δ���д���
     unsigned int *puiHitrate;    ///������

     unsigned int *puiState;     ////��¼״̬��,������checkpoint
     long   *pulStateTime;     ////state_date 
     unsigned int *puiSegment;
    
     unsigned int *puiExthwm;
     unsigned int *puiFreshHighPct;
     unsigned int *puiFreshLowPct;
     unsigned int *puiFreshInterval;
     unsigned int *puiWrnhwm;
     unsigned int *puiInitLength;
     unsigned int *puiUpperLength;
     unsigned int *puiCommitDownFile;
	 unsigned int *puiWriteFileFlag;
    unsigned int * puiExpandMaxSize;
  private:
///����˽���ڴ�
    char m_sFileName[SHM_LRU_FILENAME_LEN];
    char m_sFileExName[SHM_LRU_FILENAME_LEN+8];

#ifdef SHM_EXPAND_GLOBAL_POS
    LruExpand  m_pPointer;
    LruUnitData *m_pLruUnit;
#else
    LruUnitData *m_pPointer;
#endif
    unsigned int m_iSessionID;
    unsigned int m_iLastTotal; 
    unsigned int m_iCntPerFile;/////һ���ļ����ɶ��ٸ���Ŀ
    
    CSemaphore m_oDataLock;
    unsigned int m_iLastAccess;
    long m_lClock;
    T m_oDelUnit;
    T m_oReadUnit;
};


///////////////////////////////////////////////////////////////////////////////

////25%��log�ռ�
template <class T>
long SHMLruData<T>::getNeedSize(unsigned int itemnumber ){
        return (long)sizeof(LruUnitData) * (itemnumber +  itemnumber/4 )  +
         (64+SHM_LRU_MAX_SESSION)*sizeof(unsigned int);
};


template <class T>
void SHMLruData<T>::init()
{
    m_lClock = 0;
    
    puiFree=0;
    puiNorm=0;
    puiLog=0;
    puiDirty=0;
    puiDelete=0;

    puiFreeCnt=0;
    puiNormCnt=0;
    puiDirtyCnt=0;
    puiLogCnt=0;
    puiLogTotalCnt=0;

	puiInitLength=0;
	puiUpperLength=0;
    puiCommitDownFile=0;
	puiWriteFileFlag=0;
    puiExpandMaxSize=0;
    
    puiLogBegin=0; 
    puiHashValue=0;
        
    puiIterCnt=0;
    puiRealCnt=0;
    puiMallocCnt=0;
    puiExpandCnt=0;
    
    puiVer=0;
    puiContentSize=0;
    puiSessionCnt=0;
    
    puiAccessCnt=0; 
    puiFailCnt=0;
    puiHitrate=0;

    puiState=0;
    pulStateTime=0;
    
    puiSegment = 0;
    
#ifdef SHM_EXPAND_GLOBAL_POS
    m_pPointer.m_pLruData = this;
    m_pLruUnit = 0;
#else
    m_pPointer=0;
#endif

    m_iSessionID=0;
    m_iLastTotal = 0;
    
    m_iCntPerFile = SHM_LRU_BATCH_SIZE/sizeof( FileLruData<T> );
    m_iCntPerFile = (m_iCntPerFile/8)*8;
    
    memset(m_sFileName, 0 , sizeof(m_sFileName) );
    memset(m_sFileExName, 0 , sizeof(m_sFileExName) );    
    memset(&m_oDelUnit, 0, sizeof(m_oDelUnit) );

    m_iLastAccess=0;
}


template <class T>
void SHMLruData<T>::initFile(bool bTrunc )
{
    FILE *fp;
    if( !m_sFileName[0] ) 
        return ;
        
    if(bTrunc){
        if( (fp = fopen( m_sFileName,  "w" )) == NULL ){
            THROW(MBC_SHMLruData+7);
        }
    }else{
        if( (fp = fopen( m_sFileName,  "ab+" )) == NULL ){
            THROW(MBC_SHMLruData+7);
        }
    }
    fclose (fp);
    fp = 0;
    
    if( SHMAccess::exist() 
        && (*puiContentSize)==sizeof(T) ){ 
        if ((fp = fopen (m_sFileName, "rb+")) != NULL) {
            fseek(fp, 0,  SEEK_END);
            if( ftell(fp) < sizeof(FileLruHead) ) {
                FileLruHead oHead;
                if(0 == writeFileHead( fp, oHead, 0) ){
                    fclose (fp);
                    THROW(MBC_SHMLruData+9);
                }
            }

            fclose (fp);
            return;
        }
        THROW(MBC_SHMLruData+7);
    }
    
}


template <class T>
SHMLruData<T>::SHMLruData(char * shmname, const char * sFileName, int iSession, 
     long extSize, int iMaxExtCnt):
    SHMAccess (shmname, extSize, iMaxExtCnt )
{
    init();

    strncpy( m_sFileName , sFileName, sizeof(m_sFileName)-1 );
    m_iSessionID = iSession%SHM_LRU_MAX_SESSION;

    char sTemp[64];
    int  iTemp = strlen(shmname)>9 ? (strlen(shmname)-9 ) : 0;
    snprintf(sTemp, sizeof(sTemp), "%u", atoi(&shmname[iTemp])+9);
    m_oDataLock.getSem (sTemp, 1, 1);
    
        
    if (m_poSHM){
        open ();
        if( SHM_LRU_CURR_VERSION != *puiVer )
            THROW(MBC_SHMLruData+1);
            
    }

    initFile(false);
}


template <class T>
SHMLruData<T>::SHMLruData(long shmkey, const char * sFileName, int iSession, 
      long extSize, int iMaxExtCnt):
    SHMAccess (shmkey, extSize, iMaxExtCnt )
{
    init();

    strncpy( m_sFileName , sFileName, sizeof(m_sFileName)-1 );
    m_iSessionID = iSession%SHM_LRU_MAX_SESSION;

    char sTemp[64];
    snprintf(sTemp, sizeof(sTemp), "%ld", shmkey+9 );
    m_oDataLock.getSem (sTemp, 1, 1);    

    if (m_poSHM){
        open ();
        if( SHM_LRU_CURR_VERSION != *puiVer )
            THROW(MBC_SHMLruData+1);
    
    }


    
    initFile(false);
    
}

template <class T>
SHMLruData<T>::~SHMLruData()
{

}



// �ǳ����汾,
template <class T>
T & SHMLruData<T>::operator [](unsigned int indexBig){
    while(  (*puiFreeCnt)+(*puiNormCnt) < 128  ){
        usleep(10000);
    }
    while(!m_oDataLock.P());
try{
    unsigned int indexReal = getLruDataForWrite(indexBig);
    if(indexReal ){
        ////m_pPointer[indexReal].oListData;
        m_oDataLock.V();
        return m_pPointer[indexReal].oListData;
    }
    m_oDataLock.V();
    memset(&m_oReadUnit, 0,sizeof(m_oReadUnit) );
    return m_oReadUnit;

}catch(...){
    m_oDataLock.V();
    throw;
}

}

// �����汾,��������session�����ص����޸ĵ��ǿ飬���ڷ�����session�����ص��Ǳ��ݿ�
template <class T>
const T& SHMLruData<T>::operator [](unsigned int indexBig) const {
    while(  (*puiFreeCnt)+(*puiNormCnt) < 128  ){
        usleep(10000);
    }
    m_oDataLock.P();
try{
    unsigned int indexReal = getLruDataForRead(indexBig);
    if(indexReal ){
        m_oReadUnit=m_pPointer[indexReal].oListData;
        m_oDataLock.V();
        return m_oReadUnit;
    }
    m_oDataLock.V();
    memset(&m_oReadUnit, 0,sizeof(m_oReadUnit) );
    return m_oReadUnit;
}catch(...){
    m_oDataLock.V();
    throw;
}

}

template <class T>
bool SHMLruData<T>::getForWrite(unsigned int indexBig, T& Tin){
    while(  (*puiFreeCnt)+(*puiNormCnt) < 128  ){
        usleep(10000);
    }
    while(!m_oDataLock.P());
try{
    unsigned int indexReal = getLruDataForWrite(indexBig);
    if(indexReal){
        m_pPointer[indexReal].oListData = Tin;
        m_oDataLock.V();
        return true;
    }
    m_oDataLock.V();
    return false;
}catch(...){
    m_oDataLock.V();
    throw;
}


}


template <class T>
T& SHMLruData<T>::getForWrite(unsigned int indexBig){
    while(  (*puiFreeCnt)+(*puiNormCnt) < 128  ){
        usleep(10000);
    }
    while(!m_oDataLock.P());
try{
    unsigned int indexReal = getLruDataForWrite(indexBig);
    if(indexReal){
        m_oDataLock.V();
        return m_pPointer[indexReal].oListData;
    }
    m_oDataLock.V();
    THROW(MBC_SHMLruData+13);
}catch(...){
    m_oDataLock.V();
    throw;
}


}

template <class T>
bool SHMLruData<T>::getForRead(unsigned int indexBig, T& Tout) {


    while(!m_oDataLock.P());
    while(  (*puiFreeCnt)+(*puiNormCnt) < 128  ){
        usleep(10000);
    }


try{
    unsigned int indexReal = getLruDataForRead(indexBig);
    if(indexReal){
        Tout=m_pPointer[indexReal].oListData;
        m_oDataLock.V();
        return true;
    }
    m_oDataLock.V();
    return false;
}catch(...){
    m_oDataLock.V();
    throw;
}


}


template <class T>
T& SHMLruData<T>::getForRead(unsigned int indexBig ) {
    while(  (*puiFreeCnt)+(*puiNormCnt) < 128  ){
        usleep(10000);
    }
    while(!m_oDataLock.P());
try{
    unsigned int indexReal = getLruDataForRead(indexBig);
    if(indexReal ){
        m_oReadUnit=m_pPointer[indexReal].oListData;
        m_oDataLock.V();
        return m_oReadUnit;
    }
    m_oDataLock.V();
    memset(&m_oReadUnit, 0,sizeof(m_oReadUnit) );
    return m_oReadUnit;
}catch(...){
    m_oDataLock.V();
    throw;
}


}





template <class T>
bool SHMLruData<T>::getForMemRead(unsigned int indexBig, T & Tout) {


    while(!m_oDataLock.P());
try{
    
    if( indexBig < 1 || indexBig > (*puiMallocCnt) )
        THROW(MBC_SHMLruData+2);

    if(m_iLastTotal != (*puiIterCnt)) {
        SHMAccess::reopen();
        m_iLastTotal = (*puiIterCnt);
    }

///    (*puiAccessCnt)++;
    unsigned int indexReal = m_pPointer[(indexBig-1)%(*puiHashValue)+1].uiIndex;
    while(indexReal){
        if(indexBig == m_pPointer[indexReal].uiListIdxID ) {
            if( 0 == m_iSessionID ){
                Tout = m_pPointer[indexReal].oListData;
                m_oDataLock.V();
                return true;
            }

            if(SHM_LRU_FLAG_DEL ==  m_pPointer[indexReal].sFlag[0]  ){
                /////�Ѿ�ɾ���Ĳ��ܱ��鵽
                THROW(MBC_SHMLruData+6);
                
            }
            unsigned int indexReal = getLruDataForRead(indexBig);
            Tout = m_pPointer[indexReal].oListData;
            m_oDataLock.V();
            return true;            
        }
        indexReal = m_pPointer[indexReal].uiListIdxNext ;
    }
    
    ////
    
}catch(...){
    m_oDataLock.V();
    throw;
}

    m_oDataLock.V();
    return false;

}



/////ɾ���ı������ͷų���ʱ����������� �������ֻ����DEL״̬ת���� MPDIFY ״̬ 
template <class T>
unsigned int SHMLruData<T>::getLruDataForMalloc(unsigned int indexBig) 
{        
    if( indexBig < 1 || indexBig > (*puiMallocCnt) )
        THROW(MBC_SHMLruData+2);
    
    if(m_iLastTotal != (*puiIterCnt)) {
        SHMAccess::reopen();
        m_iLastTotal = (*puiIterCnt);
    }
      	
    if(m_iLastAccess!=indexBig){
        (*puiAccessCnt)++;
        m_iLastAccess=indexBig;
    }
    unsigned int indexReal = (indexBig-1)%(*puiHashValue)+1 ;
    indexReal = m_pPointer[indexReal].uiIndex;
    while(indexReal){
        if( indexBig == m_pPointer[indexReal].uiListIdxID ) {
            ////ɾ�������ݿ��Բ���أ���ʱ��������������
            ////THROW(MBC_SHMLruData+2);
            
            ////����һ��log��
            unsigned int uitemp = mallocLog();
            if(0==uitemp){
                ////û��log����������
                THROW(MBC_SHMLruData+4);
            }

            ///��0��dirty�ŵ�dirty��session������ȥ
            doListOut(indexReal, puiDirty+0, puiDirtyCnt);
            addToDirty(indexReal, m_iSessionID);

            ////����flag
            m_pPointer[uitemp].sFlag[0] = m_pPointer[indexReal].sFlag[0];
            m_pPointer[indexReal].sFlag[0] = SHM_LRU_FLAG_MODIFY;

            ////mallocLog �����Ŀռ䣬���ܽ���memset�������ڴ�ȫ��
            ////m_pPointer[uitemp].sFlag[0] = SHM_LRU_FLAG_LOG;
            m_pPointer[uitemp].oListData = m_pPointer[indexReal].oListData;
            m_pPointer[uitemp].uiListLog = m_iSessionID;
            
            m_pPointer[indexReal].uiListLog = uitemp;
            
            return indexReal;

        }
        indexReal = m_pPointer[indexReal].uiListIdxNext ;
    }
        
    (*puiFailCnt)++;


    indexReal = doPageIn(indexBig, true );

    if( indexReal ){
        if(SHM_LRU_FLAG_DEL != m_pPointer[indexReal].sFlag[0] ){
            /////page in ��Ӧ�����Ѿ�ɾ�������ݲŶԣ�
            ////��Ȼ����del�������Ѿ������ˣ�flagΪNORMAL, ����session������
            doListOut(indexReal, puiDirty+m_iSessionID, puiDirtyCnt);
            addToNormal(indexReal);

            THROW(MBC_SHMLruData+8);
        }
        ////����һ��log��
        unsigned int uitemp = mallocLog();
        if(0==uitemp){
            ////Ŀǰpagein���������Ѿ�����dirty�����棬����û��log����������
            ///ɾ�������ݣ�û����������˽����Ƶ���Free
            doListOut(indexReal, puiDirty+m_iSessionID, puiDirtyCnt);
            addToFree(indexReal);
            addToIndex(indexBig, indexReal);
            THROW(MBC_SHMLruData+4);
        }
        ////����flag
        m_pPointer[uitemp].sFlag[0] = m_pPointer[indexReal].sFlag[0];

        m_pPointer[indexReal].sFlag[0] = SHM_LRU_FLAG_MODIFY;

        ////mallocLog �����Ŀռ䣬���ܽ���memset�������ڴ�ȫ��
        ////m_pPointer[uitemp].sFlag[0] = SHM_LRU_FLAG_LOG;
        m_pPointer[uitemp].oListData = m_pPointer[indexReal].oListData;
        m_pPointer[uitemp].uiListLog = m_iSessionID;
        
        m_pPointer[indexReal].uiListLog = uitemp;
        
        addToIndex(indexBig, indexReal);

        
        return indexReal;
    }
    
    THROW(MBC_SHMLruData+5);


}


template <class T>
unsigned int SHMLruData<T>::getLruDataForWrite(unsigned int indexBig, bool bFreshNew) 
{        
    if( indexBig < 1 || indexBig > (*puiMallocCnt) )
        THROW(MBC_SHMLruData+2);
    
    if(m_iLastTotal != (*puiIterCnt)) {
        SHMAccess::reopen();
        m_iLastTotal = (*puiIterCnt);
    }
      	
    if(m_iLastAccess!=indexBig){
        (*puiAccessCnt)++;
        m_iLastAccess=indexBig;
    }
    unsigned int indexReal = (indexBig-1)%(*puiHashValue)+1 ;
    indexReal = m_pPointer[indexReal].uiIndex;
    while(indexReal){
        if( indexBig == m_pPointer[indexReal].uiListIdxID ) {
            if( 0 == m_iSessionID )
                return indexReal;
            
            if( m_pPointer[indexReal].uiListLog ){
                
                if( m_iSessionID ==  m_pPointer[ m_pPointer[indexReal].uiListLog ].uiListLog  ){ 
                    ////�������ɾ���������Ͳ��������޸�
                    if( SHM_LRU_FLAG_FORDEL == m_pPointer[indexReal].sFlag[0] ){
                        THROW(MBC_SHMLruData+6);
                    }
                    return indexReal;
                }else
                    THROW(MBC_SHMLruData+3);
            }else{
    
                ////����һ��log���ݿ�
                unsigned int uiLogtemp = mallocLog();
                if(0==uiLogtemp)
                    THROW(MBC_SHMLruData+4);

                ///�ƶ�����
                ///����
                ////���������ͷ������Ҫ�ƶ�����ͷ;���������β����Ҫ�޸�����ͷ
                if(SHM_LRU_FLAG_NORM == m_pPointer[indexReal].sFlag[0]){
                    doListOut(indexReal, puiNorm, puiNormCnt );
                }else if(SHM_LRU_FLAG_MODIFY == m_pPointer[indexReal].sFlag[0] ){
                    doListOut(indexReal, puiDirty+0, puiDirtyCnt);
                }else {
                    ////
                    if( uiLogtemp < (*puiLogBegin) 
                        || uiLogtemp >= (*puiLogBegin)+(*puiLogTotalCnt)  ){
                        ////��������Log�ռ�
                        addToFree(uiLogtemp);
                        (*puiLogCnt)--;
                    }else{
                        ////Logԭ���Ŀռ�
                        addToFreeLog(uiLogtemp);
                    }
                    
                    THROW(MBC_SHMLruData+6);
                }

                ///������session������ȥ,��Ϊ������
                addToDirty(indexReal, m_iSessionID);

                ////����flag
                ////mallocLog �����Ŀռ䣬���ܽ���memset�������ڴ�ȫ��
                m_pPointer[uiLogtemp].sFlag[0] = m_pPointer[indexReal].sFlag[0];

                if(SHM_LRU_FLAG_NORM == m_pPointer[indexReal].sFlag[0] ){
                    m_pPointer[indexReal].sFlag[0] = SHM_LRU_FLAG_MODIFY;
                }
//                if( SHM_LRU_FLAG_DEL == m_pPointer[indexReal].sFlag[0] ){
//                    m_pPointer[indexReal].sFlag[0] = SHM_LRU_FLAG_MODIFY;
//
//                }

                m_pPointer[uiLogtemp].oListData = m_pPointer[indexReal].oListData;
                m_pPointer[uiLogtemp].uiListLog = m_iSessionID;
                
                m_pPointer[indexReal].uiListLog = uiLogtemp;
                return indexReal;
            }
        }
        indexReal = m_pPointer[indexReal].uiListIdxNext ;
    }
        


    if(bFreshNew){
        unsigned int iTempxx = mallocFree( );
        if(0 == iTempxx){
            iTempxx = doPageOut(0);
            if(0 == iTempxx){
                THROW(MBC_SHMLruData+5);
            }
        }

        m_pPointer[iTempxx].uiListLog  = 0;
        m_pPointer[iTempxx].oListData  = m_oDelUnit;
        m_pPointer[iTempxx].sFlag[0] = '\0';
        
        m_pPointer[iTempxx].uiListIdxID = indexBig;
        
        addToDirty(iTempxx, m_iSessionID);
        addToIndex(indexBig, iTempxx);
        indexReal = iTempxx;
    }else{

    
        (*puiFailCnt)++;        

        indexReal = doPageIn(indexBig, true );
        if(indexReal 
            && SHM_LRU_FLAG_DEL == m_pPointer[indexReal].sFlag[0] ){
            /////page in �����Ѿ�ɾ�������ݣ���������˲ſ��Խ���д����
            ////��Ȼ��del�����ݣ���ô������û�е�
            doListOut(indexReal, puiDirty+m_iSessionID, puiDirtyCnt);
            addToFree(indexReal);
            delFromIndex(indexBig, 0);
            m_pPointer[indexReal].oListData  = m_oDelUnit;
            m_pPointer[indexReal].sFlag[0] = '\0';
            
            THROW(MBC_SHMLruData+6);
        }

    }
    
    if( indexReal ){

        if( 0 == m_iSessionID ){
            return indexReal;
        }

        ////����һ��log��
        unsigned int uitemp = mallocLog();
        if(0==uitemp){
            ////Ŀǰpagein���������Ѿ�����dirty�����棬����û��log����������
            ////����ɾ������Ŀ��index�Ѿ��źã������Ҫ�����Ƶ���normal
            doListOut(indexReal, puiDirty+m_iSessionID, puiDirtyCnt);
            addToNormal(indexReal);

            THROW(MBC_SHMLruData+4);
        }
        ////����flag
        m_pPointer[uitemp].sFlag[0] = m_pPointer[indexReal].sFlag[0];

        if(SHM_LRU_FLAG_NORM == m_pPointer[indexReal].sFlag[0] 
            || SHM_LRU_FLAG_DEL == m_pPointer[indexReal].sFlag[0] ){
            m_pPointer[indexReal].sFlag[0] = SHM_LRU_FLAG_MODIFY;
        }
        ////mallocLog �����Ŀռ䣬���ܽ���memset�������ڴ�ȫ��
        ////m_pPointer[uitemp].sFlag[0] = SHM_LRU_FLAG_LOG;
        m_pPointer[uitemp].oListData = m_pPointer[indexReal].oListData;
        m_pPointer[uitemp].uiListLog = m_iSessionID;
        
        m_pPointer[indexReal].uiListLog = uitemp;
        return indexReal;
    }
    
    THROW(MBC_SHMLruData+5);


}

template <class T>
unsigned int SHMLruData<T>::getLruDataForRead(unsigned int indexBig, FileLruData<T> * pInitData )
{          
    if( indexBig < 1 || indexBig > (*puiMallocCnt) )
        THROW(MBC_SHMLruData+2);

    if(m_iLastTotal != (*puiIterCnt)) {
        SHMAccess::reopen();
        m_iLastTotal = (*puiIterCnt);
    }

    if(m_iLastAccess!=indexBig){
        (*puiAccessCnt)++;
        m_iLastAccess=indexBig;
    }
    unsigned int indexReal = m_pPointer[(indexBig-1)%(*puiHashValue)+1].uiIndex;
    while(indexReal){
        if(indexBig == m_pPointer[indexReal].uiListIdxID ) {
            if( 0 == m_iSessionID )
                return indexReal;

            if(SHM_LRU_FLAG_DEL ==  m_pPointer[indexReal].sFlag[0]  ){
                /////�Ѿ�ɾ���Ĳ��ܱ��鵽
                THROW(MBC_SHMLruData+6);
                
            }    
            if( m_pPointer[indexReal].uiListLog ){
                
                if( m_iSessionID ==  m_pPointer[ m_pPointer[indexReal].uiListLog ].uiListLog  ) {
                    if(SHM_LRU_FLAG_FORDEL ==  m_pPointer[indexReal].sFlag[0]  ){
                        /////�Լ��Ѿ�ɾ�����Լ����ܱ��鵽
                        THROW(MBC_SHMLruData+6);
                    }
                    return indexReal;
                }else{
                    if(SHM_LRU_FLAG_DEL ==  m_pPointer[m_pPointer[indexReal].uiListLog].sFlag[0] ){
                        /////�Ѿ�ɾ���Ĳ��ܱ��鵽
                        THROW(MBC_SHMLruData+6);
                    }
                    return m_pPointer[indexReal].uiListLog ;
                }
            }else{
                
                ////���������ͷ������Ҫ�ƶ�����ͷ
                if(SHM_LRU_FLAG_NORM != m_pPointer[indexReal].sFlag[0] ){

                    if( indexReal == (*(puiDirty+0) ) ){
                        return indexReal;
                    }
                    
                    ///�ƶ�����
                    ///����
                    unsigned int uitemp = m_pPointer[indexReal].uiListPre;
                    if(uitemp)m_pPointer[uitemp].uiListNext = m_pPointer[indexReal].uiListNext;
                    uitemp = m_pPointer[indexReal].uiListNext;
                    if(uitemp)m_pPointer[uitemp].uiListPre = m_pPointer[indexReal].uiListPre;                    
                    ///������Dirty[0]������ȥ

                    if( indexReal == m_pPointer[*(puiDirty+0)].uiListPre ){
                        m_pPointer[indexReal].uiListNext = *(puiDirty+0);
                        *(puiDirty+0) = indexReal;
                        return  indexReal;
                    }


                    if( *(puiDirty+0 ) ){
                        m_pPointer[indexReal].uiListPre = m_pPointer[*(puiDirty+0)].uiListPre;
                        m_pPointer[indexReal].uiListNext = *(puiDirty+0) ;
                        m_pPointer[*(puiDirty+0)].uiListPre = indexReal; 
                        (*(puiDirty+0)) = indexReal;
                    }else{
                        m_pPointer[indexReal].uiListPre = indexReal;
                        m_pPointer[indexReal].uiListNext = 0;
                        *(puiDirty+0) = indexReal;
                    } 

                }else{
                    if( indexReal == (*puiNorm) ){
                        return  indexReal;
                    }

                    ///�ƶ�����
                    ///����
                    unsigned int uitemp = m_pPointer[indexReal].uiListPre;
                    if(uitemp)m_pPointer[uitemp].uiListNext = m_pPointer[indexReal].uiListNext;
                    uitemp = m_pPointer[indexReal].uiListNext;
                    if(uitemp)m_pPointer[uitemp].uiListPre = m_pPointer[indexReal].uiListPre;
                    
                    if( indexReal == m_pPointer[(*puiNorm)].uiListPre ){
                        m_pPointer[indexReal].uiListNext = *puiNorm;
                        *puiNorm = indexReal;
                        return  indexReal;
                    }
                    
                    ///������normal������ȥ
                    if(0 == (*puiNorm) ){
                        m_pPointer[indexReal].uiListPre = indexReal;
                        m_pPointer[indexReal].uiListNext = 0;
                        *puiNorm = indexReal;
                    }else{
                        ///������normal������ȥ
                        m_pPointer[indexReal].uiListPre = m_pPointer[(*puiNorm)].uiListPre;
                        m_pPointer[indexReal].uiListNext = *puiNorm ;
                        m_pPointer[(*puiNorm)].uiListPre = indexReal; 
                        *puiNorm = indexReal;
                    }

                }

                return indexReal;
            }
        }
        indexReal = m_pPointer[indexReal].uiListIdxNext ;
    }
        

    if(pInitData){
        if( pInitData->uiNext ){
            ////��ɾ��������
            ////������ֱ�ӷ���
            return 0;
            
        }else{
            unsigned int iTempxx = mallocFree( );
            if(0 == iTempxx){
                iTempxx = doPageOut(0);
                if(0 == iTempxx){
                    THROW(MBC_SHMLruData+5);
                }
            }
    
            m_pPointer[iTempxx].uiListLog  = 0;
            m_pPointer[iTempxx].oListData  = pInitData->oData;
            m_pPointer[iTempxx].sFlag[0] = '\0';
            
            m_pPointer[iTempxx].uiListIdxID = indexBig;
            
            addToNormal( iTempxx );
            addToIndex(indexBig, iTempxx);
            ////indexReal = iTempxx;
            return iTempxx;
        }
    }else{
        
        indexReal = doPageIn(indexBig, false );
//        if(indexReal 
//            && SHM_LRU_FLAG_DEL == m_pPointer[indexReal].sFlag[0] ){
//            ////ɾ������Ŀ �� doPageIn ��ʱ�򣬾��Ѿ��ж��ˣ����ﲻ��Ҫ���ж���
//            
//        }

    }

    (*puiFailCnt)++;

    if( indexReal ){
        return indexReal;
    }

    THROW(MBC_SHMLruData+5);
        

}



template <class T>
void SHMLruData<T>::create(unsigned int itemnumber) 
{
    SHMAccess::create( getNeedSize(itemnumber) );
    open();
    *puiSessionCnt = SHM_LRU_MAX_SESSION;
    *puiExthwm = 98;
    *puiFreshHighPct = 60;
    *puiFreshLowPct = 10;
    *puiFreshInterval=60;
    *puiWrnhwm = 90;

#ifdef SHM_EXPAND_GLOBAL_POS
    m_pLruUnit = 
        (LruUnitData *)( (char *)(*m_poSHM) + (64+(*puiSessionCnt))*sizeof(unsigned int) );
    m_pLruUnit--;  ////���±��1��ʼ��itemnumber
//    SHMAccess::m_iSegment[0] = itemnumber+itemnumber/4;
    *puiSegment = itemnumber+itemnumber/4;
    SHMAccess::m_sSegment[0] = (char *)m_pLruUnit;
        

#else

    m_pPointer = (LruUnitData *)( (char *)(*m_poSHM) + (64+(*puiSessionCnt))*sizeof(unsigned int) );
    m_pPointer--;  ////���±��1��ʼ��itemnumber

#endif
    
    *puiVer = SHM_LRU_CURR_VERSION;
    *puiContentSize = sizeof(T);
    
    *puiFree = 1;
    for(int i =1; i< itemnumber; i++){
        m_pPointer[i].uiListNext = i+1;
        m_pPointer[i].uiListPre = i-1;
    }
    m_pPointer[itemnumber].uiListNext = 0;
    m_pPointer[itemnumber].uiListPre = itemnumber-1;
    
    *puiNorm = 0;
    *puiLog = itemnumber+1;
    m_pPointer[itemnumber+1].uiListNext = itemnumber+2;
    m_pPointer[itemnumber+1].uiListPre = 0;    
    for(int i = 2 ; i< itemnumber/4; i++){
        m_pPointer[itemnumber+i].uiListNext = itemnumber+i+1;
        m_pPointer[itemnumber+i].uiListPre = itemnumber+i-1;
    }
    m_pPointer[itemnumber+itemnumber/4].uiListNext = 0;
    m_pPointer[itemnumber+itemnumber/4].uiListPre = itemnumber+ itemnumber/4 -1;


    *puiDelete = 0;

    *puiFreeCnt = itemnumber;
    *puiNormCnt = 0;
    *puiDirtyCnt = 0;
    *puiLogCnt = 0;
    *puiLogTotalCnt = itemnumber/4;
    *puiInitLength=itemnumber;
    *puiUpperLength=0;
    *puiCommitDownFile=0;
	*puiWriteFileFlag=0;
    *puiExpandMaxSize =0;
    
    *puiLogBegin = itemnumber+1; 
    *puiHashValue = itemnumber;
    
    *puiIterCnt = itemnumber;
    m_iLastTotal = (*puiIterCnt); 

    *puiRealCnt = 0;
    *puiMallocCnt =0;
    *puiExpandCnt = 0;
    
    *puiAccessCnt=0; 
    *puiFailCnt=0;
    *puiHitrate=0;
    *puiState=0;
    *pulStateTime=0;

///    *puiSegment = 0;
    
    *puiDirty =0;      

    setState(0);


    SHMAccess::initExpand( puiExpandCnt, sizeof(LruUnitData), puiSegment); 
    
    if(0 == recoverFromFile() ){
        initFile(true);
    }
    
  
}


////�������±�Ϊ������±�
template <class T>
unsigned int  SHMLruData<T>::malloc()
{
    while(  (*puiFreeCnt)+(*puiNormCnt) < 128  ){
        usleep(10000);
    }

//    while( (*puiUpperLength) && (*puiRealCnt)>=(*puiUpperLength) ){
//        //ALARMLOG28(0,MBC_CLASS_Fatal, 2,"%s","�ڴ�ռ䲻�㣬����LRU�ڴ潫��lru_queue_max_len.");
//        sleep(1);
//    }
    
    while(!m_oDataLock.P());

try{
    if(0 == (*puiDelete) ){
        if( (*puiMallocCnt) < 0x00fffffffe){
            (*puiMallocCnt)++;
            (*puiRealCnt)++;
            getLruDataForWrite(*puiMallocCnt, true );
            m_oDataLock.V();
            return *puiMallocCnt;
        }else{
            m_oDataLock.V();
            return 0;
        }
    }else{
        unsigned int iTemp= *puiDelete;
        unsigned int iReal = getLruDataForMalloc( iTemp ); 

        
        unsigned int * pNext =(unsigned int *)(& m_pPointer[iReal].oListData);
        *puiDelete = (*pNext);
        
        if(iTemp == (*puiDelete) ){
            ////���һ��ɾ������������
            (*puiDelete) =0;
        }
            
        //addToIndex(iTemp, iReal);
        
        (*puiRealCnt)++;
        m_oDataLock.V();
        return iTemp;
    }
}catch(...){
    m_oDataLock.V();
    throw;
}
    m_oDataLock.V();
    return 0;
}


////�������±�Ϊ������±�
template <class T>
unsigned int  SHMLruData<T>::malloc(T & indata )
{
    while(  (*puiFreeCnt)+(*puiNormCnt) < 128  ){
        usleep(10000);
    }
//    while( (*puiUpperLength) && (*puiRealCnt)>=(*puiUpperLength) ){
//        //ALARMLOG28(0,MBC_CLASS_Fatal, 2,"%s","�ڴ�ռ䲻�㣬����LRU�ڴ潫��lru_queue_max_len.");
//        sleep(1);
//    }
    while(!m_oDataLock.P());

try{
    if(0 == (*puiDelete) ){
        if( (*puiMallocCnt) < 0x00fffffffe){
            (*puiMallocCnt)++;
            (*puiRealCnt)++;
            unsigned int indexReal = getLruDataForWrite(*puiMallocCnt, true );
            if(indexReal){ 
                m_pPointer[indexReal].oListData = indata;
                m_oDataLock.V();
                return *puiMallocCnt;
            }else{
                m_oDataLock.V();
                return 0;
            }
        }else{
            m_oDataLock.V();
            return 0;
        }
    }else{
        unsigned int iTemp= *puiDelete;
        unsigned int iReal = getLruDataForMalloc( iTemp ); 

        
        unsigned int * pNext =(unsigned int *)(& m_pPointer[iReal].oListData);
        *puiDelete = (*pNext);
        
        if(iTemp == (*puiDelete) ){
            ////���һ��ɾ������������
            (*puiDelete) = 0;
        }
        //addToIndex(iTemp, iReal);
        (*puiRealCnt)++;

        m_pPointer[iReal].oListData = indata;
        m_oDataLock.V();
        return iTemp;
    }
}catch(...){
    m_oDataLock.V();
    throw;
}
    m_oDataLock.V();
    return 0;
}

template <class T>
void SHMLruData<T>::reset()
{
    ////ֱ��resetΣ����̫��
    
}


template <class T>
bool SHMLruData<T>::revoke(unsigned int indexBig)
{        
    while(  (*puiFreeCnt)+(*puiNormCnt) < 128  ){
        usleep(10000);
    }
    while(!m_oDataLock.P());
try{
    unsigned int indexReal = getLruDataForWrite(indexBig);
    if(indexReal) {
        m_pPointer[indexReal].sFlag[0] = SHM_LRU_FLAG_FORDEL;
        m_oDataLock.V();
        return true;
    }
}catch(...){
    m_oDataLock.V();
    throw;
}
    m_oDataLock.V();
    return false;
    
}




template <class T>
void SHMLruData<T>::showDetail()
{
    if(m_iLastTotal != (*puiIterCnt)) {
        SHMAccess::reopen();
        m_iLastTotal = (*puiIterCnt);
    }

    char sW[4][16]={
        "�Զ�����",
        "��ʱд",
        "����д",
        "ʵʱд"};
    
    unsigned int state;
    long  thetime;
    if(  getState(state,  thetime) ){
        if( state>3)
            printf("�ڴ�д��ʽδ֪, ���һ���޸�д��ʽ��ʱ��:%s", ctime(&thetime) );
        else
            printf("�ڴ�д��ʽ:%s, ���һ���޸�д��ʽ��ʱ��:%s", sW[state], ctime(&thetime) );
    }
   unsigned int initsize=getNeedSize((*puiInitLength));
   unsigned int expsize=SHMAccess::m_lNextSize * (*(SHMAccess::m_piExpandCnt));
   unsigned int iiUsed=initsize+expsize ;
   iiUsed = iiUsed < getExpandMaxSize() ? iiUsed : getExpandMaxSize() ;
   printf("%-16s:%-10u %-16s:%-10u %-16s:%-10u %-16s:%-10u\n", 
      "������", getCount(), 
      "�ڴ�����", getTotal(),
      "�ڴ��С(�ֽ�)",iiUsed,
      "�ڴ����ֵ(�ֽ�)",getExpandMaxSize() );
      //"�ڴ�(����+��־)", getMemCount()  );
    printf("%-16s:%-10u %-16s:%-10u %-16s:%-10u\n", 
      "�ڴ�(����)",getMemDataCount(),
      "�ڴ�ɻ�����",(*puiNormCnt),
      "�ڴ��������",(*puiFreeCnt));
    printf("%-16s:%-3u%%       %-16s:%-10u %-16s:%-3u%%      \n", 
      "�ڴ�ռ�ðٷֱ�",100*getCount()/getTotal(),
      "�ڴ�����������",getDirtyCount(),
      "�ڴ������ݰٷֱ�",getDirtyPercent());
    printf("%-16s:%-10u %-16s:%-10u %-16s:%-3u%%      \n", 
      "δ�ύ����",(*puiLogCnt),
      "�ύ��δ�������",getDirtyCount()-(*puiLogCnt),
      "������־�ٷֱ�",getLogPercent());

    printf("%-16s:%-3u%%       %-16s:%-10u %-16s:%-10u\n", 
      "������",getHitrate(),
      "���д���",(*puiAccessCnt)>(*puiFailCnt)? ((*puiAccessCnt)-(*puiFailCnt)):0 ,
      "���뻻������", (*puiAccessCnt)>(*puiFailCnt)? (*puiFailCnt): (*puiAccessCnt) );
    printf("%-16s:%-10u %-16s:%-10u %-16s:%-10u\n", 
      "LRU���г���", getLruLength(), 
      "LRU���г�ʼ����", (*puiInitLength),
      "LRU������󳤶�", (*puiUpperLength)  );

    printf("%-16s:%-3u%%       %-16s:%-3u%%       %-16s:%-10u\n", 
      "�Զ���չ��ֵ",  getExthwm(), 
      "��ˮλ�澯��ֵ", getWrnhwm(),
      "��ʱд���(��)", getFreshInt()  );        

    printf("%-16s:%-3u%%       %-16s:%-3u%%       %-16s:%-10s\n", 
      "������ˢ�·�ֵH", getFreshHighPct(), 
      "������ˢ�·�ֵL", getFreshLowPct(),
      "ҵ��ˢ���Ƿ��ύ", getCommitDownFile()>0? "true":"false" );  
 
                
    SHMAccess::showExpandInfo();

}

template <class T>
void SHMLruData<T>::showNormal( bool bDetail )
{
    unsigned int *pui = puiNorm;
    unsigned int ucnt = 0;
    while( *pui){
        if(bDetail)
            printf("%u-pre%u,", *pui,  m_pPointer[*pui].uiListPre );
        
        pui = &m_pPointer[*pui].uiListNext;
        ucnt++;
    }
    printf("normal tree cnt %u, head:%u,tail:%u\n", ucnt, (*puiNorm), m_pPointer[*puiNorm].uiListPre );
}

template <class T>
void SHMLruData<T>::open()
{
    
    puiVer = (unsigned int *)( (char *)(*m_poSHM) ) ;

    puiFree = puiVer+1;    
    puiNorm = puiVer+2;
    puiLog = puiVer+3;
    puiDelete = puiVer+4;

    puiFreeCnt = puiVer+5;
    puiNormCnt = puiVer+6;
    puiDirtyCnt = puiVer+7;
    puiLogCnt = puiVer+8;
    puiLogTotalCnt =  puiVer+9;
    
    puiIterCnt = puiVer+10;
    puiRealCnt = puiVer+11;
    puiExpandCnt = puiVer + 12;    //��չ�ڴ�Ĵ���
    puiMallocCnt = puiVer+13;
    puiSessionCnt = puiVer+14;
    
    puiState = puiVer+15; 
    pulStateTime = (long *) (puiVer+16);  //long��Ҫ8�ֽڶ��룺��ռ������intλ�ã�16��17

    puiFailCnt  = puiVer+18;
    puiAccessCnt= puiVer+19;
    puiHitrate  = puiVer+20;    

    puiLogBegin = puiVer+21; 
    puiHashValue= puiVer+22;

  

    puiSegment = puiVer+23; ////��������8��int������¼��չ���ڴ���ƫ�Ʒֶ����
    ////segment end puiVer+23+1+MAX_SHM_EX_CNT =  puiVer+32
    ////�����puiVer+ 32 ��ʼ��puiVer+52 Ԥ���Ժ���չʹ�á�
    puiExpandMaxSize=puiVer+53;
    puiWriteFileFlag=puiVer+54;
    puiCommitDownFile=puiVer+55;
    puiInitLength=puiVer+56;
    puiUpperLength=puiVer+57;
    puiContentSize = puiVer+58;
    puiFreshInterval= puiVer+59;
    puiWrnhwm = puiVer+60;
    puiFreshHighPct = puiVer+61;
    puiFreshLowPct = puiVer+62;
    puiExthwm = puiVer+63;////����Ԥ��ʹ�õķ�ֵ���Զ���չ��
    puiDirty = puiVer+64;  
    
#ifdef SHM_EXPAND_GLOBAL_POS
    m_pLruUnit = 
        (LruUnitData *)( (char *)(*m_poSHM) + (64+(*puiSessionCnt))*sizeof(unsigned int) );
    m_pLruUnit--;  ////���±��1��ʼ��itemnumber
//    SHMAccess::m_iSegment[0] = (*puiLogBegin)+(*puiLogTotalCnt)-1; ////log ����ĵط�
    (*puiSegment) = (*puiLogBegin)+(*puiLogTotalCnt)-1;
    SHMAccess::m_sSegment[0] = (char *)m_pLruUnit;
#else

    m_pPointer = (LruUnitData *)( (char *)(*m_poSHM) + (64+(*puiSessionCnt))*sizeof(unsigned int) );
    m_pPointer--;  ////���±��1��ʼ��itemnumber

#endif

    SHMAccess::initExpand( puiExpandCnt, sizeof(LruUnitData), puiSegment);
    
}


////����ֵΪ���ڵ��±�
template <class T>
unsigned int SHMLruData<T>::mallocFree( )
{
    unsigned int iRet=0;
    if( puiFree && (*puiFree) ){
        iRet = *puiFree;
        //unsigned int uitemp = m_pPointer[iRet].uiListPre;
        //if(uitemp) m_pPointer[uitemp].uiListNext = m_pPointer[iRet].uiListNext;
        *puiFree = m_pPointer[iRet].uiListNext;
        (*puiFreeCnt)--;
        if(*puiFree) m_pPointer[*puiFree].uiListPre = m_pPointer[iRet].uiListPre;        
        
        m_pPointer[iRet].uiListPre =0;
        m_pPointer[iRet].uiListNext=0;
        
        return iRet;
    }
    
    iRet = doPageOut(0);
//    if(0 == iRet)
//        THROW(MBC_SHMLruData+5);

    return iRet;

}


////����ֵΪ���ڵ��±�
////mallocLog �����Ŀռ䣬�������������memset�������ڴ�ȫ��
template <class T>
unsigned int SHMLruData<T>::mallocLog()
{
    unsigned int iTemp=0;
    if( (*puiLogCnt) < (*puiLogTotalCnt) ){
        iTemp = *puiLog;
        *puiLog = m_pPointer[iTemp].uiListNext;
        (*puiLogCnt)++;
        return iTemp;
    }
    
    unsigned int iTemp2 = mallocFree( );
    if(0 == iTemp2)
        return 0 ;

    (*puiLogCnt)++;
    return iTemp2;

}


////indexBigΪ������±꣬indexΪ���ڵ��ڴ��±�
template <class T>
unsigned int SHMLruData<T>::addToIndex(unsigned int indexBig, unsigned int index )
{
    if( indexBig < 1 || indexBig > (*puiMallocCnt) )
        THROW(MBC_SHMLruData+2);
        
    unsigned int indexReal = m_pPointer[(indexBig-1)%(*puiHashValue)+1].uiIndex;
    if(indexReal
      && m_pPointer[indexReal].uiListIdxID  == indexBig ){
        return indexReal;
    }
    if( indexReal==index 
      && 0==m_pPointer[indexReal].uiListIdxID ){
        return indexReal;
    }
    

    m_pPointer[index].uiListIdxNext = indexReal;
    m_pPointer[(indexBig-1)%(*puiHashValue)+1].uiIndex = index;

    
    return indexReal;
}

////indexBigΪ������±꣬
template <class T>
unsigned int SHMLruData<T>::delFromIndex(unsigned int indexBig, int iFlag )
{
    if( indexBig < 1 || indexBig > (*puiMallocCnt) )
        THROW(MBC_SHMLruData+2);
    
    unsigned int *pPre = &m_pPointer[(indexBig-1)%(*puiHashValue)+1].uiIndex;
    unsigned int indexReal = *pPre;
    
    while(indexReal){
        if( m_pPointer[indexReal].uiListIdxID  == indexBig ){
            if(pPre){
                (*pPre)= m_pPointer[indexReal].uiListIdxNext;
            }
            ///m_pPointer[indexReal].uiListIdxNext =0 ;
            m_pPointer[indexReal].uiListIdxID =  0 ;
            m_pPointer[indexReal].sFlag[0] = 0;

            if(1==iFlag){
                addToDelIndex(indexBig, indexReal);
            }
            return indexReal;

        }
        pPre = &m_pPointer[indexReal].uiListIdxNext;
        indexReal = m_pPointer[indexReal].uiListIdxNext;
 
    }
    
    return 0;
}



////������� indexBig Ϊ������±�,indexΪĿǰ���ڴ���±�
template <class T>
void SHMLruData<T>::addToDelIndex( unsigned int indexBig, unsigned int index)
{
/*

    ///������ DeleteIdx ������ȥ
    m_pPointer[index].uiListIdxID = indexBig;
    m_pPointer[index].uiListIdxNext = *puiDelete ;
    (*puiDelete) = indexBig;

    m_pPointer[index].sFlag[0] = SHM_LRU_FLAG_DEL;

*/


}


////����ֵΪ���ڵ��±�
////�������indexΪ������±꣬����Ϊ0��ifModifyΪtrue��ʾ�����Ҫ�����޸ģ�false��ʾ���޸�

template <class T>
unsigned int SHMLruData<T>::doPageIn(unsigned int indexBig, bool ifModify )
{
    if(0 == indexBig)
        THROW(MBC_SHMLruData+2);

    unsigned int iTemp = mallocFree( );
    if(0==iTemp){
        iTemp=doPageOut(0);
        if(0 == iTemp)
        {
        	while(1)
        	{
        		usleep(10000);
        	}
            THROW(MBC_SHMLruData+5);
         }
    }

    
    if( ifModify ) addToDirty(iTemp, m_iSessionID);
    else addToNormal(iTemp);
    

    FILE *fp;
    FileLruData<T> oFiledata;
    int iErrID = 0;
    
    snprintf(m_sFileExName,sizeof(m_sFileExName)-1,
        "%s.%04u", m_sFileName, (indexBig-1)/m_iCntPerFile);
    
    if ((fp = fopen (m_sFileExName, "rb+")) != NULL) {
       
        long lFileOffset = ((indexBig-1)%m_iCntPerFile)*sizeof(FileLruData<T>);
        fseek(fp, 0,  SEEK_END);

        if( ( lFileOffset + sizeof(FileLruData<T>) ) > ftell(fp) ){
            ////��һ��malloc������
            fseek(fp, 0,  SEEK_END);

            memset(&oFiledata, 0, sizeof(oFiledata) );
            while(  ( lFileOffset + sizeof(FileLruData<T>) ) > ftell(fp) ){
                int i = fwrite(&oFiledata, sizeof(oFiledata), 1, fp);
                if (i != 1){
                    fclose (fp);
                    iErrID = MBC_SHMLruData+9;
                    goto __LRU_PAGEIN_ERR;
    
                }
            }
            fseek(fp, lFileOffset ,SEEK_SET);
            if( lFileOffset != ftell(fp) ){
                fclose (fp);
                iErrID = MBC_SHMLruData+8;
                goto __LRU_PAGEIN_ERR;

            }
            fclose (fp);
            m_pPointer[iTemp].oListData = oFiledata.oData;
            m_pPointer[iTemp].uiListIdxID = indexBig;
            if(oFiledata.uiNext){
                m_pPointer[iTemp].sFlag[0] = SHM_LRU_FLAG_DEL;
                m_pPointer[iTemp].uiListIdxNext = oFiledata.uiNext;

            }else{
                m_pPointer[iTemp].sFlag[0]=SHM_LRU_FLAG_NORM;
                addToIndex(indexBig, iTemp);
            }
            return iTemp;
        }
        fseek(fp, lFileOffset ,SEEK_SET);
        int i = fread(&oFiledata, sizeof(FileLruData<T>), 1, fp );
        if(i != 1 ){
            fclose (fp);
            iErrID = MBC_SHMLruData+8;
            goto __LRU_PAGEIN_ERR;
        }
        m_pPointer[iTemp].oListData = oFiledata.oData;
        m_pPointer[iTemp].uiListIdxID = indexBig;
        if( oFiledata.uiNext ){
            ///����ɾ������Ŀ
            
            if( !ifModify ){
                ////ɾ������Ŀ�����ط����ֻ֧�ָ�ֵ����֧�ֲ�ѯ
                fclose (fp);
                iErrID = MBC_SHMLruData+6;
                goto __LRU_PAGEIN_ERR; 
            }
            m_pPointer[iTemp].sFlag[0] = SHM_LRU_FLAG_DEL;
            //m_pPointer[iTemp].uiListIdxNext = oFiledata.uiNext;
            
        }else{
            m_pPointer[iTemp].sFlag[0]=SHM_LRU_FLAG_NORM;
            addToIndex(indexBig, iTemp);
        }
        fclose (fp);
        return iTemp;
    }
    
    iErrID = MBC_SHMLruData+7;
    
__LRU_PAGEIN_ERR:


    if( ifModify ) doDirtyOut(iTemp);
    else doPageOut(iTemp );

    addToFree(iTemp);

    THROW(iErrID);
    
}


////����ֵΪ���ڵ��±�
////�������indexΪ���ڵ��±꣬���Ϊ0����ʾ������õĻ���������index����
template <class T>
unsigned int SHMLruData<T>::doPageOut(unsigned int index)
{
    if(0 ==index){
        index = (*puiNorm);
        if(0==index){
            return 0;
        }
        index = m_pPointer[index].uiListPre; 

    }
    
    if(index!=0 && index <= (*puiIterCnt) ){
        ///����
        unsigned int uitemp = m_pPointer[index].uiListPre;

        if(index == m_pPointer[*puiNorm].uiListPre )
            m_pPointer[*puiNorm].uiListPre = m_pPointer[index].uiListPre;
        
        if(uitemp)
            m_pPointer[uitemp].uiListNext = m_pPointer[index].uiListNext;
        
        uitemp = m_pPointer[index].uiListNext;
        if(uitemp)
            m_pPointer[uitemp].uiListPre = m_pPointer[index].uiListPre;
        
        
        if(0 ==  m_pPointer[index].uiListIdxID ){
            printf("memory fault.");
        }
        delFromIndex(m_pPointer[index].uiListIdxID, 0);

        m_pPointer[index].uiListPre  = 0;
        m_pPointer[index].uiListNext = 0;
        m_pPointer[index].uiListIdxID= 0;
        m_pPointer[index].uiListLog  = 0;
        m_pPointer[index].oListData  = m_oDelUnit;
        m_pPointer[index].sFlag[0] = '\0';

        
        
        (*puiNormCnt)--;
        if( index == (*puiNorm) ) {
            (*puiNorm)=0;
        }
        
        return index;
        
    }
    
    return 0;
    
}

template <class T>
void SHMLruData<T>::doListIn(unsigned int indexReal, unsigned int * pHead, unsigned int *pCnt )
{
    if(0 == (*pHead) ){
        m_pPointer[indexReal].uiListPre = indexReal;
        m_pPointer[indexReal].uiListNext = 0;
        
    }else{
        ///������head����ȥ
        m_pPointer[indexReal].uiListPre = m_pPointer[(*pHead)].uiListPre;
        m_pPointer[indexReal].uiListNext = *pHead ;
        m_pPointer[(*pHead)].uiListPre = indexReal;
        
    }
    
    (*pHead)=indexReal;
    (*pCnt)++;

    return ;
}

template <class T>
unsigned int SHMLruData<T>::doListOut(unsigned int indexReal, unsigned int * pHead, unsigned int *pCnt )
{
    if(indexReal!=0 && indexReal <= (*puiIterCnt) ){
        ///����
        if( indexReal == (*pHead ) ){
            ///list head
            (*pHead ) =  m_pPointer[indexReal].uiListNext;
            if( *pHead ){
                m_pPointer[*pHead].uiListPre  = m_pPointer[indexReal].uiListPre;
            }

        }else{

            unsigned int uitemp = m_pPointer[indexReal].uiListPre;
            if(uitemp)
                m_pPointer[uitemp].uiListNext = m_pPointer[indexReal].uiListNext;
            
            uitemp = m_pPointer[indexReal].uiListNext;
            if(uitemp){
                m_pPointer[uitemp].uiListPre = m_pPointer[indexReal].uiListPre;
            }
    
            if( indexReal == m_pPointer[*pHead].uiListPre ){
                ////list tail 
                m_pPointer[*pHead].uiListPre = m_pPointer[indexReal].uiListPre;
            }
            
        }
        m_pPointer[indexReal].uiListPre=0;
        m_pPointer[indexReal].uiListNext=0;
        
        unsigned int uitemp = m_pPointer[indexReal].uiListLog;
        if(uitemp){
            if( uitemp < (*puiLogBegin) 
                || uitemp >= (*puiLogBegin)+(*puiLogTotalCnt)  ){
                ////��������Log�ռ�
                addToFree(uitemp);
                (*puiLogCnt)--;
            }else{
                ////Logԭ���Ŀռ�
                addToFreeLog(uitemp);
            }
        }
        m_pPointer[indexReal].uiListLog = 0;
        
        if(pCnt)
            (*pCnt)--;

        return indexReal;

    }
    return 0;
}

template <class T>
unsigned int SHMLruData<T>::doListMoveFirst(unsigned int indexReal, unsigned int * pHead)
{
    if(indexReal!=0 && indexReal <= (*puiIterCnt) ){
        ///����
        
        if( indexReal == (*pHead) ){
            ////list head 
            return  indexReal;
        }

        ///�ƶ�����
        ///����
        unsigned int uitemp = m_pPointer[indexReal].uiListPre;
        if(uitemp)m_pPointer[uitemp].uiListNext = m_pPointer[indexReal].uiListNext;
        uitemp = m_pPointer[indexReal].uiListNext;
        if(uitemp){
            m_pPointer[uitemp].uiListPre = m_pPointer[indexReal].uiListPre;
        }
        
        if( indexReal == m_pPointer[*(pHead)].uiListPre ){
            ////list tail 
            m_pPointer[indexReal].uiListNext = *pHead;
            *pHead = indexReal;
            return  indexReal;
        }
        
        ///������head����ȥ
        if(0 == (*pHead) ){
            m_pPointer[indexReal].uiListPre = indexReal;
            m_pPointer[indexReal].uiListNext = 0;
            *pHead = indexReal;
        }else{
            m_pPointer[indexReal].uiListPre = m_pPointer[(*pHead)].uiListPre;
            m_pPointer[indexReal].uiListNext = *pHead ;
            m_pPointer[(*pHead)].uiListPre = indexReal; 
            *pHead = indexReal;
        }
        return  indexReal;
    }
    return 0;
}

////����ֵΪ���ڵ��±�
////�������indexΪ���ڵ��±꣬����Ϊ0
template <class T>
unsigned int SHMLruData<T>::doDirtyOut(unsigned int index)
{
     unsigned int * pDirty = (puiDirty+m_iSessionID);
    return doListOut( index, pDirty, puiDirtyCnt);

}


////�������indexΪ���ڵ��±�
template <class T>
void SHMLruData<T>::addToNormal( unsigned int index )
{
    
    if(0 == (*puiNorm) ){
        m_pPointer[index].uiListPre = index;
        m_pPointer[index].uiListNext = 0;
        
    }else{
        ///������normal������ȥ
        m_pPointer[index].uiListPre = m_pPointer[(*puiNorm)].uiListPre;
        m_pPointer[index].uiListNext = *puiNorm ;
        m_pPointer[(*puiNorm)].uiListPre = index;
        
    }
    
    (*puiNorm)=index;
    (*puiNormCnt)++;

}


////�������indexΪ���ڵ��±�
template <class T>
void SHMLruData<T>::addToDirty( unsigned int index, int i_iSession )
{
     unsigned int * pDirty = puiDirty+i_iSession;

    if(0 == (*pDirty) ){
        m_pPointer[index].uiListPre = 0;
        m_pPointer[index].uiListNext = 0;

    }else{
        ///������Dirty������ȥ
        m_pPointer[index].uiListPre = m_pPointer[(*pDirty)].uiListPre;
        m_pPointer[index].uiListNext = *pDirty ;
        m_pPointer[(*pDirty)].uiListPre = index; 

    }

    (*pDirty)=index;
    if(0!=i_iSession) (*puiDirtyCnt)++;

}



////�������indexΪ���ڵ��±�
template <class T>
void SHMLruData<T>::addToFree( unsigned int index )
{
 
    if(0 == (*puiFree) ){
        m_pPointer[index].uiListPre = 0;
        m_pPointer[index].uiListNext = 0;

    }else{
        ///������Free������ȥ
        m_pPointer[index].uiListPre = m_pPointer[(*puiFree)].uiListPre;
        m_pPointer[index].uiListNext = *puiFree ;
        m_pPointer[(*puiFree)].uiListPre = index; 
    }

    (*puiFree)=index;    
    (*puiFreeCnt)++;
}


////�������indexΪ���ڵ��±�
template <class T>
void SHMLruData<T>::addToFreeLog( unsigned int index )
{
    
    if(0 == (*puiLog) ){
        m_pPointer[index].uiListPre = 0;
        m_pPointer[index].uiListNext = 0;

    }else{
        ///������FreeLog������ȥ
        m_pPointer[index].uiListPre = m_pPointer[(*puiLog)].uiListPre;
        m_pPointer[index].uiListNext = *puiLog ;
        m_pPointer[(*puiLog)].uiListPre = index; 
    }

    (*puiLog)=index;    
    (*puiLogCnt)--;
}





////mallocLog �������ڴ�飬ֻ��ͨ��commit��rollback�����·ŵ�����������ȥ����Ϊ������������¼���ˡ�
template <class T>
unsigned int SHMLruData<T>::commit()
{
     unsigned int * pDirty = puiDirty+m_iSessionID;
    unsigned int uCnt=0;
    while(!m_oDataLock.P());
    while( *pDirty ){
        unsigned int uitemp = m_pPointer[ *pDirty ].uiListLog;
        unsigned int uinext = m_pPointer[ *pDirty ].uiListNext ;
        

        
        if( SHM_LRU_FLAG_FORDEL ==  m_pPointer[ *pDirty ].sFlag[0]  ){
            m_pPointer[ *pDirty ].sFlag[0] = SHM_LRU_FLAG_DEL;
//            if( SHM_LRU_FLAG_DEL !=  m_pPointer[uitemp].sFlag[0]  ){
//                ////���ݱ�ɾ����
//                (*puiRealCnt)--;
//            }
            ////���ݱ�ɾ����
            (*puiRealCnt)--;
                            
            unsigned int iBig = m_pPointer[*pDirty].uiListIdxID;

            unsigned int * iDNext = (unsigned int *)(&m_pPointer[*pDirty].oListData);
            if( 0 == (*puiDelete) )
                (*iDNext) = iBig;
            else
                (*iDNext) = (*puiDelete);
                

            (*puiDelete) = iBig;
            
              
        }else if(SHM_LRU_FLAG_NORM == m_pPointer[ *pDirty ].sFlag[0]  ){
            m_pPointer[ *pDirty ].sFlag[0] = SHM_LRU_FLAG_MODIFY;
        }
        
        m_pPointer[ *pDirty ].uiListLog = 0;
        
        addToDirty(*pDirty, 0);
        
        *pDirty = uinext;
        if(uinext) m_pPointer[ uinext ].uiListPre = 0;
        
        ///*(puiDirty+m_iSessionID ) = uinext;
        if(uitemp){
            if( uitemp < (*puiLogBegin) 
                || uitemp >= (*puiLogBegin)+(*puiLogTotalCnt) ){
                ////��������Log�ռ�
                addToFree(uitemp);
                (*puiLogCnt)--;
            }else{
                ////Logԭ���Ŀռ�
                addToFreeLog(uitemp);
            }
        }
        uCnt++;


    }
    m_oDataLock.V();
    return uCnt;

}

template <class T>
unsigned int SHMLruData<T>::rollback()
{
     unsigned int * pDirty = puiDirty+m_iSessionID;
    unsigned int uCnt=0;
    while(!m_oDataLock.P());    
    while( *pDirty ){
        unsigned int uitemp = m_pPointer[ *pDirty ].uiListLog;
        unsigned int uinext = m_pPointer[ *pDirty ].uiListNext ;
        
        if(uitemp){
            if(m_pPointer[uitemp].uiListLog != m_iSessionID  ){
                THROW(MBC_SHMLruData+10);
            }
            ////�ָ����ݵ�����
            m_pPointer[ *pDirty ].oListData = m_pPointer[uitemp].oListData;
            m_pPointer[ *pDirty ].sFlag[0] = m_pPointer[uitemp ].sFlag[0] ;

        }
        if(SHM_LRU_FLAG_NORM == m_pPointer[ *pDirty ].sFlag[0] ){
            m_pPointer[ *pDirty ].sFlag[0] = SHM_LRU_FLAG_MODIFY;
        }else if( SHM_LRU_FLAG_DEL == m_pPointer[ *pDirty ].sFlag[0] ){
            ///�ոմ�ɾ�������ݷ��䵽�Ŀռ���в���
            ///����ع���������ƫ���ջ�
            (*puiRealCnt)--;
            unsigned int iBig = m_pPointer[*pDirty].uiListIdxID;
            unsigned int * iDNext = (unsigned int *)(&m_pPointer[*pDirty].oListData);
            if( 0 == (*puiDelete) )
                (*iDNext) = iBig;
            else
                (*iDNext) = (*puiDelete);
                

            (*puiDelete) = iBig;
        }
        addToDirty(*pDirty, 0);
        
        m_pPointer[ *pDirty ].uiListLog = 0;
        *pDirty = uinext;
        if(uinext) m_pPointer[ uinext ].uiListPre = 0;
        
        if(uitemp){
            if( uitemp < (*puiLogBegin) 
                || uitemp >= (*puiLogBegin)+(*puiLogTotalCnt)  ){
                ////��������Log�ռ�
                addToFree(uitemp);
                (*puiLogCnt)--;
            }else{
                ////Logԭ���Ŀռ�
                addToFreeLog(uitemp);
            }

        }
        uCnt++;

    }
    m_oDataLock.V();
    return uCnt;

}


/////�ύָ��������session,�������Ӧ�ñȽ���
template <class T>
void SHMLruData<T>::commitSession(unsigned int iSession)
{
    if(m_iSessionID){
        THROW(MBC_SHMLruData+10);
    }
    
    if( iSession >= SHM_LRU_MAX_SESSION )
        THROW(MBC_SHMLruData+11);

    if(m_iLastTotal != (*puiIterCnt)) {
        SHMAccess::reopen();
        m_iLastTotal = (*puiIterCnt);
    }
        
    unsigned int * pDirty = puiDirty+iSession;


}



////session��������ר�Ž������ع���
template <class T>
void SHMLruData<T>::rollbackSession (unsigned int iSession)
{
    if(m_iSessionID){
        THROW(MBC_SHMLruData+10);
    }
    if( iSession >= SHM_LRU_MAX_SESSION )
        THROW(MBC_SHMLruData+11);

    if(m_iLastTotal != (*puiIterCnt)) {
        SHMAccess::reopen();
        m_iLastTotal = (*puiIterCnt);
    }

    unsigned int * pDirty = puiDirty+iSession;

 
}


template <class T>
bool SHMLruData<T>::getByFile( unsigned int iOffset, T& outT )
{
    
    int iSeq=(iOffset-1)/m_iCntPerFile;
    FileLruData<T> oFileData;
    FILE *fp = prepareFile(iSeq, false, false);
    if(fp){
        long itemp = ((iOffset-1)%m_iCntPerFile)*sizeof(FileLruData<T>);
        fseek(fp, itemp, SEEK_SET);
        int i = fread( &oFileData, sizeof(FileLruData<T> ), 1, fp  );
        if(i!=1){
            fclose(fp);
            return false;
        }
        outT=oFileData.oData;
        return true;
    }
    
    return false;
    
}


/////д�����̽������
template <class T>
unsigned int SHMLruData<T>::writeToFile(bool bForce )
{
//    if(m_iSessionID){
//        THROW(MBC_SHMLruData+10);
//    }
    
    static unsigned int uiLastHash = 0;
    static unsigned int uiLastIter = 0;
    static unsigned int uiLastReal = 0;
    static unsigned int uiLastMalloc = 0;
    static unsigned int uiLastDelete = 0;
    
    
    
    if( (*puiContentSize)!= sizeof(T) )
        return 0;
    
    if( getCommitDownFile()== 1 ){
        bForce = true;
    }
    
    ///��ʱ ����
    if( !bForce ){
        switch( *puiState){
            case 1:
            {
                ////��ʱд
            	struct timeval tv;
            	struct timezone tz;
                if(0 == gettimeofday (&tv , &tz) ){
                    if(tv.tv_sec >= m_lClock + SHM_LRU_DIRTY_CLOCKCNT ){
                        ///ʱ�䵽
                        m_lClock = tv.tv_sec;
                        printf("���ﶨʱ��ֵ\n");
                        break;
                    }else{
                        return 0;
                    }
                }
                //�˴���break,�����ȡʱ��ʧ�ܣ�ֱ��ת�ɶ���д��ʽ��
                //break;
            }
            case 2:
                ////����д
                if( getDirtyPercent() < SHM_LRU_DIRTY_HIGH_LEVEL){
                    ////δ�ﵽ��������
                    return 0;
                }
                printf("���ﶨ����ֵ\n");

                break;
            case 3:
                ////ʵʱд
                if( getCommitDirtyCount() == 0 )
                    return 0;
                
                break;
            default:
                ////�����Զ����ƣ��ﵽ�����ߴﵽʱ�䣬����д
                if( getDirtyPercent() < SHM_LRU_DIRTY_HIGH_LEVEL){
                    ////δ�ﵽ��������
                	struct timeval tv;
                	struct timezone tz;
                    if(0 == gettimeofday (&tv , &tz) ){
                        if(tv.tv_sec >= m_lClock + SHM_LRU_DIRTY_CLOCKCNT ){
                            ///ʱ�䵽
                            m_lClock = tv.tv_sec;
                            printf("���ﶨʱ��ֵ\n");
                        }else{
                            return 0;
                        }
                    }else{
                        return 0;
                    }
                }else{
                    printf("���ﶨ����ֵ\n");
                }
                
                break;

        }
		if(getWriteFileFlag())
			return 0;
    }

    if(m_iLastTotal != (*puiIterCnt)) {
        SHMAccess::reopen();
        m_iLastTotal = (*puiIterCnt);
    }


    FILE *fpHead;
    if ((fpHead = fopen (m_sFileName, "rb+")) == NULL) {
        THROW(MBC_SHMLruData+7);
    }
    if( uiLastHash != (*puiHashValue) 
        || uiLastIter != (*puiIterCnt)
        || uiLastReal!=(*puiRealCnt)
        || uiLastMalloc!=(*puiMallocCnt)
        || uiLastDelete != (*puiDelete)
        ){
        
        FileLruHead oHead;
        writeFileHead( fpHead, oHead, 1);

        uiLastHash = oHead.uiHashValue;
        uiLastIter = oHead.uiIterCnt;
        uiLastReal = oHead.uiRealCnt;
        uiLastMalloc = oHead.uiMallocCnt;
        uiLastDelete = oHead.uiDelete;

    }            



    unsigned int iWriteCnt = 0;

    if( getCommitDirtyCount() > 0  ){
        
        iWriteCnt =  writeFileBatch( fpHead,  bForce );
        if( iWriteCnt != 0){
            fflush(fpHead);
            fclose (fpHead);
            return iWriteCnt;
        }
        printf("д0�����ļ�\n");

    }

    fclose(fpHead);
    fpHead=0;    
    return iWriteCnt;
}


template <class T>
unsigned int SHMLruData<T>::writeFileHead( FILE * fp, FileLruHead & oHead, unsigned int uiState )
{
    memset(&oHead, 0, sizeof(FileLruHead) );

    oHead.uiHashValue=(*puiHashValue);
    oHead.uiIterCnt=(*puiIterCnt);
    oHead.uiRealCnt=(*puiRealCnt);
    oHead.uiMallocCnt=(*puiMallocCnt);
    oHead.uiExpandCnt= (*puiExpandCnt);

    oHead.uiVer=(*puiVer);
    oHead.lKey = m_lSHMKey;
    oHead.uiState = uiState;
    oHead.uiDelete = (*puiDelete);
    oHead.uiContentSize=(*puiContentSize);

    snprintf(oHead.sName, sizeof(oHead.sName), "V%d", (*puiVer)  );
    
    fseek(fp, 0,  SEEK_SET);
    int i = fwrite(&oHead, sizeof(oHead), 1, fp);
    if(i!=1){
        return 0;
    }

    return sizeof(FileLruHead);
    
}

template <class T>
unsigned int SHMLruData<T>::expandFile(FILE * fp )
{
    unsigned int iRet=0;
    
    unsigned long lneed;
    
//    if( (*puiMallocCnt) < (*puiIterCnt) )
//        lneed  =  (unsigned long)(*puiIterCnt)*sizeof(FileLruData<T>) +  sizeof(FileLruHead);
//    else
//        lneed  =  (unsigned long)(*puiMallocCnt)*sizeof(FileLruData<T>) +  sizeof(FileLruHead);
//    
    lneed = m_iCntPerFile * sizeof(FileLruData<T>);
   fseek(fp, 0,  SEEK_END);

    if( ftell(fp) < lneed ){
        ////�ļ�ĩβ׷������
        FileLruData<T> oFileData[8192];
        memset( &oFileData, 0, sizeof(oFileData) );
        unsigned int icnt = ( lneed - ftell(fp) - 1 )/sizeof(FileLruData<T>) + 1;
        int i = 0;
        iRet = icnt;
        while( icnt ){
            if(icnt>=8192){
                i = fwrite(&oFileData, sizeof(FileLruData<T>)*8192, 1, fp);
                if(i != 1){

                    return 0xffffffff;
                    
                }
                icnt -= 8192;
            }else{
                i = fwrite(&oFileData, sizeof(FileLruData<T>)*icnt , 1, fp);
                if(i != 1 ){

                    return 0xffffffff;

                }
                icnt = 0;
            }

            
        }
    }
    
    return iRet;
}


//template <class T>
//unsigned int SHMLruData<T>::writeFileAll( FILE * fp )
//{
//    FileLruHead oHead;
//    memset(&oHead, 0, sizeof(FileLruHead) );
//    
//    m_oDataLock.P();
//    
//    unsigned int iMaxCnt = (*puiMallocCnt);
//    unsigned long ulBufSize = sizeof( FileLruData<T> )*iMaxCnt;
//
// 
//    FileLruData<T> * poFileData = new FileLruData<T>[ iMaxCnt ];
//    
//        
//    if(!poFileData){
//        printf("�ڴ����벻������С��%lu �ֽڡ�\n", ulBufSize );
//        m_oDataLock.V();
//        usleep(20000);
//        return 0;
//    }
//    expandFile(fp);
//
//    fseek(fp, sizeof(FileLruHead) ,SEEK_SET);
//    int i = fread( poFileData, sizeof(FileLruData<T> )*iMaxCnt, 1, fp  );
//
//    if(i!=1){
//        m_oDataLock.V();
//        delete [] poFileData;
//        return 0;
//    }
//        
//
//
//    if(m_iLastTotal != (*puiIterCnt)) {
//        ////��������ڴ���չ�����д�ļ��Ͳ�д��
//        m_oDataLock.V();
//        delete [] poFileData;
//        
//        SHMAccess::reopen();
//        m_iLastTotal = (*puiIterCnt);
//
//        return 0;
//    }
//
//    unsigned int uitemp = 0;
//    unsigned int uiFile = 0;
//    unsigned int iDirty = 0;
//    unsigned int uiTheNext=0;
//    unsigned int uiThePre = 0;
//    unsigned int uideleted = (*puiDelete);
//
//    unsigned int iWriteCnt = 0;
//
////////////////����˽���ڴ���������
//    
//    for(int i=0; i< SHM_LRU_MAX_SESSION; i++){
//        
//        iDirty = *(puiDirty+i);
//        
//        while( iDirty ){
//            
//            uitemp = m_pPointer[ iDirty ].uiListIdxID;
//            uiTheNext = m_pPointer[ iDirty ].uiListNext;
//            
//            uiFile = uitemp-1;
//
//            uitemp = m_pPointer[ iDirty ].uiListLog;
//            if( uitemp ){
//                ////��δ�ύ
//                poFileData[uiFile].oData = m_pPointer[uitemp].oListData;
//                if( SHM_LRU_FLAG_DEL ==  m_pPointer[ uitemp ].sFlag[0] ){
//                    ////
//
//                }else{
//                    poFileData[uiFile].uiNext = 0;
//
//                }
//                
//            }else{
//                poFileData[uiFile].oData = m_pPointer[iDirty].oListData;
//                unsigned int iBig = m_pPointer[ iDirty ].uiListIdxID; 
//                
//                if( SHM_LRU_FLAG_DEL == m_pPointer[ iDirty ].sFlag[0] ){
//
//                    if( 0 == uideleted )
//                        poFileData[uiFile].uiNext = iBig;
//                    else
//                        poFileData[uiFile].uiNext = uideleted;
//                    
//                    uideleted = iBig;
//                    
//                }else{
//                    poFileData[uiFile].uiNext = 0;
//                }
//
//                
//            }
//
//            iWriteCnt++;
//            iDirty = uiTheNext;
//
//        }
//        
//    }
//
////////////////�������ļ�
//    
//    fseek(fp, sizeof(FileLruHead) ,SEEK_SET);
//    int iw = fwrite( poFileData, sizeof(FileLruData<T> )*iMaxCnt, 1, fp  );
//    if( iw !=1 ){
//        ///û����
//        m_oDataLock.V();
//        delete [] poFileData;
//        return 0;
//    }
//
////////////////�����ڴ�ʵ�ʲ���
//
//
//    unsigned int iWriteCntMem = 0;
//    for(int i=0; i< SHM_LRU_MAX_SESSION; i++){
//        unsigned int * pDirty = puiDirty+i;
//
//        while( *pDirty ){
//            
//            uitemp = m_pPointer[ *pDirty ].uiListIdxID;
//            uiTheNext = m_pPointer[ *pDirty ].uiListNext;
//            uiThePre  = m_pPointer[ *pDirty ].uiListPre;
//            
//            uitemp = m_pPointer[ *pDirty ].uiListLog;
//            if( uitemp ){
//                ////��δ�ύ
//                pDirty = &m_pPointer[ *pDirty ].uiListNext;
//                iWriteCntMem++;
//                continue;
//            }else{
//
//                if( SHM_LRU_FLAG_DEL == m_pPointer[ *pDirty ].sFlag[0] ){
//                    delFromIndex(m_pPointer[ *pDirty ].uiListIdxID, 1);///��д��uiListIdxNext��uiListIdxID��sFlag��(*puiDelete)
//
//                    addToFree(*pDirty);
//                }else{
//
//                    m_pPointer[ *pDirty ].sFlag[0] = SHM_LRU_FLAG_NORM;
//                    addToNormal(*pDirty);
//                }
//
//                (*puiDirtyCnt)--;
//                
//            }
//
//            iWriteCntMem++;
//
//            *pDirty = uiTheNext;
//            if(uiTheNext)  m_pPointer[uiTheNext].uiListPre = uiThePre;
//            
//
//        }
//
//    }  
//
//
////////////////ˢ���ļ�ͷ
//
//    writeFileHead(fp, oHead, 2);
//    uitemp = (*puiDelete);
//    
//    m_oDataLock.V();   
//    
//    if( iWriteCnt != iWriteCntMem
//        || uideleted != uitemp ){
//        ///�ĸ��ط���������
//        printf("somewhere had problem,the memory and the file is not consistent, so sorry.\n");
//        
//    }
// 
//    delete [] poFileData;
//    poFileData = 0;
//    return iWriteCnt;
//    
//}

template <class T>
unsigned int SHMLruData<T>::writeFileBatch( FILE * fpHead , bool bAll)
{

    
    FileLruHead oHead;
    memset(&oHead, 0, sizeof(FileLruHead) );
    
    while(!m_oDataLock.P());

    if(!bAll && getWriteFileFlag() ){
        m_oDataLock.V();
		return 0;
	}
	setWriteFileFlag(1);

	if( !bAll ){
	    m_oDataLock.V();
        while(!m_oDataLock.P());	
    }



    if(m_iLastTotal != (*puiIterCnt)) {
        ////��������ڴ���չ
        SHMAccess::reopen();
        m_iLastTotal = (*puiIterCnt);

    }
        
    unsigned int iMaxCnt = (*puiMallocCnt);
    unsigned long ulBufSize = sizeof( FileLruData<T> )*iMaxCnt;
    unsigned int iBatchCnt = m_iCntPerFile;///SHM_LRU_BATCH_SIZE/sizeof( FileLruData<T> );
    unsigned int iBatchTimes = (iMaxCnt-1)/iBatchCnt + 1;
    
    unsigned int *pBatchInfo = new unsigned int[iBatchTimes];
    if(!pBatchInfo){
        printf("�ڴ����벻������С��%lu �ֽڡ�\n", iBatchTimes*sizeof(unsigned int) );
        m_oDataLock.V();
        setWriteFileFlag(0);
        usleep(20000);
        return 0;
    }
    memset(pBatchInfo,0 , iBatchTimes*sizeof(unsigned int) );
    
    FileLruData<T> * poFileData = new FileLruData<T>[ iBatchCnt ];
    if(!poFileData){
        printf("�ڴ����벻������С��%lu �ֽڡ�\n", iBatchCnt*sizeof(FileLruData<T>) );
        m_oDataLock.V();
        setWriteFileFlag(0);
        delete [] pBatchInfo;
        usleep(20000);
        return 0;
    }

/////    expandFile(fp);    
    
    unsigned int uitemp = 0;
    unsigned int uiFile = 0;
    unsigned int iDirty = 0;
    unsigned int uiTheNext=0;
    unsigned int uiThePre =0;
    unsigned int uideleted = (*puiDelete);
    unsigned int iWriteCnt = 0;   ////д���ļ�������
    unsigned int iWriteCntMem = 0;
        
    unsigned int iWCnt = 0;  /////Ŀǰ���ε���ʼд�ĵط�
    unsigned int iTimes = 0;

    FILE *fp=0;
    for(iTimes=0; iTimes<iBatchTimes; iTimes++  ){ 

        if( iTimes && (0==pBatchInfo[iTimes])  ){
            iWCnt += iBatchCnt;
            continue; 
        }
        
        if( !bAll && iTimes && ((*puiState)==2 )
            && ( getDirtyPercent() < SHM_LRU_DIRTY_LOW_LEVEL ) 
          ){
            ////��ǿ��ģʽ�£��ﵽ�����ݰٷֱȵĵ�ֵ�����ٽ������
            break;
        }
        
        if( (iMaxCnt-iWCnt) < iBatchCnt  ){
            iBatchCnt = iMaxCnt-iWCnt;
        }

        fp=prepareFile(iTimes, false);
        if(!fp){
            m_oDataLock.V();
            setWriteFileFlag(0);
            delete [] poFileData;
            delete [] pBatchInfo;
            printf("�ļ�%d��ʧ��\n", iTimes);
            return iWriteCnt;
        }
        
        ////fseek(fp, sizeof(FileLruHead) + iWCnt*sizeof(FileLruData<T> ) ,SEEK_SET);
        fseek(fp, 0, SEEK_SET);
        int i = fread( poFileData, sizeof(FileLruData<T> )*iBatchCnt, 1, fp  );
        if(i!=1){
            m_oDataLock.V();
            setWriteFileFlag(0);
            fclose(fp);
            delete [] poFileData;
            delete [] pBatchInfo;
            printf("fread�ļ�%dʧ��\n", iTimes);
            return iWriteCnt;
        }

//////////////����˽���ڴ���������
    
    for(int i=0; i< SHM_LRU_MAX_SESSION; i++){
        
        iDirty = *(puiDirty+i);
        
        while( iDirty ){
            
            uitemp = m_pPointer[ iDirty ].uiListIdxID;
            uiTheNext = m_pPointer[ iDirty ].uiListNext;
//            uiThePre  = m_pPointer[ iDirty ].uiListPre;
            
            uiFile = uitemp-1;

            if(0==iTimes ){
                if( (uiFile/iBatchCnt)<iBatchTimes)
                    pBatchInfo[uiFile/iBatchCnt]++;
            }
            if( uiFile<iWCnt || uiFile>=(iWCnt+iBatchCnt ) ){
                ////���ڷ�Χ֮�ڵ��Թ�
                iDirty = uiTheNext;
                continue;
            }
            
            uiFile -=  iWCnt;
//            if(uitemp==2)
//                printf("2in file at %u\n", uiFile);
                
            uitemp = m_pPointer[ iDirty ].uiListLog;
            if( uitemp ){
                ////��δ�ύ
                poFileData[uiFile].oData = m_pPointer[uitemp].oListData;
                if( SHM_LRU_FLAG_DEL ==  m_pPointer[ uitemp ].sFlag[0] ){
                    ////
                    
                }else{
                    poFileData[uiFile].uiNext = 0;

                }
                
            }else{
                poFileData[uiFile].oData = m_pPointer[iDirty].oListData;
                unsigned int iBig = m_pPointer[ iDirty ].uiListIdxID; 
                
                if( SHM_LRU_FLAG_DEL == m_pPointer[ iDirty ].sFlag[0] ){
                    unsigned int * pNext=(unsigned int *)(&m_pPointer[ iDirty ].oListData);
                    poFileData[uiFile].uiNext = (*pNext);

                }else{
                    poFileData[uiFile].uiNext = 0;
                }

            ///ֻͳ���ύ��
            iWriteCnt++;
            //printf("mem:%u, big:%u\n",iDirty, uiFile+iWCnt+1);
                
            }


            iDirty = uiTheNext;

        }
        
    }

//////////////�������ļ�
    ////fseek(fp, sizeof(FileLruHead) + iWCnt*sizeof(FileLruData<T> ) ,SEEK_SET);
    fseek(fp, 0, SEEK_SET);
    int iw = fwrite( poFileData, sizeof(FileLruData<T> )*iBatchCnt, 1, fp  );
    if( iw !=1 ){
        ///û����
        m_oDataLock.V();
        setWriteFileFlag(0);
        fclose(fp);
        delete [] poFileData;
        delete [] pBatchInfo;
        printf("fwrite�ļ�%dʧ��\n", iTimes);
        return iWriteCnt;
    }
    fflush(fp);
    fclose(fp);
    fp=0;
//////////////�����ڴ�ʵ�ʲ���


    for(int i=0; i< SHM_LRU_MAX_SESSION; i++){
         unsigned int * pDirty = puiDirty+i;

        while( *pDirty ){
            
            uitemp = m_pPointer[ *pDirty ].uiListIdxID;
            uiTheNext = m_pPointer[ *pDirty ].uiListNext;
            uiThePre  = m_pPointer[ *pDirty ].uiListPre;

            uiFile = uitemp-1;
            if( uiFile<iWCnt || uiFile>=(iWCnt+iBatchCnt ) ){
                ////���ڷ�Χ֮�ڵ��Թ�
                pDirty = &m_pPointer[ *pDirty ].uiListNext;
                continue;
            }

            
            uitemp = m_pPointer[ *pDirty ].uiListLog;
            if( uitemp ){
                ////��δ�ύ
                pDirty = &m_pPointer[ *pDirty ].uiListNext;
                continue;
                
            }else{

                if( SHM_LRU_FLAG_DEL == m_pPointer[ *pDirty ].sFlag[0] ){
                    delFromIndex(m_pPointer[ *pDirty ].uiListIdxID, 0);
                    ///delFromIndex ԭʼ����1�����ڴ���0
                    ///����1ʱ��д��uiListIdxNext��uiListIdxID��sFlag��(*puiDelete)
                    ////����0ʱû�и�д(*puiDelete)
                    addToFree(*pDirty);
                }else{

                    m_pPointer[ *pDirty ].sFlag[0] = SHM_LRU_FLAG_NORM;
                    addToNormal(*pDirty);
                }

                (*puiDirtyCnt)--;
                
                iWriteCntMem++;                
               // printf("mem:%u, big:%u, -- %u\n",*pDirty, uiFile+1, (*(puiDirty+1)));
            }



            *pDirty = uiTheNext;
            if(uiTheNext)  m_pPointer[uiTheNext].uiListPre = uiThePre;

//            if(iWriteCntMem==iWriteCnt)
//                break;
        }

//        if(iWriteCntMem==iWriteCnt)
//            break;

    }  

    if(iWriteCnt!=iWriteCntMem){
        //
        printf("seq=%d,iWriteCnt=%u,iWriteCntMem=%u ,where is wrong?\n",
        iTimes, iWriteCnt,iWriteCntMem );
    }
//////////////ˢ���ļ�ͷ

    writeFileHead(fpHead, oHead, 1);


    uitemp = (*puiDelete);
    
        

        iWCnt += iBatchCnt;    
    }

    writeFileHead(fpHead, oHead, 2);

    m_oDataLock.V();
    setWriteFileFlag(0);
    
//    if( uideleted != uitemp ){
//        ///�ĸ��ط���������
//        printf("somewhere had problem,the memory and the file is not consistent, so sorry.\n");
//        
//    }
 
    delete [] poFileData;
    delete [] pBatchInfo;
    poFileData = 0;
    pBatchInfo = 0;
    return iWriteCnt;
    
}


template <class T>
FILE * SHMLruData<T>::prepareFile( int iTimes, bool bTrunc, bool bCreate)
{
    FILE *fp=0;
    if( !m_sFileName[0] ) 
        return 0;

    char sFileSeqName[SHM_LRU_FILENAME_LEN+8]={0};

    snprintf(sFileSeqName, sizeof(sFileSeqName)-1, 
      "%s.%04d", m_sFileName, iTimes );

    if(bTrunc){
        if(!bCreate){
            if( (fp = fopen( sFileSeqName,  "r" )) == NULL ){
                return 0;
            }
            fclose(fp);
            fp=0;
        }
        if( (fp = fopen( sFileSeqName,  "w" )) == NULL ){
            return 0;
        }
    }else{
        if( (fp = fopen( sFileSeqName,  "rb+" )) == NULL ){
            if(! bCreate )
                return 0;
            fp = fopen( sFileSeqName,  "ab+" );
            if(fp)
                fclose(fp);
            
            if( (fp = fopen( sFileSeqName,  "rb+" )) == NULL ){
                
                return 0;
            }            

        }
        
        if(SHMAccess::exist()){
            expandFile(fp);
        }
    }
    
    return fp;

}

template <class T>
void SHMLruData<T>::truncFile()
{   
    FILE *fp=0;

    initFile(true );

    for(int i=0; i<9999; i++){
        fp=prepareFile(i, true, false );
        if(!fp)
            break;
        fclose(fp);
        fp=0;
        
    }
}


template <class T>
unsigned int SHMLruData<T>::recoverFromFile()
{
    FILE * fp;
    
    if( (fp = fopen( m_sFileName,  "rb+" )) == NULL ){
        return 0;
    }

    fseek(fp, 0,  SEEK_END);
    long lSize = ftell(fp);

    
//    if( 0 != ( (lSize - sizeof(FileLruHead) )% sizeof(FileLruData<T>) )) {
//        fclose(fp);
//        return 0;
//    }
//    unsigned int uiTotal =  (lSize - sizeof(FileLruHead) ) / sizeof(FileLruData<T>);
//
//    if(0 == uiTotal){
//        fclose(fp);
//        return 0;
//    }
    
    fseek(fp, 0,  SEEK_SET);
    FileLruHead oHead;
    int i = fread(&oHead, sizeof(FileLruHead), 1, fp );

    if(i != 1 ){
        fclose(fp);
        return 0;
    }


    if( oHead.uiMallocCnt < oHead.uiRealCnt){
        fclose(fp);
        return 0;
    }
    
    if(1 == oHead.uiState){
        ////�ϴ�д�ļ�û�������˳�
        ////û�����ļ��ָ�,��Ϊ���ݲ��ܱ�֤��ȷ
        fclose(fp);
        printf("�ļ��ϴβ���δ�ɹ�������ȱ��һ���ԣ������лָ�\n");
        THROW(MBC_SHMLruData+8);
    }


    if( oHead.uiVer!=(*puiVer)
        || oHead.lKey != m_lSHMKey
        || oHead.uiVer!= SHM_LRU_CURR_VERSION
        || oHead.uiContentSize!=sizeof(T) ){
        fclose(fp);    
        THROW(MBC_SHMLruData+8);
    }
    

    
    if( (*puiExpandCnt) != oHead.uiExpandCnt 
        || (*puiHashValue)!= oHead.uiHashValue
        || (*puiIterCnt)  != oHead.uiIterCnt ){
        /////�ڴ�������չ
        int i;///=m_iExpandCnt;
        int iexpand;
        if( oHead.uiRealCnt>(*puiIterCnt) )
            iexpand =( ( oHead.uiRealCnt-(*puiIterCnt)) * sizeof(LruUnitData) -1) /m_lNextSize + 1;
        else
            iexpand = oHead.uiExpandCnt;
            
        for(i=0; i< iexpand; i++){
            expandMem( false );
        }
        if(  (*puiExpandCnt) != oHead.uiExpandCnt 
            || (*puiHashValue)!= oHead.uiHashValue
            || (*puiIterCnt)  != oHead.uiIterCnt ){
            ////fclose(fp);
            printf("�ڴ���չ�������ļ���¼��һ��, ��ʹ���µ��ڴ�������ļ���ʼ����...\n");
            ////THROW(MBC_SHMLruData+8);
        }
        
    }
    
    (*puiRealCnt)  = oHead.uiRealCnt;
    (*puiMallocCnt)= oHead.uiMallocCnt;
    (*puiDelete)  = oHead.uiDelete;
    
    unsigned int iCount = getTotal()/2;
    FileLruData<T> * pInitData = new FileLruData<T>[m_iCntPerFile] ;
    if(!pInitData){
        fclose(fp);    
        THROW(MBC_SHMLruData+12);
    }

    fclose(fp);
    fp=0;
    int iFileCnts = (oHead.uiMallocCnt-1)/m_iCntPerFile + 1;
    int iSeqCnt;
    const int LRU_INITLOAD_FILE_CNT=4;
    int iLoadSeq[LRU_INITLOAD_FILE_CNT];
    time_t Tmodify[LRU_INITLOAD_FILE_CNT];
    for(int iTmpp =0; iTmpp<LRU_INITLOAD_FILE_CNT; iTmpp++){
        iLoadSeq[iTmpp]=-1;
    }
    memset(Tmodify,0,sizeof(Tmodify));
    
    for(iSeqCnt=0; iSeqCnt<iFileCnts; iSeqCnt++ ){
        snprintf(m_sFileExName,sizeof(m_sFileExName)-1,
            "%s.%04d", m_sFileName, iSeqCnt);
        struct stat statbuf;
		if (stat (m_sFileExName, &statbuf) != 0){
		    THROW(MBC_SHMLruData+8);
		}
		if(statbuf.st_size != sizeof(FileLruData<T>)*m_iCntPerFile){
		    THROW(MBC_SHMLruData+8);
		}
		time_t TimeMin=statbuf.st_mtime;
		time_t TimeTmp;
		int iLoadSeqTemp;
		int iLoadSeqMin=iSeqCnt;
        for(int iTmpp =0; iTmpp<LRU_INITLOAD_FILE_CNT; iTmpp++){
		    if( TimeMin>Tmodify[iTmpp] ){
		        TimeTmp = Tmodify[iTmpp];
		        Tmodify[iTmpp]=TimeMin;
		        TimeMin = TimeTmp;
		        
		        iLoadSeqTemp=iLoadSeq[iTmpp];
		        iLoadSeq[iTmpp] = iLoadSeqMin;
		        iLoadSeqMin = iLoadSeqTemp;
		    }
		}
		 
    }    
    
    for(int iTmp =0; iTmp<LRU_INITLOAD_FILE_CNT; iTmp++){
        iSeqCnt=iLoadSeq[iTmp];
        if(iSeqCnt<0)
            continue;
            
        snprintf(m_sFileExName,sizeof(m_sFileExName)-1,
            "%s.%04d", m_sFileName, iSeqCnt);
        if( (fp = fopen( m_sFileExName,  "rb+" )) == NULL ){
            break;
        }
        
        i = fread( pInitData, sizeof(FileLruData<T>)*m_iCntPerFile, 1, fp );
        if( i!=1 ){
            delete [] pInitData;
            fclose(fp);    
            THROW(MBC_SHMLruData+8);
        }
        unsigned int iBigBase=iSeqCnt*m_iCntPerFile;
        for(iCount=0; iCount < m_iCntPerFile; iCount++){
            if( (iBigBase+iCount)>=oHead.uiMallocCnt)
                break;
            
            if(0 == (pInitData+iCount)->uiNext )
                getLruDataForRead(iBigBase+iCount+1, pInitData+iCount );
    
        }
    }
    
    delete [] pInitData;
    pInitData = 0;

    resetHitrateCalc();
    
    return iCount;

}


template <class T>
unsigned int SHMLruData<T>::expandMem( bool bTreatFile )
{
    while(!m_oDataLock.P());
    
    unsigned long lRet = SHMAccess::expandit();
    
    
    if( lRet ){
        /////�Ѿ��ɹ���չ�� lRet���ֽڵĿռ�
        ///�����Ŀռ�ӵ�Free��������ȥ��FreeCnt����,puiIterCnt����
        ///log���Ȳ����ӣ�
        ///

        unsigned int iExDataCnt = lRet/sizeof(LruUnitData);
        unsigned int iMax = (*puiIterCnt)+ 1 + (*puiLogTotalCnt);
        unsigned int iCnt = 0;
        unsigned int indexReal = 0;

        
        unsigned int iNewHash = (*puiHashValue)+iExDataCnt;
        unsigned int iOldHash = (*puiHashValue);
        
        iCnt=(*puiHashValue);
        for( ; iCnt<=(*puiMallocCnt); iCnt++){
            moveIndex(iCnt, iNewHash);
        }
        (*puiHashValue) = iNewHash;
        
        for(iCnt=0; iCnt<iExDataCnt; iCnt++){
            (*puiIterCnt)++;
            m_iLastTotal++;
            addToFree( iMax + iCnt);
        }

        ////if(bTreatFile) initFile(false, true);
        m_oDataLock.V();        
        return iExDataCnt;
    }
    m_oDataLock.V();
    return 0;
    
}

template <class T>
unsigned int SHMLruData<T>::moveIndex(unsigned int indexBig, unsigned int uiNewHash)
{
    unsigned int *pPre = &m_pPointer[(indexBig-1)%(*puiHashValue)+1].uiIndex;
    unsigned int indexReal = *pPre;
    
    while(indexReal){
        if( m_pPointer[indexReal].uiListIdxID  == indexBig ){
            if(pPre){
                (*pPre)= m_pPointer[indexReal].uiListIdxNext;
            }
            unsigned int indexNew = m_pPointer[(indexBig-1)%uiNewHash + 1].uiIndex;
            if(indexNew
              && m_pPointer[indexNew].uiListIdxID  == indexBig ){
                return indexNew;
            }
            if( indexNew == indexReal 
              && 0==m_pPointer[indexNew].uiListIdxID ){
                return indexNew;
            }
            m_pPointer[indexReal].uiListIdxNext = indexNew;
            m_pPointer[(indexBig-1)%(uiNewHash)+1].uiIndex = indexReal;
            return indexNew;
        }
        pPre = &m_pPointer[indexReal].uiListIdxNext;
        indexReal = m_pPointer[indexReal].uiListIdxNext;

    }

    return 0;

}


#endif
////SHMLRUDATA_H_HEADER_INCLUDED
 

 
