/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "SimpleSHM.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include "MBC.h"
//##	ֻ֧��SYSTEM_V

//##ModelId=41DCE78C0207
void SimpleSHM::close()
{
	if (!m_bAttached) return;

	shmdt(m_pPointer);
	m_bAttached = false;
}

//##ModelId=41DCE78F00CB
int SimpleSHM::remove(ABMException &oExp)
{
	if (!m_bAttached) {
		if (!_open(oExp, 0644)) { //##û�д򿪣�����ȷ�Ŵ��ڣ� 
			if (errno != ENOENT) {
                ADD_EXCEPT0(oExp, "SimpleSHM::_open(0644) failed");
				return errno;
            }

            oExp.clear ();
			return 0;
		}
	}

	close (); //##�ȹ���ɾ

	if (shmctl(m_iSysID, IPC_RMID, 0)) {
        ADD_EXCEPT2(oExp, "shmctl(%d, IPCRMID, 0) failed, ERRNO:%d", m_iSysID, errno);
        return errno;
    } 

    return 0;
}

//##ModelId=41DCE7CF031B
bool SimpleSHM::exist(ABMException &oExp)
{
	//##����Ѿ�Open��˵���Ѿ�����
	if (m_bAttached) return true;

	//##���ܷ�_open�������˵������
	if (_open(oExp, 0644)) {
		close (); return true;
	} else {
        oExp.clear ();

		//##������ʲô����
		if (errno == EACCES) 
            return true;
		
		//##��������Ϊ������
		return false;
	}
}

//##ModelId=421DBD860141
SimpleSHM::SimpleSHM(long key, unsigned long size)
{
	m_pPointer = 0;
	m_bAttached = false;	
	m_lKey = key;
	m_ulSize = size;
}

//##ModelId=421DBD93037A
int SimpleSHM::open(ABMException &oExp, bool ifcreate, unsigned int iPermission)
{
	if (m_bAttached) close ();
	
__OPEN:	
	//##���ܷ�ֱ�Ӵ�
	if (_open(oExp, iPermission)) return 0;

	//##�������ֱ�Ӵ򿪣��жϴ�������
	if (errno != ENOENT) {
        ADD_EXCEPT1(oExp, "SimpleSHM::_open(%o) failed", iPermission);
        return errno;
    }

	//##���ifcreate
	if (!ifcreate) {
        ADD_EXCEPT1(oExp, "SimpleSHM::_open() failed, share memory 0x%x not exist", m_lKey);
        return -1;
    }

	//##����
	if (!_create(oExp, iPermission)) {
		if (errno != EEXIST) {
            ADD_EXCEPT1(oExp, "SimpleSHM::_create(%o) failed", iPermission);
            return errno;
        }

        return 0;
	}

	goto __OPEN;
}

//##ModelId=41DCE78501E9
bool SimpleSHM::_create(ABMException &oExp, unsigned int iPermission)
{
	m_iSysID = shmget (m_lKey, m_ulSize, iPermission | IPC_CREAT | IPC_EXCL);
	if (m_iSysID == -1) {
        ADD_EXCEPT3(oExp, "shmget(0x%x, %ld, IPC_CREAT|IPC_EXCL) failed, ERRNO:%d", 
                m_lKey, m_ulSize, errno);
        return false;
    }
	
	//##Attach
	m_pPointer = (char *)shmat (m_iSysID, 0, 0);
	if ((long)m_pPointer == -1) {
        ADD_EXCEPT2(oExp, "shmmat(0x%x, 0, 0) ailed, ERRNO:%d", m_iSysID, errno);
        return false;
    }

	m_bAttached = true;

	return true;
}

//##ModelId=41DCE7890299
bool SimpleSHM::_open(ABMException &oExp, unsigned int iPermission)
{
	if (m_bAttached) close ();
	
    //IPC_EXCL������ԭ�е�(���ԭ��û�оͷ���-1)��IPC_CREAT�����½�������ԭ�е�(���ԭ��û�о��½����������ԭ�е�)
	m_iSysID = shmget (m_lKey, m_ulSize, iPermission | IPC_EXCL);
	if (m_iSysID == -1) {
        ADD_EXCEPT3(oExp, "shmget(0x%x, %ld, IPC_EXCL) failed, ERRNO:%d", 
                m_lKey, m_ulSize, errno);
		return false;
    }

	//##Attach
	m_pPointer = (char *)shmat (m_iSysID, 0, 0);
	if ((long)m_pPointer == -1) {
        ADD_EXCEPT2(oExp, "shmmat(0x%x, 0, 0) ailed, ERRNO:%d", m_iSysID, errno);
		return false;
    }

	m_bAttached = true;

	return true;
}
//##ModelId=4223C4890349
unsigned long SimpleSHM::size() 
{
	struct shmid_ds ds;
	if (shmctl(m_iSysID, IPC_STAT, &ds) == -1) return 0;
	m_ulSize = ds.shm_segsz;

	return m_ulSize;
}

//##ModelId=4224288D0239
SimpleSHM::~SimpleSHM()
{
	if (m_bAttached) close();
}
//##ModelId=42807A2C0370
int SimpleSHM::nattch()
{
	struct shmid_ds ds;
	if (shmctl(m_iSysID, IPC_STAT, &ds) == -1)
         return -1;

	return ds.shm_nattch;
}

