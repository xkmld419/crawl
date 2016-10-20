#ifndef __QRY_SERV_INFO_SYN_H_INCLUDED_
#define __QRY_SERV_INFO_SYN_H_INCLUDED_

#include "abmcmd.h"
#include "DccOperation.h"
#include "DccOperationSql.h"

class QryServInfosynSql;
class ABMException;

/*
*   ����ͬ��:���������š�����
*/

class QryServInfosyn
{

public:

    QryServInfosyn();

    ~QryServInfosyn();

    int init(ABMException &oExp);

    int deal(ABMCCR *pCCR, ABMCCA *pCCA);

private:

    int _deal(QryServSynCond *pIn);  //���������š�������������

private:

    QryServInfosynSql *m_poSql;

    QryServSynCCR *m_poUnpack;

    QryServSynCCA *m_poPack;
    
    DccOperation *m_dccOperation;
    
    ABMException *oExp;

};

#endif/*__QRY_SERV_INFO_SYN_H_INCLUDED_*/
