#include "HbShutDown.h"


/* 1��ע������          
    2,����ֹͣ��Ϣ    ����ֹͣ��Ϣ
	*/

DEFINE_MAIN_EX(CHbShutDown)

CHbShutDown::CHbShutDown(void)
{
}

CHbShutDown::~CHbShutDown(void)
{
}

int CHbShutDown::run()
{
	if(!InitClient())
	{
		cout<<"����û��,�˳���"<<endl;
		return 0;
	}

	CmdMsg* pMsg = CreateNewMsg(MSG_GAME_OVER,"");
	if(send(pMsg))
	{
		int j=0;
		char* c[3]={"/ ","--","\\ "};
		CmdMsg* pRecvMsg=0;
		while(1)
		{	
			j++;
			int i=j%3;		
			cout<<"�ȴ������˳�.........................................."<<c[i];
			usleep(100);
			cout<<'\r';			
			if((pRecvMsg = GetMsg())!=0)
			{
				//��鷵����Ϣ
				variant vt;
				//if(pRecvMsg->GetMsgContent(vt))
				{
					long lCmd = pRecvMsg->GetMsgCmd();
					pRecvMsg->GetMsgContent(vt);
					if(lCmd == MSG_PROCESS_ONEXIT)
					{
						cout<<vt.asStringEx()<<endl;
					}else
					{
						if(lCmd == MSG_PROCESS_EXIT)
						{
						//	cout<<vt.asStringEx()<<endl;
							exit(0);
						}
					}
					/*if(vt.isLong()||vt.isInteger())
					{
						if(vt == INIT_OP_OK)
							cout<<"�����˳�..........................................Ok";
						else
						{
							cout<<"�����˳�ʧ��.................................:"<<vt.asLong()<<endl;
						}
						break;
					}*/
				}
			}			
		}
	}
	delete pMsg;

	return 0;
}

