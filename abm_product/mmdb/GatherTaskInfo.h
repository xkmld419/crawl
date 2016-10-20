 /*VER: 3*/
// Copyright (c) 20091214,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef GATHERTASKINFO_H_HEADER_INCLUDED_BDAE6AE7
#define GATHERTASKINFO_H_HEADER_INCLUDED_BDAE6AE7


class GatherTaskInfo
{
    public:
		    GatherTaskInfo();
            ~GatherTaskInfo();
            
			int  m_iOrgID;
            int  m_iTaskID;
    	    
		    char m_sMinCreateDate[16];
		    char m_sMaxCreateDate[16];	
            
            char m_sDescPath[500];
            int  m_iCheckInterVal;
            char m_sComments[50];
            char m_sTaskType[10];
            int  m_iProcessID;
            
            //�ɼ��Ƿ���Ŀ¼���ļ�  0:��,Ĭ��  1:�ļ� 2  ��Ŀ¼  
    	    int m_iGatherMode; 
			int m_iBegin; //ʱ�俪ʼƥ���ƫ�� 
    	    int m_iEnd;   //ʱ�����ƥ���ƫ�� 
            
			//## Ĭ�ϵĽ�����ID
    		int m_iDefaultSwitchID;
			//## ��ǰ exch_id
    	    int m_iSwitchID;
    	    //�ļ���ͨ���
    	    char m_sFileNameWildcard[50];  //file_name_mask
    	    //Ŀ¼��ͨ���
    	    char m_sPathNameWildcard[50];  //path_name_mask
			///�ļ���ͨ�������
    		char m_sFileNameWcbak[50] ;
			//##�����޸Ĳɼ��ļ���
     		char m_sModifyFileName[50] ;
    	    //�ɼ�Ŀ¼·��
    	    //Directory m_oSrcDirectory;
    	    unsigned int m_iSrcDirectory;//����һ���±�
			unsigned int m_iDesDirectory;
			//����Զ�̱���Ŀ¼
			char m_sBackupPath[500];
			
			//��ʱ����
			char m_sTaskName[50];        	
    	    int  m_iFileNum;
			
    	    //FTP���
    	    char m_sSrcPath[500];
			char m_sOrgSrcPath[500]; ////ԭʼ��·���������ܴ����ڱ��� 
    	    int  m_iHostID;
    	    int  m_iPasv;
			int m_iListMode;
    	    char m_sIP[50];
    	    int  m_iPort;
    	    char m_sUserName[50];
    	    char m_sPassword[50];
    
            //## Դ�ļ�����ʱ��
            char m_sFileDate[15];
    	    	    
    	    char m_sSubPath[500];
    	    
    	    int m_iCheckInterval;
            int m_iTaskNum;
            
            int  m_iQcSend;
            char m_sQcPath[200]; 
            char m_sQcOrgPath[200]; ////ԭʼ·��������Ϣ
            int  m_iQcHostID;

            char m_sQcIP[50];
            char m_sQcUserName[50];
            char m_sQcPassword[50];
            int  m_iQcPort;
			int  m_iProcessID2;
            int  m_iBusiClass;
			int  m_iDataSource;
            unsigned int m_iNext;//��һ��
};

class GatherTaskListInfo
{
    public:
		    GatherTaskListInfo();
            ~GatherTaskListInfo();
            
			int  m_iTaskID;
			int  m_iMinInterval;
			long  m_iOrgStr;
			int  m_iCheckIntervalStr;
            unsigned int m_iNext;//��һ��
};

#endif