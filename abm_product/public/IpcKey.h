/*VER: 1*/ 
#ifndef __IPC__KEY__H___
#define __IPC__KEY__H___

#include "TOCIQuery.h"

class IpcKeyMgr {
  public:
    /*
        ��ȡϵͳ�ж���� ��ǰ����(ͨ��hostname���û�ȡ)��
            ��ǰ����ϵͳ�û�(ͨ����������LOGNAME��ȡ)��
            ָ�����̡�IPC���� ��Ӧ��IPC_KEY ֵ
        IPC_KEY �����ڱ� B_IPC_CFG ��
        �ɹ�, ����ֵΪ��ȡ�� IPC_KEY
        ʧ��, ����ֵ С�� 0
    */
    static long getIpcKey (int _iBillFlowID, char const * _sIpcName);
    static long getIpcKeyEx (int _iBillFlowID, char const * _sIpcName);
	static long getIpcKey(bool bForceBillFlow,int _iBillFlowID, char const * _sIpcName);
  static long getNextIpcKey(int iHostID,char const * pUserName,char const * _sIpcName,long lKeyBegin,long lKeyEnd);
    // ����ȡ��ǰ���̶�Ӧ����������, ���Ҳ���, ������IDΪ -1 ��.(��ϵͳ��������)
    static long getIpcKey (char const * _sIpcName);
    
    // �ɹ�: ���ص�ǰ������ID, ��������_sHostName
    // ʧ��: ���� -1
    static int getHostInfo (char *_sHostName);
	static int getHostInfo (char *_sHostName,TOCIQuery &qry);

    /* 
     *   ���ϵͳ����Ҫ�ĵ�ǰ����,��ǰ����ϵͳ�û�,ָ������,IPC���ƶ�Ӧ��ֵ 
     */
    static bool setIpcKey(int _iBillFlowID, char const * _sIpcName,char const * sRemark, int _iIpcKey);
    static bool setIpcKey(char const * _sIpcName,char const * sRemark);
    static bool seekIpcKey(int iHostID,long lIpcKey);
};

#endif

