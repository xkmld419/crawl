#pragma once

#include "Environment.h"

#include <string>
#include <vector>
#include <map>

class DBInfo
{
public:
	char sDBStr[64];
	char sUser[32];
	char sPasswd[32];
	char sOldPasswd[32];
public:
	DBInfo(){memset(this,0,sizeof(DBInfo));};
	~DBInfo(){};

};

class FileContent
{
public:
	int iFileID;
	char sFileName[512];
	char sSection[64];
	char sDBStr[64];
	char sUser[32];
	char sPasswd[32];
public:
	FileContent(){memset(this,0,sizeof(FileContent));};
	~FileContent(){};
};



typedef std::vector<DBInfo*> VEC_DBINFO;
typedef std::vector<FileContent*> VEC_FILECONTENT;
typedef std::map<std::string,VEC_FILECONTENT*> MAP_FILECONTENT;

class DBManager
{
public:
	DBManager(void);
	~DBManager(void);
public:
	//�������
	char* encode(char* sSrcStr,char * sDecStr);
	//�������
	char* decode(char* sSrcStr,char * sDecStr);

	//��ȡ���ݿ���������Ϣ
	int GetPassInfo();
	//��ȡ�ļ�����
	int GetFileContent();

	//�޸����ݿ�����
	int ChangeDBPass(char* sDBStr,char* sUser,char* sOldPass,char* sNewPass);
	//�޸��ļ�
	int UpdateCfgFile();
	
	int doProcess();

	int CreateCfgFile();


private:
	std::string m_temp_path;
	void ClearVecDBInfo();
	void ClearFileContent();

	DBInfo *m_pDBInfo;
	VEC_DBINFO m_vecDBInfo;
	
	FileContent *m_pFileContent;
	MAP_FILECONTENT m_mapFileContent;
};
