#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

typedef char TKeys[16][48];
typedef char A8[9];
typedef char A56[56];
typedef char A64[64];
typedef char A48[48];
typedef char A32[32];
typedef char A16[17];

extern "C"
{
/********��ȡ�ҿո�*******/
static char * TrimRight(char *s)
{
	int l;

	char * rValue;

	l = strlen(s);

	while ( (l>=0) && (s[l]<=' ') )
	{
		l--;
	}
	rValue = (char *)malloc(strlen(s));

	strncpy(rValue,s,l+1);
	rValue[l+1]='\0';
	return rValue;
}
/* IP�û�
 IP�����û�*/

/*******************************************************************
***************************����Կ����******************************
*******************************************************************/
/* ��8���ַ�����Կת����Ϊ64bit����*/
void InitKeys(A8 In, A64 *Out)
{
  int i, j, CurByte,k;

  for (i = 0; i < 8; i++)
    {
    CurByte = 0x80;
    for (j = 0; j < 8; j++)
      {
      if (CurByte & In[i])
        (*Out)[i * 8 + j] = 1;
      else
        (*Out)[i * 8 + j] = 0;
      CurByte = CurByte >> 1;
      }
    }

}

/* PC-1 �û�, ����64bit,���56bit*/
char PC_1[56] = {57, 49, 41, 33, 25, 17,  9,
                  1, 58, 50, 42, 34, 26, 18,
                 10,  2, 59, 51, 43, 35, 27,
                 19, 11,  3, 60, 52, 44, 36,
                 63, 55, 47, 39, 31, 23, 15,
                  7, 62, 54, 46, 38, 30, 22,
                 14,  6, 61, 53, 45, 37, 29,
                 21, 13,  5, 28, 20, 12,  4};
void DoPC_1(A64 In, A56 *Out)
{
  int i;

  for (i = 0; i < 56; i++)
    (*Out)[i] = In[PC_1[i] - 1];
}


/* PC-2 �û�,����56bit,���48bit*/
char PC_2[48] = {14, 17, 11, 24,  1,  5,
                  3, 28, 15,  6, 21, 10,
                 23, 19, 12,  4, 26,  8,
                 16,  7, 27, 20, 13,  2,
                 41, 52, 31, 37, 47, 55,
                 30, 40, 51, 45, 33, 48,
                 44, 49, 39, 56, 34, 53,
                 46, 42, 50, 36, 29, 32};
void DoPC_2(A56 In, TKeys *Out, int Index)
{
  int i;

  for (i = 0; i < 48; i++)
    (*Out)[Index][i] = In[PC_2[i] - 1];
}


char LS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

/* ��������Կ*/
void GenKeys(char In[], TKeys *Out)
{
  int i, j;
  A64 Temp64;
  A56 Temp56;
  int SaveC00, SaveC01, SaveD00, SaveD01;

  InitKeys(In, &Temp64);
  DoPC_1(Temp64, &Temp56);
  for (i = 0; i < 16; i++)
    {
    SaveC00 = Temp56[0];
    SaveC01 = Temp56[1];
    SaveD00 = Temp56[28];
    SaveD01 = Temp56[29];
    for (j = 0; j < 28 - LS[i]; j ++)
      {
      Temp56[j] = Temp56[j + LS[i]];
      Temp56[j + 28] = Temp56[j + 28 + LS[i]];
      }
    if (LS[i] == 1)
      {
      Temp56[27] = SaveC00;
      Temp56[55] = SaveD00;
      }
    else
      {
      Temp56[26] = SaveC00;
      Temp56[27] = SaveC01;
      Temp56[54] = SaveD00;
      Temp56[55] = SaveD01;
      }
    DoPC_2(Temp56, Out, i);
    }
}
/********************************************************************
 ����Կ�������
*******************************************************************

*******************************************************************
 �ӽ���8�ַ�,64bit
*******************************************************************/

/* ��8���ַ�ת����Ϊ64λ����*/
void SeparateBits(A8 In, A64 *Out)
{
  int i, j, CurByte;

  for (i = 0; i < 8; i++)
    {
    CurByte = 0x80;
    for (j = 0; j < 8; j++)
      {
      if (CurByte & In[i])
        (*Out)[i * 8 + j] = 1;
      else
        (*Out)[i * 8 + j] = 0;
      CurByte = CurByte >> 1;
      }
    }
}

/* IP�û�*/
char IP[64] = {58, 50, 42, 34, 26, 18, 10,  2,
               60, 52, 44, 36, 28, 20, 12,  4,
               62, 54, 46, 38, 30, 22, 14,  6,
               64, 56, 48, 40, 32, 24, 16,  8,
               57, 49, 41, 33, 25, 17,  9,  1,
               59, 51, 43, 35, 27, 19, 11,  3,
               61, 53, 45, 37, 29, 21, 13,  5,
               63, 55, 47, 39, 31, 23, 15,  7};
void DoIP(A64 In, A64 *Out)
{
  int i;

  for (i = 0; i < 64; i++)
    (*Out)[i] = In[IP[i] - 1];
}

/* E�û�,��L�е�32λ����Ϊ48λ*/
char E[48] = {32,  1,  2,  3,  4,  5,
               4,  5,  6,  7,  8,  9,
               8,  9, 10, 11, 12, 13,
              12, 13, 14, 15, 16, 17,
              16, 17, 18, 19, 20, 21,
              20, 21, 22, 23, 24, 25,
              24, 25, 26, 27, 28, 29,
              28, 29, 30, 31, 32,  1};
void DoE(A32 In, A48 *Out)
{
  int i;

  for (i = 0; i < 48; i++)
    (*Out)[i] = In[E[i] - 1];
}

/* SBOX 6λ����ת����Ϊ4λ���*/
char SBOXs[8][4][16] = {{{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
                         { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
                         { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
                         {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}},
                        {{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
                         { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
                         { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
                         {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}},
                        {{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
                         {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
                         {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
                         { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}},
                        {{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
                         {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
                         {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
                         { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}},
                        {{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
                         {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
                         { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
                         {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}},
                        {{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
                         {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
                         { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
                         { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}},
                        {{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
                         {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
                         { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
                         { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}},
                        {{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
                         { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
                         { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
                         { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}}};
void DoSBOX(int Index, unsigned int I1, unsigned int I2, unsigned int I3, unsigned int I4,
                       unsigned int I5, unsigned int I6,
                       unsigned int *O1, unsigned int *O2, unsigned int *O3, unsigned int *O4)
{
  int Result;

  Result = SBOXs[Index][I1 * 2 + I6][I2 * 8 + I3 * 4 + I4 * 2 + I5];
  *O1 = (Result & 0x8 ? 1 : 0);
  *O2 = (Result & 0x4 ? 1 : 0);
  *O3 = (Result & 0x2 ? 1 : 0);
  *O4 = (Result & 0x1 ? 1 : 0);
}


/* P�û�, �û�32λ*/
char P32[32] = {16,  7, 20, 21,
              29, 12, 28, 17,
               1, 15, 23, 26,
               5, 18, 31, 10,
               2,  8, 24, 14,
              32, 27,  3,  9,
              19, 13, 30,  6,
              22, 11,  4, 25};
void DoP(A48 In, A32 *Out)
{
  int i;

  for (i = 0; i < 32; i++)
    (*Out)[i] = In[P32[i] - 1];
}

/* ����*/
void DoF(A32 R_1, A32 *R, TKeys Key, int Step)
{
  A48 Temp48;
  unsigned int i, O1, O2, O3, O4;

  DoE(R_1, &Temp48);
  for (i = 0; i < 48; i ++)
    Temp48[i] = Temp48[i] ^ Key[Step][i];
  for (i = 0; i < 8; i ++)
    {
    DoSBOX(i, (unsigned int)(Temp48[i * 6 + 0]),
              (unsigned int)(Temp48[i * 6 + 1]),
              (unsigned int)(Temp48[i * 6 + 2]),
              (unsigned int)(Temp48[i * 6 + 3]),
              (unsigned int)(Temp48[i * 6 + 4]),
              (unsigned int)(Temp48[i * 6 + 5]),
              &O1, &O2, &O3, &O4);
    Temp48[i * 4] = O1;
    Temp48[i * 4 + 1] = O2;
    Temp48[i * 4 + 2] = O3;
    Temp48[i * 4 + 3] = O4;
    }
  DoP(Temp48, R);
}

/* IP_1�û�*/
char IP_1[64] = {40,  8, 48, 16, 56, 24, 64, 32,
                 39,  7, 47, 15, 55, 23, 63, 31,
                 38,  6, 46, 14, 54, 22, 62, 30,
                 37,  5, 45, 13, 53, 21, 61, 29,
                 36,  4, 44, 12, 52, 20, 60, 28,
                 35,  3, 43, 11, 51, 19, 59, 27,
                 34,  2, 42, 10, 50, 18, 58, 26,
                 33,  1, 41,  9, 49, 17, 57, 25};
void DoIP_1(A64 In, A64 *Out)
{
  int i;

  for (i = 0; i < 64; i++)
    (*Out)[i] = In[IP_1[i] - 1];
}


/* ��64λ��ϳ�Ϊ8�ֽ�*/
void CombineBits(A64 In, A8 *Out)
{
  int i, j, CurByte,h;

  for (i = 0; i < 8; i++)
    {
    CurByte = 0;
    for (j = 0; j < 8; j++)
      {
      CurByte = CurByte << 1;
      if (In[i * 8 + j])
        CurByte = CurByte + 1;
      }
    (*Out)[i] = CurByte;
    }
}

/*16�ֽ�ת����64λ*/
void SeparateBits16(A16 In, A64 *Out)
{
 int i, j, CurByte,k,iTemp;

 for (i = 0; i < 16; i++)
 {
    iTemp=In[i];
    CurByte = 8;
    iTemp=iTemp & 60;
    iTemp=iTemp>>2;
    for(j=0; j<4; j++)
    {
      if (CurByte & iTemp)
         (*Out)[i * 4 + j] = 1;
      else
         (*Out)[i * 4 + j] = 0;
      CurByte = CurByte >> 1;
    }
  }

}

/*��64λ��ϳ�16�ֽ�*/
void CombineBits16(A64 In, A16 *Out)
{  int i, j, h, CurByte,iTemp;

  for (i = 0; i < 16; i++)
    {
    CurByte = 64;
    for (j = 0; j < 4; j++)
      {
      /*CurByte = CurByte << 1;*/
      if (In[i * 4 + j])
        {  iTemp=2;
           for(h=0; h<=3-j ; h++)
             iTemp=iTemp*2;
           CurByte = CurByte + iTemp;
        }
      }
    (*Out)[i] = CurByte;
    }

}
/*********************************************************************
**********************��8���ӽڽ��м��ܻ����************************
**************InOut���뼰���(����Ϊ8�ֽڣ����Ҳ��8�ֽ�)************
************************** CharKey   ����****************************
***********************Encrypt   = 1  ����  =0 ����******************
****Ҳ����ֱ��������������ӽ��ܣ�����CombineBits��64λ����->8�ֽ�)**
**������64λ���ݽ��д����ʱ�������һ��00000000���ݣ����������***
****����������㡣�����ṩ�������������encodepass(),decodepass()****
***��8�ֽڽ��м��ܺ����16�ֽڣ������;��Բ������������˵�����*****
***���ԣ������Ƽ���Ҫ�ô˺�������encodepass()��decodepass()���ܱ�֤**
***************************�ӽ��ܵ���ȷ��****************************
*********************************************************************/

void DoDES8(A8 *InOut, A8 CharKey, int Encrypt)
{
  A64 Temp64, Temp64_2;
  A32 L_1, R_1, L, R;
  int i, j;
  TKeys Key;

  GenKeys(CharKey, &Key);         /* ����16������Կ*/
  SeparateBits(*InOut, &Temp64);  /* �������8���ֽڸ�λȫ��ȡ��,�����Temp64��*/
  DoIP(Temp64, &Temp64_2);        /* IP�û�, Temp64->Temp64_2*/
  for (i = 0; i < 32; i++)        /* L, R��ʼֵΪTemp64_2�ĸ�32λ�͵�32λ*/
    {
    L_1[i] = Temp64_2[i];
    R_1[i] = Temp64_2[i + 32];
    }
  for (i = 0; i < 16; i++)        /* 16�ε���*/
    {
    if (Encrypt)
      DoF(R_1, &R, Key, i);       /* F�任*/
    else
      DoF(R_1, &R, Key, 15 - i);  /* F�任*/
    for (j = 0; j < 32; j++)
      {
      R[j] = R[j] ^ L_1[j];
      L[j] = R_1[j];
      L_1[j] = L[j];
      R_1[j] = R[j];
      }
    }
  for (i = 0; i < 32; i++)
    {
    Temp64[i] = R[i];
    Temp64[i + 32] = L[i];
    }
  DoIP_1(Temp64, &Temp64_2);      /* IP-1�û�*/
  CombineBits(Temp64_2, InOut);   /* ��64λ�������������λ8�ֽ�*/
}

/**********************************************************************
*   ��8�ֽڽ��м��ܣ������16���ֽڣ���֤�˼��ܺ������Ϊ�ɼ��ַ�     *
*    In ���루8�ֽڣ� CharKey �ܳף�����Ϊ8�ֽڣ�Out ��� (16�ֽ�)    *
*�Ƽ�ʹ�����ּ��ܣ���Ϊ����������˵DoDES8���ܺ��ڴ�64bit->8�ֽڵ�   *
*ת���У���ȫ�п��ܵõ�һ��00000000����ô��ת����8�ֽ������ʱ�����  *
*������                                                               *
***********************************************************************/
void DoEncDES16(A8 *In, A8 CharKey,A16 *Out)
/*void DoEncDES16(A8 In,A8 CharKey, A16 Out)*/
{
  A64 Temp64, Temp64_2,Temp64_3;
  A32 L_1, R_1, L, R;
  A16 Temp16;
  int i, j;
  TKeys Key;

  GenKeys(CharKey, &Key);       /* ����16������Կ*/
  SeparateBits(*In, &Temp64);   /* �������8���ֽڸ�λȫ��ȡ��,�����Temp64��*/
  /*SeparateBits(In, &Temp64);*/
  DoIP(Temp64, &Temp64_2);      /* IP�û�, Temp64->Temp64_2*/
  for (i = 0; i < 32; i++)      /* L, R��ʼֵΪTemp64_2�ĸ�32λ�͵�32λ*/
    {
    L_1[i] = Temp64_2[i];
    R_1[i] = Temp64_2[i + 32];
    }
  for (i = 0; i < 16; i++)      /* 16�ε���*/
    {
      DoF(R_1, &R, Key, i);       /* F�任*/
    for (j = 0; j < 32; j++)
      {
      R[j] = R[j] ^ L_1[j];
      L[j] = R_1[j];
      L_1[j] = L[j];
      R_1[j] = R[j];
      }
    }
  for (i = 0; i < 32; i++)
    {
    Temp64[i] = R[i];
    Temp64[i + 32] = L[i];
    }
  DoIP_1(Temp64, &Temp64_2);     /* IP-1�û�*/
  CombineBits16(Temp64_2,Out);   /*��64λ�������������λ16�ֽ�*/
}

/***********************************************************
****************��16�ֽڵ����Ľ��н���**********************
***In ���루16�ֽڣ�  CharKey �ܳף�8�ֽڣ� Out(8�ֽڣ�*****
************************************************************/
void DoDecDES16(A16 *In, A8 CharKey,A8 *Out)
/*void DoDecDES16(A16 In, A8 CharKey,A8 Out)*/
{
  A64 Temp64, Temp64_2,Temp64_3;
  A32 L_1, R_1, L, R;
  A16 Temp16;
  int i, j;
  TKeys Key;

  GenKeys(CharKey, &Key);         /* ����16������Կ*/
  SeparateBits16(*In, &Temp64);   /* �������16���ֽڸ�λȫ��ȡ��,�����Temp64��*/
  /*SeparateBits16(In, &Temp64); */
  DoIP(Temp64, &Temp64_2);        /* IP�û�, Temp64->Temp64_2*/
  for (i = 0; i < 32; i++)        /* L, R��ʼֵΪTemp64_2�ĸ�32λ�͵�32λ*/
    {
    L_1[i] = Temp64_2[i];
    R_1[i] = Temp64_2[i + 32];
    }
  for (i = 0; i < 16; i++)        /* 16�ε���*/
    {
      DoF(R_1, &R, Key, 15-i);    /* F�任*/
    for (j = 0; j < 32; j++)
      {
      R[j] = R[j] ^ L_1[j];
      L[j] = R_1[j];
      L_1[j] = L[j];
      R_1[j] = R[j];
      }
    }
  for (i = 0; i < 32; i++)
    {
    Temp64[i] = R[i];
    Temp64[i + 32] = L[i];
    }
  DoIP_1(Temp64, &Temp64_2);      /* IP-1�û�*/
  CombineBits(Temp64_2, Out);   /* ��64λ�������������λ8�ֽ�*/
}

/*****************************************************
***********************���ܺ���***********************
********ÿ8���ֽ����ļ��ܺ�õ�16���ֽڵ�����*********
************����ֻҪ��������������м���**************
******************************************************/
void encode(char *inblock,char * outblock)
{
	int i,l;
	char *inblocktemp, *outblocktemp,*temp;

	char *key = "12345678";

	if ( key==NULL || inblock==NULL)
	{
		outblock=NULL;
		return;
	}

	if (strlen(inblock)==0)
	{
		outblock=NULL;
		return;
	}


	l = strlen(key);

	/* key must 8 char */
	if (l>8)
		key[8]='\0';
	if (l<8)
		for (i=8;i>l;i--)
			strcat(key," ");

	/* inblock must 8*n */
	l = strlen(inblock) % 8;

	if (l!=0)
		for (i=8;i>l;i--)
			strcat(inblock," ");

	inblocktemp = (char *) malloc(9);
	outblocktemp = (char *) malloc(17);
        /*temp=(char*) malloc(9);  */
	l = strlen(inblock);

	bzero(outblock,strlen(outblock));


	for (i=0;i<l;i+=8)
	{
		temp = &inblock[i];
		strncpy(inblocktemp,temp,8);
		/*endes(inblocktemp,outblocktemp);*/
                inblocktemp[8]='\0';
                DoEncDES16((A8 *)inblocktemp,key,(A16 *)outblocktemp);
                outblocktemp[16]=0;
		strcat(outblock,outblocktemp);
	}
	free(inblocktemp);
	free(outblocktemp);
	return;
}
/*********************************************************
**************************���ܺ���************************
**************ÿ����16���ֽ����ĵõ�8�ֽ�����*************
***************����ֻҪ��������������ܼ���***************
**********************************************************/
void decodepass(char *key,char *inblock,char * outblock)
{
	int i,l;
	char *inblocktemp, *outblocktemp,*temp;

	if ( key==NULL || inblock==NULL)
	{
		strcpy(outblock, "");
		return;
	}

	if (strlen(inblock)==0)
	{
		strcpy(outblock, "");
		return;
	}


	l = strlen(key);

	/* key must 8 char */
	if (l>8)
		key[8]='\0';
	if (l<8)
		for (i=8;i>l;i--)
			strcat(key," ");

	/* inblock must 16*n */
	l = strlen(inblock) % 16;

	if (l!=0)
		for (i=16;i>l;i--)
			strcat(inblock," ");

	inblocktemp = (char *) malloc(17);
	outblocktemp = (char *) malloc(9);

	l = strlen(inblock);

	bzero(outblock,strlen(outblock));


	for (i=0;i<l;i+=16)
	{
		temp = &inblock[i];
		strncpy(inblocktemp,temp,16);
		DoDecDES16((A16 *)inblocktemp, key, (A8 *)outblocktemp);
                outblocktemp[8]=0;

		if (i==l-8)
		outblocktemp = TrimRight(outblocktemp);
		strcat(outblock,outblocktemp);
	}
	free(inblocktemp);
	free(outblocktemp);
}

void decode(char *inblock,char * outblock)
{
        char Key[9];
        strcpy(Key, "12345678");
        decodepass(Key, inblock, outblock);
}

}   //extern
