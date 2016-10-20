#ifndef _LIB_THREE_LOG_DEFINE_H_
#define _LIB_THREE_LOG_DEFINE_H_

#define  WRITEFILE          0                   //д�ļ�
#define  WRITESHARE         1                   //д����

//չʾ��
//#define  DISPLAYMAXCNT      3000                //����չʾά��
#define  DISPLAYMAXCNT     1000000        //����չʾά��


//��־�ȼ���ʶ	1-fatal��2-error��3-warning��4-info��5-debug
enum
{
	LOG_LEVEL_FATAL       =1,
	LOG_LEVEL_ERROR       =2,
	LOG_LEVEL_WARNING     =3,
	LOG_LEVEL_INFO        =4,
	LOG_LEVEL_DEBUG       =5
};


//��־����ʶ	1-����error/fatal����2-�澯��warning����3-��ͨ��Ϣ��debug/info��
enum
{
	LOG_CLASS_ERROR     =1,
	LOG_CLASS_WARNING   =2,
	LOG_CLASS_INFO      =3
};


//��־�����ʶ	1��ҵ����־��2��hbparam����������־��3 ϵͳ������־��4 ���̵�����־��5 hbaccess���ݿ�ؼ������޸���־

//��־���
enum
{
    LOG_TYPE_BUSI        =1,
    LOG_TYPE_PARAM       =2,
    LOG_TYPE_SYSTEM      =3,
    LOG_TYPE_PROC        =4,
    LOG_TYPE_DATEDB      =5
};


//��־��״̬
enum
{
    INACTIVE=1,                                 //����
    CURRENT,                                    //����
    ACTIVE                                      //���鵵
};

//���״̬
enum
{
    UNDOWN=1,                                   //δ�鵵
    ALLDOWN,                                    //�ѹ鵵
    NEEDDOWN,                                   //���鵵
    FILEDOWN                                    //�����
};

enum
{
		LOG_ACTION_INS    =1,
	  LOG_ACTION_UPD    =2,
	  LOG_ACTION_DEL    =3
};
//չʾ��

enum
{
    GROUPCREAT              =1,                 //���������ڴ�
    GROUPFREE               =2,                 //�ͷŹ����ڴ�
    GROUPFILEADD            =3,                 //�����־���Ա�ļ�
    GROUPDELETE             =4,                 //ɾ����־���Ա�ļ�
    GROUPPRINTF             =5,                 //��ʾ��־����Ϣ
    GROUPQUIT               =6,                 //�л���־��
    GROUPSET                =7,                 //���ù鵵
    GROUPBAK                =8,                 //�ֶ������ļ�
    GROUPOUT                =9,                 //����ļ���Ϣ
    FILETEST                =10,                //��������
    PIGEONHOLE              =11,                //�ֶ��鵵
    GROUPHELP               =12,                //��������ֲ�
    GROUPADD                =13,                //�����־��
    GROUPSTAT               =14,                //�鿴�ڴ�״̬
    GROUPPARAM              =15,                //չ�ֺ��Ĳ����޸���־
    GROUPMODULE             =16,                //��ģ�����Ʋ�ѯ��־
    GROUPPROCESS            =17,                //���������Ʋ�ѯ��־
    GROUPINFOLV             =18,                //����־�ȼ���ѯ��־
    GROUPCLASS              =19,                //��������Ʋ�ѯ��־
    GROUPTYPE               =20,                //���������Ʋ�ѯ��־
    GROUPCODE               =21,                //����־�����ѯ��־
    GROUPRESET              =23,                //��ʼ����չ����ڴ�
    GROUPTEST               =99                 //����
};

#endif  //_LIB_THREE_LOG_DEFINE_H_
