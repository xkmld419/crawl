// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef __PARAM_DEFINE_MGR_H_
#define __PARAM_DEFINE_MGR_H_

class ParamDefineMgr {
 public:
	static bool getParam(char const *_sSegment, char const *_sCode,
 char *_sValue);	
    static long getLongParam(char const *sSegment, char const *sCode);
    
};

#endif




