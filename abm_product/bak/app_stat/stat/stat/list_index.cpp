/*VER: 1*/ 
#include <stdio.h>

#if __DOS__
#include <malloc.h>
#include <mem.h>
#endif

#include <string.h>
#include "list_index.h"

/*------------------------
 *	usage:	�����½ڵ�
 *	arguments:	data -- �½ڵ�Ҫ���ݵ�����
 *------------------------*/
static listnode_t * NewNode ()
{
	listnode_t	*newnode = (listnode_t *) malloc(sizeof (listnode_t));
	
	if (newnode)
	{
		newnode->next = NULL;
		newnode->u.data = NULL;
	}
	return newnode;
}


static void freeNode (listnode_t *node)
{
	if(node)
	{
		if(node->u.data){
			free (node->u.data);
			node->u.data=NULL;
		}
		free(node);
		node=NULL;
	}
}

static void freeIndexNode (listnode_t *node)
{
	if(node)
	{
		/*
		if(node->u.data)
			free (node->u.data);
		*/
		free(node);
		node=NULL;
	}
}

static void freeIndexNodeAndData(listnode_t *node,void (* free_op) (void *data) )
{
	if(node)
	{
		
		if(node->u.data)
			free_op (node->u.data);
		
		free(node);
		node=NULL;
	}
}


/*-------------------------------------
 *	usage:	�ͷ�����
 *	arguments:	list -- �����ͷָ��
 *-------------------------------------*/
void FreeList (list_t **list)
{
	listnode_t	*curnode;
	listnode_t	*prenode;
	
	if(*list)
	{
		curnode = (*list)->head; 
		while (curnode != NULL)
		{
			prenode = curnode;
			curnode = curnode->next;
			freeNode (prenode);
		}
		free (*list);
		*list=NULL;
	}
}

/*-------------------------------------
 *	usage:	�ͷ�����
 *	arguments:	list -- �����ͷָ��
 *-------------------------------------*/
void FreeIndexList (list_t **list)
{
	listnode_t	*curnode;
	listnode_t	*prenode;
	
	if(*list)
	{
		curnode = (*list)->head; 
		while (curnode != NULL)
		{
			prenode = curnode;
			curnode = curnode->next;
			freeIndexNode (prenode);
		}
		free (*list);
		*list=NULL;
	}
}

/*-------------------------------------
 *	usage:	�ͷ�����
 *	arguments:	list -- �����ͷָ�������
 *-------------------------------------*/
void FreeIndexListAndData(list_t **list,void (* free_op) (void *data))
{
	listnode_t	*curnode;
	listnode_t	*prenode;
	
	if(*list)
	{
		curnode = (*list)->head; 
		while (curnode != NULL)
		{
			prenode = curnode;
			curnode = curnode->next;
			freeIndexNodeAndData (prenode,free_op);
		}
		free (*list);
		*list=NULL;
	}
}
/*-------------------------------------
 *	usage:	��������������ݺ���
 *	arguments:	list -- �����ͷָ��
 *			data -- �²�������ָ��
 *			n    -- �������ݴ�С
 *			cmp  -- ���ݱȽϺ���ָ��
 *	return:		�ɹ� -- 0
 *			ʧ�� -- -1
 *	comment:�½ڵ��������ͷ
 *-------------------------------------*/
int InsertList (list_t **list, const void *data, size_t n)
{
	listnode_t	*tempnode;
	void		*tempdata;

	if (*list == NULL)
	{
		*list = (list_t *) malloc (sizeof (list_t));
		if (*list == NULL)
			return -1;
		(*list)->head = NULL;
	}

	if ((tempdata = (void *) malloc (n)) == NULL)/* allocate data space*/
		return -1;
	memcpy (tempdata, data, n);

	if ((tempnode = NewNode ()) == NULL)
		return -1;
	tempnode->u.data = tempdata;

	tempnode->next = (*list)->head;
	(*list)->head = tempnode;
		
	return 0;
}

/*-------------------------------------
 *	usage:	��������������ݺ���
 *	arguments:	list -- �����ͷָ��
 *			data -- �²�������ָ��
 *			cmp  -- ���ݱȽϺ���ָ��
 *	return:		�ɹ� -- 0
 *			ʧ�� -- -1
 *	comment:�½ڵ��������ͷ
 *-------------------------------------*/
int InsertIndexList (list_t **list, const void *data)
{
	listnode_t	*tempnode;
	void		*tempdata;

	if (*list == NULL)
	{
		*list = (list_t *) malloc (sizeof (list_t));
		if (*list == NULL)
			return -1;
		(*list)->head = NULL;
	}

	if ((tempnode = NewNode ()) == NULL)
		return -1;
	tempnode->u.data = (void *)data;

	tempnode->next = (*list)->head;
	(*list)->head = tempnode;
		
	return 0;
}
/*-------------------------------------
 *	usage:	��������������ݺ���
 *	arguments:	list -- �����ͷָ��
 *			data -- �²�������ָ��
 *			n    -- �������ݴ�С
 *			cmp  -- ���ݱȽϺ���ָ��
 *	return:		�ɹ� -- 0
 *			ʧ�� -- -1
 *	comment:�½ڵ��������ͷ
 *-------------------------------------*/
int InsertListOrderByKey (list_t **list, const void *data, size_t n,
	int (*cmp) (const void *data1, const void *data2))
{
	listnode_t	*tempnode,**curnode;
	void		*tempdata;

	if (*list == NULL)
	{
		*list = (list_t *) malloc (sizeof (list_t));
		if (*list == NULL)
			return -1;
		(*list)->head = NULL;
	}

	if ((tempdata = (void *) malloc (n)) == NULL)/* allocate data space*/
		return -1;
	memcpy (tempdata, data, n);

	if ((tempnode = NewNode ()) == NULL)
		return -1;
	tempnode->u.data = tempdata;
	tempnode->next = NULL;
	
	for (curnode = &((*list)->head); *curnode != NULL; curnode = &((*curnode)->next) )
	{
		if (cmp ((*curnode)->u.data, data))/* use the function provided by user to do comparation*/
				break;	/*find data we need,return the pointer of the data */
	}

	tempnode->next = (*curnode);
	(*curnode) = tempnode;
		
	return 0;
}

/*-------------------------------------
 *	usage:	��������������ݺ���
 *	arguments:	list -- �����ͷָ��
 *			data -- �²�������ָ��
 *			cmp  -- ���ݱȽϺ���ָ��
 *	return:		�ɹ� -- 0
 *			ʧ�� -- -1
 *	comment:�½ڵ��������ͷ
 *-------------------------------------*/
int InsertIndexListOrderByKey (list_t **list, const void *data,
	int (*cmp) (const void *data1, const void *data2))
{
	listnode_t	*tempnode,**curnode;
	void		*tempdata;

	if (*list == NULL)
	{
		*list = (list_t *) malloc (sizeof (list_t));
		if (*list == NULL)
			return -1;
		(*list)->head = NULL;
	}

	if ((tempnode = NewNode ()) == NULL)
		return -1;
	tempnode->u.data = (void *)data;
	tempnode->next = NULL;
	
	for (curnode = &((*list)->head); *curnode != NULL; curnode = &((*curnode)->next) )
	{
		if (cmp ((*curnode)->u.data, data))/* use the function provided by user to do comparation*/
				break;	/*find data we need,return the pointer of the data */
	}

	tempnode->next = (*curnode);
	(*curnode) = tempnode;
		
	return 0;
}

/*-------------------------------------
 *	usage:	�����ڵ�
 *	arguments:	list -- �����ͷָ��
 *			data -- ���ڱȽϵ�����
 *			cmp  -- ���ڱȽϵĺ�������ͬ����0��
 * 				data1����data2����1��data1С��data2����-1
 *	return:		�ҵ� -- ����ָ���ҵ����ݵ�ָ��
 *			û�ҵ�- NULL
 *-------------------------------------*/
void * SearchList (list_t *list, const void *data,
	int (*cmp) (const void *data1, const void *data2))
{
	listnode_t	*curnode;

	if(list == NULL)
		return NULL;

	for (curnode = list->head; curnode != NULL; curnode = curnode->next)
	{
		if (!cmp (curnode->u.data, data))/* use the function provided by user to do comparation*/
		{
			return curnode->u.data;	/*find data we need,return the pointer of the data */
		}
	}

	return NULL;
}

/*-------------------------------------
 *	usage:	ɾ��������ָ���������ڵĽڵ�
 *	arguments:	list -- �����ͷָ��
 *			data -- ָ��Ҫɾ��������
 *			cmp -- �ȽϺ���ָ��
 *	comment:�����Ϊstderr
 *-------------------------------------*/
void DeleteList (list_t *list, const void *data, 
	int (*cmp) (const void *data1, const void *data2))
{
	listnode_t	**curptr;
	listnode_t	*tempnode;
	
	if (list)
	{	
		curptr = &(list->head);
		
		while (*curptr)
		{
			if (!cmp ((*curptr)->u.data, data))
			{
				tempnode = *curptr;
				*curptr = (*curptr)->next;
				freeNode (tempnode);
			}
			else
				curptr = &((*curptr)->next);
		}
	}
}

/*-------------------------------------
 *	usage:	ɾ��������ָ���������ڵĽڵ�
 *	arguments:	list -- �����ͷָ��
 *			data -- ָ��Ҫɾ��������
 *			cmp -- �ȽϺ���ָ��
 *	comment:�����Ϊstderr
 *-------------------------------------*/
void DeleteIndexList (list_t *list, const void *data, 
	int (*cmp) (const void *data1, const void *data2))
{
	listnode_t	**curptr;
	listnode_t	*tempnode;
	
	if (list)
	{	
		curptr = &(list->head);
		
		while (*curptr)
		{
			if (!cmp ((*curptr)->u.data, data))
			{
				tempnode = *curptr;
				*curptr = (*curptr)->next;
				freeIndexNode (tempnode);
			}
			else
				curptr = &((*curptr)->next);
		}
	}
}

/*-------------------------------------
 *	usage:	��ӡ��������
 *	arguments:	list -- �����ͷָ��
 *			print - ��ӡ����ָ��
 *	comment:�����Ϊstderr
 *-------------------------------------*/
void PrintList (list_t *list, void (*print) (const void *data))
{
	listnode_t	*curnode;
	
	if (list)
	{
		curnode = list->head;
		while (curnode)
		{
			print (curnode->u.data);
			curnode = curnode->next;
		}
		
	}
}

/*------------------------------------------
 *	usage:	�������еĸ��ڵ����ݽ���ָ������
 *	parameters:	list   -- ����ͷָ��
 *			op_data-- ָ���Ĳ�������
 *------------------------------------------*/
void OperateList (list_t *list, void (*op_data) (void *data))
{
	listnode_t	*curnode;
	
	if (list)
	{
		curnode = list->head;
		while (curnode)
		{
			op_data (curnode->u.data);
			curnode = curnode->next;
		}
	}
}


void *GetFirst (list_t *list)
{
	listnode_t	*curnode;
	
	if (list)
	{
		curnode = list->head;
		if (curnode)
			return curnode->u.data;
	}
	
	return NULL;
}


void DeleteFirst (list_t *list)
{
	listnode_t	*cur;
	
	if (list)
	{
		cur = list->head;
		if (cur)
		{
			list->head = cur->next;
			freeNode (cur);
		}
	}
}

void DeleteIndexFirst (list_t *list)
{
	listnode_t	*cur;
	
	if (list)
	{
		cur = list->head;
		if (cur)
		{
			list->head = cur->next;
			freeIndexNode (cur);
		}
	}
}

listnode_t *GetNode (list_t *list)
{
	if (list)
		return list->head;
	else
		return NULL;
}

/*----------------------------------------
 *	�ж������Ƿ�Ϊ�գ��������ݣ�
 *----------------------------------------*/
int ListIsNull (list_t *list)
{
	if (list)
	{
		if (list->head)
			return 0;
	}
	
	return 1;
}


/*-------------------------------------------
 *	usage: ����������ڵ㣬ÿ�ε��ã���˳��������������е�����
 *	parameter:	list   -- ����ͷָ��
 *				  �״ε���ʱ��Ҫ�����������´ε���ʱ
 *				  ��NULL
 *-------------------------------------------*/
void *TravelList (list_t *list)
{
	static listnode_t	*curnode = NULL;

	
	if (list)
	{
		curnode = list->head;
		
		if (curnode)
			return curnode->u.data;
	}
	else
	{
		if (curnode)
		{		
			curnode = curnode->next;
			if (curnode)
				return curnode->u.data;	
		}
	}
	
	return NULL;
	
}

/*-------------------------------------------
 *	usage: ����������ڵ㣬ÿ�ε��ã���˳��������������е�����
 *	parameter:	list   -- ����ͷָ��
 *				  �״ε���ʱ��Ҫ�����������´ε���ʱ
 *				  ��NULL
 *-------------------------------------------*/
void *TravelList_1 (list_t *list)
{
	static listnode_t	*curnode = NULL;

	
	if (list)
	{
		curnode = list->head;
		
		if (curnode)
			return curnode->u.data;
	}
	else
	{
		if (curnode)
		{		
			curnode = curnode->next;
			if (curnode)
				return curnode->u.data;	
		}
	}
	
	return NULL;
	
}



/* old h_list.h */

/*------------------------------------------
 *	usage:	�������еĸ��ڵ����ݽ���ָ������
 *
 *	notice:	�ڵ�������ж�
 *
 *	parameters:	list   -- ����ͷָ��
 *			op_data-- ָ���Ĳ�������
 *------------------------------------------*/
int DealList (list_t *list, int (*op_data) (void *data))
{
	listnode_t	*curnode;
	
	if (list)
	{
		curnode = list->head;
		while (curnode)
		{
			/*
			 *	 �������жϣ�����0
			 */
			if (op_data (curnode->u.data) == 0)
				return 0;
				
			curnode = curnode->next;
		}
	}
	
	return 1;
}


/*-------------------------------------
 *	usage:	�ͷ�����
 *	arguments:	list -- �����ͷָ��
 *-------------------------------------*/
void FreeList (list_t *list)
{
	listnode_t	*curnode;
	listnode_t	*prenode;
	
	if(list)
	{
		curnode = list->head; 
		while (curnode != NULL)
		{
			prenode = curnode;
			curnode = curnode->next;
			freeNode (prenode);
		}
		free (list);
	}
}
