#ifndef _LYNN_WORKER_RESOURCE_MANAGER_H_
#define _LYNN_WORKER_RESOURCE_MANAGER_H_

#include <string>
#include <vector>
#include "include/singleton.h"
#include "include/proxy.h"
#include "include/thread.h"

#include "worker/vm_pool.h"

using std::string;
using std::vector;

class ResourceManager {
public:
   int32_t Init();
   void GetMachineInfo(MachineInfo& info);
   string GetEndpoint(){
       return m_endpoint;
   }
   void GetAvailableResource(const VMPtr& ptr);
   void GetExecutorResourceInfo(const VMPtr& ptr, MachineInfo* info);
   void Start();
   void Entry();
private:
   RWLock m_lock;
   string m_endpoint;
   int32_t m_avail_cpu;
   int32_t m_avail_memory;
   int32_t m_all_cpu;
   int32_t m_all_memory;
   double m_cpu_usage;
};

typedef Singleton<ResourceManager> ResourceManagerI;
#endif

