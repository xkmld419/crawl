#include "OracleAccess.h"
#include "ReadIni.h"
#include "encode.h"


#define ECODE_INVALID_PARA 1001   //������Ч
#define ECODE_LOST_ENV     1002   //ȱʧ��������
#define ECODE_READCFG_FAIL 1003   //��ȡ����ʧ��
#define ECODE_READCFG_EXP  1004   //��ȡ�����쳣
#define ECODE_CONN_ORA_FAIL 1005  //����ORA���ݿ�ʧ��
#define ECODE_DISCONN_ORA_FAIL 1007  //�Ͽ�ORA���ݿ�����ʧ��   

#define DATA_ACCESS_INI "data_access.ini"

int GetOraConnectInfo(const char *pconf, char *usr, char *pwd, char *dsn, ABMException &oExp)
{
    if ((pconf==NULL) || (usr==NULL) || (pwd==NULL) || (dsn==NULL)) {
        ADD_EXCEPT0(oExp, "GetOraConnectInfo ����ΪNULL");
        return ECODE_INVALID_PARA;	
    }
    
    ReadIni reader;
    char *penv = NULL;
    char HomeEnv[128] = {0};
    char sfile[256] = {0};
    char Pwd[128] = {0};
    if ((penv=getenv("ABMAPP_DEPLOY")) == NULL) {
        ADD_EXCEPT0(oExp, "��������ABMAPP_DEPLOYû������, GetOraConnectInfo ���ش���");				
        return ECODE_LOST_ENV;
    }
    strncpy(HomeEnv, penv, sizeof(HomeEnv)-2);
    if (HomeEnv[strlen(HomeEnv)-1] != '/') {
        HomeEnv[strlen(HomeEnv)] = '/';				
    }
    snprintf(sfile, sizeof(sfile), "%sconfig/%s", HomeEnv, DATA_ACCESS_INI);
    
    try {
        if (reader.readIniString (sfile, pconf, "username", usr, 0) == NULL) {
            return ECODE_READCFG_FAIL;	
        }
        if (reader.readIniString (sfile, pconf, "password", Pwd, 0) == NULL) {
            return ECODE_READCFG_FAIL;
        }
        decode(Pwd, pwd);
        if (reader.readIniString (sfile, pconf, "connstr", dsn, 0) == NULL) {
            return ECODE_READCFG_FAIL;	
        } 
        if ((usr[0]=='\0') || (pwd[0]=='\0') || (dsn[0]=='\0')) {
            ADD_EXCEPT0(oExp, "��½��Ϣ��ȡ��ȫ, �����û���,����,���Ӵ�����!");
            return ECODE_READCFG_FAIL;	
        }
    }
    catch(...) {
        ADD_EXCEPT1(oExp, "��ȡ�����ļ�%sʧ��, GetOraConnectInfo ���ش���\n", sfile);	
        return ECODE_READCFG_EXP;	
    }
    return 0;
}
int ConnectOra(const char *pconf, TOCIDatabase *&database, ABMException &oExp)
{
    if (pconf == NULL) {
        ADD_EXCEPT0(oExp, "ConnectOra function  pconf�����������ΪNULL, ConnectOra ���ش���");
        return ECODE_INVALID_PARA;	
    }
    if (database != NULL) {
        ADD_EXCEPT0(oExp, "ConnectOra function database�����������ΪNULL, ConnectOra ���ش���");
        return ECODE_INVALID_PARA;	
    }
    char dbusr[128], dbpwd[128], dbdsn[256];
    memset(dbusr, 0x00, sizeof(dbusr));
    memset(dbpwd, 0x00, sizeof(dbpwd));
    memset(dbdsn, 0x00, sizeof(dbdsn));
    if (GetOraConnectInfo(pconf, dbusr, dbpwd, dbdsn, oExp) != 0) {
        ADD_EXCEPT1(oExp, "��ȡ�����ļ��б�ǩΪ%s������ʧ��, ConnectOra ���ش���", pconf);
        return ECODE_READCFG_FAIL;	
    }	
    
    try {
        database = new TOCIDatabase();
        if (database->connect(dbusr, dbpwd, dbdsn)) {
            return 0;	
        }
        else {
            ADD_EXCEPT0(oExp, "connect�������ݿ�ʧ��,�������ú����ݿ������״̬!!!, ConnectOra ���ش���");
            return ECODE_CONN_ORA_FAIL;	
        }

    }
    catch (TOCIException &oe) {
        ADD_EXCEPT2(oExp, "��½���ݿ��쳣, oe.getErrMsg()=%s ,oe.getErrSrc()=%s", oe.getErrMsg(), oe.getErrSrc());
        return ECODE_CONN_ORA_FAIL;
    }
    return -1;		
}

int DisconnectOra(TOCIDatabase *&database, ABMException &oExp)
{
    if (database == NULL) {
        //ADD_EXCEPT0(oExp, "DisconnectOra function database�����������ΪNULL,  ���ش���");
        return ECODE_INVALID_PARA;	
    }
    try {	
        database->disconnect();
        delete database;
    }
    catch(TOCIException &oe) {
        ADD_EXCEPT1(oExp, "ע�����ݿ��쳣, oe.getErrMsg()=%s ", oe.getErrMsg());
        ADD_EXCEPT1(oExp, "ע�����ݿ��쳣, oe.getErrSrc()=%s ", oe.getErrSrc());
        return ECODE_DISCONN_ORA_FAIL;		
    }
    database = NULL;
    return 0;
}

int GetOraCursor(TOCIQuery *&cursor, TOCIDatabase *&oci, const char *pconf, ABMException &oExp)
{
    if (cursor != NULL) {
        return ECODE_INVALID_PARA;		
    }	
    
    int ret;
    if (oci == NULL) {
        if (pconf == NULL) {
            return ECODE_INVALID_PARA;
        }		
        ret = ConnectOra(pconf, oci, oExp);
        if (ret != 0) {
            DisconnectOra(oci, oExp);	
            return ret;
        }
    }
    
    cursor = new TOCIQuery(oci);
    
    return 0;
}

void RelOraCursor(TOCIQuery *&cursor)
{
    if (cursor != NULL) {
        delete cursor;
        cursor = NULL;
    }
    return ;		
}
