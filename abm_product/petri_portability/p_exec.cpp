#include "petri.h"
namespace PETRI {
const int MAX_OUT_MSG_CNT  = 10;

/*
string execProcess::doExec(const vector<string>& vParamList )
{
	
	string ret = "-1";
	//int iRet = 0;
	TSHMCMA* p_shmTable = P_MNT.getShmTable();
	//bool extraLogin = false;
	TAppInfo* app = NULL;

 
_BEGIN: 
  //lock the signal.
  LockSafe(p_shmTable->m_iMonitorLock,0);
  p_shmTable->MntReqInfo.m_iIndex = P_MNT.i_index;
  p_shmTable->MntReqInfo.m_iAuthID = P_MNT.i_authID;
  p_shmTable->MntReqInfo.m_iProcessID = l_execObjID;
  p_shmTable->MntReqInfo.m_iOperation = 1;// 1 start 2 stop
  //strcpy( p_shmTable->MntReqInfo.m_sMsgBuf,sQqstBuf);
  
  p_shmTable->MntReqInfo.m_iStatus = REQ_SEND_COMMAND; //请求执行指令
  
  int iTimeOut = 0;
  while (p_shmTable->MntReqInfo.m_iStatus == REQ_SEND_COMMAND ||
      p_shmTable->MntReqInfo.m_iStatus == REQ_COMMAND_SENDING)
  {//waiting...
      usleep(USLEEP_LONG);
      if (iTimeOut++ > TIME_OUT) {
          p_shmTable->MntReqInfo.m_iStatus = WF_WAIT;
          
          UnlockSafe(p_shmTable->m_iMonitorLock,0);
      }
  }
  //调用结束 
  UnlockSafe(p_shmTable->m_iMonitorLock,0);


  if (p_shmTable->MntReqInfo.m_iStatus == MNT_LOG_DEAD) {
		goto _BEGIN;
  }

	app = p_shmTable->getAppInfoByProcID(l_execObjID);
	string sColName="SYSPID";
	ts->updateTransExecLog(sColName,app->m_iAppPID);

  	while ( app->m_cState == 'R' || app->m_cState == 'B' ) {
		string sql;
		string s_dbstate;
		DEFINE_PETRI_QUERY(query);
		try {
			sql="SELECT EXE_STATE FROM P_TRANSITION_ATTR WHERE TRANSITION_ID = :TS";
			query.close();
			query.setSQL(sql.c_str());
			query.setParameter("TS",ts->getTransID());
			query.open();
			query.next();
			s_dbstate = query.field(0).asString();
			query.close();
		} catch(TOCIException &toe) {
			LOG(toe.getErrMsg());
		}catch(...) {};	

		if ("E" == s_dbstate){
			ret = "999";
			return ret;
		}
			
		usleep(100000);
	}
	
	switch(app->m_cState) {
		case 'A':
			ts->updateTransExecLog("REMARK","access monitor,can't start");
			ret = "-1";
			break;
		case 'K':
			ret = "-999";
			break;
		case 'X':
			if( 0 != app->m_sErrMsg[0]){
				ts->updateTransExecLog("REMARK",app->m_sErrMsg);
			}
			if( 0 != app->m_iRet){
				int iRet = app->m_iRet;
				char tmpRet[128];
				sprintf(tmpRet,"%d\0",iRet);
				ret = tmpRet;
			}
			else{
				ret = "1";
			}
			break;
		case 'E':
			if( 0 != app->m_sErrMsg[0]){
				ts->updateTransExecLog("REMARK",app->m_sErrMsg);
			}
			int iRet = app->m_iRet;
			char tmpRet[128];
			sprintf(tmpRet,"%d\0",iRet);
			ret = tmpRet;
			break;
		default:
			ts->updateTransExecLog("REMARK","unexpect process state");
			ret = "-1";
			break;
	}	

	return ret;
};
*/


/*
void exec::saveErrMsg(const char* in_msg)
{
	//char sMsg[1024];
	//sprintf("变迁名称>%s ;执行工号>%ld ;执行信息>%s",ts->getName(),ts->l_staffID);
#ifdef HB_28
  Log::m_iAlarmLevel = 3;
  Log::m_iAlarmDbLevel = -1;
  ALARMLOG28(0,MBC_CLASS_Business,0,"[TransName]%s [StaffID]%ld [ExecInfo]%s",ts->getName(),ts->l_staffID,in_msg);
#endif


	DEFINE_PETRI_QUERY(query);

	string sMsg;
	if (in_msg && in_msg[0]) sMsg = in_msg;
	else sMsg ="0";

	LOG(in_msg);
	try {
		string sql =" \
			INSERT INTO P_ACTION \
			(ACTION_ID,ELEMENT_ID,ELEMENT_TYPE,ATTR,ATTR_VALUE,ACTION,CREATE_DATE,ERR_MSG,DRAW_ID,ACTION_TYPE,ELEMENT_NAME) \
			VALUES \
			(SEQ_PETRI_ACTION_ID.NEXTVAL,:ELEMID,'T','EXEC_RESULT',  NULL,'U',SYSDATE, \
			 SUBSTR(:MSG,0,256),:DRAWID,'A',:ELMNAME ) \
			";
		query.close();
		query.setSQL(sql.c_str());
		query.setParameter("ELEMID",ts->getTransID());
		query.setParameter("MSG",in_msg);
		query.setParameter("DRAWID",ts->getDrawID());
		query.setParameter("ELMNAME",ts->getName());

		query.execute();

	}catch(TOCIException& toe) {
		LOG(toe.getErrMsg());
		query.close();
		return;
	}catch (...) {
		LOG(" ");
		query.close();
		return;
	}
	query.commit();
	query.close();

};
*/


/*
void ::PETRI::exec::updateTransState(const string& cState)
{
	DEFINE_PETRI_QUERY(query);
	try {
		string sql = "UPDATE P_TRANSITION_ATTR SET EXEC_STATE=:ST WHERE TRANSITION_ID=:ID";
		query.close();
		query.setSQL(sql.c_str());
		query.setParameter("ST",cState);
		query.setParameter("ID",l_execObjID);
		query.open();

		sql.clear();
		sql = "INSERT INTO P_ACTION(ACTION_ID,ELEMENT_ID,ELEMENT_TYPE,ATTR,ATTR_VALUE,ACTION,CREATE_DATE) VALUES(SEQ_PETRI_ACTION_ID.NEXTVAL,:TS,'T','EXEC_STATE',:ST,'U',SYSDATE)";
		query.close();
		query.setSQL(sql.c_str());
		query.setParameter("TS",l_execObjID);
		query.setParameter("ST",cState);
		query.open();
		query.close();

	} catch (...) {

	};
	return;

};
*/



/*
string execProcess::doKill()
{
	string ret ="-1";
	TSHMCMA* p_shmTable = P_MNT.getShmTable();
	bool extraLogin = false;
	DEFINE_PETRI_QUERY(query);
	//TAppInfo* app = NULL;

 
_BEGIN: 
  //lock the signal.
  LockSafe(p_shmTable->m_iMonitorLock,0);
  p_shmTable->MntReqInfo.m_iIndex = P_MNT.i_index;
  p_shmTable->MntReqInfo.m_iAuthID = P_MNT.i_authID;
  p_shmTable->MntReqInfo.m_iProcessID = l_execObjID;
  p_shmTable->MntReqInfo.m_iOperation = 2;//1 start 2 stop
  
  p_shmTable->MntReqInfo.m_iStatus = REQ_SEND_COMMAND; //请求执行指令
  
  int iTimeOut = 0;
  while (p_shmTable->MntReqInfo.m_iStatus == REQ_SEND_COMMAND ||
      p_shmTable->MntReqInfo.m_iStatus == REQ_COMMAND_SENDING)
  {//waiting...
      usleep(USLEEP_LONG);
      if (iTimeOut++ > TIME_OUT) {
          p_shmTable->MntReqInfo.m_iStatus = WF_WAIT;
          if (p_shmTable->MntReqInfo.m_iStatus == REQ_SEND_COMMAND)
              P_RECVBUFF.addMsg("monitor no response");
          else
              P_RECVBUFF.addMsg("commander sending error");
          
          UnlockSafe(p_shmTable->m_iMonitorLock,0);
      }
  }
  //调用结束 

  if (p_shmTable->MntReqInfo.m_iStatus == MNT_LOG_DEAD) {
		//extraLogin = true;
//		P_MNT.GetLogInfo(true);
		goto _BEGIN;
	};

  UnlockSafe(p_shmTable->m_iMonitorLock,0);

	//if (extraLogin) P_MNT.logout();

	return ret;


};



string execProcess::doQuery()
{
	string sRet;
	char queryBuff[2048];

	sRet.clear();
	memset(queryBuff,0,2048);

	TSHMCMA* p_shmTable = P_MNT.getShmTable();
	TAppInfo *app = p_shmTable->getAppInfoByProcID(l_execObjID);

	sprintf(queryBuff,
				"<iProcessCnt=%lu><iNor=%lu><iErr=%lu><iBlk=%lu><iOth=%lu>\0",
				app->m_lProcessCnt,
				app->m_lNormalCnt,
				app->m_lErrorCnt,
				app->m_lBlackCnt,
				app->m_lOtherCnt);

	sRet = queryBuff;
	
	for(int i=0; i<MAX_OUT_MSG_CNT; i++) {
	   char str[MAX_APPINFO__LEN];
	   if ( app->getInfo(str) ) {
			 sprintf(queryBuff,"<sMsg=%s>\0",str);
			 sRet = sRet + queryBuff;
	   }
	   else break;
   }
	 return sRet;

};
*/


string execShell::getLogfile()
{
			char tmpBuf[1024];
			string sDir,sFile;
			char* p;
#ifdef HB_28
			p = getenv("HBLOG");
			sDir = p?p:"/bill/TIBS_HOME";
#else
			p = getenv("TIBS_HOME");
			sDir = p?p:"/bill/TIBS_HOME";
			sDir += "/log";
#endif
			sDir += "/petri_log/";

			sprintf(tmpBuf,"%ld\0",ts->getTransID());
			sFile = sDir+"T_"+tmpBuf+".log";
			return sFile;
			sprintf (tmpBuf,"PETRI_LOGFILE=%s\0",sFile.c_str());

}


string execShell::doExec(const vector<string>& vParamList )
{
	string sRet = "-1";
	string sStmt;
	
	int iRet=0;
	char ret[32];
	int iStatus = 0;
	string sEnv,sFile,tmp_err;
	char* p = NULL;
//	char tmpBuf[1024];
	sFile = getLogfile();
	tmp_err ="2>> checkerro.txt";

	sEnv = "PETRI_LOGFILE="+sFile;
	pid_t pid = fork();
	switch(pid) {
		case 0:
			{
			sleep(1);

			// Handle The LogFile
			putenv(sEnv.c_str());

			sStmt.clear();
			sStmt = "echo start at `date` >> ";
			sStmt += sFile;
			system(sStmt.c_str());
			sStmt.clear();
			unsigned int iParamSize = vParamList.size();

			if (iParamSize >0) {
				s_execParam = " ";
				for (unsigned i = 0;i<iParamSize;i++) 
					s_execParam = s_execParam + vParamList[i] + " ";
			}


			sStmt = s_execName+" "+s_execParam+" "+tmp_err;	
			if (s_execPath.length())
			{
			chdir(s_execPath.c_str());		
			}

		//	sStmt = s_execPath+s_execName+" "+s_execParam+" "+tmp_err;
			if (s_execName.length()) {
				iStatus = system(sStmt.c_str());
				printf("%d",iStatus);
				printf("%s",sStmt.c_str());
				iRet = WEXITSTATUS(iStatus);
			} else {
				iRet = -1;
			}
			exit(iRet);
			break;
			}
		case -1:
			return sRet;
			break;
		default:
			P_DB.getDB(true);
			ts->updateTransExecLog("SYSPID",(int)pid);
			wait(&iStatus);
			if (WIFEXITED(iStatus) ) {
				iRet = WEXITSTATUS(iStatus);
				sprintf(ret,"%d\0",iRet);
				sRet = ret;
			} else {
				ts->updateTransExecLog("REMARK","WIFEXITED execprocess fail");
				sRet= "-1";
			}

/*
	sprintf(tmpBuf,"tail -n 1 %s",sFile.c_str());
	LOG(sFile.c_str());
	FILE* f = popen(tmpBuf,"r");

	if (f != NULL )
	{
		LOG("f IS NOT NULL");

	memset(tmpBuf,0,sizeof(tmpBuf));
	p=fgets(tmpBuf,sizeof(tmpBuf),f);
	pclose(f);
				ts->updateTransExecLog("REMARK",tmpBuf);
				saveErrMsg(tmpBuf);
				P_DB.getDB()->commit();			
	}
*/
			return sRet;
			break;
	};
};

string execShell::doKill()
{
	string sRet = "-1";

	DEFINE_PETRI_QUERY(query);
	try {
		string sql="SELECT SYSPID FROM P_TRANSITION_EXEC_LOG  WHERE LOG_ID=:ID ";
		query.close();
		query.setSQL(sql.c_str());
		query.setParameter("ID",ts->getLogID());
		query.open();
		if (query.next()) {
			int iPid = query.field(0).asInteger();
			int i=kill(iPid,16);
			if (i == 0) {
				sRet = "0";
			}
		};
		query.close();
	}	catch(...) {

	}

	return sRet;

};

string execShell::doQuery()
{
	string sRet;
	char queryBuff[2048];

	sRet.clear();
	memset(queryBuff,0,2048);

	char tmpBuf[256];
	string sFile = getLogfile();
	sprintf(tmpBuf,"tail -n 1 %s",sFile.c_str());
	FILE* f = popen(tmpBuf,"r");

	if (f == NULL )
	{
		return sRet;
	}

	memset(tmpBuf,0,256);
	char *p = fgets(tmpBuf,256,f);
	if (p != NULL) {
		sprintf(queryBuff,"<sMsg=%s>\0",tmpBuf);
	}
	pclose(f);

	return sRet;

}

/*
string execShell::doQuery()
{
	DEFINE_PETRI_QUERY(query);
	int iLength = strlen(P_ACKMSG->mbody);
	string sRet = "-1";
	string sStartTime,sEndTime;
	try { 
		string sql = "SELECT TO_CHAR(START_TIME,'YYYYMMDDHH24MISS'),TO_CHAR(END_TIME,'YYYYMMDDHH24MISS') FROM P_TRANSITION_EXEC_LOG WHERE LOG_ID=:ID";
		query.close();
		query.setSQL(sql.c_str());
		query.setParameter("ID",ts->getLogID());
		query.open();
		if (query.next()) {
			sStartTime = query.field(0).asString();
			sEndTime = query.field(1).asString();
			//P_RECVBUFF.setStartTime(sStartTime.c_str());
			//P_RECVBUFF.setEndTime(sEndTime.c_str());
			//P_RECVBUFF.setStartTime(query.field(0).asString());
			//P_RECVBUFF.setEndTime(query.field(1).asString());
			if (sStartTime == "") sStartTime =" ";
			if (sEndTime == "") sEndTime =" ";
			sprintf(P_ACKMSG->mbody+iLength,"<sStartTime=%s><sEndTime=%s>\0",sStartTime.c_str(),sEndTime.c_str() );
			sRet = "0";
		};
		query.close();

	} catch(TOCIException &toe) {
		printf("%s,%d__%s\n",__FILE__,__LINE__,toe.getErrMsg());
	} catch(...) {

	};
	return sRet;

};
*/

string execSQL::doExec(const vector<string>& vParamList )
{
	string sRet = "-1";
	char ret[32];
	int iRet=0;
	if (s_execName.length() == 0) {
		ts->updateTransExecLog("REMARK","sql execname is null");
		return sRet;
	}
	char tmpret[2048];
	memset(tmpret,0,sizeof(tmpret));

	P_DB.getDB()->commit();
	long m_transID = ts->getTransID();
	pid_t pid = fork();
	string sql;

	int iStatus = 0;
	switch(pid) {
		case 0:
			sleep(1);
			try {
				
				char m_user[24];
  				char m_passwd[24];
				char m_destpasswd[24];
  				char m_constr[32];
				memset(m_user,0,sizeof(m_user));
				memset(m_passwd,0,sizeof(m_passwd));
				memset(m_destpasswd,0,sizeof(m_destpasswd));
				memset(m_constr,0,sizeof(m_constr));

				P_DB.getDB(true);
				DEFINE_PETRI_QUERY(query1);
				sql =" SELECT B.USER_NAME,B.PASSWORD,B.HOST_STR \
				FROM P_TRANSITION_HOST_INFO A ,WF_SQL_LOGIN_CONFIG B \
				WHERE A.LOGIN_ID = B.LOGIN_ID \
				AND A.TRANSITION_ID = :TRANSID";
				query1.close();				
				query1.setSQL(sql.c_str());
				query1.setParameter("TRANSID",m_transID);
				query1.open();
				query1.next();
				
				strncpy(m_user,query1.field(0).asString(),sizeof(m_user)-1);
				strncpy(m_passwd,query1.field(1).asString(),sizeof(m_passwd)-1);
				strncpy(m_constr,query1.field(2).asString(),sizeof(m_constr)-1);
				decode(m_passwd,m_destpasswd);
				DEFINE_PETRI_QUERY_SETCONN(query,m_user,m_destpasswd,m_constr);

				long tmpTransID;

				/*
				query.close();
				query.setSQL("DELETE TMP_TRANSITION_LOG WHERE TRANSITION_ID = :ID");
				query.setParameter("ID",ts->getTransID());
				query.execute();
				P_DB.getDB()->commit();
				*/


				/*
				char exeSQL [ 256];
				sprintf(exeSQL,"BEGIN %s(%ld); END;\0",s_execName.c_str(),ts->getTransID());
				LOG(exeSQL);
				query.close();
				query.setSQL(exeSQL);
				query.execute();
				*/
				/*
				string exeSQL = "SELECT " + s_execName+" FROM DUAL ";
				LOG(execSQL.c_str());
				query.close();
				query.setSQL(exeSQL.c_str());
				query.open();
				if (query.next()) {
					strncpy(tmpret,query.field(0).asString(),511);
				}
				char *p = strchr(tmpret,'|');
				if (p) {
					*(p++)=0;
					ts->updateTransExecLog("REMARK",p);
					saveErrMsg(p);
					P_DB.getDB()->commit();
				}
				iRet = atoi(tmpret);
				*/




				unsigned i = 0;
				s_execParam.clear();
				for (;i <vParamList.size();i++) {
					if (i != 0) 
						s_execParam += ",";

					s_execParam += vParamList[i];
				}
				if (i > 0) {
					s_execParam = "( " +s_execParam+" )";
				}


				sql= " \
					declare \
					ret varchar2(512);\
					begin \
						ret:=" + s_execName +  s_execParam + "; \
					end; ";
				//LOG(sql.c_str());
				query.close();
				query.setSQL(sql.c_str());
				query.execute();
				query.close();
				/*
				query.setSQL(" SELECT TRANSITION_ID ,RETURN_FLAG,REMARK  FROM ( \
					SELECT TRANSITION_ID ,RETURN_FLAG,REMARK FROM TMP_TRANSITION_LOG WHERE TRANSITION_ID = :ID \
					UNION ALL \
					SELECT -1,-1,'' FROM DUAL) WHERE ROWNUM =1"
				);
				query.setParameter("ID",ts->getTransID());
				query.open();
				if (query.next()) {
					tmpTransID = query.field(0).asLong();
					if (tmpTransID != -1) {
						iRet = query.field(1).asInteger();
						printf("TransitioN_id = %ld,iRet = %d\n",ts->getTransID(),iRet);
						strcpy(tmpret,query.field(2).asString());
					} else {
						iRet =  254;
						strcpy(tmpret,"The procedure Hasn't written Info into TMP_TRANSITION_LOG");
					}

				} else {
					iRet =  254;
					strcpy(tmpret,"The procedure Hasn't written Info into TMP_TRANSITION_LOG");
				};
				query.close();
				ts->updateTransExecLog("REMARK",tmpret);
				saveErrMsg(tmpret);
				*/
				P_DB.getDB()->commit();
				
			} catch(TOCIException &toe) {
				P_DB.getDB()->rollback();
				char sString[1000];
				memset(sString,0,sizeof(sString));
				snprintf
            	(
                	sString,
                	999,
               		"Err: %s  SQL: %s",
                	toe.getErrMsg(),
                	toe.getErrSrc()
            	);
				ts->updateTransExecLog("REMARK",sString);
				iRet = 254;
				exit(iRet);
			}catch (...) {
				exit(iRet);
			};
			exit(iRet);

			break;
			
		case -1:
			P_RECVBUFF.addMsg("资源不足");
			return sRet;
			break;
		default:
			P_DB.getDB(true);
			ts->updateTransExecLog("SYSPID",(int)pid);
			wait(&iStatus);
			if (WIFEXITED(iStatus) ) {
				iRet = WEXITSTATUS(iStatus);
				if (iRet == 254) { // 函数执行出错 
					sRet = "-1";
				} else { // 否则,从表中取执行结果
					try {
						DEFINE_PETRI_QUERY(query);
						sql = "\
							SELECT DECODE(LOG_TYPE,'A','0','E','1','1')LOG_TYPE,LOG_STATEMENT FROM P_TRANSITION_OUTPUT_LOG WHERE LOG_ID = ( \
								SELECT NVL(MAX(LOG_ID),-1) FROM P_TRANSITION_OUTPUT_LOG WHERE TRANSACTION_ID = :TA AND TRANSITION_ID = :TS) \
						";									 
						query.setSQL(sql.c_str());
						query.setParameter("TA",ts->l_transactionID);
						query.setParameter("TS",ts->getTransID());
						query.open();
						if (query.next()) {
							sRet = query.field(0).asString();
							strncpy(tmpret,query.field(1).asString(),sizeof(tmpret)-1);
						} else {
							sRet = "-1";
						}
						query.close();
						ts->updateTransExecLog("REMARK",tmpret);

					} catch(TOCIException &toe) {
						LOG(toe.getErrMsg());
						sRet = "-1";
					}
				}
			} else {
				ts->updateTransExecLog("REMARK","WIFEXITED execprocess fail");
				sRet= "-1";
			}
			return sRet;
			break;

	};


};

string execSQL::doKill()
{
	DEFINE_PETRI_QUERY(query);
	string sRet = "-1";
	try {
		string sql="SELECT SYSPID FROM P_TRANSITION_EXEC_LOG \
								WHERE LOG_ID=:ID AND END_TIME IS NULL ORDER BY LOG_ID DESC";
		query.close();
		query.setSQL(sql.c_str());
		query.setParameter("ID",ts->getLogID());
		query.open();
		if (query.next()) {
			int iPid = query.field(0).asInteger();
			int i=kill(iPid,16);
			if (i == 0) {
				sRet = "0";
			}
		};
		query.close();
	} catch(TOCIException &toe) {
				LOG(toe.getErrMsg());
	}	catch(...) {

	};
	return sRet;


};


string execSQL::doQuery()
{
	string sRet;
	char queryBuff[2048];

	sRet.clear();
	memset(queryBuff,0,2048);



	DEFINE_PETRI_QUERY(query);
	string sStartTime,sEndTime;
	try { 
		string sql = "SELECT TO_CHAR(START_TIME,'YYYYMMDDHH24MISS'),TO_CHAR(END_TIME,'YYYYMMDDHH24MISS') FROM P_TRANSITION_EXEC_LOG WHERE LOG_ID=:ID";
		query.close();
		query.setSQL(sql.c_str());
		query.setParameter("ID",ts->getLogID());
		query.open();
		if (query.next()) {
			//sStartTime = query.field(0).asString();
			//sEndTime = query.field(1).asString();
			//P_RECVBUFF.setStartTime(sStartTime.c_str());
			//P_RECVBUFF.setEndTime(sEndTime.c_str());
			//P_RECVBUFF.setStartTime(query.field(0).asString());
			//P_RECVBUFF.setEndTime(query.field(1).asString());
			sprintf(queryBuff,"<sStartTime=%s><sEndTime=%s>\0",query.field(0).asString(), query.field(1).asString() );
			sRet = queryBuff;
		};
		query.close();
	} catch(TOCIException &toe) {
		LOG(toe.getErrMsg());
	} catch(...) {
	};
	return sRet;

}

/*
string execProcess::init()
{
	return "0";

};
*/

string execShell::init()
{
	string sRet = "-1";
	s_execPath.clear();
	s_execName.clear();
	s_execParam.clear();
	DEFINE_PETRI_QUERY(query);
	try {
		string sql = " \
			SELECT A.EXEC_PATH ||\
			DECODE(SUBSTR(A.EXEC_PATH,LENGTH(A.EXEC_PATH),1),'/','','/') EXECPATH, \
			B.EXEC_NAME   EXECNAME, \
				A.PARAMETER \
				FROM WF_PROCESS A ,WF_APPLICATION B \
				WHERE A.APP_ID = B.APP_ID \
				AND PROCESS_ID=:ID" ;
		query.close();
		query.setSQL(sql.c_str());
		query.setParameter("ID",l_execObjID);
		query.open();
		if (query.next()) {
			s_execPath = query.field(0).asString();
			s_execName= query.field(1).asString();
			s_execParam= query.field(2).asString();
			sRet = "0";
		};
		query.close();
	} catch(TOCIException &toe) {
				LOG(toe.getErrMsg());
				query.close();
	} catch (...) {
		query.close();
	}

	return sRet;
};

string execSQL::init()
{
	string sRet = "-1";
	s_execPath.clear();
	s_execName.clear();
	s_execParam.clear();
	DEFINE_PETRI_QUERY(query);
	try {
		string sql = "  SELECT SQL_STATEMENT FROM WF_SQL_INFO WHERE SQL_INFO_ID=:ID"; 
		query.close();
		query.setSQL(sql.c_str());
		query.setParameter("ID",l_execObjID);
		query.open();
		if (query.next()) {
			s_execName= query.field(0).asString();
			sRet = "0";
		};
		query.close();
	} catch(TOCIException &toe) {
				LOG(toe.getErrMsg());
				query.close();
	} catch (...) {
		query.close();
	}

	return sRet;
};





};

