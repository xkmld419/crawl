#ifndef CHECKPOINT_INFO_H_HEADER_INCLUDED_BDD_1222
#define CHECKPOINT_INFO_H_HEADER_INCLUDED_BDD_1222

#include "Log.h"
#include "CSemaphore.h"

#define _CHECKPOINT		"CHECKPOINT"
#include "CommandCom.h"
#include "SysParamDefine.h"
class CheckPointInfo
{
	  public:
	  		  CheckPointInfo();
			  ~CheckPointInfo();
	  public: //HSS����
	  	      char *getChkPointBatchPath(char *batch);
			  char *getNowBatch();
			  bool createBatchID();
	  public: //2.8ʹ�ú���
			  // ��ֵ��sRootPath,��ȡ��Ŀ¼��Ϣ,��Ŀ¼�¸��ݳ�����.���Խ�����Ŀ¼.����ص��ļ���������Ŀ¼��
			  bool getCheckPointPath(char *sRootPath);
			  //��ȡ���κ�
			  char *getCharBatchID();
			  //���·���Ƿ����,�����ھʹ���
			  int  CheckFilePath (char * filepath);	
			  void getBatch(); 
			  // �鿴���λ�����Ϣ
              void showBatchBaseInfo(char *sBatch);  	  
				
			  //#ifdef DEF_SCHEDULE
			  //bool P(bool Real=false);//ֻ���Ƿ��ͳ�ȥ�Ľ��Ϊ׼
			  //bool V();//ֻ���Ƿ��ͳ�ȥ�Ľ��Ϊ׼
			  //#else
			  CSemaphore *lock;
			  int m_iLockKey;
			  bool P(bool Real=false);//ֻ���Ƿ��ͳ�ȥ�Ľ��Ϊ׼
			  bool V();//ֻ���Ƿ��ͳ�ȥ�Ľ��Ϊ׼
			  //#endif
				
			  int waitTimer(int &iTimer,int iMaxTimeNum); 
			  //����-ʱ��-������ݼ��(FileMgr)-desc
			  bool recordBatchInfo(char * sBatch, char* sTime, char *sDataType,char*desc=0,char *tablename=0); 
			  bool getNeedState();
	  private:
	          char m_sBatchID[32];
			  char m_sPath[2048];
              long m_lBatchID;
			  CommandCom *m_poCommandCom;
			  //char m_sBathPath[1024];
			  //string strCheckPath; 
};
#endif
