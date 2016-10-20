#ifndef __HB_ADMIN__MGR_H__
#define __HB_ADMIN__MGR_H__

class HbAdminMgr
{	
	public:
		
		/*���캯��*/
		HbAdminMgr();
		
		/*��������*/
		virtual ~HbAdminMgr();
		
		/*��ȡ��¼��Ϣ*/
		static  int  getLogInfo(int argc, char **argv);
		
		/*�������ļ���ȡ��Ϣ*/
		static int  getConfInfo(const char * sIniHeader);
		
		/*��ȡ��¼�ɹ���ʧ����Ϣ*/
		static	int getAuthenticate();
		
		/*�ַ����и�*/
		static	int  CutString(const char * sInputUserInfo);
		
		/*�޳����ܺ�Ŀո�*/
		static int tickOutTab (char *ptempWd, const char *sPwd);
		
		/*��¼�ĳ�ʼ��*/
		static int  loginInit(const int ihasLog);
		
		/*��¼�ɹ���ʧ��״̬�ı��*/
		static  int authenticateInit(const int ihasSucess);
		
		/*�ѵ�¼��Ϣ��������Ϣ��֤*/
		static int doJude();
		
	private:
		
		/*������û�*/
		static char m_sLogUserName[20];
		
		/*���������*/
		static char m_sLogPassWord[20];
		
		/*�����ļ��û���*/
		static char m_sUserName[20];
		
		/*�����ļ�����*/
		static char  m_sPassWord[20];
		
		/*�����ļ�Ŀ¼*/
		static char m_sTIBS_HOME[20];
};
#endif