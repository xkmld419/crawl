#include "SysParamClient.h"
#include "MBC.h"
#include "CommandCom.h"
#include "GroupDroitMgr.h"
#include "Log.h"
#define	HBPARAM_WAIT_TIME_NEED	100

int main(int argc, char* argv[])
{
    int iFdFront = 0;
    if( argc>=3 && strcmp("-adm", argv[argc-2])==0 ){
    
        iFdFront = atoi(argv[argc-1]);
        argc-=2;
        if(iFdFront<0) iFdFront = 0;
        if( iFdFront){
            if( dup2(iFdFront, STDOUT_FILENO )!= STDOUT_FILENO){
                printf("��ʼ��ǰ̨ͨѶʧ��!\n");
            }else{
                setvbuf(stdout,NULL,_IOLBF,0);
            }
        }
    }

	GroupDroitMgr *pGroupDroitCtl = new GroupDroitMgr();
	char *pChr = pGroupDroitCtl->getUsrName();
	CommandCom *m_poCmdCom = new CommandCom();
	
	if(m_poCmdCom) 
	{	
		if(!m_poCmdCom->InitClient())
		{			
			printf("ע��ʧ��,����hbparam�Լ�initserver!\n");	
			Log::log(0,"%s","��initserverע��ʧ��");	
			return -1;
		}
	} else {
		printf("��ȡ�ռ�ʧ��!\n");	
		Log::log(0,"%s","��ȡ�ռ�ʧ��");	
		return -1;
	}
	//m_poCmdCom->GetInitParam("LOG");
	//m_poCmdCom->SubscribeCmd("LOG",MSG_PARAM_CHANGE);
	SysParamClient *m_poSysParam = new SysParamClient();
	if(m_poSysParam)
	{   	
		m_poSysParam->init(m_poCmdCom,HBPARAM_WAIT_TIME_NEED);	
	} else {
		printf("��ȡ�ռ�ʧ��!\n");	
		Log::log(0,"%s","��ȡ�ռ�ʧ��");	
		return -1;
	}
	char sArgvTmp[2048] = {0};
	for(int i=0;i<argc;i++)
	{
		strcat(sArgvTmp,argv[i]);
		strcat(sArgvTmp," ");
	}
	try
	{
			   	  
		int iRes = m_poSysParam->resolveHBParam(argv,argc);
		char sData[1024] = {0};
		if(iRes>0)
		{			
			sprintf(sData,"%s%s%s%s%s\0","[ADMIN]�û�[",pChr,"]ִ��[ ",sArgvTmp," ]����ɹ�");
		} else if(iRes<0){ 	
			sprintf(sData,"%s%s%s%s%s\0","[ADMIN]�û�[",pChr,"]ִ��[ ",sArgvTmp," ]����ʧ��");
		}else{
			sprintf(sData,"%s%s%s%s%s\0","[ADMIN]�û�[",pChr,"]ִ�� ",sArgvTmp," ]�������");
		}
		printf("%s\n",sData);
		//return 0;
	} catch (...)
	{
		char sData[1024] = {0};
		sprintf(sData,"%s%s%s%s%s\0","[ADMIN]�û�[",pChr,"]ִ�� ",sArgvTmp," ]�����쳣����");
		printf("%s\n",sData);
	}
	if(iFdFront){
        printf("<END>\n");
    }
	
	return 0;
}
