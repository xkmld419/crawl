/*VER: 4*/ 
#ifndef VPNINFOREADER_H_HEADER_INCLUDED_
#define VPNINFOREADER_H_HEADER_INCLUDED_

#include "../../etc/LocalDefine.h"

#include "CommonMacro.h"
#include "VpnInfo.h"
#include "StdEvent.h"
#include "BillingCycle.h"
#include <vector>
#include "BillingCycleMgr.h"

using namespace std;

class BillingCycleMgr;


class VpnInfoReader : public VpnInfoBase
{
  public:
    //����
    VpnInfoReader();    
};

//## VPN���϶�ȡ�ӿ�
class VpnInfoInterface : public VpnInfoReader
{
  public:
    //��ȡ�Ƿ����
    operator bool();

	//�����Щ��̬�������û�б���ʼ�������г�ʼ��
    //VpnInfoInterface();

	//����vpn_code ����vpn����
	static const char * getVpnName(char * sVpnCode,char *sDate);

	/*����Member_code ����vpn��Ϣ
		iFlag ȡֵ��
		1��sVpnMember ��999 ����VpnMember����
		2��sVpnMember ����999����������VPN_NUMBER������VpnMember����
		3��sVpnMember ����999����������VPN_CODE������VpnMember����
		4: sVpnMember ��������룬���ؿ�
		5��sVpnMember û����Ϣ�����ؿ�
	*/
	static VpnMemberInfo const * getVpnInfoByMember(char * sVpnMember,char *sDate,int &iFlag);

	//���ݺ�����VPN��Ϣ
	static VpnMemberInfo const * getVpnInfo(char * sAccNbr,char *sDate);

	/*���ݺ�����VPN��Ϣ��չ
	  iFlag ȡֵ��
		1��sAccNbr ��999 ���ض����ǿ�
		2��sAccNbr ������999����������VPN�����ض����Ǿ����VpnMember
		3��sAccNbr ������999Ҳ����������VPN�����ض����ǿ�
	*/
	static VpnMemberInfo const * getVpnInfoEx(char * sAccNbr,char *sDate,int &iFlag);

	//���ݷ�999��vpn_code�ͺ��루999���͵ĺ��룩�����Ƿ���ͬһ��VPN��
	static int  isSameCodeAccNbr(char * sVpnCode,char * sAccNbr,char *sDate,int &iRoleA,int &iRoleB);
	
	//�����������루����999���͵ģ������Ƿ���һ��VPN��
	static int  isSame2AccNbr(char * sAccNbrA,char * sAccNbrB,char *sDate,int &iRoleA,int &iRoleB);

	//����VPN��Ϣ�Ҹ��ڵ���Ϣ��û�з��� NULL
	static VpnMemberInfo const * getFatherVpnInfo(VpnMemberInfo * oVpnInfo,char *sDate);

	//���ݺ�����Ҷ�Ӧ��VPN��Ϣ
	static const VpnFriendNumInfo * getVpnFriend(char * sAccNbr,char * sDate);

	//����VPN����ͺ�����֤�Ƿ���ڶ�Ӧ��ϵ
	static bool IsVpnFriend(char * sVpnCode,char * sAccNbr,char * sDate);

	//��������VPN������֤�Ƿ�����й�ϵ
	static bool isVpnRelation(char * sVpnCodeA,char * sVpnCodeB,char * sDate);
};

#endif /* VPNINFOREADER_H_HEADER_INCLUDED_ */
