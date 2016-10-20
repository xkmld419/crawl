#include <stdlib.h>
#include <string.h>
#include "ASNObject.h"

int checkDateValid(const char * sDate) {
	if (!sDate) {
		return -1;
	}

	char tempDate[9] = {0};
	strcpy (tempDate, sDate);
	int iDay = atoi (tempDate+6);
	tempDate[6] = 0;
	int iMon = atoi (tempDate+4);
	tempDate[4] = 0;
	int iYear = atoi (tempDate);

	if (iDay > 12 || iDay < 1) {
		return -2;
	}
	if (iMon > 12 || iDay < 1) {
		return -3;
	}
	if (iYear > 9999 || iYear < 1) {
		return -4;
	}

	switch (iMon) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
	{
		if ( 1 > iDay || iDay > 31 ) {
			return -5;
		}
		break;
	}
	case 4:
	case 6:
	case 9:
	case 11:
	{
		if ( 1 > iDay || iDay > 30 ) {
			return -5;
		}
		break;
	}
	case 2:
	{
		iYear = iYear % 400;
		if (!(iYear%4) && (iYear%100)) {
			if ( 1 > iDay || iDay > 29 ) {
				return -5;
			}
		}
		else {
			if ( 1 > iDay || iDay > 28 ) {
				return -5;
			}
		}
		break;
	}
	default:
		return -6;
	}
	
	return 0;
}

int checkDateTimeValid(const char * sDate) {
	if (!sDate) {
		return -1;
	}

	char tempDate[16] = {0};
	strcpy (tempDate, sDate);
	int iSec = atoi (tempDate+12);
	tempDate[12] = 0;
	int iMin = atoi (tempDate+10);
	tempDate[10] = 0;
	int iHour = atoi (tempDate+8);
	tempDate[8] = 0;
	int iDay = atoi (tempDate+6);
	tempDate[6] = 0;
	int iMon = atoi (tempDate+4);
	tempDate[4] = 0;
	int iYear = atoi (tempDate);

	if (iSec > 59 || iSec < 1) {
		return -7;
	}
	if (iMin > 59 || iMin < 1) {
		return -8;
	}
	if (iHour > 24 || iHour < 1) {
		return -9;
	}
	if (iDay > 12 || iDay < 1) {
		return -2;
	}
	if (iMon > 12 || iDay < 1) {
		return -3;
	}
	if (iYear > 9999 || iYear < 1) {
		return -4;
	}

	switch (iMon) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
	{
		if ( 1 > iDay || iDay > 31 ) {
			return -5;
		}
		break;
	}
	case 4:
	case 6:
	case 9:
	case 11:
	{
		if ( 1 > iDay || iDay > 30 ) {
			return -5;
		}
		break;
	}
	case 2:
	{
		iYear = iYear % 400;
		if (!(iYear%4) && (iYear%100)) {
			if ( 1 > iDay || iDay > 29 ) {
				return -5;
			}
		}
		else {
			if ( 1 > iDay || iDay > 28 ) {
				return -5;
			}
		}
		break;
	}
	default:
		return -6;
	}
	
	return 0;
}


/*
* �������ܣ���asnԪ���м���һ���ڵ�
* ���������������ڵ�
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int ASNUTree::UTreeAdd(ASNUTree * node)
{
	if (!node) {										// �����쳣
		printf("ASNUTree::UTreeAdd param err, node is 0x0\n");
		return -1;
	}

	ASNUTree* father = UTreeFind(node->m_usFather);		// ���������в����丸�ڵ�

	if (father) {
		if (isLeaf(father->m_oNode)) {
			return father->childAdd(node);				// ������ڵ�Ŀǰ����Ҷ�ӽڵ�Ļ�����ֱ�Ӽ���
		}
		else {											// �������ҵ����ڸ��ڵ��µ�λ���ٽ��в���
			TList* brother = father->m_oNode.poPrev;
			ASNUTree* finderUTree = listObject(brother, ASNUTree, m_oNode);
			ASNUTree* brotherUTree = listObject(brother, ASNUTree, m_oNode);
			while (brother && brother != &father->m_oNode) {
				finderUTree = listObject(brother, ASNUTree, m_oNode);

				if (!finderUTree) {
					printf("ASNUTree::UTreeAdd momery invalid, finderUTree is 0x0\n");
					return -3;
				}
				else if (node->m_usTag == finderUTree->m_usTag) {
					printf("ASNUTree::UTreeAdd node is repeat, tag is %d\n", node->m_usTag);
					return -4;
				}
				else if (node->m_iSequence < finderUTree->m_iSequence) {
					return brotherUTree->brotherAdd(node);
				}

				brotherUTree = finderUTree;
				brother = brother->poNext;
			}

			if (brother == &father->m_oNode) {
				return brotherUTree->brotherAdd(node);
			}
		}
	}

	printf("ASNUTree::UTreeAdd find the new node father failed!, tag is %d\n", node->m_usTag);
	return -2;											// ���û���ҵ��丸���򷵻�ʧ��
}

/*
* �������ܣ���asnԪ���ҵ�ָ���ڵ�
* ���������������ڵ�
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
ASNUTree* ASNUTree::UTreeFind(TAG_UINT16 tag)
{
	if (tag == m_usTag) {
		return this;
	}

	TList* finder = m_oNode.poPrev;
	ASNUTree* finderUTree = NULL;
	while (finder->poNext && finder != &m_oNode) {
		if (!isLeaf((*finder))) {							// ������ӽڵ���һ��������ݹ����
			finderUTree = listObject(finder, ASNUTree, m_oNode)->UTreeFind(tag);
			if (finderUTree) {
				return finderUTree;
			}
		}
		else {
			finderUTree = listObject(finder, ASNUTree, m_oNode);
			if (finderUTree && tag == finderUTree->m_usTag) {
				return finderUTree;
			}
		}

		finder = finder->poNext;
	}

	return NULL;
}

/*
* �������ܣ��ж���һ������Ľڵ��Ƿ�������Ĺ���
* ���������������ڵ�
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int ASNUTree::UTreeCheckField(ASNTree * node, ASNUTree * &next)
{
	if (!node) {										// �����쳣
		printf("ASNUTree::UTreeCheckField param err, node is 0x0\n");
		return -1;
	}

	if (isRoot(node->m_oNode)) {
		if (this->m_usTag != node->m_usTag) {
			printf("ASNUTree::UTreeCheckField check the root node failed\n");
			return -6;
		}
		next = listObject(this->m_oNode.poPrev, ASNUTree, m_oNode);
		return this->doCheck(node);
	}

	TList * tmp = &this->m_oNode;
	ASNUTree * tmpTree = this;
	while (!isRoot((*tmp))) {	
		tmpTree = listObject(tmp, ASNUTree, m_oNode);
		
		if (tmpTree->m_usTag == node->m_usFather) {
			tmp = tmp->poPrev;
			continue;
		}
		else if (tmpTree->m_usFather != node->m_usFather) {
			
			tmp = tmp->poNext;
			
			/* .�����һ���ڵ㲻���丸�ڵ����ж���һ���ֵܽڵ��Ƿ��ѡ*/
			if (tmpTree->m_usFather != listObject(tmp, ASNUTree, m_oNode)->m_usTag) {	
				tmpTree = listObject(tmp, ASNUTree, m_oNode);
				if (tmpTree->m_iNum != -1 && tmpTree->m_iNum != 0) {
					printf("ASNUTree::UTreeCheckField a must node has not been found, tag is %d\n", tmpTree->m_usTag);
					return -5;							// ��ѡ�ֵ��ֶ�û����д
				}
			}
			continue;
		}

		if (tmpTree->m_usTag == node->m_usTag) {
			next = tmpTree;
			return tmpTree->doCheck(node);			// ���ֶα������ݽ���У��
		}
		else {
			tmp = tmp->poNext;
			TAG_UINT16 father = tmpTree->m_usFather;
			
			/* .�����һ���ڵ����丸�ڵ�����Ϊ�����ֶβ��Ϸ�*/
			while (father != listObject(tmp, ASNUTree, m_oNode)->m_usTag) {	
				tmpTree = listObject(tmp, ASNUTree, m_oNode);
				if (tmpTree->m_usTag != node->m_usTag) {
					if (tmpTree->m_iNum != -1 || tmpTree->m_iNum != 0) {
						printf("ASNUTree::UTreeCheckField a must node has not been found, tag is %d\n", tmpTree->m_usTag);
						return -5;						// ��ѡ�ֵ��ֶ�û����д
					}
				}
				else {
					next = tmpTree;
					return tmpTree->doCheck(node);		// ���ֶα������ݽ���У��
				}
			}
			printf("ASNUTree::UTreeCheckField this tree has not the node, tag is %d\n", node->m_usTag);
			return -6;									// �ֶ�û���ڸ�λ�ñ�����
		}
	}

	printf("ASNUTree::UTreeCheckField find the new node father failed!, tag is %d\n", node->m_usTag);
	return -2;											// û���ҵ��ýڵ�ĸ��ڵ�
}

/*
* �������ܣ��ж��½ڵ��Ƿ���ϱ������ԵĶ���
* ���������������ڵ�
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int ASNUTree::doCheck(ASNTree * node)
{
	if (!node) {
		return -1;
	}

	
	return typeCheck(node->m_usType) || flagCheck(node) || lenCheck(node);
}

