/*VER: 2*/ 
#include "CheckFunction.h"
#include "Head2OrgIDMgr.h"
#include "OrgMgrEx.h"
#include "EdgeRoamMgr.h"
#include "EventToolKits.h"
#include "FormatMethod.h"
#include "EventTypeMgrEx.h"
#include "Head2OrgIDMgr.h"
#include "Date.h"
#include "ZoneMgr.h"
//#include "../app_prep/Tap3FileInfo.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "ParamInfoInterface.h"

//CheckFunction Class
CheckFunction::CheckFunction()
{
}

CheckFunction::~CheckFunction()
{
}

///* Public Function */
bool CheckFunction::doFunction(int iFuncID, const EventSection *poEventSection, 
		int iTAG, const CheckArgument *poFirstArgument, string *psResult, 
		string *psInput)
{
	#ifndef PRIVATE_MEMORY 
	CheckArgument *pArgNull = &(G_PPARAMINFO->m_poCheckArgumentList[0]);
	CheckArgument *pArgument = (G_PPARAMINFO->m_poCheckArgumentList);
	#endif
    int iAttrID = -1;
    string sAttrValue;
    char sTime[15]={0};

    char m_sFileHead[10]={0};
	//   char *p;
    //char *p = strrchr(poEventSection->m_oEventExt.m_sFileName,'_');
    //strncpy(m_sFileHead,poEventSection->m_oEventExt.m_sFileName,p-poEventSection->m_oEventExt.m_sFileName);

    //��psInput��Ϊ��ʱ�������˴ε�������Ϊ�ܵ������κ�����ʹ��
    if (psInput != NULL){
        sAttrValue = *psInput;
    }else{
	    EventToolKits toolKits;
	    iAttrID = TagSeqMgr::getArrtID(poEventSection->m_iGroupID, iTAG);
	    sAttrValue = toolKits.get_param(poEventSection, iAttrID);  
	    /*if (iTAG>=100)
	    	sAttrValue = toolKits.get_param(poEventSection, iAttrID);   
	   	else
	    {
	   	 sAttrValue =  toolKits.get_param (&poEventSection, 
	   		 Tap3FileInfo::getEventAttrID (m_sFileHead, C_Head_Block, C_record_type_Tag)).c_str();
		 if (sTValue[0] = 0)
			 sAttrValue = toolKits.get_param (&poEventSection, 
			 Tap3FileInfo::getEventAttrID (m_sFileHead, C_Tail_Block, C_record_type_Tag)).c_str();
	    }*/
    }
	
    long lValue = atoi(sAttrValue.c_str());
    const char *sValue = sAttrValue.c_str();
        
    const char *pTime  = poEventSection->m_sStartDate;
    //char sTmpName[250]={0};
    //strcpy(sTmpName,poEventSection->m_oEventExt.m_sFileName);
    const char *p = strrchr(poEventSection->m_oEventExt.m_sFileName,'/');
    char sFileName[64]={0};
    if (!p)
	strcpy(sFileName,poEventSection->m_oEventExt.m_sFileName);
    else
	   strcpy(sFileName,p+1);
		
	strcpy(sTime,pTime);
	const CheckArgument *pArgu = poFirstArgument;
        
    int  iTmp, iTmp2;
    float fTmp;
    char sTmp[32];
    char cTmp;
    const char *p1;
    const char *p2;
    bool  bRet;
    memset(sTmp, 0, sizeof(sTmp));
    Date td; //tmp date
    
    
	switch (iFuncID){
	case CK_GET_ATTR:
	    if (psResult!=NULL)
	        *psResult = sAttrValue;
	    return true;
	case CK_DO_FUNC:
	#ifdef PRIVATE_MEMORY 
	    if (pArgu==NULL || pArgu->m_sValue[0]==0 
	        || pArgu->m_poNextSibArgument==NULL
	        || pArgu->m_poNextSibArgument->m_sValue[0]==0)
	        return false;
	    if (atoi(pArgu->m_sValue)==CK_DO_FUNC)
	        return false;
	    iTmp = atoi(pArgu->m_poNextSibArgument->m_sValue);
	    return doFunction(atoi(pArgu->m_sValue), poEventSection, iTmp, 
	        pArgu->m_poNextSibArgument->m_poNextSibArgument, psResult); 
	#else
		if (pArgu==NULL || pArgu== pArgNull || pArgu->m_sValue[0]==0 
	        || pArgu->m_iNextSibArgument==0
	        || G_PPARAMINFO->m_poCheckArgumentList[pArgu->m_iNextSibArgument].m_sValue[0]==0)
	        return false;
	    if (atoi(pArgu->m_sValue)==CK_DO_FUNC)
	        return false;
	    iTmp = atoi(G_PPARAMINFO->m_poCheckArgumentList[pArgu->m_iNextSibArgument].m_sValue);
	    return doFunction(atoi(pArgu->m_sValue), poEventSection, iTmp, 
	        &(pArgument[pArgument[pArgu->m_iNextSibArgument].m_iNextSibArgument]), psResult); 
	#endif
	case CK_CMP_TWO_FUNC:
	    return cmpTwoFunc(poEventSection, pArgu);
	case CK_IS_DEC:	       
	    return isNumber(sValue, pArgu, "DEC");
	case CK_IS_OCT:	       
	    return isNumber(sValue, pArgu, "OCT");
	case CK_IS_HEX:	       
	    return isNumber(sValue, pArgu, "HEX");
	case CK_BEGIN_WITH:
	    if( beginWith(sValue, pArgu, sTmp)){
	        if (psResult!=NULL)
	            *psResult = string(sTmp, strlen(sTmp));
	        return true;
	    }
	    return false;
	case CK_GET_ORG:
	    if (strlen(sTime) == 0)
		strcpy(sTime,td.toString());
	    if (!getOrgID(sValue, iTmp, sTime, pArgu))
	        return false;
	     sprintf(sTmp, "%d", iTmp);
	     if (psResult!=NULL)
	        *psResult = string(sTmp, strlen(sTmp));
	    return true;
	case CK_AREA_CODE:
	    if (strlen(sTime) == 0)
		strcpy(sTime,td.toString());
	    if (!getAreaCode(sValue, sTmp, sizeof(sTmp), sTime, pArgu))
	        return false;
	    if (psResult!=NULL)
	        *psResult = string(sTmp, strlen(sTmp));
	    return true;
	case CK_IS_AREACODE:
	    if (strlen(sTime) == 0)
		strcpy(sTime,td.toString());
	    if(!isAreaCode(sValue, sTmp, sizeof(sTmp), sTime, pArgu))
	        return false;
	    if (psResult!=NULL)
	        *psResult = string(sTmp, strlen(sTmp));
	    return true;
	case CK_PROVIN_CODE:
	    if (strlen(sTime) == 0)
		strcpy(sTime,td.toString());
	    p1 = strrchr(poEventSection->m_oEventExt.m_sFileName, '.')-3;
	    if (!getAreaCode(sValue, sTmp, sizeof(sTmp), sTime, pArgu))
	        return false;
	    iTmp = strlen(sTmp);
	    p2 = &(sTmp[iTmp-3]);//ָ��������λ
	    if (p1[0]==p2[0] && p1[1]==p2[1] && p1[2]==p2[2]){
	        if (psResult!=NULL)
	            *psResult = string(sTmp, strlen(sTmp));
	        return true;
	    }
	    return false;
    case CK_DATE_FORMAT:
	if (strlen(sValue) == 0)
		return false;
		#ifdef PRIVATE_MEMORY 
        if (pArgu==NULL || pArgu->m_sValue[0]==0){
		#else
		if (pArgu==NULL || pArgu==pArgNull || pArgu->m_sValue[0]==0){
		#endif
            if (!td.parse(sValue, NULL))
                return false;    
            string s1 = string(td.toString()).substr(0, 8);
            string s2 = string(td.toString()).substr(8);
            		
	     if (td.checkDate(s1.c_str())!=-1 && td.checkTime( s2.c_str())!=-1)
                return true;            
            else
                return false;
        }
        else{
            if (!td.parse(sValue, pArgu->m_sValue))
                return false;
            string s1,s2;
            if (strlen(sValue) == 14 || strlen(sValue) == 8)  //���������յ����
	     {
            		s1 = string(td.toString()).substr(0, 8);
            		s2 = string(td.toString()).substr(8);
            		if (td.checkDate(s1.c_str())!=-1 && td.checkTime( s2.c_str())!=-1)
                		return true;
            		else 
            			return false;
            	}
            else   //����ʱ�������ʽ
            	{
			s1 = string(td.toString("HHMISS"));
			if (td.checkTime( s1.c_str())!=-1)
                		return true;
			else
				return false;
            	}
        }
    case CK_BEYOND_DATE:
        return isBeyondDate(sValue, pArgu,sFileName);
    case CK_LARGER_THEN:
        if (pArgu==NULL || pArgu->m_sValue[0]==0)
            iTmp = 0;
        else if (!isNumber(pArgu->m_sValue, NULL, "DEC"))
            return false;
        else
            iTmp = atoi(pArgu->m_sValue);
        return lValue>iTmp;
    case CK_LESS_THEN:
		#ifdef PRIVATE_MEMORY
        if (pArgu==NULL || pArgu->m_sValue[0]==0)
		#else
		if (pArgu==NULL || pArgu==pArgNull || pArgu->m_sValue[0]==0)
		#endif
            iTmp = 0;
        else if (!isNumber(pArgu->m_sValue, NULL, "DEC"))
            return false;
        else
            iTmp = atoi(pArgu->m_sValue);
        return lValue<iTmp;
    case CK_EQUAL:
		#ifdef PRIVATE_MEMORY
        if (pArgu==NULL || pArgu->m_sValue[0]==0)
		#else
		if (pArgu==NULL || pArgu==pArgNull || pArgu->m_sValue[0]==0)
		#endif
            iTmp = 0;
        else if (!isNumber(pArgu->m_sValue, NULL, "DEC"))
            return false;
        else
            iTmp = atoi(pArgu->m_sValue);
	if (strlen(sValue) == 0) return false;
	
        return lValue==iTmp;
    case CK_GET_ORG_2:
        if (!getOrgID(poEventSection, iTmp, pArgu, NULL))
            return false;
        sprintf(sTmp, "%d", iTmp);
        if (psResult!=NULL)
	        *psResult = string(sTmp, strlen(sTmp));
	    return true;
    case CK_LENGTH:
		#ifdef PRIVATE_MEMORY
        if (pArgu==NULL || pArgu->m_sValue[0]==0)
            iTmp = 0;
        //else if (!isNumber(pArgu->m_sValue, NULL, "DEC"))
        //    return false;
        else
            iTmp = atoi(pArgu->m_sValue);
        if (pArgu->m_poNextSibArgument == NULL || strcmp(pArgu->m_poNextSibArgument->m_sValue,"=") == 0) 
        	return strlen(sValue)==iTmp;
        else if (strcmp(pArgu->m_poNextSibArgument->m_sValue,"<") == 0)
        		return strlen(sValue)<iTmp;
        else if (strcmp(pArgu->m_poNextSibArgument->m_sValue,">") == 0)
        		return strlen(sValue)>iTmp;
        else 
        	return false;
		#else
		if (pArgu==NULL || pArgu==pArgNull || pArgu->m_sValue[0]==0)
			iTmp = 0;
        else
            iTmp = atoi(pArgu->m_sValue);
        if (pArgu->m_iNextSibArgument == 0 || strcmp(pArgument[pArgu->m_iNextSibArgument].m_sValue,"=") == 0) 
        	return strlen(sValue)==iTmp;
        else if (strcmp(pArgument[pArgu->m_iNextSibArgument].m_sValue,"<") == 0)
        		return strlen(sValue)<iTmp;
        else if (strcmp(pArgument[pArgu->m_iNextSibArgument].m_sValue,">") == 0)
        		return strlen(sValue)>iTmp;
        else 
        	return false;
		#endif
    case CK_MUX:
		#ifdef PRIVATE_MEMORY
        if (pArgu==NULL || pArgu->m_sValue[0]==0)
		#else
		if (pArgu==NULL || pArgu==pArgNull || pArgu->m_sValue[0]==0)
		#endif
            fTmp = 0;
        else
            fTmp = atof(pArgu->m_sValue);
        //����ȡ��    
        iTmp = ceil(fTmp*lValue);
        sprintf(sTmp, "%d", iTmp);
        if (psResult!=NULL)
	        *psResult = string(sTmp, strlen(sTmp));
	    return true;
	case CK_DIV:
		#ifdef PRIVATE_MEMORY
	    if (pArgu==NULL || pArgu->m_sValue[0]==0)
		#else
		if (pArgu==NULL || pArgu==pArgNull || pArgu->m_sValue[0]==0)
		#endif
            fTmp = 1;
        else
            fTmp = atof(pArgu->m_sValue);
        if (fTmp == 0.0) return false;
        //����ȡ��
        iTmp = (int)((lValue+fTmp-1)/fTmp);
        sprintf(sTmp, "%d", iTmp);
        if (psResult!=NULL)
	        *psResult = string(sTmp, strlen(sTmp));
	    return true;
    case CK_GET_CARRIER:
        if (!formatCarrier(poEventSection, iTmp))
            return false;
        sprintf(sTmp, "%d", iTmp);
        if (psResult!=NULL)
            *psResult = string(sTmp, strlen(sTmp));
        return true;
    case CK_FORMAT_CARRIER:
        bRet = formatCarrier(poEventSection, iTmp, sTmp);
        if (psResult!=NULL)
            *psResult = string(sTmp, strlen(sTmp));
        return bRet;
    case CK_FORMAT_AREACODE:
        bRet = formatAreaCode(poEventSection, sTmp);
        if (psResult!=NULL)
            *psResult = string(sTmp, strlen(sTmp));
        return bRet;
    case CK_IS_MSC:
        if (!isMSC(sValue, pArgu, sTmp))
            return false;
        if (psResult!=NULL)
	        *psResult = string(sTmp, strlen(sTmp));
	    return true;
	case CK_GET_PARTNER:
	    if (!(bRet=getPartnerID(sValue, pTime, iTmp)))
            iTmp = -1;
        sprintf(sTmp, "%d", iTmp);
        if (psResult!=NULL)
            *psResult = string(sTmp, strlen(sTmp));
        return bRet;
    case CK_PIPE:
        bRet = pipe(poEventSection, pArgu, sTmp);
        if (psResult!=NULL)
            *psResult = string(sTmp, strlen(sTmp));
        return bRet;
    case CK_IS_INNER:
        return isInner(poEventSection, pTime);
    case CK_FORMAT_0086:
        format0086(poEventSection->m_sOrgCalledNBR, sTmp);
        if (psResult!=NULL)
            *psResult = string(sTmp, strlen(sTmp));
        return true;
    case CK_GET_LONG_TYPE:
        if (!(bRet=getLongTypeAndRoamID(poEventSection, iTmp, iTmp2)))
            iTmp = -1;
        sprintf(sTmp, "%d", iTmp);
        if (psResult!=NULL)
            *psResult = string(sTmp, strlen(sTmp));
        return bRet;        
    case CK_GET_ROAM_TYPE:
        if (!(bRet=getLongTypeAndRoamID(poEventSection, iTmp, iTmp2)))
            iTmp2 = -1;
        sprintf(sTmp, "%d", iTmp2);
        if (psResult!=NULL)
            *psResult = string(sTmp, strlen(sTmp));
        return bRet;
    case CK_CMP_CHAROFSTRING:
    	if (!cmpCharofString(sValue, pArgu, cTmp))
    		return false;
    	return true;
    case CK_SUBSTR:
    	 if (!cmpSubStr(sValue,pArgu))
    	 	return false;
    	 return true;
    case CK_ZONE:	
	if (!check_zone(sValue,pArgu,0))
		return false;
	return true;
    case CK_ZONE_MAX:	
	if (!check_zone(sValue,pArgu,1))
		return false;
	return true;
	default: 
		return false;  //û�ж�Ӧ�ĺ�������Ĭ�ϲ�ͨ��	
	} /* End of switch (iFuncID) */

	return true;
}

#ifdef PRIVATE_MEMORY
bool CheckFunction::isNumber(const char* str, 
    const CheckArgument *poTable, const char *pFormat)
{
    if (str==NULL || str[0]==0)
        return true;
        
    const CheckArgument  *pArg = poTable;
    const CheckArgument  *psbArg = NULL;    //sibArgument
    const CheckArgument  *pntArg = NULL;    //nextArgument
    const char  *p;
    const int length = strlen(str);
    
    //Ĭ�ϵļ��λ�úͳ���
    int i, pos = 0;
    int len = -1;
        
    //������Ϊ��
	if (pArg != NULL){                 
	    pos = atoi(pArg->m_sValue); 
	    
	    //������ĵڶ������Ȳ�����Ϊ�ջ���ֵ��Ϊ��
	    psbArg = pArg->m_poNextSibArgument;
	    if (psbArg!=NULL && psbArg->m_sValue[0]!=0){
	        i = atoi(psbArg->m_sValue);
	        len = i>0?i:len;
	    }
	    
	    //pos<0ʱ�������⴦�������posӦ�ò�С��0
	    if (pos<0)
	        pos = length+pos;
	        
	    len = (len==-1)?(length-pos):len;
	    //�ж�pos, len�Ƿ�Ƿ�
        if (pos<0 || (len+pos)>length)
            return false;   	    
    }
    len = (len==-1)?length:len;
    
    //Ŀǰ֧�֣�ʮ���ˡ�ʮ������
    enum Format {
        DEC,            //decimal
        OCT,            //octal
        HEX,            //hex
        UNKNOW          //unknow format
    } eFmt;    
    
    if (pFormat==NULL){
        eFmt = DEC;    
    }else if (strcmp(pFormat, "DEC")==0)
        eFmt = DEC;
    else if (strcmp(pFormat, "OCT")==0)
        eFmt = OCT;
    else if (strcmp(pFormat, "HEX")==0)
        eFmt = HEX;
    else
        eFmt = UNKNOW;
    
     
    p = &(str[pos]);
    
    
    i = 0;
    //������,����һ��'-'�ַ��������������֣�����Ӧ���ַ�����λ
    if (len>=2 && pos==0 && p[0]=='-') i++; 
        
    switch(eFmt){
    case DEC:
        for (; i<len; i++){
            if (p[i]<'0' || p[i]>'9')
                return false;
        }
        return true;
    case OCT:
        for (; i<len; i++){
            if (p[i]<'0' || p[i]>'7')
                return false;
        }
        return true;
    case HEX:
        for (; i<len; i++){
            if (p[i]<'0' || (p[i]>'9' && p[i]<'A') 
                || (p[i]>'F' && p[i]<'a') || p[i]>'z' )
                return false;
        }
        return true;
    default:
        return false;
    }
}
#else
bool CheckFunction::isNumber(const char* str, 
    const CheckArgument *poTable, const char *pFormat)
{
#ifndef PRIVATE_MEMORY 
	CheckArgument *pArgNull = &(G_PPARAMINFO->m_poCheckArgumentList[0]);
	CheckArgument *pArgument = (G_PPARAMINFO->m_poCheckArgumentList);
#endif
    if (str==NULL || str[0]==0)
        return true;
        
    const CheckArgument  *pArg = poTable;
    const CheckArgument  *psbArg = NULL;    //sibArgument
    const CheckArgument  *pntArg = NULL;    //nextArgument
    const char  *p;
    const int length = strlen(str);
    //Ĭ�ϵļ��λ�úͳ���
    int i, pos = 0;
    int len = -1;
    //������Ϊ��
	if (pArg != NULL){                 
	    pos = atoi(pArg->m_sValue); 
	    //������ĵڶ������Ȳ�����Ϊ�ջ���ֵ��Ϊ��
		if(pArg->m_iNextSibArgument)
	    	psbArg = &(pArgument[pArg->m_iNextSibArgument]);
	    if (psbArg!=NULL && psbArg->m_sValue[0]!=0){
	        i = atoi(psbArg->m_sValue);
	        len = i>0?i:len;
	    }
	    //pos<0ʱ�������⴦�������posӦ�ò�С��0
	    if (pos<0)
	        pos = length+pos;	  
	    len = (len==-1)?(length-pos):len;
	    //�ж�pos, len�Ƿ�Ƿ�
        if (pos<0 || (len+pos)>length)
            return false;   	    
    }
    len = (len==-1)?length:len;    
    //Ŀǰ֧�֣�ʮ���ˡ�ʮ������
    enum Format {
        DEC,            //decimal
        OCT,            //octal
        HEX,            //hex
        UNKNOW          //unknow format
    } eFmt;    
    if (pFormat==NULL){
        eFmt = DEC;    
    }else if (strcmp(pFormat, "DEC")==0)
        eFmt = DEC;
    else if (strcmp(pFormat, "OCT")==0)
        eFmt = OCT;
    else if (strcmp(pFormat, "HEX")==0)
        eFmt = HEX;
    else
        eFmt = UNKNOW;   
    p = &(str[pos]);
    i = 0;
    //������,����һ��'-'�ַ��������������֣�����Ӧ���ַ�����λ
    if (len>=2 && pos==0 && p[0]=='-') i++; 
    switch(eFmt){
    case DEC:
        for (; i<len; i++){
            if (p[i]<'0' || p[i]>'9')
                return false;
        }
        return true;
    case OCT:
        for (; i<len; i++){
            if (p[i]<'0' || p[i]>'7')
                return false;
        }
        return true;
    case HEX:
        for (; i<len; i++){
            if (p[i]<'0' || (p[i]>'9' && p[i]<'A') 
                || (p[i]>'F' && p[i]<'a') || p[i]>'z' )
                return false;
        }
        return true;
    default:
        return false;
    }
}
#endif

#ifdef PRIVATE_MEMORY
bool CheckFunction::beginWith(const char* str, 
    const CheckArgument *poTable, char *pHead)
{
    if (str==NULL || str[0]==0 || poTable==NULL)
        return false;
       
    const CheckArgument  *pArg = poTable;
    const CheckArgument  *pntArg = NULL;    //m_poNextArgument
    const CheckArgument  *psbArg = NULL;    //m_poNextSibArgument
    const char  *p1;
    const char  *p2;   
       
    const int length = strlen(str);
    int len;
    int pos = 0;
    
    //����ΪNULL��ӦΪʮ�����ַ���
    if (pArg->m_sValue[0]!=0 && !isNumber(pArg->m_sValue, NULL, "DEC"))
        return false;
        
    //��pos, m_sValueΪNULLʱ����0, 
    pos = atoi(pArg->m_sValue);
    
    if (pos<0 || pos>=length)
        return false;
    
    pntArg = pArg->m_poNextSibArgument;    
    while (pntArg!=NULL && pntArg->m_sValue[0]!=0){
        if (pntArg->m_sValue[0]==0 || length<strlen(pntArg->m_sValue)+pos){
            pntArg = pntArg->m_poNextArgument;
            continue;
        }
            
        p1 = &(str[pos]);
        p2 = pntArg->m_sValue;
        while (*p2!='\0' && *p1!='\0' && *p1==*p2){
            p1++;
            p2++;
        }
        if (*p2=='\0'){  //found it!
            if (pHead!=NULL)
                strcpy(pHead, pntArg->m_sValue);
            break;
        }
            
        pntArg = pntArg->m_poNextArgument;
    }
    
    if (pntArg == NULL) //not match all
        return false;
    
    return true;    
}
#else
bool CheckFunction::beginWith(const char* str, 
    const CheckArgument *poTable, char *pHead)
{
#ifndef PRIVATE_MEMORY 
	CheckArgument *pArgNull = &(G_PPARAMINFO->m_poCheckArgumentList[0]);
	CheckArgument *pArgument = (G_PPARAMINFO->m_poCheckArgumentList);
#endif
	if (str==NULL || str[0]==0 || poTable==NULL || poTable == pArgNull)
        return false;
    const CheckArgument  *pArg = poTable;
    const CheckArgument  *pntArg = NULL;    //m_poNextArgument
    const CheckArgument  *psbArg = NULL;    //m_poNextSibArgument
    const char  *p1;
    const char  *p2;   
    const int length = strlen(str);
    int len;
    int pos = 0;	
    //����ΪNULL��ӦΪʮ�����ַ���
    if (pArg->m_sValue[0]!=0 && !isNumber(pArg->m_sValue, NULL, "DEC"))
        return false;
    //��pos, m_sValueΪNULLʱ����0, 
    pos = atoi(pArg->m_sValue);
    if (pos<0 || pos>=length)
        return false;
    if(pArg->m_iNextSibArgument){
   	 	pntArg = &(pArgument[pArg->m_iNextSibArgument]); 
	} 
    while (pntArg!=NULL && pntArg->m_sValue[0]!=0){
        if (pntArg->m_sValue[0]==0 || length<strlen(pntArg->m_sValue)+pos){
			if(pntArg->m_iNextArgument){
            	pntArg = &(pArgument[pntArg->m_iNextArgument]);
			}else{
				pntArg = 0;
			}
            continue;
        }
        p1 = &(str[pos]);
        p2 = pntArg->m_sValue;
        while (*p2!='\0' && *p1!='\0' && *p1==*p2){
            p1++;
            p2++;
        }
        if (*p2=='\0'){  //found it!
            if (pHead!=NULL)
                strcpy(pHead, pntArg->m_sValue);
            break;
        }
		if(pntArg->m_iNextArgument){       
        	pntArg = &(pArgument[pntArg->m_iNextArgument]);
		}else{
			pntArg = NULL;
		}
    }
    if (pntArg == NULL) //not match all
        return false;  
    return true;    
}
#endif

#ifdef PRIVATE_MEMORY
bool CheckFunction::getOrgID(const char *sNumber, int &iOrgID, 
        const char* pTime, const CheckArgument *poTable, char *pHead, int *piShift)
{
    if (sNumber==NULL || sNumber[0]==0
        || poTable==NULL || poTable->m_sValue[0]==NULL)
        return false;
    
    int iPos = 0;
    int iOffset=0; 
    char sFlag[32];  
    char  sHead[32];
    char  sTmp[50];
    
    memset(sFlag, 0, sizeof(sFlag));
    strcpy(sFlag, poTable->m_sValue);
    poTable = poTable->m_poNextSibArgument;
    
    //�жϺ�ͷ������к�ͷ����ı�iOffset
    memset(sHead, 0, sizeof(sHead));
    if (poTable->m_poNextSibArgument!=NULL && poTable->m_poNextSibArgument->m_sValue[0]!=0){
        if (!beginWith(sNumber, poTable, sHead))
            //return false;
            iOffset = 0;
        else
        {
        	iOffset = strlen(sHead);
        	if (pHead!=NULL)
            		strcpy(pHead, sHead);	
        }
     }
        
        //get iPos
        iPos=atoi(poTable->m_sValue);  
    
    
    const char *p = &(sNumber[iPos+iOffset]);
    int j = 0;
    
    //ȥ����ͷǰ��0086���Ҵ���
    char toNBR[64];
    if (!format0086(p, toNBR, &j, piShift))
        return false;
    
    //pHead������Ҫ����һЩ����
    if (j>0)
        strncpy(&(pHead[iOffset]), &(sNumber[iPos+iOffset]), j);
    
    //test code
    /* printf("sNumber: %s\niPos: %d\niOffset: %d\nsHead: %s\np1: %s\np2: %s\n",
            sNumber, iPos, iOffset, sHead, &(sNumber[iPos+iOffset]), p);     
    return true;
    */

    return Head2OrgIDMgr::getOrgID(p, pTime, iOrgID,sFlag);

}
#else
bool CheckFunction::getOrgID(const char *sNumber, int &iOrgID, 
        const char* pTime, const CheckArgument *poTable, char *pHead, int *piShift)
{
	CheckArgument *pArgNull = &(G_PPARAMINFO->m_poCheckArgumentList[0]);
	CheckArgument *pArgument = (G_PPARAMINFO->m_poCheckArgumentList);
    if (sNumber==NULL || sNumber[0]==0 || poTable==NULL || poTable->m_sValue[0]==NULL)
        return false;
    int   iPos = 0;
    int   iOffset = 0; 
    char  sFlag[32] = {0};  
    char  sHead[32] = {0};
    char  sTmp[50] = {0};

    memset(sFlag, 0, sizeof(sFlag));
    strcpy(sFlag, poTable->m_sValue);
	if(poTable->m_iNextSibArgument){
    	poTable = &(pArgument[poTable->m_iNextSibArgument]);
	}else{
		poTable = NULL;
	}
    //�жϺ�ͷ������к�ͷ����ı�iOffset
    memset(sHead, 0, sizeof(sHead));
    if (poTable->m_iNextSibArgument!=0 && pArgument[poTable->m_iNextSibArgument].m_sValue[0]!=0){
        if (!beginWith(sNumber, poTable, sHead)){
            iOffset = 0;
        }else{
        	iOffset = strlen(sHead);
        	if (pHead!=NULL)
            	strcpy(pHead, sHead);	
        }
     }
    iPos=atoi(poTable->m_sValue);  
    const char *p = &(sNumber[iPos+iOffset]);
    int j = 0;
    //ȥ����ͷǰ��0086���Ҵ���
    char toNBR[64];
    if (!format0086(p, toNBR, &j, piShift))
        return false;
	//pHead������Ҫ����һЩ���� 
    if (j>0)  
        strncpy(&(pHead[iOffset]), &(sNumber[iPos+iOffset]), j);
    return Head2OrgIDMgr::getOrgID(p, pTime, iOrgID,sFlag);
}
#endif

#ifdef PRIVATE_MEMORY
bool CheckFunction::getAreaCode(const char *sNumber, char *pAreaCode, int iSize,
        const char *pTime, const CheckArgument *poTable)
{
    if (sNumber==NULL || sNumber[0]==0 || pAreaCode==NULL)
        return false;
    
    //�������Arg1,Arg2����Ϊ��
    int iLevelID = 0;
    CheckArgument *psbArg = poTable->m_poNextSibArgument;
    if (poTable==NULL || poTable->m_sValue[0]==0 
        || psbArg==NULL || psbArg->m_sValue[0]==0)
        return false;
        
    int iOrgID = 0;
    int iShift = 0;
    char sHead[32];
    
    //��ȡOrgID
    memset(sHead, 0, sizeof(sHead));
    if (!getOrgID(sNumber, iOrgID, pTime, psbArg, sHead, &iShift))
        return false;

    //test code
    //printf("sHead:%s iShift:%d\n", sHead, iShift);   
        
    //��OrgID��levelID��ȡ�����ֺ�
    if (!isNumber(poTable->m_sValue, NULL, NULL))
        return false;
    
    iLevelID = atoi(poTable->m_sValue);
    OrgMgrEx *poOrgMgrEx = OrgMgrEx::GetInstance(); 
    if (!poOrgMgrEx->getAreaCode(iOrgID, pAreaCode, iSize, iLevelID))
        return false;    
            
    //У�����ƥ��ĺ�ͷ
    int iPos = 0;
    int offset;
    const char *p1;
    const char *p2;
    CheckArgument *pntArg;
    
    //�ж��Ƿ������ƥ��ĺ�ͷ����û�У�����true.
    psbArg = poTable->m_poNextSibArgument->m_poNextSibArgument;
    if (psbArg==NULL || psbArg->m_poNextSibArgument==NULL
        || (pntArg=psbArg->m_poNextSibArgument->m_poNextSibArgument)==NULL)
        return true;

    iPos = atoi(psbArg->m_sValue);    
    offset = iPos+strlen(sHead)+strlen(pAreaCode)-iShift;  
    
    while (pntArg!=NULL){
        if (pntArg->m_sValue[0]==0 
            || strlen(sNumber)<strlen(pntArg->m_sValue)+offset){
            pntArg = pntArg->m_poNextArgument;
            continue;
        }
        
        p1 = &(sNumber[offset]);
        p2 = pntArg->m_sValue;
        while (*p2!='\0' && *p1!='\0' && *p1==*p2){
            p1++;
            p2++;
        }
        
        if (*p2=='\0'){ //found it 
            printf("sHead:%s\n", pntArg->m_sValue);
            break;
        }
            
        pntArg = pntArg->m_poNextArgument;
    }
   
    if (pntArg == NULL)
        return false;
        
    return true;
        
}
#else
bool CheckFunction::getAreaCode(const char *sNumber, char *pAreaCode, int iSize,
        const char *pTime, const CheckArgument *poTable)
{
	CheckArgument *pArgument = (G_PPARAMINFO->m_poCheckArgumentList);
    CheckArgument *pTmp = NULL;
	CheckArgument *pTmp1 = NULL;
	CheckArgument *pArgNull = &(G_PPARAMINFO->m_poCheckArgumentList[0]);
	if (sNumber==NULL || sNumber[0]==0 || pAreaCode==NULL)
        return false;  
    //�������Arg1,Arg2����Ϊ��
    int iLevelID = 0;
	if (poTable==NULL || poTable == pArgNull || poTable->m_sValue[0]==0) return false;
	CheckArgument *psbArg = NULL;
	if(poTable->m_iNextSibArgument){
    	psbArg = &(pArgument[poTable->m_iNextSibArgument]);
	}
    if (poTable==NULL || poTable->m_sValue[0]==0 || psbArg==NULL || psbArg->m_sValue[0]==0)
        return false;    
    int iOrgID = 0;
    int iShift = 0;
    char sHead[32];   
    //��ȡOrgID
    memset(sHead, 0, sizeof(sHead));
    if (!getOrgID(sNumber, iOrgID, pTime, psbArg, sHead, &iShift))
        return false;      
    //��OrgID��levelID��ȡ�����ֺ�
    if (!isNumber(poTable->m_sValue, NULL, NULL))
         return false;
    iLevelID = atoi(poTable->m_sValue);
    OrgMgrEx *poOrgMgrEx = OrgMgrEx::GetInstance(); 
    if (!poOrgMgrEx->getAreaCode(iOrgID, pAreaCode, iSize, iLevelID))
        return false;              
    //У�����ƥ��ĺ�ͷ
    int iPos = 0;
    int offset;
    const char *p1;
    const char *p2;
    CheckArgument *pntArg;   
    //�ж��Ƿ������ƥ��ĺ�ͷ����û�У�����true.
	pTmp = &(pArgument[poTable->m_iNextSibArgument]);
	if(pTmp->m_iNextSibArgument){
    	psbArg = &(pArgument[pTmp->m_iNextSibArgument]);
	}else{
		psbArg = NULL;
	}
	unsigned int iTmp2 = 0;
	if(psbArg->m_iNextSibArgument){
		iTmp2 = pArgument[psbArg->m_iNextSibArgument].m_iNextSibArgument; 
	}
	if(iTmp2){
		pTmp1 = &(pArgument[iTmp2]);
    }else{
		pTmp1 = NULL; 
	}
	if (psbArg == NULL || psbArg->m_iNextSibArgument==0
        || (pntArg=pTmp1)== NULL)
        return true;
    iPos = atoi(psbArg->m_sValue);    
    offset = iPos+strlen(sHead)+strlen(pAreaCode)-iShift;    
    while (pntArg!=NULL && pntArg!=pArgNull){
        if (pntArg->m_sValue[0]==0 
            || strlen(sNumber)<strlen(pntArg->m_sValue)+offset){
			if(pntArg->m_iNextArgument){
            	pntArg = &(pArgument[pntArg->m_iNextArgument]);
			}else{
				pntArg = NULL;
			}
            continue;
        }       
        p1 = &(sNumber[offset]);
        p2 = pntArg->m_sValue;
        while (*p2!='\0' && *p1!='\0' && *p1==*p2){
            p1++;
            p2++;
        }        
        if (*p2=='\0'){ //found it 
            printf("sHead:%s\n", pntArg->m_sValue);
            break;
        }        
		if(pntArg->m_iNextArgument){
        	pntArg = &(pArgument[pntArg->m_iNextArgument]);
		}else{
			pntArg = NULL;
		}
    }
    if (pntArg == NULL) //|| pntArg == pArgNull)
        return false;
    return true;  
}
#endif

bool CheckFunction::isBeyondDate(const char *pTime, const CheckArgument *poTable,char *sFileName)
{
    if (pTime==NULL || poTable==NULL )
        return false;
            
    if (poTable->m_sValue[0]!=0 && !isNumber(poTable->m_sValue, NULL, "DEC"))
        return false;
    
    Date oStartDate;    
    Date oRightDate;  //��ǰʱ��    
    oStartDate.parse(pTime, NULL);

    char sDiffType[5]={0};

    strcpy(sDiffType,poTable->m_poNextSibArgument->m_poNextSibArgument->m_sValue);
    sDiffType[1]=0;
    sDiffType[0] = toupper(sDiffType[0]);

    if (sDiffType[0] !='Y' && sDiffType[0] !='M' && sDiffType[0] !='D'
    	&& sDiffType[0] !='H' && sDiffType[0] !='I' && sDiffType[0] !='S' )
    	return false;
    
    //���poTable�ĵڶ���������Ϊ�գ���ֵ��Ϊ��
    if (poTable->m_poNextSibArgument!=NULL 
        && poTable->m_poNextSibArgument->m_sValue[0]!=0){
	char sFileTime[15]={0};
	if (strncmp(poTable->m_poNextSibArgument->m_sValue,"$FILE_NAME$",11) == 0)
	{
		if (!getFileTime(poTable->m_poNextSibArgument->m_sValue,sFileTime,sFileName))
		return false;

		strcpy(poTable->m_poNextSibArgument->m_sValue,sFileTime);
	}
        
        //���poTable�ĵڶ��������롰YYYYMMDDHHMISS����׼��ʽ����
        if (!oRightDate.parse(poTable->m_poNextSibArgument->m_sValue,"YYYYMMDDHH24MISS"))
            return false;
            
        string s1 = string(oRightDate.toString()).substr(0, 8);
        string s2 = string(oRightDate.toString()).substr(8);
        if (oRightDate.checkDate(s1.c_str())==-1 || oRightDate.checkTime( s2.c_str())==-1)
            return false;
    }
    
    int iDiffMin = atoi(poTable->m_sValue);
    
    //oStartDate.addMin(iDiffMin);
    
    //printf("oStartDate:   %s\n", oStartDate.toString());
    //printf("oRightDate:   %s\n", oRightDate.toString());

//�������ڣ����������ʱ����Ϊ0�����Ϊ������Ϊ1��
    if (sDiffType[0] =='Y')
    	{
    		oStartDate.setMonth(1);
    		oStartDate.setDay(1);
    		oRightDate.setMonth(1);
    		oRightDate.setDay(1);    		
    	}
    if (sDiffType[0] =='M')
    	{
    		oStartDate.setDay(1);    	
    		oRightDate.setDay(1);    		
    	}
    if (sDiffType[0] =='D')
    	{
    		oStartDate.setHour(0);
    		oStartDate.setMin(0);
    		oStartDate.setSec(0);
    		oRightDate.setHour(0);
    		oRightDate.setMin(0);
    		oRightDate.setSec(0);
    	}    
    if (sDiffType[0] =='H')
    	{
    		oStartDate.setMin(0);
    		oStartDate.setSec(0);    	
    		oRightDate.setMin(0);
    		oRightDate.setSec(0);
    	}
    if (sDiffType[0] =='I')
    	{
    		oStartDate.setSec(0);    	
    		oRightDate.setSec(0);    	
    	}
    
    //��ǰԽ��
    if (iDiffMin<=0 )
    {
    		if (sDiffType[0] =='Y' )
    			{
				oRightDate.addYear(iDiffMin);
    				if (oStartDate< oRightDate)
    					return true;
    			}
			
    		if (sDiffType[0] =='M' )
    			{
				oRightDate.addMonth(iDiffMin);
    				if (oStartDate < oRightDate)
    					return true;
    			}
			
    		if (sDiffType[0] =='D' )
    			{
				oRightDate.addDay(iDiffMin);
    				if (oStartDate < oRightDate)
    					return true;
    			}
			
    		if (sDiffType[0] =='H' )
    			{
    				oRightDate.addHour(iDiffMin);
    				if (oStartDate < oRightDate)
    					return true;
    			}			
    		if (sDiffType[0] =='I' )
    			{
    				oRightDate.addMin(iDiffMin);
    				if (oStartDate < oRightDate)
    					return true;
    			}
			
    		if (sDiffType[0] =='S' )
    			{
    				oRightDate.addSec(iDiffMin);
    				if (oStartDate < oRightDate)
    					return true;   
    			}
			
    }      
    //���Խ��
    else if (iDiffMin>0 )
	    	{
	    		if (sDiffType[0] =='Y' )
    			{
				oRightDate.addYear(iDiffMin);
    				if (oStartDate> oRightDate)
    					return true;
    			}
			
    		if (sDiffType[0] =='M' )
    			{
				oRightDate.addMonth(iDiffMin);
    				if (oStartDate > oRightDate)
    					return true;
    			}
			
    		if (sDiffType[0] =='D' )
    			{
				oRightDate.addDay(iDiffMin);
    				if (oStartDate > oRightDate)
    					return true;
    			}
			
    		if (sDiffType[0] =='H' )
    			{
    				oRightDate.addHour(iDiffMin);
    				if (oStartDate > oRightDate)
    					return true;
    			}			
    		if (sDiffType[0] =='I' )
    			{
    				oRightDate.addMin(iDiffMin);
    				if (oStartDate > oRightDate)
    					return true;
    			}
			
    		if (sDiffType[0] =='S' )
    			{
    				oRightDate.addSec(iDiffMin);
    				if (oStartDate >oRightDate)
    					return true;   
    			}
	    		
	    }      
    //δԽ��
     return false;
}

bool CheckFunction::getOrgID(const EventSection *poEventSection, int &iOrgID, 
       const CheckArgument *poTable, char *pHead)
{
    const char *sMSC = poEventSection->m_sMSC;
    const char *sLAC = poEventSection->m_sLAC;
    const char *sCellID = poEventSection->m_sCellID;
    
    if (sMSC==NULL)
        return false;
        
    int iPos = 0;
    char sHead[32];
    CheckArgument *psbArg = NULL;
    memset(sHead, 0, sizeof(sHead));
    
    //��������Ϊ��
    if (poTable!=NULL){
        iPos = atoi(poTable->m_sValue);
     
        CheckArgument *psbArg = poTable->m_poNextSibArgument;
        
        //��������ڶ�����ͷ������Ϊ�գ� �жϺ�ͷ�Ƿ�ƥ��
        if (psbArg!=NULL && !beginWith(sMSC, psbArg, sHead))
            return false;
            
        //�����ѡ����pHead
        if (pHead != NULL && sHead[0]!=0)
            strcpy(pHead, sHead);
    }
    
   int iHeadLen = strlen(sHead);
   int iOffset = iPos + iHeadLen;
   
   if (iOffset>=strlen(sMSC))
        return false;
   
   //���ý��εĽӿ�
   const char* szTime =0;
   return MobileRegionMgr::GetOrgID(&(sMSC[iOffset]),sLAC,sCellID,szTime,iOrgID);
   
}

bool CheckFunction::isMSC(const char *sMSC, const CheckArgument *poTable, 
    char *pHead)
{
    if (sMSC==NULL)
        return false;
        
    int iPos = 0;
    char sHead[32];
    CheckArgument *psbArg = NULL;
    memset(sHead, 0, sizeof(sHead));
    
    //��������Ϊ��
    if (poTable!=NULL){
        iPos = atoi(poTable->m_sValue);
     
        CheckArgument *psbArg = poTable->m_poNextSibArgument;
        
        //��������ڶ�����ͷ������Ϊ�գ� �жϺ�ͷ�Ƿ�ƥ��
        if (psbArg!=NULL && psbArg->m_sValue[0]!=0 && !beginWith(sMSC, poTable, sHead))
            return false;
            
        //�����ѡ����pHead
        if (pHead != NULL && sHead[0]!=0)
            strcpy(pHead, sHead);
    }
    
   int iHeadLen = strlen(sHead);
   int iOffset = iPos + iHeadLen;
   
   if (iOffset>=strlen(sMSC))
        return false;
   
   //get switch ID
   //���ý��εĽӿ�
   return MobileRegionMgr::MscHasExists(&(sMSC[iOffset]));
   
}

bool CheckFunction::isAreaCode(const char *sNumber, char *pAreaCode, int iSize, 
        const char *pTime, const CheckArgument *poTable)
{
    if (sNumber==NULL || sNumber[0]==0
        || poTable==NULL || poTable->m_sValue[0]==0)
        return false;
    
    int iPos = 0;
    int iOffset = 0; 
    int iOrgID = 0;
    char  sFlag[32];  
    char  sHead[32];
    char  sCode[50];
    
    memset(sFlag, 0, sizeof(sFlag));
    strcpy(sFlag, poTable->m_sValue);
    poTable = poTable->m_poNextSibArgument;
    
    //�жϺ�ͷ������к�ͷ����ı�iOffset
    memset(sHead, 0, sizeof(sHead));
    if (poTable->m_poNextSibArgument!=NULL || poTable->m_poNextSibArgument->m_sValue[0]!=0){
        if (!beginWith(sNumber, poTable, sHead))
            return false;
                 
        iOffset = strlen(sHead);
        //get iPos
        iPos=atoi(poTable->m_sValue);       
     }
    
    const char *p = &(sNumber[iPos+iOffset]);
    memset(sCode, 0, sizeof(sCode));
    
    //�Զ�������ǰ�ӡ�0��
    if (p[0]!='0'){
    		sCode[0]='0';
    		strncpy(&(sCode[1]), p, sizeof(sCode)-2 );
    	}
    else
    	strncpy(sCode,p,sizeof(sCode)-1);
    
    //test code
    //printf("sCode:%s", sCode);       
       
   if (!Head2OrgIDMgr::getOrgID(sCode, pTime, iOrgID, sFlag))
        return false;
 
   if (pAreaCode!=NULL)
        strncpy(pAreaCode, sCode, iSize-1);

    return true;    
}

bool CheckFunction::cmpTwoFunc(const EventSection *poEventSection, 
        const CheckArgument *poTable)
{
    if (poTable==NULL || poTable->m_sValue[0]==0)
        return false;
    
    const CheckArgument *psbArg = poTable;
    int iFuncID1, iFuncID2;
    int iTAG1, iTAG2;
    string sRet1, sRet2;
    
    iFuncID1 = atoi(psbArg->m_sValue);
    if (iFuncID1==CK_CMP_TWO_FUNC)
        return false;
    
    psbArg = psbArg->m_poNextSibArgument;
    if (psbArg==NULL || psbArg->m_sValue[0]==0)
        return false;
        
    iTAG1 = atoi(psbArg->m_sValue);    
    //ִ�е�һ������
    psbArg = psbArg->m_poNextSibArgument;
    doFunction(iFuncID1, poEventSection, iTAG1, psbArg, &sRet1);
    
    //ִ�еڶ�������
    while (psbArg!=NULL){
        if (strcmp(psbArg->m_sValue, "NULL")==0)
            break;
        psbArg = psbArg->m_poNextSibArgument;
    }
    
    if (psbArg==NULL)
        return false;
    
    psbArg = psbArg->m_poNextSibArgument;
    if (psbArg==NULL || psbArg->m_sValue[0]==0)
        return false;
        
    iFuncID2 = atoi(psbArg->m_sValue);
    if (iFuncID1==CK_CMP_TWO_FUNC)
        return false;
        
    psbArg = psbArg->m_poNextSibArgument;
    if (psbArg==NULL || psbArg->m_sValue[0]==0)
        return false;
        
    iTAG2 = atoi(psbArg->m_sValue);
    psbArg = psbArg->m_poNextSibArgument;
    doFunction(iFuncID2, poEventSection, iTAG2, psbArg, &sRet2);
    
    //test code
    //printf("sRet1: %s\nsRet2: %s\n", sRet1.c_str(), sRet2.c_str());
    
    return sRet1==sRet2;
}

bool CheckFunction::formatCarrier(const EventSection *poEventSection, 
        int &iCarrierType, char *psResult)
{
    EventSection oEvent = *poEventSection;
    FormatMethod::formatCarrier(&oEvent);
    
    iCarrierType = oEvent.m_oEventExt.m_iCarrierTypeID;
    
    if (psResult!=NULL){
        strcpy(psResult, oEvent.m_sCalledNBR);        
    }
    
    if (iCarrierType==0)
        return false; 
    
    return true;
        
}

bool CheckFunction::pipe(const EventSection *poEventSection, 
        const CheckArgument *poTable, char *pResult)
{
    if (poTable==NULL || poTable->m_sValue[0]==0)
        return false;
    
    const CheckArgument *psbArg = poTable;
    int iFuncID1, iFuncID2;
    int iTAG1, iTAG2;
    string sPrev, sCurr;
    bool bRet = false;
    
    iFuncID1 = atoi(psbArg->m_sValue);
    if (iFuncID1==CK_PIPE)
        return false;
    
    psbArg = psbArg->m_poNextSibArgument;
    if (psbArg==NULL || psbArg->m_sValue[0]==0)
        return false;
        
    iTAG1 = atoi(psbArg->m_sValue);    
    //ִ�е�һ������
    psbArg = psbArg->m_poNextSibArgument;
    doFunction(iFuncID1, poEventSection, iTAG1, psbArg, &sPrev);
    
    //ִ�еں�̺���
    while (1){
        while (psbArg!=NULL){
            if (strcmp(psbArg->m_sValue, "NULL")==0){
                psbArg = psbArg->m_poNextSibArgument;
                break;
            }
            psbArg = psbArg->m_poNextSibArgument;
        }
        
        if (psbArg==NULL)
            break;
            
        iFuncID2 = atoi(psbArg->m_sValue); 
        
        //֧��Ƕ�׵���CK_PIPE
        if (iFuncID2 == CK_PIPE)
            break;
               
        iTAG2 = -1;
        psbArg = psbArg->m_poNextSibArgument;
        
        bRet = doFunction(iFuncID2, poEventSection, iTAG2, psbArg, &sCurr, &sPrev);
        sPrev = sCurr;
    }
    
    if (pResult!=NULL)
            strcpy(pResult, sCurr.c_str());
    return bRet;
}

bool CheckFunction::formatAreaCode(const EventSection *poEventSection,
        char *psResult)
{
    EventSection oEvent = *poEventSection;
    int iCarrierType = oEvent.m_oEventExt.m_iCarrierTypeID;
    
    
    char sTmp[64];
    char sAccNBR[64];
    strcpy(sAccNBR, oEvent.m_sCalledNBR);
    
    //���iCarrierType=0���������������
    if (iCarrierType==0){
        formatCarrier(&oEvent, iCarrierType, sAccNBR);
    }
    
    //ȥ����ͷǰ��0086���Ҵ���
    char toNBR[64];
    const char *p = sAccNBR;
    if (!format0086(p, toNBR))
        return false;

    //��������
    strcpy(oEvent.m_sCalledNBR, p);
    EventTypeMgrEx *poEvtTypeMgrEX = EventTypeMgrEx::GetInstance();
    poEvtTypeMgrEX->fillAreaCode(&oEvent);
    
    
    if (psResult!=NULL){
        strcpy(psResult, oEvent.m_sCalledNBR);        
    }
    
    return strcmp(p, oEvent.m_sCalledNBR)!=0;
}

bool CheckFunction::getPartnerID(const char *sAccNBR, const char *pTime, int &iPartnerID)
{

    if (sAccNBR==NULL)
        return false;
    
    //ȥ����ͷǰ���ܴ��ڵ�0086���Ҵ���
    char toNBR[64];
    if (!format0086(sAccNBR, toNBR))
        return false;
    
    //������ͷ    
    HeadEx oLocalHead;
    bool bRet = Head2OrgIDMgr::searchLocalHead(toNBR, pTime, &oLocalHead);
     
    //��ȡ��Ӫ�̺�
    iPartnerID = oLocalHead.getPartnerID();
    
    return bRet;
}

bool CheckFunction::format0086(const char *fromNBR, char *toNBR, int *trimLen, int *piShift)
{

    if (fromNBR==NULL || toNBR==NULL)
        return false;
    
    const char *p = fromNBR;
    char sTmp[64];
    bool bHead86 = false;
    
    //��p�ַ�����Ϊ0086��ͷʱ�����������д���
    if (strlen(p)>=7 && p[0]=='0' && p[1]=='0' && p[2]=='8' && p[3]=='6'){
        
        //008610Ϊ�������ţ�00861[2-9]�����ֻ���
        if (p[4]=='1' && p[5]!='0')
            p = &(p[4]);
        
        //008610,��0086[2-9]���Ƿ��ֻ��û���ת��Ϊ: 0+�������ŵĸ�ʽ��
        else if (p[4]!='0'){
            memset(sTmp, 0, sizeof(sTmp));
            sTmp[0]='0';
            strcpy(&(sTmp[1]), &(p[4]));
            p = sTmp;
            if (piShift!=NULL) *piShift = 1;
        }
        
        //0086+0+��������/�ֻ��ţ� ת��Ϊ: 0+��������/�ֻ��ŵĸ�ʽ
        else
            p = &p[4];
            
        if (trimLen!=NULL) *trimLen = 4;
        bHead86 = true;
    }

    //����013��015��018��13��15��18��ͷ�Ĳ������ж�
    if(strncmp(p,"013",3) == 0 ||strncmp(p,"015",3) == 0 || strncmp(p,"018",3) == 0  )
    {
		p = &p[1];	
		if (bHead86)
    			*trimLen = 5;
		else
			*trimLen = 1;
    }
    
    strcpy(toNBR, p);
    
    return true;
}

bool CheckFunction::isInner(const EventSection *poEventSection, 
    const char *pTime)
{
    if (poEventSection==NULL)
        return false;
    
    int iTmp, iTmp2;
    
    //��ȡ��Ӫ��ID
    if (!getPartnerID(poEventSection->m_sOrgCallingNBR, pTime, iTmp))
        return false;
        
    //������������
    char sTmp[64];
    formatAreaCode(poEventSection, sTmp);
   
    if (!getPartnerID(sTmp, pTime, iTmp2))
        return false;
        
    return iTmp==iTmp2;
    
       
}

bool CheckFunction::getLongTypeAndRoamID(const EventSection *poEventSection, 
    int &iLongType, int &iRoamID)
{
    if (poEventSection==NULL)
        return false;    
    
    EventSection oEvtSect = *poEventSection;
    EventTypeMgrEx *poEvtTypeMgrEx = EventTypeMgrEx::GetInstance();
    poEvtTypeMgrEx->fillLongTypeAndRoamID(&oEvtSect);
       
    iLongType = oEvtSect.m_oEventExt.m_iLongTypeID;
    iRoamID   = oEvtSect.m_oEventExt.m_iRoamTypeID;
    
    return true;
    
}

bool CheckFunction::cmpCharofString(const char *str,const CheckArgument *poTable,char &cNotFind)
{
	string sHeadList;
	char sCmpStr[50]={0};
	int iPos = 0,i = 0;
	CheckArgument *p;
	if (poTable == NULL)
		return false;

	sHeadList.clear();
	iPos = atoi(poTable->m_sValue);
	if (iPos>=strlen(str))
		return false;
	
	p = poTable->m_poNextSibArgument;
	while (p)
	{
		if (strlen(p->m_sValue) > 1)
			return false;
		sHeadList.append(p->m_sValue);
		p = p->m_poNextArgument;
	}

	strcpy(sCmpStr,str+iPos);
	for (i = 0;i<strlen(sCmpStr);i++)
	{
		iPos = sHeadList.find(sCmpStr[i]);
		if (iPos == string::npos)
		{
			cNotFind = sCmpStr[i];
			return false;
		}
	}

	return true;
}

void CheckFunction::to_Lower(char * sStr)
{
	int i=0;	
	
	for (i = 0; i<strlen(sStr); i++)
		sStr[i] = tolower(sStr[i]);	
}

void CheckFunction::to_Upper(char * sStr)
{
	int i=0;	
	
	for (i = 0; i<strlen(sStr); i++)
		sStr[i] = toupper(sStr[i]);	
}
	
bool CheckFunction::cmpSubStr(const char *str,const CheckArgument *poTable)
{
	char sCmpStr[30]={0},sDesStr[30]={0};
	int iPos=0,iLen=0;
	CheckArgument *poArg;	

	if (poTable->m_poNextSibArgument == NULL || poTable->m_poNextSibArgument->m_poNextSibArgument== NULL)
		return false;

	iPos = atoi(poTable->m_sValue);
	if (poTable->m_poNextSibArgument->m_sValue[0] == 0)
	{
		if (iPos>=0)
			iLen = strlen(str+iPos);
		else
			iLen = 0-iPos;
	}
	else
		iLen = atoi(poTable->m_poNextSibArgument->m_sValue);

	if (iLen < 0 ) return false;

	poArg = poTable->m_poNextSibArgument->m_poNextSibArgument;
	strcpy(sDesStr,poArg->m_sValue);

	if (iPos>=0)
	{
		if ((iPos+iLen) > strlen(str))
			return false;
		strncpy(sCmpStr,str+iPos,iLen);
	}
	else
	{
		if ((iPos+iLen) > 0 || (strlen(str)+iPos) <0 )
			return false;

		strncpy(sCmpStr,str+strlen(str)+iPos,iLen);
	}

	to_Upper(sCmpStr);
	to_Upper(sDesStr);
	if (strcmp(sCmpStr,sDesStr) == 0 )
		return true;
	return false;
}

bool CheckFunction::check_zone(const char * str, const CheckArgument * poTable,int iMaxFlag)
{
	static ZoneMgr * zone = 0;
        if (!zone) {
        	zone = new ZoneMgr ();
        }

	char sZoneValue[MAX_STRING_VALUE_LEN]={0};
	strcpy(sZoneValue,str);
	to_Upper(sZoneValue);

	const char *sZoneID=poTable->m_sValue;
	const char *sZoneItemID = poTable->m_poNextSibArgument->m_sValue;
	if (!isNumber(sZoneID, NULL, "DEC"))
		return false;
	if (!isNumber(sZoneItemID, NULL, "DEC"))
		return false;

	int iZoneID = atoi(sZoneID);
	int iZoneItemID = atoi(sZoneItemID);

	if (iMaxFlag == 0)
	{
		if (zone->check (iZoneID, sZoneValue,iZoneItemID)) 
		{
                 	return true;   
        	} 
	}
	else
	{
		if (zone->checkMax(iZoneID,sZoneValue,iZoneItemID))
		{
			return true;
		}
	}
	
	return false;
}

bool CheckFunction::getFileTime(char *sTag,char *sFileTime,char *sFileName)
{
	char sLow[3]={0},sLen[10]={0};
	Date td;
	//$FILE_NAME$
	char *sFlag=strrchr(sTag,'$')+1;
	char *sSep=strrchr(sTag,',');
	if (!sSep)
		return false;

	strncpy(sLow,sFlag,sSep-sFlag);
	strcpy(sLen,sSep+1);
	if (!isNumber(sLow,NULL) || !isNumber(sLen,NULL))
		return false;

	int iLow=atoi(sLow);
	int iLen = atoi(sLen);
	if (iLow<0)
	{
		iLow=strlen(sFileName)+iLow;
		if (iLow<0)
			iLow=0;
	}
	else
	{
		if (iLow && iLow>strlen(sFileName))
			return false;
	}

	if (iLen>14)
		iLen=14;

	strncpy(sFileTime,sFileName+iLow,iLen);
	if (strlen(sFileTime)<14)
	{
		char sSuffix[15]={0};
		strncpy(sSuffix,"00000000000000",14-strlen(sFileTime));
		strcat(sFileTime,sSuffix);
	}
	sFileTime[14]=0;

	if (!isNumber(sFileTime,NULL))
		return false;

	if (!td.parse(sFileTime))
		return false;
		
	
}


