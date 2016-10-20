#ifndef _OCP_MSG_PARSER_H
#define _OCP_MSG_PARSER_H
#include <string>

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




typedef struct{
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
  int  EventTypeBCSM;
  char RedirectingPartyId[30];
  char RedirectionInformation[30];
  char IMSI[30];
  char TeleServiceCode[10];
  char BearerServiceCode[10];
  char CallReferenceNumber[30];
  char MSCAddress[50];
  char GMSCAddress[50];
  char TimeZone[10];
  int  INServiceIndicator;
  char MsClassmark2[10];
  char TeleServiceCode2[10];
  char BearerServiceCode2[10];
  int  CallTypeID;
}InInformation;
extern char *ntp_fmt_ts(long reftime, char* buff);

class OcpMsgParser
{
public:
    OcpMsgParser():  
    	  i(0),  	
    		m_oMsg(std::auto_ptr<AAAMessage>(new AAAMessage)),    			
         m_oDm(std::auto_ptr<AAADictionaryManager>(new AAADictionaryManager)) {
    }

    OcpMsgParser(char *dict_file) :
           m_oMsg(std::auto_ptr<AAAMessage>(new AAAMessage)),
           m_oDm(std::auto_ptr<AAADictionaryManager>(new AAADictionaryManager)){
      m_oDm->init(dict_file);
      m_bSetFlag = false;
    }
    virtual ~OcpMsgParser() {}

    void initDictionary(char *dict_file) {
      ::RemoveDictionary();
      m_oDm->init(dict_file);
    }
    void setDiameterHeader(int code,		//����Diameter��Ϣͷ
                      int request = true,
                      int appId = AAA_BASE_APPLICATION_ID, int hh=0, int ee=0,int pFlag=true,int eFlag=false ) {
      m_oMsg->hdr.ver = AAA_PROTOCOL_VERSION;
      m_oMsg->hdr.length = 0;
      m_oMsg->hdr.flags.r = request ? AAA_FLG_SET : AAA_FLG_CLR;
      m_oMsg->hdr.flags.p = pFlag?AAA_FLG_SET : AAA_FLG_CLR;
      m_oMsg->hdr.flags.e = eFlag?AAA_FLG_SET : AAA_FLG_CLR;
      m_oMsg->hdr.flags.t = AAA_FLG_CLR;
      m_oMsg->hdr.code = code;
      m_oMsg->hdr.appId = appId;
      m_oMsg->hdr.hh = hh;
      m_oMsg->hdr.ee = ee;
    }

    void setDiameterHeaderX(AAADiameterHeader hdr ) {
      m_oMsg->hdr = hdr;
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
    	setDiameterHeader(COMMAND_CODE_AS, true, AAA_BASE_APPLICATION_ID + 4 , random(), random());
    }

    void setDWRDiameterHeader() {		//����DWR��Ϣͷ
    	setDiameterHeader(COMMAND_CODE_DW, true, AAA_BASE_APPLICATION_ID + 4 , random(), random());
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

    int getFailedAvpCode()					//��ȡʧ�ܵ�Avp�ַ���
    {
    	return m_sFailedAvpCode;
    }
    
    int getDiameterCommandCode() ;		//��ȡOcp��Ϣ���룬����CCR,CCA�Ⱥ궨��

    AAADiameterHeader& getDiameterHeader()	//��ȡdiameter��Ϣͷ
    {
    	return m_oMsg->hdr;
    }

    int getDiameterHeaderRequest()
    {
    	 return m_oMsg->hdr.flags.r;
    }

    virtual int parseRawToApp(void *buf, int bufSize, ParseOption option = PARSE_STRICT); //����Ocp��Ϣ
    virtual int parseRawToApp(std::string &buf, ParseOption option = PARSE_STRICT); //����Ocp��Ϣ

    virtual int parseAppToRaw(void *buf, int bufSize, bool output=false, bool bad_answer=false, ParseOption option = PARSE_STRICT ); //ת��Ocp��Ϣ
    virtual int parseAppToRaw(std::string &buf, bool output=false, ParseOption option = PARSE_STRICT); //ת��Ocp��Ϣ

    int getGroupedAvpNums(const char *avp_name);						//��ȡGroupedAvp����
    int getGroupedAvpNums1(const char *avp_name);						//��ȡGroupedAvp����


    int setAvpValue(char *avp_name, std::string &data);			//����Avp�ַ���ֵ
    int getAvpValue(char *avp_name, std::string &data);			//��ȡAvp�ַ���ֵ

    int setAvpValue(char *avp_name, char* data)				//����Avp�ַ���ֵ
	{
		std::string str(data);
		return setAvpValue( avp_name, str);
	}

    int getAvpValue(char *avp_name, char* data)			//��ȡAvp�ַ���ֵ
	{
		std::string str(data);
		return getAvpValue( avp_name, str);
	}

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

	//��ӡavpֵ���ַ���
	int printAvpValue(char *avp_name1, std::string &data);

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
   	int i;
   	std::auto_ptr<AAAMessage> m_oMsg;  		//OCP��Ϣ�ڲ��ṹ
   	std::auto_ptr<AAADictionaryManager> m_oDm;	//OCPЭ���ֵ������
    InInformation m_oInInformation;
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
		int m_sFailedAvpCode;

		////���ܲ���Э�����
public:
		char m_sMsgBuf[4096];
		char *m_pCurr;
		int  m_iCurrLen;
		int  m_iMsgLen;

	    int  setMsgBuf(std::string &msg_buf);
		int  setMsgBuf(const char * pszBuf,int iLen);
	    void  getMsgBuf(std::string &msg_buf);

	    //int getAvpValue( std::vector<int> &vAvpCode, std::vector<int> &vAvpPos);
	    int getAvpValue( int *vAvpCode, int *vAvpPos,int iCnt);
	    int getAvpValue( int avp_code);

	    //int getAvpValue( std::vector<int> &vAvpCode, std::vector<int> &vAvpPos, ACE_INT32 &data);
	    int getAvpValue( int *vAvpCode, int *vAvpPos,int iCnt, ACE_INT32 &data);
	    //int getAvpValue( std::vector<int> &vAvpCode, ACE_INT32 &data);
	    int getAvpValue( int *vAvpCode,int iCnt, ACE_INT32 &data);
	    int getAvpValue( int *vAvpCode,int iCnt, ACE_INT64 &data);//add by xupy 2011-10-11
	    int getAvpValue( int *vAvpCode,int iCnt, float &data);//add by xupy 2011-10-11
	    int getAvpValue( int *vAvpCode,int iCnt, diameter_address_t &data);//add by xupy 2011-10-11
	    int getAvpValue( int *vAvpCode,int iCnt, ACE_UINT64 &data);

	    //int getAvpValue( std::vector<int> &vAvpCode, std::vector<int> &vAvpPos, ACE_UINT32 &data);
	    int getAvpValue( int *vAvpCode, int *vAvpPos,int iCnt, ACE_UINT32 &data);
	    //int getAvpValue( std::vector<int> &vAvpCode, ACE_UINT32 &data);
	    int getAvpValue( int *vAvpCode, int iCnt, ACE_UINT32 &data);

	    //int getAvpValue( std::vector<int> &vAvpCode, std::vector<int> &vAvpPos, std::string &data);
	    int getAvpValue( int *vAvpCode, int *vAvpPos,int iCnt, std::string &data);
	    //int getAvpValue( std::vector<int> &vAvpCode, std::string &data);
	    int getAvpValue( int *vAvpCode,int iCnt, std::string &data);

	    //int getAvpValue( std::vector<int> &vAvpCode, std::vector<int> &vAvpPos, ACE_UINT64 &data);
	    int getAvpValue( int *vAvpCode, int *vAvpPos,int iCnt,  diameter_address_t &data); //add by xupy 2011-10-11
	    int getAvpValue( int *vAvpCode, int *vAvpPos,int iCnt,  float &data); //add by xupy 2011-10-11
	    int getAvpValue( int *vAvpCode, int *vAvpPos,int iCnt,  ACE_INT64 &data); //add by xupy 2011-10-11
	    int getAvpValue( int *vAvpCode, int *vAvpPos,int iCnt,  ACE_UINT64 &data);
	    int getAvpValue( std::vector<int> &vAvpCode, ACE_UINT64 &data);

	    int getAvpValue( int avp_code, int pos, ACE_INT32 &data);
	    int getAvpValue( int avp_code, int pos, ACE_UINT32 &data);
	    int getAvpValue( int avp_code, ACE_INT32 &data);
	    int getAvpValue( int avp_code, ACE_UINT32 &data);

	    int getAvpValue( int avp_code, int pos, std::string &data);
	    int getAvpValue( int avp_code, std::string &data);

	    int getAvpValuePos( int curr_avp_code, int pos, int &counter); //��ȡָ��avp_code�µ�ֵλ��
	    AAADiameterHeader& getDiameterHeaderX();	//��ȡdiameter��Ϣͷ
	    
	    void setDictHandleX(std::string serviceContextId,int iReqType,bool output=false);
	    int parseRawToAppCCA( void *buf, int bufSize,std::string serviceContextId,int iReqType, ParseOption option=PARSE_STRICT);
	    int changeMsgBuf(char* buf,int iLen,int avp_code,std::string avp_data,std::string& newBuf);
	    	
	    /////////////////////////////////////////////////////////////////
	    void setDiameterHeaderX(int code,int request = true,
                      int appId = AAA_BASE_APPLICATION_ID,unsigned int hh=0,
                      unsigned int ee=0 ,int set_or_not = false) ;
                      
      void setHeaderLen();
      void setAvpValueX(int avp_code,int vendor_id, std::string data,int mflag = true,int align = false);
      void setAvpValueXWithNotSetLen(int avp_code,int vendor_id, std::string data,int mflag=false,int align = false);
      void setAvpValueX(int avp_code,int vendor_id,unsigned int data,int mflag=true);
      void setAvpHeadWithNotData(int avp_code,int vendor_id,int avp_len,int mflag=false);
      void setAvpValueX(int* avp_code,int* vendor_id, int iCnt,std::string data,int mflag=false,int align = false);	    	
};
#endif
