/*VER: 1*/ 
#ifndef __IPC__KEY__H____
#define __IPC__KEY__H____

class IpcKeyMgr 
{
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
    
    // ����ȡ��ǰ���̶�Ӧ����������, ���Ҳ���, ������IDΪ -1 ��.(��ϵͳ��������)
    static long getIpcKey (char const * _sIpcName);
    
    // �ɹ�: ���ص�ǰ������ID, ��������_sHostName
    // ʧ��: ���� -1
    static int getHostInfo (char *_sHostName);
    
};


#endif

