/*VER: 1*/ 
/* Copyright (c) 2004-<2006> Linkage, All rights Reserved */
/* #ident       "@(#)Socket.h     1.0     2004/11/07" */
#ifndef SOCKET_H_HEADER_INCLUDED_BE6E2CF5
#define SOCKET_H_HEADER_INCLUDED_BE6E2CF5

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

//##ModelId=41DB83F60012
//##Documentation
//## TCP/IP Socket��װ
class Socket
{
  public:
    //##ModelId=41DCB45A02BF
    //##Documentation
    //## ����������(�����ʹ��)
    int listen(
        //##Documentation
        //## �˿ں�
        int iPortID, 
        //##Documentation
        //## ����������Ӻ��Ƿ�fork����.
        bool bForkFlag = false);

    //##ModelId=41DCB45A0338
    //##Documentation
    //## ����socket(�ͻ���ʹ��)
    int connect(
        //##Documentation
        //## Զ�̷�����IP
        char *sRemoteHostIP, 
        //##Documentation
        //## Զ�̷������˿�
        int iRemotePortID,
		char *sLocalHostIP = NULL,
		int iLocalPort = 0);

    //##ModelId=41DCB45A037E
    Socket();


    //##ModelId=41DCB45A0388
    virtual ~Socket();

    //##ModelId=41DCB45A039C
    //##Documentation
    //## ��socket
    int read(
        //##Documentation
        //## ��ȡsocket��ŵĻ���
        unsigned char *sBuffer, 
        //##Documentation
        //## ָ����ȡ����
        int iLen);

    //##ModelId=41DCB45B0004
    //##Documentation
    //## дsocket
    int write(
        //##Documentation
        //## д��socket�����ݵ�ַ
        unsigned char *sBuffer, 
        //##Documentation
        //## д��socket����
        int iLen);

	// add by zhangch
	int closeSocket();
	
	// add by zhangch
	int selectRead(unsigned char *sBuffer, int iLen, int iSec);

  private:
    //##ModelId=41DCB45A028D
    //##Documentation
    //## SocketID
    int m_iSocketID;

};



#endif /* SOCKET_H_HEADER_INCLUDED_BE6E2CF5 */
