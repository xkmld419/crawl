#ifndef _ABM_EXCEPTION_H_
#define _ABM_EXCEPTION_H_
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
using namespace std;

#define ERR_BUFFER_MAX 1024    //add by huff TT err_msg�Ƚϴ� 256 ��������
struct ExceptMsg {
    char m_sFileName[32];       //����������ļ���
    int  m_iLineNo;            //��������ĵط�
	char m_sTime[32];         //��������ʱ��
    char m_sMsg[ERR_BUFFER_MAX];            //�������Ϣ����
};

#define ADD_EXCEPT0(X,Y) (X).addErr(__FILE__, __LINE__,Y)
#define ADD_EXCEPT1(X,Y,Z) (X).addErr(__FILE__, __LINE__,Y,Z)
#define ADD_EXCEPT2(X,Y,Z,Z1) (X).addErr(__FILE__, __LINE__,Y,Z,Z1)
#define ADD_EXCEPT3(X,Y,Z,Z1,Z2) (X).addErr(__FILE__, __LINE__,Y,Z,Z1,Z2)
#define ADD_EXCEPT4(X,Y,Z,Z1,Z2,Z3) (X).addErr(__FILE__, __LINE__,Y,Z,Z1,Z2,Z3)
#define ADD_EXCEPT5(X,Y,Z,Z1,Z2,Z3,Z4) (X).addErr(__FILE__, __LINE__,Y,Z,Z1,Z2,Z3,Z4)
#define ADD_EXCEPT6(X,Y,Z,Z1,Z2,Z3,Z4,Z5) (X).addErr(__FILE__, __LINE__,Y,Z,Z1,Z2,Z3,Z4,Z5)
#define ADD_EXCEPT7(X,Y,Z,Z1,Z2,Z3,Z4,Z5,Z6) (X).addErr(__FILE__, __LINE__,Y,Z,Z1,Z2,Z3,Z4,Z5,Z6)
#define ADD_EXCEPT8(X,Y,Z,Z1,Z2,Z3,Z4,Z5,Z6,Z7) (X).addErr(__FILE__, __LINE__,Y,Z,Z1,Z2,Z3,Z4,Z5,Z6,Z7)

class SGWException {
public:
    SGWException() {
        m_oErrBuf.resize (16);
        m_oErrBuf.clear ();
    }

    //����Ϣͨ��getErrs��ɺ���Ӧ�ó�����ʽ�ؽ������
    void clear() {
        m_oErrBuf.clear ();
		memset(m_sFileName,0,sizeof(m_sFileName));
    }

    //ͨ���˷�����ȡ�����󣻸�ָ�뷵���ڲ����򣬲�Ҫ��ָ����ɾ������
    vector<ExceptMsg> const * getErrs() {
        return &m_oErrBuf;
    }

    //ͨ�����·������������������׼��������ļ���
    //sLineHead: ������Ϣ��ÿ�����ף����Դ���
    void printErr(char *sLineHead=0,int _iSign = 0);

    //ͨ��addErr�������󱣴�������ͨ��ʹ�� ADD_EXCEPT��
    void addErr(char const *sFile,int const iLine, char const *sFormat, ...);

	void setFileName(const char *_sFileName,long _lParam=0) {
#ifdef HB_28
		char *p = getenv("BILLDIR");
#else
		char *p = getenv("TIBS_HOME");
#endif
		if (p == NULL) {
			snprintf(m_sFileName,sizeof(m_sFileName),"%s_%d.log",_sFileName,((_lParam==0)?getpid():_lParam));
		} else {
			char _sTempDIR[200] = {0};
			snprintf(_sTempDIR,sizeof(_sTempDIR),"%s/log",p);

			if (opendir(_sTempDIR) == NULL) {

				if (errno ==ENOENT) {//Ŀ¼������
					if (mkdir(_sTempDIR,0666)==0) {
						snprintf(m_sFileName,sizeof(m_sFileName),"%s/%s_%d.log",_sTempDIR,_sFileName,((_lParam==0)?getpid():_lParam));
						return;
					}
				} 
			} 
			snprintf(m_sFileName,sizeof(m_sFileName),"%s/%s_%d.log",_sTempDIR,_sFileName,((_lParam==0)?getpid():_lParam));
			return;
		}
		snprintf(m_sFileName,sizeof(m_sFileName),"%s_%d.log",_sFileName,((_lParam==0)?getpid():_lParam));
	}

private:
    vector<ExceptMsg> m_oErrBuf;
	char m_sFileName[256];

};

#endif

