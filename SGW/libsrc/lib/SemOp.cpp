#ifndef _DEFINE_INCLUDE_SEMOP_H_
#define _DEFINE_INCLUDE_SEMOP_H_

#include "SemOp.h"

SemOp::SemOp(key_t keyvalue, int maxval)
{
	semkey = keyvalue;
	iMaxSemVal = maxval;
	bIfWait = true;
}

SemOp::~SemOp()
{

}

int SemOp::initSem(int value, bool rCreate)
{
	union semun su;

	su.val = value;

	iSemID = semget(semkey, 1, 0666|IPC_CREAT);
	if (iSemID == -1)
	{
		cout << "errno = " << errno << "; errinfo=: " << strerror(errno) << endl;
		cout << "append err file name is:" << __FILE__ << "; line no is:" << __LINE__ << endl;
		return -1;
	}

/*
	//Ҫô�����´������ź������ı�ʶ����
	//Ҫô����ϵͳ���Ѿ����ڵ�ͬ���Ĺؼ���ֵ���ź����ı�ʶ��
	if (semget(semkey, 1, IPC_CREAT) != -1)
*/
	//IPC_EXCL��IPC_CREATһͬʹ�ã���Ҫô�����´������ź������ı�ʶ����Ҫô����-1��
	//IPC_EXCL����ʹ��û������
	if (rCreate)
	{
		if (semctl(iSemID, 0, IPC_RMID, 0) == -1)  //ɾ���ź���
		{
			cout << "errno = " << errno << "; errinfo=: " << strerror(errno) << endl;
			cout << "append err file name is:" << __FILE__ << "; line no is:" << __LINE__ << endl;
			return -1;
		}

		iSemID = semget(semkey, 1, 0666|IPC_EXCL| IPC_CREAT);
		if (iSemID == -1)
		{
			cout << "errno = " << errno << "; errinfo=: " << strerror(errno) << endl;
			cout << "append err file name is:" << __FILE__ << "; line no is:" << __LINE__ << endl;
			return -1;
		}
	}

	if (semctl(iSemID, 0, SETVAL, su) == -1)
	{
		cout << "errno = " << errno << "; errinfo=: " << strerror(errno) << endl;
		cout << "append err file name is:" << __FILE__ << "; line no is:" << __LINE__ << endl;
		return -1;
	}

	return 0;
}

int SemOp::attachSem ()
{
	iSemID = semget(semkey, 1, 0666);
	if (iSemID == -1)
	{
		cout << "errno = " << errno << "; errinfo=: " << strerror(errno) << endl;
		cout << "append err file name is:" << __FILE__ << "; line no is:" << __LINE__ << endl;
		return -1;
	}
	return 0;
}

int SemOp::destroySem()
{

	iSemID = semget(semkey, 1, 0660|IPC_CREAT);
	if (iSemID == -1)
	{
		cout << "errno = " << errno << "; errinfo=: " << strerror(errno) << endl;
		cout << "append err file name is:" << __FILE__ << "; line no is:" << __LINE__ << endl;
		return -1;
	}

	if (semctl(iSemID, 0, IPC_RMID, 0) == -1)  //ɾ���ź���
	{
		cout << "errno = " << errno << "; errinfo=: " << strerror(errno) << endl;
		cout << "append err file name is:" << __FILE__ << "; line no is:" << __LINE__ << endl;
		return -1;
	}

	return 0;
}

int SemOp::P()
{
//	struct sembuf buf_p = {0, -1, SEM_UNDO};
	struct sembuf buf_p;
	buf_p.sem_num = 0;
	buf_p.sem_op = -1;
	if (bIfWait)
	{
		buf_p.sem_flg = SEM_UNDO;
	}
	else
	{
		buf_p.sem_flg = IPC_NOWAIT|SEM_UNDO;
	}
	if (semop (iSemID, &buf_p, 1) == -1)
	{
		if (errno == 11)
		{
			return 1;	//�ź�������
		}
		else
		{
			cout << "errno = " << errno << "; errinfo=: " << strerror(errno) << endl;
			cout << "append err file name is:" << __FILE__ << "; line no is:" << __LINE__ << endl;
			return -1;
		}
	}
//	union semun su;
//	cout << "�ź���ֵΪ��" << semctl(iSemID, 0, GETVAL, su) << endl;
	return 0;
}

int SemOp::V()
{
	struct sembuf buf_p = {0, 1, SEM_UNDO};
	union semun su;

	if (semctl(iSemID, 0, GETVAL, su) >= iMaxSemVal)
	{
		cout << "sem is max val [" << semctl(iSemID, 0, GETVAL, su) << "]" << endl;
		setVal(iMaxSemVal);
		usleep(10000);
		return 0;
	}

	if (semop (iSemID, &buf_p, 1) == -1)
	{
		cout << "�ź���ֵΪ��" << semctl(iSemID, 0, GETVAL, su) << endl;

		if (errno == 34)  // Numerical result out of range
		{
			cout << "sem is max val [" << semctl(iSemID, 0, GETVAL, su) << "]" << endl;
			setVal(iMaxSemVal);
			usleep(10000);
			return 0;
		}
		else
		{
			cout << "errno = " << errno << "; errinfo=: " << strerror(errno) << endl;
			cout << "append err file name is:" << __FILE__ << "; line no is:" << __LINE__ << endl;
			return -1;
		}
	}
//	cout << "�ź���ֵΪ��" << semctl(iSemID, 0, GETVAL, su) << endl;

	return 0;
}

void SemOp::setWait(bool wait)
{
	bIfWait = wait;
}

int SemOp::setVal(int val)
{
	union semun su;
	su.val = val;

	if (semctl(iSemID, 0, SETVAL, su) == -1)
	{
		cout << "errno = " << errno << "; errinfo=: " << strerror(errno) << endl;
		cout << "append err file name is:" << __FILE__ << "; line no is:" << __LINE__ << endl;
		return -1;
	}
	return 0;
}

#endif

