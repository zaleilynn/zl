#ifndef _LYNN_WORKER_VM_POOL_H_
#define _LYNN_WORKER_VM_POOL_H_

#include <map>
#include "include/singleton.h"
#include "include/rwlock.h"
#include "worker/vm.h"

using std::map;
using lynn::RWLock;

class VMPool {
public:
    typedef function<void(VMPtr)> VMFunc;
    void Insert(const VMPtr& ptr);
    void Delete(const string& vm_id);
private:
    RWLock m_lock;
    map<string, VMPtr> m_vm_pool;
};

typedef Singleton<VMPool> VMPoolI;
#endif 