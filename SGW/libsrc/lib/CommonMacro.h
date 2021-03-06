#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#define DB_LINK Environment::getDBConn()  //数据库连接宏定义

const int MAX_STRING_VALUE_LEN = 80;
const int MAX_SQL_WHERE_LEN	= 200;
const int MAX_AVPSTR_BUF = 4096;	// AVP串的缓冲大小
const int MAX_XML_TREE_BUF_LENGTH = 4096;

const int NUMBER = 2;
const int STRING = 3;

const int CONDITION_HASH_VALUE = 2000;

//pid len
const int PID_LEN = 10;

//事件来源
const int OCS_EVENT = 101;				//OCS请求
const int HB_EVENT  = 102;				//HB请求
const int VC_EVENT  = 103;				//VC请求
const int WSS_EVENT = 104;				//WSS请求
const int CRM_EVENT = 105;				//CRM请求

const int ALERT_SIG_ERR = 100;  //signal函数定义信号处理时失败

#define  MAX_GATE_WAY_LEN       11    //网关编码长度
#define  MAX_RP_STR_LEN         16    //基站编码长度

#endif // COMMON_MACRO_H




