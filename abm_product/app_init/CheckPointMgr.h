#ifndef CHECKPOINT_MGR_H_HEADER_INCLUDED_BDD_1222
#define CHECKPOINT_MGR_H_HEADER_INCLUDED_BDD_1222

#include "Log.h"
#include <vector>
#include <string.h>
//#include "string.h"
#include "SysParamDefine.h"
#include "SHMLruData.h"
#include "CheckPointInfo.h"
#include "CmdMsg.h"

#define CHECKPOINT		"CHECKPOINT"
#define MAX_STD_INT_LEN  		4
//#define CHECKPOINTPATH		"/cbilling/app/DEV_TMP/src_jt/log/checkpoint"

class ThreeLogGroup;
class ThreeLogGroupMgr;
//class CommandCom;
//struct HighFeeData;

//add by hsir
class CheckPointMgr
{
    public:
            CheckPointMgr();
            ~CheckPointMgr();
			bool touchInfoDown();
            bool touchInfoDown(bool Show,bool RealTime=false);//checkpoint�����Լ��������α�ţ��������checkpoint�漰�����ڴ�
			char *getCharBatchID();
			
			bool touchInfoDown(int iAppID);//����APPID�����Ӧ�����ڴ�
            bool touchInfoDown(char *sBatchID);//��������ṩ�����α���������checkpoint�漰�����ڴ�
            
            // ���ض�Ӧ���α�ŵ��ļ����ڴ�
			// ����˵��:sBatch�����α�ʶ.
			bool touchInfoLoad(char *sBatch);
			
			// ���ض�Ӧ���α�ŵ��ļ����ڴ�
			// ����˵��:sBatch�����α�ʶ.
			bool touchInfoLoad(char *sBatch, int iAppID);
			
			// ��ֵ��sRootPath,��ȡ��Ŀ¼��Ϣ,��Ŀ¼�¸��ݳ�����.���Խ�����Ŀ¼.����ص��ļ���������Ŀ¼��
			bool getCheckPointPath(char *sRootPath);
			// ����������α�Ż�ȡ��Ŀ¼·��
			bool getDatePath(char *sBatch);
			int  CheckFilePath (char * filepath);
	public: //
			long getLongBatchID();
			long getBatchID(); 	
    public: 
            // �鿴���λ�����Ϣ
            void showBatchBaseInfo(char *sBatch);
			//�ж��ļ��Ƿ����
			bool FileExist(char *sFileName);   
			void getBatch();      
    private:
            //FileMgr *m_poFileMgr;
            ThreeLogGroupMgr *m_poLogGroup;
            char m_sBatchID[32]; //���κ�
			char m_sPath[1024];
            long m_lBatchID;
			//CommandCom *m_poCommandCom;
			
			char sHFKey[128];
			char sHFKeyName[128];
			char sHFFileName[1024];			      
			//SHMLruData<HighFeeData> *m_poSHMHFData;	 
			void formatInt(int iNum,char *sStdInt);
			char sStdInt[MAX_STD_INT_LEN+1];
			ThreeLogGroup *m_poLog;
			CheckPointInfo *m_poCheckPointInfo;     		     
};

#endif
