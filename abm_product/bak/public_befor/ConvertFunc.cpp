/*VER: 1*/ 
#include <strings.h>
#include "Environment.h"
#include "ConvertFunc.h"
#include "Log.h"
#ifndef PRIVATE_MEMORY
#include "ParamInfoInterface.h"
#endif


void ParseRuleMgr::initRule ()
{
    DEFINE_QUERY (qry);
    
    for (int i=1; i<=MAX_TLV_FLAGS; i++)
        m_pParseRule[i] = 0;
    
    qry.setSQL (" Select nvl(Max(parse_rule_group_id),0) "
        " From B_PROCESS_PARSE_COMBINE Where process_id = :ProcID "
    );
    qry.setParameter ("ProcID", m_iProcessID);
    
    qry.open ();
    qry.next ();
    int iGroupID = qry.field(0).asInteger();
    qry.close();
    
    //����Ƿ���Ĭ�Ͻ���������0
    if (iGroupID == 0) {
        qry.setSQL (" select count(*) from B_PARSE_RULE_GROUP "
            " where parse_rule_group_id = 0" );
        qry.open ();
        qry.next ();
        if (!qry.field(0).asInteger()) {
            Log::log (0, "��ǰ����(%d)δ�������������,Ҳδ�ҵ�Ĭ��(��ID=0��)����������",m_iProcessID);
            Log::log (0, "������ñ���:B_PARSE_RULE_GROUP,B_PROCESS_PARSE_COMBINE,B_PARSE_RULE");
            THROW (100100003);
        }
        qry.close ();
    }
    
    qry.setSQL ("  Select tlv_tag_id, begin_pos, DATA_LEN, "
        " parse_func_id, attr_id, bit_position "
        " From b_parse_rule "
        " Where parse_rule_group_id = :GrpID "
        " Order By tlv_tag_id, begin_pos "
    );
    qry.setParameter ("GrpID",iGroupID);
    qry.open ();
    
    while (qry.next ()) 
    {   
        ParseRule **ppRule;
        
        int iTag = qry.field ("tlv_tag_id").asInteger();
        int iBeginPos = qry.field ("begin_pos").asInteger();
        int iDataLen = qry.field ("DATA_LEN").asInteger();
        int iFuncID = qry.field ("parse_func_id").asInteger();
        int iAttrID = qry.field ("attr_id").asInteger();
        int iBitPos = qry.field ("bit_position").asInteger();     
        
        //# ��bitλ��Чʱ, ת������ǿ��ʹ��3, ���ݳ���ǿ����Ϊ1���ֽ�
        if (iBitPos>=1 && iBitPos<=8) {
            iFuncID = 3;
            iDataLen = 1;
        }
        
        if (iTag<1 || iTag>MAX_TLV_FLAGS) {
            Log::log (0,"���ؽ��������쳣, ϵͳ����TAGλ��Χ(1--%d) \n"
                "��ǰ�������õ�TAG��ʶΪ: %d ",
                MAX_TLV_FLAGS, iTag
            );
            THROW (100100004);
        }
        
        for (ppRule=&(m_pParseRule[iTag]); *ppRule; ppRule=&((*ppRule)->m_pNext));
        
        *ppRule = new ParseRule;
        
        (*ppRule)->m_iTag = iTag;
        (*ppRule)->m_iBeginPos = iBeginPos;
        (*ppRule)->m_iDataLen = iDataLen;
        (*ppRule)->m_iFuncID = iFuncID;
        (*ppRule)->m_iAttrID = iAttrID;
        (*ppRule)->m_iBitPosition = iBitPos;
        
        (*ppRule)->m_pNext = 0;
    }   
    
    qry.close ();
}



    
void ParseRuleMgr::destoryRule ()
{
    for (int i=1; i<=MAX_TLV_FLAGS; i++) {
        ParseRule *pHead, *pDel;
        pHead = m_pParseRule[i];
        for (;;) {
            if (!pHead) break;
            pDel = pHead;
            pHead = pHead->m_pNext;
            delete pDel;
        }
    }
}

#ifndef PRIVATE_MEMORY
ParseRule *ParseRuleMgr::getHeadParseRule()
{
	return (ParseRule*)G_PPARAMINFO->m_poParseRuleList;
}

#endif


// ����Tagλ, ������Ӧtagλ��Ӧ�Ľ�������ͷָ��
#ifdef PRIVATE_MEMORY
ParseRule *ParseRuleMgr::getParseRule (int _iTag)
{
    return m_pParseRule[_iTag];
}
#else
ParseRule *ParseRuleMgr::getParseRule (int _iTag)
{
	ParseComb *pParseComb=0;
	ParseComb *pComb=0;
	ParseRule *pParseRule=0;
	ParseRule *pRule=0;
    unsigned int iIdx1 = 0,iIdx2 = 0;

	pComb = (ParseComb*)G_PPARAMINFO->m_poParseCombList;
	pRule = (ParseRule*)G_PPARAMINFO->m_poParseRuleList;
	if(G_PPARAMINFO->m_poParseCombIndex->get(m_iProcessID,&iIdx1))
	{
		pParseComb = pComb + iIdx1;

		char sKey[30]={0};
		sprintf(sKey,"%d_%d",pParseComb->m_iParseGroupID,_iTag);
		if(G_PPARAMINFO->m_poParseRuleIndex->get(sKey,&iIdx2))
		{
			pParseRule = pRule + iIdx2;
			return pParseRule;
		}
		else
			return NULL;
	}
	else
	  return NULL;

}
#endif



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void  ConvertMgr::convTLV (int iTlvFlag, int iTlvLen, BIT8 *src, 
        ParseRuleMgr *pRuleMgr, TicketTlv *pTlv, StdEvent *pStd)
{
    static ID *pID = new ID("");
    static BIT8 sTemp[2048];
    Value oValue;
    
    memset (&oValue, 0, sizeof(Value));
    
    pTlv->setTagValue (iTlvFlag, src, iTlvLen);
    
    ParseRule *pRuleHead = pRuleMgr->getParseRule (iTlvFlag);
    
    // δ�ҵ���������
    if (!pRuleHead) return;
    
    // ִ�е�ǰtag��ÿһ����������
#ifdef PRIVATE_MEMORY
    for (ParseRule *pRule=pRuleHead; pRule; pRule=pRule->m_pNext) 	
	{        
        int iLen;
        
        //��ʼλ����Ч,���Թ���
        if (pRule->m_iBeginPos<0 || pRule->m_iBeginPos>=iTlvLen) 
            continue;
        
        if (pRule->m_iDataLen>0 && (pRule->m_iBeginPos+pRule->m_iDataLen)<=iTlvLen)
            iLen = pRule->m_iDataLen;
        else
            iLen = iTlvLen - pRule->m_iBeginPos;
            
        memset (sTemp, 0, 2048);
        
        memcpy (sTemp, src+pRule->m_iBeginPos, iLen);
                    
        convert (sTemp, &oValue, pRule->m_iFuncID, iLen, pRule->m_iBitPosition);
        
        pID->m_iAttrID = pRule->m_iAttrID;
        
        IDParser::setValue ((EventSection *)pStd, pID, &oValue);
    }
#else
	ParseRule *pHead = pRuleMgr->getHeadParseRule();
	ParseRule *pRule = pRuleHead; 

	for(;pRule != pHead; pRule = pHead+pRule->m_iNextOffset)
	{		 
		int iLen;
			
		//��ʼλ����Ч,���Թ���
		if (pRule->m_iBeginPos<0 || pRule->m_iBeginPos>=iTlvLen) 
			continue;
			
		if (pRule->m_iDataLen>0 && (pRule->m_iBeginPos+pRule->m_iDataLen)<=iTlvLen)
			iLen = pRule->m_iDataLen;
		else
			iLen = iTlvLen - pRule->m_iBeginPos;
				
		memset (sTemp, 0, 2048);
			
		memcpy (sTemp, src+pRule->m_iBeginPos, iLen);
						
		convert (sTemp, &oValue, pRule->m_iFuncID, iLen, pRule->m_iBitPosition);
			
		pID->m_iAttrID = pRule->m_iAttrID;
			
		IDParser::setValue ((EventSection *)pStd, pID, &oValue);
	}
#endif	

    return;
}


void  ConvertMgr::convert (BIT8 *src, Value *pValue, int _iFuncID, int _iLen, int _iBitPos)
{
    static int iAdjustLen;
    int iLen = 0;
    switch (_iFuncID) {
        case 0: // ����ת��, �ڴ�ֱ�Ӹ�ֵ
            //memcpy (pValue->m_sValue, src, _iLen);
            //pValue->m_sValue[_iLen]=0;
			iLen= _iLen>(sizeof(pValue->m_sValue)-1)?(sizeof(pValue->m_sValue)-1):_iLen;
            memcpy (pValue->m_sValue, src, iLen);
            pValue->m_sValue[iLen]=0;
            break;
        
        case 1: // �� BIN ��ʽ�ֶ�ת����Ϊ ASC, ������ת��Ϊ��Ӧ��ʽ���ַ������
            iAdjustLen = MAX_STRING_VALUE_LEN-1 < _iLen ? MAX_STRING_VALUE_LEN-1 : _iLen ;
            binToAscii (src, pValue->m_sValue, iAdjustLen);
            break;
        
        case 2: // �� BCD ��ʽת���� long (BCDԴ��ʽ�����������bcd��,����,��ض�,�� 123e56-->123)
            pValue->m_lValue = bcdToLong (src, _iLen);
            break;
        
        case 3: // �� BIN ��ʽת��Ϊ long
            pValue->m_lValue = binToLong (src, _iLen);            
            if (_iBitPos>=1 && _iBitPos<=8 && _iLen==1) //## ��Ҫȡbitλ��ֵ
                pValue->m_lValue = getBitValue ((unsigned char)pValue->m_lValue, _iBitPos);
            break;
        
        case 4: // �� BCD ��ʽת���� Ascii �� (������������ַ�)
            iAdjustLen = (MAX_STRING_VALUE_LEN-1)/2 < _iLen ? (MAX_STRING_VALUE_LEN-1)/2 : _iLen ;
            bcdToAscii_Left_D (src, (BIT8 *)pValue->m_sValue, iAdjustLen);
            break;
        
        case 5: // �� BCD ��ʽת���� Ascii �� (����������ַ�A~F)
            iAdjustLen = (MAX_STRING_VALUE_LEN-1)/2 < _iLen ? (MAX_STRING_VALUE_LEN-1)/2 : _iLen ;
            bcdToAscii_Left_X (src, (BIT8 *)pValue->m_sValue, iAdjustLen);
            break;
            
        case 6: // Ascii���ʽת����Long
            pValue->m_lValue = asciiToLong (src, _iLen);
            break;
        

        case 7: // �� BIN ��ʽת��Ϊ long
            pValue->m_lValue = binToLong2 (src, _iLen);
            if (_iBitPos>=1 && _iBitPos<=8 && _iLen==1) //## ��Ҫȡbitλ��ֵ
                pValue->m_lValue = getBitValue ((unsigned char)pValue->m_lValue, _iBitPos);
            break;

        case 8: // ��ASC��� ����(��λԪ) ת������ֵ�͵ķ���(��λ��)
            iAdjustLen = MAX_STRING_VALUE_LEN-1 < _iLen ? MAX_STRING_VALUE_LEN-1 : _iLen ;
            memcpy (pValue->m_sValue, src, iAdjustLen);
            pValue->m_sValue[iAdjustLen]=0;
            //pValue->m_lValue = (long)(atof(pValue->m_sValue)*100);
            strcat(pValue->m_sValue,"e+2");
            pValue->m_lValue = (long)(atof(pValue->m_sValue));
            break;
        
        case 9:
            longToString (src, (BIT8 *)pValue->m_sValue, _iLen);
            break;

        default :
            break;
    }
}

long  ConvertMgr::asciiToLong (BIT8 *src, int length)
{
    char sTemp[21];
    int iLen = length<20 ? length:20;
    
    memcpy (sTemp, src, iLen);
    sTemp[iLen] = 0;
    
    return atol(sTemp);
}


// �� BIN ��ʽת���� long
unsigned long ConvertMgr::binToLong (BIT8 *src, int length)
{
    unsigned short sValue = 0;
    unsigned int   iValue = 0;
    unsigned long  lValue = 0;
    unsigned long  lRet;

    if (length<=0 || length>sizeof(long))
        return 0L;
    
    switch (length) {
        case 1:
            lRet = (int)src[0];
            break;
            
        case 2:
            memcpy (&sValue, src, 2);
            lRet = ntohs(sValue);
            break;
            
        case 4:
            memcpy (&iValue, src, 4);
            lRet = ntohl(iValue);
            break;
        
        default:
            memcpy ((BIT8 *)&lValue+(sizeof(long)-length), src, length);
            lRet = ntohl(lValue);
            break;
    }
    
    return lRet;
}

// �� BIN ��ʽת���� long 2
long ConvertMgr::binToLong2 (BIT8 *src, int length)
{
    short sValue = 0;
    int   iValue = 0;
    long  lValue = 0;
    long  lRet;

    if (length<=0 || length>sizeof(long))
        return 0L;

    switch (length) {
        case 1:
            lRet = (int)src[0];
            break;

        case 2:
            memcpy (&sValue, src, 2);
            lRet = ntohs(sValue);
            break;

        case 4:
            memcpy (&iValue, src, 4);
            lRet = ntohl(iValue);
            break;

        default:
            memcpy ((BIT8 *)&lValue+(sizeof(long)-length), src, length);
            lRet = ntohl(lValue);
            break;
    }

    return lRet;
}

// ��bin��ʽ�ֶ�ת����ΪASC, ת��Ϊ��Ӧ�ַ������
void  ConvertMgr::binToAscii (BIT8 * src,char * dest,int length)
{
    char sBuff[200];
    char ssBuff[200];
    sBuff[0]=0;
    int j=(int)(src[0]);
    sprintf(sBuff,"%d",j);
    strcpy(ssBuff,sBuff);
    
    for (int i=1;i<length;i++)
    {
        sBuff[0]=0;
        j=(int)(src[i]);
        sprintf(sBuff,"%d",j);
        strcat(ssBuff,sBuff);
    }
    strcpy(dest,ssBuff);
}

// �� BCD ��ʽת���� long (�����������bcd��)
long ConvertMgr::bcdToLong (BIT8 *src, int length)
{
    static long lTemp;
    
    long lValue = 0;
    int iLen = length * 2;
    int iConv = 1;
    
    for (int i=0; i<iLen; i++) {
        
        if (i%2) {
            lTemp = (long)src[i/2]%16;
            if (lTemp>9) break; //bcd ����Ч
            lValue = lValue*10 + lTemp;
        }
        else {
            lTemp = (long)src[i/2]/16;
            if (i==0 && lTemp==15){ //�����һλΪF,��︺��
                iConv = -1;
                continue;
            }
            if (lTemp>9) break; //bcd ����Ч
            lValue = lValue*10 + (long)src[i/2]/16;
        }
    }

    return lValue * iConv;
}

//���޷��ŵ���ֵ��ֱ��ת�����ַ��� ����ֵ 123 ת�����ַ��� "123"
void  ConvertMgr::longToString (BIT8 * src, BIT8 * dest, int length)
{
    unsigned long lValue = binToLong (src, length);
    
    sprintf ((char *)dest, "%lu", lValue);
}

//////////////////////////////////////////////////////
//    ��Bcd��ת����Ascii�� (������������ַ�)       //
//        �����������ַ�ʱֱ�ӽض�                  //
//////////////////////////////////////////////////////
void  ConvertMgr::bcdToAscii_Left_D (BIT8 * src, BIT8 * dest, int length)
{
    // length ���ֽ���
    BIT8 temp,l;
    int    iLen = length * 2;

    for (int i=0; i<iLen; i++)
    {
        temp = *src;
        
        if (i%2 == 0){
            l = (temp>>4) + 0x30;
            if (l>'9') break;
            *dest++ = l;
        }
        else
        {
            temp = temp<<4;
            l = (temp>>4) + 0x30;
            if (l>'9') break;
            *dest++ = l;
            src ++;
        }
    }
    *dest = (BIT8)NULL;
}

//////////////////////////////////////////////////////
//    ��Bcd��ת����Ascii��   (����������ַ�A~F)       //
//////////////////////////////////////////////////////
void ConvertMgr::bcdToAscii_Left_X (BIT8 * src,BIT8 * dest, int length)
{
    // length ���ֽ���
    BIT8   temp,l;
    int    iLen = length * 2;
    
    for (int i=0; i<iLen; i++)
    {        
        temp = *src;
        
        if (i%2 == 0) 
        {
            l = (temp>>4) + 0x30;
            if (l>'9') l += 39;
            *dest++ = l;
        }
        else
        {
            temp = temp<<4;
            l = (temp>>4) + 0x30;
            if (l>'9') l += 39;
            *dest++ = l;
            src ++;
        }
    }
    *dest = (BIT8)NULL;
}

int   ConvertMgr::getBitValue (unsigned char cValue, int _iBitPos)
{
    unsigned char cTemp = cValue;
    
    if (_iBitPos<1 || _iBitPos>8) return 0;
    
    cTemp <<= (long)(_iBitPos-1);
    
    if (cTemp & 0x80)
        return 1;
    
    return 0;
}

