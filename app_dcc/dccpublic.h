#ifndef _DCC_PUBLIC_H_
#define _DCC_PUBLIC_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "DccMsgParser.h"
#include "abmobject.h"
#include "abmcmd.h"
#include "errorcode.h"
#include "LogV2.h"
#include "ReadDccConf.h"
#include "TimesTenAccess.h"
//#include "../src/src_test2/dcc_ra.h"
#include "DccSql.h"

#define SECS_70_YEARS 2208988800   //70�������
//#define SECS_70_YEARS 2207520000   //=70*365*24*60*60,70���������û����366������
#define MSG_SIZE 4096
#define _SEED_NUM_ 17

//��socket���պͷ�����Ϣ��ͳһ���ݽṹ
typedef struct StruMqMsg {
    long m_lMsgType;
    char m_sMsg[MSG_SIZE];
}StruMqMsg;

//int getH2H();
//int getE2E();

//��16������ʾ����������
void disp_buf(char *buf, int size);

unsigned int getE2E();
unsigned int getH2H();

//����service-context-id(�����)��service-context-id(Ӧ���)
//�ĵ�һ�����ж�ҵ�����ͣ�����ҵ��Ա����
int switchCmd(string &sCmd, int iReqFlag);


//��ȡdcc������Ĺ�����Ϣ������ṹ�壬�ɹ�����������Ϣ��ҵ�����ͣ�������
//iReqFlag: Ĭ����1����ʾ���յ�����Ϣ��������Ϣ������ȫ������abmϵͳ����
//          ��������CCR�Ĺ��ܣ�����յ����Ƕ�Ӧ��CCAʱ,iReqFlag��0
int getDccHead(DccMsgParser *poDcc, ABMCCR *pCCR, int iReqFlag = 1, TimesTenCMD *poTTCmd = NULL);

//int getDccHead(DccMsgParser *poDcc, ABMCCR *pCCR, int iReqFlag = 1);
//���dccӦ����Ĺ�����Ϣ��������Ϣȡ�Թ���ͷ���ṹ�壬�ɹ�����0��ʧ��-1
int setDccHead(DccMsgParser *poDcc, ABMCCA *pCCA, int iReqFlag,char *sessCCR,char *srealmCCR);

//������ҵ��Ӧ����
int packCEA(DccMsgParser *poDcc, StruMqMsg *pSendMsg);
int packDWA(DccMsgParser *poDcc, StruMqMsg *pSendMsg);
int packDPA(DccMsgParser *poDcc, StruMqMsg *pSendMsg);
int packNetCtrl(DccMsgParser *poDcc, StruMqMsg *pSendMsg);

//3.4.1.1 ���ϲ�ѯ������
int parserInfoQryR(DccMsgParser *poDcc, ABMCCR *oSendBuf);
int packInfoQryA(DccMsgParser *poDcc, ABMCCA *oSendBuf);


//3.4.2.1 ֧��
int parserChargeR(DccMsgParser *poDcc, ABMCCR *poCCR);
int packChargeA(DccMsgParser *poDcc, ABMCCA *poCCA);


//3.4.2.5 ������֤�������·�
int parserRandPswdR(DccMsgParser *poDcc, ABMCCR *poCCR);
int packRandPaswA(DccMsgParser *poDcc, ABMCCA *poCCA);


//3.4.5.1
int AllocateBalanceSvcR(DccMsgParser *poDcc, ABMCCR *poCCR);
int AllocateBalanceSvcA(DccMsgParser *poDcc, ABMCCA *poCCA);

//jiancheng20110530
//3.4.5.1 --ȫ������abm��������
int packDeductBalR(DccMsgParser *poDcc, ABMCCA *poCCR);
int parserDeductBalA(DccMsgParser *poDcc, ABMCCR *poCCA);

//3.4.5.5
int ReqReverseDeductSvcR(DccMsgParser *poDcc, ABMCCR *poCCR);
int ReqReverseDeductSvcA(DccMsgParser *poDcc, ABMCCA *poCCA);
//jiancheng20110530
//3.4.5.5--ȫ������abm��������
int packRvsChargeR(DccMsgParser *poDcc, ABMCCA *poCCR);
int parserRvsChargeA(DccMsgParser *poDcc, ABMCCR *poCCA);


//3.4.6.16
int RechargeBillQueryR(DccMsgParser *poDcc, ABMCCR *poCCR);
int RechargeBillQueryA(DccMsgParser *poDcc, ABMCCA *poCCA);


//3.4.6.15
int QueryPayLogByAccoutR(DccMsgParser *poDcc, ABMCCR *poCCR);
int QueryPayLogByAccoutA(DccMsgParser *poDcc, ABMCCA *poCCA);


//3.4.6.14
int RechargeRecQueryR(DccMsgParser *poDcc, ABMCCR *poCCR);
int RechargeRecQueryA(DccMsgParser *poDcc, ABMCCA *poCCA);


//3.4.6.4 ������ѯ --- abm�����յ���Χ����
int parserQryBalDrawR(DccMsgParser *poDcc, ABMCCR *poCCR);
int packQryBalDrawA(DccMsgParser *poDcc, ABMCCA *poCCA);
//3.4.6.4 ������ѯ --- abm������������
int parserQryBalDrawA(DccMsgParser *poDcc, ABMCCR *poCCA);
int packQryBalDrawR(DccMsgParser *poDcc, ABMCCA *poCCR);

//3.4.6.13,  3.4.5.3,  3.4.5.4 ����ѯ
int QueryBalanceR(DccMsgParser *poDcc, ABMCCR *poCCR);
int QueryBalanceA(DccMsgParser *poDcc, ABMCCA *poCCA);
//jiancheng20110530
//3.4.6.13,  3.4.5.3,  3.4.5.4 ����ѯ --- abm������������
int packQryBalanceR(DccMsgParser *poDcc, ABMCCA *poCCR,char *serNUMCCR);
int parserQryBalanceA(DccMsgParser *poDcc, ABMCCR *poCCA);
/*****************************************************
 *
 *    3.4.4.1 ������ֵ VC�淶2.1  �ų� 
 *    ������ֵCCR    
 *
 *****************************************************/
//���
int DepositServCondOneR(DccMsgParser *poDcc, ABMCCR *poCCR);
int DepositServCondOneA(DccMsgParser *poDcc, ABMCCA *poCCA);
//3.4.4.3 ������ֵ VC�淶2.3 �ų� 
int ReverseDepositR(DccMsgParser *poDcc, ABMCCR *poSendBuf);
int ReverseDepositA(DccMsgParser *poDcc, ABMCCA *poSendBuf);

/****
3.4.6.5  ����
****/

int ReqBalanceSvcA(DccMsgParser *poDcc, ABMCCR *poCCA);
int ReqBalanceSvcR(DccMsgParser *poDcc, ABMCCA *poCCR);

/*************
3.4.5.2 ȫ����������
************/
int ReqPlatformSvcR(DccMsgParser *poDcc, ABMCCR *poCCR);
int ReqPlatformSvcA(DccMsgParser *poDcc, ABMCCA *poCCA);

//3.4.2.4  ֧������
int ReversePaySvcR(DccMsgParser *poDcc, ABMCCR *poSendBuf);
int ReversePaySvcA(DccMsgParser *poDcc, ABMCCA *poSendBuf);

//3.4.1.2 ����ͬ��  HSS��CCR
int QryServSynCondR(DccMsgParser *poDcc, ABMCCR *poSendBuf);
int QryServSynCondA(DccMsgParser *poDcc, ABMCCA *poSendBuf);

//3.4.1.2 ����ͬ��  ABM��CCR
int QryServSynCondRT(DccMsgParser *poDcc, ABMCCA *poCCR);
int QryServSynCondAT(DccMsgParser *poDcc, ABMCCR *poCCA);

//3.4.1.3 �����Ȩ  ABM��HSS
int PasswdIdentifyCondRT(DccMsgParser *poDcc, ABMCCA *poCCR);
int PasswdIdentifyCondAT(DccMsgParser *poDcc, ABMCCR *poCCA);

//3.4.1.6 ��½��Ȩ  �Է�ƽ̨����ABM
int LoginPasswdCCRR(DccMsgParser *poDcc, ABMCCR *poCCR);
int LoginPasswdCCRA(DccMsgParser *poDcc, ABMCCA *poCCA);

#endif