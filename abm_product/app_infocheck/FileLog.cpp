/******************************************************************************************
�ļ���      : FileLogAppender.cpp
����        :
�汾        :
��������    : 2008-07-02
�ļ�����    : CFileLog �࣬��¼�ļ���־��
����        :
�����б�    :
�޸ļ�¼    :

 ******************************************************************************************/
#include "FileLog.h"
//default constructor
CFileLog::CFileLog()
{
    m_pStream = NULL;
    m_bIsOpen = false;
    m_strFileName = "";
    m_strFilePath = "";
    m_pLock = new CmutexLock();
}

//commonly used constructor
CFileLog::CFileLog ( std::string strFilePath )
{
    m_pStream = NULL;
    m_bIsOpen = false;
    m_strFilePath = strFilePath;
    //��������·�����ƺ�û�м�/" �����/"
    char szFilePath[MAXSORTPATH + 1] =
    {
        0
    };
    strncpy ( szFilePath, strFilePath.c_str(), MAXSORTPATH );

    m_strFilePath = szFilePath;
    if ( m_strFilePath.length() > 0 )
    {
        if ( m_strFilePath.c_str() [m_strFilePath.length() - 1] != '/' )
        {
            m_strFilePath.append ( "/" );
        }
    }
    m_strFileName = "";
    m_pLock = new CmutexLock();
}

//destructor
CFileLog::~CFileLog()
{
    DelObject();
    if (NULL != m_pLock)
    {
        delete m_pLock;
        m_pLock = NULL;
    }
}

void CFileLog::DelObject()
{
    if ( m_bIsOpen )
    {
        m_bIsOpen = false;
    }
    if ( NULL != m_pStream )
    {
        m_pStream->close();
        delete m_pStream;
        m_pStream = NULL;
    }
}


//open method
bool CFileLog::OpenFile ( std::string strFileName )
{

    // ������־�ļ�·������־�ļ�����������������־�ļ���������·����
    std::string strfilename; //·��+�ļ���
    m_strFileName = strFileName;
    strfilename = m_strFilePath + strFileName;

    //���û�д��ļ�
    if ( false == m_bIsOpen )
    {
        //����ļ���Ϊ��
        if ( NULL == m_pStream )
        {
            //���ļ���
            m_pStream = new std::ofstream ( strfilename.c_str(), std::ios::app );

            //�����ʧ�ܣ�����false
            if ( NULL == m_pStream )
            {
                std::cerr << "CFileLog::OpenFile��can't open file  " << strfilename.c_str() << "! : error in : " << __FILE__ << "  " << __LINE__ << std::endl;
                DelObject();
                return false;
            }

            // �ж��Ƿ�����Ĵ򿪣�
            //�����־·�����ԣ����Գ�ʼ��m_pStream������ʵ���ϲ�û�������
            if ( false == m_pStream->is_open() )
            {
                //��ʧ��
                std::cerr << "CFileLog::OpenFile��m_pStream status is not open ! : error in : " << __FILE__ << "  " << __LINE__ << std::endl;
                DelObject();
                return false;
            }

            m_pStream->setf ( std::ios::unitbuf ); //turn off buffering
        }
        else
        {
            m_pStream->open ( strfilename.c_str(), std::ios::app );
            if ( !m_pStream )
            {
                std::cerr << "CFileLog::OpenFile��can't open file  " << strfilename.c_str() << "! : error in : " << __FILE__ << "  " << __LINE__ << std::endl;
                DelObject();
                return false;
            }

            // �ж��Ƿ�����Ĵ򿪣�
            //�����־·�����ԣ����Գ�ʼ��m_pStream������ʵ���ϲ�û�������
            if ( 0 == m_pStream->is_open() )
            {
                //��ʧ��
                std::cerr << "CFileLog::OpenFile�� m_pStream status is not open ! : error in : " << __FILE__ << "  " << __LINE__ << std::endl;
                DelObject();
                return false;
            }

            m_pStream->setf ( std::ios::unitbuf ); //turn off buffering
        }
        m_bIsOpen = true;
    }

    return true;
}

//close method
bool CFileLog::CloseFile()
{
    bool bResult = false;
    //�����־�ļ��Ѿ���
    if ( m_bIsOpen )
    {
        //�����־�ļ���Ϊ�գ�����ʧ��
        if ( NULL == m_pStream )
        {
            bResult = false;
        }
        else
        {
            //�ر��ļ��������سɹ�
            m_pStream->close();
            bResult = true;
        }
        //���ô򿪱�־Ϊfalse
        m_bIsOpen = false;
    }
    return bResult;
}



std::string CFileLog::CreateFileName()
{

    int iPosition;
    struct tm* today;
    std::string strmode;
    time_t ltime;
    char szdatestr[MAXSORTPATH + 1] = "\0";

    //���ʱ��ṹtoday
    time ( &ltime );
    today = localtime ( &ltime );

    //���ģ����
    strmode = m_moduleName;

    if ( std::string::npos != ( iPosition = strmode.find ( "_" ) ) )
    {
        strmode.resize ( iPosition );
    }

    snprintf ( szdatestr, MAXSORTPATH, "process_%04d-%02d-%02d_%s.log", today->tm_year + 1900, today->tm_mon + 1, today->tm_mday, strmode.c_str() );


    //������־�ļ�����
    return std::string ( szdatestr );
}


bool CFileLog::IsReady()
{
    if ( m_bIsOpen && ( NULL != m_pStream ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

int CFileLog::WriteLog ( int _nPriority, const char* _FileName, int _nPosition, const char* _szLogInfo )
{
    m_pLock->lock();
    //��ʽ����־��Ϣ
    std::string strInfo = formatLog ( _nPriority, _FileName, _nPosition, _szLogInfo );
    int nResult =  - 1;

    //Ϊ��ʵ������ʱ��ɾ����־�ļ��󻹿��������ļ�
    if ( m_bIsOpen )
    {
        std::string strfilename = m_strFilePath + m_strFileName;
        //������ʲ����ļ�
        if ( access ( strfilename.c_str(), R_OK | W_OK ) < 0 )
        {
            DelObject();
        }
    }

    //���û�д��ļ�����ô���ļ�
    if ( !m_bIsOpen )
    {
        //����ļ������Ҵ��ļ�
        m_strFileName = CreateFileName();
        if ( !OpenFile ( m_strFileName ) )
        {
            std::cerr << "CFileLog::WriteLog��can't open file! : error in : " << __FILE__ << "  " << __LINE__ << std::endl;
            return  - 1;
        }
    }

    //�ж��Ƿ��Ѿ�׼���ã�����д�ļ���־
    if ( !IsReady() )
    {
        std::cerr << "CFileLog::WriteLog��write file is not ready �� " << __FILE__ << "  " << __LINE__ << std::endl;
        return  - 1;
    }

    struct tm* today;
    time_t ltime;
    time ( &ltime );
    today = localtime ( &ltime );
    char szcntime[ID_LEN_32 + 1] =
    {
        0
    };
    snprintf ( szcntime, ID_LEN_32, "process_%04d-%02d-%02d", today->tm_year + 1900, today->tm_mon + 1, today->tm_mday );
    std::string strDate = szcntime;



    /*��־�ļ�����:  YYYY-MM-DD_ģ����.log��һ��һ����־�ļ�
    �ж��ļ����͵�ǰʱ���Ƿ���һ�����������һ����
    �رմ򿪵��ļ������Ҵ����������ļ�
     */

    bool bSameFile = false; //�ж��Ƿ��ǵ�ǰʱ�����־
    if ( m_strFileName.length() > 18 )
    {
        if ( m_strFileName.substr ( 0, 18 ) == strDate )
        {
            bSameFile = true;
        }
    }
    //�������ͬһ���ļ�
    if ( !bSameFile )
    {
        //����Ѿ�����һ���ļ����ر�
        if ( m_bIsOpen )
        {
            CloseFile();
        }

        //��ý�Ҫ�򿪵��ļ���
        m_strFileName = CreateFileName();
        //���ļ�
        if ( !OpenFile ( m_strFileName ) )
        {
            std::cerr << "CFileLog::WriteLog��can't open file! : error in : " << __FILE__ << "  " << __LINE__ << std::endl;
            return  - 1;
        }
    }

    if ( NULL == m_pStream )
    {
        nResult =  - 1;
    }
    else
    {
        //д��־��Ϣ
        *m_pStream << strInfo << "\n";
	std::cout<<strInfo<<std::endl;
        nResult = 1;
    }
    m_pLock->unlock();
    return nResult;
}

std::string CFileLog::formatLog ( int _nPriority, const char* _FileName, int _nPosition, const char* _szLogInfo )
{
    std::ostringstream message;
    // ��õ�ǰʱ�䣬����ת��ΪYYYY-MM-DD HH:MI:SS ��ʽ���ַ���
    struct tm* today;
    time_t ltime;
    time ( &ltime );
    today = localtime ( &ltime );
    char sztime[ID_LEN_32 + 1] =
    {
        0
    };
    snprintf ( sztime, ID_LEN_32, "%04d-%02d-%02d %02d:%02d:%02d", today->tm_year + 1900, today->tm_mon + 1, today->tm_mday, today->tm_hour, today->tm_min, today->tm_sec );

    //������Ĳ����͵�ǰʱ�䴮����ʽ��Ϊһ����������־��Ϣ
    //const std::string& priorityName =nsCLog::getPriorityName(nPriority);

    message << "[TIME: " << sztime << "] [Level: " << _nPriority << "] [FileName: " << _FileName
    << "] [Position: " << _nPosition << "] " << std::endl <<
    " |-------------------------------------------------Message-------------------------------------------------|"
    << std::endl<<_szLogInfo << std::endl;

    return message.str();

}

void CFileLog::setInfo ( const char* _moduleName )
{
    //GetSource(_moduleName);
    m_moduleName = _moduleName;
    m_strFileName = CreateFileName();
}
