#ifndef _LYNN_WORKER_VM_POOL_H_
#define _LYNN_WORKER_VM_POOL_H_

#include <map>
#include <tr1/memory>
#include <tr1/functional>
#include "include/singleton.h"
#include "include/rwlock.h"
#include "worker/vm.h"

using std::map;
using lynn::RWLock;
using std::tr1::function;
using std::tr1::placeholders::_1;

class VMPool {
public:
    typedef function<void(VMPtr)> VMFunc;
    void Insert(const VMPtr& ptr);
    void Delete(const string& vm_id);
    void MapToDo(VMFunc func);
    bool DeleteByTaskId(int64_t task_id);
private:
    RWLock m_lock;
    map<string, VMPtr> m_vm_pool;
};

typedef Singleton<VMPool> VMPoolI;
#endif 
