#ifndef __QRY_SERV_SYN_SQL_H_INCLUDED_
#define __QRY_SERV_SYN_SLQ_H_INCLUDED_

#include "TTTns.h"
#include "abmcmd.h"
#include "PublicConndb.h"
#include "errorcode_pay.h"

/*
* ����ͬ�������·�������
*/

class ABMException;

class QryServSynSql : public TTTns
{

public:

    QryServSynSql();

    ~QryServSynSql();

    int init(ABMException &oExp);
    int selectServAdd(ABMException &oExp);		//ȡ�û���Ϣ��������(��Կ���)
    int selectSession(char *m_oDsnNbr,ABMException &oExp);
    int updateServAddState(char *m_oSessionId,ABMException &oExp);		//�û���Ϣ����ͬ��״̬����
    int insertSendSql(char *m_oSessionId,ABMException &oExp);	       //д���Ź�����
    int insertUserAddErr(char *m_oSessionId,int m_oErrId,ABMException &oExp);	//�û���Ϣ������ͬ�����ش�����
    
public:
	vector<struct QryServSynCond> vec;
	//char m_oSessionId[64];
	vector<char *> ses;
};

#endif/*__QRY_SERV_SYN_SQL_H_INCLUDED_*/
