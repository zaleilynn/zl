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
                  << " vcpu:" << m_task_info.cpu
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
    rtn.cpu = m_task_info.cpu;
    rtn.memory = m_task_info.memory;
    rtn.ip = m_task_info.ip;
    rtn.IO = m_task_info.IO;
    rtn.app = m_task_info.app;
    return rtn;
}

void Executor::Start() {
    ExecutorStarted();
    VMPtr ptr(new VM(GetVMInfo()));  
    if(ptr->Init() == 0) {
        if(ptr->Execute() == 0) {
            //启动成功
            ptr->VMStarted();
            ExecutorRunned();
            VMPoolI::Instance()->Insert(ptr);
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
    VMPoolI::Instance()->DeleteByTaskId(m_task_info.id); 
    WriteLocker locker(m_lock);
    m_state = EXECUTOR_FINISH; 
}

void Executor::ExecutorStarted(){
    WriteLocker locker(m_lock);
    m_state = EXECUTOR_START;
}

void Executor::ExecutorFailed(){
    WriteLocker locker(m_lock);
    m_state = EXECUTOR_FAIL;
}

void Executor::ExecutorFinished(){
    WriteLocker locker(m_lock);
    m_state = EXECUTOR_FINISH;
}

void Executor::ExecutorRunned() {
    WriteLocker locker(m_lock);
    m_state = EXECUTOR_RUN;
}
