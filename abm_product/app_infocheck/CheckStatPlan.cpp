#include "CheckStatPlan.h"

//#define _debug

TOCIQuery * CheckStatPlan::m_pLockQry = NULL;
PlanItem * CheckStatPlan::m_pPlan = NULL;

char ** CheckStatPlan::m_argv;

void CheckStatPlan::startPlan (PlanItem &oPlan, char ** argv)
{
    m_argv = argv;
    
    StatInterface::m_pPlan = &oPlan;
    
    //##������״̬
    bool bret = updateState (oPlan.m_iPlanID, "RUN", "<Message> ��ʼִ��...");
    
    if (!bret) {
        Log::log (0,"�ƻ�ִ��ʧ��[Plan_ID=%d],��ִ�� updateState(RUN) ���ɹ�", oPlan.m_iPlanID);
        return;
    }
    
    while (oPlan.m_iDebugFlag == 1)
        waitDebug (oPlan); //�ȴ�����, ��B_Check_Stat_Plan.DEBUG_FLAG=0 ������ѭ��
    
    try {
        //����һ������ʱ��
        sleep (1);
        
        setNextRunTime (oPlan);
        
        if (oPlan.m_iAppType == 0)
        {
            switch (atoi (oPlan.m_sStatName))
            {
            
            	///////////////////////HSS������Ϣ��300000/////////////////////////////////////////
            	case 800001://2.2.1	���ݽ���ƽ�⣨AuditId=0001��
            	{
            		CheckStatInfo oReceBalance;		
            		oReceBalance.checkDataRecvBalance("0001",50,MIDL_FREG);
            		break;
            	}	
            	
            	case 800002://2.2.2	���ݼ���ƽ�⣨AuditId=0002��
            	{
            		CheckStatInfo oReceBalance;
            		oReceBalance.checkDataLoadBalance("0002",50,MIDL_FREG);
            		break;
            	}
            	
            	case 800003://2.2.3	�������ƽ�⣨AuditId=0003��
            	{
            		CheckStatInfo oReceBalance;
            		oReceBalance.checkDataInputBalance("0003",50,MIDL_FREG);
            		break;
            	}
            	
            	case 800004://2.2.4	���ݷ���ƽ�⣨AuditId=0004��
            	{
            		CheckStatInfo oCheckStatInfo;
            		oCheckStatInfo.checkOutputBalance();
            		break;
            	}
            	
            	case 800005://2.2.5	ʵ���������AuditId=0005��
            	{
            		CheckStatInfo oCheckStatInfo;
            		oCheckStatInfo.checkInstanceUpdate();
            		break;
            	}
            	
            	case 800006://2.2.6	������־��AuditId=0006��
            	{
                	CheckStatInfo oCheckHSSStatInfo;
                	oCheckHSSStatInfo.checkHSSAlertLog();            		
            		break;
            	}            	            	            	            	            	            	
            	
            	case 800007://2.2.7	��Ȩ�쳣��AuditId=0008��
            	{
            		CheckStatInfo oCheckStatInfo;
            		oCheckStatInfo.checkAuthenticationError();
            		break;
            	}   
            	
            	///////////////////////HSS��Ϣ��500000/////////////////////////////////////////
            	//2.3.1 ���սӿڴ�����AuditId=0100��
            	case 502311://	ʡ�ڷǼƷ���ӿڣ�AuditId=0101��	����ָ�꣨AuditId=0101,type=40��
            	{	
            		CheckStatInfo oCheckStatInfo;
            		oCheckStatInfo.checkNoBillingCap();
            		break;
            	}
             	case 502312://		����HSS�ӿڣ�AuditId=0102��	����ָ��	��AuditId=0102,type = 40��
            	{	
            		CheckStatInfo oCheckStatInfo;
            		oCheckStatInfo.checkHssCap();
            		break;
            	}
            	//2.3.2 ͬ���ӿڴ�����AuditId=0200��
             	case 502321://		OFCS�ӿڣ�AuditId=0201��	����ָ�꣨AuditId=0201,type=40��
            	{	
            		CheckStatInfo oCheckStatInfo;
            		oCheckStatInfo.checkOfcsCap();
            		break;
            	}
             	case 502322://			OCS�ӿڣ�AuditId=0202��		����ָ�꣨AuditId=0202,type=40��
            	{	
            		CheckStatInfo oCheckStatInfo;
            		oCheckStatInfo.checkOcsCap();
            		break;
            	}           	      	
            	//2.3.4	ϵͳ״̬��AuditId=0400��
            	case 502341://2.3.4.1	�¼���Ϣ�㣨AuditId=0400,type=60��
            	{
            		CheckStatInfo oCheckHSSEventInfo;
            		oCheckHSSEventInfo.checkOcsCap();
            		break;
            	}	
            	
            	case 502342://2.3.4.2	�澯��Ϣ�㣨AuditId=0400,type=20��
            	{
            		CheckStatInfo oCheckHSSSystemCaution;
            		oCheckHSSSystemCaution.checkHSSSystemCaution();
            		break;
            	}
            	         	
            	///////////////////////һ��Ϊ�Ʒ���Ϣ�㣬��HSS���ô�//////////////////////
              //������Ϣ��������ȡ
               case 100001:  //##2.2.1	�Ʒѽ��գ�AuditId=0001��Type=50��
                {
                	CheckStatInfo oCheckStatInfo(MODULE_CHECK_NORMAL_FILE);
                	oCheckStatInfo.checkNormalFile();
                  break;                	     
                }
               case 100002:  //##2.2.2	�Ʒѽ����쳣�ļ���AuditId=0002��Type=50��
                {
                	CheckStatInfo oCheckStatInfo(MODULE_CHECK_ERR_FILE);
                  oCheckStatInfo.checkErrFile();
                  break;                	     
                }
               case 100003:  //##2.2.3	Ԥ����ƽ�⣨AuditId=0003��Type=50��
                {
                	CheckStatInfo oCheckStatInfo(MODULE_CHECK_FILE_PREP_BALANCE);
                  oCheckStatInfo.checkPrepBalance();
                  break;                	     
                }
               case 100004:  //##2.2.4	����ƽ�⣨AuditId=0004��Type=50��
                {
                	CheckStatInfo oCheckStatInfo(MODULE_CHECK_FILE_PRICING_BALANCE);
                  oCheckStatInfo.checkPricingBalance();
                  break;                	     
                }
               case 100005:  //##2.2.5	���ƽ�⣨AuditId=0005��Type=50��
                {
                	CheckStatInfo oCheckStatInfo(MODULE_CHECK_FILE_INSTORE_BALANCE);
                  oCheckStatInfo.checkInStoreBalance();
                  break;                	     
                }
               case 100006:  //##2.2.6	������־��AuditId=0006��Type=50��
                {
                	CheckStatInfo oCheckStatInfo;
                  oCheckStatInfo.checkTicketLog();
                  break;                	     
                }
               case 100007:  //##2.2.7	IDEP�ļ����䣨AuditId=0007��Type=50��
                {
                	CheckStatInfo oCheckStatInfo;
                  oCheckStatInfo.checkTransLog();
                  break;                	     
                }
               case 100008:  //##2.2.8	����ƽ�⣨AuditId=0008��Type=50��
                {
                  //CheckDailyAcctItemAggr oCheckItem;
                  //oCheckItem.checkDailyAcctItemAggrNew();
                  break;                	     
                }                                                                                                                
                   
               case 100009:  //##2.2.9	������־��AuditId=0009��Type=50��
                {
                 
                  break;                	     
                }
               case 100010:  //##2.2.10	������־��AuditId=0010��Type=50��
                {
                  //CheckDailyAcctItemAggr oCheckItem;
                  //oCheckItem.checkMonthAcctItemAggrNew();                	
                  break;                	     
                }
               case 100011:  //##2.2.11	�߶�ǩ����AuditId=0011��Type=50��
                {
                	//CheckStatInfo oCheckStatInfo;
                 // oCheckStatInfo.checkSignHighFee();
                  break;                	     
                }
               case 100012:  //##2.2.12	ʡ���߶AuditId=0012��Type=50��
                {
                	//CheckStatInfo oCheckStatInfo;
                	//oCheckStatInfo.checkHighFeeServ();
                	/*
                	char sValue1[3+1]={0};
                	ParamDefineMgr::getParam("HIGH_FEE_ALARM", "LOG_OR_DB", sValue1);
                	int iValue=atoi(sValue1);
                	if(iValue >= 2)
                     oCheckStatInfo.checkHighFeeServ();
                  else 
                     oCheckStatInfo.checkHighFeeLog();
                   */
                  break;                	     
                }
               case 100013:  //##2.2.13	������־��AuditId=0013��Type=50��
                {
                	CheckStatInfo oCheckStatInfo;
                	oCheckStatInfo.checkAlertLog();
                	/*
                	char sValue1[3+1]={0};
                	ParamDefineMgr::getParam("INFO_DATA", "PROCESSLOG", sValue1);
                	int iValue=atoi(sValue1);
                	if(iValue == 1)                	
                    oCheckStatInfo.checkAlertLog();
                   else 
                    oCheckStatInfo.checkProcessLog();
                    */
                  break;                	     
                }
               case 100014:  //##2.2.14	�ƷѴ��������ӳ٣�AuditId=0014��Type=50��
                {
                	CheckStatInfo oCheckStatInfo(MODULE_CHECK_DELAY_FILE);
                  oCheckStatInfo.checkDelayFile();
                  break;                	     
                }
               case 100015:  //##2.2.15	ģ�鴦������ָ�꣨AuditId=0015��Type=50��
                {
                	CheckStatInfo oCheckStatInfo;
                	oCheckStatInfo.prepMouldeCapability();
                	oCheckStatInfo.pricingMouldeCapability();
                  oCheckStatInfo.checkMouldeCapabilityKpi();
                  break;                	     
                }
               case 100016:  //##2.2.16	ϵͳ�������ָ�꣨AuditId=0016��Type=50��
                {
                	CheckStatInfo oCheckStatInfo;
                  oCheckStatInfo.checkSystemLossKpi()	;		
                  break;                	     
                }
                
                case 100017:  //##JS_20101215_001��AuditId=0060  Type=50��
                {
                	CheckStatInfo oCheckStatInfo;
                  oCheckStatInfo.checkDayAcctFee();		
                  break;                	     
                }
 
            // ��Ϣ��������ȡ
            // 20000x ����ָ����Ϣ��
                case 200001:  //2.3.1.1	�Ʒѽ�������ָ�꣨AuditId=0100��Type=40��
                {
                	Information oCheckInfor;
                  oCheckInfor.checkGather()	;		
                  break;                	     
                }  
                case 200002:  //2.3.2.1.1	Ԥ��������ָ�꣨AuditId=0201��Type=40��
                {
                	Information oCheckInfor;
                	//oCheckInfor.checkBalance(MODULE_PREP);  //Ԥ�����澯�м���������
                  oCheckInfor.checkPerp()	;		
                  break;                	     
                }   
                case 200003:  //2.3.2.2.1	��������ָ�꣨AuditId=0202��Type=40��
                {
                	Information oCheckInfor;
                	//oCheckInfor.checkBalance(MODULE_PRICING) ;//���۸澯�м���������  
                  oCheckInfor.checkPricing()	;		
                  break;                	     
                }   
                case 200004:  //2.3.2.3.1	�������ָ�꣨AuditId=0203��Type=40��
                {
                	Information oCheckInfor;
                	//oCheckInfor.checkBalance(MODULE_INSTORE) ;//���澯�м���������
                  oCheckInfor.checkIndb()	;		
                  break;                	     
                }   
                case 200005:  //2.3.3.1.1	��������ָ�꣨AuditId=0301��Type=40��
                {
	                CheckStatInfo oCheckAddItem ;
                  oCheckAddItem.checkAcctCap("0301",40,MIDL_FREG)	;
                  break;                	     
                }
                case 200006:  //2.3.3.2.1	��������ָ�꣨AuditId=0302��Type=40��
                {
                	CheckStatInfo oCheckAddItem ;
                  oCheckAddItem.checkAddItem()	;		
                  break;                	     
                }  
                case 200007:  //2.3.3.4.1	��������ָ�꣨AuditId=0304��Type=40��
                {
                	CheckStatInfo oCheckMonthCap ;
                  oCheckMonthCap.checkMonthCap()	;		
                  break;                	     
                }     
                 
                case 200008:  //2.3.4.1.1	��������ָ�꣨AuditId=0401��Type=40��
                {
	                CheckStatInfo oCheckAddItem ;
                  oCheckAddItem.checkAcctCap("0401",40,MIDL_FREG)	;
                  break;                	     
                }
                case 200009:  //2.3.4.2.1	��ֵ������ָ�꣨AuditId=0402��Type=40��
                {
	                CheckStatInfo oCheckAddItem ;
                  oCheckAddItem.checkAcctCap("0402",40,MIDL_FREG)	;
                  break;                	     
                }      
                 case 200010:  //.3.3.5.1 Ƿ����������ָ�꣨AuditId=0305��Type=40��
                {
                CheckStatInfo oCheckAddItem ;
                  oCheckAddItem.checkAcctCap("0305",40,HIGH_FREG) ;
                  break;                      
                } 
                                                                                          
               //20001x  �澯ָ����Ϣ��
               case 200012:  //2.3.2.1.2       ?
                {
                  Information oCheckInfor;
                  //oCheckInfor.checkBalance(MODULE_PREP);  //?���|����???��?D??��y?Y����3��
                  oCheckInfor.checkPrepAlarm();
                  oCheckInfor.writePrepAlarm()  ;
                  break;
                }
                case 200013:  //2.3.2.2.2       
                {
                  Information oCheckInfor;
                  //oCheckInfor.checkBalance(MODULE_PRICING) ;//?��?????��?D??��y?Y����3��
                  oCheckInfor.checkPricingAlarm();
                  oCheckInfor.writePricingAlarm()       ;
                  break;
                }
                case 200014:  //2.3.2.3.2      
                {
                  Information oCheckInfor;
                  //oCheckInfor.checkBalance(MODULE_INSTORE) ;//��??a???��?D??��y?Y����3��
                  oCheckInfor.checkIndbAlarm()    ;
                  oCheckInfor.writeIndbAlarm()  ;
                  break;
                }        
                
                case 200015:  //2.3.3.2.2	���ʸ澯ָ�꣨AuditId=0302��Type=20��
                {
                	CheckStatInfo oCheckInfor;
                  oCheckInfor.checkAddItemCaution()	;		
                  break;                	     
                }         
                        
                case 200020:  //2.3.3.5.2 Ƿ������澯ָ�꣨AuditId=0305��Type=20
                {
                CheckStatInfo oCheckAddItem ;
                  oCheckAddItem.checkAcctCaution("0305",20,HIGH_FREG) ;
                  break;                      
                } 
                /* ���Ĳ����澯��Ϣ��*/
                case 300001:  //���ÿ�ջ�������,������������ʱ����������
                {
                	CheckStatInfo oCheckInfo;
                  oCheckInfo.checkParamBalance()	;		
                  break;                	     
                }  
                
                case 300002:  //���ÿ�����������û���
                {
                	CheckStatInfo oCheckInfo;
                  oCheckInfo.checkRoamServs()	;		
                  break;                	     
                }   
               /*
                case 400001:  //2.3.5.1	�¼���Ϣ�㣨AuditId=0900��Type=60��
                {
                	CheckStatInfo checkEventInfo;
                  checkEventInfo.checkEventInfo()	;		
                  break;                	     
                }    
                */
    
                case 400002:  //2.3.4.3.1	��Ʒ����ָ�꣨AuditId=0403��Type=30��
                {
                	Information checkInfo;
                  checkInfo.checkAllProductInfo()	;		
                  break;                	     
                }   
                case 400003:  //2.3.4.4.1	��Ʒ����ָ�꣨AuditId=0404��Type=30��
                {
                	Information checkInfo;
                	  checkInfo.checkAllProductOfferInfo()	;		
                 	 break;                	     
                }  
                case 400004:  //2.3.4.5.1	���ۼƻ�����ָ�꣨AuditId=0405��Type=30��
                {
                	Information checkInfo;
                  	checkInfo.checkAllPricingPlanInfo()	;		
                  break;                	     
                }  
                
                case 900001:  //2.3.5.1	ϵͳ״̬����ָ�꣨AuditId=0900��Type=40��
                {
	                CheckStatInfo oCheckAddItem ;
                  oCheckAddItem.checkAcctCap("0900",40,MIDL_FREG)	;
                  break;                	     
                }    
                               
                case 900002:  //2.3.5.2	ϵͳ״̬�¼���Ϣ�㣨AuditId=0900��Type=60��
                {
                	CheckStatInfo checkEventInfo;
                  checkEventInfo.checkHSSEventInfo()	;		
                  break;                	     
                }  
                
                case 900003:  //	2.3.5.3	�澯��Ϣ�㣨AuditId=0900��Type=20��
                {
                	CheckStatInfo checkEventInfo;
                  	checkEventInfo.checkSystemCaution()	;		
                  break;                	     
                }
                case 900004 ://    2.3.5.4 VC�ӿ�����ָ�� (AuditId=0203��Type=40)
                {
               		CheckStatInfo oCheckVCinter ;
                	oCheckVCinter.checkVCCable("0203",40,MIDL_FREG)	;
                	break;
                	
            	}
            	case 900005 ://    2.3.5.5 ��ѯ�ӿ����� (AuditId=0300��Type=40)
                {
               		CheckStatInfo oCheckSeekInter ;
                 	oCheckSeekInter.checkSeekInterCap("0300",40,MIDL_FREG);
                 	break;
                	
            	}
            	case 900006 ://    2.3.5.5 ��ѯ�ӿڸ澯 (AuditId=0300��Type=20)
                {
               		CheckStatInfo oCheckSeekInter ;
                 	oCheckSeekInter.checkSeekInterAlarm("0300",20,MIDL_FREG); 
                 	break;            	
            	}
                                                                                                                                                                                                                                                                                                                                                                  
               default:
                {   //## ��δ֪ͳ����ID ����
                   updateState (oPlan.m_iPlanID, "NUL", "<Error> δ֪ͳ����(stat_name),��������", true);
                   return;
                }
            }
        }
        else if (oPlan.m_iAppType == 1) //## PLSQL��ִ��
        {
            DEFINE_QUERY (qry);
            char sql[4096];
            sprintf (sql,"%s", oPlan.m_sStatName);
            qry.setSQL (sql);
            qry.execute();
            qry.commit ();
        }
        else { //## δ֪Ӧ������(app_type)
            updateState (oPlan.m_iPlanID, "NUL", "<Error> δ֪Ӧ������(app_type),��������", true);
            return;
        }
    }
    catch (TOCIException &e)
    {
        DB_LINK->rollback ();
        char sMsg[2048];
        sprintf (sMsg, "<Error> %s \n %s", e.getErrMsg(), e.getErrSrc());
        updateState (oPlan.m_iPlanID, "ERR", sMsg, true);
        return;
    }
    //# TException
    catch (TException &e) 
    {
        DB_LINK->rollback ();
        char sMsg[2048];
        sprintf (sMsg, "<Error> %s ", e.GetErrMsg());
        updateState (oPlan.m_iPlanID, "ERR", sMsg, true);
        return;
    }
    catch (Exception &e) 
    {
        DB_LINK->rollback ();
        char sMsg[2048];
        sprintf (sMsg, "<Error> %s \n", e.descript());
        updateState (oPlan.m_iPlanID, "ERR", sMsg, true);
        return;
    }
    //# Other Exception
    catch (...) 
    {
        DB_LINK->rollback ();
        char sMsg[2048];
        sprintf (sMsg, "<Error> �����쳣�˳��� \n");
        updateState (oPlan.m_iPlanID, "ERR", sMsg, true);
        return;
    }
        
    updateState (oPlan.m_iPlanID, "END", "<Message> ִ�гɹ�!", true);
}


//## ture:��Ҫ����ִ��, false:��ִ��
bool CheckStatPlan::analyzePlan (PlanItem &oPlan)
{
    //##���״̬
    if (!strcmp(oPlan.m_sState,"RUN")) {
        
        //����������״̬
        checkRun (oPlan.m_iPlanID);
        
        return false;
    }
    
    //##������״̬��, ����Ƿ񵽴�����ʱ��
    Date d;
    if (strcmp(d.toString(), oPlan.m_sNextRunTime) >= 0) {
        #ifdef _debug
            Log::log (0,"��ǰʱ��:%s, �ƻ�����ʱ��:%s",d.toString(), oPlan.m_sNextRunTime);
        #endif
        return true;        
    }
    
    return false;
}

bool CheckStatPlan::checkRun (int iPlanID)
{
    int  iLineCnt = 0;
    
    lockPlan ();
    
    DEFINE_QUERY (qry);
    
    qry.setSQL ("select sys_pid, exec_name, state from B_Check_Stat_Plan "
        " where nvl(sys_pid,0)>0 and plan_id = :vPlanID " 
    );
    qry.setParameter ("vPlanID", iPlanID);
    qry.open ();
    
    while (qry.next ()) {
        
        if (strcmp(qry.field("state").asString(),"RUN"))
            break;
        
        char sCommand[100];
        FILE *fp;
        sprintf (sCommand, "ps -p %ld -f|grep %s|awk '{print $2}'", 
                qry.field(0).asLong(), qry.field(1).asString());        
        fp = popen (sCommand, "r");
        if (fp == NULL) {
            Log::log (0, "popen error!");
            break;
        }        
        while (!feof (fp)) {
            char sPID[10];
            if (fscanf (fp, "%s\n", sPID) == EOF)
                break;
            iLineCnt ++;
        }
        pclose (fp);
        
        if (iPlanID == 0)
            break;
        
        if (iLineCnt == 0) { //���̲�����
            Log::log (0,"�ӽ���(pid=%ld) %s ������",
                    qry.field(0).asLong(), qry.field(1).asString());
            updateState (iPlanID, "ERR", "<Alarm> �����̼�⵽�ӽ���δ�������˳�");
        }
        
        break;
    }
    unlockPlan ();
    qry.close();
    
    if (iLineCnt)
        return true;
    else
        return false;
}

void CheckStatPlan::lockPlan ()
{
    if (m_pLockQry) 
        delete m_pLockQry;
    m_pLockQry = new TOCIQuery (DB_LINK);
    
    m_pLockQry->setSQL ("Lock table B_Check_Stat_Plan in exclusive mode");
    m_pLockQry->execute ();
}

void CheckStatPlan::unlockPlan ()
{
    if (!m_pLockQry) {
        DB_LINK->commit();
        return;
    }
    
    m_pLockQry->commit();
    
    delete m_pLockQry;
    m_pLockQry = NULL;
}

void CheckStatPlan::openPlan ()
{
    DEFINE_QUERY (qry);
    
    qry.setSQL (" SELECT "
        "  Plan_id,"
        "  app_type,"
        "  stat_name,"
        "  interval_type,"
        "  time_interval,"
        "  nvl(next_run_time, sysdate) next_run_time,"
        "  nvl(run_parameter,'') run_parameter, "
        "  nvl(state,'END') state,"
        "  nvl(debug_flag,0) debug_flag, "
        "  nvl(data_time_offset, 0) offset, "
        "  file_directory "
        " From B_Check_Stat_Plan"
        " where open_state=1 and nvl(state,'END')<>'NUL' and Plan_id>0 "
        " order by plan_id desc"
    );
    
    qry.open();

    if (m_pPlan)
        delete m_pPlan;
    
    while (qry.next ())
    {
        PlanItem *p = new PlanItem ();
        
        p->m_iPlanID = qry.field("Plan_id").asInteger();
        p->m_iAppType = qry.field("app_type").asInteger();
        strcpy (p->m_sStatName, qry.field("stat_name").asString());
        p->m_iIntervalType = qry.field("interval_type").asInteger();
        strcpy (p->m_sTimeInterval, qry.field("time_interval").asString());
        strcpy (p->m_sNextRunTime, qry.field("next_run_time").asString());
        strcpy (p->m_sParameter, qry.field("run_parameter").asString());
        strcpy (p->m_sState, qry.field("state").asString());
        p->m_iDebugFlag = qry.field("debug_flag").asInteger();
        
        p->m_iDataTimeOffset = qry.field("offset").asInteger();
        if (p->m_iDataTimeOffset < 0)
            p->m_iDataTimeOffset = 0;
            
        strncpy(p->m_sDirectory, qry.field("file_directory").asString(), 1000);
        
        parsePlan (p);
        
        p->m_pNext = m_pPlan;
        m_pPlan = p;
    }
    
    qry.close ();
}

void CheckStatPlan::parsePlan (PlanItem *p)
{
    Date d;
    d.getTimeString (p->m_sDataBeginTime);
    d.getTimeString (p->m_sDataEndTime);    
    
   // p->m_iDataType = 0; //Ĭ�� ��ȡ��̬ˢ��ָ��
     p->m_iDataType = 1 ;
    
    if (p->m_iDataTimeOffset>0/*����ƫ��ʱ��*/  
        && p->m_iIntervalType==1/*ʱ��������*/) 
    {
        Date d (p->m_sNextRunTime);
        
        d.addSec (0 - p->m_iDataTimeOffset);
        strcpy (p->m_sDataEndTime, d.toString());
        
        d.addSec (0 - atoi(p->m_sTimeInterval));
        strcpy (p->m_sDataBeginTime, d.toString()); 
        
        p->m_iDataType = 1; //�ж�Ϊ����ָ��
    }
    else if (p->m_iDataTimeOffset == 0 //��Զ�̬ˢ��ָ��
        && p->m_iIntervalType==1)
    {
        strcpy (p->m_sDataEndTime, p->m_sNextRunTime);
        Date d (p->m_sNextRunTime);
        d.addSec (0 - atoi(p->m_sTimeInterval));
        strcpy (p->m_sDataBeginTime, d.toString());
    }
    else if (p->m_iIntervalType==2)
    {
        strcpy (p->m_sDataEndTime, p->m_sNextRunTime);
        Date d (p->m_sNextRunTime);
    	d.addMonth(-1);
        strcpy (p->m_sDataBeginTime, d.toString());   
        p->m_iDataType = 2; //ÿ�¹̶�ֵ 	
    }	
    
}

bool CheckStatPlan::getPlan (PlanItem &oPlan)
{
    if (!m_pPlan)
        return false;
    
    PlanItem *p = m_pPlan;
    m_pPlan = m_pPlan->m_pNext;
    
    p->m_pNext = NULL;
    oPlan = *p;
    
    delete p;

    return true;
}

void CheckStatPlan::closePlan ()
{
    if (m_pPlan)
        delete m_pPlan;
}

void CheckStatPlan::setNextRunTime (PlanItem &oPlan)
{
    Date d (oPlan.m_sNextRunTime);
    Date dNow;
    char sDate[15];
    char sMsg[1024] = {0};
    
    if (oPlan.m_iIntervalType == 1) //ʱ����(��)
    {
        int iAddSecs = atoi (oPlan.m_sTimeInterval);
        if (iAddSecs <= 0) {
            strcpy (sDate, "30000101000000");
        }
        else {            
            d.addSec (iAddSecs);
            
            if (oPlan.m_iDataType == 0) {
                while (strcmp(d.toString(), dNow.toString()) <= 0)
                    d.addSec (iAddSecs);
                
                //������̬ˢ��ָ�������ʱ��
                Date dData (d.toString());
                dData.addSec (0 - atoi(oPlan.m_sTimeInterval));
                strcpy (oPlan.m_sDataEndTime, dData.toString());
                dData.addSec (0 - atoi(oPlan.m_sTimeInterval));
                strcpy (oPlan.m_sDataBeginTime, dData.toString());
            }
            
            strcpy (sDate, d.toString());
        }
    }
    else if (oPlan.m_iIntervalType == 2) //ÿ���¹̶�ʱ��
    {
        strcpy (sDate, oPlan.m_sNextRunTime);
        while (strcmp(sDate, dNow.toString()) <= 0)
            getNextMonth (sDate, oPlan.m_sTimeInterval);
    }
    else {
        //##�쳣, �������
        strcpy (sDate, "30000101000000");
        strcpy (sMsg, "interval_type �ֶ�ֵ�Ƿ�, �����޸�");
    }
    
    //update table    
    DEFINE_QUERY (qry);
    if (sMsg[0])
    {
        qry.setSQL ("update B_Check_Stat_Plan "
            " set next_run_time = to_date(:vDate,'yyyymmddhh24miss'),"
            " Message = :vMsg "
            " where plan_id = :vPlanID "
        );
        qry.setParameter ("vMsg", sMsg);
    }
    else {
        qry.setSQL ("update B_Check_Stat_Plan "
            " set next_run_time = to_date(:vDate,'yyyymmddhh24miss')"
            " where plan_id = :vPlanID "
        );
    }
    qry.setParameter ("vPlanID", oPlan.m_iPlanID);
    qry.setParameter ("vDate", sDate);
    qry.execute();
    qry.close();
    
    if (sMsg[0]) {
        qry.setSQL ("Insert into b_check_stat_log ( "
                " log_id, plan_id, msg_time, message ) "
                " values (:vLogID, :vPlanID, sysdate, '[RUN] '||:vMsg )"
        );
        qry.setParameter ("vLogID", getNextStatLogID());
        qry.setParameter ("vPlanID", oPlan.m_iPlanID);
        qry.setParameter ("vMsg", sMsg);
        qry.execute();
    }
    qry.commit ();
    
    #ifdef _debug
        Log::log (0, "������һ������ʱ��:%s", sDate);
    #endif
}

void CheckStatPlan::getNextMonth (char sDate[], const char sTimeInterval[])
{
    char sTemp[15];
    
    DEFINE_QUERY (qry);
    qry.setSQL ("select to_char(Add_months(to_date(:vDate,'YYYYMMDDHH24MISS'),1),'YYYYMMDDHH24MISS') from dual");
    qry.setParameter ("vDate", sDate);
    qry.open();
    qry.next();
    strcpy (sDate, qry.field(0).asString());     
    
    strcpy (sTemp, sDate);
    strncpy(sTemp+6, sTimeInterval, 8);
    sTemp[14] = 0;
    
    try {
        DEFINE_QUERY (qry);
        qry.setSQL ("Select to_char(to_date(:vDate,'YYYYMMDDHH24MISS'),'YYYYMMDDHH24MISS') from dual");
        qry.setParameter ("vDate", sTemp);
        qry.open();
        qry.next();
        strcpy (sDate, qry.field(0).asString()); //ʹ���������ʱ��
    }
    catch (TOCIException &e) {
        return;
    }
}
    

void CheckStatPlan::waitDebug (PlanItem &oPlan)
{
    DEFINE_QUERY (qry);
    
    static bool bFirst(true);
    
    if (bFirst) {
        qry.setSQL ("update B_Check_Stat_Plan set message = :vMsg where plan_id = :vPlanID");
        qry.setParameter ("vMsg", "[RUN] �ȴ�����...");
        qry.setParameter ("vPlanID", oPlan.m_iPlanID);
        qry.execute();
        qry.commit();
        bFirst = false;
    }
    
    qry.close();
    qry.setSQL ("select debug_flag from B_Check_Stat_Plan where plan_id=:vPlanID");
    qry.setParameter ("vPlanID", oPlan.m_iPlanID);
    qry.open();
    if (qry.next())
        oPlan.m_iDebugFlag = qry.field(0).asInteger();
    
    sleep (3);
}


bool CheckStatPlan::updateState (const int iPlanID, const char *sState,
            const char *sMsg, const bool bLock)
{
    int iFailedAdd = 0, iSucessAdd=0;
    
    char sExeName[256], sMessage[1024];
    strcpy (sExeName, m_argv[0]);
    trimExeName (sExeName);
    
    sprintf (sMessage, "[%s] %s", sState, sMsg);
    
    if (!strcmp(sState,"ERR"))      //ִ��ʧ��
        iFailedAdd ++;
    else if (!strcmp(sState,"END")) //ִ�гɹ�
        iSucessAdd ++;
    
    if (bLock)
        lockPlan ();
    
    try 
    {
        DEFINE_QUERY (qry);
        qry.setSQL ("update B_Check_Stat_Plan "
            " set state = :vState, "
            " failed_times = failed_times + :vFailedAdd, "
            " sucess_times = sucess_times + :vSucessAdd, "
            " state_date = sysdate ,"
            " message = :vMsg "
            " where plan_id = :vPlanID"
        );
        qry.setParameter ("vState",sState);
        qry.setParameter ("vFailedAdd", iFailedAdd);
        qry.setParameter ("vSucessAdd", iSucessAdd);
        qry.setParameter ("vMsg", sMessage);
        qry.setParameter ("vPlanID", iPlanID);
        qry.execute ();
        
        if (!strcmp (sState,"RUN")) {
            qry.close();
            qry.setSQL ("update B_Check_Stat_Plan "
                " set sys_pid = :vPid, exec_name = :vExecName "
                " where plan_id = :vPlanID " );
            qry.setParameter ("vPid", getpid());
            qry.setParameter ("vExecName", sExeName);
            qry.setParameter ("vPlanID", iPlanID);
            qry.execute ();
        }
        
        qry.close();
        qry.setSQL ("Insert into b_check_stat_log ( "
                " log_id, plan_id, msg_time, message ) "
                " values (:vLogID, :vPlanID, sysdate, :vMsg )"
        );
        qry.setParameter ("vLogID", getNextStatLogID());
        qry.setParameter ("vMsg", sMessage);
        qry.setParameter ("vPlanID", iPlanID);
        qry.execute ();
        qry.commit ();        
        if (bLock)  unlockPlan ();
        
        if (!strcmp (sState,"ERR")) 
        {
            cout << endl << sMsg <<endl;
            writeErrIntoInterface (iPlanID, sMsg);
        }
        
        return true;
    }
    catch (TOCIException &e) 
    {
        DB_LINK->rollback ();
        if (bLock)  unlockPlan ();
        
        Log::log (0, "%s\n %s", e.getErrMsg(), e.getErrSrc());
        return false;
    }
}

void CheckStatPlan::writeErrIntoInterface (int iPlanID, const char *sMsg)
{
    //MNT_SELF_WARN_01: ����ID; �澯ʱ��; �澯��Ϣ
    StatInterface oWarn ("MNT_SELF_WARN_01");
    
    Date d;
    
    sprintf (oWarn.sDimensions, "%d;%s;%s", iPlanID, d.toString(), sMsg);
    
    //oWarn.insert (true);
}