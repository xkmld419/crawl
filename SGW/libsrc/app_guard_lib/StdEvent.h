// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef STDEVENT_H_HEADER_INCLUDED_BDB42691
#define STDEVENT_H_HEADER_INCLUDED_BDB42691

#include "CommonMacro.h"
#include <string.h>


class StdEventExt
{
  public:
   

};


//##ModelId=41CF742F03B3
//##Documentation
//## �˽ṹ����ϵͳ���漰�������¼����Ե�ȫ���������ټ�һ���ֱ����ֶΡ�
class StdEvent
{
	public:
 		char m_sXMLTreeBuf[MAX_XML_TREE_BUF_LENGTH];	
 	 
 	 
	public:  //function
		void resetMember()
		{
			memset(m_sXMLTreeBuf,0,MAX_XML_TREE_BUF_LENGTH);
		}
}; 		


#endif /* STDEVENT_H_HEADER_INCLUDED_BDB42691 */




