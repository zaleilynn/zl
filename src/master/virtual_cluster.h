#ifndef _LYNN_MASTER_VIRTUAL_CLUSTER_H_
#define _LYNN_MASTER_VIRTUAL_CLSUTER_H_

#include <string>
#include <boost/shared_ptr.hpp>

#include "include/proxy.h"
#include "include/rwlock.h"
#include "include/block_queue.h"
#include "include/thread.h"
#include "master/task_queue.h"
#include "master/machine.h"
#include "master/executor.h"
#include "master/executor_pool.h"
#include "master/trigger_queue.h"

using std::string;
using std::map;
using boost::shared_ptr;
using lynn::RWLock;

class VC {
public:
    ~VC() throw();
    VC(const VCInfo& info);
    void Init();
    void LogInfo();
    string GetVCName() {
        return m_vc_info.name;
    }
    TaskPtr PopTask(TaskState type);
    void PushTask(const TaskPtr& task);
    void RemoveTask(const TaskPtr& task);
    void AddTask();
    void DeleteTask();
    void KillTask(const TaskPtr& task);
    void AddEvent(const ExecutorStat& vm_stat);
    void Push(const ExecutorStat& stat);
    void Start();
    void Entry();
private:
    ExecutorPoolPtr m_executor_pool;
    VCInfo m_vc_info;
    TaskInfo m_task_info;
    lynn::Thread* m_thread;
    BlockQueue<ExecutorStat> m_queue;
    TaskQueue m_wait_queue;
    TaskQueue m_run_queue;
    TriggerQueue m_trigger_list;
};

typedef shared_ptr<VC> VCPtr;
#endif
