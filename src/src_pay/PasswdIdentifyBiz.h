#ifndef __PASSWD_IDENTIFY_BIZ_H_INCLUDED_
#define __PASSWD_IDENTIFY_BIZ_H_INCLUDED_

#include "abmcmd.h"
#include "DccOperation.h"
#include "DccOperationSql.h"

class ABMException;
class DccOperation;

/*
*   ��½�����Ȩ��
*	 ȫ��HSS�˽��յ�ȫ��ABM�ĵ�½�����ȨCCR �Ե�½�����Ȩ���ؽ����ȫ������ABM
*/

class PasswdIdentifyBiz
{
	public:
		PasswdIdentifyBiz();
		~PasswdIdentifyBiz();
		
		//int init(ABMException &oExp);
		int deal(ABMCCR *pRecivePack, ABMCCA *pSendPack); // ������յ�CCA��������CCA
	private:
		int _deal(LoginPasswdStruct oRes,PasswdIdentifyData *pData,char *sAccNbr);
	private:
		ABMException *oExp;
		DccOperation *m_dccOperation;
		PasswdIdentifyCCA *m_poUnpack;	//���-ȫ��HSS���ص�CCA�� ȫ��HSS��ȫ��ABM
		LoginPasswdCCA *m_poPack;	//���-���ظ��Է���ƽ̨��CCA��		
};

#endif /*__PASSWD_IDENTIFY_BIZ_H_INCLUDED_*/