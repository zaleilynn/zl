#ifndef _LYNN_MASTER_EVENT_H_
#define _LYNN_MASTER_EVENT_H_

#include <boost/shared_ptr.hpp>
#include "include/event.h"
#include "include/singleton.h"
#include "include/block_queue.h"

using boost::shared_ptr;
using lynn::BlockQueue;

class StartEvent : public Event {
public:
    StartEvent(int64_t id) : Event(id) {}
    int32_t Handle();
};

class FinishEvent : public Event {
public:
    FinishEvent(int64_t id) : Event(id) {}
    int32_t Handle();
};

class FailEvent : public Event {
public:
    FailEvent(int64_t id) : Event(id) {}
    int32_t Handle();
};

class RemoveEvent: public Event {
public:
    RemoveEvent(int64_t id) : Event(id) {}
    int32_t Handle();
};

typedef shared_ptr<Event> EventPtr;
typedef Singleton< BlockQueue<EventPtr> > EventBufferI;
#endif
