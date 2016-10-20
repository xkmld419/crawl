#ifndef _PARSER_XML_MSG_H_
#define _PARSER_XML_MSG_H_

#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "abmobject.h"
#include "abmcmd.h"
#include "Application.h"
#include "MessageQueue.h"
#include "LogV2.h"
#include "tinyxml.h"
#include "ReadXmlConf.h"

class ParserXMLMsg : public Application {

public:
	
	ParserXMLMsg(): m_poXmlDoc(0)/*,m_fp(0)*/ {}
	
	virtual ~ParserXMLMsg() 
	{
	    if (m_poXmlDoc != NULL) delete m_poXmlDoc;
	    //if (m_fp != NULL) fclose(m_fp);
    }
    
	virtual int init(ABMException &oExp);
	
	virtual int run();
	
	virtual int destroy();

private:
    int selfDccHead( ABMCCR *pCCR);
    //���
    int toStruct();
	
	//����
    int parseReady(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.1 ��¼��Ȩ
    int parseLoginR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.2 ֧��
    int parseChargeR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.3 ���ֻ������ѯ
    int parseBindNbrR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.4 ������ѯ
    int parseQryBalR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.5 ����
    int parseTransBalR(TiXmlHandle &SrvInfoHandle);

    //3.4.6.6 ���������ѯ
    int parseQryTransRuleR(TiXmlHandle &SrvInfoHandle);

    //3.4.6.7 ������������
    int parseResetTransRuleR(TiXmlHandle &SrvInfoHandle);

    //3.4.6.8 ֧�������޸�
    int parseResetPassWdR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.9 ֧������״̬��ѯ
    int parseQryPayStatusR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.10 ֧��״̬���
    int parseResetPayStatusR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.11 ֧�������ѯ
    int parseQryPayRulesR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.12 ֧����������
    int parseResetPayRulesR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.13 ����ѯ
    int parseQueryR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.14 ���˼�¼��ѯ
    int parseRechargeRecR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.15 ���׼�¼��ѯ
    int parseQryPayLogR(TiXmlHandle &SrvInfoHandle);
    
    //3.4.6.16 �����ʵ���ѯ
    int parseQryRechargeBillR(TiXmlHandle &SrvInfoHandle);
	
private:
    
    TiXmlDocument *m_poXmlDoc;
    
    //FILE *m_fp;
    
    char m_sXmlFile[64];
	
};

DEFINE_MAIN(ParserXMLMsg)

#endif