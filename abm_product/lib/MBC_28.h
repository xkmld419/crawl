#ifndef __MBC_28_H__
#define __MBC_28_H__
//#ifdef __28_TEST_

//######################################################
//# 2.8�������
//# �����������: 3λģ����+2λ�����+4λ��Ϣ���
//# 1)ģ����ڽ��̻������ʼ��ʱ��ֵ��Log��
//# 2)��ALARMLOG��ͬʱ����������Լ�4λ��Ϣ��
//# 3)���ݸ���ʵ�������ѡ���Ƿ���ҪTHROW
//######################################################

//******************************************************
//Module_ID
//******************************************************
//----------------------------------------
//Application
//----------------------------------------
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
//#define MBC_APP_CfgExp	              223
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

//----------------------------------------
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
#endif //__MBC_28_H__

