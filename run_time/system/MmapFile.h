#ifndef __MMAP_FILE_H_INCLUDED_
#define __MMAP_FILE_H_INCLUDED_

class ABMException;

class MmapFile
{

public:

    //sFile: Ҫӳ���ļ���·��
    //iSize: �����ڴ��С
    MmapFile(char *sFile, int iSize=0);

    ~MmapFile();

    int openMap(ABMException &oExp);  //���ļ�ӳ�䵽˽�пռ�

    //openMap�� ���øú�����ù����ڴ��׵�ַ
    void *getMap() {
        return m_pvAddr;
    }

    int synFile(void *pAddr, int iSize);    //�����ڴ���޸�ˢ�µ��ļ�

    int closeMap();                        //�ͷŹ����ڴ� 

private:

    char m_sFile[512];

    int m_iSize;

    void *m_pvAddr;
};

#endif/*__MMAP_FILE_H_INCLUDED_*/

