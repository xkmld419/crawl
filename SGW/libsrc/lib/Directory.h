// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.
#ifndef DIRECTORY_H_HEADER_INCLUDED_C02647E2
#define DIRECTORY_H_HEADER_INCLUDED_C02647E2
class File;
#include "File.h"
#include <dirent.h>


//##ModelId=41DCAC7D03BF
//##Documentation
//## Ŀ¼��
class Directory
{
  public:
    //##ModelId=41DCAC7E0028
    //##Documentation
    //## ������(·��)
    Directory(char *sPath);

    //##ModelId=41DCAC7E0031
    //##Documentation
    //## �����캯��(����·������,ȡ��ǰ·��)
    Directory();

    //##ModelId=41DCAC7E0032
    //##Documentation
    //## ����
    ~Directory();

    //##ModelId=41DCAC7E0033
    //##Documentation
    //## ����Ŀ¼(֧�ֶ༶Ŀ¼)
    bool makeDir(char *subDir = "");

    //##ModelId=41DCAC7E003B
    //##Documentation
    //## ȡ�õ�ǰĿ¼���ļ��б�
    int getFileList();

    //##ModelId=41DCAC7E003C
    //##Documentation
    //## �ͷ�Ŀ¼������ļ��б�
    int freeFileList();

    //##ModelId=41DCAC7E003D
    //##Documentation
    //## ȡ�õ�ǰĿ¼���ļ�����
    int getFileNum();

    //##ModelId=41DCAC7E003E
    //##Documentation
    //## ȡ�õ�i���ļ�
    File *getFile(
	//##Documentation
	//## �±�
	int index = 0);

    //##ModelId=41DCAC7E0046
    //##Documentation
    //## ȡ��Ŀ¼��
    char *getPathName();

    //##ModelId=41DCAC7E0047
    //##Documentation
    //## ����Ŀ¼�е��ļ�,ȡ��һ��.
    File *nextFile();



  private:
    //##ModelId=41DCDA490010
    //##Documentation
    //## Ŀ¼�е��ļ�����
    int m_iFileNum;

    //##ModelId=41DCDA49009C
    //##Documentation
    //## ·����
    char m_sPathName[100];

    //##ModelId=41DCDA4900D9
    //##Documentation
    //## Ŀ¼�е��ļ��б�
    File *m_poFileList;

    //##ModelId=41DCDA4900E3
    //##Documentation
    //## ����ʱ��ǰ�ļ�
    File m_oCurFile;

    //##ModelId=41DCDA8001FA
    //##Documentation
    //## Ŀ¼ָ��
    DIR *m_pDir;



};



#endif /* DIRECTORY_H_HEADER_INCLUDED_C02647E2 */



