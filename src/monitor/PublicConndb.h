#ifndef __PUBLIC_CONNDB_H_INCLUDED_
#define __PUBLIC_CONNDB_H_INCLUDED_
#include "ABMException.h"
#include "TTTns.h"
#include "abmpaycmd.h"
#include "abmcmd.h"
#include "LogV2.h"
#include <time.h>
#include <iostream>
using namespace std;

extern TimesTenCMD *m_pBalanceTransReset;//���»�������
extern TimesTenCMD *m_pPasswordResetUpdate;//֧�������޸�
extern TimesTenCMD *m_pPasswordResetDelete;//֧�������޸�
extern TimesTenCMD *m_pPasswordResetInsert;//֧�������޸�
extern TimesTenCMD *m_pPayStatusResetUpdate;//֧�������޸�
extern TimesTenCMD *m_pPayRulesReset;//֧�������޸�
extern TimesTenCMD *m_pTransferRulesInsert;//����֧����������

extern TimesTenCMD *m_poPublicConn;//��ѯ
extern TimesTenCMD *m_pInsertPayout;//֧��
extern TimesTenCMD *m_pInsertPayment;//��¼ҵ�����
extern TimesTenCMD *m_pInsertRela;//��¼֧����Դ��ϵ
extern TimesTenCMD *m_pUpdateBalance;//�����˱�
extern TimesTenCMD *m_pUpdateSource;//������Դ
extern TimesTenCMD *m_pInsertServInfo;//serv
extern TimesTenCMD *m_pInsertAcctInfo;//acct
extern TimesTenCMD *m_pInsertServAcctInfo;//serv_acct
extern TimesTenCMD *m_pInsertAcctBalance;//acct_balance
extern TimesTenCMD *m_pInsertBalanceSource;//balance_source

extern TimesTenCMD *m_pUpdatePayoutState;//����֧����¼״̬
extern TimesTenCMD *m_pUpdateRealState;// ����֧����Դ��ϵ״̬
extern TimesTenCMD *m_pInsertAttUserInfo;// att_user_info
extern TimesTenCMD *m_pInsertAppUserInfo;// app_user_info


extern TimesTenCMD *m_pInsertServHis;//��¼SERV��ʷ��Ϣ��
extern TimesTenCMD *m_pInsertAcctHis;//��¼ACCT��ʷ��Ϣ��
extern TimesTenCMD *m_pInsertServAcctHis;//��¼SERV_ACCT��ʷ��Ϣ��
extern TimesTenCMD *m_pDeleteAcctBalance;//ɾ������˵����Ϊ0����
extern TimesTenCMD *m_pDeleteServInfo;//ɾ��SERV����Ϣ
extern TimesTenCMD *m_pDeleteAcctInfo;//ɾ��ACCT����Ϣ
extern TimesTenCMD *m_pDeleteServAcctInfo;//ɾ��SERV_ACCT����Ϣ
extern TimesTenCMD *m_pChangeInservHis;//�����ü�¼SERV��ʷ��Ϣ��
extern TimesTenCMD *m_pChangeNbr;//����
extern TimesTenCMD *m_pChangeAcct;//��������ACCT_ID
extern TimesTenCMD *m_pUpdateServAcct;//��������SERV_ACCT

extern TimesTenCMD *m_pUpdateServAddState;	//�û���Ϣ������ͬ��״̬����
extern TimesTenCMD *m_pInsertSendSql;	//д���Ź�����

extern TimesTenCMD *m_pLoginSaveDcc;	//��½�����Ȩ
extern TimesTenCMD *m_pPasswdChangeDcc; //��½�����޸�

extern TimesTenCMD *m_pGetSeq;//ȡѭ������
extern TimesTenCMD *m_pInserUserAddErr;	//����ͬ���������¼������

extern TimesTenCMD *m_pSaveActiveCCR; // ����CCR������ˮ
extern TimesTenCMD *m_pUpdateCCR;// ����CCR������ˮ��¼״̬

class ABMException;
#define FreeObject(x) \
	if (x){ \
		delete x ;x=NULL; \
	}
#define FreeVector(X) {\
		for (unsigned int u=0; u<X.size();u++) { \
			delete X[u]; X[u] = NULL; \
		} \
		X.clear(); \
	}
inline char *ltoa(const long lValue)
{
	static char sTemp[32];
	sprintf(sTemp,"%ld",lValue);
	return (char*)sTemp;
};
inline int GetDate(char *date)
{
	time_t   Time;
	struct tm *T;
	time(&Time);
	T=localtime(&Time);
	sprintf(date, "%04d%02d%02d",T->tm_year+1900,T->tm_mon+1,T->tm_mday);
	return 0;
};
inline int GetTime(char *date)
{
	time_t   Time;
	struct tm *T;
	time(&Time);
	T=localtime(&Time);
	sprintf(date, "%04d%02d%02d",T->tm_year+1900,T->tm_mon+1,T->tm_mday,T->tm_hour, T->tm_min, T->tm_sec);
	return 0;
};
//��ʱ��ת��Ϊ����
//����ֵ����1900~1970������+8Сʱʱ��
inline unsigned int date2long( long  x){
    int b ;
    int y;
    struct tm tmtime;  
    b =x/100000000;
    b= b/100;
    tmtime.tm_year=b-1900;
    b= ((x/1000)/100000)%100;
    tmtime.tm_mon=b-1;
    b= ((x/1000)/1000)%100;
    tmtime.tm_mday=b;
    b= ((x/1000)/10)%100;
    tmtime.tm_hour=b;
    b= (x/100)%100;
    tmtime.tm_min=b;
    b= x%100;
    tmtime.tm_sec=b;
    y=mktime(&tmtime);
    cout<<endl;
    return y+2209017599; 
}
//������ת��Ϊ�ַ�������ʱ��
//Ŀǰ���Ϊ��1900��ʼ��������Ҫ�ȼ�ȥ��1900~1970�������ټ�ȥ8Сʱʱ���ټ���
inline int long2date(unsigned int U32Var,char *str)
{
   U32Var -=2209017599;
   time_t t;
   time_t nowtime;
   long x;    
   struct tm *ptm;
   nowtime = (time_t)U32Var;
   ptm = localtime(&nowtime);
   
   sprintf(str, "%04d%02d%02d%02d%02d%02d", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec); 
   return 0;
};

class PublicConndb : public TTTns
{

public:

	PublicConndb();

	~PublicConndb();

	//��ʼ�����ݿ������
	int init(ABMException &oExp);
	//�ύ����
	int Commit(ABMException &oExp);
	//�ع�����
	int RollBack(ABMException &oExp);
};

#endif/*__PUBLIC_CONNDB_H_INCLUDED_*/

