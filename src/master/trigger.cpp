#include "master/trigger.h"
#include "master/machine_pool.h"

void Trigger::Action(const ExecutorPoolPtr& epp) {
   if(Condition(epp)) 
       Operation(epp);
}

bool IdleTrigger::Condition(const ExecutorPoolPtr& epp) {
    bool ret = epp->IsIdle(static_cast<int32_t>(GetPeriod()*60), 
                           static_cast<double>(GetValue())/100, m_id);
    SetTriggerState(ret);
    return ret;
}

bool IdleTrigger::Operation(const ExecutorPoolPtr& epp) {
    return true;
}

bool OverloadTrigger::Condition(const ExecutorPoolPtr& epp) {
    if(!IsTriggered()) {
        if(epp->IsOverLoad(static_cast<int32_t>(GetPeriod()*60), 
                          static_cast<double>(GetValue())/100)) {
            SetTriggerState(true);
            m_trigger_time = time(0);
            return true;
        } else {
            return false;
        }
    } else {
        if(time(0) - m_trigger_time > 30 && epp->Size() < MachinePoolI::Instance()->Size()) {
            SetTriggerState(false);
        }
        return false; 
    } 
}

bool OverloadTrigger::Operation(const ExecutorPoolPtr& epp) {
    return true;
}
