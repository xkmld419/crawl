#ifndef __AVPLIST_H__
#define __AVPLIST_H__

#include <stdlib.h>
#include <search.h>
#include <list>
#include <vector>
#include <map>
#include <hash_map.h>
#include "diameter_parser_api.h"

struct AvpCodeInfo 
{
	AAA_AVPCode avpCode;
	std::string avpName;
};

struct AvpEnumInfo 
{
	AAA_AVPCode avpCode;
  std::vector<ACE_UINT32> vectAvpEnumValue;
};


class AvpEnumeratedInfo
{
//  friend class ACE_Singleton<AvpEnumeratedInfo_S, ACE_Null_Mutex>;
  public:
	  static AvpEnumeratedInfo * instance() 
	  { 
      if( instance_ == 0 ) 
        instance_ = new AvpEnumeratedInfo;
      return instance_ ; 
    }

	  struct eqAvpCode
    {
	    bool operator() (const AAA_AVPCode s1, const AAA_AVPCode s2) const
	    {
		    return ( s1 == s2);
	    }
    };
#ifdef DEF_HP
	  hash_map<const AAA_AVPCode, AAA_AVPCode, hash<AAA_AVPCode>, AvpEnumeratedInfo::eqAvpCode, string> avpEnumeratedHashmap;
#else
        hash_map<const AAA_AVPCode, AAA_AVPCode, hash<AAA_AVPCode>, AvpEnumeratedInfo::eqAvpCode> avpEnumeratedHashmap;
#endif

    void add(AAA_AVPCode avpCode, ACE_UINT32 avpEnumValue);
    void release();
    std::vector<AvpEnumInfo> v_item;

  private:

    AvpEnumeratedInfo();
    ~AvpEnumeratedInfo();
    AvpEnumInfo avpEnumInfo; 
    static AvpEnumeratedInfo *instance_;

};

class AAAAvpList: public std::list<AAADictionaryEntry*> 
{
  public:
	  static AAAAvpList * instance() 
	  { 
      if( instance_ == 0 ) 
        instance_ = new AAAAvpList;
      return instance_ ; 
    }

  struct eqAvpCode
    {
	    bool operator() (const AAA_AVPCode s1, const AAA_AVPCode s2) const
	    {
		    return ( s1 == s2);
	    }
    };

    struct eqAvpName
    {
	    bool operator() (const std::string& s1, const std::string& s2) const
	    {
		    return  (s1 == s2);
	    }
    };

#ifdef DEF_HP
	  hash_map<const AAA_AVPCode, AvpCodeInfo *, hash<AAA_AVPCode>, AAAAvpList::eqAvpCode, string> avpCodeHashmap;
#else
        hash_map<const AAA_AVPCode, AvpCodeInfo *, hash<AAA_AVPCode>, AAAAvpList::eqAvpCode> avpCodeHashmap;
#endif
 	
    void add(AAADictionaryEntry*);
    AAADictionaryEntry* search(const std::string&, int protocol = -1, int scenario_id = 0);
    AAADictionaryEntry* search(AAA_AVPCode, AAAVendorId, int protocol = -1, int scenario_id = 0);
    void release();

  private:
    AAAAvpList();
    ~AAAAvpList();
    std::vector<AvpCodeInfo> v_item;
//    ACE_Null_Mutex mutex;
    AvpCodeInfo avpCodeInfo; 
    static AAAAvpList *instance_;
};

//typedef ACE_Singleton<AAAAvpList_S, ACE_Null_Mutex> AAAAvpList;
//typedef ACE_Singleton<AvpEnumeratedInfo_S, ACE_Null_Mutex> AvpEnumeratedInfo;
//typedef ACE_Singleton<AAAAvpList_S, ACE_Null_Mutex> AAAAvpList;

#define AVP_HEADER_LEN(avp) \
  (avp->avpCode == 0 ? 0 : \
  (avp->flags & AAA_AVP_FLAG_VENDOR_SPECIFIC ? 12 : 8))

ACE_UINT32 getMinSize(AAADictionaryEntry*) throw (AAAErrorStatus);

#endif // __AVPLIST_H__

  


