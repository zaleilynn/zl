#include "master/executor.h"

using lynn::WriteLocker;
using lynn::ReadLocker;

void Executor::AddStatInfo(const ExecutorStat& stat){
    WriteLocker locker(m_lock);
    m_stat_list.push_back(stat);
}

bool Executor::Condition(){
    return true;
}

void Executor::Action() {
}
