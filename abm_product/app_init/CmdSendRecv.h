#ifndef CMD_SEND_RECV_H_
#define CMD_SEND_RECV_H_
#include "CommandCom.h"
//#include "ProcessIDMgr.h"
//#include "Variant.h"
#include "CmdMsg.h"
#include "hbcfg.h"
#include <vector>
#include "SysParamInfo.h"
#include "SysParamDefine.h"
#include  <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "MsgDefineClient.h"
#include "MsgDefineServer.h"
//#include "ProcessEx.h"
#define LONG_WAIT_TIME	200
#define WAIT_TIME	100

class CmdSendRecv
{
	public:
			CmdSendRecv();
			~CmdSendRecv();	
			//����˵��: ����ID,����,����ֵ,����ת�����ĵ�ַpParamRecord
			long dealParamValue(long lCMDID,char *sSect,char *sValue,ParamHisRecord *pParamRecord,int &m_iParamNum);			
						
	public:	//�����ú���	
			long sendAndGetDataNew(long lCMDID,char *sSect,char *sValue,ParamHisRecord *pParamRecord,CommandCom *pCmdCom);
			long sendAndGetDataEx(long lCMDID,char *sSect,char *sValue,ParamHisRecord *pParamRecord,CommandCom *pCmdCom,int iMsgID);
			int iParamNum; //��������[��ʱ����]
			void show();
			
			//����˵��: ����ID,����,����ֵ,����ת�����ĵ�ַ
			long dealParamValue(long lCMDID,char *sSect,char *sValue);
			
	private: //����������
			ParamHisRecord pRecord[1024];	//�����õĲ���
	private:
			int m_iProcID;//��ȡ�����úõĽ���ID,��Ҫ�Ƿ��������Ϣ����
			int m_iMsgID;// ��Ϣ����ID
			
			vector<CmdMsg*> vec_CmdMsg;
			vector<CmdMsg>  vec_CmdMsgNew;//����						
			
			static CommandCom *pCmd;
			//static ProcessIDMgr *pProID;
			static CmdMsgFactory *pMsgFactory;
			
			static CmdSender* pSender;
			
			bool m_bRegist;
			// ���õ��Ĳ��������Ƿ���LONG����(ptr - 1;long -2)
			int checkParam(CmdMsg *pCmdMsg);
			//���ͽ�������
			int sendAndGetData(long lCMDID,char *sSect,char *sValue);
			// ת�����ݸ�ʽΪParamHisRecord
			long VecToSysParamInfo(ParamHisRecord *pParamHisRecord,int &m_iParamNum);
			//����ֵ=1����ֵ[�����Ǳ����ֵ];=2[�ɹ�,����������];
			long  VecToSysParamInfo(ParamHisRecord *pParamHisRecord,CmdMsg *pCmdMsg);
			
			//���ͽ�������
			//long sendAndGetData(vector<CmdMsg*> &vec_Msg,long lCMDID,char *sSect,char *sValue);
			// ת�����ݸ�ʽΪParamHisRecord
			long VecToSysParamInfo(ParamHisRecord *pParamHisRecord,vector<CmdMsg*> &vec_Msg);
			// ��������
			void rePerpare();	
	protected:
					
};

#endif
