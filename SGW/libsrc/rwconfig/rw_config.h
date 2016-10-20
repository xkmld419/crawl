/*
* Copyright 2001 LinkAge Co.,Ltd. Inc. All Rights Reversed
*/

/*********************************************************************************/
/*** Name             : CONFIG.H                                               ***/
/***                                                                           ***/
/*** Description      : ��д�����ļ�ģ��                                       ***/
/***                                                                           ***/
/*** Author           : ����                                                   ***/
/***                                                                           ***/
/*** Begin Time       : 2001/11/03                                             ***/
/***                                                                           ***/
/*** Last Change Time : 2001/11/09                                             ***/
/***                                                                           ***/
/*********************************************************************************/

/*********************************************************************************/
/***               Class C_Config user include Compile declare                 ***/
/***               Config encapsulate all global methods(functions)            ***/
/*********************************************************************************/

#ifndef CONFIG_H_
#define CONFIG_H_

/*********************************************************************************/
/***                           Standard Include                                ***/
/*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <ctype.h>
#include <vector>

#include <stdexcept>
using namespace std;
#ifdef DEF_AIX
#include "c++dcl.h"
#endif

/*********************************************************************************/
/***                          General Constants                                ***/
/*********************************************************************************/

#ifndef	LINELENGTH
#define LINELENGTH 4096
#endif

class C_Config
{
public:
	C_Config();

	/*�������ļ�����*/
	bool GetParam(const char *FileName,const char *Section,const char *Key,char *Value);
	/*д�����ļ�����*/
	bool SetParam(const char *FileName,const char *Section,const char *Key,const char *Value);

private:
	/*ȥ���ַ������ߵĿո���*/
	void Trim(char * String);
	/*���ļ��еõ�һ�����ݺ���*/
	int GetLine(int Fd,char *Line);
	/*���ļ���д��һ�����ݺ���*/
	int PutLine(int Fd,char *Line);
	/*������״̬*/
	int SetLockStatus(int Fd, int Type,
			int Cmd = F_SETLKW, off_t Offset = 0,
			int Whence = SEEK_SET, off_t Len = 0);
private:
	/*�������ļ�������Ϣ������*/
	vector<string> VRecord;
	/*��һ�ζ�ȡ�������ļ���*/
	char chFileTemp[LINELENGTH];
	/*д�ļ���־λ*/
	int iWriteFlag;

};

//typedef C_Config CConfig;

#endif/*CONFIG.H*/



