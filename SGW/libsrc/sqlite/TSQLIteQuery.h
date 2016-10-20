/*VER: 1*/ 
#ifndef _SQLITEQuery_h
#define _SQLITEQuery_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <string.h>
//#include "exception.h"
#include "platform.h"
#include "../libite/sqlite3.h"
#include "../lib/CSemaphore.h"
#include "../lib/ReadIni.h"


#define  NAME_LENGTH 128 
#define  MAX_ITESQL_LENGTH 4096
#define  ITE_ERRMSG_LENGTH 2048
#define  MAX_FIELD_LEN 2048
class SITEException 
{
public:
	char *getErrMsg() const;
	char *getErrSrc() const;
	int  getErrCode() const;

public:
	SITEException(int code, const char* errFormat, ...);		
	~SITEException();

private:
	char	errMessage[ITE_ERRMSG_LENGTH+1];		//������Ϣ
	char	errSQL[MAX_ITESQL_LENGTH+1];			//���������sql���
	int	m_iErrCode;
};

class  SQLITEQuery
{
 public:
  	SQLITEQuery();
  	SQLITEQuery(const char *sName);
  	~SQLITEQuery();

 public:
 	 int  openDB(const char *sDBName);
    int closeDB() 	 ;
 	 void init();
	 int  setSQL(char *inSqlstmt);								//����Sqlstatement
	 int  setSQL(const char *inSqlstmt);								//����Sqlstatement
	 int  setSqlWaitLock(char *inSqlstmt);
	 int  open();			//��SQL SELECT��䷵�ؽ����
	 bool next();													//�ƶ�����һ����¼
	 bool execute();	                          //ִ�з�SELECT���,û�з��ؽ����
 	 int  close();													//�ر�SQL��䣬��׼��������һ��sql���	 
	 bool commit();													//�����ύ
	 bool rollback();		
	 int  prepare();
	 bool reset();
	 void printError();
	 
 
  int getFieldAsInteger(int iCol);
  long getFieldAsLong(int iCol)	;
  char* getFieldAsString(int iCol);
  double getFieldAsFloat(int iCol);

  void setParameter(int iCol,int value); 
  void setParameter(int iCol,long value);
  void setParameter(int iCol,const char *value);
  void setParameter(int iCol,double value); 
  
  void setParameter(const char *paramName,int value); 
  void setParameter(const char *paramName,long value);
  void setParameter(const char *paramName,const char *value);
  void setParameter(const char *paramName,double value);   
  
	SQLITEQuery& field(int index)		;	//���ص�i������Ϣ	  
	SQLITEQuery& field(const char *name);	//��������(���ִ�Сд)��������Ϣ; ����ʹ��Field(int i)��ø��ߵ�Ч�� 
	char		*asString();
	double	asFloat();
	int		  asInteger();
	long		asLong();	
	
	void setfun();
private:
	inline void MakeLower(char * pchString)
  {
	  int iLen = strlen(pchString);

	  for(int i=0; i<iLen; i++)
	  {
		  pchString[i] = tolower(pchString[i]);
	  }
  }	;


 public:
   sqlite3 *m_pDB ;
   sqlite3_stmt *m_stmt;
   const char *m_tail;   
   char *m_errMsg ;
   char  m_sSql[MAX_ITESQL_LENGTH];
   char  *m_pSql ;
   char  m_sDataName[256];
   long  m_lRowNum ;
   long  m_lCurrNum ;
    
   int   m_iErrorNo ;   
   bool  m_bConnect ;
	 bool  m_bActivated;						//�Ƿ��Ѿ����ڴ�״̬���ڵ���OCIStmtPrepare�ɹ���ΪTrue      
	 bool  m_bOpened;							//���ݼ��Ƿ��	
	 bool  m_bTransaction ;       //�����Ƿ��  ��excuteʱ��commit/rollbackʱ�رա�
	 
 private:
 	  char m_sTableName[NAME_LENGTH];
 	  char m_sIndexName[NAME_LENGTH];
 	  char m_sViewName[NAME_LENGTH];
 	  char m_sTriggerName[NAME_LENGTH];
	  char *m_pDate;
	  //�����ź����� by jx 20111006
	  long m_lshmkey;
	  CSemaphore *pSem;
	  bool m_bLockFlag;
	  long getKey(char *sDBName);
	  void lock();
	  void unlock();
	  
public:
	 int m_iParamCount ; //��������
	 int m_iCol ;
	 map<string,int> m_mParamMap ;
	 map<string,int> m_mOutParamMap ;	 
	 map<string,int>::iterator m_mIterator ;
	 void getParamsDef();					//��setSQLʱ���ò�����Ϣ	 

};




#endif

   



