/**
* (C) Copyright 2007, linkage
* @version 	v1.0
* @author 	chenli
* @brief  otl���ӻ���������,�ṩ�˻�ȡ���ݿ�����,�ύ����,�ع�����,
					�Ͽ����ӵȲ���
*
* history:
* <p>chenli 2007-04-3 1.0 build this moudle</p>
*/
#ifndef __OCI_ENV_H__
#define __OCI_ENV_H__

#include "platform.h"

#include "TOCIQuery.h"

#include "platform.h"

#define DEFINE_OCICMD(x) 		TOCIQuery x(OciEnv::getInstance()->getOCIDatabase());
#define DEFINE_OCICMDPOINT(x) 		x = new TOCIQuery (OciEnv::getInstance()->getOCIDatabase());
class OciEnv
{
  public:
  
  static OciEnv * getInstance();
  /*********************************************************
	* ����˵�����������ݿ�,���ָ����sConnectstr��,����sConnectstr
							�������ݿ�;����ʹ��user,pass,tnsconnstr����ƴ���
							user/pass@connstr��ʽ���������ݿ�
	* ����˵����[in]sConnectstr		user/pass@connstr��ʽ�����Ӵ�
	* ����ֵ  ��true�ɹ�,falseʧ��
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
 	bool connectDb(const string &user="", const string &pass="", const string &tnsStr="");
 	/*********************************************************
	* ����˵��������OCI���ݿ�����Ӧ��
	* ����˵������
	* ����ֵ  ��otl_connect����
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
  TOCIDatabase* getOCIDatabase();
  
  /*********************************************************
	* ����˵�����������ݿ��û���
	* ����˵����[in]user���ݿ��û���
	* ����ֵ  ����
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
  void setUser(const string& user);
  /*********************************************************
	* ����˵�����������ݿ�����
	* ����˵����[in]pass	���ݿ�����
	* ����ֵ  ����
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
  void setPass(const string& pass);
  /*********************************************************
	* ����˵�����������ݿ�tns���Ӵ�
	* ����˵����[in]connStr	tns���Ӵ�
	* ����ֵ  ����
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
  void setConnStr(const string& connStr);
  	
  /*********************************************************
	* ����˵�����ύ����
	* ����˵������
	* ����ֵ  ����
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
  void commit();
  /*********************************************************
	* ����˵�����ر������Զ��ύ
	* ����˵������
	* ����ֵ  ����
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
  void auto_commit_off();
  /*********************************************************
	* ����˵�����������Զ��ύ
	* ����˵������
	* ����ֵ  ����
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
  void auto_commit_on();
  /*********************************************************
	* ����˵�����ع�����
	* ����˵������
	* ����ֵ  ����
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
	void rollback();
	/*********************************************************
	* ����˵�����Ͽ����ݿ�����
	* ����˵������
	* ����ֵ  ����
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
	void disconnect();
	/*********************************************************
	* ����˵�����ж��Ƿ����ӵ����ݿ�
	* ����˵������
	* ����ֵ  ����
	* ��  ��  ��ChenLi
	* ��дʱ�䣺2007-07-18
	**********************************************************/
	bool isConnected();
	
private:
	~OciEnv()
	{
		oci_database.disconnect();
	}
  OciEnv()
  {
  	Connected = false;
  	//otl_connect::otl_initialize();
  }//��ֹ�ⲿOciEnv����
  
  //otl���ݿ����Ӷ���
 	TOCIDatabase	oci_database;
	bool Connected;
	
 	string	m_userName;//�û���
	string	m_passWord;//����
	string	m_tnsStr;//tns���Ӵ�
  //static 	string	sConnStr;//���Ӵ���ʽuser/pwd@tnsStr
  	
  stringstream stream_last_error;//������¼����Ĵ�����Ϣ
  //static	string  last_error;//���������Ϣ 
};//ȫ�־�̬��

#endif


