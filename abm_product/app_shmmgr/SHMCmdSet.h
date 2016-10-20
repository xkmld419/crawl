#ifndef _SHMCMD_SET_H_HEAD_
#define _SHMCMD_SET_H_HEAD_
#include <vector>
#include "SHMDataMgr.h"
using namespace std;

class SHMCmdSet
{
public:
	//�������й����ڴ�
	static int createSHMALL();

	//ж�����й����ڴ�
	static int unloadSHMALL();

	//�Ͽ����й����ڴ������
	static int detachSHMALL();

	//���������ڴ����ݵ�ָ����table��
	//static int exportSHMALL(const char * sTab);

	//������й����ڴ�
	static int clearSHMALL();

	//��ѯ���й����ڴ��ʹ�����
	static int querySHMALL(vector<SHMInfo> &_vShmInfo);

	//�������й����ڴ�����
	static int loadSHMALL();

	//���������ڴ����ݵ�ָ����table��
	static int exportOneSHMALL(const char *_sType,const char * sTab);

	//����ĳһ���鹲���ڴ�
	static int createoneSHM(const char *_sType);

	//����ĳһ���鹲���ڴ�
	static int ralloconeSHM(const char *_sType);

	//չʾһ�鹲���ڴ���Ϣ
	static int queryoneSHM(const char *_sType,vector<SHMInfo> &_vShmInfo);

	//ж��ĳһ�鹲���ڴ�
	static int unloadoneSHM(const char *_sType);

	//���ĳһ�鹲���ڴ�
	static int clearoneSHM(const char *_sType);

	//�Ͽ�ĳһ�鹲���ڴ������
	static int detachoneSHM(const char *_sType);

	//����ĳһ�鹲���ڴ������
	static int loadoneSHM(const char *_sType,const char * _sFileName = NULL);

	//�������ڴ��������
	static int insertoneSHM(const char *_sType);

	//��ĳһ�鹲���ڴ�����
	static int lockoneSHM(const char *_sType);

	//��֤���Ĳ����Ƿ���ȷ
	static int checkParam();

	//��ѯ�����ڴ��keyֵ
	static int querySHMData(const char *_sType,char *_sKey=NULL,char *_sKey1 = NULL);
};
#endif
