#ifndef _OCIQuery_h
#define _OCIQuery_h

#include <oci.h>
//#include <stdio.h>
#include "TException.h"

typedef signed int dword;
const int PREFETCH_ROWS = 200;						//Ԥ����ȡn�����ݵ�������,������������
const	int MAX_STRING_VALUE_LENGTH = 255;			//���ص��ַ��������С����ع��̲�������
const	int MAX_LOB_BUFFER_LENGTH = 1024;			//LOB���ݻ����������ռ�
const	int MAX_ERRMSG_LENGTH = 1024;					//������Ϣ����󳤶�
const	int MAX_SQLSTMT_LENGTH = 1024;				//���ִ����SQL��䳤��
const	int MAX_PARAMS_COUNT = 100;						//���������Ŀ
const	int MAX_ERR_CAT_LENGTH = 50;					//������೤��
const	int LOB_FLUSH_BUFFER_SIZE = 400*1024;		//LOB���ݻ��۵�����ʱ��д�����ݿ�

//error message definination:
const	char* const ERR_GENERAL = "General Error: %s"; //throw TOCIException("TOCIQuery(TOCIDatabase &db)", ERR_GENERAL, "Can not declare a TOCIQuery when the database is not connected");
const	char* const ERR_INDEX_OUT_OF_BOUND = "%s";    //throw TOCIException(fSqlStmt , ERR_INDEX_OUT_OF_BOUND, "field index out of bound when call Field(i)");	
const	char* const ERR_DB_INIT = "OCI: OCI handles init fail in TDatabase constructor: @line:%d";
const	char* const ERR_SET_LOGIN = "OCI: You can only set login infomation on disconnect status: line %d";
const	char* const ERR_CONNECT_NO_LOGIN_INFO = "No login information provided before Connect(), call SetLogin first, line:%d";
const	char* const ERR_NO_DATASET = "OCI: function:%s , Result Dataset is on Bof/Eof. field:%s"; //throw TOCIException(fParentQuery->fSqlStmt, ERR_NO_DATASET, "asBlobBuffer()", name);
const	char* const ERR_DATA_TYPE_CONVERT = "Data type convertion error: field:%s data type:%d can not be access by %s"; //throw TOCIException(fParentQuery->fSqlStmt, ERR_DATA_TYPE_CONVERT, name, type, "asLobBuffer()");
const	char* const ERR_NOMORE_MEMORY_BE_ALLOCATED = "no more memory can be allocate when :%s, source code:%d"; //throw TOCIException(fParentQuery->fSqlStmt, ERR_NOMORE_MEMORY_BE_ALLOCATED, "asBlobBuffer()", __LINE__);
const	char* const ERR_FILE_IO = "%s: can not open file:%s"; //throw TOCIException(fParentQuery->fSqlStmt, ERR_FILE_IO, "loadFromFile()", fileName);
const	char* const ERR_MEM_BUFFER_IO = "asBlobWriter() error: read from file to buffer, field:%s, file:%s, @line:%d"; //throw TOCIException(fParentQuery->fSqlStmt, ERR_MEM_BUFFER_IO, name, fileName, __LINE__);
const	char* const ERR_DATA_TYPE_NOT_SUPPORT = "field:%s, datatype:%d not yet supported"; //, pCurrField->name,innerDataType);
const	char* const ERR_PARAM_NOT_EXISTS = "param:%s does not exists."; //throw TOCIException(fSqlStmt, ERR_PARAM_NO_EXISTS, paramName, "check spelling error");
const	char* const ERR_FIELD_NOT_EXISTS = "field:%s does not exists.";
const	char* const ERR_INVALID_METHOD_CALL	= "%s: invalid call method:%s";
const	char* const ERR_CAPABILITY_NOT_YET_SUPPORT = "capability not support yet:%s"; //�������������Խ��Χ
const	char* const ERR_READ_PARAM_DATA = "read parameter value data type error, parameter name:%s, method:%s";

//const define:
const char* const NULL_STRING = "";
const int NULL_NUMBER = 0;

/* classes defined in this file: */
class TOCIException;
class TOCIDatabase;
class TOCIQuery;
class TOCIField;
class TOCIParam;
class TOCISession;
class TOCIDirPath;	//define in TOCIDirPath.h

class TOCIException : public TException
{
public:
	char *getErrMsg() const;
	char *getErrSrc() const;
	int getErrCode() const;

public:
	TOCIException(sword errNumb, OCIError *err, char *cat, char *sql);//ִ��OCI���������Ĵ���
	TOCIException(const char *sql, const char* errFormat, ...);	
	~TOCIException();

private:
	char	errCategory[MAX_ERR_CAT_LENGTH+1];	//�������
	text	errMessage[MAX_ERRMSG_LENGTH+1];		//������Ϣ
	char	errSQL[MAX_SQLSTMT_LENGTH+1];			//���������sql���
	int	errNo;										//�����
	int	m_iErrCode;
};

class TOCIDatabase
{
friend class TOCIQuery;
friend class TOCISession;
friend class TOCIDirPath;
public:

public:
	TOCIDatabase();
	~TOCIDatabase();
	void setLogin(char *user, char *password, char *tnsString) ;
	bool connect();
	bool connect(char *usr, char *pwd, char *tns) ;
	void disconnect();
	void commit();
	void rollback();
	OCISession *hUser;
	//OCISvcCtx *hSvc;						//����
	OCISvcCtx *hDBSvc;		//���ڵ�¼���ӷ�����	

private:
	char *usr, *pwd, *tns;	//��¼��
	bool fConnected;			//��Connect���Ƿ����ӳɹ�
	sword fErrorNo;						//�����
	void checkError() ;					//�����жϵ�ǰ������Ƿ���ȷִ�У�����д�����Ѵ�����Ϣ����errMsg;
	
	OCIError *hDBErr;
	
	OCIEnv *hEnv;
	OCIServer *hSvr;
};

class TOCIParam
{
friend class TOCIQuery;
public:
	~TOCIParam();
	char	*name;

	int		asInteger();	//��ȡ���ز���ֵ
	double	asFloat();
	long		asLong();
	char		*asString();
	bool		isNULL();
private:
	TOCIParam();
	ub2		dataType;
	
	int		intValue;
	double	dblValue;			//�洢����������ֵ(������)
	long		longValue;			//long���ݻ�����
	char		*stringValue;		//�ַ������ػ�����
	
	int    * intArray;    //INT����
	double * dblArray;    //DOUBLE����
	long   * longArray;  //LONG����
	char ** stringArray; //STRING����
	int   stringSize;    //string�����е�string��С
	
	bool		fIsOutput;			//�Ƿ����������.Ĭ�����������
	sb2		indicator;			//�ڷ���ֵʱ���Ƿ�Ϊ��
	OCIBind  *hBind;
};

class TOCIField
{
friend class TOCIQuery;

public:	
	~TOCIField(); 
	char	*name;				//�ֶ�����
	long	size;					//���ݳ���
	long	type;					//�������� in(INT_TYPE,FLOAT_TYPE,DATE_TYPE,STRING_TYPE,ROWID_TYPE)
	int	precision;			//��ֵ�ܳ���
	int	scale;				//��ֵ��С�������
	bool	nullable;			//�ֶζ���ʱ�Ƿ�����Ϊ��ֵ--Ϊ�˺���������һ��

	bool		isNULL();			//��fetch�����и��е������Ƿ�Ϊ��
	char		*asString();
	double	asFloat();
	int		asInteger();
	long		asLong();
	//Blob����
	void		asBlobFile(const char *fileName);			//��ȡ��file��
	void		loadFromFile(const char *fileName);			//д�뵽blob��
	void		loadFromBuffer(unsigned char *buf, unsigned int bufLength);	//��LOB�������û������������
	void		asBlobBuffer(unsigned char* &buf, unsigned int *bufLength);	//���浽������,�������Ĵ�С�Զ������������ػ�������С*bufLength.
	
	//���ڴ���
	char		*asDateTimeString();	//�������͵�����HH:MM:DD HH24:MI��ʽ��ȡ,ʹ��asString()��ȡ����������������ΪHH:MM:DD
	void		asDateTime(int &year, int &month, int &day, int &hour, int &minute, int &second); //�������ڵĸ�������
private:
	//���ڴ���
	void		asDateTimeInternal(int &year, int &month, int &day, int &hour, int &minute, int &second); //�������ڵĸ�������,û��������У�飬ֻ���ڲ�����					
	TOCIQuery *fParentQuery;					//ָ���Field�����ڵ�Query
	TOCIField();
	//���ݻ�����,�ֱ�Ϊ�ַ���������������������ռ�
	ub1	fStrBuffer[MAX_STRING_VALUE_LENGTH];	//���ڱ���ת��Ϊ�ַ������ֵ
	ub1	*fDataBuf;						//�ڷ����ֶ�ʱ���ÿռ�max(���е���󳤶�,MAX_STRING_VALUE_LENGTH), ��Destructor���ͷ�
	OCILobLocator *hBlob;				//֧��LOB
	sb2	*fDataIndicator;				//��defineByPos��ʹ�ã�������fetchʱ�쿴�Ƿ����ֶ�ֵ���ء��ֶ�ֵ�Ƿ񱻽ض�;valueIsNULL, isTruncated���ݴ�ֵ��ý��

	ub2   fReturnDataLen;				//��ȡ����ʱ���ص���ʵ����
	ub2	fInternalType;					//Oracle�ڲ���������
	ub2	fRequestType;					//�ڶ�ȡ����ʱ���������������
	OCIDefine *hDefine;					//���ڶ�ȡ����Ϣ
	int	  iBlobBufCnt;					//֧��LOB����ĳ���
};

class TOCIQuery
{
friend class TOCIField;

public:
	//��Ҫ����
	void close();													//�ر�SQL��䣬��׼��������һ��sql���
	void setSQL(const char *inSqlstmt);								//����Sqlstatement
	void open(int prefetchRows=PREFETCH_ROWS);			//��SQL SELECT��䷵�ؽ����
	bool next();													//�ƶ�����һ����¼
	bool execute(int iters=1);	                          //ִ�з�SELECT���,û�з��ؽ����
	bool commit();													//�����ύ
	bool rollback();												//����ع�
	int  rowsAffected() { return fTotalRowsFetched;};	//DELETE/UPDATE/INSERT����޸ĵļ�¼��Ŀ
	int  getSQLCode() { return fErrorNo;};				//����Oracleִ�н������

	//������Ϣ���				
	int fieldCount();												//�ܹ��м�����
	TOCIField& field(int index)		;						//���ص�i������Ϣ
	TOCIField& field(char *fieldName) ;						//��������(���ִ�Сд)��������Ϣ; ����ʹ��Field(int i)��ø��ߵ�Ч��
	
	//�������Ϣ���
	TOCIParam& param(long index);								//���ص�i������Ϣ
	TOCIParam& param(char *paramName);						//��������(���ִ�Сд)��������Ϣ; ����ʹ��Field(int i)��ø��ߵ�Ч��
	int paramCount();
																		//���������ò���ֵ
	void setParameter(const char *paramName, const char* paramValue, bool isOutput = false); 	
	void setParameter(const char *paramName, int paramValue, bool isOutput = false); 
	void setParameter(const char *paramName, double paramValue, bool isOutput = false) ;
	void setParameter(const char *paramName, long paramValue, bool isOutput = false);
	void setParameterNULL(const char *paramName);

//add by Liaogd 2007/12/26
//modify by jiangmj 2008/1/11
	void setParameter(const long iPos, const char* paramValue, bool isOutput = false);
	/* ���� long raw ���� */
    void setLongRawParameter(char *paramName, char* paramValue, bool isOutput = false);
	/* ���� blob ����*/
    void setParameter(char *paramName, void* paramValue, int iLen,bool isOutput = false);

    //�������
	void setParamArray(char *paramName, char ** paramValue,int iStructSize,int iStrSize ,bool isOutput = false); 	
	void setParamArray(char *paramName, int * paramValue,int iStructSize, bool isOutput = false); 
	void setParamArray(char *paramName, double * paramValue, int iStructSize,bool isOutput = false) ;
	void setParamArray(char *paramName, long * paramValue, int iStructSize,bool isOutput = false);
	/* ����blob����*/
	void setParamArray(char *paramName, void ** paramValue,int iStructSize,int iStrSize,  bool isOutput=false);
	
	
	//##ModelId=424248FC01E0
  int get_fFieldCount() const;  /*  2005.03.24  Ϊ����SQLAGENT��������   */
  //##ModelId=4242496102F3
  int get_fParamCount() const;
  //##ModelId=4242511200B0
  ub2 get_fStmtType() const;
	
	//constructor & destructor
	TOCIQuery(TOCIDatabase *oradb);
	TOCIQuery(TOCIDatabase *oradb,TOCISession *session);
	~TOCIQuery();
	
private:
	char *fSqlStmt;						//����open��Select��䣬���Է������
	ub2 fStmtType;							//***ub2!!! ����sqlstmt������:SELECT/UPDATE/DELETE/INSERT/CREATE/BEGIN/ALTER...
	bool fActivated;						//�Ƿ��Ѿ����ڴ�״̬���ڵ���OCIStmtPrepare�ɹ���ΪTrue

	unsigned	fFetched;					//0..prefetchRows
	unsigned	fPrefetchRows;				//1.. 
	unsigned	fCurrRow;					//0..fetched */
	unsigned	fTotalRowsFetched;		//rows fetched from the start

	int fFieldCount;						//�ֶθ���
	TOCIDatabase *db;						//��query�����ĸ�Dabase,��Constructor�д���
	TOCIField *fieldList;				//���ڲ�����������ֶ���Ϣ
	void getFieldsDef();					//����ֶ���Ϣ,��Ϊ�ֶη���ȡֵ�Ļ�����

	TOCIParam *paramByName(const char *paramName);//���ڲ�ʹ�ã�ֱ�ӷ��ز�����ָ��
	//ADD BY LIAOGD 2007/12/26
	TOCIParam *paramByIndex(const long iIndex);//���ڲ�ʹ�ã�ֱ�ӷ��ز�����ָ��
	
	void checkError() ;					//�����жϵ�ǰ������Ƿ���ȷִ�У�����д�����Ѵ�����Ϣ����errMsg;

	int fParamCount;						//��������
	TOCIParam *paramList;				//���в������õ���Ϣ
	void getParamsDef();					//��setSQLʱ���ò�����Ϣ
	int nTransTimes;						//�Ƿ�����ִ�й�Execute()����������Ա���ع�.

	//OCITrans* hTrans;
	OCIStmt *hStmt;						//���ڷ���sql����handle
	OCIError *hErr;						//������
	sword fErrorNo;						//�����
	bool fEof;								//��Fetchʱ���Ѿ��ﵽ���һ����¼,��ֹ�Ѿ��������һ����¼����fetch�����Ĵ���
	bool fBof;								//��Open()ʱ��ΪTrue,��Next()���������Ϊfalse;�����ж��û��Ƿ���Դӻ������ж�ȡ��ֵ,�ò�����δ���
	bool fOpened;							//���ݼ��Ƿ��	
//#ifdef __DEBUG__
//    bool bExecuteFlag;
//#endif
};

class TOCISession
{	
public:
	TOCISession(TOCIDatabase *pDB);
	~TOCISession();
	
	void sessionBegin();
	void sessionEnd();

private:
	
	OCISvcCtx	*m_hSrvCtx;
	OCISession	*m_hSession;
	OCIError	*m_hError;
	
	bool	m_bActive;
	
	int	m_iErrorNo;
	
	void checkError();

friend	class TOCIDirPath;
friend  class TOCIQuery;	
};

#endif
