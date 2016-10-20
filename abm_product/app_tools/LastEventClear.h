#ifndef _LASTEVENTCLEAR_H_
#define _LASTEVENTCLEAR_H_

#include <strings.h>
#include "Process.h"
#include "ProcessInfo.h"
#include "WfPublic.h"
#include "IpcKey.h"

using namespace std;

class ClearLastEvent
{
	public:
		ClearLastEvent();
		~ClearLastEvent();
		void log(char *format,...);  //��־����
		bool SetEventStateProcessID(int iProcessID,int iState ); //�����̺�����
		int GetShmAddress(TSHMCMA **ppTable,long iShmKey);		
		TSHMCMA * GetShmTable();
	private:
		TSHMCMA * p_shmTable;
};

#endif
