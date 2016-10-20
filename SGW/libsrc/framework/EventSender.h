// Copyright (c) 2011
// All rights reserved.

#ifndef EVENTSENDER_H_HEADER_INCLUDED_BDD948082
#define EVENTSENDER_H_HEADER_INCLUDED_BDD948082

//#include "Process.h"
#include "SGWSHMParamApi.h"


class MessageQueue;

#include <vector>

using namespace std;
class InnerStruct;

typedef struct 
{
	int		iSendConditionID;
	int		iMqId;
	int		iSendMode;
	int		iBuffType;
	MessageQueue * pmq;
}TRouterRule;

//## �¼�������
class EventSender
{
  public:

    //## ��ʼ�����������طַ����򡢸��ݷַ�����ʵ����Distributer
    EventSender(int iAppID);

    //## �ͷ����������Դ
    ~EventSender();

	//## ���ͱ䳤����
	int send(long lMtype=0);
	int send(string & strSend,ProtoToBusiData & tProtoData,long lMtype=0);

  private:

	vector<TRouterRule> VTRouterRule;

   
   int m_iSendCondition;
   
   CongestLevel m_tCongestLevel;
   
	ServiceContextListBaseData  tmpServiceContextListData;
	UserInfoData  tmpUserInfoData;
	NetInfoData tmpNetInfoData;   
   
   int m_iMaxLev;
   
   int m_iLevSum[10];
   
   int m_iNetSum[10];
   
   int m_iPriorityType;

   InnerStruct * m_poInnerStruct;
};



#endif /* EVENTSENDER_H_HEADER_INCLUDED_BDD94802 */
