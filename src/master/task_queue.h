#ifndef _LYNN_MASTER_TASK_QUEUE_H_
#define _LYNN_MASTER_TASK_QUEUE_H_

#include <list>
#include <string>
#include "master/task.h"
#include "include/rwlock.h"
    
using std::list;
using std::string;
using lynn::RWLock;

class TaskQueue {
public:
    void PushBack(const TaskPtr& task);
    TaskPtr PopFront();
    bool Erase(int64_t task_id);
    int32_t Size();
private:
    RWLock m_lock;
    list<TaskPtr> m_list;
};

#endif
