#ifndef _LYNN_WORKER_KVM_H_
#define _LYNN_WORKER_KVM_H_

#include <stdint.h>
#include "worker/vm.h"

class KVM public : VM {
public:
    int32_t Init();
    int32_t Execute();
};

#endif
