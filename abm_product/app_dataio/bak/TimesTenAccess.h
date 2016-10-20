/*
*	TT���ݿ��½API
*/

#ifndef __TIMESTEN_ACCESS_H_INCLUDED_
#define __TIMESTEN_ACCESS_H_INCLUDED_

#include "TimesTenInterface.h"
#include "ABMException.h"

#define GET_CURSOR(cursor, conn, conf, oExp) GetTTCursor(cursor, conn, conf, oExp)
#define REL_CURSOR(cursor) RelTTCursor(cursor)


/*
*	��������: ����һ����ò���TT���ݿ���α�
*	����:
*        TimesTenCMD *&cursor: �������ݿ���α�, ָ������ʼ��ΪNULL
*        TimesTenConn *&conn: ��½�����ݿ��α�, ���instanceΪNULL, �ڲ������LoginTT��ȡ
*        [label]�µ�����, ����һ����½TT��ʵ���ĻỰ, �ڸûỰ����������α�, ���instance��NULL,
*        ֱ���ڸûỰ����������α�
*        const char *pconf: �����ļ��еĶ�ȡ��ǩ
*	����: �����ɹ�����0, ʧ�ܷ��ش����
*   ע��: GetTTHdl�� RelTTHdlҪ�ɶԳ���, ��ֹ���ڴ�й©,TimesTenConn���ͷ���Ҫ����LogoutTT
*/
extern int GetTTCursor(TimesTenCMD *&cursor, TimesTenConn *&conn, const char *pconf, ABMException &oExp);

/*
*	��������: �ͷ�һ��TT���ݿ�ľ��
*	����:
*        TimesTenCMD *&hdl: �������ݿ�ľ��
*	����: 
*/
extern void RelTTCursor(TimesTenCMD *&cursor);


/*   ��½TT���ݿ�
* pconf:  ��ȡ�����ļ��ı�־
* conn: TT��ʵ��,��ʼֵ����ΪNULL
* �ɹ�����0, ʧ�ܷ��ش����
*/
extern int ConnectTT(const char *pconf, TimesTenConn *&conn, ABMException &oExp);

/* ע��
* refTTHandle: TT��ʵ��
* �ɹ�����0 , ʧ�ܷ��ش����
*/
extern int DisconnectTT(TimesTenConn *&conn, ABMException &oExp);

/* ��ȡ������Ϣ*/
extern int GetConnectInfo (const char *pconf, char *usr, char *pwd, char *dsn, ABMException &oExp);

#endif/*__TIMESTEN_ACCESS_H_INCLUDED_*/
