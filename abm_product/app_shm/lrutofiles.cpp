/*VER: 1*/
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.
/// lrutofiles
/// zhangap 

#include <stdio.h>
#include "SHMLruData.h"
#include "LruAggrData.h"
#include "CommandCom.h"
#include "ReadIni.h"

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

  CommandCom* pCmdCom = new CommandCom();

  if(!pCmdCom){
      Log::log(0, "�ڴ����ʧ�ܣ�");
      //bool Pprintf(int iAlertCode,bool bInsertDB,int iFileID,char const *fmt,...);  
      Log::log(0, "lrutofiles�ڴ����ʧ�ܣ�");
      return -1;
  }


    if(! pCmdCom->InitClient() ){
        Log::log(0, "��ʼ��ʧ�ܡ�"); 
        return -1;
    };
    char sTemp[64]={0};


    unsigned int icnt = 0;
    int iWrnIter = 0;
    
    while(1){
    	//ˢ����
    	long lInitSize = pCmdCom->readIniInteger("MEMORY","testlru.init_data_size",1000);
      long lExtSize = pCmdCom->readIniInteger("MEMORY","testlru.ext_data_size",0);
      pLruAggr->setNextSize( lExtSize );  
      int iExtNum = pCmdCom->readIniInteger("MEMORY","testlru.max_extents",0);
      pLruAggr->setMaxExCnt( iExtNum);
      int iWriType = pCmdCom->readIniInteger("MEMORY","testlru.shm_write_type",0);
      pLruAggr->setState( iWriType);   
      int iExtHwm = pCmdCom->readIniInteger("MEMORY","testlru.ext_hwm",0);
      pLruAggr->setExthwm( iExtHwm );  
      int iDirtyPer = pCmdCom->readIniInteger("MEMORY","testlru.lru_dirty_max_per",0);    
      //pLruAggr->setFreshPct( iDirtyPer ); 
      int iHwmWarning = pCmdCom->readIniInteger("MEMORY","testlru.shm_hwm_warming",0);      
      pLruAggr->setWrnhwm( iHwmWarning ); 
      
      
      int iiMaxSize = pCmdCom->readIniInteger("MEMORY","testlru.max_shm_size",0);      
      pLruAggr->setExpandMaxSize( iiMaxSize );       
        
      if( pLruAggr->exist() ){
          icnt = pLruAggr->writeToFile();
          if(icnt)
                printf("write1  %u ��\n", icnt);
        }

      if(( pLruAggr->getCount()*100/pLruAggr->getTotal()  ) >= ( pLruAggr->getExthwm() ) ){
            printf("����Ԥ��ʹ�õķ�ֵ(%u%%),�Զ���չ\n", pLruAggr->getExthwm());
            pLruAggr->expandMem();
        }
        if(( pLruAggr->getCount()*100/pLruAggr->getTotal()  ) >= ( pLruAggr->getWrnhwm() ) ){
            if(iWrnIter%30 == 0)
                printf("�澯���������ݳ�����ˮλ%u%%��\n",  pLruAggr->getWrnhwm() );         
            iWrnIter++;
            
        }else{
            iWrnIter=0;
        }
        usleep(300000);
    }
  delete pCmdCom;

}



