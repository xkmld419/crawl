/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�ASNTree.h 
* ժ    Ҫ���ļ�������һ��ASN.1��ʽ���������ڳ���asnЭ�����������
*	���ڵ�ΪҶ�ӽڵ�ʱ:
*		list.nextָ�����ͬһ���ڵ���һ���ֵܽڵ�
*		���һ���ֵܽڵ�ָ�򸸽ڵ�;list.prevָ�򸸽ڵ�
*	���ڵ�Ϊ��Ҷ�ӽڵ�ʱ:
*		list.prev��ָ���ӽڵ������߽ڵ�
*		list.nextָ��ͬһ���ڵ��µ���һ���ֵܽڵ�
*		���һ���ֵܽڵ�nextָ�򸸽ڵ�
* 		root�ڵ�nextָ��������û�о����ֵ
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��10��20��
*/
#ifndef __ASN_TREE_H_
#define __ASN_TREE_H_

#include <stdio.h>
#include "TLV.h"
#include "MoreNodeTree.h"

/* ************** ���Ͷ���************ */


/* ************** ��������************ */
#define		ASN_FLAG_STRUCT				1				// �ṹ����
#define		ASN_FLAG_UNION				0				// ԭ��ʽ
#define 	ASN_FLAG_INT				2				// INT ��
#define 	ASN_FLAG_STRING				3				// string ��
#define 	ASN_FLAG_LONG				4				// LONG ��
#define 	ASN_FLAG_DATE				5				// ʱ�� ��<yyyymmdd>
#define 	ASN_FLAG_CHOICE				6				// ö������
#define 	ASN_FLAG_DATETIME			7				// ʱ�� ��<yyyymmddHH24MISS>

#define		ASN_TYPE_UNIVERSAL			0				// ͨ������
#define		ASN_TYPE_APPLICATION		1				// Ӧ�ó���������ͽڵ�
#define		ASN_TYPE_CONTEXTSPECIFIC 	2				// ������������ͽڵ�
#define		ASN_TYPE_PRIVATE			3				// ��asn�ṹ˽�����ͽڵ�

class ASNTree;

typedef int (*PROCESS)(ASNTree* node, void * data);	// ���ڱ�����ʱ����ָ���Ĵ�����


/*
* �������ܣ��ж������Ƿ���ֽ���
* �����������
* �����������
* ����ֵ  ��1-�߶��ֽ���, 0-�Ͷ��ֽ���
*/
int isBigEndian();

/*
* �������ܣ����ı���ʽ��ӡ����Ҷ�ӽڵ�
* �����������
* �����������
* ����ֵ  ��0�ɹ�, 1ʧ��
*/
int printRecord(ASNTree* node, void * data);

/*
* �������ܣ����ı���ʽ��ӡ����Ҷ�ӽڵ㵽�ļ���
* �����������
* �����������
* ����ֵ  ��0�ɹ�, 1ʧ��
*/
int printRecordToFile(ASNTree* node, void * data);


/* ************** ASN���ݻ���************ */
class ASNTree : public CMoreNodeTree, public TLV {
public:
	ASNTree() {
		m_usFlag = ASN_FLAG_UNION;
		m_usType = ASN_TYPE_UNIVERSAL;
		m_v.iData = 0;
	}

	virtual ~ASNTree() {
		m_usFather = 0;
	}
	
	/*
	* �������ܣ��ڱ��ڵ�������һ�����ӽڵ�
	* ���������������ĺ��ӽڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int childAdd(ASNTree* child) {
		child->m_usFather = this->m_usTag;
		return CMoreNodeTree::childAdd(child);
	}
	
	virtual int asInt() {
		if (m_usValid) {
			return m_v.iData;
		}
		else {
			return deInt();
		}
	}

	virtual long asLong() {
		if (m_usValid) {
			return m_v.lData;
		}
		else {
			return deLong();
		}
	}

	virtual const char* asString() {
		if (m_usValid) {
			return m_v.sData;
		}
		else {
			return deString();
		}
	}

	/*
	* �������ܣ�������������ڵ�
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int fetchProcess(PROCESS fn, void * data);

	/*
	* �������ܣ�������������ڵ�
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual ASNTree* listNext() {
		return listObject(m_oNode.poNext,ASNTree,m_oNode);
	}
	
	/*
	* �������ܣ���16���ƽ�values��ӡ����
	* �����������
	* �����������
	* ����ֵ  ��void
	*/
	virtual void traceValues() {
		if (1 == m_usFlag) {
			return;
		}
		char *p = m_sValues;
		for (int i = 0; i < m_uiLen; i++, p++) {
			printf(" %02x", (*p) & 0x000000FF);
		}
		p = NULL;
		return ;
	}
protected:
	
	/*
	* �������ܣ��Խڵ㱾���������������������ӽڵ�
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int process();

	/*
	* �������ܣ���ӡ�ڵ�key���˴����ڲ���
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int printKey();

	
	/*
	* �������ܣ�����nodeλ���ҵ��������
	* �����������
	* �����������
	* ����ֵ  ��void
	*/
	virtual CMoreNodeTree* listEntry(TList* node) {
		return listObject(node,ASNTree,m_oNode);
	}

	virtual int deInt() {
		if (NULL == m_sValues 
			|| m_usFlag != ASN_FLAG_INT) {
			return DEFAULTINT;
		}
		
		char *p = const_cast<char*>(m_sValues);
		m_v.iData = 0;
		while(m_uiLen--) {									// �˴���int���ɸߵ����ֽ�����н���
			m_v.iData *= 1 << 8;
			m_v.iData += 0x000000ff&((unsigned int )(*p));
			p++;
		}
		m_usValid = 1;
		
		return m_v.iData;
	}
	
	virtual long deLong() {
		if (NULL == m_sValues 
			|| m_usFlag != ASN_FLAG_LONG) {
			return DEFAULTINT;
		}
		
		char *p = const_cast<char*>(m_sValues);
		m_v.lData = 0;
		while(m_uiLen--) {									// �˴���int���ɸߵ����ֽ�����н���
			m_v.lData *= 1 << 8;
			m_v.lData += 0x000000ff&((unsigned int )(*p));
			p++;
		}
		m_usValid = 1;
		
		return m_v.lData;
	}
	
	virtual char* deString() {
		if (NULL == m_sValues || (
			m_usFlag != ASN_FLAG_STRING
			&& m_usFlag != ASN_FLAG_DATE
			&& m_usFlag != ASN_FLAG_CHOICE)) {
			return NULL;
		}
		m_usValid = 1;
		
		m_v.sData = m_sValues;

		return m_v.sData;
	}
public:
	unsigned short 		m_usFlag;
	unsigned short		m_usType;
	TAG_UINT16			m_usFather;

	

	union tlv_v {
		int 	iData;
		long	lData;
		char*	sData;
	}					m_v;
};




/* ************** ASN������������INT************ */
class ASNInt : public ASNTree {
public:
	ASNInt() {
		m_iData 	= DEFAULTINT;
		m_bParse 	= false;
	}

	virtual ~ASNInt() {

	}

	virtual int asInt() {
		return m_bParse ? m_iData : decode();
	}
private:
	virtual int decode();
private:
	int			m_iData;
	bool		m_bParse;
};




/* ************** ASN������������LONG************ */
class ASNLong : public ASNTree {
public:
	ASNLong() {
		m_lData 	= DEFAULTLONG;
		m_bParse 	= false;
	}

	virtual ~ASNLong() {

	}

	virtual long asLong() {
		return m_bParse ? m_lData : decode();
	}
private:
	virtual long decode();
private:
	long		m_lData;
	bool		m_bParse;
};




/* ********* ASN�����������Ͷ���string******** */
class ASNString : public ASNTree {
public:
	ASNString() {
		m_sData 	= NULL;
		m_bParse 	= false;
	}

	virtual ~ASNString() {
		if (NULL != m_sData) {
			delete m_sData;
			m_sData = NULL;
		}
	}

	virtual const char* asString() {
		return m_bParse ? m_sData : decode();
	}
private:
	virtual const char* decode();
private:
	char*		m_sData;
	bool		m_bParse;
};

#endif

