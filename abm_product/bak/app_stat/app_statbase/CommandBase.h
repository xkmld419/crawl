#ifndef COMMANDBASE_H_HEADER
#define COMMANDBASE_H_HEADER

#include <stdio.h>
#include <string>
#include <unistd.h>
#include "../app_init/CommandCom.h"
#include "Environment.h"
#include "Date.h"
#include "Log.h"


#define DEFINE_COMMAND_MAIN(x) CommandBase *GetCommandBase () { return (new x()); }
#define DEFINE_SPLIT "|"

class 	CommandBase
{
    public:

	//���麯��
	virtual int run()=0;
	
	CommandBase();
	virtual ~CommandBase();

	bool WriteSocket(int ConnectSocket,char * msg);  //��ǰ̨����Ϣ
	bool WriteSocket(int ConnectSocket,string & msg);  //��ǰ̨����Ϣ

	int log(int log_level, char *format,  ...);  //��־�ӿ�
    //��ǰ̨Э���ǰ4�е������ʽ
    //sCommand:���iSize:���������
    //sLable:�����������ƴ�ӣ�ifTable:��ʾ�Ƿ��Ǳ���ʽ�����ǰ̨
	void FourOutForWeb(int iSize,char * sLable,int ifTable=1);
	void ErrOutForWeb(char *sErrMsg);
	void EndOutForWeb();
	void SpecialOutForWeb(char *sMsg,int ifTable=0);  //���������

	int ConnectSocket;   //socket�׽���
	CommandCom * m_pCommandComm;
};
#endif




