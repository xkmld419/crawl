#include "LoggWeb.h"
#include <algorithm>
#include "MBC_SGW.h"

//int g_argc;
//char ** g_argv;

extern int g_iFdFront;
extern int g_iStart;
extern int g_iEnd;
extern char g_sStartDate[16];
extern char g_sEndDate[16];

LoggMgr::LoggMgr():m_iMode(0),m_iGroupId(-1),m_poLoggMgr(0),m_poLogg(0)
                    ,m_bPigeonhole(false),m_bParamErr(false),m_RD(0)
{
    memset(m_sParam,'\0',sizeof(m_sParam));
    memset(m_sPath,'\0',sizeof(m_sPath));
    memset(m_sFileName,'\0',sizeof(m_sFileName));
    memset(m_sParamFile,'\0',sizeof(m_sParamFile));
   // m_pCommandComm = 0;
    m_RD = new ReadIni();
    m_iNum=0;
   
   //modify by liyr
   // pPoint= new CheckPointMgr();
   pPoint = 0;
}

LoggMgr::~LoggMgr()
{
    if(m_poLoggMgr)
    {
        delete m_poLoggMgr;
        m_poLoggMgr = 0;
    }
    if(m_poLogg)
    {
        delete m_poLogg;
        m_poLogg = 0;
    }
    if(m_RD)
    {
        delete m_RD;
        m_RD = 0;
    }
    if(pPoint)
    {
    	delete pPoint;
    	pPoint=0;
    }
    DisplayLogg::m_vAppInfo.clear();
    DisplayLogg::m_vProInfo.clear();
    DisplayLogg::m_vAlarmInfo.clear();
    DisplayLogg::m_vParamInfo.clear();
    DisplayLogg::m_vSystemInfo.clear();
    DisplayLogg::m_vSysOperInfo.clear();
}


/*
 *	�� �� ��:   printUsage
 *	��������:   ��������
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   void
 *	����˵��:  
*/
void LoggMgr::printUsage()
{
    cout << endl << endl;
    cout << "*********************************************************************************" << endl;
    cout << "                                  չ��������:                                    " <<endl;  
    cout << "sgwlog" << " "  << "-param   [parameter] :                    ��ʾ[ָ��������]����־����"<<endl;
    cout << "sgwlog" << " "  << "-process [process] :                      ��ʾ����[ָ������ID]����־����"<<endl;
    cout << "sgwlog" << " "  << "-module  [module] :                       ��ʾ����[ָ��ģ��ID|Name]����־����"<<endl;
    cout << "sgwlog" << " "  << "-class   [class] :                        ��ʾ����[ָ�����ID]����־����"<<endl;
    cout << "sgwlog" << " "  << "-level   [level] :                        ��ʾ����[ָ���ȼ�ID]����־����"<<endl;
	cout << "sgwlog" << " "  << "-code    CodeID  :                        ��ʾָ��CodeID����־����" << endl;
//    cout << "sgwlog" << " "  << "-type    [type] :                         ��ʾ����[ָ������ID]����־����"<<endl;
    cout << endl;
    cout << "                                  ����������:                                    " <<endl;
    cout << "sgwlog" << " "  << "-c :                                      ������־�鹲���ڴ�" << endl;
    cout << "sgwlog" << " "  << "-f :                                      �ͷ���־�鹲���ڴ�" << endl;
    cout << "sgwlog" << " "  << "-a :                                      ��־���ֶ��鵵" << endl;
    cout << "sgwlog" << " "  << "-b groupid path(��ָ�����ݵ�Ĭ��Ŀ¼) :   ����ָ����־�鵽ָ��Ŀ¼" << endl;
    cout << "sgwlog" << " "  << "-d groupid filename :                     ɾ����־���ļ�" << endl;
    cout << "sgwlog" << " "  << "-e param :                                ������־��鵵" << endl;
    cout << "sgwlog" << " "  << "-i groupid path(����ȡĬ��·��) :         ������־���ļ�" << endl;
//    cout << "sgwlogg" << " "  << "-g :                                      ������־��" << endl;
    cout << "sgwlog" << " "  << "-l (groupid)(����groupid��ӡָ����) :     ��ʾ(��ǰ/ָ��)��־����Ϣ" << endl;
    cout << "sgwlog" << " "  << "-s groupid(��ָ���Զ��л���ǰ��) :        �л���־��" << endl;
    cout << "*********************************************************************************" << endl;
    cout << endl << endl;
    return ;
}

/*
 *	�� �� ��:   AnalyzesParam
 *	��������:   �����������
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::AnalyzesParam(char * sParam, char * sReturn)
{
    if(!sParam)
        return true;
    char *p = strchr(sParam,'[');
    if(!p)
    {
        char *m = strchr(sParam,'-');
        if(!m)
            strcpy(sReturn,sParam);
        return true;
    }
    p++;
    char *q = strchr(sParam,']');
    if(!q)
        return false;
    strncpy(sReturn,p,q-p);
    return true;
}

/*
 *	�� �� ��:   CheckParamName
 *	��������:   ���������Ƿ���ȷ
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::CheckParamName()
{
  if(!m_pCommandComm )
    {
        m_pCommandComm = new CommandCom();
        if(!m_pCommandComm->InitClient())
        {
            Log::log(0,"ע����Ĳ���ʧ��");
        }	
    }
    char ParamSec[50],ParamKey[50];

    memset(ParamSec,0,sizeof(ParamSec));
    memset(ParamKey,0,sizeof(ParamKey));
    
    char *p = strchr(m_sParam,'.');
    if(!p)
    {
        return false;
    }
    strncpy(ParamSec,m_sParam,p-m_sParam);
    p++;
    strncpy(ParamKey,p,strlen(p));

    if(!m_pCommandComm)
    {
        return false;
    }
    //SysParamHis oParam;
    //if(!m_pCommandComm->getSectionParam(ParamSec,ParamKey,oParam))
      //  return false;
    char sStr[128]={0};
    m_pCommandComm->readIniString(ParamSec,ParamKey,sStr,NULL);
	
	if(!strlen(sStr))return false;
	
    return true;
}

/*
 *	�� �� ��:   GetField
 *	��������:   ȡ�����ֶ���Ϣ
 *	ʱ    ��:   2011��6��28��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::GetField(int iSeq, char *sBuf,char *sParam)
{
    if(!sBuf)
        return false;
    char *p = strchr((char *)sBuf,'|');
    char *p1= sBuf;
    int cnt = 1;
    while(p)
    {
        if(cnt==iSeq)
        {
            strncpy(sParam,p1,p-p1);
            break;
        }
        cnt++;
		p++;
		p1=p;
		p=strchr(p1,'|');
    }
    return true;
}

/*
 *	�� �� ��:   GreaterMark
 *	��������:   �ȽϺ���
 *	ʱ    ��:   2011��6��28��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::GreaterMark(const string & s1, const string & s2)
{
    char sFile1[LOG_DATA_SIZE+1],sFile2[LOG_DATA_SIZE+1];
    memset(sFile1,0,sizeof(sFile1));
    memset(sFile2,0,sizeof(sFile2));
    GetField(9,(char *)s1.c_str(),sFile1);
    GetField(9,(char *)s2.c_str(),sFile2);
    if(strcmp(sFile1,sFile2)>=0)
        return true;
    else 
        return false;
}


/*
 *	�� �� ��:   StringCopy
 *	��������:   ��vSrc�е����ݸ��Ƶ�vDest��
 *	ʱ    ��:   2011��8��22��
 *	�� �� ֵ:   void
 *	����˵��:  
*/
void LoggMgr::StringCopy(vector<string> &vDest, vector<string> &vSrc)
{
	for(size_t i = 0; i != vSrc.size(); ++i)
	{
		vDest.push_back(vSrc[i]);
	}
}

/*
 *	�� �� ��:   DisplayInfo
 *	��������:   չ����־��Ϣ����
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::DisplayInfo(int iMode)
{
    int iLogType = 0,iLogClass = 0,iLevelId = 0;
    int iCondId=-1;
    vector<string> vLogInfo;
    vLogInfo.clear();

	if(!g_iFdFront) {
		memset(g_sStartDate,0,sizeof(g_sStartDate));
		memset(g_sEndDate,0,sizeof(g_sEndDate));
	}
	
    switch(iMode)
    {
    case GROUPPARAM:
        iLogType = LOG_TYPE_PARAM;
        //m_poLogg->GetInfoFromMem(DisplayLogg::m_vParamInfo,iLogType,GROUPPARAM,iLogClass,m_sParam);
        m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeParam,iMode,LOG_TYPE_PARAM,m_sParam,g_sStartDate,g_sEndDate);  
		if(g_iFdFront) DisPlayInfoForWeb(DisplayLogg::m_vTypeParam);
        break;

    case GROUPINFOLV:
        if(strlen(m_sParam))
        {
        	if(isdigit(m_sParam[0]))
        	{
            	iCondId = atoi(m_sParam);
        	} else {
				if(strcasecmp(m_sParam,"fatal") == 0)
					iCondId = LOG_LEVEL_FATAL;
				else if(strcasecmp(m_sParam,"error") == 0)
					iCondId = LOG_LEVEL_ERROR;
				else if(strcasecmp(m_sParam,"warning") == 0)
					iCondId = LOG_LEVEL_WARNING;
				else if(strcasecmp(m_sParam,"info") == 0)
					iCondId = LOG_LEVEL_INFO;
				else if(strcasecmp(m_sParam,"debug") == 0)
					iCondId = LOG_LEVEL_DEBUG;
				sprintf(m_sParam,"%d",iCondId);
			}
			
            if(iCondId==LOG_LEVEL_FATAL) {
               m_poLogg->GetInfoFromMem(DisplayLogg::m_vLevFatal,iMode,LOG_LEVEL_FATAL,m_sParam,g_sStartDate,g_sEndDate); 
               if(g_iFdFront) DisPlayInfoForWeb(DisplayLogg::m_vLevFatal);
            } 
            else if(iCondId==LOG_LEVEL_ERROR) {
               m_poLogg->GetInfoFromMem(DisplayLogg::m_vLevError,iMode,LOG_LEVEL_ERROR,m_sParam,g_sStartDate,g_sEndDate);  
               if(g_iFdFront) DisPlayInfoForWeb( DisplayLogg::m_vLevError); 	
            }
            else if(iCondId==LOG_LEVEL_WARNING) {
              m_poLogg->GetInfoFromMem(DisplayLogg::m_vLevWarn,iMode,LOG_LEVEL_WARNING,m_sParam,g_sStartDate,g_sEndDate); 
              if(g_iFdFront) DisPlayInfoForWeb( DisplayLogg::m_vLevWarn);
            }	
            else if(iCondId==LOG_LEVEL_INFO) {
               m_poLogg->GetInfoFromMem(DisplayLogg::m_vLevInfo,iMode,LOG_LEVEL_INFO,m_sParam,g_sStartDate,g_sEndDate);
               if(g_iFdFront) DisPlayInfoForWeb(DisplayLogg::m_vLevInfo);
            }
            else if(iCondId==LOG_LEVEL_DEBUG) {
              m_poLogg->GetInfoFromMem(DisplayLogg::m_vLevDebug,iMode,LOG_LEVEL_DEBUG,m_sParam,g_sStartDate,g_sEndDate); 
              if(g_iFdFront) DisPlayInfoForWeb( DisplayLogg::m_vLevDebug);
            }	               	 
            else
            {
            		char errstr[500]={0};
            		strcat(errstr,"�������־�ȼ�! \n��־�ȼ�ȡֵ���£�\n");
            	 	strcat(errstr,"    1:FATAL  2:ERROR  3:WARNGING  4:INFO  5:DEBUG \n");
            		if(g_iFdFront)
            		{                 
                    ErrOutForWeb(errstr);
                    EndOutForWeb();            			
            		} else {
            	 		printf("%s",errstr);
            		}
               return false;   
            }     	
        }else{
             m_poLogg->GetInfoFromMem(DisplayLogg::m_vLevFatal,iMode,LOG_LEVEL_FATAL,m_sParam,g_sStartDate,g_sEndDate);  
             m_poLogg->GetInfoFromMem(DisplayLogg::m_vLevError,iMode,LOG_LEVEL_ERROR,m_sParam,g_sStartDate,g_sEndDate);  
             m_poLogg->GetInfoFromMem(DisplayLogg::m_vLevWarn,iMode,LOG_LEVEL_WARNING,m_sParam,g_sStartDate,g_sEndDate); 
             m_poLogg->GetInfoFromMem(DisplayLogg::m_vLevInfo,iMode,LOG_LEVEL_INFO,m_sParam,g_sStartDate,g_sEndDate);  
             m_poLogg->GetInfoFromMem(DisplayLogg::m_vLevDebug,iMode,LOG_LEVEL_DEBUG,m_sParam,g_sStartDate,g_sEndDate); 
             if(g_iFdFront)
             {
            	 StringCopy(vLogInfo,DisplayLogg::m_vLevFatal);
            	 StringCopy(vLogInfo,DisplayLogg::m_vLevError);
            	 StringCopy(vLogInfo,DisplayLogg::m_vLevWarn);		
             	 StringCopy(vLogInfo,DisplayLogg::m_vLevInfo);
               	 StringCopy(vLogInfo,DisplayLogg::m_vLevDebug);
				 DisPlayInfoForWeb(vLogInfo);
             }
        }
        break;    	    	
    case GROUPTYPE:
        if(strlen(m_sParam))
        {
            iCondId = atoi(m_sParam);
            if(iCondId==LOG_TYPE_BUSI) {
               m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeBusi,iMode,LOG_TYPE_BUSI,m_sParam,g_sStartDate,g_sEndDate);  
                if(g_iFdFront) DisPlayInfoForWeb(DisplayLogg::m_vTypeBusi);		
            }
            else if(iCondId==LOG_TYPE_PARAM) {
               m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeParam,iMode,LOG_TYPE_PARAM,m_sParam,g_sStartDate,g_sEndDate);  
               if(g_iFdFront) DisPlayInfoForWeb( DisplayLogg::m_vTypeParam); 	
            }
            else if(iCondId==LOG_TYPE_SYSTEM) {
              m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeSystem,iMode,LOG_TYPE_SYSTEM,m_sParam,g_sStartDate,g_sEndDate); 
              if(g_iFdFront) DisPlayInfoForWeb(DisplayLogg::m_vTypeSystem);
           }
            else if(iCondId==LOG_TYPE_PROC) {
               m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeStart,iMode,LOG_TYPE_PROC,m_sParam,g_sStartDate,g_sEndDate); 
               if(g_iFdFront) DisPlayInfoForWeb( DisplayLogg::m_vTypeStart);	
            } 
            else if(iCondId==LOG_TYPE_DATEDB) {
              m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeOracle,iMode,LOG_TYPE_DATEDB,m_sParam,g_sStartDate,g_sEndDate);  
              if(g_iFdFront) DisPlayInfoForWeb( DisplayLogg::m_vTypeOracle);	
            }              	 
            else
            {
            		char errstr[500]={0};
            		strcat(errstr,"�������־���! \n��־���ȡֵ���£�\n");
            	 	strcat(errstr,"    1:ҵ����־  2:����������־  3:ϵͳ������־  4:���̵�����־  5:��������־ \n");
            	 	
             		if(g_iFdFront)
            		{                 
                    ErrOutForWeb(errstr);
                    EndOutForWeb();            			
            		}else {
            	 		printf("%s",errstr);
            		}
            		
            		return false;
            }     	     	
        }else{
           m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeBusi,iMode,LOG_TYPE_BUSI,m_sParam,g_sStartDate,g_sEndDate);  
           m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeParam,iMode,LOG_TYPE_PARAM,m_sParam,g_sStartDate,g_sEndDate);  
           m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeSystem,iMode,LOG_TYPE_SYSTEM,m_sParam,g_sStartDate,g_sEndDate); 
           m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeStart,iMode,LOG_TYPE_PROC,m_sParam,g_sStartDate,g_sEndDate);  
           m_poLogg->GetInfoFromMem(DisplayLogg::m_vTypeOracle,iMode,LOG_TYPE_DATEDB,m_sParam,g_sStartDate,g_sEndDate); 
           if(g_iFdFront)
           {
            	 StringCopy(vLogInfo,DisplayLogg::m_vTypeBusi);
            	 StringCopy(vLogInfo,DisplayLogg::m_vTypeParam);
            	 StringCopy(vLogInfo,DisplayLogg::m_vTypeSystem);		
             	 StringCopy(vLogInfo,DisplayLogg::m_vTypeStart);
                 StringCopy(vLogInfo,DisplayLogg::m_vTypeOracle);
				 DisPlayInfoForWeb(vLogInfo);
           }            	  
        }
        break;
    case GROUPMODULE:
    case GROUPPROCESS:        
       m_poLogg->GetInfoFromMem(DisplayLogg::m_vClaError,iMode,LOG_CLASS_ERROR,m_sParam,g_sStartDate,g_sEndDate);    
       m_poLogg->GetInfoFromMem(DisplayLogg::m_vClaWarn,iMode,LOG_CLASS_WARNING,m_sParam,g_sStartDate,g_sEndDate);    
       m_poLogg->GetInfoFromMem(DisplayLogg::m_vClaInfo,iMode,LOG_CLASS_INFO,m_sParam,g_sStartDate,g_sEndDate);
       if(g_iFdFront)
       {
       		StringCopy(vLogInfo,DisplayLogg::m_vClaError);
       		StringCopy(vLogInfo,DisplayLogg::m_vClaWarn);
       		StringCopy(vLogInfo,DisplayLogg::m_vClaInfo);
			DisPlayInfoForWeb(vLogInfo);
       }
       	break ;
    case GROUPCLASS:       	         	       		
        if(strlen(m_sParam))
        {//������ѡ�񣬸���m_vAlarmInfo�洢�����Ϣ
            iCondId = atoi(m_sParam);
            if(iCondId==LOG_CLASS_ERROR) {
               m_poLogg->GetInfoFromMem(DisplayLogg::m_vClaError,iMode,LOG_CLASS_ERROR,m_sParam,g_sStartDate,g_sEndDate);  
               if(g_iFdFront)  DisPlayInfoForWeb(DisplayLogg::m_vClaError);
             }
            else if(iCondId==LOG_CLASS_WARNING) {
               m_poLogg->GetInfoFromMem(DisplayLogg::m_vClaWarn,iMode,LOG_CLASS_WARNING,m_sParam,g_sStartDate,g_sEndDate);  
               if(g_iFdFront) DisPlayInfoForWeb( DisplayLogg::m_vClaWarn);	
            }
            else if(iCondId==LOG_CLASS_INFO) {
              m_poLogg->GetInfoFromMem(DisplayLogg::m_vClaInfo,iMode,LOG_CLASS_INFO,m_sParam,g_sStartDate,g_sEndDate); 
              if(g_iFdFront) DisPlayInfoForWeb( DisplayLogg::m_vClaInfo); 	 
            }
            else
            {
            	 char errstr[500]={0};
            	 strcat(errstr,"�������־���.\n ��־�������\n");
            	 strcat(errstr,"1:FATAL/ERROR  2:WARNGING  3:INFO/DEBUG\n");
             		if(g_iFdFront)
            		{                 
                    ErrOutForWeb(errstr);
                    EndOutForWeb();            			
            		}else {
            	 		printf("%s",errstr);
            		}
               return false; 
             }    
           
        }
        else
        {//��־������࣬����m_vAppInfo��m_vProInfo��m_vSystemInfo
            m_poLogg->GetInfoFromMem(DisplayLogg::m_vClaError,iMode,LOG_CLASS_ERROR,m_sParam,g_sStartDate,g_sEndDate);
            m_poLogg->GetInfoFromMem(DisplayLogg::m_vClaWarn,iMode,LOG_CLASS_WARNING,m_sParam,g_sStartDate,g_sEndDate);
            m_poLogg->GetInfoFromMem(DisplayLogg::m_vClaInfo,iMode,LOG_CLASS_INFO,m_sParam,g_sStartDate,g_sEndDate);
            if(g_iFdFront)
            {
            	StringCopy(vLogInfo,DisplayLogg::m_vClaError);
            	StringCopy(vLogInfo,DisplayLogg::m_vClaWarn);
            	StringCopy(vLogInfo,DisplayLogg::m_vClaInfo);
				DisPlayInfoForWeb(vLogInfo);
            }
        }
        break;
		case GROUPCODE:
        if(strlen(m_sParam))
        {
        	int iCode = atoi(m_sParam);
        	m_poLogg->GetInfoFromMem(DisplayLogg::m_vCodeInfo,iMode,iCode,m_sParam,g_sStartDate,g_sEndDate);
        }
		if(g_iFdFront) DisPlayInfoForWeb(DisplayLogg::m_vCodeInfo);
		
        break;
    default:
        return false;
    }
    /*
    sort(DisplayLogg::m_vAppInfo.begin(),DisplayLogg::m_vAppInfo.end(),GreaterMark);
    sort(DisplayLogg::m_vProInfo.begin(),DisplayLogg::m_vProInfo.end(),GreaterMark);
    sort(DisplayLogg::m_vAlarmInfo.begin(),DisplayLogg::m_vAlarmInfo.end(),GreaterMark);
    sort(DisplayLogg::m_vParamInfo.begin(),DisplayLogg::m_vParamInfo.end(),GreaterMark);
    sort(DisplayLogg::m_vSystemInfo.begin(),DisplayLogg::m_vSystemInfo.end(),GreaterMark);
    sort(DisplayLogg::m_vSysOperInfo.begin(),DisplayLogg::m_vSysOperInfo.end(),GreaterMark);
    
    sort(DisplayLogg::m_vLevFatal.begin(),DisplayLogg::m_vLevFatal.end(),GreaterMark);
    sort(DisplayLogg::m_vLevError.begin(),DisplayLogg::m_vLevError.end(),GreaterMark);
    sort(DisplayLogg::m_vLevWarn.begin(),DisplayLogg::m_vLevWarn.end(),GreaterMark);
    sort(DisplayLogg::m_vLevInfo.begin(),DisplayLogg::m_vLevInfo.end(),GreaterMark);
    sort(DisplayLogg::m_vLevDebug.begin(),DisplayLogg::m_vLevDebug.end(),GreaterMark);
    sort(DisplayLogg::m_vClaError.begin(),DisplayLogg::m_vClaError.end(),GreaterMark);
    sort(DisplayLogg::m_vClaWarn.begin(),DisplayLogg::m_vClaWarn.end(),GreaterMark);
    sort(DisplayLogg::m_vClaInfo.begin(),DisplayLogg::m_vClaInfo.end(),GreaterMark);
    sort(DisplayLogg::m_vTypeBusi.begin(),DisplayLogg::m_vTypeBusi.end(),GreaterMark);
    sort(DisplayLogg::m_vTypeParam.begin(),DisplayLogg::m_vTypeParam.end(),GreaterMark);
    sort(DisplayLogg::m_vTypeSystem.begin(),DisplayLogg::m_vTypeSystem.end(),GreaterMark);
    sort(DisplayLogg::m_vTypeStart.begin(),DisplayLogg::m_vTypeStart.end(),GreaterMark);
    sort(DisplayLogg::m_vTypeOracle.begin(),DisplayLogg::m_vTypeOracle.end(),GreaterMark);   
    */ 
    if(g_iFdFront == 0)
    {
    	DisplayLogg::LoggProcessMain(iMode,m_sParam);
    }
    return true;
}


/*
 *	�� �� ��:   DisPlayInfoForWeb
 *	��������:   Ԥ�����������
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::DisPlayInfoForWeb(vector < string > & v_list)
{
    int cnt = v_list.size();
    char sOut[1024]={0};

    sprintf(sOut,"ʱ��%s����%s���%s����%s��������%s����ID%s�߳�ID%sģ��ID%s�Զ���Ԫ%s����%s����%s��������%s�����ֶ�%s��־����",
    							DEFINE_SPLIT,DEFINE_SPLIT,DEFINE_SPLIT,DEFINE_SPLIT,DEFINE_SPLIT,DEFINE_SPLIT,DEFINE_SPLIT,DEFINE_SPLIT,
                  DEFINE_SPLIT,DEFINE_SPLIT,DEFINE_SPLIT,DEFINE_SPLIT,DEFINE_SPLIT);
    FourOutForWeb(cnt,sOut);
   
    int i = (g_iStart > 0) ?  (g_iStart - 1) : 0;	
    int j = (g_iEnd < cnt) ? g_iEnd : cnt;

	int iLen = 0;
	
    for(; i != j; ++i)
    {	
      iLen = v_list[i].length();
	  if(iLen <= 0) continue;
	  
	  if(v_list[i][iLen-1] != '\n')
	  {
        cout << v_list[i].c_str() << endl;
	  }
	  else
	  {
	  	cout << v_list[i].c_str();
	  }
	}
    
    EndOutForWeb();
    
    return true;
}

/*
 *	�� �� ��:   prepare
 *	��������:   Ԥ�����������
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::prepare()
{
    int i = 0;
    bool bErrParam = false,bHasNone = false,bNoSup = false;
    bool bFind = false;
    
    if(g_argc == 1)
    {
        printUsage();
        return true;
    }
    while(i<g_argc)
    {
        if(g_argv[i][0]!='-')
        {
            i++;
            continue;
        }
        bFind = true;
        int ilen = strlen(g_argv[i]);
        switch(g_argv[i][1])
        {
            case 'a':
                {
                    if(strcmp(g_argv[i],"-a")!=0)
                        bNoSup = true;
                    m_iMode = PIGEONHOLE;
                }
                break;
            case 'b':
                {
                    if(strcmp(g_argv[i],"-b")!=0)
                        bNoSup = true;
                    m_iMode = GROUPBAK;
                    if(g_argv[++i])
                    {
                         m_iGroupId = atoi(g_argv[i]);
                         if(m_iGroupId<1 ||m_iGroupId>GROUP_LOG_NUMS)
                            bErrParam= true;
                    }
                    else
                        m_iGroupId = -1;
                    if(g_argv[++i])
                        strcpy(m_sPath,g_argv[i]);
                }
                break;
            case 'c':
                {
                    if(strcmp(g_argv[i],"-c")==0)
                        m_iMode = GROUPCREAT;
                    else if(strncmp(g_argv[i],"-class",6)==0)
                    {
						//modify by liyr:����û��type��class������ǰ��type
						//m_iMode = GROUPCLASS;
                        m_iMode = GROUPTYPE;
						
                        AnalyzesParam(g_argv[g_argc-1],m_sParam);
                    }
					else if(strcmp(g_argv[i],"-code") == 0)
					{
						m_iMode = GROUPCODE;
						if(i == g_argc-1)
						{
							printf("�������������߲�����!\n");
							return false;
						}
						AnalyzesParam(g_argv[g_argc-1],m_sParam);
					}
					else
                        bNoSup = true;
                    i++;
                }
                break;
            case 'd':
                {
                    if(strcmp(g_argv[i],"-d")!=0)
                        bNoSup = true;
                    m_iMode = GROUPDELETE;
                    if(g_argv[++i])
                    {
                        m_iGroupId = atoi(g_argv[i]);
                        if(m_iGroupId<1 ||m_iGroupId>GROUP_LOG_NUMS)
                            bErrParam = true;
                    }
                    else
                        bHasNone = true;
                    if(g_argv[++i])
                        strcpy(m_sFileName,g_argv[i]);
                    else
                        bHasNone = true;
                }
                break;
            case 'e':
                {
                    if(strcmp(g_argv[i],"-e")!=0)
                        bNoSup = true;
                    m_iMode = GROUPSET;
                    if(g_argv[++i])
                    {
                        m_bPigeonhole = atoi(g_argv[i]);
                        int phole = atoi(g_argv[i]);
                        if(phole<0 || phole>1)
                            bErrParam = true;
                    }
                    else
                        bHasNone = true;
                }
                break;
            case 'f':
                {
                    if(strcmp(g_argv[i],"-f")!=0)
                        bNoSup = true;
                    m_iMode = GROUPFREE;
                }
                break;
            case 'i':
                {
                    if(strcmp(g_argv[i],"-i")!=0)
                        bNoSup = true;
                    m_iMode = GROUPFILEADD;
                    if(g_argv[++i])
                    {
                        if(strcmp(g_argv[i],"all"))
                        {
                            m_iGroupId = atoi(g_argv[i]);
                            if(m_iGroupId<1 ||m_iGroupId>GROUP_LOG_NUMS)
                                bErrParam = true;
                        }
                        else
                            m_iGroupId = 0;
                    }
                    else
                        bHasNone = true;
                    if(g_argv[++i])
                        strcpy(m_sPath,g_argv[i]);
                }
                break;

            case 'g':
                {
                    if(strcmp(g_argv[i],"-g")!=0)
                        bNoSup = true;
                    m_iMode = GROUPADD;
                }
                break;
            case 'l':
                {
                    if(strncmp(g_argv[i],"-level",6)==0)
                    {
                        m_iMode = GROUPINFOLV;
                        AnalyzesParam(g_argv[g_argc-1],m_sParam);
                    }
                    else if(strcmp(g_argv[i],"-l")==0)
                    {
                        m_iMode = GROUPPRINTF;
                        if(g_argv[++i])
                        {
                            if(i<g_argc) {
                            	m_iGroupId = atoi(g_argv[i]);
                            	if(m_iGroupId<1 ||m_iGroupId>GROUP_LOG_NUMS)
                                	bErrParam = true;    
                            }
                        } 
                    }
                    else
                        bNoSup = true;
                    i++;
                }
                break;
            case 'p':
                {
                    if(strncmp(g_argv[i],"-param",6)==0)
                    {
                        m_iMode = GROUPPARAM;
                        AnalyzesParam(g_argv[g_argc-1],m_sParam);
                    }
                    else if(strncmp(g_argv[i],"-process",8)==0)
                    {
                        m_iMode = GROUPPROCESS;
                        AnalyzesParam(g_argv[g_argc-1],m_sParam);
                    }
                    else
                        bNoSup = true;
                    i++;
                }
                break;
            case 'r':
                {
                    if(strcmp(g_argv[i],"-reset")==0)
                       m_iMode = GROUPRESET;
                    if(g_argv[++i])
                    {
                         m_iGroupId = atoi(g_argv[i]);
                    }else{
                         m_iGroupId=0;
                     } 
                    
                                         
                }
                break;                
            case 's':
                {
                    if(strcmp(g_argv[i],"-s")==0)
                    {
                        m_iMode = GROUPQUIT;
                        if(g_argv[++i])
                        {
                       		if(i<g_argc) {
                           		 m_iGroupId = atoi(g_argv[i]);
                           		 if(m_iGroupId<1 ||m_iGroupId>GROUP_LOG_NUMS)
                               		 bErrParam = true;    
                       		}
                        }                        
                    }
                    else if(strncmp(g_argv[i],"-stat",5)==0)
                        m_iMode = GROUPSTAT;
                    else
                        bNoSup = true;

                }
                break;
            case 'm':
                {
                    if(strncmp(g_argv[i],"-module",7)==0)
                    {
                        m_iMode = GROUPMODULE;
                        AnalyzesParam(g_argv[g_argc-1],m_sParam);
                    }
                    else
                        bNoSup = true;
                    i++;
                }
                break;
            case 't':
                {
                	if(strncmp(g_argv[i],"-type",6)==0)
                    {
                        m_iMode = GROUPTYPE;
                        AnalyzesParam(g_argv[g_argc-1],m_sParam);
                    }
                    else{
                      m_iMode = GROUPTEST;
                      AnalyzesParam(g_argv[g_argc-1],m_sParam);
                    }
                }
                break;
            default:
                {
                    Log::log(0,"Ӧ�ý��̲�������ȷ����ο���ʾ��");
                    printUsage();
                    return false;
                }
                break;
        }
        if(bHasNone==true)
        {
            Log::log(0,"Ӧ�ý��̲�������ȷ(ȱ�ٱ�ѡ��)");
            return false;
        }
        if(bNoSup==true)
        {
            Log::log(0,"Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�");
            return false;
        }
        if(bErrParam==true)
        {
            Log::log(0,"Ӧ�ý��̲�������ȷ�������ȡֵ��");
            return false; 
        }
        i++;
    }
    
    if(bFind==false)
    {
        printUsage();
        Log::log(0,"Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�");
        return false;
    }
    
    //add by liyr
    if(m_iMode == GROUPQUIT || m_iMode == PIGEONHOLE)
    {
    	pPoint= new CheckPointMgr();
    }
    
    return true;
}

/*
 *	�� �� ��:   init
 *	��������:   ��ʼ��
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::init()
{
    //if(m_bParamErr)
    //{
        if(!m_RD)
        {
            Log::log(0,"��ȡ���Ĳ���ʧ��");
            return false;
        }
        char *p;
        char sHome[500];
        memset(sHome,'\0',sizeof(sHome));
        if ((p=getenv ("SGW_HOME")) == NULL)
            sprintf (sHome, "/opt/opthb/etc");
        else
            sprintf (sHome, "%s/etc", p);
        if(sHome[strlen(sHome)-1] == '/') 
            sprintf (m_sParamFile, "%ssgwconfig", sHome);
        else
            sprintf (m_sParamFile, "%s/sgwconfig", sHome);
    //}
    
    if(m_iMode==GROUPCREAT || m_iMode==GROUPFREE || m_iMode==GROUPSTAT)
    {
        if(!m_poLoggMgr)
            m_poLoggMgr = new ThreeLogGroupMgr();
        
        long lCount = 0;

        m_iNum = m_RD->readIniInteger(m_sParamFile,"LOG","group_num",0);;

        lCount =  m_RD->readIniInteger(m_sParamFile,"MEMORY","log_data_size",0);

        if(m_iNum < 3)
        {
            Log::log(0,"��ȡ���Ĳ�����־�����ȡֵ��Χ��������");
            return false;
        }
        if(lCount <= 0)
        {
            Log::log(0,"��ȡ���Ĳ�����־����������С��������");
            return false;                    
        }
        m_poLoggMgr->setGroupNum(m_iNum);
        m_poLoggMgr->setGroupInfoDataCount(lCount);
    }
    else
    {
        if(!m_poLogg)
            m_poLogg = new ThreeLogGroup();
    }
    
    /*
    if(!m_pCommandComm && m_iMode != GROUPCREAT)
    {
        m_pCommandComm = new CommandCom();
        if(!m_pCommandComm->InitClient())
        {
            Log::log(0,"ע����Ĳ���ʧ��");
            //return false;
            m_bParamErr = true;
        }
        m_bParamErr= true;
      */
        /*
        if(!m_pCommandComm->GetInitParam("LOG"))
        {
            Log::log(0,"���Ĳ�����ȡ��(LOG)ʧ��");
            //return false;
            m_bParamErr = true;
        }
        if(!m_pCommandComm->GetInitParam("MEMORY"))
        {
            Log::log(0,"���Ĳ�����ȡ��(MEMORY)ʧ��");
            //return false;
            m_bParamErr = true;
        }
        */
      
    return true;
}


/*
 *	�� �� ��:   prepare
 *	��������:   ����������
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::deal()
{
    switch(m_iMode)
    {
        case GROUPCREAT:
            {
                if(m_poLoggMgr->create())
                    Log::log(0,"��־�鹲���ڴ洴���ɹ�!");
                else
                    Log::log(0,"��־�鹲���ڴ洴��ʧ��");
                //�״δ�����־��
                if(!m_poLogg)
                    m_poLogg = new ThreeLogGroup(true);
                m_poLogg->InitLogGroup();
            }
            break;
        case GROUPFILEADD:
            {
                if(m_iGroupId==0)
                {//��ʼ���أ�Ĭ�������ļ�
                    int iGroupNum = m_poLogg->GetGroupNum();
                    for(int num=1;num<=iGroupNum;++num)
                    {
                        if(!m_poLogg->AddLogGroupFile(num,m_sPath,true))
                            Log::log(0,"������־���ļ�ʧ��");
                        if(!m_poLogg->AddLogGroupFile(num,m_sPath,true))
                            Log::log(0,"������־���ļ�ʧ��");
                    }
                    Log::log(0,"������־���ļ����");
                }
                else
                {
                    if(m_poLogg->AddLogGroupFile(m_iGroupId,m_sPath,true))
                        Log::log(0,"������־���ļ��ɹ�");
                    else
                        Log::log(0,"������־���ļ�ʧ��");
                }
            }
            break;
        case GROUPBAK:
            {
                if(strlen(m_sPath)==0)
                {
                    char *pPath = m_poLogg->GetGroupBakPath();
                    strcpy(m_sPath,pPath);
                }
                m_poLogg->bakGroupFile(m_iGroupId,m_sPath);            
                Log::log(0,"��־�鱸�����");
            }
            break;
        case GROUPDELETE:
            {
                if(m_poLogg->ClearGroupFile(m_iGroupId,m_sFileName,true))
                    Log::log(0,"ɾ����־���ļ��ɹ�");
                else
                {
                    Log::log(0,"ɾ����־���ļ�ʧ��");
                    return false;
                }
            }
            break;
        case GROUPFREE:
            {
                if(m_poLoggMgr->remove())
                    Log::log(0,"��־�鹲���ڴ�ɾ���ɹ�!");
                else
                {
                    Log::log(0,"��־�鹲���ڴ�ɾ��ʧ��!");
                    return false;
                }
            }
            break;
        case GROUPPRINTF:
            {
                m_poLogg->DisplayLogGroup(m_iGroupId);
            }
            break;
        case GROUPQUIT:
            {
                int iGroupId = m_poLogg->GetUseGroup();
                if(m_poLogg->ChangeLogGroup(false,m_iGroupId)==true)
                {
                    Log::log(0,"�л���־��ɹ�!");
                    char stemp[200]={0};
                    int itempId = m_poLogg->GetUseGroup();
                    sprintf(stemp,"[logg] ��־������%d�л�����%d",iGroupId,itempId);
					Log::log(0,"%s",stemp);
					m_poLogg->log(MBC_THREELOG_CHANGE,LOG_LEVEL_INFO,LOG_TYPE_SYSTEM,"sgwlog -s �л���־��ɹ�:%s",stemp);
                }
                else
                {
                    Log::log(0,"�л���־��ʧ��");
					m_poLogg->log(MBC_THREELOG_CHANGE,LOG_LEVEL_WARNING,LOG_TYPE_SYSTEM,"sgwlog -s �л���־��ʧ��");
                    return false;
                }
                pPoint->touchInfoDown();
                printf("CHECKPOINTִ�гɹ�\n");
                m_poLogg->DisplayLogGroup(m_iGroupId); 
            }
            break;
        case GROUPSET:
            {
                if(m_poLogg->SetLogGroupPigeonhole(m_bPigeonhole))
                {
                    if(m_bPigeonhole==true)
                        Log::log(0,"��־��鵵�����Ѵ�");
                    else
                        Log::log(0,"��־��鵵�����ѹر�");
                }
            }
            break;
        case PIGEONHOLE:
            {
                if(m_poLogg->ChangeLogGroup(true,m_iGroupId)==false)
                {
                    Log::log(0,"�л���־��ʧ��");
					m_poLogg->log(MBC_THREELOG_PIGE,LOG_LEVEL_WARNING,LOG_TYPE_SYSTEM,"sgwlog -a �ֶ��鵵��־��ʧ��");
                    return false;
                }
                else
                {
                	m_poLogg->log(MBC_THREELOG_PIGE,LOG_LEVEL_INFO,LOG_TYPE_SYSTEM,"sgwlog -a �ֶ��鵵��־��ɹ�");
                    Log::log(0,"�ֶ��鵵�ɹ�");
                }
            }
                pPoint->touchInfoDown();
                printf("CHECKPOINTִ�гɹ�\n");
            break;
        case GROUPPARAM:
            {
                if(strlen(m_sParam))
                {
                    if(!CheckParamName())
                    {
                    	   if(g_iFdFront)
                         {
                            char errstr[500]={0};
                            sprintf(errstr,"����ĺ��Ĳ�����%s��������,��ͨ��sgwparam -list�����ѯ���в���",m_sParam);
                            ErrOutForWeb(errstr);
                            EndOutForWeb();
                         }
                         else                         
                        	printf("����ĺ��Ĳ�����%s��������,��ͨ��sgwparam -list�����ѯ���в���\n",m_sParam);                        
                       
                        return false;
                    }
                }
                //��ͳһ�ӿ�20100623
                if(!DisplayInfo(m_iMode))
                {
                    //Log::log(0,"��ȡ���ݼ���ʧ��,������ȡ�����Ƿ���ȷ��");
                    return false;
                }
            }
            break;

        case GROUPMODULE:
        	    if(strlen(m_sParam))
                { 
                	int iAppid;
					bool bAppID;
					if(isdigit(m_sParam[0])) {
						iAppid = atoi(m_sParam) ;
						bAppID = true;
        	    	} else {
        	    		iAppid = GetAppIDByName(m_sParam);
						bAppID = false;
        	    	}
					
                    if(!CheckAppId(iAppid))
                    {
                    	  if(g_iFdFront)
                         {
                            char errstr[500]={0};
                            sprintf(errstr,"�����Module%s��%s��������,��ͨ��sgwstat -ses��ѯģ����Ϣ\n",(bAppID?"ID":"Name"),m_sParam);
                            ErrOutForWeb(errstr);
                            EndOutForWeb();
                         }
                         else
                        	printf("�����Module%s��%s��������,��ͨ��sgwstat -ses��ѯģ����Ϣ\n",(bAppID?"ID":"Name"),m_sParam);
                        		
                        return false;
                    }
					if(!bAppID) sprintf(m_sParam,"%d",iAppid);
                }
                DisplayInfo(m_iMode);
               break ;
        case GROUPPROCESS:
        	    if(strlen(m_sParam))
                {
                	  int iProcessid=atoi(m_sParam) ;
                    if(!CheckProcessId(iProcessid))
                    {
                    	   if(g_iFdFront)
                         {
                            char errstr[500]={0};
                            sprintf(errstr,"�����ProcessID��%s��������,��ͨ��sgwstat -ses��ѯ������Ϣ\n",m_sParam);
                            ErrOutForWeb(errstr);
                            EndOutForWeb();
                         }
                         else
                        	printf("�����ProcessID��%s��������,��ͨ��sgwstat -ses��ѯ������Ϣ\n",m_sParam);
                   
                        return false;
                    }
                }
                DisplayInfo(m_iMode);
               break ;        	
        case GROUPCLASS:        	
        case GROUPINFOLV:
        case GROUPTYPE:
		case GROUPCODE:
            {
                if(!DisplayInfo(m_iMode))
                {
                    //Log::log(0,"��ȡ���ݼ���ʧ��,������ȡ�����Ƿ���ȷ��");
                    return false;
                }
            }
            break;
        case GROUPSTAT:
            {
                //int iNum = m_pCommandComm->readIniInteger("LOG","group_num",-1);
               /* int iNum=m_iNum ;
                printf("*********************************************************\n");
                for(int i=1;i<=iNum;i++)
                {
                    printf("GROUP_ID:%d\n",i);
                    printf("\n");
                    int iDataCnt = m_poLoggMgr->GetShmDataCount(i);
                    int iDataTotal = m_poLoggMgr->GetShmDataTotalCount(i);
                    printf("SHM_DATA_USED:%d\n",iDataCnt);
                    printf("SHM_DATA_TOTAL:%d\n",iDataTotal);
                    printf("\n");
                    int iIndexCnt = m_poLoggMgr->GetShmIndexCount(i);
                    int iIndexTotal = m_poLoggMgr->GetShmIndexTotalCount(i);
                    printf("SHM_INDEX_USED:%d\n",iIndexCnt);
                    printf("SHM_INDEX_TOTAL:%d\n",iIndexTotal);
                    printf("\n");

                    int iIndexDataTotal = m_poLoggMgr->GetShmIndexDataTotal(i);
                    printf("SHM_INDEX_DATA_TOTAL:%d\n",iIndexDataTotal);

                    for(int j=1;j<iIndexTotal;j++)
                    {
                        int iIndexDataCount = m_poLoggMgr->GetShmIndexDataCount(i,j);
                        if(iIndexDataCount>0)
                            printf("INDEX(%d):COUNT:%d\n",j,iIndexDataCount);
                    }
                    printf("-------------------\n");
                }
                printf("*********************************************************\n");
               */
			m_poLoggMgr->showShmInfo();
            }
            break;
        case GROUPTEST:
            {
                int cnt = atoi(m_sParam);
                for(int i=0;i!=cnt;i++)
                {                
                    m_poLogg->log(1000,7,2,LOG_TYPE_PROC,0,"","7|1000|2|1|10|0|888|OcpTranMgr.cpp|20110628151814|||0|0||���̵���\n");
                    m_poLogg->log(1000,7,2,LOG_TYPE_PROC,0,"LOG.log_info_level","7|1000|2|2|10|0|888|OcpTranMgr.cpp|20110628151814|LOG.log_info_level||0|0||�����޸�\n");
                    m_poLogg->log(1000,7,2,LOG_TYPE_PROC,0,"","7|1000|2|3|10|0|888|OcpTranMgr.cpp|20110628151815|||0|0||ҵ����Ϣ\n");
                    m_poLogg->log(1000,7,2,LOG_TYPE_PROC,0,"","7|1000|2|4|10|0|888|OcpTranMgr.cpp|20110628151812|||0|0||״̬���\n");
                    m_poLogg->log(1000,7,2,LOG_TYPE_PROC,0,"","7|1000|2|5|10|0|888|OcpTranMgr.cpp|20110628151816|||0|0||ϵͳ����\n");
                }
            }
            break;
        case GROUPRESET:
            {
                if(m_iGroupId==0)
                {//��ʼ���أ�Ĭ�������ļ�
                    int iGroupNum = m_poLogg->GetGroupNum();
                    for(int num=1;num<=iGroupNum;++num)
                    {
                        if(!m_poLogg->CallBackInfo(num))
                            printf("�����־��[%d]�ڴ�ʧ��\n",num);
                    }
                    printf("�����־�����\n");
                }
                else
                {
                     if(!m_poLogg->CallBackInfo(m_iGroupId))
                         printf("�����־��[%d]�ڴ�ʧ��\n",m_iGroupId);
                    else
                         printf("�����־�����\n");
                }
            }
        	 break ;
        default:
            break;
    }
    return true;
}

/*
 *	�� �� ��:   run
 *	��������:   ������
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
int LoggMgr::run()
{
    if(!prepare())
    {
        Log::log(0,"sgwlogԤ����ʧ��");
        return -2;
    }
    if(m_iMode==0)
        return 0;
    if(!init())
    {
        Log::log(0,"sgwlog��ʼ��ʧ��");
        return -1;
    }
    if(!deal())
    {
        Log::log(0,"sgwlogִ��ʧ��");
        return -3;
    }
    return 0;
}

/*
int main(int argc, char **argv)
{
    g_argc = argc;
    g_argv = argv;
    LoggMgr * pMgr = new LoggMgr();
    if(!pMgr)
    {
        Log::log(0,"����LoggMgr����ʧ��");
        return -1;
    }
    pMgr->run();
    delete pMgr;
    return 0;
}
*/

/*
 *	�� �� ��:   CheckProcessId
 *	��������:   ���ProcessID�Ƿ���ȷ
 *	ʱ    ��:   2011��7��24��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::CheckProcessId(int iProcessID)
{
	try{
		char sSqlcode[512]={0};
	  //TOCIQuery qry (Environment::getDBConn());
	  DEFINE_QUERY(qry );
	  sprintf (sSqlcode, "select a.process_id "
  						         "from wf_process a "
 						           "where  a.Process_Id =%d ",iProcessID);
	  qry.setSQL(sSqlcode);
	  qry.open();
	  if (qry.next()) 
	  {
	  	qry.close();
	  	return true ;
	  }

	  qry.close();
	  return false;
	}
	catch(TOCIException &e) 
	{
		ThreeLogGroup Log;
		if(e.getErrCode()==MBC_DB_LINK_FAIL)
			Log.log(MBC_DB_LINK_FAIL,LOG_LEVEL_FATAL,LOG_TYPE_SYSTEM,e.getErrMsg());
		if(e.getErrCode()==MBC_DB_SERV_PWD_ERROR)
			Log.log(MBC_DB_SERV_PWD_ERROR,LOG_LEVEL_FATAL,LOG_TYPE_SYSTEM,e.getErrMsg());
	}
	
}	

/*
 *	�� �� ��:   CheckAppId
 *	��������:   ���AppID�Ƿ���ȷ
 *	ʱ    ��:   2011��7��24��
 *	�� �� ֵ:   bool
 *	����˵��:  
*/
bool LoggMgr::CheckAppId(int iAppID)
{
	try{
		char sSqlcode[512]={0};
	  //TOCIQuery qry (Environment::getDBConn());
	  DEFINE_QUERY(qry );
	  sprintf (sSqlcode, "select a.app_id "
  						         "from wf_application a "
 						           "where  a.app_id =%d ",iAppID);
	  qry.setSQL(sSqlcode);
	  qry.open();
	  if (qry.next()) 
	  {
	  	qry.close();
	  	return true ;
	  }

	  qry.close();
	  return false;
	}
	catch(TOCIException &e) 
	{
		ThreeLogGroup Log;
		if(e.getErrCode()==MBC_DB_LINK_FAIL)
			Log.log(MBC_DB_LINK_FAIL,LOG_LEVEL_FATAL,LOG_TYPE_SYSTEM,e.getErrMsg());
		if(e.getErrCode()==MBC_DB_SERV_PWD_ERROR)
			Log.log(MBC_DB_SERV_PWD_ERROR,LOG_LEVEL_FATAL,LOG_TYPE_SYSTEM,e.getErrMsg());
	}
	
}	

int LoggMgr::GetAppIDByName(char *sAppName)
{
	char sSqlcode[512]={0};
	int iAppID = -1;
	DEFINE_QUERY(qry );
	sprintf (sSqlcode, "select a.APP_ID from wf_application a "
 					   "where  a.EXEC_NAME = '%s' ",sAppName);
	qry.setSQL(sSqlcode);
	qry.open();
	if (qry.next()) 
	{
		iAppID = qry.field(0).asInteger(); 
	}

	qry.close();
	
	return iAppID;
}


DEFINE_COMMAND_MAIN (LoggMgr);

