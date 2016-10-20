/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�ASNObject.h 
* ժ    Ҫ���ļ�����������У��asn�����ʽ��Ԫ������
* 
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��10��21��
*/
#ifndef __ASN_OBJECT_
#define __ASN_OBJECT_

#include "ASNTree.h"


int checkDateValid(const char * sDate);
int	checkDateTimeValid(const char * sDate);



/* ************** ASNԪ��¼�ͳ�����************ */
/* ******* ���ڴ��Ԥ���õ������ֵ�******* */
class ASNUTree : public ASNTree {
public:
	ASNUTree() {
		m_uiMaxLen 	= 0;
		m_iNum 		= 0;
		m_iSequence	= 0;
		m_uiMinLen	= 0;
		m_iCurNum	= 0;
	}

	virtual ~ASNUTree() {
		
	}

	/*
	* �������ܣ���asnԪ���м���һ���ڵ�
	* ���������������ڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int UTreeAdd(ASNUTree * node);

	/*
	* �������ܣ���asnԪ���ҵ�ָ���ڵ�
	* ���������������ڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual ASNUTree* UTreeFind(TAG_UINT16 tag);

	/*
	* �������ܣ��ж���һ������Ľڵ��Ƿ�������Ĺ���
	* ���������������ڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int UTreeCheckField(ASNTree * node, ASNUTree * &next);

	
	/*
	* �������ܣ��ж��½ڵ��Ƿ���ϱ������ԵĶ���
	* ���������������ڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int doCheck(ASNTree * node);

private:
	int typeCheck(const unsigned short type) {
		return 0;//m_usType == type ? 0 : -1;
	}
	int flagCheck(ASNTree * node) {
		if (1 == m_usFlag) {
			return 1 == node->m_usFlag? 0 : -1;
		}
		else {
			if (1 == node->m_usFlag) {
				printf("ASNUTree::flagCheck the m_usFlag of utree is 0 , but this is 1, node tag is %d\n", this->m_usTag);
				return -1;
			}

			node->m_usFlag = m_usFlag;
		}

		int iRet = 0;
		switch (m_usFlag) {
		case ASN_FLAG_INT : {
			if (node->m_uiLen > 4 || 0 >= node->m_uiLen) {
				printf("ASNUTree::flagCheck the node(int) len invalid, node len is %d, node tag is %d\n", node->m_uiLen, this->m_usTag);
				iRet = -1;
			}
			break;
		}
		case ASN_FLAG_LONG : {
			if (node->m_uiLen > 8 || 0 >= node->m_uiLen) {
				printf("ASNUTree::flagCheck the node(long) len invalid, node len is %d, node tag is %d\n", node->m_uiLen, this->m_usTag);
				iRet = -1;
			}
			break;
		}
		case ASN_FLAG_STRING : {
			
			break;
		}
		case ASN_FLAG_DATE : {
			if (node->m_uiLen != 8) {
				printf("ASNUTree::flagCheck the node(date) len invalid, node len is %d, node tag is %d\n", node->m_uiLen, this->m_usTag);
				iRet = checkDateValid(node->asString());
			}
			break;
		}
		case ASN_FLAG_CHOICE : {
			
			break;
		}
		case ASN_FLAG_DATETIME: {
			if (node->m_uiLen != 14) {
				printf("ASNUTree::flagCheck the node(datetime) len invalid, node len is %d, node tag is %d\n", node->m_uiLen, this->m_usTag);
				iRet = checkDateTimeValid(node->asString());
			}
			break;
		}
		default: {
		
			break;
		}
		}

		return iRet;
	}
	int lenCheck(ASNTree * node) {
		if (!m_uiMinLen && !m_uiMaxLen) {
			return 0;
		}

		if (node->m_uiLen < m_uiMinLen || node->m_uiLen > m_uiMaxLen) {
			printf("ASNUTree::lenCheck the node len invalid, node len is %d, node tag is %d\n", node->m_uiLen, this->m_usTag);
			return -1;
		}
		return  0;
	}
	
public:
	unsigned int 		m_uiMaxLen;
	unsigned int 		m_uiMinLen;
	int					m_iNum;
	int					m_iSequence;
	int 				m_iCurNum;
};




/* ************** ASN�ļ���¼�ͳ�����************ */
class ASNRecord : public ASNTree {
public:
	ASNRecord() {
		// current = m_poNode;
	}

	virtual ~ASNRecord() {
		current = NULL;
	}

	/*
	* �������ܣ��ڱ��ڵ�������һ�����ӽڵ�
	* ���������������ĺ��ӽڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	/*
	virtual int childAdd(CMoreNodeTree* child) {
		int iRet = CMoreNodeTree::childAdd(child);

		if (!iRet) {
			current = child->m_poNode;				// �������нڵ����������current
		}

		return iRet;
	}
	*/
	/*
	* �������ܣ��ڱ��ڵ�������һ���ֵܽڵ�
	* �����������������ֵܽڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	/*
	virtual int brotherAdd(CMoreNodeTree* brother) {
		int iRet = CMoreNodeTree::brotherAdd(brother);

		if (!iRet) {
			current = brother->m_poNode;			// �������нڵ����������current
		}

		return iRet;
	}
	*/
public:
	TList*	current;		// ����һ����ǰ����Ľڵ�ָ�룬��ָ��m_poNode�µ��κ�һ���ڵ�
};




/* ************** ASN��Ϣ��¼�ͳ�����************ */
class ASNMessage : public ASNTree {
public:
	ASNMessage() {

	}

	virtual ~ASNMessage() {

	}
};


#endif

