/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "RecycleSHMIntHashIndex.h"
#include "MBC.h"

//##ModelId=424426D6039B
RecycleSHMIntHashIndex::RecycleSHMIntHashIndex(char *shmname) :
SHMAccess(shmname), m_piHashValue(0), m_piTotal(0), m_piUsed(0), m_poHash(0)
{
	if (m_poSHM)
		open();
}


//##ModelId=424426D603B9
RecycleSHMIntHashIndex::RecycleSHMIntHashIndex(long shmkey) :
SHMAccess(shmkey), m_piHashValue(0), m_piTotal(0), m_piUsed(0), m_poHash(0)
{
    if (m_poSHM) {
		open();
    }
}


//##ModelId=424426D603D7
RecycleSHMIntHashIndex::~RecycleSHMIntHashIndex()
{
}

long RecycleSHMIntHashIndex::getValue(unsigned int key)
{
    for (int i =1; i<*m_piTotal;i++) {
        if (m_poList[i].m_iValue == key) {
            return m_poList[i].m_lKey;
        }
    }
    return -1;
}

//##ModelId=42442A29038B
unsigned int RecycleSHMIntHashIndex::search(long key)
{
	unsigned int i = (key & (*m_piHashValue));
	unsigned int j;
	
	if (m_poHash[i]) {
	    
		j = m_poHash[i];
		while (j) {
			if (m_poList[j].m_lKey == key) {
				m_poList[j].m_iValue = value;
				return;
			} else {
				j = m_poList[j].m_iNext;
			}
		}
		
		if (*m_piUsed < *m_piTotal) {
			(*m_piUsed)++;
			j = *m_piHead;
			
			if( *m_piUsed == *m_piTotal )
			{
				*m_piHead = 0;
				*m_piTail = 0;
			}
			else
				*m_piHead = m_poList[j].iIdleNext;
			
			m_poList[j].m_iNext = m_poHash[i];
			m_poHash[i] = j;
			m_poList[j].m_lKey = key;
			m_poList[j].m_iValue = value;
			m_poList[j].iIdleNext = -1;
		} else {
			//##�ռ䲻��
			THROW(MBC_SHMIntHashIndex+1);
		}

	       		
	} 
	else {
		if (*m_piUsed < *m_piTotal) {
			(*m_piUsed)++
			j = *m_piHead;
			m_poHash[i] = j;
			*m_piHead = m_poList[j].m_iNext;
			m_poList[j].m_iNext = 0;
			m_poList[j].m_lKey = key;
			m_poList[j].m_iValue = value;
			
		} 
		else {
			//##�ռ䲻��
			THROW(MBC_SHMIntHashIndex+1);
		}
	}
}

//##ModelId=42442A5B0134
bool RecycleSHMIntHashIndex::get(long key, unsigned int *pvalue)
{
	unsigned int i = (key & (*m_piHashValue));
	unsigned int j;

	if (!(j=m_poHash[i]))
		return false;

	while (j) {
		if (m_poList[j].m_lKey == key) {
			*pvalue = m_poList[j].m_iValue;
			return true;
		}
		
		j = m_poList[j].m_iNext;
	}

	return false;
}

//ɾ��



void RecycleSHMIntHashIndex::reset()
{
	*m_piUsed = 0;
	*m_piHead = 1;
	*m_piTail = *m_piTotal;
    memset(m_poHash,0, (*m_piHashValue+1)*sizeof(unsigned int));
    memset (m_poList, 0, sizeof (RecycleSHMIntList)*(*m_piTotal+1));

    for (int i=*m_piHead; i<*m_piTail; i++) {
        m_poList[i].iNext = i+1;
    }
    m_poList[*m_piTail].iNext = 0;    
}

//##ModelId=42442B02030B
void RecycleSHMIntHashIndex::create(unsigned int itemnumber) 
{
	//##����HASHVALUE
	unsigned int hashvalue;
	hashvalue = (unsigned int)(itemnumber*0.8);
	hashvalue |= 1;
	unsigned int i = 2;

	while (i && (hashvalue > i)) {
		i = (i << 1);
	}

	if (i) {
		hashvalue = i - 1;

		if (hashvalue > 1) {
			hashvalue = (hashvalue >> 1);
		}

	} else {
		hashvalue = ((~0)>>1);
	}

	//##����
	unsigned long size = (hashvalue+1)*sizeof(unsigned int) + \
				(itemnumber+1)*sizeof(RecycleSHMIntList) + \
				6*sizeof(unsigned int);
	SHMAccess::create (size);

	open ();

	*m_piHashValue = hashvalue;
	*m_piTotal = itemnumber;
	*m_piUsed = 0;

	*m_piHead  = 1;
	*m_piTail  = *m_piTotal;

	m_poList = (RecycleSHMIntList *)(m_poHash + (*m_piHashValue)+1);

	for(int ii=*m_piHead;ii<*m_piTail;ii++)
	{
		m_poList[ii].iNext = ii+1;
	}
	m_poList[*m_piTail].iNext = 0;

}

//##ModelId=4244D04101A3
void RecycleSHMIntHashIndex::open()
{
	m_piHashValue = (unsigned int *)((char *)(*m_poSHM)) + 1;
	m_piTotal = m_piHashValue + 1;
	m_piUsed = m_piTotal + 1;

	m_piHead = m_piUsed + 1;
	m_piTail = m_piHead + 1;

	m_poHash = m_piTail + 1;
	m_poList = (RecycleSHMIntList *)(m_poHash + (*m_piHashValue)+1);

}

void RecycleSHMIntHashIndex::empty()
{
  memset (m_poHash, 0, sizeof (unsigned int)*(*m_piHashValue+1));
	memset (m_poList, 0, sizeof (RecycleSHMIntList)*(*m_piTotal+1));
  *m_piUsed = 0;
	*m_piHead = 1;
	*m_piTail = *m_piTotal;
	for(int ii=*m_piHead;ii<*m_piTail;ii++)
	{
		m_poList[ii].iIdleNext = ii+1;
	}
	m_poList[*m_piTail].iIdleNext = 0;
}

//ɾ��
bool RecycleSHMIntHashIndex::erase(long key, unsigned int &value)
{
	unsigned int i = (key & (*m_piHashValue));
	unsigned int j;
	int iPreviousPosition = -1;
	int iCount = 1;

	if (!(j=m_poHash[i]))
		return false;
	while(j)
	{
		if(m_poList[j].m_lKey == key)//��Ԫ��ΪҪɾ����Ԫ��
		{
			value = m_poList[j].m_iValue;
			m_poList[j].m_lKey = 0;
			m_poList[j].m_iValue = 0;

			if( 1 == iCount )//��һ��Ԫ��
			{
				if( 0 == m_poList[j].m_iNext )//ֻ��һ��Ԫ��
				{
					m_poHash[i] = 0;
				}
				else//��������Ԫ��
				{
					m_poHash[i] = m_poList[j].m_iNext;
					m_poList[j].m_iNext = 0;
				}
			}
			else//�ǵ�һ��Ԫ��
			{
				if( m_poList[j].m_iNext != 0 )//�м�Ԫ��
				{
					m_poList[iPreviousPosition].m_iNext = m_poList[j].m_iNext;
					m_poList[j].m_iNext = 0;
				}
				else//���һ��Ԫ��
				{
					m_poList[iPreviousPosition].m_iNext = 0;
				}
			}

			m_poList[j].iIdleNext = 0;

			if( *m_piUsed < *m_piTotal )//�����ռ�δ����
				m_poList[*m_piTail].iIdleNext = j;
			else
				*m_piHead = j;

			*m_piTail = j;

			(*m_piUsed)--;

			return true;
		}
		else
		{
			iCount++;
			iPreviousPosition = j;
			j = m_poList[j].m_iNext;
		}
	}
	return false;
}

