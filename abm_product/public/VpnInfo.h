/*VER: 3*/ 
// Copyright (c) 2008,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef VPNINFO_H_HEADER_INCLUDED_
#define VPNINFO_H_HEADER_INCLUDED_

#include "SHMData.h"
#include "SHMData_A.h"
#include "SHMStringTreeIndex.h"
#include "SHMIntHashIndex.h"
#include "SHMStringTreeIndex_A.h"
#include "SHMIntHashIndex_A.h"
#include "IpcKey.h"

#ifdef  USERINFO_REUSE_MODE
///////�� USERINFO_REUSE_MODE
    #define USERINFO_SHM_DATA_TYPE  SHMData_A
    #define USERINFO_SHM_INT_INDEX_TYPE  SHMIntHashIndex_A
    #define USERINFO_SHM_STR_INDEX_TYPE  SHMStringTreeIndex_A
	#define USERINFO_ATTACH_DATA(X, Y, Z) \
		X = new SHMData_A<Y> (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}

	#define USERINFO_ATTACH_INT_INDEX(X, Z) \
		X = new SHMIntHashIndex_A (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}

	#define USERINFO_ATTACH_STRING_INDEX(X, Z) \
		X = new SHMStringTreeIndex_A (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}


#else
///////�ر� USERINFO_REUSE_MODE 
    #define USERINFO_SHM_DATA_TYPE  SHMData
    #define USERINFO_SHM_INT_INDEX_TYPE  SHMIntHashIndex
    #define USERINFO_SHM_STR_INDEX_TYPE  SHMStringTreeIndex
	#define USERINFO_ATTACH_DATA(X, Y, Z) \
		X = new SHMData<Y> (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}

	#define USERINFO_ATTACH_INT_INDEX(X, Z) \
		X = new SHMIntHashIndex (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}

	#define USERINFO_ATTACH_STRING_INDEX(X, Z) \
		X = new SHMStringTreeIndex (Z); \
		if (!(X)) { \
			freeAll (); \
			THROW(MBC_UserInfo+1); \
		} \
		if (!(X->exist())) { \
			m_bAttached = false; \
			printf("%s:%d: ATTACE_DATA failed!\n",__FILE__, __LINE__); \
		}
#endif

#define VPN_INFO_DATA                     IpcKeyMgr::getIpcKey (-1,"SHM_VPN_INFO_DATA")
#define VPN_MEMBER_DATA                   IpcKeyMgr::getIpcKey (-1,"SHM_VPN_MEMBER_DATA")
#define VPN_ANDVPN_DATA                   IpcKeyMgr::getIpcKey (-1,"SHM_VPN_ANDVPN_DATA")
#define VPN_FRIENDNUM_DATA                IpcKeyMgr::getIpcKey (-1,"SHM_VPN_FRIENDNUM_DATA")

#define VPN_INFO_INDEX                    IpcKeyMgr::getIpcKey (-1,"SHM_VPN_INFO_INDEX")
#define VPN_MEMBER_INDEX                  IpcKeyMgr::getIpcKey (-1,"SHM_VPN_MEMBER_INDEX")
#define VPN_MEMBER_INDEX2                 IpcKeyMgr::getIpcKey (-1,"SHM_VPN_VPNHAVEMEMBER_INDEX")
#define VPN_MEMBER_999_INDEX              IpcKeyMgr::getIpcKey (-1,"SHM_VPN_VPNHAVEMEMBER999_INDEX")
#define VPN_ANDVPN_INDEX                  IpcKeyMgr::getIpcKey (-1,"SHM_VPN_ANDVPN_INDEX")
#define VPN_FRIENDNUM_INDEX               IpcKeyMgr::getIpcKey (-1,"SHM_VPN_FRIENDNUM_INDEX")
#define VPN_FRIENDNUM_INDEX2              IpcKeyMgr::getIpcKey (-1,"SHM_VPN_VPNHAVEFRIENDNUM_INDEX")



//VPN��Ϣ
class VpnInfo
{
public:
    //VPN���
    char m_sVpnCode[21];

    //VPN����
    char m_sVpnName[242];

	//VPN���� 1 ������ 2 �籾����
	char m_sVpnType;

	//��Чʱ��
	char m_sEffDate[16];

	//ʧЧʱ��
	char m_sExpDate[16];
};

//VPN���Ա��ϵ
class VpnMemberInfo
{
public:
	//VPN��Ա��ϵseq
	long m_lVpnMemberID;

    //VPN���
    char m_sVpnCode[21];

    //VPN��Ա
    char m_sVpnMember[21];

	//VPN��Ա���� 1������ 2 VPN
    char m_sVpnType;

	//VPN��Ա��ɫ
	char m_sVpnMemberRole[10];

	//��Чʱ��
	char m_sEffDate[16];

	//ʧЧʱ��
	char m_sExpDate[16];

	//VPN��ŵ���һ����Ա
    unsigned int m_iVpnNextOffset;

	//VPN��Ա��Ӧ����һ����¼
    unsigned int m_iMemberNextVpnOffset;
};

//VPN��VPN��ϵ
class VpnAndVpnInfo
{
public:
    //VPN���
    char m_sVpnCodeA[21];

    //VPN���
    char m_sVpnCodeB[21];

	//��Чʱ��
	char m_sEffDate[16];

	//ʧЧʱ��
	char m_sExpDate[16];

	//��ϵ����һ����¼
	unsigned int m_iNextOffset;
};

//VPN��������ϵ
class VpnFriendNumInfo
{
public:
    //VPN���
    char m_sVpnCode[21];

    //VPN��Ա
    char m_sFriendNum[21];

	//��Чʱ��
	char m_sEffDate[16];

	//ʧЧʱ��
	char m_sExpDate[16];

	//VPN��Ŷ�Ӧ����һ�������
    unsigned int m_iVpnNextOffset;
	//һ��������Ӧ����һ����¼
    unsigned int m_iNumNextOffset;
};

class VpnInfoBase
{
  public:
    VpnInfoBase();
    
    operator bool();

  protected:
    
    void bindData();

    //Vpn��Ϣ������
    static USERINFO_SHM_DATA_TYPE<VpnInfo> *m_poVpnData;

    //Vpn���Ա��ϵ������
    static USERINFO_SHM_DATA_TYPE<VpnMemberInfo> *m_poVpnMemberData;

    //Vpn��VPn��ϵ������
    static USERINFO_SHM_DATA_TYPE<VpnAndVpnInfo> *m_poVpnAndVpnData;

    //Vpn��������ϵ������
    static USERINFO_SHM_DATA_TYPE<VpnFriendNumInfo> *m_poVpnFriendNumData;

    //Vpn��Ϣ����������������VPN��ŵ�����
    static USERINFO_SHM_STR_INDEX_TYPE *m_poVpnIndex;

    //Vpn��Ϣ����������������VPN��Ա������
    static USERINFO_SHM_STR_INDEX_TYPE *m_poVpnMemberIndex;
	//Vpn��Ϣ����������������VPN�¸�����Ա������
	static USERINFO_SHM_STR_INDEX_TYPE *m_poVpnMemberIndex2;
	//Vpn��Ϣ��������������ɫ999��Ա������
	static USERINFO_SHM_STR_INDEX_TYPE *m_poVpnMember999Index;

    //Vpn��Vpn��ϵ��Ϣ��������������������VPN��Ź�ϵ������
    static USERINFO_SHM_STR_INDEX_TYPE *m_poVpnAndVpnIndex;

    //Vpn��������ϵ��Ϣ��������������������������
    static USERINFO_SHM_STR_INDEX_TYPE *m_poVpnFriendNumIndex;
    //Vpn��������ϵ��Ϣ������������VPN�¸�������������
    static USERINFO_SHM_STR_INDEX_TYPE *m_poVpnFriendNumIndex2;

	//�󶨱���
    static VpnInfo *m_poVpnInfo;

    static VpnMemberInfo *m_poVpnMemberInfo;

    static VpnAndVpnInfo *m_poVpnAndVpnInfo;

    static VpnFriendNumInfo *m_poVpnFriendNumInfo;

  private:
    //���ع����ڴ���Ϣ
    void loadSHMInfo();
    //�ͷ�
    void freeAll();

public:
	static bool m_bAttached;

};

#endif /* VPNINFO_H_HEADER_INCLUDED_ */
