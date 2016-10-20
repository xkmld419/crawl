/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�TLV.h 
* ժ    Ҫ���ļ�������TLV��ʽ���ݽṹ
* 
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��10��20��
*/
#ifndef __TLV_H_
#define __TLV_H_

/* ************** ���Ͷ���************ */
typedef unsigned short	valid_UINT16; 
typedef unsigned short 	TAG_UINT16;
typedef unsigned int 	LEN_UINT32;
typedef char *		 	VALUES_CHARPTR;




/* ************** ��������************ */
#define INTBITS			(sizeof(int) * 8)
#define LONGBITS		(sizeof(long) * 8)
#define DEFAULTINT  	((1 << (INTBITS - 1)) - 1)
#define DEFAULTLONG  	((1 << (LONGBITS - 1)) - 1)	
#ifndef NULL
#define NULL 0
#endif




/* ************** TLV���Ͷ���************ */
/* TLV�ඨ����tlv���͵����������* */
/* �ṹ��������һ��valid������ʾ** */
/* �ֶ��Ƿ���ʵ��Ч                             *** */
class TLV {
public:
	TLV()	{
		m_usTag 		= 0;
		m_uiLen 		= 0;
		m_sValues 		= NULL;
	}

	virtual ~TLV() {
		if (NULL != m_sValues) {
			delete m_sValues;
			m_sValues = NULL;
		}
	}

	virtual valid_UINT16 isValid() {					// �ж��ֶ��Ƿ���Ч,1:��Ч,0:��Ч
		return m_usValid;
	}

	virtual int asInt() {								// ������32λ�������ֵ����ΪĬ��ֵ
		return DEFAULTINT;								//�������п��Ը���values�������ֵ���з���
	}

	virtual long asLong() {							// �����ó��������ֵ����ΪĬ��ֵ
		return DEFAULTLONG;								//�������п��Ը���values�������ֵ���з���
	}

	virtual const char* asString() {
		return m_sValues;
	}
public:
	valid_UINT16		m_usValid;
	TAG_UINT16			m_usTag;
	LEN_UINT32  		m_uiLen;
	VALUES_CHARPTR		m_sValues;
};

#endif

