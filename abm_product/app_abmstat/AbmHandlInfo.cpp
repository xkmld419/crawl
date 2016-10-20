#include "AbmHandlInfo.h"
#include "ThreeLogGroup.h"
#include "OracleConn.h"

AbmHandlInfo::AbmHandlInfo()
{
//	ConnectGuardMain();
}

AbmHandlInfo::~AbmHandlInfo()
{
}


void AbmHandlInfo::Rebuild()
{
	//�˱����������϶��»��Ʋ˵�
	int n = 0;
	int lv_iTotalRow = 0;
	int lv_iCurrentRow = 0;
	static char Buffer[COLS_LENS];

	LoadEmptyMenu();
	DisplayTime();

	int iTaskNum = 0;
	int i = 0;
	
	//char sUserLogin[10240]={0};
	//char g_stat_result[300][40]={0};
	//char g_stat_name[300][40]={0};
	int i_stat=0;

	char sBuff[1000][128];

	memset(Buffer,0,COLS_LENS);
	sprintf(Buffer,"ABMҵ������Ϣͳ��   ͳ����Ϣ���ƣ�ͳ����Ϣ���");
	mvaddstr(4,0,Buffer);
	
	
	//---------------------------------------------------------

  //sprintf(sBuff[0],"%s","ͳ����Ϣ���ƣ�ͳ����Ϣ���"); 
  
	
    string sORACLEuser = "abmtest";
    string sORACLEpwd = "abmtest,.804";
    string sORACLEName = "ABM_RAC";

    OracleConn *pOracleConn = NULL;
    TOCIQuery *pQryCursor = NULL;    //select
    pOracleConn = new OracleConn;        
    if (!pOracleConn->connectDb(sORACLEuser, sORACLEpwd, sORACLEName)) {
         printf("ORACLE CONNECT ERROR");
         //return  - 1;
    }
    DEFINE_OCICMDPOINTBYCONN(pQryCursor, (*pOracleConn));
    if (pQryCursor == NULL) {
        printf("pQryCursor ERROR");
       // return  - 1;
    } 
	  string sPreSql2="select stat_name,stat_result from abm_stat ";
	  pQryCursor->close();
    pQryCursor->setSQL(sPreSql2.c_str());
    pQryCursor->open();
              
	  while(pQryCursor->next()) {
		  //strcpy(g_stat_name[i_stat],pQryCursor->field(0).asString());
		  //strcpy(g_stat_result[i_stat],pQryCursor->field(1).asString());
		
		  
		  sprintf(sBuff[n],"%s""               ��""%s",pQryCursor->field(0).asString(),pQryCursor->field(1).asString());
		  n=n+1; 
    }
    pQryCursor->close();
    
	

	lv_iTotalRow=n;

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

