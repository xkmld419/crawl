/***************************/
/* Create 18/11/2000.      */
/* Modify 23/11/2000 9:30  */
/***************************/

#ifndef MID_COMPONENT_COMMON_FML_INTERFACE_H
#define MID_COMPONENT_COMMON_FML_INTERFACE_H

#ifndef NULL
#define NULL 			0
#endif /* NULL */


#ifdef TUXEDO
/* Include �ļ� */
#   include <fml32.h>
#   include <atmi.h>
typedef struct _mcci_tuxedo_fml_
{
    int    ierrno;	       /* ������ */
   char   *errstr;	       /* ������Ϣ */
} MCCI_TUXEDO_FML_ERROR;
#endif /*TUXEDO*/

/* ����Ӧ�ó�����Ϣ�ṹ */
typedef  FBFR32    CSLFBFR;
typedef  FLDID32   CSLFLDID;
typedef  FLDOCC32  CSLFLDOCC;
   
/* �ռ���չ���� */
#define   FIXED     0      /*�̶�ֵ��ʽ*/
#define   PERCENT   1      /*�ٷֱȷ�ʽ*/
   
/*************************************
*    FML MANAGER FUNCTIONS DECLARE   *
**************************************/

/*
ex_method:��չ��ʽ������ȡֵ��FIXED PERCENT; ex_values����չֵ
ex_values=0ʱʹ��Ĭ�Ϸ�ʽ��������չҪ��ŵĵ�ǰ��¼�Ĵ�С��10��
*/
extern int cslfexpolicy(int ex_method,int ex_values,int value1,int value2,int value3);

extern int cslfnum(CSLFBFR *fbfr,CSLFLDID fieldid);/*fbfr:ָ��������;fieldid:���ʶ*/

/*
fbfr	ָ��������
fieldid	���ʶ
oc	�±�
len	ֵ����
value	�Ǹ�ָ�룬�������Ͳ�һ����(char *)��Ӧ�����㶨���fieldid������ƥ��
*/
extern int cslfput(CSLFBFR **fbfr, CSLFLDID fieldid, CSLFLDOCC oc, void *value);

extern int cslfgets(CSLFBFR *fbfr, CSLFLDID fieldid, CSLFLDOCC oc, char *buf);/*fbfr:ָ��������;fieldid:���ʶ;oc:�±�*/

extern int cslfgetl(CSLFBFR *fbfr, CSLFLDID fieldid, CSLFLDOCC oc, long *buf);/*fbfr:ָ��������;fieldid:���ʶ;oc:�±�*/

extern char * cslferrstr(int ierrno);/*ierrno:�����*/

#endif /*MID_COMPONENT_COMMON_FML_INTERFACE_H*/

