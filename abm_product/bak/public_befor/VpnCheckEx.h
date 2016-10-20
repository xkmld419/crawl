/*VER: 1*/ 
#ifndef _VPNCHECK_EX_H_
#define _VPNCHECK_EX_H_

#include "Process.h"
#include "CommonMacro.h"
#include "Environment.h"
#include "VpnInfo.h"
#include "VpnInfoReader.h"
#include "interrupt.h"
#include "NbrTypeMgr.h"
#include "AttrTransMgr.h"


#define VPNINFO_SEGMENT "VPN"
#define VPN_TYPE_NO 			"VPN_TYPE_NO"
#define VPN_TYPE_LOCAL 		"VPN_TYPE_LOCAL"
#define VPN_TYPE_PROVINCE "VPN_TYPE_PROVINCE"
#define VPN_TYPE_COUNTRY  "VPN_TYPE_COUNTRY"
#define VPN_TYPE_RELATION "VPN_TYPE_RELATION"
#define VPN_TYPE_FRIEND 	"VPN_TYPE_FRIEND"

#define VPN_TYPE_LOCAL_NOMBER 		"VPN_TYPE_LOCAL_NOMBER"
#define VPN_TYPE_LOCAL_CALLING_CENTER 		"VPN_TYPE_LOCAL_CALLING_CENTER"//�ܻ����У��������ܻ���
#define VPN_TYPE_LOCAL_CALLED_CENTER 		"VPN_TYPE_LOCAL_CALLED_CENTER"   //�����ܻ����������ܻ���




#define	VPN_ACC_NBR_999				0x001
#define	VPN_ACC_NBR_VPN				0x002


#define __VPN_JIANGSHU_
class VpnCheckEx ;
class VpnCheckEx
{
public:
	VpnCheckEx(void);
	~VpnCheckEx(void);
	/**
	vpn ��� 0 - ��vpn  1-����vpn 2-�籾����vpn 3-vpn������ 4-vpn�������
	**/
	int check(EventSection *poEventSection);
	int checkAccNBR(const char* pAccNBR,const char* pStartDate);

	static VpnCheckEx* GetInstance(void);
private:
	VpnMemberInfo * m_poVpnInfoA;
	VpnMemberInfo * m_poVpnInfoB;
	char m_sTmpAccNbrA[MAX_BILLING_NBR_LEN];
	char m_sTmpAccNbrB[MAX_CALLED_NBR_LEN];

	char m_sTmpStartTime[MAX_CREATED_DATE_LEN];
	VpnInfoInterface * interface;

	void bindVpnInfo(void);

	bool checkMobile(char * mobile);
	int m_iVpn_type_no;
	int m_iVpn_type_local;
	int m_iVpn_type_province;
	int m_iVpn_type_country;
	int m_iVpn_type_relation;
	int m_iVpn_type_friend;

	int m_iVpn_type_local_nomber;
	int m_iVpn_type_local_calling_center;
	int m_iVpn_type_local_called_center;
	NbrTypeMgr nbrMgr;

private:
	static VpnCheckEx* m_gpVpnCheckEx;
};

#endif
