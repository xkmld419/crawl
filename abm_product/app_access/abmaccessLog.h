#ifndef __ABM_ACCESS_LOG_H_
#define __ABM_ACCESS_LOG_H_

#define HBACCESS_FILE_LOG 1

//#define HBACCESS_DB_LOG 1

#ifdef HBACCESS_OPT_OTHER
#undef HBACCESS_OPT_OTHER
#endif

#ifdef HBACCESS_STATE_OK
#undef HBACCESS_STATE_OK
#endif

#ifdef HBACCESS_STATE_FALSE
#undef HBACCESS_STATE_FALSE
#endif

#define HBACCESS_OPT_INS 1  //����
#define HBACCESS_OPT_DEL 2  //ɾ��
#define HBACCESS_OPT_UPD 3  //����
#define HBACCESS_OPT_SEL 4  //��ѯ
#define HBACCESS_OPT_OTHER 5  //����

#define HBACCESS_STATE_OK "OK"  //�ɹ�
#define HBACCESS_STATE_FALSE "FALSE"  //ʧ��

void hbDBLog(char *sStaffID,int type,char *result,char *cmd,char *res);

void hbFileLog(char *sStaffID,int type,char *result,char *cmd,char *res);


#endif