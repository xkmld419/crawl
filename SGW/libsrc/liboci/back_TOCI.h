#ifndef TOCI_H__
#define TOCI_H__

#include <oci.h>
#include <oci8dp.h>
#include <stdio.h>
#include "./exception.h"

//classes delcared in this file
class TOCIException;
class TOCIDatabase;
//classes use in this file
class TOCIDataSet;	//use in TOCIDataSet.h
class TOCIDirPath;	//use in TOCIDirPath.h

class TOCIException
{
public:
	char *GetErrMsg() const;
	char *GetErrSrc() const;

public:
	TOCIException(sword errNumb, OCIError *err, char *cat, char *sql);//ִ��OCI���������Ĵ���
	TOCIException(const char *sql, const char* errFormat, ...);	
	~TOCIException();

private:
	char	errCategory[MAX_ERR_CAT_LENGTH+1];		//�������
	text	errMessage[MAX_ERRMSG_LENGTH+1];		//������Ϣ
	char	errSQL[MAX_SQLSTMT_LENGTH+1];			//���������sql���
	int	errNo;										//�����
};




