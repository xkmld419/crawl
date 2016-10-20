#ifndef __AUTO_CLEAR_ABMTT_H_
#define __AUTO_CLEAR_ABMTT_H_

#include "TimesTenAccess.h"
#include "ABMException.h"
#include "ReadCfg.h"

//#include "ThreeLogGroup.h"


struct ABMClearInfo
{
    char m_cMDBTable[32+1]; //�ڴ�����
    char m_cMDBName[32+1];//tt����
    char m_cTimeName[32+1];//ʱ���ֶ���    
    int  m_iTableType; //������ͣ�0�����ۼ�����1�������
};

struct ABMArchiveData
{
    char m_cVectorData[1024];
};



class ABMClearProduct
{
public:
    ABMClearProduct();
    ~ABMClearProduct();
    int clearInit(ABMException &oExp);
    int getABMClearConf(ABMException &oExp, char *pTableName, char *pTTDBName);
    int executeClear(ABMException &oExp);  //abmclear
     
    int connectTT(ABMException &oExp);
    int executeArchive(ABMException &oExp,char *sDbName);
    int executeArchiveSQL(ABMException &oExp, char *sDbTable, char *cDbName);   
private:
    vector<ABMClearInfo> m_oABMClearConf;
    
    char m_sTTStr[32+1]; 
    ReadCfg m_poTableCfg;
    int m_iLineNum;
    char m_sFileName[1024]; 
    
    
    TimesTenConn *m_poTTconn; //TT������
    TimesTenCMD *m_poClearMDB; //TT �α� 
    
    string m_sArchiveSQL;
    vector<ABMArchiveData> m_oABMArchiveConf;
    int m_iFieldNum; //�ֶ���
    
    
    
    /*
        int m_iColFlag[20];
    char m_cColName[20][30];
    */
public:
    char  m_sDateTime[14 + 1]; //ʱ�� 
    FILE *m_poArchiveData;
   
};

#endif