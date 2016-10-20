#include "HssParamMonitor.h"

#include <ctype.h>
#include "Log.h"
#include "MBC.h"

TSysParamInfo *HBParamMonitor::SysParamList = 0; 
//SysParamClient *HBParamMonitor::gpCmdOpera = 0;

HBParamMonitor::HBParamMonitor()
{
	m_poCmdOpera = 0;
	m_poCmdCom = 0;
	m_iWaitTime = 10;
	pIndexList  = 0;
	INFOPOINT(3,"[HBParamMonitor]%s","��ʼ��");
	m_poBaseRuleMgr = new BaseRuleMgr();;
}

HBParamMonitor::~HBParamMonitor()
{
	free(m_poParamHisRecord);
	INFOPOINT(3,"[HBParamMonitor]%s","����");
}


void HBParamMonitor::memsetSysInfo()
{
	for(int i=0;i<MAX_ITEMS;i++)
	{
		memset(&m_poParamHisRecord[i],'\0',sizeof(struct ParamHisRecord));
	}
	INFOPOINT(3,"[HBParamMonitor.memsetSysInfo]%s","����m_poParamHisRecord����");
}

// ��ʼ����������,���ô�����֮ǰ����Ҫ��ʼ����Щ����
void HBParamMonitor::init(int iWaitTime,CommandCom *pCmdCom)
{
     m_poCmdCom = pCmdCom;
     m_iWaitTime = iWaitTime*10+1;
}

// ��ȡ������monitor��ʾ��ʱ�������ֵ
int HBParamMonitor::getRows()
{
	return m_iRowNum;
}

bool HBParamMonitor::getErrInfo(char *sErrInfo)
{
	 if(sizeof(sErrInfo)<=0)
	 	return false;
	 sErrInfo[0] = '\0';
	 strncpy(sErrInfo,m_poCmdOpera->m_sMONErrInfo,sizeof(sErrInfo));
	 return true;
}

void HBParamMonitor::clearEnv()
{
	 memsetSysInfo();
	 resetParams();
	 m_iRowNum = 0;
}

// sFSectTmpΪ����
// ����ǰ��ն���-��������������(sSectName�Ƕ���,��SYSTEM;sVal�Ǵ�����.����������ʽ;��:SYSTEM.state=online,SYSTEM.state.eff_date=20101010235600)
long HBParamMonitor::dealParamOpera(char *sSectName,char *sVal, long lCMDID)
{
	/*if(!m_poCmdCom)
		return -1;// ������*/
	if(!m_poCmdOpera){
		m_poCmdOpera = new SysParamClient();
		m_poCmdOpera->setDebugState(true);
	}
	long res = -1;
	resetParams();
	INFOPOINT(3,"[HBParamMonitor.dealParamOpera]%s","����ʵ�ʲ������ݴ�������");
	res = m_poCmdOpera->deal4Monitor(sSectName,sVal,lCMDID,m_poCmdCom);
	if(res == 0)
		return 0;//�޷��ؽ��,��ʱ
	
	switch(lCMDID)
	{
		case MSG_PARAM_CHANGE:
		case MSG_PARAM_S_C:
		case MSG_PARAM_S_D:	
		{					
			/*if(pIndexList)
			{
				pIndexList->clearAll();
				delete pIndexList; pIndexList = 0;
			}*/
			m_iRowNum = 0;
			return res;
		}
		default:
		{
				m_iParamNum = 0;
				memsetSysInfo();
				long lRes = m_poCmdOpera->VecToSysParamInfo(m_poParamHisRecord,m_iParamNum,lCMDID);
				if(m_iParamNum<=0)
					return 0;//�з�������,����������ΪNULL
				res = GetParamInfoAll();
				return res;
				break;
		}
	}
	return 0;//������
}	

void HBParamMonitor::FormatParamHisRecord()
{
	 for(int iKey=0;iKey<m_iParamNum; iKey++) 
	 {
	 	memset(m_poParamHisRecord[iKey].sWholeParamName,'\0',sizeof(m_poParamHisRecord[iKey].sWholeParamName));
		sprintf(m_poParamHisRecord[iKey].sWholeParamName,"%s%s%s%s","[",m_poParamHisRecord[iKey].sSectName,"]",".");
		strcat(m_poParamHisRecord[iKey].sWholeParamName,m_poParamHisRecord[iKey].sParamName);
	 }
}

ParamHisRecord *HBParamMonitor::getParamRecordByOffset(int iOffset)
{
	if(iOffset>=0 && iOffset<= MAX_ITEMS && m_poParamHisRecord)
		return &m_poParamHisRecord[iOffset];
	else 
		return NULL;
}

//  ��ȡ����(����ת��,ֱ�Ӱ��� �� ����=ֵ ��Ч���ڵĸ�ʽ��ʾ)[�Ľ���]
long HBParamMonitor::GetParamInfoAll()
{
	long res = 0; 
	resetParams();
	FormatParamHisRecord();
	if(pIndexList){
		pIndexList->clearAll();
		//delete pIndexList;
		//pIndexList = 0;
	} else {
		pIndexList = new HashList<int>(MAX_ITEMS*4+16);
	}
	//  ��ʼ�������±�
	int iListPos = 0;
	int iKey = 0;
	for(;iKey<m_iParamNum; iKey++) 
	{
	  if(m_poParamHisRecord[iKey].sParamValue[0] != '\0' && strcmp(m_poParamHisRecord[iKey].sParamOrValue,m_poParamHisRecord[iKey].sParamValue)!=0)
	  {
		 analyseParamInfo(iListPos,m_poParamHisRecord[iKey].sWholeParamName,m_poParamHisRecord[iKey].sParamOrValue,".ԭֵ=",iKey);
		 analyseParamInfo(iListPos,m_poParamHisRecord[iKey].sWholeParamName,m_poParamHisRecord[iKey].sParamValue,".��ֵ=",iKey);	 
	  } else {
	  	analyseParamInfo(iListPos,m_poParamHisRecord[iKey].sWholeParamName,m_poParamHisRecord[iKey].sParamValue,"=",iKey);		
	  }
		if(m_poParamHisRecord[iKey].sDate[0] != '\0')
	  {
			 analyseParamInfo(iListPos,m_poParamHisRecord[iKey].sWholeParamName,m_poParamHisRecord[iKey].sDate,"��Ч����: ",iKey);
	  } 
	  try
	  {
	  	if(!m_poBaseRuleMgr)
			m_poBaseRuleMgr = new BaseRuleMgr();
	  } catch (...)
	  {	
			 	m_poBaseRuleMgr = 0;		 	
	  }
	  if(m_poBaseRuleMgr)
	  {
	  		char sEffType[64] = {0};
			BaseSysParamRule *pParamRule = m_poBaseRuleMgr->getParamRule(m_poParamHisRecord[iKey].sSectName,m_poParamHisRecord[iKey].sParamName);
			if(pParamRule)
			{
				switch(pParamRule->iEffDateFlag)
				{
					case 1:
						strcpy(sEffType,"������Ч");
						break;
					case 2:
						strcpy(sEffType,"������Ч");
						break;
					case 3:
						strcpy(sEffType,"�����趨����Ч������Ч");
						break;
					default:
						strcpy(sEffType,"������Ч");
						break;
			 	}
		  	} else {
		  		 strcpy(sEffType,"������Ч");
		  	}
		  	analyseParamInfo(iListPos,m_poParamHisRecord[iKey].sWholeParamName,sEffType," ��Ч��ʽ: ",iKey);
	  }	
	}
	m_iRowNum = iListPos;	
	res =  (long)iListPos;   
	INFOPOINT(3,"[HBParamMonitor.GetParamInfoAll],���������ݸĳ�����ʾ��ʽ,��%ld������",res);
    return res;
}

//  ���ַ���Ŀ,data�Ŀ�ʼ�±�;ͬʱ����index��data����ϵ,sStrAdd:Ҫ�ڲ���ǰ���ӵ��ַ�,��'.OldValue='
int HBParamMonitor::analyseParamInfo(int &iListPos,char *sParamName,char *sParam,char *sStrAdd,int iSysParamKey)
{
	if(sParam[0] == '\0' || sParamName[0] == '\0') 
		return 0;
	if(iListPos >= MAX_ITEMS)
		return iListPos;
	char sMaxParam[2048] = {0};
	sprintf(sMaxParam,"%s%s%s\0",sParamName,sStrAdd,sParam);
	
	int iMaxParamStrlen = strlen(sMaxParam); //��ȡ�ַ���ֵ
	int iNeedRow = 0; // ����Ҫ������
	
	iNeedRow = iMaxParamStrlen/HBPARAM_LINE_VAL;
	int iMod = iMaxParamStrlen%HBPARAM_LINE_VAL;
	if(iMod)
		iNeedRow++;
	
	for(int iNL = 0; iNL<iNeedRow; iNL++)
	{
		strncpy(param_list_data[iListPos],&sMaxParam[iNL*HBPARAM_LINE_VAL],sizeof(char)*HBPARAM_LINE_VAL);
		param_list_index[iListPos] = param_list_data[iListPos];
		pIndexList->add(iListPos,iSysParamKey);
		iListPos++;				
	}
	INFOPOINT(3,"[HBParamMonitor.analyseParamInfo],������%s�ĳ�����ʾ��ʽ",sParamName);
	return iListPos;
}

void HBParamMonitor::linkParamListIndex(char pData[MAX_ITEMS][80],char *pIndex[MAX_ITEMS])
{
	for(int iN=0;iN<MAX_ITEMS;iN++)
	{
		memset(&pData[iN],'\0',sizeof(char)*80);
		pIndex[iN] = 0;
	}
	for(int i =0;i<m_iRowNum;i++)
	{
		strncpy(pData[i],param_list_data[i],sizeof(char)*80);
		pIndex[i] = pData[i];
	}
	INFOPOINT(3,"[HBParamMonitor.linkParamListIndex]%s","�����ⲿ��ʾ������Ϣָ��");
}

//  ����ʾ��������
void HBParamMonitor::resetParams()
{
	 for(int i =0;i<MAX_ITEMS;i++)
	 {
	 	memset(param_list_data[i],'\0',strlen(param_list_data[i]));
		param_list_index[i] = NULL;
	 }
	 INFOPOINT(3,"[HBParamMonitor.resetParams]%s","param_list_data�Լ�param_list_index��������");
}

void HBParamMonitor::show(long lCMD)
{
	INFOPOINT(3,"[HBParamMonitor.show]%s","���Ժ���,��ʾ����");
	switch(lCMD)
	{
		case MSG_PARAM_S_N:
		case MSG_PARAM_S_ALL:
     			for(int i=0;i<m_iRowNum;i++)
	 			{
	 		 		printf("%s\n",param_list_index[i]);
	 			}
				break;
		
		case MSG_PARAM_S_C://create
		case MSG_PARAM_S_D://delete
		case MSG_PARAM_ACTIVE_MOUDLE_S:
		case MSG_PARAM_ACTIVE_MOUDLE_M:
		case MSG_PARAM_CHANGE:  
						if(m_poParamHisRecord[0].lResult == INIT_OP_OK) 			
	 		 				printf("%s\n","�ɹ�");
						else if(m_poParamHisRecord[0].lResult == INIT_ERROR_NO_PARAM) 			
	 		 				printf("%s\n","�޶�Ӧ����");
						else if(m_poParamHisRecord[0].lResult == INIT_ERROR_UNRECOGNIZED)
							printf("%s\n","�����޷�ʶ��");
						else if(m_poParamHisRecord[0].lResult == -90)
							printf("%s\n","����ִ��ʧ��");
						else
							printf("%s%ld\n","���δ֪: ",m_poParamHisRecord[0].lResult);
						break;
	}
}
// �����������
// ��������:lCMD����ID.sWholeVlaue�������Ĳ��������ߴ�����ֵ��ȫ����ʽ; ����:SYSTEM.location=beijing.effdate=2010031010101010
int HBParamMonitor::ruleCheck(long lCMD,char *sWholeVlaue)
{
	if(!m_poCmdOpera){
		m_poCmdOpera = new SysParamClient();
		m_poCmdOpera->setDebugState(true);
	}
	return m_poCmdOpera->ruleCheck(lCMD,sWholeVlaue);
}

ParamHisRecord *HBParamMonitor::getParamRecordByLine(int iLine)
{
    int iOffset = -1;
	if(NULL == pIndexList)
		return NULL;
    if(pIndexList->get(iLine,&iOffset))
    {
        return &m_poParamHisRecord[iOffset];
    }
    return NULL;
}
/*-----------------------------------------------------------------*/
// sFSectTmpΪ����
// ����ǰ��ն���-��������������(sSectName�Ƕ���,��SYSTEM;sVal�Ǵ�����.����������ʽ;��:SYSTEM.state=online,SYSTEM.state.eff_date=20101010235600)
long HBParamMonitor::dealParamOpera(int iLine,char *sNewVal,char *sEffDate, long lCMDID)
{
	return -3;
}	

void HBParamMonitor::copyParamHisRecord(ParamHisRecord *pPRecord)
{
}
