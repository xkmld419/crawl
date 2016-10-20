/* Copyright (c) 2004-<2006> Linkage, All rights Reserved */
/* #ident       "@(#)TcpSocket.cpp     1.0     2004/11/07" */
#include "TcpSocket.h"

using namespace std;

//##ModelId=4314101E02A7
int TcpSocket::listen(int iPortID, bool bForkFlag)
{
	int optval=1;
	int pid;
	struct sockaddr_in ServerAddr,NewAddr;
	int iTcpSocketId, iNewTcpSocketId, iNewLen=sizeof(struct sockaddr);

	memset((char *)&ServerAddr,0,sizeof(ServerAddr));
	ServerAddr.sin_family      = AF_INET;
	ServerAddr.sin_addr.s_addr = htonl( INADDR_ANY);
//	ServerAddr.sin_addr.s_addr = inet_addr("192.168.191.12");
	ServerAddr.sin_port        = htons( iPortID );

	while((iTcpSocketId=socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		//ENOSR---��Դ����
		//ENOMEM---�ڴ治��
		if ( errno==ENOSR||errno==ENOMEM )
		{	sleep(1);
			continue;
		}
		printf("TcpSocket create error\n ");
		return -1;
	}
	m_iListenSocket = iTcpSocketId;
/*
	1������һ������ͬ���ص�ַ�Ͷ˿ڵ�TcpSocket1����TIME_WAIT״̬ʱ��������
���ĳ����TcpSocket2Ҫռ�øõ�ַ�Ͷ˿ڣ���ĳ����Ҫ�õ���ѡ�
    2��SO_REUSEADDR����ͬһport������ͬһ�������Ķ��ʵ��(�������)����
ÿ��ʵ���󶨵�IP��ַ�ǲ�����ͬ�ġ����ж����������IP Alias�����Ļ�����
�Բ������������
    3��SO_REUSEADDR���������̰���ͬ�Ķ˿ڵ����TcpSocket�ϣ���ÿ��soc
ket�󶨵�ip��ַ��ͬ�����2�����ƣ������뿴UNPv1��
    4��SO_REUSEADDR������ȫ��ͬ�ĵ�ַ�Ͷ˿ڵ��ظ��󶨡�����ֻ����UDP��
�ಥ��������TCP��
*/
	if(setsockopt(iTcpSocketId,SOL_SOCKET,SO_REUSEADDR,(char *)&optval,sizeof(optval)) != 0)
	{
		printf("setsockopt SO_REUSEADDR error\n");
		return -1;
	}

	optval = 1;
	if(setsockopt(iTcpSocketId,SOL_SOCKET,SO_LINGER,(char *)&optval,sizeof(optval)) != 0)
	{
		printf("setsockopt SO_LINGER error\n");
		return -1;
	}
        for(;;)
        {
	  if( ::bind(iTcpSocketId,(struct sockaddr*)&ServerAddr,iNewLen)<0)
	  {	printf("bind error:[%s]\n", strerror(errno));
                sleep(1);
		continue; //return -2;
	  }
          break;
        }
	if( ::listen(iTcpSocketId,5)<0 )
	{	printf("listen error:[%s]\n", strerror(errno));
		return -1;
	}
	printf ("[Message]: Listen at Port: %d\n", iPortID);

	while (1)
	{
		for(;;)
		{
			if((iNewTcpSocketId=
#ifdef DEF_HP
			accept(iTcpSocketId,(struct sockaddr*)&NewAddr,&iNewLen)) <0 )
#else
			accept(iTcpSocketId,(struct sockaddr*)&NewAddr,(socklen_t *)&iNewLen)) <0 )
#endif
			{
				printf("accept eror :%s\n",strerror(errno));
				sleep(1);
				continue;
			}
			else
			{
				m_iTcpSocketID = iNewTcpSocketId;
				break;
			}
		}
		if (bForkFlag == false)
		{
			close (iTcpSocketId);
			return iNewTcpSocketId;
		}
		else
		{
#ifndef DEF_LINUX
			sigset (SIGCHLD, SIG_IGN);
#else
			signal (SIGCHLD, SIG_IGN);
#endif
			if ((pid = fork())==0)
			{
				close (iTcpSocketId);
				return iNewTcpSocketId;
			}
			else
			{
				printf ("fork process: pid=%d\n",pid);
				close (iNewTcpSocketId);
				continue;
			}
		}
	}
}

int	TcpSocket::CloseListenSocket()
{
	::close(m_iListenSocket);
	return 1;
}

void	TcpSocket::Close(int iShutDown)
{
	if ( iShutDown )
	{
		::shutdown(m_iTcpSocketID,SHUT_RDWR);
	}
	::close(m_iTcpSocketID);
}

//##ModelId=4314101E02AA
int TcpSocket::connect(char *sRemoteHostIP, int iRemotePortID)
{
	struct sockaddr_in ServerAddr;
	struct linger mylinger;
	int iTcpSocketId,iResult,optval=1;
	ServerAddr.sin_family         = AF_INET;
	ServerAddr.sin_addr.s_addr    = inet_addr( sRemoteHostIP );
	ServerAddr.sin_port           = htons( iRemotePortID );

	if((iTcpSocketId=socket(AF_INET,SOCK_STREAM,0))<0)
	{	printf("TcpSocket error\n");
		return -1;
	}
	printf("connect [%s:%d]\n", sRemoteHostIP, iRemotePortID);
	while( 1 )
	{
		mylinger.l_onoff  = 1;
		mylinger.l_linger = 0;

		setsockopt(iTcpSocketId, SOL_SOCKET, SO_LINGER,
			(char *) &mylinger,sizeof(struct linger));

#ifndef DEF_LINUX
		sigset(SIGPIPE,SIG_IGN);
#else
		signal(SIGPIPE,SIG_IGN);
#endif

		optval = 1;
		setsockopt(iTcpSocketId, SOL_SOCKET, SO_KEEPALIVE,
			(char *) &optval,sizeof(int));

		iResult = /*std*/::connect(iTcpSocketId,(struct sockaddr*)&ServerAddr,
				sizeof(ServerAddr));
		if( iResult != 0 )
		{
			printf("connect error:%s",strerror(errno));
			shutdown( iTcpSocketId,2 );
			close( iTcpSocketId );
			/*edit by lingy on 2011-09-15
			if( errno == ECONNREFUSED )
			{	sleep(5);
				continue;
			}*/
			printf("Connect error\n");
			return -1;
		}
		else break;
	}
	m_iTcpSocketID = iTcpSocketId;
	
	return iTcpSocketId;
}

//���÷�������ʽ
int TcpSocket::setBlock()
{
	int optval = 1;
	fcntl(m_iTcpSocketID , F_GETFL , &optval);
//#ifdef DEF_AIX
//	fcntl(m_iTcpSocketID , F_SETFL , optval|O_NDELAY);
//#else
 	fcntl(m_iTcpSocketID , F_SETFL , optval|O_NONBLOCK);
//#endif
 	return 0;
}

//##ModelId=4314101E02AD
TcpSocket::TcpSocket()
{
	m_iTcpSocketID = -1;
}

//##ModelId=4314101E02AE
TcpSocket::~TcpSocket()
{
	m_iTcpSocketID = -1;
}


/*���ͳ���Ϊlen	byte��һ������
  ���أ�>=0 -- ���͵��ֽ���
        -1  -- ʧ��, ���г�ʱʱ��timeout_val��λ����
        ��������������ww_error()���� */
int	TcpSocket::Sendn(char *send_buf, int len, int timeout_val)
{
	time_t	start_tm , curr_tm;
	int	n, nbytes;
//  char sLen[14];



	time(&start_tm) ;
	nbytes = len;
//  printf("write TcpSocket len:[%d]\n", len);

	while( len > 0 ) {
		if( (n = ::write(m_iTcpSocketID, send_buf, len)) <= 0 )
		{
			if( errno != EWOULDBLOCK) {
				printf("write TcpSocket errno:[%d], err=%s\n", errno, strerror(errno));
				return -1;
			}
//		  printf("1.write %d,err=%s\n", n, strerror(errno));
		}
		else {
			len -= n;
			send_buf += n;
		}

    if( timeout_val > 0 )
    {
		  time(&curr_tm);
		  if( difftime(curr_tm , start_tm) > timeout_val ) {
			  printf("write TcpSocket timeout:[%d],nbytes-len:[%d]\n", len, nbytes-len);
			  return -1;
		  }
		}
//	  printf("2.write %d\n", n);
	}
//  printf("write TcpSocket len:[%d]\n", len);
	return ( nbytes - len );
}


//##ModelId=432E7B0E019F
int TcpSocket::read(unsigned char *sBuffer, int iLen)
{
	return ::read(m_iTcpSocketID, sBuffer, iLen);
}

//##ModelId=432E7B0E019F
int TcpSocket::recv(void *sBuffer, int len, int flags)
{
    return ::recv(m_iTcpSocketID, sBuffer, len,flags);
}

int TcpSocket::recv_ex(void *sBuffer, int len, int flags,struct timeval sTimeOut)
{
	setsockopt(m_iTcpSocketID,SOL_SOCKET,SO_RCVTIMEO,(const char *)&sTimeOut,sizeof(int));
	return ::recv(m_iTcpSocketID, sBuffer, len,flags);
}

//##ModelId=432E7B5303C5
int TcpSocket::write(unsigned char *sBuffer, int iLen)
{
	return ::write(m_iTcpSocketID, sBuffer, iLen);
}

int TcpSocket::send(unsigned char *sBuffer, int iLen)
{
	return ::send(m_iTcpSocketID, sBuffer, iLen,0);
}


int TcpSocket::listen(int iPortID, struct sockaddr_in& NewAddr,bool bForkFlag)
{
	int optval=1;
	int pid;
	struct sockaddr_in ServerAddr;
	int iTcpSocketId, iNewTcpSocketId, iNewLen=sizeof(struct sockaddr);

	memset((char *)&ServerAddr,0,sizeof(ServerAddr));
	ServerAddr.sin_family      = AF_INET;
	ServerAddr.sin_addr.s_addr = htonl( INADDR_ANY);
//	ServerAddr.sin_addr.s_addr = inet_addr("192.168.191.12");
	ServerAddr.sin_port        = htons( iPortID );

	while((iTcpSocketId=socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		//ENOSR---��Դ����
		//ENOMEM---�ڴ治��
		if ( errno==ENOSR||errno==ENOMEM )
		{	sleep(1);
			continue;
		}
		printf("TcpSocket create error\n ");
		return -1;
	}
	m_iListenSocket = iTcpSocketId;
/*
	1������һ������ͬ���ص�ַ�Ͷ˿ڵ�TcpSocket1����TIME_WAIT״̬ʱ��������
���ĳ����TcpSocket2Ҫռ�øõ�ַ�Ͷ˿ڣ���ĳ����Ҫ�õ���ѡ�
    2��SO_REUSEADDR����ͬһport������ͬһ�������Ķ��ʵ��(�������)����
ÿ��ʵ���󶨵�IP��ַ�ǲ�����ͬ�ġ����ж����������IP Alias�����Ļ�����
�Բ������������
    3��SO_REUSEADDR���������̰���ͬ�Ķ˿ڵ����TcpSocket�ϣ���ÿ��soc
ket�󶨵�ip��ַ��ͬ�����2�����ƣ������뿴UNPv1��
    4��SO_REUSEADDR������ȫ��ͬ�ĵ�ַ�Ͷ˿ڵ��ظ��󶨡�����ֻ����UDP��
�ಥ��������TCP��
*/
	if(setsockopt(iTcpSocketId,SOL_SOCKET,SO_REUSEADDR,
			(char *)&optval,sizeof(optval)) != 0)
	{
		printf("setsockopt SO_REUSEADDR error\n");
		return -1;
	}
        for(;;)
        {
	  if( ::bind(iTcpSocketId,(struct sockaddr*)&ServerAddr,iNewLen)<0)
	  {	printf("bind error:[%s]\n", strerror(errno));
                sleep(1);
		continue; //return -2;
	  }
          break;
        }
	if( ::listen(iTcpSocketId,5)<0 )
	{	printf("listen error:[%s]\n", strerror(errno));
		return -1;
	}
	printf ("[Message]: Listen at Port: %d\n", iPortID);

	while (1)
	{
		for(;;)
		{
			if((iNewTcpSocketId=
#ifdef DEF_HP
			accept(iTcpSocketId,(struct sockaddr*)&NewAddr,&iNewLen)) <0 )
#else
			accept(iTcpSocketId,(struct sockaddr*)&NewAddr,(socklen_t *)&iNewLen)) <0 )
#endif
			{
				printf("accept eror :%s\n",strerror(errno));
				sleep(1);
				continue;
			}
			else
			{
				m_iTcpSocketID = iNewTcpSocketId;
				break;
			}
		}
		if (bForkFlag == false)
		{
			close (iTcpSocketId);
			return iNewTcpSocketId;
		}
		else
		{
#ifndef DEF_LINUX
			sigset (SIGCHLD, SIG_IGN);
#else
			signal (SIGCHLD, SIG_IGN);
#endif
			if ((pid = fork())==0)
			{
				close (iTcpSocketId);
				return iNewTcpSocketId;
			}
			else
			{
				printf ("fork process: pid=%d\n",pid);
				close (iNewTcpSocketId);
				continue;
			}
		}
	}
}

char * TcpSocket::getClientAddr(char *sClientAddr,int iLen)
{
  sClientAddr[0]=0;
  struct sockaddr_in ClientAddr;
  int Addr_Len = sizeof(struct sockaddr_in);
#ifdef DEF_HP
  if (getpeername(m_iTcpSocketID,(void *)&ClientAddr,&Addr_Len)==0)
#else
  if (getpeername(m_iTcpSocketID,(struct sockaddr *)&ClientAddr,(socklen_t *)&Addr_Len)==0)
#endif
    snprintf(sClientAddr,iLen,"%s:%d",inet_ntoa(ClientAddr.sin_addr),ntohs(ClientAddr.sin_port));
  else
    snprintf(sClientAddr,iLen,"%s","0.0.0.0");
  return sClientAddr;
}

bool TcpSocket::CanRead(int time_out)
{
	fd_set fd_read;
	struct timeval time_value;

	if (m_iTcpSocketID == -1) return false;

	if (time_out <= 0) time_out = 3;

	time_value.tv_sec = time_out;
	time_value.tv_usec = 0;

	FD_ZERO(&fd_read);
	FD_SET(m_iTcpSocketID, &fd_read);
	if (select(m_iTcpSocketID + 1, &fd_read, NULL, NULL, &time_value) <= 0)
	{
		// ���Զ�errno == EINTR�Ĵ���,��Ӧ����Ҫ������ص��ź�
		return false;
	}
	if (!FD_ISSET(m_iTcpSocketID, &fd_read)) return false;
	return true;
}


bool TcpSocket::CanWrite(int time_out)
{
	fd_set fd_write;
	struct timeval time_value;

	if (m_iTcpSocketID == -1) return false;

	if (time_out <= 0) time_out = 3;

	time_value.tv_sec = time_out;
	time_value.tv_usec = 0;

	FD_ZERO(&fd_write);
	FD_SET(m_iTcpSocketID, &fd_write);
	
	if (select(m_iTcpSocketID + 1, NULL, &fd_write, NULL, &time_value) <= 0)
	{
		// ���Զ�errno == EINTR�Ĵ���,��Ӧ����Ҫ������ص��ź�
		return false;
	}

	if (!FD_ISSET(m_iTcpSocketID, &fd_write)) return false;
	return true;
}		
