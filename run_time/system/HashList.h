// Copyright (c) 2005,联创科技股份有限公司电信事业部
// All rights reserved.

#ifndef HASHLIST_H_HEADER_INCLUDED_BDD83461
#define HASHLIST_H_HEADER_INCLUDED_BDD83461

//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include "MBC.h"
#include "KeyList.h"

//##ModelId=41DB839500D0
template <class T>
class HashList
{
  public:
	friend class Iteration;

	class Iteration
	{
	  public:
		bool next(T &value)
		{
			while (!m_poCur && m_iHashIndex<m_poHashList->m_iHashValue) {
				m_poCur = m_poHashList->m_poKeyList[m_iHashIndex];
				m_iHashIndex++;
			}

			if (!m_poCur) return false;

			value = m_poCur->m_oData;
			m_poCur = m_poCur->m_poNext;
			return true;		
		}

		Iteration (int i, KeyList<T> *p1, HashList<T> *p2):
		m_iHashIndex(i), m_poCur(p1), m_poHashList(p2)
		{}

		Iteration (const Iteration & right)
		{
			m_iHashIndex = right.m_iHashIndex;
			m_poCur = right.m_poCur;
			m_poHashList = right.m_poHashList;
		}

	  private:
		unsigned int m_iHashIndex;
		KeyList<T> * m_poCur;
		HashList<T> * m_poHashList;
	};


	Iteration getIteration ()
	{
		return Iteration (0, m_poKeyList[0], this);
	}	
    //##ModelId=41DCE934027F
    //flag = true 如果Key相同，后者覆盖前者
    //flag = false 如果Key相同，后者不覆盖前者
    void add(long key, T data, bool flag = true) ;
    
    
    int getKeyList(long key, KeyList<T> *&keyList);

    //##ModelId=41DCE9340293
    bool get(long key, T *value) ;

    //##ModelId=41DCE93402B1
    void destroy();
    
    void reset();

    //##ModelId=4227CC0B039F
    HashList(unsigned int hashvalue);

    //##ModelId=4227D8560390
    ~HashList();

    //##ModelId=4227D88103A6
    void add(char *key, T data) ;

    //##ModelId=4227D88D00AA
    bool get(char *key, T *value) ;

  private:
    //##ModelId=4227D80B016C
    long calKey(long key);

    //##ModelId=4227D81903A7
    int calKey(char *key);

    //##ModelId=4227C4CA009C
    //##Documentation
    //## 数组的维数
    unsigned int m_iHashValue;

    //##ModelId=4227C50A0260
    unsigned int m_iCount;

    //##ModelId=4227CBC301BB
    KeyList<T> **m_poKeyList;

};


//##ModelId=41DCE934027F
template <class T>
void HashList<T>::add(long key, T data, bool flag) 
{
	long i = calKey (key);
	if (m_poKeyList[i]) {
		m_poKeyList[i]->add (key, data, flag);
	} else {
		m_poKeyList[i] = new KeyList<T> (key, data);
		if (!m_poKeyList[i])
			THROW(MBC_HashList+1);
	}
}


template <class T>
int HashList<T>::getKeyList(long key, KeyList<T> *&keyList)
{
    long i = calKey (key);
    if(m_poKeyList[i] != NULL) {
        keyList = m_poKeyList[i];
        return 0;
    } else {
        return -1;
    }
}
//##ModelId=41DCE9340293
template <class T>
bool HashList<T>::get(long key, T *value) 
{
	long i = calKey (key);
	if (m_poKeyList[i]) {
		return m_poKeyList[i]->get (key, value);
	} else {
		return false;
	}

}
//##ModelId=41DCE93402B1
template <class T>
void HashList<T>::reset()
{
	unsigned int i;
	if (!m_poKeyList)
		return;
    
	for (i=0; i<m_iHashValue; i++) {
		if (m_poKeyList[i]) {
			delete m_poKeyList[i];
			m_poKeyList[i] = 0;
		}
	}

}
//##ModelId=41DCE93402B1
template <class T>
void HashList<T>::destroy()
{
	unsigned int i;
	if (!m_poKeyList)
		return;
    
	for (i=0; i<m_iHashValue; i++) {
		if (m_poKeyList[i]) {
			delete m_poKeyList[i];
			m_poKeyList[i] = 0;
		}
	}

	delete [] m_poKeyList;
	m_poKeyList = 0;
}

//##ModelId=4227CC0B039F
template <class T>
HashList<T>::HashList(unsigned int hashvalue) : m_iHashValue(hashvalue)
{
	if (!hashvalue)
		THROW(MBC_HashList+2);

	m_poKeyList = new KeyList<T> * [hashvalue];
	if (!m_poKeyList)
		THROW(MBC_HashList+1);

	memset (m_poKeyList, 0, sizeof(KeyList<T> *) * hashvalue);
}

//##ModelId=4227D8560390
template <class T>
HashList<T>::~HashList()
{
	destroy ();
}

//##ModelId=4227D88103A6
template <class T>
void HashList<T>::add(char *key, T data) 
{	
	int i = calKey (key);
	if (m_poKeyList[i]) {
		m_poKeyList[i]->add (key, data);
	} else {
		m_poKeyList[i] = new KeyList<T> (key, data);
		if (!m_poKeyList[i])
			THROW(MBC_HashList+1);
	}
}

//##ModelId=4227D88D00AA
template <class T>
bool HashList<T>::get(char *key, T *value) 
{
	int i = calKey (key);
	if (m_poKeyList[i]) {
		return m_poKeyList[i]->get (key, value);
	} else {
		return false;
	}
}

//##ModelId=4227D80B016C
template <class T>
long HashList<T>::calKey(long key)
{
	return key % m_iHashValue;
}

//##ModelId=4227D81903A7
template <class T>
int HashList<T>::calKey(char *key)
{
	long i ,j;
	
	i = 0;
	for (j = 1; *key && (j<13); j++)
		i += ((*key) * j); /* ascii char times its 1-based index */
	
	return i % m_iHashValue;
}

#endif /* HASHLIST_H_HEADER_INCLUDED_BDD83461 */
