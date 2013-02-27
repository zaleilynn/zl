#ifndef _LYNN_MASTER_POOL_H_
#define _LYNN_MASTER_POOL_H_

#include <map>
#include <tr1/functional>

#include "include/rwlock.h"
#include "include/singleton.h"

#include "master/virtual_cluster.h"

using std::map;
using std::tr1::function;
using std::tr1::placeholders::_1;

using lynn::RWLock;

class VCPool {
public:
    typedef function<void(const VCPtr&)> VCFunc; 
    int32_t AddVC(const VCPtr& vc);
    void Insert(const VCPtr& vc);
    //其实也可以把指针给开放出去，但是一定要注意安全
    TaskPtr GetTask();
    void AddTask(const TaskPtr& task);
    void RemoveTask(const TaskPtr& task);
    bool FindToDo(const string& name, VCFunc func);
private:
    RWLock m_lock;
    map<string,VCPtr> m_vc_pool;
};

typedef Singleton<VCPool> VCPoolI;
#endif
