#ifndef _HBCLEANTEST_H
#define _HBCLEANTEST_H
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Log.h"
#include "MBC.h"
#include "Environment.h"
#include "ReadIni.h"

//#define SQL_USERDEL "/src_jt/app_hbinstalltest/sql/test2_deluserinfo.sql"
//#define SQL_TEMP "/src_jt/app_hbinstalltest/sql/deltableinfo_temp.sql"
//#define SQL_BILLDEL "/src_jt/app_hbinstalltest/sql/test2_billflow_unload.sql"

//#define DELLIST "/src_jt/app_hbinstalltest/shell/deltestlist.sh"
//#define CLEAR "/src_jt/app_hbinstalltest/shell/clear.sh"



class Hbcleantest
{
	private:
		
	public:
		Hbcleantest();
		~Hbcleantest();
		int getenviroment();
		bool cleanInfo();				//���������Ͷ�������
		//bool cleanTestList();		//������Ի���
		//bool cleanTempInfo();	//����Ʒѹ����м�����
		//bool cleanBillflowcfg(); //���������������
		
		char h_path[100];		//����ϵͳ·��
		char h_user[20];			//�����û�
		//char h_hostip[20];	//����IP
		//char h_hostname[100]; //������
		
		char SQL_USERDEL[1024];
		
		int Init();
		ReadIni	RD;	
		
		int getFullPath(const char * str1,const char * str2,const char * str3,char * pp,const char * scat,int pro=0);
};

#endif
