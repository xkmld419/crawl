/*VER: 1*/ 
/********************************************************************************/
/*	�������ƣ�������������                                                      */
/*	�����ܣ�ʵ�ּ����Ĳ��룬���ң�����                                    */
/*	����˵����1��ʹ���ַ�����Ϊ��ֵ���ַ����е��ַ���ͨ����ֵת��           */
/*		ת������ֵ��ȡ�õ���ֵ��Ӧ�����²��֧��                        */ 
/*		  2��������Ϊʵʱ�����з���Ŀ��������ά����Ŀ���ݡ�           */
/*		������Ŀ����Ҫ���������ݿ�ͬ����ʹ�����·���ʵ��֮��            */
/*		    ���Ľڵ㶨��һ��subchanged�����һ��listchanged��ʶ��       */
/*		    subchanged����ָʾ��ǰ�ڵ����Щ�����������Ƿ�����        */
/*		    �仯��                                                      */
/*		    listchaned����ָʾ��ǰ�ڵ�ָ�������������Ƿ�����        */
/*		    �仯��                                                      */
/*		    �����²����������ݲ�����Ҫ�������Ľڵ��subchanged        */
/*		    ��listchanged����Ӧ�޸�                                     */
/********************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "trie_list_index.h"

static char keyarray[256];

/*-------------------------------
 *	usage:	��ʼ����ֵ����
 *	comment:��charӳ���0-9����ֵ
 *-------------------------------*/
void InitTrieList ()
{
	int	c;

	for (c = 0; c < 256; c++)
	{
		if (isdigit (c))
			keyarray[c] = c - '0';
		else
			keyarray[c] = c%10;
	}
}

static trie_t * NewNode ()
{
	int	i;
	trie_t	*node = (trie_t *)malloc (sizeof (trie_t));

	if (node)
	{
		node->list = NULL;
		for (i = 0; i < TRIE_FANOUT; i++)
		{
			node->subchanged[i] = 0;
			node->listchanged = 0;
			node->subtrie[i] = NULL;
		}
	}

	return node;
}

/*--------------------------------
 *	usage:	������в���һ���µ�����
 *	arguments:	*trie -- ����ͷָ��
 *			str   -- ��ֵ�ַ���
 *			level -- ��ֵ����
 *			data  -- Ҫ���������
 *			n     -- Ҫ��������ݵĴ�С
 *			changed - ��¼��ǰ�ڵ���ӽڵ�������Ƿ����˱仯
 *					1 -- �б仯	0 -- �ޱ仯
 *	return:	-1 -- ����	0 -- ����
 *	comment��������Ҷ�ڵ���������������ʱ���ȸ��ݼ�ֵ�ҵ�Ҷ�ڵ㣬����
 *		 Ҷ�ڵ���ָ�������в������ݡ�
 *---------------------------------*/
int InsertTrieList (trie_t **trie, const char str[], const int level, 
		void *data, size_t n, int changed)
{
	int	i;
	int	key;
	trie_t	*curnode;

	if (*trie == NULL)
	{
		*trie = NewNode ();
		if (*trie == NULL)
			return -1;
	}

	curnode = *trie;
	for (i = 0; i < level ; i++)
	{
		key = keyarray[str[i]];
		if (curnode->subtrie[key] == NULL)
		{
			if ((curnode->subtrie[key] = NewNode ()) == NULL)
				return -1;
		}
		curnode->subchanged[key] = changed;
		
		curnode = curnode->subtrie[key];
	}
	
	curnode->listchanged = changed;
	
	return (InsertList (&(curnode->list), data, n));
}
/*--------------------------------
 *	usage:	������в���һ���µ�����
 *	arguments:	*trie -- ����ͷָ��
 *			str   -- ��ֵ�ַ���
 *			level -- ��ֵ����
 *			data  -- Ҫ���������
 *			n     -- Ҫ��������ݵĴ�С
 *			changed - ��¼��ǰ�ڵ���ӽڵ�������Ƿ����˱仯
 *					1 -- �б仯	0 -- �ޱ仯
 *	return:	-1 -- ����	0 -- ����
 *	comment��������Ҷ�ڵ���������������ʱ���ȸ��ݼ�ֵ�ҵ�Ҷ�ڵ㣬����
 *		 Ҷ�ڵ���ָ�������в������ݡ�
 *---------------------------------*/
int InsertIndexTrieList (trie_t **trie, const char str[], const int level, 
		void *data, int changed)
{
	int	i;
	int	key;
	trie_t	*curnode;

	if (*trie == NULL)
	{
		*trie = NewNode ();
		if (*trie == NULL)
			return -1;
	}

	curnode = *trie;
	for (i = 0; i < level ; i++)
	{
		key = keyarray[str[i]];
		if (curnode->subtrie[key] == NULL)
		{
			if ((curnode->subtrie[key] = NewNode ()) == NULL)
				return -1;
		}
		curnode->subchanged[key] = changed;
		
		curnode = curnode->subtrie[key];
	}
	
	curnode->listchanged = changed;
	
	return (InsertIndexList (&(curnode->list), data));
}
/*--------------------------------
 *	usage:	������в���һ���µ�����
 *	arguments:	*trie -- ����ͷָ��
 *			str   -- ��ֵ�ַ���
 *			level -- ��ֵ����
 *			data  -- Ҫ���������
 *			n     -- Ҫ��������ݵĴ�С
 *			changed - ��¼��ǰ�ڵ���ӽڵ�������Ƿ����˱仯
 *					1 -- �б仯	0 -- �ޱ仯
 *	return:	-1 -- ����	0 -- ����
 *	comment��������Ҷ�ڵ���������������ʱ���ȸ��ݼ�ֵ�ҵ�Ҷ�ڵ㣬����
 *		 Ҷ�ڵ���ָ�������в������ݡ�
 *---------------------------------*/
int InsertTrieListOrderByKey (trie_t **trie, const char str[], const int level, 
		void *data, size_t n, int changed,int (*cmp) (const void *data1, const void *data2))
{
	int	i;
	int	key;
	trie_t	*curnode;

	if (*trie == NULL)
	{
		*trie = NewNode ();
		if (*trie == NULL)
			return -1;
	}

	curnode = *trie;
	for (i = 0; i < level ; i++)
	{
		key = keyarray[str[i]];
		if (curnode->subtrie[key] == NULL)
		{
			if ((curnode->subtrie[key] = NewNode ()) == NULL)
				return -1;
		}
		curnode->subchanged[key] = changed;
		
		curnode = curnode->subtrie[key];
	}
	
	curnode->listchanged = changed;
	
	return (InsertListOrderByKey (&(curnode->list), data, n,cmp));
}

/*--------------------------------
 *	usage:	������в���һ���µ�����
 *	arguments:	*trie -- ����ͷָ��
 *			str   -- ��ֵ�ַ���
 *			level -- ��ֵ����
 *			data  -- Ҫ���������
 *			n     -- Ҫ��������ݵĴ�С
 *			changed - ��¼��ǰ�ڵ���ӽڵ�������Ƿ����˱仯
 *					1 -- �б仯	0 -- �ޱ仯
 *	return:	-1 -- ����	0 -- ����
 *	comment��������Ҷ�ڵ���������������ʱ���ȸ��ݼ�ֵ�ҵ�Ҷ�ڵ㣬����
 *		 Ҷ�ڵ���ָ�������в������ݡ�
 *---------------------------------*/
int InsertIndexTrieListOrderByKey (trie_t **trie, const char str[], const int level, 
		void *data, int changed,int (*cmp) (const void *data1, const void *data2))
{
	int	i;
	int	key;
	trie_t	*curnode;

	if (*trie == NULL)
	{
		*trie = NewNode ();
		if (*trie == NULL)
			return -1;
	}

	curnode = *trie;
	for (i = 0; i < level ; i++)
	{
		key = keyarray[str[i]];
		if (curnode->subtrie[key] == NULL)
		{
			if ((curnode->subtrie[key] = NewNode ()) == NULL)
				return -1;
		}
		curnode->subchanged[key] = changed;
		
		curnode = curnode->subtrie[key];
	}
	
	curnode->listchanged = changed;
	
	return (InsertIndexListOrderByKey (&(curnode->list), data,cmp));
}
/*--------------------------------
 *	usage:	�ڼ����в�������
 *	arguments:	trie  -- ����ͷָ��
 *			str   -- ��ֵ�ַ���
 *			level -- ��ֵ����
 *			data  -- Ҫ���ҵ�����
 *			cmp   -- �ȽϺ���ָ��
 *	return:	�ҵ� -- ����ָ������ݵ�ָ��	û�ҵ� -- NULL
 *	comment:���ҹ�����cmp����ָ��
 *---------------------------------*/
void * SearchTrieList (trie_t *trie, const char str[], const int level, void *data, 
		int (*cmp) (const void *data1, const void *data2))
{
	int	i;
	int	key;
	trie_t	*curnode;

	if (trie == NULL)
		return NULL;

	curnode = trie;
	for (i = 0; i < level ; i++)
	{
		key = keyarray[str[i]];
		if (curnode->subtrie[key] == NULL)
			return NULL;
		
		curnode = curnode->subtrie[key];
	}

	return (SearchList (curnode->list, data, cmp));
}
/*--------------------------------
 *	usage:	�ڼ����в��Ҽ�ֵָ�����ͷ�����������Ľڵ��changed�ֶ���1
 *		��ʾ�ýڵ���ӽڵ�Ҫ�����仯����ڵ㲻���ڣ������ɸýڵ�
 *	arguments:	trie  -- ����ͷָ��
 *			str   -- ��ֵ�ַ���
 *			level -- ��ֵ����
 *			
 *	return:	�ҵ� -- ����ָ�������ͷ��ָ��	û�ҵ� -- NULL
 *	comment:
 *---------------------------------*/
int TouchTrieListByPriKey (trie_t **trie, const char str[], const int level)
{
	int	i;
	int	key;
	trie_t	*curnode;

	if (*trie == NULL)
	{
		*trie = NewNode ();
		if (*trie == NULL)
			return -1;
	}

	curnode = *trie;
	for (i = 0; i < level ; i++)
	{
		key = keyarray[str[i]];
		if (curnode->subtrie[key] == NULL)
		{
			if ((curnode->subtrie[key] = NewNode ()) == NULL)
				return -1;
		}
		curnode->subchanged[key] = 1;
		
		curnode = curnode->subtrie[key];
	}
	
	curnode->listchanged = 1;
	
	return 0;
}

/*--------------------------------
 *	usage:	�ڼ����в��Ҽ�ֵָ�����ͷ�����������Ľڵ��changed�ֶ���1
 *		��ʾ�ýڵ���ӽڵ�Ҫ�����仯����ڵ㲻���ڣ������ɸýڵ�
 *	arguments:	trie  -- ����ͷָ��
 *			str   -- ��ֵ�ַ���
 *			level -- ��ֵ����
 *			list  -- ����ָ����ͷlistָ���ָ�룬����Ҫ����ָ���ָ�룬
 *				 ʹ��3��ָ��
 *	return:	�ҵ� -- ����ָ�������ͷ��ָ��	û�ҵ� -- NULL
 *	comment:
 *---------------------------------*/
int TouchTrieList (trie_t **trie, const char str[], const int level, list_t ***list)
{
	int	i;
	int	key;
	trie_t	*curnode;

	if (*trie == NULL)
	{
		*trie = NewNode ();
		if (*trie == NULL)
			return -1;
	}

	curnode = *trie;
	for (i = 0; i < level ; i++)
	{
		key = keyarray[str[i]];
		if (curnode->subtrie[key] == NULL)
		{
			if ((curnode->subtrie[key] = NewNode ()) == NULL)
				return -1;
		}
		curnode->subchanged[key] = 1;
		
		curnode = curnode->subtrie[key];
	}
	
	curnode->listchanged = 1;
	
	*list = &(curnode->list);
	
	return 0;
}

/*-------------------------------------------
 *
 *-------------------------------------------*/
list_t *GetListofTrieList (trie_t *trie, const char str[], const int level)
{
	int	i;
	int	key;
	trie_t	*curnode;
	
	if (trie == NULL)
		return NULL;
	
	curnode = trie;
	for (i = 0; i < level ; i++)
	{
		key = keyarray[str[i]];
		if (curnode->subtrie[key] == NULL)
			return NULL;
			
		curnode = curnode->subtrie[key];
	}
	
	return curnode->list;
}

/*-------------------------------
 *	usage:	�ͷż���
 *	arguments:	trie -- the head of trie
 *-------------------------------*/
void FreeTrieList (trie_t **trie)
{
	int	i;

	if (*trie)
	{
		for (i = 0; i < TRIE_FANOUT; i++)
			FreeTrieList (&((*trie)->subtrie[i]));
		FreeList (&((*trie)->list));
		free (*trie);
		*trie=NULL;
	}
}

/*-------------------------------
 *	usage:	�ͷż���
 *	arguments:	trie -- the head of trie
 *-------------------------------*/
void FreeIndexTrieList (trie_t **trie)
{
	int	i;

	if (*trie)
	{
		for (i = 0; i < TRIE_FANOUT; i++)
			FreeIndexTrieList (&((*trie)->subtrie[i]));
		FreeIndexList (&((*trie)->list));
		free (*trie);
		*trie=NULL;
	}
}

/*-------------------------------
 *	usage:	�ͷż���������
 *	arguments:	trie -- the head of trie
 *-------------------------------*/
void FreeIndexTrieListAndData (trie_t **trie,void (* free_op) (void *data))
{
	int	i;

	if (*trie)
	{
		for (i = 0; i < TRIE_FANOUT; i++)
			FreeIndexTrieListAndData (&((*trie)->subtrie[i]),free_op);
		FreeIndexListAndData (&((*trie)->list),free_op);
		free (*trie);
		*trie=NULL;
	}
}

/*----------------------------------
 *	usage:	print the data of the trie
 *----------------------------------*/
void PrintTrieList (trie_t *trie, const int level, const int key, void (*print) (const void *data))
{
	int	i;
	
	if (trie)
	{
		fprintf (stderr, "enter subtrie -- level:%d,key:%d\n", level, key);
		for (i = 0; i < TRIE_FANOUT; i++)
		{
			PrintTrieList (trie->subtrie[i], level + 1, i, print);
		}
		PrintList (trie->list, print);
	}
}

void OperateTrieList (trie_t *trie, void (* op_data) (void *data))
{
	int i;
	
	if (trie)
	{
		for (i = 0; i < TRIE_FANOUT; i++)
		{
			OperateTrieList (trie->subtrie[i], op_data);
		}
		OperateList (trie->list, op_data);
	}
}


/*------------------------------------------
 *	usage:	ˢ��TRIE����changedΪ1�Ľڵ���ӽڵ��������op_listָ���Ĳ���
 *	parameters:	trie   -- trie head pointer
 *			op_list-- ��list�Ĳ���
 *------------------------------------------*/
void RefreshTrieList (trie_t *trie, void (* op_data) (void *data))
{
	int	i;
	
	if (trie)
	{
		for (i = 0; i < TRIE_FANOUT; i++)
		{
			if (trie->subchanged[i]) /* �ӽڵ㷢�����仯 */
			{
				RefreshTrieList (trie->subtrie[i], op_data);
				trie->subchanged[i] = 0;
			}
		}
		if (trie->listchanged)	
		{
			OperateList (trie->list, op_data);
			trie->listchanged = 0;
		}
	}
		
}
