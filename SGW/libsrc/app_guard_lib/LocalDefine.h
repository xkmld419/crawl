#ifndef __LOCAL_DEFINE__

// ���ļ�����ά��,�����ύ���ÿ�. �޸ĺ���Ҫ���±�����Ч

///////////////////////////////////////////////////////////////////////////////////////////
// ��ⲿ�ֺ궨��

    #define EVENT_AGGR_INDEX_NUM     1            //�¼��ۻ������ĸ���(SERV_ID)
    #define ACCTITEM_AGGR_INDEX_NUM  1            //��Ŀ�ۻ������ĸ���(SERV_ID)
    #define INDEX__SERV_ID           0                //��һ����SERV_ID
    #define ADD_AGGR_MAX_TABLE_NUM  16            //һ���Ʒ��������������ɵ������ۻ���ĸ���
    
    #define EVENT_AGGR_DATA_COUNT        20000000        //�¼��ۻ����ݶμ�¼��
    #define EVENT_ADD_AGGR_DATA_COUNT    800000            //�¼������ۻ����ݶμ�¼��
    #define ACCTITEM_AGGR_DATA_COUNT     50000000        //��Ŀ�ۻ����ݶμ�¼��
    #define ACCTITEM_ADD_AGGR_DATA_COUNT 400000        //��Ŀ�����ۻ����ݶμ�¼��
    #define EVENT_AGGR_INDEX_COUNT       3000000            //�¼��ۻ������μ�¼��
    #define EVENT_ADD_AGGR_INDEX_COUNT   400000            //�¼������ۻ������μ�¼��
    #define ACCTITEM_AGGR_INDEX_COUNT    4000000            //��Ŀ�ۻ������μ�¼��
    #define ACCTITEM_ADD_AGGR_INDEX_COUNT    400000        //��Ŀ�����ۻ������μ�¼��
    #define MAX_EVENT_AGGR_TYPE_NUM_OfPerEvent 5        //һ���¼������ܴ��ڵ��¼��ۻ���������
    
    #define EVENT_AGGR_ID_INCREATE_SIZE  10000        //�¼��ۻ���ʶ����ÿ��increate�Ĵ�С
                                                     //-- ���������ݿ���EVENT_AGGR_ID_SEQ��increateֵһ��
    #define ACCT_ITEM_ID_INCREATE_SIZE   10000        //��Ŀ��ʶ����ÿ��increate�Ĵ�С
                                                     //-- ���������ݿ���ACCT_ITEM_ID_SEQ��increateֵһ��
                                
    #define MAX_END_FILE            100000            //һ����������������ļ�����
    #define MAX_ACCT_ITEM_PER_EVENT        200            //һ���¼���¼�����ܲ�������Ŀ��¼
    #define TransactionCommit_EventNum    7000        //�����ύ��С
    #define EVENT_MAX_EXTEND_ATTR_NUM    10            //�¼�������չ���Ը���
    #define MAX_STDEVENT_TABLE_NUM        5            //һ���ύ�������ܲ����ı�׼�¼��������
    #define MAX_EXTEVENT_TABLE_NUM        5            //һ���ύ�������ܲ�������չ�¼��������
    #define MAX_REDOEVENT_TABLE_NUM        5            //һ���ύ�������ܲ����Ļ����¼��������
    #define MAX_NOOWNEREVENT_TABLE_NUM    5            //һ���ύ�������ܲ����������¼��������

    #define MAX_OTHEREVENT_TABLE_NUM    1        //һ���ύ�������ܲ�����other�¼��������
    #define MAX_ROLLBACK_TABLE_NUM        1        //һ���ύ�������ܲ�����rollback�¼��������
///////////////////////////////////////////////////////////////////////////////////////////


///////////////////
// ����ػ�ģ��궨��
#define MAX_APP_NUM 2048   //����ϵͳ

///////////////////

#endif




