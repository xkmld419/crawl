#include "LogGroup.h"
#include "CmdMsg.h"
#include <curses.h>
#include <sys/types.h>
#include <signal.h>
#ifdef clear
#undef clear
#endif

vector<string> LogGroup::m_vConverInfo;
vector<string> LogGroup::m_vDisplayInfo;
vector<string> LogGroup::m_vAlarmInfo;
vector<string> LogGroup::m_vParamInfo;
vector<string> LogGroup::m_vSysInfo;
vector<string> LogGroup::m_vAttemperInfo;
vector<string> LogGroup::m_vAccessInfo;
vector<string> LogGroup::m_vDmlInfo;

LogGroup::LogGroup():m_iMode(0),m_iGroupId(-1),m_bPigeonhole(false),
                          m_iLine(0),m_iModuleId(-1),m_iDisMode(1),m_bQryFile(0)
{
    memset(m_sPath,0,sizeof(m_sPath));
    memset(m_sFileName,0,sizeof(m_sFileName));
    memset(m_sBatch,0,sizeof(m_sBatch));
    memset(m_sParam,0,sizeof(m_sParam));
    Log::init(MBC_COMMAND_hblogg);
    LOGG(ATTEMPERLOG,"--��������--");
    if(Log::m_pologg)
        m_poLogGroup = Log::m_pologg;
    else
        m_poLogGroup = new ThreeLogGroupMgr();
    if(!m_poLogGroup)
    {
        Log::log(0,"������־��������ʧ��");
        ALARMLOG(0,MBC_Log_Group+5,"%s","������־��������ʧ��");
        THROW(MBC_Log_Group+5);
    }

    m_poCheckPoint = 0;
    m_poCmdCom = 0;
    LogGroup::m_vDisplayInfo.clear();
    char sMode[10]={0};
    ParamDefineMgr::getParam("HBLOGG","DISPLAY_LOG_MODE",sMode,50);
    m_iDisMode = atoi(sMode);
        
}

LogGroup::~LogGroup()
{
    LOGG(ATTEMPERLOG,"--�����˳�(����)--");
    if(m_poLogGroup)
        delete m_poLogGroup;
    m_poLogGroup = 0;

    if(m_poCheckPoint)
        delete m_poCheckPoint;
    m_poCheckPoint = 0;

    if(m_poCmdCom)
        delete m_poCmdCom;
    m_poCmdCom = 0;
    
    LogGroup::m_vConverInfo.clear();
    LogGroup::m_vDisplayInfo.clear();
    LogGroup::m_vAlarmInfo.clear();
    LogGroup::m_vParamInfo.clear();
    LogGroup::m_vSysInfo.clear();
    LogGroup::m_vAttemperInfo.clear();
    LogGroup::m_vAccessInfo.clear();
    LogGroup::m_vDmlInfo.clear();
}

bool LogGroup::init()
{
    memset(m_sPath,0,sizeof(m_sPath));
    memset(m_sFileName,0,sizeof(m_sFileName));
    memset(m_sBatch,0,sizeof(m_sBatch));
    memset(m_sParam,0,sizeof(m_sParam));
    if(!m_poLogGroup)
    {
        if(Log::m_pologg)
            m_poLogGroup = Log::m_pologg;
        else
            m_poLogGroup = new ThreeLogGroupMgr();
    }
    if(!m_poLogGroup)
    {
        Log::log(0,"������־��������ʧ��");
        ALARMLOG(0,MBC_Log_Group+5,"%s","������־��������ʧ��");
        THROW(MBC_Log_Group+5);
    }

    m_poCheckPoint = 0;
    m_poCmdCom = 0;
    m_vDisplayInfo.clear();
    char sMode[10]={0};
    ParamDefineMgr::getParam("HBLOGG","DISPLAY_LOG_MODE",sMode,50);
    m_iDisMode = atoi(sMode);   
    return true;
}

void LogGroup::printUsage()
{
    cout << endl << endl;
    cout << "*********************************************************************************" << endl;
    cout << "hblogg" << " "  << "-c :                                      ������־�鹲���ڴ�" << endl;
    cout << "hblogg" << " "  << "-f :                                      �ͷ���־�鹲���ڴ�" << endl;
    cout << "hblogg" << " "  << "-a :                                      ��־���ֶ��鵵" << endl;
    cout << "hblogg" << " "  << "-b groupid path(��ָ�����ݵ�Ĭ��Ŀ¼) :   ����ָ����־�鵽ָ��Ŀ¼" << endl;
    cout << "hblogg" << " "  << "-d groupid filename :                     ɾ����־���ļ�" << endl;
    cout << "hblogg" << " "  << "-e param :                                ������־��鵵,paramΪ0��1" << endl;
    cout << "hblogg" << " "  << "-i groupid path(����ȡĬ��·��) :         ������־���ļ�" << endl;
//    cout << "hblogg" << " "  << "-j BatchId:                               ����checkpoint�ļ�" << endl;    
    cout << "hblogg" << " "  << "-l :                                      ��ʾ(��ǰ/ָ��)��־����Ϣ" << endl;
//    cout << "hblogg" << " "  << "-o groupid :                              չ��������������" << endl;
    cout << "hblogg" << " "  << "-s :                                      �л���־��" << endl;
//    cout << "hblogg" << " "  << "-w :                                      ���checkpoint��Ϣ" << endl;
    cout << "hblogg" << " "  << "-param[parameter] :                       ��ʾ����[ָ��������]����־����"<<endl;
    cout << "hblogg" << " "  << "-checkpoint :                             ��ʾcheckpoint����־����"<<endl;
    cout << "hblogg" << " "  << "-process[process] :                      ��ʾ����[ָ��������]����־����"<<endl;
    cout << "hblogg" << " "  << "-module[module] :                        ��ʾ����[ָ��ģ����]����־����"<<endl;
    cout << "hblogg" << " "  << "-class[class] :                          ��ʾ����[ָ�������]����־����"<<endl;
    cout << "hblogg" << " "  << "-level[level] :                          ��ʾ����[ָ���ȼ�]����־����"<<endl;
    cout << "hblogg" << " "  << "-staffid[staffid] :                      ��ʾ[ָ������]����־����"<<endl;
    cout << "hblogg" << " "  << "-tablename[tablename] :                  ��ʾ[ָ������]����־����"<<endl;
    cout << "hblogg" << " "  << "-sqltype[sqltype] :                      ��ʾ[ָ����������]����־����"<<endl;
    cout << "*********************************************************************************" << endl;
    cout << endl << endl;

}

bool LogGroup::IfCDisplayInfo()
{
	string m_key;

	cout<<"c:���� ����:�˳�"<<endl;
	cin>>m_key;
	if((!strcmp(m_key.c_str(),"c"))||(!strcmp(m_key.c_str(),"C")))
		return true;
	return false;
}

bool LogGroup::AnalyParamInfo(int LogFlag,char *sStandId1, vector < string > & vParamInfo,vector < string > & vDisInfo)
{
    /*
    string strinfo;
    unsigned int m=0;
    int loc=0;
    char rule[100]={0};
    switch(LogFlag)
	{
        case PARAMLOG:
        {
            if(strlen(sStandId1))
                sprintf(rule,"[PARAM:%s]",sStandId1);
            else
                strcpy(rule,"[PARAM:");
        }
        break;
        
        case SYSTEMLOG:
        {
            sprintf(rule,"[STAFFID:%s]",sStandId1);
        }
        break;
        
        default:
            strcpy(rule,"[");
           break;
	}
    vector<string>::reverse_iterator r_iter;
    if(vParamInfo.size()==0)
    {
        return false;
    }
    for(r_iter=vParamInfo.rbegin();r_iter!=vParamInfo.rend();++r_iter)
    {
        loc = (*r_iter).find(rule,0);
        if(loc!=string::npos)
            vDisInfo.push_back(*r_iter);
    }
    */
    return true;
}

bool LogGroup::AnalyInfoByFile(int LogFlag,int StandId1,char *sStandId1,vector<string> &vDisplayInfo)
{
    FILE *fp;
    DIR *dirp=NULL;
    struct dirent *dp=NULL;
    struct stat statbuf;
    int istate=-1,count=0,filecount=0,upCount=0,inCount=0;
    char spath[500]={0};
    char sName[500]={0},buf[1024]={0};
    char rule[100]={0};
    char rule_1[100]={0};
    char rule_2[100]={0};

    switch(LogFlag)
	{
	case OPERATIONLOG:
        if(StandId1==PINFOPOINT)
        {//���ȼ���ѯ��Ϣ��
            int iLevelid = atoi(sStandId1);
            if(iLevelid==1)
                strcpy(rule,"[LV-1]");
            else if(iLevelid==2)
                strcpy(rule,"[LV-2]");
            else if(iLevelid==3)
                strcpy(rule,"[LV-3]");
            else
                strcpy(rule,"[LV-");
        }
        else if(StandId1==DISALARM)
        {
            if(strlen(sStandId1))
                sprintf(rule,"ModuleId:[%s]",sStandId1);
            else
                strcpy(rule,"[ALARM]");
        }
        else if(StandId1==DISPLAYALL)
        {
            strcpy(rule,"[LV");
            strcpy(rule_1,"[ALARM]");
            strcpy(rule_2,"DML[");
        }
		break;
	case SYSTEMLOG:
        if(StandId1==DISPLAYALL)
        {
            strcpy(rule,"[checkpoint]");
            strcpy(rule_1,"[STAFFID:");
            strcpy(rule_2,"[ADMIN]");
        }
        else if(StandId1==DISBYCHECK)
            strcpy(rule,"[checkpoint]");
        else
            sprintf(rule,"[%s]",sStandId1);
		break;
	case PARAMLOG:
        if(StandId1==DISPLAYALL)
            strcpy(rule,"[PARAM:");
        else
            sprintf(rule,"[PARAM:%s]",sStandId1);
		break;
    case ATTEMPERLOG:
        if(StandId1==DISPLAYALL)
            strcpy(rule,"[PRO:");
        else
            sprintf(rule,"[PRO:%s]",sStandId1);
        break;
    case HBACCESS:
        if(StandId1==DISPLAYALL)
            strcpy(rule,"TABLE_NAME:[");
        else
            sprintf(rule,"[%s]",sStandId1);
        break;

	default:
		return false;
	}

    if(strlen(rule)==0)
        return false;
    
    char *p=NULL;
    char sLog_HOME[100];
    memset(sLog_HOME,0,sizeof(sLog_HOME));
    if ((p=getenv ("BILLDIR")) == NULL)
	    sprintf (sLog_HOME, "/opt/opthb/log");
	else 
	    sprintf (sLog_HOME, "%s/log", p);

    sprintf(spath,"%s/LOGG_ANALY/",sLog_HOME);
    
    if (spath[strlen(spath)-1] != '/')
    {
        strcat(spath, "/");
    }
    dirp = opendir(spath);
    if(dirp==NULL)
    {
        Log::log (0, "��ָ��Ŀ¼����");
        return false;
    }

    for(dp = readdir(dirp); dp != NULL; dp = readdir(dirp))
    {
        strncpy(sName, dp->d_name, 500);
        if(strcmp(sName, ".")==0 || strcmp(sName, "..")==0)
            continue;
        char TempName[1000]={0};
        sprintf(TempName,"%s%s/",spath,sName);
        if(lstat(TempName, &statbuf)<0)
        {
            perror("istate");
            Log::log (0, "��ȡ�ļ�״̬�����ļ���:%s",sName);
            continue;    
        }
        if(S_ISDIR(statbuf.st_mode)==1)
        {//��Ŀ¼
            continue;
        }
 		if (0 == statbuf.st_size)
        {
            Log::log (0, "��ȡ�ļ���СΪ�գ��ļ���:%s",sName);
 		    continue;
        }        
        fp = fopen(TempName,"r");
        if(fp==NULL)
        {
            Log::log (0, "���ļ�ʧ�ܣ��ļ���:%s",sName);
            continue;
        }
        string sAnalyStr;
        int maxcnt=0;
        while(fgets(buf,sizeof(buf),fp))
        {
            string::size_type loc=0,loc1=0;
            if(strcmp(buf,"\n")==0)
                 continue;
            if(maxcnt>=DISPLAYMAXCNT)
                break;
            sAnalyStr=buf;
            loc = sAnalyStr.find(rule,0);
            if(loc!=string::npos)
            {
                vDisplayInfo.push_back(sAnalyStr);
                maxcnt++;
            }
            if(strlen(rule_1))
            {
                loc1 = sAnalyStr.find(rule_1,0);
                if(loc1!=string::npos)
                {
                    vDisplayInfo.push_back(sAnalyStr);
                    maxcnt++;
                }
            }
            if(strlen(rule_2))
            {
                loc1 = sAnalyStr.find(rule_2,0);
                if(loc1!=string::npos)
                {
                    vDisplayInfo.push_back(sAnalyStr);
                    maxcnt++;
                }
            }
        }
        fclose(fp);
    }
    closedir(dirp);
    return true;
}

bool LogGroup::AnalyAlarmInfoFromFile(char *sStandId1,vector<string> &vDisplayInfo)
{
    FILE *fp;
    char rule[100]={0};
    char buf[1024]={0};
    char TempName[1024]={0};

    vector<string>::iterator iter;
    if(strlen(sStandId1))
        sprintf(rule,"ModuleId:[%s]",sStandId1);
    else
        strcpy(rule,"[ALARM]");

    char *p=NULL;
    char sLog_HOME[100];
    memset(sLog_HOME,0,sizeof(sLog_HOME));
    if ((p=getenv ("BILLDIR")) == NULL)
	    sprintf (sLog_HOME, "/opt/opthb/log");
	else 
	    sprintf (sLog_HOME, "%s/log", p);

    sprintf(TempName,"%s/Log_bottom_alarm.log",sLog_HOME);
    fp = fopen(TempName,"r");
    if(fp==NULL)
    {
        return false;
    }
    string sAnalyStr;
    while(fgets(buf,sizeof(buf),fp))
    {
        int loc=0,loc1=0;
        if(strcmp(buf,"\n")==0)
             continue;
        if(strlen(buf)==0)
            continue;
        sAnalyStr=buf;
        loc = sAnalyStr.find(rule,0);
        if(loc!=string::npos)
        {
            iter = vDisplayInfo.begin();
            if(iter!=vDisplayInfo.end())
                vDisplayInfo.insert(iter,sAnalyStr);
            else
                vDisplayInfo.push_back(sAnalyStr);
        }
    }
    fclose(fp);
    return true;
}

bool LogGroup::AnalyzesParam(char *sParam,char *sReturn)
{
    char *p = strchr(sParam,'[');
    if(!p)
        return false;
    p++;
    char *q = strchr(sParam,']');
    if(!q)
        return false;
    strncpy(sReturn,p,q-p);
    return true;
}

bool LogGroup::GetAppIdByName(char *sParam,char *sReturn)
{
    char sSql[1024]={0};
    int iAppId=0;

    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select distinct(a.app_id) from wf_application a where "
        "Upper(a.exec_name) like Upper('%s')",m_sParam);
    qry.setSQL(sSql);
    qry.open ();
    if(qry.next())
        iAppId = qry.field(0).asInteger();
    qry.close();

    if(iAppId==0)
    {
        return false;
    }
    memset(sReturn,0,sizeof(sReturn));
    sprintf(sReturn,"%d",iAppId);
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return false;
    }
    return true;
}

bool LogGroup::GetModuleIdByName(char * sParam, char * sReturn)
{
    char sSql[1024]={0};
    int iModId=0;

    memset(sReturn,0,sizeof(sReturn));
    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select distinct(a.module_id) from wf_application a where "
        "Upper(a.exec_name) like Upper('%s')",m_sParam);
    qry.setSQL(sSql);
    qry.open ();
    if(qry.next())
        iModId = qry.field(0).asInteger();
    qry.close();

    if(iModId==0)
    {
        return false;
    }
    sprintf(sReturn,"%d",iModId);
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return false;
    }
    return true;   
}

bool LogGroup::CheckParamModule()
{
    char sSql[1024]={0};
    int cnt=0;

    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select count(*) from wf_module where Upper(MODULE_NAME) like Upper('%s') ",m_sParam);
    qry.setSQL(sSql);
    qry.open ();
    if(qry.next())
        cnt = qry.field(0).asInteger();
    qry.close();

    if(cnt==0)
    {
        printf("------------Err Msg------------\r\n");
        printf("\n�Ҳ���ģ������Ϊ[%s]����ؼ�¼,���ѯģ�鶨���б�[wf_module]��ȡ��ȷ��ģ����\n\n",m_sParam);
        return false;
    }
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return false;
    }
    return true;
}

bool LogGroup::CheckModuleForMem()
{
    char sSql[1024]={0};
    bool bfind = false;

    DEFINE_QUERY(qry);
    try
    {
    if(strlen(m_sParam)==0)
//        sprintf(sSql,"select distinct(c.process_id) from wf_module a ,wf_application b,wf_process c "
//            "where a.module_id = b.module_id and b.app_id = c.app_id ");
          sprintf(sSql,"select distinct(module_id) from wf_module ");
    else
//        sprintf(sSql,"select distinct(c.process_id) from wf_module a ,wf_application b,wf_process c "
//            "where a.module_id = b.module_id and b.app_id = c.app_id "
//            "and Upper(a.MODULE_NAME) like Upper('%s')",m_sParam);
          sprintf(sSql,"select distinct(module_id) from wf_module where Upper(MODULE_NAME) like Upper('%s')",m_sParam);
    qry.setSQL(sSql);
    qry.open ();
    while(qry.next())
    {
        m_iModuleId= qry.field(0).asInteger();
        if(m_iModuleId>0)
        {
            char sOrder[20]={0};
            memset(sOrder,0,sizeof(sOrder));
            sprintf(sOrder,"%d",m_iModuleId);
            if(!m_bQryFile)
            {
                m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,DISALARM,sOrder,m_vDisplayInfo);
                if(m_iDisMode==1)
                {
                    if(!DisplayDataInfo(m_iLine))
                        continue;
                    bfind = true;
                }
            }
            else
            {
                AnalyInfoByFile(OPERATIONLOG,DISALARM,sOrder,LogGroup::m_vDisplayInfo);
                AnalyAlarmInfoFromFile(sOrder,LogGroup::m_vDisplayInfo);
                if(m_iDisMode==1)
                {
                    if(!DisplayDataInfo(m_iLine))
                        continue;
                    bfind = true;
                }
            }                
        }
    }
    if(m_iDisMode==1)
    {
        if(bfind==false)
            printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    }
    qry.close();
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return false;
    }
    return bfind; 
}

bool LogGroup::CheckParamProcess()
{
    char sSql[1024]={0};
    int cnt=0;

    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select count(*) from wf_application a where Upper(a.exec_name) like Upper('%s')",m_sParam);
    qry.setSQL(sSql);
    qry.open ();
    if(qry.next())
        cnt = qry.field(0).asInteger();
    qry.close();
    if(cnt==0)
    {
        printf("------------Err Msg------------\r\n");
        printf("\n�Ҳ�����������Ϊ[%s]����ؼ�¼,���ѯ���̶����б�[wf_application]��ȡ��ȷ�Ľ�����\n\n",m_sParam);
        return false;
    }
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return false;
    }
    return true;
}

bool LogGroup::CheckProcessForAlarm()
{

    char sSql[1024]={0};
    int cnt=0;
    bool bfind=false;

    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select distinct(a.process_id) from wf_process a, wf_application b where "
        "a.app_id=b.app_id and Upper(b.exec_name) like Upper('%s')",m_sParam);
    qry.setSQL(sSql);
    qry.open ();
    while(qry.next())
    {
        m_iModuleId = qry.field(0).asInteger();
        if(m_iModuleId>0)
        {
            char sOrder[20]={0};
            memset(sOrder,0,sizeof(sOrder));
            sprintf(sOrder,"%d",m_iModuleId);
            m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,DISALARM,sOrder,m_vDisplayInfo);
            if(!DisplayDataInfo(m_iLine))
                continue;
            bfind = true;
        }
    }
    qry.close();
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return false;
    }
    return bfind;

}

int LogGroup::CheckParamClass()
{
    char sSql[1024]={0};
    int itypeid = -1;

    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select log_type from b_class_define a where Upper(a.log_class) like Upper('%s')",m_sParam);
    qry.setSQL(sSql);
    qry.open ();
    if(qry.next())
        itypeid = qry.field(0).asInteger();
    qry.close();

    if(itypeid==-1)
    {
        printf("------------Err Msg------------\r\n");
        printf("\n�Ҳ�����־���Ϊ[%s]����ؼ�¼,���ѯ��־������б�[b_class_define]��ȡ��ȷ����־�����\n\n",m_sParam);
    }
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
    }
    return itypeid;
}


bool LogGroup::CheckParamName()
{
    char ParamSec[50],ParamKey[50];

    memset(ParamSec,0,sizeof(ParamSec));
    memset(ParamKey,0,sizeof(ParamKey));
    
    char *p = strchr(m_sParam,'.');
    if(!p)
        return false;
    strncpy(ParamSec,m_sParam,p-m_sParam);
    p++;
    strncpy(ParamKey,p,strlen(p));

    if(!m_poLogGroup->m_poCmdCom)
    {
        printf("��ȡ���Ĳ���ʧ��");
        return false;
    }
    if(!m_poLogGroup->m_poCmdCom->GetInitParam(ParamSec,ParamKey))
        return false;
    return true;
}

void LogGroup::DisplayLogInfo(char *pFile,char *pPath)
{

	char scmd[600]={0},sFile[500]={0};
    char tempFileName_1[500]={0},tempFileName_2[500]={0};
    struct stat statbuf;
    int cnt=0;
    bool bFind=false;
    
    if (pPath[strlen(pPath)-1] != '/')
    {
        strcat(pPath, "/");
    }
    sprintf(tempFileName_1,"%s%s",pPath,pFile);
    if(lstat(tempFileName_1, &statbuf)<0)
    {
        Log::log (0, "������Ϣʧ��");
        return;
    }
    if(0 == statbuf.st_size)
    {
        Log::log (0, "��ȡ����Ϊ��");
        return;
    }  
    sprintf(tempFileName_2,"%sLogGroupTemp.log",pPath);
    sprintf(scmd,"tail -n%d %s > %s",m_iLine+2,tempFileName_1,tempFileName_2);
    system(scmd);

    ifstream infofile(tempFileName_2);
    
    if(!infofile)
    {
        Log::log (0, "������Ϣʧ��");
        return;
    }
    cout<<"*************************************"<<endl;
    while(!infofile.eof())
    {
        string::size_type pos = 0;
        infofile.getline(sFile,sizeof(sFile));
        if(infofile.gcount()==0)	
        {// ʲô��û�ж���
            continue;
        }
        int len = strlen(sFile);
        if(len==0)
            continue;
        if((cnt++)==m_iLine)
            break;
        string strFile = sFile;
        if(bFind)
        {
            cout<<strFile<<endl;
            continue;
        }
        pos = strFile.find_first_of( '[', pos );
        if(pos == string::npos)
            continue;
        bFind = true;
        string temp = strFile.substr( pos, len - pos );
        cout<<temp<<endl;
     }
    cout<<"*************************************"<<endl;
    memset(scmd,0,sizeof(scmd));
    sprintf(scmd,"rm %s",tempFileName_2);
    system(scmd); 
    return ;
}

void LogGroup::DisplayInfoByModuleDB()
{
    char sSql[1024]={0};
    int cnt=0;

    DEFINE_QUERY(qry);
    try
    {
    if(strlen(m_sParam)==0)
        sprintf(sSql,"select to_char(a.LOG_TIME,'yyyy-mm-dd hh24:mi:ss'),a.MODULE_ID,b.MODULE_NAME,a.PROCESS_ID,a.FILE_NAME,"
                    "a.LOG_FUNCTION,a.ERROR_ID,a.MSG from B_ALARM_LOG a,WF_MODULE b where a.module_id=b.module_id "
                    "and rownum<%d order by LOG_TIME desc ",m_iLine);
    else
        sprintf(sSql,"select to_char(a.LOG_TIME,'yyyy-mm-dd hh24:mi:ss'),a.MODULE_ID,b.MODULE_NAME,a.PROCESS_ID"
                    ",a.FILE_NAME,a.LOG_FUNCTION,a.ERROR_ID,a.MSG from B_ALARM_LOG a ,WF_MODULE b "
                    "where Upper(b.MODULE_NAME) like Upper('%s') and a.module_id=b.module_id and  rownum<%d order by LOG_TIME desc ",m_sParam,m_iLine);
    qry.setSQL(sSql);
    qry.open ();
    printf("---------------------------------------------------------ϵͳģ����־��Ϣ-----------------------------------------------------------\r\n\n");
    while(qry.next())
    {
        cnt++;
        printf("[LOG_TIME]%s[MODULE_ID]%d[MODULE_NAME]%s[PROCESS_ID]%d"
               "[FILE_NAME]%s[LOG_FUNCTION]%s[ERROR_ID]%s"
               "[MSG]%s\r\n"
               ,qry.field(0).asString(),qry.field(1).asInteger(),qry.field(2).asString()
               ,qry.field(3).asInteger(),qry.field(4).asString(),qry.field(5).asString()
               ,qry.field(6).asString(),qry.field(7).asString());
    }
    qry.close();
    if(cnt==0)
        printf("û�и�ģ����(%s)�Ĵ��󣬸澯�������Ϣ\n",m_sParam);
    printf("\n");

    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return;
    }
    return;
}

void LogGroup::DisplayInfoByProcessDB()
{
    char sSql[1024]={0};
    int cnt=0;

    DEFINE_QUERY(qry);
    try
    {
    if(strlen(m_sParam)==0)
        sprintf(sSql,"select to_char(LOG_TIME,'yyyy-mm-dd hh24:mi:ss'),PROCESS_ID,b.EXEC_NAME,FILE_NAME,"
                    "LOG_FUNCTION,ERROR_ID,MSG from B_ALARM_LOG a,WF_APPLICATION b where a.module_id=b.module_id "
                    "and rownum<%d order by LOG_TIME desc ",m_iLine);
    else
        sprintf(sSql,"select to_char(a.LOG_TIME,'yyyy-mm-dd hh24:mi:ss'),a.PROCESS_ID,b.EXEC_NAME"
                    ",a.FILE_NAME,a.LOG_FUNCTION,a.ERROR_ID,a.MSG from B_ALARM_LOG a ,WF_APPLICATION b "
                    "where Upper(b.exec_name) like Upper('%s') and a.module_id=b.module_id and  rownum<%d order by LOG_TIME desc ",m_sParam,m_iLine);
    qry.setSQL(sSql);
    qry.open ();
    printf("------------------------------------------------------ϵͳ���̴���/�澯��־��Ϣ-----------------------------------------------------\r\n\n");
    while(qry.next())
    {
        cnt++;
        printf("[LOG_TIME]%s[PROCESS_ID]%d[PRCESS_NAME]%s"
               "[FILE_NAME]%s[LOG_FUNCTION]%s[ERROR_ID]%s"
               "[MSG]%s\r\n"
               ,qry.field(0).asString(),qry.field(1).asInteger(),qry.field(2).asString()
               ,qry.field(3).asString(),qry.field(4).asString()
               ,qry.field(5).asString(),qry.field(6).asString());
    }
    qry.close();
    if(cnt==0)
        printf("û�иý�����(%s)�Ĵ��󣬸澯�������Ϣ\n",m_sParam);
    printf("\n");
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return;
    }
    return;
}

void LogGroup::DisplayCheckPointDB()
{
    char sSql[1024]={0};
    int cnt=0;

    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select to_char(TIME_FLAG,'yyyy-mm-dd hh24:mi:ss'),BATCH_ID,DATA_TYPE from B_CHECK_POINT_RECORD where ROWNUM<%d",m_iLine);
    qry.setSQL(sSql);
    qry.open ();
    printf("-------------CheckPointģ����־��Ϣͳ��-----------------\r\n\n");
    while(qry.next())
    {
        cnt++;
        printf("[TIME_FLAG]%s[BATCH_ID]%s[DATA_TYPE]%s\r\n"
               ,qry.field(0).asString(),qry.field(1).asString(),qry.field(2).asString());
    }
    qry.close();
    if(cnt==0)
        printf("Ŀǰû��CHECKPOINT�������־��Ϣ\n");
    printf("\n");
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return;
    }
    return;
}

void LogGroup::DisplayStaffidDB()
{
    char sSql[1024]={0};
    int cnt=0;

    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select Upper(a.staff_id),to_char(a.operate_date,'yyyy-mm-dd hh24:mi:ss'),"
                 "Upper(a.table_name),a.operate_type,a.operate_result,"
                 "a.operate_cmd from HBACCESS_OPERATE_LOG A "
                 "where Upper(a.staff_id) like Upper(%s) and ROWNUM<%d",m_sParam,m_iLine);
    qry.setSQL(sSql);
    qry.open ();
    printf("------------------------------------------------------���ݿ������ѯ��Ϣ-----------------------------------------------------\r\n\n");
    while(qry.next())
    {
        cnt++;
        printf("STAFF_ID:[%s] OPERATE_DATE:[%s] TABLE_NAME:[%s] OPERATE_TYPE:[%d] OPERATE_RESULT:[%s] OPERATE_CMD:[%s]\r\n"
               ,qry.field(0).asString(),qry.field(1).asString(),qry.field(2).asString(),
               qry.field(3).asInteger(),qry.field(4).asString(),qry.field(5).asString());
    }
    qry.close();
    if(cnt==0)
        printf("û�й���[%s]����ز�����־��Ϣ\n\n",m_sParam);
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return;
    }
    return;  
}

void LogGroup::DisplayInfoByTableNameDB()
{
    char sSql[1024]={0};
    int cnt=0;

    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select Upper(a.staff_id),to_char(a.operate_date,'yyyy-mm-dd hh24:mi:ss'),"
                 "Upper(a.table_name),a.operate_type,a.operate_result,"
                 "a.operate_cmd from HBACCESS_OPERATE_LOG A "
                 "where Upper(a.table_name) like Upper('%s') and ROWNUM<%d",m_sParam,m_iLine);
    qry.setSQL(sSql);
    qry.open ();
    printf("------------------------------------------------------���ݿ������ѯ��Ϣ-----------------------------------------------------\r\n\n");
    while(qry.next())
    {
        cnt++;
        printf("TABLE_NAME:[%s] STAFF_ID:[%s] OPERATE_DATE:[%s]  OPERATE_TYPE:[%d] OPERATE_RESULT:[%s] OPERATE_CMD:[%s]\r\n"
               ,qry.field(2).asString(),qry.field(0).asString(),qry.field(1).asString(),
               qry.field(3).asInteger(),qry.field(4).asString(),qry.field(5).asString());
    }
    qry.close();
    if(cnt==0)
        printf("û�б���[%s]����ز�����־��Ϣ\n\n",m_sParam);
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return;
    }
    return; 
}

void LogGroup::DisplayInfoBySqlTypeDB()
{
    char sSql[1024]={0};
    int cnt=0;
    int itypeid=0;

    if(strcmp(m_sParam,"insert")==0)
        itypeid = 1;
    else if(strcmp(m_sParam,"delete")==0)
        itypeid = 2;
    else if(strcmp(m_sParam,"update")==0)
        itypeid = 3;
    else if(strcmp(m_sParam,"drop")==0)
        itypeid = 4;
    
    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select Upper(a.staff_id),to_char(a.operate_date,'yyyy-mm-dd hh24:mi:ss'),"
                 "Upper(a.table_name),a.operate_type,a.operate_result,"
                 "a.operate_cmd from HBACCESS_OPERATE_LOG A "
                 "where a.operate_type=%d and ROWNUM<%d",itypeid,m_iLine);
    qry.setSQL(sSql);
    qry.open ();
    printf("------------------------------------------------------���ݿ������ѯ��Ϣ-----------------------------------------------------\r\n\n");
    while(qry.next())
    {
        cnt++;
        printf("OPERATE_TYPE:[%d] TABLE_NAME:[%s] STAFF_ID:[%s] OPERATE_DATE:[%s] OPERATE_RESULT:[%s] OPERATE_CMD:[%s]\r\n"
               ,qry.field(3).asInteger(),qry.field(2).asString(),qry.field(0).asString(),qry.field(1).asString(),
               qry.field(4).asString(),qry.field(5).asString());
    }
    qry.close();
    if(cnt==0)
        printf("û�в�������[%s]����ز�����־��Ϣ\n\n",m_sParam);
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return;
    }
    return;   
}


bool LogGroup::DisplayDataInfo(int idisNum,bool bspecify)
{
//    vector<string>::reverse_iterator r_iter;
    vector<string>::iterator iter;
    int ifindcnt=0;
    
    if(m_vDisplayInfo.size()==0)
    {
//        cout<<"**************û�в�ѯ�����ϵ���Ϣ**************"<<endl;
        return false;
    }
//    for(r_iter=m_vDisplayInfo.rbegin();r_iter!=m_vDisplayInfo.rend();++r_iter)
    for(iter=m_vDisplayInfo.begin();iter!=m_vDisplayInfo.end();++iter)
    {
        if(ifindcnt>=idisNum)
        {
            if(bspecify==true)
                break;
            if(!IfCDisplayInfo())
                break;
            ifindcnt = 0;
        }
        cout<<*iter<<endl;;
        ifindcnt++;
    }
    cout<<endl;
    m_vDisplayInfo.clear();
    return true;
}


int LogGroup::DealForModule()
{
    if(strlen(m_sParam)>0)
    {
        if(!CheckParamModule())
            return 1;
    }
    if(m_bQryFile)
    {
//        DisplayInfoByModuleDB();
        if(m_iDisMode==1)
            printf("------------------------------------------------------ϵͳģ��澯��־��Ϣ----------------------------------------------------------\r\n\n");
        CheckModuleForMem();
        if(m_iDisMode==2)
            DisplayForInterface(GROUPMODULE);
    }
    else
    {
        if(m_iDisMode==1)
            printf("------------------------------------------------------ϵͳģ��澯��־��Ϣ----------------------------------------------------------\r\n\n");
        CheckModuleForMem();
        if(m_iDisMode==2)
            DisplayForInterface(GROUPMODULE);
    }
    return 0;
}

int LogGroup::DealForProcess()
{
    if(strlen(m_sParam)>0)
    {
        if(!CheckParamProcess())
            return 1;
    }
    if(m_bQryFile)
    {
        if(strlen(m_sParam))
            AnalyInfoByFile(ATTEMPERLOG,-1,m_sParam,LogGroup::m_vDisplayInfo);
        else
             AnalyInfoByFile(ATTEMPERLOG,DISPLAYALL,m_sParam,LogGroup::m_vDisplayInfo);
    }
    else
    {
        if(strlen(m_sParam))
        {
            m_poLogGroup->AnalyzesDataInfo(ATTEMPERLOG,-1,m_sParam,m_vDisplayInfo);
        }
        else
            m_poLogGroup->AnalyzesDataInfo(ATTEMPERLOG,DISPLAYALL,m_sParam,m_vDisplayInfo);
    }
    if(m_iDisMode==1)
    {
        printf("------------------------------------------------------ϵͳ���̵�����־��Ϣ----------------------------------------------------------\r\n\n");
        if(!DisplayDataInfo(m_iLine))
                printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
        
        printf("------------------------------------------------------ϵͳ���̸澯��־��Ϣ----------------------------------------------------------\r\n\n");
    }
    if(m_bQryFile)
    {
//      DisplayInfoByProcessDB();
        char sOrder[50]={0};
        GetModuleIdByName(m_sParam,sOrder);
        AnalyInfoByFile(OPERATIONLOG,DISALARM,sOrder,LogGroup::m_vAlarmInfo);
        AnalyAlarmInfoFromFile(m_sParam,LogGroup::m_vAlarmInfo);
    }
    else
    {
        char sOrder[50]={0};
        GetModuleIdByName(m_sParam,sOrder);
        if(m_iDisMode==1)
        {
            m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,DISALARM,sOrder,m_vDisplayInfo);
            AnalyAlarmInfoFromFile(sOrder,LogGroup::m_vDisplayInfo);
        }
        else if(m_iDisMode==2)
        {
            m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,DISALARM,sOrder,m_vAlarmInfo);
            AnalyAlarmInfoFromFile(sOrder,LogGroup::m_vAlarmInfo);
        }
    }
    if(m_iDisMode==1)
    {
        if(!DisplayDataInfo(m_iLine))
            printf("**************û�в�ѯ�����ϵ���Ϣ**************\n"); 
    }
    else if(m_iDisMode==2)
        DisplayForInterface(GROUPPROCESS);
    
    return 0;
}

int LogGroup::DealForParam()
{
    if(strlen(m_sParam))
    {
        if(!CheckParamName())
        {
            printf("------------Err Msg------------\r\n");
            printf("\n�Ҳ�����������Ϊ[%s]����ؼ�¼,���ѯ���Ĳ��������ļ���ȡ��ȷ�Ĳ�����\n\n",m_sParam);
            return 1;
        }
    }
    if(m_bQryFile)
    {
//        DisplayInfoBySqlTypeDB();
        if(strlen(m_sParam))
        {
            AnalyInfoByFile(PARAMLOG,-1,m_sParam,LogGroup::m_vDisplayInfo);
        }
        else
            AnalyInfoByFile(PARAMLOG,DISPLAYALL,m_sParam,LogGroup::m_vDisplayInfo);
    }
    else
    {
//        vector<string> vParam;
//        CmdMsgFactory *pCmdMsg = new CmdMsgFactory();
//        pCmdMsg->GetHistoryMsgasString(vParam,NULL,NULL);
//        AnalyParamInfo(PARAMLOG,m_sParam,vParam,m_vDisplayInfo);
        if(strlen(m_sParam))
        {
            m_poLogGroup->AnalyzesDataInfo(PARAMLOG,DISBYPARAM,m_sParam,m_vDisplayInfo);
        }
        else
            m_poLogGroup->AnalyzesDataInfo(PARAMLOG,DISPLAYALL,m_sParam,m_vDisplayInfo);
    }
    if(m_iDisMode==1)
    {
        printf("------------------------------------------------------ϵͳ����������־��Ϣ----------------------------------------------------------\r\n\n");
        if(!DisplayDataInfo(m_iLine))
            printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    }
    else if(m_iDisMode==2)
        DisplayForInterface(GROUPPARAM);

    return 0;
}

int LogGroup::DealForLevel()
{
    int ilevel = atoi(m_sParam);
    if(ilevel<0 || ilevel>3)
    {
        printf("------------Err Msg------------\r\n");
        printf("\nָ����־�ȼ�����Ŀǰ��־�ȼ���Ϊ3��,����������hblogg -level[1/2/3]չʾ\n");
        return 1;
    }
    if(m_bQryFile)
    {
        AnalyInfoByFile(OPERATIONLOG,PINFOPOINT,m_sParam,LogGroup::m_vDisplayInfo);
    }
    else
    {
        m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,PINFOPOINT,m_sParam,m_vDisplayInfo);
    }
    if(m_iDisMode==1)
    {
        printf("------------------------------------------------------ϵͳҵ�������־��Ϣ----------------------------------------------------------\r\n\n");
        if(!DisplayDataInfo(m_iLine))
            printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    }
    else if(m_iDisMode==2)
        DisplayForInterface(GROUPINFOLV);

    return 0;
}

int LogGroup::DealForTableName()
{
    if(m_bQryFile)
    {
//      DisplayInfoByTableNameDB();
        AnalyInfoByFile(HBACCESS,-1,m_sParam,LogGroup::m_vDisplayInfo);
    }
    else
    {
        m_poLogGroup->AnalyzesDataInfo(HBACCESS,-1,m_sParam,m_vDisplayInfo);
    }
    if(m_iDisMode==1)
    {
        printf("------------------------------------------------------ϵͳ���ݿ��޸���־��Ϣ--------------------------------------------------------\r\n\n");
        if(!DisplayDataInfo(m_iLine))
            printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    }
    else if(m_iDisMode==2)
        DisplayForInterface(GROUPTABNAME);
    
    return 0;
}

int LogGroup::DealForSqlType()
{
    if(m_bQryFile)
    {
        AnalyInfoByFile(HBACCESS,-1,m_sParam,LogGroup::m_vDisplayInfo);             
    }
    else
    {
        m_poLogGroup->AnalyzesDataInfo(HBACCESS,-1,m_sParam,m_vDisplayInfo);
    }
    if(m_iDisMode==1)
    {
        printf("------------------------------------------------------ϵͳ���ݿ��޸���־��Ϣ--------------------------------------------------------\r\n\n");
        if(!DisplayDataInfo(m_iLine))
            printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    }
    else if(m_iDisMode==2)
        DisplayForInterface(GROUPSQLTYPE);

    return 0;
}

int LogGroup::DealForClass()
{
    int LogType=0;
    char tempInfo[100]={0};
    if(m_iDisMode==2)
    {
        if(strlen(m_sParam))
        {
            LogType = CheckParamClass();
            if(LogType==-1)
                return 1;
            if(LogType==OPERATIONLOG)
            {
                if(m_bQryFile)
                {
                    AnalyInfoByFile(LogType,PINFOPOINT,NULL,LogGroup::m_vDisplayInfo);
                    AnalyInfoByFile(LogType,DISALARM,NULL,LogGroup::m_vAlarmInfo);
                    AnalyAlarmInfoFromFile(NULL,LogGroup::m_vAlarmInfo);
                    AnalyInfoByFile(LogType,DISBYDML,NULL,LogGroup::m_vDmlInfo);
                }
                else
                {
                    m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,PINFOPOINT,NULL,m_vDisplayInfo);
                    m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,DISALARM,NULL,m_vAlarmInfo);
                    AnalyAlarmInfoFromFile(NULL,LogGroup::m_vAlarmInfo);
                    m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,DISBYDML,NULL,m_vDmlInfo);
                }
                DisplayForInterface(DISOPERAT);
            }
            else if(LogType==PARAMLOG)
            {
                if(m_bQryFile)
                    AnalyInfoByFile(PARAMLOG,DISPLAYALL,m_sParam,LogGroup::m_vDisplayInfo);
                else
                {
//                    vector<string> vParam;
//                    CmdMsgFactory *pCmdMsg = new CmdMsgFactory();
//                    pCmdMsg->GetHistoryMsgasString(vParam,NULL,NULL);
//                    AnalyParamInfo(PARAMLOG,m_sParam,vParam,LogGroup::m_vParamInfo);
                    m_poLogGroup->AnalyzesDataInfo(PARAMLOG,DISPLAYALL,m_sParam,LogGroup::m_vDisplayInfo);
                }
                DisplayForInterface(DISPARAM);
            }
            else if (LogType==ATTEMPERLOG)
            {
                if(m_bQryFile)
                    AnalyInfoByFile(LogType,DISPLAYALL,NULL,LogGroup::m_vDisplayInfo);
                else
                    m_poLogGroup->AnalyzesDataInfo(ATTEMPERLOG,DISPLAYALL,NULL,m_vDisplayInfo);
                //�˴�����moduleչ��
                DisplayForInterface(DISATTEMP);
            }
            else if (LogType==SYSTEMLOG)
            {
                if(m_bQryFile)
                    AnalyInfoByFile(LogType,DISPLAYALL,NULL,LogGroup::m_vDisplayInfo);
                else
                {
//                    vector<string> vParam1;
//                    CmdMsgFactory *pCmdMsg = new CmdMsgFactory();
//                    pCmdMsg->GetHistoryMsgasString(vParam1,NULL,NULL);
//                    AnalyParamInfo(PARAMLOG,m_sParam,vParam1,LogGroup::m_vDisplayInfo);
                    m_poLogGroup->AnalyzesDataInfo(SYSTEMLOG,DISPLAYALL,NULL,LogGroup::m_vDisplayInfo);
                    m_poLogGroup->AnalyzesDataInfo(PARAMLOG,DISBYSTAFF,NULL,LogGroup::m_vDisplayInfo);
                }
                DisplayForInterface(DISSYS);
            }
            else if (LogType==HBACCESS)
            {//�˴�����tablenameչ��
                if(m_bQryFile)
                    AnalyInfoByFile(LogType,DISPLAYALL,NULL,LogGroup::m_vDisplayInfo);
                else
                    m_poLogGroup->AnalyzesDataInfo(HBACCESS,DISPLAYALL,NULL,m_vDisplayInfo);
                DisplayForInterface(DISACCESS);
            }
            return 0;
        }
        if(m_bQryFile)
        {
            AnalyInfoByFile(OPERATIONLOG,PINFOPOINT,NULL,LogGroup::m_vDisplayInfo);
            AnalyInfoByFile(OPERATIONLOG,DISALARM,NULL,LogGroup::m_vAlarmInfo);
            AnalyAlarmInfoFromFile(NULL,LogGroup::m_vAlarmInfo);
            AnalyInfoByFile(OPERATIONLOG,DISBYDML,NULL,LogGroup::m_vDmlInfo);
            AnalyInfoByFile(PARAMLOG,DISPLAYALL,m_sParam,LogGroup::m_vParamInfo);
            AnalyInfoByFile(ATTEMPERLOG,DISPLAYALL,NULL,LogGroup::m_vAttemperInfo);
            AnalyInfoByFile(SYSTEMLOG,DISPLAYALL,NULL,LogGroup::m_vSysInfo);
            AnalyInfoByFile(HBACCESS,DISPLAYALL,NULL,LogGroup::m_vAccessInfo);
            DisplayForInterface(DISFORALL);
            return 0;
        }
        //ҵ����־
        m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,PINFOPOINT,NULL,m_vDisplayInfo);
        m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,DISALARM,NULL,m_vAlarmInfo);
        AnalyAlarmInfoFromFile(NULL,LogGroup::m_vAlarmInfo);
        m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,DISBYDML,NULL,m_vDmlInfo);
        //ϵͳ����
        m_poLogGroup->AnalyzesDataInfo(SYSTEMLOG,DISPLAYALL,NULL,m_vSysInfo);
        //���ݿ��޸�
        m_poLogGroup->AnalyzesDataInfo(HBACCESS,DISPLAYALL,m_sParam,m_vAccessInfo);
        //��������
//        vector<string> vParam_2;
//        CmdMsgFactory *pCmdMsg = new CmdMsgFactory();
//        pCmdMsg->GetHistoryMsgasString(vParam_2,NULL,NULL);
//        AnalyParamInfo(PARAMLOG,NULL,vParam_2,LogGroup::m_vParamInfo);
          m_poLogGroup->AnalyzesDataInfo(PARAMLOG,DISPLAYALL,NULL,LogGroup::m_vParamInfo);
        //���̵���
        m_poLogGroup->AnalyzesDataInfo(ATTEMPERLOG,DISPLAYALL,NULL,m_vAttemperInfo);
        DisplayForInterface(DISFORALL);
        return 0;
    }
    
    if(strlen(m_sParam))
    {
        LogType = CheckParamClass();
        if(LogType==-1)
            return 1;
        if(m_bQryFile)
            AnalyInfoByFile(LogType,DISPLAYALL,m_sParam,LogGroup::m_vDisplayInfo);
        else
            m_poLogGroup->AnalyzesDataInfo(LogType,DISPLAYALL,m_sParam,m_vDisplayInfo);
        AnalyAlarmInfoFromFile(NULL,LogGroup::m_vDisplayInfo);
        if(LogType==OPERATIONLOG)
            strcpy(tempInfo,"ҵ����̴�����־");
        else if(LogType==PARAMLOG)
            strcpy(tempInfo,"����������־");
        else if (LogType==ATTEMPERLOG)
            strcpy(tempInfo,"���̵�����־");
        else if (LogType==HBACCESS)
            strcpy(tempInfo,"���ݿ��޸���־");
        else if (LogType==SYSTEMLOG)
            strcpy(tempInfo,"ϵͳ������־");
        printf("---------------------------------------------------------ϵͳ%s��Ϣ-----------------------------------------------------\r\n\n",tempInfo);
        if(!DisplayDataInfo(m_iLine))
            printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
        return 0;
    }
    printf("------------------------------------------------------ϵͳҵ�������־��Ϣ----------------------------------------------------------\r\n\n");
    if(m_bQryFile)
    {
        AnalyInfoByFile(OPERATIONLOG,DISPLAYALL,NULL,LogGroup::m_vDisplayInfo);
    }
    else
    {
        m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,DISPLAYALL,NULL,m_vDisplayInfo);
        m_poLogGroup->AnalyzesDataInfo(OPERATIONLOG,DISBYDML,NULL,m_vDisplayInfo);
    }
    AnalyAlarmInfoFromFile(NULL,LogGroup::m_vDisplayInfo);
    if(!DisplayDataInfo(m_iLine,true))
        printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    
    printf("------------------------------------------------------ϵͳ���̵�����־��Ϣ----------------------------------------------------------\r\n\n");
    if(m_bQryFile)
    {
        AnalyInfoByFile(ATTEMPERLOG,DISPLAYALL,NULL,LogGroup::m_vDisplayInfo);
    }
    else
    {
        m_poLogGroup->AnalyzesDataInfo(ATTEMPERLOG,DISPLAYALL,NULL,m_vDisplayInfo);
    }
    if(!DisplayDataInfo(m_iLine,true))
        printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    
    printf("------------------------------------------------------ϵͳ������־��Ϣ--------------------------------------------------------------\r\n\n");
    if(m_bQryFile)
    {
        AnalyInfoByFile(SYSTEMLOG,DISPLAYALL,NULL,LogGroup::m_vDisplayInfo);
    }
    else
    {
        m_poLogGroup->AnalyzesDataInfo(SYSTEMLOG,DISPLAYALL,NULL,m_vDisplayInfo);
    }
    if(!DisplayDataInfo(m_iLine,true))
        printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    
    printf("------------------------------------------------------ϵͳ����������־��Ϣ----------------------------------------------------------\r\n\n");
    if(m_bQryFile)
    {
        AnalyInfoByFile(PARAMLOG,DISPLAYALL,NULL,LogGroup::m_vDisplayInfo);
    }
    else
    {
//        vector<string> vParam;
//        CmdMsgFactory *pCmdMsg = new CmdMsgFactory();
//        pCmdMsg->GetHistoryMsgasString(vParam,NULL,NULL);
//        AnalyParamInfo(PARAMLOG,m_sParam,vParam,m_vDisplayInfo);
          m_poLogGroup->AnalyzesDataInfo(PARAMLOG,DISPLAYALL,NULL,LogGroup::m_vDisplayInfo);
    }
    if(!DisplayDataInfo(m_iLine,true))
        printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    
    printf("------------------------------------------------------ϵͳ���ݿ��޸���־��Ϣ--------------------------------------------------------\r\n\n");
    if(m_bQryFile)
    {
        AnalyInfoByFile(HBACCESS,DISPLAYALL,NULL,LogGroup::m_vDisplayInfo);
    }
    else
    {
        m_poLogGroup->AnalyzesDataInfo(HBACCESS,DISPLAYALL,NULL,m_vDisplayInfo);
    }
    if(!DisplayDataInfo(m_iLine,true))
        printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    
    return 0;
}

int LogGroup::DealForCheckPoint()
{
    if(m_bQryFile)
    {
//      DisplayCheckPointDB();
        AnalyInfoByFile(SYSTEMLOG,DISBYCHECK,NULL,LogGroup::m_vDisplayInfo);
    }
    else
    {
        m_poLogGroup->AnalyzesDataInfo(SYSTEMLOG,DISBYCHECK,NULL,m_vDisplayInfo);
    }
    if(m_iDisMode==0)
    {
        printf("------------------------------------------------------ϵͳ������־��Ϣ--------------------------------------------------------------\r\n\n");
        if(!DisplayDataInfo(m_iLine))
            printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
    }
    else if(m_iDisMode==2)
        DisplayForInterface(GROUPCHECKPT);

    return 0;
}

int LogGroup::DealForStaffId()
{
    printf("------------------------------------------------------ϵͳ���Ź�����־��Ϣ--------------------------------------------------------\r\n\n");
    if(m_bQryFile)
    {
//        DisplayStaffidDB();
        AnalyInfoByFile(SYSTEMLOG,-1,m_sParam,LogGroup::m_vDisplayInfo);
        if(m_iDisMode==1)
        {
            if(!DisplayDataInfo(m_iLine))
                printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
        }
    }
    else
    {
//        bool bFind = true;
//        vector<string> vParam;
//        CmdMsgFactory *pCmdMsg = new CmdMsgFactory();
//        pCmdMsg->GetHistoryMsgasString(vParam,NULL,NULL);
//        AnalyParamInfo(SYSTEMLOG,m_sParam,vParam,m_vDisplayInfo);
        m_poLogGroup->AnalyzesDataInfo(PARAMLOG,DISBYSTAFF,m_sParam,LogGroup::m_vDisplayInfo);
//        if(m_iDisMode==1)
//        {
//            if(!DisplayDataInfo(m_iLine))
//                bFind = false;
//        }
        m_poLogGroup->AnalyzesDataInfo(HBACCESS,-1,m_sParam,m_vDisplayInfo);
        if(m_iDisMode==1)
        {
            if(!DisplayDataInfo(m_iLine))
                printf("**************û�в�ѯ�����ϵ���Ϣ**************\n");
        }
    }
    if(m_iDisMode==2)
        DisplayForInterface(GROUPSTAFFID);

    return 0;
}

bool LogGroup::prepare(char g_argv[ARG_NUM][ARG_VAL_LEN],int g_argc)
{  
    int i=0;

    if(g_argc== 1)
    {
        printUsage();
        return false;
    }
    
    while (i<g_argc) 
    {
        if (g_argv[i][0] != '-') 
        {
            i++;
            continue;
        }
        int ilen = strlen(g_argv[i]);
        switch(g_argv[i][1])
        {
             case 'a':
                    {//��־��鵵
                        if(strcmp(g_argv[i],"-a")!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�");
                            THROW(MBC_Log_Group+9);
                        }
                        m_iMode = PIGEONHOLE;
                    }
                    break;
             case 'b':
                    {//����
                        if(strcmp(g_argv[i],"-b")!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                            THROW(MBC_Log_Group+9);
                        }
                        m_iMode = GROUPBAK;
                        if(strlen(g_argv[++i]))
                        {
                            m_iGroupId = atoi(g_argv[i]);
                            if(m_iGroupId<1 || m_iGroupId>3)
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,169,"%s","Ӧ�ý��̲�������ȷ(�����ȡֵ),��־���ʶȡֵ����");
                                THROW(MBC_Log_Group+9);
                            }
                        }
                        else
                            m_iGroupId = -1;
                        if(strlen(g_argv[++i]))
                            strcpy(m_sPath,g_argv[i]);
                    }
                    break;
             case 'c':
                    {//���������ڴ�
                        if(strncmp(g_argv[i],"-checkpoint",11)==0)
                        {
                            m_iMode = GROUPCHECKPT;
                            if(strcmp(g_argv[i],"-checkpoint")!=0)
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                THROW(MBC_Log_Group+9);
                            }
                            if(strlen(g_argv[++i]))
                            {
                                if(strcmp(g_argv[i],"-file")==0)
                                {
                                    m_bQryFile = true;
                                    if(strlen(g_argv[++i]))
                                        m_iLine = atoi(g_argv[i]);
                                    else
                                        m_iLine = LIENUM;
                                }
                                else
                                {
                                    m_iLine = atoi(g_argv[i]);
                                }
                            }
                            else
                                m_iLine = LIENUM;
                        }
                        else if(strncmp(g_argv[i],"-class",6)==0)
                        {
                            m_iMode = GROUPCLASS;
                            if(strcmp(g_argv[i],"-class")!=0)
                            {//����������ȡ����
                                if(g_argv[i][6]=='[' && g_argv[i][ilen-1]==']')
                                    AnalyzesParam(g_argv[i],m_sParam);
                                else
                                {
                                    ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                    THROW(MBC_Log_Group+9);
                                }
                            }
                            if(strlen(g_argv[++i]))
                            {
                                if(strcmp(g_argv[i],"-file")==0)
                                {
                                    m_bQryFile = true;
                                    if(strlen(g_argv[++i]))
                                        m_iLine = atoi(g_argv[i]);
                                    else
                                        m_iLine = LIENUM;
                                }
                                else
                                {
                                    m_iLine = atoi(g_argv[i]);
                                }
                            }
                            else
                                m_iLine = LIENUM;;
                        }
                        else
                        {
                            if(strcmp(g_argv[i],"-c")!=0)
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                THROW(MBC_Log_Group+9);
                            }
                            m_iMode = GROUPCREAT;
                        }
                    }
                    break;
             case 'd':
                    {//ɾ����־���ļ�
                        if(strcmp(g_argv[i],"-d")!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                            THROW(MBC_Log_Group+9);
                        }
                        m_iMode = GROUPDELETE;
                        if(strlen(g_argv[++i]))
                        {
                            m_iGroupId = atoi(g_argv[i]);
                            if(m_iGroupId<1 ||m_iGroupId>3)
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,169,"%s","Ӧ�ý��̲�������ȷ(�����ȡֵ),��־���ʶȡֵ����");
                                THROW(MBC_Log_Group+9);
                            }
                        }
                        else
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,167,"%s","Ӧ�ý��̲�������ȷ(ȱ�ٱ�ѡ��)����ָ����־��");
                            THROW(MBC_Log_Group+9);
                        }
                        if(strlen(g_argv[++i]))
                            strcpy(m_sFileName,g_argv[i]);
                        else
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,167,"%s","Ӧ�ý��̲�������ȷ(ȱ�ٱ�ѡ��),��ָ��Ҫɾ�����ļ�");
                            THROW(MBC_Log_Group+9);
                        }
                    }
                    break;
             case 'e':
                    {//���ù鵵��ʶ
                        if(strcmp(g_argv[i],"-e")!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                            THROW(MBC_Log_Group+9);
                        }
                        m_iMode = GROUPSET;
                        if(strlen(g_argv[++i]))
                        {
                            m_bPigeonhole = atoi(g_argv[i]);
                            int phole = atoi(g_argv[i]);
                            if(phole<0 || phole>1)
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,169,"%s","Ӧ�ý��̲�������ȷ(�����ȡֵ),�鵵����ȡֵ����");
                                THROW(MBC_Log_Group+9);
                            }
                        }
                        else
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,167,"%s","Ӧ�ý��̲�������ȷ(ȱ�ٱ�ѡ��)����ָ���鵵����");
                            THROW(MBC_Log_Group+9);
                        } 
                    }
                    break;
             case 'f':
                    {//�ͷŹ����ڴ�
                        if(strcmp(g_argv[i],"-f")!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                            THROW(MBC_Log_Group+9);
                        }
                        m_iMode = GROUPFREE;
                    }
                    break;
             case 'h':
                    {//������Ϣ
                        if(strcmp(g_argv[i],"-help")!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                            THROW(MBC_Log_Group+9);
                        }
                    }
                    break;
             case 'i':
                    {//����ļ�
                        if(strcmp(g_argv[i],"-i")!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                            THROW(MBC_Log_Group+9);
                        }
                        m_iMode = GROUPADD;
                        if(strlen(g_argv[++i]))
                        {
                            if(strcmp(g_argv[i],"all"))
                            {
                                m_iGroupId = atoi(g_argv[i]);
                                if(m_iGroupId<1 ||m_iGroupId>3)
                                {
                                    ALARMLOG28(0,MBC_CLASS_Parameter,169,"%s","Ӧ�ý��̲�������ȷ(�����ȡֵ),��־���ʶȡֵ����");
                                    THROW(MBC_Log_Group+9);
                                }
                            }
                            else
                                m_iGroupId = 0;
                        }
                        else
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,167,"%s","Ӧ�ý��̲�������ȷ(ȱ�ٱ�ѡ��)����ָ����־���ʶ");
                            THROW(MBC_Log_Group+9);
                        }
                        if(strlen(g_argv[++i]))
                            strcpy(m_sPath,g_argv[i]);
                    }
                    break;
             case 'j':
                    {//����checkpoint
                        if(strcmp(g_argv[i],"-j")!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                            THROW(MBC_Log_Group+9);
                        }
                        m_iMode = FILECLOAD;
                        if(strlen(g_argv[++i]))
                            strcpy(m_sBatch,g_argv[i]);
                        else
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,167,"%s","Ӧ�ý��̲�������ȷ(ȱ�ٱ�ѡ��),��ָ�����κ�");
                            THROW(MBC_Log_Group+9);
                        }
                    }
                    break;
             case 'l':
                    {//��ӡ��ǰ��־����Ϣ
                        if(strncmp(g_argv[i],"-level",6)==0)
                        {
                            m_iMode = GROUPINFOLV;
                            if(strcmp(g_argv[i],"-level")!=0)
                            {//����������ȡ����ID
                                if(g_argv[i][6]=='[' && g_argv[i][ilen-1]==']')
                                    AnalyzesParam(g_argv[i],m_sParam);
                                else
                                {
                                    ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                    THROW(MBC_Log_Group+9);
                                }
                            }
                            if(strlen(g_argv[++i]))
                            {
                                if(strcmp(g_argv[i],"-file")==0)
                                {
                                    m_bQryFile = true;
                                    if(strlen(g_argv[++i]))
                                        m_iLine = atoi(g_argv[i]);
                                    else
                                        m_iLine = LIENUM;
                                }
                                else
                                {
                                    m_iLine = atoi(g_argv[i]);
                                }
                            }
                            else
                                m_iLine = LIENUM;
                        }
                        else
                        {
                            if(strcmp(g_argv[i],"-l")!=0)
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                THROW(MBC_Log_Group+9);
                            }
                            m_iMode = GROUPPRINTF;
                            if(strlen(g_argv[++i]))
                            {
                                m_iGroupId = atoi(g_argv[i]);
                                if(m_iGroupId<1 ||m_iGroupId>3)
                                {
                                    ALARMLOG28(0,MBC_CLASS_Parameter,169,"%s","Ӧ�ý��̲�������ȷ(�����ȡֵ),��־���ʶȡֵ����");
                                    THROW(MBC_Log_Group+9);
                                }
                            }
                        }
                    }
                    break;
             case 'm':
                    {
                        if(strncmp(g_argv[i],"-module",7)!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                            THROW(MBC_Log_Group+9);                            
                        }
                        m_iMode = GROUPMODULE;
                        if(strcmp(g_argv[i],"-module")!=0)
                        {//����������ȡ����ID
                            if(g_argv[i][7]=='[' && g_argv[i][ilen-1]==']')
                                AnalyzesParam(g_argv[i],m_sParam);
                            else
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                THROW(MBC_Log_Group+9);
                            }
                        }
                        if(strlen(g_argv[++i]))
                        {
                            if(strcmp(g_argv[i],"-file")==0)
                            {
                                m_bQryFile = true;
                                if(strlen(g_argv[++i]))
                                    m_iLine = atoi(g_argv[i]);
                                else
                                    m_iLine = LIENUM;
                            }
                            else
                            {
                                m_iLine = atoi(g_argv[i]);
                            }
                        }
                        else
                            m_iLine = LIENUM;
                    }
                    break;
             case 'o':
                    {//���������Ϣ
                        if(strcmp(g_argv[i],"-o")!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                            THROW(MBC_Log_Group+9);
                        }
                        m_iMode = GROUPOUT;
                        if(strlen(g_argv[++i]))
                        {
                            m_iGroupId= atoi(g_argv[i]);
                            if(m_iGroupId<1 ||m_iGroupId>3)
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,169,"%s","Ӧ�ý��̲�������ȷ(�����ȡֵ),��־���ʶȡֵ����");
                                THROW(MBC_Log_Group+9);
                            }
                            if(strlen(g_argv[++i]))
                               m_iLine = atoi(g_argv[i]);
                            else
                               m_iLine = LIENUM;
                        }
                        else
                        {
                            m_iGroupId = 0;
                            m_iLine = LIENUM;
                        }
                    }
                    break;
             case 'p':
                    {
                        if(strncmp(g_argv[i],"-process",8)==0)
                        {
                            m_iMode = GROUPPROCESS;
                            if(strcmp(g_argv[i],"-process")!=0)
                            {//����������ȡ����ID
                                if(g_argv[i][8]=='[' && g_argv[i][ilen-1]==']')
                                    AnalyzesParam(g_argv[i],m_sParam);
                                else
                                {
                                    ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                    THROW(MBC_Log_Group+9);
                                }
                            }
                            if(strlen(g_argv[++i]))
                            {
                                if(strcmp(g_argv[i],"-file")==0)
                                {
                                    m_bQryFile = true;
                                    if(strlen(g_argv[++i]))
                                        m_iLine = atoi(g_argv[i]);
                                    else
                                        m_iLine = LIENUM;
                                }
                                else
                                {
                                    m_iLine = atoi(g_argv[i]);
                                }
                            }
                            else
                                m_iLine = LIENUM;
                        }
                        else if(strncmp(g_argv[i],"-param",6)==0)
                        {
                            m_iMode = GROUPPARAM;
                            if(strcmp(g_argv[i],"-param")!=0)
                            {
                                if(g_argv[i][6]=='[' && g_argv[i][ilen-1]==']')
                                    AnalyzesParam(g_argv[i],m_sParam);
                                else
                                {
                                    ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                    THROW(MBC_Log_Group+9);
                                } 
                            }
                            if(strlen(g_argv[++i]))
                            {
                                if(strcmp(g_argv[i],"-file")==0)
                                {
                                    m_bQryFile = true;
                                    if(strlen(g_argv[++i]))
                                        m_iLine = atoi(g_argv[i]);
                                    else
                                        m_iLine = LIENUM;
                                }
                                else
                                {
                                    m_iLine = atoi(g_argv[i]);
                                }
                            }
                            else
                                m_iLine = LIENUM;
                        }
                    }
                    break;
             case 's':
                    {//�л���־��
                        if(strncmp(g_argv[i],"-staffid",8)==0)
                        {
                            m_iMode = GROUPSTAFFID;
                            if(strcmp(g_argv[i],"-staffid")!=0)
                            {//����������ȡ����ID
                                if(g_argv[i][8]=='[' && g_argv[i][ilen-1]==']')
                                    AnalyzesParam(g_argv[i],m_sParam);
                                else
                                {
                                    ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                    THROW(MBC_Log_Group+9);
                                }
                                if(strlen(g_argv[++i]))
                                {
                                    if(strcmp(g_argv[i],"-file")==0)
                                    {
                                        m_bQryFile = true;
                                        if(strlen(g_argv[++i]))
                                            m_iLine = atoi(g_argv[i]);
                                        else
                                            m_iLine = LIENUM;
                                    }
                                    else
                                    {
                                        m_iLine = atoi(g_argv[i]);
                                    }
                                }
                                else
                                    m_iLine = LIENUM;
                            }
                            else
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                THROW(MBC_Log_Group+9);                                
                            }
                        }
                        else if(strncmp(g_argv[i],"-sqltype",8)==0)
                        {
                            m_iMode = GROUPSQLTYPE;
                            if(strcmp(g_argv[i],"-sqltype")!=0)
                            {//����������ȡ����ID
                                if(g_argv[i][8]=='[' && g_argv[i][ilen-1]==']')
                                    AnalyzesParam(g_argv[i],m_sParam);
                                else
                                {
                                    ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                    THROW(MBC_Log_Group+9);
                                }
                                if(strlen(g_argv[++i]))
                                {
                                    if(strcmp(g_argv[i],"-file")==0)
                                    {
                                        m_bQryFile = true;
                                        if(strlen(g_argv[++i]))
                                            m_iLine = atoi(g_argv[i]);
                                        else
                                            m_iLine = LIENUM;
                                    }
                                    else
                                    {
                                        m_iLine = atoi(g_argv[i]);
                                    }
                                }
                                else
                                    m_iLine = LIENUM;
                            }
                            else
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                THROW(MBC_Log_Group+9);                                
                            }
                        }
                        else
                        {
                            if(strcmp(g_argv[i],"-s")!=0)
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ(��֧�ֵ�ѡ��)");
                                THROW(MBC_Log_Group+9);
                            }
                            m_iMode = GROUPQUIT;
                            if(strlen(g_argv[++i]))
                            {
                                m_iGroupId = atoi(g_argv[i]);
                                if(m_iGroupId<1 ||m_iGroupId>3)
                                {
                                    ALARMLOG28(0,MBC_CLASS_Parameter,169,"%s","Ӧ�ý��̲�������ȷ(�����ȡֵ),��־���ʶȡֵ����");
                                    THROW(MBC_Log_Group+9);
                                }
                            }
                        }
                    }
                    break;
             case 'w':
                    {//���checkpoint�ļ�
                        if(strcmp(g_argv[i],"-w")!=0)
                        {
                            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�");
                            THROW(MBC_Log_Group+9);
                        }
                        m_iMode = FILECDOWN;
                    }
                    break;
             case 't':
                    {
                        if(strncmp(g_argv[i],"-tablename",10)==0)
                        {
                            m_iMode = GROUPTABNAME;
                            if(strcmp(g_argv[i],"-tablename")!=0)
                            {//����������ȡ����ID
                                if(g_argv[i][10]=='[' && g_argv[i][ilen-1]==']')
                                    AnalyzesParam(g_argv[i],m_sParam);
                                else
                                {
                                    ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�");
                                    THROW(MBC_Log_Group+9);
                                }
                                if(strlen(g_argv[++i]))
                                {
                                    if(strcmp(g_argv[i],"-file")==0)
                                    {
                                        m_bQryFile = true;
                                        if(strlen(g_argv[++i]))
                                            m_iLine = atoi(g_argv[i]);
                                        else
                                            m_iLine = LIENUM;
                                    }
                                    else
                                    {
                                        m_iLine = atoi(g_argv[i]);
                                    }
                                }
                                else
                                    m_iLine = LIENUM;
                            }
                            else
                            {
                                ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�");
                                THROW(MBC_Log_Group+9);                                
                            }
                        }
                        else
                        {
                            m_iMode = FILETEST;
                            if(strlen(g_argv[++i]))
                                m_iLine = atoi(g_argv[i]);
                            else
                                m_iLine = LIENUM;
                        }
                    }
                    break;
             default:
                    {
                        printUsage();
                        ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�");
                        return false;
                    }
                    break;
        }
        return true;
    }
    return true;
}

int LogGroup::Deal()
{
    switch(m_iMode)
    {
        case GROUPADD:
        {           
            if(m_iGroupId==0)
            {//��ʼ���أ�Ĭ�������ļ�
                for(int num=1;num<=GROUP_LOG_NUMS;++num)
                {
                    if(!m_poLogGroup->AddLogGroupFile(num,m_sPath,true))
                        Log::log(0,"������־���ļ�ʧ��");
                    if(!m_poLogGroup->AddLogGroupFile(num,m_sPath,true))
                        Log::log(0,"������־���ļ�ʧ��");
                }
                Log::log(0,"������־���ļ����");
            }
            else
            {
                if(m_poLogGroup->AddLogGroupFile(m_iGroupId,m_sPath,true))
                    Log::log(0,"������־���ļ��ɹ�");
                else
                    Log::log(0,"������־���ļ�ʧ��");
            }
            break;
        }

        case GROUPBAK:
        {
            if(strlen(m_sPath)==0)
            {
                char *pPath = m_poLogGroup->GetGroupBakPath();
                strcpy(m_sPath,pPath);
            }
            m_poLogGroup->bakGroupFile(m_iGroupId,m_sPath);            
            Log::log(0,"��־�鱸�����");
            break;
        }

        case GROUPCREAT:
        {
            if(m_poLogGroup->create())
                Log::log(0,"��־�鹲���ڴ洴���ɹ�!");
            else
            {
                Log::log(0,"��־�鹲���ڴ洴��ʧ��");
                ALARMLOG(0,MBC_Log_Group+2,"%s","��־�鹲���ڴ洴��ʧ��");
                THROW(MBC_Log_Group+2);
            }
            break;
        }
        
        case GROUPDELETE:
        {
            if(m_poLogGroup->ClearGroupFile(m_iGroupId,m_sFileName,true))
                Log::log(0,"ɾ����־���ļ��ɹ�");
            else
            {
                Log::log(0,"ɾ����־���ļ�ʧ��");
                ALARMLOG(0,MBC_Log_Group+10,"%s","ɾ����־���ļ�ʧ��");
                THROW(MBC_Log_Group+10); 
            }
            break;
        }
        
        case GROUPFREE:
        {
            if(m_poLogGroup->remove())
                Log::log(0,"��־�鹲���ڴ�ɾ���ɹ�!");
            else
            {
                Log::log(0,"��־�鹲���ڴ�ɾ��ʧ��!");
                ALARMLOG(0,MBC_Log_Group+3,"%s","��־�鹲���ڴ�ɾ��ʧ��!");
                THROW(MBC_Log_Group+3);
            }
            break;
        }
        
        case GROUPPRINTF:
        {
            m_poLogGroup->DisplayLogGroup(m_iGroupId);
            break;
        }

        case GROUPQUIT:
        {
            int iGroupId = m_poLogGroup->GetUseGroup();
            if(m_poLogGroup->ChangeLogGroup(false,m_iGroupId)==true)
            {
                Log::log(0,"�л���־��ɹ�!");
                char stemp[200]={0};
                int itempId = m_poLogGroup->GetUseGroup();
                sprintf(stemp,"[logg] ��־������%d�л�����%d",iGroupId,itempId);
                m_poLogGroup->LogGroupWriteFile(SYSTEMLOG,stemp);
            }
            else
            {
                Log::log(0,"�л���־��ʧ��");
                ALARMLOG(0,MBC_Log_Group+6,"%s","�л���־��ʧ��");
                THROW(MBC_Log_Group+6);;
            }

            m_poLogGroup->DisplayLogGroup(m_iGroupId);            
            break;
        }

        case GROUPSET:
        {
            if(m_poLogGroup->SetLogGroupPigeonhole(m_bPigeonhole))
            {
                if(m_bPigeonhole==true)
                    Log::log(0,"��־��鵵�����Ѵ�");
                else
                    Log::log(0,"��־��鵵�����ѹر�");
            }
            break;
        }

        case GROUPOUT:
        {
            char FileName[500],FilePath[500];
            memset(FileName,0,sizeof(FileName));
            memset(FilePath,0,sizeof(FilePath));
            m_poLogGroup->GetCurUseFileName(m_iGroupId,FileName,FilePath);
            DisplayLogInfo(FileName,FilePath);
            break;
        }

        case FILECDOWN:
        {
            if(!m_poCheckPoint)
            {
                m_poCheckPoint = new CheckPointMgr();
                if(!m_poCheckPoint)
                {
                    Log::log(0,"����CheckPointMgr�������ʧ��");
                    THROW(MBC_Log_Group+5);
                }
            }
            char* sBachId = m_poCheckPoint->getCharBatchID();
            m_poLogGroup->CheckPointOut(sBachId);

            break;
        }

        case FILECLOAD:
        {
            m_poLogGroup->CheckPointIn(m_sBatch);
            break;
        }

        case PIGEONHOLE:
        {
            int lastGourpId=m_poLogGroup->GetUseGroup();
            if(m_poLogGroup->ChangeLogGroup(true,m_iGroupId)==false)
            {
                Log::log(0,"�л���־��ʧ��");
                ALARMLOG(0,MBC_Log_Group+6,"%s","�л���־��ʧ��");
                THROW(MBC_Log_Group+6);;
            }
            int curGroupId=m_poLogGroup->GetUseGroup();
            char temp[500];
            memset(temp,0,sizeof(temp));
            sprintf(temp,"[LOGG] ��־������%d�л�����%d",lastGourpId,curGroupId);
            m_poLogGroup->LogGroupWriteFile(SYSTEMLOG,temp);
            break;
        }

        case GROUPHELP:
        {
            printUsage();
            break;
        }

        case GROUPMODULE:
        {
            DealForModule();
            break;
        }

        case GROUPPROCESS:
        {
            DealForProcess();
            break;
        }

        case GROUPCHECKPT:
        {
            DealForCheckPoint();
            break;
        }

        case GROUPINFOLV:
        {
            DealForLevel();
            break;
        }

        case GROUPCLASS:
        {
            DealForClass();
            break;
        }

        case GROUPSTAFFID:
        {
            DealForStaffId();
            break;
        }

        case GROUPTABNAME:
        {
            DealForTableName();
            break;
        }

        case GROUPSQLTYPE:
        {
            DealForSqlType();
            break;
        }

        case GROUPPARAM:
        {
            DealForParam();
            break; 
        }
        
        case FILETEST:
        {
            for(int i=0;i!=100000;i++)
            {
                m_poLogGroup->LogGroupWriteFile(OPERATIONLOG, "[ALARM][ALARM_TYPE:0]--ProcId:[-1] ModuleId:[155] File:[InitServer.cpp] LineNo:[350] function:[CInitServer::ServiceStart()] ErrId:[100002004] ErrNo:[0][Msg]���������Ϲ���:SectionName:CHECKPOINT ParamName:wait_time");
                m_poLogGroup->LogGroupWriteFile(OPERATIONLOG,"[LV-1][m_oEventExtractor.initialize]����ת���������ʼ�����");
            }
            break;
        }
        
        default:
        {
            ALARMLOG28(0,MBC_CLASS_Parameter,168,"%s","Ӧ�ý��̲�������ȷ����֧�ֵ�ѡ�");
//            printUsage();
            return -1;
        }
        break;
    }
    
    return 0;
}

/*
void LogGroup::ParamMain(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc)
{
    init();
    DisInit();
    if(!prepare(sArgv,iArgc))
        return ;
    Deal();
    LogGroup::m_vDisplayInfo.clear();
    return ;
}
*/

bool LogGroup::DisplayForInterface(int LogFlag)
{
    switch(LogFlag)
    {
        case GROUPMODULE:
        case GROUPCHECKPT:
        case GROUPINFOLV:
        case GROUPSTAFFID:
        case GROUPTABNAME:
        case GROUPSQLTYPE:
        case GROUPPARAM:
        case DISPARAM:
        case DISSYS:
        case DISATTEMP:
        case DISACCESS:
            LoggProcessMain_1();
            break;
        case GROUPPROCESS:
        case DISOPERAT:
        case DISFORALL:
            LoggProcessMain(LogFlag);
            break;
        default	:
            break;
    }
    LogGroup::m_vAlarmInfo.clear();
    LogGroup::m_vParamInfo.clear();
    LogGroup::m_vSysInfo.clear();
    LogGroup::m_vAttemperInfo.clear();
    LogGroup::m_vAccessInfo.clear();
    LogGroup::m_vConverInfo.clear();
    LogGroup::m_vDmlInfo.clear();
    return true;
}

void DisplayLogg::DisInit()
{
   //����������ʱ����(��λ����)
    int INTERVAL = 1;
    //�˵���ҳ��ʾ������
    int iLoggPageSize = 30;
    int Logg_Main2SelectedRow = 1;
    int Logg_Main2StartRow = 1;
    int Logg_Main3SelectedRow =1;
    int Logg_Main3StartRow = 1;
    int iDisParamCnt=0;
    int iDisParamCnt_ALL=0;
    //ȫ�ֱ�������¼�ڲ˵�֮�䴫�ݵĲ���
    int Logg_ProblemProcSelectedRow = 1;
    int Logg_ProblemProcStartRow = 1;
    int NOW_USED=0;
    Log_Menu* Logg_pMenuInfo = NULL;
    Log_Menu* Logg_pMenuInfoAll = NULL;
    int LogFlagAll=0; 
}

void DisplayLogg::Loggstartwin()
{
    //������ֹ�ź�
    signal(SIGINT,SIG_IGN);
    initscr();
    noecho();
    intrflush(stdscr,true); 
    savetty();
    keypad(stdscr,TRUE);
}
void DisplayLogg::LoggLoadGeneralInfo()
{
    //----------------------------���ز˵���ͨ����Ϣ--------------------------------
    //�����ܱ���
    attron(A_BOLD); 
    mvaddstr(0,(COLS_LENS - strlen((char *)"��־��ѯ"))/2,(char *)"��־��ѯ");   	   	
    attroff(A_BOLD);
    
    /*���ز˵�����*/
    mvhline(3,0,'=',COLS_LENS);
    mvhline(5 + iLoggPageSize + 1,0,'=',COLS_LENS);
}
void DisplayLogg::LoggLoadEmptyMenu()
{
    mvhline(2,0,' ',112);//�����
    mvhline(4,0,' ',COLS_LENS);//�����
    for(int i=6;i<iLoggPageSize+6;i++)
    {
        mvhline(i,0,' ',COLS_LENS);	
    }
    mvhline(5 + iLoggPageSize + 3,0,' ',COLS_LENS);
    mvaddstr(5 + iLoggPageSize + 3,13,(char *)"Up Arrow = ����   Down Arrow = ����   Left Arrow = ǰҳ   Right Arrow = ��ҳ   ESC = �˳�   Enter = ����");

    return;
}


void DisplayLogg::LoggRebuildProblemProc()
{
    //�˱����������϶��»��Ʋ˵�
    int lv_iTotalRow = NOW_USED;
    int lv_iCurrentRow = 0;
    static char Buffer[COLS_LENS];
    
    LoggLoadEmptyMenu();
    //�����ĸ��ж���Ҫ���ھ���һЩ�Ƿ���
    //���ȫ��ѡ������<=0�����ָ��gv_iMag_ProcSelectedRowinTotalRow ��
    if (Logg_ProblemProcSelectedRow  <= 0)
    {
       Logg_ProblemProcSelectedRow  = lv_iTotalRow ;
    }
    //���ȫ��ѡ������> lv_iTotalRow �����ָ����һ��
    if (Logg_ProblemProcSelectedRow  > lv_iTotalRow)
    {
       Logg_ProblemProcSelectedRow  = 1;
    }
    //���g_iStartRow <= 0����Ϊ��Ϊ��һ��	
    if (Logg_ProblemProcStartRow <= 0)
    {
       Logg_ProblemProcStartRow = 1;
    }
    if (Logg_ProblemProcStartRow > Logg_ProblemProcSelectedRow )
    {
       Logg_ProblemProcStartRow = Logg_ProblemProcSelectedRow ;
    }
    if (Logg_ProblemProcSelectedRow  - Logg_ProblemProcStartRow >= iLoggPageSize)
    {
       Logg_ProblemProcStartRow = Logg_ProblemProcSelectedRow  - iLoggPageSize + 1;
    }
    // --ȷ��lv_iCurrentRow��ʼֵ
    while(1)
    {
        if (lv_iCurrentRow >= Logg_ProblemProcStartRow )
        {
            break;
        }
        else
        {
            lv_iCurrentRow++;
        }
    }	
    for(int i=0; i<iLoggPageSize; i++)
    {
        if ((lv_iCurrentRow > lv_iTotalRow)||(lv_iCurrentRow >= Logg_ProblemProcStartRow + iLoggPageSize))
        {
            break;
        }
        if (lv_iCurrentRow == Logg_ProblemProcSelectedRow)
        {
            standout();
        }
        mvhline(lv_iCurrentRow - Logg_ProblemProcStartRow + 5,0,' ',COLS_LENS);
        memset(Buffer,0,COLS_LENS);
        if(Logg_ProblemProcStartRow+i<=NOW_USED)
            sprintf(Buffer,"%s",(LogGroup::m_vConverInfo[Logg_ProblemProcStartRow+i-1]).c_str());
        mvaddstr(lv_iCurrentRow - Logg_ProblemProcStartRow + 5,0,Buffer);
        if (lv_iCurrentRow == Logg_ProblemProcSelectedRow)
        {
            standend();
        }
        lv_iCurrentRow++;
    }
    //δ����������հ�		
    while (lv_iCurrentRow < Logg_ProblemProcStartRow + iLoggPageSize)
    {
        mvhline(lv_iCurrentRow - Logg_ProblemProcStartRow + 5,0,' ',COLS_LENS);
        lv_iCurrentRow ++;
    }
    wmove(stdscr,Logg_ProblemProcSelectedRow - Logg_ProblemProcStartRow + 5,0);//��ָ���Ƶ�ѡ������
    refresh();
}
void DisplayLogg::LoggTimeBreak_ProblemProc(int signo)
{
    LoggRebuildProblemProc();
    return;
}
void DisplayLogg::LoggStartProblemProc()
{
    while(1)
    {
        LoggRebuildProblemProc();
        signal(SIGALRM, LoggTimeBreak_ProblemProc);
        alarm(INTERVAL); 
        int Key = getch();
        alarm(0);
        //�������Esc�����˳��˵�
        if (Key == 27)
        {
          Logg_ProblemProcSelectedRow=1;
          Logg_ProblemProcStartRow=1;
          break;
        }
        switch (Key)
        {
            //����س�
            case '\n':
            case '\r':
              break;
            case KEY_UP:
                Logg_ProblemProcSelectedRow--;
              break;
            case KEY_DOWN:
                Logg_ProblemProcSelectedRow++;
              break;
            case KEY_LEFT:
                Logg_ProblemProcStartRow -= iLoggPageSize;
                Logg_ProblemProcSelectedRow -= iLoggPageSize;
              break;
            case KEY_RIGHT:
                Logg_ProblemProcStartRow += iLoggPageSize;
                Logg_ProblemProcSelectedRow += iLoggPageSize;
              break;
            default	:
              break;
        }
    }
}
void DisplayLogg::LoggRebuildProcess()
{
    //�˱����������϶��»��Ʋ˵�
    int lv_iTotalRow = 0;
    int lv_iCurrentRow = 0;
    static char Buffer[COLS_LENS];
    
    LoggLoadEmptyMenu();
    lv_iTotalRow = iDisParamCnt;
    lv_iCurrentRow =1;
    //�����ĸ��ж���Ҫ���ھ���һЩ�Ƿ���
    //���ȫ��ѡ������<=0�����ָ��gv_iMainTotalRow ��
    if (Logg_Main2SelectedRow  <= 0)
    {
       Logg_Main2SelectedRow  = lv_iTotalRow ;
    }
    //���ȫ��ѡ������> lv_iTotalRow �����ָ����һ��
    if (Logg_Main2SelectedRow  > lv_iTotalRow)
    {
       Logg_Main2SelectedRow  = 1;
    }
    //���g_iStartRow <= 0����Ϊ��Ϊ��һ��	
    if (Logg_Main2StartRow <= 0)
    {
       Logg_Main2StartRow = 1;
    }
    if (Logg_Main2StartRow > Logg_Main2SelectedRow )
    {
       Logg_Main2StartRow = Logg_Main2SelectedRow ;
    }
    if (Logg_Main2SelectedRow  - Logg_Main2StartRow >= iLoggPageSize)
    {
       Logg_Main2StartRow = Logg_Main2SelectedRow  - iLoggPageSize + 1;
    }
    while(1)
    {
        if (lv_iCurrentRow >= Logg_Main2StartRow )
        {
           break;
        }
        else
        {
            lv_iCurrentRow++;
        }
    }
    while(lv_iCurrentRow <= lv_iTotalRow)
    {
        if (lv_iCurrentRow >= Logg_Main2StartRow + iLoggPageSize)
        {
            break;
        }
        if (lv_iCurrentRow == Logg_Main2SelectedRow)
        {
           standout();
        }
        mvhline(lv_iCurrentRow - Logg_Main2StartRow + 5,0,' ',COLS_LENS);
        memset(Buffer,0,COLS_LENS);
        
        //������
        sprintf(Buffer,"%d",lv_iCurrentRow);
        mvaddstr(lv_iCurrentRow - Logg_Main2StartRow + 5,0,Buffer);
        //������
//        mvaddstr(lv_iCurrentRow - Logg_Main2StartRow + 5,10,(char*)"��־��Ϣ��ѯ");
        //�������
        memset(Buffer,0,COLS_LENS);
        sprintf(Buffer,"%s",Logg_pMenuInfo[lv_iCurrentRow-1].ChName);
        mvaddstr(lv_iCurrentRow - Logg_Main2StartRow + 5,10,Buffer);
        if (lv_iCurrentRow == Logg_Main2SelectedRow)
        {
            standend();
        }
        lv_iCurrentRow++;
    }
      //δ����������հ�		
    while (lv_iCurrentRow < Logg_Main2StartRow + iLoggPageSize)
    {
        mvhline(lv_iCurrentRow - Logg_Main2StartRow + 5,0,' ',COLS_LENS);
        lv_iCurrentRow ++;
    }
    wmove(stdscr,Logg_Main2SelectedRow - Logg_Main2StartRow + 5,0);//��ָ���Ƶ�ѡ������
    refresh();
}
void DisplayLogg::LoggTimeBreak_Process(int signo)
{
    LoggRebuildProcess();
    return;
}

void DisplayLogg::LoggloadAll(vector<string> &m_vInfo)
{
    LogGroup::m_vConverInfo.clear();
    NOW_USED = m_vInfo.size();

    if(NOW_USED==0)
    {
        LogGroup::m_vConverInfo.push_back("**************û�в�ѯ�����ϵ���Ϣ**************");
        NOW_USED = LogGroup::m_vConverInfo.size();
        return ;
    }
    vector<string>::iterator iter;

    for(iter=m_vInfo.begin();iter!=m_vInfo.end();++iter)
    {
        int size = (*iter).size();
        if(size>COLS_LENS)
        {
            int index=0;
            char temp_1[512],temp_2[512];
            while(size>COLS_LENS)
            {
               memset(temp_1,0,sizeof(temp_1));
               (*iter).copy(temp_1,COLS_LENS,index);
               LogGroup::m_vConverInfo.push_back(temp_1);
               size -= COLS_LENS;
               index += COLS_LENS;
            }
            memset(temp_2,0,sizeof(temp_2));
            (*iter).copy(temp_2,COLS_LENS,index);
            LogGroup::m_vConverInfo.push_back(temp_2);
        }
        else
            LogGroup::m_vConverInfo.push_back(*iter);
    }

    NOW_USED = LogGroup::m_vConverInfo.size();
}

void DisplayLogg::LoggRebuildProcessAll()
{
    //�˱����������϶��»��Ʋ˵�
    int lv_iTotalRow = 0;
    int lv_iCurrentRow = 0;
    static char Buffer[COLS_LENS];
    
    LoggLoadEmptyMenu();
    lv_iTotalRow = iDisParamCnt_ALL;
    lv_iCurrentRow =1;
    //�����ĸ��ж���Ҫ���ھ���һЩ�Ƿ���
    //���ȫ��ѡ������<=0�����ָ��gv_iMainTotalRow ��
    if (Logg_Main3SelectedRow  <= 0)
    {
       Logg_Main3SelectedRow  = lv_iTotalRow ;
    }
    //���ȫ��ѡ������> lv_iTotalRow �����ָ����һ��
    if (Logg_Main3SelectedRow  > lv_iTotalRow)
    {
       Logg_Main3SelectedRow  = 1;
    }
    //���g_iStartRow <= 0����Ϊ��Ϊ��һ��	
    if (Logg_Main3SelectedRow <= 0)
    {
       Logg_Main3SelectedRow = 1;
    }
    if (Logg_Main3StartRow > Logg_Main3SelectedRow )
    {
       Logg_Main3StartRow = Logg_Main3SelectedRow ;
    }
    if (Logg_Main3SelectedRow  - Logg_Main2StartRow >= iLoggPageSize)
    {
       Logg_Main3StartRow = Logg_Main3SelectedRow  - iLoggPageSize + 1;
    }
    while(1)
    {
        if (lv_iCurrentRow >= Logg_Main3StartRow )
        {
           break;
        }
        else
        {
            lv_iCurrentRow++;
        }
    }
    while(lv_iCurrentRow <= lv_iTotalRow)
    {
        if (lv_iCurrentRow >= Logg_Main3StartRow + iLoggPageSize)
        {
            break;
        }
        if (lv_iCurrentRow == Logg_Main3SelectedRow)
        {
           standout();
        }
        mvhline(lv_iCurrentRow - Logg_Main3StartRow + 5,0,' ',COLS_LENS);
        memset(Buffer,0,COLS_LENS);
        
        //������
        sprintf(Buffer,"%d",lv_iCurrentRow);
        mvaddstr(lv_iCurrentRow - Logg_Main3StartRow + 5,0,Buffer);
        //������
//        mvaddstr(lv_iCurrentRow - Logg_Main2StartRow + 5,10,(char*)"��־��Ϣ��ѯ");
        //�������
        memset(Buffer,0,COLS_LENS);
        sprintf(Buffer,"%s",Logg_pMenuInfoAll[lv_iCurrentRow-1].ChName);
        mvaddstr(lv_iCurrentRow - Logg_Main3StartRow + 5,10,Buffer);
        if (lv_iCurrentRow == Logg_Main3SelectedRow)
        {
            standend();
        }
        lv_iCurrentRow++;
    }
      //δ����������հ�		
    while (lv_iCurrentRow < Logg_Main3StartRow + iLoggPageSize)
    {
        mvhline(lv_iCurrentRow - Logg_Main3StartRow + 5,0,' ',COLS_LENS);
        lv_iCurrentRow ++;
    }
    wmove(stdscr,Logg_Main3SelectedRow - Logg_Main3StartRow + 5,0);//��ָ���Ƶ�ѡ������
    refresh();
}

void DisplayLogg::LoggInitAll_1()
{
    iDisParamCnt_ALL = 3;
    Logg_pMenuInfoAll = new Log_Menu[iDisParamCnt_ALL];
    Logg_pMenuInfoAll[0].Index  = 1;
    strcpy(Logg_pMenuInfoAll[0].ChName,"ϵͳҵ��ȼ���־��Ϣ");
//    Logg_pMenuInfoAll[1].pFunc  = (GETNUM)&LoggStartProblemProc;
    Logg_pMenuInfoAll[1].Index  = 2;
    strcpy(Logg_pMenuInfoAll[1].ChName,"ϵͳ���̸澯��־��Ϣ");
//    Logg_pMenuInfoAll[0].pFunc  = (GETNUM)&LoggStartProblemProc;
    Logg_pMenuInfoAll[2].Index  = 3;
    strcpy(Logg_pMenuInfoAll[2].ChName,"ϵͳDML������־��Ϣ");
}


void DisplayLogg::LoggTimeBreak_ProblemProcAll(int signo)
{
    LoggRebuildProcessAll();
    return;
}
void DisplayLogg::LoggStartProblemProcAll()
{
    LoggLoadEmptyMenu();
    LoggInitAll_1();
    while(1)
    {
        LoggRebuildProcessAll();
        signal(SIGALRM, LoggTimeBreak_ProblemProcAll);
        alarm(INTERVAL); 
        int Key = getch();
        alarm(0);
        //�������Esc�����˳��˵�
        if (Key == 27)
        {
          break;
        }
        switch (Key)
        {
            //����س�
            case '\n':
            case '\r':
                    if(Logg_pMenuInfoAll[Logg_Main3SelectedRow-1].Index == 1)
                    {
                        LoggloadAll(LogGroup::m_vDisplayInfo);
                        LoggStartProblemProc();
                    }
                    else if(Logg_pMenuInfoAll[Logg_Main3SelectedRow-1].Index == 2)
                    {
                        LoggloadAll(LogGroup::m_vAlarmInfo);
                        LoggStartProblemProc();
                    }
                    else if(Logg_pMenuInfoAll[Logg_Main3SelectedRow-1].Index == 3)
                    {
                        LoggloadAll(LogGroup::m_vDmlInfo);
                        LoggStartProblemProc();
                    }
              break;
            case KEY_UP:
                Logg_Main3SelectedRow--;
              break;
            case KEY_DOWN:
                Logg_Main3SelectedRow++;
              break;
            default	:
              break;
        }
    }
}

void DisplayLogg::LoggInitProcessInfo(int flag)
{
    if(flag==GROUPPROCESS)
    {
        iDisParamCnt = 2;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ���̵�����־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;
        
        
        Logg_pMenuInfo[1].Index  = 2;
        strcpy(Logg_pMenuInfo[1].ChName,"ϵͳ���̸澯��־��Ϣ");
//        Logg_pMenuInfo[1].pFunc  = (GETNUM)&LoggStartProblemProc;
    }
    else if(flag==GROUPMODULE)
    {
        iDisParamCnt = 1;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"ϵͳģ��澯��־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;
    }
    else if(flag==GROUPPARAM||flag==DISPARAM)
    {
        iDisParamCnt = 1;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ�����޸���־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;
    }
    else if(flag==GROUPCHECKPT||flag==DISSYS)
    {
        iDisParamCnt = 1;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"checkpoint��־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;
    }
    else if(flag==GROUPINFOLV)
    {
        iDisParamCnt = 1;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"ϵͳҵ��ȼ���־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;        
    }
    else if(flag==GROUPSTAFFID)
    {
        iDisParamCnt = 1;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ���Ų�����־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;   
    }
    else if(flag==GROUPTABNAME)
    {
        iDisParamCnt = 1;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"ϵͳhbaccess�������־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;           
    }
    else if(flag==GROUPSQLTYPE)
    {
        iDisParamCnt = 1;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"ϵͳhbaccess��������־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;               
    }
    else if(flag==DISFORALL)
    {
        iDisParamCnt = 5;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"����������־");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;

        Logg_pMenuInfo[1].Index  = 2;
        strcpy(Logg_pMenuInfo[1].ChName,"���̵�����־");
//        Logg_pMenuInfo[1].pFunc  = (GETNUM)&LoggStartProblemProcAll;

        Logg_pMenuInfo[2].Index  = 3;
        strcpy(Logg_pMenuInfo[2].ChName,"ϵͳ������־");
//        Logg_pMenuInfo[2].pFunc  = (GETNUM)&LoggStartProblemProc;

        Logg_pMenuInfo[3].Index  = 4;
        strcpy(Logg_pMenuInfo[3].ChName,"ҵ����̴�����־");
//        Logg_pMenuInfo[3].pFunc  = (GETNUM)&LoggStartProblemProc;

        Logg_pMenuInfo[4].Index  = 5;
        strcpy(Logg_pMenuInfo[4].ChName,"hbaccess���ݿ��޸���־");
//        Logg_pMenuInfo[4].pFunc  = (GETNUM)&LoggStartProblemProc;
    }
    else if(flag==DISOPERAT)
    {
        iDisParamCnt = 3;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"ϵͳҵ��ȼ���־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;
        
        
        Logg_pMenuInfo[1].Index  = 2;
        strcpy(Logg_pMenuInfo[1].ChName,"ϵͳ���̸澯��־��Ϣ");
//        Logg_pMenuInfo[1].pFunc  = (GETNUM)&LoggStartProblemProc;

        Logg_pMenuInfo[2].Index  = 3;
        strcpy(Logg_pMenuInfo[2].ChName,"ϵͳDML������־��Ϣ");
    }
    else if(flag==DISACCESS)
    {
        iDisParamCnt = 1;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"ϵͳhbaccess������־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;       
    }
    else if(flag==DISATTEMP)
    {
        iDisParamCnt = 1;
        Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
        Logg_pMenuInfo[0].Index  = 1;
        strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ���̵�����־��Ϣ");
//        Logg_pMenuInfo[0].pFunc  = (GETNUM)&LoggStartProblemProc;   
    }
}


int DisplayLogg::LoggProcessMain(int flag)
{
    Loggstartwin();
    LoggLoadGeneralInfo();
    LoggInitProcessInfo(flag);
    while(1)
    {
        LoggRebuildProcess();
        signal(SIGALRM, LoggTimeBreak_Process);
        alarm(INTERVAL); 
        int Key = getch();
        alarm(0);
        
        //�������Esc�����˳��˵�
        if (Key == 27)
        {
          break;
        }
        switch (Key)
        {
            //����س�
            case '\n':
            case '\r':
                if(flag==GROUPPROCESS||flag==DISOPERAT)
                {
                    if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 1)
                    {
                        LoggloadAll(LogGroup::m_vDisplayInfo);
                        LoggStartProblemProc();
                    }
                    else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 2)
                    {
                        LoggloadAll(LogGroup::m_vAlarmInfo);
                        LoggStartProblemProc();
                    }
                    else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 3)
                    {
                        LoggloadAll(LogGroup::m_vDmlInfo);
                        LoggStartProblemProc();
                    }
                }
                else if(flag==GROUPMODULE||flag==GROUPPARAM||flag==GROUPCHECKPT||flag==GROUPINFOLV
                        ||flag==GROUPSTAFFID||flag==GROUPTABNAME||flag==GROUPSQLTYPE)
                {
                    if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 1)
                    {
                        LoggloadAll(LogGroup::m_vDisplayInfo);
                        LoggStartProblemProc();
                    }
                }
                else if(flag==DISPARAM||flag==DISSYS||flag==DISATTEMP||flag==DISACCESS)
                {
                    LoggloadAll(LogGroup::m_vDisplayInfo);
                    LoggStartProblemProc();
                }
                else if(flag==DISFORALL)
                {
                    if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index==1)
                    {
                        LoggloadAll(LogGroup::m_vParamInfo);
                        LoggStartProblemProc();
                    }
                    else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index==2)
                    {
                        LoggloadAll(LogGroup::m_vAttemperInfo);
                        LoggStartProblemProc();
                    }
                    else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index==3)
                    {
                        LoggloadAll(LogGroup::m_vSysInfo);
                        LoggStartProblemProc();
                    }
                    else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index==4)
                    {
                        LoggStartProblemProcAll();
                    }
                    else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index==5)
                    {
                        LoggloadAll(LogGroup::m_vAccessInfo);
                        LoggStartProblemProc();
                    }
                }
              break;
            case KEY_UP:
                Logg_Main2SelectedRow--;
              break;
            case KEY_DOWN:
                Logg_Main2SelectedRow++;
              break;
            default :
              break;
        }
    }
    //�˳���������ģʽ
    resetty();
    endwin();
    system("clear");
    return 0;
}


void DisplayLogg::LoggTimeBreak_1(int signo)
{
    LoggRebuildProblemProc();
    return;
}
void DisplayLogg::LoggProcessMain_1()
{
    Loggstartwin();
    LoggLoadGeneralInfo();
    LoggloadAll(LogGroup::m_vDisplayInfo);
    while(1)
    {
        LoggRebuildProblemProc();
        signal(SIGALRM, LoggTimeBreak_1);
        alarm(INTERVAL); 
        int Key = getch();
        alarm(0);
        //�������Esc�����˳��˵�
        if (Key == 27)
        {
          break;
        }
        switch (Key)
        {
            //����س�
            case '\n':
            case '\r':
              break;
            case KEY_UP:
                Logg_ProblemProcSelectedRow--;
              break;
            case KEY_DOWN:
                Logg_ProblemProcSelectedRow++;
              break;
            case KEY_LEFT:
                Logg_ProblemProcStartRow -= iLoggPageSize;
                Logg_ProblemProcSelectedRow -= iLoggPageSize;
              break;
            case KEY_RIGHT:
                Logg_ProblemProcStartRow += iLoggPageSize;
                Logg_ProblemProcSelectedRow += iLoggPageSize;
              break;
            default	:
              break;
        }
    }
    resetty();
    endwin();
    system("clear");
}


void LogGroupAdmin::ParamMain(char sArgv[ARG_NUM][ARG_VAL_LEN],int iArgc)
{
    LogGroup *pLogg = new LogGroup();
    pLogg->init();
    pLogg->DisInit();
    if(!pLogg->prepare(sArgv,iArgc))
        return ;
    pLogg->Deal();
    LogGroup::m_vDisplayInfo.clear();
    delete pLogg;
    pLogg = 0;
    return ;
}


   //����������ʱ����(��λ����)
int DisplayLogg::INTERVAL = 1;
//�˵���ҳ��ʾ������
int DisplayLogg::iLoggPageSize = 27;
int DisplayLogg::Logg_Main2SelectedRow = 1;
int DisplayLogg::Logg_Main2StartRow = 1;
int DisplayLogg::Logg_Main3SelectedRow =1;
int DisplayLogg::Logg_Main3StartRow = 1;
int DisplayLogg::iDisParamCnt=0;
int DisplayLogg::iDisParamCnt_ALL=0;
//ȫ�ֱ�������¼�ڲ˵�֮�䴫�ݵĲ���
int DisplayLogg::Logg_ProblemProcSelectedRow = 1;
int DisplayLogg::Logg_ProblemProcStartRow = 1;
int DisplayLogg::NOW_USED=0;
Log_Menu* DisplayLogg::Logg_pMenuInfo = NULL;
Log_Menu* DisplayLogg::Logg_pMenuInfoAll = NULL;
int DisplayLogg::LogFlagAll=0; 
