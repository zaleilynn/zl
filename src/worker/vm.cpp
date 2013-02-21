#include "worker/vm.h"

VM::VM(const TaskInfo& info){
    m_info = info;
}

int32_t VM::Init() {
    return 0;
}
