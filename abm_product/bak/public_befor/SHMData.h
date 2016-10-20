/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef SHMDATA_H_HEADER_INCLUDED_BDBB63C4
#define SHMDATA_H_HEADER_INCLUDED_BDBB63C4

#include "HashList.h"
#include "SHMAccess.h"

#define ATTACH_DATA(X, Y, Z) \
        X = new SHMData<Y> (Z); \
        if (!(X)) { \
            freeAll (); \
            THROW(MBC_UserInfo+1); \
        } \
        if (!(X->exist())) { \
            m_bAttached = false; \
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
    SHMData(char *shmname);

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

    //##ModelId=42442167032D
    //##Documentation
    //## ��0Ϊ����ɹ���
    //## 0Ϊ����ʧ��
    unsigned int malloc();
    //##ModelId=4247DADF01DA
    unsigned int getCount();

//modify by jinx for app_guard 070402
	unsigned int getTotal();
  private:
    //##ModelId=4244CD2302FA
    void open();

    //##ModelId=42441A3201CC
    unsigned int *m_piTotal;

    //##ModelId=42441A47006D
    unsigned int *m_piUsed;

    //##ModelId=42442087029F
    T *m_pPointer;

};



//##ModelId=42441AD80198
template <class T>
SHMData<T>::SHMData(char *shmname) :
SHMAccess (shmname)
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

//##ModelId=424420D20076
template <class T>
void SHMData<T>::create(unsigned int itemnumber) 
{
	SHMAccess::create((itemnumber+1)*sizeof(T));
	open ();
	*m_piTotal = itemnumber + 1;
	*m_piUsed = 1;
}

template <class T>
void SHMData<T>::reset()
{
	*m_piUsed = 1;
}

//##ModelId=42442167032D
template <class T>
unsigned int SHMData<T>::malloc()
{
	if (*m_piTotal == *m_piUsed)
		return 0;

	return ((*m_piUsed)++);
}

//##ModelId=4244CD2302FA
template <class T>
void SHMData<T>::open()
{
	m_piTotal = (unsigned *)((char *)(*m_poSHM)) + 1;
	m_piUsed = m_piTotal + 1;
	m_pPointer = (T *)((char *)(*m_poSHM));
}

//##ModelId=4247DADF01DA
template <class T>
unsigned int SHMData<T>::getCount()
{
	if (*m_piUsed)
		return (*m_piUsed)-1;

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


#endif /* SHMDATA_H_HEADER_INCLUDED_BDBB63C4 */
