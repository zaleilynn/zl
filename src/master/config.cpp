#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "master/config.h"

using log4cplus::Logger;

static Logger logger = Logger::getInstance("master");

int32_t MasterConfig::CheckCompleteness() {
    const char* s[] = {"port",
                       "net",
                       "heartbeat_interval", 
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
