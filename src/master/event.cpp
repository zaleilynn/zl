#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "master/event.h"
#include "master/vc_pool.h"
#include "master/task_pool.h"

using log4cplus::Logger;

static Logger logger = Logger::getInstance("master");

int32_t StartEvent::Handle() {
    TaskPool::TaskFunc func = bind(&VCPool::AddTask,
                                 VCPoolI::Instance(), _1);                          if(TaskPoolI::Instance()->FindToDo(GetId(), func) != 0) {
        LOG4CPLUS_ERROR(logger, "can't find task: " << GetId()); 
        return 1;
    }
    LOG4CPLUS_INFO(logger, "task " << GetId() << " started");
    return 0;
}

int32_t FinishEvent::Handle() {
    TaskPool::TaskFunc func = bind(&VCPool::RemoveTask, 
                                    VCPoolI::Instance(), _1);
    if(TaskPoolI::Instance()->FindToDo(GetId(), func) != 0) {
        LOG4CPLUS_ERROR(logger, "can't find task: " << GetId());
        return 1;
    }
    TaskPoolI::Instance()->Delete(GetId()); 
    LOG4CPLUS_INFO(logger, "task " << GetId() << " finished");
    return 0;
}

int32_t FailEvent::Handle() {
    TaskPoolI::Instance()->Delete(GetId());
    LOG4CPLUS_ERROR(logger, "task " << GetId() << " failed");
    return 0;
}

int32_t RemoveEvent::Handle(){
    TaskPool::TaskFunc func = bind(&VCPool::KillTask,
                                    VCPoolI::Instance(), _1);
    if(TaskPoolI::Instance()->FindToDo(GetId(), func) != 0) {
        LOG4CPLUS_ERROR(logger, "can't find task: " << GetId());
        return 1;
    }
    TaskPoolI::Instance()->Delete(GetId());

    LOG4CPLUS_INFO(logger, "task " << GetId() << " Removed");
    return 0;
}


