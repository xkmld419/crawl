#include "ParamMgr.h"
DEFINE_MAIN(SgwParam)
extern int g_iFdFront;
extern int g_iStart;
extern int g_iEnd;
SgwParam::SgwParam()
{
}

SgwParam::~SgwParam()
{
}

int SgwParam::run()//int argc, char* argv[])
{
	ThreeLogGroup TLog;
 

	GroupDroitMgr *pGroupDroitCtl = new GroupDroitMgr();
	char *pChr = pGroupDroitCtl->getUsrName();
	CommandCom *m_poCmdCom = new CommandCom();
	
	if(m_poCmdCom) 
	{	
		if(!m_poCmdCom->InitClient())
		{			
			printf("ע��ʧ��,����abmparam�Լ�initserver!\n");	
			Log::log(0,"%s","��initserverע��ʧ��");	
			return -1;
		}
	} else {
		printf("��ȡ�ռ�ʧ��!\n");	
		Log::log(0,"%s","��ȡ�ռ�ʧ��");	
		return -1;
	}
	m_poCmdCom->setParamUse(true);

	SysParamClient *m_poSysParam = new SysParamClient();
	if(m_poSysParam)
	{   	
		m_poSysParam->init(m_poCmdCom,HBPARAM_WAIT_TIME_NEED);	
	} else {
		printf("��ȡ�ռ�ʧ��!\n");	
		Log::log(0,"%s","��ȡ�ռ�ʧ��");	
		return -1;
	}
	char sArgvTmp[1024] = {0};
	char pTmp[1024] = {0};
	for(int i=0;i<g_argc;i++)
	{

		strcat(sArgvTmp,g_argv[i]);
		strcat(sArgvTmp," ");
		 
	}
	
	char sData[1024] = {0};
	char datatri[128] = {0};
	try
	{
			   	  
		int iRes = m_poSysParam->resolveHBParam(g_argv,g_argc);
		
		if(iRes>0)
		{			
			sprintf(sData,"%s%s%s%s%s\0","�û�[",pChr,"]ִ��[ ",sArgvTmp," ]�����ɹ�");
		} else if(iRes<0){ 	
			sprintf(sData,"%s%s%s%s%s\0","�û�[",pChr,"]ִ��[ ",sArgvTmp," ]�����ʧ��");	
		}else{
			sprintf(sData,"%s%s%s%s%s\0","�û�[",pChr,"]ִ�� ",sArgvTmp," ]��������");
		}
		
		Log::log(0,sData);
	} catch (...)
	{
		sprintf(sData,"%s%s%s%s%s\0","�û�[",pChr,"]ִ�� ",sArgvTmp," ]������쳣����");
		//
		sprintf(pTmp,"%s","������쳣����!");
		TLog.log(MBC_COMMAND_ABMPARAM,LOG_LEVEL_WARNING,LOG_CLASS_INFO,LOG_TYPE_PARAM,-1,pTmp,sData);
		Log::log(0,sData);
	}

	
	return 0;
}
