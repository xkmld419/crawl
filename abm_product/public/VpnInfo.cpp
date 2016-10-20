/*VER: 3*/ 
#include "VpnInfo.h"


//��̬����
bool VpnInfoBase::m_bAttached = false;

USERINFO_SHM_DATA_TYPE<VpnInfo> *VpnInfoBase::m_poVpnData = 0;
USERINFO_SHM_DATA_TYPE<VpnMemberInfo> *VpnInfoBase::m_poVpnMemberData = 0;
USERINFO_SHM_DATA_TYPE<VpnAndVpnInfo> *VpnInfoBase::m_poVpnAndVpnData = 0;
USERINFO_SHM_DATA_TYPE<VpnFriendNumInfo> *VpnInfoBase::m_poVpnFriendNumData = 0;

USERINFO_SHM_STR_INDEX_TYPE * VpnInfoBase::m_poVpnIndex = 0;
USERINFO_SHM_STR_INDEX_TYPE * VpnInfoBase::m_poVpnMemberIndex = 0;
USERINFO_SHM_STR_INDEX_TYPE * VpnInfoBase::m_poVpnMemberIndex2 = 0;
USERINFO_SHM_STR_INDEX_TYPE * VpnInfoBase::m_poVpnMember999Index = 0;
USERINFO_SHM_STR_INDEX_TYPE * VpnInfoBase::m_poVpnAndVpnIndex = 0;
USERINFO_SHM_STR_INDEX_TYPE * VpnInfoBase::m_poVpnFriendNumIndex = 0;
USERINFO_SHM_STR_INDEX_TYPE * VpnInfoBase::m_poVpnFriendNumIndex2 = 0;

VpnInfo * VpnInfoBase::m_poVpnInfo = 0;
VpnMemberInfo * VpnInfoBase::m_poVpnMemberInfo = 0;
VpnAndVpnInfo * VpnInfoBase::m_poVpnAndVpnInfo = 0;
VpnFriendNumInfo * VpnInfoBase::m_poVpnFriendNumInfo = 0;

//���캯��
void VpnInfoBase::loadSHMInfo()
{
}

//##ModelId=4247679B031B
VpnInfoBase::VpnInfoBase()
{
	
	if (m_bAttached)
		return;

	m_bAttached = true;

	//VPN��Ϣ����
	USERINFO_ATTACH_DATA(m_poVpnData, VpnInfo, VPN_INFO_DATA);
	//VPN��Ա����
	USERINFO_ATTACH_DATA(m_poVpnMemberData, VpnMemberInfo, VPN_MEMBER_DATA);
	//VPN��VPN��ϵ����
	USERINFO_ATTACH_DATA(m_poVpnAndVpnData, VpnAndVpnInfo, VPN_ANDVPN_DATA);
	//VPN�����������
	USERINFO_ATTACH_DATA(m_poVpnFriendNumData, VpnFriendNumInfo, VPN_FRIENDNUM_DATA);

	//VPN��Ϣ����
	USERINFO_ATTACH_STRING_INDEX(m_poVpnIndex ,VPN_INFO_INDEX);
	//VPN��Ա����
	USERINFO_ATTACH_STRING_INDEX(m_poVpnMemberIndex ,VPN_MEMBER_INDEX);
	USERINFO_ATTACH_STRING_INDEX(m_poVpnMemberIndex2 ,VPN_MEMBER_INDEX2);
	USERINFO_ATTACH_STRING_INDEX(m_poVpnMember999Index ,VPN_MEMBER_999_INDEX);
	//VPN��VPN��ϵ����
	USERINFO_ATTACH_STRING_INDEX(m_poVpnAndVpnIndex ,VPN_ANDVPN_INDEX);
	//VPN���������
	USERINFO_ATTACH_STRING_INDEX(m_poVpnFriendNumIndex ,VPN_FRIENDNUM_INDEX);
	USERINFO_ATTACH_STRING_INDEX(m_poVpnFriendNumIndex2 ,VPN_FRIENDNUM_INDEX2);

	if (m_bAttached)
		bindData ();
}

//�ͷ�
void VpnInfoBase::freeAll()
{
	#define FREE(X) \
		if (X) { \
			delete X; \
			X = 0; \
		}

	FREE(m_poVpnData);
	FREE(m_poVpnMemberData);
	FREE(m_poVpnAndVpnData);
	FREE(m_poVpnFriendNumData);
	FREE(m_poVpnIndex);
	FREE(m_poVpnMemberIndex);
	FREE(m_poVpnMemberIndex2);
	FREE(m_poVpnMember999Index);
	FREE(m_poVpnAndVpnIndex);
	FREE(m_poVpnFriendNumIndex);
	FREE(m_poVpnFriendNumIndex2);
}

//��
void VpnInfoBase::bindData()
{
    m_poVpnInfo = (VpnInfo *)(*m_poVpnData);
    m_poVpnMemberInfo = (VpnMemberInfo *)(*m_poVpnMemberData);
    m_poVpnAndVpnInfo = (VpnAndVpnInfo *)(*m_poVpnAndVpnData);
    m_poVpnFriendNumInfo = (VpnFriendNumInfo *)(*m_poVpnFriendNumData);
}

//����
VpnInfoBase::operator bool()
{
	return m_bAttached;
}
