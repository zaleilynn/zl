#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "include/proxy.h"
#include "include/rpc.h"
#include "master/vc_pool.h"
#include "master/task_pool.h"

using log4cplus::Logger;
using lynn::ReadLocker;
using lynn::WriteLocker;
using std::tr1::bind;

static Logger logger = Logger::getInstance("master");

VC::VC(const VCInfo& info) : m_vc_info(info) {
    m_task_info.vc_name = info.name;
    //提交任务时，这个id会被赋值
    m_task_info.id = -1;
    m_task_info.os = info.os;
    m_task_info.cpu = info.cpu;
    m_task_info.memory = info.memory;
    ExecutorPoolPtr ptr(new ExecutorPool(info.name)); 
    m_executor_pool = ptr;
}

VC::~VC() throw () {
    m_trigger_list.Clear();
    delete m_thread;
}

void VC::Start() {
    TriggerPtr ptr(new OverloadTrigger);
    m_trigger_list.PushBack(ptr);
    lynn::Thread::ThreadFunc func = bind(&VC::Entry, this);
    m_thread = new lynn::Thread(func);
    m_thread->Start();
}

void VC::Entry() {
    while(true) {
        ExecutorStat stat;
        m_queue.PopFront(&stat);
        if(m_executor_pool->Size() > 0) {
            m_executor_pool->Push(stat);
            m_trigger_list.Map(m_executor_pool);  
            m_trigger_list.Flush(m_executor_pool);
        }
    }
}

void VC::LogInfo() {
    LOG4CPLUS_INFO(logger, "add virtual cluster"
                    << " name: " << m_vc_info.name
                    );
}

void VC::Init() {
   //开启线程监控虚拟集群状态
   Start();
   AddTask();
}

void VC::AddTask(){
    TaskPtr task(new Task(m_task_info));
    LOG4CPLUS_INFO(logger, "add a task from vc:" << m_vc_info.name);
    TaskPoolI::Instance()->Insert(task);
    //这里放入就不用TaskProcessor线程从TaskBuffer中取然后放入了
    m_wait_queue.PushBack(task);
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
    //里面有并发控制机制
    TaskState ts = task->GetTaskState();
    assert(ts == TASK_WAIT || ts == TASK_RUN);
    if(ts == TASK_WAIT) {
        m_wait_queue.Erase(task->GetId());    
    } else {
        m_run_queue.Erase(task->GetId());
    }
}

//这个错误处理是很麻烦的事情
void VC::KillTask(const TaskPtr& task) {
    TaskState ts = task->GetTaskState();
    assert(ts == TASK_WAIT || ts == TASK_RUN);
    if(ts == TASK_WAIT) {
        m_wait_queue.Erase(task->GetId());
    } else {  
        try {
            string endpoint = task->GetRunOn();
            Proxy<WorkerClient> proxy = Rpc<WorkerClient, WorkerClient>::GetProxy(endpoint);
            if(proxy().KillTask(task->GetId()) != 0) {
                 LOG4CPLUS_ERROR(logger, "delete task " << task->GetId() << " failed");
                 return;
            }
            m_run_queue.Erase(task->GetId());
        }  catch (TException &tx) {
            LOG4CPLUS_ERROR(logger, "delte task error " << task->GetId() << " failed");
        }
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

void VC::AddEvent(const ExecutorStat& stat) {
    if(m_executor_pool->Find(stat.task_id) == false) { 
        //其实这里有一个race condition，但是发生的概率应该是没有的
        ExecutorPtr executor_ptr(new Executor(stat.task_id));
        m_executor_pool->Insert(executor_ptr);
        TriggerPtr trigger_ptr(new IdleTrigger(stat.task_id));
        m_trigger_list.PushBack(trigger_ptr);
        LOG4CPLUS_INFO(logger, "add an executor with" 
                               << " Id: " << stat.task_id);
    }  
    m_queue.PushBack(stat);
}
