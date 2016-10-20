#ifndef _HSSCONSTDEF_H_
#define _HSSCONSTDEF_H_

#include "StdAfx.h"

#define HSS_ENUM_NUM 30
#define HSS_FUNCTION_NUM 20

#define HSS_DATA_FORMAT_14 "YYYYMMDDHH24MISS"
#define HSS_DATA_FORMAT_8 "YYYYMMDD"

#define HSS_FIELD_TYPE_NUM 1
#define HSS_FIELD_TYPE_STRING 2
#define HSS_FIELD_TYPE_DATE 3
#define HSS_FIELD_TYPE_MOBILE 4
#define HSS_FIELD_TYPE_IDENTITY 5
#define HSS_FIELD_TYPE_POSTCODE 8
#define HSS_FIELD_TYPE_IP 6
#define HSS_FIELD_TYPE_EFFDATE 10
#define HSS_FIELD_TYPE_EXPDATE 11
#define HSS_FIELD_TYPE_PROVINCECODE 7
#define HSS_FIELD_TYPE_LATCODE 9

#define HSS_FUN_TYPE_DOMAIN 3
#define HSS_FUN_TYPE_TABLE 4
#define HSS_FUN_TYPE_FIELDTYPE 5
#define HSS_FUN_TYPE_FIELD 1
#define HSS_FUN_TYPE_AVP 2


#define HSS_FUN_CHECK_ISNULL 10
#define HSS_FUN_CHECK_ENUM 11
#define HSS_FUN_CHECK_FIELDTYPE 12
#define HSS_FUN_CHECK_NUMBER 28
#define HSS_FUN_CHECK_CHAR 29

#define HSS_FUN_CHECK_REFCUST 13
#define HSS_FUN_CHECK_REFSERV 14
#define HSS_FUN_CHECK_REFACCT 15
#define HSS_FUN_CHECK_BANK 16
#define HSS_FUN_CHECK_SERVMAX 17
#define HSS_FUN_CHECK_OFFERINSMAX 18

#define HSS_FUN_CHECK_OFFEROBJ_DATE 19
#define HSS_FUN_CHECK_OFFERINS_DATE 20
#define HSS_FUN_CHECK_OFFEROBJ_NUM 21
#define HSS_FUN_CHECK_OFFEROBJ_TYPE 22
#define HSS_FUN_CHECK_OFFERINS_ATTR 23
#define HSS_FUN_CHECK_OFFERINS_PARAM 24

#define HSS_FUN_CHECK_SERV_EXIST 81
#define HSS_FUN_CHECK_ACCT_EXIST 82

#define HSS_FUN_CHECK_GROUP_MEMBER_IN_SERV 25






#define HSS_FUN_CHECK_EFF_EXPDATE 30

#define HSS_FUN_CHECK_GROUP_MEMBER_EXIST 40
#define HSS_FUN_CHECK_OFFERPARAM_INS_DATE 50
#define HSS_FUN_CHECK_OFFER_PARAM_ENUM_EXIST 60
#define HSS_FUN_CHECK_OFFER_PARAM_EXIST 70
#define HSS_FUN_CHECK_PRODUCT_OFFER_EXIST 80

#define HSS_FUN_CHECK_SERV_EFF_DATE_CHECK 90
#define HSS_FUN_CHECK_PRODOBJINS_EFF_DATE_CHECK 91
#define HSS_FUN_CHECK_PRODPARAMINS_EFF_DATE_CHECK 92
#define HSS_FUN_CHECK_PRODOFFERINS_I_EXP_DATE_CHECK 93
#define HSS_FUN_CHECK_PRODOFFERINS_U_EXP_DATE_CHECK 94
#define HSS_FUN_CHECK_PRODOFFERINS_INNET_DATE_CHECK 95
#define HSS_FUN_CHECK_PRODOFFE_BILLMODE_CHECK 96
#define HSS_FUN_CHECK_PRODUCT_OFFER_RELA_A 97
#define HSS_FUN_CHECK_PRODUCT_OFFER_RELA_B 98

#define HSS_FUN_CHECK_TWO_PARAM_EQUAL 200
#define HSS_FUN_CHECK_KEY_IN_TABLE_EXIST 201
#define HSS_FUN_CHECK_KEY_IN_TABLE_NOTEXIST 202
#define HSS_FUN_CHECK_XML_FIELD_EXIST 210
#define HSS_FUN_GET_XML_FIELD_INFO 220

#define HSS_FUN_SUBSCRIBE_OFCS 100
#define HSS_FUN_SUBSCRIBE_OCS 110
#define HSS_FUN_SUBSCRIBE_ALL 120
#define HSS_FUN_SUBSCRIBE_MOBILE 130
#define HSS_FUN_SUBSCRIBE_PRODUCT_ATTR 140

const int CONVERT_UPPER = 1;
const int CONVERT_LOWER = 2;
const int POINT_IS_NULL =0;
const string STRING_IS_NULL = "";
const int INT_INIT = 0;
const char FILE_FILL_LEFT = 'L';
const char FILE_FILL_RIGHT = 'R';
/********************************************************
*                   �����������
*					��1--100��
*********************************************************/


#define HSSCMD_ORDER_SYNCHRO          	1   //�������ݽ���
#define HSSCMD_REGULATION_SYNCHRO      	2   //������ݽ���
#define HSSCMD_CUSTDOMAIN_SYNCHRO       3   //�����������ݽ���
#define HSSCMD_ACCTDOMAIN_SYNCHRO       4   //���������ݽ���
#define HSSCMD_PRODUCTDOMAIN_SYNCHRO    5   //��Ʒ�����ݽ���
//�ؼ������ݽ���������

#define HSSCMD_CTOC_MIN_SYNCHRO         20  //����MIN�������ļ�
#define HSSCMD_HCODE_SYNCHRO            21  //����H�뷢�����ļ�
#define HSSCMD_CTOG_IMSI_SYNCHRO        22  //���ղ�ͬ��ʽIMSI��������ļ�
#define HSSCMD_CTRY_SYNCHRO             23  //���չ�����Ϣ�������ļ�
#define HSSCMD_CTOC_CARRIER_SYNCHRO     24  //����C������������Ӫ�������ļ�
#define HSSCMD_CTOG_CARRIER_SYNCHRO     25  //���ղ�ͬ��ʽ����������Ӫ�̱������ļ�
#define HSSCMD_SPCODE_SYNCHRO           26  //����SP��������ļ�
#define HSSCMD_OBRST_SYNCHRO            27  //����ʡ�ʱ߽��վ��Ϣ�������ļ�
#define HSSCMD_BUSI_TYPE_SYNCHRO        28  //������ֵҵ�����ͱ������ļ�
#define HSSCMD_IISRATE_SYNCHRO          29  //���չ����������ý�����ʱ������ļ�
#define HSSCMD_NPCODE_SYNCHRO           30  //����Я��ת�������ļ�
#define HSSCMD_CELLINFO_SYNCHRO         31   //��վ���ݽ���
#define HSSCMD_BORDER_SYNCHRO      			32   //�������ݽ���



#define HSSCMD_ORDER_RELEASE          	61   //�������ݷ���
#define HSSCMD_REGULATION_RELEASE      	62   //������ݷ���
#define HSSCMD_CUSTDOMAIN_RELEASE       63   //�����������ݷ���
#define HSSCMD_ACCTDOMAIN_RELEASE       64   //���������ݷ���
#define HSSCMD_PRODUCTDOMAIN_RELEASE    65   //��Ʒ�����ݷ���

//�ؼ��������·�������
#define HSSCMD_CTOC_MIN_RELEASE         40  //MIN���·������ļ�
#define HSSCMD_HCODE_RELEASE            41  //H���·������ļ�
#define HSSCMD_CTOG_IMSI_RELEASE        42  //��ͬ��ʽIMSI����·������ļ�
#define HSSCMD_CTRY_RELEASE             43  //������Ϣ���·������ļ�
#define HSSCMD_CTOC_CARRIER_RELEASE     44  //C������������Ӫ���·������ļ�
#define HSSCMD_CTOG_CARRIER_RELEASE     45  //��ͬ��ʽ����������Ӫ�̱��·������ļ�
#define HSSCMD_SPCODE_RELEASE           46  //SP����·������ļ�
#define HSSCMD_OBRST_RELEASE            47  //ʡ�ʱ߽��վ��Ϣ���·������ļ�
#define HSSCMD_BUSI_TYPE_RELEASE        48  //��ֵҵ�����ͱ��·������ļ�
#define HSSCMD_IISRATE_RELEASE          49  //�����������ý�����ʱ��·������ļ�
#define HSSCMD_NPCODE_RELEASE           50  //Я��ת���·������ļ�
#define HSSCMD_CELLINFO_RELEASE         51  //����CELL_INFO

/********************************************************
*                   ����ͨ���������
*					��101--150����ѯʹ��
*					��151--256������
*********************************************************/

#define HSSCMD_SERV_QUERY            		101   //�û���Ϣ��ѯ
#define HSSCMD_SERVPRODUCT_QUERY        102   //������Ʒ��Ϣ��ѯ
#define HSSCMD_ACCT_QUERY         			103   //�ʻ���Ϣ��ѯ
#define HSSCMD_CUST_QUERY       				104   //�ͻ���Ϣ��ѯ
#define HSSCMD_ORDERRESOURCE_QUERY      105   //����Ʒ��Ϣ��ѯ
#define HSSCMD_GROUPMEMBER_QUERY        106   //ͨ��Ⱥʵ������Ⱥ��Ա
#define HSSCMD_GROUP_QUERY 							107  //ͨ��Ⱥ��Ա����Ⱥ��Ϣ
#define HSSCMD_EXISTHEAD_QUERY					108  //�����ͷ��ѯ
#define HSSCMD_HOMEBYACCNBR_QUERY       110   //������ѯ�������룩
#define HSSCMD_HOMEBYIMSI_QUERY     		111   //������ѯ����IMSI��
#define HSSCMD_HCODE_QUERY           		112   //H���ѯ
#define HSSCMD_LOCALHEAD_QUERY      		113   //���غ�ͷ���ѯ
#define HSSCMD_BORDERROAM_QUERY         114   //�����ж�
#define HSSCMD_IBRSTCODE_QUERY     			115   //	C������������Ӫ��
#define HSSCMD_IMSICODE_QUERY           116   //��ͬ��ʽIMSI���ѯ
#define HSSCMD_MINCODE_QUERY      			117   //MIN���ѯ
#define HSSCMD_NPCODE_QUERY           	118   //Я��ת���ж�
#define HSSCMD_SPCODE_QUERY      				119   //SP�ж�
#define HSSCMD_PRODUCTOFFER_QUERY      	120   //��ֵҵ���ѯ
#define HSSCMD_DATAREPORT_QUERY         121   //ҵ����Ԫ�ṩ������Ϣ��ѯ�ӿ�




#define HSSCMD_OFFERDOMAIN_REGISTER     153   //����Ʒע��
#define HSSCMD_OFFERDOMAIN_APPROVE      154   //����Ʒ���

#define PRODUCT_COMMAND_TYPE_EFF				1			//����Ʒ��ϵ��Чʱ����
#define PRODUCT_ADD_TYPE_EXP 						2			//��װ����ƷʧЧʱ����
#define PRODUCT_MODIFY_TYPE_EXP 				3			//��װ����ƷʧЧʱ����
#define PRODUCT_ONLINE_TIME						4			//online time

#define PRODUCT_ACTION_NEW_SUB 					1			//���û�����
#define PRODUCT_ACTION_OLD_SUB 					2			//���û�����
#define PRODUCT_ACTION_UNSUB 						3			//�˶�
#define PRODUCT_ACTION_CHG_MEM 					4			//��Ա���
#define PRODUCT_ACTION_CHG_PARAM 				5			//�������

#define TIME_UNIT_HOUR 									1			//Сʱ
#define TIME_UNIT_DAY 									2			//��
#define TIME_UNIT_WEEK 									3			//��
#define TIME_UNIT_MONTH 								4			//��
#define TIME_UNIT_YEAR 									5			//��


#endif

