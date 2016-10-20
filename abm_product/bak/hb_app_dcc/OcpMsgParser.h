#ifndef _OCP_MSG_PARSER_H
#define _OCP_MSG_PARSER_H
#include <string>
#include "Process.h"
#include "diameter_parser_api.h"
#include "dcc_ConstDef.h"

using namespace std;
#define COMMAND_CODE_CC 272
#define COMMAND_CODE_RA 258
#define COMMAND_CODE_AS 274
#define COMMAND_CODE_DW 280
#define COMMAND_CODE_DP 282
#define COMMAND_CODE_ST 275
#define COMMAND_CODE_CE 257

#define NTP_BASETIME 2208988800ul

#define NOKNOWN 10999

#define CCR1 10000
#define CCA 10001
#define RAR 10002
#define RAA 10003
#define ASR 10004
#define ASA 10005
#define DWR 10006
#define DWA 10007
#define DPR 10008
#define DPA 10009
#define STR 10011
#define STA 10012
#define CER 10013
#define CEA 10014

typedef struct
{
    int ServiceKey;
    int CallingPartysCategory;
    char CallingPartyNumber[30];
    char CalledPartyNumber[32];
    char CallingVlrNumber[30];
    char CallingCellIDOrSAI[30];
    char CallingLAI[30];
    char CalledVlrNumber[30];
    char CalledCellIDOrSAI[30];
    char CalledLAI[30];
    char CalledIMEI[30];
    char CallingIMEI[30];
    char OiginalCalledPartyId[30];
    char HighLayerCompatibility[10];
    char AdditionalCallingPartyNumber[30];
    char BearerCapability[10];
    char BearerCapability2[10];
    int EventTypeBCSM;
    char RedirectingPartyId[30];
    char RedirectionInformation[30];
    char IMSI[30];
    char TeleServiceCode[10];
    char BearerServiceCode[10];
    char CallReferenceNumber[30];
    char MSCAddress[50];
    char GMSCAddress[50];
    char TimeZone[10];
    int INServiceIndicator;
    char MsClassmark2[10];
    char TeleServiceCode2[10];
    char BearerServiceCode2[10];
    int CallTypeID;
} InInformation;


#define SHM_SERVICETYPE    (IpcKeyMgr::getIpcKey(Process::m_iFlowID, "SHM_SERVICETYPE"))




class OcpMsgParser
{
public:
    OcpMsgParser(char *dict_file): m_oMsg(std::auto_ptr < AAAMessage > (new
                                              AAAMessage)), m_oDm(std::auto_ptr < AAADictionaryManager > (new
                                                      AAADictionaryManager))
    {
        char *p;
        string str;

        if ((p = getenv("TIBS_HOME")) == NULL)
        {
            cout << "Please set env value TIBS_HOME. For example  export TIBS_HOME=/bill/rtbill/TIBS_HOME" << endl;
            throw( - 1);
        }
        sprintf(m_sABM_HOME, "%s", p);
        if(p[strlen(p)] == '/')
        {
            p[strlen(p)] = '\0';
        }
        if(dict_file)
        {
            sprintf(m_sDictFile, "%s/etc/%s", m_sABM_HOME, dict_file);
        }
        else
        {
            sprintf(m_sDictFile, "%s/etc/dictionary.xml", m_sABM_HOME);
        }
        
        m_shmServiceType = (char *)getShmAddr(SHM_SERVICETYPE, SHM_ITEM_NUM * SHM_ITEM_LEN);

      
        m_oDm->init(m_sDictFile);
        m_bSetFlag = false;


        /*char chHostName[1024];
        int i;
        struct in_addr addr;
        char LocalIp[64] = {0};
        		[>ȡ�û�������<]
        if(gethostname(chHostName,sizeof(chHostName)) !=-1)
        {
        		[>ȡ�ø�����������Ϣ<]
        		struct hostent* phost = gethostbyname(chHostName);
        		[>��ÿһ����ַ��β<]
        		for(i=0;phost!= NULL&&phost->h_addr_list[i]!=NULL;i++)
        		{
        				memcpy(&addr, phost->h_addr_list[i], sizeof(struct in_addr));
        		}
        		strcpy(LocalIp, inet_ntoa(addr));
        }
        int buf[4];
        sscanf(LocalIp, "%d.%d.%d.%d", &buf[0], &buf[1], &buf[2], &buf[3]);
        char tem[4] = {0};
        tem[0] = buf[0];
        tem[1] = buf[1];
        tem[2] = buf[2];
        tem[3] = buf[3];

        m_localIp.type = 1;
        m_localIp.value.assign(tem, 4); */

    }

    virtual ~OcpMsgParser() {}

    void initDict(char *dict_file)
    {
        m_oDm->init(dict_file);
        m_bSetFlag = false;
    }

    void initDictionary(char *dict_file)
    {
        ::RemoveDictionary();
        m_oDm->init(dict_file);
    }
    void setDiameterHeader(int code,  //����Diameter��Ϣͷ
                           int request = true, int appId = AAA_BASE_APPLICATION_ID, int hh = 0, int
                           ee = 0)
    {
        m_oMsg->hdr.ver = AAA_PROTOCOL_VERSION;
        m_oMsg->hdr.length = 0;
        m_oMsg->hdr.flags.r = request ? AAA_FLG_SET : AAA_FLG_CLR;
        m_oMsg->hdr.flags.p = AAA_FLG_CLR;
        m_oMsg->hdr.flags.e = AAA_FLG_CLR;
        m_oMsg->hdr.flags.t = AAA_FLG_CLR;
        m_oMsg->hdr.code = code;
        m_oMsg->hdr.appId = appId;
        m_oMsg->hdr.hh = hh;
        m_oMsg->hdr.ee = ee;
    }

    void setCCADiameterHeader()
    {
        //����CCA��Ϣͷ
        m_oMsg->hdr.flags.r = AAA_FLG_CLR;
    }

    void setRARDiameterHeader()
    {
        //����RAR��Ϣͷ
        srand((unsigned int)time(NULL));
        int hh = rand(); // random number seed
        int ee = (unsigned int)(time(NULL)) << 20; // set high 12 bit
        setDiameterHeader(COMMAND_CODE_RA, true, AAA_BASE_APPLICATION_ID + 4,
                          hh, ee);
    }

    void setASRDiameterHeader()
    {
        //����ASR��Ϣͷ
        setDiameterHeader(COMMAND_CODE_AS, true, AAA_BASE_APPLICATION_ID + 4,
                          random(), random());
    }

    void setDWRDiameterHeader()
    {
        //����DWR��Ϣͷ
        setDiameterHeader(COMMAND_CODE_DW, true, AAA_BASE_APPLICATION_ID + 4,
                          random(), random());
    }

    int parseAppToRawWithCommandFail(int code, unsigned char *buf);


    void setAvpCheck(int avp_check); //����Dimeterͷ������λ

    void setDiameterHeaderRequest(bool request) //����Dimeterͷ������λ
    {
        m_oMsg->hdr.flags.r = request ? AAA_FLG_SET : AAA_FLG_CLR;
    }

    int getDiameterCommandCodeInfo()
    //��ȡOcp��Ϣ�������,�ڲ���COMMAND_CODE_CC
    {
        return m_oMsg->hdr.code;
    }

    std::string &getFailedAvp() //��ȡʧ�ܵ�Avp�ַ���
    {
        return m_sFailedAvp;
    }

    int getDiameterCommandCode(); //��ȡOcp��Ϣ���룬����CCR,CCA�Ⱥ궨��

    AAADiameterHeader &getDiameterHeader() //��ȡdiameter��Ϣͷ
    {
        return m_oMsg->hdr;
    }

    virtual int parseRawToApp(void *buf, int bufSize, ParseOption option =
                                  PARSE_STRICT); //����Ocp��Ϣ
    virtual int parseRawToApp(std::string &buf, ParseOption option =
                                  PARSE_STRICT); //����Ocp��Ϣ

    virtual int parseAppToRaw(void *buf, int bufSize, bool output = false,
                              bool bad_answer = false, ParseOption option = PARSE_STRICT);
    //ת��Ocp��Ϣ
    virtual int parseAppToRaw(std::string &buf, bool output = false,
                              ParseOption option = PARSE_STRICT); //ת��Ocp��Ϣ

    int getGroupedAvpNums(const char *avp_name); //��ȡGroupedAvp����
    int getGroupedAvpNums1(const char *avp_name); //��ȡGroupedAvp����


    int setAvpValue(char *avp_name, std::string &data); //����Avp�ַ���ֵ
    int getAvpValue(char *avp_name, std::string &data); //��ȡAvp�ַ���ֵ

    int setAvpValue(char *avp_name, long &data); //����Avp������ֵ
    int getAvpValue(char *avp_name, long &data); //��ȡAvp������ֵ

    int setAvpValue(char *avp_name, ACE_UINT64 &data); //����Avp64λ������ֵ
    int getAvpValue(char *avp_name, ACE_UINT64 &data); //��ȡAvp64λ������ֵ

    int setAvpValue(char *avp_name, ACE_UINT32 &data); //����Avp64λ������ֵ
    int getAvpValue(char *avp_name, ACE_UINT32 &data); //��ȡAvp64λ������ֵ

    int setAvpValue(char *avp_name);

    int deleteAvpValue(char *avp_name); //ɾ��avp����

    int setAvpValue(char *avp_name, int &data); //����Avp����ֵ
    int getAvpValue(char *avp_name, int &data); //��ȡAvp����ֵ

    int setAvpValue(char *avp_name, float &data); //����Avp������ֵ
    int getAvpValue(char *avp_name, float &data); //��ȡAvp������ֵ

    int setAvpValue(char *avp_name, diameter_address_t &data); //����Avp��ַ
    int getAvpValue(char *avp_name, diameter_address_t &data); //��ȡAvp��ַ


    std::string &getAllAvp(bool disp_flag = true); //��ȡ���е�Avp��Ϣ

    void printHeader(); //��ӡDiameterͷ��Ϣ
    std::string &getAvpOutStr() //��ȡ���е�Avp��Ϣ�ַ���
    {
        return m_sAvpOutStr;
    }

    bool getSetFlag() //����Ƿ���ù�setAvpValue
    {
        return m_bSetFlag;
    }

    void setSetFlag(bool set_flag) //����SetFlag��׼
    {
        m_bSetFlag = set_flag;
    }

    void releaseMsgAcl() //�ͷ�Msg acl
    {
        m_oMsg->acl.releaseContainers();
    }

    void getServiceType(string &serviceContextId, char *serviceType);


    void initLocalInfo(char *sOriginHost, char *sOriginRealm, char *sLocalIp);
    int avp2str(string strAvp, string &strData, int &iType);
    int str2avp(int iType, string strData, string &strAvp);

    int createCER(char *sArguments, char *sCERbuf, int iCERSize);
    int createActiveCCR(char *sArguments, char (*contextIDs)[256], int iRows, char *sCCRbuf, int iCCRSize);
    int createServCCR(char *sSrcBuf, int iSrcSize, char *sCCRbuf, int iCCRSize);
    int createServCCA(char *sSrcBuf, int iSrcSize, char *sCCAbuf, int CCASize);

    // add by zhangch
    int createDWR(char *sArguments, char *sDWRbuf, int iDWRSize);
    int createDPR(char *sArguments, char *sDPRbuf, int iDPRSize);


    int parserPack(char *sMsgBuf, int iMsgSize, char *sParseBuf, int iParseSize, char *sServiceContextID);

    string getSessionid();
    long getHopByHop();

public:
    string m_strOriginHost;
    string m_strOriginRealm;

    // add by zhangch
    string m_strLocalIp;

    string m_strDestHost;
    string m_strDestRealm;

    char m_sDictFile[101];
    char m_sABM_HOME[80];
    diameter_address_t m_localIp;
	void* getshm();


private:
    void* getShmAddr(key_t , int );

    //   private:
    auto_ptr < AAAMessage > m_oMsg; //OCP��Ϣ�ڲ��ṹ
    auto_ptr < AAADictionaryManager > m_oDm; //OCPЭ���ֵ������
    InInformation m_oInInformation;
    template <class T> int _setAvpValue(char *avp_name1, T &data,
                                        AAA_AVPDataType t);
    template <class T> int _getAvpValue(char *avp_name1, T &data);

    void getAvpData(AAAAvpContainerEntry *e, int pos, int size,std::vector<string>  &sAvpName);
    void _getAllAvp(AAAAvpContainerList *orig_acl, std::vector<string>  &sAvpName);
    void getAvpName(std::vector<string>  &sAvpName, string &avp_name);
    void setOutStr(string &avp_name, char *sFormatStr, char *sTmpBuf);
    void setDictHandle(bool output = false);
    int getServiceInfo(char *buf, string &serviceContextId, string  &serviceIdentifier);


    string m_sAvpOutStr; //Ocp��Ϣ�����ַ���
    bool m_bSetFlag; //�ж��Ƿ���ù�setAvpValue;
    bool m_bDispFlag; //�Ƿ��ӡ����Ļ
    string m_sFailedAvp;

    string getServiceType(string &serviceContextId);


    long m_iHopByHop ;
    string m_sSsessionid;

	char* m_shmServiceType;
};
#endif
