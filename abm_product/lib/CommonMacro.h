/*VER: 12*/ 
#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#include "../../etc/LocalDefine.h"

//#include "../../config/LocalDefine.h"
//##
//##�����ĺ궨��
//##
#define NUMBER                  2 //1
#define STRING                  3 //2
#define ARRAY_INT       4

#define DEFAULT_CYCLE_TYPE      0       //Ĭ�ϵ���������
#define FRIEND_ZONE_ID          2       //������������

#define PAY_MEASURE_MONEY   2   //��Ǯ�����

#define SERV_CYCLE_ACCT     0   //�û���������ĩ�Ż�
#define COMPLEX_CYCLE_ACCT  1   //�������͵�����ĩ�Ż�



//## ID_BILLING_MODE: ��Ϊһ���û����ԣ� �����Ʒ�ģʽ
#ifdef DEF_HEBEI

int const ID_BILLING_MODE = 162;

#else

int const ID_BILLING_MODE = 403;

#endif


//##
//##�Ʒ�ģ����
//##
#include "MBC.h"

#define DATA_MONTH_TYPE_PLAN    9

//##ʵ����������
#define SERV_INSTANCE_INT       10
#define OFFER_INSTANCE_INT      20
#define GENERAL_INSTANCE_INT    1000

#define PRODUCT_INT             15      //��������Ϊ��Ʒ��Ӧ�ĺ�̨���
#define OFFER_INT               25      //��������Ϊ��Ʒ��Ӧ�ĺ�̨���
#define GENERAL_INT             1000

//##�������Ͷ��壨����������ظ����Ժ���ȡ������ģ�
#define SERV_INSTANCE_TYPE      10      //����Ʒʵ������
#define PRODUCT_TYPE            10      //��Ʒ����
#define OFFER_INSTANCE_TYPE     20      //��Ʒʵ������
#define OFFER_TYPE              25      //��Ʒ����
#define CUST_TYPE               30      //�ͻ�����
#define ACCT_TYPE               35      //�˻�����
#define SERV_GROUP_TYPE         50      //�û�Ⱥ����
#define GENERAL_TYPE            1000    //ͨ������

//##�������ڵ�����
#define ABSOLUTE_TYPE           1       //��������
#define RELATIVE_YEAR_TYPE      2       //����ײ��������ڵ�����
#define RELATIVE_MONTH_TYPE     3       //����ײ��������ڵ�����
#define RELATIVE_DAY_TYPE       4       //����ײ��������ڵ�����
#define MIX_YEAR_TYPE           5       //����ײ��������ڵ��������߾������ڣ�������һ
#define MIX_MONTH_TYPE          6       //����ײ��������ڵ��������߾������ڣ�������һ
#define MIX_DAY_TYPE            7       //����ײ��������ڵ��������߾������ڣ�������һ
#define MOD_MONTH_TYPE          8
#define MOD_CYCLE_TYPE          9
#define ABSOLUTE_NOYEAR_TYPE    10      //�������ڲ�������
#define RELATIVE_CYCLE_TYPE     11      //����ײ���Ч������
#define MOD_NORMAL_MONTH_TYPE   12  //��Ȼ��

#define DB_LINK Environment::getDBConn()  //���ݿ����Ӻ궨��

//##
//##�¼���
//##
#define  MAX_ATTR_VAL_LEN       31              //������Ʒ����ȡֵ�ַ�����󳤶�
#define  MAX_ATTR_EFF_DATE_LEN  15              //������Ʒ����ȡֵ��Чʱ�䳤��
#define  MAX_ATTR_EXP_DATE_LEN  15              //������Ʒ����ȡֵʧЧʱ�䳤��
#define  MAX_BILLING_AREA_CODE_LEN      11      //�Ʒѷ�����������󳤶�
#define  MAX_BILLING_NBR_LEN    41              //�Ʒѷ�������󳤶�
#define  MAX_BILLING_VISIT_AREA_CODE    11      //�Ʒѷ����õ�������󳤶�
#define  MAX_CALLING_AREA_CODE_LEN      11      //����������󳤶�
#define  MAX_CALLING_NBR_LEN    33              //���к�����󳤶�
#define  MAX_ORG_CALLING_NBR_LEN        33      //ԭʼ���к�����󳤶�
#define  MAX_CALLED_AREA_CODE_LEN       11      //����������󳤶�
#define  MAX_CALLED_NBR_LEN     33              //���к�����󳤶�
#define  MAX_ORG_CALLED_NBR_LEN 33              //ԭʼ���к�����󳤶�
#define  MAX_THIRD_PARTY_AREA_CODE_LEN  11      //������������󳤶�
#define  MAX_THIRD_PARTY_NBR_LEN        33      //������������󳤶�
#define  MAX_START_DATE_LEN     18              //��ʼʱ����󳤶�
#define  MAX_END_DATE_LEN       18              //����ʱ����󳤶�
#define  MAX_TRUNK_IN_LEN       13              //���м̺�����󳤶�
#define  MAX_TRUNK_OUT_LEN      13              //���м̺�����󳤶�
#define  MAX_EVENT_STATE_LEN    4               //�¼�״̬��󳤶�
#define  MAX_CREATED_DATE_LEN   15              //�¼�����ʱ����󳤶�
#define  MAX_STATE_DATE_LEN     15              //�¼�״̬�޸�ʱ����󳤶�
#define  MAX_CARD_ID_LEN        41              //������󳤶�
#define  MAX_OFFER_NUM  5                       //��Ʒ��ʶ�������
#define  MAX_BILLING_RECORD_LEN 101             //��ѹ��̼�¼��󳤶�
#define  MAX_ACCOUNT_AREA_CODE_LEN      11      //�����ʺ�������󳤶�
#define  MAX_ACCOUNT_NBR_LEN    33              //�����ʺź�����󳤶�

#ifndef  DEF_HAINAN
#define  MAX_NAS_IP_LEN 17                      //����IP��ַ��󳤶�
#else
#define  MAX_NAS_IP_LEN 32                      //����IP��ַ��󳤶�---������Ҫ���ӳ���
#endif

#define  MAX_CYCLE_BEGIN_DATE_LEN       15      //������ʼʱ����󳤶�
#define  MAX_CYCLE_END_DATE_LEN 15              //���ڽ���ʱ����󳤶�
//#define  MAX_DAFAULT_STRING_NUM       10      //�������ַ��������ֶ��������
#define  MAX_BYTES_PER_STRING   33              //ÿ���������ַ��������ֶ���󳤶�
//#define  MAX_DEFAULT_INT_NUM  10              //�������������ֶ��������
#define  MAX_SERV_PROD_ATTR_NUM 2               //������Ʒ��Ϣ��Ŀ
#define  MAX_ORG_CHARGE_NUM 2       //ԭʼ����
#define  MAX_EXT_FIELD_NUM 10                   //�¼���չ���Ե������Ŀ
#define  MAX_FILE_NAME_LEN 200      //�ļ�����󳤶�
#define  MAX_MSC_LEN    16              //�ƶ���������ʶ��󳤶�
//����У��ismpҵ����255λ�����ֶ�
#define MAX_ACCESS_POINT_NAME_LEN 260  //�����������󳤶�

//7��19���շ�����6λCELL_ID,��ͨ����ģ����8λ
#define  MAX_LAC_LEN    9               //С���ű�ʶ��󳤶�
#define  MAX_CELLID_LEN         9               //��վ����󳤶�
//#define  MAX_LAC_LEN  6               //С���ű�ʶ��󳤶�
//#define  MAX_CELLID_LEN               6               //��վ����󳤶�
#define  MAX_ROMING_NBR_LEN             31              //��̬���κ���󳤶�
#define  MAX_CALLING_VISIT_AREA_CODE_LEN         11  //���е��õ�������󳤶�
#define  MAX_CALLED_VISIT_AREA_CODE_LEN         11              //���е��õ�������󳤶�
#define  MAX_IMSI_LEN   21              //IMSI��󳤶�
#define  MAX_IMEI_LEN   21              //IMEI��󳤶�
#define  MAX_ORG_SEQ_LEN 32             // ����ԭʼ���к���󳤶�
#define  MAX_RESERVED_FIELD_NUM 5                       //�¼��������Ե������Ŀ
#define  MAX_RESERVED_FIELD_LEN 41                      //�¼����������ֶε���󳤶�
#define  MAX_GATE_WAY_LEN       11    //���ر��볤��
#define  MAX_RP_STR_LEN         16    //��վ���볤��
#define  MAX_SESSION_ID_LEN     56    //SessionID ����
#define  MAX_EXT_STRING_LEN     31    //sExtendValue ����󳤶�
#define  MAX_DOMAIN_NAME_LEN    21    //��������
#define  MAX_BIND_ATTR_LEN      101   //�����Գ���
#define  MAX_AREA_NO_LEN        21    //�û�����
#define MAX_ACCU_RECORD_LEN     200   //�ۻ����켣����2010-09-08
#define  DUP_CHK_MAX_CALLING_LEN 41   //������ ���к����򳤶�
#define  DUP_CHK_MAX_CALLED_LEN 41    //������ ���к����򳤶�
#define DUP_CHK_MAX_SEQKEY_LEN 16
#define DUP_CHK_MAX_SEQID_LEN  16


//���Ա�ʶ
#define   ID_EVENT_ID               1           //���ڱ�ʶ                     
#define   ID_BILLING_CYCLE_ID        2          //�¼���ʶ                     
#define   ID_EVENT_TYPE_ID                   3          //�¼����ͱ�ʶ                 
#define   ID_BILLING_AREA_CODE               4          //�Ʒѷ�����                   
#define   ID_BILLING_NBR                    5           //�Ʒѷ�����                   
#define   ID_BILLING_VISIT_AREA_CODE          6         //�Ʒѷ����õ�����             
#define   ID_CALLING_AREA_CODE               7          //���з�����                   
#define   ID_CALLING_NBR                    8           //���з�����                   
#define   ID_ORG_CALLING_NBR                 9          //ԭʼ�����з�����             
#define   ID_CALLING_SP_TYPE_ID               10        //������Ӫ�����ͱ�ʶ           
#define   ID_CALLING_SERVICE_TYPE_ID          11        //����ҵ�����ͱ�ʶ             
//#define   ID_CALLING_EXCHANGE_ID             12         //���о����ʶ                 
#define   ID_CALLED_AREA_CODE                13         //��������                     
#define   ID_CALLED_NBR                     14          //���к���                     
#define   ID_ORG_CALLED_NBR                  15         //ԭʼ�ı��з�����             
#define   ID_CALLED_SP_TYPE_ID                16        //������Ӫ�����ͱ�ʶ           
#define   ID_CALLED_SERVICE_TYPE_ID           17        //����ҵ�����ͱ�ʶ             
//#define   ID_CALLED_EXCHANGE_ID              18         //���о����ʶ                 
#define   ID_THIRD_PARTY_AREA_CODE            19        //��������������               
#define   ID_THIRD_PARTY_NBR                 20         //����������                   
#define   ID_START_DATE                     21          //�¼���ʼʱ��                 
#define   ID_END_DATE                       22          //�¼�����ʱ��                 
#define   ID_DURATION                      23           //�¼�ʱ��                     
#define   ID_SWITCH_ID                      24          //��������ʶ                   
#define   ID_TRUNK_IN                       25           //���м̺�                     
#define   ID_TRUNK_IN_TYPE_ID                 26         //���м����ͱ�ʶ               
#define   ID_TRUNK_OUT                      27          //���м̺�                     
#define   ID_TRUNK_OUT_TYPE_ID                28        //���м�����                   
#define   ID_FILE_ID                        29          //�ļ���ʶ                     
#define   ID_EVENT_STATE                    30          //�¼�״̬                     
#define   ID_CREATED_DATE                   31          //�¼�����Ʒ�ϵͳʱ��         
#define   ID_STATE_DATE                     32          //�¼�ȷ��״̬��ʱ��           
#define   ID_CARD_ID                        33          //����ʶ                       
#define   ID_BILLING_TYPE_ID                 34         //�Ʒ����ͱ�ʶ                 
#define   ID_PARTER_ID                      35          //��������ʶ                   
#define   ID_SERV_ID                        36          //�û���ʶ                     
#define   ID_CUST_ID                        37          //�ͻ���ʶ                     
#define   ID_PROD_ID                     38             //��Ʒ��ʶ                     
#define   ID_SERV_PROD_ID                 39            //������Ʒ��ʶ                 
#define   ID_PROD_BUNDLE_ID               40            //��Ʒ����ʶ                   
#define   ID_OFFERS1                     41             //��Ʒ��ʶ1                    
#define   ID_OFFERS2                     42             //��Ʒ��ʶ1                    
#define   ID_OFFERS3                     43             //��Ʒ��ʶ1                    
#define   ID_OFFERS4                     44             //��Ʒ��ʶ1                    
#define   ID_OFFERS5                     45             //��Ʒ��ʶ1                    
#define   ID_BILLING_RECORD                 46          //��ѹ��̼�¼                 
#define   ID_ERROR_ID                       47          //����ű�ʶ                   
#define   ID_ACCOUNT_AREA_CODE            48         //����ҵ���ʺ����ڵ�������     
#define   ID_ACCOUNT_NBR                 49          //����ҵ���ʺ�                 
#define   ID_NAS_IP                      50          //�������IP                   
#define   ID_NAS_PORT_ID                  51         //�������˿ڱ�ʶ             
#define   ID_RECV_BYTES                  52          //�����ֽ���                   
#define   ID_SEND_BYTES                  53          //�����ֽ���                   
#define   ID_RECV_PACKETS                54          //���հ���                     
#define   ID_SEND_PACKETS                55          //���Ͱ���                     
#define   ID_NEGOTIATED_QOS              56          //��������                     
#define   ID_CYCLE_BEGIN_DATE             57         //������ʼʱ��                 
#define   ID_CYCLE_END_DATE               58         //���ڽ���ʱ��                 
#define   ID_EXT_USED_NUM                 59         //��չ�����ֶ�ʹ�ø���         
#define   ID_BUSINESS_KEY               60           //ҵ���                       
#define   ID_SER_PROD_ATTR_USED_NUM         61       // ������Ʒ����ʵ��ʹ�õ���Ŀ  
#define   ID_CHARGE_INFO_ORG_CHARGE1    62           //��1���������ԭʼ����        
#define   ID_CHARGE_INFO_STD_CHARGE1    63           //��1��������ı�׼����        
#define   ID_CHARGE_INFO_CHARGE1       64            //��1������������շ���        
#define   ID_CHARGE_INFO_ACCT_ITEM_TYPE_ID1       65         //��1�����������Ŀ���ͱ�ʶ    
#define   ID_CHARGE_INFO_DISCOUNT_ACCT_ITEM_TYPE_ID1      66        //��1����������Ż���Ŀ���ͱ�ʶ
#define   ID_CHARGE_INFO_ORG_CHARGE2      67           //��2���������ԭʼ����        
#define   ID_CHARGE_INFO_STD_CHARGE2      68           //��2��������ı�׼����        
#define   ID_CHARGE_INFO_CHARGE2         69            //��2������������շ���        
#define   ID_CHARGE_INFO_ACCT_ITEM_TYPE_ID2     70         //��2�����������Ŀ���ͱ�ʶ    
#define   ID_CHARGE_INFO_DISCOUNT_ACCT_ITEM_TYPE_ID2    71        //��2����������Ż���Ŀ���ͱ�ʶ
#define   ID_SERV_PROD_ATTR_ID1             72               //��1��������Ʒ��Ϣ�����Ա�ʶ  
#define   ID_SERV_PROD_ATTR_VAL1            73               //��1��������Ʒ��Ϣ������ȡֵ  
#define   ID_SERV_PROD_ATTR_EFF_DATE1        74              //��1��������Ʒ��Ϣ����Чʱ��  
#define   ID_SERV_PROD_ATTR_EXP_DATE1        75              //��1��������Ʒ��Ϣ��ʧЧʱ��  
#define   ID_SERV_PROD_ATTR_ID2             76               //��2��������Ʒ��Ϣ�����Ա�ʶ  
#define   ID_SERV_PROD_ATTR_VAL2            77               //��2��������Ʒ��Ϣ������ȡֵ  
#define   ID_SERV_PROD_ATTR_EFF_DATE2        78              //��2��������Ʒ��Ϣ����Чʱ��  
#define   ID_SERV_PROD_ATTR_EXP_DATE2        79              //��2��������Ʒ��Ϣ��ʧЧʱ��  
#define   ID_CALL_TYPE_ID      80            //�������ͱ�ʶ
#define   ID_BILLING_ORG_ID 81    //�Ʒѷ���Ӫ��֯��ʶ
#define   ID_BILL_FLOW_ID 82       //�Ʒ����̱�ʶ
#define   ID_CHARGE_INFO_BILL_MEASURE1  83       //�Ʒ�����ʶ
#define   ID_CHARGE_INFO_BILL_MEASURE2  84       //�Ʒ�����ʶ
#define   ID_MSC        207                                                     //�ƶ���������ʶ
#define   ID_LAC        208                                                     //С���ű�ʶ
#define   ID_CELLID      209                                            //��վ�ű�ʶ
#define   ID_ROMING_NBR  210                                            //��̬���κű�ʶ
#define   ID_NO_BILL_FLAG  211                                  //��ѱ�־��ʶ
#define   ID_CALLING_VISIT_AREA_CODE  212               //���е��õ����ű�ʶ
#define   ID_CALLED_VISIT_AREA_CODE  213                //���е��õ����ű�ʶ
#define   ID_IMSI         214                                           //IMSI��ʶ
#define   ID_IMEI  215                                                  //IMEI��ʶ
#define   ID_EVENT                      402     //�¼� �����ۻ��¼���Ŀ

//##
//##����������ģ��
//##
#define CONDITION_HASH_VALUE    2000
#define OPERATION_HASH_VALUE    2000
#define MAX_STRING_VALUE_LEN    80
#define MAX_SQL_WHERE_LEN       200

//##
//## ���̿��
//##
#define MAX_OUT_PROCESS_NUM     128     //һ���ļ����¼���෢�͸����ٸ�����

//##
//##����
//##
#define MAX_BREAKPOINT_NUM          200
#define MAX_EVENTSECTION_NUM    4096    //һ���¼����ֳɶ��ٶΣ����ж����ֶ���֮�ͣ�



#define UNKNOWN_RELATION    0
#define COVER_RELATION          1       //������ϵĸ��ǹ�ϵ
#define ASSOC_RELATION          2       //������ϵĹ�����ϵ
#define MIN_RELATION        3   //������ϵ����Ź�ϵ
#define SWAP_RELATION       4   //������ϵķ�ת��ϵ
#define IGNORE_RELATION     5   //������ϵĺ��Թ�ϵ
#define CUTORREP_RELATION   6   //������ϵ��ۻ���ϵ


//##
//##�û����ϲ���ģ��
//##
#define MAX_OFFERS                      5       //һ������Ʒ������ڶ��ٸ���Ʒ
#define MAX_OFFERINSTANCES          5   //............................ʵ��
#define MAX_PRICINGPLANS            64  //һ������Ʒ����Ӧ���ٸ����ۼƻ�
#define AVG_PRICINGPLANS            4   //ƽ��һ������Ʒ��Ӧ�ĸ��Ի��Ķ��ۼƻ���
#define MAX_ACCTITEM_PERGROUP   4096    //һ����Ŀ�������ٸ���Ŀ
#define MAX_EVENTTYPE_PERGROUP  1024    //
#define MAX_OFFER_ATTRS         30   //һ����Ʒʵ������ж��ٸ�����
#define MAX_OFFER_MEMBERS       8192 //һ����Ʒʵ������ж��ٸ���Ա

//##
//## PricingEngine Buffer
//## for new Version

#define MAX_OFFER_INSTANCE_NUMBER      512 //һ������������ڶ��ٸ���Ʒʵ��,20071128��128��Ϊ512
#define MAX_PRICING_COMBINE_NUMBER    1024 //������Ʒʵ�����������Ķ��������,20071128��512��Ϊ1024
#define MAX_TARIFF_NUMBER             2048 //����������Ϻ��е�Tariff��������
#define MAX_DISCOUNT_NUMBER           2048 //����Discount��
#define MAX_PERCENT_NUMBER            2048 //�����ۿ���
#define MAX_ACCUMULATE_NUMBER         2048 //�����ۻ���
#define MAX_PAY_ITEM_NUMBER         131072 //���۽�����������
#define MAX_PRESENT_ITEM_NUMBER       2048 //���͵��������
#define MAX_ACCUMULATE_ITEM_NUMBER    2048 //���۽�����ۻ������������
#define MAX_CAL_MEASURE_NUMBER         512 //�嵥���۵����Ķ�������
#define MAX_EVENTPART_NUMBER          4096 //���ķֶ�����
#define MAX_ACCU_PER_STRATEGY            4 //һ�����۲����������õ��ۻ�������

#define MAX_DISCT_APPLYER_NUMBER      1024 //������������Ʒʵ�������ķ�̯��������
#define MAX_DISCT_APPLYER_OBJ_NUMBER  2048 //���ķ�̯Ӧ�ö�������
#define MAX_PRICING_SECTION_NUMBER    5120 //���Ķ��۶�������
#define MAX_CONDITION_POINT_NUMBER    3072 //��������ֵ����
#define MAX_MEASURE_NUMBER            2048 //���Ķ�������
#define MAX_FILTER_COND_NUMBER        3072 //���Ĺ��˶�������

//add by xn 2010-6-25
#define MAX_APP_PARAM_LEN                                                       256     // Ӧ�ý������Я����������
//##
//##
//## Item_Source_Id �ĺ���
#define ITEM_SOURCE_MINUS        7 //����
#define ITEM_SOURCE_OUT_STD      8 //�ײ����굥����


#define ITEM_SOURCE_STD         11 //�굥����
#define ITEM_SOURCE_DISCOUNT    12 //�굥�Ż�---��������ʱ����
#define ITEM_SOURCE_ADD_ACCT    13 //�����˵��Ż�
#define ITEM_SOURCE_BT_ACCT     14 //ȫ���˵��Żݺ�����ĩ�Ż�
#define ITEM_SOURCE_CYCLE_ACCT  15 //����ĩ�Ż�
#define IVPN_ITEM_SOURCE_ID             16 //IVPN�����ʵ�
#define ITEM_SOURCE_ADJUST       9 //�Ż�ǰ����
#define ITEM_SOURCE_NODISCT     10 //�������Żݵĵ���
#define ITEM_SOURCE_IVPN_ADJUST 19 //IVPN����
#define ITEM_SOURCE_IVPN_PAYMENT 20 //IVPN���Ŵ����˵�
#define IVPN_ITEM_SOURCE_PROV    21 //IVPN�����ʵ�_ʡ��
#define IVPN_ITEM_SOURCE_LOCAL    22 //IVPN�����ʵ�_������



//##
//##
//## Item_Class_Id�ĺ���
#define ITEM_CLASS_BLOCKED_FLAG  1 //������ʶ

//##
//##
//##
#define MAX_EVENT_PERSERV       2048    //max cycle_fee_event each serv


//##
//##Ԥ����
//##
//�����ʽ�����¼�����ļ���·��
#define ERR_FULL_PATH "/home/bill/TIBS_HOME/src/app_prep/ErrPath/"
#define OUTPUT_FULL_PATH "/home/bill/TIBS_HOME/src/app_prep/OutputPath/"

#define CALLTYPE_MOC 1
#define CALLTYPE_MTC 2
#define CALLTYPE_FORWARD 100
#define CALLTYPE_FWD 3

const int  EVENT_ID_INCREMENT = 1000;

//const int  ROAMTYPE_ROAM_BORD = 1;     //��������-�߽�����
//const int  ROAMTYPE_ROAM_SAME = 2;     //��������-ͬ������
//const int  ROAMTYPE_ROAM = 3;          //��������-����(����)
//const int  ROAMTYPE_ROAM_PROV = 30;    //��������-ʡ������
//const int  ROAMTYPE_ROAM_NATION = 31;  //��������-��������
//const int  ROAMTYPE_ROAM_INTER = 32;   //��������-��������
//const int  ROAMTYPE_NOT_ROAM = 4;      //��������-������

const int  ROAMTYPE_NOT_ROAM = 0;         //��������-������
const int  ROAMTYPE_ROAM_PROV = 1;        //��������-ʡ������
const int  ROAMTYPE_ROAM_NATION_IN = 2;   //��������-��������
const int  ROAMTYPE_ROAM_NATION_OUT = 3;  //��������-��������
const int  ROAMTYPE_ROAM_INTER_IN = 4;    //��������-��������
const int  ROAMTYPE_ROAM_INTER_OUT = 5;   //��������-��������
const int  ROAMTYPE_ROAM_BORD = 6;        //��������-�߽�����
const int  ROAMTYPE_ROAM_SAME = 7;        //��������-ͬ������
const int  ROAMTYPE_ROAM_BORD_EX= 8 ;

/* xueqt 20080523       
const int  LONGTYPE_LONG = 1;          //��;����-��;(����)
*/
const int  LONGTYPE_LONG = 1;          //��;����-��;(����)
const int  LONGTYPE_PROV = 10;         //��;����-ʡ��
const int  LONGTYPE_NATION = 11;       //��;����-����
const int  LONGTYPE_INTER = 12;        //��;����-����
const int  LONGTYPE_THIRT = 13;        //��;����-13��
const int  LONGTYPE_AMERC = 14;        //��;����-����
const int  LONGTYPE_KONG = 15;         //��;����-�۰�̨
const int  LONGTYPE_LATN = 2;          //��;����-�ǳ�;(�л�)
const int  LONGTYPE_LATN_XK = 151; //�۰�̨-��۳�;
const int  LONGTYPE_LATN_AM = 152; //�۰�̨-���ų�;
const int  LONGTYPE_LATN_TW = 153; //�۰�̨-̨�峤;
const int  LONGTYPE_LATN_HS = 16;  //��������
const int  LONGTYPE_LATN_PROV_NATION = 17;  //ʡ�ڳ�;+���ڳ�;
const int  LONGTYPE_LATN_NATION_PROV = 18;  //���ڳ�;+ʡ�ڳ�;
const int  LONGTYPE_LATN_PROV_PROV = 19;  //ʡ�ڳ�;+ʡ�ڳ�;
const int  LONGTYPE_LATN_NATION_NATION = 19;  //���ڳ�;+���ڳ�;



const int LONGTYPE_LATN_EX = 1000;          //C������


const int  LONG_GROUPTYPE_PROV = 10;         //��;������-ʡ��
const int  LONG_GROUPTYPE_NATION = 20;       //��;������-����
const int  LONG_GROUPTYPE_INTER = 30;        //��;������-����
const int  LONG_GROUPTYPE_THIRT = 31;        //��;������-13��
const int  LONG_GROUPTYPE_AMERC = 32;        //��;������-����
const int  LONG_GROUPTYPE_KONG = 40;         //��;������-�۰�̨
const int  LONG_GROUPTYPE_XK = 401; //��;������-��
const int  LONG_GROUPTYPE_AM = 402; //��;������-��
const int  LONG_GROUPTYPE_TW = 403; //��;������-̨
const int  LONG_GROUPTYPE_HS = 50; //��������




const int  FORWARDTYPE_UNCOND = 33; //��������ת
const int  FORWARDTYPE_DEFLECTION = 36; //��תƫ��
const int  FORWARDTYPE_COND = 40; //��������ת
const int  FORWARDTYPE_BUSY = 41; //��æ��ת
const int  FORWARDTYPE_NOREPLAY = 42; //��Ӧ���ת
const int  FORWARDTYPE_NOTREACH = 43; //���ɼ���ת


const int  FORWARDTYPE_UNCOND_EX = 1;//��������ת
const int  BUSINESSKEY_FORWARDTYPE_UNCOND = 1001;  //����Ҫ����2�γ�;
const int  BUSINESSKEY_FORWARDTYPE_COND = 1002;    //��Ҫ����2�γ�;  

const int  D_M_ACCNBR_LEN = 8;   //�̻�������󳤶�
const int  D_M_MOBNBR_LEN = 11;  //�ֻ�������󳤶�

const int  MAX_FORMAT_RECORD_LEN = 8000; //��¼�������̵��ַ�������

#define MAX_BLOCK_SIZE 10000000

const int MAX_SPFLAG_LEN = 80;  //ISMP���SP��ʶ������

//##

//�����Է�������
#define CYCLE_FEE_EVENT_FILE_PATH "/home/bill/TIBS_HOME/src/app_cyclefee/FilePath/"
//#define MAX_FILE_NAME_LEN 200
#define MAX_PATH_NAME_LEN 200
#define MAX_FULL_FILE_NAME_LEN 300
#define RETRY_TIMES 3
#define EVENTS_PER_FILE 50000

//ERROR_ID
#define ERROR_NO_OWNER          2000    //�Ҳ����û�����
#define ERROR_NO_CYCLE          2001    //�Ҳ�����������
#define ERROR_NO_SERVID         4000    //���۵�ʱ�����servid���û�����ʧ��
#define ERROR_NO_COMBINE        4001    //���۵�ʱ���Ҳ����������
#define ERROR_NO_SECTION        4002    //�ֶβ�ȫ



//#######################__�澯���붨��__######################## add by yangks.

/*
bool Pprintf(int iAlertCode,bool bInsertDB,int iFileID,char *fmt,...)

���Ӧ�ý�����־���澯��Ϣ��ǰ̨���Ƚ���
        iAlertCode==0ʱ ��ʾ�Ǹ澯��Ϣ
                (AlertCode �ڱ�BILL.WF_ALERT_DEFINE ���н���,�澯�Ĵ�������
                ��ģ�����Ӹ澯����ʱ,��Ҫ�����ͷ�ļ������ݿ����������Ӧ�ļ�¼)
        bInsertDB: �Ƿ���Ҫ��澯��־��(wf_alert_log)
        iFileID  : ��ǰ�������ļ�ID. ������ʱ��0
        fmt      : ��ʽ�����
*/

//## 
//## �����澯����
//## ��Χ: 0-999
const int ALERT_SIG_ERR = 100;  //signal���������źŴ���ʱʧ��
//##


//## 
//## �����Է����¼��澯���� 
//## ��Χ: 1000-1999

//##


//## 
//## Ԥ�����澯����
//## ��Χ: 2000-2999
const int  ALERT_RCVMODE_ERR  = 2001;
const int  ALERT_NO_BATCHTIME = 2002;
const int  PRE_GET_FILEID_ERR = 2003;
const int  GET_EVENTIDSEQ_ERR = 2004; //��ȡ�¼�ID����ʧ��
//##


//## 
//## ���ظ澯���� 
//## ��Χ: 3000-3999

//##


//## 
//## �ƷѸ澯���� 
//## ��Χ: 4000-4999

//##


//## 
//## ���澯���� 
//## ��Χ: 5000-5999

//##


//## 
//## ���ո澯���� 
//## ��Χ: 6000-6999

//##


//## 
//## ���˸澯���� 
//## ��Χ: 7000-7999

//##


//## 
//## ���̵��ȸ澯���� 
//## ��Χ: 8000-8999

//##

//ԭ�¼�����
#define DIRECT_PUTINDB_EVENT_TYPE       9
#define DS_SOURCE_EVENT_TYPE 6

//�¼����Ͷ���
#define NORMAL_SERV_CYCLE_FEE_EVENT_TYPE 171
#define NORMAL_SERV_PORD_CYCLE_FEE_EVENT_TYPE 172

#define CYCLE_2HC_EVENT_TYPE    177

//#define ACCT_EVENT_TYPE       100     //�˵��¼�����
#define FILE_END_EVENT_TYPE     -99     //�ļ������¼�
#define FORCE_COMMIT_EVENT_TYPE -98     //ǿ���ύ�¼�
#define FORCE_COMMIT_MERGE_TYPE -97 //ǿ�ƺϲ��¼�
#define BT_CHARGE_ACCU_EX_EVENT_TYPE -96  // ���ò��䷢���¼�
#define CYCLE_ACCT_EVENT_TYPE   108     //����ĩ�Ż��¼�
#define CYCLE_BASE_EVENT_TYPE   174     //��Ʒ�����Ѽ����¼� (���ڳ����͵��¼�)
#define REAL_ACCT_EVENT_TYPE    3483    //ʵʱ�˵��¼�
#define REAL_CYCLE_ACCT_EVENT_TYPE 3531 //BTʵʱ�ʵ��¼�
#define SPECIAL_STRATEGY_EVENT_TYPE     3482    //�ض������¼�
#ifdef GROUP_CONTROL
#define CG_STRATEGY_EVENT_TYPE          3484    //���������ֻ�Ӫ���¼�
#endif
#define CALL_EVENT_TYPE 105                  //�����¼�
#define DATA_EVENT_TYPE 106                  //�����¼�  
#define VALUE_ADDED_EVENT_TYPE 107           //��ֵ�¼�
#define CYCLE_FEE_EVENT_TYPE 2               //�����Է����¼�
#define PP_PARAM_ERR_EVENT_TYPE 110          //Ԥ�������������¼�
#define RATE_PARAM_ERR_EVENT_TYPE 111        //���۲��������¼�
#define FILE_PARAM_ERR_EVENT_TYPE 102            //�ļ�������������¼�
#define NOOWNER_EVENT_TYPE 109               //�����¼�
#define OTHER_EVENT_TYPE 167                 //��Ч�¼� (�ּ�����Ʒ�ϵͳ�޹ص��¼�)
#define ERR_EVENT_TYPE 168                   //��ʽ�����¼�
#define DUP_EVENT_TYPE 101                   //�ظ��¼�


//## Ԥ���� �¼��������ͱ��� ��Ӧerr_id
const int PRE_NO_FORMAT_RULE  =   1000; //�嵥�Ҳ������Թ�������
const int START_TIME_YEAR_INVALID = 1001;
const int START_TIME_MONTH_INVALID = 1002;
const int START_TIME_DATE_INVALID = 1003;
const int START_TIME_HOUR_INVALID = 1004;
const int START_TIME_MINUTE_INVALID = 1005;
const int START_TIME_SECOND_INVALID = 1006;

const int NBR_EXIST_CHAR = 1010;

//OWNER_ID
const int CALLED_NUMBER_OWNERID=    21;
const int JUDGE_AGREEMENT_EVENT_TYPE    = 20;
//���ϼ���
const int ALL_INFO                                      = 0;
const int A_SERV_TYPE_CHANGE            = 1;
const int SERV_LOCATION                 = 2;
const int SERV_STATE_ATTR                       = 3;
const int SERV_ATTR                             = 4;
const int SERV_IDEN                             = 5;
const int SERV_PRODUCT_ANDATTR          = 6;
const int SERV_ACCT                             = 7;
const int ACCT                                          = 8;
const int PRODUCT_OFFER_INFO            = 9;
const int A_ACC_NBR_OFFER_TYPE  =20;

const int CUST                                          = 10;
const int SERV                                          = 11;
const int SERV_PRODUCT                          = 12;
const int SERV_PRODUCT_ATTR             = 13;
const int OFFER_INSTANCE                        = 14;
const int INSTANCE_DETAIL                       = 15;

//ʹ��Envionment.useUserInfo(int iMode) ʱʹ�õİ�ģʽ����
const int USER_INFO_BASE                    = 1;
const int USRE_INFO_NOITEMGROUP     = 2;

#ifdef GROUP_CONTROL
const int GROUP_NUM                                     = 100;
#endif

//������־���ķ�����������
const int UIO_PARTITION_ID_INIT = 1;//��ʼ����
const int UIO_PARTITION_ID_MODACCT      = 2;//�Ѵ����Ĺ������ݷ���
const int UIO_PARTITION_ID_OTHER        = 3;//�Ѵ����ķǹ������ݷ���

//�������� add by xn 2009.10.22
const int B_NP            = 10000;
//����

const int MEM_VER_OLD  = 0;
const int MEM_VER_SHMDATA_A   = 1;
const int MEM_VER_SHMDATA_B   = 2;
const int MEM_VER_INTINDEX_A  = 3;

//���ڶ������������е����鳤��
const int MAX_ARRARY_LEN      = 10;
	
//���ڶ�����Ϣ���д�����ʵ��ĳ���
const int MAX_ITEM_RECORD     = 10;

//���ڶ������Ƿ����ʵ��ı�ʶ
const int ACCT_ITEM_TRANS     = 9999;

//����汾�ŵĳ�ʼֵ
const int AGGR_VERSION_INITVAL		= 0;


const int MAX_AVPSTR_BUF = 4096;	// AVP���Ļ����С
const int MAX_XML_TREE_BUF_LENGTH = 4096;

//pid len
const int PID_LEN = 10;

//�¼���Դ
const int OCS_EVENT = 101;				//OCS����
const int HB_EVENT  = 102;				//HB����
const int VC_EVENT  = 103;				//VC����
const int WSS_EVENT = 104;				//WSS����
const int CRM_EVENT = 105;				//CRM����

#endif // COMMON_MACRO_H
//end. def.
