#ifndef ERROR_CODE_PARSE_H_
#define ERROR_CODE_PARSE_H_

//#ifndef HBE_CODE_PARSE_H_
//#define HBE_CODE_PARSE_H_

#include <string>
#include <stdio.h>
#include <map>
#include "ABMException.h"
#include "TimesTenAccess.h"
using namespace std;


const unsigned MAX_ERROR_CAUSE_LEN = 100;       // ����������Ϣ����
const unsigned MAX_ERROR_ADVICE_LEN = 100;      // �޸����鳤��

struct EInfo
{
public:
    EInfo()
    {
        memset(m_sCause, 0x00, sizeof(m_sCause));
        memset(m_sAdvice, 0x00, sizeof(m_sAdvice));
    }

public:
    char m_sCause[MAX_ERROR_CAUSE_LEN + 1];     // ����������Ϣ
    char m_sAdvice[MAX_ERROR_ADVICE_LEN + 1];   // �޸�����

};

typedef std::map<int, EInfo*>  EInfoMap;
class File;
class HBErrorCode
{
public:
    HBErrorCode();

    virtual ~HBErrorCode();

    /*
    *   �������ܣ���ʼ��HBErrorCode�����ļ�
    *   �����������
    *   �����������
    *   ����ֵ  �����ش����룬����ɹ��򷵻�0
    */
    int init();

    /*
    *   �������ܣ���ȡ�����������Ϣ
    *   ���������������
    *   �����������
    *   ����ֵ  �����ش�����������Ϣ
    */
    char* getCauseInfo(int iHBErrorCode);

    /*
    *   �������ܣ���ȡ�����ר���޸�����
    *   ���������������
    *   �����������
    *   ����ֵ  �����ش������ר���޸�����
    */
    char* getAdviceInfo(int iHBErrorCode);

    /*
    *   �������ܣ���ȡ����������Ϣ���������������޸�����
    *   ���������������
    *   �����������
    *   ����ֵ  �����ش������������Ϣ���޸�����
    */
    EInfo* getErrorInfo(int iHBErrorCode);

private:
    /*
    *   �������ܣ������ļ�����ȡ��������Ϣ
    *   �����������
    *   �����������
    *   ����ֵ  ���ɹ�����0�����򷵻�ָ������
    */
#ifdef _DEFINE_ECODE_FILE_
    int ParseErrorFile();
	
	/*
	*	�������ܣ���ȡ������Ϣ����ȡ��������Ϣ
	*	�����������
	*	�����������
	*	����ֵ	���ɹ�����0�����򷵻�ָ������
	*/
#else
	int ParseErrorTable();
#endif

    /*
    *   �������ܣ��ж��ַ���ǰ4λ�Ƿ���//@@
    *   ��������������ַ���
    *   �����������
    *   ����ֵ  ���ɹ�����0�����򷵻�ָ������
    */
    int CompStrHead(const char *sStr);
private:
#ifdef	_DEFINE_ECODE_FILE_
    File *m_poEFile;              // �����붨���ļ�
#else
	TimesTenCMD 		*m_poTTCursor;				// tt�α�
	TimesTenConn		*m_poTTConn;				
#endif
    EInfoMap m_oEInfo; // ��������Ϣ

};


#endif
