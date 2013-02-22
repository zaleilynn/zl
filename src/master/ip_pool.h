#ifndef _LYNN_MASTER_IP_POOL_H_
#define _LYNN_MASTER_IP_POOL_H_

#include <set>
#include <string>
#include <stdint.h>
#include "include/rwlock.h"
#include "include/singleton.h"


using std::string;
using std::set;
using lynn::RWLock;

class IpPool {
public:
    int32_t Init();
    bool AllocateIp(const string& ip);
    bool ReleaseIp(const string& ip);
    string GetNextIp();
private:
    RWLock m_lock;
    string m_prefix;
    int32_t m_start, m_end, m_cur;
    set<string> m_ip_pool;
    set<string> m_ip_to_reuse;
};

typedef Singleton<IpPool> IpPoolI;
#endif
