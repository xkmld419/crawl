#include "IniFileMgrEx.h"
#include "WriteIni.h"
#include "CSemaphore.h"
#include "hash_list.h"
#include <iostream>
#ifdef	DEF_LINUX
#include <sys/types.h>
#include <sys/wait.h>
#endif
#include <assert.h>
using namespace std;
int hashex (const char *key)
{
         int     hval;
        const char      *ptr;
        char    c;
        int     i;

        hval = 0;
        for (ptr = key, i = 1; c=*ptr++; i++)
                hval += c*i; /* ascii char times its 1-based index */

        return (hval%HASH_NUM);
}



CIniFileMgrEx::CIniFileMgrEx(long lShmKeyID,unsigned int uiSize)
{
	m_lShmKey=lShmKeyID;
	m_uiSize=uiSize;
	m_bAttached=false;
	m_poParamInfoEx=0;
}

void CIniFileMgrEx::DestoryShm(void)
{
	if(0!=m_poParamInfoEx)
	{
		m_poParamInfoEx->remove();
		delete m_poParamInfoEx;
		m_poParamInfoEx=0;
	}
}
void CIniFileMgrEx::ClearShmData(void)
{
	if(0!=m_poParamInfoEx){
		m_poParamInfoEx->reset();	
	}
}

void CIniFileMgrEx::Clear(void)
{
	if(0!= m_poParamInfoEx)
	{
		delete m_poParamInfoEx;
		m_poParamInfoEx=0;
		return ;
	}
}
void CIniFileMgrEx::freeAll(void)
{
	ClearShm();
}

CIniFileMgrEx::~CIniFileMgrEx(void)
{
	Clear();
}

bool CIniFileMgrEx::InitShm(void)
{
	if(m_lShmKey<=0)
		return false;
	//��鹲���ڴ���Ƿ����
	//���¹����ڴ��Ƿ����
	ATTACH_DATA(m_poParamInfoEx,SysParamInfoEx,m_lShmKey);
	if(m_poParamInfoEx==0)
		return false;
	unsigned int iCount = m_uiSize/sizeof(SysParamInfoEx) +1;
	if(m_poParamInfoEx->exist())
	{
#ifdef		_GTEST_DEBUG_
	cout<<m_poParamInfoEx->getSysID()<<endl;
#endif
		//���һ�¹����ڴ��С
		//assert(m_poParamInfoEx->getTotal()==iCount);
		if(m_poParamInfoEx->getTotal()!=iCount)
			return false;
		//TODO::�����С��һ���Ƿ���Ҫ���Ĵ�С
	}else{
		//û�оʹ�����
		ATTACH_DATA(m_poParamInfoEx,SysParamInfoEx,m_lShmKey);
		m_poParamInfoEx->create(iCount);	
	}
	//���⹲���ڴ��Ѿ�������
	assert(m_poParamInfoEx->exist());
#ifdef		_GTEST_DEBUG_
	cout<<m_poParamInfoEx->getSysID()<<endl;
#endif
	return true;
}

bool CIniFileMgrEx::LoadIniFromFile(const char *cpsFileName)
{
	//�����ļ�
	if(!InitShm())
		return false;
	
	//����ļ��Ƿ����
	if (-1 ==access( cpsFileName, R_OK| W_OK| F_OK ) )
	{
		Log::log(0,"�ļ���ʧ��,����ļ��Ƿ����� :%s",m_strFileName);
		//�ļ������ڻ����ļ����ɶ�д
		return false;
	}
	if(m_poParamInfoEx->getTotal()>0){
		//�����ڴ��Ѿ���������Ҫ��ɾ��������
		return false;
	}
	m_strFileName = (char *)cpsFileName;
	ifstream oInputFile;
	oInputFile.open(m_strFileName.c_str(),ios_base::in);
	if(!oInputFile.is_open())
	{
		Log::log(0,"�ļ���ʧ��,����ļ��Ƿ����� :%s",m_strFileName);
		return false;
	}

	if( LoadFromStream(oInputFile)<=0)
	{
		Log::log(0,"װ�������ļ�ʧ�� :%s",cpsFileName);
		oInputFile.close();
		m_iNum=0;
		return false;
	}
	oInputFile.close();
	return true;
}


unsigned int CIniFileMgrEx::LoadFromStream(istream& inputStream)
{
	if (!inputStream.good())
	{
		//��ȡ��ʧ��
		Log::log(0,"��ȡ������ʧ��,%d",errno);
		return 0;
	}

	string strLine;
	string strSection,strKey,strValue;
	//������һ�ε�section���Ͳ�����
	string strSectionPre,strParamNamePre,strParamValuePre;	
	int i=0;
	int iCpyLen=0;
	SysParamInfoExOp stParamInfoOp={0};
	SysParamInfoExOp stParamInfoOpTmp={0};
	stParamInfoOp.enOp=OP_NEW;
	do{
		getline(inputStream,strLine, '\n');
		//�ɵ�ͷβ�ո�
		strLine.erase(0,strLine.find_first_not_of(' '));
		strLine.erase(strLine.find_last_not_of(' ')+1);
		if(!strLine.empty())
		{
			if(!AnalysisLine(strLine,strSection,strKey,strValue))
			{
				//�޷�ʶ�����
				Log::log(0,"�޷�ʶ�����:%s",strLine.c_str());
			}else
			{
				if(!strKey.empty())
				{
					i++;
					strSection.erase(0,strSection.find_first_not_of(' '));
					strSection.erase(strSection.find_last_not_of(' ')+1);
					iCpyLen=MAX_SECTION_LEN>strSection.length()?strSection.length():MAX_SECTION_LEN-1;
					strncpy(stParamInfoOp.stParamInfoEx.sSectionName,strSection.c_str(),iCpyLen);
					stParamInfoOp.stParamInfoEx.sSectionName[iCpyLen]=0;

					strKey.erase(0,strKey.find_first_not_of(' '));
					strKey.erase(strKey.find_last_not_of(' ')+1);
					iCpyLen=MAX_KEY_VALUE_LEN>strKey.length()?strKey.length():MAX_KEY_VALUE_LEN-1;
					strncpy(stParamInfoOp.stParamInfoEx.sParamName,strKey.c_str(),iCpyLen);
					stParamInfoOp.stParamInfoEx.sParamName[iCpyLen]=0;

					strValue.erase(0,strValue.find_first_not_of(' '));
					strValue.erase(strValue.find_last_not_of(' ')+1);
					iCpyLen=MAX_KEY_VALUE_LEN>strValue.length()?strValue.length():MAX_KEY_VALUE_LEN-1;
					strncpy(stParamInfoOp.stParamInfoEx.sParamValue,strValue.c_str(),iCpyLen);
					stParamInfoOp.stParamInfoEx.sParamValue[iCpyLen]=0;
					//������ȡ������xxx.newvalue�Ǿ�Ҫ���������ǲ���xxx.effdate��
					//������Ǿ͵úϳ�һ��
					if((!strSectionPre.empty())&&(strSectionPre.compare(
						stParamInfoOp.stParamInfoEx.sSectionName)==0)){
							string str1 = stParamInfoOp.stParamInfoEx.sParamName;
							str1.erase(str1.find_last_of('.')+1);
							if(str1==strParamNamePre){
								//��ʾ��һ��value,��һ����xxx.newvalue
								stParamInfoOpTmp = stParamInfoOp;
							}else{
								string str2 = strParamNamePre;
								string str3 = strParamNamePre;
								str2.erase(str2.find_last_of('.')+1);
								str3.erase(0,str3.find_last_of('.'));
								if(str1==str2){
									//��ʾ��һ����xxx.newvalue,��һ����xxx.effdate
									strcpy(stParamInfoOpTmp.stParamInfoEx.sEffDate,
										stParamInfoOp.stParamInfoEx.sParamValue);
									ModifyKey(stParamInfoOpTmp);
								}else{
									//��ǰ���û�й�ϵֱ�ӿ��Բ���
									ModifyKey(stParamInfoOp);
								}
							}
					}else
						ModifyKey(stParamInfoOp);
					//�����ϴμ�¼
					strSectionPre = stParamInfoOp.stParamInfoEx.sSectionName;
					strParamNamePre=stParamInfoOp.stParamInfoEx.sParamName;
					strParamValuePre=stParamInfoOp.stParamInfoEx.sParamValue;
				}
			}
		}
	}while(!inputStream.eof());
	return i;
}

bool CIniFileMgrEx::SyncToFile(const char* strFileName)
{
	assert(false);
	return false;
}
bool CIniFileMgrEx::ModifyKey(SysParamInfoExOp& stParamInfoOp)
{
	if(!m_poParamInfoEx)
		return false;
	bool bFind = false;
	if(stParamInfoOp.enOp==OP_NEW){
		unsigned int i = m_poParamInfoEx->malloc();
		if(i==0){
			//�����ڴ治����
			Log::log(0,"�����ڴ��޷��ٷ���,��鿪�ٵĴ�С");
			return false;	
		}
		(*m_poParamInfoEx)[i]=stParamInfoOp.stParamInfoEx;
		if(stParamInfoOp.stParamInfoEx.sEffDate[0]!=0){
			for(int j=1;j<i;j++){
				if((strcmp((*m_poParamInfoEx)[j].sSectionName,
					stParamInfoOp.stParamInfoEx.sSectionName)==0)){
						string str1=(*m_poParamInfoEx)[j].sParamName;
						string str2=stParamInfoOp.stParamInfoEx.sParamName;
						str2.erase(str2.find_last_of('.'));
						if((str1==str2)&&(stParamInfoOp.stParamInfoEx.sEffDate[0]!=0)){
							//�����������δ��ֵ
							(*m_poParamInfoEx)[j].uiNext=j;
							bFind=true;
						}
				}
			}
			assert(bFind);
		}
	}else if(stParamInfoOp.enOp==OP_FIX){
		int i = m_poParamInfoEx->getCount();
		int k=0;
		for(int j=1;j<i;j++){
			if(strcmp((*m_poParamInfoEx)[j].sSectionName,
				stParamInfoOp.stParamInfoEx.sSectionName)==0){
					if(stParamInfoOp.stParamInfoEx.sExpDate[0]!=0){
						//����һ��δ��ֵ
						string str1=stParamInfoOp.stParamInfoEx.sParamName;
						str1.erase(str1.find_last_of('.'));
						if(str1.compare((*m_poParamInfoEx)[j].sParamName)==0){
							//�ֶ���ͬ	
							assert((*m_poParamInfoEx)[j].uiNext!=0);
							int k = (*m_poParamInfoEx)[j].uiNext=0;
							bFind = true;
							break;
						}
					}else{
						if(strcmp((*m_poParamInfoEx)[j].sParamName,
							stParamInfoOp.stParamInfoEx.sParamName)==0){
								bFind = true;
								k=j;
								break;
						}
					}
			}
		}
		assert(bFind);
		if(bFind){
			strcpy((*m_poParamInfoEx)[k].sParamValue,
				stParamInfoOp.stParamInfoEx.sParamValue);
			strcpy((*m_poParamInfoEx)[k].sEffDate,
				stParamInfoOp.stParamInfoEx.sEffDate);
		}
	}else if(stParamInfoOp.enOp==OP_DEL){
		assert(stParamInfoOp.stParamInfoEx.sEffDate[0]==0);
		if(stParamInfoOp.stParamInfoEx.sEffDate[0]!=0){
			//����ɾ��һ��δ��ֵ
			return false;
		}
		int i = m_poParamInfoEx->getCount();
		for(int j=1; j<i;j++){
			if(bFind){
				if(j<(i-1)){
					(*m_poParamInfoEx)[j]=(*m_poParamInfoEx)[j+1];
					(*m_poParamInfoEx)[j].uiNext?((*m_poParamInfoEx)[j].uiNext-1):0;
					continue;	
				}
			}
			if((strcmp((*m_poParamInfoEx)[j].sSectionName,
				stParamInfoOp.stParamInfoEx.sSectionName)==0)&&
				(strcmp((*m_poParamInfoEx)[j].sParamName,
				stParamInfoOp.stParamInfoEx.sParamName)==0)){
					assert((*m_poParamInfoEx)[j].uiNext==0);
					bFind=true;
			}
		}
	}
	return true;
}

bool CIniFileMgrEx::SaveKey(const char* cpsFileName,SysParamInfoExOp& oIniKey)
{
	if(0== cpsFileName)
		return false;

	CSemaphore m_Lock;
	string strFileName=cpsFileName;


	//����ļ��Ƿ��д
	if (-1 ==access(strFileName.c_str(), R_OK| W_OK| F_OK ) )
	{
		Log::log(0,"�ļ������ڻ����ļ����ɶ�д, �����ļ� :%s",
			strFileName);
		return false;
	}
	//��Ҫ�����Ƿ���ס�ļ�
	WriteIni oWriteIni;
	if(oIniKey.enOp== OP_DEL)
	{
		oWriteIni.DelIniByIdent(strFileName.c_str() ,oIniKey.stParamInfoEx.sSectionName,
			oIniKey.stParamInfoEx.sParamName);
	}else
	{
		//�����sEffDate�ֶα�ʾ������Чʱ���	
		if(strlen(oIniKey.stParamInfoEx.sEffDate)>0)
		{
			string strParamTmp;
			strParamTmp = oIniKey.stParamInfoEx.sParamName;
			strParamTmp.append(".");
			strParamTmp.append(NEW_VALUE);
			oWriteIni.WriteIniString(strFileName.c_str(),
				oIniKey.stParamInfoEx.sSectionName,strParamTmp.c_str(),
				oIniKey.stParamInfoEx.sParamValue);
			strParamTmp = oIniKey.stParamInfoEx.sParamName;
			strParamTmp.append(".");
			strParamTmp.append(EFF_DATE_NAME);
			oWriteIni.WriteIniString(strFileName.c_str(),oIniKey.stParamInfoEx.sSectionName,
				strParamTmp.c_str(),oIniKey.stParamInfoEx.sEffDate);
		}else
			oWriteIni.WriteIniString(strFileName.c_str(),oIniKey.stParamInfoEx.sSectionName,
			oIniKey.stParamInfoEx.sParamName,oIniKey.stParamInfoEx.sParamValue);
	}
	return true;
}
bool CIniFileMgrEx::GetIniKey(vector<SysParamInfoEx>& arraySysParamInfoEx,
		const char* sSectionName,const char* sParamName,bool bCurrent,
		const char* sDate)
{
	if(0 ==m_poParamInfoEx)
		return false;
	int k =m_poParamInfoEx->getCount();
	for(int i=1;i<k;i++){
		if(sSectionName!=0){
			if(strcmp((*m_poParamInfoEx)[i].sSectionName,sSectionName)==0){
				if(sParamName!=0){
					if(strcmp((*m_poParamInfoEx)[i].sParamName,sParamName)==0){
						if(bCurrent){
							if(((*m_poParamInfoEx)[i].sEffDate[0]!=0)){
								arraySysParamInfoEx.push_back((*m_poParamInfoEx)[i]);
							}else
								continue;
						}else{
							if(sDate==0)
								arraySysParamInfoEx.push_back((*m_poParamInfoEx)[i]);	
							else{
								if(((*m_poParamInfoEx)[i].sEffDate[0]!=0)&&
									(strcmp((*m_poParamInfoEx)[i].sEffDate,sDate)>0)){
										arraySysParamInfoEx.push_back((*m_poParamInfoEx)[i]);	
								}
							}
						}
					}
				}else{
					if(bCurrent){
						if(((*m_poParamInfoEx)[i].sEffDate[0]!=0)){
							arraySysParamInfoEx.push_back((*m_poParamInfoEx)[i]);
						}else
							continue;	
					}else{
						if(sDate==0)
								arraySysParamInfoEx.push_back((*m_poParamInfoEx)[i]);	
						else{
							if(((*m_poParamInfoEx)[i].sEffDate[0]!=0)&&
								(strcmp((*m_poParamInfoEx)[i].sEffDate,sDate)>0)){
									arraySysParamInfoEx.push_back((*m_poParamInfoEx)[i]);	
							}
						}
					}
				}
			}
		}else{
			if(bCurrent){
				if(((*m_poParamInfoEx)[i].sEffDate[0]!=0)){
					arraySysParamInfoEx.push_back((*m_poParamInfoEx)[i]);
				}else
					continue;
			}else{
				if(sDate==0)
					arraySysParamInfoEx.push_back((*m_poParamInfoEx)[i]);
				else{	
					if(((*m_poParamInfoEx)[i].sEffDate[0]!=0)&&
						(strcmp((*m_poParamInfoEx)[i].sEffDate,sDate)>0)){
							arraySysParamInfoEx.push_back((*m_poParamInfoEx)[i]);	
					}
				}
			}
		}
	}
	return true;
}

bool CIniFileMgrEx::GetAllIniKey(vector<SysParamInfoEx>& arraySysParamInfoEx,
		const char* sSectionName ,
		const char* sParamName )
{
	return GetIniKey(arraySysParamInfoEx,sSectionName,sParamName,false,0);
}

bool CIniFileMgrEx::GetCurrentIniKey(vector<SysParamInfoEx>& arraySysParamInfoEx,
		const char* sSectionName ,
		const char* sParamName )
{
	return GetIniKey(arraySysParamInfoEx,sSectionName,sParamName,true,0);
}
bool CIniFileMgrEx::GetNextIniKey(vector<SysParamInfoEx>& arraySysParamInfoEx,
								  const char* sSessionName,
								  const char* sParamName,
								  const char* sDate)
{
	return GetIniKey(arraySysParamInfoEx,sSessionName,sParamName,false,sDate);
}

bool CIniFileMgrEx::GetNextIniKeyValue(const char* sSectionName,
										  const char* sParamName,
										  string& strValue,
										  const char* sDate)
{
	vector<SysParamInfoEx> arraySysParamInfo;
	if(!GetIniKey(arraySysParamInfo,sSectionName,sParamName,false,sDate))
		return false;
	assert(arraySysParamInfo.size()==1);
	strValue = arraySysParamInfo[0].sParamValue;
	return false;
}
bool CIniFileMgrEx::GetCurrentIniKeyValue(const char *sSectionName,
		const char *sParamName,string& strValue)
{
	static vector<SysParamInfoEx> arraySysParamInfoEx;
	arraySysParamInfoEx.clear();
	if(!GetCurrentIniKey(arraySysParamInfoEx,sSectionName,sSectionName))
		return false;
	//ȡ��ǰ��ֵ�����ܳ�������
	assert(arraySysParamInfoEx.size()==1);

	if(arraySysParamInfoEx.empty())
		return false;
	strValue = arraySysParamInfoEx[0].sParamValue;
	return true;
}


bool CIniFileMgrEx::AnalysisLine(string& strLine,string& strSection,
		string& strKey,string& strValue)
{
	strLine.erase(0,strLine.find_first_not_of(' '));
	strLine.erase(strLine.find_last_not_of(' ')+1);
	if(strLine.empty())
		return true;

	strKey.clear();
	strValue.clear();
	int iState=0,iStatePrev=0;//-1,����״̬,0,��ʼ̬,1, [  2,] ,3#,4=,5����
	int iPosBegin=0,iPosEnd=0;
	for(unsigned int i=0;i<strLine.length();i++)
	{
		switch(strLine[i])
		{
		case ' ':
			break;
		case '[':
			{
				if(iState==5)
					break;
				iPosBegin=i;
				iState=1;
			}break;
		case ']':
			{
				if(iState==5)
					break;
				if(iState!= 1){
					iState=-1;
					return false;
				}
				else
					iState=2;
				iPosEnd =i;
			}break;
		case '#':
			{
				if(iState==1){
					iState=-1;
					return false;
				}
				else
					iState=3;
				iPosEnd=i;
			}break;
		case '=':
			{
				if(iState==1)
					break;
				else
				{
					if(iState==3)
						break;
					iState=4;
				}
				iPosEnd=i;
			}break;
		default:
			{
				if(iState==5)
					break;
				else
				{
					if(iState==2)
					{
						iState=-1;
						return false;
					}
					if((iState==1)||(iState==3))
						break;
					iPosBegin=i;
					iState=5;
				}
			}
			break;
		}
		if(3==iState)
			break;
		if(-1==iState)
		{
			Log::log(0,"�޷�������:%s",strLine.c_str());
			return false;
		}
		if(2==iState)
		{
			strSection.assign(strLine,iPosBegin+1,iPosEnd-iPosBegin-1);
		}else
		{
			if(4==iState)
			{
				strKey.assign(strLine,iPosBegin,iPosEnd-iPosBegin);
			}else
			{
				if((3==iState)&&(!strKey.empty()))
				{
					strValue.assign(strLine,iPosBegin,iPosEnd-iPosBegin);
				}
			}
		}
	}
	if(((3==iState)&&(!strKey.empty()))||((5==iState)&&(!strKey.empty())))
	{
		if(iPosEnd>iPosBegin)
			strValue.assign(strLine,iPosBegin,iPosEnd-iPosBegin);
		else
			strValue.assign(strLine,iPosBegin,strLine.length());
	}else
	{
		if(-1==iState)
			return false;
	}
	return true;
}




//bool CIniFileMgrEx::GetIniKeyByString(std::string &strLine, stIniCompKey &oIniCompKey)
//{
//	string strSection,strParam,strValue,strContent;
//	strContent = strLine;
//	string stra,strb;
//	vector<string> arrayStringValue;
//	vector<string> arrayStringParam;
//	vector<string> arrayParamPrefix;
//	vector<string> arrayParamSuffix;
//	vector<string> arraySection;
//	int i=0;
//	while(splitParamInfo(strContent,strSection,strParam,strValue))
//	{
//		i++;
//		arraySection.push_back(strSection);
//		arrayStringParam.push_back(strParam);
//		arrayStringValue.push_back(strValue);
//		int iPosBegin = strParam.find_last_of('.');
//		if(iPosBegin>0){
//			stra.clear();strb.clear();
//			stra.append(strParam,0,iPosBegin);
//			strb.append(strParam,iPosBegin+1,strParam.length());
//			arrayParamPrefix.push_back(stra);
//			arrayParamSuffix.push_back(strb);
//		}
//		strSection.clear();strParam.clear();strValue.clear();
//	}
//	if(i==0)
//		return false;
//	memset(&oIniCompKey,0,sizeof(oIniCompKey));	
//	vector<string>::iterator it = arraySection.begin();
//	strncpy(oIniCompKey.sSectionName,it->c_str(),
//		MAX_SECTION_LEN>it->length()?it->length():MAX_SECTION_LEN-1);
//	it = arrayStringParam.begin();
//	strncpy(oIniCompKey.sKeyName,it->c_str(),
//		MAX_KEY_NAME_LEN>it->length()?it->length():MAX_KEY_NAME_LEN-1);
//	it=arrayStringValue.begin();
//	strncpy(oIniCompKey.sKeyValue[0],it->c_str(),
//			MAX_KEY_VALUE_LEN>it->length()?it->length():MAX_KEY_VALUE_LEN-1);
//
//	if((arrayParamPrefix.size()!=arrayParamSuffix.size())
//		||(arrayParamPrefix.size()!=arrayStringValue.size())
//		||arrayParamPrefix.size()<2)
//	{
//		return true;
//	}
//	oIniCompKey.sKeyValue[0][0]=0;
//	i =0;
//	for(vector<string>::iterator it =arrayParamSuffix.begin();
//		it!= arrayParamSuffix.end();it++,i++)
//	{
//		strValue=arrayStringValue[i];	
//		if(strcasecmp(it->c_str(),NEW_VALUE)==0)
//			strncpy(oIniCompKey.sKeyValue[1],strValue.c_str(),
//						MAX_KEY_VALUE_LEN>strValue.length()?strValue.length():MAX_KEY_VALUE_LEN-1);
//		else if (strcasecmp(it->c_str(),OLD_VALUE)==0)
//			strncpy(oIniCompKey.sKeyValue[0],strValue.c_str(),
//						MAX_KEY_VALUE_LEN>strValue.length()?strValue.length():MAX_KEY_VALUE_LEN-1);
//		else if (strcasecmp(it->c_str(),EFF_DATE_NAME)==0)
//			strncpy(oIniCompKey.sEffDate,strValue.c_str(),
//						MAX_DATE_LEN>strValue.length()?strValue.length():MAX_DATE_LEN-1);
//	}
//
//	char* stringFind = strrchr(oIniCompKey.sKeyName,'.');
//                if(0==stringFind)
//                        return true;
//                int iPos = (int)(stringFind -oIniCompKey.sKeyName );	
//		oIniCompKey.sKeyName[iPos]=0;
//	return true;
//}
//
//bool CIniFileMgrEx::Compare(CIniFileMgr* pIniFileMgr,vector<stIniCompKey>& vtCompKey)
//{
//	int iFlags=0; //1,������2���޸�,3,ɾ��
//	if(0 != pIniFileMgr)
//	{
//		unsigned int i=0,j=0;
//		for( j=1;j<=pIniFileMgr->m_iNum;j++)
//		{
//			iFlags=1;
//			for( i=1; i<=m_iNum;i++)
//			{
//				if(0==strcmp(m_pIniKey[i].sSectionName,pIniFileMgr->m_pIniKey[j].sSectionName))
//				{
//					iFlags=1;
//					if(0==strcmp(m_pIniKey[i].sKeyName,pIniFileMgr->m_pIniKey[j].sKeyName))
//					{
//						iFlags=2;
//						if(0==strcmp(m_pIniKey[i].sKeyValue[0],pIniFileMgr->m_pIniKey[j].sKeyValue[0]))
//						{
//							if(0==strcmp(m_pIniKey[i].sKeyValue[1],pIniFileMgr->m_pIniKey[j].sKeyValue[1]))
//							{
//								if(0== strcmp(m_pIniKey[i].sEffDate,pIniFileMgr->m_pIniKey[j].sEffDate))
//								{
//									iFlags=0;	break;
//								}break;
//							}break;
//						}break;
//					}
//				}
//			}
//			if((iFlags==1)||(iFlags==2))	
//			{
//				stIniCompKey oIniCompKey;
//				strcpy(oIniCompKey.sSectionName ,pIniFileMgr->m_pIniKey[j].sSectionName);
//				strcpy(oIniCompKey.sKeyName,pIniFileMgr->m_pIniKey[j].sKeyName);
//				memcpy(oIniCompKey.sKeyValue,pIniFileMgr->m_pIniKey[j].sKeyValue,sizeof(oIniCompKey.sKeyValue));
//				strcpy(oIniCompKey.sEffDate,pIniFileMgr->m_pIniKey[j].sEffDate);
//				if(iFlags==1)
//				{
//					//����
//					oIniCompKey.flag = OP_NEW;
//				}
//				if(iFlags==2)
//				{
//					//�޸�
//					oIniCompKey.flag = OP_FIX;
//				}
//				vtCompKey.push_back(oIniCompKey);
//			}
//		}
//		for( i=1; i<=m_iNum;i++)
//		{
//			iFlags=3;
//			for( j=1;j<=pIniFileMgr->m_iNum;j++)
//			{
//				if((0==strcmp(m_pIniKey[i].sSectionName,pIniFileMgr->m_pIniKey[j].sSectionName))
//					&&(0==strcmp(m_pIniKey[i].sKeyName,pIniFileMgr->m_pIniKey[j].sKeyName)))
//				{
//					iFlags=0;
//					break;
//				}
//			}
//			if(iFlags==3)
//			{
//				stIniCompKey oIniCompKey;
//				strcpy(oIniCompKey.sSectionName ,m_pIniKey[i].sSectionName);
//				strcpy(oIniCompKey.sKeyName,m_pIniKey[i].sKeyName);
//				strcpy(oIniCompKey.sKeyValue[0],m_pIniKey[i].sKeyValue[0]);
//				oIniCompKey.flag = OP_DEL;
//				vtCompKey.push_back(oIniCompKey);
//			}
//		}
//		return true;
//	}else
//		return false;
//}
//
//
//unsigned int CIniFileMgrEx::LoadFromStream(istream& inputStream)
//{
//	if (!inputStream.good())
//	{
//		//��ȡ��ʧ��
//		Log::log(0,"��ȡ������ʧ��,%d",errno);
//		return 0;
//	}
//
//	string strLine;
//	string strSection,strKey,strValue;
//	int i=0;
//	int iCpyLen=0;
//	CIniSection oIniSection;
//	memset(&oIniSection,0,sizeof(CIniSection));
//	do{
//		getline(inputStream,strLine, '\n');
//		strLine.erase(0,strLine.find_first_not_of(' '));
//		strLine.erase(strLine.find_last_not_of(' ')+1);
//		if(!strLine.empty())
//		{
//			if(!AnalysisLine(strLine,strSection,strKey,strValue))
//			{
//				//�޷�ʶ�����
//				Log::log(0,"�޷�ʶ�����:%s",strLine.c_str());
//			}else
//			{
//				if(!strKey.empty())
//				{
//					i++;
//					strSection.erase(0,strSection.find_first_not_of(' '));
//					strSection.erase(strSection.find_last_not_of(' ')+1);
//					iCpyLen=MAX_SECTION_LEN>strSection.length()?strSection.length():MAX_SECTION_LEN-1;
//					strncpy(oIniSection.sSectionName,strSection.c_str(),iCpyLen);
//					oIniSection.sSectionName[iCpyLen]=0;
//
//					strKey.erase(0,strKey.find_first_not_of(' '));
//					strKey.erase(strKey.find_last_not_of(' ')+1);
//					iCpyLen=MAX_KEY_VALUE_LEN>strKey.length()?strKey.length():MAX_KEY_VALUE_LEN-1;
//					strncpy(oIniSection.sKeyName,strKey.c_str(),iCpyLen);
//					oIniSection.sKeyName[iCpyLen]=0;
//
//					strValue.erase(0,strValue.find_first_not_of(' '));
//					strValue.erase(strValue.find_last_not_of(' ')+1);
//					iCpyLen=MAX_KEY_VALUE_LEN>strValue.length()?strValue.length():MAX_KEY_VALUE_LEN-1;
//					strncpy(oIniSection.sKeyValue[0],strValue.c_str(),iCpyLen);
//					oIniSection.sKeyValue[0][iCpyLen]=0;
//
//					InsertSection(&oIniSection);
//				}
//			}
//		}
//	}while(!inputStream.eof());
//	return i;
//
//}
//
//
//bool CIniFileMgrEx::InsertSection(CIniSection *pIniSection)
//{
//	if(((0!= pIniSection)&&(0!=m_pIniKey))&&(m_iNum<m_iIniKeyNum))
//	{
//		int i=1;
//		for(; i<=m_iNum;i++)
//		{
//			if((0==strcasecmp(pIniSection->sSectionName,m_pIniKey[i].sSectionName))&&
//				(0==strcasecmp(pIniSection->sKeyName,m_pIniKey[i].sKeyName)))
//			{
//				Log::log(0,"��ͬ��Section ��Key�Ѿ�����,SectionName:%s,KeyName:%s",pIniSection->sSectionName,pIniSection->sKeyName);
//				return false;
//			}
//		}
//		if((i>2)&&(MergeSection(&m_pIniKey[m_iNum-1],&m_pIniKey[m_iNum],pIniSection)))
//		{
//			m_iNum--;
//			return true;
//		}
//		m_pIniKey[++m_iNum] = *pIniSection;
//		return true;
//	}else
//	{
//		Log::log(0,"����Section �ڵ�ʧ��");
//		return false;
//	}
//}
//bool CIniFileMgrEx::CopyToShm(const char*cpsKey,const char*sLockKey,long lShmSize)
//{
//	if((0==cpsKey)||(cpsKey[0]==0))
//	{
//		Log::log(0,"���Ĳ��������ڴ�û������");
//		return false;
//	}
//	m_strMemKey=cpsKey;
//	m_strMemLockKey = sLockKey;
//	long lKey =atol(cpsKey);
//	CSemaphore m_Lock;
//	if((0==sLockKey)||(sLockKey[0]==0))
//	{
//		Log::log(0,"���Ĳ�����û������");
//		return false;
//	}
//	else
//	{
//		if(!m_Lock.getSem(sLockKey))
//			return false;
//	}
//
//	if(0!= m_poIniSection)
//		delete m_poIniSection;
//	m_poIniSection = new SHMData<CIniSection>(lKey);	
//	if(!m_poIniSection->exist())
//	{
//		if(0>=lShmSize)
//			m_poIniSection->create(10240);
//		else
//		{
//			lShmSize = (1024*lShmSize)/sizeof(CIniSection);
//			m_poIniSection->create(lShmSize);
//		}
//	}
//
//	m_Lock.P();
//
//	CIniSection* pSection= (CIniSection *)(*m_poIniSection);
//	if(m_poIniSection->getTotal()<=m_iIniKeyNum)
//	{
//		Log::log(0,"�����ڴ�̫С,�޷���������");
//		m_Lock.V();
//		Log::log(0,"���Ĳ��������ڴ��С");
//		return false;
//	}
//
//	int i=1;//m_iNum;
//	unsigned int iOffset=0;
//	m_poIniSection->reset();
//	while(i<=m_iNum)
//	{
//		iOffset = m_poIniSection->malloc();
//		memcpy((void*)&pSection[iOffset],(void*)&m_pIniKey[i++],sizeof(CIniSection));
//	}
//	m_Lock.V();
//	return true;
//}
//bool CIniFileMgrEx::GetDataFromShm(const char* cpsKey)
//{
//	if((0!=m_pIniKey)&&(m_poIniSection==0))
//	{
//		delete[] m_pIniKey;
//		m_pIniKey=0;
//	}
//	if((0==cpsKey)||(cpsKey[0]==0))
//		return false;
//	if(!AttachIniShm(cpsKey))
//		return false;
//	m_pIniKey = (CIniSection*)(*m_poIniSection);
//	m_iNum = m_poIniSection->getCount();
//	return true;
//}
//bool CIniFileMgrEx::AttachIniShm(const char *cpsKey)
//{
//	if(0!= m_poIniSection)
//		delete m_poIniSection;
//	m_poIniSection = new SHMData<CIniSection>(atol(cpsKey));
//
//	if(!m_poIniSection)
//		return false;
//	else
//	{
//		//����Ƿ�attach�ɹ�
//		if(!m_poIniSection->exist())
//		{
//			delete m_poIniSection;
//			m_poIniSection=0;
//			return false;
//		}
//	}
//	return true;
//}
//
//bool CIniFileMgrEx::ReLoadIniFromFile(void)
//{
//	CIniFileMgrEx* pIniFileMgr = new CIniFileMgr();
//	pIniFileMgr->LoadIniFromFile(m_strFileName.c_str());
//	vector<stIniCompKey> vtKey;
//	if(Compare(this,vtKey))
//	{
//		for(vector<stIniCompKey> ::iterator it = vtKey.begin();
//			it!=vtKey.end();it++)
//		{
//			AddKey(*it);
//		}
//	}else
//	{
//		delete pIniFileMgr; pIniFileMgr=0;
//		return false;
//	}
//	delete pIniFileMgr;pIniFileMgr=0;
//	return true;
//}
//
//bool CIniFileMgrEx::LoadIniFromFile(const char *cpsFileName)
//{
//	//��ȫ��� 
//	if(0==cpsFileName)
//		return false;
//
//	if (-1 ==access( cpsFileName, R_OK| W_OK| F_OK ) ){
//		//MBC_File+1
//		Log::log(0,"�ļ������ڻ����ļ����ɶ�д, �����ļ� :%s",cpsFileName);
//		return false;
//	}
//	try
//	{
//		if((0!= m_pIniKey)&&(0==m_poIniSection))//û�йҵ������ڴ����ɾ��
//		{	delete[] m_pIniKey; m_iNum=0;}
//		m_pIniKey = new CIniSection[m_iIniKeyNum];
//	}catch(bad_alloc& e)
//	{
//		badalloc(e);
//	}
//	memset(m_pIniKey,0,sizeof(CIniSection)*m_iIniKeyNum);
//
//	m_strFileName = (char *)cpsFileName;
//	ifstream oInputFile;
//	oInputFile.open(cpsFileName);
//	if(!oInputFile.is_open())
//	{
//		Log::log(0,"�ļ���ʧ��,����ļ��Ƿ����� :%s",cpsFileName);
//		return false;
//	}
//
//	if( LoadFromStream(oInputFile)<=0)
//	{
//		Log::log(0,"װ�������ļ�ʧ�� :%s",cpsFileName);
//		oInputFile.close();
//		m_iNum=0;
//		return false;
//	}
//	oInputFile.close();
//	return true;
//
//}
//
