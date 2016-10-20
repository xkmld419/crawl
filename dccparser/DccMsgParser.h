#ifndef _OCP_MSG_PARSER_H
#define _OCP_MSG_PARSER_H
#include <string>
#include <stdlib.h>
#include "diameter_parser_api.h"
#include "parser_avpvalue.h"

#define 	COMMAND_CODE_CC 272
#define 	COMMAND_CODE_RA 258
#define		COMMAND_CODE_AS 274
#define		COMMAND_CODE_DW 280
#define		COMMAND_CODE_DP 282
#define		COMMAND_CODE_ST 275
#define		COMMAND_CODE_CE 257

#define NTP_BASETIME 2208988800ul

#define 	NOKNOWN 10999

#define 	CCR1 10000
#define 	CCA 10001
#define 	RAR 10002
#define 	RAA 10003
#define		ASR 10004
#define		ASA 10005
#define		DWR 10006
#define		DWA 10007
#define		DPR 10008
#define		DPA 10009
#define		STR 10011
#define		STA 10012
#define		CER 10013
#define		CEA 10014

extern char *ntp_fmt_ts(long reftime, char* buff);

class DccMsgParser
{
public:
    DccMsgParser():
    		m_oMsg(std::auto_ptr<AAAMessage>(new AAAMessage)),
         m_oDm(std::auto_ptr<AAADictionaryManager>(new AAADictionaryManager)) {
    }

    DccMsgParser(char *dict_file) :
           m_oMsg(std::auto_ptr<AAAMessage>(new AAAMessage)),
           m_oDm(std::auto_ptr<AAADictionaryManager>(new AAADictionaryManager)){
      m_oDm->init(dict_file);
      m_bSetFlag = false;
    }
    virtual ~DccMsgParser() {}

    void initDictionary(char *dict_file) {
      ::RemoveDictionary();
      m_oDm->init(dict_file);
    }
    void setDiameterHeader(int code,		//����Diameter��Ϣͷ
                      int request = true,
                      int appId = AAA_BASE_APPLICATION_ID, int hh=0, int ee=0 ) {
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


    void setCCADiameterHeader() {		//����CCA��Ϣͷ
    	m_oMsg->hdr.flags.r = AAA_FLG_CLR;
    }

    void setRARDiameterHeader() {		//����RAR��Ϣͷ
      srand((unsigned int)time(NULL));
      int hh = rand(); // random number seed
      int ee = (unsigned int)(time(NULL)) << 20; // set high 12 bit
    	setDiameterHeader(COMMAND_CODE_RA, true, AAA_BASE_APPLICATION_ID + 4 , hh, ee);
    }

    void setASRDiameterHeader() {		//����ASR��Ϣͷ
    	setDiameterHeader(COMMAND_CODE_AS, true, AAA_BASE_APPLICATION_ID + 4 , rand(), rand());
    }

    void setDWRDiameterHeader() {		//����DWR��Ϣͷ
    	setDiameterHeader(COMMAND_CODE_DW, true, AAA_BASE_APPLICATION_ID + 4 , rand(), rand());
    }

    int parseAppToRawWithCommandFail(int code, unsigned char *buf);


    void setAvpCheck(int avp_check); //����Dimeterͷ������λ

    void setDiameterHeaderRequest(bool request) //����Dimeterͷ������λ
    {
    	m_oMsg->hdr.flags.r = request ? AAA_FLG_SET : AAA_FLG_CLR;
    }

    int getDiameterCommandCodeInfo() 		//��ȡOcp��Ϣ�������,�ڲ���COMMAND_CODE_CC
    {
    	return m_oMsg->hdr.code;
    }

    std::string& getFailedAvp()					//��ȡʧ�ܵ�Avp�ַ���
    {
    	return m_sFailedAvp;
    }

    int getDiameterCommandCode() ;		//��ȡOcp��Ϣ���룬����CCR,CCA�Ⱥ궨��

    AAADiameterHeader& getDiameterHeader()	//��ȡdiameter��Ϣͷ
    {
    	return m_oMsg->hdr;
    }

    virtual int parseRawToApp(void *buf, int bufSize, ParseOption option = PARSE_STRICT); //����Ocp��Ϣ
    virtual int parseRawToApp(std::string &buf, ParseOption option = PARSE_STRICT); //����Ocp��Ϣ

    virtual int parseAppToRaw(void *buf, int bufSize, bool output=false, bool bad_answer=false, ParseOption option = PARSE_STRICT ); //ת��Ocp��Ϣ
    virtual int parseAppToRaw(std::string &buf, bool output=false, ParseOption option = PARSE_STRICT); //ת��Ocp��Ϣ

    int getGroupedAvpNums(const char *avp_name);						//��ȡGroupedAvp����
    int getGroupedAvpNums1(const char *avp_name);						//��ȡGroupedAvp����


    int setAvpValue(char *avp_name, std::string &data);			//����Avp�ַ���ֵ
    int getAvpValue(char *avp_name, std::string &data);			//��ȡAvp�ַ���ֵ

    int setAvpValue(char *avp_name, long &data);			//����Avp������ֵ
    int getAvpValue(char *avp_name, long &data);			//��ȡAvp������ֵ

    int setAvpValue(char *avp_name, ACE_UINT64 &data);			//����Avp64λ������ֵ
    int getAvpValue(char *avp_name, ACE_UINT64 &data);			//��ȡAvp64λ������ֵ

    int setAvpValue(char *avp_name, ACE_UINT32 &data);			//����Avp64λ������ֵ
    int getAvpValue(char *avp_name, ACE_UINT32 &data);			//��ȡAvp64λ������ֵ

    int setAvpValue(char *avp_name);

    int deleteAvpValue(char *avp_name);			//ɾ��avp����

    int setAvpValue(char *avp_name, int &data);				//����Avp����ֵ
    int getAvpValue(char *avp_name, int &data);				//��ȡAvp����ֵ

    int setAvpValue(char *avp_name, float &data);			//����Avp������ֵ
    int getAvpValue(char *avp_name, float &data);			//��ȡAvp������ֵ

    int setAvpValue(char *avp_name, diameter_address_t &data);				//����Avp��ַ
    int getAvpValue(char *avp_name, diameter_address_t &data);				//��ȡAvp��ַ


    std::string& getAllAvp(bool disp_flag = true);				 //��ȡ���е�Avp��Ϣ

    void printHeader();							//��ӡDiameterͷ��Ϣ
    std::string& getAvpOutStr()						//��ȡ���е�Avp��Ϣ�ַ���
    {
    	return m_sAvpOutStr;
    }

    bool getSetFlag()							//����Ƿ���ù�setAvpValue
    {
    	return m_bSetFlag;
    }

    void setSetFlag(bool set_flag)					//����SetFlag��׼
    {
    	m_bSetFlag = set_flag;
    }

    void releaseMsgAcl()										//�ͷ�Msg acl
    {
    	m_oMsg->acl.releaseContainers();
    }

    void getServiceType(std::string &serviceContextId, char *serviceType);
		
   private:
   	std::auto_ptr<AAAMessage> m_oMsg;  		//OCP��Ϣ�ڲ��ṹ
   	std::auto_ptr<AAADictionaryManager> m_oDm;	//OCPЭ���ֵ������
    template<class T> int _setAvpValue(char *avp_name1, T &data, AAA_AVPDataType t);
    template<class T> int _getAvpValue(char *avp_name1, T &data);
		void getAvpData(AAAAvpContainerEntry *e, int pos, int size, std::vector<std::string>& sAvpName);
		void _getAllAvp(AAAAvpContainerList *orig_acl, std::vector<std::string>& sAvpName);
		void getAvpName(std::vector<std::string>& sAvpName, std::string& avp_name);
		void setOutStr(std::string& avp_name, char *sFormatStr, char *sTmpBuf);
    void setDictHandle(bool output=false);
    int getServiceInfo(char *buf, std::string &serviceContextId, std::string &serviceIdentifier);

//		std::map<std::string, int> 	    	mapInt32;
//		std::map<std::string, long> 	    	mapInt64;
//		std::map<std::string, std::string>    	mapStr;
		std::string 	m_sAvpOutStr;			//Ocp��Ϣ�����ַ���
		bool		m_bSetFlag;			//�ж��Ƿ���ù�setAvpValue;
		bool 		m_bDispFlag;			//�Ƿ��ӡ����Ļ
		std::string	m_sFailedAvp;
};
#endif
