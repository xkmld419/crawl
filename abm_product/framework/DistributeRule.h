/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef DISTRIBUTERULE_H_HEADER_INCLUDED_BDB2D7D5
#define DISTRIBUTERULE_H_HEADER_INCLUDED_BDB2D7D5

#include "CommonMacro.h"
#include "MessageQueue.h"
#include "KeyList.h"
class SendRecord;

//##ModelId=41F4ED18001F
//##Documentation
//## �ַ����򣬶�Ӧ���ݿ����������
class DistributeRule
{
  public:
    //##ModelId=424CFCE6018D
    DistributeRule(MessageQueue *poMQ, int conID, DistributeRule *next, int iOutProcessID);

    //##ModelId=41F4F02702A6
    //##Documentation
    //## ������ʶ
    int m_iConditionID;

    //##ModelId=424BED8E0348
    MessageQueue *m_poMQ;

    //##ModelId=424CFC3302E4
    DistributeRule *m_poNext;
    //##ModelId=428D5A6D0120
    int m_iOutProcessID;

	int m_iMsgEventNum;  // add by zhangch��������Ϣ������StdEvent����

    //##ModelId=429141D00057
    KeyList <SendRecord *> *m_poSendRecord;

};

#endif /* DISTRIBUTERULE_H_HEADER_INCLUDED_BDB2D7D5 */
