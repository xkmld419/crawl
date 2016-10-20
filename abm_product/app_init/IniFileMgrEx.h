#ifndef	_INIT_FILE_MGR_EX_H_
#define	_INIT_FILE_MGR_EX_H_
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "Log.h"
#include <vector>
#include "SHMData.h"
#include "WriteIni.h"
#include <string>
#include <iostream>

using namespace std;
#define		MAX_SECTION_LEN_INIT				64
#define		MAX_KEY_NAME_LEN		512	
#define		MAX_KEY_VALUE_LEN		512	
#define		MAX_REMARK_LEN				256

#define		MAX_INI_KEY_NUM				4096
#define		MAX_KEY_NUM					64
#define		MAX_SECTION_NUM			128
#define		MAX_STACK_LEN					3

#define		MAX_DATE_LEN					16
#define		MAX_TMP_STRING_LEN		MAX_KEY_NAME_LEN+MAX_DATE_LEN


#define   MAX_SECTION_LEN  32
//2.8����ʹ�õĲ�������ṹ������ʧЧʱ��
#define		MAX_SECTION_NAME_LEN		256
#define		MAX_PARAM_NAME_LEN			256
#define		MAX_INI_VALUE_LEN			128
#define		MAX_DATE_LEN				16
typedef	struct	_SYS_PARAM_INFO_EX_
{
	char	sSectionName[MAX_SECTION_NAME_LEN];
	char	sParamName[MAX_PARAM_NAME_LEN];
	char	sParamValue[MAX_INI_VALUE_LEN];
	char	sEffDate[MAX_DATE_LEN];
	char	sExpDate[MAX_DATE_LEN];
	unsigned	int	uiNext;
}SysParamInfoEx;


#define		EFF_DATE_NAME				"effdate"			//�жϲ��������Ƿ���eff_date
#define		NEW_VALUE						"new_value"
#define		OLD_VALUE						"value"


//1,����.2,�޸�.3,ɾ��
typedef enum { OP_NEW=1,OP_FIX,OP_DEL} OP_FLAG;


//ini�ļ��ṹ��������
typedef	struct	_SYS_PARAM_INFO_EX_OP_
{
	SysParamInfoEx	stParamInfoEx;
	OP_FLAG			enOp;
}SysParamInfoExOp;

class CIniFileMgrEx
{
public:
	CIniFileMgrEx(long lShmKeyID,unsigned int uiSize);
	~CIniFileMgrEx(void);	

	//���ļ���װ��Ini����
	bool LoadIniFromFile(const char* cpsFileName);

	//����װ��Ini�ļ�
	//ֻ�����µ�����LoadIniFromFile����
	bool ReLoadIniFromFile(void);

	//�������ڴ�����
	void ClearShmData(void);

	//�������ڴ�
	void DestoryShm(void);

	//��ȡ���е�Section�ֶ�ֵ
	//��ʹ������ӿں�����Ľӿڷ���ʱ����ʹ�ýṹ�������pNextָ�����
	//��һ���ṹ��
	bool GetAllIniKey(vector<SysParamInfoEx>& arraySysParamInfoEx,
		const char* sSectionName =0,
		const char* sParamName =0);

	//��ȡ��ǰʱ��ʹ�õ�Section�ֶ�ֵ
	bool GetCurrentIniKey(vector<SysParamInfoEx>& arraySysParamInfoEx,
		const char* sSectionName =0,
		const char* sParamName =0);

	//��ȡ��ǰSection�ֶε�ֵ
	bool GetCurrentIniKeyValue(const char* sSectionName,
		const char* sParamName,
		string& strValue);

	//��ȡ��һ��ʱ����Ч���ֶ�
	bool GetNextIniKey(vector<SysParamInfoEx>& arraySysParamInfoEx,
		const char* sSectionName =0,
		const char* sParamName =0,const char* sDate=0);

	//��ȡ��һ��Section�ֶε�ֵ
	bool GetNextIniKeyValue(const char* sSectionName,
		const char* sParamName,
		string& strValue,const char* sDate=0);

	//�޸Ĳ���
	bool ModifyKey(SysParamInfoExOp& stParamInfoOp);

	//������ͬ�����ļ�
	bool SyncToFile(const char* strFileName);

	////���ӹ����ڴ�
	//bool AttachShm(const char* cpsKey);
	
	//���ַ������ж�ȡ
	unsigned int LoadFromStream(istream& inputStream);

private:
	//��ʼ������
	bool Init(void);
	bool InitShm(void);
	
	//��������
	void Clear(void);
	bool GetIniKey(vector<SysParamInfoEx>& arraySysParamInfoEx,
		const char* sSectionName,const char* sParamName,bool bCurrent,
		const char* sDate);
	//�޸Ľ��
	bool AnalysisLine(string& strLine,string& strSection,
			string& strKey,string& strValue);

	bool SaveKey(const char* cpsFileName,SysParamInfoExOp& stParamInfoOp);
	void freeAll(void);
private:
	SHMData<SysParamInfoEx>* m_poParamInfoEx;	//�����ڴ�ṹ
	unsigned int m_iNum;						//SysParamInfoEx�ṹ����
	long m_lShmKey;								//�����ڴ��ֵ
	unsigned int m_uiSize;						//�����ڴ��С
	string m_strMemLockKey;						//�����ڴ�����ֵ
	string m_strFileName;						//ini�ļ���	
	bool m_bAttached;
};

#endif

