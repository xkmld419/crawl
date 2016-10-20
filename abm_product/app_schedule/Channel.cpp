#include "StdAfx.h"

CommandCom* Channel::m_pCmdCom=0;
THeadInfo*	Channel::m_pInfoHead = 0;
TProcessInfo*	Channel::m_pProcInfoHead = 0;
ThreeLogGroup* Channel::m_pLog = 0;
char Channel::m_sSysRunState[MAX_STATE_LEN];
	
Channel::Channel()
{
}
Channel::~Channel()
{
}

//���ӹ����ڴ�
int Channel::connectGuardMain()
{
	DetachShm ();
	if(AttachShm (0, (void **)&m_pInfoHead) >= 0)		//����GuardMain
	{
		m_pProcInfoHead = &m_pInfoHead->ProcInfo;			//�õ�������Ϣ�׵�ַ
	}
	else 
	{
		//Log::log(0,"����ʱ�澯�����ӹ����ڴ�ʧ�ܣ�GuardMain���ܲ�����\n");
		//ALARMLOG(0,MBC_schedule+24,"%s","���ӹ����ڴ�ʧ�ܣ�GuardMain���ܲ�����");
		return -1;
	}
	return 0;
}

//����ͨ����״̬
int Channel::calcWater(int iVpID)
{
	//�ҵ�ͨ�����������������������������VPID
	if(iVpID < 0)
		return 0;
	int iMessageNum = 0;
	int iPercent = 0;
	for (int i=0; i<m_pInfoHead->iProcNum; i++)			//�������н���
	{
		if(iVpID == (m_pProcInfoHead+i)->iVpID)		//����
		{
			if((m_pProcInfoHead+i)->iMqID > 0)
			{
				 MessageQueue mq( (m_pProcInfoHead+i)->iMqID);
         if(mq.exist())
         {
				  iMessageNum = mq.getMessageNumber();
				 }
				else
				  iMessageNum = 0;
			}
			else
			{
				iMessageNum = 0;
			}
			break;
		}
	}
	
	int iLowLevel = 0;
	int iHighLevel = 0;
	int iQueueSize = 0;
	//�Ӻ��Ĳ����ж�ȡ��ͨ��������
	int iState;
	char sTemp[100] = {0};
	memset(sTemp,0,100);
	
	int iTemp = iVpID/1000;
	
	sprintf(sTemp,"vp%d.process_high_level",iTemp);
	iHighLevel = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", sTemp, 100);//��ˮֵ
	
	memset(sTemp,0,100);
	sprintf(sTemp,"vp%d.process_low_level",iTemp);
	iLowLevel = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", sTemp, 0); //��ˮֵ
		
		
	memset(sTemp,0,100);
	sprintf(sTemp,"vp%d.queue_size",iTemp);
	iQueueSize = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", sTemp, 0); //���г���
	//int iRet = CalcState( iVpID, iTaskNum);
	if(iQueueSize > 0)
		iPercent = iMessageNum*100/iQueueSize;
	
	if(iPercent >= iHighLevel)
		iState = CHANNEL_HIGH;
		
	else if(iPercent <= iLowLevel)
		iState = CHANNEL_LOW;
	else
		iState = CHANNEL_NOMAL;
		
	if(iPercent == 0)
		iState = CHANNEL_EMPTY;
	
	if(iPercent >= 100)
	{
		iPercent = 100;
		iState = CHANNEL_FULL;
	}
	//����ӵ�������ָͣ��ģ��
	int iBlockLevel = 0;
	int iScheduleBlockVP = 0;
	iBlockLevel = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", "schedule_block_level", 0); //ӵ������
	iScheduleBlockVP = Channel::m_pCmdCom->readIniInteger ("SCHEDULE", "schedule_block_vp", 0); //ӵ�����vp
	if( iScheduleBlockVP == iVpID / 1000 )
	{
		if (iBlockLevel > 0)
		{
			iState = CHANNEL_HIGH;
		}
	}
	return iState;
}

//Trim����
char* Channel::trim(char* sp)
{
	char sDest[1024];
	char *pStr;
	int i = 0;

	if ( sp == NULL )
		return NULL;

	pStr = sp;
	while ( *pStr == ' ' || *pStr == '\t' )  pStr ++;
	strcpy( sDest, pStr );

	i = strlen( sDest ) - 1;
	while((i>=0)&&(sDest[i] == ' ' || sDest[i] == '\t' || sDest[i] == '\r' ) )
	{
		sDest[i] = '\0';
		i -- ;
	}

	strcpy( sp, sDest );

	return ( sp );

}
