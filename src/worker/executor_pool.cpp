#include "worker/executor_pool.h"

using lynn::WriteLocker;
using lynn::ReadLocker;
using std::tr1::placeholders::_1;

void ExecutorPool::Insert(const ExecutorPtr& ptr){
    WriteLocker locker(m_lock);
    m_executor_map[ptr->GetId()] = ptr;
}

void ExecutorPool::Delete(int64_t id) {
    WriteLocker locker(m_lock);
    m_executor_map.erase(id);
}

void ExecutorPool::StartExecutor() {
    ReadLocker locker(m_lock);
    for(map<int64_t, ExecutorPtr>::iterator it = m_executor_map.begin();
        it != m_executor_map.end(); ++it) {
        if((it->second)->GetStatus() == EXECUTOR_WAIT) {
            (it->second)->Start();
            return;
        }
    }
}

bool ExecutorPool::FindToDo(int64_t id, ExecutorFunc func) {
    ReadLocker locker(m_lock);
    map<int64_t, ExecutorPtr>::iterator it = m_executor_map.find(id);
    if(it != m_executor_map.end()) {
        func(it->second);
        return true;
    }
    return false;
}

bool ExecutorPool::DeleteExecutor(int64_t id){
    ExecutorFunc func = bind(&Executor::Kill, _1);
    if(FindToDo(id, func)) {
        Delete(id);
        return true;
    }   
    return true;
}
