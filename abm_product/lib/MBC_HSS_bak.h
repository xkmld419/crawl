#ifndef __MBC_HSS_H__
#define __MBC_HSS_H__
//#
//# ����������� 2λĿ¼���+3λģ����+4λ��Ϣ���
//#AVP�������Ϊ 2λĿ¼���+7λ��Ϣ���

#define	MBC_HSS_LIB	0
//@@	0	�ɹ�		�ɹ�
#define MBC_MEM_MALLOC_FAIL						1
//@@	1	�ڴ����ʧ��		�����Ƿ����㹻���ڴ�
#define MBC_MEM_SPACE_SMALL						2
//@@	2	�ڴ�ռ䲻��		�����Ƿ����㹻���ڴ�
#define MBC_MEM_INTERFACE_ERROR					3
//@@	3	�ڴ���ӿڴ���	�����Ƿ����㹻���ڴ�
#define MBC_MEM_LINK_FAIL							4
//@@	4	�����ڴ�����ʧ��	�����Ƿ����㹻���ڴ�
#define MBC_THREAD_CREAT_FAIL						5
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
//@@	10	���̴洢�ռ䲻��	��������̿ռ䣬������Ƿ��г����ļ�ռ�ô������̿ռ�
#define MBC_CPU_OVERLOAD							11
//@@	11	CPU������			�����Ƿ�ϵͳ��Դ�Ƿ񱻽���ռ��
#define MBC_DB_LINK_FAIL							12
//@@	12	�������ݿ�ʧ��		�����û��������Ƿ���ȷ
#define MBC_DB_TABLESPACE_SMALL					13
//@@	13	��ռ䲻��			�����Ƿ����㹻�ı�ռ�
#define MBC_ENV_NOT_SET							14
//@@	14	��������δ����		�����û�������
#define MBC_SYS_BUSY								15
//@@	15	ϵͳ��æ	      ����������Դ

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

#define MBC_NET_PROTOCOL_PACKAGE_FAIL				156
//@@	156	Э���װʧ��					�����װЭ��
#define MBC_NET_PROTOCOL_PARSE_FAIL				157
//@@	157	Э�����ʧ��	        �������Э��


#define MBC_PARAM_LOAD_ERROR				165
//@@	165	��������ʧ��					�������ļ�
#define MBC_PARAM_FILE_FORMAT_ERROR				166
//@@	166	�����ļ���ʽ����					��������ļ���ʽ
#define MBC_PARAM_THREAD_LOST_ERROR				167
//@@	167	Ӧ�ý��̲�������ȷ��ȱ�ٱ�ѡ�	����ӱ�ѡ�����
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
#define MBC_HREAD_EXIST_ERROR						196
//@@	196	Ӧ�ý����Ѿ�����					����Ӧ�ý����Ƿ��Ѿ�����
#define MBC_HREAD_ERROR							197
//@@	197	Ԥ��������쳣						���Ԥ�����������
#define MBC_HREAD_STATUS_ERROR					198
//@@	198	����״̬�Ƿ�						��������Ƿ���������
#define MBC_HREAD_EXIT_ERROR						199
//@@	199	Ӧ�ó����յ��˳��ź��쳣�˳�		����Ӧ�ý���״̬�����ϵͳ״̬

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
#define MBC_PARAM_IP_FROMAT_ERROR					250
//@@	250	IP��ַ��飨IP��ַ��ʽ����ȷ��							���IP��ַ�ĸ�ʽ
#define MBC_PARAM_ACCNBR_FROMAT_ERROR				251
//@@	251	�����ʽ������Ҫ��	                        �������ʽ�Ƿ���ȷ
#define MBC_PARAM_ACCNBR_FROMAT_ERROR				252
//@@	252	ʡ�ݱ����飨ʡ�ݱ��벻��ȷ��	            ���ʡ�ݱ����Ƿ���ȷ
#define MBC_PARAM_LOCAL_FROMAT_ERROR				253
//@@	253	�����������飨���������벻��ȷ��					��鱾���������Ƿ���ȷ
#define MBC_PARAM_POST_FROMAT_ERROR				254
//@@	254	���������飨����������Ҫ��							������������Ƿ����Ҫ��
#define MBC_PARAM_NUMBER_FROMAT_ERROR				255
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



#define MBC_HSS_UPDATE_NOT_EXIST					12000
//@@	12000	���¼�¼������        	                         �����µļ�¼�Ƿ����
#define MBC_HSS_LAST_CONTINUE_NOT_EXIT			12001
//@@	12001	�Ҳ�����һ��������Ϣ��	                         ��鴫����Ϣ����������
#define MBC_HSS_AUTH_PAYPWD_FAIL					12002
//@@	12002	֧�������Ȩʧ��      	                         ���֧�������Ȩ
#define MBC_HSS_CUSTID_ERROR						12003
//@@	12003	��ͻ���ʶ������Ҫ��  	                         ����Ʒ�����Ƿ���Ϲ淶
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



/*    ������    */
#define  MBC_ACTION_SYSTEM_START               100001
//@@	100001	����ϵͳ	      ϵͳ�����ɹ�
#define  MBC_ACTION_SYSTEM_END                 100002
//@@	100002	�˳�ϵͳ	      ϵͳ�˳��ɹ�
#define  MBC_ACTION_PROCESS_START              100003
//@@	100003	��������	      ���������ɹ�
#define  MBC_ACTION_PROCESS_END                100004
//@@	100004	�˳�����	      �����˳��ɹ�
#define  MBC_ACTION_MODIFY_PARAM               100005
//@@	100005	���²���	      �������³ɹ�
#define  MBC_ACTION_LOAD_PARAM                 100006
//@@	100006	���ز���	      �������سɹ�
#define  MBC_ACTION_MODIFY_DB                  100007
//@@	100007	�������ݿ�	    ���ݿ�����ɹ�
#define  MBC_ACTION_CHECK_POINT                100008
//@@	100008	����CheciPoint	CheciPoint�ɹ�
#define  MBC_ACTION_DEBUG                      200000
//@@	200000	���Բ���	      ���������ɹ�
#endif
