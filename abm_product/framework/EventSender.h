/*VER: 2*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef EVENTSENDER_H_HEADER_INCLUDED_BDD94802
#define EVENTSENDER_H_HEADER_INCLUDED_BDD94802
//#include "DistributeRule.h"



class MessageQueue;
class BillConditionMgr;

#include "StdEvent.h"

class StdEvent;
class AcctItemTransData;

//##ModelId=41EF55000310
//##Documentation
//## �¼�������
class EventSender
{
  public:
    //##ModelId=41EF5A7203BF
    //##Documentation
    //## ����һ���¼������ε���ÿ��Distributer��send����
    //## ��������¼������͸���Щ���̵��б�
   // int * send (StdEvent *evnet);

	// add by zhangch
    // ����һ��buffer����СΪStdEvent������size�������ε���ÿ��Distributer��send����
    // ��������¼������͸���Щ���̵��б�
   // int * send (char *buffer);

	//����һ���ʵ�����
	//int * send (AcctItemTransData * itemData,StdEvent * evnet,bool bNew = true);



    //##ModelId=41F603420134
    //##Documentation
    //## ��ʼ�����������طַ����򡢸��ݷַ�����ʵ����Distributer
    EventSender(int iProcID);

    //##ModelId=41F603420166
    //##Documentation
    //## �ͷ����������Դ
    ~EventSender();
    //##ModelId=428D5B2203B6
    //##Documentation
    //## ����ĳFile�Ļ��������͵�����Щ����
    //int *getOutProcessList(int iFileID);


  private:



    //##ModelId=4226D671038B
   // DistributeRule * m_poRuleList;
    //##ModelId=428D4A1000E2
   // BillConditionMgr *m_poBillConditionMgr;
    //##ModelId=428D5B92014A
    //##Documentation
    //## ���ProcessID�Ļ�����������getOutProcessList
    int m_aiProcessIDBuf[MAX_OUT_PROCESS_NUM];


	// int  m_iMsgEventNum;  // add by zhangch��������Ϣ������StdEvent����

};



#endif /* EVENTSENDER_H_HEADER_INCLUDED_BDD94802 */
