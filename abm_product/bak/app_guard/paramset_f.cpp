/*VER: 2*/ 
/* Copyright (c) 2001-<2003> Wholewise, All rights Reserved */
/* #ident "@(#)process_f.c	1.0	2008/04/15	<AutoCreate>" */
#include "../app_init/HssParamMonitor.h"
#include "../app_init/SysParamInfo.h"
#include "CommandCom.h"
#include "paramset.h"
#include "mainmenu.h"
#include "MsgDefineClient.h"
#include "MsgDefineServer.h"
#include "string.h"
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include "../app_init/BaseRuleMgr.h"
#include "../app_init/BaseFunc.h"
#include "Log.h"
extern char g_sUsrName[256];
/*
#ifdef erase
#undef erase
#endif

#ifdef move
#undef move
#endif
*/

#define EFFDATE		"effdate"

extern int iUsrDroit;
void AllTrims(char sBuf[])
{
	int iFirstChar=0;
	int iEndPos=0;
	int i;
	int iFirstPos=0;
	for(i=0;sBuf[i]!='\0';i++){
		if(sBuf[i]==' '){
			if(iFirstChar==0) iFirstPos++;
		}
		else{
			iEndPos=i;
			iFirstChar=1;
		}
	}
	for(i=iFirstPos;i<=iEndPos;i++)  sBuf[i-iFirstPos]=sBuf[i];
	sBuf[i-iFirstPos]='\0';
	return ;
}

Control *param_handle;

char *param_list_index[MAX_ITEMS];
char param_list_data[MAX_ITEMS][80];
char sWholeVlaue[2048];
char sSectionInfo[1024];
char sErrInfo[80];
int g_param_monitor_flag;

void ParamMonitorEnd ();
int getLineSeq();
int dealParam(Control *pCtl,int ch,long lCMD,bool needFlag);
long getInfo(char *sSectEditData,char *sParamEditData,char *sEditValue,char *sEffDateEditData,long lCMD);
void flashParam(char *sSect,char *sFlashSectParam);

//�����������Ƿ���ϲ�������[�޸Ĳ���ʱʹ��]
int ParamRuleCheck(char *sSect,char *sParam,char *sNewVal,char *sEffDate,char *sOldVal);
//����������
bool CheckAll(char *sSect,char *sParam,char *sNewVal,char *sEffDate,long lCMDID,char *sAllInfo);
//�����кż��
bool CheckAll(int iLine,char *sNewVal,char *sEffDate,long lCMDID,char *sAllInfo);
//��ȡ���������Ϣ
void setErrInfo(int iErrID,char *sErrInfo);

ParamHisRecord* getRecordByLine(int iLine);

HBParamMonitor *pHBParamMonitor = 0;
CommandCom *pCommandCom = 0;
BaseRuleMgr *pBaseRuleMgr = 0;
bool bRegister = false;

int l_lparamLinenum;

int paramset_SysHotKeyPress(Control *pCtl, int ch, long lParam)
{   
    if (ch == 27) { /* ESC */
    	return FORM_KILL;
    }
	
    return ch;
}

void paramset_entry(Control *pHandle) /* process ����ʱ�������� */
{   	
    	param_handle = pHandle;
		Control *pParamList;
		if(!pHBParamMonitor)
		{
			pHBParamMonitor = new HBParamMonitor();
		}
			pCommandCom = new CommandCom();
			if(pCommandCom->InitClient())
			{
				bRegister = true;
		} else {
				THROW(1);
		}
		pHBParamMonitor->init(60,pCommandCom);	
		
		if(bRegister)
		{
			int rows = 0;
		
			pParamList = GetCtlByID (param_handle, 6);	

			rows = pHBParamMonitor->dealParamOpera("ROOT",NULL,MSG_PARAM_S_ALL);
			rows = rows>0?rows:0;
			pParamList->iAdditional = rows;
			l_lparamLinenum = rows;
		
			pParamList->pData = pHBParamMonitor->param_list_index;	
			
			CtlShow (pParamList);
		}
}

int paramsetCtl1Press(Control *pCtl,int ch,long lParam)
{   /* ˢ��button�İ�����Ϣ���� */
    /* Add your self code here: */
		
		if (ch == '\r') {
			dealParam(pCtl,ch,MSG_PARAM_S_N,false);
				//char sData[1024] = {0};
				//sprintf(sData,"%s%s%s\0","[ADMIN]�û�[",g_sUsrName,"]ִ��[����] ˢ�� ����");
				//LOGG(SYSTEMLOG,sData);
		}
		
		return ch;

}

int paramsetCtl2Press(Control *pCtl,int ch,long lParam)
{   	/* ����button�İ�����Ϣ���� */
		int iFlag = 1;
		int result = -1;
		if (ch == '\r') {
			/*if(iUsrDroit == 1)
			dealParam(pCtl,ch,MSG_PARAM_S_C,false);
			else
				prtmsg (pCtl, 21, 12, "��BillingOnline�����û��޴˲���Ȩ��");
			//flashParam("ROOT");*/
		}
		return ch;
}

int paramsetCtl3Press(Control *pCtl,int ch,long lParam)
{   /* ɾ��button�İ�����Ϣ���� *///
    /* Add your self code here: */
	//prtmsg (pCtl, 20, 41, "ȷ��ɾ��Y/N");
	
	Control *pList;
	Control *pList2;
	int i, iFlag = 1;
	int rows;
	if (ch == '\r') {
		/*if(iUsrDroit == 1)
		dealParam(pCtl,ch,MSG_PARAM_S_D,true);
		else
			prtmsg (pCtl, 21, 12, "��BillingOnline�����û��޴˲���Ȩ��");*/
	}
	return ch;
}

int paramsetCtl4Press(Control *pCtl,int ch,long lParam)
{   /*�ύ.button�İ�����Ϣ���� */
    /* Add your self code here: */
	Control *pList;
	int result = -1;
	int i, iFlag = 1;
	if (ch == '\r') {
		dealParam(pCtl,ch,MSG_PARAM_CHANGE,true);						
		//char sData[1024] = {0};
		//sprintf(sData,"%s%s%s\0","[ADMIN]�û�[",g_sUsrName,"]ִ��[����] �޸� ����");
		//LOGG(SYSTEMLOG,sData);
	}	

	return ch;
}

int paramsetCtl5Press(Control *pCtl,int ch,long lParam)
{   /* �˳��İ�����Ϣ���� */
    /* Add your self code here: */
	if (ch == '\r') {
		mainmenu mm;
		mm.run();
		if(pHBParamMonitor){
			//pCommandCom->Logout();
			delete pHBParamMonitor;
			pHBParamMonitor = 0;
			if(pCommandCom)
			{
				delete pCommandCom; pCommandCom=0;
			}
		}
		return FORM_KILL;
	}
	
    return ch;
}

int paramsetCtl6Press(Control *pCtl,int ch,long lParam)
{   /* ����ܵİ�����Ϣ���� */
    /* Add your self code here: */
    if (ch == '\r') {
    if (ListSelNum (pCtl) == 0) {
                ListSetItem (pCtl, ALL_ITEMS,SELECTED);
                CtlShow (pCtl);
        }
    }

    return ch;
}

int paramsetCtl13Press(Control *pCtl,int ch,long lParam)
{   /* �κŵİ�����Ϣ���� */
    /* Add your self code here: */
	char* sFidtmp = 0;
	if (ch == '\r') {
		paramsetCtl1Press(pCtl,ch,lParam);
	}
    return ch;
}

int paramsetCtl15Press(Control *pCtl,int ch,long lParam)
{   /* PARAM_ID�İ�����Ϣ���� */
    /* Add your self code here: */
		if (ch == '\r') {
			paramsetCtl1Press(pCtl,ch,lParam);
		}

    return ch;
}

int paramsetCtl17Press(Control *pCtl,int ch,long lParam)
{   /* NEW_PARAM_ID�İ�����Ϣ���� */
    /* Add your self code here: */
	if (ch == '\r') {
			paramsetCtl4Press(pCtl,ch,lParam);
	}
	
    return ch;
}

int paramsetCtl19Press(Control *pCtl,int ch,long lParam)
{   /* NEW_eff_data�İ�����Ϣ���� */
    /* Add your self code here: */
	if (ch == '\r') {
			paramsetCtl4Press(pCtl,ch,lParam);
	}

    return ch;
}



void ParamMonitorEnd ()
{
    g_param_monitor_flag = 0;
	if(pHBParamMonitor){
			delete pHBParamMonitor;
			pHBParamMonitor = 0;
		}
}

int getLineSeq()
{
	//ʹ���в����Ѿ���+����
	Control *pList = GetCtlByID (param_handle, 6);
	for (int i=0; i<l_lparamLinenum; i++) 
	{
		if (ListItemState (pList, i) != SELECTED)
		{
			continue;
		} else {//�����ж��Ȳ���
			return i;
		}	    
	}
	return -1;
}


int dealParam(Control *pCtl,int ch,long lCMD,bool needFlag)
{
		if(!bRegister)
		{
			prtmsg (pCtl, 21, 12, "ע��ʧ��,���������Լ�������˵�����");
			return 0;
		}
		Control *pList = GetCtlByID (param_handle, 6);
		Control *pSectEdit = GetCtlByID(param_handle, 13);//��
		Control *pParamEdit = GetCtlByID (param_handle, 15);//����
		Control *pValEdit = GetCtlByID(param_handle, 17);//��ֵ
		Control *pEffDateEdit = GetCtlByID (param_handle, 19);//��Ч����
		AllTrims(pSectEdit->sData);
		AllTrims(pParamEdit->sData);
		AllTrims(pValEdit->sData);
		AllTrims(pEffDateEdit->sData);
		if(lCMD == MSG_PARAM_S_N)
		{
			Chang2Upper(pSectEdit->sData);
			Chang2Lower(pParamEdit->sData);
		}
		if(!pHBParamMonitor){
			pHBParamMonitor = new HBParamMonitor();
			pCommandCom = new CommandCom();
			pCommandCom->InitClient();
			pHBParamMonitor->init(15,pCommandCom);	
		}
		long result;
		bool bRes = true;
		int iLine = getLineSeq();
		char sErrInfo[80] = {0};
		char sFlashSectParam[MAX_SECTION_LEN+MAX_KEY_NAME_LEN+1] = {0};
		
		//�����Ƿ���ϻ������
		char sSectInfo[MAX_SECTION_LEN+1] = {0};
		char sParamInfo[MAX_KEY_NAME_LEN+1] = {0};
		char sAllInfo[MAX_SECTION_LEN+MAX_KEY_NAME_LEN+MAX_KEY_VALUE_LEN*2+10] = {0};//����.������=ֵ,effdate=ʱ��
		
		if(needFlag)
		{
			if(pSectEdit->sData[0] == '\0' || pParamEdit->sData[0] == '\0')
			{
				ParamHisRecord* pRecord = getRecordByLine(iLine);
				if(pRecord)
				{
					strncpy(sSectInfo,pRecord->sSectName,sizeof(sSectionInfo));
					strncpy(sParamInfo,pRecord->sParamName,sizeof(sParamInfo));
					bRes =  CheckAll(iLine,pValEdit->sData,pEffDateEdit->sData,lCMD,sAllInfo);
				} else {
					bRes = false;
					SETLOGPRINT;
					ALARMLOG28(0,MBC_COMMAND_hbmonitor,618,"%s",sAllInfo);
				}
			}
		} else {
			strncpy(sSectInfo,pSectEdit->sData,sizeof(sSectionInfo));
			strncpy(sParamInfo,pParamEdit->sData,sizeof(sParamInfo));
			if(pSectEdit->sData[0] || pParamEdit->sData[0])
			{
			  bRes = CheckAll(sSectInfo,sParamInfo,pValEdit->sData,pEffDateEdit->sData,lCMD,sAllInfo);
			}
			else
			{
				bRes = 1;
			}
		}
			
		if(!bRes)
		{
			prtmsg (pCtl, 21, 12, "������ʽ����ȷ,����ʧ��");
			SETLOGPRINT;
			ALARMLOG28(0,MBC_COMMAND_hbmonitor,618,"%s",sAllInfo);
			return -1;
		}
		int iErrID;
		//���������Ч
		if(lCMD == MSG_PARAM_CHANGE)
		{
			iErrID = ParamRuleCheck(sSectInfo,sParamInfo,pValEdit->sData,pEffDateEdit->sData,NULL);
			if(iErrID != 0){
				SETLOGPRINT;
				ALARMLOG28(0,MBC_COMMAND_hbmonitor,618,"%s",sAllInfo);
				bRes = false;
			}
		}
		
		if(!bRes)
		{			
			setErrInfo(iErrID,sErrInfo);			
			prtmsg (pCtl, 21, 12, sErrInfo);//"����ֵ�����Ϲ����趨,����ʧ��");
			return -1;
		}
		
		result = pHBParamMonitor->dealParamOpera(sSectInfo,sAllInfo,lCMD);
		pSectEdit->sData[0] = '\0';
		pParamEdit->sData[0] = '\0';
		pValEdit->sData[0] = '\0';
		pEffDateEdit->sData[0] = '\0';
		switch(lCMD)
		{
			case MSG_PARAM_CHANGE:
			case MSG_PARAM_S_C:
			case MSG_PARAM_S_D:
			{
						if(result == 1)
						{
							prtmsg (pCtl, 21, 12, "�����ɹ�");
						} else
						{ 							
							/*if(result == INIT_ERROR_NO_PARAM)
							{
								prtmsg (pCtl, 21, 12, "�޶�Ӧ����");
							} else if(result == INIT_ERROR_UNRECOGNIZED){
								prtmsg (pCtl, 21, 12, "�����޷�ʶ��"); 
							} else if(result == -1){
								prtmsg (pCtl, 21, 12, "������"); 
							} else if(result == -2){
								prtmsg (pCtl, 21, 12, "��ʱ,�޷��ؽ��"); 
							} else if(result == MSG__RETURN_ERROR){
								prtmsg (pCtl, 21, 12, "����ʧ��"); 
							} else {*/
								prtmsg (pCtl, 21, 12, "�������ɹ�");
							//}
							SETLOGPRINT;
							ALARMLOG28(0,MBC_COMMAND_hbmonitor,result,"%s","��������ʧ��");
						}
						//pHBParamMonitor->clear();
						if(lCMD == MSG_PARAM_CHANGE)
						{
							if(sSectInfo[0]!='\0' && sParamInfo[0]!='\0')
						    {
								strcat(sFlashSectParam,sSectInfo);
								strcat(sFlashSectParam,".");
								strcat(sFlashSectParam,sParamInfo);
							}
							if(sFlashSectParam[0] != '\0')
							{
								flashParam(sSectInfo,sFlashSectParam);
							}
							return ch;
						}
						pList->iAdditional = 0;
						pList->pData = 0;
						return ch;
			}	
			case MSG_PARAM_S_N:
			case MSG_PARAM_S_ALL:						
			{						
						if(!pHBParamMonitor->param_list_index)
						{						
							pList->iAdditional = 0;
							pList->pData = 0;
							prtmsg (pCtl, 21, 12, "���β���ʧ��");
							SETLOGPRINT;
							ALARMLOG28(0,MBC_COMMAND_hbmonitor,580,"%s","��������ʧ��");
							return ch;
						}
						pList->pData = pHBParamMonitor->param_list_index;
						pList->iAdditional = pHBParamMonitor->getRows();
						CtlShow (pList);
						CtlAtv (pCtl, pCtl);
			    		return ch;
			}
			default:
			{
						prtmsg (pCtl, 21, 12, "�������δ֪");
						SETLOGPRINT;
						ALARMLOG28(0,MBC_COMMAND_hbmonitor,581,"%s","��������ʧ��");
						pList->iAdditional = 0;
						pList->pData = 0;
						break;
			}
		}
		return ch;
}

//�����������Ƿ���ϲ�������[�޸Ĳ���ʱʹ��]
int ParamRuleCheck(char *sSect,char *sParam,char *sNewVal,char *sEffDate,char *sOldVal)
{
    if(!pBaseRuleMgr)
        pBaseRuleMgr = new BaseRuleMgr();
    int iRes = pBaseRuleMgr->check(sSect,sParam,sNewVal,sEffDate,sOldVal,false);
    if(iRes != 0){
		SETLOGPRINT;
        ALARMLOG28(0,MBC_COMMAND_hbmonitor,619,"%s%s%s",sSect,".",sParam);
	}
    return iRes;
}

//��ȡһ������
ParamHisRecord* getRecordByLine(int iLine)
{
    ParamHisRecord *pRecord = 0;
    if(!pHBParamMonitor)
        pHBParamMonitor = new HBParamMonitor();
    pRecord = pHBParamMonitor->getParamRecordByLine(iLine);
    return pRecord;
}

bool CheckAll(int iLine,char *sNewVal,char *sEffDate,long lCMDID,char *sAllInfo)
{
    ParamHisRecord* pRecord = getRecordByLine(iLine);
	if(pRecord)
	{
		return CheckAll(pRecord->sSectName,pRecord->sParamName, sNewVal, sEffDate,lCMDID,sAllInfo);
	}	
	return 	false;
}

//����������,ͬʱ��װ����INFO��initserver
bool CheckAll(char *sSect,char *sParam,char *sNewVal,char *sEffDate,long lCMDID,char *sAllInfo)
{
	 switch(lCMDID)
	 {
	 		memset(sAllInfo,'\0',sizeof(sAllInfo));
			case MSG_PARAM_CHANGE:
			{
				if(sSect[0] == '\0' || sParam[0] == '\0' || sNewVal[0] == '\0')
				{
					SETLOGPRINT;
					ALARMLOG28(0,MBC_COMMAND_hbmonitor,617,"%s","����ȱ�ٱ�Ҫ����");
					return false;
				} else {
					if(sEffDate[0] == '\0')
					{
					sprintf(sAllInfo,"%s%s%s%s%s\0",sSect,".",sParam,"=",sNewVal);
					} else {
						sprintf(sAllInfo,"%s%s%s%s%s%s%s\0",sSect,".",sParam,".",NEW_VALUE,"=",sNewVal);
					}
					if(sEffDate[0] != '\0')
					{
						char sTmpCH[1024] = {0};
						sprintf(sTmpCH,"%s%s%s%s%s%s%s%s\0",",",sSect,".",sParam,".",EFFDATE,"=",sEffDate);
						strcat(sAllInfo,sTmpCH);
					}
					return true;
				}
				break;
			}
			case MSG_PARAM_S_N:
			{
				if(sSect[0] == '\0')
				{
					SETLOGPRINT;
					ALARMLOG28(0,MBC_COMMAND_hbmonitor,617,"%s","����ȱ�ٱ�Ҫ����");
					return false;
				} else {
					strcat(sAllInfo,sSect);
					if(sParam[0] != '\0')
					{
						strcat(sAllInfo,"."); 
						strcat(sAllInfo,sParam);
					}
					return true;
				}
				break;
			}
			case MSG_PARAM_S_C:
			{
				if(sSect[0] == '\0' || sParam[0] == '\0' || sNewVal[0] == '\0')
				{
					SETLOGPRINT;
					ALARMLOG28(0,MBC_COMMAND_hbmonitor,617,"%s","����ȱ�ٱ�Ҫ����");
					return false;
				} else {
					sprintf(sAllInfo,"%s%s%s%s%s\0",sSect,".",sParam,"=",sNewVal);
					if(sEffDate[0] != '\0')
					{
						char sTmpC[1024] = {0};
						sprintf(sTmpC,"%s%s%s%s%s%s%s%s\0",",",sSect,".",sParam,".",EFFDATE,"=",sEffDate);
						strcat(sAllInfo,sTmpC);
					}
					return true;
				}
				break;
			}
			case MSG_PARAM_S_D:
			{
				if(sSect[0] == '\0' || sParam[0] == '\0')
				{
					SETLOGPRINT;
					ALARMLOG28(0,MBC_COMMAND_hbmonitor,617,"%s","����ȱ�ٱ�Ҫ����");
					return false;
				} else {
					sprintf(sAllInfo,"%s%s%s\0",sSect,".",sParam);					
					return true;
				}
				break;
			}
		}
	 return true;
}

//��ȡ���������Ϣ
void setErrInfo(int iErrID,char *sErrInfo)
{
	if(iErrID!=0)
	{
		memset(sErrInfo,'\0',sizeof(sErrInfo));
		switch(iErrID)
		{
			case MAX_ERR:
				strcpy(sErrInfo,"������ֵ�������������趨���ֵ");//,sizeof(sErrInfo));
				break;
			case MIN_ERR:
				strcpy(sErrInfo,"������ֵС�ڲ��������趨��Сֵ");//,sizeof(sErrInfo));
				break;
			case ENUM_ERR:
				strcpy(sErrInfo,"������ֵ���ڲ��������趨��ö��ֵ��Χ");//,sizeof(sErrInfo));
				break;
			case EFF_ERR_LESS:
				strcpy(sErrInfo,"��Ч����ֵ������,����δ������");//,sizeof(sErrInfo));
			break;
			case EFF_ERR_NONEED:
				strcpy(sErrInfo,"����������Ч����");//,sizeof(sErrInfo));
				break;
			case MAX_MIN_SET_ERR:
				strcpy(sErrInfo,"���������Сֵ���ô���");//,sizeof(sErrInfo));
				break;
			case CHANGE_ERR:
				 strcpy(sErrInfo,"���������޸�");
			default:
				strcpy(sErrInfo,"����ֵ���������ù���");//,sizeof(sErrInfo));
				break;
		}
	}
}

void flashParam(char *sSect,char *sFlashSectParam)
{
	Control *pParamList = 0;
	if(bRegister)
	{
			int rows = 0;		
			pParamList = GetCtlByID (param_handle, 6);	

		rows = pHBParamMonitor->dealParamOpera(sSect,sFlashSectParam,MSG_PARAM_S_N);
			pParamList->iAdditional = rows;
			l_lparamLinenum = rows;
		
			pParamList->pData = pHBParamMonitor->param_list_index;	
			CtlShow (pParamList);
	}
}
