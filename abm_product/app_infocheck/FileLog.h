/******************************************************************************************
���ʤ
���żƷ��з���
2010.05.19
�ļ���־��
 ******************************************************************************************/
#ifndef _FILELOGAPPENDER_H_
#define _FILELOGAPPENDER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sstream>
#include "MutexLock.h"
const int MAXSORTPATH = 128;
const int ID_LEN_32 = 32;
class CFileLog
{
public:
      //����·��Ŀ¼
      CFileLog ( std::string strFilePath );
      //��ǰĿ¼
      CFileLog();
      void setInfo ( const char* );
      //std::string GetSource(const char*);
      virtual ~CFileLog();
      int WriteLog ( int nPriority, const char* _FileName, int nPosition, const char* szLogInfo );
private:
      bool OpenFile ( std::string strFileName );
      bool CloseFile();
      std::string CreateFileName();
      std::string formatLog ( int nPriority, const char* _FileName, int nPosition, const char* szLogInfo );
      void DelObject();
      bool IsReady();
private:
      //std::ofstream* pStream;
      std::string m_strFilePath;
      std::string m_strFileName;
      std::ofstream* m_pStream;
      std::string m_moduleName;
      bool m_bIsOpen;

      CmutexLock* m_pLock;
};
#endif
