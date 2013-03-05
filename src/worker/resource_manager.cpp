#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "include/system.h"

#include "worker/resource_manager.h"
#include "worker/config.h"

using log4cplus::Logger;

static Logger logger = Logger::getInstance("worker");

int32_t ResourceManager::Init(){
    m_endpoint = System::GetIP(WorkerConfigI::Instance()->Get("interface"))
                  + ":" + WorkerConfigI::Instance()->Get("port");
    m_all_vcpu = System::CpuNum();
    m_all_memory = System::TotalMemory();
    LOG4CPLUS_DEBUG(logger, "all memory:" << m_all_memory);
    m_avail_vcpu = m_all_vcpu;
    m_avail_memory = m_all_memory; 
    return 0;
}

void ResourceManager::GetMachineInfo(MachineInfo& info) {
    m_cpu_usage = System::CpuUsage();
    m_avail_memory = m_all_memory;
    m_avail_vcpu = m_all_vcpu;
  
    info.endpoint = m_endpoint;
    info.cpu_usage = m_cpu_usage; 
    info.all_memory = m_all_memory;
    info.all_vcpu = m_all_vcpu;

    VMPool::VMFunc func = bind(&ResourceManager::GetAvailableResource, this, _1);
    VMPoolI::Instance()->MapToDo(func);
    info.avail_vcpu = m_avail_vcpu;
    info.avail_memory = m_avail_memory;   

    func = bind(&ResourceManager::GetExecutorResourceInfo, this, _1, info);
    VMPoolI::Instance()->MapToDo(func);
}

void ResourceManager::GetAvailableResource(const VMPtr& ptr) {
    if(ptr->GetState() == VM_RUN) {
        m_avail_vcpu -= ptr->GetAllocatedVCpu();
        m_avail_memory -= ptr->GetAllocatedMemory();
    } 
}

void ResourceManager::GetExecutorResourceInfo(const VMPtr& ptr, MachineInfo& info) {
    if(ptr->GetState() == VM_RUN) {
       info.vm_list.push_back(ptr->GetUsedResource()); 
    }
}
