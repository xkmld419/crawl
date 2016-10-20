/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�checkFunc.h 
* ժ    Ҫ���ļ�����������У�����õ����к���
* 
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��11��11��
*/

#ifndef __CHECK_FUNC_
#define __CHECK_FUNC_

#include "string.h"
#include "Date.h"


/* ************ ����ֵ�Ƚ�*********** */
int compare_H(const int & src, const int & dest);
int compare_H(const long & src, const long & dest);
int compare_H(const char* src, const char* dest);

/* ************ ����ֵ�Ƚ�*********** */
inline int biggerDateCheck_H(const char* src, const char* dest, const long & second){
	if (!src) return -1;
	
	long s = second;

	if (strcmp(src, dest) <= 0) return 0;

	
	Date srcDate(src);
	Date destDate;
	if (!dest) destDate.parse(dest);

	return destDate.diffSec(srcDate) >= second ? 0 : 1;
}

/* ************ ����ֵ�Ƚ�*********** */
inline int lowerDateCheck_H(const char* src, const char* dest, const long & second) {
	if (!src) return -1;
	
	long s = second;

	if (strcmp(src, dest) >= 0) return 0;

	
	Date srcDate(src);
	Date destDate;
	if (!dest) destDate.parse(dest);

	return srcDate.diffSec(destDate) >= second ? 0 : 1;
}

#endif

