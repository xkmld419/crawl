// Copyright (c) 2005,联创科技股份有限公司电信事业部
// All rights reserved.

#ifndef EXCEPTION_H_HEADER_INCLUDED_BDDDB27C
#define EXCEPTION_H_HEADER_INCLUDED_BDDDB27C

#include <errno.h>

//##ModelId=421DCACA0311
class Exception
{
  public:
    //##ModelId=421DCC2F01DE
    Exception(int code, int err, int line, char *file);
    Exception(int code, int err, int line, char const *file); 
        

    //##ModelId=421DCC5E0308
    char *descript();

  private:
    //##ModelId=42226D9E00AD
    int m_iCode;

    //##ModelId=42226DA502AC
    int m_iErrNo;

    //##ModelId=42226DAC01F7
    int m_iLineNo;

    //##ModelId=42226DB20228
    char  m_sFile[200];
    //##ModelId=422270CA00F1
    char m_sBuf[1024];


};

#define THROW(x) \
		throw Exception((x), errno, __LINE__, __FILE__)

#endif /* EXCEPTION_H_HEADER_INCLUDED_BDDDB27C */
