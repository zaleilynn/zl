#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "worker/executor.h"
#include "worker/vm.h"

using log4cplus::Logger;
using lynn::ReadLocker;
using lynn::WriteLocker;

static Logger logger = Logger::getInstance("worker");

Executor::Executor(const TaskInfo& info){
    m_task_info = info;
    m_state = EXECUTOR_WAIT;
}

void Executor::LogInfo(){
    LOG4CPLUS_INFO(logger, "new task :" << m_task_info.id 
                  << " virtual cluster:" << m_task_info.vc_name 
                  << " vcpu:" << m_task_info.need_vcpu
                  << " memory:" << m_task_info.need_memory);
}

ExecutorState Executor::GetStatus() {
    ReadLocker locker(m_lock);
    return m_state;
}

void Executor::Start() {
    WriteLocker locker(m_lock);
    VMPtr ptr(new VM(m_task_info));  
    if(ptr->Init() == 0) {
    } else {

    } 
}

void Executor::Kill() {
    WriteLocker locker(m_lock);  
}
