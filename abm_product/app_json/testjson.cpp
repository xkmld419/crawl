#include "JsonMgr.h"

int main(int argc, char ** argv)
{
    
    vector<string> test;
    test.push_back("I-plan_id,S-item_desc,S-file_dir,S-open_state");
    test.push_back("1,shishikan,1\n\",1.1");
    char stemp[256];
    sprintf(stemp,"%s",  "2,test,/cbilling\n\\,1.3");
    test.push_back("2,test,/cbilling\n\\,1.30000");
    test.push_back(stemp);
    test.push_back("3,������,/cbilling/app/trans_data/50-30-02-00-00-001,4.5");
    test.push_back("4,�Ʒѽ����쳣�ļ�,/cbilling/app/trans_data/50-30-02-00-00-001,-2.0");
    test.push_back("5,Ԥ��������ģ�鴦������ָ������,/cbilling/app/trans_data/50-30-02-00-00-001,-30");
    
    
    JsonMgr json;
    
    json.writeJsonFile("testJson.json",test, ',' );
    
    
    return 0;
         
}


