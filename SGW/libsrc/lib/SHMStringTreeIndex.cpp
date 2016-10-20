/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "SHMStringTreeIndex.h"
#include <string.h>
#include "MBC.h"
#include "Stack.h"
#include<iostream>
#include "platform.h"
#define STRING_KEY_LEN	12
//##ʵ��Ӱ��key�ĳ��ȣ�����Ŀǰ���ź���һ����11λ�����ԣ���������Ϊ11

//##ModelId=424429AB039E
SHMStringTreeIndex::SHMStringTreeIndex(char *shmname) :
SHMAccess(shmname), m_piListTotal(0), m_piListUsed(0), m_piTreeTotal(0),m_piIndexTime(0),
m_piTreeUsed(0), m_piKeyLen(0)
{
	if (m_poSHM)
		open ();
}


//##ModelId=424429AB03BC
SHMStringTreeIndex::SHMStringTreeIndex(long shmkey) :
SHMAccess(shmkey), m_piListTotal(0), m_piListUsed(0), m_piTreeTotal(0),m_piIndexTime(0),
m_piTreeUsed(0), m_piKeyLen(0)
{
	if (m_poSHM)
		open ();
}


//##ModelId=424429AC001A
SHMStringTreeIndex::~SHMStringTreeIndex()
{
}

//##ModelId=42442A8B0093
void SHMStringTreeIndex::add(char *key, unsigned int value)
{
	int i, j;
	char *p = key;
	int index = 0;
	SHMStringList *pl;
	
	for(i=0; i<STRING_KEY_LEN && *p; p++,i++) {
		j = (*p)%10;
		if (m_poTree[index].m_iNext[j]) {
			index = m_poTree[index].m_iNext[j];
		} else {
			if (*m_piTreeUsed < *m_piTreeTotal) {
				m_poTree[index].m_iNext[j] = (*m_piTreeUsed)++;
				index = m_poTree[index].m_iNext[j];
			} else {
				THROW(MBC_SHMStringTreeIndex+1);
			}
		}
	}

	if (!(m_poTree[index].m_iValue)) {
		goto __ADD;
	}

	j = m_poTree[index].m_iValue;
	pl = getList (j);
	while (pl ) {
		if (!strncmp(pl->m_sKey, key, *m_piKeyLen)) {
			pl->m_iValue = value;
			return ;
		}
		pl = getList (pl->m_iNext);
	}		

__ADD:
	if (*m_piListUsed < *m_piListTotal) {
		j = (*m_piListUsed)++;
		pl = getList(j);
		pl->m_iNext = m_poTree[index].m_iValue;
		m_poTree[index].m_iValue = j;
		pl->m_iValue = value;
		memset(pl->m_sKey,0,sizeof(pl->m_sKey));
		memcpy (pl->m_sKey, key, strlen(key));
	} else {
		THROW(MBC_SHMStringTreeIndex+2);
	}
	return;
}

bool SHMStringTreeIndex::getMax(char *key, unsigned int *pvalue)
{
	int i, j;
	char *p = key;
	int index = 0;
	SHMStringList *pl;

    Stack<int> stack;
	
	for(i=0; i<STRING_KEY_LEN && *p; p++,i++) {
		j = (*p)%10;
		if (m_poTree[index].m_iNext[j]) {
			index = m_poTree[index].m_iNext[j];
            stack.push (index);
		} else {
            break;
		}
	}

    //--xueqt 20060710 STRING_KEY_LEN=12 
    //���Բ��������ų���12λ�ľ�ȷƥ������
    //����Ǹ��������Ժ���ֳ���12λ�ģ��������������Ż��㷨

    while (stack.pop (index)) {
        j = m_poTree[index].m_iValue;
        if (j) {
            pl = getList (j);
            while (pl ) {
                if (!strncmp(pl->m_sKey, key, i)) {
                    *pvalue = pl->m_iValue ;
                    return true;
                }
                pl = getList (pl->m_iNext);
            }		
        }

        i--;
    }

    return false;
}


//##ModelId=42442A9101C8
bool SHMStringTreeIndex::get(char *key, unsigned int *pvalue)
{
	int i, j;
	char *p = key;
	int index = 0;
	SHMStringList *pl;
	
	for(i=0; i<STRING_KEY_LEN && *p; p++,i++) {
		j = (*p)%10;
		if (m_poTree[index].m_iNext[j]) {
			index = m_poTree[index].m_iNext[j];
		} else {
			return false;
		}
	}

	if (!m_poTree[index].m_iValue) return false;


	j = m_poTree[index].m_iValue;
	pl = getList (j);
	while (pl ) {
		if (!strncmp(pl->m_sKey, key, *m_piKeyLen)) {
			*pvalue = pl->m_iValue ;
			return true;
		}
		pl = getList (pl->m_iNext);
	}		
	return false;
}

//##ModelId=42442B0802C3
void SHMStringTreeIndex::create(unsigned int itemnumber, unsigned int keylen, unsigned int multiple)

{
	unsigned long size1, size2, size3;
	unsigned int iListLen;
	unsigned int iKeyLen = keylen;

	//##����SHMStringList�ĳ��ȣ��ڴ水��8���ֽڶ��롣
	if (keylen <= 8) {
		iListLen = sizeof(SHMStringList);
	} else { 
		iListLen = sizeof(SHMStringList) + (keylen&(~7u));
	}

	//##SHMStringList��ռ�ĳ���
	size1 = iListLen * itemnumber;
	
	//##SHMStringTree��ռ�ĳ���
	size2 = sizeof(SHMStringTree)*itemnumber*multiple;

	size3 = size1 + size2 + sizeof(unsigned int)*12 + 3*sizeof(unsigned long);
	
	SHMAccess::create (size3);
	open ();
	
	*m_piListLen = iListLen;
	*m_piKeyLen = iKeyLen;
	*m_piListTotal = itemnumber;
	*m_piTreeTotal = itemnumber*multiple;
	*m_piListUsed = 1;
	*m_piTreeUsed = 1; 
	Date d ;
	*m_piIndexTime = atol(d.toString());
	*m_pimemAdr = (unsigned long)(m_poSHM);
	m_poList = (char *)(((unsigned long)(char *)(m_poTree+*m_piTreeTotal)+8ul) & (~7ul)) \
			- ((*m_piListLen)/8ul)*8ul ;
}

//##ModelId=4244EBC6017B
void SHMStringTreeIndex::open()
{
	m_piListTotal = ((unsigned int *)((char *)(*m_poSHM))) + 1;
	m_piListUsed = m_piListTotal + 1;
	m_piTreeTotal = m_piListUsed + 1;
	m_piTreeUsed = m_piTreeTotal + 1;
	m_piIndexTime = (unsigned long*) (m_piTreeUsed + 2);
	m_pimemAdr = (unsigned long *)(m_piIndexTime+1);
	m_piKeyLen =(unsigned int*)(m_pimemAdr + 1);
	m_piListLen = m_piKeyLen + 1;

	m_poTree = (SHMStringTree *) (m_piListLen + 1);	
	m_poList = (char *)(((unsigned long)(char *)(m_poTree+*m_piTreeTotal)+8ul) & (~7ul)) \
			- ((*m_piListLen)/8)*8 ;
}

//##ModelId=424512FC000C
SHMStringList *SHMStringTreeIndex::getList(unsigned int offset)
{
	if (!offset)
		return 0;

	return (SHMStringList *)(m_poList + (*m_piListLen)*offset);
}

time_t  SHMStringTreeIndex::getLoadTime()
{
	time_t shm_ctime;
	
	if (m_poSHM)
	{
		shm_ctime = m_poSHM->getShmTime();	
		
		return shm_ctime;
	}
	return  0;
	
}

int SHMStringTreeIndex::getMemSize()
{	
	if (*m_piListTotal)
		
		return ((*m_piListLen) * (*m_piListTotal) + sizeof(SHMStringTree) * (*m_piListTotal) * 3 + sizeof(unsigned int) * 8 + sizeof(unsigned long));		
		return 0;
}
int SHMStringTreeIndex::getUsedSize()
{	  
	if(*m_piListUsed)
	return ((*m_piListLen) * (*m_piListUsed) + sizeof(SHMStringTree) * (*m_piListUsed) * 3 + sizeof(unsigned int) * 8 + sizeof(unsigned long));
	return 0;
}
void SHMStringTreeIndex::empty()
{
		memset (m_poTree, 0, sizeof (SHMStringTree)*(*m_piTreeTotal));
		*m_piTreeUsed = 1;
		memset (m_poList, 0, sizeof (SHMStringList)*(*m_piListTotal));
		*m_piListUsed = 1;
}
unsigned long  SHMStringTreeIndex::getMemAdress()
{		
	if (m_poSHM)
	{
		return  (unsigned long )m_poSHM;	
	}
	return  0;
}

