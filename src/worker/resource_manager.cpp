#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "include/system.h"

#include "worker/resource_manager.h"
#include "worker/config.h"

using log4cplus::Logger;
using std::tr1::bind;
using lynn::ReadLocker;
using lynn::WriteLocker;

static Logger logger = Logger::getInstance("worker");

int32_t ResourceManager::Init(){
    m_endpoint = System::GetIP(WorkerConfigI::Instance()->Get("interface"))
                  + ":" + WorkerConfigI::Instance()->Get("port");
    m_all_cpu = System::CpuNum();
    m_all_memory = System::TotalMemory();
    m_avail_cpu = m_all_cpu;
    m_avail_memory = m_all_memory; 
    return 0;
}

void ResourceManager::GetMachineInfo(MachineInfo& info) {
    m_cpu_usage = System::CpuUsage();
    m_avail_memory = m_all_memory;
    m_avail_cpu = m_all_cpu;
  
    info.endpoint = m_endpoint;
    info.cpu_usage = m_cpu_usage; 
    info.all_memory = m_all_memory;
    info.all_cpu = m_all_cpu;

    VMPool::VMFunc func = bind(&ResourceManager::GetAvailableResource, this, _1);
    VMPoolI::Instance()->MapToDo(func);
    info.avail_cpu = m_avail_cpu;
    info.avail_memory = m_avail_memory;   

    func = bind(&ResourceManager::GetExecutorResourceInfo, this, _1, &info);
    VMPoolI::Instance()->MapToDo(func);    
}

void ResourceManager::GetAvailableResource(const VMPtr& ptr) {
    if(ptr->GetState() == VM_RUN) {
        m_avail_cpu -= ptr->GetAllocatedCpu();
        m_avail_memory -= ptr->GetAllocatedMemory();
    } 
}

void ResourceManager::GetExecutorResourceInfo(const VMPtr& ptr, MachineInfo* info) {
    if(ptr->GetState() == VM_RUN) {
       info->vm_list.push_back(ptr->GetUsedResource()); 
    }
}
