/* Copyright (c) 2001-<2010> Linkage, All rights Reserved */
/* #ident "@(#)hbclear_f.c	1.0	2010/08/30	<AutoCreate>" */
#include "abmclearfile.h"
#include "abmclear.h"
#include "abmmainmenu.h"

//���ӵ�½�û����Ȩ
#include "GroupDroitMgr.h"

char *s_hbclear[10] ;
char m_hbclear[10][100] ;

Control *hbclear_handle;


int initlist() {
	sprintf(m_hbclear[0],"����ԭʼ�ɼ��ļ�") ;
	sprintf(m_hbclear[1],"����У������ļ�") ;
	sprintf(m_hbclear[2],"�������ݺϲ��м��ļ�") ;
	sprintf(m_hbclear[3],"����������ʷ�ļ�") ;
	sprintf(m_hbclear[4],"����д�ļ���ʷ�ļ�") ;
	sprintf(m_hbclear[5],"����tdtrans�������������ļ�") ;
	sprintf(m_hbclear[6],"����asn����ǰ�ı����ļ�") ;
	sprintf(m_hbclear[7],"��������ײͷѻ��˱��ݵĻ����ļ�") ;
	s_hbclear[0]=m_hbclear[0] ;
	s_hbclear[1]=m_hbclear[1] ;
	s_hbclear[2]=m_hbclear[2] ;
	s_hbclear[3]=m_hbclear[3] ;
	s_hbclear[4]=m_hbclear[4] ;
	s_hbclear[5]=m_hbclear[5] ;
	s_hbclear[6]=m_hbclear[6] ;
	s_hbclear[7]=m_hbclear[7] ;
	return 8 ;
}


int hbclear_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    /* Add your self code here: */

    return ch;
}

void hbclear_entry(Control *pHandle) /* hbclear ����ʱ�������� */
{   
    hbclear_handle = pHandle;
    /* Add your self code here: */
    Control *hbclear_list ;
    int num ;
    hbclear_list = GetCtlByID (hbclear_handle, 3) ;
    num = initlist() ;
    hbclear_list->iAdditional = num ;
	  hbclear_list->pData = s_hbclear ;
}

int hbclearCtl13Press(Control *pCtl,int ch,long lParam)
{   /* ʱ��İ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}

int hbclearCtl1Press(Control *pCtl,int ch,long lParam)
{   /* ִ��button�İ�����Ϣ���� */
    /* Add your self code here: */
    if (ch == '\r') {
    	Control *hbClear_List =NULL;
    	Control *hbClear_Edit =NULL;
    	hbClear_Edit = GetCtlByID(hbclear_handle,13) ;
    	
    	int flag = 0 ;
	    	try
	    	{
		    	ClearFile cf ;
		    	cf.setEndTime(hbClear_Edit->sData) ;
		    	hbClear_List = GetCtlByID (hbclear_handle, 3) ;

			/*
			GroupDroitMgr *MDroitMgr=new GroupDroitMgr();
			strcpy(sFullStaffID,MDroitMgr->getUsrName());
			UserFlag = MDroitMgr->IsRootUsr();
		    */
		    UserFlag=true;
		    	if(UserFlag){
		    		  if(strlen(hbClear_Edit->sData) == 0){
		    		  		prtmsg(pCtl,20,53,"������ʱ��") ;
				    		 	return ch;
		    		  } 
						  for(int i = 0 ; i<hbClear_List->iAdditional ;i++)  {
								 if(hbClear_List->sData[0] == SELECTED )  {
								 	 cf.clearGaherTaskFile() ;
								 	 flag++ ;
								 }
								 if(hbClear_List->sData[1] == SELECTED )  {
								 	 cf.clearCheckErrorFile() ;
								 	 flag++ ;
								 }
								 if(hbClear_List->sData[2] == SELECTED )  {
								 	 cf.clearDataMergeFile() ;
								 	 flag++ ;
								 }
								 if(hbClear_List->sData[3] == SELECTED )  {
								 	 cf.clearDupHisFile() ;
								 	 flag++ ;
								 }
								 if(hbClear_List->sData[4] == SELECTED )  {
								 	 cf.clearwdHisFile() ;
								 	 flag++ ;
								 }
								 if(hbClear_List->sData[5] == SELECTED )  {
								 	 cf.clearTdtransTmpFile() ;
								 	 flag++ ;
								 }
								 if(hbClear_List->sData[6] == SELECTED )  {
								 	 cf.clearAsnBkFile() ;
								 	 flag++ ;
								 }
								 if(hbClear_List->sData[7] == SELECTED )  {
								 	 cf.clearRSexbakFile() ;
								 	 flag++ ;
								 }		  	
				    }
				 }else{
				 	  for(int j = 0 ; j<hbClear_List->iAdditional ;j++)  {
				    		 if(hbClear_List->sData[0] == SELECTED ) {
				    		 	      flag++ ;
				    		 		 		prtmsg(pCtl,20,53,"�Բ���,���û�û�������ļ���Ȩ��") ;
				    		 		 		break;
				    		 	}
		    		}
				 }
				 
		    if(!flag)
		      prtmsg(pCtl,20,53,"���ÿո��ѡ��Ҫִ�е�����") ;
	    }
	    catch(TOCIException &e)
	    {
	    		prtmsg(pCtl,20,53,e.getErrMsg()) ;
	    		return ch;	
	    }
    }
    return ch;
}

int hbclearCtl2Press(Control *pCtl,int ch,long lParam)
{   /* �˳��İ�����Ϣ���� */
    /* Add your self code here: */
    if (ch == '\r') {
     hbmainmenu tm;
	   tm.run() ;
		 return FORM_KILL;
	 }

    return ch;
}

int hbclearCtl3Press(Control *pCtl,int ch,long lParam)
{   /* ����ܵİ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}


