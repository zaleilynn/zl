#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "master/trigger.h"
#include "master/machine_pool.h"
#include "master/vc_pool.h"
#include "master/event.h"

using log4cplus::Logger;
using std::tr1::bind;

static Logger logger = Logger::getInstance("master");

void Trigger::Action(const ExecutorPoolPtr& epp) {
   if(Condition(epp)) 
       Operation(epp);
}

bool IdleTrigger::Condition(const ExecutorPoolPtr& epp) {
    bool ret = epp->IsIdle(static_cast<int32_t>(GetPeriod()*60), 
                           static_cast<double>(GetValue())/100, m_id);
    SetTriggerState(ret);
    if(ret) {
        LOG4CPLUS_INFO(logger, "overload triggered");
    }
    return ret;
}

bool IdleTrigger::Operation(const ExecutorPoolPtr& epp) {
    LOG4CPLUS_INFO(logger, "Idle trigger triggeed:");
    //只有一个Executor再闲也不能删除
    if(epp->Size() > 1) {
        EventPtr event(new RemoveEvent(m_id));
        EventBufferI::Instance()->PushBack(event);
        return true;
    } else {
        LOG4CPLUS_WARN(logger, "can't kill the last executor of vc:" << epp->GetName());
        //特殊情况可以重新触发
        SetTriggerState(false);
        return false;
    }
}

bool OverloadTrigger::Condition(const ExecutorPoolPtr& epp) {
    if(!IsTriggered()) {
        if(epp->IsOverLoad(static_cast<int32_t>(GetPeriod()*60), 
                          static_cast<double>(GetValue())/100)) {
            SetTriggerState(true);
            m_trigger_time = time(0);
            LOG4CPLUS_INFO(logger, "overload triggered");
            return true;
        } else {
            return false;
        }
    } else {
        //30秒之后才能再触发
        if(time(0) - m_trigger_time > 30){
            LOG4CPLUS_DEBUG(logger, "reset trigger state");
            SetTriggerState(false);
        }
        return false; 
    } 
}

bool OverloadTrigger::Operation(const ExecutorPoolPtr& epp) {
    LOG4CPLUS_INFO(logger, "virtual cluster " << epp->GetName()
                            << " is overloaded. Add executor for it.");
    VCPool::VCFunc func = bind(&VC::AddTask, _1);
    return VCPoolI::Instance()->FindToDo(epp->GetName(), func);
}
