/*VER: 1*/ 
#ifndef _MACRO_H
#define _MACRO_H

/* ��ؼ������йصĺ궨�� */
#define AMOUNT 		1   		  /* EDIT�ؼ�����ΪС��������� */
#define IPADDR          1  		  /* EDIT�ؼ�����ΪС��������� */
#define NUMERIC		2   		  /* ������  */
//#define NUMBER          2   		  /* ������  */
#define CHAR    	3   		  /* �ַ�    */
#define PASSWD          4   		  /* Ϊ��������� */
#define STATIC          5   		  /* Ϊ��̬�༭�� ==> ���ɱ༭ */
#define sCaption        sData             /* */  
#define pItems          pData             /* */
#define iItemCount      iAdditional       /* */
#define iInputType      iAdditional       /* */
#define MAX_INPUT_LEN	100 		  /*  EDIT ������볤�� */
#define MAX_ITEMS       6300 		  /*  LIST ���items��Ŀ */
#define SELECTED        1		  /*  LIST �� item ��ѡ�� */ 
#define UNSELECTED      0		  /*  LIST �� item δ��ѡ�� */ 
#define ALL_ITEMS       9999              /*  ��ʾ���е� items ,    */

/* ��FORM�йص���Ϣ  */
#define FORM_KILL       999               /*  ����������Ϣ, ����FormKill()      */ 
#define FORM_KILL_OK    998               /*  ����������Ϣ�����Դ�����Form���ػ�*/ 
#define FORM_CLEAR      997		  /*  ����������Ϣ�����Դ����Ե�ǰForm�Ŀؼ�������� */

/* ���� */
#define HOT_KEY         888               /*  ���м����ȼ��¼�ʱ�Ĳ��� */ 

#define BASE_ID		10000		  /*  Base ID Value */
#define ID(_n_)		(BASE_ID+_n_)     /*  ID Value */
#endif




