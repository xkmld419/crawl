#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

/********************************************************
*
*                    ϵͳ����
*˵��:  
*       ϵͳ������ֲ���Χ��0~1999��
*
*********************************************************/

#define ECODE_ABM_SUCCESS       0           //�ɹ� 

#define ECODE_ABM_TIMEOUT       1001        //ϵͳ��ʱ

#define ECODE_ABM_FAILURE       1002        //ϵͳ����

#define ECODE_ENV_LOST          1003        //����������ʧ

#define ECODE_RDCFG_FAIL        1100        //�ͻ��˶�ȡ�����ļ�����

#define ECODE_PARAM_INVALID     1101        //�ͻ��˲���ʧЧ

#define ECODE_MQ_OP_FAIL        1102        //�ͻ�����Ϣ���в���ʧ��

#define ECODE_MQ_NOMSG          1103        //�ͻ�����Ϣ����Ϊ��

#define ECODE_SIGREG_FAIL       1104        //�ͻ����ź�ע���쳣

#define ECODE_FTP_SEND_ERR      1105        //�ͻ���FTP�����쳣

#define ECODE_FTP_RECV_ERR      1106        //�ͻ���FTP�����쳣

#define ECODE_DISPATCH_FAIL     1107        //�ͻ��˷ַ��쳣

#define ECODE_MALLOC_FAIL       1108        //�ͻ����ڴ����ʧ��

#define ECODE_SHARE_MEM_FAIL    1109        //�ͻ��˹����ڴ����ʧ��

#define ECODE_SOCKET_EXCEPTION  1110        //SOCKET�����쳣

#define ECODE_RCVMQ_BLOCK       1111        //������Ϣ�������������

#define ECODE_GETFTP_INFOR_FAIL 1112        //��ȡ�ڴ���FTP��Ϣʧ��

#define ECODE_FTP_LOGIN_FAIL    1113        //FTP��½ʧ��

#define ECODE_FTP_LOAD_FAIL     1114        //FTP����ʧ�� 

#define ECODE_ACCU_DEALFILE_FAIL 1115      //FTP�ļ�����ʧ��

#define ECODE_SRV_MQ_NOMEM       1116     //�������Ϣ�������

#define ECODE_CLNT_MQ_NOMEM      1117     //������������Ϣ���ͻ��˶���ʧ��

#define ECODE_CLNT_RCVMQ_FAIL    1118     //�ͻ��˽�����Ϣ����ʧ��

#define ECODE_CLNT_SNDMQ_FAIL    1119     //�ͻ��˷�����Ϣ����ʧ��

#define ECODE_CLNTSND_TIMEOUT    1120     //�ͻ��˷��ͳ�ʱ
 

#define ECODE_MEM_DB_ERROR      1900        //�ڴ����ݿ����

#define ECODE_TIMEOUT           ECODE_ABM_TIMEOUT


//FTP����������
#define ECODE_INIT_FTP_FAIL  1801
#define ECODE_GET_FTP_INFO   ECODE_INIT_FTP_FAIL + 1
#define ECODE_FTP_CHECK_LOCAL_DIR ECODE_GET_FTP_INFO + 1
#define ECODE_FTP_LOGIN_IN ECODE_FTP_CHECK_LOCAL_DIR + 1   //��¼FTP ����
#define ECODE_FTP_SET_ATTR ECODE_FTP_LOGIN_IN + 1          //����FTP���Գ���
#define ECODE_FTP_REMOTE_DIR ECODE_FTP_SET_ATTR + 1        //���ҷ�����Ŀ¼����
#define ECODE_FTP_GET_FILE_ATTR ECODE_FTP_REMOTE_DIR + 1   //��ȡԶ���ļ���Ϣʧ��
#define ECODE_FTP_GET_FILE ECODE_FTP_GET_FILE_ATTR + 1
#define ECODE_FTP_DEL_ROMOTE_FILE ECODE_FTP_GET_FILE + 1
#define ECODE_FTP_PUT_FILE ECODE_FTP_DEL_ROMOTE_FILE + 1
#define ECODE_FTP_DEL_LOCAL_FILE ECODE_FTP_PUT_FILE + 1

#define ECODE_SRV_GET_FTP_FAIL ECODE_FTP_DEL_LOCAL_FILE + 1



// ���������� ���ڿͻ���ѯ��ABM������Ƿ�ִ�й�ĳ������, �����m_iRltCode=ECODE_ABM_CMD_REPEAT
//��ʾ������ִ�й�(Ŀǰ�����ڲ�ѯ�ۻ������˸�������)
#define ECODE_ABM_CMD_REPEAT    1901         

/********************************************************
*
*                   ��Ϣ����
*˵��:  
*      �ô�����Ϊ���Ź涨���ο���������ƽ̨V1.0�ĵ�,
*      �ֲ���Χ��2000~2999��
*********************************************************/

#define ECODE_MSG_FORMAT_ERROR        2000        //��Ϣ��ʽ����

#define ECODE_MSG_TYPE_ERROR          2001        //��Ϣ���ʹ���

#define ECODE_MSG_PARA_LOST           2002        //����ȱʧ

#define ECODE_MSG_PARA_NULL           2003        //����Ϊ��

#define ECODE_MSG_PARA_INVALID        2004        //������Ч

#define ECODE_MSG_OVERSIZED           2005        //��Ϣ�������

/********************************************************
*
*                   ҵ��ͨ�ô���
*˵��:  
*      �ô�����Ϊ���Ź涨���ο���������ƽ̨V1.0�ĵ�,
*      �ֲ���Χ��3000~3999��
*********************************************************/

#define ECODE_TRANS_ERROR          3001          //���ױ�ʶ����

/********************************************************
*                   ҵ�����
*˵��:  
*    1 ����ۻ������ô�����,�ֲ���Χ��4000 ~ 4099������
*      ������Ϊ���Ź涨���ο���������ƽ̨V1.0�ĵ�;
*    2 ������ֲ���ΧΪ��4100 ~ 4599��,�ô�����Ϊ��
*      �ҹ涨��
*    3 �ۻ���������ֲ���ΧΪ��4600 ~ 4999��,�ô�����Ϊ��
*      �ҹ涨��
*
*********************************************************/

#define ECODE_SERV_NOT_EXIST          4001      //�û�������

#define ECODE_SERV_INFO_ERROR         4002      //�û���Ϣ�쳣      

#define ECODE_CUST_NOT_EXIST          4003      //�ͻ�������      

#define ECODE_CUST_INFO_ERROR         4004      //�ͻ���Ϣ�쳣     

#define ECODE_ACCT_NOT_EXIST          4005      //�˻�������       

#define ECODE_ACCT_INFO_ERROR         4006      //�˻���Ϣ�쳣       

#define ECODE_ACCT_SERV_CUST_RELA_ERROR  4007      //�û��˻��ͻ���������       

#define ECODE_ADJUST_TYPE_FLAG_ERROR  4008      //�ؿ۱�ʶ����       
/*********************************************************
*               ������
***********************************************************/

#define ECODE_NO_BALANCE           4100         //û�����

#define ECODE_BAL_NOT_ENOUGH       4101         //Ԥ������ɹ������

#define ECODE_BAL_NOT_ENOUGH_FAIL  4102         //�������ʧ�ܵ����

#define ECODE_DATA_NOTBALANCE      4103         //����˱�����Դ��һ��

#define ECODE_ACCT_LOCK_FAIL       4104        //ACCT�����ڴ����ʧ��

#define ECODE_BUSINESS_OVRERLOADED  4105        //ҵ���ع��󣬴���ʧ��

#define ECODE_DATA_DIFF             4106        //���ݲ���

#define ECODE_FIFE_OPER_FAIL        4107        //�ļ�����ʧ��

#define ECODE_FRESH_REGISTER_ACCT   4108        //ABM��ע���ʻ���

#define ECODE_CHECK_EM_DED          4109       //������ĩ�Żݷ��ؽ��

/*********************************************************
*               �ۻ���������
***********************************************************/

#define ECODE_ACCU_AUDIT_FAIL     4700          //�ۻ�������ʧ��

#define ECODE_ACCU_KEY_DISABLED   4701          //�ۻ���������ABM������

#define ECODE_ACCU_VERSION_EXCEED 4702          //�ۻ����汾Խ��

#define ECODE_ACCU_UPLOAD_FAIL    4703          //�ۻ�������ʧ��

#define ECODE_ACCU_LOAD_FAIL      4704          //�ۻ�������ʧ��

#define ECODE_ADTQRY_ZERO_ERROR   4705          //�����˰汾Ϊ0�Ĳ�����δ�鵽��

#define ECODE_ADTQRY_EMPTY_ERROR   4706          //������δ�鵽�κ�����

#define ECODE_ACCU_PACK_FAIL      4900          //�ۻ������ʧ��

#define ECODE_DETAIL_UPDATE_FAIL  4901          //�Ż��ۻ�����ϸ�ͻ��ܲ�һ��

#define ECODE_ADTQRY_PARAM_ERROR  4902          //���˸��²Ų�ѯ�����ݰ���������

#define ECODE_ACCU_QRY_NULL       4903          //��ѯ���ذ�ΪNULL �ô˴�����

#define ECODE_ACCU_CLNT_UPACK_FAIL 4904         //�ͻ��˽��ʧ��


#define ECODE_ACCU_LOAD_FREQUENT  4905          //�ۻ���������ִ��

#define ECODE_ACCU_NO_RESOURCE    4906          //�ۻ������������Դ���� 

//#define ECODE_ACCU_LOAD_FAIL      4907          //�ۻ�������ʧ��

#define ECODE_ACCULOAD_UNVALID_NODE  4908         //�ۻ������� ��Ч�Ľڵ�����

#define ECODE_ACCU_BACKUP_FAIL      4909          //�ۻ�������ʧ��
#define ECODE_ACCU_BACKUP_WORKING   4910          //�ۻ�����ʼ����
#define ECODE_ACCUBACKUP_UNVALID_NODE 4911        // �����ļ��޸ýڵ㼰��������
#define ECODE_ACCU_BACKUP_FREQUENT  4912          //�ۻ���������ִ��

/********************************************************
*                   �ڴ����ݿ����
*
*********************************************************/

#define MEM_DB_ERROR              8000          //�ڴ����ݿ����
#endif
