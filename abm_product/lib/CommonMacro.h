/*VER: 12*/ 
#ifndef COMMON_MACRO_H
#define COMMON_MACRO_H

#include "../../etc/LocalDefine.h"

//#include "../../config/LocalDefine.h"
//##
//##公共的宏定义
//##
#define NUMBER                  2 //1
#define STRING                  3 //2
#define ARRAY_INT       4

#define DEFAULT_CYCLE_TYPE      0       //默认的帐期类型
#define FRIEND_ZONE_ID          2       //亲情号码的区表

#define PAY_MEASURE_MONEY   2   //金钱类度量

#define SERV_CYCLE_ACCT     0   //用户级的帐期末优惠
#define COMPLEX_CYCLE_ACCT  1   //其他类型的帐期末优惠



//## ID_BILLING_MODE: 作为一种用户属性， 描述计费模式
#ifdef DEF_HEBEI

int const ID_BILLING_MODE = 162;

#else

int const ID_BILLING_MODE = 403;

#endif


//##
//##计费模块编号
//##
#include "MBC.h"

#define DATA_MONTH_TYPE_PLAN    9

//##实例对象类型
#define SERV_INSTANCE_INT       10
#define OFFER_INSTANCE_INT      20
#define GENERAL_INSTANCE_INT    1000

#define PRODUCT_INT             15      //对象类型为产品对应的后台编号
#define OFFER_INT               25      //对象类型为商品对应的后台编号
#define GENERAL_INT             1000

//##对象类型定义（和上面的有重复，以后逐渐取消上面的）
#define SERV_INSTANCE_TYPE      10      //主产品实例类型
#define PRODUCT_TYPE            10      //产品类型
#define OFFER_INSTANCE_TYPE     20      //商品实例类型
#define OFFER_TYPE              25      //商品类型
#define CUST_TYPE               30      //客户类型
#define ACCT_TYPE               35      //账户类型
#define SERV_GROUP_TYPE         50      //用户群类型
#define GENERAL_TYPE            1000    //通用类型

//##生命周期的类型
#define ABSOLUTE_TYPE           1       //绝对日期
#define RELATIVE_YEAR_TYPE      2       //相对套餐受理日期的年数
#define RELATIVE_MONTH_TYPE     3       //相对套餐受理日期的月数
#define RELATIVE_DAY_TYPE       4       //相对套餐受理日期的天数
#define MIX_YEAR_TYPE           5       //相对套餐受理日期的年数或者绝对日期，二者其一
#define MIX_MONTH_TYPE          6       //相对套餐受理日期的月数或者绝对日期，二者其一
#define MIX_DAY_TYPE            7       //相对套餐受理日期的天数或者绝对日期，二者其一
#define MOD_MONTH_TYPE          8
#define MOD_CYCLE_TYPE          9
#define ABSOLUTE_NOYEAR_TYPE    10      //绝对日期不考虑年
#define RELATIVE_CYCLE_TYPE     11      //相对套餐生效的帐期
#define MOD_NORMAL_MONTH_TYPE   12  //自然月

#define DB_LINK Environment::getDBConn()  //数据库连接宏定义

//##
//##事件域
//##
#define  MAX_ATTR_VAL_LEN       31              //附属产品属性取值字符串最大长度
#define  MAX_ATTR_EFF_DATE_LEN  15              //附属产品属性取值生效时间长度
#define  MAX_ATTR_EXP_DATE_LEN  15              //附属产品属性取值失效时间长度
#define  MAX_BILLING_AREA_CODE_LEN      11      //计费方号码区号最大长度
#define  MAX_BILLING_NBR_LEN    41              //计费方号码最大长度
#define  MAX_BILLING_VISIT_AREA_CODE    11      //计费方到访地区号最大长度
#define  MAX_CALLING_AREA_CODE_LEN      11      //主叫区号最大长度
#define  MAX_CALLING_NBR_LEN    33              //主叫号码最大长度
#define  MAX_ORG_CALLING_NBR_LEN        33      //原始主叫号码最大长度
#define  MAX_CALLED_AREA_CODE_LEN       11      //被叫区号最大长度
#define  MAX_CALLED_NBR_LEN     33              //被叫号码最大长度
#define  MAX_ORG_CALLED_NBR_LEN 33              //原始被叫号码最大长度
#define  MAX_THIRD_PARTY_AREA_CODE_LEN  11      //第三方区号最大长度
#define  MAX_THIRD_PARTY_NBR_LEN        33      //第三方号码最大长度
#define  MAX_START_DATE_LEN     18              //起始时间最大长度
#define  MAX_END_DATE_LEN       18              //结束时间最大长度
#define  MAX_TRUNK_IN_LEN       13              //入中继号码最大长度
#define  MAX_TRUNK_OUT_LEN      13              //出中继号码最大长度
#define  MAX_EVENT_STATE_LEN    4               //事件状态最大长度
#define  MAX_CREATED_DATE_LEN   15              //事件创建时间最大长度
#define  MAX_STATE_DATE_LEN     15              //事件状态修改时间最大长度
#define  MAX_CARD_ID_LEN        41              //卡号最大长度
#define  MAX_OFFER_NUM  5                       //商品标识最大数量
#define  MAX_BILLING_RECORD_LEN 101             //算费过程记录最大长度
#define  MAX_ACCOUNT_AREA_CODE_LEN      11      //上网帐号区号最大长度
#define  MAX_ACCOUNT_NBR_LEN    33              //上网帐号号码最大长度

#ifndef  DEF_HAINAN
#define  MAX_NAS_IP_LEN 17                      //上网IP地址最大长度
#else
#define  MAX_NAS_IP_LEN 32                      //上网IP地址最大长度---海南需要增加长度
#endif

#define  MAX_CYCLE_BEGIN_DATE_LEN       15      //帐期起始时间最大长度
#define  MAX_CYCLE_END_DATE_LEN 15              //帐期结束时间最大长度
//#define  MAX_DAFAULT_STRING_NUM       10      //保留的字符串类型字段最大数量
#define  MAX_BYTES_PER_STRING   33              //每个保留的字符串类型字段最大长度
//#define  MAX_DEFAULT_INT_NUM  10              //保留的整数型字段最大数量
#define  MAX_SERV_PROD_ATTR_NUM 2               //附属产品信息数目
#define  MAX_ORG_CHARGE_NUM 2       //原始费用
#define  MAX_EXT_FIELD_NUM 10                   //事件扩展属性的最大数目
#define  MAX_FILE_NAME_LEN 200      //文件名最大长度
#define  MAX_MSC_LEN    16              //移动交换机标识最大长度
//数据校验ismp业务有255位长的字段
#define MAX_ACCESS_POINT_NAME_LEN 260  //接入点名称最大长度

//7月19江苏发现有6位CELL_ID,联通数据模型有8位
#define  MAX_LAC_LEN    9               //小区号标识最大长度
#define  MAX_CELLID_LEN         9               //基站号最大长度
//#define  MAX_LAC_LEN  6               //小区号标识最大长度
//#define  MAX_CELLID_LEN               6               //基站号最大长度
#define  MAX_ROMING_NBR_LEN             31              //动态漫游号最大长度
#define  MAX_CALLING_VISIT_AREA_CODE_LEN         11  //主叫到访地区号最大长度
#define  MAX_CALLED_VISIT_AREA_CODE_LEN         11              //被叫到访地区号最大长度
#define  MAX_IMSI_LEN   21              //IMSI最大长度
#define  MAX_IMEI_LEN   21              //IMEI最大长度
#define  MAX_ORG_SEQ_LEN 32             // 话单原始序列号最大长度
#define  MAX_RESERVED_FIELD_NUM 5                       //事件保留属性的最大数目
#define  MAX_RESERVED_FIELD_LEN 41                      //事件保留属性字段的最大长度
#define  MAX_GATE_WAY_LEN       11    //网关编码长度
#define  MAX_RP_STR_LEN         16    //基站编码长度
#define  MAX_SESSION_ID_LEN     56    //SessionID 长度
#define  MAX_EXT_STRING_LEN     31    //sExtendValue 的最大长度
#define  MAX_DOMAIN_NAME_LEN    21    //域名长度
#define  MAX_BIND_ATTR_LEN      101   //绑定属性长度
#define  MAX_AREA_NO_LEN        21    //用户属地
#define MAX_ACCU_RECORD_LEN     200   //累积量轨迹长度2010-09-08
#define  DUP_CHK_MAX_CALLING_LEN 41   //排重域 主叫号码域长度
#define  DUP_CHK_MAX_CALLED_LEN 41    //排重域 被叫号码域长度
#define DUP_CHK_MAX_SEQKEY_LEN 16
#define DUP_CHK_MAX_SEQID_LEN  16


//属性标识
#define   ID_EVENT_ID               1           //帐期标识                     
#define   ID_BILLING_CYCLE_ID        2          //事件标识                     
#define   ID_EVENT_TYPE_ID                   3          //事件类型标识                 
#define   ID_BILLING_AREA_CODE               4          //计费方区号                   
#define   ID_BILLING_NBR                    5           //计费方号码                   
#define   ID_BILLING_VISIT_AREA_CODE          6         //计费方到访地区号             
#define   ID_CALLING_AREA_CODE               7          //主叫方区号                   
#define   ID_CALLING_NBR                    8           //主叫方号码                   
#define   ID_ORG_CALLING_NBR                 9          //原始的主叫方号码             
#define   ID_CALLING_SP_TYPE_ID               10        //主叫运营商类型标识           
#define   ID_CALLING_SERVICE_TYPE_ID          11        //主叫业务类型标识             
//#define   ID_CALLING_EXCHANGE_ID             12         //主叫局向标识                 
#define   ID_CALLED_AREA_CODE                13         //被叫区号                     
#define   ID_CALLED_NBR                     14          //被叫号码                     
#define   ID_ORG_CALLED_NBR                  15         //原始的被叫方号码             
#define   ID_CALLED_SP_TYPE_ID                16        //被叫运营商类型标识           
#define   ID_CALLED_SERVICE_TYPE_ID           17        //被叫业务类型标识             
//#define   ID_CALLED_EXCHANGE_ID              18         //被叫局向标识                 
#define   ID_THIRD_PARTY_AREA_CODE            19        //第三方号码区号               
#define   ID_THIRD_PARTY_NBR                 20         //第三方号码                   
#define   ID_START_DATE                     21          //事件起始时间                 
#define   ID_END_DATE                       22          //事件结束时间                 
#define   ID_DURATION                      23           //事件时长                     
#define   ID_SWITCH_ID                      24          //交换机标识                   
#define   ID_TRUNK_IN                       25           //入中继号                     
#define   ID_TRUNK_IN_TYPE_ID                 26         //入中继类型标识               
#define   ID_TRUNK_OUT                      27          //出中继号                     
#define   ID_TRUNK_OUT_TYPE_ID                28        //出中继类型                   
#define   ID_FILE_ID                        29          //文件标识                     
#define   ID_EVENT_STATE                    30          //事件状态                     
#define   ID_CREATED_DATE                   31          //事件到达计费系统时间         
#define   ID_STATE_DATE                     32          //事件确定状态的时间           
#define   ID_CARD_ID                        33          //卡标识                       
#define   ID_BILLING_TYPE_ID                 34         //计费类型标识                 
#define   ID_PARTER_ID                      35          //合作方标识                   
#define   ID_SERV_ID                        36          //用户标识                     
#define   ID_CUST_ID                        37          //客户标识                     
#define   ID_PROD_ID                     38             //产品标识                     
#define   ID_SERV_PROD_ID                 39            //附属产品标识                 
#define   ID_PROD_BUNDLE_ID               40            //产品包标识                   
#define   ID_OFFERS1                     41             //商品标识1                    
#define   ID_OFFERS2                     42             //商品标识1                    
#define   ID_OFFERS3                     43             //商品标识1                    
#define   ID_OFFERS4                     44             //商品标识1                    
#define   ID_OFFERS5                     45             //商品标识1                    
#define   ID_BILLING_RECORD                 46          //算费过程记录                 
#define   ID_ERROR_ID                       47          //错误号标识                   
#define   ID_ACCOUNT_AREA_CODE            48         //数据业务帐号所在地区区号     
#define   ID_ACCOUNT_NBR                 49          //数据业务帐号                 
#define   ID_NAS_IP                      50          //网络接入IP                   
#define   ID_NAS_PORT_ID                  51         //网络接入端口标识             
#define   ID_RECV_BYTES                  52          //接收字节数                   
#define   ID_SEND_BYTES                  53          //发送字节数                   
#define   ID_RECV_PACKETS                54          //接收包数                     
#define   ID_SEND_PACKETS                55          //发送包数                     
#define   ID_NEGOTIATED_QOS              56          //服务质量                     
#define   ID_CYCLE_BEGIN_DATE             57         //帐期起始时间                 
#define   ID_CYCLE_END_DATE               58         //帐期结束时间                 
#define   ID_EXT_USED_NUM                 59         //扩展属性字段使用个数         
#define   ID_BUSINESS_KEY               60           //业务键                       
#define   ID_SER_PROD_ATTR_USED_NUM         61       // 附属产品属性实际使用的数目  
#define   ID_CHARGE_INFO_ORG_CHARGE1    62           //第1个费用域的原始费用        
#define   ID_CHARGE_INFO_STD_CHARGE1    63           //第1个费用域的标准费用        
#define   ID_CHARGE_INFO_CHARGE1       64            //第1个费用域的最终费用        
#define   ID_CHARGE_INFO_ACCT_ITEM_TYPE_ID1       65         //第1个费用域的帐目类型标识    
#define   ID_CHARGE_INFO_DISCOUNT_ACCT_ITEM_TYPE_ID1      66        //第1个费用域的优惠帐目类型标识
#define   ID_CHARGE_INFO_ORG_CHARGE2      67           //第2个费用域的原始费用        
#define   ID_CHARGE_INFO_STD_CHARGE2      68           //第2个费用域的标准费用        
#define   ID_CHARGE_INFO_CHARGE2         69            //第2个费用域的最终费用        
#define   ID_CHARGE_INFO_ACCT_ITEM_TYPE_ID2     70         //第2个费用域的帐目类型标识    
#define   ID_CHARGE_INFO_DISCOUNT_ACCT_ITEM_TYPE_ID2    71        //第2个费用域的优惠帐目类型标识
#define   ID_SERV_PROD_ATTR_ID1             72               //第1个附属产品信息的属性标识  
#define   ID_SERV_PROD_ATTR_VAL1            73               //第1个附属产品信息的属性取值  
#define   ID_SERV_PROD_ATTR_EFF_DATE1        74              //第1个附属产品信息的生效时间  
#define   ID_SERV_PROD_ATTR_EXP_DATE1        75              //第1个附属产品信息的失效时间  
#define   ID_SERV_PROD_ATTR_ID2             76               //第2个附属产品信息的属性标识  
#define   ID_SERV_PROD_ATTR_VAL2            77               //第2个附属产品信息的属性取值  
#define   ID_SERV_PROD_ATTR_EFF_DATE2        78              //第2个附属产品信息的生效时间  
#define   ID_SERV_PROD_ATTR_EXP_DATE2        79              //第2个附属产品信息的失效时间  
#define   ID_CALL_TYPE_ID      80            //呼叫类型标识
#define   ID_BILLING_ORG_ID 81    //计费方运营组织标识
#define   ID_BILL_FLOW_ID 82       //计费流程标识
#define   ID_CHARGE_INFO_BILL_MEASURE1  83       //计费量标识
#define   ID_CHARGE_INFO_BILL_MEASURE2  84       //计费量标识
#define   ID_MSC        207                                                     //移动交换机标识
#define   ID_LAC        208                                                     //小区号标识
#define   ID_CELLID      209                                            //基站号标识
#define   ID_ROMING_NBR  210                                            //动态漫游号标识
#define   ID_NO_BILL_FLAG  211                                  //免费标志标识
#define   ID_CALLING_VISIT_AREA_CODE  212               //主叫到访地区号标识
#define   ID_CALLED_VISIT_AREA_CODE  213                //被叫到访地区号标识
#define   ID_IMSI         214                                           //IMSI标识
#define   ID_IMEI  215                                                  //IMEI标识
#define   ID_EVENT                      402     //事件 用于累积事件数目

//##
//##条件和运算模块
//##
#define CONDITION_HASH_VALUE    2000
#define OPERATION_HASH_VALUE    2000
#define MAX_STRING_VALUE_LEN    80
#define MAX_SQL_WHERE_LEN       200

//##
//## 进程框架
//##
#define MAX_OUT_PROCESS_NUM     128     //一个文件的事件最多发送给多少个流程

//##
//##定价
//##
#define MAX_BREAKPOINT_NUM          200
#define MAX_EVENTSECTION_NUM    4096    //一个事件最多分成多少段（所有度量分段数之和）



#define UNKNOWN_RELATION    0
#define COVER_RELATION          1       //定价组合的覆盖关系
#define ASSOC_RELATION          2       //定价组合的关联关系
#define MIN_RELATION        3   //定价组合的最优关系
#define SWAP_RELATION       4   //定价组合的翻转关系
#define IGNORE_RELATION     5   //定价组合的忽略关系
#define CUTORREP_RELATION   6   //定价组合的折换关系


//##
//##用户资料查找模块
//##
#define MAX_OFFERS                      5       //一个主产品最多属于多少个商品
#define MAX_OFFERINSTANCES          5   //............................实例
#define MAX_PRICINGPLANS            64  //一个主产品最多对应多少个定价计划
#define AVG_PRICINGPLANS            4   //平均一个主产品对应的个性化的定价计划数
#define MAX_ACCTITEM_PERGROUP   4096    //一个账目组最多多少个账目
#define MAX_EVENTTYPE_PERGROUP  1024    //
#define MAX_OFFER_ATTRS         30   //一个商品实例最多有多少个属性
#define MAX_OFFER_MEMBERS       8192 //一个商品实例最多有多少个成员

//##
//## PricingEngine Buffer
//## for new Version

#define MAX_OFFER_INSTANCE_NUMBER      512 //一个对象最多属于多少个商品实例,20071128由128改为512
#define MAX_PRICING_COMBINE_NUMBER    1024 //上述商品实例符合条件的定价组合数,20071128由512改为1024
#define MAX_TARIFF_NUMBER             2048 //上述定价组合含有的Tariff的最大个数
#define MAX_DISCOUNT_NUMBER           2048 //最大的Discount数
#define MAX_PERCENT_NUMBER            2048 //最大的折扣数
#define MAX_ACCUMULATE_NUMBER         2048 //最大的累积数
#define MAX_PAY_ITEM_NUMBER         131072 //批价结果的最大数量
#define MAX_PRESENT_ITEM_NUMBER       2048 //赠送的最大数量
#define MAX_ACCUMULATE_ITEM_NUMBER    2048 //批价结果的累积量的最大数量
#define MAX_CAL_MEASURE_NUMBER         512 //清单批价的最多的度量数量
#define MAX_EVENTPART_NUMBER          4096 //最多的分段数量
#define MAX_ACCU_PER_STRATEGY            4 //一个定价策略下面引用的累积量数量

#define MAX_DISCT_APPLYER_NUMBER      1024 //订购的所有商品实例下最大的分摊对象数量
#define MAX_DISCT_APPLYER_OBJ_NUMBER  2048 //最大的分摊应用对象数量
#define MAX_PRICING_SECTION_NUMBER    5120 //最大的定价段落数量
#define MAX_CONDITION_POINT_NUMBER    3072 //最大的条件值数量
#define MAX_MEASURE_NUMBER            2048 //最大的度量数量
#define MAX_FILTER_COND_NUMBER        3072 //最大的过滤对象数量

//add by xn 2010-6-25
#define MAX_APP_PARAM_LEN                                                       256     // 应用进程最大携带参数长度
//##
//##
//## Item_Source_Id 的含义
#define ITEM_SOURCE_MINUS        7 //补退
#define ITEM_SOURCE_OUT_STD      8 //套餐外详单定价


#define ITEM_SOURCE_STD         11 //详单定价
#define ITEM_SOURCE_DISCOUNT    12 //详单优惠---特殊需求时有用
#define ITEM_SOURCE_ADD_ACCT    13 //增量账单优惠
#define ITEM_SOURCE_BT_ACCT     14 //全量账单优惠和帐期末优惠
#define ITEM_SOURCE_CYCLE_ACCT  15 //帐期末优惠
#define IVPN_ITEM_SOURCE_ID             16 //IVPN代付帐单
#define ITEM_SOURCE_ADJUST       9 //优惠前调帐
#define ITEM_SOURCE_NODISCT     10 //不参与优惠的调帐
#define ITEM_SOURCE_IVPN_ADJUST 19 //IVPN调帐
#define ITEM_SOURCE_IVPN_PAYMENT 20 //IVPN集团代付账单
#define IVPN_ITEM_SOURCE_PROV    21 //IVPN代付帐单_省内
#define IVPN_ITEM_SOURCE_LOCAL    22 //IVPN代付帐单_本地网



//##
//##
//## Item_Class_Id的含义
#define ITEM_CLASS_BLOCKED_FLAG  1 //封锁标识

//##
//##
//##
#define MAX_EVENT_PERSERV       2048    //max cycle_fee_event each serv


//##
//##预处理
//##
//定义格式错误事件输出文件的路径
#define ERR_FULL_PATH "/home/bill/TIBS_HOME/src/app_prep/ErrPath/"
#define OUTPUT_FULL_PATH "/home/bill/TIBS_HOME/src/app_prep/OutputPath/"

#define CALLTYPE_MOC 1
#define CALLTYPE_MTC 2
#define CALLTYPE_FORWARD 100
#define CALLTYPE_FWD 3

const int  EVENT_ID_INCREMENT = 1000;

//const int  ROAMTYPE_ROAM_BORD = 1;     //漫游类型-边界漫游
//const int  ROAMTYPE_ROAM_SAME = 2;     //漫游类型-同城漫游
//const int  ROAMTYPE_ROAM = 3;          //漫游类型-漫游(大类)
//const int  ROAMTYPE_ROAM_PROV = 30;    //漫游类型-省内漫游
//const int  ROAMTYPE_ROAM_NATION = 31;  //漫游类型-国内漫游
//const int  ROAMTYPE_ROAM_INTER = 32;   //漫游类型-国际漫游
//const int  ROAMTYPE_NOT_ROAM = 4;      //漫游类型-非漫游

const int  ROAMTYPE_NOT_ROAM = 0;         //漫游类型-非漫游
const int  ROAMTYPE_ROAM_PROV = 1;        //漫游类型-省内漫游
const int  ROAMTYPE_ROAM_NATION_IN = 2;   //漫游类型-国内漫入
const int  ROAMTYPE_ROAM_NATION_OUT = 3;  //漫游类型-国内漫出
const int  ROAMTYPE_ROAM_INTER_IN = 4;    //漫游类型-国际漫入
const int  ROAMTYPE_ROAM_INTER_OUT = 5;   //漫游类型-国际漫出
const int  ROAMTYPE_ROAM_BORD = 6;        //漫游类型-边界漫游
const int  ROAMTYPE_ROAM_SAME = 7;        //漫游类型-同城漫游
const int  ROAMTYPE_ROAM_BORD_EX= 8 ;

/* xueqt 20080523       
const int  LONGTYPE_LONG = 1;          //长途类型-长途(大类)
*/
const int  LONGTYPE_LONG = 1;          //长途类型-长途(大类)
const int  LONGTYPE_PROV = 10;         //长途类型-省内
const int  LONGTYPE_NATION = 11;       //长途类型-国内
const int  LONGTYPE_INTER = 12;        //长途类型-国际
const int  LONGTYPE_THIRT = 13;        //长途类型-13国
const int  LONGTYPE_AMERC = 14;        //长途类型-美加
const int  LONGTYPE_KONG = 15;         //长途类型-港澳台
const int  LONGTYPE_LATN = 2;          //长途类型-非长途(市话)
const int  LONGTYPE_LATN_XK = 151; //港澳台-香港长途
const int  LONGTYPE_LATN_AM = 152; //港澳台-澳门长途
const int  LONGTYPE_LATN_TW = 153; //港澳台-台湾长途
const int  LONGTYPE_LATN_HS = 16;  //海事卫星
const int  LONGTYPE_LATN_PROV_NATION = 17;  //省内长途+国内长途
const int  LONGTYPE_LATN_NATION_PROV = 18;  //国内长途+省内长途
const int  LONGTYPE_LATN_PROV_PROV = 19;  //省内长途+省内长途
const int  LONGTYPE_LATN_NATION_NATION = 19;  //国内长途+国内长途



const int LONGTYPE_LATN_EX = 1000;          //C网本地


const int  LONG_GROUPTYPE_PROV = 10;         //长途组类型-省内
const int  LONG_GROUPTYPE_NATION = 20;       //长途组类型-国内
const int  LONG_GROUPTYPE_INTER = 30;        //长途组类型-国际
const int  LONG_GROUPTYPE_THIRT = 31;        //长途组类型-13国
const int  LONG_GROUPTYPE_AMERC = 32;        //长途组类型-美加
const int  LONG_GROUPTYPE_KONG = 40;         //长途组类型-港澳台
const int  LONG_GROUPTYPE_XK = 401; //长途组类型-港
const int  LONG_GROUPTYPE_AM = 402; //长途组类型-澳
const int  LONG_GROUPTYPE_TW = 403; //长途组类型-台
const int  LONG_GROUPTYPE_HS = 50; //海事卫星




const int  FORWARDTYPE_UNCOND = 33; //无条件呼转
const int  FORWARDTYPE_DEFLECTION = 36; //呼转偏差
const int  FORWARDTYPE_COND = 40; //有条件呼转
const int  FORWARDTYPE_BUSY = 41; //遇忙呼转
const int  FORWARDTYPE_NOREPLAY = 42; //无应答呼转
const int  FORWARDTYPE_NOTREACH = 43; //不可及呼转


const int  FORWARDTYPE_UNCOND_EX = 1;//无条件呼转
const int  BUSINESSKEY_FORWARDTYPE_UNCOND = 1001;  //不需要计算2段长途
const int  BUSINESSKEY_FORWARDTYPE_COND = 1002;    //需要计算2段长途  

const int  D_M_ACCNBR_LEN = 8;   //固话号码最大长度
const int  D_M_MOBNBR_LEN = 11;  //手机号码最大长度

const int  MAX_FORMAT_RECORD_LEN = 8000; //记录规整过程的字符串长度

#define MAX_BLOCK_SIZE 10000000

const int MAX_SPFLAG_LEN = 80;  //ISMP域的SP标识串长度

//##

//周期性费用生成
#define CYCLE_FEE_EVENT_FILE_PATH "/home/bill/TIBS_HOME/src/app_cyclefee/FilePath/"
//#define MAX_FILE_NAME_LEN 200
#define MAX_PATH_NAME_LEN 200
#define MAX_FULL_FILE_NAME_LEN 300
#define RETRY_TIMES 3
#define EVENTS_PER_FILE 50000

//ERROR_ID
#define ERROR_NO_OWNER          2000    //找不到用户资料
#define ERROR_NO_CYCLE          2001    //找不到帐务周期
#define ERROR_NO_SERVID         4000    //批价的时候根据servid找用户资料失败
#define ERROR_NO_COMBINE        4001    //批价的时候找不到定价组合
#define ERROR_NO_SECTION        4002    //分段不全



//#######################__告警编码定义__######################## add by yangks.

/*
bool Pprintf(int iAlertCode,bool bInsertDB,int iFileID,char *fmt,...)

输出应用进程日志、告警信息到前台调度界面
        iAlertCode==0时 表示非告警信息
                (AlertCode 在表BILL.WF_ALERT_DEFINE 中有解释,告警的处理方法
                各模块增加告警代码时,需要在这个头文件和数据库表中增加相应的记录)
        bInsertDB: 是否需要入告警日志表(wf_alert_log)
        iFileID  : 当前处理的文件ID. 不存在时置0
        fmt      : 格式化输出
*/

//## 
//## 公共告警编码
//## 范围: 0-999
const int ALERT_SIG_ERR = 100;  //signal函数定义信号处理时失败
//##


//## 
//## 周期性费用事件告警编码 
//## 范围: 1000-1999

//##


//## 
//## 预处理告警编码
//## 范围: 2000-2999
const int  ALERT_RCVMODE_ERR  = 2001;
const int  ALERT_NO_BATCHTIME = 2002;
const int  PRE_GET_FILEID_ERR = 2003;
const int  GET_EVENTIDSEQ_ERR = 2004; //获取事件ID序列失败
//##


//## 
//## 排重告警编码 
//## 范围: 3000-3999

//##


//## 
//## 计费告警编码 
//## 范围: 4000-4999

//##


//## 
//## 入库告警编码 
//## 范围: 5000-5999

//##


//## 
//## 回收告警编码 
//## 范围: 6000-6999

//##


//## 
//## 回退告警编码 
//## 范围: 7000-7999

//##


//## 
//## 流程调度告警编码 
//## 范围: 8000-8999

//##

//原事件类型
#define DIRECT_PUTINDB_EVENT_TYPE       9
#define DS_SOURCE_EVENT_TYPE 6

//事件类型定义
#define NORMAL_SERV_CYCLE_FEE_EVENT_TYPE 171
#define NORMAL_SERV_PORD_CYCLE_FEE_EVENT_TYPE 172

#define CYCLE_2HC_EVENT_TYPE    177

//#define ACCT_EVENT_TYPE       100     //账单事件类型
#define FILE_END_EVENT_TYPE     -99     //文件结束事件
#define FORCE_COMMIT_EVENT_TYPE -98     //强制提交事件
#define FORCE_COMMIT_MERGE_TYPE -97 //强制合并事件
#define BT_CHARGE_ACCU_EX_EVENT_TYPE -96  // 费用补充发送事件
#define CYCLE_ACCT_EVENT_TYPE   108     //帐期末优惠事件
#define CYCLE_BASE_EVENT_TYPE   174     //商品基本费计算事件 (帐期初发送的事件)
#define REAL_ACCT_EVENT_TYPE    3483    //实时账单事件
#define REAL_CYCLE_ACCT_EVENT_TYPE 3531 //BT实时帐单事件
#define SPECIAL_STRATEGY_EVENT_TYPE     3482    //特定策略事件
#ifdef GROUP_CONTROL
#define CG_STRATEGY_EVENT_TYPE          3484    //党政机关手机营销事件
#endif
#define CALL_EVENT_TYPE 105                  //语音事件
#define DATA_EVENT_TYPE 106                  //数据事件  
#define VALUE_ADDED_EVENT_TYPE 107           //增值事件
#define CYCLE_FEE_EVENT_TYPE 2               //周期性费用事件
#define PP_PARAM_ERR_EVENT_TYPE 110          //预处理参数错误事件
#define RATE_PARAM_ERR_EVENT_TYPE 111        //批价参数错误事件
#define FILE_PARAM_ERR_EVENT_TYPE 102            //文件缓冲参数错误事件
#define NOOWNER_EVENT_TYPE 109               //无主事件
#define OTHER_EVENT_TYPE 167                 //无效事件 (分拣发现与计费系统无关的事件)
#define ERR_EVENT_TYPE 168                   //格式错误事件
#define DUP_EVENT_TYPE 101                   //重复事件


//## 预处理 事件错误类型编码 对应err_id
const int PRE_NO_FORMAT_RULE  =   1000; //清单找不到属性规整规则
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
//资料加载
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

//使用Envionment.useUserInfo(int iMode) 时使用的绑定模式定义
const int USER_INFO_BASE                    = 1;
const int USRE_INFO_NOITEMGROUP     = 2;

#ifdef GROUP_CONTROL
const int GROUP_NUM                                     = 100;
#endif

//资料日志表的分区常量定义
const int UIO_PARTITION_ID_INIT = 1;//初始分区
const int UIO_PARTITION_ID_MODACCT      = 2;//已处理的过户数据分区
const int UIO_PARTITION_ID_OTHER        = 3;//已处理的非过户数据分区

//开发测试 add by xn 2009.10.22
const int B_NP            = 10000;
//结束

const int MEM_VER_OLD  = 0;
const int MEM_VER_SHMDATA_A   = 1;
const int MEM_VER_SHMDATA_B   = 2;
const int MEM_VER_INTINDEX_A  = 3;

//用于定义条件运算中的数组长度
const int MAX_ARRARY_LEN      = 10;
	
//用于定义消息队列传输的帐单的长度
const int MAX_ITEM_RECORD     = 10;

//定于队列中是否是帐单的标识
const int ACCT_ITEM_TRANS     = 9999;

//定义版本号的初始值
const int AGGR_VERSION_INITVAL		= 0;


const int MAX_AVPSTR_BUF = 4096;	// AVP串的缓冲大小
const int MAX_XML_TREE_BUF_LENGTH = 4096;

//pid len
const int PID_LEN = 10;

//事件来源
const int OCS_EVENT = 101;				//OCS请求
const int HB_EVENT  = 102;				//HB请求
const int VC_EVENT  = 103;				//VC请求
const int WSS_EVENT = 104;				//WSS请求
const int CRM_EVENT = 105;				//CRM请求

#endif // COMMON_MACRO_H
//end. def.

