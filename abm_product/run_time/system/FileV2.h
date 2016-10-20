#ifndef __FILE_V2_H_INCLUDED_
#define __FILE_V2_H_INCLUDED_

#include <stdio.h>

class FileV2 
{

#define OPEN_ASCII 0  
#define OPEN_BIN   1

public:

	FileV2();
	
	~FileV2();
	
	int openFile(char const* sFile, int iMode=OPEN_BIN);
	
	template <typename T> int writeFile(T &data) {
        return write((void *)&data, sizeof(T));
    }
	
	template <typename T> int readFile(T &data) {
        return read((void *)&data, sizeof(T));
    }
	
	int closeFile();

    int write(void *pAddr, int iSize);

    int read(void *pAddr, int iSize);
    
    //���д�ļ�ʱ д������  ���ļ���Ч
    int getCnt() {
        return m_lCnt;    
    }
    
    //���д�ļ� д�Ĵ�С�� ���ļ���Ч
    int getSize() {
        return m_lSize;    
    }

    FILE* getFd() {
        return m_poFp;
    }
private:    

	FILE *m_poFp;
	
	int m_lCnt;
	
	long  m_lSize;	
};

#endif/*__FILE_V2_H_INCLUDED_*/
