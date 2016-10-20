#ifndef SYS_PARAMINFO_H
#define SYS_PARAMINFO_H

#define MAX_P_VAL_LEN  1024
#define MAX_PN_VAL_LEN  512
#define MAX_PARAM_SECT_NUM 1024

struct ParamHisRecord //����
{
	char sSectName[MAX_PN_VAL_LEN];
	char sParamName[MAX_P_VAL_LEN];
	char sParamOrValue[MAX_P_VAL_LEN];
	char sParamValue[MAX_P_VAL_LEN];
	char sDate[16];	
	long  lResult; //ֻ�����޸�ֵ���ص��޸Ľ��
	char sWholeParamName[MAX_P_VAL_LEN+MAX_PN_VAL_LEN];   // ������(�����˶���.��������ȫ��)
};

// ��ʾ�ýṹ�����
typedef struct TSysParamInfo 
{    
    /* ��Ҫ���� */
	char sSrcSectName[MAX_PN_VAL_LEN];	//ԭʼ����[]
	char sSrcParamName[MAX_P_VAL_LEN]; //ԭʼ������[�������ֶ���ʾ��ͬ]
    char sParamName[MAX_P_VAL_LEN+MAX_PN_VAL_LEN];   // ������(�����˶���.��������ȫ��)
    char sParamOldVal[MAX_P_VAL_LEN]; // ����ԭֵ
    char sParamNewVal[MAX_P_VAL_LEN]; // ������ֵ
    char sDate[16];      // ��Чʱ��(��ʽ��yyyy mm dd hh mm ss����)
	int  iResult; //ֻ�����޸�ֵ���ص��޸Ľ��
    //char sExpDate[16];      // ʧЧʱ��(��ʽͬ��)
	//char sNormalValue[512];
    TSysParamInfo *pNext; //��������ò���
}TSysParamInfo;

// ģ��->����
typedef struct TSysMoudleHead
{
    char sMoudle[128];  //��������ģ����
	int iUsedNum;
	int iTotalNum;  // ���ָ���Ƕ��µ�ģ���ж��ٸ�����
    TSysParamInfo *pSysParamInfo; //�����һ������������Ĳ����ռ�
    TSysMoudleHead *pNext; //���������
}TSysMoudleHead;

// ��->ģ��
typedef struct TSysSectHead
{
    char sSectName[MAX_PARAM_SECT_NUM];       //���������ε�����
    int iParamTotalNum;      //��������
	int iSectionNum;    //�Ѿ�ʹ�õĶθ���(ȫ����)
	bool Used;    		//�˶��Ƿ��Ѿ�ʹ��
	int iMoudleNum;     //ģ�����
    int iRefreshFlag;   //����Ǳ�ʶ��Ϣ���µģ�����Ҫ��¼һ��ˢ�¼�¼ֵ
    TSysMoudleHead *pSysMoudleHead;
	TSysSectHead *pNext;
    //TSysParamInfo SysParamInfo;
}TSysInfoHead;


#endif

