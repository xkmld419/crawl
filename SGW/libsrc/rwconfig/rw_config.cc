/*
* Copyright 2001 LinkAge Co.,Ltd. Inc. All Rights Reversed
*/

/*********************************************************************************/
/*** Name             : CONFIG.CPP                                             ***/
/***                                                                           ***/
/*** Description      : ��д�����ļ�ģ�鹦�ܺ���                               ***/
/***                                                                           ***/
/*** Author           : ����                       	                       ***/
/*** Upate			  : ����                                               ***/
/***                                                                           ***/
/*** Begin Time       : 2001/11/03                                             ***/
/***                                                                           ***/
/*** Last Change Time : 2001/11/09       	                               ***/
/***                                                                           ***/
/*********************************************************************************/
//modify:��Ҫ�޸��˴˹���ģ��ͳ����Ļ�������ͻ������, bad smell.
/*********************************************************************************/
/***              Class C_Config user include Compile body                     ***/
/***              Config encapsulate all methods(functions)            	       ***/
/*********************************************************************************/

#include "rw_config.h"

C_Config::C_Config()
{
	iWriteFlag = 0;
	memset(chFileTemp, 0, sizeof(chFileTemp));
}

/*
*	Function Name	:Trim
*	Description	:ȥ���ַ������ҵĿո�
*	Input Param	:��Ҫȥ���ո���ַ���
*	Returns		:��
*	complete	:2001/11/20
*/
void C_Config::Trim(char * String)
{
	char *Position = String;
	/*�ҵ���һ�����ǿո��λ��*/
	while(isspace((unsigned char)*Position))
	{
		Position++;
	}
	/*���Ϊһ�մ����˳�*/
	if (*Position == '\0')
	{
		*String = '\0';
		return;
	}

	/*��ȥǰ��Ŀո�*/
	while(*Position)
	{
		*String = *Position;
		String++;
		Position++;
	}
	/*��ȥ����Ŀո�*/
	do
	{
		*String = '\0';
		String--;
	}while(isspace((unsigned char)*String));
}

/*
*    Function Name    : GetLine
*    Description      :	�������ļ��л��һ�е�����
*    Input Param      :
*	Line        -----------> ��һ�е����ݣ��ַ�����
*	Fd          -----------> �򿪵��ļ�������
*	Returns          :�õ����ֽ���.
*	complete	:2001/11/09
*/
int C_Config::GetLine(int Fd,char *Line)
{
	int iByteRead = 0;
	for(;;)
	{
		if(read(Fd,Line,1) <= 0)
		{
			break;
		}
		iByteRead ++;
		if (*Line == '\n')
		{
			break;
		}
		Line++;
	}
	*Line='\0';
	return iByteRead;
}

/*
*    Function Name    : PutLine
*    Description      :	�������ļ���д��һ�е�����
*    Input Param      :
*	Line        -----------> ��һ�е����ݣ��ַ�����
*	Fd          -----------> �򿪵��ļ�������
*	Returns          :д����ֽ���.
*	complete	:2001/11/09
*/
int C_Config::PutLine(int Fd,char *Line)
{
	char chLine[LINELENGTH];

	sprintf(chLine, "%s\n", Line);
	return write(Fd,chLine,strlen(chLine));
}
/*
*    Function Name    : SetLockStatus
*    Description      :	��������״̬
*    Input Param      :
*	Fd         -----------> �򿪵��ļ�˵����
*	Cmd        -----------> ������
*	Type       -----------> �ļ�������
*	Offset     -----------> ���λ����
*	Whence     -----------> ���λ�������
*	Len        -----------> ����ĳ���
*	Returns          :����״̬
*	complete	:2002/11/19
*/
int C_Config::SetLockStatus(int Fd, int Type, int Cmd, off_t Offset, int Whence, off_t Len)
{
	struct flock Lock;

	Lock.l_type = Type;
	Lock.l_whence = Whence;
	Lock.l_start = Offset;
	Lock.l_len = Len;

	return fcntl(Fd, Cmd, &Lock);
}

/*
*    Function Name    : GetParam
*    Description      :	��ȡ�����ļ��ļ�ֵ
*    Input Param      :
*	FileName     -----------> �����ļ���
*	Section      -----------> ��ȡ������
*	Key          -----------> ��ȡ�Ĺؼ�����
*	Value        -----------> ��ȡ�ļ�ֵ
*	Returns          :��������͹ؼ����������ڣ��򷵻�true���õ���ȷ�ļ�ֵ
*			  ���򷵻�false
*	complete	:2002/11/19
*/
bool C_Config::GetParam(const char *FileName,const char *Section,const char *Key,char *Value)
{
	if(-1 == access(FileName, F_OK|R_OK))
	{
		return false;
	}

	int fd;
	int iCount,iLength;
	char chBuff[LINELENGTH],chTemp[LINELENGTH];
	char chValueTemp[LINELENGTH];
	char chSection[LINELENGTH],chKey[LINELENGTH];
	char *pchPoint = NULL,*pchTemp = NULL,*pchEnv = NULL;
	string sLine,sSection;

	memset(chTemp, 0, sizeof(chTemp));

	iCount = 0;

	if(strcmp(FileName,chFileTemp) != 0)
	{
		VRecord.clear();
		strcpy(chFileTemp,FileName);

		if(-1 == (fd = open(FileName,O_RDONLY)))
		{
			return false;
		}

		//�������ļ����ݷ�������������
		for(;;)
		{
			memset(chBuff,0,sizeof(chBuff));
			if ((iCount = GetLine(fd,chBuff)) == 0)
			{
				break;
			}
			Trim(chBuff);
			if ((';' == chBuff[0]) || ('#' == chBuff[0]) || ('\0' == chBuff[0]))
			{
				continue;
			}
			sLine = chBuff;
			VRecord.push_back(sLine);
		}
		close(fd);
	}

	iCount = 0;
	vector<string>::iterator it;
	it = VRecord.begin();

	memset(chSection, 0, sizeof(chSection));
	memset(chKey,0,sizeof(chKey));

//�õ�Section
	memset(chTemp, 0, sizeof(chTemp));
	sprintf(chTemp,"[%s]",Section);
	sSection = chTemp;

	while(it<VRecord.end())
	{
		if (*it == sSection)
		{
			it++;
			iCount++;
			break;
		}
		it++;
		iCount++;
		if (iCount == VRecord.size())
		{
			return false;
		}
	}

//�õ�Key
	while(it<VRecord.end())
	{
		memset(chBuff, 0, sizeof(chBuff));
		strcpy(chBuff, (*it).c_str());

		//�ڱ������Ҳ����ؼ���
		if ('[' == chBuff[0])
		{
			return false;
		}

		pchPoint = chBuff;
		if ((pchTemp = strchr(pchPoint,'=')) != NULL)
		{
			strncpy(chKey, chBuff, pchTemp - pchPoint);
			chKey[pchTemp - pchPoint] = '\0';
			Trim(chKey);
		}

		if (!strcmp(chKey,Key))
		{
			it++;
			iCount++;
			break;
		}
		it++;
		iCount++;
		if (iCount == VRecord.size())
		{
			return false;
		}
	}

//�õ�Value
	pchTemp++;
	strcpy(chValueTemp,pchTemp);
	Trim(chValueTemp);//20030424�����޸ģ����ܲ������������ڴ沿�֣���Ӧ�ò����Լ����ڴ沿��
	chValueTemp[LINELENGTH-1] = 0;
	strcpy(Value, chValueTemp);

//���������ļ��Ƚϳ���ʱ��,��Ҫ����,����'\'���ű�־��Ϊ�ָ���
	iLength = strlen(Value);
	
	if ('\\' == Value[iLength - 1])
	{
		Value[iLength - 1] ='\0';
		while(it<VRecord.end())
		{
			memset(chBuff,0,sizeof(chBuff));
			strcpy(chBuff,(*it).c_str());
			iLength = strlen(chBuff);
			
			if (chBuff[iLength - 1] == '\\')
			{
				chBuff[iLength - 1] = '\0';
				strcat(Value,chBuff);
			}
			else if (chBuff[iLength - 1] != '\\')
			{
				strcat(Value,chBuff);
				break;
			}
			it++;
			iCount++;
			if (iCount == VRecord.size())
			{
				break;
			}
		}
	}

	if('$' == Value[0])
	{
		pchPoint = Value;
		pchPoint++;
		if(((pchTemp = strchr(pchPoint,'.')) != NULL) && (strchr(pchPoint, '/') == NULL))
		{
			strncpy(chSection,pchPoint,pchTemp - pchPoint);
			chSection[pchTemp - pchPoint] = '\0';
			pchTemp++;
			strcpy(chKey,pchTemp);

			return GetParam(FileName,chSection,chKey,Value);
		}
		if(pchTemp = strchr(pchPoint,'/'))
		{
			strncpy(chTemp,pchPoint,pchTemp - pchPoint);
			chTemp[pchTemp - pchPoint] = '\0';

			memset(Value,0,sizeof(Value));
			if((pchEnv = getenv(chTemp)) != NULL)
			{
				if(('/' == *(pchEnv + strlen(pchEnv) - 1))&&('/' == *pchTemp))
				{
					pchTemp++;
				}
				strcpy(chTemp,pchTemp);
				sprintf(Value,"%s%s",pchEnv,chTemp);
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

/*
*    Function Name    : SetParam
*    Description      :	���������ļ�
*    Input Param      :
*	Filename     -----------> �����ļ���
*	Section      -----------> ����������
*	Key          -----------> �����Ĺؼ�����
*	Value        -----------> �����ļ�ֵ
*	Returns          :����ļ������ڣ�����false
*                         ����ļ����ڣ���������򷵻�true
*	complete	:2001/11/09
*/
bool C_Config::SetParam(const char *FileName,const char *Section,const char *Key,const char *Value)
{
	int fdt,fd;
	int iCount = 0;
	char chTemp[LINELENGTH], chTempName[LINELENGTH];;
	char chSection[LINELENGTH],chKey[LINELENGTH];
	char *pchPoint = NULL,*pchTemp = NULL;
	string sLine;

	sprintf(chTempName,"%s.tmp",FileName);
	if(-1 == (fdt = open(chTempName,O_RDWR|O_CREAT|O_EXCL)))
	{
		if(-1 == (fdt = open(chTempName,O_RDWR|O_TRUNC)))
		{
			unlink(chTempName);
			return false;
		}
	}
	//�����ļ�д��
	SetLockStatus(fdt,F_WRLCK);

	fchmod(fdt,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	//�����ļ�д��־
	iWriteFlag = 1;
	//�����������
	VRecord.clear();

	if(-1 == (fd = open(FileName,O_RDONLY)))
	{
		SetLockStatus(fdt,F_UNLCK);
		close(fdt);
		iWriteFlag = 0;
		unlink(chTempName);
		return false;
	}

	//���ļ�����ȫ������������
	for(;;)
	{
		memset(chTemp, 0, sizeof(chTemp));
		if(0 == GetLine(fd, chTemp))
		{
			break;
		}
		sLine = chTemp;
		VRecord.push_back(sLine);
	}
	close(fd);

	//��ֹ�ļ��ն�����ԭ�ļ����Ƚ�Ϊ0
	if (-1 == (fd = open(FileName,O_RDWR|O_CREAT|O_TRUNC)))
	{
		SetLockStatus(fdt,F_UNLCK);
		close(fdt);
		iWriteFlag = 0;
		unlink(chTempName);
		return false;
	}

	//�����ļ�����Ϊֻ��,��ֹ�������޸�
	if (-1 == fchmod(fd,S_IRUSR|S_IRGRP))
	{
		SetLockStatus(fdt,F_UNLCK);
		close(fdt);
		iWriteFlag = 0;
		unlink(chTempName);
		return false;
	}

	iCount = 0;
	vector<string>::iterator it;
	it = VRecord.begin();

	memset(chSection, 0, sizeof(chSection));
	memset(chKey,0,sizeof(chKey));

//�õ�Section
	sprintf(chSection,"[%s]",Section);

	//����2003-01-20�޸�
	//����ļ�Ϊ��,���½�����ѡ��,д���ļ�
	if (VRecord.size() == 0)
	{
		sprintf(chKey, "\t%s = %s", Key, Value);
		PutLine(fd, chSection);
		PutLine(fd, chKey);
		//�ͷ������ر��ļ�
		SetLockStatus(fdt,F_UNLCK);
		fchmod(fd,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
		close(fdt);
		close(fd);
		unlink(chTempName);
		return true;
	}

	while(it<VRecord.end())
	{
		memset(chTemp, 0, sizeof(chTemp));
		strcpy(chTemp, (*it).c_str());
		PutLine(fdt, chTemp);
		it++;
		iCount++;
		Trim(chTemp);
		if ((';' == chTemp[0]) || ('#' == chTemp[0]))
		{
			continue;
		}

		if (!strcmp(chTemp, chSection))
		{
			break;
		}

		//�Ѿ����ļ�ĩβ,û�и�����,�����ļ�ĩβ�½���������д�����ֵ
		if (iCount == VRecord.size())
		{
			for(it = VRecord.begin();it<VRecord.end();it++)
			{
				memset(chTemp, 0, sizeof(chTemp));
				strcpy(chTemp, (*it).c_str());
				PutLine(fd, chTemp);
			}

			sprintf(chKey, "\t%s = %s", Key, Value);
			PutLine(fd, chSection);
			PutLine(fd, chKey);
			//�ͷ������ر��ļ�
			SetLockStatus(fdt,F_UNLCK);
			fchmod(fd,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
			close(fdt);
			close(fd);
			unlink(chTempName);
			return true;
		}
	}

	//����2003-01-21�޸�
	//�ҵ��˸�����,�����Ѿ����ļ����һ��,�����ļ�ĩβд�����ֵ
	if (iCount == VRecord.size())
	{
		for(it = VRecord.begin();it<VRecord.end();it++)
		{
			memset(chTemp, 0, sizeof(chTemp));
			strcpy(chTemp, (*it).c_str());
			PutLine(fd, chTemp);
		}
		sprintf(chKey, "\t%s = %s", Key, Value);
		PutLine(fd, chKey);
		//�ͷ������ر��ļ�
		SetLockStatus(fdt,F_UNLCK);
		fchmod(fd,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
		close(fdt);
		close(fd);
		unlink(chTempName);
		return true;
	}

//�õ�Key
	while(it<VRecord.end())
	{
		memset(chTemp, 0, sizeof(chTemp));
		strcpy(chTemp, (*it).c_str());
		Trim(chTemp);

		//������û�иùؼ���,�½�����ֵ��д���ļ�
		if ('[' == chTemp[0])
		{
			sprintf(chKey, "\t%s = %s", Key, Value);
			PutLine(fdt, chKey);
			PutLine(fdt, chTemp);
			it++;
			iCount++;
			break;
		}
		if ((';' == chTemp[0]) || ('#' == chTemp[0]))
		{
			it++;
			iCount++;
			PutLine(fdt, chTemp);
			continue;
		}

		pchPoint = chTemp;
		if ((pchTemp = strchr(pchPoint,'=')) != NULL)
		{
			strncpy(chKey, chTemp, pchTemp - pchPoint);
			chKey[pchTemp - pchPoint] = '\0';
			Trim(chKey);
		}
		if (!strcmp(chKey,Key))
		{
			it++;
			iCount++;
			sprintf(chKey, "\t%s = %s", Key, Value);
			PutLine(fdt, chKey);
			break;
		}

		sprintf(chTemp, "%s",(*it).c_str());
		PutLine(fdt, chTemp);
		it++;
		iCount++;
		//�Ѿ����ļ�ĩβ,����Ӹò���ֵ
		if (iCount == VRecord.size())
		{
			for(it = VRecord.begin();it<VRecord.end();it++)
			{
				memset(chTemp, 0, sizeof(chTemp));
				strcpy(chTemp, (*it).c_str());
				PutLine(fd, chTemp);
			}

			sprintf(chKey, "\t%s = %s", Key, Value);
			PutLine(fd, chKey);
			//�ͷ������ر��ļ�
			SetLockStatus(fdt,F_UNLCK);
			fchmod(fd,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
			close(fdt);
			close(fd);
			unlink(chTempName);
			return true;
		}
	}

	while(it<VRecord.end())
	{
		memset(chTemp, 0, sizeof(chTemp));
		strcpy(chTemp, (*it).c_str());
		PutLine(fdt, chTemp);
		it++;
	}

	//����ʱ�ļ��������ƶ�����ʱ�ļ�ͷ,������д��ԭ�ļ�
	lseek(fdt,0,SEEK_SET);

	for(;;)
	{
		memset(chTemp, 0, sizeof(chTemp));
		if(0 == GetLine(fdt, chTemp))
		{
			break;
		}
		PutLine(fd, chTemp);
	}

	SetLockStatus(fdt,F_UNLCK);
	fchmod(fd,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	close(fdt);
	close(fd);
	unlink(chTempName);
	return true;
}

