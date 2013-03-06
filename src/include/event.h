#ifndef _LYNN_INCLUDE_EVENT_H_
#define _LYNN_INCLUDE_EVENT_H_

#include <stdint.h>
#include <string>

using std::string;

class Event {
public:
    Event(int64_t task_id) {
        m_task_id = task_id;
    }
    virtual int32_t Handle() = 0;
    int64_t GetId() {
        return m_task_id;
    }
protected:
    int64_t m_task_id;
};

#endif
