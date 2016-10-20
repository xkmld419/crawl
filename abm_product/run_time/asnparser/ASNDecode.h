/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�ASNDecode.h 
* ժ    Ҫ���ļ������˽���asn.1Э�鷽��
* 
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��10��28��
*/

#include <sys/stat.h>
#include "ASNTree.h"
#include "ASNCheck.h"


#define BIGSIZE 			256 * 1024 * 1024
#define CACHESIZE 			8 * 1024
#define BIGGERCACHETIMES	(BIGSIZE) / (CACHESIZE)


enum err {
	err_param_invalid = 1,
	err_data_invalid,
	err_file_open,
	err_file_read,
	err_file_close,
	err_file_null,
	err_check_byUTree,
	err_data_enough,
	err_add_child,
	err_big_size,
};

enum decode_status{
	decode_tag		= 0,
	decode_len		= 1,
	decode_values	= 2
};

class ASNParser {

#define TOP	stack[top - 1]
#define TOPSIZE sizestack[top - 1]
#define DECODEOVER	(1 << 31 - 1)

protected:
	ASNParser() {
		for (int i = 0; i < 20; i++) {
			stack[i] = NULL;
			sizestack[i] = 0;
		}
		code 		= NULL;
		current 	= stack[0];
		size 		= 0;
		top			= 0;
		status 		= decode_tag;
	}

	virtual ~ASNParser() {
		for (int i = 0; i < 20; i++) {
			stack[i] = NULL;
			sizestack[i] = 0;
		}
		code 		= NULL;
		current		= NULL;
		top			= 0;
		status 		= decode_tag;
	}

	/*
	* �������ܣ�����tag����
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	int DecodeTag();
	/*
	* �������ܣ�����len����
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	int DecodeLen();

	/*
	* �������ܣ�����values����
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	int copyValues();

	/*
	* �������ܣ������쳣�ĳ�������
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int catchException(err & iRet);
	/*
	* �������ܣ�����ǰ������Դ��ȫ�ͷ�
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int destorySite();
	/*
	* �������ܣ������ڵ��У�����
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	int check() {
		return 0;
	}
public:
	/*
	* �������ܣ�������ں���
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int decode();
	
	virtual ASNTree * getRecord() {
		return record;
	}

private:
	
	inline int tagCode(unsigned short tag)	{
		if (tag < 0x1F) {
			return 1;
		}

		int num = 2;

		for (int key = tag; key > 0x7F; key = key / 0x7F) {
			num++;
		}

		return num;
	}
	
	inline int lenCode(unsigned int len)	{
		if (len <= 0x7F) {
			return 1;
		}

		int num = 2;

		for (int key = len; key > 0xFF; key = key / 0xFF) {
			num++;
		}

		return num;
	}
	
	/*
	* �������ܣ����½�������Ľڵ��������
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int encodeTree(ASNTree* node);
	/*
	* �������ܣ���Э��ջ�е�����һ��Ԫ��
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	int pop();
protected:
	char*			code;								// ����������������
	unsigned int 	size;								// ���ݴ�С
	decode_status 	status;
	
	ASNTree*		stack[20];							// Э�����ջ
	unsigned int 	sizestack[20];						// ���ݳ���ջ
	int				top;								// ջ��ָ��

	ASNTree*		record;								// ���ݽ�����ĳ�����
	ASNTree*		current;							// ������뵽���еĽڵ�

	static unsigned int m_uiCheckIdx;					// У����������
};

class ASNFileParser : public ASNParser {
public:

	static ASNFileParser* getInstance(unsigned int index = 0) {
		if (!m_poInstance) {
			m_poInstance = new ASNFileParser();
			if (!m_poInstance) {
				printf("new memory error\n");
				exit(-1);
			}
		}
		m_uiCheckIdx = index;
		return m_poInstance;
	}
	/*
	* �������ܣ�������ں���
	* ����������ļ�ȫ·��
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int decode(const char* sFileFullName);

	virtual int freeSomeNode();
	
protected:
	ASNFileParser() {
		m_iFd = 0;

		m_sCacheBuff	= new char[CACHESIZE];					// ����10k�������ڶ�ȡ�ļ�
		code = m_sCacheBuff;
		m_uiCacheSize = 0;
	}

	virtual ~ASNFileParser() {
		m_iFd = 0;

		if (m_sCacheBuff) {
			delete m_sCacheBuff;
			m_sCacheBuff = NULL;
		}

		m_uiCacheSize = 0;
	}


	/*
	* �������ܣ������ݼ��ص�������
	* ����������ļ�ȫ·��
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	int cacheBuff();
	/*
	* �������ܣ������쳣�ĳ�������
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int catchException(err & iRet);
	
protected:
	int			m_iFd;

	char*		m_sCacheBuff;
	unsigned int m_uiCacheSize;
private:
	static ASNFileParser* m_poInstance;
};

class ASNMessageParser : public ASNParser {
public:
	static ASNMessageParser* getInstance(unsigned int index = 0) {
		if (!m_poInstance) {
			m_poInstance = new ASNMessageParser();
			if (!m_poInstance) {
				printf("new memory error\n");
				exit(-1);
			}
		}
		m_uiCheckIdx = index;
		return m_poInstance;
	}

	int decode(char* sCode, unsigned int len);
protected:
	ASNMessageParser() {
		
	}

	virtual ~ASNMessageParser() {

	}

private:
	static ASNMessageParser* m_poInstance;
};

class ASNFileMutiParser : public ASNFileParser {

struct TRecordList {
	ASNTree*	node;
	TList 		list;
	TRecordList () {
		node = NULL;
		list.poNext = &list;
		list.poPrev = &list;
	}
};
public:
	static ASNFileMutiParser* getInstance(unsigned int index = 0) {
		if (!m_poInstance) {
			m_poInstance = new ASNFileMutiParser();
			if (!m_poInstance) {
				printf("new memory error\n");
				exit(-1);
			}
		}
		m_uiCheckIdx = index;
		return m_poInstance;
	}
	

	/*
	* �������ܣ�������ں���
	* ����������ļ�ȫ·��
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int decode(const char* sFileFullName);
	
	virtual ASNTree * getRecord() {
		return m_poHead ? m_poHead->node : NULL;
	}

protected:
	ASNFileMutiParser() {
		m_poHead = NULL;
		m_uiNum  = 0;
	}

	virtual ~ASNFileMutiParser() {
		freeList();
	}
	
	/*
	* �������ܣ��ͷ���Դ
	* �����������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int freeList();
protected:
	TRecordList* 	m_poHead;
	unsigned int	m_uiNum;
	unsigned int 	m_uiFileSize;
private:
	static ASNFileMutiParser* m_poInstance;
};

