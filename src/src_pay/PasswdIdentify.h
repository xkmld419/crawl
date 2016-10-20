#ifndef __PASSWD_IDENTIFY_H_INCLUDED_
#define __PASSWD_IDENTIFY_H_INCLUDED_

#include "abmcmd.h"
#include "DccOperation.h"
#include "DccOperationSql.h"

class ABMException;
class DccOperation;

/*
*   ��½�����Ȩ��
*/

class PasswdIdentify
{
	public:
		PasswdIdentify();
		~PasswdIdentify();
		
		//int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pSendPack); // �������յ�CCR
		
	private:
		 int _deal(LoginPasswdStruct *oRes,vector<LoginPasswdCond *> g_vLoginPasswdCond);//��½�����Ȩ������
		 int loginHSSIdentify(LoginPasswdStruct *oRes,LoginPasswdCond *g_itr2);
		 
	private:
		ABMException *oExp;
		DccOperation *m_dccOperation;
		PasswdIdentifyCCR *m_poPasswdPack; // ���-��ȫ��HSS�������½�����Ȩ��CCR��
		//PasswdIdentifyCCA *m_poPasswdUnpack;//���-ȫ��HSS�˷��ؼ�Ȩ�����CCA��
		LoginPasswdCCR *m_poUnpack; // ���-�Է���ƽ̨��CCR��
		LoginPasswdCCA *m_poPack;//���-�����ӷ���ƽ̨��CCA��
};



#endif/*__PASSWD_IDENTIFY_H_INCLUDED_*/