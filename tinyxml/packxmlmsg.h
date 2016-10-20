#ifndef _PACK_XML_MSG_H_
#define _PACK_XML_MSG_H_

#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "abmobject.h"
#include "abmcmd.h"
#include "Application.h"
#include "MessageQueue.h"
#include "abmtime.h"
#include "LogV2.h"
#include "tinyxml.h"
#include "ReadXmlConf.h"

class PackXMLMsg : public Application {

public:
	
	PackXMLMsg(): m_poXmlDoc(0)/*, m_fp(0)*/ {}
	
	virtual ~PackXMLMsg()
	{
	    if (m_poXmlDoc != NULL) delete m_poXmlDoc;
	    //if (m_fp != NULL) fclose(m_fp);
    }
    
	virtual int init(ABMException &oExp);
	
	virtual int run();
	
	virtual int destroy();
	
private:
    //���
    int toXml();
    
    //3.4.6.1 ��¼��Ȩ
    int packLoginA();
    
    //3.4.6.2 ֧��
    int packChargeA();
    
    //3.4.6.3 ���ֻ������ѯ
    int packBindNbrA();
    
    //3.4.6.4 ������ѯ
    int packQryBalA();
    
    //3.4.6.5 ����
    int packTransBalA();

    //3.4.6.6 ���������ѯ
    int packQryTransRuleA();

    //3.4.6.7 ������������
    int packResetTransRuleA();

    //3.4.6.8 ֧�������޸�
    int packResetPassWdA();
    
    //3.4.6.9 ֧������״̬��ѯ
    int packQryPayStatusA();
    
    //3.4.6.10 ֧��״̬���
    int packResetPayStatusA();
    
    //3.4.6.11 ֧�������ѯ
    int packQryPayRulesA();
    
    //3.4.6.12 ֧����������
    int packResetPayRulesA();
    
    //3.4.6.13 ����ѯ
    int packQueryA();
    
    //3.4.6.14 ���˼�¼��ѯ
    int packRechargeRecA();
    
    //3.4.6.15 ���׼�¼��ѯ
    int packQryPayLogA();
    
    //3.4.6.16 �����ʵ���ѯ
    int packQryRechargeBillA();
    
private:
    
    TiXmlDocument *m_poXmlDoc;
    
    //FILE *m_fp;
    
    //char m_sXmlFile[64];
	
};

DEFINE_MAIN(PackXMLMsg)


#endif