#include "worker/config.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

using log4cplus::Logger;

static Logger logger = Logger::getInstance("worker");

//检查配置是否完整
int32_t WorkerConfig::CheckCompleteness() {
    const char* s[] = {
                       "master_endpoint",
                       "port",
                       "heartbeat_interval",
                       "interface"
                      };
    int32_t size = sizeof(s)/sizeof(s[0]);
    for(int32_t i = 0; i < size; i++) {
        if(Get(s[i]).empty()) {
            LOG4CPLUS_ERROR(logger, s[i] << " is not set");
            return 1;
        }
    }
    return 0;
}
