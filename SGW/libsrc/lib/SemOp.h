#include "platform.h"
#include <errno.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

#include "platform.h"

union semun
{
	int val;				/* value for SETVAL */ //�ź���ֵ����ִ��SETVAL����ʱʹ��
	struct semid_ds *buf;	/* buffer for IPC_STAT & IPC_SET */ //��IPC_STAT/IPC_SET������ʹ�á��������ں���ʹ�õ��ź��������ݽṹ��
	ushort *array;			/* array for GETALL & SETALL */ //��ʹ��GETALL/SETALL����ʱʹ�õ�ָ�롣
};

//�ź���������
//����ʵ�ֽ��̼����ݵ�ͬ���ͻ������
class SemOp
{
  public:
	SemOp(key_t keyvalue, int maxval);
	~SemOp();

	//��ʼ���ź�����ֵΪvalue
	//rCreate ���ڵĻ��Ƿ��ؽ��ź���
	int initSem (int value = 1, bool rCreate = false);
	int attachSem (); //�����ź���
	int destroySem ();	//�ͷ��ź���
	int P() ;	//ִ���ź�������P����
	int V();	//ִ���ź�������V����

	void setWait(bool wait = true);

	int setVal(int val);

  private:
  	int 	iSemID;	// �ź�������IPC ��ʶ��
	key_t	semkey;
	bool	bIfWait;	//��û����Դʱ���Ƿ���ס���ȴ���Դ
	int 	iMaxSemVal; //����ź���ֵ
};




