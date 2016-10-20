/*VER: 1*/ 
#ifndef EVENTTYPE_H_HEADER_INCLUDED_BDAAED9D
#define EVENTTYPE_H_HEADER_INCLUDED_BDAAED9D

#include "CommonMacro.h"
#include "EventTypeMgr.h"

//##ModelId=4254FF8F01E1
class EventType
{
    friend class EventTypeMgr;
    friend class ParamInfoMgr;

  public:
    //##ModelId=4255001A0173
    EventType();

//  private:
    //##ModelId=4254FF960209
    int m_iEventType;

    //##ModelId=4255053C03A3
    int m_iParentType;

    //##ModelId=4281CF33015D
    int m_iTableType;

    //##ModelId=4254FFCB029B
    EventType *m_poParent;

	//��ʾ���¼������ڲ�ͬ���ײ����Ƿ���(1:����,0:������),Ĭ��Ϊ1
	int m_iIsMutex;

#ifndef PRIVATE_MEMORY
    unsigned int m_iParentOffset;
#endif

};

#endif /* EVENTTYPE_H_HEADER_INCLUDED_BDAAED9D */
