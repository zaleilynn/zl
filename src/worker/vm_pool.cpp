#include "worker/vm_pool.h"

using lynn::ReadLocker;
using lynn::WriteLocker;

void VMPool::Insert(const VMPtr& ptr) {
    WriteLocker locker(m_lock);
    m_vm_pool[ptr->GetId()] = ptr;
}

void VMPool::Delete(const string& id) {
}
