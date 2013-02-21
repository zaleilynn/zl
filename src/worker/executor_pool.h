#ifndef _LYNN_WORKER_EXECUTOR_POOL_H_
#define _LYNN_WORKER_EXECUTOR_POOL_H_

#include <map>
#include <tr1/memory>
#include <tr1/functional>

#include "include/rwlock.h"
#include "include/singleton.h"
#include "worker/executor.h"

using std::map;
using std::tr1::function;
using lynn::RWLock;

class ExecutorPool {
public:
    typedef function<void(ExecutorPtr)> ExecutorFunc;
    void Insert(const ExecutorPtr& ptr);
    void Delete(int64_t id);
    void StartExecutor();
    bool DeleteExecutor(int64_t id);
    bool FindToDo(int64_t id, ExecutorFunc func);
private:
    RWLock m_lock;
    map<int64_t, ExecutorPtr> m_executor_map;
};

typedef Singleton<ExecutorPool> ExecutorPoolI;

#endif
