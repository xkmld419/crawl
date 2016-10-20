/*VER: 2*/ 
#ifndef TIBS_CHECK_FUNCTION_H_
#define TIBS_CHECK_FUNCTION_H_
/*-----------------------------------------
* ����һ��ȫ�ֵĺ꣬���ڱ�ʶFUNC_ID
* ��Щ����ǰ׺CK_��ʼ���Է�ֹ����
-------------------------------------------*/
#define     CK_IS_DEC           1   //����Ƿ�Ϊʮ��������eg:���������Ƿ�Ϊ15λȫ���֡�
#define     CK_BEGIN_WITH       2   //����ַ������Ƿ���ĳ����ͷ��ͷ��
#define     CK_GET_ORG          3   //����ܷ��ȡORG_ID
#define     CK_AREA_CODE        4   //У���������Ƿ�Ϸ�
#define     CK_PROVIN_CODE      5   //����ļ����е�ʡ�����Ƿ�������ֵ��ʡ��������ͬ
#define     CK_DATE_FORMAT      6   //���ʱ���ʽ�Ƿ�Ϸ���Ĭ��Ϊ"YYYYMMDDHH24MISS"
#define     CK_BEYOND_DATE      7   //�Ƚ�ʱ���ֵ
#define     CK_LARGER_THEN      8   //����ĳ����
#define     CK_LESS_THEN        9   //С��ĳ����
#define     CK_GET_ORG_2        10  //����MSC��LAC��CELL_ID��ȡORG_ID
#define     CK_IS_OCT           11  //����Ƿ�Ϊ�˽�����
#define     CK_IS_HEX           12  //����Ƿ�Ϊʮ�˽�����
#define     CK_EQUAL            13  //����ĳ����
#define     CK_LENGTH           14  //��ⳤ���Ƿ���ָ����Χ
#define     CK_DO_FUNC          15  //ִ����������
#define     CK_IS_AREACODE      16  //��������Ƿ�Ϸ�����������
#define     CK_GET_ATTR         17  //��ȡ����ֵ
#define     CK_CMP_TWO_FUNC     18  //�Ƚ�����������ִ�н���Ƿ���ͬ��
#define     CK_GET_CARRIER      19  //��ȡ��������
#define     CK_IS_MSC           20  //���MSC�Ƿ�Ϸ�
#define     CK_PIPE             21  //�ܵ���ǰһ��������ִ�н��������Ϊ��һ�����������롣
#define     CK_FORMAT_CARRIER   22  //������������
#define     CK_FORMAT_AREACODE  23  //������������
#define     CK_GET_PARTNER      24  //��ȡ��Ӫ��ID
#define     CK_IS_INNER         25  //�ж����������Ƿ�Ϊ�����û�
#define     CK_FORMAT_0086      26  //���������п��ܳ��ֵ�0086���Ҵ��룬ʹ���Ϊ��׼����
#define     CK_GET_LONG_TYPE    27  //������;����
#define     CK_GET_ROAM_TYPE    28  //������������
#define     CK_MUX              29  //�˷�
#define     CK_DIV              30  //����
#define     CK_CMP_CHAROFSTRING		31  //�ж��ַ������ַ��Ƿ��ڹ涨��Χ��
#define	  CK_SUBSTR					32 //�����ַ�������������ַ����Ƚ�
#define	CK_ZONE				33 //�ж�ȡֵ�Ƿ��ڸ��������У���ȫƥ�䣻
#define	CK_ZONE_MAX		34 //�ж�ȡֵ�Ƿ��ڸ��������У��ƥ�䣻

/*--------------CheckFunction Class----------------------*/
#include "RecordCheck.h"

using namespace std;

class CheckFunction
{
public:
	CheckFunction();
	~CheckFunction();

	//ִ����iFuncID��Ӧ�ĺ�������Щ����������ֻ�Ƕ�strVal���ֵ�����жϣ�
	//����true��false�������������κθı䡣�����ĺ������ֽ�ν��-pridicate
	static bool doFunction(int iFuncID, const EventSection *poEventSection, 
		int iTAG, const CheckArgument *poFirstArgument, string *psResult=NULL,
		string *psInput=NULL);
		
    static bool isNumber(const char *str, const CheckArgument *poTable,
        const char *pFormat=NULL);
        
	static bool beginWith(const char *str, const CheckArgument *poTable, 
	    char *pHead=NULL);
	    
	static bool getOrgID(const char *sNumber, int &iOrgID, 
        const char* pTime, const CheckArgument *poTable, char *pHead=NULL,
        int *piShift=NULL);
        
    static bool getOrgID(const EventSection *poEventSection, int &iOrgID, 
       const CheckArgument *poTable, char *pHead=NULL);
        
    static bool getAreaCode(const char *sNumber, char *pAreaCode, int iSize, 
        const char *pTime, const CheckArgument *poTable);
        
    static bool isBeyondDate(const char *pTime, const CheckArgument *poTable,char *sFileName);
    
    static bool isAreaCode(const char *sNumber, char *pAreaCode, int iSize, 
        const char *pTime, const CheckArgument *poTable);
        
    static bool cmpTwoFunc(const EventSection *poEventSection, 
        const CheckArgument *poTable);
    static bool formatCarrier(const EventSection *poEventSection, 
        int &iCarrierType, char *psResult=NULL);
    static bool isMSC(const char *sMSC, const CheckArgument *poTable, 
        char *pHead);
    static bool pipe(const EventSection *poEventSection, 
        const CheckArgument *poTable, char *pResult=NULL);
    static bool formatAreaCode(const EventSection *poEventSection,
        char *psResult=NULL);
    static bool getPartnerID(const char *sAccNBR, const char *pTime, 
        int &iPartnerID);
    static bool isInner(const EventSection *poEventSection, const char *pTime);
    static bool getLongTypeAndRoamID(const EventSection *poEventSection, 
    int &iLongType, int &iRoamID);
    //yuanp �����ж��ַ����������ַ��Ƿ�Ϸ�
    static bool cmpCharofString(const char *str,const CheckArgument *poTable,char &cNotFind);
    static bool cmpSubStr(const char *str,const CheckArgument *poTable);
    static bool check_zone(const char *str,const CheckArgument *poTable,int iMaxFlag);    
private:
    static bool format0086(const char *fromNBR, char *toNBR, int *trimLen=NULL, 
        int *piShift=NULL);
    static void to_Lower(char *sStr);
    static void to_Upper(char *sStr);
    static bool getFileTime(char *sTag,char *sFileTime,char *sFileName);
};

#endif /* End of _CHECK_FUNCTION_H */
