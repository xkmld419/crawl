#include <stdio.h>
#include <stdlib.h>
#include "ABMFindErr.h"


int ABMFindErr::GetNumber(char *src)
{
	if(src == NULL)
	 return -1;
	char *p = src;
	int res = 0;
	while(*p != '\t' && (*p) != ' ' && (*p) != NULL)
	{ 
	  if((*p) < '0' || (*p) > '9')
	  {
	  	return -1;
	  }
		res = res*10+(*p - '0');
		p++;
	}
	
	return res;
	
}
int ABMFindErr::ABMFindErrByFile(char * sErrCode,char * sCause,char * sAction)
{
	FILE *fp;
	char * p, sFileName[256], sTemp[1024];
	char * pend;
	int i=0;
	
	if ((p=getenv ("BILLDIR")) == NULL) 
		strcpy (sFileName, "/hss/opt/opthss/src/include/MBC_ABM.h");
	else
		sprintf (sFileName, "%s/src/include/MBC_ABM.h", p);

	if ((fp = fopen (sFileName, "r")) != NULL) {

		while (fgets(sTemp, 1024, fp) != NULL) {
			if (strncmp (sTemp, "//@@", 4) != 0)
				continue;
      
      int iErrorFileGet = GetNumber(&(sTemp[5]));
      
      if(iErrorFileGet < 0)
      	continue;
      	
	    int iErrorArgv = GetNumber(sErrCode);
	    
      if(iErrorFileGet != iErrorArgv)
      	continue;
      	
      char* sTok = " \t";
			p = strtok (&(sTemp[5]), sTok);
			pend = strtok (NULL, sTok);
			strncpy(sErrCode,p,pend - p);
			p = pend;
			pend = strtok (NULL, sTok);
			strncpy (sCause, p,pend - p);
		 		 
			strncpy (sAction, pend,255); 
			i=1;
			break;
		 }
		fclose (fp);
	}
	if(i){return 1;}
		return 0;
}

int ABMFindErr::ABMFindErrByFile()
{
		FILE *fp;
	char * p, sFileName[256], sTemp[1024];
	char *pend;
	int i=0;
	memset(sTemp,0,sizeof(sTemp));
	if ((p=getenv ("BILLDIR")) == NULL) 
		strcpy (sFileName, "/hss/opt/opthss/src/include/MBC_ABM.h");
	else
		sprintf (sFileName, "%s/src/include/MBC_ABM.h", p);

	if ((fp = fopen (sFileName, "r")) != NULL) {

		while (fgets(sTemp, 1024, fp) != NULL) {
			if (strncmp (sTemp, "//@@", 4) != 0)
				continue;
      
      if( GetNumber(&(sTemp[5])) == -1 )
      	continue;
      
      sErrInfo * pErr = new sErrInfo();
      
       char* sTok = " \t";
			p = strtok (&(sTemp[5]), sTok);
			pend = strtok (NULL, sTok);
			strncpy(pErr->scode,p,pend - p);
			p = pend;
			pend = strtok (NULL, sTok);
			strncpy (pErr->sCause, p,pend - p);
		 		 
			strncpy (pErr->sAction, pend,255); 
			
		  if(pErr->sAction[strlen(pErr->sAction)-1]=='\n')
			{
				pErr->sAction[strlen(pErr->sAction)-1] = '\0';
			}
			
			m_vErrInfo.push_back(pErr);
			memset(sTemp,0,sizeof(sTemp));
			i=1;}
		fclose (fp);
	}
	if(i){return 1;}
		return 0;
}

int ABMFindErr::ABMFindErrByTable(char * sErrCode,char * sCause,char * sAction)
{
	DEFINE_QUERY(qry);
	qry.close();
	qry.setSQL("select err_cause cause,action action from h_err_code where err_code_id=:vErrCode");
	qry.setParameter("vErrCode",sErrCode);
	qry.open();
	while(qry.next())
	{
		strcpy( sCause, qry.field("cause").asString() );
		strcpy( sAction, qry.field("action").asString() );
		return 1;
	}
	
	return 0;
}

ABMFindErr::ABMFindErr()
{
	m_vErrInfo.clear();
}

ABMFindErr::~ABMFindErr()
{
	vector<sErrInfo *>::iterator it = m_vErrInfo.begin();
	for(;it!=m_vErrInfo.end();it++)
	{
		delete (*it);
	}
	m_vErrInfo.clear();
}
int ABMFindErr::PrintUsage()
{
    printf("\n ********************************************************    \n") ;
	cout<<"usage:abmfinderr -code <message code>[�������]\n";
	cout<<"For example: abmfinderr -code 334\n";
    cout<<"abmfinderr -code չʾ���еĴ�����Ϣ\n";
    printf("\n ********************************************************    \n") ;
}
int ABMFindErr::resolve(int argc ,char ** argv)
{
	 char sCause[256]={0}, sAction[256]={0},sCode[10]={0};
   ABMFindErr m_oABMFindErr;	
   int icount=0;
   char j;
   j='0';
	if(argc>3 || argc <2)
	{
    m_oABMFindErr.PrintUsage();
		exit(1);
	}
   if(argc==3)
   {
   	  if(strcmp(argv[1],"-code"))
   	  {
   		  m_oABMFindErr.PrintUsage();
   		  exit(1);
   	  }
   	  /*
	    if(strlen(argv[1])>4)
	    {
		    strncpy(sCode,(&argv[1][0]+strlen(argv[1])-4),4);
		    sCode[4]=0;
	    }
	    else
	    {
		    strncpy(sCode,(&argv[1][1]),strlen(argv[1])-1);
		    sCode[strlen(argv[1])-1]=0;	
	    }*/
	    
	    strcpy(sCode,argv[2]);
	    sCode[9] = 0;
   //strncpy(sCode,argv[1]+7,9);
      m_oABMFindErr.ABMFindErrByFile(sCode,sCause,sAction);
      if((sCause[0]==0)&&(sAction[0] == 0))
		  {
			  cout<<"������룺"<<argv[2]<<"Ŀǰû�ж���!"<<endl;;
			  return 0;
		  }
		  
		  int iCode = atoi(sCode);
		  if(iCode > 100000)
		  {
		  	cout<<"������룺"<<argv[2]<<"Ŀǰû�ж���!"<<endl;;
			  return 0;
		  }
		   
		  char cErrType[50];
		  
      GetErrType(iCode,cErrType);
      cout<<"������룺"<<sCode<<endl;
      cout<<cErrType << endl;
	    cout<<"����ԭ��:"<<sCause<<endl;
	    cout<<"����:"<<sAction<<endl;
   }
  if(argc==2)
  {
  	if(strcmp(argv[1],"-code"))
   	  {
   		  m_oABMFindErr.PrintUsage();
   		  exit(1);
   	  }
  	 int iCode;
		  char cErrType[50];
  	m_oABMFindErr.ABMFindErrByFile();
  	vector<sErrInfo *>::iterator it = m_oABMFindErr.m_vErrInfo.begin();
  	for(;it!=m_oABMFindErr.m_vErrInfo.end();it++)
  	{
  				 
		  iCode = atoi((*it)->scode);
		  if(iCode > 100000)
		  continue;
	    GetErrType(iCode,cErrType);
  		icount++;
  		cout<<"������룺"<<(*it)->scode<<endl;
  		cout<<cErrType << endl;
  		cout<<"����ԭ��:"<<(*it)->sCause<<endl;
  		cout<<"����:"<<(*it)->sAction<<endl;
  		if((icount+1)%8==0&&j!='a'){
   	  	cout<<"ALL/BREAK/CONTINUE[a/b/c]:";
   	  	cin>>j;
   	  	if(j=='a'||j=='c'){ continue;}
   	  	else if(j=='b'){ return 0;}
   	    return 0; }
  	}
  }
	return 0;
}

void ABMFindErr::GetErrType(int iCode,char* cErrType)
{
	    if(iCode > 0 && iCode <= 16)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:���ش�����");
		  	
		  }
		  else if(iCode >100 && iCode <=115)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:�ļ���");
		  }
		  else if(iCode >115 && iCode <=140)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:�ڴ���");
		  }
		  else if(iCode >140 && iCode <=155)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:���ݿ���");
		  }
		  else if(iCode >155 && iCode <=165)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:������");
		  }
		  else if(iCode >165 && iCode <=180)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:������");
		  }
		  else if(iCode >180 && iCode <=195)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:��Ϣ��");
		  }
		  else if(iCode >195 && iCode <=210)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:������");
		  }
		  else if(iCode >210 && iCode <=245)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:ҵ����");
		  }
		  else if(iCode >245 && iCode <=260)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:������ʽ�ж���");
		  }
		  else if(iCode >260 && iCode <=300)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:������");
		  }
		  else if(iCode >300 && iCode <=310)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:Я��ת����");
		  }
		  else if(iCode >310 && iCode <=320)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:C������������Ӫ�̴�����");
		  }
		  else if(iCode >320 && iCode <=330)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:��ͬ��ʽ����������Ӫ�̴����ļ���");
		  }
		  else if(iCode >330 && iCode <=345)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:MIN�����");
		  }
		  else if(iCode >345 && iCode <=355)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:��ͬ��ʽIMSI�����");
		  }
		  else if(iCode >355 && iCode <=365)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:������Ϣ����");
		  }
		  else if(iCode >365 && iCode <=375)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:H�����");
		  }
		  else if(iCode >375 && iCode <=385)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:SP�����");
		  }
		  else if(iCode >385 && iCode <=390)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:�߽���������");
		  }
		   else if(iCode >11999 && iCode <=12004)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:�Ϸ��������");
		  }
		  else if(iCode >12099 && iCode <=12107)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:����ģ�������");
		  }
		  else if(iCode >12199 && iCode <=12206)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:����Ʒʵ�������");
		  }
		  else if(iCode == 12250)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:Ⱥ�����������");
		  }
		  else if(iCode == 12260)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:��װ������");
		  }
		  else if(iCode == 12270 || iCode == 12280)
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:ABMҵ����������");
		  }
		  else
		  {
		  	memset(cErrType,0,50);
		  	sprintf(cErrType,"��������:δ֪��");
		  }
}


