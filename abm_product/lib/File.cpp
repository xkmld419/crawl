/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "File.h"
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

//##ModelId=41DCAC7E0081
File::File(char *pFileName)
{
	
	m_sFilePath[0] = 0;
	m_sFileName[0] = 0;
	
	strcpy (m_sFullFileName, pFileName);
	this->m_iFileSize = -1;
	
	/* �����ļ�����·�� */
	this->getPathAndFile ();
	this->getSize ();
}

//##ModelId=41DCAC7E008E
File::~File()
{
}

//##ModelId=41DCAC7E008F
bool File::moveFile(char *newName)
{
	char sTempFile[500];

	strcpy (sTempFile, newName);
	/*******************/
	/******* ���� ******/
	/*******************/
	if (this->copyFile (sTempFile) == false)
		return false;
	
	/********************/
	/******* ɾ�� *******/
	/********************/
	
	if (this->rm () == false)
		return false;
	else {
		strcpy (m_sFullFileName, sTempFile);
		this->getPathAndFile ();
		return true;		
	}

}

//##ModelId=41DCAC7E0097
bool File::rm()
{
	/* ����ļ����� */
	if (access (m_sFullFileName, 0) == 0)
		unlink (m_sFullFileName);

	/* ��ʵ���ļ��Ƿ����, ȷ���ļ��Ƿ�ɾ���ɹ� */
	if (access (m_sFullFileName, 0) == 0) {
		sprintf (m_sErrorStr, "ɾ���ļ�[%s]ʧ��", m_sFullFileName);
		return false;
	} else {
		return true;
	}
}

//##ModelId=41DCAC7E0083
File::File(char *pFilePath, char *pFileName)
{
	strcpy (m_sFilePath, pFilePath);
	strcpy (m_sFileName, pFileName);
	
	int iPathLen = strlen (pFilePath);
	
	/* ·������ "/" */
	if (iPathLen > 0) {
		if (m_sFilePath[iPathLen-1] != '/') 
			strcat (m_sFilePath, "/");	
	}

	this->m_iFileSize = -1;
	
	strcpy (m_sFullFileName, m_sFilePath);
	strcat (m_sFullFileName, m_sFileName);

	this->getSize ();
}

//##ModelId=41DCAC7E0098
char *File::getFileName()
{
	return m_sFileName;
}

//##ModelId=41DCAC7E0099
char *File::getFilePath()
{
	return m_sFilePath;
}

//##ModelId=41DCAC7E00A0
char *File::getFullFileName()
{
	return m_sFullFileName;
}

//##ModelId=41DCAC7E00A2
int File::setSize(int size)
{
	m_iFileSize = size;
	return size;
}

//##ModelId=41DCAC7E00A1
int File::getSize()
{
	/* ����ļ���С��δ��� */
	if (m_iFileSize == -1) {
		struct stat statbuf;
		if (stat (m_sFullFileName, &statbuf) == 0)
			m_iFileSize = statbuf.st_size;
	}

	return m_iFileSize;
}

//##ModelId=41DCAC7E00B9
bool File::getPathAndFile()
{
	int iNameLen = strlen (m_sFullFileName);
	int i;
	
	/* m_sFullFileName����Ϊ·�� */
	if (m_sFullFileName[iNameLen-1]=='/' || m_sFullFileName[iNameLen-1]=='\\')
		return false;
		
	for (i=iNameLen-1; i>=0; i--) {
		/* ���� unix �� win ·����ʾ */
		if (m_sFullFileName[i]=='/' || m_sFullFileName[i]=='/')
			break;
	}
	
	if (i >= 0) {
		memcpy (m_sFilePath, m_sFullFileName, i+1);
		m_sFilePath[i+2] = 0;
		strcpy (m_sFileName, (char *)&m_sFullFileName[i+1]);
	}
	return true;
}

//##ModelId=41DCAC7E00AA
void File::setFileName(char *pFileName)
{
	strcpy (m_sFileName, pFileName);
	strcpy (m_sFullFileName, m_sFilePath);
	strcat (m_sFullFileName, m_sFileName);
}

//##ModelId=41DCAC7E00AC
bool File::copyFile(char *newFile)
{
	int from_fd,to_fd; 
	int iLen, bytes_read,bytes_write; 
	char buffer[10240], newName[256]; 
	char *ptr;

	/*********************/
	/****** ��׳�Ա�֤ ***/
	/*********************/
	
	if ((iLen = strlen (newFile)) == 0)
		return false;

	strcpy (newName, newFile);

	/* ���Ŀ�괮�����·�� */
	if (newFile[0] != '/')
		sprintf (newName, "%s%s", this->m_sFilePath, newFile);

	/* ���Ŀ�괮��·�� */
	iLen = strlen (newName);
	if (newName[iLen-1] == '/')
		strcat (newName, this->m_sFileName);

	/* Ŀ���ļ���Դ�ļ�������� */
	if (strcmp (this->m_sFullFileName, newName) == 0) {
		sprintf (m_sErrorStr, "Ŀ���ļ���Դ�ļ��������[%s]", newName);
		return false;
	}
		
	/*******************/
	/******* ���� ******/
	/*******************/

	if((from_fd=open(m_sFullFileName,O_RDONLY))==-1) {
		sprintf (m_sErrorStr, "���ļ�[%s]ʧ��!", m_sFullFileName);
		return false;
	}
	
	if((to_fd=open(newName,O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP))==-1) {
		sprintf (m_sErrorStr, "�����ļ�[%s]ʧ��!", newName);
		close (from_fd);
		return false;	
	}
	
        bool bRet = true;
        while(bytes_read=read(from_fd, buffer, 10240)) {
                /* һ�������Ĵ������� */
                if((bytes_read==-1)&&(errno!=EINTR)) {
                        bRet = false;
                        break;
                }
                else if(bytes_read>0) {
                        ptr=buffer;
                        while(bytes_write=write(to_fd,ptr,bytes_read)) {
                                /* һ�������������� */
                                if((bytes_write==-1)&&(errno!=EINTR)) {
                                        bRet = false;
                                        break;
                                }
                                /* д�������ж����ֽ� */
                                else if(bytes_write==bytes_read)
                                        break;
                                /* ֻд��һ����,����д */
                                else if(bytes_write>0) {
                                        ptr+=bytes_write;
                                        bytes_read-=bytes_write;
                                }
                        }
                        /* д��ʱ�������������� */
                        if(bytes_write==-1) {
                                bRet = false;
                                break;
                        }
                }
        }

        close(from_fd);
        close(to_fd);

        strcpy (newFile, newName);

        return bRet;
}

//##ModelId=41DCAC7E00B5
char *File::getError()
{
	return m_sErrorStr;
}

//##ModelId=41DCAC7E00B6
char *File::getState()
{
	return m_sState;
}

//##ModelId=41DCAC7E00B7
bool File::setState(char *pState)
{
	strncpy (m_sState, pState, 9);
	return true;
}

