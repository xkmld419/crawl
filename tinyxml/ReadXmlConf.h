/*
*   Version js at 2010-11-25
*/
#ifndef __READ_XML_CONF_H_INCLUDED_
#define __READ_XML_CONF_H_INCLUDED_

#include "ReadCfg.h"
#include "ABMException.h"

struct XmlConfStru
{
    int m_iSndKey;          //������Ϣ����KEY
    int m_iSndPeerKey;      //����sock����������Ϣ���У�������������ã�
    int m_iRcvKey;          //������Ϣ����KEY
	int m_iSndCCAKey;        //��������������Ϣ����
    int m_iProcID;          //����ID
};

class XmlConf 
{
public:

    static ReadCfg *m_poConf; 
    
    //dcc��Ϣ��ؽ��̵����ö�ȡ
    static int getXmlConf(int proc, XmlConfStru &conf, ABMException &oExp);

};


#endif

