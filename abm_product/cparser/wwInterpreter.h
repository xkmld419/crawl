/*VER: 1*/ 
/************************************************
  Copyright by Wholewise  2002
  File Name: wwInterpreter.h
  Created Date: 2002.08.01
  Author: xiaozc
  Version: 0.0.01
  Create Version: 0.0.01
  Last Version Date: 2002.08.01
  Create Version Date: 2002.08.01
  Add: ruanyj 2005.6.13
	GetOutput (int index, char *name);
	Result (int index, char *name);
 ***********************************************/
 
#ifndef __WWINTERPRETER_H_
#define __WWINTERPRETER_H_

#include <string.h>
#include <stdio.h>
#include <list>
#include <stack>
#include <string>
#include "exception.h"
using namespace std;

#ifndef STR_MAX
#define STR_MAX 255
#endif

class Interpreter;

//�ڲ��������Ͷ���
enum cmd_type { // ָ������ 
	cFIRST_COMMAND, /* no command, just marks start of list*/ 
	
	cASSIGN, // ��ֵ
	cPOP,    // ��ջ
	cJUMP,   // ��ת
	cPOS,    // ����
	cNEG,    // ����
	cNOT,    // ��
	cINC,    // ++
	cDEC,    // --
	cPINC,   // ��++
	cPDEC,   // ��--
	
	cLT,     // ���� >
	cGT,     // С�� <
	cLE,     // С�ڵ��� <=
	cGE,     // ���ڵ��� >=
	cEQ,     // ���� == 
	cNE,     // ������ !=
	cAND,    // �� &&
	cOR,     // �� ||
	
	cLABEL,  //���
	cDECIDE, //�����ж�

	
	 /* double operations */
	cDBLADD, // �� + 
	cDBLMIN, // �� -
	cDBLMUL, // �� *
	cDBLDIV, // �� /
	cDBLMOD, // ģ %
	
	cPUSHSYM, //����ѹջ
	cPOPSYM,  //���ų�ջ
	cPUSHDBL,    //��ֵѹջ
	
	/* string operations */
	cPUSHSTRSYM,
	cPOPSTRSYM,
	cPUSHSTR,
	cCONCAT,           
	cPUSHSTRPTR,
	cCHANGESTRING,
	cTOKEN,

	
	cDEFSYMBOL,  //�������
	cDEFSYMBOL_ARRAY, //�����������
	cARRAYREF,   //��������
	cOUTPUT,     //���
	cINPUT,      //����
	cINPUTPARAM, //�����������
	cPREPAREINPUT, //׼������
	cSYSINPUT,   //����ϵͳ����
	cSYSINPUTS,  //����ϵͳ����(�ַ�����)
			
	cLAST_COMMAND /* no command, just marks end of list */
};

enum stackentries { /* different types of stackentries */
  stSTRING,stNUMBER,stLABEL,stSYMBOL,stARRAYMEMBER, stINPUTPARAM
};

// ָ��Ľṹ����
typedef struct  Command {  
  int     type;              // ָ������
  char  * str_data;     // ָ����ض�����
  double  dbl_data;
  int    args;  // �����Ĳ�������
  char   tag;   // �������
  int    line;  // ָ���Ӧ���к�
}TCommand;

//���ŵ�����
enum symboltype {
	symDOUBLE, symDOUBLE_ARRAY,   /*��ֵ*/ 
	symSTRING, symSTRING_ARRAY   /*�ַ���*/
};

typedef struct DblList {
	double dbl_value;
	struct DblList * pNext;
}TDblList;

typedef struct StrList {
	char   str[STR_MAX];
	struct StrList * pNext;
}TStrList;


//���Ŷ���
typedef class  Symbol {
       
public:
	int     type;            //��������
	char    name[STR_MAX];      //��������
	double  dbl_data;       //��ֵ
	char    str_data[STR_MAX];       //�ַ���ֵ
	
	TDblList * dbl_list;  //��ֵ����
	TStrList * str_list;  //�ַ�������
	
	//ȡ��ֵ
	double get_dbl (int array_ind);
	
	//ȡ�ַ���
	char * get_str (int array_ind);
	
	//ȡ��ֵ����
	TDblList * get_dblarray();
	
	//ȡ�ַ�������
	TStrList * get_strarray();
	
	//��ֵ���鸳ֵ
	void set_array_member( int array_ind, double dbl);
	
	//�ַ�����ֵ��ֵ
	void set_array_member( int array_ind, char * str);
	
	//��ֵ��ֵ
	void set_value(int array_ind,double value);
	
	//�ַ�����ֵ
	void set_value(int array_ind,char * value);
	
	//ת��Ϊ�ַ�������ֵ
	const char * asString();
	//ת��Ϊ˫������ֵ����
	double asDouble();
	
	//ת��Ϊ����ֵ����
	long    asInteger();
	
	const char * getName();

	Symbol();
	~Symbol();
	Symbol(const Symbol & Ori);
}TSymbol;

//��ջ�Ľڵ�ṹ
typedef class Stackentry {  
public:
	int        type;             //�ڵ�����
	TSymbol *  symbol_ptr;      //ָ�������ָ��
	double dbl_data;            //��ֵ
	char   str_data[STR_MAX];
	
	//ȡ����ֵ
	double get_dbl();
	
	//ȡ���ַ���
	char * get_str();
	
	//ȡ����ֵ����
	TDblList * get_dblarray();
	
	//ȡ���ַ�������
	TStrList * get_strarray();
	
	//����ֵ
	void set_value(double value);
	
	//���ַ���
	void set_value(char * value);
	
	//�ж��ǲ���
	bool is_number();
	
	Stackentry();
	~Stackentry();
}TStackentry;


typedef struct  SymList {
	TSymbol        * pSym;
	struct SymList * pNext;
}TSymList;

typedef struct CmdList {
	TCommand  * pCmd;
	struct CmdList * pNext;
}TCmdList;

typedef struct StackList {
	TStackentry * pStackItem;
	struct StackList * pNext;
}TStackList;


//����������
typedef class Interpreter {

public:
	
	bool GetInputParam(char * name,TSymbol & sym);          //��������ȡ���������ֵ(ת��Ϊ�ַ���)
	TSymbol * & GetOutput(char * name);
	TSymbol * GetOutput(int index, char *name);
	
	void SetInputData(int type, double value);             //������ֵ(��ֵ����)������������
	void SetInputData(int type, char * value);             //�����ַ���(�ַ�������)������������

	/* end of add */
	
	virtual double Sysinput(const char * ){return 0;};
	virtual char * SysinputS(const char * ){return NULL;};
	//virtual bool InputHandler();
	
	TSymbol Param(char * name)
	{
		TSymbol sym;
	
		if ( ! (GetInputParam(name, sym))){
			throw TException("ScriptParser::Param: ָ���Ĳ�����������");
		};
		
		return sym;
	};
 
	TSymbol * & Result(char * name)
	{
		return GetOutput(name);
	}; 

	TSymbol * Result(int index, char *name)
	{
		return GetOutput(index, name);
	};

	//���뵥���ַ�����ֵ
	void SimpleInput(char * value)
	{
		SetInputData(symSTRING, value);
	}
	
	//���뵥����ֵ��ֵ
	void SimpleInput(double value)
	{
		SetInputData(symDOUBLE, value);
	}
	
	//�������ַ�����ֵ
	void MultiInput(char * value)
	{
		SetInputData(symSTRING_ARRAY, value);
	}
	
	//��������ֵ��ֵ
	void MultiInput(double value)
	{
		SetInputData(symDOUBLE_ARRAY, value);
	}
	
	
	Interpreter();   //constructor
	virtual ~Interpreter();  //destructor
	
	
	void print(); //������ɵĴ���
	bool run();   //ִ��
	
	void Clear();
	void ClearResult();
	
	void StartRet();
	bool NextRet();
	TSymbol * Ret;
	
protected:
    TSymList  *  cur_result;
    
    	
	TStackList * selection_stack; //������Ŷ�ջ
	TStackList * iteration_stack; //ѭ����Ŷ�ջ
	//ָ������
	void add_command(TCommand & cmd); //����һ��ָ��
	void create_pushsym(char * symbol);    //����һ�� "���ջѹ��һ������" ָ��
	void create_pushdbl(double value);     //����һ�� "���ջѹ��һ����ֵ" ָ��	
	void create_pushstr(char * value);     //����һ�� "���ջѹ��һ���ַ���ֵ" ָ��	
	void create_dblbin(char c);            //����һ�� "��Ԫ����" ָ��
	void create_assign();                  //����һ�� "��ֵ" ָ��
	void create_pop();                     //����һ�� "��ջ" ָ��
	void create_decide();                  //����һ�� "�����ж�" ָ��
	void create_jump(int dest_addr);                  //����һ�� "�����ж�" ָ��
	void create_unary_op(int op);          //����һ�� "һԪ����" ָ��
	void create_postinc();                 //����һ�� "�Ҽ�һ" ָ��
	void create_postdec();                 //����һ�� "�Ҽ�һ" ָ��
	void create_reop(int op);              //����һ�� "��ϵ����" ָ��
	void create_logicalop(int op);         //����һ�� "�߼�����" ָ��
	
	void create_break();                   //������תָ��,ʵ��break 
	void create_continue();                //������תָ��,ʵ��continue
	
	
	void create_symbol_def(int type, char * name);          //����һ�� "���Ŷ���"��ָ��
	void create_symbol_array_def(int type, char * name);    //����һ�� "������Ŷ���" ָ��
	void create_array_ref();                                //����һ�� "��������" ָ��
	void create_output(char * name);                        //����һ�� "���" ָ��
	void create_input_param(char * name);                   //����һ�� "�����������" ָ��
	void create_input();                                    //����һ�� "����" ָ��
	void create_prepare_input();                            //����һ�� "׼������" ָ��
	
	// add 2003/1/21
	void create_sysinput(char * name);                      //����һ�� "����ϵͳ����" ָ��
	void create_sysinputs(char * name);                      //����һ�� "����ϵͳ����(�ַ�����)" ָ��
	void create_sysoutput(char * name);                     //����һ�� "���ϵͳ����" ָ��
	// end of add 
	
	void stackswap(TStackList * & stk);             //����ջ������Ԫ��

	void pushlabel_jump(TStackList * & stk);        //������ջ,����һ��JUMPָ��
	void poplabel(TStackList * & stk);              //���ų�ջ,����һ��LABELָ��

	void pushlabel(TStackList * & stk);             //����һ��LABELָ��, ������ջ
	void poplabel_jump(TStackList * & stk);         //���ų�ջ,����һ��JUMPָ���ǰ����
	
	void pushnolabel(TStackList * & stk);           //����һ��������ջ
	void nopop_jump(TStackList * & stk);            //��ջ��ȡһ������,����һ��JUMP

private:
	int labelcount;
	int cmd_count;
	TCmdList * command_list;       //ָ����
	TSymList * symbol_list;        //������
	TStackList *  runtime_stack;  //����ʱ��ջ

	
	TSymList * input_param_list;  //�����ȡ�����б�
	TSymList * output_data_list;  //��������б�
	
	void FreeSymList(TSymList * & );
	void FreeStrList(TStrList * &);
	void FreeDblList(TDblList * &);
	void FreeStackList(TStackList * &);
	void FreeCmdList(TCmdList * &);
	
	//add 2003/1/21
	TStrList * sysinput_list;    //����ϵͳ�����б�
	//end of add 
	
	TSymbol  input_data;   //��������

	TSymList * find_symbol(char * s);
	
	//ָ��ִ��
	void exec_pushsym(TCmdList *);  //ִ��һ�� "���ջѹ��һ������" ָ��
	void exec_pushdbl(TCmdList *);     //ִ��һ�� "���ջѹ��һ����ֵ" ָ��	
	void exec_pushstr(TCmdList *);     //ִ��һ�� "���ջѹ��һ���ַ���ֵ" ָ��	
	void exec_dblbin(TCmdList *);      //ִ��һ�� "��Ԫ����" ָ��
	void exec_assign(TCmdList *);      //ִ��һ�� "��ֵ" ���
	void exec_pop(TCmdList *);         //ִ��һ�� "��ջ" ָ��
	void exec_decide(TCmdList * &);      //ִ��һ�� "�����ж�" ָ��
	void exec_jump(TCmdList * &);      //ִ��һ�� "��ת" ָ��
	void exec_unary(TCmdList *);    //ִ��һ�� "һԪ����" ָ��
	void exec_postinc(TCmdList *);   //ִ��һ�� "�Ҽ�һ" ָ��
	void exec_postdec(TCmdList *);   //ִ��һ�� "�Ҽ�һ" ָ��
	void exec_reop(TCmdList *);      //ִ��һ�� "��ϵ����" ָ��
	void exec_logicalop(TCmdList *);        //ִ��һ�� "�߼�����" ָ��
	void exec_symbol_def(TCmdList *);       //ִ��һ�� "���Ŷ���" ָ��
	void exec_symbol_array_def(TCmdList *); //ִ��һ�� "������Ŷ���" ָ��
	void exec_array_ref(TCmdList *);    //ִ��һ�� "��������" ָ��
	void exec_output(TCmdList *);       //ִ��һ�� "���" ָ��
	void exec_input(TCmdList *);       //ִ��һ�� "����" ָ��
	void exec_input_param(TCmdList *);       //ִ��һ�� "�������" ָ��
	void exec_prepare_input(TCmdList *);  //ִ��һ�� "׼������"ָ��

	// add 2003/1/21
	void exec_sysinput(TCmdList *);  //ִ��һ�� "����ϵͳ����"(double��) ָ��
	void exec_sysinputs(TCmdList *);  //ִ��һ�� "����ϵͳ����(�ַ�����)" ָ��
	  void exec_sysinputInt(TCmdList *); //ִ��һ�� "����ϵͳ����"(����) ָ��
	// end of add
	
	char *  get_cmd_exp(int type);   //ȡ��ָ������
	void print_cmd(TCmdList *);
	
}TInterpreter;



#endif
