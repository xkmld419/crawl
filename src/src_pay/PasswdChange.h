#ifndef __PASSWD_CHANGE_H_INCLUDED_
#define __PASSWD_CHANGE_H_INCLUDED_

#include "abmcmd.h"
#include "DccOperation.h"
#include "DccOperationSql.h"

class ABMException;
class DccOperation;

/*
*  ��½�����޸���
*/

class PasswdChange
{
	public:
		PasswdChange();
		~PasswdChange();
		
		//int init(ABMException &oExp);
		int deal(ABMCCR *pCCR, ABMCCA *pSendPack); // �������յ�CCR
		
	private:
		 int _deal(PasswdChangeStruct *oRes,vector<PasswdChangeCond *> g_vPasswdChangeCond);//��½�����Ȩ������
		 int PasswdHSSChange(PasswdChangeStruct *oRes,PasswdChangeCond *g_itr2);
		 
	private:
		ABMException *oExp;
		DccOperation *m_dccOperation;
		PasswdChangeCCR *m_poPasswdPack; // ���-��ȫ��HSS�������½�����޸ĵ�CCR��
		PasswdChangeCCR *m_poUnpack; // ���-�Է���ƽ̨��CCR��
		PasswdChangeCCA *m_poPack;//���-�����ӷ���ƽ̨��CCA��
};



#endif/*__PASSWD_CHANGE_H_INCLUDED_*/