
#ifndef   PARAMSHMMGR_H_HEADER_INCLUDED
#define   PARAMSHMMGR_H_HEADER_INCLUDED

#include "CSemaphore.h"
#include "ParamInfoMgr.h"


#define PARAM_SHM_NUM 2                        //�����ڴ����Ŀ.��A,B����,����Ŀ��������

#define PARAM_SHM_INDEX_ALL -1                 //
#define PARAM_SHM_INDEX_A    0                 //�����ڴ��ı��A
#define PARAM_SHM_INDEX_B    1                 //�����ڴ��ı��B

//������۵ù����ڴ�ṹ,���Ŀǰ��������ʹ�õ����⹲���ڴ��ı��
struct ParamAccessPort
{
    int m_iShmIdx;                             //�ڹ����Ĺ����ڴ�ı��
	int m_iFlashSeqIdx;
	int m_iWait2DelSec;//delete date need second() ., default is 0 no deleting other block    
	int m_iAlteration ;//
	int m_iAlteratAmend ;//
    long m_lExVal;//��չ��ֵ,����Ӧ�ý���֮��Ĵ���
    char cState;//״̬�ֶ�,��������ȫ�����ݿ���    
};


class ParamShmMgr
{
public:
    ParamShmMgr();
    ~ParamShmMgr();

    void loadShm();                          //���������빲���ڴ�
    void flashShm();                         //ˢ�¹����ڴ�    
    void unloadAllShm(bool bForce = false);  //ж�����й����ڴ�
		void getStat();													 //ͳ�ƹ����ڴ�
    void unloadShmByKey(const long lShmKey, bool bForce = false);
    void setState(char cSate);


    bool linkDataAccessShm();                //���Ӳ����������Ĺ����ڴ�

    //���ݽṹ������(����Ĺ����ڴ��ṹ,��������Ϣ��)
    ParamInfoMgr* m_pParamDataBuf[PARAM_SHM_NUM];
    
    //���ݷ�����ڽṹ
    struct ParamAccessPort* m_pParamAccessPort;    
	int showCtlInfo();

	void unloadShmByFlag(int iFlag);
private:
    
    void allocDataAccessShm();
    void detachShmByData(void * pData);
    void deleteShmByKey(const long lShmKey);
    

    long m_lShmStateKey;                     //��ڹ����ڴ�key
    long m_lShmStateLockKey;                 //��ڹ����ڴ���ź�����key
public:
    CSemaphore* m_pShmStateLockSem;          //��ڹ����ڴ���ź���


};


#endif    //PARAMSHMMGR_H_HEADER_INCLUDED

