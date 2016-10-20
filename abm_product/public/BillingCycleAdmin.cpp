/*VER: 1*/ 
#include "BillingCycleAdmin.h"
#include "BillingCycleSchedule.h"
#include "BillingCycle.h"
#include "BillingCycleCfg.h"
#include "Log.h"
#include "Environment.h"

#define NEW_CYCLE 1
#define CHANGE_CYCLE 2

//���� �����ڹ��� 05.11.21~05.12.21~06.01.01 �ϲ��� 05.11.21~06.01.01
#define CYCLE_GDQ 1

//##ModelId=42A3FFE2007E
BillingCycleAdmin::BillingCycleAdmin() :
m_poSchedule (0), m_iCfgCount (0), m_poCfgList (0)
{
	synCfg ();

	if (!m_poCycleData->exist ()) {
		load ();
	}

	makeSchedule ();
}

//##ModelId=42A4000802EF
int BillingCycleAdmin::admin(bool bBool)
{
	#define MAX_SLEEP_TIME	3000

	BillingCycleSchedule * pSchedule;
	int iSleepTime = MAX_SLEEP_TIME;

	while (1) {
		//first, execute schedule
		while (m_poSchedule) {
			if (m_poSchedule->check ()) {
				//execute
				switch (m_poSchedule->m_iOperation) {
				  case NEW_CYCLE:
					newCycle (m_poSchedule->m_iCycleType);
					break;

				  case CHANGE_CYCLE:
					changeCycle (m_poSchedule->m_iCycleType);
					break;


				  default :
					break;
				}

				//delete
				pSchedule = m_poSchedule;
				m_poSchedule = m_poSchedule->m_poNext;
				delete pSchedule;
			} else {
				break;
			}
		}

		//second, make schedule
		makeSchedule ();

		//third, calculate sleep time
		if (m_poSchedule) {
			Date oNow;
			
			iSleepTime = (int) m_poSchedule->m_oDate.diffSec (oNow);
			if (iSleepTime > MAX_SLEEP_TIME) {
				iSleepTime = MAX_SLEEP_TIME;
			} else if (iSleepTime <= 0) {
				//�����������makeSchedule̫��
				continue;
			}
		}

		if (bBool) return iSleepTime;

		Log::log (50, "Sleep %d seconds", iSleepTime);
		sleep (iSleepTime);
	}

	return iSleepTime;

	#undef MAX_SLEEP_TIME
}

//##ModelId=42A4479203BC
void BillingCycleAdmin::load()
{
	/* ��������������Ϣ�������ڴ� */
	DEFINE_QUERY (qry);
	BillingCycle  oCycle;
	unsigned int iOffset;
	char sql[2048];
	int iTemp;

	if (!m_poCycleData->exist ()) 
		m_poCycleData->create (SHM_BILLINGCYCLE_NUM);	//�ڴ������ŵ�����������
	else
		m_poCycleData->reset ();

	if (!m_poTypeIndex->exist ())
		m_poTypeIndex->create (MAX_BILLINGCYCLETYPE_NUM);

	BillingCycle * pCycle = *m_poCycleData;

        strcpy (sql, "select billing_cycle_id,billing_cycle_type_id,"
                        "to_char(cycle_begin_date,'yyyymmddhh24miss') cycle_begin_date,"
                        "to_char(cycle_end_date,'yyyymmddhh24miss') cycle_end_date,"
                        "to_char(due_date,'yyyymmddhh24miss') due_date,"
                        "to_char(block_date,'yyyymmddhh24miss') block_date,"
                        "to_char(state_date,'yyyymmddhh24miss') state_date,"
                        "nvl(last_billing_cycle_id,0) last_billing_cycle_id,"
                        "state from billing_cycle "
                        "order by cycle_begin_date");

        qry.setSQL (sql);
        qry.open ();

        while (qry.next()) {
            memset (&oCycle, 0, sizeof (oCycle));

                oCycle.m_iBillingCycleID = qry.field(0).asInteger ();
                oCycle.m_iBillingCycleType = qry.field(1).asInteger ();
                strcpy (oCycle.m_sState, qry.field(8).asString());
                strcpy (oCycle.m_sStartDate, qry.field(2).asString());
                strcpy (oCycle.m_sEndDate, qry.field(3).asString());
                strcpy (oCycle.m_sStateDate, qry.field(6).asString());
                strcpy (oCycle.m_sDueDate, qry.field(4).asString());
                strcpy (oCycle.m_sBlockDate, qry.field(5).asString());
                oCycle.m_iLastID = qry.field(7).asInteger ();

#ifdef CYCLE_GDQ
            //�ж�05.11.21~05.12.21 �޸ĳ�06.01.01
            if (strcmp(oCycle.m_sStartDate,"20051121000000")==0 
             && strcmp(oCycle.m_sEndDate,"20051221000000")==0 ){
                strcpy(oCycle.m_sEndDate,"20060101000000");
            }

            //�ж�05.12.21~06.01.01 ����
            if (strcmp(oCycle.m_sStartDate,"20051221000000")==0 
             && strcmp(oCycle.m_sEndDate,"20060101000000")==0 ){
                continue;
            }

#endif
		insert (oCycle);
        }

	qry.close ();

	//������ݵ���ȷ��
	for (iTemp=0; iTemp<m_iCfgCount; iTemp++) {
		if (!m_poTypeIndex->get (m_poCfgList[iTemp].m_iCycleType, &iOffset)) {
			 Log::log (0, "û�и��������͵�����:%d", m_poCfgList[iTemp].m_iCycleType);
			 THROW (MBC_BillingCycleAdmin);
		}

        //���ÿһ��type�µ�����ֵ
        do {
            if (pCycle[iOffset].m_sState[2]=='R')//ȡ��һ��R��������������
            {
                m_poTypeIndex->add (m_poCfgList[iTemp].m_iCycleType,iOffset);
                break;
            }
            iOffset =  pCycle[iOffset].m_iPre;
        } while (iOffset);


/*  //ע�Ͳ���Ϊԭ����߼������޸�ΪȡRΪ�����ڵ㣬���������޸ı�״̬2007.04������
		//��鵱ǰ���ڵ������Ƿ���ȷ
		if (pCycle[iOffset].m_sState[2] != 'R') {
			//�����ڴ������״̬
			pCycle[iOffset].m_sState[2] = 'R';
			
			//�������ݿ������״̬
			sprintf (sql, "update billing_cycle set state='10R' where billing_cycle_id=%d",
									 pCycle[iOffset].m_iBillingCycleID);
			qry.setSQL (sql);
			qry.execute ();
			qry.commit ();

			qry.close ();
		}

		//�����һ�����ڵ������Ƿ���ȷ
		if (!pCycle[iOffset].m_iPre) {
			if (pCycle[iOffset].m_iBillingCycleType == DEFAULT_CYCLE_TYPE) continue;

			Log::log (0, "no pre cycle [%d]", pCycle[iOffset].m_iBillingCycleType);
			THROW (MBC_BillingCycleAdmin);
		}

		iOffset =  pCycle[iOffset].m_iPre;

		if (pCycle[iOffset].m_sState[2] == 'R') {
			//�����ڴ��е�״̬
			Date oNow;
			Date oEndDate(pCycle[iOffset].m_sEndDate);

			if (oNow.diffSec (oEndDate) > m_poCfgList[iTemp].m_iDelaySec) {
				pCycle[iOffset].m_sState[2] = 'A';
				
				//�������ݿ������״̬
				sprintf (sql,"update billing_cycle set state='10A' where billing_cycle_id=%d",
									 pCycle[iOffset].m_iBillingCycleID);
				qry.setSQL (sql);
				qry.execute ();
				qry.commit ();

				qry.close ();
			} else {
				BillingCycleSchedule * pSchedule;
				pSchedule = new BillingCycleSchedule ();
				pSchedule->m_iOperation = CHANGE_CYCLE;
				pSchedule->m_iCycleType =  (m_poCfgList[iTemp].m_iCycleType);
				oEndDate.addSec (m_poCfgList[iTemp].m_iDelaySec-1);
				pSchedule->m_oDate = oEndDate;
				insertSchedule (pSchedule);

				oEndDate.addSec (1);

				m_poCfgList[iTemp].m_oLastDate = oEndDate;
			}
		}
*/
	}
}

//##ModelId=42A447A803C8
void BillingCycleAdmin::makeSchedule()
{
	Date oNow;
	int iTemp;
	unsigned int iOffset;
	BillingCycle * pCycle = *m_poCycleData;
	BillingCycleSchedule * pSchedule;
	
	for (iTemp=0; iTemp<m_iCfgCount; iTemp++) {

		if (m_poTypeIndex->get (m_poCfgList[iTemp].m_iCycleType, &iOffset)) {
			Date oEndDate (pCycle[iOffset].m_sEndDate);
			Date oStartDate (pCycle[iOffset].m_sStartDate);

			if (oEndDate > m_poCfgList[iTemp].m_oLastDate) {
				pSchedule = new BillingCycleSchedule ();
				pSchedule->m_iOperation = NEW_CYCLE;
				pSchedule->m_iCycleType = m_poCfgList[iTemp].m_iCycleType;
				oEndDate.addSec (-1);
				pSchedule->m_oDate = oEndDate;
				insertSchedule (pSchedule);

				pSchedule = new BillingCycleSchedule ();
				pSchedule->m_iOperation =CHANGE_CYCLE;
				pSchedule->m_iCycleType = m_poCfgList[iTemp].m_iCycleType;
				oEndDate.addSec (m_poCfgList[iTemp].m_iDelaySec);
				pSchedule->m_oDate = oEndDate;
				insertSchedule (pSchedule);

				oEndDate.addSec (1);

				m_poCfgList[iTemp].m_oLastDate = oEndDate;
			}

		} else {
			Log::log (0, "�����������ݴ���:[%d]", m_poCfgList[iTemp].m_iCycleType);
			//THROW (MBC_BillingCycleAdmin);
		}
	}
}

//##ModelId=42A44AB402F7
int BillingCycleAdmin::insert(BillingCycle &oBillingCycle)
{
	unsigned int iOffset, iTemp;
	int i;

	BillingCycle * pCycle = *m_poCycleData;

	iOffset = m_poCycleData->malloc ();
	if (!iOffset) {
		unsigned int *piTemp;
		unsigned int iTemp;

		if (!m_poTypeIndex->get (oBillingCycle.m_iBillingCycleType, &iOffset)) 
			THROW (MBC_BillingCycleAdmin+9);

		if (!pCycle[iOffset].m_iPre) THROW (MBC_BillingCycleAdmin+9);

		piTemp = &(pCycle[iOffset].m_iPre);
		while (pCycle[*piTemp].m_iPre) {
			piTemp = &(pCycle[*piTemp].m_iPre);
		}
		
		iTemp = *piTemp;
		*piTemp = 0;

		pCycle[iTemp] = oBillingCycle;
		
		pCycle[iTemp].m_iNext = 0;
		pCycle[iTemp].m_iPre = iOffset;

		
		m_poTypeIndex->add (oBillingCycle.m_iBillingCycleType, iTemp);
		
		return iTemp;

	} else {
		pCycle[iOffset] = oBillingCycle;

		if (m_poTypeIndex->get (oBillingCycle.m_iBillingCycleType, &iTemp)) {
			i = strcmp (pCycle[iOffset].m_sStartDate, pCycle[iTemp].m_sStartDate);
			if (i>0) {

				if (!pCycle[iTemp].m_iNext) {
					pCycle[iOffset].m_iPre = iTemp;
					pCycle[iTemp].m_iNext = iOffset;
				} else {
					while (pCycle[iTemp].m_iNext) {
						if (strcmp (pCycle[iOffset].m_sStartDate,
								 pCycle[pCycle[iTemp].m_iNext].m_sStartDate)>0)
							iTemp = pCycle[iTemp].m_iNext;
						else 
							break;
					}
					
					pCycle[iOffset].m_iNext = pCycle[iTemp].m_iNext;
					pCycle[iOffset].m_iPre = iTemp;
					if (pCycle[iTemp].m_iNext) {
						pCycle[pCycle[iTemp].m_iNext].m_iPre = iOffset;
					}
					pCycle[iTemp].m_iNext = iOffset;
				}

			} else {

				if (!pCycle[iTemp].m_iPre) {
					pCycle[iOffset].m_iNext = iTemp;
					pCycle[iTemp].m_iPre = iOffset;
				} else {
					while (pCycle[iTemp].m_iPre) {
						if (strcmp (pCycle[iOffset].m_sStartDate,
								 pCycle[pCycle[iTemp].m_iPre].m_sStartDate)<0)
							iTemp = pCycle[iTemp].m_iPre;
						else 
							break;
					}
					
					pCycle[iOffset].m_iPre = pCycle[iTemp].m_iPre;
					pCycle[iOffset].m_iNext = iTemp;
					if (pCycle[iTemp].m_iPre) {
						pCycle[pCycle[iTemp].m_iPre].m_iNext = iOffset;
					}
					pCycle[iTemp].m_iPre = iOffset;
				}

			}
		} 

		//�����ص��ǵ�ǰ����������
		m_poTypeIndex->add (oBillingCycle.m_iBillingCycleType, iOffset);
		
	}

	return iOffset;
}

//##ModelId=42A50F520296
void BillingCycleAdmin::synCfg()
{
	/* ���¼���������ڴ�	*/

	DEFINE_QUERY (qry);
	int iTemp;

	if (m_poCfgList) {
		delete [] m_poCfgList;
		m_poCfgList = 0;
	}

	qry.setSQL ("select count(*) from billing_cycle_type");
	qry.open ();
	qry.next ();
	
	m_iCfgCount = qry.field(0).asInteger ();

	qry.close ();

	m_poCfgList = new BillingCycleCfg[m_iCfgCount];
	if (!m_poCfgList) THROW (MBC_BillingCycleAdmin+1);

	qry.setSQL ("select billing_cycle_type_id, cycle_delay_hour"
			" from billing_cycle_type");
	qry.open ();

	iTemp = 0;
	while (qry.next () && (iTemp<m_iCfgCount)) {
		m_poCfgList[iTemp].m_iCycleType = qry.field(0).asInteger ();
		m_poCfgList[iTemp].m_iDelaySec = qry.field(1).asInteger () * 3600;
		iTemp++;
	}

	qry.close ();
}

//##ModelId=42A6A4DC0353
void BillingCycleAdmin::insertSchedule(BillingCycleSchedule *pSchedule)
{
	BillingCycleSchedule ** ppSchedule = &(m_poSchedule);

	while (*ppSchedule) {
		if ((*ppSchedule)->m_oDate < pSchedule->m_oDate) {
			ppSchedule = &((*ppSchedule)->m_poNext);
		} else {
			break;
		}
	}

	pSchedule->m_poNext = *ppSchedule;
	*ppSchedule = pSchedule;
}

//##ModelId=42A6A50D03D6
void BillingCycleAdmin::newCycle(int iCycleType)
{
	DEFINE_QUERY (qry);
	BillingCycle * pCycle = *m_poCycleData;
	char sSql[2048];
	unsigned int iOffset;
	BillingCycle oCycle;

	//first, load the next cycle from database
	if (!m_poTypeIndex->get (iCycleType, &iOffset)) THROW (MBC_BillingCycleAdmin);

	sprintf (sSql, "select billing_cycle_id,billing_cycle_type_id,"
                        "to_char(cycle_begin_date,'yyyymmddhh24miss') cycle_begin_date,"
                        "to_char(cycle_end_date,'yyyymmddhh24miss') cycle_end_date,"
                        "to_char(due_date,'yyyymmddhh24miss') due_date,"
                        "to_char(block_date,'yyyymmddhh24miss') block_date,"
                        "to_char(state_date,'yyyymmddhh24miss') state_date,"
                        "nvl(last_billing_cycle_id,0) last_billing_cycle_id,"
                        "state from billing_cycle "
			"where billing_cycle_type_id=%d"
			" and cycle_begin_date>="
			"to_date('%s','yyyymmddhh24miss') order by cycle_begin_date"
			, iCycleType, pCycle[iOffset].m_sEndDate);
	qry.setSQL (sSql);
	qry.open ();

	if (!qry.next ()) THROW (MBC_BillingCycleAdmin+1);

	memset (&oCycle, 0, sizeof (oCycle));

	oCycle.m_iBillingCycleID = qry.field(0).asInteger ();
	oCycle.m_iBillingCycleType = qry.field(1).asInteger ();
	strcpy (oCycle.m_sState, qry.field(8).asString());
	strcpy (oCycle.m_sStartDate, qry.field(2).asString());
	strcpy (oCycle.m_sEndDate, qry.field(3).asString());
	strcpy (oCycle.m_sStateDate, qry.field(6).asString());
	strcpy (oCycle.m_sDueDate, qry.field(4).asString());
	strcpy (oCycle.m_sBlockDate, qry.field(5).asString());
	oCycle.m_iLastID = qry.field(7).asInteger ();

	qry.close ();

	iOffset = insert (oCycle);

	//�����ڴ������״̬
	pCycle[iOffset].m_sState[2] = 'R';
	
	//�������ݿ������״̬
	sprintf (sSql, "update billing_cycle set state='10R' where billing_cycle_id=%d",
								 oCycle.m_iBillingCycleID);
	qry.setSQL (sSql);
	qry.execute ();
	qry.commit ();

	qry.close ();
}

//##ModelId=42A6A51502A1
void BillingCycleAdmin::changeCycle(int iCycleType)
{
	DEFINE_QUERY (qry);
	BillingCycle * pCycle = *m_poCycleData;
	char sSql[2048];
	unsigned int iOffset;

	if (!m_poTypeIndex->get (iCycleType, &iOffset)) THROW (MBC_BillingCycleAdmin);

	iOffset = pCycle[iOffset].m_iPre;
	if (!iOffset) THROW (MBC_BillingCycleAdmin);

	//�����ڴ��е�״̬
	pCycle[iOffset].m_sState[2] = 'A';
	
	//�������ݿ������״̬
	sprintf (sSql, "update billing_cycle set state='10A' where billing_cycle_id=%d",
								 pCycle[iOffset].m_iBillingCycleID);
	qry.setSQL (sSql);
	qry.execute ();
	qry.commit ();

	qry.close ();
}
