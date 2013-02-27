#ifndef _LYNN_MASTER_VIRTUAL_CLUSTER_H_
#define _LYNN_MASTER_VIRTUAL_CLSUTER_H_

#include <string>
#include <boost/shared_ptr.hpp>

#include "include/proxy.h"
#include "master/task_queue.h"

using std::string;

using boost::shared_ptr;

class VC {
public:
    ~VC() throw() {}
    VC(const VCInfo& info);
    void Init();
    void LogInfo();
    string GetVCName() {
        return m_vc_info.name;
    }
    TaskInfo GetTaskInfo();
    TaskPtr PopTask(TaskState type);
    void PushTask(const TaskPtr& task);
    void RemoveTask(const TaskPtr& task); 
private:
    VCInfo m_vc_info;
    TaskInfo m_task_info;
    TaskQueue m_wait_queue;
    TaskQueue m_run_queue;
};

typedef shared_ptr<VC> VCPtr;
#endif
