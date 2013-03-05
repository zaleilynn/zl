#include "master/executor_pool.h"

using lynn::ReadLocker;
using lynn::WriteLocker;

bool ExecutorPool::Find(int64_t id){
    ReadLocker locker(m_lock);
    map<int64_t, ExecutorPtr>::iterator it = m_map.find(id);
    return it != m_map.end();
}

void ExecutorPool::Insert(const ExecutorPtr& executor) {
    WriteLocker locker(m_lock);
    m_map[executor->GetId()] = executor;
}

int32_t ExecutorPool::Size() {
    ReadLocker locker(m_lock);
    return m_map.size(); 
}

void ExecutorPool::Push(const ExecutorStat& stat) {
    ReadLocker locker(m_lock);
    for(map<int64_t, ExecutorPtr>::iterator it = m_map.begin();
        it != m_map.end(); ++it) {
        if(stat.task_id == it->second->GetId()) {
            it->second->AddStatInfo(stat);
        }
    }
}

void ExecutorPool::Delete(int64_t id) {
    WriteLocker locker(m_lock);
    m_map.erase(id);
}

bool ExecutorPool::IsOverLoad(int32_t period, double cpu_usage){  
    ReadLocker locker(m_lock);
    for(map<int64_t, ExecutorPtr>::iterator it = m_map.begin();
        it != m_map.end(); ++it) {
        if(!it->second->IsOverLoad(period, cpu_usage)) 
            return false;
    }  
    //需要所有的Executor都处于比较忙的情况才触发
    return true;
}

bool ExecutorPool::IsIdle(int32_t period, double value, int64_t task_id) {
    ReadLocker locker(m_lock);
    for(map<int64_t, ExecutorPtr>::iterator it = m_map.begin();
        it != m_map.end(); ++it) {
        if(task_id == it->second->GetId()) 
            return it->second->IsIdle(period, value);
    }
    return false;
}

