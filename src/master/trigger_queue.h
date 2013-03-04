#ifndef _LYNN_MASTER_TRIGGER_QUEUE_H_
#define _LYNN_MASTER_TRIGGER_QUEUE_H_

#include <list>
#include "master/trigger.h"
#include "include/rwlock.h"

using lynn::RWLock;
using std::list;

class TriggerQueue {
public:
    void PushBack(const TriggerPtr& trigger);
    void Clear();
    void Map();
    void Flush();
private:
    RWLock m_lock; 
    list<TriggerPtr> m_list;
};
#endif
