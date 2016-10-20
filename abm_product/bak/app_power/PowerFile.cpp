/* ����Զ�̳����ļ��ӿ�
   xianggs 2010-11-01
   ʵ��ҵ��1������������ѯ�û���Ϣ(�ļ����ƣ�STATEGRID01_xxxxxxxx)
             2������������ѯ�û���������(�ļ����ƣ�STATEGRID02_xxxxxxxx)
             3������������ѯ�û���������(�ļ����ƣ�STATEGRID03_xxxxxxxx)

*/
#include "PowerFile.h"

using namespace std;

//��Ҫ��������
int PowerFile::PowerMainProcess(int m_iOperation)
{
	vector<string> v_Phone;
	v_Phone.clear();
	ReadFile(v_Phone);
	
	char sContent[1024];char sRet[10];
	char sSumAmount[18]; char sSendAmount[18];char sRecvAmount[18];
	int  iCnt = 0;
	
	//�ṩ���ݿ�����ҵ��״̬
	if (m_iOperation == 1) {
	  FILE  * fp;
	  char sWholeFileName[256];
	  sprintf(sWholeFileName,"%sRESP_%s",m_lFileDirResp,file->getFileName());
	  if ( (fp = fopen(sWholeFileName,"r+"))== NULL){
	    if( (fp = fopen(sWholeFileName,"wb") ) == NULL){
	      Log::log(0,"open file error<%s>",sWholeFileName);
	      return -1;
	    }
	  }
	  for(vector<string>::iterator iter=v_Phone.begin();iter !=v_Phone.end();iter++) {
	    char m_sState[256];
	    char sBusiName[256];
	    char sPackName[256];
	    memset(m_sState,0,sizeof(m_sState));
	    int iRet = m_pQuery->queryState((*iter).c_str(),sBusiName,sPackName,m_sState,pRespMsg);
	    //sprintf(sRet,"%d",iRet);
	    if ( iRet == 0 )//������1 �ɹ� 0ʧ��
	       strcpy(sRet,"1");
	    else   
	       strcpy(sRet,"0");
	    memset(sContent,0,sizeof(sContent));
//�ֻ�����|�������|ҵ������|�ײ�����|����״̬
			sprintf("%s|%s|%s|%s|%s\n",(*iter).c_str(),sRet,sBusiName,sPackName,m_sState);	    
	    fprintf(fp,"%s",sContent);
	    
	    iCnt++;
	    if(iCnt%1000==0)
	      printf("��ɿ�����ҵ��״̬��ѯ %d ��.\n",iCnt);
	  }
	  fclose(fp);
	  m_pQuery->insertFileList(file->getFileName(),iCnt,m_iOperation,sWholeFileName);	
	  Log::log(0,"��ɿ�����ҵ��״̬��ѯ %d ��.\n",iCnt);
	  Log::log(0,"�ļ�<%s>�еĿ�����ҵ��״̬��ѯ���,�����ļ� %s.\n",m_sFileName,sWholeFileName);
	  /*����ѯ�ĺ���д�����ݿⱣ�棬Ԥ��*/
	}
	
	//�ṩ���ݿ����뵱��ʵʱ��������ʵʱ���� ������Ϣ���к����зֱ�ͳ��
	else if (m_iOperation == 2) {
	  FILE  * fp;
	  char sWholeFileName[256];
	  sprintf(sWholeFileName,"%sRESP_%s",m_lFileDirResp,file->getFileName());
	  if ( (fp = fopen(sWholeFileName,"r+"))== NULL){
	    if( (fp = fopen(sWholeFileName,"wb") ) == NULL){
	      Log::log(0,"open file error<%s>",sWholeFileName);
	      return -1;
	    }
	  }
	  for(vector<string>::iterator iter=v_Phone.begin();iter !=v_Phone.end();iter++) {
	    vector<GprsAmount> v_Vec;
	    int iRet = m_pQuery->queryGprs((*iter).c_str(),v_Vec,pRespMsg);
	    //sprintf(sRet,"%d",iRet);
	    if ( iRet == 0 )//������1 �ɹ� 0ʧ��
	       strcpy(sRet,"1");
	    else   
	       strcpy(sRet,"0");
	    
	    
	    //ԭЭ�鴫��ֻ��һ�����룬0928�޸�Э�飬����EndDate�ֶΣ�Ϊ���㣬�ڴ˲��
	    char m_sPhoneT[30];memset(m_sPhoneT,0,sizeof(m_sPhoneT));
	    for (int i = 0; i < strlen((*iter).c_str()) ; i++) {
	      if ((*iter)[i] == '|')  {
	      strncpy(m_sPhoneT,(*iter).c_str(),i);
	      m_sPhoneT[strlen(m_sPhoneT)] = '\0';
	      break;
	     }
	    }
	    
	    for (vector<GprsAmount>::iterator iter1=v_Vec.begin();iter1 !=v_Vec.end();iter1++) {
	      memset(sContent,0,sizeof(sContent));
//�ֻ�����|�������|����|���°���ʵʱ��������Ϣ|���°���ʵʱ����������Ϣ|����ʵʱ����������Ϣ	   
				sprintf(sContent,"%s|%s|%s|%ld|%ld%ld\n",m_sPhoneT
																								,sRet
																								,(*iter1).sStartDate
																								,(*iter1).SumAmount
																								,(*iter1).SendAmount
																								,(*iter1).RecvAmount
																								);
	      fprintf(fp,"%s",sContent);
	    }
	    
	    iCnt++;
	    if(iCnt%1000==0)
	      printf("��ɿ�������Ϣ��ѯ %d ��.\n",iCnt);
	  }
	  fclose(fp);
	  m_pQuery->insertFileList(file->getFileName(),iCnt,m_iOperation,sWholeFileName);	
	  Log::log(0,"��ɿ�������Ϣ��ѯ %d ��.\n",iCnt);
	  Log::log(0,"�ļ�<%s>�еĿ�����������Ϣ��ѯ���,�����ļ� %s.\n",m_sFileName,sWholeFileName);
	  /*����ѯ�ĺ���д�����ݿⱣ�棬Ԥ��*/
	}
	
	//�ṩ������ҵ�����룬��ѯ�ú����ύʱ�������·ݵ�����GPRS����������Ϣ
	else if(m_iOperation == 3) {
	  FILE  * fp;
	  char sWholeFileName[256];
	  sprintf(sWholeFileName,"%sRESP_%s",m_lFileDirResp,file->getFileName());
	  if ( (fp = fopen(sWholeFileName,"r+"))== NULL){
	    if( (fp = fopen(sWholeFileName,"wb") ) == NULL){
	      Log::log(0,"open file error<%s>",sWholeFileName);
	      return -1;
	    }
	  }
	  for(vector<string>::iterator iter=v_Phone.begin();iter !=v_Phone.end();iter++) {
	    long SumAmount = 0;
	    int iRet = m_pQuery->queryLastGprs((*iter).c_str(),SumAmount,pRespMsg);
	    if ( iRet == 0 )//������1 �ɹ� 0ʧ��
	       strcpy(sRet,"1");
	    else   
	       strcpy(sRet,"0");
//�ֻ�����|�������|������������Ϣ	    
	    memset(sContent,0,sizeof(sContent));
	    sprintf(sContent,"%s|%s|%ld\n",(*iter).c_str(),sRet,SumAmount);
	    fprintf(fp,"%s",sContent);
	    
	    iCnt++;
	    if(iCnt%1000==0)
	      printf("��ɿ�����ҵ����GPRS����������Ϣ��ѯ %d ��.\n",iCnt);
	  }
	  fclose(fp);
	  m_pQuery->insertFileList(file->getFileName(),iCnt,m_iOperation,sWholeFileName);	
	  Log::log(0,"��ɿ�����ҵ����GPRS����������Ϣ��ѯ %d ��.\n",iCnt);
	  Log::log(0,"�ļ�<%s>�еĿ�����ҵ����GPRS����������Ϣ��ѯ���,�����ļ� %s.\n",m_sFileName,sWholeFileName);
	  /*����ѯ�ĺ���д�����ݿⱣ�棬Ԥ��*/
	}
	
	else {
	  printf("δ֪�Ľ�����ˮ");
	  return -1;
	}
	
	return 0;
	
}

int PowerFile::CheckFile( char* m_sFileName )
{
	int iTemp;
	///������ڷ������ϲ����Ѿ��ɼ����ļ�.�����ڱ����ж��Ƿ��Ѿ��ɼ���
	if ( m_pQuery->checkFileExist( m_sFileName ) )
	{
	    return -2;
	}
	
	/*��ҵ������ҵ��״̬��ѯ�ӿ� */
	if ( strncmp( &m_sFileName[0], "STATEGRID01", 11) == 0 ){
	    return 1;
	}
	
	/*��ҵ������������Ϣ��ѯ�ӿ�*/
	else if ( strncmp( &m_sFileName[0], "STATEGRID02",11 ) == 0 )	{
	    return 2;
	}
	
	/*��ҵ����������������Ϣ��ѯ�ӿ�*/
	else if ( strncmp( &m_sFileName[0], "STATEGRID03",11 ) == 0 )	{
	    return 3;
	}

	/*�ļ�������ʶ��*/
	else {		
		return -1;
	}

	return 0;
}


int PowerFile::MainProcess()
{
//

	while (( file = oDir.nextFileRecursive() ) != NULL)
	{
	   if( file->getFileName() && ( iCheckState = CheckFile( file->getFileName() ) ))  {
	      if ( iCheckState == -2 )  {   ////�ظ��ļ�
	          Log::log(0,"�����ظ��ļ�[%s],���ļ�������Ϊ[%s%s.err]",file->getFullFileName(),m_lFileDirRecvBak,
	                                                              file->getFileName() );
	          char sTmp[256];
	          sprintf(sTmp,"%s%s.err", m_lFileDirRecvBak,file->getFileName() );
	          file->moveFile(sTmp);
	          continue;
	      } 
	      else if ( iCheckState == -1  || iCheckState == 0) {
	      	   Log::log(0,"����ʶ����ļ���<%s>",file->getFileName());
	      	   //doWaring Send SMS;
	      }  
	      else {
	        memset(m_sFileName,0,sizeof(m_sFileName));
	        strcpy(m_sFileName,m_lFileDirRecv);
	        strcat(m_sFileName,file->getFileName());
	        
	        int nRet = PowerMainProcess(iCheckState);
	        if (nRet == 0 ) {
	          char sBackFile[256];
	          strcpy(sBackFile,m_lFileDirRecvBak);
	          strcat(sBackFile,file->getFileName());
	          file->moveFile(sBackFile);
	          Log::log(0,"�ļ�<%s>��ѯ��ϣ����ݳ� %s.\n",file->getFileName(),sBackFile);
	        } else {
	          Log::log (0,"�ļ����ѯ�����ļ����� %s \n",file->getFullFileName());
	        }
	      }
	   }
	}
	
	return 0;
}

int PowerFile::ReadFile(vector<string> &v_Phone)
{
	FILE *rFile; 
	int i = 0;
	
	if  (v_Phone.size() != 0)
	  v_Phone.clear();	
	char 	sInPhone[201];
	if ((rFile = fopen(m_sFileName, "r"))== NULL) 	{
	  return -1;
	}
	
	while(!feof(rFile))
	{       
	  fgets(sInPhone,201,rFile);
	  if (feof(rFile)) break;
	  Alltrim(sInPhone);
	  if (sInPhone[strlen(sInPhone)-1] == '\n')
	  	sInPhone[strlen(sInPhone)-1] = '\0';
	  v_Phone.push_back(sInPhone);
	}
}

void PowerFile::init()
{
	if (!ParamDefineMgr::getParam("POWER_MAIN","FILE_RECV",m_lFileDirRecv,128)) {
	  printf("b_param_defineδ������ ���������ļ�����Ŀ¼");
	  exit;
	}
	if (m_lFileDirRecv[strlen(m_lFileDirRecv)-1] != '/')
	  m_lFileDirRecv[strlen(m_lFileDirRecv)] = '/';
	oDir = Directory(m_lFileDirRecv);
	
	if( access(m_lFileDirRecv, F_OK) != 0 )	{
	  char sTemp[256] = "mkdir -p ";
	  strcat(sTemp, m_lFileDirRecv);
	  system(sTemp);
	}
	
	if (!ParamDefineMgr::getParam("POWER_MAIN","FILE_RECV_BAK",m_lFileDirRecvBak,128))	{
	  printf("b_param_defineδ������ ���������ļ����ձ���Ŀ¼");
	  exit;
	}
	if (m_lFileDirRecvBak[strlen(m_lFileDirRecvBak)-1] != '/')
	  m_lFileDirRecvBak[strlen(m_lFileDirRecvBak)] = '/';

	if( access(m_lFileDirRecvBak, F_OK) != 0 )	{
	  char sTemp[256] = "mkdir -p ";
	  strcat(sTemp, m_lFileDirRecvBak);
	  system(sTemp);
	}

	if (!ParamDefineMgr::getParam("POWER_MAIN","FILE_RESP",m_lFileDirResp,128)) {
	  printf("b_param_defineδ������ ���������ļ������ļ��ӿ�Ŀ¼");
	  exit;
	}
	if (m_lFileDirResp[strlen(m_lFileDirResp)-1] != '/')
	  m_lFileDirResp[strlen(m_lFileDirResp)] = '/';
	rDir = Directory(m_lFileDirResp);	
	
	if( access(m_lFileDirResp, F_OK) != 0 )	{
	  char sTemp[256] = "mkdir -p ";
	  strcat(sTemp, m_lFileDirResp);
	  system(sTemp);
	}
	
	if (!ParamDefineMgr::getParam("POWER_MAIN","FILE_RESP_BAK",m_lFileDirRespBak,128))	{
	  printf("b_param_defineδ������ POWER_MAIN TEMP_FILE_DIR");
	  exit;
	}
	if (m_lFileDirRespBak[strlen(m_lFileDirRespBak)-1] != '/')
	  m_lFileDirRespBak[strlen(m_lFileDirRespBak)] = '/';

	if( access(m_lFileDirRespBak, F_OK) != 0 )	{
	  char sTemp[256] = "mkdir -p ";
	  strcat(sTemp, m_lFileDirRespBak);
	  system(sTemp);
	}

	char sFile[254];char sTemp[254],sHome[256];
	char * p;
	if ((p=getenv ("TIBS_HOME")) == NULL)
	  sprintf (sHome, "/bill/rtbill/TIBS_HOME/");
	else
	  sprintf (sHome, "%s", p);

	if (sHome[strlen(sHome)-1] == '/') {
	  sprintf (sFile, "%setc/data_access_new.ini", sHome);
	} 
	else {
	  sprintf (sFile, "%s/etc/data_access_new.ini", sHome);
	}	
	ReadIni reader;	
	reader.readIniString (sFile, (char *)"POWER_FTP", (char *)"ip", m_sIP, (char *)"");
	reader.readIniString (sFile, (char *)"POWER_FTP", (char *)"username", m_sUser,(char *)"");
	reader.readIniString (sFile, (char *)"POWER_FTP", (char *)"password", sTemp, (char *)"");
	decode(sTemp,m_sPass);
	Alltrim(m_sPass);
	reader.readIniString (sFile, (char *)"POWER_FTP", (char *)"recv_path", m_rRecvPath, (char *)"");	
	reader.readIniString (sFile, (char *)"POWER_FTP", (char *)"recv_bak" , m_rRecvBak,(char *)"");
	reader.readIniString (sFile, (char *)"POWER_FTP", (char *)"resp_path", m_rRespPath,(char *)"");


	m_pQuery = new PowerQuery();
	
}

PowerFile::PowerFile()
{
	init();	
}

PowerFile::~PowerFile()
{
	if ( m_pQuery ) delete m_pQuery;
}

void PowerFile::sendFile()
{
	char sFile[124]={0};
	char sBakFile[256]={0};
	char sLocalFile[256]={0};
	char sRemoteFile[256]={0};
	int  iPct;
	File *pFile;
	int iDeal=0;
	CFTPClient ftpc;
	if ( ftpc.FTPLogin(m_sIP,m_sUser,m_sPass) != 0 ) 
		return;
	ftpc.FTPOption(DATATYPE,BINARY);
	while (( pFile = rDir.nextFileRecursive() ) != NULL)	{
		strcpy(sFile,pFile->getFileName());
		sprintf(sRemoteFile,"%s/%s",m_rRespPath,sFile);
		sprintf(sLocalFile,"%s%s",m_lFileDirResp,sFile);
		sprintf(sBakFile,"%s%s",m_lFileDirRespBak,sFile);
		ftpc.PutFile(sLocalFile,sRemoteFile,&iPct);
		pFile->moveFile(sBakFile);
		iDeal++;		
	}
	Log::log(0,"������ <%d> ���ļ�������FTP������!",iDeal);
}

void PowerFile::getFile()
{

	char sRet[2048]={0};
	char sFile[124]={0};
	char sBakFile[256]={0};
	char sLocalFile[256]={0};
	char sTempFile[256];
	int iDeal=0;
	CFTPClient ftpc;
	if ( ftpc.FTPLogin(m_sIP,m_sUser,m_sPass) != 0 ) 
		return;
	ftpc.FTPOption(DATATYPE,BINARY);
	ftpc.FTPCmd(CHDIR,m_rRecvPath);
	strcpy(sTempFile,"recv.txt");
	ftpc.FTPCmd(LS,NULL,sTempFile);
	listFile(sTempFile);

	for(vector<string>::iterator iter=m_listFile.begin();iter !=m_listFile.end();iter++) {
		strcpy(sFile,(*iter).c_str());
		sprintf(sLocalFile,"%s%s",m_lFileDirRecv,sFile);//�����ļ���
		sprintf(sBakFile,"%s/%s",m_rRecvBak,sFile);//ftp�����������ļ���
		//printf("file: <%s>\n",sFile);
		ftpc.GetFile(sFile,sLocalFile);//��FTP��ȡ�ļ�������
		ftpc.FTPCmd(RENAME,sFile,sBakFile);		//FTP�ļ�����
		iDeal++;
	}
	ftpc.FTPLogout();
	Log::log(0,"�ӵ���FTP�������ϻ�ȡ�� <%d> ���ļ�!",iDeal);
}

void PowerFile::listFile(char * sTempFile)
{
    struct stat statbuf;
    int         iFileSize = 0;
    int         iServerFileSize = 0;    /////���������ļ��Ĵ�С
    char        sTemp[500];
    
    m_listFile.clear();
    
    if ( stat( sTempFile, &statbuf ) == 0 )
    {
        iFileSize = statbuf.st_size;
    }

    ////////////////��ȡ�б�����
    if ( iFileSize > 0 )
    {

        char*   sFileInfo = new char[iFileSize + 1];

        if ( sFileInfo == NULL )
        {
            return ;
        }

        FILE*   theFp = fopen( sTempFile, "rb" );
        if ( theFp == NULL || fread( sFileInfo, iFileSize, 1, theFp ) != 1 )
        {
            Log::log( 0, "��ȡ�ļ��б�ʧ�ܣ�" );
            delete[] sFileInfo;
            return ;
        }

        if ( theFp )
        {
            fclose( theFp );
        }

        theFp = NULL;

        sFileInfo[iFileSize] = '\0';

        const char  sDiv[] = " ";                   ////��ָ���
        const int   iSizeOffCnt = 5;                /////�ļ���С�ǵ�5����,��1��ʼ����,��awk����һ��
        const int   iOff = sizeof( sDiv ) - 1;

        char*       pBegin = sFileInfo;             //////һ�еĿ�ͷ
        char*       pEnd = sFileInfo;               //////һ�еĽ�β
        char*       pSize = sFileInfo;              //////ָ���ļ���С
        char*       pDate = sFileInfo;               //ָ���ļ���ʱ��
        char*       pName = sFileInfo;              //////ָ�� �ļ�/Ŀ¼ ����
        char*       pOver = &sFileInfo[iFileSize];  /////�ļ����ݽ���
        int         iCnt = 0;
        pEnd--;
        while ( pEnd < pOver )
        {
            pEnd++;
            pBegin = pEnd;                  //////�ϴε�end����ε�begin
            for ( ; pEnd < pOver; pEnd++ )
            {
                if ( *pEnd == '\n' || *pEnd == '\0' || *pEnd == '\r' )
                {
                    break;
                }
            }

            /////�ҵ�һ�н�����
            
                          ///////ֻ����һ�еĿ�ʼ��d ���� - ��
            if ( *pBegin != 'd' && *pBegin != '-' )
            {
                continue;

            }

            *pEnd = '\0';
            if ( *pBegin == '\0' )
            {
                continue;
            }

            ////���ļ�/Ŀ¼����
            for ( pName = pEnd - 1; pName > pBegin; pName-- )
            {
                if ( strstr( pName, sDiv ) != NULL )
                {
                    break;
                }
            }

            pName += iOff;

            /////�ҵ�������
            if ( *pBegin == 'd' )
            {                               /////��Ŀ¼
                                /////��.��ͷ�Ķ���Ҫ
                if ( *pName != '.' )
                {
                    ;//Ŀ¼�Ͳ����ˣ������κδ���
                }
            }
            else if ( *pBegin == '-' )
            {                               /////���ļ�
                
                                ////���ļ���С
                pSize = pBegin;

                char*   pTmp;
                for ( iCnt = 0; iCnt < iSizeOffCnt - 1; iCnt++ )
                {
                    pSize = strstr( pSize, sDiv );
                    if ( pSize == NULL )
                    {
                        break;
                    }

                    if ( iOff == 1 )
                    {                       ///////���ַ��ظ�����һ���ָ�
                        while ( pSize < pEnd )
                        {                   ////
                            pSize++;
                            if ( ( *pSize ) != sDiv[0] )
                            {
                                break;
                            }
                        }
                    }
                }

                if ( pSize == NULL )
                {                           ////û���ҵ��ļ���С���Ǹ���
                    continue;
                }

                if ( iOff != 1 )
                {
                    pSize += iOff;

                }

                ////�ҵ��ļ���С����
                iServerFileSize = atoi( pSize );
                if ( iServerFileSize < 0 )
                {
                    continue;
                }
                
                //## ȡftp�ļ�����ʱ��                
                pDate = strstr(pSize, sDiv);
                if (pDate != NULL)
                {
                    pDate += iOff;
                    char sTempDate[13];
                    memset (sTempDate, 0, sizeof(sTempDate));
                    strncpy (sTempDate, pDate, 12);
                    sTempDate[12] = 0;
                    if (!CFTPClient::changeDateFormat(sTempDate, sTemp))
                    {
                        sTemp[0] = 0;
                    }
                }
                else
                    sTemp[0] = 0;
                                
//printf("%20s%20d%20s\n",pName,iServerFileSize,sTemp)        ;                        
                m_listFile.push_back( pName );

            }
        }

        delete[] sFileInfo;
    }
}

void PowerFile::setUserInfo(){
	m_pQuery->setHLRinfo();
}