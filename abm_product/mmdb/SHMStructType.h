#ifndef SHM_STRUCT_TYPE_INCLUDE_H
#define SHM_STRUCT_TYPE_INCLUDE_H

#define PARAM_STRUCT_NUM		1023//���֧�ֽṹ������ID��Ŀ,���ʹ�1��ʼ����,���ֵΪ2^n -1
//�������ӽṹ������ID
#define TEMPLATE_MAP_SWITCH_SHMTYPE		1 
#define OBJECT_DEFINE_SHMTYPE		2
#define MAP_PARSER_GROUP_SHMTYPE		3


// ��¼Ψһ�� <m_lSwitchID,m_sTrunkIn>��
struct SwitchIDTrunkInRec
{
	   long m_lSwitchID;
	   char m_sTrunkIn[20];
};

struct strLocalAreaCode
{
	char m_sAreaCode[32];
};

struct CycleTypeID
{
	int  m_iCycleTypeID;
	char m_sCycleBeginDateType[9];
};

class MServAcctInfo
{

 public:
 	
 	//��ϵ��ʶ
	long m_lServAcctID;

    //## �˻���ʶ
    long m_lAcctID;

    //## ����Ʒ��ʶ
    long m_lServID;

    //## ��Ŀ��
    int m_iAcctItemGroup;

    //## ״̬����Ϊ����ǻ
    //## 1Ϊ���0Ϊ�ǻ
    int m_iState;
	//
	int m_iBillingModeID;
	int m_iShareFlag;
	//
    char m_sEffDate[16];

    char m_sExpDate[16];

    //## ͬһ����Ʒ��ʶ��һ�����ƹ�ϵ
    unsigned int m_iNextOffset;
};

/*--std��Ϣ����--*/
//��׼8�ֽ����ݽṹ�������ֳ�
#define STD_CHAR8_LEN	8

// 'STD���ݽṹ��'
struct StdChar8
{
    char sData[STD_CHAR8_LEN];
};

//�����˽ṹ���Ӧ��Ϣ��
struct StdChar8DataDesc
{
	long lStructID;//���
	char sStructName[32];//����
	int iStructSize;//�ṹ���С
	unsigned int iMaxNum;//��Ҫrecord MaxNum[�ⶼ�ǰ����Լ��ṹ���������ֵ]
	unsigned int iUsedPos;//�Ѿ�ʹ�õ���ָ��,��0��iMaxNum-1
	int iIndexNum;//������
	int iFlashTimes;//��չ��Ϣ[]
};

#endif