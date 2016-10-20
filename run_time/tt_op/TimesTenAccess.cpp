#include "ReadIni.h"
#include "encode.h"
#include "TimesTenAccess.h"


#define ECODE_INVALID_PARA 1001   //������Ч
#define ECODE_LOST_ENV     1002   //ȱʧ��������
#define ECODE_READCFG_FAIL 1003   //��ȡ����ʧ��
#define ECODE_READCFG_EXP  1004   //��ȡ�����쳣
#define ECODE_CONN_TT_FAIL  1006  //����TT���ݿ�ʧ��
#define ECODE_DISCONN_TT_FAIL 1008   //�Ͽ�TT���ݿ�����ʧ��    

#define DATA_ACCESS_INI "data_access.ini"

int GetConnectInfo(const char *pconf, char *usr, char *pwd, char *dsn, ABMException &oExp)
{
    if ((pconf==NULL) || (usr==NULL) || (pwd==NULL) || (dsn==NULL)) {
        ADD_EXCEPT0(oExp, "GetConnectInfo ����ΪNULL");
        return ECODE_INVALID_PARA;	
    }
    
    ReadIni reader;
    char *penv = NULL;
    char HomeEnv[128] = {0};
    char sfile[256] = {0};
    char Pwd[128] = {0};
    if ((penv=getenv("ABMAPP_DEPLOY")) == NULL) {
        ADD_EXCEPT0(oExp, "��������ABMAPP_DEPLOYû������, GetConnectInfo ���ش���");				
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
        ADD_EXCEPT1(oExp, "��ȡ�����ļ�%sʧ��, GetConnectInfo ���ش���\n", sfile);	
        return ECODE_READCFG_EXP;	
    }
    return 0;
}

int ConnectTT(const char *pconf, TimesTenConn *&conn, ABMException &oExp)
{
    if (pconf == NULL) {
        ADD_EXCEPT0(oExp, "ConnectTT function pconf�����������ΪNULL, ConnectTT ���ش���");
        return ECODE_INVALID_PARA;	
    }
    if (conn != NULL) {
        ADD_EXCEPT0(oExp, "ConnectTT function conn �����������ΪNULL, ConnectTT ���ش���");
        return ECODE_INVALID_PARA;	
    }
    char dbusr[128], dbpwd[128], dbdsn[256];
    memset(dbusr, 0x00, sizeof(dbusr));
    memset(dbpwd, 0x00, sizeof(dbpwd));
    memset(dbdsn, 0x00, sizeof(dbdsn));
    
    if (GetConnectInfo(pconf, dbusr, dbpwd, dbdsn, oExp) != 0) {
        ADD_EXCEPT1(oExp, "��ȡ�����ļ��б�ǩΪ%s������ʧ��, ConnectTT ���ش���\n", pconf);
        return ECODE_READCFG_FAIL;	
    } 
    string ttdsn(dbdsn);	
    try {
        conn = new TimesTenConn();
        conn->setDBLogin(ttdsn);
        conn->getDBConn(true);

    }
    catch (TTStatus st) {
        ADD_EXCEPT0(oExp, "��½TT���ݿ�ʧ��,�������ú����ݿ������״̬!!!, ConnectTT ���ش���");
        return ECODE_CONN_TT_FAIL;
    }
    return 0;	
}

int DisconnectTT(TimesTenConn *&conn, ABMException &oExp)
{
    if (conn == NULL) {
        //log
        return ECODE_DISCONN_TT_FAIL;	
    }
    try {	
        conn->disconnect();
        delete conn;
    }
    catch(...) {
        ADD_EXCEPT0(oExp, "TTע��ʱ �����쳣!");
        return ECODE_DISCONN_TT_FAIL;
    }
    conn = NULL;
    return 0;
}

int GetTTCursor(TimesTenCMD *&cursor, TimesTenConn *&conn, const char *pconf, ABMException &oExp)
{
    if (cursor != NULL) {
        return ECODE_INVALID_PARA;		
    }	
    
    int ret;
    if (conn == NULL) {
        if (pconf == NULL) {
            return ECODE_INVALID_PARA;
        }		
        ret = ConnectTT(pconf, conn, oExp);
        if (ret != 0) {
            DisconnectTT(conn, oExp);	
            return ret;
        }
    }
    
    cursor = new TimesTenCMD(conn->getDBConn());
    
    return 0;
}

void RelTTCursor(TimesTenCMD *&cursor)
{
    if (cursor != NULL) {
        delete cursor;
        cursor = NULL;
    }
    return;		
}
