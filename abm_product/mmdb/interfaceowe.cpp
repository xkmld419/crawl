#include "interfaceowe.h"   //����ͷ�ļ�

#ifdef ABM_BALANCE
/*
*   ABM ͬ��API synExecute�۷�
*/
AbmInterfaceManage::AbmInterfaceManage() 
{
  //ABM�ӿڳ�ʼ��
  pInst = new ABM;
  m_iInitRet = pInst->init();
  if (m_iInitRet != 0) 
  	 {
       m_iInitRet = m_iInitRet>0 ? m_iInitRet*-1:m_iInitRet;
       Log::log(0, "ABM�ӿڳ�ʼ��ʧ��,�������:%d", m_iInitRet);
     }
}

AbmInterfaceManage::~AbmInterfaceManage() 
{
  if (pInst != NULL) 
  	 {
       delete pInst;
       pInst = NULL;
     }
}

/***************************************************************************
*��������int getInitResult()
*���ܣ���ȡ��ʼ������ֵ,�����ֵ������0,��ô��ʼ��ʧ��
***************************************************************************/
int AbmInterfaceManage::getInitResult() 
{
  return m_iInitRet;
}

/***************************************************************************
*��������int resetBatCharge()
*���ܣ���ʼ�������ڷѵ�����
***************************************************************************/
int AbmInterfaceManage::resetBatCharge()
{
  brdCmd.reset();
  return 0;
}

/***************************************************************************
*��������int pachBatCharge()
*���ܣ������۷Ѵ��
***************************************************************************/
int AbmInterfaceManage::pachBatCharge(DeductBalanceR &oData, vector<DeductAcctItem*> &vecBal)
{
  if (brdCmd.add(oData, vecBal) != 0) 
  	 {
       //Log::log(0, "����ʵʱ�۷�������ʧ��!");
       return -1;
     }
  else 
     return 0;
}

/***************************************************************************
*��������bool pachCharge()
*���ܣ����ڿ۷Ѽ�¼���
***************************************************************************/
int AbmInterfaceManage::pachCharge(DeductBalanceR &oData, vector<DeductAcctItem*> &vecBal)
{
  //ʵʱ�۷Ѽ�¼������
  if (qryCmd.set(oData, vecBal) != 0) 
  	 {
       Log::log(0, "ʵʱ�۷�������ʧ��!");
       return -1;
     }
  else 
     return 0;
}

/***************************************************************************
*��������bool transBatCharge()
*���ܣ������ϴ��۷�
***************************************************************************/
int AbmInterfaceManage::transBatCharge()
{
  pInst->beginTrans();
  int ret = pInst->synExecute(&brdCmd, &brdResult);
  if (ret != 0) 
  	 {
       Log::log(0, "ABM�۷��쳣,�������:%d ", ret);
       ret = ret>0 ? ret*-1:ret;
       return ret;
     }

  ret = brdResult.getResultCode();
  if (ret != 0) 
  	 {
       Log::log(0, "ABM�۷��쳣,�������:%d ", ret);
       ret = ret>0 ? ret*-1:ret;
       return ret;
     }
  return 0;
}

/***************************************************************************
*��������bool transCharge()
*���ܣ��ϴ��۷�
***************************************************************************/
int AbmInterfaceManage::transCharge()
{
  /* SYS PID
  int m_iSysPID;
  m_iSysPID = getpid();
  */
  pInst->beginTrans();
  int ret = pInst->synExecute(&qryCmd, &qryRlt);
  if (ret != 0) 
  	 {
        Log::log(0, "ABM�۷��쳣,�������:%d ", ret);
        ret = ret>0 ? ret*-1:ret;
        return ret;
     }

  ret = qryRlt.getResultCode();
  if (ret != 0) 
  	 {
        Log::log(0, "ABM�۷��쳣,�������:%d ", ret);
        ret = ret>0 ? ret*-1:ret;
        return ret;
     }
  return 0;
}

/***************************************************************************
*��������bool AbmInterfaceManage::queryBatBalance(RealDeductA &queryreal, int ioffset)
*���ܣ���ѯ�����Ϣ
***************************************************************************/
int AbmInterfaceManage::queryBatBalance(RealDeductA &queryreal, int ioffset)
{
  //��ȡ�˻����û������
  queryreal.m_lUniversalBalance = brdResult.getUniversalBalance(ioffset); 
  //��ȡ�˻�ר��ר�����
  queryreal.m_lAcctSpecialBalance  = brdResult.getAcctSpecialBalance(ioffset);
  //��ȡ�û�ר��ר�����
  queryreal.m_lServSpecialBalance = brdResult.getServSpecialBalance(ioffset);
  //��ȡǷ��
  queryreal.m_lServOweCharge = brdResult.getServOweCharge(ioffset);
  return 0;
}

/***************************************************************************
*��������bool AbmInterfaceManage::queryBalance(RealDeductA & queryreal)
*���ܣ���ѯ�����Ϣ
***************************************************************************/
int AbmInterfaceManage::queryBalance(RealDeductA &queryreal)
{
  //���ABM���صĲ�ѯ��Ϣ
  prlt = (RealDeductResult *)&qryRlt;
  //��ȡ�˻����û������
  queryreal.m_lUniversalBalance = prlt->getUniversalBalance(); 
  //��ȡ�˻�ר��ר�����
  queryreal.m_lAcctSpecialBalance  = prlt->getAcctSpecialBalance();
  //��ȡ�û�ר��ר�����
  queryreal.m_lServSpecialBalance = prlt->getServSpecialBalance();
  //��ȡǷ��
  queryreal.m_lServOweCharge = prlt->getServOweCharge();
  return 0;
}

/***************************************************************************
*��������bool AbmInterfaceManage::getBatCmdNum()
*���ܣ���ѯ�������������
***************************************************************************/
long AbmInterfaceManage::getBatCmdNum()
{
  return brdCmd.getNum();
}

/***************************************************************************
*��������bool AbmInterfaceManage::getBatResultNum()
*���ܣ���ѯ�����۷ѽ��������
***************************************************************************/
long AbmInterfaceManage::getBatResultNum()
{
  return brdResult.getNum();
}

/***************************************************************************
*��������int AbmInterfaceManage::getBreakPoint(long lFileID,long acct_id)
*���ܣ���ABM����ȡ�ļ��Ĵ���ϵ�
***************************************************************************/
int AbmInterfaceManage::getBreakPoint(long lFileID, long &loffset)
{
  if (qryCmd1.set(lFileID) != 0) 
  	 {
       Log::log(0, "��ȡ�ļ��ϵ�������ʧ��!");
       return -1;
     }

  pInst->beginTrans();
  int ret = pInst->synExecute(&qryCmd1, &qryRlt1);
  if (ret != 0) 
  	 {
       Log::log(0, "ABM��ȡ�ļ��ϵ��쳣,�������:%d", ret);
       ret = ret>0 ? ret*-1:ret;
       return ret;
     }
  
  ret = qryRlt1.getResultCode(); 
  if (ret != 0) 
  	 {
       Log::log(0, "ABM��ȡ�ļ��ϵ��쳣,�������:%d", ret);
       ret = ret>0 ? ret*-1:ret;
       return ret;
     }
  else 
  	 {
       prlt1 = (RealQueryResult *)&qryRlt1;
       loffset = prlt1->getOffset();
       return 0;
     }
}

/***************************************************************************
*������:bool querypayresult();
*���ܣ���ABM����ȡ�۷�ʵʱ��ѯ���
***************************************************************************/
int AbmInterfaceManage::querypayresult(long lAcctID, long lServID, PayResultQryA &payresult)
{
  if (qryCmd2.set(lAcctID, lServID) != 0) 
  	 {
       Log::log(0,"ABM��ȡʵʱ��ѯ���ʧ��!");
       return -1;
     }
    
  pInst->beginTrans();
  int ret = pInst->synExecute(&qryCmd2, &qryRlt2); 
  if (ret != 0) 
  	 {
       Log::log(0,"ABM��ȡʵʱ��ѯ����쳣,�������:%d", ret);
       ret = ret>0 ? ret*-1:ret;
       return ret;
     }
  
  ret = qryRlt2.getResultCode();    
  if (ret != 0) 
  	 {
       Log::log(0,"ABM��ȡʵʱ��ѯ����쳣,�������:%d", ret);
       ret = ret>0 ? ret*-1:ret;
       return ret;
     }
  else 
  	 {
       //���ABM���صĿ۷�ʵʱ��Ϣ
       prlt2 = (PayResultQryResult *)&qryRlt2;
       //��ȡ�˻����û������
       payresult.m_lUniversalBalance = prlt2->getUniversalBalance(); 
       //��ȡ�˻�ר��ר�����
       payresult.m_lAcctSpecialBalance  = prlt2->getAcctSpecialBalance();
       //��ȡ�û�ר��ר�����
       payresult.m_lServSpecialBalance = prlt2->getServSpecialBalance();
       //��ȡǷ��
       payresult.m_lServOweCharge = prlt2->getServOweCharge();
       return 0;
     }
} 

/***************************************************************************
*������:bool queryBalanceDetail();
*���ܣ���ѯabm�ʻ������ϸ
***************************************************************************/
int AbmInterfaceManage::queryBalanceDetail(long lAcctID, vector<BalanceInfoS *> &vecBalanceInfoS)
{
	qb.m_lAcctID = lAcctID;
	qb.m_iQueryFlag = 1;
	qb.m_lQueryID = lAcctID;
	qb.m_iQueryState = 1;
	qbSCmd.set(qb);
	
	pInst->beginTrans();
	int iRet = pInst->synExecute(&qbSCmd, &qbSResult);
	if (iRet != 0)
		 {
       Log::log(0, "ABM��ѯ�ʻ�%ld�����ϸ�쳣,�������:%d", lAcctID, iRet);
       iRet = iRet>0 ? iRet*-1:iRet;
		   return iRet;
		 }

	iRet = qbSResult.getResultCode();
	if (iRet != 0)
		 {
       Log::log(0, "ABM��ѯ�ʻ�%ld�����ϸ�쳣,�������:%d", lAcctID, iRet);
       iRet = iRet>0 ? iRet*-1:iRet;
		   return iRet;
		 }
	else
		 {
		   qbSResult.getRecord(vecBalanceInfoS);
		   return 0;
		 }	   
}

/***************************************************************************
*������:bool queryCurAggrDetail();
*���ܣ���ѯabm�ʻ�ʵʱ������ϸ
***************************************************************************/
int AbmInterfaceManage::queryCurAggrDetail(long lAcctID, vector<AcctItemInfoHB *> &vecAcctItemInfoHB)
{
  qaiCmd.setAcctQry(lAcctID, 1);  	
  
	pInst->beginTrans();
	int iRet = pInst->synExecute(&qaiCmd, &qaiResult);
	if (iRet != 0)
		 {
       Log::log(0, "ABM��ѯ�ʻ�%ldʵʱ������ϸ�쳣,�������:%d", lAcctID, iRet);
       iRet = iRet>0 ? iRet*-1:iRet;
		   return iRet;
		 }
	
	iRet = qaiResult.getResultCode();
	if (iRet != 0)
		 {
       Log::log(0, "ABM��ѯ�ʻ�%ldʵʱ������ϸ�쳣,�������:%d", lAcctID, iRet);
       iRet = iRet>0 ? iRet*-1:iRet;
		   return iRet;
		 }
	else
		 {
		   qaiResult.getRecord(vecAcctItemInfoHB);
		   return 0;
		 }	   
}

/***************************************************************************
*������:bool transBillingMode();
*���ܣ��û�Ԥ�󸶷���ת��֪ͨabm
***************************************************************************/
int AbmInterfaceManage::transBillingMode(long lAcctID, long lServID, long lTypeID)
{
  stCmd.set(lAcctID, lServID, lTypeID);
  
  pInst->beginTrans();
  int iRet = pInst->synExecute(&stCmd, &stResult);
  if (iRet != 0)
  	 {
       Log::log(0, "�ʻ�%ld,�û�%ld,���ѷ�ʽ%ldת��֪ͨABM�����쳣,�������:%d", lAcctID, lServID, lTypeID, iRet);
		   iRet = iRet>0 ? iRet*-1:iRet;
		   return iRet;
  	 }
  
  iRet = stResult.getResultCode();
  if (iRet != 0)
  	 {
       Log::log(0, "�ʻ�%ld,�û�%ld,���ѷ�ʽ%ldת��֪ͨABM�����쳣,�������:%d", lAcctID, lServID, lTypeID, iRet);
		   iRet = iRet>0 ? iRet*-1:iRet;
		   return iRet;
  	 }
  else
  	 {
  	   return 0;
  	 }	 
}

    //����֪ͨabm,0��ʾ�ɹ�
int AbmInterfaceManage::modifyAcctOld(long lAcctID, long lServID, long lTransID, int iBillingCycleID, vector<AcctAggrHB01 *> &vecData)
{
  maoCmd.set(lAcctID, lServID, lTransID, iBillingCycleID);
  
  pInst->beginTrans();
  int iRet = pInst->synExecute(&maoCmd, &maoResult);
  if (iRet != 0)
  	 {
       Log::log(0, "�ʻ�%ld,�û�%ld,TransID%ld����֪ͨABM�����쳣,�������:%d", lAcctID, lServID, lTransID, iRet);
		   iRet = iRet>0 ? iRet*-1:iRet;
		   return iRet;
  	 }
  
  iRet = maoResult.getResultCode();
  if (iRet != 0)
  	 {
       Log::log(0, "�ʻ�%ld,�û�%ld,TransID%ld����֪ͨABM�����쳣,�������:%d", lAcctID, lServID, lTransID, iRet);
		   iRet = iRet>0 ? iRet*-1:iRet;
		   return iRet;
  	 }
  else
  	 {
  	   maoResult.getRecord(vecData);
  	   return 0;
  	 }	 
}

int AbmInterfaceManage::modifyAcctNew(long lAcctID, long lServID, long lTransID, int iBillingCycleID, int iBillingMode, vector<AcctAggrHB01 *> &vecData)
{
  manCmd.set(lAcctID, lServID, lTransID, iBillingCycleID, iBillingMode, vecData);
  
  pInst->beginTrans();
  int iRet = pInst->synExecute(&manCmd, &manResult);
  if (iRet != 0)
  	 {
       Log::log(0, "�ʻ�%ld,�û�%ld,TransID%ld����֪ͨABM�����쳣,�������:%d", lAcctID, lServID, lTransID, iRet);
		   iRet = iRet>0 ? iRet*-1:iRet;
		   return iRet;
  	 }
  	 
  iRet = manResult.getResultCode();
  if (iRet != 0)
  	 {
       Log::log(0, "�ʻ�%ld,�û�%ld,TransID%ld����֪ͨABM�����쳣,�������:%d", lAcctID, lServID, lTransID, iRet);
		   iRet = iRet>0 ? iRet*-1:iRet;
		   return iRet;
  	 }
  else
  	 {
  	   return 0;
  	 }	 
}

/***************************************************************************
*������:bool dealacct_item_withoutabm();
*���ܣ������˵�
***************************************************************************/
bool AbmInterfaceManage::dealacct_item_withoutabm(char const *sTable)
{
  char sSQL[4096];
  char sDate[18]={0};
  Date d;
  strcpy(sDate, d.toString());

  sprintf(sSQL, "select acct_item_id, serv_id, acct_id, offer_inst_id,"
                "billing_cycle_id, charge, measure_type, acct_item_type_id,"
                "item_source_id, state_date, nvl(item_class_id, 0) "
                "from %s", sTable);
  DEFINE_QUERY(qry);

  unsigned int i;
  AcctItemData  p;
  int j(0);

  Log::log(0, "��ʼ����%s,��ǰʱ�� %s", sTable, sDate);

  qry.setSQL(sSQL); 
  qry.open();
  while(qry.next()) 
       {        
         p.m_lAcctItemID     = qry.field(0).asLong();
         p.m_lServID         = qry.field(1).asLong();
         p.m_lAcctID         = qry.field(2).asLong();
         p.m_lOfferInstID    = qry.field(3).asLong();
         p.m_iBillingCycleID = qry.field(4).asInteger();
         p.m_lValue          = qry.field(5).asLong();
         p.m_iPayMeasure     = qry.field(6).asInteger();
         p.m_iPayItemTypeID  = qry.field(7).asInteger();
         p.m_iItemSourceID   = qry.field(8).asInteger();
         strcpy (p.m_sStateDate, qry.field(9).asString());
         p.m_iItemClassID = qry.field(10).asInteger();
         j++;
         if (!(j%300000)) {
            Log::log(0, "\t�Ѵ�����%d\t��", j);
         }
       }
  qry.close();

  char sDate1[18]={0};
  Date d1;
  strcpy(sDate1, d1.toString());
  Log::log(0, "\t�Ѵ�����%d\t��,��ǰʱ����%s", j, sDate1);

  return true;
}

/***************************************************************************
*������:bool dealacct_item_withabm();
*���ܣ������˵�����abm����
***************************************************************************/
bool AbmInterfaceManage::dealacct_item_withabm(char const *sTable)
{
  char sSQL[4096];
  char sDate[18]={0};
  Date d;
  int m_iSysPID;
  m_iSysPID = getpid();
  strcpy(sDate,d.toString());
  sprintf(sSQL, "select acct_item_id, serv_id, acct_id, offer_inst_id,"
                "billing_cycle_id, charge, measure_type, acct_item_type_id,"
                "item_source_id, state_date, nvl(item_class_id, 0) "
                "from %s", sTable);
  DEFINE_QUERY(qry);
  unsigned int i;
  AcctItemData  p;
  int j (0);
    
  Log::log(0, "deal with abm ��ʼ%s,��ǰʱ�� %s pid %d", sTable, sDate, m_iSysPID);

  vector<AcctItemData> vBuf;
  vBuf.resize (20001);
    
  DeductBalanceR oData;
  vector<DeductAcctItem*> vecBal;

  DeductAcctItem ss ;
  vecBal.push_back(&ss);

  qry.setSQL(sSQL); 
  qry.open();
  while(qry.next()) 
       {
         p.m_lAcctItemID = qry.field (0).asLong ();
         p.m_lServID     = qry.field (1).asLong ();
         p.m_lAcctID     = qry.field (2).asLong ();
         p.m_lOfferInstID   = qry.field (3).asLong ();
         p.m_iBillingCycleID = qry.field (4).asInteger ();
         p.m_lValue      = qry.field (5).asLong ();
         p.m_iPayMeasure = qry.field (6).asInteger ();
         p.m_iPayItemTypeID  = qry.field (7).asInteger ();
         p.m_iItemSourceID = qry.field (8).asInteger ();
         strcpy (p.m_sStateDate, qry.field (9).asString ());
         p.m_iItemClassID = qry.field (10).asInteger ();
         j++;
         vBuf.push_back(p);
         if (j%20000 == 0) 
         	  {
              Log::log(0, "\t �Ѿ����� %d\t�� pid %d", j, m_iSysPID);
              for(int i=0; i<vBuf.size(); i++) 
                 {
                   oData.m_iBillingCycleID =p.m_iBillingCycleID;
                   oData.m_lAcctID= p.m_lAcctID;
                   oData.m_lServID = p.m_lServID;
                   oData.m_iFileID = 1000 +j;
                   oData.m_iBillingMode = 2;
                   oData.m_iOffset = i;

                   ss.m_iItemSourceID=p.m_iItemSourceID;
                   ss.m_iOfferID=8888;
                   ss.m_lCharge =p.m_lValue;
                   ss.m_lOfferInstID = p.m_lOfferInstID;
                   ss.m_uiAcctItemTypeID = p.m_iPayItemTypeID;

                   pachCharge(oData, vecBal);
                   transCharge();
                 }
              vBuf.clear();
              Log::log(0, "\t �Ѿ����� %d\t�� pid %d", j, m_iSysPID);
            }
       }
  qry.close ();

  Log::log(0, "\t �Ѿ����� %d\t�� pid %d", j, m_iSysPID);
  for(int i=0; i<vBuf.size(); i++) 
     {
       oData.m_iBillingCycleID =p.m_iBillingCycleID;
       oData.m_lAcctID= p.m_lAcctID;
       oData.m_lServID = p.m_lServID;
       oData.m_iFileID = 1000 +j;
       oData.m_iBillingMode = 2;
       oData.m_iOffset = i;

       ss.m_iItemSourceID=p.m_iItemSourceID;
       ss.m_iOfferID=8888;
       ss.m_lCharge =p.m_lValue;
       ss.m_lOfferInstID = p.m_lOfferInstID;
       ss.m_uiAcctItemTypeID = p.m_iPayItemTypeID;
 
       pachCharge(oData, vecBal);
       transCharge();
     }
  vBuf.clear();
  Log::log(0, "\t �Ѿ����� %d\t�� pid %d", j, m_iSysPID);
  char sDate1[18]={0};
  Date d1;
  strcpy(sDate1, d1.toString());
  Log::log(0, "\t�Ѿ�ͨ��abm������%d\t��,��ǰʱ����%s pid %d", j,sDate1, m_iSysPID);
  return true;
}
#endif

//������
/*
int main(int argc, char **argv)
{
  AbmInterfaceManage tt;
  switch(argv[1][0] | 0x20) 
        {
          case 'a':
            tt.dealacct_item_withabm("acct_item_aggr_201009");
            break;
          case 'n':
            tt.dealacct_item_withoutabm("acct_item_aggr_201009");
            break;
          default:
            Log::log(0,    "\n********************************************************************\n"
                            " ���÷�������\n"
                            "********************************************************************\n"
                     );
        }
}
*/





