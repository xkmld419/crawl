/***********************************************************************
 * Module:  QueueInDb.h
 * Author:  Yuan Peng
 * Modified: 2009��4��20�� 10:22:41
 * Version: 1.0
***********************************************************************/
#ifndef _QUEUE_IN_DB_H_
#define _QUEUE_IN_DB_H_

#include <vector>
#include <map>
#include "StdEvent.h"
#include "Environment.h"

using namespace std;

/***********************************************************************
 * Modified: 2009��4��20�� 10:31:59
 * Purpose: Declaration of the class EventFieldDef
 * Comment: ����event_attr_id��Ӧ��Ĭ���ֶ�
 ***********************************************************************/
 /*
 typedef struct Event_Attr_Field
{
	int m_iEventAttrID;
   	char m_sFieldName[31];
   	char m_sDataType[4];   	
} EventAttrField;

 
class EventAttrFieldMgr
{
public:
	EventAttrFieldMgr()
	{
		memset(m_sInsSql,0,4001);
		memset(m_sInsValue,0,4001);
	}
	~EventAttrFieldMgr()
	{
		delete[] m_vEventFieldIndex;
		delete[] m_vEventFieldList;
		m_vEventFieldIndex = NULL;
		m_vEventFieldList = NULL;
	};   	
	
	static bool load();
	static bool getEventAttr(int iAttrID,char *sFieldName,char *sDataType);		
protected:
private:
   	static HashList<EventAttrField *> *m_vEventFieldIndex;
	static EventAttrField *m_vEventFieldList;
	static bool m_bLoadEvent;
	static int m_iAttrCnt;

	char m_sInsSql[4001];
	char m_sInsValue[4001];
};
*/

/***********************************************************************
 * Modified: 2009��4��20�� 10:31:59
 * Purpose: Declaration of the class FileInDBField
 * Comment: Ϊÿ���ļ����Ͷ����ض�������ֶΣ�δ������ֶβ���event_attr_idĬ�϶�Ӧ���ֶ�
 ***********************************************************************/

class FileInDBField
{
public:
	FileInDBField(int iErrFileType,int iAttrID,char *sFieldName,char *sDatatype);
	~FileInDBField(){};   	

	int m_iErrFileTypeID;
   	int m_iEventAttrID;
   	char m_sFieldName[31];
   	char m_sDataType[4];       //D0A:�ַ�����D0B:��ֵ��D0C:����
   	FileInDBField *m_oNext;
protected:
private:
  

};

/***********************************************************************
 * Modified: 2009��4��20�� 10:31:59
 * Purpose: Declaration of the class FileTypeDef
 * Comment: ����filerule�Ҳ����ļ����͵����ļ�����Ϊ��1����Ӧ����ֶβ���Ĭ�ϵ�event_attr_id��Ӧ�Ķ����ֶ�
 ***********************************************************************/
class FileTypeDef
{
	friend class FileDef;
	friend class QueueInDbMgr;
public:
	FileTypeDef(int iErrFileType,char *sFileRule,int iFileType,char *sTabName,int iGrpID,char *sIndbErrPath,char *sIndBErrFile)
	{
		m_iErrFileTypeID = iErrFileType;
		memset(m_sFileRule,0,33);
		memset(m_sINDbTable,0,31);
		memset(m_sIndbErrPath,0,33);
		memset(m_sIndbErrFileName,0,33);
		
		strncpy(m_sFileRule,sFileRule,32);
		m_iFileTypeID = iFileType;
		strncpy(m_sINDbTable,sTabName,30);
		m_iGroupID = iGrpID;
		strncpy(m_sIndbErrPath,sIndbErrPath,32);		
		strncpy(m_sIndbErrFileName,sIndBErrFile,32);		
		m_oInDBField = NULL;
		memset(m_sInsSql,0,2001);
		memset(m_sInsValue,0,2001);
		m_iAttrNum = 0;
		m_vEventList.clear();
	}
	~FileTypeDef(){};
   	bool load();	
   	bool unload();   	
	bool checkName(char *sLongName);
	//void saveToDB(StdEvent &poEventSection);
	//void commitDB();
	
protected:
private:
   	int m_iErrFileTypeID;
   	char m_sFileRule[33];
   	int m_iFileTypeID;
   	char m_sINDbTable[31];
	int m_iGroupID;
	char m_sIndbErrPath[33];
	char m_sIndbErrFileName[33];
   	char m_sInsSql[4001];
	char m_sInsValue[4001];
	int m_iAttrNum;  //ͳ��ÿ���ļ����������Եĸ���������150��ƴsql
	vector<StdEvent> m_vEventList;
			
   	FileInDBField *m_oInDBField;
	bool maskStr ( char*   i_sName,char*   i_sMask );
	int formatInsSql();
	//int formatSqlValue(StdEvent & poEventSection);
   	int createTable();
   	int execSql();	
};

/*********************************************************************** 
 * Modified: 2009��7��8�� 9:58:30
 * Purpose: Declaration of the class FileDef
 * Comment: �ļ�������Ҫÿ���ļ�����ʱ�ύ
 ***********************************************************************/

class FileDef
{
	friend class QueueInDbMgr;
	public:
		FileDef(int iFileID,FileTypeDef *pFileType)
		{
			m_iFileID = iFileID;
			m_oFileType = pFileType;
			m_iSaveEventCnt = 0;
			m_vEventList.clear();
			m_vInstSqlList.clear();
			m_oNext = NULL;			
		}
		~FileDef()
		{
			m_vEventList.clear();
		}
   		int saveEvent(StdEvent &poEventSection);
   		int commitEvent();
		
	protected:
	private:   		
		vector<StdEvent> m_vEventList;
		vector<string>m_vInstSqlList;
		int m_iFileID;
		FileTypeDef *m_oFileType;
		FileDef *m_oNext;
		int m_iSaveEventCnt;

		int commitSingleEvent();
		void getInstSql(StdEvent & poEventSection,string &sSql);
		int commitEvent_2();
};


/***********************************************************************
 * Modified: 2009��4��20�� 10:27:58
 * Purpose: Declaration of the class QueueInDbMgr
 * Comment: �������
 ***********************************************************************/

class QueueInDbMgr
{
public:
	QueueInDbMgr()
	{			
			m_oFileTypeList.clear();
			m_oFileHead = NULL;			
			//m_vEventList.clear();
	}
	~QueueInDbMgr(){unload();}
   	int saveEvent(StdEvent &poEventSection,int iFileID);
	int commitEvent(int iFileID);

protected:
private:
   	int getAbstractFileName(char *m_sFileName)
   	{
   		if (strlen(m_sFileName) == 0) 
   		{
   			memset(m_sAbstractFileName,65,0);
			return 0;			
   		}

		char *p;
		p=strrchr(m_sFileName,'/');
		if (p==NULL)
			strcpy(m_sAbstractFileName,m_sFileName);
		else
			strcpy(m_sAbstractFileName,p+1);

		return 1;
   	}
   	
   	char m_sAbstractFileName[65];
   	//FileTypeDef m_oFileType;
	
	map<int,FileTypeDef *> m_oFileTypeList;

	static bool m_bLoad;
	static FileDef *m_oFileHead;

	void load();
	void unload();
	
	FileTypeDef * getFileTypeByName(char *m_sFileName);
	FileTypeDef * getFileTypeByID(int iFileTypeID);
	FileDef *searchFileDefList(int iFileID);
	void addFileDefList(FileDef *poFileDef);
	

};

#endif
