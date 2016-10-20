#include "Environment.h"
#include <string>
#include <map>
#include <vector>

#include <sstream>
#include <fstream>
#include "Date.h"

using namespace std;

class FileInfo
{
public:
	FileInfo(void):m_iCount(0){m_dStateDate.getCurDate();};
	~FileInfo(void);
public:
	string m_sFileName;
	ofstream m_File;
	int m_iCount;
	ostringstream m_osContent;
	//ostringstream m_osFileID;
	vector<long> m_vecFileID;
	Date m_dStateDate;
};

typedef map<string,FileInfo *> FILE_MAP;

class PrepLog
{
public:
	PrepLog(void);
	~PrepLog(void);

public:
	void run();

	//�����ļ�
	FileInfo *getFileInfo(string sAreaCode,int iFlag = 0);//iFlag:0 ���������ļ���1 �������ļ�

	//д�ļ�
	int writeFile(string sTmpPath,string sPath,string sAreaCode );
	//���±�״̬
	int updateState(string sAreaCode );

	void clearFileMap();
	//�ж�ʱ��ǿ���ύ
	int forceCommit(string sTmpPath,string sPath,int iSecond);
	string GetUpdateString(const char* sFileId,char* sState);
private:
	FILE_MAP m_FileMap;
};
