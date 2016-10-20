#include "mode.h"
#include "Log.h"
#include "mntapi.h"
#include "ParamDefineMgr.h"
#include <sys/wait.h>
#include "ThreeLogGroup.h"
#include "MBC_ABM.h"
//#include "../app_infomonitor/InfoMonitor.h"

/*
����LogFlagΪ��־��𣬶������£�
#define  OPERATIONLOG       1                       //ҵ����̴�����־
#define  PARAMLOG             2                       //����������־
#define  ATTEMPERLOG        3                       //���̵�����־
#define  HBACCESS             4                       //���ݿ��޸���־
#define  SYSTEMLOG           5                      //ϵͳ������־
 
���磺LOGG(ATTEMPERLOG,"--��������--");
*/

int TMode::m_iTimeOut = 30;
bool TMode::m_bInitFlag = false;

TMode::TMode()
{ 
    m_iTimeOut = 0;
    m_pCommandCom = 0; 
};

TMode::~TMode()
{
};

bool TMode::init(int iTimeOut,CommandCom *pCommandCom)
{
	bool bRet = false;
	if( m_bInitFlag )
		return m_bInitFlag;
	if(iTimeOut)
		m_iTimeOut = iTimeOut;
	if(pCommandCom)
		m_pCommandCom = pCommandCom;
//ԤԼ
	m_bInitFlag = bRet;
	return bRet;
};

int TMode::deal(char *sArgv[],int iArgc)
{
	int iRet = 0;
	char sOrgStat[128] = {0};
	char sNowStat[128] = {0};
	char sRetStr[512] = {0};
	int iStateType = 0;
	bool bNeedCheck = false;
	char ch = 0;

	char *pUser = getenv("LOGNAME");
	//LOGG(OPERATIONLOG,"�û�[%s],ִ����� hbmode!",pUser);
	Log::log(0,"�û�[%s],ִ����� abmmode!",pUser);
	//m_oLog.log(MBC_SWITCH_SYSTEM,LOG_LEVEL_INFO,LOG_CLASS_WARNING,LOG_TYPE_SYSTEM,-1,NULL,"�û�[%s],ִ����� abmmode !",
  //	pUser); 		

	if ( iArgc <= 1 )
	{
		showHelp();
		//
		//ALARMLOG28(0,MBC_CLASS_Fatal,167,"%s","Ӧ�ý��̲�������ȷ��ȱ�ٱ�ѡ�");
		Log::log(0,"%s","�������󣺴�����[167]Ӧ�ý��̲�������ȷ��ȱ�ٱ�ѡ�");
		//m_oLog.log(MBC_PARAM_THREAD_LOST_ERROR,LOG_LEVEL_WARNING,LOG_CLASS_WARNING,LOG_TYPE_SYSTEM,-1,NULL,"%s",
	  //	"�������󣺴�����[167]Ӧ�ý��̲�������ȷ��ȱ�ٱ�ѡ�");
		return -1;//���������
	}
	else if( iArgc > 3 )
	{
		//ALARMLOG28(0,MBC_CLASS_Fatal,169,"%s","Ӧ�ý��̲�������ȷ�������ȡֵ��");
		Log::log(0,"%s","�������󣺴�����[169]Ӧ�ý��̲�������ȷ�������ȡֵ��");
		//m_oLog.log(MBC_PARAM_THREAD_VALUE_ERROR,LOG_LEVEL_WARNING,LOG_CLASS_WARNING,LOG_TYPE_SYSTEM,-1,NULL,"%s",
	  //	"�������󣺴�����[169]Ӧ�ý��̲�������ȷ��ȱ�ٱ�ѡ�");		
		return -1;
	}
	//if( strcmp(sArgv[0],"hbmode") != 0 )
	//{
	//	return -1;//���������
	//}
	

	if( strcmp(sArgv[1],"-k") == 0 )
	{
		char sValue[16]={0};
		
		this->getNowSysState(_NET_STATE_,sNowStat);
		if( strcmp(sNowStat,"innet") == 0 )
		{
			printf("ϵͳ��ǰΪinnet״̬���޷��л���offline���Ƿ����л�Ϊoutnet��(Y/N)\n");
			while(1)
			{
				//scanf("%c\n",&ch);
				if(iArgc > 2)
				{
					if( strcmp(sArgv[2],"-y") == 0)
					{
						cout <<"Y"<< endl;
						break;
					}
				}
				cin >> ch;
				if(ch == 'N'||ch == 'n')
				{
					//return 0;
					iRet = -8;
					break;		
				}
				else if(ch != 'Y'&& ch != 'y')
					continue;
				else 
				{
					//cout << endl;
					break;
				}
			//
			}
			if(iRet != -8)
			{
				iStateType = _NET_STATE_;
				strcpy( sOrgStat,"innet" );
				iRet = changeSysState(_NET_STATE_,"innet","outnet");
			}
			//iRet = -8;
		}
		//else
		if(iRet >= 0)
		{
			iStateType = _SYS_STATE_;
			strcpy( sOrgStat,"maintenance" );
			if( (iRet = changeSysState(_SYS_STATE_,"maintenance","offline")) == -1)
			{
				strcpy( sOrgStat,"online" );
				iRet = changeSysState(_SYS_STATE_,"online","offline");
			}
			bNeedCheck = true;
			/*
			ParamDefineMgr::getParam("CHECKPOINT","CHECKPOINT",sValue);
			int iValue = atoi(sValue);
			if(iValue!=0)
			{
		#ifndef __IN_HB_MONITOR__
			cout<<"��ҪCheckPoint������"<<flush;
		#endif
			int iPid=0;
			 if((iPid= fork()==0))
	        	{
	                        execlp("/bin/sh","sh","-c","/opt/opthb/src/app_test/bkeventab.sh",NULL);
				exit(3);
	                }
	                	wait(0);
			}*/
		}
		

	}
	else if( strcmp(sArgv[1],"-ki") == 0 )
	{
		this->getNowSysState(_NET_STATE_,sNowStat);
		if( strcmp(sNowStat,"innet") == 0 )
		{
			iRet = -8;
		}
		else
		{
			iStateType = _SYS_STATE_;

			printf("ϵͳ��ǿ���л�(���ڴ���������)��offline״̬��(Y/N)\n");
			while(1)
			{
				//scanf("%c\n",&ch);
				if(iArgc > 2)
				{
					if( strcmp(sArgv[2],"-y") == 0)
					{
						cout <<"Y"<< endl;
						break;
					}
				}
				cin >> ch;
				if(ch == 'N'||ch == 'n')
					return 0;
				else if(ch != 'Y'&& ch != 'y')
					continue;
				else 
				{
					//cout << endl;
					break;
				}
			}
			


			strcpy( sOrgStat,"maintenance" );
			if( (iRet = changeSysState(_SYS_STATE_,"maintenance","offline",1)) == -1)
			{
				strcpy( sOrgStat,"online" );
				iRet = changeSysState(_SYS_STATE_,"online","offline",1);
			}
			//bNeedCheck = true;
		}
	}
	else if( strcmp(sArgv[1],"-m") == 0 )
	{
		this->getNowSysState(_NET_STATE_,sNowStat);
		this->getNowSysState(_RUN_STATE_,sOrgStat);

		memset( sOrgStat,0,sizeof(sOrgStat) );
		iStateType = _SYS_STATE_;
		strcpy( sOrgStat,"maintenance" );
		iRet = changeSysState(_SYS_STATE_,"maintenance","online");
		//bNeedCheck = true;

	}
	else if( strcmp(sArgv[1],"-s") == 0 )
	{
		this->getNowSysState(_NET_STATE_,sNowStat);
		if( strcmp(sNowStat,"innet") == 0 )
		{
			iRet = -10;
		}
		else
		{
			iStateType = _SYS_STATE_;
			strcpy( sOrgStat,"online" );
			iRet = changeSysState(_SYS_STATE_,"online","maintenance");
		}
		bNeedCheck = true;
	}
	else if( strcmp(sArgv[1],"-u") == 0 )
	{
		iStateType = _SYS_STATE_;
		strcpy( sOrgStat,"online" );
		iRet = changeSysState(_SYS_STATE_,"online","maintenance",1);
	}
	else if( strcmp(sArgv[1],"-r") == 0 )
	{
		getNowSysState(_SYS_STATE_,sNowStat);
		if( strcmp(sNowStat,"offline") == 0 )
		{
			iRet = -7;
		}
		else if( strcmp(sNowStat,"maintenance") == 0)
		{
			iRet = -11;
		}
		else
		{
			iStateType = _NET_STATE_;
			strcpy( sOrgStat,"outnet" );
			iRet = changeSysState(_NET_STATE_,"outnet","innet");
		}
	}
	else if( strcmp(sArgv[1],"-d") == 0 )
	{
		iStateType = _NET_STATE_;
		strcpy( sOrgStat,"innet" );
		iRet = changeSysState(_NET_STATE_,"innet","outnet");
	}
	else if( strcmp(sArgv[1],"-p") == 0 )
	{
		iRet = 1;
	}
	else if( strcmp(sArgv[1],"-n") == 0 )
	{
		if( getProcessFault() == 0 )
			iRet = -9;
		else
		{
			iStateType = _RUN_STATE_;
			if((iRet = getNowSysState(_SYS_STATE_,sNowStat)) == 1)
			{
				strcpy( sOrgStat,"fault" );
				if(strcmp(sNowStat,"online")==0)
					iRet = changeSysState(_RUN_STATE_,"fault","normal",0,1);
				else
					iRet = -1;
			}
		}
	}
	else if( strcmp(sArgv[1],"-N") == 0 )
	{
		iStateType = _RUN_STATE_;
		if((iRet = getNowSysState(_SYS_STATE_,sNowStat)) == 1)
		{
			strcpy( sOrgStat,"fault" );
			if( strcmp(sNowStat,"offline")==0 )
				iRet = changeSysState(_RUN_STATE_,"fault","normal",0,1);
			else
				iRet = -1;
		}
	}
	else if( strcmp(sArgv[1],"-h") == 0 )
	{
		iRet = 1;
		showHelp();
	}
	else if(strcmp(sArgv[1],"-sh")==0)//��ʾ�м�̬
	{
		char sNowStat[128] = {0};
		getNowMidState(_SYS_STATE_,sNowStat);
		printf("ϵͳ��ǰ�м�״̬:[%s]\n",sNowStat);
		memset(sNowStat,0,sizeof(sNowStat));
		getNowMidState(_RUN_STATE_,sNowStat);
		printf("ϵͳ����״̬:[%s]\n",sNowStat);	
		memset(sNowStat,0,sizeof(sNowStat));
		getNowMidState(_NET_STATE_,sNowStat);
		printf("ϵͳ�����м�״̬:[%s]\n",sNowStat);		
		return 1;
	}
	else if(strcmp(sArgv[1],"-ss")==0)//����ϵͳ״̬
	{
		char sNowStat[128] = {0};
		char sMidStat[128] = {0};
		
		if(strcmp(sArgv[2],"o")==0) //online
		{
			strcpy(sMidStat,"SYSTEM.mid_state=online");
			strcpy(sNowStat,"SYSTEM.state=online");
		}
		else if(strcmp(sArgv[2],"f")==0) //offline
		{
			strcpy(sMidStat,"SYSTEM.mid_state=offline");
			strcpy(sNowStat,"SYSTEM.state=offline");			
		}
		else if(strcmp(sArgv[2],"m")==0) //maintenance
		{
			strcpy(sMidStat,"SYSTEM.mid_state=maintenance");
			strcpy(sNowStat,"SYSTEM.state=maintenance");				
		}
		else
		{
			printf("����ʧ�ܣ���\n");
			return -1;
		}		
		m_pCommandCom->changeSectionKey(sMidStat);
		m_pCommandCom->changeSectionKey(sNowStat);
		printf("���óɹ�����\n");
		return 1;
		
	}	
	else if(strcmp(sArgv[1],"-sn")==0)//��������״̬
	{
		char sNowStat[128] = {0};
		char sMidStat[128] = {0};
		
		if(strcmp(sArgv[2],"o")==0) //outnet
		{
			strcpy(sMidStat,"SYSTEM.mid_net_state=outnet");
			strcpy(sNowStat,"SYSTEM.net_state=outnet");
		}
		else if(strcmp(sArgv[2],"i")==0) //innet
		{
			strcpy(sMidStat,"SYSTEM.mid_net_state=innet");
			strcpy(sNowStat,"SYSTEM.net_state=innet");
		}
		else if(strcmp(sArgv[2],"m")==0) //maintenance
		{
			strcpy(sMidStat,"SYSTEM.mid_state=maintenance");
			strcpy(sNowStat,"SYSTEM.state=maintenance");				
		}
		else
		{
			printf("����ʧ�ܣ���\n");
			return -1;
		}
		m_pCommandCom->changeSectionKey(sMidStat);
		m_pCommandCom->changeSectionKey(sNowStat);
		
		printf("���óɹ�����\n");
		return 1;
	}
	else
	{
		
		showHelp();
		//ALARMLOG28(0,MBC_CLASS_Fatal,168,"%s","Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�");
		return -1;//���������
	}
	memset( sNowStat,0,sizeof(sNowStat) );
	getNowSysState(iStateType,sNowStat);
	
	switch ( iRet )
	{
	case 0:
		sprintf(sRetStr,"��ȡ����ԭʼ״̬!\n");
		break;
	case -1:
		sprintf(sRetStr,"ԭ״̬[%s]��ƥ��!\n",sOrgStat);
		break;
	case -2:
		sprintf(sRetStr,"����Ƿ��б�Ľ������޸ģ�Ŀ��״̬����[%s]!\n",sNowStat);
		break;
	case -3:
		sprintf(sRetStr,"״̬���ʧ�ܣ���鿴��־!\n");
		break;
	case -4:
		sprintf(sRetStr,"״̬���������!\n");
		break;
	case -5:
		sprintf(sRetStr,"��Ϣ����ʧ��!\n");
		break;
	case -6:
		sprintf(sRetStr,"ԭ״̬[%s]��Ŀ��״̬һ�£�����Ҫ���\n",sOrgStat);
		break;
	case -7:
		sprintf(sRetStr,"ϵͳΪ[offline]״̬,�޷�ע������[innet]!\n");
		break;
	case -8:
		sprintf(sRetStr,"ϵͳ����[innet],�޷��л���[offline]״̬!\n");
		break;
	case -9:
		sprintf(sRetStr,"ϵͳ����״̬�쳣[fault]���н���״̬�쳣�������޸������ٱ��״̬\n");
		break;
	case -10:
		sprintf(sRetStr,"ϵͳ����[innet],�޷��л���[maintenance]״̬!\n");		
		break;
	case -11:
		sprintf(sRetStr,"ϵͳΪ[maintenance]״̬,�޷�ע������[innet]!\n");
		break;	
	case 1:
		sprintf(sRetStr,"abmmode����ִ�гɹ�!\n");
		break;
	default:
		sprintf(sRetStr,"δ֪����!\n");
		break;
	}

	printf(sRetStr);
	//if(iRet < 1)
		//m_oLog.log(MBC_SGW_UNDOWN_FAIL,LOG_LEVEL_WARNING,LOG_CLASS_WARNING,LOG_TYPE_SYSTEM,-1,NULL,"%s",sRetStr);
	//else
  	//m_oLog.log(MBC_SWITCH_SYSTEM,LOG_LEVEL_INFO,LOG_CLASS_WARNING,LOG_TYPE_SYSTEM,-1,NULL,"%s",sRetStr);
	showSysState();
	//checkpoint
/*
	if( bNeedCheck && iRet == 1 )
	{
		printf("��ʼCheckPoint...!\n");
		m_pCheckPointMgr = new CheckPointMgr();
		if(iRet = m_pCheckPointMgr->touchInfoDown())
			printf("CheckPoint�ɹ�!\n");
		else
			printf("CheckPointʧ��!\n");
		//delete m_pCheckPointMgr;
	}
*/	
	//LOGG(OPERATIONLOG,"�û�[%s],ִ����� hbmode ���!",pUser);
	if(iArgc > 1)
	{
  	//m_oLog.log(MBC_SWITCH_SYSTEM,LOG_LEVEL_INFO,LOG_CLASS_WARNING,LOG_TYPE_SYSTEM,-1,NULL,"�û�[%s],ִ����� abmmode %s���!",
	  	//pUser,sArgv[1]); 				
		Log::log(0,"�û�[%s],ִ����� abmmode %s���!",pUser,sArgv[1]);
	}
	else
	{
  	//m_oLog.log(MBC_SWITCH_SYSTEM,LOG_LEVEL_INFO,LOG_CLASS_WARNING,LOG_TYPE_SYSTEM,-1,NULL,"�û�[%s],ִ����� abmmode ���!",
	  //	pUser); 				
		Log::log(0,"�û�[%s],ִ����� abmmode ���!",pUser);
	}
	//char ShutDown[]=". ./stop.sh";
	char sStatTmp[128]={0};
	getNowSysState(_SYS_STATE_,sStatTmp);
	/*
	if(strcmp(sStatTmp,"offline")==0
		&&(strcmp(sArgv[1],"-k") == 0||strcmp(sArgv[1],"-ki") == 0)
		)//&&(*getenv("STOPALL")=='1'))
	//system(ShutDown);
	{
		//InfoMonitor::stopBin();
		//InfoMonitor::stopSys();
	}
	*/
	return iRet;

};

int TMode::getNowSysState( int iStatType ,char *sNowStat )
{
	//char sSaction[128] = "SYSTEM";
	char sKey[128] = {0};

	if ( iStatType == _SYS_STATE_ )
		strcpy(sKey,"state");
	else if( iStatType == _RUN_STATE_ )
		strcpy( sKey,"run_state" );
	else if( iStatType == _NET_STATE_ )
	strcpy( sKey,"net_state" );
	else
	return 0;
	//���ýӿڻ�ȡ
	m_pCommandCom->readIniString( _HBMODE_SECTION,sKey,sNowStat,"" );

	if( strlen(sNowStat) > 0 )
		return 1;
	else
		return 0;
};


int TMode::getNowMidState( int iStatType ,char *sNowStat )
{
	//char sSaction[128] = "SYSTEM";
	char sKey[128] = {0};

	if ( iStatType == _SYS_STATE_ )
		strcpy(sKey,"mid_state");
	else if( iStatType == _RUN_STATE_ )
		strcpy( sKey,"run_state" );
	else if( iStatType == _NET_STATE_ )
		strcpy( sKey,"mid_net_state" );
	else
	return 0;
	//���ýӿڻ�ȡ
	m_pCommandCom->readIniString( _HBMODE_SECTION,sKey,sNowStat,"" );

	if( strlen(sNowStat) > 0 )
		return 1;
	else
		return 0;
};


int TMode::changeSysState( int iStatType, char *sOrgStat,char *sNewStat,int iFource /*= 0 */,int iFlag /*= 0*/ )
{
	char sNowStat[128] = {0};
	char sMidStat[128] = {0};
	char sKey[128] = {0};
	char strUrl[512] = {0};
	char sTmpVar[128]= {0};
	char sRunStat[128] = {0};
	char sNowMidStat[128] = {0};
	//char sSaction[128] = "SYSTEM";
	if(strcmp(sOrgStat,sNewStat)== 0)
		return -6;//ԭ״̬��Ŀ��״̬һ�£�����Ҫ���
	getNowSysState(iStatType,sNowStat);

	if( strlen( sNowStat ) == 0 )
		return 0;//��ȡ����ϵͳ��ǰ״̬
	if( strcmp( sNowStat,sOrgStat ) != 0 )
		return -1;//ԭ״̬�͵�ǰ״̬��ƥ��


	//���ýӿ�

	if ( iStatType == _SYS_STATE_ )
		strcpy(sKey,"state");
	else if( iStatType == _RUN_STATE_ )
		strcpy( sKey,"run_state" );
	else if( iStatType == _NET_STATE_ )
		strcpy( sKey,"net_state" );
	else
		return -4;//���������
		
		//if(iStatType == _RUN_STATE_ || iStatType == _NET_STATE_ )
		if(iStatType == _RUN_STATE_ )
		{
			sprintf(sTmpVar,"%s.%s=%s",_HBMODE_SECTION,sKey,sNewStat);
			if(m_pCommandCom->changeSectionKey(sTmpVar))
			{
				return 1;
			}
			else
			{
				return -3;
			}
			
		}

	//�޸ĵ�ǰ״̬��sNewstat(���л����м�̬)
	
	if( iFlag  == 0 )
		sprintf( sMidStat,"%d|%s|%s",iFource,sOrgStat,sNewStat );
	else if( iFlag >= 1 )
		sprintf( sMidStat,"%s",sNewStat );
	
	//sprintf( sMidStat,"%s",sNewStat );
	sprintf( sTmpVar,"%s.%s=%s",_HBMODE_SECTION,sKey,sMidStat );
	
	
	sprintf(strUrl,"%s\0",_HBMODE_SECTION);
	//hbmode ֻ�޸�mid_state����ȡstate,�ߵ�ˮ�޸�state����ȡmid_state
	if( iFlag ==0){
		if(iStatType == _SYS_STATE_)
			strcpy(sKey,"mid_state");
		else if(iStatType == _NET_STATE_)
			strcpy(sKey,"mid_net_state");
		sprintf(sTmpVar,"%s.%s=%s",_HBMODE_SECTION,sKey,sMidStat);
		m_pCommandCom->changeSectionKey(sTmpVar);
	}else {
		if(iStatType == _SYS_STATE_)
			strcpy(sKey,"mid_state");
		else if(iStatType == _NET_STATE_)
			strcpy(sKey,"mid_net_state");
		sprintf(sTmpVar,"%s.%s=%s",_HBMODE_SECTION,sKey,sMidStat);
		m_pCommandCom->changeSectionKey(sTmpVar);
	}
	
	#ifndef __IN_HB_MONITOR__
	printf("����״̬����ָ�\n");
	#endif
	
	#ifndef __IN_HB_MONITOR__
	printf("����ָ��ɹ���\n");
	#endif
	//ѭ���ȴ�����̬sNewstatֱ����ʱ
	char* c[3]={"/ ","--","\\ "};
	int i =0;
	while(1)
	{
		int j=i%3;
	#ifndef __IN_HB_MONITOR__
		cout<<"�ȴ�״̬����������......................................."<<c[j]<<flush;
	#endif
		sleep(1);
	#ifndef __IN_HB_MONITOR__		
		cout<<'\r';
	#endif
		
		memset(sNowStat,0,128);
		getNowSysState(iStatType,sNowStat);
		if (strcmp( sNowStat,sNewStat ) == 0)
		{
	#ifndef __IN_HB_MONITOR__		
		cout<<endl;
	#endif			
			if(iStatType == _NET_STATE_)
			{
#ifndef __IN_HB_MONITOR__
				if( strcmp( sNowStat,"outnet" ) == 0 )
					printf("ȥ����ɹ�\n");
				else
					printf("����ɹ�\n");
#endif		
			}
			return 1;
		}
		//���ʧ��
		//���м�̬��Ϊԭ״̬��Ϊ���ʧ�ܡ�
		memset(sNowMidStat,0,128);
		getNowMidState(iStatType,sNowMidStat);
		if(strcmp( sNowStat,sNowMidStat ) == 0)
		{
	#ifndef __IN_HB_MONITOR__		
		cout<<endl;
	#endif			
			if(iStatType == _NET_STATE_)
			{
#ifndef __IN_HB_MONITOR__
				if( strcmp( sNowStat,"outnet" ) == 0 )
					printf("����ʧ��\n");
				else
				{
		char sNowStat[128] = {0};
		char sMidStat[128] = {0};
		
		strcpy(sMidStat,"SYSTEM.mid_net_state=outnet");
		strcpy(sNowStat,"SYSTEM.net_state=outnet");
		
		m_pCommandCom->changeSectionKey(sMidStat);
		m_pCommandCom->changeSectionKey(sNowStat);
							
					printf("ȥ����ʧ��\n");
				}
#endif
			}
			return -3;
		}

		i++;
	}
	return -3;//���ʧ��
};


int TMode::changeSysState( int iStatType, char *sOrgStat,char *sNewStat,bool Ismonitor,int iFource ,int iFlag )
{
		char sNowStat[128] = {0};
	char sMidStat[128] = {0};
	char sKey[128] = {0};
	char strUrl[512] = {0};
	char sTmpVar[128]= {0};
	char sRunStat[128] = {0};
	//char sSaction[128] = "SYSTEM";
	if(strcmp(sOrgStat,sNewStat)== 0)
		return -6;//ԭ״̬��Ŀ��״̬һ�£�����Ҫ���
	getNowSysState(iStatType,sNowStat);

	if( strlen( sNowStat ) == 0 )
		return 0;//��ȡ����ϵͳ��ǰ״̬
	if( strcmp( sNowStat,sOrgStat ) != 0 )
		return -1;//ԭ״̬�͵�ǰ״̬��ƥ��


	//���ýӿ�

	if ( iStatType == _SYS_STATE_ )
		strcpy(sKey,"state");
	else if( iStatType == _RUN_STATE_ )
		strcpy( sKey,"run_state" );
	else if( iStatType == _NET_STATE_ )
		strcpy( sKey,"net_state" );
	else
		return -4;//���������
		
		//if(iStatType == _RUN_STATE_ || iStatType == _NET_STATE_ )
		if(iStatType == _RUN_STATE_ )
		{
			sprintf(sTmpVar,"%s.%s=%s",_HBMODE_SECTION,sKey,sNewStat);
			if(m_pCommandCom->changeSectionKey(sTmpVar))
			{
				return 1;
			}
			else
			{
				return -3;
			}
			
		}

	//�޸ĵ�ǰ״̬��sNewstat(���л����м�̬)
	
	if( iFlag  == 0 )
		sprintf( sMidStat,"%d|%s|%s",iFource,sOrgStat,sNewStat );
	else if( iFlag >= 1 )
		sprintf( sMidStat,"%s",sNewStat );
	
	//sprintf( sMidStat,"%s",sNewStat );
	sprintf( sTmpVar,"%s.%s=%s",_HBMODE_SECTION,sKey,sMidStat );
	
	
	sprintf(strUrl,"%s\0",_HBMODE_SECTION);
	//hbmode ֻ�޸�mid_state����ȡstate,�ߵ�ˮ�޸�state����ȡmid_state
	if( iFlag ==0){
		if(iStatType == _SYS_STATE_)
			strcpy(sKey,"mid_state");
		else if(iStatType == _NET_STATE_)
			strcpy(sKey,"mid_net_state");
		sprintf(sTmpVar,"%s.%s=%s",_HBMODE_SECTION,sKey,sMidStat);
		m_pCommandCom->changeSectionKey(sTmpVar);
	}else {
		if(iStatType == _SYS_STATE_)
			strcpy(sKey,"mid_state");
		else if(iStatType == _NET_STATE_)
			strcpy(sKey,"mid_net_state");
		sprintf(sTmpVar,"%s.%s=%s",_HBMODE_SECTION,sKey,sMidStat);
		m_pCommandCom->changeSectionKey(sTmpVar);
	}
	
	if(!Ismonitor)
	{
	  printf("����״̬����ָ�\n");
  }
	
	//ѭ���ȴ�����̬sNewstatֱ����ʱ
	char* c[3]={"/ ","--","\\ "};
	int i =0;
	while(1)
	{
		int j=i%3;
		if(!Ismonitor)
	  {
		  cout<<"�ȴ�״̬����������......................................."<<c[j]<<flush;
	  }
		sleep(1);
		if(!Ismonitor)
	  {
		  cout<<'\r';
    }
		
		
		getNowSysState(iStatType,sNowStat);
		if (strcmp( sNowStat,sNewStat ) == 0)
		{ 
			if(!Ismonitor)
			{
			  cout<<endl;
			}
			return 1;
		}
		

		i++;
	}
	return -3;//���ʧ��
}
int TMode::showSysState()
{
	char sNowStat[128] = {0};
	getNowSysState(_SYS_STATE_,sNowStat);
	printf("ϵͳ��ǰ״̬:[%s]\n",sNowStat);
	memset(sNowStat,0,sizeof(sNowStat));
	getNowSysState(_RUN_STATE_,sNowStat);
	printf("ϵͳ����״̬:[%s]\n",sNowStat);	
	memset(sNowStat,0,sizeof(sNowStat));
	getNowSysState(_NET_STATE_,sNowStat);
	printf("ϵͳ����״̬:[%s]\n",sNowStat);

	return 1;
};

int TMode::showHelp()
{
	cout <<" ******************************************************************" << endl;
	cout <<"  ʹ�÷�ʽ:" << endl;
	cout <<" -k ִ������shutdown����ϵͳ��maintenance  ��onlineģʽ��Ϊofflineģʽ" << endl;
	cout <<" -ki ִ������shutdown��ǿ��ϵͳ��maintenance  ��onlineģʽ��Ϊofflineģʽ" << endl;
	cout <<" -m ��ϵͳ��maintenanceģʽ��Ϊonlineģʽ" << endl;
	cout <<" -s ִ�У���ǿ�ƣ���onlineģʽ���maintenanceģʽ" << endl;
	cout <<" -u ִ�У�ǿ�ƣ���onlineģʽ���maintenanceģʽ" << endl;
	//cout <<" -r ִ���������������״̬(�Ʒ���ע��)" << endl;
	//cout <<" -d ִ��������״̬�������״̬(�Ʒ���ע��)" << endl;
	cout <<" -p ��ʾ��ǰABM��״̬" << endl;
	//cout <<" -n ��online���ϵ�ϵͳ�����޸��������л�������̬��" << endl;
	//cout <<" -N ��offline���ϵ�ϵͳ�����޸��������л�������̬��" << endl;
	cout <<" -y ��-Ki�����л�Ĭ��ѡ��yes" << endl;

	cout <<" -h �����������Ի��ʹ�ð���" << endl;
	cout <<" ******************************************************************" << endl;
	return 1;
};


int TMode::getProcessFault()
{
	THeadInfo *pInfoHead = NULL;
	TProcessInfo *pProcInfoHead = NULL;
	int i =0;

	DetachShm ();
	if (AttachShm (0, (void **)&pInfoHead) >= 0) {
	    pProcInfoHead = &pInfoHead->ProcInfo;
	} else {
	    return -1;
	}
	
	for(i=0; i<pInfoHead->iProcNum; i++)
	{
		if( (pProcInfoHead + i)->iState == ST_ABORT||(pProcInfoHead + i)->iState == ST_KILLED )
			return 0;
	}
	return 1;
	
}
