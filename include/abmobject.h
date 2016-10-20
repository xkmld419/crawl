/*
*   version abm-center 1.0 at 20110504
*/
#ifndef    __ABMOBJECT_H_INCLUDED_
#define    __ABMOBJECT_H_INCLUDED_

#include <vector>
#include <memory.h>
#include <iostream>
#include "LogV2.h"

//using namespace std;

#define ABMCMD_BUF_MAX  61440
#define ABMCMD_DCC_MAX  sizeof(struDccHead)
#define ABMCMD_OFFSET   (ABMCMD_DCC_MAX + 40)
#define ABMCMD_BUF_SIZE (ABMCMD_BUF_MAX-ABMCMD_OFFSET)

typedef long        __DFT_T1_;
typedef __DFT_T1_   __DFT_T2_;
typedef __DFT_T1_   __DFT_T3_;


//Dcc��Ϣ��������
struct struDccHead {
    char m_sSessionId[64];    //һ�λỰ��Ψһ��ʶ�������������Ϣ��Ҫҵ����д
    char m_sSrvTextId[64];    //ͨ������ֶ�����ҵ����Ϣ����
    char m_sOutPlatform[64];  //SGW������д�ⲿƽ̨�������Ϣ
    
    char m_sOrignHost[64];
    char m_sOrignRealm[32];
    char m_sDestRealm[32];   //�����������Ϣ��Ҫҵ����д
    
    char m_sSrvFlowId[192];  //�����ʽ��sn-qqqwe-aaabbbcccddd-version-flowid-track
                             //          24   5     12         max<40>   6    <>;<>;.... 
    int  m_iAuthAppId; 
    int  m_iReqType;
    int  m_iReqNumber;
    int  m_iResultCode;
	unsigned int m_iHH;
    unsigned int m_iEE;
    
#ifdef _DEBUG_DCC_    
    void print()
    {
        cout<<"==========================="<<endl;
        cout<<"public message of dcc head:"<<endl;
        cout<<"  session-id---------"<<m_sSessionId<<endl;
        cout<<"  service-context-id-"<<m_sSrvTextId<<endl;
        cout<<"  outp-latform-id----"<<m_sOutPlatform<<endl;
        cout<<"  orgin-host---------"<<m_sOrignHost<<endl;
        cout<<"  orgin-realm--------"<<m_sOrignRealm<<endl;
        cout<<"  dst-realm----------"<<m_sDestRealm<<endl;
        cout<<"  Service-Flow-Id----"<<m_sSrvFlowId<<endl;
        cout<<"  auth-app-id--"<<m_iAuthAppId<<",request-type--"<<m_iReqType
            <<",request-number--"<<m_iReqNumber<<",result-code--"<<m_iResultCode<<endl;
        return;
    }
#endif

};

/***************************************************************************
*	������ABMObject
*	���ܣ�������ABM�������ݽṹ�壬��Ϊ���պͷ��͵�����
***************************************************************************/

class ABMObject 
{
    
public:
    
    ABMObject() {};
	
	ABMObject(int iCmd): m_iCmdID(iCmd) {};
	    
    void clear() {
        m_iSize = ABMCMD_OFFSET;
        m_iFirNum = 0;
        m_iSecNum = 0;
        m_iFlag = 0;
        m_iRltCode = 0;
    }

    void memcpyh(ABMObject const *pSrc) {
        if (pSrc != NULL) {
            memcpy((void *)&m_lType, (const void *)pSrc, ABMCMD_OFFSET);
            clear();
        }    
    }

    int getRltCode() {
        return m_iRltCode;
    }

    void setRltCode(int iCode) {
        m_iRltCode = iCode;
    }

public:	    
	/*��Ϣ����*/
	long m_lType;  
	
	/*dcc��Ϣͷ*/
	char m_sDccBuf[ABMCMD_DCC_MAX];  
	
	/*����ID, ��־ABMϵͳ�ڵ�һ�ִ���ҵ��*/
	int m_iCmdID;
	
	/*��Ϣ��С, ��־������Ϣ���ܴ�С*/
	int m_iSize;

protected:
    	
	int m_iFirNum;
	
	int m_iSecNum;
	
	int m_iFlag;
	
	int m_iRltCode;
	
	long *m_plNum;
	
	char m_sDataBuf[ABMCMD_BUF_SIZE];
	
};

class ABMCCR : public ABMObject
{
};

class ABMCCA : public ABMObject
{
}; 

/**************************************************************************
*   ������ABMTpl
*   ���ܣ�������ABM���������ģ����
*   ˵����
*       1. __clear(): ���ݽṹ���ʼ��
*       2. __add() :  ��װ�ۻ���������
*       3. __getValue(): ��ȡ�������е��ۻ�������
***************************************************************************/

template <typename T1, typename T2, typename T3, int CMD>
class ABMTpl : public ABMObject
{
    
public:
    
    ABMTpl() : ABMObject(CMD) {
    	clear();
    };
    
protected:  
    
    bool __add1(T1 &data) {
        if((m_iSize+sizeof(T1)) > ABMCMD_BUF_MAX)
            return false;
        if (m_iFlag != 0)
            return false;        
        memcpy((void *)&m_sDataBuf[m_iSize-ABMCMD_OFFSET], (const void *)&data, sizeof(T1));
        m_iSize += sizeof(T1);
        m_iFirNum++;
        return true;
    };
    
    bool __add2(T2 &data) {      
        if((m_iSize+sizeof(T2)+sizeof(long)) > ABMCMD_BUF_MAX)
            return false;
            
        m_iFlag = 1;     
   __DEBUG_LOG1_(0, "add2  size=[%d]",sizeof(T2));
        memcpy((void *)&m_sDataBuf[m_iSize-ABMCMD_OFFSET], (const void *)&data, sizeof(T2));
        m_iSize += sizeof(T2);
   __DEBUG_LOG1_(0, "add2  size=[%d]",sizeof(T2));
        m_iSecNum ++;
        
        m_plNum = (long *)&m_sDataBuf[m_iSize-ABMCMD_OFFSET];
        *m_plNum = 0;
        m_iSize += sizeof(long);
        return true;
    }
    
    bool __add3(T3 &data) {
        if((m_iSize+sizeof(T3)) > ABMCMD_BUF_MAX)
            return false;
        if (m_iFlag == 0) 
            return false;
        (*m_plNum) ++;
        memcpy((void *)&m_sDataBuf[m_iSize-ABMCMD_OFFSET], (const void *)&data, sizeof(T3));
        m_iSize += sizeof(T3);  
        return true;        
    }
    
    int __getValue1(vector<T1 *> &vec) {
    	int _size;
        for (int i=0; i<m_iFirNum; ++i) {
        	_size = i*sizeof(T1);
        	if (_size >= ABMCMD_BUF_SIZE)
        		return -1;
            vec.push_back((T1 *)&m_sDataBuf[_size]);  
        }   
        return vec.size();                  
    }
    
    //����ϸ��
    int __getValue2(vector<T2 *> &vec) {
        long *plNum;
        if (m_iSecNum == 0)
            return 0;
    	else if (m_iSecNum < 0)
    		return -1;
        //���T2����ʼ��ַ
        int _size = m_iFirNum*sizeof(T1);
        if (_size >= ABMCMD_BUF_SIZE) 
            return -1;
        vec.push_back((T2 *)&m_sDataBuf[_size]);
        
        for (int i=1; i<m_iSecNum; ++i) {
            _size += sizeof(T2);  
            //���T2��ϸT3�ĸ���
            plNum = (long *)&m_sDataBuf[_size];
            //�����һ��T2�ĵ�ַ
            _size += ((*plNum) * sizeof(T3) + sizeof(long));
            if (_size > ABMCMD_BUF_SIZE) 
                return -1; 
            vec.push_back((T2 *)&m_sDataBuf[_size]);    
        }   
        
     	_size += sizeof(T2);
     	plNum = (long *)&m_sDataBuf[_size];
     	_size += ((*plNum) * sizeof(T3) + sizeof(long));
     	if (_size > ABMCMD_BUF_SIZE) 
     	    return -1;
        return vec.size();      
    }
    
    //�����Ż��ۻ����ĵ�ַ ��ȡ��ص���ϸ
    long __getValue3(T2 *addr, vector<T3 *> &vec) {
        long *plNum;
        plNum = (long *) (addr+1);
        T3 *pData = (T3 *)(plNum+1);
        for (int i=0; i<*plNum; ++i) {
            vec.push_back(pData);
            pData += 1; 
        }   
        return vec.size();
    }
 
};   

#endif/*__ABMOBJECT_H_INCLUDED_*/

