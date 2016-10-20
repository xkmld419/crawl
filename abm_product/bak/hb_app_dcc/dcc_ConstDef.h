/*VER:1*/



#ifndef __DCC_CONST_DEF_H__

#define __DCC_CONST_DEF_H__



#include <vector>

#include <map>

#include <string>

#include <stdlib.h>


const int FILE_FIELD_LENGTH = 1024;

const int FILE_NAME_LENGTH = 128;

const int FILE_PATHER_LENGTH = 30;

const int SESSION_ID_LENGTH = 120;



const int FILE_NONE_TYPE = 0;

const int FILE_HEAD_TYPE = 1;

const int FILE_BODY_TYPE = 2;

const int FILE_TAIL_TYPE = 3;



//DCC��Ϣͷ�е�hop�ֶε�seq����

const int DCC_SEQ_STEP = 500;

// ��Ԫ����״̬
typedef enum {
    ONLINE,
    OFFLINE
} Bsn_state;

/*
 *0-P0_Online_����������CCR���ͳɹ��� ����dccUnPack�жϼ���CCR�Ƿ�ɹ���
 *1-P1_Online�����ɣ���Ϣ����������������������dccTranÿ�η���CCA���жϷ���CCA�������CCR������ĳ��ֵ��
 *2-P2_Online���ϣ���Ϣ�޷����� ����dccUnPack�жϣ��������������
 *3-P3_Maintenance�������˳� ����dccTran�жϸ������˳����ӽ��̸����˳���
 *4-P4_Offline�����������˳� ����dccTran�жϸ������˳����ӽ��̸����˳�������3�ظ�
 *5-P5_Offline_���ϣ��������˳� ����dccTran�жϷ�����Ϣʧ�ܣ�������SRͣ�����£�
 */
typedef enum {
    ONLINE_NORMAL,
    ONLINE_OVERLOAD,
    ONLINE_FAULT,
    MAINTENCE,
    OFFLINE_NORMAL,
    OFFLINE_FAULT
}Ne_state;


// �洢CCR��serviceType�������ڴ�
#define SHM_ITEM_NUM 10000      // serviceType�����ڴ�Ԫ�ظ���
#define SHM_ITEM_LEN 50        // serviceType�����ڴ�ÿ��Ԫ�صĳ���

#endif

