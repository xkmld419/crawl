#include "ReadDccConf.h"

#define ABM_HOME "ABMAPP_DEPLOY"

ReadCfg * DccConf::m_poConf = NULL;

int DccConf::getDccConf(int proc, DccConfStru &conf, ABMException &oExp)
{
    char * const pLable = "DCC_CONF";
    if (m_poConf == NULL) { 
        if ((m_poConf=new ReadCfg) == NULL) {
            ADD_EXCEPT0(oExp, "ABMConf::getConf m_poConf malloc failed!");
            return -1;
        }
    }    
    char *p = getenv(ABM_HOME);
    if (p == NULL) {
        ADD_EXCEPT0(oExp, "AccuConf::getConf ABMAPP_DEPLOY ��������û������!");
        return -1;
    }
    
    char sValue[128];
    char path[1024];
    char tmp[1024] = {0};
    strncpy(tmp, p, sizeof(tmp)-2);
    if (tmp[strlen(tmp)-1] != '/') {
        tmp[strlen(tmp)] = '/';
    }
    snprintf(conf.m_sDictFile, sizeof(conf.m_sDictFile), "%sconfig/dictionary.xml", tmp);
    snprintf(path, sizeof(path), "%sconfig/abm_app_cfg.ini", tmp);
    if (m_poConf->read(oExp, path, pLable) != 0) {
        ADD_EXCEPT2(oExp, "AccuConf::getConf ��ȡ�����ļ�%s�е�%s����", path, pLable);
        return -1;
    }  
    
    int iLines = m_poConf->getRows();

    while(iLines > 0) {
        memset(sValue, 0x00, sizeof(sValue));
        if (!m_poConf->getValue(oExp, sValue, "proc", iLines)) {
            ADD_EXCEPT1(oExp, "�����ļ���%s��δ����procѡ��,��ȡʧ��!", pLable);
            return -1;
        }
        if (proc != atoi(sValue)) {
            iLines--;
            continue;
        }         
        
        memset(sValue, 0x00, sizeof(sValue));
        if (!m_poConf->getValue(oExp, sValue, "mqsnd", iLines)) {
            ADD_EXCEPT1(oExp, "�����ļ���%s��δ����mqsndѡ��,��ȡʧ��!", pLable);
            return -1;
        }
        conf.m_iSndKey = atoi(sValue);
        
        memset(sValue, 0x00, sizeof(sValue));
        if (!m_poConf->getValue(oExp, sValue, "mqsend_peer", iLines)) {
            ADD_EXCEPT1(oExp, "�����ļ���%s��δ����mqsndѡ��,��ȡʧ��!", pLable);
            return -1;
        }
        conf.m_iSndPeerKey = atoi(sValue);
        
        memset(sValue, 0x00, sizeof(sValue));
        if (!m_poConf->getValue(oExp, sValue, "mqsend_cca", iLines)) {
            ADD_EXCEPT1(oExp, "�����ļ���%s��δ����mqsndѡ��,��ȡʧ��!", pLable);
            return -1;
        }
        conf.m_iSndCcaKey = atoi(sValue);

        memset(sValue, 0x00, sizeof(sValue));
        if (!m_poConf->getValue(oExp, sValue, "mqrcv", iLines)) {
            ADD_EXCEPT1(oExp, "�����ļ���%s��δ����mqrcvѡ��,��ȡʧ��!", pLable);
            return -1;
        }
        conf.m_iRcvKey = atoi(sValue);
		
		memset(sValue, 0x00, sizeof(sValue));
        if (!m_poConf->getValue(oExp, sValue, "SOCKID", iLines)) {
            ADD_EXCEPT1(oExp, "�����ļ���%s��δ����mqrcvѡ��,��ȡʧ��!", pLable);
            return -1;
        }
        conf.m_iSockID = atoi(sValue);
        break;
    }
    
    conf.m_iProcID = proc;
    
    return !iLines;
}


int DccConf::getNodeConf(NodeConfStru &conf, ABMException &oExp)
{
    char * const pLable = "DCC_NODE_CONF";
    if (m_poConf == NULL) { 
        if ((m_poConf=new ReadCfg) == NULL) {
            ADD_EXCEPT0(oExp, "ABMConf::getConf m_poConf malloc failed!");
            return -1;
        }
    }
    char *p = getenv(ABM_HOME);
    if (p == NULL) {
        ADD_EXCEPT0(oExp, "AccuConf::getConf ABMAPP_DEPLOY ��������û������!");
        return -1;
    }
    
    char sValue[128];
    char path[1024];
    char tmp[1024] = {0};
    strncpy(tmp, p, sizeof(tmp)-2);
    if (tmp[strlen(tmp)-1] != '/') {
        tmp[strlen(tmp)] = '/';
    }
    snprintf(path, sizeof(path), "%sconfig/abm_app_cfg.ini", tmp);
    if (m_poConf->read(oExp, path, pLable) != 0) {
        ADD_EXCEPT2(oExp, "AccuConf::getConf ��ȡ�����ļ�%s�е�%s����", path, pLable);
        return -1;
    }
    
    int iLines = m_poConf->getRows();
    
    if (iLines > 0) {
        memset(sValue, 0x00, sizeof(sValue));
        if (!m_poConf->getValue(oExp, sValue, "origin-host", iLines)) {
            ADD_EXCEPT1(oExp, "�����ļ���%s��δ����origin-hostѡ��,��ȡʧ��!", pLable);
            return -1;
        }
        snprintf(conf.m_sOriginHost, sizeof(conf.m_sOriginHost), sValue);   
        
        memset(sValue, 0x00, sizeof(sValue));
        if (!m_poConf->getValue(oExp, sValue, "origin-realm", iLines)) {
            ADD_EXCEPT1(oExp, "�����ļ���%s��δ����origin-realmѡ��,��ȡʧ��!", pLable);
            return -1;
        }
        snprintf(conf.m_sOriginRealm, sizeof(conf.m_sOriginHost), sValue);
    }
    
    return !iLines;
}