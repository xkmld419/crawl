#include "LoggInfo.h"
#include "ThreeLogGroup.h"

LoggInfo::LoggInfo()
{
//	ConnectGuardMain();
}

LoggInfo::~LoggInfo()
{
}


void LoggInfo::Rebuild()
{
	//�˱����������϶��»��Ʋ˵�
	int lv_iTotalRow = 0;
	int lv_iCurrentRow = 0;
	static char Buffer[COLS_LENS];

	LoadEmptyMenu();
	DisplayTime();

	int iTaskNum = 0;
	int i = 0;
	
	char sLoggInfo[4096]={0};

	ThreeLogGroup* pLogg = NULL;
	try
	{
		pLogg = new ThreeLogGroup();
	}
	catch (...)
	{
		pLogg = NULL;
	}
	
	memset(Buffer,0,COLS_LENS);
	sprintf(Buffer,"��־����Ϣ");
	mvaddstr(4,0,Buffer);

	if( pLogg == 0 )
	{
		mvaddstr(8,(COLS_LENS - strlen((char *)"������־���ڴ�ʧ��"))/2,(char *)"������־���ڴ�ʧ��");
		return;
	}

	if( !pLogg->DisplayLogGroup(-1,sLoggInfo))
	{
		mvaddstr(8,(COLS_LENS - strlen((char *)"��ȡ��־����Ϣʧ��"))/2,(char *)"��ȡ��־����Ϣʧ��");
		return;
	}

	if(strlen(sLoggInfo)==0){
		mvaddstr(8,(COLS_LENS - strlen((char *)"�޷���������ѡ��"))/2,(char *)"�޷���������ѡ��");
		return;
	}

	char *p = sLoggInfo;

	while(*p)
	{
		if(*p == '\n')
			lv_iTotalRow ++;
		p++;
	}


	lv_iCurrentRow =1;

	//�����ĸ��ж���Ҫ���ھ���һЩ�Ƿ���
	//���ȫ��ѡ������<=0�����ָ�� ��
	if (m_iSelectedRow  <= 0)
	{
		m_iSelectedRow  = lv_iTotalRow ;
	}		
	//���ȫ��ѡ������> lv_iTotalRow �����ָ����һ��
	if (m_iSelectedRow  > lv_iTotalRow)
	{
		m_iSelectedRow  = 1;
	}		
	//���g_iStartRow <= 0����Ϊ��Ϊ��һ��	
	if (m_iStartRow <= 0)
	{
		m_iStartRow = 1;
	}				
	if (m_iStartRow > m_iSelectedRow )
	{
		m_iStartRow = m_iSelectedRow ;
	}				
	if (m_iSelectedRow  - m_iStartRow >= LINE_SIZE)
	{
		m_iStartRow = m_iSelectedRow  - LINE_SIZE + 1;
	}				

	// --ȷ��lv_iCurrentRow��ʼֵ
	while(1)
	{
		if (lv_iCurrentRow >= m_iStartRow )
		{
			break;
		}
		else
		{
			lv_iCurrentRow++;
		}				
	}
	i = lv_iCurrentRow -1;

	char *sTok = "\n";
	p = strtok(sLoggInfo,sTok);
	while(p)
	{
		if ((lv_iCurrentRow > lv_iTotalRow)||(lv_iCurrentRow >= m_iStartRow + LINE_SIZE))
		{
			break;
		}
		if (lv_iCurrentRow == m_iSelectedRow)
		{
			standout();
		}

		memset(Buffer,0,COLS_LENS);
		sprintf(Buffer,"%s",p);
		mvaddstr(lv_iCurrentRow - m_iStartRow + 6,0,Buffer);
		p = strtok(NULL,sTok);

		if (lv_iCurrentRow == m_iSelectedRow)
		{
			standend();
		}
		lv_iCurrentRow ++;
	}


	//δ����������հ�		
	while (lv_iCurrentRow < m_iStartRow + LINE_SIZE)
	{
		mvhline(lv_iCurrentRow - m_iStartRow + 6,0,' ',COLS_LENS);
		lv_iCurrentRow ++;
	}

	wmove(stdscr,m_iSelectedRow - m_iStartRow + 6,0);//��ָ���Ƶ�ѡ������
	refresh();
}

