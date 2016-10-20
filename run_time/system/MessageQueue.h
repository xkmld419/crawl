#ifndef _ABM_MESSAGEQUEUE_H_
#define _ABM_MESSAGEQUEUE_H_

#include <string>
#include "ABMException.h"

using namespace std;

//##ModelId=4314101B0389
//##Documentation
//## ��Ϣ���з�װ�࣬
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
    int open(ABMException &oExp, bool ifblock = true, bool ifcreate = true);

    //##ModelId=4314101D0059
    //##Documentation
    //## ɾ��һ���Ѿ����ڵĶ���
    int remove(ABMException &oExp);

    //##ModelId=4314101D005A
    //##Documentation
    //## �Ͽ���ĳ���е�����
    void close();

    //##ModelId=4314101D005B
    //##Documentation
    //## ���ظö����Ƿ����
    bool exist();

    //##ModelId=4314101D0066
    //##Documentation
    //## �����ڵ���Ϣ����
    int getMessageNumber();

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

    //##ModelId=4331083B0017
    void setBlockFlag(bool ifblock = true) { m_iBlock = ifblock;}

    bool getBlockFlag(){return m_iBlock; };

	
  public:
    //add by huff ֧�ִ���Ϣ���ȼ�����
    /*
    *   ��������: ����Ϣ���н�����Ϣ���ȼ�Ϊmqtype,���ΪmaxLen������
    *   ����:
    *       void *buf: ���ջ��������׵�ַstruct my_msgbuf { mtyp_t mtype;
    *                                                     char  mtext[MSGSIZ]; } buf
    *       int maxLen: ���ջ������ĳ���
    *       long mqtype: ���յ���Ϣ���ȼ�
    *       bool inter: ��������ʱ �Ƿ��ź��ж�
    *   ����: �ɹ����ؽ��յ���Ϣ����, ʧ�ܷ���-1or -2(���ź��ж�)
    */
    int Receive( void *buf, int maxLen, long mqtype, bool inter=false, bool forcenoblock=false);
    
    /*
    *   ��������: ���׵�ַΪvoid *buf, ����Ϊlen�����ݷ�����Ϣ����
    *   ����:
    *       void *buf : ʵ�ʴ�������Ϊstruct my_msgbuf { mtyp_t mtype;
    *                                                     char  mtext[MSGSIZ]; }
    *       int len: ���������ݵ��ܳ���, ����sizeof(mtyp_t)��ͷ
    *       bool &inter: ��������ʱ, ���ź��ж��Ƿ񷵻�, inter==true ����-2
    *   ����: �ɹ����ط��ͳ���, ʧ�ܷ���-1 or -2(���ź��ж�)
    */
    int Send( void *buf, int len, bool inter=false, bool forcenoblock=false);   
    
  private:
    //##ModelId=4314101D006D
    int _open(ABMException &oExp);

    //##ModelId=4314101D006E
    int _create(ABMException &oExp);

    //##ModelId=4314101D0032
    int m_iKey;

    //##ModelId=4314101D0039
    int m_iSysMQID;

    //##ModelId=4314101D0045
    int m_iBlock;

    //##ModelId=4314101D0046
    bool m_bAttached;
};

#define MQ_PERMISSION	0666

#endif
