/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�common.h 
* ժ    Ҫ���ļ�������һЩ���õ��������ͻ���
* 
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��11��10��
*/
#ifndef __COMMON_H_
#define __COMMON_H_
#include "HSSLogV3.h"
#include "LogV2.h"

/* ************** ��������************ */
#define MBC_DOCHECK_CONNECTDB				99001
#define MBC_DOCHECK_CURSORINVALID			99002
#define MBC_DOCHECK_EXECDBFAULT				99003

#define MBC_DOCHECK_PARAMINVALID			99011
#define MBC_DOCHECK_ALLOCATEMEMERY			99012

#define MBC_DOCHECK_OPENFILE				99021

#define MBC_DOCHECK_FINDRULEFAILED			99901
#define MBC_DOCHECK_FINDURULEFAILED			99902
#define MBC_DOCHECK_VALIDREGEX				99903

/* ************* ���ú궨��********** */
#define getParamValues(p) p.type == type_int ? p.values.iData : (p.type == type_long ? p.values.lData : p.values.sData)
#define getParamLen(p)    p.type == type_string? strlen(p.values.sData) : (p.type == type_long ? sizeof(long) : sizeof(int))
#define getValueParamType(p, v) p.type == type_int ? atoi(v) : (p.type == type_long ? atol(v) : v)


/* ************** ���Ͷ���************ */
typedef int (*checkFunc)(int argc, char* argv[]);


/* ************ У��ֵ����*********** */
enum Param_t {
	type_int						= 1,
	type_long						= 2,
	type_string						= 3,
};

union Param_v {
	int 	iData;
	long	lData;
	char*	sData;
};

struct Param {
	Param_t type;
	Param_v values;
};

/* ************* ����id����************ */
enum TFuncId {
	FUNC_ID_DEFUALT					= 0,
	FUNC_ID_HASH					= 1,
	FUNC_ID_MAXVALUES				= 2,
	FUNC_ID_MINVALUES				= 3,
	FUNC_ID_MAXLEN					= 4,
	FUNC_ID_MINLEN					= 5,
	FUNC_ID_REGEX					= 6,
	FUNC_ID_CURDATE_BIGLIMIT		= 7,
	FUNC_ID_CURDATE_SMALLLIMIT		= 8,
	FUNC_ID_SPEDATE_BIGLIMIT		= 9,
	FUNC_ID_SPEDATE_SMALLLIMIT		= 10,

	FUNC_ID_END,
};

#endif

