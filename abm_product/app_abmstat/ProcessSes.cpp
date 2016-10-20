#include "ProcessSes.h"
#include "ThreeLogGroup.h"

ProcessSes::ProcessSes()
{
	
}

ProcessSes::~ProcessSes()
{
	
}


void ProcessSes::Rebuild()
{
	//�˱����������϶��»��Ʋ˵�
	int lv_iTotalRow = 0;
	int lv_iCurrentRow = 0;
	static char Buffer[COLS_LENS];

	LoadEmptyMenu();
	DisplayTime();

	int iTaskNum = 0;
	int i = 0;
	
	//char sProcessSes[10240]={0};

	char sBuff[1000][128];

	memset(Buffer,0,COLS_LENS);
	sprintf(Buffer,"��ǰ�û��µĽ�����Ϣ    ");
	mvaddstr(4,0,Buffer);
	char sCommand[256] = {0};
	char sParam[16] = {0};
	FILE *fp = 0;
	int ret = 0;
	
	char * p;
	p=getenv ("LOGNAME");

	sprintf (sCommand, "%s%s","ps -ef|grep ",p);

	fp = popen (sCommand, "r");
	if (fp == NULL)
	{
		mvaddstr(8,(COLS_LENS - strlen((char *)"����ϵͳ����ʧ��"))/2,(char *)"����ϵͳ����ʧ��");
		return ;
	}
	//---------------------------------------------------------

  //sprintf(sBuff[0],"%s","��¼�û�����¼��ʽ��IP�����ڣ���ʼʱ�䣺����ʱ�䣺����ʱ��"); 
	int n = 0;
	
	while(!feof(fp))
	{
		if( !fgets(sBuff[n],128,fp) )
			break;
		n++;
		lv_iTotalRow ++;
	}
	fclose(fp);

	

	if(lv_iTotalRow==0){
		mvaddstr(8,(COLS_LENS - strlen((char *)"�޷���������ѡ��"))/2,(char *)"�޷���������ѡ��");
		return;
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

	for(; i<n ; i++)
	{
		if ((lv_iCurrentRow > lv_iTotalRow)||(lv_iCurrentRow >= m_iStartRow + LINE_SIZE))
		{
			break;
		}

		if (lv_iCurrentRow == m_iSelectedRow)
		{
			standout();
		}

		//������  			
		memset(Buffer,0,COLS_LENS);
		sprintf(Buffer,"%-10d",lv_iCurrentRow);
		mvaddstr(lv_iCurrentRow - m_iStartRow + 6,0,Buffer);

		memset(Buffer,0,COLS_LENS);
		sprintf(Buffer,"%s",sBuff[i]);
		mvaddstr(lv_iCurrentRow - m_iStartRow + 6,10,Buffer);

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

