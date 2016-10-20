#ifndef _DCC_PUBLIC_H_
#define _DCC_PUBLIC_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <dcc_ra.h>
//#include "abmsendcmd.h"
#include "DccMsgParser.h"
#include "abmobject.h"
#include "abmcmd.h"
#include "errorcode.h"
#include "LogV2.h"

#define SECS_70_YEARS 2207520000   //=70*365*24*60*60,70���������û����366������
#define MSG_SIZE 4096

//��socket���պͷ�����Ϣ��ͳһ���ݽṹ
typedef struct StruMqMsg {
    long m_lMsgType;
    char m_sMsg[MSG_SIZE];
}StruMqMsg;


//��16������ʾ����������
void disp_buf(char *buf, int size);


//����service-context-id�ĵ�һ�����ж�ҵ�����ͣ�����ҵ��Ա����
int switchCmd(string &sCmd);


//��ȡdcc������Ĺ�����Ϣ������ṹ�壬�ɹ�����������Ϣ��ҵ�����ͣ�������
int getCCRHead(DccMsgParser *poDcc, ABMCCR *pCCR);


//���dccӦ����Ĺ�����Ϣ��������Ϣȡ�Թ���ͷ���ṹ�壬�ɹ�����0��ʧ��-1
int setCCAHead(DccMsgParser *poDcc, ABMCCA *pCCA);


//���ϲ�ѯ������
int parserInfoQryR(DccMsgParser *poDcc, ABMCCR *oSendBuf);
int packInfoQryA(DccMsgParser *poDcc, ABMCCA *oSendBuf);


//3.4.2.1֧��
int parserChargeR(DccMsgParser *poDcc, ABMCCR *poCCR);
int packChargeA(DccMsgParser *poDcc, ABMCCA *poCCA);

//3.4.5.1
int AllocateBalanceSvcR(DccMsgParser *poDcc, ABMCCR *poCCR);
int AllocateBalanceSvcA(DccMsgParser *poDcc, ABMCCA *poCCA);

int AllocateBalanceSvcR(DccMsgParser *poDcc, ABMCCR *poCCR);
int AllocateBalanceSvcA(DccMsgParser *poDcc, ABMCCA *poCCA);
//3.4.6.16
int RechargeBillQueryR(DccMsgParser *poDcc, ABMCCR *poCCR);
int RechargeBillQueryA(DccMsgParser *poDcc, ABMCCA *poCCA);

//3.4.6.15
int QueryPayLogByAccoutR(DccMsgParser *poDcc, ABMCCR *poCCR);
int QueryPayLogByAccoutA(DccMsgParser *poDcc, ABMCCA *poCCA);

//3.4.6.14
int RechargeRecQueryR(DccMsgParser *poDcc, ABMCCR *poCCR);
int RechargeRecQueryA(DccMsgParser *poDcc, ABMCCA *poCCA);

//3.4.5.5
int ReqReverseDeductSvcR(DccMsgParser *poDcc, ABMCCR *poCCR);
int ReqReverseDeductSvcA(DccMsgParser *poDcc, ABMCCA *poCCA);



#endif