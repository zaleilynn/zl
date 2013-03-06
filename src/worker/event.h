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

class StartedEvent : public Event {
public:
    StartedEvent(int64_t task_id) : Event(task_id) {};
    int32_t Handle();
};

class FailedEvent : public Event {
public:
    FailedEvent(int64_t task_id, const string& reason) : Event(task_id) {
        m_reason = reason;
    };
    int32_t Handle();
private:
    string m_reason;
};

class FinishedEvent : public Event {
public:
    FinishedEvent(int64_t task_id) : Event(task_id) {};
    int32_t Handle();
};

typedef shared_ptr<Event> EventPtr;
typedef Singleton< BlockQueue<EventPtr> > EventBufferI;
#endif
