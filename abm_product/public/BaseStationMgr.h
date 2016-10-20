/*VER: 1*/ 
#ifndef __BASE_DOMAIN__H__
#define __BASE_DOMAIN__H__

#include "CommonMacro.h"
#include "KeyTree.h"

class BaseStation {
  public:
    //������Ϣ
    char m_sGateWay[MAX_GATE_WAY_LEN];
    
    //Ѱ����(PA)
    int  m_iPageArea; 
    
    //��վ
    char m_sRP[MAX_RP_STR_LEN];
    
    //��վ����(����������Ϣ��pa������վ����)
    int  m_iRPArea;
};

class BaseStationMgr {
  public:
    //## init BaseStation info
    static void init ();  
    
    //## ���� ����/Ѱ����(pa)/��վ ��ȡ ��վ��Ϣ
    static bool getBaseStationInfo (const char sGateWay[], 
        const int iPageArea, const char sRP[], int *piRPArea);

    static bool getBaseStationInfo_v2 (const char sGateWay[], 
        const int iPageArea, const char sRP[], int *piRPArea);
                
    //�ж��Ƿ�����
    static bool getBelong(int iChildStation, int iParentStation);    
  
  private:
    static KeyTree<int>* m_poBaseStation;
    static KeyTree<int>* m_poStationTree;
    
    static bool m_bInitFlag;
};

#endif



