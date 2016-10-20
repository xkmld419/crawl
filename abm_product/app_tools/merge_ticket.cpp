/*VER: 1*/ 
/*
 *   ���������ϲ�
 *   
 *   ruanyj create  2004/04/01
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ticket.h"
#include "billcommon.h"
#include "ascdate.h"
#include "TicketCal.h"
#include "MakeBill.h"

#define SPAN_BUFF_LEN 10000
#define MAX_CFG_COUNTS 100

#include "TOCIQuery.h"
#include "ConnectDB.h"
#include "CheckDup.h"
#include "TicketRead.h"
#include "INCallTicketRead.h"
#include "DTCallTicketRead.h"
#include "billing_cycle.h"
#include "mntapi.h"
#include "../cma/monitor.h"
#include "CallTicketRead.h"
#include "CallTicketSave.h"

#define TAB_TS	1
#define TAB_LS	2
#define TAB_IN	3
#define TAB_DT	4
#define TAB_MS	5

typedef struct TMergeTicketCfg {
	int iExchID;
	int iMinInterval;
	int iMaxInterval;
}TMergeTicketCfg;

typedef struct TTicketMainField {
	long lTicketID;
	char sCallingNbr[21];
	char sCalledNbr[21];
	char sStartTime[15];
	char sEndTime[15];
	int iDuration;
	int iCharge;
	int iAddCharge;
	int iExchID;
}TTicketMainField;

class TimeOrderSpan {

public:
	TimeOrderSpan ();
	~TimeOrderSpan ();
	
	/*
	 * �������
	 */
	void clear ();

	/*
	 * �������, ���������ұ߽�sEndTime
	 * ����: 1--���䷢������ 0--����û�н��� -1--����ʧ��
	 */
	int insert (TicketRead *ticket_read, bool bEndFlag); 

	/* 
	 * �������ڵĻ������кϲ�
	 */
	int mergeTicket ();
	
private:
	char sCallingNbr[22];
	char sCalledNbr[22];
	int iExchID;

	char sEndTime[20];	// �����յ�
	int TicketCount;	// ������������Ļ�����
	TTicketMainField cTicket[SPAN_BUFF_LEN]; // ������������Ļ���

	CallTicketStruct m_ticket; // ���л���Ҫ�����㷨, ��Ҫ�ýṹ�������㷨����ֶ� 
	
};

CheckDup *dup_checker;
TimeOrderSpan time_span;
TOCIDatabase gpDBLink;
static int iCounter = 0;
static int iMergeCounter = 0;
static TMergeTicketCfg gCfg[MAX_CFG_COUNTS];
static int iCfgCounts;
static char gsTableName[200];
static int giTableType;
MakeBillController * make_bill_controller;

void SaveMergeTicket(TTicketMainField *f_ticket, TicketRead *ticket_read)
{
	f_ticket->lTicketID = ticket_read->lTicketID;
	f_ticket->iDuration = ticket_read->iDuration;
	f_ticket->iExchID   = ticket_read->iExchID;
	f_ticket->iCharge   = ticket_read->iCharge;
	if (giTableType==TAB_TS || giTableType==TAB_LS) {
	    f_ticket->iAddCharge   = ticket_read->iAddCharge;
	}

	strcpy (f_ticket->sCallingNbr, ticket_read->sCallingNbr);
	strcpy (f_ticket->sCalledNbr,  ticket_read->sCalledNbr);
	strcpy (f_ticket->sStartTime,  ticket_read->sStartTime);
	strcpy (f_ticket->sEndTime,    ticket_read->sEndTime);
}

TimeOrderSpan::TimeOrderSpan()
{
	TicketCount = 0;
	sEndTime[0] = 0;
}

TimeOrderSpan::~TimeOrderSpan()
{

}

void TimeOrderSpan::clear()
{
	sCallingNbr[0] = 0;
	sCalledNbr[0] = 0;
	iExchID = -1;

	TicketCount = 0;
	sEndTime[0] = 0;
}
int TimeOrderSpan::mergeTicket ()
{

	int i;

	char sSqlcode[500];
	TOCIQuery qry(&gpDBLink);

	/* ����������2�Ż������úϲ� */
	if (TicketCount < 2) {
		return 0;
	}

	for (i=1; i<TicketCount; i++) {

		/* ��һ�Ż����ۼ�ʱ���ͷ��� */
		cTicket[0].iDuration += cTicket[i].iDuration;
		cTicket[0].iCharge += cTicket[i].iCharge;
		if (giTableType==TAB_TS || giTableType==TAB_LS) {
		    cTicket[0].iAddCharge += cTicket[i].iAddCharge;
	    }
		sprintf (sSqlcode, "update %s set state='MRG', state_date=sysdate where ticket_id=%d", gsTableName, cTicket[i].lTicketID);
		qry.Close ();
		qry.SetSQL (sSqlcode);
		qry.Execute ();

	}

	/* ���л���Ҫ�ѷ������� */
	if (giTableType==TAB_TS || giTableType==TAB_LS) {

		/* ��¼��ʱ������(��) */
		int iOldCharge=0, iOldAddCharge=0;
		iOldCharge   = cTicket[0].iCharge;
		iOldAddCharge = cTicket[0].iAddCharge;

		/* ����ʱ��, ������� */
		m_ticket.iDuration = cTicket[0].iDuration;
		PreRating ((CallTicketStruct *) &m_ticket);
		   Rating ((CallTicketStruct *) &m_ticket);

		/* ��¼�·���(�ǵ���һ���»�����) */
		cTicket[0].iCharge = m_ticket.iCharge;
		cTicket[0].iAddCharge = m_ticket.iAddCharge;

		/* �����ò�, ���º���,������Ŀ���� */
		m_ticket.iCharge -= iOldCharge;
		m_ticket.iAddCharge -= iOldAddCharge;
		MakeBill * make_bill = make_bill_controller->Get(m_ticket.iBillingCycleID);
		if ( make_bill){
			make_bill->AddChargeItem((CallTicketStruct *) &m_ticket);
		}
	}

	/* �Ե�һ���������и��� */
	sprintf (sSqlcode, "update %s set duration=%d, charge=%d, add_charge=%d where ticket_id=%d", gsTableName, cTicket[0].iDuration, cTicket[0].iCharge, cTicket[0].iAddCharge, cTicket[0].lTicketID);
	qry.Close ();
	qry.SetSQL (sSqlcode);
	qry.Execute ();

	gpDBLink.Commit ();

	return TicketCount;

}

int TimeOrderSpan::insert(TicketRead *ticket_read, bool bEndFlag)
{
	int i, ret;
	char sTicketEndTime[20];

	/* �����ֹ��־,�������ڵĻ����ϲ�,ˢ����� */
	if (bEndFlag == true) {
		ret = mergeTicket ();
		return ret;
	}

	if (TicketCount >= SPAN_BUFF_LEN) {
		pprintf (0,0,"[ERROR]: merge ticket buffer full. buffer size:%d\n", SPAN_BUFF_LEN);
		return -1;
	}

	/* ��ͬ�ı���,exch_id,����, ��ԭ�����л����ϲ�, �������,д��û��� */
	if (TicketCount==0 || strcmp (ticket_read->sCalledNbr, sCalledNbr)!=0 || ticket_read->iExchID!=iExchID || strcmp (ticket_read->sCallingNbr, sCallingNbr)!=0 ) {

		ret = mergeTicket ();

		strcpy (sCalledNbr, ticket_read->sCalledNbr);
		strcpy (sCallingNbr, ticket_read->sCallingNbr);
		iExchID = ticket_read->iExchID;
		
		strcpy (sEndTime, ticket_read->sStartTime);
		AddSeconds (sEndTime, ticket_read->iDuration);

		SaveMergeTicket (&cTicket[0], ticket_read);
		if (giTableType==TAB_TS || giTableType==TAB_LS) {
			m_ticket = *((CallTicketStruct *)ticket_read->getTicketAddr());
		}
		TicketCount = 1;
		return ret;
	}

	/* ���ҵ�ǰ exch_id �������� */
	for (i=0; i<iCfgCounts; i++) {
		if (gCfg[i].iExchID == ticket_read->iExchID)
			break;
	}
	
	if (i < iCfgCounts) {

		/* ȡ�õ�ǰ�������������ֹʱ���ʱ��� */
		int iDiffTime = iGetTime(ticket_read->sStartTime) - iGetTime(sEndTime);

		/* ʱ���������� */
		if (iDiffTime>=gCfg[i].iMinInterval && iDiffTime<=gCfg[i].iMaxInterval) {

			/* �����������ֹʱ��Ϊ����������ֹʱ�� */
			strcpy (sTicketEndTime, ticket_read->sStartTime);
			AddSeconds (sTicketEndTime, ticket_read->iDuration);
			if (strcmp (sTicketEndTime, sEndTime) > 0) {
				strcpy (sEndTime, sTicketEndTime);
			}

			/* ���汾���������� */
			SaveMergeTicket (&cTicket[TicketCount], ticket_read);
			if (giTableType==TAB_TS || giTableType==TAB_LS) {
				m_ticket = *((CallTicketStruct *)ticket_read->getTicketAddr());
			}
			TicketCount++;
			return 0;

		/* ʱ���������� */
		} else {

			/* �ϲ�������ԭ���Ļ��� */
			ret = mergeTicket ();

			/* �����������ֹʱ��Ϊ����������ֹʱ�� */
			strcpy (sTicketEndTime, ticket_read->sStartTime);
			AddSeconds (sTicketEndTime, ticket_read->iDuration);
			strcpy (sEndTime, sTicketEndTime);

			/* �������, ���汾���� */
			SaveMergeTicket (&cTicket[0], ticket_read);
			if (giTableType==TAB_TS || giTableType==TAB_LS) {
				m_ticket = *((CallTicketStruct *)ticket_read->getTicketAddr());
			}
			TicketCount = 1;

			return ret;
		}
	}

	return 0;
}

void InitAll()
{
	char sSqlcode[500];

	ConnectOciDB(gpDBLink);

        pprintf (0,0,"[MESSAGE]: ��ʼ��Merge_Ticket_cfg...\n");
	iCfgCounts = 0;
	TOCIQuery qry(&gpDBLink);
	sprintf (sSqlcode,"select exch_id, min_interval, max_interval from merge_ticket_cfg order by exch_id");
	qry.Close ();
	qry.SetSQL (sSqlcode);
	qry.Open ();
	while (qry.Next ()) {
		if (++iCfgCounts > MAX_CFG_COUNTS) {
			printf ("[ERROR]: merge_ticket_cfg ���ó���%d��\n", MAX_CFG_COUNTS);
			exit (-1);
		}
		gCfg[iCfgCounts-1].iExchID = qry.Field ("exch_id").asInteger ();
		gCfg[iCfgCounts-1].iMinInterval = qry.Field ("min_interval").asInteger ();
		gCfg[iCfgCounts-1].iMaxInterval = qry.Field ("max_interval").asInteger ();
	}
	if (iCfgCounts == 0) {
		printf ("[WARNING]: û������ merge_ticket_cfg\n");
		exit (-2);
	}
	
	time_span.clear ();

        //��ʼ�����ʴ���
	WriteMsg(1, "[MESSAGE]: ��ʼ�����ʴ���...\n");
	make_bill_controller = new MakeBillController;
	
}

void UpdateTicketState(char *sTableName, int iTicketID, char *sState)
{
	char sSqlcode[500];
	TOCIQuery qry(&gpDBLink);

	sprintf (sSqlcode, "update %s set state='%s', state_date=sysdate where ticket_id=%d", sTableName, sState, iTicketID);
	qry.SetSQL (sSqlcode);
	qry.Execute ();
	qry.Commit ();
}

int getTableName(char *sTableName, int iBillingCycleID, char *sTableType)
{
	char sSqlcode[500];
	TOCIQuery qry(&gpDBLink);
	
	sprintf (sSqlcode, "select table_name from table_list_cycle where billing_cycle_id=%d and table_type='%s'", iBillingCycleID, sTableType);
	qry.SetSQL (sSqlcode);
	qry.Open ();

	if (qry.Next()) {
		strcpy (sTableName, qry.Field("table_name").asString());
		return 1;
	}

	return 0;
}

main (int argc, char *argv[])
{
try {

	char sTableType[20], sTemp[20];
	int opt, iNormalTicketID, iDupType, ret, iExchID=-1, iBillTaskID;
	TicketRead *ticket_read;
	
	while((opt=getopt(argc,argv,"t:"))!=EOF)
	{
		switch(opt)
		{
			case 't':
				gsTableName[0] = 0;
				/* ���� */
			        if ( strstr( optarg,"TS") != NULL || strstr(optarg,"ts")!=NULL ) {
					giTableType = TAB_TS;
					strcpy (sTableType, "TS");
					ticket_read = new CallTicketRead();
				}
					
				/* �л� */
			        if ( strstr( optarg,"LS") != NULL || strstr(optarg,"ls")!=NULL ) {
					giTableType = TAB_LS;
					strcpy (sTableType, "LS");
					ticket_read = new CallTicketRead();
				}

				/* ������ */
			        if ( strstr( optarg,"IN") != NULL || strstr(optarg,"in")!=NULL ) {
					giTableType = TAB_IN;
					strcpy (sTableType, "IN");
					ticket_read = new INCallTicketRead();
				}

				/* ���ݼƷ� */
			        if ( strstr( optarg,"DT") != NULL || strstr(optarg,"dt")!=NULL ) {
					giTableType = TAB_DT;
					strcpy (sTableType, "DT");
					ticket_read = new DTCallTicketRead();
				}

				/* ��Ϣ̨, ��ע�� 
			        if ( strstr( optarg,"MS") != NULL || strstr(optarg,"ms")!=NULL ) {
					giTableType = TAB_MS;
					strcpy (sTableType, "MS");
					ticket_read = new MSCallTicketRead();
				} */
					
					break;
		}
	}

	ConnectMonitor(1);

	if (giTableType == 0) {
		pprintf (0,0,"[Usage]: %s -t [ts|ls|in|dt]\n", argv[0]);
		DisconnectMonitor(PS_ABORT);
		exit (0);
	}

	InitAll ();

	iBillTaskID = GetBillTaskID();
	pprintf (0,0,"[MESSAGE]: env: BILL_TASK_ID=%d\n", iBillTaskID);
	BillTask bt(iBillTaskID);
	bt.First ();

	while (bt.Next()) {

	/* ���� table_list_cycle ��ñ���,��������������� */
	if (getTableName (gsTableName, bt.cycle->iBillingCycleID, sTableType) == 0)
		continue;

	iCounter = iMergeCounter = 0;

	pprintf (0,0,"[MESSAGE]: ��ʼ�򿪻�����: %s\n", gsTableName);
	//CallTicketRead ticket_read;
        ticket_read->SetTableName (gsTableName);
	if (giTableType==TAB_TS || giTableType==TAB_LS) {

		ticket_read->AddCondition ("and state='T00' and switch_id in (select distinct exch_id from merge_ticket_cfg) order by calling_area_code asc, calling_nbr asc, switch_id asc, start_time asc");

	} else {

		ticket_read->AddCondition ("and state='T00' and exch_id in (select distinct exch_id from merge_ticket_cfg) order by calling_area_code asc, calling_nbr asc, exch_id asc, start_time asc");

	}
        ticket_read->Open ();

        pprintf (0,0,"[MESSAGE]: ��ʼ�����ϲ�����...\n");
	time_span.clear ();
	while (ticket_read->Next ()) {
		
		ticket_read->getBaseInfo ();

		if (++iCounter%50000 == 0)
			pprintf (0,0,"[MESSAGE]: �Ѵ�����%d��, �ϲ�%d��\n", iCounter, iMergeCounter);
		
		ret = time_span.insert (ticket_read, false);
		if (ret < 0) {
			DisconnectMonitor(PS_ABORT);
			exit (-1);
		} else {
			iMergeCounter += ret;
		}
		
	}
	
	ret = time_span.insert (NULL, true);
	iMergeCounter += ret;

	make_bill_controller->FlushAll();
	gpDBLink.Commit();
	pprintf (0,0,"[MESSAGE]: ��������%d��, �ϲ�%d��\n", iCounter, iMergeCounter);

	}

	pprintf (0,0,"[MESSAGE]: �������\n");

	DisconnectMonitor(PS_NORMALDONE);

}catch (TException &e) {
	pprintf (0,0,"[Exception]: %s.\n", e.GetErrMsg());
	DisconnectMonitor(PS_NORMALDONE);
	exit (-1);
}

}

