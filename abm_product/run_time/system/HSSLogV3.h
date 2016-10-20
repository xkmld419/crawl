/*
*   ����������־��д��ABM�ڲ���־�� ģ��ABMException, ��Ҫ��Ϊ�˽���ƷѺ�HSS�ײ㲻����
*   ���߳���Ҫ���� 
*/

#ifndef __HSS_DIRECT_LOG_H_INCLUDED_
#define __HSS_DIRECT_LOG_H_INCLUDED_

#include "MBC_ABM.h"

//֧��ǰ�涨��ĺ�
#define DIRECT_LOG directLog
#define ADD_LOG    directLog
#define FLUSH_LOG       

//addLog ����˵��

//log level iLevel 
#define V3_FATAL 1   
#define V3_ERROR 2
#define V3_WARN  3
#define V3_INFO  4
#define V3_DEBUG 5

//log type  int iTypeID
#define V3_SERVICE   1     //ҵ����־
#define V3_PARAM_MGR 2     //����������־
#define V3_SYS_MGR   3     //ϵͳ������־
#define V3_SCHEDUL   4     //���̵�����־
#define V3_DB        5     //���ݿ�ؼ����ݲ�����־

//log iErrID  ���ձ�׼������ errorcode.h

//��������������־

struct HSSLogStruct
{
#define HSSLOGBUF_MAX 512
    int m_iModuID;    //=appID
    int m_iProcID;
    int m_iLevel;
    int m_iTypeID;
    int m_iErrNo;    //== iCodeID
    int m_iTime;
    char m_sErrMsg[HSSLOGBUF_MAX];
};

    
    //ֱ����־ д����־�ӿڲŷ���
extern void directLog(int iLevel, int iTypeID, int iErrNo, char *fmt, ...);
    
    //�����־�� д�뻺�棬 ����flushLog �����ۻ�����󻺴��� �ύ���ӿ�
    //static void addLog(int iLevel, int iTypeID, int iErrNo, char *fmt, ...);
    
    //��־ˢ�� ����������־ˢ�µ���־�ӿ�
   // static void flushLog();
//��Ϣ���Ȩ��־ר��   
extern void authLog(int iNeID, int iServiceID, char *pServiceName, char *pErrMsg);

//����ȫ�ֺ��Ĳ����Ļ�ȡ ��ֹ���������ڴ�
class CommandCom;

extern CommandCom* getKernel();      

#endif/*__HSS_DIRECT_LOG_H_INCLUDED_*/
