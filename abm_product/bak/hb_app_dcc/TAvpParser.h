#ifndef _T_AVP_PARSER_H
#define _T_AVP_PARSER_H

#include "diameter_parser_api.h"
#include "avplist.h"
#include "g_avplist.h"
#include "comlist.h"

#include "HashList.h"
#include "Log.h"
#include "dcc_ParamsMgr.h"
#include <string>
#include <map>
#include <utility>
using namespace std;
#include <stdlib.h>
#include <string.h>

#define COMMAND_CODE_CC 272
#define COMMAND_CODE_RA 258
#define COMMAND_CODE_AS 274
#define COMMAND_CODE_DW 280
#define COMMAND_CODE_DP 282
#define COMMAND_CODE_ST 275
#define COMMAND_CODE_CE 257

#define COMMAND_NAME_CC "Credit-Control"
#define COMMAND_NAME_RA "Re-Auth"
#define COMMAND_NAME_AS "Abort-Session"
#define COMMAND_NAME_DW "Device-Watchdog"
#define COMMAND_NAME_DP "Disconnect-Peer"
#define COMMAND_NAME_ST "Session-Termination"
#define COMMAND_NAME_CE "Capabilities-Exchange"

#define BODY_LEN 8192

#define DEBUG_TAVPPARSER 0

enum eCommandCode
{
COMMAND_CC = 272,
COMMAND_DW= 280,
COMMAND_DP = 282,
COMMAND_CE = 257
};

class TAvpHead 
{ 
public:
    int setHeadValue(const char *sAvpHead);
    void toCString(char *sAvpHead);

    int  Version;
    unsigned int  Length;
    int  CommandCode;
    char CommandFlag;
    int  AppId;
    unsigned int  HopId;
    unsigned int  EndId;
    int initHead(eCommandCode,bool);

private:
    char* subStr2Int(char *pos1, const char *delim, unsigned int &num);
};

class TAVPObject : public map<string, string>
{
public:
    int setObjectValue(const char *sAvpObject);
    void toCString(char *sAvpObject);
public:
    TAvpHead avpHead;
};


class DictCommand;
class AvpField;

class TAvpParser 
{
public:
    TAvpParser():
        m_oMsg(auto_ptr<AAAMessage>(new AAAMessage)),
        m_oDm(auto_ptr<AAADictionaryManager>(new AAADictionaryManager)),
        m_bDispFlag(false), m_bBuiledBody(false), m_option(PARSE_LOOSE)
    { }

    TAvpParser(char *dict_file) :
        m_oMsg(auto_ptr<AAAMessage>(new AAAMessage)),
        m_oDm(auto_ptr<AAADictionaryManager>(new AAADictionaryManager)),
        m_bDispFlag(false), m_bBuiledBody(false), m_option(PARSE_LOOSE)
    {
        m_oDm->init(dict_file);
        initDictCommand();
    }

    virtual ~TAvpParser() 
    { }

    void initDictionary(char *dict_file) 
    {
        ::RemoveDictionary();
        m_oDm->init(dict_file);
        initDictCommand();
    }
    
    // ����ȫ�ֺϷ�����ʶhead_check��avp_check��0������⣬��0�����
    void setCheckOption(int head_check, int avp_check) 
    {
        if (head_check == 0)
            m_option = PARSE_LOOSE;
        else
            m_option = PARSE_STRICT;

        ::setAvpCheck(avp_check);
    }
    
    //��ȡDCC����log
    string& getDccLog() 
    {
        return m_strDccLog;
    }

    //��ȡʧ�ܵ�Avp�ַ���
    string& getFailedAvp() 
    {
        return m_strFailedAvp;
    }

    // �����Ƿ��ӡlog��ʶ
    void setDispFlag(bool bDispFlag)
    {
        m_bDispFlag = bDispFlag;
    }

public:
    // ������Ϣ����ͷ��Ϣ
    int ParseHead(unsigned char *pDataBuffer, unsigned int iDataSize, TAvpHead *pAvpHead);

    // ������ĳһ��DCC��Ϣ������
    int ParseBody(unsigned char *pDataBuffer, unsigned int iDataSize, TAVPObject *pAVPObject);

    // ���ڴ�����Ϣ����б���
    int BuildBody(TAVPObject *pAvpObject, unsigned char *pDataBuffer, unsigned int *iDataSize);

    // ����Ϣͷ���б���
    int BuildHead(TAvpHead *pAvpHead, unsigned char *pDataBuffer, unsigned int *iDataSize);

    // �ж���Ϣͷ�����ݻ�����ʽ���Ե�Ҫ�󣬸�ҵ����ص�AVP�����ж�
    int IsValidMsg(TAVPObject *pAVPObject);

    // ������ϢЯ����ҵ������
    void setServiceType(const string strServiceType);
    // ��ȡ��ϢЯ����ҵ������
    string& getServiceType();

    // ��ȡ�������TAVPObject
    void getAVPObject(TAVPObject *pAVPObject);

    
private:
    // �ͷ�Msg acl
    void releaseMsgAcl()                                    
    {
        m_oMsg->acl.releaseContainers();
    }

    void initDictCommand();
    void initGroupedAvp(AAADictionary* pAAADictionary, const char* avpNamePrefix, const char* avpCodePrefix, DictCommand *pDictCommand);
    void initQualiedAvp(AAAQualifiedAvpList* pQualifiedAvpList, const char* avpNamePrefix, const char* avpCodePrefix, DictCommand *pDictCommand);


    void initFixedAvp(AAADictionary* pAAADictionary, const char* prefix, DictCommand *pDictCommand);

    template <class T> int _setAvpValue(const char *avp_name1, T &data, AAA_AVPDataType t);
    template <class T> int _getAvpValue(const char *avp_name1, T &data);

    void recordHeadLog();

    void getAllAvp(AAAAvpContainerList *orig_acl, vector<string> &sAvpName, TAVPObject *pAVPObject);
    void getAvpName(vector<string>& sAvpName, string& avp_name);
    void getAvpData(AAAAvpContainerEntry *e, int pos, int size, vector<string>  &sAvpName, TAVPObject *pAVPObject);

    void getAvpField(int commandCode, int request, const string &scenario, string strValue, int valueType, AvpField &avpField);
    int setMsgAvp(int commandCode, int request, const string &scenario, TAVPObject *pAVPObject);
    

private:
    auto_ptr<AAAMessage>           m_oMsg;      // DCC��Ϣ�ڲ��ṹ
    auto_ptr<AAADictionaryManager> m_oDm;       // DCCЭ���ֵ������

    bool          m_bDispFlag;          // �Ƿ��ӡ����Ļ
    bool          m_bBuiledBody;        // �Ƿ����Ϣ���ѽ��б���
    ParseOption   m_option;             // ��Ϣͷ�������ʱ���Ƿ����Ϣͷ���Խ���У��

    TAvpHead m_avpHead;               // DCC��Ϣͷ
    string  m_strDccLog;              // DCC��Ϣlog�ַ���
    string  m_strFailedAvp;           // ʧ�ܵ�Avp�ַ���
    string  m_strServiceType;         // ��ʶCCR��CCAЯ����ҵ�����ͣ�ȡService-Context-Id��ǰ׺����̬���ݶ�ȡPara 

    HashList<DictCommand *> *m_pDictCommandIndex;       // key ֵΪcommandCode_scenario-id��commandCode
};

class AvpField
{
public:
    AvpField(string avpName, string avpCode, AAA_AVPDataType avpType) :
        m_avpName(avpName), m_avpCode(avpCode), m_avpType(avpType)
    { }

    AvpField() :
        m_avpName(""), m_avpCode(""), m_avpType(AAA_AVP_DATA_TYPE)
    { }

    string m_avpName;
    string m_avpCode;
    AAA_AVPDataType m_avpType;
};

class DictCommand
{
public:
    DictCommand()
    { }

    ~DictCommand()
    {
        delete []m_pAvpFieldNameIndex; m_pAvpFieldNameIndex = NULL;
        delete []m_pAvpFieldCodeIndex; m_pAvpFieldCodeIndex = NULL;
        m_vecFixedAvp.clear();
    }

    HashList<AvpField *> *m_pAvpFieldNameIndex;       // key ֵΪavp_name1.avp_name2.avp_name3
    HashList<AvpField *> *m_pAvpFieldCodeIndex;       // key ֵΪavp_code1.avp_code2.avp_code3
    vector<string> m_vecFixedAvp;
};


#endif

