/*VER: 1*/ 
#include "TicketTlv.h"


// ��ǰTicketTlv ����tagλ�������
void TicketTlv::reset ()
{
    for (int i=0; i<=MAX_TLV_FLAGS; i++)
        resetField (i);
}

// ����tagֵ��ȡ��Ӧ���ֶ�ֵ, Tlv������Ч����, �ֶγ���
// _iTag      : TAG��ʶ
// pValue     : ���ص��ֶ�ֵ�ĵ�ַ
// piTlvLen   : Tlv������Ч����
bool TicketTlv::getTagValue (int _iTag, unsigned char *pValue, int &iTlvLen)
{
    if (_iTag<0 || _iTag>MAX_TLV_FLAGS) return false;
    
    if (!m_lAddress[_iTag] || !m_iTlvLen[_iTag]) return false;
        
    memcpy (pValue, (void *)m_lAddress[_iTag], m_iTlvLen[_iTag]);
    
    iTlvLen = m_iTlvLen[_iTag];
    
    return true;
}

// ��Tagλ��ֵ
bool TicketTlv::setTagValue (int _iTag, void *pValue, int _iTlvLen)
{
    if (_iTag<0 || _iTag>MAX_TLV_FLAGS) return false;
    
    ajustTagLen (_iTag, _iTlvLen);
     
    memcpy ((void *)m_lAddress[_iTag], pValue, _iTlvLen);
    
    m_iTlvLen[_iTag] = _iTlvLen;
    
    return true;
}


// ����һ��Tag�ֶνڵ�  return: 0--OK, -1--Malloc Err, 1--Tagλ���Ϸ�
int TicketTlv::mallocField (int _iTag, int _iLen) 
{
    if (_iTag<0 || _iTag>MAX_TLV_FLAGS )  return 1;
    
    if (_iLen > MAX_FIELD_LEN) return -1;
    
    m_lAddress[_iTag] = (unsigned long)malloc (_iLen+1);
    
    if (!m_lAddress[_iTag]) THROW (100100001);
    
    m_iFieldLen[_iTag] = _iLen;
    
    resetField (_iTag);
    
    return 0;
}

// У��,����Tagλ�ֶ�, ����ʵ�ʳ���(_iTlvLen)����Ҫ
void TicketTlv::ajustTagLen (int _iTag, int _iTlvLen)
{
    if (_iTag<0 || _iTag>MAX_TLV_FLAGS) return;
    
    // �ֶ���δ����
    if (!m_lAddress[_iTag]) {
        mallocField (_iTag, _iTlvLen);
        return;
    }
    
    // ��鵱ǰ�ֶγ����Ƿ�����ʵ����Ҫ
    if (m_iFieldLen[_iTag] >= _iTlvLen) return;
            
    free ((void *)m_lAddress[_iTag]);
    
    mallocField (_iTag, _iTlvLen);
}

// ָ��tagλ�������
void TicketTlv::resetField (int _iTag)
{
    if (_iTag<0 || _iTag>MAX_TLV_FLAGS) return;
    
    if (!m_lAddress[_iTag]) return;
    
    memset ((void *)m_lAddress[_iTag], 0, m_iFieldLen[_iTag]);
    
    m_iTlvLen[_iTag] = 0;
}
