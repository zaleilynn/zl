#ifndef _LYNN_WORKER_EXECUTOR_H_
#define _LYNN_WORKER_EXECUTOR_H_

#include <boost/shared_ptr.hpp>
#include "include/rwlock.h"
#include "include/proxy.h"

using std::string;
using boost::shared_ptr;
using lynn::RWLock;


enum ExecutorState {
    EXECUTOR_WAIT,
    EXECUTOR_RUN,
    EXECUTOR_FINISH,
    EXECUTOR_FAIL,
};

class Executor {
public:
    explicit Executor(const TaskInfo& info);
    int64_t GetId() {
        return m_task_info.id;
    }
    void LogInfo();
    ExecutorState GetStatus();
    void Start();
    void Kill();
private:
    RWLock m_lock;
    TaskInfo m_task_info;
    ExecutorState m_state; 
};

typedef shared_ptr<Executor> ExecutorPtr;

#endif
