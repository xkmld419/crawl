#include "BaseRuleMgr.h"
#include "Exception.h"
#include "MBC.h"
#include "MBC_ABM.h"
#include  <stdio.h>
#include  <stdlib.h>
#include "MsgDefineServer.h"
#include "Log.h"
#include "Env.h"
//CIniFileMgr *BaseRuleMgr::gpCIniFileMgr = 0;
//BaseSysSectRule *BaseRuleMgr::gpBaseSysSectRule = 0;

#define EFFDATE			"effdate" 

#define LINK_FLAG			"link"
#define EFF_FLAG			"eff"
#define DESC_FLAG			"desc"
#define DEFAULT_FLAG		"default"
#define MIN_FLAG			"min"
#define MAX_FLAG			"max"
#define ENUM_FLAG			"enum"
#define SUGGEST_FLAG		"suggest"
#define KEYWORD_FLAG		"keyword"
#define VALUE_FLAG			"value"
#define CHANGE_FLAGE		"change"
#define NEED_FLAGE			"need"
#define ARBI_FLAGE			"arbi"
#define TIME24_START_FLAGE		"time24start"
#define TIME24_END_FLAGE		"time24end"
#define CHANGE_VAL_FLAGE    "changeval"
#define CHAR_OR_NUMBER_FLAGE    "char_or_num"
#define DISOBEY_FLAGE    "disobey"
#define GROUP    		"group"

BaseRuleMgr::BaseRuleMgr()
{
//	if(!gpCIniFileMgr)
		gpCIniFileMgr = new CIniFileMgr();
	m_bLoadRule = false;
	m_bMonDebug = false;
	//m_poParamName.clear();
	//m_poSectName.clear();
	m_poParamRule = 0;
	gpBaseSysSectRule = 0;
	m_poIndex = 0;
	m_poGroupIndex = 0;
	for(int i=0;i<MAX_GROUP_NUM;i++)
	{
		m_iCurGroupParam[i] = 0;
	}
	//memset(m_iCurGroupParam,0,sizeof(int)*MAX_GROUP_NUM);
	m_iCurGroup = 0;
	loadRule();
}

BaseRuleMgr::~BaseRuleMgr()
{
    if(gpCIniFileMgr)
	{
		delete gpCIniFileMgr;
    	gpCIniFileMgr = 0;
	}
	freeALLMalloc();
	if(m_poIndex)
	{
		delete m_poIndex;
		m_poIndex = 0;
	}
	if(m_poGroupIndex)
	{
		delete m_poGroupIndex;
		m_poGroupIndex = 0;
	}
	if(m_poGroupNoIndex)
	{
		delete m_poGroupNoIndex;
		m_poGroupNoIndex = 0;
	}
}
void BaseRuleMgr::freeALLMalloc()
{
	 BaseSysSectRule *pSectRule = 0;
	 BaseSysSectRule *pSectRuleTmp = 0;
	 pSectRule = gpBaseSysSectRule;
	 while(pSectRule)
	 {
	 	pSectRuleTmp = pSectRule->next;
		freeALLParamMalloc(pSectRule);
		free(pSectRule);
		pSectRule = pSectRuleTmp;
	 }
}

void BaseRuleMgr::freeALLParamMalloc(BaseSysSectRule *pSectRule)
{
	 if(pSectRule)
	 {
	 	BaseSysParamRule *pParamRule = 0;
	 	BaseSysParamRule *pParamRuleTmp = 0;
	 	pParamRule = pSectRule->pFirst;
	 	while(pParamRule)
	 	{
	 		pParamRuleTmp = pParamRule->pNext;
			free(pParamRule);
			pParamRule = pParamRuleTmp;
	 	}
	 }
}

//�����ú���
void BaseRuleMgr::show()
{
	BaseSysSectRule *pSect = gpBaseSysSectRule;
	while(pSect)
	{
		printf("%s\n",pSect->sSectName);
		BaseSysParamRule *pParam = pSect->pFirst;
		while(pParam)
		{
			printf("%s%s\n","������: ",pParam->sParamName);
			printf("%s%s\n","���ֵ: ",pParam->sMaxVal);
			printf("%s%s\n","��Сֵ: ",pParam->sMinVal);
			printf("%s%s\n","����ֵ: ",pParam->sSuggestVal);
			printf("%s%s\n","Ĭ��ֵ: ",pParam->sDefaultVal);
			printf("%s%s\n","����: ",pParam->sDesc);
			printf("%s%s\n","��ǰֵ: ",pParam->sVal);
			for(int i =0;i<pParam->iEnumPos;i++)
			{
				printf("%s%s\n","ö��ֵ: ",pParam->sEnumVal[i]);
			}
			pParam = pParam->pNext;
		}
		pSect = pSect->next;
	}
}

void BaseRuleMgr::setDebugState(bool _Debug)
{
	 m_bMonDebug = _Debug;
}


BaseVarInfo *BaseRuleMgr::mallocBaseVar(int iGroup)
{
	if(m_iCurGroupParam[iGroup]<MAX_GROUP_PARAM_NUM)
	{ //������ʹ����С���������ֵ
		BaseVarInfo *p = &m_oBaseVarInfo[iGroup][ m_iCurGroupParam[iGroup]];
		memset(p,0,sizeof(BaseVarInfo));
		p->next = NULL;
		return  &m_oBaseVarInfo[iGroup][ m_iCurGroupParam[iGroup]++ ];
	}
	return NULL;
}

//���ݶ���,��������ȡ��������(public����)
//���������,�����������ڷ��ض��µ�һ������;���������ڷ���NULL
BaseSysParamRule *BaseRuleMgr::getParamRule(char *sSectNameSrc,char *sParamNameSrc)
{
	if(sSectNameSrc[0] == '\0' || sParamNameSrc[0] == '\0')
		return NULL;
	char sSectName[MAX_SECTION_LEN+1] = {0};
	char sParamName[MAX_KEY_NAME_LEN+1] = {0};
	strncpy(sSectName,sSectNameSrc,sizeof(sSectName));
	strncpy(sParamName,sParamNameSrc,sizeof(sParamName));
	//if(!Chang2Upper(sSectName) || !Chang2Lower(sParamName))
	//	return NULL;
	BaseSysSectRule *pSect = gpBaseSysSectRule;
	while(pSect)
	{
		if(strcmp(pSect->sSectName,sSectName)==0)
		{
			BaseSysParamRule *pParam = pSect->pFirst;
			//if(sParamName[0] == '\0')
			while(pParam)
			{
				if(strcmp(pParam->sParamName,sParamName)==0)
				{
					return pParam;
				}
				pParam = pParam->pNext;
			}
			return NULL;
		} else {
			pSect = pSect->next;		
		}
	}
	return NULL;
}

// �����÷ָ���ƫ����
int BaseRuleMgr::anayParam(char *sParam,char sep)
{
	 string *p = new string(sParam);
	 int iPos = p->rfind(sep);
	 delete p;
	 p = 0;
	 return iPos;
}

// ��ȡ������(����sDscParamNameҪ�Լ���֤�㹻��¼Դ��sSrcName������)
int BaseRuleMgr::getParamName(char *sSrcName,char *sDscParamName,char sep)
{
	 if(sSrcName[0] == '\0'){
	 	sDscParamName[0] = '\0';
		return 0;
	 }
	 int resPos = anayParam(sSrcName,sep);
	 if(resPos>0) {
	 	strncpy(sDscParamName,sSrcName,resPos);
	 } else
	 { 
	 	sDscParamName[0] = '\0';
		return -1;
	 }
	 return resPos;
}

// ���ݶ���malloc���νṹ(ͬ����ֱ�ӷ����Ѿ�malloc���Ķ�ָ��)
BaseSysSectRule *BaseRuleMgr::MallocSectRule(char *sSectName)
{
	BaseSysSectRule *pSRule = gpBaseSysSectRule;
	BaseSysParamRule *pPRule = 0;
	if(NULL == pSRule)
	{
		pSRule = (BaseSysSectRule*)malloc(sizeof(struct BaseSysSectRule));
		while(NULL == pSRule)
		{
			sleep(3);
			pSRule = (BaseSysSectRule*)malloc(sizeof(struct BaseSysSectRule));
		}
		memset(pSRule,'\0',sizeof(struct BaseSysSectRule));				
		strncpy(pSRule->sSectName,sSectName,sizeof(pSRule->sSectName));
		pSRule->pFirst = NULL;
		pSRule->next = NULL;
		gpBaseSysSectRule = pSRule;
		return pSRule;
	} else {
		while(pSRule)
		{
			if(strcmp(pSRule->sSectName,sSectName) == 0)
			{
				return pSRule;
			} else if(NULL == pSRule->next)
			{
				pSRule->next = (BaseSysSectRule*)malloc(sizeof(struct BaseSysSectRule));	
				while(NULL == pSRule->next)
				{
					sleep(3);
					pSRule->next = (BaseSysSectRule*)malloc(sizeof(struct BaseSysSectRule));
				}			
				pSRule = pSRule->next;			
				memset(pSRule,'\0',sizeof(struct BaseSysSectRule));
				strncpy(pSRule->sSectName,sSectName,sizeof(pSRule->sSectName));
				pSRule->pFirst = NULL;
				pSRule->next = NULL;				
				return pSRule;
			} else {
				pSRule = pSRule->next;
			}
		} // end...while(pSRule)
	}

return pSRule;

}

// ���ݶνṹmalloc����������
BaseSysParamRule *BaseRuleMgr::MallocParamRule(BaseSysSectRule *pSectRule)
{
	BaseSysParamRule *pPRule = 0;
	if(pSectRule)
	{
		pPRule = pSectRule->pFirst;
		if(!pPRule){
			pPRule = (BaseSysParamRule*)malloc(sizeof(struct BaseSysParamRule));
			while(!pPRule)
			{
				sleep(3);
				pPRule = (BaseSysParamRule*)malloc(sizeof(struct BaseSysParamRule));
			}
			memset(pPRule,'\0',sizeof(struct BaseSysParamRule));
			pPRule->pNext = NULL;
			pPRule->iEnumPos = 0;		
			pPRule->iLinkNum = 0;
			pSectRule->pFirst = pPRule;
			return pPRule;
		}	
		while(pPRule)
		{			
			if(!pPRule->pNext){
				pPRule->pNext = (BaseSysParamRule*)malloc(sizeof(struct BaseSysParamRule));	
				while(!pPRule->pNext)
				{
					sleep(3);
					pPRule->pNext = (BaseSysParamRule*)malloc(sizeof(struct BaseSysParamRule));
				}						
				pPRule = pPRule->pNext;				
				memset(pPRule,'\0',sizeof(struct BaseSysParamRule));	
				pPRule->iEnumPos = 0;
				pPRule->iLinkNum = 0;
				pPRule->pNext = NULL;				
				return pPRule;
			}
			pPRule = pPRule->pNext;
		}
	} else {
		return NULL;
	}
return pPRule;
}


//�Ҳ����½�һ���ṹ�巵��,ֱ��ʹ������ṹ��Ϳ�����
BaseSysParamRule *BaseRuleMgr::SearchParamRule(char *sSectName,char *sParamName)
{
	if(sSectName[0] == '\0')
		return NULL;
	BaseSysSectRule *pSectRule = 0;
	BaseSysParamRule *pParamRule = 0;
	pSectRule = gpBaseSysSectRule;
	if(!pSectRule) //�޶�
	{
		gpBaseSysSectRule = MallocSectRule(sSectName);
		pParamRule = MallocParamRule(gpBaseSysSectRule);
		strncpy(pParamRule->sParamName,sParamName,sizeof(pParamRule->sParamName));
		return pParamRule;
	} else { //�ж�
		while(pSectRule)
		{
			if(strcmp(pSectRule->sSectName,sSectName) == 0) //ͬ��
			{
				pParamRule = pSectRule->pFirst;
				if(!pParamRule){
					pParamRule =  MallocParamRule(pSectRule);
					strncpy(pParamRule->sParamName,sParamName,sizeof(pParamRule->sParamName));
					return pParamRule;
				}
				while(pParamRule)
				{
					if(strcmp(pParamRule->sParamName,sParamName)==0)
							return pParamRule;					

					if(!pParamRule->pNext)
					{
						pParamRule->pNext = MallocParamRule(pSectRule);
						pParamRule = pParamRule->pNext;
						strncpy(pParamRule->sParamName,sParamName,sizeof(pParamRule->sParamName));
						return pParamRule;
					} else {
						pParamRule = pParamRule->pNext;
					}
				}
			} else if(!pSectRule->next)
			{
				pSectRule->next = MallocSectRule(sSectName);
				pParamRule = MallocParamRule(pSectRule->next);
				strncpy(pParamRule->sParamName,sParamName,sizeof(pParamRule->sParamName));
				return pParamRule;
			} else {
				pSectRule = pSectRule->next;
			}
		}
	}
return pParamRule;
}

// 
BaseVarInfo *BaseRuleMgr::MallocBaseVar(char *sGroupName)
{
		BaseVarInfo *pBaseVarInfoGroup = 0;
		int iOffset = 0;
		if(m_poGroupNoIndex->get(sGroupName,&iOffset))//m_poGroupNoIndex�����õ���ʱ����
		{
			return mallocBaseVar(iOffset);
		} else if(m_iCurGroup<MAX_GROUP_NUM)
		{
			pBaseVarInfoGroup =  mallocBaseVar(m_iCurGroup);
			m_poGroupNoIndex->add(sGroupName,m_iCurGroup++);
			return pBaseVarInfoGroup;
		} else {
			return 0;//�˳�GROUP�ļ���
		}	
}

//���ز�������
bool BaseRuleMgr::loadRule()
{
	char sHome[254];
        char * p;

        if ((p=getenv (ENV_HOME)) == NULL){
         	sprintf (sHome, "/home/SGW_HOME/etc");
		 }
        else {
         sprintf (sHome, "%s/etc", p);
		}
	string strFileName=sHome;
        strFileName+= "/SysParamRule";
	if(!gpCIniFileMgr->LoadIniFromFile(strFileName.c_str())){
		printf("%s","�����ļ����ڴ�ʧ��");
		return false;
	}
	BaseSysSectRule *pSectRule = 0;
	BaseSysParamRule *pParamRule = 0;
	for(int j = 1;j<gpCIniFileMgr->m_iUseNum+1;j++)
	{	
		if(gpCIniFileMgr->m_pIniKey[j].sSectionName[0] != '\0' && gpCIniFileMgr->m_pIniKey[j].sKeyName[0] != '\0')
		{
				char sNameTmp[512] = {0};
				//����һ�����������
				BaseVarInfo mBaseVarInfo;
				memset(&mBaseVarInfo,'\0',sizeof(BaseVarInfo));
				strcpy(mBaseVarInfo.sSectName,gpCIniFileMgr->m_pIniKey[j].sSectionName);
				//Chang2Upper(mBaseVarInfo.sSectName);//ֻ�ֱܷ��������				
				
				if(strcmp(mBaseVarInfo.sSectName,GROUP)==0) //���������ξ��������
				{
					
					strcpy(mBaseVarInfo.sParamName,gpCIniFileMgr->m_pIniKey[j].sKeyName);
					//Chang2Lower(mBaseVarInfo.sParamName);
					
					BaseVarInfo *pBaseVarInfoGroup = 0;
					
					if(!m_poGroupIndex)
						m_poGroupIndex = new HashList<BaseVarInfo*>(90000);
					if(!m_poGroupNoIndex)
						m_poGroupNoIndex = new HashList<int>(90000);
					if(!m_poIndex)
						m_poIndex = new HashList<BaseSysParamRule*>(90000);
					char *pValTmp = gpCIniFileMgr->m_pIniKey[j].sKeyValue[0];//�������ε�ֵ��ʵ���ǲ���ȫ����ʽ��,����
					char *pVal = new char[strlen(pValTmp)+2];
					if(!pVal)
						THROW(100);
					memset(pVal,'\0',sizeof(pVal));
					strcpy(pVal,pValTmp);
					char *pRec = gpCIniFileMgr->m_pIniKey[j].sKeyValue[0];//
					int iOffset = 0;
					int iTime = 0;
					vector <string> vec_tag;
					getTagStringN(pVal,vec_tag,',');
					iTime = 0;
					
					while(iTime<vec_tag.size())
					{											
						BaseVarInfo *pBaseVarInfoGroup = 0;
						int iOffset = 0;
						if(m_poGroupNoIndex->get(mBaseVarInfo.sParamName,&iOffset))//m_poGroupNoIndex�����õ���ʱ����
						{
							pBaseVarInfoGroup = mallocBaseVar(iOffset);
							if(!pBaseVarInfoGroup)
								break;
						} else if(m_iCurGroup<MAX_GROUP_NUM)
						{
							pBaseVarInfoGroup = mallocBaseVar(m_iCurGroup);
							if(!pBaseVarInfoGroup)
								break;
							m_poGroupNoIndex->add(mBaseVarInfo.sParamName,m_iCurGroup++);
						} else {
							break;//�˳�GROUP�ļ���
						}	
						
						memset(pBaseVarInfoGroup,'\0',sizeof(BaseVarInfo));
						char sKey[MAX_SECTION_LEN+MAX_KEY_NAME_LEN+1] = {0};
						strcpy(sKey,(char*)vec_tag[iTime].c_str());
						char sGroupParamName[MAX_KEY_NAME_LEN+1] = {0};
						string strGroup(sKey);
						trimN(strGroup);
						sKey[0] = 0;
						strcpy(sKey,(char*)strGroup.c_str());
						int iGroupPos = strGroup.find('.');//�ǰ��ձ�׼�Ķ�.���������ĸ�ʽ�ʹ���
						if(iGroupPos)
						{
							strncpy(pBaseVarInfoGroup->sSectName,sKey,iGroupPos);
							strncpy(pBaseVarInfoGroup->sParamName,&sKey[iGroupPos+1],sizeof(sKey));
							//Chang2Upper(pBaseVarInfoGroup->sSectName);
							//Chang2Lower(pBaseVarInfoGroup->sParamName);
							BaseVarInfo *pBaseVarInfoGroupGet = 0;
							if(!m_poGroupIndex->get(mBaseVarInfo.sParamName,&pBaseVarInfoGroupGet))//GROUP��Ϊ����
							{
								m_poGroupIndex->add(mBaseVarInfo.sParamName,pBaseVarInfoGroup);
							} else {
								pBaseVarInfoGroup->next = pBaseVarInfoGroupGet;
								m_poGroupIndex->add(mBaseVarInfo.sParamName,pBaseVarInfoGroup);//ò�ƻ�Ҫ�������ʶ							
							}
							#ifndef DEF_UNLIMIT_GROUP_
							//���ӵ�������
							BaseSysParamRule *pBR = 0;
							if(m_poIndex->get(sKey,&pBR))
							{
								strcpy(pBR->sGroupParamName,mBaseVarInfo.sParamName);
							} else {
								BaseSysSectRule *pSect = MallocSectRule(pBaseVarInfoGroup->sSectName);
								if(!pSect)
									continue;
								BaseSysParamRule *pParam = MallocParamRule(pSect);
								if(!pParam)
									continue;
								strcpy(pParam->sGroupParamName,mBaseVarInfo.sParamName);
								m_poIndex->add(sKey,pParam);
							}
							#else//����鹦��
							
							#endif
						}
						iTime++;
					}															
					if(pVal)
					{
						delete []pVal;
						pVal = 0;
					}
				} else { //ԭ���Ĵ���
					int resPos = 0 ;
					int iRes = getParamName(gpCIniFileMgr->m_pIniKey[j].sKeyName,sNameTmp,'.');	
					if(iRes<0){
						//m_bLoadRule = false;//THROW(MBC_PARAM_RULE+1);
						Log::log(0,"���ݹ������[%s]��ȡ����ռ�ʧ��!",gpCIniFileMgr->m_pIniKey[j].sKeyName);
						continue;//return false;
					}																						
					resPos = anayParam(gpCIniFileMgr->m_pIniKey[j].sKeyName,'.');
					strcpy(mBaseVarInfo.sParamName,sNameTmp);
					//Chang2Lower(mBaseVarInfo.sParamName);
					pParamRule = SearchParamRule(mBaseVarInfo.sSectName,mBaseVarInfo.sParamName);
					if(!pParamRule){
						//m_bLoadRule = false;//THROW(MBC_PARAM_RULE+1);
						Log::log(0,"�������[%s.%s]��ȡ����ռ�ʧ��!",mBaseVarInfo.sSectName,mBaseVarInfo.sParamName);
						continue;//return false;
					}
					int iResType = 0;
					
					iResType = getType(gpCIniFileMgr->m_pIniKey[j].sKeyName,gpCIniFileMgr->m_pIniKey[j].sKeyValue[0],pParamRule);
					if(!m_poIndex)
						m_poIndex = new HashList<BaseSysParamRule*>(90000);
					
					BaseSysParamRule *pParamRuleGet = 0;
					char sKey[MAX_SECTION_LEN+MAX_KEY_NAME_LEN+1] = {0};
					sprintf(sKey,"%s.%s",mBaseVarInfo.sSectName,mBaseVarInfo.sParamName);
					if(!m_poIndex->get(sKey,&pParamRuleGet))
					{
						m_poIndex->add(sKey,pParamRule);
					}
					if(iResType == 0)
					{
						//m_bLoadRule = false;
						Log::log(0,"�����������[%s]ʧ��!",sKey);
						continue;	//THROW(MBC_PARAM_RULE+1);
					}
				}
						
		} 
	}
	m_bLoadRule = true;
	return m_bLoadRule;
}

// ��ȡ������(����sDscParamNameҪ�Լ���֤�㹻��¼Դ��sSrcName������)
int BaseRuleMgr::getParam(char *sSrcName,char *sDscParamName,char sep)
{
	 if(sSrcName[0] == '\0')
		return 0;
	 sDscParamName[0] = '\0';
	 string str(sSrcName);
	 int resPos = str.find(sep);
	 if(resPos!=0) 
	 {
	 	string strTmp;
		if(resPos>0)
		strTmp.assign(str,0,resPos);
	 	else
			strTmp = str;
	 	trimN(strTmp);
	 	strcpy(sDscParamName,(char*)strTmp.c_str());
		if(resPos>0)
		return resPos;
		else
			return 0;
	 } else//������ַ��ڵ�һλ
	 {
	 	return 0;
	 }
}

int BaseRuleMgr::addGroupType(BaseVarInfo *pBaseVarInfo,char *sVal,BaseSysParamRule *pSPRule)
{
	/*if(sVal[0] == '\0')
		return 0;
	//
	string str(sVal);
	while(str.size()>0)
	{
		int iPos = str.find(',');
		string strT;
		strT.assign(str,0,iPos);
		str.assign(str,iPos+1,str.size());
		strcpy(pSPRule->sGroupParamName[pSPRule->iGroupParamNum++],(char*)str.c_str());
	}
	return pSPRule->iGroupParamNum;*/
	return 0;
}

// ʵ�ʲ��������ֶθ�ֵ����
// ����˵��:sStr-���������Լ�suggest�Ȳ����Ĳ�����;rPos-�ָ���������ƫ����;sVal-����ֵ;pSPRule-Ҫ��ֵ�Ĳ����ṹ
// ����ֵ�Ǳ�ʶ���Ǹ��ֶθ�ֵ��
int BaseRuleMgr::getType(char *sStr,char *sVal,BaseSysParamRule *pSPRule)
{
	if(!sStr || sStr[0] == '\0')
		return 99;
	string source = string(sStr);
	int pos = source.rfind('.');
	if(pos<=0){
		printf("%s%s%s\n","����[",sStr,"]�������ø�ʽ����,����!");
		return 99;
	}
	char sDsc[1024] = {0};
	strncpy(sDsc,&sStr[pos+1],strlen(sStr)-pos);
	if(sDsc[0] == '\0'){
		if(!m_bLoadRule)
			printf("%s%s%s\n","����[",sStr,"]�������ø�ʽ����,����!");
		return 99;
	}
	//Chang2Lower(sDsc);
	if(strcmp(sDsc,SUGGEST_FLAG)==0)//����ֵ
	{
		strncpy(pSPRule->sSuggestVal,sVal,sizeof(pSPRule->sSuggestVal));
		return 1;
	} /*else if(strcmp(sDsc,VALUE_FLAG)==0)//��׼ֵ
	{
		strncpy(pSPRule->sVal,sVal,sizeof(pSPRule->sVal));
		return 2;
	} */else if(strcmp(sDsc,DEFAULT_FLAG)==0)//Ĭ��ֵ
	{
		strncpy(pSPRule->sDefaultVal,sVal,sizeof(pSPRule->sDefaultVal));
		return 3;
	}else if(strcmp(sDsc,MIN_FLAG)==0)//��Сֵ
	{
		strncpy(pSPRule->sMinVal,sVal,sizeof(pSPRule->sMinVal));
		return 4;
	}else if(strcmp(sDsc,MAX_FLAG)==0)//���ֵ
	{
		strncpy(pSPRule->sMaxVal,sVal,sizeof(pSPRule->sMaxVal));
		return 5;
	}else if(strcmp(sDsc,ENUM_FLAG)==0)//ö��ֵ
	{
			vector<string> vec_tag;
			vec_tag.clear();
			getTagStringN(sVal,vec_tag,',');
			vector<string>::iterator itr = vec_tag.begin();
			for(;itr != vec_tag.end();itr++)
			{
				strncpy(pSPRule->sEnumVal[pSPRule->iEnumPos++],(*itr).c_str(),sizeof(pSPRule->sEnumVal[pSPRule->iEnumPos++]));
			}
			return 6;
	}else if(strcmp(sDsc,DESC_FLAG)==0)//����
	{
		strncpy(pSPRule->sDesc,sVal,sizeof(pSPRule->sDesc));
		return 7;
	}else if(strcmp(sDsc,EFF_FLAG)==0)//����
	{
		pSPRule->iEffDateFlag = atoi(sVal);
		return 8;
	}/*else if(strcmp(sDsc,KEYWORD_FLAG)==0)//����
	{
		pSPRule->iKeyWordFlag = atoi(sVal);
		return 9;
	}*/else if(strcmp(sDsc,CHANGE_FLAGE)==0)//����
	{
		pSPRule->iValueChange = atoi(sVal);
		return 9;//CHANGE_FLAGE
	}else if(strcmp(sDsc,CHANGE_VAL_FLAGE)==0)//����
	{
		strcpy(pSPRule->sValueChange,sVal);
		return 15;//CHANGE_FLAGE
	}else if(strcmp(sDsc,NEED_FLAGE)==0)//����
	{
		pSPRule->iParamNeed = atoi(sVal);
		return 10;//NEED_FLAGE
	}else if(strcmp(sDsc,ARBI_FLAGE)==0)
	{
		pSPRule->cArbi = 'Y';
		return 11;//ARBITIONE_FLAGE
	}else if(strcmp(sDsc,TIME24_START_FLAGE)==0)
	{
		strcpy(pSPRule->sTime24Start,sVal);
		return 12;
	}else if(strcmp(sDsc,TIME24_END_FLAGE)==0)
	{
		strcpy(pSPRule->sTime24End,sVal);
		return 13;
	}else if(strcmp(sDsc,LINK_FLAG)==0)//���Ӳ���
	{
		vector<string> vec_tag;
		vec_tag.clear();
		getTagStringN(sVal,vec_tag,',');
		vector<string>::iterator itr = vec_tag.begin();
		for(;itr != vec_tag.end();itr++)
		{
			strncpy(pSPRule->sLinkParamName[pSPRule->iLinkNum++],(*itr).c_str(),sizeof(pSPRule->sLinkParamName[pSPRule->iLinkNum]));
		}
		return 14;
	}else if(strcmp(sDsc,CHAR_OR_NUMBER_FLAGE)==0)//�Ƿ���NUM����
	{
		pSPRule->iCharOrNum = atoi(sVal);
		return 15;
	}else if(strcmp(sDsc,DISOBEY_FLAGE)==0)//�Ƿ���NUM����
	{
		strncpy(pSPRule->sDisobey,sVal,sizeof(pSPRule->sDisobey));
		return 16;
	}
	else {
		if(!m_bLoadRule)
			printf("%s%s%s\n","����[",sStr,"]�������ø�ʽ����,����!");
		return 99;//���ڷ���һ����Чֵ ����֧�ֲ�����׺�Ĳ���
	}
	return 0;
}

// srcStr�ĸ�ʽ: ��.����=ֵ,....
bool BaseRuleMgr::check(const char *srcVal,bool Show)
{
	 if(srcVal[0] == '\0')
	 	return false;
	 return check((char*)srcVal,Show);
}

// srcStr�ĸ�ʽ: ��.����=ֵ,....
bool BaseRuleMgr::check(char *srcVal,bool Show)
{
	 if(srcVal[0] == '\0')
	 	return true;//�޲��������
	 if(!m_bLoadRule)
	 {
	 	if(Show)
		{
			printf("�����������ʧ��,�˲����������,���������������!\n");
		}
		return true;//�������ʧ��,�����
	 }
	 bool res = false;
	 m_poParamRule = 0;
	 /*char sSect[MAX_KEY_VALUE_LEN] = {0};
	 char sParam[MAX_KEY_VALUE_LEN] = {0};
	 char sVal[MAX_KEY_VALUE_LEN] = {0};
	 
	 vector<string> vec_tag;
	 vec_tag.clear();
	 getTagStringN(srcVal,vec_tag,',');
	 if(vec_tag.size()>2 || vec_tag.size()<1)
	 {
	 	if(Show)
	 		printf("%s\n","�����������,���ֲ�����ʽ��֧��,����!");
	 	return false;
	 }*/
	 res = paramParse(srcVal,Show);
	 return res;
}
// ��Ч���ڵļ��,strWholeEffdate��ʽeffdate=2010....
bool BaseRuleMgr::checkEffDate(string strWholeEffdate)
{	 
	 int iEffDatePos = 0;
	 if(m_poParamRule)
	 {
	 	switch(m_poParamRule->iEffDateFlag)
		{
			case 3:
					iEffDatePos = strWholeEffdate.find('=');
	 				if(iEffDatePos)
	 				{
	 					string sPureEffdate(strWholeEffdate,0,iEffDatePos);
						string sPureEffdateVal(strWholeEffdate,iEffDatePos+1,strWholeEffdate.size());
						trimN(sPureEffdate);
						trimN(sPureEffdateVal);
						if(sPureEffdate.size()<7 || sPureEffdateVal.size()<4)//effdate,�Լ��������پ�ȷ����
							return false;
						char sEffFlag[32] = {0};
						strncpy(sEffFlag,sPureEffdate.c_str(),sizeof(sEffFlag));
						//Chang2Lower(sEffFlag);
						if(strcmp(sEffFlag,EFFDATE)!=0)
							return false;
						if(checkEffdate(m_poParamRule,(char*)sPureEffdateVal.c_str())==0)
							return true;
						else	
							return false;
	 				} else {
						if(!m_bMonDebug)
								printf("%s\n","��������ȷ��Ч����ֵ,����!");
						return false;
	 				}				
					break;
			case 1:
			case 2:
					if(strWholeEffdate.size()>0)
					{
						if(!m_bMonDebug)
							printf("%s\n","�˲����������Ч����ֵ��");
						return false;
					}
					break;
			default:
					break;
		}
	 }
	 return true;
}

bool BaseRuleMgr::paramParse(const char* str,bool Show)
{
	 if(str[0] == '\0')
	 	return false;

	 string strEffdate;
	 string sPureEff;
	 string sEffFlag;//��ʶeffdate
	 string sParam;
	 string strAllParam(str);
	 int iParamEffPos = strAllParam.find(",");
	 if(iParamEffPos == 0)
	    return false;
	 if(iParamEffPos>0)
	 {
	    strEffdate.assign(strAllParam,iParamEffPos+1,strAllParam.size());
	    sParam.assign(strAllParam,0,iParamEffPos);
	 } else {
	    sParam = strAllParam;
	 }
	 int iValPos = sParam.rfind('=');
	 int iParamPos = sParam.find('.');
	 if(iValPos<0 || iParamPos<0){
		if(Show) 
	 		printf("%s\n","������ʽ����ȷ,�������ò���ֵʱ��������ֵ֮���=���߰����пո�������ַ�.��ȷ������ʽ����: Section.param=value");
		return false;
	 }
	 string sPureSect(sParam,0,iParamPos);
	 string sPureParam(sParam,iParamPos+1,iValPos-iParamPos-1);
	 string sPureVal(sParam,iValPos+1,sParam.size());
	 trimN(sPureSect);
	 trimN(sPureParam);
	 trimN(sPureVal);
	 trimN(sPureEff);
	 //m_poSectName = sPureSect;
	 //m_poParamName = sPureParam;
	 //
	 m_sParamName[0] = '\0';
	 m_sSectName[0] = '\0';
	 strcpy(m_sParamName,(char*)sPureParam.c_str());
	 strcpy(m_sSectName,(char*)sPureSect.c_str());
	 //
	 // ���Ӷ���Ч���ڱ�ʶ���ж�
	 if(strEffdate.size()>0)
	 {
	 	int iEffPos = strEffdate.find("=");
	 	if(iEffPos>0)
	 	{
			sPureEff.assign(strEffdate,iEffPos+1,strEffdate.size());
			sEffFlag.assign(strEffdate,0,iEffPos);
			trimN(sEffFlag);
			trimN(sPureEff);
			char sEffDateFlag[32] = {0};
			strncpy(sEffDateFlag,(char*)sEffFlag.c_str(),sizeof(sEffDateFlag));
			//Chang2Lower(sEffDateFlag); 
			if( strcmp(sEffDateFlag,EFFDATE) != 0 )
			{
				if(Show) 
	 				printf("%s\n","������ʽ����ȷ,��Ч���ڱ�ʶ����ȷ");
				m_poParamRule = 0;//getParamRule((char*)sPureSect.c_str(),(char*)sParam.c_str());
				return false;
			}
	 	}
	 }
	 int iRes = check((char*)sPureSect.c_str(),(char*)sPureParam.c_str(),(char*)sPureVal.c_str(),(char*)sPureEff.c_str(),NULL,Show);
	 if(iRes == 0)
	 	return true;
	 else
	 {
	 	if(Show)
		{
			int iResEnd = ERR_RULE_PARAM+iRes;
			Log::log(0,"%s","������ƥ�����!");
		}
		return false;	
	 }
}

// ����˵��
void BaseRuleMgr::paramExplain(char*sSect,char *sParam,bool bAfterCheck)
{
	 if(m_bMonDebug)
	 	return;
	 if(!m_bLoadRule){
	 	printf("%s\n","�������ʧ��,�������������������!");
		return ;
	 }
	 if(sSect[0] == '\0' || sParam[0] == '\0')
	 {
	 	printf("%s\n","�������ݲ�ȫ������!");
		return;
	 }
	 BaseSysParamRule *pParamRule = 0;
	 if(!bAfterCheck)
	 	pParamRule = getParamRule(sSect,sParam);
	 else
	 	pParamRule = m_poParamRule;
	 if(pParamRule)
	 {
	 	if(pParamRule->sParamName[0] != '\0')
	 		printf("%s%s\n","������: ",pParamRule->sParamName);
		if(pParamRule->sMaxVal[0] != '\0')
			printf("%s%s\n","���ֵ: ",pParamRule->sMaxVal);
		if(pParamRule->sMinVal[0] != '\0')
			printf("%s%s\n","��Сֵ: ",pParamRule->sMinVal);
		if(pParamRule->sSuggestVal[0] != '\0')
			printf("%s%s\n","����ֵ: ",pParamRule->sSuggestVal);
		if(pParamRule->sDefaultVal[0] != '\0')
			printf("%s%s\n","Ĭ��ֵ: ",pParamRule->sDefaultVal);
		if(pParamRule->sDesc[0] != '\0')
			printf("%s%s\n","��������: ",pParamRule->sDesc);
		if(pParamRule->sEnumVal[0][0] != '\0')
			printf("%s","ö��ֵ: ");
		for(int i =0;i<pParamRule->iEnumPos;i++)
		{
			if(pParamRule->sEnumVal[i][0] != '\0')
				printf("%s",pParamRule->sEnumVal[i]);
			if(pParamRule->sEnumVal[i+1][0] == '\0')
				printf("\n");
			else 
				printf("%s",", ");
		}
		switch(pParamRule->iEffDateFlag)
		{
			case 1:
					printf("%s\n","�˲�����������Ч��ʽ");
					break;
			case 2:
					printf("%s\n","�˲�����������Ч��ʽ");
					break;
			case 3:
					printf("%s\n","�˲����Ǵ���Ч���ڷ�ʽ��Ч");
					break;
			default:
					printf("%s\n","������Ч");
					break;
		}
		switch(pParamRule->iValueChange)
		{
			case 1:
					printf("%s\n","�˲���ֻ������");
					break;
			case 2:
					printf("%s\n","�˲��������޸�");
					break;
			case 3:
					printf("%s\n","�˲�����Ҫƥ�����");
					if(pParamRule->sValueChange[0] != '\0')
						printf("%s%s\n","�˲���ƥ��Ĺ���:", pParamRule->sValueChange);
					break;
			case 4:
					printf("%s\n","�˲���ֻ�ܼ�С");
					break;
			case 5:
					printf("%s\n","�˲���ֻ����ip");
					break;
		}
		if(pParamRule->sDisobey[0] != '\0')
		{
			printf("%s%s\n","�˲�����ֵ��Ϊ������Ҫ��������ַ�:",pParamRule->sDisobey);
		}
		if(pParamRule->cArbi == 'Y')
		{
			printf("%s\n","�˲�����Ҫ�ٲ�");
		}
		if(pParamRule->sTime24Start[0] != '\0' && pParamRule->sTime24End[0] != '\0')
		{
			printf("%s%s%s%s\n","�˲������õ�ʱ����Сֵ: ",pParamRule->sTime24Start ,"���ֵ: ",pParamRule->sTime24End);
		}
		if(pParamRule->iLinkNum>0)
		{
			printf("%s","��������: ");
			for(int i =0;i<pParamRule->iLinkNum;i++)
			{
				if(pParamRule->sLinkParamName[i][0] != '\0')
					printf("%s",pParamRule->sLinkParamName[i]);
				if(pParamRule->sLinkParamName[i+1][0] == '\0')
					printf("\n");
				else 
					printf("%s",", ");
			}
		}
		if(pParamRule->iParamNeed>0)
		{
			printf("%s\n","�˲����������");
		}
		if(pParamRule->iCharOrNum==1)
		{
			printf("%s\n","�˲�����������ֵ");
		}
		if(pParamRule->sGroupParamName[0] != '\0')
		{
			if(!m_poGroupIndex){
				printf("%s%s\n","�˲������ڲ�����:",pParamRule->sGroupParamName);
				return ;//û����Ͳ�������ʾ��
			}
			BaseVarInfo *pVar = 0;
			int i = 1;
			if(m_poGroupIndex->get(pParamRule->sGroupParamName,&pVar))
			{
				printf("�˲������ڲ�����[%s]\n",pParamRule->sGroupParamName);
				while(pVar)
				{
					printf("������[%s]��[%d]��������[ %s.%s ]\n",pParamRule->sGroupParamName,i++,pVar->sSectName,pVar->sParamName);
					pVar = pVar->next;		
				}
			}
		}
	 } else {
	    if(!bAfterCheck)
	 		printf("%s\n","��Ӧ����δ���ù���,����!");
	 }
}

// ��ʾ����˵��(ֻ��ʾ)
void BaseRuleMgr::paramParseExplain(const char* str)
{
	 if(m_bMonDebug)
	 	return;
	 if(!m_bLoadRule){
	 	printf("%s\n","�������ʧ��,�������������������!");
		return ;
	 }
	 if(str[0] == '\0')
	 {
	 	printf("%s\n","�޲���������!");
		return;
	 }
	 string sParam = str;
	 int iValPos = sParam.rfind('=');
	 int iParamPos = sParam.find('.');
	 if(iParamPos<0)
	 {
	 	printf("%s\n","������δ��ȫ���ʽ����,�����޷��ṩ��Ӧ�����Ĺ�����Ϣ!");
		return;
	 }
	 string sPureSect;
	 string sPureParam;
	 string sPureVal;
	 sPureSect.clear();
	 sPureParam.clear();
	 sPureVal.clear();
	 if(iValPos<0)
	 {
	 	sPureSect.assign(sParam,0,iParamPos);
		sPureParam.assign(sParam,iParamPos+1,sParam.size());
	 } else {
	 	sPureSect.assign(sParam,0,iParamPos);
	 	sPureParam.assign(sParam,iParamPos+1,iValPos-iParamPos-1);
	 	sPureVal.assign(sParam,iValPos+1,sParam.size());	 		 	
	 }
	 trimN(sPureSect);
	 trimN(sPureParam);
	 trimN(sPureVal);
	 paramExplain((char*)sPureSect.c_str(),(char*)sPureParam.c_str());// ʵ�ʴ�����
}

// bAfterCheck=true��������check֮��Ĳ��������ѯ
void BaseRuleMgr::paramParseExplain(bool bAfterCheck)
{
	 //if(m_poSectName.size()>0 && m_poParamName.size())
	 if(m_sSectName[0] != '\0' && m_sParamName[0] != '\0')
	 {
	 	//paramExplain((char*)m_poSectName.c_str(),(char*)m_poParamName.c_str(),true);
		paramExplain(m_sSectName,m_sParamName,true);
	 } else {
	 	if(!m_bMonDebug)
			printf("%s\n","��������ʧ��,���������ʽ!");
	 }
}

/*********************************/
// 
void BaseRuleMgr::addParamRule(CIniSection *pCIniSection)
{
	BaseSysSectRule *pSectRule = 0;
	BaseSysSectRule *pLastSectRule = 0;
	BaseSysParamRule *pParamRule = 0;
	BaseSysParamRule *pLastParamRule = 0;	
	char sNameTmp[MAX_KEY_NAME_LEN+1] = {0};
	getParamName(pCIniSection->sKeyName,sNameTmp,'.');
	if(sNameTmp[0] == '\0')
	{
		return;
	}
	pParamRule = SearchParamRule(pCIniSection->sSectionName,sNameTmp);
	int resPos = anayParam(pCIniSection->sKeyName,'.');
	if(pParamRule->sParamName[0] == '\0')
		strncpy(pParamRule->sParamName,pCIniSection->sKeyName,sizeof(char)*resPos);
	int iValType = getType(pCIniSection->sKeyName,pCIniSection->sKeyValue[0],pParamRule);
	if(iValType == 0){
		THROW(34030000+1);
	}
}

bool BaseRuleMgr::checkCfgParam()
{
	 return true;
}

bool BaseRuleMgr::getCurDate(char *sDate,int iSize)
{
	 if(sizeof(sDate)<=iSize || iSize<=0)
	 	return false;
	 Date mDate;
	 sDate[0] = '\0';
	 strncpy(sDate,mDate.toString(),sizeof(char)*iSize);
	 return true;
}
/***************************************/

void BaseRuleMgr::showErrInfo(int iErrID)
{
	switch(iErrID)
	{
		case MAX_ERR:
			printf("%s\n","������ֵ�������������趨���ֵ,����!");
			break;
		case MIN_ERR:
			printf("%s\n","������ֵС�ڲ��������趨��Сֵ,����!");
			break;
		case ENUM_ERR:
			printf("%s\n","������ֵ���ڲ��������趨��ö��ֵ��Χ,����!");
			break;
		case EFF_ERR_LESS:
			printf("%s\n","��Ч����ֵ������,����δ������,����!");
			break;
		case EFF_ERR_NONEED:
			printf("%s\n","����������Ч����,����!");
			break;
		case MAX_MIN_SET_ERR:
			printf("%s\n","���������Сֵ���ô���,����!");
			break;
		case TIME_ERR:
			printf("%s\n","�������������õ�ʱ������,����!");
			break;
		case ISNOTNUM_ERR:
			printf("%s\n","����������������������(Ϊ��ֵ�Ҳ�Ϊ�ջ��ڹ�����ֵ��Χ֮��),����!");
			break;
		case DISOBEY_ERR:
			printf("%s\n","��������������,ĳЩ�ַ���������,����!");
			break;
		default:
			break;
	}
}

int BaseRuleMgr::checkNowValue(char *section, char *param, char *value, char *effdate)
{
	int res = 0;
	if( (section != NULL) && (param != NULL))
	{
		//û���ù��� ֱ������ȷ��
		BaseSysParamRule *rule = getParamRule(section,param);
		if(!rule)
			return 0;
		//�����Сֵ
		if(rule->sMaxVal[0] != '\0' || rule->sMinVal[0] != '\0')
		{
			if(!value || value[0] == '\0')
				res = MAX_MIN_SET_ERR;
			else
				res = checkMinMax(rule,value);
			if(res != 0){
				sprintf(sErrInfo,"����[%s.%s]ֵ���������Сֵ����Χ��",section,param);
				return res;
			}
		}
		//ö��ֵ
		if(rule->iEnumPos>0)
		{
			if(!value || value[0] == '\0')
				res = ENUM_ERR;
			else
				res = checkEnum(rule,value);
			if(res != 0){
				sprintf(sErrInfo,"����[%s.%s]ֵ����ö�ٹ���Χ��",section,param);
				return res;
			}
		}
		//��Чʱ��ֵ
		switch(rule->iEffDateFlag)
		{
			case 1:
			case 2:
			{
				if(effdate && effdate[0] != '\0'){
					sprintf(sErrInfo,"����[%s.%s]ֵ������Чʱ��",section,param);
					return EFF_ERR_NONEED;
				}
				break;
			}
			case 3:
			{
				if(!effdate || effdate[0] == '\0'){
					sprintf(sErrInfo,"����[%s.%s]ֵ��Ҫ��Чʱ��",section,param);
					return EFF_ERR_LESS;
				}
				break;
			}
		}
		//��������ƥ��
		if(rule->sValueChange[0] != '\0')
		{
			if(!value || value[0] == '\0')
				res = ENUM_ERR;
			else {
				res = MaskString(value,rule->sValueChange);
			}
			if(!res)
			{
				//ֵ���������õ�ƥ������
				sprintf(sErrInfo,"����[%s.%s]ֵ���������õ�ƥ������[%s]",section,param,rule->sValueChange);
				return CHANGE_ERR;
			} else {
				if(rule->iValueChange == 5)
				{
					vector<string> vec_tagChange;
					vec_tagChange.clear();
					getTagStringN(value,vec_tagChange,'.');
					vector<string>::iterator itr = vec_tagChange.begin();
					for(;itr != vec_tagChange.end();itr++)
					{
						char *p = (char*)((*itr).c_str());
						for(int i = 0;i<strlen(p);i++)
						{
							if(!isdigit(p[i]))
							{
								return CHANGE_ERR;
							}
						}
						if(atoi(p) >=0 && atoi(p)<256)
						{
						} else {
							return CHANGE_ERR;
						}
					}	
				}
			}
		}
		//�����������
		if(rule->iParamNeed>0)
		{
			if(!value || value[0] == '\0')
			{
				sprintf(sErrInfo,"����[%s.%s]ֵ�������",section,param);
				return PARAM_INFO_NEED;
			}
		}
		if(rule->sDisobey[0] != '\0')
		{
			if(!value || value[0] == '\0')
				res = DISOBEY_ERR;
			else
				res = checkDisobey(rule,value);
			if(res != 0){
				sprintf(sErrInfo,"����[%s.%s]ֵ�����˲������õ��ַ�",section,param);
				return res;
			}
		}
	}
	return 0;
}

int BaseRuleMgr::check(char *sSect,char *sParam,char *sValue,char *sEffVal,char *OldVal,bool ShowErr,int iFlag)
{
	int iRes = check(sSect,sParam,sValue,sEffVal,OldVal,iFlag);
	if(iRes != 0){
		if(ShowErr){
			showErrInfo(iRes);
		}
	}
	return iRes;	
}

/*
����:�������Լ�����ֵ�Ƿ���Ϲ淶
����:sSect:����;sParam������;sValue����ֵ
����ֵ:���ͨ������Ҫ���Ϊtrue������false
����: sSect��SYSTEM  sParam��location  sValue��beijing 
����  sSect��SYSTEM  sParam��location  sValue��beijing,sEffValΪ20101010//�����Ǵ���Ч���ڵ�
*/
int BaseRuleMgr::check(char *sSect,char *sParam,char *sValue,char *sEffVal,char *OldVal,int iFlag)
{
	 BaseSysParamRule* pParamRule = getParamRule(sSect,sParam);
	 m_poParamRule = pParamRule;
	 int iRes = 0;
	 if(pParamRule)
	 {
		if(pParamRule->iValueChange>0)
		{
			iRes = checkValueChange(pParamRule,sValue,OldVal);
			if(iRes!=0)
				return iRes;	
		}	
		if(pParamRule->iCharOrNum==1)
		{
			iRes = CheckDigitString(pParamRule,sValue);
			if(iRes!=0)
				return iRes;	
		}	
		//�����Сֵ����ö��ֵƥ��
	 	if(pParamRule->sMaxVal[0]!='\0' || pParamRule->sMinVal[0]!='\0')
		{
			 iRes = checkMinMax(pParamRule,sValue);
			 if(iRes!=0)
			 	return iRes;
		} else if(pParamRule->sEnumVal[0][0]!='\0')
		{
			iRes = checkEnum(pParamRule,sValue);
			if(iRes!=0)
			 	return iRes;
		}
		
		if(pParamRule->iEffDateFlag>0)
		{
			iRes = checkEffdate(pParamRule,sEffVal,iFlag);
			if(iRes!=0)
				return iRes;
		} else {
			 if(sEffVal[0] != '\0')
				return EFF_ERR_NONEED;
		}
		
		//����ȡ��ֵ
		if(pParamRule->sDisobey[0] != '\0')
		{
			if(!sValue || sValue[0] == '\0')
				iRes = DISOBEY_ERR;
			else
				iRes = checkDisobey(pParamRule,sValue);
			if(iRes != 0)
				return iRes;
		}
	
		iRes = checkTime(pParamRule,sValue);
		if(iRes)
			return iRes;
		else
			return 0;//�����˹���,û�ҵ���Ӧ��,�����ƥ��ɹ�
	
	 } else { //return 0[û���ù���],ֱ�ӳɹ�,�����˹���İ��չ����ж�
	 	return 0;	 
	 }
}

int BaseRuleMgr::checkDisobey(char *sSect,char *sParam,char *sVal)
{
	 BaseSysParamRule* pParamRule = getParamRule(sSect,sParam);
	 if(pParamRule)
	 {
	 	return checkDisobey(pParamRule,sVal);
	 }
	 return 0;
}


int BaseRuleMgr::checkDisobey(BaseSysParamRule *pParamRule,char *sVal)
{
	 if(pParamRule)
	 {
	 	if(pParamRule->sDisobey[0] != '\0')
		{
			if(!sVal || sVal[0]=='\0')
				return DISOBEY_ERR;
			for(int i=0;i<strlen(sVal);i++)
			{
				for(int j=0;j<strlen(pParamRule->sDisobey);j++)
				{
					if(sVal[i] == pParamRule->sDisobey[j])
					{
						return DISOBEY_ERR;
					}
				}
			}
		}
	 }
	 return 0;
}

int BaseRuleMgr::checkEffdate(char *sSect,char *sParam,char *sEffDate,int iFlag)
{
	 BaseSysParamRule* pParamRule = getParamRule(sSect,sParam);
	 if(pParamRule)
	 {
	 	return checkEffdate(pParamRule,sEffDate,iFlag);
	 }
	 return 0;
}

int BaseRuleMgr::checkEffdate(BaseSysParamRule *pParamRule,char *sEffDateSrc,int iFlag)
{
	 if(iFlag == 1)//�������Ч���ڷ�ʽ
	 	return 0;
	 int iMaxSize = strlen(sEffDateSrc);
	 char sEffDate[14] = {0};
	 strncpy(sEffDate,sEffDateSrc,sizeof(sEffDate));
	 for(int i=0;i<(14-iMaxSize);i++)
	 {
	 	strcat(sEffDate,"0");
	 }
	 Date srcDate(sEffDate);
	 if(pParamRule && pParamRule->iEffDateFlag)
	 {
			Date mDate;		
	 		switch(pParamRule->iEffDateFlag)
			{
				case 3:	//����Ч����				
						if(srcDate>mDate)
							return 0;
						else
							return EFF_ERR_LESS;
					    break;
				case 1: //����������Ч
				case 2:
						if(sEffDateSrc && sEffDateSrc[0] != '\0')
						{
							return EFF_ERR_NONEED;
						}
						break;
				default:
						break;
				}
	 }
	 return 0;
}

/*
����: �������Ƿ���������Сֵ�趨
����˵��:sSect����,sParam������
����ֵ:true�ǲ����ض�Ҫ���ڵ�,false�ǲ�һ��Ҫ����
����:sSectΪSYSTEM sParamΪlocation
���÷�������: �ڹ����ļ��������µ�����
[SYSTEM]
location.need = 1	# ������ڵĲ���
*/
int BaseRuleMgr::checkParamNeed(char *sSect,char *sParam)
{
	BaseSysParamRule* pParamRule = getParamRule(sSect,sParam);
	return checkParamNeed(pParamRule);
}

int BaseRuleMgr::checkParamNeed(BaseSysParamRule* pParamRule)
{
	if(!pParamRule)
		return PARAM_INFO_NONEED;
	if(pParamRule->iParamNeed)
	{
		   return 0;
	}
	return PARAM_INFO_NONEED;
}

/***************************************/
int BaseRuleMgr::checkMinMax(BaseSysParamRule* pParamRule,char *sPtr)
{
	if(pParamRule->sMaxVal[0]!='\0' || pParamRule->sMinVal[0]!='\0')
	{
			if(sPtr==NULL || sPtr[0] == '\0')
				return ISNOTNUM_ERR;
			if(!isRuleNumeric(sPtr))
			{
				return ISNOTNUM_ERR;
			}
			long lMax = 0;
			long lMin = 0;
			long lVal = 0;
			/*if(sPtr && sPtr[0]!='\0')
			{
				int i=0;
				if(sPtr[0] == '-')
				{
					i++;
				}
				while((sPtr[i]))
				{
					if(!isdigit(sPtr[i]))
						break;
					i++;
				}
				if(strlen(sPtr)>i)
					return MAX_ERR;
			}*/
			lVal = atol(sPtr);
			if(pParamRule->sMaxVal[0] == '\0' && pParamRule->sMinVal[0]!='\0')
			{
				lMin = atol(pParamRule->sMinVal);
				if(lVal<lMin){
					return MIN_ERR;
				}
				else 
					return 0;
			}
			else if(pParamRule->sMaxVal[0] != '\0' && pParamRule->sMinVal[0] == '\0')
			{
				lMax = atol(pParamRule->sMaxVal);
				if(lVal>lMax){
					return MAX_ERR;
				}
				else
					return 0;
			} else {//�����Сֵ�����õ�
				lMax = atol(pParamRule->sMaxVal);
				lMin = atol(pParamRule->sMinVal);
				if(lMax>lMin)
				{
					if(lVal<=lMax && lVal>=lMin){
						return 0;
					} else {
						if(lVal>lMax)
							return MAX_ERR;
						else
							return MIN_ERR;
					}
				} else {
					return MAX_MIN_SET_ERR;
				}
			}
	} 
	return 0;
	
}

/*
����: �������Ƿ���ϲ���ֵ�޸��趨
����˵��:sSect����,sParam������,sValue��ֵ
����ֵ:true�ǲ��������޸�
����:sSectΪSYSTEM sParamΪnum
���÷�������: �ڹ����ļ��������µ�����
[SYSTEM]
num.change = 2	# 1-ֻ������ //.change; 2-�����޸� ; 3-����ƥ��	;//4-ֻ�ܼ�С
*/
int BaseRuleMgr::checkValueChange(char *sSect,char *sParam,char *sValue,char *sOldVal)
{
	 BaseSysParamRule* pParamRule = getParamRule(sSect,sParam);
	 return checkValueChange(pParamRule,sValue,sOldVal);
}

int BaseRuleMgr::checkValueChange(BaseSysParamRule* pParamRule,char *sPtr,char *sOldVal)
{
 	if(pParamRule->iValueChange)
	{
		switch(pParamRule->iValueChange)
		{
				case 1:	
						if(!sPtr || sPtr[0] == '\0')
							return IN_CREASE_ERR;
						if(!sOldVal || sOldVal[0] == '\0'){
							return 0;
						}else{
							if(atol(sOldVal)>=atol(sPtr))
								return IN_CREASE_ERR;
							else 
								return 0;
						}
						break;
				case 4:				
						if(!sPtr || sPtr[0] == '\0')
							return DE_CREASE_ERR;	
						if(!sOldVal || sOldVal[0] == '\0'){
							return 0;
						}else{
							if(atol(sOldVal)<=atol(sPtr))
								return DE_CREASE_ERR;
							else
								return 0;
						}
						break;
				case 2:
						if(sPtr && sPtr[0] != '\0')
							return CHANGE_ERR;
				case 3:
				case 5:
						if(pParamRule->sValueChange[0] == '\0'){
							//δ���õ�ƥ������
							return 0;
						}
						if(!sPtr || sPtr[0] == '\0')
							return IN_CREASE_ERR;
						if(!MaskString(sPtr,pParamRule->sValueChange))
						{
							//����ֵ���������õ�ƥ������
							return CHANGE_ERR;
						}
						if(pParamRule->iValueChange == 5)
						{
							vector<string> vec_tagChange;
							vec_tagChange.clear();
							getTagStringN(sPtr,vec_tagChange,'.');
							vector<string>::iterator itr = vec_tagChange.begin();
							for(;itr != vec_tagChange.end();itr++)
							{
								char *p = (char*)((*itr).c_str());
								for(int i = 0;i<strlen(p);i++)
								{
									if(!isdigit(p[i]))
									{
										return CHANGE_ERR;
									}
								}
								if(atoi(p) >=0 && atoi(p)<256)
								{
								} else {
									return CHANGE_ERR;
								}
							}	
						}
						break;
				default:
						break;
		 }
	}
	return 0;
}

int BaseRuleMgr::checkEnum(char *sSect,char *sParam,char *sValue)
{
	 BaseSysParamRule* pParamRule = getParamRule(sSect,sParam);
	 return checkEnum(pParamRule,sValue);
}

/*
����: �������Ƿ����ö��ֵ�趨
����˵��:sPtr�Ǿ���ֵ
����ֵ:true�ǲ�������ö��Ҫ��
����:sPtrΪ
���÷�������: �ڹ����ļ��������µ�����
[SYSTEM]
location.need = 1	# ������ڵĲ���
*/
int BaseRuleMgr::checkEnum(BaseSysParamRule* pParamRule,char *sPtr)
{
	if(pParamRule->sEnumVal[0][0]!='\0')
	{
		for(int i=0;i<pParamRule->iEnumPos;i++)
		{
			if(strcmp(pParamRule->sEnumVal[i],sPtr)==0)
				return 0;
		}
		//if(!m_bMonDebug)
		//	printf("%s\n","����ֵ���ڹ�������Χ!");
		return ENUM_ERR;
	}
	return 0;
}

int BaseRuleMgr::checkTime(char *sSect,char *sParam,char *sValue)
{
	 BaseSysParamRule* pParamRule = getParamRule(sSect,sParam);
	 return checkTime(pParamRule,sValue);
}


int BaseRuleMgr::checkTime(BaseSysParamRule* pParamRule,char *sPtr)
{
	if(pParamRule->sTime24Start[0]!='\0' && pParamRule->sTime24End[0]!='\0')
	{
			if(!sPtr || sPtr[0] == '\0')
				return TIME_ERR;
			if(strlen(pParamRule->sTime24Start) != strlen(pParamRule->sTime24End))
				return 0;//�������,���Ƚ�
			if(strlen(pParamRule->sTime24Start) != strlen(sPtr))
				return TIME_ERR;
			if(strcmp(pParamRule->sTime24Start,sPtr)<=0 && strcmp(pParamRule->sTime24End,sPtr)>=0)
			{
				return 0;
			} else {
				return TIME_ERR;
			}	
	}
	return 0;
}

//  �������Ƿ�����ֻ�����ӻ���ֻ�ܼ���
int BaseRuleMgr::checkInOrDecrease(const char* str)
{
	 if(!m_bLoadRule){
		return RULE_UNLOAD_ERR;
	 }
	 if(str[0] == '\0')
	 {
		return PARAM_FORMAT_ERR;
	 }
	 string sPureSect;
	 string sPureParam;
	 string sPureVal;
	 BaseSysParamRule* pParamRule = 0;
	 if(getEveryInfo(str,sPureSect,sPureParam,sPureVal))
	 {
	 	pParamRule = getParamRule((char*)sPureSect.c_str(),(char*)sPureParam.c_str());
	 } else {
	 	return -1;
	 }
	 if(pParamRule)
	 {
		return pParamRule->iValueChange;
	 }
	 return 0;
}

bool BaseRuleMgr::IfNeedLinkParam(char *sSect,char *sParam)
{
     if(sSect[0] != '\0'  && sParam[0] != '\0')
     {
        BaseSysParamRule* pParamRule = 0;
        pParamRule = getParamRule(sSect,sParam);
        if(pParamRule)
        {
            if(pParamRule->iLinkNum>0)
                return true;
            else
                return false;
        }
     }
     return false;
}

//����GROUP
bool BaseRuleMgr::checkGroupParam(vector<string> str, bool bShow)
{
	bool res = false;
	//���׸���������Ϣ,Ȼ������׸�����
	BaseSysParamRule *pRule = 0;
	BaseVarInfo mBaseVarInfo;
	
	//����ת������ ת��BaseVarInfo
	if(str.size()<=0)
		return false;
	if(!m_poIndex || !m_poGroupNoIndex ||!m_poGroupIndex)
		return true;//û����Ͳ��ж���
	//
	for(int i=0;i<str.size();i++)
	{
		memset(&mBaseVarInfo,'\0',sizeof(BaseVarInfo));
		string strT = str[i];
		trimN(strT);
		int iPos = strT.find('.');
		string sPureSect;
		string sPureParam;
		if(iPos)//������ʽ��ȷ
		{
			sPureSect.assign(strT,0,iPos);
			strcpy(mBaseVarInfo.sSectName,(char*)sPureSect.c_str());
			//Chang2Upper(mBaseVarInfo.sSectName);
			sPureParam.assign(strT,iPos+1,strT.size());
			strcpy(mBaseVarInfo.sParamName,(char*)sPureParam.c_str());
			//Chang2Lower(mBaseVarInfo.sParamName);
			BaseSysParamRule *pTmp = 0;
			char sFullFName[MAX_SECTION_LEN+MAX_KEY_NAME_LEN+1] = {0};
			sprintf(sFullFName,"%s.%s",mBaseVarInfo.sSectName,mBaseVarInfo.sParamName);
			if(m_poIndex->get(sFullFName,&pTmp))
			{
				//�й���ͼ���
				if(pTmp->sGroupParamName[0] != '\0')
				{
					/*if(str.size() == 1)
						return false;*/
					char sKey[MAX_SECTION_LEN+MAX_KEY_NAME_LEN+1] = {0};
					BaseVarInfo *pVar = 0;
					if(m_poGroupIndex->get(pTmp->sGroupParamName,&pVar))
					{
						
						while(pVar)
						{
							sprintf(sKey,"%s.%s",pVar->sSectName,pVar->sParamName);
							if(strcmp(sKey,sFullFName)!=0 )
							{
								if(! searchGroupParam(str,sKey,i)){
									if(bShow)
										printf("%s%s%s%s%s\n","GROUP[",pTmp->sGroupParamName,"]�б�Ҫ����[",sKey,"]�˴�δ��ֵ!");
									return false;
								}
								pVar = pVar->next;
							} else {
								pVar = pVar->next;
							}
						}
					}
				}
				return true;
			} else {
				return true;//û�ҵ�������ɹ�
			}
		} else {
			if(bShow)
				printf("%s%s%s\n","����[",(char*)strT.c_str(),"]��ʽ����ʧ��!");
			return false;
		}
	}
	return false;//��ʽ�쳣 ����
}

//��һ�����ݿ�����û��Ҫ�ҵ�
bool BaseRuleMgr::searchGroupParam(vector<string> str, string strcmp,int iNoCmpPos)
{
	//���׸���������Ϣ,Ȼ������׸�����
	BaseSysParamRule *pRule = 0;
	BaseVarInfo mBaseVarInfo;
	for(int i = 0;i<str.size();i++)
	{
		if( i!=iNoCmpPos && str[i] == strcmp)
			return true;
	}
	return false;//�����ж� ֱ����ɹ�
}
//iPos�Ǵ�1��ʼ
char *BaseRuleMgr::getLinkParam(char *sSect,char *sParam,int iPos)
{
     if(sSect[0] != '\0'  && sParam[0] != '\0' && iPos>0)
     {
        BaseSysParamRule* pParamRule = 0;
        pParamRule = getParamRule(sSect,sParam);
        if(pParamRule)
            return getLinkParam(pParamRule,iPos);
        else
            return NULL;
     }
     return NULL;
}
char *BaseRuleMgr::getLinkParam(BaseSysParamRule *pParamRule,int iPos)
{
     if(pParamRule)
     {
        if(pParamRule->iLinkNum>=iPos)
            return pParamRule->sLinkParamName[iPos-1];
        else
            return false;
     }
     return false;
}
//true������Ҫ�ٲõĲ���
bool BaseRuleMgr::neddArbi(char *sSect,char *sParam)
{
	 if(sParam[0] == '\0' || sSect[0] == '\0')
	 	return false;
	 BaseSysParamRule* pParamRule = getParamRule(sSect,sParam);
	 if(pParamRule)
	 {
	 	if(pParamRule->cArbi == 'Y')
		{
			return true;
		}
	 }
	 return false;
}

//str��ʽ ��.����=����ֵ  ����:SYSTEM.location=beijing�����ĸ�ʽ,��ö���SYSTEM,������location,����ֵbeijing
int BaseRuleMgr::getEveryInfo(const char* str,string &sPureSect,string &sPureParam,string &sPureVal)
{
	 string sParam = str;
	 trimN(sParam);
	 int iValPos = sParam.rfind('=');
	 int iParamPos = sParam.find('.');
	 if(iParamPos<0){
		return -1;
	 }
	 sPureSect.clear();
	 sPureParam.clear();
	 sPureVal.clear();
	 if(iValPos<0)
	 {
	 	sPureSect.assign(sParam,0,iParamPos);
		sPureParam.assign(sParam,iParamPos+1,sParam.size());
	 } else {
	 	sPureSect.assign(sParam,0,iParamPos);
	 	sPureParam.assign(sParam,iParamPos+1,iValPos-iParamPos-1);
	 	sPureVal.assign(sParam,iValPos+1,sParam.size());	 		 	
	 }
	 trimN(sPureSect);
	 trimN(sPureParam);
	 trimN(sPureVal);
	 return 1;
}

//  �������Ƿ�����ֻ�����ӻ���ֻ�ܼ���
int BaseRuleMgr::showInOrDecrease(char* sSect,char *sParam)
{
	 BaseSysParamRule* pParamRule = getParamRule(sSect,sParam);
	 if(pParamRule)
	 {
		return pParamRule->iValueChange;
	 }
	 return 0;
}

bool BaseRuleMgr::IsGroupParam(char *section,char *param)
{
	 BaseSysParamRule *pTmp = 0;
	 char sFullFName[MAX_SECTION_LEN+MAX_KEY_NAME_LEN+1] = {0};
	 sprintf(sFullFName,"%s.%s",section,param);
	 if(m_poIndex && m_poIndex->get(sFullFName,&pTmp))
	 {
	 	//�й���ͼ���
	 	if(pTmp->sGroupParamName[0] != '\0')
	 	{
	 		/*char sKey[MAX_SECTION_LEN+MAX_KEY_NAME_LEN+1] = {0};
			BaseVarInfo *pVar = 0;
			if(m_poGroupIndex->get(pTmp->sGroupParamName,&pVar))
			{
				
			}*/
			return true;
		}
	 }
	 return false;
}

int BaseRuleMgr::CheckDigitString(char *value)
{
	 if(!value || value[0] == '\0')
	 	return 0;//
	 for(int i = 0;i<strlen(value);i++)
	 {
	 	 if(i==0)
		 {
		 	if(isdigit(value[i])==0)
			{
		 		if(!(value[i] == '-'))
					return CHAR_OR_NUM_ERR;
			}
		 } else {
		 	if(isdigit(value[i])==0)
				return CHAR_OR_NUM_ERR;
		 }
	 }
	 return 0;
}

int BaseRuleMgr::CheckDigitString(BaseSysParamRule* pParamRule, char *value)
{
	if(pParamRule)
    {
		switch(pParamRule->iCharOrNum)
		{
			case 1: //num
			{
				return CheckDigitString(value);
				break;
			}
			default:
			{
				return 0;
			}
		}
	}
	return 0;
}

int BaseRuleMgr::CheckDigitString(char *section, char *param, char *value)
{
	if(section[0] != '\0'  && param[0] != '\0')
    {
        BaseSysParamRule* pParamRule = 0;
        pParamRule = getParamRule(section,param);
		if(!pParamRule)
			return 0;
		switch(pParamRule->iCharOrNum)
		{
			case 1:
			{
				return CheckDigitString(value);
				break;
			}
			default:
			{
				return 0;
			}
		}
	}
	return 0;
}
bool BaseRuleMgr::isRuleNumeric(char* pString)
{
	if(0== pString)
		return false;
	int i=0;
	for( ;i<strlen(pString);i++){
		if((i==0)&&(pString[0]=='-'))
			continue;
		else{
			if(isdigit(pString[i]))
				continue;
			else
				break;
		}
	}
	if(i<strlen(pString))
		return false;
	return true;
}
