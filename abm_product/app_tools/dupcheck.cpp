/*VER: 1*/ 
/*
 *   �ѻ��ص����� (�������л��������������ݼƷ�, ��Ϣ̨)
 *
 */
#include <stdio.h>
#include <stdlib.h>

#ifdef DEF_LINUX
#include <getopt.h>
#endif

#include "ticket.h"
#include "billcommon.h"
#include "ascdate.h"
#include "InterfaceServ.h"

#define SPAN_BUFF_LEN 60000

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

typedef struct TDupTicket {
	long lTicketID;
	char sCallingNbr[21];
	char sCalledNbr[21];
	char sStartTime[15];
	char sEndTime[15];
	int iDuration;
	int iExchID;
}TDupTicket;

class TimeOrderSpan {
public:
	TimeOrderSpan ();
	~TimeOrderSpan ();
	
	/*
	 * �������
	 */
	void clear ();

	/*
	 * ���Ի�λ��(�ص��±�)
	 */
	void resetPos ();

	/*
	 * �������, ���������ұ߽�sEndTime
	 * ����: 1--���䷢������ 0--����û�н��� -1--����ʧ��
	 */
	int insert (TicketRead *ticket_read); 

	/* 
	 * �ж��Ƿ��ص�����ȡ�ص���Ϣ
	 * iNormalTicketID: ����������ID,����һ�Ż����뵱ǰ������
	 * iDupType: 0--�����ص�  ���������ص�
	 */
	int getDupInfo (int *iNormalTicketID, int *iDupType);
	
private:
	int iCurPos;
	bool ifDup;
	char sEndTime[20];	// �����յ�
	int TicketCount;	// ������������Ļ�����
	TDupTicket cTicket[SPAN_BUFF_LEN]; // ������������Ļ���
	
};

void SaveDupTicket(TDupTicket *dup_ticket, TicketRead *ticket_read)
{
	dup_ticket->lTicketID = ticket_read->lTicketID;
	dup_ticket->iDuration = ticket_read->iDuration;
	dup_ticket->iExchID   = ticket_read->iExchID;

	strcpy (dup_ticket->sCallingNbr, ticket_read->sCallingNbr);
	strcpy (dup_ticket->sCalledNbr,  ticket_read->sCalledNbr);
	strcpy (dup_ticket->sStartTime,  ticket_read->sStartTime);
	strcpy (dup_ticket->sEndTime,    ticket_read->sStartTime);
	AddSeconds (dup_ticket->sEndTime, dup_ticket->iDuration);
}

TimeOrderSpan::TimeOrderSpan()
{
	TicketCount = 0;
	sEndTime[0] = 0;
	iCurPos = 0;
}

TimeOrderSpan::~TimeOrderSpan()
{

}

void TimeOrderSpan::clear()
{
	TicketCount = 0;
	sEndTime[0] = 0;
	iCurPos = 0;
	ifDup = false;
}

int TimeOrderSpan::insert(TicketRead *ticket_read)
{
	char sTicketEndTime[20];

	if (TicketCount >= SPAN_BUFF_LEN) {
		pprintf (0,0,"[ERROR]: Dup ticket buffer full. buffer size:%d\n", SPAN_BUFF_LEN);
		return -1;
	}

	/* new span ==> no dup */
	if (TicketCount==0 || strcmp (ticket_read->sStartTime, sEndTime)>=0 ) {

		//cTicket[0] = ticket;
		SaveDupTicket (&cTicket[0], ticket_read);
		strcpy (sEndTime, cTicket[0].sEndTime);
		
		TicketCount = 1;
		ifDup = false;
		return 0;
	}

	/* stcmp (ticket.sStartTime, sEndTime) < 0 ===> dup */
	//cTicket[TicketCount] = ticket;
	SaveDupTicket (&cTicket[TicketCount], ticket_read);

	strcpy (sTicketEndTime, cTicket[TicketCount].sEndTime);
	if (strcmp (sEndTime, sTicketEndTime) < 0)
		strcpy (sEndTime, sTicketEndTime);

	TicketCount++;
	ifDup = true;
	return 1;
}

void TimeOrderSpan::resetPos ()
{
	iCurPos = 0;
}


/*****************************************************************************************
 DupType ����ֵ��: (�������ļ���)
  
    ����dupType   ������ͬ   ͨ�����桡��ʼʱ����ͬ��ʱ����ͬ��������ͬ����������ʶ��ͬ
  ���ص��� 0�������� 

��ȫ�ص� ��1         �ǡ��������ǡ����������ǡ������������ǡ�������--          --  
�����ص� ��2   �������ǡ���������          --            --        --          --

��ȫ�ص�   3         �ǡ��������ǡ����������ǡ������������ǡ��������ǡ�����������
��ȫ�ص�   4         �ǡ��������ǡ����������ǡ������������ǡ���������          --
��ȫ�ص�   5         �ǡ��������ǡ����������ǡ������������ǡ�������--  ���������ǡ�

�����ص�   6         �ǡ��������ǡ�������  --            --  �������ǡ�����������
�����ص�   7         �ǡ��������ǡ���������--������������--����������          --
�����ص�   8         �ǡ��������ǡ���������--������������--��      --������������
�����ص�   9         �ǡ��������ǡ���������--�������������ǡ�      �ǡ���������--
******************************************************************************************/
int TimeOrderSpan::getDupInfo (int *iNormalTicketID, int *iDupType)
{
	int i;
	char sTicketEndTime[20];

	if (ifDup == false) {
		*iDupType = 0;
		return 0;
	}

	for (; iCurPos<TicketCount-1; ) {
		strcpy (sTicketEndTime, cTicket[iCurPos].sEndTime);

		if (strcmp (cTicket[TicketCount-1].sStartTime, sTicketEndTime) >= 0) {    /*****/
			iCurPos++;
			continue;							    /********/
		}
												  /****/
		int iSameCalled = strcmp (cTicket[TicketCount-1].sCalledNbr, cTicket[iCurPos].sCalledNbr);
		int iSameDuration = cTicket[TicketCount-1].iDuration - cTicket[iCurPos].iDuration;
		int iSameStartTime = strcmp (cTicket[TicketCount-1].sStartTime, cTicket[iCurPos].sStartTime);
		int iSameExchID = cTicket[TicketCount-1].iExchID - cTicket[iCurPos].iExchID;
	
		*iNormalTicketID = cTicket[iCurPos].lTicketID;

		iCurPos++;

		if (iSameStartTime==0 && iSameDuration==0 && iSameCalled==0 && iSameExchID==0) {// tttt
			return (*iDupType = 3); 
		}
		if (iSameStartTime==0 && iSameDuration==0 && iSameCalled==0 && iSameExchID!=0) {// tttf
			return (*iDupType = 4); 
		}
		if (iSameStartTime==0 && iSameDuration==0 && iSameCalled!=0 && iSameExchID!=0) {// ttff
			return (*iDupType = 5); 
		}
		if (iSameStartTime!=0 && iSameDuration!=0 && iSameCalled==0 && iSameExchID==0) {// fftt
			return (*iDupType = 6); 
		}
		if (iSameStartTime!=0 && iSameDuration!=0 && iSameCalled==0 && iSameExchID!=0) {// fftf
			return (*iDupType = 7); 
		}
		if (iSameStartTime!=0 && iSameDuration!=0 && iSameCalled!=0 && iSameExchID==0) {// ffft
			return (*iDupType = 8); 
		}
		if (iSameStartTime!=0 && iSameDuration==0 && iSameCalled==0 && iSameExchID!=0) {// fttf
			return (*iDupType = 9); 
		}
		if (iSameStartTime==0 && iSameDuration==0 && iSameCalled!=0 && iSameExchID!=0) {// ttff
			return (*iDupType = 1); 
		}

		return (*iDupType = 2);
	}

	return -1;

}

CheckDup *dup_checker;
TimeOrderSpan time_span;
TOCIDatabase gpDBLink;
static int iCounter = 0;
static int iDupCounter = 0;

void InitAll()
{
	ConnectOciDB(gpDBLink);

        pprintf (0,0,"[MESSAGE]: ��ʼ��ʼ���ص����˹���\n");
	dup_checker = new CheckDup();
	
	time_span.clear ();
	
}

void UpdateTicketState(char *sTableName, int iTicketID, char *sState)
{
	char sSqlcode[500];
	TOCIQuery qry(&gpDBLink);

	sprintf (sSqlcode, "update %s set state=:v_state, state_date=sysdate where ticket_id=:v_ticket_id", sTableName);
	qry.SetSQL (sSqlcode);
	qry.SetParameter ("v_state", sState);
	qry.SetParameter ("v_ticket_id", iTicketID);
	qry.Execute ();
	qry.Commit ();
}

void InsertDupResult(char *sTableName, int iDupTicketID, int iTicketID, int iDupType, int iTicketType)
{
	char sSqlcode[500];
	TOCIQuery qry(&gpDBLink);

	sprintf (sSqlcode, "insert into check_dup_result(table_name, dup_ticket_id, normal_ticket_id, dup_type, dup_ticket_type) values (:v_table_name, :v_dup_ticket_id, :v_ticket_id, :v_dup_type, :v_ticket_type)");
	qry.SetSQL (sSqlcode);
	qry.SetParameter ("v_table_name", sTableName);
	qry.SetParameter ("v_dup_ticket_id", iDupTicketID);
	qry.SetParameter ("v_ticket_id", iTicketID);
	qry.SetParameter ("v_dup_type", iDupType);
	qry.SetParameter ("v_ticket_type", iTicketType);
	qry.Execute ();
	qry.Commit ();
}

int GetDupTableName(char *sTableName, int iBillingCycleID, char *sTableType)
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

	char sTableType[20], sTableName[200], sPreCallingNbr[30], sFullUndoServID[20], sTemp[20];
	int opt, iNormalTicketID, iDupType, ret, iFlowID=-1, iBillTaskID, iTableType;
	TicketRead *ticket_read;
	
	while((opt=getopt(argc,argv,"t:f:"))!=EOF)
	{
		switch(opt)
		{
			case 'f':
				iFlowID = atoi(optarg);
				sprintf (sTemp, "BILLFLOW_ID=%d", iFlowID);
				putenv (sTemp);
				break;
			case 't':
				sTableName[0] = 0;
				/* ���� */
			        if ( strstr( optarg,"TS") != NULL || strstr(optarg,"ts")!=NULL ) {
					iTableType = TAB_TS;
					strcpy (sTableType, "TS");
					ticket_read = new CallTicketRead();
				}
					
				/* �л� */
			        if ( strstr( optarg,"LS") != NULL || strstr(optarg,"ls")!=NULL ) {
					iTableType = TAB_LS;
					strcpy (sTableType, "LS");
					ticket_read = new CallTicketRead();
				}

				/* ������ */
			        if ( strstr( optarg,"IN") != NULL || strstr(optarg,"in")!=NULL ) {
					iTableType = TAB_IN;
					strcpy (sTableType, "IN");
					ticket_read = new INCallTicketRead();
				}

				/* ���ݼƷ� */
			        if ( strstr( optarg,"DT") != NULL || strstr(optarg,"dt")!=NULL ) {
					iTableType = TAB_DT;
					strcpy (sTableType, "DT");
					ticket_read = new DTCallTicketRead();
				}

				/* ��Ϣ̨, ��ע�� 
			        if ( strstr( optarg,"MS") != NULL || strstr(optarg,"ms")!=NULL ) {
					iTableType = TAB_MS;
					strcpy (sTableType, "MS");
					ticket_read = new MSCallTicketRead();
				} */
					
					break;
		}
	}

	ConnectMonitor(1);

	if (iFlowID==-1 || strlen(sTableType)==0) {
		pprintf (0,0,"[Usage]: %s -f [flow_id] -t [ts|ls|in|dt]\n", argv[0]);
		DisconnectMonitor(PS_ABORT);
		exit (0);
	}

	InitAll ();
	InterfaceServ Serv;

	iBillTaskID = GetBillTaskID();
	pprintf (0,0,"[MESSAGE]: env: BILL_TASK_ID=%d\n", iBillTaskID);
	BillTask bt(iBillTaskID);
	bt.First ();

	/* ������ȡ���� */
        ticket_read->AddCondition ("and state='T00' order by calling_area_code asc, calling_nbr asc, start_time asc");

	while (bt.Next()) {

	/* ���� table_list_cycle ��ñ���,��������������� */
	if (GetDupTableName (sTableName, bt.cycle->iBillingCycleID, sTableType) == 0)
		continue;

	iCounter = iDupCounter = 0;

	pprintf (0,0,"[MESSAGE]: ��ʼ�򿪻�����: %s\n", sTableName);
        ticket_read->SetTableName (sTableName);
        ticket_read->Open ();

        pprintf (0,0,"[MESSAGE]: ��ʼ�ص����˴���...\n");
        strcpy (sPreCallingNbr, " ");
        strcpy (sFullUndoServID, " ");
	time_span.clear ();
	while (ticket_read->Next ()) {
		
		ticket_read->getBaseInfo ();

		if (++iCounter%10000 == 0)
			pprintf (0,0,"[MESSAGE]: �Ѵ�����%d��, �����ص�%d��\n", iCounter, iDupCounter);
		
		/* ���ϴδ�������бȽ�, ��ͬ����, ���ػ������ */
		if (strcmp (ticket_read->sCallingNbr, sPreCallingNbr) != 0) {
			time_span.clear ();
			strcpy (sPreCallingNbr, ticket_read->sCallingNbr);
		}

		/* ���ϴδ��������dup_type������serv_id�Ƚ�, �����ϴε�serv_id, ���ػ������ */
		if (strcmp (ticket_read->sServID, sFullUndoServID) == 0) {
			time_span.clear ();
		} else
			strcpy (sFullUndoServID, " ");

		/* �ı䵱ǰ���е�ʱ������ʱ���������ص����ص���ʶ */
		if (time_span.insert (ticket_read) < 0) {
			pprintf (0,0,"[ERROR]: ��ǰ����%s,����ʱ������ʧ��\n", sPreCallingNbr);
			DisconnectMonitor(PS_ABORT);
			exit (-1);
		}

		/* �ж��Ƿ��ص�, ret==0���ص�, ret==-1�ص�����������(Ʃ��A,B,C����, C��A��,Ҳ��B��) */
		time_span.resetPos ();
		while ((ret = time_span.getDupInfo (&iNormalTicketID, &iDupType)) > 0) {

			bool b_ret;

			// ĳЩserv_id,����dup_type(==0)������Ҫ�ص�����
			Serv.SearchServIDInServ(ticket_read->sServID,ticket_read->iServSeqNbr);
			b_ret = dup_checker->IsUndoServ (ticket_read->sServID,0,Serv.serv->iProductPackageID);
			if (b_ret == true) {
				strcpy (sFullUndoServID, ticket_read->sServID);
				time_span.clear ();
				break;
			}

			/* �ص� */
			int iDupTicketType=ticket_read->iTicketType;
			if (dup_checker->GetDupCallCatg(ticket_read->iTicketType,iDupType,&iDupTicketType)){
				
				// ĳЩserv_id,ĳЩdup_type, ����Ҫ�ص�����
				if (iTableType==TAB_TS || iTableType==TAB_LS) {
					b_ret = dup_checker->IsUndoServ
						(ticket_read->sServID,iDupType,Serv.serv->iProductPackageID);
				} else { 
					b_ret = dup_checker->IsUndoServ 
						(ticket_read->sServID, iDupType, -1);
				}

				if ( !b_ret) {
				
					ticket_read->iTicketType = iDupTicketType;

					UpdateTicketState(sTableName,ticket_read->lTicketID,"DUP");
					InsertDupResult(sTableName, ticket_read->lTicketID, iNormalTicketID, iDupType, iDupTicketType);
					
					iDupCounter++;
					/* �״��ҵ��ص�,�Ϳ��˳� */
					break;
				}
			}
		}
	}
	
	gpDBLink.Commit();
	pprintf (0,0,"[MESSAGE]: ��������%d��, �����ص�%d��\n", iCounter, iDupCounter);

	}

	pprintf (0,0,"[MESSAGE]: �������\n");

	DisconnectMonitor(PS_NORMALDONE);

}catch (TException &e) {
	pprintf (0,0,"[Exception]: %s.\n", e.GetErrMsg());
	DisconnectMonitor(PS_NORMALDONE);
	exit (-1);
}

}

