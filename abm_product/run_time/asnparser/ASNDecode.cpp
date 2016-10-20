#include <string.h>
#include "ASNDecode.h"
#include "HSSLogV3.h"


unsigned int ASNParser::m_uiCheckIdx = 0;

int ASNParser::DecodeTag()
{
	if (!code || !size) {
		return err_data_invalid;						// ���������ݲ���Ϊ��
	}


	/*
		8	7	6	5	4	3	2	1
	  +-------+---+---+---+---+---+---|
	1 | CLASS |P/C| 1 | 1 | 1 | 1 | 1 |
	  +-------------------------------+

		7 \ 8 ��λ��ʾ����,6λ��ʾ�Ƿ�ԭ��ʽ

		���5-1λȫΪ1,���ʾtagֵ����31,�ú����ֽڱ�ʾ
	*/

	TOP->m_usType  = (((*code) & 0xC0) >> 6) && 0x00FF;	// �����ֶε�type,���������ֽڵ����2λ(��������)
	TOP->m_usFlag	= (((*code) & 0x20) >> 5) && 0x00FF;// �����ֶε�flag,���������ֽڵĵ�6λ(��������)



	if (((*code) & 0x1F) != 0x1F) {						// tagС��31ʱֱ�ӽ���tag��ȡ
		TOP->m_usTag = (TAG_UINT16)(*code & 0x1F) * 1;
		code++;
		size--;
		return 0;
	}
	
	while (size-- && code++ && ((*code) & 0x80)) {		// tag����31ʱ���ֽ�ת����ȡ
		if (!size || !code ) {
			return err_data_enough;
		}
		
		TOP->m_usTag	*= 1 << 7;						// tag��intergate��ʽ�ɸ�λ����λ��ȡ
		TOP->m_usTag 	+= (TAG_UINT16)((*code) & 0x7F) & 0x00FF;
		
	};

	{													// �ٽ����һλ����
		if (!size || !code ) {
			return err_data_enough;
		}
		
		TOP->m_usTag	*= 1 << 7;						// tag��intergate��ʽ�ɸ�λ����λ��ȡ
		TOP->m_usTag	+= (TAG_UINT16)((*code) & 0x7F) & 0x00FF;

		size--;
		code++;
	}
	return 0;	
}

int ASNParser::DecodeLen()							// ���ȱ�ʾ�����ֶεľ����ȣ���������������������ֶεĳ���
{
	if (!code || !size) {
		return err_data_invalid;						// ���������ݲ���Ϊ��
	}

	/*

		8	7	6	5	4	3	2	1
	  +-------------------------------|
	1 | 0	L	L	L	L	L	L	L |
	  +-------------------------------+

		�̱��뷽ʽ: ��8λΪ0, 7-1λ��ʾ��ʵ����
		�����뷽ʽ: ��8λΪ1, 7-1λ��ʾ���س��ȵ��ֽ���
		�䳤����: ���ֽ�λ0x80, valuesͨ��0x00����
		
	*/	

	TAG_UINT16 len = (TAG_UINT16)((*code) & 0x7F) & 0x00FF;

	if (0x80 == (*code)) {								// �䳤������ֱ�Ӹ�ֵλ-1
		TOP->m_uiLen 		= 1 << 32;
		size--;
		code++;
	}
	else if (((*code) & 0x80)) {						// �������������峤��
		if (len > size) {								// ������Ȳ����򷵻�ʧ��
			return err_data_enough;
		}
		while (len-- && ++code && --size) {
			TOP->m_uiLen 	*= 1 << 8;
			TOP->m_uiLen	+= (TAG_UINT16)(*code) & 0x00FF;
		}

		size--;
		code++;
	}
	else {												// �̱�����ֵ֮ǰ����õĳ���
		TOP->m_uiLen	= len;
		size--;
		code++;
	}

	return 0;
}

int ASNParser::copyValues()
{
	if (!code || !size) {
		return err_data_invalid;						// ���������ݲ���Ϊ��
	}

	if (size < TOP->m_uiLen) {
		return err_data_enough;							// ���������ݲ���С�ڸ��ֶε�len
	}

	if (TOP->m_sValues) {								
		delete TOP->m_sValues;
	}
	
	TOP->m_sValues = new char[TOP->m_uiLen + 1];		// ���Կ�������һ���ڴ��ظ�ʹ��
	TOP->m_sValues[TOP->m_uiLen] = 0x00;
	memcpy(TOP->m_sValues, code, TOP->m_uiLen);
	size -= TOP->m_uiLen;
	code += TOP->m_uiLen;

	return 0;
}


int ASNParser::decode()
{
	if (!code || !size) {
		return err_data_invalid;						// ���������ݲ���Ϊ��
	}

	int	iRet			= 0;
	ASNTree* node		= NULL;

	while (TOP) {
		switch (status) {
		case decode_tag: {			
			if ((iRet = pop())) {							// ���Ƚ�������ɵĽ��г�ջ
				return iRet == DECODEOVER ? 0 : iRet;
			}
			
			node 			= new ASNTree();			// ����һ���½ڵ�,���ڱ����������Ҫ����������
			/* ��Ҫ�Ƚ��ڵ��������,�ٽ������ջ*/
			if ((iRet = encodeTree(node)))	{		// ���½ڵ����tree��
				return iRet;
			}
			stack[top++] 	= node;						// push�������½ڵ����stack
			
			if ((iRet = DecodeTag())) {
				break;
			}

			status = decode_len;
			break;
		}
		case decode_len: {
			if ((iRet = DecodeLen())) {
				break;
			}
			sizestack[top - 1] 	= TOP->m_uiLen;			// �¼���ڵ�����ݳ��ȼ���ջ��
			
			status = TOP->m_usFlag ? decode_tag : decode_values;
			break;
		}
		case decode_values:	{
			if ((iRet = copyValues())) {
				break;
			}
				
			status = decode_tag;
			break;
		}
		default: {
			destorySite();								// ���ֳ���Դ�����ͷ�
			return -3;
		}
		}

		if (iRet && catchException((err)iRet)) {
			printf("ASNParser::decode error, error no is %d!\n", iRet);
			destorySite();								// ���ֳ���Դ�����ͷ�
			return iRet;
		}
	}

	return 0;
}


int ASNParser::encodeTree(ASNTree* node)
{
	if (!node) {
		return err_param_invalid;
	}

	if (!record) {
		record	= node;
		return check();
	}

	int iRet = TOP->childAdd(node);
	return iRet ? iRet : check();
}

int ASNParser::pop()
{
	unsigned int len	= 0;
	int 		 iRet	= 0;
	if (top) {											// �״ν���root�ڵ�ʱ�����г�ջ�ж�

		if (!TOP->m_usFlag) {							// ����ǽṹ���򲻽���pop����
			TOPSIZE 	-= TOP->m_uiLen;
		}
		
		iRet	= ASNDBConf::getInstance(m_uiCheckIdx)->checkField(TOP);
		if (iRet) {
			return iRet;
		}
		while (!TOPSIZE ){								// ���ջ��Ԫ���Ѿ���ȫ��������pop����

			/* ���ﲻ������Ҫ��ȥvalues����,Ҳ��Ҫ����tag\len��ռ���ֽ�*/
			len 	= TOP->m_uiLen + tagCode(TOP->m_usTag) + lenCode(TOP->m_uiLen);
			if (!--top) { 								// ������һ�����ڵ㵯������Ϊ�������
				return DECODEOVER;
			}
			TOPSIZE -= len;
		}

	}

	return 0;
}

int ASNParser::catchException(err &iRet)
{
	return  iRet;
}

int ASNParser::destorySite()
{
	for (int i = 0; i < 20; i++) {
		stack[i] = NULL;
		sizestack[i] = 0;
	}

	code = NULL;
	current = NULL;
	top = 0;
	size = 0;
	status = decode_tag;

	if (record) {
		delete record;
		record = NULL;
	}
	return  0;
}

ASNFileParser* ASNFileParser::m_poInstance = NULL;

int ASNFileParser::decode(const char* sFileFullName)
{
	if (NULL == sFileFullName) {						// ����У��
		return err_param_invalid;
	}

	if (m_iFd) {										// ���֮ǰ�Ѿ��򿪹��ļ�����Ҫ����֮ǰ����
		destorySite();									// ����֮ǰ������¼

		if  (close(m_iFd) != 0) {						// �ر�֮ǰ�ļ�
			if (errno != EINTR) {
				perror("fclose");
				return err_file_close;
			}
		}

	}
	
	m_iFd = 0;
	m_uiCacheSize = 0;
	status = decode_tag;
	
	if (0 == (m_iFd = open(sFileFullName, O_RDONLY))) {	// ���ļ�
        if (errno != EINTR) {
			DIRECT_LOG(V3_ERROR, V3_SERVICE, MBC_FILE_OPEN_FAIL, "OPEN FILE FAILED");
            perror("fopen");
            return err_file_open;
        }
	}

	int iRet = 0;
	if ((iRet = cacheBuff())) {									// ���ļ�������װ���뻺����н���
		return iRet;
	}

	return ASNParser::decode();
}
int ASNFileParser::cacheBuff()
{
	if (m_uiCacheSize == BIGSIZE) {						// ���װ�ص��ļ������Ѿ�����256m�ˣ�����ѯ�û��Ƿ��������ݻ�������
		return err_big_size;
	}
	
	if (size && code != m_sCacheBuff) {
		char * tmp = m_sCacheBuff;
		for (int i = 0; i < size; i++) {
			*tmp = *code;
			tmp++;
			code++;
		}

	}

	int bytes = read(m_iFd, m_sCacheBuff + size, CACHESIZE - size);

	if (!bytes) {
		return err_file_null;							// ��ʾ�ļ��Ѿ�����
	}
	else if (-1 == bytes) {
		return err_file_read;							// ��ȡ�ļ�ʧ��
	}
 
	size += bytes;
	m_uiCacheSize += bytes;
	code = m_sCacheBuff;

	return 0;
}

int ASNFileParser::catchException(err &iRet)
{
	switch(iRet) {
	case err_data_invalid: {
		if (!(size && !code)) {
			iRet = (err)cacheBuff();
		}
		break;
	}
	case err_check_byUTree: {
		break;
	}
	case err_data_enough: {
		iRet = (err)cacheBuff();
		break;
	}
	case err_add_child: {
		break;
	}
	default: {
		break;
	}
	}

	return iRet;
}

int ASNFileParser::freeSomeNode()
{
	
	return 0;
}

ASNMessageParser* ASNMessageParser::m_poInstance = NULL;

int ASNMessageParser::decode(char* sCode, unsigned int len)
{
	if (!sCode || !len) {								// ����У��
		return err_param_invalid;
	}

	destorySite();										// ����֮ǰ������¼

	status = decode_tag;
	size = len;
	code = sCode;

	return ASNParser::decode();
}

ASNFileMutiParser* ASNFileMutiParser::m_poInstance = NULL;

int ASNFileMutiParser::decode(const char* sFileFullName)
{
	if (NULL == sFileFullName) {						// ����У��
		return err_param_invalid;
	}

	if (m_iFd) {										// ���֮ǰ�Ѿ��򿪹��ļ�����Ҫ����֮ǰ����
		destorySite();									// ����֮ǰ������¼
		freeList();
		if  (close(m_iFd) != 0) {						// �ر�֮ǰ�ļ�
			if (errno != EINTR) {
				perror("fclose");
				return err_file_close;
			}
		}

	}
	
	m_iFd = 0;
	m_uiCacheSize = 0;
	status = decode_tag;

	
	
	if (0 == (m_iFd = open(sFileFullName, O_RDONLY))) {	// ���ļ�
        if (errno != EINTR) {
			DIRECT_LOG(V3_ERROR, V3_SERVICE, MBC_FILE_OPEN_FAIL, "OPEN FILE FAILED");
            perror("fopen");
            return err_file_open;
        }
	}

	struct stat statbuf;
	if (stat (sFileFullName, &statbuf)) {
		perror("stat");
		return err_file_open;
	}
	m_uiFileSize = statbuf.st_size;
	
	int iRet = 0;
	if ((iRet = cacheBuff())) {									// ���ļ�������װ���뻺����н���
		return iRet;
	}

	m_poHead = NULL;
	m_uiNum = 0;
	TRecordList * tmpnew = NULL;
	while(!(iRet = ASNParser::decode())) {
		
		tmpnew = new TRecordList();
		m_uiNum++;
		if (!m_poHead) {
			m_poHead = tmpnew;
		}
		else {													// ��֯һ��˫������
			tmpnew->list.poNext = &m_poHead->list;			
			tmpnew->list.poPrev = m_poHead->list.poPrev;
			m_poHead->list.poPrev->poNext = &tmpnew->list;
			m_poHead->list.poPrev = &tmpnew->list;
		}

		record = NULL;
		current = NULL;
		status = decode_tag;
		for (int i = 0; i < 20; i++) {
			stack[i] = NULL;
			sizestack[i] = 0;
		}
		top = 0;
		
		if (m_uiFileSize == m_uiCacheSize && !size) {
			break;
		}

	}

	if (iRet) {
		freeList();
	}
	
	return iRet;
}

int ASNFileMutiParser::freeList()
{
	TRecordList * tmp = m_poHead;
	TList * tmpList = NULL;
	while (tmp && m_uiNum--) {
		delete tmp;

		tmpList = tmp->list.poNext;
		tmp = listObject(tmpList, TRecordList, list);

		if (tmp == m_poHead) break;
	}

	return 0;
}
