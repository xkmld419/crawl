include $(MAKE_DEFINES)

##### self define #####
TARGETS= 
ALL_OBJS=ParamDefineMgr.o EventSection.o LongTypeMgr.o RoamTariff.o\
	SHMAccess.o SHMIntHashIndex.o SHMStringTreeIndex.o SHMStringTreeIndexEx.o SHMIntHashIndex_KR.o \
    SHMIntHashIndex_A.o SHMStringTreeIndex_A.o SHMIntHashIndex_AX.o SHMStringTreeIndex_AX.o \
	AcctItemMgr.o OrgMgr.o EventType.o HighFee.o \
	EventTypeMgr.o  LifeCycleMgr.o ZoneMgr.o \
	LocalHead.o LongEventTypeRule.o LocalEventTypeRule.o FormatMethod.o \
	BillingCycle.o BillingCycleAdmin.o BillingCycleBase.o BillingCycleCfg.o BillingCycleMgr.o \
	BillingCycleSchedule.o CheckFunction.o AttrTransMgr.o EventIDMaker.o BaseStationMgr.o \
	VirtualFileMaker.o  StateConvertMgr.o PlanType.o CRMControl.o \
	ProductAcctItem.o MEMControl.o FilterRule.o OfferMgr.o InstTableListMgr.o \
    Environment_DB.o encode.o  IpcKey.o CustomFilter.o ExtEventTypeRule.o \
    FilterByAcctMgr.o SplitTicketCfg.o UsageTypeMgr.o TrunkSegBilling.o \
    PageID_OrgID_Relation.o GroupInstMgr.o SplitMgr.o ThresholdAlarm.o\
    LongEventTypeRuleEx.o Head2OrgIDMgr.o OrgMgrEx.o EdgeRoamMgr.o OrgLongTypeMgr.o EventTypeMgrEx.o \
    EventToolKits.o  DistanceMgr.o RecordCheck.o SpecialTariffMgr.o ConvertCityCodeMgr.o \
    NbrTypeMgr.o  NbrMapMgr.o SpecialNbrStrategyMgr.o ValueMapMgr.o SpInfoSeekMgr.o RegionBilling.o \
    SegmentMgr.o VpnInfoReader.o VpnInfo.o VpnCheckEx.o QueueInDb.o  DBTableCheck.o DataOfferMgr.o MonitorFlow.o \
    PublicAttrMgr.o TicketTlv.o ConvertFunc.o BillingNodeInfoMgr.o LogDB.o LogDBBuf.o

CURENT_LIBS=$(BILL_PUBLIC_LIBS)

##### create curent libs #####
$(CURENT_LIBS): $(ALL_OBJS)
	@echo "=======[ $(CURENT_LIBS) ]======="
	$(AR) $(CURENT_LIBS) $(ALL_OBJS)
	$(RANLIB) $(CURENT_LIBS)

all: $(CURENT_LIBS) $(TARGETS)

##### create executed files #####
#test: test1.o test2.o
#	make -f ./Makefile OBJS="test1.o test2.o" EXE=$@ build_usedb

###############################################################
## 这个脚本放在 $TIBS_HOME/etc/install 下面 
## 这个脚本只能包含在子目录 Makefile 的最后面
## 相当于 make clean
## 请不要再写 make clean ，包含这个脚本就可以了
include $(BILLDIR)/etc/install/Make.Clean
