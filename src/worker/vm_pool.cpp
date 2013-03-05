#include "worker/vm_pool.h"

using lynn::ReadLocker;
using lynn::WriteLocker;

void VMPool::Insert(const VMPtr& ptr) {
    WriteLocker locker(m_lock);
    m_vm_pool[ptr->GetId()] = ptr;
}

void VMPool::Delete(const string& id) {
}

void VMPool::MapToDo(VMFunc func) {
    ReadLocker locker(m_lock);
    for(map<string, VMPtr>::iterator it = m_vm_pool.begin();
        it != m_vm_pool.end(); ++it) {
        func(it->second);
    }
}
