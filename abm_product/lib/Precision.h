/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef PRECISION_H_HEADER_INCLUDED_BDAA833B
#define PRECISION_H_HEADER_INCLUDED_BDAA833B

#include <stdlib.h>
#include <stdio.h>

//##ModelId=4255060C001A
class Precision
{
  public:
    //##ModelId=425506480319
    //0: ����; >0 ����; <0 ����
    //iMode: 0��������; 1:����ȡ��; 2:����ȡ��
    static int set(int *iValue, int iRatePrecision, int iCalPrecision, 
        int iMode=0);

    static int set(long *lValue, int iRatePrecision, int iCalPrecision, 
        int iMode=0);
};



#endif /* PRECISION_H_HEADER_INCLUDED_BDAA833B */
