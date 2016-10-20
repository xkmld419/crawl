#ifndef __CSEMAPHORE_HEADER_INCLULDED__
#define __CSEMAPHORE_HEADER_INCLULDED__

/**********************************************************************************************\
* �ź�����
* 
*
\**********************************************************************************************/

class CSemaphore 
{
public:
	
	bool getSem(int sem_name, int init_count=1, int max_count=64);
	
    bool getSem(const char * sem_name, 
 /*��Windows�Ͽ������ַ�����ʾ����unix�����������֣���ʾkey_t*/
                int init_count = 1,
                 /*��ʼ��Դ������Ĭ��Ϊ 1 */
                int max_count = 64);
            /*���Է���Ľ���������Ĭ��Ϊ64*/
    bool P(bool bWait = true);                           /*P��������ʾ������Դ, bWait�Ƿ�Ҫͬ���ȴ�*/
    bool V();                                               /*v��������ʾ�ͷ���Դ*/
    bool remove();                                           /*�ر��ź���*/
public:
    CSemaphore();                                           /*���캯�����������semaphore�����룬��ҪgetSem��ʹ��*/
    virtual ~CSemaphore();                                  /*�رվ��*/
private:
    int m_iHandle;                                         /*�ź�����������*/
};
#endif

