#include "master/task_queue.h"

using lynn::ReadLocker;
using lynn::WriteLocker;

int32_t TaskQueue::Size() {
    ReadLocker locker(m_lock);
    return m_list.size();
}

void TaskQueue::PushBack(const TaskPtr& task) {
    WriteLocker locker(m_lock);
    m_list.push_back(task);
}

TaskPtr TaskQueue::PopFront() {
    WriteLocker locker(m_lock);
    if(m_list.size() <= 0) {
       return TaskPtr();
    }
    TaskPtr ptr = m_list.front();
    m_list.pop_front();
    return ptr;
}

bool TaskQueue::Erase(int64_t task_id) {
    WriteLocker locker(m_lock);
    for (list<TaskPtr>::iterator it = m_list.begin(); it != m_list.end(); ++it)
        // find the task
        if (task_id == (*it)->GetId()) {
            m_list.erase(it);
            return true;
        }
    // not found return false
    return false;
}

