#ifndef _LIB_THREE_LOG_BASE_H_
#define _LIB_THREE_LOG_BASE_H_

#include "SHMData.h"
#include "ReadIni.h"
#include "SHMData_A.h"
#include "CSemaphore.h"
#include "SHMIntHashIndex.h"
#include "SHMIntHashIndex_A.h"
#include "MBC.h"

#include "Env.h"

using namespace std;
#define  GROUP_NAME_LENGTH        64
#define  LOG_DATA_SIZE           320                     //��־���ݴ�С

#define  GROUP_LOG_NUMS     6                       //��־�����
#define  GROUP_FILE_CNT     6                       //��־���Ա�ļ�����

/*
#define SHM_LOG_GROUP_DATA          (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_DATA"))
#define SHM_LOG_GROUP_INDEX         (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INDEX"))
#define SHM_LOG_GROUP_LOCK          (IpcKeyMgr::getIpcKey(-1, "LOCK_LOGGROUP_DATA"))
#define SHM_LOG_GROUP_INDEX_LOCK    (IpcKeyMgr::getIpcKey(-1, "LOCK_LOGGROUP_INDEX_DATA"))

#define SHM_LOG_GROUP_INFO_DATA_1       (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_DATA_1"))
#define SHM_LOG_GROUP_INFO_DATA_2       (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_DATA_2"))
#define SHM_LOG_GROUP_INFO_DATA_3       (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_DATA_3"))
#define SHM_LOG_GROUP_INFO_DATA_4       (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_DATA_4"))
#define SHM_LOG_GROUP_INFO_DATA_5       (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_DATA_5"))
#define SHM_LOG_GROUP_INFO_DATA_6       (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_DATA_6"))

#define SHM_LOG_GROUP_INFO_INDEX_1      (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_INDEX_1"))
#define SHM_LOG_GROUP_INFO_INDEX_2      (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_INDEX_2"))
#define SHM_LOG_GROUP_INFO_INDEX_3      (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_INDEX_3"))
#define SHM_LOG_GROUP_INFO_INDEX_4      (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_INDEX_4"))
#define SHM_LOG_GROUP_INFO_INDEX_5      (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_INDEX_5"))
#define SHM_LOG_GROUP_INFO_INDEX_6      (IpcKeyMgr::getIpcKey(-1, "SHM_LOGGROUP_INFO_INDEX_6"))

#define SHM_LOG_GROUP_INFO_LOCK		(IpcKeyMgr::getIpcKey(-1, "LOCK_LOGGROUP_INFO_DATA"))
*/
#define SHM_LOG_GROUP_COUNT 6


class LogFileData
{//�ļ�������
 public:
    LogFileData():m_lFileSize(0)
    {
        memset(m_sFileName,0,sizeof(m_sFileName));
        memset(m_sGroupPath,0,sizeof(m_sGroupPath));
    }
 public:
    long m_lFileSize;               //�ļ���С:byte
    char m_sFileName[100];          //�ļ���
    char m_sGroupPath[500];         //�ļ�·��
};

class LogInfoData
{//�ڴ���Ϣ������
 public:
    LogInfoData():m_illCode(-1),m_illProId(-1),m_illAppId(-1),m_illLevelId(-1),m_illClassId(-1),
                    m_illTypeId(-1),m_iNextOffset(0),m_iThreadId(-1),m_iNetId(-1)
    {
        memset(m_sAreaCode,0,sizeof(m_sAreaCode));
        memset(m_sLogTime,0,sizeof(m_sLogTime));
        memset(m_sCodeName,0,sizeof(m_sCodeName));
        memset(m_sInfo,0,sizeof(m_sInfo));
        memset(m_sActionName,0,sizeof(m_sActionName));
		memset(m_sFlow,0,sizeof(m_sFlow));
    }
 public:
    int m_illCode;                  //��־����
    char m_sLogTime[17+1];          //��־ʱ��
    int m_illProId;                 //���̺�
    int m_illEmployee;              //����Ա���� 
    int m_illLevelId;               //��־�ȼ���ʶ
    int m_illClassId;               //��־����ʶ
    int m_illTypeId ;               //��־�����ʶ
	int m_illAppId;                 //ģ���    
    int m_illActionId;              //�����޸����ͻ�SQL��������
    char m_sActionName[GROUP_NAME_LENGTH];//�����������
    char m_sAreaCode[4+1];          //��������ʶ
    char m_sCodeName[GROUP_NAME_LENGTH];  //��������
    int m_iThreadId;               //�̺߳�
    int m_iNetId;					//�Զ���Ԫ��ʶ
    char m_sFlow[8];				//����eg. send  recv
    
    char m_sInfo[LOG_DATA_SIZE];         //��־����

    unsigned int m_iNextOffset;     //�α�
};

class ThreeLogGoupData
{
 public:
    ThreeLogGoupData():m_iGoupId(0),m_iSeq(0),m_iState(0)
        ,m_bPigeonhole(false),m_bCheckPoint(false),m_bPigeonholeState(0),
        m_lCurFileSize(0),m_lCurDirSize(0),m_iNext(0),m_iWritSeq(0),m_iMemSeq(0)
        ,m_lPredictFileSize(0),m_iFileCheckPercent(0),m_lFileSizeThreshold(0)
    {
    }
 public:
	int m_iGoupId;                      //��־���ʶ
	int m_iLevel ;                      //��־�ȼ�
	int m_iSeq;                         //�ļ���
	int m_iState;                       //��ǰ״̬
	int m_bPigeonholeState;             //�鵵״̬
	int m_iMemSeq;                      //��¼�ڴ�������
	int m_iWritSeq;                     //��¼�ļ�������
	bool m_bPigeonhole;                 //�Ƿ�鵵
	bool m_bCheckPoint;                 //�Ƿ�checkpoint
	long m_lCurFileSize;                //��ǰ�ļ���С:byte

	long m_lPredictFileSize;			//��ǰ��־��ʵ�ʿ���ش�С����ҪԤ��,��Ϊ����ӳ٣����ܵ���m_lCurFileSize��׼ȷ

	int m_iFileCheckPercent;           //���ļ���С�ﵽ�˰ٷֱ�ʱ���л���־��
	long m_lFileSizeThreshold;         //�ļ���С����ֵ
		
	long m_lCurDirSize;                 //��ǰ��־���С:byte
	LogFileData m_File[GROUP_FILE_CNT]; //��Ա�ļ�
	unsigned int m_iNext;               //��һ����־��
};

class ThreeLogGroupBase
{
public:
    ThreeLogGroupBase();
    ~ThreeLogGroupBase();

	long getLogGroupDataKey(int iGroupId);
	long getLogGroupIndexKey(int iGroupId);
	
	void initBase();

protected:
    static void bindData();
    static void bindGroupData(int GroupId);
    static void freeAll();
    static bool m_bAttached;

    static ThreeLogGoupData *m_poLogGroup;
    static SHMIntHashIndex_A *m_poLogGroupIndex;
    static SHMData<ThreeLogGoupData> *m_poLogGroupData;

    static SHMData_A<LogInfoData> *m_poInfoDataAll[GROUP_LOG_NUMS+1];
    static SHMIntHashIndex_A *m_poInfoIndexAll[GROUP_LOG_NUMS+1];

    static CSemaphore* m_poInfoDataLock;                       //ȫ����������
    static CSemaphore* m_poDataLock;                           //��־����Ϣ��
    static CSemaphore* m_poIndexLock;                          //��������
    static CSemaphore* m_poLogGroupMgrLock;					  //��ع鵵����

    static LogInfoData *m_poInfoGroup;                          //������
    static SHMData_A<LogInfoData> *m_poInfoData;                //ȫ��������
    static SHMIntHashIndex_A *m_poInfoIndex;                    //ȫ��������

    static bool getIpcCfg() ;
private :
	 static long SHM_LOG_GROUP_DATA   ;
   static long SHM_LOG_GROUP_INDEX        ;
   static long SHM_LOG_GROUP_LOCK         ;
   static long SHM_LOG_GROUP_INDEX_LOCK   ;
   static long SHM_LOG_GROUP_INFO_LOCK ;
   static long SHM_LOG_GROUP_MGR_LOCK ;
   static long SHM_LOG_GROUP_INFO_DATA_1  ;
   static long SHM_LOG_GROUP_INFO_DATA_2  ;
   static long SHM_LOG_GROUP_INFO_DATA_3  ;
   static long SHM_LOG_GROUP_INFO_DATA_4  ;
   static long SHM_LOG_GROUP_INFO_DATA_5  ;
   static long SHM_LOG_GROUP_INFO_DATA_6  ;
   static long SHM_LOG_GROUP_INFO_INDEX_1 ;
   static long SHM_LOG_GROUP_INFO_INDEX_2 ;
   static long SHM_LOG_GROUP_INFO_INDEX_3 ;
   static long SHM_LOG_GROUP_INFO_INDEX_4 ;
   static long SHM_LOG_GROUP_INFO_INDEX_5 ;
   static long SHM_LOG_GROUP_INFO_INDEX_6 ;     
   static bool SHM_KEY_IFLOAD;
};

#endif //_LIB_THREE_LOG_BASE_H_
