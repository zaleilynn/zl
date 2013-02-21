#ifndef _LYNN_WORKER_VM_H_
#define _LYNN_WORKER_VM_H_

#include <boost/shared_ptr.hpp>
#include "include/proxy.h"

using boost::shared_ptr;

class VM {
public:
    VM(const TaskInfo& info);
    int32_t Init();
private:
    TaskInfo m_info;
};

typedef shared_ptr<VM> VMPtr;
#endif
