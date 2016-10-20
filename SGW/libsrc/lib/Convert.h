#ifndef _INCLUDE_CONVERT_H_
#define _INCLUDE_CONVERT_H_

#include "platform.h"
#include "platform.h"
#include <string>
#include "platform.h"

template<class T>
void to_string(string & result,const T & t)
{
	 //static ostringstream oss;//����һ����
	 ostringstream oss;//����һ����
	 oss.clear();
	 oss<<t;//��ֵ����������
  	 result=oss.str();//��ȡת������ַ�ת������д��result
}

/*
template<class out_type,class in_value> out_type convert(const in_value & t)
{
//static stringstream stream;
stringstream stream;
stream.clear();

stream<<t;//�����д�ֵ

out_type result;//����洢ת�����

stream>>result;//��result��д��ֵ

return result;
}
*/

template<class out_type,class in_value> out_type convert(const in_value & t)
{

	 ostringstream oss;//����һ����
	 oss.clear();
	 oss<<t;//��ֵ����������
	 string result=oss.str();//��ȡת������ַ�ת������д��result
  	 return result;
}


#endif



