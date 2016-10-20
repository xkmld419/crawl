/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef SHMDATA_H_HEADER_INCLUDED_BDBB63C4
#define SHMDATA_H_HEADER_INCLUDED_BDBB63C4

#include "HashList.h"
#include "SHMAccess.h"

#define ATTACH_DATA(X, Y, Z) \
	if (Z==0) { \
	THROW(MBC_UserInfo+1); \
	} \
	X = new SHMData<Y> (Z); \
	if (!(X)) { \
	freeAll (); \
	THROW(MBC_UserInfo+1); \
	} \
	if (!(X->exist())) { \
	m_bAttached = false; \
	printf("%s(%d): ATTCH_DATA failed (key:%ld)\n",__FILE__,__LINE__,(long)Z); \
	}


//##ModelId=424418EA006B
//##Documentation
//## �����ڴ�������ģ���࣬Ҫ���ŵ����ݵ�
//## ÿ��Ԫ�صĴ�С��С��8���ֽڣ����Ԫ�ص�
//## ��СС��8���ֽڣ����������
template <class T>
class SHMData : public SHMAccess
{
public:
	typedef T value_type;

	//##ModelId=42441AD80198
	SHMData(char *shmname, bool bShm = true);

	//##ModelId=42441D7D0001
	SHMData(long shmkey);

	//##ModelId=42441AD801E8
	~SHMData();

	//##ModelId=4244200C0125
	operator T *();

	//##ModelId=424420D20076
	//##Documentation
	//## ����������
	void create(
		//##Documentation
		//## ���Ԫ�ظ���
		unsigned int itemnumber) ;

	void reset();

	void clear();

	//##ModelId=42442167032D
	//##Documentation
	//## ��0Ϊ����ɹ���
	//## 0Ϊ����ʧ��
	unsigned int malloc();

	//##ModelId=4247DADF01DA
	unsigned int erase(unsigned int i);

	unsigned int getCount();

	void	setCount(unsigned int iCount);

	//modify by jinx for app_guard 070402
	unsigned int getTotal();

	time_t getLoadTime();

	unsigned  int  getMemSize();

	unsigned  long getRealSize();

	unsigned  int  getUsedSize();

	unsigned int getHead();//�õ�ͷ���

	unsigned int getTail();//�õ�β�ӵ�

	int *m_piIdleNext;                //��־ĳ���ڵ��Ƿ�Ϊ���нڵ�

	unsigned long   getMemAdress();

private:
	//##ModelId=4244CD2302FA
	void open();

	unsigned int countChange(unsigned int count);

	//##ModelId=42441A3201CC
	unsigned int *m_piTotal;

	//##ModelId=42441A47006D
	unsigned int *m_piUsed;

	unsigned int *m_piHead;//ͷ���

	unsigned int *m_piTail;//β�ӵ�

	//##ModelId=42442087029F
	T *m_pPointer;

};

//##ModelId=42441AD80198
template <class T>
SHMData<T>::SHMData(char *shmname, bool bShm) :
SHMAccess (shmname, bShm)
{
	if (m_poSHM)
		open ();
}


//##ModelId=42441D7D0001
template <class T>
SHMData<T>::SHMData(long shmkey) :
SHMAccess (shmkey)
{
	if (m_poSHM)
		open ();
}


//##ModelId=42441AD801E8
template <class T>
SHMData<T>::~SHMData()
{
}

//##ModelId=4244200C0125
template <class T>
SHMData<T>::operator T *()
{
	return m_pPointer;
}

/*
*�������ܣ��洢�ڵ����ת��
*���������countΪ�������ݿ��B_PARAM_DEFINE�����õĲ�����
*/
template <class T>
unsigned int SHMData<T>::countChange(unsigned int count)
{
	//cout<<sizeof(T)<<endl;
	// 	unsigned int uiSize = count*(sizeof(T)+4);
	// 	uiSize += 20;
	// 	uiSize = uiSize/sizeof(T);
	// 	//return (((count*(sizeof(T)+4)+20)/sizeof(T)));
	// 	return uiSize;
	if ((20+4*(count+1))%sizeof(T) == 0){
		return ((20+4*(count+1))/sizeof(T)+count+1);
	} else {
		return (((20+4*(count+1))/sizeof(T)+1)+count+1);
	}
}

//##ModelId=4244CD2302FA
template <class T>
void SHMData<T>::open()
{
	m_piTotal = (unsigned *)((char *)(*m_poSHM)) + 1;
	m_piUsed = m_piTotal + 1;
	m_piHead = m_piUsed + 1;
	m_piTail = m_piHead + 1;
	m_piIdleNext =(int *)m_piTail + 1;

	m_pPointer = (T *)(m_piIdleNext+(*m_piTotal)+1);
	//m_pPointer = (T *)((char *)(*m_poSHM)+20+(*m_piTotal)*4);

}

//##ModelId=424420D20076
template <class T>
void SHMData<T>::create(unsigned int itemnumber)                                                                                                                   
{
	unsigned long ulItemnum = 0;
	ulItemnum = countChange(itemnumber);
	SHMAccess::create(ulItemnum*(sizeof(T)));//���乲���ڴ沢��ʼ��

	open ();
	//printf("sizeof(T) = %d\n",sizeof(T));
	//ulItemnum = (itemnumber+1)*sizeof(T);
	*m_piTotal = itemnumber;      //ʵ�ʿ��õĽڵ����
	*m_piUsed  = 0;
	*m_piHead  = 1;
	*m_piTail  = *m_piTotal;

	m_pPointer = (T *)(m_piIdleNext+(*m_piTotal)+1);

	for(int i=*m_piHead;i<*m_piTail;i++){
		m_piIdleNext[i] = i+1;
	}
	m_piIdleNext[*m_piTail] = 0;
}

template <class T>
void SHMData<T>::reset()
{
	*m_piUsed = 0;	
	*m_piHead = 1;
	*m_piTail = *m_piTotal;
	for(int i=*m_piHead;i<*m_piTail;i++){
        m_piIdleNext[i] = i+1;
    }
    m_piIdleNext[*m_piTail] = 0;
}

template <class T>
void SHMData<T>::clear()
{
	*m_piUsed = 0;	
	*m_piHead = 1;
	*m_piTail = *m_piTotal;
	for(int i=*m_piHead;i<*m_piTail;i++){
		m_piIdleNext[i] = i+1;
	}
	m_piIdleNext[*m_piTail] = 0;

	memset(m_pPointer,0,*m_piTotal*sizeof(T));
}

//##ModelId=42442167032D
template <class T>
unsigned int SHMData<T>::malloc()
{
	if (*m_piUsed == *m_piTotal)
		return 0;

	(*m_piUsed)++;
	int iResult = *m_piHead;

	if( *m_piUsed == *m_piTotal )
	{
		*m_piHead = 0;
		*m_piTail = 0;
	}
	else
		*m_piHead = m_piIdleNext[iResult];
	m_piIdleNext[iResult] = -1;                                //��ʾ�˽ڵ��Ѿ���ʹ��
	return (iResult);
}

template <class T>
unsigned int SHMData<T>::erase(unsigned int i)
{
	m_piIdleNext[i] = 0;
	if( *m_piUsed < *m_piTotal )//�ռ�δ����
		m_piIdleNext[*m_piTail] = i;
	else
		*m_piHead = i;
	*m_piTail = i;
	(*m_piUsed)--;
	return 1;
}

//���Ĳ�����Ҫ�õ��������ݴ�С
template <class T>
void SHMData<T>::setCount(unsigned int iCount)
{
	if(*m_piTotal<iCount)
		return ;
	(*m_piUsed)=iCount;
}

//##ModelId=4247DADF01DA
template <class T>
unsigned int SHMData<T>::getCount()
{
	if (*m_piUsed)
		return (*m_piUsed);

	return 0;
}

//modify by jinx for app_guard 070402
template <class T>
unsigned int SHMData<T>::getTotal()
{
	if (*m_piTotal)
		return (*m_piTotal)-1;

	return 0;	
}
template <class T>
unsigned int SHMData<T>::getMemSize()
{
	if (*m_piTotal)
		return (*m_piTotal)* sizeof(T);

	return 0;
}
template <class T>
unsigned int SHMData<T>::getUsedSize()
{
	if (*m_piUsed)
		return (*m_piUsed)* sizeof(T);

	return 0;
}

template <class T>
unsigned long SHMData<T>::getMemAdress()
{		
	if (m_poSHM)
	{
		return   (unsigned long)m_poSHM;
	}

	return 0;
}

template <class T>
time_t SHMData<T>::getLoadTime()
{
	time_t shm_ctime;

	if (m_poSHM)
	{
		shm_ctime = m_poSHM->getShmTime();	

		return shm_ctime ;
	}
	return  0;
}

template <class T>
unsigned long SHMData<T>::getRealSize()
{
	unsigned long size = m_poSHM->size();
	return size;

}

#endif /* SHMDATA_H_HEADER_INCLUDED_BDBB63C4 */
