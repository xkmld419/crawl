/*VER: 1*/ 
#ifndef BILLINGCYCLEADMIN_H_HEADER_INCLUDED_BD5BDEB0
#define BILLINGCYCLEADMIN_H_HEADER_INCLUDED_BD5BDEB0
#include "BillingCycleBase.h"
class BillingCycleSchedule;
class BillingCycleCfg;
class BillingCycle;

//##ModelId=42A3FAA50061
//##Documentation
//## �������ڵķ������
class BillingCycleAdmin : public BillingCycleBase
{
  public:
    //##ModelId=42A3FFE2007E
    //##Documentation
    //## ���캯����
    //## �������û�м��أ���ô�����������
    BillingCycleAdmin();

    //##ModelId=42A4000802EF
    //##Documentation
    //## ���ݹ���
    //## 1�������������ڣ�ͬʱ������صı��
    //## 2������״̬���ģ����ڴ沢�Ҹ����ݿ⡣
    //## ״̬����ʱ���ȸ��ڴ棬�ٸ����ݿ�
    //## 
    //## ���ݣ�
    //## һ��ĳ�������͵Ŀ�ʼʱ�䣬�������
    //## �����������ڵ�����
    int admin(
        //##Documentation
        //## �Ƿ����̷��أ�
        //## ��������̷��صĻ������ص��Ƕ������Ҫ������һ�ι���
        bool bBool = false);
    //##ModelId=42A56ED400A8
    BillingCycleSchedule *m_poSchedule;


  private:
    //##ModelId=42A4479203BC
    //##Documentation
    //## ִ�е�һ�εļ�������
    void load();

    //##ModelId=42A447A803C8
    //##ModelId=42A447A803C8
    //##Documentation
    //## �ƶ�ά���ƻ�
    void makeSchedule();

    
    //##ModelId=42A44AB402F7
    //##Documentation
    //## ����һ���������ڵ��ڴ�
    //## ���ܻᵼ���������ڳ��ڴ�
    int insert(BillingCycle &oBillingCycle);

    
    //##ModelId=42A50F520296
    void synCfg();
    //##ModelId=42A6A4DC0353
    void insertSchedule(BillingCycleSchedule *pSchedule);


    //##ModelId=42A6A50D03D6
    void newCycle(int iCycleType);

    //##ModelId=42A50EE7007F
    //##ModelId=42A6A51502A1
    void changeCycle(int iCycleType);

    int m_iCfgCount;


    //##ModelId=42A50ECB031E
    BillingCycleCfg *m_poCfgList;


};



#endif /* BILLINGCYCLEADMIN_H_HEADER_INCLUDED_BD5BDEB0 */
