#ifndef _PARAMACCESSOR_H
#define _PARAMACCESSOR_H
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Date.h"
#include "Log.h"

#ifndef LINEMAXCHARS
#define LINEMAXCHARS	150
#endif
using namespace std;
//����������
//ͨ��ParamAccessor::GetInstance()�õ��������ʽӿ�,ParamAccessor->GetParamValue()ȡ�ò����ľ���ֵ
//ParamAccessor->ModifyParamValue()�޸������ļ�sys.ini�Ĳ���ֵ������Ƕ��ڹ���ͬʱ����newValue��effDate��
class ParamAccessor
{
  public:
    ~ParamAccessor();

    static ParamAccessor* GetInstance(char *sFileName);
   
    bool GetParamValue(char* strParamName, char* oValue,int iMaxLen=150,int iMode=0);//��ò�����valueֵ
    bool ModifyParamValue(char* strParamName, char* oModifyValue,char *oModifyDate=0,int iMode=0);//�޸Ĳ�����valueֵ
		
		    void BeginTrans();
    void rollback();
    void commit();
    char m_sFileName[150];
    char m_sTmpFileName[150];
    char m_sBakFileName[150];
  private:
    ParamAccessor(char *sFileName);

    bool Init();

    static ParamAccessor* m_gpParamAccessor;
    
    char *TruncStr( char *sp , char *sDst , int len);
    char * trim( char *sp );
    
    bool GetFieldValue(const char* strParamName,const char* field,char *oValue,int iMaxLen=150);//��ò���ĳ�ֶε�ֵ
    bool ModifyFieldValue(char *strParamName,char * field,char * oModifyValue);//�޸Ĳ���ĳ�ֶε�ֵ
    

};



#endif
