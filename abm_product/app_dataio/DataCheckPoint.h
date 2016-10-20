#ifndef DATACHECKPOINT_H_HEADER_INCLUDED
#define DATACHECKPOINT_H_HEADER_INCLUDED

#include "HashList.h"
#include "SHMSqlMgr.h"
#include "ThreeLogGroup.h"
//#include "CommandCom.h"
#include "ReadCfg.h"
#include "TableStruct.h"
#include "MBC_ABM.h"

#define DATAFILE  "datafile/"

#define ABMPROCONFIG  "etc/"

int connectTT(char *memDSN);


class DataCheckPoint:public TTTns {
public:
    int g_argc;
    char **g_argv;
    char* m_sMemDsn;              //�ڴ��ʶ��Key�����ڴ����
    char* m_sDbTable;               //���ݿ����
    char* m_sFileName;              //�������ļ���
    char m_sFileNameX[150];
    int m_iMode;                    //1:���������ݿ��,2:�������ļ�
    bool m_bIgnore;                 //���ý������������Ļ��
    int m_iTableType;
    HashList<int> *m_phList;        //�ڴ��ʶ��������  
    ThreeLogGroup *m_poLogGroup;
    //CommandCom *m_poCommandCom;
    
    //static TimesTenCMD  *qryCursor2;
	
    //SHMSqlMgr *g_poSql;
public:
    DataCheckPoint(int argc, char **argv);
    ~DataCheckPoint();
    int run();
    int runApi();
	
	int check();
	int checkDSN();
	char m_sConnTns[256];

public:
    ReadCfg *m_poConfS;
    ReadCfg *m_poConfT;
	int TableCol;

	int TableNum;
  string sql;
	int QurSql();
	int expdata();
	int iTotalCnt;
	FILE *fp;
private:
    void reset();
    void init();
    void printUsage();
    bool prepare();
};


#endif //#ifndef DATACHECKPOINT_H_HEADER_INCLUDED
