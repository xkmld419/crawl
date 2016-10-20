#ifndef __ERROR_CODE_H_INCLUDED_
#define __ERROR_CODE_H_INCLUDED_

#define ECODE_NOMSG    4001
#define ECODE_UNPACK_FAIL 4002
#define ECODE_PACK_FAIL   4003
#define ECODE_TT_FAIL     4004
#define QUERY_FAIL_OTHER  13005         //ȫ����������ѯ�޼�¼���߶�����¼
#define TT_NO_RECORD      13013
#define OVERDUE_FLG       238
#define QUERY_BILL_FAIL   188
/*
��ѯҵ�񲿷ִ�����-���� ����ѯ�����˼�¼��ѯ�����׼�¼��ѯ�������˵���ѯ��
������ѯ
*/
#define  QRY_ECODE_NOMSG_ERR 13700       //������Ϣ��Ϊ��
#define  QRY_ECODE_UNPACK_FAIL 13701     //������Ϣ��������
#define  QRY_USER_TYPE_ERR     13702     //�û�����Ϊ1��Ŀǰֻ֧��2
#define  QRY_BALANCE_NODATA_ERR 13703    //��ѯȫ��ABM���û������
#define  QRY_MUL_RECORD_ERR     13704    //��ѯȫ��ABM����ж�����¼��Ŀǰֻ֧��1��
#define  QRY_GET_SEQUENCE_FAIL_ERR 13705 //��ȡ����DCCͷ�����к�ʧ��
#define  QRY_INSERT_DCC_FAIL_ERR 13706   //����DCCͷ��Ϣʧ��
#define  QRY_SAVEABMDATA_FAIL_ERR 13707  //����ȫ������ABM����ʧ��
#define  QRY_PACKMSG_TO_PROC_ERR  13708  //������͵�ʡ���������Ϣʧ��
#define  QRY_GET_DCC_FAIL_ERR 13709      //��ѯDCC��Ϣͷʧ��,���ݿ����ʧ��
#define  QRY_GET_DCC_NOMSG_ERR 13710     //��ѯDCC��Ϣû��¼�����Ҫ����������֪����ô����
#define  QRY_GET_ABMDATA_FAIL_ERR 13711  //��ѯ�����ȫ��ABM����ʧ�ܣ����ݿ����ʧ��
#define  QRY_GET_ABMDATA_NODATA_ERR 13712 //��ѯ�����ȫ��ABM�����޼�¼
#define  QRY_UNPACK_PROC_FAIL_ERR 13713  //����ʡ�ﷵ�ص���Ϣʧ��
#define  QRY_PACK_SERVICE_FAIL_ERR 13714 //������Է���ƽ̨�ķ�����Ϣʧ��
//����ѯ
#define  QRY_PACK_FAIL_ERR  13715        //������ؽ������Ϣʧ��
#define  QRY_PACK_DATA_ERR 13716         //�������ѯ���ݲ���ʧ��

//���˼�¼��ѯ
#define  QRY_PAY_PACK_FAILE 13717        //���˼�¼��ѯ���ʧ��
#define  QRY_PAY_NORECORD_ERR 13718      //���˼�¼��ѯ�޼�¼
#define  QRY_PAY_PACKRLT_FAIL 13719      //���˼�¼������ؽ����ʧ��
#define  QRY_PAY_PACKDATA_FAIL 13720     //������˼�¼����ʧ��

//���׼�¼��ѯ
#define  QRY_REC_UNPACK_FAIL 13721       //����������Ϣ��¼���ʧ��
#define  QRY_REC_NORECORD_ERR 13722      //���׼�¼��ѯ�޽��
#define  QRY_REC_PACK_FAIL   13723       //���׼�¼������ؽ����ʧ��
#define  QRY_REC_PACKDATA_FAIL  13724    //���׼�¼�������ʧ��

//�����˵���ѯ
#define  QRY_BIL_UNPACK_FAIL 13725       //�����˵�������Ϣ��¼���ʧ��
#define  QRY_BIL_NORECORD_ERR 13726      //�����˵���¼��ѯ�޽��
#define  QRY_BIL_PACK_FAIL   13727       //�����˵���¼������ؽ����ʧ��
#define  QRY_BIL_PACKDATA_FAIL  13728    //�����˵���¼�������ʧ��
#define  QRY_BIL_PREPARE_FAIL  13729     //Ԥ����sqlʧ��

//����ѯ
#define  QRY_BAL_UNKNOWNTYPE   13730    //δʶ����û�����
#define  QRY_BAL_SERV_NOTDIGIT 13731    //����ѯ���û�ID��ѯ���û�ID������
#define  QRY_BAL_NORECORD      13732    //����ѯ�޼�¼
#define  QRY_BAL_MULRECORD     13733    //����ѯ������¼


#endif/*__ERROR_CODE_H_INCLUDED_*/
