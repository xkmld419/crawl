/*VER: 2*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "EventSender.h"
//#include "BillConditionMgr.h"
#include "Environment.h"
#include "MessageQueue.h"
//#include "DistributeRule.h"
#include "Process.h"
#include "SendRecord.h"
#include "CommonMacro.h"
//#include "DataIncrementMgr.h"

const int PRICING_APP_ID = 10;

//##ModelId=41EF5A7203BF
/*
int * EventSender::send (StdEvent *event)
{
    SendRecord * pSendRecord;
    int iTemp (0);
    
    int iFromProcessId = Process::m_iFromProcID;
    event->m_iProcessID = Process::m_iProcID;

    SetEventState(1);  //�������һ�������������

    memset (m_aiProcessIDBuf, 0, sizeof (m_aiProcessIDBuf));

    if (event->m_iEventTypeID == FILE_END_EVENT_TYPE) goto _FILEENDEVENT;
    if (event->m_iEventTypeID == FORCE_COMMIT_EVENT_TYPE) goto _FORCECOMMITEVENT;

_NORMALEVENT: //��ͨ���¼�

    for (DistributeRule *p=m_poRuleList; p; p=p->m_poNext) {
        if (m_poBillConditionMgr->check (p->m_iConditionID, event)) {

            p->m_poMQ->send (event, sizeof(StdEvent));

            if (iTemp == MAX_OUT_PROCESS_NUM) THROW (MBC_EventSender);

            m_aiProcessIDBuf[iTemp++] = p->m_iOutProcessID;

            if (!(p->m_poSendRecord)) {
                pSendRecord = new SendRecord (event->m_iFileID);
                if (!pSendRecord) THROW (MBC_EventSender);

                p->m_poSendRecord = new KeyList<SendRecord *> (event->m_iFileID, pSendRecord);
                if (!(p->m_poSendRecord)) THROW (MBC_EventSender);

            } else {
                if (p->m_poSendRecord->get (event->m_iFileID, &pSendRecord)) {
                    pSendRecord->m_iNumber++;
                } else {
                    pSendRecord = new SendRecord (event->m_iFileID);
                    if (!pSendRecord) THROW (MBC_EventSender);

                    p->m_poSendRecord->add (event->m_iFileID, pSendRecord);
                }
            }
        }
    }
    
    TStatLogMgr::addOutEventEX(event->m_iFileID,iFromProcessId,event,m_aiProcessIDBuf,iTemp,1);
    	
  	//�ַ���ֶ���д��־  			 
    if (iTemp == 0)//����
    {
    	if (Process::m_iAppID != PRICING_APP_ID)//�������û�к������� ��˵��ֻд�˹����ڴ�
    	{	
    		TStatLogMgr::addDistributeRemianB(event->m_iFileID,iFromProcessId,event,iTemp,1);
    	}
    }
    else//������۲���ֶ�ֱ���͹����ڴ�
    {
    	TStatLogMgr::addDistributeRemianA(event->m_iFileID,Process::m_iAppID,iFromProcessId,event,m_aiProcessIDBuf,iTemp,1);
    }
    
    return m_aiProcessIDBuf;


_FILEENDEVENT: //�ļ������¼����⴦��
    for (DistributeRule *p=m_poRuleList; p; p=p->m_poNext) {
        if (p->m_poSendRecord && p->m_poSendRecord->get (event->m_iFileID, &pSendRecord)) {

            p->m_poMQ->send (event, sizeof (StdEvent));

            if (iTemp == MAX_OUT_PROCESS_NUM) THROW (MBC_EventSender);
            m_aiProcessIDBuf[iTemp++] = p->m_iOutProcessID;

            delete pSendRecord;
            p->m_poSendRecord->remove (&(p->m_poSendRecord), event->m_iFileID);
        }
    	//���ڷַ������ӵ����û�����ύ���˵����, 
    	//p->m_poSendRecord->get (event->m_iFileID, &pSendRecord)�����ж���Դ���̻ᵼ��������Щ�����޷��ύ
    	//�ȸĳ�����,�����������ύ����,���ǲ���©�ύ        
        TStatLogMgr::insertLogEx(event->m_iFileID, iFromProcessId, &p->m_iOutProcessID, 1);
    }
    
    if (iTemp == 0)//writelocal�Ƚ��ջ���ȴ���ڷ��͵����
    {
		static int iDestProcessID[1] = {0};    	
        TStatLogMgr::insertLogEx(event->m_iFileID, iFromProcessId, iDestProcessID, 1);    	
    }	
    //TStatLogMgr::insertLogEx(event->m_iFileID,iFromProcessId,m_aiProcessIDBuf,iTemp);
    	
    return m_aiProcessIDBuf;

_FORCECOMMITEVENT:
    for (DistributeRule *p=m_poRuleList; p; p=p->m_poNext) {
        p->m_poMQ->send (event, sizeof(StdEvent));

        if (iTemp == MAX_OUT_PROCESS_NUM) THROW (MBC_EventSender);

        m_aiProcessIDBuf[iTemp++] = p->m_iOutProcessID;
    }

	TStatLogMgr::insertLogForce();//ǿ���������wf_stat_log��־ 12.21

    return m_aiProcessIDBuf;
}


int * EventSender::send (AcctItemTransData * itemData,StdEvent * event,bool bNew)
{
	if(!bNew)
	{
		for (DistributeRule *p=m_poRuleList; p; p=p->m_poNext) {
			if (m_poBillConditionMgr->check (p->m_iConditionID, event)){
				p->m_poMQ->send (itemData, sizeof(AcctItemTransData)*MAX_ITEM_RECORD);
			}
		}
		return 0;
	}
    SendRecord * pSendRecord;
    int iTemp (0);
    
    int iFromProcessId = Process::m_iFromProcID;
    event->m_iProcessID = Process::m_iProcID;
    
    memset (m_aiProcessIDBuf, 0, sizeof (m_aiProcessIDBuf));

    if (event->m_iEventTypeID == FILE_END_EVENT_TYPE) goto _FILEENDEVENT;
    if (event->m_iEventTypeID == FORCE_COMMIT_EVENT_TYPE) goto _FORCECOMMITEVENT;

_NORMALEVENT: //��ͨ���¼�

    for (DistributeRule *p=m_poRuleList; p; p=p->m_poNext) {
        if (m_poBillConditionMgr->check (p->m_iConditionID, event)) {
			if(ACCT_ITEM_TRANS == p->m_iMsgEventNum){
				p->m_poMQ->send (itemData, sizeof(AcctItemTransData)*MAX_ITEM_RECORD);
			}else{
				p->m_poMQ->send (event, sizeof(StdEvent));
			}

            if (iTemp == MAX_OUT_PROCESS_NUM) THROW (MBC_EventSender);

            m_aiProcessIDBuf[iTemp++] = p->m_iOutProcessID;

            if (!(p->m_poSendRecord)) {
                pSendRecord = new SendRecord (event->m_iFileID);
                if (!pSendRecord) THROW (MBC_EventSender);

                p->m_poSendRecord = new KeyList<SendRecord *> (event->m_iFileID, pSendRecord);
                if (!(p->m_poSendRecord)) THROW (MBC_EventSender);

            } else {
                if (p->m_poSendRecord->get (event->m_iFileID, &pSendRecord)) {
                    pSendRecord->m_iNumber++;
                } else {
                    pSendRecord = new SendRecord (event->m_iFileID);
                    if (!pSendRecord) THROW (MBC_EventSender);

                    p->m_poSendRecord->add (event->m_iFileID, pSendRecord);
                }
            }
        }
    }
    
    TStatLogMgr::addOutEventEX(event->m_iFileID,iFromProcessId,event,m_aiProcessIDBuf,iTemp,1);
    	
  	//�ַ���ֶ���д��־  			 
    if (iTemp == 0)//����
    {
    	if (Process::m_iAppID != PRICING_APP_ID)//�������û�к������� ��˵��ֻд�˹����ڴ�
    	{	
    		TStatLogMgr::addDistributeRemianB(event->m_iFileID,iFromProcessId,event,iTemp,1);
    	}
    }
    else//������۲���ֶ�ֱ���͹����ڴ�
    {
    	TStatLogMgr::addDistributeRemianA(event->m_iFileID,Process::m_iAppID,iFromProcessId,event,m_aiProcessIDBuf,iTemp,1);
    }

    return m_aiProcessIDBuf;


_FILEENDEVENT: //�ļ������¼����⴦��
    for (DistributeRule *p=m_poRuleList; p; p=p->m_poNext) {
        if (p->m_poSendRecord && p->m_poSendRecord->get (event->m_iFileID, &pSendRecord)) {
			if(ACCT_ITEM_TRANS == p->m_iMsgEventNum){
				memset(&itemData[0],0,sizeof(AcctItemTransData));
				itemData[0].m_iEventTypeID = FILE_END_EVENT_TYPE;
				p->m_poMQ->send (itemData, sizeof(AcctItemTransData)*MAX_ITEM_RECORD);
			}else{
				p->m_poMQ->send (event, sizeof(StdEvent));
}

            if (iTemp == MAX_OUT_PROCESS_NUM) THROW (MBC_EventSender);
            m_aiProcessIDBuf[iTemp++] = p->m_iOutProcessID;

            delete pSendRecord;
            p->m_poSendRecord->remove (&(p->m_poSendRecord), event->m_iFileID);
        }
    	//���ڷַ������ӵ����û�����ύ���˵����, 
    	//p->m_poSendRecord->get (event->m_iFileID, &pSendRecord)�����ж���Դ���̻ᵼ��������Щ�����޷��ύ
    	//�ȸĳ�����,�����������ύ����,���ǲ���©�ύ        
        TStatLogMgr::insertLogEx(event->m_iFileID, iFromProcessId, &p->m_iOutProcessID, 1);
    }
    
    if (iTemp == 0)//writelocal�Ƚ��ջ���ȴ���ڷ��͵����
    {
		static int iDestProcessID[1] = {0};    	
        TStatLogMgr::insertLogEx(event->m_iFileID, iFromProcessId, iDestProcessID, 1);    	
    }	
    //TStatLogMgr::insertLogEx(event->m_iFileID,iFromProcessId,m_aiProcessIDBuf,iTemp);
    	
    return m_aiProcessIDBuf;

_FORCECOMMITEVENT:
    for (DistributeRule *p=m_poRuleList; p; p=p->m_poNext) {
		if(ACCT_ITEM_TRANS == p->m_iMsgEventNum){
			memset(&itemData[0],0,sizeof(AcctItemTransData));
			itemData[0].m_iEventTypeID = FORCE_COMMIT_EVENT_TYPE;
			p->m_poMQ->send (itemData, sizeof(AcctItemTransData)*MAX_ITEM_RECORD);
		}else{
			p->m_poMQ->send (event, sizeof(StdEvent));
		}

        if (iTemp == MAX_OUT_PROCESS_NUM) THROW (MBC_EventSender);

        m_aiProcessIDBuf[iTemp++] = p->m_iOutProcessID;
    }

	TStatLogMgr::insertLogForce();//ǿ���������wf_stat_log��־ 12.21

    return m_aiProcessIDBuf;
}

// add by zhangch
int * EventSender::send (char *buffer)
{
    StdEvent *pEvent = new StdEvent;
    int iTemp (0);
    
    memset (m_aiProcessIDBuf, 0, sizeof (m_aiProcessIDBuf));

	// ��ȡbufferǰ���StdEvent
	memcpy((char*)pEvent, buffer, sizeof(StdEvent));

    for (DistributeRule *p=m_poRuleList; p; p=p->m_poNext) {
        if (m_poBillConditionMgr->check (p->m_iConditionID, pEvent)) {

            p->m_poMQ->send (buffer, sizeof(StdEvent)*p->m_iMsgEventNum);

            if (iTemp == MAX_OUT_PROCESS_NUM) THROW (MBC_EventSender);

            m_aiProcessIDBuf[iTemp++] = p->m_iOutProcessID;
		}
	}

	delete pEvent;

    return m_aiProcessIDBuf;
}
*/
//##ModelId=41F603420134
EventSender::EventSender(int iProcID)
{
	//
	//m_poRuleList = 0;
	//m_poBillConditionMgr = Process::m_poConditionMgr;

	TOCIQuery qry (Environment::getDBConn());
	char sql[2048];
	MessageQueue * pmq;
	bool bTransAcctItem = false;//�ʵ�����

	/*sprintf (sql, "select a.mq_id mq_id, a.max_msg_num max_msg_num, nvl(b.condition_id,-1),"
				"b.out_process_id "
				"from wf_mq_attr a, wf_distribute_rule b, wf_process_mq c "
				"where b.process_id = %d and c.process_id = b.out_process_id "
				"and a.mq_id = c.mq_id", iProcID);
*/
	// modified by zhangch
	sprintf (sql, "select a.mq_id mq_id, a.max_msg_num max_msg_num, nvl(b.condition_id,-1),"
				"b.out_process_id, nvl(a.msg_event_num, 1) "
				"from wf_mq_attr a, wf_distribute_rule b, wf_process_mq c "
				"where b.process_id = %d and c.process_id = b.out_process_id "
				"and a.mq_id = c.mq_id", iProcID);
	qry.setSQL (sql);
	qry.open ();

	while (qry.next ()) {
		bTransAcctItem = false;
		pmq = new MessageQueue (qry.field(0).asInteger ());
		if (!pmq) THROW(MBC_EventSender+1);

		//pmq->open (true, false, sizeof(StdEvent), qry.field(1).asInteger ());

		//m_poRuleList = new DistributeRule (pmq, qry.field(2).asInteger (), m_poRuleList, qry.field(3).asInteger ());
		//if (!m_poRuleList) THROW(MBC_EventSender+1);
		// add by zhangch
		//m_poRuleList->m_iMsgEventNum = qry.field(4).asInteger();
		//if (ACCT_ITEM_TRANS == m_poRuleList->m_iMsgEventNum){
		//	bTransAcctItem = true;
		//}else{
		//	m_poRuleList->m_iMsgEventNum = m_poRuleList->m_iMsgEventNum>20 ? 20 : m_poRuleList->m_iMsgEventNum;
		//}

		//if (bTransAcctItem){
		//	pmq->open (true, false, sizeof(AcctItemTransData)*MAX_ITEM_RECORD, qry.field(1).asInteger ());
		//}else{
		//pmq->open (true, false, sizeof(StdEvent)*m_poRuleList->m_iMsgEventNum, qry.field(1).asInteger ());
		//}

	}

	qry.close ();


}


//##ModelId=41F603420166
EventSender::~EventSender()
{
	/*
	DistributeRule *p ;
	while ((p = m_poRuleList)!= 0) {
		m_poRuleList = p->m_poNext;

		p->m_poMQ->close ();
		delete (p->m_poMQ);
		p->m_poMQ = 0;

		delete p;
	}
	*/
	//TStatLogMgr::insertLogForce();//��ֹ���̱�ɱ�ڴ��д���δ�ύ����־ ��ɵĲ�ƽ 12,9
}



//##ModelId=428D5B2203B6
/*
int *EventSender::getOutProcessList(int iFileID)
{
	int iTemp;
	SendRecord * pSendRecord;

	iTemp = 0;
	memset (m_aiProcessIDBuf, 0, sizeof (m_aiProcessIDBuf));

	for (DistributeRule *p=m_poRuleList; p; p=p->m_poNext) {
		if (p->m_poSendRecord && p->m_poSendRecord->get (iFileID, &pSendRecord)) {
			if (iTemp == MAX_OUT_PROCESS_NUM) THROW (MBC_EventSender);
			m_aiProcessIDBuf[iTemp++] = p->m_iOutProcessID;
		}
	}

	return m_aiProcessIDBuf;
}
*/
