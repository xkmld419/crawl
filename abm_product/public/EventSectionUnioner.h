/*VER: 1*/ 
#ifndef EVENTSECTIONUNIONER_H_HEADER_INCLUDED_BD9255AE
#define EVENTSECTIONUNIONER_H_HEADER_INCLUDED_BD9255AE

#include "CommonMacro.h"
#include "StdEvent.h"

//##ModelId=426DAD3A03D5
//##Documentation
//## �¼��ֶεķ��úϲ�����
class EventSectionUnioner
{
  public:
    //##ModelId=426DAD8000DC
    void clear();

    //##ModelId=426DAD880084
    void insert(ChargeInfo *pChargeInfo);

    //##ModelId=426DADB50287
    //##Documentation
    //## ��ȡĳ�ַ��õ�ֵ
    int getCharge(int iAcctItemType);

    //##ModelId=426DAE05007A
    //##Documentation
    //## �ϲ��Ľ�����õ�StdEvent��ȥ
    void copyToEvent(StdEvent *pEvent);
    //##ModelId=426DB00D037A
    EventSectionUnioner();
    //##ModelId=4272E61602A2
    void setCurMeasure(int iMeasure);


    //##ModelId=4272E62902F9
    int getCurMeasure();

    void adjust(int iCharge);


  private:
    //##ModelId=426DAF6D0398
    ChargeInfo m_aoChargeInfo[MAX_CHARGE_NUM];

    //##ModelId=426DAFAD0051
    int m_iChargeNum;
    //##ModelId=4272E60800D5
    int m_iCurMeasure;

    int m_iLastI;

};



#endif /* EVENTSECTIONUNIONER_H_HEADER_INCLUDED_BD9255AE */
