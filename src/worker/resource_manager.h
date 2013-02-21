#ifndef _LYNN_WORKER_RESOURCE_MANAGER_H_
#define _LYNN_WORKER_RESOURCE_MANAGER_H_

#include <string>
#include "include/singleton.h"
#include "include/proxy.h"

using std::string;

class ResourceManager {
public:
   int32_t Init();
   int32_t GetMachineInfo(MachineInfo& info);
private:
   string m_endpoint;
   int32_t m_avail_vcpu;
   int32_t m_avail_memory;
   int32_t m_all_vcpu;
   int32_t m_all_memory;
};

typedef Singleton<ResourceManager> ResourceManagerI;
#endif

