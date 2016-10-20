/*VER: 1*/ 
/************************************************
  Copyright by Wholewise  2002
  File Name: TScriptParser.h
  Created Date: 2002.08.01
  Author: xiaozc
  Version: 0.0.01
  Create Version: 0.0.01
  Last Version Date: 2002.08.01
  Create Version Date: 2002.08.01
 ***********************************************/

#ifndef _TSCRIPTPARSER_H_
#define _TSCRIPTPARSER_H_
#include <string.h>
#include "wwInterpreter.h"
#include "wwLex.h"
#include "wwParser.h"
#include "exception.h"

//�ű��������ඨ��
class ScriptParser :public wwLex, public wwParser
{
private:
	
public:
	ScriptParser(){input_len =0;};
	~ScriptParser(){

		Clear();
	};
	virtual int yylex();
	virtual void yyerror(char *m);
	//virtual double SysinputHandler(const char *){};
	//virtual char * SysinputHandlerS(const char *){};
	
	void SetSource(char * buf,int size);        //�ű�Դ��������
	void Parse();                               //��������ű�
	void Run();                                 //ִ�нű�
	
	typedef  TSymbol TValuePair;
	
	 //���뺯������Ĳ���
	TValuePair   Param(char * name);
	 
	//������
	TValuePair * & Result(char * name); 
	TValuePair * Result(int index, char * name); 
	
	void SimpleInput(char * value); //���뵥���ַ�����ֵ
	void SimpleInput(double value); //���뵥����ֵ��ֵ

	void MultiInput(char * value);  //�������ַ�����ֵ
	void MultiInput(double value);  //��������ֵ��ֵ
	

	void Clear();                   //�������(����������ɵ�ָ��)
	void ClearResult();             //���ִ�н��(����ָ��)
		
	void PrintCommandFlow();        //��ӡָ����
	
};

#endif
