/*
*   Version js at 2010-11-25
*/
#ifndef __READ_APP_CONF_H_INCLUDED_
#define __READ_APP_CONF_H_INCLUDED_

#include "ReadCfg.h"
#include "ABMException.h"

/*******�ۻ��������ļ���ȡ��************/
struct ABMConfStruct
{
    int m_iSndKey;     //������Ϣ����KEY
    int m_iRcvKey;     //������Ϣ����KEY
    int m_iProcID;     //����ID
    
    int m_iSndToPlatformKey; // �����Է���ƽ̨��Ϣ����KEY
};

struct DccConfStru
{
    int m_iSndKey;          //������Ϣ����KEY
    int m_iSndPeerKey;      //����sock����������Ϣ���У�������������ã�
    int m_iRcvKey;          //������Ϣ����KEY
    int m_iProcID;          //����ID
    char m_sDictFile[128];  //�ֵ��ļ�����
};

class ABMConf 
{
public:

    static ReadCfg *m_poConf; 

    //���ݴ���Ľ���ID����������ۻ�������
    static int getConf(int proc, ABMConfStruct &conf, ABMException &oExp);
    
    //dcc��Ϣ��ؽ��̵����ö�ȡ
    static int getDccConf(int proc, DccConfStru &conf, ABMException &oExp);

};


#endif

