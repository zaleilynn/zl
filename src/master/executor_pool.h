#ifndef _LYNN_MASTER_EXECUTOR_POOL_H_
#define _LYNN_MASTER_EXECUTOR_POOL_H_

#include <map>
#include <string>
#include "include/rwlock.h"
#include "master/executor.h"

using std::map;
using std::string;
using lynn::RWLock;

class ExecutorPool {
public:
    explicit ExecutorPool(const string& name) : m_vc_name(name) {}
    bool Find(int64_t id);
    void Insert(const ExecutorPtr& executor);
private:
    string m_vc_name;
    RWLock m_lock;
    map<int64_t, ExecutorPtr> m_map;
};

typedef shared_ptr<ExecutorPool> ExecutorPoolPtr;
#endif

