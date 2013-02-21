#include "master/task_pool.h"

using lynn::ReadLocker;
using lynn::WriteLocker;

void TaskPool::Insert(const TaskPtr& task){
    WriteLocker locker(m_lock);
    m_task_pool[task->GetId()] = task;
}
