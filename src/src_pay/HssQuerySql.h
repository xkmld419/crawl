#ifndef _HSS_TT_QUERY_INCLUDE_
#define _HSS_TT_QUERY_INCLUDE_

#include "TTTns.h"
#include "TimesTenInterface.h"
#include "ABMException.h"
#include <string>

class HssQuerySql : public TTTns
{
public:
    HssQuerySql();
    
    ~HssQuerySql();
    
    int init(ABMException &oExp);

public:
    
    TimesTenCMD *m_poBal;  //����ѯ
    TimesTenCMD *m_poBal2; //��serv_id��ѯ
    TimesTenCMD *m_poRec;  //���˲�ѯ
    TimesTenCMD *m_poPay;  //���׼�¼��ѯ
    TimesTenCMD *m_poBil;  //�����˵���ѯ
    TimesTenCMD *m_poTest;
};

#endif /*end _HSS_TT_QUERY_INCLUDE_*/