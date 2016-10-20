/*VER: 1*/ 
// // Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// // All rights reserved.

#ifndef ORGATTRFORMATMGR_H_HEADER_INCLUDED_BDB33138
#define ORGATTRFORMATMGR_H_HEADER_INCLUDED_BDB33138

#include "BillConditionMgr.h"
#include "BillOperationMgr.h"
#include "StdEvent.h"
#include "TOCIQuery.h"
#include "Environment.h"
#include "FormatMethod.h"
#include <iostream>

#define RULE_EXIT 1
#define RULE_NOT_EXIT 0
#define MAX_RULE_SQL_LEN 1000

const int FORMART_NORMAL = 0;

//##ModelId=4250AF2E0300
//##Documentation
//## ���Թ�������
class FormatRule
{
  public:
    //##ModelId=42595EB50064
    FormatRule();

    //##ModelId=4250E5BB022E
    int m_iRuleSeq;

    //##ModelId=4250E62C00F0
    int m_iConditionID;

    void * m_poCondition;

    //##ModelId=4250E63D0325
    int m_iOperationID;

    void * m_poOperation;

    //##ModelId=4250E93E0082
    int m_iInEventType;

    //##ModelId=4250E9640145
    int m_iOutEventType;

    //##ModelId=4250E9F703D1
    //##Documentation
    //## 0�����˳�  1���˳�����
    int m_iExitFlag;
    
    char m_sRuleName[80];

    //##ModelId=4250EA30004E
    char m_sEffDate[15];

    //##ModelId=4250EA5501B0
    char m_sExpDate[15];

    //##ModelId=4250EC840120
    FormatRule *m_poNext;


};

//##ModelId=4250AF01003E
//##Documentation
//## ���Թ�������
class FormatStep
{
  public:
    //##ModelId=42595E9D0240
    FormatStep();

    //##ModelId=4250EB1A03C6
    int m_iStepSeq;

    //##ModelId=42510D3801C6
    int m_iFormatID;
    
    char m_sStepName[80];
    
    //##ModelId=4250EC300132
    FormatRule *m_poFormatRule;
    
    //##ModelId=4250EC30013C
    FormatStep *m_poNext;

#ifndef PRIVATE_MEMORY
    unsigned int m_iFormatRuleOffset;
    unsigned int m_iFormatRuleCnt;
#endif

};


//##ModelId=42548B92033E
//##Documentation
//## ���Թ�����
class FormatGroup
{
  public:
    //##ModelId=42595D3E024F
    FormatGroup();

    //##ModelId=42548CF80299
    int m_iFormatID;

    
    //##ModelId=4254C3090226
    FormatGroup *m_poNext;
    
    //##ModelId=4254C3950351
    FormatStep *m_poFormatStep;

#ifndef PRIVATE_MEMORY
    unsigned int m_iFormatStepOffset;
    unsigned int m_iFormatStepCnt;
#endif

};


//##ModelId=4259FE4A019E
//##Documentation
//## ���Թ������,һ�����̿ɶ�Ӧ������Թ���(��),�ֱ���������ͬ���¼�����(����/���ݵ�)
class FormatComb
{
  public:
    //##ModelId=4259FEC102E9
    FormatComb();

    //##ModelId=4259FECD01A6
    //##Documentation
    //## Դ�¼�����
    int m_iSourceEventType;

    //##ModelId=4259FF1C0222
    int m_iFormatID;

    //##ModelId=4259FF780333
    FormatGroup *m_poFormatGroup;
    
    //##ModelId=425A00DF00B8
    FormatComb *m_poNext;

#ifndef PRIVATE_MEMORY
    int m_iProcessID;
    unsigned int m_iFormatGroupOffset;
#endif

};

//##ModelId=41CF5D0E0355
//##Documentation
//## ԭʼ���Թ����Ǹ������õ�ҵ���������ȡ�������¼����ԡ��¼����Ե���ȡ����Ŀ����Ϊ����ȡ�¼����͡�
//## ԭʼ���Թ������ò����ӿ����ȡ���������ʹ�����򣬵��ù�����
//## BillConditionMgr��BillOperationMgr��������������Ļ�ȡ���ж��Լ����ù�
//## ���ĵײ����㺯�����в����������ڸ��ӵ�ҵ����������
//## FormatMethod�ṩ�ķ�����
//## �ڴ�������У���������쳣������
//## ������š�
//## 
//## 
//## ���¼����ԵĲ���������Ϊ�������ࣺ
//## 1������һ������������ֱ�Ӹ�ֵ����ϵ����ͼ򵥵�ҵ���������Ĳ������費��Ҫ̫�࣬
//##    �ɵײ㹫������ɣ�
//## 2�����ӵ�ҵ������Ҫ��ѯ�������ұ����ݺܶ࣬�ɸ���ģ���ṩ������
//##    ���еĲ��������ɺ�̨�ṩ��Ӧ�Ĵ���ʽ�����������ɵײ㹫������
//## ��ɣ����ӵ�ҵ�����ɸ���ģ��ʵ�֡�ϵͳ�ṩ�������в�����˵����
//## �û���ǰ̨�����������Թ����Ĵ������̣�ѡ����Ҫ�Ĳ���������
//## 
//## 
class AttrTransMgr
{
  public:
    //##ModelId=42510090028F
    AttrTransMgr(int iProcessID);

    //##ModelId=41EF8BBF026F
    //##Documentation
    //## �Ե�ǰ�¼��������Թ���
    //## �¼�������������0
    //## �¼������쳣�����쳣����
    int trans(StdEvent* pStdEvent);

    //##ModelId=4250DD97000B
    //##Documentation
    //## ����/�����������Թ�������
    void reloadRule();
    
    //##ModelId=4250F0750395
    void unloadRule();
    
    //##ModelId=425952FB005D
    void printRule();
    
    static void printEvent(StdEvent *_StdEvent);

    //##ModelId=4259E98B002E
    ~AttrTransMgr();


  private:
    //##ModelId=425123D60045
    //##Documentation
    //## �д��󣬷��ش����
    //## �޴��󣬷���0
    int checkAttrErr();

	int existChar(const char * in_string);
	
    //##ModelId=425124ED019B
    //##ModelId=425124ED019B
    //##Documentation
    //## �д��󣬷��ش����
    //## �޴��󣬷���0
    int checkTimeEffect(char *sTime);
    
    //##ModelId=425104F40277
    int m_iProcID;
    
    //##ModelId=4254BD4E0261
    StdEvent* m_poStdEvent;
    //##ModelId=4254C0170125
    
    //##Documentation
    //## ���Թ������ͷָ��
    FormatComb * m_poFormatComb;
    
    //##ModelId=425A087C0056
    //##Documentation
    //## ���Թ�����ͷָ��
    FormatGroup *m_poFormatGroup;
    
    int m_iNeedSavFormatRec;
};



#endif /* ORGATTRFORMATMGR_H_HEADER_INCLUDED_BDB33138 */

