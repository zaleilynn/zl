#ifndef _LYNN_SRC_WORKER_SERVICE_H_
#define _LYNN_SRC_WORKER_SERVICE_H_

#include <string>
#include "include/proxy.h"

using std::string;

class WorkerService : public WorkerIf {
public:
    bool StartTask(const TaskInfo& task_info);
    bool KillTask(int64_t task_id);
};

#endif

