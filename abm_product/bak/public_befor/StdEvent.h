/*VER: 2*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

/*

	2008-05-22 ע�͵���Ҫ�õ�
		class ServProdAttr

*/

#ifndef STDEVENT_H_HEADER_INCLUDED_BDB42691
#define STDEVENT_H_HEADER_INCLUDED_BDB42691

#include "CommonMacro.h"

class ChargeInfo;
class ServProductAttr;

//##ModelId=423E82E30286
//##Documentation
//## ���ÿ����������ȡ��Ӧ��������Ϣ��
struct StdChargeInfo
{
	long m_lStdCharge;
	int m_iStdAcctItemTypeID;
};
struct ChargeInfo
{
	long m_lCharge;
	int  m_iAcctItemTypeID;
	long m_lBillMeasure;
	int  m_iMeasureType;
	long m_lAcctID;
	long m_lOfferInstID;
    int  m_iItemSourceID;
};

#ifdef EVENT_CHARGE_ACCU_EXCEEDED
struct AccuItemInfo
{
    long m_lEventAggrID;
    long m_lOfferInstID;
    long m_lValue;
};
#endif

/*

//##ModelId=423F70E901A2
//##Documentation
//## ������Ʒ�������ݽṹ
class ServProdAttr
{
public:
    //##ModelId=423F718E027C
    //##Documentation
    //## ���Ա�ʶ
    int m_iAttrID;

    //##ModelId=423F71B00022
    //##Documentation
    //## ����ȡֵ
    char m_sAttrVal[MAX_ATTR_VAL_LEN];

    //##ModelId=423F71CD010A
    //##Documentation
    //## ������Чʱ��
    char m_sAttrEffDate[MAX_ATTR_EFF_DATE_LEN];

    //##ModelId=423F71DA028F
    //##Documentation
    //## ����ʧЧʱ��
    char m_sAttrExpDate[MAX_ATTR_EXP_DATE_LEN];

};

*/


//##ModelId=4257605C01D2
//##Documentation
//## �¼��ṹ��չ
//## Ŀ��: ���Ӧ��ģ���ڵ��¼���ʱ����
class StdEventExt
{
  public:
    
    long m_lExtendValue[10];
    char m_sExtendValue[10][MAX_EXT_STRING_LEN];
    
    //##ModelId=425762460140
    //##Documentation
    //## ��������
    int m_iRoamTypeID;
	// �߽�����-1/ͬ������-2/������-0/����3(��Ҫϸ��)
	// ����ϸ��: ʡ��-30/����-31/����-32



    int m_iStdRoamTypeID;	
    int m_iStdLongTypeID;



    //##ModelId=4273190B009C
    //##Documentation
    //## ��;����
    int m_iLongTypeID;
    // ��;-1(��Ҫϸ��)/�ǳ�;(�л�)-2
    // ��;ϸ��: ʡ�ڳ�;-10/���ڳ�;-11/��ͨ����-12/ʮ����-13/����-14/�۰�̨-15
    
    //##ModelId=42732006013C
    //##Documentation
    //## Ԥ�������ڴ�����ļ���
    char m_sFileName[MAX_FILE_NAME_LEN];

    //##ModelId=4273252403B7
    //##Documentation
    //## ���Դ�¼�����
    int m_iSourceEventType;
    
    //##ModelId=42732B8603DE
    //##Documentation
    //## �����������
    int m_iCarrierTypeID;

    //##ModelId=427BA09B039E
    int m_iCallingLongGroupTypeID;

    //##ModelId=427BA0DD01EB
    int m_iCalledLongGroupTypeID;

    //##ModelId=4281779C02E9
    //##Documentation
    //## ������ά����ID
    int m_iCallingOrgID;

    //##ModelId=428177BB03B6
    //##Documentation
    //## ������ά����ID
    int m_iCalledOrgID;

    //##ModelId=4289582C01E3
    //##Documentation
    //## �¼�ԭʼ���к�
    char m_sOrgSequence[MAX_ORG_SEQ_LEN];

    //##ModelId=42895853015D
    //##Documentation
    //## �¼�����ָʾ 
    int m_iPartialIndicator;

	
	//�м��¼���кţ���1��ʼ����
	int m_iMiddleSequnce;
	
	//## ��������;����
	int m_iSwitchLongType;
	
	//Ϊ1�����������ϲ�����������Ʒ�ϵͳ��Ҫ��2����֮�ڼƷ����
	int  m_iHotBillingTag;
	
	//�Ҷ�ԭ�������жϲ��ֻ����Ľ�����ʶ��Ϊ0Ϊ����������
	int m_iCausefortermination;
	
	//## ����������
	char m_sSwitchAreaCode[MAX_CALLING_AREA_CODE_LEN];
	
	//���е��õ�����  
	char   m_sCallingVisitAreaCode[MAX_CALLING_VISIT_AREA_CODE_LEN];

	//���е��õ�����  
	char   m_sCalledVisitAreaCode[MAX_CALLED_VISIT_AREA_CODE_LEN];
    
    //���������뵽�õ�����
    char m_sThirdPartyVisitAreaCode[MAX_CALLED_VISIT_AREA_CODE_LEN];
    
	//IMSI    
	char m_sIMSI[MAX_IMSI_LEN];
	
	//IMEI    
	char m_sIMEI[MAX_IMEI_LEN];

	//���������
	char m_sAccessPointName[MAX_ACCESS_POINT_NAME_LEN];
	
	//�Ʒѽڵ�ID
	int m_iNodeID;
};


//##ModelId=41CF742F03B3
//##Documentation
//## �˽ṹ����ϵͳ���漰�������¼����Ե�ȫ���������ټ�һ���ֱ����ֶΡ�
class StdEvent
{
  public:
    
    //##ModelId=4236525C0222
    //##Documentation
    //## �¼���ʶ
    long m_lEventID;
	//wuan 2010-07-14
    #ifdef ORG_BILLINGCYCLE
    int m_iOrgBillingCycleID;
    #endif
    //##ModelId=423652AB03C0
    //##Documentation
    //## ���ڱ�ʶ
    int m_iBillingCycleID;

    //##ModelId=423652000356
    //##Documentation
    //## �¼����ͱ�ʶ
    int m_iEventTypeID;

    //##ModelId=423652ED002C
    //##Documentation
    //## �Ʒѷ�����
    char m_sBillingAreaCode[MAX_BILLING_AREA_CODE_LEN];
	
    //##ModelId=4236530B006B
    //##Documentation
    //## �Ʒѷ�����
    char m_sBillingNBR[MAX_BILLING_NBR_LEN];

    //##ModelId=42366C2F032C
    //##Documentation
    //## �Ʒѷ����õ�����
    char m_sBillingVisitAreaCode[MAX_BILLING_VISIT_AREA_CODE];

    //##ModelId=423653260286
    //##Documentation
    //## ���з�����
    char m_sCallingAreaCode[MAX_CALLING_AREA_CODE_LEN];

    //##ModelId=423653350133
    //##Documentation
    //## ���з�����
    char m_sCallingNBR[MAX_CALLING_NBR_LEN];

    //##ModelId=423653350133
    //##Documentation
    //## ���з��������
    char m_sCallingPhysicalNBR[MAX_CALLING_NBR_LEN];

    //##ModelId=4236537F02CA
    //##Documentation
    //## ԭʼ�����з�����
    char m_sOrgCallingNBR[MAX_ORG_CALLING_NBR_LEN];
    
     //##ModelId=4236537F02CA
    //##Documentation
    //## ԭʼ�������������
    char m_sOrgCallingPhysicalNBR[MAX_ORG_CALLING_NBR_LEN];

    //##ModelId=4236534D037D
    //##Documentation
    //## ������Ӫ�����ͱ�ʶ
    int m_iCallingSPTypeID;

    //##ModelId=42365370017E
    //##Documentation
    //## ����ҵ�����ͱ�ʶ
    int m_iCallingServiceTypeID;

    //##ModelId=4236546502FD
    //##Documentation
    //## ��������
    char m_sCalledAreaCode[MAX_CALLED_AREA_CODE_LEN];

    //##ModelId=423654730127
    //##Documentation
    //## ���к���
    char m_sCalledNBR[MAX_CALLED_NBR_LEN];
    
    //##ModelId=423654730127
    //##Documentation
    //## �����������
    char m_sCalledPhysicalNBR[MAX_CALLED_NBR_LEN];

    //##ModelId=423654A20020
    //##Documentation
    //## ԭʼ�ı��к���
    char m_sOrgCalledNBR[MAX_ORG_CALLED_NBR_LEN];
    
    //##ModelId=423654A20020
    //##Documentation
    //## ԭʼ�ı����������
    char m_sOrgCalledPhysicalNBR[MAX_ORG_CALLED_NBR_LEN];

    //##ModelId=42366B6502E6
    //##Documentation
    //## ������Ӫ�����ͱ�ʶ
    int m_iCalledSPTypeID;

    //##ModelId=42366B8A03BB
    //##Documentation
    //## ����ҵ�����ͱ�ʶ
    int m_iCalledServiceTypeID;

    //##ModelId=42366B2302F5
    //##Documentation
    //## ��������������
    char m_sThirdPartyAreaCode[MAX_THIRD_PARTY_AREA_CODE_LEN];

    //##ModelId=42366BC70066
    //##Documentation
    //## ����������
    char m_sThirdPartyNBR[MAX_THIRD_PARTY_NBR_LEN];
    
     //##ModelId=42366BC70066
    //##Documentation
    //## �������������
    char m_sThirdPartyPhysicalNBR[MAX_THIRD_PARTY_NBR_LEN];

    //##ModelId=42366BE90263
    //##Documentation
    //## �¼���ʼʱ��
    char  m_sStartDate[MAX_START_DATE_LEN];


    //##ModelId=42366BFC0329
    //##Documentation
    //## �¼�����ʱ��
    char  m_sEndDate[MAX_END_DATE_LEN];

    //##ModelId=42366C05014B
    //##Documentation
    //## �¼�ʱ��
    //int m_iDuration;
    long  m_lDuration;
    
    //IP�û�����
    int m_iIPType;
    
    // ��������
    long  m_lRecvAmount;
    
    // ��������
    long  m_lSendAmount;
    
    //IPv6��������
    long  m_lRecvAmountV6;
    
    //IPv6��������
    long  m_lSendAmountV6;
    
    //IPv4������
    long  m_lSumAmountV4;
    
    //IPv6������
    long  m_lSumAmountV6;
    
    //## ������
    long  m_lSumAmount;

    //##ModelId=42366C680270
    //##Documentation
    //## ���м̺�
    char m_sTrunkIn[MAX_TRUNK_IN_LEN];

    //##ModelId=42366C740368
    //##Documentation
    //## ���м����ͱ�ʶ
    int m_iTrunkInTypeID;

    //##ModelId=42366CAE01B2
    //##Documentation
    //## ���м̺�
    char m_sTrunkOut[MAX_TRUNK_OUT_LEN];

    //##ModelId=42366CBD02FE
    //##Documentation
    //## ���м�����
    int m_iTrunkOutTypeID;

    //##ModelId=42366CF703D4
    //##Documentation
    //## �ļ���ʶ
    int m_iFileID;

    //##ModelId=42366D070341
    //##Documentation
    //## �¼�״̬: ��ʾ�¼���Դ, ����ʱ��.  �� �ڻ�����/������
    char m_sEventState[MAX_EVENT_STATE_LEN];

    //##ModelId=42366D2D0378
    //##Documentation
    //## �¼�����Ʒ�ϵͳʱ��
    char  m_sCreatedDate[MAX_CREATED_DATE_LEN];

    //##ModelId=42366D350301
    //##Documentation
    //## �¼�ȷ��״̬��ʱ��
    char   m_sStateDate[MAX_STATE_DATE_LEN];

    //##ModelId=42366D4E01DB
    //##Documentation
    //## ����ʶ
    char m_sCardID[MAX_CARD_ID_LEN];

    //##ModelId=42366D56024A
    //##Documentation
    //## �Ʒ����ͱ�ʶ��������ʶԤ���Ѻͺ󸶷�
    int m_iBillingTypeID;

    //##ModelId=42366D7701CF
    //##Documentation
    //## ��������ʶ
    int m_iParterID;

    //##ModelId=42366DCA030F
    //##Documentation
    //## �û���ʶ
    long m_iServID;

    //##ModelId=42366DD30109
    //##Documentation
    //## �ͻ���ʶ
    long m_iCustID;

    //##ModelId=42366E080282
    //##Documentation
    //## ��Ʒ��ʶ
    int m_iProdID;

    //##ModelId=423F733A0318
    //##Documentation
    //## ������Ʒ��ʶ
    long m_iServProdID;

    //##ModelId=423E788002F0
    long  m_lOffers[MAX_OFFER_NUM];

    //##ModelId=42367F7302B6
    //##Documentation
    //## ��ѹ��̼���
    char m_sBillingRecord[MAX_BILLING_RECORD_LEN];

    //##ModelId=42367F7F00FB
    //##Documentation
    //## ����ű�ʶ
    int m_iErrorID;

    //##ModelId=4236805800CB
    //##Documentation
    //## ����ҵ���ʺ����ڵ�������
    char m_sAccountAreaCode[MAX_ACCOUNT_AREA_CODE_LEN];

    //##ModelId=4236807100A9
    //##Documentation
    //## ����ҵ���ʺ�
    char m_sAccountNBR[MAX_ACCOUNT_NBR_LEN];

    //##ModelId=42368089016B
    //##Documentation
    //## �������IP
    char m_sNASIp[MAX_NAS_IP_LEN];

    //##ModelId=42368233039F
    //##Documentation
    //## �������˿ڱ�ʶ
    int m_iNASPortID;

    //##ModelId=423921860255
    //##Documentation
    //## �õ��ķ�������
    int m_iNegotiatedQoS;

    //##ModelId=425793160080
    //##Documentation
    //## ��ǰ�¼�����չ�ֶ��б�ʹ�õ����Ը���
    int m_iExtUsedNum;

    //##ModelId=423E93DE0236
    //##Documentation
    //## ��չ�ֶε�����ID��־
    int    m_aiExtFieldID[MAX_EXT_FIELD_NUM];

    //##ModelId=423E93C201A0
    //##Documentation
    //## ��չ�ֶ�ֵ����
    char   m_asExtValue[MAX_EXT_FIELD_NUM][MAX_BYTES_PER_STRING];

    //##ModelId=424D0FFE0152
    //##Documentation
    //## ��������ʶ
    int m_iSwitchID;
   
    //##ModelId=4255EDD40362
    //##Documentation
    //## ҵ���(�ڲ�����):
    int m_iBusinessKey;

    //##ModelId=423E8C540028
    //##Documentation
    //## �����Ϣ
    //int m_iOrgCharge[MAX_ORG_CHARGE_NUM]; 
    long m_lOrgCharge[MAX_ORG_CHARGE_NUM];

    ChargeInfo m_oChargeInfo[MAX_CHARGE_NUM];
    
    StdChargeInfo m_oStdChargeInfo[MAX_CHARGE_NUM];
    
    //##ModelId=426481FE004B
    //##Documentation
    //## �Ʒѷ�������ά��֯����
    int m_iBillingOrgID;

    //##ModelId=4282D21C006E
    //##Documentation
    //## ��������:  1-���л���  2-���л���
    int m_iCallTypeID;

    //##ModelId=428D37320285
    int m_iPricingCombineID;
    
  
    // ����ӱ6��16������,�����޷�����VSS,������
    // m_iEventSourceType�ֶ�ֻ��Ԥ���������ʹ��,��Ԥ����ֵ,��Ϊ�Ƿ�������ش�����ж�����
    // ȡֵ����:
    // ԭʼ�ļ�:1
    // ��������:2
    // Ԥ��������������:3
    // ���۲����������:4
    // �ص�����:5
    // ����:6
    int m_iEventSourceType;
    
    //��ѱ�־
    int m_iNoBillFlag;

	//�ƶ���������ʶ  
	char  m_sMSC[MAX_MSC_LEN];
	
	//С���ű�ʶ  
	char  m_sLAC[MAX_LAC_LEN];
	char  m_sCalledMSC[MAX_MSC_LEN];

	char  m_sCalledLAC[MAX_LAC_LEN];
	char  m_sCalledCellID[MAX_CELLID_LEN];
	//��վ��   
	char  m_sCellID[MAX_CELLID_LEN];

	//��̬���κ�   
	char m_sRomingNBR[MAX_ROMING_NBR_LEN];
    
    
    //##ModelId=428956460086
    StdEventExt m_oEventExt;

	//## SPԭʼ��ʶ��
	char m_sSPFLAG[MAX_SPFLAG_LEN];
	
    //SP��ʶ
    int m_iSPID;
    
    //## 3G CS�򲹳�ҵ���ʶ
    int m_iSuppleServiceCode;
    
    //## 3G CS��CUGҵ���ʶ;
    int m_iCUGCode;
    
    char m_sReservedField[MAX_RESERVED_FIELD_NUM][MAX_RESERVED_FIELD_LEN];
    
    //add by yangks 2006.12.31  �������μ��㹦��(�ӱ�����)
    //�л�����
    int m_iTimes;
    
    int m_iNetworkID;
    
    int m_iFlowID;
    
    //add by yangks 2007.03.09 for JS
    char m_sGateWay[MAX_GATE_WAY_LEN];      /*������Ϣ*/
    int  m_iCallingPA;                    /*����pa��*/
    int  m_iCalledPA;                     /*����pa��*/
    char m_sCallingRP[MAX_RP_STR_LEN];        /*���л�վ*/
    char m_sCalledRP[MAX_RP_STR_LEN];         /*���л�վ*/
    int  m_iCallingRPArea;                //���л�վ����(����������Ϣ������pa������վ����)
    int  m_iCalledRPArea;                 //���л�վ����
    char m_sSessionID[MAX_SESSION_ID_LEN];              /*����ҵ��ĻỰ��ʶ*/
    char m_sOrgThirdPartyNBR[MAX_THIRD_PARTY_NBR_LEN];  /*ԭʼ�ĵ���������*/
    char m_sOrgThirdPartyPhysicalNBR[MAX_THIRD_PARTY_NBR_LEN];
    
    //���������ɼ��ṩ. Ҫ���ֶ����,�����˻���ʹ��
    //�ļ������. ��ԭʼ�����ļ���,��1��ʼ����ı��ʻ��������.
    int  m_iEventSeq;
    
    //���ݼƷ�(ԭʼ����,��Ҫ���)
    char m_sDomainName[MAX_DOMAIN_NAME_LEN]; //DOMAIN_NAME:   ����    
    int  m_iGroupID;                         //GROUP_ID:      �Ʒ����
    int  m_iRoamFlag;                        //ROAM_FLAG :    ���α�־
    char m_sBindAttr[MAX_BIND_ATTR_LEN];     //BIND_ATTR :    ������
    int  m_iServiceType;    //SERVICE_TYPE:  ��������
    int  m_iUserType;       //USER_TYPE :    �û�����
    char m_sAreaNo[MAX_AREA_NO_LEN];         //AREANO:        �û�����
    long m_lSepRecID;       //SEP_REC_ID:    ��ֻ�����ʶ,��һ����¼�����ticket_id,����Ĳ�ֻ�����¼�ο�������TICKET_ID
    int  m_iSepRecSeq;      //SEP_REC_SEQ:   ��ּ�¼����,��rec_idһͬ��Ϊ��ֻ���������
    long m_lResourceID;     //RESOURCEID :   �����Դ,�ָ��Ļ�����¼����ԭʼ������ticket_id��
    int  m_iQueryFlag;      //QUERY_FLAG :   ��ѯ��ʶ,
                            //// 0��δ����ֵ�ԭʼ����,��ʼ���л���Ĭ��Ϊ0,δ�μ��ײ͵Ķ���Ҫ�Ʒ�;
                            //// 1��������ֺ�ԭʼ����,���Ʒ�; 2�����
    
    int  m_iProcessID;

#ifdef EVENT_CHARGE_ACCU_EXCEEDED
	AccuItemInfo m_oAccuInfo[MAX_ACCU_NUM];
#endif
    char m_sAccuRecord[MAX_ACCU_RECORD_LEN];    //�ۻ����켣 wuan 2010-09-08
};



//ģ�����ʱʹ��,�������޸�Ϊ�ⲿ����
struct StdAcctItem {
	int iFileID; //�ļ�����ʱ����Ŀ��Ҫ���ˡ�
	long lEventID;//�ɸ��¼���������Ŀ,�¼�����ʱ����ĿҲ��Ҫ���ˡ�
	long iServID;
	long iAcctID;
	int iOrgID;
	int iBillingCycleID;
	int iAcctItemTypeID;
	long lCharge;
	int iCombineID; //�������ID
	char sStateDate[16]; //State Date
	int iItemGroupID;//������Ŀ��ķ���ֵ
};

#endif /* STDEVENT_H_HEADER_INCLUDED_BDB42691 */

