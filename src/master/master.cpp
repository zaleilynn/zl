#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/thread.hpp>

#include "include/rpc.h"
#include "master/config.h"
#include "master/service.h"
#include "master/ip_pool.h"
#include "master/watcher.h"

extern void* TaskProcessor();
extern void* SchedulerProcessor();
extern void* StateEventProcessor();

using log4cplus::Logger;
using log4cplus::ConsoleAppender;
using log4cplus::Appender;
using log4cplus::Layout;
using log4cplus::PatternLayout;
using log4cplus::helpers::SharedObjectPtr;
using boost::thread;
using std::auto_ptr;
using std::string;

int32_t main(int argc, char ** argv) {
    int result;
    bool debug = false;
    while((result = getopt(argc, argv, "d")) != -1) {
        switch(result) {
        case 'd':
            debug = true;
            break;
        default:
            fprintf(stderr, "unknown option: %c\n", (char)optopt);
            break;
        }
    }
    SharedObjectPtr<Appender> append(new ConsoleAppender());
    append->setName(LOG4CPLUS_TEXT("append for master"));
    auto_ptr<Layout> layout(new PatternLayout(LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S} %p %l:%m %n")));
    append->setLayout(layout);
    Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("master"));
    logger.addAppender(append);

    if(debug) {
        logger.setLogLevel(log4cplus::DEBUG_LOG_LEVEL);
    } else {
        logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
    }

    char *p = getenv("LYNN_HOME");
    if( NULL == p) {
        LOG4CPLUS_ERROR(logger, "LYNN_HOME is not set");
        exit(1);
    }

    string zlynn_home(p);
    string conf_file = zlynn_home + "/conf/master.xml";
    if(MasterConfigI::Instance()->Init(conf_file) != 0) {
        LOG4CPLUS_ERROR(logger, "error in reading master config");
        exit(1);
    }
   
    if(IpPoolI::Instance()->Init() != 0) {
        LOG4CPLUS_ERROR(logger, "error in init ip pool");
        exit(1);
    }

    thread task_processor_t(TaskProcessor);
    thread scheduler_processor_t(SchedulerProcessor);
    thread state_event_processor_t(StateEventProcessor);
   
    //启动Watcher线程
    WatcherI::Instance()->Start(); 
   
    Rpc<MasterService, MasterProcessor>::Listen(atoi(MasterConfigI::Instance()->Get("port").c_str()));
    return 0;
}
