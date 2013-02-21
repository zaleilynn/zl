#ifndef _LYNN_MASTER_MACHINE_POOL_H
#define _LYNN_MASTER_MACHINE_POOL_H

#include <map>
#include <tr1/functional>
#include <classad/classad.h>

#include "include/rwlock.h"
#include "include/singleton.h"
#include "master/machine.h"

using std::map;
using std::list;
using std::tr1::function;
using lynn::RWLock;

class MachinePool {
public: 
    typedef function<void(const MachinePtr&)> MachineFunc;
    void Insert(const MachinePtr& machine_ptr);
    void MapToDo(MachineFunc func);
    string AssignTask(const ClassAd& ad);
private:
    static bool Compare(const MachinePtr& left, const MachinePtr& right);
    RWLock m_lock;
    map<string, MachinePtr> m_machine_pool;
};

typedef Singleton<MachinePool> MachinePoolI;
#endif
