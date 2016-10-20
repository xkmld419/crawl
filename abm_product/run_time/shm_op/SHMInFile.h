#ifndef __SHM_IN_FILE_H_INCLUDED_
#define __SHM_IN_FILE_H_INCLUDED_

class ABMException;

class SHMInFile
{

public:

    SHMInFile();

    ~SHMInFile();
    
    //��ʼ�� �ɹ�����0 ʧ�ܷ���-1
    int init(ABMException &oExp);
    
    //�����ڴ���HCODE��Ϣд���ļ� �ɹ�����д������ ʧ�ܷ���-1
    long hcodeInFile(const char *pDir, long lBatchID);
    
    //�����ڴ��е�MIN��Ϣд���ļ� �ɹ�����д������ ʧ�ܷ���-1
    long minInFile(const char *pDir, long lBatchID);
};

#endif/*__SHM_IN_FILE_H_INCLUDED_*/
