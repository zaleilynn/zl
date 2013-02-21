#include "master/vc_pool.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

using log4cplus::Logger;
using lynn::ReadLocker;
using lynn::WriteLocker;
using std::tr1::bind;

static Logger logger = Logger::getInstance("master");


//初始化一个框架,并添加一个task
int32_t VCPool::AddVC(const VCPtr& vc) {
    Insert(vc);
    vc->Init();
    return 0;
}

void VCPool::Insert(const VCPtr& vc) {
    string name = vc->GetVCName();
    WriteLocker locker(m_lock);
    m_vc_pool[name] = vc; 
} 

TaskPtr VCPool::GetTask() {
    ReadLocker locker(m_lock);
    for(map<string, VCPtr>::iterator it = m_vc_pool.begin(); 
        it != m_vc_pool.end(); ++it) {
        TaskPtr task = it->second->PopTask(TASK_WAIT);
        if(task) 
            return task;
    }
    return TaskPtr();
}

void VCPool::AddTask(const TaskPtr& task){
    //bind 这个名字可能是socket里面的函数，用using限定一下
    VCFunc func = bind(&VC::PushTask, _1, task);
    FindToDo(task->GetVCName(), func);
}

bool VCPool::FindToDo(const string& name, VCFunc func) {
    ReadLocker locker(m_lock);
    map<string, VCPtr>::iterator it = m_vc_pool.find(name); 
    if(it != m_vc_pool.end()){
        func(it->second);
        return true;
    }
    return false;
}
