/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef  _LOCAL_MEM_BASE_
#define  _LOCAL_MEM_BASE_

#include "SHMData.h"
#include "SHMData_A.h"
#include "SHMData_B.h"
#include "SHMIntHashIndex.h"
#include "SHMIntHashIndex_A.h"
#include "SHMStringTreeIndex.h"
#include "SHMStringTreeIndex_A.h"

#include "IpcKey.h"
#include "ParamDefineMgr.h"
#include "CSemaphore.h"
#include "UserInfo.h"

using namespace std;

//�û������������峣����ȡ��b_param_define
//1����Ӧ��param_segment�ֶ�
#define USERINFO_SEGMENT		"USERINFO_UPLOAD"
#define USERINFO_LIMIT			"USERINFO_MEM_LIMIT"
#define E8_DIV_GROUP_SEGMENT	"E8_DIV_PROC_GROUP"
#define E8_USER              "DATA_USER"
#define EXT_SERV              "B_EXT_SERV"


//2�����๲���ڴ��Ӧ��groupid
#define E8_USER_MEM_GRP			0
#define E8_DIV_MEM_GRP_HEAD		1
#define EXT_SERV_MEM_GRP		101

//���e8�ʺ�
#define DATA_E8_ACCOUNT_DATA					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_E8_ACCOUNT_DATA")
#define DATA_E8_ACCOUNT_INDEX					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_E8_ACCOUNT_INDEX")


//����������
//��һ��
#define DATA_EVENT_DIV_DATA11					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_DATA11")
#define DATA_EVENT_DIV_INDEX11					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_INDEX11")
#define DATA_EVENT_DIV_DATA12					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_DATA12")
#define DATA_EVENT_DIV_INDEX12					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_INDEX12")

//�ڶ���
#define DATA_EVENT_DIV_DATA21					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_DATA21")
#define DATA_EVENT_DIV_INDEX21					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_INDEX21")
#define DATA_EVENT_DIV_DATA22					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_DATA22")
#define DATA_EVENT_DIV_INDEX22					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_INDEX22")

//������
#define DATA_EVENT_DIV_DATA31					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_DATA31")
#define DATA_EVENT_DIV_INDEX31					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_INDEX31")
#define DATA_EVENT_DIV_DATA32					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_DATA32")
#define DATA_EVENT_DIV_INDEX32					IpcKeyMgr::getIpcKey (-1,"SHM_DATA_EVENT_DIV_INDEX32")

//### 4008��800��Ext_serv
#define EXT_SERV_DATA							IpcKeyMgr::getIpcKey(-1,"EXT_SERV_DATA")
#define EXT_SERV_INDEX							IpcKeyMgr::getIpcKey(-1,"EXT_SERV_INDEX")


//ADD 20080508 YUANP E8�ʺ�
class DataE8AccountInfo
{
public:
	//e8�ʺű�ʶ
	long lUserID;
	//e8�ʺű������
    int  iUserSeq;              
	//e8�ʺ�
    char sUserName[41];
	//�ʺŹ���������
    char sAreaCode[10];
	//��¼�޸�ʱ��,20080716Ϊʵ�ְ��쵼���ʺ���������ȶ��޸ģ�������ʱ���Ϊ�޸�ʱ��
    char sStateDate[15];
	//��Чʱ��
    char sEffDate[15];
	//ʧЧʱ��
    char sExpDate[15];
    //״̬
    char sState[4];    
    //�ֱ�ָ����ͬ�ʺŵ���һ��㣬��ʱ������
    unsigned int m_iNextOffset; 
};

//add yuanp 20080513 ���������ֺϲ�
class DataEventDiv
{
public:
	long lUserID;
	char sStartDate[15];
	char sEndDate[15];
	long lSepRecID;/*��ּ�¼��ʶ,����ǰ��һ��������*/
	long lResourceID;/*�����Դ����¼��ԭʼ��������ϵ*/
	int  iSepRecSeq;
	int  iDuration;
	int  iState;		
	unsigned int m_iNextOffset;
};

//add yuanp 20080721 4008/800����Ϣ
class ExtServ
{
public:
	long lExtServID;
	char sAreaCode[7];
	char sExtNbr[MAX_CALLED_NBR_LEN];
	char sExtType[4];
	char sState[4];
	char sEffDate[15];
	char sExpDate[15];	
    unsigned int m_iNextOffset; 
};

class DataEventMemBase 
{
public:
	DataEventMemBase();
	DataEventMemBase(int iGroup);
	~DataEventMemBase();
	void insMonitorInterface(char *sInfo,char *sRcvAlarmAccNbr);
//protected:
	void bindData();
	void getGroupIndex();
	
	static bool m_bAttached;
	int m_iGroupID;
	
    //���e8�������캽���ݼ�����
	static	USERINFO_SHM_DATA_TYPE<DataE8AccountInfo> * m_poE8AccountData;
	static  DataE8AccountInfo * m_poE8AccountIns;
	static	USERINFO_SHM_STR_INDEX_TYPE * m_poE8AccountIndex;	

	//���������ֺϲ�
	DataEventDiv *m_poIns1;   //��ǰ���һƬ�ڴ������ָ��
	USERINFO_SHM_INT_INDEX_TYPE *m_poIndex1; //��ǰ���һƬ�ڴ�������ڴ�ָ��
	USERINFO_SHM_DATA_TYPE<DataEventDiv> *m_poData1; //��ǰ���һƬ�ڴ�������ڴ�ָ��
	
	DataEventDiv *m_poIns2;   //��ǰ��ڶ�Ƭ�ڴ������ָ��
	USERINFO_SHM_INT_INDEX_TYPE *m_poIndex2; //��ǰ��ڶ�Ƭ�ڴ�������ڴ�ָ��
	USERINFO_SHM_DATA_TYPE<DataEventDiv> *m_poData2; //��ǰ��ڶ�Ƭ�ڴ�������ڴ�ָ��
	
    
	//��һ��
	static USERINFO_SHM_DATA_TYPE<DataEventDiv> * m_poDataEventDivData11;
	static DataEventDiv * m_poDataEventDivIns11;
	static USERINFO_SHM_INT_INDEX_TYPE * m_poDataEventDivIndex11;
	static USERINFO_SHM_DATA_TYPE<DataEventDiv> * m_poDataEventDivData12;
	static DataEventDiv * m_poDataEventDivIns12;
	static USERINFO_SHM_INT_INDEX_TYPE * m_poDataEventDivIndex12;

	//�ڶ���
	static USERINFO_SHM_DATA_TYPE<DataEventDiv> * m_poDataEventDivData21;
	static DataEventDiv * m_poDataEventDivIns21;
	static USERINFO_SHM_INT_INDEX_TYPE * m_poDataEventDivIndex21;
	static USERINFO_SHM_DATA_TYPE<DataEventDiv> * m_poDataEventDivData22;
	static DataEventDiv * m_poDataEventDivIns22;
	static USERINFO_SHM_INT_INDEX_TYPE * m_poDataEventDivIndex22;

	//������
	static USERINFO_SHM_DATA_TYPE<DataEventDiv> * m_poDataEventDivData31;
	static DataEventDiv * m_poDataEventDivIns31;
	static USERINFO_SHM_INT_INDEX_TYPE * m_poDataEventDivIndex31;
	static USERINFO_SHM_DATA_TYPE<DataEventDiv> * m_poDataEventDivData32;
	static DataEventDiv * m_poDataEventDivIns32;
	static USERINFO_SHM_INT_INDEX_TYPE * m_poDataEventDivIndex32;

	//4008��800��ext_serv��Ϣ
	static USERINFO_SHM_DATA_TYPE<ExtServ> * m_poExtServData;
	static ExtServ * m_poExtServIns;
	static USERINFO_SHM_STR_INDEX_TYPE * m_poExtServIndex;

	void freeAll();
};

class DataEventMemCtl:public DataEventMemBase
{
public:
	DataEventMemCtl();	
	DataEventMemCtl(int iGroup);	
	~DataEventMemCtl();	
	
	//DataEventMemCtl();
	bool exist();	
};

#endif
