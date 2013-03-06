#include "worker/vm_pool.h"

using lynn::ReadLocker;
using lynn::WriteLocker;

void VMPool::Insert(const VMPtr& ptr) {
    WriteLocker locker(m_lock);
    m_vm_pool[ptr->GetId()] = ptr;
}

void VMPool::Delete(const string& id) {
    WriteLocker locker(m_lock);
    m_vm_pool.erase(id);
}

void VMPool::MapToDo(VMFunc func) {
    ReadLocker locker(m_lock);
    for(map<string, VMPtr>::iterator it = m_vm_pool.begin();
        it != m_vm_pool.end(); ++it) {
        func(it->second);
    }
}

bool VMPool::DeleteByTaskId(int64_t task_id) {
    WriteLocker locker(m_lock);
    for(map<string, VMPtr>::iterator it = m_vm_pool.begin();
        it != m_vm_pool.end(); ++it) { 
            if(atoi(it->second->GetId().c_str()) == task_id) { 
                if(it->second->Recycle() == 0) {
                    m_vm_pool.erase(it);
                    return true;
                } else {
                    return false;
                }
            }
    }
    return false;
}
