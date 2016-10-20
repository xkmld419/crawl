#include <string.h>
#include <vector>
using namespace std;

class version
{
public :
	string m_sVersion;        //�汾
	string m_sEnvironment;    //����
};

class subcommand
{
public :
	string m_sSubCode;        //�������
	string m_sFunction;       //�����
	string m_sFormat;         //�����ʽ
	string m_sDescription;    //��������
};

class command
{
public :
	string m_sCode;                        //�������
	vector <subcommand *> m_pSubcommand;   //������
};

class attributes
{
public :
	string m_sRule;   //��Ч����
	string m_sRange;  //ȡֵ��Χ
  string m_sValue;  //����ȡֵ
};

class param
{
public :
	string m_sCode;   //��������
	string m_sName;   //��������
	string m_sDescription;  	  //��������
	attributes m_attributes;		//��������
};

class help
{
public :
	version  m_versionHelp;
	vector <command *> m_pCommandHelp;
	vector <param *>   m_pParamHelp;
};
