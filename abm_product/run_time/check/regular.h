/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�regular.h 
* ժ    Ҫ���ļ�������regular expressģ��
* 
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��11��10��
*/
#ifndef __REGULAR_H_
#define __REGULAR_H_

#include <regex.h>
#include <string.h>
#include "stack.h"

#ifndef NULL
#define NULL 0
#endif

class Regex {
public:
	Regex(const char* reg) {
		memset(errbuf, 0x00, sizeof(errbuf));
		regexComp(reg);
	}	

	virtual ~Regex() {
		if (errbuf) {
			delete errbuf;
		}

		regfree(&regex);
	}

	int exec(const char* str);

protected:
	Regex() {
		memset(errbuf, 0x00, sizeof(errbuf));
	}


	int regexComp(const char* reg);
public:
	regex_t 		regex; 			/* ������������ʽ */ 
	regmatch_t 		pMatchs; 		/* ƥ����ַ���λ�� */ 
	char 			errbuf[28]; 	/* ������Ϣ */ 
	int 			compNo;
	int 			execNo;
};

#endif

