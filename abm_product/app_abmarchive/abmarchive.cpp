#include <string.h>
#include <stdio.h>

#include "AutoClearABMTT.h"
#include "AutoClearABMTT.h"
//#include "ThreeLogGroup.h"
//#include "Log.h"
#include "ABMException.h"


void PrintHelpInfo(){
    printf("ʹ�÷�����\n");
    printf("======================= ���ݵ�������:abmarchive version1.0.0 =======================\n"
           "abmarchive -m -d -k -f \n"
           "  -m �ڴ����ݿ��й鵵���� \n"
           "  -d ָ���鵵��ɾ��ʱ�� \n"
           "  -k ָ���鵵�����ڴ����ݿ����� \n"
           "  -f ָ���鵵�ļ���ŵ�λ��\n "
           "ʾ����\n"
           "  abmclear -m tablename -d 20110908 -k tt_node -f /root/abm/deploy/bin \n"
           "���ͣ�\n"
           "  ͬʱ��tt_node�ڴ����ݿ�ı�tablename��20110908֮ǰ�����ݹ鵵��Ŀ¼/root/abm/deploy/bin����,\n"
           "  ��ɾ�����ڴ����ݿ��б�tablename�Ѿ��鵵�����ݣ�\n");
}

int main(int argc, char *argv[])
{
    int oc=0;
    char sTableListName[1024+1];
    char sDBListName[1024+1]; 
    char sDBFileName[1024+1];
    ABMException oExp; 
    ABMClearProduct *poABMArchive=new ABMClearProduct();
       
    if(3 >= argc) {
        PrintHelpInfo();
        return -1;
    }
    
    memset(sTableListName, 0x00, sizeof(sTableListName));
    memset(sDBListName, 0x00, sizeof(sDBListName));
    
    char *paraOpt = "m:d:M:D:K:k:f:";
    while((oc=getopt(argc, argv, paraOpt)) != -1) {
        switch (oc) {
            case 'm':
                if(1024 < strlen(optarg)){
                    printf("���ݿ�������ϴ���1024\n");
                    exit(1);    
                }
                strcpy(sTableListName, optarg);
                sTableListName[strlen(optarg)] = '\0';
                break;
            case 'd':
                if(14 < strlen(optarg)){
                    printf("ʱ���ʽ����\n");
                    printf("eg:-d 20110908112233\n");
                }
                strcpy(poABMArchive->m_sDateTime, optarg);
                poABMArchive->m_sDateTime[14] = '\0';
                break;
            case 'M':
                if(1024 < strlen(optarg)){
                    printf("���ݿ�������ϴ���1024\n");
                    exit(1);    
                }
                strcpy(sTableListName, optarg);
                sTableListName[strlen(sTableListName)] = '\0';
                break;
            case 'D':
                if(14 < strlen(optarg)){
                    printf("ʱ���ʽ����\n");
                    printf("eg:-d 20110908112233\n");
                }
                strcpy(poABMArchive->m_sDateTime, optarg);
                poABMArchive->m_sDateTime[14] = '\0';                
                break;
            case 'k':
                if(1024 < strlen(optarg)){
                    printf("���ݿ������ϴ���1024\n");
                    exit(1);    
                }
                strcpy(sDBListName, optarg);
                sDBListName[strlen(optarg)] = '\0';
                break;
                //g_iClearNodeID = atoi(optarg);
                //break;
            case 'f':
                strcpy(sDBFileName, optarg);
                sDBListName[strlen(optarg)] = '\0';
                break;
            default:
                printf("������δ֪���� %c\n", oc);
                break;            
        }
    }
    if(0 == strcspn(sDBFileName,"/")){
	    //m_poLogGroup->log(MBC_PARAM_THREAD_LOST_ERROR,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_PROC,-1,NULL,"����ļ�Ϊȫ·��");
        //Log::log(0, "����ļ�Ϊȫ·��!");
	} else {
	    //m_poLogGroup->log(MBC_PARAM_THREAD_LOST_ERROR,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_PROC,-1,NULL,"����ļ�·����������·�����");
	    //Log::log(0, "����ļ�·����������·�����!");
	} 
	poABMArchive->m_poArchiveData=fopen(sDBFileName,"w+"); 
	if(poABMArchive->m_poArchiveData == NULL) {
		//m_poLogGroup->log(MBC_FILE_OPEN_FAIL,LOG_LEVEL_ERROR,LOG_CLASS_ERROR,LOG_TYPE_PROC,-1,NULL,"���ļ�ʧ��[%s]",m_sFileName);
		return -1;
	}    
    poABMArchive->clearInit(oExp);
    poABMArchive->executeArchiveSQL(oExp, sTableListName, sDBListName);
    poABMArchive->executeArchive(oExp, sDBListName);
    poABMArchive->getABMClearConf(oExp, sTableListName, sDBListName);
    poABMArchive->executeClear(oExp);
    return 0;    
}