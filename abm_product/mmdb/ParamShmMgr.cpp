#ifndef PRIVATE_MEMORY
#include "ParamShmMgr.h"
#include "IpcKey.h"
#include "Log.h"
#include "MBC.h"
#include "fee_shm.h"


ParamShmMgr::ParamShmMgr()
{
    //��ʼ����ڹ����ڴ��key���ź���
    m_pParamAccessPort = NULL;
    m_pShmStateLockSem = NULL;
    m_lShmStateKey = IpcKeyMgr::getIpcKey(-1, "SHM_PARAMDATA_STATE");
    m_lShmStateLockKey = IpcKeyMgr::getIpcKey(-1, "LOCK_PARAMDATA_STATE");
    if(m_lShmStateKey > 0L && m_lShmStateLockKey > 0L)
    {
        m_pShmStateLockSem = new CSemaphore();
        if(NULL == m_pShmStateLockSem)
            THROW(MBC_Semaphore);                   //��������ʧ�� �����Ƿ����㹻���ڴ�
    }
    else
    {
        Log::log(0, "[����]:�����ڴ����ô���!����ȷ���ù����ڴ��KEY");
        THROW(MBC_ParamShmMgr+1);                   //����������ڵĹ����ڴ����ô���
    }

    //��ʼ��������Ϣ��
    m_pParamDataBuf[PARAM_SHM_INDEX_A] = new ParamInfoMgr(PARAM_SHM_INDEX_A);
    m_pParamDataBuf[PARAM_SHM_INDEX_B] = new ParamInfoMgr(PARAM_SHM_INDEX_B);

    //���ݲ�����Ϣ��ı�ų�ʼ����Ӧ�Ĳ������ڵĲ��������ڴ��key
    m_pParamDataBuf[PARAM_SHM_INDEX_A]->bindKey(PARAM_SHM_INDEX_A);
    m_pParamDataBuf[PARAM_SHM_INDEX_B]->bindKey(PARAM_SHM_INDEX_B);

    //���ݲ�����Ϣ��ı�����Ӷ�Ӧ�Ĳ������ڵĲ��������ڴ�
    m_pParamDataBuf[PARAM_SHM_INDEX_A]->attachAll();
    m_pParamDataBuf[PARAM_SHM_INDEX_B]->attachAll();


}

ParamShmMgr::~ParamShmMgr()
{
    //������ڹ����ڴ�
    detachShmByData(m_pParamAccessPort);
    
    //ɾ����ڹ����ڴ���ź���
    if(m_pShmStateLockSem != NULL)
    {
        delete m_pShmStateLockSem;
        m_pShmStateLockSem = NULL;
    }
}


void ParamShmMgr::unloadShmByFlag(int iFlag)
{
    if(iFlag == PARAM_SHM_INDEX_A){
		if(m_pParamDataBuf[PARAM_SHM_INDEX_A])
		{
			m_pParamDataBuf[PARAM_SHM_INDEX_A]->empty();
        	m_pParamDataBuf[PARAM_SHM_INDEX_A]->unload();
        	delete m_pParamDataBuf[PARAM_SHM_INDEX_A];
        	m_pParamDataBuf[PARAM_SHM_INDEX_A] = 0;
		}
    } else {
		if(m_pParamDataBuf[PARAM_SHM_INDEX_B])
		{
			m_pParamDataBuf[PARAM_SHM_INDEX_B]->empty();
        	m_pParamDataBuf[PARAM_SHM_INDEX_B]->unload();
        	delete m_pParamDataBuf[PARAM_SHM_INDEX_B];
        	m_pParamDataBuf[PARAM_SHM_INDEX_B] = 0;
		}
    }
}
void ParamShmMgr::loadShm()
{
    //��ǿ�Ʒ�ʽж�����еĹ����ڴ�,������κν����Ѿ���������3�鹲���ڴ������쳣�˳�
    unloadAllShm(false);
    
    //���ȴ������ݷ��ʵĹ����ڴ�
    allocDataAccessShm();
    m_pShmStateLockSem->P();
		Log::log(0,"��������A�����ڴ棬���Ժ�...");
    //����load()�����������
	try{
    if(!m_pParamDataBuf[PARAM_SHM_INDEX_A]->load())
	{
		Log::log(0,"���ع����ڴ����ʧ��,�������ݿ���������!");
		return;
	}
	}catch(...)
	{
		Log::log(0,"���ع����ڴ�ʧ��");
		return;
	}

    m_pParamAccessPort->m_iShmIdx = PARAM_SHM_INDEX_A;
	m_pParamAccessPort->m_iFlashSeqIdx = 0;
	m_pParamAccessPort->cState = 'R';
		Log::log(0,"��������B�����ڴ棬���Ժ�...");
    if(!m_pParamDataBuf[PARAM_SHM_INDEX_B]->load())
	{
		Log::log(0,"���ع����ڴ����ʧ��,�������ݿ���������!");
		return;
	}

    m_pShmStateLockSem->V();
    detachShmByData(m_pParamAccessPort); 
    
}


void ParamShmMgr::flashShm()
{
    int iSpareShmID;
    int iWorkShmID;

    //���������ڹ����ڴ�ʧ��,˵����ڹ����ڴ����û�д���
    //���õ���������ڴ�
    if(linkDataAccessShm() == false)
    {
        Log::log(0,"�������������ʹ����ڴ�ʧ��! �����ɳ���������.."); 
        loadShm();    //���ÿ��ٵ�1�������ڴ�ʹ��
        return;
    }

    m_pShmStateLockSem->P();

    iWorkShmID = m_pParamAccessPort->m_iShmIdx;
    if(iWorkShmID == PARAM_SHM_INDEX_A)
        iSpareShmID = PARAM_SHM_INDEX_B;
    else
        iSpareShmID = PARAM_SHM_INDEX_A;

    //����load()�����������(������ǵ�ǰ�л�ǰ���п�)
	try{
    if(!m_pParamDataBuf[iSpareShmID]->load())
	{
		Log::log(0,"�л������ڴ�ʧ��,�������ݿ���������,��ǰ�����ڴ�ָ��[%d]",iWorkShmID);
		return;
	}
	}catch(...)
	{
		Log::log(0,"�л������ڴ�ʧ��,��ǰ�����ڴ�ָ��[%d]",iWorkShmID);
		return;
	}
    m_pParamAccessPort->m_iShmIdx = iSpareShmID;   //�л�
	m_pParamAccessPort->m_iFlashSeqIdx++;
	Log::log(0,"�л������ڴ��[%d]��[%d]",iWorkShmID,iSpareShmID);
	Log::log(0,"ˢ�´���[%d]",m_pParamAccessPort->m_iFlashSeqIdx);
    Log::log(0,"�ȴ�10��......������տ��й����ڴ������."); 
    sleep(10);  //����ȴ�30����Ϊ�˹��������Ѿ�ȡ���˹����ڴ��������ʹ��,�ȴ�ʹ�ý������������

    //����unload()����ж�ز���(ж�ص��ǵ�ǰ�л�����п�)
    //ֻɾ������,����ɾ�������ڴ�,�Ա��ֹ����ڴ�Ĵ���
    //m_pParamDataBuf[iWorkShmID]->empty();


    m_pShmStateLockSem->V();
    detachShmByData(m_pParamAccessPort);

}


//�ṩ���ⲿ����,����ɾ������2�鹲���ڴ沢�ͷ��ڴ���Դ
void ParamShmMgr::unloadAllShm(bool bForce)
{
    //��ǿ�Ʒ�ʽж�ط�����ڹ����ڴ�,�����������������ʹ�øù����ڴ����׳��쳣
    unloadShmByKey(m_lShmStateKey, bForce);

    //��ǿ�Ʒ�ʽж�ز��������ڴ�,�����������������ʹ�øù����ڴ����׳��쳣
    m_pParamDataBuf[PARAM_SHM_INDEX_A]->unload();
    m_pParamDataBuf[PARAM_SHM_INDEX_B]->unload();

}

void ParamShmMgr::unloadShmByKey(const long lShmKey, bool bForce)
{
    int iNattach = 0;  //�����ڹ����ڴ��ϵĽ�����

    if(bForce == true)
    {
        deleteShmByKey(lShmKey);
    }
    else
    {
        //��ȡ�ù����ڴ�����ӽ��̸���
        iNattach = GetShmNattch(lShmKey);
        if(iNattach == 0)
        {
            deleteShmByKey(lShmKey);
        }
        else if(iNattach > 1)
        {
            Log::log(0,"������������ʹ��,�޷�ɾ�������ڴ�[%ld]", lShmKey);
            THROW(MBC_ParamShmMgr+6);
        }
        else
        {
            Log::log(0,"��ȡ�����ڴ�����ӽ�����ʧ��, �����ڴ�KEY[%ld].", lShmKey);
            //THROW(MBC_ParamShmMgr+7);
        }
    }
}



bool ParamShmMgr::linkDataAccessShm()
{
    int  iErrCode;
    char sSemName[64];

    iErrCode = sizeof(struct ParamAccessPort);
    m_pParamAccessPort = (struct ParamAccessPort*)GetShm(m_lShmStateKey, &iErrCode, 0);

    if(1 == iErrCode)
    {
        detachShmByData(m_pParamAccessPort);
        Log::log(0, "���ӹ����ڴ�ʧ��, �����ڴ�[SHM_PARAMDATA_STATE]");
        return false;
    }
    else
    {
        sprintf(sSemName, "%d", (int)m_lShmStateLockKey);
        m_pShmStateLockSem->getSem(sSemName, 1, 1);

        return true;
    }
}


void ParamShmMgr::allocDataAccessShm()
{
    int  iErrCode = 0;
    char sSemName[64];

    m_pParamAccessPort = (struct ParamAccessPort*)AllocShm(m_lShmStateKey, 
                         sizeof(struct ParamAccessPort), &iErrCode);

    if(iErrCode == 1)
    {
        Log::log(0, "���乲���ڴ�ʧ��, �����ڴ�[SHM_PARAMDATA_STATE]");
        THROW(MBC_ParamShmMgr+4);  //���乲���ڴ�ʧ��  �����Ƿ����㹻���ڴ�
    }
    //��ʼ�������ڴ��е����ݽṹ��
    memset(m_pParamAccessPort, 0, sizeof(struct ParamAccessPort));  

    sprintf(sSemName, "%d", (int)m_lShmStateLockKey);
    m_pShmStateLockSem->getSem(sSemName, 1, 1);

}



void ParamShmMgr::detachShmByData(void *pData)
{
    if(pData)
    {
        DetachShm(pData);
    }
    Log::log(0,"��������Ĺ����ڴ�.");
}



void ParamShmMgr::deleteShmByKey(const long lShmKey)
{
    FreeShm(lShmKey);

    Log::log(0,"�ͷŲ����Ĺ����ڴ�.");
}

void ParamShmMgr::getStat()
{
    int iWorkShmID;

    //���������ڹ����ڴ�ʧ��,˵����ڹ����ڴ����û�д���
    //���õ���������ڴ�
    if(linkDataAccessShm() == false)
    {
        Log::log(0,"�������������ʹ����ڴ�ʧ��! ��ʹ�ó��򴴽�..");
        return;
    }

    m_pShmStateLockSem->P();

    iWorkShmID = m_pParamAccessPort->m_iShmIdx;

    m_pParamDataBuf[iWorkShmID]->getState();

    m_pShmStateLockSem->V();
    detachShmByData(m_pParamAccessPort);
    
}

void ParamShmMgr::setState(char cState)
{
	if(linkDataAccessShm() == false)
    {
        Log::log(0,"�������������ʹ����ڴ�ʧ��! ��ʹ�ó��򴴽�..");
        return;
    }
    m_pShmStateLockSem->P();
	Log::log(0,"����״̬Ϊ[%c]",cState);
	switch(cState)
    {
        case 'R'://run��������
        {
            m_pParamAccessPort->cState = cState;
        }
        break;
        case 'S'://stop��ͣ
        {
            m_pParamAccessPort->cState = cState;
        }
        break;
        case 'W'://error
        {
            m_pParamAccessPort->cState = cState;
        }
        break;
        default:
        {
            Log::log(0,"state[%c]��֧��!ֻ֧������̬R ��̬ͣS �쳣̬W",cState);
        }
        break;
    }
	m_pShmStateLockSem->V();
    detachShmByData(m_pParamAccessPort);
}

int ParamShmMgr::showCtlInfo()
{
	if(linkDataAccessShm() == false)
    {
        Log::log(0,"�������������ʹ����ڴ�ʧ��! ��ʹ�ó��򴴽�..");
        return 1;
    }
	printf("-----------�ܿ���Ϣ-----------\n");
	printf("����ָ���ڴ��[%d]\n",m_pParamAccessPort->m_iShmIdx);
	printf("�ڴ��״̬[%c]\n",m_pParamAccessPort->cState);
	printf("����ˢ�´���[%d]\n",m_pParamAccessPort->m_iFlashSeqIdx);
	printf("��չֵ[%ld]\n",m_pParamAccessPort->m_lExVal);
	if(m_pParamDataBuf[PARAM_SHM_INDEX_A])
		printf("�����ڴ��[%d]����������[%ld]\n",PARAM_SHM_INDEX_A,m_pParamDataBuf[PARAM_SHM_INDEX_A]->getConnnum());
	if(m_pParamDataBuf[PARAM_SHM_INDEX_B])
		printf("�����ڴ��[%d]����������[%ld]\n",PARAM_SHM_INDEX_B,m_pParamDataBuf[PARAM_SHM_INDEX_B]->getConnnum());
	printf("-----------������������Ϣ------------------\n");
	return 0;
}

#endif
