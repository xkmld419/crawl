/*VER: 1*/ 
#ifndef NETWORKMGR_H_HEADER_INCLUDED_BDB745F9
#define NETWORKMGR_H_HEADER_INCLUDED_BDB745F9
/*
**
**  ����������Դ����ȷ����������ƽ̨�����ȼ�˳��
**  ���ִ�����ȼ���ߣ�����ƥ���û�����
**  ����������Դ���Ͳ���ȷ�ģ�m_iNetworkSourceIDΪ0����������ƽ̨�����ȼ���Ӹߵ��Ͳ���
**  2007.12.12 caidk 
**      created
*/

#include "CommonMacro.h"
#include "HashList.h"
class NetworkPriority
{
public:
    NetworkPriority()
    {
        Init(0,0);
    }
    NetworkPriority(int source, int networkid)
    {
    		Init(source,networkid);
    }
    bool Init(int source,int networkid)
    {
    	  m_iNetworkSourceID = source;
        m_iNetworkID = networkid;
        m_iPriority = 0;
        m_poNext = 0;    		
    		return true;
    }
    //������Դ���ͱ�ʶ
    int m_iNetworkSourceID;
    //�������ͱ�ʶ
    int m_iNetworkID;
    //���ȼ�
    int m_iPriority;
    //��һ����¼
    NetworkPriority * m_poNext;
    
    static NetworkPriority NETWORK_NONE;
    
#ifndef PRIVATE_MEMORY
	unsigned int m_iNextOffset;
#endif

};
class NetworkPriorityMgr
{
    //�����������ȼ���
    NetworkPriority * m_poNetworkPriList;
    //�����������ȼ������������m_iNetworkSourceID������
    HashList<NetworkPriority *> *m_poNetworkPriIndex;
    
    bool m_bUploaded;
    
public:
    //## ���ض���Ʒ�������������
    void load();

    //## ж�������ص���Ϣ
    void unload();
    
    NetworkPriority * getPriority(int iNetworkSourceID);
    
#ifndef PRIVATE_MEMORY
	
	NetworkPriority *getHeadNetworkPriority();

#endif 
    
    NetworkPriorityMgr();
    
    ~NetworkPriorityMgr();
};
#endif
