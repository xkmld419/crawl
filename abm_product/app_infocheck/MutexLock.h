/**********************************************************************
���ʤ
���żƷ��з���
2010.07.16
��������
**********************************************************************/
#ifndef CMUTEXTLOCK_H_
#define CMUTEXTLOCK_H_
#include <pthread.h>
#include <iostream>
using namespace std;
class CmutexLock
{
public:
    CmutexLock();
    ~CmutexLock();
    void lock();
    void unlock();
protected:
private:
    pthread_mutex_t m_mutexLock;
};
#endif

