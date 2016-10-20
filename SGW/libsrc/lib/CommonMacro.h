#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#define DB_LINK Environment::getDBConn()  //���ݿ����Ӻ궨��

const int MAX_STRING_VALUE_LEN = 80;
const int MAX_SQL_WHERE_LEN	= 200;
const int MAX_AVPSTR_BUF = 4096;	// AVP���Ļ����С
const int MAX_XML_TREE_BUF_LENGTH = 4096;

const int NUMBER = 2;
const int STRING = 3;

const int CONDITION_HASH_VALUE = 2000;

//pid len
const int PID_LEN = 10;

//�¼���Դ
const int OCS_EVENT = 101;				//OCS����
const int HB_EVENT  = 102;				//HB����
const int VC_EVENT  = 103;				//VC����
const int WSS_EVENT = 104;				//WSS����
const int CRM_EVENT = 105;				//CRM����

const int ALERT_SIG_ERR = 100;  //signal���������źŴ���ʱʧ��

#define  MAX_GATE_WAY_LEN       11    //���ر��볤��
#define  MAX_RP_STR_LEN         16    //��վ���볤��

#endif // COMMON_MACRO_H



