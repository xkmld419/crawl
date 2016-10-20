#include "CmdSendRecv.h"
#include <signal.h>
#include <time.h>

CommandCom *CmdSendRecv::pCmd = 0;
//ProcessIDMgr *CmdSendRecv::pProID = 0;
CmdMsgFactory *CmdSendRecv::pMsgFactory = 0;
CmdSender* CmdSendRecv::pSender = 0;

int iProIDBak = -1;
void ProcessIDEnd ()
{
     /*ProcessIDMgr *pM = new ProcessIDMgr();
	 if(iProIDBak)
	 	pM->reSetProcessID(iProIDBak);
	 delete pM; pM = 0;*/
}

void signalCheck()
{
		/*signal( SIGINT,  (void (*)(int))ProcessIDEnd );
		signal( SIGQUIT,  (void (*)(int))ProcessIDEnd );
		//signal( SIGBUS,  (void (*)(int))ProcessIDEnd );
		//signal( SIGEMT,  (void (*)(int))ProcessIDEnd );
		signal( SIGILL,  (void (*)(int))ProcessIDEnd );
		//signal( SIGFPE,  (void (*)(int))ProcessIDEnd );
		signal( SIGIOT,  (void (*)(int))ProcessIDEnd );
		signal( SIGPWR,  (void (*)(int))ProcessIDEnd );
		signal( SIGSYS,  (void (*)(int))ProcessIDEnd );
		signal( SIGSTOP,  (void (*)(int))ProcessIDEnd );
		//signal( SIGXFSZ,  (void (*)(int))ProcessIDEnd );
		signal( SIGUSR1,  (void (*)(int))ProcessIDEnd );
		signal( SIGUSR2,  (void (*)(int))ProcessIDEnd );*/
}

CmdSendRecv::CmdSendRecv()
{
	m_iProcID = -1;
	m_iMsgID = -1;
	m_bRegist = false;
}

CmdSendRecv::~CmdSendRecv()
{
	if(pCmd)
		delete pCmd;
	//if(pProID)
	//	delete pProID;
	if(pMsgFactory)
		delete pMsgFactory;
	if(pSender)
		delete pSender;
}

void CmdSendRecv::rePerpare()
{
	 //m_iProcID = -1;
	 //iProIDBak = -1;
	 //m_iMsgID = -1;
	 iParamNum = 0;
	 //m_bRegist = false;
	 if(vec_CmdMsg.size())
	 {
	 	for(vector<CmdMsg*>::iterator it = vec_CmdMsg.begin(); it!= vec_CmdMsg.end(); it++)
		{
			delete (*it);
			(*it) = 0;
		}
	 }
	 vec_CmdMsg.clear();
	 vec_CmdMsgNew.clear();
	 if(!pCmd)
		pCmd = new CommandCom();
	 //if(!pProID)
	//	pProID = new ProcessIDMgr();
	 if(!pMsgFactory)
		pMsgFactory = new CmdMsgFactory();
}

 
//���ͽ�������
int CmdSendRecv::sendAndGetData(long lCMDID,char *sSect,char *sValue)
{
	/*rePerpare(); 
	try
	{ 		
		signal( SIGINT,  (void (*)(int))ProcessIDEnd );
		if(!m_bRegist){
			m_iProcID = pProID->loadProcessInfo();				
			if(m_iProcID){
				m_iMsgID = pProID->getMsgID(m_iProcID);
				m_bRegist = pCmd->InitClient(m_iMsgID);
				iProIDBak = m_iProcID;
			} else {
				return -1;
			}
		}
				
		int res = -1;
		if(m_bRegist)
		{
			variant var; 
			var = sValue;
			string *pStrName = new string (sSect);
			CmdMsg* pMsg = CommandCom::CreateNewMsg(lCMDID, *pStrName,var);//pCmd->m_pCmdSender->SendCmd(pMsg);
			while(1)
			{
				CmdMsg *pMsgTmp = pCmd->receive();		
				if(NULL == pMsgTmp)
				{
					break;	
				} else {
					delete pMsgTmp; pMsgTmp = 0;
				}
			}
			//signal( SIGINT,  (void (*)(int))ProcessIDEnd );
			if(!pCmd->send(pMsg))  
				return -1;
			delete pMsg; pMsg=0;
			delete pStrName; pStrName = 0;
		
			vec_CmdMsg.clear();
			int i = 1;
			while(1)
			{
        		if(i == 1) {
					sleep(1); i++;
				} else {
					if(lCMDID == MSG_PARAM_S_ALL)
					{
						if(i<LONG_WAIT_TIME){
							usleep(100); i++;
						} else {
							printf("�������ݳ�ʱ!\n");
							INFOPOINT(3,"[CmdSendRecv.sendAndGetData]%s","��ʾ���в���,�������ݳ�ʱ");
							return 0;
						}
					} else {
						if(i<WAIT_TIME){
							usleep(100); i++;
						} else {
							printf("�������ݳ�ʱ!\n");
							INFOPOINT(3,"[CmdSendRecv.sendAndGetData]%s","�������ݳ�ʱ");
							return 0;//��ȡ��Ϣʧ�� 
						}
					}
				}
				signalCheck();
				CmdMsg *pMsg = pCmd->receive();
				if(NULL!= pMsg) {
					vec_CmdMsg.push_back(pMsg);//vec_CmdMsgNew.push_back(*pMsg);
					break;
				}			
			}	
			res = vec_CmdMsg.size();	
		} else { 
			res = -1;
		}
		pProID->reSetProcessID(m_iProcID);
		return res;
	} catch(...)
	{
		if(m_iProcID)
			pProID->reSetProcessID();
		return -1;
	}*/	
}

// ת�����ݸ�ʽ(Ҫ��֤pParamHisRecord��ָ�ռ��㹻��)
long  CmdSendRecv::VecToSysParamInfo(ParamHisRecord *pParamHisRecord,vector<CmdMsg*> &vec_Msg)
{
}

//����ֵ=1����ֵ[�����Ǳ����ֵ];=2[�ɹ�,����������];
long  CmdSendRecv::VecToSysParamInfo(ParamHisRecord *pParamHisRecord,int &m_iParamNum)
{
		/*long res = -1;
		if(!pParamHisRecord)
		{
			m_iParamNum = 0;
			return res;
		}
		int i = 0;
		int iPos = 0;
		//iParamNum = 0;
		vector<CmdMsg*>::iterator it = vec_CmdMsg.begin();
		for(; it!= vec_CmdMsg.end(); it++,i++)
		{	
			SysParamHis *pParamHis=0;
		  	variant varRet;
		  	(*it)->GetMsgContent(varRet);
		  	int iSize = (*it)->GetVariantSize(varRet);
			
			// ��ȡ����LIST  		
			if(varRet.get_type() != variant::vt_ptr)
			{
				if(varRet.get_type()==variant::vt_long)
				{
					pParamHisRecord[0].lResult = varRet.asLong();
					m_iParamNum = 1;
					//pProID->reSetProcessID(m_iProcID);
					return 1;//pParamHisRecord[0].lResult;
				} else {
					return -1;
				}
			}
			if(iSize%sizeof(SysParamHis)!=0)
			{ 
						m_iParamNum = 0;
						return -1;
			}
		  	int iNum =iSize/sizeof(SysParamHis);
		  	void *pData = varRet.asPtr();			
			for(int j=0;j<iNum;j++,iPos++)
	 	 	{
	 			pParamHis = (SysParamHis*)(pData)+j;
				strcpy(pParamHisRecord[iPos].sSectName, pParamHis->sSectionName);
	 			strcpy(pParamHisRecord[iPos].sParamName, pParamHis->sParamName);
	 			strcpy(pParamHisRecord[iPos].sParamOrValue, pParamHis->sParamOrValue);
	 			strcpy(pParamHisRecord[iPos].sParamValue, pParamHis->sParamValue);
	 			strcpy(pParamHisRecord[iPos].sDate, pParamHis->sDate);
				pParamHisRecord[0].lResult = -30;
	 	 	}
			//delete (*it);
			//(*it) = 0;
		}
		m_iParamNum = iPos;
		iParamNum = iPos;
		*/
		return 2;
}

//����ֵ=0�ǽṹ��,>0�Ǵ�����.<0�쳣
long  CmdSendRecv::VecToSysParamInfo(ParamHisRecord *pParamHisRecord,CmdMsg *pCmdMsg)
{
	   		SysParamHis *pParamHis=0;
		  	variant varRet;
		  	pCmdMsg->GetMsgContent(varRet);
		  	int iSize = pCmdMsg->GetVariantSize(varRet);
			long lResult = -1;
			// ��ȡ����LIST  		
			if(varRet.get_type() != variant::vt_ptr)
			{
				if(varRet.get_type()==variant::vt_long)
				{
					lResult = varRet.asLong();
					pParamHisRecord[0].lResult = lResult;
					return lResult;//�����붼�Ǵ���0��LONGֵ
				} else {
					pParamHisRecord[0].lResult = -30;
					return -1;
				}
			}
			if(iSize%sizeof(SysParamHis)!=0)
			{ 
				return -1;
			}
		  	int iNum =iSize/sizeof(SysParamHis);
		  	void *pData = varRet.asPtr();	
			int iPos = 0;
			if(!pParamHisRecord)
				return -1;		
			for(int j=0;j<iNum;j++,iPos++)
	 	 	{
	 			pParamHis = (SysParamHis*)(pData)+j;
				strcpy(pParamHisRecord[iPos].sSectName, pParamHis->sSectionName);
	 			strcpy(pParamHisRecord[iPos].sParamName, pParamHis->sParamName);
	 			strcpy(pParamHisRecord[iPos].sParamOrValue, pParamHis->sParamOrValue);
	 			strcpy(pParamHisRecord[iPos].sParamValue, pParamHis->sParamValue);
	 			strcpy(pParamHisRecord[iPos].sDate, pParamHis->sDate);
				pParamHisRecord[0].lResult = -30;
	 	 	}
			iParamNum = iPos;
			return iParamNum;
}

//����˵��: ����ID,����,����ֵ,����ת�����ĵ�ַ
long CmdSendRecv::dealParamValue(long lCMDID,char *sSect,char *sValue,ParamHisRecord *pParamRecord,int &m_iParamNum)
{
	if(sendAndGetData(lCMDID, sSect,sValue))
	{
		return VecToSysParamInfo(pParamRecord,m_iParamNum);
	} else {
		return -1;
	}
}

//���ͽ�������
long CmdSendRecv::sendAndGetDataNew(long lCMDID,char *sSect,char *sValue,ParamHisRecord *pParamRecord,CommandCom *pCmdCom)
{
	//rePerpare();  	
	iProIDBak = -1;
	iParamNum = 0;	
	if(!pCmdCom)
		return -1;
	long res = -1;
	variant var;
	var = sValue;
	string *pStrName = new string (sSect);
	CmdMsg* pMsg = CommandCom::CreateNewMsg(lCMDID, *pStrName,var);
	while(1)
	{
			CmdMsg *pMsgTmp = pCmdCom->receive();		
			if(NULL == pMsgTmp)
			{
				break;	
			} else {
				delete pMsgTmp;
				pMsgTmp = 0;
			}
	}
	if(!pCmdCom->send(pMsg))
			return -1;
	delete pMsg;
	pMsg=0;
	delete pStrName;
	pStrName = 0;
		
	vec_CmdMsg.clear();
	int i = 1;
	while(1)
	{
        	if(i == 1)
			{
				sleep(1);
				i++;
			} else {
				if(lCMDID == MSG_PARAM_S_ALL)
				{
					if(i<LONG_WAIT_TIME){
						usleep(100);
						i++;
					} else {
						printf("�������ݳ�ʱ!\n");
						INFOPOINT(3,"[CmdSendRecv.sendAndGetDataNew]%s","��ʾ���в���,�������ݳ�ʱ");
						return 0;
					}
				} else {
					if(i<WAIT_TIME){
						usleep(100);
						i++;
					} else {
						printf("�������ݳ�ʱ!\n");
						INFOPOINT(3,"[CmdSendRecv.sendAndGetDataNew]%s","�������ݳ�ʱ");
						return 0;//��ȡ��Ϣʧ�� 
					}
				}
			}
			signalCheck();
			CmdMsg *pMsg = pCmdCom->receive();
			if(NULL!= pMsg)
			{
				iParamNum  =0;
				return VecToSysParamInfo(pParamRecord,pMsg);
				break;
			} 				
	}	
	return -1;
}

//���ͽ�������
long CmdSendRecv::sendAndGetDataEx(long lCMDID,char *sSect,char *sValue,ParamHisRecord *pParamRecord,CommandCom *pCmdCom,int iMsgID)
{
	//rePerpare();  	
	iProIDBak = -1;
	iParamNum = 0;	
	if(!pCmdCom)
		return -1;
	long res = -1;
	variant var;
	var = sValue;
	string *pStrName = new string (sSect);
	CmdMsg* pMsgT = CommandCom::CreateNewMsg(lCMDID, *pStrName,var);
	if(!pCmdCom->send(pMsgT))
	{
			INFOPOINT(3,"[CmdSendRecv.sendAndGetDataEx]%s","���ݷ���ʧ��");
			return -1;
	}
	delete pMsgT;
	pMsgT=0;
	delete pStrName;
	pStrName = 0;
	
	vec_CmdMsg.clear();
	int i = 1;
	while(1)
	{
        	if(i == 1)
			{
				sleep(1);
				i++;
			} else {
				if(lCMDID == MSG_PARAM_S_ALL)
				{
					if(i<LONG_WAIT_TIME){
						usleep(100);
						i++;
					} else {
						printf("�������ݳ�ʱ!\n");
						INFOPOINT(3,"[CmdSendRecv.sendAndGetDataEx]%s","��ʾ���в���,�������ݳ�ʱ");
						return 0;
					}
				} else {
					if(i<WAIT_TIME){
						usleep(100);
						i++;
					} else {
						printf("�������ݳ�ʱ!\n");
						INFOPOINT(3,"[CmdSendRecv.sendAndGetDataEx]%s","�������ݳ�ʱ");
						return 0;//��ȡ��Ϣʧ�� 
					}
				}
			}
			signalCheck();
			CmdMsg *pMsg = pCmdCom->receive();
			if(NULL!= pMsg)
			{
				iParamNum  =0;
				int iType = checkParam(pMsg);
				if(iType == 1)//ptr
				{
					delete pSender;
					pSender = 0;				
					return VecToSysParamInfo(pParamRecord,pMsg);
				} else {	
					//���·��õ���Ϣ����
					if(!pSender)
					{
						pSender = new CmdSender();
						pSender->Init(iMsgID,true);
					}
					pSender->SendData((void*)pMsg,sizeof(CmdMsg));
				}
			} 				
	}	
	return -1;
}

// ���õ��Ĳ��������Ƿ���LONG����(ptr - 1;long -2;string-3;4-bool;5-date;6-float;7-integer;8-empty;0-δ֪)
int CmdSendRecv::checkParam(CmdMsg *pCmdMsg)
{
	variant varRet;
	pCmdMsg->GetMsgContent(varRet);
	int iSize = pCmdMsg->GetVariantSize(varRet);
	long lResult = -1;
	// ��ȡ�������� 		
	if(varRet.get_type() != variant::vt_ptr)
	{
		if(varRet.get_type()==variant::vt_long)
		{
			return 2; //long
		} else {
			if(varRet.get_type() == variant::vt_string) 
				return 3; //string
			else if(varRet.get_type() == variant::vt_boolean) 
				return 4; //bool
			else if(varRet.get_type() == variant::vt_date) 
				return 5; //date
			else if(varRet.get_type() == variant::vt_float) 
				return 6; //float
			else if(varRet.get_type() == variant::vt_integer) 
				return 7; //initeger
			else if(varRet.get_type() == variant::vt_empty) 
				return 8; //empty
			else 
				return 0; //δ֪ 
				
		}
	} else {
		if(iSize%sizeof(SysParamHis)!=0)
		{ 
			return 0;
		} else {
			return 1; //ptr
		}
	}
}

/* �����ú��� */
void CmdSendRecv::show()
{
	 for(int i=0;i<iParamNum;i++)
	 {
		cout<<"����: "<<this->pRecord[i].sSectName<<endl;
	 	cout<<"������: "<<this->pRecord[i].sParamName<<endl;
	 	cout<<"����ԭֵ: "<<this->pRecord[i].sParamOrValue<<endl;
	 	cout<<"������ֵ: "<<this->pRecord[i].sParamValue<<endl;
	 	cout<<"������Ч����: "<<this->pRecord[i].sDate<<endl;
	 }
	 //pProID->reSetProcessID(m_iProcID);
}

