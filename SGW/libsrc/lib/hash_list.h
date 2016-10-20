#ifndef HASHLIST_H
#define HASHLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "h_list.h"

#define HASH_NUM	256		/* HASH���С */

#define THashList	list_t **

/*
 *	��ʼ��HASH��
 */
void initHashList (THashList *h_list);

/*
 *	�Ը���KEYΪ��������HASH����
 *	hash(KEY)��ͬ�����ݽӵ���ͬ���������Ժ������
 *	pre:  ��һ�ε���ʱ��Ҫ����*keylist=NULL
 */
int insertHashList (THashList h_list, const char *key, void *data, int data_size);

/*
 *	����HASH����
 *
 *	key	Ϊ��ֵ
 *	data	Ϊ�Ƚ�����
 *	cmp	Ϊ�ȽϺ���ָ�룬����ֵ����Ϊ����0��ʾ��ȣ�����Ϊ����
 *		list_dataΪ�����е����ݣ�cmp_dataΪ���ڱȽϵ�����
 *
 *	����ֵ	ָ��ƥ������ݵ�ָ��
 */
void *searchHashList (THashList h_list, char *key, void *cmp_data,
	int (*cmp) (const void *list_data, const void *cmp_data));

void freeHashList (THashList h_list);
#endif




