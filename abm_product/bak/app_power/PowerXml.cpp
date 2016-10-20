/* ����Զ�̳���ʵʱ�ӿ�
   xianggs 2010-11-01
   ʵ��ҵ��1������Զ�̲�ѯ�û���Ϣ(���״��룺cc_qryuserinfo)
             2��������ѯ�û���������(���״��룺cc_qryfluxinfo)
             3��������ѯ�û���������(���״��룺cc_qrylastmonthflux)
             4��������ѯ�û���HLR��״̬��NOC��IP������������
                 �����ύ��ѯ��Ϣ���Ʒ��������ز�ѯ��ˮ(���״��룺cc_qryHLRformnum)
                 �����ٴ��첽�ύ��ѯ��Ϣ���ƷѰ���ѯ��ˮ�����ϴβ�ѯ��Ϣ(���״��룺cc_qryHLRinfo)
   �Ʒ��ڲ�ѯHLR��NOCʱ���õ�gsoap������Webservice�ṩ�ķ���              
*/
#include "PowerXml.h"

using namespace std;

PowerXml::PowerXml()
{
	m_pXmlLogID = new SeqMgrExt("SEQ_XML_LOG_ID",50);
	memset(m_sFilePath,0,sizeof(m_sFilePath));
	if( !ParamDefineMgr::getParam("POWER_MAIN", "FILE_SAVE_PATH", m_sFilePath) )
		 strcpy(m_sFilePath,"./log");
		
	if( access(m_sFilePath, F_OK) != 0 )
	{
		 char sTemp[256] = "mkdir -p ";
		 strcat(sTemp, m_sFilePath);
		 system(sTemp);
	}
//	pParserXml=new ParserXml();

}

PowerXml::~PowerXml()
{

}

//ÿ����ɺ󶼽��������һ�£�����Ӱ���´εĴ���
void PowerXml::init()
{	
	char sRespSeq[20]={0};
	memset(&m_Resp,0,sizeof(RespMsg));
	//��ʼ��
	m_Resp.m_iRespType = 0 ;
	strcpy(m_Resp.m_sRespDesc,"�ɹ�");
	strcpy(m_Resp.m_sRespCode,"0000");
	m_qry.getRespSeq(sRespSeq);
	sprintf(m_Resp.m_sRespSeq,"%s",sRespSeq);
	Date d1;
	strcpy(m_Resp.m_sRespTime,d1.toString("yyyymmddhhmiss"));

	
		memset(&pParserXml.m_pin,0,sizeof(OperationIn));	

		memset(&pParserXml.m_Hlr,0,sizeof(HLRMSG));	

}


//��Ҫ��������
int PowerXml::PowerXmlProcess(char* xmlbuf,char *retbuf)
{
	//����ǰ�ȳ�ʼ������
	init();
	//����XML��
	int iRet = pParserXml.ProcessMessage(xmlbuf,DATA_SOURCE_POWER);
	m_qry.insertOpertion(pParserXml.m_pin,m_Resp);
	//Ŀǰ�ȴ洢���ļ�ϵͳ��
	SaveXMLToFile(xmlbuf,pParserXml.m_pin.m_sReqSeq);	
	strcpy(m_Resp.m_sReqSeq,pParserXml.m_pin.m_sReqSeq);//��¼��������������
	//�ṩ���ݿ�����ҵ��״̬��������Ϣ��ѯ�ӿڡ��ɲ�ѯ���ݣ�ͨ�ſ���ͨ��ҵ����ײ���Ϣ��ѯ����ǰ����״̬��������ͣ�����պ�)��ѯ
	if (strcmp(pParserXml.m_pin.m_sProcessCode,"cc_qryuserinfo") == 0) {
		char m_sState[128];
		char sBusiName[128];
		char sPackName[128];
		
		int nRet = m_qry.queryState(pParserXml.m_pin.m_sMsisdn,sBusiName,sPackName,m_sState,m_Resp);
		memset(retbuf,0,sizeof(retbuf));
		pParserXml.PackXmlinfo(sBusiName,sPackName,m_sState,m_Resp,retbuf);
printf("%s\n",retbuf);
	 }

	//�ṩ���ݿ����뵱��ʵʱ������Ϣ�͵���ʵʱ���� ������Ϣ���к����зֱ�ͳ��
	else if(strcmp(pParserXml.m_pin.m_sProcessCode,"cc_qryfluxinfo") == 0) {
		vector<GprsAmount> v_Vec;
		char m_sPhone[60] = {0};
		strcpy(m_sPhone,pParserXml.m_pin.m_sMsisdn);
		strcat(m_sPhone,"|");
		strcat(m_sPhone,pParserXml.m_pin.m_sEndDate);

		int nRet = m_qry.queryGprs(m_sPhone,v_Vec,m_Resp);
		memset(retbuf,0,sizeof(retbuf));
		pParserXml.PackXmlFluxinfo(v_Vec,m_Resp,retbuf);
printf("%s\n",retbuf);			
	}

	//�ṩ������ҵ�����룬��ѯ�ú����ύʱ�������·ݵ�����GPRS����������Ϣ
	else if(strcmp(pParserXml.m_pin.m_sProcessCode,"cc_qrylastmonthflux") == 0) {
		long sumAmount = 0;
		int nRet = m_qry.queryLastGprs(pParserXml.m_pin.m_sMsisdn,sumAmount,m_Resp);
		memset(retbuf,0,sizeof(retbuf));
		pParserXml.PackXmlLastTotalinfo(sumAmount,m_Resp,retbuf);
printf("%s\n",retbuf);	
	}

	//�ṩ������ҵ�����룬��ѯ�ú���HLR�����ͨѶ״̬�ӿ�
	//�������̣��������Ͳ�ѯ���󣬼Ʒ��յ���Ϣ���ȡ������ˮд���������ظ�������
	else if(strcmp(pParserXml.m_pin.m_sProcessCode,"cc_qryHLRformnum") == 0) {
		//��������
			//���ɷ��ص�����XML��
			char formnum[30] = {0};
			memset(retbuf,0,sizeof(retbuf));
			m_qry.getFormNum(formnum);//��ȡһ��������ˮ�����ظ�����
			m_qry.insertFormNumLog(pParserXml.m_pin.m_sMsisdn,formnum,"","","");
			pParserXml.PackXmlHLRformnum(formnum,m_Resp,retbuf);
printf("%s\n",retbuf);			
	}
	//          �����첽���Ͳ�ѯ������󣬼ƷѸ��ݽ�����ˮ����ѯHLR���صĽ������������
	else if(strcmp(pParserXml.m_pin.m_sProcessCode,"cc_qryHLRinfo") == 0) 
	{
		HlrFormnum hlr;
		memset(&hlr,0,sizeof(HlrFormnum));
		m_qry.getHLRinfo(pParserXml.m_pin.m_sFormnum,hlr,m_Resp);
		strcpy(pParserXml.m_pin.m_sMsisdn,hlr.m_sMsisdn);//��ʱ�������ͺ��룬��Ҫ���Ǵӱ���ȡ��
		memset(retbuf,0,sizeof(retbuf));
		pParserXml.PackXmlHLRInfo(hlr,m_Resp,retbuf);
printf("%s\n",retbuf);
	}	
	else {
		Log::log(0,"δ֪�Ľ�����ˮ");
		return -1;
	}
	
	return 0;
}

void PowerXml::SaveXMLToFile(const char *xmlbuf,char * req_seq)
{
	char sTemp[9] = {0},sPath[256] = {0},sFileName[256]={0};
	Date cur;
	cur.getTimeString(sTemp,"yyyymmdd");
  sprintf(sPath,"%s/%s",m_sFilePath,sTemp);
  
	if(access(sPath,F_OK) != 0)
	{
		if(mkdir(sPath, 0777) < 0 )
		{
			Log::log(0,"SaveXMLToFile failed! mkdir %s error!\n",sPath);
			return ;
		};
		chmod(sPath,0777);
	}

	sprintf(sFileName,"%s/%s.xml",sPath,req_seq);
	FILE *fp;
	if( (fp=fopen(sFileName,"a+")) == NULL)
	{
		Log::log(0,"SaveXMLToFile failed! fopen file error!\n");
		return;
	}
	
	fprintf(fp,"%s",xmlbuf);
	fclose(fp);

}

//�洢�����ݿ��У���Ҫ��liboci/TOCIQuery.cpp���Ӷ�lob�ֶβ�����������������������Կ�������OCIQuery.cpp
//�����Ŀǰ���������ϲ����д������Ƿ񡣡�����
void  PowerXml::SaveXML(const char *xmlbuf,char * req_seq)
{
//	//����־�������һ����¼
//	char sSQL[512];      
//	long XML_LogID = m_pXmlLogID->getNextVal();
//	DEFINE_QUERY(qry);
//	sprintf(sSQL,"INSERT INTO Power_Xml_Log(XML_LOG_ID,REQ_SEQ_ID,XML_TEXT,CREATE_DATE)"
//	             "VALUES(%ld,%s,empty_clob(),sysdate)",XML_LogID,req_seq);
//	qry.setSQL(sSQL);
//	qry.execute();
//	qry.close();
//	
//	//��XML�����浽���CLOB��
//	if( *xmlbuf )
//	{
//	  sprintf(sSQL,"SELECT XML_TEXT FROM xmlbuf WHERE XML_LOG_ID = %ld for update",XML_LogID);
//	  qry.setSQL(sSQL);
//	  qry.open();
//	  qry.next();
//	  qry.field(0).loadFromBufferOnce((unsigned char*)xmlbuf);
//	  qry.commit();
//	  qry.close();
//	} catch(TOCIException &ex) 
//  {
//  	Log::log(0,"SaveXML failed! req_seq = %s! "
//  		         "TOCIException catched! \n"                      
//  		         "\t *Err: %s\n"                                  
//               "\t *SQL: %s\n"
//               ,req_seq, ex.getErrMsg(), ex.getErrSrc()
//             );
//             
//    qry.rollback();
//    SaveXMLToFile(xmlbuf,lOrderListId,sAreaCode);
//  }
//  catch(...)
//  {
//  	Log::log(0,"SaveXML failed! Unknown Exception! req_seq = %s!\n",req_seq);
//  
//    qry.rollback(); 
//    		
//  	SaveXMLToFile(xmlbuf,lOrderListId,sAreaCode);
//  }
//  qry.close();
//
}

