/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef MESSAGEQUEUE_EX_H_HEADER_INCLUDED_BDFE1869
#define MESSAGEQUEUE_EX_H_HEADER_INCLUDED_BDFE1869

#include "Exception.h"
//##ModelId=41DB83BD00A6
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


class MessageQueueEx
{
  public:
    //##ModelId=41DCE72A01B6
    //##Documentation
    //## ����Ϣ���У��ƶ���ʽ��
    //## 1���Ƿ�����
    //## 2������������Ƿ񴴽���
    void open(bool ifblock = true, bool ifcreate = true, int maxlen = 2048, int maxnum = 10000 );

    //##ModelId=41DCE731009D
    //##Documentation
    //## ɾ��һ���Ѿ����ڵĶ���
    void remove();
	
    //##ModelId=41DCE73802A6
    //##Documentation
    //## �Ͽ���ĳ���е�����
    void close();

    //##ModelId=41DCE73F033C
    //##Documentation
    //## ���ظö����Ƿ����
    bool exist();

    //##ModelId=41DCE76F01B5
    //##Documentation
    //## ����һ������
    int send(void *data, int len);
	int send(void* data,long lType,int len);
    //##ModelId=41DCE77202B4
    //##Documentation
    //## ����Ϣ���н���һ������
    int receive(char *buf);

	int receive(char* buf,unsigned int iLen);
	int receive(char* buf,long lType,unsigned int iLen);	//=SJ\V86(@`PM5DO{O"
    //##ModelId=41DCE77902FA
    //##Documentation
    //## �����ڵ���Ϣ����
    int getMessageNumber();

    //##ModelId=4200B83B0109
    //##Documentation
    //## ���캯��
    /*MessageQueue(
        //##Documentation
        //## ��Ϣ���е����֣�Ҫ��
        //## ��һ���ļ�����Ŀ¼����������·�����磺
        //## /home/bill/mqfile/process1000.input
        //## ��ע�⣬·���в��ܰ���"."��".."���磺/home/bill/../bill/mqfile/process1000.input��
        char *mqname);*/
    MessageQueueEx(int key);

    //##ModelId=42017C500050
    //##Documentation
    //## ������Ϣ���е�IPCID
    int getSysID();

    //##ModelId=42017CA40228
    //##Documentation
    //## ����ʹ�ðٷֱȣ���ȷ������
    int getOccurPercent();

    //##ModelId=42018258024F
    ~MessageQueueEx();
    //##ModelId=4219719B0006
    operator bool();

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
    //##ModelId=4201CC4C01B2
    bool _open();

    //##ModelId=4201CC5203B9
    bool _create();
    //##ModelId=421590F302CE
    int _getStandardError(int err);

    int m_iKey;

    //##ModelId=41F4B0100358
    int m_iSysMQID;

    //##ModelId=420087E102BF
    char *m_sMQName;

    //##ModelId=4200CDE6012D
    //##Documentation
    //## ���ɵ������Ϣ��
    int m_iMaxNum;

    //##ModelId=4200CE0003D3
    //##Documentation
    //## ÿ����Ϣ����󳤶�
    int m_iMaxLen;

    //##ModelId=4200CE3A0304
    int m_iBlock;

    //##ModelId=4200CE6001D2
    bool m_bAttached;
    //##ModelId=4215913D0342
    int m_iErrorNo;


    //##ModelId=421591750375
    char *m_pBuff;
	
	char* m_pBuffEx;
	unsigned int m_iBuffExSize;

};

#ifdef SYSTEM_V
#define MQ_PERMISSION	0666
#else
#define MQ_PERMISSION	0660
#endif

#endif /* MESSAGEQUEUE_H_HEADER_INCLUDED_BDFE1869 */
