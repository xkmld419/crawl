#ifndef _EVENTTOOLKITS_H_
#define _EVENTTOOLKITS_H_

#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "StdEvent.h"

using namespace std;
/*class IndexTimeStamp
{
public:
    IndexTimeStamp(time_t ltmStamp,const string strIndex1):lTimeStamp(ltmStamp),strIndex(strIndex1){};
public:
	time_t    lTimeStamp;
    const string  strIndex;
};

bool TimeStampGreate(const IndexTimeStamp& stamp1,const IndexTimeStamp& stamp2)
{
    return stamp1.lTimeStamp>=stamp2.lTimeStamp;
};
*/
class EventToolKits
{
	public :

		// ���캯��
		EventToolKits();

		// ��������
		~EventToolKits();

		// �����¼�����ID��ȡ�¼�����ֵ������
		string get_param( const StdEvent *pStd, int iEventAttrId, int &iType);

		// �����¼�����ID��ȡ�¼�����ֵ
		string get_param( const StdEvent *pStd, int iEventAttrId );

		string get_param2(const StdEvent * pStd, int iEventAttrId, int & iType,void **pEventAttr,int &iValueSize);
		string get_param2(const StdEvent *pStd,int iEventAttrID,int &iType);

		// �����¼�����ID�����¼�����ֵ
		void set_param( StdEvent *pStd, int iEventAttrId, string strAttr );

		// long ת��Ϊ string
		string toString( const long lval );

		// int ת��Ϊ string
		string toString( const int ival );

		// string ת��Ϊ int
		int toInt( string strval );

		// string ת��Ϊ bool
		bool toBool( string strval );

		// ת��ΪСд
		string &lowercase( string &strval );

		// ת��Ϊ��д
		string &uppercase( string &strval );

		// ȥ����β�ո�
		string trim( string &strval );

		// ����ʱ����λ��
		double time_diff( const char *strTime1, const char *strTime2 );
		double time_diff( string const &strTime1, string const &strTime2 );

		// �ַ���ת��Ϊ����
		time_t toDate( const char *strDate );

		// �ַ���ת��Ϊʱ��
		time_t toTime( const char *strTime );

		// �ַ���ת��Ϊ����ʱ��
		time_t toDateTime( const char *strDateTime );

		// stringת��Ϊfloat
		double toFloat( string strval );
		
		// string ת����long
		long toLong( string strval);
	private :

		// ���ڸ�ʽת�����ַ���
		ostringstream m_oSstream;

		// Сдת��functor
		struct lowertrans
		{
			void operator()( string::value_type &r )
			{
				r = r | 0x20;
			}
		};
		// ��дת��functor
		struct uppertrans
		{
			void operator()( string::value_type &r )
			{
				r = r & ( !0x20 );
			}
		};
};




















#endif
