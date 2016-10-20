// Copyright (c) 2010,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "DccCheckRecord.h"
#include "Exception.h"
#include "Log.h"
#include "ParamDefineMgr.h"
#include "Environment.h"
#include "RecordCheck.h"


using namespace std;
DEFINE_MAIN (DccCheckRecord)

DccCheckRecord::DccCheckRecord()
{
	vecEVENTATTRID.clear();
	m_totalNum=0;
	m_iBillTypeID=-1;
	m_struPackageHead.m_sSessionId.clear(); 	  
	m_struPackageHead.m_sHopByHop.clear();	  
	m_struPackageHead.m_sEndToEnd.clear();	  
	m_struPackageHead.m_sAuthApplicationId.clear();
	m_struPackageHead.m_sCCRequestType.clear();  
	m_struPackageHead.m_sCCRequestNumber.clear(); 
	m_struPackageHead.m_sServiceContextId.clear();	  
	memset (&m_oStdEvent,0,sizeof(StdEvent));
	getBuffSize();
	getSign();  
	m_seviceInformationToCCA="";
	m_strToCCA="";
	m_iGroup_Id=0;
	
	headStdEvent=new StdEvent;
  	if (!headStdEvent) 
      THROW (MBC_EventFormat+1);
	
	m_poFormatMgr = new AttrTransMgr(m_iProcID);
  	if (!m_poFormatMgr) 
      THROW (MBC_EventFormat+3);


}

DccCheckRecord::~DccCheckRecord()
{
	delete headStdEvent;
	if(headStdEvent)
		headStdEvent=NULL;

	delete m_poFormatMgr;
	if(m_poFormatMgr)
		m_poFormatMgr=NULL;
}

int DccCheckRecord::run()
{
	DenyInterrupt();
	Log::log(0, "dccCheckRecord run ......");
	loadTransFileGroupid();
	
	char *cBuffer=new char[m_iBuffSize];
	memset(cBuffer, 0, m_iBuffSize);
	
	while (!GetInterruptFlag())
	{
		if(!m_poReceiver->receive(cBuffer,false))//int receive(char *buffer, bool ifBlock = true); ����Ϣ���н���buffer��buffer��СΪStdEvent������size��
		{
			sleep(2);
			continue;
		}
		m_strToCCA="";
		m_seviceInformationToCCA="";
		int iresult=analysisStr(cBuffer);
	  	if(iresult==-1)
	  	{
	      	Log::log(0,"�����ַ���ʧ�ܣ�");
	      	cout<<"�����ַ���ʧ�ܣ�"<<endl;
			AddTicketStat (TICKET_ERROR);
	      	continue;
	  	}
		AddTicketStat(TICKET_NORMAL);

	}
	delete[] cBuffer;
	return true;
}

int DccCheckRecord::analysisStr(char *cgetBuffStr)
{
	m_struPackageHead.m_sSessionId.clear(); 	  
	m_struPackageHead.m_sHopByHop.clear();	  
	m_struPackageHead.m_sEndToEnd.clear();	  
	m_struPackageHead.m_sAuthApplicationId.clear();
	m_struPackageHead.m_sCCRequestType.clear();  
	m_struPackageHead.m_sCCRequestNumber.clear(); 
	m_struPackageHead.m_sServiceContextId.clear();	  
	
	string strBuff(cgetBuffStr,m_iBuffSize);
	if(strBuff.length()==0)
	{
		cout<<"strBuff is empty!"<<endl;
		Log::log(0,"strBuff is empty!");
		return -1;
	}

	string strTemp = strBuff.substr(0,sizeof(StdEvent)); //ȡ��ǰ���stdEvent,�ŵ�strTemp����
  	
    strBuff.erase(0,sizeof(StdEvent));       //ɾ����ǰ���stdEvent
	
  	vecEVENTATTRID.clear();
  	m_totalNum=0;
  	 Log::log(0,"���յ����ַ���=%s",strBuff.c_str());
	cout<<strBuff<<endl;
  	int iresult=getEventAttr(vecEVENTATTRID,m_totalNum,strTemp,strBuff);  //ȡ������Ϣ
	if(iresult==-2)
  	{
      	Log::log(0,"���յ��Ĳ���CCR����!");
      	return -2;
  	}
	if(iresult==-1)
  	{
      	Log::log(0,"ȡvecEVENTATTRID������Ϣʧ��!");
      	return -1;
  	}

	char *testToCCA=new char[m_iBuffSize];
	memset (testToCCA,0,m_iBuffSize);
	memcpy(testToCCA,(char*)headStdEvent,sizeof(StdEvent));

    strBuff=m_sSignTotal+strBuff;                     // ��"|",�������Ĵ���    
	//cout<<strBuff<<endl;
	
	int pos1=0;
	int pos2=0;
	int recordPos1=strBuff.find(m_sSignRecord,0);    //^^^��ʼ��λ��
	if(recordPos1==-1) //���û�л�����¼
	{
		formSeviceInformationToCCA(0);
	}
	
	int recordPos2=0;                        //^^^һ������������λ��
	int i=0;
	string strGet;                           //��ȡ�����ַ���
	
	while( ( (pos2=strBuff.find(m_sSignTotal,pos1+1))!=-1)&&( (pos2<recordPos1)||(recordPos1==-1) )  )
	{
		strGet=strBuff.substr(pos1+1,pos2-pos1-1);
		//cout<<strGet<<endl;
		pos1=pos2;
		i++;
		switch(i)
		{
			case 1:
				m_struPackageHead.m_sSessionId=strGet;
				break;
			case 2:
				m_struPackageHead.m_sHopByHop=strGet;
				break;
			case 3:
				m_struPackageHead.m_sEndToEnd=strGet;
				break;
			case 4:
				m_struPackageHead.m_sAuthApplicationId=strGet;
				break;
			case 5:
				m_struPackageHead.m_sCCRequestType=strGet;
				break;
			case 6:
				m_struPackageHead.m_sCCRequestNumber=strGet;
				break;
			default:
				m_struPackageHead.m_sServiceContextId=strGet;
				break; 
		}
	}
	if(recordPos1!=-1)  //ȷ���л�����¼
	{
		strGet=strBuff.substr(pos1+1,recordPos1-pos1-1);
		m_struPackageHead.m_sServiceContextId=strGet;   //��ŵ��ṹ��
		
		//cout<<m_struPackageHead.m_sServiceContextId<<endl;
		
		while( ( recordPos2=strBuff.find(m_sSignRecord,recordPos1+3))!=-1  )
		{
			strGet=strBuff.substr(recordPos1+3,recordPos2-recordPos1-3);   //ȡ��һ��������¼
			recordPos1=recordPos2;
			if( strGet.length()!=0 ) //����л�����¼
				int result=DealOneRecord(strGet);
			else
			{
				delete[] testToCCA;
				return true;			 //û�л�����¼û������
			}
		}
		//ȡ���һ��������¼
		strGet=strBuff.substr(recordPos1+3);   
		if( strGet.length()!=0 )     //����л�����¼
		{
			int result=DealOneRecord(strGet);
			if(result==-1)
			{
				delete[] testToCCA;
				return -1;
			}
		}
		else
		{
			delete[] testToCCA;
			return true;             //û�л�����¼û������
		}
		
	}
	formStrToCCA();                //����CCA��
	
	memcpy(testToCCA+sizeof(StdEvent),m_strToCCA.c_str(),m_strToCCA.length());
	cout<<"testToCCA="<<testToCCA+sizeof(StdEvent)<<endl;
	m_poSender->send(testToCCA);
	delete[] testToCCA;
	return true;
}

//ҵ���߼�������͸����������Ҫ���CCA����
//StdEvent��billingNbr�д�Ű����ͣ�CCR/CCA  StdEvent��m_iBillingTypeID�д��ҵ�����ͣ���TransFileTypeID����Ӧ����Ҫ��У���ʱ����������Ϣ
//<Session-Id>|{hop_by_hop}|{end_to_end}|{Auth-Application-Id}|{CC-Request-Type}|{CC-Request-Number}^^^Service-Result-Code|Para-Field-Result^^^...^^^Service-Result-Code|Para-Field-Result
void DccCheckRecord::formStrToCCA()    //����CCA��
{        
	
	m_strToCCA=m_struPackageHead.m_sSessionId+m_sSignTotal+m_struPackageHead.m_sHopByHop+m_sSignTotal+m_struPackageHead.m_sEndToEnd+m_sSignTotal+m_struPackageHead.m_sAuthApplicationId+m_sSignTotal+m_struPackageHead.m_sCCRequestType+m_sSignTotal+m_struPackageHead.m_sCCRequestNumber+m_seviceInformationToCCA;
  Log::log(0,"���͵�CCA��=%s",m_strToCCA.c_str());
	//cout<<"m_strToCCA="<<m_strToCCA<<endl;

}

//void EventToolKits::set_param( StdEvent *pStd, int iEventAttrId, string strAttr )
//     StdEvent *pStd        �����õ��¼�
//     int iEventAttrId      ����ID
//     string strAttr        ����ֵ
int	DccCheckRecord::DealOneRecord(string strOneRecord)
{
	int pos1=0;
	int pos2=0;
	int i=1;
	string strField,resultstrField;             //��ȡ�����ַ���
	memset (&m_oStdEvent,0,sizeof(StdEvent));
	
	pos2=strOneRecord.find(m_sSignTotal,0);
	if(pos2==-1)                //�ü�¼ֻ��һ���ֶ�
	{
 	Log::log(0,"�ü�¼ֻ��һ���ֶ�");
		return -1;
	}else
	{
		strField=strOneRecord.substr(0,pos2);// ȡ��һ���ֶ�
		pos1=pos2;
		resultstrField.clear();
		replaceSign(strField,resultstrField);
		m_evnetToolKits.set_param(&m_oStdEvent,vecEVENTATTRID.at(0),resultstrField);
		//cout<<"i="<<1<<endl;
		//cout<<"strField="<<strField<<endl;
	}
	
	while( (( pos2=strOneRecord.find(m_sSignTotal,pos1+1))!=-1)&&(i<m_totalNum) )
	{
		strField=strOneRecord.substr(pos1+1,pos2-pos1-1);  //ȡÿ���ֶ�
		resultstrField.clear();
		replaceSign(strField,resultstrField);
		m_evnetToolKits.set_param(&m_oStdEvent,vecEVENTATTRID.at(i),resultstrField);

		//cout<<"i="<<i<<endl;
		//cout<<"strField="<<strField<<endl;
		pos1=pos2;
		i++;
	}
	if( (pos2==-1)&&(i<m_totalNum) )
	{
		strField=strOneRecord.substr(pos1+1);  //ȡ���һ���ֶ�
		resultstrField.clear();
		replaceSign(strField,resultstrField);
		m_evnetToolKits.set_param(&m_oStdEvent,vecEVENTATTRID.at(i),resultstrField);
		//cout<<"i="<<i<<endl;
		//cout<<"strField="<<strField<<endl;
	}else	
	{
		Log::log(0,"b_asn1_field��b_asn1_tag_seq��������ֶ��� С�� ���ܵ���¼���ֶ���");
		return -1;
	}	

	checkRecord();                //��¼��У��
  	formSeviceInformationToCCA(m_oStdEvent.m_iErrorID); //����CCA���е�{Service-Information}	ҵ����Ϣ  ���ֵ�����

	return true;
}

//��¼��У��
int DccCheckRecord::checkRecord()
{
	m_oStdEvent.m_oEventExt.m_iSourceEventType=1;
	m_oStdEvent.m_iGroupID=m_iGroup_Id;

	m_oStdEvent.m_iFileID=getNextFileID();
	strcpy(m_oStdEvent.m_oEventExt.m_sFileName,(m_struPackageHead.m_sServiceContextId).c_str());

	setSessionIdToStdEvent(m_struPackageHead.m_sSessionId);  //���session_id
    m_oStdEvent.m_lEventID = getNextEventID();
	
    m_poFormatMgr->trans(&m_oStdEvent); //ִ���¼�����

	m_poSender->send(&m_oStdEvent);
	SendFileEndEvt(m_oStdEvent.m_iFileID);  //�����ļ������źź�ǿ���ύ�¼�
	return true;

}

void DccCheckRecord::setSessionIdToStdEvent(string strSessionId)
{
	char tmp[32];
	int ieventID=0;
	string resultstrSessionId;
	memset( tmp,0,sizeof(tmp) );
	
	if( ParamDefineMgr::getParam("DCC", "SESSION_ID", tmp,32) )
	{
	    ieventID=atoi(tmp);
 	}else
  	{
  		ieventID=252;
   		Log::log(0,"b_param_define���DCC/SESSION_ID����û������,ȡĬ��ֵ��");
  	}	
  resultstrSessionId.clear();
	replaceSign(strSessionId,resultstrSessionId);
	m_evnetToolKits.set_param(&m_oStdEvent,ieventID,resultstrSessionId);

}

//����CCA���е�{Service-Information}	ҵ����Ϣ  ���ֵ�����
//^^^Service-Result-Code|Para-Field-Result^^^...^^^Service-Result-Code|Para-Field-Result
void DccCheckRecord::formSeviceInformationToCCA(int errID)
{
	char cerrorID1[64];
	memset (cerrorID1,0,64);
	sprintf(cerrorID1,"%d",errID);

	char cerrorID2[64];
	memset (cerrorID2,0,64);
	sprintf(cerrorID2,"%03d",errID);   //����

	m_seviceInformationToCCA=m_seviceInformationToCCA+m_sSignRecord;
	if(errID==0)
	{
		m_seviceInformationToCCA=m_seviceInformationToCCA+"0";
		m_seviceInformationToCCA=m_seviceInformationToCCA+m_sSignTotal;
		m_seviceInformationToCCA=m_seviceInformationToCCA+"0";
	}else
	{
		m_seviceInformationToCCA=m_seviceInformationToCCA+cerrorID1;
		m_seviceInformationToCCA=m_seviceInformationToCCA+m_sSignTotal;
		m_seviceInformationToCCA=m_seviceInformationToCCA+ "F"+cerrorID2;
	}
}

//������ǰ���stdEvent,�ҵ�iEventAttrId����
//trans_file_groupid ��,������vector<int> vecEventAttrId  ����event_attr_id,  icount��vecEventAttrId�еļ�¼����
int DccCheckRecord::getEventAttr(vector<int>& vecEventAttrId,int& icount,string headString,string strBuff)
{
	//memset(headStdEvent,0,sizeof(StdEvent));
	memset(str,0,sizeof(StdEvent));
	memcpy(str,headString.c_str(),sizeof(StdEvent));

	headStdEvent=(StdEvent*)str;         //��ǰ����ַ���ǿ��ת����StdEvent
	m_iBillTypeID=headStdEvent->m_iBillingTypeID;
	if( strcmp(headStdEvent->m_sBillingNBR,"CCR")!=0 )
	{
	
		writeDccLog("A",headStdEvent->m_sBillingNBR,m_iBillTypeID,(char*)strBuff.c_str());
		return -2;                                                  //�������Ĳ���CCR����
	}
	
	int iGroupId=getGroupIdByTransFileTypeId(m_iBillTypeID);        //����TRANS_FILE_TYPE_ID��ȡGROUP_ID,TRANS_FILE_SERVICE_CONTEXT��
	
	if(iGroupId==-1)
	{
	  	Log::log(0,"û������TRANS_FILE_TYPE_ID=%d��GROUP_ID�Ĺ�ϵ,����trans_file_groupid�������",m_iBillTypeID);
		writeDccLog("B",headStdEvent->m_sBillingNBR,m_iBillTypeID,(char*)strBuff.c_str());
		return -1;
	}else{
		icount=getEventAttrByGroupId(vecEventAttrId,iGroupId);      //����iGroupId��ȡEventAttrId �� b_asn1_field,b_asn1_tag_seq
		strcpy(headStdEvent->m_sBillingNBR,"CCA");
		if(icount==0)
		{
			Log::log(0,"����GroupId=%d��ȡEventAttrIdʧ��,����ģ������",iGroupId);
			return -1;
		}else
		   	return icount;
	}
}

int DccCheckRecord::getGroupIdByTransFileTypeId(int iTransFileTypeId)  //����TRANS_FILE_TYPE_ID��ȡGROUP_ID,����TRANS_FILE_SERVICE_CONTEXT��,����iGroupId
{
	TRANSFILEMAP::iterator pos=m_mapTransFileGroupid.find(iTransFileTypeId);
	if(pos==m_mapTransFileGroupid.end())
		return -1;
	else
	{
		m_iGroup_Id=(*pos).second;
		Log::log(0,"������¼:Billfiletype=%d,GroupId=%d",iTransFileTypeId,m_iGroup_Id);
		return m_iGroup_Id;
	}
}

//���ػ�õ��ֶγ���
int DccCheckRecord::getEventAttrByGroupId(vector<int> &vecEventid,int iGroupId)  //����iGroupId��ȡEventAttrId �� b_asn1_field,b_asn1_tag_seq
{
	vecEventid.clear();
	EVENTATTRIDMAP::iterator pos=m_mapEventAttrId.find(iGroupId);
	if(pos==m_mapEventAttrId.end())
		return -1;
	else
	{
		vecEventid=(*pos).second;
		Log::log(0,"������¼��Ӧ��eventattrid��%d��",vecEventid.size());
		return vecEventid.size();
	}
}

int DccCheckRecord::loadTransFileGroupid()  //װ��trans_file_groupid�����Ϣ
{
	int igroupid,itransFileTypeId;
	int icount=0;
	char sSql[1000];
	memset(sSql,0,1000);
	m_mapTransFileGroupid.clear();
	m_mapEventAttrId.clear();

	sprintf(sSql,
		" select trans_file_type_id,group_id from trans_file_groupid order by trans_file_type_id");
	DEFINE_QUERY(qry);
	qry.setSQL(sSql);
	qry.open();
	while(qry.next ()) {
		itransFileTypeId=qry.field("trans_file_type_id").asInteger();
		igroupid=qry.field("group_id").asInteger();
		m_mapTransFileGroupid.insert(TRANSFILEMAP::value_type(itransFileTypeId,igroupid));
		loadEventAttrIdByGroupid(igroupid);
		m_mapEventAttrId.insert(EVENTATTRIDMAP::value_type(igroupid,m_vAttrId));
		icount++;
	}	
	qry.close();

	Log::log(0,"���ݿ��������Ϣ����:");
	Log::log(0,"trans_file_groupid��:");
	map<int,int>::iterator iter1;
	for(iter1=m_mapTransFileGroupid.begin();iter1!=m_mapTransFileGroupid.end();iter1++)
	{
		Log::log(0,"%d->%d",(*iter1).first,(*iter1).second);
		cout<<(*iter1).first<<"->";
		cout<<(*iter1).second<<std::endl;
	}

	Log::log(0,"b_asn1_field b_asn1_tag_seq��:");
	EVENTATTRIDMAP::iterator iter2;
	for(iter2=m_mapEventAttrId.begin();iter2!=m_mapEventAttrId.end();iter2++)
	{
		Log::log(0,"%d->",(*iter2).first);

		cout<<(*iter2).first<<"->";
		vector<int>::iterator iter3;
		for(iter3=((*iter2).second).begin();iter3!=((*iter2).second).end();iter3++)
		{
			Log::log(0,"%d",*iter3);
			cout<<*iter3<<endl;
		}
	}
	
	return icount;	
}

int DccCheckRecord::loadEventAttrIdByGroupid(int igroupid)  //װ��event_attr_id����Ϣ
{
	int m_iCount=0;
	m_vAttrId.clear();

	DEFINE_QUERY (qry) ;

	//���SQL���
	char m_sSql[1024];
	sprintf(m_sSql,"select b.event_attr_id from b_asn1_tag_seq a,b_asn1_field b where a.group_id=b.group_id and a.tag=b.tag and a.group_id=%d order by a.seq",igroupid);

	qry.setSQL (m_sSql);

	qry.open ();

	while(qry.next ())
	{
		m_vAttrId.push_back(qry.field(0).asInteger());
		m_iCount++;
	}
	qry.close ();
	return m_iCount;

}

void DccCheckRecord::SendFileEndEvt(int iFileID)
{

/*	�����ļ������ź�	*/
	Date d; 
	StdEvent oFileEndEvent; 
	oFileEndEvent.m_iEventTypeID = FILE_END_EVENT_TYPE;
	oFileEndEvent.m_iFileID = iFileID; 
	strcpy (oFileEndEvent.m_sStartDate, d.toString ());
	m_poSender->send (&oFileEndEvent); 
		
/*	����ǿ���ύ�¼�	*/
	StdEvent oCommitEvent; 
	oCommitEvent.m_iFileID = iFileID; 
	oCommitEvent.m_iEventTypeID = FORCE_COMMIT_EVENT_TYPE; 
	m_poSender->send (&oCommitEvent);	

}

//�����ݿ���л�ȡ���ܵ���Buffstr�Ĵ�С
int DccCheckRecord::getBuffSize()
{
	char sql[1024];
	memset (sql, 0, sizeof(sql));
	sprintf(sql, "SELECT MAX(A.MSG_EVENT_NUM) FROM WF_MQ_ATTR A");

	DEFINE_QUERY (qry);
	qry.setSQL(sql);
	qry.open();
	qry.next();
	int iMsgEventNum = qry.field(0).asInteger();
	m_iBuffSize=(sizeof(StdEvent))*iMsgEventNum;
	qry.close();
	
	return true;
}

//���b_param_define�������õķָ���
void DccCheckRecord::getSign()
{
	char tmp[32];
	memset( tmp,0,sizeof(tmp) );
	m_sSignTotal="";
	m_sSignRecord="";
	
	if( ParamDefineMgr::getParam("DCC", "DELIMITER_CHAR", tmp,32) )
	{
	    m_sSignTotal.assign(tmp);
 	}else
  	{
  		m_sSignTotal="|";
   		Log::log(0,"b_param_define��Ĳ���û������,ȡĬ��ֵ��");
  	}	
	memset( tmp,0,sizeof(tmp) );
	if( ParamDefineMgr::getParam("DCC", "DELIMITER_RECORD", tmp,32) )
	{
	    m_sSignRecord.assign(tmp);
  	}else
  	{
  		m_sSignRecord="^^^";
   		Log::log(0,"b_param_define��Ĳ���û������,ȡĬ��ֵ��");
  	}	
	
}


long DccCheckRecord::getNextFileID()
{
    long    iRet = 0;
    DEFINE_QUERY( query );

    query.setSQL( "select seq_file_id.nextval file_id from dual" );
    query.open();
    query.next();

    iRet = query.field( "file_id" ).asInteger();

    query.close();

    return iRet;
}
long DccCheckRecord::getNextDccLogID()
{
    long    iRet = 0;
    DEFINE_QUERY( query );

    query.setSQL( "select seq_dcc_log.nextval dcc_log_id from dual" );
    query.open();
    query.next();

    iRet = query.field( "dcc_log_id" ).asInteger();

    query.close();

    return iRet;
}

void DccCheckRecord::writeDccLog(char* cLogType,char* cMessageType,int iTransFileTypeId,char* cRemark)
{
	DEFINE_QUERY (qry);
    char sSql[1024];
	long ldccLog=getNextDccLogID();	
    
    sprintf (sSql, "Insert into dcc_log "
        " (log_id, err_type, message_type, trans_file_type_id, remark,log_time ) "
        " values ('%d', '%s', '%s', '%d', '%s', sysdate)",
        ldccLog,cLogType, cMessageType, iTransFileTypeId,cRemark );
    qry.setSQL (sSql);
    qry.execute();
    qry.commit ();
    qry.close ();

}
void DccCheckRecord::replaceSign(string strfield,string& resultStr )
{
	int pos=0;
	while( (pos=strfield.find("^"))!=-1 )
		strfield.replace(pos,1,"");
	resultStr=strfield;
	
}

long DccCheckRecord::getNextEventID()
{
    static long alFlag = 0;
    
    if (!alFlag) 
        getNextEventSeq();
    else
        m_lCurEventID++;
        
    alFlag++;
    alFlag = alFlag%EVENT_ID_INCREMENT;
    
    return m_lCurEventID;
}

long DccCheckRecord::getNextEventSeq()
{
    static TOCIQuery qry(DB_LINK);
    
    qry.close();
    qry.setSQL("select seq_event_id.nextval event_id from dual");
    qry.open();
    
    if (!qry.next() ) THROW(EVENT_ID_INCREMENT);
    
    m_lCurEventID = qry.field("event_id").asLong();
    
    qry.close();
    
    return m_lCurEventID;
}

