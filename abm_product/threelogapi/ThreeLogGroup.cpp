#include "ThreeLogGroup.h"
#include "ThreeLogGroupBase.h"
#include "MBC_ABM.h"

ThreeLogGroup::ThreeLogGroup(bool bGreat):ThreeLogGroupBase(),m_lMaxLogFileSize(0),
                m_lMaxLogDirSize(0),m_iCurUseGroupId(0),m_poSeqExt(0),m_iWriteMode(1),
                m_lInfoDataSize(0),m_iMaxGroupNum(0),m_iCheckPointTouch(75)
{
    char sTemp[32];
    memset(m_sBakLogGroupPath,0,sizeof(m_sBakLogGroupPath));
    memset(m_sDefaultLogGroupPath,0,sizeof(m_sDefaultLogGroupPath));
    memset(sTemp,0,sizeof(sTemp));
               

    init();
    if(bGreat==false)
    {
        GetUseGroup();
        bindGroupData(m_iCurUseGroupId);
        GetCurUsedInfoData(m_iCurUseGroupId);
        GetCurUsedInfoIndex(m_iCurUseGroupId);
        GetCurUsedInfoLock(m_iCurUseGroupId);

		//�����������ļ���ȡ��ֵ,������־�鹲���ڴ��Сһ�£�����ȡ
		m_lInfoDataSize = m_poInfoDataAll[1]->getTotal();
    }
    m_poSeqExt = new SeqMgrExt ("seq_log_group_id",1);
    
     m_iLogProid=-1;
     m_iLogAppid=-1;
     m_iLogLevel=0;    
     memset(m_ssAreaCode,0,sizeof(m_ssAreaCode));
     
     m_pErrorCode = new HBErrorCode();
     if(!m_pErrorCode)
     	 printf("create m_pErrorCode error\n");
     m_pErrorCode->init();
     
}

ThreeLogGroup::~ThreeLogGroup()
{
    if(m_poSeqExt)
    {
        delete m_poSeqExt;
        m_poSeqExt=0;
    }
}


/*
 *	�� �� �� : init
 *	�������� : ��־���ȡ���Ĳ����Ľӿ�
 *	ʱ    �� : 2010��9��13��
 *	�� �� ֵ : bool
 *	����˵�� : bRegister-�Ƿ�ע��ɹ�
*/
bool ThreeLogGroup::init()
{
    char sPath[100],sBakPath[100];
    bool balarm = false;

    memset(sPath,0,sizeof(sPath));
    memset(sBakPath,0,sizeof(sBakPath));

    //ע����Ĳ���ʧ�ܣ��������ļ���
    char sHome[254]={0};
    char *p;
    ReadIni RD;

    if ((p=getenv (ENV_HOME)) == NULL)
        sprintf (sHome, "/opt/opthb/etc");
    else
        sprintf (sHome, "%s/etc", p);
        
    char sFile[254];

    if (sHome[strlen(sHome)-1] == '/') {
        sprintf (sFile, "%sabmconfig", sHome);
    } else {
        sprintf (sFile, "%s/abmconfig", sHome);
    }
	
    m_lInfoDataSize = RD.readIniInteger(sFile,"MEMORY","log_data_size",100000);

	int iFileSize = RD.readIniInteger(sFile,"LOG","log_size",0);
    if(iFileSize==0)
    {
        Log::log(0,"���Ĳ���log.sizeû�����ã�ȡĬ���ļ���С");
        iFileSize = 200;
    }
    SetGroupMaxFileSize(iFileSize);
		
    int iGroupSize = RD.readIniInteger(sFile,"LOG","log_file_dir_size",0);
    if(iGroupSize==0)
    {
        Log::log(0,"���Ĳ���log.log_file_dir_sizeû�����ã�ȡĬ����־���С");
        iGroupSize = 700;
    }
    SetGroupMaxSize(iGroupSize);
	
    m_iCheckPointTouch = RD.readIniInteger(sFile,"LOG","log_percent_load",75);
    
    m_iWriteMode = RD.readIniInteger(sFile,"LOG","log_group_mode",1);
    
    RD.readIniString(sFile,"LOG","log_group_path",sPath,NULL);
    SetGroupDefaultPath(sPath);
    
    memset(sPath,0,sizeof(sPath));
    RD.readIniString(sFile,"LOG","1.1.path",sPath,NULL);
    
    SetGroupFilePath(1,1,sPath);
    memset(sPath,0,sizeof(sPath));
    RD.readIniString(sFile,"LOG","1.2.path",sPath,NULL);
    SetGroupFilePath(1,2,sPath);
    memset(sPath,0,sizeof(sPath));
    RD.readIniString(sFile,"LOG","2.1.path",sPath,NULL);
    SetGroupFilePath(2,1,sPath);
    memset(sPath,0,sizeof(sPath));
    RD.readIniString(sFile,"LOG","2.2.path",sPath,NULL);
    SetGroupFilePath(2,2,sPath);
    memset(sPath,0,sizeof(sPath));
    RD.readIniString(sFile,"LOG","3.1.path",sPath,NULL);
    SetGroupFilePath(3,1,sPath);
    memset(sPath,0,sizeof(sPath));
    RD.readIniString(sFile,"LOG","3.2.path",sPath,NULL);
    SetGroupFilePath(3,2,sPath);
    /*
	  memset(sPath,0,sizeof(sPath));
	  RD.readIniString(sFile,"log","4.1.path",sPath,NULL);
	  SetGroupFilePath(4,1,sPath);
	  memset(sPath,0,sizeof(sPath));
	  RD.readIniString(sFile,"log","4.2.path",sPath,NULL);
	  SetGroupFilePath(4,2,sPath);
	  memset(sPath,0,sizeof(sPath));
	  RD.readIniString(sFile,"log","5.1.path",sPath,NULL);
	  SetGroupFilePath(5,1,sPath);
	  memset(sPath,0,sizeof(sPath));
	  RD.readIniString(sFile,"log","5.2.path",sPath,NULL);
	  SetGroupFilePath(5,2,sPath);
	  memset(sPath,0,sizeof(sPath));
	  RD.readIniString(sFile,"log","6.1.path",sPath,NULL);
	  SetGroupFilePath(6,1,sPath);
	  memset(sPath,0,sizeof(sPath));
	  RD.readIniString(sFile,"log","6.2.path",sPath,NULL);
	  SetGroupFilePath(6,2,sPath);
    */
		
     RD.readIniString(sFile,"LOG","log_archive_path",sBakPath,NULL);
     SetGroupBakPath(sBakPath);
     
     m_iMaxGroupNum = RD.readIniInteger(sFile,"LOG","group_num",0);
     if(m_iMaxGroupNum==0)
     {
         Log::log(0,"��ȡ���Ĳ���log.group_numû�����ã�ȡĬ��ֵ");
         m_iMaxGroupNum = 3;
     }
       
     RD.readIniString(sFile,"SYSTEM","area_code",m_ssAreaCode,NULL);
        
     return true;
}


/*
 *	�� �� �� : InitLogGroup
 *	�������� : ��ʼ����־��
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : void
 *	����˵�� : 
*/
void ThreeLogGroup::InitLogGroup()
{
    ThreeLogGoupData * p = m_poLogGroup;
    if(!p)
    {
        LoggLog(0,"��־���ȡ�����ڴ���Ϣʧ��");
        THROW(MBC_Log_Group+4);
    }
    for(int num=1;num<=m_iMaxGroupNum;++num)
    {//��ʼ����־�飬����������Ĭ�����õĳ�Ա�ļ�
        if(!AddLogGroup(num)) continue ;
        char stemp[50],stempPath[500];
        memset(stemp,0,sizeof(stemp));
        memset(stempPath,0,sizeof(stempPath));
        sprintf(stemp,"%d_1",num);
        string sFileId_1(stemp);
        strcpy(stempPath,m_mFilePathMap.find(sFileId_1)->second.c_str());
        if(AddLogGroupFile(num,stempPath,true)==false)
        {
		Log::log(0,"%s",stempPath);
            THROW(MBC_Log_Group+7);
        }
        memset(stemp,0,sizeof(stemp));
        memset(stempPath,0,sizeof(stempPath));
        sprintf(stemp,"%d_2",num);
        string sFileId_2(stemp);
        strcpy(stempPath,m_mFilePathMap.find(sFileId_2)->second.c_str());
        if(AddLogGroupFile(num,stempPath,true)==false)
        {
            THROW(MBC_Log_Group+7);            
        }
    }
}


/*
 *	�� �� �� : AddLogGroupFile
 *	�������� : �����־���Ա�ļ�
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : bool
 *	����˵�� : GroupId-��־���ʶ;pPath-�ļ�·��;bmml-�Ƿ���Χ��������
 *             �˹��ܺ���������Χ������ӵ��û�������־���ڲ��л�������ˮ�ļ�����
*/
bool ThreeLogGroup::AddLogGroupFile(int GroupId,char * pPath,bool bmml)
{
    unsigned int i ;
    int count=-1,fd;
    long seq=0;
    char tempPath[500]={0};
    ThreeLogGoupData * p = m_poLogGroup;

    if(!p)
    {
        return false;
    }
    
    if(!m_poLogGroupIndex->get(GroupId, &i))
    {
        return false;        
    }

    if(bmml == true)
    {//�Ƿ���Դ����Χ����
        m_poDataLock->P();
        p[i].m_iSeq++;
        if(p[i].m_iSeq > GROUP_FILE_CNT)
        {
            p[i].m_iSeq--;
            m_poDataLock->V();
            Log::log(0,"������־���Ա�ļ������");
            return  false;
        }
        m_poDataLock->V();
        if(strlen(pPath)==0)
        {//20100803���ӳ�Ա�ļ�Ĭ�ϴ洢·���ĺ��Ĳ���
            if(strlen(m_sDefaultLogGroupPath))
                strcpy(pPath,m_sDefaultLogGroupPath);
            else
            {
                return false;
            }
        }
        if(strlen(pPath))
        {
            for(int j=0;j!=GROUP_FILE_CNT;j++)
            {
                if(strlen(p[i].m_File[j].m_sFileName)==0)
                {
                    count = j;
                    sprintf(p[i].m_File[count].m_sGroupPath,"%s",pPath);
                    break;
                }
            }            
        }
        if(count == -1)
        {
            return false;
        }
        m_poDataLock->P();
        strcpy(p[i].m_File[count].m_sFileName,"abmlog");
        p[i].m_File[count].m_lFileSize = 0;
        m_poDataLock->V();
        CheckLogDir(p[i].m_File[count].m_sGroupPath);
        sprintf(tempPath,"%s%s",p[i].m_File[count].m_sGroupPath,p[i].m_File[count].m_sFileName);
        if((fd=open(tempPath, O_RDWR|O_CREAT|O_TRUNC, 0770)) < 0)
        {
            return false;
        }
        close(fd);
        return true;
    }
    else
    {
        for(int j=0;j!=GROUP_FILE_CNT;j++)
        {  
            if(strlen(p[i].m_File[j].m_sFileName)==0)
                continue;
            m_poDataLock->P();
            strcpy(p[i].m_File[j].m_sFileName,"abmlog");
            p[i].m_File[j].m_lFileSize = 0;
            m_poDataLock->V();
            CheckLogDir(p[i].m_File[j].m_sGroupPath);
            sprintf(tempPath,"%s%s",p[i].m_File[j].m_sGroupPath,p[i].m_File[j].m_sFileName);
            if((fd=open (tempPath, O_RDWR|O_CREAT|O_TRUNC, 0770)) < 0)
            {
                return false;
            }
            close(fd);
        }    
    }
    return true;
}


/*
 *	�� �� �� : AddLogGroup
 *	�������� : �����־��
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : bool
 *	����˵�� : GroupId-��־���ʶ
*/
bool ThreeLogGroup::AddLogGroup(int GroupId)
{
    unsigned int i ;
	ThreeLogGoupData * p = m_poLogGroup;

    if(!m_poLogGroupIndex->get(GroupId, &i)) 
    {
        i = m_poLogGroupData->malloc();
        if(!i) 
        {
            LoggLog(0,"��־�������ڴ�ʧ��");
            THROW(MBC_Log_Group+1);
        }
        m_poIndexLock->P();
        m_poLogGroupIndex->add(GroupId,i);
        m_poIndexLock->V();
        m_poDataLock->P();
        p[i].m_iGoupId = GroupId;
        p[i].m_iSeq = 0;
        if(p[i].m_iGoupId == 1)
            p[i].m_iState = CURRENT;
        else
            p[i].m_iState = INACTIVE;
        p[i].m_lCurFileSize = 0;
        p[i].m_lCurDirSize = 0;
		p[i].m_lPredictFileSize = 0;
        p[i].m_bPigeonhole = true;
        p[i].m_bCheckPoint = false;
        p[i].m_bPigeonholeState = UNDOWN;
        p[i].m_iWritSeq = 0;
        p[i].m_iMemSeq = 0;
        p[i].m_iLevel = 5;
		p[i].m_iFileCheckPercent = 90;
		p[i].m_lFileSizeThreshold = m_lMaxLogFileSize;
        if(GroupId!=m_iMaxGroupNum)
            p[i].m_iNext = GroupId +1;
        else
            p[i].m_iNext = 1;
        m_poDataLock->V();
        return true;
    }
   return false ;

}

//add by liyr
bool  ThreeLogGroup::AddInfoDataInfo(int iCodeId,int iLogProcId,int iThreadId,int iLogAppId,int iLogLevel,
						 int iLogClassId,int iLogTypeId,int iAction,int iNetId,const char *sFlow,const char *pExpand,const char *pData,ThreeLogGoupData* p)
{
    unsigned int i=0;
    unsigned int m=0;

    if(!m_poIndexLock || !m_poDataLock || !m_poInfoDataLock)
        return false;
    if(!m_poInfoGroup ||!m_poInfoData ||!m_poInfoIndex)
        return false;

    LogInfoData * pInfo = m_poInfoGroup;
    if(!pInfo)
    {
        LoggLog(0,"��ȡ��־�������������ڴ���Ϣʧ��\n");
        return false;
    }
    m_poInfoDataLock->P();
    i = m_poInfoData->malloc();
    m_poInfoDataLock->V();
    if(i<=0)
    {
        LoggLog(0,"��־���ڴ������������ڴ�ʧ��\n");
        m_poInfoDataLock->V();
        return false;
    }
    m_dDT.getCurDate();
    pInfo[i].m_illCode = iCodeId ;
    pInfo[i].m_illAppId = iLogAppId;
    pInfo[i].m_illProId = iLogProcId;
	pInfo[i].m_iThreadId = iThreadId;
    pInfo[i].m_illLevelId = iLogLevel;
    pInfo[i].m_illClassId = iLogClassId;
    pInfo[i].m_illTypeId = iLogTypeId;
    pInfo[i].m_illActionId = iAction;
    if(pExpand==NULL) strcpy(pInfo[i].m_sActionName,"NULL");
    else              strcpy(pInfo[i].m_sActionName,pExpand);
    strncpy(pInfo[i].m_sInfo,pData,LOG_DATA_SIZE-1);
    //strncpy(pInfo[i].m_sLogTime,m_dDT.toString(),14);
	sprintf(pInfo[i].m_sLogTime,"%s%03d",m_dDT.toString(),m_dDT.getMilSec()/1000);
    if(strlen(m_ssAreaCode)==0) strcpy(pInfo[i].m_sAreaCode,"025");
    else   strncpy(pInfo[i].m_sAreaCode,m_ssAreaCode,4);
    //strcpy(pInfo[i].m_sCodeName,	m_pErrorCode->getCauseInfo(iCodeId) );
    strncpy(pInfo[i].m_sCodeName,m_pErrorCode->getCauseInfo(iCodeId),GROUP_NAME_LENGTH-1);

	if(sFlow == NULL) {
		strcpy(pInfo[i].m_sFlow,"NULL");
	} else {
		strcpy(pInfo[i].m_sFlow,sFlow);
	}
	
    pInfo[i].m_iNextOffset = 0;
    m_poIndexLock->P();
    if(m_poInfoIndex->get(iCodeId,&m))
    {
        pInfo[i].m_iNextOffset = pInfo[m].m_iNextOffset;
        pInfo[m].m_iNextOffset = i;
    }
    else
        m_poInfoIndex->add(iCodeId,i);
    m_poIndexLock->V();

	char sStr[1024] = {0};

	//�������ļ���С
	combineLog(pInfo+i,sStr,2);
	int iLen = strlen(sStr) + 1;
    m_poDataLock->P();
    p->m_iMemSeq++;
	p->m_lPredictFileSize += iLen;
    m_poDataLock->V();
    return true;

}


/*
 *	�� �� �� : bakGroupFile
 *	�������� : ����ָ����־���ļ�
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : 
 *	����˵�� : LogGroupId-Ҫ���ݵ���־���ʶ; path-Ҫ���ݵ���Ŀ¼
*/
void ThreeLogGroup::bakGroupFile(int LogGroupId,char *path)
{
    char scmd[1024]={0};
    char TempPath[500],tempName[500],tempbak[500];
    unsigned int i;

    if(strlen(path)==0)
    {
        Log::log(0,"δָ������Ŀ¼\n");
        return;
    }
    CheckLogDir(path);
    if(LogGroupId==-1)
        LogGroupId = GetUseGroup();
    if(!m_poLogGroupIndex->get(LogGroupId, &i))
    {
        Log::log(0,"������־�鹲���ڴ�ʧ��\n");
        return ;        
    }
    ThreeLogGoupData * p = m_poLogGroup + i;
    if(!p)
    {
        Log::log(0, "��ȡ��־�鹲���ڴ�����ʧ��\n");
        return;
    }
    for(int j=0;j!=GROUP_FILE_CNT;j++)
    {
        if(strlen(p->m_File[j].m_sGroupPath) && strlen(p->m_File[j].m_sFileName))
        {
            memset(tempName,0,sizeof(tempName));
            memset(TempPath,0,sizeof(TempPath));
            memset(tempbak,0,sizeof(tempbak));
            strcpy(tempName,p->m_File[j].m_sFileName);
            sprintf(TempPath,"%s%s",p->m_File[j].m_sGroupPath,p->m_File[j].m_sFileName);
            if(!GetPigeonholeFileName(tempName))
                sprintf(tempbak,"%s%s",path,p->m_File[j].m_sFileName);
            else
                sprintf(tempbak,"%s%s",path,tempName);
            sprintf(scmd,"cp -f %s %s",TempPath,tempbak);
            system(scmd);
        }
    }
}

/*
 *	�� �� �� : LoggLog
 *	�������� : ��־���ڲ���־
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : 
 *	����˵�� : iLogType=0 :ά����־ iLogType=1:������־
*/
void ThreeLogGroup::LoggLog(int iLogType,char const * format,...)
{
    char sTemp[10000], sLine[10000];
    int fd;
    char sLoggName[256];
    
    memset(sLoggName,0,sizeof(sLoggName));

    if(iLogType==0)
	    sprintf (sLoggName, "%s/Log_group_alarm.log",m_sDefaultLogGroupPath);
	else
	    sprintf (sLoggName, "%s/Log_group_bak.log",m_sBakLogGroupPath);

    Date dt;
    sprintf (sTemp, "[%s]: %s\n", dt.toString("yyyy-mm-dd hh:mi:ss"), format);

    va_list ap;
    va_start(ap, format);
    vsnprintf (sLine, 9999, sTemp, ap);
    va_end(ap);

    if((fd=open (sLoggName, O_RDWR|O_CREAT|O_APPEND, 0770)) < 0)
        return;
    write (fd, sLine, strlen (sLine));
    close (fd);
}


/*
 *	�� �� �� : CheckLogDir
 *	�������� : �������Ŀ¼�Ƿ񴴽�
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : 
 *	����˵�� : 
*/
void ThreeLogGroup::CheckLogDir(char* Path)
{
    DIR *dirp=NULL;

    if(strlen(Path)==0)
        return;
    dirp = opendir(Path);
    if(dirp==NULL)
    {
        mkdir(Path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
    }
    else
        closedir(dirp);
    if(Path[strlen(Path)-1] != '/')
    {
        strcat(Path, "/");
    }
}

/*
 *	�� �� �� : CheckLogFile
 *	�������� : �����־���ļ�
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : 
 *	����˵�� : 
*/
bool ThreeLogGroup::CheckLogFile(int iGroupId)
{
    unsigned int i;
    char tempPath[500];
    ThreeLogGoupData * p = m_poLogGroup;

   // for(int num=1;num<=m_iMaxGroupNum;++num)
    //{
        if(!m_poLogGroupIndex->get(iGroupId,&i))
            THROW(MBC_Log_Group+4);
        if(p[i].m_iSeq==0)
        {//�����ǰ��־���ļ���λ0����Ĭ�ϴ�������
            AddLogGroupFile(iGroupId,m_sDefaultLogGroupPath,true);
            AddLogGroupFile(iGroupId,m_sDefaultLogGroupPath,true);
            GetGroupFilePath(iGroupId);
            //continue;
        }
        for(int j=0;j!=GROUP_FILE_CNT;j++)
        {
            if(strlen(p[i].m_File[j].m_sFileName)==0)
                continue;
            if(strlen(p[i].m_File[j].m_sGroupPath)==0)
                continue;
            CheckLogDir(p[i].m_File[j].m_sGroupPath);
            memset(tempPath,0,sizeof(tempPath));
            sprintf(tempPath,"%s%s",p[i].m_File[j].m_sGroupPath,p[i].m_File[j].m_sFileName);
            if(access(tempPath, F_OK)!=0)
            {
                Log::log(0,"��־��%d�ĳ�Ա�ļ�:%s��ɾ�������飡",iGroupId,tempPath);
                log(MBC_FILE_LOG_FAIL,LOG_LEVEL_WARNING,LOG_TYPE_SYSTEM,"��־��%d�ĳ�Ա�ļ�:%s��ɾ�������飡",iGroupId,tempPath);
				return false;
            }
        }
    //}

	return true;
}

/*
 *	�� �� ��:   CheckParam
 *	��������:   ��������ֶ��Ƿ���ϲ�ѯ����
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   bool
 *	����˵��:   iLogClassId-��־���sParam-ִ�в���,iLogTypeId-�����ʶ
*/
bool ThreeLogGroup::CheckParam(int iLogTypeId,int iLogDisFlag,int iLogClassId,char *sParam,LogInfoData *pInfo)
{
    int iAppid=-1,iProid=-1,iLevelId=-1,iClassId=-1;

    if(iLogTypeId!=pInfo->m_illTypeId)
        return false;

    switch(iLogDisFlag)
    {
    case GROUPPARAM:
        if(strlen(sParam)==0)
            return true;
        if(strcmp(sParam,pInfo->m_sActionName)!=0)
            return false;
        break;
    case GROUPMODULE:
        if(strlen(sParam)==0)
            return true;
        iAppid = atoi(sParam);
        if(pInfo->m_illAppId!=iAppid)
            return false;
        break;
    case GROUPPROCESS:
        if(strlen(sParam)==0)
            return true;
        iProid = atoi(sParam);
        if(pInfo->m_illProId!=iProid)
            return false;
        break;
    case GROUPINFOLV:
        if(strlen(sParam)==0)
            return true;
        iLevelId = atoi(sParam);
        if(pInfo->m_illLevelId!=iLevelId)
            return false;
        break;
    case GROUPCLASS:
        if(iLogClassId==0)
            return true;
        if(iLogClassId!=pInfo->m_illClassId)
            return false;
        break;
    default:
        return false;
    }
    return true;
}

//ˢ�¹鵵·�����鵵ǰ����
void ThreeLogGroup::refreshArchivePath()
{
	char sHome[254]={0};
	char *p;
	ReadIni RD;
	
	if ((p=getenv (ENV_HOME)) == NULL)
		sprintf (sHome, "/opt/opthb/etc");
	else
		sprintf (sHome, "%s/etc", p);
	 
	char sFile[254] = {0};
	
	if (sHome[strlen(sHome)-1] == '/') {
		sprintf (sFile, "%sabmconfig", sHome);
	} else {
		sprintf (sFile, "%s/abmconfig", sHome);
	}

	char sBakPath[100] = {0};
	RD.readIniString(sFile,"LOG","log_archive_path",sBakPath,NULL);
    SetGroupBakPath(sBakPath);
}

/*
 *	�� �� �� : RefreshLogFilePath
 *	�������� : ˢ����־�ļ�·��
 *	ʱ    �� : 2011��9��28��
 *	�� �� ֵ : bool
 *	����˵�� : GroupId-��־���ʶ  iFileSeq-�ļ��ĸ���˳��   sNewPath-��·��
*/
bool  ThreeLogGroup::RefreshLogFilePath(int iGroupID,int iFileSeq,char *sNewPath)
{
    unsigned int i ;
	
	if(iGroupID > m_iMaxGroupNum || iGroupID < 1)
	{
		return false;
	}

	if(iFileSeq < 1 || iFileSeq > GROUP_FILE_CNT)
	{
		return false;
	}

	if(strlen(sNewPath) == 0)
	{
		return false;
	}
	    
    if(!m_poLogGroupIndex->get(iGroupID, &i))
    {
        return false;
    }

	ThreeLogGoupData * p = m_poLogGroup + i;
	if(!p)
    {
        return false;
    }

	char sCmd[1024] = {0};
	iFileSeq--;
	CheckLogDir(sNewPath);
	if(strcmp(p->m_File[iFileSeq].m_sGroupPath,sNewPath))
	{
		sprintf(sCmd,"cp -f %s%s %s",p->m_File[iFileSeq].m_sGroupPath,p->m_File[iFileSeq].m_sFileName,sNewPath);
				
		system(sCmd);
		
		memset(p->m_File[iFileSeq].m_sGroupPath,0,sizeof(p->m_File[iFileSeq].m_sGroupPath));

		sprintf(p->m_File[iFileSeq].m_sGroupPath,"%s",sNewPath);
	}

	return true;
}

/*
 *	�� �� �� : ChangeLogGroup
 *	�������� : �л���־��
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : bool
 *	����˵�� : GroupId-Ҫ�л�������־���ʶ;bmml-�Ƿ�������Χ����
*/
bool ThreeLogGroup::ChangeLogGroup(bool bmml,int GroupId)
{
    unsigned int icur,ilast;

    if(!m_poLogGroupIndex->get(m_iCurUseGroupId,&icur))
    {
        LoggLog(0,"������־�鹲���ڴ�ʧ��\n");
        return false;        
    }
    ThreeLogGoupData *pCur = m_poLogGroup + icur;
    if(!pCur)
    {
        LoggLog(0,"������־�鹲���ڴ�ʧ��\n");
        return false;
    }
    ThreeLogGoupData *pNext = 0;
    if(GroupId!=-1)
    {//ָ��Ŀ���л�
        if(GroupId >GROUP_LOG_NUMS || GroupId < 1 || GroupId> m_iMaxGroupNum)
        {
            LoggLog(0,"�޷�ʶ�����־��\n");
            return false;
        }
        if(!m_poLogGroupIndex->get(GroupId, &ilast))
        {
            LoggLog(0,"������־�鹲���ڴ�ʧ��\n");
            return false;
        }
        pNext =  m_poLogGroup + ilast;
        if(!pNext)
        {
            LoggLog(0,"������־�鹲���ڴ�ʧ��\n");
            return false;
        }
        if(pNext->m_iState!=INACTIVE)
        {
            if(m_iWriteMode==WRITESHARE)
            {//�����澯
                LoggLog(0,"ָ���л�����־�鲻Ϊ����̬��������ָ��\n");
                return false;
            }
        }
    }
    else
    {//�Զ��л�
        bool bFind = false;
        int iNextId = pCur->m_iNext;
        for(int i=1;i<=m_iMaxGroupNum;i++)
        {
            if(!m_poLogGroupIndex->get(iNextId, &ilast))
            {
                LoggLog(0,"������־�鹲���ڴ�ʧ��\n");
                return false;
            }
            pNext =  m_poLogGroup + ilast;
            if(!pNext)
            {
                LoggLog(0,"������־�鹲���ڴ�ʧ��\n");
                return false;
            }
            if(pNext->m_iState!=INACTIVE)
            {
                if(m_iWriteMode==WRITESHARE)
                    iNextId = pNext->m_iNext;
            }
            else
            {
                bFind = true;
				break;
            }
        }
        if(bFind==false)
        {//�޿��е���־��
            LoggLog(0,"��ǰû�п��е���־�飬������־�鼰��ֽ���״̬\n");
            return false;
        }
    }
    if(pNext->m_iSeq==0)                                   
    {//�����ǰ��־���ļ���Ϊ0����Ĭ�ϴ�������           
        AddLogGroupFile(pNext->m_iGoupId,m_sDefaultLogGroupPath,true);
        AddLogGroupFile(pNext->m_iGoupId,m_sDefaultLogGroupPath,true);
        GetGroupFilePath(pNext->m_iGoupId);
    }
    if(pNext->m_lCurDirSize == 0)
    {//����л�����־��Ϊ�գ�ֱ���л�
        m_poDataLock->P();
        pCur->m_iState = ACTIVE;
        if(m_iWriteMode==WRITESHARE)
        {
            //if(bmml)
            //{//��Դ������Ĺ鵵
            //    pCur->m_bPigeonholeState = NEEDDOWN;
            //}
			pCur->m_bPigeonholeState = NEEDDOWN;
        }
        pNext->m_iState = CURRENT;
        pNext->m_bCheckPoint = false;
        pNext->m_bPigeonholeState = UNDOWN;
		pNext->m_lPredictFileSize = 0;
        m_iCurUseGroupId = pNext->m_iGoupId;
        m_poDataLock->V();
		if(bmml)
		{
			CommitInfoFile();
		}
        //���¶�λ������
        bindGroupData(m_iCurUseGroupId);
        GetCurUsedInfoData(m_iCurUseGroupId);
        GetCurUsedInfoIndex(m_iCurUseGroupId);
        GetCurUsedInfoLock(m_iCurUseGroupId);
        return true;
    }

    if(pNext->m_bPigeonhole == true && pNext->m_bPigeonholeState != ALLDOWN)
    {//��Ҫ�鵵
    	refreshArchivePath();
        bakGroupFile(pNext->m_iGoupId,m_sBakLogGroupPath);    
        m_poDataLock->P();
        pNext->m_iState = INACTIVE;
        pNext->m_bPigeonholeState=ALLDOWN;
        m_poDataLock->V();
    }
    char tempPath[500];
    for(int num =0;num!=GROUP_FILE_CNT;++num)
    {//����Ҫ�鵵��ֱ������
        if(strlen(pNext->m_File[num].m_sFileName)==0)
            continue;
        memset(tempPath,0,sizeof(tempPath));
        sprintf(tempPath,"%s%s",pNext->m_File[num].m_sGroupPath,pNext->m_File[num].m_sFileName);
        if(ClearGroupFile(pNext->m_iGoupId,tempPath)==false)
            return false;
    }
    if(AddLogGroupFile(pNext->m_iGoupId,NULL)==false)
        return false;

    m_poDataLock->P();
    pCur->m_iState = ACTIVE;
    //if(bmml)
    //{//��Դ������Ĺ鵵
    //    pCur->m_bPigeonholeState = NEEDDOWN;
    //}

	pCur->m_bPigeonholeState = NEEDDOWN;
	
    pNext->m_iState = CURRENT;
    pNext->m_bCheckPoint = false;
    pNext->m_bPigeonholeState=UNDOWN;
    m_iCurUseGroupId = pNext->m_iGoupId;
    pNext->m_lCurDirSize = 0;
    pNext->m_lCurFileSize = 0;
	pNext->m_lPredictFileSize = 0;
    m_poDataLock->V();
	if(bmml)
	{
		CommitInfoFile();
	}
    //���¶�λ������
    bindGroupData(m_iCurUseGroupId);
    GetCurUsedInfoData(m_iCurUseGroupId);
    GetCurUsedInfoIndex(m_iCurUseGroupId);
    GetCurUsedInfoLock(m_iCurUseGroupId);
    return true;
}


/*
 *	�� �� �� : ClearTempGroup
 *	�������� : ɾ����־���Ա�ļ�
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : bool
 *	����˵�� : GroupId-��־���ʶ; pFileName-��Ա�ļ���; bmml-�Ƿ���Χ��������
*/
bool ThreeLogGroup::ClearGroupFile(int LogGroupId,char *pFileName,bool bmml)
{
    char scmd[1024]={0};
    char tempPath[500];
    unsigned int i;

    if(LogGroupId >GROUP_LOG_NUMS || LogGroupId < 1 || LogGroupId > m_iMaxGroupNum)
    {
        LoggLog(0,"�޷�ʶ�����־��\n");
        return false;        
    }
    if(!m_poLogGroupIndex->get(LogGroupId,&i))
    {
        LoggLog(0,"������־�鹲���ڴ�ʧ��\n");
        return false;        
    }

    ThreeLogGoupData * p = m_poLogGroup + i;
        
    if(!p)
    {
        LoggLog(0,"������־�鹲���ڴ�ʧ��\n");
        return false;
    }

    for(int j=0;j!=GROUP_FILE_CNT;j++)
    {
        if(strlen(p->m_File[j].m_sFileName))
        {
            memset(tempPath,0,sizeof(tempPath));
            sprintf(tempPath,"%s%s",p->m_File[j].m_sGroupPath,p->m_File[j].m_sFileName);
            if(strcmp(tempPath,pFileName)==0)
            {
                sprintf(scmd,"rm -r %s",tempPath);
                system(scmd);
                m_poDataLock->P();
                p->m_File[j].m_lFileSize = 0;
                if(bmml==true)
                {
                    LoggLog(0,"��־��%d��ɾ����һ����Ա�ļ�:%s",p->m_iGoupId,p->m_File[j].m_sFileName);
                    p->m_iSeq--;
                    memset(p->m_File[j].m_sFileName,0,sizeof(p->m_File[j].m_sFileName));
                    memset(p->m_File[j].m_sGroupPath,0,sizeof(p->m_File[j].m_sGroupPath));
                }
                m_poDataLock->V();
                return true;
            }
        }           
    }
    LoggLog(0,"����־��û���ҵ������������ļ�������\n");
    return false;
}

/*
 *	�� �� �� : DisplayLogGroup
 *	�������� : չ��ָ����־����Ϣ
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : 
 *	����˵�� : GroupId-Ҫչ�ֵ���־���ʶ
*/
bool ThreeLogGroup::DisplayLogGroup(int GroupId,char * sDest)
{
    unsigned int i;
/*
    if(m_bAttached==false)
    {
        THROW(MBC_Log_Group+4);
    }
*/
	if(m_bAttached == false)
	{
		initBase();
		GetUseGroup();
		bindGroupData(m_iCurUseGroupId);
		GetCurUsedInfoData(m_iCurUseGroupId);
		GetCurUsedInfoIndex(m_iCurUseGroupId);
		GetCurUsedInfoLock(m_iCurUseGroupId);
	}

    if(GetUseGroup()<=0)
    {
        THROW(MBC_Log_Group+12);
    }
    if(GroupId == -1)
        GroupId = m_iCurUseGroupId;           

    if(GroupId > GROUP_LOG_NUMS || GroupId <1 || GroupId > m_iMaxGroupNum)
    {
        THROW(MBC_Log_Group+11);
    }

    if(!m_poLogGroupIndex->get(GroupId, &i))
    {
        THROW(MBC_Log_Group+4);
    }

    ThreeLogGoupData * p = m_poLogGroup + i;
    if(!p)
    {
        THROW(MBC_Log_Group+4);
    }


	char sStr[10240] = {0},sStrTmp[1024] = {0};
	sprintf(sStrTmp,"��־���ʶ:%d(%d)\n",p->m_iGoupId,m_iMaxGroupNum);
	strcat(sStr,sStrTmp);
    if(p->m_iState==CURRENT)
        sprintf(sStrTmp,"��־��״̬:CURRENT\n");
    else if(p->m_iState==INACTIVE)
        sprintf(sStrTmp,"��־��״̬:INACTIVE\n");
    else if(p->m_iState==ACTIVE)
        sprintf(sStrTmp,"��־��״̬:ACTIVE\n");
    else
        return false;
	strcat(sStr,sStrTmp);
	
    if(p->m_bPigeonhole)
        sprintf(sStrTmp,"��־��鵵��ʶ:��\n");
    else
        sprintf(sStrTmp,"��־��鵵��ʶ:�ر�\n");
	strcat(sStr,sStrTmp);

    if(p->m_bPigeonholeState==UNDOWN)
        sprintf(sStrTmp,"��־��鵵״̬:δ�鵵\n");
    else if(p->m_bPigeonholeState==ALLDOWN)
        sprintf(sStrTmp,"��־��鵵״̬:�ѹ鵵\n");
    else if(p->m_bPigeonholeState==NEEDDOWN)
        sprintf(sStrTmp,"��־��鵵״̬:�ȴ��鵵\n");
    else if(p->m_bPigeonholeState==FILEDOWN)
        sprintf(sStrTmp,"��־��鵵״̬:�����\n");	
	strcat(sStr,sStrTmp);

    if(m_iWriteMode==WRITEFILE)
        sprintf(sStrTmp,"��־��д��ʽ: �ļ�\n");
    else if(m_iWriteMode==WRITESHARE)
        sprintf(sStrTmp,"��־��д��ʽ: �ڴ�\n");
	strcat(sStr,sStrTmp);

	sprintf(sStrTmp,"��־��ȼ�:%d\n",p->m_iLevel);
	strcat(sStr,sStrTmp);
        
    sprintf(sStrTmp,"��־���ļ���:%d\n",p->m_iSeq);
	strcat(sStr,sStrTmp);

	sprintf(sStrTmp,"��־���Ա�ļ��б�:\n");
	strcat(sStr,sStrTmp);

	DisplayFileList(p->m_iGoupId,sStrTmp);
	strcat(sStr,sStrTmp);

    if(m_iWriteMode!=WRITESHARE)
    {//ֱ��д�ļ���ʱ�����Ҫչʾ
        double size = GetGroupSize(GroupId);
        double used = GetGroupPercent(GroupId);
        double unused = 1-used;
        printf("�߼��ռ�ʹ��:%6.3f M\n",size);
        printf("�߼��ռ�ʹ����:%6.3f %%\n",used*100);
        printf("�߼��ռ������:%6.3f %%\n",unused*100);
    }
    sprintf(sStrTmp,"��������:%d \n",p->m_iWritSeq);
	strcat(sStr,sStrTmp);
    sprintf(sStrTmp,"�ڴ�����:%d \n",p->m_iMemSeq);
	strcat(sStr,sStrTmp);
    sprintf(sStrTmp,"�ڴ�ʹ��:%d \n",GetGroupInfoUsed(GroupId));
	strcat(sStr,sStrTmp);
    //sprintf(sStrTmp,"�ڴ�����:%d \n",m_lInfoDataSize);
	sprintf(sStrTmp,"�ڴ�����:%d \n",GetGroupInfoTotal(GroupId));
	strcat(sStr,sStrTmp);
    sprintf(sStrTmp,"�ڴ�ʹ����:%6.3f %%\n",(GetGroupInfoPercent(GroupId))*100);
	strcat(sStr,sStrTmp);
    //sprintf(sStrTmp,"�ļ���С����:%ld M\n",(m_lMaxLogFileSize/(1024*1024)));
    sprintf(sStrTmp,"�ļ���С����:%ld M\n",(p->m_lFileSizeThreshold/(1024*1024)));
	strcat(sStr,sStrTmp);
    sprintf(sStrTmp,"��־���С����:%ld M\n",(m_lMaxLogDirSize/(1024*1024)));
	strcat(sStr,sStrTmp);
    sprintf(sStrTmp,"��־��鵵�ļ�����Ŀ¼:%s \n\n",m_sBakLogGroupPath);
	strcat(sStr,sStrTmp);

	if(sDest)
	{
		strcpy(sDest,sStr);
	} 
	else
	{
		printf("*******************************************\n");	
		printf("%s",sStr);
		printf("*******************************************\n");
	}
	return true;
}


/*
 *	�� �� �� : DisplayFileList
 *	�������� : չ��ָ����־���Ա�ļ���Ϣ
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : 
 *	����˵�� : GroupId-Ҫչ�ֵ���־���ʶ
*/
void ThreeLogGroup::DisplayFileList(int GroupId,char *sDest)
{
    unsigned int i;
    
    if(!m_poLogGroupIndex->get(GroupId, &i))
    {
        THROW(MBC_Log_Group+4);
    }
    ThreeLogGoupData * p = m_poLogGroup + i;
    if(!p)
    {
        THROW(MBC_Log_Group+4);
    }

	char sStr[2048] = {0},sStrTmp[1024] = {0};
    for(int j=0;j!=GROUP_FILE_CNT;++j)
    {
        double dFileSize=0;
        if(strlen(p->m_File[j].m_sFileName)==0)
            continue;
        
        sprintf(sStrTmp," �ļ���  :%s\n",p->m_File[j].m_sFileName);
		strcat(sStr,sStrTmp);
		
        sprintf(sStrTmp," �ļ�·��:%s\n",p->m_File[j].m_sGroupPath);
		strcat(sStr,sStrTmp);
		
        if(p->m_File[j].m_lFileSize < 1024)
            sprintf(sStrTmp," �ļ���С:%ld Byte\n",p->m_File[j].m_lFileSize);
        else if(p->m_File[j].m_lFileSize < 1024 *1024)
        {
            dFileSize = p->m_File[j].m_lFileSize/1024.000;
            sprintf(sStrTmp," �ļ���С:%6.3f K\n",dFileSize);
        }
        else
        {
            dFileSize = ((int)(p->m_File[j].m_lFileSize/1024.000)/1024.000);
            sprintf(sStrTmp," �ļ���С:%6.3f M\n",dFileSize);
        }
		strcat(sStr,sStrTmp);
       	strcat(sStr,"\n");
    }

	if(sDest)
	{
		strcpy(sDest,sStr);
	}
	else
	{
		printf("%s",sStr);
	}
	
    return ;
}

/*
 *	�� �� �� : GetGroupNum
 *	�������� : ��ȡ��ǰ��־�����
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : int
 *	����˵�� : 
*/
int ThreeLogGroup::GetGroupNum()
{
    return m_iMaxGroupNum;
}


/*
 *	�� �� �� : GetUseGroup
 *	�������� : ��ȡ��ǰ�������е���־���ʶ
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : int
 *	����˵�� : 
*/
int ThreeLogGroup::GetUseGroup()
{
    unsigned int i;
    ThreeLogGoupData * p = m_poLogGroup;
    bool bFind = false;
/*
    if(m_bAttached==false)
       return -1;
*/    
    if(!p)
    {
//        Log::log(0,"��־���ȡ�����ڴ���Ϣʧ��\n");
        LoggLog(0,"��־���ȡ�����ڴ���Ϣʧ��\n");
        return -1;
    }

    for(int num=1;num<=m_iMaxGroupNum;++num)
    {
        if(!m_poLogGroupIndex->get(num,&i))
        {
//            Log::log(0,"��־���ȡ�����ڴ���Ϣʧ��\n");
            LoggLog(0,"��־���ȡ�����ڴ���Ϣʧ��\n");
            return -1;            
        }
        p = m_poLogGroup + i;
        if(p->m_iState==CURRENT)
        {
            m_iCurUseGroupId = p->m_iGoupId;
            bFind = true;
            break;
        }
    }
    if(bFind==false)
    {
//        Log::log(0,"��־��״̬����\n");
        LoggLog(0,"��־��״̬����\n");
        return -1;    
    }
    return m_iCurUseGroupId;
}


/*
 *	�� �� �� : GetGroupSize
 *	�������� : ��ȡָ����־���С
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : ��־���С(��λM)
 *	����˵�� : GroupId-��־���ʶ
*/
double ThreeLogGroup::GetGroupSize(int GroupId)
{
    unsigned int i;
    
    if(GroupId >GROUP_LOG_NUMS || GroupId < 1 || GroupId>m_iMaxGroupNum)
        GroupId = m_iCurUseGroupId;
    if(!m_poLogGroupIndex->get(GroupId,&i))
    {
        THROW(MBC_Log_Group+4);
    }
    ThreeLogGoupData * p  = m_poLogGroup + i;
    if(!p)
    {
        THROW(MBC_Log_Group+4);
    }
    if(p->m_iGoupId == GroupId)
    {
       double dGroupSize = ((int)(p->m_lCurDirSize/1024.000)/1024.000);
       dGroupSize = floor(dGroupSize*100.000)/100.000;
       return dGroupSize;
    }
    return 0;
}

/*
 *	�� �� �� : GetGroupPercent
 *	�������� : ��ȡָ����־��������
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : double
 *	����˵�� : GroupId-��־���ʶ
*/

double ThreeLogGroup::GetGroupPercent(int GroupId)
{
    unsigned int i;
    
    if(GroupId >GROUP_LOG_NUMS || GroupId < 1 || GroupId>m_iMaxGroupNum)
        GroupId = m_iCurUseGroupId;    
    if(!m_poLogGroupIndex->get(GroupId,&i))
    {
        THROW(MBC_Log_Group+4);
    }
    ThreeLogGoupData * p  = m_poLogGroup + i;
    if(!p)
    {
        THROW(MBC_Log_Group+4);
    }
    if(p->m_iGoupId == GroupId)
    {
        double temp = p->m_lCurDirSize/(m_lMaxLogDirSize*1.000);
        return temp;
    }
    return 0;
}

/*
 *	�� �� �� : GetGroupInfoPercent
 *	�������� : ��ȡ������־�鹲���ڴ���������
 *	ʱ    �� : 2010��9��3��
 *	�� �� ֵ : double
 *	����˵�� : GroupId-��־���ʶ
*/

double ThreeLogGroup::GetGroupInfoPercent(int GroupId)
{
    int usedcnt = GetGroupInfoUsed(GroupId);
    if(usedcnt==0)
        return 0;
    //double temp = usedcnt/(m_lInfoDataSize*1.000);

    int totalcnt = GetGroupInfoTotal(GroupId);

	double temp = usedcnt*1.0/totalcnt;  
	
    return temp;
}

/*
 *	�� �� �� : GetGroupState
 *	�������� : ��ȡָ����־��״̬
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : double
 *	����˵�� : GroupId-��־���ʶ
*/
int ThreeLogGroup::GetGroupState(int GroupId,char *sRetVal)
{
    unsigned int i;
	if(m_bAttached == false)
	{
		initBase();
		GetUseGroup();
		bindGroupData(m_iCurUseGroupId);
		GetCurUsedInfoData(m_iCurUseGroupId);
		GetCurUsedInfoIndex(m_iCurUseGroupId);
		GetCurUsedInfoLock(m_iCurUseGroupId);
	}


    if(GroupId >GROUP_LOG_NUMS || GroupId < 1 || GroupId>m_iMaxGroupNum)
        GroupId = m_iCurUseGroupId;    
    if(!m_poLogGroupIndex->get(GroupId,&i))
    {
        THROW(MBC_Log_Group+4);
    }
    ThreeLogGoupData * p  = m_poLogGroup + i;
    if(!p)
    {
        THROW(MBC_Log_Group+4);
    }
    if(p->m_iGoupId == GroupId)
    {
    	if(sRetVal == NULL) 
		{
			return p->m_iState;
    	}

		strcpy(sRetVal,"UnKnown");
		if(p->m_iState == CURRENT)
    	{
    		strcpy(sRetVal,"����");
    	} 
		else if (p->m_iState == INACTIVE)
		{
			strcpy(sRetVal,"����");
		}
		else if(p->m_iState == ACTIVE)
		{
			strcpy(sRetVal,"�����");
		}
		
        return p->m_iState;
    }
	
    return 0;
}

/*
 *	�� �� �� : GetGroupWriteMode
 *	�������� : ��ȡ��־��д���ݷ�ʽ
 *	ʱ    �� : 2010��9��9��
 *	�� �� ֵ : int
 *	����˵�� : 0-д�ļ� 1-д�����ڴ�
*/
int ThreeLogGroup::GetGroupWriteMode()
{
    return m_iWriteMode;
}

/*
 *	�� �� �� : GetGroupPigeonhole
 *	�������� : ��ȡָ����־��鵵��ʶ״̬
 *	ʱ    �� : 2010��9��9��
 *	�� �� ֵ : bool
 *	����˵�� : GroupId-��־���ʶ
*/
bool ThreeLogGroup::GetGroupPigeonhole(int GroupId)
{
    unsigned int i;
    
    if(GroupId >GROUP_LOG_NUMS || GroupId < 1 || GroupId>m_iMaxGroupNum)
        GroupId = m_iCurUseGroupId;    
    if(!m_poLogGroupIndex->get(GroupId,&i))
    {
        THROW(MBC_Log_Group+4);
    }
    ThreeLogGoupData * p  = m_poLogGroup + i;
    if(!p)
    {
        THROW(MBC_Log_Group+4);
    }
    if(p->m_iGoupId == GroupId)
        return p->m_bPigeonhole;
    return false;    
}

/*
 *	�� �� �� : UpdateLogFileArchiveSeq
 *	�������� : ����sequence��1��ʼ
 *	ʱ    �� : 2011��9��15��
 *	�� �� ֵ : void
 *	����˵�� : 
*/
void ThreeLogGroup::UpdateLogFileArchiveSeq()
{
	#ifdef __SQLITE__
		
	DEFINE_QUERY (qry);
	char sSQL[512] = {0};
	sprintf (sSQL, "update sqlite_sequence set seq = 1 where name = 'seq_log_group_id'");
	qry.setSQL (sSQL); 
	qry.execute ();
	qry.commit ();
	qry.close ();

	#endif

	return;
}


/*
 *	�� �� �� : GetPigeonholeFileName
 *	�������� : ��ȡ�鵵�ļ���
 *	ʱ    �� : 2010��9��15��
 *	�� �� ֵ : bool
 *	����˵�� : pFileName -���ص��ļ���
*/
bool ThreeLogGroup::GetPigeonholeFileName(char *pFileName)
{
    char *p;

/*    
    char temp[500];

    p = strchr(pFileName,'_');
    if(!p)
        return false;
    p++;
    memset(temp,0,sizeof(temp));
    strcpy(temp,p);
*/
    memset(pFileName,0,sizeof(pFileName));
    long seq = m_poSeqExt->getNextVal();
    Date dt;
    sprintf(pFileName,"abmlog_archive_%s%04ld",dt.toString("yyyymmdd"),seq);
    return true;
}

/*
 *	�� �� �� : GetGroupFileSeq
 *	�������� : ��ȡָ����־���Ա�ļ���
 *	ʱ    �� : 2010��9��9��
 *	�� �� ֵ : bool
 *	����˵�� : GroupId-��־���ʶ
*/
int ThreeLogGroup::GetGroupFileSeq(int GroupId)
{
    unsigned int i;
    
    if(GroupId >GROUP_LOG_NUMS || GroupId < 1 || GroupId>m_iMaxGroupNum)
        GroupId = m_iCurUseGroupId;    
    if(!m_poLogGroupIndex->get(GroupId,&i))
    {
        THROW(MBC_Log_Group+4);
    }
    ThreeLogGoupData * p  = m_poLogGroup + i;
    if(!p)
    {
        THROW(MBC_Log_Group+4);
    }
    if(p->m_iGoupId == GroupId)
        return p->m_iSeq;
    return 0;
}

/*
 *	�� �� �� : GetGroupInfoUsed
 *	�������� : ��ȡָ����־���ڴ��ʹ����
 *	ʱ    �� : 2010��9��9��
 *	�� �� ֵ : int
 *	����˵�� : GroupId-��־���ʶ
*/
int ThreeLogGroup::GetGroupInfoUsed(int GroupId)
{
    if(GroupId<=0 || GroupId>GROUP_LOG_NUMS || GroupId > m_iMaxGroupNum)
        return 0;
    else
        return m_poInfoDataAll[GroupId]->getCount();
}

int ThreeLogGroup::GetGroupInfoTotal(int GroupId)
{
    if(GroupId<=0 || GroupId>GROUP_LOG_NUMS || GroupId > m_iMaxGroupNum)
        return 1;
    else
        return m_poInfoDataAll[GroupId]->getTotal();

}

/*
 *	�� �� �� : GetGroupMaxFileSize
 *	�������� : ��ȡ��־���ļ���С����
 *	ʱ    �� : 2010��7��23��
 *	�� �� ֵ : long
 *	����˵�� : ���ص�λΪM
*/
long ThreeLogGroup::GetGroupMaxFileSize()
{
    return m_lMaxLogFileSize/(1024*1024);    
}

/*
 *	�� �� �� : GetGroupMaxSize
 *	�������� : ��ȡ��־���С����
 *	ʱ    �� : 2010��7��23��
 *	�� �� ֵ : long
 *	����˵�� : ���ص�λΪM
*/
long ThreeLogGroup::GetGroupMaxSize()
{
    return m_lMaxLogDirSize/(1024*1024);
}

/*
 *	�� �� �� : GetGroupBakPath
 *	�������� : ��ȡ��־��鵵��ı���Ŀ¼
 *	ʱ    �� : 2010��7��23��
 *	�� �� ֵ : char
 *	����˵�� : 
*/
char* ThreeLogGroup::GetGroupBakPath()
{
    return m_sBakLogGroupPath;
}

/*
 *	�� �� �� : GetCurUseFileName
 *	�������� : ��ȡ��ǰ����д���ļ���
 *	ʱ    �� : 2010��7��28��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::GetCurUseFileName(int GroupId,char *pFileName,char *pFilePath)
{
    unsigned int i;

    if(GroupId >GROUP_LOG_NUMS || GroupId < 1 || GroupId>m_iMaxGroupNum)
        GroupId = m_iCurUseGroupId;   
    if(!m_poLogGroupIndex->get(GroupId,&i))
    {
        THROW(MBC_Log_Group+4);
    }
    ThreeLogGoupData * p  = m_poLogGroup + i;
    if(!p)
    {
        THROW(MBC_Log_Group+4);
    }
    for(int j=0;j!=GROUP_FILE_CNT;++j)
    {
        if(strlen(p->m_File[j].m_sFileName)==0)
            continue;
        strcpy(pFileName,p->m_File[j].m_sFileName);
        strcpy(pFilePath,p->m_File[j].m_sGroupPath);
        return true;
    }
    return false;
}

/*
 *	�� �� �� : GetCurUsedInfoData
 *	�������� : ��ȡ��ǰ�����õĹ����ڴ�������
 *	ʱ    �� : 2010��9��18��
 *	�� �� ֵ : void
 *	����˵�� : GroupId-��ǰ��־���ʶ
*/
bool ThreeLogGroup::GetCurUsedInfoData(int GroupId)
{
    if(GroupId<=0 || GroupId>GROUP_LOG_NUMS || GroupId>m_iMaxGroupNum)
        m_poInfoData=0;
    else
        m_poInfoData = m_poInfoDataAll[GroupId];

    return true;
}

/*
 *	�� �� �� : GetCurUsedInfoIndex
 *	�������� : ��ȡ��ǰ�����õĹ����ڴ�������
 *	ʱ    �� : 2010��9��18��
 *	�� �� ֵ : void
 *	����˵�� : GroupId-��ǰ��־���ʶ
*/
bool ThreeLogGroup::GetCurUsedInfoIndex(int GroupId)
{
    if(GroupId<=0 || GroupId>GROUP_LOG_NUMS || GroupId>m_iMaxGroupNum)
        m_poInfoIndex = 0;
    else
        m_poInfoIndex = m_poInfoIndexAll[GroupId];

    return true;    
}

/*
 *	�� �� �� : GetCurUsedInfoLock
 *	�������� : ��ȡ��ǰ�����õĹ����ڴ���������
 *	ʱ    �� : 2010��9��18��
 *	�� �� ֵ : void
 *	����˵�� : GroupId-��ǰ��־���ʶ
*/
bool ThreeLogGroup::GetCurUsedInfoLock(int GroupId)
{
    return true;    
}

/*
 *	�� �� �� : GetGroupFilePath
 *	�������� : ��ȡ��ǰ��־���ļ�·������
 *	ʱ    �� : 2010��9��8��
 *	�� �� ֵ : 
 *	����˵�� : GroupId-��־���ʶ
*/
bool ThreeLogGroup::GetGroupFilePath(int GroupId)
{
    unsigned int i;
    int filecnt=0;
    map<string, string>::iterator mIter;
    
    for(int num=1;num<=m_iMaxGroupNum;++num)
    {
        if(!m_poLogGroupIndex->get(num, &i))
        {
            THROW(MBC_Log_Group+4);
        }
        ThreeLogGoupData * p = m_poLogGroup + i;
        if(!p)
        {
            THROW(MBC_Log_Group+4);
        }
        if(p->m_iGoupId!=GroupId)
            continue;
        for(int j=0;j!=GROUP_FILE_CNT;++j)
        {
            if(strlen(p->m_File[j].m_sFileName)==0)
                continue;
            filecnt++;
            char temp[10]={0};
            sprintf(temp,"%d_%d",num,filecnt);
            string sFileId(temp);
            mIter = m_mFilePathMap.find(sFileId);
            if(mIter == m_mFilePathMap.end())
            {//û���ҵ�
                continue;
            }
            string sPath (m_mFilePathMap.find(sFileId)->second);
            m_poDataLock->P();
            strcpy(p->m_File[j].m_sGroupPath,sPath.c_str());
            m_poDataLock->V();
        }
    }
    return true;
}


/*
 *	�� �� �� : GetProNameByModule
 *	�������� : ���ݽ�������ȡģ���
 *	ʱ    �� : 2010��9��23��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::GetProNameByModule(int iParam, char * sReturn)
{
    char sSql[1024]={0};
    int iModId=0;

    memset(sReturn,0,sizeof(sReturn));
    DEFINE_QUERY(qry);
    try
    {
    sprintf(sSql,"select a.exec_name from wf_application a where "
        "a.module_id = %d",iParam);
    qry.setSQL(sSql);
    qry.open ();
    if(qry.next())
        strcpy(sReturn,qry.field(0).asString());
    qry.close();

    if(strlen(sReturn)==0)
        return false;
    }
    catch(TOCIException & e)
    {
        Log::log(0,"���ݿ������Ϣ���£�\nSQL->%s\nERROR->%s",e.getErrSrc(),e.getErrMsg());
        return false;
    }
    return true;
}

/*
 *	�� �� ��:   GetField
 *	��������:   ȡ������ָ���ֶ�����
 *	ʱ    ��:   2011��6��28��
 *	�� �� ֵ:   bool
 *	����˵��:   iSeq-�ֶ���ţ�sBuf-��������sParam���ز�ѯ���ֶ�
*/
bool ThreeLogGroup::GetField(int iSeq,char *sBuf,char *sParam)
{
    if(!sBuf)
        return false;
    char *p = strchr(sBuf,'|');
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
 *	�� �� ��:   GetInfoFromMem
 *	��������:   ����־����ȡ����
 *	ʱ    ��:   2011��3��15��
 *	�� �� ֵ:   bool
 *	����˵��:  iLogTypeId-�����ʶ��iLogClassId-����ʶ��iLogDisFlag-չ�ַ��࣬
 *	           sParam-����������v_list-���ش洢����
*/
bool ThreeLogGroup::GetInfoFromMem(vector<string> &v_list,int iLogTypeId,
	           int iLogDisFlag,int iLogClassId,char *sParam)
{
    unsigned int m=0;
    int cnt=0;
    char sBuf[1000];

    int iCurGroupId = GetUseGroup();
    if(iCurGroupId<=0)
    {
        printf("��ȡ��ǰ����־���ʶ��ʧ�ܣ�����\n");
        return false;
    }
    bindGroupData(iCurGroupId);
    GetCurUsedInfoData(iCurGroupId);
    GetCurUsedInfoIndex(iCurGroupId);
    GetCurUsedInfoLock(iCurGroupId);

    LogInfoData * pInfo = m_poInfoGroup;
    if(!pInfo)
    {
        printf("��λ��ǰ��־��������ʧ��,����\n");
        return false;
    }
    if(!m_poInfoIndex)
    {
        printf("��λ��ǰ��־��������ʧ��,����\n");
        return false;        
    }
    unsigned int lPrm=0;
    
    if(iLogDisFlag==GROUPCLASS)
    {//ȫ��ɨ��
        for(int i=1;i<=m_lInfoDataSize;i++)
        {
            if(strlen(pInfo[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(CheckParam(iLogTypeId,iLogDisFlag,iLogClassId,sParam,&pInfo[i]))
            {
                memset(sBuf,0,sizeof(sBuf));
                sprintf(sBuf,"%s|%d|%s|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s",
                            pInfo[i].m_sLogTime,pInfo[i].m_illCode,pInfo[i].m_sAreaCode,
                            pInfo[i].m_illProId,pInfo[i].m_illAppId,
                            pInfo[i].m_illLevelId,pInfo[i].m_illClassId,pInfo[i].m_illTypeId,
                            pInfo[i].m_illEmployee,pInfo[i].m_illActionId,
                            pInfo[i].m_sActionName,pInfo[i].m_sCodeName,pInfo[i].m_sInfo);
                v_list.push_back(sBuf);
                cnt++;
            }
        }
        return true;
    }
    if(m_poInfoIndex->get(iLogTypeId,&m))
    {
        while(m)
        {
            if(lPrm==m)
                break;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(CheckParam(iLogTypeId,iLogDisFlag,iLogClassId,sParam,&pInfo[m]))
            {
                memset(sBuf,0,sizeof(sBuf));
                sprintf(sBuf,"%s|%d|%s|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s",
                            pInfo[m].m_sLogTime,pInfo[m].m_illCode,pInfo[m].m_sAreaCode,
                            pInfo[m].m_illProId,pInfo[m].m_illAppId,
                            pInfo[m].m_illLevelId,pInfo[m].m_illClassId,pInfo[m].m_illTypeId,
                            pInfo[m].m_illEmployee,pInfo[m].m_illActionId,
                            pInfo[m].m_sActionName,pInfo[m].m_sCodeName,pInfo[m].m_sInfo);
                v_list.push_back(sBuf);
                cnt++;
            }
            lPrm = m;
            m = pInfo[m].m_iNextOffset;
        }
    }
    return true;
}



/*
 *	�� �� �� : SetLogGroupPigeonhole
 *	�������� : ������־��鵵��ʶ
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : bool
 *	����˵�� : bParam-�鵵��ʶ
*/
bool ThreeLogGroup::SetLogGroupPigeonhole(bool bParam)
{
    unsigned int i;

    for(int num=1;num<=m_iMaxGroupNum;++num)
    {
        if(!m_poLogGroupIndex->get(num,&i))
        {
            THROW(MBC_Log_Group+4);
        }
        ThreeLogGoupData * p  = m_poLogGroup + i;
        if(!p)
        {
            THROW(MBC_Log_Group+4);
        }
        m_poDataLock->P();
        p->m_bPigeonhole = bParam;
        m_poDataLock->V();
    }
    return true;
}

/*
 *	�� �� �� : SetLogLevel
 *	�������� : ������־����־�ȼ�
 *	ʱ    �� : 2011��7��24��
 *	�� �� ֵ : bool
 *	����˵�� : level��־�ȼ�
*/
bool ThreeLogGroup::SetLogLevel(int level)
{
    unsigned int i;

    for(int num=1;num<=m_iMaxGroupNum;++num)
    {
        if(!m_poLogGroupIndex->get(num,&i))
        {
            THROW(MBC_Log_Group+4);
        }
        ThreeLogGoupData * p  = m_poLogGroup + i;
        if(!p)
        {
            THROW(MBC_Log_Group+4);
        }
        m_poDataLock->P();
        p->m_iLevel = level;
        m_poDataLock->V();
    }
    return true;
}

/*
 *	�� �� �� : SetFileCheckPercent
 *	�������� : �����ļ���С�ٷֱȣ��ﵽ��ֵʱ���л���־��
 *	ʱ    �� : 2011��9��20��
 *	�� �� ֵ : bool
 *	����˵�� : �ļ���С�л��ٷֱ�
*/
bool  ThreeLogGroup::SetFileCheckPercent(int iValue)
{
	if(iValue <= 0 || iValue > 100)
	{
		iValue = 90;
	}
	
    unsigned int i;

    for(int num=1;num<=m_iMaxGroupNum;++num)
    {
        if(!m_poLogGroupIndex->get(num,&i))
        {
            THROW(MBC_Log_Group+4);
        }
        ThreeLogGoupData * p  = m_poLogGroup + i;
        if(!p)
        {
            THROW(MBC_Log_Group+4);
        }
		m_poDataLock->P();
        p->m_iFileCheckPercent = iValue;
		m_poDataLock->V();
    }
    return true;
}

/*
 *	�� �� �� : SetCheckPointTouch
 *	�������� : ������־��checkpoint�ٷֱ�
 *	ʱ    �� : 2011��7��18��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::SetCheckPointTouch(int num)
{
    m_iCheckPointTouch = num;
    return true;
}

/*
 *	�� �� �� : SetGroupMaxFileSize
 *	�������� : ������־���ļ���С����
 *	ʱ    �� : 2010��7��23��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::SetGroupMaxFileSize(int size)
{
    if(size<0)
        return false;
    if(size<=FILEMIN)
    {
        Log::log(0,"��־�ļ���С���ù�С,���ܻᵼ��Ƶ���л�,����������.���鷶Χ:200~300M");
        size = 200;
    }
    if(size>=FILEMAX)
    {
        Log::log(0,"��־�ļ���С���ù���,����������.���鷶Χ:200~300M");
        size = 200;
    }
    m_lMaxLogFileSize = size*1024*1024;
	return true;
}

bool ThreeLogGroup::RefreshGroupMaxFileSize(int size)
{
	if(size <= 0 || size >= 500)
	{
		size = 200;
	}
	
	unsigned int i;
	
    for(int num=1;num<=m_iMaxGroupNum;++num)
    {
        if(!m_poLogGroupIndex->get(num,&i))
        {
            THROW(MBC_Log_Group+4);
        }
        ThreeLogGoupData * p  = m_poLogGroup + i;
        if(!p)
        {
            THROW(MBC_Log_Group+4);
        }
		m_poDataLock->P();
        p->m_lFileSizeThreshold = size*1024*1024;
		m_poDataLock->V();
    }

	return true;
}
/*
 *	�� �� �� : SetGroupMaxSize
 *	�������� : ������־���С����
 *	ʱ    �� : 2010��7��23��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::SetGroupMaxSize(int size)
{
    if(size<0)
        return false;
    m_lMaxLogDirSize = size*1024*1024;
    return true;
}

/*
 *	�� �� �� : SetGroupDefaultPath
 *	�������� : ������־���Ա�ļ���Ĭ�ϴ洢Ŀ¼
 *	ʱ    �� : 2010��8��3��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::SetGroupDefaultPath(char *path)
{
    if(strlen(path)==0)
        return false;
    strcpy(m_sDefaultLogGroupPath,path);
    CheckLogDir(m_sDefaultLogGroupPath);
    return true;
}

/*
 *	�� �� �� : SetGroupDefaultPath
 *	�������� : ������־���Ա�ļ��洢Ŀ¼
 *	ʱ    �� : 2010��8��3��
 *	�� �� ֵ : void
 *	����˵�� : 
*/
void ThreeLogGroup::SetGroupFilePath(int GroupId,int FileSeq,char *path)
{
    char temp[10]={0};

    sprintf(temp,"%d_%d",GroupId,FileSeq);
    string sFileId(temp);
    m_mFilePathMap.insert(map<string,string>::value_type(sFileId,path));
}

/*
 *	�� �� �� : SetGroupBakPath
 *	�������� : ������־��鵵��ı���Ŀ¼
 *	ʱ    �� : 2010��7��23��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::SetGroupBakPath(char *path)
{
    if(strlen(path)==0)
        return false;
    strcpy(m_sBakLogGroupPath,path);
    CheckLogDir(m_sBakLogGroupPath);
    return true;
}

/*
 *	�� �� �� : SetGroupWriteMode
 *	�������� : ������־��д���ݷ�ʽ
 *	ʱ    �� : 2010��9��7��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::SetGroupWriteMode(int mode)
{
    m_iWriteMode = mode;
    return true;
}

/*
 *	�� �� �� : FileDownInfo
 *	�������� : ����ڴ���Ϣ���ļ�
 *	ʱ    �� : 2010��9��16��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::FileDownInfo(int GroupId,bool bPrintFlag)
{
    DIR *dirp=NULL;
    char tempPath[500],sInfo[1024];
    int fd;
    long lCnt=0,MixCnt=0;
    unsigned int i;
    ssize_t  sRet;

    if(m_iWriteMode!=WRITESHARE)
        return true;
/*        
    if(m_bAttached==false)
        THROW(MBC_Log_Group+4);
*/
    if(GroupId > GROUP_LOG_NUMS || GroupId <1)
        THROW(MBC_Log_Group+11);
    if(!m_poLogGroupIndex->get(GroupId, &i))
        THROW(MBC_Log_Group+4);
    ThreeLogGoupData * p = m_poLogGroup + i;
    if(!p)
        THROW(MBC_Log_Group+4);
	
	m_poLogGroupMgrLock->P();
		
	//�����־��������ļ��Ƿ�ȫ������
	bool bLogGroupExist = CheckLogFile(GroupId);
	
		int iCnt = m_poInfoDataAll[GroupId]->getCount();
		
    if(iCnt<=p->m_iWritSeq && bLogGroupExist)
    {
    	m_poLogGroupMgrLock->V();
        return true;
    }

	LogInfoData *pInfo = 0;
    pInfo = (LogInfoData *)(*(m_poInfoDataAll[GroupId]));
    if(!pInfo) {
        return false;
		m_poLogGroupMgrLock->V();
    }

	//add by liyr    
	m_poDataLock->P();
	long iMemSeq = p->m_iMemSeq;
	m_poDataLock->V();
	bool bFlag = true;
    for(int j=0;j!=GROUP_FILE_CNT;j++)
    {
        if(strlen(p->m_File[j].m_sFileName)==0)
            continue;
        dirp = opendir(p->m_File[j].m_sGroupPath);
        if(dirp==NULL)
        {
            mkdir(p->m_File[j].m_sGroupPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
        }
        else
            closedir(dirp);
        memset(tempPath,0,sizeof(tempPath));
        sprintf(tempPath,"%s%s",p->m_File[j].m_sGroupPath,p->m_File[j].m_sFileName);



		bool bFileExist = true;
		//�����־�ļ������ڣ���Ҫ�������ڴ�����д����־�ļ�
		if(access(tempPath,F_OK) == -1) 
		{
			bFileExist = false;
		}
		
		if((fd=open (tempPath, O_RDWR|O_CREAT|O_APPEND, 0770)) < 0)
        {        
            Log::log(0,"����־���Ա�ļ�ʧ��");
            sprintf(tempPath,"%s.temp",tempPath);
            if((fd=open (tempPath, O_RDWR|O_CREAT|O_APPEND, 0770)) < 0)
            {
                Log::log(0,"д��־���Ա�ļ�ʧ��");
				m_poLogGroupMgrLock->V();
                return false;
            }
        }
        lCnt=0;
        MixCnt=0;
        //for(int i=1;i<=m_lInfoDataSize;i++)


		int i;
		if(!bFileExist) {
			i = 1;
		} else {
			i = p->m_iWritSeq+1;
		}
		for(;i<=iMemSeq;i++)
        //for(int i=p->m_iWritSeq;i<=p->m_iMemSeq;i++)
        //for(int i=p->m_iWritSeq;i<=iCnt;i++)
        {
            if(strlen(pInfo[i].m_sInfo)==0)
                continue;
        //    lCnt++;
        //    if(lCnt<=p->m_iWritSeq)
        //        continue;
           // strncpy(sInfo,pInfo[i].m_sInfo,LOG_DATA_SIZE-1);
           /*snprintf(sInfo,LOG_DATA_SIZE-1,
                   "[%d][%s][%d][%d][%s]",pInfo[i].m_illCode,pInfo[i].m_sLogTime,
                   						pInfo[i].m_illProId,pInfo[i].m_illAppId,pInfo[i].m_sInfo);
                   						*/
		
            combineLog(pInfo+i, sInfo,2);
            if(sInfo[strlen(sInfo)-1]!='\n')
                sprintf(sInfo,"%s\n",sInfo);
            sRet = write (fd, sInfo, strlen (sInfo));
            if(sRet<0)
            {
                sprintf(tempPath,"%s.temp",tempPath);
                if((fd=open (tempPath, O_RDWR|O_CREAT|O_APPEND, 0770)) < 0)
                {
                    Log::log(0,"д��־���Ա�ļ�ʧ��");
					m_poLogGroupMgrLock->V();
                    return false;
                }
                sRet = write (fd, sInfo, strlen (sInfo));
                if(sRet<0)
                {
                    close (fd);
					m_poLogGroupMgrLock->V();
                    return false;
                }
            }


			//���һ�Σ�ֻ��Ҫ����һ�δ�С��Ϣ
			if(bFlag) {

				//��������ļ�ɾ�������
				if(i <= p->m_iWritSeq) 
				{
					continue;
				}
				
           		p->m_lCurFileSize += strlen(sInfo)*sizeof(char );
           		//p->m_lCurDirSize += strlen(sInfo) * sizeof(char ) * p->m_iSeq;
           		p->m_lCurDirSize += strlen(sInfo) * sizeof(char );
			}
            //p->m_lCurFileSize+=strlen(sInfo)*sizeof(char );
        }
        close (fd);
        //MixCnt = lCnt-p->m_iWritSeq;
        //MixCnt=p->m_iMemSeq-p->m_iWritSeq;
        MixCnt=iMemSeq-p->m_iWritSeq;
		bFlag = false;
        if(MixCnt>0)
        {
            m_poDataLock->P();
            p->m_File[j].m_lFileSize = p->m_lCurFileSize;
            m_poDataLock->V();
        }
    }
    if(MixCnt>0)
    {
        m_poDataLock->P();
        p->m_iWritSeq+=MixCnt;
        m_poDataLock->V();
        //Log::log(0,"��־����ֽ���д�ļ�[GroupId:%d]: %ld ��\n",GroupId,MixCnt);
		Date dt;
		if( bPrintFlag)
		{
			printf("[%s]: ��־�����д���ļ�[GroupId:%d]: %ld ��\n",dt.toString("yyyy-mm-dd hh:mi:ss"),GroupId,MixCnt);
		}
	}

	m_poLogGroupMgrLock->V();
	
    return true;
}


/*
 *	�� �� �� : CallBackInfo
 *	�������� : ���չ����ڴ����ݺ�����
 *	ʱ    �� : 2010��9��16��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::CallBackInfo(int GroupId)
{
    unsigned int iOffset=0;
    unsigned int i;
    int iMaxCn=0;
  
    if(m_iWriteMode!=WRITESHARE)
        return true;
/*
    if(m_bInfoAttached==false)
        return false;
*/
    if(GroupId > GROUP_LOG_NUMS || GroupId <1)
        THROW(MBC_Log_Group+11);
    if(!m_poLogGroupIndex->get(GroupId, &i))
        THROW(MBC_Log_Group+4);
    ThreeLogGoupData * p = m_poLogGroup + i;  
    if(!p)  return false ;          
    	
    LogInfoData *pInfo = 0;
    pInfo = (LogInfoData *)(*(m_poInfoDataAll[GroupId]));

    if(!pInfo)
        return false;
        
    //ThreeLogGoupData * p = m_poLogGroup + i ;
      
    //������
    m_poInfoDataLock->P();
    //long cnt=m_lInfoDataSize;
    long cnt = m_poInfoDataAll[GroupId]->getTotal();
   // int cnt=m_poInfoDataAll[GroupId]->getCount() ;
   // for(int i=1;i<=m_lInfoDataSize;i++)
   // for(int i=1;i<=p->m_iMemSeq;i++)
    for(long i=1;i<=cnt;i++)
    {
        if(strlen(pInfo[i].m_sInfo)==0)
            continue;
        m_poInfoDataAll[GroupId]->revoke(i);
    }
	m_poInfoDataAll[GroupId]->reset();
	
    m_poInfoDataLock->V();
    //������
    m_poIndexLock->P();
    SHMIntHashIndex_A::Iteration iterd = m_poInfoIndexAll[GroupId]->getIteration();
    long iIndexkey=0;
    unsigned int iIndexOffset=0;
    while(iterd.next(iIndexkey,iIndexOffset))
    {
        if(iMaxCn>=m_lInfoDataSize)
            break;            
        m_poInfoIndexAll[GroupId]->revokeIdx(iIndexkey,iIndexOffset);
        iMaxCn++;
    }
    m_poIndexLock->V();
    m_poDataLock->P();
    p->m_iWritSeq=0;
    p->m_iMemSeq=0;
    m_poDataLock->V();
    return true;
}

/*
 *	�� �� �� : CheckInfoCallBack
 *	�������� : ��鹲���ڴ��Ƿ���Ҫ����
 *	ʱ    �� : 2010��9��16��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::CheckInfoCallBack()
{
    unsigned int i;
    int GroupId=0;

    if(m_iWriteMode!=WRITESHARE)
        return true;
/*
    if(m_bAttached==false || m_bInfoAttached==false)
        THROW(MBC_Log_Group+4);
*/
    GroupId = GetUseGroup();
    double iPercent = GetGroupInfoPercent(GroupId)*100;
    if(iPercent>=CALLBACKPOINT)
    {
        //����ļ�
        if(!FileDownInfo(GroupId))
        {
            Log::log(0,"��ֽ�������ļ�ʧ��");
            return false;
        }
        //�����Ҫ����
        if(!CallBackInfo(GroupId))
        {
            Log::log(0,"��ֽ��̻��չ����ڴ�ʧ��");
            return false;
        } 
    }
    return true;
}

/*
 *	�� �� �� : CommitInfoFile
 *	�������� : �ڴ����������ļ�
 *	ʱ    �� : 2010��9��6��
 *	�� �� ֵ : bool
 *	����˵�� :
*/
bool ThreeLogGroup::CommitInfoFile()
{
    unsigned int i;
    ThreeLogGoupData * p = m_poLogGroup;
    //�ȼ���Ƿ����ӹ����ڴ�
/*    
    if(m_bAttached==false)
    {
        Log::log(0,"������־����Ϣ�������ڴ�ʧ�ܣ���鿴�Ƿ񴴽�");
        THROW(MBC_Log_Group+4);
    }
    if(m_bInfoAttached==false)
    {
        Log::log(0,"������־�������������ڴ�ʧ�ܣ���鿴�Ƿ񴴽�");
        THROW(MBC_Log_Group+4);        
    }
*/
    //�������־���ļ�״̬
    //CheckLogFile();

    //�������̬����־��
    for(int num=1;num<=m_iMaxGroupNum;++num)
    {
        if(!m_poLogGroupIndex->get(num,&i))
            return false;
       
        if(p[i].m_iState != INACTIVE)
        {
          if(!FileDownInfo(num))
          {
            Log::log(0,"��ֽ�������ļ�ʧ��");
            return false;
          }
        }

        m_poLogGroupMgrLock->P();

        if(p[i].m_iState!=ACTIVE)
        {
            m_poLogGroupMgrLock->V();
            continue;  
        }
        //�����Ҫ����
        if(!CallBackInfo(num))
        {
            Log::log(0,"��ֽ��̻��չ����ڴ�ʧ��");
            m_poLogGroupMgrLock->V();
            return false;
        }
        //�������˸�����״̬Ϊ����̬     
        m_poDataLock->P();
        p[num].m_iState = INACTIVE;
        if(p[num].m_bPigeonholeState!=NEEDDOWN)
            p[num].m_bPigeonholeState = FILEDOWN;
        p[num].m_iWritSeq = 0;
        m_poDataLock->V();
        //���Ƿ���Ҫ�鵵
        if(p[num].m_bPigeonhole==true && p[num].m_bPigeonholeState==NEEDDOWN)
        {//��Ҫ�鵵�����������ֶ��鵵����
        	refreshArchivePath();
            bakGroupFile(p[num].m_iGoupId,m_sBakLogGroupPath);
            char tempPath[500];
            for(int fnum =0;fnum!=GROUP_FILE_CNT;++fnum)
            {
                if(strlen( p[num].m_File[fnum].m_sFileName)==0)
                    continue;
                memset(tempPath,0,sizeof(tempPath));
                sprintf(tempPath,"%s%s",p[num].m_File[fnum].m_sGroupPath,p[num].m_File[fnum].m_sFileName);
                if(ClearGroupFile( p[num].m_iGoupId,tempPath)==false)
                {
                    m_poLogGroupMgrLock->V();
                    return false;
                }
            }
            if(AddLogGroupFile(p[num].m_iGoupId,NULL)==false)
            {
                m_poLogGroupMgrLock->V();
                return false;
            }
            m_poDataLock->P();
            p[num].m_iState = INACTIVE;
            p[num].m_bPigeonholeState = ALLDOWN;
            p[num].m_lCurDirSize = 0;
            p[num].m_lCurFileSize = 0;
            p[num].m_lPredictFileSize = 0;
            m_poDataLock->V();
        }
		
        m_poLogGroupMgrLock->V();
    }
    return true;
}

/*
 *	�� �� �� : Check75CheckPoint
 *	�������� : �߼���־д��75%��ʱ�򴥷�checkpoint
 *	ʱ    �� : 2011��7��18��
 *	�� �� ֵ : bool
 *	����˵�� : 
*/
bool ThreeLogGroup::Check75CheckPoint(bool &bCheck)
{
    unsigned int i;
    int GroupId=0;

    if(m_iWriteMode!=WRITESHARE)
        return true;
    GroupId = GetUseGroup();
    if(GroupId > GROUP_LOG_NUMS || GroupId <1)
        THROW(MBC_Log_Group+11);
    if(!m_poLogGroupIndex->get(GroupId, &i))
        THROW(MBC_Log_Group+4);
    ThreeLogGoupData * p = m_poLogGroup + i;
    if(!p)
        THROW(MBC_Log_Group+4);
    if(p->m_bCheckPoint == true)
        return true;
    double iPercent = GetGroupInfoPercent(GroupId)*100;
    if(iPercent<m_iCheckPointTouch)
        return true;
    //�߼���־75%����checkpoint
    bCheck=true;
    m_poDataLock->P();
    p->m_bCheckPoint = true;
    m_poDataLock->V();
	m_poLogGroupMgrLock->P();
	int iGroupId = GetUseGroup();
	char stemp[200]={0};
    if(ChangeLogGroup(false,-1)==true)
    {
        int itempId = GetUseGroup();
        sprintf(stemp,"[logg] ��־������%d�л�����%d",iGroupId,itempId);
    }
	m_poLogGroupMgrLock->V();

	log(MBC_ACTION_LOG_CONVERT,LOG_LEVEL_INFO,LOG_TYPE_SYSTEM,"%s",stemp);

	//checkpoint�ӿ�
    
    return true;
}

/*
 *	�� �� �� : log
 *	�������� : ������־��д���ݽӿ�
 *	ʱ    �� : 2010��6��26��
 *	�� �� ֵ : 
 *	����˵�� : iCodeId����־���� ��LogClass:��־���iLogTypeId:�����LogLevel:��־�ȼ�
 *             LogProid:��־��ԴPorcessId,LogAppid:��־��ԴAppId
 *             iAction�������޸Ļ������ݿ�sql����ʱ��,pExpand:��չ�ֶΣ�������/������
*/
void ThreeLogGroup::log(int iCodeId,int iLogLevel,int iLogClassId,
	                      int iLogTypeId,int iAction,const char *pExpand,const char* format,...)
{	
	if(m_bAttached == false)
	{
		initBase();
		GetUseGroup();
		bindGroupData(m_iCurUseGroupId);
		GetCurUsedInfoData(m_iCurUseGroupId);
		GetCurUsedInfoIndex(m_iCurUseGroupId);
		GetCurUsedInfoLock(m_iCurUseGroupId);
	}

	if(!m_pErrorCode->getCauseInfo(iCodeId))
	{
		return ;
		//THROW(MBC_ERRORCODE+1);
	}

    char pData[LOG_DATA_SIZE]={0};
    va_list ap;
    va_start(ap, format);
    vsnprintf (pData, LOG_DATA_SIZE-1, format, ap);
    va_end(ap);
    
    
    struct dirent *dp=NULL;
    struct stat statbuf;
    DIR *dirp=NULL;
    long tempSize=0,MaxFileSize=0,MaxGroupSize=0;
    int fd;
    char tempPath[500];
    unsigned int i = 0,j=0;
	int icLastId=0;

    if(m_iWriteMode==-1)
    {
        return;
    }

    ThreeLogGoupData* p;

    m_poLogGroupIndex->get(m_iCurUseGroupId,&i);
    p = m_poLogGroup + i;
    if(!p)
    {
        LoggLog(0,"��־���ȡ�����ڴ���Ϣʧ��\n");
        LoggLog(1,pData);
        return;
    }

    if(p->m_iLevel < iLogLevel)
    	return ;

	//�����־���Ѿ��л�����Ҫ�ض�λ������    	
    if(p->m_iState!=CURRENT)
    {
        if(GetUseGroup()<=0)
        {
            LoggLog(0,"��־���ȡ��ǰʹ����ʧ��\n");
            LoggLog(1,pData);
            return ;
        }
        m_poLogGroupIndex->get(m_iCurUseGroupId,&i);
        p = m_poLogGroup + i;
        if(!p)
        {
            LoggLog(0,"��־���ȡ�����ڴ���Ϣʧ��\n");
            LoggLog(1,pData);
            return;
        }
        bindGroupData(m_iCurUseGroupId);
        GetCurUsedInfoData(m_iCurUseGroupId);
        GetCurUsedInfoIndex(m_iCurUseGroupId);
        GetCurUsedInfoLock(m_iCurUseGroupId);
    }

    if(p->m_lCurDirSize > m_lMaxLogDirSize || p->m_lCurFileSize > p->m_lFileSizeThreshold * p->m_iFileCheckPercent/100.0
       || p->m_iMemSeq >= m_lInfoDataSize-1 || p->m_lPredictFileSize > p->m_lFileSizeThreshold * p->m_iFileCheckPercent/100.0) 
    {
    	//������־��������Դ�����л���־��
	    //��ֹ�����ͬʱ�л��������������л�ǰ��Ҫ�ж�һ�£����������Ͳ�Ҫ�л���
	    //����������ж���Ϊ�˼��پ�̬��������log�ӿ�һ��
		bool bChange = false;
		m_poLogGroupMgrLock->P(); 
		//����֮�����¶�λ���Է�ֹ�Ѿ��н����л���
		if(GetUseGroup()<=0)
		{
            LoggLog(0,"��־���ȡ��ǰʹ����ʧ��\n");
            LoggLog(1,pData);
			m_poLogGroupMgrLock->V();
            return ;		
		}
		
        m_poLogGroupIndex->get(m_iCurUseGroupId,&i);
        p = m_poLogGroup + i;
        if(!p)
        {
            LoggLog(0,"��־���ȡ�����ڴ���Ϣʧ��\n");
            LoggLog(1,pData);
			m_poLogGroupMgrLock->V();
            return;
        }
        bindGroupData(m_iCurUseGroupId);
        GetCurUsedInfoData(m_iCurUseGroupId);
        GetCurUsedInfoIndex(m_iCurUseGroupId);
        GetCurUsedInfoLock(m_iCurUseGroupId);

		//������Ǻ���ǰһ�������л�
		if(p->m_lCurDirSize > m_lMaxLogDirSize || p->m_lCurFileSize > p->m_lFileSizeThreshold * p->m_iFileCheckPercent/100.0
      		 || p->m_iMemSeq >= m_lInfoDataSize-1 || p->m_lPredictFileSize > p->m_lFileSizeThreshold * p->m_iFileCheckPercent/100.0)
		{
      		 
	        icLastId = m_iCurUseGroupId;
	        if(ChangeLogGroup(false)==false)
	        {
	            LoggLog(0,"�л���־��ʧ��\n");
	            LoggLog(1,pData);
				m_poLogGroupMgrLock->V(); 
	            return;
	        } 
			
			bChange = true;			
	        //m_poLogGroupIndex->get(p->m_iNext,&i);
	        m_poLogGroupIndex->get(m_iCurUseGroupId,&i);
    		p = m_poLogGroup + i;
			
		}
		m_poLogGroupMgrLock->V(); 

		if(bChange) 
		{
			log(MBC_THREELOG_CHANGE,LOG_LEVEL_INFO,LOG_TYPE_SYSTEM,"[logg]��־����%d�л���%d",icLastId,m_iCurUseGroupId);
		}
    }

	//д�����ڴ�ģʽ
    if(m_iWriteMode==WRITESHARE)
    {
       if(AddInfoDataInfo(iCodeId,m_iLogProid,-1,m_iLogAppid,iLogLevel,iLogClassId,
	   					  iLogTypeId,iAction,-1,"NULL",pExpand,pData,p)==false)
					   LoggLog(1,pData);

        return ;
    }


    //����Ϊֱ��д�ļ�ģʽ
    if(p->m_iSeq==0)
    {//�����ǰ��־���ļ���Ϊ0����Ĭ�ϴ�������
        AddLogGroupFile(p->m_iGoupId,m_sDefaultLogGroupPath,true);
        AddLogGroupFile(p->m_iGoupId,m_sDefaultLogGroupPath,true);
        GetGroupFilePath(p->m_iGoupId);
    }
    for(int j=0;j!=GROUP_FILE_CNT;j++)
    {
        if(strlen(p->m_File[j].m_sFileName)==0)
            continue;

        dirp = opendir(p->m_File[j].m_sGroupPath);
        if(dirp==NULL)
        {
            mkdir(p->m_File[j].m_sGroupPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
        }
        else
            closedir(dirp);
        memset(tempPath,0,sizeof(tempPath));
        sprintf(tempPath,"%s%s",p->m_File[j].m_sGroupPath,p->m_File[j].m_sFileName);
        if((fd=open (tempPath, O_RDWR|O_CREAT|O_APPEND, 0770)) < 0)
        {
            LoggLog(0,"����־���Ա�ļ�ʧ��\n");
            return;
        }
        char sInfo[LOG_DATA_SIZE]={0};
        m_dDT.getCurDate();
        snprintf(sInfo,LOG_DATA_SIZE-1,
                 "[%d][%s][%s]",iCodeId, m_dDT.toString(),pData);
        write (fd, sInfo, strlen (pData));
        close (fd);
        if(lstat(tempPath,&statbuf)<0)
        {
            LoggLog(0,"��־���Ա�ļ�״̬����\n");
            return;
        }
        else
        {
            m_poDataLock->P();
            tempSize = statbuf.st_size - p->m_File[j].m_lFileSize;
            p->m_File[j].m_lFileSize = statbuf.st_size;
            p->m_lCurFileSize = statbuf.st_size;
            p->m_lCurDirSize += tempSize;
            m_poDataLock->V();
        }
    }
    return;
}
void ThreeLogGroup::log(int iCodeId,int iLogLevel, int iLogTypeId,const char* format,...)
{	     
	  int iAction=-1;
    char pExpand[GROUP_NAME_LENGTH]={0};
    int iLogClassId =getClassID(iLogLevel);
    char pData[LOG_DATA_SIZE]={0};
    va_list ap;
    va_start(ap, format);
    vsnprintf (pData, LOG_DATA_SIZE-1, format, ap);
    va_end(ap);    
    log(iCodeId,iLogLevel,iLogClassId,iLogTypeId,iAction,pExpand,pData);
}  

//add by liyr
void ThreeLogGroup::Product_log(int iCodeId,int iProcId,int iAppId,int iLogLevel,int iLogTypeId,
				int iNetId,const char *sFlow,const char *format,...)
{
	int iThreadId=0;
	if(m_bAttached == false)
	{
		initBase();
		GetUseGroup();
		bindGroupData(m_iCurUseGroupId);
		GetCurUsedInfoData(m_iCurUseGroupId);
		GetCurUsedInfoIndex(m_iCurUseGroupId);
		GetCurUsedInfoLock(m_iCurUseGroupId);
	}
	
	if(!m_pErrorCode->getCauseInfo(iCodeId))
	{
		THROW(MBC_ERRORCODE+1);
	}
    char pData[LOG_DATA_SIZE]={0};
    va_list ap;
    va_start(ap, format);
    vsnprintf (pData, LOG_DATA_SIZE-1, format, ap);
    va_end(ap);
    
    
    struct dirent *dp=NULL;
    struct stat statbuf;
    DIR *dirp=NULL;
    long tempSize=0,MaxFileSize=0,MaxGroupSize=0;
    int fd;
    char tempPath[500];
    unsigned int i = 0,j=0;
    int icLastId=0;

    if(m_iWriteMode==-1)
    {
        return;
    }

    ThreeLogGoupData* p;

    m_poLogGroupIndex->get(m_iCurUseGroupId,&i);
    p = m_poLogGroup + i;
    if(!p)
    {
        LoggLog(0,"��־���ȡ�����ڴ���Ϣʧ��\n");
        LoggLog(1,pData);
        return;
    }
    
    if(p->m_iLevel < iLogLevel)
    	return ;

	//�����־���Ѿ��л�����Ҫ�ض�λ������    	
    if(p->m_iState!=CURRENT)
    {
        if(GetUseGroup()<=0)
        {
            LoggLog(0,"��־���ȡ��ǰʹ����ʧ��\n");
            LoggLog(1,pData);
            return ;
        }
        m_poLogGroupIndex->get(m_iCurUseGroupId,&i);
        p = m_poLogGroup + i;
        if(!p)
        {
            LoggLog(0,"��־���ȡ�����ڴ���Ϣʧ��\n");
            LoggLog(1,pData);
            return;
        }
        bindGroupData(m_iCurUseGroupId);
        GetCurUsedInfoData(m_iCurUseGroupId);
        GetCurUsedInfoIndex(m_iCurUseGroupId);
        GetCurUsedInfoLock(m_iCurUseGroupId);
    }

    if(p->m_lCurDirSize > m_lMaxLogDirSize || p->m_lCurFileSize > p->m_lFileSizeThreshold * p->m_iFileCheckPercent/100.0
       || p->m_iMemSeq >= m_lInfoDataSize-1 || p->m_lPredictFileSize > p->m_lFileSizeThreshold * p->m_iFileCheckPercent/100.0) 
    {
    	//������־��������Դ�����л���־��
	    //��ֹ�����ͬʱ�л��������������л�ǰ��Ҫ�ж�һ�£����������Ͳ�Ҫ�л���
	    //����������ж���Ϊ�˼��پ�̬��������log�ӿ�һ��
		bool bChange = false;
		m_poLogGroupMgrLock->P(); 
		//����֮�����¶�λ���Է�ֹ�Ѿ��н����л���
		if(GetUseGroup()<=0)
		{
            LoggLog(0,"��־���ȡ��ǰʹ����ʧ��\n");
            LoggLog(1,pData);
			m_poLogGroupMgrLock->V();
            return ;		
		}
		
        m_poLogGroupIndex->get(m_iCurUseGroupId,&i);
        p = m_poLogGroup + i;
        if(!p)
        {
            LoggLog(0,"��־���ȡ�����ڴ���Ϣʧ��\n");
            LoggLog(1,pData);
			m_poLogGroupMgrLock->V();
            return;
        }
        bindGroupData(m_iCurUseGroupId);
        GetCurUsedInfoData(m_iCurUseGroupId);
        GetCurUsedInfoIndex(m_iCurUseGroupId);
        GetCurUsedInfoLock(m_iCurUseGroupId);

		//������Ǻ���ǰһ�������л�
		if(p->m_lCurDirSize > m_lMaxLogDirSize || p->m_lCurFileSize > p->m_lFileSizeThreshold * p->m_iFileCheckPercent/100.0
      		 || p->m_iMemSeq >= m_lInfoDataSize-1 || p->m_lPredictFileSize > p->m_lFileSizeThreshold * p->m_iFileCheckPercent/100.0)
		{
      		 
	        icLastId = m_iCurUseGroupId;
	        if(ChangeLogGroup(false)==false)
	        {
	            LoggLog(0,"�л���־��ʧ��\n");
	            LoggLog(1,pData);
				m_poLogGroupMgrLock->V(); 
	            return;
	        }  

			bChange = true;
	        //m_poLogGroupIndex->get(p->m_iNext,&i);
	        m_poLogGroupIndex->get(m_iCurUseGroupId,&i);
    		p = m_poLogGroup + i;
		}
		
		m_poLogGroupMgrLock->V(); 

		if(bChange)
		{
			log(MBC_THREELOG_CHANGE,LOG_LEVEL_INFO,LOG_TYPE_SYSTEM,"[logg]��־����%d�л���%d",icLastId,m_iCurUseGroupId);
		}
    }
    
    int iLogClassId =getClassID(iLogLevel);
    int iAction=-1;
    char pExpand[GROUP_NAME_LENGTH]={0};

	//д�����ڴ�ģʽ
	if(m_iWriteMode==WRITESHARE)
    {
	   if(AddInfoDataInfo(iCodeId,iProcId,iThreadId,iAppId,iLogLevel,iLogClassId,
	   						iLogTypeId,iAction,iNetId,sFlow,pExpand,pData,p)==false)
					   LoggLog(1,pData);

        return ;
    }


    //����Ϊֱ��д�ļ�ģʽ
    if(p->m_iSeq==0)
    {//�����ǰ��־���ļ���Ϊ0����Ĭ�ϴ�������
        AddLogGroupFile(p->m_iGoupId,m_sDefaultLogGroupPath,true);
        AddLogGroupFile(p->m_iGoupId,m_sDefaultLogGroupPath,true);
        GetGroupFilePath(p->m_iGoupId);
    }
    for(int j=0;j!=GROUP_FILE_CNT;j++)
    {
        if(strlen(p->m_File[j].m_sFileName)==0)
            continue;

        dirp = opendir(p->m_File[j].m_sGroupPath);
        if(dirp==NULL)
        {
            mkdir(p->m_File[j].m_sGroupPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
        }
        else
            closedir(dirp);
        memset(tempPath,0,sizeof(tempPath));
        sprintf(tempPath,"%s%s",p->m_File[j].m_sGroupPath,p->m_File[j].m_sFileName);
        if((fd=open (tempPath, O_RDWR|O_CREAT|O_APPEND, 0770)) < 0)
        {
            LoggLog(0,"����־���Ա�ļ�ʧ��\n");
            return;
        }
        write (fd, pData, strlen (pData));
        close (fd);
        if(lstat(tempPath,&statbuf)<0)
        {
            LoggLog(0,"��־���Ա�ļ�״̬����\n");
            return;
        }
        else
        {
            m_poDataLock->P();
            tempSize = statbuf.st_size - p->m_File[j].m_lFileSize;
            p->m_File[j].m_lFileSize = statbuf.st_size;
            p->m_lCurFileSize = statbuf.st_size;
            p->m_lCurDirSize += tempSize;
            m_poDataLock->V();
        }
    }
    return;

}


/*
 *	�� �� ��:   GetInfoFromMem
 *	��������:   ����־����ȡ����
 *	ʱ    ��:   2011��7��7��
 *	�� �� ֵ:   bool
 *	����˵��:  iLogFlag-չ�ַ��࣬iLogCond ����
 *	           sParam-����������v_list-���ش洢����
*/

bool ThreeLogGroup::GetInfoFromMem(vector<string> &v_list,int iLogFlag,int iLogCond, char *sParam,char *sBeginTime, char *sEndTime)
{
    unsigned int m=0;
    int cnt=0;
    char sBuf[1024];

	if(m_bAttached == false)
	{
		initBase();
	}

    int iCurGroupId = GetUseGroup();

    if(iCurGroupId<=0)
    {
        printf("��ȡ��ǰ����־���ʶ��ʧ�ܣ�����\n");
        return false;
    }
    bindGroupData(iCurGroupId);
    GetCurUsedInfoData(iCurGroupId);
    GetCurUsedInfoIndex(iCurGroupId);
    GetCurUsedInfoLock(iCurGroupId);
    LogInfoData * p = m_poInfoGroup;
    if(!p)
    {
        printf("��λ��ǰ��־��������ʧ��,����\n");
        return false;
    }
    if(!m_poInfoIndex)
    {
        printf("��λ��ǰ��־��������ʧ��,����\n");
        return false;        
    }
   ThreeLogGoupData * pGroup = m_poLogGroup + iCurGroupId;  
   //int iCount=pGroup->m_iMemSeq;
   //int iCount=m_poInfoDataAll[iCurGroupId]->getCount() ; 
   //int iCount=m_lInfoDataSize;
   int iCount = m_poInfoDataAll[iCurGroupId]->getTotal(); 
   unsigned int lPrm=0;
    
    if(iLogFlag==GROUPPROCESS)
    {
    		int iCondition = 0 ;
    	  if(strlen(sParam) == 0) {iCondition=-1;}
    		else                  {iCondition=atoi(sParam);}
    			
        for(int i=iCount;i>=1;i--)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illClassId!=iLogCond)
            	  continue ;
        	  if( ( p[i].m_illProId > 0 && iCondition==-1 ) ||
        	  	  ( p[i].m_illProId ==iCondition && iCondition>=0 ) )
             {
             	if(sBeginTime) {
             		if(sBeginTime[0] && strcmp(p[i].m_sLogTime,sBeginTime) < 0) continue;
             	}
				if(sEndTime) {
					if(sEndTime[0] && strcmp(p[i].m_sLogTime,sEndTime) > 0) continue;
				}
				
                memset(sBuf,0,sizeof(sBuf));
                /*sprintf(sBuf,"%s|%d|%s|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s",
                            p[i].m_sLogTime,p[i].m_illCode,p[i].m_sCodeName,
                            p[i].m_illProId,p[i].m_illAppId,
                            p[i].m_illLevelId,p[i].m_illClassId,p[i].m_illTypeId,
                            p[i].m_illEmployee,p[i].m_illActionId,
                            p[i].m_sActionName,p[i].m_sAreaCode,p[i].m_sInfo); */
                //����     
                combineLog(p+i,sBuf);
                v_list.push_back(sBuf);
                cnt++;
             }
        }
        return true;    		
    }

    if(iLogFlag==GROUPMODULE)
    {
    		int iCondition = 0 ;
    	  if(strlen(sParam)==0) iCondition=-1;
    		else                  iCondition=atoi(sParam);
    			
         for(int i=iCount;i>=1;i--)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illClassId!=iLogCond)
            	  continue ;
        	  if( ( p[i].m_illAppId > 0 && iCondition==-1 ) ||
        	  	  ( p[i].m_illAppId ==iCondition && iCondition>=0 ) )
             {
				 if(sBeginTime) {
					 if(sBeginTime[0] && strcmp(p[i].m_sLogTime,sBeginTime) < 0) continue;
				 }
				 if(sEndTime) {
					 if(sEndTime[0] && strcmp(p[i].m_sLogTime,sEndTime) > 0) continue;
				 }
				  
                memset(sBuf,0,sizeof(sBuf));
                /*sprintf(sBuf,"%s|%d|%s|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s",
                            p[i].m_sLogTime,p[i].m_illCode,p[i].m_sCodeName,
                            p[i].m_illProId,p[i].m_illAppId,
                            p[i].m_illLevelId,p[i].m_illClassId,p[i].m_illTypeId,
                            p[i].m_illEmployee,p[i].m_illActionId,
                            p[i].m_sActionName,p[i].m_sAreaCode,p[i].m_sInfo);
                            */
                combineLog(p+i,sBuf);
                v_list.push_back(sBuf);
                cnt++;
             }
        }
        return true;    		
    }  
    
    if(iLogFlag==GROUPCLASS)
    {    			
         for(int i=iCount;i>=1;i--)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illClassId==iLogCond)
             {
				 if(sBeginTime) {
					 if(sBeginTime[0] && strcmp(p[i].m_sLogTime,sBeginTime) < 0) continue;
				 }
				 if(sEndTime) {
					 if(sEndTime[0] && strcmp(p[i].m_sLogTime,sEndTime) > 0) continue;
				 }
				 
                memset(sBuf,0,sizeof(sBuf));
                /*sprintf(sBuf,"%s|%d|%s|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s",
                            p[i].m_sLogTime,p[i].m_illCode,p[i].m_sCodeName,
                            p[i].m_illProId,p[i].m_illAppId,
                            p[i].m_illLevelId,p[i].m_illClassId,p[i].m_illTypeId,
                            p[i].m_illEmployee,p[i].m_illActionId,
                            p[i].m_sActionName,p[i].m_sAreaCode,p[i].m_sInfo);*/
                combineLog(p+i,sBuf);
                v_list.push_back(sBuf);
                cnt++;
             }
        }
        return true;    		
    } 
    
    if(iLogFlag==GROUPINFOLV)
    {    			
         for(int i=iCount;i>=1;i--)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illLevelId==iLogCond)
             {
				 if(sBeginTime) {
					 if(sBeginTime[0] && strcmp(p[i].m_sLogTime,sBeginTime) < 0) continue;
				 }
				 if(sEndTime) {
					 if(sEndTime[0] && strcmp(p[i].m_sLogTime,sEndTime) > 0) continue;
				 }
				 
                memset(sBuf,0,sizeof(sBuf));
                /*sprintf(sBuf,"%s|%d|%s|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s",
                            p[i].m_sLogTime,p[i].m_illCode,p[i].m_sCodeName,
                            p[i].m_illProId,p[i].m_illAppId,
                            p[i].m_illLevelId,p[i].m_illClassId,p[i].m_illTypeId,
                            p[i].m_illEmployee,p[i].m_illActionId,
                            p[i].m_sActionName,p[i].m_sAreaCode,p[i].m_sInfo);
                            */
                combineLog(p+i,sBuf);
                v_list.push_back(sBuf);
                cnt++;
             }
        }
        return true;    		
    } 
    
    if(iLogFlag==GROUPTYPE)
    {    			
        for(int i=iCount;i>=1;i--)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illTypeId==iLogCond)
             {
				 if(sBeginTime) {
					 if(sBeginTime[0] && strcmp(p[i].m_sLogTime,sBeginTime) < 0) continue;
				 }
				 if(sEndTime) {
					 if(sEndTime[0] && strcmp(p[i].m_sLogTime,sEndTime) > 0) continue;
				 }
				 
                memset(sBuf,0,sizeof(sBuf));
                /*sprintf(sBuf,"%s|%d|%s|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s",
                            p[i].m_sLogTime,p[i].m_illCode,p[i].m_sCodeName,
                            p[i].m_illProId,p[i].m_illAppId,
                            p[i].m_illLevelId,p[i].m_illClassId,p[i].m_illTypeId,
                            p[i].m_illEmployee,p[i].m_illActionId,
                            p[i].m_sActionName,p[i].m_sAreaCode,p[i].m_sInfo);*/
                combineLog(p+i,sBuf);
                v_list.push_back(sBuf);
                cnt++;
             }
        }
        return true;    		
    }  
    
    if(iLogFlag==GROUPPARAM)
    {
      for(int i=iCount;i>=1;i--)
      {
        if(strlen(p[i].m_sInfo)==0)
            continue;
        if(cnt>=DISPLAYMAXCNT)
            break;
        if(p[i].m_illTypeId==LOG_TYPE_PARAM && 
        	(strlen(sParam)==0 || strcmp(sParam,p[i].m_sActionName)==0) )
         {
				 if(sBeginTime) {
					 if(sBeginTime[0] && strcmp(p[i].m_sLogTime,sBeginTime) < 0) continue;
				 }
				 if(sEndTime) {
					 if(sBeginTime[0] && strcmp(p[i].m_sLogTime,sEndTime) > 0) continue;
				 }
				 
            memset(sBuf,0,sizeof(sBuf));
            /*sprintf(sBuf,"%s|%d|%s|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s",
                        p[i].m_sLogTime,p[i].m_illCode,p[i].m_sCodeName,
                        p[i].m_illProId,p[i].m_illAppId,
                        p[i].m_illLevelId,p[i].m_illClassId,p[i].m_illTypeId,
                        p[i].m_illEmployee,p[i].m_illActionId,
                        p[i].m_sActionName,p[i].m_sAreaCode,p[i].m_sInfo);
                      */
            combineLog(p+i,sBuf);
            v_list.push_back(sBuf);
            cnt++;
         }
      }
      return true;		
    }
    
    
    if(iLogFlag==GROUPCODE)
    {    			
        for(int i=iCount;i>=1;i--)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illCode==iLogCond)
             {
 				 if(sBeginTime) {
					 if(sBeginTime[0] && strcmp(p[i].m_sLogTime,sBeginTime) < 0) continue;
				 }
				 if(sEndTime) {
					 if(sEndTime[0] && strcmp(p[i].m_sLogTime,sEndTime) > 0) continue;
				 }
				 
                memset(sBuf,0,sizeof(sBuf));
                /*sprintf(sBuf,"%s|%d|%s|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s",
                            p[i].m_sLogTime,p[i].m_illCode,p[i].m_sCodeName,
                            p[i].m_illProId,p[i].m_illAppId,
                            p[i].m_illLevelId,p[i].m_illClassId,p[i].m_illTypeId,
                            p[i].m_illEmployee,p[i].m_illActionId,
                            p[i].m_sActionName,p[i].m_sAreaCode,p[i].m_sInfo);*/
				combineLog(p+i,sBuf);
                v_list.push_back(sBuf);
                cnt++;
             }
        }
        return true;    		
    }      
}

void ThreeLogGroup::combineLog(LogInfoData *p, char *sDest,int iMode)
{
	char sLevel[8] = {0};
	if(p->m_illLevelId == LOG_LEVEL_FATAL) {
		strcpy(sLevel,"fatal");
	} else if(p->m_illLevelId == LOG_LEVEL_ERROR) {
		strcpy(sLevel,"error");
	} else if(p->m_illLevelId == LOG_LEVEL_WARNING) {
		strcpy(sLevel,"warning");
	} else if(p->m_illLevelId == LOG_LEVEL_INFO) {
		strcpy(sLevel,"info");
	} else if(p->m_illLevelId == LOG_LEVEL_DEBUG) {
		strcpy(sLevel,"debug");
	}

	//��־��ѯʹ��
	if(iMode == 1) {
    	sprintf(sDest,"%s|%s|%d|%d|%s|"
					 "%d|%d|%d|"
					 "%d|%s|"
					 "%d|%d|%s|%s",
              	  	 p->m_sLogTime,sLevel,p->m_illTypeId,p->m_illCode,p->m_sCodeName,
              	  	 p->m_illProId,p->m_iThreadId,p->m_illAppId,
               	 	 p->m_iNetId,p->m_sFlow,
                  	 p->m_illEmployee,p->m_illActionId,p->m_sActionName,p->m_sInfo);
	} 
	else if(iMode == 2) 
	{
    	sprintf(sDest,"%s|%s|%d|"
					 "%d|%d|%d|"
					 "%d|%s|%d|%s",
              	  	 p->m_sLogTime,sLevel,p->m_illTypeId,
              	  	 p->m_illProId,p->m_iThreadId,p->m_illAppId,
               	 	 p->m_iNetId,p->m_sFlow,p->m_illCode,p->m_sInfo);	
	}
}

/*
 *	�� �� ��:   GetLoggInfo
 *	��������:   ����־����ȡ����
 *	ʱ    ��:   2011��7��9��
 *	�� �� ֵ:   bool
 *	����˵��:  iLogFlag-չ�ַ��࣬iLogCond ����
 *	           sParam-����������v_list-���ش洢����
*/
/*
bool ThreeLogGroup::GetLoggInfo(vector<LogInfoData> &v_list,int iLogFlag,int iLogCond, char *sParam)
{
    unsigned int m=0;
    int cnt=0;
    char sBuf[1000];

    int iCurGroupId = GetUseGroup();

    if(iCurGroupId<=0)
    {
        printf("��ȡ��ǰ����־���ʶ��ʧ�ܣ�����\n");
        return false;
    }
    bindGroupData(iCurGroupId);
    GetCurUsedInfoData(iCurGroupId);
    GetCurUsedInfoIndex(iCurGroupId);
    GetCurUsedInfoLock(iCurGroupId);
    LogInfoData * p = m_poInfoGroup;
    if(!p)
    {
        printf("��λ��ǰ��־��������ʧ��,����\n");
        return false;
    }
    if(!m_poInfoIndex)
    {
        printf("��λ��ǰ��־��������ʧ��,����\n");
        return false;        
    }
   ThreeLogGoupData * pGroup = m_poLogGroup + iCurGroupId;  
   int iCount=pGroup->m_iMemSeq;
     
    unsigned int lPrm=0;
    
    if(iLogFlag==GROUPPROCESS)
    {    			
        for(int i=1;i<=iCount;i++)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
        	  if( ( p[i].m_illProId >=0 && iLogCond==-1 ) ||
        	  	  ( p[i].m_illProId ==iLogCond && iLogCond>=0 ) )
             {
                v_list.push_back(p[i]);
                cnt++;
             }
        }
        return true;    		
    }

    if(iLogFlag==GROUPMODULE)
    {

        for(int i=1;i<=iCount;i++)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
        	  if( ( p[i].m_illAppId >=0 && iLogCond==-1 ) ||
        	  	  ( p[i].m_illAppId ==iLogCond && iLogCond>=0 ) )
             {
                v_list.push_back(p[i]);
                cnt++;
             }
        }
        return true;    		
    }  
    
    if(iLogFlag==GROUPCLASS)
    {    			
        for(int i=1;i<=iCount;i++)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illClassId==iLogCond)
             {
                v_list.push_back(p[i]);
                cnt++;
             }
        }
        return true;    		
    } 
    
    if(iLogFlag==GROUPINFOLV)
    {    			
        for(int i=1;i<=iCount;i++)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illLevelId==iLogCond)
             {
                v_list.push_back(p[i]);
                cnt++;
             }
        }
        return true;    		
    } 
    
    if(iLogFlag==GROUPTYPE)
    {    			
        for(int i=1;i<=iCount;i++)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illTypeId==iLogCond)
             {
                v_list.push_back(p[i]);
                cnt++;
             }
        }
        return true;    		
    }  
    
    if(iLogFlag==GROUPPARAM)
    {
      for(int i=1;i<=iCount;i++)
      {
        if(strlen(p[i].m_sInfo)==0)
            continue;
        if(cnt>=DISPLAYMAXCNT)
            break;
        if(p[i].m_illTypeId==LOG_TYPE_PARAM && 
        	(strlen(sParam)==0 || strcmp(sParam,p[i].m_sActionName)==0) )
         {
             v_list.push_back(p[i]);
             cnt++;
         }
      }
        	
      return true;		

    }
    
    if(iLogFlag==GROUPCODE)
    {
      if(m_poInfoIndex->get(iLogCond,&m))
      {
        while(m)
        {
            if(lPrm==m)
                break;
            if(cnt>=DISPLAYMAXCNT)
                break;
            v_list.push_back(p[m]);
                cnt++; 
            lPrm = m;
            m = p[m].m_iNextOffset;
          }
      }    
      return true;  		
    }           
}
*/
/*
 *	�� �� ��:   GetLoggInfo
 *	��������:   ����־����ȡ����
 *	ʱ    ��:   2011��7��9��
 *	�� �� ֵ:   bool
 *	����˵��:   iLogFlag-չ�ַ��࣬iLogCond ����
 *	            char *sBTime,char *sETime-����ʱ��������
 *              v_list-���ش洢����
*/
bool ThreeLogGroup::GetLoggInfo(vector<LogInfoData> &v_list,int iLogFlag,int iLogCond, char *sBTime,char *sETime)
{
    unsigned int m=0;
    int cnt=0;
    char sBuf[1000];

	if(m_bAttached == false)
	{
		initBase();
	}

    int iCurGroupId = GetUseGroup();

    if(iCurGroupId<=0)
    {
        printf("��ȡ��ǰ����־���ʶ��ʧ�ܣ�����\n");
        return false;
    }
    bindGroupData(iCurGroupId);
    GetCurUsedInfoData(iCurGroupId);
    GetCurUsedInfoIndex(iCurGroupId);
    GetCurUsedInfoLock(iCurGroupId);
    LogInfoData * p = m_poInfoGroup;
    if(!p)
    {
        printf("��λ��ǰ��־��������ʧ��,����\n");
        return false;
    }
    if(!m_poInfoIndex)
    {
        printf("��λ��ǰ��־��������ʧ��,����\n");
        return false;        
    }
    unsigned int i;
   m_poLogGroupIndex->get(iCurGroupId,&i);
   ThreeLogGoupData * pGroup = m_poLogGroup + i;  
   //int iCount=pGroup->m_iMemSeq;
   //int iCount=m_poInfoDataAll[iCurGroupId]->getCount() ; 
   //int iCount=m_lInfoDataSize;    
   int iCount = m_poInfoDataAll[iCurGroupId]->getTotal();
   unsigned int lPrm=0;
    
    if(iLogFlag==GROUPPROCESS)
    {    			
        for(int i=iCount;i>=1;i--)
        {
           if(strlen(p[i].m_sInfo)==0)
                continue;
           if(cnt>=DISPLAYMAXCNT)
                break;
        	 if( p[i].m_illProId !=iLogCond && iLogCond>=0  )
        	  	  continue ;
        	 if(strcmp(p[i].m_sLogTime,sBTime)<0 )
        	 	    continue ;
        	  if(sETime==NULL ||  strcmp(p[i].m_sLogTime,sETime )<0 )          	  	  
             {
                v_list.push_back(p[i]);
                cnt++;
             }
        }
        return true;    		
    }

    if(iLogFlag==GROUPMODULE)
    {

        for(int i=iCount;i>=1;i--)
        {
           if(strlen(p[i].m_sInfo)==0)
                continue;
           if(cnt>=DISPLAYMAXCNT)
                break;
           if( p[i].m_illAppId !=iLogCond && iLogCond>=0 )
            	  continue ;
        	 if(strcmp(p[i].m_sLogTime,sBTime)<0 )
        	 	    continue ;
        	  if(sETime==NULL ||  strcmp(p[i].m_sLogTime,sETime )<0 )        
             {
                v_list.push_back(p[i]);
                cnt++;
             }
        }
        return true;    		
    }  
    
    if(iLogFlag==GROUPCLASS)
    {    			
        for(int i=iCount;i>=1;i--)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illClassId!=iLogCond)
            	 continue ;
        	 if(strcmp(p[i].m_sLogTime,sBTime)<0 )
        	 	    continue ;
        	  if(sETime==NULL ||  strcmp(p[i].m_sLogTime,sETime )<0  )                 	 
             {
                v_list.push_back(p[i]);
                cnt++;
             }
        }
        return true;    		
    } 
    
    if(iLogFlag==GROUPINFOLV)
    {    			
        for(int i=iCount;i>=1;i--)
        {
            if(strlen(p[i].m_sInfo)==0)
                continue;
            if(cnt>=DISPLAYMAXCNT)
                break;
            if(p[i].m_illLevelId!=iLogCond)
            	continue ;
        	 if(strcmp(p[i].m_sLogTime,sBTime)<0 )
        	 	    continue ;
        	  if(sETime==NULL ||  strcmp(p[i].m_sLogTime,sETime )<0  )                  	
             {
                v_list.push_back(p[i]);
                cnt++;
             }
        }
        return true;    		
    } 
    
    if(iLogFlag==GROUPTYPE)
    {    			
        for(int i=iCount;i>=1;i--)
        {
           if(strlen(p[i].m_sInfo)==0)
                continue;
           if(cnt>=DISPLAYMAXCNT)
                break;
           if(p[i].m_illTypeId!=iLogCond)
            	  continue ;
        	 if(strcmp(p[i].m_sLogTime,sBTime)<0 )
        	 	    continue ;
        	  if(sETime==NULL ||  strcmp(p[i].m_sLogTime,sETime )<0  )                  	  
             {
                v_list.push_back(p[i]);
                cnt++;
             }
        }
        return true;    		
    }  
    
    if(iLogFlag==GROUPPARAM)
    {
      for(int i=iCount;i>=1;i--)
      {
        if(strlen(p[i].m_sInfo)==0)
            continue;
        if(cnt>=DISPLAYMAXCNT)
            break;
        if(p[i].m_illTypeId!=LOG_TYPE_PARAM )
        	  continue;
        if(strcmp(p[i].m_sLogTime,sBTime)<0 )
        	 	    continue ;
        if(sETime==NULL ||  strcmp(p[i].m_sLogTime,sETime )<0  )        
         {
             v_list.push_back(p[i]);
             cnt++;
         }
      }
        	
      return true;		

    }
    
    if(iLogFlag==GROUPCODE)
    {
      if(m_poInfoIndex->get(iLogCond,&m))
      {
        while(m)
        {
            if(lPrm==m)
                break;
            if(cnt>=DISPLAYMAXCNT)
                break;
        	 if(strcmp(p[m].m_sLogTime,sBTime)<0 )
        	 	{
        	 		  m = p[m].m_iNextOffset;
        	 	    continue ;
        	 	 }
        	  if(sETime==NULL ||  strcmp(p[m].m_sLogTime,sETime )<0 )  
        	 {                
            v_list.push_back(p[m]);
                cnt++; 
            lPrm = m;
            m = p[m].m_iNextOffset;
           }
         m = p[m].m_iNextOffset;
        }
      }    
      return true;  		
    }           
}

bool ThreeLogGroup::GetAllLoggInfo(vector<LogInfoData> &v_list)
{
    unsigned int m=0;
    int cnt=0;
    char sBuf[1000];
    v_list.clear();

	if(m_bAttached == false)
	{
		initBase();
	}
	
    int iCurGroupId = GetUseGroup();
    if(iCurGroupId<=0)
    {
        printf("��ȡ��ǰ����־���ʶ��ʧ�ܣ�����\n");
        return false;
    }
    bindGroupData(iCurGroupId);
    GetCurUsedInfoData(iCurGroupId);
    GetCurUsedInfoIndex(iCurGroupId);
    GetCurUsedInfoLock(iCurGroupId);
    LogInfoData * p = m_poInfoGroup;
    if(!p)
    {
        printf("��λ��ǰ��־��������ʧ��,����\n");
        return false;
    }
    if(!m_poInfoIndex)
    {
        printf("��λ��ǰ��־��������ʧ��,����\n");
        return false;        
    }
   //ThreeLogGoupData * pGroup = m_poLogGroup + iCurGroupId; 
    unsigned int i;
   m_poLogGroupIndex->get(iCurGroupId,&i);
   ThreeLogGoupData * pGroup = m_poLogGroup + i;      
   //int iCount=pGroup->m_iMemSeq;	
   //int iCount=m_poInfoDataAll[iCurGroupId]->getCount() ; 
   //int iCount=m_lInfoDataSize;  
   int iCount=m_poInfoDataAll[iCurGroupId]->getTotal() ;  
    for(int i=iCount;i>=1;i--)
    {
        if(strlen(p[i].m_sInfo)==0)
            continue;
        if(cnt>=DISPLAYMAXCNT)
            break;
         v_list.push_back(p[i]);
        cnt++;

    }
    return true;   
}

int ThreeLogGroup::getClassID(int iLevelID)
{
	int iRet=1;
	switch(iLevelID) {
		case 1:
		case 2:
			iRet=1;
			break;
		case 3:
			iRet=2;
			break;	
	  case 4:		
	  case 5:
			iRet=3;
			break;	
	 default :
	 	  break ;
	 	}
	 	
	 return iRet ;	
			
}

