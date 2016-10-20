/*VER: 1*/ 
/********************************************************

 * class name:CFTPClient

 * purpose:ftp �ͻ���

 * Language:c++ 

 * version:4.1.4    

 * OS Platform: hp,sun, aix, dec

 * Author:  huangjb 

 * Date:  2002.02.05

 * modify: 2002.03.12, �޸�linux ftp server �˳���½ʱ����core dump���⣡

                ���ҶԴ�������һЩ������޲�

 *  2002.03.27, ���ӵ�½ʱ���ɹ��ķ���״̬

 *  2002.03.28, ���ӶԳ�ʱ�����趨���޸��Լ��Դ���һ���ļ������ʱ�����Ƶ������趨

 *  2002.04.04, ����SIZE, STAT,RENAME����, 4.0.8

 *  2002.04.04, ����FTPCmd���ػ�������󳤶ȵĲ�����ȡĬ��ֵʱʵ��ʹ��strcpy,����ʹ��strncpy, 4.1.0

 *  2002.04.04, �޸Ĵ�����з��ص������ 4.1.1

 *  2002.05.09, �޸�PutFile��STOR����������趨��Ӧ����ʹ��STOR REMOTEFILE)�� 4.1.2

 *  2002.05.20, �޸��ϵ��������ļ�ģʽ���ԣ����⽫STAT����ĳ�STATCMD���4.1.3

 *  2002.06.10, ���ӶԴ����ر��׽ӿڵļ�飬4.1.4

 * Cpryight (C)  Nanjing Linkage System Integration CO.,LTD.

 ********************************************************/


/*
//////////
zhangap
2007 05 
��������:
1. ʹ��ip��ַ��¼ʱ���²��ܵ�¼��bug
2. ��ȡ�ļ��б�ʱ,�еķ��������ڿ��ļ��в�����data�˿ڴ�������,����ֱ���ڿ��ƶ˿ڷ��ش������
   ���³����жϻ�ȡ�ļ��б�ʧ�ܵļ���
3.���� pasv ģʽ,ֱ���ں���������ж�, �е��ѿ���   
/////////

*/



#ifndef __CFTP_H_CLIENT

#define __CFTP_H_CLIENT



#define FTP_E_CONNECT -4
#define FTP_E_USER  -3
#define FTP_E_PASS  -2
#define FTP_E_OTHER -1
#define FTP_E_FILE  -5


typedef int   OPTVAL;



enum  QUERY{NO=(OPTVAL)0,YES=(OPTVAL)1,MAYBE=(OPTVAL)2};
enum  FTPOPTION{TIMEOUT,  DATATYPE,  RESUME, MAXWAITTIME, PASSIVE};
enum  FTPCMDTYPE{CHDIR, CDUP, DEL, LS, PWD, MKDIR, RMDIR, SYSTYPE, SIZE, STATCMD, RENAME, PASV ,MDTM};


#define BINARY    (OPTVAL)1
#define AUTO    (OPTVAL)1
#define ASCII   (OPTVAL)0



class CFTPClient

{

public:

  CFTPClient();

  virtual ~CFTPClient();

    

public: 

  int FTPLogin(const char *strServer, const char *strUser,
      const char *strPassword=0, const int i_iPort=21);  

  int FTPCmd(FTPCMDTYPE cmd, const char* cmdvalue=0, char *returnvalue=0, int retmaxsize = 0);

  int GetFile(const char* strRemote, const char* strLocal = 0);

  int PutFile(const char* strLocal, const char* strRemote,int *iPct); 

  void FTPLogout();

  int FTPOption(FTPOPTION option, OPTVAL value);

    
  char   m_strReplyBuffer[10240];  //////��������ȡ����

    
private:

  int    m_hControlChannel; 

  int    m_hListenSocket;      

  int    m_hDataSocket;



  /* �����ֽ��� */

  int   m_IsBigOrder;

  /* ��ʱ���� */

  OPTVAL  m_iTimeOut;

  OPTVAL  m_iResume;

  OPTVAL  m_iMaxWaitTime;

  OPTVAL  m_iPassive;

private:

  int   IsBigOrder();

  int CreateListenSocket();

  int WaitResult(int fd);

  int FTPCommand(const char* strCommand);

  int Accept(int fd,struct sockaddr* addr,int* len);

  int Write(int fd,char* buff,int *len);

  int Read(int fd,char* buff,int *len);

  int GetLine( char* src, char** nextline);

};


#endif /* __CFTP_H_CLIENT */

