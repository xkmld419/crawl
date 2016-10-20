#include "ASNTree.h"
#include <string.h>
#include <unistd.h>

/*
* �������ܣ��ж������Ƿ���ֽ���
* �����������
* �����������
* ����ֵ  ��1-�߶��ֽ���, 0-�Ͷ��ֽ���
*/
int isBigEndian()
{
	union {
		short 		n;
		char		s[sizeof(short)];
	} un;

	un.n 	= 0x0102;
	if (0x01 == un.s[0] && 0x02 == un.s[1]) {
		return 1;
	}

	return 0;
}

/*
* �������ܣ����ı���ʽ��ӡ����Ҷ�ӽڵ�
* �����������
* �����������
* ����ֵ  ��0�ɹ�, 1ʧ��
*/
int printRecord(ASNTree* node, void * data)
{
	if (!node) {
		return -1;
	}
	
	if (1 == node->m_usFlag) {								// �ṹ���Ͳ���ӡ
		return 0;
	}

	switch(node->m_usFlag) {
	case ASN_FLAG_LONG : {
		printf("%l", node->asLong());
		break;
	}
	case ASN_FLAG_INT : {
		printf("%d", node->asInt());
		break;
	}
	case ASN_FLAG_STRING :
	case ASN_FLAG_DATE :
	case ASN_FLAG_CHOICE : {
		printf("%s", node->asString());
		break;
	}
	default: {
		node->traceValues();		
		return -1;
	}
	}

	if (node->m_usFather == node->listNext()->m_usTag) {
		printf("\n");
	}
	else {
		printf("|");
	}

	return 0;
}

/*
* �������ܣ����ı���ʽ��ӡ����Ҷ�ӽڵ㵽�ļ���
* �����������
* �����������
* ����ֵ  ��0�ɹ�, 1ʧ��
*/
int printRecordToFile(ASNTree* node, void * data)
{
	if (!node || !data) {
		return -1;
	}
	
	if (1 == node->m_usFlag) {								// �ṹ���Ͳ���ӡ
		return 0;
	}

	int * fd = static_cast<int*>(data);
	if (!fd || !(*fd)) {
		return -2;											// �ļ����ָ��λ�գ����߾��idλ0�򷵻��쳣
	}
	static char sData[128] = { 0 };
	static char* pData = NULL;
	
	switch(node->m_usFlag) {
	case ASN_FLAG_LONG : {
		snprintf(sData, 128, "%l", node->asLong());
		write(*fd, sData, strlen(sData));
		break;
	}
	case ASN_FLAG_INT : {
		snprintf(sData, 128, "%d", node->asInt());
		write(*fd, sData, strlen(sData));
		break;
	}
	case ASN_FLAG_STRING :
	case ASN_FLAG_DATE :
	case ASN_FLAG_CHOICE : {
		write(*fd, node->asString(), node->m_uiLen);
		break;
	}
	default: {
		node->traceValues();
		return -1;
	}
	}

	if (node->m_usFather == node->listNext()->m_usTag) {
		write(*fd, "\n", 1);
	}
	else {
		write(*fd, "|", 1);
	}

	return 0;
}


/*
* �������ܣ��Խڵ㱾���������������������ӽڵ�
* �����������
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int ASNTree::process()
{
	return CMoreNodeTree::process();
}

/*
* �������ܣ�������������ڵ�
* �����������
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int ASNTree::fetchProcess(PROCESS fn, void * data)
{
	TList* node = m_oNode.poPrev;
	int iRet = 0;
	
	iRet = fn(this, data); 									// �������ڵ����Ƚ��д�ӡ
	if (iRet) {
		printf("ASNTree::fetchNode call fn failed, this tag is %d\n", this->m_usTag);
		return iRet;
	}
	while (node->poNext && node != &m_oNode) {
		if (!isLeaf((*node))) {							// ������ӽڵ���һ��������ݹ����
			iRet = listObject(node,ASNTree,m_oNode)->fetchProcess(fn, data);
			if (iRet) {
				return iRet;
			}
		}
		else {
			iRet = fn(listObject(node,ASNTree,m_oNode), data); 				// ������Ҷ�ӽڵ�����д�ӡ
			if (iRet) {
				printf("ASNTree::fetchNode call fn failed, the tag is %d\n", listObject(node,ASNTree,m_oNode)->m_usTag);
				return iRet;
			}
		}
		node = node->poNext;							// ������һ���ֵܽڵ�ı���
	}
	return 0;
}

/*
* �������ܣ���ӡ�ڵ�key���˴����ڲ���
* �����������
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int ASNTree::printKey()
{	
	printf("%d( %04d ) =", m_usTag, m_uiLen);				// ��tag(len):values��ʽ��ӡ����,���ڲ���
	traceValues();
	return 0;
}

/* asnЭ��������͵Ľ��뺯�� */
int ASNInt::decode()
{
    if (NULL == m_sValues || 0 == m_uiLen || m_uiLen > sizeof(int)) {
        return DEFAULTINT;
    }

    char *p = const_cast<char*>(m_sValues);

    while(m_uiLen--) {									// �˴���int���ɸߵ����ֽ�����н���
        m_iData *= 1 << 8;
        m_iData += 0x000000ff&((unsigned int )(*p));
        p++;
    }
    m_usValid = 1;

    return m_iData;
	
}

/* asnЭ��������͵Ľ��뺯�� */
long ASNLong::decode()
{
    if (NULL == m_sValues || 0 == m_uiLen || m_uiLen > sizeof(long)) {
        return DEFAULTLONG;
    }

    int bytes = 1;
    char *p = const_cast<char*>(m_sValues);
    while(m_uiLen--) {									// �˴���int���ɸߵ����ֽ�����н���
        m_lData *= bytes;
        m_lData += 0x000000ff&((unsigned int )(*p));
        bytes = 1 << 8;
        p++;
    }
    m_usValid = 1;

    return m_lData;
}

/* asnЭ��������͵Ľ��뺯�� */
const char* ASNString::decode()
{
    m_usValid = 1;
	m_sData = m_sValues;
	
    return m_sData;
}


