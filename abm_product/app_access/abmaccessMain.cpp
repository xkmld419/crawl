/* add by  zhangyz */
/* date:2010 09 15 */

#include "abmaccessAdmin.h"
#include "abmmainmenu.h"
#include "abmlogin.h"
#include "Log.h"
//#include "HbAdminMgr.h"
void showAdminHelp()
{
    printf("\n ****************************************************************   \n") ;
	  printf("  1:���뽻��ʽģʽ \n") ;
	  printf("     ex:abmaccess \n");
	  printf("  2:����������ģʽ \n");
    printf("     ex1:abmaccess ������� \n");
    printf("  3:ʹ������˵�� \n");
    printf("     ex:abmaccess help \n");
    printf(" *****************************************************************   \n") ;
}

int main(int argc, char* argv[])
{
	
	if(argc > 1){
		if(argc == 2 && strcmp(argv[1],"help")==0){
			showAdminHelp();
			return 0;
		}
	//	if(bIsLogin==-1)
	//	{
       	//	Log::log (0, "������ͳһ��֤������������֤!лл!");
	//		return 0;
	//	}
	//	else if (bIsLogin == 0)
	//	{
       	//	Log::log (0, "�û�δ��½�����¼������!лл!");
	//		return 0;			
	//	}
		hbaccessAdmin *phbAdmin = new hbaccessAdmin();
		phbAdmin->run();
		delete phbAdmin;
		phbAdmin = 0;
		return 0;
		
	}	
	else if(argc == 1) 
	{
	/*�����ж��û��Ƿ��Ѿ���½*/
		//if(bIsLogin==-1)
		//{
       		//Log::log (0, "������ͳһ��֤������������֤!лл!");
		//	return 0;
	//	}	
	//	else if (bIsLogin == 0)
	//	{
       	//	Log::log (0, "�û�δ��½�����¼������!лл!");
	//		return 0;			
	//	}
  //���뽻��ģʽ    
	   try {
	        signal (SIGINT, SIG_IGN);
	        signal (SIGQUIT, SIG_IGN);
	                
	        startwin();
	        //hblogin lg;
	        //lg.run();
	        hbmainmenu mm;
				  mm.run();
	        endwin();
	    }
    //# OCI Exception
    catch (TOCIException &e) {
        cout<<"[EXCEPTION]: TOCIException catched"<<endl;
        cout<<"  *Err: "<<e.getErrMsg()<<endl;
        cout<<"  *SQL: "<<e.getErrSrc()<<endl;
        Log::log (0, "�յ�TOCIException�˳�\n[TOCIEXCEPTION]: %s", e.getErrMsg());
    }

    //# Exception
    catch (Exception &e) {
        cout << "[EXCEPTION]: " << e.descript () << endl;
        Log::log (0, "�յ�Exception�˳�\n[EXCEPTION]: %s", e.descript());
    }

    //# Other Exception
    catch (...) {
        cout << "[EXCEPTION]: Exception catched" <<endl;
        Log::log (0, "�յ�����Exception�˳�");
    }
	}
	//if(cHbLogin)
	//{
	//	delete cHbLogin; 
	//	cHbLogin = NULL;
	//}
    return 0;
}
