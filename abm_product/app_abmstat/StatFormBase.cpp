#include "StatFormBase.h"
#include "mntapi.h"

StatFormBase* StatFormBase::pThis;
int StatFormBase::m_iInterval = 0;
char StatFormBase::m_sMoudle[32] = {0};
int StatFormBase::m_iShowFlag = 0;

TProcessInfo *StatFormBase::pProcInfoHead = NULL;
THeadInfo *StatFormBase::pInfoHead = NULL;

StatFormBase::StatFormBase()
{
	m_bCursesOn = false;
	m_iSelectedRow = 1;
	m_iStartRow = 1;
	m_iInterval = 1;
	pThis = this;
}
StatFormBase::~StatFormBase()
{
}

bool StatFormBase::ConnectGuardMain()
{
	DetachShm ();
	if (AttachShm (0, (void **)&pInfoHead) >= 0) 
	{
		pProcInfoHead = &pInfoHead->ProcInfo;
	} 
	else 
	{
		return false;
	}
	return true;
}

void StatFormBase::StartWin()
{
	if(m_bCursesOn)
		return;
	//������ֹ�ź�
	signal(SIGINT,SIG_IGN);
	initscr();
	noecho();
	intrflush(stdscr,true); 
	keypad(stdscr,TRUE);
	savetty();
	
	m_bCursesOn = true;
}
void StatFormBase::EndWin()
{
	if(!m_bCursesOn)
		return;
	m_bCursesOn = false;
	//�˳���������ģʽ
	resetty();
	endwin();
}
void StatFormBase::DisplayTime()
{
	char Buffer[15];
	//-----------------------------------���ʱ��-----------------------------------------
	time_t Now;
	struct tm  *Time;
	time(&Now);
	Time = localtime(&Now);	
	
	sprintf(Buffer,"%04d/%02d/%02d %02d:%02d:%02d",Time->tm_year+1900,Time->tm_mon+1,Time->tm_mday,Time->tm_hour,Time->tm_min,Time->tm_sec);
    mvaddstr(2,110,Buffer);
}
void StatFormBase::LoadGeneralInfo()
{
	//----------------------------���ز˵���ͨ����Ϣ--------------------------------
	//�����ܱ���
	attron(A_BOLD); 
	char stitle[64] = {0};
	sprintf(stitle,"%s״̬���",SYSNAME);
	mvaddstr(1,(COLS_LENS - strlen((char *)stitle))/2,(char *)stitle);   	   	
	attroff(A_BOLD);
	
	char sInterval[50] = {0};
	sprintf(sInterval,"ˢ�¼��: %d",m_iInterval);
	mvaddstr(2,0,sInterval);
	
	/*����ʱ��*/
	DisplayTime();
	
	/*���ز˵�����*/
	mvhline(3,0,'=',COLS_LENS);
	mvhline(5,0,'-',COLS_LENS);
	mvhline(5 + LINE_SIZE + 1,0,'=',COLS_LENS);
	
}
void StatFormBase::LoadEmptyMenu()
{
	mvhline(4,0,' ',COLS_LENS);//�����
	for(int i=6;i<LINE_SIZE+6;i++)
	{
		mvhline(i,0,' ',COLS_LENS);	
	}
	//mvhline(5 + LINE_SIZE + 3,0,' ',COLS_LENS);
	mvaddstr(5 + LINE_SIZE + 3,18,(char *)"Up Arrow = ����   Down Arrow = ����   Left Arrow = ǰҳ   Right Arrow = ��ҳ   ESC = �˳�"); 
	return;	
}

void StatFormBase::TimeBreak(int signo)
{
	//cout << "here" << endl;
	signal(SIGALRM,TimeBreak);
	alarm(m_iInterval);
	pThis->Rebuild();
	return;
}
void StatFormBase::Rebuild()
{
	
}
void StatFormBase::Build()
{
	StartWin();
	LoadGeneralInfo();
	while(1)
	{
		Rebuild();
		signal(SIGALRM, TimeBreak);
		alarm(m_iInterval);
		int Key = getch();
		alarm(0);
		if (Key == 27)
		{
		  return;
		}
		switch (Key)
		{
			//����س�
			case '\n':
			case '\r':
			  break;
			case KEY_UP:
				m_iSelectedRow--;
			  break;
			case KEY_DOWN:
				m_iSelectedRow++;
			  break;
			case KEY_LEFT:
				m_iSelectedRow -= LINE_SIZE;
				m_iStartRow -= LINE_SIZE;
			  break;
			case KEY_RIGHT:
				m_iSelectedRow += LINE_SIZE;
				m_iStartRow += LINE_SIZE;
				break;
			default	:
			  break;
		}
	}

}

void StatFormBase::SetInterval(int iInterval)
{
	if(iInterval > 0)
		m_iInterval = iInterval;
}

void StatFormBase::SetMoudle(char *sMoudle)
{
	if( strlen(sMoudle) )
		strcpy(m_sMoudle,sMoudle);
}

void StatFormBase::SetShowFlag(int iShowFlag)
{
	if(iShowFlag > 0)
		m_iShowFlag = iShowFlag;
}

