/*VER: 1*/ 
/*
 *	HASH����Һ���(���ڽ������ݴ����HASH��ͨ��KEYֵ����ָ������)
 *
 *	creator		�κ���
 *	date		02/23/2001
 */
#include "./hash_list.h"

/*
list_t	**hash_list;
*/

/*��HASH�����HASH��ɢ�Ѽ���������������ù���list.c�еĲ��� comentate by wangzy*/

static int hash (const char *key)
{
	int	hval;
	const char	*ptr;
	char	c;
	int	i;
	
	hval = 0;
	for (ptr = key, i = 1; c=*ptr++; i++)
		hval += c*i; /* ascii char times its 1-based index */
		
	return (hval%HASH_NUM);
}

void initHashList (THashList *h_list)
{
	int	i;
	list_t	**pplist;
	
	pplist = (list_t **) calloc (sizeof (list_t *), HASH_NUM);
	
	for (i = 0; i < HASH_NUM; i++)
		pplist[i] = NULL;
	
	*h_list = pplist;
}

/*
 *	�Ը���KEYΪ�������Ĳ�������
 *	KEY��ͬ�����ݽӵ���ͬ���������Ժ������
 *	pre:  ��һ�ε���ʱ��Ҫ����*keylist=NULL
 */
int insertHashList (THashList h_list, const char *key, void *data, int data_size)
{
	int	hval;
	
	hval = hash (key);
	
	if (InsertList (&(h_list[hval]), data, data_size) == -1)
			return -1;	
	return 0;
}

/*
 *	������ֵ����
 *
 *	key	Ϊ��ֵ
 *	data	Ϊ�Ƚ�����
 *	cmp	Ϊ�ȽϺ���ָ�룬����ֵ����Ϊ����0��ʾ��ȣ�����Ϊ����
 *		data1Ϊ�����е����ݣ�data2Ϊ���ڱȽϵ�����
 *
 *	����ֵ	ָ��ƥ������ݵ�ָ��
 */
void *searchHashList (THashList h_list, char *key, void *cmp_data,
	int (*cmp) (const void *list_data, const void *cmp_data))
{
	list_t	*pcur;
	int	hval;
	
	hval = hash (key);
	pcur = h_list[hval];
	
	return SearchList (pcur, cmp_data, cmp);
}

void freeHashList (THashList h_list)
{	
	int	i;
	
	for (i = 0; i < HASH_NUM; i++)
	{
		FreeList (h_list[i]);
	}
	
	free (h_list);
}

void printHashList (THashList h_list, void (*print) (const void *data))
{
	int	i;
	
	for (i = 0; i < HASH_NUM; i++)
		PrintList (h_list[i], print);
}
