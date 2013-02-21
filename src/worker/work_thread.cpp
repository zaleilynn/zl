#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include <boost/thread.hpp>

#include "include/proxy.h"
#include "include/rpc.h"

#include "worker/config.h"
#include "worker/resource_manager.h"
#include "worker/executor_pool.h"

using log4cplus::Logger;

static Logger logger = Logger::getInstance("worker");

void* HeartbeatProcessor() {
    while(true) {
       MachineInfo info;
       ResourceManagerI::Instance()->GetMachineInfo(info);
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
