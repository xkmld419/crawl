#ifndef _HSS_USER_CHECK_H_
#define _HSS_USER_CHECK_H_

#include <string>
#include <stdio.h>
#include <map>

using namespace std;

typedef std::map<string, string>  USERMap;
	
class CheckUserInfo
{
public:
    CheckUserInfo()
    {
    }

    ~CheckUserInfo()
    {
    };

    /*
    *   �������ܣ���ʼ��CheckUserInfo�����ļ�
    *   �����������
    *   �����������
    *   ����ֵ  �����ش����룬����ɹ��򷵻�0
    */
    static int ParseFile();

    /*
    *   �������ܣ���ȡ�����������Ϣ
    *   ���������������
    *   �����������
    *   ����ֵ  �����ش�����������Ϣ
    */
    static bool checkUerInfo(char * sUser,char *sPwd);



private:
    static USERMap m_mParamMap; // ��������Ϣ
};


#endif
