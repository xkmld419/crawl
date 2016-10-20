/*���Ƕ�TUXEDO��TONGEASY����API�ķ�װ����,������TUXEDO�����·ǿͻ��˳���ʹ��*/
/*************************************
 *  Author: Many                     *
 *  Create Date:     2000/11/29     * 
 *  The last Modify: 2000/11/29      *
**************************************/
/*
#define TUXEDO
#define COMMON
*/

#include "./mcci.h"

extern "C"
{

/*************************************
 *  ����: ����ת��                   *
 *  ����:   svc�� ת���ķ�����       * 
 *          data�� ���͵����ݰ�      *
 *          len��  ���͵����ݰ�����  *
 *  ����ֵ��                         *
 *          ��                       *
**************************************/
int cslsvrfwd(char *svc,char *data,long len)
{
	tpforward(svc,data,len,(long)0);
	return 0;
}

/*************************************
 *  ����: ͬ��Ӧ��                   *
 *  ����:   rval�� ��������־      * 
 *          rcode�����ش���          *
 *          data�� ���ص����ݰ�      *
 *          len��  ���ص����ݰ�����  *
 *  ����ֵ��                         *
 *          ��                       *
**************************************/
int cslreturn_common(int rval,long rcode,char *data,long len)
{
	tpreturn(rval,rcode,data,len,(long)0);
	return 0;	
}

int cslopen_common()
{
	if(tpopen()==-1)
	{
		return -1;
	}
	
	return 0;
}

int cslclose_common()
{
	if (tpclose()==-1)
	{
		return -1;
	}
	
	return 0;
}

}
