#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

/********************************************************
*  ���ļ������ˡ��й����żƷ���ȫ�����������루V1.01����
*  �Ĺ������ֺ�HSS���֣���������밴�ո���ģ�����д���
*								2011.5.30   liqf
*********************************************************/


/********************************************************
*                   �������ش���
*					��1--100��
*********************************************************/
#define ECODE_MEM_MALLOC_FAIL						1		//�ڴ����ʧ��
#define ECODE_MEM_SPACE_SMALL						2		//�ڴ�ռ䲻��
#define ECODE_MEM_INTERFACE_ERROR					3		//�ڴ���ӿڴ���
#define ECODE_MEM_LINK_FAIL							4		//�����ڴ�����ʧ��
#define ECODE_THREAD_CREAT_FAIL						5		//�������̻��߳�ʧ��
#define ECODE_SEMP_OPERA_FAIL						6		//�źŴ���ʧ��
#define ECODE_LOCK_GET_FAIL							7		//��ȡ����Դʧ��
#define ECODE_SOCKET_LISTEN_FAIL					8		//Socket--��������
#define ECODE_SOCKET_LINK_FAIL						9		//����Զ��Socket��
#define ECODE_DISC_SPACE_SMALL						10		//���̴洢�ռ䲻��
#define ECODE_CPU_OVERLOAD							11		//CPU������
#define ECODE_DB_LINK_FAIL							12		//�������ݿ�ʧ��
#define ECODE_DB_TABLESPACE_SMALL					13		//��ռ䲻��
#define ECODE_ENV_NOT_SET							14		//��������δ����
#define ECODE_SYS_BUSY								15		//ϵͳ��æ
/********************************************************
*                   ϵͳ���д���
*					��101--300��
*********************************************************/
#define ECODE_FILE_OPEN_FAIL						101		//���ļ�ʧ��
#define ECODE_FILE_WRITE_FAIL						102		//д�ļ�ʧ��
#define ECODE_DIR_OPEN_FAIL							103		//��Ŀ¼����
#define ECODE_FILE_RENAME_FAIL						104		//�ļ���������
#define ECODE_FILE_SEQ_GET_FAIL						105		//���л�ȡʧ��
#define ECODE_FILE_NOT_EXIST						106		//ָ���ļ�������
#define ECODE_FILE_COPY_FAIL						107		//�����ļ�ʧ��
#define ECODE_FILE_MOVE_FAIL						108		//�ƶ��ļ�ʧ��

#define ECODE_MEM_UPDATE_FAIL						116		//�����ڴ����ʧ��
#define ECODE_MEM_IMPORT_FAIL						117		//�����쳣   hbimport����
#define ECODE_MEM_EXPORT_FAIL						118		//�ڴ����ݵ����쳣 hbexport
#define ECODE_MEM_CREATE_FAIL						119		//���������ڴ�ʧ��
#define ECODE_MEM_DELETE_FAIL						120		//ɾ�������ڴ�ʧ��
#define ECODE_MEM_DESTROY_FAIL						121		//���ٹ����ڴ�ʧ��
#define ECODE_SEMP_CREATE_FAIL						122		//�����ź���ʧ��

#define ECODE_DB_SERV_PWD_ERROR						141		//�û����������
#define ECODE_DB_LINK_CLOSE_FAIL					142		//�ر����ݿ�����ʧ��
#define ECODE_DB_TABLE_NOT_EXIT						143		//����������
#define ECODE_DB_ROLLBACK_FAIL						144		//���ݿ�����ع�����
#define ECODE_DB_EXECUTE_FAIL						145		//����sqlִ���쳣
#define ECODE_DB_TICKET_IMPORT_FAIL					146		//���������ݿ�ʧ��


#define ECODE_NET_PROTOCOL_PACKAGE_FAIL				156		//Э���װʧ�ܣ�Э������%s������ԭ��%s.
#define ECODE_NET_PROTOCOL_PARSE_FAIL				157		//Э�����ʧ�ܣ�Э������%s������ԭ��%s.
#define ECODE_NET_TUXEDO_CALL_FAIL					158		//tuxedo����ʧ�ܣ�ʧ��ԭ��%s

#define ECODE_PARAM_FILE_FORMAT_ERROR				166		//�����ļ���ʽ����
#define ECODE_PARAM_THREAD_LOST_ERROR				167		//Ӧ�ý��̲�������ȷ��ȱ�ٱ�ѡ�
#define ECODE_PARAM_THREAD_NONSUPP_ERROR			168		//Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�
#define ECODE_PARAM_THREAD_VALUE_ERROR				169		//Ӧ�ý��̲�������ȷ�������ȡֵ��

#define ECODE_PARAM_UPDATE_FAIL						170		//��������ʧ��
#define ECODE_PARAM_FILE_ERROR						171		//�����ļ�ȱʧ
#define ECODE_PARAM_LOST_ERROR						172		//����������Ϣȱʧ
#define ECODE_PARAM_VALUE_NULL_ERROR				173		//�ֶβ���Ϊ��
#define ECODE_PARAM_LEN_MIN_ERROR					174		//С����С����
#define ECODE_PARAM_LEN_MAX_ERROR					175		//������󳤶�
#define ECODE_PARAM_LEN_FIXED_ERROR					176		//�ǹ̶�����
#define ECODE_PARAM_VALUE_ENUM_ERROR				177		//ö��ֵ����ȷ
#define ECODE_PARAM_VALUE_MIN_ERROR					178		//С����Сֵ
#define ECODE_PARAM_VALUE_MAX_ERROR					179		//�������ֵ
#define ECODE_PARAM_ERROR							180		//��������


#define ECODE_INFO_CUST_GET_FAIL					181		//ȡ�ͻ���Ϣʧ��
#define ECODE_INFO_ACCT_GET_FAIL					182		//ȡ�ʻ���Ϣʧ��
#define ECODE_INFO_SERV_GET_FAIL					183		//ȡ�û���Ϣʧ��
#define ECODE_INFO_BALANCE_GET_FAIL					184		//ȡ���ʧ��
#define ECODE_INFO_CURR_BALANCE_GET_FAIL			185		//ȡʵʱ����ʧ��
#define ECODE_INFO_HIS_OWE_GET_FAIL					186		//ȡ��ʷǷ��ʧ��
#define ECODE_INFO_CREDIT_GET_FAIL					187		//ȡ���ö�ʧ��
#define ECODE_INFO_BILL_GET_FAIL					188		//���˵�ʧ��
#define ECODE_INFO_TICKET_GET_FAIL					189		//���嵥ʧ��
#define ECODE_INFO_INVOICE_GET_FAIL					190		//�鷢Ʊʧ��


#define ECODE_HREAD_EXIST_ERROR						196		//Ӧ�ý����Ѿ�����
#define ECODE_HREAD_ERROR							197		//Ԥ��������쳣
#define ECODE_HREAD_STATUS_ERROR					198		//����״̬�Ƿ�
#define ECODE_HREAD_EXIT_ERROR						199		//Ӧ�ó����յ��˳��ź��쳣�˳�

#define ECODE_SRVC_BILLINGCYCLE_GET_FAIL			211		//��ȡ��Ӧ����ʧ��
#define ECODE_SRVC_ARCHIVES_MEM_UPDATE_FAIL			212		//CRM�����ӿڸ����ڴ�ʧ��
#define ECODE_SRVC_SEQ_CONTINUE_FAIL				213		//���������У��ʧ��
#define ECODE_SRVC_SOURCE_EVENT_ERROR				214		//Դ�¼����Ͳ��Ϸ�
#define ECODE_SRVC_TICKET_FROMAT_TRANS_FAIL			215		//������ʽת��ʧ��
#define ECODE_SRVC_BILL_INIT_FAIL					216		//�Ʒ�Ӧ�ó�ʼ��ʧ��
#define ECODE_SRVC_PRICING_GET_FAIL					217		//��ȡ���ۼƻ���Ϣ����
#define ECODE_SRVC_PRICING_EXECUTE_FAIL				218		//���ۼƻ��������
#define ECODE_SRVC_BILL_ROLLBACK_FAIL				219		//�ƷѴ������ʧ��
#define ECODE_SRVC_RECOVE_FAIL						220		//�������ʧ��
#define ECODE_SRVC_TICKET_DISTRIBUTE_FAIL			221		//�����ַ�ʧ��
#define ECODE_SRVC_ACCU_FAIL						222		//��������
#define ECODE_SRVC_BALANCE_UPDATE_FAIL				223		//�������ʧ��
#define ECODE_SRVC_CURR_BALANCE_UPDATE_FAIL			224		//����ʵʱ����ʧ�ܣ��磺����ʵʱ�����ر�ɷ������崦����
#define ECODE_SRVC_OWE_UPDATE_FAIL					225		//����Ƿ��ʧ��
#define ECODE_SRVC_STOPDATA_CREATE_FAIL				226		//����ͣ������ʧ��
#define ECODE_SRVC_REGAINDATA_CREATE_FAIL			227		//���ɸ�������ʧ��
#define ECODE_SRVC_REMIND_PAYMENT_CREATE_FAIL		228		//���ɴ߽�����ʧ��
#define ECODE_SRVC_INVOICE_FORMAT_FAIL				229		//ȡ��Ʊ��ʽ����
#define ECODE_SRVC_INVOICE_PRINT_FAIL				230		//��Ʊ��ӡʧ��
#define ECODE_SRVC_INTERFACE_FAIL					231		//��Χ�ӿڹ���(�糬ʱ��)
#define ECODE_SRVC_RECEIVE_FAIL						232		//ϵͳ�����쳣����
#define ECODE_SRVC_PAYMENT_MONEY_MAX_ERROR			233		//�ɷѽ����������
#define ECODE_SRVC_BANK_BILL_FAIL					234		//���д�������ʧ��
#define ECODE_SRVC_BATCH_BILL_FAIL					235		//��������ʧ��
#define ECODE_SRVC_BATCH_REGAIN_FAIL				236		//������������
#define ECODE_SRVC_VC_MONEY_LARGE_ERROR				237		//��ֵ������ϵͳ��֧��
#define ECODE_SRVC_BALANCE_DATE_FAIL				238		//����˱�����
#define ECODE_SRVC_BILLBACK_REPEAT_FAIL				239		//�Ѿ����� �����ظ�����
#define ECODE_SRVC_SEQ_REPEAT_FAIL					240		//��ˮ���ظ�

#define ECODE_PARAM_DATE_FORMAT_ERROR				246		//ʱ���ʽ����ȷ
#define ECODE_PARAM_DATE_MIN_ERROR					247		//С����Сʱ��
#define ECODE_PARAM_DATE_MAX_ERROR					248		//�������ʱ��
#define ECODE_PARAM_STRING_ERROR					249		//���ַǷ��ַ�
#define ECODE_PARAM_IP_FROMAT_ERROR					250		//IP��ַ��ʽ����ȷ
#define ECODE_PARAM_ACCNBR_FROMAT_ERROR				251		//�����ʽ������Ҫ��
#define ECODE_PARAM_PROVINCE_FROMAT_ERROR			252		//ʡ�ݱ��벻��ȷ
#define ECODE_PARAM_LOCAL_FROMAT_ERROR				253		//���������벻��ȷ
#define ECODE_PARAM_POST_FROMAT_ERROR				254		//����������Ҫ��
#define ECODE_PARAM_NUMBER_FROMAT_ERROR				255		//�������ֶγ����ַ�
#define ECODE_PARAM_CHARS_ERROR						256		//�ַ����ֶγ��ַǷ��ַ�
#define ECODE_ARCHIVING_PROCESS_ERROR				261		//���ݹ鵵ʧ��
#define ECODE_OPERATE_ERROR							262		//����ʧ��



/********************************************************
*                   ��̬����������
*					��301--500��
*********************************************************/

#define ECODE_NP_NOT_EXIST							301		//Я��ת�����ݲ�����
#define ECODE_NP_EXP								302		//Я��ת��������ʧЧ
#define ECODE_NP_INVALID							303		//Я��ת������δ��Ч
#define ECODE_NP_SERV_REMOVED						304		//�û��Ѳ��

#define ECODE_IBRST_CARRIER_NOT_EXIST				311		//C������������Ӫ�̲�����
#define ECODE_IBRST_CARRIER_EXP						312		//C������������Ӫ����ʧЧ
#define ECODE_IBRST_CARRIER_VALID					313		//C������������Ӫ����δ��Ч
#define ECODE_IBRST_COUNTRY_NOT_EXIST				314		//C������������Ӫ�����ڹ��Ҳ�����
#define ECODE_IBRST_REGION_NOT_EXIST				315		//C������������Ӫ�������ʷ����򲻴���

#define ECODE_CTOG_CARRIER_NOT_EXIST				321		//��ͬ��ʽ����������Ӫ�̲�����
#define ECODE_CTOG_CARRIER_EXP						322		//��ͬ��ʽ����������Ӫ����ʧЧ
#define ECODE_CTOG_CARRIER_INVALID					323		//��ͬ��ʽ����������Ӫ����δ��Ч
#define ECODE_CTOG_COUNTRY_NOT_EXIST				324		//��ͬ��ʽ����������Ӫ�����ڹ��Ҳ�����
#define ECODE_CTOG_NOT_EXIST						325		//��ͬ��ʽ����CtoG������Ӫ�̲�����
#define ECODE_CTOG_REGION_NOT_EXIST					326		//��ͬ��ʽ����������Ӫ���������ʷ����򲻴���


#define ECODE_MIN_NOT_EXIST							331		//MIN�벻����
#define ECODE_MIN_EXP								332		//MIN����ʧЧ
#define ECODE_MIN_INVALID							333		//MIN����δ��Ч
#define ECODE_MIN_COUNTRY_NOT_EXIST					334		//MIN��������Ҵ����쳣�������ڣ�
#define ECODE_MIN_COUNTRY_EXP						335		//MINMIN��������Ҵ����쳣��ʧЧ��
#define ECODE_MIN_COUNTRY_MUL						336		//MINMIN��������Ҵ����쳣�������
#define ECODE_MIN_CARRIER_NOT_EXIT					337		//MINMIN�������Ӫ�̴����쳣�������ڣ�
#define ECODE_MIN_CARRIER_EXP						338		//MIN�������Ӫ�̴����쳣��ʧЧ��
#define ECODE_MIN_CARRIER_MUL						339		//MIN�������Ӫ�̴����쳣�������

#define ECODE_IMSI_NOT_EXIST						346		//IMSI�벻����
#define ECODE_IMSI_EXP								347		//IMSI����ʧЧ
#define ECODE_IMSI_INVALID							348		//IMSI����δ��Ч
#define ECODE_IMSI_CTOG_ERROR						349		//IMSI��ӦCtoG������Ӫ�̴����쳣
#define ECODE_IMSI_LOCAL_MUL						350		//IMSI��Ӧ���������

#define ECODE_COUNTRY_NOT_EXIST						356		//������Ϣ������
#define ECODE_COUNTRY_EXP							357		//������Ϣ��ʧЧ
#define ECODE_COUNTRY_INVALID						358		//������Ϣ��δ��Ч
#define ECODE_COUNTRY_MUL							359		//���Ҵ����ظ�
#define ECODE_COUNTRY_ERROR							360		//���������쳣

#define ECODE_HCODE_NOT_EXIST						366		//H�벻����
#define ECODE_HCODE_EXP								367		//H����ʧЧ
#define ECODE_HCODE_INVALID							368		//H����δ��Ч
#define ECODE_HCODE_LOCAL_MUL						369		//H������������쳣�������
#define ECODE_HCODE_CARRIER_MUL						370		//H�������Ӫ���쳣�������
#define ECODE_HCODE_CARRIER_NOT_EXIT				371		//H�������Ӫ���쳣�������ڣ�

#define ECODE_SP_NOT_EXIST							376		//SP���벻����
#define ECODE_SP_EXP								377		//SP������ʧЧ
#define ECODE_SP_INVALID							378		//SP������δ��Ч

#define ECODE_BORDER_ROAM_PROVINCE_MUL				386		//���������������ʡ



/********************************************************
*                   HSS�߼��жϴ�����
*					��12000--12999��
*********************************************************/

#define ECODE_HSS_UPDATE_NOT_EXIST					12000	//���¼�¼������
#define ECODE_HSS_LAST_CONTINUE_NOT_EXIT			12001	//�Ҳ�����һ��������Ϣ��
#define ECODE_HSS_AUTH_PAYPWD_FAIL					12002	 //֧�������Ȩʧ��
#define ECODE_HSS_CUSTID_ERROR						12003	//��ͻ���ʶ������Ҫ��
#define ECODE_HSS_PRODUCTID_ERROR					12004	//��Ʒ���벻����Ҫ��

#define ECODE_HSS_SERV_CUST_ERROR					12100	//���û�����û�пͻ�����
#define ECODE_HSS_SERV_ACCT_ERROR					12101	//���û�����û���ʻ�����
#define ECODE_HSS_ACCT_CUST_ERROR					12102	//���ʻ�����û�пͻ�����
#define ECODE_HSS_REFCUST_NOT_EXIT					12103	//�����Ĵ�ͻ�������
#define ECODE_HSS_REFSERV_NOT_EXIT					12104	//�������û���Ϣ������
#define ECODE_HSS_REFACCT_NOT_EXIT					12105	//�������˻���Ϣ������
#define ECODE_HSS_BANK_NOT_EXIST					12106	//���������������ʺ�
#define ECODE_HSS_SERV_MAX_ERROR					12107	//��������û���

#define ECODE_HSS_OFFERINS_DATE_NOT_CONST_ERROR		12200	//����Ʒʵ��������Ʒ��������ʵ����ʧЧʱ�䲻һ��
#define ECODE_HSS_OFFER_EXP_MIN_EFF_ERROR			12201	//����ƷʧЧ����С����Ч����
#define ECODE_HSS_OFFER_MEMBER_NUM_ERROR			12202	//����Ʒ���������Ա�����쳣����Ʒȱʧ��ࣩ
#define ECODE_HSS_OFFER_MEMBER_TYPE_ERROR			12203	//����Ʒ���������Ա�����쳣
#define ECODE_HSS_OFFERINS_ATTR_LOST_ERROR			12204	//ȱ�ٱ��������Ʒ����ʵ��
#define ECODE_HSS_OFFERINS_PARAM_LOST_ERROR			12205	//ȱ�ٱ��������Ʒ����ʵ��
#define ECODE_HSS_OFFERINS_NUM_MAX_ERROR			12206	//�����������Ʒʵ����
#define ECODE_HSS_OFFERINS_NUM_MAX_ERROR			12250	//��������Ⱥ��Ա����������Ʒʵ����������
#define ECODE_HSS_OFFERINS_NUM_MAX_ERROR			12260	//����ʧ��
#define ECODE_HSS_OFFER_CATALOG_APPROVE_FAIL		12270	//����ƷĿ¼����ʧ��

/********************************************************
*                   HSS�Զ��������
*					��12900--12999��
*********************************************************/

#define ECODE_HSS_SUCCESS							0		//�ɹ� 
#define ECODE_HSS_TIMEOUT							12900	//ϵͳ��ʱ
#define ECODE_HSS_FAILURE							12901	//ϵͳ����
#define ECODE_HSS_RDCFG_FAIL						12902	//�ͻ��˶�ȡ�����ļ�����
#define ECODE_HSS_PARAM_INVALID						12903	//�ͻ��˲���ʧЧ
#define ECODE_HSS_MQ_OP_FAIL						12904	//�ͻ�����Ϣ���в���ʧ��
#define ECODE_HSS_MQ_NOMSG							12905	//�ͻ�����Ϣ����Ϊ��
#define ECODE_HSS_FTP_SEND_ERR						12906	//�ͻ���FTP�����쳣
#define ECODE_HSS_FTP_RECV_ERR						12907	//�ͻ���FTP�����쳣
#define ECODE_HSS_DISPATCH_FAIL						12908	//�ͻ��˷ַ��쳣
#define ECODE_HSS_MEM_DB_ERROR						12909	//�ڴ����ݿ����

// ���������� ���ڿͻ���ѯ��HSS������Ƿ�ִ�й�ĳ������, �����m_iRltCode=ECODE_HSS_CMD_REPEAT

#define ECODE_HSS_CMD_REPEAT						12910	//��ʾ������ִ�й�

#endif
/*****************************************************
 *	 ����ABM��������������������
 *                  ����������֧������������
 *
 *	 (13800~13899)
 *
 */
#define DEDUCT_UNPACK_NOMSG						13800	//��������-��Ϣ��Ϊ��
#define DEDUCT_UNPACK_ERR						13801	//��������-�����
#define DEDUCT_SERV_NOT_EXIST						13802	//�û���Ϣ������
#define DEDUCT_SERV_QRY_ERR						13803	//�û���ѯʧ��
#define TRANSFER_RULUES_NO_EXIT						13804	//�������򲻴���
#define TRANSFER_RULUES_QRY_ERR						13805	//���������ѯʧ��
#define OVER_DAY_TRANS_FREQ						13806	//�����ѷ�������>ÿ�컮��Ƶ��
#define OVER_DAY_TRANS_LIMIT						13807	//�������컮���޶�
#define OVER_MONTH_TRANS_LIMIT						13808	//�����»����޶�
#define BALANCE_NOT_DEDUCT						13809	//����ʱ����㻮��
#define DEDUCT_TT_ERR							13810	//�������ݿ����
#define DEDUCT_PACK_ERR							13811	//�������ʧ��
#define UNPACK_PLATFORM_CCR_NOMSG					13812	//���Է���ƽ̨����CCR-��Ϣ��Ϊ��
#define UNPACK_PLATFORM_CCR_ERR						13813	//���Է���ƽ̨����CCR-�����
#define ACTIVE_DEDUCT_TT_ERR						13814	//��������ActiveAllocateBalance,���ݿ����
#define CENTER_ABM_DEDUCT_IN_ERR					13815	//����ABM������ֵʧ��
#define CENTER_ABM_DEDUCT_OUT_ERR					13816	//����ABM������ֵʧ��
#define SAVE_PLATFOR_CCR_ERR						13817	//�����Է���ƽ̨����CCR��Ϣͷʧ��
#define SAVE_PLATFOR_CCR_BIZ_ERR					13818	//�����Է���ƽ̨����CCRҵ����Ϣʧ��
#define QRY_SESSION_ID_ERR						13819	//��ѯDCC�Ự��ʶSession-Idʧ��
#define QRY_SERV_ORGID_ERR						13820	// ��ѯ�û�������������ʧ��
#define ACTIVE_PACK_CCR_ERR						13821	// ��ʡABM������������CCR-���ʧ��
#define ACTIVE_PACK_CCR_OUT_ERR						13822	// ������ABM������������CCR-���ʧ��

#define ACTIVE_DEDUCT_BIZ_TT_ERR					13823	//���������շ�CCA�����ݿ����
#define DEDUCT_BIZ1_IN_ERR						13824	//������ֵʧ��
#define DEDUCT_BIZ2_IN_ERR						13825	//��������ʧ��

#define UNPACK_CCA1_ERR							13826	//����������CCA���ʧ�ܣ�ʡABM��������CCA
#define UNPACK_CCA2_ERR							13827	//����������CCA���ʧ�ܣ�����ABM��������CCA
#define UNPACK_REVERSE_DEDUCT_NOMSG					13828	//��������-����CCR�����Ϣ��Ϊ��
#define UNPACK_REVERSE_DEDUCT_ERR					13829	//��������-����CCR���ʧ��
#define REVERSE_DEDUCT_TT_ERR						13830	//��������,���ݿ����
#define REVERSE_DEDUCT_BIZ_ERR						13831	//��������,ҵ����ʧ��


#define SAVE_RECEIVE_DCC_CCA_ERR					13832	//������յ�CCA��Ϣͷʧ��
#define SAVE_SEND_DCC_CCA_ERR						13833	//���淢����CCA��Ϣͷʧ��

#define SAVE_RECEVIE_DCC_CCR_ERR					13834	//������յ�CCR��Ϣͷʧ��
#define REVERSE_DEDUCT_PACK_ERR						13835	//��������-����CCA���ʧ��
#define GET_DCC_INFO_RECORD_SEQ_ERR					13836	//��ȡDCC_INFO_RECORD_SEQʧ��

#define RETURN_CCA_UNPACK_NOMSG						13837	//����ʡABM���ص�CCA����ϢΪ��
#define RETURN_CCA_UNPACK_ERR						13838	//����ʡABM���ص�CCA�������
#define DEDUCT_IN_ERR							13839	//������ֵʧ��
#define GET_SESSION_ID_SEQ_ERR						13840	//ȡSESSION_ID ����ʧ��
#define GET_ACCT_NBR_ORGID_ERR						13841	//ȡ����������������ʧ��
#define DEDUCT_IN_PACK_ERR						13842	//������ֵ���ʧ��
#define GET_DCC_INFO_RECORD_SEQ_ERR					13843	//ȡDCC_INFO_RECORD_SEQʧ��
#define DEDUCT_OUT_ERR							13844	//��������ҵ����ʧ��
#define QRY_PRE_SESSION_ID_ERR						13845	//��ѯԴ�Ự��ʶʧ��
#define RETURN_BIZ_CODE_ERR						13846	//ʡABM����CCA-ҵ�񼶽�������0��
#define DEDUCT_AMOUNT_ERR						13847	//�������С�ڵ���0��


/*****************************************************
 *	 ����abm��ֵ����ֵ������֧��������
 *
 *        ��13900~13999��
 *
 * **************************************************/
#define DEPOSIT_UNPACK_ERR						13900	//��ֵ�����
#define DEPOSIT_PACK_ERR						13901	//��ֵ�����
#define DEPOSIT_TT_ERR							13902	//��ֵ���ݿ��
#define PAYMENT_UNPACK_ERR						13903	//֧�������
#define PAYMENT_PACK_ERR						13904	//֧�������
#define PAYMENT_TT_ERR							13905	//֧�����ݿ��
#define REVERSE_PAYMENT_TT_ERR						13906	//֧�������
#define BALANCE_PAYOUT_TT_ERR						13907	//�������ݿ��
#define REVER_BALANCE_PAYOUT_TT_ERR					13908	//�����������ݿ��
#define BALANCE_NOT_PAYMENT_ERR						13909	//�˱�������֧��
#define REVERSE_DEP_UNPACK_ERR						13910	//��ֵ���������
#define REVERSE_DEP_PACK_ERR						13911	//��ֵ���������
#define REVERSE_DEP_TT_ERR						13912	//��ֵ�������ݿ��  
#define REVERSE_DEP_QRY_OPTID_ERR					13913	//��ֵ������ѯԭҵ���¼ʧ��
#define REVERSE_DEP_QRY_DATA_ERR					13914	//��ֵ������ѯԭҵ���¼����ʧ��
#define REVERSE_DEP_BALANCE_CHANGE_ERR					13915	//��ֵ���������Դ����ѷ����仯
#define COMPARE_PASSWORD_ERR						13916	//֧�������
#define QUERY_SEQ_ERR							13917	//��ѯ���д�
#define INSERT_PAYMENT_ERR						13918	//��¼ҵ��������
#define QRY_ACCT_BALANCE_ACC_NBR_ERR					13919	//�������ѯ����˱���
#define QRY_ACCT_BALANCE_ID_ERR						13920	//���˱����в��
#define INSERT_BALANCE_PAYOUT_ERR					13921	//��¼���֧�����
#define INSERT_BALANCE_SOURCE_PAYOUT_ERR				13922	//��¼��Դ֧����ϵ���
#define UPDATE_ACCT_BALANCE_ERR						13923	//��������˱����
#define UPDATE_BALANCE_SOURCE_ERR					13924	//���������Դ��
#define QRY_BALANCE_PAYOUT_ERR						13925	//��ѯ���֧�����ݴ�
#define QRY_BALANCE_SOURCE_ERR						13926	//��ѯ�����Դ����
#define UPDATE_BALANCE_PAYOUT_ERR					13927	//�������֧����
#define UPDATE_BALANCE_SOURCE_PAYOUT_ERR				13928	//������Դ֧����ϵ��
#define QRY_SERV_ACCT_SEQ_ERR						13929	//��ѯ�û���Ϣ���д�
#define INSERT_SERV_ERR							13930	//��¼�û���Ϣ���
#define INSERT_ACCT_ERR							13931	//��¼�˻���Ϣ����
#define INSERT_SERV_ACCT_ERR						13932	//��¼�û��˻���Ӧ��ϵ��
#define INSERT_ACCT_BALANCE_ERR						13933	//��¼����˱���
#define INSERT_BALANCE_SOURCE_ERR					13934	//��¼�����Դ��
#define QRY_ACCT_BALANCE_SIMPLE_ERR					13935	//��ѯ����˱���Ϣ��
#define INSERT_USER_INFO_ADD_ERR					13936	//��¼�û���Ϣ������
#define INSERT_ATT_USER_INFO_ERR					13937	//��¼�û�֧�������
#define INSERT_APP_USER_INFO_ERR					13938	//��¼�û��������Ŵ�
#define UPDATE_PAYMENT_ERR						13939	//����ҵ�������
#define QRY_PAYMENT_ERR							13940	//��ѯҵ�������


/*****************************************************
 *	 ����abm����ͬ��������
 *
 *        ��14000~14007��
 *
 * **************************************************/
 #define INFOSYN_TT_ERR			14000	//ͬ���������ݿ��
 #define QRY_USER_INFO_ADD_ERR			14001	//��ѯ�û���Ϣ�������
 #define QRY_SESSION_ID_ERR			14002	//��ѯ��������ˮ�Ŵ�
 #define UPDATE_USER_INFO_ADD_SYNSTATE_ERR			14003	//�����û���Ϣ���������״̬�ֶδ�
 #define INSERT_SMS_SEND_ERR			14004	//��¼���Ź������
 #define INFO_SYN_UNPACK_ERR			14005	//����ͬ�������
 #define INFO_SYN_PACK_ERR				14006	//����ͬ�������
 #define INFO_SYN_ERROR_ID_ERR		14007	//�û���Ϣ�������ؽ��ID�������¼��
 
 
 /*****************************************************
 *	 ����abm�Է����ܷ��ش�����
 *
 *        ��14000~14007��
 *
 * **************************************************/
 #define 	PASSWD_AUTHEN_ERR					15001	// ֧�������Ȩʧ��
 #define 	PASSWD_FLAG_TYPE_ERR				15002	// ����������ʹ���
 #define 	PAY_PASSWD_UPDATE_ERR				15003	// ֧���������ʧ��
 #define 	PAY_PASSWD_DELETE_ERR				15004	// ֧������ɾ��ʧ��
 #define 	PAY_PASSWD_INSERT_ERR				15005	// ��������������
 #define 	PAY_PASSWD_SELECT_ERR				15006	// ���������ѯ����
 #define 	PAY_PASSWD_AUTHEN_ERR				15007	// ���������Ȩ����
 #define 	PAY_RULE_NULL						15008	// �޻�������
 #define 	PAY_RULE_ERR						15009	// ���������ѯʧ��
 #define 	TRANSFERRULES_INSERT_ERR			15010	// ����֧���������ʧ��