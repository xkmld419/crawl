#include "Hbcleantest.h"


Hbcleantest::Hbcleantest()
{
	
}

Hbcleantest::~Hbcleantest()
{
	
}

int Hbcleantest::getFullPath(const char * str1,const char * str2,const char * str3,char * pp,const char * scat,int pro)
{
	char *ptr = NULL;
	char pwdbuf[1024] = {0};
	
	if (NULL == getcwd(pwdbuf, sizeof(pwdbuf)))
	{
		Log::log(0,"ȡ������ǰ�������е�·��");
		return -1;
	}
	
	ptr = RD.readIniString((char *)str1,(char *)str2,(char *)str3,pp,NULL);
	if (  ptr == NULL || strlen(pp) == 0)
	{
		sprintf(pp,"%s/%s",pwdbuf,(char *)scat);
	}
	
	if (pro !=0)
	{
		if(access(pp, F_OK))
		{
			Log::log(0,"ȡ���� %s ·��",pp);
			return -1;
		}
	}
	
	if(strlen(pp) == 0)
	{
		Log::log(0,"ȡ���ֶ��ǿ�ֵ %s",str3);
		return -1;
	}
		
	return 0;
}

int Hbcleantest::Init()
{
	
	int ret = getFullPath("hbtest","OTHER_PATH","SQL_USERDEL",SQL_USERDEL,"sql/test2_deluserinfo.sql");
	
	return ret;
	
}

/*��ȡ��������*/
int Hbcleantest::getenviroment()
{
	char *p;
	char *u;
	struct hostent*     lpHostEnt;
	struct sockaddr_in addr;
	char _sHostName[200];
  
	//ȡ·��
	if((p=getenv("BILLDIR")) == NULL)
	{
		Log::log(0,"ȡ����ϵͳ·��");
		return -1;
	}
	else
		sprintf(h_path,"%s",p);
		
	//ȡ�û�
	if((u=getenv("LOGNAME")) == NULL)
	{
		Log::log(0,"ȡ����ϵͳ�û�");
		return -1;
	}
	else
		sprintf(h_user,"%s",u);
		
    //��ȡ����name
    int iret = gethostname(_sHostName,100);
    
    if (iret) {
        Log::log(0,"ȡ����������");
        return -1;	
    }
    /*
    strcpy(h_hostname,_sHostName);
    //��ȡ����ip
    if ( !( lpHostEnt = gethostbyname( _sHostName ) ) )
        {
             Log::log(0,"ȡ����lpHostEnt");
             return -1;
        }
    addr.sin_addr = *( (struct in_addr*) *lpHostEnt->h_addr_list );//ip��ַ��ֵ
    inet_ntop(AF_INET, &addr.sin_addr,h_hostip, 16);  //��ַת��
    */
    
    return 0;
}

bool Hbcleantest::cleanInfo()
{
	FILE *fp = NULL;
	char sLine[2048] = {0};
	char sSql[2048] = {0};
	char userDelPath[100] ={0};
	
	//strcpy(userDelPath,h_path);
	strcpy(userDelPath,SQL_USERDEL);

	DEFINE_QUERY (qry);
	fp = fopen(userDelPath,"r");
	if(fp == NULL){
		char file[100] = {0};
		strcpy(file,userDelPath);
		Log::log(0,"ж���ļ�%s��ʧ�ܣ�",file);
		return false;	
	}else{
			while(fgets(sLine,2048,fp) != NULL){
				if (('\0' == sLine[0]) || ('#' == sLine[0]) || ('-' == sLine[0]) || ('\n' == sLine[0])) //���Կ��к�ע����
					continue;
				try
				{
					if(sLine[strlen(sLine)-2] == ';')
						sLine[strlen(sLine)-2] = '\n';
					else if(sLine[strlen(sLine)-3] == ';')
						sLine[strlen(sLine)-3] = '\n';
					strcpy(sSql,sLine);
					qry.setSQL(sSql);
					qry.execute();
					qry.close();
					continue;
				}catch(TOCIException & e){
					Log::log(0,"------------ж��ʧ��!");
					Log::log(0,e.getErrMsg());
					Log::log(0,e.getErrSrc());
					throw e;
					return false;
				}
			}
		qry.commit();
		qry.close();
		return true;
	}
}

/*
bool  Hbcleantest::cleanTestList()
{
	char s[100]={0};
	strcpy(s,"sh ");
	strcat(s,h_path);
	strcat(s,DELLIST);
	//system("sh /cbilling/app/DEV_TMP/src_jt/app_hbinstalltest/shell/deltestlist.sh");
	system(s);
	return true;
}

bool  Hbcleantest::cleanTempInfo()
{
	char s[100]={0};
	strcpy(s,"sh ");
	strcat(s,h_path);
	strcat(s,CLEAR);
	//system("sh /cbilling/app/DEV_TMP/src_jt/app_hbinstalltest/shell/clear.sh");
	system(s);
	FILE *fp;
	char sLine[2048];
	char sSql[2048];
	char tempPath[100];
	
	//strcpy(tempPath,h_path);
	strcpy(tempPath,SQL_TEMP);	

	DEFINE_QUERY (qry);
	fp = fopen(tempPath,"r");
	if(fp == NULL){
		char file[100];
		strcpy(file,tempPath);
		Log::log(0,"�м������ļ�%s��ʧ�ܣ�",file);
		return false;	
	}else{
			while(fgets(sLine,2048,fp) != NULL){
				if (('\0' == sLine[0]) || ('#' == sLine[0]) || ('-' == sLine[0]) || ('\n' == sLine[0])) //���Կ��к�ע����
					continue;
				try
				{
					if(sLine[strlen(sLine)-2] == ';')
						sLine[strlen(sLine)-2] = '\n';
					else if(sLine[strlen(sLine)-3] == ';')
						sLine[strlen(sLine)-3] = '\n';
					strcpy(sSql,sLine);
					qry.setSQL(sSql);
					qry.execute();
					qry.close();
					continue;
				}catch(TOCIException & e){
					Log::log(0,"�м���������ʧ��!");
					Log::log(0,e.getErrMsg());
					Log::log(0,e.getErrSrc());
					throw e;
					return false;
				}
			}
		qry.commit();
		qry.close();
		return true;
	}
}

bool Hbcleantest::cleanBillflowcfg()
{
	FILE *fp;
	char sLine[2048];
	char sSql[2048];
	char billDelPath[100];
	
	//strcpy(billDelPath,h_path);
	strcpy(billDelPath,SQL_BILLDEL);	

	DEFINE_QUERY (qry);
	fp = fopen(billDelPath,"r");
	if(fp == NULL){
		char file[100];
		strcpy(file,billDelPath);
		Log::log(0,"������������%s��ʧ�ܣ�",file);
		return false;	
	}else{
			while(fgets(sLine,2048,fp) != NULL){
				if (('\0' == sLine[0]) || ('#' == sLine[0]) || ('-' == sLine[0]) || ('\n' == sLine[0])) //���Կ��к�ע����
					continue;
				try
				{
					if(sLine[strlen(sLine)-2] == ';')
						sLine[strlen(sLine)-2] = '\n';
					else if(sLine[strlen(sLine)-3] == ';')
						sLine[strlen(sLine)-3] = '\n';
					strcpy(sSql,sLine);
					qry.setSQL(sSql);
					qry.execute();
					qry.close();
					continue;
				}catch(TOCIException & e){
					Log::log(0,"������������ʧ��!");
					Log::log(0,e.getErrMsg());
					Log::log(0,e.getErrSrc());
					throw e;
					return false;
				}
			}
		qry.commit();
		qry.close();
		return true;
	}
}
*/
