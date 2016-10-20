/*
*   Version js at 2010-11-25
*/
#ifndef __READ_DCC_CONF_H_INCLUDED_
#define __READ_DCC_CONF_H_INCLUDED_

#include "ReadCfg.h"
#include "ABMException.h"

struct DccConfStru
{
    int m_iSndKey;          //������Ϣ����KEY
    int m_iSndPeerKey;      //����sock����������Ϣ���У�������������ã�
    int m_iSndCcaKey;       //�����������Ӧ�Ļذ���������Ϣ����
    int m_iRcvKey;          //������Ϣ����KEY
    int m_iProcID;          //����ID
    char m_sDictFile[128];  //�ֵ��ļ�����
	int m_iSockID;
};

struct NodeConfStru
{
    char m_sOriginHost[64];
    char m_sOriginRealm[32];
};

class DccConf 
{
public:

    static ReadCfg *m_poConf; 
    
    //dcc��Ϣ��ؽ��̵����ö�ȡ
    static int getDccConf(int proc, DccConfStru &conf, ABMException &oExp);
    
    //��ȡOrigin-Host��Origin-Realm������
    static int getNodeConf(NodeConfStru &conf, ABMException &oExp);

};


#endif

