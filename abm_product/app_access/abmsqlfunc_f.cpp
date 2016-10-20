/* Copyright (c) 2001-<2010> Linkage, All rights Reserved */
/* #ident "@(#)hbsqlfunc_f.c    1.0 2010/05/10  <AutoCreate>" */
#ifdef DEF_HP
#define _XOPEN_SOURCE_EXTENDED
#endif

#include "TOCIQuery.h"
#include "Environment.h"
#include "abmmainmenu.h"
#include "Date.h"
#include "abmsqlfunc.h"
#include "abmlogin.h"
#include "abmaccessLog.h"

//���ӵ�½�û����Ȩ
#include "GroupDroitMgr.h"

#include <signal.h>
//#include <curses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

Control *hbsqlfunc_handle;

#define MAX_TABLE_NUM 10240
#define _FILE_NAME_ "abmsqlfunc_f.cpp"

#define NUM_ROWS 5000

char list[MAX_TABLE_NUM][512];
char *tmp[MAX_TABLE_NUM];


void Log( char const *format, ...)
{
    char sTemp[10000] , sLine[10000];
    int fd;
    if ((fd=open ("./Log.log", O_RDWR|O_CREAT|O_APPEND, 0770)) < 0) {
        printf ("[ERROR]: log() open ./Log.log error\n");
         return;
    }
    Date dt;
    sprintf (sTemp, "[%s]: %s\n", dt.toString("yyyy-mm-dd hh:mi:ss"), format);
     va_list ap;
    va_start(ap, format);
    vsnprintf (sLine, 9999, sTemp, ap);
    va_end(ap);
    write (fd, sLine, strlen (sLine));
    close (fd);
}

//SQL ɾ������
int executeDel()
{
    int i=0,cnt=0;
    char sEdittmp[100] ={0};
    try{
	    TOCIQuery qry(Environment::getDBConn());
	    Control *pHandle,*tHandle ;
	    pHandle = hbsqlfunc_handle ;
	    tHandle = GetCtlByID (hbsqlfunc_handle, 13);	
	    strcpy(sEdittmp ,tHandle->sData) ;
	    Date d1;
	    d1.getCurDate(); 
	    if(sEdittmp[strlen(sEdittmp)-1] == ';') 
	    	    sEdittmp[strlen(sEdittmp)-1]='\0';
	    qry.setSQL (sEdittmp);
	    if(qry.execute() == true){
	    	 cnt = qry.rowsAffected();
	       qry.commit();
	       Date d2;
	       d2.getCurDate(); 
	       float ft = (d2.getMilSec() -d1.getMilSec());	       
	       sprintf(list[0]," %d rows deleted in %.3f seconds"
	         ,cnt,d2.diffSec(d1)+ ft/10000);
	      //hbDBLog(sFullStaffID,HBACCESS_OPT_DEL,HBACCESS_STATE_OK,sEdittmp,"ɾ���ɹ�");
	      //hbFileLog(sFullStaffID,HBACCESS_OPT_DEL,HBACCESS_STATE_OK,sEdittmp,"ɾ���ɹ�");
		  }
		   else {
		     qry.rollback();		       
		     sprintf(list[0],"delete data lose,please check");
		     //hbDBLog(sFullStaffID,HBACCESS_OPT_DEL,HBACCESS_STATE_FALSE,sEdittmp,"ɾ������ʧ��,����");
	       //hbFileLog(sFullStaffID,HBACCESS_OPT_DEL,HBACCESS_STATE_FALSE,sEdittmp,"ɾ������ʧ��,����");
		  }		  

	    tmp[0] = list[0] ;
	    i = 1 ;
	    qry.close();
    }
    catch(TOCIException &oe)
    {
        Control *lHandle2;
        lHandle2 = GetCtlByID (hbsqlfunc_handle, 11);
        memset(lHandle2->sData,0,300) ;
        sprintf(lHandle2->sData,"   %s"," delete data lose,please check again");
        mvwaddstr(lHandle2->pWin,0,0,lHandle2->sData) ;
    		touchwin(lHandle2->pWin);
    		refresh();
   			wrefresh(lHandle2->pWin);
        sprintf(list[0],"%s\0",oe.getErrMsg());
        i=1 ;
        tmp[0]=list[0] ;
        //hbDBLog(sFullStaffID,HBACCESS_OPT_DEL,HBACCESS_STATE_FALSE,sEdittmp,oe.getErrMsg());
        //hbFileLog(sFullStaffID,HBACCESS_OPT_DEL,HBACCESS_STATE_FALSE,sEdittmp,oe.getErrMsg());
    }
    catch(...)
    {
	      sprintf(list[0],"[EXCEPTION]: Exception catched\n") ;
	      i=1 ;
	      tmp[0]=list[0] ;
	      prtmsg(hbsqlfunc_handle,21,2,"[EXCEPTION]: Exception catched,ִ��ʧ��");
    }
    
    return i;
}

//SQl���²���
int executeUpd()
{
    int i=0,cnt=0;
    char sEdittmp[100] ={0};
    try {
	    TOCIQuery qry(Environment::getDBConn());
	    Control *pHandle,*tHandle ;
	    pHandle = hbsqlfunc_handle ;
	    tHandle = GetCtlByID (hbsqlfunc_handle, 13);
	    if(sEdittmp[strlen(sEdittmp)-1] == ';') 
	    	    sEdittmp[strlen(sEdittmp)-1]='\0';
	    Date d1;
	    d1.getCurDate(); 
	    strcpy(sEdittmp ,tHandle->sData) ;
	    qry.setSQL (sEdittmp);
	    if(qry.execute() == true){
	    	 cnt = qry.rowsAffected();
	       Date d2;
	       d2.getCurDate(); 
	       float ft = (d2.getMilSec() -d1.getMilSec());
	       qry.commit();	       
	       sprintf(list[0],"%d rows updated in %.3f seconds"
	           ,cnt,d2.diffSec(d1)+ ft/10000);
	        //hbDBLog(sFullStaffID,HBACCESS_OPT_UPD,HBACCESS_STATE_OK,sEdittmp,"���³ɹ�");
	        //hbFileLog(sFullStaffID,HBACCESS_OPT_UPD,HBACCESS_STATE_OK,sEdittmp,"���³ɹ�");
		  } else {
		       qry.rollback();
		       sprintf(list[0],"update data lose,please check");
		        //hbDBLog(sFullStaffID,HBACCESS_OPT_UPD,HBACCESS_STATE_FALSE,sEdittmp,"����ʧ��");
	         //hbFileLog(sFullStaffID,HBACCESS_OPT_UPD,HBACCESS_STATE_FALSE,sEdittmp,"����ʧ��");
		  }
		  //Log("ִ��sql��䣺%s\n",sEdittmp) ;
	    tmp[0] = list[0] ;
	    i = 1 ;
	    qry.close();
    }
    catch(TOCIException &oe)
    {
       //prtmsg(hbsqlfunc_handle,5,5,"��������ʧ�ܣ�����");
        Control *lHandle2;
        lHandle2 = GetCtlByID (hbsqlfunc_handle, 11);
        memset(lHandle2->sData,0,300) ;
        sprintf(lHandle2->sData,"   %s"," update data lose,please check again");
        mvwaddstr(lHandle2->pWin,0,0,lHandle2->sData) ;
    		touchwin(lHandle2->pWin);
    		refresh();
   			wrefresh(lHandle2->pWin);
        sprintf(list[0],"%s\0",oe.getErrMsg());
        i=1 ;
        tmp[0]=list[0];
         //hbDBLog(sFullStaffID,HBACCESS_OPT_UPD,HBACCESS_STATE_FALSE,sEdittmp,oe.getErrMsg());
        //hbFileLog(sFullStaffID,HBACCESS_OPT_UPD,HBACCESS_STATE_FALSE,sEdittmp,oe.getErrMsg());  
    }
    catch(...)
    {
       sprintf(list[0],"sqlִ�д���\n");
       i=1;
       tmp[0]=list[0];
       //Log("ִ��sql���ʧ�ܣ�%s\n",sEdittmp) ;
       prtmsg(hbsqlfunc_handle,21,2,"[EXCEPTION]: Exception catched,����ʧ��");
    }
    return i;
 }

//SQL�������
int executeIns()
{
    int i=0,cnt=0;
    char sEdittmp[100] ={0};
    try {
	    TOCIQuery qry(Environment::getDBConn());
	    Control *pHandle,*tHandle ;
	    pHandle = hbsqlfunc_handle ;
	    tHandle = GetCtlByID (hbsqlfunc_handle, 13);
	    strcpy(sEdittmp ,tHandle->sData) ;
	    if(sEdittmp[strlen(sEdittmp)-1] == ';') 
	    	   sEdittmp[strlen(sEdittmp)-1]='\0';
	    qry.setSQL (sEdittmp);
	    Date d1;
	    d1.getCurDate(); 
	    if(qry.execute() == true){
	    	 cnt = qry.rowsAffected();
	       qry.commit();
	       Date d2;
	       d2.getCurDate(); 
	       float ft = (d2.getMilSec() -d1.getMilSec());	       
	       sprintf(list[0]," %d rows insert in %.3f seconds"
	         ,cnt,d2.diffSec(d1)+ ft/10000);
	        //hbDBLog(sFullStaffID,HBACCESS_OPT_INS,HBACCESS_STATE_OK,sEdittmp,"����ɹ�");
	       //hbFileLog(sFullStaffID,HBACCESS_OPT_INS,HBACCESS_STATE_OK,sEdittmp,"����ɹ�");
		  }
		   else {
		       qry.rollback();		       
		       sprintf(list[0],"insert data lose,please check");
		        //hbDBLog(sFullStaffID,HBACCESS_OPT_INS,HBACCESS_STATE_FALSE,sEdittmp,"����ʧ��");
	         //hbFileLog(sFullStaffID,HBACCESS_OPT_INS,HBACCESS_STATE_FALSE,sEdittmp,"����ʧ��");
		  }
	    tmp[0] = list[0] ;
	    i = 1 ;
	    qry.close();
    }
    catch(TOCIException &oe)
    {
       //prtmsg(hbsqlfunc_handle,5,5,"��������ʧ�ܣ�����");
        Control *lHandle2;
        lHandle2 = GetCtlByID (hbsqlfunc_handle, 11);
        memset(lHandle2->sData,0,300) ;
        sprintf(lHandle2->sData,"   %s"," insert data lose,please check again");
        mvwaddstr(lHandle2->pWin,0,0,lHandle2->sData) ;
    		touchwin(lHandle2->pWin);
    		refresh();
   			wrefresh(lHandle2->pWin);
        sprintf(list[0],"%s\0",oe.getErrMsg());
        i=1 ;
        tmp[0]=list[0] ;
         //hbDBLog(sFullStaffID,HBACCESS_OPT_INS,HBACCESS_STATE_FALSE,sEdittmp,oe.getErrMsg());
        //hbFileLog(sFullStaffID,HBACCESS_OPT_INS,HBACCESS_STATE_FALSE,sEdittmp,oe.getErrMsg());      
    }
    catch(...)
    {
        sprintf(list[0],"sqlִ�д���\n") ;
        i=1 ;
        tmp[0]=list[0] ;
        prtmsg(hbsqlfunc_handle,21,2,"[EXCEPTION]: Exception catched,����ʧ��");
    }
    return i;
}

int SetField(char *sEdit)
{
	try{
	    char sTmp[100] = {0};
	    char sTable[50] = {0};
	    char sSql[100] = {0};    
	    char *p = 0;
	    char *q = 0;
	    int n=0;

	    char sUser[32]={0};
	    char sPwd[32]={0};
	    char sStr[32]={0};
	    TOCIDatabase * pConn = new TOCIDatabase();
	    Environment::getConnectInfo("BILL", sUser, sPwd, sStr);
	    if( ! pConn->connect(sUser, sPwd, sStr) ){
	        delete pConn;
	        THROW (1);
	    }
	    TOCIQuery qQry(pConn);
	    strcpy(sTmp,sEdit);
	    
	    for(int i=0;i<strlen(sTmp);i++)   {
	    	if(sTmp[i]>='A'&&sTmp[i]<='Z')  {
	    		sTmp[i]|=0X20 ;
	    	}
	    }
	    
	    p = strstr(sTmp,"from") ;
	    p = p+4 ;
	    while(*p)  {
	        if(*p ==' ')
	            p=p+1 ;
	        else
	            break ;
	    }
	    q = strtok(p," ");
	    
	    strcpy(sTable,q);
	    for(int i=0;i<strlen(sTable);i++)   {
	        if(sTable[i]>='a'&&sTable[i]<='z')
	          sTable[i] = sTable[i] - 32;
	    }
	    
	  	Control *lHandle1,*lHandle2;
	    lHandle1 = GetCtlByID (hbsqlfunc_handle, 10);
	    lHandle2 = GetCtlByID (hbsqlfunc_handle, 11);
	    memset(lHandle1->sData,0,100);
	    memset(lHandle2->sData,0,100);
	    
	    if(strchr(sTmp,'*')){
	    	  memset(sSql,0,100);
	        sprintf(sSql,"select column_name from all_tab_columns where table_name = '%s' \
	           and owner in('BILL') order by column_id",sTable) ;
	        qQry.setSQL(sSql) ;
			    qQry.open();
			    int len=0;
			    char sTTemp[100]={0};
			    while(qQry.next()){
			    	 memset(sTTemp,0x00,sizeof(sTTemp));
			    	 strncpy(sTTemp,qQry.field(0).asString(),sizeof(sTTemp));
			    	 len = len + strlen(sTTemp);			    	 
			       if(len + 2 < 100 )  {
			          if(!lHandle1->sData[0])
			            sprintf(lHandle1->sData,"%s",qQry.field(0).asString()) ;
			          else
			            sprintf(lHandle1->sData,"%s|%s",lHandle1->sData,qQry.field(0).asString()) ;
			       } else if(len +4 < 200) {
			       	  if(!lHandle2->sData[0])
			            sprintf(lHandle2->sData,"%s",qQry.field(0).asString()) ;
			          else
			            sprintf(lHandle2->sData,"%s|%s",lHandle2->sData,qQry.field(0).asString()) ;
			       }else{
			            sprintf(lHandle2->sData,"%s|%s",lHandle2->sData,"...") ;	
			            break;
			       }
			    }		    
	    } else  {
	    	p = strchr(sTmp,' ') ;
	      while(*p)  {
		        if(*p ==' ')
		            p=p+1 ;
		        else
		            break ;
		    }
		    while(q = strchr(p,','))    {
		    	if( (strlen(lHandle1->sData)+2+q-p) < 100 )  {
			         if(!lHandle1->sData[0])
			            strncpy(lHandle1->sData,p,q-p) ;
			         else  {
			            strcat(lHandle1->sData,"|") ;
			            strncat(lHandle1->sData,p,q-p) ;  
			          }
			       }
			       else  {
			       	if(!lHandle2->sData[0])
			            strncpy(lHandle2->sData,p,q-p) ;
			         else  {
			            strcat(lHandle2->sData,"|") ;
			            strncat(lHandle2->sData,p,q-p) ;  
			          }          
	          }
	          p = q+1 ;
	        }
	       q = strchr(p,' ')  ;
	       if( (strlen(lHandle1->sData)+2+q-p) < 100)  {
	       			if(!lHandle1->sData[0])
			            strncpy(lHandle1->sData,p,q-p) ;
			         else  {
			            strcat(lHandle1->sData,"|") ;
			            strncat(lHandle1->sData,p,q-p) ;  
			          }
			       }
			   else  {
		       	if(!lHandle2->sData[0])
		            strncpy(lHandle2->sData,p,q-p) ;
		         else  {
		            strcat(lHandle2->sData,"|") ;
		            strncat(lHandle2->sData,p,q-p) ;  
		          }          
		      }
		  }
	   // prtmsg (hbsqlfunc_handle, 3,1, lHandle1->sData);
	    mvwaddstr(lHandle1->pWin,0,0,lHandle1->sData) ;
	    mvwaddstr(lHandle2->pWin,0,0,lHandle2->sData) ;
	    touchwin(lHandle1->pWin);
	    touchwin(lHandle2->pWin);
	    refresh();
	    wrefresh(lHandle1->pWin);
	    wrefresh(lHandle2->pWin);
	   // refresh();
	  //noecho();
	  qQry.close() ;
	  }
    catch(TOCIException &oe)
    {
       cout<<oe.getErrMsg()<<endl;  
    }
    catch(...)
    {
       cout<<"13"<<endl;
    }
    return 0 ;
}

int checktables(char *sql)   {
	int num =0;
	//char * p ;
	//char tmp[100] ={0} ;
	char sSql[200] = {0} ;
	/*if((p = strstr(sql,"from")))  {
    	p = p+4 ;
  }else 	return 0 ;
   
  strcpy(tmp,p);
  hbtrim(tmp);
  p = strchr(tmp,' ') ;
  if(p)  {
   	*p = '\0' ;
  }
  
  for(int i=0;i<strlen(tmp) ;i++)  {
  	if(tmp[i]>='a' && tmp[i]<='z') 
       tmp[i]=tmp[i]-32 ;
  }*/
 
  TOCIQuery qry(Environment::getDBConn());
 // sprintf(sSql,"SELECT NUM_ROWS  FROM ALL_TABLES  WHERE TABLE_NAME = '%s' AND "
  //             "(OWNER = 'COMM' OR OWNER = 'BILL')",tmp ) ;
  sprintf(sSql,"select count(*) from (%s) ",sql);
  qry.setSQL(sSql) ;
  qry.open() ;
  if(qry.next ())  {
  	num = qry.field (0).asInteger ();
  }
  qry.close ();
  if(num > NUM_ROWS)
  	return 1 ;
  else 
  	return 2 ;
} 

int executeSec()
{
   int i=0;
   char sEdittmp[512] ={0};
try {
    TOCIQuery qry(Environment::getDBConn());
    Control *tHandle,*lHandle;
    char tmpField[100] = {0} ;

    tHandle = GetCtlByID (hbsqlfunc_handle, 13);
    lHandle = GetCtlByID (hbsqlfunc_handle, 3);
    
    strcpy(sEdittmp ,tHandle->sData) ;
    
    if(sEdittmp[strlen(sEdittmp)-1] == ';') 
	    	   sEdittmp[strlen(sEdittmp)-1]='\0';
	    	   
    int flag = checktables(sEdittmp ) ;
    
    if(flag == 1) {
    	memset(list[0],0,512) ;
    	sprintf(list[0],"�޷��Դ����в�����\n") ;
    	tmp[0] = list[0] ;
    	return 1 ;
    }
    //prtmsg (hbsqlfunc_handle, 3,1, sEdittmp);    
    qry.setSQL(sEdittmp);
    qry.open ();

    int j;
    int seq = 1 ;
    int cols = qry.fieldCount();
    while(qry.next ())
    {
        j=1 ;
        memset(list[i],0,512) ;
        sprintf(list[i],"%-3d- %s",seq,qry.field(0).asString());
        while(cols >j) {
            memset(tmpField,0,100);
            
            if(strlen(qry.field(j).asString()) > lHandle->iWidth -8)
                snprintf(tmpField,lHandle->iWidth -8,"%s ...",qry.field(j).asString()) ;
            else
                strcpy(tmpField,qry.field(j).asString());
               
            if(strlen(tmpField)==0)
                strcpy(tmpField,"NULL") ;
            if((strlen(list[i])+ 8 + strlen(tmpField)) > lHandle->iWidth)  {
            	
            	  if(strlen(tmpField) > lHandle->iWidth -8){
	            	  	tmp[i]=list[i] ;
		                i++ ;
		                snprintf(list[i],lHandle->iWidth -8,"%s ...",tmpField) ;
		                j++ ;
		                continue ;
            	  }else{
	                tmp[i]=list[i] ;
	                i++ ;
	                strcpy(list[i],tmpField) ;
	                j++ ;
	                continue ;
              }
            }
            sprintf(list[i],"%s  %s",list[i],tmpField) ;
            j++ ;
        }
        tmp[i]=list[i] ;
        i++ ;
        seq++ ;
    };
     qry.close ();
     SetField(sEdittmp) ;
     
     //hbDBLog(sFullStaffID,HBACCESS_OPT_SEL,HBACCESS_STATE_OK,sEdittmp,"��ѯ�ɹ�");
	  // hbFileLog(sFullStaffID,HBACCESS_OPT_SEL,HBACCESS_STATE_OK,sEdittmp,"��ѯ�ɹ�");
     }
    catch(TOCIException &oe)
    {
       //prtmsg(hbsqlfunc_handle,5,5,"��������ʧ�ܣ�����");
        Control *lHandle2;
        lHandle2 = GetCtlByID (hbsqlfunc_handle, 11);
        memset(lHandle2->sData,0,300) ;
        sprintf(lHandle2->sData,"   %s"," select data lose,please check again");
        mvwaddstr(lHandle2->pWin,0,0,lHandle2->sData) ;
    		touchwin(lHandle2->pWin);
    		refresh();
   			wrefresh(lHandle2->pWin);
        sprintf(list[0],"%s\0",oe.getErrMsg());
        i=1 ;
        tmp[0]=list[0] ;
      // hbDBLog(sFullStaffID,HBACCESS_OPT_SEL,HBACCESS_STATE_FALSE,sEdittmp,oe.getErrMsg());
      // hbFileLog(sFullStaffID,HBACCESS_OPT_SEL,HBACCESS_STATE_FALSE,sEdittmp,oe.getErrMsg());  

    }
    catch(...)
    {
        sprintf(list[0],"sqlִ�д���\n") ;
      i=1 ;
      tmp[0]=list[0] ;
      prtmsg(hbsqlfunc_handle,21,2,"[EXCEPTION]: Exception catched,��ѯʧ��");
       //Log("ִ��sql���ʧ�ܣ�%s\n",sEdittmp) ;
    }
        return i;

}

void clearEmpty(Control *tHandle)
{
	Control *tmp = tHandle ;
	for(int i = 0 ;i<78;i++)  {
		mvwaddch(tmp->pWin,0,i,' ');
	}
	touchwin(tmp->pWin);
  refresh();
  wrefresh(tmp->pWin);
}
	
	
int hbsqlfunc_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{
    /* Add your self code here: */
    if (ch == 27) { /* ESC */
        return FORM_KILL;
    }
    return ch;
}

void hbsqlfunc_entry(Control *pHandle) /* hbsqlfunc ����ʱ�������� */
{
    hbsqlfunc_handle = pHandle;
    /* Add your self code here: */


}

int hbsqlfuncCtl13Press(Control *pCtl,int ch,long lParam)
{   /* SQL�İ�����Ϣ���� */
    /* Add your self code here: */
    return ch;
}

int hbsqlfuncCtl1Press(Control *pCtl,int ch,long lParam)
{   
	  /* ִ��button�İ�����Ϣ���� */
    /* Add your self code here: */
	//GroupDroitMgr *MDroitMgr=new GroupDroitMgr() ;
	//strcpy(sFullStaffID,MDroitMgr->getUsrName());
	//UserFlag = MDroitMgr->IsRootUsr();
    UserFlag=true;
    int i = 0 ;
    char *p ;
    char chTmp[10] = {0} ;
    Control *pProList ;
    Control *pHandle,*tHandle ;
    pHandle = hbsqlfunc_handle ;
    tHandle = GetCtlByID (hbsqlfunc_handle, 13);
   
    char sEdittmp[100] ={0};
    strcpy(sEdittmp ,tHandle->sData) ;
    p = strtok(sEdittmp," ") ;
    while(*p) {
        *p |= 0x20 ;
        chTmp[i] = *p ;
        i++ ;
        p++;
    }
    
    //prtmsg(hbsqlfunc_handle,5,5,chTmp);
   if (ch == '\r') {
   	
   	  Control *lHandle1,*lHandle2 ;
      lHandle1 = GetCtlByID (hbsqlfunc_handle, 10);
      lHandle2 = GetCtlByID (hbsqlfunc_handle, 11);
      memset(lHandle1->sData,0,300) ;
      memset(lHandle2->sData,0,300) ;
      wclear(lHandle1->pWin) ;
      wclear(lHandle2->pWin) ;
      clearEmpty(lHandle1);
      clearEmpty(lHandle2);
    
      pProList = GetCtlByID (hbsqlfunc_handle, 3);
      clearEmpty(pProList);
      int num = 0 ;
	    if(strcmp(chTmp,"select") == 0) {
            num = executeSec() ;
            pProList->iAdditional = num;
            pProList->pData = tmp ;
            CtlShow (pProList);
            CtlAtv (pCtl, pCtl);
	     }else if(strcmp(chTmp,"insert") == 0 ) {
	     	    if(UserFlag){
		            num = executeIns() ;
		            pProList->iAdditional = num;
		            pProList->pData = tmp ;
		            CtlShow (pProList);
		            CtlAtv (pCtl, pCtl);
             }else{
             		pProList->iAdditional = 1;
		         	  sprintf(list[0]," �Բ���,���û�û��INSERTȨ��");
		         	  tmp[0] = list[0] ;
		            pProList->pData = tmp ;
		            CtlShow (pProList);
		            CtlAtv (pCtl, pCtl); 
             }
       }else if(strcmp(chTmp,"update") == 0 ) {
       	   if(UserFlag){
	            num = executeUpd() ;
	            pProList->iAdditional = num;
	            pProList->pData = tmp ;
	            CtlShow (pProList);
	            CtlAtv (pCtl, pCtl);
	          }else{
             		pProList->iAdditional = 1;
		         	  sprintf(list[0]," �Բ���,���û�û��UPDATEȨ��");
		         	  tmp[0] = list[0] ;
		            pProList->pData = tmp ;
		            CtlShow (pProList);
		            CtlAtv (pCtl, pCtl); 
             }
        }else if(strcmp(chTmp,"delete") == 0 ) {
        	  if(UserFlag){
		            num = executeDel() ;
		            pProList->iAdditional = num;
		            pProList->pData = tmp ;
		            CtlShow (pProList);
		            CtlAtv (pCtl, pCtl);
		         }else{
		         	  pProList->iAdditional = 1;
		         	  sprintf(list[0]," �Բ���,���û�û��DELETEȨ��");
		         	  tmp[0] = list[0] ;
		            pProList->pData = tmp ;
		            CtlShow (pProList);
		            CtlAtv (pCtl, pCtl); 
             }
        }else{
        	  sprintf(list[0],"please insert full or right sql sentence");		 
	          tmp[0] = list[0] ;
	        	pProList->iAdditional = 1;
	        	pProList->pData = tmp ;
	          CtlShow (pProList);
	          CtlAtv (pCtl, pCtl);
        }
    }
    return ch;
}

int hbsqlfuncCtl2Press(Control *pCtl,int ch,long lParam)
{   /* �˳��İ�����Ϣ���� */
    /* Add your self code here: */
     if (ch == '\r') {
      hbmainmenu mm;
      mm.run() ;
        return FORM_KILL;
    }

    return ch;
}

int hbsqlfuncCtl3Press(Control *pCtl,int ch,long lParam)
{   /* ����ܵİ�����Ϣ���� */
    /* Add your self code here: */


    return ch;
}
