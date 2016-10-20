// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "MessageQueueH.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#ifndef NULL
#define NULL 0
#endif

/*
##	���˼·��
##	2��ʹ�÷�ʽ��
##				���죺MessageQueue *p = new MessageQueue("/home/bill");
##				�жϣ�if (!p || !(*p)) throw 1;
##				ʹ��...
##
*/

//##ModelId=4314101D004F
//void open(bool ifblock = true, bool ifcreate = true, int maxlen = 2048, int maxnum = 1000, int buflen = -1 );
int MessageQueue::open(ABMException &oExp, bool ifblock, bool ifcreate)
{
	m_iBlock = ifblock;

	//##����Ѿ�open�ˣ����ٵ���
	if (m_bAttached) return 0;

__OPEN:
	//##û��open���ȿ����ܲ���ֱ��open
	int iRet = _open (oExp);

	if (!iRet) return 0;

	//##������ԭ��
	if (iRet != ENOENT) {
        ADD_EXCEPT0(oExp, "MessageQueue::_open() failed");
        return iRet;
    }

	//##�����Ϣ��������
	if (!ifcreate) {
        ADD_EXCEPT1(oExp, "MessageQueue::_open() failed: MessageQueue 0x%x not exist", m_iKey);
        return iRet;
    }

    oExp.clear ();

	//##���Ŵ���
	iRet = _create (oExp);
	if (!iRet) return 0;

	//##������ԭ��
	if (iRet != EEXIST) {
        ADD_EXCEPT0(oExp, "MessageQueue::_create() failed");
        return iRet;
    }

	goto __OPEN;
}

//##ModelId=4314101D0059
int MessageQueue::remove(ABMException &oExp)
{
	//##���������
	if (!exist()) return 0;

	//##SYSTEM_V����Ĳ���
	int iRet = _open (oExp);

	if (!iRet) {
		//##�쳣����
		if (iRet == ENOENT) goto _TRUE;
		else {
            ADD_EXCEPT0(oExp, "MessageQueue::_open() failed");
            return iRet;
        }
	}

	if (msgctl (m_iSysMQID, IPC_RMID, NULL)) {
		//##�쳣����
		if (errno == EIDRM)
			goto _TRUE;
		else {
            ADD_EXCEPT1(oExp, "msgctl (IPC_RMID) failed, ERRNO: %d", errno);
			return errno;
        }
	}

_TRUE:
	m_bAttached = false;
	return 0;
}

//##ModelId=4314101D005A
void MessageQueue::close()
{
	return;
}

//##ModelId=4314101D005B
bool MessageQueue::exist()
{
	//##����Ѿ�open�ˣ���һ��������
	if (m_bAttached)
		return true;

    ABMException oExp;
	//##�����ֱ�Ӵ򿪣�˵������
	int iRet = _open (oExp);
	if (!iRet) {
		//##���ܹر��Ƿ�ɹ�
		close();
		return true;
	} else {

		if (iRet == EPERM) return true;

		return false;
	}
}

//##ModelId=4314101D0066
int MessageQueue::getMessageNumber()
{
	if (!m_bAttached) {
		return 0;
	}

	struct msqid_ds attr;

	//##��ȡ��Ϣ���������Ϣ
	if (msgctl (m_iSysMQID, IPC_STAT, &attr)) {
		//##�쳣����
		if (errno == ENOENT) m_bAttached = false;

		return 0;
	}

	return attr.msg_qnum;
}

//MessageQueue::MessageQueue(char *mqname)
//##ModelId=4314101D0067
MessageQueue::MessageQueue(int key)
{

	m_iSysMQID = -1;
	m_iBlock = 0;
	m_bAttached = false;

	//##ʮ���Ƶ���ת��ʮ�����Ƶ�����1000->0X1000
	//##����ȥ�Ҷ���

	{
		int iTemp = 0;

		m_iKey = 0;
		while (key) {
			m_iKey |= ((key %10) << iTemp);
			key = key / 10;
			iTemp += 4;
		}
	}

}

//##ModelId=4314101D0069
int MessageQueue::getSysID()
{
	return m_iSysMQID;
}

//##ModelId=4314101D006A
int MessageQueue::getOccurPercent()
{

	if (!m_bAttached) {
		return 0;
	}

	struct msqid_ds attr;

	//##��ȡ��Ϣ���������Ϣ
	if (msgctl (m_iSysMQID, IPC_STAT, &attr)) {
		//##�쳣����
		if (errno == ENOENT)
			m_bAttached = false;

		return -1;
	}

	if (!attr.msg_qbytes) return 0;

	return attr.msg_cbytes * 100 / attr.msg_qbytes;
}

//##ModelId=4314101D006B
MessageQueue::~MessageQueue()
{
	close ();
}

//##ModelId=4314101D006D
int MessageQueue::_open(ABMException &oExp)
{
	if (m_bAttached) return 0;

	//##SYSTEM_V�µĲ���
	m_iSysMQID = msgget (m_iKey, MQ_PERMISSION);  //���ɹ���Ϊ��Ϣ������������������Ϊ-1��
	if (m_iSysMQID == -1) { //##�쳣����
        //ADD_EXCEPT3(oExp, "msgget (0x%x, %o) failed, ERRNO:%d", m_iKey, MQ_PERMISSION, errno);
        //oExp.printErr();
		return errno;
	}

	m_bAttached = true;
	return 0;
}

//##ModelId=4314101D006E
int MessageQueue::_create(ABMException &oExp)
{
	//##SYSTEM_V����Ĳ���
	m_iSysMQID = msgget (m_iKey, MQ_PERMISSION | IPC_CREAT | IPC_EXCL);  //���ɹ���Ϊ��Ϣ������������������Ϊ-1��

	if (m_iSysMQID == -1) {
		//##�쳣����
        ADD_EXCEPT3(oExp, "msgget (0x%x, %o) failed, ERRNO:%d",
                m_iKey, MQ_PERMISSION|IPC_CREAT|IPC_EXCL, errno);
		return errno;
	}


	m_bAttached = true;
	return 0;
}

//##ModelId=4314101D0064
int MessageQueue::Receive(void *buf, int maxLen, long mqtype, bool inter, bool forcenoblock)
{
	//##���û����Ϣ�����ؿ�
	/*
		2005-05-24
		������Ϣ��ʱ�򣬺����ź�������ж�
	*/
	int ret;
    
	__RECEIVE:

	if (forcenoblock)    //�����MSG_NOERROR�Ƿ���Ҫ, ������ջ���С����Ϣ���� ��ض� 20110401 DELETED BY huff 
		ret = msgrcv (m_iSysMQID, buf, maxLen, mqtype, IPC_NOWAIT);
	else 
		ret = msgrcv (m_iSysMQID, buf, maxLen, mqtype, (m_iBlock ? 0 : IPC_NOWAIT));

    if (ret < 0) {
		if (errno == ENOMSG) 
		    return 0;
		else if (errno == EINTR) {
		    if (inter) 
				return -2;
            else
		    	goto __RECEIVE;
		}		
		else 
		    return -1;
	}

	return ret+sizeof(long);
}


/*
*   ��������: ���׵�ַΪvoid *buf, ����Ϊlen�����ݷ�����Ϣ����
*   ����:
*       void *data : ʵ�ʴ�������Ϊstruct my_msgbuf { mtyp_t mtype;
*                                                     char  mtext[MSGSIZ]; }
*       int len: ���������ݵ��ܳ���, ����sizeof(mtyp_t)��ͷ
*       bool &inter: ��������ʱ, ���ź��ж��Ƿ񷵻�, inter==true ����-2
*		bool forcenoblock: ����ʱǿ�Ʒ�����
*   ����: �ɹ����ط��ͳ���, ʧ�ܷ���-1 or -2
*/
int MessageQueue::Send(void *buf, int len, bool inter, bool forcenoblock)
{
	/*
	*  ��������� struct my_msgbuf
	*  struct my_msgbuf { mtyp_t mtype;
    *                     char   mtext[MSGSIZ]; } buf;
	*/
	int ret;
	int _len = len - sizeof(long);
	
	if (*(long *)buf == 0) 
	    *(long *)buf = 9999;    
	
__SEND1:
	
	if (forcenoblock)
		ret = msgsnd (m_iSysMQID, buf, _len, IPC_NOWAIT);
	else	 
		ret = msgsnd (m_iSysMQID, buf, _len, (m_iBlock ? 0 : IPC_NOWAIT));

	if (ret == -1) {
		
	    switch (errno) {
	    case ENOMEM:
			return 0;
	    case EAGAIN:
	        return 0;
	    case EINTR:
	        if (inter)  
	        	return -2;
	        else	
	        	goto __SEND1;
	    default:
	        return -1;
	    }
	}

	return len;
}
