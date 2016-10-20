#include "SysParamClient.h"
#include "Exception.h"
#include "MBC.h"
#include "Date.h"
#include "ParamDefineMgr.h"
#include "ThreeLogGroup.h"
#include "MBC_ABM.h"


#define SHOW_RULE	9 //��ʾ����
#define EFFDATE     "effdate"

SysParamClient::SysParamClient()
{ 
	m_poCmdCom = 0;
	m_iWaitMax = 0;
	m_bMONDebug = false;
	m_bLogOut = true;
	m_poBaseRuleMgr = 0;
	//Log::init(508);

    m_bFront=false;
    m_iBegin=0;
    m_iEnd=0;
    m_pSplit=0;
	m_poSysParamHis  = 0;
	m_poSysParamHisEx = 0;
	m_pSysParamGet = 0;
#ifdef _ARBITRATION_
	m_pArbitrationMgr= ArbitrationMgr::GetInstance();
	if(!m_pArbitrationMgr)
	{
			throw(MBC_Arbitration+1);
	}
	m_iState = m_pArbitrationMgr->getSystemState();		//�������ϵͳ
	if(m_iState <0)
	{
			throw(MBC_Arbitration+2);
	}
	
    char instanceID[10]={0};
	sprintf(instanceID,"%d",1000);//�����ٲ�ƽ̨��ʼ��Ҫ����ģ������ʵ�������Լ�����ʵ������1000
	int iTemp=m_pArbitrationMgr->Init("hbparam",instanceID); 	
	if(iTemp !=0)
	{
		Log::log(0,"�ٲ�ƽ̨��ʼ�����ɹ���");
		throw(MBC_Arbitration+3);
		return ;
	}
	DenyInterrupt_A();
#endif
}

SysParamClient::SysParamClient(int iMode)
{
}

SysParamClient::~SysParamClient()
{
	if(m_poBaseRuleMgr){
		delete m_poBaseRuleMgr;
		m_poBaseRuleMgr = 0;
	}
}

/*
��������:��ʼ��
����˵��:���hbadmin����Χ�ṩ��CommandComָ���Լ����õ����ȴ�ʱ��(��λ�ǰٺ���)
����ֵ˵��:��
����:init(pCmdCom,30)
*/ 
void SysParamClient::init(CommandCom *pCmdCom,int iWaitTime)
{
	 m_poCmdCom = pCmdCom;
	 m_iWaitMax = iWaitTime*100;
}

/*
��������:������ʽ�������
����˵��:sParamDate �û�������Ч���� , lCMD����ID
����ֵ˵��:trueͨ�����
����:checkBaseFormat("SYSTEM.location=beijing,",MSG_PARAM_S_N);����ʧ��;"SYSTEM.location=beijing,"���˸�';'
*/ 
bool SysParamClient::checkBaseFormat(char *sParamDate,long lCMD)
{
	 int iPos = 0;
	 string strParamDate(sParamDate);
	 if(lCMD == MSG_PARAM_S_N)
	 {
	 	if(sParamDate[0] == '\0')
			return false;
	    iPos = strParamDate.rfind("=");
	 	if(iPos>0)
			return false;
	 } else if(lCMD == MSG_PARAM_CHANGE)
	 {
	 	if(sParamDate[0] == '\0')
			return false;
	    iPos = strParamDate.rfind("=");
		if(iPos<=0)
			return false;
	 } else if(lCMD == MSG_PARAM_S_C)
	 {
	 	if(strParamDate.size()==0)
			return false;
	 	iPos = strParamDate.rfind('=');
		if(iPos<=0)
			return false;
	 } else if(lCMD == MSG_PARAM_S_D)
	 {
	 	if(strParamDate.size()==0)
			return false;
	 	/*iPos = strParamDate.rfind('=');
		if(iPos>0)
			return false;*/
	 }
	 return true;
}

/*
��������:�����û���������,����õ����ս��,��ʾ���û�
����˵��:sArgv, iArgc ��ͬmain��������������
����ֵ˵��:>0�����ɹ�;=0�����������;<0����ʧ��
����:sArgv = {"hbparam","-list","SYSTEM"} iArgc=3
*/ 
int SysParamClient::resolveHBParam(char **Argv,int Argc)
{
	char sArgv[ARG_NUM][ARG_VAL_LEN];
	for(int i=0;i<Argc;i++)
	{
		memset(sArgv[i],'\0',strlen(sArgv[i]));
		strcpy(sArgv[i],Argv[i]);
	}
	
	return resolveMemParam(sArgv,Argc);
}

/*
��������:�����û���������,����õ����ս��,��ʾ���û�
����˵��:sArgv, iArgc ��ͬmain��������������
����ֵ˵��:>0�����ɹ�;=0�����������;<0����ʧ��
����:sArgv = {"hbparam","-list","SYSTEM"} iArgc=3
*/ 
int SysParamClient::resolveMemParam(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc)
{
    int iRes = resolveUsrParam(sArgv,iArgc);
    if(iRes>0)
    {
        char sTmpName[ARG_VAL_LEN] = {0};
        analyseParamName(m_sParamsValue,sTmpName);
        iRes = dealMem(sTmpName,m_sParamsValue,m_lSrcCMDID,true);
        if(iRes>0)
        {
			
            if(showMemMsgValues()<0)
            {
                return -1;
            }
			return 1;
        } else {
			if(m_lSrcCMDID == MSG_PARAM_ACTIVE_MOUDLE_S)
			{
				return	showMemMsgValues();
			}
        	return -1;
		}
    } else if(iRes == 0)
    {
        return 1;
    }
    return -1; 
}

/*
��������:�����û���������
����˵��:sArgv, iArgc ��ͬmain��������������
����ֵ˵��:>0�����ɹ�;=0�����������;<0����ʧ��
����:sArgv = {"hbparam","-list","SYSTEM"} iArgc=3
*/ 
int SysParamClient::resolveUsrParam(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc)
{
	int res = 0;
	reSetBaseParams(); //��������	
	m_iArgc = iArgc; //if((strcmp(sArgv[0],"hbparam") == 0) || (strcmp(sArgv[0],"hbinitserver") == 0)) 	
	
	char sFlag[128] = {0};
	strncpy(sFlag,sArgv[1],sizeof(sFlag));
	//Chang2Lower(sFlag);
	
	int i = 1;
	if(strcmp(sFlag,"-set")==0)
	{
	    m_bSetParams = true;
		m_sParamsValue[0] = '\0';
		i++;
		if(i<iArgc)
		{
            strcpy(m_sParamsValue,sArgv[i]);
        }
	} 
	else if(strcmp(sFlag,"-show")==0)
	{
		//m_bListAllParams = true;
		m_bListParams = true;
		m_sParamsValue[0] = '\0';
    }						
	else if(strcmp(sFlag,"-kill") == 0) 
	{ 	//���������Ϊhbinitserver����
		if(strcmp(sArgv[0],"hbinitserver") == 0)
	    {					
			m_lSrcCMDID = MSG_PROCESS_STOP_SIG;
		}
	}
	else if(strcmp(sFlag,"-list") == 0)
	{
		m_bListParams = true;
		m_sParamsValue[0] = '\0';
		i++;
		if (i<iArgc)
		{
            strcpy (m_sParamsValue, sArgv[i]); //�����������
        }
	} 
	else if(strcmp(sFlag,"-listall") == 0)
	{
		m_bListAllParams = true;
    }    	
    else if(strcmp(sFlag,"-ls")==0)
	{
		m_bListParams=true;
		m_sParamsValue[0]=0;

	}
	else if(strcmp(sFlag,"-create") == 0)
	{
		m_bCreateParams = true;
		m_sParamsValue[0] = '\0';
		i++;
		if (i<iArgc){
            strcpy (m_sParamsValue, sArgv[i]); //�����������
        }
    }   	
	else if(strcmp(sFlag,"-delete") == 0)
	{
		m_bDeleteParams = true;
		i++;
		if (i<iArgc){
			m_sParamsValue[0] = '\0';
            strcpy (m_sParamsValue, sArgv[i]); //�����������
		}
	}    	
    else if(strcmp(sFlag,"-refresh") == 0)
    {
		m_bRefreshParams = true;
		i++;
		if (i<iArgc)
		{
				m_sParamsValue[0] = '\0';
                strcpy (m_sParamsValue, sArgv[i]); //�����������
		}
	}				
	else if(strcmp(sFlag,"-rule")==0)
	{
		string strval;
		for(int j=2;j<iArgc;j++)
		{
			if(sArgv[j][0] != '\0')
			strval += sArgv[j];
		}
		if(!m_poBaseRuleMgr)
			m_poBaseRuleMgr = new BaseRuleMgr();
		m_poBaseRuleMgr->setDebugState(m_bMONDebug);
		m_poBaseRuleMgr->paramParseExplain(strval.c_str());
		return 0;
	}else {
		Log::log(0,"�������д�����[%s%s]��֧�ֵ�ѡ��",sArgv[0],sArgv[1]);
	    showHowToUseCMD();
	    return 0;
	}
	if(m_iArgc>4)
	{	
		//
		if(m_bSetParams && strcmp(sArgv[3],"=")==0)
		{
			//for(int j=4;j<iArgc;j++)
			//{
				strcat(m_sParamsValue,sArgv[3]);
				strcat(m_sParamsValue,sArgv[4]);
			//}
			
		}
		//
		//setFrontInfo(true);
	}
	if(m_bListAllParams)
	{
		m_lSrcCMDID = MSG_PARAM_S_ALL;
	} else if(m_bSetParams){ //-set
		if(m_sParamsValue[0] != '\0' ) // hbparam -set name1=vaue1,effdate=201010101010
		{
			m_lSrcCMDID = MSG_PARAM_CHANGE; 
		} else {
			m_lSrcCMDID = -1;
			Log::log(0,"�������д�����[%s%s]ȱ�ٵı�ѡ��",sArgv[0],sArgv[1]);
			return -1;
		}
	} else 	if(m_bListParams){
		if(m_sParamsValue[0] != '\0' ) // hbparam -list SectionName.name
		{ 
			m_lSrcCMDID = MSG_PARAM_S_N; //���ֵ��MSG.H����
		} else {
		    m_lSrcCMDID = MSG_PARAM_S_ALL;//MSG_PARAM_S_ALL;//MSG_LIST_SECTION;
				
		}
	} else if(m_bCreateParams){
		if(m_sParamsValue[0] != '\0' ) // hbparam -create SectionName.ParamName1=value
		{ 
			m_lSrcCMDID = MSG_PARAM_S_C; //���ֵ��MSG.H����
		} else {
			m_lSrcCMDID = -1;
			Log::log(0,"�������д�����[%s%s]ȱ�ٵı�ѡ��",sArgv[0],sArgv[1]);
			return -1;
		}
	} else if(m_bDeleteParams)
	{
		if(m_sParamsValue[0] != '\0' ){ // hbparam -delete SectionName.ParamName
			m_lSrcCMDID = MSG_PARAM_S_D; //���ֵ��MSG.H����
		} else {
			m_lSrcCMDID = -1;
			Log::log(0,"�������д�����[%s%s]ȱ�ٵı�ѡ��",sArgv[0],sArgv[1]);
			return -1;
		}
	} else if(m_bRefreshParams){ // hbparam -refresh -m[-s]				
		if(m_sParamsValue[0] != '\0' )
		{ 
			//Chang2Lower(m_sParamsValue);
			if(strcmp(m_sParamsValue,"-s")==0)
			{
				m_lSrcCMDID = MSG_PARAM_ACTIVE_MOUDLE_S; 
			} /*else if(strcmp(m_sParamsValue,"-m")==0)
			{
				m_lSrcCMDID = MSG_PARAM_ACTIVE_MOUDLE_M; 
			}*/
		} else {
			m_lSrcCMDID = MSG_PARAM_ACTIVE_MOUDLE_S; //Ĭ�����ڴ�ˢ���ļ�
		}
	} else {
		Log::log(0,"�������д�����[%s%s]��֧��ѡ��",sArgv[0],sArgv[1]);
		showHowToUseCMD();
		return -1;
	}
	//�ȼ��һ�²���������ʽ
	/*if(!checkBaseFormat(sArgv[2],m_lSrcCMDID))
	{
			Log::log(0,"����[%s%s]����ȡֵ",sArgv[0],sArgv[1]);
			printf("����[%s]��ʽ����\n",sArgv[2]);
			return -1;
	}*/
    /*if(!checkFrontSet(sArgv,iArgc))
	{
		return -1;
	}*/
	return 1;
}

bool SysParamClient::checkFrontSet(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc)
{
	 if(m_iArgc<8)
	 {	
		//
		return true;
		//
		//setFrontInfo(true);
	 }
	 //�����-set
	 if(m_bSetParams)
	 {
	 	//
		if(strcmp(sArgv[3],"=")==0)
		{
			//�ֶε����� abmparam -set system.desc = 123������
			return getFrontBeginEnd(5,7,sArgv,iArgc);
		} else {
			//�ֶε����� abmparam -set system.desc=123 -start 1 -end 2 -adm 1 ������
			return getFrontBeginEnd(3,5,sArgv,iArgc);
		}
	 } else if(m_bRefreshParams) {
	 	//abmparam -refresh -adm 1 ������
	 	if(strcmp(sArgv[2],"-adm")==0)
		{
			m_bFront = true;
		}
	 } else if(m_bListParams)
	 {
	 	//abmparam -list system.desc -start 1 -end 2 -adm 1 ������
		return getFrontBeginEnd(3,5,sArgv,iArgc);
	 } else if(m_bListAllParams)
	 {
	 	//abmparam -list -start 1 -end 2 -adm 1 ������
		return getFrontBeginEnd(2,4,sArgv,iArgc);
	 }
	 return true;
}

bool SysParamClient::getFrontBeginEnd(int begineOffset, int endOffset,char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc)
{
			m_bFront = false;
			if(strcmp(sArgv[begineOffset],"-start")==0)
			{
				for(int i=0;i<strlen(sArgv[begineOffset+1]);i++)
				{
					if(i==0 && sArgv[begineOffset+1][0] == '-')
						i++;
					if(!isdigit(sArgv[begineOffset+1][i]))
						return false;
				}
				m_iBegin = atoi(sArgv[begineOffset+1]);
			} else {
				return false;
			}
			if(strcmp(sArgv[endOffset],"-end")==0)
			{
				for(int i=0;i<strlen(sArgv[endOffset+1]);i++)
				{
					if(i==0 && sArgv[endOffset+1][0] == '-')
						i++;
					if(!isdigit(sArgv[endOffset+1][i]))
						return false;
				}
				m_iBegin = atoi(sArgv[endOffset+1]);
			} else {
				return false;
			}
			m_bFront = true;
			return true;
}

/*
��������:��������
*/ 
void SysParamClient::reSetBaseParams()
{
	m_bSetParams = false;
	m_bRefreshParams = false;
	m_bCreateParams = false;
	m_bDeleteParams = false;
	m_bListParams = false;
	m_bListAllParams = false;
	m_lSrcCMDID = 0;
	m_iArgc = 0;
	memset(m_sParamsValue,'\0',sizeof(m_sParamsValue));
	memset(m_sParamSetVal,'\0',sizeof(m_sParamSetVal));
	
	m_bShowHelp = false;
	clearPtr();
}

/*
����˵��:�����ΪHSS���Ĳ�����ʽת��
����ֵ:ת���ɹ�true �� ʧ��false
����˵��:ԭʼֵ value  , ת�����ֵ strdesc
����:bool res = change2hssString("SYSTEM.location=china,effdate=2019,SYSTEM.server_name=server1",str);
*/
bool SysParamClient::change2hssString(char *value,string &strdesc)
{
	vector<string> vec_str;
	getTagStringN(value,vec_str,',');
	if(vec_str.size()==0){
		return false;
	}
	int i = 0;
	for(vector<string>::iterator itr = vec_str.begin();itr!=vec_str.end();itr++)
	{
		trimN(*itr);
		if((*itr).size()==0)
		 	continue;
		if((*itr).size()>=7)
		{
		 	string tmp;
			tmp.assign((*itr),0,7);
			if(tmp.compare(EFFDATE)==0||tmp.compare("EFFDATE")==0)
			{
				if(itr == vec_str.begin())
					return false;
				//һ��˵�����stringӦ��
				strdesc += string(",");
				strdesc += (*itr);
			} else {
				if(itr == vec_str.begin()){
		 			strdesc += (*itr);
				} else {
					strdesc += string("+");
					strdesc += (*itr);
				}
			}
		} else {
		 	if(itr == vec_str.begin()){
		 		strdesc += (*itr);
			} else {
				strdesc += string("+");
				strdesc += (*itr);
			}
		}
	}
	return true;
}


/*
��������:��������ʽ�Լ���ֵ�Ƿ���Ϲ涨
����˵��:sParamSetVal�û��������Ч����,Show�Ƿ���ʾ������ʾ
����ֵ˵��:true���Ϲ����趨,false���Ϲ涨
����:IfSuitRule(lCMDID,"SYSTEM.location=beijing",true)
*/ 
bool SysParamClient::IfSuitRule(long lCMDID,char *sParamSetVal,string &strDsc,bool Show)
{
    if(lCMDID<0) 
	{
		m_lSrcCMDID = 0;
		if(Show)
		    printf("%s\n","������ʽ����!");
		return false;
	}
    if(lCMDID != MSG_PARAM_CHANGE)
	{
		strDsc = sParamSetVal;
		return true;
	}
	if(!m_poBaseRuleMgr)
	{
		try 
		{
			m_poBaseRuleMgr = new BaseRuleMgr();
		} catch(...) {
			if(Show)
				printf("%s\n","�������������в��Ϲ淶��,����!");
			Log::log(0,"����[%s]����ȡֵ",sParamSetVal);
			return false;
		}	
	}
		string strFrom;
		if(!change2hssString(sParamSetVal,strFrom))
		{
			if(Show)
				printf("%s\n","������ʽ�����Ϲ淶,����!");
			return false;
		}
	string strUse;
	string strUse2;
	int iStartGet=0;
	int iStartFind = 0;
	vector<string> vecParams;
	while(1){
		int iDepart = strFrom.find('+', iStartFind);
		if(iDepart>0){
		    if( strFrom.c_str()[iDepart-1] != '\\' ){
    		    strUse.append(strFrom, iStartGet, iDepart-iStartGet);
    		    strUse2.clear();
    		    bool bParser = paramParser((char*)strUse.c_str(),strUse2);
    		    if(!bParser ){
    		        printf("����[%s]��ʽ��������", strUse.c_str() );	
    		        return false;
    		    }
    		    if(!m_poBaseRuleMgr->check((char*)strUse.c_str(),Show) ){
    		        m_poBaseRuleMgr->paramParseExplain(Show);
    		        return false;
    		    }
    		    if( iStartGet>0 ){ strDsc += ","; }
    		    strDsc += strUse2;
    		    
    		    string strParam(mRecord[0].sSectName);
    		    strParam.append(".");
    		    strParam.append(mRecord[0].sParamName);
    		    vecParams.push_back(strParam);

    		    iStartGet = iDepart+1;
    		    strUse.clear();
    		}else{
    		    if(iDepart-iStartGet-1>0)
    		        strUse.append(strFrom, iStartGet, iDepart-iStartGet-1);
    		    strUse.append("+");
    		    iStartGet = iDepart+1;
    		}
		    iStartFind = iDepart+1;
	    }else{
	        break;
	    }
	    
	}
 
	if( strFrom.size() > iStartGet){
	    strUse.append(strFrom, iStartGet, strFrom.size());
	}
	
    strUse2.clear();
    bool bParser = paramParser((char*)strUse.c_str(),strUse2);
    if(!bParser ){
        printf("����[%s]��ʽ��������", strUse.c_str() );	
        return false;
    }
    if(!m_poBaseRuleMgr->check((char*)strUse.c_str(),Show) ){
        m_poBaseRuleMgr->paramParseExplain(Show);
        return false;
    }
    if( vecParams.size() >0 ){ strDsc += ","; }
    strDsc += strUse2;
    
    string strParam(mRecord[0].sSectName);
    strParam.append(".");
    strParam.append(mRecord[0].sParamName);
    vecParams.push_back(strParam);

	//���һ�²��������
	bool resRule = m_poBaseRuleMgr->checkGroupParam(vecParams,Show);
	if(!resRule)
	{
		if(Show)
			printf("%s\n","GROUP����ʧ��!");
	}	
	return resRule;
}

//2.8���°汾
int SysParamClient::dealMem(char *sSectName, string sSysParamValue, long lCMDID,bool Show)
{ 
	if(lCMDID == MSG_PARAM_CHANGE)
		prompt(sSysParamValue,Show);
	string strDsc;
	bool bRes = IfSuitRule(lCMDID,(char*)sSysParamValue.c_str(),strDsc,Show);
	if(bRes)
	{
	 	return dealWithMem(sSectName, (char*)strDsc.c_str(), lCMDID, Show);
	} else {
		if(Show)
			printf("����%s�����Чʧ��\n",strDsc.c_str());
		ThreeLogGroup TLog;
		char paramfullname[256] = {0};
		sprintf(paramfullname,"%s.%s",mRecord[0].sSectName,mRecord[0].sParamName);
		char woringinof[512] = {0};
		char username[32] = {0};
		FILE* fp = popen("whoami","r");
		if(fp)
		{
			fgets(username,32,fp);
		}
		pclose(fp);
		for(int i=0;i<strlen(username);i++)
		{
			if(username[i] == '\n')
				username[i] = '\0';
		}
		sprintf(woringinof,"�û�%sִ��abmparam -set�޸Ĳ����Ĳ����� ��������[%s]�����쳣,�����Ϲ����趨!",username,(char*)sSysParamValue.c_str());
		TLog.log(MBC_COMMAND_ABMPARAM,LOG_LEVEL_WARNING,LOG_CLASS_INFO,LOG_TYPE_PARAM,-1,paramfullname,woringinof);
		return -1;;
	}
}

/*
��������: �������������initserver,�õ�������
��������: sSectName��, sSysParamValue ����ֵ(��.����=ֵ,effdate=2011������Ч����), lCMDID����ID, ShowErrInfo�Ƿ���ʾ������Ϣ
����ֵ: ʧ�ܷ���ֵ<0;��ʱ0;��ȡ��ֵΪ1
����: dealWithMem("SYSTEM","SYSTEM.location=beijing",MSG_PARAM_S_N, true)
*/
int SysParamClient::dealWithMem(char *sSectName,char *sSysParamValue,long lCMDID,bool ShowErrInfo)
{
	m_iErrNo = 0;
	if(!m_poCmdCom){
		m_iErrNo = 1;
		return 0;
	}
	if(m_poSysParamHis)
		delete []m_poSysParamHis;
	m_poSysParamHis = 0;
	
	switch(lCMDID)
	{
		case MSG_PARAM_CHANGE:
		{
			if(!sSysParamValue || sSysParamValue[0] == '\0')
				return 0;
			string strParamVal(sSysParamValue);
			if(!m_poCmdCom->changeSectionKey(strParamVal))
			{
				m_iErrNo = 2;
				return 0;
			}
			string sect;
			string param;
			string full;
			string strvalue(sSysParamValue);
			int iRes = strvalue.find('.');
			int iResEnd = strvalue.rfind('=');
			int iResTmp=strvalue.rfind(".new_value");
			if(iResTmp>0){
				iResEnd=iResTmp;
			}
			if(iRes)
			{
				//
				sect.assign(strvalue,0,iRes);
				param.assign(strvalue,iRes+1,iResEnd-iRes-1);
			}
			full.assign(strvalue,0,iResEnd);
			//
						
			//dealWithMem((char*)sect.c_str(),(char*)full.c_str(),MSG_PARAM_S_N,ShowErrInfo);
			break;
		}
		case MSG_PARAM_S_ALL:
		{
			if(!m_poCmdCom->getSection(NULL,&m_poSysParamHis,m_iCnt))
			{
					return 0;
			}
			break;
		}
		case MSG_PARAM_ACTIVE_MOUDLE_S:
		{
			m_poSysParamHis = 0;
			m_iCnt = 0;
			/*if(!m_poCmdCom->FlashMemory(&m_poSysParamHis,m_iCnt))
			{
				m_iErrNo = 4;
				return 0;
			}*/
			if(!m_poCmdCom->FlashMemoryEx(&m_poSysParamHis,m_iCnt,&m_poSysParamHisEx,m_iCntEx))
			{
				m_iErrNo = 4;
				return 0;
			}
			break;
		}
		case MSG_PARAM_S_C:
		{
			if(!sSysParamValue || sSysParamValue[0] == '\0')
				return 0;
			string strParamVal(sSysParamValue);
			string createstr;
			createstr = "#create ";
			createstr += strParamVal;
			memset(mRecord,'\0',sizeof(ParamHisRecord));
			int pos = strParamVal.find('=');
			if(pos<=0)
				return 0;
			string tmpval;
			tmpval.assign(strParamVal,0,pos);
			int pos2 = tmpval.find('.');
			strncpy(mRecord[0].sSectName,(char*)tmpval.c_str(),pos2);
			string tmp;
			tmp.assign(tmpval,pos2+1,tmpval.size());
			strcpy(mRecord[0].sParamName,(char*)tmp.c_str());
			if(!m_poCmdCom->changeSectionKey(createstr));
				return 0;
			break;
		}
		case MSG_PARAM_S_D:
		{
			if(!sSysParamValue || sSysParamValue[0] == '\0')
				return 0;
			string strParamVal(sSysParamValue);
			string delstr;
			delstr = "#delete  ";
			delstr += strParamVal;
			memset(mRecord,'\0',sizeof(ParamHisRecord));
			int pos = strParamVal.find('.');
			strncpy(mRecord[0].sSectName,(char*)strParamVal.c_str(),pos);
			string tmp;
			tmp.assign(strParamVal,pos+1,strParamVal.size());
			strcpy(mRecord[0].sParamName,(char*)tmp.c_str());
			if(!m_poCmdCom->changeSectionKey(delstr));
				return 0;
			break;
		}
		/*case MSG_PARAM_ACTIVE_MOUDLE_M:
		{
			m_iErrNo = 0;
			return 1;
		}*/
		case MSG_LIST_SECTION:
		{
			m_poSysParamHis = 0;
			m_iCnt = 0;
			if(!m_poCmdCom->getAllSections(&m_poSysParamHis,m_iCnt))
			{
				return 0;
			}
			break;
		}
		case MSG_PARAM_S_N:
		{
			if(!sSysParamValue || sSysParamValue[0] == '\0')
				return 0;
			string strParamVal(sSysParamValue);
			int iRes = strParamVal.find('.');
			if(iRes>0)
			{
				string strTmp =strParamVal;
				strTmp.erase(0,strTmp.find_first_of('.')+1);
				trimN(strTmp);
				//-list SYSTEM.*��ͬ -list SYSTEM
				if(strTmp.size()>0 && strTmp.compare("*")==0)
				{
					if(!m_poCmdCom->getSection(sSectName,&m_poSysParamHis,m_iCnt))
					{
						return 0;
					}
				} else {
			//		m_poSysParamHis = new SysParamHis[1];
				//	if(!m_poCmdCom->getSectionParam(sSectName,strTmp.c_str(),*m_poSysParamHis))
					if(!m_poCmdCom->getSectionParams(sSectName,strTmp.c_str(),&m_poSysParamHis,m_iCnt))
					{
						return 0;
					}else {
						//if(m_poSysParamHis->sSectionName[0] == '\0'){
						if(m_iCnt<=0){
							if(ShowErrInfo)
								printf("����[%s]����ʧ��!\n",sSysParamValue);
							return 0;
						}
					}
				}
			} else {				
				if(!m_poCmdCom->getSection(sSectName,&m_poSysParamHis,m_iCnt))
				{
					return 0;
				}
			}
			break;
		}
		default:
		{
			return 0;
			//break;
		}
	}
	return 1;
}


/*
������showMemMsgValues
���ܣ���ʾ�ӹ����ڴ��õ����ݽ��к�����ʾ
����������Show�Ƿ���ʾ���
����:showMemMsgValues(true)
*/
long SysParamClient::showMemMsgValues()
{	
	 	switch(m_lSrcCMDID)
		{
			case MSG_PARAM_S_C:
	 		case MSG_PARAM_CHANGE:
			{
				/*printf("<%s>\n","WEBFORSGW");
    			printf("%d\n",1);
    			printf("%d\n",1);
    			printf("%s\n","����������");*/
				if( !m_bFront )
				{
				if(m_poBaseRuleMgr)//��������û����Ч˵��NEWʧ����
				{
					BaseSysParamRule *pParamRule = m_poBaseRuleMgr->getParamRule(mRecord[0].sSectName,mRecord[0].sParamName);
					if(pParamRule)
					{
						if(pParamRule->iEffDateFlag == 1)
						{
								printf("%s\n","������Ч");
						} else if(pParamRule->iEffDateFlag == 2){
								printf("%s\n","������Ч");
						} else if(pParamRule->iEffDateFlag == 3){
								Date mDate(mRecord[0].sDate);
								printf("%s%s%s\n","�����趨����Ч����",mDate.toString(),"��Ч");
						} else {
								printf("%s\n","������Ч");
			 			}
					  } else {
								printf("%s\n","������Ч");
			 		  }	
				} else {
		  		  	printf("%s\n","������Ч");
				}
				} 
				//printf("</%s>\n","WEBFORSGW");
				break;
			}
			case MSG_PARAM_S_D:
			{		
				printf("%s\n","����ɹ�!");
				break;
			}
			case MSG_PARAM_S_ALL:
			{	
				
				showResults(m_poSysParamHis,m_iCnt); // ��ʾ����
				delete []m_poSysParamHis;
				break;
			}
			case MSG_PARAM_ACTIVE_MOUDLE_S://�ļ�ˢ�ڴ�
			{	
				/*printf("<%s>\n","WEBFORSGW");
    			printf("%d\n",1);
    			printf("%d\n",1);
    			printf("%s\n","����������");*/
				if( !m_bFront )
				{
				if(m_iCnt)
				{
					printf("%s\n","���²���ˢ��ʧ��,����");
					showResults(m_poSysParamHis,m_iCnt); // ��ʾ���� 
					delete []m_poSysParamHis;
					return -1;
				} else {
						printf("%s\n","����ˢ�³ɹ�");
						if(m_iCntEx)
						{
							printf("%s\n","���²�����ֵ�����仯:");
							showResults(m_poSysParamHisEx,m_iCntEx); // ��ʾ���� 
							delete []m_poSysParamHisEx;
							return 1;
						}
						
				}
				} 
				//printf("</%s>\n","WEBFORSGW");		
				break;
			}
			/*case MSG_PARAM_ACTIVE_MOUDLE_M://�ڴ�ˢ�ļ�
			{//�ڴ�ˢ�ļ� ���ߵ�������ǳɹ�
				printf("%s\n","����ˢ�³ɹ�");
			}*/
			case MSG_LIST_SECTION:
			case MSG_PARAM_S_N:
			{		
				if(m_iCnt)
				{					
					showResults(m_poSysParamHis,m_iCnt); // ��ʾ���� 
					delete []m_poSysParamHis;
				} else {
					printf("%s\n","����������,�鿴ʧ��");
					return -1;
				}	
				break;
			}
			default:
			{
				break;
			}
		}		
	 	return 1;//�ߵ���ߵ�Ĭ�϶��ǳɹ�
}


/*
������showMsgValues
���ܣ���ʾ����Ϣ���л�õ���Ϣ���к�����ʾ
����������Show�Ƿ���ʾ���
����:showMsgValues(true)
*/
long SysParamClient::showMsgValues(bool Show)
{
	 return 0;
}


void SysParamClient::setFrontInfo(bool bFront,int iBegin, int iEnd, const char *sSplit)
{
     m_bFront=bFront;
     m_iBegin=iBegin;
     m_iEnd=iEnd;
     m_pSplit=sSplit;
}


/*
��������:��ʾ��ȡ�����в�������
����˵��:pData :SysParamHis���͵�����Դ��ָ��,iNum:����Դ��������Ŀ
����ֵ˵��:��
����:showResults((void*)pData,100)
*/
void SysParamClient::showResults(void *pData, int iNum)
{ 
   SysParamHis *pParamHis = 0;
   pParamHis = (SysParamHis*)(pData);

if( !m_bFront )
{

   int iSeqNum = iNum/PAGE_MAX_SHOW_NUM;
   if(iNum%PAGE_MAX_SHOW_NUM)
		 iSeqNum++;		 
	for(int i = 0;i<iSeqNum;)
	{
		showOnePage(pParamHis,i,iNum);
		if(iSeqNum == 1)
			i++;
		if(iSeqNum>1)
		{			 
			int iRes = showControl(i);	
			if(iRes == -9)
			    return;	
			else if(iRes == -1)
			{
				if(i>0)
					i--;
			}
			else if(iRes == 1)
			{
				i++;
		    } else {
			 	i++;
			}	 
		 }
	}	 
}else{
    char sLable[128]={0};
    sprintf(sLable,"����%s������%s����ԭֵ%s������ֵ%s������Ч����%s��Ч��ʽ%sȡֵ��Χ",
      m_pSplit,m_pSplit,m_pSplit,m_pSplit ,m_pSplit ,m_pSplit);
	//if(iNum > 1)
	//{
    //printf("%d\n%d\n%s\n", 1, iNum, sLable );
	printf("<%s>\n","WEBFORSGW");
    printf("%d\n",1);
    printf("%d\n",iNum);
	//}
    printf("%s\n",sLable);
    if( m_iBegin==m_iEnd && m_iEnd==0){
        m_iEnd=9999;
    }
	int iEndSeq = m_iEnd;
	for(int j=( m_iBegin-1>=0?m_iBegin-1:0); j<iEndSeq && j<iNum; j++ )
	{
		pParamHis = (SysParamHis*)(pData)+j;
		if(pParamHis->sSectionName[0] == '\0'){
			iEndSeq++;
			continue;
		}
        printf("%s%s%s", pParamHis->sSectionName, m_pSplit , pParamHis->sParamName);
		//��ȡ��������
		if(!m_poBaseRuleMgr)
			m_poBaseRuleMgr = new BaseRuleMgr();
		char effType[32] = {0};
		BaseSysParamRule *pParamRule = 0;
		if(m_poBaseRuleMgr)//��������û����Ч˵��NEWʧ����
		{
			pParamRule = m_poBaseRuleMgr->getParamRule(pParamHis->sSectionName,pParamHis->sParamName);
			if(pParamRule)
			{
				if(pParamRule->iEffDateFlag == 1)
				{
					sprintf(effType,"%s","������Ч");
				} else if(pParamRule->iEffDateFlag == 2){
					sprintf(effType,"%s","������Ч");
				} else if(pParamRule->iEffDateFlag == 3){
					sprintf(effType,"%s%","�����趨����Ч������Ч");
				} else {
					sprintf(effType,"%s","������Ч");
			 	}
			} else {
				sprintf(effType,"%s","������Ч");
			}
		}
		char valuerule[512] = {0};
		if(pParamRule)
		{
			if(pParamRule->sMaxVal[0] != '\0'){
				sprintf(valuerule,"%s%s","���ֵ:",pParamRule->sMaxVal);
			}
			if(pParamRule->sMinVal[0] != '\0'){
				strcat(valuerule," ��Сֵ:");
				strcat(valuerule,pParamRule->sMinVal);
			}
			if(pParamRule->sEnumVal[0][0] != '\0'){
				strcat(valuerule," ö��ֵ:");
				for(int i =0;i<pParamRule->iEnumPos;i++)
				{
					if(pParamRule->sEnumVal[i][0] != '\0')
						strcat(valuerule,pParamRule->sEnumVal[i]);
					if(pParamRule->sEnumVal[i+1][0] == '\0'){
						break;
					} else 
						strcat(valuerule,",");
				}
			}
			switch(pParamRule->iValueChange)
			{
				case 1:
					strcat(valuerule," �˲���ֻ������");
					break;
				case 2:
					strcat(valuerule," �˲��������޸�");
					break;
				case 3:
					if(pParamRule->sValueChange[0] != '\0'){
						strcat(valuerule," ����ƥ��Ĺ���:");
						strcat(valuerule,pParamRule->sValueChange);
					}
					break;
				case 4:
					strcat(valuerule," �˲���ֻ�ܼ�С");
					break;
				case 5:
					strcat(valuerule," �˲���ֻ����ip");
					break;
			}
			if(pParamRule->sDisobey[0] != '\0')
			{
				strcat(valuerule," ���������ַ�:");
				strcat(valuerule,pParamRule->sDisobey);
			}
			if(pParamRule->sTime24Start[0] != '\0' && pParamRule->sTime24End[0] != '\0')
			{
				strcat(valuerule," ʱ����Сֵ:");
				strcat(valuerule,pParamRule->sTime24Start);
				strcat(valuerule," ʱ�����ֵ:");
				strcat(valuerule,pParamRule->sTime24End);
			}
			if(pParamRule->iParamNeed>0)
			{
				strcat(valuerule," �˲����������");
			}
			if(pParamRule->sGroupParamName[0] != '\0')
			{
				strcat(valuerule," �˲������ڲ�����:");
				strcat(valuerule,pParamRule->sGroupParamName);
			}
		} 
        printf("%s%s%s%s%s%s%s%s%s%s\n", 
            m_pSplit,pParamHis->sParamOrValue,
            m_pSplit,pParamHis->sParamValue,
            m_pSplit,pParamHis->sDate,
			m_pSplit,effType,m_pSplit,valuerule);

	}
	//if(iNum > 1)
		printf("</%s>\n","WEBFORSGW");
}

}

/*
��������:��ʾһ��ҳ�������
����˵��:pData :SysParamHis���͵�����Դ,iNowPos:������Դ�ĵ�ǰƫ����(ÿ��ҳ����ʾ�������ƫ������Ӧ���ݿ�ʼ��ʾ);iTotalNum:����Դ��������Ŀ
����ֵ˵��:��
����:showOnePage(pData,0,100)
*/
void SysParamClient::showOnePage(SysParamHis *pData,int iNowPos,int iTotalNum)
{
	SysParamHis *pParamHis = 0;
	int iNumLeft = iTotalNum-iNowPos*PAGE_MAX_SHOW_NUM;
	int iShowNum = iNumLeft>PAGE_MAX_SHOW_NUM?PAGE_MAX_SHOW_NUM:iNumLeft;
	int iEffDateFlag = 2; //Ĭ������
	
	for(int j=0;j<iShowNum;j++)
	{
		pParamHis = (pData+PAGE_MAX_SHOW_NUM*iNowPos)+j;
		if(pParamHis->sSectionName[0] == '\0')
			continue;
		//��ȡ��������
		if(!m_poBaseRuleMgr)
			m_poBaseRuleMgr = new BaseRuleMgr();
		char effType[32] = {0};
		BaseSysParamRule *pParamRule = 0;
		if(m_poBaseRuleMgr)//��������û����Ч˵��NEWʧ����
		{
			pParamRule = m_poBaseRuleMgr->getParamRule(pParamHis->sSectionName,pParamHis->sParamName);
			if(pParamRule)
			{
				iEffDateFlag = pParamRule->iEffDateFlag;
				if(pParamRule->iEffDateFlag == 1)
				{
					sprintf(effType,"%s","������Ч");
 			  } else if(pParamRule->iEffDateFlag == 2){
					sprintf(effType,"%s","������Ч");
				} else if(pParamRule->iEffDateFlag == 3){
					sprintf(effType,"%s%","�����趨����Ч������Ч");
				} else {
					sprintf(effType,"%s","������Ч");
			 	}
			} else {
				sprintf(effType,"%s","������Ч");
			}
		}
		printf("\n");
	 	printf("%-18s%s%s%-18s\n", "[������]: ",pParamHis->sSectionName,"." ,pParamHis->sParamName);
		if(pParamRule && pParamRule->sDesc[0] != '\0'){
			printf("%-18s%-18s\n","��������: ",pParamRule->sDesc);
		}
		switch (iEffDateFlag)
		{
			case 1:
				if(pParamHis->sParamValue[0] != '\0' && pParamHis->sParamOrValue[0] != '\0' 
						&& strcmp(pParamHis->sParamValue,pParamHis->sParamOrValue)!=0)
				{
	 				printf("%-18s%-18s\n", "  ԭֵ: ", pParamHis->sParamOrValue);
	 				printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamValue);					
				}
				else if(pParamHis->sParamValue[0] != '\0' && pParamHis->sParamOrValue[0] == '\0')
				{
	 				printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamValue);
	 			} 
	 			else if(pParamHis->sParamOrValue[0] != '\0')
	 			{
					printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamOrValue);
				}
				break;
			case 2:
				if(pParamHis->sParamValue[0] != '\0' && pParamHis->sParamOrValue[0] != '\0' 
						&& strcmp(pParamHis->sParamValue,pParamHis->sParamOrValue)!=0)
				{
	 				printf("%-18s%-18s\n", "    ��ǰֵ: ", pParamHis->sParamOrValue);
	 				printf("%-18s%-18s\n", "��������ֵ: ", pParamHis->sParamValue);					
				}
				else if(pParamHis->sParamValue[0] != '\0' && pParamHis->sParamOrValue[0] == '\0')
				{
	 				printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamValue);
	 			} 
	 			else if(pParamHis->sParamOrValue[0] != '\0')
	 			{
					printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamOrValue);
				}			
				break;
			case 3:
				if(pParamHis->sDate[0] != '\0')
				{
					if(pParamHis->sParamValue[0] != '\0' && pParamHis->sParamOrValue[0] != '\0' 
							&& strcmp(pParamHis->sParamValue,pParamHis->sParamOrValue)!=0)
					{
							printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamOrValue);
							printf("%-18s%-18s\n", "δ��ֵ: ", pParamHis->sParamValue);
					}
					else if(pParamHis->sParamValue[0] != '\0' && pParamHis->sParamOrValue[0] == '\0')
					{
		 				printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamValue);
		 			} 
		 			else if(pParamHis->sParamOrValue[0] != '\0')
		 			{
						printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamOrValue);
					}
				}
				else
				{
					if(pParamHis->sParamValue[0] != '\0' && pParamHis->sParamOrValue[0] != '\0' 
							&& strcmp(pParamHis->sParamValue,pParamHis->sParamOrValue)!=0)
					{
							printf("%-18s%-18s\n", "  ԭֵ: ", pParamHis->sParamOrValue);
							printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamValue);
					}
					else if(pParamHis->sParamValue[0] != '\0' && pParamHis->sParamOrValue[0] == '\0')
					{
		 				printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamValue);
		 			} 
		 			else if(pParamHis->sParamOrValue[0] != '\0')
		 			{
						printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamOrValue);
					}					
				}
				break;
		}
		/*
		if(pParamHis->sParamValue[0] != '\0' && pParamHis->sParamOrValue[0] != '\0' 
			&& strcmp(pParamHis->sParamValue,pParamHis->sParamOrValue)!=0)
		{
			if(pParamHis->sDate[0] != '\0')
			{
				printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamOrValue);
				printf("%-18s%-18s\n", "δ��ֵ: ", pParamHis->sParamValue);
				printf("%-18s%-18s\n", "δ����Ч����: ", pParamHis->sDate);
				if(effType[0] != '\0'){
					printf("%-18s%-18s\n", "��Ч��ʽ: ", effType);
				}
				continue;
			} else {
				if(pParamRule && pParamRule->iEffDateFlag == 1)
				{
	 				printf("%-18s%-18s\n", "  ԭֵ: ", pParamHis->sParamOrValue);
	 				printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamValue);
				} else 
				{
					printf("%-18s%-18s\n", "    ��ǰֵ: ", pParamHis->sParamOrValue);
	 				printf("%-18s%-18s\n", "��������ֵ: ", pParamHis->sParamValue);
				}
			}
	 	} else if(pParamHis->sParamValue[0] != '\0' && pParamHis->sParamOrValue[0] == '\0')
		{
	 		printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamValue);
	 	} else if(pParamHis->sParamOrValue[0] != '\0'){
			printf("%-18s%-18s\n", "��ǰֵ: ", pParamHis->sParamOrValue);
		}
		*/
		
		if(pParamHis->sDate[0] != '\0'){
			printf("%-18s%-18s\n", "������Ч����: ", pParamHis->sDate);
		}
		if(effType[0] != '\0'){
			printf("%-18s%-18s\n", "��Ч��ʽ: ", effType);
		}
		if(pParamRule)
		{
			if(pParamRule->sMaxVal[0] != '\0'){
				printf("%-18s%-18s\n","���ֵ: ",pParamRule->sMaxVal);
			}
			if(pParamRule->sMinVal[0] != '\0'){
				printf("%-18s%-18s\n","��Сֵ: ",pParamRule->sMinVal);
			}
			if(pParamRule->sEnumVal[0][0] != '\0'){
				printf("%-18s","ö��ֵ: ");
				for(int i =0;i<pParamRule->iEnumPos;i++)
				{
					if(pParamRule->sEnumVal[i][0] != '\0')
						printf("%s",pParamRule->sEnumVal[i]);
					if(pParamRule->sEnumVal[i+1][0] == '\0'){
						printf("\n");
						break;
					} else 
						printf("%s",", ");
				}
			}
			switch(pParamRule->iValueChange)
			{
				case 1:
					printf("%-18s%-18s\n", "����ֵ����: " ,"�˲���ֻ������");
					break;
				case 2:
					printf("%-18s%-18s\n", "����ֵ����: ","�˲��������޸�");
					break;
				case 3:
					if(pParamRule->sValueChange[0] != '\0'){
						printf("%-18s%-18s\n","����ƥ��Ĺ���:", pParamRule->sValueChange);
					}
					break;
				case 4:
					printf("%-18s%-18s\n", "����ֵ����: ","�˲���ֻ�ܼ�С");
					break;
				case 5:
					printf("%-18s%-18s\n", "����ֵ����: ","�˲���ֻ����ip");
					break;
			}
			if(pParamRule->sDisobey[0] != '\0')
			{
				printf("%-18s%-18s\n","���������ַ�:",pParamRule->sDisobey);
			}
			if(pParamRule->sTime24Start[0] != '\0' && pParamRule->sTime24End[0] != '\0')
			{
				printf("%s%s%s%s","ʱ����Сֵ: ",pParamRule->sTime24Start ," ʱ�����ֵ: ",pParamRule->sTime24End);
				printf("\n");
			}
			if(pParamRule->iParamNeed>0)
			{
				printf("%-18s%-18s\n","����ֵ����: ","�˲����������");
			}
			if(pParamRule->sGroupParamName[0] != '\0')
			{
				printf("%-18s%-18s\n","�˲������ڲ�����:",pParamRule->sGroupParamName);
			}
		} 
	}
}
/*
��������:���Ƹ���ҳ��
����˵��:iPos:��ǰҳ�濪ʼ��ʾ���ݵ�ƫ����(��ǰҳ������ƫ������Ӧ���ݿ�ʼ��ʾ)
����ֵ˵��:1:������ʾ;-1:�����ϸ�ҳ����ʾ;-9:�˳���ʾ
����:showControl(1)
*/
int SysParamClient::showControl(int &iPos)
{
	 char sChoose[32] = {0};
	 printf("Please select: \n"
                   "c: ������ҳ��ʾ | "
                   "b: ������ҳ��ʾ | "
                   "q: �˳�,��������ʾ\n");
	 scanf("%s", &sChoose);
	 if(strcmp(sChoose,"c")==0 || strcmp(sChoose,"C")==0)
	 {
	 	return 1;
	 } else if(strcmp(sChoose,"b")==0 || strcmp(sChoose,"B")==0)
	 {
	 	return -1;
	 } else {
	 	return -9;
	 }
}

/*
��������:�Ӳ�����ȡ����
����˵��:sParamValue����;sParamName��Ż�ȡ��Ķ���
����ֵ˵��:��
����:analyseParamName("SYSTEM.location=beijing",sParamName);sParamName=SYSTEM
*/
void SysParamClient::analyseParamName(char *sParamValue,char *sParamName)
{
	string str(sParamValue);
	int iPos = str.find(".");
	sParamName[0] = '\0';
	if(iPos>0)
	    strncpy(sParamName,sParamValue,iPos*sizeof(char));
	else{
		strcpy(sParamName,sParamValue);
	}
	
}


/*
��������:Ϊmonitor����ĺ���,ȥ���˲���������鹦���Լ�������ʾ����(��Щ���ܷŵ�monitorC�ļ�����)
����˵��:
����ֵ˵��:0�����ݷ���;1-�����ݷ���
����:deal4Monitor("SYSTEM","SYSTEM.location",MSG_PARAM_S_N,pCmdCom)��ȡSYSTEM.location�ֶε�ֵ
*/
int SysParamClient::deal4Monitor(char *sParamName, string sSysParamValue, long lCMDID, CommandCom *pCmdCom)
{
	m_poCmdCom = pCmdCom;
	if(NULL == m_poCmdCom)
			return -1;
	return deal4Monitor(sParamName,sSysParamValue,lCMDID);
}

/*
��������:Ϊmonitor����ĺ���,ȥ���˲���������鹦���Լ�������ʾ����(��Щ���ܷŵ�monitorC�ļ�����)
����˵��:
����ֵ˵��:0�����ݷ���;1-�����ݷ���
����:deal4Monitor("SYSTEM","SYSTEM.location",MSG_PARAM_S_N)��ȡSYSTEM.location�ֶε�ֵ
*/
int SysParamClient::deal4Monitor(char *sParamName, string sSysParamValue, long lCMDID)
{ 
	return dealWithMem(sParamName, (char*)sSysParamValue.c_str(), lCMDID,false);
}

/*
��������:��ȡinitserver����ǰ����Ϣ������մ���
����˵��:
����ֵ˵��:��
����:
*/
void SysParamClient::clearMsg()
{
}

/*
��������:��ȡinitserver���ݺ�����ݸ�ʽת��
����˵��:
����ֵ˵��:ת�����ݸ�ʽ(һ��hbmonitorʹ��)[-1-������ȡʧ��;-2-intserver��ʾ�Ĵ���ʧ��;-3-�õ��ķ��ز�����ʽ����;0-��ָ����ʽ�ķ���;��ֵ�Ǵ�����]
����:
*/
long SysParamClient::VecToSysParamInfo(ParamHisRecord *pParamHisRecord,int &iParamNum,long lCMDID)
{
	if(!pParamHisRecord)
		return -1;
	switch(lCMDID)
	{
		case MSG_PARAM_CHANGE:
		case MSG_PARAM_ACTIVE_MOUDLE_S:
		{
			break;
		}
		case MSG_PARAM_S_ALL:
		case MSG_LIST_SECTION:
		case MSG_PARAM_S_N:
		{
			for(int i = 0;i<m_iCnt;i++)
			{
				strcpy(pParamHisRecord[i].sSectName,m_poSysParamHis[i].sSectionName);
				strcpy(pParamHisRecord[i].sParamName,m_poSysParamHis[i].sParamName);
				strcpy(pParamHisRecord[i].sParamOrValue,m_poSysParamHis[i].sParamOrValue);
				strcpy(pParamHisRecord[i].sParamValue,m_poSysParamHis[i].sParamValue);
				strcpy(pParamHisRecord[i].sDate,m_poSysParamHis[i].sDate);
			}
			iParamNum = m_iCnt;
			return m_iCnt;
		}
		default:
		{
			return 0;
		}
	}
	return 0;
}

/*
��������:��ʾ������Ϣ
����˵��:
����ֵ˵��:��
����:
*/
void SysParamClient::showHowToUseCMD()
{
     cout <<" ******************************************************************" << endl;
     cout <<"  ʹ�÷�ʽ:" << endl;
     cout <<"  abmparam -list <����.������>                    :��ʾ������Ϣ"<< endl;
     cout <<"  abmparam -set ����.������=����ֵ	:�趨����ֵ(=���߲����ո�)"<< endl;
     cout <<"  abmparam -refresh :�����޸ĵĲ���ˢ�£���������Ч" << endl;
	 cout <<" ******************************************************************" << endl;
}


/*
������:paramParser
��������:�������ݵó�initserver��ʶ�������,��initserver����
����˵��:sAllParamVal���û�������,δ����������;strDsc�������������ݸ�����string
����ֵ˵��:true�����ɹ�;false����ʧ��
����:string strDsc; paramParser("SYSTEM.location=beijing,effdate=2011", strDsc)
	strDsc���������"SYSTEM.location=beijing,SYSTEM.location.effdate=2011"
*/
bool SysParamClient::paramParser(char *sAllParamVal,string &strDsc)
{
	 
	 if(sAllParamVal[0] != '\0')
	 {
	 	memset(mRecord,'\0',sizeof(struct ParamHisRecord));	
	 } else {
	 	return false;
	 }
	 
	 string strEffdate;
	 string sPureEff;//��Ч����ֵ
	 string sEffFlag;//��ʶeffdate
	 string sParam;
	 string strAllParam(sAllParamVal);
	 int iParamEffPos = strAllParam.rfind(",");
	 if(iParamEffPos == 0)
	    return false;
	 if(iParamEffPos>0)
	 {
	    strEffdate.assign(strAllParam,iParamEffPos+1,strAllParam.size());
		int iEffPos = strEffdate.find("=");
		if(iEffPos>0){
			sPureEff.assign(strEffdate,iEffPos+1,strEffdate.size());
			sEffFlag.assign(strEffdate,0,iEffPos);
			char sEffDateFlag[32] = {0};
			strncpy(sEffDateFlag,(char*)sEffFlag.c_str(),sizeof(sEffDateFlag));
			//Chang2Lower(sEffDateFlag);
			if( strcmp(sEffDateFlag,EFFDATE) != 0 )
			{
				return false;
			}
		}
	    sParam.assign(strAllParam,0,iParamEffPos);
	 } else {
	    sParam = strAllParam;
	 }
	 int iValPos = sParam.rfind('=');
	 int iParamPos = sParam.find('.');
	 if(iValPos<0 || iParamPos<0)
	 {		
		return false;
	 }
	 string sPureSect(sParam,0,iParamPos);
	 string sPureParam(sParam,iParamPos+1,iValPos-iParamPos-1);
	 string sPureVal(sParam,iValPos+1,sParam.size());
	 trimN(sPureSect);
	 trimN(sPureParam);
	 trimN(sPureVal);
	 trimN(sPureEff);
	 if(sPureSect.size()>0 && sPureParam.size()>0)
	 {
	 	strncpy(mRecord[0].sSectName,(char*)sPureSect.c_str(),sizeof(mRecord[0].sSectName));
	 	strncpy(mRecord[0].sParamName,(char*)sPureParam.c_str(),sizeof(mRecord[0].sParamName));
	 	//Chang2Upper(mRecord[0].sSectName);
	 	//Chang2Lower(mRecord[0].sParamName);
		strDsc+=sPureSect;
		strDsc+=".";
		strDsc+=sPureParam;
	 } else {
	 	return false;
	 }
	 
	 if(sPureVal.size()>0)
	 {
	 	strncpy(mRecord[0].sParamValue,(char*)sPureVal.c_str(),sizeof(mRecord[0].sParamValue));
		if(sPureEff.size()>0)
	 	{
			strDsc += ".";
			strDsc += NEW_VALUE;
		}
		strDsc += "=";
		strDsc+=sPureVal;

	 	if(sPureEff.size()>0)
	 	{
			string strTmp;
			strTmp = "," + sPureSect + "." + sPureParam + "." + EFF_DATE_NAME;
			strDsc += strTmp;
			strDsc += "=";
			strDsc += sPureEff;
			strncpy(mRecord[0].sDate,(char*)sPureEff.c_str(),sizeof(mRecord[0].sDate));
	 	}
	 	//return true;
	 }
	 return true;
}

//MEMORY_DB ����ʾ
bool SysParamClient::prompt(string sysparam,bool show)
{
	 if(!show)
	 	return false;
	 string str = sysparam;
	 trimN(str);
	 int pos = str.find('.');
	 if(pos>0)
	 {
	 	//�������� "MEMORY_DB"
		string sect;
		sect.assign(str,0,pos);
		int posend = str.rfind('=');
		if(posend<=pos)
			return false;
		string param;
		param.assign(str,pos+1,posend-pos-1);
		char *set = new char[sect.size()+1];
		char *aram = new char[param.size()+1];
		if(!set || !aram)
		{
			printf("%s\n","�ڴ治��");
			THROW(1);
		}
		strcpy(set,(char*)sect.c_str());
		strcpy(aram,(char*)param.c_str());
		//Chang2Upper(set);
		//Chang2Lower(aram);
		if(strcmp(set,"MEMORY_DB")==0)
		{
			if(strcmp(aram,"acctitemcount")==0)
			{
				printf("%s\n","���ڴ���ʹ��ctlacctitem����");
			}
			if(strcmp(aram,"acc_data_count")==0)
			{
				printf("%s\n","�ۻ���ʵ����ʹ��ctlaccu ����");
			}
			if(strcmp(aram,"acc_cycle_count")==0)
			{
				printf("%s\n","�ۼ�����ʵ����ʹ��ctlaccu����");
			}
			
		}
		if(strcmp(set,"PRODOFFERAGGR")==0)
		{
			if(strcmp(aram,"totaldata")==0)
			{
				printf("%s\n","�Ż�������ʹ��ctlofferaggr���� ");
			}
			if(strcmp(aram,"detaildata")==0)
			{
				printf("%s\n","�Ż���ϸ��ʹ��ctlofferaggr����");
			}
		}
		delete []set;
		delete []aram;
	 }
	 return true;
}			 

/*2.8�������Ժ��� �Ѳ�ʹ��*/

//2.8���԰汾
int SysParamClient::deal(char *sParamName, string sSysParamValue, long lCMDID, CommandCom *pCmdCom)
{
	 m_poCmdCom = pCmdCom;
	 return deal(sParamName, sSysParamValue, lCMDID);
}

/*
��������: �������������initserver,�õ�������
��������: sSectName��, sSysParamValue ����ֵ(��.����=ֵ,effdate=2011������Ч����), lCMDID����ID, ShowErrInfo�Ƿ���ʾ������Ϣ
����ֵ: ʧ�ܷ���ֵ<0;��ʱ0;��ȡ��ֵΪ1
����: dealWithServer("SYSTEM","SYSTEM.location=beijing",MSG_PARAM_S_N, true)
*/
int SysParamClient::dealWithServer(char *sSectName,char *sSysParamValue,long lCMDID,bool ShowErrInfo)
{
	
	return 0;
}

/*
��������:��ȡinitserver�����ǵ�ʱ��ȴ�����
����˵��:iTimer��ǰ��������ֵ��ʶ, lCMDID����ID, Show�Ƿ���ʾ��ʱ��ʾ
����ֵ˵��:>0���Լ�����ʱ,=0�����Լ�����ʱ
����:waitTimer(iTimer,MSG_PARAM_S_ALL, true)
*/
int SysParamClient::waitTimer(int &iTimer,long lCMDID,bool Show)
{
	if(m_iWaitMax<=0)
		m_iWaitMax = 5;
	if(iTimer == 1)
	{
		if(lCMDID == MSG_PARAM_S_ALL)
			sleep(2);
		else
			sleep(1);
		iTimer++;
	} else {
		if(iTimer<m_iWaitMax){
			usleep(100);
			iTimer++;
		} else {
			if(Show)
				printf("�������ݳ�ʱ!\n");			
			clearPtr();
			return 0;//��ȡ��Ϣʧ�� 
		}	
	}
	return iTimer;
}

/*
������clearPtr
���ܣ������ʱ�������
����������
����:
*/
void SysParamClient::clearPtr()
{
}

/*
������deal(variant sParamName, variant sSysParamValue, long lCMDID,bool Show)
���ܣ�ʵ���¼��ķ�����ӣ�ֻ����һ�Σ����ԽӶ����Ϣ��ֻ����Ϣ��ѹ����
����������sParamName��Ҫ���û�ı�������ļ��еĲ���������ʽ�ǣ���Ŀ¼/�ļ���/����/����
		 sSysParamValue �� ��������ֵ ; lCMDID ���������ı�� ; Show �Ƿ���ʾ������Ϣ
����ֵ:  <0 ��ʾʧ��  >=0�ɹ�
������   deal("ROOT", "ROOT.location=beijing", MSG_PARAM_CHANGE,true) 
*/
int SysParamClient::deal(char *sParamName, string sSysParamValue, long lCMDID,bool Show)
{ 
	string strDsc;
	bool bRes = IfSuitRule(lCMDID,(char*)sSysParamValue.c_str(),strDsc,Show);
		
	if(bRes)
	{
	 	return dealWithServer(sParamName, (char*)strDsc.c_str(), lCMDID, Show);
	} else {
		if(Show)
			printf("����%s�����Чʧ��\n",strDsc.c_str());
		return -1;;
	}
}

/*
��������:�ٲù��ܷ�������(ֻ�ṩ�޸Ĳ���ʹ��)
����˵��:str�û��������Ч����
����ֵ˵��:
����:send2Arbi("SYSTEM.location=beijing")
*/ 
void SysParamClient::send2Arbi(string str)
{
#ifdef _ARBITRATION_
	arbi = false;
	string strTmp = str;
	string strSect;
	string strParam;
	int iPos = strTmp.find(',');
	string first;
	if(iPos>0)
	{
		first.assign(strTmp,0,iPos);
	} else {
		first = strTmp;
	}
	int iSectPos = first.find('=');
	if(iSectPos>0)
	{
		strSect.assign(first,0,iSectPos);
		strParam.assign(first,iSectPos+1,first.size());
	}
	if(strSect.size()>0 && strSect.size()>0)
	{
		if(m_poBaseRuleMgr->neddArbi((char*)strSect.c_str(),(char*)strParam.c_str()tr))
		{				
				vector<string> vecSysValues;
				vecSysValues.clear();
				vecSysValues.push_back(str);
				if(m_iState==0)//����
				{
					int iTemp=m_pArbitrationMgr->sendMulVal(vecSysValues,"vecSysValues");
					if(iTemp == -1)
					{
						//��ʼ������ʧ��
						throw(MBC_Arbitration +4);
					}
					else if( iTemp  != 0)
					{
						//������������ʧ��
						throw(MBC_Arbitration +5);
					}
				}					
		}
	}
#endif
}

/*
��������:�ٲù��ܻ�ȡ����
����˵��:
����ֵ˵��:
����:
*/  
void SysParamClient::recData()
{
#ifdef _ARBITRATION_
	vector<string> vecSysValues;
	vecSysValues.clear();
	if(m_iState==1)//����
	{
		 while(1)
    	 {
    		if (GetInterruptFlag_A())
			{
				break;
			}
			sleep(1);
		 }
		 DenyInterrupt_A();
		 int iTemp=m_pArbitrationMgr->receiveSingleVal(vecSysValues,"vecSysValues");
		 if(iTemp == -1)
		 {
		 	//��ʼ������ʧ��
		 	throw(MBC_Arbitration +4);
		 }
		 else if( iTemp  != 0)
		 {
		 	//������������ʧ��
			throw(MBC_Arbitration +5);
		 }			
		 //ȡ��	vecSysValues,�ٰ�ȡ���Ĳ�����ȡ�����������������
		 for(int i =0;i<vecSysValues.size();i++)
		 {
		 	char sParamName[1024] = {0};
			analyseParamName(vecSysValues[i],sParamName);
		 	deal(sParamName, vecSysValues[i], MSG_PARAM_CHANGE, true);
		 }
	}
#endif
}

// "true","false"
bool SysParamClient::getArbiResult(char *sValue)
{
#ifdef _ARBITRATION_
	if(m_iState ==2)
	{
		//���㲻���ٲ�
		return true;
	}
	else
	{
		if(m_pArbitrationMgr->Audit((char *) sValue) == 1)
		{
			int iTemp=m_pArbitrationMgr->Success();
			if(iTemp == -1)
			{									
				throw(MBC_Arbitration +11);
			}
			//Log::log(0,"%s�ٲóɹ�������",sValue);
			return true;
		}
		else
		{
			//Log::log(0,"%s�ٲ�ʧ�ܣ�����",sValue);
			int iTemp=m_pArbitrationMgr->Fail();
			return false;
		}	
	}					
#endif
	return true;
}

/*
��������:�����û���������,����õ����ս��,��ʾ���û�
����˵��:sArgv, iArgc ��ͬmain��������������
����ֵ˵��:>0�����ɹ�;=0�����������;<0����ʧ��
����:sArgv = {"hbparam","-list","SYSTEM"} iArgc=3
*/ 
int SysParamClient::resolveParam(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc)
{
    return -1;
}

void SysParamClient::setLogLevel()
{
	sprintf(sLogLevel,"%s%d","LOG_LEVEL=",-2);
	putenv(sLogLevel);
}
//�����Ч
int SysParamClient::ruleCheck(long lCMDID,char *m_sSysParamValue)
{
	switch(lCMDID)
	{
		case MSG_PARAM_CHANGE:	
				m_poBaseRuleMgr->setDebugState(m_bMONDebug);			
				if(!m_poBaseRuleMgr->check(m_sSysParamValue)){					
					m_poBaseRuleMgr->paramParseExplain(true);//check֮��Ĳ�������˵��
					return -1;
				}
				break;	
		default:
				break;	
	}
	return 1;
}

char *SysParamClient::getInitParamValue()
{
	return m_sInitParamValue;
}
void SysParamClient::setDebugState(bool _Debug)
{
	 m_bMONDebug = _Debug;
	 if(m_poBaseRuleMgr)
	 	m_poBaseRuleMgr->setDebugState(_Debug);
}

