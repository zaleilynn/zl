#include <vector>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include <boost/thread.hpp>

#include "include/proxy.h"
#include "include/rpc.h"

#include "worker/config.h"
#include "worker/resource_manager.h"
#include "worker/executor_pool.h"
#include "worker/event.h"

using log4cplus::Logger;
using std::vector;

static Logger logger = Logger::getInstance("worker");

//搞一个MachineInfo wrapper是否可以让这些函数都封装进去？:
void LogHeartbeat(MachineInfo info) {
     LOG4CPLUS_DEBUG(logger, "a heartbeat, machine:" << info.endpoint
                              << " vm num: " << info.vm_list.size());
     for(vector<ExecutorStat>::iterator it = info.vm_list.begin(); 
         it != info.vm_list.end(); it++) {
         LOG4CPLUS_DEBUG(logger, "vm id:" << it->task_id <<
                                 " cpu usage:" << it->cpu_usage);
     }   
}

//获得资源利用率利用另外一个线程
//这个线程只负责发心跳
void* HeartbeatProcessor() {
    while(true) {
       MachineInfo info;
       ResourceManagerI::Instance()->GetMachineInfo(info);
       LogHeartbeat(info);
       try {
           Proxy<MasterClient> proxy = Rpc<MasterClient, MasterClient>::GetProxy(WorkerConfigI::Instance()->Get("master_endpoint"));
           proxy().Heartbeat(info);
       } catch (TException &tx) {
           LOG4CPLUS_ERROR(logger, "send heartbeat error: " << tx.what());
       }
       sleep(atoi(WorkerConfigI::Instance()->Get("heartbeat_interval").c_str()));
    }
    return NULL;
}

void* SchedulerProcessor(){
    while(true) {
        ExecutorPoolI::Instance()->StartExecutor();
        usleep(1000*100);
    }
    return NULL;
}

void* StateEventProcessor(){
    while(true) {
        EventPtr event;
        EventBufferI::Instance()->PopFront(&event);  
        //这句话怎么有时候会引起段错误？
        //非确定性BUG？
        //LOG4CPLUS_DEBUG(logger, "catch a event");
        if(event->Handle() != 0) {
            LOG4CPLUS_ERROR(logger, "process state error"); 
        }
    }
    return NULL;
}
