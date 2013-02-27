#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "master/service.h"
#include "master/machine_pool.h"
#include "master/vc_pool.h"
#include "master/event.h"

using log4cplus::Logger;

static Logger logger = Logger::getInstance("master");

void MasterService::Heartbeat(const MachineInfo& info) {
    //  物理机器的资源利用情况是一直在变的，实时更新的
    //  master也没有维护物理的剩余资源,只是接受物理机器的上报
    MachinePtr ptr(new Machine(info));
    //  ptr->LogInfo();
    MachinePoolI::Instance()->Insert(ptr);
}

//Add了一个virtual cluster，同时初始化了一个task
int32_t MasterService::AddVC(const VCInfo& vc_info){
    VCPtr ptr(new VC(vc_info));
    ptr->LogInfo();
    return VCPoolI::Instance()->AddVC(ptr); 
}

int32_t MasterService::TaskStarted(int64_t task_id){
    StateEventPtr event(new StartEvent(task_id));
    StateEventBufferI::Instance()->PushBack(event);
    return 0;
}

int32_t MasterService::TaskFinished(int64_t task_id){
    StateEventPtr event(new FinishEvent(task_id));
    StateEventBufferI::Instance()->PushBack(event);
    return 0;
}

int32_t MasterService::TaskFailed(int64_t task_id){
    StateEventPtr event(new FailEvent(task_id));
    StateEventBufferI::Instance()->PushBack(event);
    return 0;
}
