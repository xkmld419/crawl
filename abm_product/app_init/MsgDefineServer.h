#ifndef _MSG_DEFINE_SERVER_H_
#define _MSG_DEFINE_SERVER_H_

#define				SRC_ROOT								"\\ROOT"
#define				SRC_SHM								"\\ROOT\\SHM"
#define				SRC_RETURN						"\\ROOT\\RETURN"

#define				SRC_ROOT_APP_HBADMIN			"\\ROOT\\1_APPLICATION"
#define				SRC_ROOT_APP_SCHEDLUE			"\\ROOT\\1_APPLICATION"
#define				SRC_ROOT_APP_OTHER					"\\ROOT\\2_APPLICATION"
#define				SRC_ROOT_APP_INFOMONITOR	"\\ROOT\\2_INFOMONITOR"
#define				SRC_ROOT_APP_ASSISTPROC		"\\ROOT\\1_ASSISTPROC"
#define				SRC_ROOT_APP_MEMMGR		"\\ROOT\\1_MEMMGR"

#define				ROLE_MANAGER							1			//һ�Ž���
#define				ROLE_BUSINESS							2			//ҵ�����
#define				ROLE_INFOMONITOR					3			//	��ؽ���

#define				ROLE_MEMMGR						4

#define				MSG_REGISTER						0x10000001			//ע����Ϣ
#define				MSG_SUBSCRIBE					0x10000002			//ԤԼ��Ϣ
#define				MSG_BROAD							0x10000003			//�㲥��Ϣ
#define				MSG_UNREGISTER				0x10000004			//ע����Ϣ
#define				MSG_RETURN						0x10000005			//������Ϣ

#define				MSG_START							0x10000006			//Ft6/7~Nq
#define				MSG_PROCESS_ONEXIT		0x10000007
#define				MSG_PROCESS_EXIT		0x10000008

#define				MSG_OP_RETURN				0x20000000			//ȷ����Ϣ
//#define				MSG_OP_ERROR				0x20000001
#define				MSG_OP_RETURN_OK				0x20000000			//ȷ����Ϣ
#define				MSG_OP_RETURN_ERROR				0x20000001

#define				MASK_MSG_TYPE				0xFFFF0000
#define				MASK_RETURN_MSG			0xF0000000
#define				MASK_OP_MSG					0x0FFFFFFF

#define				MSG_PARAM_SHM_ATTACH	0x03001100	

#define             MSG_PROCESS_START           0x04000001
#define             MSG_PROCESS_STOP            0x04000002
#define             MSG_PROCESS_INFO            0x04000003

#define				MSG_PROCESS_STOP_SIG		0x05000002			//	����ֹͣ�ź�
#define				MSG_PROCESS_START_SIG		0x05000001			//���������ź�

#define				MSG_SYSTEM_FILE_SYSTEM	0x06000001			//��ȡ�ļ��洢��Ϣ

#define				MSG_MEM_CREATE					0x07000001			//���������ڴ�
#define				MSG_MEM_DESTORY				0x07000002			//���ٹ����ڴ�
#define				MSG_PARAM_SHM_ATTACH	0x03001100		//�ҽӲ��������ڴ�
#define				MSG_PARAM_SAVE_IT				0x03001101		//CmdParser�Լ�����ı���
#define				MSG_PARAM_LOG_PATH			0x03001102	



#define				INIT_ERROR									100001000		
#define				INIT_ERROR_REGISTER				100001100		//ע��ʧ��
//@@	100001100 ע��ʧ�� �����û����Ӧע����Ϣ
#define				INIT_MESSAGEQUEUE_DUP		100001101		//��Ϣ�����ظ�
//@@	100001101 ͬһ������Ϣ���������ظ�  ͬһ������Ϣ���������ظ��޸�����
#define				INIT_ERROR_CREATE_SENDER	100001102		//��Ϣ���д���ʧ��
//@@	100001200 ��Ϣ���д���ʧ��	����˷�����Ϣ���д���ʧ��
#define				INIT_ERROR_NO_PARAM			100001001		//����������
//@@ 100001001 ��ѯ���������� ������Ĳ������Ƿ���ڸĲ���
#define				INIT_ERROR_UNRECOGNIZED	100001002		//�����޷�ʶ��
//@@ 100001002 �����޷�ʶ�� ������޷�ʶ�������
#define				INIT_ERROR_PARAM_LOCK_KEY	100001201	//���Ĳ�����û������
//@@ 100001201 ���Ĳ�����û������ ���Ĳ�����û�����ü��ini�����ļ�
#define				INIT_ERROR_PARAM_MEM_KEY	100001202	//���Ĳ����ڴ��ַû��ָ��
//@@	100001202 ���Ĳ����ڴ��ַû��ָ�� ���Ĳ����ڴ��ַû��ָ�����ini�����ļ�
#define				INIT_ERROR_PARAM_MEM			100001300	
//@@	100001301	���Ĳ��������ڴ��С ���Ĳ��������ڴ��С

#define				INIT_ERROR_INI_FILE						100002000	//���Ĳ����ļ�
//@@	100002001	���Ĳ����ļ�������	�ļ������ڻ��߲��ɷ���	
//@@	100002002	���Ĳ����ļ�����		�ļ��ڲ�����ָ������
//@@  100002003	���Ĳ�������ʧ��		���Ĳ�������ʧ��




#define				INIT_PROCESS_ERROR			100004000	//Ft6/2NJ}
//@@	100004001	Ft6/2NJ}N^7(J61p	Ft6/2NJ}N^7(J61p
//@@	100004002	Ft6/D#J=N^7(J61p	Ft6/D#J=N^7(J61p=x3LFt6/J'0\
//@@	100004100	=x3LforkJ'0\		=x3LforkJ'0\
//@@	100004200	7~NqFt6/J'0\		7~NqFt6/J'0\
//@@	100004201	7~NqFt6/3,J1		7~NqFt6/3,J1
//@@	100004202	7~NqFt6/3I9&		7~NqFt6/3I9&
#define				INIT_OP_OK								000000000	//�ɹ�

#define				INIT_ERROR_DBACCESS					100005000

//#define				ROLE_INFOMONITOR					100006000

typedef struct  _PROCESS_EXEC_INFO_
{
        char sProcessName[256];
        char     sProcessPath[256];
        char sArgv[3][256];
		int	iProcessStartTimes;
	int	iPid;
	int	iProcessState;	
	int	iRunOnce;
	int 	iProcType;
}ProcessExecInfo;

#endif

