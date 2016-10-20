// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

/* #ifdef DEF_HP
#include "/usr/include/../old/usr/include/curses.h"
#else
#include <curses.h>
#endif
*/
#include "MessageQueue.h"
#include "Publicfunction.h"

#ifdef DEF_HP
#include <sys/mqueue.h>
#endif


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "platform.h"
#include "Exception.h"
#include "Log.h"

#include "MBC.h"

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
void MessageQueue::open(bool ifblock, bool ifcreate, int maxlen, int maxnum, int buflen)
{
	m_iBlock = ifblock;
	m_iMaxNum = maxnum;
	m_iMaxLen = maxlen;
	if (buflen==-1)
		m_iBufLen = maxlen;
	else
		m_iBufLen = buflen;

	if (m_iBufLen>maxlen*maxnum || m_iBufLen<maxlen)
		THROW(MBC_MessageQueue+701);

	//##����Ѿ�open�ˣ����ٵ���
	if (m_bAttached)
		return;

__OPEN:
	//##û��open���ȿ����ܲ���ֱ��open
	if (_open())
		return;

	//##������ԭ��
	if (m_iErrorNo != E_NOENT)
		THROW(MBC_MessageQueue+100+m_iErrorNo);

	//##�����Ϣ��������
	if (!ifcreate)
		THROW(MBC_MessageQueue+100+E_NOENT);

	//##���Ŵ���
	if (_create())
		return;

	//##������ԭ��
	if (m_iErrorNo != E_EXIST)
		THROW(MBC_MessageQueue+600+m_iErrorNo);

	goto __OPEN;
}

//##ModelId=4314101D0059
void MessageQueue::remove()
{
	//##���������
	if (!exist())
		return;

	//##�����Ϣ���д��ڣ�����ɾ��
	#ifdef SYSTEM_V
	{
		//##SYSTEM_V����Ĳ���
		if (!_open())
		{
			//##�쳣����
			if (m_iErrorNo == E_NOENT)
				goto _TRUE;
			else
				THROW(MBC_MessageQueue+400+m_iErrorNo);
		}

		if (msgctl (m_iSysMQID, IPC_RMID, NULL))
		{
			//##�쳣����
			if (errno == EIDRM)
				goto _TRUE;
			else
				THROW(MBC_MessageQueue+500+m_iErrorNo);
		}

		goto _TRUE;
	}
	#else
	{
		//##POSIX����Ĳ���
		close();

		if (mq_unlink (m_sMQName))
		{
			//##�쳣����
			if (errno == ENOENT)
				goto _TRUE;
			else
				THROW(MBC_MessageQueue+500+m_iErrorNo);
		}
	}
	#endif


_TRUE:
	m_bAttached = false;
	return;
}

//##ModelId=4314101D005A
void MessageQueue::close()
{
	//##������û�д�
	if (!m_bAttached)
		return;

	//##�Ͽ�����Ϣ���е����ӣ���ɾ����Ϣ���У�
	#ifdef SYSTEM_V
		//##����Ҫ�رգ����þ�����
	#else
	{
		if (mq_close (m_iSysMQID))
		{
			//##�쳣����
			m_iErrorNo = _getStandardError(errno);
			if (m_iErrorNo == E_NOENT)
			{
				m_bAttached = false;
				return;
			}

			THROW(MBC_MessageQueue+400+m_iErrorNo);
		}

		m_bAttached = false;
	}
	#endif

	return;
}

//##ModelId=4314101D005B
bool MessageQueue::exist()
{
	//##����Ѿ�open�ˣ���һ��������
	if (m_bAttached)
		return true;

	//##�����ֱ�Ӵ򿪣�˵������
	if (_open())
	{
		//##���ܹر��Ƿ�ɹ�
		close();
		return true;
	}
	else
	{
		if (m_iErrorNo == E_PERM)
			return true;

		return false;
	}
}

//##ModelId=4314101D0061
int MessageQueue::send(void *data, int len,long lMtype)
{
	//##�����ʵ�ֲ��������ȼ�
	int ret;

	if (!len)
		return 0;

	#ifdef SYSTEM_V

		if (len>m_iBufLen)
			throw(MBC_MessageQueue+702);

		void *p = m_pBuff;

		if (lMtype==0)
			*((long *)p) = len; // modified by linjinxin
		else
			*((long *)p) = lMtype;

		memcpy((char *)p+sizeof(long), data, len);

		ret = msgsnd (m_iSysMQID, p, len, (m_iBlock ? 0 : IPC_NOWAIT));
	#else
		ret = mq_send (m_iSysMQID, (char *)data, len, 1);
	#endif

	if (ret == -1)
	{
		//##�쳣����
		m_iErrorNo = _getStandardError (errno);

		if (m_iErrorNo == E_NOMSG)
			m_iErrorNo = E_FULL;//POSIX

		if (m_iErrorNo == E_FULL)
			return 0;

		if (m_iErrorNo == E_NOENT)
			m_bAttached = false;

		THROW(MBC_MessageQueue+200+m_iErrorNo);
//		return -1;
	}

	return len;
}

//##ModelId=4314101D0064
int MessageQueue::receive(char *buf,long lMtype)
{
	//##���û����Ϣ�����ؿ�
	/*
		2005-05-24
		������Ϣ��ʱ�򣬺����ź�������ж�
	*/
	int ret;
	int len;

	#ifdef SYSTEM_V
//		memset ((char *)m_pBuff, 0, m_iBufLen+sizeof(long));
		
		__RECEIVE:

		//ret = msgrcv (m_iSysMQID, m_pBuff, m_iBufLen, 0, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));
		ret = msgrcv (m_iSysMQID, m_pBuff, m_iBufLen, lMtype, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));

		/*
			  If msgtyp is 0, then the first message in the queue is read.

              If msgtyp is greater than 0, then the first message on the queue
              of  type  msgtyp  is  read,  unless  MSG_EXCEPT  was asserted in
              msgflg, in which case the first message on the queue of type not
              equal to msgtyp will be read.

              If  msgtyp  is  less than 0, then the first message on the queue
              with the lowest type less than or equal to the absolute value of
              msgtyp will be read.

		*/

		if (ret >=0)
		{
			memcpy (buf, (char *)m_pBuff+sizeof(long), ret);
		}
		else
		{
			m_iErrorNo = _getStandardError (errno);

			if (m_iErrorNo == E_NOMSG)
				return 0;

			if (m_iErrorNo == E_NOENT)
				m_bAttached = false;

			if (m_iErrorNo == E_INTR) {
				Log::log (1000, "������Ϣ�������յ��źţ������ź�...");
				goto __RECEIVE;
			}

			THROW(MBC_MessageQueue+300+m_iErrorNo);
		}

		return ret;
	#else
		/*struct mq_attr attr;
		ret = mq_getattr (m_iSysMQID, &attr);
		if (ret == -1)
		{
			m_iErrorNo = _getStandardError (errno);
			return -1;
		}
		*/

		__RECEIVE:
		ret= mq_receive (m_iSysMQID, buf, /*attr.mq_msgsize*/m_iMaxLen, NULL);

		if (ret == -1)
		{
			m_iErrorNo = _getStandardError (errno);
			if (m_iErrorNo == E_NOMSG)
				return 0;

			if (m_iErrorNo == E_INTR) {
				Log::log (1000, "������Ϣ�������յ��źţ������ź�...");
				goto __RECEIVE;
			}

			THROW(MBC_MessageQueue+300+m_iErrorNo);
		}
		return ret;
	#endif
}

//##ModelId=4314101D0066
int MessageQueue::getMessageNumber()
{
	if (!m_bAttached)
	{
		m_iErrorNo = E_CLOSE;
		return 0;
	}

	#ifdef SYSTEM_V
		struct msqid_ds attr;
		//int ret;

		//##��ȡ��Ϣ���������Ϣ
		if (msgctl (m_iSysMQID, IPC_STAT, &attr))
		{
			//##�쳣����
			m_iErrorNo = _getStandardError (errno);
			if (m_iErrorNo == E_NOENT)
				m_bAttached = false;

			THROW(MBC_MessageQueue+700+m_iErrorNo);
		}

		return attr.msg_qnum;
	#else
		struct mq_attr attr;

		//##��ȡ��Ϣ���������Ϣ
		if (mq_getattr (m_iSysMQID, &attr))
		{
			//##�쳣����
			m_iErrorNo = _getStandardError (errno);
			//return -1;
			THROW(MBC_MessageQueue+700+m_iErrorNo);
		}

		return attr.mq_curmsgs;
	#endif
}

//MessageQueue::MessageQueue(char *mqname)
//##ModelId=4314101D0067
MessageQueue::MessageQueue(int key)
{

	m_iSysMQID = -1;
	m_iMaxNum = 0;
	m_iMaxLen = 0;
	m_iBlock = 0;
	m_bAttached = false;
	m_pBuff = 0;
	m_iBufLen=0;

	//##ʮ���Ƶ���ת��ʮ�����Ƶ�����1000->0X1000
	//##����ȥ�Ҷ���

	#ifdef SYSTEM_V
	{
		int iTemp = 0;

		m_iKey = 0;
		while (key) {
			m_iKey |= ((key %10) << iTemp);
			key = key / 10;
			iTemp += 4;
		}
	}
	#endif

	/*

	m_sMQName = new char[strlen(mqname)+1];
	if (!m_sMQName)
		return;

	strcpy(m_sMQName, mqname);

	*/
	m_sMQName = new char[32];
	if (!m_sMQName) THROW(MBC_MessageQueue+12);

	sprintf (m_sMQName, "/tmp/mq%d", key);
}

//##ModelId=4314101D0069
int MessageQueue::getSysID()
{
	return m_iSysMQID;
}

//##ModelId=4314101D006A
int MessageQueue::getOccurPercent()
{

	if (!m_bAttached)
	{
		m_iErrorNo = E_CLOSE;
		return 0;
	}

	#ifdef SYSTEM_V
		struct msqid_ds attr;
		//int ret;

		//##��ȡ��Ϣ���������Ϣ
		if (msgctl (m_iSysMQID, IPC_STAT, &attr))
		{
			//##�쳣����
			m_iErrorNo = _getStandardError (errno);
			if (m_iErrorNo == E_NOENT)
				m_bAttached = false;

			THROW(MBC_MessageQueue+700+m_iErrorNo);
		}

		if (!attr.msg_qbytes) return 0;

		return attr.msg_cbytes * 100 / attr.msg_qbytes;
	#else
		int i;

		i = getMessageNumber ();

		if (!m_iMaxNum)
			return 0;

		return i*100/m_iMaxNum;
	#endif
}

//##ModelId=4314101D006B
MessageQueue::~MessageQueue()
{
	close ();

	if (m_sMQName)
		delete [] m_sMQName;

	if (m_pBuff)
		delete [] m_pBuff;
}

//##ModelId=4314101D006D
bool MessageQueue::_open()
{
	if (m_bAttached)
		return true;

	#ifdef SYSTEM_V
	{
		//##SYSTEM_V�µĲ���
//		struct msqid_ds attr;
//		int ret;

		//int key = ftok (m_sMQName, '0');

		m_iSysMQID = msgget (m_iKey, MQ_PERMISSION);
		if (m_iSysMQID == -1)
		{
			//##�쳣����
			goto _FALSE;
		}

		//##��ȡ��Ϣ���������Ϣ

		/*
		ret = msgctl (m_iSysMQID, IPC_STAT, &attr);
		if (ret == -1)
		{
			//##�쳣����
			goto _FALSE;
		}

			m_iMaxLen = attr.__unused5;
			m_iMaxNum = attr.__unused4;
		*/

		if (m_pBuff)
			delete [] m_pBuff;

		m_pBuff = new char[m_iBufLen+sizeof(long)];
		if (!m_pBuff)
		{
			//##�쳣����
			m_iErrorNo = E_NOMEM;
			return false;
		}

	}
	#else
	{
		//##POSIX�µĲ���
		struct mq_attr attr;
		int ret;

		attr.mq_flags = m_iBlock ? 0 : O_NONBLOCK;

		m_iSysMQID = mq_open (m_sMQName, O_RDWR, MQ_PERMISSION);
		if (m_iSysMQID == -1)
		{
			//##�쳣����
			goto _FALSE;
		}

		ret = mq_setattr (m_iSysMQID, &attr, NULL);
		if (ret == -1)
		{
			//##�쳣����
			m_iErrorNo = _getStandardError (errno);
			mq_close (m_iSysMQID);
			return false;
		}

		//##��ȡ��Ϣ���������Ϣ
		ret = mq_getattr (m_iSysMQID, &attr);
		if (ret == -1)
		{
			//##�쳣����
			m_iErrorNo = _getStandardError (errno);
			mq_close (m_iSysMQID);
			return false;
		}

		m_iMaxLen = attr.mq_msgsize;
		m_iMaxNum = attr.mq_maxmsg;
	}
	#endif

	m_bAttached = true;
	return true;

_FALSE:
	m_iErrorNo = _getStandardError (errno);
	return false;
}

//##ModelId=4314101D006E
bool MessageQueue::_create()
{
	//##�������趨��Ϣ���д�С����
	#ifdef SYSTEM_V
	{
		//##SYSTEM_V����Ĳ���
		//int key = ftok (m_sMQName, '0');
		struct msqid_ds attr;
		int ret;

		m_iSysMQID = msgget (m_iKey, MQ_PERMISSION | IPC_CREAT | IPC_EXCL);
		if (m_iSysMQID == -1)
		{
			//##�쳣����
			goto _FALSE;
		}
		//##�趨��С
		ret = msgctl (m_iSysMQID, IPC_STAT, &attr);
		if (ret == -1)
		{
			//##�쳣����
			goto _FALSE;
		}

		if ((int)attr.msg_qbytes != m_iMaxNum*m_iMaxLen)
		{
			attr.msg_qbytes = m_iMaxNum * m_iMaxLen;
			//##����һ���Ƿ��е�ͨ
			//##����֤�������У�
			/*{{{
			attr.__unused4 = m_iMaxNum;
			attr.__unused5 = m_iMaxLen;
			}}}*/

			ret = msgctl (m_iSysMQID, IPC_SET, &attr);
			if (ret == -1)
			{
				//##�쳣����
				int eb = errno;
				msgctl (m_iSysMQID, IPC_RMID, NULL);
				errno = eb;
				goto _FALSE;
			}
		}

		if (m_pBuff)
			delete [] m_pBuff;

		m_pBuff = new char[m_iBufLen+sizeof(long)];
		if (!m_pBuff)
		{
			m_bAttached = false;
			m_iErrorNo = E_NOMEM;
			return false;
		}

	}
	#else
	{
		//##POSIX����Ĳ���
		struct mq_attr attr;
		int ret;

		attr.mq_flags = m_iBlock ? 0 : O_NONBLOCK;
		attr.mq_maxmsg  = m_iMaxNum ;
		attr.mq_msgsize = m_iMaxLen;

		m_iSysMQID = mq_open (m_sMQName, O_RDWR | O_CREAT | O_EXCL, MQ_PERMISSION, &attr);
		if (m_iSysMQID == -1)
		{
			//##�쳣����
			goto _FALSE;
		}

		ret = mq_setattr (m_iSysMQID, &attr, NULL);
		if (ret == -1)
		{
			//##�쳣����
			goto _FALSE;
		}
	}
	#endif

	m_bAttached = true;
	return true;

_FALSE:
	m_iErrorNo = _getStandardError (errno);
	return false;
}

//##ModelId=4314101D006F
int MessageQueue::_getStandardError(int err)
{
	/*
	SYSTEM_V                                         POSIX

	EACCES  NO PERMISSION
	EFAULT  POINTER UNACCESSIBLE
	EIDRM   MQ REMOVED
	EINVAL  INVALID VALUE(mqid,parameter value)	EBADF,EINVAL
	EPERM	NO PERMISSION
	EEXIST	MQ ALREADY EXIST
	ENOENT  MQ DOES NOT EXIST
	ENOMEM  NOT ENOUGH MEMORY
	ENOSPC  CAN NOT CREATE MORE MQ
	E2BIG
	EAGAIN
	ENOMSG

			NOT SUPPORTED BY THE IMPLEMENTATION	ENOSYS
			TOO MANY

	*/
	#ifdef SYSTEM_V
		switch (err)
		{
		case EFAULT:
			return E_INVAL;
		case EIDRM:
			return E_NOENT;
		case EINVAL:
			return E_INVAL;
		case EPERM:
			return E_PERM;
		case EEXIST:
			return E_EXIST;
		case ENOENT:
			return E_NOENT;
		case ENOMEM:
			return E_NOMEM;
		case ENOSPC:
			return E_SYS;
		case ENOMSG:
			return E_NOMSG;
		case EINTR:
			return E_INTR;
		case EAGAIN:
			return E_FULL;
		default:
			return E_UNKNW;
		}
	#else
		switch (err)
		{
		case EBADF:
			return E_INVAL;
//		case ENOSYS:
//			return E_NOSYS;
		case EINVAL:
			return E_INVAL;
//		case EACCESS:
//			return E_PERM;
		case EEXIST:
			return E_EXIST;
		case EINTR:
			return E_INTR;
		case EMFILE:
			return E_SYS;
		case ENOENT:
			return E_NOENT;
		case ENOSPC:
			return E_SYS;
		case EAGAIN:
			return E_NOMSG;
		case EMSGSIZE:
			return E_SYS;
		default:
			return E_UNKNW;
		}
	#endif
}

//##ModelId=4314101D006C
MessageQueue::operator bool()
{
 	return m_sMQName;
 // return m_bAttached;
}

//##ModelId=432E88AD022F
int MessageQueue::send(string &str,long lMtype)
{
	//##�����ʵ�ֲ��������ȼ�
	string strtemp;
	int ret;
	int	len=str.size();

	if (!len)
		return 0;

	if (len>m_iBufLen)
			throw(MBC_MessageQueue+702);

	#ifdef SYSTEM_V

		strtemp.resize(sizeof(long),0);

		void *p = (void *)strtemp.data();

		if (lMtype==0)
			*((long *)p) = len; //modified by linjinxin
		else
			*((long *)p) = lMtype;

		strtemp+=str;

		ret = msgsnd (m_iSysMQID, strtemp.data(), len, (m_iBlock ? 0 : IPC_NOWAIT));
	#else
		ret = mq_send (m_iSysMQID,str.data() , len, 1);
	#endif

	if (ret == -1)
	{
		//##�쳣����
		m_iErrorNo = _getStandardError (errno);

		if (m_iErrorNo == E_NOMSG)
			m_iErrorNo = E_FULL;//POSIX

		if (m_iErrorNo == E_FULL)
			return 0;

		if (m_iErrorNo == E_NOENT)
			m_bAttached = false;

		THROW(MBC_MessageQueue+200+m_iErrorNo);
//		return -1;
	}

	return len;
}

//##ModelId=432E88D90188
int MessageQueue::receive(string &str,long lMtype)
{
	//##���û����Ϣ�����ؿ�
	/*
		2005-05-24
		������Ϣ��ʱ�򣬺����ź�������ж�
	*/
	int ret;

	string strtemp(m_iBufLen+sizeof(long),0);

//	msg_rbuf m_STmsg_rbuf;

	#ifdef SYSTEM_V


		__RECEIVE2:

		//ret = msgrcv (m_iSysMQID, m_pBuff, m_iBufLen, 0, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));
		//ret = msgrcv (m_iSysMQID, (void *)strtemp.data(), m_iBufLen, 0, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));
		ret = msgrcv (m_iSysMQID, (void *)strtemp.data(), m_iBufLen, lMtype, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));
		//ret = msgrcv (m_iSysMQID, &m_STmsg_rbuf, m_iBufLen, lMtype, (m_iBlock ? MSG_NOERROR : IPC_NOWAIT|MSG_NOERROR));
		/*
			  If msgtyp is 0, then the first message in the queue is read.

              If msgtyp is greater than 0, then the first message on the queue
              of  type  msgtyp  is  read,  unless  MSG_EXCEPT  was asserted in
              msgflg, in which case the first message on the queue of type not
              equal to msgtyp will be read.

              If  msgtyp  is  less than 0, then the first message on the queue
              with the lowest type less than or equal to the absolute value of
              msgtyp will be read.
		*/
		if (ret >=0)
		{
			str.assign(strtemp,sizeof(long),ret);
			//str.assign(m_STmsg_rbuf.mtext,ret);
			//disp_buf((char*)str.c_str(), ret);
		}
		else
		{
			m_iErrorNo = _getStandardError (errno);

			if (m_iErrorNo == E_NOMSG){
				str="";
				return 0;
			}
			if (m_iErrorNo == E_NOENT)
				m_bAttached = false;

			if (m_iErrorNo == E_INTR) {
				Log::log (1000, "������Ϣ�������յ��źţ������ź�...");
				goto __RECEIVE2;
			}

			THROW(MBC_MessageQueue+300+m_iErrorNo);
		}

		return ret;
	#else
		/*struct mq_attr attr;
		ret = mq_getattr (m_iSysMQID, &attr);
		if (ret == -1)
		{
			m_iErrorNo = _getStandardError (errno);
			return -1;
		}
		*/

		__RECEIVE2:
		ret= mq_receive (m_iSysMQID, (char *)strtemp.data(), /*attr.mq_msgsize*/m_iMaxLen, NULL);

		if (ret == -1)
		{
			m_iErrorNo = _getStandardError (errno);
			if (m_iErrorNo == E_NOMSG)
				return 0;

			if (m_iErrorNo == E_INTR) {
				Log::log (1000, "������Ϣ�������յ��źţ������ź�...");
				goto __RECEIVE2;
			}

			THROW(MBC_MessageQueue+300+m_iErrorNo);
		}
		str=strtemp;
		return ret;
	#endif
}

//##ModelId=4331083B0017
void MessageQueue::setBlockFlag(bool ifblock)
{
	m_iBlock = ifblock;
}


