#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "master/service.h"
#include "master/machine_pool.h"
#include "master/vc_pool.h"
#include "master/event.h"
#include "master/watcher.h"

using log4cplus::Logger;

static Logger logger = Logger::getInstance("master");

void MasterService::Heartbeat(const MachineInfo& info) {
    //  物理机器的资源利用情况是一直在变的，实时更新的
    //  master也没有维护物理的剩余资源,只是接受物理机器的上报
    //  这里不能做过多的操作，一定要快的返回，否则客户机一多，这里RPC受不了
    MachinePtr ptr(new Machine(info));
    //  ptr->LogInfo();
    MachinePoolI::Instance()->Insert(ptr);
    // 下面的功能如果做到MachinePool::Instance()->Insert里面去了
    // 那么就相对不独立了
    WatcherI::Instance()->AddEvent(ptr);
    // 也是否可以不转发指针，直接实现WatcherI::Instance()->AddEvent(info)?
}

//Add了一个virtual cluster，同时初始化了一个task
//这里需要不需要弄一个缓冲区?
int32_t MasterService::AddVC(const VCInfo& vc_info){
    VCPtr ptr(new VC(vc_info));
    ptr->LogInfo(); 
    return VCPoolI::Instance()->AddVC(ptr); 
}

int32_t MasterService::TaskStarted(int64_t task_id){
    EventPtr event(new StartEvent(task_id));
    EventBufferI::Instance()->PushBack(event);
    return 0;
}

int32_t MasterService::TaskFinished(int64_t task_id){
    EventPtr event(new FinishEvent(task_id));
    EventBufferI::Instance()->PushBack(event);
    return 0;
}

int32_t MasterService::TaskFailed(int64_t task_id){
    EventPtr event(new FailEvent(task_id));
    EventBufferI::Instance()->PushBack(event);
    return 0;
}
