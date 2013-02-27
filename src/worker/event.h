#ifndef _LYNN_WORKER_EVENT_H_
#define _LYNN_WORKER_EVENT_H_

#include <stdint.h>
#include <string>

#include <boost/shared_ptr.hpp>
#include "include/singleton.h"
#include "include/block_queue.h"
#include "include/event.h"

using boost::shared_ptr;
using std::string;
using lynn::BlockQueue;

class StartedEvent : public StateEvent {
public:
    StartedEvent(int64_t task_id) : StateEvent(task_id) {};
    int32_t Handle();
};

class FailedEvent : public StateEvent {
public:
    FailedEvent(int64_t task_id, const string& reason) : StateEvent(task_id) {
        m_reason = reason;
    };
    int32_t Handle();
private:
    string m_reason;
};

class FinishedEvent : public StateEvent {
public:
    FinishedEvent(int64_t task_id) : StateEvent(task_id) {};
    int32_t Handle();
};

typedef shared_ptr<StateEvent> StateEventPtr;
typedef Singleton< BlockQueue<StateEventPtr> > StateEventBufferI;
#endif
