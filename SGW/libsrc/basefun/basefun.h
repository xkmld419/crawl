/*
* Copyright 2002 LinkAge Co.,Ltd. Inc. All Rights Reversed
*/

/*********************************************************************************/
/*** Name             : BASEFUN.H                                              ***/
/***                                                                           ***/
/*** Description      : �������ܺ���                                           ***/
/***                                                                           ***/
/*** Author           : ����                       	                       ***/
/***                             	                                       ***/
/*** Begin Time       : 2002/05/26                                             ***/
/***                                                                           ***/
/*** Last Change Time : 2002/05/30       	                               ***/
/*** Revision History : 2003/08/28	�޸�readdir_r������
										**/
/***                                                                           ***/
/*********************************************************************************/

#ifndef BASEFUN_H_
#define BASEFUN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#include <vector>
#include <list>
#include <stdexcept>
using namespace std;
/*********************************************************************************/
/***                           Standard Include                                ***/
/*********************************************************************************/

extern int errno;

/*********************************************************************************/
/***                           General Constants                               ***/
/*********************************************************************************/

#ifndef LENGTH
#define LENGTH 255
#endif

/*�ļ���У�����Ŷ���*/
#define	ERROR_FILE_FILTE_RULE 					"F0001"	//�ļ���������ʽ�������󣬲������ô�����������
#define ERROR_FILE_SEQUENCE_DEF	  			"F0002" //�ļ���������ʽ�п�ʼ���к��������󣬺��Ƿ��ַ�
#define	ERROR_FILE_NAME_FORMAT					"F8000"	//�ļ�����ʽ����
#define	ERROR_DATE_FORMAT								"F8001"	// �ļ����е����ڲ��Ϸ�
#define	ERROR_FILE_DATE_BEFORE					"F8002"	// �ļ����������ڹ涨��ʱ�䷶Χ
#define	ERROR_FILE_EXPIRE								"F8003"	// �ļ���ʱ�䳬��(�ļ���ʱ�䳬��ϵͳ��ǰʱ��1��)
#define	ERROR_SERIAL_NUMBER							"F8004"	//�ļ�����Ų���ȷ
#define	ERROR_PROVINCE_NO								"F8005"	//�ļ��������ϴ�ʡ
#define	ERROR_FILE_NOT_SEQUENCE					"F9000"	// �ļ����ŵ�


const int BUFFSIZE = 8192;
const int TOTAL = 20;

//SearchOneFile�ķ���ֵ
const int FIND_NOFILE = 0;
const int FIND_NOREAD = -2;
const int FIND_PATHERROR = -1;
const int FIND_OK = 1;
const int FIND_FILTER_NOMATCH=-3;
const int FIND_IGNOREERROR = -4;


/*********************************************************************************/
/***                           Function Declare                                ***/
/*********************************************************************************/

	/*�ַ���ƥ�亯��*/
	bool Compare(const char *lv_chCompareString,const char *lv_chCompareMod);
	/*�ַ���ƥ�亯��*/
	bool Compare(const char *lv_chCompareString,const char *lv_chCompareMod,char *lv_ErrorMsg);
	/*��·����������*/
	int FullPath(char *chPath);
	/*�õ�ϵͳ��ǰ����(��λ��)*/
	void GetShortSystemDate(char *pchCurrentDate);
	/*�õ�ϵͳ��ǰ����*/	
	void GetSystemDate(char *CurrentDate);
	/*�õ�ϵͳ��ǰʱ��*/
	void GetSystemTime(char *CurrentTime);
	/*�õ�ϵͳ��ǰ����ʱ��(14λYYYYMMDDHHMMSS)*/
	void GetSystemDateTime(char *CurrentDateTime);
	/*�õ�ϵͳ��ǰ����ʱ��(12λYYMMDDHHMMSS)*/
	void GetShortSystemDateTime(char *CurrentDateTime);
	/*�õ�����IP*/
	bool GetLocalIp(char *LocalIp);
	/*��ȡ�ļ���С*/
	long GetFileSize(const char *chFileName);
	/*ȥ���ַ������ҵĿո�*/
	void Trim(char * String);
	/*���ַ���ȫ����Ϊ��д*/
	void MakeUpper(char * pchString);
	/*���ַ���ȫ����ΪСд*/
	void MakeLower(char * pchString);
	/*У��������ȷ��*/
	bool CheckDate(const char *pchString);
	/*У��ʱ����ȷ��*/
	bool CheckTime(const char *pchString);
	/*У������ʱ����ȷ��*/
	bool CheckDateTime(const char *pchString);
	/*ʱ����ת��Ϊ��*/
	long hhmmssToss(char * pchString);
	/*���ļ��ж�ȡһ������*/
	int GetLine(char *Line,FILE *File);
	/*�ж�����·���Ƿ����*/
	bool IsDirectory(const char *dirname);
	/*�����ļ������������һ���ļ�*/
	int SearchOneFile(const char *Pathname, const char *Format, char *Filename);
	int SearchOneFile(const char *Pathname, const char *Format, const int IgnoreFlag, char *Filename);
	int SearchOneFile(const char *Pathname, const char *Format, char *Filename, char *ErrorMsg);
	/*�����ļ���������Ӷ��Ŀ¼�в���һ���ļ�*/
	int SearchOneFile(const char Pathname[TOTAL][LENGTH], const char Format[TOTAL][LENGTH], const int TotalIn, char *Filename);
	bool SearchAllFiles(const char *Pathname, const char *Format, vector<string> &vecFileSet);
	bool SearchAllFiles(const char *Pathname, const char *Format, list<string> &lstFileSet);
	/*���ļ���һ��Ŀ¼�ƶ��������Ŀ¼������*/
	int MoveFile(const char * DesFullFileName, const char *SrcFullFileName);
	/*���ļ���һ��Ŀ¼�����������һ��Ŀ¼������*/
	int CopyFile(const char * DesFullFileName, const char *SrcFullFileName);
	/*��ȡ�ļ�������*/
	long GetFileRows(const char *pchFullFileName);
	/*��ȡ�̳�����*/
	void GetShortProcName(char *pchProcName);
	/*��ȡ�̳�������������·����*/
	void GetShortProcNameAndPath(char * pchProcNameWithPath, char *pchProcName, char * pchpath);
	//����
	void Encrypt(const char *pszPlain, char *pszCryptograph);
	//����
	void Decrypt(const char *pszCryptograph, char *pszPlain);
	//����ת���ַ�����ѭc style
	char* DecodeBackSlash(char* Str);
	/*У������->����ȷ��*/
	bool CheckDay(const char *pchString);
	/*У������->�·���ȷ��*/
	bool CheckMonth(const char *pchString);
	/*У������->������ȷ��*/
	bool CheckMonthDay(const char *pchString);
	/*У������->�����ȷ��*/
	bool CheckYear(const char *pchString);
	/*У������->������ȷ��*/
	bool CheckYearMonth(const char *pchString);
	/*ɾ��ָ��ͨ����ָ�������ļ�*/
	bool DeleteFile(const char *pFullPathName, const char *pBatchCode);
	void JudgeBusiClassAndType(char * chFileName, int iBusiClass, int iBusiType);
	void 	JudgeYcFlag(const int iBusiClass, const char * chRecordType, const int iDuring, int iYcFlag);
	int time_diff(char *pchStrBeginTime,char *pchStrEndTime);
	
	
#endif		//BASEFUN_H_



