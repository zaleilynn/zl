#ifndef _LYNN_MASTER_EVENT_H_
#define _LYNN_MASTER_EVENT_H_

#include <boost/shared_ptr.hpp>
#include "include/event.h"
#include "include/singleton.h"
#include "include/block_queue.h"

using boost::shared_ptr;
using lynn::BlockQueue;

class StartEvent : public StateEvent {
public:
    StartEvent(int64_t id) : StateEvent(id) {}
    int32_t Handle();
};

class FinishEvent : public StateEvent {
public:
    FinishEvent(int64_t id) : StateEvent(id) {}
    int32_t Handle();
};

class FailEvent : public StateEvent {
public:
    FailEvent(int64_t id) : StateEvent(id) {}
    int32_t Handle();
};

typedef shared_ptr<StateEvent> StateEventPtr;
typedef Singleton< BlockQueue<StateEventPtr> > StateEventBufferI;
#endif
