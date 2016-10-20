#ifndef _ABM_EXCEPTION_H_
#define _ABM_EXCEPTION_H_

#include <vector>
#include <stdio.h>

using namespace std;

struct ExceptMsg {
    char m_sFileName[32];       //����������ļ���
    int  m_iLineNo;            //��������ĵط�
    char m_sMsg[256];            //�������Ϣ����
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
#define ADD_EXCEPT12(X,Y,Z,Z1,Z2,Z3,Z4,Z5,Z6,Z7,Z8,Z9,Z10,Z11) (X).addErr(__FILE__, __LINE__,Y,Z,Z1,Z2,Z3,Z4,Z5,Z6,Z7,Z8,Z9,Z10,Z11)
	
class ABMException {
public:
    ABMException() {
        m_oErrBuf.resize (16);
        m_oErrBuf.clear ();
    }

    //����Ϣͨ��getErrs��ɺ���Ӧ�ó�����ʽ�ؽ������
    void clear() {
        m_oErrBuf.clear ();
    }

    //ͨ���˷�����ȡ�����󣻸�ָ�뷵���ڲ����򣬲�Ҫ��ָ����ɾ������
    vector<ExceptMsg> const * getErrs() {
        return &m_oErrBuf;
    }

    //ͨ�����·������������������׼��������ļ���
    //sLineHead: ������Ϣ��ÿ�����ף����Դ���
    void printErr(char *sLineHead=0, FILE *pFile = 0);

    //ͨ��addErr�������󱣴�������ͨ��ʹ�� ADD_EXCEPT��
    void addErr(char const *sFile, int const iLine, char const *sFormat, ...);

private:
    vector<ExceptMsg> m_oErrBuf;
};


#endif

