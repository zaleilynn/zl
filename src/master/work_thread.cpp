#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "master/task_pool.h"
#include "master/vc_pool.h"
#include "master/event.h"

using log4cplus::Logger;

static Logger logger = Logger::getInstance("master");

//只有这一个线程操作task结构，故可以直接返回指针，不考虑同步问题
void* SchedulerProcessor() {
    while(true) { 
        //这句话是调度的核心关键
        //非阻塞队列
        TaskPtr task = VCPoolI::Instance()->GetTask();
        if(task) {
           LOG4CPLUS_INFO(logger, "find a new task to schedule");
           if(task->AssignTask()) {
               LOG4CPLUS_INFO(logger, "Task " << task->GetId() 
               << " for virtual cluster " << task->GetVCName()
               << " run on " << task->GetRunOn()); 
           } else {
               LOG4CPLUS_WARN(logger, "scheduler task error");
               //可以考虑重新进行调度
               //VCPoolI::Instance()->AddTask(task);
           }
        }
        usleep(1000 * 50); 
    }
    return NULL; 
}

//这个处理task提交时候的缓冲区的
void* TaskProcessor() {
    while(true) {
        TaskPtr task;
        TaskBufferI::Instance()->PopFront(&task);
        LOG4CPLUS_INFO(logger, "receive a task");
        VCPoolI::Instance()->AddTask(task);
    }
    return NULL;
}

void* StateEventProcessor() {
    while(true) {
        StateEventPtr event;
        StateEventBufferI::Instance()->PopFront(&event);
        if(event->Handle() != 0) {
            LOG4CPLUS_ERROR(logger, "process state error");
        }
    }
    return NULL;
}
