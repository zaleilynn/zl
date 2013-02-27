#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>


#include "master/vc_pool.h"
#include "master/task_pool.h"

using log4cplus::Logger;

static Logger logger = Logger::getInstance("master");

VC::VC(const VCInfo& info) : m_vc_info(info) {
    m_task_info.vc_name = info.name;
    //提交任务时，这个id会被赋值
    m_task_info.id = -1;
    m_task_info.os = info.os;
    m_task_info.vcpu = info.vcpu;
    m_task_info.memory = info.memory; 
}

void VC::LogInfo() {
    LOG4CPLUS_INFO(logger, "add virtual cluster"
                    << " name: " << m_vc_info.name
                    );
}

void VC::Init() {
   TaskPtr task(new Task(m_task_info));
   LOG4CPLUS_INFO(logger, "add a task from vc:" << m_vc_info.name);
   TaskPoolI::Instance()->Insert(task);
   m_wait_queue.PushBack(task);
}

TaskInfo VC::GetTaskInfo(){
    return m_task_info;
}

//根据状态插入到相应队列中
void VC::PushTask(const TaskPtr& task) {
    TaskState ts = task->GetTaskState(); 
    assert(ts == TASK_WAIT || ts == TASK_RUN);
    if(ts == TASK_WAIT) 
        m_wait_queue.PushBack(task);
    else 
        m_run_queue.PushBack(task);
}

void VC::RemoveTask(const TaskPtr& task) {
    TaskState ts = task->GetTaskState();
    assert(ts == TASK_WAIT || ts == TASK_RUN);
    if(ts == TASK_WAIT) {
        m_wait_queue.Erase(task->GetId());    
    } else {
        m_run_queue.Erase(task->GetId());
    }
}

TaskPtr VC::PopTask(TaskState type) {
    assert(type == TASK_WAIT || type == TASK_RUN);
    if(type == TASK_WAIT) {
        return m_wait_queue.PopFront();
    } else {
        return m_run_queue.PopFront();
    }
}
