#ifndef __MBC_SGW_H__
#define __MBC_SGW_H__




//#
//# ����������� 2λĿ¼���+3λģ����+4λ��Ϣ���
//#AVP�������Ϊ 2λĿ¼���+7λ��Ϣ���
#define MBC_MEM_MALLOC_FAIL						1
//@@	1	�ڴ����ʧ��		�����Ƿ����㹻���ڴ�
#define MBC_MEM_SPACE_SMALL						2
//@@	2	�ڴ�ռ䲻��		�����Ƿ����㹻���ڴ�
#define MBC_MEM_INTERFACE_ERROR					3
//@@	3	�ڴ����ӿڴ���	�����Ƿ����㹻���ڴ�
#define MBC_MEM_LINK_FAIL						4
//@@	4	�����ڴ�����ʧ��	�����Ƿ����㹻���ڴ�
#define MBC_THREAD_CREAT_FAIL					5
//@@	5	�������̻��߳�ʧ��	�����Ƿ����㹻���ڴ�
#define MBC_SEMP_OPERA_FAIL						6
//@@	6	�źŴ���ʧ��		�����Ƿ����㹻���ڴ�
#define MBC_LOCK_GET_FAIL							7
//@@	7	��ȡ����Դʧ��		�����Ƿ����㹻���ڴ�
#define MBC_SOCKET_LISTEN_FAIL					8
//@@	8	Socket--��������	����Socket�������Ƿ�����
#define MBC_SOCKET_LINK_FAIL						9
//@@	9	����Զ��Socket��	����˿��Ƿ�ռ��
#define MBC_DISC_SPACE_SMALL						10
//@@	10	���̴洢�ռ䲻��	���������̿ռ䣬������Ƿ��г����ļ�ռ�ô������̿ռ�
#define MBC_CPU_OVERLOAD							11
//@@	11	CPU������			�����Ƿ�ϵͳ��Դ�Ƿ񱻽���ռ��
#define MBC_DB_LINK_FAIL							12
//@@	12	�������ݿ�ʧ��		�����û��������Ƿ���ȷ
#define MBC_DB_TABLESPACE_SMALL					13
//@@	13	���ռ䲻��			�����Ƿ����㹻�ı��ռ�
#define MBC_ENV_NOT_SET							14
//@@	14	��������δ����		�����û�������
#define MBC_SYS_BUSY								15
//@@	15	ϵͳ��æ	      ����������Դ
#define MBC_MAX_DCC_REQUEST_CNT							16
//@@	16	DCC��Ϣ����ﵽ���ֵ			�����Ƿ�ϵͳ���������Ƿ�����
#define MBC_SHM_ERRNO_DATA_NOT_EXIST			50
//@@	50	�����ڴ�������������	���鹲���ڴ��������Ƿ񴴽�
#define MBC_SHM_ERRNO_INDEX_NOT_EXIST			51
//@@	51	�����ڴ�������������	���鹲���ڴ������Ƿ񴴽�
#define MBC_SHM_ERRNO_DATA_NOTCONNECT			52
//@@	52	δ���ӹ����ڴ�������	���������ӹ����ڴ�������
#define MBC_SHM_ERRNO_INDEX_NOTCONNECT			53
//@@	53	δ���ӹ����ڴ�������	���������ӹ����ڴ�������
#define MBC_SHM_ERRNO_ADD_DATA_EXIST			54
//@@	54	�����ڴ��������Ѿ�����	��
#define MBC_SHM_ERRNO_UPDATEINFO_NOTEXIST		55
//@@	55	�����ڴ���µ����ݲ�����		����ò�������
#define MBC_SHM_ERRNO_DEL_DATA_NOTEXIST			56
//@@	56	ɾ�������ݹ����ڴ治����	��
#define MBC_SHM_ERRNO_DEL_INDEX_FAILED			57
//@@	57	ɾ�����ݵ�����ʧ��	��
#define MBC_SHM_ADD_DATA_EXIST					58
//@@	58	���ӵ������Ѿ�����	��
#define MBC_SHM_QUERY_NO_DATE_FOUND				59
//@@	59	��ѯ���ݲ�����	��
#define MBC_SHM_ULOAD_DATA_NOT_EXIST			60
//@@	60	ж�ص�������������	��
#define MBC_SHM_LOAD_NO_DATA					61
//@@	61	���صı����ݲ�����	��
#define MBC_SHM_OPT_NOT_SUPPORT					62
//@@	62	�����ڴ治֧�ִ��ֲ���	��
#define MBC_SHM_NOT_CONNECT_DATA				63
//@@	63	δ����������	�������ӹ����ڴ�
#define MBC_CFG_GETPARAM_FAILED					64
//@@	64	��ȡ���Ĳ���ʧ��	�鿴�����ļ��Ƿ�����
#define MBC_SHM_INIT_FAILED						65   
//@@	65	��ʼ�������ڴ�ʧ��	�鿴������־��Ϣ

#define MBC_FTP_CREAT_FAIL                    30
#define MBC_FTP_SEND_FAIL                     31
#define MBC_FTP_RECV_FAIL                     32


#define MBC_FILE_OPEN_FAIL						101
//@@	101	���ļ�ʧ��		�����ļ��Ƿ����
#define MBC_FILE_WRITE_FAIL						102
//@@	102	д�ļ�ʧ��			�����ļ��Ƿ����
#define MBC_DIR_OPEN_FAIL							103
//@@	103	��Ŀ¼����		����Ŀ¼�Ƿ���ڣ�·���Ƿ���ȷ
#define MBC_FILE_RENAME_FAIL						104
//@@	104	�ļ���������		�����ļ��Ƿ���ڣ��Ƿ�ռ��
#define MBC_FILE_SEQ_GET_FAIL						105
//@@	105	���л�ȡʧ��		���������Ƿ���ڣ��Ƿ�ռ��
#define MBC_FILE_NOT_EXIST						106
//@@	106	ָ���ļ�������		�����ļ���������Ŀ¼�Ƿ���ȷ
#define MBC_FILE_COPY_FAIL						107
//@@	107	�����ļ�ʧ��		�����ļ���������Ŀ¼�Ƿ���ȷ
#define MBC_FILE_MOVE_FAIL						108
//@@	108	�ƶ��ļ�ʧ��		�����ļ���������Ŀ¼�Ƿ���ȷ
#define MBC_FILE_LOG_FAIL						109
//@@	109	��־���ļ���ʧ		�����ļ���������Ŀ¼�Ƿ���ȷ

#define MBC_MEM_UPDATE_FAIL						116
//@@	116	�����ڴ����ʧ��	�����Ƿ����㹻���ڴ�
#define MBC_MEM_IMPORT_FAIL						117
//@@	117	�����쳣			�����Ƿ����㹻���ڴ�
#define MBC_MEM_EXPORT_FAIL						118
//@@	118	�ڴ����ݵ����쳣	�����Ƿ����㹻�Ŀռ�
#define MBC_MEM_CREATE_FAIL						119
//@@	119	���������ڴ�ʧ��	�����Ƿ����㹻���ڴ�
#define MBC_MEM_DELETE_FAIL						120
//@@	120	ɾ�������ڴ�ʧ��	���鹲���ڴ��ַ�Ƿ���ȷ
#define MBC_MEM_DESTROY_FAIL						121
//@@	121	���ٹ����ڴ�ʧ��	���鹲���ڴ��Ƿ�����ʹ��
#define MBC_SEMP_CREATE_FAIL						122
//@@	122	�����ź���ʧ��		�����ź����Ƿ��ѱ�ʹ��
#define MBC_PV_ERRNO_P_FAILED						123
//@@	123	�ź�������ʧ��		�����ź����Ƿ��ѱ�ʱ��

#define MBC_DB_SERV_PWD_ERROR						141
//@@	141	�û����������		�����û���������
#define MBC_DB_LINK_CLOSE_FAIL					142
//@@	142	�ر����ݿ�����ʧ��	�������ݿ������Ƿ�����
#define MBC_DB_TABLE_NOT_EXIT						143
//@@	143	����������			���������
#define MBC_DB_ROLLBACK_FAIL						144
//@@	144	���ݿ�����ع�����	�������ݿ����������
#define MBC_DB_EXECUTE_FAIL						145
//@@	145	����sqlִ���쳣		����SQL����Ƿ���ȷ

#define MBC_DB_OP_FAIL          146
//@@	146	���ݿ����ʧ��	����SQL����Ƿ���ȷ

#define MBC_NET_PROTOCOL_PACKAGE_FAIL				156
//@@	156	Э���װʧ��					�����װЭ��
#define MBC_NET_PROTOCOL_PARSE_FAIL				157
//@@	157	Э�����ʧ��	        �������Э��


#define MBC_PARAM_LOAD_ERROR				165
//@@	165	��������ʧ��					�������ļ�
#define MBC_PARAM_FILE_FORMAT_ERROR				166
//@@	166	�����ļ���ʽ����					��������ļ���ʽ
#define MBC_PARAM_THREAD_LOST_ERROR				167
//@@	167	����������	�ں��Ĳ������������˲���
#define MBC_PARAM_THREAD_NONSUPP_ERROR			168
//@@	168	Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�	����ѡ�����
#define MBC_PARAM_THREAD_VALUE_ERROR				169
//@@	169	Ӧ�ý��̲�������ȷ�������ȡֵ��	���޸Ĳ���ֵ
#define MBC_PARAM_UPDATE_FAIL						170
//@@	170	��������ʧ��	            �������ļ�״̬����������ʽ��
#define MBC_PARAM_FILE_ERROR						171
//@@	171	�����ļ�������						�������ļ�·��
#define MBC_PARAM_LOST_ERROR						172
//@@	172	����������Ϣȱʧ					������������Ϣ������
#define MBC_PARAM_VALUE_NULL_ERROR				173
//@@	173	�ǿռ�飨�ֶβ���Ϊ��)	  ����ֶΣ��Ƿ���ڷǿ��ֶ�Ϊ��ֵ
#define MBC_PARAM_LEN_MIN_ERROR					174
//@@	174	���ȼ�飨С����С����	  ����ֶΣ��Ƿ�����ֶγ���С���趨����С����
#define MBC_PARAM_LEN_MAX_ERROR					175
//@@	175	���ȼ�飨������󳤶�)	  ����ֶΣ��Ƿ�����ֶγ��ȴ����趨����󳤶�
#define MBC_PARAM_LEN_FIXED_ERROR					176
//@@	176	���ȼ�飨�ǹ̶�����)	    ������������Ϣ������
#define MBC_PARAM_VALUE_ENUM_ERROR				177
//@@	177	ö�ټ��(ö��ֵ����ȷ)	  ������ֵ�Ƿ��ڲ�����ö�ٷ�Χ��
#define MBC_PARAM_VALUE_MIN_ERROR					178
//@@	178	��ֵ��Χ��飨С����Сֵ)	��������ֵ��Χ�Ƿ�С���趨����Сֵ
#define MBC_PARAM_VALUE_MAX_ERROR					179
//@@	179	��ֵ��Χ��飨�������ֵ)	��������ֵ��Χ�Ƿ�����趨�����ֵ
#define MBC_PARAM_ERROR							180
//@@	180	��������	                ��鲻���Ѷ���Ĳ�����ʽ�����У������ڲ�����Ĵ���

#define MBC_INFO_CUST_GET_FAIL                  181
//@@	181	ȡ�ͻ���Ϣʧ��	          ���ͻ���Ϣ�Ƿ����
#define MBC_INFO_ACCT_GET_FAIL                  182
//@@	182	ȡ�˻���Ϣʧ��	          ����˻���Ϣ�Ƿ����
#define MBC_INFO_SERV_GET_FAIL                  183
//@@	183	ȡ�û���Ϣʧ��	          ����û���Ϣ�Ƿ����

#define MBC_SYSTEM_START_ERROR         195
//@@	195	ϵͳ����ʧ��					���ϵͳ״̬
#define MBC_THREAD_EXIST_ERROR						196
//@@	196	Ӧ�ý����Ѿ�����					����Ӧ�ý����Ƿ��Ѿ�����
#define MBC_THREAD_ERROR							197
//@@	197	�����쳣						����������
#define MBC_THREAD_STATUS_ERROR					198
//@@	198	����״̬�Ƿ�						��������Ƿ���������
#define MBC_THREAD_EXIT_ERROR						199
//@@	199	Ӧ�ó����յ��˳��ź��쳣�˳�		����Ӧ�ý���״̬�����ϵͳ״̬

#define MBC_THREAD_EXIT_NORMAL                  //�����յ���ȫ�˳��ź������˳�

#define MBC_CHECK_POINT_ERROR     200
//@@	200	CHECKPOINT�쳣�˳�		�����ڴ����ݻ�����ļ�

#define MBC_MAX_SYSTEM_PROCESS     201
//@@	201	�ﵽϵͳ����������		�������Ĳ���������ϵͳ������������

#define MBC_SRVC_SEQ_CONTINUE_FAIL				213
//@@	213	���������У��ʧ��				�����ŵ�������
#define MBC_SRVC_RECEIVE_FAIL						232
//@@	232	ϵͳ�����쳣����					���ϵͳ״̬�����ϵͳ���ݴ���
#define MBC_SRVC_SEQ_REPEAT_FAIL					240
//@@	240	��ˮ���ظ�				        ����Ƿ�����ظ���ˮ��


#define MBC_PARAM_DATE_FORMAT_ERROR				246
//@@	246	ʱ���飨ʱ���ʽ����ȷ��									���ʱ���ʽ�Ƿ���ȷ
#define MBC_PARAM_DATE_MIN_ERROR					247
//@@	247	ʱ���飨С����Сʱ�䣩									���ʱ���Ƿ�С����Сʱ��
#define MBC_PARAM_DATE_MAX_ERROR					248
//@@	248	ʱ���飨�������ʱ�䣩										���ʱ���Ƿ�������ʱ��
#define MBC_PARAM_STRING_ERROR					249
//@@	249	�ַ�����飨���ַǷ��ַ���								����ַ������Ƿ���ַǷ��ַ�
#define MBC_PARAM_IP_FORMAT_ERROR					250
//@@	250	IP��ַ��飨IP��ַ��ʽ����ȷ��							���IP��ַ�ĸ�ʽ
#define MBC_PARAM_ACCNBR_FORMAT_ERROR				251
//@@	251	�����ʽ������Ҫ��	                        �������ʽ�Ƿ���ȷ
#define MBC_PARAM_PROVINCE_FORMAT_ERROR				252
//@@	252	ʡ�ݱ����飨ʡ�ݱ��벻��ȷ��	            ���ʡ�ݱ����Ƿ���ȷ
#define MBC_PARAM_LOCAL_FORMAT_ERROR				253
//@@	253	�����������飨���������벻��ȷ��					��鱾���������Ƿ���ȷ
#define MBC_PARAM_POST_FORMAT_ERROR				254
//@@	254	���������飨����������Ҫ��							������������Ƿ����Ҫ��
#define MBC_PARAM_NUMBER_FORMAT_ERROR				255
//@@	255	�������ͼ�飨�������ֶγ����ַ���					����������ֶ��Ƿ�����ַ�
#define MBC_PARAM_CHARS_ERROR						256
//@@	256	�������ͼ�飨�ַ����ֶγ��ַǷ��ַ���			����ַ����ֶ��Ƿ���ַǷ��ַ�


#define MBC_ARCHIVING_PROCESS_ERROR				261
//@@	261	���ݹ鵵ʧ��	      ������ݹ鵵���ã�ִ��Ȩ�ޣ����̿ռ�
#define MBC_OPERATE_ERROR							262
//@@	262	����ʧ��	          ����޷��������д���Ĳ���ʧ��


#define MBC_NP_NOT_EXIST							301
//@@	301	Я��ת�����ݲ�����	                               ����Ƿ����Я��ת������
#define MBC_NP_EXP								302
//@@	302	Я��ת��������ʧЧ	                               ���Я��ת������״̬���Ƿ���ʧЧ
#define MBC_NP_INVALID							303
//@@	303	Я��ת������δ��Ч	                               ���Я��ת������״̬���Ƿ�δ��Ч
#define MBC_NP_SERV_REMOVED						304
//@@	304	�û��Ѳ��        	                               ���Я��ת���û�״̬���Ƿ��Ѳ��


#define MBC_IBRST_CARRIER_NOT_EXIST				311
//@@	311	C������������Ӫ�̲�����            	               ���C���������α�����Ӫ���Ƿ����
#define MBC_IBRST_CARRIER_EXP						312
//@@	312	C������������Ӫ����ʧЧ            	               ���C���������α�����Ӫ������״̬���Ƿ���ʧЧ
#define MBC_IBRST_CARRIER_VALID					313
//@@	313	C������������Ӫ����δ��Ч          	               ���C���������α�����Ӫ������״̬���Ƿ�δ��Ч
#define MBC_IBRST_COUNTRY_NOT_EXIST				314
//@@	314	C������������Ӫ�����ڹ��Ҳ�����    	               ���C���������α�����Ӫ�����ڹ����Ƿ񲻴���
#define MBC_IBRST_REGION_NOT_EXIST				315
//@@	315	C������������Ӫ�������ʷ����򲻴���	               ���C���������α�����Ӫ�������ʷ������Ƿ񲻴���



#define MBC_CTOG_CARRIER_NOT_EXIST				321
//@@	321	��ͬ��ʽ����������Ӫ�̲�����            	         ��鲻ͬ��ʽ�������α�����Ӫ���Ƿ����
#define MBC_CTOG_CARRIER_EXP						322
//@@	322	��ͬ��ʽ����������Ӫ����ʧЧ            	         ��鲻ͬ��ʽ�������α�����Ӫ������״̬���Ƿ���ʧЧ
#define MBC_CTOG_CARRIER_INVALID					323
//@@	323	��ͬ��ʽ����������Ӫ����δ��Ч          	         ��鲻ͬ��ʽ�������α�����Ӫ������״̬���Ƿ�δ��Ч
#define MBC_CTOG_COUNTRY_NOT_EXIST				324
//@@	324	��ͬ��ʽ����������Ӫ�����ڹ��Ҳ�����    	         ��鲻ͬ��ʽ�������μ�鱻����Ӫ�����ڹ����Ƿ񲻴���
#define MBC_CTOG_NOT_EXIST						325
//@@	325	��ͬ��ʽ����CtoG������Ӫ�̲�����        	         ��鲻ͬ��ʽ��������CtoG������Ӫ���Ƿ񲻴���
#define MBC_CTOG_REGION_NOT_EXIST					326
//@@	326	��ͬ��ʽ����������Ӫ�������ʷ����򲻴���	         ��鲻ͬ��ʽ�������μ�鱻����Ӫ�������ʷ������Ƿ񲻴���


#define MBC_MIN_NOT_EXIST							331
//@@	331	MIN�벻����                      	                 ���MIN���Ƿ����
#define MBC_MIN_EXP								332
//@@	332	MIN����ʧЧ                      	                 ���MIN���Ƿ���ʧЧ
#define MBC_MIN_INVALID							333
//@@	333	MIN����δ��Ч                    	                 ���MIN���Ƿ�δ��Ч
#define MBC_MIN_COUNTRY_NOT_EXIST					334
//@@	334	MIN��������Ҵ����쳣�������ڣ�  	                 ���MIN��������Ҵ����Ƿ񲻴���
#define MBC_MIN_COUNTRY_EXP						335
//@@	335	MIN��������Ҵ����쳣��ʧЧ��    	                 ���MIN��������Ҵ����Ƿ���ʧЧ
#define MBC_MIN_COUNTRY_MUL						336
//@@	336	MIN��������Ҵ����쳣�������    	                 ����Ƿ���ڶ��MIN��������Ҵ���
#define MBC_MIN_CARRIER_NOT_EXIT					337
//@@	337	MIN�������Ӫ�̴����쳣�������ڣ�	                 ���MIN�������Ӫ�̴����Ƿ񲻴���
#define MBC_MIN_CARRIER_EXP						338
//@@	338	MMIN�������Ӫ���쳣��ʧЧ��     	                 ���MIN�������Ӫ�̴����Ƿ���ʧЧ
#define MBC_MIN_CARRIER_MUL						339
//@@	339	MIN�������Ӫ�̴����쳣�������  	                 ����Ƿ���ڶ��MIN�������Ӫ�̴���



#define	 MBC_HSS_IMSI_CODE	 346
//@@	346	IMSI�벻����                  	                   ���IMSI���Ƿ����
#define MBC_IMSI_EXP								347
//@@	347	IMSI����ʧЧ                  	                   ���IMSI���Ƿ���ʧЧ
#define MBC_IMSI_INVALID							348
//@@	348	IMSI����δ��Ч                	                   ���IMSI���Ƿ�δ��Ч
#define MBC_IMSI_CTOG_ERROR						349
//@@	349	IMSI��ӦCtoG������Ӫ�̴����쳣	                   ���IMSI��CtoG������Ӫ�̴���Ķ�Ӧ��ϵ
#define MBC_IMSI_LOCAL_MUL						350
//@@	350	IMSI��Ӧ���������            	                   ����Ƿ����IMSI��Ӧ��������������



#define	 MBC_COUNTRY_NOT_EXIST	 356
//@@	356	������Ϣ������  	                                 ��������Ϣ�Ƿ����
#define	 MBC_COUNTRY_EXP	 357
//@@	357	������Ϣ��ʧЧ  	                                 ��������Ϣ�Ƿ���ʧЧ
#define MBC_COUNTRY_INVALID						358
//@@	358	������Ϣ��δ��Ч	                                 ��������Ϣ�Ƿ�δ��Ч
#define MBC_COUNTRY_MUL							359
//@@	359	���Ҵ����ظ�    	                                 ����Ƿ�����ظ��Ĺ��Ҵ���
#define MBC_COUNTRY_ERROR							360
//@@	360	���������쳣    	                                 ����Ƿ�����쳣��������


#define MBC_HCODE_NOT_EXIST						366
//@@	366	H�벻����                  	                       ���H���Ƿ����
#define MBC_HCODE_EXP								367
//@@	367	H����ʧЧ                  	                       ���H���Ƿ���ʧЧ
#define MBC_HCODE_INVALID							368
//@@	368	H����δ��Ч                	                       ���H���Ƿ�δ��Ч
#define MBC_HCODE_LOCAL_MUL						369
//@@	369	H������������쳣�������  	                       ����Ƿ����H��������������
#define MBC_HCODE_CARRIER_MUL						370
//@@	370	H�������Ӫ���쳣�������  	                       ����Ƿ����H����������Ӫ��
#define MBC_HCODE_CARRIER_NOT_EXIT				371
//@@	371	H�������Ӫ���쳣�������ڣ�	                       ����Ƿ����H���޹�����Ӫ��



#define MBC_SP_NOT_EXIST							376
//@@	376	SP���벻����  	                                   ���SP�����Ƿ����
#define MBC_SP_EXP								377
//@@	377	SP������ʧЧ  	                                   ���SP�����Ƿ���ʧЧ
#define MBC_SP_INVALID							378
//@@	378	SP������δ��Ч	                                   ���SP�����Ƿ�δ��Ч


#define MBC_BORDER_ROAM_PROVINCE_MUL				386
//@@	386	���������������ʡ	                               ������������������������
#define ERR_RULE_PARAM				          1900
//@@	1900	�����������ô���	                               ��������������

#define MBC_SERVICE_CONTEXT_NOT_EXIST                 11005
//@@	11005	ҵ������������	          ����û���ҵ�������Ƿ����
#define MBC_SERVICE_CONTEXT_NOT_PERMISSION            11006
//@@	11006	ҵ��������Ȩ�޷���	    ����û��Ƿ���Ȩ�޷��ʴ�ҵ������
#define MBC_FLOW_OF_BYTES_WARN            11030
//@@	11030	�����������ֽ��������澯��ֵ	    �����ֽ����Ѵﵽ�澯ֵ�����飡
#define MBC_FLOW_OF_BYTES_DINCON            11031
//@@	11031	�����������ֽ�������������ֵ	    �����ֽ����Ѵﵽ����ֵ�����飡
#define MBC_FLOW_OF_REQUESTED_WARN            11032
//@@	11032	�������������������澯��ֵ	    ���������Ѵﵽ�澯ֵ�����飡
#define MBC_FLOW_OF_REQUESTED_DINCON            11033
//@@	11033	������������������������ֵ	    ���������Ѵﵽ����ֵ�����飡


#define MBC_HSS_UPDATE_NOT_EXIST					12000
//@@	12000	���¼�¼������        	                         �����µļ�¼�Ƿ����
#define MBC_HSS_LAST_CONTINUE_NOT_EXIT			12001
//@@	12001	�Ҳ�����һ��������Ϣ��	                         ��鴫����Ϣ����������
#define MBC_HSS_AUTH_PAYPWD_FAIL					12002
//@@	12002	֧�������Ȩʧ��      	                         ���֧�������Ȩ
#define MBC_HSS_CUSTID_ERROR						12003
//@@	12003	��ͻ���ʶ������Ҫ��  	                         ����ͻ���ʶ�Ƿ���Ϲ淶
#define MBC_HSS_PRODUCTID_ERROR					12004
//@@	12004	��Ʒ���벻����Ҫ��    	                         ����Ʒ�����Ƿ���Ϲ淶



#define MBC_HSS_SERV_CUST_ERROR					12100
//@@	12100	���û�����û�пͻ�����	                         ����û��Ŀͻ������Ƿ����
#define MBC_HSS_SERV_ACCT_ERROR					12101
//@@	12101	���û�����û���ʻ�����	                         ����û����ʻ������Ƿ����
#define MBC_HSS_ACCT_CUST_ERROR					12102
//@@	12102	���ʻ�����û�пͻ�����	                         ����ʻ��Ŀͻ������Ƿ����
#define MBC_HSS_REFCUST_NOT_EXIT					12103
//@@	12103	�����Ĵ�ͻ�������    	                         ��������Ĵ�ͻ������Ƿ����
#define MBC_HSS_REFSERV_NOT_EXIT					12104
//@@	12104	�������û���Ϣ������  	                         ����������û���Ϣ�Ƿ����
#define MBC_HSS_REFACCT_NOT_EXIT					12105
//@@	12105	�������˻���Ϣ������  	                         ����������˻���Ϣ�Ƿ����
#define MBC_HSS_BANK_NOT_EXIST					12106
//@@	12106	���������������ʺ�    	                         ����������յ������ʺ�
#define MBC_HSS_SERV_MAX_ERROR					12107
//@@	12107	��������û���        	                         ����û����Ƿ񳬹��趨������û���



#define MBC_HSS_OFFERINS_DATE_NOT_CONST_ERROR		12200
//@@	12200	����Ʒʵ��������Ʒ��������ʵ����ʧЧʱ�䲻һ��	 �������Ʒʵ��������Ʒ��������ʵ������ʧЧʱ���Ƿ�һ��
#define MBC_HSS_OFFER_EXP_MIN_EFF_ERROR			12201
//@@	12201	����ƷʧЧ����С����Ч����                    	 �������Ʒ���õ�ʧЧ�����Ƿ�С����Ч����
#define MBC_HSS_OFFER_MEMBER_NUM_ERROR			12202
//@@	12202	����Ʒ���������Ա�����쳣����Ʒȱʧ��ࣩ    	 �������Ʒ���������Ա����
#define MBC_HSS_OFFER_MEMBER_TYPE_ERROR			12203
//@@	12203	����Ʒ���������Ա�����쳣                    	 �������Ʒ���������Ա����
#define MBC_HSS_OFFERINS_ATTR_LOST_ERROR			12204
//@@	12204	ȱ�ٱ��������Ʒ����ʵ��                      	 ����Ƿ�ȱ�ٱ��������Ʒ����ʵ��
#define MBC_HSS_OFFERINS_PARAM_LOST_ERROR			12205
//@@	12205	ȱ�ٱ��������Ʒ����ʵ��                      	 ����Ƿ�ȱ�ٱ��������Ʒ����ʵ��
#define MBC_HSS_OFFERINS_NUM_MAX_ERROR			12206
//@@	12206	�����������Ʒʵ����                          	 �������Ʒʵ�����Ƿ񳬹��趨���������Ʒʵ����


#define MBC_HSS_GROUPMEMBER_NODATA_ERROR			12250
//@@	12250	��������Ⱥ��Ա����������Ʒʵ����������	         ����������Ⱥ��Ա�����Ƿ�������Ʒʵ����������


#define MBC_HSS_CUST_CREATE_MAX_ERROR			    12260
//@@	12260	����ʧ��	                                       ����û�����������


#define MBC_HSS_OFFER_CATALOG_APPROVE_FAIL		12270
//@@	12270	����ƷĿ¼����ʧ��	                             �������ƷĿ¼�淶

//Command
#define MBC_COMMAND_SGWPARAM           		11801
//@@	11801	ִ��sgwparam	      ִ��sgwparam
#define MBC_COMMAND_SGWPROCESS         		11802
//@@	11802	ִ��process	      ִ��process
#define MBC_COMMAND_SGWMONITOR         		11803
//@@	11803	ִ��sgwmonitor	      ִ��sgwmonitor

/*    ������    */
#define  MBC_ACTION_SEARCH_PARAM               90000
//@@	90000	���Ҳ���	      �������ҳɹ�
#define  MBC_ACTION_SYSTEM_START               90001
//@@	90001	����ϵͳ	      ϵͳ�����ɹ�
#define  MBC_ACTION_SYSTEM_END                 90002
//@@	90002	�˳�ϵͳ	      ϵͳ�˳��ɹ�
#define  MBC_ACTION_PROCESS_START              90003
//@@	90003	��������	      ���������ɹ�
#define  MBC_ACTION_PROCESS_END                90004
//@@	90004	�˳�����	      �����˳��ɹ�
#define  MBC_ACTION_MODIFY_PARAM               90005
//@@	90005	���²���	      �������³ɹ�
#define  MBC_ACTION_LOAD_PARAM                 90006
//@@	90006	���ز���	      �������سɹ�
#define  MBC_ACTION_MODIFY_DB                  90007
//@@	90007	�������ݿ�	    ���ݿ�����ɹ�
#define  MBC_ACTION_CHECK_POINT                90008
//@@	90008	����CheciPoint	CheciPoint�ɹ�
#define  MBC_ACTION_LOG_CONVERT                90009
//@@	90009	����CheciPoint	CheciPoint�ɹ�
#define  MBC_ACTION_DEBUG                      90200
//@@	90200	���Բ���	      ���������ɹ�

#define MBC_ACTION_SGWEXPORT                   90012
//@@	90012	����Sgwexport	sgwexport�ɹ�
#define MBC_ACTION_SGWIMPORT                   90013
//@@	90013	����Sgwimport	sgwimport�ɹ�

#define MBC_SWITCH_SYSTEM        90300
//@@	90300	ϵͳ״̬�л�����	      �л������ɹ�
#define MBC_DISPLAY_SYSTEM        90301
//@@	90301	��ʾsgwinit�Ĵ������̲���	��ʾ�����ɹ�

#define  MBC_ACTION_SCHEDULE_HIGH                90010
//@@	90010	������ˮλ��������	������ˮλ��������
#define  MBC_ACTION_SCHEDULE_LOW                90011
//@@	90011	������ˮλֹͣ����	������ˮλֹͣ����
#define  MBC_CTP_FILE_TRANS						90014
//@@	90014	�ļ�������Ϣ			����ļ�������Ϣ����ʾ������

#define  MBC_HSS_AUTH_LOG                      90201  //hss��Ȩ��־��

#define  MBC_THREELOG_CHANGE              90100
//@@	90100	��־���л�	��־���л�
#define  MBC_THREELOG_PIGE              90101
//@@	90101	��־��鵵	��־��鵵

#define  MBC_INSTALL_TEST              90222
//@@	90222	��װ��֤��־	��װ��֤��־

//--------------------------------------------------------------------------
#define	MBC_SGW_UNKNOW_ERROR	-1
//@@	-1	δ֪����	δ֪����
#define	MBC_SGW_SUCC 0
//@@	0	�������������ɹ�	�������������ɹ�
#define	MBC_SGW_USER_NOT_EXIST	11001
//@@	11001	�û���������	�û���������
#define	MBC_SGW_USER_LOCKED	11002
//@@	11002	�û�������	�û�������
#define	MBC_SGW_PWD_FAIL	11003
//@@	11003	�������	�������
#define	MBC_SGW_IP_FAIL		11004
//@@	11004	IP��ƥ��	IP��ƥ��
#define	MBC_SGW_SERVICE_BUSY	11007
//@@	11007	ϵͳ��æ����ʱ�����ṩ����	ϵͳ��æ����ʱ�����ṩ����
#define	MBC_SGW_CONNECT_RE_CONNECT	11008
//@@	11008	�Եȶˣ��û����Ѿ��������ӣ����յ���������	�Եȶˣ��û����Ѿ��������ӣ����յ���������
#define	MBC_SGW_MSG_UNRECOGNIZED	11009
//@@	11009	��Ϣ������ʶ��	��Ϣ������ʶ��
#define	MBC_SGW_ABNORMAL_HEARTBEAT	11010
//@@	11010	�����쳣	�����쳣
#define	MBC_SGW_THREE_AUTHENTICATION_FAILED	11020
//@@	11020	����3�μ�Ȩʧ��	����3�μ�Ȩʧ��
#define	MBC_SGW_MSG_SIZE_OVERFLOW	11034
//@@	11034	��������С������ֵ	��������С������ֵ
#define	MBC_SGW_SIG_CHECK_FAIL	11101
//@@	11101	����ǩ����֤ʧ��	����ǩ����֤ʧ��
#define	MBC_SGW_ILLEGAL_KEYWORDS	11102
//@@	11102	���зǷ��ؼ���	���зǷ��ؼ���
#define	MBC_SGW_PROTOCOL_VERSION_FAIL	11103
//@@	11103	Э��汾����	Э��汾����
#define	MBC_SGW_MISS_FIELD	11104
//@@	11104	ȱ�ٱ����ֶ�	ȱ�ٱ����ֶ�
#define	MBC_SGW_VALUE_FIELD	11105
//@@	11105	�ֶ�ֵ����	�ֶ�ֵ����
#define	MBC_SGW_UNDEFINED_FIELD	11106
//@@	11106	δ����ı�ǩ���ֶ�	δ����ı�ǩ���ֶ�
#define	MBC_SGW_FORMAT_ERROR	11107
//@@	11107	��ʽת������	��ʽת������
#define	MBC_SGW_REPACKAGE_ERROR	11108
//@@	11108	����յ��İ��Ƿ��ظ�	����յ��İ��Ƿ��ظ�
#define	MBC_SGW_UNFOUND_SR	11201
//@@	11201	û�в鵽��Ӧ��SR	û�в鵽��Ӧ��SR
#define	MBC_SGW_BON_UNAVAILABLE	11202
//@@	11202	BON������(�ⲿ�����ڲ�)	BON������(�ⲿ�����ڲ�)
#define	MBC_SGW_BON_TIMEOUT	11203
//@@	11203	�ȴ�BON������Ϣ��ʱ�������Ҫ����Ӧ��	�ȴ�BON������Ϣ��ʱ�������Ҫ����Ӧ��
#define	MBC_SGW_EXTERNAL_UNAVAILABLE	11204
//@@	11204	�ⲿ��Ԫ�����ã��ڲ������ⲿ��	�ⲿ��Ԫ�����ã��ڲ������ⲿ��
#define	MBC_SGW_EXTERNAL_TIMEOUT	11205
//@@	11205	�ⲿ��Ԫû�м�ʱ�ظ�Ӧ��(�ⲿ��Ԫ��ʱ)	�ⲿ��Ԫû�м�ʱ�ظ�Ӧ��(�ⲿ��Ԫ��ʱ)
#define MBC_SGW_BON_RETURN_ERROR 11206
//@@	11206	BON ���ش���	BON ���ش���
#define	MBC_SGW_SCSP_INFO	11207
//@@	11207	scspЭ����Ϣ	scspЭ����Ϣ
#define	MBC_SGW_SERVICEPACKET_INFO	11208
//@@	11208	ҵ��������װ����	ҵ��������װ����
#define	MBC_SGW_TUXEDO_INFO	11209
//@@	11209	tuxedoЭ����Ϣ	tuxedoЭ����Ϣ
#define	MBC_SGW_SOAP_INFO	11210
//@@	11210	soapЭ����Ϣ	soapЭ����Ϣ
#define	MBC_SGW_DCC_INFO	11211
//@@	11211	dccЭ����Ϣ	dccЭ����Ϣ
//#define	MBC_PUBLIC_MALLOC_FAIL	1
////@@  1 �ڴ����ʧ��
//#define	MBC_PUBLIC_MEMERY_NOT_ENOUGH	2
////@@  2 �ڴ�ռ䲻��
//#define	MBC_PUBLIC_MEMERY_QUERY_TAB_FAIL	3
////@@  3 �ڴ����ӿڴ���
//#define	MBC_PUBLIC_ATTCH_MEMERY_FAIL	4
////@@  4 �����ڴ�����ʧ��
//#define	MBC_PUBLIC_FORK_OR_THREAD_FAIL	5
////@@  5 �������̻��߳�ʧ��
//#define	MBC_PUBLIC_SIGN_HANDLE_FAIL	6
////@@  6 �źŴ���ʧ��
//#define	MBC_PUBLIC_LOCK_FAIL	7
////@@  7 ��ȡ����Դʧ��
//#define	MBC_PUBLIC_LISTEN_FAIL	8
////@@  8 Socket--��������
//#define	MBC_PUBLIC_CONNECT_SERV_FAIL	9
////@@  9 ����Զ��Socket��
//#define	MBC_PUBLIC_DISK_NOT_ENOUGH	10
////@@  10 ���̴洢�ռ䲻��
//#define	MBC_PUBLIC_CPU_OVERLOAD		11
////@@  11 CPU������
//#define	MBC_PUBLIC_CONNECT_DB_FAIL	12
////@@  12 �������ݿ�ʧ��
//#define	MBC_PUBLIC_TABSPACE_NOT_ENOUGH	13
////@@  13 ���ռ䲻��
//#define	MBC_PUBLIC_EVN_UNSET	14
////@@  14 ��������δ����
//#define	MBC_PUBLIC_SERVICE_BUSY	15
////@@  15 ϵͳ��æ

#define	MBC_SGW_READ_MEMERY_FAIL	11301
//@@	11301	��ȡ�����ڴ������쳣	���鹲���ڴ�����
#define	MBC_SGW_AVP_TYPE_UNDEF	11302
//@@	11302	AVP��������δ����	����AVP�������Ͷ���
#define	MBC_SGW_MSG_MAP_TYPE_UNDEF	11303
//@@	11303	��Ϣӳ������δ����	������Ϣӳ�������Ͷ���
#define	MBC_SGW_CONVERT_EXIT	11304
//@@	11304	��Ϣת�������˳�	��ο�ϵͳ��־
#define	MBC_SGW_MISS_AVP	11305
//@@	11305	δ�ҵ������е�AVPֵ	�������û���ϢԴ
#define	MBC_SGW_UNFOUND_SESSION_BUFF	11306
//@@	11306	δ�һỰ����Ϣ��	����Ϣ�Ƿ񷵻�
#define	MBC_SGW_UNDEF_GETVALUE_TYPE	11307
//@@	11307	δ�����ȡֵ����	��ȡֵ��������
#define	MBC_SGW_CREATE_XML_FAIL	11308
//@@	11308	����xml����ʧ��	�����Ƿ����㹻�ڴ�
#define	MBC_SGW_CONVERT_FAIL	11309
//@@	11309	��Ϣת��ʧ��	������Ϣ������
#define	MBC_SGW_UNDEF_DCC_CMD	11310
//@@	11310	δ����DCC������	������Ϣ����
#define	MBC_SGW_PARSE_MSG_FAIL	11311
//@@	11311	������Ϣʧ��	������Ϣ����
#define	MBC_SGW_UNDEF_PROTO_TYPE	11312
//@@	11312	��֧�ֵ�Э������	�������Э��
#define	MBC_SGW_UNDEF_CONVERT_TYPE	11313
//@@	11313	��֧�ֵ���Ϣ����ת��	������Ϣӳ������
#define	MBC_SGW_MSG_MAP_FAIL	11314
//@@	11314	��Ϣӳ�����ô���	������Ϣӳ������
#define MBC_SGW_EXT_NE_LINK_MISS	11315
//@@	11315	�ⲿ��Ԫ���ӶϿ�	���鳬ʱ����
#define	MBC_SGW_PARAM_DEAL_FAIL	11316
//@@	11316	SGW��������ʧ��	SGW��������ʧ��
#define	MBC_SGW_DISCARD_MESSAGE	11317
//@@	11317	������Ϣ	ӵ��������Ϣ
#define	MBC_SGW_CONGESTION_INFO	11318
//@@	11318	ӵ����Ϣ	ӵ��������Ϣ
#define	MBC_SGW_MESSAGE_INFO	11319
//@@	11319	��Ϣ������Ϣ	��Ϣ������Ϣ
#define	MBC_SGW_MESSAGE_OUT_TIME	11800
//@@	11800	��Ϣ����ʱ	��Ϣ����ʱ
#define	MBC_SGW_SR_ROUTE_FAIL	3002
//@@	3002	�Ʒ���·���쳣	�Ʒ���·���쳣

#define	MBC_SGW_SYSTEM_MAINTAIN	11500
//@@	11500	ϵͳ����ά��	ϵͳ����ά��
#define	MBC_SGW_MSG_PACKET_FAIL	11501
//@@	11501	��Ϣ������	������ṹ
#define	MBC_SGW_SYSTEM_BUSY	11502
//@@	11502	ϵͳ���������У���Ҫ�ͷ�����	ϵͳ���������У���Ҫ�ͷ�����
#define	MBC_SGW_CLR_TIMEOUT	11503
//@@	11503	CLR��ʱ	������·�����
#define	MBC_SGW_USRE_BALACKLIST	11504
//@@	11504	�Զ˱����������	�Զ˱����������
#define	MBC_SGW_USER_MSG_TOOBIG	11505
//@@	11505	�ڵ�λʱ��T��NE2��NE1���͵���Ϣ�ֽ����ܺ��Ѿ�����ϵͳ���������ֵ	�뽵�ͷ���Ƶ��
#define	MBC_SGW_USER_MSG_TIMES_TOOMORE	11506
//@@	11506	�ڵ�λʱ��T��NE2��NE1���͵�����������������Ѿ�����ϵͳ���������ֵ	�뽵�ͷ���Ƶ��
#define	MBC_SGW_USER_MISS_REQ	11507
//@@	11507	�Զ�����Ӧ	������·�����
#define	MBC_SGW_SCSP_PROTOCOL_VERSION_FAIL	11508
//@@	11508	Э��汾�Ŵ���	Э��汾�Ŵ���
#define	MBC_SGW_UNDOWN_FAIL	11599
//@@	11599	����ԭ��	δ֪�쳣
#define	MBC_SGW_DCC_FAIL	99999
//@@	99999	SGWУ��DCC��Ϣʧ��	SGWУ��DCC��Ϣʧ��
#endif
