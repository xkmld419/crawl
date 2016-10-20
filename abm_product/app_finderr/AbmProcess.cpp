#include "AbmProcess.h"
//#include "mntapi.h"
#define ABMPROCONFIG "etc/configtest.ini"

/***
flagΪfalseʱ����ʾ��ʼ�����
flagΪtrueʱ����ʾΪ�����������

***/
ABMException oExp;
char HomeEnv[128] = {0};
char *_pUser = getenv("USER");

int findENV()
{
	char *penv = NULL;
    char HomeEnv[128] = {0};
    if ((penv=getenv("ABM_PRO_DIR")) == NULL) {
        ADD_EXCEPT0(oExp, "��������\"HSSCONFIGDIR\"û������, GetConnectInfo ���ش���");
        return -1;
    }
    strncpy(HomeEnv, penv, sizeof(HomeEnv)-2);
    if (HomeEnv[strlen(HomeEnv)-1] != '/') {
        HomeEnv[strlen(HomeEnv)] = '/';
    }
	
	}



void itoaX (int n,char s[])
{ int i,j,sign;
if((sign=n)<0)//��¼���� 
n=-n;//ʹn��Ϊ����    
i=0; 
do
{ 
s[i++]=n%10+'0';//ȡ��һ������ 
}
while ((n/=10)>0);//ɾ��������
if(sign<0) 
s[i++]='-'; 
s[i]='\0'; 
for(j=i;j>=0;j--)//���ɵ�����������ģ�����Ҫ������� 
printf("%c",s[j]);
}

int ABMProcess::CheckProcess(int iProcessId)
{
	findENV();
	char sCommand[256] = {0};
	char *sAudit=NULL;
	char *sAuditT=NULL;
	char sBuff[1][128];
	long numProcess=0;
	FILE *fp = 0;
  
	   sprintf (sCommand, "ps -ef|grep '%d'|grep -v grep|grep -v abmprocess|wc -l",iProcessId);
	   fp = popen (sCommand, "r");
	   if (fp == NULL)
	   {
	   	return -1;
	   }
	   
	   while(!feof(fp))
	   {
	   	if( !fgets(sBuff[1],128,fp) )
	   	{	break;}
	   }
	   fclose(fp);
	   numProcess= strtol(sBuff[1], &sAuditT, 10);
	   if(numProcess>0){
	      
	      return 2;
	   }
	   return 0;
		
	   return 0;
		
}


int ABMProcess::StartProcess(int iProcessId)
{
	findENV();
	char sCommand[256] = {0};
  long lMinProc=0;
  long lMaxProc=0;
	FILE *fp = 0;
	char *sModelProc;
	char *sParammeter;
	char *sVodID;
	bool iFlag=false;
  
	m_poConfS = NULL; 
  if (m_poConfS == NULL) { 
      if ((m_poConfS=new ReadCfg) == NULL) {
           Log::log(0, "ABMConf::getConf m_poConfS malloc failed!");
           return -1;
        }
    }
	  int TableCol = 0;
    char path[1024];
	  int checkI=0;
	  int checkJ=0;
	  snprintf(path, sizeof(path), "%s%s", HomeEnv,ABMPROCONFIG);
    //snprintf(path, sizeof(path), "/cbilling/app/petri/test.ini");
    if (m_poConfS->read(oExp, path, "SYSTEM_MONITOR") != 0) {
            Log::log(0, "check table config failed!");
            return -1;
    }
	TableCol = m_poConfS->getRows();
	for(checkI=0;checkI<TableCol;checkI++)
	{
	    checkJ = checkI+1;
	    //cout<<"�������������"<<checkJ<<endl;
	    if (!m_poConfS->getValue(oExp,lMinProc , "process_id_min", checkJ)) {
                  Log::log(0, "�����ļ���COLFLAG,��ȡʧ��!");
                  return -1;
      }
			  
	    if (!m_poConfS->getValue(oExp, lMaxProc, "process_id_max", checkJ)) {
                    Log::log(0, "�����ļ���COLNAME,��ȡʧ��!");
                    return -1;
      }
      if((iProcessId>lMinProc)&&(iProcessId<lMaxProc)){
      	iFlag=true;
      	
      }
      else{
      	
      	break;
      	}
      
      if (!m_poConfS->getValue(oExp, sModelProc, "app_exec", checkJ)) {
                    Log::log(0, "�����ļ���COLNAME:app_exec,��ȡʧ��!");
                    return -1;
      }
      if (!m_poConfS->getValue(oExp, sParammeter, "parammeter", checkJ)) {
                    Log::log(0, "�����ļ���COLNAME:parammeter,��ȡʧ��!");
                    return -1;
      }
      if (!m_poConfS->getValue(oExp, sVodID, "vod_id", checkJ)) {
                    Log::log(0, "�����ļ���COLNAME:vod_id,��ȡʧ��!");
                    return -1;
      }
      
      
      
	}
	
	if(iFlag){
	    string  sTCommnd;
      
      sTCommnd+="nohup ";
      sTCommnd+=sModelProc;
      sTCommnd+=" -p";
      sTCommnd+=iProcessId;
      sTCommnd+=" ";
      sTCommnd+=sParammeter;
      sTCommnd+=" ";
      sTCommnd+=" -S";
      sTCommnd+=sVodID;
      
	    sprintf (sCommand, sTCommnd.c_str());
      
	    fp = popen (sCommand, "r");
	    if (fp == NULL)
	    {
	    	//mvaddstr(8,(COLS_LENS - strlen((char *)"����ϵͳ����ʧ��"))/2,(char *)"����ϵͳ����ʧ��");
	    	return -1;
	    }
	    while(!feof(fp))
	    {

			     break;

	    }
	    fclose(fp);
	    return 0;
	}
	else{
	 return 2	;
	}
   
		
}

/*
   ���ܣ���������
*/
int ABMProcess::RunProcess(int iProcessId)
{
	findENV();
	cout <<"..�������PROCESSID�Ƿ��Ѿ�����,PROCESSID:"<<iProcessId<<endl;
	if(CheckProcess(iProcessId)==0){
		 cout <<"..Ŀǰû������,PROCESSID:"<<iProcessId<<endl;
		 cout <<"..��ʼ��������:"<<iProcessId<<endl;	
	}else{
		cout <<"..���������Ѿ�����:"<<iProcessId<<endl;
		return 0;
	}
	
	if(StartProcess(iProcessId)==0){
		if(RunProcess(iProcessId)==0){
		    cout <<"..��������δ�ɹ�,PROCESSID:"<<iProcessId<<endl;
		    
	  }else{
		   cout <<"..�������̳ɹ�:"<<iProcessId<<endl;
	  }
	}else{
		cout <<"..��������δ�ɹ�:"<<iProcessId<<endl;		
	}
	

   return 0;
}


int ABMProcess::StopProcess(int iProcessId)
{
	findENV();
	FILE *fp = 0;
	string  sTCommnd;
	char sCommand[256] = {0};
	char s[8];
	cout <<"..�������PROCESSID�Ƿ��Ѿ�����,PROCESSID:"<<iProcessId<<endl;
	if(CheckProcess(iProcessId)==0){
		 cout <<"..Ŀǰû������,PROCESSID:"<<iProcessId<<endl;
		 return 0;
		 //cout <<"..��ʼ��������:"<<iProcessId<<endl;	
	}else{
		/**
		itoaX(iProcessId,s);
	 sTCommnd+="ps -ef|grep ";
	 sTCommnd+=s;
	 sTCommnd+="|grep -v grep|grep ";
	 sTCommnd+=_pUser;
	 sTCommnd+="| awk '{print $2";
   sTCommnd+="}'";
   sTCommnd+="|xargs kill";
	
		sprintf (sCommand, sTCommnd.c_str());
**/
sprintf (sCommand, "ps -ef|grep %d|grep -v grep|grep -v abmprocess|grep %s| awk '{print $2}'|xargs kill",iProcessId,_pUser);


	fp = popen (sCommand, "r");
	if (fp == NULL)
	{
		return -1;
	}
	//---------------------------------------------------------

  //sprintf(sBuff[0],"%s","��¼�û�����¼��ʽ��IP�����ڣ���ʼʱ�䣺����ʱ�䣺����ʱ��"); 
	int n = 0;
	
	while(!feof(fp))
	{
		break;
	}
	fclose(fp);
	
	if(RunProcess(iProcessId)==0){
		    cout <<"�����Ѿ�ֹͣ"<<iProcessId<<endl;
		    
	  }else{
		   cout <<"����ֹͣδ�ɹ�"<<iProcessId<<endl;
	  }
		return 0;
	}
	
}





int ABMProcess::ShowProcess(int iProcessId)
{
	findENV();
	FILE *fp = 0;
	int lv_iTotalRow = 0;
	string  sTCommnd;
	char sCommand[256] = {0};
	char sBuff[1000][128];
	
	cout <<"..�������PROCESSID�Ƿ��Ѿ�����,PROCESSID:"<<iProcessId<<endl;
	if(CheckProcess(iProcessId)==0){
		 cout <<"..Ŀǰû������,PROCESSID:"<<iProcessId<<endl;
		 return 0;
		 //cout <<"..��ʼ��������:"<<iProcessId<<endl;	
	}else{
		/**
	 sTCommnd+="ps -ef|grep ";
	 sTCommnd+=iProcessId;
	 sTCommnd+="|grep -v grep|grep -v abmprocess|grep ";
	 sTCommnd+=_pUser;
	 
	
		sprintf (sCommand, sTCommnd.c_str());
		***/
		
		sprintf (sCommand, "ps -ef|grep %d|grep -v grep|grep -v abmprocess|grep %s",iProcessId,_pUser);

	fp = popen (sCommand, "r");
	if (fp == NULL)
	{
		return -1;
	}
	//---------------------------------------------------------

  //sprintf(sBuff[0],"%s","��¼�û�����¼��ʽ��IP�����ڣ���ʼʱ�䣺����ʱ�䣺����ʱ��"); 
	int n = 0;
	
	while(!feof(fp))
	{
		if( !fgets(sBuff[n],128,fp) )
			break;
		n++;
		lv_iTotalRow ++;
	}
	fclose(fp);
	
	if(lv_iTotalRow==0){
		//mvaddstr(8,(COLS_LENS - strlen((char *)"�޷���������ѡ��"))/2,(char *)"�޷���������ѡ��");
		cout <<"�޷���������ѡ�PROCESS_ID��"<<iProcessId<<endl;
		return 0;
	}else{
		for(n=0;n<lv_iTotalRow;n++){
			cout<<sBuff[n]<<endl;
			n++;
		}
		 
		}
	
		return 0;
	}
	
}



int ABMProcess::ShowProcessInfoByApp(char *sAppName)
{
	findENV();
	FILE *fp = 0;
	int lv_iTotalRow = 0;
	string  sTCommnd;
	char sCommand[256] = {0};
	char sBuff[1000][128];
	cout <<"..�������PROCESSID�Ƿ��Ѿ�����,PROCESSID:"<<iProcessId<<endl;
/***
	 sTCommnd+="ps -ef|grep ";
	 sTCommnd+=sAppName;
	 sTCommnd+="grep -v grep|grep ";
	 sTCommnd+=_pUser;
	 
	
		sprintf (sCommand, sTCommnd.c_str());
		**/
		sprintf (sCommand, "ps -ef|grep %s|grep -v grep|grep -v abmprocess",sAppName);

	fp = popen (sCommand, "r");
	if (fp == NULL)
	{
		return -1;
	}
	//---------------------------------------------------------

  //sprintf(sBuff[0],"%s","��¼�û�����¼��ʽ��IP�����ڣ���ʼʱ�䣺����ʱ�䣺����ʱ��"); 
	int n = 0;
	
	while(!feof(fp))
	{
		if( !fgets(sBuff[n],128,fp) )
			break;
		n++;
		lv_iTotalRow ++;
	}
	fclose(fp);
	
	if(lv_iTotalRow==0){
		//mvaddstr(8,(COLS_LENS - strlen((char *)"�޷���������ѡ��"))/2,(char *)"�޷���������ѡ��");
		cout <<"�޷���������ѡ�PROCESS_ID��"<<iProcessId<<endl;
		return 0;
	}else{
		for(n=0;n<lv_iTotalRow;n++){
			cout<<sBuff[n]<<endl;
			n++;
		}
		 
		}
	
		return 0;
	
}

ABMProcess::ABMProcess()
{
	
}

ABMProcess::~ABMProcess()
{
	
}
