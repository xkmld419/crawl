/*VER: 1*/ 
#define MAX_ITEMS  300
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define  MAX_LEN   500

typedef struct TControl{
    int      iId;
    int      iTop,iLeft,iWidth,iHeight;   /* �ؼ���λ�����С */  
    char     sType[20];                       /* �ؼ������ */
    int      bFrame;                      /* 0:��Ҫ�߿� 1: Ҫ�߿� */
    char     sHotKey[10];			  /* �ؼ����ȼ�  2001.2.5 ���� */
    char     sAdditional[20];                 /* �ؼ��������ص� */
                                          /*  ��EDIT: ��ʾ�������� 
						      ��ѡ��AMOUNT,NUMERIC,NUMBER,PASSWD,STATIC,STRING
					      ��LIST: ����list�е� ITEMS ��Ŀ  
    					  */     
    int      bSingle;                     /* LIST�ؼ�ʹ�� 1:��ѡ  ����:��ѡ  */
    char     sData[MAX_ITEMS];            /* �ؼ��Ĵ��ռ� */
    char     **pData;                     /* �ؼ���ָ����ָ�� */
    char     sIndex[30];
    char     sTab[30];
    char     sUp[30];
    char     sDown[30];
    char     sLeft[30];
    char     sRight[30];
    struct TControl *pIndex;
    char     sKeyPress[30];                   /* �ؼ��İ��������¼� ��ӷ�ʽ ���緵�ص���0�����пؼ��ṩ�ļ����¼�*/
    char     sDesc[50];
} Control;

FILE *FileOpen(char *File,int ii,char flag);
void PreCreate( Control *pHandle );
void BuildJiemian(FILE *fp,Control *pCtl,char *bb);
void BuildFunction(FILE *fp,Control *pCtl,char *bb);
void BuildHeadFile(FILE *fp,Control *pCtl,char *bb);
void ReadToControl( Control *tmp1, char WorkBuf[]);
int ReadField(int ii,char WorkBuf[MAX_LEN],char buff[]);
int CheckFile(char *str);
int g_date(int day[3]);


