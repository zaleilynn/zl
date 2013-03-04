#include "master/trigger_queue.h"

using lynn::ReadLocker;
using lynn::WriteLocker;

void TriggerQueue::PushBack(const TriggerPtr& trigger) {
    WriteLocker locker(m_lock);
    m_list.push_back(trigger);
}

void TriggerQueue::Clear() {
    WriteLocker locker(m_lock);
    m_list.clear(); 
}

void TriggerQueue::Map() {
    ReadLocker locker(m_lock);
    for(list<TriggerPtr>::iterator it = m_list.begin();
        it != m_list.end(); ++it) {
        if((*it)->Condition()) {
            (*it)->Action();
        }
    }
}

void TriggerQueue::Flush() {
}
