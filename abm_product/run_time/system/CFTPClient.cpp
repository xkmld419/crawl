/*VER: 1*/ 
#include "CFTPClient.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <iostream>

#ifndef INADDR_NONE
    #define INADDR_NONE ( in_addr_t ) 0xffffffff
#endif

#ifdef DEF_LINUX
  #ifndef __linux__
    #define __linux__
  #endif
#endif

#ifdef DEF_AIX
  #ifndef _AIX
    #define _AIX
  #endif
#endif


CFTPClient::CFTPClient ( void )
{
    /* Ĭ�ϳ�ʱ��Ϊ600�� */
    m_iTimeOut = ( OPTVAL ) 1800;
    m_iMaxWaitTime = ( OPTVAL ) 100000;

    /* Ĭ��Ϊ��֧�ֶϵ����� */
    m_iResume = NO;

    /* ��ȡ�����ֽ��� */
    m_IsBigOrder = IsBigOrder();

    m_iPassive=0;
}

/********************************************************
* function:FTPOption
* purpose:����ftpѡ��
* return:
*********************************************************/
int CFTPClient::FTPOption ( FTPOPTION   option,
                            OPTVAL      value )
{
    switch ( option )
    {
        /*��ʱ����*/
        case TIMEOUT:
            m_iTimeOut = value;
            break;
        case MAXWAITTIME:
            m_iMaxWaitTime = value;
            break;

        /*�ļ�����������������*/
        case DATATYPE:
            /* ���ô����������� */
            if ( value == BINARY )
            {
                return FTPCommand( "TYPE I\r\n" );
            }
            else if ( value == ASCII )
            {
                return FTPCommand( "TYPE A\r\n" );
            }
            else
            {
                printf( " DATATYPE does not exist\n" );
                return -1;
            }

        /*�ϵ���������*/
        case RESUME:
            m_iResume = value;
            break;

        /*����ģʽ*/
        case PASSIVE:
            m_iPassive= value;
            break;
            
        default:
            printf( " option does not exist\n" );
            return -1;
    }

    return 0;
}

CFTPClient::~CFTPClient ( void )
{
}

/********************************************************
* function:FTPLogIn
* purpose:��¼��FTP Server
* return:   FTP_SUCCEED �ɹ� ��        FTP_FAIL ʧ��
*
*********************************************************/
int CFTPClient::FTPLogin ( const char*     strServer,
                           const char*     strUser,
                           const char*     strPassword,
                           const int       i_iPort )
{
    struct hostent*     lpHostEnt;  /* Internet host information structure*/
    struct sockaddr_in  sockAddr;   /* Socket address structure  */
    unsigned long       lFtpAddr;

    sockAddr.sin_family = AF_INET;

    ////sockAddr.sin_port = htons(21);
    if ( i_iPort > 0 )
    {
        sockAddr.sin_port = htons( i_iPort );
    }
    else
    {
        sockAddr.sin_port = htons( 21 );
    }

    /* ����FTP server Internet ��ַ��
	���ȣ��ж��Ƿ��� ��ʽ  "133.96.168.1"
	�����ж��Ƿ���������  �磺"sjz1" */
    lFtpAddr = inet_addr( strServer );
    if ( lFtpAddr == INADDR_NONE )
    {
        if ( !( lpHostEnt = gethostbyname( strServer ) ) )
        {
            perror( "gethostbyname" );
            return -1;
        }

        sockAddr.sin_addr = *( (struct in_addr*) *lpHostEnt->h_addr_list );
    }
    else
    {   //sockAddr.sin_addr=*((struct in_addr *)&lFtpAddr);
        sockAddr.sin_addr.s_addr = lFtpAddr;

    }

    if ( ( m_hControlChannel = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        perror( "socket() create " );
        return -1;
    }

    if (
        connect
            (
                m_hControlChannel,
                reinterpret_cast < struct sockaddr * > (&sockAddr),
                sizeof(sockAddr)
            ) < 0 )
    {
        close( m_hControlChannel );
        perror( "connect()" );
        return FTP_E_CONNECT;
    }

    int ret = WaitResult( m_hControlChannel );
    if ( ret != 220 )
    {
        close( m_hControlChannel );
        return FTP_E_CONNECT;
    }

    /* ����user���� */
    char    buf[256];
    snprintf( buf, sizeof( buf ), (char*)"USER %s\r\n", strUser );

    int len = strlen( buf );
    ret = Write( m_hControlChannel, buf, &len );
    if ( ret < 0 || len != strlen( buf ) )
    {
        close( m_hControlChannel );
        return FTP_E_CONNECT;
    }

    ret = WaitResult( m_hControlChannel );
    if ( ret != 331 )
    {
        close( m_hControlChannel );
        return FTP_E_USER;
    }

    /*����pass����*/
    snprintf( buf, sizeof( buf ), (char*)"PASS %s\r\n", strPassword );
    len = strlen( buf );
    ret = Write( m_hControlChannel, buf, &len );
    if ( ret < 0 || len != strlen( buf ) )
    {
        close( m_hControlChannel );
        return FTP_E_CONNECT;
    }

    ret = WaitResult( m_hControlChannel );
    if ( ret != 230 )
    {
        close( m_hControlChannel );
        return FTP_E_PASS;
    }

    return 0;
}

/********************************************************
* function:CreateListenSocket
* purpose:create listen socket (let it choose the port) & start the socket listening
* return:   FTP_SUCCEED �ɹ� ��        FTP_FAIL ʧ��
*
********************************************************/
int CFTPClient::CreateListenSocket ( void )
{
    struct sockaddr_in  sockAddr;

    /* �����׽ӿ� */
    if ( ( m_hListenSocket = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        perror( "socket()" );
        return -1;
    }


    if( m_iPassive ==1 )
    { 
        //����ģʽ  
        char sRemote[50];
        if(FTPCmd(PASV, 0, sRemote, 50)!=0 ) return -1;
        char *pTmp;
        for(int i=0; i<3; i++)
        {
            //pTmp=strchr(sRemote, ',' );
	    pTmp = strchr(sRemote, ',');
            if(pTmp==NULL)return -1;
            *pTmp='.';
        }
        pTmp=strchr(sRemote, ',' );
        if(pTmp==NULL)return -1;
        *pTmp=0;
        unsigned int iPortTmp=atoi(++pTmp);
        pTmp=strchr(pTmp, ',' );
        if(pTmp==NULL)return -1;
        iPortTmp=iPortTmp*256+atoi(++pTmp);
        unsigned long lRemote = inet_addr( sRemote );

        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons( iPortTmp ); 
        sockAddr.sin_addr.s_addr  = lRemote;

        if (
            connect
                (
                    m_hListenSocket,
                    reinterpret_cast < struct sockaddr * > (&sockAddr),
                    sizeof(sockAddr)
                ) < 0 )
        {
            close( m_hListenSocket );
            perror( "connect()" );
            return -1;
        }

        return 0;
        

    }

    
    /* Let the system assign a socket address                */
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons( 0 ); /* htons() is just a reminder.*/
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    
    /* Bind the socket       */
    if (
        bind
            (
                m_hListenSocket,
                reinterpret_cast < struct sockaddr * > (&sockAddr),
                sizeof(sockAddr)
            ) )
    {
        close( m_hListenSocket );
        perror( "bind()" );
        return -1;
    }

    int tmpint = fcntl( m_hListenSocket, F_GETFL, 0 );

    if ( fcntl( m_hListenSocket, F_SETFL, tmpint | O_NONBLOCK ) < 0 )
    {
        close( m_hListenSocket );
        perror( "fcntl" );
        return -1;
    }

    /* Listen for the FTP server connection */
    if ( listen( m_hListenSocket, 3 ) )
    {
        close( m_hListenSocket );
        perror( "listen()" );
        return -1;
    }

    /* Ask the server to connect to the port monitored by the listener socket*/
    #if defined( _AIX ) || defined( __linux__ )

    socklen_t   iLength = sizeof( sockAddr );
    #else
    int         iLength = sizeof( sockAddr );
    #endif

    /* ����PORT���� */
    /* Get port number */
    if (
        getsockname
            (
                m_hListenSocket,
                reinterpret_cast < struct sockaddr * > (&sockAddr),
                &iLength
            ) < 0 )
    {
        close( m_hListenSocket );
        printf( "getsockname" );
        return -1;
    }

    int iPort = sockAddr.sin_port;

    /* Get local ip address  */
    if (
        getsockname
            (
                m_hControlChannel,
                reinterpret_cast < struct sockaddr * > (&sockAddr),
                &iLength
            ) < 0 )
    {
        close( m_hListenSocket );
        printf( "getsockname" );
        return -1;
    }

    char    strCmd[128];

    //PORT h1,h2,h3,h4,p1,p2
    //where h1 is the high order 8 bits of the internet host  address.
    if ( m_IsBigOrder == 1 )
    {
        sprintf
        (
            strCmd,
            "PORT %d,%d,%d,%d,%d,%d\r\n",
            ( sockAddr.sin_addr.s_addr >> 24 ) & 0x000000ff,
            ( sockAddr.sin_addr.s_addr >> 16 ) & 0x000000ff,
            ( sockAddr.sin_addr.s_addr >> 8 ) & 0x000000ff,
            ( sockAddr.sin_addr.s_addr ) & 0x000000ff,
            iPort >> 8,
            iPort & 0xFF
        );
    }
    else
    {
        sprintf
        (
            strCmd,
            "PORT %d,%d,%d,%d,%d,%d\r\n",
            ( sockAddr.sin_addr.s_addr ) & 0x000000ff,
            ( sockAddr.sin_addr.s_addr >> 8 ) & 0x000000ff,
            ( sockAddr.sin_addr.s_addr >> 16 ) & 0x000000ff,
            ( sockAddr.sin_addr.s_addr >> 24 ) & 0x000000ff,
            iPort & 0xFF,
            iPort >> 8
        );

    }

    int len = strlen( strCmd );
    int ret = Write( m_hControlChannel, strCmd, &len );
    if ( ret < 0 || len != strlen( strCmd ) )
    {
        close( m_hControlChannel );
        return -1;
    }

    /* here we should get the result: 200 PORT command successful. */
    ret = WaitResult( m_hControlChannel );
    if ( ret != 200 )
    {
        close( m_hControlChannel );
        return -1;
    }

    return 0;
}

/*
 *get a file from remote ftp server
 *return 0 -- get successfully
 *return -1 -- some error !
 *return -2 -- remote file doesn't exist
 */
int CFTPClient::GetFile ( const char*     strRemote,
                          const char*     strLocal )
{
    if ( CreateListenSocket() < 0 )
    {
        return -1;
    }

    const char*     cpLocalFile = ( strLocal == NULL ? strRemote : strLocal );
    char            buf[10240];

    if ( m_iResume == YES )
    {
        struct stat filestat;
        if ( stat( cpLocalFile, &filestat ) < 0 )
        {
            filestat.st_size = 0;
        }

        snprintf( buf, sizeof( buf ), (char*)"REST %ld\r\n", filestat.st_size );

        int len = strlen( buf );
        int ret = Write( m_hControlChannel, buf, &len );
        if ( ret < 0 || len != strlen( buf ) )
        {
            close( m_hListenSocket );
            return -1;
        }

        // ���������֧�ֶϵ���������ô����Ӧ��Ӧ�ǣ�
        // 350 Restarting at 0. Send STORE or RETRIEVE to initiate transfer.
        ret = WaitResult( m_hControlChannel );
        if ( ret < 0 )
        {
            close( m_hListenSocket );
            return -1;
        }

        if ( ret / 100 != 3 )
        {
            m_iResume = NO;
            printf( "FTP server does not support rest command\n" );
        }
    }

    snprintf( buf, sizeof( buf ), (char*)"RETR %s\r\n", strRemote );

    int len = strlen( buf );
    int ret = Write( m_hControlChannel, buf, &len );
    if ( ret < 0 || len != strlen( buf ) )
    {
        close( m_hListenSocket );
        return -1;
    }

    ret = Accept( m_hListenSocket, NULL, NULL );
    if(m_hListenSocket>0) close( m_hListenSocket );
    if ( ret > 0 )
    {
        m_hDataSocket = ret;
    }
    else
    {
        ret = WaitResult( m_hControlChannel );
        if ( ret == 550 )
        {
            return -2;
        }

        return -1;
    }

    ret = WaitResult( m_hControlChannel );
    if ( ret != 150 )
    {
        close( m_hDataSocket );
        return -1;
    }

    int fd;
    if (
        (
            fd = open
                (
                    cpLocalFile,
                    m_iResume == YES ? O_CREAT |
                        O_APPEND |
                        O_RDWR : O_CREAT |
                        O_TRUNC |
                        O_RDWR,
                    0660
                )
        ) < 0 )
    {
        close( m_hDataSocket );
        perror( "open file" );
        return -1;
    }

    int timer = time( 0 );
    int totallen = 0;
    int tmptime;
    while ( 1 )
    {
        tmptime = time( 0 );
        if ( tmptime >= timer + m_iMaxWaitTime )
        {
            close( m_hDataSocket );
            printf
            (
                "time out %d seconds when get a remote file\n",
                m_iMaxWaitTime
            );
            close( fd );
            return -1;
        }

        len = 10240;
        ret = Read( m_hDataSocket, buf, &len );
        if ( ret != 0 )
        {
            break;
        }
        else if ( len == 0 )
        {
            continue;

        }

        timer = tmptime;
        if ( write( fd, buf, len ) < len )
        {
            perror( "write file" );
            close( fd );
            close( m_hDataSocket );
            return -1;
        }

        totallen += len;
    }

    close( fd );
    close( m_hDataSocket );
    ret = WaitResult( m_hControlChannel );
    if ( ret == 226 )
    {
        return 0;
    }

    return -1;
}

int CFTPClient::PutFile ( const char*     strLocal,
                          const char*     strRemote,
                          int*            iPct )
{
    if ( CreateListenSocket() < 0 )
    {
        printf( "CreateListenSocket error!\n" );
        return -1;
    }

    struct stat     filestat;

    const char*     cpRemoteFile = ( strRemote == NULL ? strLocal : strRemote );

    char            buf[10240];

    //�ϵ�����֧�� add 2004-01-05
    if ( m_iResume == YES )
    {

        if ( FTPCmd( SIZE, strRemote, buf ) == 0 )
        {
            int iSize = atoi( buf );
            snprintf( buf, sizeof( buf ), (char*)"REST %d\r\n", iSize );

            int len = strlen( buf );
            int ret = Write( m_hControlChannel, buf, &len );
            if ( ret < 0 || len != strlen( buf ) )
            {
                close( m_hListenSocket );
                printf( "m_hListenSocket1 error!\n" );
                return -1;
            }

            // ���������֧�ֶϵ���������ô����Ӧ��Ӧ�ǣ�
            // 350 Restarting at 0. Send STORE or RETRIEVE to initiate transfer.
            ret = WaitResult( m_hControlChannel );
            if ( ret < 0 )
            {
                close( m_hListenSocket );
                printf( "m_hListenSocket2 error!\n" );
                return -1;
            }

            if ( ret / 100 != 3 )
            {
                m_iResume = NO;
                printf( "FTP server does not support rest command\n" );
            }
        }
    }

    sprintf( buf, "STOR %s\r\n", cpRemoteFile );

    int len = strlen( buf );
    int ret = write( m_hControlChannel, buf, len );
    if ( ret < 0 || len != strlen( buf ) )
    {
        close( m_hListenSocket );
        return -1;
    }

    ret = Accept( m_hListenSocket, NULL, NULL );
    if(m_hListenSocket>0) close( m_hListenSocket );
    if ( ret > 0 )
    {
        m_hDataSocket = ret;
    }
    else
    {
        ret = WaitResult( m_hControlChannel );
        if ( ret == 550 )
        {
            return -2;
        }

        return -1;
    }

    ret = WaitResult( m_hControlChannel );
    if ( ret != 150 )
    {
        close( m_hDataSocket );
        return -1;
    }

    int fd;
    if ( ( fd = open( strLocal, O_RDONLY, 0660 ) ) < 0 )
    {
        close( m_hDataSocket );
        perror( "open file" );
        return -1;
    }

    if ( fstat( fd, &filestat ) < 0 )
    {
        printf( "File:%s stat error!\n", strLocal );
        close( m_hDataSocket );
        close( fd );
        return -1;
    }

    int totallen = filestat.st_size;
    int iSize = totallen;
    int timer = time( 0 );
    ret = 0;

    int tmplen, tmptime;
    while ( totallen > 0 )
    {
        //���ô������
        *iPct = ( ( (float)(iSize - totallen) / iSize ) * 100 );

        tmptime = time( 0 );
        if ( tmptime >= timer + m_iMaxWaitTime )
        {
            printf( "time out %d seconds in putfile\n", m_iMaxWaitTime );
            ret = -1;
            break;
        }

        if ( totallen < 256 )
        {
            len = totallen;
        }
        else
        {
            len = 256;

        }

        ret = read( fd, buf, len );
        if ( ret <= 0 )
        {
            break;
        }
        else
        {
            tmplen = len = ret;

        }

        ret = Write( m_hDataSocket, buf, &tmplen );

        if ( ret < 0 || tmplen != len )
        {
            printf( "ret = %d\n", ret );
            perror( "ttt" );
            close( fd );
            close( m_hDataSocket );
            return -1;
        }

        timer = tmptime;
        totallen -= len;

    }

    close( fd );
    close( m_hDataSocket );

    if ( ret < 0 )
    {
        return ret;

    }

    ret = WaitResult( m_hControlChannel );
    if ( ret != 226 )
    {
        return -1;

    }

    //�������
    *iPct = 100;

    return 0;
}

/********************************************************
* function:FTPCmd
* purpose:ִ��ftp������������
* return:   FTP_SUCCEED �ɹ� ��        FTP_FAIL ʧ��
*CHDIR, CDUP, DEL, LS, PWD, MKDIR, RMDIR,SYSTYPE
*********************************************************/
int CFTPClient::FTPCmd ( FTPCMDTYPE      cmd,
                         const char*     cmdvalue,
                         char*           returnvalue,
                         int             retmaxsize )
{
    char    buf[10240];
    switch ( cmd )
    {
        // ����
        case RENAME:
            {
                snprintf( buf, sizeof( buf ), (char*)"RNFR %s\r\n", cmdvalue );     //rename from
                int len = strlen( buf );
                int ret = Write( m_hControlChannel, buf, &len );
                if ( ret < 0 || len != strlen( buf ) )
                {
                    return -1;
                }

                ret = WaitResult( m_hControlChannel );
                if ( ret != 350 )
                {
                    return -1;
                }

                snprintf( buf, sizeof( buf ), (char*)"RNTO %s\r\n", returnvalue );  //rename to
                if ( FTPCommand( buf ) != 0 )
                {
                    return -1;
                }

                return 0;
            }

        // ��ȡԶ���ļ�״̬
        case STATCMD:
            {
                snprintf( buf, sizeof( buf ), (char*)"STAT %s\r\n", cmdvalue );

                int len = strlen( buf );
                int ret = Write( m_hControlChannel, buf, &len );
                if ( ret < 0 || len != strlen( buf ) )
                {
                    return -1;
                }

                ret = WaitResult( m_hControlChannel );
                if ( ret != 213 )
                {
                    return -1;
                }
                else
                {
                    m_strReplyBuffer[strlen( m_strReplyBuffer ) - 1] = 0;

                    char*   p = strchr( m_strReplyBuffer, '\n' );
                    if ( p == NULL )
                    {
                        return -1;
                    }

                    p++;

                    char*   q = strrchr( m_strReplyBuffer, '\n' );
                    if ( q == NULL )
                    {
                        return -1;
                    }

                    *( q + 1 ) = 0;
                    if ( retmaxsize == 0 )
                    {
                        strcpy( returnvalue, p );
                    }
                    else
                    {
                        strncpy( returnvalue, p, retmaxsize );
                    }

                    return 0;
                }
            }

        /* �ı�������˵�ǰ·�� */
        case CHDIR:
            snprintf( buf, sizeof( buf ), (char*)"CWD %s\r\n", cmdvalue );
            return FTPCommand( buf );

        // ��ȡԶ���ļ��Ĵ�С
        case SIZE:
            {
                snprintf( buf, sizeof( buf ), (char*)"SIZE %s\r\n", cmdvalue );

                int len = strlen( buf );
                int ret = Write( m_hControlChannel, buf, &len );
                if ( ret < 0 || len != strlen( buf ) )
                {
                    return -1;
                }

                ret = WaitResult( m_hControlChannel );
                if ( ret / 100 != 2 )
                {
                    if ( ret <= 0 )
                    {
                        return -1;
                    }

                    return FTP_E_FILE;
                }
                else
                {
                    char*   p = strchr( m_strReplyBuffer, ' ' );
                    if ( p == NULL )
                    {
                        return -1;
                    }

                    while ( *p > '9' || *p < '0' )
                    {
                        p++;
                    }

                    if ( retmaxsize == 0 )
                    {
                        strcpy( returnvalue, p );
                    }
                    else
                    {
                        strncpy( returnvalue, p, retmaxsize );
                    }

                    return 0;
                }
            }

        /* �ı��������·��Ϊ��ǰ·���ĸ�Ŀ¼ */
        case CDUP:
            return FTPCommand( "CDUP\r\n" );

        /* ɾ����������ָ���ļ� */
        case DEL:
            snprintf( buf, sizeof( buf ), (char*)"DELE %s\r\n", cmdvalue );
            return FTPCommand( buf );

        /* �г���������ָ��Ŀ¼��Ĭ��Ϊ��ǰĿ¼�������ݣ�������ָ�����ļ��� */
        case LS:
            {
                if ( CreateListenSocket() )
                {
                    return -1;
                }

                if ( cmdvalue == NULL )
                {
                    strcpy( buf, "LIST\r\n" );
                }
                else
                {
                    snprintf( buf, sizeof( buf ), (char*)"LIST %s\r\n", cmdvalue );

                }

                int len = strlen( buf );
                int ret = Write( m_hControlChannel, buf, &len );
                if ( ret < 0 || len != strlen( buf ) )
                {
                    close( m_hListenSocket );
                    return -1;
                }

                ret = WaitResult( m_hControlChannel );
                if ( ret != 150 )
                {           ///////Ӧ�÷��� 150 ��asciiģʽ�����ļ��б�
                    close( m_hListenSocket );
                    return -1;
                }

//                if(m_iPassive==0)
//                {
//                    ///////////////�е�ftp���������ļ���Ϊ��ʱ,���������ݶ˿�,����ֱ���ڿ��ƶ˷��� 226 ��ʾû���κ�����
//                    fd_set          readfds;
//                    fd_set          errfds;
//                    struct timeval  tv;
//
//                    FD_ZERO( &readfds );
//                    FD_SET( m_hControlChannel, &readfds );
//                    FD_ZERO( &errfds );
//                    FD_SET( m_hControlChannel, &errfds );
//                    tv.tv_sec = 0;
//                    tv.tv_usec = 0;
//
//                    while
//                    (
//                        select
//                            (
//                                m_hControlChannel + 1,
//                                &readfds,
//                                NULL,
//                                &errfds,
//                                &tv
//                            )
//                    )
//                    {
//                        if (
//                            ( !FD_ISSET( m_hControlChannel, &errfds ) )
//                        &&  ( FD_ISSET( m_hControlChannel, &readfds ) ) )
//                        {   //////������
//                            ret = WaitResult( m_hControlChannel );
//                            if ( ret == 150 )
//                            {
//                                continue;
//                            }
//
//                            if ( ret == 226 )
//                            {
//                                /////226 ��ȡ�ļ��б��Ѿ��������,���Ǹ�����û�н�����������
//                                int fd;
//                                if (
//                                    (
//                                        fd = open
//                                            (
//                                                returnvalue,
//                                                O_CREAT | O_TRUNC | O_RDWR,
//                                                0660
//                                            )
//                                    ) < 0 )
//                                {
//                                    perror( "open file" );
//                                    close( m_hListenSocket );
//                                    return -1;
//                                }
//
//                                close( fd );
//                                close( m_hListenSocket );
//                                return 2;
//
//                                ////////����2 ��ʾ�б�Ϊ��
//                            }
//                            if( ret > 500 ){
//                                ////���������ܲ�֧�ִ�������LIST���������550
//                                close( m_hListenSocket );
//                                return -2;
//                            }
//                        }
//
//                        break;
//                    }
//                }           ///////////////////////

                ret = Accept( m_hListenSocket, NULL, NULL );
                if(m_hListenSocket>0) close( m_hListenSocket );

                if ( ret > 0 )
                {
                    m_hDataSocket = ret;
                }
                else
                {
                    ret = WaitResult( m_hControlChannel );
                    if ( ret != 226 )
                    {
                        if ( ret > 500 )
                        {
                            return -2;
                        }

                        return -1;
                    }

                    //////////listen��ʱ,���Ƿ��������� 226 �������
                    int fd;
                    if (
                        (
                            fd = open
                                (
                                    returnvalue,
                                    O_CREAT | O_TRUNC | O_RDWR,
                                    0660
                                )
                        ) < 0 )
                    {
                        perror( "open file" );
                        return -1;
                    }

                    close( fd );
                    return 2;

                    ////////����2 ��ʾ�б�Ϊ��
                }

                int fd;
                if (
                    ( fd = open( returnvalue, O_CREAT | O_TRUNC | O_RDWR, 0660 ) ) < 0 )
                {
                    close( m_hDataSocket );
                    perror( "open file" );
                    return -1;
                }

                int timer = time( 0 );
                int totallen = 0;
                int tmptime;
                while ( 1 )
                {
                    tmptime = time( 0 );
                    if ( tmptime >= timer + m_iMaxWaitTime )
                    {
                        close( m_hDataSocket );
                        printf
                        (
                            "time out %d seconds when get a remote file\n",
                            m_hDataSocket
                        );
                        close( fd );
                        return -1;
                    }

                    len = 10240;               
                    ret = Read( m_hDataSocket, buf, &len );

                    if ( ret != 0 )
                    {
                        break;
                    }
                    else if ( len == 0 )
                    {
                        continue;

                    }

                    timer = tmptime;
                    if ( write( fd, buf, len ) < len )
                    {
                        perror( "write file" );
                        close( fd );
                        close( m_hDataSocket );
                        return -1;
                    }

                    totallen += len;
                }

                close( fd );
                close( m_hDataSocket );
                ret = WaitResult( m_hControlChannel );
                if ( ret == 226 )
                {
                    return 0;
                }

                return -1;
            }

        /* ��ʾ�������˵�ǰĿ¼�ľ���·�� */
        case PWD:
            if ( FTPCommand( "PWD\r\n" ) )
            {
                return -1;
            }
            {
                char*   p1 = strchr( m_strReplyBuffer, '\"' );
                *strchr( ++p1, '\"' ) = 0;
                if ( retmaxsize == 0 )
                {
                    strcpy( returnvalue, p1 );
                }
                else
                {
                    strncpy( returnvalue, p1, retmaxsize );
                }
            }

            return 0;

        /* �ڷ������˴���Ŀ¼ */
        case MKDIR:
            snprintf( buf, sizeof( buf ), (char*)"MKD %s\r\n", cmdvalue );
            return FTPCommand( buf );

        /* �ڷ�������ɾ��Ŀ¼  */
        case RMDIR:
            snprintf( buf, sizeof( buf ), (char*)"RMD %s\r\n", cmdvalue );
            return FTPCommand( buf );

        /* ��÷�������ϵͳ���� */
        case SYSTYPE:
            if ( FTPCommand( "SYST\r\n" ) )
            {
                return -1;
            }

            *strstr( m_strReplyBuffer, "\r\n" ) = 0;
            if ( retmaxsize == 0 )
            {
                strcpy( returnvalue, m_strReplyBuffer + 4 );
            }
            else
            {
                strncpy( returnvalue, m_strReplyBuffer + 4, retmaxsize );
            }

            return 0;
            
        case PASV:
            if ( FTPCommand( "PASV\r\n" ) )
            {
                return -1;
            }

            *strstr( m_strReplyBuffer, ")" ) = 0;
            if ( retmaxsize == 0 )
            {
                strcpy( returnvalue, strstr(m_strReplyBuffer, "(" )+1 );
            }
            else
            {
                strncpy( returnvalue, strstr(m_strReplyBuffer, "(" )+1, retmaxsize );
            }

            return 0;
            
		//dz 2007.12.29
		case MDTM:
			snprintf(buf, sizeof(buf), (char *)"MDTM %s\r\n", cmdvalue);
			if (FTPCommand(buf))
			{
				return -1;
			}	
			
            *strstr( m_strReplyBuffer, "\r\n" ) = 0;
            if ( retmaxsize == 0 )
            {
                strcpy( returnvalue, m_strReplyBuffer + 4 );
            }
            else
            {
                strncpy( returnvalue, m_strReplyBuffer + 4, retmaxsize );
            }
			
            return 0;              
            
        default:
            printf( "valid command\n" );
            return -1;
    }
}

/********************************************************
* function:FTPCommand
* purpose:����FTP ���FTP Server
* return:   FTP_SUCCEED �ɹ� ��        FTP_FAIL ʧ��
*
*********************************************************/
int CFTPClient::FTPCommand ( const char*     strCommand )
{
    int len = strlen( strCommand );
    int ret = Write( m_hControlChannel, (char*)strCommand, &len );
    if ( ret < 0 || len != strlen( strCommand ) )
    {
        return -1;
    }

    ret = WaitResult( m_hControlChannel );
    if ( ret / 100 != 2 )
    {
        return -1;
    }

    return 0;
}

/********************************************************
* function:FTPLogout
* purpose:�˳���½
* return:  
*
*********************************************************/
void CFTPClient::FTPLogout ( void )
{
    FTPCommand( "QUIT\r\n" );
    close( m_hControlChannel );
}

/*******************************************************
 *
 *      �����������ֽ������
 *      ���أ�1-�߶��ֽ���0-�Ͷ��ֽ���
 *
 ********************************************************/
int CFTPClient::IsBigOrder ( void )
{
    union
    {
        short   s;
        char    c[sizeof( short )];
    } un;

    un.s = 0x0102;

    /*printf("%s:",CPU_VENDOR_OS);*/
    if ( sizeof( short ) >= 2 )
    {
        if ( un.c[0] == 1 && un.c[1] == 2 )
        {
            //printf("big-endian\n");
            return 1;
        }
        else if ( un.c[0] == 2 && un.c[1] == 1 )
        {
            //printf("little-endian\n");
            return 0;
        }
        else
        {
            //printf("unknown\n");
            return -1;
        }
    }
    else
    {
        printf( "sizeof(short) = %d\n", (int)sizeof( short ) );
        return -1;
    }
}

/* return: -1:дʧ�ܣ� 0 д�ɹ� */
int CFTPClient::Write ( int     fd,
                        char*   buff,
                        int*    len )
{
    fd_set          writefds;

    //fd_set errfds;
    struct timeval  tv;
    int             ret;

    FD_ZERO( &writefds );
    FD_SET( fd, &writefds );

    //	FD_ZERO(&errfds);
    //	FD_SET(fd,&errfds);
    tv.tv_sec = m_iTimeOut;
    tv.tv_usec = 0;
    if ( select( fd + 1, NULL, &writefds, 0, &tv ) )
    {
        //		if(FD_ISSET(fd, &er)) {
        //			*len = 0;
        //			return -1;
        //		}
        if ( FD_ISSET( fd, &writefds ) )
        {
            ret = write( fd, buff, *len );
            *len = 0;
            if ( ret > 0 )
            {
                *len = ret;
                return 0;
            }
            else
            {
                return -2;
            }
        }
    }

    return -3;
}

/*return : -1,ʧ�ܣ�0���ɹ����߳�ʱ��1��EOF */
int CFTPClient::Read ( int     fd,
                       char*   buff,
                       int*    len )
{
    fd_set          readfds;
    fd_set          errfds;
    struct timeval  tv;
    int             ret;

    FD_ZERO( &readfds );
    FD_SET( fd, &readfds );
    FD_ZERO( &errfds );
    FD_SET( fd, &errfds );
    tv.tv_sec = m_iTimeOut;
    tv.tv_usec = 0;
    if ( select( fd + 1, &readfds, NULL, &errfds, &tv ) )
    {
        if ( FD_ISSET( fd, &errfds ) )
        {
            *len = 0;
            return -1;
        }

        if ( FD_ISSET( fd, &readfds ) )
        {
            ret = read( fd, buff, *len );
            *len = 0;
            if ( ret == 0 )
            {
                return 1;
            }

            if ( ret < 0 )
            {
                return -1;

            }

            *len = ret;
            return 0;
        }
    }

    *len = 0;
    return 0;
}

/* �ȴ����ӣ����أ�0--��ʱ��-1�����������׽��� */
int CFTPClient::Accept ( int                 fd,
                         struct sockaddr*    addr,
                         int*                len )
{
  
    if (m_iPassive==1)
    {
        m_hDataSocket = m_hListenSocket;
        m_hListenSocket = 0;
        return m_hDataSocket;
    }
    
    fd_set          readfds;
    fd_set          errfds;
    struct timeval  tv;

    FD_ZERO( &readfds );
    FD_SET( fd, &readfds );
    FD_ZERO( &errfds );
    FD_SET( fd, &errfds );
    tv.tv_sec = m_iTimeOut;
    tv.tv_usec = 0;

    if ( select( fd + 1, &readfds, NULL, &errfds, &tv ) )
    {
        if ( FD_ISSET( fd, &errfds ) )
        {
            return -1;
        }
        else if ( FD_ISSET( fd, &readfds ) )
        {
            #if defined( _AIX ) || defined( __linux__ )
            return accept( fd, addr, (socklen_t*)len );
            #else
            return accept( fd, addr, len );
            #endif
        }
        else
        {
            return 0;
        }
    }

    return 0;
}

/* return -1:ʧ�ܣ� 0����ʱ�� ����������Ӧ������*/
int CFTPClient::WaitResult ( int fd )
{
    int tmptime;
    int ret;
    memset( m_strReplyBuffer, 0, sizeof( m_strReplyBuffer ) );

    int totallen = 0;
    int len = 0;
    int timer = time( 0 );

    while ( 1 )
    {
        tmptime = time( 0 );
        if ( tmptime >= timer + m_iMaxWaitTime )
        {
            ////////////
            if ( totallen > 3 )
            {               /////����Ѿ�����Ӧ�������˾ͷ�����
                printf( "-- %s\n", m_strReplyBuffer );
                return atol( m_strReplyBuffer );
            }

            return 0;
        }

        len = 1;            //sizeof(m_strReplyBuffer)-totallen;
        ret = Read( fd, &m_strReplyBuffer[totallen], &len );

        if ( ret != 0 )
        {
            return -1;
        }
        else if ( len == 0 )
        {
            continue;
        }

        totallen += len;

        ////////zhangap
        if ( totallen < 2 )
        {                   //////////
            continue;
        }

        if ( totallen >= sizeof( m_strReplyBuffer ) - 10 )
        {                   //////����ftp������????
            return atol( m_strReplyBuffer );
        }

        /* ���ﷵ�ض�����ϸ���ǣ�����ʱ����һ�е��ĸ��ַ��Ǽ��� */
        if (
            (
                m_strReplyBuffer[totallen - 2] == '\r'
            &&  m_strReplyBuffer[totallen - 1] == '\n'
            )
        ||  m_strReplyBuffer[totallen - 1] == 0 )
        {
            int retcode = atol( m_strReplyBuffer );
            if ( strlen( m_strReplyBuffer ) < 4 || m_strReplyBuffer[3] != '-' )
            {
                printf( "-- %s\n", m_strReplyBuffer );
                return atol( m_strReplyBuffer );
            }

            ///////////////////			
            char*   p = m_strReplyBuffer;
            char*   q;
            while ( GetLine( p, &q ) == 0 )
            {
                if (
                    strlen( q ) > 3
                &&  *( q + 3 ) == ' '
                &&  atol( q ) == retcode )
                {
                    printf( "-- %s\n", m_strReplyBuffer );
                    return retcode;
                }

                p = q;
            }

            /* ��������еĻ�����Ѱ���һ�� */
            /*			if( m_strReplyBuffer[3] == '-' ) {
				if( m_strReplyBuffer[totallen-2] == '\r' )
					m_strReplyBuffer[totallen-2] = 0;
				else
					m_strReplyBuffer[totallen-1] = 0;
				char* cp = strrchr(m_strReplyBuffer,'\n');
				if( cp )
					return atol(cp + 1 );
			}
			return atol(m_strReplyBuffer);
*/
        }
    }

}

int CFTPClient::GetLine ( char*   src,
                          char **  nextline )
{
    char*   p = src;
    for ( ; *p != 0; p++ )
    {
        if ( *p == '\n' && *( p + 1 ) != 0 )
        {
            *nextline = p + 1;
            return 0;
        }
    }

    return -1;
}


