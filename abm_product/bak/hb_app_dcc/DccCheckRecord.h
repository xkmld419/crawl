#ifndef DCC_CHECK_RECORD_H_HEADER_INCLUDED
#define DCC_CHECK_RECORD_H_HEADER_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <iostream>

#include "StdEvent.h"
#include "Process.h"
#include "EventToolKits.h"
#include "interrupt.h"
#include "AttrTransMgr.h"

using namespace std;
typedef std::map<int,int> TRANSFILEMAP;
typedef std::map<int,std::vector<int> > EVENTATTRIDMAP;

/*
�������ҵ���߼���(CCR�����)
<Session-Id>|{hop_by_hop}|{end_to_end}|{Auth-Application-Id}|{CC-Request-Type}|{CC-Request-Number}|{Service-Context-Id}^^^����1^^^����2^^^...^^^����n
<Session-Id>|{hop_by_hop}|{end_to_end}|{Auth-Application-Id}|{CC-Request-Type}|{CC-Request-Number}|{Service-Context-Id}^^^02|2|818233|818233|USA|USA02|0|0|20030101|20350101|20000101^^^03|2|818233|818233|USA|USA02|0|0|20030101|20350101|20000101^^^02|4|818233|818233|USA|USA02|0|0|20030101|20350101|20000101
ҵ���߼�������͸����������Ҫ���CCA����
<Session-Id>|{hop_by_hop}|{end_to_end}|{Auth-Application-Id}|{CC-Request-Type}|{CC-Request-Number}^^^Service-Result-Code|Para-Field-Result^^^...^^^Service-Result-Code|Para-Field-Result
<Session-Id>|{hop_by_hop}|{end_to_end}|{Auth-Application-Id}|{CC-Request-Type}|{CC-Request-Number}^^^Service-Result-Code|F50^^^Service-Result-Code|F50^^^Service-Result-Code|F20

���͸������ccr���ַ����ṹ��Ҫ
<Session-Id>|{application_id}|{hop_by_hop}|{end_to_end}|{Destination-Realm}|{Auth-Application-Id}|{Service-Context-Id}|{CC-Request-Type}|{CC-Request-Number}^^^����1^^^...^^^����n 

*/

// �ṹ����� ��ͷ��Ϣ 
struct PackageHeadInfo
{
	string m_sSessionId;         //��ѡ�ҵ�һ�����֡��ỰID��Ψһ��ʶһ��DCC �Ự����
	string m_sHopByHop;          //Hop-by-Hop��ʶ��Ϊһ���޷���32���������ֶΣ��������ֽ�˳�򣩣���������ƥ���������Ӧ�� �����߱��뱣֤�����е�Hop-by-Hop��ʶ�����ض������������κ��ض���ʱ����Ψһ�ģ����ұ�֤�������ھ�������������ȻΨһ��Ӧ����Ϣ�ķ����߱���ȷ��Hop-by-Hop��ʶ���ֶ�ά������Ӧ��������ͬ��ֵ�� Hop-by-Hop��ʶ��ͨ���ǵ�����������֣� �俪ʼֵ��������ɵġ� һ������δ֪Hop-by-Hop��ʶ����Ӧ����Ϣ���뱻����
	string m_sEndToEnd;          //�˵��˱�ʶ����һ���޷���32���������ֶΣ��������ֽ�˳�򣩣���������ظ���Ϣ�� ������ʵʩ�������ø�λ12����Ϊ������ǰʱ��ĵ�λ12���أ� ��λ20����Ϊ���ֵ��������Ϣ�ķ����߱���Ϊÿһ����Ϣ����һ��Ψһ�ı�ʶ���� �ñ�ʶ������ά�ֱ���Ψһ����4���ӣ���ʹ������������Ӧ����Ϣ�������߱���ȷ���ö˵��˱�ʶ���ֶΰ�������Ӧ��������ͬ��ֵ���˵��˱�ʶ�������Ա�Diameter�������κ�ԭ���޸ġ�Դ����AVP��Origin-Host���͸��ֶεĽ�Ͽ������ڼ���ظ����ظ�����������ͬӦ��Ĵ��䣬���Ҳ�����Ӱ���κ�״̬�����ã�������ԭʼ����ʱ��Ӧ���ڱ��ر��������ظ���Ӧ����Ϣ���ᱻ����
	string m_sAuthApplicationId; //��������֤/��Ȩ��Ӧ��Ψһ��ʶ  DCCA 4
	string m_sCCRequestType;     //����CCR ��Ϣ��ԭ�򡣻Ự�����Ϣ��һ���ỰID���ж��CCR����Ϣ����INITIAL_REQUEST 1 UPDATE_REQUEST             2      TERMINATION_REQUEST   3   �¼������Ϣ��һ���ỰIDֻ��һ��CCR����EVENT_REQUEST 4
	string m_sCCRequestNumber;   //����Ϊ0�������һ���Ự�ж��CCR���󣬰��ջỰ�Ľ�������������	
	string m_sServiceContextId;  //һ��ҵ��������Ψһ��ʶ����Ϊ�����������ȥ�����Ѱַ�Ĺؼ��֡���������Ϊ��<ServiceName>.<CatagoryName>@<DomainName>.ChinaTelecom.com  ���磺Recharge.VC@021.ChinaTelecom.com
};



class DccCheckRecord:public Process
{
public:
	int m_iBuffSize;
	int m_iBillTypeID;                  //��ӦStdEvent�е�m_iBillingTypeID
	string m_sSignTotal,m_sSignRecord;  //�ָ���
	PackageHeadInfo m_struPackageHead;
	EventToolKits   m_evnetToolKits;
  	AttrTransMgr* m_poFormatMgr;

	char str[sizeof(StdEvent)];
	StdEvent* headStdEvent;
	string   m_seviceInformationToCCA;  //CCA���е�{Service-Information}	ҵ����Ϣ  ���ֵ�����
	string   m_strToCCA;	            //����õ�CCA��
	StdEvent m_oStdEvent;
	int m_iGroup_Id;
	long m_lCurEventID;
	
	
	vector<int> vecEVENTATTRID;
	vector<int> m_vAttrId;
	TRANSFILEMAP m_mapTransFileGroupid;
	EVENTATTRIDMAP m_mapEventAttrId;
	int m_totalNum;
	
public:
	DccCheckRecord();
	~DccCheckRecord();
	
	int run();

	int analysisStr(char *cgetBuffStr);                               //���ܣ������ַ���
	int	DealOneRecord(string strOneRecord);
	int getEventAttr(vector<int>& vecEventAttrId,int& icount,string headString,string strBuff);
	int getGroupIdByTransFileTypeId(int iTransFileTypeId);            //����TRANS_FILE_TYPE_ID��ȡGROUP_ID;��ѯTRANS_FILE_SERVICE_CONTEXT��,����iGroupId
	int getEventAttrByGroupId(vector<int> &vecEventid,int iGroupId);  //����iGroupId��ȡvecEventid, �� b_asn1_field,b_asn1_tag_seq,���ػ�õ��ֶγ���
	int getBuffSize();
	void getSign();                                                   //���b_param_define�������õķָ���
	int checkRecord();                                                //��¼��У��
	void setSessionIdToStdEvent(string strSessionId);             	  //���session_id
	void formSeviceInformationToCCA(int errID);                       //����CCA���е�{Service-Information}	ҵ����Ϣ  ���ֵ�����
	void formStrToCCA();                                              //����CCA��
	long getNextFileID();                                             //��ȡFILEID����
	long getNextDccLogID();
	void writeDccLog(char* cLogType,char* cMessageType,int iTransFileTypeId,char* cRemark);

	void SendFileEndEvt(int iFileID);                                 //�����ļ������źź�ǿ���ύ�¼�
	void replaceSign(string strfield,string& resultStr );
	long getNextEventID();
	long getNextEventSeq();
	int loadTransFileGroupid();	//װ��trans_file_groupid�����Ϣ
	int loadEventAttrIdByGroupid(int igroupid);	//װ��event_attr_id����Ϣ

};


#endif /* DCC_CHECK_RECORD_H_HEADER_INCLUDED */
