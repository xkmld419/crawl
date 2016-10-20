#ifndef __TBATCH_INSERT_H__
#define __TBATCH_INSERT_H__

#include "TOCIQuery.h"
#include "TOCIDirPath.h"
#include <string.h>

#define LOAD_BUFFER_SIZE  50000

//class TBatchInsert;

template <class  TDataStruct > 
class TBatchInsert 
{
protected:
    int   iBufferSize;                  //�����С
    int   iRecordCount;                 //�����¼��
    TOCIDatabase * db_connection;       //���ݿ�����
    char sTableName[81];                //����
    TDataStruct * pBuffer;
    void  AllocBuffer();       //���仺��
    void  FreeBuffer();        //�ͷŻ���
public:
	TBatchInsert( TDataStruct & );
	~TBatchInsert();
    void  SetTableName(char * sName);       //���ñ���
    void  SetConnection(TOCIDatabase * db);  //�������ݿ�����

    virtual void  Save();              //�����ṹ�뻺��
    virtual void  Flush();             //���ݻ������
};

#endif



