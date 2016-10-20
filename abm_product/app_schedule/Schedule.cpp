#include "StdAfx.h"
//#include "../app_infomonitor/InfoMonitor.h"
Schedule::Schedule()
{
	memset(m_sRunState,0,sizeof(m_sRunState));
	memset(m_sSysState,0,sizeof(m_sSysState));
	memset(m_sMidSysState,0,sizeof(m_sMidSysState));
	m_iMemPercent = 0;
	m_iCpuPercent = 0;
	if(Channel::m_pLog == NULL)
	{
	  try
	  {
		  Channel::m_pLog = new ThreeLogGroup();
	  }
	  catch (bad_alloc& e)
	  {
		  badalloc(e);
		  exit(-1);
	  }
	  
	   Channel::m_pLog->m_iLogProid = -1;
     Channel::m_pLog->m_iLogAppid = -1;	
	}	
}

Schedule::~Schedule()
{
	if(0!= Channel::m_pCmdCom)
	{
		Channel::m_pCmdCom->Logout();
		delete Channel::m_pCmdCom;
	}
	Channel::m_pCmdCom=0;
	if(Channel::m_pLog)
	{
		delete Channel::m_pLog;
		Channel::m_pLog=NULL;	
	}
	for(int i=0; i<m_vVirtual.size(); i++)
	{
		if(m_vVirtual[i] != NULL)
			delete m_vVirtual[i];
	}
}


/***********************************************
 *	Function Name	:isTimeOut
 *	Description	:ʹ��������
 *	Input Param	:
 *	Returns		:
 *	complete	:
 ***********************************************/

int Schedule::isTimeOut()
{
	sleep(m_iWaitTime);
	return 1;
}
//�����м�̬
char* Schedule::parserState()
{
	Channel::trim(m_sMidSysState);
	char *p = strrchr(m_sMidSysState,'|');
	memset(m_sSysState,0,sizeof(m_sSysState));
	if(p != NULL){
		strncpy(m_sSysState,p+1,14);
	}
	else
	{
		strncpy(m_sSysState,m_sMidSysState,14);
	}
	return p;
}
/***********************************************
 *	Function Name	:refresh
 *	Description	:ˢ������
 *	Input Param	:
 *	Returns		:
 *	complete	:
 ***********************************************/
int Schedule::refresh()
{
	if(0==Channel::m_pCmdCom)
		return 0;
	if(Channel::m_pInfoHead->iRefreshFlag == 2)//��⵽mnt -r
	{
		Channel::m_pInfoHead->iRefreshFlag = 0;
		Log::log(0,"%s","��⵽���ø��£����¼����鴦����");
		//INFOPOINT(1,"%s","��⵽���ø��£����¼����鴦����");
		getCurVirProcessor();
	}
	static char sTemp[100] = {0};
	memset(sTemp,0,sizeof(sTemp));
	
	m_vtSysParamHis.clear();
	//Channel::m_pCmdCom->FlushParam(&m_vtSysParamHis);
	getSysConfigEx();
	//Log::log(0,"ϵͳ:��λ[%d],��תʱ��[%ds],״̬[%s],�Զ�����[%d]",m_iShift,m_iWaitTime,m_sSysState,m_iAutoCtlFlag);
	//INFOPOINT(2,"ϵͳ:��λ[%d],��תʱ��[%ds],״̬[%s],�Զ�����[%d]",m_iShift,m_iWaitTime,m_sSysState,m_iAutoCtlFlag);
	return 0;
}

//�޸�ϵͳ״̬
int Schedule::modifySysState(char * state)
{
	char sCmd[100] = {0};

	sprintf(sCmd,"SYSTEM.state=%s\0",state);

	//variant var;
	//var = sCmd;

	//CmdMsg* pMsg = Channel::m_pCmdCom->CreateNewMsg( MSG_STATE_CHANGE_RETURN,"SYSTEM",var);
	
	//if(!Channel::m_pCmdCom->send(pMsg))
	if(!Channel::m_pCmdCom->changeSectionKey(sCmd))
	{
		Log::log(0,"�޸ĺ��Ĳ�����Ϣ����ʧ��\n");
	  return -1;//��Ϣ����ʧ��
	 }
	sprintf(sCmd,"SYSTEM.mid_state=%s\0",state);
	if(!Channel::m_pCmdCom->changeSectionKey(sCmd))
	{
		Log::log(0,"�޸ĺ��Ĳ�����Ϣ����ʧ��\n");
	  return -1;//��Ϣ����ʧ��
	 }
	 
	 time_t tt = time(NULL);
	 
	 struct tm *p = localtime(&tt);
	 
	 char chargedate[15] = {0};
	
	strftime(chargedate,15,"%Y%m%d%H%M%S",p);
	
	sprintf(sCmd,"SYSTEM.state_date=%s\0",chargedate);
	
	if(!Channel::m_pCmdCom->changeSectionKey(sCmd))
	{
		Log::log(0,"�޸ĺ��Ĳ�����Ϣ����ʧ��\n");
	  return -1;//��Ϣ����ʧ��
	}	 
	return 0;
}
//�޸�CHECKPOINT״̬
int Schedule::modifyCheckPointState(int iState){
	char sCmd[100] = {0};

	sprintf(sCmd,"SYSTEM.checkpoint_state=%d\0",iState);

	//variant var;
	//var = sCmd;

	//CmdMsg* pMsg = Channel::m_pCmdCom->CreateNewMsg( MSG_PARAM_CHANGE,"SYSTEM",var);
	//if(!Channel::m_pCmdCom->send(pMsg))
	if(!Channel::m_pCmdCom->changeSectionKey(sCmd))
	{
		Log::log(0,"�޸ĺ��Ĳ�����Ϣ����ʧ��\n");
	  return -1;//��Ϣ����ʧ��
	 }
	return 0;
}

//ϵͳ����̬���
int Schedule::checkSysRunStateEx()
{
	int iTotal = m_vVirtual.size();
	int iProcessID = 0;
	int bWaringFlag = false;
	for(int i=0; i<iTotal; i++){
		m_vVirtual[i]->refreshEx();
		m_vVirtual[i]->getProcInfoFromGuardMain();
		iProcessID = m_vVirtual[i]->isErrExist();
		if(iProcessID > 0)
		{
			Channel::m_pLog->log(MBC_THREAD_ERROR,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_SYSTEM,-1,NULL,"���� %d �쳣,����",iProcessID);
			modifySysRunState("fault");			
			bWaringFlag = true;
		}
		if(m_vVirtual[i]->isBusy() > 0){
				
			Channel::m_pLog->log(MBC_SYS_BUSY,LOG_LEVEL_WARNING,LOG_CLASS_WARNING,LOG_TYPE_SYSTEM,-1,NULL,"ϵͳ������,ϵͳ��ʱ�䴦�ڸ�ˮλ");
			modifySysRunState("busy");
			bWaringFlag = true;
		}
	}
	
	
	if( GetCpuInfo() && m_iCpuPercent >= m_iCpuHighValue ){
			
			Channel::m_pLog->log(MBC_CPU_OVERLOAD,LOG_LEVEL_WARNING,LOG_CLASS_WARNING,LOG_TYPE_SYSTEM,-1,NULL,"ϵͳ������,CPUʹ����[%d%%]",m_iCpuPercent);
			modifySysRunState("busy");
			bWaringFlag = true;
	} 
	if( GetMemInfo() && m_iMemPercent >= m_iMemHighValue ){			
			 Channel::m_pLog->log(MBC_MEM_SPACE_SMALL,LOG_LEVEL_WARNING,LOG_CLASS_WARNING,LOG_TYPE_SYSTEM,-1,NULL,"ϵͳ������,�ڴ�ʹ����[%d%%]",m_iMemPercent);
			 modifySysRunState("busy");
			bWaringFlag = true;
	} 
	
	if(!bWaringFlag)
	{
	  modifySysRunState("normal");
	}
	
	return 0;
}
//�޸�ϵͳ����״̬
int Schedule::modifySysRunState(char* state)
{
	char sCmd[100] = {0};
	char sTempState[100] = {0};
	memset(sTempState,0,sizeof(sTempState));

	if(!strcmp(state,m_sRunState)){
		return 0;
	}
	sprintf(sCmd,"SYSTEM.run_state=%s\0",state);

	//variant var;
	//var = sCmd;
	//CmdMsg* pMsg = Channel::m_pCmdCom->CreateNewMsg( MSG_PARAM_CHANGE,"SYSTEM",var);
	//MSG_STATE_CHANGE_RETURN
	//CmdMsg* pMsg = Channel::m_pCmdCom->CreateNewMsg( MSG_STATE_CHANGE_RETURN,"SYSTEM",var);
	if(!Channel::m_pCmdCom->changeSectionKey(sCmd)){
		//Log::log(0,"�޸ĺ��Ĳ�����Ϣ����ʧ��\n");
		//ALARMLOG(0,MBC_schedule+25,"%s","�޸ĺ��Ĳ�����Ϣ����ʧ��");
	  return -1;//��Ϣ����ʧ��
	 }
	return 0;
}
//��0�Ž��̷���ע��
int Schedule::registerToInitEx()
{
	try{
		Channel::m_pCmdCom = new CommandCom();
	}catch (bad_alloc& e)
	{
		badalloc(e);
		//ALARMLOG(0,MBC_initserver+1,"%s","����ͨ��ʵ��ʧ��");
		exit(-1);
	}

	//ע��ͻ���
	if(!Channel::m_pCmdCom->InitClient())
	{
		delete Channel::m_pCmdCom; Channel::m_pCmdCom=0;
		//ALARMLOG(0,MBC_initserver+1,"%s","ע��ͻ���ʧ��");
		exit(-1);
	}
  
  /*
	//��ȡĬ������
	Channel::m_pCmdCom->GetInitParam("SYSTEM");
	Channel::m_pCmdCom->GetInitParam("SCHEDULE");
	Channel::m_pCmdCom->GetInitParam("log");
	Channel::m_pCmdCom->GetInitParam("monitor");
	//ԤԼ����ĸ���
	Channel::m_pCmdCom->SubscribeCmd("SYSTEM",MSG_PARAM_CHANGE);
	Channel::m_pCmdCom->SubscribeCmd("SYSTEM",MSG_STATE_CHANGE);
	Channel::m_pCmdCom->SubscribeCmd("SCHEDULE",MSG_PARAM_CHANGE);
	Channel::m_pCmdCom->SubscribeCmd("log",MSG_PARAM_CHANGE);
	Channel::m_pCmdCom->SubscribeCmd("monitor",MSG_PARAM_CHANGE);
	//Channel::m_pCmdCom->SubscribeCmd("SCHEDULE",MSG_PARAM_CHANGE);
	*/

	return 0;

}

//�����鴦��������
int Schedule::getCurVirProcessor()
{
	if(0==Channel::m_pCmdCom)
		return 0;
	char *  pUserName = getenv("LOGNAME");
  if (!strlen(pUserName) ) {
        Log::log (0, "%s","��ȡ����LOGNAME");
        THROW (MBC_IpcKey + 1);
  }
	//�ȶԱ� �ҵ�����VPID
	//DEFINE_QUERY(qry);
	char sSQL[1024]={0};
	int iProcess;
	int iOutProcess;
	vector<int> vVirProc;
	vector<int>::iterator it;
	/*
	sprintf(sSQL,"select distinct a.vp_id from wf_process a where a.vp_id>0 and a.sys_username='%s' order by vp_id",pUserName);
	qry.setSQL(sSQL);
	qry.open();
	while(qry.next())
	{
		vVirProc.push_back(qry.field (0).asInteger());
	}
	qry.close();
	*/
	
	TProcessInfo *pProcInfo = Channel::m_pProcInfoHead;//ָ������׵�ַ
	int iCnt = Channel::m_pInfoHead->iProcNum;
	for(int i=0; i<iCnt; i++)
	{
		if( (Channel::m_pProcInfoHead+i)->iVpID >0 )
		{
			it = find(vVirProc.begin(),vVirProc.end(),(Channel::m_pProcInfoHead+i)->iVpID);
			if(it == vVirProc.end())
				vVirProc.push_back((Channel::m_pProcInfoHead+i)->iVpID);
		}
	}
	
	char sTemp[100]={0};
	int iRet = -1;
	for(int i=0; i<m_vVirtual.size(); i++)
	{
		if(m_vVirtual[i] != NULL)
		{
			delete m_vVirtual[i];
		}
	}
	m_vVirtual.clear();
	//������ǰ״̬��ʵ�ʹ�����鴦����
	int iTempID;
	for(int i=0; i<vVirProc.size(); i++)
	{
		iTempID= vVirProc[i]/1000;
		memset(sTemp,0,100);
		sprintf(sTemp,"vp%d.%s_state",iTempID,m_sSysState);
		iRet = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", sTemp, -1);
		if(iRet != -1)
		{
			VirtualProcessor * vp = new VirtualProcessor(vVirProc[i]);
			m_vVirtual.push_back(vp);
		}
	}

	return 0;

}
//��ȡ���Ĳ���
int Schedule::getSysConfigEx()
{
	if(0==Channel::m_pCmdCom)
		return 0;
	char sTemp[100]={0};
	//��ϵͳ��λ
	m_iShift = Channel::m_pCmdCom->readIniInteger ( "SCHEDULE", "global_shift", 0);	
	if(m_iShift<=0 || m_iShift>=11)
	{
		//ALARMLOG(0,MBC_schedule+3,"�Ƿ��ĵ�λֵ%d��ʹ��Ĭ������10��",m_iShift);
		m_iShift = 10;
	}
	//����ʱ��
	m_iWaitTime= Channel::m_pCmdCom->readIniInteger ( "SCHEDULE", "water_check_interval", 0);	
	//m_iWaitTime= Channel::m_pCmdCom->readIniInteger ( "other", "check_process_time", 0);	
	if(m_iWaitTime <= 0)
	{
		//ALARMLOG(0,MBC_schedule+4,"�Ƿ��Ĳ���ʱ��%d��ʹ��Ĭ������10��",m_iWaitTime);
		m_iWaitTime =2;
	}
	Channel::m_pCmdCom->readIniString("SYSTEM","run_state",m_sRunState,NULL);		
	//ϵͳ״̬
	Channel::m_pCmdCom->readIniString("SYSTEM","mid_state",m_sMidSysState,NULL);	
	parserState();
		


	//�ߵ�ˮ����
	sprintf(sTemp,"%s_auto_schedule",m_sSysState);
	m_iAutoCtlFlag=Channel::m_pCmdCom->readIniInteger ("SCHEDULE", sTemp, -1);
	if(m_iAutoCtlFlag<0)
	{
		//ALARMLOG(0,MBC_schedule+5,"�Ƿ����Զ����ƿ���ֵ%d��ʹ��Ĭ�ϲ���0",m_iAutoCtlFlag);
		m_iAutoCtlFlag = 0;
	}
	//����������ʱ��ֵ
	m_iStartTime = Channel::m_pCmdCom->readIniInteger("SCHEDULE","process_start_time_out",-1);
	if(m_iStartTime < 0){
		//ALARMLOG(0,MBC_schedule+6,"��������ʱ�䳬ʱ��ֵ%d����ȷ��ʹ��Ĭ��ֵ10s",m_iStartTime);
		m_iStartTime = 10;
	}
	//CPU����ֵ
	//m_iCpuHighValue = Channel::m_pCmdCom->readIniInteger("monitor","cpu_warning_value",-1);
	m_iCpuHighValue = Channel::m_pCmdCom->readIniInteger("SYSTEM","cpu_used",-1);
	if(m_iCpuHighValue < 0){
		//ALARMLOG(0,MBC_schedule+7,"CPU���ظ澯��ֵ%d����ȷ��ʹ��Ĭ��ֵ90",m_iCpuHighValue);
		m_iCpuHighValue = 90;
	}
	//MEM����ֵ
	m_iMemHighValue = Channel::m_pCmdCom->readIniInteger("SYSTEM","mem_used",-1);
	if(m_iMemHighValue < 0){
		//ALARMLOG(0,MBC_schedule+8,"�ڴ泬�޸澯��ֵ%d����ȷ��ʹ��Ĭ��ֵ90",m_iMemHighValue);
		m_iMemHighValue = 90;
	}
	//ϵͳ��Ǩ��ʱ��ֵ
	m_iStateTimeOut = Channel::m_pCmdCom->readIniInteger("SCHEDULE","system_state_time_out",-1);
	if(m_iStateTimeOut < 0){
		//ALARMLOG(0,MBC_schedule+9,"ϵͳ��Ǩ��ʱ��ֵ%d����ȷ��ʹ��Ĭ��ֵ1800��",m_iStateTimeOut);
		m_iStateTimeOut = 1800;
	}
	//checkpoint״̬
	m_iCheckPointState = Channel::m_pCmdCom->readIniInteger("SYSTEM","checkpoint_state",-1);
	if(m_iCheckPointState < 0){
		//ALARMLOG(0,MBC_schedule+15,"checkpointֵ%d����ȷ��ʹ��Ĭ��ֵ0",m_iCheckPointState);
		m_iCheckPointState = 0;
	}
	//�澯��־����
	//Log::m_iInfoLevel = Channel::m_pCmdCom->readIniInteger("log","log_level",-1);
	//Log::m_iAlarmLevel = Channel::m_pCmdCom->readIniInteger("log","log_alarm_level",-1);
	//Log::m_iAlarmDbLevel = Channel::m_pCmdCom->readIniInteger("log","log_alarmdb_level",-1);
	
  char cScheduleVp[200];
  char cCmd[100];
  memset(cScheduleVp,0,200);
  memset(cCmd,0,100);
  Channel::m_pCmdCom->readIniString("SCHEDULE","module_list",cScheduleVp,NULL);
  
  int iDiffWarn = Channel::m_pCmdCom->readIniInteger("SCHEDULE","diff_warning_level",50);
  int iSrcMax = 0;
  int iMax = 0;
  int iVpID = 0;
  if(cScheduleVp[0])
  {
  	char *pt = strtok(cScheduleVp," ,\n");
  	
  	iVpID = atoi(pt);
  	sprintf(cCmd,"vp%d.process_high_level",iVpID);
  	iSrcMax = Channel::m_pCmdCom->readIniInteger("SCHEDULE",cCmd,-1);
  	
  	
  	while((pt = strtok(NULL," ,\n")) != NULL)
  	{
  		 iVpID = atoi(pt);
  		 memset(cCmd,0,100);
  	   sprintf(cCmd,"vp%d.process_high_level",iVpID);
  	   iMax = Channel::m_pCmdCom->readIniInteger("SCHEDULE",cCmd,-1);
  	   
  	   if( (iSrcMax - iMax) > iDiffWarn)
  	   {
  	   	 /*Channel::m_pLog->log(MBC_PARAM_ERROR,LOG_LEVEL_WARNING,LOG_TYPE_SYSTEM,
  	   	 "�������%d ����ˮλԶС�������������,�����޸�����ˮλΪ%d",iVpID*1000,iSrcMax);
  	   	 Log::log(0,"�������%d ����ˮλԶС�������������,�����޸�����ˮλΪ%d",iVpID*1000,iSrcMax); */
  	   	 break;
  	   }
  	   iSrcMax = iMax;  	   
  	}
  	
  }
  
}

/*
�ߵ�ˮ���ȣ�CheckPoint��Ӧ��״̬��Ǩ��Ӧ
*/
int Schedule::runEx()
{
	//�����ƶ���־�ļ���
	char sLogFile[256] ;
	memset(sLogFile, 0, sizeof(sLogFile));
	char* p= getenv( "ABM_PRO_DIR" );
	snprintf (sLogFile, sizeof(sLogFile), "%s/log/%s.log", p, "schedule");
	Log::setFileName (sLogFile);
	//Log::setAlarmName (sLogFile);
	
	Log::log(0,"���̵��ȿ�ʼ����");
	//INFOPOINT(1,"%s","���̵��ȿ�ʼ����");
	
	//��0�Ž��̷���ע��
	Log::log(0,"��initserver����ע��");
	//INFOPOINT(1,"%s","��initserver����ע��");
	registerToInitEx();
	
	//���������ػ�����
	Log::log(0,"����GuradMain�ػ�����");
	//INFOPOINT(1,"%s","����GuradMain�ػ�����");
	if(Channel::connectGuardMain() < 0){
		return -1;
	}
	Log::log(0,"��ʼ��ת");
	//��ȡ���Ĳ������̵��ȵ�����
	getSysConfigEx();
	//��ȡ��ǰ״̬��ϵͳ��Ҫ������鴦����
	getCurVirProcessor();
	
	char sLastState[50] = {0};//����ϵͳ�ĵ�ǰ״̬
	int iRet = -1;
	int iType;
	long lRet;
	//strncpy(sLastState,m_sSysState,49);//��һ����������״̬��Ǩ���ȴ���Ǩָ���
	//�ӹ��ж��ź�
	DenyInterrupt();
	while (!GetInterruptFlag())
	{
		
		//ˢ�º��Ĳ���
		refresh(); 
		
		//����Ĭ��ϵͳ״̬���м���ܻᱻ�޸�
		//Channel::setSystemRunState(m_sRunState);
		
		//���״̬��Ǩ
		if(strcmp(sLastState,m_sSysState) !=0 )	//��鵽ϵͳ״̬�����仯
		{
			Log::log(0,"ϵͳ״̬%s���Ա�ǨΪ%s...",sLastState,m_sSysState);
			//INFOPOINT(1,"ϵͳ״̬%s���Ա�ǨΪ%s...",sLastState,m_sSysState);
			/*
			//��ϵͳ״̬Ҫ��maintenance��Ǩ��onlineʱ���ж�infomonitor�Ƿ�����������������Ҫ����
			if(strcmp(sLastState,"maintenance") == 0)
				InfoMonitor::runProcess("infomonitor",NULL);
			else if(strcmp(m_sSysState,"maintenance") == 0)//��ϵͳ��һ������ʱ��Ҫ����infomonitor��д���Ľ���״̬��ʱ��
				InfoMonitor::runProcess("infomonitor",NULL);
			*/
			//״̬��Ǩ
			int iRet = stateTransitionEx();  
			if(iRet < 0)
			{ //��Ǩʧ�ܻ᲻ͣ������Ǩ
				memset(sLastState,0,sizeof(sLastState));
				//modifySysState(m_sSysState);	//��Ǩʧ��Ҳ��Ŀ��̬
				//��Ǩʧ�ܣ�������޸�ϵͳ����״̬
				//Channel::setSystemRunState("fault");
				//ALARMLOG(0,MBC_schedule+1,"ϵͳ״̬%s��Ǩ%sʧ�ܣ�",sLastState,m_sSysState);
				//strcpy(sLastState,m_sSysState); 
			}
			else
			{
				memset(sLastState,0,sizeof(sLastState));
				//��Ǩ�ɹ����޸�ϵͳSYSTE.state=m_sSysState
				modifySysState(m_sSysState);		
				Log::log(0,"ϵͳ״̬��ǨΪ%s�ɹ�\n",m_sSysState); 
				//INFOPOINT(1,"ϵͳ״̬%s��Ǩ%s�ɹ�",sLastState,m_sSysState);
				strncpy(sLastState,m_sSysState,49);
			}
                   continue; 
		}
		
		//�ߵ�ˮ�Զ�����
		iRet = doDynamicScheduleEx();		
		if(iRet == 0){
			//INFOPOINT(2,"%s","�ߵ�ˮ�Զ����ȹر�");
		}
		else if(iRet == -1){
			//�ߵ�ˮ����ʧ�ܣ��޸�����ʱ״̬
			//Channel::setSystemRunState("fault");
		}
		else if(iRet == 1){
			Log::log(0,"��Ҫ���checkpoint����ʱ�ر��Զ�����");
			//INFOPOINT(1,"%s","��Ҫ���checkpoint����ʱ�ر��Զ�����");
		}
		else{
			//INFOPOINT(2,"%s","�ߵ�ˮ�Զ��������");
		}
		
		/*
		//��checkpoint�ļ��
		iRet = checkPoint(); 
		if(iRet < 0){
			//�޸�����ʱ״̬
			//Channel::setSystemRunState("fault");
			//ALARMLOG(0,MBC_schedule+15,"%s","checkpoint׼��ʧ��");
		}
		else if (iRet > 0){
			//�޸ĺ��Ĳ���Ϊ2,��ʾ������checkpoint��׼��
			modifyCheckPointState(2); 
			Log::log(0,"checkpoint׼�����");
			//INFOPOINT(1,"%s","checkpoint׼�����");
		}
		else{
			;
		}*/
		
		//ϵͳ����̬״̬��⣬�����޸�ϵͳ����״̬
		//checkSysRunState(); 
		//�����ж������֮���޸�ϵͳ����ʱ״̬
		//Channel::modifySystemRunState();
		
		//����̬��Ӧ
		//doBackUp();
		
		checkSysRunStateEx();
		//���������˯��
		isTimeOut();
		//INFOPOINT(2,"%s","��ʼ��һ�ֵ���");
	}

	return 0;
}
//�ߵ�ˮ�Զ�����
int Schedule::doDynamicScheduleEx()
{
	//�������Զ�����
	if(!m_iAutoCtlFlag)							
		return 0;
	//��Ҫ��checkpoint ��ʱ"�ر�"�ߵ�ˮ
	if(m_iCheckPointState !=0 ){   
		return 1;
	}

	int iRet = -1;
	int  Flag = 0;								//�������ı�־
	
	for(int i=0; i<m_vVirtual.size(); i++)
	{
		iRet = m_vVirtual[i]->checkVirtualState();
		if(iRet<0){
			//Channel::setSystemRunState("fault");
		}
	}
	//������1���ʱ�䣬�ȴ�����״̬�ı仯
	sleep(1);
	int iSleepCnt = 1;	//��ʱ������
	int iWrongProc;			//������Ľ��̺�
	while(1)
	{
		Flag = 0;					//�ȼ��費Ҫ�����
		for(int j=0; j<m_vVirtual.size(); j++)
		{
			Flag = Flag | m_vVirtual[j]->m_iCheckFlag;//������鴦������Ҫ��⣬Flag��Ϊ1
			
			if(m_vVirtual[j]->m_iCheckFlag == 0)//״̬�ѷ���Ҫ�󣬲���Ҫ��Ǩ
			{
				//INFOPOINT(3,"�鴦����[%d]�����н����Ѵ�Ŀ��̬",m_vVirtual[j]->m_iVirtualId);
				continue;
			}
			//��������־λ
			iRet = m_vVirtual[j]->checkProcState();
			if(iRet > 0){
				iWrongProc = iRet;
				//INFOPOINT(3,"��%d�μ��:�鴦����[%d]�н���[%d]δ��Ŀ��̬",iSleepCnt,m_vVirtual[j]->m_iVirtualId,iRet);
			}
			else{
				//INFOPOINT(3,"�鴦����[%d]�����н����Ѵ�Ŀ��̬",m_vVirtual[j]->m_iVirtualId);
			}
		}
		//״̬3��ȷ�ϣ���ʱ�ж�
		if(1==Flag && iSleepCnt>m_iStartTime*3){
			//ALARMLOG(0,MBC_schedule+11,"����[%d]״̬�ı䳬ʱ",iWrongProc);
			//Channel::setSystemRunState("fault");
			return -1;
		}
		if(0==Flag){
			//INFOPOINT(1,"%s","�����鴦��������״̬���ɹ�");
			break;
		}
		else{
			sleep(1);
			iSleepCnt++;
		}
	}
	return 2;
}
//״̬��Ǩ��Ӧ
int Schedule::stateTransitionEx(){
	Log::log(0,"���ȴ�������Դͷ���鴦����...");
	//INFOPOINT(1,"%s","���ȴ�������Դͷ���鴦����...");
	int iSleepCnt = 1; //��ʱ������
	for(int i=0; i<m_vVirtual.size(); i++)
	{
		m_vVirtual[i]->refreshEx();									//ˢ�²���
		m_vVirtual[i]->getProcInfoFromGuardMain();	//ˢ������
		m_vVirtual[i]->firstCheckProc();						//ͣ����Դͷ ������ֱ�������
		usleep(10000);															//ÿ��һ����ָͣ���Ϣһ�ᣬ��GuardMain����ʱ��forkҵ�����
	}
	int iWrongProc;	//��¼������Ľ��̺�
	int Flag = 0;
	int iRet;
	Log::log(0,"����ˮλ���...");
	//��������ˮλ��⣬�����һ������ͣ���򶼲���ͣ�����Ҫ��ľ�������
	
	//���ģ��ֻ�Է�����Դ�Ľ��̽��м�⣬���ǰ������Դһֱͣ�����������һֱ����Ϣ���Ǿͻ�һֱͣ������
	while(1) 
	{
		Flag = 0;
		for(int i=0; i<m_vVirtual.size(); i++)
		{
			if(m_vVirtual[i]->m_iSrcStopCheckFlag == 1 ) //����Դͷ��ֱ��ͣ�ã�����ˮλ�ļ��
				continue;
				//����鴦�����ܲ���ͣ������m_iCanStop����
			m_vVirtual[i]->checkTask();								//���ν���ˮλ���
			Flag = Flag | m_vVirtual[i]->m_iCanStop;
		}
		if(1==Flag && iSleepCnt>m_iStateTimeOut){ 	//����ˮλʼ���²���
			//ALARMLOG(0,MBC_schedule+12,"%s","����ֹͣ��ʱ��ʼ�������������");
			//Channel::setSystemRunState("fault");
			return -1;
		}
		if(0==Flag){
			break;
		} 
		else{
			sleep(1);
			iSleepCnt++;
		}
	}
	
	//�����ͣ������ȷ������Դͷֹͣ�����ģ��ֻ������Դģ�����
	while(1)
	{
		Flag = 0; //�ȼ��費Ҫ�����
		for(int j=0; j<m_vVirtual.size(); j++)
		{
			if(m_vVirtual[j]->m_iSrcStopCheckFlag != 1 ) //������Ǵ�ֹͣ������Դͷ���Ȳ������
				continue;
				
			Flag = Flag | m_vVirtual[j]->m_iCheckFlag;//������鴦������Ҫ��⣬Flag��Ϊ1
			
			if(m_vVirtual[j]->m_iCheckFlag == 0)//״̬�ѷ���Ҫ�󣬲���Ҫ��Ǩ
			{
				//INFOPOINT(3,"�鴦����[%d]�����н����Ѵ�Ŀ��̬",m_vVirtual[j]->m_iVirtualId);
				continue;
			}
			iRet = m_vVirtual[j]->checkProcState();
			if(iRet > 0)
			{
				iWrongProc = iRet;
				//INFOPOINT(3,"��%d�μ��:�鴦����[%d]�н���[%d]δ��Ŀ��̬",iSleepCnt,m_vVirtual[j]->m_iVirtualId,iRet);
				if(iSleepCnt%10 == 0)
					Log::log(0,"��%d�μ��:�鴦����[%d]�н���[%d]δ��Ŀ��̬",iSleepCnt,m_vVirtual[j]->m_iVirtualId,iRet);
			}
			else
			{
				//INFOPOINT(3,"�鴦����[%d]�����н����Ѵ�Ŀ��̬",m_vVirtual[j]->m_iVirtualId);
			}
		}
		//״̬��Ǩ��ʱ
		if(1==Flag && iSleepCnt>m_iStateTimeOut){											//��ʱ
			//ALARMLOG(0,MBC_schedule+11,"����[%d]״̬�ı䳬ʱ",iWrongProc);
			//Channel::setSystemRunState("fault");
			return -1;
		}
		if(0==Flag){
			sleep(2);
			break;
		}
		else{
			sleep(1);
			iSleepCnt++;
		}
	}
	Log::log(0,"��ʼ���������鴦����...");
	//INFOPOINT(1,"%s","��ʼ���������鴦����...");

	Log::log(0,"���������鴦����ͨ����⣬��ʼ״̬��Ǩ");
	//INFOPOINT(1,"%s","���������鴦����ͨ����⣬��ʼ״̬��Ǩ");
	int iNeedStop = 0;
	for(int i=0; i<m_vVirtual.size(); i++) 			//��ʣ�µĽ��̸�ͣ��ͣ��
	{
		iNeedStop = m_vVirtual[i]->stopLeftProc();						//ͣ����
		if(iNeedStop <= 0) //��Ҫͣ�ò���Ϣһ�£���GuardMainһ��ЪϢ
		 usleep(10000);														//������һ����ʱ�䣬�ȴ�����״̬�ı仯
	}
	//������״̬�仯
	while(1)
	{
		Flag = 0; //�ȼ��費Ҫ�����
		for(int j=0; j<m_vVirtual.size(); j++)
		{
			Flag = Flag | m_vVirtual[j]->m_iCheckFlag;//������鴦������Ҫ��⣬Flag��Ϊ1
			
			if(m_vVirtual[j]->m_iCheckFlag == 0)//״̬�ѷ���Ҫ�󣬲���Ҫ��Ǩ
			{
				//INFOPOINT(3,"�鴦����[%d]�����н����Ѵ�Ŀ��̬",m_vVirtual[j]->m_iVirtualId);
				continue;
			}
				
			iRet = m_vVirtual[j]->checkProcState();
			if(iRet > 0){
				iWrongProc = iRet;
				//INFOPOINT(3,"��%d�μ��:�鴦����[%d]�н���[%d]δ��Ŀ��̬",iSleepCnt,m_vVirtual[j]->m_iVirtualId,iRet);
				if(iSleepCnt%10 == 0)
					Log::log(0,"��%d�μ��:�鴦����[%d]�н���[%d]δ��Ŀ��̬",iSleepCnt,m_vVirtual[j]->m_iVirtualId,iRet);
			}
			else{
				//INFOPOINT(3,"�鴦����[%d]�����н����Ѵ�Ŀ��̬",iSleepCnt,m_vVirtual[j]->m_iVirtualId);
			}
		}
		if(1==Flag && iSleepCnt>m_iStateTimeOut){
			//ALARMLOG(0,MBC_schedule+11,"%s","����[%d]״̬�ı䳬ʱ",iWrongProc);
			//Channel::setSystemRunState("fault");
			return -1;
		}
		if(0==Flag){
			//INFOPOINT(3,"%s","�����鴦������Ǩ�ɹ�");
			break;
		}
		else{
			sleep(1);
			iSleepCnt++;
		}
	}
	return 0;
}
//checkpoint״̬��Ӧ,��ͣ����Դ���ټ������ˮλ����ͣ���ν���
int Schedule::checkPoint(){
	if(m_iCheckPointState != 1)  //����Ҫ��checkpoint
		return 0;
	Log::log(0,"��Ӧcheckpoint,����ֹͣ����Դͷ���鴦����...");
	//INFOPOINT(1,"%s","��Ӧcheckpoint,����ֹͣ����Դͷ���鴦����...");
	int iSleepCnt = 1; //��ʱ������
	for(int i=0; i<m_vVirtual.size(); i++)
	{
		m_vVirtual[i]->refreshEx();									//ˢ�²���
		m_vVirtual[i]->getProcInfoFromGuardMain();	//ˢ������
		m_vVirtual[i]->stopAllSrcProc();						//ֱ��ͣ����Դͷ
		usleep(100000);
	}
	int iWrongProc;	//������Ľ��̺�
	int Flag = 0;
	int iRet;
	while(1) //ȷ������Դͷֹͣ
	{
		Flag = 0; //�ȼ��費Ҫ�����
		for(int j=0; j<m_vVirtual.size(); j++)
		{
			if(m_vVirtual[j]->m_iSrcStopCheckFlag != 1 ) //������Ǵ�ֹͣ������Դͷ���Ȳ������
				continue;
				
			Flag = Flag | m_vVirtual[j]->m_iCheckFlag;//������鴦������Ҫ��⣬Flag��Ϊ1
			
			if(m_vVirtual[j]->m_iCheckFlag == 0)//״̬�ѷ���Ҫ�󣬲���Ҫ��Ǩ
			{
				//INFOPOINT(3,"�鴦����[%d]�����н����Ѵ�Ŀ��̬",m_vVirtual[j]->m_iVirtualId);
				continue;
			}
			iRet = m_vVirtual[j]->checkProcState();
			if(iRet > 0){
				iWrongProc = iRet;
				//INFOPOINT(3,"��%d�μ��:�鴦����[%d]�н���[%d]δ��Ŀ��̬",iSleepCnt,m_vVirtual[j]->m_iVirtualId,iRet);
			}
			else{
				//INFOPOINT(3,"�鴦����[%d]�����н����Ѵ�Ŀ��̬",m_vVirtual[j]->m_iVirtualId);
			}
		}
		if(1==Flag && iSleepCnt>m_iStateTimeOut){											//��ʱ����ôд��ʱ���ȷ��
			//ALARMLOG(0,MBC_schedule+11,"����[%d]״̬�ı䳬ʱ",iWrongProc);
			//Channel::setSystemRunState("fault");
			return -1;
		}
		if(0==Flag){
			break;
		}
		else{
			sleep(1);
			iSleepCnt++;
		}
	}
	Log::log(0,"��Ӧcheckpoint,��ʼ��������鴦����ˮλ...");
	//INFOPOINT(1,"%s","��Ӧcheckpoint,��ʼ��������鴦����ˮλ...");
	while(1) //��������ˮλ���
	{
		Flag = 0;
		for(int i=0; i<m_vVirtual.size(); i++)
		{
			m_vVirtual[i]->checkTask();								//���ν���ˮλ���
			Flag = Flag | m_vVirtual[i]->m_iCanStop;
		}
		if(1==Flag && iSleepCnt>m_iStateTimeOut){ 	//����ˮλʼ���²���
			//ALARMLOG(0,MBC_schedule+12,"%s","����ֹͣ��ʱ��ʼ�������������");
			//Channel::setSystemRunState("fault");
			return -1;
		}
		if(0==Flag){
			break;
		}
		else{
			sleep(1);
			iSleepCnt++;
		}
	}
	return 1;
}

int Schedule::doBackUp(){
	DEFINE_QUERY(qry);
	char sSQL[1024] = {0};
	char sState[20] = {0};
	sprintf(sSQL,"%s","select state from b_backup_state_process where process_id=-1");
	try{
		qry.close();
		qry.setSQL(sSQL);
		qry.open();
		if(qry.next()){
			strncpy(sState,qry.field("state").asString(),19);
		}
		else{
			qry.close();
			return 0;
		}
		qry.close();
	}
	catch(TOCIException& e){
		Log::log(0,"%s","��ȡ����̬״̬ʧ��");
		qry.close();
		return  -1;
	}
	if( !strcmp(sState,"END") || !strcmp(sState,"RUN") ){
		return 0;
	}
	if( !strcmp(sState,"OUT") ){
		moveBackData();//������Ų����
		memset(sSQL,0,sizeof(sSQL));
		sprintf(sSQL,"%s","update b_backup_state_process set state='END',out_date=sysdate where process_id=-1 and state='OUT' ");
	  try{
	  	qry.close();
			qry.setSQL(sSQL);
			qry.execute();
			qry.commit();
	  }
	  catch(TOCIException& e) 
	  {
	  	Log::log(0,"%s","����schedule����̬ʧ��");
	    qry.close();
	    return -1;
	  }
	  qry.close();
	  Log::log(0,"%s","schedule����̬����");
	  return 0;
	}
	if(strcmp(sState,"RDY") != 0)
		return 0;
	int Flag = 0;
	int iSleepCnt = 0;
	for(int i=0; i<m_vVirtual.size(); i++)
	{
		m_vVirtual[i]->refreshEx();									//ˢ�²���
		m_vVirtual[i]->getProcInfoFromGuardMain();	//ˢ������
	}
	while(1) //��������ˮλ���
	{
		Flag = 0;
		for(int i=0; i<m_vVirtual.size(); i++)
		{
			m_vVirtual[i]->checkBackUpTask();								//���ν���ˮλ���
			Flag = Flag | m_vVirtual[i]->m_iCanStop;
		}
		if(1==Flag && iSleepCnt>900){ 	//����ˮλʼ���²���
			//ALARMLOG(0,MBC_schedule+12,"%s","����̬�ȴ�ˮλΪ0��ʱ");
			return -1;
		}
		if(0==Flag){
			memset(sSQL,0,sizeof(sSQL));
			sprintf(sSQL,"%s","update b_backup_state_process set state='RUN',in_date=sysdate where process_id=-1 and state='RDY' ");
		  try{
		  	qry.close();
				qry.setSQL(sSQL);
				qry.execute();
				qry.commit();
		  }
		  catch(TOCIException& e) 
		  {
		  	Log::log(0,"%s","����schedule����̬ʧ��");
		    qry.close();
		    return -1;
		  }
			Log::log(0,"schedule����̬׼���ɹ�");
			break;
		}
		else{
			sleep(1);

			iSleepCnt++;
		}
	}
}
int Schedule::moveBackData(){
	DEFINE_QUERY(qry);
	char sSQL[1024] = {0};
	sprintf(sSQL,"%s","begin insert into b_package_state_merge "
                    "select * from b_package_state_merge_backup; "
                    "delete from b_package_state_merge_backup; "
                    "end;" );
  try{
		qry.setSQL(sSQL);
		qry.execute();
		qry.commit();
  }
  catch(TOCIException& e) 
  {
  	Log::log(0,"%s","ִ������ת��ʱ���ִ���");
    qry.close();
    return -1;
  }
  qry.close();
	return 0;
}

bool Schedule::GetMemInfo()
{
	m_iMemPercent = 0;
	int MemTotal;
	int MemFree;
	char ScanBuff[256]={'\0'};
	char sTmpBuff[128]={'\0'};
	char sTmp[128]={'\0'};
	char sTotalMem[128]={'\0'};
	char sFreeMem[128]={'\0'};
	char *p=NULL;
	FILE * fp=NULL;

	#ifdef DEF_LINUX
	strcpy(ScanBuff," free -m|sed -n '2p'|awk '{print $2}' ");
	if((fp = popen(ScanBuff ,"r")) == 0)
	{
		throw TException("DCCollectFactor::PrepareMemPercent Error");
	}
	if (fgets(sTmpBuff,sizeof(sTmpBuff),fp))
	{
		pclose(fp);
		strcpy(sTotalMem,sTmpBuff);
		
	}
	else
	{
		pclose(fp);
		return false;
	}
	#else
	
	#ifdef DFF_AIX
	strcpy(ScanBuff," vmstat |grep \"System\"|grep -v \"grep\"|awk '{print $4}' ");//������Ϊmem=41984MB
	#endif
	#ifdef DEF_HP
	strcpy(ScanBuff," machinfo|grep \"Memory\"|awk '{print $2}' ");//Memory = 16353 MB (15.969727 GB)
	#endif
	if((fp = popen(ScanBuff ,"r")) == 0)
	{
		throw TException("DCCollectFactor::PrepareMemPercent Error");
	}
	if (fgets(sTmpBuff,sizeof(sTmpBuff),fp))
	{
		pclose(fp);
		#ifdef DEF_AIX
		//strcpy(sTmp,&sTmpBuff[4]);//ȥ��ǰ��λmem=
		p=strstr(sTmpBuff,"=");
		strcpy(sTmp,p+1);
		p=NULL;
		p=index(sTmp,'M');
		strncpy(sTotalMem,sTmp,p-sTmp);//ȥ������λMB
		strcat(sTotalMem,'\0');
		#endif	
		
		#ifdef DEF_HP
		 strcpy(sTotalMem,sTmpBuff);
		#endif	
	}
	else
	{
		pclose(fp);
		return false;
	}
	#endif

	memset(ScanBuff,0x00,sizeof(ScanBuff));

	#ifdef DFF_AIX
	//hpunix vmstat 1 4 |sed -n '3,7p'|awk 'BEGIN{total = 0} {total += $4} END{print total/4/1024}'
	strcpy(ScanBuff," vmstat  |sed -n '7p'|awk '{print $4/256}' ");//��ȡFREE�ڴ棬�Ѿ�ת��ΪMB
	#endif
	#ifdef DEF_HP
	strcpy(ScanBuff," vmstat  |sed -n '3p'|awk '{print $5/256}' ");//��ȡFREE�ڴ棬�Ѿ�ת��ΪMB
	#endif
	#ifdef DEF_LINUX
	strcpy(ScanBuff," vmstat  |sed -n '3p'|awk '{print $4/1024}' ");//��ȡFREE�ڴ棬�Ѿ�ת��ΪMB	
	#endif
	if((fp = popen(ScanBuff ,"r")) == 0)
	{
		return false;
	}
	if (fgets(sFreeMem,sizeof(sFreeMem),fp))
	{
		pclose(fp);	
	}
	else
	{
		pclose(fp);
		return false;
	}
	// //## ����ʹ�ðٷֱȣ���ȷ������
	MemTotal = atoi(sTotalMem);
	if(MemTotal == 0)
	{
		 return false;
	}
	MemFree = atoi(sFreeMem);
	
	m_iMemPercent=((MemTotal-MemFree)*100/MemTotal);
	return true;
}


/******************************************************************************************
*���룺	��
*�����	��
*���أ�	��
*���ã�	��Ա����
*������	׼��CPU����
*******************************************************************************************/
bool Schedule::GetCpuInfo()
{
	m_iCpuPercent = 0;
	FILE *fp=NULL;
	char ScanBuff[256]={'\0'};
	char sCpuPercent[256]={'\0'};
 
	memset(sCpuPercent,0x00,sizeof(sCpuPercent));

	/*
	//ps aux|sed -n '2,$ p' |awk 'BEGIN{total = 0} {total += $3} END{print total}'
	//hpunix vmstat 1 4 |sed -n '3,7p'|awk 'BEGIN{total = 0} {total +=$14} END{print total/4}'
	#ifdef DEF_AIX
	strcpy(ScanBuff," vmstat  |sed -n '7p'|awk '{print $16}' ");
	#endif
	#ifdef DEF_HP
	strcpy(ScanBuff," vmstat  |sed -n '3p'|awk '{print $18}' ");
	#endif
	#ifdef DEF_LINUX
	//strcpy(ScanBuff," vmstat 1 4 |sed -n '3,7p'|awk 'BEGIN{total = 0} {total +=$15} END{print total/4}' ");
  strcpy(ScanBuff," vmstat |sed -n '3p'|awk '{print $15}' ");	
	#endif
	*/

#ifdef DEF_AIX
	strcpy(ScanBuff," vmstat 1 2 |sed -n '8p'|awk '{print $16}' ");
#endif
#ifdef DEF_HP
	strcpy(ScanBuff," vmstat 1 2 |sed -n '4p'|awk '{print $18}' ");
#endif
#ifdef DEF_LINUX
	strcpy(ScanBuff," vmstat 1 2 |sed -n '4p'|awk '{print $15}' "); 
#endif

	if((fp = popen(ScanBuff ,"r")) == 0)
	{
		throw TException("DCCollectFactor::PrepareCpuPercent Error");
	}
	if (fgets(sCpuPercent,sizeof(sCpuPercent),fp))
	{
		pclose(fp);
		m_iCpuPercent=100-atoi(sCpuPercent);
		return true;

	}
	else
	{
		pclose(fp);
		return false;
	}
}
