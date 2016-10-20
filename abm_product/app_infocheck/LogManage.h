/************************************************************************
���ʤ
���żƷ��з���
2010.05.19
��־������
*************************************************************************/

#ifndef _LOGMAMAGE_H__
#define _LOGMAMAGE_H__
#include <pthread.h>
#include "FileLog.h"
#include "MutexLock.h"
class CLogManage
{
public:
      static CLogManage* getInstance();
      int setLog ( const char* _ProcessName);
      int setLog(const int _ProcessId);
      virtual ~CLogManage();
      static int WriteLog ( int nPriority, const char* _FileName, int nPosition, const char* szLogInfo );
protected:
      CLogManage();
private:
      static CLogManage* m_pLogManage;
      static CFileLog* m_pFileLog;
};
#endif
