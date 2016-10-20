/*VER: 1*/ 

//#include "Process.h"
#include "WorkFlow.h"
#include "Log.h"
#include "ParamDefineMgr.h"
#include "Environment.h"

#include "E8_User.h"

E8UserCtl::E8UserCtl(int iGroup):DataEventMemCtl(iGroup)
{

}

E8UserCtl::~E8UserCtl()
{

}

void E8UserCtl::create()
{
	int count = 0;
	char countbuf[32];
	memset(countbuf,'\0',sizeof(countbuf));	
	if(!ParamDefineMgr::getParam(USERINFO_SEGMENT, E8_USER_INFO_NUM, countbuf))
	{
		Log::log(0,"�����ڱ�b_param_define������E8�������ڴ��С����.");
		THROW(MBC_E8UserCtl+50);
	}
	count = atoi(countbuf);
	
	m_poE8AccountData->create(count);
	m_poE8AccountIndex->create(count,MAX_ACCOUNT_NBR_LEN,2);
}

void E8UserCtl::unLoadE8User()
{
#define UNLOAD(X) \
				if(X) \
                if (X->exist()) { \
				if(X->connnum() > 2) \
				{ \
				THROW(MBC_E8UserCtl+3); \
} \
				X->remove (); \
                }
	
	UNLOAD(m_poE8AccountData);	
	UNLOAD(m_poE8AccountIndex);
	
	m_bAttached = false;

	return;
}

void E8UserCtl::linkUserName(DataE8AccountInfo &pData,int iPos)
{
	DataE8AccountInfo *p;
	unsigned int k=0;
	
	p = (DataE8AccountInfo *)(*m_poE8AccountData);
	
	//����string�������ж�ֻ�Ǹ���ǰ12λ��Ҳ��������ʺŵ�ǰ12λ
	if (!m_poE8AccountIndex->get(pData.sUserName,&k))
	{											
		m_poE8AccountIndex->add(pData.sUserName,iPos);			
	}
	else //����ܹ��ҵ�����Ҫ�ж��Ƿ��ʺ��Ƿ���ͬ�����Ұ���Чʱ�䵹������
	{
		unsigned int iCur = k;
		unsigned int iPre = k;
		unsigned int iHead = k;
		while (iCur)
		{																
			if (strcmp(pData.sEffDate,p[iCur].sEffDate) > 0 )
			{
				if (iCur == iHead)
				{
					pData.m_iNextOffset = iCur;
					m_poE8AccountIndex->add(pData.sUserName,iPos);
				}
				else
				{
					pData.m_iNextOffset = iCur;
					p[iPre].m_iNextOffset = iPos;
				}
				break;
			}
			iPre = iCur;
			iCur = p[iCur].m_iNextOffset;
		}  //end while
		
		if (!iCur)
		{
			pData.m_iNextOffset = iCur;
			p[iPre].m_iNextOffset = iPos;
		}
		
	}//end else
}


void E8UserCtl::loadE8User()
{		
	unLoadE8User();		
	
	int count,num;
	char sql[2048];
	int iRecCnt =0;
	char countbuf[32];
	
	DataE8AccountInfo * p;
	unsigned int i, k;
	
	TOCIQuery qry(Environment::getDBConn());
	if (!ParamDefineMgr::getParam(USERINFO_LIMIT, USERINFO_NUM_SERVLOCATION, countbuf)) {
		num = 0;
	} else {
		num = atoi(countbuf);
	}
	char strDate[16];
	memset(strDate,0,sizeof(strDate));
	if (num>0) {
		getBillingCycleOffsetTime(num,strDate);
	}	
	//�Ƚ����ڴ�
	this->create();

	p = (DataE8AccountInfo *)(*m_poE8AccountData);

	/*TRUNC(STATE_DATE)>=TRUNC(SYSDATE)-(:iSynDays-1) OR :iSynDays<=0*/
		/*mdf by yuanp 20070212 ʡ��˾Ҫ���ǲμӹ�E8�Ķ����зָ�������ڵĵ���״̬������Ԫ���ַǼ�ʱ����*/       	
		/*       		 AND nvl(EXP_DATE,sysdate+3000)>=to_date(:sCycleBeginDate,'yyyymmdd')-3
		AND STATE='D0A'*/
	sprintf(sql,"SELECT USER_ID, USER_SEQ, USER_NAME, AREA_CODE , "
		"to_char(nvl(STATE_DATE,sysdate-3000),'yyyymmddhh24miss'), "
		"STATE ,"		
		"to_char(nvl(EFF_DATE,sysdate-3000),'yyyymmddhh24miss'),"
		"to_char(nvl(EXP_DATE,sysdate+3000),'yyyymmddhh24miss') "           
        "FROM DATA_USER "
		"WHERE "
		"(USER_ID,USER_SEQ) IN (SELECT USER_ID,MAX(USER_SEQ) FROM DATA_USER GROUP BY USER_ID) "
		"ORDER BY USER_ID,USER_SEQ");

	qry.setSQL(sql);   qry.open();

	while (qry.next())
	{
		/*if (strcmp(qry.field(7).asString(),strDate)<0) {
			continue;
		}*/
		i = m_poE8AccountData->malloc();
		if (!i) 
		{
			Log::log(0,"E8�������ݿռ��޷��������䣬���ܿռ�������");
			THROW(MBC_E8UserCtl+1);	
		}

		memset(&p[i],0,sizeof(DataE8AccountInfo));
		
		p[i].lUserID = qry.field(0).asInteger();
		p[i].iUserSeq = qry.field(1).asInteger();
		strcpy(p[i].sUserName,qry.field(2).asString());
		strcpy(p[i].sAreaCode,qry.field(3).asString());
		strcpy(p[i].sStateDate,qry.field(4).asString());
		strcpy(p[i].sState,qry.field(5).asString());
		strcpy(p[i].sEffDate,qry.field(6).asString());
		strcpy(p[i].sExpDate,qry.field(7).asString());

		linkUserName(p[i],i);
		iRecCnt++;
		
		//����string�������ж�ֻ�Ǹ���ǰ12λ��Ҳ��������ʺŵ�ǰ12λ
		/*if (!m_poE8AccountIndex->get(p[i].sUserName,&k))
		{											
			m_poE8AccountIndex->add(p[i].sUserName,i);	
			iUpdCnt++;
		}
		else //����ܹ��ҵ�����Ҫ�ж��Ƿ��ʺ��Ƿ���ͬ�����Ұ���Чʱ�䵹������
		{
			unsigned int *iCur = k;
			unsigned int *iPre = k;
			unsigned int *iHead = k;
			while (*iCur)
			{																
				if (strcmp(p[i].sEffDate,p[*iCur].sEffDate) > 0 )
				{
					if (*iCur == *iHead)
					{
						p[i].m_iNextOffset = *iCur;
						m_poE8AccountIndex->add(p[i].sUserName,i);
					}
					else
					{
						p[i].m_iNextOffset = *iCur;
						p[*iPre].m_iNextOffset = i;
					}
					break;
				}
				*iPre = *iCur;
				*iCur = p[*iCur].m_iNextOffset;
			}  //end while
			
			if (!(*iCur))
			{
				p[i].m_iNextOffset = *iCur;
				p[*iPre].m_iNextOffset = i;
			}
			
			iInsertCnt++;
		}//end else */	

		if (iRecCnt%10000 == 0)
			Log::log(0,"�Ѹ���%d��.\n",iRecCnt);
	} //end while qry.next

	qry.close();

	Log::log(0,"data buffer total:%d,used :%d \n",m_poE8AccountData->getTotal(),m_poE8AccountData->getCount());
	Log::log(0,"index buffer total:%d,used :%d \n",m_poE8AccountIndex->getTotal(),m_poE8AccountIndex->getCount());
	return;

}

#ifdef  USERINFO_REUSE_MODE
void  E8UserCtl::revokeE8User()
{		 
	
	const char * skey;
	unsigned int uiKeyOffset=0;
	unsigned int uiIndex=0;
	char countbuf[32];
	int 	iNum = 0;	

	int iKickCnt=0;
	char sLimitDate[16];//����һ������
	iKickCnt = 0;
	if(!ParamDefineMgr::getParam(USERINFO_LIMIT, E8_USER, countbuf))
	{
		Log::log(0,"�����ڱ�b_param_define������E8�������ڴ��С����.");
		THROW(MBC_E8UserCtl+60);
	}
	iNum = atoi(countbuf);	
	getBillingCycleOffsetTime(iNum,sLimitDate);
	SHMStringTreeIndex_A::Iteration iter = m_poE8AccountIndex->getIteration();
	DataE8AccountInfo * pp = (DataE8AccountInfo *)(*m_poE8AccountData);    
	if ( !pp ) {Log::log(0,"��ȡ������ʧ��!");return;}
		/*
		while(  iter.next(skey, uiKeyOffset) ){//����������,�ҳ�key
			uiIndex     = uiKeyOffset;
			uiKeyOffset = pp[uiKeyOffset].m_iNextOffset;//������ƫ����
			while(uiKeyOffset){
				if( strncmp(pp[uiKeyOffset].sExpDate, sLimitDate, 8)<0 ){
					unsigned int  iTtmp;
					m_poE8AccountIndex->revokeIdx(pp[uiKeyOffset].sUserName , iTtmp);
					m_poE8AccountData->revoke(uiKeyOffset);                        
					iKickCnt++;                        
				}
				uiKeyOffset = pp[uiKeyOffset].m_iNextOffset;
			}
			
                /////��һ���ڵ����Ƿ���Ҫɾ��
		if( strncmp(pp[uiIndex].sExpDate, sLimitDate, 8)<0 ){
			unsigned int iTmp=pp[uiIndex].m_iNextOffset;
			if( 0 == iTmp){
				iter.removeMe();
			}else{
				iter.setValue( iTmp );
			}
			m_poE8AccountIndex->revokeIdx(pp[uiIndex].sUserName , iTmp);
			m_poE8AccountData->revoke(uiIndex);
                    
			iKickCnt++;
		}
		if ( ( iKickCnt % 1000 ) == 0 && iKickCnt)
		  Log::log(0,"�Ѵ�������ڴ� %d\n",iKickCnt);

	}////end while iter.next
	cout<<"kick data_user:"<< iKickCnt<<"."<<endl;
*/
            while( iter.next(skey, uiKeyOffset) ){
                unsigned int uiTemp=0;
                bool bKickSelf=false;
                
                if( uiKeyOffset ){
                    iKickCnt += m_poE8AccountData->kickAndRevoke(
                        uiKeyOffset,
                        offsetof(DataE8AccountInfo, m_iNextOffset),
                        offsetof(DataE8AccountInfo, sExpDate),  sLimitDate,
                        -1,
                        -1,
                        &uiTemp,
                        &bKickSelf);
                    if( bKickSelf ){
                        if(0 == uiTemp)
                            iter.removeMe();
                        else
                            iter.setValue(uiTemp);
                    }

                }
								if ( ( iKickCnt % 1000 ) == 0 && iKickCnt)
		  						Log::log(0,"�Ѵ�������ڴ� %d\n",iKickCnt);                
            }///end while iter.next
            cout<< "kick "<< "data_user" <<":"<< iKickCnt<<"."<<endl;

	return ;
   
}
#endif

bool E8UserCtl::updateE8UserInfo(DataE8AccountInfo *pData,char *sAction)
{
	DataE8AccountInfo *p;
	unsigned int i=0,k=0;
	bool bFind=false;

	p = (DataE8AccountInfo *)(*m_poE8AccountData);
	m_poE8AccountIndex->get(pData->sUserName,&k);
	if (strcmp(sAction,"I")==0)   //��־��Ϊ������ʶ,real��־�е�����������ԭ�е�update
	{
		i = m_poE8AccountData->malloc();
		memcpy(&p[i],pData,sizeof(DataE8AccountInfo));
		linkUserName(p[i],i);
		return true;
		/*
		if (k)
		{
			Log::log(0,"�û�id:%ld,seq:%d�Ѵ��ڣ��ӿڱ�ʶΪI������ӿڴ���");
			return false;
		}
		else
		{
			i = m_poE8AccountData->malloc();
			memcpy(&p[i],pData,sizeof(DataE8AccountInfo));
			m_poE8AccountIndex->add(pData->sUserName,i);
			return true;
		}*/
	}
	else if (strcmp(sAction,"U") == 0 )  //��־��Ϊ�޸ı�ʶ��������ʶ��������
	{
		if (!k) 
		{
			Log::log(0,"�û�id:%ld,seq:%d�����ڣ��ӿڱ�ʶΪU������ӿڴ���",pData->lUserID,pData->iUserSeq);
			return false;
		}
		else
		{
			while (k) 
			{
				if (p[k].lUserID == pData->lUserID && p[k].iUserSeq == pData->iUserSeq)
				{
					strcpy(p[k].sAreaCode,pData->sAreaCode);
					strcpy(p[k].sEffDate,pData->sEffDate);
					strcpy(p[k].sExpDate,pData->sExpDate);
					strcpy(p[k].sState,pData->sState);
					strcpy(p[k].sStateDate,pData->sStateDate);
					bFind = true;
					return true;
				}
				else
					k = p[k].m_iNextOffset;
			}
			if (!bFind)
			{
				Log::log(0,"�û�id:%ld,seq:%d�����ڣ��ӿڱ�ʶΪU������ӿڴ���",pData->lUserID,pData->iUserSeq);
				return false;
			}
		}
	}		

	return true;
}


E8UserMgr::E8UserMgr():DataEventMemBase()
{

}

E8UserMgr::E8UserMgr(int iGroup):DataEventMemBase(iGroup)
{
	if (!m_bAttached) {
		THROW(MBC_E8UserCtl+4);
	}
	
	bindData();
	vE8UserList.clear();
}

E8UserMgr::~E8UserMgr()
{
	vE8UserList.clear();
}

int E8UserMgr::percentOfMemUsed(int iMemMode)
{
	unsigned int iPercent,iUsed,iTotal;

	
	if (iMemMode == DATA_BUFFER)
	{		
		iUsed = m_poE8AccountData->getCount();
		iTotal = m_poE8AccountData->getTotal();
		iPercent = iUsed*100/iTotal;
		//iPercent = (m_poE8AccountData->getCount()/m_poE8AccountData->getTotal()) * 100;
		printf("���������ڴ�ʹ�����������[%d],��ʹ��[%d],ʹ�ñ���[%d]\n",m_poE8AccountData->getTotal(),m_poE8AccountData->getCount(),iPercent);		
	}
	
	if (iMemMode == INDEX_BUFFER)
	{
		iUsed = m_poE8AccountIndex->getCount();
		iTotal = m_poE8AccountIndex->getTotal();
		iPercent = iUsed*100/iTotal;
		//iPercent = (m_poE8AccountIndex->getCount()/m_poE8AccountIndex->getTotal()) * 100;
		printf("���������ڴ�ʹ�����������[%d],��ʹ��[%d],ʹ�ñ���[%d]\n",m_poE8AccountIndex->getTotal(),m_poE8AccountIndex->getCount(),iPercent);
	}

	return iPercent;
	
}


void E8UserMgr::checkMemUse()
{
	percentOfMemUsed(DATA_BUFFER);
	percentOfMemUsed(INDEX_BUFFER);
}

void E8UserMgr::dispUserInfo(char *sUserName)
{
	unsigned int k=0;
	DataE8AccountInfo *p;

	p = (DataE8AccountInfo *)(*m_poE8AccountData);
	
	if (m_poE8AccountIndex->get(sUserName,&k)) 
	{
		while(k)
		{
			printf("username:%s,index:%d\n",sUserName,k);
			printf("user_id:%ld,user_seq:%d,area_code:%s,state_date:%s\n",p[k].lUserID,p[k].iUserSeq,p[k].sAreaCode,p[k].sStateDate);
			printf("eff_date:%s,exp_date:%s,state:%s\n\n",p[k].sEffDate,p[k].sExpDate,p[k].sState);
			k = p[k].m_iNextOffset;
		}
	}
	else
	{
		printf("������û���%s���ڴ��в�����.\n",sUserName);
	}
}

void E8UserMgr::dispAll()
{
	unsigned int i;
	DataE8AccountInfo *p;

	p = (DataE8AccountInfo *)(*m_poE8AccountData);

	for (i=1;i<=m_poE8AccountData->getCount();i++)
	{
		printf("username:%s,index:%d\n",p[i].sUserName,i);
		printf("user_id:%ld,user_seq:%d,area_code:%s,StateDate:%s\n",p[i].lUserID,p[i].iUserSeq,p[i].sAreaCode,p[i].sStateDate);
		printf("eff_date:%s,exp_date:%s,state:%s\n\n",p[i].sEffDate,p[i].sExpDate,p[i].sState);
	}
}

/*void E8UserMgr::insMonitorInterface(char *sInfo,char *sRcvAlarmAccNbr)
{
	char sSql[501];

	try
	{
		DEFINE_QUERY(qry);

		sprintf(sSql,"INSERT INTO ISSU_NOTE_LISTING("
			"		NOTE_ID,"
			"		WORK_ORDER_NBR,"
			"		NOTE,"
			"		OBJ_NBR,"
			"		AREA_CODE,"
			"		STAFF_ID,"
			"		SEND_TIME,"
			"		DEAL_FLAG,"
			"		DEAL_DATE,"
			"		MSGFORMAT,"
			"		MSGLEVEL,"
			"		SERVICEID,"
			"		SMGP_SEND_ID) "
			"VALUES (ISSU_NOTE_LISTING_ID_SEQ.NEXTVAL,"
			"		NULL,"
			"		:TMPINFO,"
			"		:ALARM_ACCNBR,"
			"		'025',"
			"		2,"
			"		sysdate,"
			"		0,"
			"		sysdate,"
			"		15,"
			"		1,"
			"		'025',"
			"		1)");

		qry.setSQL(sSql);
		qry.setParameter("TMPINFO",sInfo);
		qry.setParameter("ALARM_ACCNBR",sRcvAlarmAccNbr);
		qry.execute();
		qry.commit();
		qry.close();
	}
	catch (TOCIException &e) {
		Log::log(0,"e8�ڴ�澯����澯�������������:%s\n,sql:%s",e.getErrMsg(),e.getErrSrc());
		throw e;
	}
}*/

void E8UserMgr::alarmMemUser(char *sRcvAlarmAccNbr)
{
	int iPercent;
	char sAlarmInfo[101];

	iPercent = this->percentOfMemUsed(DATA_BUFFER);
	if (iPercent > 97)
	{
		sprintf(sAlarmInfo,"e8�����ڴ�������ʹ�����Ѵﵽ%d��������ڴ��С",iPercent);
		insMonitorInterface(sAlarmInfo,sRcvAlarmAccNbr);
	}

	iPercent = percentOfMemUsed(INDEX_BUFFER);
	if (iPercent > 97)
	{
		sprintf(sAlarmInfo,"e8�����ڴ�������ʹ�����Ѵﵽ%d��������ڴ��С",iPercent);
		insMonitorInterface(sAlarmInfo,sRcvAlarmAccNbr);
	}
}

bool E8UserMgr::getE8User(DataE8AccountInfo &E8Info,char *e8Account,char *sAreaCode,char *sStartTime,char cGetMode)
{
	unsigned int i=0,k=0;
	DataE8AccountInfo *p;
	

	char sTempNbr[MAX_BILLING_NBR_LEN];
	memset(sTempNbr,0,MAX_BILLING_NBR_LEN);
	
	strcpy(sTempNbr,e8Account);

	if (!(m_poE8AccountIndex->get(sTempNbr,&k)))
		return false;
	
	p = (DataE8AccountInfo *)(*m_poE8AccountData);
	while (k) {
		if (strcmp(sAreaCode,p[k].sAreaCode) ==0 )
		{
			//�Ƿ����ͨ��ʱ���ж���YΪ�ж���NΪ���ж���ȡƥ�䵽�ĵ�һ���ڵ���Ϣ����
			//�Ƿ��жϵĲ�����B_PARAM_DEFINE���ж���
			// 20070212 ʡ��˾Ҫ����Ҫ�ж���Чʱ�����Ч״̬��ֻҪ������������㣬����Ԫ���п���
			if (cGetMode == 'Y'|| cGetMode == 'y') { 
				if (strcmp(sStartTime,p[k].sEffDate) <0 || strcmp(sStartTime,p[k].sExpDate)>0) {
					k = p[k].m_iNextOffset;
					continue;
				}
			}			
			E8Info.lUserID = p[k].lUserID;
			E8Info.iUserSeq = p[k].iUserSeq;
			strcpy(E8Info.sUserName,p[k].sUserName);
			strcpy(E8Info.sEffDate,p[k].sEffDate);
			strcpy(E8Info.sExpDate,p[k].sExpDate);
			return true;
		}
		else
			k = p[k].m_iNextOffset;
	}

	if (!k) 
		return false;
	else
		return true;
}

void E8UserMgr::inTableCommit(char *sEndDay,char *sExpTabName)
{
	char sSql[501]={0};
	char sEnd_Day[9]={0};

	strcpy(sEnd_Day,sEndDay);
	sEnd_Day[8]=0;
	
	if(vE8UserList.size() == 0)
		return;
	
	DEFINE_QUERY(qry);
	sprintf(sSql," insert into %s (user_id,user_name,area_code,state_date,eff_date,exp_date,end_date,in_date) "  //end_date,
		" values (:USERID,:USERNAME,:AREACODE,TO_DATE(:STATEDATE,'yyyymmddhh24miss'),TO_DATE(:EFFDATE,'yyyymmddhh24miss'),to_date(:EXPDATE,'yyyymmddhh24miss'),"
		" to_date(%s,'yyyymmddhh24miss'),SYSDATE)",sExpTabName,sEnd_Day);  //
	qry.setSQL(sSql);
	qry.setParamArray("USERID",&(vE8UserList[0].lUserID),sizeof(vE8UserList[0]));
	qry.setParamArray("USERNAME",(char **)&(vE8UserList[0].sUserName),sizeof(vE8UserList[0]),sizeof(vE8UserList[0].sUserName));
	qry.setParamArray("AREACODE",(char **)&(vE8UserList[0].sAreaCode),sizeof(vE8UserList[0]),sizeof(vE8UserList[0].sAreaCode));
	qry.setParamArray("STATEDATE",(char **)&(vE8UserList[0].sStateDate),sizeof(vE8UserList[0]),sizeof(vE8UserList[0].sStateDate));
	qry.setParamArray("EFFDATE",(char **)&(vE8UserList[0].sEffDate),sizeof(vE8UserList[0]),sizeof(vE8UserList[0].sEffDate));
	qry.setParamArray("EXPDATE",(char **)&(vE8UserList[0].sExpDate),sizeof(vE8UserList[0]),sizeof(vE8UserList[0].sExpDate));
	//qry.setParamArray("ENDDAY",(char **)&(sEnd_Day),sizeof(sEnd_Day),sizeof(sEnd_Day));
	qry.execute(vE8UserList.size());
	qry.commit();
	qry.close();
	vE8UserList.clear();
}

void E8UserMgr::expUserInfo(char *sEndDay,char *sExpTabName)
{
	unsigned int i,iSumCnt=0;
	DataE8AccountInfo *p;
	Date dEndDay;
	
	dEndDay.parse(sEndDay,"YYYYMMDD");
	p = (DataE8AccountInfo *)(*m_poE8AccountData);
	vE8UserList.clear();

	for(i=1; i<=m_poE8AccountData->getCount();i++) 
	{		
	//	dStateDay.parse(p[i].sStateDate,"YYYYMMDD");
	//	if ( dStateDay<= dEndDay )
//		{
			DataE8AccountInfo poTmp;
			memcpy(&poTmp,&p[i],sizeof(DataE8AccountInfo));

			vE8UserList.push_back(poTmp);			
//			cout<<poTmp.lUserID<<":"<<poTmp.sUserName<<":"<<poTmp.sAreaCode<<":"
//				<<poTmp.sStateDate<<":"<<poTmp.sEffDate<<":"<<poTmp.sExpDate<<endl;
			iSumCnt++;
			if (vE8UserList.size() % COMMIT_BUFFER == 0)
			{
				inTableCommit(sEndDay,sExpTabName);
				Log::log(0,"�����%ld��",iSumCnt);
			}
//		}
	}
	
	vIntList.clear();
	inTableCommit(sEndDay,sExpTabName);
	Log::log(0,"������,�����%ld����",iSumCnt);
}

void E8UserMgr::cmpMem_TabUserInfo(char *sEndDay,char *sRcvAlarmAccNbr)
{
	char sSql[500]={0},sExpTableName[31]={0},sInfo[201]={0};
	
	sEndDay[8] = 0;

	DEFINE_QUERY(qry);
	if(!ParamDefineMgr::getParam(USERINFO_SEGMENT, E8_EXP_TABLE, sExpTableName))
	{
		Log::log(0,"�����ڱ�b_param_define������E8����ÿ�մ��ڴ浼�����ݵı���.");
		THROW(MBC_E8UserCtl+51);
	}

	Log::log(0,"����м��%s�е�����.",sExpTableName);
	sprintf(sSql,"truncate table %s ",sExpTableName);
	qry.setSQL(sSql);
	qry.execute();
	qry.commit();
	qry.close();

	Log::log(0,"��ʼ���ڴ浵������ʱ��%s",sExpTableName);
	expUserInfo(sEndDay,sExpTableName);

	Log::log(0,"��ʼ�ȶ�...");
	sprintf(sSql,"insert into b_cmp_E8_result(user_name,end_date,in_date)"
				" select user_name,to_date(%s,'yyyymmdd'),sysdate from ("
				" select user_name from Data_user "
				" where state_date<to_date(%s,'yyyymmdd')"
				" minus select user_name from %s )",sEndDay,sEndDay,sExpTableName);
	qry.setSQL(sSql);
	qry.execute();
	qry.commit();
	qry.close();
	
	sprintf(sSql," select count(1) cnt from b_cmp_E8_result "
				" where end_date = to_date(%s,'yyyymmdd')",sEndDay);
	qry.setSQL(sSql);
	qry.open();
	qry.next();
	int iCnt = qry.field(0).asInteger();

	if (iCnt>0)
	{		
		Log::log(0,"�澯��¼����ӿڱ�");
		sprintf(sInfo,"���ֱȶԵ�%sǰ��E8��������%d�����ڴ��в����ڣ�������鿴b_cmp_E8_result��.",sEndDay,iCnt);
		insMonitorInterface(sInfo,sRcvAlarmAccNbr);		
	}
	else
	{
		Log::log(0,"%s��E8�����ȶ�û�в��",sEndDay);
	}
	qry.close();
}

bool E8UserCtl::getBillingCycleOffsetTime(int iOffset,char * strDate)
{
    Date d;
    char sql[1024];
    memset(sql,0,sizeof(sql));      
    DEFINE_QUERY (qry);
    iOffset = iOffset -1;
    sprintf(sql, "select to_char(add_months(cycle_begin_date,-%d),'yyyymmddhh24miss') begin_date FROM billing_cycle where "
            " billing_cycle_type_id=1 AND to_date(%s,'yyyymmddhh24miss')>=cycle_begin_date and "
            " to_date(%s,'yyyymmddhh24miss')<cycle_end_date ",iOffset,d.toString(),d.toString());
    qry.setSQL (sql);
    qry.open ();
    qry.next();
    strcpy(strDate,qry.field(0).asString());
    qry.commit();
    qry.close ();
    return true;
}

