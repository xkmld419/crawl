/*VER: 1*/ 
#ifndef BILLINGCYCLESCHEDULE_H_HEADER_INCLUDED_BD5BFFF5
#define BILLINGCYCLESCHEDULE_H_HEADER_INCLUDED_BD5BFFF5

#include "Date.h"
//##ModelId=42A407F401FB
//##Documentation
//## ���ڹ���ָ��
class BillingCycleSchedule
{
  public:

    //##ModelId=42A4101002BF
    //##Documentation
    //## �жϵ�ǰ�����Ƿ�ִ��
    bool check();

    //##ModelId=42A4081C028F
    //##Documentation
    //## op��
    //## 1��new һ�������������͵��������ڵ���ز���
    //## 2��update һ���������ڵ�״̬
    int m_iOperation;
    //##ModelId=42A4086E0279
    //##Documentation
    //## ��new��ʱ��
    //## ����������������
    //## ��update��ʱ��
    //## �����������ڱ�ʶ
    int m_iCycleType;
    //##ModelId=42A40E9E005A
    Date m_oDate;
    //##ModelId=42A56E6800DF
    BillingCycleSchedule *m_poNext;


  private:





};



#endif /* BILLINGCYCLESCHEDULE_H_HEADER_INCLUDED_BD5BFFF5 */
