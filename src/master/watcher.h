#ifndef _LYNN_MASTER_MONITOR_H_
#define _LYNN_MASTER_MONITOR_H_

#include <tr1/memory>
#include <tr1/functional>
#include <map>

#include "include/singleton.h"
#include "include/block_queue.h"
#include "include/thread.h"

#include "master/machine.h"
#include "master/vc_pool.h"

using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;
using std::tr1::bind;

using lynn::BlockQueue;
using lynn::Thread;

class Watcher {
public:
    Watcher() : m_watcher_thread(NULL) {}
    void Start();
    void Entry();
    void AddEvent(const MachinePtr& machine);
private:
    BlockQueue<MachinePtr> m_queue;
    lynn::Thread* m_watcher_thread; 
};

//奇了怪了，如果用名字Monitor，这样就编译不通过，报模版第一个参数无效
typedef Singleton<Watcher> WatcherI;
#endif
