/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�MoreNodeTree.h 
* ժ    Ҫ���ļ�������һ����ӵ�ж��ӽڵ����״�ṹ
* 
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��10��20��
*/
#ifndef __MORE_NODE_TREE_H_
#define __MORE_NODE_TREE_H_

#include <stdio.h>


#ifndef NULL
#define NULL 0
#endif

#define listself(node) { \
		node->poNext = node; \
		node->poPrev = node; \
	}
// �жϽڵ��Ƿ���Ǳ��½ڵ�
#define isMySelf(node) (node == &this->m_oNode)
// �жϽڵ��Ƿ�Ҷ�ӽڵ�
#define isLeaf(node) (node.poPrev == &node)
// �жϽڵ��Ƿ�root�ڵ�
#define isRoot(node) (node.poNext == &node)
// �������һ�����ӽڵ�
#define findLatestChild(node, latestChild) { \
	for(latestChild = node->poPrev; latestChild->poNext && latestChild->poNext != node; latestChild = latestChild->poNext) { } \
	}
#define listObject(ptr, type, member) \
	((type*)((char*)ptr - (unsigned long)(&((type*)0)->member)))

/*�������Ӹ��ڵ��ָ����*/
struct TList {
	TList *poNext;	// 
	TList *poPrev;	// 
	
	TList() {
		poNext = NULL;
		poPrev = NULL;
	}
};

class CMoreNodeTree {								//�������һ����ڵ����״�ṹ
public:
	CMoreNodeTree() {
		listself((&m_oNode));
	}

	virtual ~CMoreNodeTree() {
		if (!isLeaf(m_oNode)) {
			destory();
		}
	}

	/*
	* �������ܣ��ڱ��ڵ�������һ�����ӽڵ�
	* ���������������ĺ��ӽڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int childAdd(CMoreNodeTree* child) {
		return child == NULL ? -1 : childAdd(&child->m_oNode);
	}

	/*
	* �������ܣ��ڱ��ڵ�������һ���ֵܽڵ�
	* �����������������ֵܽڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int brotherAdd(CMoreNodeTree* brother) {
		return brother == NULL ? -1 : brotherAdd(&brother->m_oNode);
	}

	
	/*
	* �������ܣ��ڱ��ڵ���ɾ��ָ�����ӽڵ�
	* �����������ɾ���ĺ��ӽڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int childDrop(CMoreNodeTree* child) {
		return child == NULL ? -1 : childDrop(&child->m_oNode);
	}

	/*
	* �������ܣ�������������ڵ�
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int fetchNode();

protected:
	/*
	* �������ܣ��ڱ��ڵ�������һ�����ӽڵ�
	* ���������������ĺ��ӽڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int childAdd(TList* child);

	
	/*
	* �������ܣ��ڱ��ڵ�������һ���ֵܽڵ�
	* �����������������ֵܽڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int brotherAdd(TList* brother);

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
	* �������ܣ��ڱ��ڵ���ɾ��ָ�����ӽڵ�
	* �����������ɾ���ĺ��ӽڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int childDrop(TList* child);

	/*
	* �������ܣ��ͷ��������м�����Դ
	* �����������
	* �����������
	* ����ֵ  ��void
	*/
	virtual void destory();
	
	/*
	* �������ܣ�����nodeλ���ҵ��������
	* ���������node�ڵ�λ��
	* �����������
	* ����ֵ  ��void
	*/
	virtual CMoreNodeTree* listEntry(TList* node);
public:
	TList	m_oNode;		/*��򵥵���ֻ��һ��ָ��������ɽڵ�֮������Ӽ���*/
};

#endif

