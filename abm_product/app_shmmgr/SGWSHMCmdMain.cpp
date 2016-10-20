#include "SHMCmdSet.h"
#include <iostream>
using namespace std;

void showvalue(vector<SHMInfo> _vShmInfo)
{
	cout<<endl;
	for (int i =0; i<_vShmInfo.size(); i++) {
		cout<<_vShmInfo[i].m_sKey<<"(";
		cout<<_vShmInfo[i].m_lKey<<")"<<"   ";
		cout<<_vShmInfo[i].m_sOwner<<"   ";
		cout<<_vShmInfo[i].m_sIPCType<<"   ";
		cout<<_vShmInfo[i].m_sDataFrom<<"   ";
		cout<<_vShmInfo[i].m_sDataNbr<<"   ";
		cout<<_vShmInfo[i].m_iUsed<<"   ";
		cout<<_vShmInfo[i].m_iAllSize<<"   ";
		cout<<_vShmInfo[i].m_sUseRate<<"   ";
		cout<<_vShmInfo[i].m_iNattch<<"   ";
		cout<<_vShmInfo[i].m_iTotal<<"   ";
		cout<<_vShmInfo[i].m_sLoadTime<<"   "<<endl;
	}
	cout<<endl;
}

int run_onlyone(const char *filename)
{
	int  fd;
	char buf[15];
	memset(buf,'\0',sizeof(buf));
	if ((fd = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0)
		return -1;
	// try and set a write lock on the entire file
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;

	if (fcntl(fd, F_SETLK, &lock) < 0) {
		if (errno == EACCES || errno == EAGAIN) {
			cout<<"ʹ����ͬ��������PROCESS_ID�Ľ��̲��ܶ�����У�"<<endl;
			exit(0);     // gracefully exit, daemon is already running
		} else {
			cout<<"�����쳣�������˳���"<<endl;
			close(fd);
			return -1;
		}
	}
	// truncate to zero length, now that we have the lock
	if (ftruncate(fd, 0) < 0) {
		close(fd);
		return -1;
	}
	// and write our process ID
	sprintf(buf, "%ld\n", (long)getpid());
	if (write(fd, buf, strlen(buf)) != (int)strlen(buf)){
		close(fd);
		return -1;
	}

	// leave file open until we terminate: lock will be held
	return 0;
}


//�ڴ�������
int run()
{
	int pid = fork();
	if (pid == 0) {
		char *p =getenv("SGW_HOME");
		char sFile[128] = {0};
		snprintf(sFile,sizeof(sFile)-1,"%s/src/app_petri/sgwshmadmin_n",p);
		run_onlyone(sFile);

		// 		SHMCmdSet::unloadSHMALL();
		// 		SHMCmdSet::createSHMALL();
		// 		SHMCmdSet::clearSHMALL();
		// 		SHMCmdSet::loadSHMALL();

		sleep(2);
		cout<<"�����ڴ������������ɹ�����"<<endl;
		while (true) {
			sleep(30);
		}
		exit(0);
	} else if (pid == -1) {
		cout<<"�����ڴ�����������ʧ�ܡ���" <<endl;
	} else {
		exit(0);
	}
	return 0;
}

void help()
{
	printf("\n---------------------------------------------------\n");
	printf("    sgwshmtools -c				[�������й����ڴ����������������]\n");
	printf("    sgwshmtools -c �ļ���			[����ĳ�ļ��Ĺ����ڴ���������������]\n");
	printf("    sgwshmtools -p �ļ���			[���������ڴ�ȫ������]\n");
	printf("    sgwshmtools -v �ļ���			[���������ڴ�ȫ������]\n");
	printf("    sgwshmtools -l				[�������й����ڴ����������������]\n");
	printf("    sgwshmtools -l �ļ���			[����ĳ���ļ��Ĺ����ڴ���������������]\n");
	printf("    sgwshmtools -q				[��ѯ���й����ڴ���Ϣ]\n");
	printf("    sgwshmtools -q �ļ���			[��ѯĳ���ļ��Ĺ����ڴ�]\n");
	printf("    sgwshmtools -d				[ж�����й����ڴ�]\n");
	printf("    sgwshmtools -d �ļ���			[ж��ĳ���ļ��Ĺ����ڴ�]\n");
	printf("    sgwshmtools -k				[������й����ڴ�����]\n");
	printf("    sgwshmtools -k �ļ���			[���ĳ���ļ��Ĺ����ڴ�����]\n");
	printf("    sgwshmtools -r �ļ���			[���·���ĳ���ļ������ڴ�ռ��С]\n");
	printf("    sgwshmtools -m 				[��֤���Ĳ��������Ƿ���ȷ]\n");
	printf("    sgwshmtools -n 				[���������ڴ�������]\n");
	printf("    sgwshmtools -e table			[���ݵ�������table��]\n");
	printf("    sgwshmtools -show FimeName (IndexName=Value)[��ʾĳ�ű����ڴ�����]\n");
	printf("---------------------------------------------------\n\n");
	printf("Ŀǰ֧�ֵ��ļ�����:\n    context_filter            tab_msg_map               service_context_list\
		   \n    stream_ctrl_info          stream_ctrl_data          userinfo\
		   \n    sgw_sla_que_rel           netInfodata               sessiondata\
		   \n    shmcongestleve            service_context_list_base loadbalancedata\
		   \n    base_method               service_package_route     s_packet_session\
		   \n    service_package_node      service_package_route_ccr service_package_judge\
		   \n    service_package_cca       service_package_func      service_package_variable\
		   \n    statisticscongestmsg      loadbalancedata           wf_process_mq\
		   \n    portinfo                  session_clear             sgw_org_route\
		   \n    sgw_org_head              channelInfo               sgw_global_map\
		   \n    sgw_user_staff            sgw_route_rule            sgw_area_org\
		   \n    sgw_tux_rela_in           sgw_tux_rela_out          sgw_svr_list\
		   \n    sgw_svrs_param_rela       sgw_svrs_param_list       sgw_svrs_register\
		   \n    sgw_param_ins_list        sgw_enum_area\n");
	printf("ע��:\n    sgw_org_route               sgw_org_head\
		   \n    ֧�ֵ��������������ݲ�֧��\n\n");
}

int main(int argc,char *argv[])
{
	if (argc<2||argv[1][0] !='-') {
		printf("\n[ERRNO]: �����������!\n\n");
		printf("[��ע]����ͨ�� sgwshmtools -h  ��ѯ�����ڴ������ʹ�÷�ʽ\n\n");
		return -1;
	}
	cout<<endl;

	try {
		switch(argv[1][1])
		{
		case 'c'://���������ڴ�
			{
				if (argc==2) {
					SHMCmdSet::createSHMALL();
				} else if (argc == 3) {
					SHMCmdSet::createoneSHM(argv[2]);
				}
				break;
			}
		case 'r':
			if (argc == 3||argc == 4) {
				if (!SHMCmdSet::ralloconeSHM(argv[2])) {
					SHMCmdSet::loadoneSHM(argv[2],argv[3]);
				}
			} else if (argc == 2){
				cout<<"�������·�����������ڴ�ռ��С��֧��ȫ������"<<endl;
			} else {
				cout<<"������������������ʽ�磺sgwshmtools -r userinfo filename ��ʽ"<<endl;
			}
			break;
		case 'q'://��ѯ
			{
				vector<SHMInfo> _vShmInfo;
				if (argc==2) {
					SHMCmdSet::querySHMALL(_vShmInfo);
					showvalue(_vShmInfo);
				} else if (argc == 3) {
					SHMCmdSet::queryoneSHM(argv[2],_vShmInfo);
					showvalue(_vShmInfo);
				}
				break;
			}
		case 'd'://ж��
			{
				if (argc==2) {
					SHMCmdSet::unloadSHMALL();
				} else if (argc == 3) {
					SHMCmdSet::unloadoneSHM(argv[2]);
				}
			}
			break;
		case 'k'://���
			{
				if (argc==2) {
					SHMCmdSet::clearSHMALL();
				} else if (argc == 3) {
					SHMCmdSet::clearoneSHM(argv[2]);
				}
			}
			break;
		case 'f'://�Ͽ�����
			{
				if (argc==2) {
					SHMCmdSet::detachSHMALL();
				} else if (argc == 3) {
					SHMCmdSet::detachoneSHM(argv[2]);
				}
			}
			break;
		case 'l'://����
			if (argc==2) {
				SHMCmdSet::loadSHMALL();
			} else if (argc == 3) {
				SHMCmdSet::loadoneSHM(argv[2]);
			}
			break;
		case 'h'://����
			{
				help();
				break;
			}
		case 'p'://���������ڴ�
			{
				if (argc==2) {
					cout<<"�������󣬹����ڴ��ݲ�֧��ȫ�������������ڴ�"<<endl;
				} else if (argc == 3) {
					SHMCmdSet::lockoneSHM(argv[2]);
				}
			}
			break;
		case 'v': //����
			{
				if (argc==2) {
					cout<<"�������󣬹����ڴ��ݲ�֧��ȫ�������������ڴ�"<<endl;
				} else if (argc == 3) {
					char _sVale[32] = {0};
					char _sCmd[BUFF_MAX_SIZE] = {0};
					FILE* fp  = NULL;
					sprintf(_sCmd,"ps -ef|grep -w \"sgwshmtools -p %s\" |grep -v grep |awk '{print $2}'",argv[2]);
					fp = popen (_sCmd, "r");

					if (fp == NULL) {
						cout<<"�ù����ڴ�δ����"<<endl;
						return 0;
					}
					if (feof(fp)) {
						pclose (fp);
						cout<<"�ù����ڴ�δ����"<<endl;
						return false;
					}
					if (fscanf (fp, "%s\n",_sVale) == EOF) {
						pclose (fp);
						cout<<"�ù����ڴ�δ����"<<endl;
						return false;
					} else {
						pclose (fp);
						memset(_sCmd,0,sizeof(_sCmd));
						snprintf(_sCmd,sizeof(_sCmd),"kill -9 %d",atoi(_sVale));
						system(_sCmd);
						cout<<"�����ڴ�����ɹ�"<<endl;
					}
				}
			}
			break;
		case 'n':
			{
				cout<<"���������ڴ������̡���"<<endl;
				run();
				break;
			}
		case 'm':
			{
				cout<<"��ʼ��֤�����ڴ����������Ĳ���������Ϣ�Ƿ���ȷ����"<<endl;
				if (!SHMCmdSet::checkParam()) {
					cout<<"�����ڴ����������Ĳ���������Ϣ��֤�ɹ�����"<<endl;
				} else {
					ThreeLogGroup _oLogObj;
					_oLogObj.Product_log(MBC_PARAM_ERROR,-1,-1,-1,2,2,-1,NULL,"�����ڴ����������Ĳ���������Ϣ���ɹ���������Ĳ��������ļ�");
				}
				break;
			}
		case 'e'://���ݵ���
			{
				if (argc==2) {
					//SHMCmdSet::exportSHMALL();
				} else if (argc == 4) {
					SHMCmdSet::exportOneSHMALL(argv[2],argv[3]);
				}
			}
			break;
		default:
			break;
		}

		if (strcmp(argv[1],"-show") == 0) {
			if (argc==2) {
				cout<<"������������������ʽ�磺sgwshmtools -show context_filter ��ʽ"<<endl;
				return -1;
			} else if (argc == 3) {
				SHMCmdSet::querySHMData(argv[2],argv[3]);
			} else {
				cout<<"������������������ʽ�磺sgwshmtools -show context_filter ��ʽ"<<endl;
				return -1;
			}
		}
	}catch (TOCIException & toe){
		cout<<toe.getErrCode()<<endl;
		cout<<toe.getErrMsg()<<endl;
		cout<<toe.getErrSrc()<<endl;
		return -1;
	} catch (Exception &e) {
		cout<<e.descript()<<endl;
		cout<<"errno="<<errno<<endl;
		return -1;
	} catch (SITEException &se) {
		cout<<se.getErrMsg()<<endl;
		cout<<se.getErrSrc()<<endl;
		cout<<se.getErrCode()<<endl;
		return -1;
	}
}
