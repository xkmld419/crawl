#pragma once
#ifndef _SHMAPI_HEAD_
#define _SHMAPI_HEAD_
#include <vector>
#include "SHMSGWInfoBase.h"
#include "CSemaphore.h"
#include "SgwDefine.h"
#include "ShmParamDef.h"
#include "SHMDataMgr.h"
using namespace std;

//context_filter����ص������ڴ�
//�ṩ���ݹ���ģ��Ĺ����ڴ�ӿ�
class SHMContentFiltering:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool contentFilter_P(bool bWait = true){
		m_pSHMMgrContextDataLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool contentFilter_V(){
		m_pSHMMgrContextDataLock->V();
	};

	//����ֵ 0 �ɹ� ��0ʧ��
	static int GetContextFilterInfo(int _iFilterSeq, vector<ContextData> & vsContextInfo);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int AddContextFilterInfo(const ContextData & osContextInfo);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int DelContextFilterInfo(const ContextData & osContextInfo);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int UpdateContextFilterInfo(const ContextData & osContextInfo);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int GetContextFilterInfoAll(vector<ContextData> & vsContextInfo);
};

//TAB_MSG_MAP����ص������ڴ�
//�ṩ���ҵ����̲����ӿ�
class SHMMsgMapInfo:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool msgmap_P(bool bWait = true){
		m_pSHMMgrMsgMapInfoLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool msgmap_V(){
		m_pSHMMgrMsgMapInfoLock->V();
	};

	//�ṩ�����ѯ����������	����ֵ 0 �ɹ� ��0ʧ��
	static int GetMsgMapInfo(vector<SMsgMapInfo> & vsMsgMapInfo);
	//���ӽӿڰ���������ѯ
	static	int GetMsgMapInfo(int _iID ,SMsgMapInfo & vsMsgMapInfo);
	static int AddMsgMapInfo(SMsgMapInfo & vsMsgMapInfo);
	static int DelMsgMapInfo(SMsgMapInfo & vsMsgMapInfo);
	static int UpdateMsgMapInfo(SMsgMapInfo & vsMsgMapInfo);

};

//sgw_global_map����ص������ڴ�
//�ṩ���ҵ����̲����ӿ�
class SHMGlobalMap:public SHMSGWInfoBase
{
public:
	//�ṩ�����ѯ����������	����ֵ 0 �ɹ� ��0ʧ��
	static int GetGlobalMapALL(vector<SgwGlobalMap>&vGlobalMap);
	//���ӽӿ�
	//����������ѯ
	static int GetGlobalMap(int _iGID, SgwGlobalMap &sGlobalMap);
	static int AddGlobalMap(SgwGlobalMap &sGlobalMap);
	static int DelGlobalMap( SgwGlobalMap &sGlobalMap);
	static int UpdateGlobalMap( SgwGlobalMap &sGlobalMap);

};


//service_context_list����ص������ڴ�
//�ṩ���ҵ����̲����ӿ�
class SHMServiceTxtList:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool servicetxtlist_P(bool bWait = true){
		m_pSHMMgrServiceTxtListLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool servicetxtlist_V(){
		m_pSHMMgrServiceTxtListLock->V();
	};

	//����ֵ 0 �ɹ� ��0ʧ��
	static int GetServiceContextListAll(vector<ServiceContextListData> & vServiceContextListData);

	//����������ѯ
	static	int GetServiceContextList(int _iServiceContextListId,ServiceContextListData &sServiceContextListData);

	static int GetServiceContextList(const char *_strServiceContextId, vector<ServiceContextListData>& vServiceContextListData);

	//ͨ��_iUserSeq ����m_strServiceContextId������ֵ 0 �ɹ� ��0ʧ��
	static int GetServiceContextID(int _iUserSeq,vector<ServiceContextListData> &vServiceContextListData);

	//ͨ��_iUserSeq ����m_strServiceContextId������ֵ 0 �ɹ� ��0ʧ��
	static int GetServiceContextID(int _iUserSeq,const char *_strServiceContextId,ServiceContextListData &sServiceContextListData);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int DelServiceContextData(ServiceContextListData sServiceContextListData);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int UpdateServiceContextdata(ServiceContextListData sServiceContextListData);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int AddServiceContextData(ServiceContextListData sServiceContextListData);
};


//STREAM_CTRL_INFO����ص������ڴ�
//�ṩҵ����̲����ӿ�
//m_sUserName���ֶ�m_sSerConId�����������
class SHMStreamCtrlInfo:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool streamctl_P(bool bWait = true){
		m_pSHMMgrStreamCtrlInfoLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool streamctl_V(){
		m_pSHMMgrStreamCtrlInfoLock->V();
	};

	//����m_sUserName��m_sSerConIdΪ�������в��ң�����ֵ 0 �ɹ� ��0ʧ��
	static int GetStreamCtrlInfo(StreamControlInfo &tStreamCtrlInfo);
	//ȫ����ѯ
	static int GetStreamCtrlInfoAll(vector<StreamControlInfo> &tStreamCtrlInfo);
	//����m_sUserName��m_sSerConIdΪ��������ɾ��������ֵ 0 �ɹ� ��0ʧ��
	static int DelStreamCtrlInfo(StreamControlInfo &tStreamCtrlInfo);

	//����m_sUserName��m_sSerConIdΪ���������޸ģ�����ֵ 0 �ɹ� ��0ʧ��
	static int UpdateStreamCtrlInfo(StreamControlInfo &tStreamCtrlInfo);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int AddStreamCtrlInfo(StreamControlInfo &tStreamCtrlInfo);
};

//�ù����ڴ��ṩ����ҵ����̲����ӿڣ��ޱ�����
//�ṩҵ����̲����ӿ�
class SHMStreamCtrlData:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool streamctldata_P(bool bWait = true){
		m_pSHMMgrStreamCtrlDataLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool streamctldata_V(){
		m_pSHMMgrStreamCtrlDataLock->V();
	};

	static int GetStreamCtrlDataAll(vector<StreamControlData> &vStreamCtrlData);

	//����m_sUserName��m_sSerConIdΪ�������в��ң�����ֵ 0 �ɹ� ��0ʧ��
	static int GetStreamCtrlData(StreamControlData &tStreamCtrlData);

	//����m_sUserName��m_sSerConIdΪ��������ɾ��������ֵ 0 �ɹ� ��0ʧ��
	static int DelStreamCtrlData(StreamControlData &tStreamCtrlData);

	//����m_sUserName��m_sSerConIdΪ���������޸ģ�����ֵ 0 �ɹ� ��0ʧ��
	static int UpdateStreamCtrlData(StreamControlData &tStreamCtrlData);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int AddStreamCtrlData(StreamControlData &tStreamCtrlData);

	//����������ѯ
	static int GetStreamCtrlData(const char *sUserName,const char * sSerConId , long lMonth, StreamControlData &tStreamCtrlData);

};

//UserInfo����ص������ڴ�
//�ṩҵ����̲����ӿ�
class SHMUserInfoData:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool userinfo_P(bool bWait = true){
		m_pSHMMgrUserInfoDataLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool userinfo_V(){
		m_pSHMMgrUserInfoDataLock->V();
	};

	//��ѯ�ӿڣ��ɹ�����0��ʧ�ܷ��ط�0
	static int GetUserInfoData(const char *_sUserName,UserInfoData & vsUserInfoData);

	static int GetUserInfoDataAll(vector<UserInfoData> & vsUserInfoData);

	static int AddUserInfoData(UserInfoData & tUserInfoData);

	static int UpdateUserInfoData(UserInfoData & tUserInfoData);

	static int GetUserInfoData(int iUserSeq,UserInfoData & vsUserInfoData);

	//����USERSEQ��ɾ��
	static int DelUserInfoData(const UserInfoData & vsUserInfoData);

	//����user_name��ɾ��
	static int DeleteUserInfoData(const UserInfoData & vsUserInfoData);

};

//SGW_SLA_QUE_REL����ص������ڴ�
//�ṩҵ����̲����ӿ�
class SHMSgwSlaqueRel:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool sgwslaquerel_P(bool bWait = true){
		m_pSHMMgrSgwSlaqueRelLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool sgwslaquerel_V(){
		m_pSHMMgrSgwSlaqueRelLock->V();
	};

	//��ѯ�ӿڣ��ɹ�����0��ʧ�ܷ��ط�0
	static int GetSlaQueueAllData(vector<SgwSlaQueue> & vSgwSlaQueue);
	//����ֵ 0 �ɹ� ��0ʧ��
	static	int GetSlaQueueData(SgwSlaQueue & sSgwSlaQueue);
	//����ֵ 0 �ɹ� ��0ʧ��
	static	int DelSlaQueueData(SgwSlaQueue sSgwSlaQueue);
	//����ֵ 0 �ɹ� ��0ʧ��
	static	int UpdateSlaQueueData(SgwSlaQueue sSgwSlaQueue);
	//����ֵ 0 �ɹ� ��0ʧ��
	static	int AddSlaQueueData(SgwSlaQueue sSgwSlaQueue);
};

//NetInfoData����ص������ڴ�
//�ṩҵ����̲����ӿ�
class SHMNetInfoData:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool netinfo_P(bool bWait = true){
		m_pSHMMgrNetInfoLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool netinfo_V(){
		m_pSHMMgrNetInfoLock->V();
	};

	//�ṩ�����Լ���������
	static int GetNetInfoAllData(vector<NetInfoData> & vNetInfoData);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int GetNetInfoData(NetInfoData &sNetInfoData);

	static int GetNetInfoData(const char *_strNetinfoName,NetInfoData &oNetInfoData);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int AddNetInfoData(const NetInfoData &sNetInfoData);

	static int UpdateNetInfoData(const NetInfoData &sNetInfoData);

	//���ӽӿ�
	//����������ѯ
	static	int DelNetInfoData(const NetInfoData &sNetInfoData);

};

//SessionData����ص������ڴ�
//�ṩҵ����̲����ӿ�
class SHMSessionData:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool session_P(bool bWait = true){
		m_pSHMMgrSessionLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool session_V(){
		m_pSHMMgrSessionLock->V();
	};

	//�ṩ�����Լ���������
	static int GetSessionData(SessionData &tSessionData);

	static int DelSessionData(SessionData &tSessionData,bool _bCopy = false);

	static int UpdateSessionData(SessionData &tSessionData);

	static int AddSessionData(SessionData &tSessionData);

	//���±��ѯ����
	static int GetSessionDataALL(vector<SessionData> &vSessionData);
};

//�ṩҵ����̲����ӿ�
class SHMCongestLevel:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool congestlevel_P(bool bWait = true){
		m_pSHMMgrCongestLevelLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool congestlevel_V(){
		m_pSHMMgrCongestLevelLock->V();
	};

	static int GetShmCongestLevel(CongestLevel & sShmCongestLevel);

	static int UpdateShmCongestLevel(CongestLevel sShmCongestLevel);

	static int AddShmCongestLevel(const CongestLevel & sShmCongestLevel);
};

//��service_context_list_base �ṩ��ѯ�͸��¹����ڴ�Ľӿ�
class SHMServiceTxtListBase:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool servicetxtlistbase_P(bool bWait = true){
		m_pSHMMgrServiceTxtListBaseLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool servicetxtlistbase_V(){
		m_pSHMMgrServiceTxtListBaseLock->V();
	};

	//��������service_context_id��ѯ��Ҫ���ֶ�
	static int getServiceTxtListBase(ServiceContextListBaseData &sTxtBaseData);

	//��������service_context_id���¹����ڴ���Ϣ
	static int updateServiceTxtListBase(ServiceContextListBaseData &sTxtBaseData);

	//��������service_context_id��ѯ��Ҫ���ֶ�
	static int getServiceTxtListBaseAll(vector<ServiceContextListBaseData> &vTxtBaseData);

	//�����������Ӻ�ɾ��
	static int AddServiceTxtListBase(ServiceContextListBaseData &sTxtBaseData);

	static int  DelServiceTxtListBase(ServiceContextListBaseData &sTxtBaseData);


};

//�ṩҵ����ʽӿ�
class SHMLoadBalance:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool loadbalance_P(bool bWait = true){
		m_pSHMMgrLoadBalanceLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool loadbalance_V(){
		m_pSHMMgrLoadBalanceLock->V();
	};

	//��������service_context_id��ѯ��Ҫ���ֶ�
	static int getLoadbalance(LoadBalanceData &sLodbalance);

	//��������service_context_id���¹����ڴ���Ϣ
	static int updateLoadbalance(LoadBalanceData &sLodbalance);

	//��������service_context_id��ѯ��Ҫ���ֶ�
	static int getLoadbalanceAll(vector<LoadBalanceData> &vLodbalance);
};


//�ṩbase_method_list
class SHMBaseMethod:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool basemethod_P(bool bWait = true){
		m_pSHMMgrBaseMethodLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool basemethod_V(){
		m_pSHMMgrBaseMethodLock->V();
	};

	//����ֵ 0 �ɹ� ��0ʧ��
	static int getBaseMethod(int m_iBaseMethodID, CBaseMethod & sBaseMethod);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int deleteBaseMethod(const CBaseMethod & sBaseMethod);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int updateBaseMethod(const CBaseMethod & sBaseMethod);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int addBaseMethod(const CBaseMethod & sBaseMethod);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int getBaseMethodAll(vector<CBaseMethod> & vBaseMethod);
};

class SHMPacketSession:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool packetsession_P(bool bWait = true){
		m_pSHMMgrPacketSessionLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool packetsession_V(){
		m_pSHMMgrPacketSessionLock->V();
	};

	//����ֵ 0 �ɹ� ��0ʧ��
	//--ȡ�����������лỰ
	static int getPacketSessionAll(const char * psSessionID, vector<SPacketSession> &sPackageSession);
	//--ȡ�����������Ự
	static int getPacketSession(const char * psChildSessionID, SPacketSession &sPackageSession);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int deletePacketSession(const SPacketSession &sPackageSession);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int updatePacketSession(const SPacketSession &sPackageSession);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int addPacketSession(const SPacketSession &sPackageSession);

};

class SHMServicePackage:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool servicepackage_P(bool bWait = true){
		m_pSHMMgrServicePackageLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool servicepackage_V(){
		m_pSHMMgrServicePackageLock->V();
	};

	static int getServicePackageNode(int iNodeSeq, ServicePackageNode & vServicePackageNode); // ---��ѯ�����ڵ�
	static int getServicePackageNodeALL(const char * psServiceContextId, vector<ServicePackageNode> & vServicePackageNode);//---��ѯĳ�����������нڵ�
	//����ֵ 0 �ɹ� ��0ʧ��
	static int deleteServicePackageNode(const ServicePackageNode & sServicePackageNode);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int updateServicePackageNode(const ServicePackageNode & sServicePackageNode);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int addServicePackageNode(const ServicePackageNode & sServicePackageNode);
};

class SHMServicePackageRoute:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool packageroute_P(bool bWait = true){
		m_pSHMMgrServicePackageRouteLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool packageroute_V(){
		m_pSHMMgrServicePackageRouteLock->V();
	};

	static int getServicePackageRoute(int iRouteSeq, ServicePackageRoute & SPackageRoute);  //---��ѯ����·��
	static int getServicePackageRouteALL(const char * psServicePackageId, vector<ServicePackageRoute> & vSPackageRoute);//---��ѯĳ������������·��
	//����ֵ 0 �ɹ� ��0ʧ��
	static int deleteServicePackageRoute(const ServicePackageRoute & sServicePackageRoute);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int updateServicePackageRoute(const ServicePackageRoute & sServicePackageRoute);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int addServicePackageRoute(const ServicePackageRoute & sServicePackageRoute);

};

class SHMServicePackageRouteCCR:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool packagerouteccr_P(bool bWait = true){
		m_pSHMMgrServicePackageRouteCCRLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool packagerouteccr_V(){
		m_pSHMMgrServicePackageRouteCCRLock->V();
	};

	static int getServicePackageRouteCCR(int iRouteCCRSeq, CServicePackageRouteCCR & sServicePackageRouteCCR);//--��ѯ����AVP
	static int getServicePackageRouteCCR(int iRouteSeq, vector<CServicePackageRouteCCR> & vServicePackageRouteCCR);//--��ѯĳ�������µ�����AVP
	//����ֵ 0 �ɹ� ��0ʧ��
	static int deleteServicePackageRouteCCR(const CServicePackageRouteCCR & sServicePackageRouteCCR);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int updateServicePackageRouteCCR(const CServicePackageRouteCCR & sServicePackageRouteCCR);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int addServicePackageRouteCCR(const CServicePackageRouteCCR & sServicePackageRouteCCR);


};

class SHMServicePackageJudge:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool packagejudge_P(bool bWait = true){
		m_pSHMMgrServicePackageJudgeLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool packagejudge_V(){
		m_pSHMMgrServicePackageJudgeLock->V();
	};
	//����ֵ 0 �ɹ� ��0ʧ��
	static int getServicePackageJudge(int iJudgeConditionSeq, CServicePackageJudge & sServicePackageJudge);//--��ѯ��������
	static int getServicePackageJudgeAll(int iNodeSeq, vector<CServicePackageJudge> & vServicePackageJudge);//--��ѯĳ���ڵ��µ���������
	//����ֵ 0 �ɹ� ��0ʧ��

	static int deleteServicePackageJudge(const CServicePackageJudge & sServicePackageJudge);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int updateServicePackageJudge(const CServicePackageJudge & sServicePackageJudge);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int addServicePackageJudge(const CServicePackageJudge & sServicePackageJudge);
};

class SHMServicePackageCCA:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool servicepackageCCA_P(bool bWait = true){
		m_pSHMMgrServicePackageCCALock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool servicepackageCCA_V(){
		m_pSHMMgrServicePackageCCALock->V();
	};

	static int getServicePackageCCA(int  iServicePackageCCASeq,CServicePackageCCA & sServicePackageCCA);//--��ѯ����AVP
	static int getServicePackageCCA(const char * psServiceContextId,vector<CServicePackageCCA> & vServicePackageCCA);//--��ѯĳ�������µ�����AVP
	//����������ѯ
	static	int AddServicePackageCCA(CServicePackageCCA & sServicePackageCCA);
	static int DelServicePackageCCA(CServicePackageCCA & sServicePackageCCA);
	static int UpdateServicePackageCCA(CServicePackageCCA & sServicePackageCCA);

};

class SHMServicePackageFunc:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool servicepackagefunc_P(bool bWait = true){
		m_pSHMMgrServicePackageFuncLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool servicepackagefunc_V(){
		m_pSHMMgrServicePackageFuncLock->V();
	};

	// ������������ѯһ������������Ϣ 0�ɹ� ��0ʧ�ܷ�����
	static int GetServicePackageFunc(int iCallID,CServicePackageFunc & sServicePackageFunc);
	//���ӽӿ� ����������ѯ
	static	int AddServicePackageFunc(CServicePackageFunc & sServicePackageFunc);
	static int DelServicePackageFunc(CServicePackageFunc & sServicePackageFunc);
	static int UpdateServicePackageFunc(CServicePackageFunc & sServicePackageFunc);


};

class SHMServicePackageVle:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool servicepackagevle_P(bool bWait = true){
		m_pSHMMgrServicePackageVariableLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool servicepackagevle_V(){
		m_pSHMMgrServicePackageVariableLock->V();
	};

	// ������������ѯһ������������Ϣ 0�ɹ� ��0ʧ�ܷ�����
	static int GetServicePackageVariable (int iVariableID, CServicePackageVariable & sServicePackageVariable);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int DelServicePackageVariable(CServicePackageVariable & sServicePackageVariable);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int UpdateServicePackageVariable(CServicePackageVariable & sServicePackageVariable);
	static int AddServicePackageVariable(CServicePackageVariable & sServicePackageVariable);
};

class SHMSCongestMsg:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool scongestmsg_P(bool bWait = true){
		m_pSHMMgrSCongestMsgLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool scongestmsg_V(){
		m_pSHMMgrSCongestMsgLock->V();
	};

	//4.�ṩ�����Լ���������
	static int GetAllStatisticsCongestMsg(vector<StatisticsCongestMsg> &vStatisticsCongestMsg);//ȡ������Ϣ

	//����ֵ 0 �ɹ� ��0ʧ��
	static int GetStatisticsCongestMsg(const char* m_strServiceContextId,StatisticsCongestMsg &sStatisticsCongestMsg);

	//����ֵ 0 �ɹ� ��0ʧ��
	static int InsertStatisticsCongestMsg(StatisticsCongestMsg sStatisticsCongestMsg);

	////����ֵ 0 �ɹ� ��0ʧ��
	static int UpdateStatisticsCongestInMsg(const char* m_strServiceContextId,int num=1);  //���� ����Ϣ��+1

	//����ֵ 0 �ɹ� ��0ʧ��
	static int UpdateStatisticsCongestOutMsg(const char* m_strServiceContextId,int num=1);  //���� ����Ϣ��+1

	//����ֵ 0 �ɹ� ��0ʧ��
	static int UpdateStatisticsCongestDiscardMsg(const char* m_strServiceContextId,int num=1);  //���� ������Ϣ��+1

};

class SHMWfProcessMq:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool wfprocessmq_P(bool bWait = true){
		m_pSHMMgrWfprocessMqLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool wfprocessmq_V(){
		m_pSHMMgrWfprocessMqLock->V();
	};

	//����ֵ 0 �ɹ� ��0ʧ��
	static int getAllStWfProcessMq(vector<ST_WF_PROCESS_MQ> & vStWfProcessMq);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int getStWfProcessMq(ST_WF_PROCESS_MQ &sStWfProcessMq);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int addStWfProcessMq(const  ST_WF_PROCESS_MQ &sStWfProcessMq);
};

class SHMPortInfo:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool portinfo_P(bool bWait = true){
		m_pSHMMgrPortInfoLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool portinfo_V(){
		m_pSHMMgrPortInfoLock->V();
	};

	//����ֵ 0 �ɹ� ��0ʧ��
	static int getPortInfo(int m_iPortID,PortInfo &oPortInfo);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int updatePortinfo(const PortInfo &oPortInfo);
	//����ֵ 0 �ɹ� ��0ʧ��
	static int addPortinfo(const PortInfo &oPortInfo);

	static int delPortInfo(const PortInfo &oPortInfo);
	static int getPortInfoAll(vector<PortInfo> &vPortInfo);

	static int clearPortInfo();
};

class SHMSessionClear:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool sessionClear_P(bool bWait = true){
		m_pSHMMgrSessionClearLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool sessionClear_V(){
		m_pSHMMgrSessionClearLock->V();
	};

	//����ֵ 0 �ɹ� ��0ʧ��
	static int getSessionClear(SessionClearData &oSessionClear);
	//����ֵ 0 �ɹ� ��0ʧ��
	static void addSessionClear(const SessionClearData &oSessionClear);

	static int delSessionClear(const SessionClearData &oSessionClear);

	static int getSessionClearAll(vector<SessionClearData> &vSessionClear,long _lTime);

	static int setPreDeal(int value) {
		m_pMgrSessionHeadSign[0].m_iPreDeal = value;
	}

	static int getPreDeal() {
		return m_pMgrSessionHeadSign[0].m_iPreDeal;
	}

	static int setFreePos(int value) {
		m_pMgrSessionHeadSign[0].m_iFree = value;
	}

	static int getFreePos() {
		return m_pMgrSessionHeadSign[0].m_iFree;
	}
};

class SHMOrgRoute:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool orgRoute_P(bool bWait = true){
		m_pSHMMgrOrgRouteLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool orgRoute_V(){
		m_pSHMMgrOrgRouteLock->V();
	};

	//ͨ������org_id,org_level_id;���ظ���Ӧorg_id����Ӧ�ĸ��ڵ�org_id����org_level_idΪ�������Ӧ����
	static int GetRouteOrgID(int src_org_id,int org_level_id,int &descOrgID );

	//֧������org_area_code,org_level_id,����org_area_code����Ӧ����Ӧ�ļ����org_id;
	static int GetRouteOrgIDUserCode(char *org_area_code,int org_level_id,int &descOrgID );

	//֧�ְ�org_id �������ݣ��������޸ģ�
	static int UpdateRouteOrg(int org_id,int org_level_id,int parent_org_id,char *org_area_code); //���ԭ���е��������޸ģ���������ݣ�������������

	//֧�ֵ������ݡ�
	//����ֶΰ�org����ֶν�����֧�ְ��ڴ����ݵ��뵽���ݱ��С��Ա�Ա������Ƿ�����������һ�¡�
	static int ExpRouteOrg(char *sTable_name);

};

class SHMOrgHead:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool orghead_P(bool bWait = true){
		m_pSHMMgrOrgHeadLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool orghead_V(){
		m_pSHMMgrOrgHeadLock->V();
	};

	//����Ҫ������һ��Ӧ�ĺ��룬������Ӧ��Ч���ݵ�org_id;
	static int GetOrgIDbyNbr(char *AccNbr,int &Org_Id);

	//֧�ְ�head �������ݣ��������޸ģ�
	static int UpdateAccNbrHead(int org_id,char *head,char *eff_date,char *exp_date,char *table_name); //���ԭ���е��������޸ģ���������ݣ�������������

	//���ӵ�������
	static int ExpAccNbrHead(char *sTable_name);//����ֶΰ�b_head_region����ֶν�����������һ��table_name�ı�ʾ�ֶΣ��ֶ�����Ϊvarchar2(20)��֧�ְ��ڴ����ݵ��뵽���ݱ��С��Ա�Ա������Ƿ�����������һ��

};

class SHMChannelInfo:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool ChannelInfo_P(bool bWait = true){
		m_pSHMMgrChannelLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool ChannelInfo_V(){
		m_pSHMMgrChannelLock->V();
	};

	//�����������������Ϣ
	static int GetChannelInfo(char * pszChannelID,SgwChannelInfo & sSgwChannelInfo);
	//��������Ÿ���������Ϣ
	static int UpdateChannelInfo(char * pszChannelID,SgwChannelInfo & sSgwChannelInfo);

	static int AddChannelInfo(const SgwChannelInfo & sSgwChannelInfo);

	static int DeleteChannelInfo(char * pszChannelID);

};

//class SHMStaffInfo:public SHMSGWInfoBase
//{
//public:
//	//���� �ɹ�����true��ʧ�ܷ���false
//	static bool StaffInfo_P(bool bWait = true){
//		m_pSHMMgrStaffLock->P(bWait);
//	};
//
//	//�������ɹ�����truce��ʧ�ܷ���false
//	static bool StaffInfo_V(){
//		m_pSHMMgrStaffLock->V();
//	};
//
//	//������ȡ����
//	static int GetStaffInfo(long lStaffID,SgwStaffInfo& sSgwStaffInfo);
//	//�����Ÿ�������
//	static int UpdateStaffInfo(long lStaffID,SgwStaffInfo& sSgwStaffInfo);
//
//	static int AddStaffInfo(const SgwStaffInfo& sSgwStaffInfo);
//
//	static int DeleteStaffInfo(long lStaffID);
//
//};

//class SHMNetChannelInfo:public SHMSGWInfoBase
//{
//public:
//	//���� �ɹ�����true��ʧ�ܷ���false
//	static bool NetChannel_P(bool bWait = true){
//		m_pSHMMgrNetChannelLock->P(bWait);
//	};
//
//	//�������ɹ�����truce��ʧ�ܷ���false
//	static bool NetChannel_V(){
//		m_pSHMMgrNetChannelLock->V();
//	};
//
//	//����Ԫ�����ѯ����
//	static int GetNetChannelByNet(char * pszNetInfoCode,SgwNetChannel& sSgwNetChannel);
//	//��������ѯ��Ԫ����
//	static int GetNetChannelByChannel(char * pszChannel,SgwNetChannel& sSgwNetChannel);
//	//��������������
//	static int UpdateNetChannel(int iChannelNetID,SgwNetChannel& sSgwNetChannel);
//
//	static int GetNetChannel(int iChannelNetID,SgwNetChannel& sSgwNetChannel);
//
//	static int AddNetChannel(const SgwNetChannel& sSgwNetChannel);
//	static int DeleteNetChannel(int iChannelNetID);
//};

class SHMUserStaff:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool UserStaff_P(bool bWait = true){
		m_pSHMMgrUserStaffLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool UserStaff_V(){
		m_pSHMMgrUserStaffLock->V();
	};

	//���û�����
	static int GetUserStaffByUserStarr(int iUserStaffID,SgwUserStaff& sSgwUserStaff);
	//�����Ų���Ϣ
	static	int GetUserStaffByStarr(int iStaffID,SgwUserStaff& sSgwUserStaff);
	//����������Ϣ
	static	int GetUserStaffByChannel(char * pszChannelID,vector<SgwUserStaff>& vSgwUserStaff);
	//������������Ϣ
	static	int UpdateUserStaff(int iUserStaffID,SgwUserStaff& sSgwUserStaff);

	static int DeleteUserStaff(SgwUserStaff& sSgwUserStaff);

	static int AddUserStaff(SgwUserStaff& sSgwUserStaff);
};

class SHMOrgRouteRule:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool OrgRouteRule_P(bool bWait = true){
		m_pSHMMgrOrgRoutRuleLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool OrgRouteRule_V(){
		m_pSHMMgrOrgRoutRuleLock->V();
	};

	//��OrgID����·����Ϣ
	static	int GetOrgRouteRuleByOrg(int iOrgID,SgwOrgRouteRule& sOrgRouteRule);
	//����������
	static	int UpdateOrgRouteRule(int iRouteID,SgwOrgRouteRule& sOrgRouteRule);

	static int DeleteOrgRouteRule(const SgwOrgRouteRule& sOrgRouteRule);

	static int AddOrgRouteRule(const SgwOrgRouteRule& sOrgRouteRule);

	static int GetOrgRouteRule(int _iRouteID,SgwOrgRouteRule& sOrgRouteRule);

};

class SHMAreaOrg:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool AreaOrg_P(bool bWait = true){
		m_pSHMMgrAreaOrgLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool AreaOrg_V(){
		m_pSHMMgrAreaOrgLock->V();
	};

	//������������
	static int GetAllAreaOrg(vector<SgwAreaOrg>&vSgwAreaOrg);
	//��������������
	static int UpdateAreaOrg(int iAreaSeq,SgwAreaOrg&sSgwAreaOrg);

	static int AddAreaOrg(const SgwAreaOrg&sSgwAreaOrg);
	static int DeleteAreaOrg(const SgwAreaOrg&sSgwAreaOrg);
	static int GetAreaOrg(int iAreaSeq,const SgwAreaOrg &sSgwAreaOrg);
};

class SHMTuxRelaIn:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool TuxRelaIn_P(bool bWait = true){
		m_pSHMMgrTuxRelaInLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool TuxRelaIn_V(){
		m_pSHMMgrTuxRelaInLock->V();
	};

	//�����������
	static int GetServicesContextID(int iServicesID,SgwTuxedoRelationIn&sTuxedoRelation);

	static int UpdateServicesContextID(SgwTuxedoRelationIn&sTuxedoRelation);

	static int DeleteServicesContextID(SgwTuxedoRelationIn&sTuxedoRelation);

	static int AddServicesContextID(SgwTuxedoRelationIn&sTuxedoRelation);
};

class SHMTuxRelaOut:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool TuxRelaOut_P(bool bWait = true){
		m_pSHMMgrTuxRelaOutLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool TuxRelaOut_V(){
		m_pSHMMgrTuxRelaOutLock->V();
	};

	//�����������
	static	int GetServicesID(char * pszServiceContextID,SgwTuxedoRelationOut&sTuxedoRelation);

	static int GetServicesID(int m_iRelationID ,SgwTuxedoRelationOut&sTuxedoRelation);

	static int AddServicesID(const SgwTuxedoRelationOut&sTuxedoRelation);

	static int DeleteServicesID(const SgwTuxedoRelationOut&sTuxedoRelation);

	static int UpdateServicesID(const SgwTuxedoRelationOut&sTuxedoRelation);
};

class SHMSvrList:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool SvrList_P(bool bWait = true){
		m_pSHMMgrSrvsListLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool SvrList_V(){
		m_pSHMMgrSrvsListLock->V();
	};

	//���������ƣ�SERVICES_NAME��������Ϣ
	static	int GetServicesInfo(char * iServicesName,SgwServicesList&sServicesList);

	//�������ŷ�����Ϣ
	static	int GetServicesInfo(int iServicesID,SgwServicesList&sServicesList);

	static	int AddServicesInfo(const SgwServicesList&sServicesList);

	static	int UpdateServicesInfo(const SgwServicesList&sServicesList);

	static	int DeleteServicesInfo(const SgwServicesList&sServicesList);

};

class SHMSrvsParamRela:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool SrvsParamRela_P(bool bWait = true){
		m_pSHMMgrSrvsParamRelaLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool SrvsParamRela_V(){
		m_pSHMMgrSrvsParamRelaLock->V();
	};

	//������ID��������
	static	int GetServicesParam(int iServicesID,vector<SgwServicesParamRelation>& vServicesParam);

	static	int AddServicesParam(const SgwServicesParamRelation &oServicesParam);

	static	int DeleteServicesParam(const SgwServicesParamRelation &oServicesParam);

	static	int UpdateServicesParam(const SgwServicesParamRelation &oServicesParam);

	static int GetServicesParam(int iMappingID,SgwServicesParamRelation &oServicesParam);
};

class SHMSrvsParamList:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool SrvsParamList_P(bool bWait = true){
		m_pSHMMgrSrvsParamListLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool SrvsParamList_V(){
		m_pSHMMgrSrvsParamListLock->V();
	};

	//������ID������Ϣ
	static	int GetServiceParamList(int iParamID,SgwServicesParamList& sServicesparamList);

	static int AddServiceParamList(const SgwServicesParamList& sServicesparamList);

	static int DeleteServiceParamList(const SgwServicesParamList& sServicesparamList);

	static int UpdateServiceParamList(const SgwServicesParamList& sServicesparamList);

};

class SHMSrvsRegister:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool SrvsRegister_P(bool bWait = true){
		m_pSHMMgrSrvsRegisterLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool SrvsRegister_V(){
		m_pSHMMgrSrvsRegisterLock->V();
	};

	//����Ԫ������ԭʼ�����ѯ����
	static	int GetServiceRegisterBySrc(char * pszChannelID,int iSrcID,vector<SgwServiceRegister>& vsServiceRegister);
	//����Ԫ������Ŀ������ѯ����
	static	int GetServiceRegisterByMap(char * pszChannelID,int iMapID,vector<SgwServiceRegister>& vsServiceRegister);

	static	int GetServiceRegister(int iServicesInstanceID,SgwServiceRegister& sServiceRegister);

	static	int AddServiceRegister(const SgwServiceRegister& sServiceRegister);

	static	int DeleteServiceRegister(const SgwServiceRegister& sServiceRegister);

	static	int UpdateServiceRegister(const SgwServiceRegister& sServiceRegister);
};

class SHMParamInsList:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool ParamInsList_P(bool bWait = true){
		m_pSHMMgrParamInsListLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool ParamInsList_V(){
		m_pSHMMgrParamInsListLock->V();
	};

	//��ʵ��ID������������
	static int GetParamInstanceList(int iServicesInstanceID,vector<SgwParamInstanceList>&vInstanceList);

	static int GetParamInstanceList(int iMappingID,SgwParamInstanceList &oInstanceList);

	static int DeleteParamInstanceList(const SgwParamInstanceList &oInstanceList);

	static int AddParamInstanceList(const SgwParamInstanceList &oInstanceList);

	static int UpdateParamInstanceList(const SgwParamInstanceList &oInstanceList);
};

class SHMEnumArea:public SHMSGWInfoBase
{
public:
	//���� �ɹ�����true��ʧ�ܷ���false
	static bool SgwEnumArea_P(bool bWait = true){
		m_pSHMMgrSgwEnumAreaLock->P(bWait);
	};

	//�������ɹ�����truce��ʧ�ܷ���false
	static bool SgwEnumArea_V(){
		m_pSHMMgrSgwEnumAreaLock->V();
	};

	//��seq/orgid������������
	static int GetSgwEnumArea(int iAreaSeq,SgwEnumArea &oSgwEnumArea);

	static int GetSgwEnumAreaByOrgId(int iOrgID,SgwEnumArea &oSgwEnumArea);

	static int DeleteSgwEnumArea(const SgwEnumArea &oSgwEnumArea);

	static int AddSgwEnumArea(const SgwEnumArea &oSgwEnumArea);

	static int UpdateSgwEnumArea(const SgwEnumArea &oSgwEnumArea);
};
#endif
