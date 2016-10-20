#ifndef __QRY_PASSWORD_SEND_H_INCLUDED_
#define __QRY_PASSWORD_SEND_H_INCLUDED_

#include "abmsendcmd.h"
#include "DccOperation.h"
#include "DccOperationSql.h"

class QryPasswordSendSql;
class ABMException;
class DccOperation;

/*
*   ������֤�����·�������
*/

class QryPasswordSend
{

public:

    QryPasswordSend();

    ~QryPasswordSend();

    int init(ABMException &oExp);

    int deal(ABMCCR *pCCR, ABMCCA *pCCA);

private:

    int _deal(PasswordResponsStruct *oRes,QryPasswordCond *pIn);

private:

    QryPasswordSendSql *m_poSql;

    QryPasswordCCR *m_poUnpack;

    QryPasswordCCA *m_poPack;
    
    DccOperation *m_dccOperation;
    
    ABMException *oExp;

};

#endif/*__QRY_PASSWORD_SEND_H_INCLUDED_*/
