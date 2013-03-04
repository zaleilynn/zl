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

