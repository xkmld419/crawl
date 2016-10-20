/***********************************************************
 *
 *   Copyright (c) 1997-2010  Asiainfo-Linkage Technology Co.,Ltd.
 *
 *   All rights reserved.
 *
 *   �ļ����ƣ�CPacketComm.h
 *
 *   ��Ҫ������Э������������
 *
 *   ��ǰ�汾��1.0
 *
 *   ����/�޸��ߣ��ָ���  lingy@asiainfo-linkage.com
 *
 *   ��д���ڣ�2010/10/30
 *
 ************************************************************/

#ifndef __PACKET_COMM_H__
#define __PACKET_COMM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <exception>
#include <string>
#include <vector>
#include <iconv.h>
#include <iostream>
#include <errno.h>

using namespace std;

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char uchar;
//typedef char	int8;				/* Signed integer >= 8	bits */
typedef short int16; /* Signed integer >= 16 bits */
typedef unsigned char uint8; /* Short for unsigned integer >= 8  bits */
typedef unsigned short uint16; /* Short for unsigned integer >= 16 bits */
typedef int int32;
typedef unsigned int uint32; /* Short for unsigned integer >= 32 bits */
typedef unsigned long ulong;
typedef unsigned long long ulonglong;
typedef char* PBYTE;
//typedef char BYTE;
//typedef char TCHAR;
typedef unsigned int UINT;
//typedef bool BOOL;
//typedef int  HANDLE;
typedef void * LPVOID;
typedef char byte;

//Starting here, for Big Endian defition
#define storeInt(T, A) {	\
	*((T)+3) =	(uchar) ((A));	\
	*((T)+2) = (uchar) (((A) >> 8));	\
	*((T)+1) = (uchar) (((A) >> 16));	\
	*(T) = (uchar) (((A) >> 24)); }

#define storeInt2(T, A) {	\
	*((T)+1) = (uchar) (((A)));	\
	*(T) = (uchar) (((A) >> 8)); }

#define readInt(A)	((int) (	\
				    (((int) ((uchar) (A)[0])) << 24) +\
				    (((uint32) ((uchar) (A)[1])) << 16) +\
				    (((uint32) ((uchar) (A)[2])) << 8) +\
					((uchar) (A)[3])))

#define readInt2(A)	((int) (	\
				    (((int) ((uchar) (A)[0])) << 8) +\
					((uchar) (A)[1])))

//��ת��������С
const int MAX_CONVBUF_LEN = 4096;
const int MILLISECONDS_PER_SECONDS = 1000; //1��=1000����
const int MICROSECONDS_PER_MILLSECONDS = 1000; //1����=1000΢��
const int MICROSECONDS_PER_SECONDS = 1000000; //1��=1000000΢��

/*
 char C_EVENT_RETURN = '\n';
 char C_EVENT_SPLIT = '|';
 string STR_EVENT_SPLIT = "|";
 */
//����ת������
typedef enum
{
	EN_NO_CONVERT = 0, //��ת,���ֲ���
	EN_Utf8ToGB2312 = 1,
	EN_GB2312ToUtf8 = 2,
	EN_Utf8ToGBK = 3,
	EN_GBKToUtf8 = 4,
	EN_Ucs2ToGB2312 = 5,
	EN_GB2312ToUcs2 = 6,
	EN_Ucs2ToGBK = 7,
	EN_GBKToUcs2 = 8
} E_CODE_CONVERT_SCHEMA;

typedef enum
{
	CODE_SCHEMA_TUXC_TO_SGWS = EN_Utf8ToGBK, //�ⲿTUXEDO�ͻ��˵�SGW�����
	CODE_SCHEMA_SGWS_TO_TUXC = EN_GBKToUtf8,//SGW����˵��ⲿTUXEDO�ͻ���
	CODE_SCHEMA_SGWC_TO_TUXS = EN_GBKToUtf8,//SGW�ͻ��˵��ⲿTUXED�����
	CODE_SCHEMA_TUXS_TO_SGWC = EN_Utf8ToGBK
//�ⲿTUXEDO����˵�SGW�ͻ���
} E_CODE_CONVERT_DIRECTION;

typedef enum
{
	EN_EVENT_SPLIT = 1, EN_EVENT_RETURN = 2

} E_RECORD_STYLE;

//����������
class CPacketUtil
{
public:
	CPacketUtil();
	~CPacketUtil();
private:
	//����ת��
	int convert(const char *sFromCharset,const char *sToCharset,const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);
public:
	//����ת��
	int convert(int iConvertSchema,const char *sInBuf, char *sOutBuf, int &iOutLen);
	//����ת��
	int convert(int iConvertSchema,const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);

	//����������
	int calcbuf2(char *buf, bool bTrans);
	int calcbuf3(char *buf, bool bTrans);
	int calcbuf4(char *buf, bool bTrans);
	void Int32toArray(char *buf, uint32 uiValue);
	uint32 ArraytoInt32(char *buf);
	void Int16toArray(char *buf, uint16 uiValue);
	uint16 ArraytoInt16(char *buf);

	//���㺺���ַ�����
	int ChineseCharCount(const char *sMsg, int iFirst, int iLast);
	//��Ϣ���
	int splitMessage(const char *sMsgContent, int iMsgLen, int iSingleMsgLen, vector<string> &vOutMsg);
	//��¼�ֽ�
	int splitRecord(string &in_sOneEvent, const string &in_sSep, int in_iStyle, vector<string> &out_vFieldList);

	//UTF8��תΪGB2312��
	int Utf8ToGB2312(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);
	//GB2312��תΪUTF8��
	int GB2312ToUtf8(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);

	//UTF8��תΪGBK��
	int Utf8ToGBK(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);
	//GBK��תΪUTF8��
	int GBKToUtf8(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);

	//UCS2��תΪGB2312��
	int Ucs2ToGB2312(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);
	//GB2312��תΪUCS2��
	int GB2312ToUcs2(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);

	//UCS2��תΪGBK��
	int Ucs2ToGBK(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);
	//GBK��תΪUCS2��
	int GBKToUcs2(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);

	//ֻ����,��ת��
	int copy(const char *sInBuf, char *sOutBuf, int &iOutLen);
	//ֻ����,��ת��
	int copy(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);

	// ϵͳ����תUTF8
	int Sys2Utf(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);
	// UTF8תϵͳ����
	int Utf2Sys(const char *sInBuf, int iInLen, char *sOutBuf, int &iOutLen);
};

#endif  /* __PACKET_COMM_H__ */
