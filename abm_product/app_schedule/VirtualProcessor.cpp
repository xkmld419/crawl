
#include "StdAfx.h"


int VirtualProcessor::g_RunProcNum = 0;
/***********************************************
 *	Function Name	:���캯��
 *	Description	:��ʼ��������
 *	Input Param	:
 *	Returns		:
 *	complete	:
 ***********************************************/
VirtualProcessor::VirtualProcessor(int ID)
{
	m_iVirtualId = ID;	
	m_iChLastState = -1;
	m_iCnt = 0;
	m_iCheckFlag = 0;//Ĭ�ϱ�ʾ����Ҫ��⣬ֻҪ����ͣ���̵Ķ�����Ҫ���
	//m_dLastTime.getCurDate();
	time(&m_iLastTime);
	m_iSrcStopCheckFlag = 0; //�����ʾ����Դͷ�Ƿ�����ͣ���
	m_iCanStop = 1; //�����ʾ�����ܷ�ֹͣ�ı�־����ˮ����ͣ
	m_iForceStop = 0; //��ǿͣ��ʶ 
	m_iBusyFlag = 0;
	m_iBackUpSrc = 0; //Ĭ�ϲ��Ǳ���̬��Դ 
}
/***********************************************
 *	Function Name	:��������
 *	Description	:�ͷ���Դ
 *	Input Param	:
 *	complete	:
 *	Returns		:
 ***********************************************/
VirtualProcessor::~VirtualProcessor()
{
}
/***********************************************
 *	Function Name	:
 *	Description	:��ȡ���鴦���������еĽ���
 *	Input Param	:
 *	complete	:
 *	Returns		:
 ***********************************************/

int VirtualProcessor::getProcInfoFromGuardMain()
{
	m_iCnt = 0;
	bool bHaveOp = false;
	if(m_iVirtualId <= 0)//�鴦����ID����
		return -1;

	for (int i=0; i<Channel::m_pInfoHead->iProcNum; i++)			//�������н���
	{
		if(m_iVirtualId == (Channel::m_pProcInfoHead+i)->iVpID && (Channel::m_pProcInfoHead+i)->iProcessID != -1)		//����
		{
			m_ProcInfoList[m_iCnt].iPos = i;
			m_ProcInfoList[m_iCnt].iOpFlag = 0;
			m_ProcInfoList[m_iCnt].iDestState = -1;
			m_iCnt++;
		}
	}
	
//	//������õ����������С��������������wf_process���������һ����¼
//	if((m_iMaxProc > m_iCnt || m_iMinProc > m_iCnt) && m_iIfInsert)
//	{
//	 int iPos = m_ProcInfoList[m_iCnt-1].iPos;
//	 int iOpTimes = 0;
//	 
//	 if(m_iMaxProc > m_iCnt)
//	 {
//	 	iOpTimes = m_iMaxProc - m_iCnt;
//	 }
//	 else
//	 {
//	 	 iOpTimes = m_iMinProc - m_iCnt;
//	 }
//	 
//	 if((Channel::m_pProcInfoHead+iPos)->iProcessID < 0)
//	 {
//	 	  Log::log(0,"�鴦����%d���ִ������ID%d",m_iVirtualId,(Channel::m_pProcInfoHead+iPos)->iProcessID);
//	 	  return 0;
//	 }
//	 try
//   {
//   	DEFINE_QUERY(qry);
//  
//    char cSql[1024] = {0};
//    
//    char sParam[100] = {0};
//    /*
//    for(int i = 1;i<= iOpTimes;i++)
//    {
//    	memset(cSql,0,1024);
//    	memset(sParam,0,100);
//    	
//     //sprintf(sParam,"'-m %d -p %d'",(Channel::m_pProcInfoHead+iPos)->iAppID,(Channel::m_pProcInfoHead+iPos)->iProcessID+i);
// 		 sprintf(sParam,"''");
//		 sprintf(cSql,"insert into wf_process(process_id,caption,hint,app_id,parameter, \
//		 left,top,width,height,billflow_id,auto_run,exec_path,db_user_id,host_id,state, \
//		 sys_username,vp_id,org_id,PRIORITY_TYPE,RUN_MESSAGE,STOP_MESSAGE,ROLLBACK_MESSAGE,DICT_FILE) \
//		 select %d,caption,hint,app_id,%s, \
//		 left,top,width,height,billflow_id,auto_run,exec_path,db_user_id,host_id,state, \
//		 sys_username,vp_id,org_id,PRIORITY_TYPE,RUN_MESSAGE,STOP_MESSAGE,ROLLBACK_MESSAGE,DICT_FILE \
//		 from wf_process where process_id = %d",
//		 (Channel::m_pProcInfoHead+iPos)->iProcessID+i,sParam,(Channel::m_pProcInfoHead+iPos)->iProcessID);
//				
//		 qry.setSQL(cSql);   
//	  
//		 qry.execute();
//		 qry.commit();
//		 qry.close();
//	   
//	   
//		 if(((Channel::m_pProcInfoHead+iPos)->iMqID) > 0)
//		 {
//		   memset(cSql,0,sizeof(cSql));
//		   sprintf(cSql,"insert into wf_process_mq(process_id,mq_id) values(%d,%d)",(Channel::m_pProcInfoHead+iPos)->iProcessID+i,(Channel::m_pProcInfoHead+iPos)->iMqID);
//		   qry.setSQL(cSql);
//		   qry.execute();
//		   qry.commit();
//		   qry.close();
//
//		   memset(cSql,0,sizeof(cSql));
//		   sprintf(cSql,"insert into wf_distribute_rule (process_id,out_process_id,CONDITION_ID,SEND_MODE) \
//						select %d,out_process_id,CONDITION_ID,SEND_MODE from wf_distribute_rule \
//						where process_id = %d ",
//						(Channel::m_pProcInfoHead+iPos)->iProcessID+i,(Channel::m_pProcInfoHead+iPos)->iProcessID);
//		   qry.setSQL(cSql);
//		   qry.execute();
//		   qry.commit();
//		   qry.close();
//		 }
//    }
//    */
//    for(int i = 1;i<= iOpTimes;i++)
//    {
//    	memset(cSql,0,1024);
//    	memset(sParam,0,100);
//    	
//     //sprintf(sParam,"'-m %d -p %d'",(Channel::m_pProcInfoHead+iPos)->iAppID,(Channel::m_pProcInfoHead+iPos)->iProcessID+i);
// 		 sprintf(sParam,"''");
// 		 
// 		 sprintf(cSql,"delete from wf_process where process_id=%d",(Channel::m_pProcInfoHead+iPos)->iProcessID+i);
// 		 qry.setSQL(cSql);
//	  
//		 qry.execute();
//		 qry.commit();
//		 qry.close();
// 		 
// 		 memset(cSql,0,sizeof(cSql));
//		 sprintf(cSql,"insert into wf_process(process_id,caption,hint,app_id,parameter, \
//		 left,top,width,height,billflow_id,auto_run,exec_path,db_user_id,host_id,state, \
//		 sys_username,vp_id,org_id,PRIORITY_TYPE,RUN_MESSAGE,STOP_MESSAGE,ROLLBACK_MESSAGE,DICT_FILE) \
//		 select %d,caption,hint,app_id,parameter, \
//		 left,top,width,height,billflow_id,auto_run,exec_path,db_user_id,host_id,state, \
//		 sys_username,vp_id,org_id,PRIORITY_TYPE,RUN_MESSAGE,STOP_MESSAGE,ROLLBACK_MESSAGE,DICT_FILE \
//		 from wf_process where process_id = %d",
//		 (Channel::m_pProcInfoHead+iPos)->iProcessID+i,(Channel::m_pProcInfoHead+iPos)->iProcessID);
//		
//		 
//		 qry.setSQL(cSql);
//	  
//		 qry.execute();
//		 qry.commit();
//		 qry.close();
//	   
//	   
//		 if(((Channel::m_pProcInfoHead+iPos)->iMqID) > 0)
//		 {
//		   memset(cSql,0,sizeof(cSql));
//	 		 sprintf(cSql,"delete from wf_process_mq where process_id=%d",(Channel::m_pProcInfoHead+iPos)->iProcessID+i);
//	 		 qry.setSQL(cSql);		  
//			 qry.execute();
//			 qry.commit();
//			 qry.close();
//		   
//		   memset(cSql,0,sizeof(cSql));
//		   sprintf(cSql,"insert into wf_process_mq(process_id,mq_id) values(%d,%d)",(Channel::m_pProcInfoHead+iPos)->iProcessID+i,(Channel::m_pProcInfoHead+iPos)->iMqID);
//		   qry.setSQL(cSql);
//		   qry.execute();
//		   qry.commit();
//		   qry.close();
//		   
//		   sprintf(cSql,"delete from wf_distribute_rule where process_id=%d",(Channel::m_pProcInfoHead+iPos)->iProcessID+i);
//		   qry.setSQL(cSql);
//		   qry.execute();
//		   qry.commit();
//		   qry.close();
//
//		   memset(cSql,0,sizeof(cSql));
//		   sprintf(cSql,"insert into wf_distribute_rule (process_id,out_process_id,CONDITION_ID,SEND_MODE) \
//						select %d,out_process_id,CONDITION_ID,SEND_MODE from wf_distribute_rule \
//						where process_id = %d ",
//						(Channel::m_pProcInfoHead+iPos)->iProcessID+i,(Channel::m_pProcInfoHead+iPos)->iProcessID);
//		   qry.setSQL(cSql);
//		   qry.execute();
//		   qry.commit();
//		   qry.close();
//		 }
//    }
//    //qry.commit();
//    //qry.close();
//    
//	Channel::m_pInfoHead->iRefreshFlag = 1;
//	int iTimes = 5;
//	while (iTimes--)
//	{ 
//	  if (Channel::m_pInfoHead->iRefreshFlag == 2)
//	  { 
//		 return getProcInfoFromGuardMain();  
//	  }
//		sleep(1);
//	} 
//
//	Log::log(0,"�����ڴ�ˢ��ʧ��");  
//	}
//	catch (TOCIException &oe) 
//	{    
//		printf(oe.getErrMsg());  
//		printf(oe.getErrSrc());
//		throw oe;
//	}
//	}
//
//	if((m_iCnt > m_iMaxProc) && m_iIfDelete)
//	{
//	int iPos;
//	int iOpTimes = m_iCnt - m_iMaxProc;
//	DEFINE_QUERY(qry);
//	char cSql[1024];
//	for(int i=0;i<iOpTimes;i++)
//	{
//		iPos = m_ProcInfoList[m_iCnt-1-i].iPos;
//		if((Channel::m_pProcInfoHead+iPos)->iProcessID < 0)
//		{
// 			Log::log(0,"�鴦����%d���ִ������ID%d",m_iVirtualId,(Channel::m_pProcInfoHead+iPos)->iProcessID);
// 			return 0;
//		}
//		if((Channel::m_pProcInfoHead+iPos)->iState == ST_SLEEP)
//		{
//			memset(cSql,0,1024);
//			sprintf(cSql,"delete from wf_process_mq where process_id = %d",
//				(Channel::m_pProcInfoHead+iPos)->iProcessID);
//			qry.setSQL(cSql);
//			qry.execute();
//			qry.commit();
//			qry.close();
//			memset(cSql,0,1024);
//			sprintf(cSql,"delete from wf_process where process_id = %d",
//				(Channel::m_pProcInfoHead+iPos)->iProcessID);
//			qry.setSQL(cSql);
//			qry.execute();
//			qry.commit();
//			qry.close();
//			sprintf(cSql,"delete from wf_distribute_rule where process_id = %d",
//				(Channel::m_pProcInfoHead+iPos)->iProcessID);
//			qry.setSQL(cSql);
//			qry.execute();
//			qry.commit();
//			qry.close();			
//			
//			bHaveOp = true;
//		}	
//	}
//  	
//  	int iTimes = 5;
//  	Channel::m_pInfoHead->iRefreshFlag = 1;
//    while (bHaveOp && iTimes--)
//    { 
//      if (Channel::m_pInfoHead->iRefreshFlag == 2)
//      { 
//          break;
//      }
//       sleep(1);
//    }
//    
//    if(iTimes <= 0)
//    {
//    	Log::log(0,"ɾ������ʱ�������ڴ�ˢ��ʧ��");  
//    }
//    //����ˢ�³ɹ�û�У�������ȡһ��
//    if(bHaveOp)
//    {
//      m_iCnt = 0;
//      for (int i=0; i<Channel::m_pInfoHead->iProcNum; i++)			//�������н���
//	    {
//		     if(m_iVirtualId == (Channel::m_pProcInfoHead+i)->iVpID)		//����
//		     {
//			     m_ProcInfoList[m_iCnt].iPos = i;
//			     m_ProcInfoList[m_iCnt].iOpFlag = 0;
//			     m_ProcInfoList[m_iCnt].iDestState = -1;
//			     m_iCnt++;
//		     }
//	    }	
//	  }  
//  }

	return m_iCnt;
}
/***********************************************
 *	Function Name	:
 *	Description	:��ȡ���鴦����Ӧ������ͣ����
 *	Input Param	:
 *	complete	:
 *	Returns		:
 ***********************************************/
int VirtualProcessor::getVirtualActionEx()
{
	static time_t tNow;
	time(&tNow);
	
	int iState = Channel::calcWater(m_iVirtualId);				//���㱾ͨ��ˮλ
	//int iNextState = Channel::calcNextWater(m_iVirtualId);	//�������ͨ��ˮλ
	
	int iRunProcCnt = getBusyProcNumEx();
	if(iRunProcCnt == 0 && iState!=CHANNEL_EMPTY){  //���û�н��������У�����ˮ�͸���
		return START;
	}
  else if( (iState == CHANNEL_EMPTY) && (tNow-m_iLastTime > m_iDurTimeOut) )	
		return STOP;
	//��ˮ����Ҫͣ
	else if( (iState == CHANNEL_LOW) && (tNow-m_iLastTime > m_iDurTimeOut) )
		return STOP;
	//��ˮ����Ҫ��
	else if( (iState == CHANNEL_HIGH || iState == CHANNEL_FULL) && (tNow-m_iLastTime > m_iDurTimeOut) )
		return START;
	else{
		return NOACTION_NOMAL;
	}
	
	//ȡ����Щ�߼��жϣ��ĳ���ˮ����ûˮ��ͣ
	//����ʱ���ڣ�ͨ��ˮλ״̬�����ı䣬�ؼǿ�ʼʱ��
	/*
	if(iState != m_iChLastState)
	{
		time(&m_iLastTime);
		m_iChLastState = iState; //����״̬
		return NOACTION_STATE;															//ͨ��ˮλ���䣬��������
	}
	
	//�ڲ���ʱ����ˮλû�з����ı�
	if( tNow-m_iLastTime < m_iDurTimeOut)					//��δ�ﵽʱ�䷧ֵ
		return NOACTION_TIME;																//ͨ��ˮλ״̬����ʱ�䲻������������
		
	//ˮλ����ʱ���Ѵ﷧ֵ
	if(  ( (iState==CHANNEL_HIGH) || (iState==CHANNEL_FULL) )  && ( (iNextState!=CHANNEL_HIGH) && (iNextState!=CHANNEL_FULL) ) )		//ͨ����״̬ == ��ˮ && ����ͨ��״̬ != ��ˮ
	{
		//m_dLastTime = curTime; 															//���ܵ�����������һ�����̣�Ӧ�ؼǿ�ʼʱ��
		return START;																				//�������������̶���
	}
	else if( (iState==CHANNEL_LOW) || (iState==CHANNEL_EMPTY) )				//ͨ��״̬ == ��ˮ
	{
		//m_dLastTime = curTime; 															//���ܵ�����ֹͣ��һ�����̣��ؼǿ�ʼʱ��
		return STOP;																				//������ֹͣ���̶���
	}
	else if( iState==CHANNEL_NOMAL )															//ͨ��״̬ == ���� ���޶���
		return NOACTION_NOMAL;
	else
		return NOACTION_NEXT;																//�����������ں�ͨ����ˮλ��ʹ���������½���
	*/
}


//����ST_INIT ST_WAIT_BOOT ST_RUNNING�Ľ��̸�����ͬʱ��ST_INIT ST_WAIT_BOOT�Ľ��̴��ϴ������
int VirtualProcessor::getBusyProcNumEx()
{
	int ret = 0;
	int iPos;
	TProcessInfo *pProcInfo = Channel::m_pProcInfoHead;//ָ������׵�ַ
	for(int i=0; i<m_iCnt; i++)
	{
		iPos = m_ProcInfoList[i].iPos;
		if( ((pProcInfo+iPos)->iState == ST_INIT) || ((pProcInfo+iPos)->iState == ST_WAIT_BOOT) )
		{
			ret++;
			m_ProcInfoList[i].iOpFlag = 1; //����Ԥ�����Ľ��̣�����Ϊ������Ȼ����������Ƿ�ɹ�
		}
		else if( (pProcInfo+iPos)->iState == ST_RUNNING || (pProcInfo+iPos)->iState == ST_DESTROY) //modify on 9/18
		{
			ret++;
		}
	}
	return ret; 
}

// ��ȡһ�����еĽ��̣�Ҫ��û�д����ı��
int VirtualProcessor::getOneBusyProcEx(){
	int iPos;
	for(int i=m_iCnt-1; i>=0; i--){ //���Ų�ѯ�����ں������ͣ
		iPos = m_ProcInfoList[i].iPos;
		if( m_ProcInfoList[i].iOpFlag==0 && ((Channel::m_pProcInfoHead+iPos)->iState==ST_RUNNING || (Channel::m_pProcInfoHead+iPos)->iState==ST_INIT || (Channel::m_pProcInfoHead+iPos)->iState==ST_WAIT_BOOT) ){
			return i;//���ؽ��̵�λ��
		}
	}
	return -1;
}
// ��ȡһ�����еĽ��̣�Ҫ��û�д����ı�ǣ�����ABORT�Ľ����ٿ�����
int VirtualProcessor::getOneIdleProcEx(){
	int iPos;
	for(int i=0; i<m_iCnt; i++){ //���Ų�ѯ�����ڽ��̺�С������
		iPos = m_ProcInfoList[i].iPos;
		if( m_ProcInfoList[i].iOpFlag==0 && ((Channel::m_pProcInfoHead+iPos)->iState==ST_SLEEP || (Channel::m_pProcInfoHead+iPos)->iState==ST_KILLED )){
			return i;//���ؽ��̵�λ��
		}
		else if(m_ProcInfoList[i].iOpFlag==0 && (Channel::m_pProcInfoHead+iPos)->iState==ST_ABORT && m_iAbort>0){
			return i;
		}
	}
	return -1;
}
//ֹͣһ���������еĽ��̣�ֻ�Ǹı���״̬�ͱ�ǣ������ɹ�ʧ�ܵ��ж�
int VirtualProcessor::stopOneProcEx(){
	int	iIndex = getOneBusyProcEx();
	if(iIndex < 0)
		return -1;
	int iPos = m_ProcInfoList[iIndex].iPos;
	(Channel::m_pProcInfoHead+iPos)->iState = ST_WAIT_DOWN; //�޸���״̬��ʹGuardMain��֪
	m_ProcInfoList[iIndex].iDestState = ST_SLEEP; //ָ����Ŀ��״̬
	m_ProcInfoList[iIndex].iOpFlag = 1; //���ϱ�־ ��ʾ��Ҫ���
	m_iCheckFlag = 1;//�н���״̬�ı䣬���鴦������Ҫ���
	g_RunProcNum--;
	Log::log(0,"�鴦����%d����ֹͣ����%d",m_iVirtualId,(Channel::m_pProcInfoHead+iPos)->iProcessID);
	return 0;
}
//����һ���Ѿ���ֹ�Ľ��̣�ֻ�Ǹı���״̬�ͱ�ǣ������ɹ�ʧ�ܵ��ж�
int VirtualProcessor::startOneProcEx(){
	if(g_RunProcNum >= m_iSysMaxProcCnt){
		   
		  Channel::m_pLog->log(MBC_MAX_SYSTEM_PROCESS,LOG_LEVEL_WARNING,LOG_TYPE_SYSTEM,"�ﵽϵͳ����������");
		return -1;
	}
	int	iIndex = getOneIdleProcEx();
	if(iIndex < 0){
		Log::log(0,"***�澯***���鴦����[%d]���޽��̿��ã�����",m_iVirtualId);
		////ALARMLOG(0,MBC_schedule+27,"�鴦����[%d]���޽��̿��ã�����",m_iVirtualId);
		return -1;
	}
	int iPos = m_ProcInfoList[iIndex].iPos;
	(Channel::m_pProcInfoHead+iPos)->iState = ST_WAIT_BOOT; //�޸���״̬��ʹGuardMain��֪
	m_ProcInfoList[iIndex].iDestState = ST_RUNNING; //ָ����Ŀ��״̬
	m_ProcInfoList[iIndex].iOpFlag = 1; //���ϱ�־ �´μ��
	m_iCheckFlag = 1;
	g_RunProcNum++;
	Log::log(0,"�鴦����%d������������%d",m_iVirtualId,(Channel::m_pProcInfoHead+iPos)->iProcessID);
	return 0;
}
//����Ƿ񵽴�Ŀ��̬����m_iCheckFlagֵ���̲������������Ǹ����̵�ԭ��ͣ������
int VirtualProcessor::checkProcState(){
	int iPos;

	for(int i=0; i<m_iCnt; i++)
	{
		m_iCheckFlag = 0;              //�ȼ��費��Ҫ�����
		iPos = m_ProcInfoList[i].iPos; //�������λ��
		if( (m_ProcInfoList[i].iOpFlag==1) && ((Channel::m_pProcInfoHead+iPos)->iState != m_ProcInfoList[i].iDestState) )
			{			
				if( ( (Channel::m_pProcInfoHead+iPos)->iState==ST_ABORT || 
					  ( Channel::m_pProcInfoHead+iPos)->iState==ST_KILLED || 
						( Channel::m_pProcInfoHead+iPos)->iState==ST_SLEEP )  )
			 { //���⴦�� modify on 20100930 ������ͨ����� �´α��� �����Ǩ����ʧ��
				 m_ProcInfoList[i].iOpFlag = 0;	
				 m_ProcInfoList[i].iCheckCnt = 0;
				 //Channel::setSystemRunState("fault");
				 //ALARMLOG(0,MBC_schedule+14,"����%d״̬�쳣",(Channel::m_pProcInfoHead+iPos)->iProcessID);
				 continue;
			 }
			 m_iCheckFlag = 1;
			 return (Channel::m_pProcInfoHead+iPos)->iProcessID;// �˽���û�е���Ŀ��̬
		}
		else if(m_ProcInfoList[i].iOpFlag==1 )
		{//�Ѵ�Ŀ��̬������ҪҪ���ȷ��
			m_ProcInfoList[i].iCheckCnt++;
		}

		if(m_ProcInfoList[i].iCheckCnt > 1)
		{//��Ȼ��ȷ������Ŀ��̬�������������ı�־����������0
			m_ProcInfoList[i].iOpFlag = 0;	
			m_ProcInfoList[i].iCheckCnt = 0;
			/*
			if( ST_RUNNING == (Channel::m_pProcInfoHead+iPos)->iState ) //��������������У�������һ���������еĽ��̸���
				g_RunProcNum++;
			else
				g_RunProcNum--;
				*/
		}
		
		m_iCheckFlag = m_iCheckFlag | m_ProcInfoList[i].iOpFlag; //ֻҪ��һ������Ҫ��⣬�鴦��������Ҫ���
	}
	return 0;
}
int VirtualProcessor::isBusy(){
	return m_iBusyFlag;
}
/***********************************************
 *	Function Name	:
 *	Description	:�������������ã���Ӧ�ߵ�ˮ
 *	Input Param	:��λ
 *	Returns		:
 *	complete	:
 ***********************************************/
int VirtualProcessor::refreshEx()
{
	static char sTemp[100] = {0};
	memset(sTemp,0,100);
	int iTempVirtualId = m_iVirtualId/1000;
	sprintf(sTemp,"vp%d.min_process",iTempVirtualId);
	m_iMinProc = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", sTemp, -1);
	if(m_iMinProc<0){
		Log::log(0,"***�澯***���鴦����[%d]�Ƿ�����С����������%d��ʹ��Ĭ��ֵ0",m_iVirtualId,m_iMinProc);
		////ALARMLOG(0,MBC_schedule+16,"�鴦����[%d]�Ƿ�����С����������%d��ʹ��Ĭ��ֵ0",m_iVirtualId,m_iMinProc);
		m_iMinProc = 1;  
	}
	memset(sTemp,0,100);
	sprintf(sTemp,"vp%d.max_process",iTempVirtualId);
	m_iMaxProc = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", sTemp, -1);
	if(m_iMaxProc < 0){
		Log::log(0,"***�澯***���鴦����[%d]�Ƿ���������������%d��ʹ��Ĭ��ֵ0",m_iVirtualId,m_iMaxProc);
		////ALARMLOG(0,MBC_schedule+17,"�鴦����[%d]�Ƿ���������������%d��ʹ��Ĭ��ֵ0",m_iVirtualId,m_iMaxProc);
		m_iMaxProc = 1;
	}

  m_iIfDelete = 1;
  m_iIfDelete = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", "change_delete", 1);
  	
  m_iIfInsert = 1;
  m_iIfInsert = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", "change_insert", 1);
	memset(sTemp,0,100);
	sprintf(sTemp,"vp%d.shake_time",iTempVirtualId);
	m_iDurTimeOut = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", sTemp, -1);
	if(m_iDurTimeOut<0){
		Log::log(0,"***�澯***���鴦����[%d]�Ƿ��ķ�����ʱ�����Ϊ%ds��ʹ��Ĭ��ֵ60s",m_iVirtualId,m_iDurTimeOut);
		////ALARMLOG(0,MBC_schedule+18,"�鴦����[%d]�Ƿ��ķ�����ʱ�����Ϊ%ds��ʹ��Ĭ��ֵ60s",m_iVirtualId,m_iDurTimeOut);
		m_iDurTimeOut = 60;
	}
	memset(sTemp,0,100);
	sprintf(sTemp,"vp%d.hwm_switch",iTempVirtualId);
	m_iAutoSchedule = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", sTemp, -1);
	if(m_iAutoSchedule<0){
		////ALARMLOG(0,MBC_schedule+19,"�鴦����[%d]�Ƿ����Զ����Ȳ���Ϊ%ds��Ĭ�ϲ��������",m_iVirtualId,m_iAutoSchedule);
		m_iAutoSchedule = 0;
	}
	m_iAbort = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", "force_start_abort", -1);
	if(m_iAbort < 0){
		Log::log(0,"***�澯***��ϵͳǿ����ͣABORT���̵Ŀ���%d����ȷ��ʹ��Ĭ��ֵ0",m_iAbort);
		////ALARMLOG(0,MBC_schedule+19,"ϵͳǿ����ͣABORT���̵Ŀ���%d����ȷ��ʹ��Ĭ��ֵ0",m_iAbort);
		m_iAbort = 0;
	}
	m_iShift = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", "global_shift", -1);
	if(m_iShift < 0){
		Log::log(0,"***�澯***��ϵͳ��λ%d����ȷ��ʹ��Ĭ��ֵ10",m_iShift);
		////ALARMLOG(0,MBC_schedule+20,"ϵͳ��λ%d����ȷ��ʹ��Ĭ��ֵ10",m_iShift);
		m_iShift = 10;
	}
	
	m_iBusyTimeOut = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", "schedule_busy_time", -1);
	if(m_iBusyTimeOut < 0){
		Log::log(0,"***�澯***��������ʱ�䷧ֵ����ȷ��ʹ��Ĭ��ֵ10����",m_iShift);
		////ALARMLOG(0,MBC_schedule+21,"������ʱ�䷧ֵ����ȷ��ʹ��Ĭ��ֵ10����",m_iBusyTimeOut);
		m_iBusyTimeOut = 600;
	}
	
	m_iSysMaxProcCnt = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", "max_process_num", -1);
	if(m_iSysMaxProcCnt < 0){
		Log::log(0,"***�澯***��ϵͳҵ��������������ȷ��ʹ��Ĭ��ֵ200��",m_iShift);
		////ALARMLOG(0,MBC_schedule+22,"ϵͳҵ��������������ȷ��ʹ��Ĭ��ֵ400��",m_iSysMaxProcCnt);
		m_iSysMaxProcCnt = 400;
	}
	
	if(m_iMaxProc < m_iMinProc){
		Log::log(0,"***�澯***���鴦����[%d]����������С����С��������ǿ�Ƹı���С������Ϊ��������!",m_iVirtualId);
		////ALARMLOG(0,MBC_schedule+23,"�鴦����[%d]����������С����С��������ǿ�Ƹı���С������Ϊ��������!",m_iVirtualId);
		m_iMinProc = m_iMaxProc;
		sprintf(sTemp,"SCHEDULE.vp%d.min_process=%d",iTempVirtualId,m_iMinProc);
		Channel::m_pCmdCom->changeSectionKey(sTemp);
	}
	
	memset(sTemp,0,100);
	static char sMidSysState[100] = {0};//�п�����ϵͳ�м�̬
	static char sSysState[100] = {0};
	memset(sMidSysState,0,sizeof(sMidSysState));
	memset(sSysState,0,sizeof(sSysState));
	Channel::m_pCmdCom->readIniString("SYSTEM","mid_state",sMidSysState,NULL);	
	Channel::trim(sMidSysState);
	char *p = strrchr(sMidSysState,'|');//���Ų�
	if(p != NULL){
		strncpy(sSysState,p+1,99);
	}
	else{
		strncpy(sSysState,sMidSysState,99);
	}
	p = strchr(sMidSysState,'|');//���Ų�
	if(p != NULL){
		memset(sTemp,0,sizeof(sTemp));
		sTemp[0] = sMidSysState[0];
		m_iForceStop = atoi(sTemp);//��ȡǿ��ɱ���̵ı��
	}
	else{
		m_iForceStop = 0;
	}
	sprintf(sTemp,"vp%d.%s_state",iTempVirtualId,sSysState);
	m_iDestState = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", sTemp, -1);
	if(m_iDestState < 0){
		//Log::log(0,"***�澯***���鴦����[%d]û�����ø�ϵͳ״̬�µ�Ŀ��̬�������ı�",m_iVirtualId);
		////ALARMLOG(0,MBC_schedule+24,"�鴦����[%d]û�����ø�ϵͳ״̬�µ�Ŀ��̬�������ı�",m_iVirtualId);
		m_iDestState = m_iVirtualState;
	}

	m_iMaxShiftProc = m_iMaxProc*m_iShift/10;		//��λ�½��������
	if(m_iMaxShiftProc==0 && m_iShift!=0)				//���ٱ���1��
		m_iMaxShiftProc = 1;
	if(m_iMaxShiftProc < m_iMinProc)					//�ı���С������
		m_iMinProc = m_iMaxShiftProc;
	else if(m_iMaxShiftProc > m_iMaxProc)			//�ⶥֵ������������
		m_iMaxShiftProc = m_iMaxProc;
}
/***********************************************
 *	Function Name	:
 *	Description	:�ж��Ƿ�������Դͷ
 *	Input Param	:
 *	Returns		:
 *	complete	:
 ***********************************************/
int VirtualProcessor::isDataSrc()
{
	if(m_iCnt <= 0)
		return 0;
	int iPos = m_ProcInfoList[0].iPos;
	TProcessInfo* pProcInfo=Channel::m_pProcInfoHead+iPos;
	//��Ϣ�շ�����
	if( pProcInfo->iAppID == 1 )
	{
		return 1;
	} 
	return 0;
}
int VirtualProcessor::isBackUpSrc()
{
	if(m_iCnt <= 0)
		return 0;
	int iPos = m_ProcInfoList[0].iPos;
	TProcessInfo* pProcInfo=Channel::m_pProcInfoHead+iPos;
	// ��ʽ�����ɼ����ļ���У�飬��¼��У��,���,�͸߶�,�����ļ�����,���������,loadparaminfo,tdtrans,tygroup
	if( pProcInfo->iAppID==2 || pProcInfo->iAppID==18 || 
		pProcInfo->iAppID==20 || pProcInfo->iAppID==34 || 
		pProcInfo->iAppID==91 || pProcInfo->iAppID==114 || 
		pProcInfo->iAppID==777777 || pProcInfo->iAppID==30 || 
		pProcInfo->iAppID==300)
	{
		return 1;
	} 
	return 0;
}
int VirtualProcessor::checkVirtualState(){
	
 

	refreshEx();																//���¶�ȡ��������
	
	if(m_iAutoSchedule <= 0)										//����������Զ����ȣ�������
			return 0;
			
	getProcInfoFromGuardMain();									//���»�ȡ������Ϣ
	
	if(m_iMaxShiftProc > m_iCnt){								//������ʵ�ʿ��õĽ�����,�������ã����ر���
		m_iMaxShiftProc = m_iCnt;
	}
	
	int iNum = 0;
	int iRet = -1;
	iNum = getBusyProcNumEx()-m_iMaxShiftProc;		//��λ�ı�Ǩ�������и����������ã���Ҫͣ����

	for(int i=0; i<iNum; i++)
	{
		stopOneProcEx();
	}

	//����С�Ľ�����
	//Log::log(0,"�鴦����[%d]��ʼ�����С������...",m_iVirtualId);
	if(startMinProcEx()<0)
		return -1;

	//�����ߵ�ˮ�ı�Ǩ
	//Log::log(0,"�鴦����[%d]��ʼ������ͨ��ˮλ...",m_iVirtualId);
	int action = getVirtualActionEx();
	//static Date curTime;
	static time_t sNow;
	int iBlockFlag = 0;
	//Channel::setSystemRunState("normal");//����Ϊϵͳ״̬����
	m_iBusyFlag = 0;
	switch(action)
	{
		case START:
			if(getBusyProcNumEx() < m_iMaxShiftProc)
			{
				//INFOPOINT(1,"�鴦����[%d]��������һ������",m_iVirtualId);
				Log::log(0,"�鴦����[%d]��������һ������",m_iVirtualId);
		 Channel::m_pLog->log(MBC_ACTION_SCHEDULE_HIGH,LOG_LEVEL_INFO,LOG_TYPE_PROC,"������ˮλ��������");
				iRet = startOneProcEx();
				if(iRet >= 0){
					time(&m_iLastTime);
				}
			}
			else
			{
				
				time(&sNow);
				if(sNow-m_iLastTime > m_iBusyTimeOut){
					//Channel::setSystemRunState("busy"); 
					iBlockFlag = 1; //��Ϊ��ѹ
					m_iBusyFlag = 1;//������
					Log::log(0,"�鴦����[%d]������",m_iVirtualId);
					//ALARMLOG(0,MBC_schedule+25,"�鴦����[%d]������",m_iVirtualId);
				}
				
			}
			break;
		case STOP:
			if(getBusyProcNumEx() > m_iMinProc)				
			{
				//INFOPOINT(1,"�鴦����[%d]����ֹͣһ������",m_iVirtualId);
				Log::log(0,"�鴦����[%d]����ֹͣһ������",m_iVirtualId);
				
		  	Channel::m_pLog->log(MBC_ACTION_SCHEDULE_LOW,LOG_LEVEL_INFO,LOG_TYPE_PROC,"������ˮλֹͣ����");
				iRet = stopOneProcEx();
				if(iRet >= 0){
					time(&m_iLastTime);
				}
			}
			else
			{
				//INFOPOINT(2,"�鴦����[%d]�Ѵ���С������,��������",m_iVirtualId);
			}
			break;
		/*
		case NOACTION_STATE:
			//INFOPOINT(2,"�鴦����[%d]��ͨ����ˮλ���䣬��������",m_iVirtualId);
			break;
		case NOACTION_TIME:
			//INFOPOINT(2,"�鴦����[%d]��ͨ��ˮλ״̬����ʱ��δ�ﵽ��ֵ����������",m_iVirtualId);
			break;
		case NOACTION_NEXT:
			//INFOPOINT(2,"�鴦����[%d]��ͨ���ͺ���ͨ��ͬʱ��ˮ����������",m_iVirtualId);
			break;
		case NOACTION_NOMAL:
			//INFOPOINT(2,"�鴦����[%d]��ͨ����ˮλ��������������",m_iVirtualId);
			break;
		*/
		default:
			break;	
	}
	/* ���������ò��������Ͳ���ʾ��������
	if(m_iCnt > 0){
		int iPos = m_ProcInfoList[0].iPos;
		(Channel::m_pProcInfoHead+iPos)->iBlockFlag = iBlockFlag; //�޸Ļ�ѹ��ʶ���õ�һ�����̴����鴦������
	}
	*/
	return 0;
}

/***********************************************
 *	Function Name	:startMinProc
 *	Description	:������С�������޸�
 *	Input Param	:
 *	Returns		:
 *	complete	:
 ***********************************************/
int VirtualProcessor::startMinProcEx()
{
	int iRunCnt = getBusyProcNumEx();					//��һ����ʵ�������У���������Ԥ��Ľ���
	int iNeedProc = m_iMinProc - iRunCnt;			//��������Ľ��̸���
	int iRet = 0;
	for(int i=0; i<iNeedProc; i++)
	{
		if(startOneProcEx()<0)
			iRet = -1;
	}
	return iRet;
}


//��һ�μ�⣬�����Ҫ���ֱ����Ҫͣ���������
int VirtualProcessor::firstCheckProc()
{
	
	int state;
	int iRetState = getVirtualState(state);//�ȵõ��鴦����Ŀǰ��״̬
	if(iRetState < -1){
		return -2;
	}
	if(  m_iDestState==MODULE_RUNING  && \
		(state==MODULE_IDLE || state==MODULE_RUNING || state==MODULE_FULL) )//��ǰ״̬����Ŀ��̬
	{
		return 0;
	}
	if( m_iDestState==MODULE_STOP && \
		(state==MODULE_ABORT || state==MODULE_STOP) )//��ǰ״̬����Ŀ��̬
	{
		return 0;
	}
	if( m_iDestState == MODULE_MAINTAINANCE)//���ֲ���ķ�ʽ
	{
		return 0;
	}
	int iRet = 0;
	switch(m_iDestState)
	{
		case MODULE_RUNING:	//Ŀ��̬����Ļ�����ֱ����
			iRet = startMinProcEx();
			break;
		case MODULE_STOP:
			if(m_iForceStop>0)								//�����ǿ��ͣ����ֱ���޸�ͣ״̬
			{
				forceStopProc();
			}
			else if(isDataSrc() > 0)
			{									//���������Դͷ����ֱ��ͣ
				stopAllSrcProc();
				m_iSrcStopCheckFlag = 1;        //Դͷ�����
			}
			else if(state == MODULE_IDLE) 		//������������ÿ�ͣ���
				m_iCanStop = 0; 								//����ͣ
			else
				m_iCanStop = 1; 								//������ͣ
			break;
		default:
			break;
	}
	return iRet;
}
//ͣ����Դͷ�����н���
int VirtualProcessor::stopAllSrcProc(){
	if(isDataSrc() > 0)
	{
		m_iSrcStopCheckFlag = 1;
	}
	else
	{
		m_iSrcStopCheckFlag = 0; //�����������Դ��ֱ�ӷ���
		return 0;
	}
	int iPos;
	int iPid;
	for(int iIndex=0; iIndex<m_iCnt; iIndex++)
	{
		 
	  iPos = m_ProcInfoList[iIndex].iPos;
		iPid = (Channel::m_pProcInfoHead+iPos)->iSysPID;
		/*
		if ( (Channel::m_pProcInfoHead+iPos)->iState == ST_INIT || \
			(Channel::m_pProcInfoHead+iPos)->iState == ST_RUNNING || \
			(Channel::m_pProcInfoHead+iPos)->iState == ST_WAIT_BOOT )
		{
			(Channel::m_pProcInfoHead+iPos)->iState = ST_WAIT_DOWN; //�޸���״̬��ʹGuardMain��֪
			m_ProcInfoList[iIndex].iDestState = ST_SLEEP; //ָ����Ŀ��״̬
			m_ProcInfoList[iIndex].iOpFlag = 1; //���ϱ�־ �´μ��
			m_iCheckFlag = 1;//�����鴦������Ҫ���
			g_RunProcNum--;
		}
		*/
		
			if(iPid > 0 && kill(iPid,0)==0 )
		    kill(iPid,SIGUSR1);
	}
	return 0;
}
int VirtualProcessor::stopLeftProc(){
	if( m_iDestState!=MODULE_STOP )
		return 1;
	if(m_iForceStop>0){
		forceStopProc();
		return 0;
	}
	int iPos;
	for(int iIndex=0; iIndex<m_iCnt; iIndex++)
	{
		int iPos = m_ProcInfoList[iIndex].iPos;
		if ( (Channel::m_pProcInfoHead+iPos)->iState == ST_INIT || \
			(Channel::m_pProcInfoHead+iPos)->iState == ST_RUNNING || \
			(Channel::m_pProcInfoHead+iPos)->iState == ST_WAIT_BOOT )
		{
			(Channel::m_pProcInfoHead+iPos)->iState = ST_WAIT_DOWN; //�޸���״̬��ʹGuardMain��֪
			m_ProcInfoList[iIndex].iDestState = ST_SLEEP; //ָ����Ŀ��״̬
			m_ProcInfoList[iIndex].iOpFlag = 1; //���ϱ�־ �´μ��
			m_iCheckFlag = 1;
			g_RunProcNum--; 
		}
	}
	return 0;
}
int VirtualProcessor::getAllProcDetailNum(int& boot,int& init,int& run,int& kill,int& destroy,int& abort,int& sleep)
{
	boot = 0;
	init = 0;
	run = 0;
	kill = 0;
	destroy = 0;
	sleep = 0;
	abort = 0;
	int iPos;
	TProcessInfo* pProcInfo = NULL;
	for(int i=0; i<m_iCnt; i++)
	{
		int iPos = m_ProcInfoList[i].iPos;
		pProcInfo = Channel::m_pProcInfoHead+iPos;
		switch(pProcInfo->iState)
		{
			case ST_INIT:
				init++;
				break;
			case ST_WAIT_BOOT:
				boot++;
				break;
			case ST_RUNNING:
				run++;
				break;
			case ST_SLEEP:
				sleep++;
				break;
			case ST_KILLED:
				kill++;
				break;
			case ST_ABORT:
				abort++;
				break;
			case ST_DESTROY:
				destroy++;
				break;
			default:
				break;
		}
	}
	return init+boot+run+sleep+kill+destroy+abort;
}
int VirtualProcessor::getVirtualState(int& state)
{
	int boot = 0;
	int init = 0;
	int run = 0;
	int kill = 0;
	int destroy = 0;
	int sleep = 0;
	int abort = 0;
	
	int iProcNum = getAllProcDetailNum(boot,init,run,kill,destroy,abort,sleep);
	
	int iBusyNum = boot + init + destroy + run;
	int iIdleNum = kill + sleep  + abort;
	
	int iState = Channel::calcWater(m_iVirtualId);
	if(iState < 0)
	{
		return -1;
	}
	if( (abort+kill) == m_iCnt) //������еĽ��̶������⣬�ɷ���
	{
		m_iVirtualState = MODULE_ABORT;
		state = m_iVirtualState;
		return iState;
	}
	if(sleep == m_iCnt)
	{
		m_iVirtualState = MODULE_STOP;
		state = m_iVirtualState;
		return iState;
	}
	if( (iState==CHANNEL_EMPTY) && (iBusyNum>0) )//ͨ�����������н���������
	{
		m_iVirtualState = MODULE_IDLE;
		state = m_iVirtualState;
		return iState;
	}
	if((run==m_iMaxShiftProc) && iBusyNum>0) //�ﵽ��λ������������æ
	{
		m_iVirtualState = MODULE_FULL;
		state = m_iVirtualState;
		return iState;
	}
	if(iBusyNum > 0 && abort==0 && kill==0)//
	{
		m_iVirtualState = MODULE_RUNING;
		state = m_iVirtualState;
		return iState;
	}
	if(iBusyNum>0 && (abort>0 || kill>0))
	{
		m_iVirtualState = MODULE_PROC_RUN_ABORT;
		state = m_iVirtualState;
		return iState;
	}
	if(iBusyNum==0 && (abort>0 || kill>0) )
	{
		m_iVirtualState = MODULE_PROC_STOP_ABORT;
		state = m_iVirtualState;
		return iState;
	}
}
int VirtualProcessor::checkTask()
{
	if(m_iForceStop>0 || m_iDestState==MODULE_IDLE || m_iDestState==MODULE_RUNING || m_iDestState==MODULE_FULL || m_iDestState==MODULE_MAINTAINANCE)
	{
		m_iCanStop  = 0;				//�������ģ����߱��ֲ��䣬����˵��ǿɱ�����Դ��ϡ�����ͣ���ı��
		return 0;
	}
	int iState;
	getVirtualState(iState); //��ȡ�鴦������״̬
	if(iState==MODULE_IDLE)
	{
		m_iCanStop  = 0;						//�鴦�������У�����ͣ
	}
	else if(iState==MODULE_ABORT || iState==MODULE_STOP || iState==MODULE_PROC_STOP_ABORT )
	{
		//m_iCanStop  = 0;						//��Ȼ�����񣬵����鴦�����Ѿ����ˣ�Ӧ�������鴦������ʹ��������ͨ���������ο��ܻ��ס
		if(isDataSrc() > 0)
		{ //��������Դͷ��������Ҳû��ϵ������ֱ��ͣ
			m_iCanStop = 0; //�������������ͣ��
			return 0;
		}
		int iTaskNum = Channel::calcWater(m_iVirtualId); //����һ���鴦������ˮλ
		if(iTaskNum != 0){ //��������Ҫ����
			m_iCanStop = 1; //������ͣ
			//Log::log(0,"�鴦����[%d]ǿ���������̴����������...",m_iVirtualId); 
			if(startOneProcEx()<0) //�����鴦�����������ͨ,�������ʧ�ܣ�û�취��,ֻ����Ϊ����ͣ�ˣ��������ǰ����̶�ס��ֻ���˹�������
				m_iCanStop = 0; 
		  checkProcState(); //����Ƿ��ֵ�BUG������Ϊ��ʹ���̲�����ʶ�ܹ�������
		}
		else{
			m_iCanStop = 0; //û��������Ҫ��������ͣ��
		}
	}
	else
	{
		m_iCanStop = 1;							//����ͣ��Ҫ�����������
	}
	return 0;
}
int VirtualProcessor::checkBackUpTask()
{
	if(isBackUpSrc() > 0){
		m_iCanStop = 0;
		return 0;
	}
	if(isDataSrc() > 0){
		m_iCanStop = 0;
		return 0;
	}
	int iState;
	getVirtualState(iState); //��ȡ�鴦������״̬
	if(iState==MODULE_IDLE){
		m_iCanStop  = 0;						//�鴦�������У�����ͣ
	}
	else if(iState==MODULE_ABORT || iState==MODULE_STOP || iState==MODULE_PROC_STOP_ABORT ){
		//m_iCanStop  = 0;						//��Ȼ�����񣬵����鴦�����Ѿ����ˣ�Ӧ�������鴦������ʹ��������ͨ���������ο��ܻ��ס
		if(isDataSrc() > 0){ //��������Դͷ��������Ҳû��ϵ������ֱ��ͣ
			m_iCanStop = 0; //�������������ͣ��
			return 0;
		}
		int iTaskNum = Channel::calcWater(m_iVirtualId); //����һ���鴦������ˮλ
		if(iTaskNum != 0){ //��������Ҫ����
			m_iCanStop = 1; //������ͣ
			Log::log(0,"�鴦����[%d]ǿ���������̴����������...",m_iVirtualId); 
			if(startOneProcEx()<0) //�����鴦�����������ͨ,�������ʧ�ܣ�û�취��,ֻ����Ϊ����ͣ�ˣ��������ǰ����̶�ס��ֻ���˹�������
				m_iCanStop = 0; 
			checkProcState(); //����Ƿ��ֵ�BUG������Ϊ��ʹ���̲�����ʶ�ܹ�������
		}
		else{
			m_iCanStop = 0; //û��������Ҫ��������ͣ��
		}
	}
	else{
		m_iCanStop = 1;							//����ͣ��Ҫ�����������
		Log::log(0,"�账����%d����ˮ",m_iVirtualId);
	}
	return 0;
}
// ǿ��ֹͣ�鴦���������н���
int VirtualProcessor::forceStopProc()
{
	int iPos;
	TProcessInfo* pProcInfo = NULL;
	for(int iIndex=0; iIndex<m_iCnt; iIndex++)
	{
		int iPos = m_ProcInfoList[iIndex].iPos;
		pProcInfo = Channel::m_pProcInfoHead+iPos;
			/*
		if ( kill(pProcInfo->iSysPID,0)==0)
		{
			kill(pProcInfo->iSysPID,9);
		}
		*/
		pProcInfo->iState = ST_WAIT_DOWN;
		g_RunProcNum--; //���и�����1
	}
	return 0;
}
//�鿴�Ƿ����쳣�Ľ���
int VirtualProcessor::isErrExist(){
	int iPos;
	TProcessInfo* pProcInfo = NULL;
	for(int iIndex=0; iIndex<m_iCnt; iIndex++)
	{
		int iPos = m_ProcInfoList[iIndex].iPos;
		pProcInfo = Channel::m_pProcInfoHead+iPos;
		if( ST_KILLED==pProcInfo->iState || ST_ABORT==pProcInfo->iState){ 
			return pProcInfo->iProcessID;
		}
	}
	return 0;
}
