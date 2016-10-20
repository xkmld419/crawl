/*
*	ll���ݿ��½API
*/

#ifndef __ORACLE_ACCESS_H_INCLUDED_
#define __ORACLE_ACCESS_H_INCLUDED_

#include "TOCIQuery.h"
#include "ABMException.h"

/*
*	��������: ����һ����ò���ORA���ݿ���α�
*	����:
*        TOCIQuery *&cursor: �������ݿ���α�, ָ������ʼ��ΪNULL
*        TOCIDatabase *&oci: ��½�����ݿ�ʵ��, ���ociΪNULL, �ڲ������LoginOra��ȡ
*        [label]�µ�����, ����һ����½ORA��ĻỰ, �ڸûỰ����������α�, ���oci��NULL,
*        ֱ���ڸûỰ����������α�
*        const char *label: �����ļ��еĶ�ȡ��ǩ
*	����: �����ɹ�����0, ʧ�ܷ��ش����
*   ע��: GetOraHdl�� RelOraHdlҪ�ɶԳ���, ��ֹ���ڴ�й©,TOCIDatabase���ͷ���Ҫ����LogoutOra
*/
extern int GetOraCursor(TOCIQuery *&cursor, TOCIDatabase *&oci, const char *pconf, ABMException &oExp);

/*
*	��������: �ͷ�һ��ORA���ݿ���α�
*	����:
*        TOCIQuery *&hdl: �������ݿ���α�
*	����: 
*/
extern void RelOraCursor(TOCIQuery *&cursor);


/*   ��½ORA���ݿ�
* pconf:  ��ȡ�����ļ��ı�־
* database: ORA��ʵ��, ��ʼֵ����ΪNULL
* �ɹ�����0, ʧ�ܷ��ش����
*/
extern int ConnectOra(const char *pconf, TOCIDatabase *&database, ABMException &oExp);

/* ע��
* refOciHandle: ORA��ʵ��
* �ɹ�����0 , ʧ�ܷ��ش����
*/
extern int DisconnectOra(TOCIDatabase *&database, ABMException &oExp);


/* ��ȡ������Ϣ*/
extern int GetOraConnectInfo (const char *pconf, char *usr, char *pwd, char *dsn, ABMException &oExp);

#endif/*__ORACLE_ACCESS_H_INCLUDED_*/
