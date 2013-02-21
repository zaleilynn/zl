#ifndef _LYNN_INCLUDE_IDENTITY_H_
#define _LYNN_INCLUDE_IDENTITY_H_

#include "include/mutex.h"
#include "include/singleton.h"

using lynn::Mutex;

class Identity {
public:
    explicit Identity(int32_t num) : m_id(num) {}

    Identity() : m_id(0) {}

    int64_t GetTaskId() {
        MutexLocker locker(m_lock); 
        return ++m_id;
    }

private:
    Mutex m_lock;
    int64_t m_id;
};

#endif
