// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef MESSAGEQUEUE_H_HEADER_INCLUDED_BDFE1869
#define MESSAGEQUEUE_H_HEADER_INCLUDED_BDFE1869

#include "Exception.h"
#include <string>

#include "platform.h"
//##ModelId=4314101B0389
//##Documentation
//## ��Ϣ���з�װ�࣬
//## ֧��POSIX��SYSTEM V���ֱ�׼
//## ��SYSTEM V��׼��ʱ��Ҫ��#define SYSTEM_V

/******************************************************************************
	��SYSTEM_V����º�POSIX����£�MessageQueueʹ�÷���������

	����	SYSTEM_V					POSIX
	----------------------------------------------------------------
	����	Ҫ���д������г����û�		����Ҫ
		Ȩ�ޣ���ΪĬ��ֻ��16k
		��С�Ŀؼ���ͨ����Ҫ��
		չ�ռ�

	��	Ҫ������ʹ�ø���Ϣ����		�򿪵�ʱ�������ƶ����Զ���ȡ
		�ĳ����ڴ򿪻򴴽���ʱ		����ʱ�õĲ���ֵ��
		��ʹ�õ�maxnum��maxlen
		��ͬ��

	�ر�	��ʵû�йر�				�ر���

	����һ��

	�ο����ϣ�SYSTEM_V if cmd is IPC_SET,an attempt is being made
			to increase to the value of msg_qbytes,and the calling process does not
		    have the SUPER-USER PRIVILEGE.

	��ϵͳPOSIX��ʱ�򣬽���ʹ��POSIX����Ϣ���С�
******************************************************************************/

struct msg_rbuf
{
   long mtype;     /* message type, must be > 0 */
   char mtext[1025];  /* message data */
};

class MessageQueue
{
  public:
    //##ModelId=4314101D004F
    //##Documentation
    //## ����Ϣ���У��ƶ���ʽ��
    //## 1���Ƿ�����
    //## 2������������Ƿ񴴽���
    void open(bool ifblock = true, bool ifcreate = true, int maxlen = 2048, int maxnum = 1000, int buflen = -1 );

    //##ModelId=4314101D0059
    //##Documentation
    //## ɾ��һ���Ѿ����ڵĶ���
    void remove();

    //##ModelId=4314101D005A
    //##Documentation
    //## �Ͽ���ĳ���е�����
    void close();

    //##ModelId=4314101D005B
    //##Documentation
    //## ���ظö����Ƿ����
    bool exist();

    //##ModelId=4314101D0061
    //##Documentation
    //## ����һ������
    int send(void *data, int len, long lMtype=0);

    //##ModelId=4314101D0064
    //##Documentation
    //## ����Ϣ���н���һ������
    int receive(char *buf,long lMtype=0);

    //##ModelId=4314101D0066
    //##Documentation
    //## �����ڵ���Ϣ����
    int getMessageNumber();

    //##ModelId=4314101D0067
    //##Documentation
    //## ���캯��
    /*MessageQueue(
        //##Documentation
        //## ��Ϣ���е����֣�Ҫ��
        //## ��һ���ļ�����Ŀ¼����������·�����磺
        //## /home/bill/mqfile/process1000.input
        //## ��ע�⣬·���в��ܰ���"."��".."���磺/home/bill/../bill/mqfile/process1000.input��
        char *mqname);*/
    MessageQueue(int key);

    //##ModelId=4314101D0069
    //##Documentation
    //## ������Ϣ���е�IPCID
    int getSysID();

    //##ModelId=4314101D006A
    //##Documentation
    //## ����ʹ�ðٷֱȣ���ȷ������
    int getOccurPercent();

    //##ModelId=4314101D006B
    ~MessageQueue();
    //##ModelId=4314101D006C
    operator bool();
    //##ModelId=432E88AD022F
    int send(string &str,long lMtype=0);

    //##ModelId=432E88D90188
    int receive(string &str,long lMtype=0);

    //##ModelId=4331083B0017
    void setBlockFlag(bool ifblock = true);

    bool getBlockFlag(){return m_iBlock; };


    //##ModelId=4314101B0393
	enum ERRTYPE
	{
		E_PERM = 1,
		E_NOENT,
		E_INVAL,
		E_EXIST,
		E_NOMEM,
		E_SYS,
		E_NOMSG,
		E_INTR,
		E_CLOSE,
		E_FULL,
		E_UNKNW
	};
  private:
    //##ModelId=4314101D006D
    bool _open();

    //##ModelId=4314101D006E
    bool _create();
    //##ModelId=4314101D006F
    int _getStandardError(int err);

    //##ModelId=4314101D0032
    int m_iKey;

    //##ModelId=4314101D0039
    int m_iSysMQID;

    //##ModelId=4314101D003A
    char *m_sMQName;

    //##ModelId=4314101D0043
    //##Documentation
    //## ���ɵ������Ϣ��
    int m_iMaxNum;

    //##ModelId=4314101D0044
    //##Documentation
    //## ÿ����Ϣ����󳤶�
    int m_iMaxLen;

    //##ModelId=4314101D0045
    int m_iBlock;

    //##ModelId=4314101D0046
    bool m_bAttached;
    //##ModelId=4314101D004D
    int m_iErrorNo;


    //##ModelId=432E78810204
    char *m_pBuff;

    //##ModelId=432E789F01D5
    int m_iBufLen;


};

#ifdef SYSTEM_V
#define MQ_PERMISSION	0666
#else
#define MQ_PERMISSION	0660
#endif

#endif /* MESSAGEQUEUE_H_HEADER_INCLUDED_BDFE1869 */



