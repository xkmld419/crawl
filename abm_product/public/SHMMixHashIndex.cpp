/*VER: 1*/ 
// Copyright (c) 2010,���������Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "SHMMixHashIndex.h"
#include <string.h>
#include "MBC.h"
#include "Stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define MIX_STRING_KEY_LEN	15
//##ʵ��Ӱ���key������Ч�ʣ�����Ŀǰlong,int������ֵ�����һ�㲻����15λ

//iType��1��ʼ��
SHMMixHashIndex::SHMMixHashIndex(char *shmname) :
SHMAccess(shmname), m_piListTotal(0), m_piListUsed(0), m_piTreeTotal(0),m_sType(0),m_pAddr(0),m_iType(0),
m_piTreeUsed(0), m_piKeyLen(0),m_iCount(0),m_bInitCount(false),m_iMultiple(0),m_iReuseListTotalNum(0),m_iReuseTreeTotalNum(0)
{
	m_sMixKey = 0;
	if (m_poSHM)
	{
		open ();
		m_sMixKey = new char[(*m_piKeyLen)+16];
		if(!m_sMixKey)
			THROW(MBC_SHMMixIndex+3);
		m_sMixKey[0] = '\0';
	} 
	

}


SHMMixHashIndex::SHMMixHashIndex(long shmkey):
SHMAccess(shmkey), m_piListTotal(0), m_piListUsed(0), m_piTreeTotal(0),m_sType(0),m_pAddr(0),m_iType(0),
m_piTreeUsed(0), m_piKeyLen(0),m_iCount(0),m_bInitCount(false),m_iMultiple(0)
{
	m_sMixKey = 0;
	if (m_poSHM)
	{
		open ();
		m_sMixKey = new char[(*m_piKeyLen)+16];
		if(!m_sMixKey)
			THROW(MBC_SHMMixIndex+3);
		m_sMixKey[0] = '\0';
	}
	
}

SHMMixHashIndex::SHMMixHashIndex(char *sType,char *pAddr):
SHMAccess(sType,pAddr), m_piListTotal(0), m_piListUsed(0), m_piTreeTotal(0),m_sType(0),m_pAddr(pAddr),m_iType(0),
m_piTreeUsed(0), m_piKeyLen(0),m_iCount(0),m_bInitCount(false),m_iMultiple(0)
{
	m_sMixKey = 0;
	if (pAddr)
	{
		open (pAddr);
		m_sMixKey = new char[(*m_piKeyLen)+strlen(sType)+1];
		if(!m_sMixKey)
			THROW(MBC_SHMMixIndex+3);
		m_sMixKey[0] = '\0';
	}
	if(strlen(sType)>0)
	{
		m_sType = new char[strlen(sType)+1];
		if(!m_sType)
			THROW(MBC_SHMMixIndex+3);
		m_sType[0] = '\0';
		strcpy(m_sType,sType);
	}
}

SHMMixHashIndex::SHMMixHashIndex(unsigned int iType,char *pAddr):
SHMAccess(iType,pAddr), m_piListTotal(0), m_piListUsed(0), m_piTreeTotal(0),m_sType(0),m_pAddr(pAddr),
m_piTreeUsed(0), m_piKeyLen(0),m_iCount(0),m_bInitCount(false),m_iMultiple(0)
{
	m_sMixKey = 0;
	if (pAddr)
	{
		open (pAddr);
		m_sMixKey = new char[(*m_piKeyLen)+16];
		if(!m_sMixKey)
			THROW(MBC_SHMMixIndex+3);
		m_sMixKey[0] = '\0';
	}
	m_iType = iType;
	if(m_iType != 0)
	{
		m_sType = new char[20];
		if(!m_sType)
			THROW(MBC_SHMMixIndex+3);
		m_sType[0] = '\0';
		sprintf(m_sType,"%d",m_iType);
	}
}

//##ModelId=424429AC001A
SHMMixHashIndex::~SHMMixHashIndex()
{
	if(m_sMixKey)
		delete []m_sMixKey;
	if(m_sType)
		delete []m_sType;
}

//
void SHMMixHashIndex::add(long lKey,unsigned int value)
{
	 char sKey[19] = {0};
	 sprintf(sKey,"%ld",lKey);
	 add(sKey,value);
}

//����������, �ͽ�ȡǰ(*m_piKeyLen)λ���� �����Ͳ�׼ȷ�� 
void SHMMixHashIndex::add(char *key, unsigned int value)
{
	char *p = 0;
	char *KeyMix = 0;
	if(m_sType)
	{	
		if(m_sMixKey)
			m_sMixKey[0] = '\0';
		else{
			m_sMixKey = new char[(*m_piKeyLen)+ strlen(m_sType) + 1];
			if(!m_sMixKey)
				THROW(MBC_SHMMixIndex+3);
			m_sMixKey[0] = '\0';
		}
		//�Ͳ��ж�ʵ���ַ����� 
		sprintf(m_sMixKey,"%s+%s",m_sType,key);
		p = m_sMixKey;
	} else {
		p = key;
	}
	KeyMix = p;
	//��add��ʱ��֤ ������ض��ַ�
	int iStrKeyLen = strlen(KeyMix);
	int iLen = iStrKeyLen>(*m_piKeyLen)?(*m_piKeyLen):iStrKeyLen;
	
	int i, j;
	int index = 0;
	SHMMixStringList *pl;
	//����
	for(i=0; i<MIX_STRING_KEY_LEN && *p; p++,i++) {
		j = (*p)%10;
		if (m_poTree[index].m_iNext[j]) {
			index = m_poTree[index].m_iNext[j];
		} else {
			if (*m_piTreeUsed < *m_piTreeTotal) {
				m_poTree[index].m_iNext[j] = (*m_piTreeUsed)++;
				index = m_poTree[index].m_iNext[j];
			} else {
				THROW(MBC_SHMMixIndex+1);
			}
		}
	}
	
	if (!(m_poTree[index].m_iValue)) {
		goto __ADD;
	}
	
	j = m_poTree[index].m_iValue;
	pl = getList (j);
	while (pl ) {
		if (!strncmp(pl->m_sKey, KeyMix, *m_piKeyLen)) {
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
		
		memcpy (pl->m_sKey, KeyMix, iLen);
		pl->m_sKey[iLen] = 0;
	} else {
		THROW(MBC_SHMMixIndex+2);
	}
	return;
}

bool SHMMixHashIndex::getMax(char *key, unsigned int *pvalue)
{
	char *p = 0;
	char *KeyMix = 0;
	if(m_sType)
	{	
		if(m_sMixKey)
			m_sMixKey[0] = '\0';
		else{
			m_sMixKey = new char[(*m_piKeyLen)+strlen(m_sType) + 1];
			if(!m_sMixKey)
				THROW(MBC_SHMMixIndex+3);
			m_sMixKey[0] = '\0';
		}
		
		sprintf(m_sMixKey,"%s+%s",m_sType,key);
		p = m_sMixKey;
	} else {
		p = key;
	}
	KeyMix = p;
	int i, j;
	int index = 0;
	SHMMixStringList *pl;
	
    Stack<int> stack;
	
	for(i=0; i<MIX_STRING_KEY_LEN && *p; p++,i++) {
		j = (*p)%10;
		if (m_poTree[index].m_iNext[j]) {
			index = m_poTree[index].m_iNext[j];
            stack.push (index);
		} else {
            break;
		}
	}
	
    while (stack.pop (index)) {
        j = m_poTree[index].m_iValue;
        if (j) {
            pl = getList (j);
            while (pl ) {
                if (!strncmp(pl->m_sKey, KeyMix, i)) {
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

bool SHMMixHashIndex::get(long lKey, unsigned int *pvalue)
{
	 char sKey[19] = {0};
	 sprintf(sKey,"%ld",lKey);
	 return get(sKey,pvalue);
}


//
bool SHMMixHashIndex::get(char *key, unsigned int *pvalue)
{
	char *p = 0;
	char *KeyMix = 0;
	if(m_sType)
	{	
		if(m_sMixKey)
			m_sMixKey[0] = '\0';
		else{
			m_sMixKey = new char[(*m_piKeyLen)+strlen(m_sType) + 1];
			if(!m_sMixKey)
				THROW(MBC_SHMMixIndex+3);
			m_sMixKey[0] = '\0';
		}
		
		sprintf(m_sMixKey,"%s+%s",m_sType,key);
		p = m_sMixKey;
	} else {
		p = key;
	}
	KeyMix = p;
	int i, j;
	int index = 0;
	SHMMixStringList *pl;
	
	for(i=0; i<MIX_STRING_KEY_LEN && *p; p++,i++) {
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
		if (!strncmp(pl->m_sKey, KeyMix, *m_piKeyLen)) {
			*pvalue = pl->m_iValue ;
			return true;
		}
		pl = getList (pl->m_iNext);
	}	
	return false;
}

//##ModelId=42442B0802C3
void SHMMixHashIndex::create(unsigned int itemnumber, unsigned int keylen, unsigned int multiple,unsigned int keyAddLen,unsigned int reuseNum, bool VarRay)
{
	m_iMultiple = multiple;
	m_iReuseListTotalNum = reuseNum + 1;
	m_iReuseTreeTotalNum = m_iReuseListTotalNum * m_iMultiple;
	unsigned long size1, size2, size3;
	unsigned int iListLen;
	unsigned int iKeyLen = keylen;
	iKeyLen += keyAddLen;
	
	//##����SHMMixStringList�ĳ��ȣ��ڴ水��8���ֽڶ��롣
	if (iKeyLen <= 8) {
		iListLen = sizeof(SHMMixStringList);
	} else { 
		iListLen = sizeof(SHMMixStringList) + (iKeyLen&(~7u));
	}
	
	//##SHMMixStringList��ռ�ĳ���
	size1 = iListLen * itemnumber;
	
	//##SHMMixStringTree��ռ�ĳ���
	size2 = sizeof(SHMMixStringTree)*itemnumber*m_iMultiple;
	
	size3 = size1 + size2 + sizeof(unsigned int)*(8+m_iReuseTreeTotalNum+m_iReuseListTotalNum+8);//�������ù���
	
	SHMAccess::create (size3);
	open ();
	
	*m_piListLen = iListLen;
	*m_piKeyLen = iKeyLen;
	*m_piListTotal = itemnumber;
	*m_piTreeTotal = itemnumber*multiple;
	*m_piListUsed = 1;
	*m_piTreeUsed = 1; 
	//
	
	*m_piListCurGet = 0; 
	*m_piReuseListTotal = m_iReuseListTotalNum;
	*m_piReuseListCanUse = 0;
	memset(m_piReuseList,0,(*m_piReuseListTotal)*sizeof(unsigned int));
		
	*m_piTreeCurGet = 0; 
	*m_piReuseTreeTotal = m_iReuseTreeTotalNum;
	*m_piReuseTreeCanUse = 0;
	memset(m_piReuseTree,0,(*m_piReuseTreeTotal)*sizeof(unsigned int));
	//
	m_poList = (char *)(((unsigned long)(char *)(m_poTree+*m_piTreeTotal)+8ul) & (~7ul)) \
			- (*m_piListLen) ;
}

//
void SHMMixHashIndex::open(char *pAddr)
{
	if(pAddr){
		m_piListTotal = (unsigned int *)pAddr + 1;
		m_pAddr = pAddr;
	} else {
		if(m_poSHM)
		{
			m_pAddr = (char*)(*m_poSHM);
			m_piListTotal = ((unsigned int *)m_pAddr) + 1;
		} else {
			THROW(MBC_SHMMixIndex+4);
		}
	}
	m_piListUsed = m_piListTotal + 1;
	m_piTreeTotal = m_piListUsed + 1;
	m_piTreeUsed = m_piTreeTotal + 1;
	m_piKeyLen = m_piTreeUsed + 1;
	m_piListLen = m_piKeyLen + 1;
	//
	m_piReuseListTotal = m_piListLen + 1;
	m_piReuseListCanUse = m_piReuseListTotal + 1;
	m_piReuseList = m_piReuseListCanUse + 1;
	if(m_iReuseListTotalNum)
		m_piListCurGet = m_piReuseList + m_iReuseListTotalNum;
	else
		m_piListCurGet = m_piReuseList + *m_piReuseListTotal;
	
	//  
	m_piReuseTreeTotal = m_piListCurGet + 1;
	m_piReuseTreeCanUse = m_piReuseTreeTotal + 1;
	m_piReuseTree = m_piReuseTreeCanUse + 1;
	if(m_iReuseTreeTotalNum)
		m_piTreeCurGet = m_piReuseTree + m_iReuseTreeTotalNum;
	else
		m_piTreeCurGet = m_piReuseTree + *m_piReuseTreeTotal;
	
	//
	m_poTree = (SHMMixStringTree *) (m_piTreeCurGet + 2);	
	m_poList = (char *)(((unsigned long)(char *)(m_poTree+*m_piTreeTotal)+8ul) & (~7ul)) \
			- (*m_piListLen) ;
}

//
SHMMixStringList *SHMMixHashIndex::getList(unsigned int offset)
{
	if (!offset)
		return 0;

	return (SHMMixStringList *)(m_poList + (*m_piListLen)*offset);
}

//��д�ĺ���
SHMMixStringList *SHMMixHashIndex::getList()
{
	//
	//��ʹ�������ṩ������
	if((*m_piListUsed)<(*m_piListTotal))
		return (SHMMixStringList *)(m_poList + (*m_piListLen)*(*m_piListUsed++));
	else //�ڲ��ҿ��Ը��õ�����
	{
		unsigned int iList =  MallocReuseList();
		if(iList)
			return (SHMMixStringList *)(m_poList + (*m_piListLen)*(iList));
		else
			return 0;
	}
}

//��ȡTREE�ṹ��
SHMMixStringTree *SHMMixHashIndex::getTree(unsigned int offset)
{
	//��ʹ�������ṩ������
	if(offset<(*m_piTreeTotal))
		return &m_poTree[offset];
	return 0;
}

//��ȡTREE�ṹ��
SHMMixStringTree *SHMMixHashIndex::getTree()
{
	//��ʹ�������ṩ������
	if((*m_piTreeUsed)<(*m_piTreeTotal))
		return &m_poTree[*m_piTreeUsed++];
	else //�ڲ��ҿ��Ը��õ�����
	{
		unsigned int iTree =  MallocReuseTree();
		if(iTree)
			return &m_poTree[iTree];
		else
			return 0;
	}
}

//��ȡTREE�ṹ��
unsigned int SHMMixHashIndex::MallocReuseTree()
{
	//��ʹ�������ṩ������
	if(*m_piReuseTreeTotal)
	{
		if(*m_piReuseTreeCanUse == 0)
			return 0;
			unsigned int iOffset = *m_piTreeCurGet;
		if((*m_piTreeCurGet) == ((*m_piReuseTreeTotal)-1))
			(*m_piTreeCurGet) = 0;
			else
			(*m_piTreeCurGet)++;
		(*m_piReuseTreeCanUse)--;
			return m_piReuseTree[iOffset];
	}
	return 0;
}

//��ȡTREE�ṹ��
unsigned int SHMMixHashIndex::MallocReuseList()
{
	//
	if(*m_piReuseListTotal)
	{
		//
		if(*m_piReuseListCanUse == 0)
			return 0;

			unsigned int iOffset = *m_piListCurGet;
		if((*m_piListCurGet) == ((*m_piReuseListTotal)-1))
			(*m_piListCurGet) = 0;
			else
			(*m_piListCurGet)++;
		(*m_piReuseListCanUse)--;
			return m_piReuseList[iOffset];
		
	}
	return 0;
}

//ɾ��
bool SHMMixHashIndex::removeIndex(char *key)
{
	//�����븴λ
	m_iErrNo = 0;
	//��¼�����Ϣ
	unsigned int iTreeNumRec = 0;//����
	unsigned int iTreeRec[MIX_STRING_KEY_LEN] = {0};//ÿһ���TREEƫ�ƣ��Ϳ����ֱ���ͷ���
	//
	unsigned int ivalue = 0;
	unsigned int *pvalue = &ivalue;
	char *p = 0;
	char *KeyMix = 0;
	if(m_sType)
	{	
		m_sMixKey[0] = '\0';
		sprintf(m_sMixKey,"%s+%s",m_sType,key);
		p = m_sMixKey;
	} else {
		p = key;
	}
	KeyMix = p;
	int i, j;
	int index = 0;
	SHMMixStringList *pl,*p2;
	unsigned int iList1 = 0;
	
	for(i=0; i<MIX_STRING_KEY_LEN && *p; p++,i++) {
		j = (*p)%10;
		if (m_poTree[index].m_iNext[j]) {
			index = m_poTree[index].m_iNext[j];
		} else {
			return false;
		}
		iTreeRec[iTreeNumRec++] = index;//
	}

	if (!m_poTree[index].m_iValue) return false;
	
	j = m_poTree[index].m_iValue;
	if(!j)
		return false;
	pl = getList (j);
	bool bIndex = true;
	unsigned int iLastList = 0;
	unsigned int iThisList = 0;
	
	while (pl ) 
	{
		if (strncmp(pl->m_sKey, KeyMix, *m_piKeyLen) == 0) 
		{
			*pvalue = pl->m_iValue ;
			//�ҵ���Ӧ��LIST�� ����ɾ�����ô���
			if(bIndex)
			{
				m_poTree[index].m_iValue = pl->m_iNext;
				if(m_poTree[index].m_iValue == 0)
				{
					//ͬʱû����TREE��ɾ��
					bool TreeDelete = true;
					for(unsigned int iT=0; iT<10; iT++)
					{
						if(m_poTree[index].m_iNext[iT])
						{
							TreeDelete = false;
							break;
						}
					}
					if(TreeDelete)
						deleteTree(index);
				}
				deleteList(j);
				return true;
			} else if(iLastList)
			{
				p2 = getList (iLastList);
				if(!p2){
					sprintf(m_sErrDesc,"LISTƫ��[%d]��Ч",iLastList);
					return false;
				}
				p2->m_iNext = pl->m_iNext;
				deleteList(iThisList);
				return true;
			}
			sprintf(m_sErrDesc,"�ҵ�KEY[%s]������,LISTƫ��[%d],ɾ������ʧ��",key,iThisList);
			return false;
		}
		if(bIndex){
			iLastList = j;
			bIndex = false;
		} else {
			iLastList = iThisList;
		}
		iThisList = pl->m_iNext;//��һ�������ƫ��ֵ
		pl = getList (iThisList);
		
	}
	sprintf(m_sErrDesc,"�޶�ӦKEY[%s]ֵ����������",key);
	return false;
}

//
bool SHMMixHashIndex::deleteList(unsigned int iListOffset)
{
	if(!iListOffset)
		return false;
	return addReuseList(iListOffset);
}

bool SHMMixHashIndex::deleteTree(unsigned int iTreeOffset)
{
	if(!iTreeOffset)
		return false;
	return addReuseTree(iTreeOffset);
}
//
bool SHMMixHashIndex::addReuseList(unsigned int iListOffset)
{
	//
	if(*m_piReuseListTotal == 0 || !iListOffset)
		return false;
	if(*m_piReuseListCanUse == 0)
	{
		m_piReuseList[0] = iListOffset;
		(*m_piReuseListCanUse)++;
		*m_piListCurGet = 0;
		return true;
	} 
	if((*m_piReuseListCanUse) < (*m_piReuseListTotal))
	{
		//
		unsigned int iOffset = (*m_piListCurGet + *m_piReuseListCanUse);
		if(iOffset > (*m_piReuseListTotal - 1))
			iOffset -= (*m_piReuseListTotal);
		//unsigned int iOffsetKeep = iOffset;
		// 
		if(iOffset != *m_piListCurGet)
		{
			m_piReuseList[iOffset] = iListOffset;
			(*m_piReuseListCanUse)++;
			return true;
		}
		//
		
	}
	return false;
}

//
bool SHMMixHashIndex::addReuseTree(unsigned int iTreeOffset)
{
	//
	if(*m_piReuseTreeTotal == 0 || !iTreeOffset)
		return false;
	if(*m_piReuseTreeCanUse == 0)
	{
		m_piReuseTree[0] = iTreeOffset;
		(*m_piReuseTreeCanUse)++;
		*m_piTreeCurGet = 0;
		return true;
	} 
	if((*m_piReuseTreeCanUse) < (*m_piReuseTreeTotal))
	{
		// 
		unsigned int iOffset = (*m_piTreeCurGet + *m_piReuseTreeCanUse);
		if(iOffset > (*m_piReuseTreeTotal - 1))
			iOffset -= (*m_piReuseTreeTotal);
		// 
		if(iOffset != *m_piTreeCurGet)
		{
			m_piReuseTree[iOffset] = iTreeOffset;
			(*m_piReuseTreeCanUse)++;
			return true;
		}
	}
	return false;
}


// 
void SHMMixHashIndex::empty()
{
		memset (m_poTree, 0, sizeof (SHMMixStringTree)*(*m_piTreeTotal));
		*m_piTreeUsed = 1;
		memset (m_poList, 0, (*m_piListLen)*(*m_piListTotal));
		*m_piListUsed = 1;
		//���ù���
		*m_piTreeCurGet = 0;
		*m_piListCurGet = 0;
		*m_piReuseListCanUse = 0;
		*m_piReuseTreeCanUse = 0;
		memset(m_piReuseTree,0,(*m_piReuseTreeTotal)*sizeof(unsigned int));
		memset(m_piReuseList,0,(*m_piReuseListTotal)*sizeof(unsigned int));
}
//��ȡ����������ʹ����Ŀ
unsigned int SHMMixHashIndex::getCount(bool ReInitCount)
{
		unsigned int iIndexCount = 0;
		if(m_sType)
		{
			if(!ReInitCount)
			{
				if(m_bInitCount)
				{
					return m_iCount;
				}
			}
			int iTypeLen = 1;
			unsigned int iBase = 10;
			
			SHMMixStringList *pl = 0;
			unsigned int iOffset = 1;
			char key[18] = {0};
			sprintf(key,"%s_",m_sType);
			int iSTypeLen = strlen(m_sType);
			while(iOffset <= (*m_piListUsed))
			{
				pl = getList (iOffset);
				if (pl) 
				{
					if (!strncmp(pl->m_sKey, key, iSTypeLen+1)) 
						iIndexCount++;
				}
				iOffset++;
			}
			m_iCount = iIndexCount;
			m_bInitCount = true;
			return iIndexCount;
			
		} else {
			return getTCount();
		}
}

unsigned int SHMMixHashIndex::getTotal()
{
		return getTTotal();
}	
//TREE
unsigned int SHMMixHashIndex::getTCount()
{
		if (*m_piTreeUsed)
			return (*m_piTreeUsed);
		
		return 0;
}

unsigned int SHMMixHashIndex::getTTotal()
{
		if (*m_piTreeTotal)
			return (*m_piTreeTotal);

		return 0;
}
//LIST
unsigned int SHMMixHashIndex::getlCount()
{
		if (*m_piListUsed)
			return (*m_piListUsed);

		return 0;		
}

unsigned int SHMMixHashIndex::getlTotal()
{
		if (*m_piListTotal)
			return (*m_piListTotal);

		return 0;
}

bool SHMMixHashIndex::typeExist()
{
	 if(m_sType)
	 {
	 	if(m_pAddr)
	 		return true;
	 	return false;
	 } else {
	 	return exist();
	 }
}

//���ù��� ���븴����Ч�����ݿռ�
unsigned int SHMMixHashIndex::ReuseMalloc()
{
	//
	
	return 0;
}	

