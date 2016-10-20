/*
* Copyright (c) 2011,��˾����
* All rights reserved.
* 
* �ļ����ƣ�ASNCheck.h 
* ժ    Ҫ���ļ�����������У��asn�����ʽ��Ԫ������
* 
*
* ȡ���汾��1.0 
* ԭ����  ��������
* ������ڣ�2011��10��21��
*/
#ifndef __ASN_CHECK_
#define __ASN_CHECK_

#include <map>
#include <unistd.h>
#include "ASNObject.h"
#include "TimesTenAccess.h"

using namespace std;

extern ABMException oExp;

struct ASNFormatData {
	ASNUTree*	UTree;
	ASNUTree*	current;

	ASNFormatData() {
		UTree 	= NULL;
		current = NULL;
	}

	~ASNFormatData() {
		if (UTree) {
			delete UTree;
			UTree = NULL;
		}
		current = NULL;
	}
};

typedef std::map<int, ASNFormatData*>			TASNFORMATMAP;
typedef std::map<int, ASNFormatData*>::iterator	TASNFORMATITER;


/* ************** ASN�ֶθ�ʽУ����************ */
class ASNCheck {
public:
	
	/*
	* �������ܣ���asn�������и�ʽУ��
	* �����������У��Ľڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int checkField(ASNTree * field);
	
protected:
	ASNCheck() {
		for (int i = 0; i < 20 ; i++) {
			m_oFormatMap[i] = NULL;
		}
	}

	virtual ~ASNCheck() {
		destory();
	}

	/*
	* �������ܣ�У���ʽ��Ԫ����װ��
	* ���������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int init() = 0;

	virtual void destory()
	{
		for (int i = 0; i < 20 ; i++) {
			if (m_oFormatMap[i]) {
				delete m_oFormatMap[i];
			}

			m_oFormatMap[i] = NULL;
		}
	}

public:
	ASNFormatData 		*m_oFormatMap[20];

	static unsigned int m_uiCurIdx;
};




/* ************** ASN�ֶθ�ʽУ������************ */
/* ** �������ֵ�,��������δ��������ʱ��*** */
class ASNNULLConf : public ASNCheck {
public:
	static ASNNULLConf* getInstance(unsigned int index = 0) {
		if (!m_poInstance) {
			m_poInstance = new ASNNULLConf();
			if (!m_poInstance) {
				printf("ASNCheck::ASNCheck()��������ʧ��,�����ڴ�ʧ��!");
				exit(-1);
				return NULL;
			}
			ABMException oExp;
			if (m_poInstance->init()) {
				printf("ASNCheck::init��������ʧ��!");
				exit(-1);
				return NULL;
			}
		}
		m_uiCurIdx = index;
		return m_poInstance;
	}
	/*
	* �������ܣ���asn�������и�ʽУ��
	* �����������У��Ľڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int checkField(ASNTree * field)
	{
		return 0;
	}
protected:
	ASNNULLConf() {
		m_poInstance = NULL;
	}

	virtual ~ASNNULLConf() {

	}

	/*
	* �������ܣ�װ�������ֵ�,����ͨ�����ݿ��������
	* ���������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int init()
	{
		return 0;
	}
private:
	static ASNNULLConf	*m_poInstance;
};



/* ************** ASN�ֶθ�ʽУ������************ */
/* *********** �����ֵ������ݿ���װ��********* */
class ASNDBConf : public ASNCheck {
public:
	static ASNDBConf* getInstance(unsigned int index = 0) {
		if (!m_poInstance) {
			m_poInstance = new ASNDBConf();
			if (!m_poInstance) {
				printf("ASNCheck::ASNCheck()��������ʧ��,�����ڴ�ʧ��!");
				exit(-1);
				return NULL;
			}

			if (m_poInstance->init()) {
				printf("ASNCheck::init��������ʧ��!");
				exit(-1);
				return NULL;
			}
		}
		m_uiCurIdx = index;
		return m_poInstance;
	}
	
protected:
	ASNDBConf() {
		m_poTTCursor = NULL;
		m_poTTConn   = NULL;
	}

	virtual ~ASNDBConf() {
		RelTTCursor(m_poTTCursor);

		m_poTTCursor = NULL;
		m_poTTConn   = NULL;
	}

	/*
	* �������ܣ�װ�������ֵ�,����ͨ�����ݿ��������
	* ���������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int init();

private:
	TimesTenCMD 		*m_poTTCursor;				// tt�α�
	TimesTenConn		*m_poTTConn;
	
	static ASNDBConf	*m_poInstance;
};



/* ************** ASN�ֶθ�ʽУ������************ */
/* ********* �����ֵ��������ļ���װ��******** */
class ASNFileConf : public ASNCheck {
public:
	static ASNFileConf* getInstance(unsigned int index = 0) {
		if (!m_poInstance) {
			m_poInstance = new ASNFileConf();
			if (!m_poInstance) {
				printf("ASNCheck::ASNCheck()��������ʧ��,�����ڴ�ʧ��!");
				exit(-1);
				return NULL;
			}

			ABMException oExp;
			if (m_poInstance->init()) {
				printf("ASNCheck::init��������ʧ��!");
				exit(-1);
				return NULL;
			}
		}
		m_uiCurIdx = index;
		return m_poInstance;
	}
	
	/*
	* �������ܣ���asn�������и�ʽУ��
	* �����������У��Ľڵ�
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int checkField(ASNTree * field)
	{
		//��Ҫ��ʵ��
		return 0;
	}
protected:
	ASNFileConf() {
		m_poFile 	= NULL;
		m_poInstance = NULL;
	}

	virtual ~ASNFileConf() {
		if (m_poFile) {
			fclose(m_poFile);
			m_poFile = NULL;
		}
	}

	/*
	* �������ܣ�װ�������ֵ�,����ͨ�������ļ���������
	* ���������
	* �����������
	* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
	*/
	virtual int init()
	{
		//��Ҫ��ʵ��
		return 0;
	}

private:
	FILE				*m_poFile;
	
	static ASNFileConf	*m_poInstance;
};


#endif

