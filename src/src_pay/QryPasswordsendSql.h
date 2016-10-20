#ifndef __QRY_PASSWORD_SEND_SQL_H_INCLUDED_
#define __QRY_PASSWORD_SEND_SQL_H_INCLUDED_

#include "TTTns.h"
#include "abmsendcmd.h"
#include "LogV2.h"

class ABMException;

class QryPasswordSendSql : public TTTns
{

public:

    QryPasswordSendSql();

    ~QryPasswordSendSql();

	 int init(ABMException &oExp);
    int selectServNbr(QryPasswordCond *psd,ABMException &oExp);		//�·�������֤
    int insertpwSql(char *passwd,PasswordResponsStruct *Res,QryPasswordCond *psd,ABMException &oExp);			//д��������
    int insertSendSql(long t_servID,char *msg,QryPasswordCond *psd,ABMException &oExp);			//д���Ź�����
    int updatepwSql(char *passwd,QryPasswordCond *psd,ABMException &oExp);			//������������
    int selectServID(QryPasswordCond *psd,ABMException &oExp);			//�����ֻ�������û�ID
public:
	long servId;				//�·��ֻ������ӦSERVID
    
};

#endif/*__QRY_PASSWORD_SEND_SQL_H_INCLUDED_*/
