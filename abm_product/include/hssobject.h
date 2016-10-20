/*
*   version js at 2010-09-09
*/
#ifndef    __HSSOBJECT_H_INCLUDED_
#define    __HSSOBJECT_H_INCLUDED_

#include <arpa/inet.h>
#include <iostream>

#define CMD_BUF_MAX  6144000
#define CMD_DCC_MAX  568
#define CMD_OFFSET   (CMD_DCC_MAX + 104)
#define CMD_BUF_SIZE (CMD_BUF_MAX-CMD_OFFSET)


struct DccHeadInfo
{
	char m_sSessionID[128];
	char m_sOrignHost[64];
	char m_sSrvTextId[64];       //ͨ������ֶ�����ҵ����Ϣ����
	char m_sSrvFlowId[256];
	char m_sDestRealm[32];       //�����������Ϣ��Ҫҵ����д	
	int  m_iAuthAppId;
	int  m_iReqType;
	int  m_iReqNumber;
	int  m_iResultCode;
	int  m_iHH;
	int  m_iEE;
	
#ifdef _DEBUG_DCC_    
    void print()
    {
        std::cout<<"==========================="<<std::endl;
        std::cout<<"public message of dcc head:"<<std::endl;
        std::cout<<"  session-id--------- "<<m_sSessionID<<std::endl;
        std::cout<<"  service-context-id- "<<m_sSrvTextId<<std::endl;        
        std::cout<<"  orgin-host--------- "<<m_sOrignHost<<std::endl;
        //std::cout<<"  orgin-realm-------- "<<m_sOrignRealm<<std::endl;
        std::cout<<"  dst-realm---------- "<<m_sDestRealm<<std::endl;
        std::cout<<"  Service-Flow-Id---- "<<m_sSrvFlowId<<std::endl;
        std::cout<<"  auth-app-id-- "<<m_iAuthAppId<<",request-type-- "<<m_iReqType
                 <<",request-number-- "<<m_iReqNumber<<",result-code-- "<<m_iResultCode<<std::endl;
        return;
    }
#endif
};

/***************************************************************************
*	������HSSObject
*	���ܣ������˲�ѯHSS�ۻ����Ļ������ݽṹ�壬��Ϊ���պͷ��͵�����
***************************************************************************/

class HSSObject 
{
    
public:
    
  HSSObject() {};
	
	HSSObject(int cmd): m_iCmdID(cmd) {};

    void clear()
    {
        m_iSize = CMD_OFFSET + sizeof(m_uBody.m_tNum);
        memset(&m_uBody.m_tNum, 0x00, sizeof(m_uBody.m_tNum));
    }
    
    void memcpyh(HSSObject const *pSrc) {
        if (pSrc != NULL) {
            memcpy((void *)&m_lType, (const void *)pSrc, CMD_OFFSET);
            clear();
        }
    }
		
public:
    
    long m_lType;
    
    /*dcc��Ϣͷ*/
	char m_sDccBuf[CMD_DCC_MAX];
     	
	/*��Ϣ��С, ��־������Ϣ���ܴ�С*/
	int m_iSize;

	/*����ID, ��־HSSϵͳ�ڵ�һ�ִ���ҵ��*/
	int m_iCmdID;

	/*HSS�ڲ������к�, ֻ��ͬ���ӿ�ʱ���� API�и�ֵ*/
	long m_lTransID;
	

	int m_iMsgSource;//��Ϣ��Դ��1.�ļ� ��2.DCC���������Ϣ����Դ���Ǵ��ļ�����dccת��������

	int m_iMsgType;//��Ϣ���ͣ�1.�ļ���2��Ϣ����������ļ���������Ϣ��

		/*�ͻ��˽ڵ�ID, ��ҵ�����transID��� ,
	  Ψһȷ��һ������(��socket�ͻ����и�ֵ)*/
	int m_iClntID;  

	int m_iSrvID;//����ڵ�	

	/*��HSSϵͳʱ��, ����ͳ����*/
	long m_lInTm;

	/*��HSSϵͳʱ��, ����ͳ����*/
	long m_lOutTm;

    /*���ش���*/
    int  m_iRltCode;

    /*ҵ�񼶵Ĵ����룬������ã�����������*/
    int  m_iSrvRltCode;
	int m_iFlag;
	int m_iFlag2;
	int *m_pSrvSize;
	int *m_pDataSize;
	long *m_pTableNum;
	long *m_pDataNum;
	/*��Ϣ��*/
	union {	
		/*��Ϣͷ*/
		struct {
			int m_iFirstNum;   //��ϰ��е�һ��ҵ�����Ŀ��OrderInfo��
			int m_iSecondNum;   //��ϰ��еڶ���ҵ�����Ŀ��xmlInfo��
		} m_tNum;
		
		/*������*/
		char m_sBuf[CMD_BUF_SIZE];
		
	} m_uBody;
	
};

/***************************************************************************
*   DccOrderInfo
*   ���ܣ�������DCCЭ���ж�����������ݽṹ��
***************************************************************************/



class HSSCCR : public HSSObject
{
};

class HSSCCA : public HSSObject
{
};
//typedef HSSObject HSSCCR;
//typedef HSSObject HSSCCA;

#ifdef HSS_LITTLE_ENDIAN
//�ߵ�λת��
inline long htonl64(long dig)
{   	
	union {
		long l64;
		struct {
			int a1;
			int a2;	
		} st64;	
	} u1, u2;
	
	u1.l64 = dig;
	u2.st64.a1 = htonl(u1.st64.a2);
	u2.st64.a2 = htonl(u1.st64.a1);
	return u2.l64;
};
#else
#define htonl64
#endif

#endif/*__HSSOBJECT_H_INCLUDED_*/

