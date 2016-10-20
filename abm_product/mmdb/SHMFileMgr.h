#ifndef SHMFILEMGR_H_HEADER_INCLUDED
#define SHMFILEMGR_H_HEADER_INCLUDED

#include <stdio.h>
#include "SHMInfoCtl.h"
#include "ThreeLogGroup.h"
//#include "CommandCom.h"

#define SHM_TABLE_NAME_LEN 64
//ÿ���������ֶ���
#define SHM_FIELD_NUM 128
#define SHM_FIELD_LEN 64
#define SHM_FILE_NAME_LEN 256
//ÿ�����ݵ���󳤶�
#define SHM_DATA_LEN 2048
#define SHM_SQL_LEN 4096

const char  SHM_FILEMGR_TRANS_NULL     ='0';
const char  SHM_FILEMGR_TRANS_CHAR     ='1';
const char  SHM_FILEMGR_TRANS_INT      ='2';
const char  SHM_FILEMGR_TRANS_LONG     ='3';
const char  SHM_FILEMGR_TRANS_FLOAT    ='4';
const char  SHM_FILEMGR_TRANS_DOUBLE   ='5';
const char  SHM_FILEMGR_TRANS_STRING   ='6';
const char  SHM_FILEMGR_TRANS_EVTDATE  ='7';


class SHMFileMgr{
public:
    SHMFileMgr(int iMode);
    ~SHMFileMgr();
    int m_iTotalCnt;                            //�ļ��ڼ�¼����
    char m_sFilePath[500];                      //��/д�ļ�·��������·����
    char m_sFileName[200];                      //��/д�ļ���
    char m_sDivChar;                            //�ļ��ֶηָ�������"|",","
    int m_iTableType ;                          //��ǰ��/д�ļ��ı�����
    int m_iMode;                                //1���� 2����

    //��������
    int m_iFileFieldCnt;
    char m_sFileTransType[FIELD_NUM];                       //ÿ���ֶε�����
    int m_iFileFieldOffSet[FIELD_NUM];                      //ÿ���ֶ������ݽṹ���ж�Ӧ��ƫ�� 
    int m_iFileFieldLen[FIELD_NUM];                         //ÿ���ֶε���󳤶�
    char m_sFileFieldInfo[4096];                            //�ļ��ֶ���Ϣ���ֶ�1���ֶ�2
    unsigned int m_iDataStructSize ;
    char *m_sData;                                          //�ļ�������
    FILE *fp;                                               //��ǰ�����ļ�
    ThreeLogGroup *m_poLogGroup;
	  //CommandCom *m_pCommandComm;
		
public:
    void resetInfo();
    bool setColumnInfo(const char *sColumn, const int iTableType);
    void writeFieldBuf(const char* sColumn, const char cTransType,
                  unsigned int iFieldOffset, unsigned int iFieldMaxLen);
		
    void init(int iMode);
    bool openPath(const char* sPathName);
    bool openFile(const char *sFullFileName, char *sMode);          // ���ļ�
    bool closeFile();
    bool getLine(char *sLine, const size_t iSize, const char cFlag);

    bool setColumnInfo(const int iTableType);
    bool checkColumn(char *sColumn);
    bool fileToMem(const char * sFileName, const int iTableType,bool bShow,bool bReplace=false);
    bool flushToMem(const char *sData, bool bShow, int &iUpdCnt,bool bReplace=false);
    bool parseString(const char *sData);
    bool writePair(void **sDes, const char *sSrc, const char cTransType, 
                   const int iOffSet, const int iFieldLen);

    bool memToFile(const char * sFileName, const int iTableType, bool bShow);    // д�ַ��������ݵ��ļ�
    bool openDbFile(const char* sFullFileName);
    bool writeHeadInfo();
    bool writeFileData(char * sData, int iDataLen );
    bool addData(char *sData, int iCnt);

    void setDataStructSize(size_t iSize){
        m_iDataStructSize = iSize;
    };

    void setFilePath(char * sFilePath){
        strcpy(m_sFilePath, sFilePath);
    };
    char* getFilePath(){
        return m_sFilePath;
    };

    void setFileName(const char * sFileName){
        strcpy(m_sFileName, sFileName);
    };
    char* getFileName(){
        return m_sFileName;
    };

    void setTableType(const int iTableType){
        m_iTableType = iTableType;
    };

    char getDivChar() {
        return m_sDivChar ;
    };
};


#endif //#ifndef SHMFileMgr_H_HEADER_INCLUDED
