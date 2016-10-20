/* add by  zhangyz */
/* date:2010 09 15 */

#ifndef INCLUDED_ABMACCESS_ADMIN_H
#define INCLUDED_ABMACCESS_ADMIN_H

#include <signal.h>
#include <iostream>
#include <map>

#include "Environment.h"
#include "TOCIQuery.h"
#include "exception.h"

#include "abmaccessLog.h"

#include "abmclearfile.h"
#include "Log.h"
#include "Date.h"
#include "ReadIni.h"

//#include "UsrInfoCheckMgr.h"

#define ARG_NUM   128
#define ARG_VAL_LEN  1024


//#define HBACCESS_DEBUG 1

/*
#define HBACCESS_OPT_ADD 1  //����
#define HBACCESS_OPT_DEL 2  //ɾ��
#define HBACCESS_OPT_QRY 3  //��ѯ
#define HBACCESS_OPT_OTHER 4  //����

#define HBACCESS_STATE_OK "OK"  //�ɹ�
#define HBACCESS_STATE_FALSE "FALSE"  //ʧ��
*/

using namespace std;

class hbaccessAdmin
{
  public:
    hbaccessAdmin();
    ~hbaccessAdmin();
    int run();
    int cirPre();
    int deal();
    int usrInfoCheck(char *sInfo);
    int usrInfoCheck(char *sStaffID,char *sKeyWord);
    void trimEcho(char *s1,char *s2);
    void operateLog(int type,char *result,char *cmd,char *res);

  private:
  	int specialOrder( char * orderName,char *paramId1,char *paramId2);
  	int setSQL(char *name, char * line);
  	int delSQL(char *name);
  	int dealcomm(char *name,char *paramId1);
  	int showFieldAd(char *sql);
  	int showField(char *sql);
  	int executeOrder( char * orderName,char *paramId,char *paramId2);
  	void redirectDB(const char * sIniHeader);
  	void clearDataBase();
  	void printHbaccess();
  	void printHbaccessad();
  	

  private:
  	int getTagString();
  	int clearParam();

  	void printfManual()
		{
			  printf("\n ************************************************************************   \n") ;
			  printf("  1:help ��ӡ�����ļ� \n");
			  printf("  2:abmaccess -u ����һ��������Ӧ��ִ�нű� \n");
		    printf("     ex:abmaccess  -u getallproduct=SQL \"select * from PRODUCT\"\n");
		    printf("  3:abmaccess -d ɾ��һ��������Ӧ��ִ�нű�\n");
		    printf("     ex:abmaccess -d getallproduct \n");
		    printf("  4:abmaccess -s ��ʾ�����ִ������ \n");
		    printf("     ex:abmaccess -s ��ʾ�����Զ��������\n");
		    printf("     ex:abmaccess -s getallproduct ִ��getallproduct����\n");
		    printf("     ex:abmaccess -s getallproduct 555 ִ��getallproduct����\n");
		  /*printf("  5:abmaccess -l  �������ݿ��ռ�����������\n");
		   
		    printf("  6:hbaccess ���� + ʱ��(YYYYMMDD)  ����ָ���ļ�ϵͳ��������\n");
		    printf("     -g ����ԭʼ�ɼ��ļ�\n") ;
				printf("     -c ����У������ļ� \n") ;
				printf("     -m �������ݺϲ��м��ļ�\n") ;
				printf("     -r ����������ʷ�ļ�\n") ;
				printf("     -w ����д�ļ���ʷ�ļ�\n") ;
				printf("     -t ����tdtrans�������������ļ�\n") ;
				printf("     -a ����asn����ǰ�ı����ļ�\n") ;
				printf("     -e ��������ײͷѻ��˱��ݵĻ����ļ�\n") ;
				*/
				printf("  5:q or quit or exit �˳�����̨ \n");
		    printf(" ************************************************************************   \n") ;
		  }

  	private:
  		char m_sStaffId[100];
  		char m_sPassed[100];
  		bool m_bUserFlag;

  		int m_iargc;
      char m_sOrder[ARG_VAL_LEN*ARG_NUM];
      char m_sArgv[ARG_NUM][ARG_VAL_LEN];

};

#endif

