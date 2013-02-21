#ifndef _LYNN_MASTER_TASK_POOL_H_
#define _LYNN_MASTER_TASK_POOL_H_

#include <map>

#include <tr1/functional>

#include "include/singleton.h"
#include "include/rwlock.h"
#include "master/task.h"

using std::map;
using std::tr1::function;
using lynn::RWLock;


class TaskPool {
public:
    typedef function<void(const TaskPtr&)> TaskFunc;
    void Insert(const TaskPtr& task);
private:
    RWLock m_lock;
    map<int64_t, TaskPtr> m_task_pool;
};

typedef Singleton<TaskPool> TaskPoolI;
#endif
