/*VER: 1*/ 
// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#ifndef TRANSACTIONMGR_H
#define TRANSACTIONMGR_H

#include "CommonMacro.h"
#include <vector>

#include "OfferAccuMgr.h"
#include "AcctItemAccu.h"
#include "StdEvent.h"

using namespace std;

/*
����Ǽ����Կ�����
*/

class AggrMgr : public AcctItemAccuMgr, public OfferAccuMgr
{
public:

	//�¼��ۻ�����ѯ
		//��ѯ�û�ָ������ָ���¼����͵Ľ��
		long getEventAggr(long lServID,int iBillingCycleID,
	                                        int iEventAggrTypeID);	                                        
	    //��ѯ�û�ָ�������¼���¼������¼����
		int getAllEventAggr(long lServID, int iBillingCycleID, 
	                                        vector<AccuData *> & v);
	                                        
	    //��Ŀ�ۻ�����ѯ
		//��ѯ�û�ָ������ָ����Ŀ���͵ķ���
		long getAcctItemAggr(long lServID,int iBillingCycleID,
	                                            int iAcctItemTypeID);	                                            
		long getPartyRoleAggr(long lServID, int iBillingCycleID,int iPartyRoleID);

		//��ѯ�û�ָ������ָ����Ŀ���͵ļ�¼������¼����
		int getDisctItemAggr(long lServID, int iBillingCycleID, 
	                                    vector<AcctItemData *> & vAcctItemAggr);	    
	    //��ѯ�û�ָ�����ڼ�¼������¼����
		int getAllItemAggr(long lServID, int iBillingCycleID, 
	                                    vector<AcctItemData *> & vAcctItemAggr,
										int iOrgBillingCycleID = 0);

		//�޸��ڴ��е�����
		long insertAcctItemAggr(AcctItemData &itemData, bool &bNew, bool isModifyAcct=false);

	    //�ʻ����ʻ����û���Ŀ�ۻ�����ѯ
		/*
	    //��ѯָ���ʻ�ָ���û�ָ�����ڼ�¼������¼����
		int getAcctServAggr(long lAcctID,long lServID,int iBillingCycleID,
		                                        vector <long> &rvAcctItemData); 
	
	    //����ʻ��������û���ָ�����ڵķ���
		int  getAcctAllAggr(long lAcctID, int iBillingCycleID);
		
		//����ʻ��������û���ָ�����ڵķ���
		long  getAcctAllAggr(long lAcctID, int iBillingCycleID, 
	                                    vector<long> & rvAcctItemData);	                                
		*/
};

//##ModelId=42368B4B03BE
class TransactionMgr : public AcctItemAccuMgr, public OfferAccuMgr
{
  public:
    //##ModelId=42368E37000C
    TransactionMgr();

    //##ModelId=42368E41027D
    ~TransactionMgr();

	//�¼��ۻ�����ѯ
		//��ѯ�û�ָ������ָ���¼����͵Ľ��
	    long selectEventAggr(long lServID,int iBillingCycleID,
	                                            int iEventAggrTypeID);
		//��ѯ�û�ָ�������¼���¼������¼����
	    int selectAllEventAggr(long lServID, int iBillingCycleID, 
	                                        vector<AccuData *> & vEventAggr);

	//��Ŀ�ۻ�����ѯ
		//��ѯ�û�ָ������ָ����Ŀ���͵ķ���
	    long selectAcctItemAggr(long lServID,int iBillingCycleID,
	                                            int iAcctItemTypeID);                                            
		//��ѯ�û�ָ������ָ����Ŀ���͵ļ�¼������¼����
	    int selectDisctItemAggr(long lServID, int iBillingCycleID, 
	                                    vector<AcctItemData *> & vAcctItemAggr);
	    //��ѯ�û�ָ�����ڼ�¼������¼����
	    int selectAllItemAggr(long lServID, int iBillingCycleID, 
	                                vector<AcctItemData *> & vAcctItemAggr,int iOrgBillingCycleID = 0);

		//��ѯ�ڴ����û���ʷ�������˼�¼������¼����
		int selectHisItemAggr(long lServID,vector<AcctItemData *> & vAcctItemAggr);

};

#endif /* TRANSACTIONMGR_H_HEADER_INCLUDED_BDBC8749 */
