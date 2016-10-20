#ifndef _ABMFINDERR_H
#define _ABMFINDERR_H
#include "Environment.h"
#include <vector>
#include <iostream>
using namespace std;

class sErrInfo{
public:
    char scode[10];
	char sCause[256];
	char sAction[256];
	sErrInfo(){
		memset(scode,0,sizeof(scode));
		memset(sCause,0,sizeof(sCause)); memset(sAction,0,sizeof(sAction));
    }
};

class ABMFindErr
{
	public:
		ABMFindErr();
		~ABMFindErr();
		int ABMFindErrByFile(char * sErrCode,char * sCause,char * sAction);  //MBC_28.h�в��Ҵ�����Ϣ
		int ABMFindErrByTable(char * sErrCode,char * sCause,char * sAction); //���ݿ��������Ҵ�����Ϣ
		int ABMFindErrByFile();   //MBC_28.h�����еĴ�����Ϣ
		int PrintUsage();
		void GetErrType(int iCode,char* cErrType); 
		vector<sErrInfo *> m_vErrInfo;
		int resolve(int argc ,char ** argv);
		int GetNumber(char *src);
	
};

#endif
