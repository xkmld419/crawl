/*VER: 2*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef EVENTRECEIVER_H_HEADER_INCLUDED_BDC328FA
#define EVENTRECEIVER_H_HEADER_INCLUDED_BDC328FA

#include "Exception.h"
#include "MessageQueue.h"


#include "StdEvent.h"

class StdEvent;
class AcctItemTransData;

//##ModelId=41EF57D20051
//##Documentation
//## �¼�������������Ϣ���н����¼�
class EventReceiver
{
  public:
    //##ModelId=41EF5A6C0117
    //##Documentation
    //## ����Ϣ���н����¼�
    int receive(
        //##Documentation
        //## ���մ�ŵ��Ļ�����
        StdEvent *pEvent, bool ifBlock = true);

	// add by zhangcch
	// ����Ϣ���н���buffer��buffer��СΪStdEvent������size��
	int receive(char *buffer, bool ifBlock = true);

	//����Ϣ���н���һ���ʵ�����
	int receive(AcctItemTransData * itemData, bool ifBlock = true);


    //##ModelId=41F6032A020C
    //##Documentation
    //## ��ʼ��������ʵ������Ϣ���з��ʶ���������Ϣ����
    EventReceiver(int iProcID);

    //##ModelId=41F6032A0252
    //##Documentation
    //## �ͷ����������Դ
    ~EventReceiver();

  private:

    //##ModelId=4226C8C20370
    MessageQueue * m_poMQ;


    
    // �Ƿ�����, add by yangks 2007.02.02
    bool m_bIfBlock;
    int  m_iMaxNum;
	int  m_iMsgEventNum;  // add by zhangch��������Ϣ������StdEvent����
};



#endif /* EVENTRECEIVER_H_HEADER_INCLUDED_BDC328FA */
