// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef SIMPLEMATRIX_H_HEADER_INCLUDED_BDB752AF
#define SIMPLEMATRIX_H_HEADER_INCLUDED_BDB752AF

//#include "CommonMacro.h"

//##ModelId=4248C57101A4
//##Documentation
//## �򵥵ľ����࣬���������жϹ�����ϵ
class SimpleMatrix
{
  public:
    //##ModelId=4248C65A0312
    //##Documentation
    //## �鲢����
    SimpleMatrix(int iRow, int iCol, 
        //##Documentation
        //## ���к����е����������Ƿ�һ����ͨ��Ϊһ����
        bool isSame = true);

    //##ModelId=4248C69C01B8
    //##Documentation
    //## ���ù�����ϵ��
    //## ��x����������y
    void setBelong(int x, int y);

    //##ModelId=4248C79802AB
    //##Documentation
    //## ��x�Ƿ�������y
    bool getBelong(int x, int y);
    //##ModelId=4248CA9E0181

    int * getChilds (int y);

    ~SimpleMatrix();


  private:
    //##ModelId=4248CC4E0223
    void set(int x, int y);

    //##ModelId=4248CC590341
    //##ModelId=4248CC590341
    int get(int x, int y);

    //##ModelId=4248C6450308
    int m_iRow;

    //##ModelId=4248C64F0208
    int m_iCol;
    //##ModelId=4248C9B100CC
    bool m_bSame;


    //##ModelId=4248CAA6016F
    char *m_sBuf;
    //##ModelId=4248CD5F015D
    unsigned char m_cBuf[8];

    int * m_piRetBuf;

};



#endif /* SIMPLEMATRIX_H_HEADER_INCLUDED_BDB752AF */



