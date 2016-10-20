/*VER: 1*/ 
#include "StateConvertMgr.h"
#include "Environment.h"
#ifndef PRIVATE_MEMORY
#include "ParamInfoInterface.h"
#endif

HashList<HashList<HashList<int> *> *> * StateConvertMgr::m_poIndex = 0;
char StateConvertMgr::m_poBuf[MAX_STATE_NUM][4];
bool StateConvertMgr::m_bUploaded = false;

#ifdef PRIVATE_MEMORY
char * StateConvertMgr::getBillingState(char *sState, int iProductID, int iOfferID)
{
	int i;		
	HashList<HashList<int> *> * pProductIndex;
	HashList<int> * pOfferIndex;

	if (!m_poIndex->get ((int) sState[2], &pProductIndex)) {
		return sState;
	}

	if (!pProductIndex->get (iProductID, &pOfferIndex)) {
		if (!pProductIndex->get (0L, &pOfferIndex)) {
			return sState;
		}
	} 

	if (!pOfferIndex->get (iOfferID, &i)) {
		if (!pOfferIndex->get (0L, &i)) {
			return sState;
		}
	}

	return m_poBuf[i];
}
#else
char * StateConvertMgr::getBillingState(char *sState, int iProductID, int iOfferID)
{
    unsigned int iIdx = 0;
    StateConvert *pStateConvert = 0;
    StateConvert *pTemp = 0;
    StateConvert *pHead = G_PPARAMINFO->m_poStateConvertList;
    
    
	if(!(G_PPARAMINFO->m_poStateConvertIndex->get((int)sState[2], &iIdx)))
	{
        return sState;
    }
    else
    {
        pStateConvert = pHead + iIdx;
        while(pStateConvert != pHead)
        {
            if(pStateConvert->m_iProductID != iProductID)//����ȵ�product_id
            {
                pStateConvert = pHead + pStateConvert->m_iNextProdOffset;
            }
			else
				break;//����ͬ��productid
        }
        
        if(pStateConvert == pHead)//û����ͬ��productid
        {
            pStateConvert = pHead + iIdx;
            while(pStateConvert != pHead)
            {
                if(pStateConvert->m_iProductID != 0)//��productid=0�Ľڵ�
                {
                    pStateConvert = pHead + pStateConvert->m_iNextProdOffset;
                }
				else
					return pStateConvert->m_sBillingState; //��product_id=0�ģ�����Ĭ�ϵ�
            }
        }
        if(pStateConvert == pHead)//û��Ĭ��product_id=0��
            return sState;

//��productid��ͬ�ģ��Ҷ�άoffer_id �Ľڵ�
        pTemp = pStateConvert;
        while(pTemp != pHead)
        {
            if(pTemp->m_iOfferID != iOfferID)//����ȵ�offerid
            {
                pTemp = pHead + pTemp->m_iNextOfferOffset;
            }
			else
            	break;//����ͬ��offerid
        }
        if(pTemp == pHead)//û�ҵ���ͬ��offerid������offer_id=0�Ľڵ�
            pTemp = pStateConvert;
        while(pTemp != pHead)
        {
         	if(pTemp->m_iOfferID != 0)
            {
            	pTemp = pHead + pTemp->m_iNextOfferOffset;
            }
			else
				return pTemp->m_sBillingState; //��offerid=0�ģ�����Ĭ�ϵ�
            
        }
        if(pTemp == pHead)//û��offer_id =0 �ġ�û��Ĭ�ϵ�
            return sState;
        else
            return pTemp->m_sBillingState;
    }
}

#endif

void StateConvertMgr::unload()
{
	if (!m_bUploaded) return;

	memset (m_poBuf, 0, sizeof (m_poBuf));

	if (m_poIndex) {
		HashList<HashList<HashList<int> *> *>::Iteration iter=m_poIndex->getIteration ();
		HashList<HashList<int> *> * pHashList;

		while (iter.next (pHashList)) {
			HashList<HashList<int> *>::Iteration iter1 = pHashList->getIteration ();
			HashList<int> * pHashList1;

			while (iter1.next (pHashList1)) {
				delete pHashList1;
			}

			delete pHashList;
		}

		delete m_poIndex;
		m_poIndex = 0;
	}

	m_bUploaded = false;
}

int StateConvertMgr::getOffset(char *sBillingState)
{
	int i = 0;
	
	while (i<MAX_STATE_NUM) {
		if (m_poBuf[i][0] == 0) {
			strcpy (m_poBuf[i], sBillingState);
			return i;
		}

		if (!strcmp (m_poBuf[i], sBillingState))
			return i;

		i++;
	}

	THROW (MBC_StateConvertMgr+2);
}

void StateConvertMgr::load()
{
	DEFINE_QUERY (qry);

	m_poIndex = new HashList<HashList<HashList<int> *> *> (7);
	if (!m_poIndex) THROW (MBC_StateConvertMgr+1);

	HashList<HashList<int> *> * pProductIndex;
	HashList<int> * pOfferIndex;
	
	memset (m_poBuf, 0, sizeof (m_poBuf));

	qry.setSQL ("select nvl(product_id, 0), nvl(product_offer_id, 0), serv_state, billing_state"
			" from b_serv_state_convert");
	qry.open ();

	int iProductID, iOfferID;
	char sServState[4];
	char sBillingState[4];
	int iOffset;

	while (qry.next ()) {
		iProductID = qry.field (0).asInteger ();
		iOfferID = qry.field (1).asInteger ();

		strcpy (sServState, qry.field (2).asString ());
		strcpy (sBillingState, qry.field (3).asString ());

		if (!m_poIndex->get ((int) sServState[2], &pProductIndex)) {
			pProductIndex = new HashList<HashList<int> *> (13);
			if (!pProductIndex) THROW (MBC_StateConvertMgr+1);
			m_poIndex->add ((int) sServState[2], pProductIndex);
		}

		if (!pProductIndex->get (iProductID, &pOfferIndex)) {
			pOfferIndex = new HashList<int> (7);
			if (!pOfferIndex) THROW (MBC_StateConvertMgr+1);
			pProductIndex->add (iProductID, pOfferIndex);
		}

		iOffset = getOffset (sBillingState);
		pOfferIndex->add (iOfferID, iOffset);
	}

	qry.close ();

	m_bUploaded = true;
}

#ifdef PRIVATE_MEMORY
StateConvertMgr::StateConvertMgr()
{
	if (m_bUploaded) return;

	load ();
}
#else
StateConvertMgr::StateConvertMgr()
{
}
#endif

