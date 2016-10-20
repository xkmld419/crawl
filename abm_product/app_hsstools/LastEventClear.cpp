/* add by  zhuangxf */
/* date:20110428 */

#include "LastEventClear.h"
#include "Exception.h"
#include "mntapi.h"
#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <stdarg.h>
static TProcessInfo *pProcInfoHead = NULL;
static THeadInfo *pInfoHead = NULL;

ClearLastEvent::ClearLastEvent()
{
	if(p_shmTable)
		p_shmTable = NULL;
}

//-------------------------------------------------------------------------------
ClearLastEvent::~ClearLastEvent()
{
	if(p_shmTable)
		p_shmTable = NULL;
}

//-------------------------------------------------------------------------------
TSHMCMA * ClearLastEvent::GetShmTable()
{
	long lShmKey = IpcKeyMgr::getIpcKey (-1,"SHM_WorkFlowKey");
	if (lShmKey < 0)
	{
		p_shmTable=NULL;
		return NULL;
	}
	if (GetShmAddress(&p_shmTable,lShmKey)<0 )
	{
		p_shmTable = NULL;
		return NULL;
	}

	return p_shmTable;
}

//-------------------------------------------------------------------------------
int ClearLastEvent::GetShmAddress(TSHMCMA **ppTable,long iShmKey)
{
	int iShmID;

	//��ȡ�����ڴ�ID
	if ((iShmID = shmget(iShmKey,0,0))<0){
		return -1;
	}

	//get the buff addr.
	*ppTable = (TSHMCMA *)shmat(iShmID,(void *)0,0);

	if (-1 == (long)(*ppTable)){
		return -2;
	}

	return 0;
}
//-------------------------------------------------------------------------------
bool ClearLastEvent::SetEventStateProcessID(int iProcessID,int iState )
{
	char cResultValue[512]={0};
	this->GetShmTable();//���ӹ����ڴ�
	if(!p_shmTable)
	{
		this->log("���ӹ����ڴ�ʧ��!");		
		THROW(MBC_SimpleSHM+6);
	}
	int iLastEventState=0;  //���һ��������״̬
	TAppSaveBuff *MyAppbuff=NULL;
	if (!MyAppbuff && p_shmTable) 
		MyAppbuff = p_shmTable->getAppSaveBuffByProcID(iProcessID);
	if (!MyAppbuff)
	{
		sprintf(cResultValue,"���ڴ���PROCESS_ID=%d�޻�����Ϣ!",iProcessID);
		this->log(cResultValue);
		return false;
	}
	if(MyAppbuff->getEventState(&iLastEventState))
	{
		MyAppbuff->setEventState(iState); //�������һ��������״̬Ϊ1:�Ѿ�����״̬	
		sprintf(cResultValue,"PROCESS_ID=%d::LastEventstate:%d---->%d",iProcessID,iLastEventState,iState);
		this->log(cResultValue);
		return true;
	}
	else
	{
		sprintf(cResultValue,"����PROCESS_ID=%d�����һ��������Ϣ",iProcessID);
		this->log(cResultValue);	
		return false;
	}
	return true;
}

//-------------------------------------------------------------------------------
void ClearLastEvent::log(char *format,...)
{
	char *p = NULL, sTemp[10000]={0}, sLine[10000]={0};
	int fd = 0;
	int iLeng=0;
	char sLogFileName[10240] = {0};

	if ((p=getenv("BILLDIR")) == NULL)
		return ;

	iLeng=strlen(p);

	if (iLeng != 0)
	{
		if(p[iLeng] == '/')
			sprintf(sLogFileName,"%slog/%s",p,"ClearEvent.log");
		else
			sprintf(sLogFileName,"%s/log/%s",p,"ClearEvent.log");
	}
	else
		return ;

	if ((fd=open (sLogFileName, O_RDWR|O_CREAT|O_APPEND, 0770)) < 0) 
	{
		printf ("[ERROR]: ClearEvent::log() openFile %s error\n", sLogFileName);
		return;
	}

	Date dt;
	sprintf (sTemp, "[%s]: %s\n", dt.toString("yyyy-mm-dd hh:mi:ss"), format);

	va_list ap;
	va_start(ap, format);
	vsnprintf (sLine, 9999, sTemp, ap);
	va_end(ap);
	printf ("%s", sLine);
	write (fd, sLine, strlen (sLine));
	close (fd);
}

//-------------------------------------------------------------------------------
void showExpHelp()
{
	printf("\n ****************************************************************   \n") ;
	printf("  1:�����������һ����������״̬Ϊ�Ѿ����� \n") ;
	printf("     ex1:cleanlastevent -a \n");
	printf("  2:�����������һ����������״̬Ϊ�Ѿ�����  \n");
	printf("     ex2:cleanlastevent -b BILLFLOW_ID \n");
	printf("  3:���ý������һ����������״̬Ϊ�Ѿ�����  \n");
	printf("     ex3:cleanlastevent -p PROCESS_ID \n");
	printf("  4:���ý������һ����������״̬Ϊδ����  \n");
	printf("     ex4:cleanlastevent -s PROCESS_ID \n");	
	printf(" *****************************************************************   \n") ;
}

//-------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	int iBillflowID=-1;	//����ID
	int iProcessID=-1;	//����ID
	int iCount = 0; //���޸ĵĽ�����
	char cTmpResult[512]={0};
	TAppSaveBuff * cMyAppBuff=NULL;
	ClearLastEvent *tClearEvent = new ClearLastEvent();	//������
	if(argc > 1)
	{
		if(argc == 2 && strcmp(argv[1],"-help")==0)
		{
			showExpHelp();
			return 0;
		}
		try 
		{	//coding here
			if(pInfoHead)
				pInfoHead = NULL;			
			if (AttachShm (1000*sizeof (THeadInfo), (void **)&pInfoHead) >= 0)
			{
				pProcInfoHead = &pInfoHead->ProcInfo;
			}    
			else
			{
				tClearEvent->log ( "���ӹ����ڴ�AttachShm ʧ��"); 
				return -1;
			} 		
			if(strcmp(argv[1],"-b")==0 || strcmp(argv[1],"-B")==0)
			{
				//��һ�����̵����һ���������д���
				iBillflowID = atoi(argv[2]);
				for(int i=0; i<pInfoHead->iProcNum;i++)
				{
					if((pProcInfoHead+i)->iBillFlowID == iBillflowID) 
					{
						if((pProcInfoHead+i)->iState != ST_RUNNING && 
							(pProcInfoHead+i)->iState != ST_INIT &&
							(pProcInfoHead+i)->iState != ST_WAIT_BOOT)
	       	   			{
							if(!tClearEvent->SetEventStateProcessID((pProcInfoHead+i)->iProcessID,1))
								continue;
							iCount++;
	       	   			}
	       	   			else
	       	   			{
							sprintf(cTmpResult,"PROCESSS_ID:%d���̵�״̬��������ֹͣ״̬��������",(pProcInfoHead+i)->iProcessID);
							tClearEvent->log(cTmpResult);
							continue;
	       	   			}
	       	   		}
				}
				if(iCount>0)
				{
					sprintf(cTmpResult,"������%d�����̵����һ������״̬",iCount);	
					tClearEvent->log(cTmpResult);
				}
				else
					tClearEvent->log("δ�ҵ���Ӧ�Ľ��̽��и���");
			}
			else if(strcmp(argv[1],"-p")==0 || strcmp(argv[1],"-P")==0)
			{
				//��һ�����̵����һ���������д���
				iProcessID = atoi(argv[2]);
				
				for(int i=0; i<pInfoHead->iProcNum;i++)
				{
					if((pProcInfoHead+i)->iProcessID == iProcessID) 
					{
						iCount++;
						if((pProcInfoHead+i)->iState != ST_RUNNING && 
							(pProcInfoHead+i)->iState != ST_INIT &&
							(pProcInfoHead+i)->iState != ST_WAIT_BOOT)
	       	   			{
							if(!tClearEvent->SetEventStateProcessID((pProcInfoHead+i)->iProcessID,1))
							{
								break;								
							}
							else
							{
								sprintf(cTmpResult,"PROCESS_ID=%d���³ɹ�",iProcessID);
								tClearEvent->log(cTmpResult);
								break;
							}
	       	   			}
	       	   			else
	       	   			{
							sprintf(cTmpResult,"PROCESSS_ID:%d���̵�״̬��������ֹͣ״̬��������",(pProcInfoHead+i)->iProcessID);
							tClearEvent->log(cTmpResult);
							break;
	       	   			}
	       	   		}
				}
				if(iCount==0)
				{
					sprintf(cTmpResult,"δ�ҵ�PROCESSS_ID:%d����",iProcessID);
					tClearEvent->log(cTmpResult);
				}					
			}
			else if(strcmp(argv[1],"-s")==0 || strcmp(argv[1],"-S")==0)
			{
				//��һ�����̵����һ���������д���
				iProcessID = atoi(argv[2]);
				
				for(int i=0; i<pInfoHead->iProcNum;i++)
				{
					if((pProcInfoHead+i)->iProcessID == iProcessID) 
					{
						iCount++;
						if((pProcInfoHead+i)->iState != ST_RUNNING && 
							(pProcInfoHead+i)->iState != ST_INIT &&
							(pProcInfoHead+i)->iState != ST_WAIT_BOOT)
	       	   			{
							if(!tClearEvent->SetEventStateProcessID((pProcInfoHead+i)->iProcessID,0))
							{
								break;								
							}
							else
							{
								sprintf(cTmpResult,"PROCESS_ID=%d���³ɹ�",iProcessID);
								tClearEvent->log(cTmpResult);
								break;
							}
	       	   			}
	       	   			else
	       	   			{
							sprintf(cTmpResult,"PROCESSS_ID:%d���̵�״̬��������ֹͣ״̬��������",(pProcInfoHead+i)->iProcessID);
							tClearEvent->log(cTmpResult);
							break;
	       	   			}
	       	   		}
				}
				if(iCount==0)
				{
					sprintf(cTmpResult,"δ�ҵ�PROCESSS_ID:%d����",iProcessID);
					tClearEvent->log(cTmpResult);
				}					
			}			
			else if(strcmp(argv[1],"-a")==0  || strcmp(argv[1],"-A")==0)
			{
				//���������̵����һ���������д���
				char ch;
				cout<<"ȷʵҪ������?�����ֻ����-s���лָ�!(y/n)"<<endl;
				cin>>ch;
				if(!(ch=='1' ||ch =='y' || ch=='Y'))  //��ȷ�Ͻ����˳�
					return 0;
				for(int i=0; i<pInfoHead->iProcNum;i++)
				{
					if((pProcInfoHead+i)->iProcessID >0)
					{
						if((pProcInfoHead+i)->iState != ST_RUNNING && 
							(pProcInfoHead+i)->iState != ST_INIT &&
							(pProcInfoHead+i)->iState != ST_WAIT_BOOT)
	       	   			{
							if(!tClearEvent->SetEventStateProcessID((pProcInfoHead+i)->iProcessID,1))
								continue;
	       	   			}
	       	   			else
	       	   			{
							sprintf(cTmpResult,"PROCESSS_ID:%d���̵�״̬��������ֹͣ״̬��������",(pProcInfoHead+i)->iProcessID);
							tClearEvent->log(cTmpResult);
							continue;
	       	   			}
	       	   		}
				}
			}
			else
			{
				showExpHelp();
				return 0;
			}	
			if(tClearEvent)  //�ͷ��ڴ�
			{
				delete tClearEvent;
				tClearEvent = NULL;
			}
			if(pProcInfoHead || pInfoHead)
			{
				pInfoHead = NULL;
				pProcInfoHead=NULL;
			}			
		}
		catch (Exception &e) 
		{
			sprintf(cTmpResult,"�յ�Exception�˳�\n[EXCEPTION]: %s",e.descript());
			tClearEvent->log(cTmpResult);	
			if(tClearEvent)
			{
				delete tClearEvent;
				tClearEvent = NULL;
			}
			if(pProcInfoHead || pInfoHead)
			{
				pInfoHead = NULL;
				pProcInfoHead=NULL;
			}			
		}
		catch (...) 
		{
			cout << "[EXCEPTION]: Exception catched" <<endl;		
			if(tClearEvent)
			{
				delete tClearEvent;
				tClearEvent = NULL;
			}
			if(pProcInfoHead || pInfoHead)
			{
				pInfoHead = NULL;
				pProcInfoHead=NULL;
			}
		}
	}
	else
	{
		showExpHelp();
		return 0;	
	}
}
//-------------------------------------------------------------------------------

