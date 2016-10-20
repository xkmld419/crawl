/*VER: 5*/ 
#include "Process.h"
#include <iostream>
#include "UserInfoReader.h"
#include "Log.h"
#include "TransactionMgr.h"
#include "StdEvent.h"
#ifdef GROUP_CONTROL
#include "FormatMethod.h"
#include "UserInfo.h"
#endif
using namespace std;

class SeekUserInfo 
{
  public:
	int run();

  public:
	SeekUserInfo();
	//add by xn 2009.11.13 for np
	int seekNpInfo(char* sOut, char* accNbr, char* sDate);

};

int main(int argc, char *argv[])
{
	g_argc = argc;
	g_argv = argv;

	SeekUserInfo x;

	return x.run ();
}

SeekUserInfo::SeekUserInfo()
{
//	Environment::useUserInfo(USER_INFO_BASE);
    Environment::useUserInfo(USRE_INFO_NOITEMGROUP);

	if (!G_PTRANSMGR) {
		G_PTRANSMGR = new TransactionMgr ();
	}

	if (!G_PTRANSMGR) {
		THROW (10);
	}
}
int SeekUserInfo::seekNpInfo(char* sOut,char* accNbr, char* sDate)
{
	NpInfo npInfo;
	char sTemp[128];
	if ( G_PUSERINFO->getNpInfo(npInfo,accNbr,sDate) )
	{
		strcat (sOut, "\nЯ��ת����Ϣ:\n");
		sprintf(sTemp,
				"\t\t����:%s\n"
				"\t\tԭʼ����:%d\n"
				"\t\t��������:%d\n"
				"\t\tЯ��״̬:%d\n"
				"\t\t��Чʱ��:%s\n"
				"\t\tʧЧʱ��:%s\n",
				npInfo.m_sAccNbr,
				npInfo.m_iOrgNetwork,
				npInfo.m_iNetwork,
				npInfo.m_iNpState,
				npInfo.m_sEffDate,
				npInfo.m_sExpDate);
			strcat (sOut, sTemp);
		return 1;
	}
	return 0;
}
int SeekUserInfo::run()
{
	if (g_argc < 2) {
		Log::log (0, "[ERROR] seekuser SERV_ID|ACC_NBR [DATE]");
#ifdef GROUP_CONTROL
		Log::log (0, "[ERROR] seekuser -c|-g ACC_NBR [DATE]");
		Log::log (0, "[ERROR] seekuser -e ACC_NBR [DATE]");
#endif
#ifdef	IMSI_MDN_RELATION
		Log::log (0, "[ERROR] seekuser -i IMSI [DATE]");
#endif

		return 0;
	}
	char sDate[16];
	char sOut[10240];
	char sTemp[128];
	memset (sOut, 0, sizeof (sOut));
	memset (sTemp, 0, sizeof (sTemp));

#ifdef GROUP_CONTROL

	if(strcmp(g_argv[1],"-e")==0 ||
		strcmp(g_argv[1],"-E")==0)
	{

		int iRet;
		bool bRet;
		if(g_argc < 3)
		{
			Log::log (0, "[ERROR] seekuser -e ACC_NBR [DATE]");
			return 0;
		}
		if(g_argc > 3)
		{
			Date d(g_argv[3]);
			strcpy (sDate, d.toString ());
		}else{
			Date d;
			strcpy (sDate, d.toString ());
		}
		char * pTemp1 = g_argv[2];
		AccNbrOfferType oAccNbrOfferType;
		if(!G_PUSERINFO->getAccNbrOfferType(pTemp1,sDate,oAccNbrOfferType))
		{
			sprintf (sOut, "\n=========��ѯʧ��,��ʧ��,^@^=========\n");
		}
		else
		{
			sprintf (sOut, "\n=========��ѯ�������Ϣ����=========\n");
			sprintf (sTemp,
				"��    ��:%s\n"
				"��Чʱ��:%s\n"
				"��Чʱ��:%s\n"
				"��Ʒ����:%d\n",
				oAccNbrOfferType.m_sAccNbr,oAccNbrOfferType.m_sEffDate,
				oAccNbrOfferType.m_sExpDate,oAccNbrOfferType.m_iOfferType);
			strcat (sOut, sTemp);
		}
		cout << sOut << endl << endl;
		return 0;
	}
	if(strcmp(g_argv[1],"-c")==0 ||
		strcmp(g_argv[1],"-C")==0 ||
		strcmp(g_argv[1],"-g")==0 ||
		strcmp(g_argv[1],"-G")==0)
	{
		int iRet;
		bool bRet;
		if(g_argc < 3)
		{
			Log::log (0, "[ERROR] seekuser -c|-g ACC_NBR [DATE]");
			return 0;
		}
		if(g_argc > 3)
		{
			Date d(g_argv[3]);
			strcpy (sDate, d.toString ());
		}else{
			Date d;
			strcpy (sDate, d.toString ());
		}
		char * pTemp1 = g_argv[2];
		GroupInfo groupInfo;
		NumberGroup numberInfo[GROUP_NUM];
		GroupMember memberInfo[GROUP_NUM];
		switch (g_argv[1][1]|0x20)
		{
		case 'c':
			iRet = G_PUSERINFO->getNumberGroupByAccNbr(numberInfo,pTemp1,sDate);
			if(iRet>0)
			{
				sprintf (sOut, "\n=========��ѯ�������Ϣ����=========\n");
				for(int i=0;i<iRet ;i++)
				{
					bRet = G_PUSERINFO->getGroupInfoByID(groupInfo,numberInfo[i].m_iGroupID,sDate);
					sprintf (sTemp,
					"��ϵ  ID:%ld\n"
					"��    ��:%s\n"
					"Ⱥ��  ID:%d\n"
					"��Ч����:%s\n"
					"ʧЧ����:%s\n"
					"��Ʒʵ��:%ld\n",
					numberInfo[i].m_lRelationID,numberInfo[i].m_sAccNbr,
					numberInfo[i].m_iGroupID,numberInfo[i].m_sEffDate,
					numberInfo[i].m_sExpDate,numberInfo[i].m_lOfferInstID);
					strcat (sOut, sTemp);
					if(bRet)
					{
						sprintf (sTemp,
						"\tȺ������:%s\n"
						"\tȺ������:%s\n"
						"\t����  ID:%d\n"
						"\t����  ID:%d\n"
						"\t��Ч����:%s\n"
						"\tʧЧ����:%s\n"
						"\t��  �ȼ�:%d\n"
						"\t���� ORG:%d\n",
						groupInfo.m_sGroupName,groupInfo.m_sGroupDesc,
						groupInfo.m_iConditionID,groupInfo.m_iStrategyID,
						groupInfo.m_sEffDate,groupInfo.m_sExpDate,
						groupInfo.m_iPriority,groupInfo.m_iOrgID);
						strcat (sOut, sTemp);
					}
				}
			}else
			{
				Log::log (0, "[SORRY] û���ҵ�����[%s]�Ķ���Ⱥ��Ϣ", pTemp1);
				return 0;
			}
			break;
		case 'g':
			//�Ѻ����ֳ����źͺ���
			char sAccNbr[33];
			char sAreaCode[33];
			char sFullAccNbr[66];
			strncpy(sAccNbr,pTemp1,sizeof(sAccNbr)-1);
			strcpy(sFullAccNbr,sAccNbr);
			int iLongGroup;
			if(FormatMethod::interceptAreaCodeCycle(sAccNbr, sAreaCode,iLongGroup))
			{
				//����ר���ƷѺ������
				PNSegBilling oPNSegBilling;
				if(FormatMethod::getPNSegBilling(
						sAccNbr,
						sAreaCode,
						sDate,
						oPNSegBilling))
				{
					sprintf(sFullAccNbr,"%s%s",sAreaCode,oPNSegBilling.m_sAccNBR);
				}
			}
			iRet = G_PUSERINFO->getGroupMemberByAccNbr(memberInfo,sFullAccNbr,sDate);
			if(iRet>0)
			{
				sprintf (sOut, "\n=========��ѯ�������Ϣ����=========\n");
				for(int i=0;i<iRet ;i++)
				{
					bRet = G_PUSERINFO->getGroupInfoByID(groupInfo,memberInfo[i].m_iGroupID,sDate);
					sprintf (sTemp,
					"��ϵ  ID:%ld\n"
					"��    ��:%s\n"
					"Ⱥ��  ID:%d\n"
					"��Ч����:%s\n"
					"ʧЧ����:%s\n",
					memberInfo[i].m_lMemberID,memberInfo[i].m_sAccNbr,
					memberInfo[i].m_iGroupID,memberInfo[i].m_sEffDate,
					memberInfo[i].m_sExpDate);
					strcat (sOut, sTemp);
					if(bRet)
					{
						sprintf (sTemp,
						"\tȺ������:%s\n"
						"\tȺ������:%s\n"
						"\t����  ID:%d\n"
						"\t����  ID:%d\n"
						"\t��Ч����:%s\n"
						"\tʧЧ����:%s\n"
						"\t��  �ȼ�:%d\n"
						"\t���� ORG:%d\n",
						groupInfo.m_sGroupName,groupInfo.m_sGroupDesc,
						groupInfo.m_iConditionID,groupInfo.m_iStrategyID,
						groupInfo.m_sEffDate,groupInfo.m_sExpDate,
						groupInfo.m_iPriority,groupInfo.m_iOrgID);
						strcat (sOut, sTemp);
					}
				}
			}else
			{
				Log::log (0, "[SORRY] û���ҵ�����[%s]�Ĺ���Ⱥ��Ϣ", pTemp1);
				return 0;
			}
			break;
		default:
			break;
			return 0;
		}
		cout << sOut << endl << endl;
		return 0;
	}
#endif

#ifdef	IMSI_MDN_RELATION
	if ((g_argv[1][1] | 0x20)=='i'){
		if (g_argc<3){
			Log::log (0, "[ERROR] seekuser -i IMSI [DATE]");
			return 0;
		}else if (g_argc==3){	
			G_PUSERINFO->showImsiMdnRelationByImsi(g_argv[2]);
		}else{
			G_PUSERINFO->showImsiMdnRelationByImsi(g_argv[2], g_argv[3]);
		}
#ifdef DEBUG
		G_PUSERINFO->showAllImsiMdnRelation();
#endif
		return 1;
	}
#endif

	char * pTemp1 = g_argv[1];

	if (g_argc > 2) {
		Date d(g_argv[2]);
		strcpy (sDate, d.toString ());
	} else {
		Date d;
		strcpy (sDate, d.toString ());
	}

	if (pTemp1[0] == '0') {
		if (!G_PUSERINFO->getServ (G_SERV, pTemp1, sDate)) {
			Log::log (0, "[SORRY] û���ҵ�����%s��%s��Ӧ���û�����", pTemp1, sDate);
			if(seekNpInfo(sOut,pTemp1,sDate))//add by xn 2009.11.13 for np
				cout << sOut << endl;
			return 0;
		}

	} else {
		if (!G_PUSERINFO->getServ (G_SERV, strtol (pTemp1, 0, 10), sDate)) {
			Log::log (0, "[SORRY] û���ҵ�����Ʒ��ʶ%s���û�����", pTemp1);

			if (!G_PUSERINFO->getServ (G_SERV, pTemp1, sDate)) {
				Log::log (0, "[SORRY] û���ҵ�����%s��%s��Ӧ���û�����", pTemp1, sDate);
				if(seekNpInfo(sOut,pTemp1,sDate))//add by xn 2009.11.13 for np
					cout << sOut << endl;
				return 0;
			}
		} 
	}	

	sprintf (sOut, "\n=========��ѯ�û���������Ϣ����=========\n"
			"����Ʒ��ʶ:%ld\n"
			"��������  :%d\n"
			"��������  :%s\n"
			"��������  :%s\n"
			"��������  :%s\n" 
			"��������  :%d\n"
			"�����ͻ�  :%ld[����:%s]\n"
			"״    ̬  :%s\n"
			"״̬����  :%s\n"
			"��Ʒ��ʶ  :%d\n"
			"�Ʒ�ģʽ  :%d\n"
			"������Ʒ  :\n",
			G_PSERV->getServID (), G_PSERV->getOrgID (), 
            G_PSERV->getCreateDate (), G_PSERV->getRentDate (), 
            G_PSERV->getNscDate (), G_PSERV->getBillingCycleType (),
			G_PSERV->getCustID (), G_PSERV->getCustType (), 
            G_PSERV->getState (), G_PSERV->getStateDate (),
			G_PSERV->getProductID (), G_PSERV->getServInfo ()->m_iBillingMode);

	int iTemp, jTemp;
    OfferInsQO          poOfferInstBuf[MAX_OFFER_INSTANCE_NUMBER];
	ProdOfferInsAttr	ret[MAX_OFFER_ATTRS];
    iTemp = G_PSERV->getOfferIns (poOfferInstBuf);
	
	for (jTemp=0; jTemp<iTemp; jTemp++) {
        //if (jTemp == 0) {
            sprintf (sTemp,
				"\t��ƷID:%d\n"
				"\tʵ��ID:%ld\n"
				"\t��Чʱ��:%s\n"
				"\tʧЧʱ��:%s\n",
            poOfferInstBuf[jTemp].m_poOfferIns->m_iOfferID,
			poOfferInstBuf[jTemp].m_poOfferIns->m_lProdOfferInsID,
			poOfferInstBuf[jTemp].m_poOfferIns->m_sEffDate,
			poOfferInstBuf[jTemp].m_poOfferIns->m_sExpDate);
        /*} else {
            sprintf (sTemp, ",%d[%ld]", 
                        poOfferInstBuf[jTemp].m_poOfferIns->m_iOfferID,
			poOfferInstBuf[jTemp].m_poOfferIns->m_lProdOfferInsID);
        }*/
		strcat (sOut, sTemp);
		int iResult = poOfferInstBuf[jTemp].getOfferInsAttrAndDate(ret,sDate);
		if(iResult){
			strcat(sOut,"\t��Ʒ������:\n");
		}
		for( int m=0;m<iResult;m++)
		{
			sprintf(sTemp,
				"\t\t����ID:%d\n"
				"\t\t����ֵ:%s\n"
				"\t\t��Чʱ��:%s\n"
				"\t\tʧЧʱ��:%s\n",
				ret[m].m_iAttrID,
				ret[m].m_sAttrValue,
				ret[m].m_sEffDate,
				ret[m].m_sExpDate);
			strcat (sOut, sTemp);
		}
	}

	vector<UserInfoAttr> attrs;
	vector<UserInfoAttr>::iterator iter;

	attrs.clear ();
    G_PSERV->getAttrs(attrs);
    if (attrs.size ()) {
        strcat (sOut, "\n��Ʒ������:\n");
        for (iter=attrs.begin (); iter!=attrs.end (); iter++) {
            sprintf (sTemp, "\t���Ա�ʶ:%d\n"
                            "\t����ȡֵ:%s\n"
                            "\t��Чʱ��:%s\n"
                            "\tʧЧʱ��:%s\n",
                     (*iter).getAttrID (), (*iter).getAttrValue (),
                     (*iter).getEffDate (), (*iter).getExpDate ());

            strcat (sOut, sTemp);
        }

        //strcat (sOut, "\n");
    }

	ServProductIteration productIter;
	ServProduct product;

	if (G_PSERV->getServProductIteration (productIter)) {
		strcat (sOut, "\n������Ʒ��Ϣ:\n");

		while (productIter.next(product)) {
			sprintf (sTemp, "\t��Ʒ��ʶ:%d\n"
					"\t��Чʱ��:%s\n"
					"\tʧЧʱ��:%s\n",
					 product.getProductID (), product.getEffDate (),
					 product.getExpDate () );
			strcat (sOut, sTemp);

			attrs.clear ();
			product.getAttrs(attrs);
			if (attrs.size ()) {
				strcat (sOut, "\t������Ʒ������:\n");
				for (iter=attrs.begin (); iter!=attrs.end (); iter++) {
					sprintf (sTemp, "\t\t���Ա�ʶ:%d\n"
							"\t\t����ȡֵ:%s\n"
							"\t\t��Чʱ��:%s\n"
							"\t\tʧЧʱ��:%s\n",
						 (*iter).getAttrID (), (*iter).getAttrValue (), 
						 (*iter).getEffDate (), (*iter).getExpDate ());

					strcat (sOut, sTemp);
				}

				//strcat (sOut, "\n");
			}

		}

	}
	const char *pAccNBR = G_PSERV->getAccNBR(25,sDate);
	char accNbr[33];
	strcpy(accNbr,pAccNBR);//���Ͳ�ͬ��������Ϊ֮
	seekNpInfo(sOut,accNbr,sDate);
	long lCharge;
	
	lCharge = G_PTRANSMGR->selectAcctItemAggr (G_PSERV->getServID (), 
					G_PSERV->getBillingCycle()->getBillingCycleID (), 0);

	sprintf (sTemp, "\n����[%d]�ĵ�ǰ���� %ld ��", 
                G_PSERV->getBillingCycle()->getBillingCycleID (), lCharge);
	strcat (sOut, sTemp);

	sprintf (sTemp, "\n\n���û����ʻ�ID:");

    strcat (sOut, sTemp);

    vector<long> vAcct;
    G_PSERV->getAcctID (vAcct,sDate);

    for (lCharge=0; lCharge < vAcct.size (); lCharge++) {
		if(lCharge>0){
			strcat(sOut,",");
		}
        sprintf (sTemp, "%ld ", vAcct[lCharge]);
        strcat (sOut, sTemp);
    }

	cout << sOut << endl << endl;
	
	return 0;
}
