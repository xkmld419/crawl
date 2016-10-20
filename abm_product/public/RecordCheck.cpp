/*VER: 4*/
#include "RecordCheck.h"
#include "Environment.h"
#include "MBC.h"
#include "Log.h"
#include "CheckFunction.h"
#include "EventToolKits.h"
#include <string.h>
#include <string>
#ifndef PRIVATE_MEMORY
#include "ParamInfoInterface.h"
#endif

//CheckRuleInfo Class
///* Initial static member variable */
HashList<CheckRule *> *CheckRuleInfo::s_poRuleIndex = NULL;       
HashList<CheckSubRule *> *CheckRuleInfo::s_poSubRuleIndex = NULL;
HashList<CheckArgument *> *CheckRuleInfo::s_poArgumentIndex = NULL;    

CheckRule *CheckRuleInfo::s_poRuleList = NULL;
CheckSubRule *CheckRuleInfo::s_poSubRuleList = NULL;
CheckArgument *CheckRuleInfo::s_poArgumentList = NULL;
CheckRelation *CheckRuleInfo::s_poRelationList = NULL;

bool CheckRuleInfo::s_bUploaded                = false;
bool CheckRuleInfo::s_bRuleUploaded            = false; 
bool CheckRuleInfo::s_bSubRuleUploaded         = false;
bool CheckRuleInfo::s_bArgumentUploaded        = false;
bool CheckRuleInfo::s_bRelationUploaded        = false;
    
int CheckRuleInfo::nRuleSize        = 0;
int CheckRuleInfo::nSubRuleSize     = 0;
int CheckRuleInfo::nArgumentSize    = 0;
int  CheckRuleInfo::nRelationSize   = 0;
    

///* Public function */
CheckRuleInfo::CheckRuleInfo()
{}

CheckRuleInfo::~CheckRuleInfo()
{}

void CheckRuleInfo::load()
{
#ifdef PRIVATE_MEMORY	
	if (s_bUploaded)
		return;
	
	//����������̣�����������ϵ������ִ��˳���ܸı�
	loadRule();
	loadSubRule();
	loadArgument();
	loadRelation();
	TransFileMgr::load();
	TagSeqMgr::load();
		
	//��λ
	s_bUploaded = true;
#endif	
}

void CheckRuleInfo::unload()
{
    TagSeqMgr::unload();
    TransFileMgr::unload();
    unloadArgument();
	unloadSubRule();
	unloadRelation();
	unloadRule();

	s_bUploaded = false;
	
}

///* Private function */
void CheckRuleInfo::loadRule()
{
	int count, i;

	unloadRule();

	TOCIQuery qry(Environment::getDBConn());

	qry.setSQL ("select count(*) from Check_Rule");
	qry.open ();

	qry.next ();
	count = qry.field(0).asInteger () + 16;
	qry.close ();

	s_poRuleList = new CheckRule[count];
	s_poRuleIndex = new HashList<CheckRule *> ((count>>1) | 1);

	//MBC_CheckRule��,����MBC.h����Ӷ���
	if (!s_poRuleList || !s_poRuleIndex)
		THROW(MBC_CheckRule+1);

	qry.setSQL ( "select rule_id, nvl(TAG, -1) TAG, "
				 "nvl(err_code, -1) err_code, nvl(err_level, 0) err_level "
				 "from Check_Rule ");
	qry.open ();

	i = 0;
	int iRuleID = 0;
	int iTag    = 0;
	int iErrorCode = 0;
	while (qry.next ()) {
		if (i==count) THROW (MBC_CheckRule+2);
        
        iRuleID = qry.field(0).asInteger();
        iTag    = qry.field(1).asInteger();
        iErrorCode = qry.field(2).asInteger();
        
        if (iTag<0){ 	
				Log::log (0, "��Check_Rule�У�����(rule_id=%d), TAGֵ����Ч", 
				    iRuleID); 			
				continue;
		}
		if (iErrorCode == -1){ 	
				Log::log (0, "��Check_Rule�У�����(rule_id=%d), err_codeֵ�Ƿ�", iRuleID); 			
				continue;
		}	
		s_poRuleList[i].m_iRuleID = iRuleID;
		s_poRuleList[i].m_iTAG    = iTag;
		s_poRuleList[i].m_iErrCode = iErrorCode;
		s_poRuleList[i].m_iErrLevel = qry.field(3).asInteger();

		s_poRuleIndex->add (s_poRuleList[i].m_iRuleID, &(s_poRuleList[i]));
        
		i++;
	}
	qry.close ();
	
    nRuleSize = i;
	s_bRuleUploaded = true;
}

void CheckRuleInfo::loadSubRule()
{
	int count, i;

	unloadSubRule();

	TOCIQuery qry(Environment::getDBConn());

	qry.setSQL("select count(*) from Check_SubRule");
	qry.open();

	qry.next();
	count = qry.field(0).asInteger() + 16;
	qry.close();
	
	s_poSubRuleList = new CheckSubRule[count];
	s_poSubRuleIndex = new HashList<CheckSubRule *>((count>>1) | 1);

	//MBC_CheckSubRule��,����MBC.h����Ӷ���
	if (!s_poSubRuleList || !s_poSubRuleIndex)
		THROW(MBC_CheckSubRule+1);

	qry.setSQL(	"select a.sub_rule_id, nvl(b.rule_id, -1) rule_id, "
                "nvl(a.func_id, -1) func_id, nvl(a.check_seq, -1) check_seq, "
                "nvl(a.not_id, 0) not_id "
                "from Check_Subrule a, Check_Rule b "
                "where a.rule_id = b.rule_id(+) "
                "order by rule_id, check_seq asc " );
	qry.open();
	
	i = 0;
	int ioldRuleID = -2; //��Ϊ�յ�iRuleID����NVLת��Ϊ-1���ʴ˴�������-1
	CheckRule * poRule = NULL;
	CheckSubRule * poldSubRule = NULL;
	while (qry.next()){
		if (i == count) THROW(MBC_CheckSubRule + 2);

		//����SubRule�Ƿ���ڹ�����Rule
		int iSubRuleID = qry.field(0).asInteger();
		int iRuleID    = qry.field(1).asInteger();
		int iFuncID    = qry.field(2).asInteger();
		
		//�Ϸ��Լ��
		if (iRuleID == -1){ 	
			Log::log (0, "��Check_Subrule�У��ӹ���(sub_rule_id=%d), û���ҵ��������У�����", iSubRuleID); 			
			continue;
		}else if (iFuncID == -1){
		    Log::log (0, "��Check_Subrule�У��ӹ���(sub_rule_id=%d), func_idֵ�Ƿ�", iSubRuleID); 			
			continue;
		}
		
		//������,����Rule�ĵ�һ���ӹ�����ʹRuleָ��SubRule
		if ( ioldRuleID != iRuleID ){
			poRule = NULL;
			if (!s_poRuleIndex->get(iRuleID, &poRule )){ 	
				Log::log (0, "��Check_Subrule�У��ӹ���(sub_rule_id=%d), û���ҵ��������У�����", iSubRuleID); 			
				continue;
			}	
			poRule->m_opFirstSubRule = &(s_poSubRuleList[i]);
			ioldRuleID = iRuleID;
		}
		//�����ǵ�һ�ӹ�����ʹ��һ���ӹ���ָ���ӹ���
		else{
			poldSubRule->m_opNextSubRule = &(s_poSubRuleList[i]);
		}
		
		s_poSubRuleList[i].m_iSubRuleID  = iSubRuleID;
		s_poSubRuleList[i].m_iRuleID     = iRuleID;
		s_poSubRuleList[i].m_iFuncID     = iFuncID;
		s_poSubRuleList[i].m_iCheckSeq   = qry.field(3).asInteger();
		s_poSubRuleList[i].m_iNOT        = qry.field(4).asInteger();

		s_poSubRuleIndex->add(iSubRuleID, &(s_poSubRuleList[i]));

		poldSubRule = &(s_poSubRuleList[i]);
		i++;		
	}
	qry.close();

    nSubRuleSize = i;
	s_bSubRuleUploaded = true;
	
}

void CheckRuleInfo::loadArgument()
{
	int count, i;

	unloadArgument();

	TOCIQuery qry(Environment::getDBConn());

	qry.setSQL("select count(*) from Check_Argument");
	qry.open();

	qry.next();
	count = qry.field(0).asInteger() + 16;
	qry.close();

	s_poArgumentList = new CheckArgument[count];
	s_poArgumentIndex = new HashList<CheckArgument *>((count>>1) | 1);
	
	//MBC_CheckArgument��,����MBC.h����Ӷ���
	if (!s_poArgumentList || !s_poArgumentIndex)
		THROW(MBC_CheckArgument + 1);

	qry.setSQL(	"select a.arg_id, nvl(b.sub_rule_id, -1) sub_rule_id, "
                "nvl(a.location, -1) location, nvl(trim(a.value), '') value "
                "from Check_Argument a, Check_Subrule b "
                "where a.sub_rule_id = b.sub_rule_id(+) "
                "order by sub_rule_id, location  asc ");
	qry.open();

	i = 0;
	int ioldSubRuleID = -2;
	int ioldLocation  = -2;
	CheckSubRule *poSubRule = NULL;
	CheckArgument *poldSibArgument = NULL;
	CheckArgument *pParentSibArgument = NULL;
	CheckArgument *poldArgument = NULL;
	CheckArgument *prev = NULL;
	CheckArgument *curr = NULL;
	CheckArgument *p = NULL;
	
	
	while (qry.next()){
		if (i == count)
			THROW(MBC_CheckArgument + 2);

		//���˲����Ƿ���ڹ������ӹ���
		int iArgID     = qry.field(0).asInteger();
		int iSubRuleID = qry.field(1).asInteger();
		int iLocation  = qry.field(2).asInteger();
		
		//�Ϸ��Լ��
		if (iSubRuleID <=0 ){
	        Log::log (0, "��Check_Argument�У�����(arg_id=%d), sub_rule_idֵ�Ƿ�!", iArgID); 			
		    continue;
		}
		if (iLocation <0 ){
	        Log::log (0, "��Check_Argument�У�����(arg_id=%d), locationֵ�Ƿ�!", iArgID); 			
		    continue;
		}
		
		//�����ڣ��Ҵ˲�����SubRule�ĵ�һ����������ʹSubRuleָ��˲���
		if (ioldSubRuleID != iSubRuleID){
			poSubRule = NULL;
			if (!s_poSubRuleIndex->get(iSubRuleID, &poSubRule))	{ 	
				Log::log (0, "��Check_Argument�У�����(arg_id=%d), û���ҵ���������ӹ���", iArgID); 			
				continue;
			}
			
			poSubRule->m_opFirstAgrument = &(s_poArgumentList[i]);
			ioldSubRuleID = iSubRuleID;
			poldSibArgument = &(s_poArgumentList[i]);
		}
		//��������ͬһ���ӹ��򣬵�������λ�÷����ı�
		else if (ioldLocation != iLocation ){
		    pParentSibArgument = poldSibArgument;
		    poldSibArgument = &(s_poArgumentList[i]);
		    pParentSibArgument->m_poNextSibArgument = poldSibArgument;	
		}
		//��������ͬһ���ӹ��򣬲���λ��δ�����ı�
		else{
		    //��ͬһλ�õĲ����������ַ������ȣ��Ӵ�С��������
		    //���������Ϊ��ͷʱ����˳��������ƥ�䡣
		    prev = poldSibArgument;
		    curr = poldSibArgument;
		    p = &(s_poArgumentList[i]);
		    strcpy(s_poArgumentList[i].m_sValue, qry.field(3).asString());
		    
		    //���p�ĳ��ȴ���poldSibArgument�ĳ���
		    if (strlen(p->m_sValue)>strlen(curr->m_sValue)){
		        p->m_poNextArgument = curr;
		        poldSibArgument = p;
		        pParentSibArgument->m_poNextSibArgument = poldSibArgument;
		        prev = NULL;
		        curr = NULL;
		    }else{
		        prev = curr;
		        curr = curr->m_poNextArgument;
		    }
		    
		    //�ӵ�ǰ������ʼ�ң�ֱ���ҵ�һ���Ƚ�p����С�Ĳ���
		    while (curr!=NULL){
		        if (strlen(p->m_sValue) >= strlen(curr->m_sValue)){
		            p->m_poNextArgument = curr;
		            prev->m_poNextArgument = p;
		            break;		              
		        }
		        else{
		            prev = curr;
		            curr = curr->m_poNextArgument;
		        }
		    }
		    
		    //p�ĳ�����̣�Ӧ�����������
		    if (curr==NULL && prev!=NULL){
		        prev->m_poNextArgument = p;
		        poldArgument = p;
		    }
		}

		s_poArgumentList[i].m_iArgID     = iArgID;
		s_poArgumentList[i].m_iSubRuleID = iSubRuleID;
		s_poArgumentList[i].m_iLocation  = iLocation;
		strcpy(s_poArgumentList[i].m_sValue, qry.field(3).asString());
		
		ioldLocation = iLocation;
		i++;
	}
	qry.close();

    nArgumentSize = i;
	s_bArgumentUploaded = true;
	
}

void CheckRuleInfo::loadRelation()
{
	int count, i;

	unloadRelation();

	TOCIQuery qry(Environment::getDBConn());

	qry.setSQL("select count(*) from Check_Relation");
	qry.open();

	qry.next();
	count = qry.field(0).asInteger() + 16;
	qry.close();

	s_poRelationList = new CheckRelation[count];
	//s_poRuleIndex = new HashList<CheckRelation *>((count>>1)| 1);

	//MBC_CheckRelation��,����MBC.h����Ӷ���
	if (!s_poRelationList)
		THROW(MBC_CheckRelation + 1);

	qry.setSQL(	"select relation_id, rule_id, "
                "bill_type, record_type, check_seq, allow_null "
                "from ("
                "select a.relation_id, nvl(c.rule_id, -1) rule_id, "
                "nvl(b.trans_file_type_id, -1) bill_type, " 
                "nvl(a.record_type, -1) record_type, "
                "nvl(a.check_seq, -1) check_seq, "
                "nvl(a.allow_null,'N') allow_null "
                "from Check_Relation a, trans_file_type b, check_rule c "
                "where a.rule_id = c.rule_id(+) "
                "and   a.bill_type = b.trans_file_type_id(+) "
                "and   nvl(a.bFlag, 1) <>0 and nvl(a.bFlag,1)<>99) "
                "order by bill_type asc , record_type asc, check_seq asc " );
	qry.open();

	i = 0;
	int ioldBillType   = -2;
	int ioldRecordType = -2;
	CheckRelation *poldSibBill   = NULL;
	CheckRelation *poldSibRecord = NULL;
	CheckRelation *poldRelation  = NULL;
	while (qry.next()){
		if ( i == count) THROW(MBC_CheckRelation + 2);

		//���ж�RuleID�Ƿ������⽡�����ԣ���������m_poCheckRuleָ���У�����
		int iRelationID = qry.field(0).asInteger();
		int iRuleID     = qry.field(1).asInteger();
		int iBillType   = qry.field(2).asInteger();
		int iRecordType = qry.field(3).asInteger();
		
		//�Ϸ��Լ��
		if (!s_poRuleIndex->get(iRuleID, &(s_poRelationList[i].m_poCheckRule)))
		{		
			Log::log (0, "��Check_Relation�У�У���ϵ(relation_id=%d), û���ҵ������Ӧ��У�����", 
				         iRelationID);				
			continue;
		}else if (iBillType<=0){
		    
		    Log::log (0, "��Check_Relation�У�У���ϵ(relation_id=%d), bill_typeֵ�Ƿ�", 
				         iRelationID);				
			continue;
		}else if (iRecordType<0){
		    
		    Log::log (0, "��Check_Relation�У�У���ϵ(relation_id=%d), record_typeֵ�Ƿ�", 
				         iRelationID);				
			continue;
		}

		//��һ��ѭ����ִ�г�ʼ��
		if (i == 0)
		{
			ioldBillType   = iBillType;
			ioldRecordType = iRecordType;
			poldSibBill    = &(s_poRelationList[i]);
			poldSibRecord  = &(s_poRelationList[i]);
		}
		//�����У���ϵ��ǰһ��У���ϵ��BillType����ͬ
		else if (ioldBillType != iBillType){
			poldSibBill->m_poSibBillRelation = &(s_poRelationList[i]);
			poldSibBill = &(s_poRelationList[i]);
			poldSibRecord = &(s_poRelationList[i]);
		}
		//�����У���ϵ��ǰһ��У���ϵ��BillType��ͬ����RecordType����ͬ
		else if (ioldBillType == iBillType && ioldRecordType != iRecordType){
			poldSibRecord->m_poSibRecordRelation = &(s_poRelationList[i]);
			poldSibRecord = poldSibRecord->m_poSibRecordRelation;
		}
		//BillType��RecordType����ͬ
		else{
			poldRelation->m_poNextRelation = &(s_poRelationList[i]);
		}
		
		s_poRelationList[i].m_iRelationID = iRelationID;
		s_poRelationList[i].m_iRuleID     = iRuleID;
		s_poRelationList[i].m_iBillType   = iBillType;
		s_poRelationList[i].m_iRecordType = iRecordType;
		s_poRelationList[i].m_iCheckSeq   = qry.field(4).asInteger();
		strcpy(s_poRelationList[i].m_sAllowNull,qry.field(5).asString());
		s_poRelationList[i].m_sAllowNull[1] = 0;
		
		ioldBillType = iBillType;
		ioldRecordType = iRecordType;
		poldRelation = &(s_poRelationList[i]);
		i++;	
	}
	qry.close();

    nRelationSize = i;
	s_bRelationUploaded = true;
	
}

///* Private Function */
void CheckRuleInfo::unloadRule()
{
	delete [] s_poRuleIndex;
	delete [] s_poRuleList;

	s_poRuleIndex = NULL;
	s_poRuleList = NULL;
	nRuleSize = 0;
	s_bRuleUploaded = false;
}

void CheckRuleInfo::unloadSubRule()
{
	delete [] s_poSubRuleIndex;
	delete [] s_poSubRuleIndex;

	s_poSubRuleIndex = NULL;
	s_poSubRuleIndex = NULL;
	nSubRuleSize = 0;
	s_bSubRuleUploaded = false;
}

void CheckRuleInfo::unloadArgument()
{
	delete [] s_poArgumentIndex;
	delete [] s_poArgumentList;
	
	s_poArgumentIndex = NULL;
	s_poArgumentList = NULL;
	nArgumentSize = 0;
	s_bArgumentUploaded = false;
}

void CheckRuleInfo::unloadRelation()
{
	delete [] s_poRelationList;

	s_poRelationList = NULL;
	nRelationSize = 0;
	s_bRelationUploaded = false;
}


//CheckRule Class
CheckRule::CheckRule()
:	m_iRuleID(0), m_iTAG(0), m_iErrCode(0), m_iErrLevel(0),
	m_opFirstSubRule(NULL)
{
}

CheckRule::~CheckRule()
{
	m_opFirstSubRule = NULL;
}

///* Pulbic Function */
#ifdef PRIVATE_MEMORY
bool CheckRule::execute(EventSection *poEventSection, char *sAllowNull, int iBillType,int iRecordType,bool bCheckWarning)
{
	//����У�龯�漶��Ĺ���
	if (!bCheckWarning && m_iErrLevel==3)
		return true;	

    int iAttrID = TagSeqMgr::getArrtID(poEventSection->m_iGroupID, m_iTAG);
    if (iAttrID==-1){
	    Log::log (0, "��b_asn1_tag_seq�У�δ�ҵ���GroupID=%d AND TAG=%d "
	        "��Ӧ��Attr_ID, ����У�����(Rule_ID=%d)��TAGֵ��"
	        "��b_asn1_tag_seq��������Ƿ���ȷ, ��У�������Ч����", 
	    poEventSection->m_iGroupID, m_iTAG, m_iRuleID);
	    
	    //��ȻTAG��Ч����ô��У�����Ҳ��Ч
	    return true; 
	}
   	EventToolKits toolKits;
	string sValue =  toolKits.get_param(poEventSection, iAttrID);
	 //�������Ϊ��ֵ����ֵΪ��ʱ�������ӹ���У��
	if (sValue.length() == 0 && (strcmp(sAllowNull,"Y") == 0 || strcmp(sAllowNull,"y") == 0) ) 
		return true;
	    
	CheckSubRule *pSubRule = m_opFirstSubRule;	
	//if (pSubRule->m_iFuncID == 5)
	//	return true;
	while (pSubRule != NULL && pSubRule->execute(poEventSection, m_iTAG)){
		pSubRule = pSubRule->m_opNextSubRule;
		//if (pSubRule->m_iFuncID == 5)
		//	break;
	}

	//�������ӹ���ִ�гɹ���������У������������
	//���ߣ����������һ���ӹ���ִ��ʧ�ܣ���ʱ�������������㣬
	//Ҳ��У��ͨ����
	if (pSubRule==NULL || pSubRule->m_opNextSubRule!=NULL)
		return true;

	//�����һ��У�����δͨ�����Ѵ������д����׼�¼���m_iErrorID������
	poEventSection->m_iErrorID = m_iErrCode;
	
	//�ѳ��������ID������servID�У���д�����ļ�����ʹ��
	poEventSection->m_iServID = iAttrID;
	
	//test code �ں�̨��ӡ������Ϣ
//	EventToolKits toolKits;
	Log::log(0, "������δͨ������(BillType=%d,RecordType=%d,Rule_ID=%d, TAG=%d)��У�飺m_iAttrID=%d m_iErrorID=%d ����ֵ=%s,        �����ļ�:%s,��%d��", 
	    iBillType,iRecordType,m_iRuleID, m_iTAG, iAttrID, m_iErrCode, toolKits.get_param(poEventSection, iAttrID).c_str(),poEventSection->m_oEventExt.m_sFileName,poEventSection->m_iEventSeq);
	
	return false;
}
#else
bool CheckRule::execute(EventSection *poEventSection, char *sAllowNull, int iBillType,int iRecordType,bool bCheckWarning)
{
	CheckSubRule *pCheckSubRule = (CheckSubRule*)(G_PPARAMINFO->m_poCheckSubRuleList);
	//����У�龯�漶��Ĺ���
	if (!bCheckWarning && m_iErrLevel==3)
		return true;	
    int iAttrID = TagSeqMgr::getArrtID(poEventSection->m_iGroupID, m_iTAG);
    if (iAttrID==-1){
	    Log::log (0, "��b_asn1_tag_seq�У�δ�ҵ���GroupID=%d AND TAG=%d "
	        "��Ӧ��Attr_ID, ����У�����(Rule_ID=%d)��TAGֵ��"
	        "��b_asn1_tag_seq��������Ƿ���ȷ, ��У�������Ч����", 
	    poEventSection->m_iGroupID, m_iTAG, m_iRuleID);
	    //��ȻTAG��Ч����ô��У�����Ҳ��Ч
	    return true; 
	}
   	EventToolKits toolKits;
	string sValue =  toolKits.get_param(poEventSection, iAttrID);
	 //�������Ϊ��ֵ����ֵΪ��ʱ�������ӹ���У��
	if (sValue.length() == 0 && (strcmp(sAllowNull,"Y") == 0 || strcmp(sAllowNull,"y") == 0) ) 
		return true;
	unsigned int iFirstSubRuleTmp = m_iFirstSubRule;    
	CheckSubRule *pSubRule = NULL;	
	if(iFirstSubRuleTmp){
		pSubRule = &(pCheckSubRule[iFirstSubRuleTmp]);
	}
	//if (pSubRule->m_iFuncID == 5)
	//	return true;
	while (pSubRule != NULL && pSubRule->execute(poEventSection, m_iTAG)){
		//pSubRule = pSubRule->m_opNextSubRule;
		if(pSubRule->m_iNextSubRule){
			pSubRule = &(pCheckSubRule[pSubRule->m_iNextSubRule]);
		}else{
			pSubRule = NULL;
		}
		//if (pSubRule->m_iFuncID == 5)
		//	break;
	}
	//�������ӹ���ִ�гɹ���������У������������
	//���ߣ����������һ���ӹ���ִ��ʧ�ܣ���ʱ�������������㣬
	//Ҳ��У��ͨ����
	if (pSubRule==NULL || pSubRule->m_iNextSubRule!=0)
		return true;
	//�����һ��У�����δͨ�����Ѵ������д����׼�¼���m_iErrorID������
	poEventSection->m_iErrorID = m_iErrCode;
	//�ѳ��������ID������servID�У���д�����ļ�����ʹ��
	poEventSection->m_iServID = iAttrID;
	//  test code �ں�̨��ӡ������Ϣ
	//	EventToolKits toolKits;
	Log::log(0, "������δͨ������(BillType=%d,RecordType=%d,Rule_ID=%d, TAG=%d)��У�飺m_iAttrID=%d m_iErrorID=%d ����ֵ=%s,        �����ļ�:%s,��%d��", 
	    iBillType,iRecordType,m_iRuleID, m_iTAG, iAttrID, m_iErrCode, toolKits.get_param(poEventSection, iAttrID).c_str(),poEventSection->m_oEventExt.m_sFileName,poEventSection->m_iEventSeq);
	return false;
}
#endif

//CheckSubRule Calss
CheckSubRule::CheckSubRule()
:	m_iSubRuleID(0), m_iRuleID(0), m_iFuncID(0), m_iCheckSeq(0),
	m_opNextSubRule(NULL), m_opFirstAgrument(NULL)
{
}

CheckSubRule::~CheckSubRule()
{
	m_opNextSubRule = NULL;
	m_opFirstAgrument = NULL;
}

///* Public Function */
bool CheckSubRule::execute(const EventSection *poEventSection, int iTAG)
{
#ifdef PRIVATE_MEMORY	
	return (CheckFunction::doFunction(m_iFuncID, poEventSection, iTAG, 
	    m_opFirstAgrument)!= m_iNOT);
#else
	if(m_iFirstAgrument){
		return (CheckFunction::doFunction(m_iFuncID, poEventSection, iTAG, 
	    &(G_PPARAMINFO->m_poCheckArgumentList[m_iFirstAgrument]))!= m_iNOT);
	}else{
		return (CheckFunction::doFunction(m_iFuncID, poEventSection, iTAG, 
	    NULL)!= m_iNOT);
	}
#endif

}

//CheckArgument Class
CheckArgument::CheckArgument()
:	m_iArgID(0), m_iSubRuleID(0), m_iLocation(0),
	m_poNextSibArgument(NULL), m_poNextArgument(NULL)
{
	memset(m_sValue, 0, sizeof(m_sValue));
}

CheckArgument::~CheckArgument()
{
	m_poNextArgument = NULL;
	m_poNextSibArgument = NULL;
}

//CheckRelation Class
CheckRelation::CheckRelation()
	:m_iRelationID(0), m_iRuleID(0), m_iBillType(0), m_iRecordType(0),
	 m_iCheckSeq(0), m_poCheckRule(NULL), m_poSibBillRelation(NULL), 
	 m_poSibRecordRelation(NULL), m_poNextRelation(NULL)
{
}

CheckRelation::~CheckRelation()
{
}

//RecordCheck Class
RecordCheck::RecordCheck()
{}

RecordCheck::~RecordCheck()
{}

///* Public Function */
#ifdef PRIVATE_MEMORY
bool RecordCheck::check(EventSection *poEventSection, bool bCheckWarning)
{
	//����У�������صı�, ֻ������һ��
	CheckRuleInfo::load();
	
	//�ɻ�����BillType��RecordType,����ִ����ЩУ�����
	int iBillType   = getBillType(poEventSection);
	int iRecordType = getRecordType(poEventSection);

	CheckRelation * pRelation = CheckRuleInfo::s_poRelationList;	
	while (pRelation != NULL && pRelation->m_iBillType != iBillType)\
		pRelation=pRelation->m_poSibBillRelation;
	
    //��δ�ҵ�iBillType��У����򣬱�ʾ���������������У�飬����true
	if (pRelation == NULL)
		return true;
	
	while (pRelation!= NULL && pRelation->m_iRecordType != iRecordType)
		pRelation = pRelation->m_poSibRecordRelation;
	
	//��δ�ҵ�iRecordType��У����򣬱�ʾ���������������У�飬����true
	if (pRelation == NULL)
		return true;
	
	//�ҵ���ͬ���У�����������ִ�У�ֱ�����ϲ���ͨ��У��Ĺ����
	//ֹͣУ��
	bool isPassed = true;
	while (pRelation!= NULL && isPassed ){  
//	while (pRelation != NULL){ //�����ã���ÿ���ֶ�У���ȥ		
		isPassed = pRelation->m_poCheckRule->execute(poEventSection, pRelation->m_sAllowNull,pRelation->m_iBillType,pRelation->m_iRecordType,bCheckWarning);		
		pRelation = pRelation->m_poNextRelation;
	}

	if (isPassed)
		return true;
	
	return false;
		
}
#else
bool RecordCheck::check(EventSection *poEventSection, bool bCheckWarning)
{
	//����У�������صı�, ֻ������һ��
	//CheckRuleInfo::load();
	//�ɻ�����BillType��RecordType,����ִ����ЩУ�����
	int iBillType   = getBillType(poEventSection);
	int iRecordType = getRecordType(poEventSection);
	CheckRelation * pCheckRelation = (CheckRelation *)(G_PPARAMINFO->m_poCheckRelationList);//CheckRuleInfo::s_poRelationList;	
	CheckRule *pCheckRule = (CheckRule *)(G_PPARAMINFO->m_poCheckRuleList);
	CheckRelation * pRelation = &(G_PPARAMINFO->m_poCheckRelationList[1]);
	CheckRelation * pRelationNull = &(G_PPARAMINFO->m_poCheckRelationList[0]);
	while (pRelation != NULL && pRelation != pRelationNull && pRelation->m_iBillType != iBillType){
		if(pRelation->m_iSibBillRelation){
			pRelation = &(pCheckRelation[pRelation->m_iSibBillRelation]);
		}else{
			pRelation = NULL;
		}
	}
    //��δ�ҵ�iBillType��У����򣬱�ʾ���������������У�飬����true
	if (pRelation == NULL)
		return true;
	while (pRelation!= NULL && pRelation->m_iRecordType != iRecordType){
		if(pRelation->m_iSibBillRelation){
			pRelation = &(pCheckRelation[pRelation->m_iSibBillRelation]);
		}else{
			pRelation = NULL;
		}
		//pRelation = pRelation->m_poSibRecordRelation;
	}
	//��δ�ҵ�iRecordType��У����򣬱�ʾ���������������У�飬����true
	if (pRelation == NULL)
		return true;
	//�ҵ���ͬ���У�����������ִ�У�ֱ�����ϲ���ͨ��У��Ĺ����
	//ֹͣУ��
	bool isPassed = true;
	while (pRelation!= NULL && isPassed ){  
	//	while (pRelation != NULL){ //�����ã���ÿ���ֶ�У���ȥ
			CheckRule *pCheckRuleTmp = new CheckRule;
			if(pRelation->m_iCheckRule){	
				 memcpy(pCheckRuleTmp,&(pCheckRule[pRelation->m_iCheckRule]),sizeof(class CheckRule));	
			}else{
				 pCheckRuleTmp->m_iFirstSubRule = 0;
			}
			isPassed = pCheckRuleTmp->execute(poEventSection, pRelation->m_sAllowNull,pRelation->m_iBillType,pRelation->m_iRecordType,bCheckWarning);		
			if(pRelation->m_iNextRelation){
				pRelation = &(pCheckRelation[pRelation->m_iNextRelation]);
			}else{
				pRelation = NULL;
			}
			delete pCheckRuleTmp;
			pCheckRuleTmp = 0;
			//pRelation = pRelation->m_poNextRelation;
	}
	if (isPassed)
		return true;
	return false;
}
#endif

///* Private Function */
int RecordCheck::getBillType(EventSection *poEventSection)
{	
    int iPos = 0, iBillType;
    char sHead[20];
    char const *p1;
    char const *p2;
    memset(sHead, 0, sizeof(sHead));
    p1 = (poEventSection->m_oEventExt).m_sFileName;    

    if (p1==NULL || *p1 == '\0'){		
		Log::log (0, "��׼�¼���m_sFileNameֵ��ӦΪ��!");				
		return -1;
	}
	
    //p2 = strchr(p1, '_');
    //strncpy(sHead, p1, p2-p1);

    //ͨ���ļ�����ͷ�����ж�billType
    iBillType = TransFileMgr::getFileTypeID(p1);
   
    return iBillType;        
}

int RecordCheck::getRecordType(EventSection *poEventSection)
{
	return poEventSection->m_iCallTypeID;
}

//TransFileMgr Class
int  TransFileMgr::nSize       = 0;
bool TransFileMgr::s_bUploaded = false;
TransFileType *TransFileMgr::s_poFileTypeList = NULL;
HashList<TransFileType *> *TransFileMgr::s_poFileTypeIndex = NULL;
    
void TransFileMgr::load()
{
    if (s_bUploaded)
		return;
		
    int count, i;

	TOCIQuery qry(Environment::getDBConn());

	qry.setSQL("select count(*) from trans_file_type");
	qry.open();

	qry.next();
	count = qry.field(0).asInteger() + 16;
	qry.close();
	
	s_poFileTypeList = new TransFileType[count];
	s_poFileTypeIndex = new HashList<TransFileType *>((count>>1) | 1);

	//MBC_CheckSubRule��,����MBC.h����Ӷ���
	if (!s_poFileTypeList || !s_poFileTypeIndex)
		THROW(MBC_TransFileType+1);

	/*qry.setSQL(	"select a.trans_file_type_id type_id, "
                "nvl(a.trans_file_type_desc, '') type_name,  "
                "nvl(b.file_path, '') path, nvl(b.temp_file_path, '') tmp_path "
                "from trans_file_type a, trans_file_store_config b "
                "where a.trans_file_type_id = b.trans_file_type_id(+) "
                "order by type_id asc " );*/
       qry.setSQL(" SELECT filetypeid type_id,nvl(filerule,'') type_name, "
			"        nvl(b.file_path,'') PATH,                      "
			"        NVL(B.TEMP_FILE_PATH,'') TMP_PATH,             "
			"        nvl(err_head,0)  err_head,nvl(err_tail,0) err_tail,nvl(write_file,1) write_file "
			" FROM                                                  "
			"      (SELECT filetypeid,filerule,err_head,err_tail,write_file FROM b_tap3file      "
			"       UNION                                           "
			"       SELECT filetypeid,filerule,err_head,err_tail,write_file FROM b_sepfile) a,   "
			"       TRANS_FILE_STORE_CONFIG B                       "
			"  WHERE a.filetypeid=b.trans_file_type_id(+)           "
			"  ORDER BY type_id  ");
	qry.open();
	
	i = 0;
	TransFileType *pOldFileType = NULL;
	while (qry.next()){
		if (i == count) THROW(MBC_TransFileType + 2);
		
		s_poFileTypeList[i].m_iTypeID  = qry.field(0).asInteger();
		strcpy(s_poFileTypeList[i].m_sTypeName, qry.field(1).asString());
		strcpy(s_poFileTypeList[i].m_sPath,     qry.field(2).asString());
		strcpy(s_poFileTypeList[i].m_sTmpPath,  qry.field(3).asString());
		s_poFileTypeList[i].m_iErrHead = qry.field(4).asInteger();
		s_poFileTypeList[i].m_iErrTail = qry.field(5).asInteger();
		s_poFileTypeList[i].m_iWriteFile = qry.field(6).asInteger();
		
		s_poFileTypeIndex->add(s_poFileTypeList[i].m_iTypeID, &(s_poFileTypeList[i]));
		i++;	
	}
	qry.close();
    
    nSize = i;
	s_bUploaded = true;
}

void TransFileMgr::unload()
{
    delete [] s_poFileTypeIndex;
    delete [] s_poFileTypeList;
    
    s_bUploaded = false;
    nSize = 0;
    return;
}

#ifdef PRIVATE_MEMORY
const char* TransFileMgr::getFileTypeName(int iTypeID)
{
    if (s_poFileTypeIndex==NULL|| iTypeID<=0)
        return NULL;
        
    TransFileType *poFileType;
    if (!s_poFileTypeIndex->get(iTypeID, &poFileType))
        return NULL;
    
    return poFileType->m_sTypeName;      
}
#else
const char* TransFileMgr::getFileTypeName(int iTypeID)
{
 	if( iTypeID<=0) return NULL;       
    TransFileType *poFileType = (TransFileType *)(G_PPARAMINFO->m_poTransFileTypeList);
	unsigned int iFileType = 0;
    if (!G_PPARAMINFO->m_poTransFileTypeIndex->get(iTypeID, &iFileType))
        return NULL;
    return poFileType[iFileType].m_sTypeName;      
}
#endif

#ifdef PRIVATE_MEMORY
const char* TransFileMgr::getFileTypePath(int iTypeID)
{
    if (s_poFileTypeIndex==NULL|| iTypeID<=0)
        return NULL;
        
    TransFileType *poFileType;
    if (!s_poFileTypeIndex->get(iTypeID, &poFileType))
        return NULL;
    
    return poFileType->m_sPath; 
}
#else
const char* TransFileMgr::getFileTypePath(int iTypeID)
{
    if (iTypeID<=0) return NULL;
    TransFileType *poFileType = (TransFileType*)(G_PPARAMINFO->m_poTransFileTypeList);
	unsigned int iFileType = 0;
    if (!G_PPARAMINFO->m_poTransFileTypeIndex->get(iTypeID, &iFileType))
        return NULL;
    return poFileType[iFileType].m_sPath; 
}
#endif

#ifdef PRIVATE_MEMORY
const char* TransFileMgr::getFileTypeTmpPath(int iTypeID)
{
    if (s_poFileTypeIndex==NULL || iTypeID<=0)
        return NULL;
        
    TransFileType *poFileType;
    if (!s_poFileTypeIndex->get(iTypeID, &poFileType))
        return NULL;
    
    return poFileType->m_sTmpPath; 
        
}
#else
const char* TransFileMgr::getFileTypeTmpPath(int iTypeID)
{
    if (iTypeID<=0) return NULL; 
    TransFileType *poFileType = (TransFileType*)(G_PPARAMINFO->m_poTransFileTypeList);
	unsigned int iFileType = 0;
    if (!G_PPARAMINFO->m_poTransFileTypeIndex->get(iTypeID, &iFileType))
        return NULL;   
    return poFileType[iFileType].m_sTmpPath;        
}
#endif

#ifdef PRIVATE_MEMORY
int TransFileMgr::getFileErrHead(int iTypeID)
{
	if (s_poFileTypeIndex==NULL || iTypeID<=0)
        	return 0;

	TransFileType *poFileType;
    if (!s_poFileTypeIndex->get(iTypeID, &poFileType))
        return 0;
    
    return poFileType->m_iErrHead; 
}
#else
int TransFileMgr::getFileErrHead(int iTypeID)
{
	if (iTypeID<=0) return NULL; 
    TransFileType *poFileType = (TransFileType*)(G_PPARAMINFO->m_poTransFileTypeList);
	unsigned int iFileType = 0;
    if (!G_PPARAMINFO->m_poTransFileTypeIndex->get(iTypeID, &iFileType))
        return NULL;  
    return poFileType[iFileType].m_iErrHead; 
}
#endif

#ifdef PRIVATE_MEMORY
int TransFileMgr::getFileErrTail(int iTypeID)
{
	if (s_poFileTypeIndex==NULL || iTypeID<=0)
        	return 0;

	TransFileType *poFileType;
    if (!s_poFileTypeIndex->get(iTypeID, &poFileType))
        return 0;
    
    return poFileType->m_iErrTail; 
}
#else
int TransFileMgr::getFileErrTail(int iTypeID)
{
	if (iTypeID<=0)	return 0;
	TransFileType *poFileType = (TransFileType*)(G_PPARAMINFO->m_poTransFileTypeList);
	unsigned int iFileType = 0;
    if (!G_PPARAMINFO->m_poTransFileTypeIndex->get(iTypeID, &iFileType))
        return NULL;  
    return poFileType[iFileType].m_iErrTail; 
}
#endif


int TransFileMgr::getWriteErrFile(int iTypeID)
{
	if (s_poFileTypeIndex==NULL || iTypeID<=0)
        	return 0;

	TransFileType *poFileType;
    if (!s_poFileTypeIndex->get(iTypeID, &poFileType))
        return 0;
    
    return poFileType->m_iWriteFile; 
}

#ifdef PRIVATE_MEMORY
int TransFileMgr::getFileTypeID(const char *sFileTypeName)
{
    if (nSize<=0 || s_poFileTypeList==NULL || sFileTypeName==NULL)
        return -1;
    
    int i;    
    int len = strlen(sFileTypeName);
    char sTmp[64];
    char *p;
    memset(sTmp, 0, sizeof(sTmp));
    
    if (len>sizeof(sTmp)-1)
        return -1;
    
   /* for (i=0; i<nSize; i++){
        strncpy(sTmp, s_poFileTypeList[i].m_sTypeName, len);        
        if (strcmp(sTmp, sFileTypeName)==0)
            return s_poFileTypeList[i].m_iTypeID;       
    }*/

    for (i=0;i<nSize;i++)
    {
    	  int iLen = strlen(s_poFileTypeList[i].m_sTypeName);
         p = s_poFileTypeList[i].m_sTypeName;
	  int j=0;
	  for (j=0;j<iLen;j++)
	  {
	  	if (p[j] == '?')
			continue;
		if (sFileTypeName[j] != p[j])
			break;
	  }

	  if (j == iLen)
	 {		
		return s_poFileTypeList[i].m_iTypeID;
	 }	
    }
    
    return -1;
}
#else
int TransFileMgr::getFileTypeID(const char *sFileTypeName)
{
    TransFileType *pTransFileType = (TransFileType*)(G_PPARAMINFO->m_poTransFileTypeList);
	int nsizeTmp = G_PPARAMINFO->m_poTransFileTypeData->getCount();
	if (nsizeTmp<1 ||sFileTypeName==NULL)
        return -1;
    int i;    
    int len = strlen(sFileTypeName);
    char sTmp[64];
    char *p;
    memset(sTmp, 0, sizeof(sTmp)); 
    if (len>sizeof(sTmp)-1)
        return -1;   
   // for (i=0; i<nSize; i++){
   //     strncpy(sTmp, s_poFileTypeList[i].m_sTypeName, len);        
   //     if (strcmp(sTmp, sFileTypeName)==0)
   //         return s_poFileTypeList[i].m_iTypeID;       
   // }
    for (i=1;i<=nsizeTmp;i++)
    {
    	  int iLen = strlen(pTransFileType[i].m_sTypeName);
          p = pTransFileType[i].m_sTypeName;
	  	  int j=0;
	  	  for (j=0;j<iLen;j++)
	  	 {
	  		if (p[j] == '?')
				continue;
			if (sFileTypeName[j] != p[j])
				break;
	  	 }
	  	 if (j == iLen)
	 	 {		
			return pTransFileType[i].m_iTypeID;
	 	 }	
   }
   return -1;
}
#endif

const char* TransFileMgr::getFileTypeName(const char *sFileTypeName)
{
    return getFileTypeName(getFileTypeID(sFileTypeName));
}

const char* TransFileMgr::getFileTypePath(const char *sFileTypeName)
{
    return getFileTypePath(getFileTypeID(sFileTypeName));
}

const char* TransFileMgr::getFileTypeTmpPath(const char *sFileTypeName)
{
    return getFileTypeTmpPath(getFileTypeID(sFileTypeName));
}

int TransFileMgr::getFileErrHead(const char *sFileTypeName)
{
	return getFileErrHead(getFileTypeID(sFileTypeName));
}

int TransFileMgr::getFileErrTail(const char *sFileTypeName)
{
	return getFileErrTail(getFileTypeID(sFileTypeName));
}

int TransFileMgr::getWriteErrFile(const char * sFileTypeName)
{
	return getWriteErrFile(getFileTypeID(sFileTypeName));
}

//test code
void CheckArgument::showMe(int iLevel)
{
    CheckArgument *p = this;
    int i = 0;
    
    
    do{
        int j = iLevel*4;
        while(j--){
            printf(" ");
        }
        
        if (i==0)
            printf("|---");
        else
            printf("    ");
        
        j = i;
        while (j--){
            printf(" ");
        }
        
        if (i!=0)
            printf("��");
        
        printf("ArgID:%d SubRuleID:%d Location:%d Value:%s\n", 
        p->m_iArgID, p->m_iSubRuleID, p->m_iLocation, p->m_sValue);
        p = p->m_poNextArgument;
        i++;
    
    }while(p!=NULL);
                
}

void CheckSubRule::showMe(int iLevel)
{
    int i = iLevel*4;
    while(i--){
        printf(" ");
    }
    printf("|---");
    printf("SubRuleID:%d RuleID:%d FuncID:%d CheckSeq:%d NOT:%d\n", 
        m_iSubRuleID, m_iRuleID, m_iFuncID, m_iCheckSeq, m_iNOT);
   
   CheckArgument *p = m_opFirstAgrument;
   while(p!=NULL){
        p->showMe(iLevel+1);
        p = p->m_poNextSibArgument;
   }
        
}

void CheckRule::showMe(int iLevel)
{
    int i=iLevel*4;
    while(i--){
        printf(" ");
    }
    printf("|---");
    printf("RuleID:%d TAG:%d ErrCode:%d ErrLevel:%d\n", 
        m_iRuleID, m_iTAG, m_iErrCode, m_iErrLevel);
   
   CheckSubRule *p = m_opFirstSubRule;
   while(p!=NULL){
        p->showMe(iLevel+1);
        p = p->m_opNextSubRule;
   } 
        
}

void CheckRelation::showMe(int iLevel)
{
    CheckRelation *p = this;
    
    int i = 0;
    do{
        int j = iLevel*4;
        while(j--){
            printf(" ");
        }
        
        if (i==0)
            printf("|---");
        else
            printf("    ");
        
        j = i;
        while (j--){
            printf(" ");
        }
        
        if (i!=0)
            printf("��");
        printf("RelationID:%d RuleID:%d BillType:%d m_iRecordType:%d CheckSeq:%d "
            "#SibBillID:%d #SibRecordID:%d #NextID:%d\n", 
            p->m_iRelationID, p->m_iRuleID, p->m_iBillType, p->m_iRecordType, p->m_iCheckSeq,
            (p->m_poSibBillRelation != NULL)?(p->m_poSibBillRelation->m_iRelationID):-1, 
            (p->m_poSibRecordRelation != NULL)?(p->m_poSibRecordRelation->m_iRelationID):-1,
            (p->m_poNextRelation != NULL)?(p->m_poNextRelation->m_iRelationID):-1);
                
        if (p->m_poCheckRule!=NULL)
            p->m_poCheckRule->showMe(iLevel+1+i);
        
        p = p->m_poNextRelation; 
        i++; 
   }while (p!=NULL);
}

void TagSeq::showMe(int iLevel)
{
    TagSeq *p = this;
    
    int i = 0;
    do{
        int j = iLevel*4;
        while(j--){
            printf(" ");
        }
        
        if (i==0)
            printf("|---");
        else
            printf("    ");
        
        j = i;
        while (j--){
            printf(" ");
        }
        
        if (i!=0)
            printf("��");
        printf("GroupID:%d TAG:%d AttrID:%d m_iSeq:%d Name:%s "
            "#SibTagGroupID:%d #NextTagID:%d\n", 
            p->m_iGroupID, p->m_iTAG, p->m_iAttrID, p->m_iSeq, p->m_sName,
            (p->m_poNextSibTagSeq != NULL)?(p->m_poNextSibTagSeq->m_iGroupID):-1, 
            (p->m_poNextTagSeq != NULL)?(p->m_poNextTagSeq->m_iTAG):-1);
        
        p = p->m_poNextTagSeq; 
        i++; 
   }while (p!=NULL);
}

void TagSeqMgr::showMe()
{
    if (nSize==0)
    return;
    int i=0;
    printf("\nTagSeq\n");
    TagSeq *p = &s_poTagSeqList[0];
    while (p!=NULL){
        p->showMe(1);
        p = p->m_poNextSibTagSeq;
        i++;
    }
    printf("��%d�飬�ϼ�%d��TagSeq\n", i, nSize);
	
}

//Class TagSeqMgr
///* Initial static member variable */
int TagSeqMgr::nSize        = 0;
bool TagSeqMgr::s_bUploaded = false;
TagSeq *TagSeqMgr::s_poTagSeqList = NULL;
    
///Public function

void TagSeqMgr::load()
{
    if (s_bUploaded)
		return;
		
    int count, i;

	TOCIQuery qry(Environment::getDBConn());

	qry.setSQL("select count(*) from b_asn1_tag_seq");
	qry.open();

	qry.next();
	count = qry.field(0).asInteger() + 16;
	qry.close();
	
	s_poTagSeqList = new TagSeq[count];

	//MBC_CheckSubRule��,����MBC.h����Ӷ���
	if (!s_poTagSeqList)
		THROW(MBC_TagSeq+1);

	qry.setSQL(	"select a.group_id, a.tag, nvl(b.event_attr_id, -1) attr_id, "
                "nvl(a.seq, -1) seq, nvl(a.name, '') name "
                "from b_asn1_tag_seq a, b_asn1_field b "
                "where a.group_id = b.group_id(+) "
                "and   a.tag = b.tag(+) "
                "AND a.tag_seq=b.tag_seq(+) "
                "AND a.group_id  IN (SELECT DISTINCT group_id FROM b_asn1_field) "
                "union "
                "SELECT A.GROUP_ID, "                      
		  "       A.TAG, "                    
		  "       NVL(B.EVENT_ATTR_ID, -1) ATTR_ID, "
		  "       NVL(A.SEQ, -1) SEQ, "
		  "       NVL(A.NAME, '') NAME "      
		  "  FROM B_ASN1_TAG_SEQ A, b_sep_field B "
		  " WHERE A.GROUP_ID = B.GROUP_ID(+) "
		  "   AND a.seq=b.field_seq "     
		  "   AND a.group_id  IN (SELECT DISTINCT group_id FROM b_sep_field) "
                " order by group_id asc, seq asc " );
	qry.open();
	
	i = 0;	
    TagSeq *oldSibTag = NULL;
    TagSeq *prevTag   = NULL;
    
	while (qry.next()){
		if (i == count) THROW(MBC_TagSeq + 2);
		
		s_poTagSeqList[i].m_iGroupID   = qry.field(0).asInteger();
		s_poTagSeqList[i].m_iTAG       = atoi(qry.field(1).asString()); 
		s_poTagSeqList[i].m_iAttrID    = qry.field(2).asInteger();
		s_poTagSeqList[i].m_iSeq       = qry.field(3).asInteger();				
		strcpy(s_poTagSeqList[i].m_sName, qry.field(4).asString());
		
		if (i==0){
		    oldSibTag = &(s_poTagSeqList[i]);
		}
		//���GroupID�����仯
		else if (prevTag->m_iGroupID != s_poTagSeqList[i].m_iGroupID){
		    oldSibTag->m_poNextSibTagSeq = &(s_poTagSeqList[i]);
		    oldSibTag = oldSibTag->m_poNextSibTagSeq;
		}
		//���GroupIDδ�����仯
		else
		    prevTag->m_poNextTagSeq = &(s_poTagSeqList[i]);

		prevTag = &(s_poTagSeqList[i]);
		i++;		
	}
	qry.close();
    
    nSize = i;
	s_bUploaded = true;
}

void TagSeqMgr::unload()
{
    delete [] s_poTagSeqList;
    
    s_bUploaded = false;
    nSize = 0;
    return;
}
    
const TagSeq *TagSeqMgr::getFirstTagSeq(int iGroupID)
{
    if (nSize==0 || iGroupID<=0)
        return NULL;
    
    TagSeq *p = &(s_poTagSeqList[0]);
    while (p != NULL){
        if (p->m_iGroupID == iGroupID)
            return p;
        p = p->m_poNextSibTagSeq;
    }
    
    return NULL;
}

int TagSeqMgr::getArrtID(int iGroupID, int iTag)
{
    const TagSeq *p = getFirstTagSeq(iGroupID);
    
    if (p==NULL)
        return -1;
    
    while (p!=NULL){
        if (p->m_iTAG == iTag)
            return p->m_iAttrID;
        p = p->m_poNextTagSeq;
    }    
    
    return -1;
}

