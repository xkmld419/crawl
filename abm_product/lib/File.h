/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef FILE_H_HEADER_INCLUDED_C02619C0
#define FILE_H_HEADER_INCLUDED_C02619C0

//##ModelId=41DCAC7D03DD
//##Documentation
//## �ļ���
class File
{
  public:
    //##ModelId=41DCAC7E0081
    //##Documentation
    //## ���캯��(��·���ļ���)
    File(char *pFileName = "");

    //##ModelId=41DCAC7E0083
    //##Documentation
    //## ���캯��(·��, �ļ���)
    File(char *pFilePath, char *pFileName);

    //##ModelId=41DCAC7E008E
    //##Documentation
    //## ��������
    ~File();

    //##ModelId=41DCAC7E008F
    //##Documentation
    //## �ƶ��ļ�,�ҵ�ǰ������ļ���Ҳ��Ϊ���ļ�
    bool moveFile(
	//##Documentation
	//## ����Ҫ���Ŀ�괮: 
	//## example:
	//## �ļ���:        aaa.txt
	//## ȫ·���ļ���   /home/bill/aaa.txt
	//## ���·���ļ��� dir1/dir2/aaa.txt
	//## ����·��       /home/bill/
	//## ���·��       dir1/dir2/
	char *newName = "");

    //##ModelId=41DCAC7E0097
    //##Documentation
    //## ɾ�����ļ�
    bool rm();

    //##ModelId=41DCAC7E0098
    //##Documentation
    //## ȡ�õ�ǰ�ļ�������ļ���
    char *getFileName();

    //##ModelId=41DCAC7E0099
    //##Documentation
    //## ȡ�õ�ǰ�ļ�������ļ�·��
    char *getFilePath();

    //##ModelId=41DCAC7E00A0
    //##Documentation
    //## ȡ�õ�ǰ�ļ������ȫ�ļ���, ��·��
    char *getFullFileName();

    //##ModelId=41DCAC7E00A1
    //##Documentation
    //## ȡ�õ�ǰ�ļ�������ļ���С
    int getSize();

    //##ModelId=41DCAC7E00A2
    //##Documentation
    //## set��ǰ�ļ�������ļ���С
    int setSize(int size);

    //##ModelId=41DCAC7E00AA
    //##Documentation
    //## ���û��޸��ļ���
    void setFileName(
	//##Documentation
	//## ���ļ���
	char *pFileName = "");

    //##ModelId=41DCAC7E00AC
    //##Documentation
    //## �����ļ�.
    bool copyFile(
	//##Documentation
	//## Ŀ���ļ���. �ο�moveFile�Ĳ���
	char *newFile = "");

    //##ModelId=41DCAC7E00B5
    //##Documentation
    //## �õ�����
    char *getError();

    //##ModelId=41DCAC7E00B6
    //##Documentation
    //## ȡ���ļ�״̬
    char *getState();

    //##ModelId=41DCAC7E00B7
    //##Documentation
    //## �����ļ�״̬
    bool setState(
	//##Documentation
	//## ״̬
	char *pState = "");

  protected:
    //##ModelId=41DCDA41029A
    //##Documentation
    //## �ļ�·��
    char m_sFilePath[100];

    //##ModelId=41DCDA410312
    //##Documentation
    //## �ļ���(����·��)
    char m_sFileName[100];

    //##ModelId=41DCDA410362
    //##Documentation
    //## ��·���ļ���
    char m_sFullFileName[200];

    //##ModelId=41DCDA4103BC
    //##Documentation
    //## �ļ���С(�ֽ�)
    int m_iFileSize;

  private:
    //##ModelId=41DCAC7E00B9
    //##Documentation
    //## �����·���ļ����е�·�����ļ���(�ڲ�����)
    bool getPathAndFile();

    //##ModelId=41DCDA420024
    //##Documentation
    //## ��Ŷ�������еĴ���
    char m_sErrorStr[200];

    //##ModelId=41DCDA420074
    //##Documentation
    //## �ļ�״̬
    char m_sState[10];

};



#endif /* FILE_H_HEADER_INCLUDED_C02619C0 */
