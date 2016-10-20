#include "SgwParam.h"
DEFINE_COMMAND_MAIN(SgwParam)
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
    /*int iFdFront = 0;
	int srcgc = g_argc;
    if( g_argc>=3 && strcmp("-adm", g_argv[g_argc-2])==0 ){
    
        iFdFront = atoi(g_argv[g_argc-1]);
        //g_argc-=2;
        if(iFdFront<0) iFdFront = 0;
        if( iFdFront){
            if( dup2(iFdFront, STDOUT_FILENO )!= STDOUT_FILENO){
                printf("��ʼ��ǰ̨ͨѶʧ��!\n");
            }else{
                setvbuf(stdout,NULL,_IOLBF,0);
            }
        }
    }*/

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
	//m_poCmdCom->GetInitParam("LOG");
	//m_poCmdCom->SubscribeCmd("LOG",MSG_PARAM_CHANGE);
	SysParamClient *m_poSysParam = new SysParamClient();
	if(m_poSysParam)
	{   	
		if(g_iFdFront)
			m_poSysParam->setFrontInfo(true,g_iStart,g_iEnd,"|");
		else
			m_poSysParam->setFrontInfo(false,g_iStart,g_iEnd,"|");
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
		/*if(i > 1){
			strcat(pTmp,g_argv[i]);
		}*/
		strcat(sArgvTmp,g_argv[i]);
		strcat(sArgvTmp," ");
		 
	}
	
	char sData[1024] = {0};
	char datatri[128] = {0};
	try
	{
			   	  
		int iRes = m_poSysParam->resolveHBParam(g_argv,g_argc);
		/*
		switch(m_poSysParam->m_lSrcCMDID)
		{
			case MSG_PARAM_CHANGE:
			{
				 if(m_poSysParam->m_iCnt == 1)
				 {
				 	sprintf(pTmp,"%s.%s",m_poSysParam->m_poSysParamHis->sSectionName,m_poSysParam->m_poSysParamHis->sParamName);
					sprintf(datatri,"%s%s%s%s"," ԭֵ:",m_poSysParam->m_poSysParamHis->sParamOrValue,"  ��ֵ:",m_poSysParam->m_poSysParamHis->sParamValue);
				 }  else {
				 	sprintf(datatri,"%s","�޸�ʧ��");
				 }
				 break;
			}
			case MSG_PARAM_ACTIVE_MOUDLE_S:
			{
				 if(m_poSysParam->m_iCnt > 0)
				 {				 	
					sprintf(datatri,"%s%d%s","��",m_poSysParam->m_iCnt,"����ˢ��ʧ��!");
				 }  else {
				 	if(m_poSysParam->m_iCntEx > 0)
				 	{
						sprintf(datatri,"%s%d%s","��",m_poSysParam->m_iCntEx,"������ֵ���͸ı�,ˢ����������!");
					} else {
				 		sprintf(datatri,"%s","ˢ����������!");
					}
				 }
				 break;
			}
		}
		*/
		if(iRes>0)
		{			
			sprintf(sData,"%s%s%s%s%s\0","�û�[",pChr,"]ִ��[ ",sArgvTmp," ]�����ɹ�");
		} else if(iRes<0){ 	
			sprintf(sData,"%s%s%s%s%s\0","�û�[",pChr,"]ִ��[ ",sArgvTmp," ]�����ʧ��");	
		}else{
			sprintf(sData,"%s%s%s%s%s\0","�û�[",pChr,"]ִ�� ",sArgvTmp," ]��������");
		}
		//if(strlen(datatri)>0)
		//{
		//	strcat(sData,datatri);
		//}
		/*switch(m_poSysParam->m_lSrcCMDID)
		{
			case MSG_PARAM_ACTIVE_MOUDLE_S:
			{
				if(m_poSysParam->m_iCnt > 0)
				{
					TLog.log(MBC_COMMAND_SGWPARAM,LOG_LEVEL_WARNING,LOG_CLASS_INFO,LOG_TYPE_PARAM,-1,pTmp,sData);
				} else {
					TLog.log(MBC_COMMAND_SGWPARAM,LOG_LEVEL_INFO,LOG_CLASS_INFO,LOG_TYPE_PARAM,-1,pTmp,sData);
				}
				break;
			}
		}*/
		//TLog.log(MBC_COMMAND_SGWPARAM,LOG_LEVEL_INFO,LOG_CLASS_INFO,LOG_TYPE_PARAM,-1,pTmp,sData);
		//if(!g_iFdFront){
		Log::log(0,sData);
	} catch (...)
	{
		sprintf(sData,"%s%s%s%s%s\0","�û�[",pChr,"]ִ�� ",sArgvTmp," ]������쳣����");
		//
		sprintf(pTmp,"%s","������쳣����!");
		TLog.log(MBC_COMMAND_SGWPARAM,LOG_LEVEL_WARNING,LOG_CLASS_INFO,LOG_TYPE_PARAM,-1,pTmp,sData);
		Log::log(0,sData);
	}

	
	return 0;
}
