#ifndef _LYNN_MASTER_TASK_H_
#define _LYNN_MASTER_TASK_H_

#include <boost/shared_ptr.hpp>
#include <classad/classad.h>

#include "include/proxy.h"
#include "include/block_queue.h"
#include "include/singleton.h"
#include "include/rwlock.h"

using boost::shared_ptr;
using lynn::BlockQueue;
using lynn::RWLock;

enum TaskState {
    TASK_WAIT,
    TASK_RUN,
};

class Task {
public:
    ~Task() throw() {};
    Task(const TaskInfo& task_info);
    int64_t GetId(){
        return m_task_info.id;
    }
    string GetVCName() {
        return m_task_info.vc_name;
    }
    TaskState GetTaskState();
    void SetTaskState(TaskState ts);
    bool AssignTask();
    string GetRunOn(){
        return m_run_on;
    }
    TaskInfo GetTaskInfo(){
        return m_task_info;
    }
private:
    RWLock m_lock;
    TaskState m_state;
    ClassAd m_ad;
    string m_run_on;
    TaskInfo m_task_info;
};

typedef shared_ptr<Task> TaskPtr;
typedef Singleton< BlockQueue<TaskPtr> > TaskBufferI;

#endif
