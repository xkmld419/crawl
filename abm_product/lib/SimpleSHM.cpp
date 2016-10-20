/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "SimpleSHM.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>

#include "MBC.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
//##	ֻ֧��SYSTEM_V


//##ModelId=41DCE78C0207
void SimpleSHM::close()
{
	if (!m_bAttached)
		return;

    if(m_bSHMFile)
        shmdt(m_pPointer);
    else
        munmap(m_pPointer, m_ulSize );
    
	m_bAttached = false;
}

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

//##ModelId=41DCE78F00CB
void SimpleSHM::remove()
{
	if (!m_bAttached)
	{
		//##û�д򿪣�����ȷ�Ŵ��ڣ�
		if (!_open())
		{
			//##������ʲôԭ��
			if (errno != ENOENT)
                if(m_bSHMFile)
				    THROW(MBC_SimpleSHM+1);
            
            if(!m_bSHMFile)
                THROW(MBC_SimpleFile+4);
            
			return;
		}
	}

	//##�ȹ���ɾ
	close ();
    if(m_bSHMFile){
        if (shmctl(m_iSysID, IPC_RMID, 0))
            THROW(MBC_SimpleSHM+2);
    }else{
        if(::remove(m_sFileName.c_str()))
            THROW(MBC_SimpleFile+2);
    }
}

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

//##ModelId=41DCE7CF031B
bool SimpleSHM::exist()
{
	//##����Ѿ�Open��˵���Ѿ�����
	if (m_bAttached)
		return true;

	//##���ܷ�_open�������˵������
	if (_open())
	{
		close ();
		return true;
	}
	else
	{
		//##������ʲô����
		if(m_bSHMFile)
    		if (errno == EACCES)
    			return true;
        
		if(!m_bSHMFile)
            THROW(MBC_SimpleFile+4);
        
		//##��������Ϊ������
		return false;
	}
}

//##ModelId=421DBD860141
SimpleSHM::SimpleSHM(long key, unsigned long size, char * pNeed )
{
	m_pPointer = 0;
	m_pNeedPos = pNeed;
	m_bAttached = false;	
	m_lKey = key;
	m_ulSize = size;
    m_bSHMFile = true;
	
}

SimpleSHM::SimpleSHMPRO(long key, unsigned long size)
{
	m_pPointer = 0;
	m_bAttached = false;	
	m_lKey = key;
	m_ulSize = size;
}

//##ModelId=421DBD8D0259
SimpleSHM::SimpleSHM(char *name, unsigned long size, bool bShm, char * pNeed )
{
	m_pPointer = 0;
	m_pNeedPos = pNeed;
	m_bAttached = false;
	m_ulSize = size;
    m_bSHMFile = bShm;
    if(m_bSHMFile){
        m_lKey = ftok (name, '0');
        if (m_lKey == -1)
            THROW (MBC_SimpleSHM+3);
    }else
        m_sFileName = name;
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


//##ModelId=421DBD93037A
void SimpleSHM::open(bool ifcreate )
{
	if (m_bAttached)
		close ();
	
__OPEN:	
	//##���ܷ�ֱ�Ӵ�
	if (_open())
		return;

	//##�������ֱ�Ӵ򿪣��жϴ�������
	if (errno != ENOENT)
        if(m_bSHMFile)
		    THROW (MBC_SimpleSHM+1);

	//##���ifcreate
	if (!ifcreate)
        if(m_bSHMFile)
		    THROW (MBC_SimpleSHM+4);
        else
            THROW (MBC_SimpleFile+4);

	//##����
	if (!_create())
	{
		if (errno != EEXIST)
            if(m_bSHMFile)
			    THROW (MBC_SimpleSHM+5);
        
        if(!m_bSHMFile)
            THROW (MBC_SimpleFile+5);
	}

	goto __OPEN;
}

//##ModelId=421DBDEC0043
SimpleSHM::operator char *()
{
	return m_pPointer;
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

//##ModelId=41DCE78501E9
bool SimpleSHM::_create()
{
    if(!m_bSHMFile){
        if(m_sFileName.empty())
            return false;
        
        int fd=0;
        
        struct stat FileStat;           //�ļ�����,ȡ�ļ���С���߶Է�
        if(stat(m_sFileName.c_str(),&FileStat)!=0)
            return false;
        
        fd=::open( m_sFileName.c_str(),O_CREAT|O_RDONLY,00777 );
        if(fd==-1)
            return false;
        
        m_ulSize = FileStat.st_size;
        m_pPointer =(char*)mmap(NULL, sizeof(char)*m_ulSize, PROT_READ, MAP_SHARED, fd, 0);
        ::close(fd);
        if(m_pPointer== MAP_FAILED)
            return false;
        
        m_bAttached = true;

	    return true;
    }
    
	m_iSysID = shmget (m_lKey, m_ulSize, SHM_PERMISSION | IPC_CREAT | IPC_EXCL);
	if (m_iSysID == -1)
		return false;
	
	//##Attach
	m_pPointer = (char *)shmat (m_iSysID, m_pNeedPos, SHM_RND);
	if ((long)m_pPointer == -1)
		return false;
    
	m_bAttached = true;

	return true;
}

//##ModelId=41DCE7890299
bool SimpleSHM::_open()
{
	if (m_bAttached) close ();
    
	if(!m_bSHMFile){
        if(_create())
            return true;
        else
            return false;
	}
    
    m_iSysID = shmget (m_lKey, m_ulSize, SHM_PERMISSION | IPC_EXCL);
	if (m_iSysID == -1)
		return false;

	//##Attach
	m_pPointer = (char *)shmat (m_iSysID, m_pNeedPos, SHM_RND);
	if ((long)m_pPointer == -1)
		return false;

	m_bAttached = true;
    if(0 == m_pNeedPos)
        m_pNeedPos = m_pPointer;
    
	return true;
}

#ifdef	_GTEST_DEBUG_
long SimpleSHM::getSysID()
{
	return m_iSysID;
}
#endif

long SimpleSHM::getIPCKey()
{
	 return m_lKey;
}

//##ModelId=4223C4890349
unsigned long SimpleSHM::size() 
{
    if(!m_bSHMFile){
        struct stat FileStat;  //�ļ�����,ȡ�ļ���С���߶Է�
        if(stat(m_sFileName.c_str(), &FileStat)==-1)
            THROW(MBC_SimpleFile+6);
        
        return FileStat.st_size;
    }
    
    struct shmid_ds ds;
	if (shmctl(m_iSysID, IPC_STAT, &ds) == -1)
		THROW(MBC_SimpleSHM+6);
    
    return  ds.shm_segsz;
	///m_ulSize = ds.shm_segsz;

	///return m_ulSize;
}

unsigned long SimpleSHM::getLastSize() 
{
    return m_ulSize;
}

void SimpleSHM::setLastSize(unsigned long ulsize ) 
{
    m_ulSize = ulsize;
    return ; 
}


//##ModelId=4224288D0239
SimpleSHM::~SimpleSHM()
{
	if (m_bAttached)
		close();
}
//##ModelId=42807A2C0370
int SimpleSHM::nattch()
{
    if(!m_bSHMFile)
        return 0;
    
	struct shmid_ds ds;
	if (shmctl(m_iSysID, IPC_STAT, &ds) == -1)
		THROW(MBC_SimpleSHM+6);

	return ds.shm_nattch;
}


unsigned long SimpleSHM::getlba() 
{
    if(!m_bSHMFile)
        return 0;
    
#ifdef SHM_EXPAND_FREE_POS
    if(!m_bAttached)
        return 0;

	struct shmid_ds ds;
	if (shmctl(m_iSysID, SHM_GETLBA, &ds) == -1)
		THROW(MBC_SimpleSHM+6);
		
	return ds.shm_lba;
#endif


#ifdef SHM_EXPAND_GLOBAL_POS
    
    long lPageSize = sysconf(_SC_PAGE_SIZE);
    return (unsigned long)lPageSize;

#endif

    return 1;

}


////������ֻ��������ڵ�����
unsigned long SimpleSHM::expand( unsigned long xSize )
{
    unsigned long ulSize=0;
    unsigned long ulba=0;
    
    if(!m_bSHMFile)
        return 0;
    
    if(0 == xSize || m_iSysID<=0 )
        return 0;

#ifdef SHM_EXPAND_FREE_POS
    struct shmid_ds ds;
	if (shmctl(m_iSysID, IPC_STAT, &ds) == -1){
	    printf("IPC_STAT fail \n");
	    return 0;
	}
    ulSize = ds.shm_segsz;
	if (shmctl(m_iSysID, SHM_GETLBA, &ds) == -1){
	    printf("SHM_GETLBA fail \n");
	    return 0;
	}
	ulba = ds.shm_lba;
	
	long lRest = ulba - (ulSize % ulba);
	if( lRest == ulba)
	    lRest = 0;
	
	unsigned long ltemp = (xSize <= lRest ? xSize : lRest);
	
	if(ltemp){
	    ///ֱ������
	    ds.shm_segsz += ltemp;
    	if (shmctl(m_iSysID, SHM_SIZE, &ds) == -1){
    	    printf("SHM_SIZE fail \n");
    	    return 0;
    	}
        ds.shm_segsz = 0L;
    	if (shmctl(m_iSysID, IPC_STAT, &ds) == -1){
    	    printf("IPC_STAT2 fail \n");
    	    return 0;
    	}
    	if( ds.shm_segsz != ulSize+ltemp){
    	    printf("expand wrong size %lu \n", ds.shm_segsz);
    	    return 0;
    	}
        m_ulSize = ds.shm_segsz;
	}
    return ltemp;   

#endif

#ifdef SHM_EXPAND_GLOBAL_POS
    return 0;
#endif

}

time_t SimpleSHM::getShmTime() 
{
	struct shmid_ds ds;
	
	if (shmctl(m_iSysID, IPC_STAT, &ds) == -1)
		THROW(MBC_SimpleSHM+6);
			
	return ds. shm_ctime; 
}
