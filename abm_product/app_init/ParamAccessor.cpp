/********************************************
*˵�������������������Ͷ��ڹ���
*			�����ļ��д���effDate�ֶεģ���Ϊ���ڹ���
*			�����ļ��в�����effDate�ֶεģ���Ϊ��������
*			��������	
*							��ȡ����ֵ��ֱ��ȡvalueֵ
*							�޸Ĳ���ֵ��ֱ���޸�valueֵ
*			���ڹ���
*							��ȡ����ֵ���жϵ�ǰ��ȡʱ���Ƿ���ڵ���effDate���ǣ���ȡnewValueֵ������ȡvalueֵ
*							�޸Ĳ���ֵ�����effDate�ֶ�Ϊ�գ���ֱ���޸�newValueֵ
*													���effDate�ֶβ�Ϊ�գ����жϵ�ǰ�޸�ʱ���Ƿ���ڵ���effDate���ǣ���ʹ��newValue�滻value,������effDateΪ��ǰ�޸�ʱ��
*																																												�����޸�
*
********************************************/

#include "ParamAccessor.h"

ParamAccessor::~ParamAccessor()
{
}

ParamAccessor* ParamAccessor::GetInstance(char *sFileName)
{

	if(0 == ParamAccessor::m_gpParamAccessor)
	{
		ParamAccessor::m_gpParamAccessor = new ParamAccessor(sFileName);
		if(!m_gpParamAccessor->Init())
		{
			delete m_gpParamAccessor;m_gpParamAccessor=0;
			return 0;
		}

	}
	return m_gpParamAccessor;
}

ParamAccessor::ParamAccessor(char *sFileName)
{
	memset(m_sFileName,0,150);
	memset(m_sTmpFileName,0,150);
	memset(m_sBakFileName,0,150);
	
	strcpy(m_sFileName,sFileName);
	sprintf(m_sTmpFileName,"%s.tmp",m_sFileName);
	sprintf(m_sBakFileName,"%s.bak",m_sFileName);
}

bool ParamAccessor::Init()
{
	return true;
}
char *ParamAccessor::TruncStr( char *sp , char *sDst , int len)
{
	memset(sDst , 0 ,  LINEMAXCHARS);
	strncpy(sDst , 	sp , len) ;
	return sDst;
}

char * ParamAccessor::trim( char *sp )
{
	char sDest[LINEMAXCHARS];
	char *pStr;
	int i = 0;

	if ( sp == NULL )
		return NULL;

	pStr = sp;
	while ( *pStr == ' ' || *pStr == '\t' )  pStr ++;
	strcpy( sDest, pStr );

	i = strlen( sDest ) - 1;
	while((i>=0)&&(sDest[i] == ' ' || sDest[i] == '\t' || sDest[i] == '\r' ) )
	{
		sDest[i] = '\0';
		i -- ;
	}

	strcpy( sp, sDest );

	return ( sp );
}

bool ParamAccessor::GetFieldValue(const char* strParamName, const char* field,char *oValue,int iMaxLen)
{
	char sBuffer[LINEMAXCHARS];
	char sKey[LINEMAXCHARS];
	char *p;
		char *pTmp;
	char sTmp1[LINEMAXCHARS];
	bool bFind=false;

	FILE *fd;
	if(!(fd=fopen(m_sFileName,"rb+")))
	{
        //ALARMLOG28(0,MBC_CLASS_File,101,"���ļ�ʧ��: File[%s]",m_sFileName);
        if(errno == ENOENT)
            //ALARMLOG28(0,MBC_CLASS_File,106,"ָ���ļ�������: File[%s]",m_sFileName);
		return false;
	}
	
	if(!field)
		sprintf(sKey,"%s",strParamName);
	else
		sprintf(sKey,"%s.%s",strParamName,field);

	while ( fgets( sBuffer, LINEMAXCHARS, fd ) )
	{
		trim( sBuffer );
		pTmp = (char *) sBuffer;
		if ( ( sBuffer[0] == '#' ) || ( sBuffer[0] == '\n' ) )
			continue;
		char *tmp=strstr(sBuffer,sKey);
		if(tmp==NULL)
		{
			continue;			
		}
		if((p=(char *) strchr(sBuffer,'=')) 
				&&(strcmp( trim(TruncStr( sBuffer , sTmp1 , p - pTmp))  ,  sKey ) == 0)
			)
		{
			//strcpy(oValue,trim(p+1));
			strncpy(oValue,trim(p+1),iMaxLen);
			
			//�ѻ��з�('\n')ȥ��
			oValue[strlen(oValue)-1]='\0';
			
			bFind=true;
			break;
		}
	}
	fclose(fd);
	return bFind;
}

bool ParamAccessor::GetParamValue(char* strParamName, char* oValue,int iMaxLen,int iMode)
{
	if(iMode==0)
	{
		bool flag=GetFieldValue(strParamName,NULL,oValue,iMaxLen);
		return false;
	}
	
	char effDate[15]={0};
	bool flag;//�Ƿ��ȡ������ֵ
	
	bool effFlag=GetFieldValue(strParamName,"effDate",effDate,sizeof(effDate));
	if(!effFlag)//����
	{
		flag=GetFieldValue(strParamName,"value",oValue,iMaxLen);
		return flag;
	}
	
	//����
		if(strcmp(trim(effDate),"")==0)//newValueΪ��
			flag=GetFieldValue(strParamName,"value",oValue,iMaxLen);
		else
		{
			Date d;
			Date d1(effDate);
			if(d<d1)
				flag=GetFieldValue(strParamName,"value",oValue,iMaxLen);
			else
				flag=GetFieldValue(strParamName,"newValue",oValue,iMaxLen);
		}
	return flag;
}

bool ParamAccessor::ModifyFieldValue(char *strParamName,char * field,char * oModifyValue)
{
	char sBuffer[LINEMAXCHARS]={0};
	char sKey[LINEMAXCHARS]={0};
	char *p;
		char *pTmp;
	char sTmp1[LINEMAXCHARS];
	
	FILE *fd;
	FILE *tmpFile;
	if(!(fd=fopen(m_sFileName,"rb")))
	{
        //ALARMLOG28(0,MBC_CLASS_File,101,"���ļ�ʧ��: File[%s]",m_sFileName);
        if(errno == ENOENT)
            //ALARMLOG28(0,MBC_CLASS_File,106,"ָ���ļ�������: File[%s]",m_sFileName);
		return false;
	}

	if(!(tmpFile=fopen(m_sTmpFileName,"wb+")))
	{
        //ALARMLOG28(0,MBC_CLASS_File,101,"���ļ�ʧ��: File[%s]",m_sTmpFileName);
        if(errno == ENOENT)
            //ALARMLOG28(0,MBC_CLASS_File,106,"ָ���ļ�������: File[%s]",m_sTmpFileName);
		return false;
	}
	
	if(!field || strcmp(field,"")==0)
		sprintf(sKey,"%s",strParamName);
	else
		sprintf(sKey,"%s.%s",strParamName,field);

	while ( fgets( sBuffer, LINEMAXCHARS, fd ) )
	{
		trim( sBuffer );
		pTmp = (char *) sBuffer;
		if ( ( sBuffer[0] == '#' ) || ( sBuffer[0] == '\n' ) )
		{
			fwrite(sBuffer,strlen(sBuffer),1,tmpFile);
			continue;
		}

		if(!strstr(sBuffer,sKey))
		{
			fwrite(sBuffer,strlen(sBuffer),1,tmpFile);
			continue;			
		}

		if((p=(char *) strchr(sBuffer,'=')) 
				&&(strcmp( trim(TruncStr( sBuffer , sTmp1 , p - pTmp))  ,  sKey ) == 0)
			)
		{
			fwrite(sKey,strlen(sKey),1,tmpFile);
			fwrite("=",strlen("="),1,tmpFile);
			fwrite(oModifyValue,strlen(oModifyValue),1,tmpFile);
			fwrite("\n",strlen("\n"),1,tmpFile);
		}
		else
			fwrite(sBuffer,strlen(sBuffer),1,tmpFile);
	}
	
	fflush(tmpFile);
	fclose(fd);
	fclose(tmpFile);
	remove(m_sFileName);
	rename(m_sTmpFileName,m_sFileName);

	return true;
}

bool ParamAccessor::ModifyParamValue(char* strParamName, char* oModifyValue,char *oModifyDate,int iMode)
{
	if(iMode==0)
	{
		char oValue[150]={0};
		if(GetFieldValue(strParamName,NULL,oValue,sizeof(oValue)))
		{
			ModifyFieldValue(strParamName,NULL,oModifyValue);
			return true;
		}
		return false;
	}
	
	char effDate[15]={0};
	char orignValue[LINEMAXCHARS]={0};
	//Date d;//��ǰ�޸ĵ�ϵͳʱ��
	bool flag=GetFieldValue(strParamName,"effDate",effDate,sizeof(effDate));
	GetFieldValue(strParamName,"newValue",orignValue,sizeof(orignValue));
	
	if(!flag)//����
	{
			ModifyFieldValue(strParamName,"value",oModifyValue);
			Log::log(0,"=========�޸�%s��valueֵΪ%s�ɹ�=============",strParamName,oModifyValue);
			return true;
	}

	//���ڹ���
		if(strcmp(trim(effDate),"")==0)//newValueΪ��
		{
			ModifyFieldValue(strParamName,"newValue",oModifyValue);
			ModifyFieldValue(strParamName,"effDate",oModifyDate);
		}
		else//newValue��Ϊ��
		{
			Date d1(effDate);
			Date d(oModifyDate);
			if(d>d1)//����޸�ʱ����ڵ���effDate����ʹ��newValue�滻vlaue���޸�newValue��effDate
			{								
				ModifyFieldValue(strParamName,"effDate",oModifyDate);
				ModifyFieldValue(strParamName,"newValue",oModifyValue);
				ModifyFieldValue(strParamName,"value",orignValue);
			}
			//����޸�ʱ��С��effDate���򲻴����ܾ��޸ģ�
			else
			{
				Log::log(0,"=========�޸�ʱ��С��effDate,�����޸�=============");
				return false;
			}
		}
	Log::log(0,"=========�޸�%s��valueֵΪ%s�ɹ�=============",strParamName,oModifyValue);
	return true;
}

void ParamAccessor::BeginTrans()
{
	//�ȱ��ݺ������ļ����Ա�ع���ʱ�򣬻ع�������ļ�
	Log::log(0,"=========�����޸�֮ǰ���ȱ��������ļ�sys.ini��sys.ini.bak���Ա����Ļع���========");
	FILE *fd;
	FILE *tmpFile;
	if(!(fd=fopen(m_sFileName,"rb")))
	{
        //ALARMLOG28(0,MBC_CLASS_File,101,"���ļ�ʧ��: File[%s]",m_sFileName);
        if(errno == ENOENT)
            //ALARMLOG28(0,MBC_CLASS_File,106,"ָ���ļ�������: File[%s]",m_sFileName);
		return ;
	}

	if(!(tmpFile=fopen(m_sBakFileName,"wb+")))
	{
        //ALARMLOG28(0,MBC_CLASS_File,101,"���ļ�ʧ��: File[%s]",m_sBakFileName);
        if(errno == ENOENT)
            //ALARMLOG28(0,MBC_CLASS_File,106,"ָ���ļ�������: File[%s]",m_sBakFileName);
		return ;
	}
	char buffer[150];
	 while(fgets(buffer,150,fd)!=NULL)
	 	fputs(buffer,tmpFile);

	fclose(fd);
	fclose(tmpFile);
	Log::log(0,"=========������ɣ�========");	
}
void ParamAccessor::rollback()
{
	remove(m_sFileName);
	rename(m_sBakFileName,m_sFileName);
	Log::log(0,"=========�ع��ɹ���=========");
}
void ParamAccessor::commit()
{
	remove(m_sBakFileName);
	Log::log(0,"=========�ύ�ɹ���=========");
}


ParamAccessor* ParamAccessor::m_gpParamAccessor;
