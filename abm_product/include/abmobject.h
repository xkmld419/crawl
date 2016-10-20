/*
*   version 1.0 js at 2010-09-09
*   v 1.1 
*         1. ����8���ֽڱ����ֶ� �ú�EXTEND_ABMOBJECT ����
*         2. ��ԭ��������AccuTpl�����16�ֽڷŵ�m_uBody��.
*         3. ԭ���ڸ�ҵ������ʵ�ֵ�clear �ŵ���ABMObject.
*         4. ������memcpyh ����copy
*        2,3,4 �����ۻ�����Ч  
*/
#ifndef    __ABMOBJECT_H_INCLUDED_
#define    __ABMOBJECT_H_INCLUDED_

#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#define BUF_SIZE_MAX 61440

#ifdef EXTEND_ABMOBJECT
#define OFFSET_BUF 64
#else
#define OFFSET_BUF 56
#endif

#define CMD_BUF_SIZE (BUF_SIZE_MAX-OFFSET_BUF)

/***************************************************************************
*	������ABMObject
*	���ܣ������˲�ѯABM�ۻ����Ļ������ݽṹ�壬��Ϊ���պͷ��͵�����
***************************************************************************/

class ABMObject 
{
    
public:
    
    ABMObject() {};
	
	ABMObject(int cmd): m_iCmdID(cmd) {};
    //�������к�
    void setSeqID(int iSeqID) {
        m_iSeqID = iSeqID;
    }
    
    //Accu��������
    void clearA(int iCode=0) {
        m_iSize = OFFSET_BUF + sizeof(m_uBody.m_tNum);
        memset(&m_uBody.m_tNum, 0x00, sizeof(m_uBody.m_tNum));
        m_iRltCode = iCode;    
    }
    //Accu��ר�ú���
    void memcpyh(ABMObject const *pSrc) {
        memcpy((void *)&m_lHash, (const void *)pSrc, OFFSET_BUF);
        clearA();    
    }
		
public:
    
    /*API�и��ݴ�ֵ,����HASH���� ����ַ����ĸ�ABMҵ�����*/
    //����С��ת��ʱ ���ֶλ���Ϊ�Ƿ�����ת���ı�־ = -1 ת����
    long m_lHash;
	
	/*�ͻ��˽ڵ�ID, ��ҵ�����transID��� ,
	  Ψһȷ��һ������(��socket�ͻ����и�ֵ)*/
	int m_iClntID;   
	 
	/*����ʱ����Ϣ�������ȼ�,API�и�ֵ*/
	int m_iMqType;
	
	/*ABM�ڲ������к�, ֻ��ͬ���ӿ�ʱ���� API�и�ֵ*/
	long m_lTransID;
	
	/*��ABMϵͳʱ��, ����ͳ����*/
	long m_lInTm;
	
	/*��ABMϵͳʱ��, ����ͳ����*/
	long m_lOutTm;	
	
	/*ABMҵ��˴�����, ���ö˿ɸ��ݴ��ֶ��ж�ABM�Ƿ�ɹ�����*/
	int m_iRltCode;
	
	int m_iSeqID;
	
	/*����ID, ��־ABMϵͳ�ڵ�һ�ִ���ҵ��*/
	int m_iCmdID;
	
	/*��Ϣ��С, ��־������Ϣ���ܴ�С*/
	int m_iSize;
	
	#ifdef EXTEND_ABMOBJECT 
	
	/*�����ֶ�*/
	long m_lReserval;
	
	#endif
	
	/*��Ϣ��*/
	union {	
		/*��Ϣͷ*/
		struct {
			int m_iFirstNum;   //��ϰ��е�һ��ҵ�����Ŀ
			int m_iSecondNum;   //��ϰ��еڶ���ҵ�����Ŀ
			int m_iFlag;
			long *m_plNum;
		} m_tNum;
		
		/*������*/
		char m_sBuf[CMD_BUF_SIZE];
		
	} m_uBody;
	
};

typedef ABMObject ABMCmd;

class ABMResult : public ABMObject 
{

public:
    
    ABMResult() {};
    
    ABMResult(int cmd): ABMObject(cmd) {};
    
    //���ABM������
	int getResultCode() {
		return m_iRltCode;
	};
	
	//��ý������к�
	long getTransID() {
		return m_lTransID;
	};
	
	//���ABM����ID
	int getClassID() {
	    return m_iCmdID;    
	}
	       
    //��ȡ���к�
    int getSeqID() {
        return m_iSeqID;
    }
};

//���ļ���ʽ����
#define RLT_CODE_FILE_RETURN 1000


class ABMFile:public ABMResult {
public:
    char *getFileName() {
        return m_uBody.m_sBuf;
    }

     //ptr ���ص�ǰ����ָ���ַ
    int setFileName(char *sFileName, char *&ptr) {
        ptr = m_uBody.m_sBuf;
        m_iSize = (long)ptr - (long)this;
        int iLen = ((strlen(sFileName) >> 3) + 1) << 3;
        if (m_iSize + iLen > sizeof(*this)) {
            return -1;
        }
        strncpy(ptr, sFileName, iLen);
        ptr += iLen;
        m_iSize += iLen;
        return 0;
    }

    int setFileName(char *sFileName) {
       char *ptr = NULL;
       return setFileName(sFileName, ptr);
   }
};

#ifdef ABM_LITTLE_ENDIAN
inline long htonl64(long dig)
{   	
	union {
		long l64;
		struct {
			int a1;
			int a2;	
		} st64;	
	} u1, u2;
	
	u1.l64 = dig;
	u2.st64.a1 = htonl(u1.st64.a2);
	u2.st64.a2 = htonl(u1.st64.a1);
	return u2.l64;
};
#else
#define htonl64
#endif

#endif/*__ABMOBJECT_H_INCLUDED_*/

