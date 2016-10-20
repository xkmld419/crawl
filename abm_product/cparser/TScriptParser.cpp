/*VER: 1*/ 
/************************************************
  Copyright by Wholewise  2002
  File Name: TScriptParser.cpp
  Created Date: 2002.08.01
  Author: xiaozc
  Version: 0.0.01
  Create Version: 0.0.01
  Last Version Date: 2002.08.01
  Create Version Date: 2002.08.01
 ***********************************************/

#include "wwInterpreter.h"
#include "TScriptParser.h"
#include <stdio.h>

typedef TSymbol TValuePair;
 
TValuePair  ScriptParser::Param(char * name)
{
	Symbol sym;

	if ( ! ( wwParser::GetInputParam(name, sym))){
		throw TException("ScriptParser::Param: ָ���Ĳ�����������");
	};
	
	return sym;
};
 
TValuePair * & ScriptParser::Result(char * name)
{
	return  wwParser::GetOutput(name);
}; 

TValuePair * ScriptParser::Result(int index, char * name)
{
	return  wwParser::GetOutput(index, name);
}; 


//wwParser::yylexʵ��
int ScriptParser::yylex()
{
	return wwLex::yylex(&yylval,&yylloc);
}

//wwParser::yyerrorʵ��
void ScriptParser::yyerror(char *m)
{ 

	cout<<m<<"line:"<<cur_line<<"column:"<<cur_column<<endl;
	return;
}

//�ű�Դ��������
void ScriptParser::SetSource(char * buf, int size)
{
	wwLex::SetBuf(buf,size);		
}

//��������ű�
void ScriptParser::Parse()
{
	if ( yyparse()){
	    char sErrMsg[200];
	    sprintf(sErrMsg,"�﷨����,��:%d,��:%d",cur_line,cur_column);
		throw TException(sErrMsg);
	}
}

//ִ�нű�
void ScriptParser::Run()
{
	wwParser::run();
}

/* end of add */

//���뵥���ַ�����ֵ
void ScriptParser::SimpleInput(char * value)
{
	wwParser::SetInputData(symSTRING, value);
}

//���뵥����ֵ��ֵ
void ScriptParser::SimpleInput(double value)
{
	wwParser::SetInputData(symDOUBLE, value);
}

//�������ַ�����ֵ
void ScriptParser::MultiInput(char * value)
{
	wwParser::SetInputData(symSTRING_ARRAY, value);
}

//��������ֵ��ֵ
void ScriptParser::MultiInput(double value)
{
	wwParser::SetInputData(symDOUBLE_ARRAY, value);
}

//�������(����������ɵ�ָ��)
void ScriptParser::Clear()
{
	wwParser::Clear();
}
               
//���ִ�н��(����ָ��)
void ScriptParser::ClearResult()
{
	wwParser::ClearResult();
}

//��ӡָ����	
void ScriptParser::PrintCommandFlow()
{
	wwParser::print();
}
