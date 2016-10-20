#pragma once
#ifndef _SHMDataMgr_HEAD_
#define  _SHMDataMgr_HEAD_
#include "SgwDefine.h"
#include "ShmParamDef.h"
#include "CSemaphore.h"
#include "SHMSGWInfoBase.h"

//��ȡ�����ڴ���Ĳ�����X�Ǿֲ�����,Y��ȫ�ֱ���
int  GETVALUE(int X,int Y);

class SHMDataMgr:public SHMSGWInfoBase
{
public:
	//////////��context_filter
	//���������ڴ���������������
	static int createContentFilteringSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadContentFilteringSHM();
	//���ݼ���
	static int loadContentFiltering();
	//��չ����ڴ�����
	static void clearContentFilteringSHM() {
		m_pSHMMgrContextData->clear();
		//m_pSHMMgrContextIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachContentFiltering() {
		m_pSHMMgrContextData->close();
		//m_pSHMMgrContextIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryContentFilteringData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	//static int queryContentFilteringIndex(SHMInfo &_oShmInfo);

	//////////table tab_msg_map
	//���������ڴ���������������
	static int createMsgMapSHM(int _iExtenSzie = 0);
	//���¹����ڴ�
	//static int updateMsgMapSHM();
	//ж�ع����ڴ�
	static void unloadMsgMapSHM();
	//���ݼ���
	static int loadMsgMap();
	//��չ����ڴ�����
	static void clearMsgMapSHM() {
		m_pSHMMgrMsgMapInfoData->clear();
		m_pSHMMgrMsgMapInfoIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachMsgMap() {
		m_pSHMMgrMsgMapInfoData->close();
		m_pSHMMgrMsgMapInfoIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryMsgMapData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryMsgMapIndex(SHMInfo &_oShmInfo);

	/////////service_context_list
	//���������ڴ���������������
	static int createServiceTxtListSHM(int _iExtenSzie = 0);
	//���¹����ڴ�
	//static int updateServiceTxtListSHM();
	//ж�ع����ڴ�
	static void unloadServiceTxtListSHM();
	//���ݼ���
	static int loadServiceTxtList();
	//��չ����ڴ�����
	static void clearServiceTxtListSHM() {
		m_pSHMMgrServiceTxtListData->clear();
		m_pSHMMgrServiceTxtListIndex->empty();
		m_pSHMMgrSrvTxtListIDIndex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachServiceTxtList() {
		m_pSHMMgrServiceTxtListData->close();
		m_pSHMMgrServiceTxtListIndex->close();
		m_pSHMMgrSrvTxtListIDIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryServiceTxtListData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryServiceTxtListIndex(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySrvTxtListIDIndex(SHMInfo &_oShmInfo);

	//////////////��stream_ctrl_info
	//���������ڴ���������������
	static int createStreamCtrlSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadStreamCtrlSHM();
	//���ݼ���
	static int loadStreamCtrl();
	//��չ����ڴ�����
	static void clearStreamCtrlSHM() {
		m_pSHMMgrStreamCtrlInfoData->clear();
		m_pSHMMgrStreamCtrlInfoIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachStreamCtrl() {
		m_pSHMMgrStreamCtrlInfoData->close();
		m_pSHMMgrStreamCtrlInfoIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryStreamCtrlData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryStreamCtrlIndex(SHMInfo &_oShmInfo);

	///////////��stream_ctrl_data
	//���������ڴ���������������
	static int createStreamCtrlDataSHM(int _iExtenSzie = 0);
	//���¹����ڴ�
	//static int updateStreamCtrlDataSHM();
	//ж�ع����ڴ�
	static void unloadStreamCtrlDataSHM();
	//�������ݵ������ڴ�
	static int loadStreamCtrlData();
	//��չ����ڴ�����
	static void clearStreamCtrlDataSHM() {
		m_pSHMMgrStreamCtrlData->clear();
		m_pSHMMgrStreamCtrlDataIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachStreamCtrlData() {
		m_pSHMMgrStreamCtrlData->close();
		m_pSHMMgrStreamCtrlDataIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryStreamCtrlDataData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryStreamCtrlDataIndex(SHMInfo &_oShmInfo);

	/////////userinfo
	//���������ڴ���������������
	static int createUserInfoSHM(int _iExtenSzie = 0);
	//���¹����ڴ�
	//static int updateUserInfoSHM();
	//ж�ع����ڴ�
	static void unloadUserInfoSHM();
	//���ݼ���
	static int loadUserInfo();
	//��չ����ڴ�����
	static void clearUserInfoSHM() {
		m_pSHMMgrUserInfoData->clear();
		m_pSHMMgrUserInfoIndex->empty();
		m_pSHMMgrUserInfoIndex_A->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachUserInfo() {
		m_pSHMMgrUserInfoData->close();
		m_pSHMMgrUserInfoIndex->close();
		m_pSHMMgrUserInfoIndex_A->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryUserInfoData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryUserInfoIndex(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryUserInfoIndex_A(SHMInfo &_oShmInfo);

	//////////////table sgw_sla_que_rel
	//���������ڴ���������������
	static int createSgwSlaqueRelSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadSgwSlaqueRelSHM();
	//���ݼ���
	static int loadSgwSlaqueRel();
	//��չ����ڴ�����
	static void clearSgwSlaqueRelSHM() {
		m_pSHMMgrSgwSlaqueRelData->clear();
		m_pSHMMgrSgwSlaqueRelIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachSgwSlaqueRel() {
		m_pSHMMgrSgwSlaqueRelData->close();
		m_pSHMMgrSgwSlaqueRelIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querySgwSlaqueRelData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySgwSlaqueRelIndex(SHMInfo &_oShmInfo);

	////////////table netInfodata
	static int createNetSHM(int _iExtenSzie = 0);
	//���¹����ڴ�
	//static int updateNetSHM();
	//ж�ع����ڴ�
	static void unloadNetSHM();
	//���ݼ���
	static int loadNet();
	//��չ����ڴ�����
	static void clearNetSHM() {
		m_pSHMMgrNetInfoData->clear();
		m_pSHMMgrNetInfoIndex->empty();
		m_pSHMMgrNetInfoIndex_S->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachNet() {
		m_pSHMMgrNetInfoData->close();
		m_pSHMMgrNetInfoIndex->close();
		m_pSHMMgrNetInfoIndex_S->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryNetData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryNetIndex(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryNetIndex_s(SHMInfo &_oShmInfo);

	///////////////sessiondata
	//���������ڴ���������������
	static int createSessionSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadSessionSHM();
	//��չ����ڴ�����
	static void clearSessionSHM() {
		m_pSHMMgrSessionData->clear();
		m_pSHMMgrSessionIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachSession() {
		m_pSHMMgrSessionData->close();
		m_pSHMMgrSessionIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querySessionData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySessionIndex(SHMInfo &_oShmInfo);

	///////sessionclear
	static int createSessionClearSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�����
	static void unloadSessionClearSHM();
	//��չ����ڴ�����
	static void clearSessionClearSHM() {
		m_pSHMMgrSessionClearData->clear();
		m_pSHMMgrSessionClearIndex->empty();
		m_pMgrSessionClearData = (SessionClearData *)(*m_pSHMMgrSessionClearData);
		m_pMgrSessionHeadSign = (SessionHead *)(char *)m_pMgrSessionClearData;
		m_pMgrSessionHeadSign[0].m_iFree = 1;//����ͷ
		int iTotal = m_pSHMMgrSessionClearData->getTotal();
		for(int i=1; i<iTotal; i++){
			m_pMgrSessionClearData[i].m_iNext = i+1;
			m_pMgrSessionClearData[i].m_iIndexNext = 0;
		}

		//���һ�����⴦��
		m_pMgrSessionClearData[iTotal].m_iNext = 1;
		m_pMgrSessionClearData[iTotal].m_iIndexNext = 0;
		m_pMgrSessionHeadSign[0].m_iPreDeal =1;
	};
	//��ѯ�����ڴ���������Ϣ
	static int querySessionClearData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySessionClearIndex(SHMInfo &_oShmInfo);

	//////////////table shmcongestlevel
	//���������ڴ���������������
	static int createCongestLeveSHM(int _iExtenSzie = 0);
	//���¹����ڴ�
	//static int updateCongestLeveSHM();
	//ж�ع����ڴ�
	static void unloadCongestLeveSHM();
	//��չ����ڴ�����
	static void clearCongestLeveSHM() {
		m_pSHMMgrCongestLevelData->clear();
	};
	//�Ͽ��빲���ڴ������
	static int detachCongestLeve() {
		m_pSHMMgrCongestLevelData->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryCongestLeveData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryCongestLeveIndex(SHMInfo &_oShmInfo);

	///////////service_context_list_base
	//���������ڴ�
	static int createServiceTxtListBaseSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadServiceTxtListBaseSHM();
	//���ݼ���
	static int loadServiceTxtListBase();
	//��չ����ڴ�����
	static void clearServiceTxtListBaseSHM() {
		m_pSHMMgrServiceTxtListBaseData->clear();
		m_pSHMMgrServiceTxtListBaseIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachServiceTxtListBase() {
		m_pSHMMgrServiceTxtListBaseData->close();
		m_pSHMMgrServiceTxtListBaseIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryServiceTxtListBaseData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryServiceTxtListBaseIndex(SHMInfo &_oShmInfo);

	//////////table LoadBalanceData
	//���������ڴ�
	static int createloadbalanceSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadloadbalanceSHM();
	//��ȡ���Ĳ���
	static int loadBalance();
	//��չ����ڴ�����
	static void clearloadbalanceSHM() {
		m_pSHMMgrLoadBalanceData->clear();
		m_pSHMMgrLoadBalanceIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachloadbalance() {
		m_pSHMMgrLoadBalanceData->close();
		m_pSHMMgrLoadBalanceIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryloadbalanceData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryloadbalanceIndex(SHMInfo &_oShmInfo);

	//////////////table base_method_list
	static int createbasemethodSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadbasemethodSHM();
	//��չ����ڴ�����
	static void clearbasemethodSHM() {
		m_pSHMMgrBaseMethodData->clear();
		m_pSHMMgrBaseMethodindex->empty();
	};
	//���ݼ���
	static int loadbasemethod();
	//�Ͽ��빲���ڴ������
	static int detachbasemethod() {
		m_pSHMMgrBaseMethodData->close();
		m_pSHMMgrBaseMethodindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querybasemethodData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querybasemethodIndex(SHMInfo &_oShmInfo);

	////////others CPacketSession
	static int createpacketsessionSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadpacketsessionSHM();
	//��չ����ڴ�����
	static void clearpacketsessionSHM() {
		m_pSHMMgrPacketSessionData->clear();
		m_pSHMMgrPacketSessionindex->empty();
		m_pSHMMgrPacketSubSessionindex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachpacketsession() {
		m_pSHMMgrPacketSessionData->close();
		m_pSHMMgrPacketSessionindex->close();
		m_pSHMMgrPacketSubSessionindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querypacketsessionData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querypacketsessionIndex(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querypacketsubsessionIndex(SHMInfo &_oShmInfo);

	////////table service_package_route
	static int createpackagerouteSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadpackagerouteSHM();
	//��չ����ڴ�����
	static void clearpackagerouteSHM() {
		m_pSHMMgrServicePackageRouteData->clear();
		m_pSHMMgrServicePackageRouteindex->empty();
		m_pSHMMgrSrvPackageRouteSeqIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachpackageroute() {
		m_pSHMMgrServicePackageRouteData->close();
		m_pSHMMgrServicePackageRouteindex->close();
		m_pSHMMgrSrvPackageRouteSeqIndex->close();
	};
	//���ݼ���
	static int loadpackageroute();
	//��ѯ�����ڴ���������Ϣ
	static int querypackagerouteData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querypackagerouteIndex(SHMInfo &_oShmInfo);
	static int querypackagerouteIndex_a(SHMInfo &_oShmInfo);

	////////service_package_node
	static int createservicepackageSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadservicepackageSHM();
	//���ݼ���
	static int loadservicepackage();
	//��չ����ڴ�����
	static void clearservicepackageSHM() {
		m_pSHMMgrServicePackageData->clear();
		m_pSHMMgrServicePackageindex->empty();
		m_pSHMMgrSrvPackageNodeSeqIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachservicepackage() {
		m_pSHMMgrServicePackageData->close();
		m_pSHMMgrServicePackageindex->close();
		m_pSHMMgrSrvPackageNodeSeqIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryservicepackageData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryservicepackageIndex(SHMInfo &_oShmInfo);
	static int queryservicepackageIndex_a(SHMInfo &_oShmInfo);

	////////table service_package_route_ccr
	static int createservicepackageCCRSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadservicepackageCCRSHM();
	//���ݼ���
	static int loadservicepackageCCR();
	//��չ����ڴ�����
	static void clearservicepackageCCRSHM() {
		m_pSHMMgrServicePackageRouteCCRData->clear();
		m_pSHMMgrSrvPackageRouteCCRSeqindex->empty();
		m_pSHMMgrSrvPackageRouteSeqindex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachservicepackageCCR() {
		m_pSHMMgrServicePackageRouteCCRData->close();
		m_pSHMMgrSrvPackageRouteCCRSeqindex->close();
		m_pSHMMgrSrvPackageRouteSeqindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryservicepackageCCRData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryservicepackageCCRIndex(SHMInfo &_oShmInfo);
	static int queryservicepackageCCRIndex_a(SHMInfo &_oShmInfo);

	////////table service_package_judge
	static int createpackagejudgeSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadpackagejudgeSHM();
	//���ݼ���
	static int loadpackagejudge();
	//��չ����ڴ�����
	static void clearpackagejudgeSHM() {
		m_pSHMMgrServicePackageJudgeData->clear();
		m_pSHMMgrServicePackageJudgeindex->empty();
		m_pSHMMgrSrvPackageJudgeNodeSeqIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachpackagejudge() {
		m_pSHMMgrServicePackageJudgeData->close();
		m_pSHMMgrServicePackageJudgeindex->close();
		m_pSHMMgrSrvPackageJudgeNodeSeqIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querypackagejudgeData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querypackagejudgeIndex(SHMInfo &_oShmInfo);
	static int querypackagejudgeIndex_a(SHMInfo &_oShmInfo);

	////////table service_package_cca
	static int createpackageccaSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadpackageccaSHM();
	//���ݼ���
	static int loadpackagecca();
	//��չ����ڴ�����
	static void clearpackageccaSHM() {
		m_pSHMMgrServicePackageCCAData->clear();
		m_pSHMMgrServicePackageCCAindex->empty();
		m_pSHMMgrSrvPackageCCASeqIndex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachpackagecca() {
		m_pSHMMgrServicePackageCCAData->close();
		m_pSHMMgrServicePackageCCAindex->close();
		m_pSHMMgrSrvPackageCCASeqIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querypackageccaData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querypackageccaIndex(SHMInfo &_oShmInfo);
	static int querypackageccaIndex_a(SHMInfo &_oShmInfo);

	////////table service_package_variable
	static int createpackagevleSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadpackagevleSHM();
	//���ݼ���
	static int loadpackagevle();
	//��չ����ڴ�����
	static void clearpackagevleSHM() {
		m_pSHMMgrServicePackageVariableData->clear();
		m_pSHMMgrServicePackageVariableindex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachpackagevle() {
		m_pSHMMgrServicePackageVariableData->close();
		m_pSHMMgrServicePackageVariableindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querypackagevleData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querypackagevleIndex(SHMInfo &_oShmInfo);

	////////table service_package_func
	static int createpackagefuncSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadpackagefuncSHM();
	//���ݼ���
	static int loadpackagefunc();
	//��չ����ڴ�����
	static void clearpackagefuncSHM() {
		m_pSHMMgrServicePackageFuncData->clear();
		m_pSHMMgrServicePackageFuncindex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachpackagefunc() {
		m_pSHMMgrServicePackageFuncData->close();
		m_pSHMMgrServicePackageFuncindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querypackagefuncData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querypackagefuncIndex(SHMInfo &_oShmInfo);

	////////�����ڴ���Ϣͳ��
	static int createscongestmsgSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadscongestmsgSHM();
	//��չ����ڴ�����
	static void clearscongestmsgSHM() {
		m_pSHMMgrSCongestMsgData->clear();
		m_pSHMMgrSCongestMsgindex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachscongestmsg() {
		m_pSHMMgrSCongestMsgData->close();
		m_pSHMMgrSCongestMsgindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryscongestmsgData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryscongestmsgIndex(SHMInfo &_oShmInfo);

	////////wf_process_mq
	static int createwfprocessmqSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadwfprocessmqSHM();
	//���ݼ���
	static int loadwfprocessmq();
	//��չ����ڴ�����
	static void clearwfprocessmqSHM() {
		m_pSHMMgrWfprocessMqData->clear();
		m_pSHMMgrWfprocessMqindex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachwfprocessmq() {
		m_pSHMMgrWfprocessMqData->close();
		m_pSHMMgrWfprocessMqindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querywfprocessmqData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querywfprocessmqIndex(SHMInfo &_oShmInfo);

	////////portinfo
	static int createportinfoSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadportinfoSHM();
	//��չ����ڴ�����
	static void clearportinfoSHM() {
		m_pSHMMgrPortInfoData->clear();
		m_pSHMMgrPortInfoindex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachportinfo() {
		m_pSHMMgrPortInfoData->close();
		m_pSHMMgrPortInfoindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryportinfoData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryportinfoIndex(SHMInfo &_oShmInfo);

	////////sgw_org_route
	static int createOrgRouteSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadOrgRouteSHM();
	//���ݼ���
	static int loadOrgRouteData();
	//��չ����ڴ�����
	static void clearOrgRouteSHM() {
		m_pSHMMgrOrgRouteData->clear();
		m_pSHMMgrOrgRouteindex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachOrgRouteSHM() {
		m_pSHMMgrOrgRouteData->close();
		m_pSHMMgrOrgRouteindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryOrgRouteData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryOrgRouteIndex(SHMInfo &_oShmInfo);

	////////sgw_org_head
	static int createOrgHeadSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadOrgHeadSHM();
	//��չ����ڴ�����
	static void clearOrgHeadSHM() {
		m_pSHMMgrOrgHeadData->clear();
		m_pSHMMgrOrgHeadindex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachOrgHeadSHM() {
		m_pSHMMgrOrgHeadData->close();
		m_pSHMMgrOrgHeadindex->close();
	};
	static int loadOrgHeadData();
	//��ѯ�����ڴ���������Ϣ
	static int queryOrgHeadData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryOrgHeadIndex(SHMInfo &_oShmInfo);

	////////sgw_global_map
	static int createGlobalMapSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadGlobalMapSHM();
	//���ݼ���
	static int loadGlobalMapData();
	//��չ����ڴ�����
	static void clearGlobalMapSHM() {
		m_pSHMMgrGlobalMapData->clear();
		m_pSHMMgrGlobalMapindex->empty();
	};
	//�Ͽ��빲���ڴ������
	static int detachGlobalMapSHM() {
		m_pSHMMgrGlobalMapData->close();
		m_pSHMMgrGlobalMapindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryGlobalMapData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryGlobalMapIndex(SHMInfo &_oShmInfo);

	////////channel_info
	static int createChannelSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadChannelSHM();
	//���ݼ���
	static int loadChannelData();
	//��չ����ڴ�����
	static void clearChannelSHM() {
		m_pSHMMgrChannelData->clear();
		m_pSHMMgrChannelindex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachChannelSHM() {
		m_pSHMMgrChannelData->close();
		m_pSHMMgrChannelindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryChannelData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryChannelIndex(SHMInfo &_oShmInfo);

	////////sgw_user_staff
	static int createUserStaffSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadUserStaffSHM();
	//���ݼ���
	static int loadUserStaffData();
	//��չ����ڴ�����
	static void clearUserStaffSHM() {
		m_pSHMMgrUserStaffData->clear();
		m_pSHMMgrUserStaffIDindex->empty();
		m_pSHMMgrStaffIDindex->empty();
		m_pSHMMgrUserStaffChannelIDindex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachUserStaffSHM() {
		m_pSHMMgrUserStaffData->close();
		m_pSHMMgrUserStaffIDindex->close();
		m_pSHMMgrStaffIDindex->close();
		m_pSHMMgrUserStaffChannelIDindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryUserStaffData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryUserStaffIndex_a(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryUserStaffIndex_b(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryUserStaffIndex_c(SHMInfo &_oShmInfo);

	////////sgw_route_rule
	static int createRouteRuleSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadRouteRuleSHM();
	//���ݼ���
	static int loadRouteRuleData();
	//��չ����ڴ�����
	static void clearRouteRuleSHM() {
		m_pSHMMgrOrgRoutRuleData->clear();
		m_pSHMMgrOrgRouteIDindex->empty();
		m_pSHMMgrOrgIDindex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachRouteRuleSHM() {
		m_pSHMMgrOrgRoutRuleData->close();
		m_pSHMMgrOrgRouteIDindex->close();
		m_pSHMMgrOrgIDindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryRouteRuleData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryRouteRuleIndex_a(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryRouteRuleIndex_b(SHMInfo &_oShmInfo);

	////////sgw_area_org
	static int createAreaOrgSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadAreaOrgSHM();
	//���ݼ���
	static int loadAreaOrgData();
	//��չ����ڴ�����
	static void clearAreaOrgSHM() {
		m_pSHMMgrAreaOrgData->clear();
		m_pSHMMgrAreaOrgSeqindex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachAreaOrgSHM() {
		m_pSHMMgrAreaOrgData->close();
		m_pSHMMgrAreaOrgSeqindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryAreaOrgData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryAreaOrgIndex(SHMInfo &_oShmInfo);


	////////sgw_tuxedo_relation_in
	static int createTuxRelaInSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadTuxRelaInSHM();
	//���ݼ���
	static int loadTuxRelaInData();
	//��չ����ڴ�����
	static void clearTuxRelaInSHM() {
		m_pSHMMgrTuxRelaInData->clear();
		m_pSHMMgrTuxInSvrsIDindex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachTuxRelaInSHM() {
		m_pSHMMgrTuxRelaInData->close();
		m_pSHMMgrTuxInSvrsIDindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryTuxRelaInData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryTuxRelaInIndex(SHMInfo &_oShmInfo);


	////////sgw_tuxedo_relation_out
	static int createTuxRelaOutSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadTuxRelaOutSHM();
	//���ݼ���
	static int loadTuxRelaOutData();
	//��չ����ڴ�����
	static void clearTuxRelaOutSHM() {
		m_pSHMMgrTuxRelaOutData->clear();
		m_pSHMMgrTuxOutSvrTxtIDindex->empty();
		m_pSHMMgrTuxOutRelaIDindex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachTuxRelaOutSHM() {
		m_pSHMMgrTuxRelaOutData->close();
		m_pSHMMgrTuxOutSvrTxtIDindex->close();
		m_pSHMMgrTuxOutRelaIDindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryTuxRelaOutData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryTuxRelaOutIndex(SHMInfo &_oShmInfo);
	static int queryTuxRelaOutRelaIDIndex(SHMInfo &_oShmInfo);

	////////sgw_services_list
	static int createSvrListSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadSvrListSHM();
	//���ݼ���
	static int loadSvrListData();
	//��չ����ڴ�����
	static void clearSvrListSHM() {
		m_pSHMMgrSrvsListData->clear();
		m_pSHMMgrSrvsListIDindex->empty();
		m_pSHMMgrSrvsListServicesNameindex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachSvrListSHM() {
		m_pSHMMgrSrvsListData->close();
		m_pSHMMgrSrvsListIDindex->close();
		m_pSHMMgrSrvsListServicesNameindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querySvrListData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySvrListIndex(SHMInfo &_oShmInfo);
	static int querySvrListServicesNameIndex(SHMInfo &_oShmInfo);


	////////sgw_services_param_relation
	static int createSvrsParamRelaSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadSvrsParamRelaSHM();
	//���ݼ���
	static int loadSvrsParamRelaData();
	//��չ����ڴ�����
	static void clearSvrsParamRelaSHM() {
		m_pSHMMgrSrvsParamRelaData->clear();
		m_pSHMMgrSrvsParamRelaindex->empty();
		m_pSHMMgrSrvsParamRelaMappingIDIndex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachSvrsParamRelaSHM() {
		m_pSHMMgrSrvsParamRelaData->close();
		m_pSHMMgrSrvsParamRelaindex->close();
		m_pSHMMgrSrvsParamRelaMappingIDIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querySvrsParamRelaData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySvrsParamRelaIndex(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySvrsParamRelaMappingIDIndex(SHMInfo &_oShmInfo);

	////////sgw_services_param_list
	static int createSvrsParamListSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadSvrsParamListSHM();
	//���ݼ���
	static int loadSvrsParamListData();
	//��չ����ڴ�����
	static void clearSvrsParamListSHM() {
		m_pSHMMgrSrvsParamListData->clear();
		m_pSHMMgrSrvsParamListindex->empty();
	}
	//�Ͽ��빲���ڴ������
	static void detachSvrsParamListSHM() {
		m_pSHMMgrSrvsParamListData->close();
		m_pSHMMgrSrvsParamListindex->close();
	}
	//��ѯ�����ڴ���������Ϣ
	static int querySvrsParamListData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySvrsParamListIndex(SHMInfo &_oShmInfo);

	////////sgw_service_register
	static int createSvrsRegisterSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadSvrsRegisterSHM();
	//���ݼ���
	static int loadSvrsRegisterData();
	//��չ����ڴ�����
	static void clearSvrsRegisterSHM() {
		m_pSHMMgrSrvsRegisterData->clear();
		m_pSHMMgrSrvsRegisterindex->empty();
		m_pSHMMgrSrvsRegisterSvrInsIDIndex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachSvrsRegisterSHM() {
		m_pSHMMgrSrvsRegisterData->close();
		m_pSHMMgrSrvsRegisterindex->close();
		m_pSHMMgrSrvsRegisterSvrInsIDIndex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querySvrsRegisterData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySvrsRegisterIndex(SHMInfo &_oShmInfo);
	static int querySvrsRegisterInsIDIndex(SHMInfo &_oShmInfo);


	////////sgw_param_instance_list
	static int createParamInsListSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadParamInsListSHM();
	//���ݼ���
	static int loadParamInsListData();
	//��չ����ڴ�����
	static void clearParamInsListSHM() {
		m_pSHMMgrParamInsListData->clear();
		m_pSHMMgrParamInsListindex->empty();
		m_pSHMMgrParamInsListMapIDindex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachParamInsListSHM() {
		m_pSHMMgrParamInsListData->close();
		m_pSHMMgrParamInsListindex->close();
		m_pSHMMgrParamInsListMapIDindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int queryParamInsListData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryParamInsListIndex(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int queryParamInsListMapIDIndex(SHMInfo &_oShmInfo);

	////////sgw_enum_area
	static int createSgwEnumAreaSHM(int _iExtenSzie = 0);
	//ж�ع����ڴ�
	static void unloadSgwEnumAreaSHM();
	//���ݼ���
	static int loadSgwEnumAreaData();
	//��չ����ڴ�����
	static void clearSgwEnumAreaSHM() {
		m_pSHMMgrSgwEnumAreaData->clear();
		m_pSHMMgrSgwEnumAreaindex->empty();
		m_pSHMMgrSgwEnumAreaOrgIDindex->empty();
	}
	//�Ͽ��빲���ڴ������
	static int detachSgwEnumAreaSHM() {
		m_pSHMMgrSgwEnumAreaData->close();
		m_pSHMMgrSgwEnumAreaindex->close();
		m_pSHMMgrSgwEnumAreaOrgIDindex->close();
	};
	//��ѯ�����ڴ���������Ϣ
	static int querySgwEnumAreaData(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySgwEnumAreaIndex(SHMInfo &_oShmInfo);
	//��ѯ�����ڴ���������Ϣ
	static int querySgwEnumAreaOrgIDIndex(SHMInfo &_oShmInfo);

public:
	//�������key����һ���Ĺ���ת��
	static long changekey(const char *_skey,int _iModValue);

	static unsigned long trim(const char *sHead);

};

#endif
