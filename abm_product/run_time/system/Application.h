#ifndef __APPLICATION_H_INCLUDED_
#define __APPLICATION_H_INCLUDED_

#define ENV_LEN 128
#define HOST_NAME_LEN 64

class ABMException;
class Application;

extern int g_iModuID;    //ģ��ID
extern int g_iProcID;    //ȫ�ֽ���ID
extern int g_iNodeID;    //ȫ�ֽڵ�ID
extern char g_sAppName[32];  //Ӧ�ó�������


/*��ȫ�˳��źź�*/
#define RECEIVE_STOP() ReceiveStop()
/*�źŴ���ע�ắ��*/
extern int SigHandlerSet(int sig, void (*handler)(int));
/*��ȫ�˳�����, trueΪ�յ���ȫ�˳��ź�*/
extern bool ReceiveStop(void);
extern bool RcvExcepSig(void);
extern void SigHandler(int sig);
#define DEFINE_MAIN(x) Application *GetApplication(){ return (new x()); }

class Application 
{	

public:

	Application();

	virtual ~Application();

	int appInit(ABMException &oExp); 

	virtual int init(ABMException &oExp) {
		return 0;
	}
	
	static int tryLock(const char *pfname, ABMException &oExp);

//	virtual int destory();

	virtual int run() = 0;		           

    char const * getHomeDir() { return m_sEnvAppDly; }

protected:

	char m_sEnvAppDly[ENV_LEN];//ABMAPP_DEPLOY��������

	char m_sHostName[HOST_NAME_LEN];   //��������

};

#endif



