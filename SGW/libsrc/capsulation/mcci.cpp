/*���Ƕ�TUXEDO��TONGEASY����API�ķ�װ����,������TUXEDO����ʹ��*/
/*************************************
 *  Author: Linkage                  *
 *  Create Date:     2005/04/04      * 
 *  The last Modify: 2005/04/04      *
**************************************/
/*#define TUXEDO*/

#include "./mcci.h"
#include <stdlib.h>

/* ��������б� */
MCCI_TUXEDO_ERROR TuxedoErrNo[27] = {
	                              {0,     "no error"      },
                                      {10205, "TPEABORT"      },
                                      {10012, "TPEBADDESC"    },
                                      {10010, "TPEBLOCK"      },
                                      {10013, "TPEINVAL"      },
                                      {10310, "TPELIMIT"      },
                                      {10007, "TPENOENT"      },
                                      {10003, "TPEOS"         },
                                      {10106, "TPEPERM"       },
                                      {10001, "TPEPROTO"      },
                                      {10306, "TPESVCERR"     },
                                      {10305, "TPESVCFAIL"    },
                                      {10002, "TPESYSTEM"     },
                                      {10006, "TPETIME"       },
                                      {10003, "TPETRAN"       },
                                      {10011, "TPGOTSIG"      },
                                      {10319, "TPERMERR"      },
                                      {10008, "TPEITYPE"      },
                                      {10009, "TPEOTYPE"      },
                                      {10320, "TPERELEASE"    },
                                      {10207, "TPEHAZARD"     },
                                      {10206, "TPEHEURISTIC"  },
                                      {10314, "TPEEVENT" 	  },
                                      {10315, "TPEMATCH"	  },
                                      {10316, "TPEDIAGNOSTIC" },
                                      {10317, "TPEMIB"  	  },
                                      {10318, "TPMAXVAL"  	  }
                                    };
extern "C"
{
/*************************************
 *  ����: ����ע��                   *
 *  ����: AppID��Ӧ�ó�������        * 
 *        AppInfo��Ӧ�ó���ע����Ϣ  *
 *  ����ֵ��                         *
 *          =0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int csllogin(int AppID,APPINIT *AppInfo)
{
	if (tpinit(AppInfo)==-1)
		return (0-TuxedoErrNo[tperrno].ierrno);
	else
		return 0;
}

/*************************************
 *  ����: ����ע��                   *
 *  ����: ��                         * 
 *  ����ֵ��                         *
 *          =0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int csllogout()
{
	if (tpterm()==-1)
		return(0-TuxedoErrNo[tperrno].ierrno);
	else
		return 0;
}

/*************************************
 *  ����: ���׿�ʼ                   *
 *  ����: ��                         * 
 *  ����ֵ��                         *
 *          =0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int cslbegin()
{
	int i;
	
	#ifdef TX
		i=tx_begin();
	#else
		i=tpbegin((long)0,(long)0);
	#endif /*TX*/
	
	if (i==-1)
		return(0-TuxedoErrNo[tperrno].ierrno);
	else
		return 0;
}

/*************************************
 *  ����: �����ύ                   *
 *  ����: ��                         * 
 *  ����ֵ��                         *
 *          =0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int cslcommit()
{
	int i;
	
	#ifdef TX
		i=tx_commit();     
	#else
		i=tpcommit((long)0);
	#endif /*TX*/
	
	if(i==-1)
		return(0-TuxedoErrNo[tperrno].ierrno);
	else
		return 0;
}

/*************************************
 *  ����: ����ʧ��                   *
 *  ����: ��                         * 
 *  ����ֵ��                         *
 *          =0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int cslrollback()
{
	int i;
	
	#ifdef TX
		i=tx_rollback();     
	#else
		i=tpabort((long)0);
	#endif /*TX*/
	
	if (i==-1)
		return(0-TuxedoErrNo[tperrno].ierrno);
	else
		return 0;
}

/*************************************
 *  ����: ͬ������                   *
 *  ����: svc��  ������              * 
 *        idata���������ݻ�����      *
 *        ilen�� �������ݰ�����      *
 *        odata���������ݻ�������ַ  *
 *        olen�� �������ݳ��ȵĵ�ַ  *
 *  ����ֵ��                         *
 *          =0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int cslcall(char *svc,char *idata,long ilen,char **odata,long *olen)
{
	if (tpcall(svc,idata,ilen,odata,olen,(long)0)==-1)
		return(0-TuxedoErrNo[tperrno].ierrno);
	else
		return 0;	
}

/*************************************
 *  ����: �Ự����                   *
 *  ����: svc��  ������              * 
 *        idata���������ݻ�����      *
 *        ilen�� �������ݰ�����      *
 *        flags�����Ʊ�־            *
 *  ����ֵ��                         *
 *          >0   ����������          *
 *          <0   ʧ��                *       
**************************************/
int cslconnect(char *svc,char *data,long len,long flags)
{
	int i;
	
	if ((i=tpconnect(svc,data,len,flags))==-1)
		return(0-TuxedoErrNo[tperrno].ierrno);
	else
		return i;
}

/*************************************
 *  ����: �Ự�Ͽ�                   *
 *  ����: cd��  ����������           * 
 *  ����ֵ��                         *
 *          >0   ����������          *
 *          <0   ʧ��                *       
**************************************/
int csldiscon(int cd)
{
	if (tpdiscon(cd)==-1)
		return(0-TuxedoErrNo[tperrno].ierrno);
	else
		return 0;
}

/*************************************
 *  ����: �Ự����                   *
 *  ����: cd��   ����������          * 
 *        data�� �������ݻ�����      *
 *        len��  �������ݰ�����      *
 *        flags�����Ʊ�־            *
 *  ����ֵ��                         *
 *          >0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int cslsend(int cd,char *data,long len,long flags)
{
	int i;
	long test;
	
	if ((i=tpsend(cd,data,len,flags,&test))==-1)
	{
		if (tperrno==TPEEVENT)
		{
			switch ((int)test)
			{
				case TPEV_DISCONIMM: 
					return (0-TuxedoErrNo[23].ierrno);
				case TPEV_SVCERR :
					return (0-TuxedoErrNo[24].ierrno);
				case TPEV_SVCFAIL:
					return (0-TuxedoErrNo[25].ierrno); 
			}
		}
		else
			return(0-TuxedoErrNo[tperrno].ierrno);
	}
	else
		return i;
}

/*************************************
 *  ����: �Ự����                   *
 *  ����: cd��   ����������          * 
 *        data�� �������ݻ�������ַ  *
 *        len��  �������ݳ��ȵ�ַ    *
 *        flags�����Ʊ�־            *
 *  ����ֵ��                         *
 *          >0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int cslrecv(int cd,char **data,long *len,long flags)
{
	int i;
	long test;
	
	if ((i=tprecv(cd,data,len,flags,&test))==-1)
	{
		if (tperrno==TPEEVENT)
		{
			switch((int)test)
			{
				case TPEV_DISCONIMM: 
					return (0-TuxedoErrNo[23].ierrno);
				case TPEV_SENDONLY:
					return (0-TuxedoErrNo[22].ierrno);
				case TPEV_SVCERR:
					return (0-TuxedoErrNo[24].ierrno);
				case TPEV_SVCFAIL:
					return (0-TuxedoErrNo[25].ierrno);
				case TPEV_SVCSUCC:
					return (0-TuxedoErrNo[26].ierrno);
			}
		}
		else
			return(0-TuxedoErrNo[tperrno].ierrno);
	}
	else
		return i;
}

/*************************************
 *  ����: ���仺����                 *
 *  ����: type�� ����������          * 
 *        size�� ����������          *
 *  ����ֵ��                         *
 *          <>NULL   ��������ַ      *
 *          =NULL   ʧ��             *       
**************************************/
char * cslalloc(char *type ,long size)
{
	return(tpalloc(type,NULL,size));	
}



/*************************************
 *  ����: �ͷŻ�����                 *
 *  ����: ptr�� ��������ַ           * 
 *  ����ֵ��                         *
 *          ��                       *
**************************************/
void  cslfree(char *ptr)
{
	tpfree(ptr);	
}


/*************************************
 *  ����: ȡ������Ϣ                 *
 *  ����: ierrno�� �����             * 
 *  ����ֵ��                         *
 *          <>NULL   ������Ϣ        *
 *          =NULL   ʧ��             *       
**************************************/
char * cslerrstr(int ierrno)
{
	int i;
	
	for(i=0;i<27;i++)
		if(TuxedoErrNo[i].ierrno==ierrno)
			return (TuxedoErrNo[i].errstr);
	
	return NULL;
}

}
