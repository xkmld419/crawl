/*VER: 1*/ 
/*
	�Ѷ��ۼƻ�������������.
	Ϊ�˽������ҵ��� �˺ź͹̶��绰 ������; 
	��Ϊ������Ʋ�֧�����������ϵ�����������
*/

#include "PlanType.h"
#include "Environment.h"
#ifndef PRIVATE_MEMORY
#include "ParamInfoInterface.h"
#endif 

bool PlanTypeMgr::m_bUploaded = false;
HashList<int> * PlanTypeMgr::m_poIndex = 0;

#ifdef PRIVATE_MEMORY
int PlanTypeMgr::getPlanType(int iPricingPlanID)
{
	int iRet = 0;

	m_poIndex->get (iPricingPlanID, &iRet);

	return iRet;
}
#else
int PlanTypeMgr::getPlanType(int iPricingPlanID)
{
	unsigned int iIdx = 0;
    if(G_PPARAMINFO->m_poOfferSpecAttrIndex->get(iPricingPlanID, &iIdx))
        return (int)iIdx;
    else
        return 0;
}
#endif

#ifdef PRIVATE_MEMORY
PlanTypeMgr::PlanTypeMgr()
{
	if (!m_bUploaded) load();
}
#else
PlanTypeMgr::PlanTypeMgr()
{
}
#endif

void PlanTypeMgr::load()
{
	if (m_bUploaded) unload ();

	DEFINE_QUERY (qry);
	
	m_poIndex = new HashList<int> (53);
	if (!m_poIndex) THROW (MBC_PlanType+1);

	qry.setSQL ("select pricing_plan_id, pricing_type from pricing_plan");
	qry.open ();

	while (qry.next ()) {
		m_poIndex->add (qry.field (0).asInteger (), qry.field (1).asInteger ());
	}

	qry.close ();

	m_bUploaded = true;
}

void PlanTypeMgr::unload()
{
	delete m_poIndex;
	m_poIndex = 0;
	m_bUploaded = false;
}

