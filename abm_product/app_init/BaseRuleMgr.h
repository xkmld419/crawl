#ifndef _BASERULE_MGR_H_
#define _BASERULE_MGR_H_

#include "BaseFunc.h"
#include "IniFileMgr.h"
#include "Log.h"
#include "HashList.h"

#define MAX_ENUM_NUM	  30  //���ֵ�������,ö����Ŀ����ֵ
#define MAX_LINK_NUM	  2  //���ֵ�������,����������Ŀ����ֵ
#define MAX_LONG_NUM	  18  //���ֵ�Ͳ��ܸĴ���,���Ը�С
#define MAX_GROUP_NUM	  100  //���������Ŀ
#define MAX_GROUP_PARAM_NUM	  30  //������������������Ŀ
#define MAX_DISOBYE_NUM	  64  //���ܳ��ֵ��ַ�

//�ɹ�����0
//������Ϣ��ʶ����
#define MAX_ERR	1
#define MIN_ERR	2
#define ENUM_ERR	3
#define EFF_ERR_LESS	4
#define EFF_ERR_NONEED	5
#define CHANGE_ERR	6
#define PARAM_INFO_NONEED	7
#define IN_OR_DECREASE_ERR	8
#define IN_CREASE_ERR           11
#define DE_CREASE_ERR           12 
#define MAX_MIN_SET_ERR			13
#define TIME_ERR				14
#define PARAM_INFO_NEED			15
#define PARAM_FORMAT_ERR	9
#define RULE_UNLOAD_ERR		10
#define CHAR_OR_NUM_ERR		16
#define ISNOTNUM_ERR		17
#define DISOBEY_ERR			18

// ��������(���ȼ��������Сֵ,ö��,��ʱ������Ĭ��ֵ)
struct BaseSysParamRule
{
	char sParamName[MAX_KEY_NAME_LEN];  //������
	char sMaxVal[MAX_LONG_NUM];	//���ֵ //.min
	char sMinVal[MAX_LONG_NUM];	//���ֵ //.max
	char sEnumVal[MAX_ENUM_NUM][MAX_KEY_VALUE_LEN];	//ö��ֵ //.enum
	int  iEnumPos; 
	char sSuggestVal[MAX_KEY_VALUE_LEN];//�Ƽ�ֵ //.suggest
	char sDefaultVal[MAX_KEY_VALUE_LEN];//Ĭ��ֵ //.default
	char sDesc[MAX_KEY_VALUE_LEN];//˵��	//.desc
	char sVal[MAX_KEY_VALUE_LEN]; //.value
	int  iEffDateFlag; // ��Ч����ģʽ;1-������Ч ;2-������Ч ;3-�����޸�Ҫ����Ч����(����������ֶλ���������ֵ�ݲ���������Ч�����ֶ�)
	int  iKeyWordFlag; // �����ֶα�ʶ;1-����ֶ��������ֶ� //.keyword
	int  iValueChange; // 1-ֻ������ //.change; 2-�����޸� ; 3-����ƥ��	;//4-ֻ�ܼ�С
	char sValueChange[MAX_KEY_VALUE_LEN];//����ƥ��
	int  iParamNeed;// >=1 ������ڵĲ��� 
	char cArbi; // �����ٲõĲ��� --'Y'
	char sTime24Start[14]; //�������õĿ�ʼʱ��ֵ
	char sTime24End[14];   //�������õĽ���ʱ��ֵ ����24Сʱ��ʽ
	char sLinkParamName[MAX_LINK_NUM][MAX_KEY_NAME_LEN+MAX_KEY_VALUE_LEN+1];//���ӵĲ���:������ ��.����������ʽ
	char sGroupParamName[MAX_SECTION_LEN+MAX_KEY_NAME_LEN];//���������Ĳ����� ����
	int  iLinkNum;//�ݲ�ʹ��
	int  iCharOrNum; //1-number ����-����������ֶ� Ĭ�϶��ǶԵ�
	char sDisobey[MAX_DISOBYE_NUM];   //���ܳ��ֵ��ַ�
	BaseSysParamRule *pNext;	
};

/* 
	������1------|-->����1-->����2-->����3
	|     		
	|
	������2------|-->����1-->����2
*/
struct BaseSysSectRule
{
	char sSectName[MAX_SECTION_LEN];//������
	BaseSysParamRule *pFirst;
	BaseSysSectRule *next;
};


//���������Ĳ����鹦��ʹ�õ� ��¼�����β�����������������Ϣ
struct BaseVarInfo
{
	char sSectName[MAX_SECTION_LEN];
	char sParamName[MAX_KEY_NAME_LEN]; 
	BaseVarInfo *next;
};

//add by hsir
class BaseRuleMgr
{
    public:
             		 BaseRuleMgr();
             		 ~BaseRuleMgr();
            
             		 //���ݶ���,��������ȡ��������(public����);���������,�����������ڷ��ض��µ�һ������;���������ڷ���NULL
             		 BaseSysParamRule *getParamRule(char *sSectNameSrc,char *sParamNameSrc); 
					 bool check(const char *srcVal,bool Show=true);
					 // srcStr�ĸ�ʽ: ��.����=ֵ,....
             		 bool check(char *srcVal,bool Show=true);
					 //_DebugΪtrue����ʾ������ʾ			 
					 void setDebugState(bool _Debug);
	public:
			 // �Բ����Ƿ�Ϻ�������ж�,str��ʽΪSection.param=value
			 bool paramParse(const char* str,bool Show=true);
			 bool paramParse(string str,string strEffDate);

			 // ����˵��,sSect��,sParam������
			 void paramExplain(char*sSect,char *sParam,bool bAfterCheck=false);
			 // ��ò��������˵��,str��ʽΪ ��.����
			 void paramParseExplain(const char* str);
			 // bAfterCheck=true��������check֮��Ĳ��������ѯ
			 void paramParseExplain(bool bAfterCheck);
			 // �����Ч���ڵ�
			 bool checkEffDate(string strWholeEffdate);
			 // �����÷ָ���ƫ����
			 int anayParam(char *sParam,char sep);//-list�������ܴ�'='
	public: //hss
			int checkNowValue(char *section, char *param, char *value, char *effdate);
			int CheckDigitString(char *section, char *param, char *value);//sgw
			int CheckDigitString(BaseSysParamRule* pParamRule, char *value);//
			// ���ܴ��ڵ��ַ� 
			int checkDisobey(char *sSect,char *sParam,char *sVal);
			int checkDisobey(BaseSysParamRule *pParamRule,char *sVal);
	public:	 /* �ṩ�������ܵĺ��� */
			void show();
			bool checkCfgParam();
			char sErrInfo[512];
	   		/* ���º������ǻ���������ԭ��[�����ù���,�����û������һ��,Ĭ���ǲ�����,��ͨ��] */
			//
			void showErrInfo(int iErrID);
			//
			int check(char *sSect,char *sParam,char *sValue,char *sEffVal,char *OldVal,bool ShowErr,int iFlag=0);		
			//�ܵļ������Ƿ�淶�ĵ��ú���
			int check(char *sSect,char *sParam,char *sValue,char *sEffVal,char *OldVal,int iFlag=0);
			//�������Ƿ��Ǳ�����ڲ���[����true]
			int checkParamNeed(char *sSect,char *sParam);
			//�������Ƿ���������Сֵ������
			int checkMinMax(char *sSect,char *sParam);		
			//��������Ч�����Ƿ���ڵ�ǰֵ
			int checkEffdate(char *sSect,char *sParam,char *sEffDate,int iFlag=0);
			//��������ֵ�Ƿ��������
			int checkValueChange(char *sSect,char *sParam,char *sValue,char *sOldVal);
			//�������Ƿ���ֻ�����ӻ���ֻ�ܼ���
			int checkInOrDecrease(const char* str);
			//��������ֵ�Ƿ����ö��ֵ����[true���Ϲ���]
			int checkEnum(char *sSect,char *sParam,char *sValue);
			//����GROUP
			bool checkGroupParam(vector<string> str, bool bShow=false);
			bool IsGroupParam(char *section,char *param);
			bool searchGroupParam(vector<string> str, string strcmp,int iNoCmpPos);
			int  getParam(char *sSrcName,char *sDscParamName,char sep);
			
			bool neddArbi(char *sSect,char *sParam);
			int checkTime(char *sSect,char *sParam,char *sValue);
			/* -------���������ĺ���------- */
			//�鿴��������ǲ���ֻ�����Ӳ��ܼ�С�Ĳ���
			//[-1��û���ù�������������0-�����˲�������,��û�Բ������������;1-ֻ������;2-�����޸�;3-����ƥ��;4-ֻ�ܼ�С]
			int showInOrDecrease(char* sSect,char *sParam);
			//������ȡ�� ���� ֵ,����ֵ:-1-ʧ��;1-�ɹ�
			int getEveryInfo(const char* str,string &sPureSect,string &sPureParam,string &sPureVal);
	public:	 /* �����ṩ�ĵ������ܵĺ��� */
			//-list �Ĳ������ܴ�'=';sVal��ʽ�Ƕ���.������ ����:SYSTEM.location=ĳ��ʵ��ֵ����Ϊ��ֵ
			bool checkListParam(const char *sVal);
			//��������ֵ�Ƿ����ֵ����
			bool IfNeedLinkParam(char *sSect,char *sParam);
			char *getLinkParam(char *sSect,char *sParam,int iPos);		
			char *getLinkParam(BaseSysParamRule *pParamRule,int iPos);
			int CheckDigitString(char *value);//sgw
    private:
			//  sPtr��ʽ��: ������ֵ
			int checkMinMax(BaseSysParamRule* pParamRule,char *sPtr);
			//  sPtr��ʽ��: ������ֵ
			int checkValueChange(BaseSysParamRule* pParamRule,char *sPtr,char *sOldVal);
			//  ����ֵ�ǹ����趨ֵ
			int checkParamNeed(BaseSysParamRule* pParamRule);
			//  
			int checkEnum(BaseSysParamRule* pParamRule,char *sPtr);
			//  
			int checkEffdate(BaseSysParamRule *pParamRule,char *sEffDateSrc,int iFlag=0);
			int checkTime(BaseSysParamRule* pParamRule,char *sPtr);
			
			BaseSysParamRule *m_pParamRuleLast;//��ʱ��¼�ϴ��õĲ�������
			char sSectLast[MAX_SECTION_LEN];//��ʱ��¼�ϴ��õĲ�����
			char sParamLast[MAX_KEY_NAME_LEN];//��ʱ��¼�ϴ��õĲ�����
			//<��.����,����ָ��>
			HashList<BaseSysParamRule*> *m_poIndex;
			//<GROUP�εĲ�����[�޶���],����ָ��>
			HashList<BaseVarInfo*> *m_poGroupIndex;
			BaseVarInfo m_oBaseVarInfo[MAX_GROUP_NUM][MAX_GROUP_PARAM_NUM];//���֧������2000�������
			int m_iCurGroupParam[MAX_GROUP_NUM];//���䵽�ĸ��±���?
			int m_iCurGroup;
			BaseVarInfo *mallocBaseVar(int iGroup);
			BaseVarInfo *MallocBaseVar(char *sGroupName);
			HashList<int> *m_poGroupNoIndex;//��������ŵĶ�Ӧ��ϵ
    private:                       
             /* �ļ�������,�������� */
			 CIniFileMgr *gpCIniFileMgr; //�ļ���ȡ��
			 BaseSysSectRule *gpBaseSysSectRule; //�׶ε�ַ			 			 			 
			 // ��ȡ������
			 int getParamName(char *sSrcName,char *sDscParamName,char sep);
			 
			 //�Ҳ�������newһ���ṹ�巵��,ֱ��ʹ������ṹ��Ϳ�����
			 BaseSysParamRule *SearchParamRule(char *sSectName,char *sParamName);
			 // ���ݶ���malloc���νṹ
			 BaseSysSectRule *MallocSectRule(char *sSectName);
			 // malloc�������ṹ
			 BaseSysParamRule *MallocParamRule(BaseSysSectRule *pSectRule);
			 
			 void addParamRule(CIniSection *pCIniSection);
			 //bool paramParse(const char* str);
			 int getType(char *sStr,char *sVal,BaseSysParamRule *pSPRule);
			 //
			 int addGroupType(BaseVarInfo *pBaseVarInfo,char *sVal,BaseSysParamRule *pSPRule);
			 //�����������
             bool loadRule();
			 bool m_bLoadRule;
			 bool m_bMonDebug;
			 bool isRuleNumeric(char* pString);
	private: //���ڼ���
			 bool getCurDate(char *sDate,int iSize);
	protected://��ʱ��¼ÿ��������Ҫ��Ϣ
			 //string m_poParamName;
			 //string m_poSectName;
			 //��char[]����string���� �еı���������
			 char m_sParamName[MAX_KEY_NAME_LEN];
			 char m_sSectName[MAX_SECTION_LEN];
			 BaseSysParamRule *m_poParamRule;
			 
			 void freeALLMalloc();
			 void freeALLParamMalloc(BaseSysSectRule *pSectRule);
};

#endif
