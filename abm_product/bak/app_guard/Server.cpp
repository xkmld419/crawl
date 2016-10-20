/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "Server.h"

extern "C" {

//��ȡ�������ַ
int GetShmAddress(TSHMCMA **ppTable,int iShmKey)
{
    int iShmID;
    
    //��ȡ�����ڴ�ID
    if ((iShmID = shmget(iShmKey,0,0))<0){
        return -1;
    }

    //get the buff addr.
    *ppTable = (TSHMCMA *)shmat(iShmID,(void *)0,0);
    
    if (-1 == (long)(*ppTable)){
        return -2;
    }

    return 0;
}

} // End extern "C"

