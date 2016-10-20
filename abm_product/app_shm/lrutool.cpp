/*VER: 1*/
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.
///testlru 
////LRU����ļ������ļ���С�� ���ļ��Ĳ��ԣ�
////����ϵͳ�Ļ�����ô�ָ� ��checkpoint�� ���ڴ��Ӱ��
////�ṩһ���������ļ��лָ������ʼ��ʱ��
////����Ǵ���: ����ļ�����,�ҿɻָ�������ļ��ָ����ݣ��������������ֱ�Ӵ����յ�
////����ǻָ�: ����ļ�����,�Ҵ�С���ʣ�����ļ��лָ��ϴε����ݣ�����ļ������⣬�򲻴��ڣ���ָ�ʧ��
////zhangap


///#define DEBUG_LRU


#include <stdio.h>
#include <time.h>
#include "SHMLruData.h"
#include "LruAggrData.h"
#include "CommandCom.h"
#include "ReadIni.h"

void lruPrintData(LruAggrData &t )
{
    printf("lServID:%d\n", t.lServID);
    printf("lAcctID:%ld\n", t.lAcctID);
    printf("lCustID:%ld\n", t.lCustID);
    printf("lProductID:%ld\n", t.lProductID);
    printf("lOfferID:%d\n", t.lOfferID);
    printf("sArea:%s\n", t.sArea);
    printf("sExpDate:%s\n", t.sExpDate);
    printf("sEffDate:%s\n", t.sEffDate);
}

void lruParseData(char * p, LruAggrData &t )
{
            memset(&t, 0, sizeof(LruAggrData) );

            if(p){ 
                t.lServID = atol(p);
            }
            
            if(p){ 
                p = strstr(p+1,"," );
                if(p) t.lAcctID = atol(p+1);
            }
            if(p){
                 p = strstr(p+1,"," );
                if(p) t.lProductID = atol(p+1);
            }
            if(p){
                 p = strstr(p+1,"," );
                if(p) t.lOfferID = atol(p+1);
            }
            if(p){
                p = strstr(p+1,"," );
                if(p) strncpy (t.sArea, p+1, sizeof(t.sArea) );
            }
            if(p){
                p = strstr(p+1,"," );
                if(p) strncpy (t.sExpDate, p+1, sizeof(t.sExpDate) );
            }
            if(p){
                p = strstr(p+1,"," );
                if(p) strncpy (t.sEffDate, p+1, sizeof(t.sEffDate) );
            }
    
}

void printHelp()
{
printf("%s",
    "ʹ�ò���˵��:\n"
    "-c	�����ڴ棬����������ļ�������лָ��ڴ棬���򴴽�����ļ�\n"
    "-d	ɾ�������ڴ棬��trunc����ļ�\n"
    "-q	ɾ�������ڴ棬��������ļ�\n"
    "-l	��ʾĿǰ���ڴ�״̬��Ϣ\n"
    "-l offset	��ʾoffset���ڵ�����\n"
    "-l offset1,offset2	��ʾ��offset1��offset2�����е��ڴ�����\n"
    "-p offset	��ʾoffset�����ڴ滹���ڴ��̣������ӡ���ݣ�����Ҳ�������ڴ�\n"
    "-p offset1,offset2	��ʾ��offset1��offset2�Ƿ����ڴ棬������ʾ������Ҳ������\n"
    "-v offset	ɾ��offset���ڵ�����\n"
    "-v offset,offset2	ɾ����offset1��offset2\n"
    "-i x,x,x,x,x	����һ�����ݵ��ڴ�\n"
    "-u offset x,x,x,x,x	��ָ��offset�����ݸ���\n"
    "-a count	�ɳ����Լ�����count�����ݲ��뵽�ڴ棬�����ָ��count��Ĭ�ϲ���һ������\n"
    "-x 	��̬��չ�ڴ��С\n"
    "-w 0|1|2|3  ����3��д��ʽ 0:�Զ�����,  1:��ʱд,  2:����д,  3:ʵʱд\n\n"
    );
}

    
int main( int argc, char **argv )
{
    ReadIni oRead;
    char * pPath = getenv("BILLDIR");
    char sTestLruFile[256]={0};
    char sFileName[512]={0};
    sprintf(sFileName, "%s/etc/abmconfig", pPath);
    long lTestLruKey =  oRead.readIniLong(sFileName, "MEMORY", "testlru.ipc_key_value", 1000);
    unsigned int iExCnt = oRead.readIniInteger(sFileName, "MEMORY", "testlru.max_extents", 4);
    unsigned int iNextSize = oRead.readIniInteger(sFileName, "MEMORY", "testlru.ext_data_size",1024*1024*10);
    oRead.readIniString(sFileName,"MEMORY","testlru.ext_file_name",sTestLruFile,NULL);
    //CommandCom* pCmdCom = new CommandCom();
      
    SHMLruData<LruAggrData> *  pLruAggr 
      = new SHMLruData<LruAggrData>(lTestLruKey, 
        sTestLruFile , 1,
        iNextSize, iExCnt) ;

    if( argc<=1 || argv[1][0]!='-'){
        printHelp();
        return 0;
    }

try{
    if( strcmp(&argv[1][1], "commit" )==0 ){
        if(!pLruAggr->exist() ){printf("�ڴ治���ڡ�"); return 0;}
        pLruAggr->commit();
        printf("�ύ�ڴ档\n");
        return 0;
    }

    
    if( strcmp(&argv[1][1], "rollback" )==0 ){
        if(!pLruAggr->exist() ){printf("�ڴ治���ڡ�"); return 0;}
        pLruAggr->rollback();
        printf("�ع��ڴ档\n");
        return 0;
    }
    
    
    switch(argv[1][1]){
        case 'c':
        {
            if(! pLruAggr->exist() ) {
            	//long lInitSize = pCmdCom->readIniInteger("MEMORY","testlru.init_data_size",1000);
               long lInitSize= oRead.readIniLong(sFileName, "MEMORY","testlru.init_data_size",1000);
                pLruAggr->create(lInitSize);
                if(! pLruAggr->exist() ) {
                    printf("���������ڴ�ʧ�ܡ�\n");
                    return 0;
                }
                printf("���������ڴ�ɹ�,��������1000����\n");
            }else{
                printf("�ڴ��Ѿ����ڣ����贴����\n");
            }
            return 0;
        }
        break;

        case 'd':
        {
            if( pLruAggr->exist() ){
                printf("�ڴ�ɾ����\n");
                pLruAggr->remove();
        
                printf("LRU����ļ���ա�\n");
                pLruAggr->truncFile();
            }else{
                printf("�ڴ汾���Ͳ����ڡ�\n");
            }
            return 0;
        }
        break;
        case 'q':
        {
            if(!pLruAggr->exist() ){printf("�ڴ治���ڡ�"); return 0;}
            printf("�ڴ�ɾ����\n");
            pLruAggr->remove();
            return 0;
        }
        break;
        case 'l':    
        case 'p': 
        {
            if(!pLruAggr->exist() ){printf("�ڴ治���ڡ�"); return 0;}
            if(argc == 2 ){
                pLruAggr->showDetail();

                
                return 0;
            }
            unsigned int iOffset1= atoi( &argv[2][0] );
            unsigned int iOffset2=  iOffset1;
            char * p = strstr( &argv[2][0],"," );
            if(p) 
                iOffset2= atoi( p+1);
            LruAggrData t;
            memset(&t, 0, sizeof(LruAggrData) );
            for(unsigned int i=iOffset1; i<=iOffset2 ; i++ ){
                try{
                    if('l' == argv[1][1]){
                        t = pLruAggr->getForRead( i );
                        printf("ƫ����%u��ֵΪ��\n", i);
                        lruPrintData(t);
                    }else{
                        if( !pLruAggr->getForMemRead( i, t ) ){
                            printf("ƫ����%u��ֵ�����ڴ档��\n", i);
                        }else{
                            printf("ƫ����%u�����ڴ棬ֵΪ��\n", i);
                            lruPrintData(t);
                        }
                        
                    }

                }catch(Exception &e){
                    printf("%s\n" , e.descript() );
                }

            }
            
        }
        break;
   

        case 'v':    
        {
            if(!pLruAggr->exist() ){printf("�ڴ治���ڡ�"); return 0;}

            unsigned int iOffset1= atoi(&argv[2][0] );
            unsigned int iOffset2=  iOffset1;
            char * p = strstr(&argv[2][0],"," );
            if(p) 
                iOffset2= atoi( p+1);

            for(unsigned int i=iOffset1; i<=iOffset2 ; i++ ){
                try{
                    pLruAggr->revoke( i );
                    printf("ɾ�� %u\n", i);
                }catch( Exception &e ){
                    printf("%s\n" , e.descript() );
                }

            }
            
        }
        break;
        case 'i':    
        {
            if(!pLruAggr->exist() ){printf("�ڴ治���ڡ�"); return 0;}
            if(argc < 3){
                printf("������������ݡ�\n");
                return 0;
            }
            LruAggrData t;
            lruParseData(&argv[2][0], t);
            unsigned int uiOffset = pLruAggr->malloc( t );
            printf("����ɹ���ƫ����Ϊ��%u,���Ժ��ֹ��ύ\n", uiOffset);
            return 0;

        }
        break;
        case 'u':    
        {
            if(!pLruAggr->exist() ){printf("�ڴ治���ڡ�"); return 0;}
            if(argc < 4){
                printf("������ƫ�����͸������ݡ�\n");
                return 0;
            }
            LruAggrData t;
            lruParseData(&argv[3][0], t);
            unsigned int uiOffset = atoi( &argv[2][0] ) ;
            (*pLruAggr)[uiOffset ] = t;
            
            printf("���³ɹ������Ժ��ֹ��ύ\n", uiOffset);
            return 0;
        }
        break;
        case 'a':    
        {
            if(!pLruAggr->exist() ){printf("�ڴ治���ڡ�"); return 0;}
            int j=1;
            if(argc >= 3){
                j = atoi(&argv[2][0] );
                if(j<=0){
                    j =1;
                }
            }
                
            
            unsigned int     uiOffset = 0;
            LruAggrData t;
            memset(&t, 0, sizeof(LruAggrData) );

            strcpy(t.sArea,"025");
            strcpy(t.sExpDate,"20100101000000");
            strcpy(t.sEffDate,"20501231595959");
            for(int i=1; i<=j; i++) {
                t.lServID=10000000+i;
                t.lAcctID=20000000+i;
                t.lCustID=30000000+i;
                t.lProductID=10000+i%1000;
                t.lOfferID=20000+i%1000;;
                uiOffset = pLruAggr->malloc( );
                if(!uiOffset){
                    printf("malloc faile  \n");
                    return 0;
                }
                (*pLruAggr)[uiOffset] = t;
                pLruAggr->commit();
            }
            
            if( j==1) printf("��������1��,ƫ����Ϊ%u \n", uiOffset);
            else printf("��������%d��.\n",j );
            return 0;
        }
        break;
        case 'x':    
        {
            if(!pLruAggr->exist() ){printf("�ڴ治���ڡ�"); return 0;}
            unsigned int iexpand = pLruAggr->expandMem( );
            printf("��չ�ڴ�ɹ�������%u���ṹ��ռ�\n", iexpand);
        }
        break;

        case 'w':    
        {
            if(!pLruAggr->exist() ){printf("�ڴ治���ڡ�"); return 0;}
            unsigned int j=0;
            
            if(argc >= 3)
                j = atoi(&argv[2][0] );
            else{
                printf("������д��ʽ\n");
                return 0;
            }
            unsigned int old = 0;
            long ltime=0;
            pLruAggr->getState( old, ltime);
            if(  pLruAggr->setState( j) ){
                printf("�ϴ�д��ʽΪ%u, ���ó�%u \n", old, j);
            }else{
                printf("����д��ʽʧ�ܡ�");
            }
        }
                
        break;
        
        default : 
            printHelp();
            break;
       }
       
}catch( ...){
    printf("�쳣�˳�\n" );
}

    return 0;
}
