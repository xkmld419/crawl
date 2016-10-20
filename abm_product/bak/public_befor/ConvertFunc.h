/*VER: 1*/ 
#ifndef _CONVERT_FUNC_H__
#define _CONVERT_FUNC_H__

#include "TicketTlv.h"
#include "Brtp.h"
#include "StdEvent.h"
#include "IDParser.h"


class ParseRule {

  public:
    int m_iTag;
    
    int m_iBeginPos;
    
    int m_iDataLen;
    
    int m_iFuncID;
    
    int m_iAttrID; //StdEvent���¼�����ID
    
    int m_iBitPosition; //bit λλ��
    
    ParseRule *m_pNext;
	
#ifndef PRIVATE_MEMORY
	int m_iParseGroupID;
	unsigned int m_iNextOffset;
#endif

  public:
#ifndef PRIVATE_MEMORY
    ParseRule () 
    {
        m_iTag=0; m_iBeginPos=0; m_iDataLen=0; m_iFuncID=0; m_iAttrID=0; 
		m_iParseGroupID=0; m_pNext=0; m_iNextOffset=0;
    };
#else
	ParseRule () 
    {
        m_iTag=0; m_iBeginPos=0; m_iDataLen=0; m_iFuncID=0; m_iAttrID=0; 
		m_pNext=0;
};
#endif
};





class ParseRuleMgr {

  public:

	#ifdef PRIVATE_MEMORY
    ParseRuleMgr(int iProcessID) 
    {
        m_iProcessID = iProcessID;
        initRule ();
    }
	#else
    ParseRuleMgr(int iProcessID) 
    {
        m_iProcessID = iProcessID;
    }
	#endif

	
	#ifdef PRIVATE_MEMORY
    ~ParseRuleMgr() {destoryRule ();}
	#else
	~ParseRuleMgr() {}
	#endif
    
    void initRule ();
    
    void destoryRule ();
    
    // ����Tagλ, ������Ӧtagλ��Ӧ�Ľ�������ͷָ��
    ParseRule *getParseRule (int _iTag);
    
	#ifndef PRIVATE_MEMORY
	ParseRule *getHeadParseRule();
	#endif
  
  private:
    int m_iProcessID;


    ParseRule *m_pParseRule[MAX_TLV_FLAGS+1];


};





class ParseComb
{
	public:
		ParseComb();

		int m_iParseGroupID;
		int m_iProcessID;
		unsigned int m_iParseGroupOffset;
};

class ConvertMgr {

  public:
    static void  convTLV (int iTlvFlag, int iTlvLen, BIT8 *src, ParseRuleMgr *pRuleMgr,
                                TicketTlv *pTlv, StdEvent *pStd);
    
    static void  convert (BIT8 *src, Value *pValue, int _iFuncID, int _iLen, int _iBitPos);
    
  public:    
    // ��bin��ʽ�ֶ�ת����ΪASC, ת��Ϊ��Ӧ�ַ������
    static void  binToAscii (BIT8 * src,char * dest,int length);
        

    // �� BCD ��ʽת���� long
    static long   bcdToLong (BIT8 *src, int length);
    
    // �� BIN ��ʽת���� long
    static unsigned long   binToLong (BIT8 *src, int length);


    // �� BIN ��ʽת���� long
    static  long   binToLong2 (BIT8 *src, int length);
        
    // ��bcd��ʽת��ΪAscii��ʽ(�����)
    static void  bcdToAscii_Left_D (BIT8 * src,BIT8 * dest, int length);
    
    // ��bcd��ʽת��ΪAscii��ʽ(�����, ����������ַ�)
    static void  bcdToAscii_Left_X (BIT8 * src,BIT8 * dest, int length);
    
    static int   getBitValue (unsigned char cValue, int _iBitPos);
    
    static long  asciiToLong (BIT8 *src, int length);
    
    // ���޷��ŵ���ֵ��ֱ��ת�����ַ��� ����ֵ 123 ת�����ַ��� "123"
    static void  longToString (BIT8 * src,BIT8 * dest, int length);

};

#endif

