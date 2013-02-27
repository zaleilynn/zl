#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "worker/event.h"
#include "worker/config.h"
#include "include/rpc.h"
#include "include/proxy.h"

using log4cplus::Logger;

static Logger logger = Logger::getInstance("worker");

int32_t StartedEvent::Handle() {
    string master_endpoint = WorkerConfigI::Instance()->Get("master_endpoint");
    if(master_endpoint.empty()) {
        LOG4CPLUS_ERROR(logger, "cannot get master_endpoint");
        return 1;
    }

    Proxy<MasterClient> proxy = Rpc<MasterClient, MasterClient>::GetProxy(master_endpoint);
    try {
        proxy().TaskStarted(m_task_id);
    } catch (TException &tx) { 
        LOG4CPLUS_ERROR(logger, "report executor start error: " << tx.what());
    }
    return 0;
}

int32_t FinishedEvent::Handle() {
    return 0;
}

int32_t FailedEvent::Handle() {
    return 0;
}
