#ifndef __MBC_SGW_H__
#define __MBC_SGW_H__




//#
//# 基本编码规则 2位目录编号+3位模块编号+4位信息编号
//#AVP编码规则为 2位目录编号+7位信息编号
#define MBC_MEM_MALLOC_FAIL						1
//@@	1	内存分配失败		请检查是否有足够的内存
#define MBC_MEM_SPACE_SMALL						2
//@@	2	内存空间不足		请检查是否有足够的内存
#define MBC_MEM_INTERFACE_ERROR					3
//@@	3	内存查表接口错误	请检查是否有足够的内存
#define MBC_MEM_LINK_FAIL						4
//@@	4	共享内存连接失败	请检查是否有足够的内存
#define MBC_THREAD_CREAT_FAIL					5
//@@	5	派生进程或线程失败	请检查是否有足够的内存
#define MBC_SEMP_OPERA_FAIL						6
//@@	6	信号处理失败		请检查是否有足够的内存
#define MBC_LOCK_GET_FAIL							7
//@@	7	获取锁资源失败		请检查是否有足够的内存
#define MBC_SOCKET_LISTEN_FAIL					8
//@@	8	Socket--监听出错	请检查Socket的连接是否正常
#define MBC_SOCKET_LINK_FAIL						9
//@@	9	连接远端Socket错	请检查端口是否被占用
#define MBC_DISC_SPACE_SMALL						10
//@@	10	磁盘存储空间不足	请整理磁盘空间，并检查是否有程序文件占用大量磁盘空间
#define MBC_CPU_OVERLOAD							11
//@@	11	CPU超负荷			请检查是否系统资源是否被进程占用
#define MBC_DB_LINK_FAIL							12
//@@	12	连接数据库失败		请检查用户名密码是否正确
#define MBC_DB_TABLESPACE_SMALL					13
//@@	13	表空间不足			请检查是否有足够的表空间
#define MBC_ENV_NOT_SET							14
//@@	14	环境变量未设置		请设置环境变量
#define MBC_SYS_BUSY								15
//@@	15	系统繁忙	      请检查主机资源
#define MBC_MAX_DCC_REQUEST_CNT							16
//@@	16	DCC消息请求达到最大值			请检查是否系统进程运行是否正常
#define MBC_SHM_ERRNO_DATA_NOT_EXIST			50
//@@	50	共享内存数据区不存在	请检查共享内存数据区是否创建
#define MBC_SHM_ERRNO_INDEX_NOT_EXIST			51
//@@	51	共享内存索引区不存在	请检查共享内存索引是否创建
#define MBC_SHM_ERRNO_DATA_NOTCONNECT			52
//@@	52	未连接共享内存数据区	请重新连接共享内存数据区
#define MBC_SHM_ERRNO_INDEX_NOTCONNECT			53
//@@	53	未连接共享内存索引区	请重新连接共享内存索引区
#define MBC_SHM_ERRNO_ADD_DATA_EXIST			54
//@@	54	共享内存数据区已经存在	无
#define MBC_SHM_ERRNO_UPDATEINFO_NOTEXIST		55
//@@	55	共享内存更新的数据不存在		请改用插入数据
#define MBC_SHM_ERRNO_DEL_DATA_NOTEXIST			56
//@@	56	删除的数据共享内存不存在	无
#define MBC_SHM_ERRNO_DEL_INDEX_FAILED			57
//@@	57	删除数据的索引失败	无
#define MBC_SHM_ADD_DATA_EXIST					58
//@@	58	添加的数据已经存在	无
#define MBC_SHM_QUERY_NO_DATE_FOUND				59
//@@	59	查询数据不存在	无
#define MBC_SHM_ULOAD_DATA_NOT_EXIST			60
//@@	60	卸载的数据区不存在	无
#define MBC_SHM_LOAD_NO_DATA					61
//@@	61	加载的表数据不存在	无
#define MBC_SHM_OPT_NOT_SUPPORT					62
//@@	62	共享内存不支持此种操作	无
#define MBC_SHM_NOT_CONNECT_DATA				63
//@@	63	未连接数据区	重新连接共享内存
#define MBC_CFG_GETPARAM_FAILED					64
//@@	64	获取核心参数失败	查看配置文件是否配置
#define MBC_SHM_INIT_FAILED						65   
//@@	65	初始化共享内存失败	查看错误日志信息

#define MBC_FTP_CREAT_FAIL                    30
#define MBC_FTP_SEND_FAIL                     31
#define MBC_FTP_RECV_FAIL                     32


#define MBC_FILE_OPEN_FAIL						101
//@@	101	打开文件失败		请检查文件是否存在
#define MBC_FILE_WRITE_FAIL						102
//@@	102	写文件失败			请检查文件是否存在
#define MBC_DIR_OPEN_FAIL							103
//@@	103	打开目录出错		请检查目录是否存在，路径是否正确
#define MBC_FILE_RENAME_FAIL						104
//@@	104	文件改名出错		请检查文件是否存在，是否被占用
#define MBC_FILE_SEQ_GET_FAIL						105
//@@	105	序列获取失败		请检查序列是否存在，是否被占用
#define MBC_FILE_NOT_EXIST						106
//@@	106	指定文件不存在		请检查文件名，访问目录是否正确
#define MBC_FILE_COPY_FAIL						107
//@@	107	拷贝文件失败		请检查文件名，访问目录是否正确
#define MBC_FILE_MOVE_FAIL						108
//@@	108	移动文件失败		请检查文件名，访问目录是否正确
#define MBC_FILE_LOG_FAIL						109
//@@	109	日志组文件丢失		请检查文件名，访问目录是否正确

#define MBC_MEM_UPDATE_FAIL						116
//@@	116	共享内存更新失败	请检查是否有足够的内存
#define MBC_MEM_IMPORT_FAIL						117
//@@	117	上载异常			请检查是否有足够的内存
#define MBC_MEM_EXPORT_FAIL						118
//@@	118	内存数据导出异常	请检查是否有足够的空间
#define MBC_MEM_CREATE_FAIL						119
//@@	119	创建共享内存失败	请检查是否有足够的内存
#define MBC_MEM_DELETE_FAIL						120
//@@	120	删除共享内存失败	请检查共享内存地址是否正确
#define MBC_MEM_DESTROY_FAIL						121
//@@	121	销毁共享内存失败	请检查共享内存是否正被使用
#define MBC_SEMP_CREATE_FAIL						122
//@@	122	创建信号量失败		请检查信号量是否已被使用
#define MBC_PV_ERRNO_P_FAILED						123
//@@	123	信号量上锁失败		请检查信号量是否已被时候

#define MBC_DB_SERV_PWD_ERROR						141
//@@	141	用户名密码错误		请检查用户名及密码
#define MBC_DB_LINK_CLOSE_FAIL					142
//@@	142	关闭数据库连接失败	请检查数据库连接是否正常
#define MBC_DB_TABLE_NOT_EXIT						143
//@@	143	表名不存在			请检查表名称
#define MBC_DB_ROLLBACK_FAIL						144
//@@	144	数据库事务回滚错误	请检查数据库事务的完整
#define MBC_DB_EXECUTE_FAIL						145
//@@	145	发生sql执行异常		请检查SQL语句是否正确

#define MBC_DB_OP_FAIL          146
//@@	146	数据库操作失败	请检查SQL语句是否正确

#define MBC_NET_PROTOCOL_PACKAGE_FAIL				156
//@@	156	协议封装失败					请检查封装协议
#define MBC_NET_PROTOCOL_PARSE_FAIL				157
//@@	157	协议解析失败	        请检查解析协议


#define MBC_PARAM_LOAD_ERROR				165
//@@	165	参数加载失败					检查参数文件
#define MBC_PARAM_FILE_FORMAT_ERROR				166
//@@	166	配置文件格式错误					检查配置文件格式
#define MBC_PARAM_THREAD_LOST_ERROR				167
//@@	167	参数不存在	在核心参数配置新增此参数
#define MBC_PARAM_THREAD_NONSUPP_ERROR			168
//@@	168	应用进程参数不正确（不支持的选项）	请检查选项参数
#define MBC_PARAM_THREAD_VALUE_ERROR				169
//@@	169	应用进程参数不正确（错误的取值）	请修改参数值
#define MBC_PARAM_UPDATE_FAIL						170
//@@	170	参数更新失败	            检查参数文件状态，检查参数格式。
#define MBC_PARAM_FILE_ERROR						171
//@@	171	参数文件不存在						检查参数文件路径
#define MBC_PARAM_LOST_ERROR						172
//@@	172	参数配置信息缺失					检查参数配置信息完整性
#define MBC_PARAM_VALUE_NULL_ERROR				173
//@@	173	非空检查（字段不能为空)	  检查字段，是否存在非空字段为空值
#define MBC_PARAM_LEN_MIN_ERROR					174
//@@	174	长度检查（小于最小长度	  检查字段，是否存在字段长度小于设定的最小长度
#define MBC_PARAM_LEN_MAX_ERROR					175
//@@	175	长度检查（大于最大长度)	  检查字段，是否存在字段长度大于设定的最大长度
#define MBC_PARAM_LEN_FIXED_ERROR					176
//@@	176	长度检查（非固定长度)	    检查参数配置信息完整性
#define MBC_PARAM_VALUE_ENUM_ERROR				177
//@@	177	枚举检查(枚举值不正确)	  检查参数值是否在参数的枚举范围内
#define MBC_PARAM_VALUE_MIN_ERROR					178
//@@	178	数值范围检查（小于最小值)	检查参数数值范围是否小于设定的最小值
#define MBC_PARAM_VALUE_MAX_ERROR					179
//@@	179	数值范围检查（大于最大值)	检查参数数值范围是否大于设定的最大值
#define MBC_PARAM_ERROR							180
//@@	180	参数错误	                检查不在已定义的参数格式错误中，但属于参数类的错误

#define MBC_INFO_CUST_GET_FAIL                  181
//@@	181	取客户信息失败	          检查客户信息是否存在
#define MBC_INFO_ACCT_GET_FAIL                  182
//@@	182	取账户信息失败	          检查账户信息是否存在
#define MBC_INFO_SERV_GET_FAIL                  183
//@@	183	取用户信息失败	          检查用户信息是否存在

#define MBC_SYSTEM_START_ERROR         195
//@@	195	系统启动失败					检查系统状态
#define MBC_THREAD_EXIST_ERROR						196
//@@	196	应用进程已经存在					检查该应用进程是否已经存在
#define MBC_THREAD_ERROR							197
//@@	197	进程异常						检查相关配置
#define MBC_THREAD_STATUS_ERROR					198
//@@	198	进程状态非法						请检查进程是否正常运行
#define MBC_THREAD_EXIT_ERROR						199
//@@	199	应用程序收到退出信号异常退出		检查该应用进程状态，检查系统状态

#define MBC_THREAD_EXIT_NORMAL                  //程序收到安全退出信号正常退出

#define MBC_CHECK_POINT_ERROR     200
//@@	200	CHECKPOINT异常退出		检查该内存数据或落地文件

#define MBC_MAX_SYSTEM_PROCESS     201
//@@	201	达到系统最大进程限制		调整核心参数，增加系统最大进程数限制

#define MBC_SRVC_SEQ_CONTINUE_FAIL				213
//@@	213	序号连续性校验失败				检查序号的连续性
#define MBC_SRVC_RECEIVE_FAIL						232
//@@	232	系统接收异常数据					检查系统状态，检查系统数据传输
#define MBC_SRVC_SEQ_REPEAT_FAIL					240
//@@	240	流水号重复				        检查是否存在重复流水号


#define MBC_PARAM_DATE_FORMAT_ERROR				246
//@@	246	时间检查（时间格式不正确）									检查时间格式是否正确
#define MBC_PARAM_DATE_MIN_ERROR					247
//@@	247	时间检查（小于最小时间）									检查时间是否小于最小时间
#define MBC_PARAM_DATE_MAX_ERROR					248
//@@	248	时间检查（大于最大时间）										检查时间是否大于最大时间
#define MBC_PARAM_STRING_ERROR					249
//@@	249	字符串检查（出现非法字符）								检查字符串里是否出现非法字符
#define MBC_PARAM_IP_FORMAT_ERROR					250
//@@	250	IP地址检查（IP地址格式不正确）							检查IP地址的格式
#define MBC_PARAM_ACCNBR_FORMAT_ERROR				251
//@@	251	号码格式不符合要求	                        检查号码格式是否正确
#define MBC_PARAM_PROVINCE_FORMAT_ERROR				252
//@@	252	省份编码检查（省份编码不正确）	            检查省份编码是否正确
#define MBC_PARAM_LOCAL_FORMAT_ERROR				253
//@@	253	本地网编码检查（本地网编码不正确）					检查本地网编码是否正确
#define MBC_PARAM_POST_FORMAT_ERROR				254
//@@	254	邮政编码检查（邮政不符合要求）							检查邮政编码是否符合要求
#define MBC_PARAM_NUMBER_FORMAT_ERROR				255
//@@	255	数据类型检查（数字型字段出现字符）					检查数字型字段是否出现字符
#define MBC_PARAM_CHARS_ERROR						256
//@@	256	数据类型检查（字符型字段出现非法字符）			检查字符型字段是否出现非法字符


#define MBC_ARCHIVING_PROCESS_ERROR				261
//@@	261	数据归档失败	      检查数据归档配置，执行权限，磁盘空间
#define MBC_OPERATE_ERROR							262
//@@	262	操作失败	          检查无法归入已有错误的操作失败


#define MBC_NP_NOT_EXIST							301
//@@	301	携号转网数据不存在	                               检查是否存在携号转网数据
#define MBC_NP_EXP								302
//@@	302	携号转网数据已失效	                               检查携号转网数据状态，是否已失效
#define MBC_NP_INVALID							303
//@@	303	携号转网数据未生效	                               检查携号转网数据状态，是否未生效
#define MBC_NP_SERV_REMOVED						304
//@@	304	用户已拆机        	                               检查携号转网用户状态，是否已拆机


#define MBC_IBRST_CARRIER_NOT_EXIST				311
//@@	311	C网国漫被访运营商不存在            	               检查C网国际漫游被访运营商是否存在
#define MBC_IBRST_CARRIER_EXP						312
//@@	312	C网国漫被访运营商已失效            	               检查C网国际漫游被访运营商资料状态，是否已失效
#define MBC_IBRST_CARRIER_VALID					313
//@@	313	C网国漫被访运营商尚未生效          	               检查C网国际漫游被访运营商资料状态，是否未生效
#define MBC_IBRST_COUNTRY_NOT_EXIST				314
//@@	314	C网国漫被访运营商所在国家不存在    	               检查C网国际漫游被访运营商所在国家是否不存在
#define MBC_IBRST_REGION_NOT_EXIST				315
//@@	315	C网国漫被访运营商所属资费区域不存在	               检查C网国际漫游被访运营商所属资费区域是否不存在



#define MBC_CTOG_CARRIER_NOT_EXIST				321
//@@	321	不同制式国漫被访运营商不存在            	         检查不同制式国际漫游被访运营商是否存在
#define MBC_CTOG_CARRIER_EXP						322
//@@	322	不同制式国漫被访运营商已失效            	         检查不同制式国际漫游被访运营商资料状态，是否已失效
#define MBC_CTOG_CARRIER_INVALID					323
//@@	323	不同制式国漫被访运营商尚未生效          	         检查不同制式国际漫游被访运营商资料状态，是否未生效
#define MBC_CTOG_COUNTRY_NOT_EXIST				324
//@@	324	不同制式国漫被访运营商所在国家不存在    	         检查不同制式国际漫游检查被访运营商所在国家是否不存在
#define MBC_CTOG_NOT_EXIST						325
//@@	325	不同制式国漫CtoG代理运营商不存在        	         检查不同制式国际漫游CtoG代理运营商是否不存在
#define MBC_CTOG_REGION_NOT_EXIST					326
//@@	326	不同制式国漫被访运营商所属资费区域不存在	         检查不同制式国际漫游检查被访运营商所属资费区域是否不存在


#define MBC_MIN_NOT_EXIST							331
//@@	331	MIN码不存在                      	                 检查MIN码是否存在
#define MBC_MIN_EXP								332
//@@	332	MIN码已失效                      	                 检查MIN码是否已失效
#define MBC_MIN_INVALID							333
//@@	333	MIN码尚未生效                    	                 检查MIN码是否未生效
#define MBC_MIN_COUNTRY_NOT_EXIST					334
//@@	334	MIN码归属国家代码异常（不存在）  	                 检查MIN码归属国家代码是否不存在
#define MBC_MIN_COUNTRY_EXP						335
//@@	335	MIN码归属国家代码异常（失效）    	                 检查MIN码归属国家代码是否已失效
#define MBC_MIN_COUNTRY_MUL						336
//@@	336	MIN码归属国家代码异常（多个）    	                 检查是否存在多个MIN码归属国家代码
#define MBC_MIN_CARRIER_NOT_EXIT					337
//@@	337	MIN码归属运营商代码异常（不存在）	                 检查MIN码归属运营商代码是否不存在
#define MBC_MIN_CARRIER_EXP						338
//@@	338	MMIN码归属运营商异常（失效）     	                 检查MIN码归属运营商代码是否已失效
#define MBC_MIN_CARRIER_MUL						339
//@@	339	MIN码归属运营商代码异常（多个）  	                 检查是否存在多个MIN码归属运营商代码



#define	 MBC_HSS_IMSI_CODE	 346
//@@	346	IMSI码不存在                  	                   检查IMSI码是否存在
#define MBC_IMSI_EXP								347
//@@	347	IMSI码已失效                  	                   检查IMSI码是否已失效
#define MBC_IMSI_INVALID							348
//@@	348	IMSI码尚未生效                	                   检查IMSI码是否未生效
#define MBC_IMSI_CTOG_ERROR						349
//@@	349	IMSI对应CtoG代理运营商代码异常	                   检查IMSI与CtoG代理运营商代码的对应关系
#define MBC_IMSI_LOCAL_MUL						350
//@@	350	IMSI对应多个本地网            	                   检查是否存在IMSI对应多个本地网的情况



#define	 MBC_COUNTRY_NOT_EXIST	 356
//@@	356	国家信息不存在  	                                 检查国家信息是否存在
#define	 MBC_COUNTRY_EXP	 357
//@@	357	国家信息已失效  	                                 检查国家信息是否已失效
#define MBC_COUNTRY_INVALID						358
//@@	358	国家信息尚未生效	                                 检查国家信息是否未生效
#define MBC_COUNTRY_MUL							359
//@@	359	国家代码重复    	                                 检查是否存在重复的国家代码
#define MBC_COUNTRY_ERROR							360
//@@	360	国家名称异常    	                                 检查是否存在异常国家名称


#define MBC_HCODE_NOT_EXIST						366
//@@	366	H码不存在                  	                       检查H码是否存在
#define MBC_HCODE_EXP								367
//@@	367	H码已失效                  	                       检查H码是否已失效
#define MBC_HCODE_INVALID							368
//@@	368	H码尚未生效                	                       检查H码是否未生效
#define MBC_HCODE_LOCAL_MUL						369
//@@	369	H码归属本地网异常（多个）  	                       检查是否存在H码归属多个本地网
#define MBC_HCODE_CARRIER_MUL						370
//@@	370	H码归属运营商异常（多个）  	                       检查是否存在H码归属多个运营商
#define MBC_HCODE_CARRIER_NOT_EXIT				371
//@@	371	H码归属运营商异常（不存在）	                       检查是否存在H码无归属运营商



#define MBC_SP_NOT_EXIST							376
//@@	376	SP代码不存在  	                                   检查SP代码是否存在
#define MBC_SP_EXP								377
//@@	377	SP代码已失效  	                                   检查SP代码是否已失效
#define MBC_SP_INVALID							378
//@@	378	SP代码尚未生效	                                   检查SP代码是否未生效


#define MBC_BORDER_ROAM_PROVINCE_MUL				386
//@@	386	边漫话单归属多个省	                               检查边漫话单，边漫归属配置
#define ERR_RULE_PARAM				          1900
//@@	1900	参数规则配置错误	                               检查参数规则配置

#define MBC_SERVICE_CONTEXT_NOT_EXIST                 11005
//@@	11005	业务能力不存在	          检查用户的业务能力是否存在
#define MBC_SERVICE_CONTEXT_NOT_PERMISSION            11006
//@@	11006	业务能力无权限访问	    检查用户是否有权限访问此业务能力
#define MBC_FLOW_OF_BYTES_WARN            11030
//@@	11030	周期内流量字节数超过告警阀值	    流量字节数已达到告警值，请检查！
#define MBC_FLOW_OF_BYTES_DINCON            11031
//@@	11031	周期内流量字节数超过断链阀值	    流量字节数已达到断链值，请检查！
#define MBC_FLOW_OF_REQUESTED_WARN            11032
//@@	11032	周期内流量次数超过告警阀值	    流量次数已达到告警值，请检查！
#define MBC_FLOW_OF_REQUESTED_DINCON            11033
//@@	11033	周期内流量次数超过断链阀值	    流量次数已达到断链值，请检查！


#define MBC_HSS_UPDATE_NOT_EXIST					12000
//@@	12000	更新记录不存在        	                         检查更新的记录是否存在
#define MBC_HSS_LAST_CONTINUE_NOT_EXIT			12001
//@@	12001	找不到上一个连续消息包	                         检查传递消息包的连续性
#define MBC_HSS_AUTH_PAYPWD_FAIL					12002
//@@	12002	支付密码鉴权失败      	                         检查支付密码鉴权
#define MBC_HSS_CUSTID_ERROR						12003
//@@	12003	大客户标识不符合要求  	                         检查大客户标识是否符合规范
#define MBC_HSS_PRODUCTID_ERROR					12004
//@@	12004	产品编码不符合要求    	                         检查产品编码是否符合规范



#define MBC_HSS_SERV_CUST_ERROR					12100
//@@	12100	有用户资料没有客户资料	                         检查用户的客户资料是否存在
#define MBC_HSS_SERV_ACCT_ERROR					12101
//@@	12101	有用户资料没有帐户资料	                         检查用户的帐户资料是否存在
#define MBC_HSS_ACCT_CUST_ERROR					12102
//@@	12102	有帐户资料没有客户资料	                         检查帐户的客户资料是否存在
#define MBC_HSS_REFCUST_NOT_EXIT					12103
//@@	12103	依赖的大客户不存在    	                         检查依赖的大客户资料是否存在
#define MBC_HSS_REFSERV_NOT_EXIT					12104
//@@	12104	依赖的用户信息不存在  	                         检查依赖的用户信息是否存在
#define MBC_HSS_REFACCT_NOT_EXIT					12105
//@@	12105	依赖的账户信息不存在  	                         检查依赖的账户信息是否存在
#define MBC_HSS_BANK_NOT_EXIST					12106
//@@	12106	银行托收无银行帐号    	                         检查银行托收的银行帐号
#define MBC_HSS_SERV_MAX_ERROR					12107
//@@	12107	超过最大用户数        	                         检查用户数是否超过设定的最大用户数



#define MBC_HSS_OFFERINS_DATE_NOT_CONST_ERROR		12200
//@@	12200	销售品实例与销售品包含对象实例生失效时间不一致	 检查销售品实例与销售品包含对象实例的生失效时间是否一致
#define MBC_HSS_OFFER_EXP_MIN_EFF_ERROR			12201
//@@	12201	销售品失效日期小于生效日期                    	 检查销售品配置的失效日期是否小于生效日期
#define MBC_HSS_OFFER_MEMBER_NUM_ERROR			12202
//@@	12202	销售品包含对象成员数量异常（产品缺失或多）    	 检查销售品包含对象成员数量
#define MBC_HSS_OFFER_MEMBER_TYPE_ERROR			12203
//@@	12203	销售品包含对象成员类型异常                    	 检查销售品包含对象成员类型
#define MBC_HSS_OFFERINS_ATTR_LOST_ERROR			12204
//@@	12204	缺少必填的销售品属性实例                      	 检查是否缺少必填的销售品属性实例
#define MBC_HSS_OFFERINS_PARAM_LOST_ERROR			12205
//@@	12205	缺少必填的销售品参数实例                      	 检查是否缺少必填的销售品参数实例
#define MBC_HSS_OFFERINS_NUM_MAX_ERROR			12206
//@@	12206	超过最大销售品实例数                          	 检查销售品实例数是否超过设定的最大销售品实例数


#define MBC_HSS_GROUPMEMBER_NODATA_ERROR			12250
//@@	12250	电信网内群成员资料在主产品实例表无数据	         检查电信网内群成员资料是否在主产品实例表无数据


#define MBC_HSS_CUST_CREATE_MAX_ERROR			    12260
//@@	12260	开户失败	                                       检查用户资料完整性


#define MBC_HSS_OFFER_CATALOG_APPROVE_FAIL		12270
//@@	12270	销售品目录审批失败	                             检查销售品目录规范

//Command
#define MBC_COMMAND_SGWPARAM           		11801
//@@	11801	执行sgwparam	      执行sgwparam
#define MBC_COMMAND_SGWPROCESS         		11802
//@@	11802	执行process	      执行process
#define MBC_COMMAND_SGWMONITOR         		11803
//@@	11803	执行sgwmonitor	      执行sgwmonitor

/*    操作码    */
#define  MBC_ACTION_SEARCH_PARAM               90000
//@@	90000	查找参数	      参数查找成功
#define  MBC_ACTION_SYSTEM_START               90001
//@@	90001	启动系统	      系统启动成功
#define  MBC_ACTION_SYSTEM_END                 90002
//@@	90002	退出系统	      系统退出成功
#define  MBC_ACTION_PROCESS_START              90003
//@@	90003	启动程序	      程序启动成功
#define  MBC_ACTION_PROCESS_END                90004
//@@	90004	退出程序	      程序退出成功
#define  MBC_ACTION_MODIFY_PARAM               90005
//@@	90005	更新参数	      参数更新成功
#define  MBC_ACTION_LOAD_PARAM                 90006
//@@	90006	加载参数	      参数加载成功
#define  MBC_ACTION_MODIFY_DB                  90007
//@@	90007	操作数据库	    数据库操作成功
#define  MBC_ACTION_CHECK_POINT                90008
//@@	90008	启动CheciPoint	CheciPoint成功
#define  MBC_ACTION_LOG_CONVERT                90009
//@@	90009	启动CheciPoint	CheciPoint成功
#define  MBC_ACTION_DEBUG                      90200
//@@	90200	调试操作	      调整操作成功

#define MBC_ACTION_SGWEXPORT                   90012
//@@	90012	启动Sgwexport	sgwexport成功
#define MBC_ACTION_SGWIMPORT                   90013
//@@	90013	启动Sgwimport	sgwimport成功

#define MBC_SWITCH_SYSTEM        90300
//@@	90300	系统状态切换操作	      切换操作成功
#define MBC_DISPLAY_SYSTEM        90301
//@@	90301	显示sgwinit的处理过程操作	显示操作成功

#define  MBC_ACTION_SCHEDULE_HIGH                90010
//@@	90010	触发高水位启动进程	触发高水位启动进程
#define  MBC_ACTION_SCHEDULE_LOW                90011
//@@	90011	触发低水位停止进程	触发低水位停止进程
#define  MBC_CTP_FILE_TRANS						90014
//@@	90014	文件传递信息			检查文件传递信息所提示的内容

#define  MBC_HSS_AUTH_LOG                      90201  //hss鉴权日志号

#define  MBC_THREELOG_CHANGE              90100
//@@	90100	日志组切换	日志组切换
#define  MBC_THREELOG_PIGE              90101
//@@	90101	日志组归档	日志组归档

#define  MBC_INSTALL_TEST              90222
//@@	90222	安装验证日志	安装验证日志

//--------------------------------------------------------------------------
#define	MBC_SGW_UNKNOW_ERROR	-1
//@@	-1	未知错误	未知错误
#define	MBC_SGW_SUCC 0
//@@	0	接受请求或操作成功	接受请求或操作成功
#define	MBC_SGW_USER_NOT_EXIST	11001
//@@	11001	用户名不存在	用户名不存在
#define	MBC_SGW_USER_LOCKED	11002
//@@	11002	用户被锁定	用户被锁定
#define	MBC_SGW_PWD_FAIL	11003
//@@	11003	密码错误	密码错误
#define	MBC_SGW_IP_FAIL		11004
//@@	11004	IP不匹配	IP不匹配
#define	MBC_SGW_SERVICE_BUSY	11007
//@@	11007	系统繁忙，暂时不能提供服务	系统繁忙，暂时不能提供服务
#define	MBC_SGW_CONNECT_RE_CONNECT	11008
//@@	11008	对等端（用户）已经建立链接，又收到链接请求	对等端（用户）已经建立链接，又收到链接请求
#define	MBC_SGW_MSG_UNRECOGNIZED	11009
//@@	11009	消息包不可识别	消息包不可识别
#define	MBC_SGW_ABNORMAL_HEARTBEAT	11010
//@@	11010	心跳异常	心跳异常
#define	MBC_SGW_THREE_AUTHENTICATION_FAILED	11020
//@@	11020	连续3次鉴权失败	连续3次鉴权失败
#define	MBC_SGW_MSG_SIZE_OVERFLOW	11034
//@@	11034	单个包大小超过阀值	单个包大小超过阀值
#define	MBC_SGW_SIG_CHECK_FAIL	11101
//@@	11101	数字签名验证失败	数字签名验证失败
#define	MBC_SGW_ILLEGAL_KEYWORDS	11102
//@@	11102	含有非法关键字	含有非法关键字
#define	MBC_SGW_PROTOCOL_VERSION_FAIL	11103
//@@	11103	协议版本错误	协议版本错误
#define	MBC_SGW_MISS_FIELD	11104
//@@	11104	缺少必填字段	缺少必填字段
#define	MBC_SGW_VALUE_FIELD	11105
//@@	11105	字段值错误	字段值错误
#define	MBC_SGW_UNDEFINED_FIELD	11106
//@@	11106	未定义的标签或字段	未定义的标签或字段
#define	MBC_SGW_FORMAT_ERROR	11107
//@@	11107	格式转换错误	格式转换错误
#define	MBC_SGW_REPACKAGE_ERROR	11108
//@@	11108	检查收到的包是否重复	检查收到的包是否重复
#define	MBC_SGW_UNFOUND_SR	11201
//@@	11201	没有查到对应的SR	没有查到对应的SR
#define	MBC_SGW_BON_UNAVAILABLE	11202
//@@	11202	BON不可用(外部访问内部)	BON不可用(外部访问内部)
#define	MBC_SGW_BON_TIMEOUT	11203
//@@	11203	等待BON返回消息超时（如果需要返回应答）	等待BON返回消息超时（如果需要返回应答）
#define	MBC_SGW_EXTERNAL_UNAVAILABLE	11204
//@@	11204	外部网元不可用（内部访问外部）	外部网元不可用（内部访问外部）
#define	MBC_SGW_EXTERNAL_TIMEOUT	11205
//@@	11205	外部网元没有及时回复应答(外部网元超时)	外部网元没有及时回复应答(外部网元超时)
#define MBC_SGW_BON_RETURN_ERROR 11206
//@@	11206	BON 返回错误	BON 返回错误
#define	MBC_SGW_SCSP_INFO	11207
//@@	11207	scsp协议信息	scsp协议信息
#define	MBC_SGW_SERVICEPACKET_INFO	11208
//@@	11208	业务能力封装管理	业务能力封装管理
#define	MBC_SGW_TUXEDO_INFO	11209
//@@	11209	tuxedo协议信息	tuxedo协议信息
#define	MBC_SGW_SOAP_INFO	11210
//@@	11210	soap协议信息	soap协议信息
#define	MBC_SGW_DCC_INFO	11211
//@@	11211	dcc协议信息	dcc协议信息
//#define	MBC_PUBLIC_MALLOC_FAIL	1
////@@  1 内存分配失败
//#define	MBC_PUBLIC_MEMERY_NOT_ENOUGH	2
////@@  2 内存空间不足
//#define	MBC_PUBLIC_MEMERY_QUERY_TAB_FAIL	3
////@@  3 内存查表接口错误
//#define	MBC_PUBLIC_ATTCH_MEMERY_FAIL	4
////@@  4 共享内存连接失败
//#define	MBC_PUBLIC_FORK_OR_THREAD_FAIL	5
////@@  5 派生进程或线程失败
//#define	MBC_PUBLIC_SIGN_HANDLE_FAIL	6
////@@  6 信号处理失败
//#define	MBC_PUBLIC_LOCK_FAIL	7
////@@  7 获取锁资源失败
//#define	MBC_PUBLIC_LISTEN_FAIL	8
////@@  8 Socket--监听出错
//#define	MBC_PUBLIC_CONNECT_SERV_FAIL	9
////@@  9 连接远端Socket错
//#define	MBC_PUBLIC_DISK_NOT_ENOUGH	10
////@@  10 磁盘存储空间不足
//#define	MBC_PUBLIC_CPU_OVERLOAD		11
////@@  11 CPU超负荷
//#define	MBC_PUBLIC_CONNECT_DB_FAIL	12
////@@  12 连接数据库失败
//#define	MBC_PUBLIC_TABSPACE_NOT_ENOUGH	13
////@@  13 表空间不足
//#define	MBC_PUBLIC_EVN_UNSET	14
////@@  14 环境变量未设置
//#define	MBC_PUBLIC_SERVICE_BUSY	15
////@@  15 系统繁忙

#define	MBC_SGW_READ_MEMERY_FAIL	11301
//@@	11301	读取共享内存数据异常	请检查共享内存数据
#define	MBC_SGW_AVP_TYPE_UNDEF	11302
//@@	11302	AVP数据类型未定义	请检查AVP数据类型定义
#define	MBC_SGW_MSG_MAP_TYPE_UNDEF	11303
//@@	11303	消息映射类型未定义	请检查消息映射类型型定义
#define	MBC_SGW_CONVERT_EXIT	11304
//@@	11304	消息转换进程退出	请参考系统日志
#define	MBC_SGW_MISS_AVP	11305
//@@	11305	未找到配置中的AVP值	请检查配置或消息源
#define	MBC_SGW_UNFOUND_SESSION_BUFF	11306
//@@	11306	未找会话的消息包	请消息是否返回
#define	MBC_SGW_UNDEF_GETVALUE_TYPE	11307
//@@	11307	未定义的取值类型	请取值类型配置
#define	MBC_SGW_CREATE_XML_FAIL	11308
//@@	11308	创建xml对象失败	请检查是否有足够内存
#define	MBC_SGW_CONVERT_FAIL	11309
//@@	11309	消息转换失败	请检查消息与配置
#define	MBC_SGW_UNDEF_DCC_CMD	11310
//@@	11310	未定义DCC命令码	请检查消息类型
#define	MBC_SGW_PARSE_MSG_FAIL	11311
//@@	11311	解析消息失败	请检查消息数据
#define	MBC_SGW_UNDEF_PROTO_TYPE	11312
//@@	11312	不支持的协议类型	请检查接入协议
#define	MBC_SGW_UNDEF_CONVERT_TYPE	11313
//@@	11313	不支持的消息类型转换	请检查消息映射配置
#define	MBC_SGW_MSG_MAP_FAIL	11314
//@@	11314	消息映射配置错误	请检查消息映射配置
#define MBC_SGW_EXT_NE_LINK_MISS	11315
//@@	11315	外部网元连接断开	请检查超时设置
#define	MBC_SGW_PARAM_DEAL_FAIL	11316
//@@	11316	SGW参数处理失败	SGW参数处理失败
#define	MBC_SGW_DISCARD_MESSAGE	11317
//@@	11317	丢弃消息	拥塞丢弃消息
#define	MBC_SGW_CONGESTION_INFO	11318
//@@	11318	拥塞信息	拥塞管理信息
#define	MBC_SGW_MESSAGE_INFO	11319
//@@	11319	消息处理信息	消息处理信息
#define	MBC_SGW_MESSAGE_OUT_TIME	11800
//@@	11800	消息超超时	消息超超时
#define	MBC_SGW_SR_ROUTE_FAIL	3002
//@@	3002	计费网路由异常	计费网路由异常

#define	MBC_SGW_SYSTEM_MAINTAIN	11500
//@@	11500	系统正常维护	系统正常维护
#define	MBC_SGW_MSG_PACKET_FAIL	11501
//@@	11501	消息包错误	请检查包结构
#define	MBC_SGW_SYSTEM_BUSY	11502
//@@	11502	系统超负载运行，需要释放链接	系统超负载运行，需要释放链接
#define	MBC_SGW_CLR_TIMEOUT	11503
//@@	11503	CLR超时	请检查链路或程序
#define	MBC_SGW_USRE_BALACKLIST	11504
//@@	11504	对端被纳入黑名单	对端被纳入黑名单
#define	MBC_SGW_USER_MSG_TOOBIG	11505
//@@	11505	在单位时间T内NE2向NE1发送的消息字节数总和已经超过系统允许的最大值	请降低发包频率
#define	MBC_SGW_USER_MSG_TIMES_TOOMORE	11506
//@@	11506	在单位时间T内NE2向NE1发送的能力调用请求次数已经超过系统允许的最大值	请降低发包频率
#define	MBC_SGW_USER_MISS_REQ	11507
//@@	11507	对端无响应	请检查链路或程序
#define	MBC_SGW_SCSP_PROTOCOL_VERSION_FAIL	11508
//@@	11508	协议版本号错误	协议版本号错误
#define	MBC_SGW_UNDOWN_FAIL	11599
//@@	11599	其它原因	未知异常
#define	MBC_SGW_DCC_FAIL	99999
//@@	99999	SGW校验DCC信息失败	SGW校验DCC信息失败
#endif

