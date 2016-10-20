/*VER: 7*/ 
#ifndef __MBC_H__
#define __MBC_H__

#define MBC_FileNameRule  1000
#define MBC_HeadTailRule  1001
#define MBC_AccuCfg       1002
#define MBC_AccuRule      1003
//#include "Log.h"
#include "MBC_28.h"
#define MBC				999990000
#define MBC_MEM_IMPORT_FAIL                                             117
//@@    117     �����쳣                        �����Ƿ����㹻���ڴ�
#define MBC_PARAM_THREAD_LOST_ERROR                             167
//@@    167     ����������      �ں��Ĳ������������˲���
#define MBC_PARAM_THREAD_NONSUPP_ERROR                  168
//@@    168     Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�      ����ѡ�����
#define MBC_MEM_EXPORT_FAIL                                             118
//@@    118     �ڴ����ݵ����쳣        �����Ƿ����㹻�Ŀռ�
#define MBC_PARAM_THREAD_VALUE_ERROR                            169
//@@    169     Ӧ�ý��̲�������ȷ�������ȡֵ��        ���޸Ĳ���ֵ
#define MBC_DB_SERV_PWD_ERROR                                           141
//@@    141     �û����������          �����û���������
#define MBC_DB_LINK_CLOSE_FAIL                                  142
//@@    142     �ر����ݿ�����ʧ��      �������ݿ������Ƿ�����
#define MBC_DB_TABLE_NOT_EXIT                                           143
//@@    143     ����������                      ���������
#define MBC_DB_ROLLBACK_FAIL                                            144
//@@    144     ���ݿ�����ع�����      �������ݿ����������
#define MBC_DB_EXECUTE_FAIL                                             145
//@@    145     ����sqlִ���쳣         ����SQL����Ƿ���ȷ
//#
//# �����������: 2λĿ¼���+3λģ����+4λ��Ϣ���
//#
#define MBC_MEM_MALLOC_FAIL                                             1
//@@    1       �ڴ����ʧ��            �����Ƿ����㹻���ڴ�
#define MBC_MEM_SPACE_SMALL                                             2
//@@    2       �ڴ�ռ䲻��            �����Ƿ����㹻���ڴ�
#define MBC_MEM_INTERFACE_ERROR                                 3
//@@    3       �ڴ���ӿڴ���        �����Ƿ����㹻���ڴ�
#define MBC_MEM_LINK_FAIL                                               4
//@@    4       �����ڴ�����ʧ��        �����Ƿ����㹻���ڴ�
#define MBC_THREAD_CREAT_FAIL                                   5
//@@    5       �������̻��߳�ʧ��      �����Ƿ����㹻���ڴ�
#define MBC_SEMP_OPERA_FAIL                                             6
//@@    6       �źŴ���ʧ��            �����Ƿ����㹻���ڴ�
#define MBC_LOCK_GET_FAIL                                                       7
//@@    7       ��ȡ����Դʧ��          �����Ƿ����㹻���ڴ�
#define MBC_SOCKET_LISTEN_FAIL                                  8
//@@    8       Socket--��������        ����Socket�������Ƿ�����
#define MBC_SOCKET_LINK_FAIL                                            9
//@@    9       ����Զ��Socket��        ����˿��Ƿ�ռ��
#define MBC_DISC_SPACE_SMALL                                            10
//@@    10      ���̴洢�ռ䲻��        ��������̿ռ䣬������Ƿ��г����ļ�ռ�ô������̿ռ�
#define MBC_CPU_OVERLOAD                                                        11
//@@    11      CPU������                       �����Ƿ�ϵͳ��Դ�Ƿ񱻽���ռ��
#define MBC_DB_LINK_FAIL                                                        12
//@@    12      �������ݿ�ʧ��          �����û��������Ƿ���ȷ
#define MBC_DB_TABLESPACE_SMALL                                 13
//@@    13      ��ռ䲻��                      �����Ƿ����㹻�ı�ռ�
#define MBC_ENV_NOT_SET                                                 14
//@@    14      ��������δ����          �����û�������
#define MBC_SYS_BUSY                                                            15
//@@    15      ϵͳ��æ              ����������Դ
#define MBC_MAX_DCC_REQUEST_CNT                                                 16
//@@    16      DCC��Ϣ����ﵽ���ֵ                   �����Ƿ�ϵͳ���������Ƿ�����
#define MBC_SHM_ERRNO_DATA_NOT_EXIST                    50
//@@    50      �����ڴ�������������    ���鹲���ڴ��������Ƿ񴴽�
#define MBC_SHM_ERRNO_INDEX_NOT_EXIST                   51
//@@    51      �����ڴ�������������    ���鹲���ڴ������Ƿ񴴽�
#define MBC_SHM_ERRNO_DATA_NOTCONNECT                   52
//@@    52      δ���ӹ����ڴ�������    ���������ӹ����ڴ�������
#define MBC_SHM_ERRNO_INDEX_NOTCONNECT                  53
//@@    53      δ���ӹ����ڴ�������    ���������ӹ����ڴ�������
#define MBC_SHM_ERRNO_ADD_DATA_EXIST                    54
//@@    54      �����ڴ��������Ѿ�����  ��
#define MBC_SHM_ERRNO_UPDATEINFO_NOTEXIST               55
//@@    55      �����ڴ���µ����ݲ�����                ����ò�������
#define MBC_SHM_ERRNO_DEL_DATA_NOTEXIST                 56
//@@    56      ɾ�������ݹ����ڴ治����        ��
#define MBC_SHM_ERRNO_DEL_INDEX_FAILED                  57
//@@    57      ɾ�����ݵ�����ʧ��      ��
#define MBC_SHM_ADD_DATA_EXIST                                  58
//@@    58      ��ӵ������Ѿ�����      ��
#define MBC_SHM_QUERY_NO_DATE_FOUND                             59
//@@    59      ��ѯ���ݲ�����  ��
#define MBC_SHM_ULOAD_DATA_NOT_EXIST                    60
//@@    60      ж�ص�������������      ��
#define MBC_SHM_LOAD_NO_DATA                                    61
//@@    61      ���صı����ݲ�����      ��
#define MBC_SHM_OPT_NOT_SUPPORT                                 62
//@@    62      �����ڴ治֧�ִ��ֲ���  ��
#define MBC_SHM_NOT_CONNECT_DATA                                63
//@@    63      δ����������    �������ӹ����ڴ�
#define MBC_CFG_GETPARAM_FAILED                                 64
//@@    64      ��ȡ���Ĳ���ʧ��        �鿴�����ļ��Ƿ�����
#define MBC_SHM_INIT_FAILED                                             65
//@@    65      ��ʼ�������ڴ�ʧ��      �鿴������־��Ϣ
//# MBC_Example
#define MBC_Example			990010000
//@@ 990010001	Example����Exception1 	----
//@@ 990010002	Example����Exception2   ----

//#
//# MBC_LIB
#define MBC_APP_AccuStd								101
#define MBC_APP_adjustfee             102
#define MBC_APP_apportion             103
#define MBC_APP_archive               104
#define MBC_APP_asn                   105
#define MBC_APP_backup                106
#define MBC_APP_billfee               107
#define MBC_APP_billstat              108
#define MBC_APP_CfgExp                109
#define MBC_APP_checkaudit            110
#define MBC_APP_checkback             111
#define MBC_APP_checkdup              112
#define MBC_APP_CheckFile             113
#define MBC_APP_CheckRecord           114
#define MBC_APP_CheckRoamDup          115
#define MBC_APP_CmdParser             116
#define MBC_APP_Collect               117
#define MBC_APP_CommExp               118
#define MBC_APP_count                 119
#define MBC_APP_CreateErrFile         120
#define MBC_APP_Credit                121
#define MBC_APP_creditcal             122
#define MBC_APP_ctlacctitem           123
#define MBC_APP_ctlaccu               124
#define MBC_APP_ctlinfopoint          125
#define MBC_APP_ctlofferaggr          126
#define MBC_APP_DailyAcctItem         127
#define MBC_APP_DataEventDivMain      128
#define MBC_APP_DataEventMemAdm       129
#define MBC_APP_DataExport            130
#define MBC_APP_Datafilterstop        131
#define MBC_APP_Datastopbuf           132
#define MBC_APP_dccCheckRecord        133
#define MBC_APP_dccPack               134
#define MBC_APP_dccTran               135
#define MBC_APP_dccUnPack             136
#define MBC_APP_disctmaker            137
#define MBC_APP_distribute            138
#define MBC_APP_E8UserMain            139
#define MBC_APP_EdgeStationInfo       140
#define MBC_APP_errfileInDb           141
#define MBC_APP_EventGet              142
#define MBC_APP_eventmaker            143
#define MBC_APP_eventoutput           144
#define MBC_APP_exportuserinfo        145
#define MBC_APP_fileinstore           146
#define MBC_APP_fileinstore_stat      147
#define MBC_APP_filterstop            148
#define MBC_APP_format                149
#define MBC_APP_gather                150
#define MBC_APP_gendmfile             151
#define MBC_APP_hbctl                 152
#define MBC_APP_highFee               153
#define MBC_APP_incept                154
#define MBC_APP_Init                  155
#define MBC_APP_ivpn                  156
#define MBC_APP_load                  157
#define MBC_APP_loadaggrctl           158
#define MBC_APP_LoadBalance           159
#define MBC_APP_LoadIvpnInfo          160
#define MBC_APP_loadparam             161
#define MBC_APP_loadparaminfo         162
#define MBC_APP_loadstatlog           163
#define MBC_APP_loaduserctl           164
#define MBC_APP_loaduserinfo          165
#define MBC_APP_loadvpninfo           166
#define MBC_APP_loggroup              167
#define MBC_APP_mergeevent            168
#define MBC_APP_modifyacct            169
#define MBC_APP_msgQueTest            170
#define MBC_APP_msgSend               171
#define MBC_APP_offerremind           172
#define MBC_APP_offersimu             173
#define MBC_APP_output                174
#define MBC_APP_overtime              175
#define MBC_APP_owe                   176
#define MBC_APP_Points                177
#define MBC_APP_prep                  178
#define MBC_APP_pricing               179
#define MBC_APP_procerrfile           180
#define MBC_APP_putinfile             181
#define MBC_APP_rebillex              182
#define MBC_APP_recvHbCharge          183
#define MBC_APP_RecycleEvent          184
#define MBC_APP_redoevents            185
#define MBC_APP_remindbuf             186
#define MBC_APP_remindmaker           187
#define MBC_APP_remindtemplate        188
#define MBC_APP_rentex                189
#define MBC_APP_RoamCheckDup          190
#define MBC_APP_RTDataStop            191
#define MBC_APP_rt_make_stop          192
#define MBC_APP_schedule              193
#define MBC_APP_Score                 194
#define MBC_APP_sendFile              195
#define MBC_APP_setex                 196
#define MBC_APP_show                  197
#define MBC_APP_sockComm              198
#define MBC_APP_sp_bill_percent       199
#define MBC_APP_srSimulator           200
#define MBC_APP_stat                  201
#define MBC_APP_stattree              202
#define MBC_APP_stopbuf               203
#define MBC_APP_tdtrans               204
#define MBC_APP_to_tdtrans            205
#define MBC_APP_trans                 206
#define MBC_APP_TransTermInfo         207
#define MBC_APP_trustbank             208
#define MBC_APP_tygroup               209
#define MBC_APP_UpdateE8Info          210
#define MBC_APP_updatenpinfo          211
#define MBC_APP_updateuserinfo        212
#define MBC_APP_updatevpninfo         213
#define MBC_APP_UserInterface         214
#define MBC_APP_virement              215
#define MBC_APP_vpn                   216
#define MBC_APP_writefiledb           217
#define MBC_APP_writelocal            218
#define MBC_APP_memmgr                219
#define MBC_APP_loggrum               220
#define MBC_APP_seekuser              221
//add by jx 20100917
#define MBC_APP_infomonitor           222
//add by xn 20100927
#define MBC_APP_scheduleserver        224
#define MBC_APP_statserver            225
#define	MBC_APP_testinfo	      226

// add by zhangch 20101001
#define MBC_APP_dccSrv                		226
#define MBC_APP_dccSend                   227
#define MBC_APP_tuxClient                 228
#define MBC_APP_contextBill               229
#define MBC_APP_CheckPointCharge          230
#define MBC_APP_ArrearageCheck            231
#define MBC_APP_QueryBalance              232
#define MBC_APP_WriteOff				          233
#define MBC_APP_ReversePaymen             234
#define MBC_APP_QueryProdHSS              235
#define MBC_APP_QueryVisitCountry			    236
#define MBC_APP_BalanceTrans      				237
#define MBC_APP_RealtimeFeeQuery      		238
#define MBC_APP_FeeBillQuery          		239
#define MBC_APP_NmpSysParam               240
#define MBC_APP_hblogg										520
#define MBC_APP_hbparam										982
#define MBC_APP_checkpoint								984
#define MBC_APP_hbshm											987
#define MBC_APP_DRQHighUsage		241
#define MBC_APP_IRQHighUsage		242
#define MBC_APP_IFQHighUsage		243
#define	MBC_APP_PointQuery		244
#define MBC_APP_GetUserHssInfo		245
#define MBC_APP_GetUserData		246
#define MBC_APP_GetCustData		247
#define	MBC_APP_GetAcctData		248
#define MBC_APP_GetServData		249
#define MBC_APP_GetServAcctData		250
#define	MBC_APP_GetServStateData		251
#define MBC_APP_GetServProdData		252
#define MBC_APP_HQBillQueryBill		253
#define MBC_APP_HQBillTest		254
#define MBC_APP_UpdUserAndRecord	255
//Command Group      
//----------------------------------------
#define MBC_COMMAND_hbsyscheck				501
#define MBC_COMMAND_hbinstalltest     502
#define MBC_COMMAND_hbswinstall       503
#define MBC_COMMAND_hbswuninstall     504
#define MBC_COMMAND_Hbcheckpoint      505
#define MBC_COMMAND_hbcleantest       506
#define MBC_COMMAND_hbadmin           507
#define MBC_COMMAND_hbparam           508
#define MBC_COMMAND_hbinit            509
#define MBC_COMMAND_hbprocess         510
#define MBC_COMMAND_hbshm             511
#define MBC_COMMAND_hbmonitor         512
#define MBC_COMMAND_hbmode            513
#define MBC_COMMAND_hbstat            514
#define MBC_COMMAND_hbimport          515
#define MBC_COMMAND_hbexport          516
#define MBC_COMMAND_hbaccess          517
#define MBC_COMMAND_hbhelp            518
#define MBC_COMMAND_hbfinderr         519
#define MBC_COMMAND_hblogg            520
//******************************************************
//Class_ID
//******************************************************
#define MBC_CLASS_Fatal										0				//���ش���                      
#define MBC_CLASS_File                    1        //�ļ���0                       
#define MBC_CLASS_Memory                  2        //�ڴ���1                       
#define MBC_CLASS_Database                3        //���ݿ���2                     
#define MBC_CLASS_Network                 4        //������                        
#define MBC_CLASS_Parameter               5        //������                        
#define MBC_CLASS_Information             6        //��Ϣ��                        
#define MBC_CLASS_Process                 7        //������                        
#define MBC_CLASS_Business                8        //ҵ����                        
#define MBC_CLASS_LNP                     9        //Я��ת��                      
#define MBC_CLASS_Carrier                 10        //C������������Ӫ�̴���         
#define MBC_CLASS_DiffStandCarrier        11        //��ͬ��ʽ����������Ӫ�̴����ļ�
#define MBC_CLASS_MIN                     12        //MIN���                       
#define MBC_CLASS_IMSI                    13        //��ͬ��ʽIMSI���              
#define MBC_CLASS_State                   14        //������Ϣ��                    
#define MBC_CLASS_HCode                   15        //H���                         
#define MBC_CLASS_SP                      16        //SP���                        
#define MBC_CLASS_Roaming                 17        //�߽���������                  
#define MBC_CLASS_Prepaid                 18        //��ֵ����ֵ��                  
#define MBC_CLASS_RemotePayment           19        //��ؽɷ���                    
#define MBC_CLASS_HighFee                 20        //�߶���                        
#define MBC_CLASS_StaticParam             21        //��̬������                    
#define MBC_CLASS_DataTrans               22        //���ϴ�����                    
#define MBC_CLASS_HSS                     23        //HSS��ѯ��
//******************************************************
//Error_ID      
//******************************************************
//0001					//�ڴ����ʧ��                            //�������ļ����ڴ�������Ϣ�������ڴ��Ƿ��ѱ����䡣                                                                                                                      
//0002          //�ڴ�ռ䲻��                            //�������ļ����ڴ�������Ϣ������ϵͳ�ڴ��Ƿ��㹻��                                                                  
//0003          //�ڴ���ӿڴ���                        //����ڴ������ݽṹ��                                                                                                
//0004          //�����ڴ�����ʧ��                        //��鹲���ڴ��Ƿ���ڣ��������ļ����ڴ��Key��                                                                                                                            
//0005          //�������̻��߳�ʧ��                      //��������Ľ����Ƿ���ڡ�����Ƿ�ﵽϵͳ���ơ�                                                                                                                             
//0006          //�źŴ���ʧ��                            //����Ƿ���ڸ��ź����ã������ź��Ƿ�������                                                                                                                             
//0007          //��ȡ����Դʧ��                          //����Ƿ���ڸ�����Դ���ã���������Դ�Ƿ�������                                                                                                                         
//0008          //Socket--��������                        //�������˿��Ƿ��ѱ�ռ�ã�����Ƿ�ﵽϵͳ���ơ�                                                                                                                           
//0009          //����Զ��Socket��                        //���Զ�˼����˿��Ƿ��ѱ����ڣ�����Ƿ�ﵽϵͳ���ơ�                                                                                                                       
//0010          //���̴洢�ռ䲻��                        //���ϵͳ���̿ռ䡣                                                                                                                                                         
//0011          //CPU������                               //���ϵͳ�Ƿ����й���Ľ��̣����CPU�����ɷ�ֵ���á�                                                                                                                        
//0012          //�������ݿ�ʧ��                          //������ݿ����ã�������ݿ������״̬��                                                                                                                                     
//0013          //��ռ䲻��                              //������ݿ��ռ����ã�������ݿ������״̬��                                                                                                                               
//0014          //��������δ����                          //���ϵͳ����������                                                                                                                                                         
//0015          //�ڴ泬����															//������
//0016          //�������ݿ��쳣                          //������ݿ�����                                                                                                                                          
      
//0101          //���ļ�ʧ��                            //����ļ��Ƿ���ڣ�����ļ�Ȩ�����á������ˡ�                                                                                                                     
//0102          //д�ļ�ʧ��                              //����ļ��Ƿ���ڣ�����ļ�Ȩ�����á������ˡ�                                                                                                                     
//0103          //��Ŀ¼����                            //                                                                                                                     
//0104          //�ļ���������                            //                                                                                                                     
//0105          //���л�ȡʧ��                            //                                                                                                                       
//0106          //ָ���ļ�������                          //����ļ��Ƿ���ڣ�����ļ�Ȩ�����á������ˡ�                                                                                                                        
//0107          //�����ļ�ʧ��                            //����ļ��Ƿ���ڣ����Ŀ��Ŀ¼�Ƿ���ڡ�                                                                                                                           
//0108          //�ƶ��ļ�ʧ��                            //����ļ��Ƿ���ڣ�����ļ�Ȩ�����á������ˡ�                                                                                                                       
//0109          //                                                                                                                                                     
//0110          //                                                                                                                                                     
//0111          //                                                                                                                                                     
//0112          //                                                                                                                                                     
//0113          //                                                                                                                                                     
//0114          //                                                                                                                                                     
//0115          //                                                                                                                                                     
      
//0116          //�����ڴ����ʧ��                         //��鹲���ڴ��Ƿ���ڣ���鹲���ڴ�ṹ��                                                                                                                     
//0117          //�����쳣hbimport����                  //��鹲���ڴ��Ƿ���ڣ���鹲���ڴ�ṹ��                                                                                                                     
//0118          //�ڴ����ݵ����쳣hbexport                //��鹲���ڴ��Ƿ���ڣ���鹲���ڴ�ṹ��                                                                                                                     
//0119          //���������ڴ�ʧ��                         //��鹲���ڴ��Ƿ��Ѿ������ˡ�����ڴ�������                                                                                                                      
//0120          //ɾ�������ڴ�ʧ��                         //��鹲���ڴ��Ƿ���ڣ������ڴ������ߡ�                                                                                                                      
//0121          //���ٹ����ڴ�ʧ��                         //��鹲���ڴ��Ƿ���ڣ���鹲���ڴ�ṹ��                                                                                                                      
//0122          //�����ź���ʧ��                           //����ź������ã�����Ƿ�ﵽϵͳ���ơ�                                                                                                                      
//0123          //�����ڴ浽�﷧ֵ                         //                                                                                                                            
//0124          //                                                                                                                                                     
//0125          //                                                                                                                                                     
//0126          //                                                                                                                                                     
//0127          //                                                                                                                                                     
//0128          //                                                                                                                                                     
//0129          //                                                                                                                                                     
//0130          //                                                                                                                                                     
//0131          //��Ϣ���д���ʧ��												//�����Ϣ�����Ƿ���������û��Ȩ��                                                                                                                                                     
//0132          //                                                                                                                                                     
//0133          //                                                                                                                                                     
//0134          //                                                                                                                                                     
//0135          //                                                                                                                                                     
//0136          //                                                                                                                                                     
//0137          //                                                                                                                                                     
//0138          //                                                                                                                                                     
//0139          //                                                                                                                                                     
//0140          //                                                                                                                                                     
      
//0141          //�û����������                            //������ݿ����ã�������ݿ������״̬��                                                                                                                     
//0142          //�ر����ݿ�����ʧ��                        //������ݿ������״̬���������״̬��                                                                                                                       
//0143          //����������                                //                                                                                                                       
//0144          //���ݿ�����ع�����                        //                                                                                                                     
//0145          //����sqlִ���쳣                           //���Sql����﷨��������ݿ�����״̬                                                                                                                    
//0146          //���������ݿ�ʧ��                          //                                                                                                                     
//0147          //�����ɹ����                              //�ɹ�                                                                                                                      
//0148          //                                                                                                                                                     
//0149          //                                                                                                                                                     
//0150          //                                                                                                                                                     
//0151          //                                                                                                                                                     
//0152          //                                                                                                                                                     
//0153          //                                                                                                                                                     
//0154          //                                                                                                                                                     
//0155          //                                                                                                                                                     
      
//0156          //Э���װʧ�ܣ�Э������%s������ԭ��%s.      //������ݿ����ã���黰����ṹ���á�                                                                                                             
//0157          //Э�����ʧ�ܣ�Э������%s������ԭ��%s.      //����Э���װ������á�   
//0158          //tuxedo����ʧ�ܣ�ʧ��ԭ��%s                 //���tuxdo����״̬��                                                                                                                    
//0159          //ϵͳ�����ﵽ���������                     //ϵͳ�����ﵽ���������,���Ժ�����                                                                                                                                           
//0160          //                                                                                                                                                     
//0161          //                                                                                                                                                     
//0162          //                                                                                                                                                     
//0163          //                                                                                                                                                     
//0164          //                                                                                                                                                     
//0165          //                                                                                                                                                     
      
//0166          //�����ļ���ʽ����                            //��������ļ���ʽ��                                                                                                                              
//0167          //Ӧ�ý��̲�������ȷ��ȱ�ٱ�ѡ�            //���Ӧ�ý���ִ�в�����                                                                                                                          
//0168          //Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�          //���Ӧ�ý���ִ�в�����                                                                                                                          
//0169          //Ӧ�ý��̲�������ȷ�������ȡֵ��            //���Ӧ�ý���ִ�в�����                                                                                                                          
//0170          //��������ʧ��                                //�������ļ�״̬����������ʽ��                                                                                                                
//0171          //�����ļ�ȱʧ                                //�������ļ�·����                                                                                                                              
//0172          //����������Ϣȱʧ                            //������������Ϣ�����ԡ�                                                                                                                        
//0173          //                                                                                                                                                     
//0174          //                                                                                                                                                     
//0175          //                                                                                                                                                     
//0176          //                                                                                                                                                     
//0177          //                                                                                                                                                     
//0178          //                                                                                                                                                     
//0179          //                                                                                                                                                     
//0180          //                                                                                                                                                     
      
//0181          //ȡ�ͻ���Ϣʧ��                               //��鴫����Ϣ�Ƿ���ȷ�����ù����ڴ�״̬��                                                                                                                 
//0182          //ȡ�ʻ���Ϣʧ��                               //��鴫����Ϣ�Ƿ���ȷ�����ù����ڴ�״̬��                                                                                                                 
//0183          //ȡ�û���Ϣʧ��                               //��鴫����Ϣ�Ƿ���ȷ�����ù����ڴ�״̬��                                                                                                                 
//0184          //ȡ���ʧ��                                   //��鴫����Ϣ�Ƿ���ȷ�����ù����ڴ�״̬��                                                                                                                 
//0185          //ȡʵʱ����ʧ��                               //��鴫����Ϣ�Ƿ���ȷ�����ù����ڴ�״̬��                                                                                                                 
//0186          //ȡ��ʷǷ��ʧ��                               //��鴫����Ϣ�Ƿ���ȷ�����ù����ڴ�״̬��                                                                                                                 
//0187          //ȡ���ö�ʧ��                                 //��鴫����Ϣ�Ƿ���ȷ�����ù����ڴ�״̬��                                                                                                                 
//0188          //���˵�ʧ��                                   //��鴫����Ϣ�Ƿ���ȷ�����ù����ڴ�״̬��                                                                                                                 
//0189          //���嵥ʧ��                                   //��鴫����Ϣ�Ƿ���ȷ�����ù����ڴ�״̬��                                                                                                                 
//0190          //�鷢Ʊʧ��                                   //��鴫����Ϣ�Ƿ���ȷ�����ù����ڴ�״̬��                                                                                                                  
//0191          //                                                                                                                                                     
//0192          //                                                                                                                                                     
//0193          //                                                                                                                                                     
//0194          //                                                                                                                                                     
//0195          //                                                                                                                                                     
      
//0196          //Ӧ�ý����Ѿ�����                               //����Ӧ�ý����Ƿ��Ѿ����ڡ�                                                                                                                       
//0197          //Ԥ��������쳣                                 //���Ԥ����������á�                                                                                                                               
//0198          //����״̬�Ƿ�                                   //����Ӧ�ý���״̬�����ϵͳ״̬ ��
//0199          //Ӧ�ó����յ��˳��ź��쳣�˳�                   //                                                                                                                                 
//0200          //                                                                                                                                                     
//0201          //                                                                                                                                                     
//0202          //                                                                                                                                                     
//0203          //                                                                                                                                                     
//0204          //                                                                                                                                                     
//0205          //                                                                                                                                                     
//0206          //                                                                                                                                                     
//0207          //                                                                                                                                                     
//0208          //                                                                                                                                                     
//0209          //                                                                                                                                                     
//0210          //                                                                                                                                                     
      
//0211          //��ȡ��Ӧ����ʧ��                                						 //�������������Ϣ��                                                                                                                                                 
//0212          //CRM�����ӿڸ����ڴ�ʧ��                                      //��鵵���ӿ����ã���鹲���ڴ�״̬��                                                                                                                 
//0213          //���������У��ʧ��                                           //��齻�������á�                                                                                                                                     
//0214          //Դ�¼����Ͳ��Ϸ�                                             //����Դ�¼�������á�                                                                                                                               
//0215          //������ʽת��ʧ��                                             //��黰��ģ�����á�                                                                                                                                   
//0216          //�Ʒ�Ӧ�ó�ʼ��ʧ��                                           //����Ӧ�����ã�����Ӧ�ô��������                                                                                                                 
//0217          //��ȡ���ۼƻ���Ϣ����                                         //��鶨�ۼƻ���Ϣ���á�                                                                                                                               
//0218          //���ۼƻ��������                                             //��鶨�ۼƻ���Ϣ���á�                                                                                                                               
//0219          //�ƷѴ������ʧ��                                             //������������á�                                            																															                                                                                       
//0220          //�������ʧ��                                                 //������������á�                                            																															                                                                                       
//0221          //�����ַ�ʧ��                                                 //��黰���ַ����á�                                            																															                                                                                       
//0222          //��������                                                     //����ۻ������ã������ڴ��Ƿ���ڡ�                          																															                                                                                       
//0223          //�������ʧ��                                                 //������������ã������ڴ�����ݿ���Ƿ���ڡ�              																															                                                                                       
//0224          //����ʵʱ����ʧ�ܣ��磺����ʵʱ�����ر�ɷ������崦����     //���ʵʱ����������ã������ڴ�����ݿ���Ƿ���ڡ�          																															                                                                                       
//0225          //����Ƿ��ʧ��                                                 //���Ƿ��������ã������ڴ�����ݿ���Ƿ���ڡ�              																															                                                                                       
//0226          //����ͣ������ʧ��                                             //���ͣ�������ã������ؽӿ�״̬��                            																															                                                                                       
//0227          //���ɸ�������ʧ��                                             //���ͣ�������ã������ؽӿ�״̬��                            																															                                                                                       
//0228          //���ɴ߽�����ʧ��                                             //���߽����ã������ؽӿ�״̬��                              																															                                                                                       
//0229          //ȡ��Ʊ��ʽ����                                               //��鷢Ʊ��ʽ���á�                                            																															                                                                                       
//0230          //��Ʊ��ӡʧ��                                                 //��鷢Ʊ������Ϣ���������豸��Ϣ��                          																															                                                                                       
//0231          //��Χ�ӿڹ���(�糬ʱ��)                                       //�����Χ�ӿ�״̬����鳬ʱ��������Ϣ��                        																															                                                                                       
//0232          //ϵͳ�����쳣����                                                         																															                                                                                       
//0233          //�ɷѽ����������                                       //���ýɷ���Ϣ��ȷ�ԣ��������ֵ��                      																															                                                                                       
//0234          //���д�������ʧ��                                             //������д���������ã������ؽӿ�״̬��                      																															                                                                                       
//0235          //��������ʧ��                                                 //����������ã����������ȷ�ԡ�                                																															                                                                                       
//0236          //������������                                                 //�������ͣ�������ã������ؽӿ�״̬��                        																															                                                                                       
//0237          //��ֵ������ϵͳ��֧��                                     //����ֵ���ã��������ֵ��                              																															                                                                                       
//0238          //����˱�����                                                 //�������˱���Ч�ڡ�                                          																															                                                                                       
//0239          //�Ѿ����� �����ظ�����                                        //���÷���������ȷ�ԡ�                                                                                                                              
//0240          //��������                                                     //��鿴ϵͳ��־                                                                                              
//0241          //����Ӫҵ�ն���������ֵ                                       // ����ϵ����Ա�޸ķ�ֵ���Ժ�����                                                                                                                                                    
//0242          //�ƷѺ����쳣																								// �������������ԭʼ����                                                                                                                                                    
//0243          //����ͣ������																									//�û�ͣ��																														                                                                                                                                                     
//0244          //�����ص�                                                      //����ص�����                                                                                               
//0245          //                                                                                                                                                     
      
//0301          //Я��ת�����ݲ�����                                            //���Я��ת��������Ϣ�������ؽӿ�״̬��                                                                                                
//0302          //Я��ת��������ʧЧ                                            //���Я��ת��������Ϣ�������ؽӿ�״̬��                                                                                                
//0303          //Я��ת������δ��Ч                                            //���Я��ת��������Ϣ�������ؽӿ�״̬��                                                                                                 
//0304          //�û��Ѳ��                                                    //���Я��ת��������Ϣ�������ؽӿ�״̬��                                                                                                 
//0305          //                                                                                                                                                     
//0306          //                                                                                                                                                     
//0307          //                                                                                                                                                     
//0308          //                                                                                                                                                     
//0309          //                                                                                                                                                     
//0310          //                                                                                                                                                     
      
//0311          //������Ӫ�̲�����                                               //�����Ӫ��������Ϣ�������ؽӿ�״̬��                                                                                                      
//0312          //������Ӫ����ʧЧ                                               //�����Ӫ��������Ϣ�������ؽӿ�״̬��                                                                                                      
//0313          //������Ӫ����δ��Ч                                             //�����Ӫ��������Ϣ�������ؽӿ�״̬��                                                                                                      
//0314          //������Ӫ�����ڹ��Ҳ�����                                       //�����Ӫ���Լ�����������Ϣ�������ؽӿ�״̬��                                                                                              
//0315          //������Ӫ�������ʷ����򲻴���                                   //�����Ӫ���Լ��ʷ�����������Ϣ�������ؽӿ�״̬��                                                                                              
//0316          //                                                                                                                                                     
//0317          //                                                                                                                                                     
//0318          //                                                                                                                                                     
//0319          //                                                                                                                                                     
//0320          //                                                                                                                                                     

//0321          //������Ӫ�̲�����                                                //�����Ӫ��������Ϣ�������ؽӿ�״̬��                                                                                                      
//0322          //������Ӫ����ʧЧ                                                //�����Ӫ��������Ϣ�������ؽӿ�״̬��                                                                                                      
//0323          //������Ӫ����δ��Ч                                              //�����Ӫ��������Ϣ�������ؽӿ�״̬��                                                                                                      
//0324          //������Ӫ�����ڹ��Ҳ�����                                        //�����Ӫ���Լ�����������Ϣ�������ؽӿ�״̬��                                                                                              
//0325          //CtoG������Ӫ�̲�����                                            //                                                                                              
//0326          //������Ӫ���������ʷ����򲻴���                                  //�����Ӫ���Լ��ʷ�����������Ϣ�������ؽӿ�״̬��                                                                                              
//0327          //                                                                                                                                                     
//0328          //                                                                                                                                                     
//0329          //                                                                                                                                                     
//0330          //                                                                                                                                                     

//0331          //MIN�벻����                                                      //���MIN��������ã������ؽӿ�״̬��                                                                                                     
//0332          //MIN����ʧЧ                                                      //���MIN��������ã������ؽӿ�״̬��                                                                                                     
//0333          //MIN����δ��Ч                                                    //���MIN��������ã������ؽӿ�״̬��                                                                                                     
//0334          //MIN��������Ҵ����쳣�������ڣ�                                  //���MIN�Լ�����������Ϣ�������ؽӿ�״̬��                                                                                               
//0335          //MIN��������Ҵ����쳣��ʧЧ��                                    //���MIN�Լ�����������Ϣ�������ؽӿ�״̬��                                                                                               
//0336          //MIN��������Ҵ����쳣�������                                    //���MIN�Լ�����������Ϣ�������ؽӿ�״̬��                                                                                               
//0337          //MIN�������Ӫ�̴����쳣�������ڣ�                                //���MIN�Լ���Ӫ��������Ϣ�������ؽӿ�״̬��                                                                                             
//0338          //MIN�������Ӫ�̴����쳣��ʧЧ��                                  //���MIN�Լ���Ӫ��������Ϣ�������ؽӿ�״̬��                                                                                             
//0339          //MIN�������Ӫ�̴����쳣�������                                  //���MIN�Լ���Ӫ��������Ϣ�������ؽӿ�״̬��                                                                                          
//0340          //                                                                                                                                                     
//0341          //                                                                                                                                                     
//0342          //                                                                                                                                                     
//0343          //                                                                                                                                                     
//0344          //                                                                                                                                                     
//0345          //                                                                                                                                                     

//0346          //IMSI�벻����                                                      //���IMSI��������ã������ؽӿ�״̬��                                                                                                   
//0347          //IMSI����ʧЧ                                                      //���IMSI��������ã������ؽӿ�״̬��                                                                                                   
//0348          //IMSI����δ��Ч                                                    //���IMSI��������ã������ؽӿ�״̬��                                                                                                   
//0349          //IMSI��ӦCtoG������Ӫ�̴����쳣                                    //                                                                                         
//0350          //IMSI��Ӧ���������                                                //���IMSI���Լ�����������ã������ؽӿ�״̬��                                                                                            
//0351          //                                                                                                                                                     
//0352          //                                                                                                                                                     
//0353          //                                                                                                                                                     
//0354          //                                                                                                                                                     
//0355          //                                                                                                                                                     

//0356          //������Ϣ������                                    //                                                                                                             
//0357          //������Ϣ��ʧЧ                                    //                                                                                                             
//0358          //������Ϣ��δ��Ч                                  //                                                                                                             
//0359          //���Ҵ����ظ�                                      //                                                                                                             
//0360          //���������쳣                                      //                                                                                                             
//0361          //                                                                                                                                                     
//0362          //                                                                                                                                                     
//0363          //                                                                                                                                                     
//0364          //                                                                                                                                                     
//0365          //                                                                                                                                                     

//0366          //H�벻����                                          //���H��������ã������ؽӿ�״̬��                                                                                                     
//0367          //H����ʧЧ                                          //���H��������ã������ؽӿ�״̬��                                                                                                     
//0368          //H����δ��Ч                                        //���H��������ã������ؽӿ�״̬��                                                                                                     
//0369          //H������������쳣�������                          //���H���Լ�����������ã������ؽӿ�״̬��                                                                                             
//0370          //H�������Ӫ���쳣�������                          //���H���Լ���Ӫ��������ã������ؽӿ�״̬��                                                                                           
//0371          //H�������Ӫ���쳣�������ڣ�                        //���H���Լ���Ӫ��������ã������ؽӿ�״̬��                                                                                           
//0372          //                                                                                                                                                     
//0373          //                                                                                                                                                     
//0374          //                                                                                                                                                     
//0375          //                                                                                                                                                     

//0376          //SP���벻����                                        //���SP����������ã������ؽӿ�״̬��                                                                                                            
//0377          //SP������ʧЧ                                        //���SP����������ã������ؽӿ�״̬��                                                                                                            
//0378          //SP������δ��Ч                                      //���SP����������ã������ؽӿ�״̬��                                                                                                           
//0379          //                                                                                                                                                     
//0380          //                                                                                                                                                     
//0381          //                                                                                                                                                     
//0382          //                                                                                                                                                     
//0383          //                                                                                                                                                     
//0384          //                                                                                                                                                     
//0385          //                                                                                                                                                     
//0386          //���������������ʡ                                    //                                                                                                         
//0387          //                                                                                                                                                     
//0388          //                                                                                                                                                     
//0389          //                                                                                                                                                     
//0390          //                                                                                                                                                     

//0501          // �û������ڣ��û��������û���������ƥ�䣩                                              //    �������                                                                
//0502          // �û������䶳��                                                                            //   �������                                                                 
//0503          // �û�δʹ��                                                                                //   �������                                                                 
//0504          // �û����ڹ�ʧ״̬                                                                          //   �������                                                                 
//0505          // ��ˮ�Ŵ������ʻ�����ʧ��                                                                //   �������                                                                 
//0506          // ��ʡ�󸶷��û���ʱ���ṩ����ѯ                                                          //    �������                                                                
//0507          // ���й������û���eg������ֵ�û����й������û���                                            //    �������                                                                
//0508          // �û����ϴ����û��Ѳ���ȣ�                                                              //    �������                                                                
//0509          // ��ֵ�û����ޣ��������ֵ���繫������������ʡ�绰��ֵ�ȣ�                                  //    �������                                                                
//0510          // �������ʺŲ�����                                                                          //     �������                                                               
//0511          // ����������                                                                            //    �������                                                                
//0512          // ������״̬��ע��                                                                          //     �������                                                               
//0513          // ������״̬���������������������                                                          //     �������                                                               
//0514          // ������״̬����ͣ����ʱ����ʹ�ã�                                                          //     �������                                                               
//0515          // �Ʒ�ϵͳ���ݿ����                                                                        //     �������                                                               
//0516          // �Ʒ�ϵͳ�ڲ����󣨃ȴ����ʧ���ȣ�                                                        //     �������                                                               
//0517          // ��ֵʧ�ܣ����׹���Ĵ����µĳ�ֵʧ�ܣ�                                                  //     �������                                                               
//0518          // ��ֵʧ�ܣ���֧�ֿͻ�����ֵ                                                                //    �������                                                                
//0519          // ����ʧ�ܣ��û�������ѱ�ʹ�ã����½��㣬�������������ƣ�                              //    �������                                                                
//0520          // ���ʻ���Ϣʧ�ܣ�����˱������ڵȣ�                                                        //    �������                                                                
//0521          // ���ʻ���Ϣ��������������ִ��ĳЩ�����������ԣ����ʻ�ʧ�ܣ�                                //     �������                                                               
//0522          // �޷������˱����״γ�ֵ������Ҫ�����˱�����                                              //    �������                                                                
//0523          // ��ֵʧ�ܣ���Ӧ��������Ͳ����ڣ�����˱����Ͳ�֧��balanceType�����������Ч��             //    �������                                                                
//0524          // �Ʒ�ϵͳ���ڳ��ʻ�ά���ڣ���ʱ���ܳ�ֵ  ������ʱ�䣩                                      //     �������                                                               
//0525          // �˻��¸����û����࣬���ܳ�ֵ���磺һ����ͬ������1000���û���                              //     �������                                                               
//0526          // �Ҳ����ʷѲ���																																						//  �������                                                                                                                                                   
//0527          //                                                                                                                                                     
//0528          //                                                                                                                                                     
//0529          //                                                                                                                                                     
//0530          //                                                                                                                                                     
//0531          //û��Ƿ�Ѽ�¼                                                          //���ӿ�״̬������û���������Ϣ�Ƿ���ȷ��                                                                                         
//0532          //��ؽɷѹ����ػ�ȡ���ʷ������ݳ���                                    //���ӿ�״̬���ȴ��������HBϵͳ�ָ�������                                                                                         
//0533          //û��������ˮ                                                          //���ӿ�״̬��֪ͨ�����ؼ�����ݿ�״̬��                                                                                           
//0534          //��ط�����ȡ���ʼ�¼����                                              //���ӿ�״̬��֪ͨ�����ؼ��HBϵͳ״̬��                                                                                           
//0535          //                                                                                                                                                     
//0536          //                                                                                                                                                     
//0537          //                                                                                                                                                     
//0538          //                                                                                                                                                     
//0539          //                                                                                                                                                     
//0540          //                                                                                                                                                     
//0541          //��Լ����ѯ�߶�����                                                    //���߶��ѯ�������á�
//0542          //ϵͳ���ṩ�����ڵĲ�ѯ                                                //���߶��ѯ�������á�
//0543          //û�в�ѯ����Ϣ                                                        //                                                                                         
//0544          //                                                                                                                                                     
//0545          //                                                                                                                                                     
//0546          //                                                                                                                                                     
//0547          //                                                                                                                                                     
//0548          //                                                                                                                                                     
//0549          //                                                                                                                                                     
//0550          //                                                                                                                                                     
//0551          //ɾ����¼ʧ��                                                          //��龲̬�������ã������ؽӿ�״̬��                                                                                          
//0552          //���Ӽ�¼ʧ��                                                          //��龲̬�������ã������ؽӿ�״̬��                                                                                          
//0553          //���¼�¼ʧ��                                                          //��龲̬�������ã������ؽӿ�״̬��                                                                                         
//0554          //                                                                                                                                                     
//0555          //                                                                                                                                                     
//0556          //�������ϲ�����                                                        //���������ȷ�ԣ������ؽӿ�״̬��                                                                                         
//0557          //ϵͳ���ṩ��Ӧ��ѯ                                                    //���������ȷ�ԣ������ؽӿ�״̬��                                                                                         
//0558          //����ʧ��                                                              //���������ȷ�ԣ������ؽӿ�״̬��                                                                                         
//0559          //                                                                                                                                                     
//0560          //                                                                                                                                                     
//0561          //                                                                                                                                                     
//0562          //                                                                                                                                                     
//0563          //                                                                                                                                                     
//0564          //                                                                                                                                                     
//0565          //                                                                                                                                                     
//0566          //��Լ����ѯ����                                                         //���������ȷ�ԣ������ؽӿ�״̬��   
//0567          //��ѯ���ϲ�����                                                         //���������ȷ�ԣ������ؽӿ�״̬��   
//0568          //����ʧ��                                                               //���������ȷ�ԣ������ؽӿ�״̬��   
//0569          //                                                                                                                                                     
//0570          //                                                                                                                                                     
//0571          //                                                                                                                                                     
//0572          //                                                                                                                                                     
//0573          //                                                                                                                                                     
//0574          //                                                                                                                                                     
//0575          // 
//0576          //�������ݴ���																													//
//0577          //��initserverע��ʧ��																									//
//0578          //������ʽ����																													//		
//0579          //��ȡintserver���ؽ��ʧ��																							//
//0580          //��initserverͨ�Ż�ȡ���ؿ�����

//1901					//�������ֵ                            //                                                                                                                      
//1902          //С����Сֵ                            //                                                                  
//1903          //����ö��ֵ��Χ                        //                                                                                              
//1904          //��Ч����Ӧ��δ������                        //                                                                                                                            
//1905          //������Ч����                      //                                                                                                                             
//1906          //����ֵ���ɸ���                            //                                                                                                                            
//1907          //���Ǳ�����ڵĲ���                          //                                                                                                                         
//1908          //ֻ�����ӻ��С�Ĳ���                        //                                                                                                                           
//1911          //����ֻ������                        //                                                                                                                       
//1912          //����ֻ�ܼ�С                        //                                                                                                                                                      
//1913          //�����Сֵ���ô���                               //                                                                                                                        
//1914          //ʱ����ֵ�����Ϲ������÷�Χ                          //                                                                                                                                   
																				//

//----------------------------------------
//#endif //__28_TEST_

//5004					//��������ʧ��																													//PPP�����û��Ѿ�����,��ͼ�����ڶ���PPP����ʧ��					
#define MBC_MEM_INTERFACE_ERROR                                 3

//@@    3       �ڴ���ӿڴ���        �����Ƿ����㹻���ڴ�
#define MBC_SEMP_CREATE_FAIL                                            122
//@@    122     �����ź���ʧ��          �����ź����Ƿ��ѱ�ʹ��
#define	ERROR_Exceed_Max_User			1000
//1001		����ϵͳ���Ƶ�����û���		//�޸�ϵͳ���Ĳ������ã�����ϵͳ
#define ERR_RULE_PARAM				1900
	

#define	ERROR_Param_Not_Exist			172

#define	ERROR_Param_Rule_Noncomp	166			
//1202		���Ĳ�����鲻���Ϲ���			//����������
#define	ERROR_Param_Connection_Failed	141

#define	ERROR_Param_RefreshDB_Failed		145
#define	ERROR_Param_RefreshMem_Failed	172
//1204		����ˢ���ڴ�ʧ��						//����ڴ��Ƿ��иò���
#define	ERROR_Param_RefreshFile_Failed		171
#define	ERROR_Init_Cmd_Unrec						1206
//1206		�����޷�ʶ��								//�汾��ƥ��


#define	ERROR_Init_File_Access						106

#define	ERROR_Init_File_Analysis					166

#define	ERROR_Init_Boot_Failed						1301
//1301	����������ʧ��								//����������ʧ�ܼ������

#define	ERROR_Init_Msg_Failed						131

#define	ERROR_Init_Proc_Fork							5

#define	ERROR_Param_Not_Set						14

#define	ERROR_Param_Max_Users					1401
//1401	��������û���								//�����Ĳ���������

#define	ERROR_Param_Max_Products			1402
//1402	��������Ʒ��								///�����Ĳ�������

#define	ERROR_Param_Max_Plans					1403
//1403	������󶨼ۼƻ���						//�����Ĳ�������

#define	ERROR_Param_Max_Offers					1404
//1404	���������Ʒ��								//�����Ĳ�������

#define ERROR_Process_Max_Times					1405
//1405	��������ʧ��									//������������������ֵ

#define ERROR_RejectFile                                        1406
//1406	���������ļ�                                                           // ���鼯���·��ļ�


//#
#define MBC_LIB				100000000
#define MBC_AvlTree			100010000
#define MBC_Compress			100020000
#define MBC_Date			100030000
//@@ 100030001	��������ʧ��	�������ڴ��͸�ʽ��(sDateStr, sFormat)
//@@ 100030002	ȡϵͳʱ��ʧ��	���gettimeofday�����Լ�����
#define MBC_Directory			100040000
#define MBC_Encrypt			100050000
#define MBC_Exception			100060000
#define MBC_File			100070000
#define MBC_FtpClient			100080000
//@@ 100080001	ftp��¼ʧ��	����ftp�����Ƿ�����ͨ�Լ��û��������Ƿ���ȷ
//@@ 100080002	ִ������ʧ��	����ftp�����Ƿ�����ͨ
//@@ 100080003	��ȡ�ļ��б�ʧ��	����ftp�Ƿ���Է���
//@@ 100080004	ftp��ȡ�ļ�ʧ��	����ftp�������Ƿ���Է���
//@@ 100080005	�����ļ�ʧ��	����ftp�������Ƿ���Է���
//@@ 100080006	ɾ���������ϵ��ļ�ʧ��	����ftp�������Ƿ���Է���
//@@ 100080007	����ftpԶ�̱���Ŀ¼ʧ��	����ftp������Զ�̱���Ŀ¼�Ƿ���Է���


#define MBC_HashList			100090000
//@@ 100090001	��������ʧ��		�����Ƿ����㹻���ڴ�
//@@ 100090002	��������ʧ��		HashListValue�������󣬲���Ϊ0
#define MBC_KeyList			100100000
//@@ 100100001	��������ʧ��		�����Ƿ����㹻���ڴ�
#define MBC_KeyTree			100110000
//@@ 100110001	��������ʧ��		�����Ƿ����㹻���ڴ�
#define MBC_MessageQueue		100120000
//@@ 100120012	��������ʧ��		�����Ƿ����㹻���ڴ�
//@@ 100120101	����Ϣ����ʧ��	����Ȩ������
//@@ 100120102	����Ϣ����ʧ��	��Ϣ���в����ڣ�������Ϣ�����Ƿ�ɾ��
//@@ 100120103	����Ϣ����ʧ��	����򿪶��еĲ������Ƿ���Ч
//@@ 100120104	����Ϣ����ʧ��	���ܴ����Ѿ����ڵ���Ϣ����
//@@ 100120105	����Ϣ����ʧ��	�����Ƿ����㹻���ڴ洴������
//@@ 100120106	����Ϣ����ʧ��	ϵͳ������
//@@ 100120107	����Ϣ����ʧ��	û�пɶ�ȡ����Ϣ
//@@ 100120108	����Ϣ����ʧ��	�յ��ж��ź�
//@@ 100120109	����Ϣ����ʧ��	������Ϣ�����Ƿ��Ѿ����ر�
//@@ 100120110	����Ϣ����ʧ��	��Ϣ��������
//@@ 100120111	����Ϣ����ʧ��	δ֪�Ĵ���
//@@ 100120201	������Ϣʧ��		����Ȩ������
//@@ 100120202	������Ϣʧ��		��Ϣ���в����ڣ�������Ϣ�����Ƿ�ɾ��
//@@ 100120203	������Ϣʧ��		����򿪶��еĲ������Ƿ���Ч
//@@ 100120204	������Ϣʧ��		���ܴ����Ѿ����ڵ���Ϣ����
//@@ 100120205	������Ϣʧ��		�����Ƿ����㹻���ڴ洴������
//@@ 100120206	������Ϣʧ��		ϵͳ������
//@@ 100120207	������Ϣʧ��		û�пɶ�ȡ����Ϣ
//@@ 100120208	������Ϣʧ��		�յ��ж��ź�
//@@ 100120209	������Ϣʧ��		������Ϣ�����Ƿ��Ѿ����ر�
//@@ 100120210	������Ϣʧ��		��Ϣ��������
//@@ 100120211	������Ϣʧ��		δ֪�Ĵ���
//@@ 100120301	������Ϣʧ��		����Ȩ������
//@@ 100120302	������Ϣʧ��		��Ϣ���в����ڣ�������Ϣ�����Ƿ�ɾ��
//@@ 100120303	������Ϣʧ��		����򿪶��еĲ������Ƿ���Ч
//@@ 100120304	������Ϣʧ��		���ܴ����Ѿ����ڵ���Ϣ����
//@@ 100120305	������Ϣʧ��		�����Ƿ����㹻���ڴ洴������
//@@ 100120306	������Ϣʧ��		ϵͳ������
//@@ 100120307	������Ϣʧ��		û�пɶ�ȡ����Ϣ
//@@ 100120308	������Ϣʧ��		�յ��ж��ź�
//@@ 100120309	������Ϣʧ��		������Ϣ�����Ƿ��Ѿ����ر�
//@@ 100120310	������Ϣʧ��		��Ϣ��������
//@@ 100120311	������Ϣʧ��		δ֪�Ĵ���
//@@ 100120401	�ر���Ϣ����ʧ��	����Ȩ������
//@@ 100120402	�ر���Ϣ����ʧ��	��Ϣ���в����ڣ�������Ϣ�����Ƿ�ɾ��
//@@ 100120403	�ر���Ϣ����ʧ��	����򿪶��еĲ������Ƿ���Ч
//@@ 100120404	�ر���Ϣ����ʧ��	���ܴ����Ѿ����ڵ���Ϣ����
//@@ 100120405	�ر���Ϣ����ʧ��	�����Ƿ����㹻���ڴ洴������
//@@ 100120406	�ر���Ϣ����ʧ��	ϵͳ������
//@@ 100120407	�ر���Ϣ����ʧ��	û�пɶ�ȡ����Ϣ
//@@ 100120408	�ر���Ϣ����ʧ��	�յ��ж��ź�
//@@ 100120409	�ر���Ϣ����ʧ��	������Ϣ�����Ƿ��Ѿ����ر�
//@@ 100120410	�ر���Ϣ����ʧ��	��Ϣ��������
//@@ 100120411	�ر���Ϣ����ʧ��	δ֪�Ĵ���
//@@ 100120501	ɾ����Ϣ����ʧ��	����Ȩ������
//@@ 100120502	ɾ����Ϣ����ʧ��	��Ϣ���в����ڣ�������Ϣ�����Ƿ�ɾ��
//@@ 100120503	ɾ����Ϣ����ʧ��	����򿪶��еĲ������Ƿ���Ч
//@@ 100120504	ɾ����Ϣ����ʧ��	���ܴ����Ѿ����ڵ���Ϣ����
//@@ 100120505	ɾ����Ϣ����ʧ��	�����Ƿ����㹻���ڴ洴������
//@@ 100120506	ɾ����Ϣ����ʧ��	ϵͳ������
//@@ 100120507	ɾ����Ϣ����ʧ��	û�пɶ�ȡ����Ϣ
//@@ 100120508	ɾ����Ϣ����ʧ��	�յ��ж��ź�
//@@ 100120509	ɾ����Ϣ����ʧ��	������Ϣ�����Ƿ��Ѿ����ر�
//@@ 100120510	ɾ����Ϣ����ʧ��	��Ϣ��������
//@@ 100120511	ɾ����Ϣ����ʧ��	δ֪�Ĵ���
//@@ 100120601	������Ϣ����ʧ��	����Ȩ������,���߶��д�С����
//@@ 100120602	������Ϣ����ʧ��	��Ϣ���в����ڣ�������Ϣ�����Ƿ�ɾ��
//@@ 100120603	������Ϣ����ʧ��	����򿪶��еĲ������Ƿ���Ч
//@@ 100120604	������Ϣ����ʧ��	���ܴ����Ѿ����ڵ���Ϣ����
//@@ 100120605	������Ϣ����ʧ��	�����Ƿ����㹻���ڴ洴������
//@@ 100120606	������Ϣ����ʧ��	ϵͳ������
//@@ 100120607	������Ϣ����ʧ��	û�пɶ�ȡ����Ϣ
//@@ 100120608	������Ϣ����ʧ��	�յ��ж��ź�
//@@ 100120609	������Ϣ����ʧ��	������Ϣ�����Ƿ��Ѿ����ر�
//@@ 100120610	������Ϣ����ʧ��	��Ϣ��������
//@@ 100120611	������Ϣ����ʧ��	δ֪�Ĵ���

#define MBC_Precision			100130000
#define MBC_Queue			100140000
//@@ 100140001	��������ʧ��		�����Ƿ����㹻���ڴ�
#define MBC_Semaphore			100150000
#define MBC_SimpleMatrix		100160000
//@@ 100160001	��������ʧ��		�����Ƿ����㹻���ڴ�
#define MBC_SimpleSHM			100170000
//@@ 100170001	�򿪹����ڴ�ʧ��	ϵͳ����ʧ�ܣ���ο��ӱ���(shmget)
//@@ 100170002	ɾ�������ڴ�ʧ��	ϵͳ����ʧ�ܣ���ο��ӱ���(shmctl)
//@@ 100170003	��������ʧ��		ϵͳ����ʧ�ܣ���ο��ӱ���(ftok)
//@@ 100170004	�򿪹����ڴ�ʧ��	�����ڴ治���ڣ������Ƿ�ɾ��
//@@ 100170005	���������ڴ�ʧ��	ϵͳ����ʧ�ܣ���ο��ӱ���(shmat)
//@@ 100170006	��ȡ�����ڴ���Ϣʧ��	ϵͳ����ʧ�ܣ���ο��ӱ���(shmctl)
#define MBC_SimpleFile          100171000
//@@ 100171001	���ļ�ӳ��ʧ��	    ϵͳ����ʧ��
//@@ 100171002	ɾ���ļ�ʧ��	        ϵͳ����ʧ��
//@@ 100171003	��������ʧ��		    ϵͳ����ʧ��
//@@ 100171004	���ļ�ʧ��	        �ļ������ڣ������Ƿ�ɾ��
//@@ 100171005	�����ļ�ӳ��ʧ��	    ϵͳ����ʧ��
//@@ 100171006	��ȡӳ���ļ���Ϣʧ��	ϵͳ����ʧ��
#define MBC_Socket			100180000
#define MBC_Log				100190000
//@@ 100190000	����־�ļ�ʧ��	����logĿ¼�Ƿ����,Ŀ¼�Ƿ��д
#define MBC_Stack			100200000
//@@ 100200001	��������ʧ��		�����Ƿ����㹻���ڴ�

//#
//# MBC_PUBLIC
//#
#define MBC_PUBLIC			110000000
#define MBC_AcctItemMgr			110010000
//@@ 110010001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 110010002	�����ڴ�ʧ��	��������������
#define MBC_AssignStatement		110020000
#define MBC_BillConditionMgr		110030000
#define MBC_BillOperationMgr		110040000
#define MBC_BillingCycle		110050000
#define MBC_BillingCycleMgr		110060000
//@@ 110060001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
#define MBC_EventSection		110070000
//@@ 110070001	�����ڴ�ʧ��	�����CommonMacro.h�е�MAX_EVENT_SECTION_NUM����
//@@ 110070002	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 110070003	������������ʧ��	����BILLING_CYCLE�����Ƿ���ȷ
#define MBC_EventType			110080000
#define MBC_EventTypeMgr		110090000
//@@ 110090001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 110090002	�����ڴ�ʧ��	���������ݣ���������������
#define MBC_ExtendFunctions		110100000
#define MBC_IDParser			110110000
//@@ 110110010	δ֪��ID	������IDParser::getValue�е�ID
#define MBC_LifeCycle			110120000
#define MBC_LifeCycleMgr		110130000
//@@ 110130001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 110130002	�����ڴ�ʧ��	���������ݣ���������������
#define MBC_LogicStatement		110140000
#define MBC_OrgMgr			110150000
//@@ 110150001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 110150002	�����ڴ�ʧ��	���������ݣ���������������
#define MBC_SHMAccess			110160000
//@@ 110160001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 110160002	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
#define MBC_SHMData			110170000
//@@ 110170001	�����ڴ�ṹ����	���鴴�����������ʹ�õĳ����Ƿ�ͬ���汾
//@@ 110170002	�������ù����ڴ�	�ڴ浥���ṹ��̫С�����ʺϰ�������
//@@ 110170003	�������ù����ڴ�	�ڴ浥���ṹ��̫С�����ʺϰ�������
//@@ 110170004	���ù����ڴ�	�ڴ治����
#define MBC_SHMIntHashIndex		110180000
//@@ 110180001	�����ڴ�ʧ��	����Ĺ����ڴ��Ѿ�ʹ���꣬��������������
#define MBC_SHMStringTreeIndex		110190000
//@@ 110190001	�����ڴ�ʧ��	����Ĺ����ڴ��Ѿ�ʹ���꣬�����ʵʱ���£��������������ϣ������ȫ�����أ������SHMStringTreeIndex::create����ʱ�Ĳ���
//@@ 110190002	�����ڴ�ʧ��	����Ĺ����ڴ��Ѿ�ʹ���꣬��������������
#define MBC_SHMLruData  110191000
//@@ 110191001	�����ڴ�ṹ����	���鴴�����������ʹ�õĳ����Ƿ�ͬ���汾
//@@ 110191002	�����±�Խ��	�������ݷ����Ƿ���ȷ
//@@ 110191003	��ͼ�޸�����session�����޸ĵ�����	�����´�ͷ����
//@@ 110191004	�ع��ռ䲻��	���޸ĺ����ݾ���commit�����lrutofiles�Ƿ�չ�
//@@ 110191005	�����ڴ涼�ڽ����޸ģ�û�пɹ������Ŀռ�	���޸ĺ����ݾ���commit�����lrutofiles�Ƿ�չ�
//@@ 110191006	���ʵ��±��Ѿ���ɾ��	�벻Ҫ���Ѿ�ɾ�����������ٲ���
//@@ 110191007	��Ӧ�Ĵ����ļ����ܴ�	��������ļ�Ȩ������
//@@ 110191008	��Ӧ�Ĵ����ļ�������	�������������
//@@ 110191009	��Ӧ�Ĵ����ļ�д�����ݳ���	��������ļ�Ȩ�����úʹ����Ƿ����㹻�Ŀռ�
//@@ 110191010	���ܲ�������session	�����´�ͷ����
//@@ 110191011	session����	���´�ͷ����
//@@ 110191012	�ڴ治��	��ֹͣ����ռ�ڴ�޴�ĳ���
#define MBC_SHMMixIndex		110192000
//@@ 110192001	�����ڴ�ʧ��	����Ĺ����ڴ��Ѿ�ʹ���꣬�����ʵʱ���£��������������ϣ������ȫ�����أ������SHMMixIndex::create����ʱ�Ĳ���
//@@ 110192002	�����ڴ�ʧ��	����Ĺ����ڴ��Ѿ�ʹ���꣬��������������
//@@ 110192003	��������ʧ��	�����SHMMixIndex::create

#define MBC_Statement			110200000
//@@ 110200001  �����ڴ�ʧ��    ���������Ƿ����㹻���ڴ�

#define MBC_StdEvent			110210000
#define MBC_TransactionMgr		110220000
#define MBC_ProdOfferAggr       110221000
//@@ 110220001	���빲���ڴ�(�¼������ۻ�)ʧ��	�����Ƿ����㹻���ڴ�
//@@ 110220002	���빲���ڴ�(��Ŀ�����ۻ�)ʧ��	�����Ƿ����㹻���ڴ�
//@@ 110220003	���õ��¼�������ʶ�ڳ�����δ����	����������û���ϵ��س���Ա
//@@ 110220004	�����ڴ�(�¼������ۻ�)������	���������¼������ۻ����س���
//@@ 110220005	�����ڴ�(��Ŀ�����ۻ�)������	����������Ŀ�����ۻ����س���
//@@ 110220006	�����ڴ�(�¼������ۻ�)������	����������
//@@ 110220007	�����ڴ�(��Ŀ�����ۻ�)������	����������
//@@ 110220008	�¼������ۻ����������	�뽫EVENT_AGGR_INDEX_COUNT��ֵ�Ĵ�
//@@ 110220009	�¼������ۻ����������	�뽫EVENT_ADD_AGGR_INDEX_COUNT��ֵ�Ĵ�
//@@ 110220010	��Ŀ�����ۻ����������	�뽫ACCTITEM_AGGR_INDEX_COUNT��ֵ�Ĵ�
//@@ 110220011	��Ŀ�����ۻ����������	�뽫ACCTITEM_ADD_AGGR_INDEX_COUNT��ֵ�Ĵ�
//@@ 110220012	�¼������ۻ����ݶ����	�뽫EVENT_AGGR_DATA_COUNT��ֵ�Ĵ�
//@@ 110220013	��Ŀ�����ۻ����ݶ����	�뽫ACCTITEM_AGGR_DATA_COUNT��ֵ�Ĵ�
//@@ 110220014	�¼������ۻ����������	�뽫ADD_AGGR_MAX_TABLE_NUM��ֵ�Ĵ�
//@@ 110220015	��Ŀ�����ۻ����������	�뽫ADD_AGGR_MAX_TABLE_NUM��ֵ�Ĵ�
//@@ 110220016	�¼������ۻ����ݶ����	�뽫EVENT_ADD_AGGR_DATA_COUNT��ֵ�Ĵ�
//@@ 110220017	��Ŀ�����ۻ����ݶ����	�뽫ACCTITEM_ADD_AGGR_DATA_COUNT��ֵ�Ĵ�
//@@ 110220018	һ���¼���¼����̫����¼��ۻ���¼	�뽫MAX_EVENT_AGGR_TYPE_NUM_OfPerEvent��ֵ�Ĵ�
//@@ 110220019	���빲���ڴ�(�����ļ���)ʧ��	�����Ƿ����㹻���ڴ�
//@@ 110220020	���빲���ڴ�(�¼���⻺��)ʧ��	�����Ƿ����㹻���ڴ�
//@@ 110220021	�¼���չ���Ա��������	�뽫EVENT_MAX_EXTEND_ATTR_NUM��ֵ�Ĵ�
//@@ 110220022	�¼��ۻ����˼�¼���������	�뽫MAX_EVENT_AGGR_TYPE_NUM_OfPerEvent��ֵ�Ĵ�
//@@ 110220023	��Ŀ�ۻ����˼�¼���������	�뽫MAX_ACCT_ITEM_PER_EVENT��ֵ�Ĵ�
//@@ 110220024	���Ҳ����¼��ۻ���ı���	������Ϣ��鿴��־�ļ�
//@@ 110220025	���Ҳ�����Ŀ�ۻ���ı���	������Ϣ��鿴��־�ļ�
//@@ 110220026	��չ�¼������������		�뽫MAX_EXTEVENT_TABLE_NUM��MAX_END_FILE��ֵ�Ĵ�
//@@ 110220027	�����¼������������		�뽫MAX_STDEVENT_TABLE_NUM��MAX_END_FILE��ֵ�Ĵ�
//@@ 110220028	�ύ��¼���Ѵﵽ		�����ύ�����ٲ����¼���������
//@@ 110220030	���Ҳ����¼��������		������Ϣ��鿴��־�ļ�
//@@ 110220031	���Ҳ����¼���ı���		������Ϣ��鿴��־�ļ�
//@@ 110220032	�����¼��������		�뽫MAX_REDOEVENT_TABLE_NUM��ֵ�Ĵ�
//@@ 110220033	�����¼��������		�뽫MAX_NOOWNEREVENT_TABLE_NUM��ֵ�Ĵ�
//@@ 110220034	�����¼��������		�뽫MAX_STDEVENT_TABLE_NUM��ֵ�Ĵ�
//@@ 110220035	�¼���չ���Ա������		�뽫MAX_EXTEVENT_TABLE_NUM��ֵ�Ĵ�
//@@ 110220036	��������ļ��������		�뽫MAX_END_FILE��ֵ�Ĵ�


#define MBC_UserInfo			110230000
//@@ 110230001	�����ڴ�ʧ��		�����Ƿ����㹻���ڴ�
#define MBC_UserInfoCtl			110240000
//@@ 110240001	�����ڴ�ʧ��		�ͻ��������ռ䲻������������������
//@@ 110240002	�����ڴ�ʧ��		�ͻ�Ⱥ�������ռ䲻������������������
//@@ 110240003 	�����ڴ�ʧ��		�ͻ�������Ϣ�������ռ䲻������������������
//@@ 110240004	�����ڴ�ʧ��		��Ʒ��Ϣ�������ռ䲻������������������
//@@ 110240005	�����ڴ�ʧ��		��Ʒ��Ϣ�������ռ䲻������������������
//@@ 110240006	�����ڴ�ʧ��		����Ʒʵ���������ռ䲻������������������
//@@ 110240007	�����ڴ�ʧ��		����Ʒʵ��������Ϣ�������ռ䲻������������������
//@@ 110240008	�����ڴ�ʧ��		����Ʒʵ��״̬��Ϣ�������ռ䲻������������������
//@@ 110240009	�����ڴ�ʧ��		����Ʒʵ����ʶ��Ϣ�������ռ䲻������������������
//@@ 110240010	�����ڴ�ʧ��		�û�Ⱥ��Ϣ�������ռ䲻������������������
//@@ 110240011	�����ڴ�ʧ��		������Ʒʵ����Ϣ�������ռ䲻������������������
//@@ 110240012	�����ڴ�ʧ��		��������Ϣ�������ռ䲻������������������
//@@ 110240013	�����ڴ�ʧ��		��Ʒ��ϸʵ����Ϣ�������ռ䲻������������������
//@@ 110240014	�����ڴ�ʧ��		�ͻ��������ռ䲻������������������
//@@ 110240015  ���¿ͻ���Ϣʧ��	û���ҵ���Ӧ�Ŀͻ����ݣ��������ݵ���ȷ��
//@@ 110240016  ���¿ͻ�Ⱥ��Ϣʧ��	û���ҵ���Ӧ�Ŀͻ����ݣ��������ݵ���ȷ��
//@@ 110240017	�����ڴ�ʧ��		�ͻ�Ⱥ�������ռ䲻������������������
//@@ 110240018  ���¿ͻ�������Ϣʧ��	û���ҵ���Ӧ�Ŀͻ����ݣ��������ݵ���ȷ��
//@@ 110240019 	�����ڴ�ʧ��		�ͻ�������Ϣ�������ռ䲻������������������
//@@ 110240020  ���¿ͻ�������Ϣʧ��	û���ҵ���Ӧ�Ŀͻ�������Ϣ���ݣ��������ݵ���ȷ��
//@@ 110240021	�����ڴ�ʧ��		��Ʒ��Ϣ�������ռ䲻������������������
//@@ 110240022  ���²�Ʒ��Ϣʧ��	û���ҵ���Ӧ�Ĳ�Ʒ���ݣ��������ݵ���ȷ��
//@@ 110240023	�����ڴ�ʧ��		��Ʒ��Ϣ�������ռ䲻������������������
//@@ 110240024  ������Ʒ��Ϣʧ��	û���ҵ���Ӧ��Ʒ���ݣ��������ݵ���ȷ��
//@@ 110240025	�����ڴ�ʧ��		����Ʒʵ���������ռ䲻������������������
//@@ 110240026  ��������Ʒʵ����Ϣʧ��	û���ҵ���Ӧ����Ʒʵ�����ݣ��������ݵ���ȷ��
//@@ 110240027  ��������Ʒʵ��������Ϣʧ��	û���ҵ���Ӧ����Ʒʵ�����ݣ��������ݵ���ȷ��
//@@ 110240028	�����ڴ�ʧ��		����Ʒʵ��������Ϣ�������ռ䲻������������������
//@@ 110240029  ��������Ʒʵ��״̬��Ϣʧ��	û���ҵ���Ӧ����Ʒʵ�����ݣ��������ݵ���ȷ��
//@@ 110240030	�����ڴ�ʧ��		����Ʒʵ��״̬��Ϣ�������ռ䲻������������������
//@@ 110240031	�����ڴ�ʧ��		����Ʒʵ����ʶ��Ϣ�������ռ䲻������������������
//@@ 110240032  �����û�Ⱥ��Ϣʧ��	û���ҵ���Ӧ����Ʒʵ�����ݣ��������ݵ���ȷ��
//@@ 110240033	�����ڴ�ʧ��		�û�Ⱥ��Ϣ�������ռ䲻������������������
//@@ 110240034  �����û�Ⱥ��Ϣʧ��	û���ҵ���Ӧ�û�Ⱥ���ݣ��������ݵ���ȷ��
//@@ 110240035  ���¸�����Ʒʵ����Ϣʧ��	û���ҵ���Ӧ����Ʒʵ�����ݣ��������ݵ���ȷ��
//@@ 110240036	�����ڴ�ʧ��		������Ʒʵ����Ϣ�������ռ䲻������������������
//@@ 110240037  ���¸�����Ʒʵ����Ϣʧ��	û���ҵ���Ӧ������Ʒʵ�����ݣ��������ݵ���ȷ��
//@@ 110240038  ������������Ϣʧ��	û���ҵ���Ӧ������Ʒʵ�����ݣ��������ݵ���ȷ��
//@@ 110240039	�����ڴ�ʧ��		��������Ϣ�������ռ䲻������������������
//@@ 110240040  ������������Ϣʧ��	û���ҵ���Ӧ���ƹ�ϵ���ݣ��������ݵ���ȷ��
//@@ 110240041	�����ڴ�ʧ��		��Ʒ��ϸʵ����Ϣ�������ռ䲻������������������
//@@ 110240042  ������Ʒ��ϸʵ��״̬��Ϣʧ��	û���ҵ���Ӧ����Ʒʵ�����ݣ��������ݵ���ȷ��
//@@ 110240043  ������Ʒ��ϸʵ��״̬��Ϣʧ��	û���ҵ���Ӧ��Ʒʵ�����ݣ��������ݵ���ȷ��
//@@ 110240044  ������Ʒ��ϸʵ��״̬��Ϣʧ��	û���ҵ���Ӧ��Ʒ��ϸʵ�����ݣ��������ݵ���ȷ��
//@@ 110240045  �����ڴ�ʧ��		������Ʒʵ��������Ϣ�������ռ䲻������������������
//@@ 110240046  ���¸�����Ʒʵ������ʧ�� û���ҵ���Ӧ�ĸ�����Ʒʵ�������ݣ��������ݵ���ȡ��
//@@ 110240047  �����ڴ�ʧ��		������Ʒʵ��������Ϣ�������ռ䲻������������������
//@@ 110240048  �����ڴ�ʧ��		��Ʒʵ����Ϣ�������ռ䲻�������޸Ĳ�����������������
//@@ 110240049  �����ڴ�ʧ��		��Ʒʵ����ϸ��Ϣ�������ռ䲻�������޸Ĳ�����������������
//@@ 110240050  ������Ʒʵ��ʧ��	��Ʒʵ����Ϣ�е���Ʒ��ʾ����Ʒ����δ���壬���޸ĺ�������������
//@@ 110240051  �����ڴ�ʧ��		��Ʒʵ��������Ϣ�������ռ䲻�������޸Ĳ�����������������
//@@ 110240053  �����ڴ�ʧ��		������Ʒʵ����Ϣʱ�������ռ䲻�������޸Ĳ�����������������
//@@ 110240054	ɾ���ڴ浵��ʧ��	ȷ���Ѿ�ֹͣ��ص����н���
//@@ 110240100  ��ȡ����ʧ��		ϵͳ������δ�����û��������ڴ��С
#ifdef GROUP_CONTROL
//@@ 110240104	�����ڴ�ʧ��		Ⱥ��Ϣ�����������ռ䲻������������������
//@@ 110240105	�����ڴ�ʧ��		Ⱥ��Ա��Ϣ�������ռ䲻������������������
//@@ 110240106	�����ڴ�ʧ��		������Ⱥ��ϵ��Ϣ�������ռ䲻������������������
//@@ 110240107	����Ⱥ��Ա��Ϣʧ��	û���ҵ���Ӧ��Ⱥ������Ϣ���������ݵ���ȷ��
//@@ 110240108	����Ⱥ��Ա��Ϣʧ��	�ҵ��˶�Ӧ��Ⱥ������Ϣ���ǲ�ƥ�䣬�������ݵ���ȷ��
//@@ 110240109	���º���Ⱥ��ϵ��Ϣʧ��	û���ҵ���Ӧ�ĺ���Ⱥ��ϵ��Ϣ���������ݵ���ȷ��
//@@ 110240110	���º���Ⱥ��ϵ��Ϣʧ��	�ҵ��˶�Ӧ�ĺ���Ⱥ��ϵ��Ϣ���ǲ�ƥ�䣬�������ݵ���ȷ��
#endif
#ifdef IMMEDIATE_VALID
//@@ 110240111	�����ڴ�ʧ��		�û������ײ�ʵʱ��Ч���������ռ䲻������������������
#endif
//@@ 110240120	��ȡ����ʧ��		b_param_define��δ����param_code:IMSI_MDN_RELATION_NUM
//@@ 110240121	�����ڴ�ʧ��		IMSI��MDNӳ���ϵ�������ռ䲻������������������
//@@ 110240122	����IMSI��MDN��ӳ���ϵʧ��		û���ҵ���Ӧrelation_id�����ݣ�����������ȷ��	
//@@ 110240123	����IMSI��MDN��ӳ���ϵʧ��		û���ҵ���ӦIMSI�����ݣ�����������ȷ��	
//@@ 110240124  ������Ʒʵ����ϸ����    ��Ʒʵ����ϸ�����Ѿ�����
//@@ 110240126  ��������Ʒʵ����Ϣʧ��	����Ʒʵ����Ϣ�Ѵ��ڣ��������ݵ���ȷ��
//@@ 110240127  �������������Ʒʵ������Ʒ��Ϣʧ��	����Ʒʵ����Ϣ�����ڣ��������ݵ���ȷ��
//@@ 110240128  ���¸�����Ʒʵ��������Ϣʧ��		Ҫ���µĸ�����Ʒʵ��������Ϣ�����ڣ�����������ȷ��
//@@ 110240129  �����ʻ���Ϣʧ��		�ʻ���Ϣ���ڴ����Ѵ��ڣ�����������ȷ��

#define MBC_HighFeeAggr     980000000
//@@ 980000001	�����ڴ�ʧ��	�����Ƿ����㹻����
#define MBC_VpnInfoCtl			110245000
//@@ 110245001	�����ڴ�ʧ��			VPN�������ռ䲻������������������
//@@ 110245002	�����ڴ�ʧ��			VPN��Ա�������ռ䲻������������������
//@@ 110245003 	�����ڴ�ʧ��			VPN��VPN��ϵ�������ռ䲻������������������
//@@ 110245004	�����ڴ�ʧ��			VPN��������������ռ䲻������������������
//@@ 110245005	�����������ڴ�ʧ��		VPN�������ռ䲻������������������
//@@ 110245006	�����������ڴ�ʧ��		VPN��Ա�������ռ䲻������������������
//@@ 110245007 	�����������ڴ�ʧ��		VPN��VPN��ϵ�������ռ䲻������������������
//@@ 110245008	�����������ڴ�ʧ��		VPN��������������ռ䲻������������������
//@@ 110245009	����ʧ��				VPN��Ϣ�����ڣ���������������
//@@ 110245010	����ʧ��				VPN��Ա��Ϣ�����ڣ���������������
//@@ 110245011	����ʧ��				VPN��VPN��ϵ���ݲ����ڣ���������������
//@@ 110245012	����ʧ��				VPN�������Ϣ�����ڣ���������������
//@@ 110245013	ɾ��VPN�ڴ浵��ʧ��	    ȷ���Ѿ�ֹͣ��ص����н���


//@@ 110245100  ��ȡ����ʧ��			ϵͳ������δ����VPN�������ڴ��С

#define MBC_IvpnInfoCtl			110245200
//@@ 110245201	�����ڴ�ʧ��			ServAcctEx�������ռ䲻������������������
//@@ 110245202	�����ڴ�ʧ��			AcctAttr�������ռ䲻������������������
//@@ 110245213	ɾ��IVPN�ڴ�ʧ��	    ȷ���Ѿ�ֹͣ��ص����н���
//@@ 110245221	���´�����ϵʧ��	    ������ϵ���е�SERV_ID���û�����δ����
//@@ 110245222	���´�����ϵʧ��	    ������ϵ���е�ACCT_ID���ʻ�����δ����
//@@ 110245223	���´�����ϵʧ��	    ������ϵ���е������ڹ����ڴ���δ����,�޷�����
//@@ 110245224	���������ϵʧ��	    ������ϵ���е������ڹ����ڴ����Ѷ���,�޷�����
//@@ 110245231	�����ʻ���չ����ʧ��	A_ACCT_ATTR���е�ACCT_ID���ʻ�����δ����
//@@ 110245232	�����ʻ���չ����ʧ��	A_ACCT_ATTR���е������ڹ����ڴ���δ����,�޷�����
//@@ 110245233	�����ʻ���չ����ʧ��	A_ACCT_ATTR���е������ڹ����ڴ����Ѷ���,�޷�����
//@@ 110245300  ��ȡ����ʧ��			ϵͳ������δ����IVPN���Ŵ������ڴ��С

#define MBC_ScaleInfoCtl			110245400
//@@ 110245401	�����ڴ�ʧ��		N_Route�������ռ䲻������������������
//@@ 110245402	�����ڴ�ʧ��		NetScaleInfo�������ռ䲻������������������
//@@ 110245410  ��ȡ����ʧ��		ϵͳ������δ����̯����Ӫ���ڴ��С
//@@ 110245411  ��ȡ����ʧ��		ϵͳ������δ����ר��̯�ֹ����ڴ��С
//@@ 110245420	ɾ��SCALE�ڴ�ʧ��	ȷ���Ѿ�ֹͣ��ص����н���
//@@ 110245430  ������������		������Ӫ�̵�������û����������
//@@ 110245431  ������������		������Ӫ�̵�������û����������
//@@ 110245432  ��ȡ����ʧ��		������Ӫ�̲���û����������
//@@ 110245433  ������������		������Ӫ�̲�����������
//@@ 110245440  ������������		ר��һ�ηѵ�Ŀ¼û����������
//@@ 110245451  ��ȡseq����ʧ��     �������ݿ�����scale_item_seq
//@@ 110245452  ��ȡseq����ʧ��     �������ݿ�����acct_item_id_big_seq
#define MBC_SpecialTariffMgr			110246000
//@@ 110246001	�����ڴ�ʧ��			SP�����ʷѼ����ڴ�ռ䲻���������¼���
//@@ 110246002	����δ����				SP�����ʷ���Ҫ������ID�б�δ���ã������ú���������
//@@ 110246003	�������ò�����			SP�����ʷ���Ҫ������IDδ���������������ú���������
//@@ 110246004	�������ò�����			SP������Ĵ��ֵ������IDδ���ã������ú���������

#define MBC_UserInfoReader		110250000
//@@ 110250001	���ӹ����ڴ�ʧ��	���鹲���ڴ��Ƿ�ɾ����������������
//@@ 110250002  �����ڴ�ʧ��		�����Ƿ����㹻���ڴ洴������
#define MBC_Value			110260000
#define MBC_ZoneMgr			110270000
//@@ 110270001	�����ڴ�ʧ��		�����Ƿ����㹻���ڴ洴������
//@@ 110270002	�����ڴ�ʧ��		���������ݣ���������������
//@@ 110270003	�������ݳ���		�������ݵ���ȷ��
#define MBC_EventSectionUnioner		110280000
//@@ 100280001	��������ʧ��		�����CommonMacro.h�е�MAX_CHARGE_NUM����
#define MBC_InstTableListMgr		110290000
//@@ 110290001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
#define MBC_BalanceMgr			110300000
//@@ 110300001	�޷����ӹ����ڴ�(���)	��������������س���
//@@ 110300002	�����ڴ�(���)������	��������������س���
#define MBC_ReferenceObjectMgr		110310000

#define MBC_BillingCycleAdmin		110320000
#define MBC_BillingCycleBase		110330000
#define MBC_E8UserCtl				110340000
//@@ 110340001	�����ڴ�ʧ��				e8�����������ݿռ䲻��
//@@ 110340002	�����ڴ�ʧ��				e8�������������ռ䲻��
//@@ 110340003	ɾ���ڴ�ʧ��				ȷ����ֹͣ���������ڴ����ؽ���
//@@ 110340004	�����ڴ�ʧ��				ȷ������ڴ��Ѿ�����
//@@ 110340050	��ȡ����ʧ��				ϵͳ������δ����e8�������ڴ��С

#define MBC_DataEventDiv			110350000
//@@ 110350001	�����ڴ�ʧ��				���ݻ����ڴ����ݿռ䲻��
//@@ 110350002	�����ڴ�ʧ��				e8�������������ռ䲻��
//@@ 110350003	ɾ���ڴ�ʧ��				ȷ����ֹͣ���������ڴ����ؽ���
//@@ 110350004	�����ڴ�ʧ��				ȷ������ڴ��Ѿ�����
//@@ 110350040	��ȡ��������ʧ��			�뽫��ػ�������export
//@@ 110350041	��ȡ���ʧ��				����b_param_define�����ý��̶�Ӧ�����1��2��3��
//@@ 110350050	��ȡ����ʧ��				ϵͳ������δ����e8�������ڴ��С

#define MBC_initserver  340010000
//@@ 340010001	initserverע��ʧ��			initserverע��ʧ��
//@@ 340010002	initserver��ȡ��ʼ����ֵʧ��	initserver��ȡ��ʼ����ֵʧ��
//@@ 340010003	initserverԤԼʧ��			initserverԤԼʧ��
//
#define MBC_Log_Group		100191000
//@@ 100191001	��־�������ڴ�ʧ��				����ϵͳ��Դ
//@@ 100191002	��־�鹲���ڴ洴��ʧ��			ϵͳ����ʧ�ܣ���ο��ӱ���(shmat)
//@@ 100191003	��־�鹲���ڴ�ɾ��ʧ��			ϵͳ����ʧ�ܣ���ο��ӱ���(shmat)	
//@@ 100191004	��־���ȡ�����ڴ���Ϣʧ��		�����Ƿ��Ѵ���
//@@ 100191005	��־���ʼ��ʧ��				������־������
//@@ 100191006	��־���л�ʧ��					���鵱ǰ��־��״̬����Ϣ
//@@ 100191007	��־�����־�ļ�ʧ��			�����ļ�Ŀ¼�Ƿ���ڣ�Ŀ¼�Ƿ��д
//@@ 100191008	��־���ȡ�ļ�״̬����			�����ļ�״̬
//@@ 100191009	��־����������������			��ο�����˵��
//@@ 100191010	��־���������ɾ����Ա�ļ�ʧ��	��鿴�ļ�������־��״̬��Ϣ
//@@ 100191011	��־���л���ʶ������Χ			������޷�ʶ�����־���ʶ
//@@ 100191012	��־���ȡ��ǰʹ����ʧ��		���鵱ǰ��־��״̬��
#define	MBC_Server							320030000
//@@ 320030001	�����ڴ�ʧ��	����û��ָ�������ڴ�ļ�
#define MBC_ExtServCtl				110360000
//@@ 110360001	�����ڴ�ʧ��				ext_serv�������ݿռ䲻��
//@@ 110360002	�����ڴ�ʧ��				ext_serv���������ռ䲻��
//@@ 110360003	ɾ���ڴ�ʧ��				ȷ����ֹͣ���������ڴ����ؽ���
//@@ 110360004	�����ڴ�ʧ��				ȷ������ڴ��Ѿ�����
//@@ 110360050	��ȡ����ʧ��				ϵͳ������δ����ext_serv���ڴ��С

//#
//# MBC_LIBOCI
//#
#define MBC_LIBOCI			120000000
#define MBC_TOCIDirPath			120010000
//#define MBC_exception			120020000
#define MBC_TOCIQuery			120030000
#define MBC_TBatchInsert		120040000

//#
//# MBC_CPARSER
//#
#define MBC_CPARSER			130000000
#define MBC_TScriptParser		130010000
#define MBC_wwInterpreter		130020000
#define MBC_wwLex			130030000
#define MBC_wwParser			130040000
#define MBC_exception			130050000

//#
//# MBC_FRAMEWORK
//#
#define MBC_FRAMEWORK			140000000
#define MBC_DistributeRule		140010000
#define MBC_Environment			140020000
//@@ 140020001	��������ʧ��		�����Ƿ����㹻�ڴ�
#define MBC_EventReceiver		140030000
//@@ 140030001	��������ʧ��		�����Ƿ����㹻�ڴ�
#define MBC_EventSender			140040000
//@@ 140040001	��������ʧ��		�����Ƿ����㹻�ڴ�
#define MBC_Process			140050000
//@@ 140050000	Ӧ�ó����յ��˳��ź��쳣�˳�		���SIGQUIT,SIGTERM,SIGINT�źŵ���Դ
//@@ 140050001	Ӧ�ó����յ�ǰ̨����ָ�������˳�	---
//@@ 140050002	ȡ������������PROCESS_ID		�����û�������PROCESS_ID,��:export PROCESS_ID=123
//@@ 140050003	ȡ����APP_ID,BILLFLOW_ID		����wf_process��,�Ƿ���ڸ�process_id
//@@ 140050004	����SIGQUIT�ź�ʧ��			---
//@@ 140050005	����SIGTERM�ź�ʧ��			---
//@@ 140050006	����SIGINT�ź�ʧ��			---
//@@ 140050007	����SIGUSR1�ź�ʧ��			---
//@@ 140050008	��־�ļ���ʧ��			���TIBS_HOME/log/ Ŀ¼�Ƿ����,�Ƿ��д

//#
//# MBC_MIDWARE_ACCESS
//#
#define MBC_MIDWARE_ACCESS		150000000

//#
//# MBC_MEMORY_MANAGER
//#
#define MBC_MEMORY_MANAGER		160000000

//#
//# MBC_TERMINAL_CONTROL
//#
#define MBC_TERMINAL_CONTROL		170000000
#define MBC_control			170010000
#define MBC_edit			170020000
#define MBC_button			170030000
#define MBC_listbox			170040000
#define MBC_readinfo			170050000

//#
//# MBC_APP_PRICING
//#
#define MBC_APP_PRICING			180000000
#ifdef GROUP_CONTROL
//@@ 180000001  ����δ����	�����ֻ���Ӫ��ʹ�õ��¼�����IDδ���ã�������
//@@ 180000002  ����δ����	�����ֻ���Ӫ��ʹ�õ��¼�����ֵδ���ã�������
#endif
#define MBC_PricingMeasure		180010000
#define MBC_Tariff			180020000
#define MBC_Discount			180030000
//@@ 180030010  ��Ŀ������û�г�Ա	�����Ŀ�����������
#define MBC_PricingInfoMgr		180040000
//@@ 140050001	�ڴ�����ʧ�� 			ִ�� ulimit -a �������̵��ڴ�����
//@@ 180040003	���۲���û���ҵ�  		����Լ��,������LOG_LEVEL>100, �鿴����ID
//@@ 180040004	ȡ�������ʧ�� 			��� HashList ģ��
//@@ 180040017	ȡ���۶���ʧ�� 			��� HashList ģ��
#define MBC_PricingInfo			180050000
#define MBC_BillObject			180060000
#define MBC_PricingCombine		180070000
#define MBC_Pricing			180080000
//@@ 180080002	�ڴ�����ʧ��			ִ�� ulimit -a �������̵��ڴ�����

//#
//# MBC_APP_MONITOR
//#
#define MBC_APP_MONITOR			190000000
#define MBC_monitor			190010000
#define MBC_ipc_op			190020000
#define MBC_login_f			190030000
#define MBC_login_j			190040000
#define MBC_password_f			190050000
#define MBC_password_j			190060000
#define MBC_mainmenu_j			190070000
#define MBC_mainmenu_f			190080000
#define MBC_process_f			190090000
#define MBC_process_j			190100000
#define MBC_ticket_f			190110000
#define MBC_ticket_j			190120000
#define MBC_mnt				190130000
#define MBC_mnt_interface		190140000
#define MBC_socketapi			190150000

//#
//# MBC_APP_CYCLEFEE
//#
#define MBC_APP_CYCLEFEE		200000000
//@@ 200000001	��������,������1��2	�����������
//@@ 200000002	�����������	cyclefee 2 yyyymmdd yyyymmdd,��ʼ�����Ǳ����䣬���������ǿ�����
//@@ 200000003	�����쳣	����û�����
//@@ 200000004	���ڷǷ�	������ڵ���ȷ�Ժ������ʽ��yyyymmdd��
#define MBC_CycleFeeEvent		200010000
#define MBC_CycleFeeEventCreaterMgr	200020000
//@@ 200020001	���ɱ����̶�Ӧ���������ͺ����ڱ�ʶ֮���ӳ���ϵ����	����b_creator_process,���˽����Ƿ������˶�Ӧ����������
//@@ 200020002	���ɲ�Ʒ�������Է����¼�����֮���ӳ���ϵ����	����b_cycle_fee_event_type,���˽����Ƿ������˶�Ӧ����������
//@@ 200020003	��������Է����¼����ļ�������	�����ļ�·���Ƿ����,�ļ��Ƿ�����
//@@ 200020004	���������Է����¼�ʧ��	�����û�����
//@@ 200020005	�ύ�����Է����ļ�����	�����ļ��Ƿ����
#define MBC_CycleFeeEventMgr		200030000
#define MBC_CycleInfo			200040000
#define MBC_FileInfo			200050000

//#
//# MBC_APP_PREPROCESS
//#
#define MBC_PreProcess             210000000
//@@ 210000001	��ʼ��ʧ��	�����ʽ��׼�������Ƿ���������
//@@ 210000002  ���ļ�����Ϊ����״̬ʧ��  �������ݿ��Լ�b_event_file_list��
//@@ 210000003  ���ļ�����Ϊ����״̬ʧ��  �������ݿ��Լ�b_event_file_list��
//@@ 210000004  ���ļ�����Ϊ����״̬ʧ��  �������ݿ��Լ�b_event_file_list��
//@@ 210000005	�����ļ���������	�����Ƿ���ڴ��ļ�
//@@ 210000005	����ļ���������	�����Ƿ���ڴ��ļ�
//@@ 210000007	���Ҷ�Ӧģ�������ߴ��ļ������쳣	�����Ƿ���ڴ��ļ�,�Ƿ������˸��ֽ�������Ӧ�Ľ���ģ��

#define MBC_EventExtractor         210010000
//@@ 210010001	��ȡ��������Ϣ����	����b_switch_info���ű�,�Ƿ������˸��ֽ�������Ϣ
//@@ 210010002	��ȡ�ý�������Ӧ��ģ�����	�����Ƿ������˸��ֽ�������Ӧ��ģ��
//@@ 210010003	���ļ�����	�����ļ��б��еĴ��ļ��Ƿ����Ǵ���
//@@ 210010004	��ȡ�ļ��е�һ���¼���¼����	����b_object_define���ж����Ӧ�Ƿ���ȷ
//@@ 210010005	����һ���¼���¼�е��ֶγ���	�����ֶ���Ϣ���Ƿ���ֶε���Ϣ������ȷ
//@@ 210010006	�ֶ�ת������	����ÿ���ֶ���Ϣ�����ֶ�ת����Ϣ
//@@ 210010007	ȡ�ֶν������	����LocalConvert.cpp���ֶεĸ��Ի�ת���Ƿ�������
#define MBC_FileEventOwner         210020000
//@@ 210020001	�����ݿ���ȡ���������	�������ݿ��Ƿ��������,���b_object_define��,ȷ��ÿ��ӳ��(���������͸�ģ��֮���ӳ��)���Ѿ���ȷ���ö���
//@@ 210020002	�����ݿ���ȡÿ�������¼���¼�Ľ��������	�������ݿ��Ƿ��������,���b_map_parser_group��,ȷ�ϴ�ӳ���Ӧ�ĸ����¼���¼���Ѿ���ȷ����
//@@ 210020003	�����ݿ���ȡÿ�ּ�¼���͵��ֶν�����Ϣ����	�������ݿ��Ƿ��������,����ӳ���Ӧ���ֶα�,ȷ�ϸ����ֶεĽ�����Ϣ�Ѿ�������ȷ
//@@ 210020004	���ļ�����	������ļ��Ƿ���ʵ����
#define MBC_Block                  210030000
//@@ 210030001	˵��û��һ����־ƥ����,������	���������Ƿ���ȷ,���ֶ�����ͨ����־λ��ȷ������ʼλ�õ�,����ϵͳƥ�䲻���κα�־
#define MBC_EventBlock             210040000
#define MBC_ChildEventBlock        210050000
#define MBC_OneEvent               210060000
#define MBC_Field                  210070000
#define MBC_FieldCheck             210080000
#define MBC_ParamMgr			   210090000
#define MBC_PpConvertUtility       210100000
#define MBC_LocalConvert           210110000
#define MBC_AttrTransMgr           210120000
//@@ 210120001	�����ڴ����(AttrTransMgr::reloadRule() -- *ppFormat = new FormatGroup;)
//@@ 210120002	�����ڴ����(AttrTransMgr::reloadRule() -- *ppStep = new FormatStep();)
//@@ 210120003	�����ڴ����(AttrTransMgr::reloadRule() -- *ppRule = new FormatRule();)


#define MBC_EventFormat            210130000

#define MBC_FormatMethod           210140000
//@@ 210140001  �����ڴ����(new KeyTree<Trunk>)
//@@ 210140002	�����ڴ����(new KeyTree<TServiceInfo2>)
//@@ 210140003	�����ڴ����(new KeyTree<int>)
//@@ 210140004	�����ڴ����(new KeyTree<HLR>)
//@@ 210140005	�����ڴ����(loadCarrier()-new KeyTree<int>)
//@@ 210140006	�����ڴ����: Err when new TSwitchInfo[m_iSwitchInfoCnt] - FormatMethod::loadSwitchInfo()
//@@ 210140007	�����ڴ����: err when m_pSwitchInfoPosTree = new KeyTree<int>; - FormatMethod::loadSwitchInfo()
//@@ 210140008	�����ڴ����: err when new PNSegBilling[m_iPNSegBillingCnt];
//@@ 210140009	�����ڴ����: err when new TServiceInfo[m_iServiceInfoCnt];
//@@ 210140010	�����ڴ����: ERR when m_poOrgMgr = new OrgMgr; -- FormatMethod::getServiceInfo2
//@@ 210140011	�����ڴ����: err when m_poLocalHeadMgr = new LocalHeadMgr;
//@@ 210140012	
//@@ 210140013
//@@ 210140014
//@@ 210140015
//@@ 210140016
//@@ 210140017
//@@ 210140018
//@@ 210140019

#define MBC_LongEvtRule           210150000
//@@ 210150001	�����ڴ����(LongEventTypeRuleMgr::loadRule()-- pt = new LongEventTypeRule;)

#define MBC_LocalEvtRule          210160000
//@@ 210160001  �����ڴ����(LocalEventTypeRuleMgr::loadRule() -- m_poOrgMgr = new OrgMgr;)
//@@ 210160002	�����ڴ����(LocalEventTypeRuleMgr::loadRule() -- pt = new LocalEventTypeRule;)



//#
//# MBC_APP_PUTINDB
//#
#define MBC_APP_PUTINDB			220000000

//#
//# MBC_APP_WORKFLOW
//#
#define MBC_APP_WORKFLOW		230000000
#define MBC_guard			230010000
#define MBC_guard_main			230020000
#define MBC_guard_remote		230030000
#define MBC_public			230040000
#define MBC_server			230050000
#define MBC_server_client		230060000
#define MBC_server_monitor		230070000
#define MBC_workflow			230080000

//#
//# MBC_APP_STAT
//#
#define MBC_APP_STAT			240000000
#define MBC_BssOrgMgr			240010000 

//#
//# MBC_APP_DUPCHECKER
//#
#define MBC_APP_DUPCHECKER 240020000
//@@ 240020001		CheckDup.ini��FILE_SECSû������		�����ӦPROCESS_ID��FILE_SECS������
//@@ 240020002		CheckDup.ini��BLOCK_SIZEû������	�����ӦPROCESS_ID��BLOCK_SIZE������
//@@ 240020003		CheckDup.ini��BUFFER_SIZEû������	�����ӦPROCESS_ID��BUFFER_SIZE������
//@@ 240020004		OCI�쳣����												������һ��
//@@ 240020005		���ô���													��������B_DUP_DATA_PATH��B_DUP_CHECK_CFG��û�����ü�¼
//@@ 240020006		���ô���													�����ص������жϹ���B_DUP_TYPE_RULE,����ĳ���ص����͵�CHILD_STEP_ID1��CHILD_STEP_ID2���Ǹ��ص����͵�STEP_ID
//@@ 240020007		���ô���													�����ص������жϹ���B_DUP_TYPE_RULE,&&����Ĳ���������Ϊ�ַ���
//@@ 240020008		���ô���													�����ص������жϹ���B_DUP_TYPE_RULE,||����Ĳ���������Ϊ�ַ���
//@@ 240020009		���ô���													�����ص������жϹ���B_DUP_TYPE_RULE,!����Ĳ���������Ϊ�ַ���
//@@ 240020010		���ô���													�����ص������жϹ���B_DUP_TYPE_RULE,���õ�����Ĳ���������֧��
//@@ 240020011		�ڴ����													�ڴ治��
//@@ 240020012		���ô���													���õ��ص������ֶζ��������֧��,���������ص������жϹ���B_DUP_TYPE_RULE
//@@ 240020013		���ô���													����1�����2���Ͳ�һ��,���������ص������жϹ���B_DUP_TYPE_RULE
//@@ 240020014		����Ŀ¼													�������Ŀ¼����Ȩ������


//#
//# MBC_APP_STOP
//#
#define MBC_APP_STOP 			240030000

//#
//# MBC_APP_UNIONACCT
//#
#define MBC_LoadAggr			250000000
//@@ 250000001	���乲���ڴ�(�¼������ۻ�)ʱ����	�����Ƿ����㹻���ڴ�
//@@ 250000002	���ӹ����ڴ�(�¼������ۻ�)ʱ����	����������
//@@ 250000003	���乲���ڴ�(��Ŀ�����ۻ�)ʱ����	�����Ƿ����㹻���ڴ�
//@@ 250000004	���ӹ����ڴ�(��Ŀ�����ۻ�)ʱ����	����������
//@@ 250000005	�����ڴ�(�¼������ۻ�)������		����������
//@@ 250000006	�����ڴ�(��Ŀ�����ۻ�)������		����������
//@@ 250000007	�¼������ۻ����������			�뽫EVENT_AGGR_INDEX_COUNT��ֵ�Ĵ�
//@@ 250000008	��Ŀ�����ۻ����������			�뽫ACCTITEM_AGGR_INDEX_COUNT��ֵ�Ĵ�
//@@ 250000009	�¼������ۻ����ݶ����			�뽫EVENT_AGGR_DATA_COUNT��ֵ�Ĵ�
//@@ 250000010	��Ŀ�����ۻ����ݶ����			�뽫ACCTITEM_AGGR_DATA_COUNT��ֵ�Ĵ�
//@@ 250000011	��������ȷ				���޸Ĳ�������������
//@@ 250000012	�¼��ۻ������ڴ���д���ź�����ʼ������	����KEYֵ
//@@ 250000013	��Ŀ�ۻ������ڴ���д���ź�����ʼ������	����KEYֵ

#define MBC_LoadBalance			250010000
//@@ 250010001	���乲���ڴ�(���)ʱ����		�����Ƿ����㹻���ڴ�
//@@ 250010002	�����ڴ�(���)������			����������
//@@ 250010003	������������				�뽫BALANCEBUF_INDEX_COUNT��ֵ�Ĵ�
//@@ 250010004	������ݶ����				�뽫BALANCEBUF_DATA_COUNT��ֵ�Ĵ�
//@@ 250010005	��������ȷ				���޸Ĳ�������������

#define MBC_FiltrateStopUser		250011000
//@@ 250011001	ȡ������������		�������ù�������b_filtrate_Stop_User_cond

//#
//# MBC_APP_VIREMENT
//#
#define MBC_BankMgr			250020000
//@@ 250020001	���˻�����Ӧ�������ʺŲ�����			�����BANK_BRANCH
//@@ 250020002	�������յ��ڵ�ռ�ʧ��

#define MBC_PaymentPlanMgr			250021000
//@@ 250021001	֧��������ʼ��----����qry�ռ�ʧ��
//@@ 250021002	���ʻ�û������֧������
//@@ 250021003	���ʻ�û����������֧������

#define MBC_TrustBankMgr			250022000
//@@ 250022001	���յ����ʼ��----����qry�ռ�ʧ��
//@@ 250022002	���յ����ʼ��----����pBankMgr�ռ�ʧ��
//@@ 250022003	���յ����ʼ��----����pPaymentPlanMgr�ռ�ʧ��
//@@ 250022004	��ѯ��������----δָ����Ҫ�������յ�����
//@@ 250022005	������ʼ������----Example:: trustbank -c
//@@ 250022006	������ʼ������----����TrustBankSeqMgr�ռ�ʧ��
//@@ 250022007	������ʼ������----��ָ���������Ͳ�����
//@@ 250022008	������ʼ������----��ָ����������û����Ҫ���е�����
//@@ 250022009	���ʻ����ȼ���ߵĸ��ʽ�������շ�ʽ

#define MBC_SystemSwitchMgr			250023000
//@@ 250023001	ϵͳ�������ʼ��----����qry�ռ�ʧ��
//@@ 250023001	ϵͳ�������ʼ��----�ò�������Ӧ������Ӧ��ֵδ����

#define MBC_AcctItemVireMgr			250024000
//@@ 250024001	��Ŀ�������ʼ��----����qry�ռ�ʧ��

#define MBC_BillMgr					250025000
//@@ 250025001	�Զ��ۿ����ʼ��----����qry�ռ�ʧ��
//@@ 250025002	�Զ��ۿ����ʼ��----����pSystemSwitchMgr�ռ�ʧ��
//@@ 250025003	���˻����ڶ�����ͷ���͵�����˱�
//@@ 250025004	���˻����ڶ����Զ��ۿ����͵�����˱�

#define MBC_VirementMgr				250026000
//@@ 250026001	�������ʼ��----����qry�ռ�ʧ��
//@@ 250026002	�������ʼ��----����pCfgMgr�ռ�ʧ��
//@@ 250026003	�������ʼ��----����pAcctItemMgr�ռ�ʧ��
//@@ 250026004	�������ʼ��----����pPaymentPlanMgr�ռ�ʧ��
//@@ 250026005	�������ʼ��----����pBillMgr�ռ�ʧ��
//@@ 250026006	������ʼ������----Example:: virement -c
//@@ 250026007	������ʼ������----��ָ���������Ͳ�����
//@@ 250026008	������ʼ������----��ָ����������û����Ҫ���ʵ�����
//@@ 250026009	������ʼ������----δָ����Ҫ�������ʵ�����
//@@ 250026010	run():��ǰ��ͬ����Ŀ̫��
//@@ 250026011	�û����Ϲ����ڴ��в����û�����ʧ��
//@@ 250026012	�������ϵ�����ϢΪ��
//@@ 250026013	�������ʼ��----����pRedoLogMgr�ռ�ʧ��
//@@ 250026014	��ĩ�������ȶϵ�----���(B_REDO_LOG��)
//@@ 250026015	��ĩ���ʽ�������������״̬----����ǰ����PROCESS_ID��������
#define	MBC_BalItemGroupMgr			250026500
//@@ 250026501	ר��ר����Ŀ��Ա��HASH������			����Ƿ����㹻���ڴ�

#define	MBC_RT_QRY				250027000
//@@ 250027001	�м������,ȡSERV_ID��������			��鴫���BUFFֵ
//@@ 250027002	����ռ����							����Ƿ����㹻���ڴ�
//@@ 250027003	�м������,ȡSERV_ID����				��鴫���BUFFֵ
//@@ 250027004	�м������,ȡBILLING_CYCLE_ID����		��鴫���BUFFֵ

#define	MBC_RT_VIREMENT			250027500
//@@ 250027501	�м������,ȡACCT_ID��������			��鴫���BUFFֵ
//@@ 250027502	����ռ����							����Ƿ����㹻���ڴ�
//@@ 250027503	�м������,ȡACCT_ID����				��鴫���BUFFֵ

#define	MBC_RT_VIRE_ROLL		250027800
//@@ 250027801	����ռ����							����Ƿ����㹻���ڴ�
//@@ 250027802	�������ʱ��в����ڴ���Ŀ				���ACCT_ITEM_OWE��ʵʱ�����Ƿ���RT_VIRE_ACCT_ITEM������ƥ��
//@@ 250027803	�������ʱ����ݺ�Ƿ�ѱ����ݲ���			���ACCT_ITEM_OWE��ʵʱ�����Ƿ���RT_VIRE_ACCT_ITEM������ƥ��

#define MBC_ProcessAgentServer				250028000
//@@ 250028000	��ȡ������Ϣ��		���etc/process_agent.ini��,[process_id]��LOCAL_PORT������
//@@ 250028001	Socket--listen��	���etc/process_agent.ini��,[process_id]��LOCAL_PORT������
//@@ 250028002	��ȡ�ͻ������ݴ�	���ܿͻ��˽������˳�
#define MBC_ProcessAgentClient				250029000
//@@ 250029000	��ȡ������Ϣ��		���etc/process_agent.ini��,[process_id]��REMOTE_IP,REMOTE_PORT������
//@@ 250029001	����Զ��Socket��	������Ƿ�����,���etc/process_agent.ini,[process_id]��REMOTE_IP,REMOTE_PORT������

#define MBC_FormulaMgr				250030000
//@@ 250030000	��ʽִ�д���  		��鿴log 
//@@ 250030001	��ʽִ�д���  		��鿴log 

#define	MBC_PutInFile				250031000
//@@ 250031000	�Ҳ�����������	���b_event_to_file_cfg����û�е�ǰprocess_id����������
//@@ 250031001	�Ҳ�����Ҫ�����ֶε���������	���b_event_to_file_field_cfg��, ��û�е�ǰprocess_id������
//@@ 250031001	���ļ�ʧ��	���b_event_to_file_cfg����ļ���, ���Ը��ļ��Ƿ��д

#define MBC_LoadOwe					250032000
//@@ 250032001	���乲���ڴ�(����Ƿ��)ʱ����		�����Ƿ����㹻���ڴ�
//@@ 250032002	�����ڴ�(����Ƿ��)������			����������
//@@ 250032003	����Ƿ�����������				�뽫ACCTITEMOWEBUF_INDEX_COUNT��ֵ�Ĵ�
//@@ 250032004	����Ƿ�����ݶ����				�뽫ACCTITEMOWEBUF_DATA_COUNT��ֵ�Ĵ�
//@@ 250032005	��������ȷ				���޸Ĳ�������������

#define MBC_OweMgr			250033000
//@@ 250033001	�޷����ӹ����ڴ�(����Ƿ��)	������������Ƿ�����س���
//@@ 250033002	�����ڴ�(����Ƿ��)������	������������Ƿ�����س���
//@@ 250033003	���乲���ڴ�(����Ƿ��)ʱ����		�����Ƿ����㹻���ڴ�
//@@ 250033004	�����ڴ�(����Ƿ��)������			����������

#define MBC_StateConvertMgr		250034000

#define MBC_PlanType			250035000

#define MBC_OfferMgr            250036000
//@@ 250036001  ����ռ����            ����Ƿ����㹻���ڴ�

#define MBC_Rebill            260010000
//@@ 260010001	����״̬�Ƿ�		����B_REBILL_TASK��TASK_STEP
//@@ 260010002	����״̬�Ƿ�		�����̿��:����û�з����ܹ����ڴ�,��Ȼ��������...
//@@ 260010003	����û������		����B_PARAM_DEFINE�����û��˵ı�����


#define MBC_FileDB            270010000
//@@ 270010001		�Ҳ������ʵ�org		����org�Ҳ������ʵ�org levelΪ200��org_id
//@@ 270010002  	д�����ļ�ʧ��..	��������Ƿ����㹻�Ŀռ�
//@@ 270010003  	д�ļ�PackageID��ȡʧ�� 	�쳣ʧ��,��������������
//@@ 270010004  	������⹲���ڴ�ʧ�� 	������ݿ�b_ipc_cfg�����Ƿ���ȷ
//@@ 270010005  	�Ҳ��������� 	����ratable_event_type���������ü�¼


#define MBC_Format            280010000
//@@ 280010001		���û���ѹ���ø�λ��־ʧ��		SetServBlockFlag(0)����false�����û���ѹ���ø�λ��־ʧ��


#define MBC_CheckdupDel            290010000
//@@ 290010001		CheckDup::CheckDupDel ���� TicketFileBackStruct�ڴ����		CheckDup::CheckDupDel ���� TicketFileBackStruct�ڴ����

#define MBC_RedoEvents            290030000
//@@ 290030001		RedoEvents::redoMain ���� m_ohSwitchInfo�ڴ����		RedoEvents::redoMain ���� m_ohSwitchInfo�ڴ����


#define MBC_AcctItemAccu       290020000
//@@ 290020001  �޷��������ʹ����ڴ�    ��ִ�������ڴ�������ctlacctitem
//@@ 290020002  �޷������ڴ�            �������ڴ�:)
//@@ 290020003  �޷����������ڴ�        �����AcctItemCount(b_param_define)

#define MBC_IpcKey             290040000
//@@ 290040001  �޷���ȡHOST_ID         ����wf_hostinfo�е���Ϣ�Ƿ�������ȷ
//@@ 290040002  �޷���ȡIPC_KEY         ����B_IPC_CFG��������Ƿ�����
//@@ 290040003	�޷�����IPC_KEY			����IPC_NAME������Ϊ��

#define MBC_PricingInfoLoad    290050000
//@@ 290050001  �����ڴ�ʧ��            ���������Ƿ����㹻���ڴ�
//@@ 290050002  Ԥ����ռ䲻��          B_WHOLE_DISCOUNT�仯�У�������������

#define MBC_OfferAccuMgr       290060000
//@@ 290060001  ��ʼ��AccuMgrʧ��       ������
//@@ 290060002  ��ȡseq_accu_instʧ��   �������ݿ�����seq_accu_inst
//@@ 290060003  ��ȡseq_accu_cycleʧ��  �������ݿ�����seq_accu_cycle
//@@ 290060004  �����ۻ��ڴ�ʧ��        ���������Ƿ����㹻���ڴ�
//@@ 290060005  ���������ڴ�ʧ��        ���������Ƿ����㹻���ڴ�
//@@ 290060006  �ۻ�������ʱ����ʧ��	 ���������Ƿ����㹻���ڴ�

#define MBC_ParamDefineMgr     290070000
//@@ 290070001  ��ȡHOST_IDʧ��         ����wf_hostinfo�������Ƿ���ȷ
//@@ 290070002  ��ȡ����ʧ��            ����b_param_define��������Ƿ�����
//@@ 290070003	ɾ������ʧ��			������Ҫɾ���������õ�PARAM_SEGMENT��PARAM_CODE������Ϊ��
//@@ 290070004	���ò���ʧ��			������Ҫ���ò������õ�PARAM_SEGMENT��PARAM_CODE��PARAM_VALUE������Ϊ��

#define MBC_FilterRule         290080000
//@@ 290080001  �����ڴ�ʧ��            �����Ƿ����㹻���ڴ�
//@@ 290080002  ���ز���ʧ��            �����MAX_LONG_TYPE

#define MBC_AccuTypeCfg        290090000
//@@ 290090001  �����������ô���        ���ֿ�ʼ�������Ͳ�Ӧ�����ô����͵�ƫ����

#define MBC_CheckRule          290090000
//@@ 290090001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 290090002	�����ڴ�ʧ��	���������ݣ���������������
#define MBC_CheckSubRule       290091000
//@@ 290091001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 290091002	�����ڴ�ʧ��	���������ݣ���������������
#define MBC_CheckArgument      290092000
//@@ 290092001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 290092002	�����ڴ�ʧ��	���������ݣ���������������
#define MBC_CheckRelation      290093000
//@@ 290093001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 290093002	�����ڴ�ʧ��	���������ݣ���������������
#define MBC_TransFileType      290094000
//@@ 290094001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 290094002	�����ڴ�ʧ��	���������ݣ���������������
#define MBC_TagSeq             290094100
//@@ 290094101	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 290094102	�����ڴ�ʧ��	���������ݣ���������������
#define MBC_SegmentMgr			290094200
//@@ 290094201	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 290094202	�����ڴ�ʧ��	���������ݣ���������������

#define MBC_SpecailNbrStrategyMgr  310010000
//@@ 310010001	���뽨���ռ�ʧ��	���������ڴ��Ƿ��㹻

#define MBC_TransTermFile	310020000
//@@ 310020001  ��ȡ���²����ļ���������һ��
//@@ 310020002  ��ȡ��Ӧ����ʧ��
//@@ 310020003  �ļ�У�����������С�����ڣ�ʡ�����У��
//@@ 310020004  ��ȡ�ļ�������Ϣ����
//@@ 310020004  ��ȡ�������ó���ȱ�ٲ�������

#define MBC_ParamShmMgr     310030000
//@@ 310030001	�����ڴ����ô���		����������ڵĹ����ڴ�KEY���ô���
//@@ 310030002	ɾ�������ڴ�ʧ��		�ù����ڴ��Ų�����
//@@ 310030003	���빲���ڴ�ʧ��		�ù����ڴ��Ų�����
//@@ 310030004	���乲���ڴ�ʧ��		�����Ƿ����㹻���ڴ�
//@@ 310030005	���乲���ڴ�ʧ��		�ù����ڴ��Ų�����
//@@ 310030006	ɾ�������ڴ�ʧ��		������������ʹ��,����ֹͣ��������
//@@ 310030007	��ȡ�����ڴ�����ӽ�����ʧ��		�����ڴ���ܲ�����

#define MBC_ParamInfoInterface     310040000
//@@ 310040001	���������ڴ治����		���ȴ��������ڴ�

#define MBC_ParamInfo     310020000
//@@ 310040001	��ʼ�������ò�����ȫ		�޷���ʼ�����ݿ��е����ò���

#define MBC_ParamInfoMgr     310050000
//@@ 310050001	��ȡ����ʧ��			ϵͳδ���������������ڴ��С
//@@ 310050002	�����ڴ�ʧ��			���������������ռ䲻�������޸�����������������
//@@ 310050003	��ȡ����ʧ��			ϵͳδ��������������ڴ��С
//@@ 310050004	�����ڴ�ʧ��			��������������ռ䲻�������޸�����������������

//@@ 310050005	��ȡ����ʧ��			ϵͳ������δ���ö���������ڴ��С
//@@ 310050006	�����ڴ�ʧ��			��������������ռ䲻�������޸�����������������
//@@ 310050007	���Ҳ���ʧ��			��������ƥ��ʧ�ܣ���Ӧ��ID�Ķ�����Ϣ������

//@@ 310050008	��ȡ����ʧ��			ϵͳ������δ�����ʷѲ������ڴ��С
//@@ 310050009	�����ڴ�ʧ��			�ʷѲ����������ռ䲻�������޸�����������������
//@@ 310050010	���Ҳ���ʧ��			�ʷ�����ƥ��ʧ�ܣ���Ӧ��ID����Ϣ������

//@@ 310050011	��ȡ����ʧ��			ϵͳ������δ�����Żݲ������ڴ��С
//@@ 310050012	�����ڴ�ʧ��			�Żݲ����������ռ䲻�������޸�����������������
//@@ 310050013	���Ҳ���ʧ��			�Ż�����ƥ��ʧ�ܣ���Ӧ��ID����Ϣ������

//@@ 310050014	��ȡ����ʧ��			ϵͳ������δ�����Żݷ����߲������ڴ��С
//@@ 310050015	�����ڴ�ʧ��			�Żݷ����߲����������ռ䲻�������޸�����������������
//@@ 310050016	���Ҳ���ʧ��			�Żݷ���������ƥ��ʧ�ܣ���Ӧ��ID����Ϣ������

//@@ 310050017	��ȡ����ʧ��			ϵͳ������δ�����Żݷ������������ڴ��С
//@@ 310050018	�����ڴ�ʧ��			�Żݷ����������������ռ䲻�������޸�����������������
//@@ 310050019	���Ҳ���ʧ��			�Żݷ����������ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050020	��ȡ����ʧ��			ϵͳ������δ�����ۿ۲������ڴ��С
//@@ 310050021	�����ڴ�ʧ��			�ۿ۲����������ռ䲻�������޸�����������������
//@@ 310050022	���Ҳ���ʧ��			�ۿ�����ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050023	��ȡ����ʧ��			ϵͳ������δ�����������ڲ������ڴ��С
//@@ 310050024	�����ڴ�ʧ��			�������ڲ����������ռ䲻�������޸�����������������
//@@ 310050025	���Ҳ���ʧ��			������������ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050026	��ȡ����ʧ��			ϵͳ������δ���ö����������ڴ��С
//@@ 310050027	�����ڴ�ʧ��			���������������ռ䲻�������޸�����������������
//@@ 310050028	���Ҳ���ʧ��			��������ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050029	��ȡ����ʧ��			ϵͳ������δ����ConditionPoint(������)�������ڴ��С
//@@ 310050030	�����ڴ�ʧ��			ConditionPoint(������)�����������ռ䲻�������޸�����������������
//@@ 310050031	���Ҳ���ʧ��			ConditionPoint(������)����ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050032	��ȡ����ʧ��			ϵͳ������δ����FilterCond(��������)�������ڴ��С
//@@ 310050033	�����ڴ�ʧ��			FilterCond(��������)�����������ռ䲻�������޸�����������������
//@@ 310050034	���Ҳ���ʧ��			FilterCond(��������)����ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050035	��ȡ����ʧ��			ϵͳ������δ������ϲ������ڴ��С
//@@ 310050036	�����ڴ�ʧ��			ͨ����ϲ����������ռ䲻�������޸�����������������
//@@ 310050037	���Ҳ���ʧ��			ͨ���������ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������
//@@ 310050038	�����ڴ�ʧ��			CheckRule�����������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_CHECKRULE_RULE_INFO_NUM)
//@@ 310050039	���Ҳ���ʧ��			CheckRule�������ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������(LOADPARAMINFO, PARAM_CHECKRULE_RULE_INFO_NUM)
//@@ 310050040	�����ڴ�ʧ��			CheckSubRule�����������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_CHECKRULE_SUBRULE_INFO_NUM)
//@@ 310050041	���Ҳ���ʧ��			CheckSubRule�������ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������(LOADPARAMINFO, PARAM_CHECKRULE_SUBRULE_INFO_NUM)
//@@ 310050042	�����ڴ�ʧ��			Argument��ϲ����������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_CHECKARGUMENT_INFO_NUM)
//@@ 310050043	���Ҳ���ʧ��			Argument�������ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������(LOADPARAMINFO, PARAM_CHECKARGUMENT_INFO_NUM)


//@@ 310050044	��ȡ����ʧ��			ϵͳ������δ������Ϲ�ϵ�������ڴ��С
//@@ 310050045	�����ڴ�ʧ��			��Ϲ�ϵ�����������ռ䲻�������޸�����������������
//@@ 310050046	���Ҳ���ʧ��			��Ϲ�ϵ����ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050047	��ȡ����ʧ��			ϵͳ������δ����TimeCond(ʱ��ֶ�����)�������ڴ��С
//@@ 310050048	�����ڴ�ʧ��			ͨ��TimeCond(ʱ��ֶ�����)�����������ռ䲻�������޸�����������������
//@@ 310050049	���Ҳ���ʧ��			ͨ��TimeCond(ʱ��ֶ�����)����ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050050	��ȡ����ʧ��			ϵͳ������δ����EventTypeCond�������ڴ��С
//@@ 310050051	�����ڴ�ʧ��			ͨ��EventTypeCond�����������ռ䲻�������޸�����������������
//@@ 310050052	���Ҳ���ʧ��			ͨ��EventTypeCond����ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050053	��ȡ����ʧ��			ϵͳ������δ����CycleType(��������)�������ڴ��С
//@@ 310050054	�����ڴ�ʧ��			ͨ��CycleType(��������)�����������ռ䲻�������޸�����������������
//@@ 310050055	������ʧ��			ͨ��CycleType(��������)checkʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050056	��ȡ����ʧ��			ϵͳ������δ����AccuType�������ڴ��С
//@@ 310050057	�����ڴ�ʧ��			ͨ��AccuType�����������ռ䲻�������޸�����������������
//@@ 310050058	���Ҳ���ʧ��			ͨ��AccuType����ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050059	��ȡ����ʧ��			ϵͳ������δ����OfferAccuRelation(��Ʒ���ۻ����Ͷ�Ӧ��ϵ)�������ڴ��С
//@@ 310050060	�����ڴ�ʧ��			ͨ��OfferAccuRelation(��Ʒ���ۻ����Ͷ�Ӧ��ϵ)�����������ռ䲻�������޸�����������������
//@@ 310050061	���Ҳ���ʧ��			ͨ��OfferAccuRelation(��Ʒ���ۻ����Ͷ�Ӧ��ϵ)����ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050062	��ȡ����ʧ��			ϵͳ������δ����OfferAccuExist�������ڴ��С
//@@ 310050063	���Ҳ���ʧ��			ͨ��OfferAccuExist����ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������

//@@ 310050064	��ȡ����ʧ��			ϵͳ������δ����SpecialCycleTypee�������ڴ��С
//@@ 310050065	���Ҳ���ʧ��			ͨ��SpecialCycleType����ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������
//@@ 310050066	��ȡ����ʧ��			ϵͳ������δ����Ʒ<->��ϲ������ڴ��С
//@@ 310050067	���Ҳ���ʧ��			��Ʒ<->�������ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������
//@@ 310050068	��ȡ����ʧ��			ϵͳ������δ���Ʒ<->��ϲ������ڴ��С
//@@ 310050069	���Ҳ���ʧ��			��Ʒ<->�������ƥ��ʧ�ܣ���Ӧ�����ݵ���Ϣ������
//@@ 310050070	��ȡ����ʧ��			ϵͳ������δ���¼��������ڴ��С
//@@ 310050071	�����ڴ�ʧ��			ͨ���¼������������ռ䲻�������޸�����������������
//@@ 310050072	��ȡ����ʧ��			ϵͳ������δ��Split(�ָ�����)�������ڴ��С
//@@ 310050073	�����ڴ�ʧ��			Split(�ָ�����)�����������ռ䲻�������޸�����������������
//@@ 310050074	��ȡ����ʧ��			ϵͳ������δ�����������Բ������ڴ��С
//@@ 310050075	�����ڴ�ʧ��			���������Բ����������ռ䲻�������޸�����������������
//@@ 310050076	��ȡ����ʧ��			ϵͳ������δ��������ϲ������ڴ��С
//@@ 310050077	�����ڴ�ʧ��			������ϲ����������ռ䲻�������޸�����������������
//@@ 310050078	��ȡ����ʧ��			ϵͳ������δ��CG��ϲ������ڴ��С
//@@ 310050079	�����ڴ�ʧ��			CG��ϲ����������ռ䲻�������޸�����������������
//@@ 310050080	��ȡ����ʧ��			ϵͳ������δ���������ʷѲ������ڴ��С
//@@ 310050081	�����ڴ�ʧ��			�����ʷѲ����������ռ䲻�������޸�����������������
//@@ 310050082	��ȡ����ʧ��			ϵͳ������δ���������ʷ�CHECK�������ڴ��С
//@@ 310050083	�����ڴ�ʧ��			�����ʷ�CHECK�����������ռ䲻�������޸�����������������
//@@ 310050084	��ȡ����ʧ��			ϵͳ������δ����SpInfo�������ڴ��С(LOADPARAMINFO, PARAM_SPINFOSEEKINFO_NUM)
//@@ 310050085	�����ڴ�ʧ��			SpInfo�����������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_SPINFOSEEKINFO_NUM)
//@@ 310050086	��ȡ����ʧ��			ϵͳ������δ����GatherTask�������ڴ��С(LOADPARAMINFO, PARAM_GATHERDATAINFO_NUM)
//@@ 310050087	�����ڴ�ʧ��			GatherTask�����������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_GATHERDATAINFO_NUM)
//@@ 310050088	��ȡ����ʧ��			ϵͳ������δ����GatherTaskList�������ڴ��С(LOADPARAMINFO, PARAM_GATERTASKLISTINFO_NUM)
//@@ 310050089	�����ڴ�ʧ��			GatherTaskList�����������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_GATERTASKLISTINFO_NUM)
//@@ 310050090	��ȡ����ʧ��			ϵͳ������δ����ProductAcctItem�������ڴ��С
//@@ 310050091	��ȡ����ʧ��			ϵͳ������δ����PROIDPRODUCTACCTITEM�������ڴ��С
//@@ 310050092	��ȡ����ʧ��			ϵͳ������δ����LongTypeRule�������ڴ��С(LOADPARAMINFO, PARAM_LONGTYPE_INFO_NUM)
//@@ 310050093	��ȡ����ʧ��			ϵͳ������δ����HOST�������ڴ��С(LOADPARAMINFO,PARAM_HOSTINFOINFO_NUM)
//@@ 310050094	�����ڴ�ʧ��			HOST�����������ռ䲻�������޸�����������������(LOADPARAMINFO,PARAM_HOSTINFOINFO_NUM)

//@@ 310050095	��ȡ����ʧ��			ϵͳ������δ����CheckRelation�������ڴ��С(LOADPARAMINFO, PARAM_CHECKRELATION_INFO_NUM)
//@@ 310050096	�����ڴ�ʧ��			CheckRelation�����������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_CHECKRELATION_INFO_NUM)
//@@ 310050097	��ȡ����ʧ��			ϵͳ������δ����TransFileType�������ڴ��С(LOADPARAMINFO, PARAM_TRANSFILE_INFO_NUM)
//@@ 310050098	�����ڴ�ʧ��			TransFileType�����������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_TRANSFILE_INFO_NUM)
//@@ 310050099	��ȡ����ʧ��			ϵͳ������δ����xx�������ڴ��С
//@@ 310050100	�����ڴ�ʧ��			xx�����������ռ䲻�������޸�����������������


//@@ 310050101	��ȡ����ʧ��			ϵͳδ����ORG���ڴ��С
//@@ 310050102	�����ڴ�ʧ��			ORG�������ռ䲻�������޸�����������������
//@@ 310050103	��ȡ����ʧ��			ϵͳδ����OFFER���ڴ��С(LOADPARAMINFO,PARAM_OFFER_NUM)
//@@ 310050104	�����ڴ�ʧ��			OFFER�������ռ䲻�������޸�����������������(LOADPARAMINFO,PARAM_OFFER_NUM)
//@@ 310050105	��ȡ����ʧ��			ϵͳδ����OFFERSEPCIALATTR���ڴ��С(LOADPARAMINFO, PARAM_OFFER_SPECATTR_NUM)
//@@ 310050106	�����ڴ�ʧ��			OFFERSEPCIALATTR�������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_OFFER_SPECATTR_NUM)
//@@ 310050107	��ȡ����ʧ��			ϵͳδ����OFFERNUMBERATTR���ڴ��С(LOADPARAMINFO, PARAM_OFFER_NUMATTR_NUM)
//@@ 310050108	�����ڴ�ʧ��			OFFERNUMBERATTR�������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_OFFER_NUMATTR_NUM)
//@@ 310050109	��ȡ����ʧ��			ϵͳδ����OFFERFREE���ڴ��С(LOADPARAMINFO, PARAM_OFFER_FREE_NUM)
//@@ 310050110	�����ڴ�ʧ��			OFFERFREE�������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_OFFER_FREE_NUM)
//@@ 310050111	��ȡ����ʧ��			ϵͳδ����PRODUCT���ڴ��С
//@@ 310050112	�����ڴ�ʧ��			PRODUCT�������ռ䲻�������޸�����������������
//@@ 310050113	��ȡ����ʧ��			ϵͳδ����OFFERATTR���ڴ��С
//@@ 310050114	�����ڴ�ʧ��			OFFERATTR�������ռ䲻�������޸�����������������
//@@ 310050115	��ȡ����ʧ��			ϵͳδ����ACCTITEM���ڴ��С(LOADPARAMINFO, PARAM_ACCTITEM_NUM)
//@@ 310050116	�����ڴ�ʧ��			ACCTITEM�������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_ACCTITEM_NUM)
//@@ 310050117	��ȡ����ʧ��			ϵͳδ����ACCTITEMMEMBERS���ڴ��С(LOADPARAMINFO, PARAM_ACCTITEMMEMBERS_NUM)
//@@ 310050118	��ȡ����ʧ��			ϵͳδ����ACCTITEMGROUPS���ڴ��С(LOADPARAMINFO, PARAM_ACCTITEMGROUPS_NUM)
//@@ 310050119	�����ڴ�ʧ��			ACCTITEMMEMBERS�������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_ACCTITEMGROUPS_NUM)
//@@ 310050120	��ȡ����ʧ��			ϵͳδ����ACCTITEMMEMBERSB���ڴ��С(LOADPARAMINFO, PARAM_ACCTITEMMEMBERSB_NUM)
//@@ 310050121	��ȡ����ʧ��			ϵͳδ����ACCTITEMGROUPSB���ڴ��С(LOADPARAMINFO, PARAM_ACCTITEMGROUPSB_NUM)
//@@ 310050122	�����ڴ�ʧ��			ACCTITEMMEMBERSB�������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_ACCTITEMMEMBERSB_NUM)
//@@ 310050123	��ȡ����ʧ��			ϵͳδ����AREAORGRELATION���ڴ��С(LOADPARAMINFO, PARAM_ACCTITEMGROUPSB_NUM)
//@@ 310050124	�����ڴ�ʧ��			AREAORGRELATION�������ռ䲻�������޸�����������������
//@@ 310050125	��ȡ����ʧ��			ϵͳδ����EVENTTYPERULE���ڴ��С
//@@ 310050126	�����ڴ�ʧ��			EVENTTYPERULE�������ռ䲻�������޸�����������������
//@@ 310050127	��ȡ����ʧ��			ϵͳδ����FORMATGROUP���ڴ��С
//@@ 310050128	��ȡ����ʧ��			ϵͳδ����FORMATSTEP���ڴ��С
//@@ 310050129	��ȡ����ʧ��			ϵͳδ����FORMATRULE���ڴ��С
//@@ 310050130	��ȡ����ʧ��			ϵͳδ����FORMATCOMB���ڴ��С
//@@ 310050131	�����ڴ�ʧ��			FORMATGROUP�������ռ䲻�������޸�����������������
//@@ 310050132	�����ڴ�ʧ��			FORMATSTEP�������ռ䲻�������޸�����������������
//@@ 310050133	�����ڴ�ʧ��			FORMATRULE�������ռ䲻�������޸�����������������
//@@ 310050134	�����ڴ�ʧ��			FORMATCOMB�������ռ䲻�������޸�����������������
//@@ 310050135	��ȡ����ʧ��			ϵͳδ����ZONE_ITEM���ڴ��С(LOADPARAMINFO, PARAM_ZONE_ITEM_NUM)
//@@ 310050136	�����ڴ�ʧ��			ZONE_ITEM�������ռ䲻�������޸�����������������(LOADPARAMINFO, PARAM_ZONE_ITEM_NUM)
//@@ 310050137	��ȡ����ʧ��			ϵͳδ����BAMBOTHPRODUCT���ڴ��С
//@@ 310050138	�����ڴ�ʧ��			BAMBOTHPRODUCT�������ռ䲻�������޸�����������������
//@@ 310050139	��ȡ����ʧ��			ϵͳδ����BAMGETTARIFF���ڴ��С
//@@ 310050140	�����ڴ�ʧ��			BAMGETTARIFF�������ռ䲻�������޸�����������������
//@@ 310050141	��ȡ����ʧ��			ϵͳδ����BAMPRODTARIFF���ڴ��С
//@@ 310050142	�����ڴ�ʧ��			BAMPRODTARIFF�������ռ䲻�������޸�����������������
//@@ 310050143	�����ڴ�ʧ��			��Ŀ�������ȡʧ��
//@@ 310050144	��ȡ����ʧ��			ϵͳδ����SwitchInfo���ڴ��С
//@@ 310050145	�����ڴ�ʧ��			SwitchInfo�������ռ䲻�������޸�����������������
//@@ 310050180  ��ȡ����ʧ��                    ϵͳδ����FilePoolInfo���ڴ��СPARAM_FILEPOOLINFO_NUM
//@@ 310050181  �����ڴ�ʧ��                    FilePoolInfo���ݿռ䲻�㣬���޸�PARAM_FILEPOOLINFO_NUM�������� 
//@@ 310050182  ��ȡ����ʧ��                    ϵͳδ����FilePoolCondition���ڴ��СPARAM_FILEPOOLCOND_NUM 
//@@ 310050183  �����ڴ�ʧ��                    FilePoolCondition���ݿռ䲻�㣬���޸�PARAM_FILEPOOLCOND_NUM�������� 
//@@ 310050184  ��ȡ����ʧ��                    ϵͳδ����FileTypeConfig�������ڴ��СPARAM_FILETYPECFG_NUM 
//@@ 310050185  ��ȡ����ʧ��                    ϵͳδ����Encode(FileAttr)���ڴ��СPARAM_TLVENCODE_NUM 
//@@ 310050186  �����ڴ�ʧ��                    Encode(FileAttr)���ݿռ䲻�㣬���޸�PARAM_TLVENCODE_NUM�������� 
//@@ 310050187  ��ȡ����ʧ��                    ϵͳδ����Decode(FileAttr)���ڴ��СPARAM_TLVDECODE_NUM 
//@@ 310050188  �����ڴ�ʧ��                    Decode(FileAttr)���ݿռ䲻�㣬���޸�PARAM_TLVDECODE_NUM�������� 
//@@ 310050189  ��ȡ����ʧ��                    ϵͳδ����FileParseRule���ڴ��СPARAM_FILEPARSERULE_NUM 
//@@ 310050190  ��ȡ����ʧ��                    ϵͳδ����SwitchCode���ڴ��СPARAM_SWITCHCODE_NUM 
//@@ 310050191  �����ڴ�ʧ��                    SwitchCode���ݿռ䲻�㣬���޸�PARAM_SWITCHCODE_NUM�������� 
//@@ 310050192  ��ȡ����ʧ��                    ϵͳδ����BillingNodeInfo���ڴ��СPARAM_BILLINGNODEINFO_NUM 
//@@ 310050193  �����ڴ�ʧ��                    BillingNodeInfo���ݿռ䲻�㣬���޸�PARAM_BILLINGNODEINFO_NUM�������� 
//@@ 310050197	�����ڴ�ʧ��			��׼�������������ռ䲻��,δ�����뵽�ռ�,���޸�����������������
//@@ 310050198	��ȡ����ʧ��			PARAM_TEMPLATEMAPSWITCH_NUM
//@@ 310050199	�����ڴ�ʧ��			��׼����������,���޸�����������������
//@@ 310050200	�����ڴ�ʧ��			SwitchInfo�������ռ䲻�������޸�����������������
//@@ 310050201  �����ڴ�ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_NBR_MAP_NUM)�Ĵ�С
//@@ 310050202  ��ȡ����ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_NBR_MAP_NUM)�Ĵ�С
//@@ 310050203  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_LONGEVENTTYPERULE_NUM)�Ĵ�С
//@@ 310050204  ��ȡ����ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_LONGEVENTTYPERULE_NUM)�Ĵ�С
//@@ 310050205  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_LONGEVENTTYPERULEEX_NUM)�Ĵ�С
//@@ 310050206  ��ȡ����ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_LONGEVENTTYPERULEEX_NUM)�Ĵ�С
//@@ 310050207  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_PRICINGPLANTYPE_NUM)�Ĵ�С
//@@ 310050208  ��ȡ����ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_PRICINGPLANTYPE_NUM)�Ĵ�С
//@@ 310050209  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_STATECONVERT_NUM)�Ĵ�С
//@@ 310050210  ��ȡ����ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_STATECONVERT_NUM)�Ĵ�С
//@@ 310050211  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_SPLITACCTGROUP_NUM)�Ĵ�С
//@@ 310050212  ��ȡ����ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_SPLITACCTGROUP_NUM)�Ĵ�С
//@@ 310050213  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_SPLITACCTEVENT_NUM)�Ĵ�С
//@@ 310050214  ��ȡ����ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_SPLITACCTEVENT_NUM)�Ĵ�С
//@@ 310050215  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_DISTANCETYPE_NUM)�Ĵ�С
//@@ 310050216  ��ȡ����ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_DISTANCETYPE_NUM)�Ĵ�С
//@@ 310050217  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_EDGEROAMRULEEX_NUM)�Ĵ�С
//@@ 310050218  ��ȡ����ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_EDGEROAMRULEEX_NUM)�Ĵ�С
//@@ 310050219  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_HEADEX_NUM)�Ĵ�С
//@@ 310050220  ��ȡ����ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_HEADEX_NUM)�Ĵ�С
//@@ 310050221  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_SEGMENTINFO_INFO_NUM)�Ĵ�С
//@@ 310050222  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_SEGMENTINFO_INFO_NUM)�Ĵ�С
//@@ 310050223  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_LOCALHEAD_NUM)�Ĵ�С
//@@ 310050224  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_LOCALHEAD_NUM)�Ĵ�С
//@@ 310050225  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_EVENTTYPE_NUM)�Ĵ�С
//@@ 310050226  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_EVENTTYPE_NUM)�Ĵ�С
//@@ 310050227  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_EVENTTYPEGROUP_NUM)�Ĵ�С
//@@ 310050228  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_EVENTTYPEGROUP_NUM)�Ĵ�С
//@@ 310050229  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_VALUEMAP_NUM)�Ĵ�С
//@@ 310050230  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_VALUEMAP_NUM)�Ĵ�С
//@@ 310050231  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO,PARAM_ROAMCARRIER_NUM)�Ĵ�С
//@@ 310050232  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO,PARAM_ROAMCARRIER_NUM)�Ĵ�С 
//@@ 310050233  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO,PARAM_ROAMTARIFF_NUM)�Ĵ�С
//@@ 310050234  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO,PARAM_ROAMTARIFF_NUM)�Ĵ�С 
//@@ 310050235  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO,PARAM_ROAMSPONSOR_NUM)�Ĵ�С
//@@ 310050236  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO,PARAM_ROAMSPONSOR_NUM)�Ĵ�С 
//@@ 310050237  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO,PARAM_SPECIALGROUPMEMBER_NUM)�Ĵ�С
//@@ 310050238  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO,PARAM_SPECIALGROUPMEMBER_NUM)�Ĵ�С 
//@@ 310050239  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO,PARAM_TARIFFSECTOR_NUM)�Ĵ�С
//@@ 310050240  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO,PARAM_COUNTRY_NUM)�Ĵ�С
//@@ 310050241  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO,PARAM_COUNTRY_NUM)�Ĵ�С 

//@@ 310050300  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_TEMPLATEMAPSWITCH_NUM)�Ĵ�С
//@@ 310050301  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_TEMPLATEMAPSWITCH_NUM)�Ĵ�С

//@@ 310050302  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_OBJECTDEFINE_NUM)�Ĵ�С
//@@ 310050303  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_OBJECTDEFINE_NUM)�Ĵ�С

//@@ 310050304  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_EVENTPARSERGROUP_NUM)�Ĵ�С
//@@ 310050305  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_EVENTPARSERGROUP_NUM)�Ĵ�С

//@@ 310050306  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_FIELD_SHM_NUM)�Ĵ�С
//@@ 310050307  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_TEMPLATE_RELATION_SHM_NUM)�Ĵ�С
//@@ 310050308  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_TEMPLATE_RELATION_SHM_NUM)�Ĵ�С
//@@ 310050309  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_FIELD_SHM_NUM)�Ĵ�С

//@@ 310050310  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_SWITCH_TYPE_INFO_NUM)�Ĵ�С
//@@ 310050311  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_SWITCH_TYPE_INFO_NUM)�Ĵ�С

//@@ 310050312  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_EVENT_ATTR_INFO_NUM)�Ĵ�С
//@@ 310050313  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_EVENT_ATTR_INFO_NUM)�Ĵ�С

//@@ 310050314  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_EVENTHEAD_COLUMUN_INFO_NUM)�Ĵ�С
//@@ 310050315  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_EVENTHEAD_COLUMUN_INFO_NUM)�Ĵ�С

//@@ 310050316  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_SWITICHIDMAPSWITCHINFO_INFO_NUM)�Ĵ�С
//@@ 310050317  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_SWITICHIDMAPSWITCHINFO_INFO_NUM)�Ĵ�С

//@@ 310050318  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_FIELD_CHECK_INFO_NUM)�Ĵ�С
//@@ 310050319  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_FIELD_CHECK_INFO_NUM)�Ĵ�С

//@@ 310050320  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_FIELD_MAP_INFO_NUM)�Ĵ�С
//@@ 310050321  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_FIELD_MAP_INFO_NUM)�Ĵ�С

//@@ 310050322  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_FIELD_MERGE_INFO_NUM)�Ĵ�С
//@@ 310050323  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_FIELD_MERGE_INFO_NUM)�Ĵ�С

//@@ 310050324  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_TAP3_FILE_INFO_NUM)�Ĵ�С
//@@ 310050326  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_TAP3_FILE_INFO_NUM)�Ĵ�С

//@@ 310050325  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_TAP3_BLOCK_INFO_NUM)�Ĵ�С
//@@ 310050327  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_TAP3_BLOCK_INFO_NUM)�Ĵ�С

//@@ 310050328  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_SEP_FILE_INFO_NUM)�Ĵ�С
//@@ 310050330  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_SEP_FILE_INFO_NUM)�Ĵ�С

//@@ 310050329  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_SEP_BLOCK_INFO_NUM)�Ĵ�С
//@@ 310050331  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_SEP_BLOCK_INFO_NUM)�Ĵ�С

//@@ 310050339  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_MERGE_RULE_INFO_NUM)�Ĵ�С
//@@ 310050340  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_MERGE_RULE_INFO_NUM)�Ĵ�С
//@@ 310050401  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_VISIT_AREAVODE_INFO_NUM)�Ĵ�С
//@@ 310050402  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_VISIT_AREAVODE_INFO_NUM)�Ĵ�С
//@@ 310050403  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_EDGEROAMRULE_INFO_NUM)�Ĵ�С
//@@ 310050404  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_EDGEROAMRULE_INFO_NUM)�Ĵ�С
//@@ 310050405  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_PNSEGBILLING_INFO_NUM)�Ĵ�С
//@@ 310050406  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_PNSEGBILLING_INFO_NUM)�Ĵ�С
//@@ 310050407  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_TRUNKTREENODE_INFO_NUM)�Ĵ�С
//@@ 310050408  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_TRUNKTREENODE_INFO_NUM)�Ĵ�С
//@@ 310050409  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_PNSEGBILLING_INFO_NUM)�Ĵ�С
//@@ 310050410  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_PNSEGBILLING_INFO_NUM)�Ĵ�С
//@@ 310050411  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_PNNOTBILLING_INFO_NUM)�Ĵ�С
//@@ 310050412  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_PNNOTBILLING_INFO_NUM)�Ĵ�С
//@@ 310050413  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_CARRIER_NUM)�Ĵ�С
//@@ 310050414  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_SERVICE_INFO2_NUM)�Ĵ�С
//@@ 310050415  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_SERVICE_INFO2_NUM)�Ĵ�С
//@@ 310050416  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_TRUNKBILLING_NUM)�Ĵ�С
//@@ 310050417  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_TRUNKBILLING_NUM)�Ĵ�С
//@@ 310050418  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_SPEC_TRUNK_PRODUCT_NUM)�Ĵ�С
//@@ 310050419  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_SWITCHITEMTYPE_NUM)�Ĵ�С
//@@ 310050420  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_SWITCHITEMTYPE_NUM)�Ĵ�С
//@@ 310050421  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_STR_LOCALAREACODE_NUM)�Ĵ�С
//@@ 310050423  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_TIMESTYPE_NUM)�Ĵ�С
//@@ 310050424  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_CITYINFO_NUM)�Ĵ�С
//@@ 310050425  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_CITYINFO_NUM)�Ĵ�С
//@@ 310050426  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_FILTER_RESULT_NUM)�Ĵ�С
//@@ 310050427  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_FILTER_RESULT_NUM)�Ĵ�С
//@@ 310050502  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_TRUNK_NUM)�Ĵ�С
//@@ 310050503  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_TRUNK_NUM)�Ĵ�С
//@@ 310050504  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_AREACODE_NUM)�Ĵ�С
//@@ 310050505  ��ȡ����ʧ��      B_PARAM_DEFINE����û������(LOADPARAMINFO, PARAM_AREACODE_NUM)�Ĵ�С
//@@ 310050506  �����ڴ�ʧ��      �����B_PARAM_DEFINE����(LOADPARAMINFO, PARAM_AREACODE_NUM)�Ĵ�С

//# MBC_APP_BILLFEE
#define MBC_BillFeeFilterRuleMgr  320010000
//@@ 320010001	�����ڴ�ʧ��	�����Ƿ����㹻���ڴ�
//@@ 320010002	�����ڴ�ʧ��	TIF_FEE_BILL_FILTER_RULE�������������ݣ���������������

#define MBC_CreditRuleMgr				350010000
//@@ 350010001	�ڴ�����ʧ��				(CreditRuleMgr::loadEvalIndex()--*ppEvalIndex = new EvalIndex)
//@@ 350010002	�ڴ�����ʧ��				(CreditRuleMgr::loadCreditFactor()--*ppCreditFactor = new CreditFactor)
//@@ 350010003	���ö����ؼ��㷽ʽ��������	���ö����ؼ��㷽ʽ����ָ��Ϊ���������ʶ�����洢���̣�
//@@ 350010004	�ڴ�����ʧ��				(CreditRuleMgr::loadEvalPlan--*ppEvalPlan = new EvalPlan)
//@@ 350010005	�ڴ�����ʧ��				(CreditRuleMgr::loadEvalPlan--*ppEvalRule = new EvalRule)
//@@ 350010006	�����������ô���			����ȷ������������
//@@ 350010007	�ڴ�����ʧ��				(CreditRuleMgr::loadEvalPlan--pEvalRuleStep = new EvalRuleStep)
//@@ 350010008	�����������ô���			����ȷ�����������裡
//@@ 350010009	�ƻ�ִ�з������ô���		�����ú��ʵ������ƻ�ִ�з�����
//@@ 350010010	�Ҳ��������ƻ�				���������ö�/���ֵ������ƻ���
//@@ 350010011	û������ȱʡ���ö�(δ��)	������Ԥ�����û������ö�ȱʡֵ��	
//@@ 350010012	û�����ü�������ֵ����Ŀ��	�����ü�������ֵ����Ŀ��(ʹ�á�,���ָ���)��
//@@ 350010013	δ����ȷ��ȡ����ֵ			������������	

#define MBC_SHMMixData     350060000
//@@ 350060001	�����ڴ�ʧ��			�������ռ䲻��,���������ô�С
//@@ 350060002	�����ڴ�ʧ��			���������ռ�����һ����������ʹ����
//@@ 350060003	�����ڴ�ʧ��			�������ͳ����趨�����ֵ

#define MBC_AcctItemHisOweMgr			350020000
//@@ 350020001	�޷����ӹ����ڴ�(ǰ3����Ƿ��)				������������Ƿ�����س���
//@@ 350020002	�����ڴ�(ǰ3����Ƿ��)������ 				������������Ƿ�����س���
//@@ 350020003	���乲���ڴ�(ǰ3����Ƿ��)ʱ����				�����Ƿ����㹻���ڴ�
//@@ 350020004	�����ڴ�(ǰ3����Ƿ��)������					����������
//@@ 350020005  ɾ���ڴ�ʧ��                                ȷ����ֹͣ���������ڴ����ؽ���

#define MBC_AverageOweGetter			350030000
//@@ 350030001	û��ѡ���ʺϵ���ʷ������	��ѡ����ʵ���ʷ������(1~3)��
//@@ 350030002  �ڴ��������ڲ�����          ����ȷ�����������ڣ�

#define MBC_CreditStore					350040000
//@@ 350040001	�ڴ�����ʧ��				(CreditCalInfo�ڴ����ʧ��)

#define MBC_CacheMgr					360010000
//@@ 360010001	�ڴ�����ʧ��			���ػ����������е��м仺����Ϣ��̫С����������������
//@@ 360010002	�ڴ�����ʧ��			���ػ����������е��м仺��������̫С����������������
//@@ 360010003	�ڴ�����ָ��Ϊ��		���ػ�����������ܵ����ڴ�����ָ��Ϊ�գ������������ú�����
//@@ 360010004	�ڴ��ۻ���ָ��Ϊ��		���ػ�����������ܵ����ڴ��ۻ���ָ��Ϊ�գ������������ú�����
//@@ 360010005	������ָ��Ϊ��		���ػ�����������ܵ���������Ϊ�գ������������ú�����
//@@ 360010006	���������ָ��Ϊ��		���ػ�����������ܵ������������Ϊ�գ������������ú�����
//@@ 360010007	������Ϣ����ָ��Ϊ��	���ػ�����������ܵ�����Ϣ���з���ָ��Ϊ�գ������������ú�����
//@@ 360010008	����IDȡֵ����			���۽���IDû�������ͱ��ػ��������������������ú�����

#define MBC_IpcKeyMgr    370010000  
//@@ 370010001  ��ȡ����ʧ��       ���ñ���ȱ������,����
//@@ 370010002  KEYֵ������Χ      KEYֵ��������B_IPCKEY_LIMIT���е�����,���޸�

//app_rent
#define MBC_GridRentDataMgr 380010000
//@@ 380010002	����������ڴ�ɾ��ʧ��	���˳�������ؽ��̣�����ǿ��ɾ��
//@@ 380010003	�����������ʧ��	����������ڴ�ľ��������·��乲���ڴ�

#define MBC_GridSetDataMgr 380020000
//@@ 380020002	�ײͷ��������ڴ�ɾ��ʧ��	���˳�������ؽ��̣�����ǿ��ɾ��
//@@ 380020003	�����ײͷ�����ʧ��	�ײͷ��������ڴ�ľ��������·��乲���ڴ�

#define MBC_GridProcessMgr 380030000
//@@ 380030001	�������û�����øý��̵���Ϣ	����grid_process���������ø�procss_id����Ϣ

#define MBC_GridEventMgr   380040000
//@@ 380040001	������ѻ����ײͷ�������ʵ��������ϵͳ���Ƹ���	���޸ĺ�GRID_MAKER_NUM�Ĵ�С


#define MBC_SEQCHECKDUP  110192000
//  110192001	ȡģ������������							����ȡģ����
//  110192002	���������ػ�����������						�����������ص���������
//  110192003	�����������Լ���������ID��δ���ù������� 	���������������������Լ�����ID��

#define MBC_NP 310060000
//@@ 310060001  ��ȡ�¿ռ�ʧ��
//@@ 310060002  ��ȡ����ʧ��

#define         MBC_ERRORCODE              330010000
//@@    330010001       �����������벻����,����MBC_SGW.h������
#endif
