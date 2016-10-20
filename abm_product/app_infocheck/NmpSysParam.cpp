/*VER:1*/

#include <string.h>
#include "NmpSysParam.h"
#include "Log.h"
#include "../app_init/MsgDefineClient.h"
#include "../app_init/SysParamDefine.h"
#include "ParamDefineMgr.h"
#include "interrupt.h"

bool NmpSysParamsGet::m_bFirstRun = true;

DEFINE_MAIN(NmpSysParamsGet);

int NmpOIDInfo::getValuesFrmSql()
{

//ȡvalue_id��Ӧ��sql
	DEFINE_QUERY(qry1);
	qry1.setSQL("SELECT VALUE_SQL FROM STAT_NMP_OID_VALUE WHERE VALUE_ID=:VID");
	qry1.setParameter("VID",m_iValueID);
	qry1.open();
	qry1.next();
	strcpy(m_sValueSql,qry1.field(0).asString());
	qry1.close();

//��sql�в�ѯ����ֵ�ŵ�vector��
	qry1.setSQL(m_sValueSql);
	qry1.open();
	m_vecValue.clear();
	while(qry1.next())
	{
		m_vecValue.push_back(qry1.field(0).asString());
	}
	
	qry1.close();

	return 1;
}

Nmp_OID_VALUE::Nmp_OID_VALUE()
{
//	m_iTargetID = 0;
	memset(m_sOIDName,0,sizeof(m_sOIDName));
	memset(m_sOIDValue,0,sizeof(m_sOIDName));
	m_iValueSeq = 0;   //table��oidȡֵ��˳��
	memset(m_sParamSection,0,sizeof(m_sParamSection));
	memset(m_sParamName,0,sizeof(m_sParamName));
	m_iIfIndex = 0;
	m_iValueID =0;
	m_iVisible = 1;
	//m_oSqlValue = NULL;	
};

int Nmp_OID_VALUE::getValuesFrmSql(const char *sMacroName,const char *sMacroValue,char *sParamValue)
{
	//����sql����ĸ�ʽӦΪselect to_char(***) from *** where ***=:MACRONAME;

	//ȡvalue_id��Ӧ��sql
	DEFINE_QUERY(qry1);
	qry1.setSQL("SELECT VALUE_SQL FROM STAT_NMP_OID_VALUE WHERE VALUE_ID=:VID");
	qry1.setParameter("VID",m_iValueID);
	qry1.open();
	qry1.next();
	strcpy(m_sValueSql,qry1.field(0).asString());
	qry1.close();

//��sql�в�ѯ����ֵ�ŵ�vector��
	qry1.setSQL(m_sValueSql);
	qry1.setParameter(sMacroName,sMacroValue);
	qry1.open();
	
	//m_vecValue.clear();
	qry1.next();
	strcpy(sParamValue,qry1.field(0).asString());	
	
	qry1.close();

	return 1;
}

int Nmp_OID_VALUE::getValuesFrmSql(char *sParamValue)
{
	//����sql����ĸ�ʽӦΪselect to_char(***) from *** where ***=:MACRONAME;

	//ȡvalue_id��Ӧ��sql
	DEFINE_QUERY(qry1);
	qry1.setSQL("SELECT VALUE_SQL FROM STAT_NMP_OID_VALUE WHERE VALUE_ID=:VID");
	qry1.setParameter("VID",m_iValueID);
	qry1.open();
	qry1.next();
	strcpy(m_sValueSql,qry1.field(0).asString());
	qry1.close();

//��sql�в�ѯ����ֵ�ŵ�vector��
	qry1.setSQL(m_sValueSql);
	//qry1.setParameter(sMacroName,sMacroValue);
	qry1.open();
	
	//m_vecValue.clear();
	qry1.next();
	strcpy(sParamValue,qry1.field(0).asString());	
	
	qry1.close();

	return 1;
}


NmpOIDMgr::NmpOIDMgr()
{	
	m_mapOIDInfo.clear();
	m_mapOIDValueList.clear();
	m_mapParamNameToOID.clear();
	m_oOIDValue = NULL;	

	memset(m_sAreaCode,0,sizeof(m_sAreaCode));
	memset(m_sOIDName,0,sizeof(m_sOIDName));
}

NmpOIDMgr::~NmpOIDMgr()
{
	unload();
}

int NmpOIDMgr::unload()
{
	map<string,NmpOIDInfo *>::iterator iter;
	for (iter = m_mapOIDInfo.begin();iter!=m_mapOIDInfo.end();iter++)
	{
		NmpOIDInfo *pTmp = iter->second;
		if (pTmp)
		{
			delete pTmp;
			pTmp = NULL;
		}
	}
	m_mapOIDInfo.clear();

	map<string,NmpParamOID *>::iterator iter2;
	for (iter2 = m_mapParamNameToOID.begin();iter2!=m_mapParamNameToOID.end();iter2++)
	{
		NmpParamOID *pTmp = iter2->second;
		if (pTmp)
		{
			delete pTmp;
			pTmp = NULL;
		}
	}
	m_mapParamNameToOID.clear();

	map<string,VEC_NMPOIDVALUE *>::iterator iter3=m_mapOIDValueList.begin();
	for (;iter3 !=m_mapOIDValueList.end();iter3++)
	{
		VEC_NMPOIDVALUE *vec_tmp =iter3->second;
		VEC_NMPOIDVALUE::iterator iter4=vec_tmp->begin();
		for (;iter4!=vec_tmp->end();iter4++)
		{
			Nmp_OID_VALUE *pTmp = (*iter4);
			if (pTmp)
			{
				delete pTmp;
				pTmp = NULL;
			}
		}

		vec_tmp->clear();
	}
	m_mapOIDValueList.clear();
	return 1;
}

int NmpOIDMgr::setCommandComm(CommandCom * pComm)
{
	m_pCommandCom = pComm;
	return 1;
}
	
bool NmpOIDMgr::checkParamName(char *sOIDName,char *sParamName)
{
	char sMacro[21]={0};
	char sParamLeft[51]={0};
	NmpOIDInfo *poOIDInfo = m_mapOIDInfo[sOIDName];	
	strcpy(sMacro,poOIDInfo->m_sOIDMacro);	
	if (strcmp(sMacro,"000") ==0)   //δ�����
	{
		char *p =strchr(sParamName,'$'); 
		if (p)
			return false;
		else
			return true;
	}
	else
	{
		strcpy(sParamLeft,sParamName);
		char *p1 =strchr(sParamLeft,'$');
		while (p1)
		{
			char sMacroFlag1[2]={0},sMacroFlag2[2]={0};			
			strncpy(sMacroFlag1,p1+1,strlen(sMacro));
//���ҵ��ĺ��ʶ$�����Ƿ�����ź꣬������ǣ�����false
			if (strcmp(sMacroFlag1,sMacro) == 0)  
			{
				//���$������ź꣬��Ҫ�жϺ����ַ��Ƿ�Ϊ$
				strncpy(sMacroFlag2,p1+1+strlen(sMacro)+1,1);
				if (strcmp(sMacroFlag2,"$")!=0)
					return false;
				else
				{
					char *p2=p1+2+strlen(sMacro)+1;
					p1 = strchr(p2,'$');
				}					
			}
			else
			{
				return false;
			}
		}

		return true;
	}
}

int NmpOIDMgr::load()
{
	char sSql[1024]={0};

	//��ȡ�����������Ϣ
	if (!ParamDefineMgr::getParam("BSN_NMP_PARAM", "CITY_CODE",m_sAreaCode))
		strcpy(m_sAreaCode,"000");

	VEC_NMPOIDVALUE *pVecOIDValueList;
	NmpOIDInfo *pOIDInfo;
	map<string,VEC_NMPOIDVALUE *>::iterator iter;
	
//��ȡoid�����͵���Ϣ
	DEFINE_QUERY(qry);
	sprintf(sSql," SELECT b.oid_name,b.oid_type,nvl(b.oid_macro,'000'),nvl(b.value_id,-1),nvl(c.VALUE_SQL,'000'), a.target_id "
			" FROM STAT_NMP_TARGET_DEF a,STAT_NMP_TARGET_OID_DEF b,STAT_NMP_OID_VALUE c  "
			" WHERE a.target_id=b.target_id "
			"	and b.value_id=c.value_id(+)"
			"      AND SYSDATE BETWEEN a.eff_date AND a.exp_date "
			" ORDER BY a.target_id,b.oid_name ");
	qry.setSQL(sSql);
	qry.open();
	while (qry.next())
	{
		pOIDInfo = new NmpOIDInfo();
		strcpy(pOIDInfo->m_sOIDName,qry.field(0).asString());
		strcpy(pOIDInfo->m_sOIDType,qry.field(1).asString());
		strcpy(pOIDInfo->m_sOIDMacro,qry.field(2).asString());
		pOIDInfo->m_iValueID = qry.field(3).asInteger();
		//Nmp_SQL_value *pSqlValue = NULL;
		if (pOIDInfo->m_iValueID !=-1)
			pOIDInfo->getValuesFrmSql();
			//pSqlValue	= new Nmp_SQL_value(pOIDInfo->m_iValueID);
		//pSqlValue->getValuesFrmSql();
		//pOIDInfo->m_oSqlValue = pSqlValue;
		m_mapOIDInfo[pOIDInfo->m_sOIDName] = pOIDInfo;
	}
	qry.close();

	sprintf(sSql," SELECT b.oid_name,c.value_seq,nvl(c.param_section,'000'),nvl(c.param_name,'000'),nvl(c.if_index,0) visible,nvl(c.value_id,-1) "
			  " FROM STAT_NMP_TARGET_DEF a,STAT_NMP_TARGET_OID_DEF b,STAT_NMP_OID_VALUE_CFG c "
			  " WHERE a.target_id=b.target_id "
			  "      AND b.oid_name=c.oid_name "			  
      			  "	 AND SYSDATE BETWEEN a.eff_date AND a.exp_date "
			  " ORDER BY a.target_id,b.oid_name,c.value_seq ");
	qry.setSQL(sSql);
	qry.open();
	while (qry.next())
	{
		Nmp_OID_VALUE * poOIDValue = new Nmp_OID_VALUE();
		strcpy(poOIDValue->m_sOIDName,qry.field(0).asString());
		poOIDValue->m_iValueSeq = qry.field(1).asInteger();
		strcpy(poOIDValue->m_sParamSection,qry.field(2).asString());
		strcpy(poOIDValue->m_sParamName,qry.field(3).asString());
		//poOIDValue->m_iVisible = qry.field(5).asInteger();	
		poOIDValue->m_iIfIndex= qry.field(4).asInteger();	
		poOIDValue->m_iValueID = qry.field(5).asInteger();
		//if (poOIDValue->m_iValueID!=-1)
			//poOIDValue->getValuesFrmSql();
		iter = m_mapOIDValueList.find(poOIDValue->m_sOIDName);
		if (iter != m_mapOIDValueList.end())
		{
			pVecOIDValueList = iter->second;			
		}
		else
		{
			pVecOIDValueList = new VEC_NMPOIDVALUE;
			m_mapOIDValueList[poOIDValue->m_sOIDName] = pVecOIDValueList;
		}
		
		pVecOIDValueList->push_back(poOIDValue);

		if (!checkParamName(poOIDValue->m_sOIDName,poOIDValue->m_sParamName))
		{
			Log::log(0,"oid=%s�Ĳ���%s��������������δ����ĺ꣬�������޷���ȡ����ֵ��",poOIDValue->m_sOIDName,poOIDValue->m_sParamName);
		}
	}
  qry.close();
	return 1;
	
}

bool NmpOIDMgr::replaceMacro(string & sSrc, string sSubStr,string sReplaceStr)
{
	string::size_type startpos = 0;
	int i=0;
	//�ҵ��ִ���λ��
	startpos = sSrc.find(sSubStr);
	while (startpos!= string::npos)	//string::npos��ʾû���ҵ����ַ�
	{	    	   
		sSrc.replace(startpos,1,sReplaceStr); 
		startpos = sSrc.find(sSubStr);
		i++;
	}

	if (i>0)
		return true;
	else
		return false;		//��ʾδ�ҵ���Ҫ�滻�ĺ�

}

//�����õ��ֶν����еĺ궨��Ĳ���ʵ����
//��������������˺��ȡֵ�������в�����ʵ����
int NmpOIDMgr::instantiateParamName()
{
	char sMacroName[51]={0};
	VEC_NMPOIDVALUE *vec_OIdValue;
	map<string,VEC_NMPOIDVALUE *>::iterator iter;
	vector<string>::iterator iter2;  //sqlvalue������ֵ�ڵ�ĵ�����
	VEC_NMPOIDVALUE::iterator itOIDValue;
	for (iter = m_mapOIDValueList.begin();iter!=m_mapOIDValueList.end();iter++)
	{
		string strOIDName = iter->first;
		vec_OIdValue = iter->second;
		itOIDValue = vec_OIdValue->begin();
		NmpOIDInfo *poOIDInfo = m_mapOIDInfo[strOIDName];
		sprintf(sMacroName,"$%s$",poOIDInfo->m_sOIDMacro);		
		//���oid����Ϊ��table�ͣ���������к��滻��ֻҪѹ��paramname����
		//��table�͵Ĳ���ȡֵֻ����һ������value_seqֻ��Ϊ1
		if (strcmp(poOIDInfo->m_sOIDType,"V0T")!=0)
		{			
			NmpParamOID *pParamOid = new NmpParamOID();
			strcpy(pParamOid->m_sOIDName,strOIDName.c_str());
			string sWholeParam =string( (*itOIDValue)->m_sParamSection)+"."+string((*itOIDValue)->m_sParamName);
			m_mapParamNameToOID[sWholeParam] = pParamOid;
			continue;
		}
		else  //table�͵�oid�ڵ㽫���еĲ���ʵ����
		{			
			//Nmp_SQL_value *pSqlValue =  poOIDInfo->m_oSqlValue;
			if (poOIDInfo->m_vecValue.size() ==0 )   //table�͵�δ������,ֱ�ӽ�������ѹ��
			{
				for (itOIDValue = vec_OIdValue->begin();itOIDValue!=vec_OIdValue->end();itOIDValue++)
				{					
					NmpParamOID *pParamOid = new NmpParamOID();
					strcpy(pParamOid->m_sOIDName,strOIDName.c_str());
					string sWholeParam = string((*itOIDValue)->m_sParamSection)+"."+string((*itOIDValue)->m_sParamName);
					m_mapParamNameToOID[sWholeParam] = pParamOid;					
				}
			}
			else   //table�Ͳ��Ҷ����˺�ȡֵ����Ҫ��ÿ�������еĺ�����滻
			//�����滻���ʵ��ֵѹ��m_mapParamNameToOID;
			{				
				for (iter2 = poOIDInfo->m_vecValue.begin();iter2 !=poOIDInfo->m_vecValue.end();iter2++)
				{
					for (itOIDValue = vec_OIdValue->begin();itOIDValue!=vec_OIdValue->end();itOIDValue++)
					{
						NmpParamOID *pParamOid = new NmpParamOID();
						strcpy(pParamOid->m_sOIDName,strOIDName.c_str());
						pParamOid->m_sMacroValue = (*iter2);
						string sWholeParam = string((*itOIDValue)->m_sParamSection)+"."+string((*itOIDValue)->m_sParamName);
						if (!replaceMacro(sWholeParam,sMacroName,(*iter2)))
							pParamOid->m_sMacroValue =  string("");
						if (m_mapParamNameToOID[sWholeParam] == NULL)
							m_mapParamNameToOID[sWholeParam] = pParamOid;
					}
				}
			}
		}						
		
	}
	     std::cout<<"******************************************************************"<<std::endl;

	for(std::map<std::string,NmpParamOID*>::iterator itere = m_mapParamNameToOID.begin();itere !=m_mapParamNameToOID.end();itere++)
		{
		    std::cout<<itere->first<<"   [------"<<std::endl;
		
		}
     std::cout<<"******************************************************************"<<std::endl;
    return 1;
}

bool NmpOIDMgr::ifParamNeedRenew(SysParamHis & oSysParam,NmpParamOID &oParamMacroInstance,bool bIfFirstRun)
{
	string str_Param =string(oSysParam.sSectionName)+"."+string(oSysParam.sParamName);
	//std::cout<<oSysParam.sSectionName<<" [----] "<<oSysParam.sParamName<<std::endl;
	std::cout<<str_Param<<std::endl;
	//if (strcmp(str_Param.c_str(),"BUSINESS.max_price_plans")==0)
		//printf("find2\n");
	NmpParamOID *pTmp = m_mapParamNameToOID[str_Param];
			if (pTmp)  //����ȡֵ��ͬ�������
		{
			strcpy(oParamMacroInstance.m_sOIDName,pTmp->m_sOIDName);
			oParamMacroInstance.m_sMacroValue = pTmp->m_sMacroValue;
			return true;
		}
			return false;
	
	if (bIfFirstRun)
	{
		if (pTmp)  //����ȡֵ��ͬ�������
		{
			strcpy(oParamMacroInstance.m_sOIDName,pTmp->m_sOIDName);
			oParamMacroInstance.m_sMacroValue = pTmp->m_sMacroValue;
			return true;
		}
		else
			return false;
	}
	else
	{
		if (pTmp && (strcmp(oSysParam.sParamValue,oSysParam.sParamOrValue)))//�ǵ�һ����������ȡֵ��ͬ�������
		{
			strcpy(oParamMacroInstance.m_sOIDName,pTmp->m_sOIDName);
			oParamMacroInstance.m_sMacroValue = pTmp->m_sMacroValue;
			return true;
		}
		else
			return false;
	}
	
	//if (pTmp && (strcmp(oSysParam.sParamValue,oSysParam.sParamOrValue)))  
	
}

int NmpOIDMgr::instToBsnInterface(TOCIQuery & qry,string strOID, string strBsnValue)
{
	char sDimens[2000]={0};
	strcpy(sDimens,strBsnValue.c_str());
//	qry.setSQL("INSERT INTO STAT_INTERFACE(STAT_ID, KPI_CODE, AREA_CODE,DEAL_FLAG, DEAL_DATE, DIMENSIONS) "
//			" VALUES (seq_mnt_stat_id.nextval,:OID,:AREACODE,1,SYSDATE,:DIMENSION)" );
	DEFINE_QUERY (qry1);
  qry1.setSQL ("select seq_mnt_stat_id.nextval from dual");
  qry1.open();
  qry1.next();
  qry.setParameter("seq_mnt_stat_id", qry1.field(0).asInteger());
  qry1.close();
	qry.setParameter("OID", strOID.c_str());
	qry.setParameter("AREACODE", m_sAreaCode);
	qry.setParameter("DIMENSION",strBsnValue.c_str());
	qry.execute();
	return 1;
}

int NmpOIDMgr::getMapValue(char const * sSection, char const * sParamName, char * sOriValue, char * sMapedValue)
{
//Ŀǰֻ��hb��Ԫ����	״̬��level0����״̬��Ҫ��ȡֵӳ��,��д����
         if(strcmp(sSection,"BILLDB")== 0&&strcmp(sParamName,"dbbase_type") == 0)
         	{
         	    	    if(strcmp(sOriValue,"oracle") == 0)
							strcpy(sMapedValue,"1");
						else
							strcpy(sMapedValue,"1");
         	}
         if(strcmp(sSection,"SYSTEM") == 0 && strcmp(sParamName,"run_state") == 0)
         	{
         	strcpy(sMapedValue,"1");
         	}
         if(strcmp(sSection,"SYSTEM") == 0 && strcmp(sParamName,"checkpoint_state") == 0)
         	{
         	strcpy(sMapedValue,"1");
         	}
//ip
char szLog[120]={0};
         if((strcmp(sSection,"SYSTEM") == 0 && strcmp(sParamName,"host_ip") == 0)	
		 	||(strcmp(sSection,"DC_SERVER") == 0 && strcmp(sParamName,"dc_server_ip") == 0)
		 	||(strcmp(sSection,"DC_SERVER") == 0 && strcmp(sParamName,"master_ip") == 0)
		 	||(strcmp(sSection,"DC_SERVER") == 0 && strcmp(sParamName,"slave_ip") == 0)
		 	)
         	{
         	   std::string sIP= sOriValue;
			   unsigned long Ip = 0;
			   unsigned long i = 256*256*256;
			   while(sIP.find('.') != std::string::npos)
			   	{
			   	std::string szIP;
				szIP = sIP.substr(0,sIP.find('.') );
			   	sIP = sIP.substr(sIP.find('.')+1);
			   	unsigned long ig = atoi(szIP.c_str());
				Ip = Ip*i + ig;
				i = i/256;
			   	}
			   
			   Ip = Ip + atol(sIP.c_str());

	         sprintf(sMapedValue,"%ld",Ip);
         	}




		 
	if (strcmp(sSection,"SYSTEM") == 0 )
	{
		if (strcmp(sParamName,"net_state") == 0)
		{
			if (strcmp(sOriValue,"innet") == 0)
				strcpy(sMapedValue,"0");   //����
			else
				strcpy(sMapedValue,"1");  //����
		}

		if (strcmp(sParamName,"run_state") == 0)
		{
		    strcpy(sMapedValue,"0");
			if (strcmp(sOriValue,"normal") ==0)
				strcpy(sMapedValue,"0");
			if (strcmp(sOriValue,"free") ==0)
				strcpy(sMapedValue,"0");
			if (strcmp(sOriValue,"busy") ==0)
				strcpy(sMapedValue,"1");
			if (strcmp(sOriValue,"fault") ==0)
				strcpy(sMapedValue,"2");
		}
	}
	return 1;
}

int NmpOIDMgr::refreshParamToBsn(SysParamHis & oSysParam, NmpParamOID & oParamMacro)
{
         std::cout<<oParamMacro.m_sOIDName<<"~~~~~~~~~~~~~~~~~~~~"<<std::endl;
	
	DEFINE_QUERY(qry);

	qry.setSQL("INSERT INTO STAT_INTERFACE  \
		(STAT_ID, value, stat_begin_date,stat_end_date,state,KPI_CODE,AREA_CODE,create_date, \
		DEAL_FLAG, DEAL_DATE,module_id, DIMENSIONS,dimension_flag,stat_date_flag,db_source_id,min_stat_date,max_stat_date)  \
			 VALUES (:seq_mnt_stat_id,0,sysdate,sysdate,1,:OID,:AREACODE,sysdate,  \
			 1,SYSDATE,1,:DIMENSION,1,3,1,sysdate,sysdate)" );
			
	int i=0;
	char sParamValue[MAX_INI_VALUE_LEN]={0},sOriParamValue[MAX_INI_VALUE_LEN]={0};
	string str_BsnValue=string("");

	string str_OID = string(oParamMacro.m_sOIDName);
	NmpOIDInfo *pOIdInfo = m_mapOIDInfo[str_OID];
	string str_Macro = string(pOIdInfo->m_sOIDMacro);
	VEC_NMPOIDVALUE *vec_OIDValues = m_mapOIDValueList[str_OID];
	VEC_NMPOIDVALUE::iterator itValue ;
	//���oidΪ��table�ͣ�ֱ�Ӳ��뵥����¼��stat_interface
	//���oidΪtable�ͣ����ж�Ӧ�ĺ�ʵ��ֵ���ַ�����Ϻ�����¼����ӿڱ�	
	if (!strcmp(pOIdInfo->m_sOIDType,"V0A")  || (!strcmp(pOIdInfo->m_sOIDType,"V0T") && (oParamMacro.m_sMacroValue!="")))   
	{
		for (itValue = vec_OIDValues->begin();itValue != vec_OIDValues->end(); itValue++)
		{
			i++;
			string str_ParamName = string((*itValue)->m_sParamName);
			if ((*itValue)->m_iIfIndex ==1)
				str_BsnValue.append(oParamMacro.m_sMacroValue);
			else if ((*itValue)->m_iValueID !=-1) 
			{
			        if((strcmp((*itValue)->m_sParamSection,"000") !=0&&strcmp((*itValue)->m_sParamName,"000") !=0)||
						(strcmp((*itValue)->m_sParamSection,"000") !=0&&strcmp((*itValue)->m_sParamName,"SOURCE_IP") !=0)||
						(strcmp((*itValue)->m_sParamSection,"000") !=0&&strcmp((*itValue)->m_sParamName,"DESTINATION_IP") !=0))
			        	{
			        	  (*itValue)->getValuesFrmSql(sParamValue);
						  str_BsnValue.append(sParamValue);
			        	}
					else{
				(*itValue)->getValuesFrmSql(str_Macro.c_str(),oParamMacro.m_sMacroValue.c_str(),sParamValue);
				str_BsnValue.append(sParamValue);
						}
			}
			else
			{
				replaceMacro(str_ParamName,str_Macro,oParamMacro.m_sMacroValue);			
				m_pCommandCom->readIniString((*itValue)->m_sParamSection, str_ParamName.c_str(), sParamValue, "");
				strcpy(sOriParamValue,sParamValue);
				getMapValue((*itValue)->m_sParamSection, str_ParamName.c_str(), sOriParamValue,sParamValue);
				str_BsnValue.append(sParamValue);
			}
			
			if (i<vec_OIDValues->size())
				str_BsnValue.append(";");
		}

		instToBsnInterface(qry,str_OID,str_BsnValue);
		qry.commit();	

	}
	
	//���oidΪtable�ͣ����Ƕ�Ӧ�ĺ�ʵ��ֵΪ�գ�
	//���oid�����к��ʵ��ֵ�滻���������ӿڱ�
	else if (!strcmp(pOIdInfo->m_sOIDType,"V0T") && (oParamMacro.m_sMacroValue==""))
	{
		vector<string>::iterator itMacroValue=pOIdInfo->m_vecValue.begin();
		for (;itMacroValue!=pOIdInfo->m_vecValue.end();itMacroValue++)
		{
			string strValue = (*itMacroValue);
			str_BsnValue = string("");
			i = 0;
			for (itValue = vec_OIDValues->begin();itValue != vec_OIDValues->end();itValue++)
			{
				i++;
				string str_ParamName = string((*itValue)->m_sParamName);
				//ͬ����Ҫ�����ȡ�궨��ֵ��
				if ((*itValue)->m_iIfIndex ==1)
					str_BsnValue.append((*itMacroValue));
				else if ((*itValue)->m_iValueID !=-1)
				{
					(*itValue)->getValuesFrmSql(str_Macro.c_str(),(*itMacroValue).c_str(),sParamValue);
					str_BsnValue.append(sParamValue);
				}
				else
				{									
					replaceMacro(str_ParamName,str_Macro,(*itMacroValue));
					m_pCommandCom->readIniString((*itValue)->m_sParamSection, str_ParamName.c_str(), sParamValue, "");
					strcpy(sOriParamValue,sParamValue);
					getMapValue((*itValue)->m_sParamSection, str_ParamName.c_str(), sOriParamValue,sParamValue);
					str_BsnValue.append(sParamValue);
				}
				if (i<vec_OIDValues->size())
					str_BsnValue.append(";");
			}
			instToBsnInterface(qry,str_OID,str_BsnValue);
		  qry.commit();	
		}
	}

	qry.close();
	return 1;
}

int NmpSysParamsGet::SubscribeSections()
{
	
	
	DEFINE_QUERY(qry);
	qry.setSQL("select distinct param_section from STAT_NMP_OID_VALUE_CFG "
			" where param_section is not null order by param_section ");
	qry.open();
	while(qry.next())
	{				
		//Process::m_pCommandCom->SubscribeCmd(qry.field(0).asString(),MSG_PARAM_CHANGE);
		//Process::m_pCommandCom->GetInitParam(qry.field(0).asString());		
	          m_pCommandCom->SubscribeCmd(qry.field(0).asString(),MSG_PARAM_CHANGE);
		m_pCommandCom->GetInitParam(qry.field(0).asString());
	}
	qry.close();
	return 1;
}

int NmpSysParamsGet::freeVecParamMacro()
{	
	m_vecParamMacroValue.clear();
	return 1;
}

int NmpSysParamsGet::run()
{
	Log::log(0,"nmp��Ϣ���ȡ����ʼ����...");

	NmpOIDMgr oOidMgr;
	//oOidMgr.setCommandComm(Process::m_pCommandCom);
	m_pCommandCom = new CommandCom;
	oOidMgr.setCommandComm(m_pCommandCom);

	
	oOidMgr.load();
	oOidMgr.instantiateParamName();
	//Process::m_pCommandCom->InitClient();
	if(m_pCommandCom->InitClient() == false)
		{
		 Log::log(0,"init client error");
		 return  -1;
		}
	//���濪ʼ��ÿ����Ҫ��section�µ����ݽ�����������������
	SubscribeSections();

	// �ӹ��ж�
    	DenyInterrupt();
	m_vecSysParamHis = new vector<SysParamHis>;

	try
	{
	
		while (!GetInterruptFlag())
		{
			freeVecParamMacro();
			m_vecSysParamHis->clear();
			//Process::m_pCommandCom->FlushParamEx(m_vecSysParamHis);
			m_pCommandCom->FlushParamEx(m_vecSysParamHis);
			vector<SysParamHis>::iterator iter1;
			for (iter1=m_vecSysParamHis->begin();iter1!=m_vecSysParamHis->end();iter1++)
			{
				// ��ÿ���仯�˵Ĳ����鿴�Ƿ�����Ҫ��oid�У�����ڵĻ�������:
				//1.��table�͵�ֱ�Ӳ���stat_interface��
				//2.table�͵���Ҫ��table�������ֶκϲ������stat_interface��
				//		��Ҫע������������ͬ��һ��oid�ĺ�ȡֵ����ֻ��һ��
				
				NmpParamOID oParamMacro;
				if (!oOidMgr.ifParamNeedRenew((*iter1),oParamMacro,m_bFirstRun))
					continue;

				//if (strcmp(oParamMacro.m_sOIDName,"1.3.6.1.4.1.81000.2.103.2.2.6.1.1.5.2") ==0)
				//	printf("find\n");

				//NmpParamOID oTmp;
				//strcpy(oTmp.m_sOIDName,oParamMacro.m_sOIDName);
				//oTmp.m_sMacroValue = oParamMacro.m_sMacroValue;
				//vector<NmpParamOID>::iterator iRes = find(m_vecParamMacroValue.begin(),m_vecParamMacroValue.end(),oParamMacro);
                                     
				vector<NmpParamOID>::iterator iRes = find_if(m_vecParamMacroValue.begin(),m_vecParamMacroValue.end(),
					findParamOID(oParamMacro.m_sOIDName,oParamMacro.m_sMacroValue));
				//�����ƺ��е㲻̫�Ծ�
				if (iRes != m_vecParamMacroValue.end())
					continue;			//˵����Ӧ��oid��oid�еĺ�ʵ���Ѿ�������
					
				m_vecParamMacroValue.push_back(oParamMacro);
                                        
				oOidMgr.refreshParamToBsn((*iter1),oParamMacro);
			}
			
			sleep(60*10);
			m_bFirstRun =  false;
		}
	}

	catch (TOCIException &e)
	{
		Log::log(0,"����SQL����,������Ϣ:%s\n,sqlcode:%d,sqlstr:%s",e.getErrMsg(),e.getErrCode(),e.getErrSrc());
	}
	catch (Exception &e)
	{
		Log::log(0,"�������ش��󣬴�����Ϣ:%s",e.descript());
	}
	catch(exception &e)
	{
		Log::log(0,"�������ش��󣬴�����Ϣ:%s",e.what());
	}
	catch(...)
	{
		Log::log(0,"����δ֪����!");
	}

	return 0;
	
}


