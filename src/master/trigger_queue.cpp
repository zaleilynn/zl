#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "master/trigger_queue.h"

using lynn::ReadLocker;
using lynn::WriteLocker;
using log4cplus::Logger;

static Logger logger = Logger::getInstance("master");


void TriggerQueue::PushBack(const TriggerPtr& trigger) {
    WriteLocker locker(m_lock);
    m_list.push_back(trigger);
}

void TriggerQueue::Clear() {
    WriteLocker locker(m_lock);
    m_list.clear(); 
}

void TriggerQueue::Map(const ExecutorPoolPtr& ptr) {
    ReadLocker locker(m_lock);
    for(list<TriggerPtr>::iterator it = m_list.begin();
        it != m_list.end(); ++it) {
            (*it)->Action(ptr);
    }
}

void TriggerQueue::Flush(const ExecutorPoolPtr& ptr) {
    WriteLocker locker(m_lock);
    for(list<TriggerPtr>::iterator it = m_list.begin();
        it != m_list.end();) {
        //去除触发了的Idle触发器
        //overload触发器没有管它
        if((*it)->IsTriggered() && (*it)->GetName() == "Idle") {
            ptr->Delete((*it)->GetId());
            it = m_list.erase(it);
        } else {
            ++it;
        }
    }
}
