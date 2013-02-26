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

int32_t ResourceManager::GetMachineInfo(MachineInfo& info) {
    info.endpoint = m_endpoint;
    info.cpu_usage = System::CpuUsage(); 
    info.all_memory = m_all_memory;
    info.avail_memory = m_avail_memory;
    info.all_vcpu = m_all_vcpu;
    info.avail_vcpu = m_avail_vcpu;
    return 0;
}
