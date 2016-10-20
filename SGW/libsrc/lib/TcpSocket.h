/* Copyright (c) 2004-<2006> Linkage, All rights Reserved */
/* #ident       "@(#)TcpSocket.h     1.0     2004/11/07" */
#ifndef TcpSocket_H_HEADER_INCLUDED_BE6E2CF5
#define TcpSocket_H_HEADER_INCLUDED_BE6E2CF5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <string>
using namespace std;

//##ModelId=4314101B03E3
//##Documentation
//## TCP/IP TcpSocket��װ
class TcpSocket
{
  public:
    //##ModelId=4314101E02A7
    //##Documentation
    //## ����������(�����ʹ��)
    int listen(
        //##Documentation
        //## �˿ں�
        int iPortID,
        //##Documentation
        //## ����������Ӻ��Ƿ�fork����.
        bool bForkFlag = false);

    //## ����������(�����ʹ��)
    //���� struct sockaddr_in NewAddr ���ؿͻ���IP
    int listen(
        //##Documentation
        //## �˿ں�
        int iPortID,
        //##Documentation
        //## ����������Ӻ��Ƿ�fork����.
        struct sockaddr_in& NewAddr,
        bool bForkFlag = false);
    //##ModelId=4314101E02AA
    //##Documentation
    //## ����TcpSocket(�ͻ���ʹ��)
    int connect(
        //##Documentation
        //## Զ�̷�����IP
        char *sRemoteHostIP,
        //##Documentation
        //## Զ�̷������˿�
        int iRemotePortID);

    int setBlock();

    //##ModelId=4314101E02AD
    TcpSocket();

    //##ModelId=4314101E02AE
    virtual ~TcpSocket();

    //##ModelId=432E7B0E019F
    int read(
        //##Documentation
        //## ��ȡTcpSocket��ŵĻ���
        unsigned char *sBuffer,
        //##Documentation
        //## ָ����ȡ����
        int iLen);


    //##ModelId=432E7B5303C5
    int write(
        //##Documentation
        //## д��TcpSocket�����ݵ�ַ
        unsigned char *sBuffer,
        //##Documentation
        //## д��TcpSocket����
        int iLen);

    int	Sendn(char *buf, int len, int timeout_val=10);
    int recv(void *buf, int len, int flags);
	int recv_ex(void *sBuffer, int len, int flags,struct timeval sTimeOut);

    int	CloseListenSocket();
    void	Close(int iShutDown=false);

    //## TcpSocketID
    int m_iListenSocket;
    int m_iTcpSocketID;

		int send(unsigned char *sBuffer, int iLen);

		char * getClientAddr(char *sClientAddr,int iLen);
		bool CanRead(int time_out = 1);
		bool CanWrite(int time_out = 1);		

  private:
    //##ModelId=4314101E029F
    //##Documentation
    //## TcpSocketID
//    int m_iTcpSocketID;
};


#endif /* TcpSocket_H_HEADER_INCLUDED_BE6E2CF5 */
