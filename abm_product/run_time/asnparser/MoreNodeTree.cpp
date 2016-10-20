#include "MoreNodeTree.h"

// ���ﶨ��һ����ȱ������ڲ���ʱ��ӡ���ṹ
int currentDepth = 0;

/*
* �������ܣ��ڱ��ڵ�������һ�����ӽڵ�
* ���������������ĺ��ӽڵ�
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int CMoreNodeTree::childAdd(TList* child)
{
	if (!child) {
		printf("can not insert a null of child!\n");
		return -1;
	}

	if (isMySelf(child)) {
		printf("node can not inert heself as a child!\n");
		return -1;
	}

	/* ����ڵ㲻���ں��ӵĻ���ֱ�Ӳ��뵽�ڵ�����*/
	if (isLeaf(m_oNode)) {
		m_oNode.poPrev = child;
		child->poNext = &m_oNode;
		return 0;
	}

	/* ���ȱ��������һ�����ӽڵ�*/
	TList *latestChild = m_oNode.poPrev;
	findLatestChild((&m_oNode), latestChild);

	/* ���û���ҵ����һ�����ӽڵ���˳��򷵻��쳣*/
	if (!latestChild->poNext) {
		printf("find the latest child error!\n");
		return -2;
	}

	/* ���½ڵ�������һ�����ӽڵ�֮��*/
	child->poNext = latestChild->poNext;
	latestChild->poNext = child;
	return 0;
}


/*
* �������ܣ��ڱ��ڵ�������һ���ֵܽڵ�
* �����������������ֵܽڵ�
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int CMoreNodeTree::brotherAdd(TList* brother)
{
	if (!brother) {
		printf("can not insert a null of child!\n");
		return -1;
	}

	if (isMySelf(brother)) {
		printf("node can not inert heself as a brother node!\n");
		return -1;
	}

	/* ���ﲻ��֤���ڵ�û���ֵܽڵ����� */
	brother->poNext = m_oNode.poNext;
	m_oNode.poNext = brother;
	
	return 0;
}


/*
* �������ܣ�������������ڵ�
* �����������
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int CMoreNodeTree::fetchNode()
{
	currentDepth++;
	TList* node = m_oNode.poPrev;
	int iRet = 0;
	
	iRet = this->process(); 							// �������ڵ����Ƚ��д�ӡ
	if (iRet) {
		currentDepth = 0;
		return iRet;
	}
	while (node->poNext && node != &m_oNode) {
		if (!isLeaf((*node))) {							// ������ӽڵ���һ��������ݹ����
			iRet = listEntry(node)->fetchNode();
			if (iRet) {
				currentDepth = 0;
				return iRet;
			}
		}
		else {
			iRet = listEntry(node)->process(); 			// ������Ҷ�ӽڵ�����д�ӡ
			if (iRet) {
				currentDepth = 0;
				return iRet;
			}
		}
		node = node->poNext;							// ������һ���ֵܽڵ�ı���
	}
	currentDepth--;
	return 0;
}

/*
* �������ܣ��Խڵ㱾���������������������ӽڵ�!
* �����������
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int CMoreNodeTree::process()
{
	for (int i = 1; i < currentDepth; i++) {
		printf("\t");
	}

	if (isLeaf(m_oNode)) {
		printf("  ..");
	}
	else {
		printf("+");
	}

	int iRet = this->printKey();
	if (iRet) {
		return iRet;
	}

	printf("\n");
	
	return 0;
}

/*
* �������ܣ���ӡ�ڵ�key���˴����ڲ���
* �����������
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int CMoreNodeTree::printKey()
{
	return 0;
}
/*
* �������ܣ��ڱ��ڵ����޳�ָ�����ӽڵ�
* �����������ɾ���ĺ��ӽڵ�
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int CMoreNodeTree::childDrop(TList* child)
{
	if (!child) {
		printf("can not delete a null of child!\n");
		return -1;
	}

	if (isMySelf(child)) {
		printf("node can not delete heself as a child!\n");
		return -2;
	}

	/* ����ڵ㲻���ں��ӵĻ��򷵻�ɾ��ʧ��*/
	if (isLeaf(m_oNode)) {
		return -3;
	}

	/* �������ӽڵ㣬�ҵ���ָ���ڵ�������޳�*/
	for (TList* node = m_oNode.poPrev; \
		node->poNext && node != &m_oNode; \
		node = node->poNext) {

		if (node->poNext == child) {
			node->poNext = child->poNext;
			return 0;
		}
	}

	return -3;	//���û���ҵ��򷵻ش���
}

/*
* �������ܣ��ͷ��������м�����Դ
* �����������
* �����������
* ����ֵ  ��void
*/
void CMoreNodeTree::destory()
{
	if (isLeaf(m_oNode)) {
		delete this;
		return;
	}

	TList* tmp = NULL;
	TList* node = m_oNode.poPrev;
	m_oNode.poPrev = &m_oNode;							// ���ýڵ��µ������Ͽ�,ʹ֮��ΪҶ�ӽڵ�
	while (node->poNext && node != &m_oNode) {			// �������������destory
		tmp = node->poNext;
		listEntry(node)->destory();
		node = tmp;
	}

	return;
}

/*
* �������ܣ�����nodeλ���ҵ��������
* �����������
* �����������
* ����ֵ  ��void
*/
CMoreNodeTree* CMoreNodeTree::listEntry(TList* node)
{
	return listObject(node, CMoreNodeTree, m_oNode);
}

