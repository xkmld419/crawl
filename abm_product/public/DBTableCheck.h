#ifndef __DB_TABLE_CHECK_H__
#define __DB_TABLE_CHECK_H__

class DBTableCheck
{
public:
	/*
        ���ܣ����ָ�����ָ������Ϣ�Ƿ���ڣ�������ѡ��
		����ֵ���ͣ�bool
		����ֵ���壺
			false ������
			true ����
    */
	static bool checkTabColumnExist(char * sTabName, char * sColumnName,char * sOwner = NULL);
};

#endif //__DB_TABLE_CHECK_H__
