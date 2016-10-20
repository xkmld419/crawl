/* Copyright (c) 2010,�����Ƽ��ɷ����޹�˾������ҵ��
 * All rights reserved.
 * �汾��1.0
 * ���ߣ�wangs
 * ���ڣ�2010��03��
*/

#include <stdlib.h>
#include <iostream>
#include "TOCIQuery.h"
#include "MessageQueue.h"
#include "StdEvent.h"
#include "IpcKey.h"
#include "Exception.h"
#include "Environment.h"

using namespace std;

int const QUEUE_CREATE = 1;
int const QUEUE_DELETE = 2;

void QueueCtl(int iCtlType,char * sBillFlowID)
{
	char sSql[3072];
	char sTemp[256];
	
	memset(sSql,0,sizeof(sSql));

	try
	{
		DEFINE_QUERY(qry);
		strcpy(sSql,"select a.billflow_id,b.exec_name,a.process_id,a.caption,c.mq_id,d.max_msg_num "
			" from wf_process a, wf_application b, wf_process_mq c, wf_mq_attr d "
			" where a.app_id = b.app_id and b.app_type=0 and a.process_id=c.process_id "
			" and c.mq_id=d.mq_id ");
		if (NULL != sBillFlowID){
			memset(sTemp,0,sizeof(sTemp));
			sprintf(sTemp," and a.billflow_id in (%s)",sBillFlowID);
			strcat(sSql,sTemp);
		}
		memset(sTemp,0,sizeof(sTemp));
		char temp[32];
		sprintf(sTemp," and a.sys_username='%s' and a.host_id = %d",
			getenv("LOGNAME"),IpcKeyMgr::getHostInfo(temp));
		strcat(sSql,sTemp);
		strcat(sSql," order by a.billflow_id, a.process_id");
		if (QUEUE_CREATE == iCtlType)
		{
			qry.setSQL(sSql);
			qry.open();
			while(qry.next())
			{
				MessageQueue mq(qry.field(4).asInteger());
				cout<<"����������Ϣ��"<<"����ID["<<qry.field(0).asInteger()<<"] "
									  <<"Ӧ������["<<qry.field(1).asString()<<"] "
									  <<"����ID["<<qry.field(2).asInteger()<<"] "
									  <<"��������["<<qry.field(3).asString()<<"] "
				<<endl;
				mq.open (true, true, sizeof(StdEvent), qry.field(5).asInteger());
			}		
		}
		if (QUEUE_DELETE == iCtlType)
		{
			qry.setSQL(sSql);
			qry.open();
			while(qry.next())
			{
				MessageQueue mq(qry.field(4).asInteger());
				cout<<"ɾ��������Ϣ��"<<"����ID["<<qry.field(0).asInteger()<<"] "
									  <<"Ӧ������["<<qry.field(1).asString()<<"] "
									  <<"����ID["<<qry.field(2).asInteger()<<"] "
									  <<"��������["<<qry.field(3).asString()<<"] "
				<<endl;
				mq.remove();
			}
		}
		qry.commit();
		qry.close();
	}catch(TOCIException &oe){
		cout<<"SQL:"<<sSql<<endl;
		cout<<"���ݿ����"<<oe.getErrMsg()<<endl;
	}
	catch (...) {
		cout<<"�յ�����Exception�˳�"<<endl;
	}

	return ;
}

void printUsage(void)
{
	cout<<"\n*===============��Ϣ���п���===============\n";
	cout<<"*���д���:\n";
	cout<<"+\t ctlqueue -c [billflow_id,...]\n";
	cout<<"*����ɾ��:\n";
	cout<<"+\t ctlqueue -d [billflow_id,...]\n";
	cout<<"*==========================================\n";

	return;
}

int main(int argc, char *argv[])
{
	int iCtlType = 0;
	
	//������Ϣ
	if(argc == 1){
		printUsage();
		return 0;
	}
	
	//����ѡ��
	switch(argv[1][1]| 0x20)
	{
	case 'c':
		iCtlType = QUEUE_CREATE;
		break;
	case 'd':
		iCtlType = QUEUE_DELETE;
		break;
	default:
		printUsage();
		return 0;
	}
	
	//����ѡ��
	if (argc == 3){
		QueueCtl(iCtlType,argv[2]);
	}else{
		QueueCtl(iCtlType,NULL);
	}
    
	return 0;
}

