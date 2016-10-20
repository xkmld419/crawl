/***************************/
/* Create 04/04/2005.      */
/* Modify 04/04/2005       */
/***************************/

#ifndef MID_COMPONENT_COMMON_INTERFACE_H
#define MID_COMPONENT_COMMON_INTERFACE_H

#ifndef NULL
#define NULL 			0
#endif /* NULL */

/* Include �ļ� */
#ifdef TUXEDO
#  include <atmi.h>
#  ifdef TX
#     include <tx.h>
#  endif /* TX */
#endif/*TUXEDO*/

#ifdef TONGEASY
#  include "./teapi.h"
#endif /*TONGEASY*/


/* ��������б� */
#   define _COMMON_ERROR_        0
#   define _LOG_ERROR_           100
#   define _TM_ERROR_            200
#   define _CM_ERROR_            300

/* Ϊ����TongEASY,TUXEDO�����Ľṹ���� */
#ifdef TUXEDO
   typedef struct pkt_info
   {	
	char Ver;		/* ���ݰ����� */
	char PktType[5];	/* ���ݰ����ͱ�ʶ����������д */
	long TxFlag;		/* ���װ�����������Ϣ */
				/* 0x00010000L:	���ݰ�ѹ�� */
				/* 0x00020000L:	�ļ�ѹ�� */
				/* 0x00040000L:	���ݰ�����?*/
				/* 0x00080000L:	�ļ����� */
				/* 0x00001000L:	��Ҫ���ַ��� */
	char Src[13];		/* ��������� */
	char Snd[13];		/* ���ͻ����� */
	char Rcv[13];		/* ���ջ����� */
	int  TID;		/* ������� */
	char OptCode[7];	/* ����Ĳ����� */
	char EchoCode[3];	/* ��Ӧ�� */
	int  DataLen;		/* �û����ݰ����� */
	int  FileLen;		/* �շ��ļ������� */
	int  TxType;		/* �������� */
	int  EchoTime;		/* �ȴ�Ӧ��ʱ�� */
	int  AckTime;		/* �ȴ�ȷ��ʱ�� */
	int  SvrTime;		/* server ����ʱ�� */
	int  FwdTime;		/* �ȴ�ת��ʱ�� */
	int  FileNum;
	char **FName;
	int  CvsTblNo;	
	int  KeyDataLen;
   } TEPKTINFO;
   
   typedef struct _mcci_tuxedo_
   {
        int    ierrno;	       /* ������ */
        char   *errstr;	       /* ������Ϣ */
   } MCCI_TUXEDO_ERROR;
#endif /*TUXEDO*/

#ifdef TONGEASY
   /* �ڲ�ʹ��ͨѶ�ṹ */
#  define MAXTHREAD 100	/* �߳������� */

#  ifndef FILEMAXLEN
#     define FILEMAXLEN 80
#  endif 

   typedef struct csl_pkt
   {
      TEPKTINFO pinfo;	/* ������Ϣ�ṹ */
      char      *pdata;	/* ���ݻ����� 	*/
      char      *fname; /* �ļ�������	*/
   }CSLPKT;

   typedef struct csl_info
   {
      unsigned long	 thd;	/* �̺߳�	*/
      CSLPKT		*ptr;	/* ͨѶ�ṹָ�� */
   }CSLINFO;

   typedef struct tpinfo_t
   {
      char	usrname[32];	/* client user name */
      char	cltname[32];	/* application client name */
      char	passwd[32];	/* application password */
      char	grpname[32];	/* client group name */
      long	flags;		/* initialization flags */
      long	datalen;	/* length of app specific data */
      long	data;		/* placeholder for app data */
   }TPINIT;
      
   /* client identifier structure */
   struct clientid_t
   {
     long clientdata[4];     /* reserved for internal use */
   };
   typedef struct clientid_t CLIENTID;

   /* interface to service routines */
   struct tpsvcinfo 
   {
#     define XATMI_SERVICE_NAME_LENGTH  32
      char	name[XATMI_SERVICE_NAME_LENGTH];/* service name invoked */
      long	flags;		/* describes service attributes */
      char	*data;		/* pointer to data */
      long	len;		/* request data length */
      int	cd;		/* reserved for future use */
      long	appkey;		/* application authentication client key */
      CLIENTID cltid;		/* client identifier for originating client */
   };
      
#endif /*TONGEASY*/


/* Flags to service routines */

#define CSLNOBLOCK	0x00000001	/* non-blocking send/rcv */
#define CSLSIGRSTRT	0x00000002	/* restart rcv on interrupt */
#define CSLNOREPLY	0x00000004	/* no reply expected */
#define CSLNOTRAN	0x00000008	/* not sent in transaction mode */
#define CSLTRAN		0x00000010	/* sent in transaction mode */
#define CSLNOTIME	0x00000020	/* no timeout */
#define CSLABSOLUTE	0x00000040	/* absolute value on tmsetprio */
#define CSLGETANY	0x00000080	/* get any valid reply */
#define CSLNOCHANGE	0x00000100	/* force incoming buffer to match */
#define RESERVED_BIT1	0x00000200	/* reserved for future use */
#define CSLCONV		0x00000400	/* conversational service */
#define CSLSENDONLY	0x00000800	/* send-only mode */
#define CSLRECVONLY	0x00001000	/* recv-only mode */
#define CSLACK		0x00002000	/* */

/* Flags to wwinit() */

#define CSLU_SIG		0x00000001	/* ͨ���ź�ѡ��δ�����������ͨ�� */
#define CSLU_DIP		0x00000002	/* ͨ��dip-inѡ��δ�����������ͨ�� */
#define CSLU_IGN		0x00000004	/* ����δ�����������ͨ�� */

#define CSLSA_FASTPATH	0x00000008	/* System access == fastpath */
#define CSLSA_PROTECTED	0x00000010	/* System access == protected */

/* ����rvalȡֵ*/
#define CSLFAIL	    0x00000001	        /* service FAILure for tpreturn */
#define CSLSUCCESS   0x00000002	        /* service SUCCESS for tpreturn */
#define CSLEXIT	    0x08000000	        /* service failue with server exit */



/* ����Ӧ�ó�����Ϣ�ṹ */
typedef  TPINIT    APPINIT;
typedef struct tpsvcinfo CSLSVCINFO;

/*************************************
*	FUNCTIONS DECLARE            *
**************************************/
/*
���к���ִ�гɹ�����ֵ>=0;ʧ���򷵻�-ErrNo.ErrNoΪ�������
*/

extern "C"
{
int csllogin(int AppID, APPINIT *AppInfo);/*AppID	Ӧ�ó�������	AppInfo	Ӧ�ó�����Ϣ*/
int csllogout(void);
int cslbegin(void);
int cslcommit(void);
int cslrollback(void);

/* 
svc	Service����
idata	���ͻ�������ַ
ilen	���͵����ݰ�����
odata	���ջ������ĵ�ַָ��
olen	�������ݰ����ȵĵ�ַ
*/
int cslcall(char *svc, char *idata, long ilen, char **odata, long *olen);

#ifdef COMMON

/*
rval	��������־
rcode	���ش���
data	���ص����ݰ�
len	���ص����ݰ�����
*/
int cslreturn_common(int rval,long rcode,char *data,long len);

/*
svc	����ķ�����
data	ת�������ݰ�
len	ת�������ݰ�����
*/
int cslsvrfwd(char *svc,char *data,long len);

int cslopen_common();
int cslclose_common();

#endif /*COMMON*/

/*
svc	����ķ�����
data	���͵����ݰ�
len	�������ݰ�����
flags	���Ʊ�־
*/
int cslconnect(char *svc,char *data,long len,long flags);

int csldiscon(int cd);/* cd:��wwConnect()���صĻỰ������ */

/*
cd	��wwConnect()���صĻỰ������
data	���͵����ݰ�
len	���͵����ݰ�����
flags	���Ʊ�־
*/
int cslsend(int cd,char *data,long len,long flags);

/*
cd	��wwConnect()���صĻỰ������
data	���յ����ݰ�
len	���յ����ݰ�����
flags	���Ʊ�־
*/
int cslrecv(int cd,char **data,long *len,long flags);

/*
type	����������
size	��������С
*/
char * cslalloc(char *type ,long size);


/*ptr:wwalloc()���صĵ�ַָ��*/
void  cslfree(char *ptr);

/*errno:�����*/
char * cslerrstr(int ierrno);

}

#endif /*MID_COMPONENT_COMMON_INTERFACE_H*/

