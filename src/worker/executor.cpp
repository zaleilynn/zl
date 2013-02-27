#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "worker/executor.h"
#include "worker/vm_pool.h"

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
                  << " vcpu:" << m_task_info.vcpu
                  << " memory:" << m_task_info.memory);
}

ExecutorState Executor::GetStatus() {
    ReadLocker locker(m_lock);
    return m_state;
}

VMInfo Executor::GetVMInfo() {
    VMInfo rtn;
    rtn.id = m_task_info.id; 
    rtn.vc_name = m_task_info.vc_name;
    rtn.os = m_task_info.os;
    rtn.vcpu = m_task_info.vcpu;
    rtn.memory = m_task_info.memory;
    rtn.ip = m_task_info.ip;
    return rtn;
}

void Executor::Start() {
    ExecutorStarted();
    VMPtr ptr(new VM(GetVMInfo()));  
    if(ptr->Init() == 0) {
        LOG4CPLUS_INFO(logger, "vm init succeed");
        if(ptr->Execute() == 0) {
            //启动成功
            VMPoolI::Instance()->Insert(ptr);
            ptr->VMStarted();
            ExecutorFinished();
            LOG4CPLUS_INFO(logger, "vm start succeed");
        } else {
            ptr->VMFailed();
            ExecutorFailed(); 
            LOG4CPLUS_ERROR(logger, "start vm failed");
        }
    } else {
        ptr->VMFailed();
        ExecutorFailed(); 
        LOG4CPLUS_ERROR(logger, "init vm env failed");
    } 
}

void Executor::Kill() {
    WriteLocker locker(m_lock); 
}

void Executor::ExecutorStarted(){
    WriteLocker locker(m_lock);
    m_state = EXECUTOR_RUN;
}

void Executor::ExecutorFailed(){
    WriteLocker locker(m_lock);
    m_state = EXECUTOR_FAIL;
}

void Executor::ExecutorFinished(){
    WriteLocker locker(m_lock);
    m_state = EXECUTOR_FINISH;
}
