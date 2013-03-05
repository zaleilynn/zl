#include "master/watcher.h"

void Watcher::Start() {
    Thread::ThreadFunc func = bind(&Watcher::Entry, this);
    m_watcher_thread = new Thread(func);
    m_watcher_thread->Start();
}

void Watcher::Entry() {
    while(true) {
        MachinePtr machine; 
        m_queue.PopFront(&machine);
        machine->ForwardEvent(); 
    }
}

void Watcher::AddEvent(const MachinePtr& machine) {
    m_queue.PushBack(machine);
}
