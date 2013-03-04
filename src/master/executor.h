#ifndef _LYNN_MASTER_EXECUTOR_H_
#define _LYNN_MASTER_EXECUTOR_H_

#include <boost/shared_ptr.hpp>
#include <stdint.h>
#include <list>

#include "include/proxy.h"
#include "include/rwlock.h"

using boost::shared_ptr;
using lynn::RWLock;
using std::list;

class Executor {
public:
    Executor(int64_t id) : m_id(id) {}
    int64_t GetId(){
        return m_id;
    }
    void AddStatInfo(const ExecutorStat& stat);
    bool Condition();
    void Action();
private:  
    int64_t m_id;
    RWLock m_lock;
    list<ExecutorStat> m_stat_list;
};

typedef shared_ptr<Executor> ExecutorPtr;
#endif
