/*VER: 1*/

#include "Environment.h"
#include "RunProcessTask.h"
#include "Process.h"
#include "MBC.h"
#include "Log.h"
#include <string.h>
#include <stdio.h>
#include <string>

bool TaskInfo::m_bLoad = false;
ProcessTask *TaskInfo::m_poTaskList = 0;
TaskStep *TaskInfo::m_poTaskStepList = 0;
TaskStepProcess *TaskInfo::m_poStepProcList =0;
ProcessParam *TaskInfo::m_poProcParamList = 0;
HashList<ProcessTask *> *TaskInfo::m_poTaskIndex = 0;
//HashList<TaskStep *> *TaskInfo::m_poTaskStepIndex = 0;

int doExecSql(const char * SqlString)
{    
	try
	{
		DEFINE_QUERY(qry);
		
	    	qry.setSQL(SqlString);
	                        
	    	if (qry.execute() == false)    
	        	return -1;
	    
	    	if (qry.commit() == false)    
	        	return -1;    
	    
	    	qry.close ();
	    	return 1;
	}
	catch (TOCIException &e)
	{ 
		printf ("ִ��sql����,�������:%d,\n��������:%s\n,�������:%s",
			e.getErrCode(),e.getErrMsg(),e.getErrSrc());
	}
}

void replace(char * sql,char *A,char *B)
{
	char tmp_all[2001]={0},tmp1[2001]={0},tmp2[2001]={0};//sql;
	char newStr[201]={0};
	char *p=NULL;
	/*string newStr=B;
	string::size_type   pos;
	pos = tmp_all.find(A);
	if(pos==string::npos)return;
	while(pos!=string::npos){
		int len = strlen(A);
		tmp_all.replace(pos, len, newStr);
		pos = tmp_all.find(A);
	}
	strcpy(sql,tmp_all.c_str());*/

	strcpy(tmp_all,sql);
	while ((p=strstr(tmp_all,A))!=NULL)
	{
		strncpy(tmp1,tmp_all,p-tmp_all);
		strncpy(tmp2,p+strlen(A),strlen(tmp_all)-strlen(A)-strlen(tmp1));
		strcat(tmp1,B);
		strcat(tmp1,tmp2);
		strcpy(tmp_all,tmp1);
	}	
	strcpy(sql,tmp_all);
	
}

void TaskStepProcess::writeLogDetail(int iBillingCycleID, char * sNewState, char *sLogInfo,char cOpType)
{
	char sSql[4001]={0};

	if (cOpType == 'I')
		/*������־*/
		sprintf(sSql,"insert into B_PROCESS_RUN_DETAIL (task_id,billing_cycle_id,PROCESS_ID,run_seq,BEGIN_DATE,END_DATE,"
				" RUN_STATE,result_state,state_date,log_detail) "
				" values (%d,%d,%d,%d,sysdate,sysdate,'R','S',sysdate,null) ",
				m_iTaskID,iBillingCycleID,m_iProcessID,m_iRunSeq);
	else
		sprintf(sSql,"update B_PROCESS_RUN_DETAIL set run_state=\'%s\',LOG_DETAIL=\'%s\',end_date=sysdate,state_date=sysdate "
				" where task_id=%d and billing_cycle_id=%d and process_id=%d and RUN_SEQ=%d ",
			sNewState,sLogInfo,m_iTaskID,iBillingCycleID,m_iProcessID,m_iRunSeq);
	
	doExecSql(sSql);
}

void TaskStepProcess::repalceParam(char *sSql,char *sLogSql, int iBillingCycleID)
{	
	char sParamName[31]={0},sParamValue[101]={0},sParamValue1[101]={0};
	
	strcpy(sSql,m_sSqlStatement);
	strcpy(sLogSql,sSql);
	
	if (!m_oFirstParam)
		return;

	ProcessParam *p= m_oFirstParam;
	while (p)
	{
		strcpy(sParamName,"%");
		strcat(sParamName,p->m_sParamName);
		if (p->m_iValueType == 1)   //��ֵ��
		{
			strcpy(sParamValue,p->m_sParamValue);
			strcpy(sParamValue1,p->m_sParamValue);
		}
		else 					//�ַ���
		{
			sprintf(sParamValue,"\'%s\'",p->m_sParamValue);
			//д��־ʱ��updateʱ�����value�����е�����'�ᱨ����Ҫ������������
			//����ֱ��ִ��ֻ����һ�������ţ�������Ҫ�ֿ���
			sprintf(sParamValue1,"\''%s\''",p->m_sParamValue);
		}

		replace(sSql,sParamName,sParamValue);
		replace(sLogSql,sParamName,sParamValue1);
		p = p->m_poNextParam;
	}

	//Ĭ�ϲ���:
	strcpy(sParamName,"%BILLING_CYCLE_ID");
	sprintf(sParamValue,"%d",iBillingCycleID);
	replace(sSql,sParamName,sParamValue);
	replace(sLogSql,sParamName,sParamValue);

	strcpy(sParamName,"%TASK_ID");
	sprintf(sParamValue,"%d",m_iTaskID);
	replace(sSql,sParamName,sParamValue);
	replace(sLogSql,sParamName,sParamValue);

	strcpy(sParamName,"%PROCESS_ID");
	sprintf(sParamValue,"%d",m_iProcessID);
	replace(sSql,sParamName,sParamValue);
	replace(sLogSql,sParamName,sParamValue);

	return;
}

bool TaskStepProcess::executeSql(int iBillingCycleID)
{	
	char sExecSql[2001]={0},sLogSql[2001]={0};
/*
	strcpy(sParam,"%");
	strcat(sParam,"BILLING_CYCLE_ID");
	sprintf(sBillingCycleID,"%d",iBillingCycleID);
	replace(m_sSqlStatement,sParam,sBillingCycleID);
*/
	repalceParam(sExecSql,sLogSql,iBillingCycleID);

	try
	{
		DEFINE_QUERY(qry);
		qry.setSQL(sExecSql);
		qry.execute();
		qry.commit();
		qry.close();

		return true;
	}
	catch (TOCIException &e)
	{
		sprintf(m_sErrStr,"%d-%s:%s",e.getErrCode(),e.getErrMsg(),sLogSql);
		return false;
	}
	
	
}

bool TaskStepProcess::executeSh(int iBillingCycleID)
{
	char sParams[201]={0},sCommand[501]={0};

	if (m_sExecPath[strlen(m_sExecPath)-1] != '/')
		strcat(m_sExecPath,"/");

	try
	{	
		sprintf(sCommand,"sh %s%s %s",m_sExecPath,m_sExeName,m_sParameter);
		system(sCommand);
		return true;
	}
	
	catch (TException &e)
	{
		sprintf(m_sErrStr,"������Ϣ[%s]:%s",e.GetErrMsg(),sCommand);		
		return false;
	}
	catch(exception &e)
	{
		sprintf(m_sErrStr,"������Ϣ[%s]:%s",e.what(),sCommand);		
		return false;
	}
	catch (...)
	{
		sprintf(m_sErrStr,"������Ϣ[δ֪����]:%s",sCommand);		
		return false;
	}
}

bool TaskStepProcess::doExecute(int iBillingCycleID)
{
	bool iRet = false;
	
	writeLogDetail(iBillingCycleID,"R",NULL,'I');

	switch (m_iAppType){
		case 1:   //pl/sql��
			iRet=executeSql(iBillingCycleID);
			break;
		case 3:   //shell�ű�
			iRet=executeSh(iBillingCycleID);
			break;
		default:
			printf("Ŀǰ��֧��app_type=%d�ĵ���.",m_iAppType);
			return false;
		}

	if (iRet == true)
		writeLogDetail(iBillingCycleID,"S","",'U');
	else
		writeLogDetail(iBillingCycleID,"F",m_sErrStr,'U');

	return iRet;
}

void TaskStep::writeLog(int iBillingCycleID,char * sNewState, char cOpType)
{
	char sSql[501]={0};

	if (cOpType == 'I')
		/*������־*/
		sprintf(sSql,"insert into B_PROCESS_RUN_LOG (task_id,billing_cycle_id,run_seq,run_state,result_state,state_date) "
				" values (%d,%d,%d,'R','S',sysdate) ",m_iTaskID,iBillingCycleID,m_iRunSeq);
	else
		sprintf(sSql,"update B_PROCESS_RUN_LOG set run_state=\'%s\',state_date=sysdate where task_id=%d and billing_cycle_id=%d and RUN_SEQ=%d ",
			sNewState,m_iTaskID,iBillingCycleID,m_iRunSeq);
	
	doExecSql(sSql);
}

/*
bool TaskStep::checkPreState(int iBillingCycleID)
{
	char sSql[501]={0};
	int iCnt = 0;

//���ǰһ���з���ȷ����ڵȴ���ű��ڲ�����״̬���ɹ�	�ľ��޷�����
//���Զ���Ҫ��ǰһ��״̬�Ƿ�ĳ��˴���״̬
	DEFINE_QUERY(qry);
	if (!m_iAutoFlag)
	{
		qry.setSQL("SELECT COUNT(1)																	"
				"  FROM B_PROCESS_RUN_LOG                      "
				" WHERE RUN_SEQ = (SELECT MAX(RUN_SEQ)            "
				"                    FROM B_PROCESS_RUN_LOG       "
				"                   WHERE TASK_ID =:TASKID         "
				"                     AND BILLING_CYCLE_ID = :BILLINGCYCLEID    "
				"                     AND RUN_SEQ < :RUNSEQ)           "
				"   AND TASK_ID = :TASKID				 "
				"   AND BILLING_CYCLE_ID = :BILLINGCYCLEID                     "
				"   AND (RUN_STATE <> 'S' OR RESULT_STATE = 'S')  ");
	}
	else
	{
		qry.setSQL("SELECT COUNT(1)																	"
					"  FROM B_PROCESS_RUN_LOG                      "
					" WHERE RUN_SEQ = (SELECT MAX(RUN_SEQ)            "
					"                    FROM B_PROCESS_RUN_LOG       "
					"                   WHERE TASK_ID =:TASKID         "
					"                     AND BILLING_CYCLE_ID = :BILLINGCYCLEID    "
					"                     AND RUN_SEQ < :RUNSEQ)           "
					"   AND TASK_ID = :TASKID				 "
					"   AND BILLING_CYCLE_ID = :BILLINGCYCLEID                     "
					"   AND (RUN_STATE <> 'W' OR RESULT_STATE = 'S')  ");
	}
	
	qry.setParameter("TASKID",m_iTaskID);
	qry.setParameter("BILLINGCYCLEID",iBillingCycleID);
	qry.setParameter("RUNSEQ",m_iRunSeq);
	qry.open();
	if (qry.next())
		iCnt = qry.field(0).asInteger();
	qry.close();

	return (iCnt == 0);

}*/

bool TaskStep::runOneStep(int iBillingCycleID)
{	
	
	TaskStepProcess *poUndoProc = m_poStepFirstProc;

	//�Ƿ���ҪȥУ��ǰ�沽���״̬?��Ϊֻ��ǰ��ɹ��˲Ż����µ��ã�
	//�����Զ���������ҪУ��	
	/*if (!checkPreState(iBillingCycleID) )
	{
		printf("ǰ�沽��δ��ȷ��������")
		return false;
	}*/
		

	writeLog(iBillingCycleID,"R",'I');

	while (poUndoProc)
	{
		if (!poUndoProc->doExecute(iBillingCycleID))
		{
			writeLog(iBillingCycleID,"F",'U');
			return false;
		}		
		poUndoProc = poUndoProc->m_oSibStepProc;
	}

	//һ�������е�process����ɺ��޸�B_PROCESS_RUN_LOG�е�״̬��
	writeLog(iBillingCycleID,"S",'U');
	
	return true;
}

int ProcessTask::getTaskRunInfo(int iBillingCycleID)
{
	char sSql[501]={0};
	
	TOCIQuery qry1(Environment::getDBConn());

	sprintf(sSql,"SELECT nvl(MAX(RUN_SEQ),0) FROM B_PROCESS_RUN_LOG where task_id=:TASKID "
			" AND RUN_STATE = 'S' AND RESULT_STATE='S' and billing_cycle_id =:BILLINGCYCLEID ");
	qry1.setSQL(sSql);
	qry1.setParameter("TASKID",m_iTaskID);
	qry1.setParameter("BILLINGCYCLEID",iBillingCycleID);
	qry1.open();
	if (qry1.next())
		m_iSuccRunSeq = qry1.field(0).asInteger();
	qry1.close();

	sprintf(sSql,"select max(RUN_SEQ) from B_PROCESS_RUN_STEP where task_id=:TASKID ");
	qry1.setSQL(sSql);
	qry1.setParameter("TASKID",m_iTaskID);
	qry1.open();
	if (qry1.next())
		m_iMaxRunSeq = qry1.field(0).asInteger();
	qry1.close();

	return 1;
}

int ProcessTask::checkAllowRun(int iBillingCycleID)
{
	char cRes;

	getTaskRunInfo(iBillingCycleID);

	if (m_iSuccRunSeq == 0)
		return 1;
	
	if (m_iSuccRunSeq == m_iMaxRunSeq)
	{
		printf("����%d����%d�Ѿ��ɹ����й���Ҫ���ٴ����У����������־��",m_iTaskID,iBillingCycleID);
		return 0;
	}

	if (m_iSuccRunSeq < m_iMaxRunSeq)
	{
		printf("����%d����%d�ϴβ������У��ɹ����е��Ĳ�����%d,\n,�Ƿ�������к�������[Y/N]:",
				m_iTaskID,iBillingCycleID,m_iSuccRunSeq);
		cRes = getchar();
		cRes = toupper(cRes);
		if (cRes!='Y')
		{
			cRes='N';
			return 0;
		}
		return 1;
	}
	
}

TaskStep * ProcessTask::getStepFirstProc(int iRunSeq)
{
	TaskStep *p=m_poFirstStep;
	
	if (iRunSeq == 0) 
	{			
		return m_poFirstStep;
	}
	
	while (p)
	{		
		if (p->m_iRunSeq=iRunSeq)
			break;
		p = p->m_poTaskNextStep;
	}

	if (!p) return NULL;

/*iRunSeq�ǳɹ���ɵ����һ��������Ҫ����һ��ִ�е�process��Ҫ��������һ��*/
	return p->m_poTaskNextStep;
	
}

int ProcessTask::runByStep(int iBillingCycleID)
{
	TaskStep *poTaskStep = NULL;
	char sSql[501]={0};

	if (!checkAllowRun(iBillingCycleID))
		return 0;

	try{
		TOCIQuery qry(Environment::getDBConn());
		/*��δ�ɹ���ɲ��ֵ���־*/
		sprintf(sSql,  "begin delete from B_PROCESS_RUN_DETAIL where task_id=:TASKID and RUN_SEQ>:RUNSEQ and BILLING_CYCLE_ID=:BILLINGCYCLEID ;"
				" DELETE FROM B_PROCESS_RUN_LOG WHERE TASK_ID=:TASKID AND RUN_SEQ>:RUNSEQ and BILLING_CYCLE_ID=:BILLINGCYCLEID;  "
				" END; ");
		
		qry.setSQL(sSql);
		qry.setParameter("TASKID",m_iTaskID);
		qry.setParameter("RUNSEQ",m_iSuccRunSeq);
		qry.setParameter("BILLINGCYCLEID",iBillingCycleID);
		qry.execute();
		qry.commit();
		qry.close();
	}
	catch (TOCIException &e)
	{
		printf("errid:%d,\nerrmsg:%s,\n,errstr:%s\n",e.getErrCode(),e.getErrMsg(),e.getErrSrc());
		return 0;
	}

	TaskStep *poUndoStep=getStepFirstProc(m_iSuccRunSeq);
	if (!poUndoStep)
	{
		printf("δ�ҵ�����%d,ִ�д���%d��Ľ�����Ϣ.",m_iTaskID,m_iSuccRunSeq);
		return 0;
	}

/*������ʱֻ֧��һ��ִֻ��һ��process���Ժ��ǲ���*/
	
	while (poUndoStep)
	{		
		if (!poUndoStep->runOneStep(iBillingCycleID))
		{
			return 0;
		}		
		
		poUndoStep = poUndoStep->m_poTaskNextStep;	
	}

	return 1;
}

void TaskInfo::loadAll()
{
	int iRecCnt1 = 0 ,iRecCnt2 =0,i=0,j=0,k=0,m= 0;;
	char sSql1[501]={0};
	ProcessTask *poTaskTmp = NULL;
	
	if (m_bLoad)	
		unload();		

	TOCIQuery qry1(Environment::getDBConn());
	TOCIQuery qry2(Environment::getDBConn());		
/*����*/
	qry1.setSQL("select count(1)+16 from b_process_task where state='T0A' ");
	qry1.open();
	if (qry1.next())
		iRecCnt1 = qry1.field(0).asInteger();
	qry1.close();

	m_iTaskCnt = iRecCnt1;
	m_poTaskList = new ProcessTask[iRecCnt1];
	m_poTaskIndex = new HashList<ProcessTask *>((iRecCnt1 >> 1) |1);

	qry1.setSQL("select task_id,state from b_process_task where state='T0A' ");
	qry1.open();
	while (qry1.next())
	{
		m_poTaskList[i].m_iTaskID = qry1.field(0).asInteger();
		strcpy(m_poTaskList[i].m_sTaskState,qry1.field(1).asString());
		m_poTaskIndex->add(m_poTaskList[i].m_iTaskID,&(m_poTaskList[i]));
		i++;
	}
	qry1.close();

/*�����裬������жϴ�С������ֵ��proc�ĵط���*/
	qry1.setSQL("SELECT COUNT(1) FROM (Select DISTINCT task_id,run_seq FROM B_PROCESS_RUN_STEP)");
	qry1.open();
	if (qry1.next())
		iRecCnt1= qry1.field(0).asInteger();
	qry1.close();
	
	m_poTaskStepList = new TaskStep[iRecCnt1];
	//m_poTaskStepIndex = new HashList<TaskStep *>((iRecCnt1 >> 1) | 1);

/*���������Ϣ*/
	qry1.setSQL("select count(1) from B_PROCESS_RUN_STEP ");
	qry1.open();
	if (qry1.next())
		iRecCnt2 = qry1.field(0).asInteger();
	qry1.close();	

	m_poStepProcList= new TaskStepProcess[iRecCnt2];

	qry1.setSQL(" SELECT a.task_id,a.process_id,a.run_seq,d.app_id,c.exec_path,d.exec_name,d.sql_statement,d.app_type,a.auto_flag,c.parameter "
				" FROM B_PROCESS_RUN_STEP a,b_process_task b,wf_process c,wf_application d "
				" WHERE a.task_id=b.task_id "
				"      AND a.process_id=c.process_id "
				"      AND c.app_id=d.app_id "
				"      AND b.state='T0A' "
				" ORDER BY b.task_id,a.run_seq,process_id ");
	qry1.open();
	int iOldTaskID = 0;
	int iOldRunSeq = 0;
	TaskStepProcess*pOldSib = NULL;
	TaskStep *pOldNext = NULL;
	
	while (qry1.next())
	{
		//m_poStepProcList[j].m_i
		m_poStepProcList[j].m_iTaskID = qry1.field(0).asInteger();
		m_poStepProcList[j].m_iProcessID = qry1.field(1).asInteger();
		m_poStepProcList[j].m_iRunSeq = qry1.field(2).asInteger();
		m_poStepProcList[j].m_iAppID = qry1.field(3).asInteger();
		strcpy(m_poStepProcList[j].m_sExecPath,qry1.field(4).asString());
		strcpy(m_poStepProcList[j].m_sExeName,qry1.field(5).asString());
		strcpy(m_poStepProcList[j].m_sSqlStatement,qry1.field(6).asString());		
		m_poStepProcList[j].m_iAppType = qry1.field(7).asInteger();
		//m_poStepProcList[j].m_iAutoFlag = qry1.field(8).asInteger();
		strcpy(m_poStepProcList[j].m_sParameter,qry1.field(9).asString());
		m_poStepProcList[j].m_oSibStepProc = NULL;
		m_poStepProcList[j].m_oFirstParam = NULL;
		
		if ( iOldTaskID != m_poStepProcList[j].m_iTaskID)
		{
			if (!m_poTaskIndex->get( m_poStepProcList[j].m_iTaskID,&poTaskTmp))
				continue;

			iOldTaskID = m_poStepProcList[j].m_iTaskID;			
			poTaskTmp->m_poFirstStep = &m_poTaskStepList[k];
			m_poTaskStepList[k].m_iTaskID = qry1.field(0).asInteger();
			m_poTaskStepList[k].m_iRunSeq =qry1.field(2).asInteger();
			m_poTaskStepList[k].m_poStepFirstProc = &m_poStepProcList[j];
			m_poTaskStepList[k].m_poTaskNextStep = NULL;
			pOldNext = &m_poTaskStepList[k];
			pOldSib = &m_poStepProcList[j];
			iOldRunSeq = m_poStepProcList[j].m_iRunSeq;
			k++;
		}
		else
		{
			if (iOldRunSeq !=m_poStepProcList[j].m_iRunSeq)
			{
				m_poTaskStepList[k].m_iTaskID = qry1.field(0).asInteger();
				m_poTaskStepList[k].m_iRunSeq =qry1.field(2).asInteger();
				//m_poTaskStepIndex.add(m_poTaskStepList[k].m_iTaskID+m_poTaskStepList[j].m_iRunStep,&m_poTaskStepList[k]);			

				m_poTaskStepList[k].m_poStepFirstProc = &m_poStepProcList[j];
				m_poTaskStepList[k].m_poTaskNextStep = NULL;
				pOldNext->m_poTaskNextStep = &m_poTaskStepList[k];
				pOldNext = &m_poTaskStepList[k];
				pOldSib = &m_poStepProcList[j];
				k++;
			}
			else
			{
				pOldSib->m_oSibStepProc = &m_poStepProcList[j];
				pOldSib = &m_poStepProcList[j];
			}
		}
		
		//��ȡÿ�����̵Ĳ���
		qry2.setSQL("select count(1) from B_TASK_PROC_PARAM ");
		qry2.open();
		if (qry2.next())
			m_poProcParamList = new ProcessParam[qry2.field(0).asInteger()];
		qry2.close();
		
		qry2.setSQL("select param_name,param_value,value_type,task_id,process_id from B_TASK_PROC_PARAM "
					"where task_id=:TASKID and PROCESS_ID = :PROCID ");
		qry2.setParameter("TASKID", m_poStepProcList[j].m_iTaskID);
		qry2.setParameter("PROCID",m_poStepProcList[j].m_iProcessID);
		qry2.open();
		ProcessParam *pOldParam = NULL;
		while (qry2.next())
		{			
			
			strcpy(m_poProcParamList[m].m_sParamName,qry2.field(0).asString());
			strcpy(m_poProcParamList[m].m_sParamValue,qry2.field(1).asString());
			m_poProcParamList[m].m_iValueType = qry2.field(2).asInteger();
			m_poProcParamList[m].m_iTaskID = qry2.field(3).asInteger();
			m_poProcParamList[m].m_iProcessID = qry2.field(4).asInteger();
			m_poProcParamList[m].m_poNextParam = NULL;
			if (!pOldParam)
			{
				m_poStepProcList[j].m_oFirstParam = &m_poProcParamList[m];				
			}
			else
				pOldParam->m_poNextParam =& m_poProcParamList[m];
			
			pOldParam = &m_poProcParamList[m];
			m++;
		}
		qry2.close();

		j++;
		
	}
	qry1.close();

	m_bLoad = true;
}

void TaskInfo::unload()
{
	int i = 0;
	
	//delete[] m_poTaskStepIndex;
	
	delete[] m_poTaskStepList;
	delete[] m_poTaskIndex;
	delete[] m_poTaskList;
	delete[] m_poStepProcList;
	delete[] m_poProcParamList;
	
	//m_poTaskStepIndex = NULL;
	m_poTaskStepList = NULL;
	m_poTaskIndex = NULL;
	m_poTaskList = NULL;
	m_poProcParamList = NULL;
	m_bLoad = false;
}

int TaskInfo::run()
{
	ProcessTask *poTask;

	if (m_bLoad == false)
		loadAll();

	if (!m_poTaskIndex->get(m_iTaskID,&poTask))
	{
		printf("�޷��ҵ�����%d��ִ����Ϣ,��������״̬.",m_iTaskID);
		return 0;
	}

	return poTask->runByStep(m_iBillingCycleID);
}

int main(int argc,char **argv)
{
	int opt;
	int iBillingCycleID=0,iTaskID=0;
//	char sCycleBeginDate[18]={0},sCycleEndDate[18]={0};
	
	while ((opt=getopt(argc,argv,"b:B:T:t:?"))!=EOF)
	{
		switch(opt)
		{
			case 'B':
			case 'b':
				iBillingCycleID = atoi(optarg);
				break;
			case 'T':
			case 't':
				iTaskID = atoi(optarg);
				break;
			default:
				printf("�����������,\n,RunTask -b billing_cycle_id -t task_id \n");
				break;
		}
	}

	if (iBillingCycleID == 0 || iTaskID == 0)
	{
		printf("�����������,\n,RunTask -b billing_cycle_id -t task_id ");
		return 0;
	}

	Environment::getDBConn(true);
	DEFINE_QUERY(qry);
	qry.setSQL("SELECT to_char(cycle_begin_date,'yyyy/mm/dd'),to_char(cycle_end_date,'yyyy/mm/dd') FROM billing_cycle WHERE billing_cycle_id=:BILLINGCYCLEID");
	qry.setParameter("BILLINGCYCLEID",iBillingCycleID);
	qry.open();
	if (!qry.next())
	{
		printf("��������%d��Ч.\n",iBillingCycleID);
		return 0;
	}
	printf("ִ�е���������ʱ��Ϊ[%s]-[%s]\n.",qry.field(0).asString(),qry.field(1).asString());
	qry.close();

	TaskInfo oTaskInfo(iTaskID,iBillingCycleID);

	if (oTaskInfo.run())
		printf("����ɹ�ִ�н���.\n");
	else
		printf("����ɹ�ִ��ʧ�ܣ�������־.\n");

	return 1;
	
}

