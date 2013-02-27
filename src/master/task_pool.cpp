#include "master/task_pool.h"

using lynn::ReadLocker;
using lynn::WriteLocker;

void TaskPool::Insert(const TaskPtr& task){
    WriteLocker locker(m_lock);
    m_task_pool[task->GetId()] = task;
}

//指针最好不要开放出去，太容易出错了
int32_t TaskPool::FindToDo(int64_t task_id, TaskFunc func) {
    ReadLocker locker(m_lock);
    map<int64_t, TaskPtr>::iterator it = m_task_pool.find(task_id);
    if (it != m_task_pool.end()) {
        func(it->second);
        return 0;
    }
    return 1;
}

void TaskPool::Delete(int64_t task_id) {
    WriteLocker locker(m_lock);
    m_task_pool.erase(task_id);
}
