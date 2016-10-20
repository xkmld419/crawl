/*
*   version js at 2011-5-18
*/


#ifndef    __HSSCMDGENERAL_H_INCLUDED_
#define    __HSSCMDGENERAL_H_INCLUDED_

#include "hssobject.h"
#include "hssconstdef.h"
#include <vector>

using namespace std;


typedef long  __DFT_T1_;
typedef __DFT_T1_ __DFT_T2_;
typedef __DFT_T1_   __DFT_T3_;

#define UTF8_MAX 64
typedef unsigned int ui32;
typedef unsigned long ul64;

/**************************************************************************
*   ������AccuTpl
*   ���ܣ��������ۻ������������ģ����
*   ˵����
*       1. __clear(): ���ݽṹ���ʼ��
*       2. __add() :  ��װ�ۻ���������
*       3. __getValue(): ��ȡ�������е��ۻ�������
***************************************************************************/

template <typename T1,typename T2,  int CMD>
class GeneralTpl : public HSSObject
{
    
public:
    
    GeneralTpl() : HSSObject(CMD) {
    	__clear();
    };
    
    bool empty() {
    	return !(*((long *)&m_uBody));
    }
    
    int __getValue(T1 *&ptr) {
        int size = sizeof(m_uBody.m_tNum);  
        ptr = (T1 *)&m_uBody.m_sBuf[size]; 
        return m_uBody.m_tNum.m_iFirstNum;
    } 
    
    int __getValue2(T2 *&ptr) {
        int size = sizeof(m_uBody.m_tNum);  
        ptr = (T2 *)&m_uBody.m_sBuf[size]; 
        return m_uBody.m_tNum.m_iSecondNum;
    } 

    
protected:  
    
    void __clear() {
        m_iSize = CMD_OFFSET + sizeof(m_uBody.m_tNum);
        memset(&m_uBody.m_tNum, 0x00, sizeof(m_uBody.m_tNum));
    }
    
    bool __add1(T1 &data) {
        if((m_iSize+sizeof(T1)) > CMD_BUF_MAX) {
            return false;
        }
        m_uBody.m_tNum.m_iFirstNum ++;
        memcpy((void *)&m_uBody.m_sBuf[m_iSize-CMD_OFFSET], (const void *)&data, sizeof(T1));
        m_iSize += sizeof(T1);
        return true;
    }

    bool __add2(T2 &data) {
        if((m_iSize+sizeof(T2)) > CMD_BUF_MAX) {
            return false;
        }
        m_uBody.m_tNum.m_iSecondNum ++;
        memcpy((void *)&m_uBody.m_sBuf[m_iSize-CMD_OFFSET], (const void *)&data,sizeof(T2));
        m_iSize += sizeof(T2);
        return true;
    }
       
    
    int __getValue(vector<T1 *> &vec) {
    	int _size;
        for (int i=0; i<m_uBody.m_tNum.m_iFirstNum; ++i) {
        	_size = i*sizeof(T1) + sizeof(m_uBody.m_tNum);
        	if (_size >= CMD_BUF_SIZE)
        		return -1;
            vec.push_back((T1 *)&m_uBody.m_sBuf[_size]);  
        }   
        return vec.size();                  
    }
    
    int __getValue2(vector<T2 *> &vec) {
    	int _size;
        for (int i=0; i<m_uBody.m_tNum.m_iSecondNum; ++i) {
        	_size = i*sizeof(T2) + sizeof(m_uBody.m_tNum);
        	if (_size >= CMD_BUF_SIZE)
        		return -1;
            vec.push_back((T2 *)&m_uBody.m_sBuf[_size]);  
        }   
        return vec.size();
    } 
};

//7.2.5.1�û���Ϣ��ѯ
//������Ϣ
struct QueryServCCR
{
    int  m_iValueType;           //��ѯֵ���ͣ�1-ҵ����룻2-�ͻ���ʶ��3-��ͬ�ű�ʶ
    ui32 m_uiQueryType;          //��ѯ���ͣ�1����ѯ���¼�¼��2����ѯ���м�¼��
    char m_sValue[UTF8_MAX];     //��ѯֵ
    char m_sAreaCode[UTF8_MAX];  //�绰����
};

class HssQueryServCCR : public GeneralTpl<QueryServCCR,__DFT_T2_,HSSCMD_SERV_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryServCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryServCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryServCCR *> &vData)
    {
        return __getValue(vData);
    }
};
//����CCA
struct QueryServCCA
{
    int  m_iAlign;               //����
    ui32 m_uiServState;          //�û�״̬0��������1��ͣ����2��Ƿ��ͣ����3�����
    ul64 m_ulServId;             //�û���ʶ
    ui32 m_uiProductId;          //��Ʒ��ʶ
    ui32 m_uiHomeAreaCode;       //����������
    char m_sStateDate[UTF8_MAX]; //״̬ʱ��
    ui32 m_uiPaymentFlag;        //���ѷ�ʽ��0���󸶷ѣ�1��Ԥ����
    ui32 m_uiGroupUser;          //�û����ԣ�0��ӵ�м����ʻ���1��û�м����û�
    char m_sCustName[256];       //�ͻ�����
    char m_sProductName[56];       //��Ʒ����
};
class HssQueryServCCA : public GeneralTpl<QueryServCCA,__DFT_T2_,HSSCMD_SERV_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryServCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryServCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryServCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.2������Ʒ��Ϣ��ѯ
//����CCR ʹ���û���Ϣ��ѯ������ṹQueryServCCR
class HssQueryServProductCCR : public GeneralTpl<QueryServCCR,__DFT_T2_,HSSCMD_SERVPRODUCT_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryServCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryServCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryServCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//����CCA
struct QueryServProductCCA
{
    ul64 m_ulServProductID;    //������Ʒʵ����ʶ
    char m_sServProductName[56]; //������Ʒ����
    ul64 m_ulServId;           //��������Ʒʵ����ʶ
    ui32 m_uiProductId;        //������Ʒ��ʶ
    char m_sProductName[52];   //����Ʒ����
    ui32 m_uiEffDate; //��Чʱ��
    ui32 m_uiExpDate; //ʧЧʱ��
};
class HssQueryServProductCCA : public GeneralTpl<QueryServProductCCA,__DFT_T2_,HSSCMD_SERVPRODUCT_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryServProductCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryServProductCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryServProductCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.3�˻���Ϣ��ѯ
//CCR ʹ���û���Ϣ��ѯ������ṹQueryServCCR
class HssQueryAcctCCR : public GeneralTpl<QueryServCCR,__DFT_T2_,HSSCMD_ACCT_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryServCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryServCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryServCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//����CCA
struct QueryAcctCCA
{
    ul64 m_ulCustId;                     //�ͻ���ʶ
    char m_sCustName[UTF8_MAX];          //�ͻ�����
    ul64 m_ulAcctId;                     //�ʻ���ʶ
    char m_sTelAcctName[UTF8_MAX];       //�˻�����
    char m_sAcctType[UTF8_MAX];          //�ʻ�����
    char m_sState[UTF8_MAX];             //״̬
    char m_sStateDate[UTF8_MAX];         //״̬ʱ��
    ui32 m_uiPaymentMethod;              //���ʽ
    int  m_iAlign;                       //�ֽڶ���
};

class HssQueryAcctCCA : public GeneralTpl<QueryAcctCCA,__DFT_T2_,HSSCMD_ACCT_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryAcctCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryAcctCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryAcctCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.4�ͻ���Ϣ��ѯ
//CCRʹ���û���Ϣ��ѯ������CCR�ṹQueryServCCR
//QueryCustCCR
class HssQueryCustCCR : public GeneralTpl<QueryServCCR,__DFT_T2_,HSSCMD_CUST_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryServCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryServCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryServCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//����CCA
struct QueryCustCCA
{
    char m_sCustName[UTF8_MAX];  //�ͻ�����
    ui32 m_uiCustType;           //�ͻ�����0����ͻ���1������ͻ���2���˿ͻ�
    ui32 m_uiIdType;             //֤������0�����֤��1����ʻ֤��2������֤��3���գ�4������
    char m_sCustState[UTF8_MAX]; //�ͻ�״̬
    char m_sEffDate[UTF8_MAX];   //��Чʱ��
    char m_sExpDate[UTF8_MAX];   //ʧЧʱ��
    char m_sIdCode[UTF8_MAX];    //֤������
    ui32 m_uiHomeAreaCode;       //����������
    int  m_iAlign;               //�ֽڶ���
};

class HssQueryCustCCA : public GeneralTpl<QueryCustCCA,__DFT_T2_,HSSCMD_CUST_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryCustCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryCustCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryCustCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.18��ֵҵ���ѯ
//CCRʹ���û���Ϣ��ѯ��CCR�ṹQueryServCCR
class HssQueryIncrServCCR : public GeneralTpl<QueryServCCR,__DFT_T2_,HSSCMD_PRODUCTOFFER_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryServCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryServCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryServCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//����CCA
struct QueryIncrServCCA
{
    ul64 m_ulUnitPrice;   //����
    char m_sUnit[UTF8_MAX];            //��λ
    char m_sSpDescription[UTF8_MAX];   //Sp���������ģ�
    char m_sIncrServName[UTF8_MAX];    //��ֵҵ������
    ui32 m_uiEffDate;         //��Чʱ��
    ui32 m_uiExpDate;         //ʧЧʱ��
    ui32 m_uiCreatedDate;     //��������
    int  m_iAlign;            //�ֽڶ���
};

class HssQueryIncrServCCA : public GeneralTpl<QueryIncrServCCA,__DFT_T2_,HSSCMD_PRODUCTOFFER_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryIncrServCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryIncrServCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryIncrServCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.5�ײͶ�����ѯ
struct OrderResourceCCR
{
    char m_sAccNbr[UTF8_MAX];
};
class HssOrderResourceCCR : public 
GeneralTpl<OrderResourceCCR,__DFT_T2_,HSSCMD_ORDERRESOURCE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(OrderResourceCCR &data)
    {
        return __add1(data);
    }

    int  getReq(OrderResourceCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<OrderResourceCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//����CCA
struct OrderResourceCCA
{
    ui32 m_uiProductFlag;     //�Ƿ�Ϊ�ۻ����ײͣ�0�����ۻ����ײ�1���ۻ����ײͣ�
    int  m_iAlign;                     //�ֽڶ���
    char m_sDescription[256];     //�ײ�������Ϣ
    char m_sProductOFFName[UTF8_MAX];       //�ײ�����
    ui32 m_uiEffDate;         //��Чʱ��
    ui32 m_uiExpDate;         //ʧЧʱ��
};
class HssOrderResourceCCA : public GeneralTpl<OrderResourceCCA,__DFT_T2_,HSSCMD_ORDERRESOURCE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(OrderResourceCCA &data)
    {
        return __add1(data);
    }

    int  getReq(OrderResourceCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<OrderResourceCCA *> &vData)
    {
        return __getValue(vData);
    }
};


//7.2.5.6.1ͨ��Ⱥʵ����ѯȺ��Ա
struct QueryGroupMemberCCR
{
    ul64 m_ulGroupId;   //Ⱥ��ʶ
    ui32 m_uiQueryType; //��ѯ���ͣ�1����ѯ���¼�¼��2����ѯ���м�¼��
    int  m_iAlign;      //�ֽڶ���
};
class HssQueryGroupMemberCCR : public GeneralTpl<QueryGroupMemberCCR,__DFT_T2_,HSSCMD_GROUPMEMBER_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryGroupMemberCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryGroupMemberCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryGroupMemberCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//����CCA
struct QueryGroupMemberCCA
{
    char m_sMemberDesc[UTF8_MAX];      //Ⱥ��ʵ����Ա����
    ui32 m_uiMemberTypeId;             //Ⱥ��ʵ����Ա���ͱ�ʾ
    ui32 m_uiMemberRoleId;             //Ⱥ��ʵ����Ա��ɫ��ʶ
    ul64 m_ulServId;                   //����Ʒʵ����ʶ
    ui32 m_uiEffDate;         //��Чʱ��
    ui32 m_uiExpDate;         //ʧЧʱ��
    ul64 m_ulMemberObjectId;           //Ⱥ��ʵ����Ա�����ʶ
    char m_sAccNbr[UTF8_MAX];          //��Ա����
};
class HssQueryGroupMemberCCA : public GeneralTpl<QueryGroupMemberCCA,__DFT_T2_,HSSCMD_GROUPMEMBER_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryGroupMemberCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryGroupMemberCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryGroupMemberCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.6.2ͨ��Ⱥ��Ա����Ⱥ��Ϣ
//CCR
struct QueryGroupCCR
{
    char m_sAccNbr[UTF8_MAX];     //��Ա����
    ui32 m_uiQueryType;           //��ѯ���ͣ�1����ѯ���¼�¼��2����ѯ���м�¼��
    int  m_iAlign;                //����
};

class HssQueryGroupCCR : public GeneralTpl<QueryGroupCCR,__DFT_T2_,HSSCMD_GROUP_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryGroupCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryGroupCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryGroupCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//����CCA
struct QueryGroupCCA
{
    ul64 m_ulGroupId;           //Ⱥ��ʵ����ʶ
    ui32 m_uiGroupTypeId;       //Ⱥ�����ͱ�ʶ
    int  m_iAlign;
    char m_sGroupDesc[UTF8_MAX];//Ⱥ��ʵ������
};
class HssQueryGroupCCA : public GeneralTpl<QueryGroupCCA,__DFT_T2_,HSSCMD_GROUP_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryGroupCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryGroupCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryGroupCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.7�����ͷ��ѯ
//CCR
struct IsExistHeadCCR
{
    char m_sHeadType[UTF8_MAX];  //��ͷ����
    char m_sAccNbr[UTF8_MAX];    //�绰����
    ui32 m_uiHomeAreaCode;       //����������
    int  m_iAlign;               //�ֽڶ���
};
class HssIsExistHeadCCR : public GeneralTpl<IsExistHeadCCR,__DFT_T2_,HSSCMD_EXISTHEAD_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(IsExistHeadCCR &data)
    {
        return __add1(data);
    }

    int  getReq(IsExistHeadCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<IsExistHeadCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct IsExistHeadCCA
{
    ui32 m_iQueryResult;   //��ѯ���
    int  m_iAlign;         //�ֽڶ���
};
class HssIsExistHeadCCA : public GeneralTpl<IsExistHeadCCA,__DFT_T2_,HSSCMD_EXISTHEAD_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(IsExistHeadCCA &data)
    {
        return __add1(data);
    }

    int  getReq(IsExistHeadCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<IsExistHeadCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.8������ѯ�������룩
//CCR
struct QueryHomeAreaByAccNbrCCR
{
    char m_sAccNbr[UTF8_MAX];  //�绰����
};
class HssQueryHomeAreaByAccNbrCCR : public 
GeneralTpl<QueryHomeAreaByAccNbrCCR,__DFT_T2_,HSSCMD_HOMEBYACCNBR_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryHomeAreaByAccNbrCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryHomeAreaByAccNbrCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryHomeAreaByAccNbrCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct QueryHomeAreaByAccNbrCCA
{
    ui32 m_uiHomeAreaCode;           //�û�����������
    int  m_iAlign;                   //�ֽڶ���
    char m_sProvincecode[UTF8_MAX];  //����ʡ����
};
class HssQueryHomeAreaByAccNbrCCA : public 
GeneralTpl<QueryHomeAreaByAccNbrCCA,__DFT_T2_,HSSCMD_HOMEBYACCNBR_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryHomeAreaByAccNbrCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryHomeAreaByAccNbrCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryHomeAreaByAccNbrCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.9������ѯ����IMSI��
//CCR
struct QueryHomeAreaByIMSICCR
{
    char m_sIMSI[UTF8_MAX];    //IMSI��
};
class HssQueryHomeAreaByIMSICCR : public 
GeneralTpl<QueryHomeAreaByIMSICCR,__DFT_T2_,HSSCMD_HOMEBYIMSI_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryHomeAreaByIMSICCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryHomeAreaByIMSICCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryHomeAreaByIMSICCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct QueryHomeAreaByIMSICCA
{
    ui32 m_uiHomeAreaCode;           //�û�����������
    int  m_iAlign;                   //�ֽڶ���
    char m_sProvince_code[UTF8_MAX]; //����ʡ����
};

class HssQueryHomeAreaByIMSICCA : public 
GeneralTpl<QueryHomeAreaByIMSICCA,__DFT_T2_,HSSCMD_HOMEBYIMSI_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryHomeAreaByIMSICCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryHomeAreaByIMSICCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryHomeAreaByIMSICCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.10H���ѯ
//CCR
struct QueryHCodeCCR
{
    char m_sAccNbr[UTF8_MAX];      //�绰����
    ui32 m_uiQueryType;            //��ѯ��ʽ��1:��ѯ���¼�¼��2����ѯȫ����¼��
    int  m_iAlign;                 //�ֽڶ���
};

class HssQueryHCodeCCR : public GeneralTpl<QueryHCodeCCR,__DFT_T2_,HSSCMD_HCODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryHCodeCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryHCodeCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryHCodeCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct QueryHCodeCCA
{
    char m_sHead[UTF8_MAX];       //�Ŷ�
    ui32 m_uiEmulatoryPartnerId;  //��Ӫ�̱�ʶ
    ui32 m_uiHomeAreaCode;        //����������
    char m_sEffDate[UTF8_MAX];    //��Чʱ��
    char m_sExpDate[UTF8_MAX];    //ʧЧʱ��
};

class HssQueryHCodeCCA : public GeneralTpl<QueryHCodeCCA,__DFT_T2_,HSSCMD_HCODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryHCodeCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryHCodeCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryHCodeCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.11���غ�ͷ���ѯ
//CCR
struct QueryLocalHeadCCR
{
    char m_sAccNbr[UTF8_MAX];      //�绰����
    ui32 m_uiQueryType;            //��ѯ��ʽ��1:��ѯ���¼�¼��2����ѯȫ����¼��
    int  m_iAlign;                 //�ֽڶ���
};
class HssQueryLocalHeadCCR : public GeneralTpl<QueryLocalHeadCCR,__DFT_T2_,HSSCMD_LOCALHEAD_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryLocalHeadCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryLocalHeadCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryLocalHeadCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct QueryLocalHeadCCA
{
    ui32 m_uiHeadId;                  //��ͷ��ʶ
    char m_sHead[UTF8_MAX];           //��ͷ
    ui32 m_uiEmulatoryPartnerId;      //��Ӫ�̱�ʶ
    ui32 m_uiExchangeId;              //Ӫҵ����ʶ
    char m_sEffDate[UTF8_MAX];        //��Чʱ��
    char m_sExpDate[UTF8_MAX];        //ʧЧʱ��
};
class HssQueryLocalHeadCCA : public GeneralTpl<QueryLocalHeadCCA,__DFT_T2_,HSSCMD_LOCALHEAD_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryLocalHeadCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryLocalHeadCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryLocalHeadCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.12�����ж�
//CCR
struct QueryBorderRoamingCCR
{
    ui32 m_uiBorderZone;            //�ٽӱ���������
    char m_sMscID[UTF8_MAX];        //��������ʶ
    char m_sCellID[UTF8_MAX];       //���ѱ�ʶ
    char m_sStartTime[UTF8_MAX];    //ʱ��
};

class HssQueryBorderRoamingCCR : public 
GeneralTpl<QueryBorderRoamingCCR,__DFT_T2_,HSSCMD_BORDERROAM_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryBorderRoamingCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryBorderRoamingCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryBorderRoamingCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct QueryBorderRoamingCCA
{
    ui32 m_uiQueryResult;    //��ѯ���
    int  m_iAlign;           //�ֽڶ���
};
class HssQueryBorderRoamingCCA : public 
GeneralTpl<QueryBorderRoamingCCA,__DFT_T2_,HSSCMD_BORDERROAM_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryBorderRoamingCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryBorderRoamingCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryBorderRoamingCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.13C������������Ӫ��
//CCR
struct QueryIbrstCodeCCR
{
    ui32 m_uiQuerytype;              //2 ������ 1��ָ����Ӫ����Ϣ
    int  m_iAlign;                   //�ֽڶ���
    char m_sCarrierCode[UTF8_MAX];   //������Ӫ�̴���
};
class HssQueryIbrstCodeCCR : public GeneralTpl<QueryIbrstCodeCCR,__DFT_T2_,HSSCMD_IBRSTCODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryIbrstCodeCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryIbrstCodeCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryIbrstCodeCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct QueryIbrstCodeCCA
{
    char m_sCarrierName[UTF8_MAX];         //�ݷ���Ӫ�����ƣ�Ӣ�ģ�
    char m_sCtry[UTF8_MAX];                //�ݷ���Ӫ�����ڹ��Ҵ���
    ui32 m_uiSectorID;                     //�����ʷ���ID
    int  m_iAlign;                         //�ֽڶ���
    char m_sEffDate[UTF8_MAX];             //��Ч����
    char m_sExpDate[UTF8_MAX];             //ʧЧʱ��
};
class HssQueryIbrstCodeCCA : public GeneralTpl<QueryIbrstCodeCCA,__DFT_T2_,HSSCMD_IBRSTCODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryIbrstCodeCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryIbrstCodeCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryIbrstCodeCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.14��ͬ��ʽIMSI���ѯ
//CCR
struct QueryImsiCodeCCR
{
    char m_sIMSI[UTF8_MAX];    //IMSI��
};
class HssQueryImsiCodeCCR : public GeneralTpl<QueryImsiCodeCCR,__DFT_T2_,HSSCMD_IMSICODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryImsiCodeCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryImsiCodeCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryImsiCodeCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct QueryImsiCodeCCA
{
    ui32 m_uiSponsorCode;             //������Ӫ��
    ui32 m_uiHomeAreaCode;            //����������
    char m_sEffDate[UTF8_MAX];        //��Ч����
    char m_sExpDate[UTF8_MAX];        //ʧЧ����
    ui32 m_uiImsiType;                //IMSI ����0����ͨ�ŶΣ�1��һ������
    int  m_iAlign;                    //�ֽڶ���
};

class HssQueryImsiCodeCCA : public GeneralTpl<QueryImsiCodeCCA,__DFT_T2_,HSSCMD_IMSICODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryImsiCodeCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryImsiCodeCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryImsiCodeCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.15MIN���ѯ
//CCR
struct QueryMinCodeCCR
{
    char m_sMinNbr[UTF8_MAX];         //MIN��
};

class HssQueryMinCodeCCR : public GeneralTpl<QueryMinCodeCCR,__DFT_T2_,HSSCMD_MINCODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryMinCodeCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryMinCodeCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryMinCodeCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct QueryMinCodeCCA
{
    ui32 m_uiHomeAreaCode;          //����������
    char m_sEffDate[UTF8_MAX];      //��Ч����
    char m_sExpDate[UTF8_MAX];      //ʧЧ����
    ui32 m_uiUserFlag;              //�û���ʶ 1 ���ڣ�2���ʡ�
    ui32 m_uiUserType;              //�Ŷα�ʶ 2:189�Ŷ�1:133��153�Ŷ�0:������Ӫ��
    char m_sHomeCountryCode[UTF8_MAX]; //�������Ҵ���
    char m_sHomeCarrierCode[UTF8_MAX]; //����C����Ӫ�̴���
};

class HssQueryMinCodeCCA : public GeneralTpl<QueryMinCodeCCA,__DFT_T2_,HSSCMD_MINCODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryMinCodeCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryMinCodeCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryMinCodeCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.16Я��ת���ж�
//CCR
struct QueryNPCodeCCR
{
    char m_sAccNbr[UTF8_MAX];    //�绰����
    ui32 m_uiQueryType;          //��ѯ���ͣ�1����ѯ���¼�¼��2����ѯ���м�¼��
    int  m_iAlign;               //�ֽڶ���
};
class HssQueryNPCodeCCR : public GeneralTpl<QueryNPCodeCCR,__DFT_T2_,HSSCMD_NPCODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryNPCodeCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QueryNPCodeCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryNPCodeCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct QueryNPCodeCCA
{
    char m_sInNetID[UTF8_MAX];   //����ID��Я�뷽��
    char m_sOutNetID[UTF8_MAX];  //����ID��Я������
    char m_sOwnNetID[UTF8_MAX];  //����ID������ӵ�����磩
    char m_sEffDate[UTF8_MAX];   //NP ����ʱ��
};
class HssQueryNPCodeCCA : public GeneralTpl<QueryNPCodeCCA,__DFT_T2_,HSSCMD_NPCODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QueryNPCodeCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QueryNPCodeCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QueryNPCodeCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.5.17SP�ж�
//CCR
struct QuerySPCodeCCR
{
    char m_sSpCode[UTF8_MAX];    //SP��ʶ
    ui32 m_uiQueryType;          //��ѯ���ͣ�1����ѯ���¼�¼��2����ѯ���м�¼��
    int  m_iAlign;               //�ֽڶ���
};

class HssQuerySPCodeCCR : public GeneralTpl<QuerySPCodeCCR,__DFT_T2_,HSSCMD_SPCODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QuerySPCodeCCR &data)
    {
        return __add1(data);
    }

    int  getReq(QuerySPCodeCCR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QuerySPCodeCCR *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct QuerySPCodeCCA
{
    char m_sSpDescription[UTF8_MAX];  //Sp���������ģ�
    ui32 m_uiProvinceID;              //SP����ʡ��ʶ
    ui32 m_uiSpFlag;                  //SP ��ʶ 0��������ͨ����sp4���ƶ�ҵ���sp5���ƶ�ҵ���ſͻ���sp
    char m_sEffDate[UTF8_MAX];        //��Ч����
    char m_sExpDate[UTF8_MAX];        //ʧЧ����
};
class HssQuerySPCodeCCA : public GeneralTpl<QuerySPCodeCCA,__DFT_T2_,HSSCMD_SPCODE_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(QuerySPCodeCCA &data)
    {
        return __add1(data);
    }

    int  getReq(QuerySPCodeCCA *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<QuerySPCodeCCA *> &vData)
    {
        return __getValue(vData);
    }
};

//7.2.4.1.3ҵ����Ԫ�ṩ������Ϣ��ѯ�ӿ�
//CCR
struct stService
{
    char m_sQueryTypeID[8]; //��ѯ���ͱ�ʶ
    char m_sQueryTime[24];  //��̬��ѯ��ֹʱ��
};

class HssDataReportCCR : public GeneralTpl<stService,__DFT_T2_,HSSCMD_DATAREPORT_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(stService &data)
    {
        return __add1(data);
    }

    int  getReq(stService *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<stService *> &vData)
    {
        return __getValue(vData);
    }
};

//CCA
struct stRltSrv
{
    unsigned int m_uiSrvRltCode;
    char m_sParaFieldRlt[132];
    unsigned int m_uiCustInfo;
    unsigned int m_uiAcctInfo;
    unsigned int m_uiServInfo;
    unsigned int m_uiProcInfo;
};

class HssDataReportCCA : public GeneralTpl<stRltSrv,__DFT_T2_,HSSCMD_DATAREPORT_QUERY>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(stRltSrv &data)
    {
        return __add1(data);
    }

    int  getReq(stRltSrv *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<stRltSrv *> &vData)
    {
        return __getValue(vData);
    }
};

struct struInfoApproveR
{
    char sOfferId[16];
    char sOfferCode[16];
    char sState[8];
    char sApproveInfo[128];
};

struct struFaildMsg
{
    char sMsg[128];
};

class InfoApproveCCR : public GeneralTpl<struInfoApproveR, __DFT_T2_, HSSCMD_OFFERDOMAIN_APPROVE>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(struInfoApproveR &data)
    {
        return __add1(data);
    }

    int  getReq(struInfoApproveR *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<struInfoApproveR *> &vData)
    {
        return __getValue(vData);
    }
};

class InfoApproveCCA : public GeneralTpl<struFaildMsg, __DFT_T2_, HSSCMD_OFFERDOMAIN_APPROVE>
{
public:
    void clear()
    {
        __clear();
    }
    bool addReq(struFaildMsg &data)
    {
        return __add1(data);
    }

    int  getReq(struFaildMsg *&data)
    {
        return __getValue(data);
    }

    int  getReq(vector<struFaildMsg *> &vData)
    {
        return __getValue(vData);
    }
};




#endif/*__HSSCMDGENERAL_H_INCLUDED_*/
