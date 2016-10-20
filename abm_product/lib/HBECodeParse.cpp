#include "HBECodeParse.h"
#include "File.h"

HBErrorCode::HBErrorCode()
{
#ifdef  _DEFINE_ECODE_FILE_

      m_poEFile = NULL;

#else
        m_poTTCursor = NULL;
        m_poTTConn = 0;
#endif

}

HBErrorCode::~HBErrorCode()
{
#ifdef _DEFINE_ECODE_FILE_
    if (NULL != m_poEFile) {
        delete m_poEFile;
    }
#else
	RelTTCursor(m_poTTCursor);
#endif
    for (EInfoMap::iterator begin = m_oEInfo.begin(); begin != m_oEInfo.end();) {
        if (NULL != begin->second) {
            delete begin->second;
        }
	  m_oEInfo.erase(begin++);
    }
   m_oEInfo.clear();
}

/*
*   �������ܣ���ʼ��HBErrorCode�����ļ�
*   �����������
*   �����������
*   ����ֵ  �����ش����룬����ɹ��򷵻�0
*/
int HBErrorCode::init()
{
	
    if (0 != m_oEInfo.size()) {
        return -1;
    }
#ifdef _DEFINE_ECODE_FILE_    
    char *penv = NULL;
    char sLibEnv[128] = {0};
    char sfile[256] = {0};
    char Pwd[128] = {0};
    if ((penv=getenv("ABM_PRO_DIR")) == NULL) {
        printf( "��������HSSLIBû������, init HBErrorCode Information failed�w���ش���");             
        return -1;
    }
    strncpy(sLibEnv, penv, sizeof(sLibEnv)-2);
    if (sLibEnv[strlen(sLibEnv)-1] != '/') {
        sLibEnv[strlen(sLibEnv)] = '/';             
    }

    snprintf(sfile, sizeof(sfile), "%s%s/MBC_ABM.h", sLibEnv,"src/include");
    m_poEFile = new File(sfile);

    return ParseErrorFile();
#else
	const char *conInfo = "HSSTT";
	ABMException oExp;
	int iRet = GetTTCursor(m_poTTCursor, m_poTTConn, conInfo, oExp);
	if (0 != iRet) {
		printf("get timesTen Cursor failed(result = %d), conifo is %s.", iRet, conInfo);
		return iRet;
	}

	m_poTTCursor->Close();
	const char* sSql = "SELECT DISTINCT ERR_ID, ERR_DESC, ERR_ADVICE FROM HSS_ERR_INFO;";
	m_poTTCursor->Prepare(sSql);
	m_poTTCursor->Commit();
    return ParseErrorTable();
#endif    
}

/*
*   �������ܣ���ȡ�����������Ϣ
*   ���������������
*   �����������
*   ����ֵ  �����ش�����������Ϣ
*/
char* HBErrorCode::getCauseInfo(int iHBErrorCode)
{
    return m_oEInfo[iHBErrorCode]->m_sCause;
}

/*
*   �������ܣ���ȡ�����ר���޸�����
*   ���������������
*   �����������
*   ����ֵ  �����ش������ר���޸�����
*/
char* HBErrorCode::getAdviceInfo(int iHBErrorCode)
{
    return m_oEInfo[iHBErrorCode]->m_sAdvice;
}

/*
*   �������ܣ���ȡ����������Ϣ���������������޸�����
*   ���������������
*   �����������
*   ����ֵ  �����ش������������Ϣ���޸�����
*/
EInfo* HBErrorCode::getErrorInfo(int iHBErrorCode)
{
    return m_oEInfo[iHBErrorCode];
}

/*
*   �������ܣ������ļ�����ȡ��������Ϣ
*   �����������
*   �����������
*   ����ֵ  ���ɹ�����0�����򷵻�ָ������
*/
#ifdef _DEFINE_ECODE_FILE_
int HBErrorCode::ParseErrorFile()
{
    if (NULL == m_poEFile) {
        return -1;          // �ļ�������

    }


    FILE *fd;
    char sBuffer[1024 + 1];
    char sCode[10 + 1];
    char *p = NULL;
    char *q = NULL;
    unsigned int iRow = 0;
    unsigned int iLen = 0;
    memset(sBuffer, 0x00, sizeof(sBuffer));
    memset(sCode, 0x00, sizeof(sCode));
    int     iECode = 0;
    EInfo  *poInfo = NULL;

    if (!(fd = fopen(m_poEFile->getFullFileName(), "r"))) {
        return -1;          // ���ļ�ʧ��
    }

    // �����������ж���Ϊ��
    //       "//@@\tCODE\tCAUSE\tADVICE"
    //       ����CODE��������� CAUSE�Ǵ��������� ADVICE���޸�����
    //       ����֮�䶼��\t��0-n���ո������
    while(fgets(sBuffer, 1024, fd)) {
        iRow++;
        if (CompStrHead(sBuffer)) {
            continue;
        }

        p = sBuffer + 4;
        if ('\t' != *p) {
            printf("һ�и�ʽ����Ĵ�����������Ϣ, �кţ�%d, %s\n", iRow, sBuffer);
            break;
        }
        p++;

        // ���� CODE
        q=p;
        p = strchr(p, '\t');
        if (NULL == p) {
            printf("һ�и�ʽ����Ĵ�����������Ϣ, �кţ�%d, %s\n", iRow, sBuffer);
            break;
        }
        memset(sCode, 0x00, sizeof(sCode));
        iLen = p - q;
        memcpy(sCode, q, iLen > 10 ? 10 : iLen);
        iECode = strtol(sCode, &q, 10);
        p++;

        poInfo = new EInfo();

        // ���� ��������
        q=p;
        p = strchr(p, '\t');
        if (NULL == p) {
            printf("һ�и�ʽ����Ĵ�����������Ϣ, �кţ�%d, %s\n", iRow, sBuffer);
            delete poInfo;
            poInfo = NULL;
            break;
        }
        ///ȥ��ǰ��Ŀո�
        while ((' ' == *q || '\t' == *q) && q++);
        memcpy(poInfo->m_sCause, q, p - q);
        p++;

        // ���� ��������
        while ((' ' == *p || '\t' == *p) && p++);
        char *pp=strchr(p, '\n');  
        if(*pp) *pp=0;
        strcpy(poInfo->m_sAdvice, p);

        if (!m_oEInfo.count(iECode)) {
            m_oEInfo.insert(EInfoMap::value_type(iECode, poInfo));
        }
        else {
            printf("��ȡ��һ���ظ��Ĵ�����������Ϣ, �кţ�%d\n", iRow);
        }
        poInfo = NULL;
    }

    return 0;
}
/*
*   �������ܣ���ȡ������Ϣ����ȡ��������Ϣ
*   �����������
*   �����������
*   ����ֵ  ���ɹ�����0�����򷵻�ָ������
*/
#else
int HBErrorCode::ParseErrorTable()
{
	long iECode;
	EInfo *poInfo = NULL;

	m_poTTCursor->Close();

	try {
		m_poTTCursor->Execute();
		while(!m_poTTCursor->FetchNext()) {
			poInfo = new EInfo();
			m_poTTCursor->getColumn(1, &iECode);
			m_poTTCursor->getColumn(2, poInfo->m_sCause);
			m_poTTCursor->getColumn(3, poInfo->m_sAdvice);

			if (!m_oEInfo.count(iECode)) {
				m_oEInfo.insert(EInfoMap::value_type(iECode, poInfo));
			}
			else {
				printf("��ȡ��һ���ظ��Ĵ�����������Ϣ, err_id is %d\n", iECode);
			}
	        poInfo = NULL;
		}
	}
	catch(TTStatus st)
	{
		m_poTTCursor->Close();
		printf("ErrorCode::ParseErrorTable failed! err_msg=%s", st.err_msg);
		return -1;
	}
	return 0;
}
#endif
/*
*   �������ܣ��ж��ַ���ǰ4λ�Ƿ���//@@
*   ��������������ַ���
*   �����������
*   ����ֵ  ���ɹ�����0�����򷵻�ָ������
*/
inline int HBErrorCode::CompStrHead(const char *sStr)
{
    if (NULL == sStr) {
        return -1;
    }

    if (4 >= strlen(sStr)) {
        return -1;
    }

    if ('/' == sStr[0] && '/' == sStr[1] && '@' == sStr[2] && '@' == sStr[3]) {
        return 0;
    }
    return -1;
}

