#include "Process.h"
#include "Log.h"

Process *GetProcess();

extern int g_argc;
extern char ** g_argv;
extern Process *g_pProc;

int main(int argc, char **argv)
{
//	char sString[1024];

//	try
//	{

		//# ȫ�������в���, �ӽ������Ҫʹ��, ͨ��extern�ķ�ʽ����
		g_argc = argc;
		g_argv = argv;

		//# ��ȡ�����ַ, GetProcess �������ȫ�ֺ���
		//# �����ж����: DEFINE_MAIN(class_name) ����,�ɸú�ȥ�ṩGetProcess����
		g_pProc = GetProcess();
		//# run() Ϊ�麯��, ��ִ̬�������run()����
		g_pProc->run();

		delete g_pProc;

		NormalExitProcess();

	}
/*
	//# OCI Exception
	catch (TOCIException &e)
	{
		cout << e.getErrMsg() << e.getErrSrc() << endl;
		if (g_pProc)
		{
			delete g_pProc;
			g_pProc = 0;
		}

		Log::log(0, "�յ�TOCIException�˳�\n[TOCIEXCEPTION]: TOCIException catched\n"
			"  *Err: %s\n"
			"  *SQL: %s", e.getErrMsg(), e.getErrSrc());

		//		Log::log (0, sString);

		try
		{
			//Pprintf (0, 1, 0, sString);
		}
		catch (...)
		{
		}

		AbortProcess(-1); //�쳣�˳�,��֪���̵���ģ��
	}

	//# TException
	catch (TException &e)
	{
		if (g_pProc)
		{
			delete g_pProc;
			g_pProc = 0;
		}
		Log::log(0, "�յ�Exception�˳�\n[EXCEPTION]: %s", e.GetErrMsg());
		//		Log::log (0, sString);

		try
		{
			//Pprintf (0, 1, 0, sString);
		}
		catch (...)
		{
		}

		AbortProcess(-2);
	}

	//# Exception
	catch (Exception &e)
	{
		if (g_pProc)
		{
			delete g_pProc;
			g_pProc = 0;
		}
		Log::log(0, "�յ�Exception�˳�\n[EXCEPTION]: %s", e.descript());
		//		Log::log (0, sString);

		try
		{
			//Pprintf (0, 1, 0, sString);
		}
		catch (...)
		{
		}

		AbortProcess(-3);
	}
	//#TimesTen  Exception
	catch (TTStatus st)
	{
		cerr << "Error in  " << st << endl;
		Log::log(0, "%s", st.err_msg);
		AbortProcess(-4);
	}
	//# Other Exception
	catch (...)
	{
		if (g_pProc)
		{
			delete g_pProc;
			g_pProc = 0;
		}
		Log::log(0, "�յ�����Exception�˳�");
		//		Log::log (0, sString);

		try
		{
			//Pprintf (0, 1, 0, sString);
		}
		catch (...)
		{
		}

		AbortProcess(-5);
	}

}*/

