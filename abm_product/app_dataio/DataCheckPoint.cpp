#include "DataCheckPoint.h"
#include "FileMgr.h"
#include "Log.h"
#include "MBC_28.h"
#include "Exception.h"
#include "TimesTenAccess.h"
//#include "Environment.h"
static vector<structData *> vstructData;

ABMException oExp;
char HomeEnv[128] = {0};
int ColFlag[20]= {0};
char ColName[20][30]= {0};
char TableName[20][30]= {0};

TTConnection *TTConn;
TimesTenConn *m_poTTConn;
TimesTenCMD *m_poPublicConn;
TimesTenCMD  *qryCursor1;

int connectTT(char *memDSN){

  if (GET_CURSOR(m_poPublicConn, m_poTTConn, memDSN, oExp) != 0) 
	{
		Log::log(0, "�������ݿ�ʧ��GET_CURSOR");
    return -1;
  }
        
   
	TTConn = m_poTTConn->getDBConn(true);
	if (TTConn == NULL)
	{
		Log::log(0, "�������ݿ�ʧ��TTConn");
			return -1;
	}
    
    m_poPublicConn= new TimesTenCMD(TTConn);
	 if (m_poPublicConn == NULL)
	 {
		Log::log(0, "�������ݿ�ʧ��qryCursor1");
			return -1;
	 }
	return 0;
}

/****
*    ��������DataCheckPoint::printUsage()
*    ���ã��жϴ�������淶��ʾ
*    ���ߣ��Ŀ���
*    ʱ�䣺2012-02-13
*    �޸ģ�
***/
void DataCheckPoint::printUsage(){
    printf("ʹ�÷�����\n");
    printf("======================= ���ݵ�������:ABMcheckpoint version1.0.0 =======================\n"
           "abmcheckpoint -k \n"
           "  -k �ڴ����ݿ�Ŀ���\n"
           "ʾ����\n"
           "  abmcheckpoint -k DSN: ��ABM_TABLE�ڴ���е����ݵ�����File�ļ���\n"
           "  ����ʱ�ڴ��ʱ���������ݿ�������м�������:\n"
           "==================================================================================\n");
}

DataCheckPoint::DataCheckPoint(int argc, char **argv):g_argc(argc),g_argv(argv){
    Log::init(MBC_COMMAND_hbexport);
    reset();

    Log::log(0, "--��������--");
}

DataCheckPoint::~DataCheckPoint(){
	if(m_poConfS != NULL){
	    delete m_poConfS;
		m_poConfS = NULL;
	}
	
		if(m_poConfT != NULL){
	    delete m_poConfT;
		m_poConfT = NULL;
	}
    Log::log(0, "--�����˳�(����)--");
}


	

void DataCheckPoint::reset(){
    m_poLogGroup =0;
    m_bIgnore = false;
    m_iMode = 0;
    m_phList = 0;
    m_iTableType = 0;
    m_sMemDsn = 0;
    m_sDbTable = 0;
    m_sFileName = 0;
    iTotalCnt=0;
	sql="";
    init();
	  
}
/****
*    ��������DataCheckPoint::prepare()
*    ���ã��жϴ�������Ƿ���ϳ���淶
*    ���ߣ��Ŀ���
*    ʱ�䣺2012-02-13
*    �޸ģ�
***/
bool DataCheckPoint::prepare(){
    int iTemp=0; 
	  int jTemp=0;
    jTemp = g_argc;
    
    char *penv = NULL;
    
    char sfile[256] = {0};

    for ( iTemp=1; iTemp<jTemp; ++iTemp ) {
        if (g_argv[iTemp][0] != '-') {
            m_poLogGroup->log(MBC_PARAM_THREAD_NONSUPP_ERROR,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_PROC,-1,NULL,"Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�!");
            printUsage ();
            return false;
        }

        switch (g_argv[iTemp][1] | 0x20) {
        case 'k':
            m_sMemDsn = g_argv[++iTemp];
            //bM = true;
            break;
        default:
            m_poLogGroup->log(MBC_PARAM_THREAD_NONSUPP_ERROR,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_PROC,-1,NULL,"Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�!");
            printUsage ();
            return false;
            break;
        }
    }

	
	if(!strcmp(m_sMemDsn,"")){
	    m_poLogGroup->log(MBC_PARAM_THREAD_LOST_ERROR,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_PROC,-1,NULL,"Ӧ�ý��̲�������ȷ��ȱ�ٱ�ѡ�!");
        printUsage();
	    Log::log(0, "δ����-k ������Ϣ !");
		return false;
	}
	if ((penv=getenv("ABM_PRO_DIR")) == NULL) {
   	    Log::log(0, "��������\"ABM_PRO_DIR\"û������,  ���ش���");
        ADD_EXCEPT0(oExp, "��������\"ABM_PRO_DIR\"û������,  ���ش���");
        return -1;
    }
    strncpy(HomeEnv, penv, sizeof(HomeEnv)-2);
    if (HomeEnv[strlen(HomeEnv)-1] != '/') {
        HomeEnv[strlen(HomeEnv)] = '/';
    }
    
  
    return true;
}

void DataCheckPoint::init(){
    if (!m_poLogGroup)
    {
        m_poLogGroup = new ThreeLogGroup();
        m_poLogGroup->m_iLogProid = -1;
        m_poLogGroup->m_iLogAppid = -1;
    }
    if (!m_poLogGroup) {
        Log::log(0, "������־�����ʧ��!");
        THROW(1);
    }
}

int DataCheckPoint::check(){
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
	  
	  snprintf(path, sizeof(path), "%s%s%s", HomeEnv,ABMPROCONFIG, "abm_dataio.ini");
    //snprintf(path, sizeof(path), "/cbilling/app/petri/test.ini");
    if (m_poConfS->read(oExp, path, m_sDbTable) != 0) {
            Log::log(0, "check table config failed!");
            return -1;
    }
	TableCol = m_poConfS->getRows();
	for(checkI=0;checkI<TableCol;checkI++)
	{
	    checkJ = checkI+1;
	    cout<<"��������������"<<checkJ<<endl;
	    if (!m_poConfS->getValue(oExp, ColFlag[checkI], "COLFLAG", checkJ)) {
                  Log::log(0, "�����ļ���COLFLAG,��ȡʧ��!");
                  return -1;
              }
			  
	          if (!m_poConfS->getValue(oExp, ColName[checkI], "COLNAME", checkJ)) {
                    Log::log(0, "�����ļ���COLNAME,��ȡʧ��!");
                    return -1;
              }
	}
	return 0;
}

int DataCheckPoint::checkDSN(){
    m_poConfT = NULL; 
    if (m_poConfT == NULL) { 
        if ((m_poConfT=new ReadCfg) == NULL) {
            Log::log(0, "ABMConf::getConf m_poConfT malloc failed!");
            return -1;
        }
    }
	  TableNum = 0;
    char path[1024];
	  int checkI=0;
	  int checkJ=0;
	  snprintf(path, sizeof(path), "%s%s%s", HomeEnv,ABMPROCONFIG, "abm_dataio.ini");
    //snprintf(path, sizeof(path), "/cbilling/app/petri/test.ini");
    if (m_poConfT->read(oExp, path, m_sMemDsn) != 0) {
            Log::log(0, "check table config failed!");
            return -1;
    }
	TableNum = m_poConfT->getRows();
	for(checkI=0;checkI<TableNum;checkI++)
	{
	    checkJ = checkI+1;
	    cout<<"��������������"<<checkJ<<endl;
	    if (!m_poConfT->getValue(oExp, TableName[checkI], "TABLENAME", checkJ)) {
                  Log::log(0, "�����ļ���COLFLAG,��ȡʧ��!");
                  return -1;
              }
	}
	return 0;
}

int DataCheckPoint::QurSql(){
        int i=0;
        sql="";
		sql +=" SELECT ";
		for(i=0;i<TableCol;i++){
		    if(ColFlag[i]==1){
		        sql +="to_char(";
				sql +=ColName[i];
				sql +=")";
		    }else {
			    if(ColFlag[i]==2)
				{
				    sql +=ColName[i];
				}else{
				    if(ColFlag[i]==3){
					    sql +="to_char(";
				        sql +=ColName[i];
				        sql +=")";
					}
				}
			}
			if(i<TableCol-1){
			    sql +=", ";
			}
		}
		sql +=" from ";
		sql +=m_sDbTable;
}

int DataCheckPoint::expdata(){
	try{
		m_poPublicConn->Close();
    m_poPublicConn->Prepare(sql.c_str());
	  m_poPublicConn->Execute();
    
	char Conect[30];
	while (!m_poPublicConn->FetchNext()) 
		{	string sConect="";
			int i=1;
			structData *pstructData = new structData;
			while(i<TableCol+1){
			    m_poPublicConn->getColumn(i++, Conect);
				sConect +=Conect;
				if(i<TableCol+1){
				sConect +=",";
				}
			}
			sprintf(pstructData->colConect,"%s",sConect.c_str());
			vstructData.push_back(pstructData);
			if (vstructData.size()>1000){
			iTotalCnt=iTotalCnt+vstructData.size();
		  for (i=0; i<vstructData.size(); i++)
	    {
		    fprintf(fp,"%s \n",vstructData[i]->colConect);
	    }
	    for(int j_a=0;j_a<vstructData.size();j_a++) {
        delete vstructData[j_a];
      } 
       vstructData.clear();
	    }
			
		}	
		if (vstructData.size()>0){
			iTotalCnt=iTotalCnt+vstructData.size();
		  for (int i=0; i<vstructData.size(); i++)
	    {
		    fprintf(fp,"%s \n",vstructData[i]->colConect);
	    }
	    for(int j_a=0;j_a<vstructData.size();j_a++) {
        delete vstructData[j_a];
      } 
       vstructData.clear();
	    }
	  }catch(TTStatus st) {
        m_poPublicConn->Close();
        Log::log(0, "expdata failed! err_msg=%s", st.err_msg);
        return -1;
        THROW(1);
    }
   return 0;
}	


int DataCheckPoint::run(){
	
	//int i=0;
	string stemp;
  
    if ( !prepare() )
	{
        return -1;
    }
    
    if(0!=connectTT(m_sMemDsn)){
     return -1;	
    }
  
	if (0!=checkDSN())
	{
	
	    return -1;
	}
	for(int x=0;x<TableNum;x++){
	
	time_t t = time(0); 
  char tmp[64]; 
  stemp="";
  strftime( tmp, sizeof(tmp), "_%Y%m%d%H%M.dat",localtime(&t) );
   
	//strcpy(m_sDbTable, TableName[x]);
	m_sDbTable=TableName[x];
	stemp+=HomeEnv;
	stemp+=DATAFILE;
	stemp+=TableName[x];
	stemp+=tmp;
	//m_sFileName=stemp.c_str();
	//snprintf(m_sFileNameX,"%s",stemp);
	strcpy(m_sFileNameX, stemp.c_str());

	if (0!=check())
	{
	
	    return -1;
	}
	QurSql();
	fp=NULL;		
	fp=fopen(m_sFileNameX,"w+");	
    
	if(fp==NULL)
	{
		m_poLogGroup->log(MBC_FILE_OPEN_FAIL,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_PROC,-1,NULL,"���ļ�ʧ��[%s]",m_sFileNameX);
		return -1;
	}
		
    Log::log(0, "��ʼ��������...");
	//int i=0;
	int iRet=-1;
	try{
	    if (0!=expdata())
	    {
	    	return -1;
	    }
	  
    }catch (...){
        m_poLogGroup->log(MBC_MEM_EXPORT_FAIL,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_PROC,-1,NULL,"HCode�ڴ����ݵ����쳣!");
    }
                
    Log::log (0, "������ %d ������!", iTotalCnt);
	for(int j_a=0;j_a<vstructData.size();j_a++) {
        delete vstructData[j_a];
    } 
    vstructData.clear();
    if(fp!=NULL)
	fclose(fp);
}
    return 1;
}



int DataCheckPoint::runApi(){
	//int i=0;
	string stemp;
    
    if(0!=connectTT(m_sMemDsn)){
     return -1;	
    }
  
	if (0!=checkDSN())
	{
	
	    return -1;
	}
	for(int x=0;x<TableNum;x++){
	
	stemp="";
	time_t t = time(0); 
  char tmp[64]; 
  strftime( tmp, sizeof(tmp), "_%Y%m%d%H%M.dat",localtime(&t) );
  
	m_sDbTable=TableName[x];
	stemp+=HomeEnv;
	stemp+=TableName[x];
	stemp+=tmp;
	strcpy(m_sFileNameX, stemp.c_str());

	if (0!=check())
	{
	
	    return -1;
	}
	QurSql();
	fp=NULL;		
	fp=fopen(m_sFileNameX,"w+");	
    
	if(fp==NULL)
	{
		m_poLogGroup->log(MBC_FILE_OPEN_FAIL,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_PROC,-1,NULL,"���ļ�ʧ��[%s]",m_sFileNameX);
		return -1;
	}
		
    Log::log(0, "��ʼ��������...");
	//int i=0;
	int iRet=-1;
	try{
	    if (0!=expdata())
	    {
	    	return -1;
	    }
	  
    }catch (...){
        m_poLogGroup->log(MBC_MEM_EXPORT_FAIL,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_PROC,-1,NULL,"HCode�ڴ����ݵ����쳣!");
    }
                
    Log::log (0, "������ %d ������!", iTotalCnt);
	for(int j_a=0;j_a<vstructData.size();j_a++) {
        delete vstructData[j_a];
    } 
    vstructData.clear();
    if(fp!=NULL)
	fclose(fp);
}
    return 1;
}