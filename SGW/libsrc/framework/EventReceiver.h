// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef EVENTRECEIVER_H_HEADER_INCLUDED_BDC329FA
#define EVENTRECEIVER_H_HEADER_INCLUDED_BDC329FA

#include "Exception.h"
#include "MessageQueue.h"
#include "SgwDefine.h"
#include <vector>

using namespace std;

typedef struct 
{
	int		m_iBuffType;
	MessageQueue * pmq;
}TReceiveInfo;
class InnerStruct;

//## �¼�������������Ϣ���н����¼�
class EventReceiver
{
  public:
    //## ��ʼ��������ʵ������Ϣ���з��ʶ���������Ϣ����
    EventReceiver(int iProcID);

    //## �ͷ����������Դ
    ~EventReceiver();

    //## ����Ϣ���б䳤����,��������
    int receive(long lMtype=0 , bool bParse=true);

	int receive(ProtoToBusiData & tProtoData, long lMtype=0 , bool bParse=true);
    
    int	receive(vector<ProtoToBusiData> &VTProtoToBusiData,long lMtype=0 , bool bParse=true);

    //## ����Ϣ���б䳤����,��������
    int send(long lMtype=0 );

	void setBlockFlag(bool ifblock=true);

	int getMessageNumber();

  private:
  	
  	vector<TReceiveInfo> VTReceiveInfo;
  	
  	vector<ProtoToBusiData> VTProtoToBusiData;

//	int m_iBuffType;
	
	int m_iPriorityType;
	
	int	m_iPriorityLev;
	
	int	m_iReceiveSum;
	
	int m_iLevSum[10];
	
	int	m_iNetPrior;
	
	int m_iNetSum[10];
//    MessageQueue * m_poMQ;
	InnerStruct * m_poInnerStruct;
};



#endif /* EVENTRECEIVER_H_HEADER_INCLUDED_BDC328FA */
