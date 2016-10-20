/*VER: 1*/ 
#ifndef __TICKET_TLV_H__
#define __TICKET_TLV_H__

#include <string.h>
#include <stdlib.h>

//���֧�ֵ�TAG��ʶ
#define MAX_TLV_FLAGS 1024

//һ��tagλ���ݵ���󳤶�(�ֽ�)
#define MAX_FIELD_LEN 200

#include "CommonMacro.h"
#include "Exception.h"

//## Tlv �����洢��ʽ(��̬����)
class TicketTlv {
  public:
    TicketTlv ()
    {
        memset (m_lAddress,  0, sizeof(unsigned long)*(MAX_TLV_FLAGS+1));
        memset (m_iFieldLen, 0, sizeof(int)*(MAX_TLV_FLAGS+1));
        memset (m_iTlvLen,   0, sizeof(int)*(MAX_TLV_FLAGS+1));
    }
    
    ~TicketTlv () 
    {
        for (int i=0; i<=MAX_TLV_FLAGS; i++) {
            if (m_lAddress[i])
                free ((void *)m_lAddress[i]);
        }
    }
        
    // ��ǰTicketTlv ����tagλ�������
    void reset ();

    // ����tagֵ��ȡ��Ӧ���ֶ�ֵ, Tlv������Ч����, �ֶγ���
    // _iTag      : TAG��ʶ
    // pValue     : ���ص��ֶ�ֵ�ĵ�ַ
    // piTlvLen   : Tlv������Ч����
    bool getTagValue (int _iTag, unsigned char *pValue, int &iTlvLen);

    // ��Tagλ��ֵ
    bool setTagValue (int _iTag, void *pValue, int _iTlvLen);
        
  private:
    // ����һ��Tag�ֶνڵ�  return: 0--OK, -1--Malloc Err, 1--Tagλ���Ϸ�
    int mallocField (int _iTag, int _iLen) ;
    
    // У��,����Tagλ�ֶ�, ����ʵ�ʳ���(_iTlvLen)����Ҫ
    void ajustTagLen (int _iTag, int _iTlvLen);

    // ָ��tagλ�������
    void resetField (int _iTag);
    
  private:  
    //��¼ÿ��tagλ��Ӧ�ĵ�ַ, tag_N ��Ӧ m_Address[N]
    unsigned long m_lAddress[MAX_TLV_FLAGS+1];
    
    //��¼ÿ��tagλ������ڴ��С(�ֽ���)
    int           m_iFieldLen[MAX_TLV_FLAGS+1];
    
    //��¼ÿ��tagλ��ǰ����ʵ����Ч����(�ɼ��˷���)
    int           m_iTlvLen[MAX_TLV_FLAGS+1];
};


#endif
