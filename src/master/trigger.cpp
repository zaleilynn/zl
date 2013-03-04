#include "master/trigger.h"

void Trigger::Action(const ExecutorPoolPtr& epp) {
   if(Condition(epp)) 
       Operation(epp);
}

bool IdleTrigger::Condition(const ExecutorPoolPtr& epp) {
    return true;
}

bool IdleTrigger::Action(const ExecutorPoolPtr& epp) {
    return true;
}

bool CpuTrigger::Condition(const ExecutorPoolPtr& epp) {
    return true;
}

bool CpuTrigger::Action(const ExecutorPoolPtr& epp) {
    return true;
}
