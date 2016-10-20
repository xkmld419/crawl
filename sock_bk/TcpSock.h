#ifndef __TCP_SOCK_H_INCLUDED_
#define __TCP_SOCK_H_INCLUDED_

//select ��iFlag���� �ֱ��ʾ �ɶ� ��д �쳣
#define SELECT_RD_FLAG 0             
#define SELECT_WT_FLAG 1
#define SELECT_EP_FLAG 2

//recvn ���ص��Զ�������
#define ECODE_WOULDBLOCK  1    //�����ݿɶ�
#define ECODE_BADSOCKET   2    //�����õ�SOCKET
#define ECODE_INVALID     3    //��������
#define ECODE_SYSCALL     4    //ϵͳ����ʧ��


//��� ��֧�� ���� ���֧�ַ����� ��Ҫ�����ж�
class TcpSock
{

public:
    
    TcpSock();
    
    virtual ~TcpSock();
    
    int getSock() {
        return m_iSockID;    
    }
    
    int close();
    
    int selectRD(int iSec) {
        return select(m_iSockID, SELECT_RD_FLAG, iSec);    
    }
    
protected:
    
    //�ɹ�����0 ʧ�ܷ��ش����
    int sendn(int iSock, const void *pBuff, int iSize);
    
    //�ɹ�����0 ʧ�ܷ��ش����
    int recvn(int iSock, void *pBuff, int iSize, int iSec=0);
    
    int select(int iSock, int iFlag, int iSec);

protected:
    
    int m_iSockID;           
};

class AcceptSock;

class ListenSock : public TcpSock
{
    
public:
    
    ListenSock();
    
    ~ListenSock();
    
    int listen(char *pIP, int iPort);
    
    int accept(AcceptSock &aptSock);    
            
};

class AcceptSock : public TcpSock
{

public:
    
    friend int ListenSock::accept(AcceptSock &aptSock);
        
    AcceptSock();
    
    ~AcceptSock();
    
    char *getRemIP() {
        return m_sRemIP;    
    }
    
    int getRemPort() {
        return m_iRemPort;    
    }
    
    int sendDCC(const void *pBuff, int iSize);
    
    int recvDCC(void *pBuff, int iMaxSize);
    
private:
        
    char m_sRemIP[16];
    
    int  m_iRemPort;             
};

class ConnSock : public TcpSock
{

public:
    
    ConnSock();
    
    ~ConnSock();
    
    int connect(char *pLocIP, int iLocPort, char *pRemIP, int iRemPort);
    
    int sendDCC(const void *pBuff, int iSize);

    int sendCer(const void *pBuff, int iSize);
    
    int recvDCC(void *pBuff, int iMaxSize);
    
    int rebuiltSock() {
        this->close();
        return reconnect();    
    }
    
private:
    
    int reconnect();
    
public:
        
    char m_sLocIP[16];
    
    char m_sRemIP[16];
    
    int m_iLocPort;
    
    int m_iRemPort;        
};    

#endif/*__TCP_SOCK_H_INCLUDED_*/
