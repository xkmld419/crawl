#ifndef _INCLUDE_RESULTCODE_H_
#define _INCLUDE_RESULTCODE_H_

const int MSG_RECEIVE_FAIL = -1001;				//��Ϣ����ʧ��
const int STRMSG_RECEIVE_PARSER_FAIL = -1002;	//string��Ϣ����ʧ��
const int AVPMSG_RECEIVE_PARSER_FAIL = -1003;	//avp��Ϣ����ʧ��

const int MSG_SEND_FAIL = -1011;			//��Ϣ����ʧ��
const int STRMSG_SEND_PACK_FAIL = -1012;	//string��Ϣ����ʧ��
const int AVPMSG_SEND_PACK_FAIL = -1012;	//avp��Ϣ����ʧ��

//-----------------ABM Result Code --------------------
const int SUCCEED = 0; 					//�ɹ�
const int SYSTEM_BUSY_ERROR = 1001;		//ϵͳæ�����Ժ�
const int FAIL = 1002; 					//ʧ��
const int EVENT_TYPE_ERROR = 1501;  	//�¼����ʹ���
const int EVENT_SOURCE_ERROR = 1502;	//�¼���ԴԴ����
const int MSG_TYPE_ERROR = 1503;		//��Ϣ���ʹ���string buffer��AVP buffer

//-----------------ABM Result Code --------------------

const int PARSER_FAIL = 2510;  			//��Ϣ����ʧ��

const int PARSER_STRBUFF_FAIL = 2511;	//string buffer��Ϣ����ʧ��

const int PARSER_AVPBUFF_FAIL = 2512;	//AVP buffer��Ϣ����ʧ��

const int PACK_FAIL = 2520;  			//��Ϣ���ʧ��

const int PACK_STRBUFF_FAIL = 2521;		//string buffer��Ϣ���ʧ��

const int PACK_AVPBUFF_FAIL = 2522;		//AVP buffer��Ϣ���ʧ��

#endif



