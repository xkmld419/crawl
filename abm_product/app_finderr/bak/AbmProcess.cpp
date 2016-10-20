#include "AbmProcess.h"
//#include "mntapi.h"

/***
flagΪfalseʱ����ʾ��ʼ�����
flagΪtrueʱ����ʾΪ�����������

***/

int ABMProcess::CheckProcess(int iProcessId)
{
	char sCommand[256] = {0};
	char *sAudit=NULL;
	char *sAuditT=NULL;
	long numProcess=0;
	FILE *fp = 0;
  
	   sprintf (sCommand, "ps -ef|grep -v grep|grep '%d'|wc -l",iProcessId);
	   fp = popen (sCommand, "r");
	   if (fp == NULL)
	   {
	   	return -1;
	   }
	   
	   while(!feof(fp))
	   {
	   	if( !fgets(sAudit,128,fp) )
	   	{	break;}
	   }
	   fclose(fp);
	   numProcess= strtol(sAudit, &sAuditT, 10);
	   if(numProcess>0){
	      
	      return 2;
	   }
	   return 0;
		
	   return 0;
		
}


int ABMProcess::StartProcess(int iProcessId)
{
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
	  TableCol = 0;
    char path[1024];
	  int checkI=0;
	  int checkJ=0;
	  snprintf(path, sizeof(path), "%s%s", HomeEnv,ABMPROCONFIG, "EXPIMP.ini");
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
	    	mvaddstr(8,(COLS_LENS - strlen((char *)"����ϵͳ����ʧ��"))/2,(char *)"����ϵͳ����ʧ��");
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
	 return 2	
	}
   
		
}

/*
   ���ܣ���������
*/
int ABMProcess::RunProcess(int iProcessId)
{
	cout <<"..�������PROCESSID�Ƿ��Ѿ�����,PROCESSID:"<<iProcessId<<endl;
	if(RunProcess(int iProcessId)==0){
		 cout <<"..Ŀǰû������,PROCESSID:"<<iProcessId<<endl;
		 cout <<"..��ʼ��������:"<<iProcessId<<endl;	
	}else{
		cout <<"..���������Ѿ�����:"<<iProcessId<<endl;
		return 0;
	}
	
	if(StartProcess(int iProcessId)==0){
		if(RunProcess(int iProcessId)==0){
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
	FILE *fp = 0;
	string  sTCommnd;
	char sCommand[256] = {0};
	cout <<"..�������PROCESSID�Ƿ��Ѿ�����,PROCESSID:"<<iProcessId<<endl;
	if(RunProcess(int iProcessId)==0){
		 cout <<"..Ŀǰû������,PROCESSID:"<<iProcessId<<endl;
		 return 0;
		 //cout <<"..��ʼ��������:"<<iProcessId<<endl;	
	}else{
	 sTCommnd+="ps -ef|grep "
	 sTCommnd+=iProcessId
	 sTCommnd+="grep -v grep|grep petri| awk '{print $2";
   sTCommnd+="}'";
   sTCommnd+="|xargs kill"
	
		sprintf (sCommand, sTCommnd.c_str());

	fp = popen (sCommand, "r");
	if (fp == NULL)
	{
		return ;
	}
	//---------------------------------------------------------

  //sprintf(sBuff[0],"%s","��¼�û�����¼��ʽ��IP�����ڣ���ʼʱ�䣺����ʱ�䣺����ʱ��"); 
	int n = 0;
	
	while(!feof(fp))
	{
		break;
	}
	fclose(fp);
	
	if(RunProcess(int iProcessId)==0){
		    cout <<"�����Ѿ�ֹͣ"<<iProcessId<<endl;
		    
	  }else{
		   cout <<"����ֹͣδ�ɹ�"<<iProcessId<<endl;
	  }
		return 0;
	}
	
}




