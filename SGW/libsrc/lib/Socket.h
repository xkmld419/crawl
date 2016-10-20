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
#include <string>
#include "platform.h"

//##ModelId=4314101B03E3
//##Documentation
//## TCP/IP Socket��װ
class Socket
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

    //##ModelId=4314101E02AA
    //##Documentation
    //## ����socket(�ͻ���ʹ��)
    int connect(
        //##Documentation
        //## Զ�̷�����IP
        char *sRemoteHostIP,
        //##Documentation
        //## Զ�̷������˿�
        int iRemotePortID);

    int setBlock();

    //##ModelId=4314101E02AD
    Socket();

    //##ModelId=4314101E02AE
    virtual ~Socket();

    //##ModelId=4314101E02B0
    //##Documentation
    //## ��socket
    int read(
        //##Documentation
        //## ���Ļ�����
        string &str
     	);

    //##ModelId=4314101E02B3
    //##Documentation
    //## дsocket
    int write(
        //##Documentation
        //## д�Ļ�����
        string &str
        );
    //##ModelId=432E7B0E019F
    int read(
        //##Documentation
        //## ��ȡsocket��ŵĻ���
        unsigned char *sBuffer,
        //##Documentation
        //## ָ����ȡ����
        int iLen);


    //##ModelId=432E7B5303C5
    int write(
        //##Documentation
        //## д��socket�����ݵ�ַ
        unsigned char *sBuffer,
        //##Documentation
        //## д��socket����
        int iLen);

  private:
    //##ModelId=4314101E029F
    //##Documentation
    //## SocketID
    int m_iSocketID;
};


#endif /* SOCKET_H_HEADER_INCLUDED_BE6E2CF5 */


