#include "LoggDis.h"
#include "ThreeLogGroupDefine.h"

vector<string> DisplayLogg::m_vAppInfo;
vector<string> DisplayLogg::m_vProInfo;
vector<string> DisplayLogg::m_vAlarmInfo;
vector<string> DisplayLogg::m_vParamInfo;
vector<string> DisplayLogg::m_vSystemInfo;
vector<string> DisplayLogg::m_vSysOperInfo;
	
    //Levle 
vector<string> DisplayLogg::m_vLevFatal;
vector<string> DisplayLogg::m_vLevError;
vector<string> DisplayLogg::m_vLevWarn;
vector<string> DisplayLogg::m_vLevInfo;
vector<string> DisplayLogg::m_vLevDebug;     
    
    //Class
vector<string> DisplayLogg::m_vClaError;
vector<string> DisplayLogg::m_vClaWarn;      
vector<string> DisplayLogg::m_vClaInfo; 
    
    //Type 
vector<string> DisplayLogg::m_vTypeBusi;
vector<string> DisplayLogg::m_vTypeParam;
vector<string> DisplayLogg::m_vTypeSystem ;
vector<string> DisplayLogg::m_vTypeStart;
vector<string> DisplayLogg::m_vTypeOracle;   

//Code
vector<string> DisplayLogg::m_vCodeInfo;


void DisplayLogg::DisInit()
{
   //����������ʱ����(��λ����)
    int INTERVAL = 1;
    //�˵���ҳ��ʾ������
    int iLoggPageSize = 30;
    //ȫ�ֱ�������¼�ڲ˵�֮�䴫�ݵĲ���
    int Logg_ProblemProcSelectedRow = 1;
    int Logg_ProblemProcStartRow = 1;
    int NOW_USED=0;

    int LogFlagAll=0; 
}

void DisplayLogg::Loggstartwin()
{
    //������ֹ�ź�
    signal(SIGINT,SIG_IGN);
    initscr();
    noecho();
    intrflush(stdscr,true); 
    savetty();
    keypad(stdscr,TRUE);
}
void DisplayLogg::LoggLoadGeneralInfo()
{
    //----------------------------���ز˵���ͨ����Ϣ--------------------------------
    //�����ܱ���
    attron(A_BOLD); 
    mvaddstr(0,(COLS_LENS - strlen((char *)"��־��ѯ"))/2,(char *)"��־��ѯ");   	   	
    attroff(A_BOLD);
    
    /*���ز˵�����*/
    mvhline(3,0,'=',COLS_LENS);
    mvhline(5 + iLoggPageSize + 1,0,'=',COLS_LENS);
}
void DisplayLogg::LoggLoadEmptyMenu()
{
    mvhline(2,0,' ',112);//�����
    mvhline(4,0,' ',COLS_LENS);//�����
    for(int i=6;i<iLoggPageSize+6;i++)
    {
        mvhline(i,0,' ',COLS_LENS);	
    }
    mvhline(5 + iLoggPageSize + 3,0,' ',COLS_LENS);
    mvaddstr(5 + iLoggPageSize + 3,13,(char *)"Up Arrow = ����   Down Arrow = ����   ESC = �˳�");

    return;
}

void DisplayLogg::LoggloadAll(vector<string> &m_vInfo)
{
    m_vConverInfo.clear();
    NOW_USED = m_vInfo.size();

    if(NOW_USED==0)
    {
        m_vConverInfo.push_back("**************û�в�ѯ�����ϵ���Ϣ**************");
        NOW_USED = m_vConverInfo.size();
        return ;
    }
    vector<string>::iterator iter;

    for(iter=m_vInfo.begin();iter!=m_vInfo.end();++iter)
    {
        int size = (*iter).size();
        if(size>COLS_LENS)
        {
        	//oh,my  god 
            int index=0;
            char temp_1[512],temp_2[512];
			int iTrunc = 0,iNoAsciiCnt = 0;

			while(size>COLS_LENS)
            {
               iTrunc = 0;
               memset(temp_1,0,sizeof(temp_1));
               (*iter).copy(temp_1,COLS_LENS,index);

			   iNoAsciiCnt = 0;
			   for(int k = COLS_LENS -1; k != -1; --k)
			   {
			   		if(!isascii(temp_1[k])) 
			   		{
			   			++iNoAsciiCnt;
			   		} else {
						break;
					}
			   }

			   if(iNoAsciiCnt % 2)
			   	{
			   		temp_1[COLS_LENS-1] = '\0';
					iTrunc = 1;
			   	}
               m_vConverInfo.push_back(temp_1);
               //size -= COLS_LENS;
               size = size - COLS_LENS + iTrunc;
               //index += COLS_LENS;
               index = index + COLS_LENS -iTrunc;
            }
            memset(temp_2,0,sizeof(temp_2));
            (*iter).copy(temp_2,COLS_LENS,index);
            m_vConverInfo.push_back(temp_2);
        }
        else
            m_vConverInfo.push_back(*iter);
    }

    NOW_USED = m_vConverInfo.size();
}

void DisplayLogg::LoggRebuildProblemProc()
{
    //�˱����������϶��»��Ʋ˵�
    int lv_iTotalRow = NOW_USED;
    int lv_iCurrentRow = 0;
    static char Buffer[COLS_LENS+1];
    
    LoggLoadEmptyMenu();
    //�����ĸ��ж���Ҫ���ھ���һЩ�Ƿ���
    //���ȫ��ѡ������<=0�����ָ��gv_iMag_ProcSelectedRowinTotalRow ��
    if (Logg_ProblemProcSelectedRow  <= 0)
    {
       Logg_ProblemProcSelectedRow  = lv_iTotalRow ;
    }
    //���ȫ��ѡ������> lv_iTotalRow �����ָ����һ��
    if (Logg_ProblemProcSelectedRow  > lv_iTotalRow)
    {
       Logg_ProblemProcSelectedRow  = 1;
    }
    //���g_iStartRow <= 0����Ϊ��Ϊ��һ��	
    if (Logg_ProblemProcStartRow <= 0)
    {
       Logg_ProblemProcStartRow = 1;
    }
    if (Logg_ProblemProcStartRow > Logg_ProblemProcSelectedRow )
    {
       Logg_ProblemProcStartRow = Logg_ProblemProcSelectedRow ;
    }
    if (Logg_ProblemProcSelectedRow  - Logg_ProblemProcStartRow >= iLoggPageSize)
    {
       Logg_ProblemProcStartRow = Logg_ProblemProcSelectedRow  - iLoggPageSize + 1;
    }
    // --ȷ��lv_iCurrentRow��ʼֵ
    while(1)
    {
        if (lv_iCurrentRow >= Logg_ProblemProcStartRow )
        {
            break;
        }
        else
        {
            lv_iCurrentRow++;
        }
    }	
    for(int i=0; i<iLoggPageSize; i++)
    {
        if ((lv_iCurrentRow > lv_iTotalRow)||(lv_iCurrentRow >= Logg_ProblemProcStartRow + iLoggPageSize))
        {
            break;
        }
        if (lv_iCurrentRow == Logg_ProblemProcSelectedRow)
        {
            standout();
        }
        mvhline(lv_iCurrentRow - Logg_ProblemProcStartRow + 5,0,' ',COLS_LENS);
        memset(Buffer,0,COLS_LENS);
        if(Logg_ProblemProcStartRow+i<=NOW_USED)
            sprintf(Buffer,"%s",(m_vConverInfo[Logg_ProblemProcStartRow+i-1]).c_str());
		//modify by liyr
		//mvaddstr(4,0,"ʱ��  ����  ��������  PRO_ID  APP_ID  ����  ���  ����  ����  ��������  �����ֶ�  ��������ʶ  ����");
		mvaddstr(4,0,"ʱ ��    �� ��   �� ��   �� ��   ��������   ����ID   �߳�ID   ģ��ID   �Զ���Ԫ   �� ��  �� ��   ��������   �����ֶ�   ��־����");
		mvaddstr(lv_iCurrentRow - Logg_ProblemProcStartRow + 5,0,Buffer);
        if (lv_iCurrentRow == Logg_ProblemProcSelectedRow)
        {
            standend();
        }
        lv_iCurrentRow++;
    }
    //δ����������հ�		
    while (lv_iCurrentRow < Logg_ProblemProcStartRow + iLoggPageSize)
    {
        mvhline(lv_iCurrentRow - Logg_ProblemProcStartRow + 5,0,' ',COLS_LENS);
        lv_iCurrentRow ++;
    }
    wmove(stdscr,Logg_ProblemProcSelectedRow - Logg_ProblemProcStartRow + 5,0);//��ָ���Ƶ�ѡ������
    refresh();
}
void DisplayLogg::LoggTimeBreak_ProblemProc(int signo)
{
    LoggRebuildProblemProc();
    return;
}

void DisplayLogg::LoggStartProblemProc()
{
    while(1)
    {
        LoggRebuildProblemProc();
        signal(SIGALRM, LoggTimeBreak_ProblemProc);
        alarm(INTERVAL); 
        int Key = getch();
        alarm(0);
        //�������Esc�����˳��˵�
        if (Key == 27)
        {
          Logg_ProblemProcSelectedRow=1;
          Logg_ProblemProcStartRow=1;
          break;
        }
        switch (Key)
        {
            //����س�
            case '\n':
            case '\r':
              break;
            case KEY_UP:
                Logg_ProblemProcSelectedRow--;
              break;
            case KEY_DOWN:
                Logg_ProblemProcSelectedRow++;
              break;
            case KEY_LEFT:
                Logg_ProblemProcStartRow -= iLoggPageSize;
                Logg_ProblemProcSelectedRow -= iLoggPageSize;
              break;
            case KEY_RIGHT:
                Logg_ProblemProcStartRow += iLoggPageSize;
                Logg_ProblemProcSelectedRow += iLoggPageSize;
              break;
            default	:
              break;
        }
    }
}

void DisplayLogg::LoggRebuildProcess()
{
    //�˱����������϶��»��Ʋ˵�
    int lv_iTotalRow = 0;
    int lv_iCurrentRow = 0;
    static char Buffer[COLS_LENS];
    
    LoggLoadEmptyMenu();
    lv_iTotalRow = iDisParamCnt;
    lv_iCurrentRow =1;
    //�����ĸ��ж���Ҫ���ھ���һЩ�Ƿ���
    //���ȫ��ѡ������<=0�����ָ��gv_iMainTotalRow ��
    if (Logg_Main2SelectedRow  <= 0)
    {
       Logg_Main2SelectedRow  = lv_iTotalRow ;
    }
    //���ȫ��ѡ������> lv_iTotalRow �����ָ����һ��
    if (Logg_Main2SelectedRow  > lv_iTotalRow)
    {
       Logg_Main2SelectedRow  = 1;
    }
    //���g_iStartRow <= 0����Ϊ��Ϊ��һ��	
    if (Logg_Main2StartRow <= 0)
    {
       Logg_Main2StartRow = 1;
    }
    if (Logg_Main2StartRow > Logg_Main2SelectedRow )
    {
       Logg_Main2StartRow = Logg_Main2SelectedRow ;
    }
    if (Logg_Main2SelectedRow  - Logg_Main2StartRow >= iLoggPageSize)
    {
       Logg_Main2StartRow = Logg_Main2SelectedRow  - iLoggPageSize + 1;
    }
    while(1)
    {
        if (lv_iCurrentRow >= Logg_Main2StartRow )
        {
           break;
        }
        else
        {
            lv_iCurrentRow++;
        }
    }
    while(lv_iCurrentRow <= lv_iTotalRow)
    {
        if (lv_iCurrentRow >= Logg_Main2StartRow + iLoggPageSize)
        {
            break;
        }
        if (lv_iCurrentRow == Logg_Main2SelectedRow)
        {
           standout();
        }
        mvhline(lv_iCurrentRow - Logg_Main2StartRow + 5,0,' ',COLS_LENS);
        memset(Buffer,0,COLS_LENS);
        
        //������
        sprintf(Buffer,"%d",lv_iCurrentRow);
        mvaddstr(lv_iCurrentRow - Logg_Main2StartRow + 5,0,Buffer);
        //������
//        mvaddstr(lv_iCurrentRow - Logg_Main2StartRow + 5,10,(char*)"��־��Ϣ��ѯ");
        //�������
        memset(Buffer,0,COLS_LENS);
        sprintf(Buffer,"%s",Logg_pMenuInfo[lv_iCurrentRow-1].ChName);
        mvaddstr(lv_iCurrentRow - Logg_Main2StartRow + 5,10,Buffer);
        if (lv_iCurrentRow == Logg_Main2SelectedRow)
        {
            standend();
        }
        lv_iCurrentRow++;
    }
      //δ����������հ�		
    while (lv_iCurrentRow < Logg_Main2StartRow + iLoggPageSize)
    {
        mvhline(lv_iCurrentRow - Logg_Main2StartRow + 5,0,' ',COLS_LENS);
        lv_iCurrentRow ++;
    }
    wmove(stdscr,Logg_Main2SelectedRow - Logg_Main2StartRow + 5,0);//��ָ���Ƶ�ѡ������
    refresh();
}

void DisplayLogg::LoggTimeBreak_Process(int signo)
{
    LoggRebuildProcess();
    return;
}

void DisplayLogg::LoggInitProcessInfo(int flag,char* sParam)
{
    int iClassId=0;
    int iLevel=0;
    int iType=0;
    switch(flag)
    {
        case GROUPPARAM:
            iDisParamCnt = 1;
            Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
            Logg_pMenuInfo[0].Index  = 1;
            strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ������������־��Ϣ");
            break;
        case GROUPMODULE:
        case GROUPPROCESS:
            iDisParamCnt = 3;
            Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
            Logg_pMenuInfo[0].Index  = 1;
            strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ������(error/fatal)��־��Ϣ");

            Logg_pMenuInfo[1].Index  = 2;
            strcpy(Logg_pMenuInfo[1].ChName,"ϵͳ�澯��(warning)��־��Ϣ");

            Logg_pMenuInfo[2].Index  = 3;
            strcpy(Logg_pMenuInfo[2].ChName,"ϵͳ��ͨ��(debug/info)��־��Ϣ");
            break;                    	
        case GROUPINFOLV:
            if(strlen(sParam))
            {
                iLevel = atoi(sParam);
                iDisParamCnt = 1;
                Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
                Logg_pMenuInfo[0].Index  = 1;
                if(iLevel==LOG_LEVEL_FATAL)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ�ȼ�һ(���Ѵ���fatal)��־��Ϣ");
                else if(iLevel==LOG_LEVEL_ERROR)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ�ȼ���(����error)��־��Ϣ");
                else if(iLevel==LOG_LEVEL_WARNING)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ�ȼ���(�澯warning)��־��Ϣ");
                else if(iLevel==LOG_LEVEL_INFO)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ�ȼ���(��ͨinfo)��־��Ϣ"); 
                else if(iLevel==LOG_LEVEL_DEBUG)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ�ȼ���(����debug)��־��Ϣ");                                           
                else
                    strcpy(Logg_pMenuInfo[0].ChName,"�������LEVEL_ID������!");
            }
            else
            {
                iDisParamCnt = 5;
                Logg_pMenuInfo = new Log_Menu[iDisParamCnt];

                Logg_pMenuInfo[0].Index  = 1;
                strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ�ȼ�һ(���Ѵ���fatal)��־��Ϣ");

                Logg_pMenuInfo[1].Index  = 2;
                strcpy(Logg_pMenuInfo[1].ChName,"ϵͳ�ȼ���(����error)��־��Ϣ");
                
                Logg_pMenuInfo[2].Index  = 3;
                strcpy(Logg_pMenuInfo[2].ChName,"ϵͳ�ȼ���(�澯warning)��־��Ϣ");
                
                Logg_pMenuInfo[3].Index  = 4;
                strcpy(Logg_pMenuInfo[3].ChName,"ϵͳ�ȼ���(��ͨinfo)��־��Ϣ");
                
                Logg_pMenuInfo[4].Index  = 5;
                strcpy(Logg_pMenuInfo[4].ChName,"ϵͳ�ȼ���(����debug)��־��Ϣ");                                
            }
            break;
        case GROUPCLASS:
            if(strlen(sParam))
            {
                iClassId = atoi(sParam);
                iDisParamCnt = 1;
                Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
                Logg_pMenuInfo[0].Index  = 1;
                if(iClassId==LOG_CLASS_ERROR)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ��������־��Ϣ");
                else if(iClassId==LOG_CLASS_WARNING)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ�澯����־��Ϣ");
                else if(iClassId==LOG_CLASS_INFO)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ��ͨ��Ϣ����־��Ϣ");
                else
                    strcpy(Logg_pMenuInfo[0].ChName,"�������CLASS_ID������!");
            }
            else
            {
                iDisParamCnt = 3;
                Logg_pMenuInfo = new Log_Menu[iDisParamCnt];

                Logg_pMenuInfo[0].Index  = 1;
                strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ��������־��Ϣ");

                Logg_pMenuInfo[1].Index  = 2;
                strcpy(Logg_pMenuInfo[1].ChName,"ϵͳ�澯����־��Ϣ");
                
                Logg_pMenuInfo[2].Index  = 3;
                strcpy(Logg_pMenuInfo[2].ChName,"ϵͳ��ͨ��Ϣ����־��Ϣ");
            }
            break;
        case GROUPTYPE:
            if(strlen(sParam))
            {
                iType = atoi(sParam);
                iDisParamCnt = 1;
                Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
                Logg_pMenuInfo[0].Index  = 1;
                if(iType==LOG_TYPE_BUSI)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳҵ����־��Ϣ");
                else if(iType==LOG_TYPE_PARAM)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ����������־��Ϣ");
                else if(iType==LOG_TYPE_SYSTEM)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳϵͳ������־��Ϣ");
                else if(iType==LOG_TYPE_PROC)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ���̵�����־��Ϣ");
                else if(iType==LOG_TYPE_DATEDB)
                    strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ��������־��Ϣ");                                        
                else
                    strcpy(Logg_pMenuInfo[0].ChName,"�������CLASS_ID������!");
            }
            else
            {
                iDisParamCnt = 5;
                Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
                
                Logg_pMenuInfo[0].Index  = 1;
                strcpy(Logg_pMenuInfo[0].ChName,"ϵͳҵ����־��Ϣ");
                Logg_pMenuInfo[1].Index  = 2;
                strcpy(Logg_pMenuInfo[1].ChName,"ϵͳ����������־��Ϣ");
                Logg_pMenuInfo[2].Index  = 3;
                strcpy(Logg_pMenuInfo[2].ChName,"ϵͳϵͳ������־��Ϣ");
                Logg_pMenuInfo[3].Index  = 4;
                strcpy(Logg_pMenuInfo[3].ChName,"ϵͳ���̵�����־��Ϣ");
                Logg_pMenuInfo[4].Index  = 5;
                strcpy(Logg_pMenuInfo[4].ChName,"ϵͳ��������־��Ϣ");                   

            }
            break;   
        case GROUPCODE:
            iDisParamCnt = 1;
            Logg_pMenuInfo = new Log_Menu[iDisParamCnt];
            Logg_pMenuInfo[0].Index  = 1;
            strcpy(Logg_pMenuInfo[0].ChName,"ϵͳ�������������־��Ϣ");
            break;        	     	
        default:
            break;
    }
    return;
}

void DisplayLogg::LoggProcessMain(int flag,char* sParam)
{
    Loggstartwin();
    LoggLoadGeneralInfo();
    LoggInitProcessInfo(flag,sParam);
    while(1)
    {
        LoggRebuildProcess();
        signal(SIGALRM, LoggTimeBreak_Process);
        alarm(INTERVAL); 
        int Key = getch();
        alarm(0);
        
        //�������Esc�����˳��˵�
        if (Key == 27)
        {
          break;
        }
        switch (Key)
        {
            //����س�
            case '\n':
            case '\r':
                if(flag==GROUPPROCESS||flag==GROUPMODULE)
                {
                    if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 1)
                        LoggloadAll(m_vClaError);
                    else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 2)
                        LoggloadAll(m_vClaWarn);
                    else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 3)
                        LoggloadAll(m_vClaInfo);
                    LoggStartProblemProc();
                }
                else if(flag==GROUPINFOLV)
                {
                    if(strlen(sParam))
                    {
                    	  switch(atoi(sParam))
                    	  {
                    	  	case LOG_LEVEL_FATAL:
                    	  	  LoggloadAll(m_vLevFatal);
                    	  	  break;	
                    	  	case LOG_LEVEL_ERROR:
                    	  	  LoggloadAll(m_vLevError);
                    	  	  break;	
                    	  	case LOG_LEVEL_WARNING:
                    	  	  LoggloadAll(m_vLevWarn);
                    	  	  break;	
                    	  	case LOG_LEVEL_INFO:
                    	  	  LoggloadAll(m_vLevInfo);
                    	  	  break;	
                    	  	case LOG_LEVEL_DEBUG:
                    	  	  LoggloadAll(m_vLevDebug);
                    	  	  break;	                    	  	                      	  	                      	  	                      	  	  
                    	  	default:
                    	  		break;	
                    	  }
                        LoggStartProblemProc();
                    }
                    else
                    {
                        if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 1)
                            LoggloadAll(m_vLevFatal);
                        else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 2)
                            LoggloadAll(m_vLevError);
                        else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 3)
                            LoggloadAll(m_vLevWarn);
                        else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 4)
                            LoggloadAll(m_vLevInfo);
                        else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 5)
                            LoggloadAll(m_vLevDebug);                                                       
                        LoggStartProblemProc();
                    }
                }                  
                else if(flag==GROUPCLASS)
                {
                    if(strlen(sParam))
                    {
                    	  switch(atoi(sParam))
                    	  {
                    	  	case LOG_CLASS_ERROR:
                    	  	  LoggloadAll(m_vClaError);
                    	  	  break;	
                    	  	case LOG_CLASS_WARNING:
                    	  	  LoggloadAll(m_vClaWarn);
                    	  	  break;	
                    	  	case LOG_CLASS_INFO:
                    	  	  LoggloadAll(m_vClaInfo);
                    	  	  break;	                    	  	                      	  	                      	  	                      	  	  
                    	  	default:
                    	  		break;	
                    	  }
                        LoggStartProblemProc();
                    }
                    else
                    {
                      if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 1)
                          LoggloadAll(m_vClaError);
                      else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 2)
                           LoggloadAll(m_vClaWarn);
                      else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 3)
                          LoggloadAll(m_vClaInfo);
                      LoggStartProblemProc();
                    }
                }
                else if(flag==GROUPTYPE)
                {
                    if(strlen(sParam))
                    {
                    	  switch(atoi(sParam))
                    	  {
                    	  	case LOG_TYPE_BUSI:
                    	  	  LoggloadAll(m_vTypeBusi);
                    	  	  break;	
                    	  	case LOG_TYPE_PARAM:
                    	  	  LoggloadAll(m_vTypeParam);
                    	  	  break;	
                    	  	case LOG_TYPE_SYSTEM:
                    	  	  LoggloadAll(m_vTypeSystem);
                    	  	  break;	
                    	  	case LOG_TYPE_PROC:
                    	  	  LoggloadAll(m_vTypeStart);
                    	  	  break;	
                    	  	case LOG_LEVEL_DEBUG:
                    	  	  LoggloadAll(m_vTypeOracle);
                    	  	  break;	                    	  	                      	  	                      	  	                      	  	  
                    	  	default:
                    	  		break;	
                    	  }
                        LoggStartProblemProc();
                    }
                    else
                    {
                        if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 1)
                            LoggloadAll(m_vTypeBusi);
                        else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 2)
                            LoggloadAll(m_vTypeParam);
                        else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 3)
                            LoggloadAll(m_vTypeSystem);
                        else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 4)
                            LoggloadAll(m_vTypeStart);
                        else if(Logg_pMenuInfo[Logg_Main2SelectedRow-1].Index == 5)
                            LoggloadAll(m_vTypeOracle);                                                       
                        LoggStartProblemProc();
                    }
                }                   
                else if(flag==GROUPPARAM)
                {
                    LoggloadAll(m_vTypeParam);
                    LoggStartProblemProc(); 
                }
                else if(flag==GROUPCODE)
                {
                    LoggloadAll(m_vCodeInfo);
                    LoggStartProblemProc();              	
                }
               
              break;
            case KEY_UP:
                Logg_Main2SelectedRow--;
              break;
            case KEY_DOWN:
                Logg_Main2SelectedRow++;
              break;
            default :
              break;
        }
    }
    //�˳���������ģʽ
    resetty();
    endwin();
    system("clear");
    return ;
}

   //����������ʱ����(��λ����)
int DisplayLogg::INTERVAL = 1;
//�˵���ҳ��ʾ������
int DisplayLogg::iLoggPageSize = LINS_SIZE;
//ȫ�ֱ�������¼�ڲ˵�֮�䴫�ݵĲ���
int DisplayLogg::Logg_ProblemProcSelectedRow = 1;
int DisplayLogg::Logg_ProblemProcStartRow = 1;
int DisplayLogg::Logg_Main2SelectedRow = 1;
int DisplayLogg::Logg_Main2StartRow = 1;
int DisplayLogg::NOW_USED=0;
int DisplayLogg::iDisParamCnt=0;
Log_Menu* DisplayLogg::Logg_pMenuInfo = NULL;
Log_Menu* DisplayLogg::Logg_pMenuInfoAll = NULL;

vector<string> DisplayLogg::m_vConverInfo;


