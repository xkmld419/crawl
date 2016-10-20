#ifndef __PASSWD_CHANGE_BIZ_H_INCLUDED_
#define __PASSWD_CHANGE_BIZ_H_INCLUDED_

#include "abmcmd.h"
#include "DccOperation.h"
#include "DccOperationSql.h"

class ABMException;
class DccOperation;

/*
*   ��½�����޸���
*	 ȫ��HSS�˽��յ�ȫ��ABM�ĵ�½�����޸�CCR �Ե�½�����Ȩ���ؽ����ȫ������ABM
*/

class PasswdChangeBiz
{
	public:
		PasswdChangeBiz();
		~PasswdChangeBiz();
		
		//int init(ABMException &oExp);
		int deal(ABMCCR *pRecivePack, ABMCCA *pSendPack); // ������յ�CCA��������CCA
	private:
		int _deal(PasswdChangeStruct oRes,PasswdChangeData *pData);
	private:
		ABMException *oExp;
		DccOperation *m_dccOperation;
		PasswdChangeCCA *m_poUnpack;	//���-ȫ��HSS���ص�CCA�� ȫ��HSS��ȫ��ABM
		PasswdChangeCCA *m_poPack;	//���-���ظ��Է���ƽ̨��CCA��		
};

#endif /*__PASSWD_CHANGE_BIZ_H_INCLUDED_*/
