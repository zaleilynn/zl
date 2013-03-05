#ifndef _LYNN_MASTER_TRIGGER_QUEUE_H_
#define _LYNN_MASTER_TRIGGER_QUEUE_H_

#include <list>
#include "include/rwlock.h"
#include "master/trigger.h"
#include "master/executor_pool.h"

using lynn::RWLock;
using std::list;

class TriggerQueue {
public:
    void PushBack(const TriggerPtr& trigger);
    void Clear();
    void Map(const ExecutorPoolPtr& ptr);
    void Flush(const ExecutorPoolPtr& ptr);
private:
    RWLock m_lock; 
    list<TriggerPtr> m_list;
};
#endif
