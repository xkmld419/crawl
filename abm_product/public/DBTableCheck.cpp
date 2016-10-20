#include <iostream>
#include "DBTableCheck.h"
#include "ReadIni.h"
#include "TOCIQuery.h"
#include "Exception.h"
#include "Log.h"
#include "Environment.h"
#include <string.h>

using namespace std;

//���ָ�����ָ�����Ƿ���ڣ�������ѡ��
bool DBTableCheck::checkTabColumnExist(char * sTabName, char * sColumnName,char * sOwner)
{
	//ReadIni reader;
	bool bRet = false;
	bool bTab = true;

	char sUserName[33];
	char sDbLink[64];
	memset(sUserName,0,sizeof(sUserName));
	memset(sDbLink,0,sizeof(sDbLink));

	/*char sFile[254];
	char sTIBS_HOME[80];
	memset(sTIBS_HOME,0,sizeof(sTIBS_HOME));
	memset(sFile,0,sizeof(sFile));

	char * p;
	if ((p=getenv ("TIBS_HOME")) == NULL){
		sprintf (sTIBS_HOME, "/home/bill/TIBS_HOME");
	}else{
		sprintf (sTIBS_HOME, "%s", p);
	}

	if (sTIBS_HOME[strlen(sTIBS_HOME)-1] == '/'){
		sprintf (sFile, "%setc/data_access_new.ini", sTIBS_HOME);
	}else{
		sprintf (sFile, "%s/etc/data_access_new.ini", sTIBS_HOME);
	}

	reader.readIniString (sFile, "USER_INFO", "username", sUserName, "");
	//strupr(sUserName);
	for(int i=0;i<strlen(sUserName);i++)
	{
		sUserName[i] = toupper(sUserName[i]);
	}
	*/
    char sSql[2048];
	memset(sSql,0,sizeof(sSql));
    DEFINE_QUERY(qry);
	try{
		sprintf(sSql,"select 1 from dba_tab_columns where table_name ='%s' ",sTabName);
		qry.setSQL(sSql);
		qry.open();
		if(!qry.next()){
			bTab = false;
		}
		qry.commit();
		qry.close();
		memset(sSql,0,sizeof(sSql));
		if(bTab)
		{
			if (sOwner != NULL){
				sprintf(sSql,"select table_name from  dba_tab_columns where "
				" table_name='%s' and owner='%s' and column_name ='%s' ",
				sTabName, sOwner, sColumnName);
			}else{
				sprintf(sSql,"select table_name from  dba_tab_columns where "
				" table_name='%s' and column_name ='%s' ",
				sTabName, sColumnName);
			}
			qry.setSQL (sSql);
			qry.open ();

			if (qry.next()){
				if (sOwner != NULL){
					Log::log(0,"���ݿ��û���%s��������%s��������%s�����ڣ����ͨ����",sOwner,sTabName,sColumnName);
				}else{
					Log::log(0,"������%s��������%s�����ڣ����ͨ����",sTabName,sColumnName);
				}
				bRet = true;
			}
		}else
		{
			if (sOwner != NULL){
				sprintf(sSql,"select table_owner,nvl(db_link,'0') from  all_synonyms where "
				" synonym_name='%s' and owner='%s' ",
				sTabName, sOwner);
			}else{
				sprintf(sSql,"select table_owner,nvl(db_link,'0') from  all_synonyms where "
				" synonym_name='%s' ",
				sTabName);
			}
			qry.setSQL(sSql);
			qry.open();
			if (qry.next()){
				strcpy(sUserName,qry.field(0).asString());
				strcpy(sDbLink,qry.field(1).asString());
			}
			qry.commit();
			qry.close();
			if(sUserName[0] != '\0'){
				if(sDbLink[0] !='0'){
					sprintf(sSql,"select table_name from  dba_tab_columns@%s where "
						" table_name='%s' and owner='%s' and column_name ='%s' ",
						sDbLink,sTabName, sUserName, sColumnName);
				}else{
					sprintf(sSql,"select table_name from  dba_tab_columns where "
						" table_name='%s' and owner='%s' and column_name ='%s' ",
						sTabName, sUserName, sColumnName);
				}
				qry.setSQL(sSql);
				qry.open();
				if (qry.next()){
					Log::log(0,"ͬ���->������%s��������%s��������%s�����ڣ����ͨ����",sUserName,sTabName,sColumnName);
					bRet = true;
				}
				qry.commit();
				qry.close();
			}
		}
	}catch(TOCIException & e)
	{
		Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
		bRet = false;
	}
    return bRet;
}

