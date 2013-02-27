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
    //这个不能有返回值，使用比较受限制
    typedef function<void(const TaskPtr&)> TaskFunc;
    void Insert(const TaskPtr& task);
    int32_t FindToDo(int64_t task_id, TaskFunc func);
    void Delete(int64_t task_id);
private:
    RWLock m_lock;
    map<int64_t, TaskPtr> m_task_pool;
};

typedef Singleton<TaskPool> TaskPoolI;
#endif
