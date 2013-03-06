#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "worker/service.h"
#include "worker/executor_pool.h"

using log4cplus::Logger;

static Logger logger = Logger::getInstance("worker");

bool WorkerService::StartTask(const TaskInfo& info) {
    LOG4CPLUS_INFO(logger, "start task :" << info.id);
    ExecutorPtr ptr(new Executor(info));
    ptr->LogInfo();
    ExecutorPoolI::Instance()->Insert(ptr);
    return true;
}

bool WorkerService::KillTask(int64_t task_id){
    //不弄个缓冲类似的什么东西? 
    return ExecutorPoolI::Instance()->DeleteExecutor(task_id);
}
