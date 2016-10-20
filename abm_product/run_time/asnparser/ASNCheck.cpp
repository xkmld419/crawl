#include "ASNCheck.h"
#include "HSSLogV3.h"
#include "LogV2.h"
#include "ABMException.h"


const char* sQuerySql = "SELECT INDEX, TAG, TYPE, FLAG, MAXLEN, NUM, FATHER, SEQUENCE, MINLEN "
						"FROM ASNDECODECONFIGURATION "
						"ORDER BY INDEX, FATHER, SEQUENCE ";
unsigned int ASNCheck::m_uiCurIdx = 0;
ASNDBConf* ASNDBConf::m_poInstance = NULL;

/*
* �������ܣ���asn�������и�ʽУ��
* �����������У��Ľڵ�
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int ASNCheck::checkField(ASNTree * field)
{
	if (!field) {
		return -1;
	}

	if (!m_oFormatMap[m_uiCurIdx]) {
		return  0;										// û��װ�ص�У��������ֱ�ӷ��سɹ�
	}

	ASNUTree * tmp = NULL;

	if (isRoot(field->m_oNode)) {
		m_oFormatMap[m_uiCurIdx]->current = m_oFormatMap[m_uiCurIdx]->UTree;
		m_oFormatMap[m_uiCurIdx]->current->m_iCurNum = 0;
	}
	int iRet = m_oFormatMap[m_uiCurIdx]->current->UTreeCheckField(field, tmp);

	if (m_oFormatMap[m_uiCurIdx]->current == tmp) {
		m_oFormatMap[m_uiCurIdx]->current->m_iCurNum++;

		if (m_oFormatMap[m_uiCurIdx]->current->m_iCurNum > 1 &&
			m_oFormatMap[m_uiCurIdx]->current->m_iCurNum > m_oFormatMap[m_uiCurIdx]->current->m_iNum
			&& m_oFormatMap[m_uiCurIdx]->current->m_iNum > 0) {
			printf("ASNCheck::checkField num of the node is too big!");
			return -9;									// �ڵ���ִ�����������Ľڵ���
		}
	}
	else {
		if (m_oFormatMap[m_uiCurIdx]->current->m_usFather != tmp->m_usTag) {
			m_oFormatMap[m_uiCurIdx]->current = tmp;
			m_oFormatMap[m_uiCurIdx]->current->m_iCurNum = 1;
		}
		else {
			m_oFormatMap[m_uiCurIdx]->current = tmp;
			m_oFormatMap[m_uiCurIdx]->current->m_iCurNum++;
			if (m_oFormatMap[m_uiCurIdx]->current->m_iCurNum > 1 &&
				m_oFormatMap[m_uiCurIdx]->current->m_iCurNum > m_oFormatMap[m_uiCurIdx]->current->m_iNum
				&& m_oFormatMap[m_uiCurIdx]->current->m_iNum > 0) {
				printf("ASNCheck::checkField num of the node is too big!");
				return -9;									// �ڵ���ִ�����������Ľڵ���
			}
		}
	}
/*
	TList * tmp = m_oFormatMap[2]->current->m_oNode.poNext;

	if (!m_oFormatMap[2]->current->m_iNum) {
		if (isLeaf(m_oFormatMap[2]->current->m_oNode)) {
			
			while(m_oFormatMap[2]->current->m_usFather == listObject(tmp, ASNUTree, m_oNode)->m_usTag) {
				m_oFormatMap[2]->current = listObject(tmp, ASNUTree, m_oNode);
				tmp = m_oFormatMap[2]->current->m_oNode.poNext;
			}
		}
		else {
			tmp = m_oFormatMap[2]->current->m_oNode.poPrev;
		}
		m_oFormatMap[2]->current = listObject(tmp, ASNUTree, m_oNode);
	}
*/

	return iRet;
}


/*
* �������ܣ�װ�������ֵ�,����ͨ�����ݿ��������
* �����������У��Ľڵ�
* �����������
* ����ֵ  ������ɹ��򷵻�0�����򷵻ط���
*/
int ASNDBConf::init()
{
	const char *conInfo = "HSSTT";
	int iRet = GetTTCursor(m_poTTCursor, m_poTTConn, conInfo, oExp);
	if (0 != iRet) {
		directLog(V3_FATAL, V3_DB, MBC_PARAM_LOST_ERROR, "get timesTen Cursor failed(result = %d), conifo is %s.", iRet, conInfo);
		return iRet;
	}

	m_poTTCursor->Close();
	m_poTTCursor->Prepare(sQuerySql);
	m_poTTCursor->Commit();

	try {
		m_poTTCursor->Execute();

		int lastIndex 			= -1;
		long index				= -1;
		long tag				= -1;
		char stag[100];
		long type				= -1;
		long flag 				= -1;
		long len				= -1;
		long num				= -1;
		long father				= -1;
		long sequence			= -1;
		long minlen				= -1;
		ASNFormatData *pData 	= NULL;
		ASNUTree *pUTree		= NULL;
		while(!m_poTTCursor->FetchNext()) {
			pUTree = new ASNUTree();
			m_poTTCursor->getColumn(1, &index);
			m_poTTCursor->getColumn(2, &tag);
			m_poTTCursor->getColumn(2, stag);
			int xxff = m_poTTCursor->getColumnLength(2);
			m_poTTCursor->getColumn(3, &type);
			m_poTTCursor->getColumn(4, &flag);
			m_poTTCursor->getColumn(5, &len);
			m_poTTCursor->getColumn(6, &num);
			m_poTTCursor->getColumn(7, &father);
			m_poTTCursor->getColumn(8, &sequence);
			m_poTTCursor->getColumn(9, &minlen);
			
			pUTree->m_usTag		= tag;
			pUTree->m_usType	= type;
			pUTree->m_usFlag	= flag;
			pUTree->m_uiMaxLen	= len;
			pUTree->m_iNum		= num;
			pUTree->m_usFather	= father;
			pUTree->m_iSequence = sequence;
			pUTree->m_uiMinLen = minlen;
			pUTree->m_usValid	= 1;
			
			if (lastIndex != index ) {
				pData = new ASNFormatData();
				pData->UTree = pUTree;
				pData->current = pUTree;
				lastIndex = index;

				if (m_oFormatMap[index]) {
					printf("һ���ظ����������ݳ���,�������ݿ����� index is %d\n", index);
					return -1;
				}
				else if (index >= 20) {
					printf("�������20������,�������ݿ����� index is %d\n", index);
					return -1;
				}
				m_oFormatMap[index] = pData;
			}
			else {
				if (pData->current->m_usFather == pUTree->m_usFather) {	// ����½ڵ�Ҳcurrent��ͬһ�����ڵ�,��ֱ�Ӹ���current���в���
					iRet = pData->current->brotherAdd(pUTree);			// �˴���Ҫ����ʵ��ASNUTree��brotherAdd����

					if (iRet) {
						break;
					}
				}
				else {
					iRet = pData->UTree->UTreeAdd(pUTree);
					
					if (iRet) {
						break;
					}
				}
			}

		}
	}
	catch(TTStatus st)
	{
		m_poTTCursor->Close();
		__DEBUG_LOG1_(0, "ErrorCode::ParseErrorTable failed! err_msg=%s", st.err_msg);
		iRet = -1;
	}

	if (iRet) {
		destory();
	}
	
	return iRet;
	
}


