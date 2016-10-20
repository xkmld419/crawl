/*
* version 1.5 huff at 2010-05-04
*
*/
#ifndef _PERFORMANCE_STATIC_H_INCLUDE_EFFFCD
#define _PERFORMANCE_STATIC_H_INCLUDE_EFFFCD

#include "abmobject.h"

//1,2,3,4,5,6,7,8,12,16......................500,overflow
#define DURATION_NUM        132 
#define ABM_CMD_MAX         128     //ABMϵͳ�������� 
#define TIME_SLICE_MAX      4       //ʱ��Ƭ(����) 4���� 2�� power,�������3
#define DFT_INVAL_DUR       2       //Ĭ��ʧЧʱ�� ����

//#ifdef _ABM_SERVER_
#define LINK_NUM_MAX        128     //��������������
//#endif

#ifdef _ABM_CLIENT_
#define LINK_NUM_MAX        32      //�ͻ�������������
#endif


/*
*  �ڴ浥Ԫ ʱ��Ƭ�ڴ沼��
*/
struct StatUnitData
{
    long m_lTotalDur;                   // ʱ��  ��λ0.1����
	long m_lNetDur;                     //�ͻ������ڼ��������ӳ� ��λ΢��
    int  m_iMaxWDur;                    //�������ص����ʱ��
    int  m_iMinWDur;                    //�������ص���Сʱ��
    int  m_iMaxNDur;                    //����������ʱʱ��
    int  m_iMinNDur;                    //��С��������ʱʱ��
    int  m_iSendNum;                    // ���� ����
    int  m_iRecvNum;                    // ���� ����
    int  m_iDurData[DURATION_NUM];      //������Ӧʱ��ηֲ� 20΢��
    int  m_iNDurData[DURATION_NUM];		//���紦����Ӧʱ��ηֲ� 20΢��
    int  m_iODurData[DURATION_NUM];		//��ҵ������Ӧʱ��ηֲ� 20΢��
    int  m_iNext;                       //��������ֵ���γ����������ȡ
    //1,2,3,4,5,6,7,8,12,16......................500,overflow
};

//�ڴ��е����������һ���ͳ��
struct StatCmdData
{
    long m_lTime;                       // yyyymmddhh24mi ת���� �� ,��ʾʱ��
    int  m_iSendNum;                    //���͵�������������
    int  m_iRecvNum;                    //���յĶ�Ӧ������
    int  m_iSendPack;                   //���͵��������
    int  m_iSendPackContNum;            //���͵������������������/�����
    int  m_iRecvPack;                   //����Ϊ��ĳʱ���յ��������
    int  m_iRecvPackContNum;            //����Ϊ��ʱ���յ��������������������
    StatUnitData m_oUnitData[ABM_CMD_MAX];
    int  m_iHead;                       //ָ���һ����ֵ������
};

//һ�����ӵ�ͳ������
struct StatBlockData
{
    int m_iLinkID;                              //��link_id��Ϊͳ�Ƶ�Ԫ
    StatCmdData m_oCmdData[TIME_SLICE_MAX+1];   //ÿ���ڵ���ڴ沼�� ����256������
};

//���е�ͳ������
struct StatData
{
    StatBlockData m_oData[LINK_NUM_MAX];
};

struct OnlineStatData {
    int m_iLinkID;
    long m_lTime;
    int m_iCmdID;

};

//����ͳ�ƺͼ���������ҪΪ�˺�����չ�ֺͷ���
//�������ֿ��ܴ��ڵ�Ǳ������
class StatDataMgr {
public:
    //��ʼ��
    int init(StatData *pMem) { m_poData = pMem; return 0; }

    //��û��ǰ̨���������£�����Ҫʵʱ��ص�ʱ��
    //ͨ���������µĽӿڣ����µĽӿ�Լÿ���Ӳ������ؽ����
    //���÷������ⲿsleep�����Ե���
    //����0 �� ��һ�����ݷ��� ; �������ݷ���ʱ����Ҫsleep��ֱ�ӻ�ȡ��һ��
    //����-1�� �����ݷ���
    //-2: ��������
    int getOfflineData(StatCmdData &oData, int &iLinkID);

    //��ʵʱ��صĳ����£���Ҫ���ϵػ�ȡ�µ�����
    //
    // ʵʱ��˼·���ǰ��������Ҫ�������ݻ�ȡ��
    // 20�ź�ʵ��
    // ǰ̨��Ҫʲô�������ݣ���紫��Ӧ�Ļ�ȡҪ�󣬷�����Ӧ�ĵ�ʱ������
    //int getOnlineData(OnlineStatdata & data);
private:
    StatData * m_poData;
};

class PerfStat {
public:

    PerfStat() : m_poData(0), m_lRClockTime(0), m_lSClockTime(0) {}
    
    //����ͨѶlink_id, �ڴ����ⲿ����,
    //���������ṹ�����ָ��
    int init( StatData * pMem, int iLinkID);
    
    //ȡ������Ӧʱ��ηֲ���λ��
    int getDurDataPoint(long dur);

    //˲������ж����Ϣ��������û�б�Ҫ���getdayoftime�������ⲿ������ʾ����
    //���ԣ������ڲ�����һ��ʱ�ӣ����ⲿ��ʽ����
    int updSendClock(); 
    int updRecvClock(); 

    //�ͻ��˵�ͳ�Ƶ���
    
    //�ͻ��������˷��͵�ʱ�򣬽��е���
    int statClntSend(ABMObject *pObject);
    //�ͻ��˴ӷ���˻�ȡ��ʱ��ĵ���
    int statClntRecv(ABMObject *pObject);

    //����˵�ͳ�Ƶ���

    //����˽��յ��ͻ��˵�����ʱ�����е���
    int statSrvRecv(ABMObject *pObject);
    //����˷��ؿͻ�������ʱ�����е���
    int statSrvSend(ABMObject *pObject);

    //
    int statSendPack(int packnum, int sendnum);
    int statRecvPack(int packnum, int recvnum);
    
private:
	
    long m_lSClockTime;
    long m_lRClockTime;
    StatBlockData *m_poData;    //дָ��
};

#endif

