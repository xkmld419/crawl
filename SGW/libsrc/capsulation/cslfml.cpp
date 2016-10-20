/*���Ƕ�TUXEDO��FML�ķ�װ����,������TUXEDO����ʹ��*/
/*************************************
 *  Author: Linkage                  *
 *  Create Date:     2005/04/04      * 
 *  The last Modify: 2005/04/04      *
**************************************/
/*#define TUXEDO*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "./cslfml.h"
#include "userlog.h"

/* ��������б� */
MCCI_TUXEDO_FML_ERROR FMLErrNo[27] = {
   	                           {10500,   "FMINVAL"  },
                                   {10501,   "FALIGNERR"},
                                   {10502,   "FNOTFLD"  },
                                   {10503,   "FNOSPACE" },
                                   {10504,   "FNOTPRES" },
                                   {10505,   "FBADFLD"  },
                                   {10506,   "FTYPERR"  },
                                   {10507,   "FEUNIX"   },
                                   {10508,   "FBADNAME" },
                                   {10509,   "FMALLOC"  },
                                   {10510,   "FSYNTAX"  },
                                   {10511,   "FFTOPEN"  },
                                   {10512,   "FFTSYNTAX"},
                                   {10513,   "FEINVAL"  },
                                   {10514,   "FBADTBL"  },
                                   {10515,   "FBADVIEW" },
                                   {10516,   "FVFSYNTAX"},
                                   {10517,   "FVFOPEN"  },
                                   {10518,   "FBADACM"  },
                                   {10519,   "FNOCNAME" },
                                   {10520,   "FMAXVAL"  },
                                   {10521,   "INVALID_EX_METHOD"},
                                   {10522,   "INVALID_EX_VALUES"},
                                   {10523,   "EX_ERROR"         },
                                   {10524,   "��ֵ�͵�����ȡ����ֵ"},
                                   {10525,	 "open file error"},
                                   {10526,	 "error type"}
                       };
                       
/*����ռ���չ����*/
struct _ex_policy_
{
	int ex_method;
	int ex_values;
}sg_ex_policy={PERCENT,0};

/*************************************
 *  ����: ���ÿռ���չ����           *
 *  ����: ex_method�� ��չ��ʽ       * 
 *        ex_values�� ��չֵ         *
 *        value1��    ����           *
 *        value2��    ����           *
 *        value3��    ����           *
 *  ����ֵ��                         *
 *          >0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int cslfexpolicy(int ex_method,int ex_values,int value1,int value2,int value3)
{
	if ((ex_method!=0)&&(ex_method!=1))
		return (0-FMLErrNo[21].ierrno);
	
	if (ex_values<0)
		return (0-FMLErrNo[22].ierrno);
	
	sg_ex_policy.ex_method=ex_method;
	sg_ex_policy.ex_values=ex_values;
	
	return 0;	
}

/*************************************
 *  ����: ����ָ����ļ�¼��         *
 *  ����: fbfr��    ������           * 
 *        fieldid�� ���ʶ           *
 *  ����ֵ��                         *
 *          >0   ��¼��              *
 *          <0   ʧ��                *       
**************************************/
/*����ָ����ļ�¼��*/
int cslfnum(CSLFBFR *fbfr, CSLFLDID fieldid)
{
	int i;
	
	if ((i=(int)Foccur32(fbfr,fieldid))==-1)
		return (0-FMLErrNo[Ferror32].ierrno);
	else
		return i;
}


/*************************************
 *  ����: ����ֵ���뻺����           *
 *  ����: fbfr��      ��������ַ     * 
 *        fieldid��   ���ʶ         *
 *        oc��        �±�           *
 *        value��     ���ݰ�         *
 *  ����ֵ��                         *
 *          =0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int cslfput(CSLFBFR **fbfr, CSLFLDID fieldid, CSLFLDOCC oc, void *value)
{
	int	iRet;
	long	lLen, lUnused, lSize;
	CSLFBFR	*TempBfr,*TempBuf;
	char	sLogStr[1000];
	
	TempBfr=*fbfr;
	lLen=Flen32(TempBfr,fieldid,oc-1)+50;
	lUnused=Funused32(TempBfr);
	if (lLen>(lUnused-10))
	{
		if (sg_ex_policy.ex_values==0)
			lSize = Fsizeof32(TempBfr)*20/100+Fsizeof32(TempBfr);
		else
			if((sg_ex_policy.ex_method == 0) && (sg_ex_policy.ex_values<=lLen))
				lSize = Fsizeof32(TempBfr)+1*lLen;
			else
				if((sg_ex_policy.ex_method==0) && (sg_ex_policy.ex_values>lLen))
					lSize = Fsizeof32(TempBfr)+sg_ex_policy.ex_values;
				else
					if(sg_ex_policy.ex_method==1)
						lSize = (Fsizeof32(TempBfr)*(sg_ex_policy.ex_values)/100)  +Fsizeof32(TempBfr);
		
		TempBuf = (CSLFBFR *)tprealloc((char *)TempBfr,lSize);
		userlog("unused:%ld,size:%ld",lUnused,lSize);
		
		if (TempBuf==NULL)
		{
			userlog("ext_error:tprealloc return NULL!");
			return (0-FMLErrNo[23].ierrno);
		}
		
		if (Fsizeof32(TempBuf)<lSize)
		{
			userlog("ext_error:lSize is:%ld,Fsizeof32 is:%ld",lSize,Fsizeof32(TempBuf));
			return (0-FMLErrNo[23].ierrno);
		}
		
		*fbfr=TempBuf;
		TempBfr=*fbfr;
	}
	
	if(Fchg32(TempBfr,fieldid,oc,(char*)value,(FLDLEN32)10)==-1)
		return(0-FMLErrNo[Ferror32].ierrno);
	else
		return 0;	
}

/*************************************
 *  ����: �ӻ�������ȡ���ַ��������� *
 *  ����: fbfr��      ��������ַ     * 
 *        fieldid��   ���ʶ         *
 *        oc��        �±�           *
 *        value��     ���ݰ���ַ     *
 *  ����ֵ��                         *
 *          =0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int cslfgets(CSLFBFR *fbfr, CSLFLDID fieldid, CSLFLDOCC oc, char *buf)
{
	int iRet;
	char sLogStr[1000];
	
	if (Fgets32(fbfr,fieldid,oc,buf)==-1)
		return (0-FMLErrNo[Ferror32].ierrno);
	else
	{
		return 0;
	}
}

/*************************************
 *  ����: �ӻ�������ȡ����������     *
 *  ����: fbfr��      ��������ַ     * 
 *        fieldid��   ���ʶ         *
 *        oc��        �±�           *
 *        buf��       ���ݰ���ַ     *
 *  ����ֵ��                         *
 *          =0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int cslfgetl(CSLFBFR *fbfr, CSLFLDID fieldid, CSLFLDOCC oc, long *buf)
{
	int	iRet;
	char	temp[20];
	char	sLogStr[1000];
	
	if (Fgets32(fbfr,fieldid,oc,temp)==-1)
	{
		return(0-FMLErrNo[Ferror32].ierrno);
	}
	
	if (strcmp(temp,"")==0)
		return(-10524);
	
	*(buf)= atol(temp);
	
	return 0;
}

/*************************************
 *  ����: ȡ������Ϣ                 *
 *  ����: ierrno�� �����            * 
 *  ����ֵ��                         *
 *          <>NULL   ������Ϣ        *
 *          =NULL    ʧ��            *       
**************************************/
char * cslferrstr(int ierrno)
{
	int i;
	
	for(i=0;i<24;i++)
		if(FMLErrNo[i].ierrno==ierrno)
			return (FMLErrNo[i].errstr);
	return NULL;
}

/*************************************
 *  ����: ��¼��־					 *
 *  ����: 							 * 
 *        str	����		         *
 *  ����ֵ��                         *
 *          =0   �ɹ�                *
 *          <0   ʧ��                *       
**************************************/
int csllog(char *str)
{
	struct	tm *time_now;
	time_t	secs_now;
	static char nowstr[80];
	static char logfilename[101];
	
	FILE *log;
	
	tzset();
	time(&secs_now);
	time_now = localtime(&secs_now);
	strftime(nowstr, 80,"%Y-%m-%d %H:%M:%S",time_now);
	strftime(logfilename, 101, "PAY%Y%m%d%H.LOG", time_now);
	
	if ((log = fopen(logfilename, "at"))== NULL)
		return(-10525);
	
	fprintf(log, "PID:%d;TIME:%s;LOG:%s.\n",getpid(), nowstr, str);
	
	fclose(log);
	
	return 0;
}
