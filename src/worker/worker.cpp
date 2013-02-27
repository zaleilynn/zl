#include <sys/stat.h>
#include <sys/types.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <boost/thread.hpp>

#include "include/rpc.h"
#include "worker/config.h"
#include "worker/resource_manager.h"
#include "worker/service.h"

using log4cplus::Logger;
using log4cplus::ConsoleAppender;
using log4cplus::Appender;
using log4cplus::Layout;
using log4cplus::PatternLayout;
using log4cplus::helpers::SharedObjectPtr;
using std::auto_ptr;
using std::string;
using boost::thread;

extern void* HeartbeatProcessor();
extern void* SchedulerProcessor();
extern void* StateEventProcessor();

int32_t main(int argc, char ** argv){
    //获得命令行参数
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
    
    //初始化日志
    SharedObjectPtr<Appender> append(new ConsoleAppender());
    append->setName(LOG4CPLUS_TEXT("append for worker"));
    auto_ptr<Layout> layout(new PatternLayout(LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S} %p %l:%m %n")));
    append->setLayout(layout);
    Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("worker"));
    logger.addAppender(append);
    if(debug) {
        logger.setLogLevel(log4cplus::DEBUG_LOG_LEVEL);
    } else {
        logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
    }
    
    //创建一些目录
    if(access("/var/lib/lynn", F_OK) == -1) {
       if(mkdir("/var/lib/lynn",0755) != 0){
           LOG4CPLUS_ERROR(logger, "cannot create /var/lib/lynn\n");
           exit(1);
       }
    }

    if(access("/var/lib/lynn/iso", F_OK) == -1) {
       if(mkdir("/var/lib/lynn/iso",0755) != 0){
           LOG4CPLUS_ERROR(logger, "cannot create /var/lib/lynn/iso\n");
           exit(1);
       }
    }

    if(access("/var/lib/lynn/img", F_OK) == -1) {
       if(mkdir("/var/lib/lynn/img",0755) != 0){
           LOG4CPLUS_ERROR(logger, "cannot create /var/lib/lynn/img\n");
           exit(1);
       }
    }
 
    char *p = getenv("LYNN_HOME");

    if( NULL == p) {
        LOG4CPLUS_ERROR(logger, "LYNN_HOME is not set");
        exit(1);
    }

    string zlynn_home(p);
    if(WorkerConfigI::Instance()->Init(zlynn_home + "/conf/worker.xml") != 0) {
        LOG4CPLUS_ERROR(logger, "error in reading worker config");
        exit(1);
    }

    if(ResourceManagerI::Instance()->Init() != 0) {
        LOG4CPLUS_ERROR(logger, "cannot initialize resource manager");
        exit(1);
    }

    thread heartbeat_processor_t(HeartbeatProcessor);
    thread scheduler_processor_t(SchedulerProcessor);
    thread state_event_processor_t(StateEventProcessor);
 
    Rpc<WorkerService, WorkerProcessor>::Listen(atoi(WorkerConfigI::Instance()->Get("port").c_str()));
}
