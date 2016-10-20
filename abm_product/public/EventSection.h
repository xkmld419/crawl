/*VER: 1*/ 
#ifndef EVENTSECTION_H_HEADER_INCLUDED_BDA5C464
#define EVENTSECTION_H_HEADER_INCLUDED_BDA5C464

#include "StdEvent.h"
#include "UserInfoReader.h"

#include <string>

#include <iostream>
using namespace std;

class ExtendTempValue
{
  private:
    long  aLongValue[100];
    char  aStringValue[100][MAX_STRING_VALUE_LEN];

    bool  bModFlag;
    
  public:
    ExtendTempValue () { bModFlag = true; }
    
    void setLongValue (int iPos, long lValue)
    {
        if (iPos<0 || iPos>99) return;
        aLongValue[iPos] = lValue;        
        bModFlag = true;
    }
    void setStringValue (int iPos, char *sValue)
    {
        if (iPos<0 || iPos>99) return;
	int iLen =strlen(sValue)>(MAX_STRING_VALUE_LEN-1)?MAX_STRING_VALUE_LEN-1:strlen(sValue);
        strncpy (aStringValue[iPos], sValue,iLen);
        aStringValue[iPos][iLen]=0;
	
	//strcpy (aStringValue[iPos], sValue);	
	bModFlag = true;
    }
    
    long getLongValue (int iPos)
    {
        if (iPos<0 || iPos>99) return 0;
        return aLongValue[iPos];
    }
    void getStringValue (int iPos, char *sValue)
    {
        if (iPos<0 || iPos>99) return ;
        strcpy (sValue, aStringValue[iPos]);
    }
    
    void reset () {
        if (bModFlag) {
            memset (this, 0, sizeof (ExtendTempValue));
        }
    }
};

//##ModelId=4254DFF30253
class EventSection : public StdEvent
{
  public:
    //##ModelId=4254E05601AB
    static void reset();
        
    //##ModelId=4254E6D2022B
    EventSection *malloc();

    //##ModelId=4254E84C0007
    static EventSection *initMemory(int number);

    //##ModelId=425A154500A0
    static void freeMemory();
    
    //##ModelId=42647B21027B
    EventSection *insert();
    
    //##ModelId=4272EF3A0243
    void bindUserInfo(bool bRebind = false);
    
    void unbindUserInfo();

    void setUserInfo (Serv *pServ = 0x0);

    //##ModelId=4254E43100E2
    EventSection *m_poNext;
    //##ModelId=4266423B0286
    int m_iMeasureBegin;

	//�ͻ����Ի�������Ϣƫ��
	unsigned int m_iCustPlanOffset;
        
  private:
    //##ModelId=4254E80D0396
    static int m_iTotalNumber;

    //##ModelId=4254E92A02BA
    static int m_iUsedNumber;

    //##ModelId=42672FE303C4
    static EventSection *m_poList;

  public:
    //��չ��ʱ����(ģ���ڲ�ʹ��,IDParser��֧��)
    ExtendTempValue  m_oExtTempValue;
    
    //�������������
    int  m_iOutOverTimes;
    
    //������������� 
    int  m_iInOverTimes;
    
    //IPv6�������������
    int  m_iOutOverTimesV6; 
    
    //IPv6�������������
    int  m_iInOverTimesV6; 
    
    //�м̲��Ʒѱ�ʶ
    int  m_iTrunkNotBillFlag;
    
    //(������)���к���
    //<��Ϊ��������, Ҫ������Բ�Ϊ��, ������Ϊ�κ��ص������жϵ�"��ȫƥ��"����>
    //<������ģ��Ĭ�ϻ����� ��������+����> ,���ϰ汾����һ��
    char m_sDupChkCallingNBR[DUP_CHK_MAX_CALLING_LEN]; //Ŀǰ����41λ(����40λ)
    
    //(������)���к���
    //<������ģ��Ĭ�ϻ����� ���к���>, ���ϰ汾����һ��
    char m_sDupChkCalledNBR[DUP_CHK_MAX_CALLED_LEN];   //Ŀǰ����41λ(����40λ)
    
    //���������
    char m_sMixStr[MAX_STRING_VALUE_LEN]; 
    //���������� 
	char m_sSeqKey[DUP_CHK_MAX_SEQKEY_LEN];
	//����ID��
	char m_sSeqIDCode[DUP_CHK_MAX_SEQID_LEN];
    void resetExtAttrValue ()
    {
	    m_iOutOverTimes = 0;
	    m_iInOverTimes = 0;
	    m_iOutOverTimesV6 = 0;
	    m_iInOverTimesV6 = 0;
	    m_iTrunkNotBillFlag = 0;
	    m_sDupChkCallingNBR[0] = 0;
	    m_sDupChkCalledNBR[0] = 0;
	    m_sMixStr[0] = 0;
		m_sSeqKey[0] = 0;
		m_sSeqIDCode[0] = 0;
	   // memset(m_sMixStr,' ',sizeof(m_sMixStr));
	    
	    m_oExtTempValue.reset ();        
	 m_iBillingOrgIDEx_AD = 0;
        m_iBillingOrgIDEx_VS = 0;
        m_iCallingOrgIDEx_AD = 0;
        m_iCallingOrgIDEx_VS =0;
        m_iCalledOrgIDEx_AD =0;
        m_iCalledOrgIDEx_VS =0;
        m_iThirdOrgIDEx_AD =0;
        m_iThirdOrgIDEx_VS =0;
        m_iCalledCarrier = 0; 
	m_iOtherOrgIDEx_AD = 0;
	m_iOtherOrgIDEx_VS =0;
    }
	    //�ƶ��¼�����ʹ��
    int m_iBillingOrgIDEx_AD;
    int m_iBillingOrgIDEx_VS;
    int m_iOtherOrgIDEx_AD;
    int m_iOtherOrgIDEx_VS;
    int m_iCallingOrgIDEx_AD;
    int m_iCallingOrgIDEx_VS;
    int m_iCalledOrgIDEx_AD;
    int m_iCalledOrgIDEx_VS;
    int m_iThirdOrgIDEx_AD;
    int m_iThirdOrgIDEx_VS;
    int m_iCalledCarrier;
};


#define SET_SECTION_VALUE(x, y) \
	*((StdEvent *)(x)) = *((StdEvent *)y)

#endif /* EVENTSECTION_H_HEADER_INCLUDED_BDA5C464 */
