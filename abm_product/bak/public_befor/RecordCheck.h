/*VER: 4*/
#ifndef TIBS__RECORDCHECK_H_
#define TIBS__RECORDCHECK_H_

#include "HashList.h"
#include "EventSection.h"
#include "CommonMacro.h"
using namespace std;

class CheckRuleInfo;
#ifndef PRIVATE_MEMORY
    class ParamInfoMgr;
#endif

class CheckArgument
{
	friend class CheckRuleInfo;
#ifndef PRIVATE_MEMORY
	friend class ParamInfoMgr;
#endif	
public:
    CheckArgument();	
    ~CheckArgument();
    
public:
    int m_iLocation;    //�����ں����е�λ�ã���ʼλ��Ϊ1��
    char m_sValue[50];  //����ֵ��ͳһΪ�ַ�������   
    
    //��һ��λ�õĵ�һ������
    CheckArgument *m_poNextSibArgument;

    //ͬһ��λ�õ���һ������
    CheckArgument *m_poNextArgument;
    
    //test code
    void showMe(int iLevel=0);
#ifndef PRIVATE_MEMORY
	unsigned int m_iNextSibArgument;
	unsigned int m_iNextArgument;
#endif

    
private:
    int m_iArgID;       //����ID
    int m_iSubRuleID;   //���������������ӹ���ID
        
};

class CheckSubRule
{	
	friend class CheckRuleInfo;
	friend class CheckRule;
#ifndef PRIVATE_MEMORY
	friend class ParamInfoMgr;
#endif
public:
    CheckSubRule();    
    ~CheckSubRule();

	//ִ�б��ӹ������У��ͨ������û�д��������򷵻�true
	bool execute(const EventSection *poEventSection, int iTAG);
    
//private:
    int m_iSubRuleID;   //�ӹ���ID
    int m_iRuleID;      //���ӹ����������Ĺ���ID
    int m_iFuncID;      //ִ�б��ӹ������õ��ĺ���ID
    int m_iCheckSeq;    //ִ��У��Ĵ��򣨴�С����
    int m_iNOT;         //�񶨱�ʶ

    //��һ������ϴ���ӹ�������ӹ���Ҳ������ͬһ������
    CheckSubRule *m_opNextSubRule;

    //ָ���ӹ���ĵ�һ������
    CheckArgument *m_opFirstAgrument;
#ifndef PRIVATE_MEMORY	
	unsigned int m_iNextSubRule;
	unsigned int m_iFirstAgrument;
#endif	
    
public:
    //test code
    void showMe(int iLevel=1);
};


class CheckRule
{
	friend class CheckRuleInfo;
#ifndef PRIVATE_MEMORY
	friend class ParamInfoMgr;
	friend class RecordCheck;
#endif	
public:
	CheckRule();	
	~CheckRule();

	//ִ�б��������У��ͨ������û�д��������򷵻�true
	//bCheckWarning��һ���������أ����ڿ����Ƿ�Ծ��漶��Ĺ���ִ��У�飬Ĭ��ִ��
	bool execute(EventSection *poEventSection, char *sAllowNull, int iBillType,int iRecordType,bool bCheckWarning=true );
	
//private:	
    int m_iRuleID;         //����ID
    int m_iTAG;            //TAGֵ
    int m_iErrCode;        //�������
    int m_iErrLevel;       //���󼶱� 1:Fatal-�������� 2:severe-���ش��� 2:warning-����
    
    //ָ���һ���ӹ���
    CheckSubRule *m_opFirstSubRule;
    
#ifndef PRIVATE_MEMORY
	unsigned int m_iFirstSubRule;
#endif    
public:
    //test code
    void showMe(int iLevel=1);
    
};

class CheckRelation
{
	friend class CheckRuleInfo;
	friend class RecordCheck;
#ifndef PRIVATE_MEMORY
	friend class ParamInfoMgr;
#endif	
public:
    CheckRelation();
    ~CheckRelation();
        
//private:
    int m_iRelationID;   //У���ϵID
    int m_iRuleID;       //����ID
    int m_iBillType;     //�������ͣ��磺1ʡ����������������2����������������
    int m_iRecordType;   //��¼���ͣ��磺��������������1���С�2���С�3��ת
    int m_iCheckSeq;     //ִ��У��Ĵ��򣨴�С����
    char m_sAllowNull[2];  //�ڱ������п�ֵ�Ƿ�����ȷ

	//ָ��m_iRuleID��Ӧ��У�����
	CheckRule *m_poCheckRule;

    //ָ���ֵܻ������͵ĵ�һ��У���ϵ
    CheckRelation *m_poSibBillRelation;

    //ָ���ֵܼ�¼���͵ĵ�һ��У���ϵ
    CheckRelation *m_poSibRecordRelation;

    //ָ����ͬ���ͣ����������͡���¼���Ͷ���ͬ������һ������ϴ�ļ����ϵ
    CheckRelation *m_poNextRelation;
    
#ifndef PRIVATE_MEMORY	
	unsigned int m_iCheckRule;
	unsigned int m_iSibBillRelation;
	unsigned int m_iSibRecordRelation;
	unsigned int m_iNextRelation;
#endif	
public:
    //test code
    void showMe(int iLevel=1);
    
};

//У����صı�����������������
class CheckRuleInfo
{
public:
    CheckRuleInfo();	
    ~CheckRuleInfo();
    
    static void load();
	
    static void unload();
	
public:
    static HashList<CheckRule *> *s_poRuleIndex;       
	static HashList<CheckSubRule *> *s_poSubRuleIndex;
	static HashList<CheckArgument *> *s_poArgumentIndex;  

    static CheckRule *s_poRuleList;
	static CheckSubRule *s_poSubRuleList;
	static CheckArgument *s_poArgumentList;
	static CheckRelation *s_poRelationList;
	static int nRuleSize;
	static int nSubRuleSize;
	static int nArgumentSize;
    static int nRelationSize;

private:
	static bool s_bUploaded;		//ȫ��������ɣ���true
	static bool s_bRuleUploaded;    //Rule������ɣ���true
	static bool s_bSubRuleUploaded; //����
	static bool s_bArgumentUploaded; 
	static bool s_bRelationUploaded;

	static void loadRule();
	static void loadSubRule();
	static void loadArgument();
	static void loadRelation();

	static void unloadRule();
	static void unloadSubRule();
	static void unloadArgument();
	static void unloadRelation();
	
public:
	//test code
	static void showMe()
	{
	    int i=0;
	    printf("\nRelation\n");
	    CheckRelation *pBill, *pRecord;
	    if (nRelationSize>0){
	        pBill = &s_poRelationList[0];
	        while (pBill!=NULL && i<nRelationSize){
	            
	            int j = 1;
	            pRecord = pBill;
	            while (pRecord!=NULL){
	                pRecord->showMe(j++);
	                pRecord = pRecord->m_poSibRecordRelation;
	                i++;
	            }
	            pBill = pBill->m_poSibBillRelation;
	        }
	            
	    }  
	    printf("��%d��У���ϵ\n", nRelationSize);
	    
	   i = 0;	   
	   printf("\nRule\n");
	   while (i < nRuleSize){
	        s_poRuleList[i++].showMe(1);
	   }
	   printf("��%d��У�����\n", i);
	   
	   i=0;
	   printf("\nSubRule\n");
	   while (i < nSubRuleSize){
	        s_poSubRuleList[i++].showMe(1);
	   }
	   printf("��%d���ӹ���\n", i);
	   
	   i=0;
	   printf("\nArgument\n");
	   while (i < nArgumentSize){
	        s_poArgumentList[i++].showMe(1);
	   }
	   printf("��%d������\n", i);
	   
	}

};    

//�����ṩ��������Ľӿ�
class RecordCheck
{
public:
	RecordCheck();
	~RecordCheck();
	
	static bool check(EventSection *poEventSection, bool bCheckWarning=true );

public:
	static int getBillType(EventSection *poEventSection);
	static int getRecordType(EventSection *poEventSection);
	
};

class TransFileMgr;

class TransFileType
{
friend class TransFileMgr;
#ifndef PRIVATE_MEMORY
	friend class ParamInfoMgr;
#endif	
public:
    TransFileType():m_iTypeID(0){
           memset(m_sTypeName, 0, sizeof(m_sTypeName));
           memset(m_sPath, 0, sizeof(m_sPath));
           memset(m_sTmpPath, 0, sizeof(m_sTmpPath));
    }
    
    ~TransFileType(){};
    
//private:
    
    //trans_file_type_id
    int  m_iTypeID;
    
    //trans_file_type_name
    char m_sTypeName[64];
    
    //file_path
    char m_sPath[MAX_FILE_NAME_LEN];
    
    //temp_file_path
    char m_sTmpPath[MAX_FILE_NAME_LEN];

    //�����ļ��Ƿ���Ҫͷ��¼
    int m_iErrHead;

    //�����ļ��Ƿ���Ҫβ��¼
    int m_iErrTail;

//�Ƿ���Ҫд�����ļ���add by yuanp 20100331
    int m_iWriteFile;
        
};

class TransFileMgr
{
public:
    TransFileMgr(){};
    ~TransFileMgr(){};
    
    static void load();
    static void unload();
    
    static const char* getFileTypeName(int iTypeID);
    static const char* getFileTypePath(int iTypeID);
    static const char* getFileTypeTmpPath(int iTypeID);
    
    //return iTypeID
    static int getFileTypeID(const char *sFileTypeName);
    
    static const char* getFileTypeName(const char *sFileTypeName);
    static const char* getFileTypePath(const char *sFileTypeName);
    static const char* getFileTypeTmpPath(const char *sFileTypeName);
    //add 
    static int getFileErrHead(const char *sFileTypeName);
    static int getFileErrHead(int iTypeID);
    static int getFileErrTail(const char *sFileTypeName);
    static int getFileErrTail(int iTypeID);
    static int getWriteErrFile(int iTypeID);
    static int getWriteErrFile(const char *sFileTypeName);
    
private:
    static int nSize;
    static bool s_bUploaded;
    
    static HashList<TransFileType *> *s_poFileTypeIndex;
    static TransFileType *s_poFileTypeList;
};

class TagSeqMgr;

class TagSeq
{
friend class TagSeqMgr;
#ifndef PRIVATE_MEMORY
	friend class ParamInfoMgr;
#endif
public:
    TagSeq()
    :   m_iGroupID(0), m_iTAG(0), m_iAttrID(0), m_iSeq(0),
        m_poNextSibTagSeq(NULL), m_poNextTagSeq(NULL)
    {
        memset(m_sName, 0, sizeof(m_sName));
    }
    ~TagSeq(){};
    
    const TagSeq * nextSib() const
    {   
        return m_poNextSibTagSeq;
    }
    
    const TagSeq * next() const
    {   
        return m_poNextTagSeq;
    }
    
    int getAttrID() const
    {
        return m_iAttrID;
    }
    
    int getTag() const
    {
        return m_iTAG;
    }
    
    int getSeq() const
    {
        return m_iSeq;
    }
    
    const char * getName() const
    {
        return m_sName;
    }
    
    int getGroupID() const
    {
        return m_iGroupID;
    }
    
    void showMe(int iLevel=1);
private:
    int m_iGroupID;
    int m_iTAG;
    int m_iAttrID;
    int m_iSeq;
    char m_sName[50];
    
    TagSeq *m_poNextSibTagSeq;  //ָ����һ��GroupID���׸�TagSeq.
    TagSeq *m_poNextTagSeq;     //ָ��ͬ�����һ��TagSeq;
#ifndef PRIVATE_MEMORY	
	unsigned int m_iNextSibTagSeq;
	unsigned int m_iNextTagSeq;
#endif		
};

class TagSeqMgr
{
public:
    TagSeqMgr(){}
    
    ~TagSeqMgr(){}

    static void load();
    static void unload();
    
    static const TagSeq *getFirstTagSeq(int iGroupID);
    static int getArrtID(int iGroupID, int iTag);
    
    //test code
    static void showMe();
private:
    static int nSize;
    static bool s_bUploaded;
    static TagSeq *s_poTagSeqList;
};

#endif /* End of TIBS__RECORDCHECK_H_  */
